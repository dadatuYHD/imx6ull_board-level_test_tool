#include <string.h>
#include <stdio.h>

#include <disp_manager.h>
#include <framebuffer.h>

static PDispDev_S g_pDispDevsHead = NULL;
static PDispDev_S g_pDefaultDispDev = NULL;
static DispBuffer_S g_dispBuffer;
static unsigned int g_lineWidth;
static unsigned int g_pixelWidth;

void drawTextInRegionCentral(char * pName, PDispRegion_S pRegion, unsigned int dwColor)
{
	FontBitMap_S fontBitMap;
	DispRegionCartesian_S dispRegionCartesian;

	int originX, originY;
	int i = 0;
	int error;

	/* 计算字符串的外框 */
    getStrBoxRegionCar(pName, &dispRegionCartesian);

    /* 算出第一个字符的origin */
	originX = pRegion->xLeftUp + (pRegion->width  - dispRegionCartesian.width)/2  - dispRegionCartesian.xLeftUp;
	originY = pRegion->yLeftUp + (pRegion->height - dispRegionCartesian.height)/2 + dispRegionCartesian.yLeftUp;

	while (pName[i])
	{
		/* get bitmap */
		fontBitMap.curOriginX = originX;
		fontBitMap.curOriginY = originY;
		error = getFontBitMap(pName[i], &fontBitMap);
		if (error)
		{
			printf("getFontBitMap error\n");
			return;
		}

		/* draw on buffer */		
		drawFontBitMap(&fontBitMap, dwColor);		

		originX = fontBitMap.nextOriginX;
		originY = fontBitMap.nextOriginY;	
		i++;
	}
	
}


void drawRegion(PDispRegion_S pRegion, unsigned int dwColor)
{
	int x = pRegion->xLeftUp;
	int y = pRegion->yLeftUp;
	int width = pRegion->width;
	int heigh = pRegion->height;
	int i,j;

	for (j = y; j < y + heigh; j++)
	{
		for (i = x; i < x + width; i++)
			putPixel(i, j, dwColor);
	}
}


int putPixel(int x, int y, unsigned int dwColor)
{   
    unsigned char*  pen_8 = (unsigned char *)(g_dispBuffer.pBuffer + y * g_lineWidth + x * g_pixelWidth);
	unsigned short* pen_16;	
	unsigned int*   pen_32;	

	unsigned int red, green, blue;	

	pen_16 = (unsigned short *)pen_8;
	pen_32 = (unsigned int *)pen_8;

	switch (g_dispBuffer.bpp)
	{
		case 8:
		{
			*pen_8 = dwColor;
			break;
		}
		case 16:
		{
			/* 565 */
			red   = (dwColor >> 16) & 0xff;
			green = (dwColor >> 8) & 0xff;
			blue  = (dwColor >> 0) & 0xff;
			dwColor = ((red >> 3) << 11) | ((green >> 2) << 5) | (blue >> 3);
			*pen_16 = dwColor;
			break;
		}
		case 32:
		{
			*pen_32 = dwColor;
			break;
		}
		default:
		{
			printf("can't surport %dbpp\n", g_dispBuffer.bpp);
			return -1;
			break;
		}
	}

	return 0;
}

void drawFontBitMap(PFontBitMap_S pFontBitMap, unsigned int dwColor)
{
    int i, j, p, q;
	int x = pFontBitMap->region.xLeftUp;
	int y = pFontBitMap->region.yLeftUp;
    int x_max = x + pFontBitMap->region.width;
    int y_max = y + pFontBitMap->region.height;
	int bitMapWidth = pFontBitMap->region.width;
	unsigned char * pBuffer = pFontBitMap->pBuffer;


    for (j = y, q = 0; j < y_max; j++, q++)
    {
        for ( i = x, p = 0; i < x_max; i++, p++ )
        {
            if ( i < 0      || j < 0       ||
                i >= g_dispBuffer.xRes || j >= g_dispBuffer.yRes )
            continue;

            if (pBuffer[q * bitMapWidth + p])
	            putPixel(i, j, dwColor);
        }
    }    
}



void dispDevRegister(PDispDev_S pDispDev)
{
	pDispDev->pNext = g_pDispDevsHead;
	g_pDispDevsHead = pDispDev;
}

void dispSystemRegister(void)
{
    fbDevRegister();    
}

int selectDefaultDispDev(char* pName)
{
    PDispDev_S pTmp = g_pDispDevsHead;
	int ret = -1;

	while (pTmp)
	{
        if (!strcmp(pTmp->pName, pName))
        {
            g_pDefaultDispDev = pTmp;
			ret = 0;
			break;
		}

		pTmp = pTmp->pNext;
	}

	return ret;
}

int defaultDispDevInit(void)
{
    int ret = -1;

	ret = g_pDefaultDispDev->dispDevInit();
	if (-1 == ret)
	{
        printf("g_pDefaultDispDev->dispDevInit() is fail!\n");
		return -1;
	}

	ret = g_pDefaultDispDev->getDispBuffer(&g_dispBuffer);
	if (-1 == ret)
	{
        printf("g_pDefaultDispDev->getDispBuffer() is fail!\n");
		return -1;
	}

	g_lineWidth  = g_dispBuffer.xRes * g_dispBuffer.bpp / 8;
	g_pixelWidth = g_dispBuffer.bpp / 8;

	return ret;
}

void dispDefaultDev(int x, int y, unsigned char c)
{
    g_pDefaultDispDev->dispDev(x, y, c);    
}

int flushDispRegion(PDispRegion_S pDispRegion, PDispBuffer_S pDispBuffer)
{
    int ret = g_pDefaultDispDev->flushDispRegion(pDispRegion, pDispBuffer);  

	return ret;
}

PDispBuffer_S getDispBuffer(void)
{
    return &g_dispBuffer; 
}

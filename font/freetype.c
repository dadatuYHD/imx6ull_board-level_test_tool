#include <unistd.h>
#include <linux/fb.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <wchar.h>
#include <sys/ioctl.h>
#include <font_manager.h>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H


#define DEFAULT_FONT_SIZE 12 

static FT_Face g_face;


static int freetype_fontInit(char * pFontFileName)
{
    FT_Library library;
    int error = 0;

    error = FT_Init_FreeType(&library);                      /* initialize library */    
	if (error)
	{
		printf("FT_Init_FreeType err\n");
		return -1;
	}
	
    error = FT_New_Face(library, pFontFileName, 0, &g_face); /* create face object */
	if (error)
	{
		printf("FT_New_Face err\n");
		return -1;
	}

    FT_Set_Pixel_Sizes(g_face, DEFAULT_FONT_SIZE, 0);

	return 0;
}

static int freetype_setFontSize(int fontSize)
{
    FT_Set_Pixel_Sizes(g_face, fontSize, 0);

	return 0;
}

static int freetype_getFontBitMap(unsigned int fontCode, PFontBitMap_S pFontBitMap)
{
    int error;
    FT_Vector pen;
    FT_GlyphSlot slot = g_face->glyph;

    pen.x = pFontBitMap->curOriginX * 64; /* 单位: 1/64像素 */
    pen.y = pFontBitMap->curOriginY * 64; /* 单位: 1/64像素 */

	/* 转换：transformation */
	FT_Set_Transform(g_face, 0, &pen);

	/* 加载位图: load glyph image into the slot (erase previous one) */
	error = FT_Load_Char(g_face, fontCode, FT_LOAD_RENDER);
	if (error)
	{
		printf("FT_Load_Char error\n");
		return -1;
	}

	pFontBitMap->pBuffer        = slot->bitmap.buffer;
	pFontBitMap->region.xLeftUp = slot->bitmap_left;
	pFontBitMap->region.yLeftUp = pFontBitMap->curOriginY*2 - slot->bitmap_top;
	pFontBitMap->region.width   = slot->bitmap.width;
	pFontBitMap->region.height  = slot->bitmap.rows;
	pFontBitMap->nextOriginX = pFontBitMap->curOriginX + slot->advance.x / 64;
	pFontBitMap->nextOriginY = pFontBitMap->curOriginY;

	return 0;
}
int freetype_getStrBoxRegionCar(char * pStr, PDispRegionCartesian_S pDispRegionCartesian)
{
    int i;
    int error;
    FT_BBox bbox;
    FT_BBox glyph_bbox;
    FT_Vector pen;
    FT_Glyph  glyph;
    FT_GlyphSlot slot = g_face->glyph;

    /* 初始化 */
    bbox.xMin = bbox.yMin = 32000;
    bbox.xMax = bbox.yMax = -32000;

    /* 指定原点为(0, 0) */
    pen.x = 0;
    pen.y = 0;

    /* 计算每个字符的bounding box */
    /* 先translate, 再load char, 就可以得到它的外框了 */
    for (i = 0; i < strlen(pStr); i++)
    {
        /* 转换：transformation */
        FT_Set_Transform(g_face, 0, &pen);

        /* 加载位图: load glyph image into the slot (erase previous one) */
        error = FT_Load_Char(g_face, pStr[i], FT_LOAD_RENDER);
        if (error)
        {
            printf("FT_Load_Char error\n");
            return -1;
        }

        /* 取出glyph */
        error = FT_Get_Glyph(g_face->glyph, &glyph);
        if (error)
        {
            printf("FT_Get_Glyph error!\n");
            return -1;
        }
        
        /* 从glyph得到外框: bbox */
        FT_Glyph_Get_CBox(glyph, FT_GLYPH_BBOX_TRUNCATE, &glyph_bbox);

        /* 更新外框 */
        if ( glyph_bbox.xMin < bbox.xMin )
            bbox.xMin = glyph_bbox.xMin;

        if ( glyph_bbox.yMin < bbox.yMin )
            bbox.yMin = glyph_bbox.yMin;

        if ( glyph_bbox.xMax > bbox.xMax )
            bbox.xMax = glyph_bbox.xMax;

        if ( glyph_bbox.yMax > bbox.yMax )
            bbox.yMax = glyph_bbox.yMax;
        
        /* 计算下一个字符的原点: increment pen position */
        pen.x += slot->advance.x;
        pen.y += slot->advance.y;
    }

    /* return string bbox */
    //*abbox = bbox;
    pDispRegionCartesian->xLeftUp = bbox.xMin;
    pDispRegionCartesian->yLeftUp = bbox.yMax;
    pDispRegionCartesian->width   = bbox.xMax - bbox.xMin + 1;
    pDispRegionCartesian->height  = bbox.yMax - bbox.yMin + 1;

	return 0;	
}

static FontLib_S g_fontLib = {
    .pName              = "freetype",
	.fontInit           = freetype_fontInit,
	.setFontSize        = freetype_setFontSize,
	.getFontBitMap      = freetype_getFontBitMap, 
	.getStrBoxRegionCar = freetype_getStrBoxRegionCar,
};

void freetypeFontLibRegister(void)
{
	fontlibRegister(&g_fontLib);
}


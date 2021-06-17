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


static int freetypeFontInit(char * pFontFileName)
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

static int freetypeSetFontSize(int fontSize)
{
    FT_Set_Pixel_Sizes(g_face, fontSize, 0);

	return 0;
}

static int freetypeGetFontBitMap(unsigned int fontCode, PFontBitMap_S pFontBitMap)
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

static FontLib_S g_fontLib = {
    .name          = "freetype",
	.fontInit      = freetypeFontInit,
	.setFontSize   = freetypeSetFontSize,
	.getFontBitMap = freetypeGetFontBitMap, 
};

void freetypeFontLibRegister(void)
{
	registerFontLib(&g_fontLib);
}


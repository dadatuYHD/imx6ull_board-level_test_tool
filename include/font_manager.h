#ifndef ___FONT_MANAGER__
#define ___FONT_MANAGER__

#include <common.h>

typedef struct FontBitMap
{
    DispRegion_S region;
	int curOriginX;
	int curOriginY;
	int nextOriginX;
	int nextOriginY;
	unsigned char *pBuffer;
}FontBitMap_S, * PFontBitMap_S;


typedef struct FontLib
{
    char * pName;
	int (*fontInit)(char * pFontFileName);
	int (*setFontSize)(int fontSize);
	int (*getFontBitMap)(unsigned int fontCode, PFontBitMap_S pFontBitMap);
	int (*getStrBoxRegionCar)(char * pStr, PDispRegionCartesian_S pDispRegionCartesian);
	struct FontLib *pNext;
}FontLib_S, * PFontLib_S;

//provide with model of font_manager.c to font lib
void fontlibRegister(PFontLib_S pFontLib);

//provide with model of font_manager.c to font_test.c/user
void fontsLibSystemRegister(void);
int selectAndInitFontLib(char * pFontLibName, char * pFontFileName);
int setFontSize(int fontSize);
int getFontBitMap(unsigned int fontCode, PFontBitMap_S pFontBitMap);
int getStrBoxRegionCar(char *pStr, PDispRegionCartesian_S pDispRegionCartesian);



#endif

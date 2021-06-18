#include <string.h>

#include <font_manager.h>
#include <freetype.h>

static PFontLib_S g_pFontLibHead = NULL;
static PFontLib_S g_pDefaultFontLib = NULL;

void registerFontLib(PFontLib_S pFontLib)
{
    pFontLib->pNext = g_pFontLibHead;
	g_pFontLibHead = pFontLib;
}

void fontsLibRegister(void)
{
	freetypeFontLibRegister();
}

int selectAndInitFontLib(char * pFontLibName, char * pFontFileName)
{
	PFontLib_S ptTmp = g_pFontLibHead;
	while (ptTmp)
	{
		if (strcmp(ptTmp->pName, pFontLibName) == 0)
			break;
		ptTmp = ptTmp->pNext;
	}

	if (!ptTmp)
		return -1;

	g_pDefaultFontLib = ptTmp;
	
	return ptTmp->fontInit(pFontFileName);
}

int setFontSize(int fontSize)
{
	return g_pDefaultFontLib->setFontSize(fontSize);
}

int getFontBitMap(unsigned int fontCode, PFontBitMap_S pFontBitMap)
{
	return g_pDefaultFontLib->getFontBitMap(fontCode, pFontBitMap);
}

int getStrBoxRegionCar(char *pStr, PDispRegionCartesian_S pDispRegionCartesian)
{
    return g_pDefaultFontLib->getStrBoxRegionCar(pStr, pDispRegionCartesian);
}


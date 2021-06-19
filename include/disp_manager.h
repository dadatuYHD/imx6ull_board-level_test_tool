#ifndef __DISP_MANAGER_H
#define __DISP_MANAGER_H

#include <common.h>
#include <font_manager.h>

typedef struct DispBuffer
{
    int xRes;
	int yRes;
	int bpp;
	char* pBuffer;
}DispBuffer_S, *PDispBuffer_S;

typedef struct DispDev
{
    char* pName;
	int   (*dispDevInit)(void); 
	int   (*dispDevExit)(void); 
	int   (*getDispBuffer)(PDispBuffer_S pDispBuffer);
	void  (*dispDev)(int x, int y, unsigned char c);
	int   (*flushDispRegion)(PDispRegion_S pDispRegion, PDispBuffer_S pDispBuffer);
	struct DispDev* pNext;
}DispDev_S, *PDispDev_S;

//provide with model of disp_manager.c to display device
int putPixel(int x, int y, unsigned int dwColor);
void dispDevRegister(PDispDev_S pDispDev);

//provide with model of disp_manager.c to disp_test.c/user
void dispSystemRegister(void);
int selectDefaultDispDev(char* pName);
int defaultDispDevInit(void);
void dispDefaultDev(int x, int y, unsigned char c);
int flushDispRegion(PDispRegion_S pDispRegion, PDispBuffer_S pDispBuffer);
PDispBuffer_S getDispBuffer(void);
void drawFontBitMap(PFontBitMap_S pFontBitMap, unsigned int dwColor);
void drawRegion(PDispRegion_S pRegion, unsigned int dwColor);
void drawTextInRegionCentral(char * pName, PDispRegion_S pRegion, unsigned int dwColor);

#endif

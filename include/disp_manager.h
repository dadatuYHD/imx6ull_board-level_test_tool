#ifndef __DISP_MANAGER_H
#define __DISP_MANAGER_H

#ifndef NULL
#define NULL (void *)0
#endif

typedef struct DispRegion
{
    int xLeftUp;
	int yLeftUp;
	int width;
	int height;
}DispRegion_S, *PDispRegion_S;

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
void registerDispDev(PDispDev_S pDispDev);

//provide with model of disp_manager.c to disp_test.c/user
void dispDevRegister(void);
int selectDefaultDispDev(char* pName);
int initDefaultDispDev(void);
void dispDefaultDev(int x, int y, unsigned char c);
int flushDispRegion(PDispRegion_S pDispRegion, PDispBuffer_S pDispBuffer);
PDispBuffer_S getDispBuffer(void);

#endif

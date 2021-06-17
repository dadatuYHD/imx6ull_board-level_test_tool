#ifndef __COMMON_H__
#define __COMMON_H__

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


#endif



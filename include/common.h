#ifndef __COMMON_H__
#define __COMMON_H__

#ifndef NULL
#define NULL (void *)0
#endif

//LCD坐标系
typedef struct DispRegion
{
    int xLeftUp;
	int yLeftUp;
	int width;
	int height;
}DispRegion_S, *PDispRegion_S;

//笛卡尔坐标系
typedef struct DispRegionCartesian {
	int xLeftUp;
	int yLeftUp;
	int width;
	int height;
}DispRegionCartesian_S, *PDispRegionCartesian_S;


#endif



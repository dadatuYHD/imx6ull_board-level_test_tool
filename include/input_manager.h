#ifndef __INPUT_MANAGER__
#define __INPUT_MANAGER__

#include <sys/time.h>
#include <common.h>

#define INPUT_TYPE_SCREENTOUCH 1
#define INPUT_TYPE_NET         2

typedef struct InputEvent
{
    int type;
    int x;
	int y;
	int pressure;
	char str[1024];
	struct timeval time;
}InputEvent_S, * PInputEvent_S;

typedef struct InputDevice
{
    char* name;
	int   (*inputDevInit)(void); 
	int   (*inputDevExit)(void); 
	int   (*getInputEvent)(PInputEvent_S pInputEvent);
	struct InputDevice* pNext;
}InputDevice_S, * PInputDevice_S;

//provide with model of input_manager.c to input device
void inputDevRegister(PInputDevice_S ptInputDev);

//provide with model of input_manager.c to input_test.c/user
void inputSystemRegister(void);
int  inputDeviceInit(void);
int userGetInputEventData(PInputEvent_S pInputEvent);

#endif

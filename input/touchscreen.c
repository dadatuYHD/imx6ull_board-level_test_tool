#include <tslib.h>
#include <stdio.h>

#include <input_manager.h> 
#include <touchscreen.h>

static struct tsdev* g_pTsDev = NULL;

static int touchscreenDevInit(void)
{
	g_pTsDev = ts_setup(NULL, 0);
	if (!g_pTsDev)
	{
		printf("ts_setup err\n");
		return -1;
	}

	return 0;
}

static int touchscreenDevExit(void)
{
    ts_close(g_pTsDev);
	
	return 0;
}

int touchscreenGetInputEvent(PInputEvent_S pInputEvent)
{
    struct ts_sample samp;
	int ret;
	
	ret = ts_read(g_pTsDev, &samp, 1);
	
	if (ret != 1)
	{
        return -1;
	}

	pInputEvent->type     = INPUT_TYPE_SCREENTOUCH;
	pInputEvent->x        = samp.x;
	pInputEvent->y        = samp.y;
	pInputEvent->pressure = samp.pressure;
	pInputEvent->time     = samp.tv;

	return 0;
}


static InputDevice_S g_touchscreenDevice = {
    .name          = "touchscreen",
	.inputDevInit  = touchscreenDevInit, 
	.inputDevExit  = touchscreenDevExit,
	.getInputEvent = touchscreenGetInputEvent, 
};

void touchscreenDevRegister(void)
{
    registerInputDev(&g_touchscreenDevice);    
}


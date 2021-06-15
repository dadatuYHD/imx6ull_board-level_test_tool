#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <semaphore.h>
#include <stdio.h>


#include <input_manager.h>
#include <touchscreen.h>
#include <netinput.h>

static PInputDevice_S g_pInputDevHead = NULL; 

static pthread_mutex_t g_mutex   = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t  g_condVar = PTHREAD_COND_INITIALIZER;


/* start of 实现环形buffer */
#define BUFFER_LEN 20
static int g_read  = 0;
static int g_write = 0;
static InputEvent_S g_inputEvents[BUFFER_LEN];

static int isInputBufferFull(void)
{
	return (g_read == ((g_write + 1) % BUFFER_LEN));
}

static int isInputBufferEmpty(void)
{
	return (g_read == g_write);
}

static void putInputEventToBuffer(PInputEvent_S pInputEvent)
{
	if (!isInputBufferFull())
	{
		g_inputEvents[g_write] = *pInputEvent;
		g_write = (g_write + 1) % BUFFER_LEN;
	}
}


static int getInputEventFromBuffer(PInputEvent_S pInputEvent)
{
	if (!isInputBufferEmpty())
	{
		*pInputEvent = g_inputEvents[g_read];
		g_read = (g_read + 1) % BUFFER_LEN;
		return 0;
	}
	else
	{
		return -1;
	}
}
/* end of 实现环形buffer */


void registerInputDev(PInputDevice_S ptInputDev)
{
    ptInputDev->pNext = g_pInputDevHead;
	g_pInputDevHead = ptInputDev;
}

void inputDevRegister(void)
{
    touchscreenDevRegister();

	netInputDevRegister();
}

static void * inputEventThreadFunc(void *data)
{
    PInputDevice_S pInputDevice = (PInputDevice_S)data;
    InputEvent_S inputEvent;
	int ret = -1;

	memset(&inputEvent, 0, sizeof(inputEvent));

	while (1)
	{
        ret = pInputDevice->getInputEvent(&inputEvent);

		if (!ret)
		{
            pthread_mutex_lock(&g_mutex);  

            putInputEventToBuffer(&inputEvent);

			pthread_cond_signal(&g_condVar);
			pthread_mutex_unlock(&g_mutex); 	
		}
	}

	return NULL;
}


int initInputDevice(void)
{
    PInputDevice_S pTmp = g_pInputDevHead;
    pthread_t thread;
	int ret = -1;

	while (pTmp)
	{
        ret = pTmp->inputDevInit();

		if (!ret)
		{
            ret = pthread_create(&thread, NULL, inputEventThreadFunc, pTmp);
			if (-1 == ret)
			{
                perror("pthread_create");
				return -1;
			}
		}

		pTmp = pTmp->pNext;
	}

	return ret;	
}

int userGetInputEventData(PInputEvent_S pInputEvent)
{
    InputEvent_S inputEvent;
	int ret = -1;

	memset(&inputEvent, 0, sizeof(inputEvent));

	pthread_mutex_lock(&g_mutex);
	ret = getInputEventFromBuffer(&inputEvent);
	if (!ret)
	{
        *pInputEvent = inputEvent;
		pthread_mutex_lock(&g_mutex);	
	}
	else
	{
        pthread_cond_wait(&g_condVar, &g_mutex);
		ret = getInputEventFromBuffer(&inputEvent);
		if (!ret)
		{
            *pInputEvent = inputEvent;
		}
		pthread_mutex_unlock(&g_mutex);	
	}

	return ret;	
}



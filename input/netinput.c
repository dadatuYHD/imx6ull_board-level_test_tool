#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include <input_manager.h>
#include <netinput.h>

#define SERVER_PORT 8888

static int g_socketFd = 0;

static int net_inputDevInit(void)
{
    int ret = -1;
	struct sockaddr_in serverAddr;

    g_socketFd = socket(AF_INET, SOCK_DGRAM, 0);
	if (-1 == g_socketFd)
	{
        perror("socket");
		return -1;
	}

    memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family      = AF_INET;
	serverAddr.sin_port        = htons(SERVER_PORT);  /* host to net, short */
 	serverAddr.sin_addr.s_addr = INADDR_ANY;
	ret = bind(g_socketFd, (const struct sockaddr *)&serverAddr, sizeof(struct sockaddr));
	if (-1 == ret)
	{
        perror("bind");
		return -1;    
	}

	return ret;
}

static int net_inputDevExit(void)
{
    close(g_socketFd);

	return 0;
}

static int net_inputGetInputEvent(PInputEvent_S pInputEvent)
{
	struct sockaddr_in sockClientAddr;
	int recvLen = 0;
	int ret = -1;
	char recvBuf[1000] = {'\0'};
	socklen_t addrLen = sizeof(struct sockaddr);

	memset(&sockClientAddr, 0, sizeof(sockClientAddr));

	recvLen = recvfrom(g_socketFd, recvBuf, sizeof(recvBuf) - 1, 0, (struct sockaddr *)&sockClientAddr, &addrLen);
	if (recvLen > 0)
	{
        recvBuf[recvLen] = '\0';
		pInputEvent->type 	= INPUT_TYPE_NET;
		gettimeofday(&pInputEvent->time, NULL);
		strncpy(pInputEvent->str, recvBuf, 1000);
		pInputEvent->str[999] = '\0';
		ret = 0;
	}
	else
	{
        ret = -1;
	}

	return ret;
}


static InputDevice_S g_netInputDevice = {
    .name          = "net",
	.inputDevInit  = net_inputDevInit, 
	.inputDevExit  = net_inputDevExit,
	.getInputEvent = net_inputGetInputEvent, 
};

void netInputDevRegister(void)
{
    inputDevRegister(&g_netInputDevice);    
}


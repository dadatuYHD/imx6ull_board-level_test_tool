#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <linux/fb.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>

#include <input_manager.h>

int main(int argc, char **argv)
{
	int ret;
	InputEvent_S inputEvent;
	
	inputSystemRegister();
	inputDeviceInit();

	while (1)
	{
		printf("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
		ret = userGetInputEventData(&inputEvent);
		printf("%s %s %d, ret = %d\n", __FILE__, __FUNCTION__, __LINE__, ret);
		if (ret) {
			printf("userGetInputEventData err!\n");
			return -1;
		}
		else
		{
			printf("%s %s %d, inputEvent.type = %d\n", __FILE__, __FUNCTION__, __LINE__, inputEvent.type );
			if (inputEvent.type == INPUT_TYPE_SCREENTOUCH)
			{
				printf("type     : %d\n", inputEvent.type);
				printf("x        : %d\n", inputEvent.x);
				printf("y        : %d\n", inputEvent.y);
				printf("pressure : %d\n", inputEvent.pressure);
			}
			else if (inputEvent.type == INPUT_TYPE_NET)
			{
				printf("type      : %d\n", inputEvent.type);
				printf("str       : %s\n", inputEvent.str);
			}
		}
	}
	
	return 0;	
}




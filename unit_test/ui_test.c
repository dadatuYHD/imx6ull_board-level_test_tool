#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <linux/fb.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <stdlib.h>

#include <disp_manager.h>
#include <font_manager.h>
#include <ui.h>

int main(int argc, char **argv)
{
	PDispBuffer_S pBuffer;
	int error;
	UiButton_S button;
	DispRegion_S region;

	if (argc != 2)
	{
		printf("Usage: %s <font file name>\n", argv[0]);
		return -1;
	}

	dispDevRegister();

	selectDefaultDispDev("fb");

	defaultDispDevInit();

	pBuffer = getDispBuffer();

	fontsLibSystemRegister();
	
	error = selectAndInitFontLib("freetype", argv[1]);
	if (error)
	{
		printf("SelectAndInitFont err\n");
		return -1;
	}

	region.xLeftUp = 300;
	region.yLeftUp = 300;
	region.width   = 300;
	region.height  = 100;
	
	initButton(&button, "test", &region, NULL, NULL);
	button.pDrawButton(&button, pBuffer);
	while (1)
	{
		button.pPressedButton(&button, pBuffer, NULL);
		sleep(2);
	}
	
	return 0;	
}




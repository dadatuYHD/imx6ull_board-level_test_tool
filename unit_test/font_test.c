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

int main(int argc, char **argv)
{
	PDispBuffer_S pBuffer;
	int error;

	FontBitMap_S fontBitMap;
	char * str= "www.100ask.net";
	int i = 0;
	int lcd_x;
	int lcd_y;
	int font_size;
	
	if (argc != 5)
	{
		printf("Usage: %s <font_file> <lcd_x> <lcd_y> <font_size>\n", argv[0]);
		return -1;
	}

	lcd_x = strtol(argv[2], NULL, 0);
	lcd_y = strtol(argv[3], NULL, 0);
	
	font_size  = strtol(argv[4], NULL, 0);
		
    dispDevRegister();

	selectDefaultDispDev("fb");

	initDefaultDispDev();

	pBuffer = getDispBuffer();

	fontsLibRegister();
	
	error = selectAndInitFontLib("freetype", argv[1]);
	if (error)
	{
		printf("SelectAndInitFont err\n");
		return -1;
	}
	
	setFontSize(font_size);

	while (str[i])
	{
		/* get bitmap */
		fontBitMap.curOriginX = lcd_x;
		fontBitMap.curOriginY = lcd_y;
		error = getFontBitMap(str[i], &fontBitMap);
		if (error)
		{
			printf("getFontBitMap err\n");
			return -1;
		}

		/* draw on buffer */		
		drawFontBitMap(&fontBitMap, 0xff0000);

		/* flush to lcd/web */		
		flushDispRegion(&fontBitMap.region, pBuffer);
		

		lcd_x = fontBitMap.nextOriginX;
		lcd_y = fontBitMap.nextOriginY;	
		i++;
	}
	
	return 0;	
}




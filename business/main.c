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
#include <input_manager.h>
#include <page_manager.h>


int main(int argc, char **argv)
{
	int error;

	if (argc != 2)
	{
		printf("Usage: %s <font_file_name>\n", argv[0]);
		return -1;
	}
	
	/* 初始化显示系统 */		
	dispDevRegister();
	selectDefaultDispDev("fb");
	initDefaultDispDev();

	/* 初始化输入系统 */		
    inputDevRegister();
	initInputDevice();

	/* 初始化文字系统 */		
	fontsLibRegister();
	error = selectAndInitFontLib("freetype", argv[1]);
	if (error)
	{
		printf("SelectAndInitFont err\n");
		return -1;
	}

	/* 初始化页面系统 */		
	registerPages();

	/* 运行业务系统的主页面 */
	selectPage("main_page")->run(NULL);
	
	return 0;	
}






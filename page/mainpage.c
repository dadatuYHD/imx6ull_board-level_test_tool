#include <config.h>
#include <ui.h>
#include <page_manager.h>

#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define X_GAP 5
#define Y_GAP 5

static UiButton_S g_buttons[ITEMCFG_MAX_NUM];
static int g_buttonCnt;

static int mainPagePressedButton(PUiButton_S pButton, PDispBuffer_S pDispBuffer, PInputEvent_S pInputEvent)
{
	unsigned int dwColor = BUTTON_DEFAULT_COLOR;
	char name[100];
	char status[100];
	char * pButtonName;
	char * pCommandStatus[3] = {"err", "ok", "percent"};
	int commandStatusIndex = -1;
	char command[1000];
	PItemCfg_S pItemCfg;

	pButtonName = pButton->pName;
	
	/* 1. 对于触摸屏事件 */
	if (pInputEvent->type == INPUT_TYPE_SCREENTOUCH)
	{
		/* 1.1 分辨能否被点击 */
		if (getItemCfgByName(pButton->pName)->isCanBeTouched == 0)
			return -1;

		/* 1.2 修改颜色 */
		pButton->status = !pButton->status;
		if (pButton->status)
		{
            dwColor = BUTTON_PRESSED_COLOR;
		    commandStatusIndex = 1;
		}
	}
	else if (pInputEvent->type == INPUT_TYPE_NET)
	{
		/* 2. 对于网络事件 */
		
		/* 根据传入的字符串修改颜色 : wifi ok, wifi err, burn 70 */
		sscanf(pInputEvent->str, "%s %s", name, status);
		if (strcmp(status, "ok") == 0)
		{
            dwColor = BUTTON_PRESSED_COLOR;
			commandStatusIndex = 1;
		}	
		else if (strcmp(status, "err") == 0)
		{
            dwColor = BUTTON_DEFAULT_COLOR;
			commandStatusIndex = 0;
		}
			
		else if (status[0] >= '0' && status[0] <= '9')
		{			
			dwColor = BUTTON_PERCENT_COLOR;
			pButtonName = status;
			commandStatusIndex = 2;
		}
		else
			return -1;
	}
	else
	{
		return -1;
	}

	/* 绘制底色 */
	drawRegion(&pButton->region, dwColor);

	/* 居中写文字 */
	drawTextInRegionCentral(pButtonName, &pButton->region, BUTTON_TEXT_COLOR);

	/* flush to lcd/web */
	flushDispRegion(&pButton->region, pDispBuffer);

	/* 执行command */
	pItemCfg = getItemCfgByName(pButton->pName);
	if (pItemCfg->command[0] != '\0')
	{
		sprintf(command, "%s %s", pItemCfg->command, pCommandStatus[commandStatusIndex]);
		//system(command); //容易卡死
	}
	
	return 0;
}

static int getFontSizeForAllButton(void)
{
	int i;
	int max_len = -1;
	int max_index = 0;
	int len;
	DispRegionCartesian_S regionCar;
	float k, kx, ky;
	
	/* 1. 找出name最长的Button */
	for (i = 0; i < g_buttonCnt; i++)
	{
		len = strlen(g_buttons[i].pName);
		if (len > max_len)
		{
			max_len = len;
			max_index = i;
		}
	}

	/* 2. 以font_size =100, 算出它的外框 */
	setFontSize(100);
	getStrBoxRegionCar(g_buttons[max_index].pName, &regionCar);

	/* 3. 把文字的外框缩放为Button的外框 */
	kx = (float)g_buttons[max_index].region.width / regionCar.width;
	ky = (float)g_buttons[max_index].region.height / regionCar.height;
	//printf("button width / str width   = %d/%d = %f\n", g_tButtons[max_index].tRegion.iWidth, tRegionCar.iWidth, kx);
	//printf("button height / str height = %d/%d = %f\n", g_tButtons[max_index].tRegion.iHeigh, tRegionCar.iHeigh, ky);
	if (kx < ky)
		k = kx;
	else
		k = ky;

	//printf("font size = %d\n", (int)(k*100));
	/* 4. 反算出font size, 只取0.80, 避免文字过于接近边界 */
	return k * 100 * 0.8;
}

static void generateButtonsUi(void)
{
	int width, height;
	int n_per_line;
	int row, rows;
	int col;
	int n;
	PDispBuffer_S pDispBuffer;
	int xres, yres;
	int start_x, start_y;
	int pre_start_x, pre_start_y;
	PUiButton_S pButton;
	int index = 0;
	int fontSize = 0;
	
	/* 算出单个按钮的width/height */
	g_buttonCnt = n = getItemCfgCount();
	
	pDispBuffer = getDispBuffer();
	xres = pDispBuffer->xRes;
	yres = pDispBuffer->yRes;
	width = sqrt(1.0/0.618 *xres * yres / n); //刚好占满屏幕得宽度
	n_per_line = xres / width + 1;            //每行多少个按钮 + 1
	width  = xres / n_per_line;
	height = 0.618 * width;	

	/* 居中显示:  计算每个按钮的region  */
	start_x = (xres - width * n_per_line) / 2;
	rows    = n / n_per_line;
	if (rows * n_per_line < n)
		rows++;
	
	start_y = (yres - rows*height)/2;

	/* 计算每个按钮的region */
	for (row = 0; (row < rows) && (index < n); row++)
	{
		pre_start_y = start_y + row * height;
		pre_start_x = start_x - width;
		for (col = 0; (col < n_per_line) && (index < n); col++)
		{
			pButton = &g_buttons[index];
			pButton->region.xLeftUp = pre_start_x + width;
			pButton->region.yLeftUp= pre_start_y;
			pButton->region.width   = width - X_GAP;
			pButton->region.height   = height - Y_GAP;
			pre_start_x = pButton->region.xLeftUp;

			/* InitButton */
			initButton(pButton, getItemCfgByIndex(index)->name, NULL, NULL, mainPagePressedButton);
			index++;
		}
	}

	fontSize = getFontSizeForAllButton();

	/* OnDraw */
	for (index = 0; index < n; index++)
	{
        g_buttons[index].fontSize = fontSize;
		g_buttons[index].pDrawButton(&g_buttons[index], pDispBuffer);
	}
}

static int isTouchPointInRegion(int x, int y, PDispRegion_S pRegion)
{
	if (x < pRegion->xLeftUp || x >= pRegion->xLeftUp + pRegion->width)
		return 0;

	if (y < pRegion->yLeftUp || y >= pRegion->yLeftUp + pRegion->height)
		return 0;

	return 1;
}


static PUiButton_S getButtonByName(char * pName)
{
	int i;
	
	for (i = 0; i < g_buttonCnt; i++)
	{
		if (strcmp(pName, g_buttons[i].pName) == 0)
			return &g_buttons[i];
	}

	return NULL;
}

static PUiButton_S getButtonByInputEvent(PInputEvent_S pInputEvent)
{
	int i;
	char name[100];
	
	if (pInputEvent->type == INPUT_TYPE_SCREENTOUCH)
	{
		for (i = 0; i < g_buttonCnt; i++)
		{
			if (isTouchPointInRegion(pInputEvent->x, pInputEvent->y, &g_buttons[i].region))
				return &g_buttons[i];
		}
	}
	else if (pInputEvent->type == INPUT_TYPE_NET)
	{
		sscanf(pInputEvent->str, "%s", name);
		return getButtonByName(name);
	}
	else
	{
		return NULL;
	}
	return NULL;
}

static void mainPageRun(void * pParams)
{
    int error = 0;
	InputEvent_S inputEvent;
	PUiButton_S pButton;
	PDispBuffer_S pDispBuffer = getDispBuffer();

    /* 1. 读取配置文件 */
	error = parseConfigFile();
	if (-1 == error)
	{
        printf("parseConfigFile is error!\n");
		return ;
	}

	/* 2. 根据配置文件生成按键界面*/
	generateButtonsUi();

	while (1)
	{
		/* 读取输入事件 */
		error = userGetInputEventData(&inputEvent);
		if (error || (inputEvent.pressure == 0 && inputEvent.type == 1) )
			continue;

		/* 根据输入事件找到按钮 */
		pButton = getButtonByInputEvent(&inputEvent);
		if (!pButton)
			continue;

		/* 调用按钮的OnPressed函数 */
		pButton->pPressedButton(pButton, pDispBuffer, &inputEvent);
	}
}

static PageAction_S g_pageAction = {
    .pName = "main_page",
	.run   = mainPageRun, 
};

void mainPageRegister(void)
{
    pageRegister(&g_pageAction);
}
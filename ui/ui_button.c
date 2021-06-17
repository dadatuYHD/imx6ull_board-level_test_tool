#include <ui.h>

static int defaultDrawButton(struct UiButton * pButton, PDispBuffer_S pDispBuffer)
{
    /* 绘制底色 */
	drawRegion(&pButton->region, BUTTON_DEFAULT_COLOR);

	/* 居中写文字 */
	drawTextInRegionCentral(pButton->pName, &pButton->region, BUTTON_TEXT_COLOR);

	/* flush to lcd/web */
	flushDispRegion(&pButton->region, pDispBuffer);

	return 0;
}

static int defaultPressedButton(struct UiButton * pButton, PDispBuffer_S pDispBuffer,  PInputEvent_S pInputEvent)
{
    unsigned int dwColor = BUTTON_DEFAULT_COLOR;
	
	pButton->status = !pButton->status;
	if (pButton->status)
		dwColor = BUTTON_PRESSED_COLOR;

    /* 绘制底色 */
	drawRegion(&pButton->region, dwColor);

	/* 居中写文字 */
	drawTextInRegionCentral(pButton->pName, &pButton->region, BUTTON_TEXT_COLOR);

	/* flush to lcd/web */
	flushDispRegion(&pButton->region, pDispBuffer);
	
	return 0;
}

void initButton(PUiButton_S pButton, char * pName, PDispRegion_S pRegion,  PDrawButtonFunc pDrawButton, PPressedButtonFunc pPressedButton)
{
	pButton->status = 0;
	pButton->pName = pName;
	pButton->region = *pRegion;
	pButton->pDrawButton = pDrawButton ? pDrawButton : defaultDrawButton;
	pButton->pPressedButton = pPressedButton ? pPressedButton : defaultPressedButton;
}
#ifndef __UI_H__
#define __UI_H__

#include <common.h>
#include <disp_manager.h>
#include <input_manager.h>

#define BUTTON_DEFAULT_COLOR 0xff0000
#define BUTTON_PRESSED_COLOR 0x00ff00
#define BUTTON_PERCENT_COLOR 0x0000ff
#define BUTTON_TEXT_COLOR    0x000000


struct UiButton;
typedef int (*PDrawButtonFunc)(struct UiButton * pButton, PDispBuffer_S pDispBuffer);
typedef int (*PPressedButtonFunc)(struct UiButton * pButton, PDispBuffer_S pDispBuffer,  PInputEvent_S pInputEvent);


typedef struct UiButton
{
    char * pName;
	int status;
	DispRegion_S region;
	PDrawButtonFunc pDrawButton;
	PPressedButtonFunc pPressedButton;
}UiButton_S, *PUiButton_S;


void initButton(PUiButton_S pButton, char * pName, PDispRegion_S pRegion,  PDrawButtonFunc pDrawButton, PPressedButtonFunc pPressedButton);



#endif

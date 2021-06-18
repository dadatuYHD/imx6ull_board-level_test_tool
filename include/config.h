#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <common.h>

#define ITEMCFG_MAX_NUM 30
#define CFG_FILE  "/etc/test_gui/gui.conf"

typedef struct ItemCfg {
	int index;
	char name[100];
	int isCanBeTouched;
	char command[100];
}ItemCfg_S, *PItemCfg_S;

int parseConfigFile(void);
int getItemCfgCount(void);
PItemCfg_S getItemCfgByIndex(int index);
PItemCfg_S getItemCfgByName(char * pName);



#endif

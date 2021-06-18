#include <config.h>
#include <stdio.h>
#include <string.h>

static ItemCfg_S g_itemCfgs[ITEMCFG_MAX_NUM]; //保存从配置文件读取出来的信息
static int g_itemCfgCount = 0;                //记录item的数量

int parseConfigFile(void)
{
	FILE *fp;
	char buf[100];
	char *p = buf;
	
	/* 1. open config file */
	fp = fopen(CFG_FILE, "r");
	if (!fp)
	{
		printf("can not open cfg file %s\n", CFG_FILE);
		return -1;
	}

	while (fgets(buf, 100, fp))
	{
		/* 2.1 read each line */
		buf[99] = '\0';		

		/* 2.2 吃掉开头的空格或TAB */
		p = buf;
		while (*p == ' ' || *p =='\t')
			p++;

		/* 2.3 忽略注释 */
		if (*p == '#')
			continue;

		/* 2.4 处理 */
		g_itemCfgs[g_itemCfgCount].command[0] = '\0';
		g_itemCfgs[g_itemCfgCount].index = g_itemCfgCount;
		sscanf(p, "%s %d %s", g_itemCfgs[g_itemCfgCount].name, &g_itemCfgs[g_itemCfgCount].isCanBeTouched, 
			                  g_itemCfgs[g_itemCfgCount].command);
		//printf("%s %d %s\n", g_itemCfgs[g_itemCfgCount].name, g_itemCfgs[g_itemCfgCount].isCanBeTouched, g_itemCfgs[g_itemCfgCount].command);
		g_itemCfgCount++;		
	}
	return 0;
}

int getItemCfgCount(void)
{
	return g_itemCfgCount;
}

PItemCfg_S getItemCfgByIndex(int index)
{
	if (index < g_itemCfgCount)
		return &g_itemCfgs[index];
	else
		return NULL;
}

PItemCfg_S getItemCfgByName(char *pName)
{
	int i;
	for (i = 0; i < g_itemCfgCount; i++)
	{
		if (strcmp(pName, g_itemCfgs[i].name) == 0)
			return &g_itemCfgs[i];
	}
	return NULL;
}


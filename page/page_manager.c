#include <string.h>

#include <mainpage.h>
#include <page_manager.h>
#include <common.h>

static PPageAction_S g_pageActionHead = NULL;  

void pageRegister(PPageAction_S pPageAction)
{
    pPageAction->pNext = g_pageActionHead;  
    g_pageActionHead = pPageAction;   
}

void registerPages(void)
{
    mainPageRegister();
}

PPageAction_S selectPage(char * pName)
{
	PPageAction_S pTmp = g_pageActionHead;

	while (pTmp)
	{
		if (strcmp(pName, pTmp->pName) == 0)
			return pTmp;
		pTmp = pTmp->pNext;
	}

	return NULL;
}

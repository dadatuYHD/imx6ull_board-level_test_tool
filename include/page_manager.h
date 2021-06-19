#ifndef __PAGE_MANAGER_H__
#define __PAGE_MANAGER_H__

typedef struct PageAction
{
    char * pName;
	void (*run)(void * pParams);
	struct PageAction * pNext;
}PageAction_S, * PPageAction_S;

//provide with model of page_manager.c to page_action
void pageRegister(PPageAction_S pPageAction);

//provide with model of page_manager.c to user/page_test.c
PPageAction_S selectPage(char * pName);
void pageSystemRegister(void);
#endif
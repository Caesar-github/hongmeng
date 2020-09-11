#include "los_event.h"
#include "los_task.h"
//#include "osTest.h"

/*����PID*/
UINT32 g_TestTaskID01;

/*�¼����ƽṹ��*/
EVENT_CB_S  example_event;

/*�ȴ����¼�����*/
#define event_wait 0x00000001

/*����������ں���*/
VOID Example_Event()
{
    UINT32 uwRet;
    UINT32 uwEvent;

/*��ʱ �ȴ���ʽ���¼�,��ʱʱ��Ϊ100 ticks
   ��100 ticks ��δ��ȡ��ָ���¼������¼���ʱ������ֱ�ӻ���*/
    printf("Example_Event wait event 0x%x \n",event_wait);

    uwEvent = Los_EventRead(&example_event, event_wait, LOS_WAITMODE_AND, 100);
    if(uwEvent == event_wait)
    {
        printf("Example_Event,read event :0x%x\n",uwEvent);
    }
    else
        printf("Example_Event,read event timeout\n");
    return;
}

UINT32 Example_TaskEntry()
{
    UINT32 uwRet;
    TSK_INIT_PARAM_S stTask1;

    /*�¼���ʼ��*/
    uwRet = Los_EventInit(&example_event);
    if(uwRet != LOS_OK)
    {
        printf("init event failed .\n");
        return -1;
    }

    /*��������*/
    memset(&stTask1, 0, sizeof(TSK_INIT_PARAM_S));
    stTask1.pfnTaskEntry = (TSK_ENTRY_FUNC)Example_Event;
    stTask1.pcName       = "EventTsk1";
    stTask1.uwStackSize  = OS_TSK_DEFAULT_STACK_SIZE;
    stTask1.usTaskPrio   = 5;
    uwRet = LOS_TaskCreate(&g_TestTaskID01, &stTask1);
    if(uwRet != LOS_OK)
    {
        printf("task create failed .\n");
        return LOS_NOK;
    }

    /*дg_TestTaskID01 �ȴ��¼�*/
    printf("Example_TaskEntry write event .\n");

    uwRet = Los_EventWrite(&example_event, event_wait);
    if(uwRet != LOS_OK)
    {
        printf("event write failed .\n");
        return LOS_NOK;
    }

	/*���־λ*/
    printf("EventMask:%d\n",example_event.uwEventID);
    LOS_EventClear(&example_event, ~example_event.uwEventID);
    printf("EventMask:%d\n",example_event.uwEventID);
	
    /*ɾ������*/
    uwRet = LOS_TaskDelete(g_TestTaskID01);
    if(uwRet != LOS_OK)
    {
        printf("task delete failed .\n");
        return LOS_NOK;
    }

    return LOS_OK;
}


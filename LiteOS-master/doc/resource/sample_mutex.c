#include "los_mux.h"
#include "los_task.h"



/*���������id*/
MUX_HANDLE_T  g_Testmux01;
/*����PID*/
UINT32 g_TestTaskID01;
UINT32 g_TestTaskID02;

VOID Example_MutexTask1()
{
    UINT32 uwRet;

    printf("task1 try to get  mutex, wait 10 ticks.\n");
    /*���뻥����*/
    uwRet=LOS_MuxPend(g_Testmux01, 10);

    if(uwRet == LOS_OK)
    {
        printf("task1 get mutex g_Testmux01.\n");
        /*�ͷŻ�����*/
        LOS_MuxPost(g_Testmux01);
        return;
    }
    else if(uwRet == LOS_ERRNO_MUX_TIMEOUT )
    {
            printf("task1 timeout and try to get  mutex, wait forever.\n");
            /*���뻥����*/
            uwRet=LOS_MuxPend(g_Testmux01, LOS_WAIT_FOREVER);
            if(uwRet == LOS_OK)
            {
                printf("task1 wait forever,get mutex g_Testmux01.\n");
                /*�ͷŻ�����*/
                LOS_MuxPost(g_Testmux01);
                return;
            }
    }
    return;
}

VOID Example_MutexTask2()
{
    UINT32 uwRet;

    printf("task2 try to get  mutex, wait forever.\n");
    /*���뻥����*/
    uwRet=LOS_MuxPend(g_Testmux01, LOS_WAIT_FOREVER);

    printf("task2 get mutex g_Testmux01 and suspend 100 ticks.\n");

    /*��������100 ticks*/
    LOS_TaskDelay(100);

    printf("task2 resumed and post the g_Testmux01\n");
    /*�ͷŻ�����*/
    LOS_MuxPost(g_Testmux01);
    return;

}

UINT32 Example_TaskEntry()
{
    UINT32 uwRet;
    TSK_INIT_PARAM_S stTask1;
    TSK_INIT_PARAM_S stTask2;

    /*����������*/
    LOS_MuxCreate(&g_Testmux01);

    /*���������*/
    LOS_TaskLock();

    /*��������1*/
    memset(&stTask1, 0, sizeof(TSK_INIT_PARAM_S));
    stTask1.pfnTaskEntry = (TSK_ENTRY_FUNC)Example_MutexTask1;
    stTask1.pcName       = "MutexTsk1";
    stTask1.uwStackSize  = OS_TSK_DEFAULT_STACK_SIZE;
    stTask1.usTaskPrio   = 5;
    uwRet = LOS_TaskCreate(&g_TestTaskID01, &stTask1);
    if(uwRet != LOS_OK)
    {
        printf("task1 create failed .\n");
        return LOS_NOK;
    }

    /*��������2*/
    memset(&stTask2, 0, sizeof(TSK_INIT_PARAM_S));
    stTask2.pfnTaskEntry = (TSK_ENTRY_FUNC)Example_MutexTask2;
    stTask2.pcName       = "MutexTsk2";
    stTask2.uwStackSize  = OS_TSK_DEFAULT_STACK_SIZE;
    stTask2.usTaskPrio   = 4;
    uwRet = LOS_TaskCreate(&g_TestTaskID02, &stTask2);
    if(uwRet != LOS_OK)
    {
        printf("task2 create failed .\n");
        return LOS_NOK;
    }

    /*�����������*/
    LOS_TaskUnlock();
    /*��������300 ticks*/
    LOS_TaskDelay(300);

    /*ɾ��������*/
    LOS_MuxDelete(g_Testmux01);

    /*ɾ������1*/
    uwRet = LOS_TaskDelete(g_TestTaskID01);
    if(uwRet != LOS_OK)
    {
        printf("task1 delete failed .\n");
        return LOS_NOK;
    }
    /*ɾ������2*/
    uwRet = LOS_TaskDelete(g_TestTaskID02);
    if(uwRet != LOS_OK)
    {
        printf("task2 delete failed .\n");
        return LOS_NOK;
    }

    return LOS_OK;
}



UINT32 g_uwTskLoID;
UINT32 g_uwTskHiID;
#define TSK_PRIOR_HI 4
#define TSK_PRIOR_LO 5

UINT32 Example_TaskHi(VOID)
{
    UINT32 uwRet;
    UINT32 uwCurrentID;
    TSK_INFO_S stTaskInfo;

    printf("[cpu%d] Enter TaskHi Handler.\r\n", osCurrCpuid());

    /*��ʱ2��Tick����ʱ�����������ִ��ʣ��������������ȼ�������(g_uwTskLoID����)*/
    uwRet = LOS_TaskDelay(2);
    if (uwRet != LOS_OK)
    {
        printf("Delay Task Failed.\r\n");
        return LOS_NOK;
    }

    /*2��Tickʱ�䵽�˺󣬸�����ָ�������ִ��*/
    printf("TaskHi LOS_TaskDelay Done.\r\n");

    /*������������*/
    uwRet = LOS_TaskSuspend(g_uwTskHiID);
    if (uwRet != LOS_OK)
    {
        printf("Suspend TaskHi Failed.\r\n");
        return LOS_NOK;
    }
    printf("TaskHi LOS_TaskResume Success.\r\n");
}

/*�����ȼ�������ں���*/
UINT32 Example_TaskLo(VOID)
{
    UINT32 uwRet;
    UINT32 uwCurrentID;
    TSK_INFO_S stTaskInfo;

    printf("[cpu%d] Enter TaskLo Handler.\r\n", osCurrCpuid());

    /*��ʱ2��Tick����ʱ�����������ִ��ʣ�������о͸����ȼ�������(��������)*/
    uwRet = LOS_TaskDelay(2);
    if (uwRet != LOS_OK)
    {
        printf("Delay TaskLo Failed.\r\n");
        return LOS_NOK;
    }

    printf("TaskHi LOS_TaskDelete Success.\r\n");
}

/*���������ں����������洴�����ȼ���һ������������*/
UINT32 Example_TskCaseEntry(VOID)
{
    UINT32 uwRet;
    TSK_INIT_PARAM_S stInitParam = {0};

    /*���������*/
    LOS_TaskLock();

    printf("LOS_TaskLock() Success on cpu%d!\r\n", osCurrCpuid());

    stInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)Example_TaskHi;
    stInitParam.usTaskPrio = TSK_PRIOR_HI;
    stInitParam.pcName = "HIGH_NAME";
    stInitParam.uwStackSize = 0x700;
    stInitParam.uwResved   = LOS_TASK_STATUS_DETACHED;
    /*�󶨸����ȼ�����CPU1����*/
    stInitParam.usCpuAffiMask = CPUID_TO_AFFI_MASK(osCurrCpuid());
    /*���������ȼ���������CPU1�ĵ��������������񴴽��ɹ��󲻻�����ִ��*/
    uwRet = LOS_TaskCreate(&g_uwTskHiID, &stInitParam);
    if (uwRet != LOS_OK)
    {
        LOS_TaskUnlock();

        printf("Example_TaskHi create Failed!\r\n");
        return LOS_NOK;
    }

    printf("Example_TaskHi create Success!\r\n");

    stInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)Example_TaskLo;
    stInitParam.usTaskPrio = TSK_PRIOR_LO;
    stInitParam.pcName = "LOW_NAME";
    stInitParam.uwStackSize = 0x700;
    stInitParam.uwResved   = LOS_TASK_STATUS_DETACHED;
    /*�����ȼ���������CPU�׺���*/
    stInitParam.usCpuAffiMask = 0;
    /*���������ȼ����񣬾�����������ȣ��������ڸ�����û�а󶨸ô����������񴴽��ɹ����������������CPUִ��*/
    uwRet = LOS_TaskCreate(&g_uwTskLoID, &stInitParam);
    if (uwRet != LOS_OK)
    {
        LOS_TaskUnlock();

        printf("Example_TaskLo create Failed!\r\n");
        return LOS_NOK;
    }

    printf("Example_TaskLo create Success!\r\n");

    /*����������ȣ���ʱ�ᷢ��������ȣ�ִ�о����б���������ȼ�����*/
    LOS_TaskUnlock();

    while(1){};

    return LOS_OK;
}

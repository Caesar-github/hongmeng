/*************************************************************************
	> File Name: it_membox_performance.c
	> Author: 
	> Mail: 
	> Created Time: Mon 27 Jul 2015 04:55:25 PM CST
 ************************************************************************/

#include<stdio.h>
#include "los_membox.h"

VOID los_membox_test(void) {
    UINT32 *p_num = NULL;
    UINT32 uwBlkSize = 10, uwBoxSize = 100;
    UINT32 uwRet;
    UINT32 pBoxMem[1000];
    uwRet = LOS_MemboxInit( &pBoxMem[0], uwBoxSize, uwBlkSize);
    if(uwRet != LOS_OK)
    {
        dprintf("�ڴ�س�ʼ��ʧ��!\n");
        return;
    }
    else{
        dprintf("�ڴ�س�ʼ���ɹ�!\n");
    }
    
    /*�����ڴ��*/
    p_num = (int*)LOS_MemboxAlloc(pBoxMem);
    if (NULL == p_num) {
        dprintf("�ڴ����ʧ��!\n");
        return;
    }
    dprintf("�ڴ����ɹ�\n");
    /*��ֵ*/
    *p_num = 828;
    dprintf("*p_num = %d\n", *p_num);
     /*����ڴ�����*/
     LOS_MemboxClr(pBoxMem, p_num);
     dprintf("����ڴ����ݳɹ�\n *p_num = %d\n", *p_num);
    /*�ͷ��ڴ�*/
    uwRet = LOS_MemboxFree(pBoxMem, p_num);
    if (LOS_OK == uwRet) {
        dprintf("�ڴ��ͷųɹ�!\n");
    }
    else{
        dprintf("�ڴ��ͷ�ʧ��!\n");
    }
	return;
}



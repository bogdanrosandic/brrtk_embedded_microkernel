#ifndef __SEMAPHORE_H__
#define __SEMAPHORE_H__
#include "brrtk_app.h"
#include "brrtk_platform.h"
#include "brrtk_task.h"
#include "brrtk_utilities.h"
#include "scheduler.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct SBCB *SBCBptr;

typedef struct SBCB

{
    long int timeout_lwIP;// tick number before timeout expires, after that semaphore is released
    unsigned char usage;  //0 unused, 1 used, memory pool flag
    brrtk_TCBptr task_waiting_list_periodic;
    brrtk_TCBptr task_waiting_list_aperiodic;
    brrtk_TCBptr task_waiting_list_medium;
    unsigned char brrtk_semaphore_usage_counter;            /*--semaphore is available if usage counter is 0--*/ 
                                                    
    struct SBCB * pNext_lwIP;// semaphore list, used for timeout expiration check
    
}  SBCB;



void BRRTK_Create_Cnt_Sem(SBCBptr p_Cnt_Semaphore,unsigned char count);

void BRRTK_Lock_Cnt_Sem(SBCBptr p_Cnt_Semaphore);
long int BRRTK_Lock_Cnt_Sem_wait(SBCBptr p_Cnt_Semaphore, long int timeout_ms);




void BRRTK_Release_Cnt_Sem(SBCBptr p_Cnt_Semaphore);
void BRRTK_Release_Cnt_Sem_ISR(SBCBptr p_Cnt_Semaphore);
void BRRTK_Release_Cnt_Sem_no_sched(SBCBptr p_Cnt_Semaphore);

void BRRTK_Delete_Cnt_Sem(SBCBptr p_Cnt_Semaphore);

#endif /*__SEMAPHORE_H__*/ 


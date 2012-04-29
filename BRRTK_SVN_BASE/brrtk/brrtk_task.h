/*
 * This file is part of the BRRTK source code.
 * 
 * Author: Bogdan Rosandic <bogdanrosandic@gmail.com.se>
 *
 */
 
#ifndef __BRRTK_TASK_H__
#define __BRRTK_TASK_H__


 /*---------------------------------------
     TYPEDEFS
      
  ----------------------------------------*/


 typedef struct brrtk_TCB *brrtk_TCBptr;
 typedef struct brrtk_TimeCB *brrtk_TimeCBptr;
 typedef struct brrtk_DelayCB *brrtk_DelayCBptr;
 
 typedef struct brrtk_DelayCB
 {               
    int delay;
    brrtk_TCBptr pTCB;
    unsigned int counter_startovanja;
    unsigned int brrtk_task_deadline_init; 
    unsigned int brrtk_task_deadline_counter;
    unsigned int brrtk_task_deadlines_counter;
    unsigned char lista_yes_no;  
    brrtk_DelayCBptr pNext;                        /* forward ptr for dbl linked list */
    brrtk_DelayCBptr pPrev;
  
 }  DelayCB;
 
 
 typedef struct brrtk_TimeCB
 {               
     brrtk_TCBptr pTCB;                         
     unsigned int brrtk_task_ready_counter_init;
     unsigned int brrtk_task_ready_counter;
     unsigned int brrtk_task_deadlines_counter;
     brrtk_TimeCBptr pNext;                    
     brrtk_TimeCBptr pPrev;
 }  TimeCB;
 
 typedef struct brrtk_TCB
 
{               
     void (* brrtk_task_pc)(void *);
     void *brrtk_task_sp;
     unsigned brrtk_task_id;
     unsigned char brrtk_task_state;
     unsigned char brrtk_task_Priority;
                                        
     brrtk_TimeCBptr pTimeCB; 
     brrtk_DelayCBptr pDelayCB;
     brrtk_TCBptr pNext;       
     brrtk_TCBptr pPrev;       
}  TCB;

 
 /*---------------------------------------
     FUNCTION
      DECLARATIONS
  ----------------------------------------*/

 void BRRTK_Task_Suspend(void);
 void BRRTK_Task_Suspend_no_sched(void);
 void BRRTK_Task_Delay(int delay);
 brrtk_TCBptr BRRTK_CreateNewTask_periodic(void (*task)(void * arg ) , void *  ptos, int Priority,unsigned int rdy_cnt);
 brrtk_TCBptr BRRTK_CreateNewTask_aperiodic(void (*task)(void * arg ), void *  ptos, brrtk_DelayCBptr * aper_timer,unsigned int deadline_cnt);
 brrtk_TCBptr BRRTK_CreateNewTask_aperiodic_medium(void (*task)(void * arg ), void *  ptos, int Priority, brrtk_DelayCBptr * aper_timer ,unsigned int deadline_cnt);
 void Terminate_Running_Task(void);;
 void BRRTK_Initialize(void);
 void BRRTK_StartAperiodicTask(brrtk_DelayCBptr aper_tajmer);
 void BRRTK_StopAperiodicTask(brrtk_DelayCBptr aper_tajmer);
 void BRRTK_EnterInt(void);
 void BRRTK_LeaveInt_sched(void);
 void BRRTK_Enter_Critical(void);
 void BRRTK_Leave_Critical(void);
 brrtk_TCBptr BRRTK_Current_Task(void);
 void BRRTK_StopAperiodicTask(brrtk_DelayCBptr aper_tajmer);
 void BRRTK_SystemTick_Handler(void);
 void BRRTK_ReturnFromInterruptHandler(void);




 
#endif /* __BRRTK_TASK_H__ */


/*
 * This file is part of the BRRTK source code.
 * 
 * Author: Bogdan Rosandic <bogdanrosandic@gmail.com>
 *
 */

#include "brrtk_app.h" 
#include "semaphore.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

extern brrtk_TCBptr BR_Active_TCB;
extern void remove_TCB_Aperiodic_ready_list(brrtk_TCBptr current_TCB);
extern void remove_TCB_low_ready_list(brrtk_TCBptr current_TCB);
extern void remove_TCB_medium_ready_list(brrtk_TCBptr current_TCB);
extern void insert_TCB_in_EDF_list(brrtk_TCBptr current_TCB);
extern  void insert_TCB_in_Medium_list(brrtk_TCBptr current_TCB);
extern void  insert_TCB_in_RM_list(brrtk_TCBptr current_TCB);

struct SBCB * sem_timeots_lwIP=NULL;

brrtk_TCBptr insert_TCB_in_RM_sem(brrtk_TCBptr current_TCB,brrtk_TCBptr list_head)
{
  
    if (list_head==NULL)
    {
        list_head=current_TCB;
        list_head->pNext=NULL;
        list_head->pPrev=NULL;
        return list_head;
    }
    else
    {
        brrtk_TCBptr temp=list_head;
        while(temp)
        {
            if (current_TCB->brrtk_task_Priority > temp->brrtk_task_Priority)
            {
                if (temp->pNext==NULL)
                {
                    temp->pNext=current_TCB;
                    current_TCB->pPrev=temp;
                    current_TCB->pNext=NULL;
                    temp=current_TCB;
                }
                temp=temp->pNext;
            }
            else 
            {
                if (temp->pPrev==NULL)
                {
                    temp->pPrev=current_TCB;
                    current_TCB->pNext=temp;
                    list_head=current_TCB;
                    current_TCB->pPrev=NULL;
                }
                else
                {
                    temp->pPrev->pNext=current_TCB;
                    current_TCB->pPrev=temp->pPrev;
                    current_TCB->pNext=temp;
                    temp->pPrev=current_TCB;
                }
            break;
            }
        }
    }
    return list_head;
}

brrtk_TCBptr insert_TCB_in_medium_sem(brrtk_TCBptr current_TCB,brrtk_TCBptr list_head)
{
    if (list_head==NULL)
    {
        list_head=current_TCB;
        list_head->pNext=NULL;
        list_head->pPrev=NULL;
        return list_head;
    }
    else
    {
        brrtk_TCBptr temp=list_head;
        while(temp)
        {
            if (temp->pDelayCB)
        
                if (current_TCB->brrtk_task_Priority > temp->brrtk_task_Priority)
                {
                    if (temp->pNext==NULL)
                    {
                        temp->pNext=current_TCB;
                        current_TCB->pPrev=temp;
                        current_TCB->pNext=NULL;
                        temp=current_TCB;
                    }
                    temp=temp->pNext;
                }
                else 
                {
                    if (temp->pPrev==NULL)
                    {
                        temp->pPrev=current_TCB;
                        current_TCB->pNext=temp;
                        list_head=current_TCB;
                        current_TCB->pPrev=NULL;
                    }
                    else
                    {
                        temp->pPrev->pNext=current_TCB;
                        current_TCB->pPrev=temp->pPrev;
                        current_TCB->pNext=temp;
                        temp->pPrev=current_TCB;
                    }
                    break;
                }
        }
    }
    return list_head;
}



brrtk_TCBptr insert_TCB_in_EDF_sem(brrtk_TCBptr current_TCB,brrtk_TCBptr list_head)
{
    if (list_head==NULL)
    {
        list_head=current_TCB;
        list_head->pNext=NULL;
        list_head->pPrev=NULL;
        return list_head;
    }
    else
    {
        brrtk_TCBptr temp=list_head;
        while(temp)
        {
            if (current_TCB->pDelayCB->brrtk_task_deadline_counter > temp->pDelayCB->brrtk_task_deadline_counter)
            {
                if (temp->pNext==NULL)
                {
                    temp->pNext=current_TCB;
                    current_TCB->pPrev=temp;
                    current_TCB->pNext=NULL;
                    temp=current_TCB;
                }
                temp=temp->pNext;
            }
            else 
            {
                if (temp->pPrev==NULL)
                {
                    temp->pPrev=current_TCB;
                    current_TCB->pNext=temp;
                    list_head=current_TCB;
                    current_TCB->pPrev=NULL;
                }
                else
                {
                    temp->pPrev->pNext=current_TCB;
                    current_TCB->pPrev=temp->pPrev;
                    current_TCB->pNext=temp;
                    temp->pPrev=current_TCB;
                }
                break;
            }
        }
    }
    return list_head;
}

void insert_semaphore_in_timeout_list(SBCBptr p_Cnt_Semaphore)
{
    struct SBCB * temp=sem_timeots_lwIP;
  
    if (!temp)
    { 
        sem_timeots_lwIP=p_Cnt_Semaphore;
    }
    else
    {
        if (p_Cnt_Semaphore->timeout_lwIP <= sem_timeots_lwIP->timeout_lwIP )
        { 
            p_Cnt_Semaphore->pNext_lwIP=temp;
            sem_timeots_lwIP=p_Cnt_Semaphore;
            temp->timeout_lwIP-=p_Cnt_Semaphore->timeout_lwIP;
            long int pomocna=temp->timeout_lwIP;
            while(temp->pNext_lwIP)
            {
                temp=temp->pNext_lwIP;
                temp->timeout_lwIP +=pomocna;
            }
        }
        else 
        {
            p_Cnt_Semaphore->timeout_lwIP=p_Cnt_Semaphore->timeout_lwIP-sem_timeots_lwIP->timeout_lwIP;
            while (temp->pNext_lwIP)
            {
                if (p_Cnt_Semaphore->timeout_lwIP <= temp->pNext_lwIP->timeout_lwIP)
                {
                    p_Cnt_Semaphore->pNext_lwIP=temp->pNext_lwIP;
                    temp->pNext_lwIP=p_Cnt_Semaphore;
                    break;
                }
                temp=temp->pNext_lwIP;
            }
            if (!temp->pNext_lwIP)
            {
                temp->pNext_lwIP=p_Cnt_Semaphore;
            }
        }
    }
}

void sem_timeout_regular_refresh(SBCBptr p_Cnt_Semaphore)
{
    if (sem_timeots_lwIP==p_Cnt_Semaphore)
    {
        sem_timeots_lwIP=p_Cnt_Semaphore->pNext_lwIP;
        /*when removing first in the list and its timeout is not expired timeouts of other semaphores mus be refreshed*/
        if (sem_timeots_lwIP)
        {
            SBCBptr temp =sem_timeots_lwIP;
            sem_timeots_lwIP->timeout_lwIP+=p_Cnt_Semaphore->timeout_lwIP;
            /*refreshing semaphore timeout fields*/
            temp=temp->pNext_lwIP;
            while (temp)
            {
                temp->timeout_lwIP= temp->timeout_lwIP + p_Cnt_Semaphore->timeout_lwIP - sem_timeots_lwIP->timeout_lwIP;
                temp=temp->pNext_lwIP;
            }
        }
    }
    else 
    {/* if semaphore that should be removed is not first in the list*/
        SBCBptr temp =sem_timeots_lwIP;
        while (temp->pNext_lwIP!=p_Cnt_Semaphore)
        {
            temp=temp->pNext_lwIP;
        }
        temp->pNext_lwIP=temp->pNext_lwIP->pNext_lwIP;
    }
    p_Cnt_Semaphore->pNext_lwIP=NULL;
}


void BRRTK_Create_Cnt_Sem(SBCBptr p_Cnt_Semaphore,unsigned char count)
{
  
    
    brrtk_global_disable_interrupts();
    p_Cnt_Semaphore->task_waiting_list_periodic=NULL;
    p_Cnt_Semaphore->task_waiting_list_aperiodic=NULL;
    p_Cnt_Semaphore->brrtk_semaphore_usage_counter=count;
    brrtk_global_enable_interrupts();
}


void BRRTK_Lock_Cnt_Sem(SBCBptr p_Cnt_Semaphore){
 
    brrtk_global_disable_interrupts();
 
    if (p_Cnt_Semaphore->brrtk_semaphore_usage_counter>0)//Locks semaphore
    {
        p_Cnt_Semaphore->brrtk_semaphore_usage_counter--;
        brrtk_global_enable_interrupts();
        return;
    }
    else //adds task in waiting list
    {
        BR_Active_TCB->brrtk_task_state=WAITING;
        if (BR_Active_TCB->pTimeCB)
        {
            if (BR_Active_TCB->brrtk_task_Priority < MED_BND_RM)
            {
                remove_TCB_medium_ready_list(BR_Active_TCB);
                p_Cnt_Semaphore->task_waiting_list_medium=insert_TCB_in_medium_sem(BR_Active_TCB,p_Cnt_Semaphore->task_waiting_list_medium);
            }
        else
        {
            remove_TCB_low_ready_list(BR_Active_TCB);
            p_Cnt_Semaphore->task_waiting_list_periodic=insert_TCB_in_RM_sem(BR_Active_TCB,p_Cnt_Semaphore->task_waiting_list_periodic);
        }
    }
    else
    {
        if (BR_Active_TCB->brrtk_task_Priority > 0)
        {
            remove_TCB_medium_ready_list(BR_Active_TCB);
            p_Cnt_Semaphore->task_waiting_list_medium=insert_TCB_in_medium_sem(BR_Active_TCB,p_Cnt_Semaphore->task_waiting_list_medium);
        }
        else
        {
            remove_TCB_Aperiodic_ready_list(BR_Active_TCB);
            p_Cnt_Semaphore->task_waiting_list_aperiodic=insert_TCB_in_EDF_sem(BR_Active_TCB,p_Cnt_Semaphore->task_waiting_list_aperiodic);
        }
      }
      setBlockingCall();
     
      brrtk_scheduler();
      brrtk_global_disable_interrupts();
      p_Cnt_Semaphore->brrtk_semaphore_usage_counter--;
      brrtk_global_enable_interrupts();
      
    }
  
  
}

/*Function returns number of ticks left before timeout expiration*/
long int  BRRTK_Lock_Cnt_Sem_wait(SBCBptr p_Cnt_Semaphore, long int timeout_ms){
  
    brrtk_global_disable_interrupts();
    if (p_Cnt_Semaphore->brrtk_semaphore_usage_counter>0)
    {
        p_Cnt_Semaphore->brrtk_semaphore_usage_counter--;
        p_Cnt_Semaphore->timeout_lwIP=0;
        brrtk_global_enable_interrupts();
        return timeout_ms; 
    }
    else
    {
        if (timeout_ms > 0)
        {
            p_Cnt_Semaphore->timeout_lwIP=timeout_ms;
            insert_semaphore_in_timeout_list(p_Cnt_Semaphore);
        }
        
        BR_Active_TCB->brrtk_task_state=WAITING;
        if (BR_Active_TCB->pTimeCB)
        {
            if (BR_Active_TCB->brrtk_task_Priority < MED_BND_RM)
            {
                remove_TCB_medium_ready_list(BR_Active_TCB);
                p_Cnt_Semaphore->task_waiting_list_medium=insert_TCB_in_medium_sem(BR_Active_TCB,p_Cnt_Semaphore->task_waiting_list_medium);
            }
            else
            {
                remove_TCB_low_ready_list(BR_Active_TCB);
                p_Cnt_Semaphore->task_waiting_list_periodic=insert_TCB_in_RM_sem(BR_Active_TCB,p_Cnt_Semaphore->task_waiting_list_periodic);
            }
        }
        else
        {
            if (BR_Active_TCB->brrtk_task_Priority > 0)
            {
                remove_TCB_medium_ready_list(BR_Active_TCB);
                p_Cnt_Semaphore->task_waiting_list_medium=insert_TCB_in_medium_sem(BR_Active_TCB,p_Cnt_Semaphore->task_waiting_list_medium);
            }
            else
            {
                remove_TCB_Aperiodic_ready_list(BR_Active_TCB);
                p_Cnt_Semaphore->task_waiting_list_aperiodic=insert_TCB_in_EDF_sem(BR_Active_TCB,p_Cnt_Semaphore->task_waiting_list_aperiodic);
            }
        }
        setBlockingCall();
        brrtk_scheduler();
        brrtk_global_disable_interrupts();
        p_Cnt_Semaphore->brrtk_semaphore_usage_counter--;
        long int temp= p_Cnt_Semaphore->timeout_lwIP;
      
        if (temp != 0xffffffff)
        {
            sem_timeout_regular_refresh(p_Cnt_Semaphore); 
        }
        brrtk_global_enable_interrupts();
        return temp;// 
    }
}



void BRRTK_Release_Cnt_Sem(SBCBptr p_Cnt_Semaphore)
{
    brrtk_global_disable_interrupts();
    p_Cnt_Semaphore->brrtk_semaphore_usage_counter++;
    if (p_Cnt_Semaphore->task_waiting_list_aperiodic!=NULL)
    {
     
        p_Cnt_Semaphore->task_waiting_list_aperiodic->brrtk_task_state=READY;
        brrtk_TCBptr temp=p_Cnt_Semaphore->task_waiting_list_aperiodic->pNext;
             
        insert_TCB_in_EDF_list(p_Cnt_Semaphore->task_waiting_list_aperiodic);

        p_Cnt_Semaphore->task_waiting_list_aperiodic=temp;
        temp->pPrev=NULL;
        if (brrtk_true == isInterruptEntered())
        {
            setSchedulerStarted();
        }
        else
        {
            setBlockingCall();
            brrtk_scheduler();
            brrtk_global_enable_interrupts();
        }
   }
   else 
   {
        if (p_Cnt_Semaphore->task_waiting_list_medium)
        {
            p_Cnt_Semaphore->task_waiting_list_medium->brrtk_task_state=READY;
            brrtk_TCBptr temp=p_Cnt_Semaphore->task_waiting_list_medium->pNext;
                
            insert_TCB_in_Medium_list(p_Cnt_Semaphore->task_waiting_list_medium);
    
            p_Cnt_Semaphore->task_waiting_list_medium=temp;
            temp->pPrev=NULL;
            if (brrtk_true == isInterruptEntered())
            {
                setSchedulerStarted();
            }
            else
            {
                setBlockingCall();
                brrtk_scheduler();
                brrtk_global_enable_interrupts();
            }
                   
        }
        else
        {
            if (p_Cnt_Semaphore->task_waiting_list_periodic!=NULL)
            {
                p_Cnt_Semaphore->task_waiting_list_periodic->brrtk_task_state=READY;
                brrtk_TCBptr temp=p_Cnt_Semaphore->task_waiting_list_periodic->pNext;
                insert_TCB_in_RM_list(p_Cnt_Semaphore->task_waiting_list_periodic);
                p_Cnt_Semaphore->task_waiting_list_periodic=temp;
                temp->pPrev=NULL;
                if (brrtk_true == isInterruptEntered())
                {
                    setSchedulerStarted();
                }
                else
                {
                    setBlockingCall();
                    brrtk_scheduler();
                    brrtk_global_enable_interrupts();
                }
            }
            else
            {
                
                
            }
        }
    }
}

void BRRTK_Release_Cnt_Sem_ISR(SBCBptr p_Cnt_Semaphore)
{
 
    brrtk_global_disable_interrupts();
    p_Cnt_Semaphore->brrtk_semaphore_usage_counter++;
    if (p_Cnt_Semaphore->task_waiting_list_aperiodic!=NULL)
    {
        p_Cnt_Semaphore->task_waiting_list_aperiodic->brrtk_task_state=READY;
        brrtk_TCBptr temp=p_Cnt_Semaphore->task_waiting_list_aperiodic->pNext;
        insert_TCB_in_EDF_list(p_Cnt_Semaphore->task_waiting_list_aperiodic);
        p_Cnt_Semaphore->task_waiting_list_aperiodic=temp;
        temp->pPrev=NULL;
        setSchedulerStarted();
    }
    else 
    {
        if (p_Cnt_Semaphore->task_waiting_list_medium)
        {
            p_Cnt_Semaphore->task_waiting_list_medium->brrtk_task_state=READY;
            brrtk_TCBptr temp=p_Cnt_Semaphore->task_waiting_list_medium->pNext;
            insert_TCB_in_Medium_list(p_Cnt_Semaphore->task_waiting_list_medium);
            p_Cnt_Semaphore->task_waiting_list_medium=temp;
            temp->pPrev=NULL;
            setSchedulerStarted();
        }
        else
        {
            if (p_Cnt_Semaphore->task_waiting_list_periodic!=NULL)
            {
                p_Cnt_Semaphore->task_waiting_list_periodic->brrtk_task_state=READY;
                brrtk_TCBptr temp=p_Cnt_Semaphore->task_waiting_list_periodic->pNext;
                insert_TCB_in_RM_list(p_Cnt_Semaphore->task_waiting_list_periodic);
                p_Cnt_Semaphore->task_waiting_list_periodic=temp;
                temp->pPrev=NULL;
                setSchedulerStarted();
            }
            else
            {
                
                
            }
        }
    }
}


void BRRTK_Release_Cnt_Sem_no_sched(SBCBptr p_Cnt_Semaphore)
{
 
    brrtk_global_disable_interrupts();
    p_Cnt_Semaphore->brrtk_semaphore_usage_counter++;
    if (p_Cnt_Semaphore->task_waiting_list_aperiodic!=NULL)
    {
     
        p_Cnt_Semaphore->task_waiting_list_aperiodic->brrtk_task_state=READY;
        brrtk_TCBptr temp=p_Cnt_Semaphore->task_waiting_list_aperiodic->pNext;
        insert_TCB_in_EDF_list(p_Cnt_Semaphore->task_waiting_list_aperiodic);
        p_Cnt_Semaphore->task_waiting_list_aperiodic=temp;
        temp->pPrev=NULL;
        setSchedulerStarted();
    }
    else
    {
        if(p_Cnt_Semaphore->task_waiting_list_medium!=NULL)
        {
            p_Cnt_Semaphore->task_waiting_list_medium->brrtk_task_state=READY;
            brrtk_TCBptr temp=p_Cnt_Semaphore->task_waiting_list_medium->pNext;
                
            insert_TCB_in_Medium_list(p_Cnt_Semaphore->task_waiting_list_medium);
    
            p_Cnt_Semaphore->task_waiting_list_medium=temp;
            temp->pPrev=NULL;
            setSchedulerStarted();
        }
        else
        {
            if (p_Cnt_Semaphore->task_waiting_list_periodic!=NULL)
            {
                p_Cnt_Semaphore->task_waiting_list_periodic->brrtk_task_state=READY;
                brrtk_TCBptr temp=p_Cnt_Semaphore->task_waiting_list_periodic->pNext;
                
                insert_TCB_in_RM_list(p_Cnt_Semaphore->task_waiting_list_periodic);
    
                p_Cnt_Semaphore->task_waiting_list_periodic=temp;
                temp->pPrev=NULL;
                setSchedulerStarted();
            }
            else
            {
           
            }
        }
    }
    if (brrtk_false == isInterruptEntered())
    {
        brrtk_global_enable_interrupts();
    }
}




void BRRTK_Delete_Cnt_Sem(SBCBptr p_Cnt_Semaphore){
  
}

  


long int sem_timeout_tick_refresh()
{
    brrtk_global_disable_interrupts();
    
    if (sem_timeots_lwIP)
    {
        --sem_timeots_lwIP->timeout_lwIP;
        if (sem_timeots_lwIP->timeout_lwIP==0)
        {
            struct SBCB * temp = sem_timeots_lwIP;
            while (temp->timeout_lwIP == 0)
            {
                temp->timeout_lwIP=0xffffffff;
                BRRTK_Release_Cnt_Sem_ISR(temp);
                sem_timeots_lwIP=temp->pNext_lwIP;
                temp->pNext_lwIP=NULL;
                temp=sem_timeots_lwIP;
                if (!temp) return 1; 
            }
            temp=temp->pNext_lwIP;
            while (temp)
            {
                temp->timeout_lwIP-=sem_timeots_lwIP->timeout_lwIP;
                temp=temp->pNext_lwIP;
            }
            brrtk_global_enable_interrupts();
            return 1;   
        }
    }
    brrtk_global_enable_interrupts();
    return 0;
}





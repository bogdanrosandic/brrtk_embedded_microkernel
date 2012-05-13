/*

 * This file is part of the BRRTK source code.
 * 
 * Author: Bogdan Rosandic <bogdanrosandic@gmail.com>
 *
 */


#include "brrtk_app.h"
#include "brrtk_task.h"
#include "semaphore.h"
#include "mailbox.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#ifdef LM3S8962_ARM_CORTEX_M3
#include "inc/hw_types.h"
#include "driverlib/interrupt.h"
#endif

/*---------------------------------------
    GLOBAL 
     VARIABLES
 ----------------------------------------*/

extern brrtk_TCBptr BR_Active_TCB;
struct brrtk_MBXCB * mbx_timeots_lwIP=NULL;


extern void insert_TCB_in_RM_list(brrtk_TCBptr current_TCB);
extern void insert_TCB_in_Medium_list(brrtk_TCBptr current_TCB);
extern void insert_TCB_in_EDF_list(brrtk_TCBptr current_TCB);

extern void remove_TCB_low_ready_list(brrtk_TCBptr current_TCB);
extern void remove_TCB_medium_ready_list(brrtk_TCBptr current_TCB);
extern void remove_TCB_Aperiodic_ready_list(brrtk_TCBptr current_TCB);



void ActiveTCB_send_mbx_wait(brrtk_MBXCBptr p_mbx)
{
    if (p_mbx->waiting_list_send_first)
    {
     
        p_mbx->waiting_list_send_last->pNext=BR_Active_TCB;
        BR_Active_TCB->pPrev=p_mbx->waiting_list_send_last;
        BR_Active_TCB->pNext=NULL;
        p_mbx->waiting_list_send_last=BR_Active_TCB;
    }
    else
    {
        p_mbx->waiting_list_send_first=BR_Active_TCB;
        p_mbx->waiting_list_send_last=BR_Active_TCB;
        BR_Active_TCB->pNext=NULL;
        BR_Active_TCB->pPrev=NULL;
    }
    /*as a result this call places TCB of current executing task is placed in mailbox waiting list*/
}

void ActiveTCB_recieve_mbx_wait(brrtk_MBXCBptr p_mbx)
{
    if (p_mbx->waiting_list_recieve_first)
    {
        p_mbx->waiting_list_recieve_last->pNext=BR_Active_TCB;
        BR_Active_TCB->pPrev=p_mbx->waiting_list_recieve_last;
        BR_Active_TCB->pNext=NULL;
        p_mbx->waiting_list_recieve_last=BR_Active_TCB;
    }
    else
    {
        p_mbx->waiting_list_recieve_first=BR_Active_TCB;
        p_mbx->waiting_list_recieve_last=BR_Active_TCB;
        BR_Active_TCB->pNext=NULL;
        BR_Active_TCB->pPrev=NULL;
    }
  /*as a result this call places TCB of current executing task is placed in mailbox waiting list*/
}

void add_mailbox_to_timeout_list(brrtk_MBXCBptr p_mbx)
{
    long int help_var;
    struct brrtk_MBXCB * temp=mbx_timeots_lwIP;
    if (!temp)
    {
        mbx_timeots_lwIP=p_mbx;
    }
    else
    {
        if (p_mbx->timeout_lwIP <= mbx_timeots_lwIP->timeout_lwIP )
        {
            p_mbx->pNext_lwIP=temp;
            mbx_timeots_lwIP=p_mbx;
            temp->timeout_lwIP-=p_mbx->timeout_lwIP; 
            help_var=temp->timeout_lwIP;
            while(temp->pNext_lwIP)
            {
                temp=temp->pNext_lwIP;
                temp->timeout_lwIP +=help_var;
            }
        }
        else 
        {
            p_mbx->timeout_lwIP=p_mbx->timeout_lwIP-mbx_timeots_lwIP->timeout_lwIP;
            while (temp->pNext_lwIP)
            {
                if (p_mbx->timeout_lwIP <= temp->pNext_lwIP->timeout_lwIP)
                {
                    p_mbx->pNext_lwIP=temp->pNext_lwIP;
                    temp->pNext_lwIP=p_mbx;
                    break;
                }
                temp=temp->pNext_lwIP;
            }
            if (!temp->pNext_lwIP)
            {
                temp->pNext_lwIP=p_mbx;
            }
        }
    }
}

void mbx_timeout_regular_refresh(brrtk_MBXCBptr p_mbx)
{
    if (mbx_timeots_lwIP==p_mbx)
    {
        mbx_timeots_lwIP=p_mbx->pNext_lwIP;
        if (mbx_timeots_lwIP)
        {
            brrtk_MBXCBptr temp =mbx_timeots_lwIP;
            temp=temp->pNext_lwIP;
            while (temp)
            {
                temp->timeout_lwIP-= mbx_timeots_lwIP->timeout_lwIP;
                temp=temp->pNext_lwIP;
            }
            mbx_timeots_lwIP->timeout_lwIP+=p_mbx->timeout_lwIP;
        }
    }
    else 
    {
        brrtk_MBXCBptr temp =mbx_timeots_lwIP;
        while (temp->pNext_lwIP!=p_mbx)
        {
            temp=temp->pNext_lwIP;
        }
        temp->pNext_lwIP=temp->pNext_lwIP->pNext_lwIP;
    }
    p_mbx->pNext_lwIP=NULL;
}

long int mbx_timeout_tick_refresh(void)
{
    brrtk_global_disable_interrupts();
    if (mbx_timeots_lwIP)
    {
        --mbx_timeots_lwIP->timeout_lwIP;
        if (mbx_timeots_lwIP->timeout_lwIP==0)
        {
            struct brrtk_MBXCB * temp = mbx_timeots_lwIP;
            while (temp->timeout_lwIP == 0)
            {
                temp->timeout_lwIP=0xffffffff;
                Mailbox_timeouted(temp);
                mbx_timeots_lwIP=temp->pNext_lwIP;
                temp->pNext_lwIP=NULL;
                temp=mbx_timeots_lwIP;
                if (!temp) return 1; 
            }
            temp=temp->pNext_lwIP;
            while (temp)
            {
                temp->timeout_lwIP-=mbx_timeots_lwIP->timeout_lwIP;
                temp=temp->pNext_lwIP;
            }
            brrtk_global_enable_interrupts();
            return 1;   
        }
    }
    brrtk_global_enable_interrupts();
    return 0;
}


void BR_Create_Mbx (brrtk_MBXCBptr p_mbx,unsigned char msg_lngt,unsigned int num_of_msgs,void * mbx_buffer)
{
    p_mbx->message_lenght=msg_lngt;
    p_mbx->number_of_messages=num_of_msgs;
    p_mbx->mailbox_buffer=mbx_buffer;
    p_mbx->mailbox_buffer_tail=(char *)p_mbx->mailbox_buffer+(num_of_msgs-1)*sizeof(char*);
    p_mbx->avaliable_space=num_of_msgs;
    p_mbx->write_pointer=mbx_buffer;
    p_mbx->read_pointer=mbx_buffer;
}

void BR_Send_Mbx(brrtk_MBXCBptr p_mbx, void * p_mail)
{
    
    while (1)
    {   
        brrtk_global_disable_interrupts();
        if (p_mbx->avaliable_space)
        {
            int i;
            for (i=0; i < p_mbx->message_lenght;i++ )
            {
                *((char*)p_mbx->write_pointer+i)= *((char*)p_mail+i);
            }
            if (p_mbx->write_pointer==p_mbx->mailbox_buffer_tail)
            {
                p_mbx->write_pointer=p_mbx->mailbox_buffer;
            }
            else
            {
                p_mbx->write_pointer=(char*)p_mbx->write_pointer+sizeof(char*);
            }
            p_mbx->avaliable_space--;
            if (p_mbx->waiting_list_recieve_first)
            {
                brrtk_TCBptr help_var=p_mbx->waiting_list_recieve_first;
                p_mbx->waiting_list_recieve_first=  p_mbx->waiting_list_recieve_first->pNext;
                p_mbx->waiting_list_recieve_first->pPrev=NULL;
                if (!p_mbx->waiting_list_recieve_first)
                {
                    p_mbx->waiting_list_recieve_last=NULL;
                }
                if (help_var->pTimeCB)
                {
                    if (help_var->brrtk_task_Priority < MED_BND_RM)
                    {
                        insert_TCB_in_Medium_list(help_var); 
                    }
                    else
                    {
                        insert_TCB_in_RM_list(help_var);
                    }
                }
                else
                {
                    if (help_var->brrtk_task_Priority > 0)
                    {
                        insert_TCB_in_Medium_list(help_var);
                    }
                    else
                    {
                        insert_TCB_in_EDF_list(help_var);   
                    }
                }
                help_var->brrtk_task_state=READY;
                setBlockingCall();
                brrtk_scheduler();
            }
            brrtk_global_enable_interrupts();
            return;
        }
        else
        {
            BR_Active_TCB->brrtk_task_state=WAITING;
            if (BR_Active_TCB->pTimeCB)
            {
                if (BR_Active_TCB->brrtk_task_Priority < MED_BND_RM)
                {
                    remove_TCB_medium_ready_list(BR_Active_TCB); 
                }
                else
                {
                    remove_TCB_low_ready_list(BR_Active_TCB); 
                }
            }
            else
            {
                if (BR_Active_TCB->brrtk_task_Priority > 0)
                {
                    remove_TCB_medium_ready_list(BR_Active_TCB);
                }
                else
                {
                    remove_TCB_Aperiodic_ready_list(BR_Active_TCB); 
                }
            }
            ActiveTCB_send_mbx_wait(p_mbx);
            setBlockingCall();
            brrtk_scheduler();
        }
    }
}

void BR_Recieve_Mbx (brrtk_MBXCBptr p_mbx, void * p_dest)
{
    while(1)
    {
        brrtk_global_disable_interrupts();
        if (p_mbx->avaliable_space!=p_mbx->number_of_messages)
        {
            int i;
            for (i=0; i < p_mbx->message_lenght;i++ )
            {
                *((char*)p_dest+i)= *((char*)p_mbx->read_pointer+i);
            }
            p_mbx->avaliable_space++;
            if (p_mbx->read_pointer==p_mbx->mailbox_buffer_tail)
            {
                p_mbx->read_pointer=p_mbx->mailbox_buffer;
            }
            else
            {
                p_mbx->read_pointer=(char*)p_mbx->read_pointer+sizeof(char*);
            }
            if (p_mbx->waiting_list_send_first)
            {
                brrtk_TCBptr help_var=p_mbx->waiting_list_send_first;
                p_mbx->waiting_list_send_first=  p_mbx->waiting_list_send_first->pNext;
                p_mbx->waiting_list_send_first->pPrev=NULL;
                if (!p_mbx->waiting_list_send_first)
                {
                    p_mbx->waiting_list_send_last=NULL;
                }
                if (help_var->pTimeCB)
                {
                    if (help_var->brrtk_task_Priority < MED_BND_RM)
                    {
                        insert_TCB_in_Medium_list(help_var);
                    }
                    else
                    {
                        insert_TCB_in_RM_list(help_var);
                    }
                }
                else
                {
                    if (help_var->brrtk_task_Priority > 0)
                    {
                        insert_TCB_in_Medium_list(help_var);
                    }
                    else
                    {
                        insert_TCB_in_EDF_list(help_var);
                    }
                }
                help_var->brrtk_task_state=READY;
                setBlockingCall();
                brrtk_scheduler();
            }
            brrtk_global_enable_interrupts();
            return;
        }
        else
        {
            BR_Active_TCB->brrtk_task_state=WAITING;
            if (BR_Active_TCB->pTimeCB)
            {
                if (BR_Active_TCB->brrtk_task_Priority < MED_BND_RM)
                {
                    remove_TCB_medium_ready_list(BR_Active_TCB);
                }
                else
                {
                    remove_TCB_low_ready_list(BR_Active_TCB);
                }
            }
            else
            {
                if (BR_Active_TCB->brrtk_task_Priority > 0)
                {
                    remove_TCB_medium_ready_list(BR_Active_TCB);
                }
                else
                {
                    remove_TCB_Aperiodic_ready_list(BR_Active_TCB);
                }
            }
            ActiveTCB_recieve_mbx_wait(p_mbx);
            setBlockingCall();
            brrtk_scheduler();
        }
    }
}

char BR_Send_Mbx_ISR (brrtk_MBXCBptr p_mbx, void * p_mail)
{
    if (p_mbx->avaliable_space)
    {
        int i;
        for (i=0; i < p_mbx->message_lenght;i++ )
        {
            *((char*)p_mbx->write_pointer+i)= *((char*)p_mail+i);
        }
        if (p_mbx->write_pointer==p_mbx->mailbox_buffer_tail)
        {
            p_mbx->write_pointer=p_mbx->mailbox_buffer;
        }
        else
        {
            p_mbx->write_pointer=(char*)p_mbx->write_pointer+sizeof(char*);
        }
        p_mbx->avaliable_space--;
      
        if (p_mbx->waiting_list_recieve_first)
        {
            brrtk_TCBptr help_var=p_mbx->waiting_list_recieve_first;
            p_mbx->waiting_list_recieve_first=  p_mbx->waiting_list_recieve_first->pNext;
            p_mbx->waiting_list_recieve_first->pPrev=NULL;
            if (!p_mbx->waiting_list_recieve_first)
            {
                p_mbx->waiting_list_recieve_last=NULL;
            }
            if (help_var->pTimeCB)
            {
                if (help_var->brrtk_task_Priority < MED_BND_RM)
                {
                    insert_TCB_in_Medium_list(help_var);
                }
                else
                {
                    insert_TCB_in_RM_list(help_var);
                }
            }
            else
            {
                if (help_var->brrtk_task_Priority > 0)
                {
                    insert_TCB_in_Medium_list(help_var);
                }
                else
                {
                    insert_TCB_in_EDF_list(help_var);
                }
            }
            help_var->brrtk_task_state=READY;
            setSchedulerStarted();
        }
      
        return 0;
    }
    else
    {
        return 1;
    }
} 
  
char BR_Recieve_Mbx_ISR (brrtk_MBXCBptr p_mbx, void * p_dest)
{
    if (p_mbx->avaliable_space!=p_mbx->number_of_messages)
    {
        int i;
        for (i=0; i < p_mbx->message_lenght;i++ )
        {
            *((char*)p_dest+i)= *((char*)p_mbx->read_pointer+i);
        }
        p_mbx->avaliable_space++;
        if (p_mbx->read_pointer==p_mbx->mailbox_buffer_tail)
        {
            p_mbx->read_pointer=p_mbx->mailbox_buffer;
        }
        else 
        {
            p_mbx->read_pointer=(char*)p_mbx->read_pointer+sizeof(char*);
        }
        if (p_mbx->waiting_list_send_first)
        {
            brrtk_TCBptr help_var=p_mbx->waiting_list_send_first;
            p_mbx->waiting_list_send_first=  p_mbx->waiting_list_send_first->pNext;
            p_mbx->waiting_list_send_first->pPrev=NULL;
            if (!p_mbx->waiting_list_send_first)
            {
                p_mbx->waiting_list_send_last=NULL;
            }
            if (help_var->pTimeCB)
            {
                if (help_var->brrtk_task_Priority < MED_BND_RM)
                {
                    insert_TCB_in_Medium_list(help_var);
                }
                else
                {
                    insert_TCB_in_RM_list(help_var);
                }
            }
            else
            {
                if (help_var->brrtk_task_Priority > 0)
                {
                    insert_TCB_in_Medium_list(help_var);
                }
                else
                {
                    insert_TCB_in_EDF_list(help_var);
                }
            }
            help_var->brrtk_task_state=READY;
            setSchedulerStarted(); 
        }
        return 1;
    }
    else
    {
        return 0;
    }
}

long int  BR_Recieve_Mbx_wait (brrtk_MBXCBptr p_mbx, void * p_dest, unsigned long timeout_ms)
{
    brrtk_global_disable_interrupts();
    if (timeout_ms > 0)
    {
        p_mbx->timeout_lwIP=timeout_ms;
        add_mailbox_to_timeout_list(p_mbx);
    }
    while(1)
    {
        if (p_mbx->avaliable_space!=p_mbx->number_of_messages)
        {
            int i;
            for (i=0; i < p_mbx->message_lenght;i++ )
            {
                *((char*)p_dest+i)= *((char*)p_mbx->read_pointer+i);
            }
            p_mbx->avaliable_space++;
            if (p_mbx->read_pointer==p_mbx->mailbox_buffer_tail)
            {
                p_mbx->read_pointer=p_mbx->mailbox_buffer;
            }
            else
            {
                p_mbx->read_pointer=(char*)p_mbx->read_pointer+sizeof(char*);
            }
            if (p_mbx->waiting_list_send_first)
            {
                brrtk_TCBptr help_var=p_mbx->waiting_list_send_first;
                p_mbx->waiting_list_send_first=  p_mbx->waiting_list_send_first->pNext;
                p_mbx->waiting_list_send_first->pPrev=NULL;
                if (!p_mbx->waiting_list_send_first)
                {
                    p_mbx->waiting_list_send_last=NULL;
                }
                if (help_var->pTimeCB)
                {
                    if (help_var->brrtk_task_Priority < MED_BND_RM)
                    {
                        insert_TCB_in_Medium_list(help_var);
                    }
                    else
                    {
                        insert_TCB_in_RM_list(help_var);
                    }
                }
                else
                {
                    if (help_var->brrtk_task_Priority > 0)
                    {
                        insert_TCB_in_Medium_list(help_var);
                    }
                    else
                    {
                        insert_TCB_in_EDF_list(help_var);
                    }
                }
                help_var->brrtk_task_state=READY;
                setBlockingCall();
                brrtk_scheduler();
            }
            mbx_timeout_regular_refresh(p_mbx);
            brrtk_global_enable_interrupts(); 
            return p_mbx->timeout_lwIP;
        }
        else
        {
            long int temp;
            BR_Active_TCB->brrtk_task_state=WAITING;
            if (BR_Active_TCB->pTimeCB)
            {
                if (BR_Active_TCB->brrtk_task_Priority < MED_BND_RM)
                {
                    remove_TCB_medium_ready_list(BR_Active_TCB);
                }
                else
                {
                    remove_TCB_low_ready_list(BR_Active_TCB);
                }
            }
            else
            {
                if (BR_Active_TCB->brrtk_task_Priority > 0)
                {
                    remove_TCB_medium_ready_list(BR_Active_TCB);
                }
                else
                {
                    remove_TCB_Aperiodic_ready_list(BR_Active_TCB);
                }
            }
            ActiveTCB_recieve_mbx_wait(p_mbx);
            setBlockingCall();
            brrtk_scheduler(); 
            brrtk_global_disable_interrupts();
            temp= p_mbx->timeout_lwIP;
            if (temp == 0xffffffff)
            {
                return temp;
            }
            else if (temp != 0xffffffff)
            {
                /*refresh semaphore list waiting for timeout*/
            }
        }
    }
}


char Mailbox_timeouted(brrtk_MBXCBptr p_mbx)
{
    
    if (p_mbx->waiting_list_recieve_first)
    {
        brrtk_TCBptr help_var=p_mbx->waiting_list_recieve_first;
        p_mbx->waiting_list_recieve_first=  p_mbx->waiting_list_recieve_first->pNext;
        p_mbx->waiting_list_recieve_first->pPrev=NULL;
        if (!p_mbx->waiting_list_recieve_first)
        {
            p_mbx->waiting_list_recieve_last=NULL;
        }
        if (help_var->pTimeCB)
        {
            if (help_var->brrtk_task_Priority < MED_BND_RM)
            {
                insert_TCB_in_Medium_list(help_var);
            }
            else
            {
                insert_TCB_in_RM_list(help_var);
            }
        }
        else
        {
            if (help_var->brrtk_task_Priority > 0)
            {
                insert_TCB_in_Medium_list(help_var);
            }
            else
            {
                insert_TCB_in_EDF_list(help_var);
            }
        }
        help_var->brrtk_task_state=READY;
        setSchedulerStarted();
        return 1;
    }
    else
    {
        return 0; 
    }
}

/*
 * This file is part of the BRRTK source code.
 * 
 * Author: Bogdan Rosandic <bogdanrosandic@gmail.com.se>
 *
 */

#include "brrtk_app.h" 
#include "scheduler.h" 
#include "brrtk_utilities.h" 
#include "brrtk_platform.h" 


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>





/*---------------------------------------
    GLOBAL 
     VARIABLES
 ----------------------------------------*/

extern brrtk_TCBptr BR_Active_TCB;
extern brrtk_TCBptr BR_Ready_Periodic_List;
extern brrtk_TCBptr BR_Ready_Aperiodic_List;
extern brrtk_TCBptr BR_Ready_Medium_List;



void Scheduling(void){
      
    if (BR_Ready_Aperiodic_List)
    {//prednost imaju aperiodicni taskovi
      
        brrtk_TCBptr temp=BR_Ready_Aperiodic_List,temporary=BR_Ready_Aperiodic_List;
        temp=temp->pNext;
        while (temp)
        {
            if (temporary->pDelayCB->brrtk_task_deadline_counter > temp->pDelayCB->brrtk_task_deadline_counter)
            {//EDF algoritam
                temporary=temp;
            }
            temp=temp->pNext;
        }
        BR_Active_TCB=temporary;
    }
    else 
    {
        if (BR_Ready_Medium_List)
        {//isti princip kao za periodicne taskove
            brrtk_TCBptr temp=BR_Ready_Medium_List, temporary2=BR_Ready_Medium_List;
            while (temp->brrtk_task_state==SUSPENDED)
            {
                temp=temp->pNext;
                if (temp==NULL)break;
            }
            temporary2=temp;
            if (temporary2==NULL)
            {//svi u medium listi su suspendovani
                brrtk_TCBptr temp=BR_Ready_Periodic_List, temporary3;
                while (temp->brrtk_task_state==SUSPENDED)
                {
                    temp=temp->pNext;
                }
                temporary3=temp;
                temp=temp->pNext;
                //po izlasku iz petlje temp pokazuje na prvi ready task u listi periodicnih taskova
                while (temp)
                {
                    if (temp->brrtk_task_state==READY)
                    {
                        if (temporary3->brrtk_task_Priority > temp->brrtk_task_Priority)
                        {//RM algoritam
                            temporary3=temp;
                        }
                    } 
                    temp=temp->pNext;
                }
                BR_Active_TCB=temporary3;
            }
            else
            {// postoje READY taskovi u medium listi
                temp=temp->pNext;
                //po izlasku iz petlje temp pokazuje na prvi ready task u listi periodicnih taskova
                while (temp)
                {
                    if (temp->brrtk_task_state==READY)
                    {
                        if (temporary2->brrtk_task_Priority > temp->brrtk_task_Priority)
                        {//RM algoritam
                            temporary2=temp;
                        }
                    }
                    temp=temp->pNext;
                }
                BR_Active_TCB=temporary2;
            }
        }
        else
        {
            brrtk_TCBptr temp=BR_Ready_Periodic_List, temporary2=BR_Ready_Periodic_List;
            while (temp->brrtk_task_state==SUSPENDED)
            {
                temp=temp->pNext;
            }
            temporary2=temp;
            temp=temp->pNext;
            //po izlasku iz petlje temp pokazuje na prvi ready task u listi periodicnih taskova
            while (temp)
            {
                if (temp->brrtk_task_state==READY)
                {
                    if (temporary2->brrtk_task_Priority > temp->brrtk_task_Priority)
                    {//RM algoritam
                        temporary2=temp;
                    }
                }
                temp=temp->pNext;
            }
            BR_Active_TCB=temporary2;
        }
    }
}

void brrtk_scheduler(void)
{
    brrtk_global_disable_interrupts();
    resetContextSaved();
    resetTriggerReturnFromInterruptStarted();
    if (BR_Active_TCB)
    {
        if (brrtk_true == isBlockingCall())
        {
            BR_Active_TCB->brrtk_task_sp=save_context();
            resetBlockingCall();
        }
        else
        {
            BR_Active_TCB->brrtk_task_state=READY;//currently running task returns to ready state
        }
        BR_Active_TCB=NULL;
    }
    Scheduling();
    //BR_Ctxt_SW_count++;
    context_switch(BR_Active_TCB->brrtk_task_sp);
}


void BRRTK_Run(void)
{
    setBrrtkRunCalled();
    Scheduling();
    //BR_Ctxt_SW_count++;
    context_switch(BR_Active_TCB->brrtk_task_sp);
}


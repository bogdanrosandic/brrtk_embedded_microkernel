/*
 * This file is part of the BRRTK source code.
 * 
 * Author: Bogdan Rosandic <bogdanrosandic@gmail.com>
 *
 */

#include "brrtk_task.h"
#include "scheduler.h"
#include "brrtk_app.h"
#include "brrtk_platform.h"
#include "brrtk_utilities.h"
#include "stdio.h"
 
static TCB    BR_TCBArray[MAXTASKS+1];/* array to allocate all needed TCBs(extra one is for the idle)*/
static TimeCB BR_TimeCBArray[MAX_TIME_PER+1]; 
static DelayCB BR_DelayCBArray[MAX_TIME_APER];
static unsigned char BRRTK_nesting_level = 0;// initially there is no nesting!!

brrtk_TCBptr BR_FreeMem_TCB=NULL;//pointer to list of available TCB for allocation
brrtk_TimeCBptr BR_FreeMem_PeriodCB=NULL;
brrtk_DelayCBptr BR_FreeMem_AperiodCB=NULL;



brrtk_TCBptr BR_Ready_Periodic_List=NULL;//list of ready periodic tasks
brrtk_TCBptr BR_Ready_Aperiodic_List=NULL;//list of ready aperiodic tasks
brrtk_TCBptr BR_Ready_Medium_List=NULL;


brrtk_TCBptr BR_Active_TCB=NULL;		/* Pointer to currently running task  */
brrtk_TCBptr BR_Idle_TCB=NULL;
brrtk_TimeCBptr BR_TimeTCB_List=NULL;//
brrtk_DelayCBptr BR_DelayTCB_List=NULL;
unsigned char num_of_created_tasks=0;
unsigned char num_of_periodic_structures=0;
unsigned char num_of_aperiodic_structures=0;

long int IdleStk[256];
#if (SORT==0)

void insert_TCB_in_RM_list(brrtk_TCBptr current_TCB)
{
    current_TCB->pNext=BR_Ready_Periodic_List;//
    current_TCB->pPrev=NULL;
    BR_Ready_Periodic_List->pPrev=current_TCB;
    BR_Ready_Periodic_List=current_TCB;
}


void insert_TCB_in_Medium_list(brrtk_TCBptr current_TCB)
{
    current_TCB->pNext=BR_Ready_Medium_List;//
    current_TCB->pPrev=NULL;
    if(BR_Ready_Medium_List) BR_Ready_Medium_List->pPrev=current_TCB;
    BR_Ready_Medium_List=current_TCB;
}

void insert_TCB_in_EDF_list(brrtk_TCBptr current_TCB)
{
    current_TCB->pNext=BR_Ready_Aperiodic_List;//
    current_TCB->pPrev=NULL;
    BR_Ready_Aperiodic_List->pPrev=current_TCB;
    BR_Ready_Aperiodic_List=current_TCB; 
}
#endif

#if (SORT==1)

void insert_TCB_in_EDF_list(brrtk_TCBptr current_TCB)
{
    if (BR_Ready_Aperiodic_List==NULL)
    {
        //umetanje na poceetak liste
        BR_Ready_Aperiodic_List=current_TCB;
        BR_Ready_Aperiodic_List->pNext=NULL;
        BR_Ready_Aperiodic_List->pPrev=NULL;
    }
    else
    {
        brrtk_TCBptr temp=BR_Ready_Aperiodic_List;
        while(temp)
        {
            //kriterijum je manji deadline ide na pocetak liste
            if (current_TCB->pDelayCB->brrtk_task_deadline_counter > temp->pDelayCB->brrtk_task_deadline_counter)
            {//ako deadline stize posle deadlinea TCBa na koji trenutno twmp pokazuje
                //2) provera za sledeci task, temp=temp->pNext
                if (temp->pNext==NULL)
                {// ako je kraj liste onda vezi iza TCBa
                    temp->pNext=current_TCB;
                    current_TCB->pPrev=temp;
                    current_TCB->pNext=NULL;
                    temp=current_TCB;
                }
                temp=temp->pNext;//ako nije trazi dalje
            }
            else 
            {//ako je manje tikova ostalo do isteka deadlinea current_TCBa nego tempa
                //umece se current TCB u listu
                if (temp->pPrev==NULL)
                {//1)ako je temp->pPrev=NULL znaci da je temp na pocetku liste
                    temp->pPrev=current_TCB;
                    current_TCB->pNext=temp;
                    BR_Ready_Aperiodic_List=current_TCB;
                    current_TCB->pPrev=NULL;
                }
                else
                {//2)ako temp->pPrev!=NULL znaci da ispred temp ima neki TCB 
                    temp->pPrev->pNext=current_TCB;
                    current_TCB->pPrev=temp->pPrev;
                    current_TCB->pNext=temp;
                    temp->pPrev=current_TCB;
                }
                break;
            }
        }
    }
}

void insert_TCB_in_RM_list(brrtk_TCBptr current_TCB)
{
    if (BR_Ready_Periodic_List==NULL)
    {
        //insert at the beggining of the list 
        BR_Ready_Periodic_List=current_TCB;
        BR_Ready_Periodic_List->pNext=NULL;
        BR_Ready_Periodic_List->pPrev=NULL;
    }
    else
    {
        brrtk_TCBptr temp=BR_Ready_Periodic_List;
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
                    BR_Ready_Periodic_List=current_TCB;
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
}

void insert_TCB_in_Medium_list(brrtk_TCBptr current_TCB){
    if (BR_Ready_Medium_List==NULL)
    {
        
        BR_Ready_Medium_List=current_TCB;
        BR_Ready_Medium_List->pNext=NULL;
        BR_Ready_Medium_List->pPrev=NULL;
    }
     else
     {
        brrtk_TCBptr temp=BR_Ready_Medium_List;
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
                    BR_Ready_Medium_List=current_TCB;
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
}
#endif 

void remove_TCB_medium_ready_list(brrtk_TCBptr current_TCB)
{ 
    if (current_TCB->pPrev==NULL)
    {
        BR_Ready_Medium_List=current_TCB->pNext;
        if (BR_Ready_Medium_List!=NULL)
        { 
            BR_Ready_Medium_List->pPrev=NULL;
        }  
    }
    else
    {
        current_TCB->pPrev->pNext=current_TCB->pNext;
        current_TCB->pNext->pPrev= current_TCB->pPrev; 
    }
}
void remove_TCB_Aperiodic_ready_list(brrtk_TCBptr current_TCB)
{ 
    if (current_TCB->pPrev==NULL)
    {
        BR_Ready_Aperiodic_List=current_TCB->pNext;
        if (BR_Ready_Aperiodic_List!=NULL)
        {
            BR_Ready_Aperiodic_List->pPrev=NULL;
        }  
    }
    else
    {//ako nije prvi TCB u listi
        current_TCB->pPrev->pNext=current_TCB->pNext;
        current_TCB->pNext->pPrev= current_TCB->pPrev; 
    }
}

void remove_TCB_low_ready_list(brrtk_TCBptr current_TCB)
{ 
    if (current_TCB->pPrev==NULL)
    {
        BR_Ready_Periodic_List=current_TCB->pNext;
        if (BR_Ready_Periodic_List!=NULL)
        {
            BR_Ready_Periodic_List->pPrev=NULL;
        }  
    }
    else
    {
        current_TCB->pPrev->pNext=current_TCB->pNext;
        current_TCB->pNext->pPrev= current_TCB->pPrev; 
    }
}


void  free_PeriodCB(brrtk_TimeCBptr pokazivac)
{
    if (pokazivac==BR_TimeTCB_List)
    {
        BR_TimeTCB_List=pokazivac->pNext;
          BR_TimeTCB_List->pPrev=NULL;
    }
    else
    {
        pokazivac->pPrev->pNext=pokazivac->pNext;
        pokazivac->pNext->pPrev=pokazivac->pPrev;
    }
    pokazivac->pNext=BR_FreeMem_PeriodCB;//vrati u FreeMem PeriodCB
    BR_FreeMem_PeriodCB=pokazivac;
}
void free_AperiodCB(brrtk_DelayCBptr pokazivac)
{
    if (pokazivac==BR_DelayTCB_List)
    {
        BR_DelayTCB_List=pokazivac->pNext;
        BR_DelayTCB_List->pPrev=NULL;
    }
    else
    {
        pokazivac->pPrev->pNext=pokazivac->pNext;
        pokazivac->pNext->pPrev=pokazivac->pPrev;
    }
    pokazivac->pNext=BR_FreeMem_AperiodCB;
    BR_FreeMem_AperiodCB=pokazivac;
}



void BRRTK_Initialize(void)
{ 

    brrtk_TCBptr pomoc;
    int i;
    for(i = 0; i<(MAXTASKS+1);i++)
    {
        if (!BR_FreeMem_TCB)
        {
            BR_FreeMem_TCB=&BR_TCBArray[i];
            pomoc=BR_FreeMem_TCB;
        }
        else
        {
            pomoc->pNext=&BR_TCBArray[i];
            pomoc=pomoc->pNext;
        }
    }
  // initializememory pool for allocation of PeridCB
    {
        brrtk_TimeCBptr pomoc;
        for(i = 0; i<(MAX_TIME_PER+1);i++)
        {
            if (!BR_FreeMem_PeriodCB)
            {
                BR_FreeMem_PeriodCB=&BR_TimeCBArray[i];
                pomoc=BR_FreeMem_PeriodCB;
            }
            else
            {
                pomoc->pNext=&BR_TimeCBArray[i];
                pomoc=pomoc->pNext;
            }
        }
    }
  //  initializememory pool for allocation of DelayCB
    {
        brrtk_DelayCBptr pomoc;
        for(i = 0; i<(MAX_TIME_APER);i++)
        {
            if (!BR_FreeMem_AperiodCB)
            {
                BR_FreeMem_AperiodCB=&BR_DelayCBArray[i];
                pomoc=BR_FreeMem_AperiodCB;
            }
            else
            {
                pomoc->pNext=&BR_DelayCBArray[i];
                pomoc=pomoc->pNext;
            }
        }
   }
  //creates and initialize Idle Task
    {
    brrtk_TCBptr pomoc=BR_FreeMem_TCB;
    BR_FreeMem_TCB=pomoc->pNext;
    
    pomoc->brrtk_task_id='0';
    pomoc->brrtk_task_pc=Idle_Task;
    pomoc->brrtk_task_sp=initialize_stack_idle(  (long int) (&Idle_Task),&IdleStk[256] );
    pomoc->brrtk_task_state=READY;
    pomoc->brrtk_task_Priority=255;
    BR_Ready_Periodic_List=pomoc;
    pomoc->pPrev=NULL;
    pomoc->pNext=NULL;
    BR_Idle_TCB=BR_Ready_Periodic_List;
    num_of_created_tasks++;
    num_of_periodic_structures++;
    }
}



brrtk_TCBptr BRRTK_CreateNewTask_aperiodic(void (*task)(void * arg  ), void *  ptos, brrtk_DelayCBptr * aper_timer ,unsigned int deadline_cnt)
{ 
    brrtk_TCBptr pomoc;
    brrtk_DelayCBptr pomoc_vreme;
    brrtk_global_disable_interrupts();
    pomoc=BR_FreeMem_TCB;
    BR_FreeMem_TCB=pomoc->pNext;
    pomoc->brrtk_task_pc=task;
    pomoc->brrtk_task_sp=ptos;
    pomoc->brrtk_task_sp=initialize_stack(  (int) (task),ptos );
    pomoc->brrtk_task_state=READY;
    pomoc->brrtk_task_Priority=0;
    
    pomoc->pTimeCB=NULL;
    
    pomoc_vreme=BR_FreeMem_AperiodCB;
    BR_FreeMem_AperiodCB=pomoc_vreme->pNext;  

    pomoc->pDelayCB=pomoc_vreme;
    (*aper_timer)=pomoc->pDelayCB;
    
    pomoc_vreme->pTCB=pomoc;
    pomoc_vreme->delay=0;
    pomoc_vreme->counter_startovanja=0;
    pomoc_vreme->brrtk_task_deadline_init=deadline_cnt;
    pomoc_vreme->brrtk_task_deadline_counter=deadline_cnt;
    pomoc_vreme->brrtk_task_deadlines_counter=0;
    
     
    insert_TCB_in_EDF_list(pomoc);
    
    num_of_created_tasks++;
    num_of_aperiodic_structures++;
    if (brrtk_true == isBrrtkRunCalled())
    {
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
    return pomoc;
}


brrtk_TCBptr BRRTK_CreateNewTask_aperiodic_medium(void (*task)(void * arg ), void *  ptos, int Priority, brrtk_DelayCBptr * aper_timer ,unsigned int deadline_cnt)
{ 
    brrtk_TCBptr pomoc;
    brrtk_DelayCBptr pomoc_vreme;
    brrtk_global_disable_interrupts();
    pomoc=BR_FreeMem_TCB;
    BR_FreeMem_TCB=pomoc->pNext;

    pomoc->brrtk_task_pc=task;
    pomoc->brrtk_task_sp=ptos;
    pomoc->brrtk_task_sp=initialize_stack(  (int) (task),ptos );
    pomoc->brrtk_task_state=READY;
    pomoc->brrtk_task_Priority=Priority;
      
    
    
    pomoc->pTimeCB=NULL;
    pomoc_vreme=BR_FreeMem_AperiodCB;
    BR_FreeMem_AperiodCB=pomoc_vreme->pNext;
    
    pomoc->pDelayCB=pomoc_vreme;
    (*aper_timer)=pomoc->pDelayCB;
    
    
    
    pomoc_vreme->pTCB=pomoc;//&BR_TCBArray[num_of_created_tasks];
    pomoc_vreme->delay=0;
    pomoc_vreme->counter_startovanja=0;
    pomoc_vreme->brrtk_task_deadline_init=deadline_cnt;
    pomoc_vreme->brrtk_task_deadline_counter=deadline_cnt;
    pomoc_vreme->brrtk_task_deadlines_counter=0;
    
    
     insert_TCB_in_Medium_list(pomoc);
    
    
    num_of_created_tasks++;
    num_of_aperiodic_structures++;
    if (brrtk_true == isBrrtkRunCalled())
    {
        if (isInterruptEntered())
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
    return pomoc;
}
brrtk_TCBptr BRRTK_CreateNewTask_periodic(void (*task)(void * arg ), void *  ptos, int Priority,unsigned int rdy_cnt)
{ 
    brrtk_TCBptr pomoc;
    brrtk_TimeCBptr pomoc_vreme;
    brrtk_global_disable_interrupts();
    pomoc=BR_FreeMem_TCB;
    BR_FreeMem_TCB=pomoc->pNext;
    
    pomoc->brrtk_task_pc=task;
    pomoc->brrtk_task_sp=ptos;
    pomoc->brrtk_task_sp=initialize_stack(  (long int) (task),ptos );
    pomoc->brrtk_task_state=READY;
    pomoc->brrtk_task_Priority=Priority;
      
    pomoc->pDelayCB=NULL;
    
    pomoc_vreme=BR_FreeMem_PeriodCB;
    BR_FreeMem_PeriodCB=pomoc_vreme->pNext;
    pomoc->pTimeCB=pomoc_vreme;
        
    
    pomoc_vreme->pTCB=pomoc;//&BR_TCBArray[num_of_created_tasks];
    pomoc_vreme->brrtk_task_ready_counter_init=rdy_cnt;
    pomoc_vreme->brrtk_task_ready_counter=rdy_cnt;
    pomoc_vreme->brrtk_task_deadlines_counter=0;
        
    
    if (BR_TimeTCB_List==NULL){
        BR_TimeTCB_List=pomoc_vreme;
        BR_TimeTCB_List->pNext=NULL;
        BR_TimeTCB_List->pPrev=NULL;
    }
    else{
        pomoc_vreme->pNext=BR_TimeTCB_List;
        BR_TimeTCB_List->pPrev=pomoc_vreme;
        BR_TimeTCB_List=pomoc_vreme;
        pomoc_vreme->pPrev=NULL;
    }
       
    if (pomoc->brrtk_task_Priority < MED_BND_RM){
        insert_TCB_in_Medium_list(pomoc);
    }
    else{
      insert_TCB_in_RM_list(pomoc);
    }
    
    num_of_created_tasks++;
    num_of_periodic_structures++;
    num_of_created_tasks++;
    num_of_aperiodic_structures++;
    if (brrtk_true == isBrrtkRunCalled())
    {
        if (isInterruptEntered())
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
  
    return pomoc; 
}

void Terminate_Running_Task(void)
{
    brrtk_global_disable_interrupts();
    if (BR_Active_TCB->brrtk_task_Priority == 0 )
    {
        remove_TCB_Aperiodic_ready_list(BR_Active_TCB);
        if (BR_Active_TCB->pDelayCB->counter_startovanja)
        {
     
            free_AperiodCB(BR_Active_TCB->pDelayCB);
        }
        else
        {
            BR_Active_TCB->pDelayCB->pNext=BR_FreeMem_AperiodCB;
            BR_FreeMem_AperiodCB=BR_Active_TCB->pDelayCB;
        }
    }
    else
    {
        if (BR_Active_TCB->brrtk_task_Priority < MED_BND_RM)
        {
            remove_TCB_medium_ready_list(BR_Active_TCB);
            if (BR_Active_TCB->pTimeCB)
            {
                
                free_PeriodCB(BR_Active_TCB->pTimeCB);
            }
            else
            {
                if (BR_Active_TCB->pDelayCB->counter_startovanja)
                {
                    
                    free_AperiodCB(BR_Active_TCB->pDelayCB);
                }
                else
                {
                    BR_Active_TCB->pDelayCB->pNext=BR_FreeMem_AperiodCB;
                    BR_FreeMem_AperiodCB=BR_Active_TCB->pDelayCB;
                }
            }
        }
        else
        {
            remove_TCB_low_ready_list(BR_Active_TCB);
            
            free_PeriodCB(BR_Active_TCB->pTimeCB);
        }
    }
    
    if (!BR_FreeMem_TCB)
    {
        BR_FreeMem_TCB=BR_Active_TCB;
        BR_Active_TCB->pPrev=NULL;
        BR_Active_TCB->pNext=NULL;
    }
    else
    {
        BR_Active_TCB->pNext = BR_FreeMem_TCB;
        BR_FreeMem_TCB = BR_Active_TCB;
        BR_Active_TCB->pPrev=NULL;
    }
    brrtk_scheduler();
}


brrtk_TCBptr BRRTK_Current_Task(void){
  
  return BR_Active_TCB;
  
}

void BRRTK_Enter_Critical(void)
{
    brrtk_global_disable_interrupts();
}
void BRRTK_Leave_Critical(void)
{
    brrtk_global_enable_interrupts();
}

void BRRTK_Task_Suspend(void)
{
    brrtk_global_disable_interrupts();
    if (BR_Active_TCB->pTimeCB->brrtk_task_deadlines_counter==0)
    {
        BR_Active_TCB->brrtk_task_state=SUSPENDED;// menja se stanje
    }
    else
    {
        BR_Active_TCB->pTimeCB->brrtk_task_deadlines_counter--;
    }
    setBlockingCall();
    brrtk_scheduler();
    brrtk_global_enable_interrupts();
}






void BRRTK_EnterInt(void)
{
    brrtk_global_disable_interrupts();
    setInterruptEntered();

#if (ALLOWED_NESTING_LEVEL >1)

    if (BRRTK_nesting_level < ALLOWED_NESTING_LEVEL)
    {
        BRRTK_nesting_level++;
        brrtk_global_enable_interrupts();
    }

#endif
}
void BRRTK_LeaveInt_sched(void)
{
    brrtk_global_disable_interrupts();
    if ((brrtk_true == isSchedulerStarted()) && (brrtk_false == isTriggerReturnFromInterruptStarted()))
    {
#if (ALLOWED_NESTING_LEVEL >1)

        
        if (BRRTK_nesting_level > 0)
        {
            BRRTK_nesting_level--;
            brrtk_global_enable_interrupts();
        }
#endif
        brrtk_TriggerReturnFromInterruptHandler();
        brrtk_global_enable_interrupts();
    }
    else
    {
        resetInterruptEntered();
        brrtk_global_enable_interrupts();
    }
}
void BRRTK_ReturnFromInterruptHandler(void)
{
      brrtk_global_disable_interrupts();
      resetInterruptEntered();
      resetSchedulerStarted();
      if (brrtk_false == isContextSaved())
      {
           BR_Active_TCB->brrtk_task_sp=save_context_ISR();
           setContextSaved();
           go_to_scheduler((int)&brrtk_scheduler);
      }
      brrtk_global_enable_interrupts();
}
void BRRTK_LeaveInt_no_sched(void)
{
    resetInterruptEntered();
}

void BRRTK_StartAperiodicTask(brrtk_DelayCBptr aper_tajmer)
{
      
    brrtk_global_disable_interrupts();
    ++aper_tajmer->counter_startovanja;
    if( aper_tajmer->counter_startovanja==1)
    {
        aper_tajmer->pNext=BR_DelayTCB_List;
        if (BR_DelayTCB_List) BR_DelayTCB_List->pPrev=aper_tajmer;
        BR_DelayTCB_List=aper_tajmer;
        aper_tajmer->pPrev=NULL;
    }
    if (brrtk_false == isInterruptEntered)
    {
        brrtk_global_enable_interrupts();
    }
}
void BRRTK_StopAperiodicTask(brrtk_DelayCBptr aper_tajmer)
{
    brrtk_global_disable_interrupts();
    aper_tajmer->brrtk_task_deadline_counter=aper_tajmer->brrtk_task_deadline_init;
    --aper_tajmer->counter_startovanja;
    if (aper_tajmer->counter_startovanja==0)
    {
        if (aper_tajmer->pNext)
        {
            aper_tajmer->pNext->pPrev=aper_tajmer->pPrev;
        }
        if (aper_tajmer->pPrev)
        {
          aper_tajmer->pPrev->pNext=aper_tajmer->pNext;
        }
        else
        {
            BR_DelayTCB_List=aper_tajmer->pNext;
        }
        aper_tajmer->pPrev=NULL;
        aper_tajmer->pNext=NULL;
    }
    else
    {
        
        setBlockingCall();
        brrtk_scheduler();
    }
    brrtk_global_enable_interrupts();
}

brrtk_bool Tick_Handler(void)
{
    brrtk_bool change=brrtk_false;
    
    brrtk_TimeCBptr temp=BR_TimeTCB_List;
    brrtk_DelayCBptr temp_aper_ready=BR_DelayTCB_List;
    unsigned int max_prioritet=255;
    if (BR_Active_TCB) max_prioritet=BR_Active_TCB->brrtk_task_Priority; 
      /////////////////////////////////////////////////////////////////////
    while (temp)
    {
        --temp->brrtk_task_ready_counter;
        if(temp->brrtk_task_ready_counter==0)
        {
            temp->brrtk_task_ready_counter=temp->brrtk_task_ready_counter_init;
            if (temp->pTCB->brrtk_task_state==SUSPENDED)
            {
                temp->pTCB->brrtk_task_state=READY;
                
                if (temp->pTCB->brrtk_task_Priority < max_prioritet )
                
                {
                    change=brrtk_true;
                }
            } 
            else
            {           
                temp->brrtk_task_deadlines_counter++;
            }
        }
        temp=temp->pNext;
    }

    while (temp_aper_ready)
    {
        if (temp_aper_ready->brrtk_task_deadline_counter!=0)
        {
            if (--temp_aper_ready->brrtk_task_deadline_counter==0)
            {
                temp_aper_ready->brrtk_task_deadlines_counter++;                                      
            }
        }
        temp_aper_ready=temp_aper_ready->pNext;
    }
    if (BR_Active_TCB->pDelayCB)
    {
        if (BR_Active_TCB->brrtk_task_Priority == 0)
        {
            change=brrtk_false;
        }
    }
    return change;
}


void BRRTK_SystemTick_Handler (void)
{
    
    //BR_TickNum++;
    if (brrtk_true==Tick_Handler() /*|| sem_timeout_tick_refresh()==1 || mbx_timeout_tick_refresh()==1*/)
    {
        setSchedulerStarted();
    }
    
}



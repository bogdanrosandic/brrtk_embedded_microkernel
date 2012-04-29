#include "brrtk_platform.h"


/*---------------------------------------
    This function globaly disables interrupts
 ----------------------------------------*/

void brrtk_global_disable_interrupts(void)
{
      IntMasterDisable();
}
/*---------------------------------------
    This function globaly enables interrupts
 ----------------------------------------*/
void brrtk_global_enable_interrupts(void)
{
      IntMasterEnable();
}

void brrtk_TriggerReturnFromInterruptHandler(void)
{
      //*(portNVIC_INT_CTRL) = portNVIC_PENDSVSET;
}



/*void * save_context(void)
{
    
}*/

/*int * initialize_stack_idle (int,void *)
{
    
}*/

/*---------------------------------------
    This function initialize stack of regular  task
 ----------------------------------------*/
/*void * initialize_stack(int,void *)
{
    
}

void go_to_scheduler(int)
{
    
}

void * save_context_ISR(int)
{
    
}*/


/*---------------------------------------
    This function represents Idle_Task task routine.
 ----------------------------------------*/
void Idle_Task(void * arg)
{
    
}


/*---------------------------------------
    This function should initialize all neccessary hardware.
 ----------------------------------------*/

void Hardware_Init(void )
{
    
}




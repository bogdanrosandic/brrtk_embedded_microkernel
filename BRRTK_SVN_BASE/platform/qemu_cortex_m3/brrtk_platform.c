/*

 * This file is part of the BRRTK source code.
 * 
 * Author: Bogdan Rosandic <bogdanrosandic@gmail.com>
 *
 */

#include "brrtk_platform.h"

#define portNVIC_PENDSVSET			0x10000000
#define portNVIC_INT_CTRL			( ( volatile unsigned long *) 0xe000ed04 )
/*---------------------------------------
    This function globaly disables interrupts
 ----------------------------------------*/

void brrtk_global_disable_interrupts(void)
{
     
}
/*---------------------------------------
    This function globaly enables interrupts
 ----------------------------------------*/
void brrtk_global_enable_interrupts(void)
{

}

void brrtk_TriggerReturnFromInterruptHandler(void)
{


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
  while(1)
  {

  }
}


/*---------------------------------------
    This function should initialize all neccessary hardware.
 ----------------------------------------*/

void Hardware_Init(void )
{
 
}
unsigned int Num_of_ticks;
void SysTick_Handler(void)
{
}

void PendSV_handler(void)
{

} 

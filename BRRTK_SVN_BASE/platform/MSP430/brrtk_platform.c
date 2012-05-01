/*

 * This file is part of the BRRTK source code.
 * 
 * Author: Bogdan Rosandic <bogdanrosandic@gmail.com>
 *
 */

#include "brrtk_platform.h"
#include "brrtk_task.h"
#include  <msp430x44x.h>
#include "brrtk_task.h"
/*---------------------------------------
    This function globaly disables interrupts
 ----------------------------------------*/

void brrtk_global_disable_interrupts(void)
{
    __bic_SR_register( GIE); 
}
/*---------------------------------------
    This function globaly enables interrupts
 ----------------------------------------*/
void brrtk_global_enable_interrupts(void)
{
    __bis_SR_register( GIE); 
}

void brrtk_TriggerReturnFromInterruptHandler(void)
{
    BRRTK_ReturnFromInterruptHandler();
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
    // Stop watchdog timer to prevent time out reset
   
    WDTCTL = WDTPW + WDTHOLD; 
   //BCSCTL1 = CALBC1_16MHZ;// Set DCO to 16MHz
    //DCOCTL = CALDCO_16MHZ;
    TACCTL0 = CCIE;// TACCR0 interrupt enabled
    TACCR0 = 16000;// 16MHz 16000 tick  1ms: 8000  tick  0.5ms : 4000 -> 0.25ms
    TACTL = TASSEL_2 + MC_1; //MC_1-> up:SMCLK, contmode
}

unsigned int Num_of_ticks;
// Timer A0 interrupt service routine
#pragma vector=TIMERA0_VECTOR
__interrupt void Timer_A0 (void)
{
   BRRTK_EnterInt();
   Num_of_ticks++;
   BRRTK_SystemTick_Handler();
   BRRTK_LeaveInt_sched();
}


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

      *(portNVIC_INT_CTRL) = portNVIC_PENDSVSET;
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
      IntMasterEnable();
  }
}


/*---------------------------------------
    This function should initialize all neccessary hardware.
 ----------------------------------------*/

void Hardware_Init(void )
{
 
    SysCtlClockSet(SYSCTL_SYSDIV_1 |  SYSCTL_USE_OSC | SYSCTL_OSC_MAIN |
                   SYSCTL_XTAL_8MHZ);//50Mhz  SYSCTL_USE_OSC.
    
   
    IntMasterEnable();
 
 
  
    SysTickDisable();//
    SysTickPeriodSet(SysCtlClockGet()/1000);// tick 1ms
    //!!!!!!!!!!!!!!!!!!!!!!!!! SysTickIntRegister(&SysTick_Handler);
    SysTickIntEnable();
    SysTickEnable();//
  
    IntPriorityGroupingSet(3);//moguce gnezdenje prekida, sva tri bita dostupna za podesavanje prekida
    //IntPriorityGroupingSet(0);// nema gnezdenja prekida
    IntPriorityGroupingGet();
    //IntPrioritySet(INT_ETH, 5<<5);
    IntPrioritySet(FAULT_USAGE,(2<<5));// pomera se za pet mesta u levo jer su samo najvisa tri bita dostupna za podesavanje prioriteta
    IntPrioritySet(FAULT_BUS,(2<<5));
    IntPrioritySet(FAULT_SVCALL,(3<<5));
    IntPrioritySet(FAULT_SYSTICK ,(6<<5));
    IntPrioritySet(FAULT_PENDSV,(7<<5));
    
    IntEnable(FAULT_PENDSV);
    IntEnable(FAULT_BUS);
    IntEnable(FAULT_USAGE);
    IntEnable(FAULT_SYSTICK);
}
unsigned int Num_of_ticks;
void SysTick_Handler(void)
{
    BRRTK_EnterInt();
    Num_of_ticks++;
    BRRTK_SystemTick_Handler();
    BRRTK_LeaveInt_sched();
}

void PendSV_handler(void)
{
    BRRTK_ReturnFromInterruptHandler();
} 

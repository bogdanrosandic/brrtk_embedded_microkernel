/*

 * This file is part of the BRRTK source code.
 * 
 * Author: Bogdan Rosandic <bogdanrosandic@gmail.com.se>
 *
 */


#ifndef __BRRTK_PLATFORM_H__
#define __BRRTK_PLATFORM_H__

#include "brrtk_app.h"
#include "brrtk_task.h"
#ifdef LM3S8962_ARM_CORTEX_M3
#include "inc/hw_types.h"
#include "driverlib/interrupt.h"
#include "inc/lm3s8962.h"
#include "driverlib/systick.h"
#include "driverlib/sysctl.h"
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#endif


/*---------------------------------------
    This file should  include  code 
    or definitions that are specific for platform used
 ----------------------------------------*/


/*---------------------------------------
    This function globaly disables interrupts on
     specific  platform 
 ----------------------------------------*/

void brrtk_global_disable_interrupts(void);


/*---------------------------------------
    This function globaly enables interrupts on
     specific  platform 
 ----------------------------------------*/

void brrtk_global_enable_interrupts(void);


/*---------------------------------------
    This function trigers return from interrupt handler
    This triger should be platform dependent!!
 ----------------------------------------*/
void brrtk_TriggerReturnFromInterruptHandler(void);

/*---------------------------------------
    This function initialize stack of idle task
    Ussually it should be defined in 
    brrtk_platform_asm.s
 ----------------------------------------*/
int * initialize_stack_idle (int,void *);

/*---------------------------------------
    This function initialize stack of regular  task
    Ussually it should be defined in 
    brrtk_platform_asm.s
 ----------------------------------------*/

void * initialize_stack(int,void *);

/*---------------------------------------
    This function force scheduler start.
    Ussually it should be defined in 
    brrtk_platform_asm.s. As an argument it always
    take pointer to scheduler routine!!!!
 ----------------------------------------*/
void go_to_scheduler(int);

/*---------------------------------------
    This function saves context and it is called
    from interrupt routine.
    Ussually it should be defined in 
    brrtk_platform_asm.s.
 ----------------------------------------*/
void * save_context_ISR(/*int*/);

/*---------------------------------------
    This function saves context.
    Ussually it should be defined in 
    brrtk_platform_asm.s. 
 ----------------------------------------*/
void * save_context(void);

/*---------------------------------------
    This function forces context switch.
    Ussually it should be defined in 
    brrtk_platform_asm.s. 
 ----------------------------------------*/
void * context_switch(void *);

/*---------------------------------------
    This function represents Idle_Task task routine.
 ----------------------------------------*/
void Idle_Task(void *);

void Hardware_Init(void );


#endif /* __BRRTK_PLATFORM_H__ */



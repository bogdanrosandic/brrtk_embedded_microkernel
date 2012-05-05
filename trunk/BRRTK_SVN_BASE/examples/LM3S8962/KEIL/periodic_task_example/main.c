/*

 * This file is application example for usage of the BRRTK source code.
 * 
 * Author: Bogdan Rosandic <bogdanrosandic@gmail.com>
 *
 */

/*TARGET SPECIFIC--> LM3S8962*/
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"

#include "driverlib/systick.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"
/*TARGET SPECIFIC--> LM3S8962*/

#include "brrtk_platform.h"
#include "brrtk_task.h"
#include "scheduler.h"

#define P1STACKSIZE 64         /* Size of task's stack in words */
#define P2STACKSIZE 64
#define P3STACKSIZE 64

long int P1Stk[P1STACKSIZE];           /* Space for each task's stack  */
long int P2Stk[P2STACKSIZE];
long int P3Stk[P3STACKSIZE];

void P1Task(void * arg);               /* Function prototypes for task code */
void P2Task(void * arg);
void P3Task(void * arg);

int main( void )
{
      
    Hardware_Init();
    BRRTK_Initialize();
    BRRTK_CreateNewTask_periodic(P1Task,  &P1Stk[P1STACKSIZE], 1,5);
    BRRTK_CreateNewTask_periodic(P2Task,  &P2Stk[P2STACKSIZE], 2,10);
    BRRTK_CreateNewTask_periodic(P3Task,  &P3Stk[P3STACKSIZE], 3,20);
    BRRTK_Run();
    return 0;
}



void P1Task(void * arg)
{
    while (1)
    { 
        BRRTK_Task_Suspend();
    }
}

void P2Task(void * arg)
{
    while (1)
    { 
        BRRTK_Task_Suspend();
    }
}
void P3Task(void * arg)
{
    while (1)
    { 
        BRRTK_Task_Suspend();
    }
}





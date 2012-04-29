
/*TARGET SPECIFIC--> MSP430*/
#include  <msp430x44x.h>
/*TARGET SPECIFIC--> MSP430*/

#include <brrtk_platform.h>
#include <brrtk_task.h>
#include <scheduler.h>

#define P1STACKSIZE 55         /* Size of task's stack in words */
#define P2STACKSIZE 55
#define P3STACKSIZE 55

long int P1Stk[P1STACKSIZE];           /* Space for each task's stack  */
long int P2Stk[P2STACKSIZE];
long int P3Stk[P3STACKSIZE];

void P1Task(void * arg);               /* Function prototypes for task code */
void P2Task(void * arg);
void P3Task(void * arg);

void main( void )
{
      
    Hardware_Init();
    BRRTK_Initialize();
    BRRTK_CreateNewTask_periodic(P1Task,  &P1Stk[P1STACKSIZE], 1,5);
    BRRTK_CreateNewTask_periodic(P2Task,  &P2Stk[P2STACKSIZE], 2,10);
    BRRTK_CreateNewTask_periodic(P3Task,  &P3Stk[P3STACKSIZE], 3,20);
    BRRTK_Run();
    return; 
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





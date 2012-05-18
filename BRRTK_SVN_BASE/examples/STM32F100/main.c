/*

 * This file is application example for usage of the BRRTK source code.
 * 
 * Author: Bogdan Rosandic <bogdanrosandic@gmail.com>
 *
 */

/*TARGET SPECIFIC--> LM3S8962*/
//#include "inc/hw_types.h"
//#include "inc/hw_memmap.h"
//#include "inc/hw_ints.h"

//#include "driverlib/systick.h"
//#include "driverlib/sysctl.h"
//#include "driverlib/interrupt.h"
//#include "driverlib/timer.h"
/*TARGET SPECIFIC--> LM3S8962*/

/*TARGET SPECIFIC--> STM32F*/
#include "misc.h"
#include "stm32f10x.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_sdio.h"
//#include <yfuns.h>
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

void main( void )
{
      
    Hardware_Init();
    BRRTK_Initialize();
 //   BRRTK_CreateNewTask_periodic(P1Task,  &P1Stk[P1STACKSIZE], 1,5);
 //   BRRTK_CreateNewTask_periodic(P2Task,  &P2Stk[P2STACKSIZE], 2,10);
 //   BRRTK_CreateNewTask_periodic(P3Task,  &P3Stk[P3STACKSIZE], 3,20);
  //  printf("\n\[BTR]Hardware_Init-> line52 (main.c)RUN\n\r");
    BRRTK_Run();
    
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
#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */

/**
  * @}
  */




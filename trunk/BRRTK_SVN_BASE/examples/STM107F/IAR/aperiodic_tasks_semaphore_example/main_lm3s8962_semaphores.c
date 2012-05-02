/*

 * This file is application example for usage of the BRRTK source code.
 * 
 * Author: Bogdan Rosandic <bogdanrosandic@gmail.com>
 *
 */
//main semaphore test
/*TARGET SPECIFIC--> LM3S8962*/
#include "driverlib/debug.h"
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"

#include "driverlib/gpio.h"
#include <stdio.h>
#include "driverlib/uart.h"
#include "driverlib/systick.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"
/*TARGET SPECIFIC--> LM3S8962*/

/*BRRTK headers*/
#include "brrtk_platform.h"
#include "brrtk_task.h"
#include "semaphore.h"
#include "scheduler.h"

void uart_init(void){
    //
    // Enable the peripherals used by this example.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    //
    // Set GPIO A0 and A1 as UART pins.
    //
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    //
    // Configure the UART for 115,200, 8-N-1 operation.
    //
    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200,
                        (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                         UART_CONFIG_PAR_NONE));

    //
    // Enable the UART interrupt.????
    //
    //IntEnable(INT_UART0);
    //UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);
}

void print_uart(char * poruka){
  //char[] * p_poruka=&poruka;
  for (int i=0; *(poruka+i)!='\0'; i++){
    UARTCharPut(UART0_BASE,*(poruka+i));
  }
}



#define P1STACKSIZE 64         /* Size of task's stack in words */
#define P2STACKSIZE 64
#define P3STACKSIZE 64

long int P1Stk[P1STACKSIZE];           /* Space for each task's stack  */
long int P2Stk[P2STACKSIZE];
long int P3Stk[P3STACKSIZE];

void P1Task(void * arg);               /* Function prototypes for task code */
void P2Task(void * arg);
void P3Task(void * arg);

brrtk_DelayCBptr delayP1=NULL,delayP2=NULL,delayP3=NULL;
SBCB sem_p1,sem_p2,sem_p3,sem_p1_p2;
void main( void )
{
      
    Hardware_Init();
    uart_init();
    BRRTK_Initialize();
    BRRTK_Create_Cnt_Sem(&sem_p1_p2,0);
    BRRTK_Create_Cnt_Sem(&sem_p1,0);
    BRRTK_Create_Cnt_Sem(&sem_p2,0);
    BRRTK_Create_Cnt_Sem(&sem_p3,1);
    BRRTK_CreateNewTask_aperiodic_medium(P1Task, &P1Stk[P1STACKSIZE], 1, &delayP1 ,5);
    BRRTK_CreateNewTask_aperiodic_medium(P2Task, &P2Stk[P2STACKSIZE], 2, &delayP2 ,5);
    BRRTK_CreateNewTask_aperiodic_medium(P3Task, &P3Stk[P3STACKSIZE], 3, &delayP3 ,5);
    BRRTK_Run();
    
}



void P1Task(void * arg)
{
    while (1)
    { 
         BRRTK_Lock_Cnt_Sem(&sem_p1_p2);
         print_uart("the last ");
         BRRTK_Release_Cnt_Sem(&sem_p1_p2);
         BRRTK_Lock_Cnt_Sem(&sem_p1);
         print_uart("Barry White\r\n");
         BRRTK_Release_Cnt_Sem(&sem_p3);
    }
}

void P2Task(void * arg)
{
    while (1)
    { 
       BRRTK_Lock_Cnt_Sem(&sem_p1_p2);
       print_uart("my everything - ");
       BRRTK_Release_Cnt_Sem(&sem_p1);
    }
}
void P3Task(void * arg)
{
    while (1)
    { 
        BRRTK_Lock_Cnt_Sem(&sem_p3);
        print_uart("The first");
        BRRTK_Release_Cnt_Sem(&sem_p1_p2);
    
    }
}
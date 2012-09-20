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
#include "mailbox.h"
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
  int i;
  for (i=0; *(poruka+i)!='\0'; i++){
    UARTCharPut(UART0_BASE,*(poruka+i));
  }
}



#define P1STACKSIZE 64         /* Size of task's stack in words */
#define P2STACKSIZE 64
#define P3STACKSIZE 64

#define MAILBOX_BUFFER_SIZE_BYTES 20

long int P1Stk[P1STACKSIZE];           /* Space for each task's stack  */
long int P2Stk[P2STACKSIZE];
long int P3Stk[P3STACKSIZE];

void P1Task(void * arg);               /* Function prototypes for task code */
void P2Task(void * arg);
void P3Task(void * arg);

brrtk_DelayCBptr delayP1=NULL,delayP2=NULL,delayP3=NULL;
MBXCB mbx_1,mbx_2,mbx_3,mbx_4;
char message1[]="The first";
char message2[]=" the last ";
char message3[]="my everything - ";
char message4[]="Barry White\r\n";

char mbx_1_buffer[MAILBOX_BUFFER_SIZE_BYTES];
char mbx_2_buffer[MAILBOX_BUFFER_SIZE_BYTES];
char mbx_3_buffer[MAILBOX_BUFFER_SIZE_BYTES];
char mbx_4_buffer[MAILBOX_BUFFER_SIZE_BYTES];


void main( void )
{
      
    Hardware_Init();
    uart_init();
    BRRTK_Initialize();

    BRRTK_Create_Mbx(&mbx_1,(unsigned char)sizeof(message1),1,&mbx_1_buffer);
    BRRTK_Create_Mbx(&mbx_2,(unsigned char)sizeof(message2),1,&mbx_2_buffer);
    BRRTK_Create_Mbx(&mbx_3,(unsigned char)sizeof(message3),1,&mbx_3_buffer);
    BRRTK_Create_Mbx(&mbx_4,(unsigned char)sizeof(message4),1,&mbx_4_buffer);

    BRRTK_CreateNewTask_aperiodic_medium(P1Task, &P1Stk[P1STACKSIZE], 1, &delayP1 ,5);
    BRRTK_CreateNewTask_aperiodic_medium(P2Task, &P2Stk[P2STACKSIZE], 2, &delayP2 ,5);
    BRRTK_CreateNewTask_aperiodic_medium(P3Task, &P3Stk[P3STACKSIZE], 3, &delayP3 ,5);
    BRRTK_Run();
    
}



void P1Task(void * arg)
{
    
    while (1)
    {
        char message_recieved[MAILBOX_BUFFER_SIZE_BYTES]={0};
        BRRTK_Recieve_Mbx(&mbx_1,message_recieved);
        print_uart(message_recieved); //The first
        BRRTK_Send_Mbx(&mbx_4,message4);
    }
}

void P2Task(void * arg)
{

    while (1)
    {
        char message_recieved[MAILBOX_BUFFER_SIZE_BYTES]={0};
        BRRTK_Recieve_Mbx(&mbx_2,message_recieved);
        print_uart(message_recieved); //the last
        BRRTK_Send_Mbx(&mbx_3,message3);
    }
}
void P3Task(void * arg)
{
    
    while (1)
    {
        char message_recieved[MAILBOX_BUFFER_SIZE_BYTES]={0};
        BRRTK_Send_Mbx(&mbx_1,message1);
        BRRTK_Send_Mbx(&mbx_2,message2);
        BRRTK_Recieve_Mbx(&mbx_3,message_recieved);
        print_uart(message_recieved); //my everything - 
        BRRTK_Recieve_Mbx(&mbx_4,message_recieved);
        print_uart(message_recieved); //Barry White\r\n 
    }
}


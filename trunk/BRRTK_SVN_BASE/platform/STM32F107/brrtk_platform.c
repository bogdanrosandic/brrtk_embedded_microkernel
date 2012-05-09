/*

 * This file is part of the BRRTK source code.
 * 
 * Author: Bogdan Rosandic <bogdanrosandic@gmail.com>
 *
 */

#include "brrtk_platform.h"
#include "main.h"
#include "stm32f10x.h"
#include "stm32_eval.h"
#include "stm32f10x_usart.h"
#include <stdio.h>
#include <yfuns.h>
/* Private variables ---------------------------------------------------------*/
USART_InitTypeDef USART_InitStructure;
NVIC_InitTypeDef NVIC_InitStructure;
//#define portNVIC_PENDSVSET			0x10000000
//#define portNVIC_INT_CTRL			( ( volatile unsigned long *) 0xe000ed04 )
/* Private functions ---------------------------------------------------------*/
void RCC_Configuration(void);
void GPIO_Configuration(void);

void brrtk_global_disable_interrupts(void)
{
  
  __disable_irq();
  
}
/*---------------------------------------
    This function globaly enables interrupts
 ----------------------------------------*/
void brrtk_global_enable_interrupts(void)
{
   
  __enable_irq();
  
}

void brrtk_TriggerReturnFromInterruptHandler(void)
{

     // *(portNVIC_INT_CTRL) = portNVIC_PENDSVSET;
     NVIC_SetPendingIRQ(PendSV_IRQn);
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
    RCC_Configuration();
    GPIO_Configuration();
    /* Setup SysTick Timer for 1 msec interrupts  */
    if (SysTick_Config(SystemFrequency / 1000))
  { 
    /* Capture error */ 
    while (1);
  }
    /* Configure one bit for preemption priority */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    /* Enable the Memory Manage Handler */
    //NVIC_SystemHandlerPriorityConfig(SystemHandler_UsageFault, 2);
    NVIC_SetPriority(UsageFault_IRQn, 2 << 0x03);
    //  NVIC_SystemHandlerPriorityConfig(SystemHandler_BusFault, 2);
    NVIC_SetPriority(BusFault_IRQn, 2 << 0x03);
    //  NVIC_SystemHandlerPriorityConfig(SystemHandler_SVCall, 3);
    NVIC_SetPriority(SVCall_IRQn, 3 << 0x03);
    NVIC_SetPriority(SysTick_IRQn, 5 << 0x03);
    //  NVIC_SetPriority(SystemHandler_PSV, 7);
    NVIC_SetPriority(PendSV_IRQn, 7 << 0x03);
    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    /* USART configuration */
    USART_Init(USART3, &USART_InitStructure);
    /* Enable USART */
    USART_Cmd(USART3, ENABLE);
    
    /* Enable the System Handler */
  //  NVIC_SystemHandlerConfig(SystemHandler_UsageFault, ENABLE);
  //  NVIC_SystemHandlerConfig(SystemHandler_BusFault, ENABLE);
  //  NVIC_SystemHandlerConfig(SystemHandler_PSV, ENABLE); 
    NVIC_EnableIRQ(BusFault_IRQn);
    NVIC_EnableIRQ(PendSV_IRQn);
    NVIC_EnableIRQ(UsageFault_IRQn);
    NVIC_EnableIRQ(SysTick_IRQn);
    printf("\n\[BTR]Hardware_Init-> line132 (brrtk_platform.c)init_ok\n\r");
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
/**
  * @brief  Configures the different system clocks.
  * @param  None
  * @retval None
  */
void RCC_Configuration(void)
{
    /* Setup the microcontroller system. Initialize the Embedded Flash Interface,  
    initialize the PLL and update the SystemFrequency variable. */
    SystemInit();

    /* Enable peripheral clocks --------------------------------------------------*/
    /* GPIOB Periph clock enable */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
    /* Enable UART clock */
    GPIO_PinRemapConfig(GPIO_FullRemap_USART3,ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, DISABLE);
}
/**
  * @brief  Configures the different GPIO ports.
  * @param  None
  * @retval None
  */
void GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* Configure USART Tx as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
  
    /* Configure USART Rx as input floating */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
 }
/*************************************************************************
* Function Name: __write
* Parameters: Low Level cahracter output
*
* Return:
*
* Description:
*
*************************************************************************/
 size_t __write(int Handle, const unsigned char * Buf, size_t Bufsize)
{
    size_t nChars = 0;
    for (/*Empty */; Bufsize > 0; --Bufsize)
	{
        /* Loop until the end of transmission */
	while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
	USART_SendData(USART3, * Buf++);
	
        ++nChars;
	}
	return nChars;
}

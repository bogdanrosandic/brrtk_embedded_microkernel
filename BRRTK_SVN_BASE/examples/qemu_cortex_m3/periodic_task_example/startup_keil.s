# 1 "/home/bogdan/BOGDAN/Projects/brrtk_embedded_microkernel/BRRTK_SVN_BASE/examples/qemu_cortex_m3/periodic_task_example/startup_keil.S"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "/home/bogdan/BOGDAN/Projects/brrtk_embedded_microkernel/BRRTK_SVN_BASE/examples/qemu_cortex_m3/periodic_task_example/startup_keil.S"
# 33 "/home/bogdan/BOGDAN/Projects/brrtk_embedded_microkernel/BRRTK_SVN_BASE/examples/qemu_cortex_m3/periodic_task_example/startup_keil.S"
    .syntax unified
    .arch armv7-m

    .section .stack
    .align 3



    .equ Stack_Size, 0xc00

    .globl __StackTop
    .globl __StackLimit
__StackLimit:
    .space Stack_Size
    .size __StackLimit, . - __StackLimit
__StackTop:
    .size __StackTop, . - __StackTop

    .section .heap
    .align 3



    .equ Heap_Size, 0

    .globl __HeapBase
    .globl __HeapLimit
__HeapBase:
    .if Heap_Size
    .space Heap_Size
    .endif
    .size __HeapBase, . - __HeapBase
__HeapLimit:
    .size __HeapLimit, . - __HeapLimit

    .section .isr_vector
    .align 2
    .globl __isr_vector
__isr_vector:
    .long __StackTop
    .long Reset_Handler
    .long NMI_Handler
    .long HardFault_Handler
    .long MemManage_Handler
    .long BusFault_Handler
    .long UsageFault_Handler
    .long 0
    .long 0
    .long 0
    .long 0
    .long SVC_Handler
    .long DebugMon_Handler
    .long 0
    .long PendSV_Handler
    .long SysTick_Handler


    .long Default_Handler

    .size __isr_vector, . - __isr_vector

    .text
    .thumb
    .thumb_func
    .align 2
    .globl Reset_Handler
    .type Reset_Handler, %function
Reset_Handler:
# 150 "/home/bogdan/BOGDAN/Projects/brrtk_embedded_microkernel/BRRTK_SVN_BASE/examples/qemu_cortex_m3/periodic_task_example/startup_keil.S"
    ldr r1, =__etext
    ldr r2, =__data_start__
    ldr r3, =__data_end__

.L_loop1:
    cmp r2, r3
    ittt lt
    ldrlt r0, [r1], #4
    strlt r0, [r2], #4
    blt .L_loop1
# 220 "/home/bogdan/BOGDAN/Projects/brrtk_embedded_microkernel/BRRTK_SVN_BASE/examples/qemu_cortex_m3/periodic_task_example/startup_keil.S"
    bl SystemInit





    bl _start

    .pool
    .size Reset_Handler, . - Reset_Handler

    .align 1
    .thumb_func
    .weak Default_Handler
    .type Default_Handler, %function
Default_Handler:
    b .
    .size Default_Handler, . - Default_Handler




    .macro def_irq_handler handler_name
    .weak \handler_name
    .set \handler_name, Default_Handler
    .endm

    def_irq_handler NMI_Handler
    def_irq_handler HardFault_Handler
    def_irq_handler MemManage_Handler
    def_irq_handler BusFault_Handler
    def_irq_handler UsageFault_Handler
    def_irq_handler SVC_Handler
    def_irq_handler DebugMon_Handler
    def_irq_handler PendSV_Handler
    def_irq_handler SysTick_Handler
    def_irq_handler DEF_IRQHandler

    .end

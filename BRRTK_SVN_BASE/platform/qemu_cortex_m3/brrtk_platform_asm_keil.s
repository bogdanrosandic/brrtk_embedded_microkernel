# 1 "/home/bogdan/BOGDAN/Projects/brrtk_embedded_microkernel/BRRTK_SVN_BASE/platform/qemu_cortex_m3/brrtk_platform_asm_keil.S"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "/home/bogdan/BOGDAN/Projects/brrtk_embedded_microkernel/BRRTK_SVN_BASE/platform/qemu_cortex_m3/brrtk_platform_asm_keil.S"

.syntax unified
.text
.thumb

.global initialize_stack_idle
.global initialize_stack
.global context_switch
.global SVCall_handler
.global save_context
.global go_to_scheduler
.global save_context_ISR


.thumb_func
initialize_stack_idle:

      pop {r9}
      pop {r10}
      mov r11, sp
      mov sp, r1
      mov r7,#0x01000000
      push {r7}
      push {r0}
      mov r7,#0
      push {r7}
      push {r7}
      push {r7}
      push {r7}
      push {r7}
      push {r7}
      push {r7}
      push {r7}
      push {r7}
      push {r7}
      push {r7}
      push {r7}
      push {r7}
      push {r7}
      mov r0,sp
      mov sp, r11

      push {r10}
      push {r9}

      bx lr

.thumb_func
initialize_stack:


      mov r11, sp
      mov sp, r1
      mov r7,#0x01000000
      push {r7}
      push {r0}
      mov r7,#0
      push {r7}
      push {r7}
      push {r7}
      push {r7}
      push {r7}
      push {r7}
      push {r7}
      push {r7}
      push {r7}
      push {r7}
      push {r7}
      push {r7}
      push {r7}
      push {r7}
      mov r0,sp
      mov sp, r11



      bx lr
.thumb_func
context_switch:

       mov sp,r0
       cpsie i

       svc #0


.thumb_func
SVCall_handler:

      pop {R11}
      pop {R10}
      pop {R9}
      pop {R8}
      pop {R7}
      pop {R6}
      pop {R5}
      pop {R4}

      pop {R11}
      pop {R10}
      pop {R9}
      pop {R8}
      pop {R7}
      pop {R6}
      pop {R5}
      pop {R4}

      bx lr
.thumb_func
save_context:


                  sub r13,r13,#4

                  push {r9}
                  push {r8}
                  push {r7}

      add r13,r13,#4
      add r13,r13,#4
      add r13,r13,#4
      add r13,r13,#4

      pop {r7}
      pop {r8}

      MRS r9,PSR
      ORR r9,r9,#0x01000000



                  push {r9}
                  push {r8}
                  push {r7}

                  sub r13,r13,#4
      pop {r7}
                  push {r12}

                  sub r13,r13,#4
      pop {r8}
                  push {r3}

                  sub r13,r13,#4
      pop {r9}
                  push {r2}
                  push {r1}
                  push {r0}
                  push {r4}
                  push {r5}
                  push {r6}
                  push {r7}
                  push {r8}
                  push {r9}
                  push {r10}
                  push {r11}



      mov r0,sp



      bx LR

.thumb_func
go_to_scheduler:

      pop {r3}
      pop {r2}
      mov r7,#0x01000000
      push {r7}
      push {R0}
      push {r7}
      push {r7}
      push {r7}
      push {r7}
      push {r7}
      push {r7}

      push {r2}
      push {r3}

      bx lr

.thumb_func
save_context_ISR:


      pop {r3}
      pop {r2}
      push {r4}
      push {r5}
      push {r6}
      push {r7}
      push {r8}
      push {r9}
      push {r10}
      push {r11}



      mov r0,sp

      push {r2}
      push {r3}

      bx LR
.end

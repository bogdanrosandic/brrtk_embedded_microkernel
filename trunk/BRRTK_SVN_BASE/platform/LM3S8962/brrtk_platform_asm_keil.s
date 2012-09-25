

        AREA    |.text|, CODE, READONLY
        THUMB
        
        EXPORT initialize_stack_idle
        EXPORT initialize_stack
        EXPORT context_switch
        EXPORT SVCall_handler
        EXPORT save_context
        EXPORT go_to_scheduler
        EXPORT save_context_ISR
        
       

        
        


initialize_stack_idle
      
      pop  {r9}
      pop  {r10} 
      mov r11, sp
      mov sp, r1
      mov  r7,#0x01000000
      push {r7}
      push {r0}
      mov  r7,#0
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


initialize_stack
      
     
      mov r11, sp
      mov sp, r1
      mov  r7,#0x01000000
      push {r7}
      push {r0}
      mov  r7,#0
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
      
context_switch
        
       mov sp,r0
       cpsie i
             
       svc #0
       
   
      
SVCall_handler

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
      
      bx  lr     
     
save_context
      
      
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
      
      
      
      mov  r0,sp
      
      
      
      bx LR
      

go_to_scheduler

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


save_context_ISR
      
      
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
      
      
      
      mov  r0,sp
      
      push {r2}
      push {r3}
      
      bx LR 
      

      END

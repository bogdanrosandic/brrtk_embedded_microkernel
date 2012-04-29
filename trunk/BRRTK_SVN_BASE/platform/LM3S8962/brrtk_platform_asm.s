

        RSEG    CODE:CODE(2)
	thumb
        
        PUBLIC initialize_stack_idle
        PUBLIC initialize_stack
        PUBLIC context_switch
        PUBLIC SVCall_handler
        PUBLIC save_context
        PUBLIC go_to_scheduler
        PUBLIC save_context_ISR
        
       

        
        


initialize_stack_idle
      
      pop  {r9}
      pop  {r10} //skida lr koji je stavio na stek
      mov r11, sp
      mov sp, r1
      mov  r7,#0x01000000// T bit PSR registra mora da bude 1 jer Cortex M3 ne podrzava ARM stanje nego samo Thumb
      push {r7}// stavlja se vrednost inicijalna vrednost PSR na stek
      push {r0}//stavlja se vrednost PC na stek
      mov  r7,#0
      push {r7}//LR
      push {r7}//R12
      push {r7}//r3
      push {r7}//r2
      push {r7}//r1
      push {r7}//r0
      push {r7}//r4
      push {r7}//r5
      push {r7}//r6
      push {r7}//r7
      push {r7}//r8
      push {r7}//r94
      push {r7}//r10
      push {r7}//r11
      mov r0,sp// cuva se vrednost SP inicijalizovanog steka
      mov sp, r11
      
      push {r10}
      push {r9}
      
      bx lr


initialize_stack
      
     
      mov r11, sp
      mov sp, r1
      mov  r7,#0x01000000// T bit PSR registra mora da bude 1 jer Cortex M3 ne podrzava ARM stanje nego samo Thumb
      push {r7}// stavlja se vrednost inicijalna vrednost PSR na stek
      push {r0}//stavlja se vrednost PC na stek
      mov  r7,#0
      push {r7}//LR
      push {r7}//R12
      push {r7}//r3
      push {r7}//r2
      push {r7}//r1
      push {r7}//r0
      push {r7}//r4
      push {r7}//r5
      push {r7}//r6
      push {r7}//r7
      push {r7}//r8
      push {r7}//r94
      push {r7}//r10
      push {r7}//r11
      mov r0,sp// cuva se vrednost SP inicijalizovanog steka
      mov sp, r11
      
      
      
      bx lr
      
context_switch:
        
       mov sp,r0//mozda moze i ovo iz r0 u sp
       cpsie i
             
       svc #0
       //LDR r7,=0xE000ED04
       //LDR r6,[r7]
       //ORR r6,r6,#0x10000000
       //STR r6,[r7]
      /*promena steka*/
   
      
SVCall_handler:
//PendSV_handler:
         
       // pop {r0}//?????????? skida sa steka 
        
      /*promena steka*/
      /*proba*/
       //add r13,r13,#32/// promeni SP tako da ignorise stack frame koji je postavljen na stek usled ulaska u svc rutnu
      pop {R11}
      pop {R10}
      pop {R9}
      pop {R8}
      pop {R7}
      pop {R6}
      pop {R5}
      pop {R4}
      /*proba*/
      /*preuzimanje konteksta sa novog steka*/
      pop {R11}
      pop {R10}
      pop {R9}
      pop {R8}
      pop {R7}
      pop {R6}
      pop {R5}
      pop {R4}
      //mov lr,#0xFFFFFFF9
      bx  lr     
     
save_context:
      
      /*cuvanje registara na steku*/
                  sub r13,r13,#4
      
                  push {r9}
                  push {r8}
                  push {r7}
      
      add r13,r13,#4
      add r13,r13,#4
      add r13,r13,#4
      add r13,r13,#4
      
      pop {r7}//LR
      pop {r8}//PC
      
      MRS r9,PSR
      ORR r9,r9,#0x01000000                
      
                  //sub r13,r13,#4
      
                  push {r9}//PSR
                  push {r8}// PC
                  push {r7}// LR
     
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
      /*cuvanje registara na steku*/
      
      /*vracanje nove vrednosti stek pointera*/
      mov  r0,sp//
      /*vracanje nove vrednosti stek pointera*/
      
      /*povratak iz funkcije*/
      bx LR
      /*povratak iz funkcije*/

go_to_scheduler;
/*treba da se formira takav stek frejm da se po povratku iz prekida skoci pravo u scheduler*/
      pop {r3}
      pop {r2}
      mov r7,#0x01000000
      push {r7}//PSR
      push {R0}// PC
      push {r7}//lr
      push {r7}//r12
      push {r7}//r3
      push {r7}// r2
      push {r7}//r1
      push {r7}//r0
      
      push {r2}
      push {r3}//r3
      //ret;
      bx lr


save_context_ISR: // u LR je sacuvana adresa povratka iz prekida ali ona ce da se koristi samo za vracanje iz funkcije
      
      /*dodavanje preostalih registara na stek*/
      pop {r3}
      pop {r2}
      //pop {r1}
      //pop {r0}
      push {r4}
      push {r5}
      push {r6}
      push {r7}
      push {r8}
      push {r9}
      push {r10}
      push {r11}
      /*dodavanje preostalih registara na stek*/
      
      /*vracanje nove vrednosti stek pointera*/
      mov  r0,sp//
      /*vracanje nove vrednosti stek pointera*/
      //push {r0}
      //push {r1}
      push {r2}
      push {r3}
      /*povratak iz funkcije*/
      bx LR /*povratak iz funkcije*/
      

      END
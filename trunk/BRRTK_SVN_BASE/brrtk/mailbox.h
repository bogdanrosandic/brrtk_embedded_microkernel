#ifndef __MAILBOX_H__
#define __MAILBOX_H__
#include "brrtk_app.h"
#include "brrtk_platform.h"
#include "brrtk_task.h"
#include "brrtk_utilities.h"
#include "scheduler.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct brrtk_MBXCB *brrtk_MBXCBptr;

typedef struct brrtk_MBXCB

{    unsigned char usage;  //0 unused, 1 used
    unsigned char message_lenght;/*duzina pojedinacne poruke*/
    unsigned int number_of_messages;/*maksimalni broj poruka u mailbox-u*/
    unsigned int  avaliable_space;
    unsigned long timeout_lwIP;
    brrtk_TCBptr waiting_list_send_first;/*liste cekanja za slanje poruke preko mailbox-a*/
    brrtk_TCBptr waiting_list_send_last;  
    brrtk_TCBptr waiting_list_recieve_first;/*liste cekanja za prijem poruka iz mailbox-a*/
    brrtk_TCBptr waiting_list_recieve_last;
    void * mailbox_buffer_tail;
    void * mailbox_buffer;            /*--pokazivac na buffer mailbox-a--*/
    void * write_pointer;
    void * read_pointer;
    
    struct brrtk_MBXCB * pNext_lwIP;
    
}  MBXCB;

extern void BR_Create_Mbx (brrtk_MBXCBptr p_mbx,unsigned char msg_lngt,unsigned int num_of_msgs,void * mbx_buffer);
extern void BR_Send_Mbx(brrtk_MBXCBptr p_mbx, void * p_mail);
extern char BR_Send_Mbx_ISR (brrtk_MBXCBptr p_mbx, void * p_mail);
extern void BR_Recieve_Mbx (brrtk_MBXCBptr p_mbx, void * p_dest);
extern long int BR_Recieve_Mbx_wait(brrtk_MBXCBptr p_mbx, void * p_dest,unsigned long timeout_ms);
extern char BR_Recieve_Mbx_ISR (brrtk_MBXCBptr p_mbx, void * p_dest);
extern long int mbx_timeout_tick_refresh(void);
char Mailbox_timeouted(brrtk_MBXCBptr p_mbx);
/*treba dodati funkciju koja omogucava blokiranje taska na odredjeno vreme, task ceka poruku odredjeno vreme nakon cega se deblokira*/


#endif /*__MAILBOX_H__*/


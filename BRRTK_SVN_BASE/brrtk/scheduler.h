/*

 * This file is part of the BRRTK source code.
 * 
 * Author: Bogdan Rosandic <bogdanrosandic@gmail.com.se>
 *
 */


#ifndef __BRRTK_SCHEDULER_H__
#define __BRRTK_SCHEDULER_H__

#include "brrtk_task.h"





typedef enum

{
    WAITING,
    SUSPENDED,
    READY,
    ACTIVE
}  task_state;


void brrtk_scheduler(void);
void BRRTK_Run(void);
#endif /* __BRRTK_SCHEDULER_H__*/


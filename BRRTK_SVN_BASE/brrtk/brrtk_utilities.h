/*
 * This file is part of the BRRTK source code.
 * 
 * Author: Bogdan Rosandic <bogdanrosandic@gmail.com.se>
 *
 */
 
#ifndef __BRRTK_UTILITIES_H__
#define __BRRTK_UTILITIES_H__

typedef enum
{
    BRRTK_OS_RUN = 0x0001,
    BRRTK_CONTEXT_SAVED = 0x0002,
    BRRTK_TICK = 0x0004,
    BRRTK_INTERUPT_ENTERED = 0x0008,
    BRRTK_SCHEDULER_STARTED = 0x0010,
    BRRTK_BLOCKING_CALL = 0x0020,
    BRRTK_TRIGERRRETURNFROMINTERRUPT_STARTED = 0x0040
} brrtk_flag_masks;

typedef enum
{
    brrtk_false =0,
    brrtk_true
} brrtk_bool;
 /*---------------------------------------
     GLOBAL 
      VARIABLES
  ----------------------------------------*/

brrtk_bool isBrrtkRunCalled(void); 
brrtk_bool isContextSaved(void);
brrtk_bool isTick(void);
brrtk_bool isInterruptEntered(void);
brrtk_bool isSchedulerStarted(void);
brrtk_bool isBlockingCall(void);
brrtk_bool isTriggerReturnFromInterruptStarted(void);
void setBrrtkRunCalled(void);
void setContextSaved(void);
void setTick(void);
void setInterruptEntered(void);
void setSchedulerStarted(void);
void setBlockingCall(void);
void setTriggerReturnFromInterruptStarted(void);
void resetBrrtkRunCalled(void);
void resetContextSaved(void);
void resetTick(void);
void resetInterruptEntered(void);
void resetSchedulerStarted(void);
void resetBlockingCall(void);
void resetTriggerReturnFromInterruptStarted(void);





 
#endif /* __BRRTK_UTILITIES_H__ */


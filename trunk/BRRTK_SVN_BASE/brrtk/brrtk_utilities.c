/*
 * This file is part of the BRRTK source code.
 * 
 * Author: Bogdan Rosandic <bogdanrosandic@gmail.com.se>
 *
 */

#include "brrtk_utilities.h"
 

static unsigned int brrtk_state_flags;

brrtk_bool isBrrtkRunCalled(void)
{
    if (brrtk_state_flags&BRRTK_OS_RUN)
    {
        return brrtk_true;
    }
    else
    {
        return brrtk_false;
    }
}

brrtk_bool isContextSaved(void)
{
    if (brrtk_state_flags&BRRTK_CONTEXT_SAVED)
    {
        return brrtk_true;
    }
    else
    {
        return brrtk_false;
    }
}

brrtk_bool isTick(void)
{
    if (brrtk_state_flags&BRRTK_TICK)
    {
        return brrtk_true;
    }
    else
    {
        return brrtk_false;
    }
}

brrtk_bool isInterruptEntered(void)
{
    if (brrtk_state_flags&BRRTK_CONTEXT_SAVED)
    {
        return brrtk_true;
    }
    else
    {
        return brrtk_false;
    }
}

brrtk_bool isSchedulerStarted(void)
{
    if (brrtk_state_flags&BRRTK_SCHEDULER_STARTED)
    {
        return brrtk_true;
    }
    else
    {
        return brrtk_false;
    }
}

brrtk_bool isBlockingCall(void)
{
    if (brrtk_state_flags&BRRTK_BLOCKING_CALL)
    {
        return brrtk_true;
    }
    else
    {
        return brrtk_false;
    }
}

brrtk_bool isTriggerReturnFromInterruptStarted(void)
{
    if (brrtk_state_flags&BRRTK_TRIGERRRETURNFROMINTERRUPT_STARTED)
    {
        return brrtk_true;
    }
    else
    {
        return brrtk_false;
    }
}


void setBrrtkRunCalled(void)
{
    brrtk_state_flags |= BRRTK_OS_RUN;

}

void setContextSaved(void)
{
    brrtk_state_flags |= BRRTK_CONTEXT_SAVED;

}

void setTick(void)
{
    brrtk_state_flags |= BRRTK_TICK;

}

void setInterruptEntered(void)
{
    brrtk_state_flags |= BRRTK_INTERUPT_ENTERED;

}

void setSchedulerStarted(void)
{
    brrtk_state_flags |= BRRTK_SCHEDULER_STARTED;

}

void setBlockingCall(void)
{
    brrtk_state_flags |= BRRTK_BLOCKING_CALL;
}

void setTriggerReturnFromInterruptStarted(void)
{
    brrtk_state_flags |= BRRTK_TRIGERRRETURNFROMINTERRUPT_STARTED;
}


void resetBrrtkRunCalled(void)
{
    brrtk_state_flags &= ~BRRTK_OS_RUN;

}

void resetContextSaved(void)
{
    brrtk_state_flags &= ~BRRTK_CONTEXT_SAVED;

}

void resetTick(void)
{
    brrtk_state_flags &= ~BRRTK_TICK;

}

void resetInterruptEntered(void)
{
    brrtk_state_flags &= ~BRRTK_INTERUPT_ENTERED;

}

void resetSchedulerStarted(void)
{
    brrtk_state_flags &= ~BRRTK_SCHEDULER_STARTED;

}

void resetBlockingCall(void)
{
    brrtk_state_flags &= ~BRRTK_BLOCKING_CALL;
}


void resetTriggerReturnFromInterruptStarted(void)
{
    brrtk_state_flags &= ~BRRTK_TRIGERRRETURNFROMINTERRUPT_STARTED;
}


 


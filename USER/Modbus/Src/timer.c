
/* ----------------------- Modbus includes ----------------------------------*/
#include "datatype.h"
#include "modbusprotocol.h"


/* ----------------------- static functions ---------------------------------*/

/* ----------------------- Start implementation -----------------------------*/
Boolean initTimers()
{
    return TRUE;
}


/**
 * enable timers
 */
extern void enableTimers()
{
    enableTimer4();
}


/**
 * disable timers
 */
extern void disableTimers()
{
    disableTimer4();
}


/* Create an ISR which is called whenever the timer has expired. This function
 * must then call pxMBPortCBTimerExpired( ) to notify the protocol stack that
 * the timer has expired.
 */
extern void timerExpiredISR()
{
    t35TimerExpiredCallback();
}


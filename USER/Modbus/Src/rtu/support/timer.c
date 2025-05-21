#include "datatype.h"
#include "modbusrtuprotocol.h"


/**
 * init timer
 */
extern Boolean initTimer()
{
    return TRUE;
}


/**
 * enable T35 timer
 */
extern void enableTT35Timer()
{
    enableTimer4();
}


/**
 * disable T35 timer
 */
extern void disableT35Timer()
{
    disableTimer4();
}


/**
 * ISR which is called whenever the timer has expired.
 */
extern void timerExpiredISR()
{
    t35TimerExpiredCallback();
}


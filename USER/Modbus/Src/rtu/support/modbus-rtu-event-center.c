#include "modbusrtumode.h"
#include "datatype.h"


static ModbusRtuEvent heldEvent;
static Boolean hasEvent = FALSE;



Boolean publishEvent(ModbusRtuEvent event)
{
    if (hasEvent)
    {
        return FALSE;
    }

    // firstly cover the event
    heldEvent = event;
    hasEvent = TRUE;
    return TRUE;
}


Boolean getEvent(ModbusRtuEvent * event)
{
    if (!hasEvent)
    {
        return FALSE;
    }

    // must set event firstly
    * event = heldEvent;
    hasEvent = FALSE;
    return TRUE;
}

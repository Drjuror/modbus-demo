#include "modbusrtuprotocol.h"
#include "datatype.h"


static ModbusSlaveEvent heldEvent;
static Boolean hasEvent = FALSE;



Boolean publishEvent(ModbusSlaveEvent event)
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


Boolean getEvent(ModbusSlaveEvent * event)
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

#include "modbusrtuprotocol.h"
#include "datatype.h"


// static ModbusMasterEvent heldEvent;
static Boolean hasEvent = FALSE;



// Boolean publishEvent(ModbusMasterEvent event)
// {
//     if (hasEvent)
//     {
//         return FALSE;
//     }

//     // firstly cover the event
//     heldEvent = event;
//     hasEvent = TRUE;
//     return TRUE;
// }


// Boolean getEvent(ModbusMasterEvent * event)
// {
//     if (!hasEvent)
//     {
//         return FALSE;
//     }

//     // must set event firstly
//     * event = heldEvent;
//     hasEvent = FALSE;
//     return TRUE;
// }

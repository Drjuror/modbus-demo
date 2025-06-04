#include "modbusrtumode.h"
#include "modbusfunction.h"
#include "datatype.h"


static ModbusDeviceWorkMode workMode;
static volatile Boolean enabled = FALSE;



/**
 * initialize rtu protocol
 *
 * @param workContext modbus device work context
 */
void initRtuMode(ModbusDeviceWorkContext * workContext)
{
    if (workContext->workMode == WORK_MODE_MASTER)
    {
        initRtuModeMaster(workContext);
        workMode = WORK_MODE_MASTER;
    }
    else if (workContext->workMode == WORK_MODE_SLAVE)
    {
        initRtuModeSlave(workContext);
        workMode = WORK_MODE_SLAVE;
    }
    else
    {
        return;
    }
}



void enableRtuMode()
{
    if (enabled)
    {
        return;
    }

    enabled = TRUE;
}



void disableRtuMode()
{
    if (enabled)
    {
        enabled = FALSE;
    }
}

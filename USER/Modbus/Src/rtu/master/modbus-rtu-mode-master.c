#include "modbusrtumode.h"
#include "modbusfunction.h"
#include "datatype.h"


// node address
static unsigned char address;
static volatile Boolean enabled = TRUE;


static unsigned char bytesBuffer[RTU_FRAME_CHAR_MAXIMUM_BYTES];


/**
 * initialize rtu protocol
 *
 * @param workContext modbus device work context
 */
extern void initRtuModeMaster(ModbusDeviceWorkContext * workContext)
{
    address = MASTER_DEVICE_ADDRESS;
}



void startRtuMasterPoll()
{

    ModbusRtuEvent publishedEvent;
    if (!getEvent(&publishedEvent)) {
        return;
    }

    switch (publishedEvent) {
        case FRAME_RECEIVED_EVENT:
            // do nothing
            break;
        case FRAME_TRANSMITTED_EVENT:
            // do nothing
            break;
        default:
            break;
    }


    // bytesBuffer[0] = 0x01;
    // bytesBuffer[1] = 0x06;
    // bytesBuffer[2] = 0x00;
    // bytesBuffer[3] = 0x00;
    // bytesBuffer[4] = 0x04;
    // bytesBuffer[5] = 0x57;
    // bytesBuffer[6] = 0xCA;
    // bytesBuffer[7] = 0xF4;
    // for (int i = 0; i < 8; i++) {
    //     transmitByte(bytesBuffer[i]);
    // }
}

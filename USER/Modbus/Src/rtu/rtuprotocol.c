#include "modbusprotocol.h"
#include "serialprotocol.h"
#include "datatype.h"


/**
 * node address
 */
static unsigned char address = 0x01;
static NodeWorkMode workMode;
static Boolean enabled = TRUE;


/**
 * hold received bytes in one frame
 */
static unsigned char bytesBuffer[RTU_FRAME_CHAR_MAXIMUM_SIZE];
static unsigned short receivedBytesBufferPosition = 0;
static unsigned char sendCharBuffer[RTU_FRAME_CHAR_MAXIMUM_SIZE];



static volatile ModbusReceiverState receiverState = RECEIVER_IDLE_STATE;
static volatile ModbusTransmitterState transmitterState = TRANSMITTER_IDLE_STATE;



static void initRtuMaster(ModbusNodeWorkContext * context);
static void initRtuSlave(ModbusNodeWorkContext * context);


/**
 * initialize rtu protocol
 */
void initRtu(ModbusNodeWorkContext * context)
{
    if (context->workMode == NODE_ROLE_MASTER)
    {
        initRtuMaster(context);
    }
    else if (context->workMode == NODE_ROLE_SLAVE)
    {
        initRtuSlave(context);
    }
    else
    {
        return;
    }
}


/**
 * init rtu master
 */
static void initRtuMaster(ModbusNodeWorkContext * context)
{
    address = MASTER_NODE_ADDRESS;
}


static void initRtuSlave(ModbusNodeWorkContext * context)
{
    if (context->address < MIN_SLAVE_NODE_ADDRESS || context->address > MAX_SLAVE_NODE_ADDRESS)
    {
        return;
    }

    // address = context->address;
    // address = 0x02;
    workMode = NODE_ROLE_SLAVE;
}


void enableRtuSlave()
{
    if (enabled)
    {
        return;
    }

    enabled = TRUE;
}


void disable1()
{
    if (enabled)
    {
        enabled = FALSE;
    }
}


void startRtuSlave()
{
    ModbusSlaveEvent publishedEvent;
    if (!getEvent(& publishedEvent)) {
        return;
    }

    unsigned char length = receivedBytesBufferPosition - 1 - 2;
    switch (publishedEvent) {
        case FRAME_RECEIVED_EVENT:
            readInputRegister(&bytesBuffer, &length);

            unsigned short crc16 = usMBCRC16(&bytesBuffer, 11);
            bytesBuffer[11] = (unsigned char) (crc16 & 0xFF);
            bytesBuffer[12] = (unsigned char) (crc16 >> 8);

            receiverState = RECEIVER_IDLE_STATE;
            for (int i = 0; i < 13; i++) {
                transmitByte(bytesBuffer[i]);
            }
            break;
        case FRAME_TRANSMITTED_EVENT:
            break;
        default:
            break;
    }
}


void startRtuMaster()
{
    sendCharBuffer[0] = 0x01;
    sendCharBuffer[1] = 0x06;
    sendCharBuffer[2] = 0x00;
    sendCharBuffer[3] = 0x00;
    sendCharBuffer[4] = 0x04;
    sendCharBuffer[5] = 0x57;
    sendCharBuffer[6] = 0xCA;
    sendCharBuffer[7] = 0xF4;
    for (int i = 0; i < 8; i++) {
        transmitByte(sendCharBuffer[i]);
    }
}


extern void receiveByteCallback()
{
    if (transmitterState != TRANSMITTER_IDLE_STATE)
    {
        return;
    }

    //  take away received byte from serial port
    unsigned char receivedByte;
    serialReceiveByte((char *) & receivedByte);

    switch (receiverState)
    {
        case RECEIVER_IDLE_STATE:
            // reset received bytes buffer position
            receivedBytesBufferPosition = 0;
            // not for us
            if (receivedByte != address && address != SERIAL_BROADCAST_ADDRESS)
            {
                break;
            }

            // cache received byte
            bytesBuffer[receivedBytesBufferPosition++] = receivedByte;
            receiverState = RECEIVING_STATE;
            // start a t35 timer
            enableTimers();
            break;
        case RECEIVING_STATE:
            if (receivedBytesBufferPosition < RTU_FRAME_CHAR_MAXIMUM_SIZE)
            {
                bytesBuffer[receivedBytesBufferPosition++] = receivedByte;
                enableTimers();
            }
            else
            {
                // exception handler
            }
            break;
        default:
            break;
    }
}


/**
 *
 */
void t35TimerExpiredCallback()
{
    switch (receiverState)
    {
        case RECEIVER_IDLE_STATE:
            break;
        case RECEIVING_STATE:
            publishEvent(FRAME_RECEIVED_EVENT);
            break;
        default:
            break;
    }

    disableTimers();
}

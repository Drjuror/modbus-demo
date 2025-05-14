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


static unsigned char *pucSndBufferCur;
static unsigned char usSndBufferCount;


ModbusReceiverState receiverState = RECEIVER_IDLE_STATE;
ModbusTransmitterState transmitterState = TRANSMITTER_IDLE_STATE;



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
    address = 0x02;
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

    switch (publishedEvent) {
        case FRAME_RECEIVED_EVENT:
            sendCharBuffer[0] = 0x01;
            sendCharBuffer[1] = 0x04;
            sendCharBuffer[2] = 0x08;
            sendCharBuffer[3] = 0x00;
            sendCharBuffer[4] = 0x11;
            sendCharBuffer[5] = 0x00;
            sendCharBuffer[6] = 0x22;
            sendCharBuffer[7] = 0x00;
            sendCharBuffer[8] = 0x33;
            sendCharBuffer[9] = 0x00;
            sendCharBuffer[10] = 0x44;
            sendCharBuffer[11] = 0x2D;
            sendCharBuffer[12] = 0x37;
            for (int i = 0; i < 13; i++) {
                transmitByte(sendCharBuffer[i]);
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
            if (receivedByte != address || address != SERIAL_BROADCAST_ADDRESS)
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

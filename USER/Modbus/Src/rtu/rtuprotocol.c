#include "modbusprotocol.h"
#include "serialprotocol.h"
#include "function.h"
#include "datatype.h"


/**
 * node address
 */
static unsigned char address;
static ModbusDeviceWorkMode workMode;
static Boolean enabled = TRUE;


/**
 * hold received bytes in one frame
 */
static unsigned char bytesBuffer[RTU_FRAME_CHAR_MAXIMUM_BYTES];
static unsigned char receivedBytesBufferPosition = 0;
static unsigned char transmittedBytesBufferPosition = 0;


static volatile ModbusReceiverState receiverState = RECEIVER_IDLE_STATE;
static volatile ModbusTransmitterState transmitterState = TRANSMITTER_IDLE_STATE;


/**
 * perform function
 *
 * <p>it must be noted that,
 * @param pduFrame and @param pduFrameBytes will be filled response data after function call</p>
 *
 * @param pduFrame      pdu frame
 * @param pduFrameBytes byte size of pdu frame
 */
static void performFunction(unsigned char *pduFrame, unsigned char *pduFrameBytes);


/**
 * initialize rtu protocol
 *
 * @param workContext modbus device work context
 */
void initRtu(ModbusDeviceWorkContext * workContext)
{
    if (workContext->workMode == WORK_MODE_MASTER)
    {
        address = MASTER_DEVICE_ADDRESS;
        workMode = WORK_MODE_MASTER;
    }
    else if (workContext->workMode == WORK_MODE_SLAVE)
    {
        if (workContext->address < SLAVE_DEVICE_ADDRESS_MIN || workContext->address > SLAVE_DEVICE_ADDRESS_MAX)
        {
            return;
        }

        address = workContext->address;
        workMode = WORK_MODE_SLAVE;
    }
    else
    {
        return;
    }
}



void enableRtuSlave()
{
    if (enabled)
    {
        return;
    }

    enabled = TRUE;
}



void disableRtuSlave()
{
    if (enabled)
    {
        enabled = FALSE;
    }
}


/**
 * start rtu slave
 */
void startRtuSlave()
{
    ModbusSlaveEvent publishedEvent;
    if (!getEvent(& publishedEvent)) {
        return;
    }

    switch (publishedEvent) {
        case FRAME_RECEIVED_EVENT:
            unsigned char *pduFrame = (unsigned char *) &bytesBuffer;
            // move pdu frame pointer to the first byte of pdu frame, function code field actually
            pduFrame++;

            unsigned char pduFrameBytes = receivedBytesBufferPosition -
                RTU_FRAME_DEVICE_ADDRESS_FIELD_BYTES - RTU_FRAME_CRC_FIELD_BYTES;
            performFunction(pduFrame, &pduFrameBytes);

            // compute CRC16
            pduFrameBytes += RTU_FRAME_DEVICE_ADDRESS_FIELD_BYTES;
            unsigned short crc16 = computeCRC16(&bytesBuffer, pduFrameBytes);
            // low-order byte
            bytesBuffer[pduFrameBytes++] = (unsigned char) (crc16 & 0xFF);
            // high-order byte
            bytesBuffer[pduFrameBytes++] = (unsigned char) (crc16 >> 8);

            receiverState = RECEIVER_IDLE_STATE;
            transmitterState = TRANSMITTING_STATE;
            transmittedBytesBufferPosition = 0;
            receivedBytesBufferPosition = pduFrameBytes;
            enableUSART1TransEmptyIT();
            break;
        case FRAME_TRANSMITTED_EVENT:
            break;
        default:
            break;
    }
}


static void performFunction(unsigned char *pduFrame, unsigned char *pduFrameBytes)
{
    unsigned char functionCode = pduFrame[0];
    switch (functionCode)
    {
        // only supports read input register function code
        case READ_INPUT_REGISTER_FUNCTION_CODE:
            readInputRegister(pduFrame, pduFrameBytes);
            break;
        default:
            break;
    }
}


void startRtuMaster()
{
    static unsigned char sendCharBuffer[RTU_FRAME_CHAR_MAXIMUM_BYTES];

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


/**
 * callback for receiving byte
 */
extern void receiveByteCallback()
{
   //  take away received byte from serial port
    unsigned char receivedByte;
    serialReceiveByte((char *) &receivedByte);

    if (transmitterState != TRANSMITTER_IDLE_STATE)
    {
        return;
    }

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
            if (receivedBytesBufferPosition < RTU_FRAME_CHAR_MAXIMUM_BYTES)
            {
                bytesBuffer[receivedBytesBufferPosition++] = receivedByte;
                enableTimers();
            }
            else
            {
                // todo exception handler
            }
            break;
        default:
            break;
    }
}


/**
 * transmit byte callback
 */
extern void transmitByteCallback()
{
    switch (transmitterState)
    {
        case TRANSMITTER_IDLE_STATE:
            break;
        case TRANSMITTING_STATE:
            if (transmittedBytesBufferPosition < receivedBytesBufferPosition)
            {
                transmitByte(bytesBuffer[transmittedBytesBufferPosition++]);
            }
            else
            {
                disableUSART1TransEmptyIT();
                transmitterState = TRANSMITTER_IDLE_STATE;
                enableUSART1ReceiveIT();
            }
            break;
    }
}


/**
 * t35 timer expired callback
 */
void t35TimerExpiredCallback()
{
    switch (receiverState)
    {
        case RECEIVER_IDLE_STATE:
            break;
        case RECEIVING_STATE:
            publishEvent(FRAME_RECEIVED_EVENT);
            disableUSART1ReceiveIT();
            break;
        default:
            break;
    }

    disableTimers();
}

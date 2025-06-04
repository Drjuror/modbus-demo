#include "modbusrtumode.h"
#include "modbusfunction.h"
#include "datatype.h"


// node address
static unsigned char address;
static volatile Boolean enabled = TRUE;


// hold bytes in one frame whatever received or transmitted
static unsigned char bytesBuffer[RTU_FRAME_CHAR_MAXIMUM_BYTES];
static unsigned char receivedBytesBufferPosition = 0;
static unsigned char transmittedBytesBufferPosition = 0;


static volatile ModbusReceiverState receiverState = RECEIVER_IDLE_STATE;
static volatile ModbusTransmitterState transmitterState = TRANSMITTER_IDLE_STATE;


/**
 * perform function
 *
 * <p>it must be noted that,
 * @param pduFrame and @param pduFrameBytes will be filled response data
 * </p>
 *
 * @param pduFrame      pdu frame
 * @param pduFrameBytes byte size of pdu frame
 */
static void performFunction(unsigned char *pduFrame, unsigned char *pduFrameBytes);


/**
 * check received frame
 *
 * @param frame      received frame
 * @param frameBytes byte size of received frame
 */
static void checkReceivedFrame(unsigned char *frame, unsigned char *frameBytes);



/**
 * initialize rtu protocol
 *
 * @param workContext modbus device work context
 */
extern void initRtuModeSlave(ModbusDeviceWorkContext * workContext)
{
    if (workContext->address < SLAVE_DEVICE_ADDRESS_MIN || workContext->address > SLAVE_DEVICE_ADDRESS_MAX)
    {
        return;
    }

    address = workContext->address;
}



/**
 * start rtu slave poll
 */
void startRtuSlavePoll()
{
    if (!enabled)
    {
        return;
    }

    ModbusRtuEvent publishedEvent;
    if (!getEvent(&publishedEvent)) {
        return;
    }

    switch (publishedEvent) {
        case FRAME_RECEIVED_EVENT:
            disableUSART1ReceiveIT();

            unsigned char *pduFrame = (unsigned char *) &bytesBuffer;
            unsigned char *size = (unsigned char *) &receivedBytesBufferPosition;
            checkReceivedFrame(pduFrame, size);
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
            enableUSART1ReceiveIT();
            transmitterState = TRANSMITTER_IDLE_STATE;
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


static void checkReceivedFrame(unsigned char *frame, unsigned char *frameBytes)
{
    // check frame length
    if (*frameBytes < RTU_FRAME_CHAR_MINIMUM_BYTES)
    {
        // todo handle error: frame length is too short
        return;
    }

    // check CRC16
    unsigned short computedCrc16 = computeCRC16(frame, *frameBytes - RTU_FRAME_CRC_FIELD_BYTES);
    unsigned short receivedCrc16 = (frame[*frameBytes - 1] << 8) | frame[*frameBytes - 2];
    if (computedCrc16 != receivedCrc16)
    {
        return;
    }
}



/**
 * callback for receiving byte
 */
extern void receiveByteCallback()
{
   //  take away received byte from serial port
    unsigned char receivedByte;
    receiveByteFromSerial((char *) &receivedByte);

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
            enableTT35Timer();
            break;
        case RECEIVING_STATE:
            if (receivedBytesBufferPosition < RTU_FRAME_CHAR_MAXIMUM_BYTES)
            {
                bytesBuffer[receivedBytesBufferPosition++] = receivedByte;
                enableTT35Timer();
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
                publishEvent(FRAME_TRANSMITTED_EVENT);
            }
            break;
        default:
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
        // ignore when receiver is idle
        case RECEIVER_IDLE_STATE:
            break;
        case RECEIVING_STATE: // means a frame received
            publishEvent(FRAME_RECEIVED_EVENT);
            break;
        default:
            break;
    }

    // disable t35 timer
    disableT35Timer();
}

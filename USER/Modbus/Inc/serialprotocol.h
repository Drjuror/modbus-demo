#ifndef __MODBUS_SERIAL_PROTOCOL_H
#define __MODBUS_SERIAL_PROTOCOL_H


/**
 * Modbus serial supports two transmission modes. Either ASCII or RTU.
 */
typedef enum
{
    SERIAL_TRANSMISSION_MODE_RTU,
    SERIAL_TRANSMISSION_MODE_ASCII
} SerialTransmissionMode;


/* address rules start */
#define SERIAL_BROADCAST_ADDRESS (0)
#define MASTER_NODE_ADDRESS (0)
#define MIN_SLAVE_NODE_ADDRESS (1)
#define MAX_SLAVE_NODE_ADDRESS (247)
/* address rules end */


/* rtu pdu size begin */
#define RTU_FRAME_FUNCTION_CODE_CHAR_SIZE (1)
#define RTU_FRAME_DATA_CHAR_MAXIMUM_SIZE (252)
#define RTU_FRAME_ADDRESS_CHAR_OFFSET (0)
/* rtu pdu size end */


/* rtu fram field bytes begin */
// byte size of device address field, fixed at 1 byte
#define RTU_FRAME_DEVICE_ADDRESS_FIELD_BYTES (1)
// byte size of function code field, fixed at 1 byte
#define RTU_FRAME_FUNCTION_CODE_FIELE_BYTES (1)
#define RTU_FRAME_CRC_FIELD_BYTES (2)
#define RTU_FRAME_CHAR_MAXIMUM_SIZE (256)
/* rtu fram field bytes end */




void enable();

void serialReceiveByte(char *byte);
#endif
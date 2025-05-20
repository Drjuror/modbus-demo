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



/* devide address constants start */
#define SERIAL_BROADCAST_ADDRESS (0)
#define MASTER_DEVICE_ADDRESS (0)
#define SLAVE_DEVICE_ADDRESS_MIN (1)
#define SLAVE_DEVICE_ADDRESS_MAX (247)
/* devide address constants end */



/* rtu frame field bytes constants begin */
// byte size of device address field, fixed at 1 byte
#define RTU_FRAME_DEVICE_ADDRESS_FIELD_BYTES (1)
// byte size of function code field, fixed at 1 byte
#define RTU_FRAME_FUNCTION_CODE_FIELE_BYTES (1)
#define RTU_FRAME_CRC_FIELD_BYTES (2)
#define RTU_FRAME_DATA_FIELD_MAXIMUM_BYTES (252)

// the maximum size of a modbus rtu frame is 256 bytes
#define RTU_FRAME_CHAR_MAXIMUM_BYTES (RTU_FRAME_DEVICE_ADDRESS_FIELD_BYTES + \
    RTU_FRAME_FUNCTION_CODE_FIELE_BYTES + RTU_FRAME_DATA_FIELD_MAXIMUM_BYTES + \
    RTU_FRAME_CRC_FIELD_BYTES)
/* rtu frame field bytes constants end */



/**
 * init serial protocol
 */
void initSerialProtocol();

void serialReceiveByte(char *byte);
#endif
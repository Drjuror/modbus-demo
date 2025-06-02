#ifndef __MODBUS_RTU_PROTOCOL_H
#define __MODBUS_RTU_PROTOCOL_H


/**
 * modbus devices work mode includes master mode and slave mode.
 */
typedef enum
{
    WORK_MODE_MASTER,
    WORK_MODE_SLAVE
} ModbusDeviceWorkMode;


/**
 * Modbus Node work context includes the work mode, address.
 */
typedef struct
{
    ModbusDeviceWorkMode workMode; // master or slave
    unsigned char address; // slave address
} ModbusDeviceWorkContext;


typedef enum
{
    FRAME_RECEIVED_EVENT, // a frame received
    FRAME_TRANSMITTED_EVENT // a frame transmitted
} ModbusSlaveEvent;


typedef enum
{
    FRAME_RECEIVED_EVENT, // a frame received
    FRAME_TRANSMITTED_EVENT, // a frame transmitted
} ModbusMasterEvent;


typedef enum
{
    RECEIVER_IDLE_STATE,
    RECEIVING_STATE,
} ModbusReceiverState;



typedef enum
{
    TRANSMITTER_IDLE_STATE,
    TRANSMITTING_STATE,
} ModbusTransmitterState;


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
#define RTU_FRAME_FUNCTION_CODE_FIELD_BYTES (1)
#define RTU_FRAME_CRC_FIELD_BYTES (2)
#define RTU_FRAME_DATA_FIELD_MAXIMUM_BYTES (252)

// the maximum size of a modbus rtu frame is 256 bytes
#define RTU_FRAME_CHAR_MAXIMUM_BYTES (RTU_FRAME_DEVICE_ADDRESS_FIELD_BYTES + \
    RTU_FRAME_FUNCTION_CODE_FIELD_BYTES + \
    RTU_FRAME_DATA_FIELD_MAXIMUM_BYTES + \
    RTU_FRAME_CRC_FIELD_BYTES)

// the minimum size of a modbus rtu frame is 4 bytes
#define RTU_FRAME_CHAR_MINIMUM_BYTES (RTU_FRAME_DEVICE_ADDRESS_FIELD_BYTES + \
    RTU_FRAME_FUNCTION_CODE_FIELD_BYTES + \
    RTU_FRAME_CRC_FIELD_BYTES)
/* rtu frame field bytes constants end */

#endif

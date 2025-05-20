#ifndef __MODBUS_PROTOCOL_H
#define __MODBUS_PROTOCOL_H


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
    FRAME_RECEIVED_EVENT, // frame received
    FRAME_TRANSMITTED_EVENT // frame transmitted
} ModbusSlaveEvent;



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



void t35TimerExpiredCallback();
#endif

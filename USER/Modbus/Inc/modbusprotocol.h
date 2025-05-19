#ifndef __MODBUS_PROTOCOL_H
#define __MODBUS_PROTOCOL_H


/**
 * Modbus Node work role includes master and slave.
 */
typedef enum
{
    NODE_ROLE_MASTER,
    NODE_ROLE_SLAVE
} NodeWorkMode;


/**
 * Modbus Node work context includes the work mode, address, baudrate and parity.
 */
typedef struct
{
    NodeWorkMode workMode; // master or slave
    unsigned char address; // slave address
    unsigned long baudrate;
    unsigned char parity;
} ModbusNodeWorkContext;


typedef enum
{
    MODBUS_PROTOCOL_RTU,
    MODBUS_PROTOCOL_ASCII
} Nodes;


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

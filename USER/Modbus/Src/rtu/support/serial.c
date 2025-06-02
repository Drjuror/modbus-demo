
#include "modbusrtumode.h"


/**
 * receive byte from serial
 *
 * @param byte byte to be received
 */
extern void receiveByteFromSerial(char *byte)
{
    receiveByte(byte);
}


/**
 * transmit byte to serial port
 */
void transmitByte(unsigned char byte)
{
    sendByte(byte);
}


void serialRxISR()
{
    receiveByteCallback();
}


void serialTxISR()
{
    transmitByteCallback();
}

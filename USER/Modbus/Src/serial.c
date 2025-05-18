
#include "modbusprotocol.h"
#include "serialprotocol.h"



void init()
{

}


void enable()
{

}


void disable()
{

}


void serialReceiveByte(char *byte)
{
    receiveByte(byte);
}



void transmitByte(unsigned char buffer)
{
    sendByte(buffer);
}



void serialRxISR()
{
    receiveByteCallback();
}


void serialTxISR()
{
    unsigned char buffer = 1;
    sendByte(buffer);
}

#include "function.h"

#define ADDRESS_FIELD_BYTES (2)
#define QUANTITY_FIELD_BYTES (2)
#define PDU_BYTES (1 + ADDRESS_FIELD_BYTES + QUANTITY_FIELD_BYTES)

#define REGISTER_QUANTITY_MAX (0x7D)

unsigned short registerData[4];

void fillRegisterData(unsigned char *pdu, unsigned short startingAddress, unsigned short registerQuantity);


extern void readInputRegister(unsigned char *pdu, unsigned char *pduLength)
{
    if (* pduLength != PDU_BYTES)
    {
        return;
    }

    unsigned char index = 1;
    // function code check
    if (pdu[index] != FUNCTION_CODE_READ_INPUT_REGISTER)
    {
        return;
    }

    index++;
    unsigned short startingAddress = (unsigned short) (pdu[index++] << 8);
    startingAddress |= (unsigned short) pdu[index++];
    startingAddress++;

    unsigned short registerQuantity = (unsigned short) (pdu[index++] << 8);
    registerQuantity |= (unsigned short) pdu[index++];

    if (registerQuantity > 0 && registerQuantity <= REGISTER_QUANTITY_MAX)
    {
        fillRegisterData(pdu, startingAddress, registerQuantity);
    }
    else
    {
        // todo Handle error: invalid quantity
        return;
    }
}


void fillRegisterData(unsigned char *pdu, unsigned short startingAddress, unsigned short registerQuantity)
{
    registerData[0] = 0x11;
    registerData[1] = 0x22;
    registerData[2] = 0x33;
    registerData[3] = 0x44;

    pdu++;
    pdu++;

    // each register occupies 2 bytes
    *pdu = (unsigned char) 2 * registerQuantity;
    pdu++;

    unsigned char index = 0;
    for (index = 0; index < registerQuantity; index++)
    {
        *pdu = registerData[index] >> 8;
		pdu++;
        *pdu = registerData[index] & 0xff;
		pdu++;
    }
}

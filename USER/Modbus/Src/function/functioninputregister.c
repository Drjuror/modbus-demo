#include "function.h"

#define ADDRESS_FIELD_BYTES (2)
#define QUANTITY_FIELD_BYTES (2)
#define PDU_BYTES (1 + ADDRESS_FIELD_BYTES + QUANTITY_FIELD_BYTES)

#define REGISTER_QUANTITY_MAX (0x7D)


void readInputRegister(unsigned char * pdu, unsigned char pduLength)
{
    if (pduLength != PDU_BYTES)
    {
        return;
    }

    if (pdu[0] != FUNCTION_CODE_READ_INPUT_REGISTER)
    {
        return;
    }

    unsigned short quantity = pdu[3] << 8 | pdu[4];
    if (quantity > 0 && quantity <= REGISTER_QUANTITY_MAX)
    {
        
    }
    else
    {
        // todo Handle error: invalid quantity
        return;
    }


    unsigned short startingAddress = pdu[1] << 8 | pdu[2];
    startingAddress++;
}

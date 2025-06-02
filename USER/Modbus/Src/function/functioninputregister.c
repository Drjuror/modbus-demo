#include "modbusfunction.h"
#include "modbusrtuprotocol.h"


// byte size of starting address field, fixed at 2 bytes
#define STARTING_ADDRESS_FIELD_BYTES (2)
// byte size of the quantity of input registers field, fixed at 2 bytes
#define QUANTITY_FIELD_BYTES (2)
// total byte size of pdu, including function code, starting address and quantity of input registers
#define PDU_BYTES (RTU_FRAME_FUNCTION_CODE_FIELE_BYTES + STARTING_ADDRESS_FIELD_BYTES + QUANTITY_FIELD_BYTES)

// maximum number of input registers
#define REGISTER_QUANTITY_MAXIMUM (0x7D)


/**
 * fill register data into pdu frame
 *
 * @param pduFrame         pdu frame
 * @param pduFrameBytes    byte size of pdu frame
 * @param startingAddress  starting address of input register
 * @param registerQuantity number of input registers
 */
static void fillRegisterDataIntoPduFrame(unsigned char *pduFrame,
                                         unsigned char *pduFrameBytes,
                                         unsigned short startingAddress,
                                         unsigned short registerQuantity);


/**
 * read input register function
 *
 * @param pduFrame      pdu frame
 * @param pduFrameBytes byte size of pdu frame
 */
extern void readInputRegister(unsigned char *pduFrame, unsigned char *pduFrameBytes)
{
    if (*pduFrameBytes != PDU_BYTES)
    {
        return;
    }

    unsigned char index = 0;
    // function code check
    if (pduFrame[index] != READ_INPUT_REGISTER_FUNCTION_CODE)
    {
        return;
    }

    index++;

    // extracet starting address
    // address starts from 1 not 0 actually
    unsigned short startingAddress = (pduFrame[index++] << 8) | pduFrame[index++] + 1;

    // extract input register quantity
    unsigned short registerQuantity = (pduFrame[index++] << 8) | pduFrame[index++];
    // check input register quantity
    if (registerQuantity > 0 && registerQuantity <= REGISTER_QUANTITY_MAXIMUM)
    {
        fillRegisterDataIntoPduFrame(pduFrame, pduFrameBytes, startingAddress, registerQuantity);
    }
    else
    {
        // todo Handle error: invalid quantity
        return;
    }
}


static void fillRegisterDataIntoPduFrame(unsigned char *pduFrame,
                                         unsigned char *pduFrameBytes,
                                         unsigned short startingAddress,
                                         unsigned short registerQuantity)
{
    // todo just for test
    unsigned short testNumber = 0x01;

    *pduFrameBytes = RTU_FRAME_FUNCTION_CODE_FIELE_BYTES;
    // move pdu frame pointer to the first byte of data field
    pduFrame++;

    // set byte count field, fixed at 1 byte
    // each register occupies 2 bytes
    *pduFrame = (unsigned char) 2 * registerQuantity;
    pduFrame++;
    *pduFrameBytes += 1;

    unsigned char index = 0;
    for (index = 0; index < registerQuantity; index++)
    {
        // take high-order byte
        *pduFrame = testNumber >> 8;
		pduFrame++;
        *pduFrameBytes += 1;

        // take low-order byte
        *pduFrame = testNumber & 0xff;
		pduFrame++;
        *pduFrameBytes += 1;
    }
}

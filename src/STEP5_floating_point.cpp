
#include "STEP5_floating_point.h"

using namespace std;

//---------------------------------------------------------------------------
// ��������������� ������ ������� - STEP5 float 3 byte, �� float IEEE754.
float fStep5ToFloat(unsigned char *pucSource)
{
    union Union
    {
        unsigned char aucReseiver[4];
        unsigned long ulIntermediate;
        float fResult;
    };

    unsigned long ulTemp;
    unsigned short usMantiss;
    unsigned char ucSign = 0x00;
    Union xUnion;

    xUnion.aucReseiver[0] = 0;
    xUnion.aucReseiver[1] = *(pucSource);
    xUnion.aucReseiver[2] = *(pucSource + 1);
    xUnion.aucReseiver[3] = *(pucSource + 2);

    if(xUnion.ulIntermediate == 0x00L)
    {
        return xUnion.fResult;
    }

    usMantiss = (unsigned short)(xUnion.ulIntermediate >> 8);

    if(xUnion.aucReseiver[2] & 0x80)
    {
        ucSign =  0x80;
        usMantiss = -usMantiss;

    }

    usMantiss = usMantiss << 2;
    ulTemp = (unsigned long)usMantiss;
    ulTemp = ulTemp << 8;
    ulTemp &= 0x00FFFF00;
    xUnion.ulIntermediate &= 0xFF000000;
    xUnion.ulIntermediate |= ulTemp;

    xUnion.aucReseiver[3] = xUnion.aucReseiver[3] - 1 + 127;
    xUnion.ulIntermediate   = xUnion.ulIntermediate >> 1;
    xUnion.aucReseiver[3] = xUnion.aucReseiver[3] | ucSign;

    return xUnion.fResult;
}

//---------------------------------------------------------------------------
// ��������������� ������ ������� - float IEEE754, � STEP5 float 3 byte.
void  vFloatToStep5(unsigned char *pucDestination, float fData)
{
    union Union
    {
        unsigned char aucReseiver[4];
        unsigned long ulIntermediate;
        float fResult;
    };

    unsigned long ulTemp;
    unsigned short usMantiss;
    unsigned char ucSign = 0x00;
    Union xUnion;

    xUnion.fResult = fData;

    if(xUnion.ulIntermediate != 0x00L)
    {
        ucSign = xUnion.aucReseiver[3] & 0x80;

        xUnion.ulIntermediate = xUnion.ulIntermediate << 1;
        xUnion.aucReseiver[3] = xUnion.aucReseiver[3] - 127 + 1;

        usMantiss = (unsigned short)(xUnion.ulIntermediate >> 8);
        usMantiss = usMantiss >> 2;
        usMantiss |= 0x4000;
        if(ucSign)
        {
            usMantiss = -usMantiss;
        }

        ulTemp = (unsigned long)usMantiss;
        ulTemp = ulTemp << 8;
        ulTemp &= 0x00FFFF00;
        xUnion.ulIntermediate &= 0xFF000000;
        xUnion.ulIntermediate |= ulTemp;
    }

    *(pucDestination) = xUnion.aucReseiver[1];
    *(pucDestination + 1) = xUnion.aucReseiver[2];
    *(pucDestination + 2) = xUnion.aucReseiver[3];

    return;
}

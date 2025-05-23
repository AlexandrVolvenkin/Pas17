#ifndef CRC_H_INCLUDED
#define CRC_H_INCLUDED


#include <stdint.h>

//-----------------------------------------------------------------------------
// crc16 constants.
//-----------------------------------------------------------------------------
#define START_VAL_CRC16 0xFFFF;
#define CRC16_LENGTH 2;
//-----------------------------------------------------------------------------


unsigned short usCrcSummTwoByteCalculation(uint8_t *puiSource, unsigned int nuiNbyte);
int iCrcSummTwoByteCompare(uint8_t *puiSource, unsigned int nuiNbyte);
uint8_t usCrcSummOneByteCalculation(
    uint8_t *puiSource,
    uint16_t nuiNbyte);
int iCrcSummOneByteCompare(uint8_t *puiSource, unsigned int nuiNbyte);
//unsigned short usCrc16(uint8_t *puiSource, unsigned short uiLength);
int iCrc16Check(uint8_t *puiSource, unsigned short uiLength);
uint16_t usCrc16(const uint8_t *puiSource, uint16_t uiLength);
uint8_t HammingDistanceCrc(uint8_t* puiSource,
                           uint8_t uiLength);

#endif // CRC_H_INCLUDED

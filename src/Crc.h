#ifndef CRC_H_INCLUDED
#define CRC_H_INCLUDED


//-----------------------------------------------------------------------------
// crc16 constants.
//-----------------------------------------------------------------------------
#define START_VAL_CRC16 0xFFFF
#define CRC16_LENGTH 2

//-----------------------------------------------------------------------------
class CCrc
{
public:
    typedef enum
    {
        HANDLED_ARRAY_NUMBER = 7
    };

    struct TCrcContext
    {
        // длина массива.
        uint8_t *pui8ArrayPointer;
        uint16_t ui16Length;
        uint16_t ui16Crc;
    };

    void Create(uint8_t );
    void CreateAll(void);
    int8_t Check(uint8_t );
    int8_t CheckAll(void);

    TCrcContext axTCrcContext[HANDLED_ARRAY_NUMBER];
    static TCrcContext axTCrcContextInit[HANDLED_ARRAY_NUMBER];
    uint16_t ui16Crc;
    int8_t i8Error;

public:
    CCrc()
    {

    }

    ~CCrc()
    {

    }
};

extern CCrc xCCrc;

unsigned short usCrcSummTwoByteCalculation(unsigned char *pucSource, unsigned int nuiNbyte);
int iCrcSummTwoByteCompare(unsigned char *pucSource, unsigned int nuiNbyte);
int iCrcSummOneByteCompare(unsigned char *pucSource, unsigned int nuiNbyte);
unsigned short CRC16(unsigned char *pucSource, unsigned short nusLength);
int iCrc16Check(unsigned char *pucSource, unsigned short nusLength);
unsigned short usCrc16(const unsigned char *pucSource, unsigned short nusLength);
uint8_t HammingDistanceCrc(uint8_t* puiSource,
                           uint8_t uiLength);

#endif // CRC_H_INCLUDED

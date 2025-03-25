#ifndef RINGBUFFER_H_INCLUDED
#define RINGBUFFER_H_INCLUDED

//-----------------------------------------------------------------------------------------------------

#include <stdint.h>

////-----------------------------------------------------------------------------------------------------
//class CRingBuffer
//{
//public:
//    void i8Create(uint8_t );
//    void i16Create(uint16_t );
//    void i32Create(uint32_t );
//
////    template <typename Type>
////    void Push(Type );
//
//    void i8Push(int8_t );
//    void i16Push(int16_t );
//    void i32Push(int32_t );
//
//    int8_t i8Pop(void);
//    int16_t i16Pop(void);
//    int32_t i32Pop(void);
//
//    void PopIndexSet(uint8_t );
//    void Clear(void);
//    int16_t Integrator(int16_t , uint8_t );
//    int16_t Adder(int16_t , uint8_t );
//
//    uint8_t ui8IntegrationDepth;
//    int16_t i16PreviousData;
//    int16_t i16CurrentData;
//    int16_t ui8ResultMin;
//    int16_t ui8ResultMax;
////-----------------------------------------------------------------------------------------------------
//private:
//    int8_t *pi8RingBuffer;
//    int16_t *pi16RingBuffer;
//    int32_t *pi32RingBuffer;
//    uint8_t ui8RingBufferPushIndex;
//    uint8_t ui8RingBufferPopIndex;
//    uint8_t ui8RingBufferLength;
//    uint8_t ui8RingBufferLengthMask;
//
////    template <typename Type>
////    Type *pTRingBuffer;
//
////-----------------------------------------------------------------------------------------------------
////    void Push(uint8_t ui8Data)
////    {
////        ui8RingBufferPushIndex &= ui8RingBufferLengthMask;
////        pi8RingBuffer[ui8RingBufferPushIndex++] = ui8Data;
////    }
////
////    uint8_t Pop(void)
////    {
////        ui8RingBufferPopIndex &= ui8RingBufferLengthMask;
////        return pi8RingBuffer[ui8RingBufferPopIndex++];
////    }
//
////-----------------------------------------------------------------------------------------------------
//public:
////    void Push(int16_t i16Data)
////    {
//////        if (ui8RingBufferPushIndex >= ui8RingBufferLength)
//////        {
//////            ui8RingBufferPushIndex = 0;
//////        }
////        ui8RingBufferPushIndex &= ui8RingBufferLengthMask;
////        pi16RingBuffer[ui8RingBufferPushIndex++] = i16Data;
////    }
////
////    uint16_t Pop(void)
////    {
//////        if (ui8RingBufferPopIndex >= ui8RingBufferLength)
//////        {
//////            ui8RingBufferPopIndex = 0;
//////        }
////        ui8RingBufferPopIndex &= ui8RingBufferLengthMask;
////        return pi16RingBuffer[ui8RingBufferPopIndex++];
////    }
//
////-----------------------------------------------------------------------------------------------------
////    void Push(int32_t i32Data)
////    {
////        ui8RingBufferPushIndex &= ui8RingBufferLengthMask;
////        pi32RingBuffer[ui8RingBufferPushIndex++] = i32Data;
////    }
//
////    int32_t Pop(void)
////    {
////        ui8RingBufferPopIndex &= ui8RingBufferLengthMask;
////        return pi32RingBuffer[ui8RingBufferPopIndex++];
////    }
//
//////-----------------------------------------------------------------------------------------------------
////    void PopIndexSet(uint8_t ui8Length)
////    {
////        ui8RingBufferPopIndex = ui8RingBufferPushIndex;
////        ui8RingBufferPopIndex -= ui8Length;
////        ui8RingBufferPopIndex &= ui8RingBufferLengthMask;
////
//////        ui8RingBufferPopIndex = ui8RingBufferPushIndex - ui8Length;
//////        if (ui8RingBufferPopIndex < 0)
//////        {
//////            ui8RingBufferPopIndex = ui8RingBufferLength + ui8RingBufferPopIndex;
//////        }
////    }
//
////-----------------------------------------------------------------------------------------------------
//public:
//    CRingBuffer()
//    {
////        pi16RingBuffer = new uint16_t[ui8RingBufferLength];
////        pi16RingBuffer = new uint16_t[16];
//    }
//    ~CRingBuffer()
//    {
//        delete[] pi32RingBuffer;
//    }
//};

//-----------------------------------------------------------------------------------------------------
template <class Type>
class CRingBuffer
{
public:
    void Create(Type Length);
    void Destroy(void);
    void Push(Type Data);
    Type Pop(void);
    void PopIndexSet(Type Length);

//-----------------------------------------------------------------------------------------------------
private:
    Type *pTRingBuffer;
    Type TRingBufferPushIndex;
    Type TRingBufferPopIndex;
    Type TRingBufferLength;
    Type TRingBufferLengthMask;

//-----------------------------------------------------------------------------------------------------
public:
    CRingBuffer()
    {

    }
    ~CRingBuffer()
    {

    }
};

//-----------------------------------------------------------------------------------------------------
extern CRingBuffer<int32_t> xCMainCycleMeasureRingBuffer;
//extern CRingBuffer<int32_t> xCCodeSysCycleMeasureRingBuffer;
//extern CRingBuffer<int32_t> xCCodeSysCycleMeasureRingBuffer2;
//extern CRingBuffer<int32_t> xCCodeSysCycleMeasureRingBuffer3;

#endif // RINGBUFFER_H_INCLUDED

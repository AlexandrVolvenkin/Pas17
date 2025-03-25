//-----------------------------------------------------------------------------------------------------

#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "RingBuffer.h"

CRingBuffer<int32_t> xCMainCycleMeasureRingBuffer;
//CRingBuffer<int32_t> xCCodeSysCycleMeasureRingBuffer;
//CRingBuffer<int32_t> xCCodeSysCycleMeasureRingBuffer2;
//CRingBuffer<int32_t> xCCodeSysCycleMeasureRingBuffer3;

//-----------------------------------------------------------------------------------------------------
template <class Type>
void CRingBuffer<Type>::Create(Type Length)
{
    TRingBufferLength = Length;
    TRingBufferLengthMask = Length - 1;
    pTRingBuffer = new Type[Length];
}

//-----------------------------------------------------------------------------------------------------
template <class Type>
void CRingBuffer<Type>::Destroy(void)
{
    delete[] pTRingBuffer;
}

//-----------------------------------------------------------------------------------------------------
template <class Type>
void CRingBuffer<Type>::Push(Type Data)
{
    TRingBufferPushIndex &= TRingBufferLengthMask;
    pTRingBuffer[TRingBufferPushIndex++] = Data;
}

//-----------------------------------------------------------------------------------------------------
template <class Type>
Type CRingBuffer<Type>::Pop(void)
{
    TRingBufferPopIndex &= TRingBufferLengthMask;
    return pTRingBuffer[TRingBufferPopIndex++];
}

//-----------------------------------------------------------------------------------------------------
template <class Type>
void CRingBuffer<Type>::PopIndexSet(Type Length)
{
    TRingBufferPopIndex = TRingBufferPushIndex;
    TRingBufferPopIndex -= Length;
    TRingBufferPopIndex &= TRingBufferLengthMask;
}

//-----------------------------------------------------------------------------------------------------
template class CRingBuffer<int32_t>;

////-----------------------------------------------------------------------------------------------------
//void CRingBuffer::i8Create(uint8_t ui8Length)
//{
//    ui8RingBufferLength = ui8Length;
//    ui8RingBufferLengthMask = ui8Length - 1;
//    pi8RingBuffer = new int8_t[ui8Length];
//}
//
////-----------------------------------------------------------------------------------------------------
//void CRingBuffer::i16Create(uint16_t ui16Length)
//{
//    ui8RingBufferLength = ui16Length;
//    ui8RingBufferLengthMask = ui16Length - 1;
//    pi16RingBuffer = new int16_t[ui16Length];
//}
//
////-----------------------------------------------------------------------------------------------------
//void CRingBuffer::i32Create(uint32_t ui32Length)
//{
//    ui8RingBufferLength = ui32Length;
//    ui8RingBufferLengthMask = ui32Length - 1;
//    pi32RingBuffer = new int32_t[ui32Length];
//}
//
////-----------------------------------------------------------------------------------------------------
//void CRingBuffer::i8Push(int8_t i8Data)
//{
//    ui8RingBufferPushIndex &= ui8RingBufferLengthMask;
//    pi8RingBuffer[ui8RingBufferPushIndex++] = i8Data;
//}
//
////-----------------------------------------------------------------------------------------------------
//void CRingBuffer::i16Push(int16_t i16Data)
//{
//    ui8RingBufferPushIndex &= ui8RingBufferLengthMask;
//    pi16RingBuffer[ui8RingBufferPushIndex++] = i16Data;
//}
//
////-----------------------------------------------------------------------------------------------------
//void CRingBuffer::i32Push(int32_t i32Data)
//{
//    ui8RingBufferPushIndex &= ui8RingBufferLengthMask;
//    pi32RingBuffer[ui8RingBufferPushIndex++] = i32Data;
//}
//
////-----------------------------------------------------------------------------------------------------
//int8_t CRingBuffer::i8Pop(void)
//{
//    ui8RingBufferPopIndex &= ui8RingBufferLengthMask;
//    return pi8RingBuffer[ui8RingBufferPopIndex++];
//}
//
////-----------------------------------------------------------------------------------------------------
//int16_t CRingBuffer::i16Pop(void)
//{
//    ui8RingBufferPopIndex &= ui8RingBufferLengthMask;
//    return pi16RingBuffer[ui8RingBufferPopIndex++];
//}
//
////-----------------------------------------------------------------------------------------------------
//int32_t CRingBuffer::i32Pop(void)
//{
//    ui8RingBufferPopIndex &= ui8RingBufferLengthMask;
//    return pi32RingBuffer[ui8RingBufferPopIndex++];
//}
//
////-----------------------------------------------------------------------------------------------------
//void CRingBuffer::PopIndexSet(uint8_t ui8Length)
//{
//    ui8RingBufferPopIndex = ui8RingBufferPushIndex;
//    ui8RingBufferPopIndex -= ui8Length;
//    ui8RingBufferPopIndex &= ui8RingBufferLengthMask;
//}
//
////-----------------------------------------------------------------------------------------------------
//void CRingBuffer::Clear(void)
//{
//    memset(pi16RingBuffer,
//           0,
//           (ui8RingBufferLength * sizeof(int16_t)));
//}
//
////-----------------------------------------------------------------------------------------------------
//int16_t CRingBuffer::Integrator(int16_t i16Data, uint8_t ui8Depth)
//{
//    // push.
//    ui8RingBufferPushIndex &= ui8RingBufferLengthMask;
//    pi16RingBuffer[ui8RingBufferPushIndex++] = i16Data;
//    i16Data = 0;
//    // pop index set.
//    ui8RingBufferPopIndex = ui8RingBufferPushIndex;
//    ui8RingBufferPopIndex -= ui8Depth;
//    ui8RingBufferPopIndex &= ui8RingBufferLengthMask;
//    for (int j = 0;
//            j < ui8Depth;
//            j++)
//    {
//        // pop.
//        ui8RingBufferPopIndex &= ui8RingBufferLengthMask;
//        i16Data += pi16RingBuffer[ui8RingBufferPopIndex++];
//    }
//    return i16Data / ui8Depth;
//}
//
////-----------------------------------------------------------------------------------------------------
//int16_t CRingBuffer::Adder(int16_t i16Data, uint8_t ui8Depth)
//{
//    // push.
//    ui8RingBufferPushIndex &= ui8RingBufferLengthMask;
//    pi16RingBuffer[ui8RingBufferPushIndex++] = i16Data;
//    i16Data = 0;
//    // pop index set.
//    ui8RingBufferPopIndex = ui8RingBufferPushIndex;
//    ui8RingBufferPopIndex -= ui8Depth;
//    ui8RingBufferPopIndex &= ui8RingBufferLengthMask;
//    for (int j = 0;
//            j < ui8Depth;
//            j++)
//    {
//        // pop.
//        ui8RingBufferPopIndex &= ui8RingBufferLengthMask;
//        i16Data += pi16RingBuffer[ui8RingBufferPopIndex++];
//    }
//    return i16Data;
//}


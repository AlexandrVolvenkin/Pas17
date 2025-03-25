
//-----------------------------------------------------------------------------------------------------
//  Source      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-----------------------------------------------------------------------------------------------------
#ifndef CSTORAGEDEVICE_H
#define CSTORAGEDEVICE_H

#include <stdint.h>
#include <iostream>
#include <fstream>

#include "DataTypes.h"
#include "Dfa.h"
#include "Timer.h"

using namespace std;

//-----------------------------------------------------------------------------------------------------
class CStorageDeviceInterface : public CDfa
{
public:
    virtual uint8_t PassingDataAndStartWrite(uint16_t uiOffset, uint8_t *puiSource, uint16_t uiLength) {};
    virtual uint8_t Write(void) {};
    virtual uint8_t Read(uint8_t *puiDestination, uint16_t uiOffset, uint16_t uiLength) {};
    virtual void SetIsDataWrited(bool bStatus) {};
    virtual bool IsDataWrited(void) {};
    virtual bool IsReadyToWrite(void) {};

    virtual void SetBufferPointer(uint8_t* puiBuffer) {};

    virtual uint8_t* GetBufferPointer(void) {};

    virtual void SetOffset(uint16_t uiOffset) {};
    virtual uint16_t GetOffset(void) {};

    virtual void SetLength(uint16_t uiLength) {};
    virtual uint16_t GetLength(void) {};

    virtual void SetByteCounter(uint16_t nuiByteCounter) {};
    virtual uint16_t GetByteCounter(void) {};
};
//-----------------------------------------------------------------------------------------------------







//-----------------------------------------------------------------------------------------------------
class CStorageDevice : public CStorageDeviceInterface
{
public:
    virtual uint8_t PassingDataAndStartWrite(uint16_t uiOffset, uint8_t *puiSource, uint16_t uiLength) = 0;
    virtual uint8_t Write(void) = 0;
    virtual uint8_t Read(uint8_t *puiDestination, uint16_t uiOffset, uint16_t uiLength) = 0;
//    virtual void SetIsDataWrited(bool bStatus) = 0;
//    virtual bool IsDataWrited(void) = 0;
    virtual bool IsReadyToWrite(void) = 0;

    virtual void SetBufferPointer(uint8_t* puiBuffer)
    {
        m_puiBuffer = puiBuffer;
    };

    virtual uint8_t* GetBufferPointer(void)
    {
        return m_puiBuffer;
    };

    virtual void SetOffset(uint16_t uiOffset)
    {
        m_uiOffset = uiOffset;
    };
    virtual uint16_t GetOffset(void)
    {
        return m_uiOffset;
    };

    virtual void SetLength(uint16_t uiLength)
    {
        m_uiLength = uiLength;
    };
    virtual uint16_t GetLength(void)
    {
        return m_uiLength;
    };

    virtual void SetByteCounter(uint16_t nuiByteCounter)
    {
        m_nuiByteCounter = nuiByteCounter;
    };
    virtual uint16_t GetByteCounter(void)
    {
        return m_nuiByteCounter;
    };

    void SetIsDataWrited(bool bStatus)
    {
        m_bDataIsWrited = bStatus;
    };
    bool IsDataWrited(void)
    {
        return m_bDataIsWrited;
    };

protected:
    uint16_t m_nuiByteCounter;
    uint16_t m_uiOffset;
    uint8_t* m_puiBuffer;
    uint16_t m_uiLength;
    // ���� - ������ ��������.
    bool m_bDataIsWrited;
    const char *pccFileName;
};
//-----------------------------------------------------------------------------------------------------









//-----------------------------------------------------------------------------------------------------
class CStorageDeviceFileSystem : public CStorageDevice
{
public:

    enum
    {
        MAX_BUFFER_LENGTH = 25600,
    };

    CStorageDeviceFileSystem();
    virtual ~CStorageDeviceFileSystem();

    uint8_t PassingDataAndStartWrite(uint16_t uiOffset, uint8_t *puiSource, uint16_t uiLength);
    uint8_t Read(uint8_t *puiDestination, uint16_t uiOffset, uint16_t uiLength);
    bool IsReadyToWrite(void)
    {
        // ���������� ���������� � ������.
        // � ������ ������ ���������� �� �������� �� ����������.
        return true;
    };

private:
    uint8_t Write(void);
};

//-----------------------------------------------------------------------------------------------------









//-----------------------------------------------------------------------------------------------------
class CStorageDeviceFram : public CStorageDeviceFileSystem
{
public:

    enum
    {
        MAX_BUFFER_LENGTH = 25600,
    };

    CStorageDeviceFram();
    virtual ~CStorageDeviceFram();
};

//-----------------------------------------------------------------------------------------------------









//-----------------------------------------------------------------------------------------------------
#endif // CSTORAGEDEVICE_H

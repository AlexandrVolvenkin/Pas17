//-------------------------------------------------------------------------------
//  Source      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-------------------------------------------------------------------------------
#ifndef CDATACONTAINER_H
#define CDATACONTAINER_H

#include <stdint.h>
#include <iostream>

//-------------------------------------------------------------------------------
class CDataContainerInterface
{
public:

    virtual void SetFsmCommandState(uint8_t value) {};
    virtual uint8_t GetFsmCommandState() const {};

    virtual void SetFsmOperationStatus(uint8_t value) {};
    virtual uint8_t GetFsmOperationStatus() const {};

    virtual void SetDataIndex(uint16_t value) {};
    virtual uint16_t GetDataIndex() const {};

    virtual void SetDataPointer(uint8_t* value) {};
    virtual uint8_t* GetDataPointer() const {};

    virtual void SetDataOffset(uint32_t value) {};
    virtual uint32_t GetDataOffset() const {};

    virtual void SetDataLength(uint32_t value) {};
    virtual uint32_t GetDataLength() const {};

    virtual void SetContainerData(CDataContainerInterface* pxDataContainer) {};
    virtual void SetContainerData(uint8_t uiFsmCommandState,
                                  uint16_t uiDataIndex,
                                  uint8_t *puiDataPointer,
                                  uint32_t uiDataOffset,
                                  uint32_t uiDataLength) {};

protected:

private:
};

//-------------------------------------------------------------------------------





//-------------------------------------------------------------------------------
class CDataContainerDataBase : public CDataContainerInterface
{
public:
    CDataContainerDataBase();
    virtual ~CDataContainerDataBase();

    void SetFsmCommandState(uint8_t value);
    uint8_t GetFsmCommandState() const;

    void SetFsmOperationStatus(uint8_t value);
    uint8_t GetFsmOperationStatus() const;

    void SetDataIndex(uint16_t value);
    uint16_t GetDataIndex() const;

    void SetDataPointer(uint8_t* value);
    uint8_t* GetDataPointer() const;

    void SetDataOffset(uint32_t value);
    uint32_t GetDataOffset() const;

    void SetDataLength(uint32_t value);
    uint32_t GetDataLength() const;

    void SetContainerData(CDataContainerInterface* pxDataContainer);
    void SetContainerData(uint8_t uiFsmCommandState,
                          uint16_t uiDataIndex,
                          uint8_t *puiDataPointer,
                          uint32_t uiDataOffset,
                          uint32_t uiDataLength);

public:
    uint8_t m_uiFsmCommandState;
    uint8_t m_uiFsmOperationStatus;
    uint16_t m_uiDataIndex;
    uint8_t* m_puiDataPointer;
    uint32_t m_uiDataOffset;
    uint32_t m_uiDataLength;
};

//-------------------------------------------------------------------------------
#endif // CDATACONTAINER_H

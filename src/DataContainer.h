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

    void SetContainerData(uint8_t uiFsmCommandState,
                          uint16_t uiDataIndex,
                          uint8_t *puiDataPointer,
                          uint32_t uiDataOffset,
                          uint32_t uiDataLength);

public:
    uint8_t m_uiFsmCommandState;
//    CDataContainerInterface* m_pxCommandDataPointer;
    uint16_t m_uiDataIndex;
    uint8_t* m_puiDataPointer;
    uint32_t m_uiDataOffset;
    uint32_t m_uiDataLength;
};

//-------------------------------------------------------------------------------
#endif // CDATACONTAINER_H

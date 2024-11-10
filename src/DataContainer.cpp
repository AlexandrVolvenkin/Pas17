
//-------------------------------------------------------------------------------
//  Source      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-------------------------------------------------------------------------------
#include <iostream>

#include "DataContainer.h"

//-------------------------------------------------------------------------------
CDataContainerDataBase::CDataContainerDataBase()
{
    //ctor
}

//-------------------------------------------------------------------------------
CDataContainerDataBase::~CDataContainerDataBase()
{
    //dtor
}

//-------------------------------------------------------------------------------
// Setter for m_uiDataIndex
void CDataContainerDataBase::SetFsmCommandState(uint8_t value)
{
    m_uiFsmCommandState = value;
}

// Getter for m_uiFsmCommandState
uint8_t CDataContainerDataBase::GetFsmCommandState() const
{
    return m_uiFsmCommandState;
}

//-------------------------------------------------------------------------------
// Setter for m_uiDataIndex
void CDataContainerDataBase::SetDataIndex(uint16_t value)
{
    m_uiDataIndex = value;
}

// Getter for m_uiDataIndex
uint16_t CDataContainerDataBase::GetDataIndex() const
{
    return m_uiDataIndex;
}

//-------------------------------------------------------------------------------
// Setter for m_puiDataPointer
void CDataContainerDataBase::SetDataPointer(uint8_t* value)
{
    m_puiDataPointer = value;
}

// Getter for m_puiDataPointer
uint8_t* CDataContainerDataBase::GetDataPointer() const
{
    return m_puiDataPointer;
}

//-------------------------------------------------------------------------------
// Setter for m_uiDataOffset
void CDataContainerDataBase::SetDataOffset(uint32_t value)
{
    m_uiDataOffset = value;
}

// Getter for m_uiDataOffset
uint32_t CDataContainerDataBase::GetDataOffset() const
{
    return m_uiDataOffset;
}

//-------------------------------------------------------------------------------
// Setter for m_uiDataLength
void CDataContainerDataBase::SetDataLength(uint32_t value)
{
    m_uiDataLength = value;
}

// Getter for m_uiDataLength
uint32_t CDataContainerDataBase::GetDataLength() const
{
    return m_uiDataLength;
}

//-------------------------------------------------------------------------------
void CDataContainerDataBase::SetContainerData(CDataContainerInterface* pxDataContainer)
{
    CDataContainerDataBase* pxDataContainerLocal =
        (static_cast<CDataContainerDataBase*>(pxDataContainer));

    m_uiFsmCommandState = pxDataContainerLocal -> m_uiFsmCommandState;
    m_uiFsmAnswerState = pxDataContainerLocal -> m_uiFsmAnswerState;
    m_uiDataIndex = pxDataContainerLocal -> m_uiDataIndex;
    m_puiDataPointer = pxDataContainerLocal -> m_puiDataPointer;
    m_uiDataOffset = pxDataContainerLocal -> m_uiDataOffset;
    m_uiDataLength = pxDataContainerLocal -> m_uiDataLength;
}

//-------------------------------------------------------------------------------
void CDataContainerDataBase::SetContainerData(uint8_t uiFsmCommandState,
        uint16_t uiDataIndex,
        uint8_t *puiDataPointer,
        uint32_t uiDataOffset,
        uint32_t uiDataLength)
{
    m_uiFsmCommandState = uiFsmCommandState;
    m_uiDataIndex = uiDataIndex;
    m_puiDataPointer = puiDataPointer;
    m_uiDataOffset = uiDataOffset;
    m_uiDataLength = uiDataLength;
}

//-------------------------------------------------------------------------------





////-------------------------------------------------------------------------------
//CDataContainerDataBase::CDataContainerDataBase()
//{
//    //ctor
//}
//
////-------------------------------------------------------------------------------
//CDataContainerDataBase::~CDataContainerDataBase()
//{
//    //dtor
//}


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
void CDataContainerDataBase::SetTaskId(uint8_t value)
{
    m_uiTaskId = value;
}

uint8_t CDataContainerDataBase::GetTaskId() const
{
    return m_uiTaskId;
}

//-------------------------------------------------------------------------------
void CDataContainerDataBase::SetFsmCommandState(uint8_t value)
{
    m_uiFsmCommandState = value;
}

uint8_t CDataContainerDataBase::GetFsmCommandState() const
{
    return m_uiFsmCommandState;
}

//-------------------------------------------------------------------------------
void CDataContainerDataBase::SetFsmOperationStatus(uint8_t value)
{
    m_uiFsmOperationStatus = value;
}

uint8_t CDataContainerDataBase::GetFsmOperationStatus() const
{
    return m_uiFsmOperationStatus;
}

//-------------------------------------------------------------------------------
void CDataContainerDataBase::SetDataIndex(uint16_t value)
{
    m_uiDataIndex = value;
}

uint16_t CDataContainerDataBase::GetDataIndex() const
{
    return m_uiDataIndex;
}

//-------------------------------------------------------------------------------
void CDataContainerDataBase::SetDataPointer(uint8_t* value)
{
    m_puiDataPointer = value;
}

uint8_t* CDataContainerDataBase::GetDataPointer() const
{
    return m_puiDataPointer;
}

//-------------------------------------------------------------------------------
void CDataContainerDataBase::SetDataOffset(uint32_t value)
{
    m_uiDataOffset = value;
}

uint32_t CDataContainerDataBase::GetDataOffset() const
{
    return m_uiDataOffset;
}

//-------------------------------------------------------------------------------
void CDataContainerDataBase::SetDataLength(uint32_t value)
{
    m_uiDataLength = value;
}

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
    m_uiFsmOperationStatus = pxDataContainerLocal -> m_uiFsmOperationStatus;
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

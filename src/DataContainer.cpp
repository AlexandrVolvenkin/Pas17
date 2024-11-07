
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
void CDataContainerDataBase::SetContainerData(uint8_t uiFsmCommandState,
                          uint16_t uiDataIndex,
                          uint8_t *puiDataPointer,
                          uint32_t uiDataOffset,
                          uint32_t uiDataLength)
{
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

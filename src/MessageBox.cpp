
//-------------------------------------------------------------------------------
//  Source      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-------------------------------------------------------------------------------
#include <iostream>

#include "DataContainer.h"
#include "MessageBox.h"

//-------------------------------------------------------------------------------
CMessageBoxGeneral::CMessageBoxGeneral()
{
    //ctor
}

//-------------------------------------------------------------------------------
CMessageBoxGeneral::~CMessageBoxGeneral()
{
    //dtor
}

//-------------------------------------------------------------------------------
void CMessageBoxGeneral::SetDataContainer(CDataContainerInterface* pxDataContainer)
{
    m_pxDataContainer = pxDataContainer;
}

//-------------------------------------------------------------------------------
CDataContainerInterface* CMessageBoxGeneral::GetDataContainerPointer(void)
{
    return m_pxDataContainer;
}

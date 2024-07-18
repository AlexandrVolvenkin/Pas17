//-------------------------------------------------------------------------------
//  Sourse      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-------------------------------------------------------------------------------
#include "ServiceMarket.h"
#include "Platform.h"
#include "Task.h"
#include "Resources.h"
#include "DataStore.h"

using namespace std;

//-------------------------------------------------------------------------------
CServiceMarket::CServiceMarket()
{
    //ctor
}

//-------------------------------------------------------------------------------
CServiceMarket::~CServiceMarket()
{
    //dtor
}

//-------------------------------------------------------------------------------
void CServiceMarket::SetResources(CResources* pxResources)
{
    m_pxResources = pxResources;
}

//-------------------------------------------------------------------------------
CResources* CServiceMarket::GetResources(void)
{
    return m_pxResources;
}

//-------------------------------------------------------------------------------

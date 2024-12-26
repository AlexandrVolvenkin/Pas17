
//-------------------------------------------------------------------------------
//  Source      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-------------------------------------------------------------------------------

#include <iostream>
#include <string.h>
#include <typeinfo>


#include "Task.h"
#include "Resources.h"
#include "CommunicationDevice.h"
#include "ModbusMasterLinkLayer.h"


//-------------------------------------------------------------------------------
CModbusMasterLinkLayer::CModbusMasterLinkLayer()
{
    //ctor
}

//-------------------------------------------------------------------------------
CModbusMasterLinkLayer::~CModbusMasterLinkLayer()
{
    //dtor
}

//-------------------------------------------------------------------------------
void CModbusMasterLinkLayer::SetCommunicationDeviceName(std::string sName)
{
    m_sCommunicationDeviceName = sName;
}

//-------------------------------------------------------------------------------
void CModbusMasterLinkLayer::SetCommunicationDevice(CCommunicationDeviceInterface* pxCommunicationDevice)
{
    m_pxCommunicationDevice = pxCommunicationDevice;
}


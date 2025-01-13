
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
#include "SerialMtMasterLinkLayer.h"


//-------------------------------------------------------------------------------
CSerialMtMasterLinkLayer::CSerialMtMasterLinkLayer()
{
    //ctor
}

//-------------------------------------------------------------------------------
CSerialMtMasterLinkLayer::~CSerialMtMasterLinkLayer()
{
    //dtor
}

//-------------------------------------------------------------------------------
void CSerialMtMasterLinkLayer::SetCommunicationDeviceName(std::string sName)
{
    m_sCommunicationDeviceName = sName;
}

//-------------------------------------------------------------------------------
void CSerialMtMasterLinkLayer::SetCommunicationDevice(CCommunicationDeviceInterface* pxCommunicationDevice)
{
    m_pxCommunicationDevice = pxCommunicationDevice;
}


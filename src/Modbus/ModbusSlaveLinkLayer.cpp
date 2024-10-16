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
#include "ModbusSlaveLinkLayer.h"


//-------------------------------------------------------------------------------
CModbusSlaveLinkLayer::CModbusSlaveLinkLayer()
{
    //ctor
}

//-------------------------------------------------------------------------------
CModbusSlaveLinkLayer::CModbusSlaveLinkLayer(CResources* pxResources)
{

}

//-------------------------------------------------------------------------------
CModbusSlaveLinkLayer::~CModbusSlaveLinkLayer()
{
    //dtor
}

//-------------------------------------------------------------------------------
void CModbusSlaveLinkLayer::SetCommunicationDeviceName(std::string sName)
{
    m_sCommunicationDeviceName = sName;
}

//-------------------------------------------------------------------------------
void CModbusSlaveLinkLayer::SetCommunicationDevice(CCommunicationDeviceInterfaceNew* pxCommunicationDevice)
{
    m_pxCommunicationDevice = pxCommunicationDevice;
}

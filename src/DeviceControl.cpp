//-------------------------------------------------------------------------------
//  Sourse      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-------------------------------------------------------------------------------
#include "DeviceControl.h"
#include "Platform.h"
#include "Resources.h"
#include "DataStore.h"

using namespace std;

//-------------------------------------------------------------------------------
CDeviceControl::CDeviceControl()
{
    //ctor
}

//-------------------------------------------------------------------------------
CDeviceControl::~CDeviceControl()
{
    //dtor
}

//-------------------------------------------------------------------------------
void CDeviceControl::SetResources(CResources* pxResources)
{
    m_pxResources = pxResources;
}

//-------------------------------------------------------------------------------
CResources* CDeviceControl::GetResources(void)
{
    return m_pxResources;
}

//-------------------------------------------------------------------------------
uint16_t CDeviceControl::ConfigurationRead(uint8_t *puiDestination)
{
    std::cout << "CDeviceControl::ConfigurationRead 1" << std::endl;
    uint8_t auiTempData[] = {1, 15, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 22, 4, 0,};
    memcpy(puiDestination, auiTempData, sizeof(auiTempData));
    return sizeof(auiTempData);
}

//-------------------------------------------------------------------------------
uint16_t CDeviceControl::DataBaseBlockRead(uint8_t *puiDestination, uint8_t uiBlockIndex)
{
    std::cout << "CDeviceControl::DataBaseRead 1" << std::endl;

    uint16_t uiLength;

    uiLength = m_pxResources ->
               m_pxDataStore ->
               GetBlockLength(uiBlockIndex);
    std::cout << "CDeviceControl::DataBaseRead uiLength " << (int)uiLength << std::endl;
    return uiLength;
}

//-------------------------------------------------------------------------------
uint16_t CDeviceControl::DataBaseBlockWrite(uint8_t *puiSource, uint8_t uiBlockIndex)
{
    std::cout << "CDeviceControl::DataBaseWrite 1" << std::endl;
    return 0;
}

//-------------------------------------------------------------------------------

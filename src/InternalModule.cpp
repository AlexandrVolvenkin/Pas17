//-------------------------------------------------------------------------------
//  Sourse      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-------------------------------------------------------------------------------
#include "Task.h"
#include "InternalModule.h"
#include "Platform.h"
#include "Resources.h"
#include "DataStore.h"
#include "Crc.h"

using namespace std;

//-------------------------------------------------------------------------------
CInternalModuleInterface::CInternalModuleInterface()
{
    //ctor
}

//-------------------------------------------------------------------------------
CInternalModuleInterface::~CInternalModuleInterface()
{
    //dtor
}

//-------------------------------------------------------------------------------








//-------------------------------------------------------------------------------
CInternalModule::CInternalModule()
{
    std::cout << "CInternalModule constructor"  << std::endl;
}

//-------------------------------------------------------------------------------
CInternalModule::~CInternalModule()
{
    //dtor
}

////-------------------------------------------------------------------------------
//void CInternalModule::SetResources(CResources* pxResources)
//{
//    m_pxResources = pxResources;
//}
//
////-------------------------------------------------------------------------------
//CResources* CInternalModule::GetResources(void)
//{
//    return m_pxResources;
//}

//-------------------------------------------------------------------------------
uint8_t CInternalModule::GetType(void)
{
    return MISSING_COMPONENT;
}

//-------------------------------------------------------------------------------
uint8_t CInternalModule::GetWorkingStatus(void)
{
    return MISSING_COMPONENT;
}

//-------------------------------------------------------------------------------
void CInternalModule::SetAddress(uint8_t uiAddress)
{
    m_uiAddress = uiAddress;
}

//-------------------------------------------------------------------------------
uint8_t CInternalModule::GetAddress(void)
{
    return m_uiAddress;
}

//-------------------------------------------------------------------------------
bool CInternalModule::IsAbleToReplace(uint8_t uiType)
{
    return false;
}

//-------------------------------------------------------------------------------
bool CInternalModule::IsReadyToStartWork(void)
{
    return false;
}

//-------------------------------------------------------------------------------
void CInternalModule::SetCommunicationDevice(CCommunicationDeviceInterface* pxCommunicationDevice)
{
    m_pxCommunicationDevice = pxCommunicationDevice;
}

//-------------------------------------------------------------------------------
uint8_t CInternalModule::GetModuleType(uint8_t uiAddress)
{
    std::cout << "CInternalModule::GetModuleType 1"  << std::endl;
    uint8_t auiSpiTxBuffer[TX_RX_BUFF_SIZE];
    uint8_t auiSpiRxBuffer[TX_RX_BUFF_SIZE];

    usleep(10000);
    auiSpiTxBuffer[0] = GET_MODULE_TYPE_COMMAND;
    m_pxCommunicationDevice -> Exchange(uiAddress,
                                        auiSpiTxBuffer,
                                        auiSpiRxBuffer,
                                        SPI_PREAMBLE_LENGTH +
                                        GET_MODULE_TYPE_COMMAND_ANSWER_LENGTH +
                                        ONE_BYTE_CRC_LENGTH,
                                        LOW_SPEED_IN_HZ);
    std::cout << "CInternalModule::GetModuleType 2"  << std::endl;

            {
                cout << "GET_MODULE_TYPE_COMMAND auiSpiRxBuffer" << endl;
                unsigned char *pucSourceTemp;
                pucSourceTemp = (unsigned char*)auiSpiRxBuffer;
                for(int i=0; i<16; )
                {
                    for(int j=0; j<8; j++)
                    {
                        cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
                    }
                    cout << endl;
                    i += 8;
                }
            }

    // модуль отвечает?
    if (auiSpiRxBuffer[GET_MODULE_TYPE_COMMAND_OFFSET] ==
            GET_MODULE_TYPE_COMMAND) // if echo answer is ok.
    {
    std::cout << "CInternalModule::GetModuleType 3"  << std::endl;
//                std::cout << "CInternalModule::GetModuleType ucSlaveAddress 1 "  << (int)ucSlaveAddress << std::endl;
        // обмен данными прошёл без ошибок?
        if ((iCrcSummOneByteCompare(&auiSpiRxBuffer[GET_MODULE_TYPE_COMMAND_OFFSET],
                                    GET_MODULE_TYPE_COMMAND_LENGTH +
                                    GET_MODULE_TYPE_COMMAND_ANSWER_LENGTH) > 0))
        {
    std::cout << "CInternalModule::GetModuleType 4"  << std::endl;
//            // модуль присутствует. увеличим значение переменной -
//            // фактическое количество модулей в системе.
//            (xPlcConfigSearchTemp.ui8ModulesQuantity)++;
//            // сохраним тип модуля в массиве для упорядочивания следования модулей при поиске на SPI.
//            xPlcConfigSearchTemp.axConfigSearch[ucSlaveAddress].ui8Type =
//                auiSpiRxBuffer[MODULE_TYPE_OFFSET];
//            // сохраним адрес модуля в массиве для упорядочивания следования модулей при поиске на SPI.
//            xPlcConfigSearchTemp.axConfigSearch[ucSlaveAddress].ui8Address =
//                ucSlaveAddress;
//
//            AddSystemComponent(auiSpiRxBuffer[MODULE_TYPE_OFFSET],
//                               ucSlaveAddress);
//            std::cout << "CInternalModule::GetModuleType ucSlaveAddress "  << (int)xPlcConfigSearchTemp.axConfigSearch[ucSlaveAddress].ui8Address << std::endl;
//            std::cout << "CInternalModule::GetModuleType ui8Type "  << (int)xPlcConfigSearchTemp.axConfigSearch[ucSlaveAddress].ui8Type << std::endl;
        }
        else
        {
    std::cout << "CInternalModule::GetModuleType 5"  << std::endl;
//            // сохраним адрес модуля в массиве для упорядочивания следования модулей при поиске на SPI.
//            xPlcConfigSearchTemp.axConfigSearch[ucSlaveAddress].ui8Address =
//                ucSlaveAddress;
//
//            AddSystemComponent(0,
//                               ucSlaveAddress);
        }
    }
    std::cout << "CInternalModule::GetModuleType 6"  << std::endl;
}

//-------------------------------------------------------------------------------

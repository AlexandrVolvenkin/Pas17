
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
#include <memory>

#include "Task.h"
#include "Resources.h"
#include "StorageDevice.h"
#include "Crc.h"
#include "HammingCodes.h"
#include "DataContainer.h"
#include "DataStore.h"


// (sizeof(struct TAnalogueInputDescriptionDataBase) * ANALOG_MODULE_INPUT_QUANTITY)// 28С…6=168.
#define ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_LENGTH 168
// (sizeof(struct TMrXXOneChannelDataBase) * DISCRETE_OUTPUT_MODULE_RELAY_OUTPUT_QUANTITY)// 3С…64=192.
#define DISCRETE_OUTPUT_MODULE_DATA_BASE_BLOCK_LENGTH 192
// (sizeof(struct TMbmFunction1234PackOne) * EXTERNAL_MODULE_FUNCTION_1234_QUANTITY) + 1// 13С…16=208.
#define MBM_FUNCTION_1234_BLOCK_DATA_BASE_BLOCK_LENGTH (208 + 1)
// (sizeof(struct TMbmFunction15Sourse) * EXTERNAL_MODULE_FUNCTION_15_QUANTITY)// 72С…3=216.
#define MBM_FUNCTION_15_BLOCK_DATA_BASE_BLOCK_LENGTH 216
// (sizeof(struct TDiscreteInputDescriptionWork) * DISCRETE_INPUT_SYGNALS_DATA_BASE_BLOCKS_IN_BLOCK_QUANTITY)// 11С…18=198.
#define DISCRETE_INPUT_SYGNALS_DATA_BASE_BLOCK_LENGTH 192
// (sizeof(struct TAnalogueOutputRegulatorDescriptionDataBase) * ANALOG_OUTPUT_MODULE_REGULATORS_QUANTITY)// 28С…6=168.
#define CURRENT_OUTPUT_MODULE_REGULATOR_DATA_BASE_BLOCK_LENGTH 56
#define CURRENT_OUTPUT_MODULE_PSP_DATA_BASE_BLOCK_LENGTH 56
// DIMENSIONS_PARAMETERS_DATA_BLOCKS_IN_BLOCK_QUANTITY * 6// 6С…16=96.
#define DIMENSIONS_PARAMETERS_DATA_BASE_BLOCK_LENGTH 96
// TEXT_TITLES_DATA_BASE_BLOCKS_IN_BLOCK_QUANTITY * TEXT_TITLES_DATA_LENGTH// (384 РўР , 24 Р±Р»РѕРєР°).
#define TEXT_TITLES_DATA_BASE_BLOCK_LENGTH 224
// (sizeof(struct TFunctionBlockDescriptionWork) * FUNCTION_BLOCK_DATA_BASE_BLOCKS_IN_BLOCK_QUANTITY)// 19С…10=190.
#define FUNCTION_BLOCK_DATA_BASE_BLOCK_LENGTH 160// 19С…10=190 (120 Р¤Р‘Р›, 12 Р±Р»РѕРєРѕРІ)
#define MATHEMATICAL_BLOCK_DATA_BASE_BLOCK_LENGTH 208// 82 вЂ“ 84 (48 Р¤Р‘Рњ, 3 Р±Р»РѕРєР° 13*16=208 )
#define DISCRETE_INPUT_MODULE_DATA_BASE_BLOCK_LENGTH 32// 2С…16=32.
#define NETWORK_ADDRESS_DATA_BASE_BLOCK_LENGTH 1
#define RESERVED_DATA_BASE_BLOCK_LENGTH 1
#define REFERENCE_POINTS_ADC_CODES_DATA_BASE_BLOCK_LENGTH 42

// РјР°СЃСЃРёРІ СЃРѕРґРµСЂР¶РёС‚ СЂР°Р·РјРµСЂС‹ Р±Р»РѕРєРѕРІ Р±Р°Р·С‹ РґР°РЅРЅС‹С….
// РјР°СЃСЃРёРІ Р±Р°Р·С‹ РґР°РЅРЅС‹С… СЃРѕСЃС‚РѕРёС‚ РёР· 100 Р±Р»РѕРєРѕРІ РїРѕ 256 Р±Р°Р№С‚.
// РєРѕРіРґР° РїСЂРѕРіСЂР°РјРјР°С‚РѕСЂ Р·Р°РїСЂР°С€РёРІР°РµС‚ РЅСѓР¶РЅС‹Р№ РµРјСѓ Р±Р»РѕРє, (РІ С„СѓРЅРєС†РёРё - modbus_reply() - case _FC_DATA_BASE_READ:),
// РёР· СЌС‚РѕРіРѕ РјР°СЃСЃРёРІР°, РїРѕ РЅРѕРјРµСЂСѓ, РїСЂРѕРіСЂР°РјРјР°С‚РѕСЂСѓ РІС‹РґР°С‘С‚СЃСЏ СЂР°Р·РјРµСЂ Р±Р»РѕРєР°.
const unsigned char aucDataBaseBlockLength[CDataStore::MAX_BLOCKS_NUMBER] =
{
    RESERVED_DATA_BASE_BLOCK_LENGTH,
    ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_LENGTH,
    RESERVED_DATA_BASE_BLOCK_LENGTH,
    RESERVED_DATA_BASE_BLOCK_LENGTH,
    RESERVED_DATA_BASE_BLOCK_LENGTH,
    RESERVED_DATA_BASE_BLOCK_LENGTH,
    RESERVED_DATA_BASE_BLOCK_LENGTH,
    RESERVED_DATA_BASE_BLOCK_LENGTH,
    RESERVED_DATA_BASE_BLOCK_LENGTH,
    RESERVED_DATA_BASE_BLOCK_LENGTH,



    RESERVED_DATA_BASE_BLOCK_LENGTH,
    RESERVED_DATA_BASE_BLOCK_LENGTH,
    DISCRETE_INPUT_SYGNALS_DATA_BASE_BLOCK_LENGTH,
    RESERVED_DATA_BASE_BLOCK_LENGTH,
    RESERVED_DATA_BASE_BLOCK_LENGTH,
    RESERVED_DATA_BASE_BLOCK_LENGTH,
    RESERVED_DATA_BASE_BLOCK_LENGTH,
    RESERVED_DATA_BASE_BLOCK_LENGTH,
    RESERVED_DATA_BASE_BLOCK_LENGTH,
    RESERVED_DATA_BASE_BLOCK_LENGTH,



    FUNCTION_BLOCK_DATA_BASE_BLOCK_LENGTH,
    FUNCTION_BLOCK_DATA_BASE_BLOCK_LENGTH,
    FUNCTION_BLOCK_DATA_BASE_BLOCK_LENGTH,
    FUNCTION_BLOCK_DATA_BASE_BLOCK_LENGTH,
    FUNCTION_BLOCK_DATA_BASE_BLOCK_LENGTH,
    FUNCTION_BLOCK_DATA_BASE_BLOCK_LENGTH,
    FUNCTION_BLOCK_DATA_BASE_BLOCK_LENGTH,
    FUNCTION_BLOCK_DATA_BASE_BLOCK_LENGTH,
    FUNCTION_BLOCK_DATA_BASE_BLOCK_LENGTH,
    FUNCTION_BLOCK_DATA_BASE_BLOCK_LENGTH,



    FUNCTION_BLOCK_DATA_BASE_BLOCK_LENGTH,
    FUNCTION_BLOCK_DATA_BASE_BLOCK_LENGTH,
    CURRENT_OUTPUT_MODULE_REGULATOR_DATA_BASE_BLOCK_LENGTH,
    CURRENT_OUTPUT_MODULE_PSP_DATA_BASE_BLOCK_LENGTH,
    RESERVED_DATA_BASE_BLOCK_LENGTH,
    DIMENSIONS_PARAMETERS_DATA_BASE_BLOCK_LENGTH,
    RESERVED_DATA_BASE_BLOCK_LENGTH,
    RESERVED_DATA_BASE_BLOCK_LENGTH,
    RESERVED_DATA_BASE_BLOCK_LENGTH,
    RESERVED_DATA_BASE_BLOCK_LENGTH,



    TEXT_TITLES_DATA_BASE_BLOCK_LENGTH,
    TEXT_TITLES_DATA_BASE_BLOCK_LENGTH,
    RESERVED_DATA_BASE_BLOCK_LENGTH,
    RESERVED_DATA_BASE_BLOCK_LENGTH,
    RESERVED_DATA_BASE_BLOCK_LENGTH,
    RESERVED_DATA_BASE_BLOCK_LENGTH,
    RESERVED_DATA_BASE_BLOCK_LENGTH,
    RESERVED_DATA_BASE_BLOCK_LENGTH,
    RESERVED_DATA_BASE_BLOCK_LENGTH,
    RESERVED_DATA_BASE_BLOCK_LENGTH,



    RESERVED_DATA_BASE_BLOCK_LENGTH,
    RESERVED_DATA_BASE_BLOCK_LENGTH,
    RESERVED_DATA_BASE_BLOCK_LENGTH,
    RESERVED_DATA_BASE_BLOCK_LENGTH,
    RESERVED_DATA_BASE_BLOCK_LENGTH,
    RESERVED_DATA_BASE_BLOCK_LENGTH,
    RESERVED_DATA_BASE_BLOCK_LENGTH,
    RESERVED_DATA_BASE_BLOCK_LENGTH,
    RESERVED_DATA_BASE_BLOCK_LENGTH,
    RESERVED_DATA_BASE_BLOCK_LENGTH,



    REFERENCE_POINTS_ADC_CODES_DATA_BASE_BLOCK_LENGTH,
    RESERVED_DATA_BASE_BLOCK_LENGTH,
    RESERVED_DATA_BASE_BLOCK_LENGTH,
    RESERVED_DATA_BASE_BLOCK_LENGTH,
    RESERVED_DATA_BASE_BLOCK_LENGTH,
    RESERVED_DATA_BASE_BLOCK_LENGTH,
    RESERVED_DATA_BASE_BLOCK_LENGTH,
    RESERVED_DATA_BASE_BLOCK_LENGTH,
    RESERVED_DATA_BASE_BLOCK_LENGTH,
    RESERVED_DATA_BASE_BLOCK_LENGTH,


    RESERVED_DATA_BASE_BLOCK_LENGTH,
    RESERVED_DATA_BASE_BLOCK_LENGTH,
    RESERVED_DATA_BASE_BLOCK_LENGTH,
    RESERVED_DATA_BASE_BLOCK_LENGTH,
    RESERVED_DATA_BASE_BLOCK_LENGTH,
    RESERVED_DATA_BASE_BLOCK_LENGTH,
    RESERVED_DATA_BASE_BLOCK_LENGTH,
    RESERVED_DATA_BASE_BLOCK_LENGTH,
    RESERVED_DATA_BASE_BLOCK_LENGTH,
    RESERVED_DATA_BASE_BLOCK_LENGTH,



    RESERVED_DATA_BASE_BLOCK_LENGTH,
    RESERVED_DATA_BASE_BLOCK_LENGTH,
    RESERVED_DATA_BASE_BLOCK_LENGTH,
    RESERVED_DATA_BASE_BLOCK_LENGTH,
    RESERVED_DATA_BASE_BLOCK_LENGTH,
    RESERVED_DATA_BASE_BLOCK_LENGTH,
    RESERVED_DATA_BASE_BLOCK_LENGTH,
    RESERVED_DATA_BASE_BLOCK_LENGTH,
    RESERVED_DATA_BASE_BLOCK_LENGTH,
    RESERVED_DATA_BASE_BLOCK_LENGTH,



    RESERVED_DATA_BASE_BLOCK_LENGTH,
    RESERVED_DATA_BASE_BLOCK_LENGTH,
    RESERVED_DATA_BASE_BLOCK_LENGTH,
    RESERVED_DATA_BASE_BLOCK_LENGTH,
    RESERVED_DATA_BASE_BLOCK_LENGTH,
    RESERVED_DATA_BASE_BLOCK_LENGTH,
    RESERVED_DATA_BASE_BLOCK_LENGTH,
    RESERVED_DATA_BASE_BLOCK_LENGTH,
    RESERVED_DATA_BASE_BLOCK_LENGTH,
    NETWORK_ADDRESS_DATA_BASE_BLOCK_LENGTH
};

//-------------------------------------------------------------------------------
CDataStore::CDataStore()
{
    std::cout << "CDataStore constructor"  << std::endl;
    // РїРѕР»СѓС‡РёРј РёРјСЏ РєР»Р°СЃСЃР°.
    sprintf(GetTaskNamePointer(),
            "%s",
            typeid(*this).name());
    m_pxStorageDevice = 0;
    m_puiIntermediateBuff = new uint8_t[CDataStore::MAX_SERVICE_SECTION_DATA_LENGTH];
//    m_pxArgumentData = std::make_shared<TArgumentData>();// = new TArgumentData;
    m_pxDataContainer = std::make_shared<CDataContainerDataBase>();
    SetFsmState(START);
}

//-------------------------------------------------------------------------------
CDataStore::CDataStore(CStorageDeviceInterface* pxStorageDevice) :
    m_pxStorageDevice(pxStorageDevice)
{
    m_puiIntermediateBuff = new uint8_t[CDataStore::MAX_SERVICE_SECTION_DATA_LENGTH];
    SetFsmState(IDDLE);
//    SetSavedFsmState(IDDLE);
}

//-------------------------------------------------------------------------------
CDataStore::~CDataStore()
{
    delete m_puiIntermediateBuff;
    delete m_pxStorageDevice;

}

//-------------------------------------------------------------------------------
void CDataStore::SetStorageDeviceName(std::string sName)
{
    m_sStorageDeviceName = sName;
}

//-------------------------------------------------------------------------------
void CDataStore::SetStorageDevice(CStorageDeviceInterface* pxStorageDevice)
{
    m_pxStorageDevice = pxStorageDevice;
}

//-------------------------------------------------------------------------------
void CDataStore::SetArgumentData(uint8_t *puiDataPointer,
                                 uint16_t uiDataOffset,
                                 uint16_t uiDataLength)
{
    GetArgumentDataPointer() -> m_puiDataPointer = puiDataPointer;
    GetArgumentDataPointer() -> m_uiDataOffset = uiDataOffset;
    GetArgumentDataPointer() -> m_uiDataLength = uiDataLength;
}

//-------------------------------------------------------------------------------
void CDataStore::GetArgumentData(void)
{
//    m_puiBuffer = m_pxTaskCustomer -> GetArgumentDataPointer() -> m_puiDataPointer;
//    m_uiOffset = m_pxTaskCustomer -> GetArgumentDataPointer() -> m_uiDataOffset;
//    m_uiLength = m_pxTaskCustomer -> GetArgumentDataPointer() -> m_uiDataLength;
}

////-------------------------------------------------------------------------------
//void CDataStore::SetResources(CResources* pxResources)
//{
//    m_pxResources = pxResources;
//}
//
////-------------------------------------------------------------------------------
//CResources* CDataStore::GetResources(void)
//{
//    return m_pxResources;
//}

//-------------------------------------------------------------------------------
void CDataStore::CreateServiceSection(void)
{
    std::cout << "CDataStore::CreateServiceSection 1"  << std::endl;
    SetFsmState(READY);

    // РћС‡РёСЃС‚РёРј СЃР»СѓР¶РµР±РЅС‹Р№ РєРѕРЅС‚РµРєСЃС‚.
    memset(reinterpret_cast<uint8_t*>(&m_xServiseSection.xServiseSectionData),
           0,
           sizeof(struct TServiseSectionData));

    // РЈСЃС‚Р°РЅРѕРІРёРј СЂР°Р·РјРµСЂ РїРµСЂРІРёС‡РЅС‹С… РґР°РЅРЅС‹С….
    m_xServiseSection.xServiseSectionData.uiLength = sizeof(struct TServiseSectionData);
    // РљРѕР»РёС‡РµСЃС‚РІРѕ Р±Р°Р№С‚ СЃР»СѓР¶РµР±РЅРѕРіРѕ РєРѕРЅС‚РµРєСЃС‚Р°
    // РґРѕР»Р¶РЅРѕ Р±С‹С‚СЊ РёР·РІРµСЃС‚РЅРѕ Р·Р°СЂР°РЅРµРµ.
    // РўР°Рє РєР°Рє РїРѕ РЅРµРјСѓ РёР·РІР»РµРєР°СЋС‚СЃСЏ РЅР°С‡Р°Р»СЊРЅС‹Рµ РґР°РЅРЅС‹Рµ.
    // Р�СЃРїРѕР»СЊР·СѓРµС‚СЃСЏ СЃР°РјРѕРІРѕСЃСЃС‚Р°РЅР°РІР»РёРІР°СЋС‰РёР№СЃСЏ РєРѕРґ РҐРµРјРјРёРЅРіР°(8,4).
    // РљРѕСЌС„С„РёС†РёРµРЅС‚ - 1.5: РѕРґРёРЅ Р±Р°Р№С‚ РїСЂРµРѕР±СЂР°Р·СѓРµС‚СЃСЏ РІ РєРѕРґРѕРІРѕРµ СЃР»РѕРІРѕ 12 Р±РёС‚,
    // РёР· РґРІСѓС… Р±Р°Р№С‚ РїРѕР»РµР·РЅС‹С… РґР°РЅРЅС‹С… РїРѕР»СѓС‡Р°РµС‚СЃСЏ С‚СЂРё Р±Р°Р№С‚Р° РєРѕРґРёСЂРѕРІР°РЅРЅС‹С….
    // РЈСЃС‚Р°РЅРѕРІРёРј СЂР°Р·РјРµСЂ Р·Р°РєРѕРґРёСЂРѕРІР°РЅРЅС‹С… РґР°РЅРЅС‹С….
    m_xServiseSection.xServiseSectionData.uiEncodedLength =
        (CHammingCodes::CalculateEncodedDataLength(sizeof(struct TServiseSection)));
    // РЈСЃС‚Р°РЅРѕРІРёРј СЃРјРµС‰РµРЅРёРµ РЅР° РЅР°С‡Р°Р»Рѕ РґР°РЅРЅС‹С… Р±Р»РѕРєРѕРІ.
    m_xServiseSection.xServiseSectionData.uiFreeSpaceOffset =
        (SERVICE_SECTION_DATA_BEGIN +
         m_xServiseSection.xServiseSectionData.uiEncodedLength);
    // РҐСЂР°РЅРёР»РёС‰Рµ РїСѓСЃС‚Рѕ.
    m_xServiseSection.xServiseSectionData.uiStoredBlocksNumber = 0;
    // РЈСЃС‚Р°РЅРѕРІРёРј РїСЂРёР·РЅР°Рє - С…СЂР°РЅРёР»РёС‰Рµ РЅРµ РїРѕРґРїРёСЃР°РЅРѕ.
    m_xServiseSection.xServiseSectionData.uiCrcOfBlocksCrc = 0;
}

//-------------------------------------------------------------------------------
// Р“РѕС‚РѕРІРёС‚ РґР°РЅРЅС‹Рµ РІСЂРµРјРµРЅРЅРѕРіРѕ СЃР»СѓР¶РµР±РЅРѕРіРѕ Р±Р»РѕРєР° Рє Р·Р°РїРёСЃРё.
// РљРѕРґРёСЂСѓРµС‚ РїРѕ Р°Р»РіРѕСЂРёС‚РјСѓ РҐРµРјРјРёРЅРіР° Рё
// РїРµСЂРµРґР°С‘С‚ Р°РІС‚РѕРјР°С‚Сѓ Р·Р°РїРёСЃРё СѓСЃС‚СЂРѕР№СЃС‚РІР° С…СЂР°РЅРµРЅРёСЏ.
uint8_t CDataStore::TemporaryServiceSectionWritePrepare(void)
{
    std::cout << "CDataStore::TemporaryServiceSectionWritePrepare 1"  << std::endl;
    // Р’С‹С‡РёСЃР»РёРј РєРѕРЅС‚СЂРѕР»СЊРЅСѓСЋ СЃСѓРјРјСѓ.
    m_xServiseSection.uiCrc =
        usCrc16(reinterpret_cast<uint8_t*>(&m_xServiseSection.xServiseSectionData),
                sizeof(struct TServiseSectionData));

    std::cout << "CDataStore::TemporaryServiceSectionWritePrepare 2"  << std::endl;
    // Р—РІРєРѕРґРёСЂСѓРµРј РґР°РЅРЅС‹Рµ Р°Р»РіРѕСЂРёС‚РјРѕРј РҐРµРјРјРёРЅРіР°.
    uint16_t uiEncodedByteCounter =
        CHammingCodes::BytesToHammingCodes(m_puiIntermediateBuff,
                                           reinterpret_cast<uint8_t*>(&m_xServiseSection),
                                           sizeof(struct TServiseSection));

    std::cout << "CDataStore::TemporaryServiceSectionWritePrepare 3"  << std::endl;

    SetArgumentData(m_puiIntermediateBuff,
                    TEMPORARY_SERVICE_SECTION_DATA_BEGIN,
                    uiEncodedByteCounter);

    return 1;
//    if (m_pxStorageDevice -> WriteBlock(m_puiIntermediateBuff,
//                                        TEMPORARY_SERVICE_SECTION_DATA_BEGIN,
//                                        uiEncodedByteCounter))
//    {
//        std::cout << "CDataStore::TemporaryServiceSectionWritePrepare 4"  << std::endl;
//        return 1;
//    }
//    else
//    {
//        std::cout << "CDataStore::TemporaryServiceSectionWritePrepare 5"  << std::endl;
//        return 0;
//    }
}

//-------------------------------------------------------------------------------
// Р“РѕС‚РѕРІРёС‚ РґР°РЅРЅС‹Рµ СЃР»СѓР¶РµР±РЅРѕРіРѕ Р±Р»РѕРєР° Рє Р·Р°РїРёСЃРё.
// РљРѕРґРёСЂСѓРµС‚ РїРѕ Р°Р»РіРѕСЂРёС‚РјСѓ РҐРµРјРјРёРЅРіР° Рё
// РїРµСЂРµРґР°С‘С‚ Р°РІС‚РѕРјР°С‚Сѓ Р·Р°РїРёСЃРё СѓСЃС‚СЂРѕР№СЃС‚РІР° С…СЂР°РЅРµРЅРёСЏ.
uint8_t CDataStore::ServiceSectionWritePrepare(void)
{
    // Р’С‹С‡РёСЃР»РёРј РєРѕРЅС‚СЂРѕР»СЊРЅСѓСЋ СЃСѓРјРјСѓ.
    m_xServiseSection.uiCrc =
        usCrc16(reinterpret_cast<uint8_t*>(&m_xServiseSection.xServiseSectionData),
                sizeof(struct TServiseSectionData));

    // Р—РІРєРѕРґРёСЂСѓРµРј РґР°РЅРЅС‹Рµ Р°Р»РіРѕСЂРёС‚РјРѕРј РҐРµРјРјРёРЅРіР°.
    uint16_t uiEncodedByteCounter =
        CHammingCodes::BytesToHammingCodes(m_puiIntermediateBuff,
                                           reinterpret_cast<uint8_t*>(&m_xServiseSection),
                                           sizeof(struct TServiseSection));

    SetArgumentData(m_puiIntermediateBuff,
                    SERVICE_SECTION_DATA_BEGIN,
                    uiEncodedByteCounter);

    return 1;

    //    if (m_pxStorageDevice -> WriteBlock(m_puiIntermediateBuff,
//                                        SERVICE_SECTION_DATA_BEGIN,
//                                        uiEncodedByteCounter))
//    {
//        return 1;
//    }
//    else
//    {
//        return 0;
//    }
}

//-------------------------------------------------------------------------------
// Р“РѕС‚РѕРІРёС‚ РґР°РЅРЅС‹Рµ Р±Р»РѕРєР° Рє Р·Р°РїРёСЃРё РІРѕ РІСЂРµРјРµРЅРЅС‹Р№ Р±СѓС„РµСЂ.
// РљРѕРґРёСЂСѓРµС‚ РїРѕ Р°Р»РіРѕСЂРёС‚РјСѓ РҐРµРјРјРёРЅРіР°, СЂР°Р·РјРµС‡Р°РµС‚ РјРµСЃС‚Рѕ РґР»СЏ С…СЂР°РЅРµРЅРёСЏ Рё
// РїРµСЂРµРґР°С‘С‚ Р°РІС‚РѕРјР°С‚Сѓ Р·Р°РїРёСЃРё СѓСЃС‚СЂРѕР№СЃС‚РІР° С…СЂР°РЅРµРЅРёСЏ.
uint8_t CDataStore::TemporaryBlockWritePrepare(void)
{
    std::cout << "CDataStore::TemporaryBlockWritePrepare 1"  << std::endl;
    uint8_t uiBlock = m_uiBlockIndex;
    uint8_t* puiSource = m_puiBlockDataPointer;
    uint16_t uiLength = m_uiBlockLength;

    // Р—РІРєРѕРґРёСЂСѓРµРј РґР°РЅРЅС‹Рµ Р°Р»РіРѕСЂРёС‚РјРѕРј РҐРµРјРјРёРЅРіР°.
    uint16_t uiEncodedByteCounter =
        CHammingCodes::BytesToHammingCodes(m_puiIntermediateBuff, puiSource, uiLength);

    // Р”Р°РЅРЅС‹Рµ С‚РµРєСѓС‰РµРіРѕ Р±Р»РѕРєР° РґР°РЅРЅС‹С… - uiBlock СЃРѕС…СЂР°РЅСЏСЋС‚СЃСЏ РІРїРµСЂРІС‹Рµ?
    if ((m_xServiseSection.xServiseSectionData.
            axBlockPositionData[uiBlock].uiLength == 0) &&
            (m_xServiseSection.xServiseSectionData.
             axBlockPositionData[uiBlock].uiEncodedLength == 0))
    {
        std::cout << "CDataStore::TemporaryBlockWritePrepare 2"  << std::endl;
        // Р”РѕР±Р°РІРёР»Рё РЅРѕРІС‹Р№ Р±Р»РѕРє РґР°РЅРЅС‹С….
        // РЎРѕС…СЂР°РЅРёРј СЃРјРµС‰РµРЅРёРµ РЅР° Р±Р»РѕРє.
        m_xServiseSection.xServiseSectionData.
        axBlockPositionData[uiBlock].uiOffset = m_xServiseSection.xServiseSectionData.uiFreeSpaceOffset;
        // Р�Р·РјРµРЅРёРј СЃРјРµС‰РµРЅРёРµ РЅР° СЃРІРѕР±РѕРґРЅРѕРµ РјРµСЃС‚Рѕ.
        m_xServiseSection.xServiseSectionData.uiFreeSpaceOffset += uiEncodedByteCounter;
        // РЎРѕС…СЂР°РЅРёРј СЂР°Р·РјРµСЂ РїРµСЂРІРёС‡РЅС‹С… РґР°РЅРЅС‹С….
        m_xServiseSection.xServiseSectionData.
        axBlockPositionData[uiBlock].uiLength = uiLength;
        // РЎРѕС…СЂР°РЅРёРј СЂР°Р·РјРµСЂ Р·Р°РєРѕРґРёСЂРѕРІР°РЅРЅС‹С… РґР°РЅРЅС‹С….
        m_xServiseSection.xServiseSectionData.
        axBlockPositionData[uiBlock].uiEncodedLength = uiEncodedByteCounter;
        // РЈРІРµР»РёС‡РёРј РєРѕР»РёС‡РµСЃС‚РІРѕ Р±Р»РѕРєРѕРІ РЅР°С…РѕРґСЏС‰РёС…СЃСЏ РІ С…СЂР°РЅРёР»РёС‰Рµ.
        m_xServiseSection.xServiseSectionData.uiStoredBlocksNumber += 1;
    }

    // Р’С‹С‡РёСЃР»РёРј РєРѕРЅС‚СЂРѕР»СЊРЅСѓСЋ СЃСѓРјРјСѓ РїРѕСЃС‚СѓРїРёРІС€РёС… РґР°РЅРЅС‹С….
    m_xServiseSection.xServiseSectionData.
    axBlockPositionData[uiBlock].uiCrc =
        usCrc16(puiSource, uiLength);

    m_pxStorageDevice ->
    GetDataContainerPointer() ->
    SetContainerData(CStorageDeviceInterface::WRITE_DATA_START,
                     uiBlock,
                     m_puiIntermediateBuff,
                     TEMPORARY_BLOCK_DATA_BEGIN,
                     uiEncodedByteCounter);
//    SetArgumentData(m_puiIntermediateBuff,
//                    TEMPORARY_BLOCK_DATA_BEGIN,
//                    uiEncodedByteCounter);

    return 1;
    //    if (m_pxStorageDevice -> WriteBlock(m_puiIntermediateBuff,
//                                        TEMPORARY_BLOCK_DATA_BEGIN,
//                                        uiEncodedByteCounter))
//    {
//        std::cout << "CDataStore::TemporaryBlockWritePrepare 3"  << std::endl;
//        return 1;
//    }
//    else
//    {
//        std::cout << "CDataStore::TemporaryBlockWritePrepare 4"  << std::endl;
//        return 0;
//    }
}

//-------------------------------------------------------------------------------
// Р“РѕС‚РѕРІРёС‚ РґР°РЅРЅС‹Рµ Р±Р»РѕРєР° Рє Р·Р°РїРёСЃРё.
// РљРѕРґРёСЂСѓРµС‚ РїРѕ Р°Р»РіРѕСЂРёС‚РјСѓ РҐРµРјРјРёРЅРіР°, СЂР°Р·РјРµС‡Р°РµС‚ РјРµСЃС‚Рѕ РґР»СЏ С…СЂР°РЅРµРЅРёСЏ Рё
// РїРµСЂРµРґР°С‘С‚ Р°РІС‚РѕРјР°С‚Сѓ Р·Р°РїРёСЃРё СѓСЃС‚СЂРѕР№СЃС‚РІР° С…СЂР°РЅРµРЅРёСЏ.
uint8_t CDataStore::BlockWritePrepare(void)
{
    uint8_t uiBlock = m_uiBlockIndex;
    uint8_t* puiSource = m_puiBlockDataPointer;
    uint16_t uiLength = m_uiBlockLength;

    // Р—РІРєРѕРґРёСЂСѓРµРј РґР°РЅРЅС‹Рµ Р°Р»РіРѕСЂРёС‚РјРѕРј РҐРµРјРјРёРЅРіР°.
    uint16_t uiEncodedByteCounter =
        CHammingCodes::BytesToHammingCodes(m_puiIntermediateBuff, puiSource, uiLength);

    // Р”Р°РЅРЅС‹Рµ С‚РµРєСѓС‰РµРіРѕ Р±Р»РѕРєР° РґР°РЅРЅС‹С… - uiBlock СЃРѕС…СЂР°РЅСЏСЋС‚СЃСЏ РІРїРµСЂРІС‹Рµ?
    if ((m_xServiseSection.xServiseSectionData.
            axBlockPositionData[uiBlock].uiLength == 0) &&
            (m_xServiseSection.xServiseSectionData.
             axBlockPositionData[uiBlock].uiEncodedLength == 0))
    {
        // Р”РѕР±Р°РІРёР»Рё РЅРѕРІС‹Р№ Р±Р»РѕРє РґР°РЅРЅС‹С….
        // РЎРѕС…СЂР°РЅРёРј СЃРјРµС‰РµРЅРёРµ РЅР° Р±Р»РѕРє.
        m_xServiseSection.xServiseSectionData.
        axBlockPositionData[uiBlock].uiOffset = m_xServiseSection.xServiseSectionData.uiFreeSpaceOffset;
        // Р�Р·РјРµРЅРёРј СЃРјРµС‰РµРЅРёРµ РЅР° СЃРІРѕР±РѕРґРЅРѕРµ РјРµСЃС‚Рѕ.
        m_xServiseSection.xServiseSectionData.uiFreeSpaceOffset += uiEncodedByteCounter;
        // РЎРѕС…СЂР°РЅРёРј СЂР°Р·РјРµСЂ РїРµСЂРІРёС‡РЅС‹С… РґР°РЅРЅС‹С….
        m_xServiseSection.xServiseSectionData.
        axBlockPositionData[uiBlock].uiLength = uiLength;
        // РЎРѕС…СЂР°РЅРёРј СЂР°Р·РјРµСЂ Р·Р°РєРѕРґРёСЂРѕРІР°РЅРЅС‹С… РґР°РЅРЅС‹С….
        m_xServiseSection.xServiseSectionData.
        axBlockPositionData[uiBlock].uiEncodedLength = uiEncodedByteCounter;
//        // РЈРІРµР»РёС‡РёРј РєРѕР»РёС‡РµСЃС‚РІРѕ Р±Р»РѕРєРѕРІ РЅР°С…РѕРґСЏС‰РёС…СЃСЏ РІ С…СЂР°РЅРёР»РёС‰Рµ.
//        m_xServiseSection.xServiseSectionData.uiStoredBlocksNumber += 1;
    }

    // Р’С‹С‡РёСЃР»РёРј РєРѕРЅС‚СЂРѕР»СЊРЅСѓСЋ СЃСѓРјРјСѓ РїРѕСЃС‚СѓРїРёРІС€РёС… РґР°РЅРЅС‹С….
    m_xServiseSection.xServiseSectionData.
    axBlockPositionData[uiBlock].uiCrc =
        usCrc16(puiSource, uiLength);
//    // РЎРѕС…СЂР°РЅРёРј РёРЅРґРµРєСЃ РїРѕСЃР»РµРґРЅРµРіРѕ Р·Р°РїРёСЃС‹РІР°РµРјРѕРіРѕ Р±Р»РѕРєР°.
//    m_xServiseSection.xServiseSectionData.uiLastWritedBlockNumber = uiBlock;

    SetArgumentData(m_puiIntermediateBuff,
                    m_xServiseSection.xServiseSectionData.
                    axBlockPositionData[uiBlock].uiOffset,
                    uiEncodedByteCounter);

    return 1;

//    if (m_pxStorageDevice -> WriteBlock(m_puiIntermediateBuff,
//                                        m_xServiseSection.xServiseSectionData.
//                                        axBlockPositionData[uiBlock].uiOffset,
//                                        uiEncodedByteCounter))
//    {
//        return 1;
//    }
//    else
//    {
//        return 0;
//    }
}

//-------------------------------------------------------------------------------
// Р—Р°РїРёСЃСЊ Р±Р»РѕРєР° РІ С…СЂР°РЅРёР»РёС‰Рµ РїСЂРѕС…РѕРґРёС‚ РїРѕСЌС‚Р°РїРЅРѕ.
// 1 - CDataStore::WriteBlock(uint8_t *puiSource, uint16_t uiLength, uint8_t uiBlock).
// 2 - CDataStore::BlockWritePrepare(void).
// 3 - CStorageDevice::WriteBlock(uint16_t uiOffset, uint8_t *puiSource, uint16_t uiLength).

// РџРµСЂРµРґР°С‘С‚ РґР°РЅРЅС‹Рµ РєРѕРЅС‚РµРєСЃС‚Р° Р·Р°РїРёСЃРё Р±Р»РѕРєР° Р°РІС‚РѕРјР°С‚Сѓ С…СЂР°РЅРёР»РёС‰Р° РґР°РЅРЅС‹С… Рё Р·Р°РїСѓСЃРєР°РµС‚ РїСЂРѕС†РµСЃСЃ Р·Р°РїРёСЃРё.
uint8_t CDataStore::WriteBlock(uint8_t *puiSource, uint16_t uiLength, uint8_t uiBlock)
{
    std::cout << "CDataStore::WriteBlock 1"  << std::endl;
    // РџСЂРѕРёР·РѕС€С‘Р» РІС‹С…РѕРґ Р·Р° РіСЂР°РЅРёС†С‹ Р±СѓС„РµСЂР°?
    if (uiBlock >= (MAX_BLOCKS_NUMBER + SERVICE_SECTION_BLOCK_NUMBER))
    {
        // РќРµС‚ РґР°РЅРЅС‹С….
        return 0;
    }

    // РџРѕР»СѓС‡РёРј РґР°РЅРЅС‹Рµ РєРѕРЅС‚РµРєСЃС‚Р° Р·Р°РїРёСЃРё Р±Р»РѕРєР°.
    m_puiBlockDataPointer = puiSource;
    m_uiBlockLength = uiLength;
    m_uiBlockIndex = uiBlock;
    return 1;

//    // РџСЂРѕРёР·РѕС€С‘Р» РІС‹С…РѕРґ Р·Р° РіСЂР°РЅРёС†С‹ Р±СѓС„РµСЂР°?
//    if (uiBlock >= (MAX_BLOCKS_NUMBER + SERVICE_SECTION_BLOCK_NUMBER))
//    {
//        // РќРµС‚ РґР°РЅРЅС‹С….
//        return 0;
//    }
//
//
//    // Р±Р»РѕРє РЅРµ СЃР»СѓР¶РµР±РЅС‹Р№?
//    if (uiBlock < MAX_BLOCKS_NUMBER)
//    {
//        // Р—РІРєРѕРґРёСЂСѓРµРј РґР°РЅРЅС‹Рµ Р°Р»РіРѕСЂРёС‚РјРѕРј РҐРµРјРјРёРЅРіР°.
//        m_uiBlockEncodedLength =
//            CHammingCodes::BytesToHammingCodes(m_puiIntermediateBuff,
//                                               puiSource,
//                                               uiLength);
//        // Р‘Р»РѕРє СЃСѓС‰РµСЃС‚РІСѓРµС‚?
//        if ((m_xServiseSection.xServiseSectionData.
//                axBlockPositionData[uiBlock].uiLength != 0) &&
//                (m_xServiseSection.xServiseSectionData.
//                 axBlockPositionData[uiBlock].uiEncodedLength != 0))
//        {
//            // Р”РѕР±Р°РІРёР»Рё РЅРѕРІС‹Р№ Р±Р»РѕРє РґР°РЅРЅС‹С….
//            // РЎРѕС…СЂР°РЅРёРј СЃРјРµС‰РµРЅРёРµ РЅР° Р±Р»РѕРє.
//            m_xServiseSection.xServiseSectionData.
//            axBlockPositionData[uiBlock].uiOffset = m_xServiseSection.xServiseSectionData.uiFreeSpaceOffset;
//            // Р�Р·РјРµРЅРёРј СЃРјРµС‰РµРЅРёРµ РЅР° СЃРІРѕР±РѕРґРЅРѕРµ РјРµСЃС‚Рѕ.
//            m_xServiseSection.xServiseSectionData.uiFreeSpaceOffset += m_uiBlockEncodedLength;
//            // РЎРѕС…СЂР°РЅРёРј СЂР°Р·РјРµСЂ РїРµСЂРІРёС‡РЅС‹С… РґР°РЅРЅС‹С….
//            m_xServiseSection.xServiseSectionData.
//            axBlockPositionData[uiBlock].uiLength = uiLength;
//            // РЎРѕС…СЂР°РЅРёРј СЂР°Р·РјРµСЂ Р·Р°РєРѕРґРёСЂРѕРІР°РЅРЅС‹С… РґР°РЅРЅС‹С….
//            m_xServiseSection.xServiseSectionData.
//            axBlockPositionData[uiBlock].uiEncodedLength = m_uiBlockEncodedLength;
//
//            // РџРѕР»СѓС‡РёРј РёРЅРґРµРєСЃ Р±Р»РѕРєР°.
//            m_uiBlockIndex = uiBlock;
//            // РџРѕР»СѓС‡РёРј СѓРєР°Р·Р°С‚РµР»СЊ РЅР° РґР°РЅРЅС‹Рµ Р±Р»РѕРєР°.
//            m_puiBlockDataPointer = puiSource;
//            m_uiBlockOffset = m_xServiseSection.xServiseSectionData.
//                              axBlockPositionData[uiBlock].uiOffset;
//            // РџРѕР»СѓС‡РёРј СЂР°Р·РјРµСЂ Р±Р»РѕРєР°.
//            m_uiBlockLength = uiLength;
//
//            // Р’С‹С‡РёСЃР»РёРј РєРѕРЅС‚СЂРѕР»СЊРЅСѓСЋ СЃСѓРјРјСѓ РїРѕСЃС‚СѓРїРёРІС€РёС… РґР°РЅРЅС‹С….
//            m_xServiseSection.xServiseSectionData.
//            axBlockPositionData[uiBlock].uiCrc =
//                usCrc16(puiSource, uiLength);
//
//            SetArgumentData(m_puiIntermediateBuff,
//                            m_uiBlockOffset,
//                            m_uiBlockEncodedLength);
//            return 1;
//        }
//        else
//        {
//            // РќРµС‚ РґР°РЅРЅС‹С….
//            return 0;
//        }
//    }
//    else
//    {
//        switch (uiBlock)
//        {
//        case TEMPORARY_BLOCK_INDEX:
//            // Р—РІРєРѕРґРёСЂСѓРµРј РґР°РЅРЅС‹Рµ Р°Р»РіРѕСЂРёС‚РјРѕРј РҐРµРјРјРёРЅРіР°.
//            m_uiBlockEncodedLength =
//                CHammingCodes::BytesToHammingCodes(m_puiIntermediateBuff,
//                                                   puiSource,
//                                                   uiLength);
//
//            // Р”Р°РЅРЅС‹Рµ С‚РµРєСѓС‰РµРіРѕ Р±Р»РѕРєР° РґР°РЅРЅС‹С… - uiBlock СЃРѕС…СЂР°РЅСЏСЋС‚СЃСЏ РІРїРµСЂРІС‹Рµ?
//            if ((m_xServiseSection.xServiseSectionData.
//                    axBlockPositionData[uiBlock].uiLength == 0) &&
//                    (m_xServiseSection.xServiseSectionData.
//                     axBlockPositionData[uiBlock].uiEncodedLength == 0))
//            {
//                std::cout << "CDataStore::WriteBlock 2"  << std::endl;
//                // Р”РѕР±Р°РІРёР»Рё РЅРѕРІС‹Р№ Р±Р»РѕРє РґР°РЅРЅС‹С….
//                // РЎРѕС…СЂР°РЅРёРј СЃРјРµС‰РµРЅРёРµ РЅР° Р±Р»РѕРє.
//                m_xServiseSection.xServiseSectionData.
//                axBlockPositionData[uiBlock].uiOffset = m_xServiseSection.xServiseSectionData.uiFreeSpaceOffset;
//                // Р�Р·РјРµРЅРёРј СЃРјРµС‰РµРЅРёРµ РЅР° СЃРІРѕР±РѕРґРЅРѕРµ РјРµСЃС‚Рѕ.
//                m_xServiseSection.xServiseSectionData.uiFreeSpaceOffset += m_uiBlockEncodedLength;
//                // РЎРѕС…СЂР°РЅРёРј СЂР°Р·РјРµСЂ РїРµСЂРІРёС‡РЅС‹С… РґР°РЅРЅС‹С….
//                m_xServiseSection.xServiseSectionData.
//                axBlockPositionData[uiBlock].uiLength = uiLength;
//                // РЎРѕС…СЂР°РЅРёРј СЂР°Р·РјРµСЂ Р·Р°РєРѕРґРёСЂРѕРІР°РЅРЅС‹С… РґР°РЅРЅС‹С….
//                m_xServiseSection.xServiseSectionData.
//                axBlockPositionData[uiBlock].uiEncodedLength = m_uiBlockEncodedLength;
//                // РЈРІРµР»РёС‡РёРј РєРѕР»РёС‡РµСЃС‚РІРѕ Р±Р»РѕРєРѕРІ РЅР°С…РѕРґСЏС‰РёС…СЃСЏ РІ С…СЂР°РЅРёР»РёС‰Рµ.
//                m_xServiseSection.xServiseSectionData.uiStoredBlocksNumber += 1;
//            }
//
//            // РџРѕР»СѓС‡РёРј РёРЅРґРµРєСЃ Р±Р»РѕРєР°.
//            m_uiBlockIndex = uiBlock;
//            // РџРѕР»СѓС‡РёРј СѓРєР°Р·Р°С‚РµР»СЊ РЅР° РґР°РЅРЅС‹Рµ Р±Р»РѕРєР°.
//            m_puiBlockDataPointer = puiSource;
//            // РџРѕР»СѓС‡РёРј Р°РґСЂРµСЃ Р±Р»РѕРєР° РІ EEPROM.
//            m_uiBlockOffset = TEMPORARY_BLOCK_DATA_BEGIN;
//            // РџРѕР»СѓС‡РёРј СЂР°Р·РјРµСЂ Р±Р»РѕРєР°.
//            m_uiBlockLength = uiLength;
//
//            // Р’С‹С‡РёСЃР»РёРј РєРѕРЅС‚СЂРѕР»СЊРЅСѓСЋ СЃСѓРјРјСѓ РїРѕСЃС‚СѓРїРёРІС€РёС… РґР°РЅРЅС‹С….
//            m_xServiseSection.xServiseSectionData.
//            axBlockPositionData[uiBlock].uiCrc =
//                usCrc16(puiSource, uiLength);
//
//            SetArgumentData(m_puiIntermediateBuff,
//                            m_uiBlockOffset,
//                            m_uiBlockEncodedLength);
//
//            return 1;
//            break;
//
//        case TEMPORARY_SERVICE_SECTION_INDEX:
//            // РџРѕР»СѓС‡РёРј РёРЅРґРµРєСЃ Р±Р»РѕРєР°.
//            m_uiBlockIndex = uiBlock;
//            // РџРѕР»СѓС‡РёРј СѓРєР°Р·Р°С‚РµР»СЊ РЅР° РґР°РЅРЅС‹Рµ Р±Р»РѕРєР°.
//            m_puiBlockDataPointer =
//                (reinterpret_cast<uint8_t*>(&m_xServiseSection.xServiseSectionData));
//            // РџРѕР»СѓС‡РёРј Р°РґСЂРµСЃ Р±Р»РѕРєР° РІ EEPROM.
//            m_uiBlockOffset = TEMPORARY_SERVICE_SECTION_DATA_BEGIN;
//            // РџРѕР»СѓС‡РёРј СЂР°Р·РјРµСЂ Р±Р»РѕРєР°.
//            m_uiBlockLength = sizeof(struct TServiseSectionData);
//
//            // Р’С‹С‡РёСЃР»РёРј РєРѕРЅС‚СЂРѕР»СЊРЅСѓСЋ СЃСѓРјРјСѓ РїРѕСЃС‚СѓРїРёРІС€РёС… РґР°РЅРЅС‹С….
//            m_xServiseSection.uiCrc =
//                usCrc16(m_puiBlockDataPointer, m_uiBlockLength);
//
//            // Р—РІРєРѕРґРёСЂСѓРµРј РґР°РЅРЅС‹Рµ Р°Р»РіРѕСЂРёС‚РјРѕРј РҐРµРјРјРёРЅРіР°.
//            m_uiBlockEncodedLength =
//                CHammingCodes::BytesToHammingCodes(m_puiIntermediateBuff,
//                                                   m_puiBlockDataPointer,
//                                                   m_uiBlockLength);
//
//            SetArgumentData(m_puiIntermediateBuff,
//                            m_uiBlockOffset,
//                            m_uiBlockEncodedLength);
//
//            return 1;
//            break;
//
//        case SERVICE_SECTION_INDEX:
//            // РџРѕР»СѓС‡РёРј РёРЅРґРµРєСЃ Р±Р»РѕРєР°.
//            m_uiBlockIndex = uiBlock;
//            // РџРѕР»СѓС‡РёРј СѓРєР°Р·Р°С‚РµР»СЊ РЅР° РґР°РЅРЅС‹Рµ Р±Р»РѕРєР°.
//            m_puiBlockDataPointer =
//                (reinterpret_cast<uint8_t*>(&m_xServiseSection.xServiseSectionData));
//            // РџРѕР»СѓС‡РёРј Р°РґСЂРµСЃ Р±Р»РѕРєР° РІ EEPROM.
//            m_uiBlockOffset = SERVICE_SECTION_DATA_BEGIN;
//            // РџРѕР»СѓС‡РёРј СЂР°Р·РјРµСЂ Р±Р»РѕРєР°.
//            m_uiBlockLength = sizeof(struct TServiseSectionData);
//
//            // Р’С‹С‡РёСЃР»РёРј РєРѕРЅС‚СЂРѕР»СЊРЅСѓСЋ СЃСѓРјРјСѓ РїРѕСЃС‚СѓРїРёРІС€РёС… РґР°РЅРЅС‹С….
//            m_xServiseSection.uiCrc =
//                usCrc16(m_puiBlockDataPointer, m_uiBlockLength);
//
//            // Р—РІРєРѕРґРёСЂСѓРµРј РґР°РЅРЅС‹Рµ Р°Р»РіРѕСЂРёС‚РјРѕРј РҐРµРјРјРёРЅРіР°.
//            m_uiBlockEncodedLength =
//                CHammingCodes::BytesToHammingCodes(m_puiIntermediateBuff,
//                                                   m_puiBlockDataPointer,
//                                                   m_uiBlockLength);
//
//            SetArgumentData(m_puiIntermediateBuff,
//                            m_uiBlockOffset,
//                            m_uiBlockEncodedLength);
//
//            return 1;
//            break;
//
//        default:
//            // РќРµС‚ РґР°РЅРЅС‹С….
//            return 0;
//            break;
//        }
//    }
}

//-------------------------------------------------------------------------------
// РЎС‡РёС‚С‹РІР°РµС‚ Рё РїСЂРѕРІРµСЂСЏРµС‚ С†РµР»РѕСЃС‚РЅРѕСЃС‚СЊ РІСЂРµРјРµРЅРЅРѕРіРѕ СЃР»СѓР¶РµР±РЅРѕРіРѕ Р±Р»РѕРєР°.
uint8_t CDataStore::ReadTemporaryServiceSection(void)
{
    uint16_t uiEncodedLength =
        (CHammingCodes::CalculateEncodedDataLength(sizeof(struct TServiseSection)));
    std::cout << "CDataStore::ReadTemporaryServiceSection 1 " << (int)uiEncodedLength  << std::endl;
    // РџСЂРѕС‡РёС‚Р°РµРј Р·Р°РєРѕРґРёСЂРѕРІР°РЅРЅС‹Рµ РґР°РЅРЅС‹Рµ.
    // РџСЂРё С‡С‚РµРЅРёРё РґР°РЅРЅС‹С… РІРѕР·РЅРёРєР»Р° РѕС€РёР±РєР°?
    if (!(m_pxStorageDevice -> ReadBlock(m_puiIntermediateBuff,
                                         TEMPORARY_SERVICE_SECTION_DATA_BEGIN,
                                         uiEncodedLength)))
    {
        std::cout << "CDataStore::ReadTemporaryServiceSection 2"  << std::endl;
        // РќРµС‚ РґР°РЅРЅС‹С….
        return 0;
    }

    // Р”РµРєРѕРґРёСЂСѓРµРј РїСЂРѕС‡РёС‚Р°РЅРЅС‹Рµ РґР°РЅРЅС‹Рµ.
    CHammingCodes::HammingCodesToBytes(reinterpret_cast<uint8_t*>(&m_xServiseSection),
                                       m_puiIntermediateBuff,
                                       uiEncodedLength);

    // Р‘Р»РѕРє РЅРµ РїРѕРІСЂРµР¶РґС‘РЅ?
    if (m_xServiseSection.uiCrc ==
            usCrc16(reinterpret_cast<uint8_t*>(&m_xServiseSection.xServiseSectionData),
                    sizeof(struct TServiseSectionData)))
    {
        std::cout << "CDataStore::ReadTemporaryServiceSection 3"  << std::endl;
        return 1;
    }
    else
    {
        std::cout << "CDataStore::ReadTemporaryServiceSection 4"  << std::endl;
        // РќРµС‚ РґР°РЅРЅС‹С….
        return 0;
    }
}

//-------------------------------------------------------------------------------
// РЎС‡РёС‚С‹РІР°РµС‚ Рё РїСЂРѕРІРµСЂСЏРµС‚ С†РµР»РѕСЃС‚РЅРѕСЃС‚СЊ СЃР»СѓР¶РµР±РЅРѕРіРѕ Р±Р»РѕРєР°.
uint8_t CDataStore::ReadServiceSection(void)
{
    uint16_t uiEncodedLength =
        (CHammingCodes::CalculateEncodedDataLength(sizeof(struct TServiseSection)));

    // РџСЂРѕС‡РёС‚Р°РµРј Р·Р°РєРѕРґРёСЂРѕРІР°РЅРЅС‹Рµ РґР°РЅРЅС‹Рµ.
    // РџСЂРё С‡С‚РµРЅРёРё РґР°РЅРЅС‹С… РІРѕР·РЅРёРєР»Р° РѕС€РёР±РєР°?
    if (!(m_pxStorageDevice -> ReadBlock(m_puiIntermediateBuff,
                                         SERVICE_SECTION_DATA_BEGIN,
                                         uiEncodedLength)))
    {
        // РќРµС‚ РґР°РЅРЅС‹С….
        return 0;
    }

    // Р”РµРєРѕРґРёСЂСѓРµРј РїСЂРѕС‡РёС‚Р°РЅРЅС‹Рµ РґР°РЅРЅС‹Рµ.
    CHammingCodes::HammingCodesToBytes(reinterpret_cast<uint8_t*>(&m_xServiseSection),
                                       m_puiIntermediateBuff,
                                       uiEncodedLength);

    // Р‘Р»РѕРє РЅРµ РїРѕРІСЂРµР¶РґС‘РЅ?
    if (m_xServiseSection.uiCrc ==
            usCrc16(reinterpret_cast<uint8_t*>(&m_xServiseSection.xServiseSectionData),
                    sizeof(struct TServiseSectionData)))
    {
        return 1;
    }
    else
    {
        // РќРµС‚ РґР°РЅРЅС‹С….
        return 0;
    }
}

//-------------------------------------------------------------------------------
// РЎС‡РёС‚С‹РІР°РµС‚ Рё РїСЂРѕРІРµСЂСЏРµС‚ С†РµР»РѕСЃС‚РЅРѕСЃС‚СЊ РІСЂРµРјРµРЅРЅРѕРіРѕ Р±СѓС„РµСЂР° Р±Р»РѕРєР°.
uint8_t CDataStore::CheckTemporaryBlock(void)
{
    uint8_t uiBlock = m_uiBlockIndex;

    // РџСЂРѕРёР·РѕС€С‘Р» РІС‹С…РѕРґ Р·Р° РіСЂР°РЅРёС†С‹ Р±СѓС„РµСЂР°?
    if (uiBlock >= MAX_BLOCKS_NUMBER)
    {
        // РќРµС‚ РґР°РЅРЅС‹С….
        return 0;
    }

    uint16_t uiLength;
    uint16_t uiEncodedLength;
    uint16_t uiSourceOffset;

    // Р‘Р»РѕРє СЃСѓС‰РµСЃС‚РІСѓРµС‚?
    if ((m_xServiseSection.xServiseSectionData.
            axBlockPositionData[uiBlock].uiLength != 0) &&
            (m_xServiseSection.xServiseSectionData.
             axBlockPositionData[uiBlock].uiEncodedLength != 0))
    {
        // РџРѕР»СѓС‡РёРј Р°РґСЂРµСЃ Р±Р»РѕРєР° РІ EEPROM.
        uiSourceOffset = m_xServiseSection.xServiseSectionData.
                         axBlockPositionData[uiBlock].uiOffset;
        // РџРѕР»СѓС‡РёРј СЂР°Р·РјРµСЂ Р±Р»РѕРєР°.
        uiLength = m_xServiseSection.xServiseSectionData.
                   axBlockPositionData[uiBlock].uiLength;
        // РџРѕР»СѓС‡РёРј СЂР°Р·РјРµСЂ Р·Р°РєРѕРґРёСЂРѕРІР°РЅРЅРѕРіРѕ Р±Р»РѕРєР°.
        uiEncodedLength = m_xServiseSection.xServiseSectionData.
                          axBlockPositionData[uiBlock].uiEncodedLength;
    }
    else
    {
        // РќРµС‚ РґР°РЅРЅС‹С….
        return 0;
    }

    // РџСЂРѕС‡РёС‚Р°РµРј Р·Р°РєРѕРґРёСЂРѕРІР°РЅРЅС‹Рµ РґР°РЅРЅС‹Рµ.
    // РџСЂРё С‡С‚РµРЅРёРё РґР°РЅРЅС‹С… РІРѕР·РЅРёРєР»Р° РѕС€РёР±РєР°?
    if (!(m_pxStorageDevice -> ReadBlock(m_puiIntermediateBuff,
                                         TEMPORARY_BLOCK_DATA_BEGIN,
                                         uiEncodedLength)))
    {
        // РќРµС‚ РґР°РЅРЅС‹С….
        return 0;
    }

    // Р”РµРєРѕРґРёСЂСѓРµРј РїСЂРѕС‡РёС‚Р°РЅРЅС‹Рµ РґР°РЅРЅС‹Рµ.
    CHammingCodes::HammingCodesToBytes(m_puiIntermediateBuff,
                                       m_puiIntermediateBuff,
                                       uiEncodedLength);

    // Р‘Р»РѕРє РЅРµ РїРѕРІСЂРµР¶РґС‘РЅ?
    if (m_xServiseSection.xServiseSectionData.
            axBlockPositionData[uiBlock].uiCrc ==
            usCrc16(m_puiIntermediateBuff, uiLength))
    {
        return 1;
    }
    else
    {
        // РќРµС‚ РґР°РЅРЅС‹С….
        return 0;
    }
}

//-------------------------------------------------------------------------------
// РЎС‡РёС‚С‹РІР°РµС‚ Рё РїСЂРѕРІРµСЂСЏРµС‚ С†РµР»РѕСЃС‚РЅРѕСЃС‚СЊ Р±Р»РѕРєР°.
uint8_t CDataStore::CheckBlock(void)
{
    uint8_t uiBlock = m_uiBlockIndex;

    // РџСЂРѕРёР·РѕС€С‘Р» РІС‹С…РѕРґ Р·Р° РіСЂР°РЅРёС†С‹ Р±СѓС„РµСЂР°?
    if (uiBlock >= MAX_BLOCKS_NUMBER)
    {
        // РќРµС‚ РґР°РЅРЅС‹С….
        return 0;
    }

    uint16_t uiLength;
    uint16_t uiEncodedLength;
    uint16_t uiSourceOffset;

    // Р‘Р»РѕРє СЃСѓС‰РµСЃС‚РІСѓРµС‚?
    if ((m_xServiseSection.xServiseSectionData.
            axBlockPositionData[uiBlock].uiLength != 0) &&
            (m_xServiseSection.xServiseSectionData.
             axBlockPositionData[uiBlock].uiEncodedLength != 0))
    {
        // РџРѕР»СѓС‡РёРј Р°РґСЂРµСЃ Р±Р»РѕРєР° РІ EEPROM.
        uiSourceOffset = m_xServiseSection.xServiseSectionData.
                         axBlockPositionData[uiBlock].uiOffset;
        // РџРѕР»СѓС‡РёРј СЂР°Р·РјРµСЂ Р±Р»РѕРєР°.
        uiLength = m_xServiseSection.xServiseSectionData.
                   axBlockPositionData[uiBlock].uiLength;
        // РџРѕР»СѓС‡РёРј СЂР°Р·РјРµСЂ Р·Р°РєРѕРґРёСЂРѕРІР°РЅРЅРѕРіРѕ Р±Р»РѕРєР°.
        uiEncodedLength = m_xServiseSection.xServiseSectionData.
                          axBlockPositionData[uiBlock].uiEncodedLength;
    }
    else
    {
        // РќРµС‚ РґР°РЅРЅС‹С….
        return 0;
    }

    // РџСЂРѕС‡РёС‚Р°РµРј Р·Р°РєРѕРґРёСЂРѕРІР°РЅРЅС‹Рµ РґР°РЅРЅС‹Рµ.
    // РџСЂРё С‡С‚РµРЅРёРё РґР°РЅРЅС‹С… РІРѕР·РЅРёРєР»Р° РѕС€РёР±РєР°?
    if (!(m_pxStorageDevice -> ReadBlock(m_puiIntermediateBuff,
                                         uiSourceOffset,
                                         uiEncodedLength)))
    {
        // РќРµС‚ РґР°РЅРЅС‹С….
        return 0;
    }

    // Р”РµРєРѕРґРёСЂСѓРµРј РїСЂРѕС‡РёС‚Р°РЅРЅС‹Рµ РґР°РЅРЅС‹Рµ.
    CHammingCodes::HammingCodesToBytes(m_puiIntermediateBuff,
                                       m_puiIntermediateBuff,
                                       uiEncodedLength);

    // Р‘Р»РѕРє РЅРµ РїРѕРІСЂРµР¶РґС‘РЅ?
    if (m_xServiseSection.xServiseSectionData.
            axBlockPositionData[uiBlock].uiCrc ==
            usCrc16(m_puiIntermediateBuff, uiLength))
    {
        return 1;
    }
    else
    {
        // РќРµС‚ РґР°РЅРЅС‹С….
        return 0;
    }
}

//-------------------------------------------------------------------------------
// РЎС‡РёС‚С‹РІР°РµС‚ Рё РїСЂРѕРІРµСЂСЏРµС‚ С†РµР»РѕСЃС‚РЅРѕСЃС‚СЊ Р±Р»РѕРєР°.
uint16_t CDataStore::ReadBlock(uint8_t *puiDestination, uint8_t uiBlock)
{
    // РџСЂРѕРёР·РѕС€С‘Р» РІС‹С…РѕРґ Р·Р° РіСЂР°РЅРёС†С‹ Р±СѓС„РµСЂР°?
    if (uiBlock >= (MAX_BLOCKS_NUMBER + SERVICE_SECTION_BLOCK_NUMBER))
    {
        // РќРµС‚ РґР°РЅРЅС‹С….
        return 0;
    }
    // Р±Р»РѕРє РЅРµ СЃР»СѓР¶РµР±РЅС‹Р№?
    if (uiBlock < MAX_BLOCKS_NUMBER)
    {
        // Р‘Р»РѕРє СЃСѓС‰РµСЃС‚РІСѓРµС‚?
        if ((m_xServiseSection.xServiseSectionData.
                axBlockPositionData[uiBlock].uiLength != 0) &&
                (m_xServiseSection.xServiseSectionData.
                 axBlockPositionData[uiBlock].uiEncodedLength != 0))
        {
            // РџРѕР»СѓС‡РёРј РёРЅРґРµРєСЃ Р±Р»РѕРєР°.
            m_uiBlockIndex = uiBlock;
            // РџРѕР»СѓС‡РёРј СѓРєР°Р·Р°С‚РµР»СЊ РЅР° РґР°РЅРЅС‹Рµ Р±Р»РѕРєР°.
            m_puiBlockDataPointer = puiDestination;
            // РџРѕР»СѓС‡РёРј Р°РґСЂРµСЃ Р±Р»РѕРєР° РІ EEPROM.
            m_uiBlockOffset = m_xServiseSection.xServiseSectionData.
                              axBlockPositionData[uiBlock].uiOffset;
            // РџРѕР»СѓС‡РёРј СЂР°Р·РјРµСЂ Р±Р»РѕРєР°.
            m_uiBlockLength = m_xServiseSection.xServiseSectionData.
                              axBlockPositionData[uiBlock].uiLength;
            // РџРѕР»СѓС‡РёРј СЂР°Р·РјРµСЂ Р·Р°РєРѕРґРёСЂРѕРІР°РЅРЅРѕРіРѕ Р±Р»РѕРєР°.
            m_uiBlockEncodedLength = m_xServiseSection.xServiseSectionData.
                                     axBlockPositionData[uiBlock].uiEncodedLength;

            SetArgumentData(m_puiIntermediateBuff,
                            m_uiBlockOffset,
                            m_uiBlockEncodedLength);
            return 1;
        }
        else
        {
            // РќРµС‚ РґР°РЅРЅС‹С….
            return 0;
        }
    }
    else
    {
        // РџРѕР»СѓС‡РёРј СѓРєР°Р·Р°С‚РµР»СЊ РЅР° РґР°РЅРЅС‹Рµ Р±Р»РѕРєР°.
        m_puiBlockDataPointer = puiDestination;
        // РџРѕР»СѓС‡РёРј СЂР°Р·РјРµСЂ Р·Р°РєРѕРґРёСЂРѕРІР°РЅРЅРѕРіРѕ Р±Р»РѕРєР°.
        m_uiBlockEncodedLength =
            (CHammingCodes::CalculateEncodedDataLength(sizeof(struct TServiseSection)));

        switch (uiBlock)
        {
        case TEMPORARY_BLOCK_INDEX:
            // РџРѕР»СѓС‡РёРј Р°РґСЂРµСЃ Р±Р»РѕРєР° РІ EEPROM.
            m_uiBlockOffset = TEMPORARY_BLOCK_DATA_BEGIN;
            // РџРѕР»СѓС‡РёРј СЂР°Р·РјРµСЂ Р±Р»РѕРєР°.
            m_uiBlockLength = m_xServiseSection.xServiseSectionData.
                              axBlockPositionData[m_uiBlockIndex].uiLength;
            break;

        case TEMPORARY_SERVICE_SECTION_INDEX:
            // РџРѕР»СѓС‡РёРј Р°РґСЂРµСЃ Р±Р»РѕРєР° РІ EEPROM.
            m_uiBlockOffset = TEMPORARY_SERVICE_SECTION_DATA_BEGIN;
            // РџРѕР»СѓС‡РёРј СЂР°Р·РјРµСЂ Р±Р»РѕРєР°.
            m_uiBlockLength = sizeof(struct TServiseSectionData);
            break;

        case SERVICE_SECTION_INDEX:
            // РџРѕР»СѓС‡РёРј Р°РґСЂРµСЃ Р±Р»РѕРєР° РІ EEPROM.
            m_uiBlockOffset = SERVICE_SECTION_DATA_BEGIN;
            // РџРѕР»СѓС‡РёРј СЂР°Р·РјРµСЂ Р±Р»РѕРєР°.
            m_uiBlockLength = sizeof(struct TServiseSectionData);
            break;

        default:
            // РќРµС‚ РґР°РЅРЅС‹С….
            return 0;
            break;
        }

        SetArgumentData(m_puiIntermediateBuff,
                        m_uiBlockOffset,
                        m_uiBlockEncodedLength);

        return 1;
    }

//    uint16_t uiLength;
//    uint16_t uiEncodedLength;
//    uint16_t uiSourceOffset;

//    // Р‘Р»РѕРє СЃСѓС‰РµСЃС‚РІСѓРµС‚?
//    if ((m_xServiseSection.xServiseSectionData.
//            axBlockPositionData[uiBlock].uiLength != 0) &&
//            (m_xServiseSection.xServiseSectionData.
//             axBlockPositionData[uiBlock].uiEncodedLength != 0))
//    {
//        // РџРѕР»СѓС‡РёРј Р°РґСЂРµСЃ Р±Р»РѕРєР° РІ EEPROM.
//        uiSourceOffset = m_xServiseSection.xServiseSectionData.
//                         axBlockPositionData[uiBlock].uiOffset;
//        // РџРѕР»СѓС‡РёРј СЂР°Р·РјРµСЂ Р±Р»РѕРєР°.
//        uiLength = m_xServiseSection.xServiseSectionData.
//                   axBlockPositionData[uiBlock].uiLength;
//        // РџРѕР»СѓС‡РёРј СЂР°Р·РјРµСЂ Р·Р°РєРѕРґРёСЂРѕРІР°РЅРЅРѕРіРѕ Р±Р»РѕРєР°.
//        uiEncodedLength = m_xServiseSection.xServiseSectionData.
//                          axBlockPositionData[uiBlock].uiEncodedLength;

//        // РџРѕР»СѓС‡РёРј РёРЅРґРµРєСЃ Р±Р»РѕРєР°.
//        m_uiBlockIndex = uiBlock;
//        // РџРѕР»СѓС‡РёРј СѓРєР°Р·Р°С‚РµР»СЊ РЅР° РґР°РЅРЅС‹Рµ Р±Р»РѕРєР°.
//        m_puiBlockDataPointer = puiDestination;
//        // РџРѕР»СѓС‡РёРј Р°РґСЂРµСЃ Р±Р»РѕРєР° РІ EEPROM.
//        m_uiBlockOffset = m_xServiseSection.xServiseSectionData.
//                          axBlockPositionData[uiBlock].uiOffset;
//        // РџРѕР»СѓС‡РёРј СЂР°Р·РјРµСЂ Р±Р»РѕРєР°.
//        m_uiBlockLength = m_xServiseSection.xServiseSectionData.
//                          axBlockPositionData[uiBlock].uiLength;
//        // РџРѕР»СѓС‡РёРј СЂР°Р·РјРµСЂ Р·Р°РєРѕРґРёСЂРѕРІР°РЅРЅРѕРіРѕ Р±Р»РѕРєР°.
//        m_uiBlockEncodedLength = m_xServiseSection.xServiseSectionData.
//                                 axBlockPositionData[uiBlock].uiEncodedLength;
//
//        SetArgumentData(m_puiIntermediateBuff,
//                        m_uiBlockOffset,
//                        m_uiBlockEncodedLength);
////        SetArgumentData(m_puiIntermediateBuff,
////                        m_xServiseSection.xServiseSectionData.
////                        axBlockPositionData[uiBlock].uiOffset,
////                        m_xServiseSection.xServiseSectionData.
////                        axBlockPositionData[uiBlock].uiEncodedLength);
//        return 1;
//    }
//    else
//    {
//        // РќРµС‚ РґР°РЅРЅС‹С….
//        return 0;
//    }

//    // РџСЂРѕС‡РёС‚Р°РµРј Р·Р°РєРѕРґРёСЂРѕРІР°РЅРЅС‹Рµ РґР°РЅРЅС‹Рµ.
//    m_pxStorageDevice -> ReadBlock(m_puiIntermediateBuff,
//                                   uiSourceOffset,
//                                   uiEncodedLength);
//
//    // Р”РµРєРѕРґРёСЂСѓРµРј РїСЂРѕС‡РёС‚Р°РЅРЅС‹Рµ РґР°РЅРЅС‹Рµ.
//    CHammingCodes::HammingCodesToBytes(m_puiIntermediateBuff,
//                                       m_puiIntermediateBuff,
//                                       uiEncodedLength);
//
//    // Р‘Р»РѕРє РЅРµ РїРѕРІСЂРµР¶РґС‘РЅ?
//    if (m_xServiseSection.xServiseSectionData.
//            axBlockPositionData[uiBlock].uiCrc ==
//            usCrc16(m_puiIntermediateBuff, uiLength))
//    {
//        memcpy(puiDestination, m_puiIntermediateBuff, uiLength);
//        return uiLength;
//    }
//    else
//    {
//        // РќРµС‚ РґР°РЅРЅС‹С….
//        return 0;
//    }
}

//-------------------------------------------------------------------------------
// РЎС‡РёС‚С‹РІР°РµС‚ Рё РїСЂРѕРІРµСЂСЏРµС‚ С†РµР»РѕСЃС‚РЅРѕСЃС‚СЊ Р±Р»РѕРєР°.
uint16_t CDataStore::GetReadedData()
{
    uint8_t auiTempArray[CDataStore::MAX_SERVICE_SECTION_DATA_LENGTH];

    // Р”РµРєРѕРґРёСЂСѓРµРј РїСЂРѕС‡РёС‚Р°РЅРЅС‹Рµ РґР°РЅРЅС‹Рµ.
    CHammingCodes::HammingCodesToBytes(auiTempArray,
                                       m_puiIntermediateBuff,
                                       m_uiBlockEncodedLength);

    // Р‘Р»РѕРє РЅРµ РїРѕРІСЂРµР¶РґС‘РЅ?
    if (m_xServiseSection.xServiseSectionData.
            axBlockPositionData[m_uiBlockIndex].uiCrc ==
            usCrc16(auiTempArray, m_uiBlockLength))
    {
        memcpy(m_puiBlockDataPointer, auiTempArray, m_uiBlockLength);
        return m_uiBlockLength;
    }
    else
    {
        // РќРµС‚ РґР°РЅРЅС‹С….
        return 0;
    }
}

//-------------------------------------------------------------------------------
// Р’С‹Р·С‹РІР°РµС‚СЃСЏ С‚РѕР»СЊРєРѕ РµСЃР»Рё Р±Р°Р·Р° РґР°РЅРЅС‹С… РїРѕРґС‚РІРµСЂР¶РґРµРЅР° РїРѕР»СЊР·РѕРІР°С‚РµР»РµРј.
void CDataStore::CrcOfBlocksCrcCreate(void)
{
    // РљРѕРЅС‚СЂРѕР»СЊРЅР°СЏ СЃСѓРјРјР° РІС‹С‡РёСЃР»РµРЅРЅР°СЏ РёР· РјР°СЃСЃРёРІР° РєРѕРЅС‚СЂРѕР»СЊРЅС‹С… СЃСѓРјРј Р±Р»РѕРєРѕРІ, РЅРµ РІРєР»СЋС‡Р°СЏ СЃР»СѓР¶РµР±РЅС‹Р№.
    // РЎРѕС…СЂР°РЅСЏРµС‚СЃСЏ РїСЂРё РїРµСЂРІРѕР№ Рё РїРѕСЃР»РµРґСѓСЋС‰РёС… Р·Р°РїРёСЃСЏС… Р»СЋР±С‹С… Р±Р»РѕРєРѕРІ С‡РµСЂРµР· РїСЂРѕРіСЂР°РјРјР°С‚РѕСЂ.
    // РќРѕР»СЊ РёР»Рё РµС‘ РЅРµСЃРѕРІРїР°РґРµРЅРёРµ СЃРІРёРґРµС‚РµР»СЊСЃС‚РІСѓРµС‚ Рѕ С‚РѕРј, С‡С‚Рѕ Р±Р°Р·Р° РґР°РЅРЅС‹С… СЃРѕР·РґР°РЅР° РїРѕ СѓРјРѕР»Р°РЅРёСЋ,
    // Рё РЅРµ РїРѕРґС‚РІРµСЂР¶РґРµРЅР° РїРѕР»СЊР·РѕРІР°С‚РµР»РµРј. Р’ СЌС‚РѕРј СЃР»СѓС‡Р°Рµ РїСЂРёР±РѕСЂ РїРµСЂРµС…РѕРґРёС‚ РІ СЂРµР¶РёРј СЃРёРіРЅР°Р»РёР·Р°С†РёРё РѕР± РѕС€РёР±РєРµ,
    // РѕР¶РёРґР°СЏ РєРІРёС‚РёСЂРѕРІР°РЅРёСЏ РёР»Рё Р·Р°РїРёСЃРё Р±Р°Р·С‹ РґР°РЅРЅС‹С….
    uint16_t auiBlocksCrc[MAX_BLOCKS_NUMBER];

    // РџРѕР»СѓС‡РёРј Crc РІСЃРµС… Р±Р»РѕРєРѕРІ.
    for (uint16_t i = 0;
            i < MAX_BLOCKS_NUMBER;
            i++)
    {
        auiBlocksCrc[i] =
            m_xServiseSection.xServiseSectionData.axBlockPositionData[i].uiCrc;
    }

    // РЎРѕС…СЂР°РЅРёРј Crc РІСЃРµС… Р±Р»РѕРєРѕРІ.
    m_xServiseSection.xServiseSectionData.uiCrcOfBlocksCrc =
        usCrc16(reinterpret_cast<uint8_t*>(auiBlocksCrc),
                (MAX_BLOCKS_NUMBER * sizeof(uint16_t)));

}

//-------------------------------------------------------------------------------
bool CDataStore::CrcOfBlocksCrcCheck(void)
{
    // РљРѕРЅС‚СЂРѕР»СЊРЅР°СЏ СЃСѓРјРјР° РІС‹С‡РёСЃР»РµРЅРЅР°СЏ РёР· РјР°СЃСЃРёРІР° РєРѕРЅС‚СЂРѕР»СЊРЅС‹С… СЃСѓРјРј Р±Р»РѕРєРѕРІ, РЅРµ РІРєР»СЋС‡Р°СЏ СЃР»СѓР¶РµР±РЅС‹Р№.
    // РЎРѕС…СЂР°РЅСЏРµС‚СЃСЏ РїСЂРё РїРµСЂРІРѕР№ Рё РїРѕСЃР»РµРґСѓСЋС‰РёС… Р·Р°РїРёСЃСЏС… Р»СЋР±С‹С… Р±Р»РѕРєРѕРІ С‡РµСЂРµР· РїСЂРѕРіСЂР°РјРјР°С‚РѕСЂ.
    // РќРѕР»СЊ РёР»Рё РµС‘ РЅРµСЃРѕРІРїР°РґРµРЅРёРµ СЃРІРёРґРµС‚РµР»СЊСЃС‚РІСѓРµС‚ Рѕ С‚РѕРј, С‡С‚Рѕ Р±Р°Р·Р° РґР°РЅРЅС‹С… СЃРѕР·РґР°РЅР° РїРѕ СѓРјРѕР»Р°РЅРёСЋ,
    // Рё РЅРµ РїРѕРґС‚РІРµСЂР¶РґРµРЅР° РїРѕР»СЊР·РѕРІР°С‚РµР»РµРј. Р’ СЌС‚РѕРј СЃР»СѓС‡Р°Рµ РїСЂРёР±РѕСЂ РїРµСЂРµС…РѕРґРёС‚ РІ СЂРµР¶РёРј СЃРёРіРЅР°Р»РёР·Р°С†РёРё РѕР± РѕС€РёР±РєРµ,
    // РѕР¶РёРґР°СЏ РєРІРёС‚РёСЂРѕРІР°РЅРёСЏ РёР»Рё Р·Р°РїРёСЃРё Р±Р°Р·С‹ РґР°РЅРЅС‹С….
    uint16_t auiBlocksCrc[MAX_BLOCKS_NUMBER];

    // РџРѕР»СѓС‡РёРј Crc РІСЃРµС… Р±Р»РѕРєРѕРІ.
    for (uint16_t i = 0;
            i < MAX_BLOCKS_NUMBER;
            i++)
    {
        auiBlocksCrc[i] =
            m_xServiseSection.xServiseSectionData.axBlockPositionData[i].uiCrc;
    }

    // Crc РёР· Crc РІСЃРµС… Р±Р»РѕРєРѕРІ СЃРѕРІРїР°РґР°РµС‚ СЃ СЃРѕС…СЂР°РЅС‘РЅРЅС‹Рј РІ СЃР»СѓР¶РµР±РЅРѕРј Р±Р»РѕРєРµ?
    if (m_xServiseSection.xServiseSectionData.uiCrcOfBlocksCrc ==
            usCrc16(reinterpret_cast<uint8_t*>(auiBlocksCrc),
                    (MAX_BLOCKS_NUMBER * sizeof(uint16_t))))
    {
        return true;
    }
    else
    {
        return false;
    }
}


//-------------------------------------------------------------------------------
uint8_t CDataStore::GetBlockLength(uint8_t uiBlock)
{
//    return m_xServiseSection.xServiseSectionData.
//           axBlockPositionData[uiBlock].uiLength;
    return aucDataBaseBlockLength[uiBlock];
//    return 0;
};

//-------------------------------------------------------------------------------
// Р“Р»Р°РІРЅС‹Р№ Р°РІС‚РѕРјР°С‚ Р·Р°РїРёСЃРё С…СЂР°РЅРёР»РёС‰Р°.
uint8_t CDataStore::Fsm(void)
{
//        std::cout << "CDataStore::Fsm 1"  << std::endl;
    switch (GetFsmState())
    {
    case IDDLE:
//        std::cout << "CDataStore::Fsm IDDLE"  << std::endl;
        break;

    case STOP:
//        std::cout << "CDataStore::Fsm STOP"  << std::endl;
        break;

    case START:
        std::cout << "CDataStore::Fsm START"  << std::endl;
        std::cout << "CDataStore::Fsm m_sStorageDeviceName" << " " << (m_sStorageDeviceName) << std::endl;
        GetTimerPointer() -> Set(TASK_READY_WAITING_TIME);
        SetFsmState(INIT);
        break;

    case INIT:
//        std::cout << "CDataStore::Fsm INIT"  << std::endl;
    {
        CTaskInterface* pxTask =
            GetResources() ->
            GetCommonTaskFromMapPointer(m_sStorageDeviceName);

        if (pxTask != 0)
        {
            if (pxTask -> GetFsmState() >= READY)
            {
                SetStorageDevice((CStorageDeviceInterface*)pxTask);
                SetFsmState(READY);
                std::cout << "CDataStore::Fsm READY"  << std::endl;
            }
        }
        else
        {
            if (GetTimerPointer() -> IsOverflow())
            {
                SetFsmState(STOP);
                std::cout << "CDataStore::Fsm STOP"  << std::endl;
            }
        }
    }
//    {
//        m_pxTaskCustomer =
//            GetResources() ->
//            GetCommonTaskFromMapPointer(m_sTaskCustomerName);
//
//        if (m_pxTaskCustomer != 0)
//        {
//            SetFsmState(READY);
//            std::cout << "CDataStore::Fsm READY"  << std::endl;
//        }
//        else
//        {
//            if (GetTimerPointer() -> IsOverflow())
//            {
//                SetFsmState(STOP);
//                std::cout << "CDataStore::Fsm STOP"  << std::endl;
//            }
//        }
//    }
    break;

    case READY:
//        std::cout << "CDataStore::Fsm READY"  << std::endl;
        if (((static_cast<CDataContainerDataBase*>(m_pxDataContainer)) -> m_uiFsmCommandState) != 0)
        {
            (static_cast<CDataContainerDataBase*>(m_pxDataContainer)) -> m_uiFsmCommandState);
            (static_cast<CDataContainerDataBase*>(m_pxDataContainer)) -> m_uiFsmCommandState = 0;
        }
        break;

//-------------------------------------------------------------------------------
    // Р—Р°РїРёСЃСЊ Р±Р»РѕРєР° РІРѕ РІСЂРµРјРµРЅРЅС‹Р№ Р±СѓС„РµСЂ.
    case START_WRITE_TEMPORARY_BLOCK_DATA:
        std::cout << "CDataStore::Fsm 1"  << std::endl;
//        SetFsmEvent(WRITE_IN_PROGRESS_FSM_EVENT);
//        // РЈСЃС‚Р°РЅРѕРІРёРј РІСЂРµРјСЏ РѕР¶РёРґР°РЅРёСЏ РіРѕС‚РѕРІРЅРѕСЃС‚Рё Рє Р·Р°РїРёСЃРё.
//        GetTimerPointer() -> Set(READY_TO_WRITE_WAITING_TIMEOUT);
        TemporaryBlockWritePrepare();
//        WriteBlock(uint8_t *puiSource, uint16_t uiLength, TEMPORARY_BLOCK_INDEX)
//        m_uiFsmCommandState = CStorageDeviceFileSystem::WRITE_DATA_START;
        // РЈСЃС‚Р°РЅРѕРІРёРј РІСЂРµРјСЏ РѕР¶РёРґР°РЅРёСЏ РѕРєРѕРЅС‡Р°РЅРёСЏ Р·Р°РїРёСЃРё.
        GetTimerPointer() -> Set(WRITE_END_WAITING_TIMEOUT);
        SetFsmState(WRITE_END_WAITING_TEMPORARY_BLOCK_DATA);
        //        SetFsmState(READY_TO_WRITE_WAITING_TEMPORARY_BLOCK_DATA);
        break;

//    case READY_TO_WRITE_WAITING_TEMPORARY_BLOCK_DATA:
//        std::cout << "CDataStore::Fsm 2"  << std::endl;
////        // РЈСЃС‚СЂРѕР№СЃС‚РІРѕ С…СЂР°РЅРµРЅРёСЏ РіРѕС‚РѕРІРѕ Рє Р·Р°РїРёСЃРё?
//////        if (m_pxStorageDevice -> IsReadyToWrite())
//        if (m_pxStorageDevice -> Fsm() == CStorageDeviceInterface::READY)
//        {
//            // РЈСЃС‚Р°РЅРѕРІРёРј РІСЂРµРјСЏ РѕР¶РёРґР°РЅРёСЏ РѕРєРѕРЅС‡Р°РЅРёСЏ Р·Р°РїРёСЃРё.
//            GetTimerPointer() -> Set(WRITE_END_WAITING_TIMEOUT);
////            // РЎР±СЂРѕСЃРёРј С„Р»Р°Рі - Р·Р°РїРёСЃСЊ Р·Р°РєРѕРЅС‡РµРЅР°.
////            m_pxStorageDevice -> SetIsDataWrited(false);
//            // РџРѕРґРіРѕС‚РѕРІРєР° Рє Р·Р°РїРёСЃРё РІРѕ РІСЂРµРјРµРЅРЅС‹Р№ Р±СѓС„РµСЂ РїСЂРѕС€Р»Р° СѓСЃРїРµС€РЅРѕ?
//        if (TemporaryBlockWritePrepare())
//        {
//            std::cout << "CDataStore::Fsm 3"  << std::endl;
//            SetFsmState(WRITE_END_WAITING_TEMPORARY_BLOCK_DATA);
//        }
//        else
//        {
//            std::cout << "CDataStore::Fsm 4"  << std::endl;
////                SetFsmEvent(WRITE_ERROR_FSM_EVENT);
//            SetFsmState(WRITE_ERROR);
//        }
//        }
//        // Р’СЂРµРјСЏ РѕР¶РёРґР°РЅРёСЏ РіРѕС‚РѕРІРЅРѕСЃС‚Рё Рє Р·Р°РїРёСЃРё Р·Р°РєРѕРЅС‡РёР»РѕСЃСЊ?
//        else if (GetTimerPointer() -> IsOverflow())
//        {
////                SetFsmEvent(WRITE_ERROR_FSM_EVENT);
//            SetFsmState(WRITE_ERROR);
//        }
//        break;

    case WRITE_END_WAITING_TEMPORARY_BLOCK_DATA:
        std::cout << "CDataStore::Fsm 5"  << std::endl;
        // РћР¶РёРґР°РµРј РѕРєРѕРЅС‡Р°РЅРёСЏ Р·Р°РїРёСЃРё Р°РІС‚РѕРјР°С‚РѕРј СѓСЃС‚СЂРѕР№СЃС‚РІР° С…СЂР°РЅРµРЅРёСЏ.
        // РЈСЃС‚СЂРѕР№СЃС‚РІРѕ С…СЂР°РЅРµРЅРёСЏ Р·Р°РєРѕРЅС‡РёР»Рѕ Р·Р°РїРёСЃСЊ?
//        if (m_pxStorageDevice -> IsDataWrited())
//        if (m_pxStorageDevice -> Fsm() ==
//                CStorageDeviceInterface::DATA_WRITED_SUCCESSFULLY)
//        if (m_uiFsmAnswerState ==
//                CStorageDeviceInterface::DATA_WRITED_SUCCESSFULLY)
//        {
//            std::cout << "CDataStore::Fsm 6"  << std::endl;
//            m_uiFsmAnswerState = 0;
////            m_pxStorageDevice -> SetFsmState(CStorageDeviceInterface::READY);
//            // Р—Р°РїРёСЃСЊ РїСЂРѕС€Р»Р° СѓСЃРїРµС€РЅРѕ?
//            if (CheckTemporaryBlock())
//            {
//                cerr << "CheckTemporaryBlock ok" << endl;
//                SetFsmState(START_WRITE_TEMPORARY_SERVICE_SECTION_DATA);
//            }
//            else
//            {
//                cerr << "CheckTemporaryBlock error" << endl;
////                SetFsmEvent(WRITE_ERROR_FSM_EVENT);
//                SetFsmState(WRITE_ERROR);
//            }
//        }
//        // Р’СЂРµРјСЏ РѕР¶РёРґР°РЅРёСЏ РѕРєРѕРЅС‡Р°РЅРёСЏ Р·Р°РїРёСЃРё Р·Р°РєРѕРЅС‡РёР»РѕСЃСЊ?
//        else if (GetTimerPointer() -> IsOverflow())
//        {
//            std::cout << "CDataStore::Fsm 7"  << std::endl;
//            m_pxStorageDevice -> SetFsmState(CStorageDeviceInterface::READY);
////                SetFsmEvent(WRITE_ERROR_FSM_EVENT);
//            SetFsmState(WRITE_ERROR);
//        }
        break;


//-------------------------------------------------------------------------------
    // Р—Р°РїРёСЃСЊ РІСЂРµРјРµРЅРЅРѕРіРѕ СЃР»СѓР¶РµСЊРЅРѕРіРѕ Р±Р»РѕРєР°.
    case START_WRITE_TEMPORARY_SERVICE_SECTION_DATA:
        std::cout << "CDataStore::Fsm 8"  << std::endl;
//        SetFsmEvent(WRITE_IN_PROGRESS_FSM_EVENT);
        // РЈСЃС‚Р°РЅРѕРІРёРј РІСЂРµРјСЏ РѕР¶РёРґР°РЅРёСЏ РіРѕС‚РѕРІРЅРѕСЃС‚Рё Рє Р·Р°РїРёСЃРё.
        GetTimerPointer() -> Set(READY_TO_WRITE_WAITING_TIMEOUT);
        SetFsmState(READY_TO_WRITE_WAITING_TEMPORARY_SERVICE_SECTION_DATA);
        break;

    case READY_TO_WRITE_WAITING_TEMPORARY_SERVICE_SECTION_DATA:
        std::cout << "CDataStore::Fsm 9"  << std::endl;
        // РЈСЃС‚СЂРѕР№СЃС‚РІРѕ С…СЂР°РЅРµРЅРёСЏ РіРѕС‚РѕРІРѕ Рє Р·Р°РїРёСЃРё?
//        if (m_pxStorageDevice -> IsReadyToWrite())
        if (m_pxStorageDevice -> Fsm() == CStorageDeviceInterface::READY)
        {
            std::cout << "CDataStore::Fsm 9 1"  << std::endl;
            // РЈСЃС‚Р°РЅРѕРІРёРј РІСЂРµРјСЏ РѕР¶РёРґР°РЅРёСЏ РѕРєРѕРЅС‡Р°РЅРёСЏ Р·Р°РїРёСЃРё.
            GetTimerPointer() -> Set(WRITE_END_WAITING_TIMEOUT);
//            // РЎР±СЂРѕСЃРёРј С„Р»Р°Рі - Р·Р°РїРёСЃСЊ Р·Р°РєРѕРЅС‡РµРЅР°.
//            m_pxStorageDevice -> SetIsDataWrited(false);
            // РџРѕРґРіРѕС‚РѕРІРєР° Рє Р·Р°РїРёСЃРё РїСЂРѕС€Р»Р° СѓСЃРїРµС€РЅРѕ?
            if (TemporaryServiceSectionWritePrepare())
            {
                std::cout << "CDataStore::Fsm 10"  << std::endl;
                SetFsmState(WRITE_END_WAITING_TEMPORARY_SERVICE_SECTION_DATA);
            }
            else
            {
                std::cout << "CDataStore::Fsm 11"  << std::endl;
//                SetFsmEvent(WRITE_ERROR_FSM_EVENT);
                SetFsmState(WRITE_ERROR);
            }
        }
        // Р’СЂРµРјСЏ РѕР¶РёРґР°РЅРёСЏ РіРѕС‚РѕРІРЅРѕСЃС‚Рё Рє Р·Р°РїРёСЃРё Р·Р°РєРѕРЅС‡РёР»РѕСЃСЊ?
        else if (GetTimerPointer() -> IsOverflow())
        {
            std::cout << "CDataStore::Fsm 12"  << std::endl;
//                SetFsmEvent(WRITE_ERROR_FSM_EVENT);
            SetFsmState(WRITE_ERROR);
        }
        break;

    case WRITE_END_WAITING_TEMPORARY_SERVICE_SECTION_DATA:
        // РћР¶РёРґР°РµРј РѕРєРѕРЅС‡Р°РЅРёСЏ Р·Р°РїРёСЃРё Р°РІС‚РѕРјР°С‚РѕРј СѓСЃС‚СЂРѕР№СЃС‚РІР° С…СЂР°РЅРµРЅРёСЏ.
        // РЈСЃС‚СЂРѕР№СЃС‚РІРѕ С…СЂР°РЅРµРЅРёСЏ Р·Р°РєРѕРЅС‡РёР»Рѕ Р·Р°РїРёСЃСЊ?
//        if (m_pxStorageDevice -> IsDataWrited())
        if (m_pxStorageDevice -> Fsm() ==
                CStorageDeviceInterface::DATA_WRITED_SUCCESSFULLY)
        {
            std::cout << "CDataStore::Fsm 13"  << std::endl;
            m_pxStorageDevice -> SetFsmState(CStorageDeviceInterface::READY);
            // Р—Р°РїРёСЃСЊ РїСЂРѕС€Р»Р° СѓСЃРїРµС€РЅРѕ?
            if (ReadTemporaryServiceSection())
            {
                cerr << "ReadTemporaryServiceSection ok" << endl;
                SetFsmState(START_WRITE_BLOCK_DATA);
            }
            else
            {
                cerr << "ReadTemporaryServiceSection error" << endl;
//                SetFsmEvent(WRITE_ERROR_FSM_EVENT);
                SetFsmState(WRITE_ERROR);
            }
        }
        // Р’СЂРµРјСЏ РѕР¶РёРґР°РЅРёСЏ РѕРєРѕРЅС‡Р°РЅРёСЏ Р·Р°РїРёСЃРё Р·Р°РєРѕРЅС‡РёР»РѕСЃСЊ?
        else if (GetTimerPointer() -> IsOverflow())
        {
            std::cout << "CDataStore::Fsm 14"  << std::endl;
            m_pxStorageDevice -> SetFsmState(CStorageDeviceInterface::READY);
//                SetFsmEvent(WRITE_ERROR_FSM_EVENT);
            SetFsmState(WRITE_ERROR);
        }
        break;


//-------------------------------------------------------------------------------
    // Р—Р°РїРёСЃСЊ Р±Р»РѕРєР°.
    case START_WRITE_BLOCK_DATA:
//        // РЈСЃС‚Р°РЅРѕРІРёРј РІСЂРµРјСЏ РѕР¶РёРґР°РЅРёСЏ РіРѕС‚РѕРІРЅРѕСЃС‚Рё Рє Р·Р°РїРёСЃРё.
        GetTimerPointer() -> Set(READY_TO_WRITE_WAITING_TIMEOUT);
        SetFsmState(READY_TO_WRITE_WAITING_BLOCK_DATA);
        break;

    case READY_TO_WRITE_WAITING_BLOCK_DATA:
//        // РЈСЃС‚СЂРѕР№СЃС‚РІРѕ С…СЂР°РЅРµРЅРёСЏ РіРѕС‚РѕРІРѕ Рє Р·Р°РїРёСЃРё?
////        if (m_pxStorageDevice -> IsReadyToWrite())
        if (m_pxStorageDevice -> Fsm() == CStorageDeviceInterface::READY)
        {
            std::cout << "CDataStore::Fsm 15"  << std::endl;
            // РЈСЃС‚Р°РЅРѕРІРёРј РІСЂРµРјСЏ РѕР¶РёРґР°РЅРёСЏ РѕРєРѕРЅС‡Р°РЅРёСЏ Р·Р°РїРёСЃРё Р·Р°РїРёСЃРё.
            GetTimerPointer() -> Set(WRITE_END_WAITING_TIMEOUT);
//            // РЎР±СЂРѕСЃРёРј С„Р»Р°Рі - Р·Р°РїРёСЃСЊ Р·Р°РєРѕРЅС‡РµРЅР°.
//            m_pxStorageDevice -> SetIsDataWrited(false);
            // РџРѕРґРіРѕС‚РѕРІРєР° Рє Р·Р°РїРёСЃРё РїСЂРѕС€Р»Р° СѓСЃРїРµС€РЅРѕ?
            if (BlockWritePrepare())
            {
                std::cout << "CDataStore::Fsm 16"  << std::endl;
                SetFsmState(WRITE_END_WAITING_BLOCK_DATA);
            }
            else
            {
                std::cout << "CDataStore::Fsm 17"  << std::endl;
//                SetFsmEvent(WRITE_ERROR_FSM_EVENT);
                SetFsmState(WRITE_ERROR);
            }
        }
        // Р’СЂРµРјСЏ РѕР¶РёРґР°РЅРёСЏ РіРѕС‚РѕРІРЅРѕСЃС‚Рё Рє Р·Р°РїРёСЃРё Р·Р°РєРѕРЅС‡РёР»РѕСЃСЊ?
        else if (GetTimerPointer() -> IsOverflow())
        {
            std::cout << "CDataStore::Fsm 18"  << std::endl;
//                SetFsmEvent(WRITE_ERROR_FSM_EVENT);
            SetFsmState(WRITE_ERROR);
        }
        break;

    case WRITE_END_WAITING_BLOCK_DATA:
        // РћР¶РёРґР°РµРј РѕРєРѕРЅС‡Р°РЅРёСЏ Р·Р°РїРёСЃРё Р°РІС‚РѕРјР°С‚РѕРј СѓСЃС‚СЂРѕР№СЃС‚РІР° С…СЂР°РЅРµРЅРёСЏ.
        // РЈСЃС‚СЂРѕР№СЃС‚РІРѕ С…СЂР°РЅРµРЅРёСЏ Р·Р°РєРѕРЅС‡РёР»Рѕ Р·Р°РїРёСЃСЊ?
//        if (m_pxStorageDevice -> IsDataWrited())
        if (m_pxStorageDevice -> Fsm() ==
                CStorageDeviceInterface::DATA_WRITED_SUCCESSFULLY)
        {
            std::cout << "CDataStore::Fsm 19"  << std::endl;
            m_pxStorageDevice -> SetFsmState(CStorageDeviceInterface::READY);
            // Р—Р°РїРёСЃСЊ РїСЂРѕС€Р»Р° СѓСЃРїРµС€РЅРѕ?
            if (CheckBlock())
            {
                cerr << "CheckBlock ok" << endl;
                SetFsmState(START_WRITE_SERVICE_SECTION_DATA);
            }
            else
            {
                cerr << "CheckBlock error" << endl;
//                SetFsmEvent(WRITE_ERROR_FSM_EVENT);
                SetFsmState(WRITE_ERROR);
            }
        }
        // Р’СЂРµРјСЏ РѕР¶РёРґР°РЅРёСЏ РѕРєРѕРЅС‡Р°РЅРёСЏ Р·Р°РїРёСЃРё Р·Р°РєРѕРЅС‡РёР»РѕСЃСЊ?
        else if (GetTimerPointer() -> IsOverflow())
        {
            std::cout << "CDataStore::Fsm 20"  << std::endl;
            m_pxStorageDevice -> SetFsmState(CStorageDeviceInterface::READY);
//                SetFsmEvent(WRITE_ERROR_FSM_EVENT);
            SetFsmState(WRITE_ERROR);
        }
        break;


//-------------------------------------------------------------------------------
    // Р—Р°РїРёСЃСЊ СЃР»СѓР¶РµСЊРЅРѕРіРѕ Р±Р»РѕРєР°.
    case START_WRITE_SERVICE_SECTION_DATA:
//        SetFsmEvent(WRITE_IN_PROGRESS_FSM_EVENT);
        // РЈСЃС‚Р°РЅРѕРІРёРј РІСЂРµРјСЏ РѕР¶РёРґР°РЅРёСЏ РіРѕС‚РѕРІРЅРѕСЃС‚Рё Рє Р·Р°РїРёСЃРё.
        GetTimerPointer() -> Set(READY_TO_WRITE_WAITING_TIMEOUT);
        SetFsmState(READY_TO_WRITE_WAITING_SERVICE_SECTION_DATA);
        break;

    case READY_TO_WRITE_WAITING_SERVICE_SECTION_DATA:
        // РЈСЃС‚СЂРѕР№СЃС‚РІРѕ С…СЂР°РЅРµРЅРёСЏ РіРѕС‚РѕРІРѕ Рє Р·Р°РїРёСЃРё?
//        if (m_pxStorageDevice -> IsReadyToWrite())
        if (m_pxStorageDevice -> Fsm() == CStorageDeviceInterface::READY)
        {
            std::cout << "CDataStore::Fsm 21"  << std::endl;
            // РЈСЃС‚Р°РЅРѕРІРёРј РІСЂРµРјСЏ РѕР¶РёРґР°РЅРёСЏ РѕРєРѕРЅС‡Р°РЅРёСЏ Р·Р°РїРёСЃРё.
            GetTimerPointer() -> Set(WRITE_END_WAITING_TIMEOUT);
//            // РЎР±СЂРѕСЃРёРј С„Р»Р°Рі - Р·Р°РїРёСЃСЊ Р·Р°РєРѕРЅС‡РµРЅР°.
//            m_pxStorageDevice -> SetIsDataWrited(false);
            // РџРѕРґРіРѕС‚РѕРІРєР° Рє Р·Р°РїРёСЃРё РїСЂРѕС€Р»Р° СѓСЃРїРµС€РЅРѕ?
            if (ServiceSectionWritePrepare())
            {
                std::cout << "CDataStore::Fsm 22"  << std::endl;
                SetFsmState(WRITE_END_WAITING_SERVICE_SECTION_DATA);
            }
            else
            {
                std::cout << "CDataStore::Fsm 23"  << std::endl;
//                SetFsmEvent(WRITE_ERROR_FSM_EVENT);
                SetFsmState(WRITE_ERROR);
            }
        }
        // Р’СЂРµРјСЏ РѕР¶РёРґР°РЅРёСЏ РіРѕС‚РѕРІРЅРѕСЃС‚Рё Рє Р·Р°РїРёСЃРё Р·Р°РєРѕРЅС‡РёР»РѕСЃСЊ?
        else if (GetTimerPointer() -> IsOverflow())
        {
            std::cout << "CDataStore::Fsm 24"  << std::endl;
//                SetFsmEvent(WRITE_ERROR_FSM_EVENT);
            SetFsmState(WRITE_ERROR);
        }
        break;

    case WRITE_END_WAITING_SERVICE_SECTION_DATA:
        // РћР¶РёРґР°РµРј РѕРєРѕРЅС‡Р°РЅРёСЏ Р·Р°РїРёСЃРё Р°РІС‚РѕРјР°С‚РѕРј СѓСЃС‚СЂРѕР№СЃС‚РІР° С…СЂР°РЅРµРЅРёСЏ.
        // РЈСЃС‚СЂРѕР№СЃС‚РІРѕ С…СЂР°РЅРµРЅРёСЏ Р·Р°РєРѕРЅС‡РёР»Рѕ Р·Р°РїРёСЃСЊ?
//        if (m_pxStorageDevice -> IsDataWrited())
        if (m_pxStorageDevice -> Fsm() ==
                CStorageDeviceInterface::DATA_WRITED_SUCCESSFULLY)
        {
            std::cout << "CDataStore::Fsm 25"  << std::endl;
            m_pxStorageDevice -> SetFsmState(CStorageDeviceInterface::READY);
            // Р—Р°РїРёСЃСЊ РїСЂРѕС€Р»Р° СѓСЃРїРµС€РЅРѕ?
            if (ReadServiceSection())
            {
                std::cout << "CDataStore::Fsm 26"  << std::endl;
                cerr << "ReadServiceSection ok" << endl;
//                SetFsmEvent(WRITE_OK_FSM_EVENT);
                SetFsmState(DATA_WRITED_SUCCESSFULLY);
            }
            else
            {
                std::cout << "CDataStore::Fsm 27"  << std::endl;
                cerr << "ReadServiceSection error" << endl;
//                SetFsmEvent(WRITE_ERROR_FSM_EVENT);
                SetFsmState(WRITE_ERROR);
            }
        }
        // Р’СЂРµРјСЏ РѕР¶РёРґР°РЅРёСЏ РѕРєРѕРЅС‡Р°РЅРёСЏ Р·Р°РїРёСЃРё Р·Р°РєРѕРЅС‡РёР»РѕСЃСЊ?
        else if (GetTimerPointer() -> IsOverflow())
        {
            std::cout << "CDataStore::Fsm 28"  << std::endl;
            m_pxStorageDevice -> SetFsmState(CStorageDeviceInterface::READY);
//                SetFsmEvent(WRITE_ERROR_FSM_EVENT);
            SetFsmState(WRITE_ERROR);
        }
        break;

    case DATA_WRITED_SUCCESSFULLY:
        break;

    case WRITE_ERROR:
        break;

    default:
//                SetFsmEvent(WRITE_ERROR_FSM_EVENT);
//        SetFsmState(WRITE_ERROR);
        break;
    }

    return GetFsmState();
}

//-------------------------------------------------------------------------------












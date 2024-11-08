﻿
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


// (sizeof(struct TAnalogueInputDescriptionDataBase) * ANALOG_MODULE_INPUT_QUANTITY)// 28х6=168.
#define ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_LENGTH 168
// (sizeof(struct TMrXXOneChannelDataBase) * DISCRETE_OUTPUT_MODULE_RELAY_OUTPUT_QUANTITY)// 3х64=192.
#define DISCRETE_OUTPUT_MODULE_DATA_BASE_BLOCK_LENGTH 192
// (sizeof(struct TMbmFunction1234PackOne) * EXTERNAL_MODULE_FUNCTION_1234_QUANTITY) + 1// 13х16=208.
#define MBM_FUNCTION_1234_BLOCK_DATA_BASE_BLOCK_LENGTH (208 + 1)
// (sizeof(struct TMbmFunction15Sourse) * EXTERNAL_MODULE_FUNCTION_15_QUANTITY)// 72х3=216.
#define MBM_FUNCTION_15_BLOCK_DATA_BASE_BLOCK_LENGTH 216
// (sizeof(struct TDiscreteInputDescriptionWork) * DISCRETE_INPUT_SYGNALS_DATA_BASE_BLOCKS_IN_BLOCK_QUANTITY)// 11х18=198.
#define DISCRETE_INPUT_SYGNALS_DATA_BASE_BLOCK_LENGTH 192
// (sizeof(struct TAnalogueOutputRegulatorDescriptionDataBase) * ANALOG_OUTPUT_MODULE_REGULATORS_QUANTITY)// 28х6=168.
#define CURRENT_OUTPUT_MODULE_REGULATOR_DATA_BASE_BLOCK_LENGTH 56
#define CURRENT_OUTPUT_MODULE_PSP_DATA_BASE_BLOCK_LENGTH 56
// DIMENSIONS_PARAMETERS_DATA_BLOCKS_IN_BLOCK_QUANTITY * 6// 6х16=96.
#define DIMENSIONS_PARAMETERS_DATA_BASE_BLOCK_LENGTH 96
// TEXT_TITLES_DATA_BASE_BLOCKS_IN_BLOCK_QUANTITY * TEXT_TITLES_DATA_LENGTH// (384 ТР, 24 блока).
#define TEXT_TITLES_DATA_BASE_BLOCK_LENGTH 224
// (sizeof(struct TFunctionBlockDescriptionWork) * FUNCTION_BLOCK_DATA_BASE_BLOCKS_IN_BLOCK_QUANTITY)// 19х10=190.
#define FUNCTION_BLOCK_DATA_BASE_BLOCK_LENGTH 160// 19х10=190 (120 ФБЛ, 12 блоков)
#define MATHEMATICAL_BLOCK_DATA_BASE_BLOCK_LENGTH 208// 82 – 84 (48 ФБМ, 3 блока 13*16=208 )
#define DISCRETE_INPUT_MODULE_DATA_BASE_BLOCK_LENGTH 32// 2х16=32.
#define NETWORK_ADDRESS_DATA_BASE_BLOCK_LENGTH 1
#define RESERVED_DATA_BASE_BLOCK_LENGTH 1
#define REFERENCE_POINTS_ADC_CODES_DATA_BASE_BLOCK_LENGTH 42

// массив содержит размеры блоков базы данных.
// массив базы данных состоит из 100 блоков по 256 байт.
// когда программатор запрашивает нужный ему блок, (в функции - modbus_reply() - case _FC_DATA_BASE_READ:),
// из этого массива, по номеру, программатору выдаётся размер блока.
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
//    GetArgumentDataPointer() -> m_uiDataPointer = puiDataPointer;
//    GetArgumentDataPointer() -> m_uiDataOffset = uiDataOffset;
//    GetArgumentDataPointer() -> m_uiDataLength = uiDataLength;
}

//-------------------------------------------------------------------------------
void CDataStore::GetArgumentData(void)
{
//    m_puiBuffer = m_pxTaskCustomer -> GetArgumentDataPointer() -> m_uiDataPointer;
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

    // Очистим служебный контекст.
    memset(reinterpret_cast<uint8_t*>(&m_xServiseSection.xServiseSectionData),
           0,
           sizeof(struct TServiseSectionData));

    // Установим размер первичных данных.
    m_xServiseSection.xServiseSectionData.uiLength = sizeof(struct TServiseSectionData);
    // Количество байт служебного контекста
    // должно быть известно заранее.
    // Так как по нему извлекаются начальные данные.
    // Используется самовосстанавливающийся код Хемминга(8,4).
    // Коэффициент - 1.5: один байт преобразуется в кодовое слово 12 бит,
    // из двух байт полезных данных получается три байта кодированных.
    // Установим размер закодированных данных.
    m_xServiseSection.xServiseSectionData.uiEncodedLength =
        (CHammingCodes::CalculateEncodedDataLength(sizeof(struct TServiseSection)));
    // Установим смещение на начало данных блоков.
    m_xServiseSection.xServiseSectionData.uiFreeSpaceOffset =
        (SERVICE_SECTION_DATA_BEGIN +
         m_xServiseSection.xServiseSectionData.uiEncodedLength);
    // Хранилище пусто.
    m_xServiseSection.xServiseSectionData.uiStoredBlocksNumber = 0;
    // Установим признак - хранилище не подписано.
    m_xServiseSection.xServiseSectionData.uiCrcOfBlocksCrc = 0;
}

//-------------------------------------------------------------------------------
// Готовит данные временного служебного блока к записи.
// Кодирует по алгоритму Хемминга и
// передаёт автомату записи устройства хранения.
uint8_t CDataStore::TemporaryServiceSectionWritePrepare(void)
{
    std::cout << "CDataStore::TemporaryServiceSectionWritePrepare 1"  << std::endl;
    // Вычислим контрольную сумму.
    m_xServiseSection.uiCrc =
        usCrc16(reinterpret_cast<uint8_t*>(&m_xServiseSection.xServiseSectionData),
                sizeof(struct TServiseSectionData));

    std::cout << "CDataStore::TemporaryServiceSectionWritePrepare 2"  << std::endl;
    // Звкодируем данные алгоритмом Хемминга.
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
// Готовит данные служебного блока к записи.
// Кодирует по алгоритму Хемминга и
// передаёт автомату записи устройства хранения.
uint8_t CDataStore::ServiceSectionWritePrepare(void)
{
    // Вычислим контрольную сумму.
    m_xServiseSection.uiCrc =
        usCrc16(reinterpret_cast<uint8_t*>(&m_xServiseSection.xServiseSectionData),
                sizeof(struct TServiseSectionData));

    // Звкодируем данные алгоритмом Хемминга.
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
// Готовит данные блока к записи во временный буфер.
// Кодирует по алгоритму Хемминга, размечает место для хранения и
// передаёт автомату записи устройства хранения.
uint8_t CDataStore::TemporaryBlockWritePrepare(void)
{
    std::cout << "CDataStore::TemporaryBlockWritePrepare 1"  << std::endl;
    uint8_t uiBlock = m_uiBlockIndex;
    uint8_t* puiSource = m_puiBlockDataPointer;
    uint16_t uiLength = m_uiBlockLength;

    // Звкодируем данные алгоритмом Хемминга.
    uint16_t uiEncodedByteCounter =
        CHammingCodes::BytesToHammingCodes(m_puiIntermediateBuff, puiSource, uiLength);

    // Данные текущего блока данных - uiBlock сохраняются впервые?
    if ((m_xServiseSection.xServiseSectionData.
            axBlockPositionData[uiBlock].uiLength == 0) &&
            (m_xServiseSection.xServiseSectionData.
             axBlockPositionData[uiBlock].uiEncodedLength == 0))
    {
        std::cout << "CDataStore::TemporaryBlockWritePrepare 2"  << std::endl;
        // Добавили новый блок данных.
        // Сохраним смещение на блок.
        m_xServiseSection.xServiseSectionData.
        axBlockPositionData[uiBlock].uiOffset = m_xServiseSection.xServiseSectionData.uiFreeSpaceOffset;
        // Изменим смещение на свободное место.
        m_xServiseSection.xServiseSectionData.uiFreeSpaceOffset += uiEncodedByteCounter;
        // Сохраним размер первичных данных.
        m_xServiseSection.xServiseSectionData.
        axBlockPositionData[uiBlock].uiLength = uiLength;
        // Сохраним размер закодированных данных.
        m_xServiseSection.xServiseSectionData.
        axBlockPositionData[uiBlock].uiEncodedLength = uiEncodedByteCounter;
        // Увеличим количество блоков находящихся в хранилище.
        m_xServiseSection.xServiseSectionData.uiStoredBlocksNumber += 1;
    }

    // Вычислим контрольную сумму поступивших данных.
    m_xServiseSection.xServiseSectionData.
    axBlockPositionData[uiBlock].uiCrc =
        usCrc16(puiSource, uiLength);

//    SetArgumentData(m_puiIntermediateBuff,
//                    TEMPORARY_BLOCK_DATA_BEGIN,
//                    uiEncodedByteCounter);

    CDataContainerDataBase* pxDataContainer =
        (static_cast<CDataContainerDataBase*>(m_pxStorageDevice ->
                m_pxDataContainer.get()));

    pxDataContainer ->
    SetContainerData(CStorageDeviceInterface::WRITE_DATA_START,
                     uiBlock,
                     m_puiIntermediateBuff,
                     TEMPORARY_BLOCK_DATA_BEGIN,
                     uiEncodedByteCounter);
    m_pxStorageDevice ->
    SetFsmCommandState(CStorageDeviceInterface::WRITE_DATA_START);

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
// Готовит данные блока к записи.
// Кодирует по алгоритму Хемминга, размечает место для хранения и
// передаёт автомату записи устройства хранения.
uint8_t CDataStore::BlockWritePrepare(void)
{
    uint8_t uiBlock = m_uiBlockIndex;
    uint8_t* puiSource = m_puiBlockDataPointer;
    uint16_t uiLength = m_uiBlockLength;

    // Звкодируем данные алгоритмом Хемминга.
    uint16_t uiEncodedByteCounter =
        CHammingCodes::BytesToHammingCodes(m_puiIntermediateBuff, puiSource, uiLength);

    // Данные текущего блока данных - uiBlock сохраняются впервые?
    if ((m_xServiseSection.xServiseSectionData.
            axBlockPositionData[uiBlock].uiLength == 0) &&
            (m_xServiseSection.xServiseSectionData.
             axBlockPositionData[uiBlock].uiEncodedLength == 0))
    {
        // Добавили новый блок данных.
        // Сохраним смещение на блок.
        m_xServiseSection.xServiseSectionData.
        axBlockPositionData[uiBlock].uiOffset = m_xServiseSection.xServiseSectionData.uiFreeSpaceOffset;
        // Изменим смещение на свободное место.
        m_xServiseSection.xServiseSectionData.uiFreeSpaceOffset += uiEncodedByteCounter;
        // Сохраним размер первичных данных.
        m_xServiseSection.xServiseSectionData.
        axBlockPositionData[uiBlock].uiLength = uiLength;
        // Сохраним размер закодированных данных.
        m_xServiseSection.xServiseSectionData.
        axBlockPositionData[uiBlock].uiEncodedLength = uiEncodedByteCounter;
//        // Увеличим количество блоков находящихся в хранилище.
//        m_xServiseSection.xServiseSectionData.uiStoredBlocksNumber += 1;
    }

    // Вычислим контрольную сумму поступивших данных.
    m_xServiseSection.xServiseSectionData.
    axBlockPositionData[uiBlock].uiCrc =
        usCrc16(puiSource, uiLength);
//    // Сохраним индекс последнего записываемого блока.
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
// Запись блока в хранилище проходит поэтапно.
// 1 - CDataStore::WriteBlock(uint8_t *puiSource, uint16_t uiLength, uint8_t uiBlock).
// 2 - CDataStore::BlockWritePrepare(void).
// 3 - CStorageDevice::WriteBlock(uint16_t uiOffset, uint8_t *puiSource, uint16_t uiLength).

// Передаёт данные контекста записи блока автомату хранилища данных и запускает процесс записи.
uint8_t CDataStore::WriteBlock(uint8_t *puiSource, uint16_t uiLength, uint8_t uiBlock)
{
    std::cout << "CDataStore::WriteBlock 1"  << std::endl;
    // Произошёл выход за границы буфера?
    if (uiBlock >= (MAX_BLOCKS_NUMBER + SERVICE_SECTION_BLOCK_NUMBER))
    {
        // Нет данных.
        return 0;
    }

    // Получим данные контекста записи блока.
    m_puiBlockDataPointer = puiSource;
    m_uiBlockLength = uiLength;
    m_uiBlockIndex = uiBlock;
    return 1;

//    // Произошёл выход за границы буфера?
//    if (uiBlock >= (MAX_BLOCKS_NUMBER + SERVICE_SECTION_BLOCK_NUMBER))
//    {
//        // Нет данных.
//        return 0;
//    }
//
//
//    // блок не служебный?
//    if (uiBlock < MAX_BLOCKS_NUMBER)
//    {
//        // Звкодируем данные алгоритмом Хемминга.
//        m_uiBlockEncodedLength =
//            CHammingCodes::BytesToHammingCodes(m_puiIntermediateBuff,
//                                               puiSource,
//                                               uiLength);
//        // Блок существует?
//        if ((m_xServiseSection.xServiseSectionData.
//                axBlockPositionData[uiBlock].uiLength != 0) &&
//                (m_xServiseSection.xServiseSectionData.
//                 axBlockPositionData[uiBlock].uiEncodedLength != 0))
//        {
//            // Добавили новый блок данных.
//            // Сохраним смещение на блок.
//            m_xServiseSection.xServiseSectionData.
//            axBlockPositionData[uiBlock].uiOffset = m_xServiseSection.xServiseSectionData.uiFreeSpaceOffset;
//            // Изменим смещение на свободное место.
//            m_xServiseSection.xServiseSectionData.uiFreeSpaceOffset += m_uiBlockEncodedLength;
//            // Сохраним размер первичных данных.
//            m_xServiseSection.xServiseSectionData.
//            axBlockPositionData[uiBlock].uiLength = uiLength;
//            // Сохраним размер закодированных данных.
//            m_xServiseSection.xServiseSectionData.
//            axBlockPositionData[uiBlock].uiEncodedLength = m_uiBlockEncodedLength;
//
//            // Получим индекс блока.
//            m_uiBlockIndex = uiBlock;
//            // Получим указатель на данные блока.
//            m_puiBlockDataPointer = puiSource;
//            m_uiBlockOffset = m_xServiseSection.xServiseSectionData.
//                              axBlockPositionData[uiBlock].uiOffset;
//            // Получим размер блока.
//            m_uiBlockLength = uiLength;
//
//            // Вычислим контрольную сумму поступивших данных.
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
//            // Нет данных.
//            return 0;
//        }
//    }
//    else
//    {
//        switch (uiBlock)
//        {
//        case TEMPORARY_BLOCK_INDEX:
//            // Звкодируем данные алгоритмом Хемминга.
//            m_uiBlockEncodedLength =
//                CHammingCodes::BytesToHammingCodes(m_puiIntermediateBuff,
//                                                   puiSource,
//                                                   uiLength);
//
//            // Данные текущего блока данных - uiBlock сохраняются впервые?
//            if ((m_xServiseSection.xServiseSectionData.
//                    axBlockPositionData[uiBlock].uiLength == 0) &&
//                    (m_xServiseSection.xServiseSectionData.
//                     axBlockPositionData[uiBlock].uiEncodedLength == 0))
//            {
//                std::cout << "CDataStore::WriteBlock 2"  << std::endl;
//                // Добавили новый блок данных.
//                // Сохраним смещение на блок.
//                m_xServiseSection.xServiseSectionData.
//                axBlockPositionData[uiBlock].uiOffset = m_xServiseSection.xServiseSectionData.uiFreeSpaceOffset;
//                // Изменим смещение на свободное место.
//                m_xServiseSection.xServiseSectionData.uiFreeSpaceOffset += m_uiBlockEncodedLength;
//                // Сохраним размер первичных данных.
//                m_xServiseSection.xServiseSectionData.
//                axBlockPositionData[uiBlock].uiLength = uiLength;
//                // Сохраним размер закодированных данных.
//                m_xServiseSection.xServiseSectionData.
//                axBlockPositionData[uiBlock].uiEncodedLength = m_uiBlockEncodedLength;
//                // Увеличим количество блоков находящихся в хранилище.
//                m_xServiseSection.xServiseSectionData.uiStoredBlocksNumber += 1;
//            }
//
//            // Получим индекс блока.
//            m_uiBlockIndex = uiBlock;
//            // Получим указатель на данные блока.
//            m_puiBlockDataPointer = puiSource;
//            // Получим адрес блока в EEPROM.
//            m_uiBlockOffset = TEMPORARY_BLOCK_DATA_BEGIN;
//            // Получим размер блока.
//            m_uiBlockLength = uiLength;
//
//            // Вычислим контрольную сумму поступивших данных.
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
//            // Получим индекс блока.
//            m_uiBlockIndex = uiBlock;
//            // Получим указатель на данные блока.
//            m_puiBlockDataPointer =
//                (reinterpret_cast<uint8_t*>(&m_xServiseSection.xServiseSectionData));
//            // Получим адрес блока в EEPROM.
//            m_uiBlockOffset = TEMPORARY_SERVICE_SECTION_DATA_BEGIN;
//            // Получим размер блока.
//            m_uiBlockLength = sizeof(struct TServiseSectionData);
//
//            // Вычислим контрольную сумму поступивших данных.
//            m_xServiseSection.uiCrc =
//                usCrc16(m_puiBlockDataPointer, m_uiBlockLength);
//
//            // Звкодируем данные алгоритмом Хемминга.
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
//            // Получим индекс блока.
//            m_uiBlockIndex = uiBlock;
//            // Получим указатель на данные блока.
//            m_puiBlockDataPointer =
//                (reinterpret_cast<uint8_t*>(&m_xServiseSection.xServiseSectionData));
//            // Получим адрес блока в EEPROM.
//            m_uiBlockOffset = SERVICE_SECTION_DATA_BEGIN;
//            // Получим размер блока.
//            m_uiBlockLength = sizeof(struct TServiseSectionData);
//
//            // Вычислим контрольную сумму поступивших данных.
//            m_xServiseSection.uiCrc =
//                usCrc16(m_puiBlockDataPointer, m_uiBlockLength);
//
//            // Звкодируем данные алгоритмом Хемминга.
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
//            // Нет данных.
//            return 0;
//            break;
//        }
//    }
}

//-------------------------------------------------------------------------------
// Считывает и проверяет целостность временного служебного блока.
uint8_t CDataStore::ReadTemporaryServiceSection(void)
{
    uint16_t uiEncodedLength =
        (CHammingCodes::CalculateEncodedDataLength(sizeof(struct TServiseSection)));
    std::cout << "CDataStore::ReadTemporaryServiceSection 1 " << (int)uiEncodedLength  << std::endl;
    // Прочитаем закодированные данные.
    // При чтении данных возникла ошибка?
    if (!(m_pxStorageDevice -> ReadBlock(m_puiIntermediateBuff,
                                         TEMPORARY_SERVICE_SECTION_DATA_BEGIN,
                                         uiEncodedLength)))
    {
        std::cout << "CDataStore::ReadTemporaryServiceSection 2"  << std::endl;
        // Нет данных.
        return 0;
    }

    // Декодируем прочитанные данные.
    CHammingCodes::HammingCodesToBytes(reinterpret_cast<uint8_t*>(&m_xServiseSection),
                                       m_puiIntermediateBuff,
                                       uiEncodedLength);

    // Блок не повреждён?
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
        // Нет данных.
        return 0;
    }
}

//-------------------------------------------------------------------------------
// Считывает и проверяет целостность служебного блока.
uint8_t CDataStore::ReadServiceSection(void)
{
    uint16_t uiEncodedLength =
        (CHammingCodes::CalculateEncodedDataLength(sizeof(struct TServiseSection)));

    // Прочитаем закодированные данные.
    // При чтении данных возникла ошибка?
    if (!(m_pxStorageDevice -> ReadBlock(m_puiIntermediateBuff,
                                         SERVICE_SECTION_DATA_BEGIN,
                                         uiEncodedLength)))
    {
        // Нет данных.
        return 0;
    }

    // Декодируем прочитанные данные.
    CHammingCodes::HammingCodesToBytes(reinterpret_cast<uint8_t*>(&m_xServiseSection),
                                       m_puiIntermediateBuff,
                                       uiEncodedLength);

    // Блок не повреждён?
    if (m_xServiseSection.uiCrc ==
            usCrc16(reinterpret_cast<uint8_t*>(&m_xServiseSection.xServiseSectionData),
                    sizeof(struct TServiseSectionData)))
    {
        return 1;
    }
    else
    {
        // Нет данных.
        return 0;
    }
}

//-------------------------------------------------------------------------------
// Считывает и проверяет целостность временного буфера блока.
uint8_t CDataStore::CheckTemporaryBlock(void)
{
    uint8_t uiBlock = m_uiBlockIndex;

    // Произошёл выход за границы буфера?
    if (uiBlock >= MAX_BLOCKS_NUMBER)
    {
        // Нет данных.
        return 0;
    }

    uint16_t uiLength;
    uint16_t uiEncodedLength;
    uint16_t uiSourceOffset;

    // Блок существует?
    if ((m_xServiseSection.xServiseSectionData.
            axBlockPositionData[uiBlock].uiLength != 0) &&
            (m_xServiseSection.xServiseSectionData.
             axBlockPositionData[uiBlock].uiEncodedLength != 0))
    {
        // Получим адрес блока в EEPROM.
        uiSourceOffset = m_xServiseSection.xServiseSectionData.
                         axBlockPositionData[uiBlock].uiOffset;
        // Получим размер блока.
        uiLength = m_xServiseSection.xServiseSectionData.
                   axBlockPositionData[uiBlock].uiLength;
        // Получим размер закодированного блока.
        uiEncodedLength = m_xServiseSection.xServiseSectionData.
                          axBlockPositionData[uiBlock].uiEncodedLength;
    }
    else
    {
        // Нет данных.
        return 0;
    }

    // Прочитаем закодированные данные.
    // При чтении данных возникла ошибка?
    if (!(m_pxStorageDevice -> ReadBlock(m_puiIntermediateBuff,
                                         TEMPORARY_BLOCK_DATA_BEGIN,
                                         uiEncodedLength)))
    {
        // Нет данных.
        return 0;
    }

    // Декодируем прочитанные данные.
    CHammingCodes::HammingCodesToBytes(m_puiIntermediateBuff,
                                       m_puiIntermediateBuff,
                                       uiEncodedLength);

    // Блок не повреждён?
    if (m_xServiseSection.xServiseSectionData.
            axBlockPositionData[uiBlock].uiCrc ==
            usCrc16(m_puiIntermediateBuff, uiLength))
    {
        return 1;
    }
    else
    {
        // Нет данных.
        return 0;
    }
}

//-------------------------------------------------------------------------------
// Считывает и проверяет целостность блока.
uint8_t CDataStore::CheckBlock(void)
{
    uint8_t uiBlock = m_uiBlockIndex;

    // Произошёл выход за границы буфера?
    if (uiBlock >= MAX_BLOCKS_NUMBER)
    {
        // Нет данных.
        return 0;
    }

    uint16_t uiLength;
    uint16_t uiEncodedLength;
    uint16_t uiSourceOffset;

    // Блок существует?
    if ((m_xServiseSection.xServiseSectionData.
            axBlockPositionData[uiBlock].uiLength != 0) &&
            (m_xServiseSection.xServiseSectionData.
             axBlockPositionData[uiBlock].uiEncodedLength != 0))
    {
        // Получим адрес блока в EEPROM.
        uiSourceOffset = m_xServiseSection.xServiseSectionData.
                         axBlockPositionData[uiBlock].uiOffset;
        // Получим размер блока.
        uiLength = m_xServiseSection.xServiseSectionData.
                   axBlockPositionData[uiBlock].uiLength;
        // Получим размер закодированного блока.
        uiEncodedLength = m_xServiseSection.xServiseSectionData.
                          axBlockPositionData[uiBlock].uiEncodedLength;
    }
    else
    {
        // Нет данных.
        return 0;
    }

    // Прочитаем закодированные данные.
    // При чтении данных возникла ошибка?
    if (!(m_pxStorageDevice -> ReadBlock(m_puiIntermediateBuff,
                                         uiSourceOffset,
                                         uiEncodedLength)))
    {
        // Нет данных.
        return 0;
    }

    // Декодируем прочитанные данные.
    CHammingCodes::HammingCodesToBytes(m_puiIntermediateBuff,
                                       m_puiIntermediateBuff,
                                       uiEncodedLength);

    // Блок не повреждён?
    if (m_xServiseSection.xServiseSectionData.
            axBlockPositionData[uiBlock].uiCrc ==
            usCrc16(m_puiIntermediateBuff, uiLength))
    {
        return 1;
    }
    else
    {
        // Нет данных.
        return 0;
    }
}

//-------------------------------------------------------------------------------
// Считывает и проверяет целостность блока.
uint16_t CDataStore::ReadBlock(uint8_t *puiDestination, uint8_t uiBlock)
{
    // Произошёл выход за границы буфера?
    if (uiBlock >= (MAX_BLOCKS_NUMBER + SERVICE_SECTION_BLOCK_NUMBER))
    {
        // Нет данных.
        return 0;
    }
    // блок не служебный?
    if (uiBlock < MAX_BLOCKS_NUMBER)
    {
        // Блок существует?
        if ((m_xServiseSection.xServiseSectionData.
                axBlockPositionData[uiBlock].uiLength != 0) &&
                (m_xServiseSection.xServiseSectionData.
                 axBlockPositionData[uiBlock].uiEncodedLength != 0))
        {
            // Получим индекс блока.
            m_uiBlockIndex = uiBlock;
            // Получим указатель на данные блока.
            m_puiBlockDataPointer = puiDestination;
            // Получим адрес блока в EEPROM.
            m_uiBlockOffset = m_xServiseSection.xServiseSectionData.
                              axBlockPositionData[uiBlock].uiOffset;
            // Получим размер блока.
            m_uiBlockLength = m_xServiseSection.xServiseSectionData.
                              axBlockPositionData[uiBlock].uiLength;
            // Получим размер закодированного блока.
            m_uiBlockEncodedLength = m_xServiseSection.xServiseSectionData.
                                     axBlockPositionData[uiBlock].uiEncodedLength;

            SetArgumentData(m_puiIntermediateBuff,
                            m_uiBlockOffset,
                            m_uiBlockEncodedLength);
            return 1;
        }
        else
        {
            // Нет данных.
            return 0;
        }
    }
    else
    {
        // Получим указатель на данные блока.
        m_puiBlockDataPointer = puiDestination;
        // Получим размер закодированного блока.
        m_uiBlockEncodedLength =
            (CHammingCodes::CalculateEncodedDataLength(sizeof(struct TServiseSection)));

        switch (uiBlock)
        {
        case TEMPORARY_BLOCK_INDEX:
            // Получим адрес блока в EEPROM.
            m_uiBlockOffset = TEMPORARY_BLOCK_DATA_BEGIN;
            // Получим размер блока.
            m_uiBlockLength = m_xServiseSection.xServiseSectionData.
                              axBlockPositionData[m_uiBlockIndex].uiLength;
            break;

        case TEMPORARY_SERVICE_SECTION_INDEX:
            // Получим адрес блока в EEPROM.
            m_uiBlockOffset = TEMPORARY_SERVICE_SECTION_DATA_BEGIN;
            // Получим размер блока.
            m_uiBlockLength = sizeof(struct TServiseSectionData);
            break;

        case SERVICE_SECTION_INDEX:
            // Получим адрес блока в EEPROM.
            m_uiBlockOffset = SERVICE_SECTION_DATA_BEGIN;
            // Получим размер блока.
            m_uiBlockLength = sizeof(struct TServiseSectionData);
            break;

        default:
            // Нет данных.
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

//    // Блок существует?
//    if ((m_xServiseSection.xServiseSectionData.
//            axBlockPositionData[uiBlock].uiLength != 0) &&
//            (m_xServiseSection.xServiseSectionData.
//             axBlockPositionData[uiBlock].uiEncodedLength != 0))
//    {
//        // Получим адрес блока в EEPROM.
//        uiSourceOffset = m_xServiseSection.xServiseSectionData.
//                         axBlockPositionData[uiBlock].uiOffset;
//        // Получим размер блока.
//        uiLength = m_xServiseSection.xServiseSectionData.
//                   axBlockPositionData[uiBlock].uiLength;
//        // Получим размер закодированного блока.
//        uiEncodedLength = m_xServiseSection.xServiseSectionData.
//                          axBlockPositionData[uiBlock].uiEncodedLength;

//        // Получим индекс блока.
//        m_uiBlockIndex = uiBlock;
//        // Получим указатель на данные блока.
//        m_puiBlockDataPointer = puiDestination;
//        // Получим адрес блока в EEPROM.
//        m_uiBlockOffset = m_xServiseSection.xServiseSectionData.
//                          axBlockPositionData[uiBlock].uiOffset;
//        // Получим размер блока.
//        m_uiBlockLength = m_xServiseSection.xServiseSectionData.
//                          axBlockPositionData[uiBlock].uiLength;
//        // Получим размер закодированного блока.
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
//        // Нет данных.
//        return 0;
//    }

//    // Прочитаем закодированные данные.
//    m_pxStorageDevice -> ReadBlock(m_puiIntermediateBuff,
//                                   uiSourceOffset,
//                                   uiEncodedLength);
//
//    // Декодируем прочитанные данные.
//    CHammingCodes::HammingCodesToBytes(m_puiIntermediateBuff,
//                                       m_puiIntermediateBuff,
//                                       uiEncodedLength);
//
//    // Блок не повреждён?
//    if (m_xServiseSection.xServiseSectionData.
//            axBlockPositionData[uiBlock].uiCrc ==
//            usCrc16(m_puiIntermediateBuff, uiLength))
//    {
//        memcpy(puiDestination, m_puiIntermediateBuff, uiLength);
//        return uiLength;
//    }
//    else
//    {
//        // Нет данных.
//        return 0;
//    }
}

//-------------------------------------------------------------------------------
// Считывает и проверяет целостность блока.
uint16_t CDataStore::GetReadedData()
{
    uint8_t auiTempArray[CDataStore::MAX_SERVICE_SECTION_DATA_LENGTH];

    // Декодируем прочитанные данные.
    CHammingCodes::HammingCodesToBytes(auiTempArray,
                                       m_puiIntermediateBuff,
                                       m_uiBlockEncodedLength);

    // Блок не повреждён?
    if (m_xServiseSection.xServiseSectionData.
            axBlockPositionData[m_uiBlockIndex].uiCrc ==
            usCrc16(auiTempArray, m_uiBlockLength))
    {
        memcpy(m_puiBlockDataPointer, auiTempArray, m_uiBlockLength);
        return m_uiBlockLength;
    }
    else
    {
        // Нет данных.
        return 0;
    }
}

//-------------------------------------------------------------------------------
// Вызывается только если база данных подтверждена пользователем.
void CDataStore::CrcOfBlocksCrcCreate(void)
{
    // Контрольная сумма вычисленная из массива контрольных сумм блоков, не включая служебный.
    // Сохраняется при первой и последующих записях любых блоков через программатор.
    // Ноль или её несовпадение свидетельствует о том, что база данных создана по умоланию,
    // и не подтверждена пользователем. В этом случае прибор переходит в режим сигнализации об ошибке,
    // ожидая квитирования или записи базы данных.
    uint16_t auiBlocksCrc[MAX_BLOCKS_NUMBER];

    // Получим Crc всех блоков.
    for (uint16_t i = 0;
            i < MAX_BLOCKS_NUMBER;
            i++)
    {
        auiBlocksCrc[i] =
            m_xServiseSection.xServiseSectionData.axBlockPositionData[i].uiCrc;
    }

    // Сохраним Crc всех блоков.
    m_xServiseSection.xServiseSectionData.uiCrcOfBlocksCrc =
        usCrc16(reinterpret_cast<uint8_t*>(auiBlocksCrc),
                (MAX_BLOCKS_NUMBER * sizeof(uint16_t)));

}

//-------------------------------------------------------------------------------
bool CDataStore::CrcOfBlocksCrcCheck(void)
{
    // Контрольная сумма вычисленная из массива контрольных сумм блоков, не включая служебный.
    // Сохраняется при первой и последующих записях любых блоков через программатор.
    // Ноль или её несовпадение свидетельствует о том, что база данных создана по умоланию,
    // и не подтверждена пользователем. В этом случае прибор переходит в режим сигнализации об ошибке,
    // ожидая квитирования или записи базы данных.
    uint16_t auiBlocksCrc[MAX_BLOCKS_NUMBER];

    // Получим Crc всех блоков.
    for (uint16_t i = 0;
            i < MAX_BLOCKS_NUMBER;
            i++)
    {
        auiBlocksCrc[i] =
            m_xServiseSection.xServiseSectionData.axBlockPositionData[i].uiCrc;
    }

    // Crc из Crc всех блоков совпадает с сохранённым в служебном блоке?
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
// Главный автомат записи хранилища.
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
    break;

    case READY:
//        std::cout << "CDataStore::Fsm READY"  << std::endl;
    {
        if (GetFsmCommandState() != 0)
        {
            SetFsmState(GetFsmCommandState());
            SetFsmCommandState(0);
        }
    }

    break;

//-------------------------------------------------------------------------------
    // Запись блока во временный буфер.
    case START_WRITE_TEMPORARY_BLOCK_DATA:
        std::cout << "CDataStore::Fsm 1"  << std::endl;
        TemporaryBlockWritePrepare();
        // Установим время ожидания окончания записи.
        GetTimerPointer() -> Set(WRITE_END_WAITING_TIMEOUT);
        SetFsmState(WRITE_END_WAITING_TEMPORARY_BLOCK_DATA);
        break;

    case WRITE_END_WAITING_TEMPORARY_BLOCK_DATA:
        std::cout << "CDataStore::Fsm 5"  << std::endl;
        // Ожидаем окончания записи автоматом устройства хранения.
        // Устройство хранения закончило запись успешно?
        if (m_pxStorageDevice -> GetFsmAnswerState() ==
                CStorageDeviceInterface::DATA_WRITED_SUCCESSFULLY)
        {
            std::cout << "CDataStore::Fsm 6"  << std::endl;
            // Запись прошла успешно?
            if (CheckTemporaryBlock())
            {
                cerr << "CheckTemporaryBlock ok" << endl;
                SetFsmState(START_WRITE_TEMPORARY_SERVICE_SECTION_DATA);
            }
            else
            {
                cerr << "CheckTemporaryBlock error" << endl;
//                SetFsmEvent(WRITE_ERROR_FSM_EVENT);
                SetFsmState(WRITE_ERROR);
            }
        }
        // Устройство хранения закончило запись не успешно?
        else if (m_pxStorageDevice -> GetFsmAnswerState() ==
                 CStorageDeviceInterface::WRITE_DATA_ERROR)
        {
            std::cout << "CDataStore::Fsm 7"  << std::endl;
            SetFsmState(WRITE_ERROR);
        }
        else
        {
            // Время ожидания окончания записи закончилось?
            if (GetTimerPointer() -> IsOverflow())
            {
                std::cout << "CDataStore::Fsm 7"  << std::endl;
                SetFsmState(WRITE_ERROR);
            }
        }
        break;


//-------------------------------------------------------------------------------
    // Запись временного служеьного блока.
    case START_WRITE_TEMPORARY_SERVICE_SECTION_DATA:
        std::cout << "CDataStore::Fsm 8"  << std::endl;
        TemporaryServiceSectionWritePrepare();
        // Установим время ожидания окончания записи.
        GetTimerPointer() -> Set(WRITE_END_WAITING_TIMEOUT);
        SetFsmState(WRITE_END_WAITING_TEMPORARY_SERVICE_SECTION_DATA);
        break;

    case WRITE_END_WAITING_TEMPORARY_SERVICE_SECTION_DATA:
        // Ожидаем окончания записи автоматом устройства хранения.
        // Устройство хранения закончило запись успешно?
        if (m_pxStorageDevice -> GetFsmAnswerState() ==
                CStorageDeviceInterface::DATA_WRITED_SUCCESSFULLY)
        {
            // Запись прошла успешно?
            if (ReadTemporaryServiceSection())
            {
                SetFsmState(START_WRITE_BLOCK_DATA);
            }
            else
            {
                SetFsmState(WRITE_ERROR);
            }
        }
        // Устройство хранения закончило запись не успешно?
        else if (m_pxStorageDevice -> GetFsmAnswerState() ==
                 CStorageDeviceInterface::WRITE_DATA_ERROR)
        {
            SetFsmState(WRITE_ERROR);
        }
        else
        {
            // Время ожидания окончания записи закончилось?
            if (GetTimerPointer() -> IsOverflow())
            {
                SetFsmState(WRITE_ERROR);
            }
        }
        break;


//-------------------------------------------------------------------------------
    // Запись блока.
    case START_WRITE_BLOCK_DATA:
        BlockWritePrepare();
        // Установим время ожидания окончания записи.
        GetTimerPointer() -> Set(WRITE_END_WAITING_TIMEOUT);
        SetFsmState(WRITE_END_WAITING_BLOCK_DATA);
        break;

    case WRITE_END_WAITING_BLOCK_DATA:
        // Ожидаем окончания записи автоматом устройства хранения.
        // Устройство хранения закончило запись успешно?
        if (m_pxStorageDevice -> GetFsmAnswerState() ==
                CStorageDeviceInterface::DATA_WRITED_SUCCESSFULLY)
        {
            // Запись прошла успешно?
            if (CheckBlock())
            {
                SetFsmState(START_WRITE_SERVICE_SECTION_DATA);
            }
            else
            {
                SetFsmState(WRITE_ERROR);
            }
        }
        // Устройство хранения закончило запись не успешно?
        else if (m_pxStorageDevice -> GetFsmAnswerState() ==
                 CStorageDeviceInterface::WRITE_DATA_ERROR)
        {
            SetFsmState(WRITE_ERROR);
        }
        else
        {
            // Время ожидания окончания записи закончилось?
            if (GetTimerPointer() -> IsOverflow())
            {
                SetFsmState(WRITE_ERROR);
            }
        }
        break;


//-------------------------------------------------------------------------------
    // Запись служеьного блока.
    case START_WRITE_SERVICE_SECTION_DATA:
        ServiceSectionWritePrepare();
        // Установим время ожидания окончания записи.
        GetTimerPointer() -> Set(WRITE_END_WAITING_TIMEOUT);
        SetFsmState(WRITE_END_WAITING_SERVICE_SECTION_DATA);
        break;

    case WRITE_END_WAITING_SERVICE_SECTION_DATA:
        // Ожидаем окончания записи автоматом устройства хранения.
        // Устройство хранения закончило запись успешно?
        if (m_pxStorageDevice -> GetFsmAnswerState() ==
                CStorageDeviceInterface::DATA_WRITED_SUCCESSFULLY)
        {
            // Запись прошла успешно?
            if (ReadServiceSection())
            {
                SetFsmState(DATA_WRITED_SUCCESSFULLY);
            }
            else
            {
                SetFsmState(WRITE_ERROR);
            }
        }
        // Устройство хранения закончило запись не успешно?
        else if (m_pxStorageDevice -> GetFsmAnswerState() ==
                 CStorageDeviceInterface::WRITE_DATA_ERROR)
        {
            SetFsmState(WRITE_ERROR);
        }
        else
        {
            // Время ожидания окончания записи закончилось?
            if (GetTimerPointer() -> IsOverflow())
            {
                SetFsmState(WRITE_ERROR);
            }
        }
        break;

    case DATA_WRITED_SUCCESSFULLY:
        break;

    case WRITE_ERROR:
        break;

    default:
        break;
    }

    return GetFsmState();
}

//-------------------------------------------------------------------------------












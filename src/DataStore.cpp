
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
#include "MessageBox.h"
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
    sprintf(GetTaskNamePointer(),
            "%s",
            typeid(*this).name());
    m_pxStorageDevice = 0;
    m_puiIntermediateBuff = new uint8_t[CDataStore::MAX_SERVICE_SECTION_DATA_LENGTH];
    SetFsmState(START);
}

//-------------------------------------------------------------------------------
CDataStore::~CDataStore()
{
    delete[] m_puiIntermediateBuff;
}

//-------------------------------------------------------------------------------
uint8_t CDataStore::Init(void)
{
    std::cout << "CDataStore Init"  << std::endl;
    m_pxCommandDataContainer = static_cast<CDataContainerDataBase*>(GetResources() ->
                               AddDataContainer(std::make_shared<CDataContainerDataBase>()));
    m_pxOperatingDataContainer = static_cast<CDataContainerDataBase*>(GetResources() ->
                                 AddDataContainer(std::make_shared<CDataContainerDataBase>()));
}

//-------------------------------------------------------------------------------
size_t CDataStore::GetObjectLength(void)
{
    std::cout << "CDataStore GetObjectLength"  << std::endl;
    return sizeof(*this);
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
void CDataStore::SetBlockIndex(uint8_t uiBlockIndex)
{
    CDataContainerDataBase* pxDataContainer = m_pxCommandDataContainer;

    pxDataContainer ->
    SetDataIndex(uiBlockIndex);
}

//-------------------------------------------------------------------------------
void CDataStore::CreateServiceSection(void)
{
    std::cout << "CDataStore::CreateServiceSection 1"  << std::endl;

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

    // Звкодируем данные алгоритмом Хемминга.
    uint16_t uiEncodedByteCounter =
        CHammingCodes::BytesToHammingCodes(m_puiIntermediateBuff,
                                           reinterpret_cast<uint8_t*>(&m_xServiseSection),
                                           sizeof(struct TServiseSection));

    CDataContainerDataBase* pxDataContainer = m_pxOperatingDataContainer;

    pxDataContainer ->
    SetDataIndex(0);
    pxDataContainer ->
    SetDataPointer(m_puiIntermediateBuff);
    pxDataContainer ->
    SetDataOffset(TEMPORARY_SERVICE_SECTION_DATA_BEGIN);
    pxDataContainer ->
    SetDataLength(uiEncodedByteCounter);

    if (m_pxStorageDevice -> WriteBlock(pxDataContainer))
    {
        std::cout << "CDataStore::TemporaryServiceSectionWritePrepare 4"  << std::endl;
        return 1;
    }
    else
    {
        std::cout << "CDataStore::TemporaryServiceSectionWritePrepare 5"  << std::endl;
        return 0;
    }
}

//-------------------------------------------------------------------------------
// Готовит данные служебного блока к записи.
// Кодирует по алгоритму Хемминга и
// передаёт автомату записи устройства хранения.
uint8_t CDataStore::ServiceSectionWritePrepare(void)
{
    std::cout << "CDataStore::ServiceSectionWritePrepare 1"  << std::endl;
    // Вычислим контрольную сумму.
    m_xServiseSection.uiCrc =
        usCrc16(reinterpret_cast<uint8_t*>(&m_xServiseSection.xServiseSectionData),
                sizeof(struct TServiseSectionData));

    // Звкодируем данные алгоритмом Хемминга.
    uint16_t uiEncodedByteCounter =
        CHammingCodes::BytesToHammingCodes(m_puiIntermediateBuff,
                                           reinterpret_cast<uint8_t*>(&m_xServiseSection),
                                           sizeof(struct TServiseSection));

    CDataContainerDataBase* pxDataContainer = m_pxOperatingDataContainer;

    pxDataContainer ->
    SetDataIndex(0);
    pxDataContainer ->
    SetDataPointer(m_puiIntermediateBuff);
    pxDataContainer ->
    SetDataOffset(SERVICE_SECTION_DATA_BEGIN);
    pxDataContainer ->
    SetDataLength(uiEncodedByteCounter);

    if (m_pxStorageDevice -> WriteBlock(pxDataContainer))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

//-------------------------------------------------------------------------------
// Готовит данные блока к записи во временный буфер.
// Кодирует по алгоритму Хемминга, размечает место для хранения и
// передаёт автомату записи устройства хранения.
uint8_t CDataStore::TemporaryBlockWritePrepare(void)
{
    std::cout << "CDataStore::TemporaryBlockWritePrepare 1"  << std::endl;

    CDataContainerDataBase* pxDataContainer = m_pxCommandDataContainer;

    uint8_t uiBlock = pxDataContainer -> m_uiDataIndex;
    uint8_t *puiSource = pxDataContainer -> m_puiDataPointer;
//    uint16_t uiOffset = pxDataContainer -> m_uiDataOffset;
    uint16_t uiLength = pxDataContainer -> m_uiDataLength;
    std::cout << "CDataStore::TemporaryBlockWritePrepare 1 uiBlock "  <<  (int)(uiBlock) << std::endl;

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

    std::cout << "CDataStore::TemporaryBlockWritePrepare 22"  << std::endl;
    // Вычислим контрольную сумму поступивших данных.
    m_xServiseSection.xServiseSectionData.
    axBlockPositionData[uiBlock].uiCrc =
        usCrc16(puiSource, uiLength);

    pxDataContainer = m_pxOperatingDataContainer;

    pxDataContainer ->
    SetDataIndex(uiBlock);
    pxDataContainer ->
    SetDataPointer(m_puiIntermediateBuff);
    pxDataContainer ->
    SetDataOffset(TEMPORARY_BLOCK_DATA_BEGIN);
    pxDataContainer ->
    SetDataLength(uiEncodedByteCounter);

    if (m_pxStorageDevice -> WriteBlock(pxDataContainer))
    {
        std::cout << "CDataStore::TemporaryBlockWritePrepare 3"  << std::endl;
        return 1;
    }
    else
    {
        std::cout << "CDataStore::TemporaryBlockWritePrepare 4"  << std::endl;
        return 0;
    }
}

//-------------------------------------------------------------------------------
// Готовит данные блока к записи.
// Кодирует по алгоритму Хемминга, размечает место для хранения и
// передаёт автомату записи устройства хранения.
uint8_t CDataStore::BlockWritePrepare(void)
{
    std::cout << "CDataStore::BlockWritePrepare 1"  << std::endl;

    CDataContainerDataBase* pxDataContainer = m_pxCommandDataContainer;

    uint8_t uiBlock = pxDataContainer -> m_uiDataIndex;
    uint8_t *puiSource = pxDataContainer -> m_puiDataPointer;
    uint16_t uiOffset = pxDataContainer -> m_uiDataOffset;
    uint16_t uiLength = pxDataContainer -> m_uiDataLength;
    std::cout << "CDataStore::BlockWritePrepare 1 uiBlock "  <<  (int)(uiBlock) << std::endl;
    cout << "CDataStore::Write puiSource" << " " << (int)puiSource << endl;
    cout << "CDataStore::Write uiOffset" << " " << (int)uiOffset << endl;
    cout << "CDataStore::Write uiLength" << " " << (int)uiLength << endl;

    // Звкодируем данные алгоритмом Хемминга.
    uint16_t uiEncodedByteCounter =
        CHammingCodes::BytesToHammingCodes(m_puiIntermediateBuff, puiSource, uiLength);

    // Данные текущего блока данных - uiBlock сохраняются впервые?
    if ((m_xServiseSection.xServiseSectionData.
            axBlockPositionData[uiBlock].uiLength == 0) &&
            (m_xServiseSection.xServiseSectionData.
             axBlockPositionData[uiBlock].uiEncodedLength == 0))
    {
        std::cout << "CDataStore::BlockWritePrepare 2"  << std::endl;
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

    pxDataContainer = m_pxOperatingDataContainer;

    pxDataContainer ->
    SetDataIndex(uiBlock);
    pxDataContainer ->
    SetDataPointer(m_puiIntermediateBuff);
    pxDataContainer ->
    SetDataOffset(m_xServiseSection.xServiseSectionData.
                  axBlockPositionData[uiBlock].uiOffset);
    pxDataContainer ->
    SetDataLength(uiEncodedByteCounter);

    if (m_pxStorageDevice -> WriteBlock(pxDataContainer))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

//-------------------------------------------------------------------------------
// Передаёт данные контекста записи блока автомату хранилища данных и запускает процесс записи.
uint8_t CDataStore::WriteBlock(uint8_t *puiSource, uint16_t uiLength, uint8_t uiBlock)
{
    std::cout << "CDataStore::WriteBlock 1"  << std::endl;
    std::cout << "CDataStore::WriteBlock 1 uiBlock "  <<  (int)(uiBlock) << std::endl;
    std::cout << "CDataStore::WriteBlock 1 uiLength "  <<  (int)(uiLength) << std::endl;
    // Произошёл выход за границы буфера?
    if (uiBlock >= (MAX_BLOCKS_NUMBER + SERVICE_SECTION_BLOCK_NUMBER))
    {
        std::cout << "CDataStore::WriteBlock 2"  << std::endl;
        // Нет данных.
        return 0;
    }

    CDataContainerDataBase* pxDataContainer = m_pxCommandDataContainer;

    std::cout << "CDataStore::WriteBlock 3"  << std::endl;
    pxDataContainer ->
    SetDataIndex(uiBlock);
    pxDataContainer ->
    SetDataPointer(puiSource);
    pxDataContainer ->
    SetDataOffset(0);
    pxDataContainer ->
    SetDataLength(uiLength);

    std::cout << "CDataStore::WriteBlock 4"  << std::endl;
    SetFsmCommandState(START_WRITE_TEMPORARY_BLOCK_DATA);

    return 1;
}

//-------------------------------------------------------------------------------
// Передаёт данные контекста записи блока автомату устройства хранения и запускает процесс записи.
bool CDataStore::WriteBlock(CDataContainerDataBase* pxDataContainer)
{
    std::cout << "CDataStore WriteBlock 1"  << std::endl;
//    if (GetMessageBoxDataContainerPointer() == 0)
//    {
//        std::cout << "CDataStore WriteBlock 2"  << std::endl;
////        m_pxCommandDataContainer = pxDataContainer;
////        SetFsmCommandState(WRITE_DATA_START);
//        return true;
//    }
//    else
//    {
//        std::cout << "CDataStore WriteBlock 3"  << std::endl;
//        return false;
//    }
}

//-------------------------------------------------------------------------------
// Считывает и проверяет целостность временного служебного блока.
uint8_t CDataStore::ReadTemporaryServiceSection(void)
{
    std::cout << "CDataStore::ReadTemporaryServiceSection 1 " << std::endl;
    uint16_t uiEncodedLength =
        (CHammingCodes::CalculateEncodedDataLength(sizeof(struct TServiseSection)));

    std::cout << "CDataStore::ReadTemporaryServiceSection 11 " << std::endl;
    // получим указатель на контейнер с данными задачи
    CDataContainerDataBase* pxDataContainer = m_pxOperatingDataContainer;

    std::cout << "CDataStore::ReadTemporaryServiceSection 12 " << std::endl;
    pxDataContainer ->
    SetDataIndex(0);
    pxDataContainer ->
    SetDataPointer(m_puiIntermediateBuff);
    pxDataContainer ->
    SetDataOffset(TEMPORARY_SERVICE_SECTION_DATA_BEGIN);
    pxDataContainer ->
    SetDataLength(uiEncodedLength);

    std::cout << "CDataStore::ReadTemporaryServiceSection 13 " << std::endl;
    // Прочитаем закодированные данные.
    // При чтении данных возникла ошибка?
    if (!(m_pxStorageDevice -> ReadBlock(pxDataContainer)))
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
    std::cout << "CDataStore::ReadServiceSection 1"  << std::endl;
    uint16_t uiEncodedLength =
        (CHammingCodes::CalculateEncodedDataLength(sizeof(struct TServiseSection)));

    // получим указатель на контейнер с данными задачи исполнителя
    CDataContainerDataBase* pxDataContainer = m_pxOperatingDataContainer;

    pxDataContainer ->
    SetDataIndex(0);
    pxDataContainer ->
    SetDataPointer(m_puiIntermediateBuff);
    pxDataContainer ->
    SetDataOffset(SERVICE_SECTION_DATA_BEGIN);
    pxDataContainer ->
    SetDataLength(uiEncodedLength);

    // Прочитаем закодированные данные.
    // При чтении данных возникла ошибка?
    if (!(m_pxStorageDevice -> ReadBlock(pxDataContainer)))
    {
        std::cout << "CDataStore::ReadServiceSection 2"  << std::endl;
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
        std::cout << "CDataStore::ReadServiceSection 3"  << std::endl;
        return 1;
    }
    else
    {
        std::cout << "CDataStore::ReadServiceSection 4"  << std::endl;
        // Нет данных.
        return 0;
    }
}

//-------------------------------------------------------------------------------
// Считывает и проверяет целостность временного буфера блока.
uint8_t CDataStore::CheckTemporaryBlock(void)
{
    std::cout << "CDataStore::CheckTemporaryBlock 1"  << std::endl;
//    // получим указатель на свой контейнер с данными
    CDataContainerDataBase* pxDataContainer = m_pxCommandDataContainer;

    uint8_t uiBlock = pxDataContainer -> m_uiDataIndex;
    std::cout << "CDataStore::CheckTemporaryBlock 1 uiBlock "  <<  (int)(uiBlock) << std::endl;

    // Произошёл выход за границы буфера?
    if (uiBlock >= MAX_BLOCKS_NUMBER)
    {
        std::cout << "CDataStore::CheckTemporaryBlock 2"  << std::endl;
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
        std::cout << "CDataStore::CheckTemporaryBlock 3"  << std::endl;
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
        std::cout << "CDataStore::CheckTemporaryBlock 4"  << std::endl;
        // Нет данных.
        return 0;
    }

    pxDataContainer = m_pxOperatingDataContainer;

    pxDataContainer ->
    SetDataIndex(uiBlock);
    pxDataContainer ->
    SetDataPointer(m_puiIntermediateBuff);
    pxDataContainer ->
    SetDataOffset(TEMPORARY_BLOCK_DATA_BEGIN);
    pxDataContainer ->
    SetDataLength(uiEncodedLength);

    // Прочитаем закодированные данные.
    // При чтении данных возникла ошибка?
    if (!(m_pxStorageDevice -> ReadBlock(pxDataContainer)))
    {
        std::cout << "CDataStore::CheckTemporaryBlock 5"  << std::endl;
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
        std::cout << "CDataStore::CheckTemporaryBlock 6"  << std::endl;
        return 1;
    }
    else
    {
        std::cout << "CDataStore::CheckTemporaryBlock 7"  << std::endl;
        // Нет данных.
        return 0;
    }
}

//-------------------------------------------------------------------------------
// Считывает и проверяет целостность блока.
uint8_t CDataStore::CheckBlock(void)
{
    std::cout << "CDataStore::CheckBlock 1"  << std::endl;

    CDataContainerDataBase* pxDataContainer = m_pxCommandDataContainer;

    uint8_t uiBlock = pxDataContainer -> m_uiDataIndex;
    std::cout << "CDataStore::CheckBlock 1 uiBlock "  <<  (int)(uiBlock) << std::endl;

    // Произошёл выход за границы буфера?
    if (uiBlock >= MAX_BLOCKS_NUMBER)
    {
        std::cout << "CDataStore::CheckBlock 2"  << std::endl;
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
        std::cout << "CDataStore::CheckBlock 3"  << std::endl;
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
        std::cout << "CDataStore::CheckBlock 4"  << std::endl;
        // Нет данных.
        return 0;
    }

    pxDataContainer = m_pxOperatingDataContainer;

    pxDataContainer ->
    SetDataIndex(uiBlock);
    pxDataContainer ->
    SetDataPointer(m_puiIntermediateBuff);
    pxDataContainer ->
    SetDataOffset(uiSourceOffset);
    pxDataContainer ->
    SetDataLength(uiEncodedLength);

    // Прочитаем закодированные данные.
    // При чтении данных возникла ошибка?
    if (!(m_pxStorageDevice -> ReadBlock(pxDataContainer)))
    {
        std::cout << "CDataStore::CheckBlock 5"  << std::endl;
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
        std::cout << "CDataStore::CheckBlock 6"  << std::endl;
        return 1;
    }
    else
    {
        std::cout << "CDataStore::CheckBlock 7"  << std::endl;
        // Нет данных.
        return 0;
    }
}
//-------------------------------------------------------------------------------
// Считывает и проверяет целостность блока.
uint16_t CDataStore::ReadBlock(uint8_t *puiDestination, uint8_t uiBlock)
{
    std::cout << "CDataStore::ReadBlock 1"  << std::endl;

//    uint8_t uiBlock = uiBlock;
    std::cout << "CDataStore::ReadBlock 1 uiBlock "  <<  (int)(uiBlock) << std::endl;

    // Произошёл выход за границы буфера?
    if (uiBlock >= MAX_BLOCKS_NUMBER)
    {
        std::cout << "CDataStore::ReadBlock 2"  << std::endl;
        // Нет данных.
        return 0;
    }

    uint16_t uiLength;
    uint16_t uiEncodedLength;
    uint16_t uiSourceOffset;

    for(int i=0; i<100; i++)
    {
        // Блок существует?
        if ((m_xServiseSection.xServiseSectionData.
                axBlockPositionData[i].uiLength != 0) &&
                (m_xServiseSection.xServiseSectionData.
                 axBlockPositionData[i].uiEncodedLength != 0))
        {
            std::cout << "CDataStore::ReadBlock Block ok " << i << std::endl;
        }
        else
        {
            std::cout << "CDataStore::ReadBlock Block error " << i << std::endl;
        }
    }

    // Блок существует?
    if ((m_xServiseSection.xServiseSectionData.
            axBlockPositionData[uiBlock].uiLength != 0) &&
            (m_xServiseSection.xServiseSectionData.
             axBlockPositionData[uiBlock].uiEncodedLength != 0))
    {
        std::cout << "CDataStore::ReadBlock 3"  << std::endl;
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
        std::cout << "CDataStore::ReadBlock 4"  << std::endl;
        // Нет данных.
        return 0;
    }

    CDataContainerDataBase* pxDataContainer = m_pxOperatingDataContainer;

    pxDataContainer ->
    SetDataIndex(uiBlock);
    pxDataContainer ->
    SetDataPointer(m_puiIntermediateBuff);
    pxDataContainer ->
    SetDataOffset(uiSourceOffset);
    pxDataContainer ->
    SetDataLength(uiEncodedLength);

    // Прочитаем закодированные данные.
    // При чтении данных возникла ошибка?
    if (!(m_pxStorageDevice -> ReadBlock(pxDataContainer)))
    {
        std::cout << "CDataStore::ReadBlock 5"  << std::endl;
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
        std::cout << "CDataStore::ReadBlock 6"  << std::endl;
        memcpy(puiDestination, m_puiIntermediateBuff, uiLength);
        return 1;
    }
    else
    {
        std::cout << "CDataStore::ReadBlock 7"  << std::endl;
        // Нет данных.
        return 0;
    }
}

//-------------------------------------------------------------------------------
// Вызывается только если база данных подтверждена пользователем.
void CDataStore::CrcOfBlocksCrcCreate(void)
{
    std::cout << "CDataStore::CrcOfBlocksCrcCreate 1"  << std::endl;
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
    std::cout << "CDataStore::CrcOfBlocksCrcCheck 1"  << std::endl;
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
        std::cout << "CDataStore::CrcOfBlocksCrcCheck 2"  << std::endl;
        return true;
    }
    else
    {
        std::cout << "CDataStore::CrcOfBlocksCrcCheck 3"  << std::endl;
        return false;
    }
}


//-------------------------------------------------------------------------------
uint8_t CDataStore::GetBlockLength(uint8_t uiBlock)
{
    std::cout << "CDataStore::GetBlockLength 1"  << std::endl;
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
        Init();
        GetTimerPointer() -> Set(TASK_READY_WAITING_TIME);
        SetFsmState(INIT);
        break;

    case INIT:
//        std::cout << "CDataStore::Fsm INIT"  << std::endl;
    {
        CTaskInterface* pxTask =
            GetResources() ->
            GetTaskPointerByNameFromMap(m_sStorageDeviceName);

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
        m_uiRequestRetryCounter = 0;
        // Установим время ожидания окончания записи.
        GetTimerPointer() -> Set(WRITE_END_WAITING_TIMEOUT);
        SetFsmState(WRITE_END_WAITING_TEMPORARY_BLOCK_DATA);
        break;

    case WRITE_END_WAITING_TEMPORARY_BLOCK_DATA:
//        std::cout << "CDataStore::Fsm 2"  << std::endl;
        // Ожидаем окончания записи автоматом устройства хранения.
        // Устройство хранения закончило запись успешно?
        if (m_pxStorageDevice -> IsDoneOk())
        {
            std::cout << "CDataStore::Fsm 3"  << std::endl;
            // Запись прошла успешно?
            if (CheckTemporaryBlock())
            {
                cerr << "CheckTemporaryBlock ok" << endl;
                SetFsmState(START_WRITE_TEMPORARY_SERVICE_SECTION_DATA);
            }
            else
            {
                cerr << "CheckTemporaryBlock error" << endl;
                SetFsmState(WRITE_ERROR);
            }
        }
        // Устройство хранения закончило запись не успешно?
        else if (m_pxStorageDevice -> IsDoneError())
        {
            std::cout << "CDataStore::Fsm 4"  << std::endl;
            SetFsmState(WRITE_ERROR);
        }
        else
        {
            // Время ожидания окончания записи закончилось?
            if (GetTimerPointer() -> IsOverflow())
            {
                std::cout << "CDataStore::Fsm 5"  << std::endl;
                SetFsmState(WRITE_ERROR);
            }
        }
        break;


//-------------------------------------------------------------------------------
    // Запись временного служеьного блока.
    case START_WRITE_TEMPORARY_SERVICE_SECTION_DATA:
        std::cout << "CDataStore::Fsm 6"  << std::endl;
        TemporaryServiceSectionWritePrepare();
        // Установим время ожидания окончания записи.
        GetTimerPointer() -> Set(WRITE_END_WAITING_TIMEOUT);
        SetFsmState(WRITE_END_WAITING_TEMPORARY_SERVICE_SECTION_DATA);
        break;

    case WRITE_END_WAITING_TEMPORARY_SERVICE_SECTION_DATA:
        // Ожидаем окончания записи автоматом устройства хранения.
        // Устройство хранения закончило запись успешно?
        if (m_pxStorageDevice -> IsDoneOk())
        {
            std::cout << "CDataStore::Fsm 7"  << std::endl;
            // Запись прошла успешно?
            if (ReadTemporaryServiceSection())
            {
                std::cout << "CDataStore::Fsm 8"  << std::endl;
                SetFsmState(START_WRITE_BLOCK_DATA);
            }
            else
            {
                std::cout << "CDataStore::Fsm 9"  << std::endl;
                SetFsmState(WRITE_ERROR);
            }
        }
        // Устройство хранения закончило запись не успешно?
        else if (m_pxStorageDevice -> IsDoneError())
        {
            std::cout << "CDataStore::Fsm 10"  << std::endl;
            SetFsmState(WRITE_ERROR);
        }
        else
        {
            // Время ожидания окончания записи закончилось?
            if (GetTimerPointer() -> IsOverflow())
            {
                std::cout << "CDataStore::Fsm 12"  << std::endl;
//                m_pxStorageDevice -> SetFsmOperationStatus(0);
                SetFsmState(WRITE_ERROR);
            }
        }
        break;


//-------------------------------------------------------------------------------
    // Запись блока.
    case START_WRITE_BLOCK_DATA:
        std::cout << "CDataStore::Fsm 13"  << std::endl;
        BlockWritePrepare();
        // Установим время ожидания окончания записи.
        GetTimerPointer() -> Set(WRITE_END_WAITING_TIMEOUT);
        SetFsmState(WRITE_END_WAITING_BLOCK_DATA);
        break;

    case WRITE_END_WAITING_BLOCK_DATA:
        // Ожидаем окончания записи автоматом устройства хранения.
        // Устройство хранения закончило запись успешно?
        if (m_pxStorageDevice -> IsDoneOk())
        {
            std::cout << "CDataStore::Fsm 14"  << std::endl;
            // Запись прошла успешно?
            if (CheckBlock())
            {
                std::cout << "CDataStore::Fsm 15"  << std::endl;
                SetFsmState(START_WRITE_SERVICE_SECTION_DATA);
            }
            else
            {
                std::cout << "CDataStore::Fsm 16"  << std::endl;
                SetFsmState(WRITE_ERROR);
            }
        }
        // Устройство хранения закончило запись не успешно?
        else if (m_pxStorageDevice -> IsDoneError())
        {
            std::cout << "CDataStore::Fsm 17"  << std::endl;
            SetFsmState(WRITE_ERROR);
        }
        else
        {
            // Время ожидания окончания записи закончилось?
            if (GetTimerPointer() -> IsOverflow())
            {
                std::cout << "CDataStore::Fsm 18"  << std::endl;
                SetFsmState(WRITE_ERROR);
            }
        }
        break;


//-------------------------------------------------------------------------------
    // Запись служеьного блока.
    case START_WRITE_SERVICE_SECTION_DATA:
        std::cout << "CDataStore::Fsm 19"  << std::endl;
        ServiceSectionWritePrepare();
        // Установим время ожидания окончания записи.
        GetTimerPointer() -> Set(WRITE_END_WAITING_TIMEOUT);
        SetFsmState(WRITE_END_WAITING_SERVICE_SECTION_DATA);
        break;

    case WRITE_END_WAITING_SERVICE_SECTION_DATA:
        // Ожидаем окончания записи автоматом устройства хранения.
        // Устройство хранения закончило запись успешно?
        if (m_pxStorageDevice -> IsDoneOk())
        {
            std::cout << "CDataStore::Fsm 20"  << std::endl;
            // Запись прошла успешно?
            if (ReadServiceSection())
            {
                std::cout << "CDataStore::Fsm 21"  << std::endl;
                SetFsmState(DATA_WRITED_SUCCESSFULLY);
            }
            else
            {
                std::cout << "CDataStore::Fsm 22"  << std::endl;
                SetFsmState(WRITE_ERROR);
            }
        }
        // Устройство хранения закончило запись не успешно?
        else if (m_pxStorageDevice -> IsDoneError())
        {
            std::cout << "CDataStore::Fsm 23"  << std::endl;
            SetFsmState(WRITE_ERROR);
        }
        else
        {
            // Время ожидания окончания записи закончилось?
            if (GetTimerPointer() -> IsOverflow())
            {
                std::cout << "CDataStore::Fsm 24"  << std::endl;
                SetFsmState(WRITE_ERROR);
            }
        }
        break;

    case DATA_WRITED_SUCCESSFULLY:
        std::cout << "CDataStore::Fsm 25"  << std::endl;
        SetFsmOperationStatus(DONE_OK);
        SetFsmState(READY);
        break;

    case WRITE_ERROR:
        std::cout << "CDataStore::Fsm 26"  << std::endl;
        SetFsmOperationStatus(DONE_ERROR);
        SetFsmState(READY);
        break;

    default:
        std::cout << "CDataStore::Fsm default"  << std::endl;
        break;
    }

    return GetFsmState();
}

//-------------------------------------------------------------------------------












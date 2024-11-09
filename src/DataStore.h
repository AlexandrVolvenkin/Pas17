//-------------------------------------------------------------------------------
//  Source      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-------------------------------------------------------------------------------
#ifndef CDATASTORE_H
#define CDATASTORE_H

#include <stdint.h>
#include <iostream>
#include <fstream>


class CTimer;
class CTask;
class CResources;
class CStorageDeviceInterface;
class CStorageDeviceFileSystem;
class CDataContainerInterface;
class CDataContainerDataBase;

using namespace std;

//-------------------------------------------------------------------------------
// Хранилище блочых данных.
// Проверяет целостность данных хранилища.
// Восстанавлмвает повреждённые данные с помощью алгоритма Хемминга.
// Восстанавливает данные записываемого блока при сбое питания и т.д.
// Состав хранилища:
// 1 - блок служебных данных.
// 2 - дублирующий блок для записи временных служебных данных.
// 3 - временная копия записываемого блока.
// 4 - пространство с последовательно расположенными хранимыми блоками.
class CDataStore : public CTask//, public CDfa
{
public:
    enum
    {
        CRC_LENGTH = 2,
        TAIL_LENGTH = 2,
        MAX_BLOCK_LENGTH = 256,
        MAX_ENCODED_BLOCK_LENGTH =
            ((MAX_BLOCK_LENGTH + TAIL_LENGTH) + ((MAX_BLOCK_LENGTH + TAIL_LENGTH) / 2)),
        MAX_BLOCKS_NUMBER = 100,//(TDataBase::BLOCKS_QUANTITY + SERVICE_SECTION_DATA_BLOCK_NUMBER),
    };

    enum
    {
        READY_TO_WRITE_WAITING_TIMEOUT = 200,
        WRITE_END_WAITING_TIMEOUT = 10000,
    };

    enum
    {
        START_WRITE_BLOCK_DATA = NEXT_STEP,
        READY_TO_WRITE_WAITING_BLOCK_DATA,
        WRITE_END_WAITING_BLOCK_DATA,

        START_WRITE_TEMPORARY_BLOCK_DATA,
        READY_TO_WRITE_WAITING_TEMPORARY_BLOCK_DATA,
        WRITE_END_WAITING_TEMPORARY_BLOCK_DATA,

        START_WRITE_TEMPORARY_SERVICE_SECTION_DATA,
        READY_TO_WRITE_WAITING_TEMPORARY_SERVICE_SECTION_DATA,
        WRITE_END_WAITING_TEMPORARY_SERVICE_SECTION_DATA,

        START_WRITE_SERVICE_SECTION_DATA,
        READY_TO_WRITE_WAITING_SERVICE_SECTION_DATA,
        WRITE_END_WAITING_SERVICE_SECTION_DATA,

        DATA_WRITED_SUCCESSFULLY,
        WRITE_ERROR,
    };

//    enum
//    {
//        NO_EVENT_FSM_EVENT = 0,
//        WRITE_IN_PROGRESS_FSM_EVENT,
//        STORAGE_DEVICE_BUSY_FSM_EVENT,
//        WRITE_OK_FSM_EVENT,
//        WRITE_ERROR_FSM_EVENT,
//    };

    struct TBlockPositionData
    {
        // Смещение на данные блока.
        uint16_t uiOffset;
        // Размер хранимого блока.
        uint16_t uiLength;
        // Размер закодированного хранимого блока.
        uint16_t uiEncodedLength;
        // Crc блока хранится отдельно от него в служебном блоке.
        // По его совпадению определяется целостность блока и его принадлежность к хранилищу.
        uint16_t uiCrc;
    };

    struct TServiseSectionData
    {
        // Смещение на свободное место для записи нового блока.
        uint16_t uiFreeSpaceOffset;
        // Размер служебного блока.
        uint16_t uiLength;
        // Размер закодированных служебного блока.
        // Используется самовосстанавливающийся код Хемминга(8,4).
        // Коэффициент - 1.5: один байт преобразуется в кодовое слово 12 бит,
        // из двух байт полезных данных получается три байта кодированных.
        uint16_t uiEncodedLength;
//        uint16_t uiLastWritedBlockNumber;
        uint16_t uiStoredBlocksNumber;
        // Контрольная сумма вычисленная из массива контрольных сумм блоков, не включая служебный.
        // Сохраняется при первой и последующих записях любых блоков через программатор.
        // Ноль или её несовпадение свидетельствует о том, что база данных создана по умоланию,
        // и не подтверждена пользователем. В этом случае прибор переходит в режим сигнализации об ошибке,
        // ожидая квитирования или записи базы данных.
        uint16_t uiCrcOfBlocksCrc;
        TBlockPositionData axBlockPositionData[MAX_BLOCKS_NUMBER];
    };

    struct TServiseSection
    {
        TServiseSectionData xServiseSectionData;
        // Crc служебного блока.
        uint16_t uiCrc;
    };

    enum
    {
        // Нулевой байт может быть стёрт при сбое питания.
        TEMPORARY_BLOCK_DATA_BEGIN = 8,
        TEMPORARY_SERVICE_SECTION_DATA_BEGIN =
            (TEMPORARY_BLOCK_DATA_BEGIN + MAX_ENCODED_BLOCK_LENGTH),
        SERVICE_SECTION_DATA_BEGIN =
            TEMPORARY_SERVICE_SECTION_DATA_BEGIN +
            (sizeof(struct TServiseSection) + (sizeof(struct TServiseSection) / 2)),
        MAX_SERVICE_SECTION_DATA_LENGTH =
            (sizeof(struct TServiseSection) + (sizeof(struct TServiseSection) / 2)),
        TEMPORARY_BLOCK_INDEX = MAX_BLOCKS_NUMBER,
        TEMPORARY_SERVICE_SECTION_INDEX = MAX_BLOCKS_NUMBER + 1,
        SERVICE_SECTION_INDEX = MAX_BLOCKS_NUMBER + 2,
        SERVICE_SECTION_BLOCK_NUMBER = 3,
    };

    CDataStore();
    CDataStore(CStorageDeviceInterface* pxStorageDevice);
    virtual ~CDataStore();

    void SetStorageDeviceName(std::string sName);
    void SetStorageDevice(CStorageDeviceInterface* pxStorageDevice);
    void SetArgumentData(uint8_t *puiDataPointer,
                         uint16_t uiDataOffset,
                         uint16_t uiDataLength);
    void GetArgumentData(void);
//    void SetResources(CResources* pxResources);
//    CResources* GetResources(void);

//    uint8_t Check(void);
    void CreateServiceSection(void);
    uint8_t WriteBlock(uint8_t *puiSource, uint16_t uiLength, uint8_t uiBlock);
    uint16_t ReadBlock(uint8_t *, uint8_t );
    uint16_t GetReadedData();
    void CrcOfBlocksCrcCreate(void);
    bool CrcOfBlocksCrcCheck(void);
    uint8_t Fsm(void);

    uint8_t* GetIntermediateBuff(void)
    {
        return m_puiIntermediateBuff;
    };

    uint8_t GetBlockLength(uint8_t uiBlock);
//    {
//        return m_xServiseSection.xServiseSectionData.
//               axBlockPositionData[uiBlock].uiLength;
//               aucDataBaseBlockLength[CDataStore::MAX_BLOCKS_NUMBER]
//    };

    uint16_t GetStoredBlocksNumber(void)
    {
        return m_xServiseSection.xServiseSectionData.uiStoredBlocksNumber;
    };

    void SetBlockIndex(uint8_t uiBlockIndex);
//    {
//        m_uiBlockIndex = uiBlockIndex;
//    };
    uint8_t GetBlockIndex(void)
    {
        return m_uiBlockIndex;
    };


//private:
    uint8_t TemporaryServiceSectionWritePrepare(void);
    uint8_t ServiceSectionWritePrepare(void);
    uint8_t ReadTemporaryServiceSection(void);
    uint8_t ReadServiceSection(void);
    uint8_t CheckTemporaryBlock(void);
    uint8_t CheckBlock(void);
    uint8_t TemporaryBlockWritePrepare(void);
    uint8_t BlockWritePrepare(void);
//    CTimer* GetTimerPointer(void)
//    {
//        return &m_xTimer;
//    };

public:


protected:
//private:
//    CResources* m_pxResources;
    // Данные контекста записи блока.
    uint8_t m_uiBlockIndex;
    uint8_t* m_puiBlockDataPointer;
    uint16_t m_uiBlockOffset;
    uint16_t m_uiBlockLength;
    uint16_t m_uiBlockEncodedLength;

    std::string m_sStorageDeviceName;
    // Указатель на объект класса устройства хранения.
    CStorageDeviceInterface* m_pxStorageDevice;
    // Служебные данные системы хранения.
    TServiseSection m_xServiseSection;
    // Массив контрольных сумм блоков.
    uint16_t m_auiBlocksCurrentCrc[MAX_BLOCKS_NUMBER];
    // Вспомогательный буфер.
    uint8_t* m_puiIntermediateBuff;
//    CTimer m_xTimer;
};

//-------------------------------------------------------------------------------
#endif // CDATASTORE_H

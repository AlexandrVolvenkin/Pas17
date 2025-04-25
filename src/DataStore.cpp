
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

// ������ �������� ������� ������ ���� ������.
// ������ ���� ������ ������� �� 100 ������ �� 256 ����.
// ����� ������������ ����������� ������ ��� ����, (� ������� - modbus_reply() - case _FC_DATA_BASE_READ:),
// �� ����� �������, �� ������, ������������� ������� ������ �����.
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

    // ������� ��������� ��������.
    memset(reinterpret_cast<uint8_t*>(&m_xServiseSection.xServiseSectionData),
           0,
           sizeof(struct TServiseSectionData));

    // ��������� ������ ��������� ������.
    m_xServiseSection.xServiseSectionData.uiLength = sizeof(struct TServiseSectionData);
    // ���������� ���� ���������� ���������
    // ������ ���� �������� �������.
    // ��� ��� �� ���� ����������� ��������� ������.
    // ������������ ����������������������� ��� ��������(8,4).
    // ����������� - 1.5: ���� ���� ������������� � ������� ����� 12 ���,
    // �� ���� ���� �������� ������ ���������� ��� ����� ������������.
    // ��������� ������ �������������� ������.
    m_xServiseSection.xServiseSectionData.uiEncodedLength =
        (CHammingCodes::CalculateEncodedDataLength(sizeof(struct TServiseSection)));
    // ��������� �������� �� ������ ������ ������.
    m_xServiseSection.xServiseSectionData.uiFreeSpaceOffset =
        (SERVICE_SECTION_DATA_BEGIN +
         m_xServiseSection.xServiseSectionData.uiEncodedLength);
    // ��������� �����.
    m_xServiseSection.xServiseSectionData.uiStoredBlocksNumber = 0;
    // ��������� ������� - ��������� �� ���������.
    m_xServiseSection.xServiseSectionData.uiCrcOfBlocksCrc = 0;
}

//-------------------------------------------------------------------------------
// ������� ������ ���������� ���������� ����� � ������.
// �������� �� ��������� �������� �
// ������� �������� ������ ���������� ��������.
uint8_t CDataStore::TemporaryServiceSectionWritePrepare(void)
{
    std::cout << "CDataStore::TemporaryServiceSectionWritePrepare 1"  << std::endl;
    // �������� ����������� �����.
    m_xServiseSection.uiCrc =
        usCrc16(reinterpret_cast<uint8_t*>(&m_xServiseSection.xServiseSectionData),
                sizeof(struct TServiseSectionData));

    // ���������� ������ ���������� ��������.
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
// ������� ������ ���������� ����� � ������.
// �������� �� ��������� �������� �
// ������� �������� ������ ���������� ��������.
uint8_t CDataStore::ServiceSectionWritePrepare(void)
{
    std::cout << "CDataStore::ServiceSectionWritePrepare 1"  << std::endl;
    // �������� ����������� �����.
    m_xServiseSection.uiCrc =
        usCrc16(reinterpret_cast<uint8_t*>(&m_xServiseSection.xServiseSectionData),
                sizeof(struct TServiseSectionData));

    // ���������� ������ ���������� ��������.
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
// ������� ������ ����� � ������ �� ��������� �����.
// �������� �� ��������� ��������, ��������� ����� ��� �������� �
// ������� �������� ������ ���������� ��������.
uint8_t CDataStore::TemporaryBlockWritePrepare(void)
{
    std::cout << "CDataStore::TemporaryBlockWritePrepare 1"  << std::endl;

    CDataContainerDataBase* pxDataContainer = m_pxCommandDataContainer;

    uint8_t uiBlock = pxDataContainer -> m_uiDataIndex;
    uint8_t *puiSource = pxDataContainer -> m_puiDataPointer;
//    uint16_t uiOffset = pxDataContainer -> m_uiDataOffset;
    uint16_t uiLength = pxDataContainer -> m_uiDataLength;
    std::cout << "CDataStore::TemporaryBlockWritePrepare 1 uiBlock "  <<  (int)(uiBlock) << std::endl;

    // ���������� ������ ���������� ��������.
    uint16_t uiEncodedByteCounter =
        CHammingCodes::BytesToHammingCodes(m_puiIntermediateBuff, puiSource, uiLength);

    // ������ �������� ����� ������ - uiBlock ����������� �������?
    if ((m_xServiseSection.xServiseSectionData.
            axBlockPositionData[uiBlock].uiLength == 0) &&
            (m_xServiseSection.xServiseSectionData.
             axBlockPositionData[uiBlock].uiEncodedLength == 0))
    {
        std::cout << "CDataStore::TemporaryBlockWritePrepare 2"  << std::endl;
        // �������� ����� ���� ������.
        // �������� �������� �� ����.
        m_xServiseSection.xServiseSectionData.
        axBlockPositionData[uiBlock].uiOffset = m_xServiseSection.xServiseSectionData.uiFreeSpaceOffset;
        // ������� �������� �� ��������� �����.
        m_xServiseSection.xServiseSectionData.uiFreeSpaceOffset += uiEncodedByteCounter;
        // �������� ������ ��������� ������.
        m_xServiseSection.xServiseSectionData.
        axBlockPositionData[uiBlock].uiLength = uiLength;
        // �������� ������ �������������� ������.
        m_xServiseSection.xServiseSectionData.
        axBlockPositionData[uiBlock].uiEncodedLength = uiEncodedByteCounter;
        // �������� ���������� ������ ����������� � ���������.
        m_xServiseSection.xServiseSectionData.uiStoredBlocksNumber += 1;
    }

    std::cout << "CDataStore::TemporaryBlockWritePrepare 22"  << std::endl;
    // �������� ����������� ����� ����������� ������.
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
// ������� ������ ����� � ������.
// �������� �� ��������� ��������, ��������� ����� ��� �������� �
// ������� �������� ������ ���������� ��������.
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

    // ���������� ������ ���������� ��������.
    uint16_t uiEncodedByteCounter =
        CHammingCodes::BytesToHammingCodes(m_puiIntermediateBuff, puiSource, uiLength);

    // ������ �������� ����� ������ - uiBlock ����������� �������?
    if ((m_xServiseSection.xServiseSectionData.
            axBlockPositionData[uiBlock].uiLength == 0) &&
            (m_xServiseSection.xServiseSectionData.
             axBlockPositionData[uiBlock].uiEncodedLength == 0))
    {
        std::cout << "CDataStore::BlockWritePrepare 2"  << std::endl;
        // �������� ����� ���� ������.
        // �������� �������� �� ����.
        m_xServiseSection.xServiseSectionData.
        axBlockPositionData[uiBlock].uiOffset = m_xServiseSection.xServiseSectionData.uiFreeSpaceOffset;
        // ������� �������� �� ��������� �����.
        m_xServiseSection.xServiseSectionData.uiFreeSpaceOffset += uiEncodedByteCounter;
        // �������� ������ ��������� ������.
        m_xServiseSection.xServiseSectionData.
        axBlockPositionData[uiBlock].uiLength = uiLength;
        // �������� ������ �������������� ������.
        m_xServiseSection.xServiseSectionData.
        axBlockPositionData[uiBlock].uiEncodedLength = uiEncodedByteCounter;
//        // �������� ���������� ������ ����������� � ���������.
//        m_xServiseSection.xServiseSectionData.uiStoredBlocksNumber += 1;
    }

    // �������� ����������� ����� ����������� ������.
    m_xServiseSection.xServiseSectionData.
    axBlockPositionData[uiBlock].uiCrc =
        usCrc16(puiSource, uiLength);
//    // �������� ������ ���������� ������������� �����.
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
// ������� ������ ��������� ������ ����� �������� ��������� ������ � ��������� ������� ������.
uint8_t CDataStore::WriteBlock(uint8_t *puiSource, uint16_t uiLength, uint8_t uiBlock)
{
    std::cout << "CDataStore::WriteBlock 1"  << std::endl;
    std::cout << "CDataStore::WriteBlock 1 uiBlock "  <<  (int)(uiBlock) << std::endl;
    std::cout << "CDataStore::WriteBlock 1 uiLength "  <<  (int)(uiLength) << std::endl;
    // ��������� ����� �� ������� ������?
    if (uiBlock >= (MAX_BLOCKS_NUMBER + SERVICE_SECTION_BLOCK_NUMBER))
    {
        std::cout << "CDataStore::WriteBlock 2"  << std::endl;
        // ��� ������.
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
// ������� ������ ��������� ������ ����� �������� ���������� �������� � ��������� ������� ������.
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
// ��������� � ��������� ����������� ���������� ���������� �����.
uint8_t CDataStore::ReadTemporaryServiceSection(void)
{
    std::cout << "CDataStore::ReadTemporaryServiceSection 1 " << std::endl;
    uint16_t uiEncodedLength =
        (CHammingCodes::CalculateEncodedDataLength(sizeof(struct TServiseSection)));

    // ������� ��������� �� ��������� � ������� ������
    CDataContainerDataBase* pxDataContainer = m_pxOperatingDataContainer;

    pxDataContainer ->
    SetDataIndex(0);
    pxDataContainer ->
    SetDataPointer(m_puiIntermediateBuff);
    pxDataContainer ->
    SetDataOffset(TEMPORARY_SERVICE_SECTION_DATA_BEGIN);
    pxDataContainer ->
    SetDataLength(uiEncodedLength);

    // ��������� �������������� ������.
    // ��� ������ ������ �������� ������?
    if (!(m_pxStorageDevice -> ReadBlock(pxDataContainer)))
    {
        std::cout << "CDataStore::ReadTemporaryServiceSection 2"  << std::endl;
        // ��� ������.
        return 0;
    }

    // ���������� ����������� ������.
    CHammingCodes::HammingCodesToBytes(reinterpret_cast<uint8_t*>(&m_xServiseSection),
                                       m_puiIntermediateBuff,
                                       uiEncodedLength);

    // ���� �� ��������?
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
        // ��� ������.
        return 0;
    }
}

//-------------------------------------------------------------------------------
// ��������� � ��������� ����������� ���������� �����.
uint8_t CDataStore::ReadServiceSection(void)
{
    std::cout << "CDataStore::ReadServiceSection 1"  << std::endl;
    uint16_t uiEncodedLength =
        (CHammingCodes::CalculateEncodedDataLength(sizeof(struct TServiseSection)));

    // ������� ��������� �� ��������� � ������� ������ �����������
    CDataContainerDataBase* pxDataContainer = m_pxOperatingDataContainer;

    pxDataContainer ->
    SetDataIndex(0);
    pxDataContainer ->
    SetDataPointer(m_puiIntermediateBuff);
    pxDataContainer ->
    SetDataOffset(SERVICE_SECTION_DATA_BEGIN);
    pxDataContainer ->
    SetDataLength(uiEncodedLength);

    // ��������� �������������� ������.
    // ��� ������ ������ �������� ������?
    if (!(m_pxStorageDevice -> ReadBlock(pxDataContainer)))
    {
        std::cout << "CDataStore::ReadServiceSection 2"  << std::endl;
        // ��� ������.
        return 0;
    }

    // ���������� ����������� ������.
    CHammingCodes::HammingCodesToBytes(reinterpret_cast<uint8_t*>(&m_xServiseSection),
                                       m_puiIntermediateBuff,
                                       uiEncodedLength);

    // ���� �� ��������?
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
        // ��� ������.
        return 0;
    }
}

//-------------------------------------------------------------------------------
// ��������� � ��������� ����������� ���������� ������ �����.
uint8_t CDataStore::CheckTemporaryBlock(void)
{
    std::cout << "CDataStore::CheckTemporaryBlock 1"  << std::endl;
//    // ������� ��������� �� ���� ��������� � �������
    CDataContainerDataBase* pxDataContainer = m_pxCommandDataContainer;

    uint8_t uiBlock = pxDataContainer -> m_uiDataIndex;
    std::cout << "CDataStore::CheckTemporaryBlock 1 uiBlock "  <<  (int)(uiBlock) << std::endl;

    // ��������� ����� �� ������� ������?
    if (uiBlock >= MAX_BLOCKS_NUMBER)
    {
        std::cout << "CDataStore::CheckTemporaryBlock 2"  << std::endl;
        // ��� ������.
        return 0;
    }

    uint16_t uiLength;
    uint16_t uiEncodedLength;
    uint16_t uiSourceOffset;

    // ���� ����������?
    if ((m_xServiseSection.xServiseSectionData.
            axBlockPositionData[uiBlock].uiLength != 0) &&
            (m_xServiseSection.xServiseSectionData.
             axBlockPositionData[uiBlock].uiEncodedLength != 0))
    {
        std::cout << "CDataStore::CheckTemporaryBlock 3"  << std::endl;
        // ������� ����� ����� � EEPROM.
        uiSourceOffset = m_xServiseSection.xServiseSectionData.
                         axBlockPositionData[uiBlock].uiOffset;
        // ������� ������ �����.
        uiLength = m_xServiseSection.xServiseSectionData.
                   axBlockPositionData[uiBlock].uiLength;
        // ������� ������ ��������������� �����.
        uiEncodedLength = m_xServiseSection.xServiseSectionData.
                          axBlockPositionData[uiBlock].uiEncodedLength;
    }
    else
    {
        std::cout << "CDataStore::CheckTemporaryBlock 4"  << std::endl;
        // ��� ������.
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

    // ��������� �������������� ������.
    // ��� ������ ������ �������� ������?
    if (!(m_pxStorageDevice -> ReadBlock(pxDataContainer)))
    {
        std::cout << "CDataStore::CheckTemporaryBlock 5"  << std::endl;
        // ��� ������.
        return 0;
    }

    // ���������� ����������� ������.
    CHammingCodes::HammingCodesToBytes(m_puiIntermediateBuff,
                                       m_puiIntermediateBuff,
                                       uiEncodedLength);

    // ���� �� ��������?
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
        // ��� ������.
        return 0;
    }
}

//-------------------------------------------------------------------------------
// ��������� � ��������� ����������� �����.
uint8_t CDataStore::CheckBlock(void)
{
    std::cout << "CDataStore::CheckBlock 1"  << std::endl;

    CDataContainerDataBase* pxDataContainer = m_pxCommandDataContainer;

    uint8_t uiBlock = pxDataContainer -> m_uiDataIndex;
    std::cout << "CDataStore::CheckBlock 1 uiBlock "  <<  (int)(uiBlock) << std::endl;

    // ��������� ����� �� ������� ������?
    if (uiBlock >= MAX_BLOCKS_NUMBER)
    {
        std::cout << "CDataStore::CheckBlock 2"  << std::endl;
        // ��� ������.
        return 0;
    }

    uint16_t uiLength;
    uint16_t uiEncodedLength;
    uint16_t uiSourceOffset;

    // ���� ����������?
    if ((m_xServiseSection.xServiseSectionData.
            axBlockPositionData[uiBlock].uiLength != 0) &&
            (m_xServiseSection.xServiseSectionData.
             axBlockPositionData[uiBlock].uiEncodedLength != 0))
    {
        std::cout << "CDataStore::CheckBlock 3"  << std::endl;
        // ������� ����� ����� � EEPROM.
        uiSourceOffset = m_xServiseSection.xServiseSectionData.
                         axBlockPositionData[uiBlock].uiOffset;
        // ������� ������ �����.
        uiLength = m_xServiseSection.xServiseSectionData.
                   axBlockPositionData[uiBlock].uiLength;
        // ������� ������ ��������������� �����.
        uiEncodedLength = m_xServiseSection.xServiseSectionData.
                          axBlockPositionData[uiBlock].uiEncodedLength;
    }
    else
    {
        std::cout << "CDataStore::CheckBlock 4"  << std::endl;
        // ��� ������.
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

    // ��������� �������������� ������.
    // ��� ������ ������ �������� ������?
    if (!(m_pxStorageDevice -> ReadBlock(pxDataContainer)))
    {
        std::cout << "CDataStore::CheckBlock 5"  << std::endl;
        // ��� ������.
        return 0;
    }

    // ���������� ����������� ������.
    CHammingCodes::HammingCodesToBytes(m_puiIntermediateBuff,
                                       m_puiIntermediateBuff,
                                       uiEncodedLength);

    // ���� �� ��������?
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
        // ��� ������.
        return 0;
    }
}
//-------------------------------------------------------------------------------
// ��������� � ��������� ����������� �����.
uint16_t CDataStore::ReadBlock(uint8_t *puiDestination, uint8_t uiBlock)
{
    std::cout << "CDataStore::ReadBlock 1"  << std::endl;

//    uint8_t uiBlock = uiBlock;
    std::cout << "CDataStore::ReadBlock 1 uiBlock "  <<  (int)(uiBlock) << std::endl;

    // ��������� ����� �� ������� ������?
    if (uiBlock >= MAX_BLOCKS_NUMBER)
    {
        std::cout << "CDataStore::ReadBlock 2"  << std::endl;
        // ��� ������.
        return 0;
    }

    uint16_t uiLength;
    uint16_t uiEncodedLength;
    uint16_t uiSourceOffset;

//    for(int i=0; i<100; i++)
//    {
//        // ���� ����������?
//        if ((m_xServiseSection.xServiseSectionData.
//                axBlockPositionData[i].uiLength != 0) &&
//                (m_xServiseSection.xServiseSectionData.
//                 axBlockPositionData[i].uiEncodedLength != 0))
//        {
//            std::cout << "CDataStore::ReadBlock Block ok " << i << std::endl;
//        }
//        else
//        {
//            std::cout << "CDataStore::ReadBlock Block error " << i << std::endl;
//        }
//    }

    // ���� ����������?
    if ((m_xServiseSection.xServiseSectionData.
            axBlockPositionData[uiBlock].uiLength != 0) &&
            (m_xServiseSection.xServiseSectionData.
             axBlockPositionData[uiBlock].uiEncodedLength != 0))
    {
        std::cout << "CDataStore::ReadBlock 3"  << std::endl;
        // ������� ����� ����� � EEPROM.
        uiSourceOffset = m_xServiseSection.xServiseSectionData.
                         axBlockPositionData[uiBlock].uiOffset;
        // ������� ������ �����.
        uiLength = m_xServiseSection.xServiseSectionData.
                   axBlockPositionData[uiBlock].uiLength;
        // ������� ������ ��������������� �����.
        uiEncodedLength = m_xServiseSection.xServiseSectionData.
                          axBlockPositionData[uiBlock].uiEncodedLength;
    }
    else
    {
        std::cout << "CDataStore::ReadBlock 4"  << std::endl;
        // ��� ������.
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

    // ��������� �������������� ������.
    // ��� ������ ������ �������� ������?
    if (!(m_pxStorageDevice -> ReadBlock(pxDataContainer)))
    {
        std::cout << "CDataStore::ReadBlock 5"  << std::endl;
        // ��� ������.
        return 0;
    }

    // ���������� ����������� ������.
    CHammingCodes::HammingCodesToBytes(m_puiIntermediateBuff,
                                       m_puiIntermediateBuff,
                                       uiEncodedLength);

    // ���� �� ��������?
    if (m_xServiseSection.xServiseSectionData.
            axBlockPositionData[uiBlock].uiCrc ==
            usCrc16(m_puiIntermediateBuff, uiLength))
    {
        std::cout << "CDataStore::ReadBlock 6"  << std::endl;
        memcpy(puiDestination, m_puiIntermediateBuff, uiLength);
        return uiLength;
    }
    else
    {
        std::cout << "CDataStore::ReadBlock 7"  << std::endl;
        // ��� ������.
        return 2;
    }
}

//-------------------------------------------------------------------------------
// ���������� ������ ���� ���� ������ ������������ �������������.
void CDataStore::CrcOfBlocksCrcCreate(void)
{
    std::cout << "CDataStore::CrcOfBlocksCrcCreate 1"  << std::endl;
    // ����������� ����� ����������� �� ������� ����������� ���� ������, �� ������� ���������.
    // ����������� ��� ������ � ����������� ������� ����� ������ ����� ������������.
    // ���� ��� � ������������ ��������������� � ���, ��� ���� ������ ������� �� ��������,
    // � �� ������������ �������������. � ���� ������ ������ ��������� � ����� ������������ �� ������,
    // ������ ������������ ��� ������ ���� ������.
    uint16_t auiBlocksCrc[MAX_BLOCKS_NUMBER];

    // ������� Crc ���� ������.
    for (uint16_t i = 0;
            i < MAX_BLOCKS_NUMBER;
            i++)
    {
        auiBlocksCrc[i] =
            m_xServiseSection.xServiseSectionData.axBlockPositionData[i].uiCrc;
    }

    // �������� Crc ���� ������.
    m_xServiseSection.xServiseSectionData.uiCrcOfBlocksCrc =
        usCrc16(reinterpret_cast<uint8_t*>(auiBlocksCrc),
                (MAX_BLOCKS_NUMBER * sizeof(uint16_t)));

}

//-------------------------------------------------------------------------------
bool CDataStore::CrcOfBlocksCrcCheck(void)
{
    std::cout << "CDataStore::CrcOfBlocksCrcCheck 1"  << std::endl;
    // ����������� ����� ����������� �� ������� ����������� ���� ������, �� ������� ���������.
    // ����������� ��� ������ � ����������� ������� ����� ������ ����� ������������.
    // ���� ��� � ������������ ��������������� � ���, ��� ���� ������ ������� �� ��������,
    // � �� ������������ �������������. � ���� ������ ������ ��������� � ����� ������������ �� ������,
    // ������ ������������ ��� ������ ���� ������.
    uint16_t auiBlocksCrc[MAX_BLOCKS_NUMBER];

    // ������� Crc ���� ������.
    for (uint16_t i = 0;
            i < MAX_BLOCKS_NUMBER;
            i++)
    {
        auiBlocksCrc[i] =
            m_xServiseSection.xServiseSectionData.axBlockPositionData[i].uiCrc;
    }

    // Crc �� Crc ���� ������ ��������� � ���������� � ��������� �����?
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
}

//-------------------------------------------------------------------------------
// ������� ������� ������ ���������.
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
    // ������ ����� �� ��������� �����.
    case START_WRITE_TEMPORARY_BLOCK_DATA:
        std::cout << "CDataStore::Fsm 1"  << std::endl;
        TemporaryBlockWritePrepare();
        m_uiRequestRetryCounter = 0;
        // ��������� ����� �������� ��������� ������.
        GetTimerPointer() -> Set(WRITE_END_WAITING_TIMEOUT);
        SetFsmState(WRITE_END_WAITING_TEMPORARY_BLOCK_DATA);
        break;

    case WRITE_END_WAITING_TEMPORARY_BLOCK_DATA:
//        std::cout << "CDataStore::Fsm 2"  << std::endl;
        // ������� ��������� ������ ��������� ���������� ��������.
        // ���������� �������� ��������� ������ �������?
        if (m_pxStorageDevice -> IsDoneOk())
        {
            std::cout << "CDataStore::Fsm 3"  << std::endl;
            // ������ ������ �������?
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
        // ���������� �������� ��������� ������ �� �������?
        else if (m_pxStorageDevice -> IsDoneError())
        {
            std::cout << "CDataStore::Fsm 4"  << std::endl;
            SetFsmState(WRITE_ERROR);
        }
        else
        {
            // ����� �������� ��������� ������ �����������?
            if (GetTimerPointer() -> IsOverflow())
            {
                std::cout << "CDataStore::Fsm 5"  << std::endl;
                SetFsmState(WRITE_ERROR);
            }
        }
        break;


//-------------------------------------------------------------------------------
    // ������ ���������� ���������� �����.
    case START_WRITE_TEMPORARY_SERVICE_SECTION_DATA:
        std::cout << "CDataStore::Fsm 6"  << std::endl;
        TemporaryServiceSectionWritePrepare();
        // ��������� ����� �������� ��������� ������.
        GetTimerPointer() -> Set(WRITE_END_WAITING_TIMEOUT);
        SetFsmState(WRITE_END_WAITING_TEMPORARY_SERVICE_SECTION_DATA);
        break;

    case WRITE_END_WAITING_TEMPORARY_SERVICE_SECTION_DATA:
        // ������� ��������� ������ ��������� ���������� ��������.
        // ���������� �������� ��������� ������ �������?
        if (m_pxStorageDevice -> IsDoneOk())
        {
            std::cout << "CDataStore::Fsm 7"  << std::endl;
            // ������ ������ �������?
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
        // ���������� �������� ��������� ������ �� �������?
        else if (m_pxStorageDevice -> IsDoneError())
        {
            std::cout << "CDataStore::Fsm 10"  << std::endl;
            SetFsmState(WRITE_ERROR);
        }
        else
        {
            // ����� �������� ��������� ������ �����������?
            if (GetTimerPointer() -> IsOverflow())
            {
                std::cout << "CDataStore::Fsm 12"  << std::endl;
//                m_pxStorageDevice -> SetFsmOperationStatus(0);
                SetFsmState(WRITE_ERROR);
            }
        }
        break;


//-------------------------------------------------------------------------------
    // ������ �����.
    case START_WRITE_BLOCK_DATA:
        std::cout << "CDataStore::Fsm 13"  << std::endl;
        BlockWritePrepare();
        // ��������� ����� �������� ��������� ������.
        GetTimerPointer() -> Set(WRITE_END_WAITING_TIMEOUT);
        SetFsmState(WRITE_END_WAITING_BLOCK_DATA);
        break;

    case WRITE_END_WAITING_BLOCK_DATA:
        // ������� ��������� ������ ��������� ���������� ��������.
        // ���������� �������� ��������� ������ �������?
        if (m_pxStorageDevice -> IsDoneOk())
        {
            std::cout << "CDataStore::Fsm 14"  << std::endl;
            // ������ ������ �������?
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
        // ���������� �������� ��������� ������ �� �������?
        else if (m_pxStorageDevice -> IsDoneError())
        {
            std::cout << "CDataStore::Fsm 17"  << std::endl;
            SetFsmState(WRITE_ERROR);
        }
        else
        {
            // ����� �������� ��������� ������ �����������?
            if (GetTimerPointer() -> IsOverflow())
            {
                std::cout << "CDataStore::Fsm 18"  << std::endl;
                SetFsmState(WRITE_ERROR);
            }
        }
        break;


//-------------------------------------------------------------------------------
    // ������ ���������� �����.
    case START_WRITE_SERVICE_SECTION_DATA:
        std::cout << "CDataStore::Fsm 19"  << std::endl;
        ServiceSectionWritePrepare();
        // ��������� ����� �������� ��������� ������.
        GetTimerPointer() -> Set(WRITE_END_WAITING_TIMEOUT);
        SetFsmState(WRITE_END_WAITING_SERVICE_SECTION_DATA);
        break;

    case WRITE_END_WAITING_SERVICE_SECTION_DATA:
        // ������� ��������� ������ ��������� ���������� ��������.
        // ���������� �������� ��������� ������ �������?
        if (m_pxStorageDevice -> IsDoneOk())
        {
            std::cout << "CDataStore::Fsm 20"  << std::endl;
            // ������ ������ �������?
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
        // ���������� �������� ��������� ������ �� �������?
        else if (m_pxStorageDevice -> IsDoneError())
        {
            std::cout << "CDataStore::Fsm 23"  << std::endl;
            SetFsmState(WRITE_ERROR);
        }
        else
        {
            // ����� �������� ��������� ������ �����������?
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












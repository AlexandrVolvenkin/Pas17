
//-------------------------------------------------------------------------------
//  Source      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-------------------------------------------------------------------------------
#include <iostream>
#include <string.h>

#include "Task.h"
#include "Resources.h"
#include "StorageDevice.h"
#include "DataStore.h"
#include "Crc.h"
#include "HammingCodes.h"


// (sizeof(struct TAnalogueInputDescriptionDataBase) * ANALOG_MODULE_INPUT_QUANTITY)// 28�6=168.
#define ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_LENGTH 168
// (sizeof(struct TMrXXOneChannelDataBase) * DISCRETE_OUTPUT_MODULE_RELAY_OUTPUT_QUANTITY)// 3�64=192.
#define DISCRETE_OUTPUT_MODULE_DATA_BASE_BLOCK_LENGTH 192
// (sizeof(struct TMbmFunction1234PackOne) * EXTERNAL_MODULE_FUNCTION_1234_QUANTITY) + 1// 13�16=208.
#define MBM_FUNCTION_1234_BLOCK_DATA_BASE_BLOCK_LENGTH (208 + 1)
// (sizeof(struct TMbmFunction15Sourse) * EXTERNAL_MODULE_FUNCTION_15_QUANTITY)// 72�3=216.
#define MBM_FUNCTION_15_BLOCK_DATA_BASE_BLOCK_LENGTH 216
// (sizeof(struct TDiscreteInputDescriptionWork) * DISCRETE_INPUT_SYGNALS_DATA_BASE_BLOCKS_IN_BLOCK_QUANTITY)// 11�18=198.
#define DISCRETE_INPUT_SYGNALS_DATA_BASE_BLOCK_LENGTH 192
// (sizeof(struct TAnalogueOutputRegulatorDescriptionDataBase) * ANALOG_OUTPUT_MODULE_REGULATORS_QUANTITY)// 28�6=168.
#define CURRENT_OUTPUT_MODULE_REGULATOR_DATA_BASE_BLOCK_LENGTH 56
#define CURRENT_OUTPUT_MODULE_PSP_DATA_BASE_BLOCK_LENGTH 56
// DIMENSIONS_PARAMETERS_DATA_BLOCKS_IN_BLOCK_QUANTITY * 6// 6�16=96.
#define DIMENSIONS_PARAMETERS_DATA_BASE_BLOCK_LENGTH 96
// TEXT_TITLES_DATA_BASE_BLOCKS_IN_BLOCK_QUANTITY * TEXT_TITLES_DATA_LENGTH// (384 ��, 24 �����).
#define TEXT_TITLES_DATA_BASE_BLOCK_LENGTH 224
// (sizeof(struct TFunctionBlockDescriptionWork) * FUNCTION_BLOCK_DATA_BASE_BLOCKS_IN_BLOCK_QUANTITY)// 19�10=190.
#define FUNCTION_BLOCK_DATA_BASE_BLOCK_LENGTH 160// 19�10=190 (120 ���, 12 ������)
#define MATHEMATICAL_BLOCK_DATA_BASE_BLOCK_LENGTH 208// 82 � 84 (48 ���, 3 ����� 13*16=208 )
#define DISCRETE_INPUT_MODULE_DATA_BASE_BLOCK_LENGTH 32// 2�16=32.
#define NETWORK_ADDRESS_DATA_BASE_BLOCK_LENGTH 1
#define RESERVED_DATA_BASE_BLOCK_LENGTH 1
#define REFERENCE_POINTS_ADC_CODES_DATA_BASE_BLOCK_LENGTH 42

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
    m_puiIntermediateBuff = new uint8_t[MAX_ENCODED_BLOCK_LENGTH];
    SetFsmState(IDDLE);
//    SetSavedFsmState(IDDLE);
}

//-------------------------------------------------------------------------------
CDataStore::CDataStore(CStorageDeviceInterface* pxStorageDevice) :
    m_pxStorageDevice(pxStorageDevice)
{
    m_puiIntermediateBuff = new uint8_t[MAX_ENCODED_BLOCK_LENGTH];
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
    SetFsmState(IDDLE);

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
    // �������� ����������� �����.
    m_xServiseSection.uiCrc =
        usCrc16(reinterpret_cast<uint8_t*>(&m_xServiseSection.xServiseSectionData),
                sizeof(struct TServiseSectionData));

    // ���������� ������ ���������� ��������.
    uint16_t uiEncodedByteCounter =
        CHammingCodes::BytesToHammingCodes(m_puiIntermediateBuff,
                                           reinterpret_cast<uint8_t*>(&m_xServiseSection),
                                           sizeof(struct TServiseSection));

    if (m_pxStorageDevice -> WriteBlock(m_puiIntermediateBuff,
                                        TEMPORARY_SERVICE_SECTION_DATA_BEGIN,
                                        uiEncodedByteCounter))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

//-------------------------------------------------------------------------------
// ������� ������ ���������� ����� � ������.
// �������� �� ��������� �������� �
// ������� �������� ������ ���������� ��������.
uint8_t CDataStore::ServiceSectionWritePrepare(void)
{
    // �������� ����������� �����.
    m_xServiseSection.uiCrc =
        usCrc16(reinterpret_cast<uint8_t*>(&m_xServiseSection.xServiseSectionData),
                sizeof(struct TServiseSectionData));

    // ���������� ������ ���������� ��������.
    uint16_t uiEncodedByteCounter =
        CHammingCodes::BytesToHammingCodes(m_puiIntermediateBuff,
                                           reinterpret_cast<uint8_t*>(&m_xServiseSection),
                                           sizeof(struct TServiseSection));

    if (m_pxStorageDevice -> WriteBlock(m_puiIntermediateBuff,
                                        SERVICE_SECTION_DATA_BEGIN,
                                        uiEncodedByteCounter))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

//-------------------------------------------------------------------------------
// ��������� � ��������� ����������� ���������� ���������� �����.
uint8_t CDataStore::ReadTemporaryServiceSection(void)
{
    uint16_t uiEncodedLength =
        (CHammingCodes::CalculateEncodedDataLength(sizeof(struct TServiseSection)));

    // ��������� �������������� ������.
    // ��� ������ ������ �������� ������?
    if (!(m_pxStorageDevice -> ReadBlock(m_puiIntermediateBuff,
                                         TEMPORARY_SERVICE_SECTION_DATA_BEGIN,
                                         uiEncodedLength)))
    {
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
        return 1;
    }
    else
    {
        // ��� ������.
        return 0;
    }
}

//-------------------------------------------------------------------------------
// ��������� � ��������� ����������� ���������� �����.
uint8_t CDataStore::ReadServiceSection(void)
{
    uint16_t uiEncodedLength =
        (CHammingCodes::CalculateEncodedDataLength(sizeof(struct TServiseSection)));

    // ��������� �������������� ������.
    // ��� ������ ������ �������� ������?
    if (!(m_pxStorageDevice -> ReadBlock(m_puiIntermediateBuff,
                                         SERVICE_SECTION_DATA_BEGIN,
                                         uiEncodedLength)))
    {
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
        return 1;
    }
    else
    {
        // ��� ������.
        return 0;
    }
}

//-------------------------------------------------------------------------------
// ��������� � ��������� ����������� ���������� ������ �����.
uint8_t CDataStore::CheckTemporaryBlock(void)
{
    uint8_t uiBlock = m_uiBlock;

    // ��������� ����� �� ������� ������?
    if (uiBlock >= MAX_BLOCKS_NUMBER)
    {
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
        // ��� ������.
        return 0;
    }

    // ��������� �������������� ������.
    // ��� ������ ������ �������� ������?
    if (!(m_pxStorageDevice -> ReadBlock(m_puiIntermediateBuff,
                                         TEMPORARY_BLOCK_DATA_BEGIN,
                                         uiEncodedLength)))
    {
        // ��� ������.
        return 0;
    }

    // ���������� ����������� ������.
    CHammingCodes::HammingCodesToBytes(m_puiIntermediateBuff, m_puiIntermediateBuff, uiEncodedLength);

    // ���� �� ��������?
    if (m_xServiseSection.xServiseSectionData.
            axBlockPositionData[uiBlock].uiCrc ==
            usCrc16(m_puiIntermediateBuff, uiLength))
    {
        return 1;
    }
    else
    {
        // ��� ������.
        return 0;
    }
}

//-------------------------------------------------------------------------------
// ��������� � ��������� ����������� �����.
uint8_t CDataStore::CheckBlock(void)
{
    uint8_t uiBlock = m_uiBlock;

    // ��������� ����� �� ������� ������?
    if (uiBlock >= MAX_BLOCKS_NUMBER)
    {
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
        // ��� ������.
        return 0;
    }

    // ��������� �������������� ������.
    // ��� ������ ������ �������� ������?
    if (!(m_pxStorageDevice -> ReadBlock(m_puiIntermediateBuff,
                                         uiSourceOffset,
                                         uiEncodedLength)))
    {
        // ��� ������.
        return 0;
    }

    // ���������� ����������� ������.
    CHammingCodes::HammingCodesToBytes(m_puiIntermediateBuff, m_puiIntermediateBuff, uiEncodedLength);

    // ���� �� ��������?
    if (m_xServiseSection.xServiseSectionData.
            axBlockPositionData[uiBlock].uiCrc ==
            usCrc16(m_puiIntermediateBuff, uiLength))
    {
        return 1;
    }
    else
    {
        // ��� ������.
        return 0;
    }
}

//-------------------------------------------------------------------------------
// ��������� � ��������� ����������� �����.
uint16_t CDataStore::ReadBlock(uint8_t *puiDestination, uint8_t uiBlock)
{
    // ��������� ����� �� ������� ������?
    if (uiBlock >= MAX_BLOCKS_NUMBER)
    {
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
        // ��� ������.
        return 0;
    }

    // ��������� �������������� ������.
    m_pxStorageDevice -> ReadBlock(m_puiIntermediateBuff, uiSourceOffset, uiEncodedLength);

    // ���������� ����������� ������.
    CHammingCodes::HammingCodesToBytes(m_puiIntermediateBuff, m_puiIntermediateBuff, uiEncodedLength);

    // ���� �� ��������?
    if (m_xServiseSection.xServiseSectionData.
            axBlockPositionData[uiBlock].uiCrc ==
            usCrc16(m_puiIntermediateBuff, uiLength))
    {
        memcpy(puiDestination, m_puiIntermediateBuff, uiLength);
        return uiLength;
    }
    else
    {
        // ��� ������.
        return 0;
    }
}

//-------------------------------------------------------------------------------
// ������� ������ ����� � ������ �� ��������� �����.
// �������� �� ��������� ��������, ��������� ����� ��� �������� �
// ������� �������� ������ ���������� ��������.
uint8_t CDataStore::TemporaryBlockWritePrepare(void)
{
    uint8_t uiBlock = m_uiBlock;
    uint8_t* puiSource = m_puiBlockSource;
    uint16_t uiLength = m_uiBlockLength;

    // ���������� ������ ���������� ��������.
    uint16_t uiEncodedByteCounter =
        CHammingCodes::BytesToHammingCodes(m_puiIntermediateBuff, puiSource, uiLength);

    // ������ �������� ����� ������ - uiBlock ����������� �������?
    if ((m_xServiseSection.xServiseSectionData.
            axBlockPositionData[uiBlock].uiLength == 0) &&
            (m_xServiseSection.xServiseSectionData.
             axBlockPositionData[uiBlock].uiEncodedLength == 0))
    {
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

    // �������� ����������� ����� ����������� ������.
    m_xServiseSection.xServiseSectionData.
    axBlockPositionData[uiBlock].uiCrc =
        usCrc16(puiSource, uiLength);

    if (m_pxStorageDevice -> WriteBlock(m_puiIntermediateBuff,
                                        TEMPORARY_BLOCK_DATA_BEGIN,
                                        uiEncodedByteCounter))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

//-------------------------------------------------------------------------------
// ������� ������ ����� � ������.
// �������� �� ��������� ��������, ��������� ����� ��� �������� �
// ������� �������� ������ ���������� ��������.
uint8_t CDataStore::BlockWritePrepare(void)
{
    uint8_t uiBlock = m_uiBlock;
    uint8_t* puiSource = m_puiBlockSource;
    uint16_t uiLength = m_uiBlockLength;

    // ���������� ������ ���������� ��������.
    uint16_t uiEncodedByteCounter =
        CHammingCodes::BytesToHammingCodes(m_puiIntermediateBuff, puiSource, uiLength);

    // ������ �������� ����� ������ - uiBlock ����������� �������?
    if ((m_xServiseSection.xServiseSectionData.
            axBlockPositionData[uiBlock].uiLength == 0) &&
            (m_xServiseSection.xServiseSectionData.
             axBlockPositionData[uiBlock].uiEncodedLength == 0))
    {
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

    if (m_pxStorageDevice -> WriteBlock(m_puiIntermediateBuff,
                                        m_xServiseSection.xServiseSectionData.
                                        axBlockPositionData[uiBlock].uiOffset,
                                        uiEncodedByteCounter))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

//-------------------------------------------------------------------------------
// ������ ����� � ��������� �������� ��������.
// 1 - CDataStore::WriteBlock(uint8_t *puiSource, uint16_t uiLength, uint8_t uiBlock).
// 2 - CDataStore::BlockWritePrepare(void).
// 3 - CStorageDevice::WriteBlock(uint16_t uiOffset, uint8_t *puiSource, uint16_t uiLength).

// ������� ������ ��������� ������ ����� �������� ��������� ������ � ��������� ������� ������.
uint8_t CDataStore::WriteBlock(uint8_t *puiSource, uint16_t uiLength, uint8_t uiBlock)
{
    // ������� �� ����� � ������?
    if ((GetFsmState() != IDDLE) ||
            (GetFsmState() != DATA_WRITED_SUCCESSFULLY) ||
            (GetFsmState() != WRITE_ERROR))
    {
//        SetFsmEvent(WRITE_ERROR_FSM_EVENT);
        return 0;
    }

    // ��������� ����� �� ������� ������?
    if (uiBlock >= MAX_BLOCKS_NUMBER)
    {
        // ��� ������.
        return 0;
    }

    // ������� ������ ��������� ������ �����.
    m_puiBlockSource = puiSource;
    m_uiBlockLength = uiLength;
    m_uiBlock = uiBlock;

//    SetFsmEvent(WRITE_IN_PROGRESS_FSM_EVENT);
    // �������� ������� ������.
    SetFsmState(START_WRITE_TEMPORARY_BLOCK_DATA);
}

//-------------------------------------------------------------------------------
// ���������� ������ ���� ���� ������ ������������ �������������.
void CDataStore::CrcOfBlocksCrcCreate(void)
{
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

////-------------------------------------------------------------------------------
//// ��������� ����������� ������ ���������.
//// ��������������� ����������� ������ � ������� ��������� ��������.
//// ��������������� ������ ������������� ����� ��� ���� ������� � �.�.
//uint8_t CDataStore::Check(void)
//{
//    enum
//    {
//        IDDLE = 0,
//
//        TEMPORARY_SERVICE_SECTION_DATA_CHECK,
//        TEMPORARY_SERVICE_SECTION_LINKED_BLOCKS_CHECK,
//
//        CORRUPTED_BLOCK_RECOVERY_WRITE_END_WAITING,
//        SERVICE_SECTION_DATA_WRITE_START,
//        SERVICE_SECTION_DATA_WRITE_END_WAITING,
//
//        SERVICE_SECTION_DATA_CHECK,
//        SERVICE_SECTION_LINKED_BLOCKS_CHECK,
//
//        DATA_STORE_NEW_VERSION_ACCEPTED,
//        DATA_STORE_OLD_VERSION_ACCEPTED,
//
//        DATA_STORE_CHECK_OK,
//        DATA_STORE_CHECK_ERROR,
//        DATA_STORE_CHECK_REPEAT,
//    };
//
//    enum
//    {
//        // ���������� ������� �������������� ���������.
//        RECOVERY_ATTEMPTS_NUMBER = 3,
//    };
//
//    uint8_t uiLocalFsmState;
//    uint8_t uiRecoveryAttemptCounter = 0;
//    uint8_t auiTempArray[MAX_BLOCK_LENGTH];
//
//    // ������� ������ ������������� ���������� ��������.
//    CHammingCodes::SetErrorCode(CHammingCodes::NONE_ERROR);
//    // �������� ������� �������� � �������������� ���������.
//    uiLocalFsmState = TEMPORARY_SERVICE_SECTION_DATA_CHECK;
//
//    while (1)
//    {
//        switch (uiLocalFsmState)
//        {
//        case IDDLE:
//            break;
//
//        case TEMPORARY_SERVICE_SECTION_DATA_CHECK:
//            // ��������� ��������� ���� �� ��������?
//            if (ReadTemporaryServiceSection())
//            {
//                uiLocalFsmState = TEMPORARY_SERVICE_SECTION_LINKED_BLOCKS_CHECK;
//            }
//            else
//            {
//                // ��������� ��������� ���� ��������, �������� �� ����� ���������� ������ ������.
//                // �������� ����������� ��������� �� ����������� ���������� �����.
//                uiLocalFsmState = SERVICE_SECTION_DATA_CHECK;
//            }
//            break;
//
//        case TEMPORARY_SERVICE_SECTION_LINKED_BLOCKS_CHECK:
//            // �������� ����������� ������ ��������� � ��������� ��������� ������, � ������������� �� �����������.
//            for (uint16_t i = 0;
//                    i < m_xServiseSection.xServiseSectionData.uiStoredBlocksNumber;
//                    i++)
//            {
//                // ���� �� ������ � ��������� ��������� �������(��� ��������)?
//                if (!(ReadBlock(auiTempArray, i)))
//                {
//                    // ��������� ������ �����, � ���������� Crc ��������,
//                    // ����� ���������� Crc ����� ���������� �� ��������� ������.
//                    m_uiBlock = i;
//                    // Crc ����� �� ���������� ������ ��������� � Crc �����
//                    // ����������� �� ��������� ��������� ������ �� �������� �������?
//                    if (CheckTemporaryBlock())
//                    {
//                        // ��������� ������ ����� ��������� ��� �������� �� ��������������� �����.
//                        memcpy(auiTempArray,
//                               m_puiIntermediateBuff,
//                               m_xServiseSection.xServiseSectionData.
//                               axBlockPositionData[i].uiLength);
//                        // ������ ����� ������� �������� �� ��������� ������,
//                        // �� ��� ������ � ��������� ��������� ����.
//                        // ��������� ��������� ������ ������ ����� �� ��������� ������� � ���������.
//                        WriteBlock(auiTempArray,
//                                   m_xServiseSection.xServiseSectionData.
//                                   axBlockPositionData[i].uiLength,
//                                   i);
//
//                        uiLocalFsmState = CORRUPTED_BLOCK_RECOVERY_WRITE_END_WAITING;
//                        break;
//                    }
//                    else
//                    {
//                        // ���� �� ������ � ��������� ��������� �������.
//                        // �������� ��������� ������ �� ����� ������ ���������� ���������� �����.
//                        // ��������� ��������.
//                        uiLocalFsmState = SERVICE_SECTION_DATA_CHECK;
//                        break;
//                    }
//                }
//                else
//                {
//                    // ���� ������������ ���������� �������� ����� ����������� ������?
//                    if (CHammingCodes::GetErrorCode() != CHammingCodes::NONE_ERROR)
//                    {
//                        // ������� ������ ������������� ���������� ��������.
//                        CHammingCodes::SetErrorCode(CHammingCodes::NONE_ERROR);
//                        cout << "CHammingCodes::GetErrorCode 1 uiBlock" << (int)i << endl;
//
//                        // ��������� ��������� ������ ������ � ���������.
//                        WriteBlock(auiTempArray,
//                                   m_xServiseSection.xServiseSectionData.
//                                   axBlockPositionData[i].uiLength,
//                                   i);
//
//                        uiLocalFsmState = CORRUPTED_BLOCK_RECOVERY_WRITE_END_WAITING;
//                        break;
//                    }
//                }
//
//                // �������.
//                uiLocalFsmState = SERVICE_SECTION_DATA_WRITE_START;
//            }
//            break;
//
//        case CORRUPTED_BLOCK_RECOVERY_WRITE_END_WAITING:
//            // ������� ��������� ������ ��������� ���������.
//            // ���������� �� ��������� ������ ���� ������� � ���������?
//            if (GetFsmEvent() == CDataStore::WRITE_OK_FSM_EVENT)
//            {
//                uiLocalFsmState = DATA_STORE_CHECK_REPEAT;
//            }
//            // ��� ������ ����� ��������� ������?
//            else if (GetFsmEvent() == CDataStore::WRITE_ERROR_FSM_EVENT)
//            {
//                uiLocalFsmState = DATA_STORE_CHECK_REPEAT;
//            }
//            break;
//
//        case SERVICE_SECTION_DATA_WRITE_START:
//            // ����� ��������� � ���������� ���������� �����, ������������� ����.
//            // ��� �������, ��������� ����� ������ �� ��������� ������ ������ ������.
//            // �� ����� ��������� ����������� � ���������� ���������� �����, ������� �����.
//            // ������� ��������� ����.
//            SetFsmEvent(WRITE_IN_PROGRESS_FSM_EVENT);
//            // �������� ������� ������ ���������� �����.
//            SetFsmState(START_WRITE_SERVICE_SECTION_DATA);
//            uiLocalFsmState = SERVICE_SECTION_DATA_WRITE_END_WAITING;
//            break;
//
//        case SERVICE_SECTION_DATA_WRITE_END_WAITING:
//            // ������� ��������� ������ ��������� ���������.
//            // ��������� ���� ������� � ���������?
//            if (GetFsmEvent() == CDataStore::WRITE_OK_FSM_EVENT)
//            {
//                // ��������� ���� �� ��������?
//                if (ReadServiceSection())
//                {
//                    uiLocalFsmState = DATA_STORE_NEW_VERSION_ACCEPTED;
//                }
//                else
//                {
//                    uiLocalFsmState = DATA_STORE_CHECK_REPEAT;
//                }
//            }
//            // ��� ������ ����� ��������� ������?
//            else if (GetFsmEvent() == CDataStore::WRITE_ERROR_FSM_EVENT)
//            {
//                uiLocalFsmState = DATA_STORE_CHECK_REPEAT;
//            }
//            break;
//
//        case SERVICE_SECTION_DATA_CHECK:
//            // �� ����� ���� ��������� ��������� ���� �� ������ � ������� �������� ������(��� ��������).
//            // ��������� ���� �� ����� ������. ���������� ������� ���������� ��������� ���������.
//            // ��������� ���� �� ��������?
//            if (ReadServiceSection())
//            {
//                uiLocalFsmState = SERVICE_SECTION_LINKED_BLOCKS_CHECK;
//            }
//            else
//            {
//                uiLocalFsmState = DATA_STORE_CHECK_ERROR;
//            }
//            break;
//
//        case SERVICE_SECTION_LINKED_BLOCKS_CHECK:
//            // �������� ����������� ������ ��������� �� ��������� ������, � ������������� �� �����������.
//            for (uint16_t i = 0;
//                    i < m_xServiseSection.xServiseSectionData.uiStoredBlocksNumber;
//                    i++)
//            {
//                // ���� �� ������ �� ��������� �������(��� ��������)?
//                if (!(ReadBlock(auiTempArray, i)))
//                {
//                    // ���� �� ������ �� ��������� �������.
//                    // �������� ��������� ������ �� ����� ������ ���������� �����.
//                    // ������������ ������ ������.
//                    uiLocalFsmState = DATA_STORE_CHECK_ERROR;
//                    break;
//                }
//                else
//                {
//                    // ���� ������������ ���������� �������� ����� ����������� ������?
//                    if (CHammingCodes::GetErrorCode() != CHammingCodes::NONE_ERROR)
//                    {
//                        // ������� ������ ������������� ���������� ��������.
//                        CHammingCodes::SetErrorCode(CHammingCodes::NONE_ERROR);
//                        cout << "CHammingCodes::GetErrorCode 2 uiBlock" << (int)i << endl;
//
//                        // ��������� ��������� ������ ������ � ���������.
//                        WriteBlock(auiTempArray,
//                                   m_xServiseSection.xServiseSectionData.
//                                   axBlockPositionData[i].uiLength,
//                                   i);
//
//                        uiLocalFsmState = CORRUPTED_BLOCK_RECOVERY_WRITE_END_WAITING;
//                        break;
//                    }
//                }
//
//                // �������.
//                // ����� ��������� � ���������� �����, ������������� ����.
//                // ����������� ���������� ����� ���������.
//                uiLocalFsmState = DATA_STORE_OLD_VERSION_ACCEPTED;
//            }
//            break;
//
//        case DATA_STORE_NEW_VERSION_ACCEPTED:
//            // ��������� ���������.
//            cerr << "DATA_STORE_NEW_VERSION_ACCEPTED" << endl;
//            return 1;
//            break;
//
//        case DATA_STORE_OLD_VERSION_ACCEPTED:
//            // ��������� �� ���������.
//            cerr << "DATA_STORE_OLD_VERSION_ACCEPTED" << endl;
//            return 1;
//            break;
//
//        case DATA_STORE_CHECK_OK:
//            return 1;
//            break;
//
//        case DATA_STORE_CHECK_ERROR:
//            // ��������� ����������.
//            cerr << "DATA_STORE_CHECK_ERROR" << endl;
//            return 0;
//            break;
//
//        case DATA_STORE_CHECK_REPEAT:
//            // ��������� �������� � ������� ��������������.
//            // ��� ���� ����������� ��� ��������������?
//            if (uiRecoveryAttemptCounter < RECOVERY_ATTEMPTS_NUMBER)
//            {
//                uiRecoveryAttemptCounter++;
//                // �������� �������� ��������� ����� ��������������.
//                uiLocalFsmState = TEMPORARY_SERVICE_SECTION_DATA_CHECK;
//            }
//            else
//            {
//                uiLocalFsmState = DATA_STORE_CHECK_ERROR;
//            }
//            break;
//
//        default:
//            return 0;
//            break;
//        }
//
//        Fsm();
//    }
//}

//-------------------------------------------------------------------------------
// ������� ������� ������ ���������.
uint8_t CDataStore::Fsm(void)
{
    switch (GetFsmState())
    {
    case IDDLE:
        break;

    case START:
        std::cout << "CDataStore::Fsm START"  << std::endl;
        std::cout << "CDataStore::Fsm m_sStorageDeviceName" << " " << (m_sStorageDeviceName) << std::endl;
        SetStorageDevice((CStorageDeviceInterface*)
                         (GetResources() ->
                          GetCommonTaskFromMapPointer(m_sStorageDeviceName)));
        SetFsmState(READY);
        break;

    case READY:
        std::cout << "CDataStore::Fsm READY"  << std::endl;
        break;

    case STOP:
        std::cout << "CDataStore::Fsm STOP"  << std::endl;
        break;


//-------------------------------------------------------------------------------
    // ������ ����� �� ��������� �����.
    case START_WRITE_TEMPORARY_BLOCK_DATA:
//        SetFsmEvent(WRITE_IN_PROGRESS_FSM_EVENT);
//        // ��������� ����� �������� ���������� � ������.
//        GetTimerPointer() -> Set(READY_TO_WRITE_WAITING_TIMEOUT);
        SetFsmState(READY_TO_WRITE_WAITING_TEMPORARY_BLOCK_DATA);
        break;

    case READY_TO_WRITE_WAITING_TEMPORARY_BLOCK_DATA:
//        // ���������� �������� ������ � ������?
////        if (m_pxStorageDevice -> IsReadyToWrite())
//        if (m_pxStorageDevice -> Fsm() == CStorageDeviceInterface::IDDLE)
//        {
        // ��������� ����� �������� ��������� ������.
        GetTimerPointer() -> Set(WRITE_END_WAITING_TIMEOUT);
//            // ������� ���� - ������ ���������.
//            m_pxStorageDevice -> SetIsDataWrited(false);
        // ���������� � ������ �� ��������� ����� ������ �������?
        if (TemporaryBlockWritePrepare())
        {
            SetFsmState(WRITE_END_WAITING_TEMPORARY_BLOCK_DATA);
        }
        else
        {
//                SetFsmEvent(WRITE_ERROR_FSM_EVENT);
            SetFsmState(WRITE_ERROR);
        }
//        }
//        // ����� �������� ���������� � ������ �����������?
//        else if (GetTimerPointer() -> IsOverflow())
//        {
////                SetFsmEvent(WRITE_ERROR_FSM_EVENT);
//            SetFsmState(WRITE_ERROR);
//        }
        break;

    case WRITE_END_WAITING_TEMPORARY_BLOCK_DATA:
        // ������� ��������� ������ ��������� ���������� ��������.
        // ���������� �������� ��������� ������?
//        if (m_pxStorageDevice -> IsDataWrited())
        if (m_pxStorageDevice -> Fsm() ==
                CStorageDeviceInterface::DATA_WRITED_SUCCESSFULLY)
        {
            m_pxStorageDevice -> SetFsmState(CStorageDeviceInterface::IDDLE);
            // ������ ������ �������?
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
        // ����� �������� ��������� ������ �����������?
        else if (GetTimerPointer() -> IsOverflow())
        {
            m_pxStorageDevice -> SetFsmState(CStorageDeviceInterface::IDDLE);
//                SetFsmEvent(WRITE_ERROR_FSM_EVENT);
            SetFsmState(WRITE_ERROR);
        }
        break;


//-------------------------------------------------------------------------------
    // ������ ���������� ���������� �����.
    case START_WRITE_TEMPORARY_SERVICE_SECTION_DATA:
//        SetFsmEvent(WRITE_IN_PROGRESS_FSM_EVENT);
//        // ��������� ����� �������� ���������� � ������.
//        GetTimerPointer() -> Set(READY_TO_WRITE_WAITING_TIMEOUT);
        SetFsmState(READY_TO_WRITE_WAITING_TEMPORARY_SERVICE_SECTION_DATA);
        break;

    case READY_TO_WRITE_WAITING_TEMPORARY_SERVICE_SECTION_DATA:
//        // ���������� �������� ������ � ������?
////        if (m_pxStorageDevice -> IsReadyToWrite())
//        if (m_pxStorageDevice -> Fsm() == CStorageDeviceInterface::IDDLE)
//        {
        // ��������� ����� �������� ��������� ������.
        GetTimerPointer() -> Set(WRITE_END_WAITING_TIMEOUT);
//            // ������� ���� - ������ ���������.
//            m_pxStorageDevice -> SetIsDataWrited(false);
        // ���������� � ������ ������ �������?
        if (TemporaryServiceSectionWritePrepare())
        {
            SetFsmState(WRITE_END_WAITING_TEMPORARY_SERVICE_SECTION_DATA);
        }
        else
        {
//                SetFsmEvent(WRITE_ERROR_FSM_EVENT);
            SetFsmState(WRITE_ERROR);
        }
//        }
//        // ����� �������� ���������� � ������ �����������?
//        else if (GetTimerPointer() -> IsOverflow())
//        {
////                SetFsmEvent(WRITE_ERROR_FSM_EVENT);
//            SetFsmState(WRITE_ERROR);
//        }
        break;

    case WRITE_END_WAITING_TEMPORARY_SERVICE_SECTION_DATA:
        // ������� ��������� ������ ��������� ���������� ��������.
        // ���������� �������� ��������� ������?
//        if (m_pxStorageDevice -> IsDataWrited())
        if (m_pxStorageDevice -> Fsm() ==
                CStorageDeviceInterface::DATA_WRITED_SUCCESSFULLY)
        {
            m_pxStorageDevice -> SetFsmState(CStorageDeviceInterface::IDDLE);
            // ������ ������ �������?
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
        // ����� �������� ��������� ������ �����������?
        else if (GetTimerPointer() -> IsOverflow())
        {
            m_pxStorageDevice -> SetFsmState(CStorageDeviceInterface::IDDLE);
//                SetFsmEvent(WRITE_ERROR_FSM_EVENT);
            SetFsmState(WRITE_ERROR);
        }
        break;


//-------------------------------------------------------------------------------
    // ������ �����.
    case START_WRITE_BLOCK_DATA:
//        // ��������� ����� �������� ���������� � ������.
//        GetTimerPointer() -> Set(READY_TO_WRITE_WAITING_TIMEOUT);
        SetFsmState(READY_TO_WRITE_WAITING_BLOCK_DATA);
        break;

    case READY_TO_WRITE_WAITING_BLOCK_DATA:
//        // ���������� �������� ������ � ������?
////        if (m_pxStorageDevice -> IsReadyToWrite())
//        if (m_pxStorageDevice -> Fsm() == CStorageDeviceInterface::IDDLE)
//        {
        // ��������� ����� �������� ��������� ������ ������.
        GetTimerPointer() -> Set(WRITE_END_WAITING_TIMEOUT);
//            // ������� ���� - ������ ���������.
//            m_pxStorageDevice -> SetIsDataWrited(false);
        // ���������� � ������ ������ �������?
        if (BlockWritePrepare())
        {
            SetFsmState(WRITE_END_WAITING_BLOCK_DATA);
        }
        else
        {
//                SetFsmEvent(WRITE_ERROR_FSM_EVENT);
            SetFsmState(WRITE_ERROR);
        }
//        }
//        // ����� �������� ���������� � ������ �����������?
//        else if (GetTimerPointer() -> IsOverflow())
//        {
////                SetFsmEvent(WRITE_ERROR_FSM_EVENT);
//            SetFsmState(WRITE_ERROR);
//        }
        break;

    case WRITE_END_WAITING_BLOCK_DATA:
        // ������� ��������� ������ ��������� ���������� ��������.
        // ���������� �������� ��������� ������?
//        if (m_pxStorageDevice -> IsDataWrited())
        if (m_pxStorageDevice -> Fsm() ==
                CStorageDeviceInterface::DATA_WRITED_SUCCESSFULLY)
        {
            m_pxStorageDevice -> SetFsmState(CStorageDeviceInterface::IDDLE);
            // ������ ������ �������?
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
        // ����� �������� ��������� ������ �����������?
        else if (GetTimerPointer() -> IsOverflow())
        {
            m_pxStorageDevice -> SetFsmState(CStorageDeviceInterface::IDDLE);
//                SetFsmEvent(WRITE_ERROR_FSM_EVENT);
            SetFsmState(WRITE_ERROR);
        }
        break;


//-------------------------------------------------------------------------------
    // ������ ���������� �����.
    case START_WRITE_SERVICE_SECTION_DATA:
//        SetFsmEvent(WRITE_IN_PROGRESS_FSM_EVENT);
//        // ��������� ����� �������� ���������� � ������.
//        GetTimerPointer() -> Set(READY_TO_WRITE_WAITING_TIMEOUT);
        SetFsmState(READY_TO_WRITE_WAITING_SERVICE_SECTION_DATA);
        break;

    case READY_TO_WRITE_WAITING_SERVICE_SECTION_DATA:
//        // ���������� �������� ������ � ������?
////        if (m_pxStorageDevice -> IsReadyToWrite())
//        if (m_pxStorageDevice -> Fsm() == CStorageDeviceInterface::IDDLE)
//        {
        // ��������� ����� �������� ��������� ������.
        GetTimerPointer() -> Set(WRITE_END_WAITING_TIMEOUT);
//            // ������� ���� - ������ ���������.
//            m_pxStorageDevice -> SetIsDataWrited(false);
        // ���������� � ������ ������ �������?
        if (ServiceSectionWritePrepare())
        {
            SetFsmState(WRITE_END_WAITING_SERVICE_SECTION_DATA);
        }
        else
        {
//                SetFsmEvent(WRITE_ERROR_FSM_EVENT);
            SetFsmState(WRITE_ERROR);
        }
//        }
//        // ����� �������� ���������� � ������ �����������?
//        else if (GetTimerPointer() -> IsOverflow())
//        {
////                SetFsmEvent(WRITE_ERROR_FSM_EVENT);
//            SetFsmState(WRITE_ERROR);
//        }
        break;

    case WRITE_END_WAITING_SERVICE_SECTION_DATA:
        // ������� ��������� ������ ��������� ���������� ��������.
        // ���������� �������� ��������� ������?
//        if (m_pxStorageDevice -> IsDataWrited())
        if (m_pxStorageDevice -> Fsm() ==
                CStorageDeviceInterface::DATA_WRITED_SUCCESSFULLY)
        {
            m_pxStorageDevice -> SetFsmState(CStorageDeviceInterface::IDDLE);
            // ������ ������ �������?
            if (ReadServiceSection())
            {
                cerr << "ReadServiceSection ok" << endl;
//                SetFsmEvent(WRITE_OK_FSM_EVENT);
                SetFsmState(DATA_WRITED_SUCCESSFULLY);
            }
            else
            {
                cerr << "ReadServiceSection error" << endl;
//                SetFsmEvent(WRITE_ERROR_FSM_EVENT);
                SetFsmState(WRITE_ERROR);
            }
        }
        // ����� �������� ��������� ������ �����������?
        else if (GetTimerPointer() -> IsOverflow())
        {
            m_pxStorageDevice -> SetFsmState(CStorageDeviceInterface::IDDLE);
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
        SetFsmState(WRITE_ERROR);
        break;
    }

    return GetFsmState();
}

//-------------------------------------------------------------------------------












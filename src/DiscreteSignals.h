#ifndef DISCRETESIGNALS_H
#define DISCRETESIGNALS_H
//-------------------------------------------------------------------------------
//  Sourse      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-------------------------------------------------------------------------------

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
//#include <fcntl.h>
#include <string.h>
#include <assert.h>
#include <time.h>


class Timer;
class Platform;
class CTask;
class CResources;
class CCommunicationDevice;
class CCommunicationDeviceInterface;
class CSpiCommunicationDevice;
class CDataContainerInterface;
class CDataContainerDataBase;
class CInternalModule;
class CConfigurationCreate;
class CDeviceControl;
class CDataBaseCreate;
class CDiscreteSignals;

#define DISCRETE_INPUT_RELAY_OUTPUT_QUANTITY 4//DO_VALUE_BIT_ARRAY_LENGTH
// �����.
#define FLOW_CONTROL_NORMA 1
// ����� ���������.
#define FLOW_CONTROL_NEW_VIOLATION 2
// �� ����� ���������.
#define FLOW_CONTROL_NOT_NEW_VIOLATION 3


// ��� ������������, ������� ������ ���������� �� ����������� �������� ������.
#pragma pack(push)
#pragma pack(1)
// ��������� ���������� �� ���������� ������� ��������.
// ��������: PAS_05_PO.DOC, ��� - 3,
// 3. ���� ������ ��������� ���������� ��������
struct TDiscreteSignalsDescriptionWorkPackOne
{
    // ���� 1:
    // D0-D4 - ��� (������): 0,1-24(0-��� ���.�� �����), D5-�� (��� ��������): 0-��,1-��.
    // D6 - ���1 � ��� ������������ ��������� ����������� �������.
    // D7 - ���2 � ��� ������������ ��������� ����������� �������.
    uint8_t 	uiTalTkGrp;
    // ���� 2:
    // ������ �� ��1:D0=1 ���� ���.�� ���� 1,D0=0-��� ��� �� ���� 1 � �.�.
    // ���� 3,4,5,6,7,8,9:
    // ������ �� ��2,��3,��4,��5,MP6,MP7,MP8:������.��1.
    uint8_t auiRelayOut[8];
    // ���� 10:
    // D7 - ������, D6 - ���:0-������.� ���,1-� ���.���.
    // D0-D5 - ���:������� ������� �������� ������ � ��.���(0-60).
    uint8_t uiDelay;
    // ���� 11 - ����������� ����� ($ff+����� ������ 1-10)    *).
    uint8_t uiCrc;
};
#pragma pack(pop)

// ��������� ���������� �� ���������� ������� ��������.
// ��������: PAS_05_PO.DOC, ��� - 3,
// 3. ���� ������ ��������� ���������� ��������
struct TDiscreteSignalsDescriptionWork
{
    // ���� 1:
    // D0-D4 - ��� (������): 0,1-24(0-��� ���.�� �����), D5-�� (��� ��������): 0-��,1-��.
    // D6 - ���1 � ��� ������������ ��������� ����������� �������.
    // D7 - ���2 � ��� ������������ ��������� ����������� �������.
    uint8_t 	uiTalTkGrp;
    // ���� 2:
    // ������ �� ��1:D0=1 ���� ���.�� ���� 1,D0=0-��� ��� �� ���� 1 � �.�.
    // ���� 3,4,5,6,7,8,9:
    // ������ �� ��2,��3,��4,��5,MP6,MP7,MP8:������.��1.
    uint8_t auiRelayOut[8];
    // ���� 10:
    // D7 - ������, D6 - ���:0-������.� ���,1-� ���.���.
    // D0-D5 - ���:������� ������� �������� ������ � ��.���(0-60).
    uint8_t uiDelay;
    // ���� 11 - ����������� ����� ($ff+����� ������ 1-10)    *).
    uint8_t uiCrc;
};

// ����� ��������� ��������� ���������� �� ���������� ������� ��������.
// struct TDiscreteSignalsDescriptionWork
// ��������: PAS_05_PO.DOC, ��� - 3,
// 3. ���� ������ ��������� ���������� ��������
// ���� 1: uiTalTkGrp.
// D0-D4 - ��� (������), ����� ������ �� �������� �����.
#define LIGHT_BOARD_CELL_NUMBER_DISC_MASK 0x1F
// D5-�� (��� ��������): 0-��,1-��
#define CONTACT_TYPE_DISC_MASK 0x20
// D6,D7 - ��� ��� ������������: 0-��� ����.,1-����.,2-����.,3-�����.(����.����)
// ���1 � ��� ������������ ��������� ����������� �������
#define SIGNALING_TYPE_1_DISC_MASK 0x40
// ���2 � ��� ������������ ��������� ����������� �������
#define SIGNALING_TYPE_2_DISC_MASK 0x80
// ���� 10: uiDelay.
// D0-D5 - ���: ������� ������� �������� ������ � ��.���(0-60)
#define DELAY_TIME_SETPOINT_DISC_MASK 0x3F
// D6 - ��� ������������ ������� ��������: 0-������.� ���,1-� ���.���.
#define DELAY_TIME_DISCRETENESS_DISC_MASK 0x40
// D7 - ��� =1 ������������, 0 � �� ������������.
#define ARCHIVE_DISC_MASK 0x80

// ��� (������), ����� ������ �� �������� �����.
#define LIGHT_BOARD_CELL_NUMBER_DIS(pointer) (((pointer) -> uiTalTkGrp) & LIGHT_BOARD_CELL_NUMBER_DISC_MASK)
// �� (��� ��������): 0-��,1-��
#define CONTACT_TYPE_DIS(pointer) (((pointer) -> uiTalTkGrp) & CONTACT_TYPE_DISC_MASK)
// ��� ��� ������������: 0-��� ����.,1-����.,2-����.,3-�����.(����.����)
// ���1 � ��� ������������ ��������� ����������� �������
#define SIGNALING_TYPE_1_DIS(pointer) (((pointer) -> uiTalTkGrp) & SIGNALING_TYPE_1_DISC_MASK)
// ���2 � ��� ������������ ��������� ����������� �������
#define SIGNALING_TYPE_2_DIS(pointer) (((pointer) -> uiTalTkGrp) & SIGNALING_TYPE_2_DISC_MASK)
// ���: ������� ������� �������� ������ � ��.���(0-60)
#define DELAY_TIME_SETPOINT_DIS(pointer) (((pointer) -> uiDelay) & DELAY_TIME_SETPOINT_DISC_MASK)
// ��� ������������ ������� ��������: 0-������.� ���,1-� ���.���.
#define DELAY_TIME_DISCRETENESS_DIS(pointer) (((pointer) -> uiDelay) & DELAY_TIME_DISCRETENESS_DISC_MASK)
// �����.
#define ARCHIVE_DIS(pointer) ((pointer) -> uiDelay) & ARCHIVE_DISC_MASK

// ����������� �����.
#define CRC_DIS(pointer) ((pointer) -> uiCrc)

// ��������� ��������������� ������ ������������ ���������� ����������� ���������� �������� �� HMI.
struct TDiscreteSignalsWorkingData
{
    // ���� ���������� ����������� �����.
    bool fbDiscreteSignalIsActive;
    // ������� ����������� ������� �������� ���.
    uint8_t uiDelay;
};

//-------------------------------------------------------------------------------
class CDiscreteSignals : public CTask
{
public:
    enum
    {
        DISCRETE_SIGNALS_CREATE_DATA_BASE_START = NEXT_STEP,
        DISCRETE_SIGNALS_CREATE_DATA_BASE_BLOCKS_WRITE_START,
        DISCRETE_SIGNALS_CREATE_DATA_BASE_BLOCKS_WRITE_EXECUTOR_DONE_OK_ANSWER_PROCESSING,
        DISCRETE_SIGNALS_CREATE_DATA_BASE_BLOCKS_WRITE_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING,
    };

    CDiscreteSignals();
    virtual ~CDiscreteSignals();

    void SetInternalModuleName(std::string sName);
    void SetDeviceControlName(std::string sName);
    // Сеттер для m_sDataStoreName
    void SetDataStoreName(const std::string& sName)
    {
        m_sDataStoreName = sName;
    }

    void DiscreteSignalsStartDataBaseCreate(void);
    void DiscreteSignalsStartDataBlockWorkToCommonFormat(uint8_t* puiBlockDataPointer,
            TDiscreteSignalsDescriptionWork *pxDiscreteSignalsDescriptionWork);
    void DiscreteSygnalTextDescriptorStartDataBaseCreate(uint8_t* puiBlockDataPointer);

    uint8_t Init(void);
    uint8_t Fsm(void);
    void Allocate(void);

private:

private:
    std::string m_sInternalModuleName;
    uint8_t m_uiInternalModuleId;

    std::string m_sDeviceControlName;
    uint8_t m_uiDeviceControlId;

    std::string m_sDataStoreName;
    uint8_t m_uiDataStoreId;

    uint8_t* m_puiIntermediateBuff;
    TDiscreteSignalsDescriptionWork *m_pxDiscreteSignalsDescriptionWork;
    uint8_t m_uiBlocksCounter;
};

#endif // DISCRETESIGNALS_H
//-------------------------------------------------------------------------------

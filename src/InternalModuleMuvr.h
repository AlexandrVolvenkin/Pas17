#ifndef CINTERNALMODULEMUVR_H
#define CINTERNALMODULEMUVR_H
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
#include <string.h>

#include "Configuration.h"
#include "DataTypes.h"
#include "InternalModule.h"

// MUVR
//// ���� ���� ������.
#define MODULE_TYPE_MVPS3 0x06
#define MODULE_TYPE_MVAI5 0x07
#define MODULE_TYPE_MVST3 0x08
#define MODULE_TYPE_MVAO3 0x09
#define MODULE_TYPE_MUVR 0x0F

enum
{
    MUVR_FSM_STATE_CHANGE_PROTOCOL = 0x00,
// ��������� �������� - ������ ���� ������.
    MUVR_FSM_STATE_WRITE_DATABASE = 0x01,
// ��������� �������� - ���������� �����.
    MUVR_FSM_STATE_TRANSFER = 0x02,
};

#define MAX_MUVR_MODULES_QUANTITY 8
#define MUVR_ANALOG_INPUT_QUANTITY 4
// ���������� ���������� ������� ������ ������.
#define MUVR_DISCRETE_OUTPUT_QUANTITY 4
// ���������� ���������� �������� ����������� ����� ���������� ������.
#define MUVR_DI_VALUE_ONE_CHANNEL_LENGTH 4
// ���������� ���������� ������ ������ ������.
#define MUVR_DISCRETE_INPUT_QUANTITY 0
// ���������� ���������� �������� ������ ������.
#define MUVR_DISCRETE_SIGNALS_QUANTITY (MUVR_ANALOG_INPUT_QUANTITY * MUVR_DI_VALUE_ONE_CHANNEL_LENGTH)
// ���������� ���� ���������� ����� ������� � ������� �������� Modbus.
#define MUVR_AI_VALUE_BYTE_ARRAY_LENGTH (MUVR_ANALOG_INPUT_QUANTITY * sizeof(float))
#define MUVR_DI_VALUE_BIT_ARRAY_LENGTH MUVR_DISCRETE_INPUT_QUANTITY
#define MUVR_BAD_AI_BIT_ARRAY_LENGTH MUVR_ANALOG_INPUT_QUANTITY
#define MUVR_BAD_DI_BIT_ARRAY_LENGTH (MUVR_ANALOG_INPUT_QUANTITY * MUVR_DI_VALUE_ONE_CHANNEL_LENGTH)
#define MUVR_STAT_AI_BIT_ARRAY_LENGTH (MUVR_ANALOG_INPUT_QUANTITY * STAT_AI_BIT_QUANTITY)
#define MUVR_STAT_AI_BYTE_ARRAY_LENGTH MUVR_ANALOG_INPUT_QUANTITY
#define MUVR_MODULE_BAD_BIT_ARRAY_LENGTH 1
#define MUVR_AIN_OFF_BIT_ARRAY_LENGTH MUVR_ANALOG_INPUT_QUANTITY

// ����� Modbus function 5 - 0x11XY - ���������� ������ ����� - ���.
// X - ����� ������, Y - ����� ������������ �����.
#define MUVR_COMMAND_CONTROL_SET_BOTTOM_OF_SCALE  0x11
// ������� ��� ������. ��� D6 - ���������� MIN (0-���, 1-����)
#define MUVR_SET_BOTTOM_OF_SCALE  0x40
// ����� Modbus function 5 - 0x12XY - ���������� ����� ����� - ���.
// X - ����� ������, Y - ����� ������������ �����.
#define MUVR_COMMAND_CONTROL_SET_TOP_OF_SCALE  0x12
// ������� ��� ������. ��� D7 - ���������� M�� (0-���, 1-����).
#define MUVR_SET_TOP_OF_SCALE  0x80
// $43 - ������ ���
#define MUVR_GET_TXS_DATA_COMMAND 0x43
#define MUVR_GET_TXS_DATA_COMMAND_ANSWER_LENGTH 5
// ����� ����� ����������� ��������� ���� ��� ����3.
#define MUVR_TXS_INPUT_NUMBER  0x07
#define MUVR_TXS_INPUT_QUANTITY  1
// ���� � ������� �������� ������ Modbus ���7 = 0, �� ������������� �������� ����� - (���0 - ���6) - ����� ����������� �����.
// ���� � ������� �������� ������ Modbus ���7 = 1, �� ������������� ��� � (���0 - ���2) - ������������� ����� ������ ����3.
#define MUVR_TXS_REQUEST_MASK  0x80
// (���0 - ���2) - ������������� ����� ������ ����3.
#define TXS_MVST3_MODULE_NUMBER_MASK  0x07
// �� ����� ����� �� ������ ���� ���� ����������� �����.
#define MUVR_TXS_CALIBRATION_DATA_OFFSET 5
// $44 - ������ �� �������� (�����������) ����� �������
#define MUVR_GET_MEASURE_DATA_COMMAND 0x44
#define MUVR_ANSWER_DATA_NOT_READY 0x24
#define MUVR_ANSWER_DATABASE_ERROR 0x25
#define MUVR_ANSWER_REPER_POINTS_ADC_DATABASE_ERROR 0x26
// $45 - ������ �������� ����� � ���� ���
#define MUVR_GET_REPER_POINTS_ADC_DATA_COMMAND 0x45
// ��� ������ - ������ �� ��������. ��� ������� � ���������� ����������������� �������� Modbus 14 �� �������������.
#define MUVR_GET_REPER_POINTS_ADC_DATA_COMMAND_ERROR 0x01
// ��� ������ - ������ ������ �������. ��� ������� � ���������� ����������������� �������� Modbus 14 �� �������������.
#define MUVR_GET_REPER_POINTS_ADC_DATA_COMMUNICATION_ERROR 0x02
#define MUVR_REPER_POINTS_ADC_CHANNEL_QUANTITY 7
// �������� ��������� ������ ���������� � �����: ������ � ���� ���-17.txt
// 3. ����� ������� ����� ��� � ���� � �����.
#define MUVR_GET_MEASURE_DATA_COMMAND_ANSWER_LENGTH 24
// $84 - ������ �� ����� ��  (�� ��� � ���)
#define MUVR_GET_DATA_BASE_COMMAND 0x84
// $81 - ������ �� �������� �� (�� ��� � ���)
#define MUVR_SET_DATA_BASE_COMMAND 0x81
// $82 - ������ � ����������� ����������������
#define MUVR_CHECK_DATA_BASE_WRITE_COMMAND 0x82
#define MUVR_CHECK_DATA_BASE_WRITE_ANSWER_BUSY 0xF0
// ���������� ����� �������� ������ �� ������, � ����������� ������ ���� ������.
#define MUVR_CHECK_DATA_BASE_WRITE_BUSY_WAITING_TIME 10
#define MUVR_CHECK_DATA_BASE_WRITE_BUSY_DELAY_TIME 500000

// �������� �� ������ � ������ auiSpiRxBuffer[], ��� ������ �� SPI.
#define MUVR_COMMAND_BYTE_OFFSET 1
#define MUVR_COMMAND_BYTE_LENGTH 1
// �������� �� ������ ���������� �������� ���������� ��������.
#define MUVR_MEASURE_DATA_OFFSET (MUVR_COMMAND_BYTE_OFFSET + 1)
#define MUVR_DATA_OFFSET (MUVR_COMMAND_BYTE_OFFSET + 1)
// ���������� ���� ������ ��������� ���������� ������.
#define MUVR_ANALOG_INPUT_STATE_BYTE_QUANTITY MUVR_ANALOG_INPUT_QUANTITY
// ���������� ���� ������ ������ ����������� �������� ����������� �������(������ float STEP5).
#define MUVR_ONE_ANALOG_INPUT_DATA_BYTE_QUANTITY 3
// �������� �� ������ ��������� ������� ���������� ������.
// �������� ��������� ������ ���������� � �����: ������ � ���� ���-17.txt
// 3. ����� ������� ����� ��� � ���� � �����.
#define MUVR_STATE_DATA_OFFSET (MUVR_MEASURE_DATA_OFFSET + (MUVR_ANALOG_INPUT_QUANTITY * MUVR_ONE_ANALOG_INPUT_DATA_BYTE_QUANTITY) + 6)
// �������� �� ������ ���������� ����������. ���������� � ������ ����� �������,
// �� ����� ����� �� ������ ���� ���� ����������� �����.
#define MUVR_CALIBRATION_DATA_OFFSET (MUVR_STATE_DATA_OFFSET + MUVR_ANALOG_INPUT_STATE_BYTE_QUANTITY)

// (sizeof(struct TAnalogueInputDescriptionDataBase) * ANALOG_MODULE_INPUT_QUANTITY)// 28�6=168.
#define ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_LENGTH 168
#define ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_TEXT_DESCRIPTOR_OFFSET 120

#define PERCENT_100 100 // ��� ���������.
#define MEASURE_ERROR_THRESHOLD 3 // ����� ������ ����������� ��������.

#define ANALOGUE_INPUT_SET_POINT_VIOLATION_L 0x03 // 3 � ��������� ������� L.
#define ANALOGUE_INPUT_SET_POINT_VIOLATION_LL_L 0x04 // 4 � ��������� ������� LL+L.
#define ANALOGUE_INPUT_SET_POINT_VIOLATION_H 0x05 // 5 � ��������� ������� H.
#define ANALOGUE_INPUT_SET_POINT_VIOLATION_HH_H 0x06 // 6 � ��������� ������� �+��.
#define ANALOGUE_INPUT_SET_POINT_VIOLATION_MASK 0x07
#define ANALOGUE_INPUT_CHANNEL_CALIBRATION 0x20 // ����� ��������� � ������ ����������.
#define ANALOGUE_INPUT_LINE_BREAK 0x40 // ����� ����� ������.
#define ANALOGUE_INPUT_CALCULATION_OVERFLOW 0x80 // ������������ ��� ��������.
// �������� �� ����� ��������� ������� � ������� DI_value ��� ������ ����������� �����.
#define ANALOGUE_INPUT_SET_POINT_VIOLATION_LL_L_OFFSET 0 // �������� ������� LL+L.
#define ANALOGUE_INPUT_SET_POINT_VIOLATION_L_OFFSET 1 // �������� ������� L.
#define ANALOGUE_INPUT_SET_POINT_VIOLATION_H_OFFSET 2 // �������� ������� H.
#define ANALOGUE_INPUT_SET_POINT_VIOLATION_HH_H_OFFSET 3 // �������� ������� �+��.

// ���� ��������� ����������� ��������� ��� �������� � ������������� � ����.
#define HMI_ANALOGUE_INPUT_LINE_BREAK 1 // ����� ����� ������.
#define HMI_ANALOGUE_INPUT_CALCULATION_OVERFLOW 2 // ������������ ��� ��������.
#define HMI_ANALOGUE_INPUT_CHANNEL_CALIBRATION 3 // ����� ��������� � ������ ����������.
#define HMI_ANALOGUE_INPUT_SET_POINT_VIOLATION_L 4 // 3 � ��������� ������� L.
#define HMI_ANALOGUE_INPUT_SET_POINT_VIOLATION_LL_L 5 // 4 � ��������� ������� LL+L.
#define HMI_ANALOGUE_INPUT_SET_POINT_VIOLATION_H 6 // 5 � ��������� ������� H.
#define HMI_ANALOGUE_INPUT_SET_POINT_VIOLATION_HH_H 7 // 6 � ��������� ������� �+��.

class CTimer;
class CPlatform;
class CTask;
class CResources;
class CCommunicationDevice;
class CCommunicationDeviceInterface;
class CSpiCommunicationDevice;
//class CInternalModule;
class CDataContainerInterface;
class CDataContainerDataBase;
class CAnalogueSignals;

//-------------------------------------------------------------------------------
class CInternalModuleMuvr : public CInternalModule
{
public:
    enum
    {
        MUVR_IDDLE = NEXT_STEP,
        MUVR_CHANGE_PROTOCOL,
        MUVR_GET_MODULE_TYPE,
        MUVR_REPER_POINTS_ADC_READ,
        MUVR_WRITE_DATA_BASE,
        MUVR_WRITE_DATA_BASE_CHECK,
        MUVR_DATA_BASE_READ,
        MUVR_DATA_EXCHANGE,
        MUVR_CALIBRATION_ON_OFF,
        MUVR_INPUT_SET_TOP_OR_BOTTOM_OF_SCALE,
    };

    CInternalModuleMuvr();
    CInternalModuleMuvr(uint8_t muiAddress);
    virtual ~CInternalModuleMuvr();

    void SetCommunicationDeviceName(std::string sName);
    void SetCommunicationDevice(CCommunicationDeviceInterface* pxCommunicationDevice);
    uint8_t Init(void);
//    bool SetTaskData(CDataContainerDataBase* pxDataContainer);
//    bool GetTaskData(CDataContainerDataBase* pxDataContainer);

    uint8_t Fsm(void);

//    uint8_t GetType(void);
//    uint8_t GetWorkingStatus(void);
//    void SetAddress(uint8_t uiAddress);
//    uint8_t GetAddress(void);
//    bool IsReadyToStartWork(void);
//    bool IsAbleToReplace(uint8_t uiType);
    void Allocate(void);
//    uint8_t GetModuleType(uint8_t uiAddress);

    uint8_t ReperPointsAdcRead(void);
    uint8_t DataBaseRead(void);
    uint8_t DataBaseBlockWrite(void);
    uint8_t DataBaseBlockWriteCheck(void);
    uint8_t DataExchange(void);

private:
//    uint8_t m_uiAddress;
//    CCommunicationDeviceInterface* m_pxCommunicationDevice;
//    CDataContainerDataBase* m_pxOperatingDataContainer;
    uint8_t* m_puiModuleBadStateBuffer;

    uint8_t* m_puiDiscreteInputsState;
    uint8_t* m_puiDiscreteInputsBadState;
    uint8_t* m_puiDiscreteOutputState;
    float* m_pfAnalogueInputsValue;
    uint8_t* m_puiAnalogueInputsState;
    uint8_t* m_puiAnalogueInputsOff;
    uint8_t* m_puiAnalogueInputsBadState;
    uint8_t* m_puiReperPointsAdcBuffer;
    uint8_t* m_puiAnalogueInputDiscreteInputsState;
    uint8_t* m_puiAnalogueInputDiscreteInputsBadState;
//    uint8_t m_uiBadAnswerCounter;
    TAnalogueInputDescriptionWork* m_pxAnalogueInputDescriptionWork;
    TDiscreteOutputControl* m_pxDiscreteOutputControl;

    CTimer m_xWriteCompleteWaitTimer;
};

#endif // CINTERNALMODULEMUVR_H

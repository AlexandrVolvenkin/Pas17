#ifndef FUNCTIONBLOCKS_H_INCLUDED
#define FUNCTIONBLOCKS_H_INCLUDED

#include "PasNewConfig.h"

// ���������� ������� ���������� ����.
#define FUNCTION_BLOCK_RELAY_OUTPUT_QUANTITY DO_VALUE_BIT_ARRAY_LENGTH
// ���� �������������� ������.
// �������������� ���� - "��".
#define FUNCTION_BLOCK_FLOW_NO 0
// �������������� ���� - "�".
#define FUNCTION_BLOCK_FLOW_AND 1
// �������������� ���� - "�-��".
#define FUNCTION_BLOCK_FLOW_AND_NOT 2
// �������������� ���� - "���".
#define FUNCTION_BLOCK_FLOW_OR 3
// �������������� ���� - "���-��".
#define FUNCTION_BLOCK_FLOW_OR_NOT 4
// �������������� ���� - "����������".
#define FUNCTION_BLOCK_FLOW_HISTERESIS 5
// �������������� ���� - "������".
#define FUNCTION_BLOCK_FLOW_TIMER 6
// �������������� ���� - "�������".
#define FUNCTION_BLOCK_FLOW_TRIGGER 7
// �������������� ���� - "���������� ����������".
#define FUNCTION_BLOCK_FLOW_ANALOGUE_COMPARATOR 8

// ��������� ������� ���������� �������� �������������� ������.
#define FUNCTION_BLOCK_INPUT_SOURCE_DI_VALUE 0
#define FUNCTION_BLOCK_INPUT_SOURCE_FUNCTION_BLOCK_OUTS 1
#define FUNCTION_BLOCK_INPUT_SOURCE_BAD_AI 2
#define FUNCTION_BLOCK_INPUT_SOURCE_LOG_SOST 3
#define FUNCTION_BLOCK_INPUT_SOURCE_BAD_DI 4

// ���� ������� ��� �������������� ������ - "������".
// �������� ���������.
#define FUNCTION_BLOCK_TIMER_TYPE_ON_DELAY 0
#define FUNCTION_BLOCK_TIMER_TYPE_RESERVED 1
// �������� ����������.
#define FUNCTION_BLOCK_TIMER_TYPE_OFF_DELAY 2
// "�������� �������".
#define FUNCTION_BLOCK_TIMER_TYPE_SHORT_PULSE 3
// "��������� �������".
#define FUNCTION_BLOCK_TIMER_TYPE_LONG_PULSE 4
// (*������������ ������� 0-200 ��, 1-1�, 2-10�*)
#define FUNCTION_BLOCK_TIMER_DISCRETENESS_100MS 0
#define FUNCTION_BLOCK_TIMER_DISCRETENESS_1S 1
#define FUNCTION_BLOCK_TIMER_DISCRETENESS_10S 2

// ���� �������� ��� �������������� ������ - "�������".
// ��������� ������.
#define FUNCTION_BLOCK_TRIGGER_TYPE_R_S 0
// ��������� ���������.
#define FUNCTION_BLOCK_TRIGGER_TYPE_S_R 1

// ��������� ���������� �� �������������� ������.
// ��������: PAS_05_PO.DOC, ��� - 3,
// 3. ���� ������ ��������� ���������� ��������
struct TFunctionBlockDescriptionWork
{
    unsigned char ucFunctionBlockType;
    // ������ ������� ����������� �������� � ��.
    // ��� ������� ���� ��������������� ����� ���� ������.
    unsigned char aucInputAddress[8];
    // D0-D4 - ��� (������): 0,1-24(0-��� ���.�� �����).
    // D5 - ��� =1 ������������, 0 � �� ������������.
    // D6 - ���1 � ��� ������������ ��������� ������ ���:
    // D7 - ���2 � ��� ������������ ��������� ������ ���:
    // 00 � ��� ������������
    // 01 � ����������������� ������������
    // 10 � ��������� ������������
    // 11 � ��������� (������ ���� ��� ��������� �������)
    unsigned char ucTalArhGrp;
    // ���� 1:
    // ������ �� ��1:D0=1 ���� ���.�� ���� 1,D0=0-��� ��� �� ���� 1 � �.�.
    // ���� 2,3,4,5,6,7,8:
    // ������ �� ��2,��3,��4,��5,MP6,MP7,MP8:������.��1.
    unsigned char aucRelayOut[8];
    // ����������� ����� ($ff+����� ������ 1-10)    *).
    unsigned char ucCrc;
};

// ��� ������������, ������� ������ ���������� �� ����������� �������� ������.
#pragma pack(push)
#pragma pack(1)
// ��������� ���������� �� �������������� ������.
// ��������: PAS_05_PO.DOC, ��� - 3,
// 3. ���� ������ ��������� ���������� ��������
struct TFunctionBlockDescriptionWorkPackOne
{
    unsigned char ucFunctionBlockType;
    // ������ ������� ����������� �������� � ��.
    // ��� ������� ���� ��������������� ����� ���� ������.
    unsigned char aucInputAddress[8];
    // D0-D4 - ��� (������): 0,1-24(0-��� ���.�� �����).
    // D5 - ��� =1 ������������, 0 � �� ������������.
    // D6 - ���1 � ��� ������������ ��������� ������ ���:
    // D7 - ���2 � ��� ������������ ��������� ������ ���:
    // 00 � ��� ������������
    // 01 � ����������������� ������������
    // 10 � ��������� ������������
    // 11 � ��������� (������ ���� ��� ��������� �������)
    unsigned char ucTalArhGrp;
    // ���� 1:
    // ������ �� ��1:D0=1 ���� ���.�� ���� 1,D0=0-��� ��� �� ���� 1 � �.�.
    // ���� 2,3,4,5,6,7,8:
    // ������ �� ��2,��3,��4,��5,MP6,MP7,MP8:������.��1.
    unsigned char aucRelayOut[8];
    // ����������� ����� ($ff+����� ������ 1-10)    *).
    unsigned char ucCrc;
};
#pragma pack(pop)

//-----------------------------------------------------------------------------------------------------
// ������ ��������� �������������� ������ - ����� ����� ���� ������.
// ����� ��������� ��������� ���������� �� �������������� ������.
// struct TFunctionBlockDescriptionWork
// ��������: PAS_05_PO.DOC, ��� - 3,
// 3. ���� ������ ��������� ���������� ��������
// ����: ucTalArhGrp.
// ���� � ��� ��������������� �����:
#define FUNCTION_BLOCK_TYPE_FBL_MASK 0x0F
// D0-D4 - � ��� � ����� ������ ��������� �����: (1-24), 0 � ��� ������ �� �������� �����.
#define LIGHT_BOARD_CELL_NUMBER_FBL_MASK 0x1F
// D5 - ��� =1 ������������, 0 � �� ������������.
#define ARCHIVE_FBL_MASK 0x20
// D6-D7 - ���2, ���1 � ��� ������������ ��������� ������ ���:
// 00 � ��� ������������;
// 01 � ����������������� ������������;
// 10 � ��������� ������������;
// 11 � ��������� (������ ���� ��� ��������� �������).
// D6 � ���1.
#define SIGNALING_TYPE_1_FBL_MASK 0x40
// D7 � ���2.
#define SIGNALING_TYPE_2_FBL_MASK 0x80

// ������ ��������� � ����� - ucFunctionBlockType, ��������� - struct TFunctionBlockDescriptionWork.
// ���� � ��� ��������������� �����:
#define FUNCTION_BLOCK_TYPE_FBL(pointer) (((pointer) -> ucFunctionBlockType) & FUNCTION_BLOCK_TYPE_FBL_MASK)

// ������ ��������� � ����� - ucTalArhGrp, ��������� - struct TFunctionBlockDescriptionWork.
// ��� � ����� ������ ��������� �����.
#define LIGHT_BOARD_CELL_NUMBER_FBL(pointer) (((pointer) -> ucTalArhGrp) & LIGHT_BOARD_CELL_NUMBER_FBL_MASK)
// ��� =1 ������������, 0 � �� ������������.
#define ARCHIVE_FBL(pointer) (((pointer) -> ucTalArhGrp) & ARCHIVE_FBL_MASK)
// ���1 � ��� ������������ ��������� ������ ���
#define SIGNALING_TYPE_1_FBL(pointer) (((pointer) -> ucTalArhGrp) & SIGNALING_TYPE_1_FBL_MASK)
// ���2 � ��� ������������ ��������� ������ ���
#define SIGNALING_TYPE_2_FBL(pointer) (((pointer) -> ucTalArhGrp) & SIGNALING_TYPE_2_FBL_MASK)
// ����������� ����� ���������.
#define CRC_FBL(pointer) ((pointer) -> ucCrc)

//-----------------------------------------------------------------------------------------------------
// ������ ��������� �������������� ������ - �Ȼ, ��-�Ż, ���Ȼ, ����-�Ż, ���һ ����������.
//TFunctionBlockDescriptionWork

// ������� � ������� - aucInputAddress[8], ��������� - struct TFunctionBlockDescriptionWork.
// ����1 � �������� �������� ����������� ������� - 1.
#define INPUT_SOURCE_FBL_1_INDEX 0
// ����2 � �������� �������� ����������� ������� - 2.
#define INPUT_SOURCE_FBL_2_INDEX 2
// ����3 � �������� �������� ����������� ������� - 3.
#define INPUT_SOURCE_FBL_3_INDEX 4
// ����4 � �������� �������� ����������� ������� - 4.
#define INPUT_SOURCE_FBL_4_INDEX 6

// ���1� ��� ����� 1.
#define INPUT_TYPE_FBL_1_INDEX 0
// ���2� ��� ����� 2.
#define INPUT_TYPE_FBL_2_INDEX 2
// ���3� ��� ����� 3.
#define INPUT_TYPE_FBL_3_INDEX 4
// ���4� ��� ����� 4.
#define INPUT_TYPE_FBL_4_INDEX 6

// �����1 � � �������-��������� �������� ����������� ������� - ������� ����;
#define ADDRESS_HIGH_FBL_1_INDEX 0
// �����2 � � �������-��������� �������� ����������� ������� - ������� ����;
#define ADDRESS_HIGH_FBL_2_INDEX 2
// �����3 � � �������-��������� �������� ����������� ������� - ������� ����;
#define ADDRESS_HIGH_FBL_3_INDEX 4
// �����4 � � �������-��������� �������� ����������� ������� - ������� ����;
#define ADDRESS_HIGH_FBL_4_INDEX 6

// �����1 � ����� � �������-��������� �������� ����������� ������� - ������� ����;
#define ADDRESS_LOW_FBL_1_INDEX 1
// �����2 � ����� � �������-��������� �������� ����������� ������� - ������� ����;
#define ADDRESS_LOW_FBL_2_INDEX 3
// �����3 � ����� � �������-��������� �������� ����������� ������� - ������� ����;
#define ADDRESS_LOW_FBL_3_INDEX 5
// �����4 � ����� � �������-��������� �������� ����������� ������� - ������� ����;
#define ADDRESS_LOW_FBL_4_INDEX 7

// ����� ��������� � ������� - aucInputAddress[8], ��������� - struct TFunctionBlockDescriptionWork.
// ����1 � ����4 � �������� �������� ����������� �������:
// 0 � ������� ������ ��� (������� ������ TSOST, ����� 1-192);
// 1 � ����� ��� (����� 1-120) ��� ������� ���������� � �������� ������ ��� (����� 121-184);
// 2 � ���� ��������������� �������� ����������� ������� (����� 1-96);
// 3 � ���������� ����� ��������� CoDeSys (����� 1-112 ������ ��� ��� ���05-16 CDS)
// 4 � ���� ��������������� �������� ����������� ������� (����� 1-192);
#define INPUT_SOURCE_FBL_MASK 0x07
// ���1 � ���4 � ��� �����: 1 � ������, 0 � ���������;
#define INPUT_TYPE_FBL_MASK 0x08
// �����1 � �����4 � ����� � �������-��������� �������� ����������� ������� - ������� ����;
#define ADDRESS_HIGH_FBL_MASK 0xC0
// �����1 � �����4 � ����� � �������-��������� �������� ����������� ������� - ������� ����;
#define ADDRESS_LOW_FBL_MASK 0xFF

// ������ ��������� � ������� - aucInputAddress[8], ��������� - struct TFunctionBlockDescriptionWork.
// ����1 � �������� �������� ����������� ������� - 1.
#define INPUT_SOURCE_FBL_1(pointer) (((pointer) -> aucInputAddress[INPUT_SOURCE_FBL_1_INDEX]) & INPUT_SOURCE_FBL_MASK)
// ����2 � �������� �������� ����������� ������� - 2.
#define INPUT_SOURCE_FBL_2(pointer) (((pointer) -> aucInputAddress[INPUT_SOURCE_FBL_2_INDEX]) & INPUT_SOURCE_FBL_MASK)
// ����3 � �������� �������� ����������� ������� - 3.
#define INPUT_SOURCE_FBL_3(pointer) (((pointer) -> aucInputAddress[INPUT_SOURCE_FBL_3_INDEX]) & INPUT_SOURCE_FBL_MASK)
// ����4 � �������� �������� ����������� ������� - 4.
#define INPUT_SOURCE_FBL_4(pointer) (((pointer) -> aucInputAddress[INPUT_SOURCE_FBL_4_INDEX]) & INPUT_SOURCE_FBL_MASK)

// ���1� ��� ����� 1.
#define INPUT_TYPE_FBL_1(pointer) (((pointer) -> aucInputAddress[INPUT_TYPE_FBL_1_INDEX]) & INPUT_TYPE_FBL_MASK)
// ���2� ��� ����� 2.
#define INPUT_TYPE_FBL_2(pointer) (((pointer) -> aucInputAddress[INPUT_TYPE_FBL_2_INDEX]) & INPUT_TYPE_FBL_MASK)
// ���3� ��� ����� 3.
#define INPUT_TYPE_FBL_3(pointer) (((pointer) -> aucInputAddress[INPUT_TYPE_FBL_3_INDEX]) & INPUT_TYPE_FBL_MASK)
// ���4� ��� ����� 4.
#define INPUT_TYPE_FBL_4(pointer) (((pointer) -> aucInputAddress[INPUT_TYPE_FBL_4_INDEX]) & INPUT_TYPE_FBL_MASK)

// �����1 � ����� � �������-��������� �������� ����������� ������� - ������� ����;
#define ADDRESS_HIGH_FBL_1(pointer) (((((pointer) -> aucInputAddress[ADDRESS_HIGH_FBL_1_INDEX]) & ADDRESS_HIGH_FBL_MASK)) >> 6)
// �����2 � ����� � ������� ��������� �������� ����������� ������� - ������� ����;
#define ADDRESS_HIGH_FBL_2(pointer) (((((pointer) -> aucInputAddress[ADDRESS_HIGH_FBL_2_INDEX]) & ADDRESS_HIGH_FBL_MASK)) >> 6)
// �����3 � ����� � �������-��������� �������� ����������� ������� - ������� ����;
#define ADDRESS_HIGH_FBL_3(pointer) (((((pointer) -> aucInputAddress[ADDRESS_HIGH_FBL_3_INDEX]) & ADDRESS_HIGH_FBL_MASK)) >> 6)
// �����4 � ����� � �������-��������� �������� ����������� ������� - ������� ����;
#define ADDRESS_HIGH_FBL_4(pointer) (((((pointer) -> aucInputAddress[ADDRESS_HIGH_FBL_4_INDEX]) & ADDRESS_HIGH_FBL_MASK)) >> 6)

// �����1 � ����� � �������-��������� �������� ����������� ������� - ������� ����;
#define ADDRESS_LOW_FBL_1(pointer) ((pointer) -> aucInputAddress[ADDRESS_LOW_FBL_1_INDEX])
// �����2 � ����� � �������-��������� �������� ����������� ������� - ������� ����;
#define ADDRESS_LOW_FBL_2(pointer) ((pointer) -> aucInputAddress[ADDRESS_LOW_FBL_2_INDEX])
// �����3 � ����� � �������-��������� �������� ����������� ������� - ������� ����;
#define ADDRESS_LOW_FBL_3(pointer) ((pointer) -> aucInputAddress[ADDRESS_LOW_FBL_3_INDEX])
// �����4 � ����� � �������-��������� �������� ����������� ������� - ������� ����;
#define ADDRESS_LOW_FBL_4(pointer) ((pointer) -> aucInputAddress[ADDRESS_LOW_FBL_4_INDEX])

//-----------------------------------------------------------------------------------------------------
// ������ ��������� ��������������� ����� - �������.
// ������� � ������� - aucInputAddress[8], ��������� - struct TFunctionBlockDescriptionWork.
// ����(SET) � �������� �������� ����������� �������.
#define INPUT_SOURCE_FBL_SET_INDEX 0
// ����(RESET) � �������� �������� ����������� �������.
#define INPUT_SOURCE_FBL_RESET_INDEX 2

// ���(SET) � ��� �����: 1 � ������, 0 � ���������;
#define INPUT_TYPE_FBL_SET_INDEX 0
// ���(RESET) � ��� �����: 1 � ������, 0 � ���������;
#define INPUT_TYPE_FBL_RESET_INDEX 2

// �����1(SET) � ����� � �������-��������� �������� ����������� ������� - ������� ����;
#define ADDRESS_HIGH_FBL_SET_INDEX 0
// �����2(SET) � ����� � �������-��������� �������� ����������� ������� - ������� ����;
#define ADDRESS_LOW_FBL_SET_INDEX 1

// �����1(RESET) � ����� � �������-��������� �������� ����������� ������� - ������� ����;
#define ADDRESS_HIGH_FBL_RESET_INDEX 2
// �����2(RESET) � ����� � �������-��������� �������� ����������� ������� - ������� ����;
#define ADDRESS_LOW_FBL_RESET_INDEX 3

// ��� � ������������ ������� ��������: 0 -100 ����,1 � ��. ���.,2 � ���. ���.
#define DELAY_TIME_DISCRETENESS_FBL_INDEX 4
// ��� � ������� ������� ��������.
#define DELAY_TIME_SETPOINT_FBL_INDEX 5
// ����� ��� � ����� ������� ������� ��������.
#define DELAY_TIME_SETPOINT_FBL_ADDRESS_INDEX 6
// ���� � ��� ������� ������� ��������.
#define DELAY_TIME_SETPOINT_FBL_TYPE_INDEX 6

// ����� ��������� � ������� - aucInputAddress[8], ��������� - struct TFunctionBlockDescriptionWork.
// ��� ������� � ��� ���� ������� 0 - 4
// 0 � ��������� ����������;
// 1 � ��������� ��������� � ������������;
// 2 � ��������� �����������;
// 3 � ��������� �������;
// 4 � ����������� �������;
#define TIMER_TYPE_FBL_MASK 0xF0
// ����1 � ����4 � �������� �������� ����������� �������:
// 0 � ������� ������ ��� (������� ������ TSOST, ����� 1-192);
// 1 � ����� ��� (����� 1-120) ��� ������� ���������� � �������� ������ ��� (����� 121-184);
// 2 � ���� ��������������� �������� ����������� ������� (����� 1-96);
// 3 � ���������� ����� ��������� CoDeSys (����� 1-112 ������ ��� ��� ���05-16 CDS)
// 4 � ���� ��������������� �������� ����������� ������� (����� 1-192);
#define INPUT_SOURCE_FBL_MASK 0x07
// ��� � ��� �����: 1 � ������, 0 � ���������;
#define INPUT_TYPE_FBL_MASK 0x08
// �����1(SET) � �����2(RESET) � ����� � �������-��������� �������� ����������� ������� - ������� ����;
#define ADDRESS_HIGH_FBL_MASK 0xC0
// �����1(SET) � �����2(RESET) � ����� � �������-��������� �������� ����������� ������� - ������� ����;
#define ADDRESS_LOW_FBL_MASK 0xFF
// ��� � ������������ ������� ��������: 0 -100 ����,1 � ��. ���.,2 � ���. ���.
#define DELAY_TIME_DISCRETENESS_FBL_MASK 0x03
// ��� � �������� ������� �� ��������� ����:
// - ��� ���� = 0: ��� � ������� ������� ��������, ���������� � �������� ��� (0-120)
// 0,1�120=12 �, 1�120=120 �(2,0 ���), 10�120=1200 � (20,0 ���)
// - ��� ���� = 1: - �� ����� ��������.
#define DELAY_TIME_SETPOINT_FBL_MASK 0xFF
// ����� ��� � �������� ������� �� ��������� ����:
// - ��� ���� = 1: ����� ������� ������� ��������, ������������� � ������� �������� ���������� ������� (1 � 10). ������ ������� ��� ��, ��� � ���. ����� ����� ���� 10 ���������� �������, � ������� ����� ������������ ����� ������.
// - ��� ���� = 0: - �� ����� ��������.
#define DELAY_TIME_SETPOINT_FBL_ADDRESS_MASK 0x0F
// ���� � ��� ������� ������� ��������
// 0 � ������������� �������� �������
// 1 � ���������� �������� �������
#define DELAY_TIME_SETPOINT_FBL_TYPE_MASK 0x10

// ������ ��������� � ������� - aucInputAddress[8], ��������� - struct TFunctionBlockDescriptionWork.
#define TIMER_TYPE_FBL(pointer) (((((pointer) -> ucFunctionBlockType) & TIMER_TYPE_FBL_MASK)) >> 4)

// ����(SET) � �������� �������� ����������� �������.
#define INPUT_SOURCE_FBL_SET(pointer) (((pointer) -> aucInputAddress[INPUT_SOURCE_FBL_SET_INDEX]) & INPUT_SOURCE_FBL_MASK)
// ����(RESET) � �������� �������� ����������� �������.
#define INPUT_SOURCE_FBL_RESET(pointer) (((pointer) -> aucInputAddress[INPUT_SOURCE_FBL_RESET_INDEX]) & INPUT_SOURCE_FBL_MASK)

// ���(SET) � ��� �����: 1 � ������, 0 � ���������;
#define INPUT_TYPE_FBL_SET(pointer) (((pointer) -> aucInputAddress[INPUT_TYPE_FBL_SET_INDEX]) & INPUT_TYPE_FBL_MASK)
// ���(RESET) � ��� �����: 1 � ������, 0 � ���������;
#define INPUT_TYPE_FBL_RESET(pointer) (((pointer) -> aucInputAddress[INPUT_TYPE_FBL_RESET_INDEX]) & INPUT_TYPE_FBL_MASK)

// �����1(SET) � � �������-��������� �������� ����������� ������� - ������� ����;
#define ADDRESS_HIGH_FBL_SET(pointer) (((((pointer) -> aucInputAddress[ADDRESS_HIGH_FBL_SET_INDEX]) & ADDRESS_HIGH_FBL_MASK)) >> 6)
// �����2(RESET) � � �������-��������� �������� ����������� ������� - ������� ����;
#define ADDRESS_HIGH_FBL_RESET(pointer) (((((pointer) -> aucInputAddress[ADDRESS_HIGH_FBL_RESET_INDEX]) & ADDRESS_HIGH_FBL_MASK)) >> 6)

// �����1(SET) � ����� � �������-��������� �������� ����������� ������� - ������� ����;
#define ADDRESS_LOW_FBL_SET(pointer) ((pointer) -> aucInputAddress[ADDRESS_LOW_FBL_SET_INDEX])
// �����2(RESET) � ����� � �������-��������� �������� ����������� ������� - ������� ����;
#define ADDRESS_LOW_FBL_RESET(pointer) ((pointer) -> aucInputAddress[ADDRESS_LOW_FBL_RESET_INDEX])

// ��� � ������������ ������� ��������: 0 -100 ����,1 � ��. ���.,2 � ���. ���.
#define DELAY_TIME_DISCRETENESS_FBL(pointer) ((pointer -> aucInputAddress[DELAY_TIME_DISCRETENESS_FBL_INDEX]) & DELAY_TIME_DISCRETENESS_FBL_MASK)
// ��� � ������� ������� ��������.
#define DELAY_TIME_SETPOINT_FBL(pointer) ((pointer) -> aucInputAddress[DELAY_TIME_SETPOINT_FBL_INDEX])
// ����� ��� � ����� ������� ������� ��������.
#define DELAY_TIME_SETPOINT_FBL_ADDRESS(pointer) (((pointer) -> aucInputAddress[DELAY_TIME_SETPOINT_FBL_ADDRESS_INDEX]) & DELAY_TIME_SETPOINT_FBL_ADDRESS_MASK)
// ���� � ��� ������� ������� ��������.
#define DELAY_TIME_SETPOINT_FBL_TYPE_INDEX 6#define DELAY_TIME_SETPOINT_FBL_TYPE(pointer) (((pointer) -> aucInputAddress[DELAY_TIME_SETPOINT_FBL_TYPE_INDEX]) & DELAY_TIME_SETPOINT_FBL_TYPE_MASK)

//-----------------------------------------------------------------------------------------------------
// ������ ��������� ��������������� ����� - ��������.
// ������� � ������� - aucInputAddress[8], ��������� - struct TFunctionBlockDescriptionWork.
// ����(SET1) � �������� �������� ����������� �������.
#define INPUT_SOURCE_FBL_SET_1_INDEX 0
// ����(SET2) � �������� �������� ����������� �������.
#define INPUT_SOURCE_FBL_SET_2_INDEX 2
// ����(RESET1) � �������� �������� ����������� �������.
#define INPUT_SOURCE_FBL_RESET_1_INDEX 4
// ����(RESET2) � �������� �������� ����������� �������.
#define INPUT_SOURCE_FBL_RESET_2_INDEX 6

// ���(SET1) � ��� �����: 1 � ������, 0 � ���������;
#define INPUT_TYPE_FBL_SET_1_INDEX 0
// ���(SET2) � ��� �����: 1 � ������, 0 � ���������;
#define INPUT_TYPE_FBL_SET_2_INDEX 2
// ���(RESET1) � ��� �����: 1 � ������, 0 � ���������;
#define INPUT_TYPE_FBL_RESET_1_INDEX 4
// ���(RESET2) � ��� �����: 1 � ������, 0 � ���������;
#define INPUT_TYPE_FBL_RESET_2_INDEX 6

// �����1(SET1) � ����� � �������-��������� �������� ����������� ������� - ������� ����;
#define ADDRESS_HIGH_FBL_SET_1_INDEX 0
// �����2(SET1) � ����� � �������-��������� �������� ����������� ������� - ������� ����;
#define ADDRESS_LOW_FBL_SET_1_INDEX 1
// �����1(SET2) � ����� � �������-��������� �������� ����������� ������� - ������� ����;
#define ADDRESS_HIGH_FBL_SET_2_INDEX 2
// �����2(SET2) � ����� � �������-��������� �������� ����������� ������� - ������� ����;
#define ADDRESS_LOW_FBL_SET_2_INDEX 3

// �����1(RESET1) � ����� � �������-��������� �������� ����������� ������� - ������� ����;
#define ADDRESS_HIGH_FBL_RESET_1_INDEX 4
// �����2(RESET1) � ����� � �������-��������� �������� ����������� ������� - ������� ����;
#define ADDRESS_LOW_FBL_RESET_1_INDEX 5
// �����1(RESET2) � ����� � �������-��������� �������� ����������� ������� - ������� ����;
#define ADDRESS_HIGH_FBL_RESET_2_INDEX 6
// �����2(RESET2) � ����� � �������-��������� �������� ����������� ������� - ������� ����;
#define ADDRESS_LOW_FBL_RESET_2_INDEX 7

// ����� ��������� � ������� - aucInputAddress[8], ��������� - struct TFunctionBlockDescriptionWork.
#define TRIGGER_TYPE_FBL_MASK 0xF0
// ����1 � ����4 � �������� �������� ����������� �������:
// 0 � ������� ������ ��� (������� ������ TSOST, ����� 1-192);
// 1 � ����� ��� (����� 1-120) ��� ������� ���������� � �������� ������ ��� (����� 121-184);
// 2 � ���� ��������������� �������� ����������� ������� (����� 1-96);
// 3 � ���������� ����� ��������� CoDeSys (����� 1-112 ������ ��� ��� ���05-16 CDS)
// 4 � ���� ��������������� �������� ����������� ������� (����� 1-192);
#define INPUT_SOURCE_FBL_MASK 0x07
// ��� � ��� �����: 1 � ������, 0 � ���������;
#define INPUT_TYPE_FBL_MASK 0x08
// �����1(SET1), �����2(SET2), �����3(RESET1), �����4(RESET2) � ����� � ������� - ��������� �������� ����������� ������� - ������� ����;
#define ADDRESS_HIGH_FBL_MASK 0xC0
// �����1(SET1), �����2(SET2), �����3(RESET1), �����4(RESET2) � ����� � �������-��������� �������� ����������� ������� - ������� ����;
#define ADDRESS_LOW_FBL_MASK 0xFF

// ������ ��������� � ������� - aucInputAddress[8], ��������� - struct TFunctionBlockDescriptionWork.
#define TRIGGER_TYPE_FBL(pointer) (((((pointer) -> ucFunctionBlockType) & TRIGGER_TYPE_FBL_MASK)) >> 4)

// ����(SET1) � �������� �������� ����������� �������.
#define INPUT_SOURCE_FBL_SET_1(pointer) (((pointer) -> aucInputAddress[INPUT_SOURCE_FBL_SET_1_INDEX]) & INPUT_SOURCE_FBL_MASK)
// ����(SET2) � �������� �������� ����������� �������.
#define INPUT_SOURCE_FBL_SET_2(pointer) (((pointer) -> aucInputAddress[INPUT_SOURCE_FBL_SET_2_INDEX]) & INPUT_SOURCE_FBL_MASK)
// ����(RESET1) � �������� �������� ����������� �������.
#define INPUT_SOURCE_FBL_RESET_1(pointer) (((pointer) -> aucInputAddress[INPUT_SOURCE_FBL_RESET_1_INDEX]) & INPUT_SOURCE_FBL_MASK)
// ����(RESET2) � �������� �������� ����������� �������.
#define INPUT_SOURCE_FBL_RESET_2(pointer) (((pointer) -> aucInputAddress[INPUT_SOURCE_FBL_RESET_2_INDEX]) & INPUT_SOURCE_FBL_MASK)

// ���(SET1) � ��� �����: 1 � ������, 0 � ���������;
#define INPUT_TYPE_FBL_SET_1(pointer) (((pointer) -> aucInputAddress[INPUT_TYPE_FBL_SET_1_INDEX]) & INPUT_TYPE_FBL_MASK)
// ���(SET2) � ��� �����: 1 � ������, 0 � ���������;
#define INPUT_TYPE_FBL_SET_2(pointer) (((pointer) -> aucInputAddress[INPUT_TYPE_FBL_SET_2_INDEX]) & INPUT_TYPE_FBL_MASK)
// ���(RESET1) � ��� �����: 1 � ������, 0 � ���������;
#define INPUT_TYPE_FBL_RESET_1(pointer) (((pointer) -> aucInputAddress[INPUT_TYPE_FBL_RESET_1_INDEX]) & INPUT_TYPE_FBL_MASK)
// ���(RESET2) � ��� �����: 1 � ������, 0 � ���������;
#define INPUT_TYPE_FBL_RESET_2(pointer) (((pointer) -> aucInputAddress[INPUT_TYPE_FBL_RESET_2_INDEX]) & INPUT_TYPE_FBL_MASK)

// �����1(SET1) � ����� � �������-��������� �������� ����������� ������� - ������� ����;
#define ADDRESS_HIGH_FBL_SET_1(pointer) (((((pointer) -> aucInputAddress[ADDRESS_HIGH_FBL_SET_1_INDEX]) & ADDRESS_HIGH_FBL_MASK)) >> 6)
// �����2(SET2) � ����� � �������-��������� �������� ����������� ������� - ������� ����;
#define ADDRESS_HIGH_FBL_SET_2(pointer) (((((pointer) -> aucInputAddress[ADDRESS_HIGH_FBL_SET_2_INDEX]) & ADDRESS_HIGH_FBL_MASK)) >> 6)
// �����3(RESET1) � ����� � �������-��������� �������� ����������� ������� - ������� ����;
#define ADDRESS_HIGH_FBL_RESET_1(pointer) (((((pointer) -> aucInputAddress[ADDRESS_HIGH_FBL_RESET_1_INDEX]) & ADDRESS_HIGH_FBL_MASK)) >> 6)
// �����4(RESET2) � ����� � �������-��������� �������� ����������� ������� - ������� ����;
#define ADDRESS_HIGH_FBL_RESET_2(pointer) (((((pointer) -> aucInputAddress[ADDRESS_HIGH_FBL_RESET_2_INDEX]) & ADDRESS_HIGH_FBL_MASK)) >> 6)

// �����1(SET1) � ����� � �������-��������� �������� ����������� ������� - ������� ����;
#define ADDRESS_LOW_FBL_SET_1(pointer) ((pointer) -> aucInputAddress[ADDRESS_LOW_FBL_SET_1_INDEX])
// �����2(SET2) � ����� � �������-��������� �������� ����������� ������� - ������� ����;
#define ADDRESS_LOW_FBL_SET_2(pointer) ((pointer) -> aucInputAddress[ADDRESS_LOW_FBL_SET_2_INDEX])
// �����3(RESET1) � ����� � �������-��������� �������� ����������� ������� - ������� ����;
#define ADDRESS_LOW_FBL_RESET_1(pointer) ((pointer) -> aucInputAddress[ADDRESS_LOW_FBL_RESET_1_INDEX])
// �����4(RESET2) � ����� � �������-��������� �������� ����������� ������� - ������� ����;
#define ADDRESS_LOW_FBL_RESET_2(pointer) ((pointer) -> aucInputAddress[ADDRESS_LOW_FBL_RESET_2_INDEX])

//-----------------------------------------------------------------------------------------------------
// ������ ��������� ��������������� ����� - ����������� ����������.
// ������� � ������� - aucInputAddress[8], ��������� - struct TFunctionBlockDescriptionWork.
// ����� - �������� �������� ����������� �������.
#define ANALOGUE_INPUT_FBL_ADDRESS_INDEX 0
// ��� �������� ����������� �������:
#define ANALOGUE_INPUT_FBL_TYPE_INDEX 0

// ������� FLOAT � ������������� �������� ������� ��� ���_��� = 0, ��� ���_��� = 1 � 10 �� ����� ��������;
// ������� FLOAT (�������� ������� ����) ��� ���_��� = 0.
#define SETPOINT_FBL_BYTE_1_INDEX 1
// ������� FLOAT (�������� ������� ����) ��� ���_��� = 0.
#define SETPOINT_FBL_BYTE_2_INDEX 2
// ������� FLOAT (�������� ������� ����) ��� ���_��� = 0.
#define SETPOINT_FBL_BYTE_3_INDEX 3
// ������� FLOAT (�������) ��� ���_��� = 0.
#define SETPOINT_FBL_BYTE_4_INDEX 4

// ��� � ���������� 0 � 31 % ��������� ��������� (�����) ����������� �������.
#define HYSTERESIS_ANALOGUE_SCALE_RANGE_FBL_INDEX 5
// cond � ������� ������������ ������.
#define COMPARISON_CONDITION_FBL_INDEX 5

// ���������� ������ "ENABLE".
// ���� � �������� �������� ����������� �������:
#define DISCRETE_INPUT_ENABLE_SOURCE_FBL_INDEX 6
// ��� � ��� �����: 1 � ������, 0 � ���������;
#define DISCRETE_INPUT_ENABLE_TYPE_FBL_INDEX 6
// ����� � ����� � �������-��������� �������� ����������� ������� - ������� ����;
#define DISCRETE_INPUT_ENABLE_ADDRESS_HIGH_FBL_INDEX 6
// ����� � ����� � �������-��������� �������� ����������� ������� - ������� ����;
#define DISCRETE_INPUT_ENABLE_ADDRESS_LOW_FBL_INDEX 7

// ����� ��������� � ������� - aucInputAddress[8], ��������� - struct TFunctionBlockDescriptionWork.
// ���_��� � ����� ������� ��������� ����������� ������� (0, 1-10):
// - 0 � ������������� �������� ������� (������� ��������� � ������ �������� FLOAT�)
// - 1 - 10 � ���������� �������� ������� (������� ��������� � ������� �������� ������� ������������ �� ������ 1-10);
#define SETPOINT_ADDRESS_FBL_MASK 0xF0
// ����� - �������� �������� ����������� �������.
#define ANALOGUE_INPUT_FBL_ADDRESS_MASK 0x7F
// ��� �������� ����������� �������:
// 0 � ������� ���������� ������ � ���������� �������� (1-96);
// 1 � ��������� �������� (1-24).
#define ANALOGUE_INPUT_FBL_TYPE_FBL_MASK 0x80

// ������� FLOAT � ������������� �������� ������� ��� ���_��� = 0, ��� ���_��� = 1 � 10 �� ����� ��������;
// ������� FLOAT (�������� LSB) ��� ���_��� = 0.
#define SETPOINT_FBL_BYTE_1_MASK 0xFF
// ������� FLOAT (�������� MSB) ��� ���_��� = 0.
#define SETPOINT_FBL_BYTE_2_MASK 0xFF
// ������� FLOAT (�������) ��� ���_��� = 0.
#define SETPOINT_FBL_BYTE_3_MASK 0xFF

// ��� � ���������� 0 � 31 % ��������� ��������� (�����) ����������� �������
// (����� �������� ������ ��� �����, ��� 7 = 0, ���� ��� 7 = 1 �� ��� = 0)
#define HYSTERESIS_ANALOGUE_SCALE_RANGE_FBL_MASK 0x7C
// cond � ������� ������������ ������:
// ��� cond = 0 ����� = 1, ���� ���������� ������ ������ �������
// ��� cond = 1 ����� = 1, ���� ���������� ������ ������ �������
#define COMPARISON_CONDITION_FBL_MASK 0x80
// ���������� ������ "ENABLE".
// ���� � �������� �������� ����������� �������:
// 0 � ������� ������ ��� (������� ������ TSOST, ����� 1-192);
// 1 � ����� ��� (����� 1-120) ��� ������� ���������� � �������� ������ ��� (����� 121-184);
// 2 � ���� ��������������� �������� ����������� ������� (����� 1-96);
// 3 � ���������� ����� ��������� CoDeSys (����� 1-112 ������ ��� ��� ���05-16 CDS)
// 4 � ���� ��������������� �������� ����������� ������� (����� 1-192);
#define DISCRETE_INPUT_ENABLE_SOURCE_FBL_MASK 0x07
// ��� � ��� �����: 1 � ������, 0 � ���������;
#define DISCRETE_INPUT_ENABLE_TYPE_FBL_MASK 0x08
// ����� � ����� � �������-��������� �������� ����������� ������� - ������� ����;
#define DISCRETE_INPUT_ENABLE_ADDRESS_HIGH_FBL_MASK 0xC0
// ����� � ����� � �������-��������� �������� ����������� ������� - ������� ����;
#define DISCRETE_INPUT_ENABLE_ADDRESS_LOW_FBL_MASK 0xFF

// ������ ��������� � ������� - aucInputAddress[8], ��������� - struct TFunctionBlockDescriptionWork.
// ����� - �������� �������� ����������� �������.
#define ANALOGUE_INPUT_FBL_ADDRESS(pointer) (((pointer) -> aucInputAddress[ANALOGUE_INPUT_FBL_ADDRESS_INDEX]) & ANALOGUE_INPUT_FBL_ADDRESS_MASK)
// ��� �������� ����������� �������:
#define ANALOGUE_INPUT_FBL_TYPE_FBL(pointer) (((pointer) -> aucInputAddress[ANALOGUE_INPUT_FBL_TYPE_INDEX]) & ANALOGUE_INPUT_FBL_TYPE_FBL_MASK)

// ������� FLOAT (�������� ������� ����) ��� ���_��� = 0.
#define SETPOINT_FBL_BYTE_1(pointer) ((pointer) -> aucInputAddress[SETPOINT_FBL_BYTE_1_INDEX])
// ������� FLOAT (�������� ������� ����) ��� ���_��� = 0.
#define SETPOINT_FBL_BYTE_2(pointer) ((pointer) -> aucInputAddress[SETPOINT_FBL_BYTE_2_INDEX])
// ������� FLOAT (�������� ������� ����) ��� ���_��� = 0.
#define SETPOINT_FBL_BYTE_3(pointer) ((pointer) -> aucInputAddress[SETPOINT_FBL_BYTE_3_INDEX])
// ������� FLOAT (�������) ��� ���_��� = 0.
#define SETPOINT_FBL_BYTE_4(pointer) ((pointer) -> aucInputAddress[SETPOINT_FBL_BYTE_4_INDEX])
// ������� FLOAT � ������������� �������� ������� ��� ���_��� = 0, ��� ���_��� = 1 � 10 �� ����� ��������;
#define SETPOINT_FBL(pointer) ((pointer) -> aucInputAddress[SETPOINT_FBL_BYTE_1_INDEX])

// ��� � ���������� 0 � 31 % ��������� ��������� (�����) ����������� �������.
#define HYSTERESIS_ANALOGUE_SCALE_RANGE_FBL(pointer) (((((pointer) -> aucInputAddress[HYSTERESIS_ANALOGUE_SCALE_RANGE_FBL_INDEX]) & HYSTERESIS_ANALOGUE_SCALE_RANGE_FBL_MASK)) >> 2)
// cond � ������� ������������ ������.
#define COMPARISON_CONDITION_FBL(pointer) (((pointer) -> aucInputAddress[COMPARISON_CONDITION_FBL_INDEX]) & COMPARISON_CONDITION_FBL_MASK)

// ���������� ������ "ENABLE".
// ���� � �������� �������� ����������� �������:
#define DISCRETE_INPUT_ENABLE_SOURCE_FBL(pointer) (((pointer) -> aucInputAddress[DISCRETE_INPUT_ENABLE_SOURCE_FBL_INDEX]) & DISCRETE_INPUT_ENABLE_SOURCE_FBL_MASK)
// ��� � ��� �����: 1 � ������, 0 � ���������;
#define DISCRETE_INPUT_ENABLE_TYPE_FBL(pointer) (((pointer) -> aucInputAddress[DISCRETE_INPUT_ENABLE_TYPE_FBL_INDEX]) & DISCRETE_INPUT_ENABLE_TYPE_FBL_MASK)
// ����� � ����� � �������-��������� �������� ����������� ������� - ������� ����;
#define DISCRETE_INPUT_ENABLE_ADDRESS_HIGH_FBL(pointer) (((((pointer) -> aucInputAddress[DISCRETE_INPUT_ENABLE_ADDRESS_HIGH_FBL_INDEX]) & DISCRETE_INPUT_ENABLE_ADDRESS_HIGH_FBL_MASK)) >> 6)
// ����� � ����� � �������-��������� �������� ����������� ������� - ������� ����;
#define DISCRETE_INPUT_ENABLE_ADDRESS_LOW_FBL(pointer) ((pointer) -> aucInputAddress[DISCRETE_INPUT_ENABLE_ADDRESS_LOW_FBL_INDEX])

// ��������� ��������������� ������ ������������ ���������� ����������� ���������� �������� �� HMI.
struct TFunctionBlockWorkingData
{
    // ���� ���������� ��������������������.
    bool fbFunctionBlockIsActive;
    // ������� ����������� ������� �������� ���.
    unsigned char ucDelay;
};


#endif // FUNCTIONBLOCKS_H_INCLUDED

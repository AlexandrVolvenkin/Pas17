#ifndef DISCRETEINPUT_H_INCLUDED
#define DISCRETEINPUT_H_INCLUDED

#include "PasNewConfig.h"

#define DISCRETE_INPUT_RELAY_OUTPUT_QUANTITY DO_VALUE_BIT_ARRAY_LENGTH
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
struct TDiscreteInputDescriptionWorkPackOne
{
    // ���� 1:
    // D0-D4 - ��� (������): 0,1-24(0-��� ���.�� �����), D5-�� (��� ��������): 0-��,1-��.
    // D6 - ���1 � ��� ������������ ��������� ����������� �������.
    // D7 - ���2 � ��� ������������ ��������� ����������� �������.
    unsigned char 	ucTalTkGrp;
    // ���� 2:
    // ������ �� ��1:D0=1 ���� ���.�� ���� 1,D0=0-��� ��� �� ���� 1 � �.�.
    // ���� 3,4,5,6,7,8,9:
    // ������ �� ��2,��3,��4,��5,MP6,MP7,MP8:������.��1.
    unsigned char aucRelayOut[8];
    // ���� 10:
    // D7 - ������, D6 - ���:0-������.� ���,1-� ���.���.
    // D0-D5 - ���:������� ������� �������� ������ � ��.���(0-60).
    unsigned char ucDelay;
    // ���� 11 - ����������� ����� ($ff+����� ������ 1-10)    *).
    unsigned char ucCrc;
};
#pragma pack(pop)

// ��������� ���������� �� ���������� ������� ��������.
// ��������: PAS_05_PO.DOC, ��� - 3,
// 3. ���� ������ ��������� ���������� ��������
struct TDiscreteInputDescriptionWork
{
    // ���� 1:
    // D0-D4 - ��� (������): 0,1-24(0-��� ���.�� �����), D5-�� (��� ��������): 0-��,1-��.
    // D6 - ���1 � ��� ������������ ��������� ����������� �������.
    // D7 - ���2 � ��� ������������ ��������� ����������� �������.
    unsigned char 	ucTalTkGrp;
    // ���� 2:
    // ������ �� ��1:D0=1 ���� ���.�� ���� 1,D0=0-��� ��� �� ���� 1 � �.�.
    // ���� 3,4,5,6,7,8,9:
    // ������ �� ��2,��3,��4,��5,MP6,MP7,MP8:������.��1.
    unsigned char aucRelayOut[8];
    // ���� 10:
    // D7 - ������, D6 - ���:0-������.� ���,1-� ���.���.
    // D0-D5 - ���:������� ������� �������� ������ � ��.���(0-60).
    unsigned char ucDelay;
    // ���� 11 - ����������� ����� ($ff+����� ������ 1-10)    *).
    unsigned char ucCrc;
};

// ����� ��������� ��������� ���������� �� ���������� ������� ��������.
// struct TDiscreteInputDescriptionWork
// ��������: PAS_05_PO.DOC, ��� - 3,
// 3. ���� ������ ��������� ���������� ��������
// ���� 1: ucTalTkGrp.
// D0-D4 - ��� (������), ����� ������ �� �������� �����.
#define LIGHT_BOARD_CELL_NUMBER_DISC_MASK 0x1F
// D5-�� (��� ��������): 0-��,1-��
#define CONTACT_TYPE_DISC_MASK 0x20
// D6,D7 - ��� ��� ������������: 0-��� ����.,1-����.,2-����.,3-�����.(����.����)
// ���1 � ��� ������������ ��������� ����������� �������
#define SIGNALING_TYPE_1_DISC_MASK 0x40
// ���2 � ��� ������������ ��������� ����������� �������
#define SIGNALING_TYPE_2_DISC_MASK 0x80
// ���� 10: ucDelay.
// D0-D5 - ���: ������� ������� �������� ������ � ��.���(0-60)
#define DELAY_TIME_SETPOINT_DISC_MASK 0x3F
// D6 - ��� ������������ ������� ��������: 0-������.� ���,1-� ���.���.
#define DELAY_TIME_DISCRETENESS_DISC_MASK 0x40
// D7 - ��� =1 ������������, 0 � �� ������������.
#define ARCHIVE_DISC_MASK 0x80

// ��� (������), ����� ������ �� �������� �����.
#define LIGHT_BOARD_CELL_NUMBER_DIS(pointer) (((pointer) -> ucTalTkGrp) & LIGHT_BOARD_CELL_NUMBER_DISC_MASK)
// �� (��� ��������): 0-��,1-��
#define CONTACT_TYPE_DIS(pointer) (((pointer) -> ucTalTkGrp) & CONTACT_TYPE_DISC_MASK)
// ��� ��� ������������: 0-��� ����.,1-����.,2-����.,3-�����.(����.����)
// ���1 � ��� ������������ ��������� ����������� �������
#define SIGNALING_TYPE_1_DIS(pointer) (((pointer) -> ucTalTkGrp) & SIGNALING_TYPE_1_DISC_MASK)
// ���2 � ��� ������������ ��������� ����������� �������
#define SIGNALING_TYPE_2_DIS(pointer) (((pointer) -> ucTalTkGrp) & SIGNALING_TYPE_2_DISC_MASK)
// ���: ������� ������� �������� ������ � ��.���(0-60)
#define DELAY_TIME_SETPOINT_DIS(pointer) (((pointer) -> ucDelay) & DELAY_TIME_SETPOINT_DISC_MASK)
// ��� ������������ ������� ��������: 0-������.� ���,1-� ���.���.
#define DELAY_TIME_DISCRETENESS_DIS(pointer) (((pointer) -> ucDelay) & DELAY_TIME_DISCRETENESS_DISC_MASK)
// �����.
#define ARCHIVE_DIS(pointer) ((pointer) -> ucDelay) & ARCHIVE_DISC_MASK

// ����������� �����.
#define CRC_DIS(pointer) ((pointer) -> ucCrc)

// ��������� ��������������� ������ ������������ ���������� ����������� ���������� �������� �� HMI.
struct TDiscreteInputWorkingData
{
    // ���� ���������� ����������� �����.
    bool fbDiscreteInputIsActive;
    // ������� ����������� ������� �������� ���.
    unsigned char ucDelay;
};

#endif // DISCRETEINPUT_H_INCLUDED

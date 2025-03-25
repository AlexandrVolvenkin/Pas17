#ifndef MODULEMTVI5_H_INCLUDED
#define MODULEMTVI5_H_INCLUDED

#include "PasNewConfig.h"


//============================================================================
// MTVI5
// ������������ ��������� ����� ������� 100mc.
// ����� ������� � ������� ����������� ������ ������������ ����� -
// (������������ ��������� ����� ������� 100mc * MTVI5_LOOP_SKIP_QUANTITY).
#define MTVI5_LOOP_SKIP_QUANTITY 5
// �������� �� 1.5 �������, ��� ������ � EEPROM ������.
// (������������ ��������� ����� ������� 100mc * MTVI5_WRITE_WAIT_QUANTITY).
#define MTVI5_WRITE_WAIT_QUANTITY 15
// ��� ���� ������ MTVI5.
//#define MODULE_TYPE_MTVI5 0x0E
#define MAX_MTVI5_MODULES_QUANTITY 4
// ���������� ����������� ������� � ������.
#define MTVI5_ANALOG_OUTPUT_QUANTITY 6
#define MTVI5_STAT_BIT_QUANTITY 8
// ���������� ����������� � ������.
#define MTVI5_REGULATOR_QUANTITY 6
// ������������ ������������� ���.
#define MTVI5_MAX_CODE 16383
// ���� ������� ������������� �������������� ����.
#define MTVI5_MAX_CODE_ONE_PERCENT (MTVI5_MAX_CODE / PERCENT_100)
// ������ ���������� � ������� ������ ������� �������.
#define MTVI5_MODULE_BAD_BIT_ARRAY_LENGTH 1

// ��������� �������� - ���������� �����.
#define MTVI5_FSM_STATE_TRANSFER 0x00
// ��������� �������� - ������ ���� ������ �����������.
#define MTVI5_FSM_STATE_WRITE_REGULATOR_DATABASE 0x01
// ��������� �������� - ������ ���� ������ ����������� � ����������.
#define MTVI5_FSM_STATE_WRITE_REGULATOR_AND_PSP_DATABASE_REGULATOR 0x02
#define MTVI5_FSM_STATE_WRITE_REGULATOR_AND_PSP_DATABASE_PSP 0x03
#define MTVI5_FSM_STATE_WRITE_REGULATOR_AND_PSP_DATABASE_PSP_WRITE_WAITING 0x04



#define MTVI5_EXCHANGE_SPEED_IN_HZ SPEED_IN_HZ//200000UL//

// ���� ��������� ����������� �CONT_ST�.
// D0 - ��/�� � 0 � ������, 1 � ��������������;
#define MTVI5_STAT_AUTO_OR_MANUAL_BIT 0
// D1 - ��ϻ � 0 - ����� �� �� �������, 1 - ����� �� �������, ��� ��������� ������ �� ������������� ��������������� ��� 0=1;
#define MTVI5_STAT_NO_AP_OR_AP_BIT 1
// D2 - ����������� � 0 - ����� ����������� �� �������, 1 - ����� ����������� �������;
#define MTVI5_STAT_MODE_PROGRAMM_BIT 2
// D3 - ��.�����.���.������ 2 (� ���������� ��� 1 � ������, ��������, ������, �������� �� �������� ��� �� ���������);
#define MTVI5_STAT_INPUT_DESCRETE_CONTROL_2_BIT 3
// D4 - ��.�����.���.������ 3 (� ���������� ��� 1 � ������, ��������, ������, �������� �� �������� ��� �� ���������);
#define MTVI5_STAT_INPUT_DESCRETE_CONTROL_3_BIT 4
// D5 - ���.�����.������ 1 (� ���������� ��� 1 � ������ ���, ��� ������ ������ - 0);
#define MTVI5_STAT_OUTPUT_DESCRETE_CONTROL_1_BIT 5
// D6 - ������ ��� � 0 � �����, 1 - ����� ����� IOUT, ������������� ���
#define MTVI5_STAT_DAC_ERROR_BIT 6
// D7 - ������ PV - 0 � �����, 1 � ���������������;
#define MTVI5_STAT_PV_ERROR_BIT 7
// ��� ������� �� ����������� ����� �������.
#define MTVI5_DATA_EXCHANGE_COMMAND 0x44
// ��� ������� �� �������� ����������� ��������� �����������.
#define MTVI5_RESTORE_OUTS_COMMAND 0x45
#define MTVI5_DATA_NOT_READY 0x24
#define MTVI5_DATA_BASE_ERROR 0x25
// ��� ����� ��� ���������� �������, ��������� �������� ����������� ��������� �����������.
#define MTVI5_LOAD_OUTS 0x26
// ������ ������ ������� � ��� ������ ����.
#define MTVI5_DAC_LINK_ERROR 0x27
// ������ ������ ������� � ��� ������ ����.
#define MTVI5_OUT_DAC_ERROR 0x28
// ������ ������ ������� � ��� ������ ����.
#define MTVI5_OUT_DAC_ERROR_OFFSET 0x01
// ������ ������ ������� � ��� ������ ����.
#define NO_ERROR_OFFSET 0x00
// ������ ������ ������� ������ ����.
#define MTVI5_BAD_ANSWER_ERROR_OFFSET 0x00
// ������ ������� ��� ������ ����.
#define MTVI5_DAC_LINK_ERROR_OFFSET 0x0A
// ������ ���� ������ ������ ����.
#define MTVI5_DATA_BASE_ERROR_OFFSET 0x0B
// ������ ���� ������ ������ ����.
#define MVAI_REPER_POINTS_DATA_BASE_ERROR_OFFSET 0x0C
// ��� ������� �� ������ ���� ������ ����������� �� ������.
#define MTVI5_GET_REGULATOR_DATA_BASE_COMMAND 0x84
// ��� ������� �� ������ ���� ������ ����������� ���������� �� ������.
#define MTVI5_GET_PSP_DATA_BASE_COMMAND 0x85
// ��� ������� �� ������ ���� ������ ����������� � ������.
#define MTVI5_SET_REGULATOR_DATA_BASE_COMMAND 0x81
// ��� ������� �� ������ ���� ������ ����������� ���������� � ������.
#define MTVI5_SET_PSP_DATA_BASE_COMMAND 0x82
// ������ ������ ����������.
#define REGULATOR_MODE_MANUAL 1
#define REGULATOR_MODE_AUTO_NORMAL 2
#define REGULATOR_MODE_AUTO_PROGRAMM_SET_POINT 3
#define REGULATOR_MODE_AUTO_FORCE_CONTROL 4
// ���� ucControl ��������� ����������.
// DO - �������� ������� PV (������������� ��������� ��� TYPE 1 ��� ����� ����������� ������� ��� TYPE 0) - ����� � ����� 0 ���������:
// DO = 0 � ���������� ������� ������ AIn (� ��� 0-16383 �����. �� �����);
// DO = 1 � ��������� �������� �alc_cds (� ������ CoDeSys ���������� �
// 0-100%, �������������� � ��� 0-16383);
// D1 � D7 � ������.
#define REGULATOR_CONTROL_PV_SOURSE_IS_CALC_CDS 0

// ;____________________________________________________________________________________
// ;	�������� ��� -> ����				����� ��� <- ����
// ;0)	N-���������� ������ � ������ ������	|	���
// ;1)	PV - �� 2 ����� (��� 0-16383)		|OUT - �� 2 ����� (��� 0-16383)
// ; 	�� ��������� (6*2=12 ������)		|�� ��������� (6*2=12 ������)
// ;2) SP-(���.����) ��� OUT (���.����) 		|SP-������
// ;	�� 2 ����� �� ���-� (2*6=12 ����� ���)	|�� 2 ����� �� ���-� (2*6=12 ����� ���)
// ;3) STAT-1 ���� (������� ��������)		|STAT-1 ���� (������� ��������)
// ;	�� 1 ����� �� ���-� (6*1=6 ������ ���)	|�� 1 ����� �� ���-� (6*1=6 ������ ���)
// ;4)	���� ��������� SP, OUT-1 ����		|	���
// ;5) �� ���������� ������ 1,2,3,4 - 2 �����	|	�� ���������� ������ 1,2,3,4 - 2 �����
// ;6) ��� ���������� $7e - ��			|	��� ���������� $7e - ��
// ;____________________________________________________________________________________
// ;���������� ������ � ������ ������ ������������ ����������� �������������������
// ;����������� �� 1 �� 6 �� 5 ������ �� ��������� - 30+1 ���� ���

// ������ ������ � ������ ������.
#define MTVI5_FRAME_LENGTH_BYTE_QUANTITY 1
// ������ ������ ������ ���������� � ������.
#define MTVI5_ONE_REGULATOR_FRAME_BYTE_QUANTITY 5
// ������ ������ ����� ��������� SP, OUT-1 ����.
#define MTVI5_SP_OUT_SET_BYTE_QUANTITY 1
// ���������� �������� ����������� ����� �� ������ ����� ������������� ��������� (PV=1-96) - ������������.
#define MTVI5_PV_ERROR 0xFE00
#define MTVI5_RESTORE_OUTS_BUFF_LENGTH (MTVI5_ANALOG_OUTPUT_QUANTITY * sizeof(short))
#define MTVI5_RESTORE_OUTS_FLAG_BYTE_QUANTITY 1

//// ��������� ������ ���������� ���������� ����������� �CONT_AV�
//struct TMtvi5ContAvData
//{
//    float fSp;
//    float fOut;
//    float fPv;
//};

// ��������� ������ ��� �������������� ������ ������������ � ������.
struct TMtvi5TransmitData
{
    unsigned short usPv;
    unsigned short usSpOut;
};
#define MTVI5_CONT_AV_BYTE_ARRAY_LENGTH MTVI5_ANALOG_OUTPUT_QUANTITY * sizeof(struct TMtvi5ContAvData)
#define MTVI5_CONT_ST_BYTE_ARRAY_LENGTH MTVI5_ANALOG_OUTPUT_QUANTITY * 1

#endif // MODULEMTVI5_H_INCLUDED

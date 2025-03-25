#ifndef MODULEMVDS9_H_INCLUDED
#define MODULEMVDS9_H_INCLUDED

//============================================================================
// MVDS9
//#define MODULE_TYPE_MVDS9 0x0A

#define MVDS9_DISCRETE_INPUT_QUANTITY 12
// ��������� 12 ���������� ������ �� ������ ��������� � ��� ������.
// ���� 1:D0,D1-����.��.1, D2,D3-����.��.2,D4,D5-����.��.3,D6,D7-����.��.4
// ���� 2:D0,D1-����.��.5, D2,D3-����.��.6,D4,D5-����.��.7,D6,D7-����.��.8
// ���� 3:D0,D1-����.��.9, D2,D3-����.��.10,D4,D5-����.��.11,D6,D7-����.��.12
// ������ ���� (D0,D2,D4,D6) =0 - ���������� (OFF), =1 - �������� (ON)
// �������� ���� (D1,D3,D5,D7) =0 - ����������, =1 - ������������
#define MVDS9_DISCRETE_INPUT_BYTE_QUANTITY 3
#define MVDS9_DISCRETE_INPUT_BITS_IN_BYTE_QUANTITY 8
// ���� ������� �� 8 ��������� �� 2 �����.
#define MVDS9_DATA_BASE_BLOCK_LENGTH 2
// ���������� ���� ���������� ������� � ������� ������� - ��������� ������� ���������� ��������.
#define MVDS9_DI_VALUE_BIT_ARRAY_LENGTH MVDS9_DISCRETE_INPUT_QUANTITY
// ���������� ���� ���������� ������� � ������� ������� - ������ ������ ��������������� ���������� ��������.
#define MVDS9_BAD_DI_BIT_ARRAY_LENGTH MVDS9_DISCRETE_INPUT_QUANTITY
// ���������� ���� ���������� ������� � ������� ������� - ������ ������ ������� ������� ���-05.
#define MVDS9_MODULE_BAD_BIT_ARRAY_LENGTH 1
// ������� - ������ �� ����������� �����.
#define MVDS9_GET_DISCRETE_INPUT_DATA_COMMAND 0x42
#define MVDS9_DATA_READY 0x42
#define MVDS9_DATA_NOT_READY 0x24
#define MVDS9_GET_DISCRETE_INPUT_DATA_COMMAND_ANSWER_LENGTH 5
// ������ �� ����� ��: $82 .
#define MVDS9_GET_DATA_BASE_COMMAND 0x82
#define MVDS9_GET_DATA_BASE_COMMAND_ANSWER_LENGTH 6
// ������ �� �������� ��: $81.
#define MVDS9_SET_DATA_BASE_COMMAND 0x81
#define MVDS9_SET_DATA_BASE_COMMAND_ANSWER_LENGTH 7

#define MVDS9_CHECK_DATA_BASE_WRITE_ANSWER_OK 0x7E
#define MVDS9_CHECK_DATA_BASE_WRITE_ANSWER_ERROR 0xE7

// ��������� ���� ������ ������ ������ ���������� ������.
struct TMvds9ModuleDataBase
{
    unsigned char ucMvds9ModuleDataBaseByte1;
    unsigned char ucMvds9ModuleDataBaseByte2;
};


#endif // MODULEMVDS9_H_INCLUDED

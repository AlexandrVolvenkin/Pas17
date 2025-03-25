//-----------------------------------------------------------------------------------------------------
//  Source      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-----------------------------------------------------------------------------------------------------
#ifndef CMODULEMRXX_H
#define CMODULEMRXX_H

//-----------------------------------------------------------------------------------------------------
#include <stdint.h>
#include "PasNewConfig.h"
#include "Driver.h"
#include "DataTypes.h"
//#include "Timer.h"
//#include "Task.h"
#include "Dfa.h"

//-----------------------------------------------------------------------------------------------------
enum
{
    MRXX_FSM_STATE_CHANGE_PROTOCOL = 0x00,
// ��������� �������� - ������ ���� ������.
    MRXX_FSM_STATE_WRITE_DATABASE = 0x01,
// ��������� �������� - ���������� �����.
    MRXX_FSM_STATE_TRANSFER = 0x02,
};

enum
{
    MRXX_IDDLE = 0,
    MRXX_CHANGE_PROTOCOL,
    MRXX_WRITE_DATA_BASE,
    MRXX_WRITE_DATA_BASE_CHECK,
    MRXX_DATA_EXCHANGE,
};


enum
{
    MRXX_RELAY_STATE_OFFSET = 2,
// ����� ��������� ������� 8 ���� ������. ������ ��53.
// ���� - 2: ��������� ������� ����, ���� ��� = 1 - �������� ���������.
    MRXX_STAT_DO_SHORT_CIRCUIT_BIT_OFFSET = 3,
// ���� - 3: ��������� ������� ����, ���� ��� = 1 - �����.
    MRXX_STAT_DO_BREACK_BIT_OFFSET = 4,
// ��� ������� - ���������-���������� ����.
    MRXX_ERROR_CODE_RELAY_ON_OFF = 0x01,
// ��� ������� - ���������-���������� ����.
    MRXX_ERROR_CODE_RELAY_SHORT_CIRCUIT = 0x04,
// ��� ������� - ���������-���������� ����.
    MRXX_ERROR_CODE_RELAY_BREACK = 0x08,
};

enum
{
// ���� ���������� �� ����� f_bou.
// ������������ � �������� ������.
    GLOBAL_KVIT_PC_BIT = 0,
// ����� � �������� ������.
    GLOBAL_RESET_PC_BIT = 1,
// ������������ � ���������� ������ ����.
    GLOBAL_KVIT_BUTTON_BIT = 2,
// ����� � ���������� ������ ����.
    GLOBAL_RESET_BUTTON_BIT = 3,
// ����� ����������.
    GLOBAL_BLOCK_BIT = 4,
};

enum
{
// ���� ��������� ���� ������ �������� ���������� ��������.
// ���� 0 (���.,���,���,���,���,���,��/��,���).
// ��� � ��� ������ 0 � ���������������, 1 � �����������
    OUTPUT_TYPE_MASK = 0x01,
// ��/��: ��� ������������ ������: 1 � � �����������, 0 � ��� ����������;
// ��� ���������������� ������: - 1 ������������ � �������� (����),
// 0 � ������������ � ��������� � ���������� �������� (����);
    BLOCK_ALARM_TYPE_MASK = 0x02,
// ��� � ����� � �������� ������: 1 � ����, 0 � ���;
    PC_RESET_MASK = 0x04,
// ��� � ������������ � �������� ������: 1 � ����, 0 � ���; (������ ��� ����. ������);
    PC_KVIT_MASK = 0x08,
// ��� � ����� � ����������� ������ ����: 1 � ����, 0 � ���;
    BUTTON_RESET_MASK = 0x10,
// ��� � ������������ � ����������� ������ ����: 1 � ����, 0 � ��� (������ ��� ����. ������);
    BUTTON_KVIT_MASK = 0x20,
// D7 - ��� =1 ������������, 0 � �� ������������.
    ARCHIVE_MRXX_MASK = 0x80,
};

enum
{
    DATA_EXCHANGE_COMMAND = 0x42,
    DATA_READY = 0x42,
//        // ���������� ���������� ������
//        DATA_EXCHANGE_OK	= 0x7E,
    DATA_EXCHANGE_OK_LENGTH = 1,
    // ������ �� ������
    DATA_NOT_READY	= 0x24,
    // �� ����������
    DBASE_ERR	= 0x24,
    // ������ �� �������� ���� ������ �� ��� � �� - $81.
    MRXX_SET_DATA_BASE_COMMAND = 0x81,
    MRXX_SET_DATA_BASE_COMMAND_ERROR = 0xE7,
    // ���������� ���� � ������ ������ �������, �����, CRC - 2 ����� � ����� �������������.
    MRXX_GET_DISCRETE_OUTPUT_DATA_COMMAND_ANSWER_LENGTH = 5,
    // ���������� ���� � ������ ������ ���� ������, �����, CRC - 2 ����� � ����� �������������.
    MRXX_SET_DATA_BASE_COMMAND_ANSWER_LENGTH = 8,
    // ����� ��������� ��������� �� ���������� PREAMBLE_LENGTH + TWO_BYTE_CRC_LENGTH.
    DATA_EXCHANGE_COMMAND_ANSWER_LENGTH = 5,
    PRIAMBLE_LENGTH = 2,
    // 2 - �������� �� ������ � ������� RX SPI. 0 - ������� �������, 1 - "���" ������� �������.
    DATA_BYTE_OFFSET = 2,
    // 1 - �������� �� ������� � ������� RX SPI.
    COMMAND_BYTE_OFFSET = 1,
    COMMAND_BYTE_LENGTH = 1,
};

enum
{
    SPI_MT_PRIAMBLE_LENGTH = 2,
    SPI_MT_FUNCTION_CODE_OFFSET = 0,
    SPI_MT_LENGTH_OFFSET = 1,
    SPI_MT_DATA_OFFSET = 2,
    SPI_MT_RX_OFFSET = 1,
};


//-----------------------------------------------------------------------------------------------------




////-----------------------------------------------------------------------------------------------------
//class CModuleMrXXDriver : public CDriver, public CDfa
//{
//public:
////    enum
////    {
////        DATA_EXCHANGE_COMMAND = 0x42,
////        DATA_READY = 0x42,
////        // ���������� ���������� ������
////        DATA_EXCHANGE_OK	= 0x7E,
////        DATA_EXCHANGE_OK_LENGTH = 1,
////        // ������ �� ������
////        DATA_NOT_READY	= 0x24,
////        // �� ����������
////        DBASE_ERR	= 0x24,
////        // ������ �� �������� ���� ������ �� ��� � �� - $81.
////        MRXX_SET_DATA_BASE_COMMAND = 0x81,
////        // ���������� ���� � ������ ������ �������, �����, CRC - 2 ����� � ����� �������������.
////        MRXX_GET_DISCRETE_OUTPUT_DATA_COMMAND_ANSWER_LENGTH = 5,
////        // ���������� ���� � ������ ������ ���� ������, �����, CRC - 2 ����� � ����� �������������.
////        MRXX_SET_DATA_BASE_COMMAND_ANSWER_LENGTH = 8,
////        // ����� ��������� ��������� �� ���������� PREAMBLE_LENGTH + TWO_BYTE_CRC_LENGTH.
////        DATA_EXCHANGE_COMMAND_ANSWER_LENGTH = 5,
////        PRIAMBLE_LENGTH = 2,
////        // 2 - �������� �� ������ � ������� RX SPI. 0 - ������� �������, 1 - "���" ������� �������.
////        DATA_BYTE_OFFSET = 2,
////        // 1 - �������� �� ������� � ������� RX SPI.
////        COMMAND_BYTE_OFFSET = 1,
////        COMMAND_BYTE_LENGTH = 1,
////    };
////
//    CModuleMrXXDriver(uint8_t uiType);
//    CModuleMrXXDriver();
//    virtual ~CModuleMrXXDriver();
////    int ChangeProtocol(TModuleContext *pxModuleContext);
//    void Allocate(TMemoryAllocationConext &xMemoryAllocationConext);
//    uint8_t WriteDataBase(void);
//    uint8_t DataExchange(void);
//    uint8_t Exchange(void);
//    void Fsm(void);
//
//protected:
//    uint8_t m_uiAddress;
//    uint8_t *m_puiRxBuffer;
//    uint8_t *m_puiTxBuffer;
//    uint8_t *m_puiDiscreteInputs;
//    uint8_t *m_puiDiscreteOutputState;
//    TDiscreteOutputControl *m_pxDiscreteOutputControl;
//    TOutputData *m_pxDiscreteOutputDataBase;
//    uint8_t m_uiBadAnswerCounter;
//};
//////-----------------------------------------------------------------------------------------------------
////#endif // CMODULEMRXX_H









//#ifndef MODULEMRXX_H_INCLUDED
//#define MODULEMRXX_H_INCLUDED
//
//
//#include "PasNewConfig.h"


//============================================================================
// MR
// ���� ���� ������.
//#define MODULE_TYPE_MR53 0x03
//#define MODULE_TYPE_MR54 0x04
//#define MODULE_TYPE_MR55 0x05
//#define MODULE_TYPE_MR51 0x0C
// ���������� ������� ���������� ����.
#define MRXX_DISCRETE_OUTPUT_QUANTITY 8
// ���������� ���� ���������� ����� ������� � ������� �������� Modbus.
#define MRXX_DO_VALUE_BIT_ARRAY_LENGTH MRXX_DISCRETE_OUTPUT_QUANTITY
#define MRXX_SOST_DO_BIT_ARRAY_LENGTH MRXX_DISCRETE_OUTPUT_QUANTITY
#define MRXX_BAD_DO_BIT_ARRAY_LENGTH MRXX_DISCRETE_OUTPUT_QUANTITY
// � ������ MRXX_STAT_DO_BIT_ARRAY ���������� ������ ���� �������� 2 ����:
// 1 - �����; 2 - ���������.
#define MRXX_STAT_DO_ONE_OUTPUT_LENGTH 2
#define MRXX_STAT_DO_BIT_ARRAY_LENGTH (MRXX_DISCRETE_OUTPUT_QUANTITY * MRXX_STAT_DO_ONE_OUTPUT_LENGTH)
#define MRXX_MODULE_BAD_BIT_ARRAY_LENGTH 1

// ������ �� ����������� �����-$42.
#define MRXX_GET_DISCRETE_OUTPUT_DATA_COMMAND 0x42
// $42-������ ������ (���).
#define MRXX_DATA_READY 0x42
// $24-������ ��, ����� �����.
#define MRXX_DATA_BASE_ERROR 0x24
//// ������ �� �������� ���� ������ �� ��� � �� - $81.
//#define MRXX_SET_DATA_BASE_COMMAND 0x81
//// ���������� ���� � ������ ������ �������, �����, CRC - 2 ����� � ����� �������������.
//#define MRXX_GET_DISCRETE_OUTPUT_DATA_COMMAND_ANSWER_LENGTH 5
//// ���������� ���� � ������ ������ ���� ������, �����, CRC - 2 ����� � ����� �������������.
//#define MRXX_SET_DATA_BASE_COMMAND_ANSWER_LENGTH 8
//
//// ��������� �������� - ���������� �����.
//#define MRXX_FSM_STATE_TRANSFER 0x00
//// ��������� �������� - ������ ���� ������.
//#define MRXX_FSM_STATE_WRITE_DATABASE 0x01
//// ����� ��������� ������� 8 ���� ������. ������ ��53.
//// ���� - 2: ��������� ������� ����, ���� ��� = 1 - �������� ���������.
//#define MRXX_STAT_DO_SHORT_CIRCUIT_BIT_OFFSET 0
//// ���� - 3: ��������� ������� ����, ���� ��� = 1 - �����.
//#define MRXX_STAT_DO_BREACK_BIT_OFFSET 1
//// ��� ������� - ���������-���������� ����.
//#define MRXX_ERROR_CODE_RELAY_ON_OFF 0x01
//// ��� ������� - ���������-���������� ����.
//#define MRXX_ERROR_CODE_RELAY_SHORT_CIRCUIT 0x04
//// ��� ������� - ���������-���������� ����.
//#define MRXX_ERROR_CODE_RELAY_BREACK 0x08
//
//// ���� ���������� �� ����� f_bou.
//// ������������ � �������� ������.
//#define GLOBAL_KVIT_PC_BIT 0
//// ����� � �������� ������.
//#define GLOBAL_RESET_PC_BIT 1
//// ������������ � ���������� ������ ����.
//#define GLOBAL_KVIT_BUTTON_BIT 2
//// ����� � ���������� ������ ����.
//#define GLOBAL_RESET_BUTTON_BIT 3
//// ����� ����������.
//#define GLOBAL_BLOCK_BIT 4
//
//// ���� ��������� ���� ������ �������� ���������� ��������.
//// ���� 0 (���.,���,���,���,���,���,��/��,���).
//// ��� � ��� ������ 0 � ���������������, 1 � �����������
//#define OUTPUT_TYPE_MASK 0x01
//// ��/��: ��� ������������ ������: 1 � � �����������, 0 � ��� ����������;
//// ��� ���������������� ������: - 1 ������������ � �������� (����),
//// 0 � ������������ � ��������� � ���������� �������� (����);
//#define BLOCK_ALARM_TYPE_MASK 0x02
//// ��� � ����� � �������� ������: 1 � ����, 0 � ���;
//#define PC_RESET_MASK 0x04
//// ��� � ������������ � �������� ������: 1 � ����, 0 � ���; (������ ��� ����. ������);
//#define PC_KVIT_MASK 0x08
//// ��� � ����� � ����������� ������ ����: 1 � ����, 0 � ���;
//#define BUTTON_RESET_MASK 0x10
//// ��� � ������������ � ����������� ������ ����: 1 � ����, 0 � ��� (������ ��� ����. ������);
//#define BUTTON_KVIT_MASK 0x20
//// D7 - ��� =1 ������������, 0 � �� ������������.
//#define ARCHIVE_MRXX_MASK 0x80

// ��������� ��������� ������ �����������������.
struct TMrXXOneChannelDataBase
{
    // ���� 0 (���.,���,���,���,���,���,��/��,���).
    // ��� � ��� ������ 0 � ���������������, 1 � �����������;
    // ��/��:
    // ��� ������������ ������: 1 � � �����������, 0 � ��� ����������;
    // ��� ���������������� ������: - 1 ������������ � �������� (����), 0 � ������������ � ��������� � ���������� �������� (����);
    // ��� � ������������ � ����������� ������ ����: 1 � ����, 0 � ��� (������ ��� ����. ������);
    // ��� � ����� � ����������� ������ ����: 1 � ����, 0 � ���;
    // ��� � ������������ � �������� ������: 1 � ����, 0 � ���; (������ ��� ����. ������);
    // ��� � ����� � �������� ������: 1 � ����, 0 � ���;
    unsigned char ucControl;
    // ����1 - ��_��� - ����� �������� ������� �������� ������������.
    unsigned char ucExternalKvitAddress;
    // ����2 - ��_��� - ����� �������� ������� �������� ������.
    unsigned char ucExternalResetAddress;
};

//#endif // MODULEMRXX_H_INCLUDED

//-----------------------------------------------------------------------------------------------------
#endif // CMODULEMRXX_H

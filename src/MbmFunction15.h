#ifndef MBMFUNCTION15_H_INCLUDED
#define MBMFUNCTION15_H_INCLUDED

#include "PasNewConfig.h"

// ����� ������� � �������� ���������� �������� � ������� ������� DI_value (0-512) ��� TUST D3 = 0
// ��� �������� ���������� ����� � ������� ������� Sost_DO (0-1536) ��� TUST D3 = 1.
#define MBM_FUNCTION_15_SOURCE_BIT_ADDRESS_ARRAY_LENGTH 32
// ����� ���� ������� - "������������".
#define MBM_FUNCTION_15_KVIT_BIT_FUNCTION_5_ADDRESS 0x00
// ����� ���� ������� - "�����".
#define MBM_FUNCTION_15_RESET_BIT_FUNCTION_5_ADDRESS 0x01
// ����� ���� ������ ��������� �������� ������� � ����� ucTust.
// D3 = 0 � ������� ������� �� ������� DI_value
// D3 = 1 � ���������� ������ �� ��������, ������� � Sost_DO
#define TUST_DI_VALUE_OR_SOST_DO_SOURCE_BIT_MASK 0x08
#define TUST_DI_VALUE_OR_SOST_DO_SOURCE_OFFSET 1000
// ����� ���� - ���������� ������������ � ����� ucTust.
// D4 � �� (�/�).
#define TUST_KVIT_PC_ENABLE_BIT_MASK 0x10
// ����� ���� - ���������� ������ � ����� ucTust.
// D5 � �� (�/�).
#define TUST_RESET_PC_ENABLE_BIT_MASK 0x20

//-----------------------------------------------------------------------------------------------------
// ��� ������ PSS7.
#define MODULE_TYPE_PSS7 0x06

// ��������� ���������� �������������� ������ ���̻, ������� 15, � ���� ������.
struct TMbmFunction15
{
    unsigned char ucModbusFunction; // ��� ������� ��������� MODBUS (1, 2, 3, 4).
    unsigned char ucModbusSlaveAddress; // ����� SLAVE � ������� MS.
    unsigned char ucAddressH; // ��������� ����� ������ � ������� � ������� ����.
    unsigned char ucAddressL; // ��������� ����� ������ � ������� � ������� ����.
    unsigned char ucQuantityH; // ����������  ������ � ������� � ������� ����.
    unsigned char ucQuantityL; // ����������  ������ � ������� � ������� ����.
    // ��� ���� ���������� (D0 � D3 �. 6.2.1)
    // D5 � �� (�/�), D6 � �� (�/�) - ������ ��� ���-07
    unsigned char ucTust;
    unsigned char ucIOQuantity; // ���������� ������ ��� ������� ��������������� �����.
    // ��������� ����� � ������� ������� Di_value (�1, 2, 3, 4, 15). ���������� ������� ������������ MBM_IO
    uint16_t ui16DiValueArrayBegin;
    // ����� ����������� ������� � ������� �������:
    // - 1-384 ������ �� ������� DI_value (0-383);
    // - 1001 � 1240 ������ �� ������� Log_Sost (0-239); // 2120-2359 �����. ������ CoDeSyst �� ���.����(240).
    // - 0 � ��� �� ���������������� (� �.15 ������ ���������� 0).
    unsigned char aucBitSourceAddress[MBM_FUNCTION_15_SOURCE_BIT_ADDRESS_ARRAY_LENGTH];
};


#endif // MBMFUNCTION15_H_INCLUDED

#ifndef DEVICECONTROL_H_INCLUDED
#define DEVICECONTROL_H_INCLUDED

#include "PasNewConfig.h"


//============================================================================
// ������� ��� ��������� ������� ������������ ����� ��������.
// ��������� ������� - SP, OUT.
#define DEVICE_CONTROL_MTVI5_INCREMENT_SP_OUT  0x01
// ��������� ������� - SP, OUT.
#define DEVICE_CONTROL_MTVI5_DECREMENT_SP_OUT  0x02
// �������� ����-������� � ������ ��������� ����������� �CONT_ST�.
#define DEVICE_CONTROL_MTVI5_CONT_ST_WRITE  0x04
// �������� ��������� � ������ ���������� ���������� ����������� �CONT_AV�.
#define DEVICE_CONTROL_MTVI5_CONT_AV_WRITE  0x05
#define DEVICE_CONTROL_MTVI5_DATA_BASE_WRITE_BY_PROGRAMMER  0x06
#define DEVICE_CONTROL_MTVI5_DATA_BASE_WRITE_BY_HMI  0x07
#define DEVICE_CONTROL_MTVI5_LOAD_OUTS_OSF  0x08
#define DEVICE_CONTROL_MTVI5_CONT_ST_CHANGE  0x09

// ������-������� modbus ������� 5.
// ���������-���������� ������ ����������.
#define DEVICE_CONTROL_CALIBRATION_ON_OFF 0x1000
// ���������� ������ �����.
#define DEVICE_CONTROL_SET_BOTTOM_OF_SCALE  0x1100
// ���������� ����� �����.
#define DEVICE_CONTROL_SET_TOP_OF_SCALE  0x1200
// ���������-���������� ������ ����������.
#define DEVICE_CONTROL_BLOCK 0x007D
// ������������ � �������� ������.
#define DEVICE_CONTROL_PC_KVIT  0x007E
// ����� � �������� ������.
#define DEVICE_CONTROL_PC_RESET 0x007F

#endif // DEVICECONTROL_H_INCLUDED

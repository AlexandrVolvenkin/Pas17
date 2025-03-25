#ifndef PLCERRORTEST_H_INCLUDED
#define PLCERRORTEST_H_INCLUDED

#include "PasNewConfig.h"

// BAD_MODULES_BIT_ARRAY_OFFSET(*������� ������ ������������� ������� ��� ������������ �� HMI
// ������ ModBus. 1016-1055:
// ���� 0 - 15 - ������ 16 ������� ���, ������ 1016-1031,
// ���� 17-19 - ������, ������ 1033-1035
// ��������� -  ������*)
// ��� 16 - ����� �AL - ����� 1032,
#define MAL_ERROR_OFFSET 16
// ���� 17-19 - ������, ������ 1033-1035
#define RESERVE_1_ERROR_OFFSET 17
// ���� 17-19 - ������, ������ 1033-1035
#define RESERVE_2_ERROR_OFFSET 18
// ���� 17-19 - ������, ������ 1033-1035
#define RESERVE_3_ERROR_OFFSET 19
// ��� 20 - ������ ���� ������ ���������� ��������, ����� 1036,
#define DISCRETE_SYGNALS_DATA_BASE_ERROR_OFFSET 20
// ��� 21 - ������ ���� ������ ������ ����������� ����� ���, ��� 1037,
#define ANALOGUE_INPUT_MODULE_DATA_BASE_ERROR_OFFSET 21
// ��� 22 - ������ ���� ������ ������ �����������  ������ ��, ��� 1038,
#define DISCRETE_OUTPUT_MODULE_DATA_BASE_ERROR_OFFSET 22
// ��� 23 - ������ ���� ������ ��������� ���� ������� ������ ����, ��� 1039
#define LOCAL_NET_MODULE_DATA_BASE_ERROR_OFFSET 23


#endif // PLCERRORTEST_H_INCLUDED

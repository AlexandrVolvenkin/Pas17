//-----------------------------------------------------------------------------------------------------
//  Sourse      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-----------------------------------------------------------------------------------------------------
#ifndef DATATYPES_H_INCLUDED
#define DATATYPES_H_INCLUDED

#include "Configuration.h"

//-----------------------------------------------------------------------------------------------------

// ��������� ������ ������
struct TOutputData
{
    unsigned char DiscreteOutputModuleDataBase[DISCRETE_OUTPUT_NUMBER];		// ��������� ��-54
    unsigned char ExternalReceiptAddress[DISCRETE_OUTPUT_NUMBER];		// ������ ������ �������� ������������
    unsigned char ExternalResetAddress[DISCRETE_OUTPUT_NUMBER];		// ������ ������ �������� ������
};

struct TDiscreteOutputControl
{
    // ����� ���������.
    uint8_t uiNewActivation;
    // ���������� ��������� ����.
    uint8_t uiRelayActivationRequest;
};

struct TAnalogueSignalsArchiveHourData
{
    float fAin1;       // ���������� ������� �����
    float fAin2;       // ���������� ������� �����
    float fAin3;       // ���������� �������� �����
    float fAin4;       // ���������� ���������� �����
    time_t currentTime; // ���������� ��� �������� �������� �������
};

#endif // DATATYPES_H_INCLUDED

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

// Описатель канала вывода
struct TOutputData
{
    unsigned char DiscreteOutputModuleDataBase[DISCRETE_OUTPUT_NUMBER];		// Настройки МР-54
    unsigned char ExternalReceiptAddress[DISCRETE_OUTPUT_NUMBER];		// Адреса входов внешнего квитирования
    unsigned char ExternalResetAddress[DISCRETE_OUTPUT_NUMBER];		// Адреса входов внешнего сброса
};

struct TDiscreteOutputControl
{
    // Новое нарушение.
    uint8_t uiNewActivation;
    // Требование включения реле.
    uint8_t uiRelayActivationRequest;
};

struct TAnalogueSignalsArchiveHourData
{
    float fAin1;       // Переменная первого входа
    float fAin2;       // Переменная второго входа
    float fAin3;       // Переменная третьего входа
    float fAin4;       // Переменная четвертого входа
    time_t currentTime; // Переменная для хранения текущего времени
};

#endif // DATATYPES_H_INCLUDED

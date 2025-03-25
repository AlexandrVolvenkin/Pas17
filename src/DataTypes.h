//-----------------------------------------------------------------------------------------------------
//  Sourse      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-----------------------------------------------------------------------------------------------------
#ifndef DATATYPES_H_INCLUDED
#define DATATYPES_H_INCLUDED

#include <stdint.h>
//#include "Configuration.h"

// Описатель канала вывода
struct TOutputData
{
    unsigned char DiscreteOutputModuleDataBase[8];		// Настройки МР-54
    unsigned char ExternalReceiptAddress[8];		// Адреса входов внешнего квитирования
    unsigned char ExternalResetAddress[8];		// Адреса входов внешнего сброса
};

struct TDiscreteOutputControl
{
    // Новое нарушение.
    uint8_t uiNewActivation;
    // Требование включения реле.
    uint8_t uiRelayActivationRequest;
};

struct TMemoryAllocationConext
{
    uint8_t uiAddress;
    uint8_t *puiRxBuffer;
    uint8_t *puiTxBuffer;
    uint8_t *puiErrorCode;
    uint8_t *puiDiscreteInputs;
    uint8_t uiUsedDiscreteInputs;
    uint8_t *puiDiscreteOutputState;
    uint8_t uiUsedDiscreteOutputState;
    TDiscreteOutputControl *pxDiscreteOutputControl;
    uint8_t uiUsedDiscreteOutputs;
    TOutputData *pxDiscreteOutputDataBase;
//    uint8_t *puiExternalReceiptAddress;
//    uint8_t *puiExternalResetAddress;
    uint8_t uiUsedDiscreteOutputDataBase;
    uint8_t* puiErrorAlarmDataArray;
    uint8_t uiUsedErrorAlarmDataArray;
};

#endif // DATATYPES_H_INCLUDED

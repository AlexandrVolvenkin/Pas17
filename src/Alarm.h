#ifndef ALARM_H_INCLUDED
#define ALARM_H_INCLUDED
//-------------------------------------------------------------------------------
//  Sourse      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin

//-------------------------------------------------------------------------------

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
//#include <fcntl.h>
#include <string.h>
#include <assert.h>
#include <time.h>

#include "Configuration.h"
#include "DataTypes.h"

class Timer;
class Platform;
class CTask;
class CResources;
class CDataStore;
class CAnalogueSignals;
class CDataContainerInterface;
class CDataContainerDataBase;
class CConfigurationCreate;
class CAlarmDfa;

//-----------------------------------------------------------------------------------------------------
// 2 бита на сигнал.
// 00 Ц норма, 01 Ц квитированна€ сигнализаци€, 10 Ц активна€ сигнализаци€.
// 128-159 (0x80 - 0x9f) массив активности сигнализации групп дискретных сиг-
// налов √р1-√р16, по 2 бита на группу (32);
//#define ALARM_ACTIVITY_DISCRETE_SIGNAL_GROUP_BIT_ARRAY_LENGTH 32
// 160-663(0xa0 - 0x297); массив активности сигнализации дискретных сигналов
// ƒс1-ƒс252, по 2 бита на сигнал (504);
//#define ALARM_ACTIVITY_DISCRETE_SIGNAL_BIT_ARRAY_LENGTH ((MAX_HANDLED_DISCRETE_INPUT + MAX_HANDLED_FUNCTION_BLOCK) * ALARM_TYPE_QUANTITY_OF_SIGNIFICANT_BIT)
//#define ALARM_TYPE_QUANTITY_OF_SIGNIFICANT_BIT 2
#define ALARM_TYPE_PREVENT 6
#define ALARM_TYPE_ALARM 7
#define ALARM_TYPE_PREVENT_OFFSET 0
#define ALARM_TYPE_ALARM_OFFSET 1

typedef enum
{
    COMMAND_SOUND_SIGNAL_TYPE_NOT = 0,
    COMMAND_SOUND_SIGNAL_TYPE_OFF = 1,
    COMMAND_SOUND_SIGNAL_TYPE_WARNING = 2,
    COMMAND_SOUND_SIGNAL_TYPE_ALARM = 3,
    COMMAND_SOUND_SIGNAL_TYPE_ERROR = 4,
    COMMAND_SOUND_SIGNAL_TYPE_NO_SOUND = 0x80,

    SOUND_SIGNAL_TYPE_NOT = 0,
    SOUND_SIGNAL_TYPE_WARNING,
    SOUND_SIGNAL_TYPE_ALARM,
    SOUND_SIGNAL_TYPE_ERROR,
    SOUND_SIGNAL_TYPE_NO_SOUND = 0x80,
};

//-----------------------------------------------------------------------------------------------------
class CAlarmDfa : public CTask
{
public:

    enum DISCRETE_SIGNAL_STATE_CODE
    {
        // дискретный сигнал неактивен
        DISCRETE_SIGNAL_IS_NOT_ACTIVE = 0,
        // дискретный сигнал активен
        DISCRETE_SIGNAL_IS_ACTIVE = 1,
        // дискретный сигнал недостоверен
        DISCRETE_SIGNAL_IS_NAMUR_ERROR = 2,
        // дискретный сигнал недостоверен
        DISCRETE_SIGNAL_IS_INVALID = 3,
    };

    enum
    {
        START = 0,
        ACTIVE_STATE_WAITING,
        RECEIPT_OR_RESET_WAITING,
        RECEIPT_OR_RESET_OR_AUTOUNSET_WAITING,
        RECEIPTED_RESET_OR_NOT_ACTIVE_STATE_WAITING,
        RESETED_NOT_ACTIVE_STATE_WAITING,
        RECEIPTED_RESET_WAITING,
        NOT_ACTIVE_STATE_WAITING,
        NAMUR_STATE_ON,
        NAMUR_INPUT_CORRECT_STATE_WAITING,
    };

    enum
    {
        NOT_NEW_VIOLATION = 0,
        NEW_VIOLATION = 1,
    };

    virtual uint8_t ALARM_TYPE(void)
    {
        return NORMAL;
    };

    virtual uint8_t ACTIVE_LEVEL(void)
    {
        return 0;
    };

    CAlarmDfa();
    virtual ~CAlarmDfa();
    uint8_t Init(void);
    void Allocate(void);
    void DiscreteOutputsSet(uint8_t *puiLinkedDiscreteOutputs, uint8_t uiNewViolation);
    virtual uint8_t DiscreteSignalStateCheck(void);
    virtual uint8_t Fsm(void);

    void SetDiscreteStateIndex(uint8_t uiDiscreteStateIndex)
    {
        m_uiDiscreteStateIndex = uiDiscreteStateIndex;
    };
    uint8_t GetDiscreteStateIndex(void)
    {
        return m_uiDiscreteStateIndex;
    };

    void SetAlarmWindowIndex(uint8_t uiAlarmWindowIndex)
    {
        m_uiAlarmWindowIndex = uiAlarmWindowIndex;
    };
    uint8_t GetAlarmWindowIndex(void)
    {
        return m_uiAlarmWindowIndex;
    };

    uint8_t* GetLinkedDiscreteOutputsPointer(void)
    {
        return m_auiLinkedDiscreteOutputs;
    };

    uint8_t* GetDiscreteInputsState()
    {
        return m_puiDiscreteInputsState;
    };
    void SetDiscreteInputsState(uint8_t* puiDiscreteInputsState)
    {
        m_puiDiscreteInputsState = puiDiscreteInputsState;
    };

    uint8_t* GetDiscreteInputsBadState()
    {
        return m_puiDiscreteInputsBadState;
    };
    void SetDiscreteInputsBadState(uint8_t* puiDiscreteInputsBadState)
    {
        m_puiDiscreteInputsBadState = puiDiscreteInputsBadState;
    };

    bool IsModbusReceipt()
    {
        if (*m_puiModbusReceipt)
        {
            return true;
        }
        else
        {
            return false;
        }
    };

    bool IsModbusReset()
    {
        if (*m_puiModbusReset)
        {
            return true;
        }
        else
        {
            return false;
        }
    };

protected:
private:
    // »ндекс окна извещател€ - светового табло.
    uint8_t m_uiAlarmWindowIndex;
    // ¬ыходы на ћ–.
    uint8_t m_auiLinkedDiscreteOutputs[DISCRETE_OUTPUT_MODULE_MAX_NUMBER];
    // »сточник дискретного сигнала.
    uint8_t m_uiDiscreteStateIndex;

    uint8_t* m_puiDiscreteInputsState;
    uint8_t* m_puiDiscreteInputsBadState;
////    uint8_t* m_puiDiscreteOutputState;
////    float* m_pfAnalogueInputsValue;
////    uint8_t* m_puiAnalogueInputsState;
////    uint8_t* m_puiAnalogueInputsOff;
////    uint8_t* m_puiAnalogueInputsBadState;
////    uint8_t* m_puiReperPointsAdcBuffer;
////    uint8_t* m_puiAnalogueInputDiscreteInputsState;
////    uint8_t* m_puiAnalogueInputDiscreteInputsBadState;
//    TAnalogueInputDescriptionWork* m_pxAnalogueInputDescriptionWork;
//    TDiscreteSignalsDescriptionWork *m_pxDiscreteSignalsDescriptionWork;
    TDiscreteOutputControl* m_pxDiscreteOutputControl;

    uint8_t* m_puiModbusReceipt;
    uint8_t* m_puiModbusReset;

    uint8_t* m_puiIntermediateBuff;
};
//-----------------------------------------------------------------------------------------------------








//-----------------------------------------------------------------------------------------------------
class CNormalAlarmDfa : public CAlarmDfa
{
public:

    uint8_t ALARM_TYPE(void)
    {
        return NORMAL;
    };

    uint8_t ACTIVE_LEVEL(void)
    {
        return 0;
    };

    CNormalAlarmDfa();
    virtual ~CNormalAlarmDfa();

protected:
private:
};
//-----------------------------------------------------------------------------------------------------








//-----------------------------------------------------------------------------------------------------
class CPreventiveAlarmLowLevelDfa : public CAlarmDfa
{
public:

    uint8_t ALARM_TYPE(void)
    {
        return PREVENTIVE;
    };

    uint8_t ACTIVE_LEVEL(void)
    {
        return 0;
    };

    CPreventiveAlarmLowLevelDfa();
    virtual ~CPreventiveAlarmLowLevelDfa();


protected:
private:
};
//-----------------------------------------------------------------------------------------------------








//-----------------------------------------------------------------------------------------------------
class CPreventiveAlarmHighLevelDfa : public CAlarmDfa
{
public:

    uint8_t ALARM_TYPE(void)
    {
        return PREVENTIVE;
    };

    uint8_t ACTIVE_LEVEL(void)
    {
        return 1;
    };

    CPreventiveAlarmHighLevelDfa();
    virtual ~CPreventiveAlarmHighLevelDfa();

protected:
private:
};
//-----------------------------------------------------------------------------------------------------








//-----------------------------------------------------------------------------------------------------
class CEmergencyAlarmLowLevelDfa : public CAlarmDfa
{
public:

    uint8_t ALARM_TYPE(void)
    {
        return EMERGENCY;
    };

    uint8_t ACTIVE_LEVEL(void)
    {
        return 0;
    };

    CEmergencyAlarmLowLevelDfa();
    virtual ~CEmergencyAlarmLowLevelDfa();

protected:
private:
};
//-----------------------------------------------------------------------------------------------------








//-----------------------------------------------------------------------------------------------------
class CEmergencyAlarmHighLevelDfa : public CAlarmDfa
{
public:

    uint8_t ALARM_TYPE(void)
    {
        return EMERGENCY;
    };

    uint8_t ACTIVE_LEVEL(void)
    {
        return 1;
    };

    CEmergencyAlarmHighLevelDfa();
    virtual ~CEmergencyAlarmHighLevelDfa();

protected:
private:
};
//-----------------------------------------------------------------------------------------------------







//-----------------------------------------------------------------------------------------------------
class CIndicationAlarmLowLevelDfa : public CAlarmDfa
{
public:

    uint8_t ALARM_TYPE(void)
    {
        return INDICATION;
    };

    uint8_t ACTIVE_LEVEL(void)
    {
        return 0;
    };

    CIndicationAlarmLowLevelDfa();
    virtual ~CIndicationAlarmLowLevelDfa();
    virtual uint8_t Fsm(void);

protected:
private:
};
//-----------------------------------------------------------------------------------------------------








//-----------------------------------------------------------------------------------------------------
class CIndicationAlarmHighLevelDfa : public CIndicationAlarmLowLevelDfa
{
public:

    uint8_t ALARM_TYPE(void)
    {
        return INDICATION;
    };

    uint8_t ACTIVE_LEVEL(void)
    {
        return 1;
    };

    CIndicationAlarmHighLevelDfa();
    virtual ~CIndicationAlarmHighLevelDfa();

protected:
private:
};

//-----------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------


#endif // ALARM_H_INCLUDED

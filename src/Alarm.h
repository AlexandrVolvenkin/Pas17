#ifndef ALARM_H_INCLUDED
#define ALARM_H_INCLUDED
//-------------------------------------------------------------------------------
//  Sourse      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin

//-------------------------------------------------------------------------------

class Timer;
class Platform;
class CTask;
class CResources;
class CDataStore;
class CAnalogueSignals;
class CDataContainerInterface;
class CDataContainerDataBase;
class CConfigurationCreate;

//-----------------------------------------------------------------------------------------------------
// 2 ���� �� ������.
// 00 � �����, 01 � ������������� ������������, 10 � �������� ������������.
// 128-159 (0x80 - 0x9f) ������ ���������� ������������ ����� ���������� ���-
// ����� ��1-��16, �� 2 ���� �� ������ (32);
//#define ALARM_ACTIVITY_DISCRETE_SIGNAL_GROUP_BIT_ARRAY_LENGTH 32
// 160-663(0xa0 - 0x297); ������ ���������� ������������ ���������� ��������
// ��1-��252, �� 2 ���� �� ������ (504);
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

#include <stdint.h>

#include "Configuration.h"
//#include "AlarmWindow.h"

//-----------------------------------------------------------------------------------------------------
class CAlarmDfa : public CTask
{
public:

    enum DISCRETE_SIGNAL_STATE_CODE
    {
        // ���������� ������ ���������
        DISCRETE_SIGNAL_IS_NOT_ACTIVE = 0,
        // ���������� ������ �������
        DISCRETE_SIGNAL_IS_ACTIVE = 1,
        // ���������� ������ ������������
        DISCRETE_SIGNAL_IS_NAMUR_ERROR = 2,
        // ���������� ������ ������������
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

    virtual uint8_t IS_NAMUR_ON(void)
    {
        return 0;
    };

    CAlarmDfa();
    virtual ~CAlarmDfa();
    virtual uint8_t DiscreteSignalStateCheck(void);
    virtual void Fsm(void);

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

protected:
private:
    // ������ ���� ���������� - ��������� �����.
    uint8_t m_uiAlarmWindowIndex;
    // ������ �� ��.
    uint8_t m_auiLinkedDiscreteOutputs[DISCRETE_OUTPUT_MODULE_MAX_NUMBER];
    // �������� ����������� �������.
    uint8_t m_uiDiscreteStateIndex;
//    // �������� ������ ������������� ����������� �������.
//    uint8_t m_uiDiscreteInputsBadStateIndex;
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

    uint8_t IS_NAMUR_ON(void)
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

    uint8_t IS_NAMUR_ON(void)
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

    uint8_t IS_NAMUR_ON(void)
    {
        return 0;
    };

    CPreventiveAlarmHighLevelDfa();
    virtual ~CPreventiveAlarmHighLevelDfa();

protected:
private:
};
//-----------------------------------------------------------------------------------------------------








//-----------------------------------------------------------------------------------------------------
class CPreventiveAlarmLowLevelNamurDfa : public CAlarmDfa
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

    uint8_t IS_NAMUR_ON(void)
    {
        return 1;
    };

    CPreventiveAlarmLowLevelNamurDfa();
    virtual ~CPreventiveAlarmLowLevelNamurDfa();


protected:
private:
};
//-----------------------------------------------------------------------------------------------------








//-----------------------------------------------------------------------------------------------------
class CPreventiveAlarmHighLevelNamurDfa : public CAlarmDfa
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

    uint8_t IS_NAMUR_ON(void)
    {
        return 1;
    };

    CPreventiveAlarmHighLevelNamurDfa();
    virtual ~CPreventiveAlarmHighLevelNamurDfa();

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

    uint8_t IS_NAMUR_ON(void)
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

    uint8_t IS_NAMUR_ON(void)
    {
        return 0;
    };

    CEmergencyAlarmHighLevelDfa();
    virtual ~CEmergencyAlarmHighLevelDfa();

protected:
private:
};
//-----------------------------------------------------------------------------------------------------








//-----------------------------------------------------------------------------------------------------
class CEmergencyAlarmLowLevelNamurDfa : public CAlarmDfa
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

    uint8_t IS_NAMUR_ON(void)
    {
        return 1;
    };

    CEmergencyAlarmLowLevelNamurDfa();
    virtual ~CEmergencyAlarmLowLevelNamurDfa();

protected:
private:
};
//-----------------------------------------------------------------------------------------------------








//-----------------------------------------------------------------------------------------------------
class CEmergencyAlarmHighLevelNamurDfa : public CAlarmDfa
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

    uint8_t IS_NAMUR_ON(void)
    {
        return 1;
    };

    CEmergencyAlarmHighLevelNamurDfa();
    virtual ~CEmergencyAlarmHighLevelNamurDfa();

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

    uint8_t IS_NAMUR_ON(void)
    {
        return 0;
    };

    CIndicationAlarmLowLevelDfa();
    virtual ~CIndicationAlarmLowLevelDfa();
    virtual void Fsm(void);

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

    uint8_t IS_NAMUR_ON(void)
    {
        return 0;
    };

    CIndicationAlarmHighLevelDfa();
    virtual ~CIndicationAlarmHighLevelDfa();

protected:
private:
};

//-----------------------------------------------------------------------------------------------------








//-----------------------------------------------------------------------------------------------------
class CIndicationAlarmLowLevelNamurDfa : public CIndicationAlarmLowLevelDfa
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

    uint8_t IS_NAMUR_ON(void)
    {
        return 1;
    };

    CIndicationAlarmLowLevelNamurDfa();
    virtual ~CIndicationAlarmLowLevelNamurDfa();

protected:
private:
};
//-----------------------------------------------------------------------------------------------------








//-----------------------------------------------------------------------------------------------------
class CIndicationAlarmHighLevelNamurDfa : public CIndicationAlarmLowLevelDfa
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

    uint8_t IS_NAMUR_ON(void)
    {
        return 1;
    };

    CIndicationAlarmHighLevelNamurDfa();
    virtual ~CIndicationAlarmHighLevelNamurDfa();

protected:
private:
};

//-----------------------------------------------------------------------------------------------------


#endif // ALARM_H_INCLUDED

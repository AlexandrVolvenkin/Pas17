

#include "PasNewConfig.h"
#include "FunctionBlocks.h"
#include "Alarm.h"
#include "Events.h"
#include "EventsDB.h"

using namespace std;

//-----------------------------------------------------------------------------------------------------
CInternalModuleErrorEvent xCInternalModuleErrorEvent;
//CExternalModuleErrorEvent xCExternalModuleErrorEvent;
//CPlcConfigurationErrorEvent xCPlcConfigurationErrorEvent;
//CPlcDataBaseErrorEvent xCPlcDataBaseErrorEvent;
CAlarmEvent xCAlarmEvent;
CPlcErrorEvent xCPlcErrorEvent;
CPlcArchiveEvent xCPlcArchiveEvent;
CInfoEvent xCInfoEvent;

//-----------------------------------------------------------------------------------------------------
CEvents::TEventsLogQueryListPackOne CEvents::xEventsLogQueryList;
int16_t CEvents::i16EventsLogQueryListQuantity;
CEvents::TOccuredEventsControl CEvents::xOccuredEventsControl;

//-----------------------------------------------------------------------------------------------------
// ������� ������� ����, ��� ������� ��� ����.
void CEvents::EventReset(int16_t ui16EventIndex)
{
    // ������� �� �������� ������� �� �������. ������� ���� ���������(�� ����� �������).
    pui8EventsControlState[ui16EventIndex] = 0;
}

//-----------------------------------------------------------------------------------------------------
// ���������� ��������� �� ������ �������..
CEvents::TEventDataCommon*
CEvents::EventGetBuffer(void)
{
    return axEventDataCommon;
}

//-----------------------------------------------------------------------------------------------------
// ���������, ������� ��� �������������?
// ���� ���, �� ���������� ��������� �� ��������� ����� � ������� �������.
CEvents::TEventDataCommon*
CEvents::EventDataPushRequest(int16_t ui16EventIndex)
{
    uint8_t *pui8EventState;
    pui8EventState = &pui8EventsControlState[ui16EventIndex];
    // ������� �����?
    // ��� ������� ����������� ��� ������ ���������?
    // (���� ���������� ���� - CEvents::EVENT_FIXED_ONCE, �� ������� �����������
    // ������ ���� ��� ����� ���� ��������).
    if ((!(*pui8EventState)) ||
            !((ui8EventTypeSetting) &
              (1 << CEvents::EVENT_FIXED_ONCE)))
    {
        // ������� �� �������� ������� �������. ��������� ���� ���������(�� ����� �������).
        *pui8EventState = 1;
        // ��������, ������ ������� ����� �� ������� ������� - ������ �������.
        // ���� ��, �� ��������� ������ ������� �� ������ ������� - ������ �������.
        // ��� ������ ����� ���������, ���������� ����������� �������(ui16RecordedEventsQuantity),
        // ������ ���� ����� ������� ������.
        // �� ����� �� ������� �������? ���� �����, ������� ������.
        ui16EventDataCommonPushIndex &=
            ui16RecordedEventsQuantity - 1;
        TEventDataCommon *pxEventData;
        pxEventData = &(pxEventDataCommon[ui16EventDataCommonPushIndex]);
        // ��������� ������ �� ��������� �������.
        ui16EventDataCommonPushIndex++;
        return pxEventData;
    }
    else
    {
        return 0;
    }
}

//-----------------------------------------------------------------------------------------------------
// ���������, �������� ��������� ������� ��� �� ����������������?
uint8_t CEvents::EventOnIsNotRegistered(
    uint16_t ui16GroupIndex,
    uint8_t ui8EventOffset)
{
    uint8_t *pui8EventState;
    pui8EventState = &pui8EventsControlState[(ui16GroupIndex * HANDLED_EVENTS_QUANTITY) + ui8EventOffset];
    // ������� ���������?
    if (!(*pui8EventState))
    {
        // ������� �� �������� ������� ���������. ������� ��������� �� - �������.
        *pui8EventState = 1;
        return 1;
    }
    else
    {
        return 0;
    }
}

#include <typeinfo>
//-----------------------------------------------------------------------------------------------------
// ���������, ���������� ��������� ������� ��� �� ����������������?
uint8_t CEvents::EventOffIsNotRegistered(
    uint16_t ui16GroupIndex,
    uint8_t ui8EventOffset)
{
//    // ������� ��� ������.
//    sprintf((char*)pcFileName,
//            "%s%s",
//            typeid(*this).name(),
//            ".map");
//    printf("EventOffIsNotRegistered name %s\n\r", (typeid(*this).name()));
//    printf("EventOffIsNotRegistered sizeof %d\n\r", (sizeof(aui8EventsControlState)));
//    printf("EventOffIsNotRegistered sizeof %d\n\r", (sizeof(xCInternalModuleErrorEvent.aui8EventsControlState)));
//
//    printf("EventOffIsNotRegistered ui8EventOffset %d\n\r", (ui8EventOffset));
//    printf("EventOffIsNotRegistered HANDLED_EVENTS_QUANTITY %d\n\r", (HANDLED_EVENTS_QUANTITY));
//    printf("EventOffIsNotRegistered HANDLED_EVENTS_QUANTITY %d\n\r", (xCInternalModuleErrorEvent.HANDLED_EVENTS_QUANTITY));
//    printf("EventOffIsNotRegistered ui16GroupIndex %d\n\r", ((ui16GroupIndex * HANDLED_EVENTS_QUANTITY) + ui8EventOffset));
    uint8_t *pui8EventState;
    pui8EventState = &pui8EventsControlState[(ui16GroupIndex * HANDLED_EVENTS_QUANTITY) + ui8EventOffset];
    // ������� �������?
    if (*pui8EventState)
    {
        printf("EventOffIsNotRegistered active %d\n\r", (*pui8EventState));
        // ������� �� �������� ������� �������. ������� ��������� �� - ���������.
        *pui8EventState = 0;
        return 1;
    }
    else
    {
        printf("CEvents EventOffIsNotRegistered no active %d\n\r", (*pui8EventState));
        return 0;
    }
}

//-----------------------------------------------------------------------------------------------------
CEvents::TEventDataCommon*
CEvents::EventDataPush(void)
{
    // ��������, ������ ������� ����� �� ������� ������� - ������ �������.
    // ���� ��, �� ��������� ������ ������� �� ������ ������� - ������ �������.
    // ��� ������ ����� ���������, ���������� ����������� �������(ui16RecordedEventsQuantity),
    // ������ ���� ����� ������� ������.
    // �� ����� �� ������� �������? ���� �����, ������� ������.
    ui16EventDataCommonPushIndex &=
        ui16RecordedEventsQuantity - 1;
    // ������� ��������� �� ������ �������.
    return &(pxEventDataCommon[ui16EventDataCommonPushIndex++]);
}

//-----------------------------------------------------------------------------------------------------
void CEvents::EventDataPointerPush(CEvents::TEventDataCommon *pxEventDataCommon)
{
    // ��������, ������ ������� ����� �� ������� ������� - ������ �������.
    // ���� ��, �� ��������� ������ ������� �� ������ ������� - ������ �������.
    // ��� ������ ����� ���������, ���������� ����������� �������(ui16RecordedEventsQuantity),
    // ������ ���� ����� ������� ������.
    // �� ����� �� ������� �������? ���� �����, ������� ������.
    ui16EventDataCommonPointersPushIndex &= (ui16RecordedEventsQuantity - 1);
    // �������� ��������� �� ������ ���������������� �������, � ������� ���������� �� ������ �������.
    // ����� ����� �������� ������ ������� ������� ����, ������������ �� �������.
    ppxEventDataCommonPointers[ui16EventDataCommonPointersPushIndex++] =
        pxEventDataCommon;
}

//-----------------------------------------------------------------------------------------------------
// ������������� ������ �� ������ �������.
// � �������� (ui16EventDataCommonPushIndex - 1) ���������� ������ �� ������ ���������� �������.
void CEvents::EventDataPopIndexSet(int8_t i8Length)
{
    ui16EventDataCommonPopIndex = ui16EventDataCommonPushIndex;
    // ������� ������ �� ��������� �������.
    ui16EventDataCommonPopIndex -= i8Length;
    // ��������, ������ ������� ����� �� ������� ������� - ������ �������.
    // ���� ��, �� ��������� ������ ������� �� ������ ������� - ������ �������.
    // ��� ������ ����� ���������, ���������� ����������� �������(ui16RecordedEventsQuantity),
    // ������ ���� ����� ������� ������.
    // �� ����� �� ������� �������? ���� �����, ������� ������.
    ui16EventDataCommonPopIndex &= (ui16RecordedEventsQuantity - 1);
}

//-----------------------------------------------------------------------------------------------------
// ������������� ������ �� ������ �������.
// � �������� (ui16EventDataCommonPushIndex - 1) ���������� ������ �� ������ ���������� �������.
void CEvents::EventDataPointerPopIndexSet(int8_t i8Length)
{
    ui16EventDataCommonPointersPopIndex = ui16EventDataCommonPointersPushIndex;
    // ������� ������ �� ��������� �������.
    ui16EventDataCommonPointersPopIndex -= i8Length;
    // ��������, ������ ������� ����� �� ������� ������� - ������ �������.
    // ���� ��, �� ��������� ������ ������� �� ������ ������� - ������ �������.
    // ��� ������ ����� ���������, ���������� ����������� �������(ui16RecordedEventsQuantity),
    // ������ ���� ����� ������� ������.
    // �� ����� �� ������� �������? ���� �����, ������� ������.
    ui16EventDataCommonPointersPopIndex &= (ui16RecordedEventsQuantity - 1);
}

//-----------------------------------------------------------------------------------------------------
// ��������� ��������� �� ������ �������.
CEvents::TEventDataCommon*
CEvents::EventDataPop(void)
{
    // ��������, ������ ������� ����� �� ������� ������� - ������ �������.
    // ���� ��, �� ��������� ������ ������� �� ������ ������� - ������ �������.
    // ��� ������ ����� ���������, ���������� ����������� �������(ui16RecordedEventsQuantity),
    // ������ ���� ����� ������� ������.
    // �� ����� �� ������� �������? ���� �����, ������� ������.
    ui16EventDataCommonPopIndex &= (ui16RecordedEventsQuantity - 1);
    // ������� ��������� �� ������ �������.
    return &(pxEventDataCommon[ui16EventDataCommonPopIndex--]);
}

//-----------------------------------------------------------------------------------------------------
// ��������� ��������� �� ������ �������.
CEvents::TEventDataCommon*
CEvents::EventDataPointerPop(void)
{
    // ��������, ������ ������� ����� �� ������� ������� - ������ �������.
    // ���� ��, �� ��������� ������ ������� �� ������ ������� - ������ �������.
    // ��� ������ ����� ���������, ���������� ����������� �������(ui16RecordedEventsQuantity),
    // ������ ���� ����� ������� ������.
    // �� ����� �� ������� �������? ���� �����, ������� ������.
    ui16EventDataCommonPointersPopIndex &= (ui16RecordedEventsQuantity - 1);
    // ������� ��������� �� ���������, �� ������ �������.
    return (ppxEventDataCommonPointers[ui16EventDataCommonPointersPopIndex--]);
}

//-----------------------------------------------------------------------------------------------------
// ������������� � ������� ������� - ��� �������.
void CEvents::NoEventsFlagSet(void)
{
    // ������� ������� ��������(0xFF) - ��� �������.
    for (int i = 0;
            i < (ui16RecordedEventsQuantity);
            i++)
    {
        pxEventDataCommon[i].ui16ID = 0;
    }
}

//-------------------------------------------------------------------------------------------------
// ���� ��������� ������� � ������� ������ ���������� �� �������.
CEvents::TEventDataCommon* CEvents::RequestedEventSearch(uint8_t ui8EventIndex)
{
    // ����� ��������� ������� � ���������� ���������
    // � ���������� ������ � �������, �������� ����� - (MAIN_CYCLE_TIME).
    // ��� �������� ���� ����� ��������� ���������� - ������� ������������
    // ������� �������� ������ �� ������� ����������� �������.
    // ����� ����� ��������, �� ���� ��� ������������� 4 �������.
    // ����������, ��������� � ���������.
    // � ���������� ������ �� ������� ��������� ��������� �������.
    // ���� ������ ������ ����� - �� ������� ��������� ��������� �������,
    // ���� ������ ������ ����� - �� ������� ��������� ���������� �������,
    // �� ����������� ������ �� ������.
    // ������-����� ���������� ������� � ��������� ������� ��������� �� ����������.
    // ������� ������-����� � ������� �������,
    // � �������� ���������� ����������� � ���������� ����.

    // ����� ������ ���������� �������,
    // � ������� �������� �������.
    for (uint8_t i = 0;
            i < CEventsDB::EVENTS_DB_QUERY_ROW_QUANTITY;
            i++)
    {
        // � ������� �������� ������� ���� ���������?
        if (axEventDataCommon[i].i16Index == ui8EventIndex)
        {
            // � ������� �������� ������� ������� ���������.
            // ������� ��������� �� ��������� �������
            // � ������� �������� �������.
            return &axEventDataCommon[i];
            break;
        }
    }
    // � ������� �������� ������� ��� ����������.
    // ������� ��������� �� ������ ������� ����� � ������� �������� �������.
    // ��� ��������� �� ������ �������� �������. ������������ ��� ������.
    return &axEventDataCommon[CEventsDB::EVENTS_DB_QUERY_ROW_QUANTITY];
}

//-----------------------------------------------------------------------------------------------------
// �������� ���������� ������� ������������ � ����� �����.
void CEvents::OccuredEventsControlReset(void)
{
    xOccuredEventsControl.ui8OccuredEventsNumber = 0;
}

//-----------------------------------------------------------------------------------------------------
// ������������ �������.
void CEvents::EventRegistration(
    uint16_t ui16GroupIndex,
    uint8_t ui8EventType,
    uint8_t ui8EventCode,
    char* pcTextDescriptor)
{
    // �� ��������� ������������ ���������� ������� ������������ � ����� �����?
    if (xOccuredEventsControl.ui8OccuredEventsNumber <
            (ONE_CYCLE_OCCURED_EVENTS_MAXIMUM_NUMBER - CONVERT_NATURAL_NUMBER_TO_INTEGER))
    {
        // ��������� �� ������ ������������� �������.
        TOccuredEventsDataBriefly* pxOccuredEventsDataBriefly;
        // ������� ��������� �� ����� ��� ���������� ������ ������������� �������.
        pxOccuredEventsDataBriefly =
            &(xOccuredEventsControl.
              axOccuredEventsData[xOccuredEventsControl.ui8OccuredEventsNumber &
                                  (ONE_CYCLE_OCCURED_EVENTS_MAXIMUM_NUMBER - CONVERT_NATURAL_NUMBER_TO_INTEGER)]);
        // �������� ������ ������ ���������� �������(�������� ����� ������).
        pxOccuredEventsDataBriefly -> ui16GroupIndex = ui16GroupIndex;
        // �������� ��� ������������� �������.
        pxOccuredEventsDataBriefly -> ui8EventType = ui8EventType;
        // �������� ��������� ������������� �������.
        pxOccuredEventsDataBriefly -> ui8EventCode = ui8EventCode;
        // �������� ��������� ��������� ������������� �������.
        pxOccuredEventsDataBriefly -> pcTextDescriptor = pcTextDescriptor;
        // �������� ������� ������� ������������ � ����� �����.
        xOccuredEventsControl.ui8OccuredEventsNumber += 1;
    }
}

//-----------------------------------------------------------------------------------------------------
// �������������� �������.
void CEvents::EventsHandler(void)
{
    uint8_t ui8EventsNumber;
    TOccuredEventsDataBriefly* pxOccuredEventsDataBriefly;
    TModuleContext *pxModuleContext;

    // ������� ���������� ������� ������������ � ����� �����.
    ui8EventsNumber = xOccuredEventsControl.ui8OccuredEventsNumber;
    // � ������� ����� ��������� �������?
    if (ui8EventsNumber)
    {
//        printf("EventsHandler ui8EventsNumber %d\n\r", (ui8EventsNumber));
        CEvents::TEventDataCommon *pxEventData;

        // ������� ��������� �� ������ ������� ������������� � ����� �����.
        pxOccuredEventsDataBriefly = xOccuredEventsControl.axOccuredEventsData;
        // ���������� ������� ������������ � ����� �����.
        for (int i = 0;
                i < (ui8EventsNumber);
                i++)
        {
            // ������� ��������� �� �������� ������.
            pxModuleContext = &xAllModulesContext.axAllModulesContext[pxOccuredEventsDataBriefly -> ui16GroupIndex];
            // ����� ��� �������?
            switch ((pxOccuredEventsDataBriefly -> ui8EventType) &
                    ~(CEvents::HANDLED_EVENTS_IS_POPUP |
                      HANDLED_EVENTS_IS_SOUND |
                      HANDLED_EVENTS_IS_ARCHIVE |
                      HANDLED_EVENTS_IS_OCCURED_ON_START))
            {
            case HANDLED_EVENTS_ALARM_TYPE:
            case HANDLED_EVENTS_DISCRETE_INPUTS_TYPE:
            case HANDLED_EVENTS_FUNCTION_BLOCKS_TYPE:
                xCAlarmEvent.EventsCompleteInformationCreate(pxOccuredEventsDataBriefly);
                break;

                // ��� ������� ������.
            case HANDLED_EVENTS_INTERNAL_MODULES_BAD_TYPE:
            case HANDLED_EVENTS_EXTERNAL_MODULES_BAD_TYPE:
            case HANDLED_EVENTS_MODULES_EVENTS_TYPE:
            case HANDLED_EVENTS_SYSTEM_EVENTS_TYPE:
            case HANDLED_EVENTS_CONFIGURATION_ERROR_TYPE:
            case HANDLED_EVENTS_SYSTEM_ERROR_TYPE:
                xCInternalModuleErrorEvent.EventsCompleteInformationCreate(pxOccuredEventsDataBriefly);
                break;

            default:
                break;
            }
            // ��������� �������.
            pxOccuredEventsDataBriefly++;
        }

        // ������� ������� ������� ������������ � �����.
        OccuredEventsControlReset();
    }
}

//-----------------------------------------------------------------------------------------------------
// ���������, �������� ��������� ������� ��� �� ����������������?
uint8_t CInternalModuleErrorEvent::EventOnIsNotRegistered(
    uint16_t ui16GroupIndex,
    uint8_t ui8EventOffset)
{
    uint8_t *pui8EventState;
    pui8EventState = &pui8EventsControlState[(ui16GroupIndex * HANDLED_EVENTS_QUANTITY) + ui8EventOffset];
    // ������� ���������?
    if (!(*pui8EventState))
    {
        // ������� �� �������� ������� ���������. ������� ��������� �� - �������.
        *pui8EventState = 1;
        return 1;
    }
    else
    {
        return 0;
    }
}

//-----------------------------------------------------------------------------------------------------
// ���������, ���������� ��������� ������� ��� �� ����������������?
uint8_t CInternalModuleErrorEvent::EventOffIsNotRegistered(
    uint16_t ui16GroupIndex,
    uint8_t ui8EventOffset)
{
    uint8_t *pui8EventState;
    pui8EventState = &pui8EventsControlState[(ui16GroupIndex * HANDLED_EVENTS_QUANTITY) + ui8EventOffset];
    // ������� �������?
    if (*pui8EventState)
    {
        // ������� �� �������� ������� �������. ������� ��������� �� - ���������.
        *pui8EventState = 0;
        return 1;
    }
    else
    {
        return 0;
    }
}

//-----------------------------------------------------------------------------------------------------
// ���������, �������� ��������� ������� ��� �� ����������������?
uint8_t CInternalModuleErrorEvent::EventOnIsNotRegistered(
    uint16_t ui16GroupIndex,
    uint8_t ui8EventOffset,
    uint8_t ui8EventCode)
{
    uint8_t *pui8EventState;
    pui8EventState = &pui8EventsControlState[(ui16GroupIndex * HANDLED_EVENTS_QUANTITY) + ui8EventOffset];
    // ������� ���������?
    if (!(*pui8EventState & ui8EventCode))
    {
//        printf("EventOnIsNotRegistered *pui8EventState %d\n\r", (*pui8EventState));
//        printf("EventOnIsNotRegistered ui8EventCode %d\n\r", (ui8EventCode));
        // ������� �� �������� ������� ���������. ������� ��������� �� - �������.
        *pui8EventState |= ui8EventCode;
        return 1;
    }
    else
    {
        return 0;
    }
}

//-----------------------------------------------------------------------------------------------------
// ���������, ���������� ��������� ������� ��� �� ����������������?
uint8_t CInternalModuleErrorEvent::EventOffIsNotRegistered(
    uint16_t ui16GroupIndex,
    uint8_t ui8EventOffset,
    uint8_t ui8EventCode)
{
    uint8_t *pui8EventState;
    pui8EventState = &pui8EventsControlState[(ui16GroupIndex * HANDLED_EVENTS_QUANTITY) + ui8EventOffset];
    // ������� �������?
    if (*pui8EventState & ui8EventCode)
    {
//        printf("EventOffIsNotRegistered *pui8EventState %d\n\r", (*pui8EventState));
//        printf("EventOffIsNotRegistered ui8EventCode %d\n\r", (ui8EventCode));
        // ������� �� �������� ������� �������. ������� ��������� �� - ���������.
        *pui8EventState &= ~ui8EventCode;
        return 1;
    }
    else
    {
        return 0;
    }
}

//-----------------------------------------------------------------------------------------------------
// ������ ��������� ����� � �������.
void CInternalModuleErrorEvent::EventsCompleteInformationCreate(TOccuredEventsDataBriefly* pxOccuredEventsDataBriefly)
{
    TEventDataCommon *pxEventData;
    uint8_t ui8EventType;

    // ������� ��� �������.
    ui8EventType = ((pxOccuredEventsDataBriefly -> ui8EventType) &
                    ~(CEvents::HANDLED_EVENTS_IS_POPUP |
                      HANDLED_EVENTS_IS_SOUND |
                      HANDLED_EVENTS_IS_ARCHIVE |
                      HANDLED_EVENTS_IS_OCCURED_ON_START));

    // ��� �������������� ������� - ������� ������� ��� ���������?
    if ((ui8EventType == HANDLED_EVENTS_MODULES_EVENTS_TYPE) ||
            (ui8EventType == HANDLED_EVENTS_SYSTEM_EVENTS_TYPE))
    {
        // ������� ��������� �� ����� � ��������� ������ ������� �������.
        pxEventData = xCAlarmEvent.EventDataPush();
    }
    // ������� ��� ����� � ������� �� �������?
    else
    {
        // ������� ��������� �� ����� � ��������� ������ ������� ������.
        pxEventData = EventDataPush();
    }

    // ������� �� ������ � ������� �� �������?
    if (((pxOccuredEventsDataBriefly -> ui8EventType) & HANDLED_EVENTS_IS_POPUP) &&
            ((pxOccuredEventsDataBriefly -> ui8EventType) & HANDLED_EVENTS_IS_SOUND))
    {
        // ��������� ���� �������� ������������.
        fucZvkMal = COMMAND_SOUND_SIGNAL_TYPE_ERROR;
    }
    // ������� ��� ����� � ������� �� �������?
    else if (((pxOccuredEventsDataBriefly -> ui8EventType) & HANDLED_EVENTS_IS_POPUP) &&
             !((pxOccuredEventsDataBriefly -> ui8EventType) & HANDLED_EVENTS_IS_SOUND))
    {
        // ��������� ���� ���� ������������ � ���� ��� �����.
        fucZvkMal = (COMMAND_SOUND_SIGNAL_TYPE_ERROR | COMMAND_SOUND_SIGNAL_TYPE_NO_SOUND);
    }
    else
    {
        // ������� ������� ���� �����. �� ���� ������ �� ����������.
        fucZvkMal = COMMAND_SOUND_SIGNAL_TYPE_NOT;
    }

// �� ���������(pxEventData) �� ����� � �������,
// �������� ������ �������.
// ��������� id �������.
    (pxEventData -> ui16ID) =
        1;

// ��������� ��� �������.
    (pxEventData -> ui8Type) =
        ((pxOccuredEventsDataBriefly -> ui8EventType) &
         ~(CEvents::HANDLED_EVENTS_IS_POPUP |
           HANDLED_EVENTS_IS_SOUND |
           HANDLED_EVENTS_IS_ARCHIVE |
           HANDLED_EVENTS_IS_OCCURED_ON_START));

    if (((pxEventData -> ui8Type) == HANDLED_EVENTS_INTERNAL_MODULES_BAD_TYPE) ||
            ((pxEventData -> ui8Type) == HANDLED_EVENTS_MODULES_EVENTS_TYPE))
    {
        // ��������� ����� ��������� �������.
        (pxEventData -> ui16Address) =
            ((pxOccuredEventsDataBriefly -> ui16GroupIndex) + CONVERT_INTEGER_TO_NATURAL_NUMBER);
    }
    else
    {
        // ��������� ����� ��������� �������.
        (pxEventData -> ui16Address) =
            pxOccuredEventsDataBriefly -> ui16GroupIndex;
    }

// ��������� ��� �������.
    (pxEventData -> ui8State) =
        pxOccuredEventsDataBriefly -> ui8EventCode;
// ����� �������� ������ ��� ������?
    if ((pxOccuredEventsDataBriefly -> ui8EventType) &
            HANDLED_EVENTS_IS_OCCURED_ON_START)
    {
        // ��������� ��� ������� ������� - ��� ������.
        (pxEventData -> xCurrentTime.tm_sec) = 0xFF;
    }
    else
    {
        // ��������� ����� �������.
        (pxEventData -> xCurrentTime) =
            xCurrentTime;
    }
// �������� ������ ������ � ���� �������������� ��������� ������ ������� ������ � ���� ������.
    sprintf((char*)(pxEventData -> acTextDescriptorAdditional),
            "%s",
            " ");

// �������������� ���������?
//    if ((pxEventData -> ui8Type) == HANDLED_EVENTS_SYSTEM_ERROR_TYPE)
    if (((pxEventData -> ui8Type) == HANDLED_EVENTS_SYSTEM_ERROR_TYPE) ||
            ((pxEventData -> ui8Type) == HANDLED_EVENTS_SYSTEM_EVENTS_TYPE))
    {
        // �������� ��������� �������� �������.
        sprintf((char*)(pxEventData -> acTextDescriptor),
                "%s",
                pxOccuredEventsDataBriefly -> pcTextDescriptor);
    }
// �������������� ���������?
    else if ((pxEventData -> ui8Type) == HANDLED_EVENTS_CONFIGURATION_ERROR_TYPE)
    {
        // �������� ��������� �������� �������.
        sprintf((char*)(pxEventData -> acTextDescriptor),
                "%s",
                pxOccuredEventsDataBriefly -> pcTextDescriptor);
        // �������� ������ ������ �������.
        // �������� ������ � ������ ������� ������� �������������.
        sprintf((char*)(pxEventData -> acTextData),
                "  %X%X%X%X%X%X%X%X%X%X%X%X%X%X%X%X",
                (xPlcConfigSearch.axConfigSearch[0].ui8Type),
                (xPlcConfigSearch.axConfigSearch[1].ui8Type),
                (xPlcConfigSearch.axConfigSearch[2].ui8Type),
                (xPlcConfigSearch.axConfigSearch[3].ui8Type),
                (xPlcConfigSearch.axConfigSearch[4].ui8Type),
                (xPlcConfigSearch.axConfigSearch[5].ui8Type),
                (xPlcConfigSearch.axConfigSearch[6].ui8Type),
                (xPlcConfigSearch.axConfigSearch[7].ui8Type),
                (xPlcConfigSearch.axConfigSearch[8].ui8Type),
                (xPlcConfigSearch.axConfigSearch[9].ui8Type),
                (xPlcConfigSearch.axConfigSearch[10].ui8Type),
                (xPlcConfigSearch.axConfigSearch[11].ui8Type),
                (xPlcConfigSearch.axConfigSearch[12].ui8Type),
                (xPlcConfigSearch.axConfigSearch[13].ui8Type),
                (xPlcConfigSearch.axConfigSearch[14].ui8Type),
                (xPlcConfigSearch.axConfigSearch[15].ui8Type));
    }
    else
    {
        // �������� ��������� �������� �������.
        sprintf((char*)(pxEventData -> acTextDescriptor),
                "%s %s%02d %s%02X",
                pxOccuredEventsDataBriefly -> pcTextDescriptor,
                "#",
                (pxEventData -> ui16Address),
                "E",
                pxOccuredEventsDataBriefly -> ui8EventCode);
    }

// �� ��� �������������� ������� - ������� �������?
    if ((ui8EventType != HANDLED_EVENTS_MODULES_EVENTS_TYPE) &&
            (ui8EventType != HANDLED_EVENTS_SYSTEM_EVENTS_TYPE))
    {
        // �������� � ������ ������.
        xCPlcErrorEvent.EventDataPointerPush(pxEventData);
    }

// ������� ������������?
    if ((pxOccuredEventsDataBriefly -> ui8EventType) & HANDLED_EVENTS_IS_ARCHIVE)
    {
        // ���������� �������.
        xCArchiveEventsDB.DataBaseDataPush(pxEventData);
    }

// ������ ��������� ���� ��������������� ����������� ������ ������� �� ���������� ������.
// ����� ���� �������� ������ ������� � �������� - ��� �������, �� �������� ��������
// �������. �� ������� ���������� ���������� � ���, ��� ������� ������ ���.
// ����� �������� ������ ��������� ���� � ���, ��� ������� ������ ��� - �������� ��
// ���������� ������ ����� ������ �������. ������� ��� �������� - ��� �������(ui16ID = 0).
// ��������� ������ (ui16EventDataCommonPopIndex) �� ����� ���������� � ��������� ������ ������ ������� �������.
    EventDataPopIndexSet(0);
// ������� ��������� �� ����� ���������� � ��������� ������ ������ ������� �������.
    pxEventData = EventDataPop();
// ������� ������� �������� - ��� �������.
    pxEventData -> ui16ID = 0;
}




//-----------------------------------------------------------------------------------------------------
// ���������, �������� ��������� ������� ��� �� ����������������?
uint8_t CAlarmEvent::EventOnIsNotRegistered(
    uint16_t ui16GroupIndex,
    uint8_t ui8EventOffset)
{
    uint8_t *pui8EventState;
    pui8EventState = &pui8EventsControlState[(ui16GroupIndex * HANDLED_EVENTS_QUANTITY) + ui8EventOffset];
    // ������� ���������?
    if (!(*pui8EventState))
    {
        // ������� �� �������� ������� ���������. ������� ��������� �� - �������.
        *pui8EventState = 1;
        return 1;
    }
    else
    {
        return 0;
    }
}

//-----------------------------------------------------------------------------------------------------
// ���������, ���������� ��������� ������� ��� �� ����������������?
uint8_t CAlarmEvent::EventOffIsNotRegistered(
    uint16_t ui16GroupIndex,
    uint8_t ui8EventOffset)
{
    uint8_t *pui8EventState;
    pui8EventState = &pui8EventsControlState[(ui16GroupIndex * HANDLED_EVENTS_QUANTITY) + ui8EventOffset];
    // ������� �������?
    if (*pui8EventState)
    {
        // ������� �� �������� ������� �������. ������� ��������� �� - ���������.
        *pui8EventState = 0;
        return 1;
    }
    else
    {
        return 0;
    }
}

//-----------------------------------------------------------------------------------------------------
// ������ ��������� ����� � �������.
void CAlarmEvent::EventsCompleteInformationCreate(TOccuredEventsDataBriefly* pxOccuredEventsDataBriefly)
{
    TEventDataCommon *pxEventData;
    TAlarmHmi *pxAlarmHmi;
    int i;
    uint8_t ui8EventType;
    uint8_t ui8EventCode;

    // ������� ��������� �� ����� � ��������� ������.
    pxEventData = EventDataPush();

    // ������� ��� �������.
    ui8EventType = ((pxOccuredEventsDataBriefly -> ui8EventType) &
                    ~(CEvents::HANDLED_EVENTS_IS_POPUP |
                      HANDLED_EVENTS_IS_SOUND |
                      HANDLED_EVENTS_IS_ARCHIVE |
                      HANDLED_EVENTS_IS_OCCURED_ON_START));
    // ������� ��������� �������.
    ui8EventCode = pxOccuredEventsDataBriefly -> ui8EventCode;

    // ������� ������ �������.
    i = (int)(pxOccuredEventsDataBriefly -> ui16GroupIndex);

    // ������������ ������� ��������������� �����?
    if (ui8EventType == HANDLED_EVENTS_FUNCTION_BLOCKS_TYPE)
    {
        // ��������� �� ����� ������� �������������� ������.
        i += (xPlcConfigService.xPlcConfigServiceData.
              uiHandledDiscreteInputQuantity);
    }

    // ������������ ���������?
    if ((ui8EventCode & (0x01 << ALARM_TYPE_ALARM)) &&
            (!(ui8EventCode & (0x01 << ALARM_TYPE_PREVENT))))
    {
        fucZvkMal = COMMAND_SOUND_SIGNAL_TYPE_ALARM;
        sprintf((char*)(pxEventData -> acTextDescriptorAdditional),
                "%s",
                "���������");
    }
    // ������������ �����������������?
    else if  ((!(ui8EventCode & (0x01 << ALARM_TYPE_ALARM))) &&
              (ui8EventCode & (0x01 << ALARM_TYPE_PREVENT)))
    {
        // ����������������� ������������.
        fucZvkMal = COMMAND_SOUND_SIGNAL_TYPE_WARNING;
        sprintf((char*)(pxEventData -> acTextDescriptorAdditional),
                "%s",
                "�����������������");
    }
    // ������������ � �����?
    else if (((!(ui8EventCode & (0x01 << ALARM_TYPE_ALARM))) &&
              (!(ui8EventCode & (0x01 << ALARM_TYPE_PREVENT)))) ||
             (((ui8EventCode & (0x01 << ALARM_TYPE_ALARM))) &&
              ((ui8EventCode & (0x01 << ALARM_TYPE_PREVENT)))))
    {
        // ������������ � �����.
        fucZvkMal = COMMAND_SOUND_SIGNAL_TYPE_WARNING;
        sprintf((char*)(pxEventData -> acTextDescriptorAdditional),
                "%s",
                "�����");
    }
//    else
//    {
//        fucZvkMal = COMMAND_SOUND_SIGNAL_TYPE_ALARM;
//    // �������� ������ ������ � ���� �������������� ��������� ������ ������� ������ � ���� ������.
//        sprintf((char*)(pxEventData -> acTextDescriptorAdditional),
//                "%s",
//                "���������");
//    }

    // ������� �� ������ � ������� �� �������?
    if (((pxOccuredEventsDataBriefly -> ui8EventType) & HANDLED_EVENTS_IS_POPUP) &&
            ((pxOccuredEventsDataBriefly -> ui8EventType) & HANDLED_EVENTS_IS_SOUND))
    {
        // ��������� ���� �������� ������������.
    }
    // ������� ��� ����� � ������� �� �������?
    else if (((pxOccuredEventsDataBriefly -> ui8EventType) & HANDLED_EVENTS_IS_POPUP) &&
             !((pxOccuredEventsDataBriefly -> ui8EventType) & HANDLED_EVENTS_IS_SOUND))
    {
        // ��������� ���� ��� �����.
        fucZvkMal |= COMMAND_SOUND_SIGNAL_TYPE_NO_SOUND;
    }
    else
    {
        // ������� ������� ���� �����. �� ���� ������ �� ����������.
        fucZvkMal = COMMAND_SOUND_SIGNAL_TYPE_NOT;
    }

    // �� ���������(pxEventData) �� ����� � �������,
    // �������� ������ �������.
    // ��������� id �������.
    (pxEventData -> ui16ID) =
        1;
    // ��������� ��� �������.
    (pxEventData -> ui8Type) =
        ui8EventType;
    // ��������� ����� ��������� �������.
    (pxEventData -> ui16Address) =
        (i + CONVERT_INTEGER_TO_NATURAL_NUMBER);
    // ��������� ��� �������.
    (pxEventData -> ui8State) =
        ui8EventCode;
    // ��������� ����� �������.
    (pxEventData -> xCurrentTime) =
        xCurrentTime;
    // �������� ������� - ���������� ����?
    if (i <
            (xPlcConfigService.xPlcConfigServiceData.
             uiHandledDiscreteInputQuantity))
    {
        // �������� ������� - ���������� ����.
        // ��������� ������ ���������� ���������.
        memcpy(&(pxEventData -> acTextDescriptor),
               (&(axDiscreteSygnalTextDescriptor[i].acTextDescriptor)),
               (CAlarm::EVENT_NAME_LENGTH + END_OF_STRING_LENGTH));
    }
    else
    {
        // �������� ������� - �������������� ����.
        // �������� ������ ���������� ���������.
        sprintf((char*)(pxEventData -> acTextDescriptor),
                "%s%d",
                "��� #",
                ((i - (xPlcConfigService.xPlcConfigServiceData.
                        uiHandledDiscreteInputQuantity)) +
                 CONVERT_INTEGER_TO_NATURAL_NUMBER));
    }

    // ������� ������������?
    if ((pxOccuredEventsDataBriefly -> ui8EventType) & HANDLED_EVENTS_IS_ARCHIVE)
    {
        // ���������� �������.
        xCArchiveEventsDB.DataBaseDataPush(pxEventData);
    }

    // ������ ��������� ���� ��������������� ����������� ������ ������� �� ���������� ������.
    // ����� ���� �������� ������ ������� � �������� - ��� �������, �� �������� ��������
    // �������. �� ������� ���������� ���������� � ���, ��� ������� ������ ���.
    // ����� �������� ������ ��������� ���� � ���, ��� ������� ������ ��� - �������� ��
    // ���������� ������ ����� ������ �������. ������� ��� �������� - ��� �������(ui16ID = 0).
    // ��������� ������ (ui16EventDataCommonPopIndex) �� ����� ���������� � ��������� ������ ������ ������� �������.
    EventDataPopIndexSet(0);
    // ������� ��������� �� ����� ���������� � ��������� ������ ������ ������� �������.
    pxEventData = EventDataPop();
    // ������� ������� �������� - ��� �������.
    pxEventData -> ui16ID = 0;
}



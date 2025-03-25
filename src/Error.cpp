

#include "PasNewConfig.h"
#include "FunctionBlocks.h"
#include "Error.h"
#include "Alarm.h"
#include "Events.h"
#include "EventsDB.h"

using namespace std;

//-----------------------------------------------------------------------------------------------------
uint8_t CError::ui8ErrorLogBriefIsUpdated;
const  CError::TError CError::xErrorNull =
{
    0,
    0,
    0,
    0
};
uint8_t CError::ui8ErrorLogLength;
uint8_t CError::ui8ErrorLogBriefLength;
CError::TErrorLogQueryListPackOne CError::xErrorQueryList;
int16_t CError::i16ErrorQueryListQuantity;
CError::TError CError::axErrorLog[];
CError::TErrorPackOne CError::xErrorLog;
CError::TErrorLogControl CError::xErrorLogControl;

float CError::fBadPacketQuantity;
float CError::fOkPacketQuantity;

//char* apcDiscreteInputGroupEventsStrings[16] =
//{
//    "apricot",
//    "apple",
//    "banana",
//    "lemon",
//    "pear",
//    "plum"
//};
//
//char* apcAnalogueInputGroupEventsStrings[16] =
//{
//    "apricot",
//    "apple",
//    "banana",
//    "lemon",
//    "pear",
//    "plum"
//};
//
//char* apcDiscreteOutputGroupEventsStrings[16] =
//{
//    "���. ����",
//    "���. ����",
//    "���. ����",
//    "���. ����",
//    "���. ����",
//    "���. ����",
//    "���. ����",
//    "���. ����",
//};
//
//char* apcAnalogueOutputGroupEventsStrings[16] =
//{
//    "������ ���",
//    "������ ���",
//    "������ ���",
//    "������ ���",
//    "������ ���",
//    "������ ���",
//};
//
//char** appcAllGroupEventsStrings[] =
//{
//    apcDiscreteInputGroupEventsStrings,
//    apcAnalogueInputGroupEventsStrings,
//    apcDiscreteOutputGroupEventsStrings,
//    apcAnalogueOutputGroupEventsStrings
//};

////-----------------------------------------------------------------------------------------------------
//CError::TErrorTextDescriptor CError::axErrorTextDescriptor[]
//{
////    {
////        "����� ������ #"
////    },
////    {
////        "����� ������ #1"
////    },
////    {
////        "����� ������ #2"
////    },
////    {
////        "����� ������ #3"
////    },
////    {
////        "����� ������ #4"
////    },
////    {
////        "����� ������ #5"
////    },
////    {
////        "����� ������ #6"
////    },
////    {
////        "����� ������ #7"
////    },
////    {
////        "����������������"
////    },
////    {
////        "����� ����. ����."
////    },
////    {
////        "������ ������������"
////    },
////    {
////        "������ �������. ����"
////    },
////    {
////        "������� ���������"
////    },
////    {
////        "��������������������"
////    },
////    {
////        "����� ���. ���������"
////    },
////    {
////        "������ ����. ������."
////    },
////    {
////        "���������� �� Reset"
////    },
////    {
////        "���������� �� WDT"
////    }
//};
//
////-----------------------------------------------------------------------------------------------------
//CError::TErrorTextDescriptor CError::axDataBaseErrorTextDescriptor[]
//{
//    {
//        "��.�����.��������"
//    },
//    {
//        "���"
//    },
//    {
//        "��"
//    },
//    {
//        "���"
//    },
//    {
//        "���������� ���.����"
//    },
//    {
//        "���. ����� ��������"
//    },
//    {
//        "���"
//    }
//};

//-----------------------------------------------------------------------------------------------------
// ������� ����������� ���� ������ Modbus �� ���������� ����.
uint8_t CError::ModbusErrorToPlcError(int32_t i32Errno)
{
    uint8_t ui8Errno;
    // ����� ������ Modbus?
    switch(i32Errno)
    {
    case EMBBADDATA:
    case ETIMEDOUT:
        ui8Errno = EXTERNAL_MODULE_ERROR_TIMEOUT;
        break;
    case EMBBADCRC:
        ui8Errno = EXTERNAL_MODULE_ERROR_CRC;
        break;
    case EMBXILFUN:
        ui8Errno = EXTERNAL_MODULE_ERROR_ILLEGAL_FUNCTION;
        break;
    case EMBXILADD:
        ui8Errno = EXTERNAL_MODULE_ERROR_ILLEGAL_DATA_ADDRESS;
        break;
    case EMBXILVAL:
        ui8Errno = EXTERNAL_MODULE_ERROR_ILLEGAL_DATA_VALUE;
        break;
    case EMBXSFAIL:
        ui8Errno = EXTERNAL_MODULE_ERROR_SLAVE_OR_SERVER_FAILURE;
        break;
    default:
        ui8Errno = 0;
        break;
    };
    return ui8Errno;
}

//-----------------------------------------------------------------------------------------------------
// ������� �������� ������� ������.
// ������ ������ - ������ � �������� � ������ ������.
void CError::ErrorLogCreate(void)
{
    uint8_t ui8IndexNumber;
    uint8_t ui8ErrorIndex;
    TModuleContext *pxModuleContext;
    CEvents::TEventDataCommon *pxEventData;

    CEvents::EventsHandler();

//    // ���������� ������ ������� ���������� �������.
//    for (int i = 0;
//            i <
//            (xPlcConfigService.xPlcConfigServiceData.ucInternalModulesQuantity);
//            i++)
//    {
//        // ������� ��������� �� �������� ������.
//        pxModuleContext = &xAllModulesContext.axAllModulesContext[i];
//        // ���� ����� �������� ������?
////        if (*(pxModuleContext ->
////                xModuleContextStatic.
////                pucModuleBadStateBufferPointer))
//        // ���� ������ �������� ������?
//        if (pxModuleContext ->
//                xModuleContextDinamic.
//                ucErrorCode)
////        if (i < 8)
//        {
//            // ���� ����� �������� ������.
//            // ��������� ���� - ����� ������.
//            aucCoilsArray[BAD_MODULES_BIT_ARRAY_OFFSET + i] = 1;
//
//            CEvents::TEventDataCommon *pxEventData;
//            pxEventData = xCInternalModuleErrorEvent.EventDataPushRequest(i);
//            // ���� ����������� ������������� �������?
//            if (pxEventData)
//            {
//                // ��������� ���� �������� ������������.
//                fucZvkMal = COMMAND_SOUND_SIGNAL_TYPE_ERROR;
////                printf("CError::ErrorLogCreate(void) ErrorLogCreate: i %d\n\r", (i));
//                xCPlcErrorEvent.EventDataPointerPush(pxEventData);
////                xCPlcArchiveEvent.EventDataPointerPush(pxEventData);
//                // ���� ���� ����������� ������������� �������,
//                // �� �� ���������(pxEventData) �� ����� � �������,
//                // �������� ������ �������.
//                // ��������� id �������.
//                (pxEventData -> ui16ID) =
//                    1;
//                // ��������� ��� �������.
//                (pxEventData -> ui8Type) =
//                    CEvents::HANDLED_EVENTS_INTERNAL_MODULES_BAD_TYPE;
//                // ��������� ����� ��������� �������.
//                (pxEventData -> ui16Address) =
//                    (pxModuleContext ->
//                     xModuleContextStatic.
//                     ucModuleAddress);
////                printf("CError::ucModuleAddress %d\n\r", (pxModuleContext ->
////                     xModuleContextStatic.
////                     ucModuleAddress));
//                // ��������� ��� �������.
//                (pxEventData -> ui8State) =
//                    (pxModuleContext ->
//                     xModuleContextDinamic.
//                     ucErrorCode);
//                // ����� �������� ������ ��� ������?
//                if (*(pxModuleContext ->
//                        xModuleContextStatic.
//                        pucModuleBadStateBufferPointer) ==
//                        BAD_MODULE_NOT_RESPONDED_ON_START)
//                {
//                    // ��������� ��� ������� ������� - ��� ������.
//                    (pxEventData -> xCurrentTime.tm_sec) = 0xFF;
//                }
//                else
//                {
//                    // ��������� ����� �������.
//                    (pxEventData -> xCurrentTime) =
//                        xCurrentTime;
//                }
//                // �������� ������ ���������� ��������� �������.
//                //                sprintf((char*)(pxEventData -> acTextDescriptor),
////                        "%s%d",
////                        "����� ������ #",
////                        ((pxModuleContext ->
////                          xModuleContextStatic.
////                          ucModuleAddress) +
////                         CONVERT_INTEGER_TO_NATURAL_NUMBER));
//                sprintf((char*)(pxEventData -> acTextDescriptor),
//                        "%s%02d %s%02X",
//                        "����� ������ #",
//                        ((pxModuleContext ->
//                          xModuleContextStatic.
//                          ucModuleAddress) +
//                         CONVERT_INTEGER_TO_NATURAL_NUMBER),
//                        "E",
//                        (pxModuleContext ->
//                         xModuleContextDinamic.
//                         ucErrorCode));
//                xCArchiveEventsDB.DataBaseDataPush(pxEventData);
//            }
//        }
//        else
//        {
//            // ������� �������� ����, ��� ������� ���� ������������� �����.
//            xCInternalModuleErrorEvent.EventReset(i);
//        }
//    }

////-----------------------------------------------------------------------------------------------------
//    // ���������� ������ ������� ������� �������.
//    for (int i = 0;
//            i < (xPlcConfigService.xPlcConfigServiceData.ucExternalModulesQuantity);
//            i++)
//    {
//        // ������� ��������� �� �������� ������.
//        pxModuleContext = &xAllModulesContext.axAllModulesContext[i +
//                          (xPlcConfigService.xPlcConfigServiceData.ucInternalModulesQuantity)];
//        // ���� ����� �������� ������?
//        if (*(pxModuleContext ->
//                xModuleContextStatic.
//                pucModuleBadStateBufferPointer))
//        {
//            pxEventData = xCExternalModuleErrorEvent.EventDataPushRequest(i);
//            // ���� ����������� ������������� �������?
//            if (pxEventData)
//            {
////                cout << "ErrorLogCreate xCExternalModuleErrorEvent" << endl;
////                // ��������� ���� �������� ������������.
////                fucZvkMal = COMMAND_SOUND_SIGNAL_TYPE_ERROR;
//                xCPlcErrorEvent.EventDataPointerPush(pxEventData);
////                xCPlcArchiveEvent.EventDataPointerPush(pxEventData);
//                // ���� ���� ����������� ������������� �������,
//                // �� �� ���������(pxEventData) �� ����� � �������,
//                // �������� ������ �������.
//                // ��������� id �������.
//                (pxEventData -> ui16ID) =
//                    1;
//                // ��������� ��� �������.
//                (pxEventData -> ui8Type) =
//                    CEvents::HANDLED_EVENTS_EXTERNAL_MODULES_BAD_TYPE;
//                // ��������� ����� ��������� �������.
//                (pxEventData -> ui16Address) =
//                    (pxModuleContext ->
//                     xModuleContextStatic.
//                     ucModuleAddress);
//                // ��������� ��� �������.
//                (pxEventData -> ui8State) =
//                    (pxModuleContext ->
//                     xModuleContextDinamic.
//                     ucErrorCode);
//                // ��������� ����� �������.
//                (pxEventData -> xCurrentTime) =
//                    xCurrentTime;
//                // �������� ������ ���������� ��������� �������.
//                sprintf((char*)(pxEventData -> acTextDescriptor),
//                        "%s%02d %s%02X",
//                        "������ SL#",
//                        (pxModuleContext ->
//                         xModuleContextStatic.
//                         ucModuleAddress),
//                        "E",
//                        (pxModuleContext ->
//                         xModuleContextDinamic.
//                         ucErrorCode));
////                sprintf((char*)(pxEventData -> acTextDescriptor),
////                        "%s%02X",
////                        "������ SL HEX-",
////                        (pxModuleContext ->
////                         xModuleContextDinamic.
////                         ucErrorCode));
//                xCArchiveEventsDB.DataBaseDataPush(pxEventData);
//            }
//        }
//        else
//        {
//            // ������� �������� ����, ��� ������� ���� ������������� �����.
//            xCExternalModuleErrorEvent.EventReset(i);
//        }
//    }

////-----------------------------------------------------------------------------------------------------
//// ���������� ������ ������������.
//// ���� ������ ������������?
//    if (fbPlcConfigurationError)//if (1)//
//    {
//        // ���� ������ ������������.
//        pxEventData = xCPlcConfigurationErrorEvent.EventDataPushRequest(0);
//        // ���� ����������� ������������� �������?
//        if (pxEventData)
//        {
//            // ��������� ���� �������� ������������.
//            fucZvkMal = COMMAND_SOUND_SIGNAL_TYPE_ERROR;
//            xCPlcErrorEvent.EventDataPointerPush(pxEventData);
//            // ���� ���� ����������� ������������� �������,
//            // �� �� ���������(pxEventData) �� ����� � �������,
//            // �������� ������ �������.
//            // ��������� id �������.
//            (pxEventData -> ui16ID) =
//                1;
//            // ��������� ��� �������.
//            (pxEventData -> ui8Type) =
//                CEvents::HANDLED_EVENTS_CONFIGURATION_ERROR_TYPE;
//            // �������� ������ ������ �������.
//            // �������� ������ � ������ ������� ������� �������������.
//            sprintf((char*)(pxEventData -> acTextData),
//                    "  %X%X%X%X%X%X%X%X%X%X%X%X%X%X%X%X",
//                    (xPlcConfigSearch.axConfigSearch[0].ui8Type),
//                    (xPlcConfigSearch.axConfigSearch[1].ui8Type),
//                    (xPlcConfigSearch.axConfigSearch[2].ui8Type),
//                    (xPlcConfigSearch.axConfigSearch[3].ui8Type),
//                    (xPlcConfigSearch.axConfigSearch[4].ui8Type),
//                    (xPlcConfigSearch.axConfigSearch[5].ui8Type),
//                    (xPlcConfigSearch.axConfigSearch[6].ui8Type),
//                    (xPlcConfigSearch.axConfigSearch[7].ui8Type),
//                    (xPlcConfigSearch.axConfigSearch[8].ui8Type),
//                    (xPlcConfigSearch.axConfigSearch[9].ui8Type),
//                    (xPlcConfigSearch.axConfigSearch[10].ui8Type),
//                    (xPlcConfigSearch.axConfigSearch[11].ui8Type),
//                    (xPlcConfigSearch.axConfigSearch[12].ui8Type),
//                    (xPlcConfigSearch.axConfigSearch[13].ui8Type),
//                    (xPlcConfigSearch.axConfigSearch[14].ui8Type),
//                    (xPlcConfigSearch.axConfigSearch[15].ui8Type));
//
//            switch(xPlcConfigDataTransmit.xConfigData.ucErrorCode)
//            {
//            case CONFIGURATION_ERROR_SEQUENCE_ERROR:
//                // �������� ������ ���������� ��������� �������.
//                sprintf((char*)(pxEventData -> acTextDescriptor),
//                        "%s",
//                        "������ ������. �����");
//                break;
//            case CONFIGURATION_ERROR_EMPTY_SLOT:
//                // �������� ������ ���������� ��������� �������.
//                sprintf((char*)(pxEventData -> acTextDescriptor),
//                        "%s",
//                        "������ ������ ����");
//                break;
//
//            case CONFIGURATION_ERROR_NEW_CONFIGURATION_AND_DATA_BASE:
//                // �������� ������ ���������� ��������� �������.
//                sprintf((char*)(pxEventData -> acTextDescriptor),
//                        "%s",
//                        "����� ����. � ����.�");
//                break;
//            case CONFIGURATION_ERROR_NEW_CONFIGURATION:
//                // �������� ������ ���������� ��������� �������.
//                sprintf((char*)(pxEventData -> acTextDescriptor),
//                        "%s",
//                        "����� ������������");
//                break;
//            case CONFIGURATION_ERROR_BAD_MODULE_FINDED_AND_REGISTERED:
//                // �������� ������ ���������� ��������� �������.
//                sprintf((char*)(pxEventData -> acTextDescriptor),
//                        "%s",
//                        "����� ���. ���������");
//                break;
//
//            default:
//                // �������� ������ ���������� ��������� �������.
//                sprintf((char*)(pxEventData -> acTextDescriptor),
//                        "%s",
//                        "������ ������������");
//                break;
//            };
//        }
//    }
//    else
//    {
//        // ������� �������� ����, ��� ������� ���� ������������� �����.
//        xCPlcConfigurationErrorEvent.EventReset(0);
//    }
//
//    ui8ErrorIndex = 0;

////-----------------------------------------------------------------------------------------------------
//// ���������� ������ ���� ������ ���������� ��������.
//// ���� ������ ���� ������ ���������� ��������?
//    if (fbDiscreteInputDataBaseError)
//    {
//        // ���� ������ ���� ������ ���������� ��������.
//        pxEventData = xCPlcDataBaseErrorEvent.EventDataPushRequest(ui8ErrorIndex);
//        // ���� ����������� ������������� �������?
//        if (pxEventData)
//        {
//            // ��������� ���� �������� ������������.
//            fucZvkMal = COMMAND_SOUND_SIGNAL_TYPE_ERROR;
//            xCPlcErrorEvent.EventDataPointerPush(pxEventData);
////                xCPlcArchiveEvent.EventDataPointerPush(pxEventData);
//            // ���� ���� ����������� ������������� �������,
//            // �� �� ���������(pxEventData) �� ����� � �������,
//            // �������� ������ �������.
//            // ��������� id �������.
//            (pxEventData -> ui16ID) =
//                1;
//            // ��������� ��� �������.
//            (pxEventData -> ui8Type) =
//                CEvents::HANDLED_EVENTS_SYSTEM_ERROR_TYPE;
//            // ��������� ����� ��������� �������.
//            (pxEventData -> ui16Address) = 0;
//            // ��������� ��� �������.
//            (pxEventData -> ui8State) = 1;
//            // ��������� ����� �������.
//            (pxEventData -> xCurrentTime) =
//                xCurrentTime;
//            // �������� ������ ���������� ��������� �������.
//            strcpy((char*)(pxEventData -> acTextDescriptor),
//                   "��.����.��������");
//            xCArchiveEventsDB.DataBaseDataPush(pxEventData);
//        }
//    }
//    else
//    {
//        // ������� �������� ����, ��� ������� ���� ������������� �����.
//        xCPlcDataBaseErrorEvent.EventReset(ui8ErrorIndex);
//    }
//    ui8ErrorIndex++;

////-----------------------------------------------------------------------------------------------------
//// ���������� ������ ���� ������ �������������� ������.
//// ���� ������ ���� ������ �������������� ������?
//    if (fbFunctionBlockDataBaseError)
//    {
//        // ���� ������ ���� ������ �������������� ������.
//        pxEventData = xCPlcDataBaseErrorEvent.EventDataPushRequest(ui8ErrorIndex);
//        // ���� ����������� ������������� �������?
//        if (pxEventData)
//        {
//            // ��������� ���� �������� ������������.
//            fucZvkMal = COMMAND_SOUND_SIGNAL_TYPE_ERROR;
//            xCPlcErrorEvent.EventDataPointerPush(pxEventData);
////                xCPlcArchiveEvent.EventDataPointerPush(pxEventData);
//            // ���� ���� ����������� ������������� �������,
//            // �� �� ���������(pxEventData) �� ����� � �������,
//            // �������� ������ �������.
//            // ��������� id �������.
//            (pxEventData -> ui16ID) =
//                1;
//            // ��������� ��� �������.
//            (pxEventData -> ui8Type) =
//                CEvents::HANDLED_EVENTS_SYSTEM_ERROR_TYPE;
//            // ��������� ����� ��������� �������.
//            (pxEventData -> ui16Address) = 0;
//            // ��������� ��� �������.
//            (pxEventData -> ui8State) = 1;
//            // ��������� ����� �������.
//            (pxEventData -> xCurrentTime) =
//                xCurrentTime;
//            // �������� ������ ���������� ��������� �������.
//            strcpy((char*)(pxEventData -> acTextDescriptor),
//                   "������ �.�. ���");
//            xCArchiveEventsDB.DataBaseDataPush(pxEventData);
//        }
//    }
//    else
//    {
//        // ������� �������� ����, ��� ������� ���� ������������� �����.
//        xCPlcDataBaseErrorEvent.EventReset(ui8ErrorIndex);
//    }
//    ui8ErrorIndex++;

////-----------------------------------------------------------------------------------------------------
//// ���������� ������ ���� ������ ���������� �������.
//// ���� ������ ���� ������ ���������� �������?
//    if (fbDiscreteOutputModuleDataBaseError)
//    {
////        fbDiscreteOutputModuleDataBaseError = 0;
//        // ���� ������ ���� ������ ���������� �������.
//        pxEventData = xCPlcDataBaseErrorEvent.EventDataPushRequest(ui8ErrorIndex);
//        // ���� ����������� ������������� �������?
//        if (pxEventData)
//        {
//            // ��������� ���� �������� ������������.
//            fucZvkMal = COMMAND_SOUND_SIGNAL_TYPE_ERROR;
//            xCPlcErrorEvent.EventDataPointerPush(pxEventData);
////                xCPlcArchiveEvent.EventDataPointerPush(pxEventData);
//            // ���� ���� ����������� ������������� �������,
//            // �� �� ���������(pxEventData) �� ����� � �������,
//            // �������� ������ �������.
//            // ��������� id �������.
//            (pxEventData -> ui16ID) =
//                1;
//            // ��������� ��� �������.
//            (pxEventData -> ui8Type) =
//                CEvents::HANDLED_EVENTS_SYSTEM_ERROR_TYPE;
//            // ��������� ����� ��������� �������.
//            (pxEventData -> ui16Address) = 0;
//            // ��������� ��� �������.
//            (pxEventData -> ui8State) = 1;
//            // ��������� ����� �������.
//            (pxEventData -> xCurrentTime) =
//                xCurrentTime;
//            // �������� ������ ���������� ��������� �������.
//            strcpy((char*)(pxEventData -> acTextDescriptor),
//                   "������ �.�. ��");
//            xCArchiveEventsDB.DataBaseDataPush(pxEventData);
//        }
//    }
//    else
//    {
//        // ������� �������� ����, ��� ������� ���� ������������� �����.
//        xCPlcDataBaseErrorEvent.EventReset(ui8ErrorIndex);
//    }
//    ui8ErrorIndex++;

////-----------------------------------------------------------------------------------------------------
//// ���������� ������ ���� ������ ���.
//// ���� ������ ���� ������ ���?
//    if (fbLocalNetworkModuleDataBaseError)
//    {
//        // ���� ������ ���� ������ ���.
//        pxEventData = xCPlcDataBaseErrorEvent.EventDataPushRequest(ui8ErrorIndex);
//        // ���� ����������� ������������� �������?
//        if (pxEventData)
//        {
//            // ��������� ���� �������� ������������.
//            fucZvkMal = COMMAND_SOUND_SIGNAL_TYPE_ERROR;
//            xCPlcErrorEvent.EventDataPointerPush(pxEventData);
////                xCPlcArchiveEvent.EventDataPointerPush(pxEventData);
//            // ���� ���� ����������� ������������� �������,
//            // �� �� ���������(pxEventData) �� ����� � �������,
//            // �������� ������ �������.
//            // ��������� id �������.
//            (pxEventData -> ui16ID) =
//                1;
//            // ��������� ��� �������.
//            (pxEventData -> ui8Type) =
//                CEvents::HANDLED_EVENTS_SYSTEM_ERROR_TYPE;
//            // ��������� ����� ��������� �������.
//            (pxEventData -> ui16Address) = 0;
//            // ��������� ��� �������.
//            (pxEventData -> ui8State) = 1;
//            // ��������� ����� �������.
//            (pxEventData -> xCurrentTime) =
//                xCurrentTime;
//            // �������� ������ ���������� ��������� �������.
//            strcpy((char*)(pxEventData -> acTextDescriptor),
//                   "������ �.�. ���");
//            xCArchiveEventsDB.DataBaseDataPush(pxEventData);
//        }
//    }
//    else
//    {
//        // ������� �������� ����, ��� ������� ���� ������������� �����.
//        xCPlcDataBaseErrorEvent.EventReset(ui8ErrorIndex);
//    }
//    ui8ErrorIndex++;

////-----------------------------------------------------------------------------------------------------
//// ���������� ������ ���� ������ �������� �����.
//// ���� ������ ���� ������ �������� �����?
//    if (fbAnalogueInputModuleReperPointsAdcDataBaseError)
//    {
//        // ���� ������ ���� ������ �������� �����.
//        pxEventData = xCPlcDataBaseErrorEvent.EventDataPushRequest(ui8ErrorIndex);
//        // ���� ����������� ������������� �������?
//        if (pxEventData)
//        {
//            // ��������� ���� �������� ������������.
//            fucZvkMal = COMMAND_SOUND_SIGNAL_TYPE_ERROR;
//            xCPlcErrorEvent.EventDataPointerPush(pxEventData);
////                xCPlcArchiveEvent.EventDataPointerPush(pxEventData);
//            // ���� ���� ����������� ������������� �������,
//            // �� �� ���������(pxEventData) �� ����� � �������,
//            // �������� ������ �������.
//            // ��������� id �������.
//            (pxEventData -> ui16ID) =
//                1;
//            // ��������� ��� �������.
//            (pxEventData -> ui8Type) =
//                CEvents::HANDLED_EVENTS_SYSTEM_ERROR_TYPE;
//            // ��������� ����� ��������� �������.
//            (pxEventData -> ui16Address) = 0;
//            // ��������� ��� �������.
//            (pxEventData -> ui8State) = 1;
//            // ��������� ����� �������.
//            (pxEventData -> xCurrentTime) =
//                xCurrentTime;
//            // �������� ������ ���������� ��������� �������.
////            strcpy((char*)(pxEventData -> acTextDescriptor),
////                   "�������� �����");
//            sprintf((char*)(pxEventData -> acTextDescriptor),
//                    "%s%d",
//                    "�������� �. #",
//                    ((pxModuleContext ->
//                      xModuleContextStatic.
//                      ucInsideTypeIndex) +
//                     CONVERT_INTEGER_TO_NATURAL_NUMBER));
//            xCArchiveEventsDB.DataBaseDataPush(pxEventData);
//        }
//    }
//    else
//    {
//        // ������� �������� ����, ��� ������� ���� ������������� �����.
//        xCPlcDataBaseErrorEvent.EventReset(ui8ErrorIndex);
//    }
//    ui8ErrorIndex++;

////-----------------------------------------------------------------------------------------------------
//// ���������� ������ ����� ���� ������.
//// ���� ������ ����� ���� ������?
//    if (fbPlcDataBaseFileError)//if (1)//
//    {
//        // ���� ������ ����� ���� ������.
//        pxEventData = xCPlcDataBaseErrorEvent.EventDataPushRequest(ui8ErrorIndex);
//        // ���� ����������� ������������� �������?
//        if (pxEventData)
//        {
//            // ��������� ���� �������� ������������.
//            fucZvkMal = COMMAND_SOUND_SIGNAL_TYPE_ERROR;
//            xCPlcErrorEvent.EventDataPointerPush(pxEventData);
////                xCPlcArchiveEvent.EventDataPointerPush(pxEventData);
//            // ���� ���� ����������� ������������� �������,
//            // �� �� ���������(pxEventData) �� ����� � �������,
//            // �������� ������ �������.
//            // ��������� id �������.
//            (pxEventData -> ui16ID) =
//                1;
//            // ��������� ��� �������.
//            (pxEventData -> ui8Type) =
//                CEvents::HANDLED_EVENTS_SYSTEM_ERROR_TYPE;
//            // ��������� ����� ��������� �������.
//            (pxEventData -> ui16Address) = 0;
//            // ��������� ��� �������.
//            (pxEventData -> ui8State) = 1;
//            // ��������� ����� �������.
//            (pxEventData -> xCurrentTime) =
//                xCurrentTime;
//            // �������� ������ ���������� ��������� �������.
//            strcpy((char*)(pxEventData -> acTextDescriptor),
//                   "������ ����� �.�");
//            xCArchiveEventsDB.DataBaseDataPush(pxEventData);
//        }
//    }
//    else
//    {
//        // ������� �������� ����, ��� ������� ���� ������������� �����.
//        xCPlcDataBaseErrorEvent.EventReset(ui8ErrorIndex);
//    }
//    ui8ErrorIndex++;

////-----------------------------------------------------------------------------------------------------
//// ���������� ������ ���� ������ ���.
//// ���� ������ ���� ������ ���?
//    if (fbAnalogueInputModuleDataBaseError)//if (1)//
//    {
//        // ������� ������ ������� ������ ���.
//        ui8IndexNumber = xPlcConfigService.xPlcConfigServiceData.ucLastDiscreteInputModuleIndex + 1;
//        // ������� ��������� �� �������� ������.
//        pxModuleContext = &xAllModulesContext.axAllModulesContext[ui8IndexNumber];
//        // ������ �� ���� ��������� ���.
//        for (int i = 0;
//                (i < xPlcConfigService.xPlcConfigServiceData.ucServiceAnalogueInputModuleQuantity);
//                i++)
//        {
//            // ���� ������ ���� ������ ���?
//            if ((pxModuleContext ->
//                    xModuleContextDinamic.
//                    ucErrorCode) == INTERNAL_MODULE_ERROR_DATA_BASE)
////            if (1)//
//            {
//                // ���� ������ ���� ������ ���.
//                pxEventData = xCPlcDataBaseErrorEvent.EventDataPushRequest(ui8ErrorIndex);
//                // ���� ����������� ������������� �������?
//                if (pxEventData)
//                {
//                    // ��������� ���� �������� ������������.
//                    fucZvkMal = COMMAND_SOUND_SIGNAL_TYPE_ERROR;
//                    xCPlcErrorEvent.EventDataPointerPush(pxEventData);
////                xCPlcArchiveEvent.EventDataPointerPush(pxEventData);
//                    // ���� ���� ����������� ������������� �������,
//                    // �� �� ���������(pxEventData) �� ����� � �������,
//                    // �������� ������ �������.
//                    // ��������� id �������.
//                    (pxEventData -> ui16ID) =
//                        1;
//                    // ��������� ��� �������.
//                    (pxEventData -> ui8Type) =
//                        CEvents::HANDLED_EVENTS_SYSTEM_ERROR_TYPE;
//                    // ��������� ����� ��������� �������.
//                    (pxEventData -> ui16Address) =
//                        ((pxModuleContext ->
//                          xModuleContextStatic.
//                          ucInsideTypeIndex) +
//                         CONVERT_INTEGER_TO_NATURAL_NUMBER);
//                    // ��������� ��� �������.
//                    (pxEventData -> ui8State) = 1;
//                    // ��������� ����� �������.
//                    (pxEventData -> xCurrentTime) =
//                        xCurrentTime;
////                    strcpy((char*)(pxEventData -> acTextDescriptor),
////                           "������ �.�. ���");
//                    sprintf((char*)(pxEventData -> acTextDescriptor),
//                            "%s%d",
//                            "������ �. ���#",
//                            ((pxModuleContext ->
//                              xModuleContextStatic.
//                              ucInsideTypeIndex) +
//                             CONVERT_INTEGER_TO_NATURAL_NUMBER));
//                    xCArchiveEventsDB.DataBaseDataPush(pxEventData);
//                }
//            }
//            else
//            {
//                // ������� �������� ����, ��� ������� ���� ������������� �����.
//                xCPlcDataBaseErrorEvent.EventReset(ui8ErrorIndex);
//            }
//
//            ui8ErrorIndex++;
//            pxModuleContext++;
//        }
//    }

////-----------------------------------------------------------------------------------------------------
//// ���������� ������ CRC ������� ��������.
//// ���� ������ CRC ������� ��������?
//    if (xCCrc.i8Error)
//    {
//        // ���� ������ CRC ������� ��������.
//        pxEventData = xCPlcDataBaseErrorEvent.EventDataPushRequest(ui8ErrorIndex);
//        // ���� ����������� ������������� �������?
//        if (pxEventData)
//        {
//            // ��������� ���� �������� ������������.
//            fucZvkMal = COMMAND_SOUND_SIGNAL_TYPE_ERROR;
//            xCPlcErrorEvent.EventDataPointerPush(pxEventData);
////                xCPlcArchiveEvent.EventDataPointerPush(pxEventData);
//            // ���� ���� ����������� ������������� �������,
//            // �� �� ���������(pxEventData) �� ����� � �������,
//            // �������� ������ �������.
//            // ��������� id �������.
//            (pxEventData -> ui16ID) =
//                1;
//            // ��������� ��� �������.
//            (pxEventData -> ui8Type) =
//                CEvents::HANDLED_EVENTS_SYSTEM_ERROR_TYPE;
//            // ��������� ����� ��������� �������.
//            (pxEventData -> ui16Address) = (xCCrc.i8Error);
//            // ��������� ��� �������.
//            (pxEventData -> ui8State) = 1;
//            // ��������� ����� �������.
//            (pxEventData -> xCurrentTime) =
//                xCurrentTime;
//            // �������� ������ ���������� ��������� �������.
//            sprintf((char*)(pxEventData -> acTextDescriptor),
//                    "%s%02X",
//                    "������ CRC E",
//                    (xCCrc.i8Error));
////            strcpy((char*)(pxEventData -> acTextDescriptor),
////                   "������ CRC");
//            xCArchiveEventsDB.DataBaseDataPush(pxEventData);
//        }
//    }
//    else
//    {
//        // ������� �������� ����, ��� ������� ���� ������������� �����.
//        xCPlcDataBaseErrorEvent.EventReset(ui8ErrorIndex);
//    }
//    ui8ErrorIndex++;

////-----------------------------------------------------------------------------------------------------
//// ���������� ������ ����� � ����.
//// ���� ������ ����� � ����?
//    if (fbMindDataExchangeError)
//    {
//        fbMindDataExchangeError = 0;
//        // ���� ������ ����� � ����.
//        pxEventData = xCPlcDataBaseErrorEvent.EventDataPushRequest(ui8ErrorIndex);
//        // ���� ����������� ������������� �������?
//        if (pxEventData)
//        {
//            // ��������� ���� �������� ������������.
//            fucZvkMal = COMMAND_SOUND_SIGNAL_TYPE_ERROR;
//            xCPlcErrorEvent.EventDataPointerPush(pxEventData);
////                xCPlcArchiveEvent.EventDataPointerPush(pxEventData);
//            // ���� ���� ����������� ������������� �������,
//            // �� �� ���������(pxEventData) �� ����� � �������,
//            // �������� ������ �������.
//            // ��������� id �������.
//            (pxEventData -> ui16ID) =
//                1;
//            // ��������� ��� �������.
//            (pxEventData -> ui8Type) =
//                CEvents::HANDLED_EVENTS_SYSTEM_ERROR_TYPE;
//            // ��������� ����� ��������� �������.
//            (pxEventData -> ui16Address) = (xCCrc.i8Error);
//            // ��������� ��� �������.
//            (pxEventData -> ui8State) = 1;
//            // ��������� ����� �������.
//            (pxEventData -> xCurrentTime) =
//                xCurrentTime;
//            // �������� ������ ���������� ��������� �������.
////            sprintf((char*)(pxEventData -> acTextDescriptor),
////                    "%s%02X",
////                    "��� ����� � ����",
////                    (xCCrc.i8Error));
//            strcpy((char*)(pxEventData -> acTextDescriptor),
//                   "��� ����� � ����");
//            xCArchiveEventsDB.DataBaseDataPush(pxEventData);
//        }
//    }
//    else
//    {
//        // ������� �������� ����, ��� ������� ���� ������������� �����.
//        xCPlcDataBaseErrorEvent.EventReset(ui8ErrorIndex);
//    }
//    ui8ErrorIndex++;
//
////-----------------------------------------------------------------------------------------------------
//// ���������� ������ ����� � CodeSys.
//// ���� ������ ����� � CodeSys?
//    if (fbCodeSysDataExchangeError)
//    {
//        fbCodeSysDataExchangeError = 0;
//        // ���� ������ ����� � CodeSys.
//        pxEventData = xCPlcDataBaseErrorEvent.EventDataPushRequest(ui8ErrorIndex);
//        // ���� ����������� ������������� �������?
//        if (pxEventData)
//        {
////            // ��������� ���� �������� ������������.
////            fucZvkMal = COMMAND_SOUND_SIGNAL_TYPE_ERROR;
//            xCPlcErrorEvent.EventDataPointerPush(pxEventData);
////                xCPlcArchiveEvent.EventDataPointerPush(pxEventData);
//            // ���� ���� ����������� ������������� �������,
//            // �� �� ���������(pxEventData) �� ����� � �������,
//            // �������� ������ �������.
//            // ��������� id �������.
//            (pxEventData -> ui16ID) =
//                1;
//            // ��������� ��� �������.
//            (pxEventData -> ui8Type) =
//                CEvents::HANDLED_EVENTS_SYSTEM_ERROR_TYPE;
//            // ��������� ����� ��������� �������.
//            (pxEventData -> ui16Address) = (0);
//            // ��������� ��� �������.
//            (pxEventData -> ui8State) = 1;
//            // ��������� ����� �������.
//            (pxEventData -> xCurrentTime) =
//                xCurrentTime;
//            // �������� ������ ���������� ��������� �������.
////            sprintf((char*)(pxEventData -> acTextDescriptor),
////                    "%s%02X",
////                    "��� ����� � ����",
////                    (xCCrc.i8Error));
//            strcpy((char*)(pxEventData -> acTextDescriptor),
//                   "������ CodeSys");
//            xCArchiveEventsDB.DataBaseDataPush(pxEventData);
//        }
//    }
//    else
//    {
//        // ������� �������� ����, ��� ������� ���� ������������� �����.
//        xCPlcDataBaseErrorEvent.EventReset(ui8ErrorIndex);
//    }
//    ui8ErrorIndex++;


//-----------------------------------------------------------------------------------------------------
}

//-----------------------------------------------------------------------------------------------------

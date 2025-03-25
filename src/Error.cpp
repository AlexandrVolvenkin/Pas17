

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
//    "Вкл. реле",
//    "Вкл. реле",
//    "Вкл. реле",
//    "Вкл. реле",
//    "Вкл. реле",
//    "Вкл. реле",
//    "Вкл. реле",
//    "Вкл. реле",
//};
//
//char* apcAnalogueOutputGroupEventsStrings[16] =
//{
//    "Ошибка ЦАП",
//    "Ошибка ЦАП",
//    "Ошибка ЦАП",
//    "Ошибка ЦАП",
//    "Ошибка ЦАП",
//    "Ошибка ЦАП",
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
////        "Отказ модуля #"
////    },
////    {
////        "Отказ модуля #1"
////    },
////    {
////        "Отказ модуля #2"
////    },
////    {
////        "Отказ модуля #3"
////    },
////    {
////        "Отказ модуля #4"
////    },
////    {
////        "Отказ модуля #5"
////    },
////    {
////        "Отказ модуля #6"
////    },
////    {
////        "Отказ модуля #7"
////    },
////    {
////        "Программирование"
////    },
////    {
////        "Отказ комм. проц."
////    },
////    {
////        "Ошибка конфигурации"
////    },
////    {
////        "Ошибка локальн. сети"
////    },
////    {
////        "Питание отключено"
////    },
////    {
////        "Переконфигурирование"
////    },
////    {
////        "Отказ мод. индикации"
////    },
////    {
////        "Ошибка сист. интерф."
////    },
////    {
////        "Перезапуск по Reset"
////    },
////    {
////        "Перезапуск по WDT"
////    }
//};
//
////-----------------------------------------------------------------------------------------------------
//CError::TErrorTextDescriptor CError::axDataBaseErrorTextDescriptor[]
//{
//    {
//        "вх.дискр.сигналов"
//    },
//    {
//        "ФБЛ"
//    },
//    {
//        "МР"
//    },
//    {
//        "МБМ"
//    },
//    {
//        "аналоговых лок.сети"
//    },
//    {
//        "реп. точки калибров"
//    },
//    {
//        "МВА"
//    }
//};

//-----------------------------------------------------------------------------------------------------
// функция преобразует коды ошибок Modbus во внутренние коды.
uint8_t CError::ModbusErrorToPlcError(int32_t i32Errno)
{
    uint8_t ui8Errno;
    // какая ошибка Modbus?
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
// функция проверки наличия ошибок.
// создаёт объект - ошибка и помещает в журнал ошибок.
void CError::ErrorLogCreate(void)
{
    uint8_t ui8IndexNumber;
    uint8_t ui8ErrorIndex;
    TModuleContext *pxModuleContext;
    CEvents::TEventDataCommon *pxEventData;

    CEvents::EventsHandler();

//    // обработаем ошибки отказов внутренних модулей.
//    for (int i = 0;
//            i <
//            (xPlcConfigService.xPlcConfigServiceData.ucInternalModulesQuantity);
//            i++)
//    {
//        // получим указатель на контекст модуля.
//        pxModuleContext = &xAllModulesContext.axAllModulesContext[i];
//        // есть отказ текущего модуля?
////        if (*(pxModuleContext ->
////                xModuleContextStatic.
////                pucModuleBadStateBufferPointer))
//        // есть ошибки текущего модуля?
//        if (pxModuleContext ->
//                xModuleContextDinamic.
//                ucErrorCode)
////        if (i < 8)
//        {
//            // есть отказ текущего модуля.
//            // установим флаг - отказ модуля.
//            aucCoilsArray[BAD_MODULES_BIT_ARRAY_OFFSET + i] = 1;
//
//            CEvents::TEventDataCommon *pxEventData;
//            pxEventData = xCInternalModuleErrorEvent.EventDataPushRequest(i);
//            // есть возможность зафиксировать событие?
//            if (pxEventData)
//            {
//                // установим флаг звуковой сигнализации.
//                fucZvkMal = COMMAND_SOUND_SIGNAL_TYPE_ERROR;
////                printf("CError::ErrorLogCreate(void) ErrorLogCreate: i %d\n\r", (i));
//                xCPlcErrorEvent.EventDataPointerPush(pxEventData);
////                xCPlcArchiveEvent.EventDataPointerPush(pxEventData);
//                // если есть возможность зафиксировать событие,
//                // то по указателю(pxEventData) на место в журнале,
//                // сохраним данные события.
//                // установим id события.
//                (pxEventData -> ui16ID) =
//                    1;
//                // установим тип события.
//                (pxEventData -> ui8Type) =
//                    CEvents::HANDLED_EVENTS_INTERNAL_MODULES_BAD_TYPE;
//                // установим адрес источника события.
//                (pxEventData -> ui16Address) =
//                    (pxModuleContext ->
//                     xModuleContextStatic.
//                     ucModuleAddress);
////                printf("CError::ucModuleAddress %d\n\r", (pxModuleContext ->
////                     xModuleContextStatic.
////                     ucModuleAddress));
//                // установим код события.
//                (pxEventData -> ui8State) =
//                    (pxModuleContext ->
//                     xModuleContextDinamic.
//                     ucErrorCode);
//                // отказ текущего модуля при старте?
//                if (*(pxModuleContext ->
//                        xModuleContextStatic.
//                        pucModuleBadStateBufferPointer) ==
//                        BAD_MODULE_NOT_RESPONDED_ON_START)
//                {
//                    // установим код времени события - при старте.
//                    (pxEventData -> xCurrentTime.tm_sec) = 0xFF;
//                }
//                else
//                {
//                    // установим время события.
//                    (pxEventData -> xCurrentTime) =
//                        xCurrentTime;
//                }
//                // создадим строку текстового реквизита события.
//                //                sprintf((char*)(pxEventData -> acTextDescriptor),
////                        "%s%d",
////                        "Отказ модуля #",
////                        ((pxModuleContext ->
////                          xModuleContextStatic.
////                          ucModuleAddress) +
////                         CONVERT_INTEGER_TO_NATURAL_NUMBER));
//                sprintf((char*)(pxEventData -> acTextDescriptor),
//                        "%s%02d %s%02X",
//                        "Отказ модуля #",
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
//            // очистим признаки того, что событие было зафиксировано ранее.
//            xCInternalModuleErrorEvent.EventReset(i);
//        }
//    }

////-----------------------------------------------------------------------------------------------------
//    // обработаем ошибки отказов внешних модулей.
//    for (int i = 0;
//            i < (xPlcConfigService.xPlcConfigServiceData.ucExternalModulesQuantity);
//            i++)
//    {
//        // получим указатель на контекст модуля.
//        pxModuleContext = &xAllModulesContext.axAllModulesContext[i +
//                          (xPlcConfigService.xPlcConfigServiceData.ucInternalModulesQuantity)];
//        // есть отказ текущего модуля?
//        if (*(pxModuleContext ->
//                xModuleContextStatic.
//                pucModuleBadStateBufferPointer))
//        {
//            pxEventData = xCExternalModuleErrorEvent.EventDataPushRequest(i);
//            // есть возможность зафиксировать событие?
//            if (pxEventData)
//            {
////                cout << "ErrorLogCreate xCExternalModuleErrorEvent" << endl;
////                // установим флаг звуковой сигнализации.
////                fucZvkMal = COMMAND_SOUND_SIGNAL_TYPE_ERROR;
//                xCPlcErrorEvent.EventDataPointerPush(pxEventData);
////                xCPlcArchiveEvent.EventDataPointerPush(pxEventData);
//                // если есть возможность зафиксировать событие,
//                // то по указателю(pxEventData) на место в журнале,
//                // сохраним данные события.
//                // установим id события.
//                (pxEventData -> ui16ID) =
//                    1;
//                // установим тип события.
//                (pxEventData -> ui8Type) =
//                    CEvents::HANDLED_EVENTS_EXTERNAL_MODULES_BAD_TYPE;
//                // установим адрес источника события.
//                (pxEventData -> ui16Address) =
//                    (pxModuleContext ->
//                     xModuleContextStatic.
//                     ucModuleAddress);
//                // установим код события.
//                (pxEventData -> ui8State) =
//                    (pxModuleContext ->
//                     xModuleContextDinamic.
//                     ucErrorCode);
//                // установим время события.
//                (pxEventData -> xCurrentTime) =
//                    xCurrentTime;
//                // создадим строку текстового реквизита события.
//                sprintf((char*)(pxEventData -> acTextDescriptor),
//                        "%s%02d %s%02X",
//                        "Ошибка SL#",
//                        (pxModuleContext ->
//                         xModuleContextStatic.
//                         ucModuleAddress),
//                        "E",
//                        (pxModuleContext ->
//                         xModuleContextDinamic.
//                         ucErrorCode));
////                sprintf((char*)(pxEventData -> acTextDescriptor),
////                        "%s%02X",
////                        "Ошибка SL HEX-",
////                        (pxModuleContext ->
////                         xModuleContextDinamic.
////                         ucErrorCode));
//                xCArchiveEventsDB.DataBaseDataPush(pxEventData);
//            }
//        }
//        else
//        {
//            // очистим признаки того, что событие было зафиксировано ранее.
//            xCExternalModuleErrorEvent.EventReset(i);
//        }
//    }

////-----------------------------------------------------------------------------------------------------
//// обработаем ошибку конфигурации.
//// есть ошибка конфигурации?
//    if (fbPlcConfigurationError)//if (1)//
//    {
//        // есть ошибка конфигурации.
//        pxEventData = xCPlcConfigurationErrorEvent.EventDataPushRequest(0);
//        // есть возможность зафиксировать событие?
//        if (pxEventData)
//        {
//            // установим флаг звуковой сигнализации.
//            fucZvkMal = COMMAND_SOUND_SIGNAL_TYPE_ERROR;
//            xCPlcErrorEvent.EventDataPointerPush(pxEventData);
//            // если есть возможность зафиксировать событие,
//            // то по указателю(pxEventData) на место в журнале,
//            // сохраним данные события.
//            // установим id события.
//            (pxEventData -> ui16ID) =
//                1;
//            // установим тип события.
//            (pxEventData -> ui8Type) =
//                CEvents::HANDLED_EVENTS_CONFIGURATION_ERROR_TYPE;
//            // создадим строку данных события.
//            // создадим строку с типами модулей текущей конфигурацией.
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
//                // создадим строку текстового реквизита события.
//                sprintf((char*)(pxEventData -> acTextDescriptor),
//                        "%s",
//                        "Ошибка послед. групп");
//                break;
//            case CONFIGURATION_ERROR_EMPTY_SLOT:
//                // создадим строку текстового реквизита события.
//                sprintf((char*)(pxEventData -> acTextDescriptor),
//                        "%s",
//                        "Ошибка пустой слот");
//                break;
//
//            case CONFIGURATION_ERROR_NEW_CONFIGURATION_AND_DATA_BASE:
//                // создадим строку текстового реквизита события.
//                sprintf((char*)(pxEventData -> acTextDescriptor),
//                        "%s",
//                        "Новая конф. и база.д");
//                break;
//            case CONFIGURATION_ERROR_NEW_CONFIGURATION:
//                // создадим строку текстового реквизита события.
//                sprintf((char*)(pxEventData -> acTextDescriptor),
//                        "%s",
//                        "Новая конфигурация");
//                break;
//            case CONFIGURATION_ERROR_BAD_MODULE_FINDED_AND_REGISTERED:
//                // создадим строку текстового реквизита события.
//                sprintf((char*)(pxEventData -> acTextDescriptor),
//                        "%s",
//                        "Отказ мод. зарегистр");
//                break;
//
//            default:
//                // создадим строку текстового реквизита события.
//                sprintf((char*)(pxEventData -> acTextDescriptor),
//                        "%s",
//                        "Ошибка конфигурации");
//                break;
//            };
//        }
//    }
//    else
//    {
//        // очистим признаки того, что событие было зафиксировано ранее.
//        xCPlcConfigurationErrorEvent.EventReset(0);
//    }
//
//    ui8ErrorIndex = 0;

////-----------------------------------------------------------------------------------------------------
//// обработаем ошибку базы данных дискретных сигналов.
//// есть ошибка базы данных дискретных сигналов?
//    if (fbDiscreteInputDataBaseError)
//    {
//        // есть ошибка базы данных дискретных сигналов.
//        pxEventData = xCPlcDataBaseErrorEvent.EventDataPushRequest(ui8ErrorIndex);
//        // есть возможность зафиксировать событие?
//        if (pxEventData)
//        {
//            // установим флаг звуковой сигнализации.
//            fucZvkMal = COMMAND_SOUND_SIGNAL_TYPE_ERROR;
//            xCPlcErrorEvent.EventDataPointerPush(pxEventData);
////                xCPlcArchiveEvent.EventDataPointerPush(pxEventData);
//            // если есть возможность зафиксировать событие,
//            // то по указателю(pxEventData) на место в журнале,
//            // сохраним данные события.
//            // установим id события.
//            (pxEventData -> ui16ID) =
//                1;
//            // установим тип события.
//            (pxEventData -> ui8Type) =
//                CEvents::HANDLED_EVENTS_SYSTEM_ERROR_TYPE;
//            // установим адрес источника события.
//            (pxEventData -> ui16Address) = 0;
//            // установим код события.
//            (pxEventData -> ui8State) = 1;
//            // установим время события.
//            (pxEventData -> xCurrentTime) =
//                xCurrentTime;
//            // создадим строку текстового реквизита события.
//            strcpy((char*)(pxEventData -> acTextDescriptor),
//                   "вх.диск.сигналов");
//            xCArchiveEventsDB.DataBaseDataPush(pxEventData);
//        }
//    }
//    else
//    {
//        // очистим признаки того, что событие было зафиксировано ранее.
//        xCPlcDataBaseErrorEvent.EventReset(ui8ErrorIndex);
//    }
//    ui8ErrorIndex++;

////-----------------------------------------------------------------------------------------------------
//// обработаем ошибку базы данных функциональных блоков.
//// есть ошибка базы данных функциональных блоков?
//    if (fbFunctionBlockDataBaseError)
//    {
//        // есть ошибка базы данных функциональных блоков.
//        pxEventData = xCPlcDataBaseErrorEvent.EventDataPushRequest(ui8ErrorIndex);
//        // есть возможность зафиксировать событие?
//        if (pxEventData)
//        {
//            // установим флаг звуковой сигнализации.
//            fucZvkMal = COMMAND_SOUND_SIGNAL_TYPE_ERROR;
//            xCPlcErrorEvent.EventDataPointerPush(pxEventData);
////                xCPlcArchiveEvent.EventDataPointerPush(pxEventData);
//            // если есть возможность зафиксировать событие,
//            // то по указателю(pxEventData) на место в журнале,
//            // сохраним данные события.
//            // установим id события.
//            (pxEventData -> ui16ID) =
//                1;
//            // установим тип события.
//            (pxEventData -> ui8Type) =
//                CEvents::HANDLED_EVENTS_SYSTEM_ERROR_TYPE;
//            // установим адрес источника события.
//            (pxEventData -> ui16Address) = 0;
//            // установим код события.
//            (pxEventData -> ui8State) = 1;
//            // установим время события.
//            (pxEventData -> xCurrentTime) =
//                xCurrentTime;
//            // создадим строку текстового реквизита события.
//            strcpy((char*)(pxEventData -> acTextDescriptor),
//                   "Ошибка б.д. ФБЛ");
//            xCArchiveEventsDB.DataBaseDataPush(pxEventData);
//        }
//    }
//    else
//    {
//        // очистим признаки того, что событие было зафиксировано ранее.
//        xCPlcDataBaseErrorEvent.EventReset(ui8ErrorIndex);
//    }
//    ui8ErrorIndex++;

////-----------------------------------------------------------------------------------------------------
//// обработаем ошибку базы данных дискретных выходов.
//// есть ошибка базы данных дискретных выходов?
//    if (fbDiscreteOutputModuleDataBaseError)
//    {
////        fbDiscreteOutputModuleDataBaseError = 0;
//        // есть ошибка базы данных дискретных выходов.
//        pxEventData = xCPlcDataBaseErrorEvent.EventDataPushRequest(ui8ErrorIndex);
//        // есть возможность зафиксировать событие?
//        if (pxEventData)
//        {
//            // установим флаг звуковой сигнализации.
//            fucZvkMal = COMMAND_SOUND_SIGNAL_TYPE_ERROR;
//            xCPlcErrorEvent.EventDataPointerPush(pxEventData);
////                xCPlcArchiveEvent.EventDataPointerPush(pxEventData);
//            // если есть возможность зафиксировать событие,
//            // то по указателю(pxEventData) на место в журнале,
//            // сохраним данные события.
//            // установим id события.
//            (pxEventData -> ui16ID) =
//                1;
//            // установим тип события.
//            (pxEventData -> ui8Type) =
//                CEvents::HANDLED_EVENTS_SYSTEM_ERROR_TYPE;
//            // установим адрес источника события.
//            (pxEventData -> ui16Address) = 0;
//            // установим код события.
//            (pxEventData -> ui8State) = 1;
//            // установим время события.
//            (pxEventData -> xCurrentTime) =
//                xCurrentTime;
//            // создадим строку текстового реквизита события.
//            strcpy((char*)(pxEventData -> acTextDescriptor),
//                   "Ошибка б.д. МР");
//            xCArchiveEventsDB.DataBaseDataPush(pxEventData);
//        }
//    }
//    else
//    {
//        // очистим признаки того, что событие было зафиксировано ранее.
//        xCPlcDataBaseErrorEvent.EventReset(ui8ErrorIndex);
//    }
//    ui8ErrorIndex++;

////-----------------------------------------------------------------------------------------------------
//// обработаем ошибку базы данных МБМ.
//// есть ошибка базы данных МБМ?
//    if (fbLocalNetworkModuleDataBaseError)
//    {
//        // есть ошибка базы данных МБМ.
//        pxEventData = xCPlcDataBaseErrorEvent.EventDataPushRequest(ui8ErrorIndex);
//        // есть возможность зафиксировать событие?
//        if (pxEventData)
//        {
//            // установим флаг звуковой сигнализации.
//            fucZvkMal = COMMAND_SOUND_SIGNAL_TYPE_ERROR;
//            xCPlcErrorEvent.EventDataPointerPush(pxEventData);
////                xCPlcArchiveEvent.EventDataPointerPush(pxEventData);
//            // если есть возможность зафиксировать событие,
//            // то по указателю(pxEventData) на место в журнале,
//            // сохраним данные события.
//            // установим id события.
//            (pxEventData -> ui16ID) =
//                1;
//            // установим тип события.
//            (pxEventData -> ui8Type) =
//                CEvents::HANDLED_EVENTS_SYSTEM_ERROR_TYPE;
//            // установим адрес источника события.
//            (pxEventData -> ui16Address) = 0;
//            // установим код события.
//            (pxEventData -> ui8State) = 1;
//            // установим время события.
//            (pxEventData -> xCurrentTime) =
//                xCurrentTime;
//            // создадим строку текстового реквизита события.
//            strcpy((char*)(pxEventData -> acTextDescriptor),
//                   "Ошибка б.д. МБМ");
//            xCArchiveEventsDB.DataBaseDataPush(pxEventData);
//        }
//    }
//    else
//    {
//        // очистим признаки того, что событие было зафиксировано ранее.
//        xCPlcDataBaseErrorEvent.EventReset(ui8ErrorIndex);
//    }
//    ui8ErrorIndex++;

////-----------------------------------------------------------------------------------------------------
//// обработаем ошибку базы данных реперных точек.
//// есть ошибка базы данных реперных точек?
//    if (fbAnalogueInputModuleReperPointsAdcDataBaseError)
//    {
//        // есть ошибка базы данных реперных точек.
//        pxEventData = xCPlcDataBaseErrorEvent.EventDataPushRequest(ui8ErrorIndex);
//        // есть возможность зафиксировать событие?
//        if (pxEventData)
//        {
//            // установим флаг звуковой сигнализации.
//            fucZvkMal = COMMAND_SOUND_SIGNAL_TYPE_ERROR;
//            xCPlcErrorEvent.EventDataPointerPush(pxEventData);
////                xCPlcArchiveEvent.EventDataPointerPush(pxEventData);
//            // если есть возможность зафиксировать событие,
//            // то по указателю(pxEventData) на место в журнале,
//            // сохраним данные события.
//            // установим id события.
//            (pxEventData -> ui16ID) =
//                1;
//            // установим тип события.
//            (pxEventData -> ui8Type) =
//                CEvents::HANDLED_EVENTS_SYSTEM_ERROR_TYPE;
//            // установим адрес источника события.
//            (pxEventData -> ui16Address) = 0;
//            // установим код события.
//            (pxEventData -> ui8State) = 1;
//            // установим время события.
//            (pxEventData -> xCurrentTime) =
//                xCurrentTime;
//            // создадим строку текстового реквизита события.
////            strcpy((char*)(pxEventData -> acTextDescriptor),
////                   "Реперные точки");
//            sprintf((char*)(pxEventData -> acTextDescriptor),
//                    "%s%d",
//                    "Реперные т. #",
//                    ((pxModuleContext ->
//                      xModuleContextStatic.
//                      ucInsideTypeIndex) +
//                     CONVERT_INTEGER_TO_NATURAL_NUMBER));
//            xCArchiveEventsDB.DataBaseDataPush(pxEventData);
//        }
//    }
//    else
//    {
//        // очистим признаки того, что событие было зафиксировано ранее.
//        xCPlcDataBaseErrorEvent.EventReset(ui8ErrorIndex);
//    }
//    ui8ErrorIndex++;

////-----------------------------------------------------------------------------------------------------
//// обработаем ошибку общей базы данных.
//// есть ошибка общей базы данных?
//    if (fbPlcDataBaseFileError)//if (1)//
//    {
//        // есть ошибка общей базы данных.
//        pxEventData = xCPlcDataBaseErrorEvent.EventDataPushRequest(ui8ErrorIndex);
//        // есть возможность зафиксировать событие?
//        if (pxEventData)
//        {
//            // установим флаг звуковой сигнализации.
//            fucZvkMal = COMMAND_SOUND_SIGNAL_TYPE_ERROR;
//            xCPlcErrorEvent.EventDataPointerPush(pxEventData);
////                xCPlcArchiveEvent.EventDataPointerPush(pxEventData);
//            // если есть возможность зафиксировать событие,
//            // то по указателю(pxEventData) на место в журнале,
//            // сохраним данные события.
//            // установим id события.
//            (pxEventData -> ui16ID) =
//                1;
//            // установим тип события.
//            (pxEventData -> ui8Type) =
//                CEvents::HANDLED_EVENTS_SYSTEM_ERROR_TYPE;
//            // установим адрес источника события.
//            (pxEventData -> ui16Address) = 0;
//            // установим код события.
//            (pxEventData -> ui8State) = 1;
//            // установим время события.
//            (pxEventData -> xCurrentTime) =
//                xCurrentTime;
//            // создадим строку текстового реквизита события.
//            strcpy((char*)(pxEventData -> acTextDescriptor),
//                   "Ошибка общей б.д");
//            xCArchiveEventsDB.DataBaseDataPush(pxEventData);
//        }
//    }
//    else
//    {
//        // очистим признаки того, что событие было зафиксировано ранее.
//        xCPlcDataBaseErrorEvent.EventReset(ui8ErrorIndex);
//    }
//    ui8ErrorIndex++;

////-----------------------------------------------------------------------------------------------------
//// обработаем ошибку базы данных МВА.
//// есть ошибка базы данных МВА?
//    if (fbAnalogueInputModuleDataBaseError)//if (1)//
//    {
//        // получим индекс первого модуля МВА.
//        ui8IndexNumber = xPlcConfigService.xPlcConfigServiceData.ucLastDiscreteInputModuleIndex + 1;
//        // получим указатель на контекст модуля.
//        pxModuleContext = &xAllModulesContext.axAllModulesContext[ui8IndexNumber];
//        // пройдём по всем имеющимся МВА.
//        for (int i = 0;
//                (i < xPlcConfigService.xPlcConfigServiceData.ucServiceAnalogueInputModuleQuantity);
//                i++)
//        {
//            // есть ошибка базы данных МВА?
//            if ((pxModuleContext ->
//                    xModuleContextDinamic.
//                    ucErrorCode) == INTERNAL_MODULE_ERROR_DATA_BASE)
////            if (1)//
//            {
//                // есть ошибка базы данных МВА.
//                pxEventData = xCPlcDataBaseErrorEvent.EventDataPushRequest(ui8ErrorIndex);
//                // есть возможность зафиксировать событие?
//                if (pxEventData)
//                {
//                    // установим флаг звуковой сигнализации.
//                    fucZvkMal = COMMAND_SOUND_SIGNAL_TYPE_ERROR;
//                    xCPlcErrorEvent.EventDataPointerPush(pxEventData);
////                xCPlcArchiveEvent.EventDataPointerPush(pxEventData);
//                    // если есть возможность зафиксировать событие,
//                    // то по указателю(pxEventData) на место в журнале,
//                    // сохраним данные события.
//                    // установим id события.
//                    (pxEventData -> ui16ID) =
//                        1;
//                    // установим тип события.
//                    (pxEventData -> ui8Type) =
//                        CEvents::HANDLED_EVENTS_SYSTEM_ERROR_TYPE;
//                    // установим адрес источника события.
//                    (pxEventData -> ui16Address) =
//                        ((pxModuleContext ->
//                          xModuleContextStatic.
//                          ucInsideTypeIndex) +
//                         CONVERT_INTEGER_TO_NATURAL_NUMBER);
//                    // установим код события.
//                    (pxEventData -> ui8State) = 1;
//                    // установим время события.
//                    (pxEventData -> xCurrentTime) =
//                        xCurrentTime;
////                    strcpy((char*)(pxEventData -> acTextDescriptor),
////                           "Ошибка б.д. МВА");
//                    sprintf((char*)(pxEventData -> acTextDescriptor),
//                            "%s%d",
//                            "Ошибка б. МВА#",
//                            ((pxModuleContext ->
//                              xModuleContextStatic.
//                              ucInsideTypeIndex) +
//                             CONVERT_INTEGER_TO_NATURAL_NUMBER));
//                    xCArchiveEventsDB.DataBaseDataPush(pxEventData);
//                }
//            }
//            else
//            {
//                // очистим признаки того, что событие было зафиксировано ранее.
//                xCPlcDataBaseErrorEvent.EventReset(ui8ErrorIndex);
//            }
//
//            ui8ErrorIndex++;
//            pxModuleContext++;
//        }
//    }

////-----------------------------------------------------------------------------------------------------
//// обработаем ошибку CRC рабочих массивов.
//// есть ошибка CRC рабочих массивов?
//    if (xCCrc.i8Error)
//    {
//        // есть ошибка CRC рабочих массивов.
//        pxEventData = xCPlcDataBaseErrorEvent.EventDataPushRequest(ui8ErrorIndex);
//        // есть возможность зафиксировать событие?
//        if (pxEventData)
//        {
//            // установим флаг звуковой сигнализации.
//            fucZvkMal = COMMAND_SOUND_SIGNAL_TYPE_ERROR;
//            xCPlcErrorEvent.EventDataPointerPush(pxEventData);
////                xCPlcArchiveEvent.EventDataPointerPush(pxEventData);
//            // если есть возможность зафиксировать событие,
//            // то по указателю(pxEventData) на место в журнале,
//            // сохраним данные события.
//            // установим id события.
//            (pxEventData -> ui16ID) =
//                1;
//            // установим тип события.
//            (pxEventData -> ui8Type) =
//                CEvents::HANDLED_EVENTS_SYSTEM_ERROR_TYPE;
//            // установим адрес источника события.
//            (pxEventData -> ui16Address) = (xCCrc.i8Error);
//            // установим код события.
//            (pxEventData -> ui8State) = 1;
//            // установим время события.
//            (pxEventData -> xCurrentTime) =
//                xCurrentTime;
//            // создадим строку текстового реквизита события.
//            sprintf((char*)(pxEventData -> acTextDescriptor),
//                    "%s%02X",
//                    "Ошибка CRC E",
//                    (xCCrc.i8Error));
////            strcpy((char*)(pxEventData -> acTextDescriptor),
////                   "Ошибка CRC");
//            xCArchiveEventsDB.DataBaseDataPush(pxEventData);
//        }
//    }
//    else
//    {
//        // очистим признаки того, что событие было зафиксировано ранее.
//        xCPlcDataBaseErrorEvent.EventReset(ui8ErrorIndex);
//    }
//    ui8ErrorIndex++;

////-----------------------------------------------------------------------------------------------------
//// обработаем ошибку связи с МИНД.
//// есть ошибка связи с МИНД?
//    if (fbMindDataExchangeError)
//    {
//        fbMindDataExchangeError = 0;
//        // есть ошибка связи с МИНД.
//        pxEventData = xCPlcDataBaseErrorEvent.EventDataPushRequest(ui8ErrorIndex);
//        // есть возможность зафиксировать событие?
//        if (pxEventData)
//        {
//            // установим флаг звуковой сигнализации.
//            fucZvkMal = COMMAND_SOUND_SIGNAL_TYPE_ERROR;
//            xCPlcErrorEvent.EventDataPointerPush(pxEventData);
////                xCPlcArchiveEvent.EventDataPointerPush(pxEventData);
//            // если есть возможность зафиксировать событие,
//            // то по указателю(pxEventData) на место в журнале,
//            // сохраним данные события.
//            // установим id события.
//            (pxEventData -> ui16ID) =
//                1;
//            // установим тип события.
//            (pxEventData -> ui8Type) =
//                CEvents::HANDLED_EVENTS_SYSTEM_ERROR_TYPE;
//            // установим адрес источника события.
//            (pxEventData -> ui16Address) = (xCCrc.i8Error);
//            // установим код события.
//            (pxEventData -> ui8State) = 1;
//            // установим время события.
//            (pxEventData -> xCurrentTime) =
//                xCurrentTime;
//            // создадим строку текстового реквизита события.
////            sprintf((char*)(pxEventData -> acTextDescriptor),
////                    "%s%02X",
////                    "Нет связи с МИНД",
////                    (xCCrc.i8Error));
//            strcpy((char*)(pxEventData -> acTextDescriptor),
//                   "Нет связи с МИНД");
//            xCArchiveEventsDB.DataBaseDataPush(pxEventData);
//        }
//    }
//    else
//    {
//        // очистим признаки того, что событие было зафиксировано ранее.
//        xCPlcDataBaseErrorEvent.EventReset(ui8ErrorIndex);
//    }
//    ui8ErrorIndex++;
//
////-----------------------------------------------------------------------------------------------------
//// обработаем ошибку связи с CodeSys.
//// есть ошибка связи с CodeSys?
//    if (fbCodeSysDataExchangeError)
//    {
//        fbCodeSysDataExchangeError = 0;
//        // есть ошибка связи с CodeSys.
//        pxEventData = xCPlcDataBaseErrorEvent.EventDataPushRequest(ui8ErrorIndex);
//        // есть возможность зафиксировать событие?
//        if (pxEventData)
//        {
////            // установим флаг звуковой сигнализации.
////            fucZvkMal = COMMAND_SOUND_SIGNAL_TYPE_ERROR;
//            xCPlcErrorEvent.EventDataPointerPush(pxEventData);
////                xCPlcArchiveEvent.EventDataPointerPush(pxEventData);
//            // если есть возможность зафиксировать событие,
//            // то по указателю(pxEventData) на место в журнале,
//            // сохраним данные события.
//            // установим id события.
//            (pxEventData -> ui16ID) =
//                1;
//            // установим тип события.
//            (pxEventData -> ui8Type) =
//                CEvents::HANDLED_EVENTS_SYSTEM_ERROR_TYPE;
//            // установим адрес источника события.
//            (pxEventData -> ui16Address) = (0);
//            // установим код события.
//            (pxEventData -> ui8State) = 1;
//            // установим время события.
//            (pxEventData -> xCurrentTime) =
//                xCurrentTime;
//            // создадим строку текстового реквизита события.
////            sprintf((char*)(pxEventData -> acTextDescriptor),
////                    "%s%02X",
////                    "Нет связи с МИНД",
////                    (xCCrc.i8Error));
//            strcpy((char*)(pxEventData -> acTextDescriptor),
//                   "Ошибка CodeSys");
//            xCArchiveEventsDB.DataBaseDataPush(pxEventData);
//        }
//    }
//    else
//    {
//        // очистим признаки того, что событие было зафиксировано ранее.
//        xCPlcDataBaseErrorEvent.EventReset(ui8ErrorIndex);
//    }
//    ui8ErrorIndex++;


//-----------------------------------------------------------------------------------------------------
}

//-----------------------------------------------------------------------------------------------------

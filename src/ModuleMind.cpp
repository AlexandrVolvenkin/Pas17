
//#include "AM335xPlatform.h"
#include "PasNewConfig.h"
#include "SerialMT.h"
#include "Alarm.h"
#include "Error.h"
#include "ModuleMind.h"
#include "Events.h"
#include "EventsDB.h"
#include "Task.h"

using namespace std;

//-----------------------------------------------------------------------------------------------------
// инициализируем массив с контекстом задач.
// (ПАС: W - запад, МИНД: E - восток)
CMind::TTaskContext CMind::axAllTasksContext[] =
{
    // задача-заглушка.
    // вызывается по указателю из массива с контекстом задач по адресу-индексу 0.
    // адрес: 0 - не определён протоколом(SerialMT).
    {
        0,
        0,
        0,
        CMind::NopTask
    },
    {
        0,
        0,
        0,
        CMind::PingTaskEWWE
    },
    {
        0,
        0,
        0,
        CMind::HighLevelFlagsSetWE
    },
    {
        0,
        0,
        0,
        CMind::HighLevelFlagsSetEW
    },
    {
        0,
        0,
        0,
        CMind::ConfigurationQueryEW
    },
    {
        0,
        0,
        0,
        CMind::CurrentTimeQueryEW
    },
    {
        0,
        0,
        0,
        CMind::CurrentTimeSetEW
    },
    {
        0,
        0,
        0,
        CMind::EventLogQueryEW
    },
//    {
//        0,
//        0,
//        0,
//        CMind::AlarmEventLogQueryEW
//    },
//    {
//        0,
//        0,
//        0,
////        CMind::ArchiveEventLogQueryEW
//        CMind::ArchiveEventLogQueryEWLite
//    },
    {
        0,
        0,
        0,
        CMind::ErrorLogQueryEW
    },
    {
        0,
        0,
        0,
        CMind::DiscreteOutputsLogQueryEW
    },
    {
        0,
        0,
        0,
        CMind::AnalogueMeasureQueryEW
    },
    {
        0,
        0,
        0,
        CMind::ReceiveRequest
    }
};

uint8_t CMind::ui8HighLevelFlagsGet;
uint8_t CMind::ui8HighLevelFlagsSet;
uint8_t CMind::ui8HighLevelFlagsSetSave;
uint8_t CMind::ui8HighLevelFlagsIsUpdated;
CMind::TAnalogueMeasureQueryListPackOne CMind::xAnalogueMeasureQueryList;
//CMind::TAnalogueMeasureQueryListPackOne CMind::axAnalogueMeasureQueryList[];
//CMind::TAnalogueMeasure CMind::axAnalogueMeasure[];
int16_t CMind::i16TasksQuantity;
uint8_t CMind::ui8HighLevelFlags;
int16_t CMind::i16AnalogueMeasureQueryListQuantity;
int16_t CMind::i16AnalogueMeasureQuantity;
uint8_t CMind::ui8AnalogueMeasureQueryListIsUpdated;
uint8_t CMind::ui8ConfigurationIsUpdated;
CMind::TDiscreteOutputsQueryListPackOne CMind::xDiscreteOutputsQueryList;
uint8_t CMind::ui8DiscreteOutputsLog[];
int16_t CMind::i16DiscreteOutputsQueryListQuantity;
uint8_t CMind::ui8DiscreteOutputsIsUpdated;
uint16_t CMind::ui16LastIndexQuery;
int16_t CMind::i16AllReceivedRecordsQuantity;
int16_t CMind::i16LastAllReceivedRecordsQuantity;

//-----------------------------------------------------------------------------------------------------
int16_t CMind::Init(void)
{
    // очистим флаги управления.
    ui8HighLevelFlagsSetSave = 0;
    // очистим флаг состояние флагов управления от МИНД к РС - F_HLEV обновлено.
    ui8HighLevelFlagsIsUpdated = 0;
    // обнулим количество записей ещё не отправленных в МИНД.
    i16AllReceivedRecordsQuantity = 0;
    i16LastAllReceivedRecordsQuantity = 0;
    // обнулим последний индекс события в списке запросов.
    ui16LastIndexQuery = 0;



    CError::fBadPacketQuantity = 0;
    CError::fOkPacketQuantity = 0;
}

////-----------------------------------------------------------------------------------------------------
//// производит обмен данными ПАС с МИНД.
//int16_t CMind::DataExchangeSpi(TModuleContext *pxModuleContext)
//{
//    int16_t i;
//    CSerialMT::TSerialMTData xData;
//    CMind::TTaskContext *pxTaskContext;
//
//    // получим указатель на контекст задач.
//    pxTaskContext = CMind::axAllTasksContext;
//    // получим количество функций-задач.
//    CMind::i16TasksQuantity =
//        (sizeof(CMind::axAllTasksContext) /
//         sizeof(CMind::TTaskContext));
//
////-----------------------------------------------------------------------------------------------------
//    // обмен данными по SPI.
//    vGpioClearPin(SPI_CHIP_MIND_ENABLE_PIN_PORT, SPI_CHIP_MIND_ENABLE_PIN);
//    usleep(100);
//    i = iSpiTransfer(iSpiDeviceHandle,
//                     CSpiMT::GetSpiTxBuff(),
//                     CSpiMT::GetSpiRxBuff(),
//                     CSpiMT::HEADER_LENGTH +
//                     CSpiMT::nui16TxFrameLength +
//                     CSpiMT::CRC_LENGTH
//                    );
//    vGpioSetPin(SPI_CHIP_MIND_ENABLE_PIN_PORT, SPI_CHIP_MIND_ENABLE_PIN);
////-----------------------------------------------------------------------------------------------------
//    // пришло сообщение?
//    if (i != -1)
//    {
//        // получим принятое общее сообщение.
//        // получим указатель на буфер принятого общего сообщения.
//        // получим длину принятого общего сообщения.
//        // передадим через объект данных сообщения(CSerialMT::TSerialMTData xData).
//        if (CSpiMT::DataReceive(&xData) != -1)
//        {
//            // передадим указатель на буфер принятого общего сообщения.
//            // передадим длину принятого общего сообщения.
//            // передадим в функцию через объект данных сообщения(CSerialMT::TSerialMTData xData).
//            if (CMind::RequestHandler(pxModuleContext,
//                                      CMind::axAllTasksContext,
//                                      &xData) == -1)
//            {
//                return -1;
//            }
//        }
//    }
//
//    // получим указатель на буфер передачи общего сообщения.
//    xData.pui8DataPointer = CSpiMT::GetTransmitMessageBuff();
//    // передадим в функцию через объект данных сообщения(CSerialMT::TSerialMTData xData).
//    CMind::AnswerHandler(pxModuleContext,
//                         CMind::axAllTasksContext,
//                         &xData);
//
//    // передадим размер созданного сообщение SerialMT в функию отправки данных по Modbus.
//    CSpiMT::DataTransmit(&xData);
//
//    return 0;
//}
//
////-----------------------------------------------------------------------------------------------------
//// производит обмен данными ПАС с МИНД.
//int16_t CMind::DataExchangeModbus(TModuleContext *pxModuleContext)
//{
//    int16_t i;
//    CSerialMT::TSerialMTData xData;
//    CMind::TTaskContext *pxTaskContext;
//
//    // получим указатель на контекст задач.
//    pxTaskContext = CMind::axAllTasksContext;
//    // получим количество функций-задач.
//    CMind::i16TasksQuantity =
//        (sizeof(CMind::axAllTasksContext) /
//         sizeof(CMind::TTaskContext));
////-----------------------------------------------------------------------------------------------------
//    // обмен данными по Modbus.
//    // установим адрес модуля локальной сети нижнего уровня.
//    modbus_set_slave(pxMasterTcpCtx,
//                     MODULE_MIND_MODBUS_ADDRESS);
//    // обменяемся данными с модулем локальной сети нижнего уровня во временный буфер.
//    i = modbus_data_exchange(pxMasterTcpCtx,
//                             CModbusMT::nui16TxFrameLength,
//                             CModbusMT::GetModbusTxBuff(),
//                             CModbusMT::GetModbusRxBuff());
//
////-----------------------------------------------------------------------------------------------------
//    // пришло сообщение?
//    if (i != -1)
//    {
//        CModbusMT::nui16RxFrameLength = i;
//        // получим принятое общее сообщение.
//        // получим указатель на буфер принятого общего сообщения.
//        // получим длину принятого общего сообщения.
//        // передадим через объект данных сообщения(CSerialMT::TSerialMTData xData).
//        if (CModbusMT::DataReceive(&xData) != -1)
//        {
//            // передадим указатель на буфер принятого общего сообщения.
//            // передадим длину принятого общего сообщения.
//            // передадим в функцию через объект данных сообщения(CSerialMT::TSerialMTData xData).
//            if (CMind::RequestHandler(pxModuleContext,
//                                      CMind::axAllTasksContext,
//                                      &xData) == -1)
//            {
//                return -1;
//            }
//        }
//    }
//
//    // получим указатель на буфер передачи общего сообщения.
//    xData.pui8DataPointer = CModbusMT::GetModbusTxBuff();
//    // передадим в функцию через объект данных сообщения(CSerialMT::TSerialMTData xData).
//    CMind::AnswerHandler(pxModuleContext,
//                         CMind::axAllTasksContext,
//                         &xData);
//
//    // передадим размер созданного сообщение SerialMT в функию отправки данных по Modbus.
//    CModbusMT::DataTransmit(&xData);
//
//    return 0;
//}

////-----------------------------------------------------------------------------------------------------
//// обрабатывает запросы и ответы от функций-задач на другом конце.
//int16_t CMind::RequestHandler(TModuleContext *pxModuleContext,
//                              CMind::TTaskContext *pxTaskContext,
//                              CSerialMT::TSerialMTData *pxData)
//{
//    CSerialMT SerialMT;
//    uint8_t ui8Index;
//    CSerialMT::TSerialMTData xDataSave;
//
//    // передадим объект данных сообщения(CSerialMT::TSerialMTData xData).
//    SerialMT.vPrepareGetMessageBag(pxData);
//    // извлечём первое вложенное сообщение.
//    // передадим в функцию-задачу в объекте данных сообщения(CSerialMT::TSerialMTData xData)
//    // указатель на вложенное сообщение и размер сообщения.
//    if (SerialMT.iGetDataMessageBag(pxData) == -1)
//    {
//        return -1;
//    }
//    // сохраним данные контекста принятого сообщения для функции-задачи(по адресу pxData -> ui8Address)
//    xDataSave = *pxData;
//    // извлечём вложенные сообщения для всех задачь.
//    for (ui8Index = 0;
//            (ui8Index < CMind::i16TasksQuantity);
//            ui8Index++)
//    {
//        // полученное сообщение для текущей функции-задачи(по индексу ui8Index)?
//        if ((ui8Index == (xDataSave.ui8Address) &&
//                ((xDataSave.i16Length) != 0)))
//        {
//            // восстановим данные контекста принятого сообщения для функции-задачи(по адресу pxData -> ui8Address)
//            *pxData = xDataSave;
//            // сообщим функции-задаче о том что она принимает сообщение.
//            pxData -> ui8Mode = CMind::TASK_MODE_SERVER;
//            // в функции-задаче получим в объекте данных сообщения(CSerialMT::TSerialMTData xData)
//            // указатель на вложенное сообщение и размер сообщения.
//            // вызовем функцию-задачу.
//            pxTaskContext[ui8Index].pfi16Task(pxModuleContext,
//                                              &pxTaskContext[ui8Index],
//                                              pxData);
//            // извлечём следующее вложенное сообщение.
//            // передадим в функцию-задачу в объекте данных сообщения(CSerialMT::TSerialMTData xData)
//            // указатель на вложенное сообщение и размер сообщения.
//            if (SerialMT.iGetDataMessageBag(pxData) == -1)
//            {
//                return -1;
//            }
//            // сохраним данные контекста принятого сообщения для функции-задачи(по адресу pxData -> ui8Address)
//            xDataSave = *pxData;
//        }
//        else
//        {
//            // сообщим о том, что нет сообщения.
//            pxData -> i16Length = 0;
//            // сообщим функции-задаче о том что она принимает сообщение.
//            pxData -> ui8Mode = CMind::TASK_MODE_SERVER;
//            // в функции-задаче получим в объекте данных сообщения(CSerialMT::TSerialMTData xData)
//            // указатель на вложенное сообщение и размер сообщения.
//            // вызовем функцию-задачу.
//            pxTaskContext[ui8Index].pfi16Task(pxModuleContext,
//                                              &pxTaskContext[ui8Index],
//                                              pxData);
//        }
//    }
//    return 0;
//}
//
////-----------------------------------------------------------------------------------------------------
//// обрабатывает запросы и ответы к функциям-задачам на другом конце.
//int16_t CMind::AnswerHandler(TModuleContext *pxModuleContext,
//                             CMind::TTaskContext *pxTaskContext,
//                             CSerialMT::TSerialMTData *pxData)
//{
//    CSerialMT SerialMT;
//    uint8_t ui8Index;
//    int16_t i;
//    int16_t i16Length;
//
//    // передадим в функцию объект данных сообщения(CSerialMT::TSerialMTData xData).
//    SerialMT.vPreparePutMessageBag(pxData);
//    // установим индекс первой задачи в массиве контекста задач(CMind::axAllTasksContext[]).
//    for (ui8Index = 0;
//            (ui8Index < CMind::i16TasksQuantity);
//            ui8Index++)
//    {
//        // функция-задача через объект данных сообщения(CSerialMT::TSerialMTData xData)
//        // поместит свои данные по указателю(xData.pui8DataPointer) и вернёт количество байт(xData.i16Length).
//        // вызовем функцию-задачу.
//        // сообщим функции-задаче о том что она выступает в роли клиента.
//        pxData -> ui8Mode = TASK_MODE_CLIENT;
//        if ((pxTaskContext[ui8Index].pfi16Task(pxModuleContext,
//                                               &pxTaskContext[ui8Index],
//                                               pxData)) != 0)
//        {
//            // передадим адрес задачи для заголовка формируемого в функции(SerialMT.iPutInMessageBag(&xData).
//            pxData -> ui8Address = ui8Index;
//            // поместим вложенное сообщение в общеее.
//            // больше нет места для вложенных сообщений?
//            if (SerialMT.iPutInMessageBag(pxData) == -1)
//            {
//                // больше нет места для вложенных сообщений.
//                // сбросим флаг - сообщение отправлено.
//                pxTaskContext[ui8Index].fui8MessageIsSent = 0;
//                // перейдём к обмену данными.
//                break;
//            }
//        }
//    }
//    // вернём указатель на общее сообщение.
//    pxData -> pui8DataPointer = SerialMT.ucGetBuffMessageBag();
//    // вернём длину общего сообщения.
//    pxData -> i16Length = SerialMT.iGetLengthMessageBag();
//    return 0;
//}

//-----------------------------------------------------------------------------------------------------
// получает запрос на передачу конфигкрации от МИНД(ПАС: W - запад, МИНД: E - восток)
// передаёт конфигкрацию в МИНД.
int16_t CMind::ConfigurationQueryEW(TModuleContext *pxModuleContext,
                                    CMind::TTaskContext *pxTaskContext,
                                    CSerialMT::TSerialMTData *pxData)
{
    int16_t i16Length;
    uint16_t ui16Index;

    switch (pxData -> ui8Mode)
    {
    case CMind::TASK_MODE_CLIENT:
        // функция-задача передаёт сообщение.
        // сообщение принято и обработано?
        if ((pxTaskContext -> ui8Status) == CMind::TASK_MODE_MESSAGE_IS_HANDLED)
        {
            // сбросим флаг - сообщение обработано.
            pxTaskContext -> ui8Status = 0;
            // получим указатель на место для вложенного сообщения в общем сообщении.
            // поместим Modbus адрес прибора во вложенное сообщение.
            *(pxData -> pui8DataPointer) = ucModbusSlaveAddress;
            // поместим конфигурацию прибора во вложенное сообщение.
            vConfigurationDataPack((TConfigDataPackOne*)((pxData -> pui8DataPointer) + 1),
                                   (TConfigData*)pxModuleContext ->
                                   xModuleContextStatic.
                                   pucPlcConfigDataTempPointer);
            // вычислим длину вложенного сообщения.
            // размер объекта конфигурации плюс один байт - адрес прибора.
            i16Length =
                (sizeof(struct TConfigDataPackOne) + 1);
            // обнулим счётчик относительного времени ожидания ответа адресата на сообщение.
            pxTaskContext -> nui8TimeCounter = 0;
            // установим флаг - сообщение отправлено.
            pxTaskContext -> fui8MessageIsSent = 1;
            // вернём длину сообщения.
            pxData -> i16Length = i16Length;
            // вернём длину сообщения.
            return i16Length;
        }
        break;

    case CMind::TASK_MODE_SERVER:
        // функция-задача принимает сообщение.
        // для функции-задачи есть сообщение?
        if (pxData -> i16Length)
        {
            // сообщение не обработано?
            if ((pxTaskContext -> ui8Status) != CMind::TASK_MODE_MESSAGE_IS_HANDLED)
            {
                // установим флаг - сообщение обработано.
                pxTaskContext -> ui8Status = CMind::TASK_MODE_MESSAGE_IS_HANDLED;
                // вернём длину пустого сообщения.
                pxData -> i16Length = 0;
                return 0;
            }
        }
        break;

    default:
        break;

    }
    // вернём длину пустого сообщения.
    pxData -> i16Length = 0;
    return 0;
}

//-----------------------------------------------------------------------------------------------------
//// отправляет текущее время в МИНД(ПАС: W - запад, МИНД: E - восток)
//int16_t CMind::TimeSetWE(TModuleContext *pxModuleContext,
//                         CMind::TTaskContext *pxTaskContext,
//                         CSerialMT::TSerialMTData *pxData)
//{
//    uint16_t *pui16Source;
//    int16_t i16Length;
//
//    switch (pxData -> ui8Mode)
//    {
//    case TASK_MODE_CLIENT:
//        // функция-задача в роли клиента(передаёт сообщение).
//        struct tm *pxSource;
//        uint8_t *pui8Destination;
//        // получим указатель на буфер с текущим временем в рабочем массиве прибора.
//        pxSource =
//            (struct tm*)pxModuleContext ->
//            xModuleContextStatic.
//            pucCurrentTimeBufferPointer;
//        // получим указатель на место для вложенного сообщения в общем сообщении.
//        pui8Destination = (uint8_t*)pxData -> pui8DataPointer;
//
//        // поместим текущее время во вложенное сообщение.
//        pui8Destination[CURRENT_TIME_SECOND_OFFSET] = pxSource -> tm_sec;
//        pui8Destination[CURRENT_TIME_MINUTE_OFFSET] = pxSource -> tm_min;
//        pui8Destination[CURRENT_TIME_HOUR_OFFSET] = pxSource -> tm_hour;
//        pui8Destination[CURRENT_TIME_MONTH_DAY_OFFSET] = pxSource -> tm_mday;
//        pui8Destination[CURRENT_TIME_MONTH_OFFSET] = pxSource -> tm_mon;
//        pui8Destination[CURRENT_TIME_YEAR_OFFSET] = pxSource -> tm_year;
//        pui8Destination[CURRENT_TIME_WEEK_DAY_OFFSET] = pxSource -> tm_wday;
//
//        // вычислим длину вложенного сообщения.
//        i16Length = CURRENT_TIME_BYTE_QUANTITY;
//        // вернём длину сообщения.
//        pxData -> i16Length = i16Length;
//        return i16Length;
//        break;
//
//    case TASK_MODE_SERVER:
//        // функция-задача в роли сервера(принимает сообщение).
//        // вернём длину пустого сообщения.
//        pxData -> i16Length = 0;
//        return 0;
//        break;
//
//    default:
//        break;
//    }
//
//    // вернём длину пустого сообщения.
//    pxData -> i16Length = 0;
//    return 0;
//}
// получает запрос на передачу текущего времени от МИНД(ПАС: W - запад, МИНД: E - восток)
// передаёт текущее время в МИНД.
int16_t CMind::CurrentTimeQueryEW(TModuleContext *pxModuleContext,
                                  CMind::TTaskContext *pxTaskContext,
                                  CSerialMT::TSerialMTData *pxData)
{
    int16_t i16Length;
    uint16_t ui16Index;

    switch (pxData -> ui8Mode)
    {
    case CMind::TASK_MODE_CLIENT:
        // функция-задача передаёт сообщение.
        // сообщение принято и обработано?
        if ((pxTaskContext -> ui8Status) == CMind::TASK_MODE_MESSAGE_IS_HANDLED)
        {
            // сбросим флаг - сообщение обработано.
            pxTaskContext -> ui8Status = 0;
            struct tm *pxSource;
            uint8_t *pui8Destination;
            // получим указатель на буфер с текущим временем в рабочем массиве прибора.
            pxSource =
                (struct tm*)pxModuleContext ->
                xModuleContextStatic.
                pucCurrentTimeBufferPointer;
            // получим указатель на место для вложенного сообщения в общем сообщении.
            pui8Destination = (uint8_t*)pxData -> pui8DataPointer;

            // поместим текущее время во вложенное сообщение.
            pui8Destination[CURRENT_TIME_SECOND_OFFSET] = pxSource -> tm_sec;
            pui8Destination[CURRENT_TIME_MINUTE_OFFSET] = pxSource -> tm_min;
            pui8Destination[CURRENT_TIME_HOUR_OFFSET] = pxSource -> tm_hour;
            pui8Destination[CURRENT_TIME_MONTH_DAY_OFFSET] = pxSource -> tm_mday;
            pui8Destination[CURRENT_TIME_MONTH_OFFSET] = (pxSource -> tm_mon) + 1;
            pui8Destination[CURRENT_TIME_YEAR_OFFSET] = (pxSource -> tm_year) - 100;
            pui8Destination[CURRENT_TIME_WEEK_DAY_OFFSET] = pxSource -> tm_wday;

            // вычислим длину вложенного сообщения.
            i16Length = CURRENT_TIME_BYTE_QUANTITY;
            // вернём длину сообщения.
            pxData -> i16Length = i16Length;
            return i16Length;
            break;
        }
        break;

    case CMind::TASK_MODE_SERVER:
        // функция-задача принимает сообщение.
        // для функции-задачи есть сообщение?
        if (pxData -> i16Length)
        {
            // сообщение не обработано?
            if ((pxTaskContext -> ui8Status) != CMind::TASK_MODE_MESSAGE_IS_HANDLED)
            {
                // установим флаг - сообщение обработано.
                pxTaskContext -> ui8Status = CMind::TASK_MODE_MESSAGE_IS_HANDLED;
                // вернём длину пустого сообщения.
                pxData -> i16Length = 0;
                return 0;
            }
        }
        break;

    default:
        break;

    }
    // вернём длину пустого сообщения.
    pxData -> i16Length = 0;
    return 0;
}

//-----------------------------------------------------------------------------------------------------
// принимает устанавливаемое текущее время от МИНД(ПАС: W - запад, МИНД: E - восток)
int16_t CMind::CurrentTimeSetEW(TModuleContext *pxModuleContext,
                                CMind::TTaskContext *pxTaskContext,
                                CSerialMT::TSerialMTData *pxData)
{
    int16_t i16Length;

    switch (pxData -> ui8Mode)
    {
    case CMind::TASK_MODE_CLIENT:
        // функция-задача передаёт сообщение.
        // сообщение принято и обработано?
        if ((pxTaskContext -> ui8Status) == CMind::TASK_MODE_MESSAGE_IS_HANDLED)
        {
            // функция-задача в роли клиента(передаёт сообщение).
            // сбросим флаг - сообщение обработано.
            pxTaskContext -> ui8Status = 0;
            // создадим сообщение подтверждения.
            *(pxData -> pui8DataPointer) = CMind::TASK_ANSWER_OK;
            // вернём длину сообщения подтверждения.
            pxData -> i16Length = CMind::TASK_CONFIRMATION_LENGTH;
            // вернём длину сообщения.
            return pxData -> i16Length;
        }
        break;

    case CMind::TASK_MODE_SERVER:
        // функция-задача принимает сообщение.
        // для функции-задачи есть сообщение?
        if (pxData -> i16Length)
        {
            // сообщение не обработано?
            if ((pxTaskContext -> ui8Status) != CMind::TASK_MODE_MESSAGE_IS_HANDLED)
            {
                // получим и установим текущее время.
                vLinuxCurrentTimeSet((unsigned char*)(pxData -> pui8DataPointer));
                // установим флаг - сообщение обработано.
                pxTaskContext -> ui8Status = CMind::TASK_MODE_MESSAGE_IS_HANDLED;
                // вернём длину пустого сообщения.
                pxData -> i16Length = 0;
                return 0;
            }
        }
        break;

    default:
        break;

    }
    // вернём длину пустого сообщения.
    pxData -> i16Length = 0;
    return 0;
}

//-----------------------------------------------------------------------------------------------------
// отправляет флаги верхнего уровня в МИНД(ПАС: W - запад, МИНД: E - восток)
int16_t CMind::HighLevelFlagsSetWE(TModuleContext *pxModuleContext,
                                   CMind::TTaskContext *pxTaskContext,
                                   CSerialMT::TSerialMTData *pxData)
{
//    int16_t i16Length;
//    uint16_t ui16Index;
//    uint8_t ui8HighLevelFlagsTemp;
//
//    switch (pxData -> ui8Mode)
//    {
//    case CMind::TASK_MODE_CLIENT:
//        // функция-задача передаёт сообщение.
//        // сообщение не отправлено?
//        if (pxTaskContext -> fui8MessageIsSent == 0)
//        {
//            ui8HighLevelFlagsTemp = 0;
//            // есть команда - "квитирование" c верхнего уровня?
//            if (pxModuleContext ->
//                    xModuleContextDinamic.
//                    fbGlobalModbusKvit)
//            {
//                // команду приняли.
//                pxModuleContext ->
//                xModuleContextDinamic.
//                fbGlobalModbusKvit = 0;
//                // установим флаг - квитирование от PC.
//                ui8HighLevelFlagsTemp |= (1 << CMind::HIGH_LEVEL_FLAGS_KVIT_BIT);
//            }
////            // есть квитирование от PC?
////            if (fbHighLevelKvit)
////            {
////                fbHighLevelKvit = 0;
////                // установим флаг - квитирование от PC.
////                ui8HighLevelFlagsTemp |= (1 << CMind::HIGH_LEVEL_FLAGS_KVIT_BIT);
////            }
//
//            // есть команда - "сброс" c верхнего уровня?
//            if (pxModuleContext ->
//                    xModuleContextDinamic.
//                    fbGlobalModbusReset)
//            {
//                // команду приняли.
//                pxModuleContext ->
//                xModuleContextDinamic.
//                fbGlobalModbusReset = 0;
//                // установим флаг - сброс от PC.
//                ui8HighLevelFlagsTemp |= (1 << CMind::HIGH_LEVEL_FLAGS_RESET_BIT);
//            }
////            // есть сброс от PC?
////            if (fbHighLevelReset)
////            {
////                fbHighLevelReset = 0;
////                // установим флаг - сброс от PC.
////                ui8HighLevelFlagsTemp |= (1 << CMind::HIGH_LEVEL_FLAGS_RESET_BIT);
////            }
//
//            // есть блокировка?
//            if (fbGlobalBlock)
//            {
//                // установим флаг - блокировка от PC.
//                ui8HighLevelFlagsTemp |= (1 << CMind::HIGH_LEVEL_FLAGS_BLOCK_BIT);
//            }
//            else
//            {
//                // сбросим флаг - блокировка от PC.
//                ui8HighLevelFlagsTemp &= ~(1 << CMind::HIGH_LEVEL_FLAGS_BLOCK_BIT);
//            }
//
////            // состояние флагов от PC изменилось?
////            if (ui8HighLevelFlagsTemp != CMind::ui8HighLevelFlags)
////            {
////            // сохраним новое состояние флагов от PC.
////            CMind::ui8HighLevelFlags = ui8HighLevelFlagsTemp;
//            // создадим сообщение.
//            *(pxData -> pui8DataPointer) = ui8HighLevelFlagsTemp;
//            // обнулим счётчик относительного времени ожидания ответа адресата на сообщение.
//            pxTaskContext -> nui8TimeCounter = 0;
//            // установим флаг - сообщение отправлено.
//            pxTaskContext -> fui8MessageIsSent = 1;
//            // вычислим длину вложенного сообщения.
//            i16Length = CMind::TASK_HIGH_LEVEL_FLAGS_MESSAGE_LENGTH;
//            // вернём длину сообщения.
//            pxData -> i16Length = i16Length;
//            // вернём длину сообщения.
//            return i16Length;
////            }
//        }
//        break;
//
//    case CMind::TASK_MODE_SERVER:
//        // функция-задача принимает сообщение.
//        // сообщение отправлено?
//        if (pxTaskContext -> fui8MessageIsSent == 1)
//        {
//            // функция-задача в роли сервера(принимает сообщение).
//            // для функции-задачи есть сообщение?
//            if (pxData -> i16Length)
//            {
//                // получено подтверждение приёма?
//                if (*(pxData -> pui8DataPointer) == CMind::TASK_ANSWER_OK)
//                {
//                    // получено подтверждение приёма(сообщение отправлено успешно).
////                    // сбросим флаги управления от РС квитирование и сброс.
////                    // флаг блокировка оставим.
////                    CMind::ui8HighLevelFlags &=
////                        ~((1 << CMind::HIGH_LEVEL_FLAGS_KVIT_BIT) | (1 << CMind::HIGH_LEVEL_FLAGS_RESET_BIT));
//                    // сбросим флаг - сообщение отправлено.
//                    pxTaskContext -> fui8MessageIsSent = 0;
//                }
//                // получено подтверждение приёма с ошибкой?
//                else if (*(pxData -> pui8DataPointer) == CMind::TASK_ANSWER_ERROR)
//                {
//                    // получено подтверждение приёма с ошибкой.
////                    // сбросим флаги управления от РС квитирование и сброс.
////                    // флаг блокировка оставим.
////                    CMind::ui8HighLevelFlags &=
////                        ~((1 << CMind::HIGH_LEVEL_FLAGS_KVIT_BIT) | (1 << CMind::HIGH_LEVEL_FLAGS_RESET_BIT));
//                    // сбросим флаг - сообщение отправлено.
//                    pxTaskContext -> fui8MessageIsSent = 0;
//                }
//            }
//            else
//            {
//                // превышено время ожидания ответа?
//                if ((pxTaskContext -> nui8TimeCounter) >= CMind::TASK_NO_ANSWER_TIMEOUT)
//                {
//                    // нет ответа на сообщение.
////                    // сбросим флаги управления от РС квитирование и сброс.
////                    // флаг блокировка оставим.
////                    CMind::ui8HighLevelFlags &=
////                        ~((1 << CMind::HIGH_LEVEL_FLAGS_KVIT_BIT) | (1 << CMind::HIGH_LEVEL_FLAGS_RESET_BIT));
//                    // сбросим флаг - сообщение отправлено.
//                    pxTaskContext -> fui8MessageIsSent = 0;
//                }
//                else
//                {
//                    // увеличим счётчик относительного времени ожидания ответа адресата на сообщение.
//                    (pxTaskContext -> nui8TimeCounter)++;
//                }
//            }
//        }
//        break;
//
//    default:
//        break;
//    }
//
//    // вернём длину пустого сообщения.
//    pxData -> i16Length = 0;
//    return 0;










    int16_t i16Length;

    // состояние флагов управления от МИНД к РС - F_HLEV не обновлено?
    if (!CMind::ui8HighLevelFlagsIsUpdated)
    {
        // есть команда - "квитирование" c верхнего уровня?
        if (pxModuleContext ->
                xModuleContextDinamic.
                fbGlobalModbusKvit)
        {
            // команду приняли.
            pxModuleContext ->
            xModuleContextDinamic.
            fbGlobalModbusKvit = 0;
            // установим флаг - квитирование от PC.
            CMind::ui8HighLevelFlagsSetSave |= (1 << CMind::HIGH_LEVEL_FLAGS_KVIT_BIT);
            // установим флаг состояние флагов управления от МИНД к РС - F_HLEV обновлено.
            CMind::ui8HighLevelFlagsIsUpdated = 1;
        }

        // есть команда - "сброс" c верхнего уровня?
        if (pxModuleContext ->
                xModuleContextDinamic.
                fbGlobalModbusReset)
        {
            // команду приняли.
            pxModuleContext ->
            xModuleContextDinamic.
            fbGlobalModbusReset = 0;
            // установим флаг - сброс от PC.
            CMind::ui8HighLevelFlagsSetSave |= (1 << CMind::HIGH_LEVEL_FLAGS_RESET_BIT);
            // установим флаг состояние флагов управления от МИНД к РС - F_HLEV обновлено.
            CMind::ui8HighLevelFlagsIsUpdated = 1;
        }

        // есть блокировка?
        if (fbGlobalBlock)
        {
            // состояние режима блокировки изменилось?
            if (!(CMind::ui8HighLevelFlagsSetSave &
                    (1 << CMind::HIGH_LEVEL_FLAGS_BLOCK_BIT)))
            {
                // установим флаг - блокировка от PC.
                CMind::ui8HighLevelFlagsSetSave |= (1 << CMind::HIGH_LEVEL_FLAGS_BLOCK_BIT);
                // установим флаг состояние флагов управления от МИНД к РС - F_HLEV обновлено.
                CMind::ui8HighLevelFlagsIsUpdated = 1;
            }
        }
        else
        {
            if (CMind::ui8HighLevelFlagsSetSave &
                    (1 << CMind::HIGH_LEVEL_FLAGS_BLOCK_BIT))
            {
                // сбросим флаг - блокировка от PC.
                CMind::ui8HighLevelFlagsSetSave &= ~(1 << CMind::HIGH_LEVEL_FLAGS_BLOCK_BIT);
                // установим флаг состояние флагов управления от МИНД к РС - F_HLEV обновлено.
                CMind::ui8HighLevelFlagsIsUpdated = 1;
            }
        }
    }

    // состояние флагов управления от МИНД к РС - F_HLEV обновлено?
    if (CMind::ui8HighLevelFlagsIsUpdated)
    {
        switch (pxData -> ui8Mode)
        {
        case CMind::TASK_MODE_CLIENT:
            // функция-задача передаёт сообщение.
            // сообщение не отправлено?
            if (pxTaskContext -> fui8MessageIsSent == 0)
            {
                // создадим сообщение.
                *(pxData -> pui8DataPointer) = CMind::ui8HighLevelFlagsSetSave;
                // обнулим счётчик относительного времени ожидания ответа адресата на сообщение.
                pxTaskContext -> nui8TimeCounter = 0;
                // установим флаг - сообщение отправлено.
                pxTaskContext -> fui8MessageIsSent = 1;
                // вычислим длину вложенного сообщения.
                i16Length = CMind::TASK_HIGH_LEVEL_FLAGS_MESSAGE_LENGTH;
                // вернём длину сообщения.
                pxData -> i16Length = i16Length;
                // вернём длину сообщения.
                return i16Length;
            }
            break;

        case CMind::TASK_MODE_SERVER:
            // функция-задача принимает сообщение.
            // сообщение отправлено?
            if (pxTaskContext -> fui8MessageIsSent == 1)
            {
                // функция-задача в роли сервера(принимает сообщение).
                // для функции-задачи есть сообщение?
                if (pxData -> i16Length)
                {
                    // получено подтверждение приёма?
                    if (*(pxData -> pui8DataPointer) == CMind::TASK_ANSWER_OK)
                    {
                        // получено подтверждение приёма(сообщение отправлено успешно).
                        // сбросим флаги управления квитирование и сброс.
                        // флаг блокировка оставим.
                        CMind::ui8HighLevelFlagsSetSave &=
                            ~((1 << CMind::HIGH_LEVEL_FLAGS_KVIT_BIT) | (1 << CMind::HIGH_LEVEL_FLAGS_RESET_BIT));
                        // очистим флаг состояние флагов управления от МИНД к РС - F_HLEV обновлено.
                        // ожидаем новое событие.
                        CMind::ui8HighLevelFlagsIsUpdated = 0;
                        // сбросим флаг - сообщение отправлено.
                        pxTaskContext -> fui8MessageIsSent = 0;
                    }
                    // получено подтверждение приёма с ошибкой?
                    else if (*(pxData -> pui8DataPointer) == CMind::TASK_ANSWER_ERROR)
                    {
                        // получено подтверждение приёма с ошибкой.
                        // сбросим флаг - сообщение отправлено.
                        pxTaskContext -> fui8MessageIsSent = 0;
                    }
                }
                else
                {
                    // превышено время ожидания ответа?
                    if ((pxTaskContext -> nui8TimeCounter) >= CMind::TASK_NO_ANSWER_TIMEOUT)
                    {
                        // нет ответа на сообщение.
                        // сбросим флаг - сообщение отправлено.
                        pxTaskContext -> fui8MessageIsSent = 0;
                    }
                    else
                    {
                        // увеличим счётчик относительного времени ожидания ответа адресата на сообщение.
                        (pxTaskContext -> nui8TimeCounter)++;
                    }
                }
            }
            break;

        default:
            break;
        }
    }

    // вернём длину пустого сообщения.
    pxData -> i16Length = 0;
    return 0;
}

//-----------------------------------------------------------------------------------------------------
// принимает флаги верхнего уровня от МИНД(ПАС: W - запад, МИНД: E - восток)
int16_t CMind::HighLevelFlagsSetEW(TModuleContext *pxModuleContext,
                                   CMind::TTaskContext *pxTaskContext,
                                   CSerialMT::TSerialMTData *pxData)
{
    int16_t i16Length;
    uint16_t ui16Index;
    uint8_t ui8HighLevelFlagsTemp;

    switch (pxData -> ui8Mode)
    {
    case CMind::TASK_MODE_CLIENT:
        // функция-задача передаёт сообщение.
        // сообщение принято и обработано?
        if ((pxTaskContext -> ui8Status) == CMind::TASK_MODE_MESSAGE_IS_HANDLED)
        {
            // функция-задача в роли клиента(передаёт сообщение).
            // сбросим флаг - сообщение обработано.
            pxTaskContext -> ui8Status = 0;
            // создадим сообщение подтверждения.
            *(pxData -> pui8DataPointer) = CMind::TASK_ANSWER_OK;
            // вернём длину сообщения подтверждения.
            pxData -> i16Length = CMind::TASK_CONFIRMATION_LENGTH;
            // вернём длину сообщения.
            return pxData -> i16Length;
        }
        break;

    case CMind::TASK_MODE_SERVER:
        // функция-задача принимает сообщение.
        // для функции-задачи есть сообщение?
        if (pxData -> i16Length)
        {
            // сообщение не обработано?
            if ((pxTaskContext -> ui8Status) != CMind::TASK_MODE_MESSAGE_IS_HANDLED)
            {
                TModuleContext *pxModuleContextOrigin;
                // получим указатель на начало в массиве с объектами контекста всех модулей.
                pxModuleContextOrigin = (pxModuleContext - (pxModuleContext ->
                                         xModuleContextStatic.
                                         ucModuleContextIndex));
                // получим состояние флагов от МИНД.
                ui8HighLevelFlagsTemp = *(pxData -> pui8DataPointer);
                // есть квитирование от МИНД?
                if (ui8HighLevelFlagsTemp & (1 << CMind::HIGH_LEVEL_FLAGS_KVIT_BIT))
                {
                    // установим флаг - квитирование от МИНД.
                    fbGlobalKvit = 1;
                    // установим флаг квитирования всем модулям.
                    for (int i = 0; i < MAX_MODULE_QUANTITY; i++)
                    {
                        pxModuleContextOrigin[i].
                        xModuleContextDinamic.
                        fbGlobalButtonKvit = 1;
                    }
                }
                // есть сброс от МИНД?
                if (ui8HighLevelFlagsTemp & (1 << CMind::HIGH_LEVEL_FLAGS_RESET_BIT))
                {
                    // установим флаг - сброс от МИНД.
                    fbGlobalReset = 1;
                    // установим флаг сброса всем модулям.
                    for (int i = 0; i < MAX_MODULE_QUANTITY; i++)
                    {
                        pxModuleContextOrigin[i].
                        xModuleContextDinamic.
                        fbGlobalButtonReset = 1;
                    }
                }
                // есть блокировка от МИНД?
                if (ui8HighLevelFlagsTemp & (1 << CMind::HIGH_LEVEL_FLAGS_BLOCK_BIT))
                {
                    // установим флаг - блокировка от МИНД.
                    fbGlobalBlock = 1;
                    // установим флаг блокировки всем модулям.
                    for (int i = 0; i < MAX_MODULE_QUANTITY; i++)
                    {
                        pxModuleContextOrigin[i].
                        xModuleContextDinamic.
                        fbGlobalButtonBlock = 1;
                    }
                }
                else
                {
                    // сбросим флаг блокировки.
                    fbGlobalBlock = 0;
                    // сбросим флаг блокировки всем модулям.
                    for (int i = 0; i < MAX_MODULE_QUANTITY; i++)
                    {
                        pxModuleContextOrigin[i].
                        xModuleContextDinamic.
                        fbGlobalButtonBlock = 0;
                    }
                }

                // установим флаг - сообщение обработано.
                pxTaskContext -> ui8Status = CMind::TASK_MODE_MESSAGE_IS_HANDLED;
                // вернём длину пустого сообщения.
                pxData -> i16Length = 0;
                return 0;
            }
        }
        break;

    default:
        break;

    }
    // вернём длину пустого сообщения.
    pxData -> i16Length = 0;
    return 0;
}

//-----------------------------------------------------------------------------------------------------
// получает запросы на передачу журнала событий от МИНД(ПАС: W - запад, МИНД: E - восток)
// передаёт журнал событий в МИНД.
int16_t CMind::EventLogQueryEW(TModuleContext *pxModuleContext,
                               CMind::TTaskContext *pxTaskContext,
                               CSerialMT::TSerialMTData *pxData)
{
    int16_t i16Length;
    uint16_t ui16Index;

    switch (pxData -> ui8Mode)
    {
    case CMind::TASK_MODE_CLIENT:
        // функция-задача передаёт сообщение.
        // сообщение принято и обработано?
        if ((pxTaskContext -> ui8Status) == CMind::TASK_MODE_MESSAGE_IS_HANDLED)
        {
            CEvents::TEventDataCommon *pxSource;
            CEvents::TEventDataPackOne *pxDestination;

            // сбросим флаг - сообщение обработано.
            pxTaskContext -> ui8Status = 0;
//            // создадим сообщение подтверждения.
//            *(pxData -> pui8DataPointer) = CMind::TASK_ANSWER_OK;
            // получим указатель на место вложенного сообщения в общем сообщении.
            pxDestination = (CEvents::TEventDataPackOne*)(pxData -> pui8DataPointer);
            // получим количество требуемых событий в запросе.
            i16Length = CEvents::xEventsLogQueryList.ui8Quantity;
            // количество полученных событий превосходит
            // размер массива?
            if (i16Length >
                    (CEvents::REQUESTED_EVENTS_QUANTITY))
            {
                // ошибка.
                break;
            }
//            CEvents::TEventDataCommon *pxSource;
//            CEvents::TEventDataPackOne *pxDestination;
            CEvents *pxCEvents;
            // индекс запрашиваемого события.
            int16_t i16IndexQuery;
pxCEvents = &xCAlarmEvent;
//            switch (CEvents::xEventsLogQueryList.ui8Type)
//            {
//            case CEvents::REQUESTED_EVENTS_ALARM_TYPE:
//                pxCEvents = &xCAlarmEvent;
//                break;
//            case CEvents::REQUESTED_EVENTS_ERROR_TYPE:
//                pxCEvents = &xCPlcErrorEvent;
//                break;
//            case CEvents::REQUESTED_EVENTS_ARCHIVE_TYPE:
//                pxCEvents = &xCPlcArchiveEvent;
//                break;
//            default:
//                break;
//            }

            // получим индекс первого запрашиваемого события.
            i16IndexQuery = CEvents::xEventsLogQueryList.i16Index - 2;
            // установим индекс в массиве указателей на события в массиве даных событий.
            pxCEvents -> EventDataPopIndexSet(i16IndexQuery + CONVERT_INTEGER_TO_NATURAL_NUMBER);
            // поместим данные событий во вложенное сообщение.
            for (uint16_t i = 0;
                    i < i16Length;
                    i++)
            {
                // получим указатель на данные события.
//                pxSource = xCAlarmEvent.EventDataPop();
                pxSource = pxCEvents -> EventDataPop();
                if (!pxSource)
                {
                    // ошибка.
                    // получим тип события.
                    pxDestination[i].ui8Type = 0;
                    // индекс запрашиваемой ошибки.
                    // для идентификации при поиске ошибки при выводе в МИНД.
                    pxDestination[i].i16Index =
                        (i16IndexQuery);
                    printf("ArchiveEventLogQueryEW error pxDestination[i].i16Index %d\n\r", pxDestination[i].i16Index);
                    // пометим события маркером - нет события.
                    pxDestination[i].ui16ID = 0;
                }
                else
                {
                    // есть маркер - нет события?
                    if ((pxSource -> ui16ID) == 0)
                    {
                        // получим тип события.
                        pxDestination[i].ui8Type =
                            pxSource -> ui8Type;
                        // индекс запрашиваемой ошибки.
                        // для идентификации при поиске ошибки при выводе в МИНД.
                        pxDestination[i].i16Index =
                            (i16IndexQuery);
                        // пометим события маркером - нет события.
                        pxDestination[i].ui16ID = 0;
                        printf("ArchiveEventLogQueryEW no event pxDestination[i].i16Index %d\n\r", pxDestination[i].i16Index);
                    }
                    else
                    {
                        // получим id события.
                        pxDestination[i].ui16ID =
                            pxSource -> ui16ID;
                        // получим тип события.
                        pxDestination[i].ui8Type =
                            pxSource -> ui8Type;
                        // индекс запрашиваемого события.
                        // для идентификации при поиске события при выводе в МИНД.
                        pxDestination[i].i16Index =
                            (i16IndexQuery);
                        printf("ArchiveEventLogQueryEW event pxDestination[i].i16Index %d\n\r", pxDestination[i].i16Index);
                        // получим адрес события.
                        pxDestination[i].ui16Address =
                            pxSource -> ui16Address;
                        // получим состояние события.
                        pxDestination[i].ui8State =
                            pxSource -> ui8State;
                        // отказ текущего модуля не при старте?
                        if ((pxSource -> xCurrentTime.tm_sec) != 0xFF)
                        {
                            // поместим время события.
                            pxDestination[i].ui8Second = (uint8_t)pxSource -> xCurrentTime.tm_sec;
                            pxDestination[i].ui8Minute = (uint8_t)pxSource -> xCurrentTime.tm_min;
                            pxDestination[i].ui8Hour = (uint8_t)pxSource -> xCurrentTime.tm_hour;
                            pxDestination[i].ui8MonthDay = (uint8_t)pxSource -> xCurrentTime.tm_mday;
                            pxDestination[i].ui8Month = (uint8_t)(pxSource -> xCurrentTime.tm_mon) + 1;
                            pxDestination[i].ui8Year = (uint8_t)(pxSource -> xCurrentTime.tm_year) - 100;
                        }
                        else
                        {
                            // установим код времени события - при старте.
                            pxDestination[i].ui8Second = 0xFF;
                        }
                        // скопируем строку текстового реквизита.
                        memcpy(&(pxDestination[i].acTextDescriptor),
                               (&(pxSource -> acTextDescriptor)),
                               (CEvents::EVENT_NAME_LENGTH + END_OF_STRING_LENGTH));
                    }
                }
                // индекс следующего запрашиваемого события.
                i16IndexQuery++;
            }




            // вычислим длину вложенного сообщения.
            // плюс длина сообщения подтверждения.
            i16Length =
                (CEvents::i16EventsLogQueryListQuantity *
                  sizeof(struct CEvents::TEventDataPackOne));
            // вернём длину сообщения.
            pxData -> i16Length = i16Length;
            // вернём длину сообщения.
            return i16Length;
        }
        break;

    case CMind::TASK_MODE_SERVER:
        // функция-задача принимает сообщение.
        // для функции-задачи есть сообщение?
        if (pxData -> i16Length)
        {
            // сообщение не обработано?
            if ((pxTaskContext -> ui8Status) != CMind::TASK_MODE_MESSAGE_IS_HANDLED)
            {



                CEvents::TEventsLogQueryListPackOne *pxSource;
                CEvents::TEventsLogQueryListPackOne *pxDestination;

                // получим указатель на место вложенного сообщения в общем сообщении.
                pxSource = (CEvents::TEventsLogQueryListPackOne*)pxData -> pui8DataPointer;
                // получим указатель на рабочий массив со списком запросов событий.
                pxDestination =
                    (CEvents::TEventsLogQueryListPackOne*)&CEvents::xEventsLogQueryList;
                // вычислим количество требуемых событий в запросе.
                // сохраним количество требуемых событий в запросе.
                CEvents::i16EventsLogQueryListQuantity = pxSource -> ui8Quantity;
                // поместим принятый список запросов событий в рабочий массив.
                *pxDestination = *pxSource;

                // установим флаг - сообщение обработано.
                pxTaskContext -> ui8Status = CMind::TASK_MODE_MESSAGE_IS_HANDLED;
                // вернём длину пустого сообщения.
                pxData -> i16Length = 0;
                return 0;
            }
        }
        break;

    default:
        break;

    }
    // вернём длину пустого сообщения.
    pxData -> i16Length = 0;
    return 0;
}

////-----------------------------------------------------------------------------------------------------
//// получает запросы на передачу журнала событий от МИНД(ПАС: W - запад, МИНД: E - восток)
//// передаёт журнал событий в МИНД.
//int16_t CMind::AlarmEventLogQueryEW(TModuleContext *pxModuleContext,
//                                    CMind::TTaskContext *pxTaskContext,
//                                    CSerialMT::TSerialMTData *pxData)
//{
//    int16_t i16Length;
//    uint16_t ui16Index;
//    // индекс запрашиваемого события.
//    int16_t i16IndexQuery;
//
//    switch (pxData -> ui8Mode)
//    {
//    case CMind::TASK_MODE_CLIENT:
//        // функция-задача передаёт сообщение.
//        // сообщение принято и обработано?
//        if ((pxTaskContext -> ui8Status) == CMind::TASK_MODE_MESSAGE_IS_HANDLED)
//        {
//            CEvents::TEventDataCommon *pxSource;
//            CEvents::TEventDataPackOne *pxDestination;
//
//            // сбросим флаг - сообщение обработано.
//            pxTaskContext -> ui8Status = 0;
//            // создадим сообщение подтверждения.
//            *(pxData -> pui8DataPointer) = CMind::TASK_ANSWER_OK;
//            // получим указатель на место вложенного сообщения в общем сообщении.
//            pxDestination = (CEvents::TEventDataPackOne*)((pxData -> pui8DataPointer) + 1);
//            // получим количество требуемых событий в запросе.
//            i16Length = CEvents::xEventsLogQueryList.ui8Quantity;
//            // количество полученных событий превосходит
//            // размер массива?
//            if (i16Length >
//                    (CEvents::REQUESTED_EVENTS_QUANTITY))
//            {
//                // ошибка.
//                break;
//            }
//
//            // получим индекс первого запрашиваемого события.
//            i16IndexQuery = CEvents::xEventsLogQueryList.i16Index;
//            // установим индекс в массиве указателей на события в массиве даных событий.
//            xCAlarmEvent.EventDataPopIndexSet(i16IndexQuery + CONVERT_INTEGER_TO_NATURAL_NUMBER);
//            // поместим данные событий во вложенное сообщение.
//            for (uint16_t i = 0;
//                    i < i16Length;
//                    i++)
//            {
//                // получим указатель на данные события.
//                pxSource = xCAlarmEvent.EventDataPop();
//                if (!pxSource)
//                {
//                    // ошибка.
//                    // получим тип события.
//                    pxDestination[i].ui8Type =
//                        pxSource -> ui8Type;
//                    // индекс запрашиваемой ошибки.
//                    // для идентификации при поиске ошибки при выводе в МИНД.
//                    pxDestination[i].i16Index =
//                        (i16IndexQuery);
//                    printf("ArchiveEventLogQueryEW error pxDestination[i].i16Index %d\n\r", pxDestination[i].i16Index);
//                    // пометим события маркером - нет события.
//                    pxDestination[i].ui16ID = 0;
//                }
//                else
//                {
//                    // есть маркер - нет события?
//                    if ((pxSource -> ui16ID) == 0)
//                    {
//                        // получим тип события.
//                        pxDestination[i].ui8Type =
//                            pxSource -> ui8Type;
//                        // индекс запрашиваемой ошибки.
//                        // для идентификации при поиске ошибки при выводе в МИНД.
//                        pxDestination[i].i16Index =
//                            (i16IndexQuery);
//                        // пометим события маркером - нет события.
//                        pxDestination[i].ui16ID = 0;
//                        printf("ArchiveEventLogQueryEW no event pxDestination[i].i16Index %d\n\r", pxDestination[i].i16Index);
//                    }
//                    else
//                    {
//                        // получим id события.
//                        pxDestination[i].ui16ID =
//                            pxSource -> ui16ID;
//                        if (i == 0)
//                        {
//                            CMind::i16LastID = pxSource -> ui16ID;
//                        }
//                        // получим тип события.
//                        pxDestination[i].ui8Type =
//                            pxSource -> ui8Type;
//                        // индекс запрашиваемого события.
//                        // для идентификации при поиске события при выводе в МИНД.
//                        pxDestination[i].i16Index =
//                            (i16IndexQuery);
//                        printf("ArchiveEventLogQueryEW event pxDestination[i].i16Index %d\n\r", pxDestination[i].i16Index);
//                        // получим адрес события.
//                        pxDestination[i].ui16Address =
//                            pxSource -> ui16Address;
//                        // получим состояние события.
//                        pxDestination[i].ui8State =
//                            pxSource -> ui8State;
//                        // отказ текущего модуля не при старте?
//                        if ((pxSource -> xCurrentTime.tm_sec) != 0xFF)
//                        {
//                            // поместим время события.
//                            pxDestination[i].ui8Second = (uint8_t)pxSource -> xCurrentTime.tm_sec;
//                            pxDestination[i].ui8Minute = (uint8_t)pxSource -> xCurrentTime.tm_min;
//                            pxDestination[i].ui8Hour = (uint8_t)pxSource -> xCurrentTime.tm_hour;
//                            pxDestination[i].ui8MonthDay = (uint8_t)pxSource -> xCurrentTime.tm_mday;
//                            pxDestination[i].ui8Month = (uint8_t)(pxSource -> xCurrentTime.tm_mon);
//                            pxDestination[i].ui8Year = (uint8_t)(pxSource -> xCurrentTime.tm_year);
//                        }
//                        else
//                        {
//                            // установим код времени события - при старте.
//                            pxDestination[i].ui8Second = 0xFF;
//                        }
//                        // скопируем строку текстового реквизита.
//                        memcpy(&(pxDestination[i].acTextDescriptor),
//                               (&(pxSource -> acTextDescriptor)),
//                               (CEvents::EVENT_NAME_LENGTH + END_OF_STRING_LENGTH));
//                    }
//                }
//                // индекс следующего запрашиваемого события.
//                i16IndexQuery++;
//            }
//
//            // вычислим длину вложенного сообщения.
//            // плюс длина сообщения подтверждения.
//            i16Length =
//                ((CEvents::i16EventsLogQueryListQuantity *
//                  sizeof(struct CEvents::TEventDataPackOne)) +
//                 CMind::TASK_CONFIRMATION_LENGTH);
//            // вернём длину сообщения.
//            pxData -> i16Length = i16Length;
//            // вернём длину сообщения.
//            return i16Length;
//
//
//
//
////            CEvents::TEventDataCommon *pxSource;
////            CAlarm::TAlarmEventPackOne *pxDestination;
////
////            // сбросим флаг - сообщение обработано.
////            pxTaskContext -> ui8Status = 0;
////
////            // получим указатель на место вложенного сообщения в общем сообщении.
////            pxDestination = (CAlarm::TAlarmEventPackOne*)(pxData -> pui8DataPointer);
////            // получим количество требуемых событий в запросе.
////            i16Length = CAlarm::i16AlarmEventQueryListQuantity;
////            // количество полученных событий превосходит
////            // размер массива axAlarmEventsLog?
////            if (i16Length >
////                    (xCAlarmEvent.ui16RecordedEventsQuantity))
////            {
////                // ошибка.
////                break;
////            }
////
////            // получим индекс первого запрашиваемого события.
////            i16IndexQuery = CAlarm::xAlarmEventsQueryList.ui16Index;
////            // установим индекс в массиве указателей на события в массиве даных событий.
////            xCAlarmEvent.EventDataPopIndexSet(i16IndexQuery + CONVERT_INTEGER_TO_NATURAL_NUMBER);
////            // поместим данные событий во вложенное сообщение.
////            for (uint16_t i = 0;
////                    i < i16Length;
////                    i++)
////            {
////                // получим указатель на данные события.
////                pxSource = xCAlarmEvent.EventDataPop();
////                if (!pxSource)
////                {
////                    // ошибка.
////                    // индекс запрашиваемой ошибки.
////                    // для идентификации при поиске ошибки при выводе в МИНД.
////                    pxDestination[i].ui16Index =
////                        i16IndexQuery;
////                    // пометим события маркером - нет события.
////                    pxDestination[i].ui16ID = 0;
////                }
////                else
////                {
////                    // есть маркер - нет события?
////                    if ((pxSource -> ui16ID) == 0)
////                    {
////                        // индекс запрашиваемой ошибки.
////                        // для идентификации при поиске ошибки при выводе в МИНД.
////                        pxDestination[i].ui16Index =
////                            i16IndexQuery;
////                        // пометим события маркером - нет события.
////                        pxDestination[i].ui16ID = 0;
////                    }
////                    else
////                    {
////                        // установим id события.
////                        pxDestination[i].ui16ID =
////                            1;
////                        // индекс запрашиваемого события.
////                        // для идентификации при поиске события при выводе в МИНД.
////                        pxDestination[i].ui16Index =
////                            i16IndexQuery;
////                        // получим адрес события.
////                        pxDestination[i].ui16Address =
////                            pxSource -> ui16Address;
////                        // получим состояние события.
////                        pxDestination[i].ui8State =
////                            pxSource -> ui8State;
////                        // скопируем строку текстового реквизита.
////                        memcpy(&(pxDestination[i].acTextDescriptor),
////                               (&(pxSource -> acTextDescriptor)),
////                               (CAlarm::EVENT_NAME_LENGTH + END_OF_STRING_LENGTH));
////                        // поместим время события.
////                        pxDestination[i].ui8Second = (uint8_t)pxSource -> xCurrentTime.tm_sec;
////                        pxDestination[i].ui8Minute = (uint8_t)pxSource -> xCurrentTime.tm_min;
////                        pxDestination[i].ui8Hour = (uint8_t)pxSource -> xCurrentTime.tm_hour;
////                        pxDestination[i].ui8MonthDay = (uint8_t)pxSource -> xCurrentTime.tm_mday;
////                        pxDestination[i].ui8Month = (uint8_t)(pxSource -> xCurrentTime.tm_mon) + 1;
////                        pxDestination[i].ui8Year = (uint8_t)(pxSource -> xCurrentTime.tm_year) - 100;
////
////
////// cout << "AlarmEventLogQueryEW pxDestination" << endl;
//////        unsigned char *pucSourceTemp;
//////        pucSourceTemp = (unsigned char*)pxDestination;
//////        for(int i=0; i<64; )
//////        {
//////            for(int j=0; j<8; j++)
//////            {
//////                cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
//////            }
//////            cout << endl;
//////            i += 8;
//////        }
////
////                    }
////                }
////                // индекс следующего запрашиваемого события.
////                i16IndexQuery++;
////            }
////
////            // вычислим длину вложенного сообщения.
////            i16Length =
////                (CAlarm::i16AlarmEventQueryListQuantity *
////                 sizeof(struct CAlarm::TAlarmEventPackOne));
////            // вернём длину сообщения.
////            pxData -> i16Length = i16Length;
////            // вернём длину сообщения.
////            return i16Length;
//        }
//        break;
//
//    case CMind::TASK_MODE_SERVER:
//        // функция-задача принимает сообщение.
//        // для функции-задачи есть сообщение?
//        if (pxData -> i16Length)
//        {
//            // сообщение не обработано?
//            if ((pxTaskContext -> ui8Status) != CMind::TASK_MODE_MESSAGE_IS_HANDLED)
//            {
//                CEvents::TEventsLogQueryListPackOne *pxSource;
//                CEvents::TEventsLogQueryListPackOne *pxDestination;
//
//                // получим указатель на место вложенного сообщения в общем сообщении.
//                pxSource = (CEvents::TEventsLogQueryListPackOne*)pxData -> pui8DataPointer;
//                // получим указатель на рабочий массив со списком запросов событий.
//                pxDestination =
//                    (CEvents::TEventsLogQueryListPackOne*)&CEvents::xEventsLogQueryList;
//                // вычислим количество требуемых событий в запросе.
//                // сохраним количество требуемых событий в запросе.
//                CEvents::i16EventsLogQueryListQuantity = pxSource -> ui8Quantity;
//                // поместим принятый список запросов событий в рабочий массив.
//                *pxDestination = *pxSource;
//                // установим флаг - сообщение обработано.
//                pxTaskContext -> ui8Status = CMind::TASK_MODE_MESSAGE_IS_HANDLED;
//                // вернём длину пустого сообщения.
//                pxData -> i16Length = 0;
//                return 0;
//            }
//
//
////            // сообщение не обработано?
////            if ((pxTaskContext -> ui8Status) != CMind::TASK_MODE_MESSAGE_IS_HANDLED)
////            {
////                CAlarm::TAlarmEventLogQueryListPackOne *pxSource;
////                CAlarm::TAlarmEventLogQueryListPackOne *pxDestination;
////
////                // получим указатель на место вложенного сообщения в общем сообщении.
////                pxSource = (CAlarm::TAlarmEventLogQueryListPackOne*)pxData -> pui8DataPointer;
////                // получим указатель на рабочий массив со списком запросов событий.
////                pxDestination =
////                    (CAlarm::TAlarmEventLogQueryListPackOne*)&CAlarm::xAlarmEventsQueryList;
////                // вычислим количество требуемых событий в запросе.
////                // сохраним количество требуемых событий в запросе.
////                CAlarm::i16AlarmEventQueryListQuantity = pxSource -> ui8Quantity;
////                // поместим принятый список запросов событий в рабочий массив.
////                *pxDestination = *pxSource;
////                // установим флаг - сообщение обработано.
////                pxTaskContext -> ui8Status = CMind::TASK_MODE_MESSAGE_IS_HANDLED;
////                // вернём длину пустого сообщения.
////                pxData -> i16Length = 0;
////                return 0;
//            //        }
//        }
//        break;
//
//    default:
//        break;
//
//    }
//// вернём длину пустого сообщения.
//    pxData -> i16Length = 0;
//    return 0;
//}
//
////-----------------------------------------------------------------------------------------------------
//// получает запросы на передачу журнала событий от МИНД(ПАС: W - запад, МИНД: E - восток)
//// передаёт журнал событий в МИНД.
//int16_t CMind::ArchiveEventLogQueryEW(TModuleContext *pxModuleContext,
//                                      CMind::TTaskContext *pxTaskContext,
//                                      CSerialMT::TSerialMTData *pxData)
//{
//    int16_t i16Length;
//    uint16_t ui16Index;
//    // индекс запрашиваемого события.
//    int16_t i16IndexQuery;
//
//    switch (pxData -> ui8Mode)
//    {
//    case CMind::TASK_MODE_CLIENT:
//        // функция-задача передаёт сообщение.
//        // сообщение принято и обработано?
//        if ((pxTaskContext -> ui8Status) == CMind::TASK_MODE_MESSAGE_IS_HANDLED)
//        {
//            CEvents::TEventDataCommon *pxSource;
//            CEvents::TEventDataPackOne *pxDestination;
//
//            // сбросим флаг - сообщение обработано.
//            pxTaskContext -> ui8Status = 0;
//            // создадим сообщение подтверждения.
//            *(pxData -> pui8DataPointer) = CMind::TASK_ANSWER_OK;
//            // получим указатель на место вложенного сообщения в общем сообщении.
//            pxDestination = (CEvents::TEventDataPackOne*)((pxData -> pui8DataPointer) + 1);
//            // получим количество требуемых событий в запросе.
//            i16Length = CEvents::xEventsLogQueryList.ui8Quantity;
//            // количество полученных событий превосходит
//            // размер массива?
//            if (i16Length >
//                    (CEvents::REQUESTED_EVENTS_QUANTITY))
//            {
//                // ошибка.
//                break;
//            }
//
//            // получим индекс первого запрашиваемого события.
//            i16IndexQuery = CEvents::xEventsLogQueryList.i16Index;
//            printf("ArchiveEventLogQueryEW i16IndexQuery %d\n\r", (i16IndexQuery));
//            // в МИНД отправлены все записи, полученные от СУБД при последнем запросе?
//            // количество записей, полученных от СУБД - страница по которой скользит
//            // окно, размером - количество запрашиваемых МИНД событий(CEvents::REQUESTED_EVENTS_QUANTITY).
////            if ((((i16AllReceivedRecordsQuantity) -
////                    ((i16IndexQuery + CONVERT_INTEGER_TO_NATURAL_NUMBER) +
////                     CEventsDB::EVENTS_DB_PREVIOUS_INDEX_GET +
////                     CEventsDB::EVENTS_DB_CURRENT_INDEX_GET +
////                     CEventsDB::EVENTS_DB_NEXT_INDEX_GET)) <= 0) ||
////                    (i16IndexQuery == 0))
//
////            ((i16IndexQuery - CMind::ui16LastIndexQuery) <=
////             (CEventsDB::EVENTS_DB_PREVIOUS_RECORDS_QUANTITY))
//
////            if (((i16LastAllReceivedRecordsQuantity) <
////                    (((i16IndexQuery - CMind::ui16LastIndexQuery) + CONVERT_INTEGER_TO_NATURAL_NUMBER) +
////                     CEventsDB::EVENTS_DB_NEXT_RECORDS_QUANTITY)) ||
////                    (i16IndexQuery == 0) ||
////                    ((i16IndexQuery - CMind::ui16LastIndexQuery) <=
////                     (CEventsDB::EVENTS_DB_PREVIOUS_RECORDS_QUANTITY)))
//
//            printf("ArchiveEventLogQueryEW (i16IndexQuery - CMind::ui16LastIndexQuery) %d\n\r", (i16IndexQuery - CMind::ui16LastIndexQuery));
//            if (((i16LastAllReceivedRecordsQuantity) <
//                    (((i16IndexQuery - CMind::ui16LastIndexQuery) + CONVERT_INTEGER_TO_NATURAL_NUMBER) +
//                     CEventsDB::EVENTS_DB_PREVIOUS_RECORDS_QUANTITY +
//                     CEventsDB::EVENTS_DB_NEXT_RECORDS_QUANTITY)) ||
//                    (i16IndexQuery == 0))
//            {
//                struct timeval  xTimeCur;
//                struct timeval xTimeLast;
//                gettimeofday( &xTimeLast, NULL );
//
//                // очистим буфер событий, чтобы иметь возможность сообщить МИНД,
//                // что события закончились.
//                xCPlcArchiveEvent.NoEventsFlagSet();
//                // извлечём записи из базы данных в буфер.
//                xCArchiveEventsDB.DataBaseDataGet();
//                if (i16IndexQuery == 0)
//                {
//                    // получим количество записей, полученных от СУБД.
//                    i16AllReceivedRecordsQuantity = 0;
//                    i16LastAllReceivedRecordsQuantity = xCArchiveEventsDB.LastQueryRecordsQuantityGet();
//                }
//                else
//                {
//                    // получим количество записей, полученных от СУБД.
//                    i16LastAllReceivedRecordsQuantity = xCArchiveEventsDB.LastQueryRecordsQuantityGet();
//                }
////                // получим количество записей, полученных от СУБД.
////                i16LastAllReceivedRecordsQuantity = xCArchiveEventsDB.LastQueryRecordsQuantityGet();
//                i16AllReceivedRecordsQuantity += i16LastAllReceivedRecordsQuantity;
//                printf("ArchiveEventLogQueryEW i16AllReceivedRecordsQuantity %d\n\r", (i16AllReceivedRecordsQuantity));
//                printf("ArchiveEventLogQueryEW i16LastAllReceivedRecordsQuantity %d\n\r", (i16LastAllReceivedRecordsQuantity));
//                // сохраним индекс при котором обновляли данные от СУБД.
//                CMind::ui16LastIndexQuery = i16IndexQuery;
//
//
//                if( gettimeofday( &xTimeCur, NULL ) != 0 )
//                {
//                    /* gettimeofday failed - retry next time. */
//                }
//                else
//                {
////                    (((xTimeCur.tv_sec - xTimeLast.tv_sec) *
////                            1000000) +
////                            (xTimeCur.tv_usec - xTimeLast.tv_usec)) >
////                            ctx->modbus_timeout.tv_usec)))
////                    printf("ArchiveEventLogQueryEW gap %d\n\r", (xTimeCur.tv_usec - xTimeLast.tv_usec));
//                }
//            }
//            else
//            {
//
//            }
//
////            // индекс события в запросе обновлён?
////            if ((i16IndexQuery - CMind::ui16LastIndexQuery) <= 0)
////            {
////
////            }
//
////            // установим индекс в массиве указателей на события в массиве даных событий.
////            xCPlcArchiveEvent.EventDataPopIndexSet(CEvents::xEventsLogQueryList.ui8Quantity);
//
//            // между отправкой запроса от МИНД с требуемыми событиями
//            // и получением ответа с данными, проходит время - (MAIN_CYCLE_TIME).
//            // эта задержка плюс время обработки клавиатуры - причина относительно
//            // долгого ожидания вывода на дисплей запрошенных событий.
//            // чтобы этого избежать, за один раз запрашивается 4 события.
//            // предыдущее, два требуемых и следующее.
//            // в нормальном режиме на дисплей выводится требуемое событие.
//            // если нажата кнопка вперёд - на дисплей выводится следующее событие,
//            // если нажата кнопка назад - на дисплей выводится предыдущее событие,
//            // из предыдущего ответа на запрос.
//            // ((i16AllReceivedRecordsQuantity -
//            // (i16IndexQuery -
//            // CMind::ui16LastIndexQuery)) + 1) - индекс на предыдущее событие.
//            // установим индекс в массиве указателей на события в массиве даных событий.
////            i16IndexQuery -= 2;
//
////            xCPlcArchiveEvent.EventDataPopIndexSet(
////                (i16AllReceivedRecordsQuantity -
////                 (i16IndexQuery -
////                  CMind::ui16LastIndexQuery)) + 2);
////            printf("ArchiveEventLogQueryEW diff %d\n\r", (i16AllReceivedRecordsQuantity -
////                    (i16IndexQuery - CMind::ui16LastIndexQuery)));
//
//
////            xCPlcArchiveEvent.EventDataPopIndexSet(
////                (((i16LastAllReceivedRecordsQuantity) +
////                  CEventsDB::EVENTS_DB_PREVIOUS_RECORDS_QUANTITY) -
////                 ((i16IndexQuery - CMind::ui16LastIndexQuery) +
////                  CONVERT_INTEGER_TO_NATURAL_NUMBER)));
//////            xCPlcArchiveEvent.EventDataPopIndexSet(i16AllReceivedRecordsQuantity +
//////                                                   CEventsDB::EVENTS_DB_PREVIOUS_INDEX_GET);
////            printf("ArchiveEventLogQueryEW pop index %d\n\r", (((i16LastAllReceivedRecordsQuantity) +
////                    CEventsDB::EVENTS_DB_PREVIOUS_RECORDS_QUANTITY) -
////                    ((i16IndexQuery - CMind::ui16LastIndexQuery) +
////                     CONVERT_INTEGER_TO_NATURAL_NUMBER)));
//
//
//
//            xCPlcArchiveEvent.EventDataPopIndexSet(i16LastAllReceivedRecordsQuantity);
//            for (uint16_t i = (i16LastAllReceivedRecordsQuantity);
//                    i != 0;
//                    i--)
//            {
//                printf("ArchiveEventLogQueryEW search no i %d\n\r", (i));
//                printf("ArchiveEventLogQueryEW search no i16IndexQuery %d\n\r", (i16IndexQuery));
//                // получим указатель на данные события извлечённые из базы данных.
//                pxSource = xCPlcArchiveEvent.EventDataPop();
//                // индекс требуемого события?
//                if ((pxSource -> i16Index) == i16IndexQuery)
//                {
//                    printf("ArchiveEventLogQueryEW search i %d\n\r", (i));
//                    break;
//                }
//            }
//
//
//
//
//            // поместим данные событий во вложенное сообщение.
//            for (uint16_t i = 0;
//                    i < i16Length;
//                    i++)
//            {
////                // получим указатель на данные события извлечённые из базы данных.
////                pxSource = xCPlcArchiveEvent.EventDataPop();
//                if (!pxSource)
//                {
//                    // ошибка.
//                    // получим тип события.
//                    pxDestination[i].ui8Type =
//                        pxSource -> ui8Type;
//                    // индекс запрашиваемой ошибки.
//                    // для идентификации при поиске ошибки при выводе в МИНД.
//                    pxDestination[i].i16Index =
//                        (i16IndexQuery - CEventsDB::EVENTS_DB_PREVIOUS_RECORDS_QUANTITY);//(pxSource -> i16Index);//
//                    printf("ArchiveEventLogQueryEW error pxDestination[i].i16Index %d\n\r", pxDestination[i].i16Index);
//                    // пометим события маркером - нет события.
//                    pxDestination[i].ui16ID = 0;
//                }
//                else
//                {
//                    // есть маркер - нет события?
//                    if ((pxSource -> ui16ID) == 0)
//                    {
//                        // получим тип события.
//                        pxDestination[i].ui8Type =
//                            pxSource -> ui8Type;
//                        // индекс запрашиваемой ошибки.
//                        // для идентификации при поиске ошибки при выводе в МИНД.
////                        pxDestination[i].ui16Index =
////                            (i16IndexQuery - 1);
//                        pxDestination[i].i16Index =
//                            (i16IndexQuery - CEventsDB::EVENTS_DB_PREVIOUS_RECORDS_QUANTITY);//(pxSource -> i16Index);//
//                        // пометим события маркером - нет события.
//                        pxDestination[i].ui16ID = 0;
//                        printf("ArchiveEventLogQueryEW no event pxDestination[i].i16Index %d\n\r", pxDestination[i].i16Index);
//                    }
//                    else
//                    {
//                        // получим id события.
//                        pxDestination[i].ui16ID =
//                            pxSource -> ui16ID;
//                        // получим тип события.
//                        pxDestination[i].ui8Type =
//                            pxSource -> ui8Type;
//                        // индекс запрашиваемого события.
//                        // для идентификации при поиске события при выводе в МИНД.
//                        pxDestination[i].i16Index =
//                            (i16IndexQuery - CEventsDB::EVENTS_DB_PREVIOUS_RECORDS_QUANTITY);//(pxSource -> i16Index);//
//                        printf("ArchiveEventLogQueryEW event pxDestination[i].i16Index %d\n\r", pxDestination[i].i16Index);
//                        // получим адрес события.
//                        pxDestination[i].ui16Address =
//                            pxSource -> ui16Address;
//                        // получим состояние события.
//                        pxDestination[i].ui8State =
//                            pxSource -> ui8State;
//                        // отказ текущего модуля не при старте?
//                        if ((pxSource -> xCurrentTime.tm_sec) != 0xFF)
//                        {
//                            // поместим время события.
//                            pxDestination[i].ui8Second = (uint8_t)pxSource -> xCurrentTime.tm_sec;
//                            pxDestination[i].ui8Minute = (uint8_t)pxSource -> xCurrentTime.tm_min;
//                            pxDestination[i].ui8Hour = (uint8_t)pxSource -> xCurrentTime.tm_hour;
//                            pxDestination[i].ui8MonthDay = (uint8_t)pxSource -> xCurrentTime.tm_mday;
//                            pxDestination[i].ui8Month = (uint8_t)(pxSource -> xCurrentTime.tm_mon);
//                            pxDestination[i].ui8Year = (uint8_t)(pxSource -> xCurrentTime.tm_year);
//                        }
//                        else
//                        {
//                            // установим код времени события - при старте.
//                            pxDestination[i].ui8Second = 0xFF;
//                        }
//                        // скопируем строку текстового реквизита.
//                        memcpy(&(pxDestination[i].acTextDescriptor),
//                               (&(pxSource -> acTextDescriptor)),
//                               (CEvents::EVENT_NAME_LENGTH + END_OF_STRING_LENGTH));
//                    }
//                }
//                // индекс следующего запрашиваемого события.
//                i16IndexQuery++;
//                pxSource++;
//            }
//
//            // вычислим длину вложенного сообщения.
//            // плюс длина сообщения подтверждения.
//            i16Length =
//                ((CEvents::i16EventsLogQueryListQuantity *
//                  sizeof(struct CEvents::TEventDataPackOne)) +
//                 CMind::TASK_CONFIRMATION_LENGTH);
//            // вернём длину сообщения.
//            pxData -> i16Length = i16Length;
//            // вернём длину сообщения.
//            return i16Length;
//        }
//        break;
//
//    case CMind::TASK_MODE_SERVER:
//        // функция-задача принимает сообщение.
//        // для функции-задачи есть сообщение?
//        if (pxData -> i16Length)
//        {
//            // сообщение не обработано?
//            if ((pxTaskContext -> ui8Status) != CMind::TASK_MODE_MESSAGE_IS_HANDLED)
//            {
//                CEvents::TEventsLogQueryListPackOne *pxSource;
//                CEvents::TEventsLogQueryListPackOne *pxDestination;
//
//                // получим указатель на место вложенного сообщения в общем сообщении.
//                pxSource = (CEvents::TEventsLogQueryListPackOne*)pxData -> pui8DataPointer;
//                // получим указатель на рабочий массив со списком запросов событий.
//                pxDestination =
//                    (CEvents::TEventsLogQueryListPackOne*)&CEvents::xEventsLogQueryList;
//                // вычислим количество требуемых событий в запросе.
//                // сохраним количество требуемых событий в запросе.
//                CEvents::i16EventsLogQueryListQuantity = pxSource -> ui8Quantity;
//                // поместим принятый список запросов событий в рабочий массив.
//                *pxDestination = *pxSource;
//                // установим флаг - сообщение обработано.
//                pxTaskContext -> ui8Status = CMind::TASK_MODE_MESSAGE_IS_HANDLED;
//                // вернём длину пустого сообщения.
//                pxData -> i16Length = 0;
//                return 0;
//            }
//        }
//        break;
//
//    default:
//        break;
//
//    }
//    // вернём длину пустого сообщения.
//    pxData -> i16Length = 0;
//    return 0;
//}
//
////-----------------------------------------------------------------------------------------------------
//// получает запросы на передачу журнала событий от МИНД(ПАС: W - запад, МИНД: E - восток)
//// передаёт журнал событий в МИНД.
//int16_t CMind::ArchiveEventLogQueryEWLite(TModuleContext *pxModuleContext,
//        CMind::TTaskContext *pxTaskContext,
//        CSerialMT::TSerialMTData *pxData)
//{
//    int16_t i16Length;
//    uint16_t ui16Index;
//    // индекс запрашиваемого события.
//    int16_t i16IndexQuery;
//
//    switch (pxData -> ui8Mode)
//    {
//    case CMind::TASK_MODE_CLIENT:
//        // функция-задача передаёт сообщение.
//        // сообщение принято и обработано?
//        if ((pxTaskContext -> ui8Status) == CMind::TASK_MODE_MESSAGE_IS_HANDLED)
//        {
//            CEvents::TEventDataCommon *pxSource;
//            CEvents::TEventDataPackOne *pxDestination;
//
//            // сбросим флаг - сообщение обработано.
//            pxTaskContext -> ui8Status = 0;
//            // создадим сообщение подтверждения.
//            *(pxData -> pui8DataPointer) = CMind::TASK_ANSWER_OK;
//            // получим указатель на место вложенного сообщения в общем сообщении.
//            pxDestination = (CEvents::TEventDataPackOne*)((pxData -> pui8DataPointer) + 1);
//            // получим количество требуемых событий в запросе.
//            i16Length = CEvents::xEventsLogQueryList.ui8Quantity;
//            // количество полученных событий превосходит
//            // размер массива?
//            if (i16Length >
//                    (CEvents::REQUESTED_EVENTS_QUANTITY))
//            {
//                // ошибка.
//                break;
//            }
//
//            // получим индекс первого запрашиваемого события.
//            i16IndexQuery = CEvents::xEventsLogQueryList.i16Index - 2;
//            printf("ArchiveEventLogQueryEW i16IndexQuery %d\n\r", (i16IndexQuery));
//
////            if (CEvents::xEventsLogQueryList.i16Index = 0)
////            {
////                //                // очистим буфер событий, чтобы иметь возможность сообщить МИНД,
//////                // что события закончились.
//////                xCPlcArchiveEvent.NoEventsFlagSet();
////                CMind::i16LastID = 0;
////            }
//
//            // очистим буфер событий, чтобы иметь возможность сообщить МИНД,
//            // что события закончились.
//            xCPlcArchiveEvent.NoEventsFlagSet();
//            // извлечём записи из базы данных в буфер.
////            xCArchiveEventsDB.DataBaseDataGet();
//            xCArchiveEventsDB.DataBaseDataGet();
//
//
//            xCPlcArchiveEvent.EventDataPopIndexSet(xCArchiveEventsDB.LastQueryRecordsQuantityGet());
//
//            // поместим данные событий во вложенное сообщение.
//            for (uint16_t i = 0;
//                    i < i16Length;
//                    i++)
//            {
//                // получим указатель на данные события извлечённые из базы данных.
//                pxSource = xCPlcArchiveEvent.EventDataPop();
//                if (!pxSource)
//                {
//                    // ошибка.
//                    // получим тип события.
//                    pxDestination[i].ui8Type =
//                        pxSource -> ui8Type;
//                    // индекс запрашиваемой ошибки.
//                    // для идентификации при поиске ошибки при выводе в МИНД.
//                    pxDestination[i].i16Index =
//                        (i16IndexQuery);
//                    printf("ArchiveEventLogQueryEW error pxDestination[i].i16Index %d\n\r", pxDestination[i].i16Index);
//                    // пометим события маркером - нет события.
//                    pxDestination[i].ui16ID = 0;
//                }
//                else
//                {
//                    // есть маркер - нет события?
//                    if ((pxSource -> ui16ID) == 0)
//                    {
//                        // получим тип события.
//                        pxDestination[i].ui8Type =
//                            pxSource -> ui8Type;
//                        // индекс запрашиваемой ошибки.
//                        // для идентификации при поиске ошибки при выводе в МИНД.
//                        pxDestination[i].i16Index =
//                            (i16IndexQuery);
//                        // пометим события маркером - нет события.
//                        pxDestination[i].ui16ID = 0;
//                        printf("ArchiveEventLogQueryEW no event pxDestination[i].i16Index %d\n\r", pxDestination[i].i16Index);
//                    }
//                    else
//                    {
//                        // получим id события.
//                        pxDestination[i].ui16ID =
//                            pxSource -> ui16ID;
//                        if (i == 0)
//                        {
//                            CMind::i16LastID = pxSource -> ui16ID;
//                        }
//                        // получим тип события.
//                        pxDestination[i].ui8Type =
//                            pxSource -> ui8Type;
//                        // индекс запрашиваемого события.
//                        // для идентификации при поиске события при выводе в МИНД.
//                        pxDestination[i].i16Index =
//                            (i16IndexQuery);
//                        printf("ArchiveEventLogQueryEW event pxDestination[i].i16Index %d\n\r", pxDestination[i].i16Index);
//                        // получим адрес события.
//                        pxDestination[i].ui16Address =
//                            pxSource -> ui16Address;
//                        // получим состояние события.
//                        pxDestination[i].ui8State =
//                            pxSource -> ui8State;
//                        // отказ текущего модуля не при старте?
//                        if ((pxSource -> xCurrentTime.tm_sec) != 0xFF)
//                        {
//                            // поместим время события.
//                            pxDestination[i].ui8Second = (uint8_t)pxSource -> xCurrentTime.tm_sec;
//                            pxDestination[i].ui8Minute = (uint8_t)pxSource -> xCurrentTime.tm_min;
//                            pxDestination[i].ui8Hour = (uint8_t)pxSource -> xCurrentTime.tm_hour;
//                            pxDestination[i].ui8MonthDay = (uint8_t)pxSource -> xCurrentTime.tm_mday;
//                            pxDestination[i].ui8Month = (uint8_t)(pxSource -> xCurrentTime.tm_mon);
//                            pxDestination[i].ui8Year = (uint8_t)(pxSource -> xCurrentTime.tm_year);
//                        }
//                        else
//                        {
//                            // установим код времени события - при старте.
//                            pxDestination[i].ui8Second = 0xFF;
//                        }
//                        // скопируем строку текстового реквизита.
//                        memcpy(&(pxDestination[i].acTextDescriptor),
//                               (&(pxSource -> acTextDescriptor)),
//                               (CEvents::EVENT_NAME_LENGTH + END_OF_STRING_LENGTH));
//                    }
//                }
//                // индекс следующего запрашиваемого события.
//                i16IndexQuery++;
//            }
//
//            // вычислим длину вложенного сообщения.
//            // плюс длина сообщения подтверждения.
//            i16Length =
//                ((CEvents::i16EventsLogQueryListQuantity *
//                  sizeof(struct CEvents::TEventDataPackOne)) +
//                 CMind::TASK_CONFIRMATION_LENGTH);
//            // вернём длину сообщения.
//            pxData -> i16Length = i16Length;
//            // вернём длину сообщения.
//            return i16Length;
//        }
//        break;
//
//    case CMind::TASK_MODE_SERVER:
//        // функция-задача принимает сообщение.
//        // для функции-задачи есть сообщение?
//        if (pxData -> i16Length)
//        {
//            // сообщение не обработано?
//            if ((pxTaskContext -> ui8Status) != CMind::TASK_MODE_MESSAGE_IS_HANDLED)
//            {
//                CEvents::TEventsLogQueryListPackOne *pxSource;
//                CEvents::TEventsLogQueryListPackOne *pxDestination;
//
//                // получим указатель на место вложенного сообщения в общем сообщении.
//                pxSource = (CEvents::TEventsLogQueryListPackOne*)pxData -> pui8DataPointer;
//                // получим указатель на рабочий массив со списком запросов событий.
//                pxDestination =
//                    (CEvents::TEventsLogQueryListPackOne*)&CEvents::xEventsLogQueryList;
//                // вычислим количество требуемых событий в запросе.
//                // сохраним количество требуемых событий в запросе.
//                CEvents::i16EventsLogQueryListQuantity = pxSource -> ui8Quantity;
//                // поместим принятый список запросов событий в рабочий массив.
//                *pxDestination = *pxSource;
//                // установим флаг - сообщение обработано.
//                pxTaskContext -> ui8Status = CMind::TASK_MODE_MESSAGE_IS_HANDLED;
//                // вернём длину пустого сообщения.
//                pxData -> i16Length = 0;
//                return 0;
//            }
//        }
//        break;
//
//    default:
//        break;
//
//    }
//// вернём длину пустого сообщения.
//    pxData -> i16Length = 0;
//    return 0;
//}

//-----------------------------------------------------------------------------------------------------
// получает запросы на передачу журнала ошибок от МИНД(ПАС: W - запад, МИНД: E - восток)
// передаёт журнал ошибок в МИНД.
int16_t CMind::ErrorLogQueryEW(TModuleContext *pxModuleContext,
                               CMind::TTaskContext *pxTaskContext,
                               CSerialMT::TSerialMTData *pxData)
{
    int16_t i16Length;
    uint16_t ui16Index;
    // индекс запрашиваемой ошибки.
    uint16_t i16IndexQuery;

    switch (pxData -> ui8Mode)
    {
    case CMind::TASK_MODE_CLIENT:
        // функция-задача передаёт сообщение.
        // сообщение принято и обработано?
        if ((pxTaskContext -> ui8Status) == CMind::TASK_MODE_MESSAGE_IS_HANDLED)
        {
            CEvents::TEventDataCommon *pxSource;
            CError::TErrorPackOne *pxDestination;

            // сбросим флаг - сообщение обработано.
            pxTaskContext -> ui8Status = 0;

            // получим указатель на место вложенного сообщения в общем сообщении.
            pxDestination = (CError::TErrorPackOne*)(pxData -> pui8DataPointer);
            // получим количество требуемых ошибок в запросе.
            i16Length = CError::i16ErrorQueryListQuantity;
            // количество полученных ошибок превосходит
            // размер массива axErrorLog?
            if (i16Length >
                    (xCPlcErrorEvent.ui16RecordedEventsQuantity))
            {
                // ошибка.
                break;
            }

            // получим индекс первой запрашиваемой ошибки.
            i16IndexQuery = CError::xErrorQueryList.ui16Index;
            // установим индекс в массиве указателей на события в массиве даных событий.
            xCPlcErrorEvent.EventDataPointerPopIndexSet(i16IndexQuery + CONVERT_INTEGER_TO_NATURAL_NUMBER);

            // поместим данные ошибок во вложенное сообщение.
            for (uint16_t i = 0;
                    i < i16Length;
                    i++)
            {
                // получим указатель на данные события.
                pxSource = xCPlcErrorEvent.EventDataPointerPop();
                if (!pxSource)
                {
                    // ошибка.
                    // индекс запрашиваемой ошибки.
                    // для идентификации при поиске ошибки при выводе в МИНД.
                    pxDestination[i].ui16Index =
                        i16IndexQuery;
                    // пометим события маркером - нет события.
                    pxDestination[i].ui16ID = 0;
                }
                else
                {
                    // есть маркер - нет события?
                    if ((pxSource -> ui16ID) == 0)
                    {
                        // индекс запрашиваемой ошибки.
                        // для идентификации при поиске ошибки при выводе в МИНД.
                        pxDestination[i].ui16Index =
                            i16IndexQuery;
                        // пометим события маркером - нет события.
                        pxDestination[i].ui16ID = 0;
                    }
                    else
                    {
                        // индекс запрашиваемой ошибки.
                        // для идентификации при поиске ошибки при выводе в МИНД.
                        pxDestination[i].ui16Index =
                            i16IndexQuery;
//                        // адрес источника ошибки.
//                        pxDestination[i].ui16Address =
//                        pxSource -> ui16Address;
//                        // состояние ошибки.
//                        pxDestination[i].ui8State =
//                        pxSource -> ui8State;
                        // скопируем строку текстового реквизита.
                        memcpy(&(pxDestination[i].acTextDescriptor),
                               (&(pxSource -> acTextDescriptor)),
                               (CError::ERROR_NAME_LENGTH + END_OF_STRING_LENGTH));
                        // ошибка конфигурации?
                        if ((pxSource -> ui8Type) ==
                                CEvents::HANDLED_EVENTS_CONFIGURATION_ERROR_TYPE)
                        {
                            // скопируем строку данных ошибки.
                            memcpy(&(pxDestination[i].acTextData),
                                   (&(pxSource -> acTextData)),
                                   (CError::ERROR_NAME_LENGTH + END_OF_STRING_LENGTH));
                        }
                        else
                        {
                            // отказ текущего модуля не при старте?
                            if ((pxSource -> xCurrentTime.tm_sec) != 0xFF)
                            {
                                // создадим строку данных ошибки.
                                // установим время ошибки.
                                sprintf((char*)(pxDestination[i].acTextData),
                                        " %s%02d%s%02d%s%02d%s%02d%s%02d%s%02d",
                                        "",
                                        (pxSource -> xCurrentTime.tm_mday),
                                        ".",
                                        ((pxSource -> xCurrentTime.tm_mon) + 1),
                                        ".",
                                        ((pxSource -> xCurrentTime.tm_year) - 100),
                                        "|",
                                        (pxSource -> xCurrentTime.tm_hour),
                                        ":",
                                        (pxSource -> xCurrentTime.tm_min),
                                        ":",
                                        (pxSource -> xCurrentTime.tm_sec));

// sprintf((char*)(pxDestination[i].acTextData),
//                                        " %s%02d%s%02d%s%02d%s%02d%s%02d%s%02d",
//                                        "",
//                                        (pxSource -> xCurrentTime.tm_mday),
//                                        ".",
//                                        ((pxSource -> xCurrentTime.tm_mon)),
//                                        ".",
//                                        ((pxSource -> xCurrentTime.tm_year)),
//                                        "|",
//                                        (pxSource -> xCurrentTime.tm_hour),
//                                        ":",
//                                        (pxSource -> xCurrentTime.tm_min),
//                                        ":",
//                                        (pxSource -> xCurrentTime.tm_sec));
                            }
                            else
                            {
                                sprintf((char*)(pxDestination[i].acTextData),
                                        " %s",
                                        "Ошибка при запуске ");
                            }
                        }
                    }
                }
                // индекс следующей запрашиваемой ошибки.
                i16IndexQuery++;
            }

            // вычислим длину вложенного сообщения.
            i16Length =
                (CError::i16ErrorQueryListQuantity *
                 sizeof(struct CError::TErrorPackOne));
            // вернём длину сообщения.
            pxData -> i16Length = i16Length;
            // вернём длину сообщения.
            return i16Length;
        }
        break;

    case CMind::TASK_MODE_SERVER:
        // функция-задача принимает сообщение.
        // для функции-задачи есть сообщение?
        if (pxData -> i16Length)
        {
            // сообщение не обработано?
            if ((pxTaskContext -> ui8Status) != CMind::TASK_MODE_MESSAGE_IS_HANDLED)
            {
                CError::TErrorLogQueryListPackOne *pxSource;
                CError::TErrorLogQueryListPackOne *pxDestination;

                // получим указатель на место вложенного сообщения в общем сообщении.
                pxSource = (CError::TErrorLogQueryListPackOne*)pxData -> pui8DataPointer;
                // получим указатель на рабочий массив со списком запросов ошибок.
                pxDestination =
                    (CError::TErrorLogQueryListPackOne*)&CError::xErrorQueryList;
                // вычислим количество требуемых ошибок в запросе.
                // сохраним количество требуемых ошибок в запросе.
                CError::i16ErrorQueryListQuantity = pxSource -> ui8Quantity;
                // поместим принятый список запросов ошибок в рабочий массив.
                *pxDestination = *pxSource;
                // установим флаг - сообщение обработано.
                pxTaskContext -> ui8Status = CMind::TASK_MODE_MESSAGE_IS_HANDLED;
                // вернём длину пустого сообщения.
                pxData -> i16Length = 0;
                return 0;
            }
        }
        break;

    default:
        break;

    }
    // вернём длину пустого сообщения.
    pxData -> i16Length = 0;
    return 0;
}

//-----------------------------------------------------------------------------------------------------
// получает запросы на передачу журнала состояния дискретных выходов от МИНД(ПАС: W - запад, МИНД: E - восток)
// передаёт журнал состояния дискретных выходов в МИНД.
int16_t CMind::DiscreteOutputsLogQueryEW(TModuleContext *pxModuleContext,
        CMind::TTaskContext *pxTaskContext,
        CSerialMT::TSerialMTData *pxData)
{
    int16_t i16Length;
    uint16_t ui16Index;
    // индекс запрашиваемого байта(модуля) состояния дискретных выходов.
    int16_t i16IndexQuery;

    switch (pxData -> ui8Mode)
    {
    case CMind::TASK_MODE_CLIENT:
        // функция-задача передаёт сообщение.
        // сообщение принято и обработано?
        if ((pxTaskContext -> ui8Status) == CMind::TASK_MODE_MESSAGE_IS_HANDLED)
        {
            uint8_t *pui8Source;
            CMind::TDiscreteOutputsPackOne *pxDestination;

            // сбросим флаг - сообщение обработано.
            pxTaskContext -> ui8Status = 0;
            // получим указатель на массив состояния дискретных выходов.
            pui8Source =
                (uint8_t*)pxModuleContext ->
                xModuleContextStatic.
                pucSostDoOriginBufferPointer;
            // получим указатель на место вложенного сообщения в общем сообщении.
            pxDestination = (CMind::TDiscreteOutputsPackOne*)(pxData -> pui8DataPointer);
            // получим количество требуемых байт(модулей) состояния дискретных выходов в запросе.
            i16Length = CMind::i16DiscreteOutputsQueryListQuantity;
            // количество полученных байт(модулей) состояния дискретных выходов превосходит
            // длину списка запросов состояния дискретных выходов?
            if (i16Length >
                    CMind::DISCRETE_INPUT_QUERY_LIST_LENGTH)
            {
                // ошибка.
                break;
            }

            // получим индекс первого запрашиваемого байта(модуля) состояния дискретных выходов.
            i16IndexQuery = CMind::xDiscreteOutputsQueryList.ui16Index;
            // поместим данные состояния дискретных выходов во вложенное сообщение.
            for (uint16_t i = 0;
                    i < i16Length;
                    i++)
            {
                // индекс запрашиваемого байта(модуля) состояния дискретных выходов
                // выходит за пределы массива состояния дискретных выходов?
                if (i16IndexQuery >=
                        (MAX_HANDLED_DISCRETE_OUTPUT /
                         MRXX_DISCRETE_OUTPUT_QUANTITY))
                {

                    // ошибка.
                    break;
                }

                // индекс запрашиваемого байта(модуля) состояния дискретных выходов.
                // для идентификации при поиске байта(модуля) состояния дискретных выходов при выводе в МИНД.
                pxDestination[i].ui16Index =
                    i16IndexQuery;

                // получим состояние дискретных выходов.
                uint8_t ui8Data;
                ui8Data = 0;
                // в массиве состояния дискретных выходов, состояние одного выхода занимает один байт.
                // в МИНД состояние всех дискретных выходов одного модуля передаём одним байтом.
                // проверим состояние всех дискретных выходов одного модуля.
                for (uint8_t j = 0;
                        j < MRXX_DISCRETE_OUTPUT_QUANTITY;
                        j++)
                {
                    // дискретный выход активен?
                    if (pui8Source[(i16IndexQuery * MRXX_DISCRETE_OUTPUT_QUANTITY) + j])
                    {
                        ui8Data |= (1 << j);
                    }
                }
                // состояние дискретных выходов.
                pxDestination[i].ui8State =
                    ui8Data;
                // индекс следующего запрашиваемого байта(модуля) состояния дискретных выходов.
                i16IndexQuery++;
            }
            // вычислим длину вложенного сообщения.
            i16Length =
                (CMind::i16DiscreteOutputsQueryListQuantity *
                 sizeof(struct CMind::TDiscreteOutputsPackOne));
            // вернём длину сообщения.
            pxData -> i16Length = i16Length;
            // вернём длину сообщения.
            return i16Length;
        }
        break;

    case CMind::TASK_MODE_SERVER:
        // функция-задача принимает сообщение.
        // для функции-задачи есть сообщение?
        if (pxData -> i16Length)
        {
            // сообщение не обработано?
            if ((pxTaskContext -> ui8Status) != CMind::TASK_MODE_MESSAGE_IS_HANDLED)
            {
                CMind::TDiscreteOutputsQueryListPackOne *pxSource;
                CMind::TDiscreteOutputsQueryListPackOne *pxDestination;

                // получим указатель на место вложенного сообщения в общем сообщении.
                pxSource = (CMind::TDiscreteOutputsQueryListPackOne*)pxData -> pui8DataPointer;
                // получим указатель на рабочий массив со списком запросов состояния дискретных выходов.
                pxDestination =
                    (CMind::TDiscreteOutputsQueryListPackOne*)&CMind::xDiscreteOutputsQueryList;
                // вычислим количество требуемых байт(модулей) состояния дискретных выходов в запросе.
                // сохраним количество требуемых байт(модулей) состояния дискретных выходов в запросе.
                CMind::i16DiscreteOutputsQueryListQuantity = pxSource -> ui8Quantity;
                // поместим принятый список запросов состояния дискретных выходов в рабочий массив.
                *pxDestination = *pxSource;
                // установим флаг - сообщение обработано.
                pxTaskContext -> ui8Status = CMind::TASK_MODE_MESSAGE_IS_HANDLED;
                // вернём длину пустого сообщения.
                pxData -> i16Length = 0;
                return 0;
            }
        }
        break;

    default:
        break;

    }
    // вернём длину пустого сообщения.
    pxData -> i16Length = 0;
    return 0;
}

//-----------------------------------------------------------------------------------------------------
// получает запросы на передачу измеренных аналоговых значений от МИНД(ПАС: W - запад, МИНД: E - восток)
// передаёт измеренные аналоговые значения в МИНД.
int16_t CMind::AnalogueMeasureQueryEW(TModuleContext *pxModuleContext,
                                      CMind::TTaskContext *pxTaskContext,
                                      CSerialMT::TSerialMTData *pxData)
{
    int16_t i16Length;
    uint16_t ui16Index;

    switch (pxData -> ui8Mode)
    {
    case CMind::TASK_MODE_CLIENT:
        // функция-задача передаёт сообщение.
        // сообщение принято и обработано?
        if ((pxTaskContext -> ui8Status) == CMind::TASK_MODE_MESSAGE_IS_HANDLED)
        {
            float *pfSource;
            TAnalogueInputDescriptionWork *pxSource;
            TDimentionParameter *pxDimentionParameter;
            uint8_t *pui8StatAiByteArray;
            CMind::TAnalogueMeasurePackOne *pxDestination;

            // сбросим флаг - сообщение обработано.
            pxTaskContext -> ui8Status = 0;
            // получим указатель на рабочий массив с аналоговыми измерениями.
            pfSource = (float*)pxModuleContext ->
                       xModuleContextStatic.
                       pucAiValueOriginBufferPointer;
            // получим указатель на рабочий массив с нормализованной базой данных обработки аналоговых сигналов.
            pxSource = (TAnalogueInputDescriptionWork*)pxModuleContext ->
                       xModuleContextStatic.
                       pucAnalogueInputDescriptionWork;
            // получим указатель на рабочий массив с нормализованной базой данных размерностей.
            pxDimentionParameter = (TDimentionParameter*)pxModuleContext ->
                                   xModuleContextStatic.
                                   pucDimentionsParametersDataBase;

            // получим указатель на рабочий массив - состояние аналоговых измерительных каналов для взаимодействия с модулем.
            pui8StatAiByteArray = pxModuleContext ->
                                  xModuleContextStatic.
                                  pucStatAiBufferPointer;
            // получим указатель на место вложенного сообщения в общем сообщении.
            pxDestination = (CMind::TAnalogueMeasurePackOne*)(pxData -> pui8DataPointer);
            // получим количество требуемых аналоговых измерений в запросе.
            i16Length = CMind::i16AnalogueMeasureQueryListQuantity;
            // количество полученных аналоговых измерений превосходит
            // размер массива axAnalogueMeasure?
            if (i16Length >
                    CMind::ANALOGUE_QUERY_MEASURE_LIST_LENGTH)
            {
                cout << "i16Length > CMind::ANALOGUE_QUERY_MEASURE_LIST_LENGTH" << endl;
                break;
            }
            // поместим данные аналоговых измерений во вложенное сообщение.
            for (ui16Index = 0;
                    ui16Index < i16Length;
                    ui16Index++)
            {
                // по требуемому адресу есть аналоговое измерение?
                if ((CMind::xAnalogueMeasureQueryList.ui8Address +
                        CMind::i16AnalogueMeasureQueryListQuantity) <
                        MAX_HANDLED_ANALOGUE_INPUT)
                {
                    uint8_t ui8Address;
                    uint8_t ui8Status;

                    ui8Address = (CMind::xAnalogueMeasureQueryList.ui8Address + ui16Index);
                    // получим адрес аналогового измерения.
                    pxDestination[ui16Index].ui8Address =
                        ui8Address;

                    // получим состояние аналогового измерения.
                    ui8Status =
                        pui8StatAiByteArray[ui8Address];

                    if( ui8Status & ANALOGUE_INPUT_LINE_BREAK )
                    {
                        pxDestination[ui16Index].ui8Status =
                            HMI_ANALOGUE_INPUT_LINE_BREAK;
                    }
                    else if( ui8Status & ANALOGUE_INPUT_CALCULATION_OVERFLOW )
                    {
                        pxDestination[ui16Index].ui8Status =
                            HMI_ANALOGUE_INPUT_CALCULATION_OVERFLOW;
                    }
                    else if( ui8Status & ANALOGUE_INPUT_CHANNEL_CALIBRATION )
                    {
                        pxDestination[ui16Index].ui8Status =
                            HMI_ANALOGUE_INPUT_CHANNEL_CALIBRATION;
                    }
                    else if( ( ui8Status & ANALOGUE_INPUT_SET_POINT_VIOLATION_MASK) ==
                             ANALOGUE_INPUT_SET_POINT_VIOLATION_L)
                    {
                        pxDestination[ui16Index].ui8Status =
                            HMI_ANALOGUE_INPUT_SET_POINT_VIOLATION_L;
                    }
                    else if( ( ui8Status & ANALOGUE_INPUT_SET_POINT_VIOLATION_MASK) ==
                             ANALOGUE_INPUT_SET_POINT_VIOLATION_LL_L)
                    {
                        pxDestination[ui16Index].ui8Status =
                            HMI_ANALOGUE_INPUT_SET_POINT_VIOLATION_LL_L;
                    }
                    else if( ( ui8Status & ANALOGUE_INPUT_SET_POINT_VIOLATION_MASK) ==
                             ANALOGUE_INPUT_SET_POINT_VIOLATION_H)
                    {
                        pxDestination[ui16Index].ui8Status =
                            HMI_ANALOGUE_INPUT_SET_POINT_VIOLATION_H;
                    }
                    else if( ( ui8Status & ANALOGUE_INPUT_SET_POINT_VIOLATION_MASK) ==
                             ANALOGUE_INPUT_SET_POINT_VIOLATION_HH_H)
                    {
                        pxDestination[ui16Index].ui8Status =
                            HMI_ANALOGUE_INPUT_SET_POINT_VIOLATION_HH_H;
                    }
                    else
                    {
                        pxDestination[ui16Index].ui8Status = 0;
                    }

                    // получим значение аналогового измерения.
                    pxDestination[ui16Index].fData =
                        pfSource[ui8Address];
                    // скопируем строку текстового реквизита.
                    memcpy(&(pxDestination[ui16Index].acTextDescriptor),
                           (&(pxSource[ui8Address].acTextDescriptor)),
                           (ANALOGUE_INPUT_NAME_LENGTH + END_OF_STRING_LENGTH));
                    // скопируем строку текстового реквизита размерности измеряемого параметра КРЗМ.
                    memcpy(&(pxDestination[ui16Index].acDimentionParameterText),
                           (&(pxDimentionParameter[(pxSource[ui8Address].ucDimentionCode)].acDimentionParameterText)),
                           (DIMENSIONS_PARAMETERS_NAME_LENGTH + END_OF_STRING_LENGTH));
                }
                else
                {
                    // сообщим о конце аналоговых измерений.
                    memset(&(pxDestination[ui16Index].acTextDescriptor),
                           0,
                           (ANALOGUE_INPUT_NAME_LENGTH + END_OF_STRING_LENGTH));
                }
            }

            // вычислим длину вложенного сообщения.
            i16Length =
                (CMind::i16AnalogueMeasureQueryListQuantity *
                 sizeof(struct CMind::TAnalogueMeasurePackOne));
            // вернём длину сообщения.
            pxData -> i16Length = i16Length;
            // вернём длину сообщения.
            return i16Length;
        }
        break;

    case CMind::TASK_MODE_SERVER:

        // функция-задача принимает сообщение.
        // для функции-задачи есть сообщение?
        if (pxData -> i16Length)
        {
            // сообщение не обработано?
            if ((pxTaskContext -> ui8Status) != CMind::TASK_MODE_MESSAGE_IS_HANDLED)
            {
                CMind::TAnalogueMeasureQueryListPackOne *pxSource;
                CMind::TAnalogueMeasureQueryListPackOne *pxDestination;

                // получим указатель на место вложенного сообщения в общем сообщении.
                pxSource = (CMind::TAnalogueMeasureQueryListPackOne*)pxData -> pui8DataPointer;
                // получим указатель на рабочий массив со списком запросов аналоговых измерений.
                pxDestination =
                    (CMind::TAnalogueMeasureQueryListPackOne*)&xAnalogueMeasureQueryList;
                // вычислим количество требуемых аналоговых измерений в запросе.
                // сохраним количество требуемых аналоговых измерений в запросе.
                CMind::i16AnalogueMeasureQueryListQuantity = pxSource -> ui8Quantity;
                // поместим принятый список запросов аналоговых измерений в рабочий массив.
                *pxDestination = *pxSource;
                // установим флаг - сообщение обработано.
                pxTaskContext -> ui8Status = CMind::TASK_MODE_MESSAGE_IS_HANDLED;
                // вернём длину пустого сообщения.
                pxData -> i16Length = 0;
                return 0;
            }
        }
        break;

    default:
        break;

    }
    // вернём длину пустого сообщения.
    pxData -> i16Length = 0;
    return 0;
}

//-----------------------------------------------------------------------------------------------------
// контролирует связь с МИНД. если МИНД не отвечает - авария.
// отвечает на запрос контроля связи от МИНД.
// (ПАС: W - запад, МИНД: E - восток)
int16_t CMind::PingTaskEWWE(TModuleContext *pxModuleContext,
                            CMind::TTaskContext *pxTaskContext,
                            CSerialMT::TSerialMTData *pxData)
{
    int16_t i16Length;

    switch (pxData -> ui8Mode)
    {
    case CMind::TASK_MODE_CLIENT:
        // функция-задача передаёт сообщение.
        // сообщение не отправлено?
        if (pxTaskContext -> fui8MessageIsSent == 0)

        {
            // создадим сообщение.
            *(pxData -> pui8DataPointer) = CMind::TASK_ANSWER_OK;
            // обнулим счётчик относительного времени ожидания ответа адресата на сообщение.
            pxTaskContext -> nui8TimeCounter = 0;
            // установим флаг - сообщение отправлено.
            pxTaskContext -> fui8MessageIsSent = 1;
            // вычислим длину вложенного сообщения.
            i16Length = CMind::TASK_PING_REQUEST_MESSAGE_LENGTH;
            // вернём длину сообщения.
            pxData -> i16Length = i16Length;
            // вернём длину сообщения.
            return i16Length;
        }
        break;

    case CMind::TASK_MODE_SERVER:
        // функция-задача принимает сообщение.
        // сообщение отправлено?
        if (pxTaskContext -> fui8MessageIsSent == 1)
        {
            // функция-задача в роли сервера(принимает сообщение).
            // для функции-задачи есть сообщение?
            if (pxData -> i16Length)
            {
                // получено подтверждение приёма?
                if (*(pxData -> pui8DataPointer) == CMind::TASK_ANSWER_OK)
                {
                    // получено подтверждение приёма(сообщение отправлено успешно).
                    // сбросим флаг - сообщение отправлено.
                    pxTaskContext -> fui8MessageIsSent = 0;
                }
                // получено подтверждение приёма с ошибкой?
                else if (*(pxData -> pui8DataPointer) == CMind::TASK_ANSWER_ERROR)
                {
                    // получено подтверждение приёма с ошибкой.
                    // сбросим флаг - сообщение отправлено.
                    pxTaskContext -> fui8MessageIsSent = 0;
                }
            }
            else
            {
                // превышено время ожидания ответа?
                if ((pxTaskContext -> nui8TimeCounter) >= CMind::TASK_NO_ANSWER_TIMEOUT)
                {
                    // нет ответа на сообщение.
                    // сбросим флаг - сообщение отправлено.
                    pxTaskContext -> fui8MessageIsSent = 0;
                }
                else
                {
                    // увеличим счётчик относительного времени ожидания ответа адресата на сообщение.
                    (pxTaskContext -> nui8TimeCounter)++;
                }
            }
        }
        break;

    default:
        break;
    }

    // вернём длину пустого сообщения.
    pxData -> i16Length = 0;
    return 0;


//    int16_t i16Length;
//
//    switch (pxData -> ui8Mode)
//    {
//    case TASK_MODE_CLIENT:
////        printf("Ping TASK_MODE_CLIENT %d\n\r", nui8FrameCounter);
//        // функция-задача в роли клиента(передаёт сообщение).
//        *(pxData -> pui8DataPointer) = nui8FrameCounter;
//        // вернём длину сообщения.
//        pxData -> i16Length = 1;
//        return i16Length;
//        break;
//
//    case TASK_MODE_SERVER:
////        printf("Ping TASK_MODE_SERVER %d\n\r", *(pxData -> pui8DataPointer));
//        // функция-задача в роли сервера(принимает сообщение).
//        // вернём длину пустого сообщения.
//        pxData -> i16Length = 0;
//        return 0;
//        break;
//
//    default:
//        break;
//    }
//
//    // вернём длину пустого сообщения.
//    pxData -> i16Length = 0;
//    return 0;
}

//-----------------------------------------------------------------------------------------------------
// задача-заглушка.
// вызывается по указателю из массива с контекстом задач по адресу-индексу 0.
// адрес: 0 - не определён протоколом(SerialMT).
int16_t CMind::NopTask(TModuleContext *pxModuleContext,
                       CMind::TTaskContext *pxTaskContext,
                       CSerialMT::TSerialMTData *pxData)
{
    // вернём длину пустого сообщения.
    pxData -> i16Length = 0;
    return 0;
}

//-----------------------------------------------------------------------------------------------------
// задача формирует пустое сообщение размером максимальной длины принимаемого кадра
// CSpiMT::MAX_RECEIVED_FRAME_LENGTH.
// передача и приём на шине SPI происходит в дуплексном режиме.
// если длина передаваемого кадра меньше длины принимаемого кадра(CSpiMT::MAX_RECEIVED_FRAME_LENGTH), то
// принимаемый кадр будет потерян.
// поэтому длина передаваемого кадра должна быть больше принимаемого кадра.
// максимальная длина принимаемого кадра известна на стадии проектирования.
// в задаче сравнивается длина общего сообщения протокола SerialMT и
// максимальная длина принимаемого кадра.
// если длина общего сообщения протокола SerialMT меньше максимальной длины
// принимаемого кадра, то задача формирует пустое сообщение
// размером максимальной длины принимаемого кадра.
int16_t CMind::ReceiveRequest(TModuleContext *pxModuleContext,
                              CMind::TTaskContext *pxTaskContext,
                              CSerialMT::TSerialMTData *pxData)
{
    int16_t i16Length;

    switch (pxData -> ui8Mode)
    {
    case TASK_MODE_CLIENT:
        // функция-задача в роли клиента(передаёт сообщение).
        // создадим сообщение.
        *(pxData -> pui8DataPointer) = 0;
        // вернём длину сообщения.
        pxData -> i16Length = CSerialMT::MAX_RECEIVED_FRAME_LENGTH;
        return pxData -> i16Length;
        break;

    case TASK_MODE_SERVER:
        // функция-задача в роли сервера(принимает сообщение).
        // вернём длину пустого сообщения.
        pxData -> i16Length = 0;
        return 0;
        break;

    default:
        break;
    }

    // вернём длину пустого сообщения.
    pxData -> i16Length = 0;
    return 0;
}


#include <iostream>
#include "Task.h"
#include "SerialMT.h"
#include "Events.h"
#include "EventsDB.h"
#include "Parse.h"
#include "Alarm.h"
#include "RingBuffer.h"

//-----------------------------------------------------------------------------------------------------
CPingTaskEWWE xCPingTaskEWWE;
CPingTaskWEEW xCPingTaskWEEW;
CHighLevelFlagsSetWE xCHighLevelFlagsSetWE;
CHighLevelFlagsSetEW xCHighLevelFlagsSetEW;
CEventLogQuery xCEventLogQuery;
CErrorEventLogQuery xCErrorEventLogQuery;
CArchiveDefaultEventLogQuery xCArchiveDefaultEventLogQuery;
CArchiveByNumberEventLogQuery xCArchiveByNumberEventLogQuery;
CArchiveByDateEventLogQuery xCArchiveByDateEventLogQuery;
CConfigurationQueryEW xCConfigurationQueryEW;
CCurrentTimeQueryEW xCCurrentTimeQueryEW;
CCurrentTimeSetEW xCCurrentTimeSetEW;
CSettingsRW xCSettingsRW;
CAnalogueMeasureQuery xCAnalogueMeasureQuery;
CDiscreteOutputLogQuery xCDiscreteOutputLogQuery;
CReceiveRequest xCReceiveRequest;
CInfoLogQuery xCInfoLogQuery;

// количество объектов должно соответствовать TASK_PROCESSED_MAX_NUMBER.
CTaskSerialMT *CTaskSerialMT::apxTasksProcessed[] =
{
    &xCPingTaskEWWE, //0
    &xCPingTaskWEEW, //1
    &xCHighLevelFlagsSetWE, //2
    &xCHighLevelFlagsSetEW, //3
    &xCEventLogQuery, //4
    &xCErrorEventLogQuery, //5
    &xCArchiveDefaultEventLogQuery, //6
    &xCArchiveByNumberEventLogQuery, //7
    &xCArchiveByDateEventLogQuery, //8
    &xCAnalogueMeasureQuery, //9
    &xCInfoLogQuery, //10
    &xCDiscreteOutputLogQuery, //11
    &xCConfigurationQueryEW, //12
    &xCCurrentTimeQueryEW, //13
    &xCCurrentTimeSetEW, //14
    &xCSettingsRW, //15
    &xCReceiveRequest, //16
    0
};

//-----------------------------------------------------------------------------------------------------
// возвращает состояние задачи.
uint8_t CTaskSerialMT::StateGet(void)
{
    return ui8DataExchangeFsmState;
}

//-----------------------------------------------------------------------------------------------------
// устанавливает состояние задачи.
void CTaskSerialMT::StateSet(uint8_t ui8State)
{
    ui8DataExchangeFsmState = ui8State;
}

//-----------------------------------------------------------------------------------------------------
// возвращает статус задачи.
uint8_t CTaskSerialMT::StatusGet(void)
{
    return ui8Status;
}

//-----------------------------------------------------------------------------------------------------
// устанавливает статус автомата обмена данными задачи.
void CTaskSerialMT::StatusSet(uint8_t ui8StatusLocal)
{
    ui8Status = ui8StatusLocal;
}

//-----------------------------------------------------------------------------------------------------
// устанавливает индекс задачи.
void CTaskSerialMT::IndexSet(uint8_t ui8Index)
{
//    ui8Index = ui8Index;
}

//-----------------------------------------------------------------------------------------------------
uint8_t CTaskSerialMT::SendMessage(CSerialMT *pxCSerialMT, uint8_t ui8Address)
{
    StatusCheck();
    switch (ui8DataExchangeFsmState)
    {
    case TASK_MODE_IDDLE:
        // функция-задача в спящем режиме.
        break;

    case TASK_MODE_MESSAGE_RECEIVE_CONTINUOUSLY:
        // функция-задача принимает сообщение.
        break;

    case TASK_MODE_MESSAGE_SEND_READ_CONTINUOUSLY:
        // функция-задача передаёт сообщение.
        ui8Address = (ui8Address & (~CSerialMT::SERIAL_MT_WRITE));
        ui8LastAddress = ui8Address;
        if (!TransmitProcessing(pxCSerialMT, ui8Address))
        {
            ui8Status = TASK_STATE_MESSAGE_NOT_SEND;
            return 0;
        }
        else
        {
            ui8Status = TASK_STATE_MESSAGE_IS_SEND;
        }
        break;

    case TASK_MODE_MESSAGE_ANSWER_WAITING:
        // ожидаем подтверждение.
        // превышено время ожидания ответа?
        if (nui8TimeCounter >= TASK_NO_ANSWER_TIMEOUT)
        {
            // нет ответа на сообщение.
            if (nui8AttemptsNumber < TASK_MESSAGE_SEND_ATTEMPTS_NUMBER)
            {
                // повторим отправку сообщения.
                // увеличим счётчик попыток отправки сообщения.
                nui8AttemptsNumber++;
                // обнулим счётчик относительного времени ожидания ответа адресата на сообщение.
                nui8TimeCounter = 0;
                if (!TransmitProcessing(pxCSerialMT, ui8LastAddress))
                {
                    ui8Status = TASK_STATE_MESSAGE_NOT_SEND;
                    return 0;
                }
                else
                {
                    ui8Status = TASK_STATE_MESSAGE_ANSWER_WAITING;
                }
            }
            else
            {
                // очистим флаг список запросов событий обновлён.
                ui8Status = TASK_STATE_MESSAGE_RECEIVE_TIMEOUT;
                ui8DataExchangeFsmState = TASK_MODE_IDDLE;
            }
        }
        else
        {
            // увеличим счётчик относительного времени ожидания ответа адресата на сообщение.
            nui8TimeCounter++;
        }
        break;

    case TASK_MODE_MESSAGE_SEND_READ:
        // функция-задача передаёт сообщение.
        // обнулим счётчик попыток отправки сообщения.
        nui8AttemptsNumber = 0;
        // обнулим счётчик относительного времени ожидания ответа адресата на сообщение.
        nui8TimeCounter = 0;
        ui8DataExchangeFsmState = TASK_MODE_MESSAGE_ANSWER_WAITING;
        ui8Status = TASK_STATE_MESSAGE_ANSWER_WAITING;
        ui8Address = (ui8Address & (~CSerialMT::SERIAL_MT_WRITE));
        ui8LastAddress = ui8Address;
        if (!TransmitProcessing(pxCSerialMT, ui8Address))
        {
            ui8Status = TASK_STATE_MESSAGE_NOT_SEND;
            return 0;
        }
        else
        {
            ui8Status = TASK_STATE_MESSAGE_ANSWER_WAITING;
        }
        break;

    case TASK_MODE_MESSAGE_SEND_WRITE:
        // функция-задача передаёт сообщение.
        // обнулим счётчик попыток отправки сообщения.
        nui8AttemptsNumber = 0;
        // обнулим счётчик относительного времени ожидания ответа адресата на сообщение.
        nui8TimeCounter = 0;
        ui8DataExchangeFsmState = TASK_MODE_MESSAGE_ANSWER_WAITING;
        ui8Status = TASK_STATE_MESSAGE_ANSWER_WAITING;
        ui8Address = (ui8Address | CSerialMT::SERIAL_MT_WRITE);
        ui8LastAddress = ui8Address;
        if (!TransmitProcessing(pxCSerialMT, ui8Address))
        {
            ui8Status = TASK_STATE_MESSAGE_NOT_SEND;
            return 0;
        }
        else
        {
            ui8Status = TASK_STATE_MESSAGE_ANSWER_WAITING;
        }
        break;

    case TASK_MODE_MESSAGE_SEND_READ_ANSWER:
        // функция-задача передаёт сообщение.
        ui8Address = (ui8Address & (~CSerialMT::SERIAL_MT_WRITE));
        ui8LastAddress = ui8Address;
        if (!TransmitProcessing(pxCSerialMT, ui8Address))
        {
            ui8Status = TASK_STATE_MESSAGE_NOT_SEND;
            return 0;
        }
        else
        {
            ui8Status = TASK_STATE_MESSAGE_IS_SEND;
        }
        ui8DataExchangeFsmState = TASK_MODE_MESSAGE_RECEIVE_CONTINUOUSLY;
        break;


    case TASK_MODE_MESSAGE_SEND_WRITE_ANSWER:
        // функция-задача передаёт сообщение.
        ui8Address = (ui8Address | CSerialMT::SERIAL_MT_WRITE);
        ui8LastAddress = ui8Address;
        if (!TransmitProcessing(pxCSerialMT, ui8Address))
        {
            ui8Status = TASK_STATE_MESSAGE_NOT_SEND;
            return 0;
        }
        else
        {
            ui8Status = TASK_STATE_MESSAGE_IS_SEND;
        }
        ui8DataExchangeFsmState = TASK_MODE_MESSAGE_RECEIVE_CONTINUOUSLY;
        break;

    default:
        break;
    }
    return 1;
}

//-----------------------------------------------------------------------------------------------------
uint8_t CTaskSerialMT::ReceiveMessage(CSerialMT *pxCSerialMT, uint8_t ui8Address)
{
    switch (ui8DataExchangeFsmState)
    {
    case TASK_MODE_IDDLE:
        // функция-задача в спящем режиме.
        break;

    case TASK_MODE_MESSAGE_RECEIVE_CONTINUOUSLY:
        // функция-задача принимает сообщение.
        if ((ui8Address & CSerialMT::SERIAL_MT_WRITE) != 0)
        {
            // переключим автомат на передачу сообщения.
            ui8DataExchangeFsmState = TASK_MODE_MESSAGE_SEND_WRITE_ANSWER;
        }
        else
        {
            // переключим автомат на передачу сообщения.
            ui8DataExchangeFsmState = TASK_MODE_MESSAGE_SEND_READ_ANSWER;
        }
        ReceiveProcessing(pxCSerialMT, ui8Address);
        ui8Status = TASK_STATE_MESSAGE_RESPONSE_OK;
        break;

    case TASK_MODE_MESSAGE_SEND_READ_CONTINUOUSLY:
        // функция-задача принимает сообщение.
        ReceiveProcessing(pxCSerialMT, ui8Address);
        ui8Status = TASK_STATE_MESSAGE_RESPONSE_OK;
        break;

    case TASK_MODE_MESSAGE_ANSWER_WAITING:
        // ожидаем подтверждение.
        ReceiveProcessing(pxCSerialMT, ui8Address);
        ui8Status = TASK_STATE_MESSAGE_RESPONSE_OK;
        ui8DataExchangeFsmState = TASK_MODE_IDDLE;
        return 1;
        break;

    default:
        break;
    }
}

//-----------------------------------------------------------------------------------------------------
// производит обмен данными МИНД с ПАС.
uint8_t CTaskSerialMT::DataExchange(CTaskSerialMT **ppxTasksProcessed, CSerialMT *pxCSerialMT)
{
    uint8_t ui8Length;
    CTaskSerialMT **ppxTasksProcessedLocal;
    CTaskSerialMT *pxCTaskSerialMT;

    ppxTasksProcessedLocal = ppxTasksProcessed;
    // размер массива с указателями на объекты задач известен?
    if (ui8TasksProcessedNumber)
    {
        pxCSerialMT -> DataExchange();
        // получим принятое общее сообщение.
        if (!(pxCSerialMT -> DataReceive()))
        {
            pxCSerialMT -> MessagePopInitialization();
            if (pxCSerialMT -> MessagePopCheck())
            {
                for (uint8_t i = 0; i < ui8TasksProcessedNumber; i++)
                {
                    if (!(pxCSerialMT -> MessagePop()))
                    {
                        break;
                    }
                    uint8_t ui8AddressLocal;
                    ui8AddressLocal = ((pxCSerialMT -> ui8Address) &
                                       (~CSerialMT::SERIAL_MT_WRITE));
                    // адрес существующей задачи?
                    if (ui8AddressLocal < ui8TasksProcessedNumber)
                    {
                        // получим указатель на объект задачи.
                        pxCTaskSerialMT = ppxTasksProcessedLocal[ui8AddressLocal];
                        pxCTaskSerialMT -> ReceiveMessage(pxCSerialMT, (pxCSerialMT -> ui8Address));
                    }
                }
            }
        }

        ppxTasksProcessedLocal = ppxTasksProcessed;
        pxCSerialMT -> MessagePushInitialization();
        for (uint8_t i = 0; i < ui8TasksProcessedNumber; i++)
        {
            pxCTaskSerialMT = *ppxTasksProcessedLocal;
            if (!(pxCTaskSerialMT -> SendMessage(pxCSerialMT, i)))
            {
//                break;
            }
            ppxTasksProcessedLocal++;
        }

        // отправим общее сообщение.
        pxCSerialMT -> DataTransmit();
    }
    else
    {
        // вычислим размер массива с указателями на объекты задач.
        for (uint8_t i = 0;
                ((0 != *ppxTasksProcessedLocal++) &&
                 (i < TASK_PROCESSED_MAX_NUMBER));
                i++)
        {
            ui8TasksProcessedNumber++;
        }
    }

    return 1;
}

//-----------------------------------------------------------------------------------------------------
void CTaskSerialMT::StatusCheck(void)
{

}

//-----------------------------------------------------------------------------------------------------



//-----------------------------------------------------------------------------------------------------
uint8_t CTaskSerialMT::TransmitProcessing(CSerialMT *pxCSerialMT, uint8_t ui8Address)
{
    uint8_t ui8Length;
    uint8_t *pui8Destination;
    // вычислим длину вложенного сообщения.
    ui8Length = TASK_PING_REQUEST_MESSAGE_LENGTH;
    // узнаем, есть ли место для сообщения.
    pui8Destination = pxCSerialMT -> MessagePushRequest(ui8Length,
                      ui8Address);
    // нет места для сообщеня?
    if (!pui8Destination)
    {
        return 0;
    }
    else
    {
        // создадим сообщение.
        (*pui8Destination) = 0;
        return 1;
    }
}

//-----------------------------------------------------------------------------------------------------
uint8_t CTaskSerialMT::LoadAnswerData(CSerialMT *pxCSerialMT, uint8_t ui8Address)
{

}

//-----------------------------------------------------------------------------------------------------
uint8_t CTaskSerialMT::ReceiveProcessing(CSerialMT *pxCSerialMT, uint8_t ui8Address)
{
    if ((ui8Address & CSerialMT::SERIAL_MT_WRITE) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

//-----------------------------------------------------------------------------------------------------



//-----------------------------------------------------------------------------------------------------
uint8_t CPingTaskEWWE::TransmitProcessing(CSerialMT *pxCSerialMT, uint8_t ui8Address)
{
    uint8_t ui8Length;
    uint8_t *pui8Destination;
    // вычислим длину вложенного сообщения.
    ui8Length = TASK_PING_REQUEST_MESSAGE_LENGTH;
    // узнаем, есть ли место для сообщения.
    pui8Destination = pxCSerialMT -> MessagePushRequest(ui8Length,
                      ui8Address);
    // нет места для сообщеня?
    if (!pui8Destination)
    {
        return 0;
    }
    else
    {
        // создадим сообщение.
        (*pui8Destination) = 0;
        return 1;
    }
}

//-----------------------------------------------------------------------------------------------------
uint8_t CPingTaskEWWE::LoadAnswerData(CSerialMT *pxCSerialMT, uint8_t ui8Address)
{

}

//-----------------------------------------------------------------------------------------------------
uint8_t CPingTaskEWWE::ReceiveProcessing(CSerialMT *pxCSerialMT, uint8_t ui8Address)
{
    if ((ui8Address & CSerialMT::SERIAL_MT_WRITE) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

//-----------------------------------------------------------------------------------------------------



//-----------------------------------------------------------------------------------------------------
uint8_t CPingTaskWEEW::TransmitProcessing(CSerialMT *pxCSerialMT, uint8_t ui8Address)
{
    uint8_t ui8Length;
    uint8_t *pui8Destination;
    // вычислим длину вложенного сообщения.
    ui8Length = TASK_PING_REQUEST_MESSAGE_LENGTH;
    // узнаем, есть ли место для сообщения.
    pui8Destination = pxCSerialMT -> MessagePushRequest(ui8Length,
                      ui8Address);
    // нет места для сообщеня?
    if (!pui8Destination)
    {
        return 0;
    }
    else
    {
        // создадим сообщение.
        (*pui8Destination) = ui8InitFsmState;
        return 1;
    }
}

//-----------------------------------------------------------------------------------------------------
uint8_t CPingTaskWEEW::LoadAnswerData(CSerialMT *pxCSerialMT, uint8_t ui8Address)
{

}

//-----------------------------------------------------------------------------------------------------
uint8_t CPingTaskWEEW::ReceiveProcessing(CSerialMT *pxCSerialMT, uint8_t ui8Address)
{
    // команда - запись?
    if ((ui8Address & CSerialMT::SERIAL_MT_WRITE) != 0)
    {
        return 1;
    }
    else
    {
        if (ui8InitFsmState == TASK_MODE_IDDLE)
        {
            ui8InitFsmState = TASK_MODE_MESSAGE_RECEIVE_CONTINUOUSLY;
        }
        return 0;
    }
}

//-----------------------------------------------------------------------------------------------------



//-----------------------------------------------------------------------------------------------------
void CHighLevelFlagsSetWE::StatusCheck(void)
{
    // есть команда - копирование диска mmc0 на mmc1?
    if (fbGlobalMmcCopy)
    {
        // команду приняли.
        fbGlobalMmcCopy = 0;
        // установим флаг - копирование диска mmc0 на mmc1.
        xTHighLevelFlagsExchangePackOne.ui8HighLevelFlagsWE |= (1 << HIGH_LEVEL_FLAGS_MMC_COPY_BIT);
        // переключим автомат на передачу сообщения.
        ui8DataExchangeFsmState = TASK_MODE_MESSAGE_SEND_WRITE;
    }

    // есть команда - закончено копирование диска mmc0 на mmc1?
    if (fbGlobalMmcCopyEnd)
    {
        // команду приняли.
        fbGlobalMmcCopyEnd = 0;
        // установим флаг - копирование диска mmc0 на mmc1.
        xTHighLevelFlagsExchangePackOne.ui8HighLevelFlagsWE |= (1 << HIGH_LEVEL_FLAGS_MMC_COPY_END_BIT);
        // переключим автомат на передачу сообщения.
        ui8DataExchangeFsmState = TASK_MODE_MESSAGE_SEND_WRITE;
    }

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
        xTHighLevelFlagsExchangePackOne.ui8HighLevelFlagsWE |= (1 << HIGH_LEVEL_FLAGS_KVIT_BIT);
        // переключим автомат на передачу сообщения.
        ui8DataExchangeFsmState = TASK_MODE_MESSAGE_SEND_WRITE;

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
        xTHighLevelFlagsExchangePackOne.ui8HighLevelFlagsWE |= (1 << HIGH_LEVEL_FLAGS_RESET_BIT);
        // переключим автомат на передачу сообщения.
        ui8DataExchangeFsmState = TASK_MODE_MESSAGE_SEND_WRITE;
    }

    // есть блокировка?
    if (fbGlobalBlock)
    {
        // состояние режима блокировки изменилось?
        if (!(xTHighLevelFlagsExchangePackOne.ui8HighLevelFlagsWE &
                (1 << HIGH_LEVEL_FLAGS_BLOCK_BIT)))
        {
            // установим флаг - блокировка от PC.
            xTHighLevelFlagsExchangePackOne.ui8HighLevelFlagsWE |= (1 << HIGH_LEVEL_FLAGS_BLOCK_BIT);
            // переключим автомат на передачу сообщения.
            ui8DataExchangeFsmState = TASK_MODE_MESSAGE_SEND_WRITE;
        }
    }
    else
    {
        if (xTHighLevelFlagsExchangePackOne.ui8HighLevelFlagsWE &
                (1 << HIGH_LEVEL_FLAGS_BLOCK_BIT))
        {
            // сбросим флаг - блокировка от PC.
            xTHighLevelFlagsExchangePackOne.ui8HighLevelFlagsWE &= ~(1 << HIGH_LEVEL_FLAGS_BLOCK_BIT);
            // переключим автомат на передачу сообщения.
            ui8DataExchangeFsmState = TASK_MODE_MESSAGE_SEND_WRITE;
        }
    }

    // есть звуковой сигнал?
    if ((fucZvkMal != 0) && (fucZvkMal != 1))
//    if (fucZvkMal)
    {
        // отправим команду звука в МИНД.
        xTHighLevelFlagsExchangePackOne.ui8SoundSignalType = fucZvkMal;
        // очистим команду звука.
        fucZvkMal = COMMAND_SOUND_SIGNAL_TYPE_NOT;
        // переключим автомат на передачу сообщения.
        ui8DataExchangeFsmState = TASK_MODE_MESSAGE_SEND_WRITE;
    }
    else
    {
        // отправим команду нет звука в МИНД.
        if ((xTHighLevelFlagsExchangePackOne.ui8SoundSignalType))
        {
            xTHighLevelFlagsExchangePackOne.ui8SoundSignalType = COMMAND_SOUND_SIGNAL_TYPE_OFF;
        }
    }
}

//-----------------------------------------------------------------------------------------------------
uint8_t CHighLevelFlagsSetWE::TransmitProcessing(CSerialMT *pxCSerialMT, uint8_t ui8Address)
{
    uint8_t ui8Length;
    uint8_t *pui8Destination;
    THighLevelFlagsExchangePackOne *pxSource;
    THighLevelFlagsExchangePackOne *pxDestination;
    // вычислим длину вложенного сообщения.
    ui8Length = sizeof(struct THighLevelFlagsExchangePackOne);
    // узнаем, есть ли место для сообщения.
    pui8Destination = pxCSerialMT -> MessagePushRequest(ui8Length,
                      ui8Address);
    // нет места для сообщеня?
    if (!pui8Destination)
    {
        return 0;
    }
    else
    {
        // получим указатель на данные.
        pxSource = &xTHighLevelFlagsExchangePackOne;
        pxDestination = (THighLevelFlagsExchangePackOne*)pui8Destination;
        // создадим сообщение.
        *pxDestination = *pxSource;
        return 1;
    }
}

//-----------------------------------------------------------------------------------------------------
uint8_t CHighLevelFlagsSetWE::LoadAnswerData(CSerialMT *pxCSerialMT, uint8_t ui8Address)
{

}

//-----------------------------------------------------------------------------------------------------
uint8_t CHighLevelFlagsSetWE::ReceiveProcessing(CSerialMT *pxCSerialMT, uint8_t ui8Address)
{
    if ((ui8Address & CSerialMT::SERIAL_MT_WRITE) != 0)
    {
        // получено подтверждение приёма(сообщение отправлено успешно).
        // сбросим флаги управления квитирование и сброс.
        // флаг блокировка оставим.
        xTHighLevelFlagsExchangePackOne.ui8HighLevelFlagsWE &=
            ~((1 << HIGH_LEVEL_FLAGS_KVIT_BIT) | (1 << HIGH_LEVEL_FLAGS_RESET_BIT) |
              (1 << HIGH_LEVEL_FLAGS_MMC_COPY_BIT) | (1 << HIGH_LEVEL_FLAGS_MMC_COPY_END_BIT));
        return 1;
    }
    else
    {
        // получено подтверждение приёма(сообщение отправлено успешно).
        // сбросим флаги управления квитирование и сброс.
        // флаг блокировка оставим.
        xTHighLevelFlagsExchangePackOne.ui8HighLevelFlagsWE &=
            ~((1 << HIGH_LEVEL_FLAGS_KVIT_BIT) | (1 << HIGH_LEVEL_FLAGS_RESET_BIT) |
              (1 << HIGH_LEVEL_FLAGS_MMC_COPY_BIT) | (1 << HIGH_LEVEL_FLAGS_MMC_COPY_END_BIT));
        return 0;
    }
}

//-----------------------------------------------------------------------------------------------------



//-----------------------------------------------------------------------------------------------------
uint8_t CHighLevelFlagsSetEW::TransmitProcessing(CSerialMT *pxCSerialMT, uint8_t ui8Address)
{
    uint8_t ui8Length;
    uint8_t *pui8Destination;
    // вычислим длину вложенного сообщения.
    ui8Length = TASK_CONFIGURATION_REQUEST_MESSAGE_LENGTH;
    // узнаем, есть ли место для сообщения.
    pui8Destination = pxCSerialMT -> MessagePushRequest(ui8Length,
                      ui8Address);
    // нет места для сообщеня?
    if (!pui8Destination)
    {
        return 0;
    }
    else
    {
        // создадим сообщение.
        (*pui8Destination) = 0;
        return 1;
    }
}

//-----------------------------------------------------------------------------------------------------
uint8_t CHighLevelFlagsSetEW::LoadAnswerData(CSerialMT *pxCSerialMT, uint8_t ui8Address)
{

}

//-----------------------------------------------------------------------------------------------------
uint8_t CHighLevelFlagsSetEW::ReceiveProcessing(CSerialMT *pxCSerialMT, uint8_t ui8Address)
{
    if ((ui8Address & CSerialMT::SERIAL_MT_WRITE) != 0)
    {
        uint8_t ui8HighLevelFlagsTemp;
        TModuleContext *pxModuleContextOrigin;
        // получим указатель на начало в массиве с объектами контекста всех модулей.
        pxModuleContextOrigin = (pxModuleContext - (pxModuleContext ->
                                 xModuleContextStatic.
                                 ucModuleContextIndex));
        // получим состояние флагов от МИНД.
        ui8HighLevelFlagsTemp = *(pxCSerialMT -> pui8DataPointer);
        // есть квитирование от МИНД?
        if (ui8HighLevelFlagsTemp & (1 << HIGH_LEVEL_FLAGS_KVIT_BIT))
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
//
//        pthread_kill(xModbusRtu, SIGRTMIN+1);
//            pthread_kill(xModbusTcp, SIGRTMIN);
//        pthread_kill(xHmiModbusRtu, SIGRTMIN+2);
//            printf("pthread_kill %d\n\r", (xModbusRtu));
        }
        // есть сброс от МИНД?
        if (ui8HighLevelFlagsTemp & (1 << HIGH_LEVEL_FLAGS_RESET_BIT))
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
        if (ui8HighLevelFlagsTemp & (1 << HIGH_LEVEL_FLAGS_BLOCK_BIT))
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

        return 1;
    }
    else
    {
        return 0;
    }
}

//-----------------------------------------------------------------------------------------------------






uint8_t CEventLogQuery::ui8DirectionIsForward;
//int16_t CEventLogQuery::i16LastRequestIndex;
//-----------------------------------------------------------------------------------------------------
void CEventLogQuery::FillingMessage(CEvents::TEventDataPackOne *pxDestination, CEvents::TEventDataCommon *pxSource, int16_t i16IndexQuery)
{
    if (!pxSource)
    {
        // ошибка.
        // получим тип события.
        pxDestination -> ui8Type = 0;
        // индекс запрашиваемой ошибки.
        // для идентификации при поиске ошибки при выводе в МИНД.
        pxDestination -> i16Index =
            (i16IndexQuery);
        // пометим события маркером - нет события.
        pxDestination -> ui16ID = 0;
    }
    else
    {
        // есть маркер - нет события?
        if ((pxSource -> ui16ID) == 0)
        {
            // получим тип события.
            pxDestination -> ui8Type = 0;
            // индекс запрашиваемой ошибки.
            // для идентификации при поиске ошибки при выводе в МИНД.
            pxDestination -> i16Index =
                (i16IndexQuery);
            // пометим события маркером - нет события.
            pxDestination -> ui16ID = 0;
        }
        else
        {
            if ((pxSource -> ui8Type) == CEvents::HANDLED_EVENTS_CONFIGURATION_ERROR_TYPE ||
                    (pxSource -> ui8Type) == CEvents::HANDLED_EVENTS_INFO_TYPE)
            {
                // получим id события.
                pxDestination -> ui16ID =
                    pxSource -> ui16ID;
                // получим тип события.
                pxDestination -> ui8Type =
                    pxSource -> ui8Type;
                // индекс запрашиваемого события.
                // для идентификации при поиске события при выводе в МИНД.
                pxDestination -> i16Index =
                    (i16IndexQuery);
                // скопируем строку данных.
                memcpy(&(pxDestination -> acTextData),
                       (&(pxSource -> acTextData)),
                       (CEvents::EVENT_NAME_LENGTH + END_OF_STRING_LENGTH));
                // скопируем строку текстового реквизита.
                memcpy(&(pxDestination -> acTextDescriptor),
                       (&(pxSource -> acTextDescriptor)),
                       (CEvents::EVENT_NAME_LENGTH + END_OF_STRING_LENGTH));
            }
//            else if ((pxSource -> ui8Type) == CEvents::HANDLED_EVENTS_INFO_TIME_TYPE)
//            {
//                // получим id события.
//                pxDestination -> ui16ID =
//                    pxSource -> ui16ID;
//                // получим тип события.
//                pxDestination -> ui8Type =
//                    CEvents::HANDLED_EVENTS_INFO_TYPE;
//                // индекс запрашиваемого события.
//                // для идентификации при поиске события при выводе в МИНД.
//                pxDestination -> i16Index =
//                    (i16IndexQuery);
//                // поместим время события.
//                pxDestination -> ui8Second = (uint8_t)pxSource -> xCurrentTime.tm_sec;
//                pxDestination -> ui8Minute = (uint8_t)pxSource -> xCurrentTime.tm_min;
//                pxDestination -> ui8Hour = (uint8_t)pxSource -> xCurrentTime.tm_hour;
//                pxDestination -> ui8MonthDay = (uint8_t)pxSource -> xCurrentTime.tm_mday;
//                pxDestination -> ui8Month = (uint8_t)(pxSource -> xCurrentTime.tm_mon) + 1;
//                pxDestination -> ui8Year = (uint8_t)(pxSource -> xCurrentTime.tm_year) - 100;
//                // скопируем строку текстового реквизита.
//                memcpy(&(pxDestination -> acTextDescriptor),
//                       (&(pxSource -> acTextDescriptor)),
//                       (CEvents::EVENT_NAME_LENGTH + END_OF_STRING_LENGTH));
//            }
            else
            {
                // получим id события.
                pxDestination -> ui16ID =
                    pxSource -> ui16ID;
                // получим тип события.
                pxDestination -> ui8Type =
                    pxSource -> ui8Type;
                // индекс запрашиваемого события.
                // для идентификации при поиске события при выводе в МИНД.
                pxDestination -> i16Index =
                    (i16IndexQuery);
                // получим адрес события.
                pxDestination -> ui16Address =
                    pxSource -> ui16Address;
                // получим состояние события.
                pxDestination -> ui8State =
                    pxSource -> ui8State;
                // отказ текущего модуля не при старте?
                if ((pxSource -> xCurrentTime.tm_sec) != 0xFF)
                {
                    // поместим время события.
                    pxDestination -> ui8Second = (uint8_t)pxSource -> xCurrentTime.tm_sec;
                    pxDestination -> ui8Minute = (uint8_t)pxSource -> xCurrentTime.tm_min;
                    pxDestination -> ui8Hour = (uint8_t)pxSource -> xCurrentTime.tm_hour;
                    pxDestination -> ui8MonthDay = (uint8_t)pxSource -> xCurrentTime.tm_mday;
                    pxDestination -> ui8Month = (uint8_t)(pxSource -> xCurrentTime.tm_mon) + 1;
                    pxDestination -> ui8Year = (uint8_t)(pxSource -> xCurrentTime.tm_year) - 100;
                }
                else
                {
                    // установим код времени события - при старте.
                    pxDestination -> ui8Second = 0xFF;
                }
                // скопируем строку текстового реквизита.
                memcpy(&(pxDestination -> acTextDescriptor),
                       (&(pxSource -> acTextDescriptor)),
                       (CEvents::EVENT_NAME_LENGTH + END_OF_STRING_LENGTH));
            }
        }
    }
}

//-----------------------------------------------------------------------------------------------------
uint8_t CEventLogQuery::TransmitProcessing(CSerialMT *pxCSerialMT, uint8_t ui8Address)
{
    uint8_t ui8Length;
    uint8_t *pui8Destination;

    // вычислим длину вложенного сообщения.
    ui8Length =
        (CEvents::xEventsLogQueryList.ui8Quantity *
         sizeof(struct CEvents::TEventDataPackOne));
    // узнаем, есть ли место для сообщения.
    pui8Destination = pxCSerialMT -> MessagePushRequest(ui8Length,
                      ui8Address);
    // нет места для сообщеня?
    if (!pui8Destination)
    {
        return 0;
    }
    else
    {
        // создадим сообщение.
        // получим количество требуемых событий в запросе.
        ui8Length = CEvents::xEventsLogQueryList.ui8Quantity;
        // количество полученных событий превосходит
        // размер массива?
        if (ui8Length >
                (CEvents::REQUESTED_EVENTS_QUANTITY))
        {
            // ошибка.
            return 0;
        }

        CEvents::TEventDataCommon *pxSource;
        CEvents::TEventDataPackOne *pxDestination;
        // индекс запрашиваемого события.
        int16_t i16IndexQuery;
        uint16_t ui16ID;


        // получим указатель на место вложенного сообщения в общем сообщении.
        pxDestination = (CEvents::TEventDataPackOne*)(pui8Destination);
        CEvents *pxCEvents;
        switch (CEvents::xEventsLogQueryList.ui8Type)
        {
        case CEvents::REQUESTED_EVENTS_ALARM_TYPE:
            printf("CEventLogQuery REQUESTED_EVENTS_ALARM_TYPE %d\n\r", (ui8Address));

            pxCEvents = &xCAlarmEvent;
            // получим индекс первого запрашиваемого события.
            i16IndexQuery = (CEvents::xEventsLogQueryList.i16Index -
                             CArchiveEventsDB::EVENTS_DB_PREVIOUS_RECORDS_QUANTITY);
            // установим индекс в массиве указателей на события в массиве даных событий.
            pxCEvents -> EventDataPopIndexSet(i16IndexQuery + CONVERT_INTEGER_TO_NATURAL_NUMBER);
            // поместим данные событий во вложенное сообщение.
            for (uint16_t i = 0;
                    i < ui8Length;
                    i++)
            {
                // получим указатель на данные события.
                FillingMessage(&pxDestination[i],
                               pxCEvents -> EventDataPop(),
                               i16IndexQuery);
                // индекс следующего запрашиваемого события.
                i16IndexQuery++;
            }
            break;

        case CEvents::REQUESTED_EVENTS_ERROR_TYPE:
            printf("CEventLogQuery REQUESTED_EVENTS_ERROR_TYPE %d\n\r", (ui8Address));
            pxCEvents = &xCPlcErrorEvent;
            // получим индекс первого запрашиваемого события.
            i16IndexQuery = (CEvents::xEventsLogQueryList.i16Index -
                             CArchiveEventsDB::EVENTS_DB_PREVIOUS_RECORDS_QUANTITY);
            // установим индекс в массиве указателей на события в массиве даных событий.
            pxCEvents -> EventDataPopIndexSet(i16IndexQuery + CONVERT_INTEGER_TO_NATURAL_NUMBER);
            // поместим данные событий во вложенное сообщение.
            for (uint16_t i = 0;
                    i < ui8Length;
                    i++)
            {
                // получим указатель на данные события.
                FillingMessage(&pxDestination[i],
                               pxCEvents -> EventDataPop(),
                               i16IndexQuery);
                // индекс следующего запрашиваемого события.
                i16IndexQuery++;
            }
            break;

        case CEvents::REQUESTED_EVENTS_INFO_TYPE:
            printf("CEventLogQuery REQUESTED_EVENTS_INFO_TYPE %d\n\r", (ui8Address));
            pxCEvents = &xCInfoEvent;
            // получим индекс первого запрашиваемого события.
            i16IndexQuery = (CEvents::xEventsLogQueryList.i16Index -
                             CArchiveEventsDB::EVENTS_DB_PREVIOUS_RECORDS_QUANTITY);
            // запрашивается первая запись?
            if (!CEvents::xEventsLogQueryList.i16Index)
            {
//                InfoEvetnsLogCreate();
            }
            // установим индекс в массиве указателей на события в массиве даных событий.
            pxCEvents -> EventDataPopIndexSet(i16IndexQuery + CONVERT_INTEGER_TO_NATURAL_NUMBER);
            // поместим данные событий во вложенное сообщение.
            for (uint16_t i = 0;
                    i < ui8Length;
                    i++)
            {
                // получим указатель на данные события.
                FillingMessage(&pxDestination[i],
                               pxCEvents -> EventDataPop(),
                               i16IndexQuery);
                // индекс следующего запрашиваемого события.
                i16IndexQuery++;
            }
            break;

        case CEvents::REQUESTED_EVENTS_ARCHIVE_TYPE:
            printf("CEventLogQuery REQUESTED_EVENTS_ARCHIVE_TYPE %d\n\r", (ui8Address));
            if (CEvents::xEventsLogQueryList.i16Index == 0)
            {
                // запросим количество записей в таблице базы данных.
                xCArchiveEventsDB.ui16TotalRecordsNumber =
                    xCArchiveEventsDB.SendQuery("SELECT COUNT(*) FROM Events;");
                // чтобы в первый раз получить начиная с последней записи,
                // получим id последней записи плюс один.
                // в запросе требуем запись с id меньше последнего.
                xCArchiveEventsDB.ui16LastLastID = (xCArchiveEventsDB.ui16TotalRecordsNumber + 1);
                xCArchiveEventsDB.ui16LastFirstID = xCArchiveEventsDB.ui16LastLastID;
                i16LastRequestIndex = 0;
            }
            // получим индекс первого запрашиваемого события.
            i16IndexQuery = (CEvents::xEventsLogQueryList.i16Index);

//            printf("CEventLogQuery i16IndexQuery %d\n\r", (i16IndexQuery));
//            printf("CEventLogQuery i16LastRequestIndex %d\n\r", (i16LastRequestIndex));

            xCArchiveEventsDB.i16IndexQuery = i16IndexQuery;
            pxCEvents = &xCPlcArchiveEvent;
            // очистим буфер событий, чтобы иметь возможность сообщить МИНД,
            // что события закончились.
            pxCEvents -> NoEventsFlagSet();
            // направление чтения вперёд(от большего ID к меньшему ID, от ближнего события к дальнему)?
            if (i16IndexQuery >= i16LastRequestIndex)
            {
                // сохраним текущий индекс запроса.
                i16LastRequestIndex = i16IndexQuery;
                // направление чтения вперёд.
                ui8DirectionIsForward = 1;
                // извлечём записи из базы данных в буфер.
                xCArchiveEventsDB.DataBaseDataGet();
                // установим индекс в массиве указателей на события в массиве даных событий.
                pxCEvents -> EventDataPopIndexSet(xCArchiveEventsDB.ui8QueryGetRowQuantity);
                // поместим данные событий во вложенное сообщение.
                for (uint16_t i = 0;
                        i < ui8Length;
                        i++)
                {
                    // получим указатель на данные события.
                    FillingMessage(&pxDestination[i],
                                   pxCEvents -> EventDataPop(),
                                   i16IndexQuery);
                    // индекс следующего запрашиваемого события.
                    i16IndexQuery++;
                }
            }
            else
            {
                // сохраним текущий индекс запроса.
                i16LastRequestIndex = i16IndexQuery;
                // направление чтения назад(от меньшего ID к большему ID, от дальнего события к ближнему).
                ui8DirectionIsForward = 0;
                // извлечём записи из базы данных в буфер.
                xCArchiveEventsDB.DataBaseDataGet();
                // установим индекс в массиве указателей на события в массиве даных событий.
                pxCEvents -> EventDataPopIndexSet(xCArchiveEventsDB.ui8QueryGetRowQuantity);
                // поместим данные событий во вложенное сообщение.
                for (uint16_t i = xCArchiveEventsDB.ui8QueryGetRowQuantity;
                        i > 0;
                    )
                {
                    // преобразуем порядковый номер в индекс в массиве.
                    i--;
                    // получим указатель на данные события.
                    FillingMessage(&pxDestination[i],
                                   pxCEvents -> EventDataPop(),
                                   (i16IndexQuery + i));
                }
            }

            break;
        default:
            break;
        }

        return 1;
    }
}

//-----------------------------------------------------------------------------------------------------
uint8_t CEventLogQuery::LoadAnswerData(CSerialMT *pxCSerialMT, uint8_t ui8Address)
{

}

//-----------------------------------------------------------------------------------------------------
uint8_t CEventLogQuery::ReceiveProcessing(CSerialMT *pxCSerialMT, uint8_t ui8Address)
{
    if ((ui8Address & CSerialMT::SERIAL_MT_WRITE) != 0)
    {
        return 1;
    }
    else
    {
        CEvents::TEventsLogQueryListPackOne *pxSource;
        CEvents::TEventsLogQueryListPackOne *pxDestination;

        // получим указатель на место вложенного сообщения в общем сообщении.
        pxSource = (CEvents::TEventsLogQueryListPackOne*)pxCSerialMT -> pui8DataPointer;
        // получим указатель на рабочий массив со списком запросов событий.
        pxDestination =
            (CEvents::TEventsLogQueryListPackOne*)&CEvents::xEventsLogQueryList;
        // поместим принятый список запросов событий в рабочий массив.
        *pxDestination = *pxSource;
        return 0;
    }
}

//-----------------------------------------------------------------------------------------------------




//-----------------------------------------------------------------------------------------------------
uint8_t CDiscreteOutputLogQuery::TransmitProcessing(CSerialMT *pxCSerialMT, uint8_t ui8Address)
{
    uint8_t ui8Length;
    uint8_t *pui8Destination;
    // вычислим длину вложенного сообщения.
    ui8Length =
        (CEvents::xEventsLogQueryList.ui8Quantity *
         sizeof(struct CEvents::TEventDataPackOne));
    // узнаем, есть ли место для сообщения.
    pui8Destination = pxCSerialMT -> MessagePushRequest(ui8Length,
                      ui8Address);
    // нет места для сообщеня?
    if (!pui8Destination)
    {
        return 0;
    }
    else
    {
        // создадим сообщение.
        // получим количество требуемых событий в запросе.
        ui8Length = CEvents::xEventsLogQueryList.ui8Quantity;
        // количество полученных событий превосходит
        // размер массива?
        if (ui8Length >
                (DISCRETE_INPUT_QUERY_LIST_LENGTH))
        {
            // ошибка.
            return 0;
        }

        uint8_t *pui8Source;
        CEvents::TEventDataPackOne *pxDestination;
        // индекс запрашиваемого события.
        int16_t i16IndexQuery;
        uint8_t ui8Data;

        // получим индекс первого запрашиваемого события.
        i16IndexQuery = (CEvents::xEventsLogQueryList.i16Index);
        // получим указатель на массив состояния дискретных выходов.
        pui8Source =
            (uint8_t*)pxModuleContext ->
            xModuleContextStatic.
            pucSostDoOriginBufferPointer;
        // получим указатель на место вложенного сообщения в общем сообщении.
        pxDestination = (CEvents::TEventDataPackOne*)(pui8Destination);
        // поместим данные событий во вложенное сообщение.
        for (uint16_t i = 0;
                i < ui8Length;
                i++)
        {
            // индекс запрашиваемого байта(модуля) состояния дискретных выходов
            // выходит за пределы массива состояния дискретных выходов?
            if (i16IndexQuery >=
                    (xPlcConfigService.xPlcConfigServiceData.ucServiceDiscreteOutputModuleQuantity))
            {
                // ошибка.
                // индекс запрашиваемой ошибки.
                // для идентификации при поиске ошибки при выводе в МИНД.
                pxDestination[i].i16Index =
                    (i16IndexQuery);
                // пометим события маркером - нет события.
                pxDestination[i].ui16ID = 0;
            }
            else
            {
                pxDestination[i].ui16ID = 1;
                // индекс запрашиваемого байта(модуля) состояния дискретных выходов.
                // для идентификации при поиске байта(модуля) состояния дискретных выходов при выводе в МИНД.
                pxDestination[i].i16Index =
                    i16IndexQuery;

                // получим состояние дискретных выходов.
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
            }
            // индекс следующего запрашиваемого события.
            i16IndexQuery++;
        }
        return 1;
    }
}

//-----------------------------------------------------------------------------------------------------
uint8_t CDiscreteOutputLogQuery::LoadAnswerData(CSerialMT *pxCSerialMT, uint8_t ui8Address)
{

}

//-----------------------------------------------------------------------------------------------------
uint8_t CDiscreteOutputLogQuery::ReceiveProcessing(CSerialMT *pxCSerialMT, uint8_t ui8Address)
{
    if ((ui8Address & CSerialMT::SERIAL_MT_WRITE) != 0)
    {
        return 1;
    }
    else
    {
        CEvents::TEventsLogQueryListPackOne *pxSource;
        CEvents::TEventsLogQueryListPackOne *pxDestination;

        // получим указатель на место вложенного сообщения в общем сообщении.
        pxSource = (CEvents::TEventsLogQueryListPackOne*)pxCSerialMT -> pui8DataPointer;
        // получим указатель на рабочий массив со списком запросов событий.
        pxDestination =
            (CEvents::TEventsLogQueryListPackOne*)&CEvents::xEventsLogQueryList;
        // поместим принятый список запросов событий в рабочий массив.
        *pxDestination = *pxSource;
        return 0;
    }
}

//-----------------------------------------------------------------------------------------------------



//int16_t CAnalogueMeasureQuery::i16LastRequestIndex;
//-----------------------------------------------------------------------------------------------------
uint8_t CAnalogueMeasureQuery::TransmitProcessing(CSerialMT *pxCSerialMT, uint8_t ui8Address)
{
    uint8_t ui8Length;
    uint8_t *pui8Destination;
    // индекс запрашиваемого аналогового измерения.
    int16_t i16IndexQuery;
    // получим индекс первого запрашиваемого аналогового измерения.
    i16IndexQuery = (CEvents::xEventsLogQueryList.i16Index -
                     CArchiveEventsDB::EVENTS_DB_PREVIOUS_RECORDS_QUANTITY);

//        cout << "CAnalogueMeasureQuery i16IndexQuery" << i16IndexQuery << endl;
//        cout << "CAnalogueMeasureQuery i16LastRequestIndex" << i16LastRequestIndex << endl;
//    if ((i16IndexQuery != i16LastRequestIndex) ||
//            (i16IndexQuery == i16LastRequestIndex == 0))
    if ((i16IndexQuery != i16LastRequestIndex) ||
            (i16IndexQuery == -1))
    {
//        cout << "CAnalogueMeasureQuery TEventDataPackOne" << endl;
        // вычислим длину вложенного сообщения.
        ui8Length =
            (CEvents::xEventsLogQueryList.ui8Quantity *
             sizeof(struct CEvents::TEventDataPackOne));
    }
    else
    {
//            cout << "CAnalogueMeasureQuery TEventDataDinamicPackOne" << endl;
        // вычислим длину вложенного сообщения.
        ui8Length =
            (CEvents::xEventsLogQueryList.ui8Quantity *
             sizeof(struct CEvents::TEventDataDinamicPackOne));
    }

    // узнаем, есть ли место для сообщения.
    pui8Destination = pxCSerialMT -> MessagePushRequest(ui8Length,
                      ui8Address);
    // нет места для сообщеня?
    if (!pui8Destination)
    {
        return 0;
    }
    else
    {
        // создадим сообщение.
        // получим количество требуемых событий в запросе.
        ui8Length = CEvents::xEventsLogQueryList.ui8Quantity;
        // количество полученных событий превосходит
        // размер массива?
        if (ui8Length >
                (CEvents::REQUESTED_EVENTS_QUANTITY))
        {
            // ошибка.
            return 0;
        }

        float *pfSource;
        TAnalogueInputDescriptionWork *pxSource;
        TDimentionParameter *pxDimentionParameter;
        uint8_t *pui8StatAiByteArray;

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
        // запрос нового объекта?
//        if ((i16IndexQuery != i16LastRequestIndex) ||
//                (i16IndexQuery == i16LastRequestIndex == 0))
        if ((i16IndexQuery != i16LastRequestIndex) ||
                (i16IndexQuery == -1))
        {
//            cout << "CAnalogueMeasureQuery TEventDataPackOne" << endl;
            // сохраним текущий индекс запроса.
            i16LastRequestIndex = i16IndexQuery;
            CEvents::TEventDataPackOne *pxDestination;

            // получим указатель на место вложенного сообщения в общем сообщении.
            pxDestination = (CEvents::TEventDataPackOne*)(pui8Destination);
            // поместим данные событий во вложенное сообщение.
            for (uint16_t i = 0;
                    i < ui8Length;
                    i++)
            {
                // по требуемому адресу нет аналогового измерения?
                if ((i16IndexQuery) >=
                        (xPlcConfigService.xPlcConfigServiceData.
                         ucServiceAnalogueInputQuantity))
                {
                    // ошибка.
                    // получим тип события.
                    pxDestination[i].ui8Type = CEvents::HANDLED_EVENTS_ANALOGUE_TYPE;
                    // индекс запрашиваемой ошибки.
                    // для идентификации при поиске ошибки при выводе в МИНД.
                    pxDestination[i].i16Index =
                        (i16IndexQuery);
                    // пометим события маркером - нет события.
                    pxDestination[i].ui16ID = 0;
                }
                else
                {
                    uint8_t ui8Address;
                    uint8_t ui8Status;

                    if (i16IndexQuery >= 0)
                    {
                        ui8Address = (i16IndexQuery);
                    }
                    else
                    {
                        ui8Address = 0;
                    }

                    // получим адрес аналогового измерения.
                    pxDestination[i].ui8Address =
                        (ui8Address + CONVERT_INTEGER_TO_NATURAL_NUMBER);
                    // получим id события.
                    pxDestination[i].ui16ID = (ui8Address + CONVERT_INTEGER_TO_NATURAL_NUMBER);
                    // получим тип события.
                    pxDestination[i].ui8Type = CEvents::HANDLED_EVENTS_ANALOGUE_TYPE;
                    // индекс запрашиваемого события.
                    // для идентификации при поиске события при выводе в МИНД.
                    pxDestination[i].i16Index =
                        (i16IndexQuery);

                    // получим состояние аналогового измерения.
                    ui8Status =
                        pui8StatAiByteArray[ui8Address];

                    if( ui8Status & ANALOGUE_INPUT_LINE_BREAK )
                    {
                        pxDestination[i].ui8Status =
                            HMI_ANALOGUE_INPUT_LINE_BREAK;
                    }
                    else if( ui8Status & ANALOGUE_INPUT_CALCULATION_OVERFLOW )
                    {
                        pxDestination[i].ui8Status =
                            HMI_ANALOGUE_INPUT_CALCULATION_OVERFLOW;
                    }
                    else if( ui8Status & ANALOGUE_INPUT_CHANNEL_CALIBRATION )
                    {
                        pxDestination[i].ui8Status =
                            HMI_ANALOGUE_INPUT_CHANNEL_CALIBRATION;
                    }
                    else if( ( ui8Status & ANALOGUE_INPUT_SET_POINT_VIOLATION_MASK) ==
                             ANALOGUE_INPUT_SET_POINT_VIOLATION_L)
                    {
                        pxDestination[i].ui8Status =
                            HMI_ANALOGUE_INPUT_SET_POINT_VIOLATION_L;
                    }
                    else if( ( ui8Status & ANALOGUE_INPUT_SET_POINT_VIOLATION_MASK) ==
                             ANALOGUE_INPUT_SET_POINT_VIOLATION_LL_L)
                    {
                        pxDestination[i].ui8Status =
                            HMI_ANALOGUE_INPUT_SET_POINT_VIOLATION_LL_L;
                    }
                    else if( ( ui8Status & ANALOGUE_INPUT_SET_POINT_VIOLATION_MASK) ==
                             ANALOGUE_INPUT_SET_POINT_VIOLATION_H)
                    {
                        pxDestination[i].ui8Status =
                            HMI_ANALOGUE_INPUT_SET_POINT_VIOLATION_H;
                    }
                    else if( ( ui8Status & ANALOGUE_INPUT_SET_POINT_VIOLATION_MASK) ==
                             ANALOGUE_INPUT_SET_POINT_VIOLATION_HH_H)
                    {
                        pxDestination[i].ui8Status =
                            HMI_ANALOGUE_INPUT_SET_POINT_VIOLATION_HH_H;
                    }
                    else
                    {
                        pxDestination[i].ui8Status = 0;
                    }

                    // получим значение аналогового измерения.
                    pxDestination[i].fData =
                        pfSource[ui8Address];
                    // скопируем строку текстового реквизита.
                    memcpy(&(pxDestination[i].acTextDescriptor),
                           (&(pxSource[ui8Address].acTextDescriptor)),
                           (ANALOGUE_INPUT_NAME_LENGTH + END_OF_STRING_LENGTH));
                    // скопируем строку текстового реквизита размерности измеряемого параметра КРЗМ.
                    memcpy(&(pxDestination[i].acDimentionParameterText),
                           (&(pxDimentionParameter[(pxSource[ui8Address].ucDimentionCode)].acDimentionParameterText)),
                           (DIMENSIONS_PARAMETERS_NAME_LENGTH + END_OF_STRING_LENGTH));
                }
                // индекс следующего запрашиваемого события.
                i16IndexQuery++;
            }
        }
        else
        {
//            cout << "CAnalogueMeasureQuery TEventDataDinamicPackOne" << endl;
            CEvents::TEventDataDinamicPackOne *pxDestination;

            // получим указатель на место вложенного сообщения в общем сообщении.
            pxDestination = (CEvents::TEventDataDinamicPackOne*)(pui8Destination);
            // поместим данные событий во вложенное сообщение.
            for (uint16_t i = 0;
                    i < ui8Length;
                    i++)
            {
                // по требуемому адресу нет аналогового измерения?
                if ((i16IndexQuery) >=
                        (xPlcConfigService.xPlcConfigServiceData.
                         ucServiceAnalogueInputQuantity))
                {
                    // ошибка.
//                    // получим тип события.
//                    pxDestination[i].ui8Type = CEvents::HANDLED_EVENTS_ANALOGUE_TYPE;
                    // индекс запрашиваемой ошибки.
                    // для идентификации при поиске ошибки при выводе в МИНД.
                    pxDestination[i].i16Index =
                        (i16IndexQuery);
                    // пометим события маркером - нет события.
                    pxDestination[i].ui16ID = 0;
                }
                else
                {
                    uint8_t ui8Address;
                    uint8_t ui8Status;

                    if (i16IndexQuery >= 0)
                    {
                        ui8Address = (i16IndexQuery);
                    }
                    else
                    {
                        ui8Address = 0;
                    }

//                    // получим адрес аналогового измерения.
//                    pxDestination[i].ui8Address =
//                        ui8Address;
                    // получим id события.
                    pxDestination[i].ui16ID = (ui8Address + CONVERT_INTEGER_TO_NATURAL_NUMBER);
//                    // получим тип события.
//                    pxDestination[i].ui8Type = CEvents::HANDLED_EVENTS_ANALOGUE_TYPE;
                    // индекс запрашиваемого события.
                    // для идентификации при поиске события при выводе в МИНД.
                    pxDestination[i].i16Index =
                        (i16IndexQuery);

                    // получим состояние аналогового измерения.
                    ui8Status =
                        pui8StatAiByteArray[ui8Address];

                    if( ui8Status & ANALOGUE_INPUT_LINE_BREAK )
                    {
                        pxDestination[i].ui8Status =
                            HMI_ANALOGUE_INPUT_LINE_BREAK;
                    }
                    else if( ui8Status & ANALOGUE_INPUT_CALCULATION_OVERFLOW )
                    {
                        pxDestination[i].ui8Status =
                            HMI_ANALOGUE_INPUT_CALCULATION_OVERFLOW;
                    }
                    else if( ui8Status & ANALOGUE_INPUT_CHANNEL_CALIBRATION )
                    {
                        pxDestination[i].ui8Status =
                            HMI_ANALOGUE_INPUT_CHANNEL_CALIBRATION;
                    }
                    else if( ( ui8Status & ANALOGUE_INPUT_SET_POINT_VIOLATION_MASK) ==
                             ANALOGUE_INPUT_SET_POINT_VIOLATION_L)
                    {
                        pxDestination[i].ui8Status =
                            HMI_ANALOGUE_INPUT_SET_POINT_VIOLATION_L;
                    }
                    else if( ( ui8Status & ANALOGUE_INPUT_SET_POINT_VIOLATION_MASK) ==
                             ANALOGUE_INPUT_SET_POINT_VIOLATION_LL_L)
                    {
                        pxDestination[i].ui8Status =
                            HMI_ANALOGUE_INPUT_SET_POINT_VIOLATION_LL_L;
                    }
                    else if( ( ui8Status & ANALOGUE_INPUT_SET_POINT_VIOLATION_MASK) ==
                             ANALOGUE_INPUT_SET_POINT_VIOLATION_H)
                    {
                        pxDestination[i].ui8Status =
                            HMI_ANALOGUE_INPUT_SET_POINT_VIOLATION_H;
                    }
                    else if( ( ui8Status & ANALOGUE_INPUT_SET_POINT_VIOLATION_MASK) ==
                             ANALOGUE_INPUT_SET_POINT_VIOLATION_HH_H)
                    {
                        pxDestination[i].ui8Status =
                            HMI_ANALOGUE_INPUT_SET_POINT_VIOLATION_HH_H;
                    }
                    else
                    {
                        pxDestination[i].ui8Status = 0;
                    }

                    // получим значение аналогового измерения.
                    pxDestination[i].fData =
                        pfSource[ui8Address];
                }
                // индекс следующего запрашиваемого события.
                i16IndexQuery++;
            }
        }

        return 1;
    }
}

//-----------------------------------------------------------------------------------------------------
uint8_t CAnalogueMeasureQuery::LoadAnswerData(CSerialMT *pxCSerialMT, uint8_t ui8Address)
{

}

//-----------------------------------------------------------------------------------------------------
uint8_t CAnalogueMeasureQuery::ReceiveProcessing(CSerialMT *pxCSerialMT, uint8_t ui8Address)
{
    if ((ui8Address & CSerialMT::SERIAL_MT_WRITE) != 0)
    {
        return 1;
    }
    else
    {
        CEvents::TEventsLogQueryListPackOne *pxSource;
        CEvents::TEventsLogQueryListPackOne *pxDestination;

        // получим указатель на место вложенного сообщения в общем сообщении.
        pxSource = (CEvents::TEventsLogQueryListPackOne*)pxCSerialMT -> pui8DataPointer;
        // получим указатель на рабочий массив со списком запросов событий.
        pxDestination =
            (CEvents::TEventsLogQueryListPackOne*)&CEvents::xEventsLogQueryList;
        // поместим принятый список запросов событий в рабочий массив.
        *pxDestination = *pxSource;
        return 0;
    }
}

//-----------------------------------------------------------------------------------------------------





//-----------------------------------------------------------------------------------------------------
// функция событий информации о приборе.
// создаёт событие и помещает запись в журнал событий "кратко".
int8_t CInfoLogQuery::InfoEvetnsLogCreate(void)
{
    uint8_t ui8InfoEvetnsIndex;
    char acCommand[128];
    char* pcBashCommandAnswer;
    CEvents::TEventDataCommon *pxEventData;
    float fAverageSumm = 0;

    pxEventData = xCInfoEvent.EventGetBuffer();

    // пометим события маркером - нет события.
    for (int i = 0;
            i < (xCInfoEvent.ui16RecordedEventsQuantity);
            i++)
    {
        pxEventData[i].ui16ID = 0;
    }

    ui8InfoEvetnsIndex = 0;
//-----------------------------------------------------------------------------------------------------
    // сохраним данные события.
    // установим id события.
    (pxEventData -> ui16ID) =
        1;
    // установим тип события.
    (pxEventData -> ui8Type) =
        CEvents::HANDLED_EVENTS_INFO_TYPE;
    // создадим строку текстового реквизита.
    strcpy(pxEventData -> acTextDescriptor, "Common load");
    // создадим строку команды.
    strcpy(acCommand,
           "ps aux | awk '{s += $3} END {print s \"%\"}'");
    pcBashCommandAnswer = xCArchiveSaveParse.BashCommandHandler(acCommand);
    // получен ответ?
    if (pcBashCommandAnswer)
    {
        // преобразуем строку данных.
        pxEventData -> fData = atof(pcBashCommandAnswer);
        // создадим строку размерностей.
        strcpy(pxEventData -> acDimentionParameterText,
               "%");
    }
    else
    {
        // нет данных.
        pxEventData -> fData = 0;
        strcpy(pxEventData -> acDimentionParameterText,
               " %");
    }
    pxEventData++;
    ui8InfoEvetnsIndex++;

//-----------------------------------------------------------------------------------------------------
    // сохраним данные события.
    // установим id события.
    (pxEventData -> ui16ID) =
        1;
    // установим тип события.
    (pxEventData -> ui8Type) =
        CEvents::HANDLED_EVENTS_INFO_TYPE;
    // создадим строку текстового реквизита.
    strcpy(pxEventData -> acTextDescriptor, "PasNew load");
    // создадим строку команды.
    strcpy(acCommand,
           "ps aux | awk '/PasNew/ && /Sl/ && !/awk/ {print $3 \"%\"}'");
    pcBashCommandAnswer = xCArchiveSaveParse.BashCommandHandler(acCommand);
    // получен ответ?
    if (pcBashCommandAnswer)
    {
        // преобразуем строку данных.
        pxEventData -> fData = atof(pcBashCommandAnswer);
        // создадим строку размерностей.
        strcpy(pxEventData -> acDimentionParameterText,
               " %");
    }
    else
    {
        // нет данных.
        pxEventData -> fData = 0;
        strcpy(pxEventData -> acDimentionParameterText,
               "%");
    }

    pxEventData++;
    ui8InfoEvetnsIndex++;

//-----------------------------------------------------------------------------------------------------
    // сохраним данные события.
    // установим id события.
    (pxEventData -> ui16ID) =
        1;
    // установим тип события.
    (pxEventData -> ui8Type) =
        CEvents::HANDLED_EVENTS_INFO_TYPE;
    // создадим строку текстового реквизита.
    strcpy(pxEventData -> acTextDescriptor, "CodeSys load");
    // создадим строку команды.
    strcpy(acCommand,
           "ps aux | awk '/codesyscontrol/ && /Sl/ && !/awk/ {print $3 \"%\"}'");
    pcBashCommandAnswer = xCArchiveSaveParse.BashCommandHandler(acCommand);
    // получен ответ?
    if (pcBashCommandAnswer)
    {
        // преобразуем строку данных.
        pxEventData -> fData = atof(pcBashCommandAnswer);
        // создадим строку размерностей.
        strcpy(pxEventData -> acDimentionParameterText,
               " %");
    }
    else
    {
        // нет данных.
        pxEventData -> fData = 0;
        strcpy(pxEventData -> acDimentionParameterText,
               " %");
    }

    pxEventData++;
    ui8InfoEvetnsIndex++;

//-----------------------------------------------------------------------------------------------------
    // сохраним данные события.
    // установим id события.
    (pxEventData -> ui16ID) =
        1;
    // установим тип события.
    (pxEventData -> ui8Type) =
        CEvents::HANDLED_EVENTS_INFO_TYPE;
    // создадим строку текстового реквизита.
    strcpy(pxEventData -> acTextDescriptor, "Main cycle");

//    float fAverageSumm = 0;
//        float fIntermediate = 0;
    for (int i = 0; i < 64; i++)
    {
//            fIntermediate = (float)xCMainCycleMeasureRingBuffer.Pop();
//            printf("fIntermediate %6.2f\n\r", (fIntermediate));
//            fAverageSumm += fIntermediate;
        fAverageSumm += (float)xCMainCycleMeasureRingBuffer.Pop();
    }
    fAverageSumm /= 64;

    // преобразуем строку данных.
    pxEventData -> fData = fAverageSumm / 1000;
    // создадим строку размерностей.
    strcpy(pxEventData -> acDimentionParameterText,
           " mS");

    pxEventData++;
    ui8InfoEvetnsIndex++;

//-----------------------------------------------------------------------------------------------------
    // сохраним данные события.
    // установим id события.
    (pxEventData -> ui16ID) =
        1;
    // установим тип события.
    (pxEventData -> ui8Type) =
        CEvents::HANDLED_EVENTS_INFO_TYPE;
    // создадим строку текстового реквизита.
    strcpy(pxEventData -> acTextDescriptor, "CDS marker");

//    float fAverageSumm = 0;
//        for (int i = 0; i < 64; i++)
//        {
//            fAverageSumm += (float)xCCodeSysCycleMeasureRingBuffer.Pop();
//        }
//        fAverageSumm /= 64;

    fAverageSumm = afSysValue[0];

    // преобразуем строку данных.
    pxEventData -> fData = fAverageSumm;
//    // создадим строку размерностей.
//    strcpy(pxEventData -> acDimentionParameterText,
//           " mS");

    pxEventData++;
    ui8InfoEvetnsIndex++;

//-----------------------------------------------------------------------------------------------------
    // сохраним данные события.
    // установим id события.
    (pxEventData -> ui16ID) =
        1;
    // установим тип события.
    (pxEventData -> ui8Type) =
        CEvents::HANDLED_EVENTS_INFO_TYPE;
    // создадим строку текстового реквизита.
    strcpy(pxEventData -> acTextDescriptor, "CDS cyclelength");

//    float fAverageSumm = 0;
//        for (int i = 0; i < 64; i++)
//        {
//            fAverageSumm += (float)xCCodeSysCycleMeasureRingBuffer2.Pop();
//        }
//        fAverageSumm /= 64;

    fAverageSumm = afSysValue[1] / 1000;

    // преобразуем строку данных.
    pxEventData -> fData = fAverageSumm;
    // создадим строку размерностей.
    strcpy(pxEventData -> acDimentionParameterText,
           " mS");

    pxEventData++;
    ui8InfoEvetnsIndex++;

//-----------------------------------------------------------------------------------------------------
    // сохраним данные события.
    // установим id события.
    (pxEventData -> ui16ID) =
        1;
    // установим тип события.
    (pxEventData -> ui8Type) =
        CEvents::HANDLED_EVENTS_INFO_TYPE;
    // создадим строку текстового реквизита.
    strcpy(pxEventData -> acTextDescriptor, "CDS deltarun");

//    float fAverageSumm = 0;
//        for (int i = 0; i < 64; i++)
//        {
//            fAverageSumm += (float)xCCodeSysCycleMeasureRingBuffer3.Pop();
//        }
//        fAverageSumm /= 64;

    fAverageSumm = afSysValue[2] / 1000;

    // преобразуем строку данных.
    pxEventData -> fData = fAverageSumm;
    // создадим строку размерностей.
    strcpy(pxEventData -> acDimentionParameterText,
           " mS");
}

//-----------------------------------------------------------------------------------------------------
// функция событий информации о приборе.
// обновляет данные события и помещает запись в журнал событий "кратко".
int8_t CInfoLogQuery::InfoEvetnsLogUpdate(void)
{
    uint8_t ui8InfoEvetnsIndex;
    char acCommand[128];
    char* pcBashCommandAnswer;
    CEvents::TEventDataCommon *pxEventData;
    float fAverageSumm = 0;

    ui8InfoEvetnsIndex = 0;

    pxEventData = xCInfoEvent.EventGetBuffer();
//-----------------------------------------------------------------------------------------------------
    pxEventData++;
    ui8InfoEvetnsIndex++;

//-----------------------------------------------------------------------------------------------------
    pxEventData++;
    ui8InfoEvetnsIndex++;

//-----------------------------------------------------------------------------------------------------
    pxEventData++;
    ui8InfoEvetnsIndex++;

//-----------------------------------------------------------------------------------------------------
    pxEventData++;
    ui8InfoEvetnsIndex++;

//-----------------------------------------------------------------------------------------------------
//    struct tm *pxSourceCurrentTime;
//    // получим указатель на буфер с текущим временем в рабочем массиве прибора.
//    pxSourceCurrentTime =
//        (struct tm*)pxModuleContext ->
//        xModuleContextStatic.
//        pucCurrentTimeBufferPointer;
//
//    // поместим текущее время во вложенное сообщение.
//    fAverageSumm = (float)pxSourceCurrentTime -> tm_sec;
    fAverageSumm = afSysValue[0];

    // преобразуем строку данных.
    pxEventData -> fData = fAverageSumm;


    pxEventData++;
    ui8InfoEvetnsIndex++;

//-----------------------------------------------------------------------------------------------------
//    fAverageSumm = afSysValue[1] / 1000;
//    // преобразуем строку данных.
//    pxEventData -> fData = fAverageSumm;

    pxEventData++;
    ui8InfoEvetnsIndex++;

//-----------------------------------------------------------------------------------------------------
//    fAverageSumm = afSysValue[2] / 1000;
//
//    // преобразуем строку данных.
//    pxEventData -> fData = fAverageSumm;
//-----------------------------------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------------------------------
uint8_t CInfoLogQuery::TransmitProcessing(CSerialMT *pxCSerialMT, uint8_t ui8Address)
{
    uint8_t ui8Length;
    uint8_t *pui8Destination;
    // индекс запрашиваемого аналогового измерения.
    int16_t i16IndexQuery;
    CEvents::TEventDataCommon *pxSource;
    CEvents *pxCEvents;
    pxCEvents = &xCInfoEvent;

    // получим индекс первого запрашиваемого события.
    i16IndexQuery = (CEvents::xEventsLogQueryList.i16Index -
                     CArchiveEventsDB::EVENTS_DB_PREVIOUS_RECORDS_QUANTITY);
    // запрос нового объекта?
    if (i16IndexQuery != i16LastRequestIndex)
    {
        // передаются все данные, включая текстовое описание.
        // вычислим длину вложенного сообщения.
        ui8Length =
            (CEvents::xEventsLogQueryList.ui8Quantity *
             sizeof(struct CEvents::TEventDataPackOne));
    }
    else
    {
        // передаются частичные данные, изменяемые динамически.
        // вычислим длину вложенного сообщения.
        ui8Length =
            (CEvents::xEventsLogQueryList.ui8Quantity *
             sizeof(struct CEvents::TEventDataDinamicPackOne));
    }

    // узнаем, есть ли место для сообщения.
    pui8Destination = pxCSerialMT -> MessagePushRequest(ui8Length,
                      ui8Address);
    // нет места для сообщеня?
    if (!pui8Destination)
    {
        return 0;
    }
    else
    {
        // создадим сообщение.
        // получим количество требуемых событий в запросе.
        ui8Length = CEvents::xEventsLogQueryList.ui8Quantity;
        // количество полученных событий превосходит
        // размер массива?
        if (ui8Length >
                (CEvents::REQUESTED_EVENTS_QUANTITY))
        {
            // ошибка.
            return 0;
        }

        // запрашивается первая запись?
        if (!CEvents::xEventsLogQueryList.i16Index)
        {
            InfoEvetnsLogCreate();
        }
        else
        {
            InfoEvetnsLogUpdate();
        }

        // запрос нового объекта?
        if (i16IndexQuery != i16LastRequestIndex)
        {
            // сохраним текущий индекс запроса.
            i16LastRequestIndex = i16IndexQuery;
            CEvents::TEventDataPackOne *pxDestination;
            // установим индекс в массиве указателей на события в массиве даных событий.
            pxSource = (xCInfoEvent.EventGetBuffer() + i16IndexQuery);

            // получим указатель на место вложенного сообщения в общем сообщении.
            pxDestination = (CEvents::TEventDataPackOne*)(pui8Destination);
            // поместим данные событий во вложенное сообщение.
            for (uint16_t i = 0;
                    i < ui8Length;
                    i++)
            {
                // есть маркер - нет события?
                if ((pxSource -> ui16ID) == 0)
                {
                    // получим тип события.
                    pxDestination -> ui8Type =
                        pxSource -> ui8Type;
                    // индекс запрашиваемой ошибки.
                    // для идентификации при поиске ошибки при выводе в МИНД.
                    pxDestination -> i16Index =
                        (i16IndexQuery);
                    // пометим события маркером - нет события.
                    pxDestination -> ui16ID = 0;
                }
                else
                {
                    // получим id события.
                    pxDestination -> ui16ID =
                        pxSource -> ui16ID;
                    // получим тип события.
                    pxDestination -> ui8Type =
                        pxSource -> ui8Type;
                    // индекс запрашиваемого события.
                    // для идентификации при поиске события при выводе в МИНД.
                    pxDestination -> i16Index =
                        (i16IndexQuery);
                    // получим значение события.
                    pxDestination -> fData =
                        pxSource -> fData;
                    // скопируем строку размерностей.
                    memcpy(&(pxDestination -> acDimentionParameterText),
                           (&(pxSource -> acDimentionParameterText)),
                           (DIMENSIONS_PARAMETERS_NAME_LENGTH + END_OF_STRING_LENGTH));
                    // скопируем строку текстового реквизита.
                    memcpy(&(pxDestination -> acTextDescriptor),
                           (&(pxSource -> acTextDescriptor)),
                           (CEvents::EVENT_NAME_LENGTH + END_OF_STRING_LENGTH));
                }
                pxSource++;
                pxDestination++;
                // индекс следующего запрашиваемого события.
                i16IndexQuery++;
            }
        }
        else
        {
            CEvents::TEventDataDinamicPackOne *pxDestination;

            // установим индекс в массиве указателей на события в массиве даных событий.
            pxSource = (xCInfoEvent.EventGetBuffer() + i16IndexQuery);
            // получим указатель на место вложенного сообщения в общем сообщении.
            pxDestination = (CEvents::TEventDataDinamicPackOne*)(pui8Destination);
            // поместим данные событий во вложенное сообщение.
            for (uint16_t i = 0;
                    i < ui8Length;
                    i++)
            {
                // есть маркер - нет события?
                if ((pxSource -> ui16ID) == 0)
                {
                    // индекс запрашиваемой ошибки.
                    // для идентификации при поиске ошибки при выводе в МИНД.
                    pxDestination -> i16Index =
                        (i16IndexQuery);
                    // пометим события маркером - нет события.
                    pxDestination -> ui16ID = 0;
                }
                else
                {
                    // получим id события.
                    pxDestination -> ui16ID =
                        pxSource -> ui16ID;
                    // индекс запрашиваемого события.
                    // для идентификации при поиске события при выводе в МИНД.
                    pxDestination -> i16Index =
                        (i16IndexQuery);
                    // получим значение события.
                    pxDestination -> fData =
                        pxSource -> fData;
                }
                pxSource++;
                pxDestination++;
                // индекс следующего запрашиваемого события.
                i16IndexQuery++;
            }
        }

        return 1;
    }
}

//-----------------------------------------------------------------------------------------------------
uint8_t CInfoLogQuery::LoadAnswerData(CSerialMT *pxCSerialMT, uint8_t ui8Address)
{

}

//-----------------------------------------------------------------------------------------------------
uint8_t CInfoLogQuery::ReceiveProcessing(CSerialMT *pxCSerialMT, uint8_t ui8Address)
{
    if ((ui8Address & CSerialMT::SERIAL_MT_WRITE) != 0)
    {
        return 1;
    }
    else
    {
        CEvents::TEventsLogQueryListPackOne *pxSource;
        CEvents::TEventsLogQueryListPackOne *pxDestination;

        // получим указатель на место вложенного сообщения в общем сообщении.
        pxSource = (CEvents::TEventsLogQueryListPackOne*)pxCSerialMT -> pui8DataPointer;
        // получим указатель на рабочий массив со списком запросов событий.
        pxDestination =
            (CEvents::TEventsLogQueryListPackOne*)&CEvents::xEventsLogQueryList;
        // поместим принятый список запросов событий в рабочий массив.
        *pxDestination = *pxSource;
        return 0;
    }
}

//-----------------------------------------------------------------------------------------------------





//-----------------------------------------------------------------------------------------------------
uint8_t CConfigurationQueryEW::TransmitProcessing(CSerialMT *pxCSerialMT, uint8_t ui8Address)
{
    uint8_t ui8Length;
    uint8_t *pui8Destination;
    // вычислим длину вложенного сообщения.
    // размер объекта конфигурации плюс один байт - адрес прибора.
    ui8Length =
        (sizeof(struct TConfigDataPackOne) + 1);
    // узнаем, есть ли место для сообщения.
    pui8Destination = pxCSerialMT -> MessagePushRequest(ui8Length,
                      ui8Address);
    // нет места для сообщеня?
    if (!pui8Destination)
    {
        return 0;
    }
    else
    {
        // создадим сообщение.
        // получим указатель на место для вложенного сообщения в общем сообщении.
        // поместим Modbus адрес прибора во вложенное сообщение.
        *(pui8Destination) = ucModbusSlaveAddress;
        // поместим конфигурацию прибора во вложенное сообщение.
        vConfigurationDataPack((TConfigDataPackOne*)((pui8Destination) + 1),
                               (TConfigData*)pxModuleContext ->
                               xModuleContextStatic.
                               pucPlcConfigDataTempPointer);
        return 1;
    }
}

//-----------------------------------------------------------------------------------------------------
uint8_t CConfigurationQueryEW::LoadAnswerData(CSerialMT *pxCSerialMT, uint8_t ui8Address)
{

}

//-----------------------------------------------------------------------------------------------------
uint8_t CConfigurationQueryEW::ReceiveProcessing(CSerialMT *pxCSerialMT, uint8_t ui8Address)
{
    if ((ui8Address & CSerialMT::SERIAL_MT_WRITE) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

//-----------------------------------------------------------------------------------------------------



//-----------------------------------------------------------------------------------------------------
uint8_t CCurrentTimeQueryEW::TransmitProcessing(CSerialMT *pxCSerialMT, uint8_t ui8Address)
{
    uint8_t ui8Length;
    uint8_t *pui8Destination;
    // вычислим длину вложенного сообщения.
    ui8Length = CURRENT_TIME_BYTE_QUANTITY;
    // узнаем, есть ли место для сообщения.
    pui8Destination = pxCSerialMT -> MessagePushRequest(ui8Length,
                      ui8Address);
    // нет места для сообщеня?
    if (!pui8Destination)
    {
        return 0;
    }
    else
    {
        struct tm *pxSource;
        // получим указатель на буфер с текущим временем в рабочем массиве прибора.
        pxSource =
            (struct tm*)pxModuleContext ->
            xModuleContextStatic.
            pucCurrentTimeBufferPointer;

        // поместим текущее время во вложенное сообщение.
        pui8Destination[CURRENT_TIME_SECOND_OFFSET] = pxSource -> tm_sec;
        pui8Destination[CURRENT_TIME_MINUTE_OFFSET] = pxSource -> tm_min;
        pui8Destination[CURRENT_TIME_HOUR_OFFSET] = pxSource -> tm_hour;
        pui8Destination[CURRENT_TIME_MONTH_DAY_OFFSET] = pxSource -> tm_mday;
        pui8Destination[CURRENT_TIME_MONTH_OFFSET] = (pxSource -> tm_mon) + 1;
        pui8Destination[CURRENT_TIME_YEAR_OFFSET] = (pxSource -> tm_year) - 100;
        pui8Destination[CURRENT_TIME_WEEK_DAY_OFFSET] = pxSource -> tm_wday;
        return 1;
    }
}

//-----------------------------------------------------------------------------------------------------
uint8_t CCurrentTimeQueryEW::LoadAnswerData(CSerialMT *pxCSerialMT, uint8_t ui8Address)
{

}

//-----------------------------------------------------------------------------------------------------
uint8_t CCurrentTimeQueryEW::ReceiveProcessing(CSerialMT *pxCSerialMT, uint8_t ui8Address)
{
    if ((ui8Address & CSerialMT::SERIAL_MT_WRITE) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

//-----------------------------------------------------------------------------------------------------



//-----------------------------------------------------------------------------------------------------
uint8_t CCurrentTimeSetEW::TransmitProcessing(CSerialMT *pxCSerialMT, uint8_t ui8Address)
{
    uint8_t ui8Length;
    uint8_t *pui8Destination;
    // вычислим длину вложенного сообщения.
    ui8Length = TASK_HIGH_LEVEL_FLAGS_MESSAGE_LENGTH;
    // узнаем, есть ли место для сообщения.
    pui8Destination = pxCSerialMT -> MessagePushRequest(ui8Length,
                      ui8Address);
    // нет места для сообщеня?
    if (!pui8Destination)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

//-----------------------------------------------------------------------------------------------------
uint8_t CCurrentTimeSetEW::LoadAnswerData(CSerialMT *pxCSerialMT, uint8_t ui8Address)
{

}

//-----------------------------------------------------------------------------------------------------
uint8_t CCurrentTimeSetEW::ReceiveProcessing(CSerialMT *pxCSerialMT, uint8_t ui8Address)
{
    if ((ui8Address & CSerialMT::SERIAL_MT_WRITE) != 0)
    {
        // получим указатель на место вложенного сообщения в общем сообщении.
        // получим и установим текущее время.
        vLinuxCurrentTimeSet((unsigned char*)(pxCSerialMT -> pui8DataPointer));
        return 1;
    }
    else
    {
        return 0;
    }
}

//-----------------------------------------------------------------------------------------------------




//-----------------------------------------------------------------------------------------------------
const char *CSettingsRW::pccSettingsFileName;
CSettingsRW::TPlcSettingsPackOne CSettingsRW::xTPlcSettingsPackOne;
CSettingsRW::TPlcSettingsQueryPackOne CSettingsRW::xTPlcSettingsQueryPackOne;
CSettingsRW::TPlcSettingsExchangePackOne CSettingsRW::xTPlcSettingsExchangePackOne;

uint8_t CSettingsRW::ui8ModbusRtuDataBits;
uint8_t CSettingsRW::ui8ModbusRtuStopBits;
uint32_t CSettingsRW::ui32ModbusRtuBaudRate;
char CSettingsRW::cModbusRtuParity;

uint8_t CSettingsRW::ui8ModbusRtuMasterDataBits;
uint8_t CSettingsRW::ui8ModbusRtuMasterStopBits;
uint32_t CSettingsRW::ui32ModbusRtuMasterBaudRate;
char CSettingsRW::cModbusRtuMasterParity;

char CSettingsRW::acModbusTcpIP[ETHERNET_IP_TEXT_LENGTH];
uint16_t CSettingsRW::ui16ModbusTcpPort;
uint8_t CSettingsRW::ui8ArchiveFileIsSaveState;
uint8_t CSettingsRW::ui8DebugLogFileSaveState;

CSettingsRW::TPlcSettingsPackOne CSettingsRW::xTPlcSettingsDefaultPackOne =
{
    {
        0,
        1,
        0
    },

    {
        0,
        1,
        0
    },

    {
        192,
        168,
        0,
        9,
        502
    },

    {
        1
    }
};

//-----------------------------------------------------------------------------------------------------
uint8_t CSettingsRW::TransmitProcessing(CSerialMT *pxCSerialMT, uint8_t ui8Address)
{
    TPlcSettingsExchangePackOne *pxDestination;
    uint8_t ui8Length;
    uint8_t *pui8Destination;

    if ((ui8Address & CSerialMT::SERIAL_MT_WRITE) != 0)
    {
        // создадим сообщение.
        switch (xTPlcSettingsQueryPackOne.ui8Type)
        {
        case RS485_HIGH_LEVEL_SET:
            // вычислим длину вложенного сообщения.
            ui8Length = TASK_CONFIRMATION_LENGTH;
            // узнаем, есть ли место для сообщения.
            pui8Destination = pxCSerialMT -> MessagePushRequest(ui8Length,
                              ui8Address);
            // нет места для сообщеня?
            if (!pui8Destination)
            {
                return 0;
            }
            else
            {
                // создадим сообщение.
                (*pui8Destination) = 0;
                return 1;
            }
            break;
        case ARCHIVE_LOG_SAVE:
            // количество отправляемых данных.
            ui8Length = sizeof(struct TPlcSettingsExchangePackOne);
            // узнаем, есть ли место для сообщения.
            pui8Destination = pxCSerialMT -> MessagePushRequest(ui8Length,
                              ui8Address);
            // нет места для сообщеня?
            if (!pui8Destination)
            {
                return 0;
            }
            else
            {
                TPlcSettingsExchangePackOne *pxSource;
                pxSource = &xTPlcSettingsExchangePackOne;
                uint8_t ui8ChoiceOption;
                // получим указатель на место вложенного сообщения в общем сообщении.
                pxDestination = (TPlcSettingsExchangePackOne*)(pui8Destination);
                // поместим данные во вложенное сообщение.
                pxDestination -> ui8Type = ARCHIVE_LOG_SAVE;
                pxDestination -> ui8ChoiceOption = pxSource -> ui8ChoiceOption;
                pxDestination -> ui8AnswerCode = pxSource -> ui8AnswerCode;
                return 1;
            }
            break;
        case PROGRAMM_FILE_UPDATE:
            // количество отправляемых данных.
            ui8Length = sizeof(struct TPlcSettingsExchangePackOne);
            // узнаем, есть ли место для сообщения.
            pui8Destination = pxCSerialMT -> MessagePushRequest(ui8Length,
                              ui8Address);
            // нет места для сообщеня?
            if (!pui8Destination)
            {
                return 0;
            }
            else
            {
                TPlcSettingsExchangePackOne *pxSource;
                pxSource = &xTPlcSettingsExchangePackOne;
                uint8_t ui8ChoiceOption;
                // получим указатель на место вложенного сообщения в общем сообщении.
                pxDestination = (TPlcSettingsExchangePackOne*)(pui8Destination);
                // поместим данные во вложенное сообщение.
                pxDestination -> ui8Type = PROGRAMM_FILE_UPDATE;
                pxDestination -> ui8ChoiceOption = pxSource -> ui8ChoiceOption;
                pxDestination -> ui8AnswerCode = pxSource -> ui8AnswerCode;
                return 1;
            }
            break;
        case DEBUG_LOG_FILE_SAVE:
            // количество отправляемых данных.
            ui8Length = sizeof(struct TPlcSettingsExchangePackOne);
            // узнаем, есть ли место для сообщения.
            pui8Destination = pxCSerialMT -> MessagePushRequest(ui8Length,
                              ui8Address);
            // нет места для сообщеня?
            if (!pui8Destination)
            {
                return 0;
            }
            else
            {
                TPlcSettingsExchangePackOne *pxSource;
                pxSource = &xTPlcSettingsExchangePackOne;
                uint8_t ui8ChoiceOption;
                // получим указатель на место вложенного сообщения в общем сообщении.
                pxDestination = (TPlcSettingsExchangePackOne*)(pui8Destination);
                // поместим данные во вложенное сообщение.
                pxDestination -> ui8Type = DEBUG_LOG_FILE_SAVE;
                pxDestination -> ui8ChoiceOption = pxSource -> ui8ChoiceOption;
                pxDestination -> ui8AnswerCode = pxSource -> ui8AnswerCode;
                return 1;
            }
            break;
        case ETHERNET_SET:
            // вычислим длину вложенного сообщения.
            ui8Length = TASK_CONFIRMATION_LENGTH;
            // узнаем, есть ли место для сообщения.
            pui8Destination = pxCSerialMT -> MessagePushRequest(ui8Length,
                              ui8Address);
            // нет места для сообщеня?
            if (!pui8Destination)
            {
                return 0;
            }
            else
            {
                // создадим сообщение.
                (*pui8Destination) = 0;
                return 1;
            }
            break;

        default:
            break;
        }
        return 1;
    }
    else
    {
        // создадим сообщение.
        switch (xTPlcSettingsQueryPackOne.ui8Type)
        {
        case RS485_HIGH_LEVEL_SET:
            // количество отправляемых данных.
            ui8Length = sizeof(struct TPlcSettingsExchangePackOne);
            // узнаем, есть ли место для сообщения.
            pui8Destination = pxCSerialMT -> MessagePushRequest(ui8Length,
                              ui8Address);
            // нет места для сообщеня?
            if (!pui8Destination)
            {
                return 0;
            }
            else
            {
                TPortSettingsPackOne *pxSource;
                // получим указатель на данные.
                pxSource = (TPortSettingsPackOne*)&xTPlcSettingsPackOne.xTRs485HighLevelSettingsPackOne;
                // получим указатель на место вложенного сообщения в общем сообщении.
                pxDestination = (TPlcSettingsExchangePackOne*)(pui8Destination);
                // поместим данные во вложенное сообщение.
                pxDestination -> ui8Type = RS485_HIGH_LEVEL_SET;
                pxDestination -> ui8BaudRate = pxSource -> ui8BaudRate;
                pxDestination -> ui8StopBits = pxSource -> ui8StopBits;
                pxDestination -> ui8Parity = pxSource -> ui8Parity;
                return 1;
            }
            break;
        case ARCHIVE_LOG_SAVE:
            // количество отправляемых данных.
            ui8Length = sizeof(struct TPlcSettingsExchangePackOne);
            // узнаем, есть ли место для сообщения.
            pui8Destination = pxCSerialMT -> MessagePushRequest(ui8Length,
                              ui8Address);
            // нет места для сообщеня?
            if (!pui8Destination)
            {
                return 0;
            }
            else
            {
                xCArchiveSaveParse.GetDiskInfo();
                // получим указатель на место вложенного сообщения в общем сообщении.
                pxDestination = (TPlcSettingsExchangePackOne*)(pui8Destination);
                // поместим данные во вложенное сообщение.
                pxDestination -> ui8Type = ARCHIVE_LOG_SAVE;
                pxDestination -> ui8ChoiceOption = 0;
                // создадим строки предложения имеющихся дисков для меню.
                for (uint8_t i = 0, j = 0, k = 0, l = 0;
                        (i < CHOICE_OPTION_TEXT_NUMBER);
                        i++)
                {
                    // есть USB диск?
                    if (memcmp((xCArchiveSaveParse.axTDiskInfo[i].acName),
                               "sd",
                               2) == 0)
                    {
                        // создадим строку с именем и ёмкостью имеющегося USB диска.
                        sprintf((char*)(pxDestination -> axChoiceOptionTextData[j].acChoiceOptionTextData),
                                ("%s%d %s"),
                                ("USB"),
                                k,
                                (xCArchiveSaveParse.axTDiskInfo[i].acSize));
                        // следующий индекс в имени диска.
                        k++;
                        // следующий диск.
                        j++;
                    }
                    // есть SD карта?
                    else if (memcmp((xCArchiveSaveParse.axTDiskInfo[i].acName),
                                    "mmc",
                                    3) == 0)
                    {
                        // создадим строку с именем и ёмкостью имеющейся SD карты.
                        sprintf((char*)(pxDestination -> axChoiceOptionTextData[j].acChoiceOptionTextData),
                                ("%s%d %s"),
                                ("SD"),
                                l,
                                (xCArchiveSaveParse.axTDiskInfo[i].acSize));
                        // следующий индекс в имени диска.
                        l++;
                        // следующий диск.
                        j++;
                    }
                    // поместим количество имеющихся дисков.
                    pxDestination -> ui8ChoiceOptionNumber = j;
                }
                return 1;
            }
            break;
        case PROGRAMM_FILE_UPDATE:
            // количество отправляемых данных.
            ui8Length = sizeof(struct TPlcSettingsExchangePackOne);
            // узнаем, есть ли место для сообщения.
            pui8Destination = pxCSerialMT -> MessagePushRequest(ui8Length,
                              ui8Address);
            // нет места для сообщеня?
            if (!pui8Destination)
            {
                return 0;
            }
            else
            {
                xCArchiveSaveParse.GetDiskInfo();
//                xCArchiveSaveParse.GetDiskInfoNew();

                // получим указатель на место вложенного сообщения в общем сообщении.
                pxDestination = (TPlcSettingsExchangePackOne*)(pui8Destination);
                // поместим данные во вложенное сообщение.
                pxDestination -> ui8Type = PROGRAMM_FILE_UPDATE;
                pxDestination -> ui8ChoiceOption = 0;
                // создадим строки предложения имеющихся дисков для меню.
                for (uint8_t i = 0, j = 0, k = 0, l = 0;
                        (i < CHOICE_OPTION_TEXT_NUMBER);
                        i++)
                {
                    // есть USB диск?
                    if (memcmp((xCArchiveSaveParse.axTDiskInfo[i].acName),
                               "sd",
                               2) == 0)
                    {
                        // создадим строку с именем и ёмкостью имеющегося USB диска.
                        sprintf((char*)(pxDestination -> axChoiceOptionTextData[j].acChoiceOptionTextData),
                                ("%s%d %s"),
                                ("USB"),
                                k,
                                (xCArchiveSaveParse.axTDiskInfo[i].acSize));
                        // следующий индекс в имени диска.
                        k++;
                        // следующий диск.
                        j++;
                    }
                    // есть SD карта?
                    else if (memcmp((xCArchiveSaveParse.axTDiskInfo[i].acName),
                                    "mmc",
                                    3) == 0)
                    {
                        // создадим строку с именем и ёмкостью имеющейся SD карты.
                        sprintf((char*)(pxDestination -> axChoiceOptionTextData[j].acChoiceOptionTextData),
                                ("%s%d %s"),
                                ("SD"),
                                l,
                                (xCArchiveSaveParse.axTDiskInfo[i].acSize));
                        // следующий индекс в имени диска.
                        l++;
                        // следующий диск.
                        j++;
                    }
                    // поместим количество имеющихся дисков.
                    pxDestination -> ui8ChoiceOptionNumber = j;
                }
                return 1;
            }
            break;
        case DEBUG_LOG_FILE_SAVE:
            // количество отправляемых данных.
            ui8Length = sizeof(struct TPlcSettingsExchangePackOne);
            // узнаем, есть ли место для сообщения.
            pui8Destination = pxCSerialMT -> MessagePushRequest(ui8Length,
                              ui8Address);
            // нет места для сообщеня?
            if (!pui8Destination)
            {
                return 0;
            }
            else
            {
                xCArchiveSaveParse.GetDiskInfo();
                // получим указатель на место вложенного сообщения в общем сообщении.
                pxDestination = (TPlcSettingsExchangePackOne*)(pui8Destination);
                // поместим данные во вложенное сообщение.
                pxDestination -> ui8Type = DEBUG_LOG_FILE_SAVE;
                pxDestination -> ui8ChoiceOption = 0;
                // создадим строки предложения имеющихся дисков для меню.
                for (uint8_t i = 0, j = 0, k = 0, l = 0;
                        (i < CHOICE_OPTION_TEXT_NUMBER);
                        i++)
                {
                    // есть USB диск?
                    if (memcmp((xCArchiveSaveParse.axTDiskInfo[i].acName),
                               "sd",
                               2) == 0)
                    {
                        // создадим строку с именем и ёмкостью имеющегося USB диска.
                        sprintf((char*)(pxDestination -> axChoiceOptionTextData[j].acChoiceOptionTextData),
                                ("%s%d %s"),
                                ("USB"),
                                k,
                                (xCArchiveSaveParse.axTDiskInfo[i].acSize));
                        // следующий индекс в имени диска.
                        k++;
                        // следующий диск.
                        j++;
                    }
                    // есть SD карта?
                    else if (memcmp((xCArchiveSaveParse.axTDiskInfo[i].acName),
                                    "mmc",
                                    3) == 0)
                    {
                        // создадим строку с именем и ёмкостью имеющейся SD карты.
                        sprintf((char*)(pxDestination -> axChoiceOptionTextData[j].acChoiceOptionTextData),
                                ("%s%d %s"),
                                ("SD"),
                                l,
                                (xCArchiveSaveParse.axTDiskInfo[i].acSize));
                        // следующий индекс в имени диска.
                        l++;
                        // следующий диск.
                        j++;
                    }
                    // поместим количество имеющихся дисков.
                    pxDestination -> ui8ChoiceOptionNumber = j;
                }
                return 1;
            }
            break;
        case ETHERNET_SET:
            TEthernetSettingsPackOne *pxTEthernetSettingsPackOneSource;
            // количество отправляемых данных.
            ui8Length = sizeof(struct TPlcSettingsExchangePackOne);
            // узнаем, есть ли место для сообщения.
            pui8Destination = pxCSerialMT -> MessagePushRequest(ui8Length,
                              ui8Address);
            // нет места для сообщеня?
            if (!pui8Destination)
            {
                return 0;
            }
            else
            {
                // получим указатель на данные.
                pxTEthernetSettingsPackOneSource = (TEthernetSettingsPackOne*)&xTPlcSettingsPackOne.xTEthernetSettingsPackOne;
                // получим указатель на место вложенного сообщения в общем сообщении.
                pxDestination = (TPlcSettingsExchangePackOne*)(pui8Destination);
                // поместим данные во вложенное сообщение.
                pxDestination -> ui8Type = ETHERNET_SET;
                pxDestination -> ui8IpByte0 = pxTEthernetSettingsPackOneSource -> ui8IpByte0;
                pxDestination -> ui8IpByte1 = pxTEthernetSettingsPackOneSource -> ui8IpByte1;
                pxDestination -> ui8IpByte2 = pxTEthernetSettingsPackOneSource -> ui8IpByte2;
                pxDestination -> ui8IpByte3 = pxTEthernetSettingsPackOneSource -> ui8IpByte3;
                pxDestination -> ui16Port = pxTEthernetSettingsPackOneSource -> ui16Port;
                return 1;
            }
            break;

        default:
            break;
        }
    }
}

pthread_t xArchiveFileSave;
//-----------------------------------------------------------------------------------------------------
// поток копирования диска mmc0 на mmc1.
void *thread_ArchiveFileSave(void *value)
{
    CSettingsRW::ArchiveSave(CSettingsRW::xTPlcSettingsExchangePackOne.ui8ChoiceOption);
}

pthread_t xDebugLogFileSave;
//-----------------------------------------------------------------------------------------------------
// поток копирования диска mmc0 на mmc1.
void *thread_DebugLogFileSave(void *value)
{
    CSettingsRW::DebugLogFileSave(CSettingsRW::xTPlcSettingsExchangePackOne.ui8ChoiceOption);
}

//-----------------------------------------------------------------------------------------------------
uint8_t CSettingsRW::ReceiveProcessing(CSerialMT *pxCSerialMT, uint8_t ui8Address)
{
    if ((ui8Address & CSerialMT::SERIAL_MT_WRITE) != 0)
    {
        // получим данные.
        TPlcSettingsExchangeClientPackOne *pxSource;

        // получим указатель на место вложенного сообщения в общем сообщении.
        pxSource = (TPlcSettingsExchangeClientPackOne*)(pxCSerialMT -> pui8DataPointer);

        switch (pxSource -> ui8Type)
        {
        case RS485_HIGH_LEVEL_SET:
            TPortSettingsPackOne *pxDestination;

            // получим указатель на данные.
            pxDestination = (TPortSettingsPackOne*)&xTPlcSettingsPackOne.xTRs485HighLevelSettingsPackOne;
            // получим данные.
            pxDestination -> ui8BaudRate = pxSource -> ui8BaudRate;
            pxDestination -> ui8StopBits = pxSource -> ui8StopBits;
            pxDestination -> ui8Parity = pxSource -> ui8Parity;
            PlcSettingsSave();
            PlcSettingsLoad();
			// обновим настройки интерфейса ModbusRtu, связь с локальной сетью нижнего уровня, RS-485.
			iModbusRtuMasterReset();
			// отправим сигнал потоку.
            // для вывода из состояния ожидания данных.
            // чтобы обновить настройки интерфейса.
            pthread_kill(xModbusRtu, SIGRTMIN+1);
            break;
        case ARCHIVE_LOG_SAVE:
            // получим данные.
            xTPlcSettingsExchangePackOne.ui8Type = pxSource -> ui8Type;
            xTPlcSettingsExchangePackOne.ui8ChoiceOption = pxSource -> ui8ChoiceOption;
            xTPlcSettingsExchangePackOne.ui8ChoiceOptionNumber = pxSource -> ui8ChoiceOptionNumber;
            xTPlcSettingsExchangePackOne.ui8AnswerCode = pxSource -> ui8AnswerCode;

            switch (ui8ArchiveFileIsSaveState)
            {
            case WRITE_IDDLE:
                // создадим поток сохранения файла архива.
                // create thread, pass reference, addr of the function and data
                if (pthread_create(&xArchiveFileSave,
                                   NULL,
                                   thread_ArchiveFileSave,
                                   NULL))
                {
                    cout << "Failed to create the thread_ArchiveFileSave" << endl;
                    xTPlcSettingsExchangePackOne.ui8AnswerCode = WRITE_ERROR;
                }
                else
                {
                    ui8ArchiveFileIsSaveState = WRITE_BUSY;
                    xTPlcSettingsExchangePackOne.ui8AnswerCode = WRITE_BUSY;
                }
                break;
            case WRITE_BUSY:
                xTPlcSettingsExchangePackOne.ui8AnswerCode = WRITE_BUSY;
                break;
            case WRITE_OK:
                xTPlcSettingsExchangePackOne.ui8AnswerCode = WRITE_OK;
                ui8ArchiveFileIsSaveState = WRITE_IDDLE;
                break;
            case WRITE_ERROR:
                xTPlcSettingsExchangePackOne.ui8AnswerCode = WRITE_ERROR;
                ui8ArchiveFileIsSaveState = WRITE_IDDLE;
                break;
            default:
                xTPlcSettingsExchangePackOne.ui8AnswerCode = WRITE_ERROR;
                ui8ArchiveFileIsSaveState = WRITE_IDDLE;
                break;
            }
            break;
        case PROGRAMM_FILE_UPDATE:
            // получим данные.
            xTPlcSettingsExchangePackOne.ui8Type = pxSource -> ui8Type;
            xTPlcSettingsExchangePackOne.ui8ChoiceOption = pxSource -> ui8ChoiceOption;
            xTPlcSettingsExchangePackOne.ui8ChoiceOptionNumber = pxSource -> ui8ChoiceOptionNumber;
            xTPlcSettingsExchangePackOne.ui8AnswerCode = pxSource -> ui8AnswerCode;
            // архив сохранён успешно?
            if (ProgrammFileUpdate(xTPlcSettingsExchangePackOne.ui8ChoiceOption))
            {
                xTPlcSettingsExchangePackOne.ui8AnswerCode = WRITE_OK;
            }
            else
            {
                xTPlcSettingsExchangePackOne.ui8AnswerCode = WRITE_ERROR;
            }
            break;
        case DEBUG_LOG_FILE_SAVE:
            // получим данные.
            xTPlcSettingsExchangePackOne.ui8Type = pxSource -> ui8Type;
            xTPlcSettingsExchangePackOne.ui8ChoiceOption = pxSource -> ui8ChoiceOption;
            xTPlcSettingsExchangePackOne.ui8ChoiceOptionNumber = pxSource -> ui8ChoiceOptionNumber;
            xTPlcSettingsExchangePackOne.ui8AnswerCode = pxSource -> ui8AnswerCode;

            switch (ui8DebugLogFileSaveState)
            {
            case WRITE_IDDLE:
                // создадим поток сохранения файла.
                // create thread, pass reference, addr of the function and data
                if (pthread_create(&xDebugLogFileSave,
                                   NULL,
                                   thread_DebugLogFileSave,
                                   NULL))
                {
                    cout << "Failed to create the thread_DebugLogFileSave" << endl;
                    xTPlcSettingsExchangePackOne.ui8AnswerCode = WRITE_ERROR;
                }
                else
                {
                    ui8DebugLogFileSaveState = WRITE_BUSY;
                    xTPlcSettingsExchangePackOne.ui8AnswerCode = WRITE_BUSY;
                }
                break;
            case WRITE_BUSY:
                xTPlcSettingsExchangePackOne.ui8AnswerCode = WRITE_BUSY;
                break;
            case WRITE_OK:
                xTPlcSettingsExchangePackOne.ui8AnswerCode = WRITE_OK;
                ui8DebugLogFileSaveState = WRITE_IDDLE;
                break;
            case WRITE_ERROR:
                xTPlcSettingsExchangePackOne.ui8AnswerCode = WRITE_ERROR;
                ui8DebugLogFileSaveState = WRITE_IDDLE;
                break;
            default:
                xTPlcSettingsExchangePackOne.ui8AnswerCode = WRITE_ERROR;
                ui8DebugLogFileSaveState = WRITE_IDDLE;
                break;
            }
            break;
        case ETHERNET_SET:
            TEthernetSettingsPackOne *pxTEthernetSettingsPackOneDestination;

            // получим указатель на данные.
            pxTEthernetSettingsPackOneDestination = (TEthernetSettingsPackOne*)&xTPlcSettingsPackOne.xTEthernetSettingsPackOne;
            // получим данные.
            pxTEthernetSettingsPackOneDestination -> ui8IpByte0 = pxSource -> ui8IpByte0;
            pxTEthernetSettingsPackOneDestination -> ui8IpByte1 = pxSource -> ui8IpByte1;
            pxTEthernetSettingsPackOneDestination -> ui8IpByte2 = pxSource -> ui8IpByte2;
            pxTEthernetSettingsPackOneDestination -> ui8IpByte3 = pxSource -> ui8IpByte3;
            pxTEthernetSettingsPackOneDestination -> ui16Port = pxSource -> ui16Port;
            PlcSettingsSave();
            PlcSettingsLoad();
			// обновим настройки интерфейса ModbusRtu, связь с локальной сетью нижнего уровня, RS-485.
			iModbusRtuMasterReset();
            // отправим сигнал потоку.
            // для вывода из состояния ожидания данных.
            // чтобы обновить настройки интерфейса.
            pthread_kill(xModbusTcp, SIGRTMIN);
            usleep(100000);
            pthread_kill(xModbusTcp, SIGRTMIN);
            break;

        default:
            break;
        }
        return 1;
    }
    else
    {
        // получим данные.
        TPlcSettingsQueryPackOne *pxSource;
        TPlcSettingsQueryPackOne *pxDestination;

        // получим указатель на место вложенного сообщения в общем сообщении.
        pxSource = (TPlcSettingsQueryPackOne*)(pxCSerialMT -> pui8DataPointer);
        // получим указатель на данные настроек.
        pxDestination = &xTPlcSettingsQueryPackOne;
        *pxDestination = *pxSource;
        return 0;
    }
}

//-----------------------------------------------------------------------------------------------------
// сохраняет архив на съёмный диск.
void CSettingsRW::ArchiveSave(uint8_t ui8ChoiceOption)
{
    char acCommand[128];
    const char *pccParseFileName;
    // создадим команду монтирования выбранного диска.
    sprintf(acCommand,
            "%s%s %s",
            "sudo mount /dev/",
            (xCArchiveSaveParse.axTDiskInfo[((ui8ChoiceOption) & 0x01)].acName),
            "/mnt/usb"
           );
    // примонтируем выбранный диск.
    system(acCommand);
    usleep(100000);

    // создадим команду сохранения и установим время.
    sprintf(acCommand,
            "%s%s_%02d%02d%02d_%02d%02d%02d.db%s",
            "sudo cp /home/debian/Pas05ArchiveDataBase.db /mnt/usb/",
            "Pas05Archive",
            (xCurrentTime.tm_mday),
            (xCurrentTime.tm_mon + 1),
            (xCurrentTime.tm_year - 100),
            (xCurrentTime.tm_hour),
            (xCurrentTime.tm_min),
            (xCurrentTime.tm_sec),
            " && sync");

    // сохраним файл.
    // файл сохранён успешно?
    if (!(system(acCommand)))
    {
        ui8ArchiveFileIsSaveState = WRITE_OK;
    }
    else
    {
        ui8ArchiveFileIsSaveState = WRITE_ERROR;
    }
}

//-----------------------------------------------------------------------------------------------------
// сохраняет архив на съёмный диск.
void CSettingsRW::DebugLogFileSave(uint8_t ui8ChoiceOption)
{
    char acCommand[128];
    const char *pccParseFileName;
    // создадим команду монтирования выбранного диска.
    sprintf(acCommand,
            "%s%s %s",
            "sudo mount /dev/",
            (xCArchiveSaveParse.axTDiskInfo[((ui8ChoiceOption) & 0x01)].acName),
            "/mnt/usb"
           );
    // примонтируем выбранный диск.
    system(acCommand);
    usleep(100000);

    // создадим команду сохранения и установим время.
    sprintf(acCommand,
            "%s%s_%02d%02d%02d_%02d%02d%02d.txt%s",
            "sudo cp /home/debian/Pas05DebugLog.txt /mnt/usb/",//Pas05DebugLog
            "Pas05DebugLog",
            (xCurrentTime.tm_mday),
            (xCurrentTime.tm_mon + 1),
            (xCurrentTime.tm_year - 100),
            (xCurrentTime.tm_hour),
            (xCurrentTime.tm_min),
            (xCurrentTime.tm_sec),
            " && sync");

    // сохраним файл.
    // файл сохранён успешно?
    if (!(system(acCommand)))
    {
        ui8DebugLogFileSaveState = WRITE_OK;
    }
    else
    {
        ui8DebugLogFileSaveState = WRITE_ERROR;
    }
}

//-----------------------------------------------------------------------------------------------------
// обновляет файл программы.
uint8_t CSettingsRW::ProgrammFileUpdate(uint8_t ui8ChoiceOption)
{
    char acCommand[128];

    // создадим команду монтирования выбранного диска.
    sprintf(acCommand,
            "%s%s %s",
            "sudo mount /dev/",
            (xCArchiveSaveParse.axTDiskInfo[((ui8ChoiceOption) & 0x01)].acName),
            "/mnt/usb && sync"
           );
    // примонтируем выбранный диск.
    system(acCommand);
    usleep(100000);

    // создадим команду сохранения.
    if ((system("sudo cp -f /mnt/usb/pas05_software_update/pas05_software_update.sh /home/debian/pas05_software_update.sh && sync")))
    {
        return 0;
    }

    // создадим команду изменим права.
    if ((system("sudo chmod +x /home/debian/pas05_software_update.sh && sync")))
    {
        return 0;
    }

    // запустим сценарий обновления ПО(pas05_software_update.sh).
    if ((system("sudo /home/debian/pas05_software_update.sh && sync")))
    {
        return 0;
    }

    return 1;
}

//-----------------------------------------------------------------------------------------------------
// проверка файла настроек.
uint8_t CSettingsRW::PlcSettingsCheck(void)
{
    const char *pxFileName;
    FILE *pxPlcSettingsFile;

    // откроем файл базы данных.
    pxFileName = pccSettingsFileName;
    pxPlcSettingsFile = fopen(pxFileName,"r+");
    if(pxPlcSettingsFile == NULL)
    {
        // файл не существует.
        printf("Cannot open PlcSettings file.\n");
        // создадим файл.
        pxPlcSettingsFile = fopen(pxFileName,"w+");
        if(pxPlcSettingsFile == NULL)
        {
            printf("Cannot create PlcSettings file.\n");
            return -1;
        }
    }
    // проверим целостность файла.
    // установим смещение в файле.
    fseek (pxPlcSettingsFile, 0, SEEK_SET);
    // скопируем содержимое файла базы данных в RAM.
    fread((void*)&xTPlcSettingsPackOne,
          sizeof(struct TPlcSettingsPackOne),
          1,
          pxPlcSettingsFile);

    // файл повреждён?
    if ((usCrc16((unsigned char*)&xTPlcSettingsPackOne,
                 (sizeof(TPlcSettingsPackOne) - CRC16_LENGTH)) != xTPlcSettingsPackOne.ui16Crc))
    {
        printf("PlcSettings CRC wrong.\n");

        memcpy(&xTPlcSettingsPackOne,
               &xTPlcSettingsDefaultPackOne,
               ((sizeof(TPlcSettingsPackOne) - CRC16_LENGTH)));

        xTPlcSettingsPackOne.ui16Crc = usCrc16((unsigned char*)&xTPlcSettingsPackOne,
                                               (sizeof(TPlcSettingsPackOne)) - CRC16_LENGTH);
        // установим смещение в файле.
        fseek (pxPlcSettingsFile, 0, SEEK_SET);
        fwrite((const void*)&(xTPlcSettingsPackOne),
               sizeof(struct TPlcSettingsPackOne),
               1,
               pxPlcSettingsFile);

        printf("Default PlcSettings create ok.\n");
    }
    else
    {
        // файл не повреждён.
        printf("PlcSettings CRC ok.\n");
    }

    fclose(pxPlcSettingsFile);
}

//-----------------------------------------------------------------------------------------------------
// сохранение файла настроек.
uint8_t CSettingsRW::PlcSettingsSave(void)
{
    const char *pxFileName;
    FILE *pxPlcSettingsFile;

    // откроем файл.
    pxFileName = pccSettingsFileName;
    pxPlcSettingsFile = fopen(pxFileName,"r+");
    if(pxPlcSettingsFile == NULL)
    {
        // файл не существует.
        printf("Cannot open PlcSettings file.\n");
    }
    else
    {
        // обновим CRC.
        xTPlcSettingsPackOne.ui16Crc = usCrc16((unsigned char*)&xTPlcSettingsPackOne,
                                               (sizeof(TPlcSettingsPackOne)) - CRC16_LENGTH);
        // установим смещение в файле.
        fseek (pxPlcSettingsFile, 0, SEEK_SET);
        // сохраним файл.
        fwrite((const void*)&(xTPlcSettingsPackOne),
               sizeof(struct TPlcSettingsPackOne),
               1,
               pxPlcSettingsFile);
    }
    // закроем файл.
    fclose(pxPlcSettingsFile);
}

//-----------------------------------------------------------------------------------------------------
// загружает настройки.
uint8_t CSettingsRW::PlcSettingsLoad(void)
{
    char acCommand[128];
    unsigned char *pucSource;
    unsigned char nucDataBaseBlockIndex;
//-----------------------------------------------------------------------------------------------------
// DataBaseBlockRead
//-----------------------------------------------------------------------------------------------------
    // номер блока базы данных.
    nucDataBaseBlockIndex = (unsigned char)MBM_FUNCTION_1234_BLOCK_DATA_BASE_BLOCK_OFFSET + 1;
    pucSource = (unsigned char*)&(xPlcDataBase.
                                  axPlcDataBaseBlocks[nucDataBaseBlockIndex].
                                  aucPlcDataBaseBlockData[MBM_FUNCTION_1234_BLOCK_DATA_BASE_BLOCK_LENGTH - 1]);

    // установим скорость интерфейса RS485 нижнего уровня.
    switch(((*pucSource >> 2) & 0x03))
    {
    case BIT_RATE_9600:
        ui32ModbusRtuMasterBaudRate = 9600;
        break;
    case BIT_RATE_19200:
        ui32ModbusRtuMasterBaudRate = 19200;
        break;
    case BIT_RATE_57600:
        ui32ModbusRtuMasterBaudRate = 57600;
        break;
    case BIT_RATE_115200:
        ui32ModbusRtuMasterBaudRate = 115200;
        break;
    default:
        ui32ModbusRtuMasterBaudRate = 9600;
        break;
    };

    // установим контроль чётности интерфейса RS485 нижнего уровня.
    switch((*pucSource & 0x03))
    {
    case PARITY_NO:
        cModbusRtuMasterParity = 'N';
        // установим количество стоп бит интерфейса RS485 нижнего уровня.
        ui8ModbusRtuMasterStopBits = 2;
        break;
    case PARITY_NO_ONE_STOP:
        cModbusRtuMasterParity = 'N';
        // установим количество стоп бит интерфейса RS485 нижнего уровня.
        ui8ModbusRtuMasterStopBits = 1;
        break;
    case PARITY_EVEN:
        cModbusRtuMasterParity = 'E';
        // установим количество стоп бит интерфейса RS485 нижнего уровня.
        ui8ModbusRtuMasterStopBits = 1;
        break;
    case PARITY_ODD:
        cModbusRtuMasterParity = 'O';
        // установим количество стоп бит интерфейса RS485 нижнего уровня.
        ui8ModbusRtuMasterStopBits = 1;
        break;
    default:
        cModbusRtuMasterParity = 'N';
        // установим количество стоп бит интерфейса RS485 нижнего уровня.
        ui8ModbusRtuMasterStopBits = 2;
        break;
    };

    // установим количество бит интерфейса RS485 нижнего уровня.
    ui8ModbusRtuMasterDataBits = 8;

    // установим скорость интерфейса RS485 верхнего уровня.
    switch(xTPlcSettingsPackOne.xTRs485HighLevelSettingsPackOne.ui8BaudRate)
    {
    case BIT_RATE_9600:
        ui32ModbusRtuBaudRate = 9600;
        break;
    case BIT_RATE_19200:
        ui32ModbusRtuBaudRate = 19200;
        break;
    case BIT_RATE_57600:
        ui32ModbusRtuBaudRate = 57600;
        break;
    case BIT_RATE_115200:
        ui32ModbusRtuBaudRate = 115200;
        break;
    default:
        ui32ModbusRtuBaudRate = 9600;
        break;
    };
    // установим контроль чётности интерфейса RS485 верхнего уровня.
    switch(xTPlcSettingsPackOne.xTRs485HighLevelSettingsPackOne.ui8Parity)
    {
    case PARITY_NO:
        cModbusRtuParity = 'N';
        break;
    case PARITY_EVEN:
        cModbusRtuParity = 'E';
        break;
    case PARITY_ODD:
        cModbusRtuParity = 'O';
        break;
    default:
        cModbusRtuParity = 'N';
        break;
    };

    // установим количество бит интерфейса RS485 верхнего уровня.
    ui8ModbusRtuDataBits = 8;
    // установим количество стоп бит интерфейса RS485 верхнего уровня.
    ui8ModbusRtuStopBits = (xTPlcSettingsPackOne.xTRs485HighLevelSettingsPackOne.ui8StopBits +
                            CONVERT_INTEGER_TO_NATURAL_NUMBER);

    // создадим команду установки IP интерфейса TCP верхнего уровня.
    // ip addr show
    // sudo ifconfig eth0 192.168.0.9 netmask 255.255.255.0 up
    sprintf(acCommand,
            "%s %d.%d.%d.%d %s",
            "sudo ifconfig eth0",
            xTPlcSettingsPackOne.xTEthernetSettingsPackOne.ui8IpByte0,
            xTPlcSettingsPackOne.xTEthernetSettingsPackOne.ui8IpByte1,
            xTPlcSettingsPackOne.xTEthernetSettingsPackOne.ui8IpByte2,
            xTPlcSettingsPackOne.xTEthernetSettingsPackOne.ui8IpByte3,
            "netmask 255.255.255.0 up"
           );
    // установим IP интерфейса TCP верхнего уровня.
    system(acCommand);
    // установим порт интерфейса TCP верхнего уровня.
    ui16ModbusTcpPort = xTPlcSettingsPackOne.xTEthernetSettingsPackOne.ui16Port;
}

//-----------------------------------------------------------------------------------------------------



//-----------------------------------------------------------------------------------------------------
uint8_t CRs485Set::TransmitProcessing(CSerialMT *pxCSerialMT, uint8_t ui8Address)
{
    uint8_t ui8Length;
    uint8_t *pui8Destination;
    // вычислим длину вложенного сообщения.
    ui8Length = TASK_HIGH_LEVEL_FLAGS_MESSAGE_LENGTH;
    // узнаем, есть ли место для сообщения.
    pui8Destination = pxCSerialMT -> MessagePushRequest(ui8Length,
                      ui8Address);
    // нет места для сообщеня?
    if (!pui8Destination)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

//-----------------------------------------------------------------------------------------------------
uint8_t CRs485Set::LoadAnswerData(CSerialMT *pxCSerialMT, uint8_t ui8Address)
{

}

//-----------------------------------------------------------------------------------------------------
uint8_t CRs485Set::ReceiveProcessing(CSerialMT *pxCSerialMT, uint8_t ui8Address)
{
    if ((ui8Address & CSerialMT::SERIAL_MT_WRITE) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

//-----------------------------------------------------------------------------------------------------



//-----------------------------------------------------------------------------------------------------
uint8_t CReceiveRequest::TransmitProcessing(CSerialMT *pxCSerialMT, uint8_t ui8Address)
{
    uint8_t ui8Length;
    uint8_t *pui8Destination;
    if ((pxCSerialMT -> ui8MessageBagLength) >= CSerialMT::MAX_RECEIVED_FRAME_LENGTH)
    {
        return 0;
    }
    // вычислим длину вложенного сообщения.
    ui8Length = (CSerialMT::MAX_RECEIVED_FRAME_LENGTH - (pxCSerialMT -> ui8MessageBagLength));
    // узнаем, есть ли место для сообщения.
    pui8Destination = pxCSerialMT -> MessagePushRequest(ui8Length,
                      ui8Address);
    // нет места для сообщеня?
    if (!pui8Destination)
    {
        return 0;
    }
    else
    {
        // создадим сообщение.
        (*pui8Destination) = 0;
        return 1;
    }
}

//-----------------------------------------------------------------------------------------------------
uint8_t CReceiveRequest::LoadAnswerData(CSerialMT *pxCSerialMT, uint8_t ui8Address)
{

}

//-----------------------------------------------------------------------------------------------------
uint8_t CReceiveRequest::ReceiveProcessing(CSerialMT *pxCSerialMT, uint8_t ui8Address)
{
    if ((ui8Address & CSerialMT::SERIAL_MT_WRITE) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

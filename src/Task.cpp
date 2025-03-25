
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

// ���������� �������� ������ ��������������� TASK_PROCESSED_MAX_NUMBER.
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
// ���������� ��������� ������.
uint8_t CTaskSerialMT::StateGet(void)
{
    return ui8DataExchangeFsmState;
}

//-----------------------------------------------------------------------------------------------------
// ������������� ��������� ������.
void CTaskSerialMT::StateSet(uint8_t ui8State)
{
    ui8DataExchangeFsmState = ui8State;
}

//-----------------------------------------------------------------------------------------------------
// ���������� ������ ������.
uint8_t CTaskSerialMT::StatusGet(void)
{
    return ui8Status;
}

//-----------------------------------------------------------------------------------------------------
// ������������� ������ �������� ������ ������� ������.
void CTaskSerialMT::StatusSet(uint8_t ui8StatusLocal)
{
    ui8Status = ui8StatusLocal;
}

//-----------------------------------------------------------------------------------------------------
// ������������� ������ ������.
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
        // �������-������ � ������ ������.
        break;

    case TASK_MODE_MESSAGE_RECEIVE_CONTINUOUSLY:
        // �������-������ ��������� ���������.
        break;

    case TASK_MODE_MESSAGE_SEND_READ_CONTINUOUSLY:
        // �������-������ ������� ���������.
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
        // ������� �������������.
        // ��������� ����� �������� ������?
        if (nui8TimeCounter >= TASK_NO_ANSWER_TIMEOUT)
        {
            // ��� ������ �� ���������.
            if (nui8AttemptsNumber < TASK_MESSAGE_SEND_ATTEMPTS_NUMBER)
            {
                // �������� �������� ���������.
                // �������� ������� ������� �������� ���������.
                nui8AttemptsNumber++;
                // ������� ������� �������������� ������� �������� ������ �������� �� ���������.
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
                // ������� ���� ������ �������� ������� �������.
                ui8Status = TASK_STATE_MESSAGE_RECEIVE_TIMEOUT;
                ui8DataExchangeFsmState = TASK_MODE_IDDLE;
            }
        }
        else
        {
            // �������� ������� �������������� ������� �������� ������ �������� �� ���������.
            nui8TimeCounter++;
        }
        break;

    case TASK_MODE_MESSAGE_SEND_READ:
        // �������-������ ������� ���������.
        // ������� ������� ������� �������� ���������.
        nui8AttemptsNumber = 0;
        // ������� ������� �������������� ������� �������� ������ �������� �� ���������.
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
        // �������-������ ������� ���������.
        // ������� ������� ������� �������� ���������.
        nui8AttemptsNumber = 0;
        // ������� ������� �������������� ������� �������� ������ �������� �� ���������.
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
        // �������-������ ������� ���������.
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
        // �������-������ ������� ���������.
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
        // �������-������ � ������ ������.
        break;

    case TASK_MODE_MESSAGE_RECEIVE_CONTINUOUSLY:
        // �������-������ ��������� ���������.
        if ((ui8Address & CSerialMT::SERIAL_MT_WRITE) != 0)
        {
            // ���������� ������� �� �������� ���������.
            ui8DataExchangeFsmState = TASK_MODE_MESSAGE_SEND_WRITE_ANSWER;
        }
        else
        {
            // ���������� ������� �� �������� ���������.
            ui8DataExchangeFsmState = TASK_MODE_MESSAGE_SEND_READ_ANSWER;
        }
        ReceiveProcessing(pxCSerialMT, ui8Address);
        ui8Status = TASK_STATE_MESSAGE_RESPONSE_OK;
        break;

    case TASK_MODE_MESSAGE_SEND_READ_CONTINUOUSLY:
        // �������-������ ��������� ���������.
        ReceiveProcessing(pxCSerialMT, ui8Address);
        ui8Status = TASK_STATE_MESSAGE_RESPONSE_OK;
        break;

    case TASK_MODE_MESSAGE_ANSWER_WAITING:
        // ������� �������������.
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
// ���������� ����� ������� ���� � ���.
uint8_t CTaskSerialMT::DataExchange(CTaskSerialMT **ppxTasksProcessed, CSerialMT *pxCSerialMT)
{
    uint8_t ui8Length;
    CTaskSerialMT **ppxTasksProcessedLocal;
    CTaskSerialMT *pxCTaskSerialMT;

    ppxTasksProcessedLocal = ppxTasksProcessed;
    // ������ ������� � ����������� �� ������� ����� ��������?
    if (ui8TasksProcessedNumber)
    {
        pxCSerialMT -> DataExchange();
        // ������� �������� ����� ���������.
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
                    // ����� ������������ ������?
                    if (ui8AddressLocal < ui8TasksProcessedNumber)
                    {
                        // ������� ��������� �� ������ ������.
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

        // �������� ����� ���������.
        pxCSerialMT -> DataTransmit();
    }
    else
    {
        // �������� ������ ������� � ����������� �� ������� �����.
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
    // �������� ����� ���������� ���������.
    ui8Length = TASK_PING_REQUEST_MESSAGE_LENGTH;
    // ������, ���� �� ����� ��� ���������.
    pui8Destination = pxCSerialMT -> MessagePushRequest(ui8Length,
                      ui8Address);
    // ��� ����� ��� ��������?
    if (!pui8Destination)
    {
        return 0;
    }
    else
    {
        // �������� ���������.
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
    // �������� ����� ���������� ���������.
    ui8Length = TASK_PING_REQUEST_MESSAGE_LENGTH;
    // ������, ���� �� ����� ��� ���������.
    pui8Destination = pxCSerialMT -> MessagePushRequest(ui8Length,
                      ui8Address);
    // ��� ����� ��� ��������?
    if (!pui8Destination)
    {
        return 0;
    }
    else
    {
        // �������� ���������.
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
    // �������� ����� ���������� ���������.
    ui8Length = TASK_PING_REQUEST_MESSAGE_LENGTH;
    // ������, ���� �� ����� ��� ���������.
    pui8Destination = pxCSerialMT -> MessagePushRequest(ui8Length,
                      ui8Address);
    // ��� ����� ��� ��������?
    if (!pui8Destination)
    {
        return 0;
    }
    else
    {
        // �������� ���������.
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
    // ������� - ������?
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
    // ���� ������� - ����������� ����� mmc0 �� mmc1?
    if (fbGlobalMmcCopy)
    {
        // ������� �������.
        fbGlobalMmcCopy = 0;
        // ��������� ���� - ����������� ����� mmc0 �� mmc1.
        xTHighLevelFlagsExchangePackOne.ui8HighLevelFlagsWE |= (1 << HIGH_LEVEL_FLAGS_MMC_COPY_BIT);
        // ���������� ������� �� �������� ���������.
        ui8DataExchangeFsmState = TASK_MODE_MESSAGE_SEND_WRITE;
    }

    // ���� ������� - ��������� ����������� ����� mmc0 �� mmc1?
    if (fbGlobalMmcCopyEnd)
    {
        // ������� �������.
        fbGlobalMmcCopyEnd = 0;
        // ��������� ���� - ����������� ����� mmc0 �� mmc1.
        xTHighLevelFlagsExchangePackOne.ui8HighLevelFlagsWE |= (1 << HIGH_LEVEL_FLAGS_MMC_COPY_END_BIT);
        // ���������� ������� �� �������� ���������.
        ui8DataExchangeFsmState = TASK_MODE_MESSAGE_SEND_WRITE;
    }

    // ���� ������� - "������������" c �������� ������?
    if (pxModuleContext ->
            xModuleContextDinamic.
            fbGlobalModbusKvit)
    {
        // ������� �������.
        pxModuleContext ->
        xModuleContextDinamic.
        fbGlobalModbusKvit = 0;
        // ��������� ���� - ������������ �� PC.
        xTHighLevelFlagsExchangePackOne.ui8HighLevelFlagsWE |= (1 << HIGH_LEVEL_FLAGS_KVIT_BIT);
        // ���������� ������� �� �������� ���������.
        ui8DataExchangeFsmState = TASK_MODE_MESSAGE_SEND_WRITE;

    }

    // ���� ������� - "�����" c �������� ������?
    if (pxModuleContext ->
            xModuleContextDinamic.
            fbGlobalModbusReset)
    {
        // ������� �������.
        pxModuleContext ->
        xModuleContextDinamic.
        fbGlobalModbusReset = 0;
        // ��������� ���� - ����� �� PC.
        xTHighLevelFlagsExchangePackOne.ui8HighLevelFlagsWE |= (1 << HIGH_LEVEL_FLAGS_RESET_BIT);
        // ���������� ������� �� �������� ���������.
        ui8DataExchangeFsmState = TASK_MODE_MESSAGE_SEND_WRITE;
    }

    // ���� ����������?
    if (fbGlobalBlock)
    {
        // ��������� ������ ���������� ����������?
        if (!(xTHighLevelFlagsExchangePackOne.ui8HighLevelFlagsWE &
                (1 << HIGH_LEVEL_FLAGS_BLOCK_BIT)))
        {
            // ��������� ���� - ���������� �� PC.
            xTHighLevelFlagsExchangePackOne.ui8HighLevelFlagsWE |= (1 << HIGH_LEVEL_FLAGS_BLOCK_BIT);
            // ���������� ������� �� �������� ���������.
            ui8DataExchangeFsmState = TASK_MODE_MESSAGE_SEND_WRITE;
        }
    }
    else
    {
        if (xTHighLevelFlagsExchangePackOne.ui8HighLevelFlagsWE &
                (1 << HIGH_LEVEL_FLAGS_BLOCK_BIT))
        {
            // ������� ���� - ���������� �� PC.
            xTHighLevelFlagsExchangePackOne.ui8HighLevelFlagsWE &= ~(1 << HIGH_LEVEL_FLAGS_BLOCK_BIT);
            // ���������� ������� �� �������� ���������.
            ui8DataExchangeFsmState = TASK_MODE_MESSAGE_SEND_WRITE;
        }
    }

    // ���� �������� ������?
    if ((fucZvkMal != 0) && (fucZvkMal != 1))
//    if (fucZvkMal)
    {
        // �������� ������� ����� � ����.
        xTHighLevelFlagsExchangePackOne.ui8SoundSignalType = fucZvkMal;
        // ������� ������� �����.
        fucZvkMal = COMMAND_SOUND_SIGNAL_TYPE_NOT;
        // ���������� ������� �� �������� ���������.
        ui8DataExchangeFsmState = TASK_MODE_MESSAGE_SEND_WRITE;
    }
    else
    {
        // �������� ������� ��� ����� � ����.
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
    // �������� ����� ���������� ���������.
    ui8Length = sizeof(struct THighLevelFlagsExchangePackOne);
    // ������, ���� �� ����� ��� ���������.
    pui8Destination = pxCSerialMT -> MessagePushRequest(ui8Length,
                      ui8Address);
    // ��� ����� ��� ��������?
    if (!pui8Destination)
    {
        return 0;
    }
    else
    {
        // ������� ��������� �� ������.
        pxSource = &xTHighLevelFlagsExchangePackOne;
        pxDestination = (THighLevelFlagsExchangePackOne*)pui8Destination;
        // �������� ���������.
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
        // �������� ������������� �����(��������� ���������� �������).
        // ������� ����� ���������� ������������ � �����.
        // ���� ���������� �������.
        xTHighLevelFlagsExchangePackOne.ui8HighLevelFlagsWE &=
            ~((1 << HIGH_LEVEL_FLAGS_KVIT_BIT) | (1 << HIGH_LEVEL_FLAGS_RESET_BIT) |
              (1 << HIGH_LEVEL_FLAGS_MMC_COPY_BIT) | (1 << HIGH_LEVEL_FLAGS_MMC_COPY_END_BIT));
        return 1;
    }
    else
    {
        // �������� ������������� �����(��������� ���������� �������).
        // ������� ����� ���������� ������������ � �����.
        // ���� ���������� �������.
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
    // �������� ����� ���������� ���������.
    ui8Length = TASK_CONFIGURATION_REQUEST_MESSAGE_LENGTH;
    // ������, ���� �� ����� ��� ���������.
    pui8Destination = pxCSerialMT -> MessagePushRequest(ui8Length,
                      ui8Address);
    // ��� ����� ��� ��������?
    if (!pui8Destination)
    {
        return 0;
    }
    else
    {
        // �������� ���������.
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
        // ������� ��������� �� ������ � ������� � ��������� ��������� ���� �������.
        pxModuleContextOrigin = (pxModuleContext - (pxModuleContext ->
                                 xModuleContextStatic.
                                 ucModuleContextIndex));
        // ������� ��������� ������ �� ����.
        ui8HighLevelFlagsTemp = *(pxCSerialMT -> pui8DataPointer);
        // ���� ������������ �� ����?
        if (ui8HighLevelFlagsTemp & (1 << HIGH_LEVEL_FLAGS_KVIT_BIT))
        {
            // ��������� ���� - ������������ �� ����.
            fbGlobalKvit = 1;
            // ��������� ���� ������������ ���� �������.
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
        // ���� ����� �� ����?
        if (ui8HighLevelFlagsTemp & (1 << HIGH_LEVEL_FLAGS_RESET_BIT))
        {
            // ��������� ���� - ����� �� ����.
            fbGlobalReset = 1;
            // ��������� ���� ������ ���� �������.
            for (int i = 0; i < MAX_MODULE_QUANTITY; i++)
            {
                pxModuleContextOrigin[i].
                xModuleContextDinamic.
                fbGlobalButtonReset = 1;
            }
        }
        // ���� ���������� �� ����?
        if (ui8HighLevelFlagsTemp & (1 << HIGH_LEVEL_FLAGS_BLOCK_BIT))
        {
            // ��������� ���� - ���������� �� ����.
            fbGlobalBlock = 1;
            // ��������� ���� ���������� ���� �������.
            for (int i = 0; i < MAX_MODULE_QUANTITY; i++)
            {
                pxModuleContextOrigin[i].
                xModuleContextDinamic.
                fbGlobalButtonBlock = 1;
            }
        }
        else
        {
            // ������� ���� ����������.
            fbGlobalBlock = 0;
            // ������� ���� ���������� ���� �������.
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
        // ������.
        // ������� ��� �������.
        pxDestination -> ui8Type = 0;
        // ������ ������������� ������.
        // ��� ������������� ��� ������ ������ ��� ������ � ����.
        pxDestination -> i16Index =
            (i16IndexQuery);
        // ������� ������� �������� - ��� �������.
        pxDestination -> ui16ID = 0;
    }
    else
    {
        // ���� ������ - ��� �������?
        if ((pxSource -> ui16ID) == 0)
        {
            // ������� ��� �������.
            pxDestination -> ui8Type = 0;
            // ������ ������������� ������.
            // ��� ������������� ��� ������ ������ ��� ������ � ����.
            pxDestination -> i16Index =
                (i16IndexQuery);
            // ������� ������� �������� - ��� �������.
            pxDestination -> ui16ID = 0;
        }
        else
        {
            if ((pxSource -> ui8Type) == CEvents::HANDLED_EVENTS_CONFIGURATION_ERROR_TYPE ||
                    (pxSource -> ui8Type) == CEvents::HANDLED_EVENTS_INFO_TYPE)
            {
                // ������� id �������.
                pxDestination -> ui16ID =
                    pxSource -> ui16ID;
                // ������� ��� �������.
                pxDestination -> ui8Type =
                    pxSource -> ui8Type;
                // ������ �������������� �������.
                // ��� ������������� ��� ������ ������� ��� ������ � ����.
                pxDestination -> i16Index =
                    (i16IndexQuery);
                // ��������� ������ ������.
                memcpy(&(pxDestination -> acTextData),
                       (&(pxSource -> acTextData)),
                       (CEvents::EVENT_NAME_LENGTH + END_OF_STRING_LENGTH));
                // ��������� ������ ���������� ���������.
                memcpy(&(pxDestination -> acTextDescriptor),
                       (&(pxSource -> acTextDescriptor)),
                       (CEvents::EVENT_NAME_LENGTH + END_OF_STRING_LENGTH));
            }
//            else if ((pxSource -> ui8Type) == CEvents::HANDLED_EVENTS_INFO_TIME_TYPE)
//            {
//                // ������� id �������.
//                pxDestination -> ui16ID =
//                    pxSource -> ui16ID;
//                // ������� ��� �������.
//                pxDestination -> ui8Type =
//                    CEvents::HANDLED_EVENTS_INFO_TYPE;
//                // ������ �������������� �������.
//                // ��� ������������� ��� ������ ������� ��� ������ � ����.
//                pxDestination -> i16Index =
//                    (i16IndexQuery);
//                // �������� ����� �������.
//                pxDestination -> ui8Second = (uint8_t)pxSource -> xCurrentTime.tm_sec;
//                pxDestination -> ui8Minute = (uint8_t)pxSource -> xCurrentTime.tm_min;
//                pxDestination -> ui8Hour = (uint8_t)pxSource -> xCurrentTime.tm_hour;
//                pxDestination -> ui8MonthDay = (uint8_t)pxSource -> xCurrentTime.tm_mday;
//                pxDestination -> ui8Month = (uint8_t)(pxSource -> xCurrentTime.tm_mon) + 1;
//                pxDestination -> ui8Year = (uint8_t)(pxSource -> xCurrentTime.tm_year) - 100;
//                // ��������� ������ ���������� ���������.
//                memcpy(&(pxDestination -> acTextDescriptor),
//                       (&(pxSource -> acTextDescriptor)),
//                       (CEvents::EVENT_NAME_LENGTH + END_OF_STRING_LENGTH));
//            }
            else
            {
                // ������� id �������.
                pxDestination -> ui16ID =
                    pxSource -> ui16ID;
                // ������� ��� �������.
                pxDestination -> ui8Type =
                    pxSource -> ui8Type;
                // ������ �������������� �������.
                // ��� ������������� ��� ������ ������� ��� ������ � ����.
                pxDestination -> i16Index =
                    (i16IndexQuery);
                // ������� ����� �������.
                pxDestination -> ui16Address =
                    pxSource -> ui16Address;
                // ������� ��������� �������.
                pxDestination -> ui8State =
                    pxSource -> ui8State;
                // ����� �������� ������ �� ��� ������?
                if ((pxSource -> xCurrentTime.tm_sec) != 0xFF)
                {
                    // �������� ����� �������.
                    pxDestination -> ui8Second = (uint8_t)pxSource -> xCurrentTime.tm_sec;
                    pxDestination -> ui8Minute = (uint8_t)pxSource -> xCurrentTime.tm_min;
                    pxDestination -> ui8Hour = (uint8_t)pxSource -> xCurrentTime.tm_hour;
                    pxDestination -> ui8MonthDay = (uint8_t)pxSource -> xCurrentTime.tm_mday;
                    pxDestination -> ui8Month = (uint8_t)(pxSource -> xCurrentTime.tm_mon) + 1;
                    pxDestination -> ui8Year = (uint8_t)(pxSource -> xCurrentTime.tm_year) - 100;
                }
                else
                {
                    // ��������� ��� ������� ������� - ��� ������.
                    pxDestination -> ui8Second = 0xFF;
                }
                // ��������� ������ ���������� ���������.
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

    // �������� ����� ���������� ���������.
    ui8Length =
        (CEvents::xEventsLogQueryList.ui8Quantity *
         sizeof(struct CEvents::TEventDataPackOne));
    // ������, ���� �� ����� ��� ���������.
    pui8Destination = pxCSerialMT -> MessagePushRequest(ui8Length,
                      ui8Address);
    // ��� ����� ��� ��������?
    if (!pui8Destination)
    {
        return 0;
    }
    else
    {
        // �������� ���������.
        // ������� ���������� ��������� ������� � �������.
        ui8Length = CEvents::xEventsLogQueryList.ui8Quantity;
        // ���������� ���������� ������� �����������
        // ������ �������?
        if (ui8Length >
                (CEvents::REQUESTED_EVENTS_QUANTITY))
        {
            // ������.
            return 0;
        }

        CEvents::TEventDataCommon *pxSource;
        CEvents::TEventDataPackOne *pxDestination;
        // ������ �������������� �������.
        int16_t i16IndexQuery;
        uint16_t ui16ID;


        // ������� ��������� �� ����� ���������� ��������� � ����� ���������.
        pxDestination = (CEvents::TEventDataPackOne*)(pui8Destination);
        CEvents *pxCEvents;
        switch (CEvents::xEventsLogQueryList.ui8Type)
        {
        case CEvents::REQUESTED_EVENTS_ALARM_TYPE:
            printf("CEventLogQuery REQUESTED_EVENTS_ALARM_TYPE %d\n\r", (ui8Address));

            pxCEvents = &xCAlarmEvent;
            // ������� ������ ������� �������������� �������.
            i16IndexQuery = (CEvents::xEventsLogQueryList.i16Index -
                             CArchiveEventsDB::EVENTS_DB_PREVIOUS_RECORDS_QUANTITY);
            // ��������� ������ � ������� ���������� �� ������� � ������� ����� �������.
            pxCEvents -> EventDataPopIndexSet(i16IndexQuery + CONVERT_INTEGER_TO_NATURAL_NUMBER);
            // �������� ������ ������� �� ��������� ���������.
            for (uint16_t i = 0;
                    i < ui8Length;
                    i++)
            {
                // ������� ��������� �� ������ �������.
                FillingMessage(&pxDestination[i],
                               pxCEvents -> EventDataPop(),
                               i16IndexQuery);
                // ������ ���������� �������������� �������.
                i16IndexQuery++;
            }
            break;

        case CEvents::REQUESTED_EVENTS_ERROR_TYPE:
            printf("CEventLogQuery REQUESTED_EVENTS_ERROR_TYPE %d\n\r", (ui8Address));
            pxCEvents = &xCPlcErrorEvent;
            // ������� ������ ������� �������������� �������.
            i16IndexQuery = (CEvents::xEventsLogQueryList.i16Index -
                             CArchiveEventsDB::EVENTS_DB_PREVIOUS_RECORDS_QUANTITY);
            // ��������� ������ � ������� ���������� �� ������� � ������� ����� �������.
            pxCEvents -> EventDataPopIndexSet(i16IndexQuery + CONVERT_INTEGER_TO_NATURAL_NUMBER);
            // �������� ������ ������� �� ��������� ���������.
            for (uint16_t i = 0;
                    i < ui8Length;
                    i++)
            {
                // ������� ��������� �� ������ �������.
                FillingMessage(&pxDestination[i],
                               pxCEvents -> EventDataPop(),
                               i16IndexQuery);
                // ������ ���������� �������������� �������.
                i16IndexQuery++;
            }
            break;

        case CEvents::REQUESTED_EVENTS_INFO_TYPE:
            printf("CEventLogQuery REQUESTED_EVENTS_INFO_TYPE %d\n\r", (ui8Address));
            pxCEvents = &xCInfoEvent;
            // ������� ������ ������� �������������� �������.
            i16IndexQuery = (CEvents::xEventsLogQueryList.i16Index -
                             CArchiveEventsDB::EVENTS_DB_PREVIOUS_RECORDS_QUANTITY);
            // ������������� ������ ������?
            if (!CEvents::xEventsLogQueryList.i16Index)
            {
//                InfoEvetnsLogCreate();
            }
            // ��������� ������ � ������� ���������� �� ������� � ������� ����� �������.
            pxCEvents -> EventDataPopIndexSet(i16IndexQuery + CONVERT_INTEGER_TO_NATURAL_NUMBER);
            // �������� ������ ������� �� ��������� ���������.
            for (uint16_t i = 0;
                    i < ui8Length;
                    i++)
            {
                // ������� ��������� �� ������ �������.
                FillingMessage(&pxDestination[i],
                               pxCEvents -> EventDataPop(),
                               i16IndexQuery);
                // ������ ���������� �������������� �������.
                i16IndexQuery++;
            }
            break;

        case CEvents::REQUESTED_EVENTS_ARCHIVE_TYPE:
            printf("CEventLogQuery REQUESTED_EVENTS_ARCHIVE_TYPE %d\n\r", (ui8Address));
            if (CEvents::xEventsLogQueryList.i16Index == 0)
            {
                // �������� ���������� ������� � ������� ���� ������.
                xCArchiveEventsDB.ui16TotalRecordsNumber =
                    xCArchiveEventsDB.SendQuery("SELECT COUNT(*) FROM Events;");
                // ����� � ������ ��� �������� ������� � ��������� ������,
                // ������� id ��������� ������ ���� ����.
                // � ������� ������� ������ � id ������ ����������.
                xCArchiveEventsDB.ui16LastLastID = (xCArchiveEventsDB.ui16TotalRecordsNumber + 1);
                xCArchiveEventsDB.ui16LastFirstID = xCArchiveEventsDB.ui16LastLastID;
                i16LastRequestIndex = 0;
            }
            // ������� ������ ������� �������������� �������.
            i16IndexQuery = (CEvents::xEventsLogQueryList.i16Index);

//            printf("CEventLogQuery i16IndexQuery %d\n\r", (i16IndexQuery));
//            printf("CEventLogQuery i16LastRequestIndex %d\n\r", (i16LastRequestIndex));

            xCArchiveEventsDB.i16IndexQuery = i16IndexQuery;
            pxCEvents = &xCPlcArchiveEvent;
            // ������� ����� �������, ����� ����� ����������� �������� ����,
            // ��� ������� �����������.
            pxCEvents -> NoEventsFlagSet();
            // ����������� ������ �����(�� �������� ID � �������� ID, �� �������� ������� � ��������)?
            if (i16IndexQuery >= i16LastRequestIndex)
            {
                // �������� ������� ������ �������.
                i16LastRequestIndex = i16IndexQuery;
                // ����������� ������ �����.
                ui8DirectionIsForward = 1;
                // �������� ������ �� ���� ������ � �����.
                xCArchiveEventsDB.DataBaseDataGet();
                // ��������� ������ � ������� ���������� �� ������� � ������� ����� �������.
                pxCEvents -> EventDataPopIndexSet(xCArchiveEventsDB.ui8QueryGetRowQuantity);
                // �������� ������ ������� �� ��������� ���������.
                for (uint16_t i = 0;
                        i < ui8Length;
                        i++)
                {
                    // ������� ��������� �� ������ �������.
                    FillingMessage(&pxDestination[i],
                                   pxCEvents -> EventDataPop(),
                                   i16IndexQuery);
                    // ������ ���������� �������������� �������.
                    i16IndexQuery++;
                }
            }
            else
            {
                // �������� ������� ������ �������.
                i16LastRequestIndex = i16IndexQuery;
                // ����������� ������ �����(�� �������� ID � �������� ID, �� �������� ������� � ��������).
                ui8DirectionIsForward = 0;
                // �������� ������ �� ���� ������ � �����.
                xCArchiveEventsDB.DataBaseDataGet();
                // ��������� ������ � ������� ���������� �� ������� � ������� ����� �������.
                pxCEvents -> EventDataPopIndexSet(xCArchiveEventsDB.ui8QueryGetRowQuantity);
                // �������� ������ ������� �� ��������� ���������.
                for (uint16_t i = xCArchiveEventsDB.ui8QueryGetRowQuantity;
                        i > 0;
                    )
                {
                    // ����������� ���������� ����� � ������ � �������.
                    i--;
                    // ������� ��������� �� ������ �������.
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

        // ������� ��������� �� ����� ���������� ��������� � ����� ���������.
        pxSource = (CEvents::TEventsLogQueryListPackOne*)pxCSerialMT -> pui8DataPointer;
        // ������� ��������� �� ������� ������ �� ������� �������� �������.
        pxDestination =
            (CEvents::TEventsLogQueryListPackOne*)&CEvents::xEventsLogQueryList;
        // �������� �������� ������ �������� ������� � ������� ������.
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
    // �������� ����� ���������� ���������.
    ui8Length =
        (CEvents::xEventsLogQueryList.ui8Quantity *
         sizeof(struct CEvents::TEventDataPackOne));
    // ������, ���� �� ����� ��� ���������.
    pui8Destination = pxCSerialMT -> MessagePushRequest(ui8Length,
                      ui8Address);
    // ��� ����� ��� ��������?
    if (!pui8Destination)
    {
        return 0;
    }
    else
    {
        // �������� ���������.
        // ������� ���������� ��������� ������� � �������.
        ui8Length = CEvents::xEventsLogQueryList.ui8Quantity;
        // ���������� ���������� ������� �����������
        // ������ �������?
        if (ui8Length >
                (DISCRETE_INPUT_QUERY_LIST_LENGTH))
        {
            // ������.
            return 0;
        }

        uint8_t *pui8Source;
        CEvents::TEventDataPackOne *pxDestination;
        // ������ �������������� �������.
        int16_t i16IndexQuery;
        uint8_t ui8Data;

        // ������� ������ ������� �������������� �������.
        i16IndexQuery = (CEvents::xEventsLogQueryList.i16Index);
        // ������� ��������� �� ������ ��������� ���������� �������.
        pui8Source =
            (uint8_t*)pxModuleContext ->
            xModuleContextStatic.
            pucSostDoOriginBufferPointer;
        // ������� ��������� �� ����� ���������� ��������� � ����� ���������.
        pxDestination = (CEvents::TEventDataPackOne*)(pui8Destination);
        // �������� ������ ������� �� ��������� ���������.
        for (uint16_t i = 0;
                i < ui8Length;
                i++)
        {
            // ������ �������������� �����(������) ��������� ���������� �������
            // ������� �� ������� ������� ��������� ���������� �������?
            if (i16IndexQuery >=
                    (xPlcConfigService.xPlcConfigServiceData.ucServiceDiscreteOutputModuleQuantity))
            {
                // ������.
                // ������ ������������� ������.
                // ��� ������������� ��� ������ ������ ��� ������ � ����.
                pxDestination[i].i16Index =
                    (i16IndexQuery);
                // ������� ������� �������� - ��� �������.
                pxDestination[i].ui16ID = 0;
            }
            else
            {
                pxDestination[i].ui16ID = 1;
                // ������ �������������� �����(������) ��������� ���������� �������.
                // ��� ������������� ��� ������ �����(������) ��������� ���������� ������� ��� ������ � ����.
                pxDestination[i].i16Index =
                    i16IndexQuery;

                // ������� ��������� ���������� �������.
                ui8Data = 0;
                // � ������� ��������� ���������� �������, ��������� ������ ������ �������� ���� ����.
                // � ���� ��������� ���� ���������� ������� ������ ������ ������� ����� ������.
                // �������� ��������� ���� ���������� ������� ������ ������.
                for (uint8_t j = 0;
                        j < MRXX_DISCRETE_OUTPUT_QUANTITY;
                        j++)
                {
                    // ���������� ����� �������?
                    if (pui8Source[(i16IndexQuery * MRXX_DISCRETE_OUTPUT_QUANTITY) + j])
                    {
                        ui8Data |= (1 << j);
                    }
                }
                // ��������� ���������� �������.
                pxDestination[i].ui8State =
                    ui8Data;
            }
            // ������ ���������� �������������� �������.
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

        // ������� ��������� �� ����� ���������� ��������� � ����� ���������.
        pxSource = (CEvents::TEventsLogQueryListPackOne*)pxCSerialMT -> pui8DataPointer;
        // ������� ��������� �� ������� ������ �� ������� �������� �������.
        pxDestination =
            (CEvents::TEventsLogQueryListPackOne*)&CEvents::xEventsLogQueryList;
        // �������� �������� ������ �������� ������� � ������� ������.
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
    // ������ �������������� ����������� ���������.
    int16_t i16IndexQuery;
    // ������� ������ ������� �������������� ����������� ���������.
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
        // �������� ����� ���������� ���������.
        ui8Length =
            (CEvents::xEventsLogQueryList.ui8Quantity *
             sizeof(struct CEvents::TEventDataPackOne));
    }
    else
    {
//            cout << "CAnalogueMeasureQuery TEventDataDinamicPackOne" << endl;
        // �������� ����� ���������� ���������.
        ui8Length =
            (CEvents::xEventsLogQueryList.ui8Quantity *
             sizeof(struct CEvents::TEventDataDinamicPackOne));
    }

    // ������, ���� �� ����� ��� ���������.
    pui8Destination = pxCSerialMT -> MessagePushRequest(ui8Length,
                      ui8Address);
    // ��� ����� ��� ��������?
    if (!pui8Destination)
    {
        return 0;
    }
    else
    {
        // �������� ���������.
        // ������� ���������� ��������� ������� � �������.
        ui8Length = CEvents::xEventsLogQueryList.ui8Quantity;
        // ���������� ���������� ������� �����������
        // ������ �������?
        if (ui8Length >
                (CEvents::REQUESTED_EVENTS_QUANTITY))
        {
            // ������.
            return 0;
        }

        float *pfSource;
        TAnalogueInputDescriptionWork *pxSource;
        TDimentionParameter *pxDimentionParameter;
        uint8_t *pui8StatAiByteArray;

        // ������� ��������� �� ������� ������ � ����������� �����������.
        pfSource = (float*)pxModuleContext ->
                   xModuleContextStatic.
                   pucAiValueOriginBufferPointer;
        // ������� ��������� �� ������� ������ � ��������������� ����� ������ ��������� ���������� ��������.
        pxSource = (TAnalogueInputDescriptionWork*)pxModuleContext ->
                   xModuleContextStatic.
                   pucAnalogueInputDescriptionWork;
        // ������� ��������� �� ������� ������ � ��������������� ����� ������ ������������.
        pxDimentionParameter = (TDimentionParameter*)pxModuleContext ->
                               xModuleContextStatic.
                               pucDimentionsParametersDataBase;

        // ������� ��������� �� ������� ������ - ��������� ���������� ������������� ������� ��� �������������� � �������.
        pui8StatAiByteArray = pxModuleContext ->
                              xModuleContextStatic.
                              pucStatAiBufferPointer;
        // ������ ������ �������?
//        if ((i16IndexQuery != i16LastRequestIndex) ||
//                (i16IndexQuery == i16LastRequestIndex == 0))
        if ((i16IndexQuery != i16LastRequestIndex) ||
                (i16IndexQuery == -1))
        {
//            cout << "CAnalogueMeasureQuery TEventDataPackOne" << endl;
            // �������� ������� ������ �������.
            i16LastRequestIndex = i16IndexQuery;
            CEvents::TEventDataPackOne *pxDestination;

            // ������� ��������� �� ����� ���������� ��������� � ����� ���������.
            pxDestination = (CEvents::TEventDataPackOne*)(pui8Destination);
            // �������� ������ ������� �� ��������� ���������.
            for (uint16_t i = 0;
                    i < ui8Length;
                    i++)
            {
                // �� ���������� ������ ��� ����������� ���������?
                if ((i16IndexQuery) >=
                        (xPlcConfigService.xPlcConfigServiceData.
                         ucServiceAnalogueInputQuantity))
                {
                    // ������.
                    // ������� ��� �������.
                    pxDestination[i].ui8Type = CEvents::HANDLED_EVENTS_ANALOGUE_TYPE;
                    // ������ ������������� ������.
                    // ��� ������������� ��� ������ ������ ��� ������ � ����.
                    pxDestination[i].i16Index =
                        (i16IndexQuery);
                    // ������� ������� �������� - ��� �������.
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

                    // ������� ����� ����������� ���������.
                    pxDestination[i].ui8Address =
                        (ui8Address + CONVERT_INTEGER_TO_NATURAL_NUMBER);
                    // ������� id �������.
                    pxDestination[i].ui16ID = (ui8Address + CONVERT_INTEGER_TO_NATURAL_NUMBER);
                    // ������� ��� �������.
                    pxDestination[i].ui8Type = CEvents::HANDLED_EVENTS_ANALOGUE_TYPE;
                    // ������ �������������� �������.
                    // ��� ������������� ��� ������ ������� ��� ������ � ����.
                    pxDestination[i].i16Index =
                        (i16IndexQuery);

                    // ������� ��������� ����������� ���������.
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

                    // ������� �������� ����������� ���������.
                    pxDestination[i].fData =
                        pfSource[ui8Address];
                    // ��������� ������ ���������� ���������.
                    memcpy(&(pxDestination[i].acTextDescriptor),
                           (&(pxSource[ui8Address].acTextDescriptor)),
                           (ANALOGUE_INPUT_NAME_LENGTH + END_OF_STRING_LENGTH));
                    // ��������� ������ ���������� ��������� ����������� ����������� ��������� ����.
                    memcpy(&(pxDestination[i].acDimentionParameterText),
                           (&(pxDimentionParameter[(pxSource[ui8Address].ucDimentionCode)].acDimentionParameterText)),
                           (DIMENSIONS_PARAMETERS_NAME_LENGTH + END_OF_STRING_LENGTH));
                }
                // ������ ���������� �������������� �������.
                i16IndexQuery++;
            }
        }
        else
        {
//            cout << "CAnalogueMeasureQuery TEventDataDinamicPackOne" << endl;
            CEvents::TEventDataDinamicPackOne *pxDestination;

            // ������� ��������� �� ����� ���������� ��������� � ����� ���������.
            pxDestination = (CEvents::TEventDataDinamicPackOne*)(pui8Destination);
            // �������� ������ ������� �� ��������� ���������.
            for (uint16_t i = 0;
                    i < ui8Length;
                    i++)
            {
                // �� ���������� ������ ��� ����������� ���������?
                if ((i16IndexQuery) >=
                        (xPlcConfigService.xPlcConfigServiceData.
                         ucServiceAnalogueInputQuantity))
                {
                    // ������.
//                    // ������� ��� �������.
//                    pxDestination[i].ui8Type = CEvents::HANDLED_EVENTS_ANALOGUE_TYPE;
                    // ������ ������������� ������.
                    // ��� ������������� ��� ������ ������ ��� ������ � ����.
                    pxDestination[i].i16Index =
                        (i16IndexQuery);
                    // ������� ������� �������� - ��� �������.
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

//                    // ������� ����� ����������� ���������.
//                    pxDestination[i].ui8Address =
//                        ui8Address;
                    // ������� id �������.
                    pxDestination[i].ui16ID = (ui8Address + CONVERT_INTEGER_TO_NATURAL_NUMBER);
//                    // ������� ��� �������.
//                    pxDestination[i].ui8Type = CEvents::HANDLED_EVENTS_ANALOGUE_TYPE;
                    // ������ �������������� �������.
                    // ��� ������������� ��� ������ ������� ��� ������ � ����.
                    pxDestination[i].i16Index =
                        (i16IndexQuery);

                    // ������� ��������� ����������� ���������.
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

                    // ������� �������� ����������� ���������.
                    pxDestination[i].fData =
                        pfSource[ui8Address];
                }
                // ������ ���������� �������������� �������.
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

        // ������� ��������� �� ����� ���������� ��������� � ����� ���������.
        pxSource = (CEvents::TEventsLogQueryListPackOne*)pxCSerialMT -> pui8DataPointer;
        // ������� ��������� �� ������� ������ �� ������� �������� �������.
        pxDestination =
            (CEvents::TEventsLogQueryListPackOne*)&CEvents::xEventsLogQueryList;
        // �������� �������� ������ �������� ������� � ������� ������.
        *pxDestination = *pxSource;
        return 0;
    }
}

//-----------------------------------------------------------------------------------------------------





//-----------------------------------------------------------------------------------------------------
// ������� ������� ���������� � �������.
// ������ ������� � �������� ������ � ������ ������� "������".
int8_t CInfoLogQuery::InfoEvetnsLogCreate(void)
{
    uint8_t ui8InfoEvetnsIndex;
    char acCommand[128];
    char* pcBashCommandAnswer;
    CEvents::TEventDataCommon *pxEventData;
    float fAverageSumm = 0;

    pxEventData = xCInfoEvent.EventGetBuffer();

    // ������� ������� �������� - ��� �������.
    for (int i = 0;
            i < (xCInfoEvent.ui16RecordedEventsQuantity);
            i++)
    {
        pxEventData[i].ui16ID = 0;
    }

    ui8InfoEvetnsIndex = 0;
//-----------------------------------------------------------------------------------------------------
    // �������� ������ �������.
    // ��������� id �������.
    (pxEventData -> ui16ID) =
        1;
    // ��������� ��� �������.
    (pxEventData -> ui8Type) =
        CEvents::HANDLED_EVENTS_INFO_TYPE;
    // �������� ������ ���������� ���������.
    strcpy(pxEventData -> acTextDescriptor, "Common load");
    // �������� ������ �������.
    strcpy(acCommand,
           "ps aux | awk '{s += $3} END {print s \"%\"}'");
    pcBashCommandAnswer = xCArchiveSaveParse.BashCommandHandler(acCommand);
    // ������� �����?
    if (pcBashCommandAnswer)
    {
        // ����������� ������ ������.
        pxEventData -> fData = atof(pcBashCommandAnswer);
        // �������� ������ ������������.
        strcpy(pxEventData -> acDimentionParameterText,
               "%");
    }
    else
    {
        // ��� ������.
        pxEventData -> fData = 0;
        strcpy(pxEventData -> acDimentionParameterText,
               " %");
    }
    pxEventData++;
    ui8InfoEvetnsIndex++;

//-----------------------------------------------------------------------------------------------------
    // �������� ������ �������.
    // ��������� id �������.
    (pxEventData -> ui16ID) =
        1;
    // ��������� ��� �������.
    (pxEventData -> ui8Type) =
        CEvents::HANDLED_EVENTS_INFO_TYPE;
    // �������� ������ ���������� ���������.
    strcpy(pxEventData -> acTextDescriptor, "PasNew load");
    // �������� ������ �������.
    strcpy(acCommand,
           "ps aux | awk '/PasNew/ && /Sl/ && !/awk/ {print $3 \"%\"}'");
    pcBashCommandAnswer = xCArchiveSaveParse.BashCommandHandler(acCommand);
    // ������� �����?
    if (pcBashCommandAnswer)
    {
        // ����������� ������ ������.
        pxEventData -> fData = atof(pcBashCommandAnswer);
        // �������� ������ ������������.
        strcpy(pxEventData -> acDimentionParameterText,
               " %");
    }
    else
    {
        // ��� ������.
        pxEventData -> fData = 0;
        strcpy(pxEventData -> acDimentionParameterText,
               "%");
    }

    pxEventData++;
    ui8InfoEvetnsIndex++;

//-----------------------------------------------------------------------------------------------------
    // �������� ������ �������.
    // ��������� id �������.
    (pxEventData -> ui16ID) =
        1;
    // ��������� ��� �������.
    (pxEventData -> ui8Type) =
        CEvents::HANDLED_EVENTS_INFO_TYPE;
    // �������� ������ ���������� ���������.
    strcpy(pxEventData -> acTextDescriptor, "CodeSys load");
    // �������� ������ �������.
    strcpy(acCommand,
           "ps aux | awk '/codesyscontrol/ && /Sl/ && !/awk/ {print $3 \"%\"}'");
    pcBashCommandAnswer = xCArchiveSaveParse.BashCommandHandler(acCommand);
    // ������� �����?
    if (pcBashCommandAnswer)
    {
        // ����������� ������ ������.
        pxEventData -> fData = atof(pcBashCommandAnswer);
        // �������� ������ ������������.
        strcpy(pxEventData -> acDimentionParameterText,
               " %");
    }
    else
    {
        // ��� ������.
        pxEventData -> fData = 0;
        strcpy(pxEventData -> acDimentionParameterText,
               " %");
    }

    pxEventData++;
    ui8InfoEvetnsIndex++;

//-----------------------------------------------------------------------------------------------------
    // �������� ������ �������.
    // ��������� id �������.
    (pxEventData -> ui16ID) =
        1;
    // ��������� ��� �������.
    (pxEventData -> ui8Type) =
        CEvents::HANDLED_EVENTS_INFO_TYPE;
    // �������� ������ ���������� ���������.
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

    // ����������� ������ ������.
    pxEventData -> fData = fAverageSumm / 1000;
    // �������� ������ ������������.
    strcpy(pxEventData -> acDimentionParameterText,
           " mS");

    pxEventData++;
    ui8InfoEvetnsIndex++;

//-----------------------------------------------------------------------------------------------------
    // �������� ������ �������.
    // ��������� id �������.
    (pxEventData -> ui16ID) =
        1;
    // ��������� ��� �������.
    (pxEventData -> ui8Type) =
        CEvents::HANDLED_EVENTS_INFO_TYPE;
    // �������� ������ ���������� ���������.
    strcpy(pxEventData -> acTextDescriptor, "CDS marker");

//    float fAverageSumm = 0;
//        for (int i = 0; i < 64; i++)
//        {
//            fAverageSumm += (float)xCCodeSysCycleMeasureRingBuffer.Pop();
//        }
//        fAverageSumm /= 64;

    fAverageSumm = afSysValue[0];

    // ����������� ������ ������.
    pxEventData -> fData = fAverageSumm;
//    // �������� ������ ������������.
//    strcpy(pxEventData -> acDimentionParameterText,
//           " mS");

    pxEventData++;
    ui8InfoEvetnsIndex++;

//-----------------------------------------------------------------------------------------------------
    // �������� ������ �������.
    // ��������� id �������.
    (pxEventData -> ui16ID) =
        1;
    // ��������� ��� �������.
    (pxEventData -> ui8Type) =
        CEvents::HANDLED_EVENTS_INFO_TYPE;
    // �������� ������ ���������� ���������.
    strcpy(pxEventData -> acTextDescriptor, "CDS cyclelength");

//    float fAverageSumm = 0;
//        for (int i = 0; i < 64; i++)
//        {
//            fAverageSumm += (float)xCCodeSysCycleMeasureRingBuffer2.Pop();
//        }
//        fAverageSumm /= 64;

    fAverageSumm = afSysValue[1] / 1000;

    // ����������� ������ ������.
    pxEventData -> fData = fAverageSumm;
    // �������� ������ ������������.
    strcpy(pxEventData -> acDimentionParameterText,
           " mS");

    pxEventData++;
    ui8InfoEvetnsIndex++;

//-----------------------------------------------------------------------------------------------------
    // �������� ������ �������.
    // ��������� id �������.
    (pxEventData -> ui16ID) =
        1;
    // ��������� ��� �������.
    (pxEventData -> ui8Type) =
        CEvents::HANDLED_EVENTS_INFO_TYPE;
    // �������� ������ ���������� ���������.
    strcpy(pxEventData -> acTextDescriptor, "CDS deltarun");

//    float fAverageSumm = 0;
//        for (int i = 0; i < 64; i++)
//        {
//            fAverageSumm += (float)xCCodeSysCycleMeasureRingBuffer3.Pop();
//        }
//        fAverageSumm /= 64;

    fAverageSumm = afSysValue[2] / 1000;

    // ����������� ������ ������.
    pxEventData -> fData = fAverageSumm;
    // �������� ������ ������������.
    strcpy(pxEventData -> acDimentionParameterText,
           " mS");
}

//-----------------------------------------------------------------------------------------------------
// ������� ������� ���������� � �������.
// ��������� ������ ������� � �������� ������ � ������ ������� "������".
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
//    // ������� ��������� �� ����� � ������� �������� � ������� ������� �������.
//    pxSourceCurrentTime =
//        (struct tm*)pxModuleContext ->
//        xModuleContextStatic.
//        pucCurrentTimeBufferPointer;
//
//    // �������� ������� ����� �� ��������� ���������.
//    fAverageSumm = (float)pxSourceCurrentTime -> tm_sec;
    fAverageSumm = afSysValue[0];

    // ����������� ������ ������.
    pxEventData -> fData = fAverageSumm;


    pxEventData++;
    ui8InfoEvetnsIndex++;

//-----------------------------------------------------------------------------------------------------
//    fAverageSumm = afSysValue[1] / 1000;
//    // ����������� ������ ������.
//    pxEventData -> fData = fAverageSumm;

    pxEventData++;
    ui8InfoEvetnsIndex++;

//-----------------------------------------------------------------------------------------------------
//    fAverageSumm = afSysValue[2] / 1000;
//
//    // ����������� ������ ������.
//    pxEventData -> fData = fAverageSumm;
//-----------------------------------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------------------------------
uint8_t CInfoLogQuery::TransmitProcessing(CSerialMT *pxCSerialMT, uint8_t ui8Address)
{
    uint8_t ui8Length;
    uint8_t *pui8Destination;
    // ������ �������������� ����������� ���������.
    int16_t i16IndexQuery;
    CEvents::TEventDataCommon *pxSource;
    CEvents *pxCEvents;
    pxCEvents = &xCInfoEvent;

    // ������� ������ ������� �������������� �������.
    i16IndexQuery = (CEvents::xEventsLogQueryList.i16Index -
                     CArchiveEventsDB::EVENTS_DB_PREVIOUS_RECORDS_QUANTITY);
    // ������ ������ �������?
    if (i16IndexQuery != i16LastRequestIndex)
    {
        // ���������� ��� ������, ������� ��������� ��������.
        // �������� ����� ���������� ���������.
        ui8Length =
            (CEvents::xEventsLogQueryList.ui8Quantity *
             sizeof(struct CEvents::TEventDataPackOne));
    }
    else
    {
        // ���������� ��������� ������, ���������� �����������.
        // �������� ����� ���������� ���������.
        ui8Length =
            (CEvents::xEventsLogQueryList.ui8Quantity *
             sizeof(struct CEvents::TEventDataDinamicPackOne));
    }

    // ������, ���� �� ����� ��� ���������.
    pui8Destination = pxCSerialMT -> MessagePushRequest(ui8Length,
                      ui8Address);
    // ��� ����� ��� ��������?
    if (!pui8Destination)
    {
        return 0;
    }
    else
    {
        // �������� ���������.
        // ������� ���������� ��������� ������� � �������.
        ui8Length = CEvents::xEventsLogQueryList.ui8Quantity;
        // ���������� ���������� ������� �����������
        // ������ �������?
        if (ui8Length >
                (CEvents::REQUESTED_EVENTS_QUANTITY))
        {
            // ������.
            return 0;
        }

        // ������������� ������ ������?
        if (!CEvents::xEventsLogQueryList.i16Index)
        {
            InfoEvetnsLogCreate();
        }
        else
        {
            InfoEvetnsLogUpdate();
        }

        // ������ ������ �������?
        if (i16IndexQuery != i16LastRequestIndex)
        {
            // �������� ������� ������ �������.
            i16LastRequestIndex = i16IndexQuery;
            CEvents::TEventDataPackOne *pxDestination;
            // ��������� ������ � ������� ���������� �� ������� � ������� ����� �������.
            pxSource = (xCInfoEvent.EventGetBuffer() + i16IndexQuery);

            // ������� ��������� �� ����� ���������� ��������� � ����� ���������.
            pxDestination = (CEvents::TEventDataPackOne*)(pui8Destination);
            // �������� ������ ������� �� ��������� ���������.
            for (uint16_t i = 0;
                    i < ui8Length;
                    i++)
            {
                // ���� ������ - ��� �������?
                if ((pxSource -> ui16ID) == 0)
                {
                    // ������� ��� �������.
                    pxDestination -> ui8Type =
                        pxSource -> ui8Type;
                    // ������ ������������� ������.
                    // ��� ������������� ��� ������ ������ ��� ������ � ����.
                    pxDestination -> i16Index =
                        (i16IndexQuery);
                    // ������� ������� �������� - ��� �������.
                    pxDestination -> ui16ID = 0;
                }
                else
                {
                    // ������� id �������.
                    pxDestination -> ui16ID =
                        pxSource -> ui16ID;
                    // ������� ��� �������.
                    pxDestination -> ui8Type =
                        pxSource -> ui8Type;
                    // ������ �������������� �������.
                    // ��� ������������� ��� ������ ������� ��� ������ � ����.
                    pxDestination -> i16Index =
                        (i16IndexQuery);
                    // ������� �������� �������.
                    pxDestination -> fData =
                        pxSource -> fData;
                    // ��������� ������ ������������.
                    memcpy(&(pxDestination -> acDimentionParameterText),
                           (&(pxSource -> acDimentionParameterText)),
                           (DIMENSIONS_PARAMETERS_NAME_LENGTH + END_OF_STRING_LENGTH));
                    // ��������� ������ ���������� ���������.
                    memcpy(&(pxDestination -> acTextDescriptor),
                           (&(pxSource -> acTextDescriptor)),
                           (CEvents::EVENT_NAME_LENGTH + END_OF_STRING_LENGTH));
                }
                pxSource++;
                pxDestination++;
                // ������ ���������� �������������� �������.
                i16IndexQuery++;
            }
        }
        else
        {
            CEvents::TEventDataDinamicPackOne *pxDestination;

            // ��������� ������ � ������� ���������� �� ������� � ������� ����� �������.
            pxSource = (xCInfoEvent.EventGetBuffer() + i16IndexQuery);
            // ������� ��������� �� ����� ���������� ��������� � ����� ���������.
            pxDestination = (CEvents::TEventDataDinamicPackOne*)(pui8Destination);
            // �������� ������ ������� �� ��������� ���������.
            for (uint16_t i = 0;
                    i < ui8Length;
                    i++)
            {
                // ���� ������ - ��� �������?
                if ((pxSource -> ui16ID) == 0)
                {
                    // ������ ������������� ������.
                    // ��� ������������� ��� ������ ������ ��� ������ � ����.
                    pxDestination -> i16Index =
                        (i16IndexQuery);
                    // ������� ������� �������� - ��� �������.
                    pxDestination -> ui16ID = 0;
                }
                else
                {
                    // ������� id �������.
                    pxDestination -> ui16ID =
                        pxSource -> ui16ID;
                    // ������ �������������� �������.
                    // ��� ������������� ��� ������ ������� ��� ������ � ����.
                    pxDestination -> i16Index =
                        (i16IndexQuery);
                    // ������� �������� �������.
                    pxDestination -> fData =
                        pxSource -> fData;
                }
                pxSource++;
                pxDestination++;
                // ������ ���������� �������������� �������.
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

        // ������� ��������� �� ����� ���������� ��������� � ����� ���������.
        pxSource = (CEvents::TEventsLogQueryListPackOne*)pxCSerialMT -> pui8DataPointer;
        // ������� ��������� �� ������� ������ �� ������� �������� �������.
        pxDestination =
            (CEvents::TEventsLogQueryListPackOne*)&CEvents::xEventsLogQueryList;
        // �������� �������� ������ �������� ������� � ������� ������.
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
    // �������� ����� ���������� ���������.
    // ������ ������� ������������ ���� ���� ���� - ����� �������.
    ui8Length =
        (sizeof(struct TConfigDataPackOne) + 1);
    // ������, ���� �� ����� ��� ���������.
    pui8Destination = pxCSerialMT -> MessagePushRequest(ui8Length,
                      ui8Address);
    // ��� ����� ��� ��������?
    if (!pui8Destination)
    {
        return 0;
    }
    else
    {
        // �������� ���������.
        // ������� ��������� �� ����� ��� ���������� ��������� � ����� ���������.
        // �������� Modbus ����� ������� �� ��������� ���������.
        *(pui8Destination) = ucModbusSlaveAddress;
        // �������� ������������ ������� �� ��������� ���������.
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
    // �������� ����� ���������� ���������.
    ui8Length = CURRENT_TIME_BYTE_QUANTITY;
    // ������, ���� �� ����� ��� ���������.
    pui8Destination = pxCSerialMT -> MessagePushRequest(ui8Length,
                      ui8Address);
    // ��� ����� ��� ��������?
    if (!pui8Destination)
    {
        return 0;
    }
    else
    {
        struct tm *pxSource;
        // ������� ��������� �� ����� � ������� �������� � ������� ������� �������.
        pxSource =
            (struct tm*)pxModuleContext ->
            xModuleContextStatic.
            pucCurrentTimeBufferPointer;

        // �������� ������� ����� �� ��������� ���������.
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
    // �������� ����� ���������� ���������.
    ui8Length = TASK_HIGH_LEVEL_FLAGS_MESSAGE_LENGTH;
    // ������, ���� �� ����� ��� ���������.
    pui8Destination = pxCSerialMT -> MessagePushRequest(ui8Length,
                      ui8Address);
    // ��� ����� ��� ��������?
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
        // ������� ��������� �� ����� ���������� ��������� � ����� ���������.
        // ������� � ��������� ������� �����.
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
        // �������� ���������.
        switch (xTPlcSettingsQueryPackOne.ui8Type)
        {
        case RS485_HIGH_LEVEL_SET:
            // �������� ����� ���������� ���������.
            ui8Length = TASK_CONFIRMATION_LENGTH;
            // ������, ���� �� ����� ��� ���������.
            pui8Destination = pxCSerialMT -> MessagePushRequest(ui8Length,
                              ui8Address);
            // ��� ����� ��� ��������?
            if (!pui8Destination)
            {
                return 0;
            }
            else
            {
                // �������� ���������.
                (*pui8Destination) = 0;
                return 1;
            }
            break;
        case ARCHIVE_LOG_SAVE:
            // ���������� ������������ ������.
            ui8Length = sizeof(struct TPlcSettingsExchangePackOne);
            // ������, ���� �� ����� ��� ���������.
            pui8Destination = pxCSerialMT -> MessagePushRequest(ui8Length,
                              ui8Address);
            // ��� ����� ��� ��������?
            if (!pui8Destination)
            {
                return 0;
            }
            else
            {
                TPlcSettingsExchangePackOne *pxSource;
                pxSource = &xTPlcSettingsExchangePackOne;
                uint8_t ui8ChoiceOption;
                // ������� ��������� �� ����� ���������� ��������� � ����� ���������.
                pxDestination = (TPlcSettingsExchangePackOne*)(pui8Destination);
                // �������� ������ �� ��������� ���������.
                pxDestination -> ui8Type = ARCHIVE_LOG_SAVE;
                pxDestination -> ui8ChoiceOption = pxSource -> ui8ChoiceOption;
                pxDestination -> ui8AnswerCode = pxSource -> ui8AnswerCode;
                return 1;
            }
            break;
        case PROGRAMM_FILE_UPDATE:
            // ���������� ������������ ������.
            ui8Length = sizeof(struct TPlcSettingsExchangePackOne);
            // ������, ���� �� ����� ��� ���������.
            pui8Destination = pxCSerialMT -> MessagePushRequest(ui8Length,
                              ui8Address);
            // ��� ����� ��� ��������?
            if (!pui8Destination)
            {
                return 0;
            }
            else
            {
                TPlcSettingsExchangePackOne *pxSource;
                pxSource = &xTPlcSettingsExchangePackOne;
                uint8_t ui8ChoiceOption;
                // ������� ��������� �� ����� ���������� ��������� � ����� ���������.
                pxDestination = (TPlcSettingsExchangePackOne*)(pui8Destination);
                // �������� ������ �� ��������� ���������.
                pxDestination -> ui8Type = PROGRAMM_FILE_UPDATE;
                pxDestination -> ui8ChoiceOption = pxSource -> ui8ChoiceOption;
                pxDestination -> ui8AnswerCode = pxSource -> ui8AnswerCode;
                return 1;
            }
            break;
        case DEBUG_LOG_FILE_SAVE:
            // ���������� ������������ ������.
            ui8Length = sizeof(struct TPlcSettingsExchangePackOne);
            // ������, ���� �� ����� ��� ���������.
            pui8Destination = pxCSerialMT -> MessagePushRequest(ui8Length,
                              ui8Address);
            // ��� ����� ��� ��������?
            if (!pui8Destination)
            {
                return 0;
            }
            else
            {
                TPlcSettingsExchangePackOne *pxSource;
                pxSource = &xTPlcSettingsExchangePackOne;
                uint8_t ui8ChoiceOption;
                // ������� ��������� �� ����� ���������� ��������� � ����� ���������.
                pxDestination = (TPlcSettingsExchangePackOne*)(pui8Destination);
                // �������� ������ �� ��������� ���������.
                pxDestination -> ui8Type = DEBUG_LOG_FILE_SAVE;
                pxDestination -> ui8ChoiceOption = pxSource -> ui8ChoiceOption;
                pxDestination -> ui8AnswerCode = pxSource -> ui8AnswerCode;
                return 1;
            }
            break;
        case ETHERNET_SET:
            // �������� ����� ���������� ���������.
            ui8Length = TASK_CONFIRMATION_LENGTH;
            // ������, ���� �� ����� ��� ���������.
            pui8Destination = pxCSerialMT -> MessagePushRequest(ui8Length,
                              ui8Address);
            // ��� ����� ��� ��������?
            if (!pui8Destination)
            {
                return 0;
            }
            else
            {
                // �������� ���������.
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
        // �������� ���������.
        switch (xTPlcSettingsQueryPackOne.ui8Type)
        {
        case RS485_HIGH_LEVEL_SET:
            // ���������� ������������ ������.
            ui8Length = sizeof(struct TPlcSettingsExchangePackOne);
            // ������, ���� �� ����� ��� ���������.
            pui8Destination = pxCSerialMT -> MessagePushRequest(ui8Length,
                              ui8Address);
            // ��� ����� ��� ��������?
            if (!pui8Destination)
            {
                return 0;
            }
            else
            {
                TPortSettingsPackOne *pxSource;
                // ������� ��������� �� ������.
                pxSource = (TPortSettingsPackOne*)&xTPlcSettingsPackOne.xTRs485HighLevelSettingsPackOne;
                // ������� ��������� �� ����� ���������� ��������� � ����� ���������.
                pxDestination = (TPlcSettingsExchangePackOne*)(pui8Destination);
                // �������� ������ �� ��������� ���������.
                pxDestination -> ui8Type = RS485_HIGH_LEVEL_SET;
                pxDestination -> ui8BaudRate = pxSource -> ui8BaudRate;
                pxDestination -> ui8StopBits = pxSource -> ui8StopBits;
                pxDestination -> ui8Parity = pxSource -> ui8Parity;
                return 1;
            }
            break;
        case ARCHIVE_LOG_SAVE:
            // ���������� ������������ ������.
            ui8Length = sizeof(struct TPlcSettingsExchangePackOne);
            // ������, ���� �� ����� ��� ���������.
            pui8Destination = pxCSerialMT -> MessagePushRequest(ui8Length,
                              ui8Address);
            // ��� ����� ��� ��������?
            if (!pui8Destination)
            {
                return 0;
            }
            else
            {
                xCArchiveSaveParse.GetDiskInfo();
                // ������� ��������� �� ����� ���������� ��������� � ����� ���������.
                pxDestination = (TPlcSettingsExchangePackOne*)(pui8Destination);
                // �������� ������ �� ��������� ���������.
                pxDestination -> ui8Type = ARCHIVE_LOG_SAVE;
                pxDestination -> ui8ChoiceOption = 0;
                // �������� ������ ����������� ��������� ������ ��� ����.
                for (uint8_t i = 0, j = 0, k = 0, l = 0;
                        (i < CHOICE_OPTION_TEXT_NUMBER);
                        i++)
                {
                    // ���� USB ����?
                    if (memcmp((xCArchiveSaveParse.axTDiskInfo[i].acName),
                               "sd",
                               2) == 0)
                    {
                        // �������� ������ � ������ � �������� ���������� USB �����.
                        sprintf((char*)(pxDestination -> axChoiceOptionTextData[j].acChoiceOptionTextData),
                                ("%s%d %s"),
                                ("USB"),
                                k,
                                (xCArchiveSaveParse.axTDiskInfo[i].acSize));
                        // ��������� ������ � ����� �����.
                        k++;
                        // ��������� ����.
                        j++;
                    }
                    // ���� SD �����?
                    else if (memcmp((xCArchiveSaveParse.axTDiskInfo[i].acName),
                                    "mmc",
                                    3) == 0)
                    {
                        // �������� ������ � ������ � �������� ��������� SD �����.
                        sprintf((char*)(pxDestination -> axChoiceOptionTextData[j].acChoiceOptionTextData),
                                ("%s%d %s"),
                                ("SD"),
                                l,
                                (xCArchiveSaveParse.axTDiskInfo[i].acSize));
                        // ��������� ������ � ����� �����.
                        l++;
                        // ��������� ����.
                        j++;
                    }
                    // �������� ���������� ��������� ������.
                    pxDestination -> ui8ChoiceOptionNumber = j;
                }
                return 1;
            }
            break;
        case PROGRAMM_FILE_UPDATE:
            // ���������� ������������ ������.
            ui8Length = sizeof(struct TPlcSettingsExchangePackOne);
            // ������, ���� �� ����� ��� ���������.
            pui8Destination = pxCSerialMT -> MessagePushRequest(ui8Length,
                              ui8Address);
            // ��� ����� ��� ��������?
            if (!pui8Destination)
            {
                return 0;
            }
            else
            {
                xCArchiveSaveParse.GetDiskInfo();
//                xCArchiveSaveParse.GetDiskInfoNew();

                // ������� ��������� �� ����� ���������� ��������� � ����� ���������.
                pxDestination = (TPlcSettingsExchangePackOne*)(pui8Destination);
                // �������� ������ �� ��������� ���������.
                pxDestination -> ui8Type = PROGRAMM_FILE_UPDATE;
                pxDestination -> ui8ChoiceOption = 0;
                // �������� ������ ����������� ��������� ������ ��� ����.
                for (uint8_t i = 0, j = 0, k = 0, l = 0;
                        (i < CHOICE_OPTION_TEXT_NUMBER);
                        i++)
                {
                    // ���� USB ����?
                    if (memcmp((xCArchiveSaveParse.axTDiskInfo[i].acName),
                               "sd",
                               2) == 0)
                    {
                        // �������� ������ � ������ � �������� ���������� USB �����.
                        sprintf((char*)(pxDestination -> axChoiceOptionTextData[j].acChoiceOptionTextData),
                                ("%s%d %s"),
                                ("USB"),
                                k,
                                (xCArchiveSaveParse.axTDiskInfo[i].acSize));
                        // ��������� ������ � ����� �����.
                        k++;
                        // ��������� ����.
                        j++;
                    }
                    // ���� SD �����?
                    else if (memcmp((xCArchiveSaveParse.axTDiskInfo[i].acName),
                                    "mmc",
                                    3) == 0)
                    {
                        // �������� ������ � ������ � �������� ��������� SD �����.
                        sprintf((char*)(pxDestination -> axChoiceOptionTextData[j].acChoiceOptionTextData),
                                ("%s%d %s"),
                                ("SD"),
                                l,
                                (xCArchiveSaveParse.axTDiskInfo[i].acSize));
                        // ��������� ������ � ����� �����.
                        l++;
                        // ��������� ����.
                        j++;
                    }
                    // �������� ���������� ��������� ������.
                    pxDestination -> ui8ChoiceOptionNumber = j;
                }
                return 1;
            }
            break;
        case DEBUG_LOG_FILE_SAVE:
            // ���������� ������������ ������.
            ui8Length = sizeof(struct TPlcSettingsExchangePackOne);
            // ������, ���� �� ����� ��� ���������.
            pui8Destination = pxCSerialMT -> MessagePushRequest(ui8Length,
                              ui8Address);
            // ��� ����� ��� ��������?
            if (!pui8Destination)
            {
                return 0;
            }
            else
            {
                xCArchiveSaveParse.GetDiskInfo();
                // ������� ��������� �� ����� ���������� ��������� � ����� ���������.
                pxDestination = (TPlcSettingsExchangePackOne*)(pui8Destination);
                // �������� ������ �� ��������� ���������.
                pxDestination -> ui8Type = DEBUG_LOG_FILE_SAVE;
                pxDestination -> ui8ChoiceOption = 0;
                // �������� ������ ����������� ��������� ������ ��� ����.
                for (uint8_t i = 0, j = 0, k = 0, l = 0;
                        (i < CHOICE_OPTION_TEXT_NUMBER);
                        i++)
                {
                    // ���� USB ����?
                    if (memcmp((xCArchiveSaveParse.axTDiskInfo[i].acName),
                               "sd",
                               2) == 0)
                    {
                        // �������� ������ � ������ � �������� ���������� USB �����.
                        sprintf((char*)(pxDestination -> axChoiceOptionTextData[j].acChoiceOptionTextData),
                                ("%s%d %s"),
                                ("USB"),
                                k,
                                (xCArchiveSaveParse.axTDiskInfo[i].acSize));
                        // ��������� ������ � ����� �����.
                        k++;
                        // ��������� ����.
                        j++;
                    }
                    // ���� SD �����?
                    else if (memcmp((xCArchiveSaveParse.axTDiskInfo[i].acName),
                                    "mmc",
                                    3) == 0)
                    {
                        // �������� ������ � ������ � �������� ��������� SD �����.
                        sprintf((char*)(pxDestination -> axChoiceOptionTextData[j].acChoiceOptionTextData),
                                ("%s%d %s"),
                                ("SD"),
                                l,
                                (xCArchiveSaveParse.axTDiskInfo[i].acSize));
                        // ��������� ������ � ����� �����.
                        l++;
                        // ��������� ����.
                        j++;
                    }
                    // �������� ���������� ��������� ������.
                    pxDestination -> ui8ChoiceOptionNumber = j;
                }
                return 1;
            }
            break;
        case ETHERNET_SET:
            TEthernetSettingsPackOne *pxTEthernetSettingsPackOneSource;
            // ���������� ������������ ������.
            ui8Length = sizeof(struct TPlcSettingsExchangePackOne);
            // ������, ���� �� ����� ��� ���������.
            pui8Destination = pxCSerialMT -> MessagePushRequest(ui8Length,
                              ui8Address);
            // ��� ����� ��� ��������?
            if (!pui8Destination)
            {
                return 0;
            }
            else
            {
                // ������� ��������� �� ������.
                pxTEthernetSettingsPackOneSource = (TEthernetSettingsPackOne*)&xTPlcSettingsPackOne.xTEthernetSettingsPackOne;
                // ������� ��������� �� ����� ���������� ��������� � ����� ���������.
                pxDestination = (TPlcSettingsExchangePackOne*)(pui8Destination);
                // �������� ������ �� ��������� ���������.
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
// ����� ����������� ����� mmc0 �� mmc1.
void *thread_ArchiveFileSave(void *value)
{
    CSettingsRW::ArchiveSave(CSettingsRW::xTPlcSettingsExchangePackOne.ui8ChoiceOption);
}

pthread_t xDebugLogFileSave;
//-----------------------------------------------------------------------------------------------------
// ����� ����������� ����� mmc0 �� mmc1.
void *thread_DebugLogFileSave(void *value)
{
    CSettingsRW::DebugLogFileSave(CSettingsRW::xTPlcSettingsExchangePackOne.ui8ChoiceOption);
}

//-----------------------------------------------------------------------------------------------------
uint8_t CSettingsRW::ReceiveProcessing(CSerialMT *pxCSerialMT, uint8_t ui8Address)
{
    if ((ui8Address & CSerialMT::SERIAL_MT_WRITE) != 0)
    {
        // ������� ������.
        TPlcSettingsExchangeClientPackOne *pxSource;

        // ������� ��������� �� ����� ���������� ��������� � ����� ���������.
        pxSource = (TPlcSettingsExchangeClientPackOne*)(pxCSerialMT -> pui8DataPointer);

        switch (pxSource -> ui8Type)
        {
        case RS485_HIGH_LEVEL_SET:
            TPortSettingsPackOne *pxDestination;

            // ������� ��������� �� ������.
            pxDestination = (TPortSettingsPackOne*)&xTPlcSettingsPackOne.xTRs485HighLevelSettingsPackOne;
            // ������� ������.
            pxDestination -> ui8BaudRate = pxSource -> ui8BaudRate;
            pxDestination -> ui8StopBits = pxSource -> ui8StopBits;
            pxDestination -> ui8Parity = pxSource -> ui8Parity;
            PlcSettingsSave();
            PlcSettingsLoad();
			// ������� ��������� ���������� ModbusRtu, ����� � ��������� ����� ������� ������, RS-485.
			iModbusRtuMasterReset();
			// �������� ������ ������.
            // ��� ������ �� ��������� �������� ������.
            // ����� �������� ��������� ����������.
            pthread_kill(xModbusRtu, SIGRTMIN+1);
            break;
        case ARCHIVE_LOG_SAVE:
            // ������� ������.
            xTPlcSettingsExchangePackOne.ui8Type = pxSource -> ui8Type;
            xTPlcSettingsExchangePackOne.ui8ChoiceOption = pxSource -> ui8ChoiceOption;
            xTPlcSettingsExchangePackOne.ui8ChoiceOptionNumber = pxSource -> ui8ChoiceOptionNumber;
            xTPlcSettingsExchangePackOne.ui8AnswerCode = pxSource -> ui8AnswerCode;

            switch (ui8ArchiveFileIsSaveState)
            {
            case WRITE_IDDLE:
                // �������� ����� ���������� ����� ������.
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
            // ������� ������.
            xTPlcSettingsExchangePackOne.ui8Type = pxSource -> ui8Type;
            xTPlcSettingsExchangePackOne.ui8ChoiceOption = pxSource -> ui8ChoiceOption;
            xTPlcSettingsExchangePackOne.ui8ChoiceOptionNumber = pxSource -> ui8ChoiceOptionNumber;
            xTPlcSettingsExchangePackOne.ui8AnswerCode = pxSource -> ui8AnswerCode;
            // ����� ������� �������?
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
            // ������� ������.
            xTPlcSettingsExchangePackOne.ui8Type = pxSource -> ui8Type;
            xTPlcSettingsExchangePackOne.ui8ChoiceOption = pxSource -> ui8ChoiceOption;
            xTPlcSettingsExchangePackOne.ui8ChoiceOptionNumber = pxSource -> ui8ChoiceOptionNumber;
            xTPlcSettingsExchangePackOne.ui8AnswerCode = pxSource -> ui8AnswerCode;

            switch (ui8DebugLogFileSaveState)
            {
            case WRITE_IDDLE:
                // �������� ����� ���������� �����.
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

            // ������� ��������� �� ������.
            pxTEthernetSettingsPackOneDestination = (TEthernetSettingsPackOne*)&xTPlcSettingsPackOne.xTEthernetSettingsPackOne;
            // ������� ������.
            pxTEthernetSettingsPackOneDestination -> ui8IpByte0 = pxSource -> ui8IpByte0;
            pxTEthernetSettingsPackOneDestination -> ui8IpByte1 = pxSource -> ui8IpByte1;
            pxTEthernetSettingsPackOneDestination -> ui8IpByte2 = pxSource -> ui8IpByte2;
            pxTEthernetSettingsPackOneDestination -> ui8IpByte3 = pxSource -> ui8IpByte3;
            pxTEthernetSettingsPackOneDestination -> ui16Port = pxSource -> ui16Port;
            PlcSettingsSave();
            PlcSettingsLoad();
			// ������� ��������� ���������� ModbusRtu, ����� � ��������� ����� ������� ������, RS-485.
			iModbusRtuMasterReset();
            // �������� ������ ������.
            // ��� ������ �� ��������� �������� ������.
            // ����� �������� ��������� ����������.
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
        // ������� ������.
        TPlcSettingsQueryPackOne *pxSource;
        TPlcSettingsQueryPackOne *pxDestination;

        // ������� ��������� �� ����� ���������� ��������� � ����� ���������.
        pxSource = (TPlcSettingsQueryPackOne*)(pxCSerialMT -> pui8DataPointer);
        // ������� ��������� �� ������ ��������.
        pxDestination = &xTPlcSettingsQueryPackOne;
        *pxDestination = *pxSource;
        return 0;
    }
}

//-----------------------------------------------------------------------------------------------------
// ��������� ����� �� ������� ����.
void CSettingsRW::ArchiveSave(uint8_t ui8ChoiceOption)
{
    char acCommand[128];
    const char *pccParseFileName;
    // �������� ������� ������������ ���������� �����.
    sprintf(acCommand,
            "%s%s %s",
            "sudo mount /dev/",
            (xCArchiveSaveParse.axTDiskInfo[((ui8ChoiceOption) & 0x01)].acName),
            "/mnt/usb"
           );
    // ������������ ��������� ����.
    system(acCommand);
    usleep(100000);

    // �������� ������� ���������� � ��������� �����.
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

    // �������� ����.
    // ���� ������� �������?
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
// ��������� ����� �� ������� ����.
void CSettingsRW::DebugLogFileSave(uint8_t ui8ChoiceOption)
{
    char acCommand[128];
    const char *pccParseFileName;
    // �������� ������� ������������ ���������� �����.
    sprintf(acCommand,
            "%s%s %s",
            "sudo mount /dev/",
            (xCArchiveSaveParse.axTDiskInfo[((ui8ChoiceOption) & 0x01)].acName),
            "/mnt/usb"
           );
    // ������������ ��������� ����.
    system(acCommand);
    usleep(100000);

    // �������� ������� ���������� � ��������� �����.
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

    // �������� ����.
    // ���� ������� �������?
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
// ��������� ���� ���������.
uint8_t CSettingsRW::ProgrammFileUpdate(uint8_t ui8ChoiceOption)
{
    char acCommand[128];

    // �������� ������� ������������ ���������� �����.
    sprintf(acCommand,
            "%s%s %s",
            "sudo mount /dev/",
            (xCArchiveSaveParse.axTDiskInfo[((ui8ChoiceOption) & 0x01)].acName),
            "/mnt/usb && sync"
           );
    // ������������ ��������� ����.
    system(acCommand);
    usleep(100000);

    // �������� ������� ����������.
    if ((system("sudo cp -f /mnt/usb/pas05_software_update/pas05_software_update.sh /home/debian/pas05_software_update.sh && sync")))
    {
        return 0;
    }

    // �������� ������� ������� �����.
    if ((system("sudo chmod +x /home/debian/pas05_software_update.sh && sync")))
    {
        return 0;
    }

    // �������� �������� ���������� ��(pas05_software_update.sh).
    if ((system("sudo /home/debian/pas05_software_update.sh && sync")))
    {
        return 0;
    }

    return 1;
}

//-----------------------------------------------------------------------------------------------------
// �������� ����� ��������.
uint8_t CSettingsRW::PlcSettingsCheck(void)
{
    const char *pxFileName;
    FILE *pxPlcSettingsFile;

    // ������� ���� ���� ������.
    pxFileName = pccSettingsFileName;
    pxPlcSettingsFile = fopen(pxFileName,"r+");
    if(pxPlcSettingsFile == NULL)
    {
        // ���� �� ����������.
        printf("Cannot open PlcSettings file.\n");
        // �������� ����.
        pxPlcSettingsFile = fopen(pxFileName,"w+");
        if(pxPlcSettingsFile == NULL)
        {
            printf("Cannot create PlcSettings file.\n");
            return -1;
        }
    }
    // �������� ����������� �����.
    // ��������� �������� � �����.
    fseek (pxPlcSettingsFile, 0, SEEK_SET);
    // ��������� ���������� ����� ���� ������ � RAM.
    fread((void*)&xTPlcSettingsPackOne,
          sizeof(struct TPlcSettingsPackOne),
          1,
          pxPlcSettingsFile);

    // ���� ��������?
    if ((usCrc16((unsigned char*)&xTPlcSettingsPackOne,
                 (sizeof(TPlcSettingsPackOne) - CRC16_LENGTH)) != xTPlcSettingsPackOne.ui16Crc))
    {
        printf("PlcSettings CRC wrong.\n");

        memcpy(&xTPlcSettingsPackOne,
               &xTPlcSettingsDefaultPackOne,
               ((sizeof(TPlcSettingsPackOne) - CRC16_LENGTH)));

        xTPlcSettingsPackOne.ui16Crc = usCrc16((unsigned char*)&xTPlcSettingsPackOne,
                                               (sizeof(TPlcSettingsPackOne)) - CRC16_LENGTH);
        // ��������� �������� � �����.
        fseek (pxPlcSettingsFile, 0, SEEK_SET);
        fwrite((const void*)&(xTPlcSettingsPackOne),
               sizeof(struct TPlcSettingsPackOne),
               1,
               pxPlcSettingsFile);

        printf("Default PlcSettings create ok.\n");
    }
    else
    {
        // ���� �� ��������.
        printf("PlcSettings CRC ok.\n");
    }

    fclose(pxPlcSettingsFile);
}

//-----------------------------------------------------------------------------------------------------
// ���������� ����� ��������.
uint8_t CSettingsRW::PlcSettingsSave(void)
{
    const char *pxFileName;
    FILE *pxPlcSettingsFile;

    // ������� ����.
    pxFileName = pccSettingsFileName;
    pxPlcSettingsFile = fopen(pxFileName,"r+");
    if(pxPlcSettingsFile == NULL)
    {
        // ���� �� ����������.
        printf("Cannot open PlcSettings file.\n");
    }
    else
    {
        // ������� CRC.
        xTPlcSettingsPackOne.ui16Crc = usCrc16((unsigned char*)&xTPlcSettingsPackOne,
                                               (sizeof(TPlcSettingsPackOne)) - CRC16_LENGTH);
        // ��������� �������� � �����.
        fseek (pxPlcSettingsFile, 0, SEEK_SET);
        // �������� ����.
        fwrite((const void*)&(xTPlcSettingsPackOne),
               sizeof(struct TPlcSettingsPackOne),
               1,
               pxPlcSettingsFile);
    }
    // ������� ����.
    fclose(pxPlcSettingsFile);
}

//-----------------------------------------------------------------------------------------------------
// ��������� ���������.
uint8_t CSettingsRW::PlcSettingsLoad(void)
{
    char acCommand[128];
    unsigned char *pucSource;
    unsigned char nucDataBaseBlockIndex;
//-----------------------------------------------------------------------------------------------------
// DataBaseBlockRead
//-----------------------------------------------------------------------------------------------------
    // ����� ����� ���� ������.
    nucDataBaseBlockIndex = (unsigned char)MBM_FUNCTION_1234_BLOCK_DATA_BASE_BLOCK_OFFSET + 1;
    pucSource = (unsigned char*)&(xPlcDataBase.
                                  axPlcDataBaseBlocks[nucDataBaseBlockIndex].
                                  aucPlcDataBaseBlockData[MBM_FUNCTION_1234_BLOCK_DATA_BASE_BLOCK_LENGTH - 1]);

    // ��������� �������� ���������� RS485 ������� ������.
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

    // ��������� �������� �������� ���������� RS485 ������� ������.
    switch((*pucSource & 0x03))
    {
    case PARITY_NO:
        cModbusRtuMasterParity = 'N';
        // ��������� ���������� ���� ��� ���������� RS485 ������� ������.
        ui8ModbusRtuMasterStopBits = 2;
        break;
    case PARITY_NO_ONE_STOP:
        cModbusRtuMasterParity = 'N';
        // ��������� ���������� ���� ��� ���������� RS485 ������� ������.
        ui8ModbusRtuMasterStopBits = 1;
        break;
    case PARITY_EVEN:
        cModbusRtuMasterParity = 'E';
        // ��������� ���������� ���� ��� ���������� RS485 ������� ������.
        ui8ModbusRtuMasterStopBits = 1;
        break;
    case PARITY_ODD:
        cModbusRtuMasterParity = 'O';
        // ��������� ���������� ���� ��� ���������� RS485 ������� ������.
        ui8ModbusRtuMasterStopBits = 1;
        break;
    default:
        cModbusRtuMasterParity = 'N';
        // ��������� ���������� ���� ��� ���������� RS485 ������� ������.
        ui8ModbusRtuMasterStopBits = 2;
        break;
    };

    // ��������� ���������� ��� ���������� RS485 ������� ������.
    ui8ModbusRtuMasterDataBits = 8;

    // ��������� �������� ���������� RS485 �������� ������.
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
    // ��������� �������� �������� ���������� RS485 �������� ������.
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

    // ��������� ���������� ��� ���������� RS485 �������� ������.
    ui8ModbusRtuDataBits = 8;
    // ��������� ���������� ���� ��� ���������� RS485 �������� ������.
    ui8ModbusRtuStopBits = (xTPlcSettingsPackOne.xTRs485HighLevelSettingsPackOne.ui8StopBits +
                            CONVERT_INTEGER_TO_NATURAL_NUMBER);

    // �������� ������� ��������� IP ���������� TCP �������� ������.
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
    // ��������� IP ���������� TCP �������� ������.
    system(acCommand);
    // ��������� ���� ���������� TCP �������� ������.
    ui16ModbusTcpPort = xTPlcSettingsPackOne.xTEthernetSettingsPackOne.ui16Port;
}

//-----------------------------------------------------------------------------------------------------



//-----------------------------------------------------------------------------------------------------
uint8_t CRs485Set::TransmitProcessing(CSerialMT *pxCSerialMT, uint8_t ui8Address)
{
    uint8_t ui8Length;
    uint8_t *pui8Destination;
    // �������� ����� ���������� ���������.
    ui8Length = TASK_HIGH_LEVEL_FLAGS_MESSAGE_LENGTH;
    // ������, ���� �� ����� ��� ���������.
    pui8Destination = pxCSerialMT -> MessagePushRequest(ui8Length,
                      ui8Address);
    // ��� ����� ��� ��������?
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
    // �������� ����� ���������� ���������.
    ui8Length = (CSerialMT::MAX_RECEIVED_FRAME_LENGTH - (pxCSerialMT -> ui8MessageBagLength));
    // ������, ���� �� ����� ��� ���������.
    pui8Destination = pxCSerialMT -> MessagePushRequest(ui8Length,
                      ui8Address);
    // ��� ����� ��� ��������?
    if (!pui8Destination)
    {
        return 0;
    }
    else
    {
        // �������� ���������.
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

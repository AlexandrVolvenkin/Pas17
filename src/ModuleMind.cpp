
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
// �������������� ������ � ���������� �����.
// (���: W - �����, ����: E - ������)
CMind::TTaskContext CMind::axAllTasksContext[] =
{
    // ������-��������.
    // ���������� �� ��������� �� ������� � ���������� ����� �� ������-������� 0.
    // �����: 0 - �� �������� ����������(SerialMT).
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
    // ������� ����� ����������.
    ui8HighLevelFlagsSetSave = 0;
    // ������� ���� ��������� ������ ���������� �� ���� � �� - F_HLEV ���������.
    ui8HighLevelFlagsIsUpdated = 0;
    // ������� ���������� ������� ��� �� ������������ � ����.
    i16AllReceivedRecordsQuantity = 0;
    i16LastAllReceivedRecordsQuantity = 0;
    // ������� ��������� ������ ������� � ������ ��������.
    ui16LastIndexQuery = 0;



    CError::fBadPacketQuantity = 0;
    CError::fOkPacketQuantity = 0;
}

////-----------------------------------------------------------------------------------------------------
//// ���������� ����� ������� ��� � ����.
//int16_t CMind::DataExchangeSpi(TModuleContext *pxModuleContext)
//{
//    int16_t i;
//    CSerialMT::TSerialMTData xData;
//    CMind::TTaskContext *pxTaskContext;
//
//    // ������� ��������� �� �������� �����.
//    pxTaskContext = CMind::axAllTasksContext;
//    // ������� ���������� �������-�����.
//    CMind::i16TasksQuantity =
//        (sizeof(CMind::axAllTasksContext) /
//         sizeof(CMind::TTaskContext));
//
////-----------------------------------------------------------------------------------------------------
//    // ����� ������� �� SPI.
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
//    // ������ ���������?
//    if (i != -1)
//    {
//        // ������� �������� ����� ���������.
//        // ������� ��������� �� ����� ��������� ������ ���������.
//        // ������� ����� ��������� ������ ���������.
//        // ��������� ����� ������ ������ ���������(CSerialMT::TSerialMTData xData).
//        if (CSpiMT::DataReceive(&xData) != -1)
//        {
//            // ��������� ��������� �� ����� ��������� ������ ���������.
//            // ��������� ����� ��������� ������ ���������.
//            // ��������� � ������� ����� ������ ������ ���������(CSerialMT::TSerialMTData xData).
//            if (CMind::RequestHandler(pxModuleContext,
//                                      CMind::axAllTasksContext,
//                                      &xData) == -1)
//            {
//                return -1;
//            }
//        }
//    }
//
//    // ������� ��������� �� ����� �������� ������ ���������.
//    xData.pui8DataPointer = CSpiMT::GetTransmitMessageBuff();
//    // ��������� � ������� ����� ������ ������ ���������(CSerialMT::TSerialMTData xData).
//    CMind::AnswerHandler(pxModuleContext,
//                         CMind::axAllTasksContext,
//                         &xData);
//
//    // ��������� ������ ���������� ��������� SerialMT � ������ �������� ������ �� Modbus.
//    CSpiMT::DataTransmit(&xData);
//
//    return 0;
//}
//
////-----------------------------------------------------------------------------------------------------
//// ���������� ����� ������� ��� � ����.
//int16_t CMind::DataExchangeModbus(TModuleContext *pxModuleContext)
//{
//    int16_t i;
//    CSerialMT::TSerialMTData xData;
//    CMind::TTaskContext *pxTaskContext;
//
//    // ������� ��������� �� �������� �����.
//    pxTaskContext = CMind::axAllTasksContext;
//    // ������� ���������� �������-�����.
//    CMind::i16TasksQuantity =
//        (sizeof(CMind::axAllTasksContext) /
//         sizeof(CMind::TTaskContext));
////-----------------------------------------------------------------------------------------------------
//    // ����� ������� �� Modbus.
//    // ��������� ����� ������ ��������� ���� ������� ������.
//    modbus_set_slave(pxMasterTcpCtx,
//                     MODULE_MIND_MODBUS_ADDRESS);
//    // ���������� ������� � ������� ��������� ���� ������� ������ �� ��������� �����.
//    i = modbus_data_exchange(pxMasterTcpCtx,
//                             CModbusMT::nui16TxFrameLength,
//                             CModbusMT::GetModbusTxBuff(),
//                             CModbusMT::GetModbusRxBuff());
//
////-----------------------------------------------------------------------------------------------------
//    // ������ ���������?
//    if (i != -1)
//    {
//        CModbusMT::nui16RxFrameLength = i;
//        // ������� �������� ����� ���������.
//        // ������� ��������� �� ����� ��������� ������ ���������.
//        // ������� ����� ��������� ������ ���������.
//        // ��������� ����� ������ ������ ���������(CSerialMT::TSerialMTData xData).
//        if (CModbusMT::DataReceive(&xData) != -1)
//        {
//            // ��������� ��������� �� ����� ��������� ������ ���������.
//            // ��������� ����� ��������� ������ ���������.
//            // ��������� � ������� ����� ������ ������ ���������(CSerialMT::TSerialMTData xData).
//            if (CMind::RequestHandler(pxModuleContext,
//                                      CMind::axAllTasksContext,
//                                      &xData) == -1)
//            {
//                return -1;
//            }
//        }
//    }
//
//    // ������� ��������� �� ����� �������� ������ ���������.
//    xData.pui8DataPointer = CModbusMT::GetModbusTxBuff();
//    // ��������� � ������� ����� ������ ������ ���������(CSerialMT::TSerialMTData xData).
//    CMind::AnswerHandler(pxModuleContext,
//                         CMind::axAllTasksContext,
//                         &xData);
//
//    // ��������� ������ ���������� ��������� SerialMT � ������ �������� ������ �� Modbus.
//    CModbusMT::DataTransmit(&xData);
//
//    return 0;
//}

////-----------------------------------------------------------------------------------------------------
//// ������������ ������� � ������ �� �������-����� �� ������ �����.
//int16_t CMind::RequestHandler(TModuleContext *pxModuleContext,
//                              CMind::TTaskContext *pxTaskContext,
//                              CSerialMT::TSerialMTData *pxData)
//{
//    CSerialMT SerialMT;
//    uint8_t ui8Index;
//    CSerialMT::TSerialMTData xDataSave;
//
//    // ��������� ������ ������ ���������(CSerialMT::TSerialMTData xData).
//    SerialMT.vPrepareGetMessageBag(pxData);
//    // �������� ������ ��������� ���������.
//    // ��������� � �������-������ � ������� ������ ���������(CSerialMT::TSerialMTData xData)
//    // ��������� �� ��������� ��������� � ������ ���������.
//    if (SerialMT.iGetDataMessageBag(pxData) == -1)
//    {
//        return -1;
//    }
//    // �������� ������ ��������� ��������� ��������� ��� �������-������(�� ������ pxData -> ui8Address)
//    xDataSave = *pxData;
//    // �������� ��������� ��������� ��� ���� ������.
//    for (ui8Index = 0;
//            (ui8Index < CMind::i16TasksQuantity);
//            ui8Index++)
//    {
//        // ���������� ��������� ��� ������� �������-������(�� ������� ui8Index)?
//        if ((ui8Index == (xDataSave.ui8Address) &&
//                ((xDataSave.i16Length) != 0)))
//        {
//            // ����������� ������ ��������� ��������� ��������� ��� �������-������(�� ������ pxData -> ui8Address)
//            *pxData = xDataSave;
//            // ������� �������-������ � ��� ��� ��� ��������� ���������.
//            pxData -> ui8Mode = CMind::TASK_MODE_SERVER;
//            // � �������-������ ������� � ������� ������ ���������(CSerialMT::TSerialMTData xData)
//            // ��������� �� ��������� ��������� � ������ ���������.
//            // ������� �������-������.
//            pxTaskContext[ui8Index].pfi16Task(pxModuleContext,
//                                              &pxTaskContext[ui8Index],
//                                              pxData);
//            // �������� ��������� ��������� ���������.
//            // ��������� � �������-������ � ������� ������ ���������(CSerialMT::TSerialMTData xData)
//            // ��������� �� ��������� ��������� � ������ ���������.
//            if (SerialMT.iGetDataMessageBag(pxData) == -1)
//            {
//                return -1;
//            }
//            // �������� ������ ��������� ��������� ��������� ��� �������-������(�� ������ pxData -> ui8Address)
//            xDataSave = *pxData;
//        }
//        else
//        {
//            // ������� � ���, ��� ��� ���������.
//            pxData -> i16Length = 0;
//            // ������� �������-������ � ��� ��� ��� ��������� ���������.
//            pxData -> ui8Mode = CMind::TASK_MODE_SERVER;
//            // � �������-������ ������� � ������� ������ ���������(CSerialMT::TSerialMTData xData)
//            // ��������� �� ��������� ��������� � ������ ���������.
//            // ������� �������-������.
//            pxTaskContext[ui8Index].pfi16Task(pxModuleContext,
//                                              &pxTaskContext[ui8Index],
//                                              pxData);
//        }
//    }
//    return 0;
//}
//
////-----------------------------------------------------------------------------------------------------
//// ������������ ������� � ������ � ��������-������� �� ������ �����.
//int16_t CMind::AnswerHandler(TModuleContext *pxModuleContext,
//                             CMind::TTaskContext *pxTaskContext,
//                             CSerialMT::TSerialMTData *pxData)
//{
//    CSerialMT SerialMT;
//    uint8_t ui8Index;
//    int16_t i;
//    int16_t i16Length;
//
//    // ��������� � ������� ������ ������ ���������(CSerialMT::TSerialMTData xData).
//    SerialMT.vPreparePutMessageBag(pxData);
//    // ��������� ������ ������ ������ � ������� ��������� �����(CMind::axAllTasksContext[]).
//    for (ui8Index = 0;
//            (ui8Index < CMind::i16TasksQuantity);
//            ui8Index++)
//    {
//        // �������-������ ����� ������ ������ ���������(CSerialMT::TSerialMTData xData)
//        // �������� ���� ������ �� ���������(xData.pui8DataPointer) � ����� ���������� ����(xData.i16Length).
//        // ������� �������-������.
//        // ������� �������-������ � ��� ��� ��� ��������� � ���� �������.
//        pxData -> ui8Mode = TASK_MODE_CLIENT;
//        if ((pxTaskContext[ui8Index].pfi16Task(pxModuleContext,
//                                               &pxTaskContext[ui8Index],
//                                               pxData)) != 0)
//        {
//            // ��������� ����� ������ ��� ��������� ������������ � �������(SerialMT.iPutInMessageBag(&xData).
//            pxData -> ui8Address = ui8Index;
//            // �������� ��������� ��������� � ������.
//            // ������ ��� ����� ��� ��������� ���������?
//            if (SerialMT.iPutInMessageBag(pxData) == -1)
//            {
//                // ������ ��� ����� ��� ��������� ���������.
//                // ������� ���� - ��������� ����������.
//                pxTaskContext[ui8Index].fui8MessageIsSent = 0;
//                // ������� � ������ �������.
//                break;
//            }
//        }
//    }
//    // ����� ��������� �� ����� ���������.
//    pxData -> pui8DataPointer = SerialMT.ucGetBuffMessageBag();
//    // ����� ����� ������ ���������.
//    pxData -> i16Length = SerialMT.iGetLengthMessageBag();
//    return 0;
//}

//-----------------------------------------------------------------------------------------------------
// �������� ������ �� �������� ������������ �� ����(���: W - �����, ����: E - ������)
// ������� ������������ � ����.
int16_t CMind::ConfigurationQueryEW(TModuleContext *pxModuleContext,
                                    CMind::TTaskContext *pxTaskContext,
                                    CSerialMT::TSerialMTData *pxData)
{
    int16_t i16Length;
    uint16_t ui16Index;

    switch (pxData -> ui8Mode)
    {
    case CMind::TASK_MODE_CLIENT:
        // �������-������ ������� ���������.
        // ��������� ������� � ����������?
        if ((pxTaskContext -> ui8Status) == CMind::TASK_MODE_MESSAGE_IS_HANDLED)
        {
            // ������� ���� - ��������� ����������.
            pxTaskContext -> ui8Status = 0;
            // ������� ��������� �� ����� ��� ���������� ��������� � ����� ���������.
            // �������� Modbus ����� ������� �� ��������� ���������.
            *(pxData -> pui8DataPointer) = ucModbusSlaveAddress;
            // �������� ������������ ������� �� ��������� ���������.
            vConfigurationDataPack((TConfigDataPackOne*)((pxData -> pui8DataPointer) + 1),
                                   (TConfigData*)pxModuleContext ->
                                   xModuleContextStatic.
                                   pucPlcConfigDataTempPointer);
            // �������� ����� ���������� ���������.
            // ������ ������� ������������ ���� ���� ���� - ����� �������.
            i16Length =
                (sizeof(struct TConfigDataPackOne) + 1);
            // ������� ������� �������������� ������� �������� ������ �������� �� ���������.
            pxTaskContext -> nui8TimeCounter = 0;
            // ��������� ���� - ��������� ����������.
            pxTaskContext -> fui8MessageIsSent = 1;
            // ����� ����� ���������.
            pxData -> i16Length = i16Length;
            // ����� ����� ���������.
            return i16Length;
        }
        break;

    case CMind::TASK_MODE_SERVER:
        // �������-������ ��������� ���������.
        // ��� �������-������ ���� ���������?
        if (pxData -> i16Length)
        {
            // ��������� �� ����������?
            if ((pxTaskContext -> ui8Status) != CMind::TASK_MODE_MESSAGE_IS_HANDLED)
            {
                // ��������� ���� - ��������� ����������.
                pxTaskContext -> ui8Status = CMind::TASK_MODE_MESSAGE_IS_HANDLED;
                // ����� ����� ������� ���������.
                pxData -> i16Length = 0;
                return 0;
            }
        }
        break;

    default:
        break;

    }
    // ����� ����� ������� ���������.
    pxData -> i16Length = 0;
    return 0;
}

//-----------------------------------------------------------------------------------------------------
//// ���������� ������� ����� � ����(���: W - �����, ����: E - ������)
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
//        // �������-������ � ���� �������(������� ���������).
//        struct tm *pxSource;
//        uint8_t *pui8Destination;
//        // ������� ��������� �� ����� � ������� �������� � ������� ������� �������.
//        pxSource =
//            (struct tm*)pxModuleContext ->
//            xModuleContextStatic.
//            pucCurrentTimeBufferPointer;
//        // ������� ��������� �� ����� ��� ���������� ��������� � ����� ���������.
//        pui8Destination = (uint8_t*)pxData -> pui8DataPointer;
//
//        // �������� ������� ����� �� ��������� ���������.
//        pui8Destination[CURRENT_TIME_SECOND_OFFSET] = pxSource -> tm_sec;
//        pui8Destination[CURRENT_TIME_MINUTE_OFFSET] = pxSource -> tm_min;
//        pui8Destination[CURRENT_TIME_HOUR_OFFSET] = pxSource -> tm_hour;
//        pui8Destination[CURRENT_TIME_MONTH_DAY_OFFSET] = pxSource -> tm_mday;
//        pui8Destination[CURRENT_TIME_MONTH_OFFSET] = pxSource -> tm_mon;
//        pui8Destination[CURRENT_TIME_YEAR_OFFSET] = pxSource -> tm_year;
//        pui8Destination[CURRENT_TIME_WEEK_DAY_OFFSET] = pxSource -> tm_wday;
//
//        // �������� ����� ���������� ���������.
//        i16Length = CURRENT_TIME_BYTE_QUANTITY;
//        // ����� ����� ���������.
//        pxData -> i16Length = i16Length;
//        return i16Length;
//        break;
//
//    case TASK_MODE_SERVER:
//        // �������-������ � ���� �������(��������� ���������).
//        // ����� ����� ������� ���������.
//        pxData -> i16Length = 0;
//        return 0;
//        break;
//
//    default:
//        break;
//    }
//
//    // ����� ����� ������� ���������.
//    pxData -> i16Length = 0;
//    return 0;
//}
// �������� ������ �� �������� �������� ������� �� ����(���: W - �����, ����: E - ������)
// ������� ������� ����� � ����.
int16_t CMind::CurrentTimeQueryEW(TModuleContext *pxModuleContext,
                                  CMind::TTaskContext *pxTaskContext,
                                  CSerialMT::TSerialMTData *pxData)
{
    int16_t i16Length;
    uint16_t ui16Index;

    switch (pxData -> ui8Mode)
    {
    case CMind::TASK_MODE_CLIENT:
        // �������-������ ������� ���������.
        // ��������� ������� � ����������?
        if ((pxTaskContext -> ui8Status) == CMind::TASK_MODE_MESSAGE_IS_HANDLED)
        {
            // ������� ���� - ��������� ����������.
            pxTaskContext -> ui8Status = 0;
            struct tm *pxSource;
            uint8_t *pui8Destination;
            // ������� ��������� �� ����� � ������� �������� � ������� ������� �������.
            pxSource =
                (struct tm*)pxModuleContext ->
                xModuleContextStatic.
                pucCurrentTimeBufferPointer;
            // ������� ��������� �� ����� ��� ���������� ��������� � ����� ���������.
            pui8Destination = (uint8_t*)pxData -> pui8DataPointer;

            // �������� ������� ����� �� ��������� ���������.
            pui8Destination[CURRENT_TIME_SECOND_OFFSET] = pxSource -> tm_sec;
            pui8Destination[CURRENT_TIME_MINUTE_OFFSET] = pxSource -> tm_min;
            pui8Destination[CURRENT_TIME_HOUR_OFFSET] = pxSource -> tm_hour;
            pui8Destination[CURRENT_TIME_MONTH_DAY_OFFSET] = pxSource -> tm_mday;
            pui8Destination[CURRENT_TIME_MONTH_OFFSET] = (pxSource -> tm_mon) + 1;
            pui8Destination[CURRENT_TIME_YEAR_OFFSET] = (pxSource -> tm_year) - 100;
            pui8Destination[CURRENT_TIME_WEEK_DAY_OFFSET] = pxSource -> tm_wday;

            // �������� ����� ���������� ���������.
            i16Length = CURRENT_TIME_BYTE_QUANTITY;
            // ����� ����� ���������.
            pxData -> i16Length = i16Length;
            return i16Length;
            break;
        }
        break;

    case CMind::TASK_MODE_SERVER:
        // �������-������ ��������� ���������.
        // ��� �������-������ ���� ���������?
        if (pxData -> i16Length)
        {
            // ��������� �� ����������?
            if ((pxTaskContext -> ui8Status) != CMind::TASK_MODE_MESSAGE_IS_HANDLED)
            {
                // ��������� ���� - ��������� ����������.
                pxTaskContext -> ui8Status = CMind::TASK_MODE_MESSAGE_IS_HANDLED;
                // ����� ����� ������� ���������.
                pxData -> i16Length = 0;
                return 0;
            }
        }
        break;

    default:
        break;

    }
    // ����� ����� ������� ���������.
    pxData -> i16Length = 0;
    return 0;
}

//-----------------------------------------------------------------------------------------------------
// ��������� ��������������� ������� ����� �� ����(���: W - �����, ����: E - ������)
int16_t CMind::CurrentTimeSetEW(TModuleContext *pxModuleContext,
                                CMind::TTaskContext *pxTaskContext,
                                CSerialMT::TSerialMTData *pxData)
{
    int16_t i16Length;

    switch (pxData -> ui8Mode)
    {
    case CMind::TASK_MODE_CLIENT:
        // �������-������ ������� ���������.
        // ��������� ������� � ����������?
        if ((pxTaskContext -> ui8Status) == CMind::TASK_MODE_MESSAGE_IS_HANDLED)
        {
            // �������-������ � ���� �������(������� ���������).
            // ������� ���� - ��������� ����������.
            pxTaskContext -> ui8Status = 0;
            // �������� ��������� �������������.
            *(pxData -> pui8DataPointer) = CMind::TASK_ANSWER_OK;
            // ����� ����� ��������� �������������.
            pxData -> i16Length = CMind::TASK_CONFIRMATION_LENGTH;
            // ����� ����� ���������.
            return pxData -> i16Length;
        }
        break;

    case CMind::TASK_MODE_SERVER:
        // �������-������ ��������� ���������.
        // ��� �������-������ ���� ���������?
        if (pxData -> i16Length)
        {
            // ��������� �� ����������?
            if ((pxTaskContext -> ui8Status) != CMind::TASK_MODE_MESSAGE_IS_HANDLED)
            {
                // ������� � ��������� ������� �����.
                vLinuxCurrentTimeSet((unsigned char*)(pxData -> pui8DataPointer));
                // ��������� ���� - ��������� ����������.
                pxTaskContext -> ui8Status = CMind::TASK_MODE_MESSAGE_IS_HANDLED;
                // ����� ����� ������� ���������.
                pxData -> i16Length = 0;
                return 0;
            }
        }
        break;

    default:
        break;

    }
    // ����� ����� ������� ���������.
    pxData -> i16Length = 0;
    return 0;
}

//-----------------------------------------------------------------------------------------------------
// ���������� ����� �������� ������ � ����(���: W - �����, ����: E - ������)
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
//        // �������-������ ������� ���������.
//        // ��������� �� ����������?
//        if (pxTaskContext -> fui8MessageIsSent == 0)
//        {
//            ui8HighLevelFlagsTemp = 0;
//            // ���� ������� - "������������" c �������� ������?
//            if (pxModuleContext ->
//                    xModuleContextDinamic.
//                    fbGlobalModbusKvit)
//            {
//                // ������� �������.
//                pxModuleContext ->
//                xModuleContextDinamic.
//                fbGlobalModbusKvit = 0;
//                // ��������� ���� - ������������ �� PC.
//                ui8HighLevelFlagsTemp |= (1 << CMind::HIGH_LEVEL_FLAGS_KVIT_BIT);
//            }
////            // ���� ������������ �� PC?
////            if (fbHighLevelKvit)
////            {
////                fbHighLevelKvit = 0;
////                // ��������� ���� - ������������ �� PC.
////                ui8HighLevelFlagsTemp |= (1 << CMind::HIGH_LEVEL_FLAGS_KVIT_BIT);
////            }
//
//            // ���� ������� - "�����" c �������� ������?
//            if (pxModuleContext ->
//                    xModuleContextDinamic.
//                    fbGlobalModbusReset)
//            {
//                // ������� �������.
//                pxModuleContext ->
//                xModuleContextDinamic.
//                fbGlobalModbusReset = 0;
//                // ��������� ���� - ����� �� PC.
//                ui8HighLevelFlagsTemp |= (1 << CMind::HIGH_LEVEL_FLAGS_RESET_BIT);
//            }
////            // ���� ����� �� PC?
////            if (fbHighLevelReset)
////            {
////                fbHighLevelReset = 0;
////                // ��������� ���� - ����� �� PC.
////                ui8HighLevelFlagsTemp |= (1 << CMind::HIGH_LEVEL_FLAGS_RESET_BIT);
////            }
//
//            // ���� ����������?
//            if (fbGlobalBlock)
//            {
//                // ��������� ���� - ���������� �� PC.
//                ui8HighLevelFlagsTemp |= (1 << CMind::HIGH_LEVEL_FLAGS_BLOCK_BIT);
//            }
//            else
//            {
//                // ������� ���� - ���������� �� PC.
//                ui8HighLevelFlagsTemp &= ~(1 << CMind::HIGH_LEVEL_FLAGS_BLOCK_BIT);
//            }
//
////            // ��������� ������ �� PC ����������?
////            if (ui8HighLevelFlagsTemp != CMind::ui8HighLevelFlags)
////            {
////            // �������� ����� ��������� ������ �� PC.
////            CMind::ui8HighLevelFlags = ui8HighLevelFlagsTemp;
//            // �������� ���������.
//            *(pxData -> pui8DataPointer) = ui8HighLevelFlagsTemp;
//            // ������� ������� �������������� ������� �������� ������ �������� �� ���������.
//            pxTaskContext -> nui8TimeCounter = 0;
//            // ��������� ���� - ��������� ����������.
//            pxTaskContext -> fui8MessageIsSent = 1;
//            // �������� ����� ���������� ���������.
//            i16Length = CMind::TASK_HIGH_LEVEL_FLAGS_MESSAGE_LENGTH;
//            // ����� ����� ���������.
//            pxData -> i16Length = i16Length;
//            // ����� ����� ���������.
//            return i16Length;
////            }
//        }
//        break;
//
//    case CMind::TASK_MODE_SERVER:
//        // �������-������ ��������� ���������.
//        // ��������� ����������?
//        if (pxTaskContext -> fui8MessageIsSent == 1)
//        {
//            // �������-������ � ���� �������(��������� ���������).
//            // ��� �������-������ ���� ���������?
//            if (pxData -> i16Length)
//            {
//                // �������� ������������� �����?
//                if (*(pxData -> pui8DataPointer) == CMind::TASK_ANSWER_OK)
//                {
//                    // �������� ������������� �����(��������� ���������� �������).
////                    // ������� ����� ���������� �� �� ������������ � �����.
////                    // ���� ���������� �������.
////                    CMind::ui8HighLevelFlags &=
////                        ~((1 << CMind::HIGH_LEVEL_FLAGS_KVIT_BIT) | (1 << CMind::HIGH_LEVEL_FLAGS_RESET_BIT));
//                    // ������� ���� - ��������� ����������.
//                    pxTaskContext -> fui8MessageIsSent = 0;
//                }
//                // �������� ������������� ����� � �������?
//                else if (*(pxData -> pui8DataPointer) == CMind::TASK_ANSWER_ERROR)
//                {
//                    // �������� ������������� ����� � �������.
////                    // ������� ����� ���������� �� �� ������������ � �����.
////                    // ���� ���������� �������.
////                    CMind::ui8HighLevelFlags &=
////                        ~((1 << CMind::HIGH_LEVEL_FLAGS_KVIT_BIT) | (1 << CMind::HIGH_LEVEL_FLAGS_RESET_BIT));
//                    // ������� ���� - ��������� ����������.
//                    pxTaskContext -> fui8MessageIsSent = 0;
//                }
//            }
//            else
//            {
//                // ��������� ����� �������� ������?
//                if ((pxTaskContext -> nui8TimeCounter) >= CMind::TASK_NO_ANSWER_TIMEOUT)
//                {
//                    // ��� ������ �� ���������.
////                    // ������� ����� ���������� �� �� ������������ � �����.
////                    // ���� ���������� �������.
////                    CMind::ui8HighLevelFlags &=
////                        ~((1 << CMind::HIGH_LEVEL_FLAGS_KVIT_BIT) | (1 << CMind::HIGH_LEVEL_FLAGS_RESET_BIT));
//                    // ������� ���� - ��������� ����������.
//                    pxTaskContext -> fui8MessageIsSent = 0;
//                }
//                else
//                {
//                    // �������� ������� �������������� ������� �������� ������ �������� �� ���������.
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
//    // ����� ����� ������� ���������.
//    pxData -> i16Length = 0;
//    return 0;










    int16_t i16Length;

    // ��������� ������ ���������� �� ���� � �� - F_HLEV �� ���������?
    if (!CMind::ui8HighLevelFlagsIsUpdated)
    {
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
            CMind::ui8HighLevelFlagsSetSave |= (1 << CMind::HIGH_LEVEL_FLAGS_KVIT_BIT);
            // ��������� ���� ��������� ������ ���������� �� ���� � �� - F_HLEV ���������.
            CMind::ui8HighLevelFlagsIsUpdated = 1;
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
            CMind::ui8HighLevelFlagsSetSave |= (1 << CMind::HIGH_LEVEL_FLAGS_RESET_BIT);
            // ��������� ���� ��������� ������ ���������� �� ���� � �� - F_HLEV ���������.
            CMind::ui8HighLevelFlagsIsUpdated = 1;
        }

        // ���� ����������?
        if (fbGlobalBlock)
        {
            // ��������� ������ ���������� ����������?
            if (!(CMind::ui8HighLevelFlagsSetSave &
                    (1 << CMind::HIGH_LEVEL_FLAGS_BLOCK_BIT)))
            {
                // ��������� ���� - ���������� �� PC.
                CMind::ui8HighLevelFlagsSetSave |= (1 << CMind::HIGH_LEVEL_FLAGS_BLOCK_BIT);
                // ��������� ���� ��������� ������ ���������� �� ���� � �� - F_HLEV ���������.
                CMind::ui8HighLevelFlagsIsUpdated = 1;
            }
        }
        else
        {
            if (CMind::ui8HighLevelFlagsSetSave &
                    (1 << CMind::HIGH_LEVEL_FLAGS_BLOCK_BIT))
            {
                // ������� ���� - ���������� �� PC.
                CMind::ui8HighLevelFlagsSetSave &= ~(1 << CMind::HIGH_LEVEL_FLAGS_BLOCK_BIT);
                // ��������� ���� ��������� ������ ���������� �� ���� � �� - F_HLEV ���������.
                CMind::ui8HighLevelFlagsIsUpdated = 1;
            }
        }
    }

    // ��������� ������ ���������� �� ���� � �� - F_HLEV ���������?
    if (CMind::ui8HighLevelFlagsIsUpdated)
    {
        switch (pxData -> ui8Mode)
        {
        case CMind::TASK_MODE_CLIENT:
            // �������-������ ������� ���������.
            // ��������� �� ����������?
            if (pxTaskContext -> fui8MessageIsSent == 0)
            {
                // �������� ���������.
                *(pxData -> pui8DataPointer) = CMind::ui8HighLevelFlagsSetSave;
                // ������� ������� �������������� ������� �������� ������ �������� �� ���������.
                pxTaskContext -> nui8TimeCounter = 0;
                // ��������� ���� - ��������� ����������.
                pxTaskContext -> fui8MessageIsSent = 1;
                // �������� ����� ���������� ���������.
                i16Length = CMind::TASK_HIGH_LEVEL_FLAGS_MESSAGE_LENGTH;
                // ����� ����� ���������.
                pxData -> i16Length = i16Length;
                // ����� ����� ���������.
                return i16Length;
            }
            break;

        case CMind::TASK_MODE_SERVER:
            // �������-������ ��������� ���������.
            // ��������� ����������?
            if (pxTaskContext -> fui8MessageIsSent == 1)
            {
                // �������-������ � ���� �������(��������� ���������).
                // ��� �������-������ ���� ���������?
                if (pxData -> i16Length)
                {
                    // �������� ������������� �����?
                    if (*(pxData -> pui8DataPointer) == CMind::TASK_ANSWER_OK)
                    {
                        // �������� ������������� �����(��������� ���������� �������).
                        // ������� ����� ���������� ������������ � �����.
                        // ���� ���������� �������.
                        CMind::ui8HighLevelFlagsSetSave &=
                            ~((1 << CMind::HIGH_LEVEL_FLAGS_KVIT_BIT) | (1 << CMind::HIGH_LEVEL_FLAGS_RESET_BIT));
                        // ������� ���� ��������� ������ ���������� �� ���� � �� - F_HLEV ���������.
                        // ������� ����� �������.
                        CMind::ui8HighLevelFlagsIsUpdated = 0;
                        // ������� ���� - ��������� ����������.
                        pxTaskContext -> fui8MessageIsSent = 0;
                    }
                    // �������� ������������� ����� � �������?
                    else if (*(pxData -> pui8DataPointer) == CMind::TASK_ANSWER_ERROR)
                    {
                        // �������� ������������� ����� � �������.
                        // ������� ���� - ��������� ����������.
                        pxTaskContext -> fui8MessageIsSent = 0;
                    }
                }
                else
                {
                    // ��������� ����� �������� ������?
                    if ((pxTaskContext -> nui8TimeCounter) >= CMind::TASK_NO_ANSWER_TIMEOUT)
                    {
                        // ��� ������ �� ���������.
                        // ������� ���� - ��������� ����������.
                        pxTaskContext -> fui8MessageIsSent = 0;
                    }
                    else
                    {
                        // �������� ������� �������������� ������� �������� ������ �������� �� ���������.
                        (pxTaskContext -> nui8TimeCounter)++;
                    }
                }
            }
            break;

        default:
            break;
        }
    }

    // ����� ����� ������� ���������.
    pxData -> i16Length = 0;
    return 0;
}

//-----------------------------------------------------------------------------------------------------
// ��������� ����� �������� ������ �� ����(���: W - �����, ����: E - ������)
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
        // �������-������ ������� ���������.
        // ��������� ������� � ����������?
        if ((pxTaskContext -> ui8Status) == CMind::TASK_MODE_MESSAGE_IS_HANDLED)
        {
            // �������-������ � ���� �������(������� ���������).
            // ������� ���� - ��������� ����������.
            pxTaskContext -> ui8Status = 0;
            // �������� ��������� �������������.
            *(pxData -> pui8DataPointer) = CMind::TASK_ANSWER_OK;
            // ����� ����� ��������� �������������.
            pxData -> i16Length = CMind::TASK_CONFIRMATION_LENGTH;
            // ����� ����� ���������.
            return pxData -> i16Length;
        }
        break;

    case CMind::TASK_MODE_SERVER:
        // �������-������ ��������� ���������.
        // ��� �������-������ ���� ���������?
        if (pxData -> i16Length)
        {
            // ��������� �� ����������?
            if ((pxTaskContext -> ui8Status) != CMind::TASK_MODE_MESSAGE_IS_HANDLED)
            {
                TModuleContext *pxModuleContextOrigin;
                // ������� ��������� �� ������ � ������� � ��������� ��������� ���� �������.
                pxModuleContextOrigin = (pxModuleContext - (pxModuleContext ->
                                         xModuleContextStatic.
                                         ucModuleContextIndex));
                // ������� ��������� ������ �� ����.
                ui8HighLevelFlagsTemp = *(pxData -> pui8DataPointer);
                // ���� ������������ �� ����?
                if (ui8HighLevelFlagsTemp & (1 << CMind::HIGH_LEVEL_FLAGS_KVIT_BIT))
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
                }
                // ���� ����� �� ����?
                if (ui8HighLevelFlagsTemp & (1 << CMind::HIGH_LEVEL_FLAGS_RESET_BIT))
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
                if (ui8HighLevelFlagsTemp & (1 << CMind::HIGH_LEVEL_FLAGS_BLOCK_BIT))
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

                // ��������� ���� - ��������� ����������.
                pxTaskContext -> ui8Status = CMind::TASK_MODE_MESSAGE_IS_HANDLED;
                // ����� ����� ������� ���������.
                pxData -> i16Length = 0;
                return 0;
            }
        }
        break;

    default:
        break;

    }
    // ����� ����� ������� ���������.
    pxData -> i16Length = 0;
    return 0;
}

//-----------------------------------------------------------------------------------------------------
// �������� ������� �� �������� ������� ������� �� ����(���: W - �����, ����: E - ������)
// ������� ������ ������� � ����.
int16_t CMind::EventLogQueryEW(TModuleContext *pxModuleContext,
                               CMind::TTaskContext *pxTaskContext,
                               CSerialMT::TSerialMTData *pxData)
{
    int16_t i16Length;
    uint16_t ui16Index;

    switch (pxData -> ui8Mode)
    {
    case CMind::TASK_MODE_CLIENT:
        // �������-������ ������� ���������.
        // ��������� ������� � ����������?
        if ((pxTaskContext -> ui8Status) == CMind::TASK_MODE_MESSAGE_IS_HANDLED)
        {
            CEvents::TEventDataCommon *pxSource;
            CEvents::TEventDataPackOne *pxDestination;

            // ������� ���� - ��������� ����������.
            pxTaskContext -> ui8Status = 0;
//            // �������� ��������� �������������.
//            *(pxData -> pui8DataPointer) = CMind::TASK_ANSWER_OK;
            // ������� ��������� �� ����� ���������� ��������� � ����� ���������.
            pxDestination = (CEvents::TEventDataPackOne*)(pxData -> pui8DataPointer);
            // ������� ���������� ��������� ������� � �������.
            i16Length = CEvents::xEventsLogQueryList.ui8Quantity;
            // ���������� ���������� ������� �����������
            // ������ �������?
            if (i16Length >
                    (CEvents::REQUESTED_EVENTS_QUANTITY))
            {
                // ������.
                break;
            }
//            CEvents::TEventDataCommon *pxSource;
//            CEvents::TEventDataPackOne *pxDestination;
            CEvents *pxCEvents;
            // ������ �������������� �������.
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

            // ������� ������ ������� �������������� �������.
            i16IndexQuery = CEvents::xEventsLogQueryList.i16Index - 2;
            // ��������� ������ � ������� ���������� �� ������� � ������� ����� �������.
            pxCEvents -> EventDataPopIndexSet(i16IndexQuery + CONVERT_INTEGER_TO_NATURAL_NUMBER);
            // �������� ������ ������� �� ��������� ���������.
            for (uint16_t i = 0;
                    i < i16Length;
                    i++)
            {
                // ������� ��������� �� ������ �������.
//                pxSource = xCAlarmEvent.EventDataPop();
                pxSource = pxCEvents -> EventDataPop();
                if (!pxSource)
                {
                    // ������.
                    // ������� ��� �������.
                    pxDestination[i].ui8Type = 0;
                    // ������ ������������� ������.
                    // ��� ������������� ��� ������ ������ ��� ������ � ����.
                    pxDestination[i].i16Index =
                        (i16IndexQuery);
                    printf("ArchiveEventLogQueryEW error pxDestination[i].i16Index %d\n\r", pxDestination[i].i16Index);
                    // ������� ������� �������� - ��� �������.
                    pxDestination[i].ui16ID = 0;
                }
                else
                {
                    // ���� ������ - ��� �������?
                    if ((pxSource -> ui16ID) == 0)
                    {
                        // ������� ��� �������.
                        pxDestination[i].ui8Type =
                            pxSource -> ui8Type;
                        // ������ ������������� ������.
                        // ��� ������������� ��� ������ ������ ��� ������ � ����.
                        pxDestination[i].i16Index =
                            (i16IndexQuery);
                        // ������� ������� �������� - ��� �������.
                        pxDestination[i].ui16ID = 0;
                        printf("ArchiveEventLogQueryEW no event pxDestination[i].i16Index %d\n\r", pxDestination[i].i16Index);
                    }
                    else
                    {
                        // ������� id �������.
                        pxDestination[i].ui16ID =
                            pxSource -> ui16ID;
                        // ������� ��� �������.
                        pxDestination[i].ui8Type =
                            pxSource -> ui8Type;
                        // ������ �������������� �������.
                        // ��� ������������� ��� ������ ������� ��� ������ � ����.
                        pxDestination[i].i16Index =
                            (i16IndexQuery);
                        printf("ArchiveEventLogQueryEW event pxDestination[i].i16Index %d\n\r", pxDestination[i].i16Index);
                        // ������� ����� �������.
                        pxDestination[i].ui16Address =
                            pxSource -> ui16Address;
                        // ������� ��������� �������.
                        pxDestination[i].ui8State =
                            pxSource -> ui8State;
                        // ����� �������� ������ �� ��� ������?
                        if ((pxSource -> xCurrentTime.tm_sec) != 0xFF)
                        {
                            // �������� ����� �������.
                            pxDestination[i].ui8Second = (uint8_t)pxSource -> xCurrentTime.tm_sec;
                            pxDestination[i].ui8Minute = (uint8_t)pxSource -> xCurrentTime.tm_min;
                            pxDestination[i].ui8Hour = (uint8_t)pxSource -> xCurrentTime.tm_hour;
                            pxDestination[i].ui8MonthDay = (uint8_t)pxSource -> xCurrentTime.tm_mday;
                            pxDestination[i].ui8Month = (uint8_t)(pxSource -> xCurrentTime.tm_mon) + 1;
                            pxDestination[i].ui8Year = (uint8_t)(pxSource -> xCurrentTime.tm_year) - 100;
                        }
                        else
                        {
                            // ��������� ��� ������� ������� - ��� ������.
                            pxDestination[i].ui8Second = 0xFF;
                        }
                        // ��������� ������ ���������� ���������.
                        memcpy(&(pxDestination[i].acTextDescriptor),
                               (&(pxSource -> acTextDescriptor)),
                               (CEvents::EVENT_NAME_LENGTH + END_OF_STRING_LENGTH));
                    }
                }
                // ������ ���������� �������������� �������.
                i16IndexQuery++;
            }




            // �������� ����� ���������� ���������.
            // ���� ����� ��������� �������������.
            i16Length =
                (CEvents::i16EventsLogQueryListQuantity *
                  sizeof(struct CEvents::TEventDataPackOne));
            // ����� ����� ���������.
            pxData -> i16Length = i16Length;
            // ����� ����� ���������.
            return i16Length;
        }
        break;

    case CMind::TASK_MODE_SERVER:
        // �������-������ ��������� ���������.
        // ��� �������-������ ���� ���������?
        if (pxData -> i16Length)
        {
            // ��������� �� ����������?
            if ((pxTaskContext -> ui8Status) != CMind::TASK_MODE_MESSAGE_IS_HANDLED)
            {



                CEvents::TEventsLogQueryListPackOne *pxSource;
                CEvents::TEventsLogQueryListPackOne *pxDestination;

                // ������� ��������� �� ����� ���������� ��������� � ����� ���������.
                pxSource = (CEvents::TEventsLogQueryListPackOne*)pxData -> pui8DataPointer;
                // ������� ��������� �� ������� ������ �� ������� �������� �������.
                pxDestination =
                    (CEvents::TEventsLogQueryListPackOne*)&CEvents::xEventsLogQueryList;
                // �������� ���������� ��������� ������� � �������.
                // �������� ���������� ��������� ������� � �������.
                CEvents::i16EventsLogQueryListQuantity = pxSource -> ui8Quantity;
                // �������� �������� ������ �������� ������� � ������� ������.
                *pxDestination = *pxSource;

                // ��������� ���� - ��������� ����������.
                pxTaskContext -> ui8Status = CMind::TASK_MODE_MESSAGE_IS_HANDLED;
                // ����� ����� ������� ���������.
                pxData -> i16Length = 0;
                return 0;
            }
        }
        break;

    default:
        break;

    }
    // ����� ����� ������� ���������.
    pxData -> i16Length = 0;
    return 0;
}

////-----------------------------------------------------------------------------------------------------
//// �������� ������� �� �������� ������� ������� �� ����(���: W - �����, ����: E - ������)
//// ������� ������ ������� � ����.
//int16_t CMind::AlarmEventLogQueryEW(TModuleContext *pxModuleContext,
//                                    CMind::TTaskContext *pxTaskContext,
//                                    CSerialMT::TSerialMTData *pxData)
//{
//    int16_t i16Length;
//    uint16_t ui16Index;
//    // ������ �������������� �������.
//    int16_t i16IndexQuery;
//
//    switch (pxData -> ui8Mode)
//    {
//    case CMind::TASK_MODE_CLIENT:
//        // �������-������ ������� ���������.
//        // ��������� ������� � ����������?
//        if ((pxTaskContext -> ui8Status) == CMind::TASK_MODE_MESSAGE_IS_HANDLED)
//        {
//            CEvents::TEventDataCommon *pxSource;
//            CEvents::TEventDataPackOne *pxDestination;
//
//            // ������� ���� - ��������� ����������.
//            pxTaskContext -> ui8Status = 0;
//            // �������� ��������� �������������.
//            *(pxData -> pui8DataPointer) = CMind::TASK_ANSWER_OK;
//            // ������� ��������� �� ����� ���������� ��������� � ����� ���������.
//            pxDestination = (CEvents::TEventDataPackOne*)((pxData -> pui8DataPointer) + 1);
//            // ������� ���������� ��������� ������� � �������.
//            i16Length = CEvents::xEventsLogQueryList.ui8Quantity;
//            // ���������� ���������� ������� �����������
//            // ������ �������?
//            if (i16Length >
//                    (CEvents::REQUESTED_EVENTS_QUANTITY))
//            {
//                // ������.
//                break;
//            }
//
//            // ������� ������ ������� �������������� �������.
//            i16IndexQuery = CEvents::xEventsLogQueryList.i16Index;
//            // ��������� ������ � ������� ���������� �� ������� � ������� ����� �������.
//            xCAlarmEvent.EventDataPopIndexSet(i16IndexQuery + CONVERT_INTEGER_TO_NATURAL_NUMBER);
//            // �������� ������ ������� �� ��������� ���������.
//            for (uint16_t i = 0;
//                    i < i16Length;
//                    i++)
//            {
//                // ������� ��������� �� ������ �������.
//                pxSource = xCAlarmEvent.EventDataPop();
//                if (!pxSource)
//                {
//                    // ������.
//                    // ������� ��� �������.
//                    pxDestination[i].ui8Type =
//                        pxSource -> ui8Type;
//                    // ������ ������������� ������.
//                    // ��� ������������� ��� ������ ������ ��� ������ � ����.
//                    pxDestination[i].i16Index =
//                        (i16IndexQuery);
//                    printf("ArchiveEventLogQueryEW error pxDestination[i].i16Index %d\n\r", pxDestination[i].i16Index);
//                    // ������� ������� �������� - ��� �������.
//                    pxDestination[i].ui16ID = 0;
//                }
//                else
//                {
//                    // ���� ������ - ��� �������?
//                    if ((pxSource -> ui16ID) == 0)
//                    {
//                        // ������� ��� �������.
//                        pxDestination[i].ui8Type =
//                            pxSource -> ui8Type;
//                        // ������ ������������� ������.
//                        // ��� ������������� ��� ������ ������ ��� ������ � ����.
//                        pxDestination[i].i16Index =
//                            (i16IndexQuery);
//                        // ������� ������� �������� - ��� �������.
//                        pxDestination[i].ui16ID = 0;
//                        printf("ArchiveEventLogQueryEW no event pxDestination[i].i16Index %d\n\r", pxDestination[i].i16Index);
//                    }
//                    else
//                    {
//                        // ������� id �������.
//                        pxDestination[i].ui16ID =
//                            pxSource -> ui16ID;
//                        if (i == 0)
//                        {
//                            CMind::i16LastID = pxSource -> ui16ID;
//                        }
//                        // ������� ��� �������.
//                        pxDestination[i].ui8Type =
//                            pxSource -> ui8Type;
//                        // ������ �������������� �������.
//                        // ��� ������������� ��� ������ ������� ��� ������ � ����.
//                        pxDestination[i].i16Index =
//                            (i16IndexQuery);
//                        printf("ArchiveEventLogQueryEW event pxDestination[i].i16Index %d\n\r", pxDestination[i].i16Index);
//                        // ������� ����� �������.
//                        pxDestination[i].ui16Address =
//                            pxSource -> ui16Address;
//                        // ������� ��������� �������.
//                        pxDestination[i].ui8State =
//                            pxSource -> ui8State;
//                        // ����� �������� ������ �� ��� ������?
//                        if ((pxSource -> xCurrentTime.tm_sec) != 0xFF)
//                        {
//                            // �������� ����� �������.
//                            pxDestination[i].ui8Second = (uint8_t)pxSource -> xCurrentTime.tm_sec;
//                            pxDestination[i].ui8Minute = (uint8_t)pxSource -> xCurrentTime.tm_min;
//                            pxDestination[i].ui8Hour = (uint8_t)pxSource -> xCurrentTime.tm_hour;
//                            pxDestination[i].ui8MonthDay = (uint8_t)pxSource -> xCurrentTime.tm_mday;
//                            pxDestination[i].ui8Month = (uint8_t)(pxSource -> xCurrentTime.tm_mon);
//                            pxDestination[i].ui8Year = (uint8_t)(pxSource -> xCurrentTime.tm_year);
//                        }
//                        else
//                        {
//                            // ��������� ��� ������� ������� - ��� ������.
//                            pxDestination[i].ui8Second = 0xFF;
//                        }
//                        // ��������� ������ ���������� ���������.
//                        memcpy(&(pxDestination[i].acTextDescriptor),
//                               (&(pxSource -> acTextDescriptor)),
//                               (CEvents::EVENT_NAME_LENGTH + END_OF_STRING_LENGTH));
//                    }
//                }
//                // ������ ���������� �������������� �������.
//                i16IndexQuery++;
//            }
//
//            // �������� ����� ���������� ���������.
//            // ���� ����� ��������� �������������.
//            i16Length =
//                ((CEvents::i16EventsLogQueryListQuantity *
//                  sizeof(struct CEvents::TEventDataPackOne)) +
//                 CMind::TASK_CONFIRMATION_LENGTH);
//            // ����� ����� ���������.
//            pxData -> i16Length = i16Length;
//            // ����� ����� ���������.
//            return i16Length;
//
//
//
//
////            CEvents::TEventDataCommon *pxSource;
////            CAlarm::TAlarmEventPackOne *pxDestination;
////
////            // ������� ���� - ��������� ����������.
////            pxTaskContext -> ui8Status = 0;
////
////            // ������� ��������� �� ����� ���������� ��������� � ����� ���������.
////            pxDestination = (CAlarm::TAlarmEventPackOne*)(pxData -> pui8DataPointer);
////            // ������� ���������� ��������� ������� � �������.
////            i16Length = CAlarm::i16AlarmEventQueryListQuantity;
////            // ���������� ���������� ������� �����������
////            // ������ ������� axAlarmEventsLog?
////            if (i16Length >
////                    (xCAlarmEvent.ui16RecordedEventsQuantity))
////            {
////                // ������.
////                break;
////            }
////
////            // ������� ������ ������� �������������� �������.
////            i16IndexQuery = CAlarm::xAlarmEventsQueryList.ui16Index;
////            // ��������� ������ � ������� ���������� �� ������� � ������� ����� �������.
////            xCAlarmEvent.EventDataPopIndexSet(i16IndexQuery + CONVERT_INTEGER_TO_NATURAL_NUMBER);
////            // �������� ������ ������� �� ��������� ���������.
////            for (uint16_t i = 0;
////                    i < i16Length;
////                    i++)
////            {
////                // ������� ��������� �� ������ �������.
////                pxSource = xCAlarmEvent.EventDataPop();
////                if (!pxSource)
////                {
////                    // ������.
////                    // ������ ������������� ������.
////                    // ��� ������������� ��� ������ ������ ��� ������ � ����.
////                    pxDestination[i].ui16Index =
////                        i16IndexQuery;
////                    // ������� ������� �������� - ��� �������.
////                    pxDestination[i].ui16ID = 0;
////                }
////                else
////                {
////                    // ���� ������ - ��� �������?
////                    if ((pxSource -> ui16ID) == 0)
////                    {
////                        // ������ ������������� ������.
////                        // ��� ������������� ��� ������ ������ ��� ������ � ����.
////                        pxDestination[i].ui16Index =
////                            i16IndexQuery;
////                        // ������� ������� �������� - ��� �������.
////                        pxDestination[i].ui16ID = 0;
////                    }
////                    else
////                    {
////                        // ��������� id �������.
////                        pxDestination[i].ui16ID =
////                            1;
////                        // ������ �������������� �������.
////                        // ��� ������������� ��� ������ ������� ��� ������ � ����.
////                        pxDestination[i].ui16Index =
////                            i16IndexQuery;
////                        // ������� ����� �������.
////                        pxDestination[i].ui16Address =
////                            pxSource -> ui16Address;
////                        // ������� ��������� �������.
////                        pxDestination[i].ui8State =
////                            pxSource -> ui8State;
////                        // ��������� ������ ���������� ���������.
////                        memcpy(&(pxDestination[i].acTextDescriptor),
////                               (&(pxSource -> acTextDescriptor)),
////                               (CAlarm::EVENT_NAME_LENGTH + END_OF_STRING_LENGTH));
////                        // �������� ����� �������.
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
////                // ������ ���������� �������������� �������.
////                i16IndexQuery++;
////            }
////
////            // �������� ����� ���������� ���������.
////            i16Length =
////                (CAlarm::i16AlarmEventQueryListQuantity *
////                 sizeof(struct CAlarm::TAlarmEventPackOne));
////            // ����� ����� ���������.
////            pxData -> i16Length = i16Length;
////            // ����� ����� ���������.
////            return i16Length;
//        }
//        break;
//
//    case CMind::TASK_MODE_SERVER:
//        // �������-������ ��������� ���������.
//        // ��� �������-������ ���� ���������?
//        if (pxData -> i16Length)
//        {
//            // ��������� �� ����������?
//            if ((pxTaskContext -> ui8Status) != CMind::TASK_MODE_MESSAGE_IS_HANDLED)
//            {
//                CEvents::TEventsLogQueryListPackOne *pxSource;
//                CEvents::TEventsLogQueryListPackOne *pxDestination;
//
//                // ������� ��������� �� ����� ���������� ��������� � ����� ���������.
//                pxSource = (CEvents::TEventsLogQueryListPackOne*)pxData -> pui8DataPointer;
//                // ������� ��������� �� ������� ������ �� ������� �������� �������.
//                pxDestination =
//                    (CEvents::TEventsLogQueryListPackOne*)&CEvents::xEventsLogQueryList;
//                // �������� ���������� ��������� ������� � �������.
//                // �������� ���������� ��������� ������� � �������.
//                CEvents::i16EventsLogQueryListQuantity = pxSource -> ui8Quantity;
//                // �������� �������� ������ �������� ������� � ������� ������.
//                *pxDestination = *pxSource;
//                // ��������� ���� - ��������� ����������.
//                pxTaskContext -> ui8Status = CMind::TASK_MODE_MESSAGE_IS_HANDLED;
//                // ����� ����� ������� ���������.
//                pxData -> i16Length = 0;
//                return 0;
//            }
//
//
////            // ��������� �� ����������?
////            if ((pxTaskContext -> ui8Status) != CMind::TASK_MODE_MESSAGE_IS_HANDLED)
////            {
////                CAlarm::TAlarmEventLogQueryListPackOne *pxSource;
////                CAlarm::TAlarmEventLogQueryListPackOne *pxDestination;
////
////                // ������� ��������� �� ����� ���������� ��������� � ����� ���������.
////                pxSource = (CAlarm::TAlarmEventLogQueryListPackOne*)pxData -> pui8DataPointer;
////                // ������� ��������� �� ������� ������ �� ������� �������� �������.
////                pxDestination =
////                    (CAlarm::TAlarmEventLogQueryListPackOne*)&CAlarm::xAlarmEventsQueryList;
////                // �������� ���������� ��������� ������� � �������.
////                // �������� ���������� ��������� ������� � �������.
////                CAlarm::i16AlarmEventQueryListQuantity = pxSource -> ui8Quantity;
////                // �������� �������� ������ �������� ������� � ������� ������.
////                *pxDestination = *pxSource;
////                // ��������� ���� - ��������� ����������.
////                pxTaskContext -> ui8Status = CMind::TASK_MODE_MESSAGE_IS_HANDLED;
////                // ����� ����� ������� ���������.
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
//// ����� ����� ������� ���������.
//    pxData -> i16Length = 0;
//    return 0;
//}
//
////-----------------------------------------------------------------------------------------------------
//// �������� ������� �� �������� ������� ������� �� ����(���: W - �����, ����: E - ������)
//// ������� ������ ������� � ����.
//int16_t CMind::ArchiveEventLogQueryEW(TModuleContext *pxModuleContext,
//                                      CMind::TTaskContext *pxTaskContext,
//                                      CSerialMT::TSerialMTData *pxData)
//{
//    int16_t i16Length;
//    uint16_t ui16Index;
//    // ������ �������������� �������.
//    int16_t i16IndexQuery;
//
//    switch (pxData -> ui8Mode)
//    {
//    case CMind::TASK_MODE_CLIENT:
//        // �������-������ ������� ���������.
//        // ��������� ������� � ����������?
//        if ((pxTaskContext -> ui8Status) == CMind::TASK_MODE_MESSAGE_IS_HANDLED)
//        {
//            CEvents::TEventDataCommon *pxSource;
//            CEvents::TEventDataPackOne *pxDestination;
//
//            // ������� ���� - ��������� ����������.
//            pxTaskContext -> ui8Status = 0;
//            // �������� ��������� �������������.
//            *(pxData -> pui8DataPointer) = CMind::TASK_ANSWER_OK;
//            // ������� ��������� �� ����� ���������� ��������� � ����� ���������.
//            pxDestination = (CEvents::TEventDataPackOne*)((pxData -> pui8DataPointer) + 1);
//            // ������� ���������� ��������� ������� � �������.
//            i16Length = CEvents::xEventsLogQueryList.ui8Quantity;
//            // ���������� ���������� ������� �����������
//            // ������ �������?
//            if (i16Length >
//                    (CEvents::REQUESTED_EVENTS_QUANTITY))
//            {
//                // ������.
//                break;
//            }
//
//            // ������� ������ ������� �������������� �������.
//            i16IndexQuery = CEvents::xEventsLogQueryList.i16Index;
//            printf("ArchiveEventLogQueryEW i16IndexQuery %d\n\r", (i16IndexQuery));
//            // � ���� ���������� ��� ������, ���������� �� ���� ��� ��������� �������?
//            // ���������� �������, ���������� �� ���� - �������� �� ������� ��������
//            // ����, �������� - ���������� ������������� ���� �������(CEvents::REQUESTED_EVENTS_QUANTITY).
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
//                // ������� ����� �������, ����� ����� ����������� �������� ����,
//                // ��� ������� �����������.
//                xCPlcArchiveEvent.NoEventsFlagSet();
//                // �������� ������ �� ���� ������ � �����.
//                xCArchiveEventsDB.DataBaseDataGet();
//                if (i16IndexQuery == 0)
//                {
//                    // ������� ���������� �������, ���������� �� ����.
//                    i16AllReceivedRecordsQuantity = 0;
//                    i16LastAllReceivedRecordsQuantity = xCArchiveEventsDB.LastQueryRecordsQuantityGet();
//                }
//                else
//                {
//                    // ������� ���������� �������, ���������� �� ����.
//                    i16LastAllReceivedRecordsQuantity = xCArchiveEventsDB.LastQueryRecordsQuantityGet();
//                }
////                // ������� ���������� �������, ���������� �� ����.
////                i16LastAllReceivedRecordsQuantity = xCArchiveEventsDB.LastQueryRecordsQuantityGet();
//                i16AllReceivedRecordsQuantity += i16LastAllReceivedRecordsQuantity;
//                printf("ArchiveEventLogQueryEW i16AllReceivedRecordsQuantity %d\n\r", (i16AllReceivedRecordsQuantity));
//                printf("ArchiveEventLogQueryEW i16LastAllReceivedRecordsQuantity %d\n\r", (i16LastAllReceivedRecordsQuantity));
//                // �������� ������ ��� ������� ��������� ������ �� ����.
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
////            // ������ ������� � ������� �������?
////            if ((i16IndexQuery - CMind::ui16LastIndexQuery) <= 0)
////            {
////
////            }
//
////            // ��������� ������ � ������� ���������� �� ������� � ������� ����� �������.
////            xCPlcArchiveEvent.EventDataPopIndexSet(CEvents::xEventsLogQueryList.ui8Quantity);
//
//            // ����� ��������� ������� �� ���� � ���������� ���������
//            // � ���������� ������ � �������, �������� ����� - (MAIN_CYCLE_TIME).
//            // ��� �������� ���� ����� ��������� ���������� - ������� ������������
//            // ������� �������� ������ �� ������� ����������� �������.
//            // ����� ����� ��������, �� ���� ��� ������������� 4 �������.
//            // ����������, ��� ��������� � ���������.
//            // � ���������� ������ �� ������� ��������� ��������� �������.
//            // ���� ������ ������ ����� - �� ������� ��������� ��������� �������,
//            // ���� ������ ������ ����� - �� ������� ��������� ���������� �������,
//            // �� ����������� ������ �� ������.
//            // ((i16AllReceivedRecordsQuantity -
//            // (i16IndexQuery -
//            // CMind::ui16LastIndexQuery)) + 1) - ������ �� ���������� �������.
//            // ��������� ������ � ������� ���������� �� ������� � ������� ����� �������.
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
//                // ������� ��������� �� ������ ������� ����������� �� ���� ������.
//                pxSource = xCPlcArchiveEvent.EventDataPop();
//                // ������ ���������� �������?
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
//            // �������� ������ ������� �� ��������� ���������.
//            for (uint16_t i = 0;
//                    i < i16Length;
//                    i++)
//            {
////                // ������� ��������� �� ������ ������� ����������� �� ���� ������.
////                pxSource = xCPlcArchiveEvent.EventDataPop();
//                if (!pxSource)
//                {
//                    // ������.
//                    // ������� ��� �������.
//                    pxDestination[i].ui8Type =
//                        pxSource -> ui8Type;
//                    // ������ ������������� ������.
//                    // ��� ������������� ��� ������ ������ ��� ������ � ����.
//                    pxDestination[i].i16Index =
//                        (i16IndexQuery - CEventsDB::EVENTS_DB_PREVIOUS_RECORDS_QUANTITY);//(pxSource -> i16Index);//
//                    printf("ArchiveEventLogQueryEW error pxDestination[i].i16Index %d\n\r", pxDestination[i].i16Index);
//                    // ������� ������� �������� - ��� �������.
//                    pxDestination[i].ui16ID = 0;
//                }
//                else
//                {
//                    // ���� ������ - ��� �������?
//                    if ((pxSource -> ui16ID) == 0)
//                    {
//                        // ������� ��� �������.
//                        pxDestination[i].ui8Type =
//                            pxSource -> ui8Type;
//                        // ������ ������������� ������.
//                        // ��� ������������� ��� ������ ������ ��� ������ � ����.
////                        pxDestination[i].ui16Index =
////                            (i16IndexQuery - 1);
//                        pxDestination[i].i16Index =
//                            (i16IndexQuery - CEventsDB::EVENTS_DB_PREVIOUS_RECORDS_QUANTITY);//(pxSource -> i16Index);//
//                        // ������� ������� �������� - ��� �������.
//                        pxDestination[i].ui16ID = 0;
//                        printf("ArchiveEventLogQueryEW no event pxDestination[i].i16Index %d\n\r", pxDestination[i].i16Index);
//                    }
//                    else
//                    {
//                        // ������� id �������.
//                        pxDestination[i].ui16ID =
//                            pxSource -> ui16ID;
//                        // ������� ��� �������.
//                        pxDestination[i].ui8Type =
//                            pxSource -> ui8Type;
//                        // ������ �������������� �������.
//                        // ��� ������������� ��� ������ ������� ��� ������ � ����.
//                        pxDestination[i].i16Index =
//                            (i16IndexQuery - CEventsDB::EVENTS_DB_PREVIOUS_RECORDS_QUANTITY);//(pxSource -> i16Index);//
//                        printf("ArchiveEventLogQueryEW event pxDestination[i].i16Index %d\n\r", pxDestination[i].i16Index);
//                        // ������� ����� �������.
//                        pxDestination[i].ui16Address =
//                            pxSource -> ui16Address;
//                        // ������� ��������� �������.
//                        pxDestination[i].ui8State =
//                            pxSource -> ui8State;
//                        // ����� �������� ������ �� ��� ������?
//                        if ((pxSource -> xCurrentTime.tm_sec) != 0xFF)
//                        {
//                            // �������� ����� �������.
//                            pxDestination[i].ui8Second = (uint8_t)pxSource -> xCurrentTime.tm_sec;
//                            pxDestination[i].ui8Minute = (uint8_t)pxSource -> xCurrentTime.tm_min;
//                            pxDestination[i].ui8Hour = (uint8_t)pxSource -> xCurrentTime.tm_hour;
//                            pxDestination[i].ui8MonthDay = (uint8_t)pxSource -> xCurrentTime.tm_mday;
//                            pxDestination[i].ui8Month = (uint8_t)(pxSource -> xCurrentTime.tm_mon);
//                            pxDestination[i].ui8Year = (uint8_t)(pxSource -> xCurrentTime.tm_year);
//                        }
//                        else
//                        {
//                            // ��������� ��� ������� ������� - ��� ������.
//                            pxDestination[i].ui8Second = 0xFF;
//                        }
//                        // ��������� ������ ���������� ���������.
//                        memcpy(&(pxDestination[i].acTextDescriptor),
//                               (&(pxSource -> acTextDescriptor)),
//                               (CEvents::EVENT_NAME_LENGTH + END_OF_STRING_LENGTH));
//                    }
//                }
//                // ������ ���������� �������������� �������.
//                i16IndexQuery++;
//                pxSource++;
//            }
//
//            // �������� ����� ���������� ���������.
//            // ���� ����� ��������� �������������.
//            i16Length =
//                ((CEvents::i16EventsLogQueryListQuantity *
//                  sizeof(struct CEvents::TEventDataPackOne)) +
//                 CMind::TASK_CONFIRMATION_LENGTH);
//            // ����� ����� ���������.
//            pxData -> i16Length = i16Length;
//            // ����� ����� ���������.
//            return i16Length;
//        }
//        break;
//
//    case CMind::TASK_MODE_SERVER:
//        // �������-������ ��������� ���������.
//        // ��� �������-������ ���� ���������?
//        if (pxData -> i16Length)
//        {
//            // ��������� �� ����������?
//            if ((pxTaskContext -> ui8Status) != CMind::TASK_MODE_MESSAGE_IS_HANDLED)
//            {
//                CEvents::TEventsLogQueryListPackOne *pxSource;
//                CEvents::TEventsLogQueryListPackOne *pxDestination;
//
//                // ������� ��������� �� ����� ���������� ��������� � ����� ���������.
//                pxSource = (CEvents::TEventsLogQueryListPackOne*)pxData -> pui8DataPointer;
//                // ������� ��������� �� ������� ������ �� ������� �������� �������.
//                pxDestination =
//                    (CEvents::TEventsLogQueryListPackOne*)&CEvents::xEventsLogQueryList;
//                // �������� ���������� ��������� ������� � �������.
//                // �������� ���������� ��������� ������� � �������.
//                CEvents::i16EventsLogQueryListQuantity = pxSource -> ui8Quantity;
//                // �������� �������� ������ �������� ������� � ������� ������.
//                *pxDestination = *pxSource;
//                // ��������� ���� - ��������� ����������.
//                pxTaskContext -> ui8Status = CMind::TASK_MODE_MESSAGE_IS_HANDLED;
//                // ����� ����� ������� ���������.
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
//    // ����� ����� ������� ���������.
//    pxData -> i16Length = 0;
//    return 0;
//}
//
////-----------------------------------------------------------------------------------------------------
//// �������� ������� �� �������� ������� ������� �� ����(���: W - �����, ����: E - ������)
//// ������� ������ ������� � ����.
//int16_t CMind::ArchiveEventLogQueryEWLite(TModuleContext *pxModuleContext,
//        CMind::TTaskContext *pxTaskContext,
//        CSerialMT::TSerialMTData *pxData)
//{
//    int16_t i16Length;
//    uint16_t ui16Index;
//    // ������ �������������� �������.
//    int16_t i16IndexQuery;
//
//    switch (pxData -> ui8Mode)
//    {
//    case CMind::TASK_MODE_CLIENT:
//        // �������-������ ������� ���������.
//        // ��������� ������� � ����������?
//        if ((pxTaskContext -> ui8Status) == CMind::TASK_MODE_MESSAGE_IS_HANDLED)
//        {
//            CEvents::TEventDataCommon *pxSource;
//            CEvents::TEventDataPackOne *pxDestination;
//
//            // ������� ���� - ��������� ����������.
//            pxTaskContext -> ui8Status = 0;
//            // �������� ��������� �������������.
//            *(pxData -> pui8DataPointer) = CMind::TASK_ANSWER_OK;
//            // ������� ��������� �� ����� ���������� ��������� � ����� ���������.
//            pxDestination = (CEvents::TEventDataPackOne*)((pxData -> pui8DataPointer) + 1);
//            // ������� ���������� ��������� ������� � �������.
//            i16Length = CEvents::xEventsLogQueryList.ui8Quantity;
//            // ���������� ���������� ������� �����������
//            // ������ �������?
//            if (i16Length >
//                    (CEvents::REQUESTED_EVENTS_QUANTITY))
//            {
//                // ������.
//                break;
//            }
//
//            // ������� ������ ������� �������������� �������.
//            i16IndexQuery = CEvents::xEventsLogQueryList.i16Index - 2;
//            printf("ArchiveEventLogQueryEW i16IndexQuery %d\n\r", (i16IndexQuery));
//
////            if (CEvents::xEventsLogQueryList.i16Index = 0)
////            {
////                //                // ������� ����� �������, ����� ����� ����������� �������� ����,
//////                // ��� ������� �����������.
//////                xCPlcArchiveEvent.NoEventsFlagSet();
////                CMind::i16LastID = 0;
////            }
//
//            // ������� ����� �������, ����� ����� ����������� �������� ����,
//            // ��� ������� �����������.
//            xCPlcArchiveEvent.NoEventsFlagSet();
//            // �������� ������ �� ���� ������ � �����.
////            xCArchiveEventsDB.DataBaseDataGet();
//            xCArchiveEventsDB.DataBaseDataGet();
//
//
//            xCPlcArchiveEvent.EventDataPopIndexSet(xCArchiveEventsDB.LastQueryRecordsQuantityGet());
//
//            // �������� ������ ������� �� ��������� ���������.
//            for (uint16_t i = 0;
//                    i < i16Length;
//                    i++)
//            {
//                // ������� ��������� �� ������ ������� ����������� �� ���� ������.
//                pxSource = xCPlcArchiveEvent.EventDataPop();
//                if (!pxSource)
//                {
//                    // ������.
//                    // ������� ��� �������.
//                    pxDestination[i].ui8Type =
//                        pxSource -> ui8Type;
//                    // ������ ������������� ������.
//                    // ��� ������������� ��� ������ ������ ��� ������ � ����.
//                    pxDestination[i].i16Index =
//                        (i16IndexQuery);
//                    printf("ArchiveEventLogQueryEW error pxDestination[i].i16Index %d\n\r", pxDestination[i].i16Index);
//                    // ������� ������� �������� - ��� �������.
//                    pxDestination[i].ui16ID = 0;
//                }
//                else
//                {
//                    // ���� ������ - ��� �������?
//                    if ((pxSource -> ui16ID) == 0)
//                    {
//                        // ������� ��� �������.
//                        pxDestination[i].ui8Type =
//                            pxSource -> ui8Type;
//                        // ������ ������������� ������.
//                        // ��� ������������� ��� ������ ������ ��� ������ � ����.
//                        pxDestination[i].i16Index =
//                            (i16IndexQuery);
//                        // ������� ������� �������� - ��� �������.
//                        pxDestination[i].ui16ID = 0;
//                        printf("ArchiveEventLogQueryEW no event pxDestination[i].i16Index %d\n\r", pxDestination[i].i16Index);
//                    }
//                    else
//                    {
//                        // ������� id �������.
//                        pxDestination[i].ui16ID =
//                            pxSource -> ui16ID;
//                        if (i == 0)
//                        {
//                            CMind::i16LastID = pxSource -> ui16ID;
//                        }
//                        // ������� ��� �������.
//                        pxDestination[i].ui8Type =
//                            pxSource -> ui8Type;
//                        // ������ �������������� �������.
//                        // ��� ������������� ��� ������ ������� ��� ������ � ����.
//                        pxDestination[i].i16Index =
//                            (i16IndexQuery);
//                        printf("ArchiveEventLogQueryEW event pxDestination[i].i16Index %d\n\r", pxDestination[i].i16Index);
//                        // ������� ����� �������.
//                        pxDestination[i].ui16Address =
//                            pxSource -> ui16Address;
//                        // ������� ��������� �������.
//                        pxDestination[i].ui8State =
//                            pxSource -> ui8State;
//                        // ����� �������� ������ �� ��� ������?
//                        if ((pxSource -> xCurrentTime.tm_sec) != 0xFF)
//                        {
//                            // �������� ����� �������.
//                            pxDestination[i].ui8Second = (uint8_t)pxSource -> xCurrentTime.tm_sec;
//                            pxDestination[i].ui8Minute = (uint8_t)pxSource -> xCurrentTime.tm_min;
//                            pxDestination[i].ui8Hour = (uint8_t)pxSource -> xCurrentTime.tm_hour;
//                            pxDestination[i].ui8MonthDay = (uint8_t)pxSource -> xCurrentTime.tm_mday;
//                            pxDestination[i].ui8Month = (uint8_t)(pxSource -> xCurrentTime.tm_mon);
//                            pxDestination[i].ui8Year = (uint8_t)(pxSource -> xCurrentTime.tm_year);
//                        }
//                        else
//                        {
//                            // ��������� ��� ������� ������� - ��� ������.
//                            pxDestination[i].ui8Second = 0xFF;
//                        }
//                        // ��������� ������ ���������� ���������.
//                        memcpy(&(pxDestination[i].acTextDescriptor),
//                               (&(pxSource -> acTextDescriptor)),
//                               (CEvents::EVENT_NAME_LENGTH + END_OF_STRING_LENGTH));
//                    }
//                }
//                // ������ ���������� �������������� �������.
//                i16IndexQuery++;
//            }
//
//            // �������� ����� ���������� ���������.
//            // ���� ����� ��������� �������������.
//            i16Length =
//                ((CEvents::i16EventsLogQueryListQuantity *
//                  sizeof(struct CEvents::TEventDataPackOne)) +
//                 CMind::TASK_CONFIRMATION_LENGTH);
//            // ����� ����� ���������.
//            pxData -> i16Length = i16Length;
//            // ����� ����� ���������.
//            return i16Length;
//        }
//        break;
//
//    case CMind::TASK_MODE_SERVER:
//        // �������-������ ��������� ���������.
//        // ��� �������-������ ���� ���������?
//        if (pxData -> i16Length)
//        {
//            // ��������� �� ����������?
//            if ((pxTaskContext -> ui8Status) != CMind::TASK_MODE_MESSAGE_IS_HANDLED)
//            {
//                CEvents::TEventsLogQueryListPackOne *pxSource;
//                CEvents::TEventsLogQueryListPackOne *pxDestination;
//
//                // ������� ��������� �� ����� ���������� ��������� � ����� ���������.
//                pxSource = (CEvents::TEventsLogQueryListPackOne*)pxData -> pui8DataPointer;
//                // ������� ��������� �� ������� ������ �� ������� �������� �������.
//                pxDestination =
//                    (CEvents::TEventsLogQueryListPackOne*)&CEvents::xEventsLogQueryList;
//                // �������� ���������� ��������� ������� � �������.
//                // �������� ���������� ��������� ������� � �������.
//                CEvents::i16EventsLogQueryListQuantity = pxSource -> ui8Quantity;
//                // �������� �������� ������ �������� ������� � ������� ������.
//                *pxDestination = *pxSource;
//                // ��������� ���� - ��������� ����������.
//                pxTaskContext -> ui8Status = CMind::TASK_MODE_MESSAGE_IS_HANDLED;
//                // ����� ����� ������� ���������.
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
//// ����� ����� ������� ���������.
//    pxData -> i16Length = 0;
//    return 0;
//}

//-----------------------------------------------------------------------------------------------------
// �������� ������� �� �������� ������� ������ �� ����(���: W - �����, ����: E - ������)
// ������� ������ ������ � ����.
int16_t CMind::ErrorLogQueryEW(TModuleContext *pxModuleContext,
                               CMind::TTaskContext *pxTaskContext,
                               CSerialMT::TSerialMTData *pxData)
{
    int16_t i16Length;
    uint16_t ui16Index;
    // ������ ������������� ������.
    uint16_t i16IndexQuery;

    switch (pxData -> ui8Mode)
    {
    case CMind::TASK_MODE_CLIENT:
        // �������-������ ������� ���������.
        // ��������� ������� � ����������?
        if ((pxTaskContext -> ui8Status) == CMind::TASK_MODE_MESSAGE_IS_HANDLED)
        {
            CEvents::TEventDataCommon *pxSource;
            CError::TErrorPackOne *pxDestination;

            // ������� ���� - ��������� ����������.
            pxTaskContext -> ui8Status = 0;

            // ������� ��������� �� ����� ���������� ��������� � ����� ���������.
            pxDestination = (CError::TErrorPackOne*)(pxData -> pui8DataPointer);
            // ������� ���������� ��������� ������ � �������.
            i16Length = CError::i16ErrorQueryListQuantity;
            // ���������� ���������� ������ �����������
            // ������ ������� axErrorLog?
            if (i16Length >
                    (xCPlcErrorEvent.ui16RecordedEventsQuantity))
            {
                // ������.
                break;
            }

            // ������� ������ ������ ������������� ������.
            i16IndexQuery = CError::xErrorQueryList.ui16Index;
            // ��������� ������ � ������� ���������� �� ������� � ������� ����� �������.
            xCPlcErrorEvent.EventDataPointerPopIndexSet(i16IndexQuery + CONVERT_INTEGER_TO_NATURAL_NUMBER);

            // �������� ������ ������ �� ��������� ���������.
            for (uint16_t i = 0;
                    i < i16Length;
                    i++)
            {
                // ������� ��������� �� ������ �������.
                pxSource = xCPlcErrorEvent.EventDataPointerPop();
                if (!pxSource)
                {
                    // ������.
                    // ������ ������������� ������.
                    // ��� ������������� ��� ������ ������ ��� ������ � ����.
                    pxDestination[i].ui16Index =
                        i16IndexQuery;
                    // ������� ������� �������� - ��� �������.
                    pxDestination[i].ui16ID = 0;
                }
                else
                {
                    // ���� ������ - ��� �������?
                    if ((pxSource -> ui16ID) == 0)
                    {
                        // ������ ������������� ������.
                        // ��� ������������� ��� ������ ������ ��� ������ � ����.
                        pxDestination[i].ui16Index =
                            i16IndexQuery;
                        // ������� ������� �������� - ��� �������.
                        pxDestination[i].ui16ID = 0;
                    }
                    else
                    {
                        // ������ ������������� ������.
                        // ��� ������������� ��� ������ ������ ��� ������ � ����.
                        pxDestination[i].ui16Index =
                            i16IndexQuery;
//                        // ����� ��������� ������.
//                        pxDestination[i].ui16Address =
//                        pxSource -> ui16Address;
//                        // ��������� ������.
//                        pxDestination[i].ui8State =
//                        pxSource -> ui8State;
                        // ��������� ������ ���������� ���������.
                        memcpy(&(pxDestination[i].acTextDescriptor),
                               (&(pxSource -> acTextDescriptor)),
                               (CError::ERROR_NAME_LENGTH + END_OF_STRING_LENGTH));
                        // ������ ������������?
                        if ((pxSource -> ui8Type) ==
                                CEvents::HANDLED_EVENTS_CONFIGURATION_ERROR_TYPE)
                        {
                            // ��������� ������ ������ ������.
                            memcpy(&(pxDestination[i].acTextData),
                                   (&(pxSource -> acTextData)),
                                   (CError::ERROR_NAME_LENGTH + END_OF_STRING_LENGTH));
                        }
                        else
                        {
                            // ����� �������� ������ �� ��� ������?
                            if ((pxSource -> xCurrentTime.tm_sec) != 0xFF)
                            {
                                // �������� ������ ������ ������.
                                // ��������� ����� ������.
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
                                        "������ ��� ������� ");
                            }
                        }
                    }
                }
                // ������ ��������� ������������� ������.
                i16IndexQuery++;
            }

            // �������� ����� ���������� ���������.
            i16Length =
                (CError::i16ErrorQueryListQuantity *
                 sizeof(struct CError::TErrorPackOne));
            // ����� ����� ���������.
            pxData -> i16Length = i16Length;
            // ����� ����� ���������.
            return i16Length;
        }
        break;

    case CMind::TASK_MODE_SERVER:
        // �������-������ ��������� ���������.
        // ��� �������-������ ���� ���������?
        if (pxData -> i16Length)
        {
            // ��������� �� ����������?
            if ((pxTaskContext -> ui8Status) != CMind::TASK_MODE_MESSAGE_IS_HANDLED)
            {
                CError::TErrorLogQueryListPackOne *pxSource;
                CError::TErrorLogQueryListPackOne *pxDestination;

                // ������� ��������� �� ����� ���������� ��������� � ����� ���������.
                pxSource = (CError::TErrorLogQueryListPackOne*)pxData -> pui8DataPointer;
                // ������� ��������� �� ������� ������ �� ������� �������� ������.
                pxDestination =
                    (CError::TErrorLogQueryListPackOne*)&CError::xErrorQueryList;
                // �������� ���������� ��������� ������ � �������.
                // �������� ���������� ��������� ������ � �������.
                CError::i16ErrorQueryListQuantity = pxSource -> ui8Quantity;
                // �������� �������� ������ �������� ������ � ������� ������.
                *pxDestination = *pxSource;
                // ��������� ���� - ��������� ����������.
                pxTaskContext -> ui8Status = CMind::TASK_MODE_MESSAGE_IS_HANDLED;
                // ����� ����� ������� ���������.
                pxData -> i16Length = 0;
                return 0;
            }
        }
        break;

    default:
        break;

    }
    // ����� ����� ������� ���������.
    pxData -> i16Length = 0;
    return 0;
}

//-----------------------------------------------------------------------------------------------------
// �������� ������� �� �������� ������� ��������� ���������� ������� �� ����(���: W - �����, ����: E - ������)
// ������� ������ ��������� ���������� ������� � ����.
int16_t CMind::DiscreteOutputsLogQueryEW(TModuleContext *pxModuleContext,
        CMind::TTaskContext *pxTaskContext,
        CSerialMT::TSerialMTData *pxData)
{
    int16_t i16Length;
    uint16_t ui16Index;
    // ������ �������������� �����(������) ��������� ���������� �������.
    int16_t i16IndexQuery;

    switch (pxData -> ui8Mode)
    {
    case CMind::TASK_MODE_CLIENT:
        // �������-������ ������� ���������.
        // ��������� ������� � ����������?
        if ((pxTaskContext -> ui8Status) == CMind::TASK_MODE_MESSAGE_IS_HANDLED)
        {
            uint8_t *pui8Source;
            CMind::TDiscreteOutputsPackOne *pxDestination;

            // ������� ���� - ��������� ����������.
            pxTaskContext -> ui8Status = 0;
            // ������� ��������� �� ������ ��������� ���������� �������.
            pui8Source =
                (uint8_t*)pxModuleContext ->
                xModuleContextStatic.
                pucSostDoOriginBufferPointer;
            // ������� ��������� �� ����� ���������� ��������� � ����� ���������.
            pxDestination = (CMind::TDiscreteOutputsPackOne*)(pxData -> pui8DataPointer);
            // ������� ���������� ��������� ����(�������) ��������� ���������� ������� � �������.
            i16Length = CMind::i16DiscreteOutputsQueryListQuantity;
            // ���������� ���������� ����(�������) ��������� ���������� ������� �����������
            // ����� ������ �������� ��������� ���������� �������?
            if (i16Length >
                    CMind::DISCRETE_INPUT_QUERY_LIST_LENGTH)
            {
                // ������.
                break;
            }

            // ������� ������ ������� �������������� �����(������) ��������� ���������� �������.
            i16IndexQuery = CMind::xDiscreteOutputsQueryList.ui16Index;
            // �������� ������ ��������� ���������� ������� �� ��������� ���������.
            for (uint16_t i = 0;
                    i < i16Length;
                    i++)
            {
                // ������ �������������� �����(������) ��������� ���������� �������
                // ������� �� ������� ������� ��������� ���������� �������?
                if (i16IndexQuery >=
                        (MAX_HANDLED_DISCRETE_OUTPUT /
                         MRXX_DISCRETE_OUTPUT_QUANTITY))
                {

                    // ������.
                    break;
                }

                // ������ �������������� �����(������) ��������� ���������� �������.
                // ��� ������������� ��� ������ �����(������) ��������� ���������� ������� ��� ������ � ����.
                pxDestination[i].ui16Index =
                    i16IndexQuery;

                // ������� ��������� ���������� �������.
                uint8_t ui8Data;
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
                // ������ ���������� �������������� �����(������) ��������� ���������� �������.
                i16IndexQuery++;
            }
            // �������� ����� ���������� ���������.
            i16Length =
                (CMind::i16DiscreteOutputsQueryListQuantity *
                 sizeof(struct CMind::TDiscreteOutputsPackOne));
            // ����� ����� ���������.
            pxData -> i16Length = i16Length;
            // ����� ����� ���������.
            return i16Length;
        }
        break;

    case CMind::TASK_MODE_SERVER:
        // �������-������ ��������� ���������.
        // ��� �������-������ ���� ���������?
        if (pxData -> i16Length)
        {
            // ��������� �� ����������?
            if ((pxTaskContext -> ui8Status) != CMind::TASK_MODE_MESSAGE_IS_HANDLED)
            {
                CMind::TDiscreteOutputsQueryListPackOne *pxSource;
                CMind::TDiscreteOutputsQueryListPackOne *pxDestination;

                // ������� ��������� �� ����� ���������� ��������� � ����� ���������.
                pxSource = (CMind::TDiscreteOutputsQueryListPackOne*)pxData -> pui8DataPointer;
                // ������� ��������� �� ������� ������ �� ������� �������� ��������� ���������� �������.
                pxDestination =
                    (CMind::TDiscreteOutputsQueryListPackOne*)&CMind::xDiscreteOutputsQueryList;
                // �������� ���������� ��������� ����(�������) ��������� ���������� ������� � �������.
                // �������� ���������� ��������� ����(�������) ��������� ���������� ������� � �������.
                CMind::i16DiscreteOutputsQueryListQuantity = pxSource -> ui8Quantity;
                // �������� �������� ������ �������� ��������� ���������� ������� � ������� ������.
                *pxDestination = *pxSource;
                // ��������� ���� - ��������� ����������.
                pxTaskContext -> ui8Status = CMind::TASK_MODE_MESSAGE_IS_HANDLED;
                // ����� ����� ������� ���������.
                pxData -> i16Length = 0;
                return 0;
            }
        }
        break;

    default:
        break;

    }
    // ����� ����� ������� ���������.
    pxData -> i16Length = 0;
    return 0;
}

//-----------------------------------------------------------------------------------------------------
// �������� ������� �� �������� ���������� ���������� �������� �� ����(���: W - �����, ����: E - ������)
// ������� ���������� ���������� �������� � ����.
int16_t CMind::AnalogueMeasureQueryEW(TModuleContext *pxModuleContext,
                                      CMind::TTaskContext *pxTaskContext,
                                      CSerialMT::TSerialMTData *pxData)
{
    int16_t i16Length;
    uint16_t ui16Index;

    switch (pxData -> ui8Mode)
    {
    case CMind::TASK_MODE_CLIENT:
        // �������-������ ������� ���������.
        // ��������� ������� � ����������?
        if ((pxTaskContext -> ui8Status) == CMind::TASK_MODE_MESSAGE_IS_HANDLED)
        {
            float *pfSource;
            TAnalogueInputDescriptionWork *pxSource;
            TDimentionParameter *pxDimentionParameter;
            uint8_t *pui8StatAiByteArray;
            CMind::TAnalogueMeasurePackOne *pxDestination;

            // ������� ���� - ��������� ����������.
            pxTaskContext -> ui8Status = 0;
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
            // ������� ��������� �� ����� ���������� ��������� � ����� ���������.
            pxDestination = (CMind::TAnalogueMeasurePackOne*)(pxData -> pui8DataPointer);
            // ������� ���������� ��������� ���������� ��������� � �������.
            i16Length = CMind::i16AnalogueMeasureQueryListQuantity;
            // ���������� ���������� ���������� ��������� �����������
            // ������ ������� axAnalogueMeasure?
            if (i16Length >
                    CMind::ANALOGUE_QUERY_MEASURE_LIST_LENGTH)
            {
                cout << "i16Length > CMind::ANALOGUE_QUERY_MEASURE_LIST_LENGTH" << endl;
                break;
            }
            // �������� ������ ���������� ��������� �� ��������� ���������.
            for (ui16Index = 0;
                    ui16Index < i16Length;
                    ui16Index++)
            {
                // �� ���������� ������ ���� ���������� ���������?
                if ((CMind::xAnalogueMeasureQueryList.ui8Address +
                        CMind::i16AnalogueMeasureQueryListQuantity) <
                        MAX_HANDLED_ANALOGUE_INPUT)
                {
                    uint8_t ui8Address;
                    uint8_t ui8Status;

                    ui8Address = (CMind::xAnalogueMeasureQueryList.ui8Address + ui16Index);
                    // ������� ����� ����������� ���������.
                    pxDestination[ui16Index].ui8Address =
                        ui8Address;

                    // ������� ��������� ����������� ���������.
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

                    // ������� �������� ����������� ���������.
                    pxDestination[ui16Index].fData =
                        pfSource[ui8Address];
                    // ��������� ������ ���������� ���������.
                    memcpy(&(pxDestination[ui16Index].acTextDescriptor),
                           (&(pxSource[ui8Address].acTextDescriptor)),
                           (ANALOGUE_INPUT_NAME_LENGTH + END_OF_STRING_LENGTH));
                    // ��������� ������ ���������� ��������� ����������� ����������� ��������� ����.
                    memcpy(&(pxDestination[ui16Index].acDimentionParameterText),
                           (&(pxDimentionParameter[(pxSource[ui8Address].ucDimentionCode)].acDimentionParameterText)),
                           (DIMENSIONS_PARAMETERS_NAME_LENGTH + END_OF_STRING_LENGTH));
                }
                else
                {
                    // ������� � ����� ���������� ���������.
                    memset(&(pxDestination[ui16Index].acTextDescriptor),
                           0,
                           (ANALOGUE_INPUT_NAME_LENGTH + END_OF_STRING_LENGTH));
                }
            }

            // �������� ����� ���������� ���������.
            i16Length =
                (CMind::i16AnalogueMeasureQueryListQuantity *
                 sizeof(struct CMind::TAnalogueMeasurePackOne));
            // ����� ����� ���������.
            pxData -> i16Length = i16Length;
            // ����� ����� ���������.
            return i16Length;
        }
        break;

    case CMind::TASK_MODE_SERVER:

        // �������-������ ��������� ���������.
        // ��� �������-������ ���� ���������?
        if (pxData -> i16Length)
        {
            // ��������� �� ����������?
            if ((pxTaskContext -> ui8Status) != CMind::TASK_MODE_MESSAGE_IS_HANDLED)
            {
                CMind::TAnalogueMeasureQueryListPackOne *pxSource;
                CMind::TAnalogueMeasureQueryListPackOne *pxDestination;

                // ������� ��������� �� ����� ���������� ��������� � ����� ���������.
                pxSource = (CMind::TAnalogueMeasureQueryListPackOne*)pxData -> pui8DataPointer;
                // ������� ��������� �� ������� ������ �� ������� �������� ���������� ���������.
                pxDestination =
                    (CMind::TAnalogueMeasureQueryListPackOne*)&xAnalogueMeasureQueryList;
                // �������� ���������� ��������� ���������� ��������� � �������.
                // �������� ���������� ��������� ���������� ��������� � �������.
                CMind::i16AnalogueMeasureQueryListQuantity = pxSource -> ui8Quantity;
                // �������� �������� ������ �������� ���������� ��������� � ������� ������.
                *pxDestination = *pxSource;
                // ��������� ���� - ��������� ����������.
                pxTaskContext -> ui8Status = CMind::TASK_MODE_MESSAGE_IS_HANDLED;
                // ����� ����� ������� ���������.
                pxData -> i16Length = 0;
                return 0;
            }
        }
        break;

    default:
        break;

    }
    // ����� ����� ������� ���������.
    pxData -> i16Length = 0;
    return 0;
}

//-----------------------------------------------------------------------------------------------------
// ������������ ����� � ����. ���� ���� �� �������� - ������.
// �������� �� ������ �������� ����� �� ����.
// (���: W - �����, ����: E - ������)
int16_t CMind::PingTaskEWWE(TModuleContext *pxModuleContext,
                            CMind::TTaskContext *pxTaskContext,
                            CSerialMT::TSerialMTData *pxData)
{
    int16_t i16Length;

    switch (pxData -> ui8Mode)
    {
    case CMind::TASK_MODE_CLIENT:
        // �������-������ ������� ���������.
        // ��������� �� ����������?
        if (pxTaskContext -> fui8MessageIsSent == 0)

        {
            // �������� ���������.
            *(pxData -> pui8DataPointer) = CMind::TASK_ANSWER_OK;
            // ������� ������� �������������� ������� �������� ������ �������� �� ���������.
            pxTaskContext -> nui8TimeCounter = 0;
            // ��������� ���� - ��������� ����������.
            pxTaskContext -> fui8MessageIsSent = 1;
            // �������� ����� ���������� ���������.
            i16Length = CMind::TASK_PING_REQUEST_MESSAGE_LENGTH;
            // ����� ����� ���������.
            pxData -> i16Length = i16Length;
            // ����� ����� ���������.
            return i16Length;
        }
        break;

    case CMind::TASK_MODE_SERVER:
        // �������-������ ��������� ���������.
        // ��������� ����������?
        if (pxTaskContext -> fui8MessageIsSent == 1)
        {
            // �������-������ � ���� �������(��������� ���������).
            // ��� �������-������ ���� ���������?
            if (pxData -> i16Length)
            {
                // �������� ������������� �����?
                if (*(pxData -> pui8DataPointer) == CMind::TASK_ANSWER_OK)
                {
                    // �������� ������������� �����(��������� ���������� �������).
                    // ������� ���� - ��������� ����������.
                    pxTaskContext -> fui8MessageIsSent = 0;
                }
                // �������� ������������� ����� � �������?
                else if (*(pxData -> pui8DataPointer) == CMind::TASK_ANSWER_ERROR)
                {
                    // �������� ������������� ����� � �������.
                    // ������� ���� - ��������� ����������.
                    pxTaskContext -> fui8MessageIsSent = 0;
                }
            }
            else
            {
                // ��������� ����� �������� ������?
                if ((pxTaskContext -> nui8TimeCounter) >= CMind::TASK_NO_ANSWER_TIMEOUT)
                {
                    // ��� ������ �� ���������.
                    // ������� ���� - ��������� ����������.
                    pxTaskContext -> fui8MessageIsSent = 0;
                }
                else
                {
                    // �������� ������� �������������� ������� �������� ������ �������� �� ���������.
                    (pxTaskContext -> nui8TimeCounter)++;
                }
            }
        }
        break;

    default:
        break;
    }

    // ����� ����� ������� ���������.
    pxData -> i16Length = 0;
    return 0;


//    int16_t i16Length;
//
//    switch (pxData -> ui8Mode)
//    {
//    case TASK_MODE_CLIENT:
////        printf("Ping TASK_MODE_CLIENT %d\n\r", nui8FrameCounter);
//        // �������-������ � ���� �������(������� ���������).
//        *(pxData -> pui8DataPointer) = nui8FrameCounter;
//        // ����� ����� ���������.
//        pxData -> i16Length = 1;
//        return i16Length;
//        break;
//
//    case TASK_MODE_SERVER:
////        printf("Ping TASK_MODE_SERVER %d\n\r", *(pxData -> pui8DataPointer));
//        // �������-������ � ���� �������(��������� ���������).
//        // ����� ����� ������� ���������.
//        pxData -> i16Length = 0;
//        return 0;
//        break;
//
//    default:
//        break;
//    }
//
//    // ����� ����� ������� ���������.
//    pxData -> i16Length = 0;
//    return 0;
}

//-----------------------------------------------------------------------------------------------------
// ������-��������.
// ���������� �� ��������� �� ������� � ���������� ����� �� ������-������� 0.
// �����: 0 - �� �������� ����������(SerialMT).
int16_t CMind::NopTask(TModuleContext *pxModuleContext,
                       CMind::TTaskContext *pxTaskContext,
                       CSerialMT::TSerialMTData *pxData)
{
    // ����� ����� ������� ���������.
    pxData -> i16Length = 0;
    return 0;
}

//-----------------------------------------------------------------------------------------------------
// ������ ��������� ������ ��������� �������� ������������ ����� ������������ �����
// CSpiMT::MAX_RECEIVED_FRAME_LENGTH.
// �������� � ���� �� ���� SPI ���������� � ���������� ������.
// ���� ����� ������������� ����� ������ ����� ������������ �����(CSpiMT::MAX_RECEIVED_FRAME_LENGTH), ��
// ����������� ���� ����� �������.
// ������� ����� ������������� ����� ������ ���� ������ ������������ �����.
// ������������ ����� ������������ ����� �������� �� ������ ��������������.
// � ������ ������������ ����� ������ ��������� ��������� SerialMT �
// ������������ ����� ������������ �����.
// ���� ����� ������ ��������� ��������� SerialMT ������ ������������ �����
// ������������ �����, �� ������ ��������� ������ ���������
// �������� ������������ ����� ������������ �����.
int16_t CMind::ReceiveRequest(TModuleContext *pxModuleContext,
                              CMind::TTaskContext *pxTaskContext,
                              CSerialMT::TSerialMTData *pxData)
{
    int16_t i16Length;

    switch (pxData -> ui8Mode)
    {
    case TASK_MODE_CLIENT:
        // �������-������ � ���� �������(������� ���������).
        // �������� ���������.
        *(pxData -> pui8DataPointer) = 0;
        // ����� ����� ���������.
        pxData -> i16Length = CSerialMT::MAX_RECEIVED_FRAME_LENGTH;
        return pxData -> i16Length;
        break;

    case TASK_MODE_SERVER:
        // �������-������ � ���� �������(��������� ���������).
        // ����� ����� ������� ���������.
        pxData -> i16Length = 0;
        return 0;
        break;

    default:
        break;
    }

    // ����� ����� ������� ���������.
    pxData -> i16Length = 0;
    return 0;
}

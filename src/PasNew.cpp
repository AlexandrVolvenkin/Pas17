//============================================================================
// Name        : PAS8.cpp
// Author      :
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "PasNewConfig.h"
#include "Platform.h"
//#include "../src/Modbus/modbus-private.h"
#include "Alarm.h"
#include "Error.h"
#include "Events.h"
#include "EventsDB.h"
#include "iconvlite.h"
#include "Task.h"
#include "Parse.h"
#include "RingBuffer.h"
#include "SystemComponent.h"
#include "MainProductionCycle.h"
#include "DataStore.h"
#include "StorageDevice.h"

//#include "Logger.h"
#include "nkh-lab/logger.hpp"

#include "test.h"

using namespace std;

#define MODBUS_RTU_ENABLE
#define MODBUS_TCP_ENABLE
#define MODBUS_HMI_ENABLE
//#define MODBUS_MASTER_TCP_ENABLE
#define MODBUS_MASTER_RTU_ENABLE

//// массив сопоставления номера посадочного слота модуля с адресом на шине SPI.
//const uint8_t aui8ModuleSlotNumberToSpiAddressMatching[SPI_CHIP_SELECT_MAX_ADDRESS] =
//{
//    0x0F,
//    0x07,
//    0x0B,
//    0x03,
//    0x0D,
//    0x05,
//    0x09,
//    0x01,
//    0x0E,
//    0x06,
//    0x0A,
//    0x02,
//    0x0C,
//    0x04,
//    0x08,
//    0x00
//};

// массив содержит размеры блоков базы данных.
// массив базы данных состоит из 100 блоков по 256 байт.
// когда программатор запрашивает нужный ему блок, (в функции - modbus_reply() - case _FC_DATA_BASE_READ:),
// из этого массива, по номеру, программатору выдаётся размер блока.
const unsigned char aucDataBaseBlockLength[PLC_DATA_BASE_BLOCK_QUANTITY] =
{
    RESERVED_DATA_BASE_BLOCK_LENGTH,

    ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_LENGTH,
    ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_LENGTH,
    ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_LENGTH,
    ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_LENGTH,
    ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_LENGTH,
    ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_LENGTH,
    ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_LENGTH,
    ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_LENGTH,
    ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_LENGTH,



    ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_LENGTH,
    ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_LENGTH,
    ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_LENGTH,
    ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_LENGTH,
    ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_LENGTH,
    ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_LENGTH,
    ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_LENGTH,

    DISCRETE_OUTPUT_MODULE_DATA_BASE_BLOCK_LENGTH,

    MBM_FUNCTION_1234_BLOCK_DATA_BASE_BLOCK_LENGTH,
    MBM_FUNCTION_1234_BLOCK_DATA_BASE_BLOCK_LENGTH,



    MBM_FUNCTION_15_BLOCK_DATA_BASE_BLOCK_LENGTH,

    DISCRETE_INPUT_SYGNALS_DATA_BASE_BLOCK_LENGTH,
    DISCRETE_INPUT_SYGNALS_DATA_BASE_BLOCK_LENGTH,
    DISCRETE_INPUT_SYGNALS_DATA_BASE_BLOCK_LENGTH,
    DISCRETE_INPUT_SYGNALS_DATA_BASE_BLOCK_LENGTH,
    DISCRETE_INPUT_SYGNALS_DATA_BASE_BLOCK_LENGTH,
    DISCRETE_INPUT_SYGNALS_DATA_BASE_BLOCK_LENGTH,
    DISCRETE_INPUT_SYGNALS_DATA_BASE_BLOCK_LENGTH,
    DISCRETE_INPUT_SYGNALS_DATA_BASE_BLOCK_LENGTH,
    DISCRETE_INPUT_SYGNALS_DATA_BASE_BLOCK_LENGTH,



    DISCRETE_INPUT_SYGNALS_DATA_BASE_BLOCK_LENGTH,
    DISCRETE_INPUT_SYGNALS_DATA_BASE_BLOCK_LENGTH,
    DISCRETE_INPUT_SYGNALS_DATA_BASE_BLOCK_LENGTH,
    DISCRETE_INPUT_SYGNALS_DATA_BASE_BLOCK_LENGTH,
    DISCRETE_INPUT_SYGNALS_DATA_BASE_BLOCK_LENGTH,
    DISCRETE_INPUT_SYGNALS_DATA_BASE_BLOCK_LENGTH,
    DISCRETE_INPUT_SYGNALS_DATA_BASE_BLOCK_LENGTH,
    DISCRETE_INPUT_SYGNALS_DATA_BASE_BLOCK_LENGTH,
    DISCRETE_INPUT_SYGNALS_DATA_BASE_BLOCK_LENGTH,
    DISCRETE_INPUT_SYGNALS_DATA_BASE_BLOCK_LENGTH,



    DISCRETE_INPUT_SYGNALS_DATA_BASE_BLOCK_LENGTH,
    DISCRETE_INPUT_SYGNALS_DATA_BASE_BLOCK_LENGTH,
    DISCRETE_INPUT_SYGNALS_DATA_BASE_BLOCK_LENGTH,

    CURRENT_OUTPUT_MODULE_REGULATOR_DATA_BASE_BLOCK_LENGTH,
    CURRENT_OUTPUT_MODULE_REGULATOR_DATA_BASE_BLOCK_LENGTH,

    DIMENSIONS_PARAMETERS_DATA_BASE_BLOCK_LENGTH,

    TEXT_TITLES_DATA_BASE_BLOCK_LENGTH,
    TEXT_TITLES_DATA_BASE_BLOCK_LENGTH,
    TEXT_TITLES_DATA_BASE_BLOCK_LENGTH,
    TEXT_TITLES_DATA_BASE_BLOCK_LENGTH,



    TEXT_TITLES_DATA_BASE_BLOCK_LENGTH,
    TEXT_TITLES_DATA_BASE_BLOCK_LENGTH,
    TEXT_TITLES_DATA_BASE_BLOCK_LENGTH,
    TEXT_TITLES_DATA_BASE_BLOCK_LENGTH,
    TEXT_TITLES_DATA_BASE_BLOCK_LENGTH,
    TEXT_TITLES_DATA_BASE_BLOCK_LENGTH,
    TEXT_TITLES_DATA_BASE_BLOCK_LENGTH,
    TEXT_TITLES_DATA_BASE_BLOCK_LENGTH,
    TEXT_TITLES_DATA_BASE_BLOCK_LENGTH,
    TEXT_TITLES_DATA_BASE_BLOCK_LENGTH,



    TEXT_TITLES_DATA_BASE_BLOCK_LENGTH,
    TEXT_TITLES_DATA_BASE_BLOCK_LENGTH,
    TEXT_TITLES_DATA_BASE_BLOCK_LENGTH,
    TEXT_TITLES_DATA_BASE_BLOCK_LENGTH,
    TEXT_TITLES_DATA_BASE_BLOCK_LENGTH,
    TEXT_TITLES_DATA_BASE_BLOCK_LENGTH,
    TEXT_TITLES_DATA_BASE_BLOCK_LENGTH,
    TEXT_TITLES_DATA_BASE_BLOCK_LENGTH,
    TEXT_TITLES_DATA_BASE_BLOCK_LENGTH,
    TEXT_TITLES_DATA_BASE_BLOCK_LENGTH,


    FUNCTION_BLOCK_DATA_BASE_BLOCK_LENGTH,
    FUNCTION_BLOCK_DATA_BASE_BLOCK_LENGTH,
    FUNCTION_BLOCK_DATA_BASE_BLOCK_LENGTH,
    FUNCTION_BLOCK_DATA_BASE_BLOCK_LENGTH,
    FUNCTION_BLOCK_DATA_BASE_BLOCK_LENGTH,
    FUNCTION_BLOCK_DATA_BASE_BLOCK_LENGTH,
    FUNCTION_BLOCK_DATA_BASE_BLOCK_LENGTH,
    FUNCTION_BLOCK_DATA_BASE_BLOCK_LENGTH,
    FUNCTION_BLOCK_DATA_BASE_BLOCK_LENGTH,
    FUNCTION_BLOCK_DATA_BASE_BLOCK_LENGTH,



    FUNCTION_BLOCK_DATA_BASE_BLOCK_LENGTH,
    FUNCTION_BLOCK_DATA_BASE_BLOCK_LENGTH,


    MATHEMATICAL_BLOCK_DATA_BASE_BLOCK_LENGTH,
    MATHEMATICAL_BLOCK_DATA_BASE_BLOCK_LENGTH,
    MATHEMATICAL_BLOCK_DATA_BASE_BLOCK_LENGTH,
//    RESERVED_DATA_BASE_BLOCK_LENGTH,
//    RESERVED_DATA_BASE_BLOCK_LENGTH,
//    RESERVED_DATA_BASE_BLOCK_LENGTH,

    RESERVED_DATA_BASE_BLOCK_LENGTH,
    RESERVED_DATA_BASE_BLOCK_LENGTH,
    RESERVED_DATA_BASE_BLOCK_LENGTH,
    RESERVED_DATA_BASE_BLOCK_LENGTH,
    RESERVED_DATA_BASE_BLOCK_LENGTH,



    RESERVED_DATA_BASE_BLOCK_LENGTH,
    RESERVED_DATA_BASE_BLOCK_LENGTH,
    RESERVED_DATA_BASE_BLOCK_LENGTH,
    RESERVED_DATA_BASE_BLOCK_LENGTH,
    RESERVED_DATA_BASE_BLOCK_LENGTH,
    RESERVED_DATA_BASE_BLOCK_LENGTH,
    RESERVED_DATA_BASE_BLOCK_LENGTH,
    RESERVED_DATA_BASE_BLOCK_LENGTH,

    DISCRETE_INPUT_MODULE_DATA_BASE_BLOCK_LENGTH,

    NETWORK_ADDRESS_DATA_BASE_BLOCK_LENGTH
};

const  TAlarmHmi xAlarmHmiNull =
{
    0
};

//uint8_t ui8TempCommand;

//CCommunicationDeviceInterface xSpiCommunicationDevice("/dev/spidev0.0");
CSpi xSpiCommunicationDevice("/dev/spidev0.0");
//CSpiInterface xSpiCommunicationDevice;
CSerialPort xSerialPortCommunicationDevice;

TPlcDataBaseControl xPlcDataBaseControl;
TAllModulesContext xAllModulesContext;
// массив с объектами контекста всех модулей.
//TModuleContext axAllModulesContext[MAX_MODULE_QUANTITY];
TPlcConfigSearch xPlcConfigSearch;
TPlcConfigSearch xPlcConfigSearchTemp;
TPlcConfigData xPlcConfigData;
TPlcConfigData xPlcConfigDataTemp;
TPlcConfigData xPlcConfigDataTransmit;
TPlcConfigData *pxPlcConfigData;
TPlcConfigService xPlcConfigService;
TPlcConfigService *pxPlcConfigService;
TPlcDataBase xPlcDataBase;
TPlcDataBase *pxPlcDataBase;
TMainFlagRegister xMainFlagRegister;
struct tm xCurrentTime;
uint8_t ui8CurrentTimeUpdateDelayCounter;
uint8_t ui8CurrentTimeSaveDelayCounter;

TDataExchangeLinuxToCodeSysBuff *pxDataExchangeLinuxToCodeSysBuff;
TDataExchangeCodeSysToLinuxBuff *pxDataExchangeCodeSysToLinuxBuff;
TDataExchangeControlBuff *pxDataExchangeControlBuff;

pthread_mutex_t xMainDataAccessMutex;

unsigned char ucMainCycleTimeStart;
unsigned char ucCodeSysExchangeErrorCycleCounter;
unsigned char ucDataBaseError;
unsigned char ucModuleError;
unsigned char ucModbusSlaveAddress;


struct sigaction MainAction;
//struct sigaction ArchiveSave;
uint8_t ui8MainRestart;

//struct timeval  xGlobalTimeCur;
//struct timeval xGlobalTimeLast;

bool fbGlobalKvit;
bool fbGlobalReset;
bool fbGlobalBlock;
//bool fbHighLevelKvit;
//bool fbHighLevelReset;
//bool fbHighLevelBlock;
//bool fbMindKvit; // флаг - квитирование от МИНД.
//bool fbMindReset; // флаг - сброс от МИНД.
//bool fbMindBlock; // флаг - блокировка от МИНД.
bool fbGlobalMmcCopy; // флаг - копирование диска mmc0 на mmc1.
bool fbGlobalMmcCopyEnd; // флаг - закончено копирование диска mmc0 на mmc1.

bool fbOneSecondPassed;
bool fbTenSecondsPassed;
unsigned char nucOneSecondCounter;
unsigned char nucTenSecondsCounter;
unsigned char ucAlarmStateCommon;
unsigned char fucZvkMal;
unsigned char fucKvSbBef;
unsigned char nucBlinkCounter;
bool fbBlinkIsOn;
bool fbDiscreteInputDataBaseError;
bool fbFunctionBlockDataBaseError;
bool fbPlcErrorReset;
bool fbPlcDataBaseFileError;
bool fbPlcConfigurationFileError;
bool fbPlcConfigurationError;
bool fbAnalogueInputModuleDataBaseError;
bool fbDiscreteOutputModuleDataBaseError;
bool fbLocalNetworkModuleDataBaseError;
bool fbAnalogueInputModuleReperPointsAdcDataBaseError;
bool fbMindDataExchangeError;
bool fbCodeSysDataExchangeError;
bool fbModuleMrOutOff;

//struct timeval  xTimeCur;
//struct timeval xTimeLast;

//extern uint8_t aui8CurrentTime[CURRENT_TIME_BYTE_QUANTITY];
unsigned char aucStatAiByteArray[STAT_AI_BYTE_ARRAY_LENGTH];
unsigned char aucContStByteArray[CONT_ST_BYTE_ARRAY_LENGTH];
unsigned char aucContStWriteBitArray[CONT_ST_BIT_ARRAY_LENGTH];
unsigned char aucContStWriteByteArray[CONT_ST_BYTE_ARRAY_LENGTH];
unsigned char aucDoValueByteArray[DO_VALUE_BIT_ARRAY_LENGTH];
unsigned char aucDoValueBlockByteArray[DO_VALUE_BIT_ARRAY_LENGTH];
TTxsData axAllModulesTxsData[INTERNAL_MODULE_QUANTITY];
float afAnalogueInputData[MAX_HANDLED_ANALOGUE_INPUT];
float afCalcValue[CALC_CDS_FLOAT_ARRAY_LENGTH];
float afSysValue[SYS_CDS_FLOAT_ARRAY_LENGTH];
unsigned char aucReperPointsAdcBufferPointer[ANALOGUE_INPUT_MODULE_REPER_POINTS_ADC_DATA_BASE_BLOCK_LENGTH * (MAX_HANDLED_ANALOGUE_INPUT / ANALOG_MODULE_INPUT_QUANTITY)];
TAnalogueInputDescriptionWork axAnalogueInputDescriptionWork[MAX_HANDLED_ANALOGUE_INPUT];
TAnalogueOutputRegulatorDescriptionWork axAnalogueOutputRegulatorDescriptionWork[MAX_HANDLED_ANALOGUE_OUTPUT];
TMtvi5ContAvData axAnalogueOutputRegulatorContAv[MAX_HANDLED_ANALOGUE_OUTPUT];
TDimentionParameter axDimentionsParametersDescriptionWork[DIMENSIONS_PARAMETERS_DATA_BLOCKS_IN_BLOCK_QUANTITY];
TDiscreteInputDescriptionWork axDiscreteInputDescriptionWork[MAX_HANDLED_DISCRETE_INPUT];
TDiscreteSygnalTextDescriptor axDiscreteSygnalTextDescriptor[MAX_HANDLED_DISCRETE_INPUT];
TMbmFunction1234 axModbusMasterFunction1234DescriptionWork[EXTERNAL_MODULE_FUNCTION_1234_QUANTITY];
TDiscreteInputWorkingData axDiscreteInputWorkingData[MAX_HANDLED_DISCRETE_INPUT];
TAlarmHmi axAlarmHmi[];
TAlarmHmi axAlarmModules[BAD_MODULES_BIT_ARRAY_LENGTH];
unsigned char aucGroupDinAction[ALARM_GROUP_QUANTITY * ALARM_TYPE_QUANTITY_OF_SIGNIFICANT_BIT];
unsigned char aucDinAction[(MAX_HANDLED_DISCRETE_INPUT + MAX_HANDLED_FUNCTION_BLOCK) * ALARM_TYPE_QUANTITY_OF_SIGNIFICANT_BIT];
TFunctionBlockDescriptionWork axFunctionBlockDescriptionWork[MAX_HANDLED_FUNCTION_BLOCK];
TFunctionBlockWorkingData axFunctionBlockWorkingData[MAX_HANDLED_FUNCTION_BLOCK];
//unsigned char aucModbusIncomingMessageData[PLC_DATA_BASE_BLOCK_LENGTH];
unsigned char aucBadModuleCompareBuff[BAD_MODULES_BIT_ARRAY_LENGTH];
unsigned char aucBadModuleStateBuff[MAX_MODULE_QUANTITY];

unsigned char aucCoilsArray[COILS_WORK_ARRAY_LENGTH];
unsigned char aucDiscreteInputsArray[DISCRETE_INPUTS_ARRAY_LENGTH];
unsigned char aucHoldingRegistersArray[HOLDING_REGISTERS_ARRAY_LENGTH];
unsigned char aucInputRegistersArray[INPUT_REGISTERS_ARRAY_LENGTH];

//-----------------------------------------------------------------------------------------------------
modbus_mapping_t *pxCommonModbusMapping;
#ifdef MODBUS_MASTER_RTU_ENABLE

modbus_t xMasterRtuCtx;
modbus_t *pxMasterRtuCtx;
//modbus_mapping_t xMasterRtuModbusMapping;
//modbus_mapping_t *pxMasterRtuModbusMapping;
//unsigned char aucMasterRtuCoilsArray[COILS_WORK_ARRAY_LENGTH];
//unsigned char aucMasterRtuDiscreteInputsArray[DISCRETE_INPUTS_ARRAY_LENGTH];
//unsigned char aucMasterRtuHoldingRegistersArray[HOLDING_REGISTERS_ARRAY_LENGTH];
//unsigned char aucMasterRtuInputRegistersArray[INPUT_REGISTERS_ARRAY_LENGTH];
//unsigned char aucMasterRtuAuxiliaryArray[MAX_MESSAGE_LENGTH];
#endif // MODBUS_MASTER_RTU_ENABLE

#ifdef MODBUS_MASTER_TCP_ENABLE
modbus_t xMasterTcpCtx;
modbus_t *pxMasterTcpCtx;
//modbus_mapping_t xMasterTcpModbusMapping;
//modbus_mapping_t *pxMasterTcpModbusMapping;
//unsigned char aucMasterTcpCoilsArray[COILS_WORK_ARRAY_LENGTH];
//unsigned char aucMasterTcpDiscreteInputsArray[DISCRETE_INPUTS_ARRAY_LENGTH];
//unsigned char aucMasterTcpHoldingRegistersArray[HOLDING_REGISTERS_ARRAY_LENGTH];
//unsigned char aucMasterTcpInputRegistersArray[INPUT_REGISTERS_ARRAY_LENGTH];
//unsigned char aucMasterTcpAuxiliaryArray[MAX_MESSAGE_LENGTH];
#endif // MODBUS_MASTER_TCP_ENABLE

#ifdef MODBUS_TCP_ENABLE
uint8_t ui8ModbusTcpRestart;
pthread_t xModbusTcp;
struct sigaction ModbusTcpAction;
modbus_t xSlaveTcpCtx;
modbus_t *pxSlaveTcpCtx;
modbus_mapping_t xTcpModbusMapping;
modbus_mapping_t *pxTcpModbusMapping;
unsigned char aucTcpCoilsArray[COILS_WORK_ARRAY_LENGTH];
unsigned char aucTcpDiscreteInputsArray[DISCRETE_INPUTS_ARRAY_LENGTH];
unsigned char aucTcpHoldingRegistersArray[HOLDING_REGISTERS_ARRAY_LENGTH];
unsigned char aucTcpInputRegistersArray[INPUT_REGISTERS_ARRAY_LENGTH];
unsigned char aucTcpAuxiliaryArray[MAX_MESSAGE_LENGTH];
#endif // MODBUS_TCP_ENABLE

#ifdef MODBUS_RTU_ENABLE
uint8_t ui8ModbusRtuRestart;
pthread_t xModbusRtu;
struct sigaction ModbusRtuAction;
modbus_t xSlaveRtuCtx;
modbus_t *pxSlaveRtuCtx;
modbus_mapping_t xRtuModbusMapping;
modbus_mapping_t *pxRtuModbusMapping;
unsigned char aucRtuCoilsArray[COILS_WORK_ARRAY_LENGTH];
unsigned char aucRtuDiscreteInputsArray[DISCRETE_INPUTS_ARRAY_LENGTH];
unsigned char aucRtuHoldingRegistersArray[HOLDING_REGISTERS_ARRAY_LENGTH];
unsigned char aucRtuInputRegistersArray[INPUT_REGISTERS_ARRAY_LENGTH];
unsigned char aucRtuAuxiliaryArray[MAX_MESSAGE_LENGTH];
#endif // MODBUS_RTU_ENABLE

#ifdef MODBUS_HMI_ENABLE
uint8_t ui8ModbusHmiRestart;
pthread_t xHmiModbusRtu;
struct sigaction ModbusHmiAction;
modbus_t xSlaveHmiRtuCtx;
modbus_t *pxSlaveHmiRtuCtx;
modbus_mapping_t xHmiRtuModbusMapping;
modbus_mapping_t *pxHmiRtuModbusMapping;
unsigned char aucHmiRtuCoilsArray[COILS_WORK_ARRAY_LENGTH];
unsigned char aucHmiRtuDiscreteInputsArray[DISCRETE_INPUTS_ARRAY_LENGTH];
unsigned char aucHmiRtuHoldingRegistersArray[HOLDING_REGISTERS_ARRAY_LENGTH];
unsigned char aucHmiRtuInputRegistersArray[INPUT_REGISTERS_ARRAY_LENGTH];
unsigned char aucHmiAuxiliaryArray[MAX_MESSAGE_LENGTH];
#endif // MODBUS_HMI_ENABLE

pthread_t xExternalModuleInteractionCycle;      // this is our handle to the pthread

//-----------------------------------------------------------------------------------------------------
// поток взаимодействия с модулями локальной сети нижнего уровня.
void *thread_vExternalModuleInteractionCycle(void *value)
{
    unsigned char nucIndexNumber;
    TMbmFunction1234* pxFunction1234Pointer;

    // получим указатель на массив с нормализованной базой данных.
    pxFunction1234Pointer =
        axModbusMasterFunction1234DescriptionWork;
    while (1)
    {
        // нет ошибок базы данных или конфигурации?
        if ((!fbPlcConfigurationError) &&
                (!fbPlcDataBaseFileError))
        {
            // внешние модули запрограммированы?
            if (xPlcConfigService.xPlcConfigServiceData.ucExternalModulesQuantity)
            {
                // начало контекста модулей локальной сети нижнего уровня.
                nucIndexNumber =
                    (xPlcConfigService.xPlcConfigServiceData.ucInternalModulesQuantity);
                for (int i = 0;
                        i < (xPlcConfigService.xPlcConfigServiceData.ucExternalModulesQuantity);
                        i++)
                {
//                printf("thread_vExternalModuleInteractionCycle switch %d\n\r", ((xAllModulesContext.axAllModulesContext[nucIndexNumber].
//                            xModuleContextStatic.
//                            ucModuleType)));
                    // какой тип внешнего модуля?
                    switch(xAllModulesContext.axAllModulesContext[nucIndexNumber].
                            xModuleContextStatic.
                            ucModuleType)
                    {
                    case OWN_MODULE_TYPE_FUNCTION_1:
//                        cout << "OWN_MODULE_TYPE_FUNCTION_1 iMbmFunction1DataHandler" << endl;
                        iMbmFunction1DataHandler(&xAllModulesContext.axAllModulesContext[nucIndexNumber]);
                        break;
                    case OWN_MODULE_TYPE_FUNCTION_2:
//                        cout << "OWN_MODULE_TYPE_FUNCTION_2 iMbmFunction2DataHandler" << endl;
                        iMbmFunction2DataHandler(&xAllModulesContext.axAllModulesContext[nucIndexNumber]);
                        break;
                    case OWN_MODULE_TYPE_FUNCTION_3:
//                        cout << "OWN_MODULE_TYPE_FUNCTION_3 iMbmFunction3OwnProductionDataHandler" << endl;
                        iMbmFunction3DataHandler(&xAllModulesContext.axAllModulesContext[nucIndexNumber]);
//                printf("thread_vExternalModuleInteractionCycle OWN_MODULE_TYPE_FUNCTION_3 %d\n\r", ((xAllModulesContext.axAllModulesContext[nucIndexNumber].
//                            xModuleContextStatic.
//                            ucModuleType)));
                        break;
                    case OWN_MODULE_TYPE_FUNCTION_4:
//                        cout << "OWN_MODULE_TYPE_FUNCTION_4 iMbmFunction4OwnProductionDataHandler" << endl;
                        iMbmFunction4DataHandler(&xAllModulesContext.axAllModulesContext[nucIndexNumber]);
                        break;


                    case OTHER_MODULE_TYPE_FUNCTION_1:
//                        cout << "OTHER_MODULE_TYPE_FUNCTION_1 iMbmFunction1DataHandler" << endl;
                        iMbmFunction1DataHandler(&xAllModulesContext.axAllModulesContext[nucIndexNumber]);
                        break;
                    case OTHER_MODULE_TYPE_FUNCTION_2:
//                        cout << "OTHER_MODULE_TYPE_FUNCTION_2 iMbmFunction2DataHandler" << endl;
                        iMbmFunction2DataHandler(&xAllModulesContext.axAllModulesContext[nucIndexNumber]);
                        break;
                    case OTHER_MODULE_TYPE_FUNCTION_3:
//                        cout << "OTHER_MODULE_TYPE_FUNCTION_3 iMbmFunction3DataHandler" << endl;
                        iMbmFunction3DataHandler(&xAllModulesContext.axAllModulesContext[nucIndexNumber]);
                        break;
                    case OTHER_MODULE_TYPE_FUNCTION_4:
//                        cout << "OTHER_MODULE_TYPE_FUNCTION_4 iMbmFunction4DataHandler" << endl;
                        iMbmFunction4DataHandler(&xAllModulesContext.axAllModulesContext[nucIndexNumber]);
                        break;
                    case OTHER_MODULE_TYPE_FUNCTION_15:
//                        cout << "OTHER_MODULE_TYPE_FUNCTION_15 vMbmFunction15DataHandler" << endl;
                        vMbmFunction15DataHandler(&xAllModulesContext.axAllModulesContext[nucIndexNumber]);
                        break;

                    default:
                        break;
                    };
                    nucIndexNumber++;
                }
            }
        }

        nui8FrameCounter++; // modbus mind.
//        xCPingTaskEWWE.DataExchange(CTaskSerialMT::apxTasksProcessed, &xCModbusSerialMT);
//        cout << "DataExchangeModbus begin" << endl;
        usleep(1000);
//        cout << "DataExchangeModbus usleep" << endl;
    }
}

//-----------------------------------------------------------------------------------------------------
// поток ModbusTcp, связь с верхним уровнем, Ethernet.
#ifdef MODBUS_TCP_ENABLE
void ModbusTcpActionHandler(int signo, siginfo_t *info, void *context)
{
    ui8ModbusTcpRestart = 1;
    cout << "action ModbusTcp" << endl;
}

void *thread_vModbusTcpHandler(void *value)
{
    int socket;
    modbus_t *ctx;
    modbus_mapping_t *mb_mapping;
    int rc;
    int header_length;
    unsigned char query[MODBUS_TCP_MAX_ADU_LENGTH * 2];

    memset((unsigned char*)&xSlaveTcpCtx, 0, sizeof(modbus_t));
    pxSlaveTcpCtx = &xSlaveTcpCtx;
    memset((unsigned char*)&xTcpModbusMapping, 0, sizeof(modbus_mapping_t));
    mb_mapping = &xTcpModbusMapping;
    pxTcpModbusMapping = &xTcpModbusMapping;

    mb_mapping->tab_bits = (unsigned char*)aucTcpCoilsArray;
    mb_mapping->tab_input_bits = (unsigned char*)aucTcpDiscreteInputsArray;
    mb_mapping->tab_registers = (unsigned short*)aucTcpHoldingRegistersArray;
    mb_mapping->tab_input_registers = (unsigned short*)aucTcpInputRegistersArray;
    mb_mapping->tab_auxiliary = (unsigned char*)aucTcpAuxiliaryArray;

    mb_mapping->nb_bits = COILS_WORK_ARRAY_LENGTH;
    mb_mapping->nb_input_bits = DISCRETE_INPUTS_ARRAY_LENGTH;
    mb_mapping->nb_registers = (HOLDING_REGISTERS_ARRAY_LENGTH / sizeof(short));
    mb_mapping->nb_input_registers = (INPUT_REGISTERS_ARRAY_LENGTH / sizeof(short));

    memset(mb_mapping->tab_bits, 0, mb_mapping->nb_bits * sizeof(unsigned char));
    memset(mb_mapping->tab_input_bits, 0, mb_mapping->nb_input_bits * sizeof(unsigned char));
    memset(mb_mapping->tab_registers, 0, mb_mapping->nb_registers * sizeof(unsigned short));
    memset(mb_mapping->tab_input_registers, 0, mb_mapping->nb_input_registers * sizeof(unsigned short));

    while (1)
    {
        printf("new tcp: %s\n", modbus_strerror(errno));
        ui8ModbusTcpRestart = 0;
        mb_mapping -> message_sourse = MODBUS_MESSAGE_SOURCE_TCP;
        ctx = modbus_new_tcp(pxSlaveTcpCtx,
                             "127.0.0.1",
                             CSettingsRW::ui16ModbusTcpPort);//502);//
        ctx->led_port = PRD_1_PIN_PORT; // свяжем светодиод с интерфейсом.
        ctx->led_pin = PRD_1_PIN;
        modbus_set_slave(ctx, ucModbusSlaveAddress);
        header_length = modbus_get_header_length(ctx);
//    modbus_set_error_recovery(ctx, MODBUS_ERROR_RECOVERY_LINK);

//    modbus_set_debug(ctx, TRUE);

        if (mb_mapping == NULL)
        {
            fprintf(stderr, "Failed to allocate the mapping: %s\n",
                    modbus_strerror(errno));
            modbus_free(ctx);
            //return -1; //?
        }

        while (1)
        {
            socket = modbus_tcp_listen(ctx, 1);
            modbus_tcp_accept(ctx, &socket);

            for (;;)
            {
                rc = modbus_receive(ctx, query);
                if (rc == -1)
                {
//                    printf("Quit the loop receive tcp: %s\n", modbus_strerror(errno));
                    /* Connection closed by the client or error */
                    break;
                }
                rc = modbus_reply(ctx, query, rc, mb_mapping);
                if (rc == -1)
                {
//                    printf("Quit the loop reply tcp: %s\n", modbus_strerror(errno));
                    //cout << "modbus_reply error break" << endl;
                    break;
                }

                if (ui8ModbusTcpRestart)
                {
                    break;
                }
            }
            printf("Quit the loop tcp: %s\n", modbus_strerror(errno));

            if (socket != -1)
            {
                close(socket);
            }

            modbus_close(ctx);

            if (ui8ModbusTcpRestart)
            {
                break;
            }
        }
    }
}
#endif // MODBUS_TCP_ENABLE

//-----------------------------------------------------------------------------------------------------
// поток HmiModbusRtu, связь с панелью HMI, RS-485. //com5 slave address 1
#ifdef MODBUS_HMI_ENABLE
void ModbusHmiActionHandler(int signo, siginfo_t *info, void *context)
{
    ui8ModbusHmiRestart = 1;
    cout << "action ModbusHmi" << endl;
}

void *thread_vHmiModbusRtuHandler(void *value)
{
    int socket;
    modbus_t *ctx;
    modbus_mapping_t *mb_mapping;
    int rc;
    int header_length;
    unsigned char query[MODBUS_TCP_MAX_ADU_LENGTH * 2];

    memset((unsigned char*)&xSlaveHmiRtuCtx, 0, sizeof(modbus_t));
    pxSlaveHmiRtuCtx = &xSlaveHmiRtuCtx;
    memset((unsigned char*)&xHmiRtuModbusMapping, 0, sizeof(modbus_mapping_t));
    mb_mapping = &xHmiRtuModbusMapping;
    pxHmiRtuModbusMapping = &xHmiRtuModbusMapping;

    mb_mapping->tab_bits = (unsigned char*)aucHmiRtuCoilsArray;
    mb_mapping->tab_input_bits = (unsigned char*)aucHmiRtuDiscreteInputsArray;
    mb_mapping->tab_registers = (unsigned short*)aucHmiRtuHoldingRegistersArray;
    mb_mapping->tab_input_registers = (unsigned short*)aucHmiRtuInputRegistersArray;
    mb_mapping->tab_auxiliary = (unsigned char*)aucHmiAuxiliaryArray;

    mb_mapping->nb_bits = COILS_WORK_ARRAY_LENGTH;
    mb_mapping->nb_input_bits = DISCRETE_INPUTS_ARRAY_LENGTH;
    mb_mapping->nb_registers = (HOLDING_REGISTERS_ARRAY_LENGTH / sizeof(short));
    mb_mapping->nb_input_registers = (INPUT_REGISTERS_ARRAY_LENGTH / sizeof(short));

    memset(mb_mapping->tab_bits, 0, mb_mapping->nb_bits * sizeof(unsigned char));
    memset(mb_mapping->tab_input_bits, 0, mb_mapping->nb_input_bits * sizeof(unsigned char));
    memset(mb_mapping->tab_registers, 0, mb_mapping->nb_registers * sizeof(unsigned short));
    memset(mb_mapping->tab_input_registers, 0, mb_mapping->nb_input_registers * sizeof(unsigned short));

    while (1)
    {
        ui8ModbusHmiRestart = 0;
        mb_mapping -> message_sourse = MODBUS_MESSAGE_SOURCE_RTU_HMI;
        ctx = modbus_new_rtu(pxSlaveHmiRtuCtx, "/dev/ttyO5", HMI_MODBUS_BAUD_RATE, 'N', 8, 2);
        ctx->led_port = PRD_3_PIN_PORT; // свяжем светодиод с интерфейсом.
        ctx->led_pin = PRD_3_PIN;
        modbus_set_slave(ctx, HMI_SLAVE_ADDRESS);
        header_length = modbus_get_header_length(ctx);
//    modbus_set_error_recovery(ctx, MODBUS_ERROR_RECOVERY_LINK);

//    modbus_set_debug(ctx, TRUE);

        if (mb_mapping == NULL)
        {
            fprintf(stderr, "Failed to allocate the mapping: %s\n",
                    modbus_strerror(errno));
            modbus_free(ctx);
            //return -1; //?
        }

        while (1)
        {
            rc = modbus_connect(ctx);
            if (rc == -1)
            {
                fprintf(stderr, "Unable to connect %s\n", modbus_strerror(errno));
                modbus_free(ctx);
//            break;
                //return -1; //?
            }

            rc = modbus_rtu_set_serial_mode(ctx, MODBUS_RTU_RS485);
            if (rc == -1)
            {
                fprintf(stderr, "Unable to set serial mode %s\n", modbus_strerror(errno));
                modbus_free(ctx);
//            break;
                //return -1; //?
            }

            for (;;)
            {
                rc = modbus_receive(ctx, query);
                if (rc == -1)
                {
                    //printf("Quit the loop receive hmi: %s\n", modbus_strerror(errno));
                    /* Connection closed by the client or error */
                    break;
                }

                rc = modbus_reply(ctx, query, rc, mb_mapping);
                if (rc == -1)
                {
                    printf("Quit the loop reply hmi: %s\n", modbus_strerror(errno));
                    break;
                }

                if (ui8ModbusHmiRestart)
                {
                    break;
                }
            }
            modbus_close(ctx);

            if (ui8ModbusHmiRestart)
            {
                break;
            }
        }
    }
}

#endif // MODBUS_HMI_ENABLE

//-----------------------------------------------------------------------------------------------------
// поток ModbusRtu, связь с верхним уровнем, RS-485. //com6
#ifdef MODBUS_RTU_ENABLE
void ModbusRtuActionHandler(int signo, siginfo_t *info, void *context)
{
    ui8ModbusRtuRestart = 1;
    cout << "action ModbusRtu" << endl;
}

void *thread_vModbusRtuHandler(void *value)
{
    int socket;
    modbus_t *ctx;
    modbus_mapping_t *mb_mapping;
    int rc;
    int header_length;
    unsigned char query[MODBUS_RTU_MAX_ADU_LENGTH * 2];

    memset((unsigned char*)&xSlaveRtuCtx, 0, sizeof(modbus_t));
    pxSlaveRtuCtx = &xSlaveRtuCtx;
    memset((unsigned char*)&xRtuModbusMapping, 0, sizeof(modbus_mapping_t));
    mb_mapping = &xRtuModbusMapping;
    pxRtuModbusMapping = &xRtuModbusMapping;

    mb_mapping->tab_bits = (unsigned char*)aucRtuCoilsArray;
    mb_mapping->tab_input_bits = (unsigned char*)aucRtuDiscreteInputsArray;
    mb_mapping->tab_registers = (unsigned short*)aucRtuHoldingRegistersArray;
    mb_mapping->tab_input_registers = (unsigned short*)aucRtuInputRegistersArray;
    mb_mapping->tab_auxiliary = (unsigned char*)aucRtuAuxiliaryArray;

    mb_mapping->nb_bits = COILS_WORK_ARRAY_LENGTH;
    mb_mapping->nb_input_bits = DISCRETE_INPUTS_ARRAY_LENGTH;
    mb_mapping->nb_registers = (HOLDING_REGISTERS_ARRAY_LENGTH / sizeof(short));
    mb_mapping->nb_input_registers = (INPUT_REGISTERS_ARRAY_LENGTH / sizeof(short));

    memset(mb_mapping->tab_bits, 0, mb_mapping->nb_bits * sizeof(unsigned char));
    memset(mb_mapping->tab_input_bits, 0, mb_mapping->nb_input_bits * sizeof(unsigned char));
    memset(mb_mapping->tab_registers, 0, mb_mapping->nb_registers * sizeof(unsigned short));
    memset(mb_mapping->tab_input_registers, 0, mb_mapping->nb_input_registers * sizeof(unsigned short));

    while (1)
    {
        printf("new rtu: %s\n", modbus_strerror(errno));
        ui8ModbusRtuRestart = 0;
        mb_mapping -> message_sourse = MODBUS_MESSAGE_SOURCE_RTU_HIGH_LEVEL;
        ctx = modbus_new_rtu(pxSlaveRtuCtx,
                             "/dev/ttyO1",
                             CSettingsRW::ui32ModbusRtuBaudRate,//CSettingsRW::xTPlcSettingsPackOne.xTRs485HighLevelSettingsPackOne.ui8BaudRate
                             CSettingsRW::cModbusRtuParity,
                             CSettingsRW::ui8ModbusRtuDataBits,
                             CSettingsRW::ui8ModbusRtuStopBits);//CSettingsRW::xTPlcSettingsPackOne.xTRs485HighLevelSettingsPackOne.ui8BaudRate
        ctx->led_port = PRD_1_PIN_PORT; // свяжем светодиод с интерфейсом.
        ctx->led_pin = PRD_1_PIN;
        modbus_set_slave(ctx, ucModbusSlaveAddress);
        header_length = modbus_get_header_length(ctx);
//    modbus_set_error_recovery(ctx, MODBUS_ERROR_RECOVERY_LINK);

//    modbus_set_debug(ctx, TRUE);

        if (mb_mapping == NULL)
        {
            fprintf(stderr, "Failed to allocate the mapping: %s\n",
                    modbus_strerror(errno));
            modbus_free(ctx);
            //return -1; //?
        }

        while (1)
        {

            rc = modbus_connect(ctx);
            if (rc == -1)
            {
                fprintf(stderr, "Unable to connect %s\n", modbus_strerror(errno));
                modbus_free(ctx);
//            break;
                //return -1; //?
            }

            rc = modbus_rtu_set_serial_mode(ctx, MODBUS_RTU_RS485);
            if (rc == -1)
            {
                fprintf(stderr, "Unable to set serial mode %s\n", modbus_strerror(errno));
                modbus_free(ctx);
//            break;
                //return -1; //?
            }

            for (;;)
            {
                rc = modbus_receive(ctx, query);
                if (rc == -1)
                {
//                    printf("Quit the loop receive rtu: %s\n", modbus_strerror(errno));
                    /* Connection closed by the client or error */
                    break;
                }

                rc = modbus_reply(ctx, query, rc, mb_mapping);
                if (rc == -1)
                {
//                    printf("Quit the loop reply rtu: %s\n", modbus_strerror(errno));
                    break;
                }

                if (ui8ModbusRtuRestart)
                {
                    break;
                }
            }
            modbus_close(ctx);

            if (ui8ModbusRtuRestart)
            {
                break;
            }
        }
    }
}
#endif // MODBUS_RTU_ENABLE

//-----------------------------------------------------------------------------------------------------
#ifdef MODBUS_MASTER_TCP_ENABLE
// ModbusRtu, связь с локальной сетью нижнего уровня, Ethernet.
int iModbusTcpMasterSet(void)
{
    int socket;
    modbus_t *ctx;
    modbus_mapping_t *mb_mapping;
    int rc;
    int header_length;
    unsigned char query[MODBUS_TCP_MAX_ADU_LENGTH * 2];

    memset((unsigned char*)&xMasterTcpCtx, 0, sizeof(modbus_t));
    pxMasterTcpCtx = &xMasterTcpCtx;
//    memset((unsigned char*)&xMasterTcpModbusMapping, 0, sizeof(modbus_mapping_t));
//    mb_mapping = &xMasterTcpModbusMapping;
//    pxMasterTcpModbusMapping = &xMasterTcpModbusMapping;

//    mb_mapping->tab_bits = (unsigned char*)aucTcpCoilsArray;
//    mb_mapping->tab_input_bits = (unsigned char*)aucTcpDiscreteInputsArray;
//    mb_mapping->tab_registers = (unsigned short*)aucTcpHoldingRegistersArray;
//    mb_mapping->tab_input_registers = (unsigned short*)aucTcpInputRegistersArray;
//    mb_mapping->tab_auxiliary = (unsigned char*)aucTcpAuxiliaryArray;
//
//    mb_mapping->nb_bits = COILS_WORK_ARRAY_LENGTH;
//    mb_mapping->nb_input_bits = DISCRETE_INPUTS_ARRAY_LENGTH;
//    mb_mapping->nb_registers = (HOLDING_REGISTERS_ARRAY_LENGTH / sizeof(short));
//    mb_mapping->nb_input_registers = (INPUT_REGISTERS_ARRAY_LENGTH / sizeof(short));
//
//    memset(mb_mapping->tab_bits, 0, mb_mapping->nb_bits * sizeof(unsigned char));
//    memset(mb_mapping->tab_input_bits, 0, mb_mapping->nb_input_bits * sizeof(unsigned char));
//    memset(mb_mapping->tab_registers, 0, mb_mapping->nb_registers * sizeof(unsigned short));
//    memset(mb_mapping->tab_input_registers, 0, mb_mapping->nb_input_registers * sizeof(unsigned short));

//    mb_mapping -> message_sourse = MODBUS_MESSAGE_SOURCE_TCP_MASTER;





//ctx = modbus_new_tcp_rtu(pxMasterTcpCtx, "192.168.0.5", 502); //test tcp
    ctx = modbus_new_tcp(pxMasterTcpCtx, "192.168.0.5", 503);
    if (ctx == NULL)
    {
        fprintf(stderr, "Unable to allocate libmodbus context\n");
        return -1;
    }

    ctx->led_port = PRD_2_PIN_PORT; // свяжем светодиод с интерфейсом.
    ctx->led_pin = PRD_2_PIN;

//    modbus_set_slave(ctx, SERVER_ID);
//    modbus_set_error_recovery(ctx, MODBUS_ERROR_RECOVERY_LINK);


    if (modbus_connect(ctx) == -1)
    {
        fprintf(stderr, "Connection failed: %s\n",
                modbus_strerror(errno));
//        modbus_free(ctx);
        return -1;
    }
//
//
//    /* Close the connection */
//    modbus_close(ctx);
////            //modbus_free(ctx);
    return 0;
}

#endif // MODBUS_MASTER_TCP_ENABLE

//-----------------------------------------------------------------------------------------------------
#ifdef MODBUS_MASTER_RTU_ENABLE
// ModbusRtu, связь с локальной сетью нижнего уровня, RS-485.
int iModbusRtuMasterSet(void)
{
    int socket;
    modbus_t *ctx;
    modbus_mapping_t *mb_mapping;
    int rc;
    int header_length;
    unsigned char query[MODBUS_TCP_MAX_ADU_LENGTH * 2];

    memset((unsigned char*)&xMasterRtuCtx, 0, sizeof(modbus_t));
    pxMasterRtuCtx = &xMasterRtuCtx;
//    memset((unsigned char*)&xMasterRtuModbusMapping, 0, sizeof(modbus_mapping_t));
//    mb_mapping = &xMasterRtuModbusMapping;
//    pxMasterRtuModbusMapping = &xMasterRtuModbusMapping;

//    mb_mapping->tab_bits = (unsigned char*)aucTcpCoilsArray;
//    mb_mapping->tab_input_bits = (unsigned char*)aucTcpDiscreteInputsArray;
//    mb_mapping->tab_registers = (unsigned short*)aucTcpHoldingRegistersArray;
//    mb_mapping->tab_input_registers = (unsigned short*)aucTcpInputRegistersArray;
//    mb_mapping->tab_auxiliary = (unsigned char*)aucTcpAuxiliaryArray;
//
//    mb_mapping->nb_bits = COILS_WORK_ARRAY_LENGTH;
//    mb_mapping->nb_input_bits = DISCRETE_INPUTS_ARRAY_LENGTH;
//    mb_mapping->nb_registers = (HOLDING_REGISTERS_ARRAY_LENGTH / sizeof(short));
//    mb_mapping->nb_input_registers = (INPUT_REGISTERS_ARRAY_LENGTH / sizeof(short));
//
//    memset(mb_mapping->tab_bits, 0, mb_mapping->nb_bits * sizeof(unsigned char));
//    memset(mb_mapping->tab_input_bits, 0, mb_mapping->nb_input_bits * sizeof(unsigned char));
//    memset(mb_mapping->tab_registers, 0, mb_mapping->nb_registers * sizeof(unsigned short));
//    memset(mb_mapping->tab_input_registers, 0, mb_mapping->nb_input_registers * sizeof(unsigned short));

//    mb_mapping -> message_sourse = MODBUS_MESSAGE_SOURCE_RTU_MASTER;

//    ctx = modbus_new_rtu(pxMasterRtuCtx, "/dev/ttyO3", 9600, 'N', 8, 2);
    ctx = modbus_new_rtu(pxMasterRtuCtx,
                         "/dev/ttyO3",
                         CSettingsRW::ui32ModbusRtuMasterBaudRate,
                         CSettingsRW::cModbusRtuMasterParity,
                         CSettingsRW::ui8ModbusRtuMasterDataBits,
                         CSettingsRW::ui8ModbusRtuMasterStopBits);

    if (ctx == NULL)
    {
        fprintf(stderr, "Unable to allocate libmodbus context\n");
        return -1;
    }

    ctx->led_port = PRD_2_PIN_PORT; // свяжем светодиод с интерфейсом.
    ctx->led_pin = PRD_2_PIN;

    modbus_set_slave(ctx, SERVER_ID);
//    modbus_set_error_recovery(ctx, MODBUS_ERROR_RECOVERY_LINK);

    if (modbus_connect(ctx) == -1)
    {
        fprintf(stderr, "Connection failed: %s\n",
                modbus_strerror(errno));
        modbus_free(ctx);
        //return -1;
    }

    rc = modbus_rtu_set_serial_mode(ctx, MODBUS_RTU_RS485);
    if (rc == -1)
    {
        fprintf(stderr, "Unable to set serial mode %s\n", modbus_strerror(errno));
        modbus_free(ctx);
        //return -1; //?
    }

    return 0;
}

int iModbusRtuMasterReset(void)
{
    cout << "iModbusRtuMasterReset ok" << endl;
    modbus_t *ctx;
    int rc;

    modbus_close(pxMasterRtuCtx);

    ctx = modbus_new_rtu(pxMasterRtuCtx,
                         "/dev/ttyO3",
                         CSettingsRW::ui32ModbusRtuMasterBaudRate,
                         CSettingsRW::cModbusRtuMasterParity,
                         CSettingsRW::ui8ModbusRtuMasterDataBits,
                         CSettingsRW::ui8ModbusRtuMasterStopBits);

    if (ctx == NULL)
    {
        fprintf(stderr, "Unable to allocate libmodbus context\n");
        return -1;
    }

    ctx->led_port = PRD_2_PIN_PORT; // свяжем светодиод с интерфейсом.
    ctx->led_pin = PRD_2_PIN;

    modbus_set_slave(ctx, SERVER_ID);
//    modbus_set_error_recovery(ctx, MODBUS_ERROR_RECOVERY_LINK);

    if (modbus_connect(ctx) == -1)
    {
        fprintf(stderr, "Connection failed: %s\n",
                modbus_strerror(errno));
        modbus_free(ctx);
        //return -1;
    }

    rc = modbus_rtu_set_serial_mode(ctx, MODBUS_RTU_RS485);
    if (rc == -1)
    {
        fprintf(stderr, "Unable to set serial mode %s\n", modbus_strerror(errno));
        modbus_free(ctx);
        //return -1; //?
    }

    return 0;
}

#endif // MODBUS_MASTER_RTU_ENABLE


//-----------------------------------------------------------------------------------------------------
// FRAM
// читает  данные из FRAM.
int iFramRead(unsigned char* pucDestination,
              unsigned long ulSource,
              unsigned int nuiLength)
{
    FILE *pxFile;
    char *pxFileName;

    pxFileName = SPI_MRAM_DEV;
    pxFile = fopen(pxFileName,"r+");
    if(pxFile == NULL)
    {
        printf("Cannot open ModuleBad file.\n");
        pxFile = fopen(pxFileName,"w");
        if(pxFile == NULL)
        {
            printf("Cannot create ModuleBad file.\n");
            return -1;
        }
    }

    fseek (pxFile, ulSource, SEEK_SET);
    fread(pucDestination,
          nuiLength,
          1,
          pxFile);

    fclose(pxFile);
    return 0;
}

//-----------------------------------------------------------------------------------------------------
// записывает данные в FRAM.
int iFramWrite(unsigned long ulDestination,
               unsigned char* pucSource,
               unsigned int nuiLength)
{
    FILE *pxFile;
    char *pxFileName;

    pxFileName = SPI_MRAM_DEV;
    pxFile = fopen(pxFileName,"r+");
    if(pxFile == NULL)
    {
        printf("Cannot open ModuleBad file.\n");
        pxFile = fopen(pxFileName,"w");
        if(pxFile == NULL)
        {
            printf("Cannot create ModuleBad file.\n");
            return -1;
        }
    }

    fseek (pxFile, ulDestination, SEEK_SET);
    fwrite(pucSource,
           nuiLength,
           1,
           pxFile);

    fclose(pxFile);
    return 0;
}

//-----------------------------------------------------------------------------------------------------
// проверка файлов базы данных.
int iPlcDataBaseCheck(void)
{
    unsigned char ucResult;
    char *pxFileName;
    char *pxReservFileName;
    FILE *pxDataBaseFile;
    FILE *pxReservDataBaseFile;
    int i;

// DataBase Check
    ucResult = 0;
    fbPlcDataBaseFileError = 0;
    // откроем файл базы данных.
    pxFileName = DATA_BASE_FILE_NAME;
    pxDataBaseFile = fopen(pxFileName,"r+");
    if(pxDataBaseFile == NULL)
    {
        // файл не существует.
        fbPlcDataBaseFileError = 1;
        ucResult = -1;
        printf("Cannot open Data base file.\n");
        // создадим файл.
        pxDataBaseFile = fopen(pxFileName,"w+");
        if(pxDataBaseFile == NULL)
        {
            printf("Cannot create Data base file.\n");
            return -1;
        }
    }
    else
    {
        ucResult = 0;
        fbPlcDataBaseFileError = 0;
    }

    // откроем резервный файл базы данных.
    pxReservFileName = RESERV_DATA_BASE_FILE_NAME;
    pxReservDataBaseFile = fopen(pxReservFileName,"r+");
    if(pxReservDataBaseFile == NULL)
    {
        // файл не существует.
        fbPlcDataBaseFileError = 1;
        ucResult = -1;
        printf("Cannot open Reserv Data base file.\n");
        // создадим файл.
        pxReservDataBaseFile = fopen(pxReservFileName,"w+");
        if(pxReservDataBaseFile == NULL)
        {
            printf("Cannot create Reserv Data base file.\n");
            return -1;
        }
    }
    else
    {
        ucResult = 0;
        fbPlcDataBaseFileError = 0;
    }

    // проверим целостность файла базы данных.
    // установим смещение в файле базы данных.
    fseek (pxDataBaseFile, 0, SEEK_SET);
    // скопируем содержимое файла базы данных в RAM.
    fread((void*)&xPlcDataBase,
          sizeof(struct TPlcDataBase),
          1,
          pxDataBaseFile);
    // файл базы данных повреждён?
    if ((usCrc16((unsigned char*)xPlcDataBase.axPlcDataBaseBlocks,
                 (sizeof(xPlcDataBase.axPlcDataBaseBlocks))) != xPlcDataBase.usCrc))
    {
        // файл повреждён.
        ucResult = -1;
        fbPlcDataBaseFileError = 1;
        printf("DataBase CRC wrong.\n");

        // установим смещение в резервном файле базы данных.
        fseek (pxReservDataBaseFile, 0, SEEK_SET);
        // скопируем содержимое резервного файла базы данных в RAM.
        fread((void*)&xPlcDataBase,
              sizeof(struct TPlcDataBase),
              1,
              pxReservDataBaseFile);
        // резервный файл повреждён?
        if ((usCrc16((unsigned char*)xPlcDataBase.axPlcDataBaseBlocks,
                     (sizeof(xPlcDataBase.axPlcDataBaseBlocks))) != xPlcDataBase.usCrc))
        {
            // файл повреждён.
            ucResult = -1;
            fbPlcDataBaseFileError = 1;
            printf("ReservDataBase CRC wrong.\n");
        }
        else
        {
            // файл не повреждён.
            ucResult = 0;
            fbPlcDataBaseFileError = 0;
            printf("ReservDataBase CRC ok.\n");

            // установим смещение в файле базы данных.
            fseek (pxDataBaseFile, 0, SEEK_SET);
            // восстановим повреждённый файл базы данных.
            fwrite((const void*)&(xPlcDataBase),
                   sizeof(struct TPlcDataBase),
                   1,
                   pxDataBaseFile);
        }
    }
    else
    {
        // файл не повреждён.
        ucResult = 0;
        fbPlcDataBaseFileError = 0;
        printf("DataBase CRC ok.\n");
    }

    // проверим целостность резервного файла базы данных.
    // установим смещение в резервном файле базы данных.
    fseek (pxReservDataBaseFile, 0, SEEK_SET);
    // скопируем содержимое резервного файла базы данных в RAM.
    fread((void*)&xPlcDataBase,
          sizeof(struct TPlcDataBase),
          1,
          pxReservDataBaseFile);
    // файл повреждён?
    if ((usCrc16((unsigned char*)xPlcDataBase.axPlcDataBaseBlocks,
                 (sizeof(xPlcDataBase.axPlcDataBaseBlocks))) != xPlcDataBase.usCrc))
    {
        // файл повреждён.
        ucResult = -1;
        fbPlcDataBaseFileError = 1;
        printf("ReservDataBase CRC wrong.\n");

        // установим смещение в файле.
        fseek (pxDataBaseFile, 0, SEEK_SET);
        // скопируем содержимое файла в RAM.
        fread((void*)&xPlcDataBase,
              sizeof(struct TPlcDataBase),
              1,
              pxDataBaseFile);
        // резервный файл повреждён?
        if ((usCrc16((unsigned char*)xPlcDataBase.axPlcDataBaseBlocks,
                     (sizeof(xPlcDataBase.axPlcDataBaseBlocks))) != xPlcDataBase.usCrc))
        {
            // файл повреждён.
            ucResult = -1;
            fbPlcDataBaseFileError = 1;
            printf("DataBase CRC wrong.\n");
        }
        else
        {
            // файл не повреждён.
            ucResult = 0;
            fbPlcDataBaseFileError = 0;
            printf("DataBase CRC ok.\n");

            // установим смещение в файле базы данных.
            fseek (pxReservDataBaseFile, 0, SEEK_SET);
            // восстановим повреждённый резервный файл базы данных.
            fwrite((const void*)&(xPlcDataBase),
                   sizeof(struct TPlcDataBase),
                   1,
                   pxReservDataBaseFile);
        }
    }
    else
    {
        // файл не повреждён.
        ucResult = 0;
        fbPlcDataBaseFileError = 0;
        printf("ReservDataBase CRC ok.\n");
    }

    // закроем файл.
    fclose(pxDataBaseFile);
    fclose(pxReservDataBaseFile);
    //    malloc(1000);//? если убрать fclose портит объект mb_mapping.

    // заполним массив управления базой данных размерами блоков.
    for (i = 0; i < PLC_DATA_BASE_BLOCK_QUANTITY; i++)
    {
        xPlcDataBaseControl.axPlcDataBaseBlockControl[i].usBlockLength =
            (unsigned int)aucDataBaseBlockLength[i];
    }

    return ucResult;
}

//-----------------------------------------------------------------------------------------------------
// проверка файлов конфигурации.
int iPlcConfigurationCheck(void)
{
    unsigned char ucResult;
    FILE *pxConfigFile;
    FILE *pxReservConfigFile;
    char *pxFileName;
    char *pxReservFileName;

// ConfigFile check
    ucResult = 0;
    fbPlcConfigurationFileError = 0;

    pxFileName = CONFIG_FILE_NAME;
    pxConfigFile = fopen(pxFileName,"r+");
    if(pxConfigFile == NULL)
    {
        ucResult = -1;
        fbPlcConfigurationFileError = 1;
        printf("Cannot open Config file.\n");
        pxConfigFile = fopen(pxFileName,"w+");
        if(pxConfigFile == NULL)
        {
            printf("Cannot create Config file.\n");
            return -1;
        }
    }
    else
    {
        ucResult = 0;
        fbPlcConfigurationFileError = 0;
    }

    pxReservFileName = RESERV_CONFIG_FILE_NAME;
    pxReservConfigFile = fopen(pxReservFileName,"r+");
    if(pxReservConfigFile == NULL)
    {
        ucResult = -1;
        fbPlcConfigurationFileError = 1;
        printf("Cannot open Reserv Config file.\n");
        pxReservConfigFile = fopen(pxReservFileName,"w+");
        if(pxReservConfigFile == NULL)
        {
            printf("Cannot create Reserv Config file.\n");
            return -1;
        }
    }
    else
    {
        ucResult = 0;
        fbPlcConfigurationFileError = 0;
    }

    // проверим целостность файла конфигурации.
    fseek (pxConfigFile, 0, SEEK_SET);
    fread((void*)&xPlcConfigData,
          sizeof(struct TPlcConfigData),
          1,
          pxConfigFile);

    if ((usCrc16((unsigned char*)&xPlcConfigData.xConfigData,
                 sizeof(struct TConfigData))) != xPlcConfigData.usCrc)
    {
        ucResult = -1;
        fbPlcConfigurationFileError = 1;
        printf("PlcConfigData CRC wrong\n");

        fseek (pxReservConfigFile, 0, SEEK_SET);
        fread((void*)&xPlcConfigData,
              sizeof(struct TPlcConfigData),
              1,
              pxReservConfigFile);

        if ((usCrc16((unsigned char*)&xPlcConfigData.xConfigData,
                     sizeof(struct TConfigData))) != xPlcConfigData.usCrc)
        {
            ucResult = -1;
            fbPlcConfigurationFileError = 1;
            printf("ReservPlcConfigData CRC wrong\n");
        }
        else
        {
            ucResult = 0;
            fbPlcConfigurationFileError = 0;
            printf("ReservPlcConfigData CRC ok\n");
            fseek (pxConfigFile, 0, SEEK_SET);
            fwrite((const void*)&xPlcConfigData,
                   sizeof(struct TPlcConfigData),
                   1,
                   pxConfigFile);

        }
    }
    else
    {
        ucResult = 0;
        fbPlcConfigurationFileError = 0;
        printf("PlcConfigData CRC ok\n");
    }


    // проверим целостность резервного файла конфигурации.
    fseek (pxReservConfigFile, 0, SEEK_SET);
    fread((void*)&xPlcConfigData,
          sizeof(struct TPlcConfigData),
          1,
          pxReservConfigFile);

    if ((usCrc16((unsigned char*)&xPlcConfigData.xConfigData,
                 sizeof(struct TConfigData))) != xPlcConfigData.usCrc)
    {
        ucResult = -1;
        fbPlcConfigurationFileError = 1;
        printf("ReservPlcConfigData CRC wrong\n");

        fseek (pxConfigFile, 0, SEEK_SET);
        fread((void*)&xPlcConfigData,
              sizeof(struct TPlcConfigData),
              1,
              pxConfigFile);

        if ((usCrc16((unsigned char*)&xPlcConfigData.xConfigData,
                     sizeof(struct TConfigData))) != xPlcConfigData.usCrc)
        {
            ucResult = -1;
            fbPlcConfigurationFileError = 1;
            printf("PlcConfigData CRC wrong\n");
        }
        else
        {
            ucResult = 0;
            fbPlcConfigurationFileError = 0;
            printf("PlcConfigData CRC ok\n");
            fseek (pxReservConfigFile, 0, SEEK_SET);
            fwrite((const void*)&xPlcConfigData,
                   sizeof(struct TPlcConfigData),
                   1,
                   pxReservConfigFile);

        }
    }
    else
    {
        ucResult = 0;
        fbPlcConfigurationFileError = 0;
        printf("ReservPlcConfigData CRC ok\n");
    }

    fclose(pxConfigFile);
    fclose(pxReservConfigFile);
//    malloc(1000);//? если убрать fclose портит объект mb_mapping.

    return ucResult;
}

//-----------------------------------------------------------------------------------------------------
// поиск модулей на SPI.
int iPlcModulesSearch(void)
{
    unsigned char ucSlaveAddress;
    unsigned char ucSequenceCheck;
    unsigned char nucIndexNumber;
    int i;
    unsigned char auiSpiTxBuffer[TX_RX_BUFF_SIZE];
    unsigned char auiSpiRxBuffer[TX_RX_BUFF_SIZE];

    memset(&xPlcConfigSearch,
           0,
           sizeof(struct TPlcConfigSearch));
    memset(&xPlcConfigSearchTemp,
           0,
           sizeof(struct TPlcConfigSearch));
    memset(&xPlcConfigDataTemp,
           0,
           sizeof(struct TPlcConfigData));

    ucSlaveAddress = 0;
    nucIndexNumber = 0;


//    int input = 0;
//    int reverce = 0;
//// опросим интерфейс SPI. какие модули присутствуют?
//    for (int i = 0;
//            i < SPI_CHIP_SELECT_MAX_ADDRESS;
//            i++)
//    {
//        input = i;
//        if (input & 0x01)
//        {
//            reverce |= 0x08;
//        }
//        else
//        {
//
//        }
//        input >>= 1;
//
//        if (input & 0x01)
//        {
//            reverce |= 0x04;
//        }
//        else
//        {
//
//        }
//        input >>= 1;
//
//        if (input & 0x01)
//        {
//            reverce |= 0x02;
//        }
//        else
//        {
//
//        }
//        input >>= 1;
//
//        if (input & 0x01)
//        {
//            reverce |= 0x01;
//        }
//        else
//        {
//
//        }
//
//        printf("iPlcModulesSearch reverce ucSlaveAddress %d\n\r", (reverce ^ 15));
//        reverce = 0;
//    }

//    while (1)
//    {
    // опросим интерфейс SPI. какие модули присутствуют?
    for (ucSlaveAddress = 0;
            ucSlaveAddress < SPI_CHIP_SELECT_MAX_ADDRESS;
            ucSlaveAddress++)
//        for (ucSlaveAddress = 0;
//                ucSlaveAddress < 5;
//                ucSlaveAddress++)
    {
        // если модуль не ответил MODULE_REQUEST_QUANTITY раз,
        // то его нет или он неисправен.
        for (i = 0; i < MODULE_REQUEST_QUANTITY; i++)
//            for (i = 0; i < 1; i++)
        {
            usleep(10000);
            auiSpiTxBuffer[0] = GET_MODULE_TYPE_COMMAND;
//            CCommunicationDeviceInterface* pxCommunicationDevice;
//            CCommunicationDevice* pxCommunicationDevice;
//            pxCommunicationDevice = &xSpiCommunicationDevice;
//            pxCommunicationDevice -> Exchange(ucSlaveAddress,
//                                             auiSpiTxBuffer,
//                                             auiSpiRxBuffer,
//                                             SPI_PREAMBLE_LENGTH +
//                                             GET_MODULE_TYPE_COMMAND_ANSWER_LENGTH +
//                                             ONE_BYTE_CRC_LENGTH,
//                                             LOW_SPEED_IN_HZ);
            xSpiCommunicationDevice.Exchange(ucSlaveAddress,
                                             auiSpiTxBuffer,
                                             auiSpiRxBuffer,
                                             SPI_PREAMBLE_LENGTH +
                                             GET_MODULE_TYPE_COMMAND_ANSWER_LENGTH +
                                             ONE_BYTE_CRC_LENGTH,
                                             LOW_SPEED_IN_HZ);

            // модуль отвечает?
            if (auiSpiRxBuffer[GET_MODULE_TYPE_COMMAND_OFFSET] ==
                    GET_MODULE_TYPE_COMMAND) // if echo answer is ok.
            {

//                    cout << "GET_MODULE_TYPE_COMMAND " << endl;
//                    unsigned char *pucSourceTemp;
//                    pucSourceTemp = (unsigned char*)auiSpiRxBuffer;
//                    for(int i=0; i<16; )
//                    {
//                        for(int j=0; j<8; j++)
//                        {
//                            cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
//                        }
//                        cout << endl;
//                        i += 8;
//                    }

                // обмен данными прошёл без ошибок?
                if ((iCrcSummOneByteCompare(&auiSpiRxBuffer[GET_MODULE_TYPE_COMMAND_OFFSET],
                                            GET_MODULE_TYPE_COMMAND_LENGTH +
                                            GET_MODULE_TYPE_COMMAND_ANSWER_LENGTH) > 0))
                {
                    //                    // модуль присутствует. увеличим значение переменной -
                    //                    // фактическое количество модулей в системе.
                    //                    (xPlcConfigDataTemp.xConfigData.ucModulesQuantity)++;
                    //                    // сохраним тип модуля в массиве фактической конфигурации.
                    //                    xPlcConfigDataTemp.xConfigData.aucModuleType[ucSlaveAddress] =
                    //                        auiSpiRxBuffer[MODULE_TYPE_OFFSET];

                    // модуль присутствует. увеличим значение переменной -
                    // фактическое количество модулей в системе.
                    (xPlcConfigSearchTemp.ui8ModulesQuantity)++;
                    //                    // сохраним адрес модуля в массиве для упорядочивания следования модулей при поиске на SPI.
                    //                    xPlcConfigSearchTemp.axConfigSearch[nucIndexNumber].ui8Address =
                    //                        ucSlaveAddress;
                    //                    // сохраним тип модуля в массиве для упорядочивания следования модулей при поиске на SPI.
                    //                    xPlcConfigSearchTemp.axConfigSearch[nucIndexNumber].ui8Type =
                    //                        auiSpiRxBuffer[MODULE_TYPE_OFFSET];
//                    // сохраним адрес модуля в массиве для упорядочивания следования модулей при поиске на SPI.
//                    xPlcConfigSearchTemp.axConfigSearch[ucSlaveAddress].ui8Address =
//                        ucSlaveAddress;
                    // сохраним тип модуля в массиве для упорядочивания следования модулей при поиске на SPI.
                    xPlcConfigSearchTemp.axConfigSearch[ucSlaveAddress].ui8Type =
                        auiSpiRxBuffer[MODULE_TYPE_OFFSET];

                    //                    printf("iPlcModulesSearch nucIndexNumber %d\n\r", (nucIndexNumber));
                    //                    printf("iPlcModulesSearch SPI  bus address %d\n\r", (ucSlaveAddress));
                    //                    printf("iPlcModulesSearch ucSlaveAddress %d\n\r", (ucSlaveAddress));
                    //                    printf("iPlcModulesSearch ui8Type %d\n\r", (xPlcConfigSearchTemp.axConfigSearch[ucSlaveAddress].ui8Type));

                    //printf("vMainTaskMemoryMapFormation ui8Address %d\n\r", (xPlcConfigSearch.axConfigSearch[nucIndexNumber].ui8Address));
                    //                    // временная подстановка второго модуля МТВИ. // MTVI TempRepair.
                    //                    if (auiSpiRxBuffer[MODULE_TYPE_OFFSET] == MODULE_TYPE_MTVI5)
                    //                    {
                    //                        cout << "MODULE_TYPE_MTVI5 temp" << endl;
                    //                        // следующий модуль.
                    //                        nucIndexNumber++;
                    //                        // модуль присутствует. увеличим значение переменной -
                    //                        // фактическое количество модулей в системе.
                    //                        (xPlcConfigSearchTemp.ui8ModulesQuantity)++;
                    //                        // сохраним адрес модуля в массиве для упорядочивания следования модулей при поиске на SPI.
                    //                        xPlcConfigSearchTemp.axConfigSearch[nucIndexNumber].ui8Address =
                    //                            ucSlaveAddress;
                    //                        // сохраним тип модуля в массиве для упорядочивания следования модулей при поиске на SPI.
                    //                        xPlcConfigSearchTemp.axConfigSearch[nucIndexNumber].ui8Type =
                    //                            auiSpiRxBuffer[MODULE_TYPE_OFFSET];
                    //                    }


                    //                             // временная подстановка типа модуля MVDS9 вместо MVDI5.
                    //                    if (auiSpiRxBuffer[MODULE_TYPE_OFFSET] == MODULE_TYPE_MVDI)
                    //                    {
                    //                        cout << "MODULE_TYPE_MVST3" << endl;
                    //                        xPlcConfigDataTemp.xConfigData.aucModuleType[ucSlaveAddress] =
                    //                        MODULE_TYPE_MVDS9;
                    //                    }
                    //                    else
                    //                    {
                    //                        cout << "MODULE_TYPE_OFFSET" << endl;
                    //                        xPlcConfigDataTemp.xConfigData.aucModuleType[ucSlaveAddress] =
                    //                        auiSpiRxBuffer[MODULE_TYPE_OFFSET];
                    //                    }



//                    // временная подстановка типа модуля MVST3 вместо MVAI.
//                    if (auiSpiRxBuffer[MODULE_TYPE_OFFSET] == MODULE_TYPE_MVAI5)
//                    {
//                        cout << "MODULE_TYPE_MVST3" << endl;
//                        xPlcConfigSearchTemp.axConfigSearch[ucSlaveAddress].ui8Type =
//                            MODULE_TYPE_MVST3;
//                    }

                    //                    // следующий модуль.
                }
//                    nucIndexNumber++;
                // перейдём к опросу следующего адреса.
                break;
            }
        }
        // сохраним адрес модуля в массиве для упорядочивания следования модулей при поиске на SPI.
        xPlcConfigSearchTemp.axConfigSearch[ucSlaveAddress].ui8Address =
            ucSlaveAddress;
    }

//        // временная подстановка модуля МТВИ. // MTVI TempRepair.
//        if (ucSlaveAddress == 3)
//        {
//            // модуль присутствует. увеличим значение переменной -
//            // фактическое количество модулей в системе.
//            (xPlcConfigSearchTemp.ui8ModulesQuantity)++;
//            // сохраним адрес модуля в массиве для упорядочивания следования модулей при поиске на SPI.
//            xPlcConfigSearchTemp.axConfigSearch[ucSlaveAddress].ui8Address =
//                ucSlaveAddress;
//            // сохраним тип модуля в массиве для упорядочивания следования модулей при поиске на SPI.
//            xPlcConfigSearchTemp.axConfigSearch[ucSlaveAddress].ui8Type =
//                MODULE_TYPE_MTVI5;
//        }

//    }



    (xPlcConfigSearch.ui8ModulesQuantity) =
        (xPlcConfigSearchTemp.ui8ModulesQuantity);

    (xPlcConfigDataTemp.xConfigData.ucModulesQuantity) =
        (xPlcConfigSearch.ui8ModulesQuantity);
    for (int i = 0;
            i < (SPI_CHIP_SELECT_MAX_ADDRESS);
            i++)
    {
        (xPlcConfigSearch.axConfigSearch[i].ui8Address) =
            (xPlcConfigSearchTemp.axConfigSearch[i].ui8Address);
        (xPlcConfigSearch.axConfigSearch[i].ui8Type) =
            (xPlcConfigSearchTemp.axConfigSearch[i].ui8Type);
    }

    for (int i = 0;
            i < (SPI_CHIP_SELECT_MAX_ADDRESS);
            i++)
    {
        xPlcConfigDataTemp.xConfigData.aucModuleType[i] =
            xPlcConfigSearch.axConfigSearch[i].ui8Type;
    }

//    // упорядочим следование модулей в конфигурации.
//    // 1 группа - модули дискретного ввода, 2 группа - модули аналогового ввода,
//    // 3 группа - модули дискретного вывода, 4 группа - модули аналогового вывода.
//    ucSlaveAddress = 0;
//    ucSequenceCheck = 0;
//    nucIndexNumber = 0;
//    // пройдём по всем найденным на SPI модулям.
//    for (int i = 0;
//            i < (xPlcConfigSearch.ui8ModulesQuantity);
//            i++)
//    {
//        // найден модуль (1 группа - модули дискретного ввода)?
//        if (((xPlcConfigSearchTemp.axConfigSearch[i].ui8Type) == MODULE_TYPE_MVDI) ||
//                ((xPlcConfigSearchTemp.axConfigSearch[i].ui8Type) == MODULE_TYPE_MVDS9))
//        {
//            (xPlcConfigSearch.axConfigSearch[nucIndexNumber].ui8Address) =
//                (xPlcConfigSearchTemp.axConfigSearch[i].ui8Address);
//            (xPlcConfigSearch.axConfigSearch[nucIndexNumber].ui8Type) =
//                (xPlcConfigSearchTemp.axConfigSearch[i].ui8Type);
//            // следующий модуль.
//            nucIndexNumber++;
//        }
//    }
//    // пройдём по всем найденным на SPI модулям.
//    for (int i = 0;
//            i < (xPlcConfigSearch.ui8ModulesQuantity);
//            i++)
//    {
//        // найден модуль (2 группа - модули аналогового ввода)?
//        if (((xPlcConfigSearchTemp.axConfigSearch[i].ui8Type) == MODULE_TYPE_MVPS3) ||
//                ((xPlcConfigSearchTemp.axConfigSearch[i].ui8Type) == MODULE_TYPE_MVAI5) ||
//                ((xPlcConfigSearchTemp.axConfigSearch[i].ui8Type) == MODULE_TYPE_MVST3) ||
//                ((xPlcConfigSearchTemp.axConfigSearch[i].ui8Type) == MODULE_TYPE_MVAO3))
//        {
//            (xPlcConfigSearch.axConfigSearch[nucIndexNumber].ui8Address) =
//                (xPlcConfigSearchTemp.axConfigSearch[i].ui8Address);
//            (xPlcConfigSearch.axConfigSearch[nucIndexNumber].ui8Type) =
//                (xPlcConfigSearchTemp.axConfigSearch[i].ui8Type);
//            // следующий модуль.
//            nucIndexNumber++;
//        }
//    }
//    // пройдём по всем найденным на SPI модулям.
//    for (int i = 0;
//            i < (xPlcConfigSearch.ui8ModulesQuantity);
//            i++)
//    {
//        // найден модуль (3 группа - модули дискретного вывода)?
//        if (((xPlcConfigSearchTemp.axConfigSearch[i].ui8Type) == MODULE_TYPE_MR53) ||
//                ((xPlcConfigSearchTemp.axConfigSearch[i].ui8Type) == MODULE_TYPE_MR54) ||
//                ((xPlcConfigSearchTemp.axConfigSearch[i].ui8Type) == MODULE_TYPE_MR55) ||
//                ((xPlcConfigSearchTemp.axConfigSearch[i].ui8Type) == MODULE_TYPE_MR51))
//        {
//            (xPlcConfigSearch.axConfigSearch[nucIndexNumber].ui8Address) =
//                (xPlcConfigSearchTemp.axConfigSearch[i].ui8Address);
//            (xPlcConfigSearch.axConfigSearch[nucIndexNumber].ui8Type) =
//                (xPlcConfigSearchTemp.axConfigSearch[i].ui8Type);
//            // следующий модуль.
//            nucIndexNumber++;
//        }
//    }
//    // пройдём по всем найденным на SPI модулям.
//    for (int i = 0;
//            i < (xPlcConfigSearch.ui8ModulesQuantity);
//            i++)
//    {
//        // найден модуль (4 группа - модули аналогового вывода)?
//        if (((xPlcConfigSearchTemp.axConfigSearch[i].ui8Type) == MODULE_TYPE_MTVI5))
//        {
//            (xPlcConfigSearch.axConfigSearch[nucIndexNumber].ui8Address) =
//                (xPlcConfigSearchTemp.axConfigSearch[i].ui8Address);
//            (xPlcConfigSearch.axConfigSearch[nucIndexNumber].ui8Type) =
//                (xPlcConfigSearchTemp.axConfigSearch[i].ui8Type);
//            // следующий модуль.
//            nucIndexNumber++;
//        }
//    }
//
//    (xPlcConfigDataTemp.xConfigData.ucModulesQuantity) =
//        (xPlcConfigSearch.ui8ModulesQuantity);
//    for (int i = 0;
//            i < (xPlcConfigSearch.ui8ModulesQuantity);
//            i++)
//    {
//        xPlcConfigDataTemp.xConfigData.aucModuleType[i] =
//            xPlcConfigSearch.axConfigSearch[i].ui8Type;
//    }
    return 0;
}

//-----------------------------------------------------------------------------------------------------
uint8_t ui8SequenceSlotCheck(void)
{
//    unsigned char nucIndexNumber;
//    unsigned char ucSlaveAddress;
//    unsigned char ucSequenceCheck;
//
//    nucIndexNumber = 0;
//    ucSlaveAddress = 0;
//    ucSequenceCheck = 0;
//    // проверка порядка следования модулей.
//    // 1 группа - модули дискретного ввода, 2 группа - модули аналогового ввода,
//    // 3 группа - модули дискретного вывода, 4 группа - модули аналогового вывода.
//    for (ucSlaveAddress = 0;
//            ucSlaveAddress < INTERNAL_MODULE_QUANTITY;
//            ucSlaveAddress++)
//    {
//        switch(xPlcConfigDataTemp.
//                xConfigData.aucModuleType[ucSlaveAddress])
//
//        {
//        case MODULE_TYPE_MVDI:
//        case MODULE_TYPE_MVDS9:
//            if (ucSequenceCheck > DISCRETE_INPUT_MODULE_GROUP)
//            {
//                printf("Sequence error.\n");
//                return 1;
//            }
//            // определим группу в которую входит модуль.
//            ucSequenceCheck = DISCRETE_INPUT_MODULE_GROUP;
//            break;
//
//        case MODULE_TYPE_MVPS3:
//        case MODULE_TYPE_MVAI5:
//        case MODULE_TYPE_MVST3:
//        case MODULE_TYPE_MVAO3:
//            if (ucSequenceCheck > ANOLOGUE_INPUT_MODULE_GROUP)
//            {
//                printf("Sequence error.\n");
//                return 1;
//            }
//            // определим группу в которую входит модуль.
//            ucSequenceCheck = ANOLOGUE_INPUT_MODULE_GROUP;
//            break;
//        case MODULE_TYPE_MR53:
//        case MODULE_TYPE_MR54:
//        case MODULE_TYPE_MR55:
//        case MODULE_TYPE_MR51:
//            if (ucSequenceCheck > DISCRETE_OUTPUT_MODULE_GROUP)
//            {
//                printf("Sequence error.\n");
//                return 1;
//            }
//            // определим группу в которую входит модуль.
//            ucSequenceCheck = DISCRETE_OUTPUT_MODULE_GROUP;
//            break;
//
//        case MODULE_TYPE_MTVI5:
//            if (ucSequenceCheck > CURRENT_OUTPUT_MODULE_GROUP)
//            {
//                printf("Sequence error.\n");
//                return 1;
//            }
//            // определим группу в которую входит модуль.
//            ucSequenceCheck = CURRENT_OUTPUT_MODULE_GROUP;
//            break;
//
//        default:
//            break;
//        };
//    }
//
//    printf("Sequence ok.\n");
//    return 0;
}

//-----------------------------------------------------------------------------------------------------
uint8_t ui8EmptySlotBetwinCheck(void)
{
    // проверка отсутствия пустых слотов - "дырок".
    for (int i = 0;
            i < (xPlcConfigData.
                 xConfigData.
                 ucModulesQuantity);
            i++)
    {
        // найден пустой слот?
        if (!xPlcConfigDataTemp.
                xConfigData.aucModuleType[i])
        {
            printf("Empty slot betwin found! \n");
            return 1;
        }
        else
        {
            return 0;
        }
    }
}

//-----------------------------------------------------------------------------------------------------
uint8_t ui8EmptySlotCheck(void)
{
    uint8_t ui8FirstEmptySlotIndex = 0;
    uint8_t ui8LastNotEmptySlotIndex = 0;

    // проверка отсутствия пустых слотов.
    for (int i = 0;
            i < INTERNAL_MODULE_QUANTITY;
            i++)
    {
        // найден модуль?
        if (xPlcConfigDataTemp.
                xConfigData.aucModuleType[i])
        {
            // по последнему индексу найдено не пустое место.
            ui8LastNotEmptySlotIndex = i;
        }
        else
        {
            // найдено первое пустое место?
            if (!ui8FirstEmptySlotIndex)
            {
                // по последнему индексу найдено пустое место.
                ui8FirstEmptySlotIndex = i;
            }
        }
    }

    // есть пустое место?
    if (ui8FirstEmptySlotIndex)
    {
        // есть пустое место между найдеными модулями?
        if (ui8FirstEmptySlotIndex < ui8LastNotEmptySlotIndex)
        {
            printf("Empty slot found! \n");
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        return 0;
    }
}

//-----------------------------------------------------------------------------------------------------
void vRegisteredBadModuleConfigurationRepare(void)
{
    // восстанавливаем конфигурацию и количество модулей по зарегистрированным отказам модулей.

    // прочитаем буфер отказа модулей.
    iBadModuleBuffRead();

    for (int i = 0;
            i < (xPlcConfigData.xConfigData.ucModulesQuantity);
            i++)
    {
        // посмотрим, есть отсутствующий модуль?
        if (!(xPlcConfigDataTemp.xConfigData.aucModuleType[i]))
        {
            printf("The module is not responding.\n");
            printf("Address: %d. Type: %X.\n",
                   i,
                   xPlcConfigData.xConfigData.aucModuleType[i]);
            // модуль признан неисправным.
//                            printf("aucBadModuleStateBuff[i] %d\n\r", i);
            aucBadModuleStateBuff[i] = BAD_MODULE_NOT_RESPONDED_ON_START;
            // посмотрим, были отказы модулей до этого?
            if (aucCoilsArray[BAD_MODULES_BIT_ARRAY_OFFSET + i])
            {
                // активное состояние события ещё не зарегистрировано?
                if(xCInternalModuleErrorEvent.EventOnIsNotRegistered(
                            i,
                            MTVI5_BAD_ANSWER_ERROR_OFFSET))
                {
                    // зарегистрируем активное состояние события.
                    // ошибка обмена данными.
                    CEvents::EventRegistration(
                        (i + CONVERT_INTEGER_TO_NATURAL_NUMBER),
                        (CEvents::HANDLED_EVENTS_INTERNAL_MODULES_BAD_TYPE |
                         CEvents::HANDLED_EVENTS_IS_POPUP |
                         CEvents::HANDLED_EVENTS_IS_OCCURED_ON_START),
                        MTVI5_BAD_ANSWER_ERROR_OFFSET,
                        "Отказ зарег.");
                }

                printf("not responding registered.\n");
                printf("Address: %d. Type: %X.\n",
                       i,
                       xPlcConfigData.xConfigData.aucModuleType[i]);
                // восстановление кода модуля в буфере фактической конфигурации
                // из массива предыдущей конфигурации.
                xPlcConfigDataTemp.xConfigData.aucModuleType[i] =
                    xPlcConfigData.xConfigData.aucModuleType[i];
                // восстановление кода модуля в буфере найденных модулей.
                xPlcConfigSearch.axConfigSearch[i].ui8Type =
                    xPlcConfigDataTemp.xConfigData.aucModuleType[i];
//                    // восстановление адреса модуля в буфере найденных модулей.
//                    xPlcConfigSearch.axConfigSearch[i].ui8Address = i; // MTVI TempRepair.
                // коррекция фактического количества модулей.
                (xPlcConfigDataTemp.xConfigData.ucModulesQuantity)++;
                // конфигурация корректировалась, вычислим служебные переменные заново.
                memset(&xPlcConfigService,
                       0,
                       sizeof(struct TPlcConfigService));
            }
        }
    }

    iPlcConfigurationCreate();
    // конфигурация восстановлена.
}

//-----------------------------------------------------------------------------------------------------
void vNewConfigurationAndDataBaseCreate(void)
{
    // создаём текущую конфигурацию и новую базу данных.
    memset(&xPlcDataBase,
           0,
           sizeof(struct TPlcDataBase));
    iPlcGetModulesDataBase();
    iModuleMrXXStartDataBaseCreate();
    iDiscreteInputStartDataBaseCreate();
    iDiscreteSygnalTextDescriptorStartDataBaseCreate();
    iDimentionsParametersDataBaseCreate();
    printf("The new DataBase of discrete signals have created.\n");
    printf("DataBase don't match.\n");
    printf("DataBase restored from new modules.\n");

    vDataBaseBlockCrcCreate();
}

//-----------------------------------------------------------------------------------------------------
void vMrXXDataBaseCreate(void)
{
    iPlcGetModulesDataBase();
    iModuleMrXXStartDataBaseCreate();
    printf("The new DataBase of MrXX have created.\n");
    printf("DataBase don't match.\n");

    vDataBaseBlockCrcCreate();
}

//-----------------------------------------------------------------------------------------------------
void vPlcGetModulesDataBaseCompare(void)
{
    // база данных модуля не совпадает с базой данных прибора?
    if (iPlcGetModulesDataBase() != 0)
    {
        // база данных модуля не совпадает с базой данных прибора.
        // получим базу данных модуля.
        printf("DataBase restored from new modules.\n");
        vDataBaseBlockCrcCreate();
        iPlcDataBaseSave();
        vWorkArraysUpdate();
    }
}

//-----------------------------------------------------------------------------------------------------
uint8_t ui8ConfigurationCompare(void)
{
// конфигурация совпадает?
    if ((memcmp((const void*)&(xPlcConfigDataTemp.xConfigData.aucModuleType),
                (const void*)&xPlcConfigData.xConfigData.aucModuleType,
                (INTERNAL_MODULE_QUANTITY))) == 0)
    {
        printf("Configuration does match.\n");
        return 1;
    }
    else
    {
        printf("Configuration does not match.\n");
        return 0;
    }
}

//-----------------------------------------------------------------------------------------------------
uint8_t ui8ModuleQuantityCompare(void)
{
// количество модулей совпадает?
    if ((xPlcConfigDataTemp.xConfigData.ucModulesQuantity) ==
            xPlcConfigData.xConfigData.ucModulesQuantity)
    {
        printf("Module quantity does match.\n");
        return 1;
    }
    else
    {
        printf("Module quantity does not match.\n");
        return 0;
    }
}

//-----------------------------------------------------------------------------------------------------
uint8_t ui8DiscreteAndAnalogueModuleQuantityCompare(void)
{
    unsigned char nucIndexNumber;
    unsigned char ucSlaveAddress;
    unsigned char ucDiscreteInputModuleQuantity;
    unsigned char ucAnalogueInputModuleQuantity;

    ucDiscreteInputModuleQuantity = 0;
    ucAnalogueInputModuleQuantity = 0;

    // вычисление служебных переменных для внутренних модулей.
    for (ucSlaveAddress = 0;
            ucSlaveAddress < INTERNAL_MODULE_QUANTITY;
            ucSlaveAddress++)
    {
        switch(xPlcConfigData.
                xConfigData.aucModuleType[ucSlaveAddress])
        {
        case MODULE_TYPE_MVDI:
        case MODULE_TYPE_MVDS9:
            ucDiscreteInputModuleQuantity++;
            break;

        case MODULE_TYPE_MVPS3:
        case MODULE_TYPE_MVAI5:
        case MODULE_TYPE_MVST3:
        case MODULE_TYPE_MVAO3:
            ucAnalogueInputModuleQuantity++;
            break;

        default:
            break;
        };
    }

// количество модулей совпадает?
    if ((ucDiscreteInputModuleQuantity ==
            (xPlcConfigService.xPlcConfigServiceData.ucServiceDiscreteInputModuleQuantity)) &&
            (ucAnalogueInputModuleQuantity ==
             (xPlcConfigService.xPlcConfigServiceData.ucServiceAnalogueInputModuleQuantity)))
    {
        printf("DiscreteInput AnalogueInput Module quantity does match.\n");
        return 1;
    }
    else
    {
        printf("DiscreteInput AnalogueInput Module quantity does not match.\n");
        return 0;
    }
}

//-----------------------------------------------------------------------------------------------------
// сравнение конфигурации, выявление ошибок.
int iPlcConfigurationCompare(void)
{
    typedef enum
    {
        SEQUENCE_CHECK = 0,
        SEQUENCE_CHECK_WRONG,
        EMPTY_SLOT_CHECK,
        EMPTY_SLOT_CHECK_WRONG,
        CONFIGURATION_CHECK,
        CONFIGURATION_CHECK_OK,
        CONFIGURATION_CHECK_WRONG,
        CONFIGURATION_AND_DATA_BASE_FILE_CHECK,
        CONFIGURATION_AND_DATA_BASE_FILE_CHECK_OK,
        CONFIGURATION_AND_DATA_BASE_FILE_CHECK_WRONG,
        CONFIGURATION_CREATE,
        CONFIGURATION_CREATE_WRONG,
        UNCONDITIONAL_STOP_SIGNALING,
        STOP_SIGNALING_CONFIRMATION_WAITING,
        CONFIGURATION_OK_NORMAL_START
    };

    unsigned char nucIndexNumber;
    unsigned char ucSlaveAddress;
    unsigned char ucSequenceCheck;
    int i;
    uint8_t ui8FsmState;

    uint8_t ui8FirstEmptySlotIndex = 0;
    uint8_t ui8LastNotEmptySlotIndex = 0;

    ui8FsmState = SEQUENCE_CHECK;

    while (1)
    {
        switch(ui8FsmState)

        {
        case SEQUENCE_CHECK:
            printf("iPlcConfigurationCompare SEQUENCE_CHECK: ui8FsmState %d\n\r", (ui8FsmState));
            // нарушена последовательность групп модулей?
            if (ui8SequenceSlotCheck())
            {
                // нарушена последовательность групп модулей.
                // остановка, код ошибки "ошибка последовательности" программатору, сигнализация.
                xPlcConfigDataTransmit.xConfigData.ucErrorCode = CONFIGURATION_ERROR_SEQUENCE_ERROR;
                printf("iPlcConfigurationCompare CONFIGURATION_ERROR_SEQUENCE_ERROR: ui8FsmState %d\n\r", (ui8FsmState));
                fbPlcConfigurationError = 1;
                ui8FsmState = UNCONDITIONAL_STOP_SIGNALING;
            }
            else
            {
                // проверим - первый запуск или есть повреждение файлов конфигурации или базы данных?.
                ui8FsmState = CONFIGURATION_AND_DATA_BASE_FILE_CHECK;
            }
            break;

        case UNCONDITIONAL_STOP_SIGNALING:
            printf("iPlcConfigurationCompare UNCONDITIONAL_STOP_SIGNALING: ui8FsmState %d\n\r", (ui8FsmState));
            // ошибку нельзя восстановить. сигнализация и сообщение программатору.
            // если ошибка конфигурации, то передадим количество модулей хранимое в файле.
            xPlcConfigDataTransmit.xConfigData.ucModulesQuantity =
                xPlcConfigData.xConfigData.ucModulesQuantity;
//            xPlcConfigDataTransmit.xConfigData.ucModulesQuantity |= CONFIGURATION_ERROR;
            return 0;
            break;

        case STOP_SIGNALING_CONFIRMATION_WAITING:
            printf("iPlcConfigurationCompare STOP_SIGNALING_CONFIRMATION_WAITING: ui8FsmState %d\n\r", (ui8FsmState));
            // если ошибка конфигурации, то передадим количество модулей хранимое в файле.
            xPlcConfigDataTransmit.xConfigData.ucModulesQuantity =
                xPlcConfigData.xConfigData.ucModulesQuantity;
//            xPlcConfigDataTransmit.xConfigData.ucModulesQuantity |= CONFIGURATION_ERROR;
            return 0;
            break;

        case CONFIGURATION_AND_DATA_BASE_FILE_CHECK:
            printf("iPlcConfigurationCompare CONFIGURATION_AND_DATA_BASE_FILE_CHECK: ui8FsmState %d\n\r", (ui8FsmState));
            // первый запуск или есть повреждение файлов конфигурации или базы данных?
            if (fbPlcConfigurationFileError || fbPlcDataBaseFileError)
            {
                printf("Configuration, or data base file error.\n");
                ui8FsmState = CONFIGURATION_AND_DATA_BASE_FILE_CHECK_WRONG;
            }
            else
            {
                printf("Configuration, or data base file ok.\n");
                ui8FsmState = CONFIGURATION_AND_DATA_BASE_FILE_CHECK_OK;
            }
            break;

        case CONFIGURATION_AND_DATA_BASE_FILE_CHECK_OK:
            printf("iPlcConfigurationCompare CONFIGURATION_AND_DATA_BASE_FILE_CHECK_OK: ui8FsmState %d\n\r", (ui8FsmState));
            printf("Configuration, or data base file ok.\n");
            ui8FsmState = CONFIGURATION_CHECK;
            break;


        case CONFIGURATION_AND_DATA_BASE_FILE_CHECK_WRONG:
            printf("iPlcConfigurationCompare CONFIGURATION_AND_DATA_BASE_FILE_CHECK_WRONG: ui8FsmState %d\n\r", (ui8FsmState));
            // найдено пустое место?
            if (ui8EmptySlotCheck())
            {
                // найдено пустое место.
                // восстановить пустые места по флагам отказов модулей нельзя - нет файла предыдущей конфигурации.
                // остановка, код ошибки "пустые места" программатору, сигнализация.
                xPlcConfigDataTransmit.xConfigData.ucErrorCode = CONFIGURATION_ERROR_EMPTY_SLOT;
                printf("iPlcConfigurationCompare CONFIGURATION_ERROR_EMPTY_SLOT: ui8FsmState %d\n\r", (ui8FsmState));
                fbPlcConfigurationError = 1;
                ui8FsmState = UNCONDITIONAL_STOP_SIGNALING;
            }
            else
            {
                // создаём текущую конфигурацию и новую базу данных.
                vNewConfigurationAndDataBaseCreate();
                // остановка, код ошибки "новая конфигурация и новая база данных" программатору и сигнализация, ожидание подтверждения.
                xPlcConfigDataTransmit.xConfigData.ucErrorCode = CONFIGURATION_ERROR_NEW_CONFIGURATION_AND_DATA_BASE;
                printf("iPlcConfigurationCompare CONFIGURATION_ERROR_NEW_CONFIGURATION_AND_DATA_BASE: ui8FsmState %d\n\r", (ui8FsmState));
                fbPlcConfigurationError = 1;
                ui8FsmState = STOP_SIGNALING_CONFIRMATION_WAITING;
            }
            break;

        case CONFIGURATION_CHECK:
            printf("iPlcConfigurationCompare CONFIGURATION_CHECK: ui8FsmState %d\n\r", (ui8FsmState));
            // не первый запуск и нет повреждений файлов конфигурации или базы данных.
            // количество модулей и конфигурация совпадает?
            if (ui8ModuleQuantityCompare() &&
                    ui8ConfigurationCompare())
            {
//                    printf("iPlcConfigurationCompare NO_ERROR: ui8FsmState %d\n\r", (ui8FsmState));
                ui8FsmState = CONFIGURATION_CHECK_OK;
            }
            else
            {
                ui8FsmState = CONFIGURATION_CHECK_WRONG;
            }
            break;

        case CONFIGURATION_CHECK_OK:
            printf("iPlcConfigurationCompare CONFIGURATION_CHECK_OK: ui8FsmState %d\n\r", (ui8FsmState));
            // сравним базу данных модуля и базу данных прибора.
            vPlcGetModulesDataBaseCompare();
            xPlcConfigDataTransmit.xConfigData.ucErrorCode = 0;
            ui8FsmState = CONFIGURATION_OK_NORMAL_START;
            break;


        case CONFIGURATION_CHECK_WRONG:
            printf("iPlcConfigurationCompare CONFIGURATION_CHECK_WRONG: ui8FsmState %d\n\r", (ui8FsmState));

            // восстанавливаем конфигурацию и количество модулей по зарегистрированным отказам модулей.
            vRegisteredBadModuleConfigurationRepare();

            // количество модулей и конфигурация совпадает?
            if (ui8ModuleQuantityCompare() &&
                    ui8ConfigurationCompare())
            {
                // сравним базу данных модуля и базу данных прибора.
                vPlcGetModulesDataBaseCompare();
                // если после восстановления конфигурации по зарегистрированным отказам модулей
                // количество модулей и конфигурация совпадает, то найден модуль с ранее зарегистрированным отказом.
                // код ошибки "есть слот с отсутствующим модулем с зарегистрированным отказом" на дисплей.
                xPlcConfigDataTransmit.xConfigData.ucErrorCode = CONFIGURATION_ERROR_BAD_MODULE_FINDED_AND_REGISTERED;
                printf("iPlcConfigurationCompare CONFIGURATION_ERROR_BAD_MODULE_FINDED_AND_REGISTERED: ui8FsmState %d\n\r", (ui8FsmState));
                // переходим в нормальный режим работы.
                ui8FsmState = CONFIGURATION_OK_NORMAL_START;
                return 0;

            }
            else
            {
                // найдено пустое место?
                if (ui8EmptySlotCheck())
                {
                    // остановка, код ошибки "пустые места" программатору, сигнализация.
                    xPlcConfigDataTransmit.xConfigData.ucErrorCode = CONFIGURATION_ERROR_EMPTY_SLOT;
                    printf("iPlcConfigurationCompare CONFIGURATION_ERROR_EMPTY_SLOT: ui8FsmState %d\n\r", (ui8FsmState));
                    fbPlcConfigurationError = 1;
                    ui8FsmState = UNCONDITIONAL_STOP_SIGNALING;
                }
                else
                {
                    // количество модулей дискретного и аналогового ввода совпадает?
                    if (ui8DiscreteAndAnalogueModuleQuantityCompare())
                    {
                        // создаём текущую конфигурацию.
                        // создаём текущую конфигурацию и новую базу данных МР.
                        vMrXXDataBaseCreate();
                        // остановка, код ошибки "новая конфигурация и текущая база данных" программатору и сигнализация, ожидание подтверждения.
                        xPlcConfigDataTransmit.xConfigData.ucErrorCode = CONFIGURATION_ERROR_NEW_CONFIGURATION;
                        printf("iPlcConfigurationCompare CONFIGURATION_ERROR_NEW_CONFIGURATION: ui8FsmState %d\n\r", (ui8FsmState));
                        fbPlcConfigurationError = 1;
                        ui8FsmState = STOP_SIGNALING_CONFIRMATION_WAITING;
                    }
                    else
                    {
                        // создаём текущую конфигурацию и новую базу данных.
                        vNewConfigurationAndDataBaseCreate();
                        // остановка, код ошибки "новая конфигурация и новая база данных" программатору и сигнализация, ожидание подтверждения.
                        xPlcConfigDataTransmit.xConfigData.ucErrorCode = CONFIGURATION_ERROR_NEW_CONFIGURATION_AND_DATA_BASE;
                        printf("iPlcConfigurationCompare CONFIGURATION_ERROR_NEW_CONFIGURATION_AND_DATA_BASE: ui8FsmState %d\n\r", (ui8FsmState));
                        fbPlcConfigurationError = 1;
                        ui8FsmState = STOP_SIGNALING_CONFIRMATION_WAITING;
                    }
                }
            }
            break;

        case CONFIGURATION_OK_NORMAL_START:
            printf("iPlcConfigurationCompare CONFIGURATION_OK_NORMAL_START: ui8FsmState %d\n\r", (ui8FsmState));
            // переходим в нормальный режим работы.
            return 0;
            break;

        default:
            break;
        };
    }

    return 0;
}

//-----------------------------------------------------------------------------------------------------
// создание конфигурации.
int iPlcConfigurationCreate(void)
{
    unsigned char ucSlaveAddress;
    unsigned char nucIndexNumber;
    FILE *pxConfigFile;
    FILE *pxReservConfigFile;
    unsigned char nucModuleCounter;
    char *pxFileName;
    char *pxReservFileName;
    int i;
    TMbmFunction1234 *pxFunction1234Pointer;
    TMbmFunction15 *pxFunction15Pointer;

    ucSlaveAddress = 0;
    memset(&xPlcConfigService,
           0,
           sizeof(struct TPlcConfigService));

    // поместим фактическое количество модулей в служебную переменную.
    xPlcConfigService.xPlcConfigServiceData.ucInternalModulesQuantity =
        xPlcConfigDataTemp.xConfigData.ucModulesQuantity;

    // вычисление служебных переменных для внутренних модулей.
    for (ucSlaveAddress = 0;
            ucSlaveAddress < INTERNAL_MODULE_QUANTITY;
            ucSlaveAddress++)
    {
        switch(xPlcConfigDataTemp.
                xConfigData.aucModuleType[ucSlaveAddress])
        {
        case MODULE_TYPE_MVDI:
        case MODULE_TYPE_MVDS9:
            xPlcConfigDataTemp.
            xConfigData.ucLastDiscreteInputModuleAddressPlusOne =
                (ucSlaveAddress + 1);
            (xPlcConfigService.xPlcConfigServiceData.ucServiceDiscreteInputModuleQuantity)++;
            xPlcConfigService.xPlcConfigServiceData.ucLastDiscreteInputModuleIndex =
                ucSlaveAddress;
            xPlcConfigService.xPlcConfigServiceData.ucServiceDiscreteInputQuantity +=
                MVDI5_DISCRETE_INPUT_QUANTITY;
            break;

        case MODULE_TYPE_MVPS3:
        case MODULE_TYPE_MVAI5:
        case MODULE_TYPE_MVST3:
        case MODULE_TYPE_MVAO3:
            xPlcConfigDataTemp.xConfigData.ucLastAnalogueInputModuleAddressPlusOne =
                (ucSlaveAddress + 1);
            (xPlcConfigService.xPlcConfigServiceData.ucServiceAnalogueInputModuleQuantity)++;
            xPlcConfigService.xPlcConfigServiceData.ucLastAnalogueInputModuleIndex =
                ucSlaveAddress;
            xPlcConfigService.xPlcConfigServiceData.ucServiceDiscreteInputQuantity +=
                MVAI5_DISCRETE_INPUT_QUANTITY;
            xPlcConfigService.xPlcConfigServiceData.ucServiceAnalogueInputQuantity +=
                MVAI5_ANALOG_INPUT_QUANTITY;

            break;
        case MODULE_TYPE_MR53:
        case MODULE_TYPE_MR54:
        case MODULE_TYPE_MR55:
        case MODULE_TYPE_MR51:
            (xPlcConfigService.xPlcConfigServiceData.ucServiceDiscreteOutputModuleQuantity)++;
            xPlcConfigService.xPlcConfigServiceData.ucLastDiscreteOutputModuleIndex =
                ucSlaveAddress;
            xPlcConfigService.xPlcConfigServiceData.ucServiceDiscreteOutputQuantity +=
                MRXX_DISCRETE_OUTPUT_QUANTITY;
            break;
        case MODULE_TYPE_MTVI5:
            (xPlcConfigService.xPlcConfigServiceData.ucServiceAnalogueOutputModuleQuantity)++;
            xPlcConfigService.xPlcConfigServiceData.ucLastAnalogueOutputModuleIndex =
                ucSlaveAddress;
            xPlcConfigService.xPlcConfigServiceData.ucServiceAnalogueOutputQuantity +=
                MTVI5_ANALOG_OUTPUT_QUANTITY;
            break;
        default:
            break;
        };
    }

    nucModuleCounter = 0;
    // получим указатель на массив с нормализованной базой данных.
    pxFunction1234Pointer =
        axModbusMasterFunction1234DescriptionWork;
    // получим указатель на блок базы данных.
    pxFunction15Pointer =
        (TMbmFunction15*)&(xPlcDataBase.
                           axPlcDataBaseBlocks[MBM_FUNCTION_15_BLOCK_DATA_BASE_BLOCK_OFFSET].
                           aucPlcDataBaseBlockData[0]);
    // посчитаем количество внешних модулей.
    for (i = 0;
            i < EXTERNAL_MODULE_QUANTITY;
            i++)
    {
        // модуль запрограммирован?
        if (pxFunction1234Pointer[i].ucModbusFunction)
        {
            nucModuleCounter++;
        }
        else
        {
            break;
        }
    }

    for (i = 0;
            i < EXTERNAL_MODULE_QUANTITY;
            i++)
    {
        // модуль запрограммирован?
        if (pxFunction15Pointer[i].ucModbusFunction)
        {
            nucModuleCounter++;
        }
        else
        {
            break;
        }
    }


    xPlcConfigService.xPlcConfigServiceData.ucExternalModulesQuantity =
        nucModuleCounter;

    // продолжим вычисление служебных переменных для внешних модулей.
    for (i = 0;
            i < EXTERNAL_MODULE_QUANTITY;
            i++)
    {
        switch(pxFunction1234Pointer[i].
                ucModbusFunction)
        {
        case 1:
        case 2:
            xPlcConfigService.xPlcConfigServiceData.ucServiceDiscreteInputQuantity +=
                (pxFunction1234Pointer[i].
                 ucIOQuantity);
            break;
        case 3:
        case 4:
            xPlcConfigService.xPlcConfigServiceData.ucServiceDiscreteInputQuantity +=
                (pxFunction1234Pointer[i].
                 ucIOQuantity * ANALOGUE_INPUT_DI_VALUE_QUANTITY);
            xPlcConfigService.xPlcConfigServiceData.ucServiceAnalogueInputQuantity +=
                (pxFunction1234Pointer[i].
                 ucIOQuantity);
            break;

        default:
            break;
        };
    }

    unsigned char ucCheck;
    unsigned char *pucSource;
    TDiscreteInputDescriptionWork *pxDiscreteInputDescriptionWork;

    nucModuleCounter = 0;
    // получим указатель на буфер с нормализованной базой данных прибора.
    pxDiscreteInputDescriptionWork = axDiscreteInputDescriptionWork;

    // посчитаем количество запрограммированных, обрабатываемых дискретных входов.
    for (int i = 0;
            i < MAX_HANDLED_DISCRETE_INPUT;
            i++)
    {
        pucSource = (unsigned char*)&pxDiscreteInputDescriptionWork[i];
        ucCheck = 0;
        // суммируем данные описателя, если, не равно нулю, то вход запрограммирован.
        for (int j = 0;
                j < (sizeof(struct TDiscreteInputDescriptionWork) - ONE_BYTE_CRC_LENGTH);
                j++)
        {
            ucCheck += pucSource[j];
        }

        if (ucCheck)
        {
            // ещё один дискретный сигнал запрограммирован.
            nucModuleCounter++;
        }
        else
        {
            break;
        }
    }

    xPlcConfigService.xPlcConfigServiceData.
    uiHandledDiscreteInputQuantity =
        nucModuleCounter;

    TFunctionBlockDescriptionWork *pxFunctionBlockDataBaseSource;
    nucModuleCounter = 0;
    // получим указатель на буфер с нормализованной базой данных прибора.
    pxFunctionBlockDataBaseSource = axFunctionBlockDescriptionWork;

    // посчитаем количество запрограммированных, обрабатываемых функциональных блоков.
    for (int i = 0;
            i < MAX_HANDLED_FUNCTION_BLOCK;
            i++)
    {
        pucSource = (unsigned char*)&pxFunctionBlockDataBaseSource[i];
        ucCheck = 0;
        // суммируем данные описателя, если, не равно нулю, то функциональный блок запрограммирован.
        for (int j = 0;
                j < (sizeof(struct TFunctionBlockDescriptionWork) - ONE_BYTE_CRC_LENGTH);
                j++)
        {
            ucCheck += pucSource[j];
        }

        if (ucCheck)
        {
            // ещё один функциональный блок запрограммирован.
            nucModuleCounter++;
        }
        else
        {
            break;
        }
    }

    xPlcConfigService.xPlcConfigServiceData.
    uiHandledFunctionBlockQuantity =
        nucModuleCounter;

    // заполним временный буфер конфигурации.
    xPlcConfigDataTemp.xConfigData.ui16DiscreteInputQuantity =
        xPlcConfigService.xPlcConfigServiceData.ucServiceDiscreteInputQuantity;
    xPlcConfigDataTemp.xConfigData.ucAnalogueInputQuantity =
        xPlcConfigService.xPlcConfigServiceData.ucServiceAnalogueInputQuantity;
    xPlcConfigDataTemp.xConfigData.ucDiscreteOutputQuantity =
        xPlcConfigService.xPlcConfigServiceData.ucServiceDiscreteOutputQuantity;

    xPlcConfigDataTemp.xConfigData.ucDisplayType =
        DISPLAY_MODULE_TYPE;

    xPlcConfigDataTemp.usCrc =
        usCrc16((unsigned char*)&(xPlcConfigDataTemp.xConfigData),
                sizeof(struct TConfigData));
    // вычислим контрольные суммы. потом будем проверять целостность перед каждым главным циклом.
    xPlcConfigService.usCrc =
        usCrcSummTwoByteCalculation((unsigned char*)&xPlcConfigService.xPlcConfigServiceData,
                                    sizeof(xPlcConfigService.xPlcConfigServiceData));

    // скопируем конфигурацию в файл из которого программатор получает её по Modbus функция 17.
    memcpy(&xPlcConfigDataTransmit,
           &xPlcConfigDataTemp,
           sizeof(xPlcConfigDataTransmit));

    // покажем временный буфер конфигурации для отладки.
    printf("Current configuration is:\n");
    for (i = 0;
            i < (INTERNAL_MODULE_QUANTITY);
            i++)
    {
        if ((xPlcConfigDataTemp.
                xConfigData.aucModuleType[i]))
        {
            printf("Address: %d Ok. Type: %X.\n",
                   (unsigned int)i,
                   (unsigned int)(xPlcConfigDataTemp.
                                  xConfigData.aucModuleType[i]));
        }
        else
        {
            printf("Address: %d Empty.\n",
                   (unsigned int)i);
        }

    }

    printf("ModulesQuantity is: %d.\n",
           xPlcConfigDataTemp.xConfigData.ucModulesQuantity);
    printf("DiscreteInputQuantity is: %d.\n",
           xPlcConfigDataTemp.xConfigData.ui16DiscreteInputQuantity);
    printf("AnalogueInputQuantity is: %d.\n",
           xPlcConfigDataTemp.xConfigData.ucAnalogueInputQuantity);
    printf("DiscreteOutputQuantity is: %d.\n",
           xPlcConfigDataTemp.xConfigData.ucDiscreteOutputQuantity);
    printf("DisplayType is: %d.\n",
           xPlcConfigDataTemp.xConfigData.ucDisplayType);

    return 0;
}

//-----------------------------------------------------------------------------------------------------
// сохранение конфигурации в файлы.
int iPlcConfigurationSave(void)
{
    FILE *pxConfigFile;
    FILE *pxReservConfigFile;
    char *pxFileName;
    char *pxReservFileName;

    pxFileName = CONFIG_FILE_NAME;
    pxConfigFile = fopen(pxFileName,"r+");
    if(pxConfigFile == NULL)
    {
        printf("Cannot open Config file.\n");
        pxConfigFile = fopen(pxFileName,"w+");
        if(pxConfigFile == NULL)
        {
            printf("Cannot create Config file.\n");
            return -1;
        }
    }

    pxReservFileName = RESERV_CONFIG_FILE_NAME;
    pxReservConfigFile = fopen(pxReservFileName,"r+");
    if(pxReservConfigFile == NULL)
    {
        printf("Cannot open Reserv Config file.\n");
        pxReservConfigFile = fopen(pxReservFileName,"w+");
        if(pxReservConfigFile == NULL)
        {
            printf("Cannot create Reserv Config file.\n");
            return -1;
        }
    }
    // конфигурация не повреждена?
    if(usCrc16((unsigned char*)&(xPlcConfigDataTemp.xConfigData),
               sizeof(struct TConfigData)) == xPlcConfigDataTemp.usCrc)
    {
        // сохраним конфигурацию.
        fseek (pxConfigFile, 0, SEEK_SET);
        fwrite((const void*)&xPlcConfigDataTemp,
               sizeof(struct TPlcConfigData),
               1,
               pxConfigFile);

        fseek (pxReservConfigFile, 0, SEEK_SET);
        fwrite((const void*)&xPlcConfigDataTemp,
               sizeof(struct TPlcConfigData),
               1,
               pxReservConfigFile);
        printf("New Configuration is Set: %d Modules.\n",
               xPlcConfigDataTemp.xConfigData.ucModulesQuantity);
    }
    else
    {
        printf("Configuration write CRC wrong.\n");
    }

    fclose(pxConfigFile);
    fclose(pxReservConfigFile);
//    malloc(1000);//? если убрать fclose портит объект mb_mapping.


    return 0;
}

//-----------------------------------------------------------------------------------------------------
// сохранение базы данных в файлы.
int iPlcDataBaseSave(void)
{
    FILE *pxDataBaseFile;
    FILE *pxReservDataBaseFile;
    char *pxFileName;
    char *pxReservFileName;
    int i;
    int iResult;
    unsigned char nucIndexNumber;


    pxFileName = DATA_BASE_FILE_NAME;
    pxDataBaseFile = fopen(pxFileName,"r+");
    if(pxDataBaseFile == NULL)
    {
        printf("Cannot open Data base file.\n");
        pxDataBaseFile = fopen(pxFileName,"w+");
        if(pxDataBaseFile == NULL)
        {
            printf("Cannot create Data base file.\n");
            return -1;
        }
    }

    pxReservFileName = RESERV_DATA_BASE_FILE_NAME;
    pxReservDataBaseFile = fopen(pxReservFileName,"r+");
    if(pxReservDataBaseFile == NULL)
    {
        printf("Cannot open Reserv Data base file.\n");
        pxReservDataBaseFile = fopen(pxReservFileName,"w+");
        if(pxReservDataBaseFile == NULL)
        {
            printf("Cannot create Reserv Data base file.\n");
            return -1;
        }
    }

    // база данных не повреждена?
    if(usCrc16((unsigned char*)xPlcDataBase.axPlcDataBaseBlocks,
               (sizeof(xPlcDataBase.axPlcDataBaseBlocks))) == xPlcDataBase.usCrc)
    {
        // сохраним базу данных.
        fseek (pxDataBaseFile, 0, SEEK_SET);
        fwrite((const void*)&xPlcDataBase,
               sizeof(struct TPlcDataBase),
               1,
               pxDataBaseFile);
        fseek (pxReservDataBaseFile, 0, SEEK_SET);
        fwrite((const void*)&xPlcDataBase,
               sizeof(struct TPlcDataBase),
               1,
               pxReservDataBaseFile);
        printf("DataBase had saved in file.\n");
    }
    else
    {
        printf("Data base write CRC wrong.\n");
    }

    fclose(pxDataBaseFile);
    fclose(pxReservDataBaseFile);

    return 0;
}

////-----------------------------------------------------------------------------------------------------
//// запишем один блок базы данных в память прибора.
//int iPlcDataBaseBlockWrite(unsigned char nucDataBaseBlockIndex,
//                           unsigned char *pucSource,
//                           unsigned short nusBlockLength)
//{
//    FILE *pxDataBaseFile;
//    FILE *pxReservDataBaseFile;
//    char *pxFileName;
//    char *pxReservFileName;
//    int i;
//    unsigned short usCrc;
//    unsigned char nucIndexNumber;
//    unsigned char *pucDestination;
//
//    // проверим и обновим буфер базы данных из файла.
//    if (iPlcDataBaseCheck() != -1)
//    {
//        pucDestination = &(xPlcDataBase.
//                           axPlcDataBaseBlocks[nucDataBaseBlockIndex].
//                           aucPlcDataBaseBlockData[0]);
//
//        // скопируем блок базы данных пришедший по Modbus в буфер базы данных RAM.
//        memset(pucDestination,
//               0,
//               PLC_DATA_BASE_BLOCK_LENGTH);
//        memcpy(pucDestination,
//               pucSource,
//               nusBlockLength);
//        vDataBaseOneBlockCrcCreate((TPlcDataBaseBlock*)pucDestination);
//
//        pxFileName = DATA_BASE_FILE_NAME;
//        pxDataBaseFile = fopen(pxFileName,"r+");
//        if(pxDataBaseFile == NULL)
//        {
//            printf("Cannot open Data base file.\n");
//            pxDataBaseFile = fopen(pxFileName,"w+");
//            if(pxDataBaseFile == NULL)
//            {
//                printf("Cannot create Data base file.\n");
//                return -1;
//            }
//        }
//
//        pxReservFileName = RESERV_DATA_BASE_FILE_NAME;
//        pxReservDataBaseFile = fopen(pxReservFileName,"r+");
//        if(pxReservDataBaseFile == NULL)
//        {
//            printf("Cannot open Reserv Data base file.\n");
//            pxReservDataBaseFile = fopen(pxReservFileName,"w+");
//            if(pxReservDataBaseFile == NULL)
//            {
//                printf("Cannot create Reserv Data base file.\n");
//                return -1;
//            }
//        }
//        // база данных не повреждена? и блок базы данных не повреждён?
//        if ((usCrc16((unsigned char*)xPlcDataBase.axPlcDataBaseBlocks,
//                     (sizeof(xPlcDataBase.axPlcDataBaseBlocks))) == xPlcDataBase.usCrc) &&
//                (iCrc16Check(pucDestination,
//                             (PLC_DATA_BASE_BLOCK_LENGTH -
//                              TWO_BYTE_CRC_LENGTH))) != -1)
//        {
//            // сохраним блок базы данных.
//            // установим смещение на требуемый блок базы данных.
//            fseek (pxDataBaseFile, ((nucDataBaseBlockIndex) * PLC_DATA_BASE_BLOCK_LENGTH), SEEK_SET);
//            fwrite((const void*)pucDestination,
//                   PLC_DATA_BASE_BLOCK_LENGTH,
//                   1,
//                   pxDataBaseFile);
//
//            // установим смещение на требуемый блок базы данных.
//            fseek (pxReservDataBaseFile, ((nucDataBaseBlockIndex) * PLC_DATA_BASE_BLOCK_LENGTH), SEEK_SET);
//            fwrite((const void*)pucDestination,
//                   PLC_DATA_BASE_BLOCK_LENGTH,
//                   1,
//                   pxReservDataBaseFile);
//
//            // запишем новую контрольную сумму базы данных.
//            fseek (pxDataBaseFile, (sizeof(struct TPlcDataBase) -
//                                    TWO_BYTE_CRC_LENGTH), SEEK_SET);
//            fwrite((const void*)&(xPlcDataBase.usCrc),
//                   TWO_BYTE_CRC_LENGTH,
//                   1,
//                   pxDataBaseFile);
//
//            fseek (pxReservDataBaseFile, (sizeof(struct TPlcDataBase) -
//                                          TWO_BYTE_CRC_LENGTH), SEEK_SET);
//            fwrite((const void*)&(xPlcDataBase.usCrc),
//                   TWO_BYTE_CRC_LENGTH,
//                   1,
//                   pxReservDataBaseFile);
//            printf("DataBase block had saved in file.\n");
//        }
//        else
//        {
//            printf("Data base block write CRC wrong.\n");
//        }
//
//        fclose(pxDataBaseFile);
//        fclose(pxReservDataBaseFile);
//    }
//
//    return 0;
//}
//
////-----------------------------------------------------------------------------------------------------
//// запишем один блок базы данных в память прибора.
//int iPlcDataBaseBlockWrite(unsigned char nucDataBaseBlockIndex,
//                           unsigned char *pucSource,
//                           unsigned short nusBlockLength)
//{
//    FILE *pxDataBaseFile;
//    FILE *pxReservDataBaseFile;
//    char *pxFileName;
//    char *pxReservFileName;
//    int i;
//    unsigned short usCrc;
//    unsigned char nucIndexNumber;
//    unsigned char *pucDestination;
//
//    pucDestination = &(xPlcDataBase.
//                       axPlcDataBaseBlocks[nucDataBaseBlockIndex].
//                       aucPlcDataBaseBlockData[0]);
//
//    // скопируем блок базы данных пришедший по Modbus в буфер базы данных RAM.
//    memset(pucDestination,
//           0,
//           PLC_DATA_BASE_BLOCK_LENGTH);
//    memcpy(pucDestination,
//           pucSource,
//           nusBlockLength);
//    vDataBaseOneBlockCrcCreate((TPlcDataBaseBlock*)pucDestination);
//
//    pxFileName = DATA_BASE_FILE_NAME;
//    pxDataBaseFile = fopen(pxFileName,"r+");
//    if(pxDataBaseFile == NULL)
//    {
//        printf("Cannot open Data base file.\n");
//        pxDataBaseFile = fopen(pxFileName,"w+");
//        if(pxDataBaseFile == NULL)
//        {
//            printf("Cannot create Data base file.\n");
//            return -1;
//        }
//    }
//
//    pxReservFileName = RESERV_DATA_BASE_FILE_NAME;
//    pxReservDataBaseFile = fopen(pxReservFileName,"r+");
//    if(pxReservDataBaseFile == NULL)
//    {
//        printf("Cannot open Reserv Data base file.\n");
//        pxReservDataBaseFile = fopen(pxReservFileName,"w+");
//        if(pxReservDataBaseFile == NULL)
//        {
//            printf("Cannot create Reserv Data base file.\n");
//            return -1;
//        }
//    }
//    // база данных не повреждена? и блок базы данных не повреждён?
//    if ((usCrc16((unsigned char*)xPlcDataBase.axPlcDataBaseBlocks,
//                 (sizeof(xPlcDataBase.axPlcDataBaseBlocks))) == xPlcDataBase.usCrc) &&
//            (iCrc16Check(pucDestination,
//                         (PLC_DATA_BASE_BLOCK_LENGTH -
//                          TWO_BYTE_CRC_LENGTH))) != -1)
//    {
//        // сохраним блок базы данных.
//        // установим смещение на требуемый блок базы данных.
//        fseek (pxDataBaseFile, ((nucDataBaseBlockIndex) * PLC_DATA_BASE_BLOCK_LENGTH), SEEK_SET);
//        fwrite((const void*)pucDestination,
//               PLC_DATA_BASE_BLOCK_LENGTH,
//               1,
//               pxDataBaseFile);
//
//        // установим смещение на требуемый блок базы данных.
//        fseek (pxReservDataBaseFile, ((nucDataBaseBlockIndex) * PLC_DATA_BASE_BLOCK_LENGTH), SEEK_SET);
//        fwrite((const void*)pucDestination,
//               PLC_DATA_BASE_BLOCK_LENGTH,
//               1,
//               pxReservDataBaseFile);
//
//        // запишем новую контрольную сумму базы данных.
//        fseek (pxDataBaseFile, (sizeof(struct TPlcDataBase) -
//                                TWO_BYTE_CRC_LENGTH), SEEK_SET);
//        fwrite((const void*)&(xPlcDataBase.usCrc),
//               TWO_BYTE_CRC_LENGTH,
//               1,
//               pxDataBaseFile);
//
//        fseek (pxReservDataBaseFile, (sizeof(struct TPlcDataBase) -
//                                      TWO_BYTE_CRC_LENGTH), SEEK_SET);
//        fwrite((const void*)&(xPlcDataBase.usCrc),
//               TWO_BYTE_CRC_LENGTH,
//               1,
//               pxReservDataBaseFile);
//        printf("DataBase block had saved in file.\n");
//    }
//    else
//    {
//        printf("Data base block write CRC wrong.\n");
//    }
//
//    fclose(pxDataBaseFile);
//    fclose(pxReservDataBaseFile);
//
//    return 0;
//}

//-----------------------------------------------------------------------------------------------------
// запишем один блок базы данных в память прибора.
int iPlcDataBaseBlockWrite(unsigned char nucDataBaseBlockIndex)
{
    FILE *pxDataBaseFile;
    FILE *pxReservDataBaseFile;
    char *pxFileName;
    char *pxReservFileName;
    int i;
    unsigned short usCrc;
    unsigned char nucIndexNumber;
    unsigned char *pucSource;

    pucSource = &(xPlcDataBase.
                  axPlcDataBaseBlocks[nucDataBaseBlockIndex].
                  aucPlcDataBaseBlockData[0]);

    pxFileName = DATA_BASE_FILE_NAME;
    pxDataBaseFile = fopen(pxFileName,"r+");
    if(pxDataBaseFile == NULL)
    {
        printf("Cannot open Data base file.\n");
        pxDataBaseFile = fopen(pxFileName,"w+");
        if(pxDataBaseFile == NULL)
        {
            printf("Cannot create Data base file.\n");
            return -1;
        }
    }

    pxReservFileName = RESERV_DATA_BASE_FILE_NAME;
    pxReservDataBaseFile = fopen(pxReservFileName,"r+");
    if(pxReservDataBaseFile == NULL)
    {
        printf("Cannot open Reserv Data base file.\n");
        pxReservDataBaseFile = fopen(pxReservFileName,"w+");
        if(pxReservDataBaseFile == NULL)
        {
            printf("Cannot create Reserv Data base file.\n");
            return -1;
        }
    }
    // база данных не повреждена? и блок базы данных не повреждён?
    if ((usCrc16((unsigned char*)xPlcDataBase.axPlcDataBaseBlocks,
                 (sizeof(xPlcDataBase.axPlcDataBaseBlocks))) == xPlcDataBase.usCrc) &&
            (iCrc16Check(pucSource,
                         (PLC_DATA_BASE_BLOCK_LENGTH -
                          TWO_BYTE_CRC_LENGTH))) != -1)
    {
        // сохраним блок базы данных.
        // установим смещение на требуемый блок базы данных.
        fseek (pxDataBaseFile, ((nucDataBaseBlockIndex) * PLC_DATA_BASE_BLOCK_LENGTH), SEEK_SET);
        fwrite((const void*)pucSource,
               PLC_DATA_BASE_BLOCK_LENGTH,
               1,
               pxDataBaseFile);

        // установим смещение на требуемый блок базы данных.
        fseek (pxReservDataBaseFile, ((nucDataBaseBlockIndex) * PLC_DATA_BASE_BLOCK_LENGTH), SEEK_SET);
        fwrite((const void*)pucSource,
               PLC_DATA_BASE_BLOCK_LENGTH,
               1,
               pxReservDataBaseFile);

        // запишем новую контрольную сумму базы данных.
        fseek (pxDataBaseFile, (sizeof(struct TPlcDataBase) -
                                TWO_BYTE_CRC_LENGTH), SEEK_SET);
        fwrite((const void*)&(xPlcDataBase.usCrc),
               TWO_BYTE_CRC_LENGTH,
               1,
               pxDataBaseFile);

        fseek (pxReservDataBaseFile, (sizeof(struct TPlcDataBase) -
                                      TWO_BYTE_CRC_LENGTH), SEEK_SET);
        fwrite((const void*)&(xPlcDataBase.usCrc),
               TWO_BYTE_CRC_LENGTH,
               1,
               pxReservDataBaseFile);
        printf("DataBase block had saved in file.\n");
    }
    else
    {
        printf("Data base block write CRC wrong.\n");
    }

    fclose(pxDataBaseFile);
    fclose(pxReservDataBaseFile);

    return 0;
}

//-----------------------------------------------------------------------------------------------------
// при замене модуля.
// сравнивает внутренние базы данных модулей с базами в памяти прибора.
// если не совпадает, записывает базу из прибора в модули.
int iPlcModuleReplacementCheck(void)
{
    int i;
    unsigned char iResult;
    unsigned char nucIndexNumber;

    iResult = 0;
    nucIndexNumber = 0;

    for (i = 0;
            i < (xPlcConfigService.
                 xPlcConfigServiceData.
                 ucInternalModulesQuantity);
            i++)
    {
        switch(xAllModulesContext.axAllModulesContext[nucIndexNumber].
                xModuleContextStatic.ucModuleType)
        {
        case MODULE_TYPE_MVDS9:
            if ((iModuleMvds9ModuleDataBaseCheck(&xAllModulesContext.axAllModulesContext[nucIndexNumber])) != 0)
            {
                iResult = -1;
            }

            break;

        case MODULE_TYPE_MVPS3:
        case MODULE_TYPE_MVAI5:
        case MODULE_TYPE_MVST3:
        case MODULE_TYPE_MVAO3:
            if ((iModuleMvai5ModuleDataBaseCheck(&xAllModulesContext.axAllModulesContext[nucIndexNumber])) != 0)
            {
                iResult = -1;
            }
            break;

        case MODULE_TYPE_MTVI5:
            if ((iModuleMtvi5RegulatorModuleDataBaseCheck(&xAllModulesContext.axAllModulesContext[nucIndexNumber])) != 0)
            {
                iResult = -1;
            }
            break;

        default:
            break;
        };

        nucIndexNumber++;
    }
    return iResult;
}

//-----------------------------------------------------------------------------------------------------
// сравнивает базу модуля с базой прибора, если не совпадает, берёт из модуля.
int iPlcGetModulesDataBase(void)
{
    int i;
    int iResult;
    unsigned char nucIndexNumber;

    iResult = 0;
    nucIndexNumber = 0;

    for (i = 0;
            i < (xPlcConfigService.
                 xPlcConfigServiceData.
                 ucInternalModulesQuantity);
            i++)
    {
        switch(xAllModulesContext.axAllModulesContext[nucIndexNumber].
                xModuleContextStatic.ucModuleType)
        {
        case MODULE_TYPE_MVDS9:
            if ((iModuleMvds9PlcDataBaseCheck(&xAllModulesContext.axAllModulesContext[nucIndexNumber])) != 0)
            {
                iResult = -1;
            }

            break;

        case MODULE_TYPE_MVPS3:
        case MODULE_TYPE_MVAI5:
        case MODULE_TYPE_MVST3:
        case MODULE_TYPE_MVAO3:
            if ((iModuleMvai5PlcDataBaseCheck(&xAllModulesContext.axAllModulesContext[nucIndexNumber])) != 0)
            {
                iResult = -1;
            }
            break;

        case MODULE_TYPE_MTVI5:
            if ((iModuleMtvi5RegulatorPlcDataBaseCheck(&xAllModulesContext.axAllModulesContext[nucIndexNumber])) != 0)
            {
                iResult = -1;
            }
            break;

        default:
            break;
        };

        nucIndexNumber++;
    }
    return iResult;
}

//-----------------------------------------------------------------------------------------------------
// инициализирует данные контекста модуля.
// задаёт начальные адреса в рабочих массивах.
void vMainTaskMemoryMapModuleContextInitialisation(TModuleContext *pxModuleContext)
{
    // указатель на объект в памяти - передаваемая по интерфейсам связи конфигурация.
    pxModuleContext ->
    xModuleContextStatic.
    pucPlcConfigDataTempPointer =
        (unsigned char*)&xPlcConfigDataTemp.xConfigData;

    // указатели на пространство в массивах "Modbus".
    // указатели на начало в рабочих массивах.
    pxModuleContext ->
    xModuleContextStatic.
    pucAiValueOriginBufferPointer =
        (unsigned char*)afAnalogueInputData;

    pxModuleContext ->
    xModuleContextStatic.
    pucDiValueOriginBufferPointer =
        &aucDiscreteInputsArray[DI_VALUE_BIT_ARRAY_OFFSET];

    pxModuleContext ->
    xModuleContextStatic.
    pucSostDoOriginBufferPointer =
        &aucCoilsArray[SOST_DO_BIT_ARRAY_OFFSET];

    pxModuleContext ->
    xModuleContextStatic.
    pucBadAiOriginBufferPointer =
        &aucCoilsArray[BAD_AI_BIT_ARRAY_OFFSET];

    pxModuleContext ->
    xModuleContextStatic.
    pucCurrentTimeBufferPointer =
        (unsigned char*)&xCurrentTime;

//    pxModuleContext ->
//    xModuleContextStatic.
//    pucAlarmEventsLog =
//        (unsigned char*)CAlarm::axAlarmEventsLog;
//
//    pxModuleContext ->
//    xModuleContextStatic.
//    pucArchiveEventsLog =
//        (unsigned char*)CArchiveEventsDB.axQueryDataCommon;
//
//    pxModuleContext ->
//    xModuleContextStatic.
//    pucErrorLog =
//        (unsigned char*)CError::axErrorLog;

    pxModuleContext ->
    xModuleContextStatic.
    pucAnalogueInputDescriptionWork =
        (unsigned char*)axAnalogueInputDescriptionWork;

    pxModuleContext ->
    xModuleContextStatic.
    pucDimentionsParametersDataBase =
        (unsigned char*)axDimentionsParametersDescriptionWork;

    pxModuleContext ->
    xModuleContextStatic.
    pucLogSostOriginBufferPointer =
        &aucCoilsArray[LOG_SOST_BIT_ARRAY_OFFSET];



    // указатели на пространство в массивах "Modbus".
    // указатели на пространство в рабочих массивах,
    // заранее определённое для каждого модуля.
    pxModuleContext ->
    xModuleContextStatic.
    pucDiValueBufferPointer =
        &aucDiscreteInputsArray[DI_VALUE_BIT_ARRAY_OFFSET];

    pxModuleContext ->
    xModuleContextStatic.
    pucDoValueBufferPointer =
        &aucDoValueByteArray[0];

    pxModuleContext ->
    xModuleContextStatic.
    pucSostDoBufferPointer =
        &aucCoilsArray[SOST_DO_BIT_ARRAY_OFFSET];

    pxModuleContext ->
    xModuleContextStatic.
    pucBadDiBufferPointer =
        &aucCoilsArray[BAD_DI_BIT_ARRAY_OFFSET];

    pxModuleContext ->
    xModuleContextStatic.
    pucAiValueBufferPointer =
        (unsigned char*)afAnalogueInputData;

    pxModuleContext ->
    xModuleContextStatic.
    pucContAvBufferPointer =
        (unsigned char*)axAnalogueOutputRegulatorContAv;

    pxModuleContext ->
    xModuleContextStatic.
    pucCalcValueOriginBufferPointer =
        (unsigned char*)afCalcValue;

    pxModuleContext ->
    xModuleContextStatic.
    pucStatAiBufferPointer =
        &aucStatAiByteArray[0];

    pxModuleContext ->
    xModuleContextStatic.
    pucBadAiBufferPointer =
        &aucCoilsArray[BAD_AI_BIT_ARRAY_OFFSET];

    pxModuleContext ->
    xModuleContextStatic.
    pucModuleBadStateBufferPointer =
        aucBadModuleStateBuff;//&aucCoilsArray[BAD_MODULES_BIT_ARRAY_OFFSET];

    pxModuleContext ->
    xModuleContextStatic.
    pucAinOffBufferPointer =
        &aucCoilsArray[AIN_OFF_BIT_ARRAY_OFFSET];

    pxModuleContext ->
    xModuleContextStatic.
    pucContStBufferPointer =
        &aucContStByteArray[0];

    pxModuleContext ->
    xModuleContextStatic.
    pucContStWriteBufferPointer =
        &aucContStWriteByteArray[0];

    pxModuleContext ->
    xModuleContextStatic.
    pucBadDoBufferPointer =
        &aucCoilsArray[BAD_DO_BIT_ARRAY_OFFSET];

    pxModuleContext ->
    xModuleContextStatic.
    pucStatDoBufferPointer =
        &aucCoilsArray[STAT_DO_BIT_ARRAY_OFFSET];

    pxModuleContext ->
    xModuleContextStatic.
    pucBadAoBufferPointer =
        &aucCoilsArray[BAD_AO_BIT_ARRAY_OFFSET];

    pxModuleContext ->
    xModuleContextStatic.
    pucDoValueBlockBufferPointer =
        &aucDoValueBlockByteArray[0];

    pxModuleContext ->
    xModuleContextStatic.
    pucTxsBufferPointer =
        (unsigned char*)&axAllModulesTxsData[0];

    pxModuleContext ->
    xModuleContextStatic.
    pucReperPointsAdcBufferPointer =
        (unsigned char*)&aucReperPointsAdcBufferPointer[0];


    // указатели на описатели модулей.
    pxModuleContext ->
    xModuleContextStatic.
    pucDataBasePointer =
        (unsigned char*)axAnalogueInputDescriptionWork;

    pxModuleContext ->
    xModuleContextStatic.
    pucDataBaseExtractPointer =
        (unsigned char*)axAnalogueInputDescriptionWork;
}

//-----------------------------------------------------------------------------------------------------
// передаёт данные контекста следующему модулю.
void vMainTaskMemoryMapToNextModule(TModuleContext *pxModuleContext)
{
    // отправим следующему модулю.
    pxModuleContext[1].
    xModuleContextStatic.
    pucDiValueBufferPointer =
        pxModuleContext ->
        xModuleContextStatic.
        pucDiValueBufferPointer;

    pxModuleContext[1].
    xModuleContextStatic.
    pucDoValueBufferPointer =
        pxModuleContext ->
        xModuleContextStatic.
        pucDoValueBufferPointer;

    pxModuleContext[1].
    xModuleContextStatic.
    pucSostDoBufferPointer =
        pxModuleContext ->
        xModuleContextStatic.
        pucSostDoBufferPointer;

    pxModuleContext[1].
    xModuleContextStatic.
    pucBadDiBufferPointer =
        pxModuleContext ->
        xModuleContextStatic.
        pucBadDiBufferPointer;

    pxModuleContext[1].
    xModuleContextStatic.
    pucAiValueOriginBufferPointer =
        pxModuleContext ->
        xModuleContextStatic.
        pucAiValueOriginBufferPointer;

    pxModuleContext[1].
    xModuleContextStatic.
    pucAiValueBufferPointer =
        pxModuleContext ->
        xModuleContextStatic.
        pucAiValueBufferPointer;

    pxModuleContext[1].
    xModuleContextStatic.
    pucContAvBufferPointer =
        pxModuleContext ->
        xModuleContextStatic.
        pucContAvBufferPointer;

    pxModuleContext[1].
    xModuleContextStatic.
    pucCalcValueOriginBufferPointer =
        pxModuleContext ->
        xModuleContextStatic.
        pucCalcValueOriginBufferPointer;

    pxModuleContext[1].
    xModuleContextStatic.
    pucLogSostOriginBufferPointer =
        pxModuleContext ->
        xModuleContextStatic.
        pucLogSostOriginBufferPointer;

    pxModuleContext[1].
    xModuleContextStatic.
    pucStatAiBufferPointer =
        pxModuleContext ->
        xModuleContextStatic.
        pucStatAiBufferPointer;

    pxModuleContext[1].
    xModuleContextStatic.
    pucBadAiOriginBufferPointer =
        pxModuleContext ->
        xModuleContextStatic.
        pucBadAiOriginBufferPointer;

    pxModuleContext[1].
    xModuleContextStatic.
    pucBadAiBufferPointer =
        pxModuleContext ->
        xModuleContextStatic.
        pucBadAiBufferPointer;

    pxModuleContext[1].
    xModuleContextStatic.
    pucCurrentTimeBufferPointer =
        pxModuleContext ->
        xModuleContextStatic.
        pucCurrentTimeBufferPointer;

//    pxModuleContext[1].
//    xModuleContextStatic.
//    pucAlarmEventsLog =
//        pxModuleContext ->
//        xModuleContextStatic.
//        pucAlarmEventsLog;
//
//    pxModuleContext[1].
//    xModuleContextStatic.
//    pucArchiveEventsLog =
//        pxModuleContext ->
//        xModuleContextStatic.
//        pucArchiveEventsLog;
//
//    pxModuleContext[1].
//    xModuleContextStatic.
//    pucErrorLog =
//        pxModuleContext ->
//        xModuleContextStatic.
//        pucErrorLog;

    pxModuleContext[1].
    xModuleContextStatic.
    pucAnalogueInputDescriptionWork =
        pxModuleContext ->
        xModuleContextStatic.
        pucAnalogueInputDescriptionWork;

    pxModuleContext[1].
    xModuleContextStatic.
    pucDimentionsParametersDataBase =
        pxModuleContext ->
        xModuleContextStatic.
        pucDimentionsParametersDataBase;

    pxModuleContext[1].
    xModuleContextStatic.
    pucModuleBadStateBufferPointer =
        pxModuleContext ->
        xModuleContextStatic.
        pucModuleBadStateBufferPointer;

    pxModuleContext[1].
    xModuleContextStatic.
    pucAinOffBufferPointer =
        pxModuleContext ->
        xModuleContextStatic.
        pucAinOffBufferPointer;

    pxModuleContext[1].
    xModuleContextStatic.
    pucDiValueOriginBufferPointer =
        pxModuleContext ->
        xModuleContextStatic.
        pucDiValueOriginBufferPointer;

    pxModuleContext[1].
    xModuleContextStatic.
    pucContStBufferPointer =
        pxModuleContext ->
        xModuleContextStatic.
        pucContStBufferPointer;

    pxModuleContext[1].
    xModuleContextStatic.
    pucContStWriteBufferPointer =
        pxModuleContext ->
        xModuleContextStatic.
        pucContStWriteBufferPointer;

    pxModuleContext[1].
    xModuleContextStatic.
    pucBadDoBufferPointer =
        pxModuleContext ->
        xModuleContextStatic.
        pucBadDoBufferPointer;

    pxModuleContext[1].
    xModuleContextStatic.
    pucStatDoBufferPointer =
        pxModuleContext ->
        xModuleContextStatic.
        pucStatDoBufferPointer;

    pxModuleContext[1].
    xModuleContextStatic.
    pucBadAoBufferPointer =
        pxModuleContext ->
        xModuleContextStatic.
        pucBadAoBufferPointer;

    pxModuleContext[1].
    xModuleContextStatic.
    pucDoValueBlockBufferPointer =
        pxModuleContext ->
        xModuleContextStatic.
        pucDoValueBlockBufferPointer;

    pxModuleContext[1].
    xModuleContextStatic.
    pucTxsBufferPointer =
        pxModuleContext ->
        xModuleContextStatic.
        pucTxsBufferPointer;

    pxModuleContext[1].
    xModuleContextStatic.
    pucReperPointsAdcBufferPointer =
        pxModuleContext ->
        xModuleContextStatic.
        pucReperPointsAdcBufferPointer;

    pxModuleContext[1].
    xModuleContextStatic.
    pucDataBasePointer =
        pxModuleContext ->
        xModuleContextStatic.
        pucDataBasePointer;


    pxModuleContext[1].
    xModuleContextStatic.
    pucDataBaseExtractPointer =
        pxModuleContext ->
        xModuleContextStatic.
        pucDataBaseExtractPointer;
}

//-----------------------------------------------------------------------------------------------------
// заполняет контекст для всех модулей. выделяет память в массивах "Modbus",
// связывает модуль с блоком базы данных.
void vMainTaskMemoryMapFormation(void)
{
    unsigned char nucIndexNumber;
    int i;
    unsigned char ucTypeCompare;
    unsigned char nucInsideTypeIndexCounter;
    TMbmFunction1234 *pxFunction1234Pointer;
    TMbmFunction15 *pxFunction15Pointer;

    // получим указатель на массив с нормализованной базой данных.
    pxFunction1234Pointer =
        axModbusMasterFunction1234DescriptionWork;
    pxFunction15Pointer = (TMbmFunction15*)&(xPlcDataBase.
                          axPlcDataBaseBlocks[MBM_FUNCTION_15_BLOCK_DATA_BASE_BLOCK_OFFSET].
                          aucPlcDataBaseBlockData[0]);

    memset(xAllModulesContext.axAllModulesContext,
           0,
           sizeof(xAllModulesContext.axAllModulesContext));
    nucIndexNumber = 0;
    ucTypeCompare = 0;
    nucInsideTypeIndexCounter = 0;

    // заполним указатели всех модулей, начальными адресами в массивах "Modbus".
    for (i = 0; i < MAX_MODULE_QUANTITY; i++)
    {
        vMainTaskMemoryMapModuleContextInitialisation(&xAllModulesContext.axAllModulesContext[i]);
        // присвоим индекс в массиве с объектами контекста всех модулей, каждому модулю.
        xAllModulesContext.axAllModulesContext[i].
        xModuleContextStatic.
        ucModuleContextIndex = i;
        // следующий модуль.
    }

    // внутренние модули.
    // заполним поля: тип модуля и адрес SPI.
    nucIndexNumber = 0;
//    for (int i = 0;
//            i <
//            (xPlcConfigService.xPlcConfigServiceData.ucInternalModulesQuantity);
//            i++)
    for (int i = 0;
            i <
            (INTERNAL_MODULE_QUANTITY);
            i++)
    {
//        xAllModulesContext.axAllModulesContext[nucIndexNumber].
//        xModuleContextStatic.
//        ucModuleAddress = nucIndexNumber;
//
//        xAllModulesContext.axAllModulesContext[nucIndexNumber].
//        xModuleContextStatic.
//        ucModuleType =
//            xPlcConfigDataTemp.xConfigData.aucModuleType[nucIndexNumber];

        // по индексу-адресу i в конфигурации есть модуль?
        if (xPlcConfigSearch.axConfigSearch[i].ui8Type)
        {
//
//            printf("vMainTaskMemoryMapFormation i %d\n\r", (i));
//            printf("vMainTaskMemoryMapFormation nucIndexNumber %d\n\r", (nucIndexNumber));
//            printf("vMainTaskMemoryMapFormation ui8Address %d\n\r", (xPlcConfigSearch.axConfigSearch[i].ui8Address));
//            printf("vMainTaskMemoryMapFormation ui8Type %d\n\r", (xPlcConfigSearch.axConfigSearch[i].ui8Type));

            xAllModulesContext.axAllModulesContext[nucIndexNumber].
            xModuleContextStatic.
            ucModuleAddress =
                (xPlcConfigSearch.axConfigSearch[i].ui8Address);

            xAllModulesContext.axAllModulesContext[nucIndexNumber].
            xModuleContextStatic.
            ucModuleType =
                (xPlcConfigSearch.axConfigSearch[i].ui8Type);
            // следующий модуль.
            nucIndexNumber++;
        }
    }

// внешние модули.
    // заполним поля тип модуля, адрес Modbus, формат данных и порядок байтов.
    for (int i = 0,
            j = 0;
            i < (xPlcConfigService.xPlcConfigServiceData.ucExternalModulesQuantity);
            i++)
    {
        // сначала заполним поля модулей функций Modbus 1,2,3,4.
        // модуль функций Modbus 1,2,3,4 есть в текущей конфигурации?
        if (pxFunction1234Pointer[i].
                ucModbusFunction)
        {
            // от 0 до 14 идут типы модулей производства ЗАО НПП "Центравтоматика".
            // они обрабатываются драйверами определёнными для этих типов.
            // 15 означает другие типы модулей(сторонних производителей).
            // они обрабатываются драйверами определёнными для функций Modbus 1,2,3,4,15.
            // тип текущего модуля обрабатывается драйвером определённым для функций Modbus 1,2,3,4,15?
            if (((pxFunction1234Pointer[i].
                    ucTust) & TUST_MODULE_TYPE_MASK) ==
                    EXTERNAL_MODULE_TYPE_OTHER)
            {
                // тип текущего модуля обрабатывается драйвером определённым для функций Modbus 1,2,3,4,15.
                xAllModulesContext.axAllModulesContext[nucIndexNumber].
                xModuleContextStatic.
                ucModuleAddress =
                    pxFunction1234Pointer[i].
                    ucModbusSlaveAddress;
                // установим тип текущего модуля по определённой для его обработки функции Modbus.
                // типы модулей от 0 до 15 заняты внутренними модулями.
                // внешние модули производства ЗАО НПП "Центравтоматика" тоже имеют типы от 0 до 15.
                // чтобы внутренние и внешние модули производства ЗАО НПП "Центравтоматика" не пересекались,
                // к типу внешних модулей производства ЗАО НПП "Центравтоматика" будем прибавлять смещение
                // на количество типов внутренних модулей(EXTERNAL_MODULE_TYPE_OFFSET).
                // внешние модули сторонних производителей тоже имеют типы от 0 до 15.
                // чтобы внешние модули производства ЗАО НПП "Центравтоматика" и
                // модули сторонних производителей не пересекались,
                // к типу внешних модулей сторонних производителей будем прибавлять смещение
                // на типы внешних модулей сторонних производителей(EXTERNAL_MODULE_OTHER_TYPE_OFFSET).

                switch (pxFunction1234Pointer[i].
                        ucModbusFunction)
                {
                case 1:
                    xAllModulesContext.axAllModulesContext[nucIndexNumber].
                    xModuleContextStatic.
                    ucModuleType =
                        OTHER_MODULE_TYPE_FUNCTION_1;
                    break;

                case 2:
                    xAllModulesContext.axAllModulesContext[nucIndexNumber].
                    xModuleContextStatic.
                    ucModuleType =
                        OTHER_MODULE_TYPE_FUNCTION_2;
                    break;

                case 3:
                    xAllModulesContext.axAllModulesContext[nucIndexNumber].
                    xModuleContextStatic.
                    ucModuleType =
                        OTHER_MODULE_TYPE_FUNCTION_3;
                    break;

                case 4:
                    xAllModulesContext.axAllModulesContext[nucIndexNumber].
                    xModuleContextStatic.
                    ucModuleType =
                        OTHER_MODULE_TYPE_FUNCTION_4;
                    break;

                default:
                    break;
                }
            }
            else
            {
                // тип текущего модуля обрабатывается драйвером определённым
                // для модулей производства ЗАО НПП "Центравтоматика".
                xAllModulesContext.axAllModulesContext[nucIndexNumber].
                xModuleContextStatic.
                ucModuleAddress =
                    pxFunction1234Pointer[i].
                    ucModbusSlaveAddress;
                // типы модулей от 0 до 15 заняты внутренними модулями.
                // внешние модули производства ЗАО НПП "Центравтоматика" тоже имеют типы от 0 до 15.
                // чтобы внутренние и внешние модули производства ЗАО НПП "Центравтоматика" не пересекались,
                // к типу внешних модулей производства ЗАО НПП "Центравтоматика" будем прибавлять смещение
                // на количество типов внутренних модулей(EXTERNAL_MODULE_TYPE_OFFSET).

                switch (pxFunction1234Pointer[i].
                        ucModbusFunction)
                {
                case 1:
                    xAllModulesContext.axAllModulesContext[nucIndexNumber].
                    xModuleContextStatic.
                    ucModuleType =
                        OWN_MODULE_TYPE_FUNCTION_1;
                    break;

                case 2:
                    xAllModulesContext.axAllModulesContext[nucIndexNumber].
                    xModuleContextStatic.
                    ucModuleType =
                        OWN_MODULE_TYPE_FUNCTION_2;
                    break;

                case 3:
                    xAllModulesContext.axAllModulesContext[nucIndexNumber].
                    xModuleContextStatic.
                    ucModuleType =
                        OWN_MODULE_TYPE_FUNCTION_3;
                    break;

                case 4:
                    xAllModulesContext.axAllModulesContext[nucIndexNumber].
                    xModuleContextStatic.
                    ucModuleType =
                        OWN_MODULE_TYPE_FUNCTION_4;
                    break;

                default:
                    break;
                }
            }
        }
        // теперь заполним поля модулей функции Modbus 15.
        // модуль функции Modbus 15 есть в текущей конфигурации?
        else if (pxFunction15Pointer[j].
                 ucModbusFunction)
        {
            // тип текущего модуля обрабатывается драйвером определённым для функций Modbus 1,2,3,4,15.
            xAllModulesContext.axAllModulesContext[nucIndexNumber].
            xModuleContextStatic.
            ucModuleAddress =
                pxFunction15Pointer[j].
                ucModbusSlaveAddress;

            xAllModulesContext.axAllModulesContext[nucIndexNumber].
            xModuleContextStatic.
            ucModuleType =
                OTHER_MODULE_TYPE_FUNCTION_15;
            j++;
        }

        nucIndexNumber++;
    }

    // вычислим индекс модуля, внутри однородной группы своего типа.
    // будет необходим для вычисления индекса модуля, для доступа к нему.
    nucIndexNumber = 0;
    ucTypeCompare = 0;
    for (i = 0; i < MAX_MODULE_QUANTITY; i++)
    {
        switch(xAllModulesContext.axAllModulesContext[nucIndexNumber].
                xModuleContextStatic.
                ucModuleType)
        {
        case MODULE_TYPE_MVDI:
        case MODULE_TYPE_MVDS9:
            if (ucTypeCompare == MODULE_TYPE_MVDI)
            {
                nucInsideTypeIndexCounter++;
            }
            else
            {
                nucInsideTypeIndexCounter = 0;
            }
            ucTypeCompare = MODULE_TYPE_MVDI;

            xAllModulesContext.axAllModulesContext[nucIndexNumber].
            xModuleContextStatic.
            ucInsideTypeIndex = nucInsideTypeIndexCounter;
            break;

        case MODULE_TYPE_MVPS3:
        case MODULE_TYPE_MVAI5:
        case MODULE_TYPE_MVST3:
        case MODULE_TYPE_MVAO3:
            if (ucTypeCompare == ANOLOGUE_INPUT_MODULE_GROUP)
            {
                nucInsideTypeIndexCounter++;
            }
            else
            {
                nucInsideTypeIndexCounter = 0;
            }
            ucTypeCompare = ANOLOGUE_INPUT_MODULE_GROUP;

            xAllModulesContext.axAllModulesContext[nucIndexNumber].
            xModuleContextStatic.
            ucInsideTypeIndex = nucInsideTypeIndexCounter;
            break;

        case MODULE_TYPE_MR53:
        case MODULE_TYPE_MR54:
        case MODULE_TYPE_MR55:
        case MODULE_TYPE_MR51:
            if (ucTypeCompare == DISCRETE_OUTPUT_MODULE_GROUP)
            {
                nucInsideTypeIndexCounter++;
            }
            else
            {
                nucInsideTypeIndexCounter = 0;
            }
            ucTypeCompare = DISCRETE_OUTPUT_MODULE_GROUP;

            xAllModulesContext.axAllModulesContext[nucIndexNumber].
            xModuleContextStatic.
            ucInsideTypeIndex = nucInsideTypeIndexCounter;
            break;

        case MODULE_TYPE_MTVI5:
            if (ucTypeCompare == CURRENT_OUTPUT_MODULE_GROUP)
            {
                nucInsideTypeIndexCounter++;
            }
            else
            {
                nucInsideTypeIndexCounter = 0;
            }
            ucTypeCompare = CURRENT_OUTPUT_MODULE_GROUP;

            xAllModulesContext.axAllModulesContext[nucIndexNumber].
            xModuleContextStatic.
            ucInsideTypeIndex = nucInsideTypeIndexCounter;
            break;

        case OWN_MODULE_TYPE_FUNCTION_1:
        case OWN_MODULE_TYPE_FUNCTION_2:
        case OWN_MODULE_TYPE_FUNCTION_3:
        case OWN_MODULE_TYPE_FUNCTION_4:
        case OTHER_MODULE_TYPE_FUNCTION_1:
        case OTHER_MODULE_TYPE_FUNCTION_2:
        case OTHER_MODULE_TYPE_FUNCTION_3:
        case OTHER_MODULE_TYPE_FUNCTION_4:
            if (ucTypeCompare == EXTERNAL_1234_MODULE_GROUP)
            {
                nucInsideTypeIndexCounter++;
            }
            else
            {
                nucInsideTypeIndexCounter = 0;
            }
            ucTypeCompare = EXTERNAL_1234_MODULE_GROUP;

            xAllModulesContext.axAllModulesContext[nucIndexNumber].
            xModuleContextStatic.
            ucInsideTypeIndex = nucInsideTypeIndexCounter;
            break;

        case OWN_MODULE_TYPE_FUNCTION_15:
        case OTHER_MODULE_TYPE_FUNCTION_15:
            if (ucTypeCompare == EXTERNAL_15_MODULE_GROUP)
            {
                nucInsideTypeIndexCounter++;
            }
            else
            {
                nucInsideTypeIndexCounter = 0;
            }
            ucTypeCompare = EXTERNAL_15_MODULE_GROUP;

            xAllModulesContext.axAllModulesContext[nucIndexNumber].
            xModuleContextStatic.
            ucInsideTypeIndex = nucInsideTypeIndexCounter;
            break;

        default:
            break;
        };

        nucIndexNumber++;
    }

// "нарежем" память для каждого модуля, в рабочих массивах "Modbus".
    nucIndexNumber = 0;
    for (i = 0; i < MAX_MODULE_QUANTITY; i++)
    {
        xAllModulesContext.axAllModulesContext[nucIndexNumber].
        xModuleContextDinamic.
        usAuxiliaryCounter = BAD_MODULE_CYCLE_COUNT_DEFAULT; //MRXX database error Debug

        xAllModulesContext.axAllModulesContext[nucIndexNumber].
        xModuleContextDinamic.
        nucBadAnswerCounter = BAD_MODULE_CYCLE_COUNT_DEFAULT;

        switch(xAllModulesContext.axAllModulesContext[nucIndexNumber].
                xModuleContextStatic.
                ucModuleType)
        {
        case MODULE_TYPE_MVDI:
            // не вышли за границы массива?
            if (nucIndexNumber < (MAX_MODULE_QUANTITY - 1))
            {
                vMainTaskMemoryMapToNextModule(&xAllModulesContext.axAllModulesContext[nucIndexNumber]);
                // увеличим адрес на длину данных, которые занимает конкретный модуль, и
                // отправим следующему модулю.
                xAllModulesContext.axAllModulesContext[nucIndexNumber + 1].
                xModuleContextStatic.
                pucDiValueBufferPointer =
                    xAllModulesContext.axAllModulesContext[nucIndexNumber].
                    xModuleContextStatic.
                    pucDiValueBufferPointer + MVDI5_DI_VALUE_BIT_ARRAY_LENGTH;

                xAllModulesContext.axAllModulesContext[nucIndexNumber + 1].
                xModuleContextStatic.
                pucBadDiBufferPointer =
                    xAllModulesContext.axAllModulesContext[nucIndexNumber].
                    xModuleContextStatic.
                    pucBadDiBufferPointer + MVDI5_BAD_DI_BIT_ARRAY_LENGTH;

                xAllModulesContext.axAllModulesContext[nucIndexNumber + 1].
                xModuleContextStatic.
                pucModuleBadStateBufferPointer =
                    xAllModulesContext.axAllModulesContext[nucIndexNumber].
                    xModuleContextStatic.
                    pucModuleBadStateBufferPointer + MVDI5_MODULE_BAD_BIT_ARRAY_LENGTH;
            }
            break;

        case MODULE_TYPE_MVDS9:
            // не вышли за границы массива?
            if (nucIndexNumber < (MAX_MODULE_QUANTITY - 1))
            {
                vMainTaskMemoryMapToNextModule(&xAllModulesContext.axAllModulesContext[nucIndexNumber]);
                // увеличим адрес на длину данных, которые занимает конкретный модуль, и
                // отправим следующему модулю.
                xAllModulesContext.axAllModulesContext[nucIndexNumber + 1].
                xModuleContextStatic.
                pucDiValueBufferPointer =
                    xAllModulesContext.axAllModulesContext[nucIndexNumber].
                    xModuleContextStatic.
                    pucDiValueBufferPointer + MVDS9_DI_VALUE_BIT_ARRAY_LENGTH;

                xAllModulesContext.axAllModulesContext[nucIndexNumber + 1].
                xModuleContextStatic.
                pucBadDiBufferPointer =
                    xAllModulesContext.axAllModulesContext[nucIndexNumber].
                    xModuleContextStatic.
                    pucBadDiBufferPointer + MVDS9_BAD_DI_BIT_ARRAY_LENGTH;

                xAllModulesContext.axAllModulesContext[nucIndexNumber + 1].
                xModuleContextStatic.
                pucModuleBadStateBufferPointer =
                    xAllModulesContext.axAllModulesContext[nucIndexNumber].
                    xModuleContextStatic.
                    pucModuleBadStateBufferPointer + MVDS9_MODULE_BAD_BIT_ARRAY_LENGTH;
            }
            break;

        case MODULE_TYPE_MVPS3:
        case MODULE_TYPE_MVAI5:
        case MODULE_TYPE_MVAO3:
            // не вышли за границы массива?
            if (nucIndexNumber < (MAX_MODULE_QUANTITY - 1))
            {
                vMainTaskMemoryMapToNextModule(&xAllModulesContext.axAllModulesContext[nucIndexNumber]);
                // увеличим адрес на длину данных, которые занимает конкретный модуль, и
                // отправим следующему модулю.
                xAllModulesContext.axAllModulesContext[nucIndexNumber + 1].
                xModuleContextStatic.
                pucDiValueBufferPointer =
                    xAllModulesContext.axAllModulesContext[nucIndexNumber].
                    xModuleContextStatic.
                    pucDiValueBufferPointer + MVAI5_DI_VALUE_BIT_ARRAY_LENGTH;

                xAllModulesContext.axAllModulesContext[nucIndexNumber + 1].
                xModuleContextStatic.
                pucBadDiBufferPointer =
                    xAllModulesContext.axAllModulesContext[nucIndexNumber].
                    xModuleContextStatic.
                    pucBadDiBufferPointer + MVAI5_BAD_DI_BIT_ARRAY_LENGTH;

                xAllModulesContext.axAllModulesContext[nucIndexNumber + 1].
                xModuleContextStatic.
                pucAiValueBufferPointer =
                    xAllModulesContext.axAllModulesContext[nucIndexNumber].
                    xModuleContextStatic.
                    pucAiValueBufferPointer + MVAI5_AI_VALUE_BYTE_ARRAY_LENGTH;

                xAllModulesContext.axAllModulesContext[nucIndexNumber + 1].
                xModuleContextStatic.
                pucStatAiBufferPointer =
                    xAllModulesContext.axAllModulesContext[nucIndexNumber].
                    xModuleContextStatic.
                    pucStatAiBufferPointer + MVAI5_STAT_AI_BYTE_ARRAY_LENGTH;

                xAllModulesContext.axAllModulesContext[nucIndexNumber + 1].
                xModuleContextStatic.
                pucBadAiBufferPointer =
                    xAllModulesContext.axAllModulesContext[nucIndexNumber].
                    xModuleContextStatic.
                    pucBadAiBufferPointer + MVAI5_BAD_AI_BIT_ARRAY_LENGTH;

                xAllModulesContext.axAllModulesContext[nucIndexNumber + 1].
                xModuleContextStatic.
                pucModuleBadStateBufferPointer =
                    xAllModulesContext.axAllModulesContext[nucIndexNumber].
                    xModuleContextStatic.
                    pucModuleBadStateBufferPointer + MVAI5_MODULE_BAD_BIT_ARRAY_LENGTH;

                xAllModulesContext.axAllModulesContext[nucIndexNumber + 1].
                xModuleContextStatic.
                pucAinOffBufferPointer =
                    xAllModulesContext.axAllModulesContext[nucIndexNumber].
                    xModuleContextStatic.
                    pucAinOffBufferPointer + MVAI5_AIN_OFF_BIT_ARRAY_LENGTH;

                xAllModulesContext.axAllModulesContext[nucIndexNumber + 1].
                xModuleContextStatic.
                pucReperPointsAdcBufferPointer =
                    xAllModulesContext.axAllModulesContext[nucIndexNumber].
                    xModuleContextStatic.
                    pucReperPointsAdcBufferPointer + ANALOGUE_INPUT_MODULE_REPER_POINTS_ADC_DATA_BASE_BLOCK_LENGTH;
            }
            break;

        case MODULE_TYPE_MVST3:
            // не вышли за границы массива?
            if (nucIndexNumber < (MAX_MODULE_QUANTITY - 1))
            {
                vMainTaskMemoryMapToNextModule(&xAllModulesContext.axAllModulesContext[nucIndexNumber]);
                // увеличим адрес на длину данных, которые занимает конкретный модуль, и
                // отправим следующему модулю.
                xAllModulesContext.axAllModulesContext[nucIndexNumber + 1].
                xModuleContextStatic.
                pucDiValueBufferPointer =
                    xAllModulesContext.axAllModulesContext[nucIndexNumber].
                    xModuleContextStatic.
                    pucDiValueBufferPointer + MVAI5_DI_VALUE_BIT_ARRAY_LENGTH;

                xAllModulesContext.axAllModulesContext[nucIndexNumber + 1].
                xModuleContextStatic.
                pucBadDiBufferPointer =
                    xAllModulesContext.axAllModulesContext[nucIndexNumber].
                    xModuleContextStatic.
                    pucBadDiBufferPointer + MVAI5_BAD_DI_BIT_ARRAY_LENGTH;

                xAllModulesContext.axAllModulesContext[nucIndexNumber + 1].
                xModuleContextStatic.
                pucAiValueBufferPointer =
                    xAllModulesContext.axAllModulesContext[nucIndexNumber].
                    xModuleContextStatic.
                    pucAiValueBufferPointer + MVAI5_AI_VALUE_BYTE_ARRAY_LENGTH;

                xAllModulesContext.axAllModulesContext[nucIndexNumber + 1].
                xModuleContextStatic.
                pucStatAiBufferPointer =
                    xAllModulesContext.axAllModulesContext[nucIndexNumber].
                    xModuleContextStatic.
                    pucStatAiBufferPointer + MVAI5_STAT_AI_BYTE_ARRAY_LENGTH;

                xAllModulesContext.axAllModulesContext[nucIndexNumber + 1].
                xModuleContextStatic.
                pucBadAiBufferPointer =
                    xAllModulesContext.axAllModulesContext[nucIndexNumber].
                    xModuleContextStatic.
                    pucBadAiBufferPointer + MVAI5_BAD_AI_BIT_ARRAY_LENGTH;

                xAllModulesContext.axAllModulesContext[nucIndexNumber + 1].
                xModuleContextStatic.
                pucModuleBadStateBufferPointer =
                    xAllModulesContext.axAllModulesContext[nucIndexNumber].
                    xModuleContextStatic.
                    pucModuleBadStateBufferPointer + MVAI5_MODULE_BAD_BIT_ARRAY_LENGTH;

                xAllModulesContext.axAllModulesContext[nucIndexNumber + 1].
                xModuleContextStatic.
                pucAinOffBufferPointer =
                    xAllModulesContext.axAllModulesContext[nucIndexNumber].
                    xModuleContextStatic.
                    pucAinOffBufferPointer + MVAI5_AIN_OFF_BIT_ARRAY_LENGTH;

                xAllModulesContext.axAllModulesContext[nucIndexNumber + 1].
                xModuleContextStatic.
                pucTxsBufferPointer =
                    xAllModulesContext.axAllModulesContext[nucIndexNumber].
                    xModuleContextStatic.
                    pucTxsBufferPointer + (sizeof(struct TTxsData));

                xAllModulesContext.axAllModulesContext[nucIndexNumber + 1].
                xModuleContextStatic.
                pucReperPointsAdcBufferPointer =
                    xAllModulesContext.axAllModulesContext[nucIndexNumber].
                    xModuleContextStatic.
                    pucReperPointsAdcBufferPointer + ANALOGUE_INPUT_MODULE_REPER_POINTS_ADC_DATA_BASE_BLOCK_LENGTH;
            }
            break;

        case MODULE_TYPE_MR53:
        case MODULE_TYPE_MR54:
        case MODULE_TYPE_MR55:
        case MODULE_TYPE_MR51:
            // не вышли за границы массива?
            if (nucIndexNumber < (MAX_MODULE_QUANTITY - 1))
            {
                vMainTaskMemoryMapToNextModule(&xAllModulesContext.axAllModulesContext[nucIndexNumber]);
                // увеличим адрес на длину данных, которые занимает конкретный модуль, и
                // отправим следующему модулю.
                xAllModulesContext.axAllModulesContext[nucIndexNumber + 1].
                xModuleContextStatic.
                pucDoValueBufferPointer =
                    xAllModulesContext.axAllModulesContext[nucIndexNumber].
                    xModuleContextStatic.
                    pucDoValueBufferPointer + MRXX_DO_VALUE_BIT_ARRAY_LENGTH;

                xAllModulesContext.axAllModulesContext[nucIndexNumber + 1].
                xModuleContextStatic.
                pucSostDoBufferPointer =
                    xAllModulesContext.axAllModulesContext[nucIndexNumber].
                    xModuleContextStatic.
                    pucSostDoBufferPointer + MRXX_SOST_DO_BIT_ARRAY_LENGTH;

                xAllModulesContext.axAllModulesContext[nucIndexNumber + 1].
                xModuleContextStatic.
                pucModuleBadStateBufferPointer =
                    xAllModulesContext.axAllModulesContext[nucIndexNumber].
                    xModuleContextStatic.
                    pucModuleBadStateBufferPointer + MRXX_MODULE_BAD_BIT_ARRAY_LENGTH;

                xAllModulesContext.axAllModulesContext[nucIndexNumber + 1].
                xModuleContextStatic.
                pucBadDoBufferPointer =
                    xAllModulesContext.axAllModulesContext[nucIndexNumber].
                    xModuleContextStatic.
                    pucBadDoBufferPointer + MRXX_BAD_DO_BIT_ARRAY_LENGTH;

                xAllModulesContext.axAllModulesContext[nucIndexNumber + 1].
                xModuleContextStatic.
                pucStatDoBufferPointer =
                    xAllModulesContext.axAllModulesContext[nucIndexNumber].
                    xModuleContextStatic.
                    pucStatDoBufferPointer + MRXX_STAT_DO_BIT_ARRAY_LENGTH;

                xAllModulesContext.axAllModulesContext[nucIndexNumber + 1].
                xModuleContextStatic.
                pucDoValueBlockBufferPointer =
                    xAllModulesContext.axAllModulesContext[nucIndexNumber].
                    xModuleContextStatic.
                    pucDoValueBlockBufferPointer + MRXX_DO_VALUE_BIT_ARRAY_LENGTH;
            }
            break;

        case MODULE_TYPE_MTVI5:
            // не вышли за границы массива?
            if (nucIndexNumber < (MAX_MODULE_QUANTITY - 1))
            {
                vMainTaskMemoryMapToNextModule(&xAllModulesContext.axAllModulesContext[nucIndexNumber]);
                // увеличим адрес на длину данных, которые занимает конкретный модуль, и
                // отправим следующему модулю.
                xAllModulesContext.axAllModulesContext[nucIndexNumber + 1].
                xModuleContextStatic.
                pucContAvBufferPointer =
                    xAllModulesContext.axAllModulesContext[nucIndexNumber].
                    xModuleContextStatic.
                    pucContAvBufferPointer +
                    (MTVI5_ANALOG_OUTPUT_QUANTITY * sizeof(struct TMtvi5ContAvData));

                xAllModulesContext.axAllModulesContext[nucIndexNumber + 1].
                xModuleContextStatic.
                pucModuleBadStateBufferPointer =
                    xAllModulesContext.axAllModulesContext[nucIndexNumber].
                    xModuleContextStatic.
                    pucModuleBadStateBufferPointer + MTVI5_MODULE_BAD_BIT_ARRAY_LENGTH;

                xAllModulesContext.axAllModulesContext[nucIndexNumber + 1].
                xModuleContextStatic.
                pucContStBufferPointer =
                    xAllModulesContext.axAllModulesContext[nucIndexNumber].
                    xModuleContextStatic.
                    pucContStBufferPointer + MTVI5_CONT_ST_BYTE_ARRAY_LENGTH;

                xAllModulesContext.axAllModulesContext[nucIndexNumber + 1].
                xModuleContextStatic.
                pucContStWriteBufferPointer =
                    xAllModulesContext.axAllModulesContext[nucIndexNumber].
                    xModuleContextStatic.
                    pucContStWriteBufferPointer + MTVI5_CONT_ST_BYTE_ARRAY_LENGTH;

                xAllModulesContext.axAllModulesContext[nucIndexNumber + 1].
                xModuleContextStatic.
                pucBadAoBufferPointer =
                    xAllModulesContext.axAllModulesContext[nucIndexNumber].
                    xModuleContextStatic.
                    pucBadAoBufferPointer + BAD_AO_BIT_ARRAY_LENGTH;
            }
            break;

        case OWN_MODULE_TYPE_FUNCTION_1:
        case OWN_MODULE_TYPE_FUNCTION_2:
        case OTHER_MODULE_TYPE_FUNCTION_1:
        case OTHER_MODULE_TYPE_FUNCTION_2:
            // не вышли за границы массива?
            if (nucIndexNumber < (MAX_MODULE_QUANTITY - 1))
            {
                vMainTaskMemoryMapToNextModule(&xAllModulesContext.axAllModulesContext[nucIndexNumber]);
                // увеличим адрес на длину данных, которые занимает конкретный модуль, и
                // отправим следующему модулю.
                xAllModulesContext.axAllModulesContext[nucIndexNumber + 1].
                xModuleContextStatic.
                pucDiValueBufferPointer =
                    xAllModulesContext.axAllModulesContext[nucIndexNumber].
                    xModuleContextStatic.
                    pucDiValueBufferPointer +
                    (pxFunction1234Pointer[(xAllModulesContext.axAllModulesContext[nucIndexNumber].
                                            xModuleContextStatic.
                                            ucInsideTypeIndex)].
                     ucIOQuantity);

                xAllModulesContext.axAllModulesContext[nucIndexNumber + 1].
                xModuleContextStatic.
                pucBadDiBufferPointer =
                    xAllModulesContext.axAllModulesContext[nucIndexNumber].
                    xModuleContextStatic.
                    pucBadDiBufferPointer +
                    (pxFunction1234Pointer[(xAllModulesContext.axAllModulesContext[nucIndexNumber].
                                            xModuleContextStatic.
                                            ucInsideTypeIndex)].
                     ucIOQuantity);

                xAllModulesContext.axAllModulesContext[nucIndexNumber + 1].
                xModuleContextStatic.
                pucModuleBadStateBufferPointer =
                    xAllModulesContext.axAllModulesContext[nucIndexNumber].
                    xModuleContextStatic.
                    pucModuleBadStateBufferPointer + MBM_1234_MODULE_BAD_BIT_ARRAY_LENGTH;
            }
            break;

        case OWN_MODULE_TYPE_FUNCTION_3:
        case OWN_MODULE_TYPE_FUNCTION_4:
        case OTHER_MODULE_TYPE_FUNCTION_3:
        case OTHER_MODULE_TYPE_FUNCTION_4:
            // не вышли за границы массива?
            if (nucIndexNumber < (MAX_MODULE_QUANTITY - 1))
            {
                vMainTaskMemoryMapToNextModule(&xAllModulesContext.axAllModulesContext[nucIndexNumber]);
                // увеличим адрес на длину данных, которые занимает конкретный модуль, и
                // отправим следующему модулю.
                xAllModulesContext.axAllModulesContext[nucIndexNumber + 1].
                xModuleContextStatic.
                pucDiValueBufferPointer =
                    xAllModulesContext.axAllModulesContext[nucIndexNumber].
                    xModuleContextStatic.
                    pucDiValueBufferPointer +
                    (pxFunction1234Pointer[(xAllModulesContext.axAllModulesContext[nucIndexNumber].
                                            xModuleContextStatic.
                                            ucInsideTypeIndex)].
                     ucIOQuantity * ANALOGUE_INPUT_DI_VALUE_QUANTITY);

                xAllModulesContext.axAllModulesContext[nucIndexNumber + 1].
                xModuleContextStatic.
                pucBadDiBufferPointer =
                    xAllModulesContext.axAllModulesContext[nucIndexNumber].
                    xModuleContextStatic.
                    pucBadDiBufferPointer +
                    (pxFunction1234Pointer[(xAllModulesContext.axAllModulesContext[nucIndexNumber].
                                            xModuleContextStatic.
                                            ucInsideTypeIndex)].
                     ucIOQuantity * ANALOGUE_INPUT_DI_VALUE_QUANTITY);

                xAllModulesContext.axAllModulesContext[nucIndexNumber + 1].
                xModuleContextStatic.
                pucAiValueBufferPointer =
                    xAllModulesContext.axAllModulesContext[nucIndexNumber].
                    xModuleContextStatic.
                    pucAiValueBufferPointer +
                    (pxFunction1234Pointer[(xAllModulesContext.axAllModulesContext[nucIndexNumber].
                                            xModuleContextStatic.
                                            ucInsideTypeIndex)].
                     ucIOQuantity * sizeof(float));

                xAllModulesContext.axAllModulesContext[nucIndexNumber + 1].
                xModuleContextStatic.
                pucStatAiBufferPointer =
                    xAllModulesContext.axAllModulesContext[nucIndexNumber].
                    xModuleContextStatic.
                    pucStatAiBufferPointer +
                    ((pxFunction1234Pointer[(xAllModulesContext.axAllModulesContext[nucIndexNumber].
                                             xModuleContextStatic.
                                             ucInsideTypeIndex)].
                      ucIOQuantity));

                xAllModulesContext.axAllModulesContext[nucIndexNumber + 1].
                xModuleContextStatic.
                pucBadAiBufferPointer =
                    xAllModulesContext.axAllModulesContext[nucIndexNumber].
                    xModuleContextStatic.
                    pucBadAiBufferPointer +
                    (pxFunction1234Pointer[(xAllModulesContext.axAllModulesContext[nucIndexNumber].
                                            xModuleContextStatic.
                                            ucInsideTypeIndex)].
                     ucIOQuantity);

                xAllModulesContext.axAllModulesContext[nucIndexNumber + 1].
                xModuleContextStatic.
                pucModuleBadStateBufferPointer =
                    xAllModulesContext.axAllModulesContext[nucIndexNumber].
                    xModuleContextStatic.
                    pucModuleBadStateBufferPointer + MBM_1234_MODULE_BAD_BIT_ARRAY_LENGTH;
            }
            break;

        case OWN_MODULE_TYPE_FUNCTION_15:
        case OTHER_MODULE_TYPE_FUNCTION_15:
            // не вышли за границы массива?
            if (nucIndexNumber < (MAX_MODULE_QUANTITY - 1))
            {
                vMainTaskMemoryMapToNextModule(&xAllModulesContext.axAllModulesContext[nucIndexNumber]);
            }
            break;

        default:
            // не вышли за границы массива?
            if (nucIndexNumber < (MAX_MODULE_QUANTITY - 1))
            {
                vMainTaskMemoryMapToNextModule(&xAllModulesContext.axAllModulesContext[nucIndexNumber]);
            }
            break;
        };
        nucIndexNumber++;
    }

// привяжем модуль к блоку базы данных.
    nucIndexNumber = 0;
    for (i = 0; i < MAX_MODULE_QUANTITY; i++)
    {
        switch(xAllModulesContext.axAllModulesContext[nucIndexNumber].
                xModuleContextStatic.
                ucModuleType)
        {
        case MODULE_TYPE_MVDI:
            break;

        case MODULE_TYPE_MVDS9:
            xAllModulesContext.axAllModulesContext[nucIndexNumber].
            xModuleContextStatic.
            pucDataBasePointer =
                (&(xPlcDataBase.
                   axPlcDataBaseBlocks[DISCRETE_INPUT_MODULE_DATA_BASE_BLOCK_OFFSET].
                   aucPlcDataBaseBlockData[0]));
            xAllModulesContext.axAllModulesContext[nucIndexNumber].
            xModuleContextStatic.
            ucDataBaseDataOffset = ((xAllModulesContext.axAllModulesContext[nucIndexNumber].
                                     xModuleContextStatic.
                                     ucInsideTypeIndex) * sizeof(struct TMvds9ModuleDataBase));
            xPlcDataBaseControl.
            axPlcDataBaseBlockControl[DISCRETE_INPUT_MODULE_DATA_BASE_BLOCK_OFFSET].
            ucModuleType =
                xAllModulesContext.axAllModulesContext[nucIndexNumber].
                xModuleContextStatic.
                ucModuleType;
            xPlcDataBaseControl.
            axPlcDataBaseBlockControl[DISCRETE_INPUT_MODULE_DATA_BASE_BLOCK_OFFSET].
            ucModuleIndex = nucIndexNumber;
            break;

        case MODULE_TYPE_MVPS3:
        case MODULE_TYPE_MVAI5:
        case MODULE_TYPE_MVST3:
        case MODULE_TYPE_MVAO3:
            xAllModulesContext.axAllModulesContext[nucIndexNumber].
            xModuleContextStatic.
            pucDataBasePointer =
                &(xPlcDataBase.
                  axPlcDataBaseBlocks[ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_OFFSET +
                                      (xAllModulesContext.axAllModulesContext[nucIndexNumber].
                                       xModuleContextStatic.
                                       ucInsideTypeIndex)].
                  aucPlcDataBaseBlockData[0]);

            xAllModulesContext.axAllModulesContext[nucIndexNumber].
            xModuleContextStatic.
            ucDataBaseDataOffset = 0;

            xAllModulesContext.axAllModulesContext[nucIndexNumber].
            xModuleContextStatic.
            pucDataBaseExtractPointer =
                (unsigned char*)&axAnalogueInputDescriptionWork[(xAllModulesContext.axAllModulesContext[nucIndexNumber].
                        xModuleContextStatic.
                        ucInsideTypeIndex) *
                        MVAI5_ANALOG_INPUT_QUANTITY];

            xPlcDataBaseControl.
            axPlcDataBaseBlockControl[ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_OFFSET +
                                      (xAllModulesContext.axAllModulesContext[nucIndexNumber].
                                       xModuleContextStatic.
                                       ucInsideTypeIndex)].
            ucModuleType =
                xAllModulesContext.axAllModulesContext[nucIndexNumber].
                xModuleContextStatic.
                ucModuleType;

            xPlcDataBaseControl.
            axPlcDataBaseBlockControl[ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_OFFSET +
                                      (xAllModulesContext.axAllModulesContext[nucIndexNumber].
                                       xModuleContextStatic.
                                       ucInsideTypeIndex)].
            ucModuleIndex = nucIndexNumber;

            xAllModulesContext.axAllModulesContext[nucIndexNumber].
            xModuleContextDinamic.
            ucFsmState = MVAI5_DATA_EXCHANGE;
            break;

        case MODULE_TYPE_MR51:
        case MODULE_TYPE_MR53:
        case MODULE_TYPE_MR54:
        case MODULE_TYPE_MR55:
            xAllModulesContext.axAllModulesContext[nucIndexNumber].
            xModuleContextStatic.
            pucDataBasePointer =
                (&(xPlcDataBase.
                   axPlcDataBaseBlocks[DISCRETE_OUTPUT_MODULE_DATA_BASE_BLOCK_OFFSET].
                   aucPlcDataBaseBlockData[0]));

            xAllModulesContext.axAllModulesContext[nucIndexNumber].
            xModuleContextStatic.
            ucDataBaseDataOffset =
                ((xAllModulesContext.axAllModulesContext[nucIndexNumber].
                  xModuleContextStatic.
                  ucInsideTypeIndex) *
                 (sizeof(struct TMrXXOneChannelDataBase) *
                  MRXX_DISCRETE_OUTPUT_QUANTITY));

            xAllModulesContext.axAllModulesContext[nucIndexNumber].
            xModuleContextDinamic.
            ucFsmState = MRXX_CHANGE_PROTOCOL;//MRXX_FSM_STATE_CHANGE_PROTOCOL;//MRXX_FSM_STATE_WRITE_DATABASE; // при старте база данных записывается из прибора в модуль реле.
            // установим начальное состояние автомата - записать базу данных в модуль.
            break;

        case MODULE_TYPE_MTVI5:
            xAllModulesContext.axAllModulesContext[nucIndexNumber].
            xModuleContextStatic.
            pucDataBasePointer =
                &(xPlcDataBase.
                  axPlcDataBaseBlocks[CURRENT_OUTPUT_MODULE_REGULATOR_DATA_BASE_BLOCK_OFFSET +
                                      (xAllModulesContext.axAllModulesContext[nucIndexNumber].
                                       xModuleContextStatic.
                                       ucInsideTypeIndex)].
                  aucPlcDataBaseBlockData[0]);

            xAllModulesContext.axAllModulesContext[nucIndexNumber].
            xModuleContextStatic.
            ucDataBaseDataOffset = 0;

            xAllModulesContext.axAllModulesContext[nucIndexNumber].
            xModuleContextStatic.
            pucDataBaseExtractPointer =
                (unsigned char*)&axAnalogueOutputRegulatorDescriptionWork[(xAllModulesContext.axAllModulesContext[nucIndexNumber].
                        xModuleContextStatic.
                        ucInsideTypeIndex) *
                        MTVI5_ANALOG_OUTPUT_QUANTITY];

            xPlcDataBaseControl.
            axPlcDataBaseBlockControl[CURRENT_OUTPUT_MODULE_REGULATOR_DATA_BASE_BLOCK_OFFSET +
                                      (xAllModulesContext.axAllModulesContext[nucIndexNumber].
                                       xModuleContextStatic.
                                       ucInsideTypeIndex)].
            ucModuleType =
                xAllModulesContext.axAllModulesContext[nucIndexNumber].
                xModuleContextStatic.
                ucModuleType;

            xPlcDataBaseControl.
            axPlcDataBaseBlockControl[CURRENT_OUTPUT_MODULE_REGULATOR_DATA_BASE_BLOCK_OFFSET +
                                      (xAllModulesContext.axAllModulesContext[nucIndexNumber].
                                       xModuleContextStatic.
                                       ucInsideTypeIndex)].
            ucModuleIndex = nucIndexNumber;
            break;

        case OWN_MODULE_TYPE_FUNCTION_1:
        case OWN_MODULE_TYPE_FUNCTION_2:
        case OWN_MODULE_TYPE_FUNCTION_3:
        case OWN_MODULE_TYPE_FUNCTION_4:
        case OTHER_MODULE_TYPE_FUNCTION_1:
        case OTHER_MODULE_TYPE_FUNCTION_2:
        case OTHER_MODULE_TYPE_FUNCTION_3:
        case OTHER_MODULE_TYPE_FUNCTION_4:
            xAllModulesContext.axAllModulesContext[nucIndexNumber].
            xModuleContextStatic.
            pucDataBasePointer =
                (unsigned char*)&axModbusMasterFunction1234DescriptionWork;
            xAllModulesContext.axAllModulesContext[nucIndexNumber].
            xModuleContextStatic.
            ucDataBaseDataOffset =
                ((xAllModulesContext.axAllModulesContext[nucIndexNumber].
                  xModuleContextStatic.
                  ucInsideTypeIndex) * sizeof(struct TMbmFunction1234));
            break;

        case OWN_MODULE_TYPE_FUNCTION_15:
        case OTHER_MODULE_TYPE_FUNCTION_15:
            xAllModulesContext.axAllModulesContext[nucIndexNumber].
            xModuleContextStatic.
            pucDataBasePointer =
                (&(xPlcDataBase.
                   axPlcDataBaseBlocks[MBM_FUNCTION_15_BLOCK_DATA_BASE_BLOCK_OFFSET].
                   aucPlcDataBaseBlockData[0]));
            xAllModulesContext.axAllModulesContext[nucIndexNumber].
            xModuleContextStatic.
            ucDataBaseDataOffset =
                ((xAllModulesContext.axAllModulesContext[nucIndexNumber].
                  xModuleContextStatic.
                  ucInsideTypeIndex) * sizeof(struct TMbmFunction15));
            break;

        default:
            break;
        };
        nucIndexNumber++;
    }

    // вычислим контрольную сумму неизменяемой части контекста. потом будем проверять целостность перед каждым главным циклом.
    vModulesContextStaticCrcCreate(&xAllModulesContext);
    xPlcDataBaseControl.usCrc =
        usCrcSummTwoByteCalculation((unsigned char*)xPlcDataBaseControl.axPlcDataBaseBlockControl,
                                    (sizeof(xPlcDataBaseControl.axPlcDataBaseBlockControl)));
}

//-----------------------------------------------------------------------------------------------------
// обмен данными с CodeSys, через разделяемую память.
void vLinuxToCodeSysDataExchange(void)
{
    unsigned char *pucSource;
    unsigned char *pucDestination;
    int i;

    // CodeSys активен?
    // CodeSys получил данные на предыдущем шаге?
    if (pxDataExchangeControlBuff ->
            fbDataExchangeReady == FALSE)
    {
// CodeSysToLinux
// получим данные от CodeSys.
        // требование включения реле может постуать как от CodeSys, так и из другой части программы.
        // поэтому эти сигналы объединяются по логическому "или".
        pucSource = &(pxDataExchangeCodeSysToLinuxBuff ->
                      aucDoValue[0]);
        pucDestination = &aucDoValueBlockByteArray[0];
        for (i = 0; i < DO_VALUE_BIT_ARRAY_LENGTH; i++)
        {
//            pucDestination[i] |= pucSource[i];
            *pucDestination++ |= *pucSource++;
        }

        memcpy((unsigned char*)afCalcValue,
               &(pxDataExchangeCodeSysToLinuxBuff ->
                 afCalcValue[0]),
               CALC_CDS_FLOAT_ARRAY_LENGTH * sizeof(float));

        memcpy((unsigned char*)afSysValue,
               &(pxDataExchangeCodeSysToLinuxBuff ->
                 afSysValue[0]),
               SYS_CDS_FLOAT_ARRAY_LENGTH * sizeof(float));

        memcpy(&aucCoilsArray[LOG_SOST_BIT_ARRAY_OFFSET],
               &(pxDataExchangeCodeSysToLinuxBuff ->
                 aucLogSost[0]),
               LOG_SOST_BIT_ARRAY_LENGTH);



// LinuxToCodeSys
// передадим данные в CodeSys.
        memcpy(&(pxDataExchangeLinuxToCodeSysBuff ->
                 afAiValue[0]),
               (unsigned char*)afAnalogueInputData,
               sizeof(afAnalogueInputData));

        memcpy(&(pxDataExchangeLinuxToCodeSysBuff ->
                 afHandUst[0]),
               &aucHoldingRegistersArray[HAND_UST_BYTE_ARRAY_OFFSET],
               HAND_UST_FLOAT_ARRAY_LENGTH * sizeof(float));

        vLittleToBigEndianFloatConverter((unsigned char*)&(pxDataExchangeLinuxToCodeSysBuff ->
                                         afHandUst[0]),
                                         &aucHoldingRegistersArray[HAND_UST_BYTE_ARRAY_OFFSET],
                                         HAND_UST_FLOAT_ARRAY_LENGTH);

        memcpy(&(pxDataExchangeLinuxToCodeSysBuff ->
                 afContAv[0]),
               (unsigned char*)axAnalogueOutputRegulatorContAv,
               sizeof(axAnalogueOutputRegulatorContAv));

        memcpy(&(pxDataExchangeLinuxToCodeSysBuff ->
                 aucDiValue[0]),
               &aucDiscreteInputsArray[DI_VALUE_BIT_ARRAY_OFFSET],
               DI_VALUE_BIT_ARRAY_LENGTH);

        memcpy(&(pxDataExchangeLinuxToCodeSysBuff ->
                 aucSostDo[0]),
               &aucCoilsArray[SOST_DO_BIT_ARRAY_OFFSET],
               SOST_DO_BIT_ARRAY_LENGTH);

        memcpy(&(pxDataExchangeLinuxToCodeSysBuff ->
                 aucBadAi[0]),
               &aucCoilsArray[BAD_AI_BIT_ARRAY_OFFSET],
               BAD_AI_BIT_ARRAY_LENGTH);

        memcpy(&(pxDataExchangeLinuxToCodeSysBuff ->
                 aucBadDi[0]),
               &aucCoilsArray[BAD_DI_BIT_ARRAY_OFFSET],
               BAD_DI_BIT_ARRAY_LENGTH);

        memcpy(&(pxDataExchangeLinuxToCodeSysBuff ->
                 aucLogCom[0]),
               &aucCoilsArray[LOG_COM_BIT_ARRAY_OFFSET],
               LOG_COM_BIT_ARRAY_LENGTH);

        memcpy(&(pxDataExchangeLinuxToCodeSysBuff ->
                 aucFbOut[0]),
               &aucCoilsArray[CURRENT_STEP_FUNCTION_BLOCK_OUTS_BIT_ARRAY_OFFSET],
               CURRENT_STEP_FUNCTION_BLOCK_OUTS_BIT_ARRAY_LENGTH);

        vByteToBitArray(&aucCoilsArray[CONT_ST_BIT_ARRAY_OFFSET], // origin MTVI TempRepair.
                        &aucContStByteArray[0],
                        CONT_ST_BYTE_ARRAY_LENGTH);

        memcpy(&(pxDataExchangeLinuxToCodeSysBuff ->
                 aucContSt[0]),
               &aucCoilsArray[CONT_ST_BIT_ARRAY_OFFSET],
               CONT_ST_BIT_ARRAY_LENGTH);
//
//    memcpy(&(pxDataExchangeLinuxToCodeSysBuff ->
//             aucContSt[0]),
//           aucContStWriteBitArray,
//           CONT_ST_BIT_ARRAY_LENGTH);

        pxDataExchangeLinuxToCodeSysBuff ->
        fucGlobalKvit = fbGlobalKvit;
        pxDataExchangeLinuxToCodeSysBuff ->
        fucGlobalReset = fbGlobalReset;

// LinuxToCodeSys
// передадим данные для управления.
//        // конфигурация прибора: содержимое объекта xPlcConfigDataTemp.xConfigData, типа - TConfigData.
//        memcpy(&(pxDataExchangeControlBuff ->
//                 aucConfigData[0]),
//               &xPlcConfigDataTemp.xConfigData,
//               sizeof(struct TConfigData));

        // конфигурация прибора: содержимое объекта xPlcConfigDataTemp.xConfigData, типа - TConfigData.
        memcpy(&(pxDataExchangeControlBuff ->
                 xConfigData),
               &xPlcConfigDataTemp.xConfigData,
               sizeof(struct TConfigData));


        // текущее время.
//    struct tm *pxSource;
        uint8_t *pui8Destination;
//    // получим указатель на объект с текущим временем.
//    pxSource =
//        (struct tm*)&xCurrentTime;
        // получим указатель на буфер для передачи в CodeSys.
        pui8Destination = (pxDataExchangeControlBuff ->
                           aucCurrentTime);

//    // поместим текущее время в буфер для передачи в CodeSys.
//    pui8Destination[CURRENT_TIME_SECOND_OFFSET] = pxSource -> tm_sec;
//    pui8Destination[CURRENT_TIME_MINUTE_OFFSET] = pxSource -> tm_min;
//    pui8Destination[CURRENT_TIME_HOUR_OFFSET] = pxSource -> tm_hour;
//    pui8Destination[CURRENT_TIME_MONTH_DAY_OFFSET] = pxSource -> tm_mday;
//    pui8Destination[CURRENT_TIME_MONTH_OFFSET] = pxSource -> tm_mon;
//    pui8Destination[CURRENT_TIME_YEAR_OFFSET] = pxSource -> tm_year;
//    pui8Destination[CURRENT_TIME_WEEK_DAY_OFFSET] = pxSource -> tm_wday;

        // поместим текущее время в буфер для передачи в CodeSys.
        *pui8Destination++ = xCurrentTime.tm_sec;
        *pui8Destination++ = xCurrentTime.tm_min;
        *pui8Destination++ = xCurrentTime.tm_hour;
        *pui8Destination++ = xCurrentTime.tm_mday;
        *pui8Destination++ = ((xCurrentTime.tm_mon) + 1);
        *pui8Destination++ = ((xCurrentTime.tm_year) - 100);
        *pui8Destination++ = xCurrentTime.tm_wday;


//    // неактивное состояние события ещё не зарегистрировано?
//    if(xCInternalModuleErrorEvent.EventOffIsNotRegistered(
//                SYSTEM_ERROR_GROUP_OFFSET,
//                SYSTEM_ERROR_CODE_SYS_NOT_RESPONDED_OFFSET))
//    {
//        // зарегистрируем неактивное состояние события.
//        CEvents::EventRegistration(
//            SYSTEM_ERROR_GROUP_OFFSET,
//            (CEvents::HANDLED_EVENTS_SYSTEM_ERROR_TYPE |
//             CEvents::HANDLED_EVENTS_IS_ARCHIVE),
//            SYSTEM_ERROR_CODE_SYS_NOT_RESPONDED_OFFSET,
//            "Норма CodeSys");
//    }

        // данные для обмена готовы.
        pxDataExchangeControlBuff ->
        fbDataExchangeReady = TRUE;
    }
    else
    {
        // время ожидания обмена с CodeSys закончилось?
        if (ucCodeSysExchangeErrorCycleCounter >= 50)
        {

//    // активное состояние события ещё не зарегистрировано?
//    if(xCInternalModuleErrorEvent.EventOnIsNotRegistered(
//                SYSTEM_ERROR_GROUP_OFFSET,
//                SYSTEM_ERROR_CODE_SYS_NOT_RESPONDED_OFFSET))
//    {
//        // зарегистрируем активное состояние события.
//        CEvents::EventRegistration(
//            SYSTEM_ERROR_GROUP_OFFSET,
//            (CEvents::HANDLED_EVENTS_SYSTEM_ERROR_TYPE |
//             CEvents::HANDLED_EVENTS_IS_POPUP |
//             CEvents::HANDLED_EVENTS_IS_SOUND |
//             CEvents::HANDLED_EVENTS_IS_ARCHIVE),
//            SYSTEM_ERROR_CODE_SYS_NOT_RESPONDED_OFFSET,
//            "Ошибка CodeSys");
//    }

            // установим флаг - ошибка обмена данными с CodeSys.
            fbCodeSysDataExchangeError = 1;
            LOG_DBG << "CodeSysExchange Error!";
            // сбросим счётчик ошибок обмена с CodeSys.
            ucCodeSysExchangeErrorCycleCounter = 0;
            CodeSysStart();
        }
        else
        {
            // увеличим счётчик ошибок обмена с CodeSys.
            ucCodeSysExchangeErrorCycleCounter++;
        }
    }
}

//-----------------------------------------------------------------------------------------------------
// копирует данные из рабочих массивов, в массивы интерфейсов, для передачи по "Modbus".
void vInternalToModbusBuffDataExchange(void)
{
    int i;
    unsigned int nuiBusyTimeCounter;
    unsigned char *pucSource;
    unsigned char *pucDestination;

//    vByteToBitArray(&aucCoilsArray[STAT_AI_BIT_ARRAY_OFFSET],
//                    &aucStatAiByteArray[0],
//                    STAT_AI_BYTE_ARRAY_LENGTH);
    vByteToBitArray(&aucCoilsArray[CONT_ST_BIT_ARRAY_OFFSET], // origin MTVI TempRepair.
                    &aucContStByteArray[0],
                    CONT_ST_BYTE_ARRAY_LENGTH);
//
//    vByteToBitArray(&aucCoilsArray[CONT_ST_BIT_ARRAY_OFFSET],
//                    &aucContStWriteByteArray[0],
//                    CONT_ST_BYTE_ARRAY_LENGTH); // change MTVI TempRepair.

    vLittleToBigEndianFloatConverter(&aucHoldingRegistersArray[AI_VALUE_BIG_ENDIAN_BYTE_ARRAY_OFFSET],
                                     (unsigned char*)afAnalogueInputData,
                                     AI_VALUE_BIG_ENDIAN_FLOAT_ARRAY_LENGTH);

    vLittleToBigEndianFloatConverter(&aucHoldingRegistersArray[CALC_CDS_BYTE_ARRAY_OFFSET],
                                     (unsigned char*)afCalcValue,
                                     CALC_CDS_FLOAT_ARRAY_LENGTH);

    vAnalogueOutputContAvWorkToModbus();

    vConfigurationModbusRepresentDataPack((TConfigDataModbusRepresentPackOne*)&aucInputRegistersArray[MODIFICATION_CODE_BYTE_ARRAY_OFFSET],
                                          &xPlcConfigDataTemp.xConfigData);
    vByteToShortArray((unsigned short*)&aucInputRegistersArray[NET_ADDRESS_BYTE_ARRAY_OFFSET],
                      &ucModbusSlaveAddress,
                      NET_ADDRESS_BYTE_ARRAY_LENGTH);

#ifdef MODBUS_TCP_ENABLE
    pxCommonModbusMapping = pxTcpModbusMapping; // переключим контекст Modbus, на ModbusTCP.
    nuiBusyTimeCounter = MAX_MAIN_BUFFER_BUSY_WAITING_TIME; // буферы Modbus могут быть заняты. установим таймаут.
    while (1)
    {
        // буферы Modbus заняты?
        if (pxCommonModbusMapping->message_ready)
        {
            usleep(COMMON_DELAY_TIME);
            if (!nuiBusyTimeCounter--)
            {
                cout << "MAX_MAIN_BUFFER_BUSY_WAITING_TIME" << endl;
                break;
            }
        }
        else
        {
            pxCommonModbusMapping->message_ready = 1; // захватим буфер.

            /** INPUT COILS **/
            memcpy(&(pxCommonModbusMapping->tab_bits[0]),
                   &aucCoilsArray[0],
                   COILS_WORK_ARRAY_LENGTH);

            /** INPUT STATUS **/
            memcpy(&(pxCommonModbusMapping->tab_input_bits[0]),
                   &aucDiscreteInputsArray[0],
                   DISCRETE_INPUTS_ARRAY_LENGTH);

            /** INPUT REGISTERS **/
            vAnalogueInputToScalableCodeConverter();

            memcpy(&(pxCommonModbusMapping->tab_input_registers[0]),
                   &aucInputRegistersArray[0],
                   INPUT_REGISTERS_ARRAY_LENGTH);

            /** HOLDING REGISTERS **/
            memcpy(&(pxCommonModbusMapping->tab_registers[0]),
                   &aucHoldingRegistersArray[0],
                   HOLDING_REGISTERS_ARRAY_LENGTH);

            pxCommonModbusMapping->message_ready = 0; // освободим буфер.
            break;
        }
    }
#endif // MODBUS_TCP_ENABLE

#ifdef MODBUS_RTU_ENABLE
    pxCommonModbusMapping = pxRtuModbusMapping; // переключим контекст Modbus, на ModbusRTU.
    nuiBusyTimeCounter = MAX_MAIN_BUFFER_BUSY_WAITING_TIME; // буферы Modbus могут быть заняты. установим таймаут.
    while (1)
    {
        // буферы Modbus заняты?
        if (pxCommonModbusMapping->message_ready)
        {
            usleep(COMMON_DELAY_TIME);
            if (!nuiBusyTimeCounter--)
            {
                cout << "MAX_MAIN_BUFFER_BUSY_WAITING_TIME" << endl;
                break;
            }
        }
        else
        {
            pxCommonModbusMapping->message_ready = 1; // захватим буфер.

            /** INPUT COILS **/
            memcpy(&(pxCommonModbusMapping->tab_bits[0]),
                   &aucCoilsArray[0],
                   COILS_WORK_ARRAY_LENGTH);

            /** INPUT STATUS **/
            memcpy(&(pxCommonModbusMapping->tab_input_bits[0]),
                   &aucDiscreteInputsArray[0],
                   DISCRETE_INPUTS_ARRAY_LENGTH);

            /** INPUT REGISTERS **/
            vAnalogueInputToScalableCodeConverter();
            memcpy(&(pxCommonModbusMapping->tab_input_registers[0]),
                   &aucInputRegistersArray[0],
                   INPUT_REGISTERS_ARRAY_LENGTH);

            /** HOLDING REGISTERS **/
            memcpy(&(pxCommonModbusMapping->tab_registers[0]),
                   &aucHoldingRegistersArray[0],
                   HOLDING_REGISTERS_ARRAY_LENGTH);

            pxCommonModbusMapping->message_ready = 0; // освободим буфер.
            break;
        }
    }
#endif // MODBUS_RTU_ENABLE

#ifdef MODBUS_HMI_ENABLE
    pxCommonModbusMapping = pxHmiRtuModbusMapping; // переключим контекст Modbus, на ModbusRTU HMI.
    nuiBusyTimeCounter = MAX_MAIN_BUFFER_BUSY_WAITING_TIME; // буферы Modbus могут быть заняты. установим таймаут.
    while (1)
    {
        // буферы Modbus заняты?
        if (pxCommonModbusMapping->message_ready)
        {
            usleep(COMMON_DELAY_TIME);
            if (!nuiBusyTimeCounter--)
            {
                cout << "MAX_MAIN_BUFFER_BUSY_WAITING_TIME" << endl;
                break;
            }
        }
        else
        {
            pxCommonModbusMapping->message_ready = 1; // захватим буфер.

            /** INPUT COILS **/
            memcpy(&(pxCommonModbusMapping->tab_bits[0]),
                   &aucCoilsArray[0],
                   COILS_WORK_ARRAY_LENGTH);

            /** INPUT STATUS **/
            memcpy(&(pxCommonModbusMapping->tab_input_bits[0]),
                   &aucDiscreteInputsArray[0],
                   DISCRETE_INPUTS_ARRAY_LENGTH);

            /** INPUT REGISTERS **/
            vAnalogueInputToScalableCodeConverter();

            memcpy(&(pxCommonModbusMapping->tab_input_registers[0]),
                   &aucInputRegistersArray[0],
                   INPUT_REGISTERS_ARRAY_LENGTH);

            /** HOLDING REGISTERS **/
            memcpy(&(pxCommonModbusMapping->tab_registers[0]),
                   &aucHoldingRegistersArray[0],
                   HOLDING_REGISTERS_ARRAY_LENGTH);

            pxCommonModbusMapping->message_ready = 0; // освободим буфер.
            break;
        }
    }
#endif // MODBUS_HMI_ENABLE
}

//-----------------------------------------------------------------------------------------------------
void vGeneralTimeCounter(void)
{
    if (nucOneSecondCounter == ONE_SECOND_TIME)
    {
        nucOneSecondCounter = 0;
        fbOneSecondPassed = 1;
    }
    else
    {
        nucOneSecondCounter++;
        fbOneSecondPassed = 0;
    }

    if (nucTenSecondsCounter == TEN_SECOND_TIME)
    {
        nucTenSecondsCounter = 0;
        fbTenSecondsPassed = 1;
    }
    else
    {
        nucTenSecondsCounter++;
        fbTenSecondsPassed = 0;
    }
}

//-----------------------------------------------------------------------------------------------------
void vThreadsCreate(void)
{
// создадим контекст Modbus интерфейса(память под массивы Modbus, служебные переменные, флаги, и т.д.).
#ifdef MODBUS_MASTER_RTU_ENABLE
    iModbusRtuMasterSet();
#endif // MODBUS_MASTER_RTU_ENABLE
#ifdef MODBUS_MASTER_TCP_ENABLE
    iModbusTcpMasterSet();
#endif // MODBUS_MASTER_TCP_ENABLE

//-----------------------------------------------------------------------------------------------------
// создадим поток ModbusTcp, связь с верхним уровнем, Ethernet.
#ifdef MODBUS_TCP_ENABLE
// создадим контекст Modbus интерфейса(память под массивы Modbus, служебные переменные, флаги, и т.д.).
//    pthread_t xModbusTcp;      // this is our handle to the pthread
    // create thread, pass reference, addr of the function and data

    if (pthread_create(&xModbusTcp,
                       NULL,
                       thread_vModbusTcpHandler,
                       NULL))
    {
        cout << "Failed to create the thread_vModbusTcp" << endl;
        //return 1;
    }

    if (iHandlerCreate(SIGRTMIN,
                       &ModbusTcpAction,
                       ModbusTcpActionHandler) == - 1)
    {
        cout << "Failed to create the action ModbusTcp" << endl;
        //return 1;
    }

#endif // MODBUS_TCP_ENABLE

//-----------------------------------------------------------------------------------------------------
// создадим поток ModbusRtu, связь с верхним уровнем, RS-485.
#ifdef MODBUS_RTU_ENABLE
// создадим контекст Modbus интерфейса(память под массивы Modbus, служебные переменные, флаги, и т.д.).
//    pthread_t xModbusRtu;      // this is our handle to the pthread
    // create thread, pass reference, addr of the function and data

    if (pthread_create(&xModbusRtu,
                       NULL,
                       thread_vModbusRtuHandler,
                       NULL))
    {
        cout << "Failed to create the thread_vModbusRtu" << endl;
        //return 1;
    }

    if (iHandlerCreate(SIGRTMIN+1,
                       &ModbusRtuAction,
                       ModbusRtuActionHandler) == - 1)
    {
        cout << "Failed to create the action ModbusRtu" << endl;
        //return 1;
    }

#endif // MODBUS_RTU_ENABLE

//-----------------------------------------------------------------------------------------------------
// создадим поток HmiModbusRtu, связь с панелью HMI, RS-485.
#ifdef MODBUS_HMI_ENABLE
// создадим контекст Modbus интерфейса(память под массивы Modbus, служебные переменные, флаги, и т.д.).

//    pthread_t xHmiModbusRtu;      // this is our handle to the pthread
    // create thread, pass reference, addr of the function and data

    if (pthread_create(&xHmiModbusRtu,
                       NULL,
                       thread_vHmiModbusRtuHandler,
                       NULL))
    {
        cout << "Failed to create the thread_vHmiModbusRtuHandler" << endl;
        //return 1;
    }

    if (iHandlerCreate(SIGRTMIN+2,
                       &ModbusHmiAction,
                       ModbusHmiActionHandler) == - 1)
    {
        cout << "Failed to create the action ModbusHmi" << endl;
        //return 1;
    }


#endif // MODBUS_HMI_ENABLE

//-----------------------------------------------------------------------------------------------------
// создадим поток взаимодействия с модулями локальной сети нижнего уровня по Modbus.
//    pthread_t xExternalModuleInteractionCycle;      // this is our handle to the pthread
    // create thread, pass reference, addr of the function and data
    if (pthread_create(&xExternalModuleInteractionCycle,
                       NULL,
                       thread_vExternalModuleInteractionCycle,
                       NULL))
    {
        cout << "Failed to create the thread_vExternalModuleInteractionCycle" << endl;
        //return 1;
    }


//-----------------------------------------------------------------------------------------------------
    //    xMainDataAccessMutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_init(&xMainDataAccessMutex, NULL);

}


//-----------------------------------------------------------------------------------------------------
void vThreadsDestroy(void)
{

//-----------------------------------------------------------------------------------------------------
// создадим поток ModbusTcp, связь с верхним уровнем, Ethernet.
#ifdef MODBUS_TCP_ENABLE
// создадим контекст Modbus интерфейса(память под массивы Modbus, служебные переменные, флаги, и т.д.).

    /* проверяем, не завершился ли thread_child */
    if (pthread_kill(xModbusTcp, 0) != ESRCH)
    {
        /* посылаем thread_child служебный сигнал, который ее завершает */
        pthread_cancel(xModbusTcp);
        /* закрываем дескриптор thread_child */
        pthread_join(xModbusTcp, 0x00);
    }
    /* убираем за собой мусор */
    xModbusTcp = 0;

#endif // MODBUS_TCP_ENABLE

//-----------------------------------------------------------------------------------------------------
// создадим поток ModbusRtu, связь с верхним уровнем, RS-485.
#ifdef MODBUS_RTU_ENABLE
// создадим контекст Modbus интерфейса(память под массивы Modbus, служебные переменные, флаги, и т.д.).

    /* проверяем, не завершился ли thread_child */
    if (pthread_kill(xModbusRtu, 0) != ESRCH)
    {
        /* посылаем thread_child служебный сигнал, который ее завершает */
        pthread_cancel(xModbusRtu);
        /* закрываем дескриптор thread_child */
        pthread_join(xModbusRtu, 0x00);
    }
    /* убираем за собой мусор */
    xModbusRtu = 0;

#endif // MODBUS_RTU_ENABLE

//-----------------------------------------------------------------------------------------------------
// создадим поток HmiModbusRtu, связь с панелью HMI, RS-485.
#ifdef MODBUS_HMI_ENABLE
// создадим контекст Modbus интерфейса(память под массивы Modbus, служебные переменные, флаги, и т.д.).


    /* проверяем, не завершился ли thread_child */
    if (pthread_kill(xHmiModbusRtu, 0) != ESRCH)
    {
        /* посылаем thread_child служебный сигнал, который ее завершает */
        pthread_cancel(xHmiModbusRtu);
        /* закрываем дескриптор thread_child */
        pthread_join(xHmiModbusRtu, 0x00);
    }
    /* убираем за собой мусор */
    xHmiModbusRtu = 0;


#endif // MODBUS_HMI_ENABLE

//-----------------------------------------------------------------------------------------------------

    /* проверяем, не завершился ли thread_child */
    if (pthread_kill(xExternalModuleInteractionCycle, 0) != ESRCH)
    {
        /* посылаем thread_child служебный сигнал, который ее завершает */
        pthread_cancel(xExternalModuleInteractionCycle);
        /* закрываем дескриптор thread_child */
        pthread_join(xExternalModuleInteractionCycle, 0x00);
    }
    /* убираем за собой мусор */
    xExternalModuleInteractionCycle = 0;


//-----------------------------------------------------------------------------------------------------
    pthread_mutex_destroy(&xMainDataAccessMutex);

    munmap(pxDataExchangeLinuxToCodeSysBuff,
           sizeof(struct TDataExchangeLinuxToCodeSysBuff));
    munmap(pxDataExchangeCodeSysToLinuxBuff,
           sizeof(struct TDataExchangeCodeSysToLinuxBuff));
    munmap(pxDataExchangeControlBuff,
           sizeof(struct TDataExchangeControlBuff));

    xSpiCommunicationDevice.ChipSelectPinDelete();
    CPlatform::LedDestroy();
}

//-----------------------------------------------------------------------------------------------------
void SharedMemoryCreate(void)
{
    int fd;
// создадим контекст разделяемой памяти, для взаимодействия с CodeSys.
    // создадим и заполним массивы со строками - именами объектов разделяемой памяти.
    char sSharedMemNameControl[100] = "DataExchangeControlBuff";
    char sSharedMemNameLinuxToCodeSys[100] = "DataExchangeLinuxToCodeSysBuff";
    char sSharedMemNameCodeSysToLinux[100] = "DataExchangeCodeSysToLinuxBuff";

//-----------------------------------------------------------------------------------------------------
    fd = shm_open(sSharedMemNameLinuxToCodeSys,
                  O_CREAT | O_RDWR,
                  S_IRWXU | S_IRWXG);
    if(fd < 0)
    {
        printf("%s\n", strerror(errno));
    }
    printf("Shared Memory Open: %s %d\n",sSharedMemNameLinuxToCodeSys, fd);
    ftruncate(fd, sizeof(struct TDataExchangeLinuxToCodeSysBuff));
    pxDataExchangeLinuxToCodeSysBuff = (TDataExchangeLinuxToCodeSysBuff*)mmap(0,
                                       sizeof(struct TDataExchangeLinuxToCodeSysBuff),
                                       PROT_READ | PROT_WRITE, MAP_SHARED,
                                       fd,
                                       0);
    close(fd);

//-----------------------------------------------------------------------------------------------------
    fd = shm_open(sSharedMemNameCodeSysToLinux,
                  O_CREAT | O_RDWR,
                  S_IRWXU | S_IRWXG);
    if(fd < 0)
    {
        printf("%s\n", strerror(errno));
    }
    printf("Shared Memory Open: %s %d\n",sSharedMemNameCodeSysToLinux, fd);
    ftruncate(fd, sizeof(struct TDataExchangeCodeSysToLinuxBuff));
    pxDataExchangeCodeSysToLinuxBuff = (TDataExchangeCodeSysToLinuxBuff*)mmap(0,
                                       sizeof(struct TDataExchangeCodeSysToLinuxBuff),
                                       PROT_READ | PROT_WRITE, MAP_SHARED,
                                       fd,
                                       0);
    close(fd);

//-----------------------------------------------------------------------------------------------------
    fd = shm_open(sSharedMemNameControl,
                  O_CREAT | O_RDWR,
                  S_IRWXU | S_IRWXG);
    if(fd < 0)
    {
        printf("%s\n", strerror(errno));
    }
    printf("Shared Memory Open: %s %d\n",sSharedMemNameControl, fd);
    ftruncate(fd, sizeof(struct TDataExchangeControlBuff));
    pxDataExchangeControlBuff = (TDataExchangeControlBuff*)mmap(0,
                                sizeof(struct TDataExchangeControlBuff),
                                PROT_READ | PROT_WRITE, MAP_SHARED,
                                fd,
                                0);
    close(fd);

    // установим флаг - данные для обмена готовы.
    pxDataExchangeControlBuff ->
    fbDataExchangeReady = TRUE;
    // сбросим счётчик ошибок обмена с CodeSys.
    ucCodeSysExchangeErrorCycleCounter = 0;
}

struct timeval  xTimeCur;
struct timeval xTimeLast;
struct timeval  xTimeMax;
int iTimeMeasureCounter;

//// обработчик сигнала посылаемого из сценария копирования диска mmc0 на mmc1(MmcCopy.sh), после его окончания.
////-----------------------------------------------------------------------------------------------------
//void MainActionHandler(int signo, siginfo_t *info, void *context)
//{
//    ui8MainRestart = 1;
//    cout << "action MainRestart" << endl;
//}
//
////// обработчик сигнала посылаемого из сценария копирования диска mmc0 на mmc1(MmcCopy.sh), после его окончания.
//////-----------------------------------------------------------------------------------------------------
////void ArchiveSaveHandler(int signo, siginfo_t *info, void *context)
////{
////    for(int i = 0; i < 10; i++)
////    {
////        cout << "action ArchiveSaveHandler" << endl;
////        usleep(100000);
////    }
////}
//
//pthread_t xSystemDiskCopyDisplay;
////-----------------------------------------------------------------------------------------------------
//// поток копирования диска mmc0 на mmc1.
//void *thread_SystemDiskCopyDisplay(void *value)
//{
////    // удалим файлы настроек, конфигурации, базы данных.
////    system("sudo rm /home/debian/PasConfig.dat");
////    system("sudo rm /home/debian/ReservPasConfig.dat");
////    system("sudo rm /home/debian/PasDataBase.dat");
////    system("sudo rm /home/debian/ReservPasDataBase.dat");
////    system("sudo rm /home/debian/Pas05ArchiveDataBase.db");
////    system("sudo rm /home/debian/PasSettings.dat");
//    // запустим сценарий копирования диска mmc0 на mmc1(MmcCopy.sh).
//    // отправим команду.
//    system("sudo /home/debian/MmcCopy.sh");
//}

//    signal(SIGALRM, handler);
//pthread_kill(pthread_self(), sig);
//kill(getpid(),SIGINT);
//-----------------------------------------------------------------------------------------------------
// запускает CodeSys.
void CodeSysStart(void)
{
    return;
    // отправим команду.
    // запретим автостарт codesyscontrol.
    system("sudo sudo update-rc.d codesyscontrol remove");
    // отправим команду.
    // остановим codesyscontrol.
    system("sudo start-stop-daemon -Kvx /opt/codesys/bin/codesyscontrol.bin");
    // запретим автостарт codesysedge.
    system("sudo sudo update-rc.d codesysedge remove");
    // отправим команду.
    // остановим codesysedge.
    system("sudo start-stop-daemon -Kvx /opt/codesysedge/bin/codesysedge.bin");
    // запустим сценарий запуска codesys(codesyscontrol.sh).
    // отправим команду.
    usleep(5000000);
    system("sudo /home/debian/codesyscontrol.sh");
}

    CMainProductionCycle xMainProductionCycle;
//-----------------------------------------------------------------------------------------------------
int main(int argc, char *argv[])
{

    xMainProductionCycle.Run();

//    xTimeMax.tv_usec = 0;
//    iTimeMeasureCounter = 0;
//
////test-----------------------------------------------------------------------------------------------------
////    printf("__FUNCTION__ %s\n\r", (__FUNCTION__));
//
////    std::cout << __func__ << std::endl
////              << __FUNCTION__ << std::endl
////              << __PRETTY_FUNCTION__ << std::endl
////              << __FILE__ << std::endl
////              << __FILENAME__ << std::endl;
//
////    DEBUGLOGGING << __INFO__;
////    DEBUGLOGGING << 8;
//
////    LOG_FNC;
////    LOG_INF << "iTimeMeasureCounter " << iTimeMeasureCounter;
////    LOG_DBG << "xTimeMax.tv_usec " << xTimeMax.tv_usec;
//
//
////        xAllModulesContext.axAllModulesContext[nucIndexNumber].
////        xModuleContextDinamic.
////
////        xAllModulesContext.
////        axModuleContextStatic[nucIndexNumber + 1].
////    cout << "0x" << hex << uppercase << setw(2) << setfill('0') << (unsigned int)~0xE0 << " " << endl;
////        cout << "COILS_WORK_ARRAY_LENGTH" << endl;
////        cout << "0x" << hex << uppercase << setw(2) << setfill('0') << (unsigned int)(COILS_WORK_ARRAY_LENGTH) << " " << endl;
////        cout << "DISCRETE_INPUTS_ARRAY_LENGTH" << endl;
////        cout << "0x" << hex << uppercase << setw(2) << setfill('0') << (unsigned int)(DISCRETE_INPUTS_ARRAY_LENGTH) << " " << endl;
////        cout << "HOLDING_REGISTERS_ARRAY_LENGTH" << endl;
////        cout << "0x" << hex << uppercase << setw(2) << setfill('0') << (unsigned int)(HOLDING_REGISTERS_ARRAY_LENGTH) << " " << endl;
////        cout << "INPUT_REGISTERS_ARRAY_LENGTH" << endl;
////        cout << "0x" << hex << uppercase << setw(2) << setfill('0') << (unsigned int)(INPUT_REGISTERS_ARRAY_LENGTH) << " " << endl;
////    cout << "CURRENT_TIME_BYTE_ARRAY_OFFSET" << endl;
////    cout << "0x" << hex << uppercase << setw(2) << setfill('0') << (unsigned int)CURRENT_TIME_HOLDING_BYTE_ARRAY_OFFSET << " " << endl;
//////
////    cout << "sizeof" << endl;
////    cout << "0x" << hex << uppercase << setw(2) << setfill('0') << (unsigned int)(sizeof(uint16_t) +
////            sizeof(uint8_t) +
////            sizeof(struct tm)) << " " << endl;
////    cout << "0x" << hex << uppercase << setw(2) << setfill('0') << (unsigned int)(sizeof(CEvents::TEventDataPackOne)) << " " << endl;
////        cout << "sizeof" << endl;
////        cout << "0x" << hex << uppercase << setw(2) << setfill('0') << (unsigned int)sizeof(CEvents::axEventsControlContextSetting) << " " << endl;
//
//
////    signal(SIGALRM, handler);
////pthread_kill(pthread_self(), sig);
////kill(getpid(),SIGINT);
//
////    // создадим обработчик сигнала посылаемого из сценария копирования диска mmc0 на mmc1, после его окончания.
////    if (iHandlerCreate(SIGRTMIN+4,
////                       &ArchiveSave,
////                       ArchiveSaveHandler) == - 1)
////    {
////        cout << "Failed to create the action ArchiveSave" << endl;
////    }
////    usleep(100000);
////    cout << "pthread_kill" << endl;
//////    signal(SIGRTMIN+4, &ArchiveSaveHandler);
////    pthread_kill(pthread_self(), SIGRTMIN+4);
////
////    for(int i = 0; i < 10; i++)
////    {
////        cout << "iHandlerCreate" << endl;
////        usleep(100000);
////    }
//
//
//
//
////um, I think what you really want to do is like that:
////
////    bool received_data = false;
////    bool beExit = false;
////    struct recvPacket;
////
////    pthread_mutex_t recvMutex;
////
////    void main()
////    {
////        pthread_t thread1;
////        void *status;
////
////        pthread_mutex_init(&recvMutex, NULL);
////        pthread_create(&thread1, NULL, myFun,  (void*) NULL);
////
////        while(1)
////        {
////            if (received_data)
////            {
////                pthread_mutex_lock(&recvMutex);             // you should also synchronize received_data and beExit valuables, cuz they are shared by two threads
////                /* do what you want with recvPacket */
////                pthread_mutex_unlock(&recvMutex);
////
////                received_data == false;
////            }
////
////            /* do else you want, or you can let beExit = true to stop the thread */
////        }
////
////        if (err = pthread_join(thr_main, &status))
////            printf("pthread_join Error. %s\n", strerror(err)), exit(1);
////    }
////
////    void * myFun(void *ptr)
////    {
////        while (!beExit)
////        {
////            if (true == tryRecvPacket())
////            {
////                pthread_mutex_lock(&recvMutex);
////                /* fill data to recvPacket */
////                pthread_mutex_unlock(&recvMutex);
////                received_data = true;
////            }
////        }
////    }
//
//
//
//
//// для сериализации, разбора данных полученных по интерфейсам передачи данных.
////#pragma pack(push)
////#pragma pack(1)
////    struct TTestStruct
////    {
//////        unsigned char ucModulesQuantity;
////        uint16_t aucModuleType[2];
//////    unsigned char aucModuleType[INTERNAL_MODULE_QUANTITY]; // коды модулей ввода (вывода) по адресам SPI.
//////    unsigned char ucLastDiscreteInputModuleAddressPlusOne; // адрес последнего модуля дискретного ввода +1
//////    unsigned char ucLastAnalogueInputModuleAddressPlusOne; // адрес последнего модуля аналогового ввода +1
//////    unsigned char ucDiscreteOutputQuantity;
//////    uint16_t ui16DiscreteInputQuantity;
////        uint8_t ui32DiscreteInputQuantity;
//////    unsigned char ucAnalogueInputQuantity;
//////        unsigned char ucDisplayType;
////        unsigned char ucErrorCode;
////    };
////#pragma pack(pop)
//
////#pragma pack(push)
////#pragma pack(4)
//// структура данных, для взаимодействия с "CodeSys", через разделяемую память.
////    struct TDataExchangeControlBuffTest
////    {
////        // конфигурация прибора: содержимое объекта xPlcConfigDataTemp.xConfigData, типа - TConfigData.
//////    unsigned char aucConfigData[sizeof(struct TConfigData)];
////        TConfigData xConfigData;
//////    // текущее время.
//////    // Байт 1 - секунда 0 – 59;
//////    // Байт 2 - минута 0 –59;
//////    // Байт 3 - час 0 – 23;
//////    // Байт 4 - день 1 – 31;
//////    // Байт 5 - месяц 1 – 12.
//////    // Байт 6 - год 0 – 99.
//////    // Байт 7 – день недели 1 – 7.
//////    unsigned char aucCurrentTime[CURRENT_TIME_BYTE_QUANTITY];
////        // флаг - данные для обмена готовы.
////        bool fbDataExchangeReady;
////    };
////#pragma pack(pop)
//
////    TConfigData xConfigData;
//
////    printf("sizeof %d\n\r", sizeof(CAlarm::TAlarmEventBrief));
//
////    printf("sizeof TDataExchangeControlBuff %d\n\r", sizeof(struct TDataExchangeControlBuff));
////    printf("sizeof TDataExchangeControlBuffTest %d\n\r", sizeof(struct TDataExchangeControlBuffTest));
////    printf("sizeof TConfigData %d\n\r", sizeof(struct TConfigData));
////    printf("sizeof TTestStruct %d\n\r", sizeof(struct TTestStruct));
////    printf("sizeof aucConfigData %d\n\r", sizeof(aucConfigData));
//
//
////        char* pcTextDescriptor;
////        pcTextDescriptor = "ui8State";
////    printf("pcTextDescriptor %s\n\r", pcTextDescriptor);
//
//
//////    printf_P("%zu ", offsetof(CAlarm::TAlarmEventBrief, ui16Address));
//////    printf_P("%zu ", offsetof(CAlarm::TAlarmEventBrief, ui8State));
//
////printf("sizeof TMbmFunction1234PackOne %d\n\r", sizeof(struct TMbmFunction1234PackOne));
////printf("sizeof TMbmFunction1234 %d\n\r", sizeof(struct TMbmFunction1234));
//
//// cout << "send_msg msg" << endl;
////        unsigned char *pucSourceTemp;
////        pucSourceTemp = (unsigned char*)msg;
////        for(int i=0; i<256; )
////        {
////            for(int j=0; j<8; j++)
////            {
////                cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
////            }
////            cout << endl;
////            i += 8;
////        }
//
////    while (1)
////    {
////        usleep(10000);
////    }
//
////    struct TTest
////    {
////        int iTest;
////        char cTest;
////    };
////    printf("sizeof %d\n\r", sizeof(struct TTest));
//
////    CSystemComponentInterface* pxSystemComponentInterface;
////    CSystemComponentModuleMrXX xSystemComponentModuleMrXX;
////    pxSystemComponentInterface = &xSystemComponentModuleMrXX;
////    pxSystemComponentInterface -> Print();
////    cout << "GetType() " << std::showbase << std::dec << (int)pxSystemComponentInterface -> GetType() << std::endl;
////
////    CCommunicationDeviceInterface* pxCommunicationDeviceInterface;
////    CTcpCommunicationDeviceInterface* pxTcpCommunicationDeviceInterface;
////    CTcpCommunicationDevice xTcpCommunicationDevice;
////    pxCommunicationDeviceInterface = &xTcpCommunicationDevice;
////    pxTcpCommunicationDeviceInterface = &xTcpCommunicationDevice;
//
//    nui8FrameCounter = 0; // modbus mind.
//
////    printf("main ui8SoundSignalTypeLocal %d\n\r", (3 & ~0x80));
//
////        cout << "iModuleMrXXTransferData ucData 2 " << std::showbase << std::dec << (int)ucData << std::endl;
//
////    CSystemComponentModuleMr51 xSystemComponentModuleMr51(4);
////    CSystemComponentInterface* pxSystemComponentInterface;
////    pxSystemComponentInterface = &xSystemComponentModuleMr51;
////
//////    CSystemComponentModuleMtvi5 xSystemComponentModuleMtvi5(2);
//////    pxSystemComponentInterface = &xSystemComponentModuleMtvi5;
////
////    if (pxSystemComponentInterface -> IsAbleToReplace(MODULE_TYPE_MR51))
//////    if (pxSystemComponentInterface -> IsAbleToReplace(MODULE_TYPE_MTVI5))
////    {
////        cout << "IsAbleToReplace true " << std::endl;
////    }
////    else
////    {
////        cout << "IsAbleToReplace false " << std::endl;
////    }
////
////    cout << "xSystemComponentModuleMr51.GetAddress() " << std::showbase << std::dec << (int)pxSystemComponentInterface -> GetAddress() << std::endl;
//
////    cout << "CSettingsRW::PlcSettingsLoad ui8ModbusRtuStopBits " << (int)ui8ModbusRtuStopBits << endl;
////-----------------------------------------------------------------------------------------------------
//
//    uint8_t ui8FlowControl = 0;
//    uint8_t ui8MtviCounter = 0;
//    unsigned char nucIndexNumber;
//    uint8_t ui8ExchangeCounter = 0;
//
//    ui8FlowControl = 20;
//    while (1)
//    {
//        switch(ui8FlowControl)
//        {
////-----------------------------------------------------------------------------------------------------
//        case 0:
////            if (xCArchiveSaveParse.CheckMountedDiskMmc0())
////            {
////                iGpioMmapInitialization();
////                vLedPinSet();
////
////                ui8FlowControl = 10;
////            }
////            else
////            {
////                ui8FlowControl = 20;
////            }
//
////            ui8FlowControl = 20;
//            break;
//
////-----------------------------------------------------------------------------------------------------
//        case 5:
//            if (ucMainCycleTimeStart)
//            {
//                ucMainCycleTimeStart = 0;
//                iTimerRestart(&timerid, &its);
//                pthread_mutex_lock(&xMainDataAccessMutex);
//                vCurrentTimeUpdate();
////                vBadModulesTest();
//                xCPingTaskEWWE.DataExchange(CTaskSerialMT::apxTasksProcessed, &xCSpiSerialMT);
////                vInternalToModbusBuffDataExchange();
//                pthread_mutex_unlock(&xMainDataAccessMutex);
//
//                // ?
//                if (nucBlinkCounter == 5)
//                {
//                    // установим флаг - копирование диска mmc0 на mmc1.
//                    fbGlobalMmcCopy = 1;
//                    ui8FlowControl = 10;
//                    nucBlinkCounter = 0;
//                }
//                else
//                {
//                    nucBlinkCounter++;
//                }
//            }
//            usleep(1000);
//            break;
//
////-----------------------------------------------------------------------------------------------------
//        case 10:
//            if (ucMainCycleTimeStart)
//            {
//                ucMainCycleTimeStart = 0;
//                iTimerRestart(&timerid, &its);
//                pthread_mutex_lock(&xMainDataAccessMutex);
//                vCurrentTimeUpdate();
////                vBadModulesTest();
//                xCPingTaskEWWE.DataExchange(CTaskSerialMT::apxTasksProcessed, &xCSpiSerialMT);
////                vInternalToModbusBuffDataExchange();
//                pthread_mutex_unlock(&xMainDataAccessMutex);
//
//                if (ui8MainRestart)
//                {
//                    ui8MainRestart = 0;
//                    // установим флаг - закончено копирование диска mmc0 на mmc1.
//                    fbGlobalMmcCopyEnd = 1;
//                    CGpio::ClearPin(PRD_EN_PIN_PORT, PRD_EN_PIN);
//                    ui8FlowControl = 15;
//                }
//                else
//                {
//                    // управление мигающим синим светодиодом.
//                    // фаза отсутствия свечения закончилась?
//                    if ((!fbBlinkIsOn) && (!nucBlinkCounter))
//                    {
//                        nucBlinkCounter = 2;
//                        // фаза свечения.
//                        fbBlinkIsOn = 1;
//                        CGpio::SetPin(PRD_EN_PIN_PORT, PRD_EN_PIN);
//                    }
//                    else
//                    {
//                        // фаза свечения закончилась?
//                        if ((fbBlinkIsOn) && (!nucBlinkCounter))
//                        {
//                            nucBlinkCounter = 3;
//                            // фаза отсутствия свечения.
//                            fbBlinkIsOn = 0;
//                            CGpio::ClearPin(PRD_EN_PIN_PORT, PRD_EN_PIN);
//                        }
//                        else
//                        {
//                            nucBlinkCounter--;
//                        }
//                    }
//                }
//            }
//            usleep(1000);
//            break;
//
////-----------------------------------------------------------------------------------------------------
//        case 15:
//            if (ucMainCycleTimeStart)
//            {
//                ucMainCycleTimeStart = 0;
//                iTimerRestart(&timerid, &its);
//                pthread_mutex_lock(&xMainDataAccessMutex);
//                vCurrentTimeUpdate();
////                vBadModulesTest();
//                xCPingTaskEWWE.DataExchange(CTaskSerialMT::apxTasksProcessed, &xCSpiSerialMT);
////                vInternalToModbusBuffDataExchange();
//                pthread_mutex_unlock(&xMainDataAccessMutex);
//            }
//            usleep(1000);
//            break;
//
////-----------------------------------------------------------------------------------------------------
//        case 20:
////            fbPlcErrorReset = 0;
////            fbGlobalKvit = 0;
////            fbGlobalReset = 0;
////            fbGlobalBlock = 0;
////            fbOneSecondPassed = 0;
////            fbTenSecondsPassed = 0;
////            nucOneSecondCounter = 0;
////            nucTenSecondsCounter = 0;
////            ucAlarmStateCommon = 0;
////            fucZvkMal = 0;
////            fucKvSbBef = 0;
////            nucBlinkCounter = 0;
////            fbBlinkIsOn = 0;
////            fbDiscreteInputDataBaseError = 0;
////            fbFunctionBlockDataBaseError = 0;
////            fbPlcDataBaseFileError = 0;
////            fbPlcConfigurationFileError = 0;
////            fbPlcConfigurationError = 0;
////            fbAnalogueInputModuleDataBaseError = 0;
////            fbDiscreteOutputModuleDataBaseError = 0;
////            fbLocalNetworkModuleDataBaseError = 0;
////            fbAnalogueInputModuleReperPointsAdcDataBaseError = 0;
////            fbMindDataExchangeError = 0;
////            fbCodeSysDataExchangeError = 0;
////            fbModuleMrOutOff = 0;
////            ui8CurrentTimeUpdateDelayCounter = 0;
////            ui8CurrentTimeSaveDelayCounter = 0;
////
////            memset(&xMainFlagRegister,
////                   0,
////                   sizeof(xMainFlagRegister));
////            memset(&xPlcConfigDataTemp,
////                   0,
////                   sizeof(struct TPlcConfigData));
////            memset(&axAlarmHmi[0],
////                   0,
////                   sizeof(axAlarmHmi));
////
////            CEvents::OccuredEventsControlReset();
////
//////            iGpioMmapInitialization();
////            CGpio::Initialization();
////            CPlatform::LedInitialization();
//////            vLedPinSet();
//
////    CDataStore* pxDataStoreFileSystem;
////    pxDataStoreFileSystem = new CDataStore(new CStorageDeviceFileSystem);
////    CDataStore* pxDataStoreFram;
////    pxDataStoreFram = new CDataStore(new CStorageDeviceFram);
//
//            xMainProductionCycle.GlobalVariablesInitInitialization();
//
//            CodeSysStart();
//
//            xMainProductionCycle.PlatformComponentsInitialization();
//
//            CSettingsRW::PlcSettingsCheck();
//            iPlcDataBaseCheck();
//            CSettingsRW::PlcSettingsLoad();
//            iPlcConfigurationCheck();
//            vAnalogueInputDescriptionWorkExstract();
//            vAnalogueInputDescriptionWorkToModbus();
//            vDimentionsParametersDataBaseExstract();
//            vModbusMasterModuleDataBaseExstract();
//            vAnalogueOutputDescriptionWorkExstract();
//            vAnalogueOutputDescriptionWorkToModbus();
//            vDiscreteInputDataBaseExstract();
//            vDiscreteSygnalTextDescriptorDataBaseExstract();
//            vFunctionBlockDataBaseExstract();
//
//////            iSpiInitialization();
////            xSpiCommunicationDevice.Initialization();
//////            xSpiCommunicationDevice.ChipSelectPinSet();
////            iPlcModulesSearch();
//            xMainProductionCycle.PlatformComponentsGathering();
//
//
//            iPlcConfigurationCreate();
//            vMainTaskMemoryMapFormation();
//
////            iPlcConfigurationCompare();
//            xMainProductionCycle.IsSpecificationCorrect();
//
//            vMainTaskMemoryMapFormation();
//
//            // откроем базу данных.
//            if (xCArchiveEventsDB.Connect())
//            {
//                // error.
//            }
//
//            // вычислим контрольные суммы. потом будем проверять целостность перед каждым главным циклом.
//            xPlcConfigService.usCrc =
//                usCrcSummTwoByteCalculation((unsigned char*)&xPlcConfigService.xPlcConfigServiceData,
//                                            sizeof(xPlcConfigService.xPlcConfigServiceData));
//
//            ucModbusSlaveAddress = (xPlcDataBase.
//                                    axPlcDataBaseBlocks[NETWORK_ADDRESS_DATA_BASE_BLOCK_OFFSET].
//                                    aucPlcDataBaseBlockData[0]) + 1;
//
//            usleep(5000);
//            vThreadsCreate();
//            SharedMemoryCreate();
//            usleep(5000);
//
//            xCCrc.CreateAll();
//
//            xCMainCycleMeasureRingBuffer.Create(64);
////            xCCodeSysCycleMeasureRingBuffer.Create(64);
////            xCCodeSysCycleMeasureRingBuffer2.Create(64);
////            xCCodeSysCycleMeasureRingBuffer3.Create(64);
//            CAlarm::PlcOnOffEvetnsCreate();
//
//            iTimerCreate(&timerid,
//                         &sev);
//            iTimerSetTime(&timerid,
//                          &its,
//                          MAIN_CYCLE_TIME);
//            xProgrammTimeMeasure.Begin();
//            ucMainCycleTimeStart = 0;
//
////-----------------------------------------------------------------------------------------------------
//            // Linux загружен с диска mmc0?
//            if (xCArchiveSaveParse.CheckMountedDiskMmc0())
//            {
//                // Linux загружен с диска mmc0.
//                // запустим сценарий копирования диска mmc0 на mmc1.
//                // создадим обработчик сигнала посылаемого из сценария копирования диска mmc0 на mmc1, после его окончания.
//                if (iHandlerCreate(SIGRTMIN+3,
//                                   &MainAction,
//                                   MainActionHandler) == - 1)
//                {
//                    cout << "Failed to create the action MainAction" << endl;
//                }
//                ui8MainRestart = 0;
//
//                // создадим поток копирования диска mmc0 на mmc1.
//                // create thread, pass reference, addr of the function and data
//                if (pthread_create(&xSystemDiskCopyDisplay,
//                                   NULL,
//                                   thread_SystemDiskCopyDisplay,
//                                   NULL))
//                {
//                    cout << "Failed to create the thread_SystemDiskCopyDisplay" << endl;
//                }
//
//                // первый сеанс связи с дисплеем его перезагрузит.
//                // на следущем сеансе отправим команду - копирование диска mmc0 на mmc1.
//                // если отправить команду сразу, на дисплее не появится окно копирования.
//                ui8FlowControl = 5;
//            }
//            else
//            {
//                // Linux загружен с диска mmc1.
//                // продолжим нормальный порядок работы.
//
////-----------------------------------------------------------------------------------------------------
//                // есть ошибки базы данных или конфигурации?
//                if ((fbPlcConfigurationError) ||
//                        (fbPlcDataBaseFileError))
//                {
//                    if (fbPlcConfigurationError)
//                    {
//                        aucCoilsArray[CONFIGURATION_ERROR_SIGNAL_BIT_ARRAY_OFFSET] = 1;
//                        printf("Configuration Error Flow Stop.\n");
//
//                        // активное состояние события ещё не зарегистрировано?
//                        if(xCInternalModuleErrorEvent.EventOnIsNotRegistered(
//                                    SYSTEM_ERROR_GROUP_OFFSET,
//                                    SYSTEM_ERROR_CONFIGURATION_ERROR_OFFSET))
//                        {
//                            // зарегистрируем активное состояние события.
//                            CEvents::EventRegistration(
//                                SYSTEM_ERROR_GROUP_OFFSET,
//                                (CEvents::HANDLED_EVENTS_CONFIGURATION_ERROR_TYPE |
//                                 CEvents::HANDLED_EVENTS_IS_POPUP |
//                                 CEvents::HANDLED_EVENTS_IS_SOUND |
//                                 CEvents::HANDLED_EVENTS_IS_OCCURED_ON_START),
//                                SYSTEM_ERROR_CONFIGURATION_ERROR_OFFSET,
//                                "Ошибка конфигурации");
//                        }
//                    }
//
//                    if (fbPlcDataBaseFileError)
//                    {
//                        printf("DataBase Error Flow Stop.\n");
//
//                        // активное состояние события ещё не зарегистрировано?
//                        if(xCInternalModuleErrorEvent.EventOnIsNotRegistered(
//                                    SYSTEM_ERROR_GROUP_OFFSET,
//                                    SYSTEM_ERROR_COMMON_DATA_BASE_ERROR_OFFSET))
//                        {
//                            // зарегистрируем активное состояние события.
//                            CEvents::EventRegistration(
//                                SYSTEM_ERROR_GROUP_OFFSET,
//                                (CEvents::HANDLED_EVENTS_SYSTEM_ERROR_TYPE |
//                                 CEvents::HANDLED_EVENTS_IS_POPUP |
//                                 CEvents::HANDLED_EVENTS_IS_SOUND |
//                                 CEvents::HANDLED_EVENTS_IS_ARCHIVE |
//                                 CEvents::HANDLED_EVENTS_IS_OCCURED_ON_START),
//                                SYSTEM_ERROR_COMMON_DATA_BASE_ERROR_OFFSET,
//                                "Ошибка общей б.д");
//                        }
//                    }
//
//                    CGpio::SetPin(PRD_EN_PIN_PORT, PRD_EN_PIN);
//                    ui8FlowControl = 40;
//                }
//                else
//                {
//                    ui8FlowControl = 50;
//                }
//            }
//
//            break;
//
////-----------------------------------------------------------------------------------------------------
//        case 30:
//            fbPlcErrorReset = 0;
//            fbGlobalKvit = 0;
//            fbGlobalReset = 0;
//            fbGlobalBlock = 0;
//            fbOneSecondPassed = 0;
//            fbTenSecondsPassed = 0;
//            nucOneSecondCounter = 0;
//            nucTenSecondsCounter = 0;
//            ucAlarmStateCommon = 0;
//            fucZvkMal = 0;
//            fucKvSbBef = 0;
//            nucBlinkCounter = 0;
//            fbBlinkIsOn = 0;
//            fbDiscreteInputDataBaseError = 0;
//            fbFunctionBlockDataBaseError = 0;
//            fbPlcDataBaseFileError = 0;
//            fbPlcConfigurationFileError = 0;
//            fbPlcConfigurationError = 0;
//            fbAnalogueInputModuleDataBaseError = 0;
//            fbDiscreteOutputModuleDataBaseError = 0;
//            fbLocalNetworkModuleDataBaseError = 0;
//            fbAnalogueInputModuleReperPointsAdcDataBaseError = 0;
//            fbMindDataExchangeError = 0;
//            fbCodeSysDataExchangeError = 0;
//            fbModuleMrOutOff = 0;
//            ui8CurrentTimeUpdateDelayCounter = 0;
//            ui8CurrentTimeSaveDelayCounter = 0;
//
//            memset(&xMainFlagRegister,
//                   0,
//                   sizeof(xMainFlagRegister));
//            memset(&xPlcConfigDataTemp,
//                   0,
//                   sizeof(struct TPlcConfigData));
//            memset(&axAlarmHmi[0],
//                   0,
//                   sizeof(axAlarmHmi));
//
//            CEvents::OccuredEventsControlReset();
//
////            iGpioMmapInitialization();
//            CGpio::Initialization();
//            CPlatform::LedInitialization();
////            vLedPinSet();
//            CSettingsRW::PlcSettingsCheck();
//            iPlcDataBaseCheck();
//            CSettingsRW::PlcSettingsLoad();
//            iPlcConfigurationCheck();
//            vAnalogueInputDescriptionWorkExstract();
//            vAnalogueInputDescriptionWorkToModbus();
//            vDimentionsParametersDataBaseExstract();
//            vModbusMasterModuleDataBaseExstract();
//            vAnalogueOutputDescriptionWorkExstract();
//            vAnalogueOutputDescriptionWorkToModbus();
//            vDiscreteInputDataBaseExstract();
//            vDiscreteSygnalTextDescriptorDataBaseExstract();
//            vFunctionBlockDataBaseExstract();
//
//            CodeSysStart();
//
////            iSpiInitialization();
//            xSpiCommunicationDevice.Initialization();
//            xSpiCommunicationDevice.ChipSelectPinSet();
//            iPlcModulesSearch();
//            iPlcConfigurationCreate();
//            vMainTaskMemoryMapFormation();
//            iPlcConfigurationCompare();
//            vMainTaskMemoryMapFormation();
//
//            // откроем базу данных.
//            if (xCArchiveEventsDB.Connect())
//            {
//                // error.
//            }
//
//            // вычислим контрольные суммы. потом будем проверять целостность перед каждым главным циклом.
//            xPlcConfigService.usCrc =
//                usCrcSummTwoByteCalculation((unsigned char*)&xPlcConfigService.xPlcConfigServiceData,
//                                            sizeof(xPlcConfigService.xPlcConfigServiceData));
//
//            ucModbusSlaveAddress = (xPlcDataBase.
//                                    axPlcDataBaseBlocks[NETWORK_ADDRESS_DATA_BASE_BLOCK_OFFSET].
//                                    aucPlcDataBaseBlockData[0]) + 1;
//
//            usleep(5000);
////            vThreadsCreate();
////            SharedMemoryCreate();
////            usleep(5000);
//
//            xCCrc.CreateAll();
//
////            iTimerCreate(&timerid,
////                         &sev);
//            iTimerSetTime(&timerid,
//                          &its,
//                          MAIN_CYCLE_TIME);
//            xProgrammTimeMeasure.Begin();
//            ucMainCycleTimeStart = 0;
//
////-----------------------------------------------------------------------------------------------------
//            // есть ошибки базы данных или конфигурации?
//            if ((fbPlcConfigurationError) ||
//                    (fbPlcDataBaseFileError))
//            {
//                if (fbPlcConfigurationError)
//                {
//                    aucCoilsArray[CONFIGURATION_ERROR_SIGNAL_BIT_ARRAY_OFFSET] = 1;
//                    printf("Configuration Error Flow Stop.\n");
//                }
//
//                if (fbPlcDataBaseFileError)
//                {
//                    printf("DataBase Error Flow Stop.\n");
//                }
//                CGpio::SetPin(PRD_EN_PIN_PORT, PRD_EN_PIN);
//                ui8FlowControl = 40;
//            }
//            else
//            {
//                ui8FlowControl = 50;
//            }
//            break;
//
////-----------------------------------------------------------------------------------------------------
//        case 40:
//            if (ucMainCycleTimeStart)
//            {
//                ucMainCycleTimeStart = 0;
//                iTimerRestart(&timerid, &its);
//                pthread_mutex_lock(&xMainDataAccessMutex);
//                vCurrentTimeUpdate();
//                vBadModulesTest();
//                xCPingTaskEWWE.DataExchange(CTaskSerialMT::apxTasksProcessed, &xCSpiSerialMT);
//                vInternalToModbusBuffDataExchange();
//                pthread_mutex_unlock(&xMainDataAccessMutex);
//                if (fbPlcErrorReset)
//                {
////                    vThreadsDestroy();
//                    fbPlcErrorReset = 0;
//                    ui8FlowControl = 30;
//                }
//            }
//            usleep(1000);
//            break;
//
////-----------------------------------------------------------------------------------------------------
//        case 50:
//            xCTimeMeasure.Begin();
//
//            pthread_mutex_lock(&xMainDataAccessMutex);
//            vCurrentTimeUpdate();
//            vBadModulesTest();
//            // если флаг "перезагрузка программы" установлен,
//            // то это значит произошёл сбой памяти.
//            // начинаем main() сначала.
//            if (fbPlcErrorReset)
//            {
////                vThreadsDestroy();
//                fbPlcErrorReset = 0;
//                ui8FlowControl = 30;
//            }
//            else
//            {
//                ui8FlowControl = 60;
//            }
//
////            for (int i = 0; i < 5; i++)
////            {
//            xCPingTaskEWWE.DataExchange(CTaskSerialMT::apxTasksProcessed, &xCSpiSerialMT);
////            }
//
//            pthread_mutex_unlock(&xMainDataAccessMutex);
//            nucIndexNumber = 0;
//            break;
//
////-----------------------------------------------------------------------------------------------------
//        case 60:
//            // обработаны не все внутренние модули?
//            if (nucIndexNumber < (xPlcConfigService.
//                                  xPlcConfigServiceData.
//                                  ucInternalModulesQuantity))
//            {
//                // какой обрабатывается модуль?
//                switch(xAllModulesContext.axAllModulesContext[nucIndexNumber].
//                        xModuleContextStatic.ucModuleType)
//                {
//                case MODULE_TYPE_MVDI:
//                    iModuleMvdiDataHandler(&xAllModulesContext.axAllModulesContext[nucIndexNumber]);
//                    break;
//                case MODULE_TYPE_MVDS9:
//                    iModuleMvds9DataHandler(&xAllModulesContext.axAllModulesContext[nucIndexNumber]);
//                    break;
//                case MODULE_TYPE_MVPS3:
//                case MODULE_TYPE_MVAI5:
//                case MODULE_TYPE_MVST3:
//                case MODULE_TYPE_MVAO3:
//                    vModuleMvai5DataHandlerNew(&xAllModulesContext.axAllModulesContext[nucIndexNumber]);
//                    break;
//                case MODULE_TYPE_MR53:
//                case MODULE_TYPE_MR54:
//                case MODULE_TYPE_MR55:
//                case MODULE_TYPE_MR51:
//                    vModuleMrXXDataHandler(&xAllModulesContext.axAllModulesContext[nucIndexNumber]);
//                    break;
//                case MODULE_TYPE_MTVI5:
//                    iModuleMtvi5DataHandler(&xAllModulesContext.axAllModulesContext[nucIndexNumber]);
//                    ui8MtviCounter++;
//                    break;
//                default:
//                    break;
//                };
//
////                ui8ExchangeCounter++;
////                if (ui8ExchangeCounter >= 8)
////                {
////                    ui8ExchangeCounter = 0;
//                // следующий модуль.
//                nucIndexNumber++;
////                }
//            }
//            else
//            {
//                ui8FlowControl = 70;
//            }
//            break;
//
////-----------------------------------------------------------------------------------------------------
//        case 70:
//            // обработаны все внутренние модули, выполним оставшиеся задачи.
//            pthread_mutex_lock(&xMainDataAccessMutex);
//            vAlarmHandler();
//            vLinuxToCodeSysDataExchange();
//            vInternalToModbusBuffDataExchange();
//            // сбросим флаги квитирование и сброс, если установлены.
//            fbGlobalKvit = 0;
//            fbGlobalReset = 0;
//            pthread_mutex_unlock(&xMainDataAccessMutex);
//            // есть блокировка?
//            if (fbGlobalBlock)
//            {
//                // активное состояние события ещё не зарегистрировано?
//                if(xCInternalModuleErrorEvent.EventOnIsNotRegistered(
//                            SYSTEM_ERROR_GROUP_OFFSET,
//                            SYSTEM_ERROR_BLOCK_MODE_OFFSET))
//                {
//                    // зарегистрируем активное состояние события.
//                    // включение режима блокировка.
//                    CEvents::EventRegistration(
//                        0,
//                        (CEvents::HANDLED_EVENTS_SYSTEM_EVENTS_TYPE |
////                         CEvents::HANDLED_EVENTS_IS_POPUP |
////                         CEvents::HANDLED_EVENTS_IS_SOUND |
//                         CEvents::HANDLED_EVENTS_IS_ARCHIVE),
//                        SYSTEM_ERROR_BLOCK_MODE_OFFSET,
//                        "Вкл. блокировки");
//                }
//            }
//            else
//            {
//                // неактивное состояние события ещё не зарегистрировано?
//                if(xCInternalModuleErrorEvent.EventOffIsNotRegistered(
//                            SYSTEM_ERROR_GROUP_OFFSET,
//                            SYSTEM_ERROR_BLOCK_MODE_OFFSET))
//                {
//                    // зарегистрируем неактивное состояние события.
//                    // выключение режима блокировка.
//                    CEvents::EventRegistration(
//                        0,
//                        (CEvents::HANDLED_EVENTS_SYSTEM_EVENTS_TYPE |
//                         CEvents::HANDLED_EVENTS_IS_ARCHIVE),
//                        SYSTEM_ERROR_BLOCK_MODE_OFFSET,
//                        "Выкл. блокировки");
//                }
//            }
//            // размер базы данных достиг максимума?
//            if (xCArchiveEventsDB.DataBaseIsFull())
//            {
//                xCArchiveEventsDB.DataBaseIsFullReset();
//                xCArchiveEventsDB.DataBaseRingCreate();
//            }
//            xCTimeMeasure.End();
//            ui8FlowControl = 80;
//            break;
//
////-----------------------------------------------------------------------------------------------------
//        case 80:
//            if (ucMainCycleTimeStart)
//            {
//                ucMainCycleTimeStart = 0;
//                iTimerRestart(&timerid, &its);
//                ui8FlowControl = 50;
//            }
//            usleep(1000);
//            break;
//
////-----------------------------------------------------------------------------------------------------
//        default:
////            ui8FlowControl = 0;
//            break;
//        };
//
////-----------------------------------------------------------------------------------------------------
//        pthread_mutex_lock(&xMainDataAccessMutex);
//#ifdef MODBUS_TCP_ENABLE
//        vDeviceControlModbusMessageHandler(pxTcpModbusMapping);
//#endif // MODBUS_TCP_ENABLE
//
//#ifdef MODBUS_RTU_ENABLE
//        vDeviceControlModbusMessageHandler(pxRtuModbusMapping);
//#endif // MODBUS_RTU_ENABLE
//
//#ifdef MODBUS_HMI_ENABLE
//        vDeviceControlModbusMessageHandler(pxHmiRtuModbusMapping);
//#endif // MODBUS_HMI_ENABLE
//        pthread_mutex_unlock(&xMainDataAccessMutex);
//    }

    pthread_mutex_destroy(&xMainDataAccessMutex);

    munmap(pxDataExchangeLinuxToCodeSysBuff,
           sizeof(struct TDataExchangeLinuxToCodeSysBuff));
    munmap(pxDataExchangeCodeSysToLinuxBuff,
           sizeof(struct TDataExchangeCodeSysToLinuxBuff));
    munmap(pxDataExchangeControlBuff,
           sizeof(struct TDataExchangeControlBuff));

    xSpiCommunicationDevice.ChipSelectPinDelete();
    CPlatform::LedDestroy();

    return 0;
}




//                gettimeofday( &xTimeLast, NULL );

//if( gettimeofday( &xTimeCur, NULL ) != 0 )
//{
//    /* gettimeofday failed - retry next time. */
//}
//else
//{
////                    (((xTimeCur.tv_sec - xTimeLast.tv_sec) *
////                            1000000) +
////                            (xTimeCur.tv_usec - xTimeLast.tv_usec)) >
////                            ctx->modbus_timeout.tv_usec)))
//
//    if (xTimeMax.tv_usec < (xTimeCur.tv_usec - xTimeLast.tv_usec))
//    {
//        xTimeMax.tv_usec = (xTimeCur.tv_usec - xTimeLast.tv_usec);
//    }
//    if (iTimeMeasureCounter == 10)
//    {
//        iTimeMeasureCounter = 0;
//        printf("ArchiveEventLogQueryEW gap %d\n\r", (xTimeMax.tv_usec));
//    }
//    iTimeMeasureCounter++;
//}


//struct timeval  xTimeCur;
//struct timeval xTimeLast;
//gettimeofday( &xTimeLast, NULL );
//
//
//if( gettimeofday( &xTimeCur, NULL ) != 0 )
//{
//    /* gettimeofday failed - retry next time. */
//}
//else
//{
////                    (((xTimeCur.tv_sec - xTimeLast.tv_sec) *
////                            1000000) +
////                            (xTimeCur.tv_usec - xTimeLast.tv_usec)) >
////                            ctx->modbus_timeout.tv_usec)))
//    printf("ArchiveEventLogQueryEW gap %d\n\r", (xTimeCur.tv_usec - xTimeLast.tv_usec));
//}


//разобраться со структурой struct modbus_t
//// _modbus_tcp_pi
/////* TCP port */
////int port;
//    /* Node */
//    char node[_MODBUS_TCP_PI_NODE_LENGTH];
//    /* Service */
//    char service[_MODBUS_TCP_PI_SERVICE_LENGTH];

// switch(ui8FsmState)
//    {
//    case CEvents::REQUESTED_EVENTS_ORDER_DEFAULT:
//        break;
//    case CEvents::REQUESTED_EVENTS_ORDER_BY_NUMBER:
//        break;
//    case CEvents::REQUESTED_EVENTS_ORDER_BY_DATE:
//        break;
//    default:
//        break;
//    };

//pidof codesyscontrol_arm_beagleboneblack_V3.5.13.0.deb
//pidof grep codesyscontrol
//pidof /opt/codesys/bin/codesyscontrol.bin
//ps axu | grep codesyscontrol

//С помощью ps мы можем узнать PID процесса:
//
//# ps auxwww |grep -v grep |grep -i 'process_name'
//
//
//Команда должна вернуть запись содержащую такие колонки:
//
//USER PID %CPU %MEM VSZ RSS TTY STAT START TIME COMMAND
//
//
//
//Далее приостанавливаем подходящий процесс командой *:
//
//# kill -STOP PID
//
//
//Чтобы продолжить остановленный процесс необходимо выполнить команду *:
//
//# kill -CONT PID
//
//
//* Вместо PID нужно подставить цифровой идентификатор процесса.



//1) The following code compiles on Linux.
// It has a number of problems, however.
// Please locate as many of those problems as you are able
// and provide your recommendations regarding how they can be resolved.

//typedef struct list_s
//{
//    struct list_s *next; /* NULL for the last item in a list */
//    int data;
//}
//list_t;
//
///* Counts the number of items in a list.
// */
//int count_list_items(const list_t *head)
//{
//    if (head->next)
//    {
//        return count_list_items(head->next) + 1;
//    }
//    else
//    {
//        return 1;
//    }
//}
//
///* Inserts a new list item after the one specified as the argument.
// */
//void insert_next_to_list(list_t *item, int data)
//{
//    (item->next = malloc(sizeof(list_t)))->next = item->next;
//    item->next->data = data;
//}
//
///* Removes an item following the one specificed as the argument.
// */
//void remove_next_from_list(list_t *item)
//{
//    if (item->next)
//    {
//        free(item->next);
//        item->next = item->next->next;
//    }
//}
//
///* Returns item data as text.
// */
//char *item_data(const list_t *list)
//{
//    char buf[12];
//
//    sprintf(buf, "%d", list->data);
//    return buf;
//}





//-------------------------------------------------------------------------------
//  Sourse      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-------------------------------------------------------------------------------

#include <memory>

#include "Timer.h"
#include "Task.h"
#include "Platform.h"
#include "Resources.h"
#include "DataStore.h"
#include "DataContainer.h"
#include "Link.h"
#include "AnalogueSignals.h"
#include "ConfigurationCreate.h"
#include "InternalModuleMuvr.h"
#include "ModbusSlave.h"
#include "DeviceControl.h"

using namespace std;

//-------------------------------------------------------------------------------
CDeviceControl::CDeviceControl()
{
    std::cout << "CDeviceControl constructor"  << std::endl;
    m_puiIntermediateBuff = new uint8_t[CDataStore::MAX_BLOCK_LENGTH];
    SetFsmState(START);
}

//-------------------------------------------------------------------------------
CDeviceControl::~CDeviceControl()
{
    delete[] m_puiIntermediateBuff;
}

//-------------------------------------------------------------------------------
uint8_t CDeviceControl::Init(void)
{
    std::cout << "CDeviceControl Init"  << std::endl;
//    m_pxCommandDataContainer = static_cast<CDataContainerDataBase*>(GetResources() ->
//                               AddDataContainer(std::make_shared<CDataContainerDataBase>()));
    m_pxOperatingDataContainer = static_cast<CDataContainerDataBase*>(GetResources() ->
                                 AddDataContainer(std::make_shared<CDataContainerDataBase>()));
    SetExecutorDataContainer(static_cast<CDataContainerDataBase*>(GetResources() ->
                             AddDataContainer(std::make_shared<CDataContainerDataBase>())));

    SetCustomerDataContainer(GetExecutorDataContainerPointer());

    CDataContainerDataBase* pxDataContainer =
        (CDataContainerDataBase*)GetExecutorDataContainerPointer();
    pxDataContainer -> m_puiDataPointer = m_puiIntermediateBuff;
}

//-------------------------------------------------------------------------------
void CDeviceControl::SetInternalModuleMuvrName(std::string sName)
{
    m_sInternalModuleMuvrName = sName;
}

//-----------------------------------------------------------------------------------------------------
// обрабатывает входящие сообщения от Modbus интерфейсов по 71 функции - чтение данных онлайн, модулей аналогового ввода.
// передаёт измеренные значения аналоговых входов, реперные точки АЦП, значения ТХС.
void CDeviceControl::OnlineDataRead(void)
{
    std::cout << "CDeviceControl OnlineDataRead 1"  << std::endl;
    float *pfSource;
    unsigned char *pucSource;
    unsigned char *pucSource2;
    unsigned char *pucDestination;
    unsigned char *pucTempArray;
    unsigned int nuiBusyTimeCounter;
    unsigned char nucIndexNumber;
    unsigned char ucTempData;
    unsigned short usCrc;
    unsigned char ucFlowControl;
    unsigned char ucAddressLow;
    unsigned char ucAddressHigh;
    int i;
    int j;
    int nb;

// ucAddressHigh -
// если в старшем регистре адреса Modbus бит7 = 0, то запрашиваются реперные точки - (бит0 - бит6) - адрес аналогового входа.
// если в старшем регистре адреса Modbus бит7 = 1, то запрашивается ТХС и (бит0 - бит2) - относительный адрес модуля МВСТ3.
// ucAddressLow - требуемое количество аналоговых входов.

    uint16_t uiAddress =
        (((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiDataOffset);
    ucAddressLow = (unsigned char)(uiAddress);
    ucAddressHigh = (unsigned char)((uiAddress) >> 8);
    pucDestination =
        (((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_puiDataPointer);

    CConfigurationCreate::TConfigDataPackOne* pxDeviceConfigSearch =
        (GetResources() -> GetDeviceConfigSearchPointer());

    // запрос ТХС или реперных точек?
    if (ucAddressHigh & MUVR_TXS_REQUEST_MASK)
    {
        std::cout << "CDeviceControl OnlineDataRead 2"  << std::endl;
// запрос ТХС.
//        cout << "TXS" << endl;

//        iModuleMvai5TxsRead(&xAllModulesContext.axAllModulesContext[nucIndexNumber]);
//
//        sprintf((char*)pucDestination,
//                "%7.4f",
//                ((TTxsData*)(xAllModulesContext.axAllModulesContext[nucIndexNumber].
//                             xModuleContextStatic.
//                             pucTxsBufferPointer)) -> fTxsValue);

//        pucDestination += ONLINE_DATA_READ_ONE_INPUT_ASCII_BYTE_QUANTITY;
//        *pucDestination = ((TTxsData*)(xAllModulesContext.axAllModulesContext[nucIndexNumber].
//                                       xModuleContextStatic.
//                                       pucTxsBufferPointer)) -> ucTxsStat;

        (((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiDataLength) =
            (ONLINE_DATA_READ_ONE_INPUT_ASCII_BYTE_QUANTITY +
             ONLINE_DATA_READ_ONE_INPUT_STAT_AI_BYTE_QUANTITY);
    }
    else
    {
        std::cout << "CDeviceControl OnlineDataRead 3"  << std::endl;
// запрос реперных точек.

        // адрес аналогового входа принадлежит внутреннему или внешнему модулю?
        if ((pxDeviceConfigSearch -> uiServiceAnalogueInputModuleQuantity) >=
                (((ucAddressHigh & ANALOGUE_INPUT_ADDRESS_MASK) /
                  MUVR_ANALOG_INPUT_QUANTITY) + 1))
        {
            std::cout << "CDeviceControl OnlineDataRead 4"  << std::endl;
            // адрес аналогового входа принадлежит внутреннему модулю.

            // получим адрес значения аналогового входа.
            pfSource = &(GetResources() ->
                         m_pfAnalogueInputsValue[(ucAddressHigh & ANALOGUE_INPUT_ADDRESS_MASK)]);
            // получим адрес значения состояние канала аналогового входа.
            pucSource2 = &(GetResources() ->
                           m_puiAnalogueInputsState[ucAddressHigh]);
            for (i = 0; i < (ucAddressLow); i++)
            {
                // преобразуем первые 7 цифр float значения, в ASCII символы, для отображения в строковом виде в программаторе.
                sprintf((char*)pucDestination,
                        "%7.4f",
                        pfSource[i]);
                pucDestination += ONLINE_DATA_READ_ONE_INPUT_ASCII_BYTE_QUANTITY;
                // получим значения состояния канала аналогового входа.
                *pucDestination = *pucSource2++;
                pucDestination += ONLINE_DATA_READ_ONE_INPUT_STAT_AI_BYTE_QUANTITY;
            }

            (((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiDataLength) =
                ((ucAddressLow *
                  (ONLINE_DATA_READ_ONE_INPUT_ASCII_BYTE_QUANTITY +
                   ONLINE_DATA_READ_ONE_INPUT_STAT_AI_BYTE_QUANTITY)));

            std::cout << "CDeviceControl::OnlineDataRead m_uiDataLength "  <<
                      (int)(((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiDataLength) << std::endl;
        }
        else
        {
            std::cout << "CDeviceControl OnlineDataRead 5"  << std::endl;
            // адрес аналогового входа принадлежит внешнему модулю.
//            cout << "LSNU" << endl;

            // получим адрес значения аналогового входа.
            pfSource = &(GetResources() ->
                         m_pfAnalogueInputsValue[(ucAddressHigh & ANALOGUE_INPUT_ADDRESS_MASK)]);
            // получим адрес значения состояние канала аналогового входа.
            pucSource2 = &(GetResources() ->
                           m_puiAnalogueInputsState[ucAddressHigh]);
            for (i = 0; i < (ucAddressLow); i++)
            {
                // преобразуем первые 7 цифр float значения, в ASCII символы, для отображения в строковом виде в программаторе.
                sprintf((char*)pucDestination,
                        "%7.4f",
                        pfSource[i]);
                pucDestination += ONLINE_DATA_READ_ONE_INPUT_ASCII_BYTE_QUANTITY;
                // получим значения состояния канала аналогового входа.
                *pucDestination = *pucSource2++;
                pucDestination += ONLINE_DATA_READ_ONE_INPUT_STAT_AI_BYTE_QUANTITY;
            }

            (((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiDataLength) =
                ((ucAddressLow *
                  (ONLINE_DATA_READ_ONE_INPUT_ASCII_BYTE_QUANTITY +
                   ONLINE_DATA_READ_ONE_INPUT_STAT_AI_BYTE_QUANTITY)));

            std::cout << "CDeviceControl::OnlineDataRead m_uiDataLength "  <<
                      (int)(((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiDataLength) << std::endl;
        }
    }

    (((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiDataOffset) = 0;

//    if (ucModuleError ==
//            PROGRAMMING_DATA_BASE_WRITE_BLOCK_MVA_TXS_READ_ERROR)
//    {
//        pxModbusMapping -> current_message_address_common =
//            PROGRAMMING_DATA_BASE_WRITE_BLOCK_MVA_TXS_READ_ERROR;
//        cout << "PROGRAMMING_DATA_BASE_WRITE_BLOCK_MVA_TXS_READ_ERROR" << endl;
//        ucModuleError = 0;
//    }
//
//    if (ucModuleError ==
//            MUVR_GET_REPER_POINTS_ADC_DATA_COMMAND_ERROR)
//    {
//        pxModbusMapping -> current_message_address_common =
//            MUVR_GET_REPER_POINTS_ADC_DATA_COMMAND_ERROR;
//        cout << "MUVR_GET_REPER_POINTS_ADC_DATA_COMMAND_ERROR" << endl;
//        ucModuleError = 0;
//    }

}

//-----------------------------------------------------------------------------------------------------
// обрабатывает входящие сообщения от Modbus интерфейсов, по 5 функции.
uint8_t CDeviceControl::ModbusFunction5Handler(void)
{
    std::cout << "CDeviceControl ModbusFunction5Handler 1" << std::endl;
    unsigned char nucIndexNumber;
    int i;

    uint16_t uiAddress =
        (((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiDataOffset);
    uint8_t* pucDestination =
        (((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_puiDataPointer);

//-----------------------------------------------------------------------------------------------------
// Function 5
    // смотрим, по какому адресу записывается бит.
    switch(uiAddress -
            COILS_ARRAY_MODBUS_BEGIN_ADDRESS)
    {
    // квитирование с верхнего уровня.
    case DEVICE_CONTROL_PC_KVIT:
    {
        CDataContainerDataBase* pxDataContainer =
            (CDataContainerDataBase*)GetCustomerDataContainerPointer();
        // бит установлен?
        if ((pxDataContainer -> m_puiDataPointer[BIT_STATE_OFFSET]))
        {
            // если установлен - сбросим.
            (pxDataContainer -> m_puiDataPointer[BIT_STATE_OFFSET]) = 0;
            (GetResources() -> m_fbIsAcknowledjedGlobal) = true;
        }
        else
        {
            (GetResources() -> m_fbIsAcknowledjedGlobal) = false;
        }
        SetFsmState(MODBUS_FUNCTION_5_HANDLER_EXECUTOR_DONE_OK_ANSWER_PROCESSING);
        return 1;
    }
    break;

    // сброс с верхнего уровня.
    case DEVICE_CONTROL_PC_RESET:
    {
        CDataContainerDataBase* pxDataContainer =
            (CDataContainerDataBase*)GetCustomerDataContainerPointer();
        // бит установлен?
        if ((pxDataContainer -> m_puiDataPointer[BIT_STATE_OFFSET]))
        {
            // если установлен - сбросим.
            (pxDataContainer -> m_puiDataPointer[BIT_STATE_OFFSET]) = 0;
            (GetResources() -> m_fbIsResetededGlobal) = true;
        }
        else
        {
            (GetResources() -> m_fbIsResetededGlobal) = false;
        }
        SetFsmState(MODBUS_FUNCTION_5_HANDLER_EXECUTOR_DONE_OK_ANSWER_PROCESSING);
        return 1;
    }
    break;

    // блокировка с верхнего уровня.
    case DEVICE_CONTROL_BLOCK:
    {
        CDataContainerDataBase* pxDataContainer =
            (CDataContainerDataBase*)GetCustomerDataContainerPointer();
        // бит установлен?
        if ((pxDataContainer -> m_puiDataPointer[BIT_STATE_OFFSET]))
        {
            // если установлен - сбросим.
            (pxDataContainer -> m_puiDataPointer[BIT_STATE_OFFSET]) = 0;
            (GetResources() -> m_fbIsBlockededGlobal) = true;
        }
        else
        {
            (GetResources() -> m_fbIsBlockededGlobal) = false;
        }
        SetFsmState(MODBUS_FUNCTION_5_HANDLER_EXECUTOR_DONE_OK_ANSWER_PROCESSING);
        return 1;
    }
    break;

    default:
        break;
    };

//-----------------------------------------------------------------------------------------------------
    // смотрим, по какому адресу записывается бит.
    switch(((uiAddress -
             COILS_ARRAY_MODBUS_BEGIN_ADDRESS) & 0xFF00))
    {
    // включение-выключение режима калибровки.
    case DEVICE_CONTROL_CALIBRATION_ON_OFF:
        // перед этим кто либо другой не вывел из обработки аналоговый вход?
        if (((GetResources() -> m_uiAinOffProcessOwnerIndex) == AIN_OFF_PROCESS_OWNER_IS_NONE) ||
                ((GetResources() -> m_uiAinOffProcessOwnerIndex) == AIN_OFF_PROCESS_OWNER_IS_PROGRAMMER))
        {
            // вычислим индекс модуля в массиве контекста, к которому поступила команда - калибровка.
            nucIndexNumber = (((((GetResources() -> GetDeviceConfigSearchPointer()) -> uiLastAnalogueInputModuleIndex) +
                                CONVERT_INTEGER_TO_NATURAL_NUMBER) -
                               ((GetResources() -> GetDeviceConfigSearchPointer()) -> uiServiceAnalogueInputModuleQuantity))  +
                              (((uiAddress -
                                 COILS_ARRAY_MODBUS_BEGIN_ADDRESS) & CALIBRATION_ANALOG_MODULE_NUMBER_MASK) >> 4));

            cout << "DEVICE_CONTROL_CALIBRATION_ON nucIndexNumber " << (int)nucIndexNumber << endl;
            cout << "DEVICE_CONTROL_CALIBRATION_ON uiLastAnalogueInputModuleIndex " << (int)(((GetResources() -> GetDeviceConfigSearchPointer()) ->
                    uiLastAnalogueInputModuleIndex) + CONVERT_INTEGER_TO_NATURAL_NUMBER) << endl;
            // по индексу - nucIndexNumber есть модуль?
            if (nucIndexNumber <=
                    (((GetResources() -> GetDeviceConfigSearchPointer()) ->
                      uiLastAnalogueInputModuleIndex) +
                     CONVERT_INTEGER_TO_NATURAL_NUMBER))
            {
                // получим указатель на контейнер с данными заказчика.
                CDataContainerDataBase* pxDataContainer =
                    (CDataContainerDataBase*)GetCustomerDataContainerPointer();
                // бит установлен?
                if ((pxDataContainer -> m_puiDataPointer[BIT_STATE_OFFSET]))
                {
                    // включение режима калибровки.
                    cout << "DEVICE_CONTROL_CALIBRATION_ON" << endl;
                    // если установлен - сбросим.
                    (pxDataContainer -> m_puiDataPointer[BIT_STATE_OFFSET]) = 0;
                    // получим указатель на контейнер с данными исполнителя..
                    pxDataContainer =
                        (CDataContainerDataBase*)GetExecutorDataContainerPointer();
                    // передадим драйверу модуля номер калибруемого входа.
                    // в DO-D2 - № входа в модуле (0-нет режима калибровки).
                    (pxDataContainer -> m_puiDataPointer[COMMON_INDEX_OFFSET]) = (((uiAddress -
                            COILS_ARRAY_MODBUS_BEGIN_ADDRESS) & CALIBRATION_INPUT_NUMBER_MASK));

                    cout << "DEVICE_CONTROL_CALIBRATION_ON ucCommonIndex " << (int)(pxDataContainer -> m_puiDataPointer[0]) << endl;
                    // вернём в рабочее состояние все выведенные из обработки аналоговые входы.
                    for (i = 0; i < MAX_HANDLED_ANALOGUE_INPUT; i++)
                    {
                        (GetResources() -> m_puiAnalogueInputsOff[i]) = 0;
                    }

                    (GetResources() -> m_puiAnalogueInputsOff[(((((uiAddress - // вычисляем номер модуля
                            COILS_ARRAY_MODBUS_BEGIN_ADDRESS) &
                            CALIBRATION_ANALOG_MODULE_NUMBER_MASK) >> 4) *
                            ANALOG_MODULE_INPUT_QUANTITY) +

                            (((uiAddress - // вычисляем номер входа
                               COILS_ARRAY_MODBUS_BEGIN_ADDRESS) &
                              CALIBRATION_INPUT_NUMBER_MASK) -
                             CONVERT_NATURAL_NUMBER_TO_INTEGER))]) = 1;

                    (GetResources() -> m_uiAinOffProcessOwnerIndex) = AIN_OFF_PROCESS_OWNER_IS_PROGRAMMER;
                }
                else
                {
                    // выключение режима калибровки.
                    cout << "DEVICE_CONTROL_CALIBRATION_OFF" << endl;
                    // получим указатель на контейнер с данными исполнителя..
                    pxDataContainer =
                        (CDataContainerDataBase*)GetExecutorDataContainerPointer();
                    // передадим драйверу модуля номер калибруемого входа. если 0, калибровка выключена.
                    // в DO-D2 - № входа в модуле (0-нет режима калибровки).
                    (pxDataContainer -> m_puiDataPointer[COMMON_INDEX_OFFSET]) = 0;
                    // вернём в рабочее состояние все выведенные из обработки аналоговые входы.
                    for (i = 0; i < MAX_HANDLED_ANALOGUE_INPUT; i++)
                    {
                        (GetResources() -> m_puiAnalogueInputsOff[i]);
                    }

                    (GetResources() -> m_uiAinOffProcessOwnerIndex) = AIN_OFF_PROCESS_OWNER_IS_NONE;
                }
            }
        }

        SetFsmState(MODBUS_FUNCTION_5_HANDLER_INPUT_CALIBRATION_COMAND_START);
        return 1;
        break;

    // калибровка начала шкалы - НШК.
    case DEVICE_CONTROL_SET_BOTTOM_OF_SCALE:
        // вычислим индекс модуля в массиве контекста, к которому поступила команда.
        nucIndexNumber = (((((GetResources() -> GetDeviceConfigSearchPointer()) -> uiLastAnalogueInputModuleIndex) +
                            CONVERT_INTEGER_TO_NATURAL_NUMBER) -
                           ((GetResources() -> GetDeviceConfigSearchPointer()) -> uiServiceAnalogueInputModuleQuantity))  +
                          (((uiAddress -
                             COILS_ARRAY_MODBUS_BEGIN_ADDRESS) & CALIBRATION_ANALOG_MODULE_NUMBER_MASK) >> 4));
        // по индексу - nucIndexNumber есть модуль?
        if (nucIndexNumber <=
                (((GetResources() -> GetDeviceConfigSearchPointer()) ->
                  uiLastAnalogueInputModuleIndex) + CONVERT_INTEGER_TO_NATURAL_NUMBER))
        {
            // получим указатель на контейнер с данными заказчика.
            CDataContainerDataBase* pxDataContainer =
                (CDataContainerDataBase*)GetCustomerDataContainerPointer();
            // бит установлен?
            if ((pxDataContainer -> m_puiDataPointer[BIT_STATE_OFFSET]))
            {
                // если установлен - сбросим.
                (pxDataContainer -> m_puiDataPointer[BIT_STATE_OFFSET]) = 0;
                // получим указатель на контейнер с данными исполнителя..
                pxDataContainer =
                    (CDataContainerDataBase*)GetExecutorDataContainerPointer();
                // передадим драйверу модуля команду - калибровка начала шкалы - НШК.
                (pxDataContainer -> m_puiDataPointer[COMMON_COMMAND_CONTROL_OFFSET]) =
                    MUVR_COMMAND_CONTROL_SET_BOTTOM_OF_SCALE;
            }
        }

        SetFsmState(MODBUS_FUNCTION_5_HANDLER_INPUT_SET_TOP_OR_BOTTOM_OF_SCALE_COMAND_START);
        return 1;
        break;

    // калибровка конца шкалы - ВШК.
    case DEVICE_CONTROL_SET_TOP_OF_SCALE:
        // вычислим индекс модуля в массиве контекста, к которому поступила команда.
        nucIndexNumber = (((((GetResources() -> GetDeviceConfigSearchPointer()) -> uiLastAnalogueInputModuleIndex) +
                            CONVERT_INTEGER_TO_NATURAL_NUMBER) -
                           ((GetResources() -> GetDeviceConfigSearchPointer()) -> uiServiceAnalogueInputModuleQuantity))  +
                          (((uiAddress -
                             COILS_ARRAY_MODBUS_BEGIN_ADDRESS) & CALIBRATION_ANALOG_MODULE_NUMBER_MASK) >> 4));
        // по индексу - nucIndexNumber есть модуль?
        if (nucIndexNumber <=
                (((GetResources() -> GetDeviceConfigSearchPointer()) ->
                  uiLastAnalogueInputModuleIndex) +
                 CONVERT_INTEGER_TO_NATURAL_NUMBER))
        {
            // получим указатель на контейнер с данными заказчика.
            CDataContainerDataBase* pxDataContainer =
                (CDataContainerDataBase*)GetCustomerDataContainerPointer();
            // бит установлен?
            if ((pxDataContainer -> m_puiDataPointer[BIT_STATE_OFFSET]))
            {
                // если установлен - сбросим.
                (pxDataContainer -> m_puiDataPointer[BIT_STATE_OFFSET]) = 0;
                // получим указатель на контейнер с данными исполнителя..
                pxDataContainer =
                    (CDataContainerDataBase*)GetExecutorDataContainerPointer();
                // передадим драйверу модуля команду - калибровка начала шкалы - НШК.
                (pxDataContainer -> m_puiDataPointer[COMMON_COMMAND_CONTROL_OFFSET]) =
                    MUVR_COMMAND_CONTROL_SET_TOP_OF_SCALE;
            }
        }

        SetFsmState(MODBUS_FUNCTION_5_HANDLER_INPUT_SET_TOP_OR_BOTTOM_OF_SCALE_COMAND_START);
        return 1;
        break;

    default:
        break;
    };

////-----------------------------------------------------------------------------------------------------
//// инкремент уставок - SP, OUT, регуляторов модулей токового вывода - MTVI5.
//	// адрес записываемого бита находится в диапазоне инкремента уставок - SP, OUT, регуляторов модулей токового вывода?
//	if (((pxModbusMapping -> current_message_address_bits -
//			COILS_ARRAY_MODBUS_BEGIN_ADDRESS) >= INCREMENT_SP_OUT_BIT_ARRAY_OFFSET) &&
//			((pxModbusMapping -> current_message_address_bits -
//			  COILS_ARRAY_MODBUS_BEGIN_ADDRESS) < (INCREMENT_SP_OUT_BIT_ARRAY_OFFSET +
//					  INCREMENT_SP_OUT_BIT_ARRAY_LENGTH)))
//	{
//		// бит установлен или сброшен?
//		if (pxModbusMapping->tab_bits[uiAddress -
//									  COILS_ARRAY_MODBUS_BEGIN_ADDRESS])
//		{
//			// если установлен - сбросим.
//			pxModbusMapping->tab_bits[uiAddress -
//									  COILS_ARRAY_MODBUS_BEGIN_ADDRESS] = 0;
//			cout << "INCREMENT_SP_OUT_1__BIT_ARRAY_OFFSET 1" << endl;
//			// установим флаг инкремента-декремента уставок - SP, OUT, регуляторов.
//			aucCoilsArray[(pxModbusMapping -> current_message_address_bits -
//						   COILS_ARRAY_MODBUS_BEGIN_ADDRESS)] = 1; //?
//		}
//		else
//		{
//			cout << "INCREMENT_SP_OUT_1__BIT_ARRAY_OFFSET 0" << endl;
//			// сбросим флаг инкремента-декремента уставок - SP, OUT, регуляторов.
//			aucCoilsArray[(pxModbusMapping -> current_message_address_bits -
//						   COILS_ARRAY_MODBUS_BEGIN_ADDRESS)] = 0; //?
//		}
//
//		// вычислим индекс модуля в массиве контекста, к которому поступила команда.
//		nucIndexNumber =  ((((xPlcConfigService.xPlcConfigServiceData.ucLastAnalogueOutputModuleIndex) + 1) -
//							((xPlcConfigService.xPlcConfigServiceData.ucServiceAnalogueOutputQuantity) /
//							 MTVI5_ANALOG_OUTPUT_QUANTITY)) +
//						   ((unsigned char)(((uiAddress -
//											  COILS_ARRAY_MODBUS_BEGIN_ADDRESS) -
//											 INCREMENT_SP_OUT_BIT_ARRAY_OFFSET) /
//											(MTVI5_ANALOG_OUTPUT_QUANTITY))));
//
//		// по индексу - nucIndexNumber есть модуль?
//		if (nucIndexNumber <= (xPlcConfigService.xPlcConfigServiceData.ucLastAnalogueOutputModuleIndex))
//		{
////        if (xPlcConfigService.xPlcConfigServiceData.ucLastAnalogueOutputModuleIndex)
////        {
//			// вычислим номер регулятора модуля токового вывода, в котором инкрементируем-декрементируем уставку.
//			xAllModulesContext.axAllModulesContext[nucIndexNumber].
//			xModuleContextDinamic.
//			ucCommonIndex = (unsigned char)((((uiAddress -
//											   COILS_ARRAY_MODBUS_BEGIN_ADDRESS) -
//											  INCREMENT_SP_OUT_BIT_ARRAY_OFFSET) %
//											 MTVI5_ANALOG_OUTPUT_QUANTITY) +
//											CONVERT_INTEGER_TO_NATURAL_NUMBER);
//
//// в драйвере модуля токового вывода, данные уставок - SP, OUT,
//// поступают из рабочего массива axAnalogueOutputRegulatorContAv[смещение на номер регулятора],
//// после изменения туда же и возвращаются. из этого же массива, по протоколу Modbus,
//// устройства верхнего уровня(HMI панели, системы SCADA, программаторы) получают данные для отображения на дисплее.
////  на текущем шаге обмен данными драйвера с модулем по SPI интерфейсу проходит в режиме одновременной записи-чтения.
//// после записи новые данные на выходе модуля появятся только на следующем шаге записи-чтения.
//// и если на текущем шаге данные с выхода модуля просто вернуть обратно, то никаких изменений не произойдёт.
////  поэтому, после записи в модуль значения уставки, изменённого на величину в переменной usAuxiliaryCounter(выражается в процентах),
//// оно же отправляется в массив источник.
////  так же, это удобно для корректного отображения устройствами верхнего уровня, изменённых данных.
//// так как вызов драйвера модуля токового вывода происходит один раз в секунду,
//// а функци vDeviceControlModbusMessageHandler(), максимум - время обработки самого медленного модуля,
//// то за это время, оператор нажатием кнопки мыши, может инкрементировать-декрементировать значение уставки
//// N - раз. значение - N помещается в переменную usAuxiliaryCounter. в конце каждого цикла обмена данными
//// драйвера с модулем, переменная usAuxiliaryCounter - обнуляется.
//			// изменим значение переменной, на которое в драйвере модуля изменится значение уставки.
//			xAllModulesContext.axAllModulesContext[nucIndexNumber].
//			xModuleContextDinamic.
//			usAuxiliaryCounter += 1;
//
//			// передадим драйверу модуля команду - инкрементировать.
//			xAllModulesContext.axAllModulesContext[nucIndexNumber].
//			xModuleContextDinamic.
//			ucCommandControl = DEVICE_CONTROL_MTVI5_INCREMENT_SP_OUT;
//		}
//	}
//
////-----------------------------------------------------------------------------------------------------
//// декремент уставок - SP, OUT, регуляторов модулей токового вывода - MTVI5.
//// адрес записываемого бита находится в диапазоне декремента уставок - SP, OUT, регуляторов модулей токового вывода?
//	if (((pxModbusMapping -> current_message_address_bits -
//			COILS_ARRAY_MODBUS_BEGIN_ADDRESS) >= DECREMENT_SP_OUT_BIT_ARRAY_OFFSET) &&
//			((pxModbusMapping -> current_message_address_bits -
//			  COILS_ARRAY_MODBUS_BEGIN_ADDRESS) < (DECREMENT_SP_OUT_BIT_ARRAY_OFFSET +
//					  DECREMENT_SP_OUT_BIT_ARRAY_LENGTH)))
//	{
//		// бит установлен или сброшен?
//		if (pxModbusMapping->tab_bits[uiAddress -
//									  COILS_ARRAY_MODBUS_BEGIN_ADDRESS])
//		{
//			// если установлен - сбросим.
//			pxModbusMapping->tab_bits[uiAddress -
//									  COILS_ARRAY_MODBUS_BEGIN_ADDRESS] = 0;
//			cout << "DECREMENT_SP_OUT_BIT_ARRAY_OFFSET 1" << endl;
//			// установим флаг инкремента-декремента уставок - SP, OUT, регуляторов.
//			aucCoilsArray[(pxModbusMapping -> current_message_address_bits -
//						   COILS_ARRAY_MODBUS_BEGIN_ADDRESS)] = 1; //?
//		}
//		else
//		{
//			cout << "DECREMENT_SP_OUT_BIT_ARRAY_OFFSET 0" << endl;
//			// сбросим флаг инкремента- декремента уставок - SP, OUT, регуляторов.
//			aucCoilsArray[(pxModbusMapping -> current_message_address_bits -
//						   COILS_ARRAY_MODBUS_BEGIN_ADDRESS)] = 0; //?
//		}
//
//		// вычислим индекс модуля в массиве контекста, к которому поступила команда.
//		nucIndexNumber =  ((((xPlcConfigService.xPlcConfigServiceData.ucLastAnalogueOutputModuleIndex) + 1) -
//							((xPlcConfigService.xPlcConfigServiceData.ucServiceAnalogueOutputQuantity) /
//							 MTVI5_ANALOG_OUTPUT_QUANTITY)) +
//						   ((unsigned char)(((uiAddress -
//											  COILS_ARRAY_MODBUS_BEGIN_ADDRESS) -
//											 DECREMENT_SP_OUT_BIT_ARRAY_OFFSET) /
//											(MTVI5_ANALOG_OUTPUT_QUANTITY))));
//		// по индексу - nucIndexNumber есть модуль?
//		if (nucIndexNumber <= (xPlcConfigService.xPlcConfigServiceData.ucLastAnalogueOutputModuleIndex))
//		{
//			// вычислим номер регулятора модуля токового вывода, в котором инкрементируем-декрементируем уставку.
//			xAllModulesContext.axAllModulesContext[nucIndexNumber].
//			xModuleContextDinamic.
//			ucCommonIndex = (unsigned char)((((uiAddress -
//											   COILS_ARRAY_MODBUS_BEGIN_ADDRESS) -
//											  DECREMENT_SP_OUT_BIT_ARRAY_OFFSET) %
//											 MTVI5_ANALOG_OUTPUT_QUANTITY) +
//											CONVERT_INTEGER_TO_NATURAL_NUMBER);
//
//// в драйвере модуля токового вывода, данные уставок - SP, OUT,
//// поступают из рабочего массива axAnalogueOutputRegulatorContAv[смещение на номер регулятора],
//// после изменения туда же и возвращаются. из этого же массива, по протоколу Modbus,
//// устройства верхнего уровня(HMI панели, системы SCADA, программаторы) получают данные для отображения на дисплее.
////  на текущем шаге обмен данными драйвера с модулем по SPI интерфейсу проходит в режиме одновременной записи-чтения.
//// после записи новые данные на выходе модуля появятся только на следующем шаге записи-чтения.
//// и если на текущем шаге данные с выхода модуля просто вернуть обратно, то никаких изменений не произойдёт.
////  поэтому, после записи в модуль значения уставки, изменённого на величину в переменной usAuxiliaryCounter(выражается в процентах),
//// оно же отправляется в массив источник.
////  так же, это удобно для корректного отображения устройствами верхнего уровня, изменённых данных.
//// так как вызов драйвера модуля токового вывода происходит один раз в секунду,
//// а функци vDeviceControlModbusMessageHandler() - максимум время обработки самого медленного модуля,
//// то за это время, оператор нажатием кнопки мыши, может инкрементировать-декрементировать значение уставки
//// N - раз. значение - N помещается в переменную usAuxiliaryCounter. в конце каждого цикла обмена данными
//// драйвера с модулем, переменная usAuxiliaryCounter - обнуляется.
//			// изменим значение переменной, на которое в драйвере модуля изменится значение уставки.
//			xAllModulesContext.axAllModulesContext[nucIndexNumber].
//			xModuleContextDinamic.
//			usAuxiliaryCounter += 1;
//
//			// передадим драйверу модуля команду - декрементировать.
//			xAllModulesContext.axAllModulesContext[nucIndexNumber].
//			xModuleContextDinamic.
//			ucCommandControl = DEVICE_CONTROL_MTVI5_DECREMENT_SP_OUT;
//		}
//	}
//
////-----------------------------------------------------------------------------------------------------
//// запись дискретных сигналов регуляторов(CONT_ST), модулей токового вывода - MTVI5.
//	// адрес записываемого бита находится в диапазоне массива дискретных сигналов регуляторов(CONT_ST) модулей токового вывода?
//	if (((pxModbusMapping -> current_message_address_bits -
//			COILS_ARRAY_MODBUS_BEGIN_ADDRESS) >= CONT_ST_BIT_ARRAY_OFFSET) &&
//			((pxModbusMapping -> current_message_address_bits -
//			  COILS_ARRAY_MODBUS_BEGIN_ADDRESS) < (CONT_ST_BIT_ARRAY_OFFSET +
//					  CONT_ST_BIT_ARRAY_LENGTH)))
//	{
//		// бит установлен или сброшен?
//		if (pxModbusMapping->tab_bits[uiAddress -
//									  COILS_ARRAY_MODBUS_BEGIN_ADDRESS])
//		{
//			// если установлен - сбросим.
//			pxModbusMapping->tab_bits[uiAddress -
//									  COILS_ARRAY_MODBUS_BEGIN_ADDRESS] = 0;
//			cout << "CONT_ST_BIT_ARRAY 1" << endl;
//			// установим бит дискретных сигналов регуляторов.
////            aucCoilsArray[(pxModbusMapping -> current_message_address_bits -
////                           COILS_ARRAY_MODBUS_BEGIN_ADDRESS)] = 1; //?
//			aucContStWriteBitArray[(pxModbusMapping -> current_message_address_bits -
//									COILS_ARRAY_MODBUS_BEGIN_ADDRESS) -
//								   CONT_ST_BIT_ARRAY_OFFSET] = 1;
//		}
//		else
//		{
//			cout << "CONT_ST_BIT_ARRAY 0" << endl;
//			// сбросим бит дискретных сигналов регуляторов.
////            aucCoilsArray[(pxModbusMapping -> current_message_address_bits -
////                           COILS_ARRAY_MODBUS_BEGIN_ADDRESS)] = 0; //?
//			aucContStWriteBitArray[(pxModbusMapping -> current_message_address_bits -
//									COILS_ARRAY_MODBUS_BEGIN_ADDRESS) -
//								   CONT_ST_BIT_ARRAY_OFFSET] = 0;
//		}
//
////        vBitToByteArray(&aucContStWriteByteArray[0],
////                        &aucCoilsArray[CONT_ST_BIT_ARRAY_OFFSET],
////                        CONT_ST_BYTE_ARRAY_LENGTH);
//		vBitToByteArray(aucContStWriteByteArray,
//						aucContStWriteBitArray,
//						CONT_ST_BYTE_ARRAY_LENGTH);
//
//		// вычислим индекс модуля в массиве контекста, к которому поступила команда.
//		nucIndexNumber =  ((((xPlcConfigService.xPlcConfigServiceData.ucLastAnalogueOutputModuleIndex) + 1) -
//							((xPlcConfigService.xPlcConfigServiceData.ucServiceAnalogueOutputQuantity) /
//							 MTVI5_ANALOG_OUTPUT_QUANTITY)) +
//						   ((unsigned char)(((uiAddress -
//											  COILS_ARRAY_MODBUS_BEGIN_ADDRESS) -
//											 CONT_ST_BIT_ARRAY_OFFSET) /
//											(MTVI5_STAT_BIT_QUANTITY * MTVI5_ANALOG_OUTPUT_QUANTITY))));
//		// по индексу - nucIndexNumber есть модуль?
//		if (nucIndexNumber <= (xPlcConfigService.xPlcConfigServiceData.ucLastAnalogueOutputModuleIndex))
//		{
////            // передадим драйверу модуля команду - запись дискретных сигналов регуляторов.
////            xAllModulesContext.axAllModulesContext[nucIndexNumber].
////            xModuleContextDinamic.
////            ucCommandControl = DEVICE_CONTROL_MTVI5_CONT_ST_WRITE;
//		}
//	}
//
////-----------------------------------------------------------------------------------------------------
//// запись команд управления верхнего уровня.
//	// адрес записываемого бита находится в диапазоне массива команд управления верхнего уровня?
//	if (((pxModbusMapping -> current_message_address_bits -
//			COILS_ARRAY_MODBUS_BEGIN_ADDRESS) >= LOG_COM_BIT_ARRAY_OFFSET) &&
//			((pxModbusMapping -> current_message_address_bits -
//			  COILS_ARRAY_MODBUS_BEGIN_ADDRESS) < (LOG_COM_BIT_ARRAY_OFFSET +
//					  LOG_COM_BIT_ARRAY_LENGTH)))
//	{
//		// бит установлен или сброшен?
//		if (pxModbusMapping->tab_bits[uiAddress -
//									  COILS_ARRAY_MODBUS_BEGIN_ADDRESS])
//		{
//			// если установлен - сбросим.
//			pxModbusMapping->tab_bits[uiAddress -
//									  COILS_ARRAY_MODBUS_BEGIN_ADDRESS] = 0;
//			cout << "LOG_COM_BIT_ARRAY_OFFSET 1" << endl;
//			// установим бит команд управления верхнего уровня.
//			aucCoilsArray[(pxModbusMapping -> current_message_address_bits -
//						   COILS_ARRAY_MODBUS_BEGIN_ADDRESS)] = 1;
//		}
//		else
//		{
//			cout << "LOG_COM_BIT_ARRAY_OFFSET 0" << endl;
//			// сбросим бит команд управления верхнего уровня.
//			aucCoilsArray[(pxModbusMapping -> current_message_address_bits -
//						   COILS_ARRAY_MODBUS_BEGIN_ADDRESS)] = 0;
//		}
//	}
//
////-----------------------------------------------------------------------------------------------------
//// выводит из обработки аналоговый вход.
////    // сообщение пришло с интерфейса ModbusRTU HMI?
////    if ((pxModbusMapping -> message_sourse) ==
////            MODBUS_MESSAGE_SOURCE_RTU_HMI)
//	//    {
//	// адрес записываемого бита находится в диапазоне массива вывода из обработки аналоговых входов?
//	if (((pxModbusMapping -> current_message_address_bits -
//			COILS_ARRAY_MODBUS_BEGIN_ADDRESS) >= AIN_OFF_BIT_ARRAY_OFFSET) &&
//			((pxModbusMapping -> current_message_address_bits -
//			  COILS_ARRAY_MODBUS_BEGIN_ADDRESS) < (AIN_OFF_BIT_ARRAY_OFFSET +
//					  AIN_OFF_BIT_ARRAY_LENGTH)))
//	{
//		// перед этим кто либо другой не вывел из обработки аналоговый вход?
//		if ((xMainFlagRegister.ui8AinOffProcessOwnerIndex == AIN_OFF_PROCESS_OWNER_IS_NONE) ||
//				(xMainFlagRegister.ui8AinOffProcessOwnerIndex == AIN_OFF_PROCESS_OWNER_IS_HMI))
//		{
//			// вычислим индекс модуля в массиве контекста, к которому поступила команда.
//			nucIndexNumber =  ((((xPlcConfigService.xPlcConfigServiceData.ucLastAnalogueInputModuleIndex) + 1) -
//								(xPlcConfigService.xPlcConfigServiceData.ucServiceAnalogueInputModuleQuantity)) +
//							   ((unsigned char)(((uiAddress -
//												  COILS_ARRAY_MODBUS_BEGIN_ADDRESS) -
//												 AIN_OFF_BIT_ARRAY_OFFSET) /
//												(MVAI5_ANALOG_INPUT_QUANTITY))));
//			// по индексу - nucIndexNumber есть модуль?
//			if (nucIndexNumber <= (xPlcConfigService.xPlcConfigServiceData.ucLastAnalogueInputModuleIndex))
//			{
//				// вернём в рабочее состояние выведенный из обработки аналоговый вход.
//				for (i = 0; i < MAX_HANDLED_ANALOGUE_INPUT; i++)
//				{
//					// аналоговый вход - i выведен из обработки?
//					if (aucCoilsArray[AIN_OFF_BIT_ARRAY_OFFSET + i])
//					{
//						// вернём в рабочее состояние выведенный из обработки аналоговый вход - i.
//						aucCoilsArray[AIN_OFF_BIT_ARRAY_OFFSET + i] = 0;
////                        // вычислим индекс модуля в массиве контекста, который в текущий момент выполняет команду.
////                        nucIndexNumber =  ((((xPlcConfigService.xPlcConfigServiceData.ucLastAnalogueInputModuleIndex) + 1) -
////                                            (xPlcConfigService.xPlcConfigServiceData.ucServiceAnalogueInputModuleQuantity)) +
////                                           ((unsigned char)((i) /
////                                                            (MVAI5_ANALOG_INPUT_QUANTITY))));
//						// передадим драйверу модуля номер калибруемого входа. если 0, калибровка выключена.
//						// выключим калибровку всех входов модуля;
//						xAllModulesContext.axAllModulesContext[
//							((((xPlcConfigService.xPlcConfigServiceData.ucLastAnalogueInputModuleIndex) + 1) -
//							  (xPlcConfigService.xPlcConfigServiceData.ucServiceAnalogueInputModuleQuantity)) +
//							 ((unsigned char)((i) /
//											  (MVAI5_ANALOG_INPUT_QUANTITY))))].
//						xModuleContextDinamic.
//						ucCommonIndex = 0;
//					}
//				}
//
//				// бит установлен или сброшен?
//				if (pxModbusMapping->tab_bits[uiAddress -
//											  COILS_ARRAY_MODBUS_BEGIN_ADDRESS])
//				{
//					// включение режима калибровки.
//					cout << "AIN_OFF_BIT_ARRAY_OFFSET 1" << endl;
//					// если установлен - сбросим.
//					pxModbusMapping->tab_bits[uiAddress -
//											  COILS_ARRAY_MODBUS_BEGIN_ADDRESS] = 0;
//					// вычислим и передадим драйверу модуля номер калибруемого входа.
//					xAllModulesContext.axAllModulesContext[nucIndexNumber].
//					xModuleContextDinamic.
//					ucCommonIndex = (unsigned char)((((uiAddress -
//													   COILS_ARRAY_MODBUS_BEGIN_ADDRESS) -
//													  AIN_OFF_BIT_ARRAY_OFFSET) %
//													 MVAI5_ANALOG_INPUT_QUANTITY) +
//													CONVERT_INTEGER_TO_NATURAL_NUMBER);
//					// выведем аналоговый вход из обработки.
//					aucCoilsArray[(pxModbusMapping -> current_message_address_bits) -
//								  COILS_ARRAY_MODBUS_BEGIN_ADDRESS] = 1;
//					xMainFlagRegister.ui8AinOffProcessOwnerIndex = AIN_OFF_PROCESS_OWNER_IS_HMI;
//
////                    aucDoValueBlockByteArray[(pxModbusMapping -> current_message_address_bits) -
////                                             COILS_ARRAY_MODBUS_BEGIN_ADDRESS - AIN_OFF_BIT_ARRAY_OFFSET] = 1;
////                    ui8TempCommand = xAllModulesContext.axAllModulesContext[nucIndexNumber].
////                                     xModuleContextDinamic.
////                                     ucCommonIndex;
//				}
//				else
//				{
//					// выключение режима калибровки.
//					cout << "AIN_OFF_BIT_ARRAY_OFFSET 0" << endl;
//					xMainFlagRegister.ui8AinOffProcessOwnerIndex = AIN_OFF_PROCESS_OWNER_IS_NONE;
//
////                    aucDoValueBlockByteArray[(pxModbusMapping -> current_message_address_bits) -
////                                             COILS_ARRAY_MODBUS_BEGIN_ADDRESS - AIN_OFF_BIT_ARRAY_OFFSET] = 0;
////                    ui8TempCommand = 0;
//				}
//			}
//
//
//		}
//	}
//
////-----------------------------------------------------------------------------------------------------
//// принимает команду - запись конфигурации.
//	if (((pxModbusMapping -> current_message_address_bits -
//			COILS_ARRAY_MODBUS_BEGIN_ADDRESS) ==
//			CONFIGURATION_SAVE_COMMAND_BIT_ARRAY_OFFSET))
//	{
//		// бит установлен или сброшен?
//		if (pxModbusMapping->tab_bits[uiAddress -
//									  COILS_ARRAY_MODBUS_BEGIN_ADDRESS])
//		{
//			// если установлен - сбросим.
//			pxModbusMapping->tab_bits[uiAddress -
//									  COILS_ARRAY_MODBUS_BEGIN_ADDRESS] = 0;
//			cout << "CONFIGURATION_SAVE_COMMAND_BIT_ARRAY_OFFSET 1" << endl;
//
//			switch(xPlcConfigDataTransmit.xConfigData.ucErrorCode)
//			{
//			case CONFIGURATION_ERROR_SEQUENCE_ERROR:
//			case CONFIGURATION_ERROR_EMPTY_SLOT:
//				break;
//
//			case CONFIGURATION_ERROR_NEW_CONFIGURATION_AND_DATA_BASE:
////            iPlcConfigurationCreate();
////            vMainTaskMemoryMapFormation();
//				// сохранение конфигурации и базы данных.
//				iPlcConfigurationSave();
//				iPlcDataBaseSave();
////    vWorkArraysUpdate();
//				iBadModuleBuffClear();
//				// сброс флагов ошибок конфигурации и базы данных,
//				// для перехода на нормальную циклическую работу.
//				fbPlcConfigurationError = 0;
//				fbPlcConfigurationFileError = 0;
//				fbPlcDataBaseFileError = 0;
//				// установим флаг "перезагрузка программы".
//				fbPlcErrorReset = 1;
//				xPlcConfigDataTransmit.xConfigData.ucErrorCode = 0;
//
//				// зарегистрируем событие.
//				CEvents::EventRegistration(
//					0,
//					(CEvents::HANDLED_EVENTS_SYSTEM_EVENTS_TYPE |
//					 CEvents::HANDLED_EVENTS_IS_ARCHIVE),
//					0,
//					"Новая конф. и бд.");
//
//				cout << "CONFIGURATION_ERROR_NEW_CONFIGURATION_AND_DATA_BASE 2" << endl;
//				break;
//			case CONFIGURATION_ERROR_NEW_CONFIGURATION:
////            iPlcConfigurationCreate();
////            vMainTaskMemoryMapFormation();
//				// сохранение конфигурации и базы данных.
//				iPlcConfigurationSave();
//				//                iPlcDataBaseSave();
////    vWorkArraysUpdate();
//				iBadModuleBuffClear();
//				// сброс флагов ошибок конфигурации и базы данных,
//				// для перехода на нормальную циклическую работу.
//				fbPlcConfigurationError = 0;
//				fbPlcConfigurationFileError = 0;
//				fbPlcDataBaseFileError = 0;
//				// установим флаг "перезагрузка программы".
//				fbPlcErrorReset = 1;
//				xPlcConfigDataTransmit.xConfigData.ucErrorCode = 0;
//
//				// зарегистрируем событие.
//				CEvents::EventRegistration(
//					0,
//					(CEvents::HANDLED_EVENTS_SYSTEM_EVENTS_TYPE |
//					 CEvents::HANDLED_EVENTS_IS_ARCHIVE),
//					0,
//					"Новая конфигурация.");
//
//				cout << "CONFIGURATION_ERROR_NEW_CONFIGURATION 3" << endl;
//				break;
//			case CONFIGURATION_ERROR_BAD_MODULE_FINDED_AND_REGISTERED:
//				// сброс флагов ошибок конфигурации и базы данных,
//				// для перехода на нормальную циклическую работу.
//				fbPlcConfigurationError = 0;
//				fbPlcConfigurationFileError = 0;
//				fbPlcDataBaseFileError = 0;
//				// установим флаг "перезагрузка программы".
//				fbPlcErrorReset = 1;
//				break;
//
//			default:
//				break;
//			};
//
//		}
//		else
//		{
//			cout << "CONFIGURATION_SAVE_COMMAND_BIT_ARRAY_OFFSET 0" << endl;
//			// сбросим бит запись - конфигурации.
//		}
//	}

    return 0;
}

//-------------------------------------------------------------------------------
uint16_t CDeviceControl::DataBaseBlockReadBlockRelatedAction(void)
{
    std::cout << "CDeviceControl::DataBaseBlockReadBlockRelatedAction 1" << std::endl;

    // номер принятого для записи блока базы данных.
    uint8_t uiBlockIndex =
        (((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiDataIndex);
    std::cout << "CDeviceControl::DataBaseBlockReadBlockRelatedAction  uiBlockIndex "  << (int)uiBlockIndex << std::endl;


    switch(uiBlockIndex)
    {
    case ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_OFFSET:
        cout << "CDeviceControl::DataBaseBlockReadBlockRelatedAction ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_OFFSET" << endl;
        {
            SetFsmState(DATA_BASE_BLOCK_READ_MODULE_MUVR_DATA_BASE_READ_START);
        }
        break;

    case DISCRETE_INPUT_SYGNALS_DATA_BASE_BLOCK_OFFSET:
        cout << "CDeviceControl::DataBaseBlockReadBlockRelatedAction DISCRETE_INPUT_SYGNALS_DATA_BASE_BLOCK_OFFSET" << endl;
        ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
        SetFsmState(DONE_OK);
        break;

    case FUNCTION_BLOCK_DATA_BASE_BLOCK_OFFSET:
        cout << "CDeviceControl::DataBaseBlockReadBlockRelatedAction FUNCTION_BLOCK_DATA_BASE_BLOCK_OFFSET" << endl;
        ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
        SetFsmState(DONE_OK);
        break;

    case CURRENT_OUTPUT_MODULE_REGULATOR_DATA_BASE_BLOCK_OFFSET:
        cout << "CDeviceControl::DataBaseBlockReadBlockRelatedAction CURRENT_OUTPUT_MODULE_REGULATOR_DATA_BASE_BLOCK_OFFSET" << endl;
        ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
        SetFsmState(DONE_OK);
        break;

    case CURRENT_OUTPUT_MODULE_PSP_DATA_BASE_BLOCK_OFFSET:
        cout << "CDeviceControl::DataBaseBlockReadBlockRelatedAction CURRENT_OUTPUT_MODULE_PSP_DATA_BASE_BLOCK_OFFSET" << endl;
        ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
        SetFsmState(DONE_OK);
        break;

    case DIMENSIONS_PARAMETERS_DATA_BASE_BLOCK_OFFSET:
        cout << "CDeviceControl::DataBaseBlockReadBlockRelatedAction DIMENSIONS_PARAMETERS_DATA_BASE_BLOCK_OFFSET" << endl;
        ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
        SetFsmState(DONE_OK);
        break;

    case TEXT_TITLES_DATA_BASE_BLOCK_OFFSET:
        cout << "CDeviceControl::DataBaseBlockReadBlockRelatedAction TEXT_TITLES_DATA_BASE_BLOCK_OFFSET" << endl;
        ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
        SetFsmState(DONE_OK);
        break;

    case REFERENCE_POINTS_ADC_CODES_DATA_BASE_BLOCK_OFFSET:
        cout << "CDeviceControl::DataBaseBlockReadBlockRelatedAction REFERENCE_POINTS_ADC_CODES_DATA_BASE_BLOCK_OFFSET" << endl;
        {
            SetFsmState(DATA_BASE_BLOCK_READ_REFERENCE_POINTS_ADC_CODES_MODULE_MUVR_DATA_READ_START);
        }
        break;

    case NETWORK_ADDRESS_DATA_BASE_BLOCK_OFFSET:
        cout << "CDeviceControl::DataBaseBlockReadBlockRelatedAction NETWORK_ADDRESS_DATA_BASE_BLOCK_OFFSET" << endl;
        {
            SetFsmState(DATA_BASE_BLOCK_NETWORK_ADDRESS_WRITE_START);
        }
        break;

    case CONFIGURATION_DATA_BASE_BLOCK_OFFSET:
        cout << "CDeviceControl::DataBaseBlockReadBlockRelatedAction CONFIGURATION_DATA_BASE_BLOCK_OFFSET" << endl;
        ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
        SetFsmState(DONE_OK);
        break;

    default:
        cout << "CDeviceControl::DataBaseBlockReadBlockRelatedAction default" << endl;
        ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
        SetFsmState(DONE_OK);
        break;
    };

    return 0;
}

//-------------------------------------------------------------------------------
uint16_t CDeviceControl::DataBaseBlockWriteBlockRelatedAction(void)
{
    std::cout << "CDeviceControl::DataBaseBlockWriteBlockRelatedAction 1" << std::endl;

    // номер принятого для записи блока базы данных.
    uint8_t uiBlockIndex =
        (((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiDataIndex);
    std::cout << "CDeviceControl::DataBaseBlockWriteBlockRelatedAction  uiBlockIndex "  << (int)uiBlockIndex << std::endl;


    switch(uiBlockIndex)
    {
    case ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_OFFSET:
        cout << "CDeviceControl::DataBaseBlockWriteBlockRelatedAction ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_OFFSET" << endl;
        {
            SetFsmState(DATA_BASE_BLOCK_MODULE_MUVR_WRITE_START);
        }
        break;

    case DISCRETE_INPUT_SYGNALS_DATA_BASE_BLOCK_OFFSET:
        cout << "CDeviceControl::DataBaseBlockWriteBlockRelatedAction DISCRETE_INPUT_SYGNALS_DATA_BASE_BLOCK_OFFSET" << endl;
        ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
        SetFsmState(DONE_OK);
        break;

    case FUNCTION_BLOCK_DATA_BASE_BLOCK_OFFSET:
        cout << "CDeviceControl::DataBaseBlockWriteBlockRelatedAction FUNCTION_BLOCK_DATA_BASE_BLOCK_OFFSET" << endl;
        ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
        SetFsmState(DONE_OK);
        break;

    case CURRENT_OUTPUT_MODULE_REGULATOR_DATA_BASE_BLOCK_OFFSET:
        cout << "CDeviceControl::DataBaseBlockWriteBlockRelatedAction CURRENT_OUTPUT_MODULE_REGULATOR_DATA_BASE_BLOCK_OFFSET" << endl;
        ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
        SetFsmState(DONE_OK);
        break;

    case CURRENT_OUTPUT_MODULE_PSP_DATA_BASE_BLOCK_OFFSET:
        cout << "CDeviceControl::DataBaseBlockWriteBlockRelatedAction CURRENT_OUTPUT_MODULE_PSP_DATA_BASE_BLOCK_OFFSET" << endl;
        ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
        SetFsmState(DONE_OK);
        break;

    case DIMENSIONS_PARAMETERS_DATA_BASE_BLOCK_OFFSET:
        cout << "CDeviceControl::DataBaseBlockWriteBlockRelatedAction DIMENSIONS_PARAMETERS_DATA_BASE_BLOCK_OFFSET" << endl;
        ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
        SetFsmState(DONE_OK);
        break;

    case TEXT_TITLES_DATA_BASE_BLOCK_OFFSET:
        cout << "CDeviceControl::DataBaseBlockWriteBlockRelatedAction TEXT_TITLES_DATA_BASE_BLOCK_OFFSET" << endl;
        ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
        SetFsmState(DONE_OK);
        break;

    case REFERENCE_POINTS_ADC_CODES_DATA_BASE_BLOCK_OFFSET:
        cout << "CDeviceControl::DataBaseBlockReadBlockRelatedAction REFERENCE_POINTS_ADC_CODES_DATA_BASE_BLOCK_OFFSET" << endl;
        ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
        SetFsmState(DONE_OK);
        break;

    case NETWORK_ADDRESS_DATA_BASE_BLOCK_OFFSET:
        cout << "CDeviceControl::DataBaseBlockWriteBlockRelatedAction NETWORK_ADDRESS_DATA_BASE_BLOCK_OFFSET" << endl;
        {
            SetFsmState(DATA_BASE_BLOCK_NETWORK_ADDRESS_WRITE_START);
        }
        break;

    case CONFIGURATION_DATA_BASE_BLOCK_OFFSET:
        cout << "CDeviceControl::DataBaseBlockWriteBlockRelatedAction CONFIGURATION_DATA_BASE_BLOCK_OFFSET" << endl;
        ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
        SetFsmState(DONE_OK);
        break;

    default:
        cout << "CDeviceControl::DataBaseBlockWriteBlockRelatedAction default" << endl;
        ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
        SetFsmState(DONE_OK);
        break;
    };

    return 0;
}

//-------------------------------------------------------------------------------
uint8_t CDeviceControl::Fsm(void)
{
//    std::cout << "CDeviceControl::Fsm 1" << endl;
    uint8_t uiReadyTaskCounter = 0;
    switch (GetFsmState())
    {
    case IDDLE:
//        std::cout << "CDeviceControl::Fsm IDDLE"  << std::endl;
        break;

    case STOP:
//        //std::cout << "CDeviceControl::Fsm STOP"  << std::endl;
        break;

    case START:
        std::cout << "CDeviceControl::Fsm START"  << std::endl;
        Init();
        GetTimerPointer() -> Set(TASK_READY_WAITING_TIME);
        SetFsmState(INIT);
        break;

    case INIT:
//        std::cout << "CDeviceControl::Fsm INIT 1"  << std::endl;
    {
        CTaskInterface* pxTask =
            GetResources() ->
            GetTaskPointerByNameFromMap(m_sDataStoreName);

        if (pxTask != 0)
        {
//                std::cout << "CDeviceControl::Fsm INIT 2"  << std::endl;
            if (pxTask -> GetFsmState() >= READY)
            {
//                    std::cout << "CDeviceControl::Fsm INIT 3"  << std::endl;
                SetDataStore((CDataStore*)pxTask);
                uiReadyTaskCounter += 1;
//                    SetFsmState(READY);
            }
        }
        else
        {
//                std::cout << "CDeviceControl::Fsm INIT 4"  << std::endl;
            if (GetTimerPointer() -> IsOverflow())
            {
                std::cout << "CDeviceControl::Fsm INIT 5"  << std::endl;
                SetFsmState(STOP);
            }
        }
    }

    if (uiReadyTaskCounter > 0)
    {
        SetFsmState(READY);
    }


    m_uiConfigurationCreateId =
        GetResources() ->
        GetTaskIdByNameFromMap(m_sConfigurationCreateName);
    break;

    case READY:
//        std::cout << "CDeviceControl::Fsm READY"  << std::endl;
//    {
//
//        if (GetCommandDataLink() != 0)
//        {
//            std::cout << "CDeviceControl::Fsm READY 2"  << std::endl;
//            SetOperatingDataLink(GetCommandDataLink());
//            SetFsmState(GetCommandDataLink() ->
//                        GetDataContainerPointer() ->
//                        GetFsmCommandState());
//            GetCommandDataLink() ->
//            GetDataContainerPointer() ->
//            SetFsmCommandState(0);
//            SetCommandDataLink(0);
//        }
//    }
        break;

    case DONE_OK:
//        std::cout << "CDeviceControl::Fsm DONE_OK"  << std::endl;
//        SetFsmOperationStatus(DONE_OK);
//        SetFsmState(READY);
        break;

    case DONE_ERROR:
//        std::cout << "CDeviceControl::Fsm DONE_ERROR"  << std::endl;
//        SetFsmOperationStatus(DONE_ERROR);
//        SetFsmState(READY);
        break;

//-------------------------------------------------------------------------------
    case SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_START:
        std::cout << "CDeviceControl::Fsm SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_START"  << std::endl;
        {
            GetTimerPointer() -> Set(TASK_READY_WAITING_TIME);
            SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_WAITING);
        }
        break;

    case SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_WAITING:
//        std::cout << "CDeviceControl::Fsm SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_WAITING 1"  << std::endl;
    {
        if (SetTaskData(GetExecutorDataContainerPointer()))
        {
            std::cout << "CDeviceControl::Fsm SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_WAITING 2"  << std::endl;
//            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
            SetFsmState(GetFsmNextStateDoneOk());
        }
        else
        {
            std::cout << "CDeviceControl::Fsm SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_WAITING 3"  << std::endl;
            // Время ожидания выполнения запроса закончилось?
            if (GetTimerPointer() -> IsOverflow())
            {
                std::cout << "CDeviceControl::Fsm SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_WAITING 4"  << std::endl;
//                ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
                SetFsmState(GetFsmNextStateReadyWaitingError());
            }
        }
    }
    break;

//-------------------------------------------------------------------------------
    case SUBTASK_EXECUTOR_READY_CHECK_START:
        std::cout << "CDeviceControl::Fsm SUBTASK_EXECUTOR_READY_CHECK_START"  << std::endl;
        {
            GetTimerPointer() -> Set(TASK_READY_WAITING_TIME);
            SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_WAITING);
        }
        break;

    case SUBTASK_EXECUTOR_READY_CHECK_WAITING:
//        std::cout << "CDeviceControl::Fsm SUBTASK_EXECUTOR_READY_CHECK_WAITING 1"  << std::endl;
    {
        if (SetTaskData(GetExecutorDataContainerPointer()))
        {
            std::cout << "CDeviceControl::Fsm SUBTASK_EXECUTOR_READY_CHECK_WAITING 2"  << std::endl;
            SetFsmState(SUBTASK_EXECUTOR_DONE_CHECK_START);
        }
        else
        {
            std::cout << "CDeviceControl::Fsm SUBTASK_EXECUTOR_READY_CHECK_WAITING 3"  << std::endl;
            // Время ожидания выполнения запроса закончилось?
            if (GetTimerPointer() -> IsOverflow())
            {
                std::cout << "CDeviceControl::Fsm SUBTASK_EXECUTOR_READY_CHECK_WAITING 4"  << std::endl;
//                ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
                SetFsmState(GetFsmNextStateReadyWaitingError());
            }
        }
    }
    break;

    case SUBTASK_EXECUTOR_DONE_CHECK_START:
        std::cout << "CDeviceControl::Fsm SUBTASK_EXECUTOR_DONE_CHECK_START 1"  << std::endl;
        {
            GetTimerPointer() -> Set(TASK_READY_WAITING_TIME);
            SetFsmState(SUBTASK_EXECUTOR_DONE_CHECK_WAITING);
        }

        break;

    case SUBTASK_EXECUTOR_DONE_CHECK_WAITING:
//        std::cout << "CDeviceControl::Fsm SUBTASK_EXECUTOR_DONE_CHECK_WAITING 1"  << std::endl;
    {
        CDataContainerDataBase* pxDataContainer =
            (CDataContainerDataBase*)GetExecutorDataContainerPointer();

        uint8_t uiFsmState = pxDataContainer -> m_uiFsmCommandState;

        if (uiFsmState == DONE_OK)
        {
            std::cout << "CDeviceControl::Fsm SUBTASK_EXECUTOR_DONE_CHECK_WAITING 2"  << std::endl;
//            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
            SetFsmState(GetFsmNextStateDoneOk());
        }
        else if (uiFsmState == DONE_ERROR)
        {
            std::cout << "CDeviceControl::Fsm SUBTASK_EXECUTOR_DONE_CHECK_WAITING 3"  << std::endl;
//            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
            SetFsmState(GetFsmNextStateDoneWaitingDoneError());
        }
        else
        {
            // Время ожидания выполнения запроса закончилось?
            if (GetTimerPointer() -> IsOverflow())
            {
                std::cout << "CDeviceControl::Fsm SUBTASK_EXECUTOR_DONE_CHECK_WAITING 4"  << std::endl;
//                ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
                SetFsmState(GetFsmNextStateDoneWaitingError());
            }
        }
    }
    break;

//-------------------------------------------------------------------------------
    case CONFIGURATION_REQUEST_START:
        std::cout << "CDeviceControl::Fsm CONFIGURATION_REQUEST_START"  << std::endl;
        {
            CDataContainerDataBase* pxDataContainer =
                (CDataContainerDataBase*)GetExecutorDataContainerPointer();
            pxDataContainer -> m_uiTaskId = m_uiConfigurationCreateId;
            pxDataContainer -> m_uiFsmCommandState =
                CConfigurationCreate::CONFIGURATION_REQUEST_START;
            pxDataContainer -> m_puiDataPointer = m_puiIntermediateBuff;

            SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_START);
            SetFsmNextStateDoneOk(CONFIGURATION_REQUEST_EXECUTOR_DONE_OK_ANSWER_PROCESSING);
            SetFsmNextStateReadyWaitingError(CONFIGURATION_REQUEST_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
            SetFsmNextStateDoneWaitingError(CONFIGURATION_REQUEST_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
            SetFsmNextStateDoneWaitingDoneError(CONFIGURATION_REQUEST_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
        }
        break;

    case CONFIGURATION_REQUEST_EXECUTOR_DONE_OK_ANSWER_PROCESSING:
        std::cout << "CDeviceControl::Fsm CONFIGURATION_REQUEST_EXECUTOR_DONE_OK_ANSWER_PROCESSING"  << std::endl;
        {
            CDataContainerDataBase* pxExecutorDataContainer =
                (CDataContainerDataBase*)GetExecutorDataContainerPointer();
            CDataContainerDataBase* pxCustomerDataContainer =
                (CDataContainerDataBase*)GetCustomerDataContainerPointer();

            memcpy(pxCustomerDataContainer -> m_puiDataPointer,
                   (pxExecutorDataContainer -> m_puiDataPointer),
                   pxExecutorDataContainer -> m_uiDataLength);
            pxCustomerDataContainer -> m_uiDataLength =
                pxExecutorDataContainer -> m_uiDataLength;

            std::cout << "CDeviceControl::Fsm CONFIGURATION_REQUEST_EXECUTOR_DONE_CHECK_WAITING pxCustomerDataContainer -> m_uiDataLength "  << (int)pxCustomerDataContainer -> m_uiDataLength << std::endl;
            std::cout << "CDeviceControl::Fsm CONFIGURATION_REQUEST_EXECUTOR_DONE_CHECK_WAITING pxExecutorDataContainer -> m_uiDataLength "  << (int)pxExecutorDataContainer -> m_uiDataLength << std::endl;

            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
            SetFsmState(DONE_OK);
        }
        break;

    case CONFIGURATION_REQUEST_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING:
        std::cout << "CDeviceControl::Fsm CONFIGURATION_REQUEST_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING"  << std::endl;
        {
            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
            SetFsmState(DONE_ERROR);
        }
        break;

//-------------------------------------------------------------------------------
    case MODBUS_FUNCTION_5_HANDLER_INPUT_SET_TOP_OR_BOTTOM_OF_SCALE_COMAND_START:
        std::cout << "CDeviceControl::Fsm MODBUS_FUNCTION_5_HANDLER_INPUT_SET_TOP_OR_BOTTOM_OF_SCALE_COMAND_START"  << std::endl;
        {
            m_uiInternalModuleMuvrId =
                GetResources() ->
                GetTaskIdByNameFromMap(m_sInternalModuleMuvrName);

            CDataContainerDataBase* pxDataContainer =
                (CDataContainerDataBase*)GetExecutorDataContainerPointer();
            pxDataContainer -> m_uiTaskId = m_uiInternalModuleMuvrId;
            pxDataContainer -> m_uiFsmCommandState =
                CInternalModuleMuvr::MUVR_INPUT_SET_TOP_OR_BOTTOM_OF_SCALE;
            pxDataContainer -> m_puiDataPointer = m_puiIntermediateBuff;

            SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_START);
            SetFsmNextStateDoneOk(MODBUS_FUNCTION_5_HANDLER_INPUT_SET_TOP_OR_BOTTOM_OF_SCALE_COMAND_EXECUTOR_DONE_OK_ANSWER_PROCESSING);
            SetFsmNextStateReadyWaitingError(MODBUS_FUNCTION_5_HANDLER_INPUT_SET_TOP_OR_BOTTOM_OF_SCALE_COMAND_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
            SetFsmNextStateDoneWaitingError(MODBUS_FUNCTION_5_HANDLER_INPUT_SET_TOP_OR_BOTTOM_OF_SCALE_COMAND_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
            SetFsmNextStateDoneWaitingDoneError(MODBUS_FUNCTION_5_HANDLER_INPUT_SET_TOP_OR_BOTTOM_OF_SCALE_COMAND_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
        }
        break;

    case MODBUS_FUNCTION_5_HANDLER_INPUT_SET_TOP_OR_BOTTOM_OF_SCALE_COMAND_EXECUTOR_DONE_OK_ANSWER_PROCESSING:
        std::cout << "CDeviceControl::Fsm MODBUS_FUNCTION_5_HANDLER_INPUT_SET_TOP_OR_BOTTOM_OF_SCALE_COMAND_EXECUTOR_DONE_OK_ANSWER_PROCESSING"  << std::endl;
        {
            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
            SetFsmState(DONE_OK);
        }
        break;

    case MODBUS_FUNCTION_5_HANDLER_INPUT_SET_TOP_OR_BOTTOM_OF_SCALE_COMAND_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING:
        std::cout << "CDeviceControl::Fsm MODBUS_FUNCTION_5_HANDLER_INPUT_SET_TOP_OR_BOTTOM_OF_SCALE_COMAND_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING"  << std::endl;
        {
            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
            SetFsmState(DONE_ERROR);
        }
        break;

//-------------------------------------------------------------------------------
    case MODBUS_FUNCTION_5_HANDLER_INPUT_CALIBRATION_COMAND_START:
        std::cout << "CDeviceControl::Fsm MODBUS_FUNCTION_5_HANDLER_INPUT_CALIBRATION_COMAND_START"  << std::endl;
        {
            m_uiInternalModuleMuvrId =
                GetResources() ->
                GetTaskIdByNameFromMap(m_sInternalModuleMuvrName);

            CDataContainerDataBase* pxDataContainer =
                (CDataContainerDataBase*)GetExecutorDataContainerPointer();
            pxDataContainer -> m_uiTaskId = m_uiInternalModuleMuvrId;
            pxDataContainer -> m_uiFsmCommandState =
                CInternalModuleMuvr::MUVR_CALIBRATION_ON_OFF;
            pxDataContainer -> m_puiDataPointer = m_puiIntermediateBuff;

            SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_START);
            SetFsmNextStateDoneOk(MODBUS_FUNCTION_5_HANDLER_INPUT_CALIBRATION_COMAND_EXECUTOR_DONE_OK_ANSWER_PROCESSING);
            SetFsmNextStateReadyWaitingError(MODBUS_FUNCTION_5_HANDLER_INPUT_CALIBRATION_COMAND_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
            SetFsmNextStateDoneWaitingError(MODBUS_FUNCTION_5_HANDLER_INPUT_CALIBRATION_COMAND_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
            SetFsmNextStateDoneWaitingDoneError(MODBUS_FUNCTION_5_HANDLER_INPUT_CALIBRATION_COMAND_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
        }
        break;

    case MODBUS_FUNCTION_5_HANDLER_INPUT_CALIBRATION_COMAND_EXECUTOR_DONE_OK_ANSWER_PROCESSING:
        std::cout << "CDeviceControl::Fsm MODBUS_FUNCTION_5_HANDLER_INPUT_CALIBRATION_COMAND_EXECUTOR_DONE_OK_ANSWER_PROCESSING"  << std::endl;
        {
            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
            SetFsmState(DONE_OK);
        }
        break;

    case MODBUS_FUNCTION_5_HANDLER_INPUT_CALIBRATION_COMAND_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING:
        std::cout << "CDeviceControl::Fsm MODBUS_FUNCTION_5_HANDLER_INPUT_CALIBRATION_COMAND_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING"  << std::endl;
        {
            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
            SetFsmState(DONE_ERROR);
        }
        break;

//-------------------------------------------------------------------------------
    case MODBUS_FUNCTION_5_HANDLER_START:
        std::cout << "CDeviceControl::Fsm MODBUS_FUNCTION_5_HANDLER_START"  << std::endl;
        {
            if (ModbusFunction5Handler())
            {
//                SetFsmState(MODBUS_FUNCTION_5_HANDLER_EXECUTOR_DONE_OK_ANSWER_PROCESSING);
            }
            else
            {
                SetFsmState(MODBUS_FUNCTION_5_HANDLER_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
            }
        }
        break;

    case MODBUS_FUNCTION_5_HANDLER_EXECUTOR_DONE_OK_ANSWER_PROCESSING:
//        std::cout << "CDeviceControl::Fsm MODBUS_FUNCTION_5_HANDLER_EXECUTOR_DONE_OK_ANSWER_PROCESSING"  << std::endl;
    {
//            CDataContainerDataBase* pxExecutorDataContainer =
//                (CDataContainerDataBase*)GetExecutorDataContainerPointer();
//            CDataContainerDataBase* pxCustomerDataContainer =
//                (CDataContainerDataBase*)GetCustomerDataContainerPointer();
//
//            memcpy(pxCustomerDataContainer -> m_puiDataPointer,
//                   (pxExecutorDataContainer -> m_puiDataPointer),
//                   pxExecutorDataContainer -> m_uiDataLength);
//            pxCustomerDataContainer -> m_uiDataLength =
//                pxExecutorDataContainer -> m_uiDataLength;

//            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
//            SetFsmState(DONE_OK);
    }
    break;

    case MODBUS_FUNCTION_5_HANDLER_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING:
        std::cout << "CDeviceControl::Fsm MODBUS_FUNCTION_5_HANDLER_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING"  << std::endl;
        {
            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
            SetFsmState(DONE_ERROR);
        }
        break;

//-------------------------------------------------------------------------------
    case ONLINE_DATA_READ_START:
        std::cout << "CDeviceControl::Fsm ONLINE_DATA_READ_START"  << std::endl;
        {
            OnlineDataRead();
            SetFsmState(ONLINE_DATA_READ_EXECUTOR_DONE_OK_ANSWER_PROCESSING);
        }
        break;

    case ONLINE_DATA_READ_EXECUTOR_DONE_OK_ANSWER_PROCESSING:
        std::cout << "CDeviceControl::Fsm ONLINE_DATA_READ_EXECUTOR_DONE_OK_ANSWER_PROCESSING"  << std::endl;
        {
//            CDataContainerDataBase* pxExecutorDataContainer =
//                (CDataContainerDataBase*)GetExecutorDataContainerPointer();
//            CDataContainerDataBase* pxCustomerDataContainer =
//                (CDataContainerDataBase*)GetCustomerDataContainerPointer();
//
//            memcpy(pxCustomerDataContainer -> m_puiDataPointer,
//                   (pxExecutorDataContainer -> m_puiDataPointer),
//                   pxExecutorDataContainer -> m_uiDataLength);
//            pxCustomerDataContainer -> m_uiDataLength =
//                pxExecutorDataContainer -> m_uiDataLength;

            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
            SetFsmState(DONE_OK);
        }
        break;

    case ONLINE_DATA_READ_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING:
        std::cout << "CDeviceControl::Fsm ONLINE_DATA_READ_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING"  << std::endl;
        {
            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
            SetFsmState(DONE_ERROR);
        }
        break;

//-------------------------------------------------------------------------------
    case DATA_BASE_BLOCK_READ:
        std::cout << "CDeviceControl::Fsm DATA_BASE_BLOCK_READ 1"  << std::endl;
        {
            m_uiDataStoreId =
                GetResources() ->
                GetTaskIdByNameFromMap(m_sDataStoreName);

            uint8_t* puiDataPointer =
                (((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_puiDataPointer);
            uint8_t uiBlockIndex =
                (((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiDataIndex);

            CDataContainerDataBase* pxDataContainer =
                (CDataContainerDataBase*)GetExecutorDataContainerPointer();
            pxDataContainer -> m_uiTaskId = m_uiDataStoreId;
            pxDataContainer -> m_uiFsmCommandState =
                CDataStore::READ_BLOCK_DATA_START;
            pxDataContainer -> m_uiDataIndex = uiBlockIndex;
            pxDataContainer -> m_puiDataPointer = m_puiIntermediateBuff;

            SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_START);
            SetFsmNextStateDoneOk(DATA_BASE_BLOCK_READ_EXECUTOR_DONE_OK_ANSWER_PROCESSING);
            SetFsmNextStateReadyWaitingError(DATA_BASE_BLOCK_READ_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
            SetFsmNextStateDoneWaitingError(DATA_BASE_BLOCK_READ_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
            SetFsmNextStateDoneWaitingDoneError(DATA_BASE_BLOCK_READ_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
        }
        break;

    case DATA_BASE_BLOCK_READ_EXECUTOR_DONE_OK_ANSWER_PROCESSING:
        std::cout << "CDeviceControl::Fsm DATA_BASE_BLOCK_READ_EXECUTOR_DONE_OK_ANSWER_PROCESSING"  << std::endl;
        {
            CDataContainerDataBase* pxExecutorDataContainer =
                (CDataContainerDataBase*)GetExecutorDataContainerPointer();
            CDataContainerDataBase* pxCustomerDataContainer =
                (CDataContainerDataBase*)GetCustomerDataContainerPointer();

            memcpy(pxCustomerDataContainer -> m_puiDataPointer,
                   (pxExecutorDataContainer -> m_puiDataPointer),
                   pxExecutorDataContainer -> m_uiDataLength);
            pxCustomerDataContainer -> m_uiDataLength =
                pxExecutorDataContainer -> m_uiDataLength;

            // выполним дополнительное действие связанное с чтением определённого блока базы данных.
            // например при чтении блока модуля аналоговых сигналов прочитаем блок из модуля.
            DataBaseBlockReadBlockRelatedAction();
        }
        break;

    case DATA_BASE_BLOCK_READ_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING:
        std::cout << "CDeviceControl::Fsm DATA_BASE_BLOCK_READ_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING"  << std::endl;
        {
            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
            SetFsmState(DONE_ERROR);
        }
        break;

//-------------------------------------------------------------------------------
    case DATA_BASE_BLOCK_READ_MODULE_MUVR_DATA_BASE_READ_START:
        std::cout << "CDeviceControl::Fsm DATA_BASE_BLOCK_READ_MODULE_MUVR_DATA_BASE_READ_START"  << std::endl;
        {
            m_uiInternalModuleMuvrId =
                GetResources() ->
                GetTaskIdByNameFromMap(m_sInternalModuleMuvrName);

            uint8_t* puiDataPointer =
                (((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_puiDataPointer);
            uint8_t uiBlockIndex =
                (((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiDataIndex);

            CDataContainerDataBase* pxDataContainer =
                (CDataContainerDataBase*)GetExecutorDataContainerPointer();
            pxDataContainer -> m_uiTaskId = m_uiInternalModuleMuvrId;
            pxDataContainer -> m_uiFsmCommandState =
                CInternalModuleMuvr::MUVR_DATA_BASE_READ;
            pxDataContainer -> m_puiDataPointer = m_puiIntermediateBuff;

            SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_START);
            SetFsmNextStateDoneOk(DATA_BASE_BLOCK_READ_MODULE_MUVR_DATA_BASE_READ_EXECUTOR_DONE_OK_ANSWER_PROCESSING);
            SetFsmNextStateReadyWaitingError(DATA_BASE_BLOCK_READ_MODULE_MUVR_DATA_BASE_READ_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
            SetFsmNextStateDoneWaitingError(DATA_BASE_BLOCK_READ_MODULE_MUVR_DATA_BASE_READ_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
            SetFsmNextStateDoneWaitingDoneError(DATA_BASE_BLOCK_READ_MODULE_MUVR_DATA_BASE_READ_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
        }
        break;

    case DATA_BASE_BLOCK_READ_MODULE_MUVR_DATA_BASE_READ_EXECUTOR_DONE_OK_ANSWER_PROCESSING:
        std::cout << "CDeviceControl::Fsm DATA_BASE_BLOCK_READ_MODULE_MUVR_DATA_BASE_READ_EXECUTOR_DONE_OK_ANSWER_PROCESSING"  << std::endl;
        {
            CDataContainerDataBase* pxExecutorDataContainer =
                (CDataContainerDataBase*)GetExecutorDataContainerPointer();
            CDataContainerDataBase* pxCustomerDataContainer =
                (CDataContainerDataBase*)GetCustomerDataContainerPointer();

            memcpy(pxCustomerDataContainer -> m_puiDataPointer,
                   (pxExecutorDataContainer -> m_puiDataPointer),
                   pxExecutorDataContainer -> m_uiDataLength);
            pxCustomerDataContainer -> m_uiDataLength =
                pxExecutorDataContainer -> m_uiDataLength;

            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
            SetFsmState(DONE_OK);
        }
        break;

    case DATA_BASE_BLOCK_READ_MODULE_MUVR_DATA_BASE_READ_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING:
        std::cout << "CDeviceControl::Fsm DATA_BASE_BLOCK_READ_MODULE_MUVR_DATA_BASE_READ_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING"  << std::endl;
        {
            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
            SetFsmState(DONE_ERROR);
        }
        break;

//-------------------------------------------------------------------------------
    case DATA_BASE_BLOCK_READ_REFERENCE_POINTS_ADC_CODES_MODULE_MUVR_DATA_READ_START:
        std::cout << "CDeviceControl::Fsm DATA_BASE_BLOCK_READ_REFERENCE_POINTS_ADC_CODES_MODULE_MUVR_DATA_READ_START"  << std::endl;
        {
            m_uiInternalModuleMuvrId =
                GetResources() ->
                GetTaskIdByNameFromMap(m_sInternalModuleMuvrName);

            uint8_t* puiDataPointer =
                (((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_puiDataPointer);
            uint8_t uiBlockIndex =
                (((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiDataIndex);

            CDataContainerDataBase* pxDataContainer =
                (CDataContainerDataBase*)GetExecutorDataContainerPointer();
            pxDataContainer -> m_uiTaskId = m_uiInternalModuleMuvrId;
            pxDataContainer -> m_uiFsmCommandState =
                CInternalModuleMuvr::MUVR_REPER_POINTS_ADC_READ;
            pxDataContainer -> m_puiDataPointer = m_puiIntermediateBuff;

            SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_START);
            SetFsmNextStateDoneOk(DATA_BASE_BLOCK_READ_REFERENCE_POINTS_ADC_CODES_MODULE_MUVR_DATA_READ_EXECUTOR_DONE_OK_ANSWER_PROCESSING);
            SetFsmNextStateReadyWaitingError(DATA_BASE_BLOCK_READ_REFERENCE_POINTS_ADC_CODES_MODULE_MUVR_DATA_READ_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
            SetFsmNextStateDoneWaitingError(DATA_BASE_BLOCK_READ_REFERENCE_POINTS_ADC_CODES_MODULE_MUVR_DATA_READ_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
            SetFsmNextStateDoneWaitingDoneError(DATA_BASE_BLOCK_READ_REFERENCE_POINTS_ADC_CODES_MODULE_MUVR_DATA_READ_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
        }
        break;

    case DATA_BASE_BLOCK_READ_REFERENCE_POINTS_ADC_CODES_MODULE_MUVR_DATA_READ_EXECUTOR_DONE_OK_ANSWER_PROCESSING:
        std::cout << "CDeviceControl::Fsm DATA_BASE_BLOCK_READ_REFERENCE_POINTS_ADC_CODES_MODULE_MUVR_DATA_READ_EXECUTOR_DONE_OK_ANSWER_PROCESSING"  << std::endl;
        {
            CDataContainerDataBase* pxExecutorDataContainer =
                (CDataContainerDataBase*)GetExecutorDataContainerPointer();
            CDataContainerDataBase* pxCustomerDataContainer =
                (CDataContainerDataBase*)GetCustomerDataContainerPointer();

            memcpy(pxCustomerDataContainer -> m_puiDataPointer,
                   (pxExecutorDataContainer -> m_puiDataPointer),
                   pxExecutorDataContainer -> m_uiDataLength);
            pxCustomerDataContainer -> m_uiDataLength =
                pxExecutorDataContainer -> m_uiDataLength;

            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
            SetFsmState(DONE_OK);
        }
        break;

    case DATA_BASE_BLOCK_READ_REFERENCE_POINTS_ADC_CODES_MODULE_MUVR_DATA_READ_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING:
        std::cout << "CDeviceControl::Fsm DATA_BASE_BLOCK_READ_REFERENCE_POINTS_ADC_CODES_MODULE_MUVR_DATA_READ_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING"  << std::endl;
        {
            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
            SetFsmState(DONE_ERROR);
        }
        break;
//-------------------------------------------------------------------------------



//-------------------------------------------------------------------------------
    case DATA_BASE_BLOCK_WRITE_START:
        std::cout << "CDeviceControl::Fsm DATA_BASE_BLOCK_WRITE_START"  << std::endl;
        {
            m_uiDataStoreId =
                GetResources() ->
                GetTaskIdByNameFromMap(m_sDataStoreName);

            uint8_t* puiDataPointer =
                (((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_puiDataPointer);
            uint8_t uiBlockIndex =
                (((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiDataIndex);
            uint8_t uiDataLength =
                (m_pxDataStore ->
                 GetBlockLength((((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiDataIndex)));

            CDataContainerDataBase* pxDataContainer =
                (CDataContainerDataBase*)GetExecutorDataContainerPointer();
            pxDataContainer -> m_uiTaskId = m_uiDataStoreId;
            pxDataContainer -> m_uiFsmCommandState =
                CDataStore::START_WRITE_TEMPORARY_BLOCK_DATA;
            pxDataContainer -> m_uiDataIndex = uiBlockIndex;
            pxDataContainer -> m_puiDataPointer = puiDataPointer;
            pxDataContainer -> m_uiDataLength = uiDataLength;

            SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_START);
            SetFsmNextStateDoneOk(DATA_BASE_BLOCK_WRITE_EXECUTOR_DONE_OK_ANSWER_PROCESSING);
            SetFsmNextStateReadyWaitingError(DATA_BASE_BLOCK_WRITE_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
            SetFsmNextStateDoneWaitingError(DATA_BASE_BLOCK_WRITE_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
            SetFsmNextStateDoneWaitingDoneError(DATA_BASE_BLOCK_WRITE_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
            // сообщим о том, что процесс записи блока начался нормально.
            // при записи блока в базу данных не ожидается результат окончания выполнения задачи.
            // проверка результата выполнения производится в _FC_PROGRAMMING_COMPLETION_REQUEST 14 функция.
            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
        }
        break;

    case DATA_BASE_BLOCK_WRITE_EXECUTOR_DONE_OK_ANSWER_PROCESSING:
        std::cout << "CDeviceControl::Fsm DATA_BASE_BLOCK_WRITE_EXECUTOR_DONE_OK_ANSWER_PROCESSING"  << std::endl;
        {
            // выполним дополнительное действие связанное с записью определённого блока базы данных.
            // например при записи блока модуля аналоговых сигналов запишим блок и в модуль.
            DataBaseBlockWriteBlockRelatedAction();
//            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
//            SetFsmState(DONE_OK);
        }
        break;

    case DATA_BASE_BLOCK_WRITE_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING:
        std::cout << "CDeviceControl::Fsm DATA_BASE_BLOCK_WRITE_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING"  << std::endl;
        {
            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
            SetFsmState(DONE_ERROR);
        }
        break;

//-------------------------------------------------------------------------------
    case DATA_BASE_BLOCK_WRITE_COMPLETE_CHECK_START:
        std::cout << "CDeviceControl::Fsm DATA_BASE_BLOCK_WRITE_COMPLETE_CHECK_START 1"  << std::endl;
        {
            if ((GetPreviousFsmState() == DONE_OK) ||
                    (GetPreviousFsmState() == READY))
            {
                std::cout << "CDeviceControl::Fsm DATA_BASE_BLOCK_WRITE_COMPLETE_CHECK_START 2"  << std::endl;
                SetFsmState(DATA_BASE_BLOCK_WRITE_COMPLETE_CHECK_EXECUTOR_DONE_OK_ANSWER_PROCESSING);
            }
            else if (GetPreviousFsmState() == DONE_ERROR)
            {
                std::cout << "CDeviceControl::Fsm DATA_BASE_BLOCK_WRITE_COMPLETE_CHECK_START 3"  << std::endl;
                SetFsmState(DATA_BASE_BLOCK_WRITE_COMPLETE_CHECK_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
            }
        }
        break;

    case DATA_BASE_BLOCK_WRITE_COMPLETE_CHECK_EXECUTOR_DONE_OK_ANSWER_PROCESSING:
        std::cout << "CDeviceControl::Fsm DATA_BASE_BLOCK_WRITE_COMPLETE_CHECK_EXECUTOR_DONE_OK_ANSWER_PROCESSING"  << std::endl;
        {
            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
            SetFsmState(DONE_OK);
        }
        break;

    case DATA_BASE_BLOCK_WRITE_COMPLETE_CHECK_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING:
        std::cout << "CDeviceControl::Fsm DATA_BASE_BLOCK_WRITE_COMPLETE_CHECK_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING"  << std::endl;
        {
            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
            SetFsmState(DONE_ERROR);
        }
        break;

//-------------------------------------------------------------------------------
    case DATA_BASE_BLOCK_MODULE_MUVR_WRITE_START:
        std::cout << "CDeviceControl::Fsm DATA_BASE_BLOCK_MODULE_MUVR_WRITE_START"  << std::endl;
        {
            m_uiInternalModuleMuvrId =
                GetResources() ->
                GetTaskIdByNameFromMap(m_sInternalModuleMuvrName);

            CDataContainerDataBase* pxDataContainer =
                (CDataContainerDataBase*)GetExecutorDataContainerPointer();
            pxDataContainer -> m_uiTaskId = m_uiInternalModuleMuvrId;
            pxDataContainer -> m_uiFsmCommandState =
                CInternalModuleMuvr::MUVR_WRITE_DATA_BASE;
            pxDataContainer -> m_puiDataPointer =
                (((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_puiDataPointer);

            SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_START);
            SetFsmNextStateDoneOk(DATA_BASE_BLOCK_MODULE_MUVR_WRITE_EXECUTOR_DONE_OK_ANSWER_PROCESSING);
            SetFsmNextStateReadyWaitingError(DATA_BASE_BLOCK_MODULE_MUVR_WRITE_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
            SetFsmNextStateDoneWaitingError(DATA_BASE_BLOCK_MODULE_MUVR_WRITE_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
            SetFsmNextStateDoneWaitingDoneError(DATA_BASE_BLOCK_MODULE_MUVR_WRITE_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
        }
        break;

    case DATA_BASE_BLOCK_MODULE_MUVR_WRITE_EXECUTOR_DONE_OK_ANSWER_PROCESSING:
        std::cout << "CDeviceControl::Fsm DATA_BASE_BLOCK_MODULE_MUVR_WRITE_EXECUTOR_DONE_OK_ANSWER_PROCESSING"  << std::endl;
        {
//            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
            SetFsmState(DATA_BASE_BLOCK_MODULE_MUVR_WRITE_COMPLETE_CHECK_START);
        }
        break;

    case DATA_BASE_BLOCK_MODULE_MUVR_WRITE_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING:
        std::cout << "CDeviceControl::Fsm DATA_BASE_BLOCK_MODULE_MUVR_WRITE_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING"  << std::endl;
        {
            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
            SetFsmState(DONE_ERROR);
        }
        break;

//-------------------------------------------------------------------------------
    case DATA_BASE_BLOCK_MODULE_MUVR_WRITE_COMPLETE_CHECK_START:
        std::cout << "CDeviceControl::Fsm DATA_BASE_BLOCK_MODULE_MUVR_WRITE_COMPLETE_CHECK_START"  << std::endl;
        {
            m_uiInternalModuleMuvrId =
                GetResources() ->
                GetTaskIdByNameFromMap(m_sInternalModuleMuvrName);

            CDataContainerDataBase* pxDataContainer =
                (CDataContainerDataBase*)GetExecutorDataContainerPointer();
            pxDataContainer -> m_uiTaskId = m_uiInternalModuleMuvrId;
            pxDataContainer -> m_uiFsmCommandState =
                CInternalModuleMuvr::MUVR_WRITE_DATA_BASE_CHECK;

            SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_START);
            SetFsmNextStateDoneOk(DATA_BASE_BLOCK_MODULE_MUVR_WRITE_COMPLETE_CHECK_EXECUTOR_DONE_OK_ANSWER_PROCESSING);
            SetFsmNextStateReadyWaitingError(DATA_BASE_BLOCK_MODULE_MUVR_WRITE_COMPLETE_CHECK_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
            SetFsmNextStateDoneWaitingError(DATA_BASE_BLOCK_MODULE_MUVR_WRITE_COMPLETE_CHECK_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
            SetFsmNextStateDoneWaitingDoneError(DATA_BASE_BLOCK_MODULE_MUVR_WRITE_COMPLETE_CHECK_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
        }
        break;

    case DATA_BASE_BLOCK_MODULE_MUVR_WRITE_COMPLETE_CHECK_EXECUTOR_DONE_OK_ANSWER_PROCESSING:
        std::cout << "CDeviceControl::Fsm DATA_BASE_BLOCK_MODULE_MUVR_WRITE_COMPLETE_CHECK_EXECUTOR_DONE_OK_ANSWER_PROCESSING"  << std::endl;
        {
            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
            SetFsmState(DONE_OK);
        }
        break;

    case DATA_BASE_BLOCK_MODULE_MUVR_WRITE_COMPLETE_CHECK_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING:
        std::cout << "CDeviceControl::Fsm DATA_BASE_BLOCK_MODULE_MUVR_WRITE_COMPLETE_CHECK_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING"  << std::endl;
        {
            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
            SetFsmState(DONE_ERROR);
        }
        break;

//-------------------------------------------------------------------------------
    case DATA_BASE_BLOCK_NETWORK_ADDRESS_WRITE_START:
        std::cout << "CDeviceControl::Fsm DATA_BASE_BLOCK_NETWORK_ADDRESS_WRITE_START"  << std::endl;
        {
            uint8_t uiAddress =
                ((((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_puiDataPointer)[0] +
                 CONVERT_INTEGER_TO_NATURAL_NUMBER);
            std::cout << "CDeviceControl::Fsm uiAddress "  << (int)uiAddress << std::endl;

            CModbusSlave* pxModbusRtuSlaveUpperLevel =
                (CModbusSlave*)(GetResources() ->
                                GetTaskPointerByNameFromMap("ModbusRtuSlaveUpperLevel"));
            pxModbusRtuSlaveUpperLevel ->
            SetOwnAddress(uiAddress);

            CModbusSlave* pxModbusTcpSlaveUpperLevel =
                (CModbusSlave*)(GetResources() ->
                                GetTaskPointerByNameFromMap("ModbusTcpSlaveUpperLevel"));
            pxModbusTcpSlaveUpperLevel ->
            SetOwnAddress(uiAddress);

            SetFsmState(DATA_BASE_BLOCK_NETWORK_ADDRESS_WRITE_EXECUTOR_DONE_OK_ANSWER_PROCESSING);
        }
        break;

    case DATA_BASE_BLOCK_NETWORK_ADDRESS_WRITE_EXECUTOR_DONE_OK_ANSWER_PROCESSING:
        std::cout << "CDeviceControl::Fsm DATA_BASE_BLOCK_NETWORK_ADDRESS_WRITE_EXECUTOR_DONE_OK_ANSWER_PROCESSING"  << std::endl;
        {
//            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
            SetFsmState(DONE_OK);
        }
        break;

    case DATA_BASE_BLOCK_NETWORK_ADDRESS_WRITE_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING:
        std::cout << "CDeviceControl::Fsm DATA_BASE_BLOCK_NETWORK_ADDRESS_WRITE_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING"  << std::endl;
        {
            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
            SetFsmState(DONE_ERROR);
        }
        break;

//-------------------------------------------------------------------------------
    default:
        break;
    }
}

//-------------------------------------------------------------------------------

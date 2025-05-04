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
#include "DeviceControl.h"

using namespace std;

//-------------------------------------------------------------------------------
CDeviceControl::CDeviceControl()
{
    std::cout << "CDeviceControl constructor"  << std::endl;
    m_puiIntermediateBuff = new uint8_t[256];
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
}

//-------------------------------------------------------------------------------
void CDeviceControl::SetInternalModuleMuvrName(std::string sName)
{
    m_sInternalModuleMuvrName = sName;
}

//-----------------------------------------------------------------------------------------------------
// ������������ �������� ��������� �� Modbus ����������� �� 71 ������� - ������ ������ ������, ������� ����������� �����.
// ������� ���������� �������� ���������� ������, �������� ����� ���, �������� ���.
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
// ���� � ������� �������� ������ Modbus ���7 = 0, �� ������������� �������� ����� - (���0 - ���6) - ����� ����������� �����.
// ���� � ������� �������� ������ Modbus ���7 = 1, �� ������������� ��� � (���0 - ���2) - ������������� ����� ������ ����3.
// ucAddressLow - ��������� ���������� ���������� ������.

    uint16_t uiAddress =
        (((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiDataOffset);
    ucAddressLow = (unsigned char)(uiAddress);
    ucAddressHigh = (unsigned char)((uiAddress) >> 8);
    pucDestination =
        (((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_puiDataPointer);

    CConfigurationCreate::TConfigDataPackOne* pxDeviceConfigSearch =
        (GetResources() -> GetDeviceConfigSearchPointer());

    // ������ ��� ��� �������� �����?
    if (ucAddressHigh & MUVR_TXS_REQUEST_MASK)
    {
        std::cout << "CDeviceControl OnlineDataRead 2"  << std::endl;
// ������ ���.
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
// ������ �������� �����.

        // ����� ����������� ����� ����������� ����������� ��� �������� ������?
        if ((pxDeviceConfigSearch -> uiServiceAnalogueInputModuleQuantity) >=
                (((ucAddressHigh & ANALOGUE_INPUT_ADDRESS_MASK) /
                  MUVR_ANALOG_INPUT_QUANTITY) + 1))
        {
            std::cout << "CDeviceControl OnlineDataRead 4"  << std::endl;
            // ����� ����������� ����� ����������� ����������� ������.

            // ������� ����� �������� ����������� �����.
            pfSource = &(GetResources() ->
                         m_pfAnalogueInputsValue[(ucAddressHigh & ANALOGUE_INPUT_ADDRESS_MASK)]);
            // ������� ����� �������� ��������� ������ ����������� �����.
            pucSource2 = &(GetResources() ->
                           m_puiAnalogueInputsState[ucAddressHigh]);
            for (i = 0; i < (ucAddressLow); i++)
            {
                // ����������� ������ 7 ���� float ��������, � ASCII �������, ��� ����������� � ��������� ���� � �������������.
                sprintf((char*)pucDestination,
                        "%7.4f",
                        pfSource[i]);
                pucDestination += ONLINE_DATA_READ_ONE_INPUT_ASCII_BYTE_QUANTITY;
                // ������� �������� ��������� ������ ����������� �����.
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
            // ����� ����������� ����� ����������� �������� ������.
//            cout << "LSNU" << endl;

            // ������� ����� �������� ����������� �����.
            pfSource = &(GetResources() ->
                         m_pfAnalogueInputsValue[(ucAddressHigh & ANALOGUE_INPUT_ADDRESS_MASK)]);
            // ������� ����� �������� ��������� ������ ����������� �����.
            pucSource2 = &(GetResources() ->
                           m_puiAnalogueInputsState[ucAddressHigh]);
            for (i = 0; i < (ucAddressLow); i++)
            {
                // ����������� ������ 7 ���� float ��������, � ASCII �������, ��� ����������� � ��������� ���� � �������������.
                sprintf((char*)pucDestination,
                        "%7.4f",
                        pfSource[i]);
                pucDestination += ONLINE_DATA_READ_ONE_INPUT_ASCII_BYTE_QUANTITY;
                // ������� �������� ��������� ������ ����������� �����.
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
// ������������ �������� ��������� �� Modbus �����������, �� 5 �������.
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
    // �������, �� ������ ������ ������������ ���.
    switch(uiAddress -
            COILS_ARRAY_MODBUS_BEGIN_ADDRESS)
    {
    // ������������ � �������� ������.
    case DEVICE_CONTROL_PC_KVIT:
    {
        CDataContainerDataBase* pxDataContainer =
            (CDataContainerDataBase*)GetCustomerDataContainerPointer();
        // ��� ����������?
        if ((pxDataContainer -> m_puiDataPointer[0]))
        {
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

    // ����� � �������� ������.
    case DEVICE_CONTROL_PC_RESET:
    {
        CDataContainerDataBase* pxDataContainer =
            (CDataContainerDataBase*)GetCustomerDataContainerPointer();
        // ��� ����������?
        if ((pxDataContainer -> m_puiDataPointer[0]))
        {
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

    // ���������� � �������� ������.
    case DEVICE_CONTROL_BLOCK:
    {
        CDataContainerDataBase* pxDataContainer =
            (CDataContainerDataBase*)GetCustomerDataContainerPointer();
        // ��� ����������?
        if ((pxDataContainer -> m_puiDataPointer[0]))
        {
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
    // �������, �� ������ ������ ������������ ���.
    switch(((uiAddress -
             COILS_ARRAY_MODBUS_BEGIN_ADDRESS) & 0xFF00))
    {
    // ���������-���������� ������ ����������.
    case DEVICE_CONTROL_CALIBRATION_ON_OFF:
        // ����� ���� ��� ���� ������ �� ����� �� ��������� ���������� ����?
        if (((GetResources() -> m_uiAinOffProcessOwnerIndex) == AIN_OFF_PROCESS_OWNER_IS_NONE) ||
                ((GetResources() -> m_uiAinOffProcessOwnerIndex) == AIN_OFF_PROCESS_OWNER_IS_PROGRAMMER))
        {
            // �������� ������ ������ � ������� ���������, � �������� ��������� ������� - ����������.
            nucIndexNumber = (((((GetResources() -> GetDeviceConfigSearchPointer()) -> uiLastAnalogueInputModuleIndex) + 1) -
                               ((GetResources() -> GetDeviceConfigSearchPointer()) -> uiServiceAnalogueInputModuleQuantity))  +
                              (((uiAddress -
                                 COILS_ARRAY_MODBUS_BEGIN_ADDRESS) & CALIBRATION_ANALOG_MODULE_NUMBER_MASK) >> 4));

            cout << "DEVICE_CONTROL_CALIBRATION_ON nucIndexNumber " << (int)nucIndexNumber << endl;
            cout << "DEVICE_CONTROL_CALIBRATION_ON uiLastAnalogueInputModuleIndex " << (int)(((GetResources() -> GetDeviceConfigSearchPointer()) ->
                    uiLastAnalogueInputModuleIndex) + 1) << endl;
            // �� ������� - nucIndexNumber ���� ������?
            if (nucIndexNumber <=
                    (((GetResources() -> GetDeviceConfigSearchPointer()) ->
                      uiLastAnalogueInputModuleIndex) + 1))
            {
                // ������� ��������� �� ��������� � ������� ���������.
                CDataContainerDataBase* pxDataContainer =
                    (CDataContainerDataBase*)GetCustomerDataContainerPointer();
                // ��� ����������?
                if ((pxDataContainer -> m_puiDataPointer[0]))
                {
                    // ��������� ������ ����������.
                    cout << "DEVICE_CONTROL_CALIBRATION_ON" << endl;
                    // ���� ���������� - �������.
                    (pxDataContainer -> m_puiDataPointer[0]) = 0;
                    // ������� ��������� �� ��������� � ������� �����������..
                    pxDataContainer =
                        (CDataContainerDataBase*)GetExecutorDataContainerPointer();
                    // ��������� �������� ������ ����� ������������ �����.
                    // � DO-D2 - � ����� � ������ (0-��� ������ ����������).
                    (pxDataContainer -> m_puiDataPointer[0]) = (((uiAddress -
                            COILS_ARRAY_MODBUS_BEGIN_ADDRESS) & CALIBRATION_INPUT_NUMBER_MASK));

                    cout << "DEVICE_CONTROL_CALIBRATION_ON ucCommonIndex " << (int)(pxDataContainer -> m_puiDataPointer[0]) << endl;
                    // ����� � ������� ��������� ��� ���������� �� ��������� ���������� �����.
                    for (i = 0; i < MAX_HANDLED_ANALOGUE_INPUT; i++)
                    {
                        (GetResources() -> m_puiAnalogueInputsOff[i]) = 0;
                    }

                    (GetResources() -> m_puiAnalogueInputsOff[(((((uiAddress - // ��������� ����� ������
                            COILS_ARRAY_MODBUS_BEGIN_ADDRESS) &
                            CALIBRATION_ANALOG_MODULE_NUMBER_MASK) >> 4) *
                            ANALOG_MODULE_INPUT_QUANTITY) +

                            (((uiAddress - // ��������� ����� �����
                               COILS_ARRAY_MODBUS_BEGIN_ADDRESS) &
                              CALIBRATION_INPUT_NUMBER_MASK) - 1))]) = 1;

                    (GetResources() -> m_uiAinOffProcessOwnerIndex) = AIN_OFF_PROCESS_OWNER_IS_PROGRAMMER;
                }
                else
                {
                    // ���������� ������ ����������.
                    cout << "DEVICE_CONTROL_CALIBRATION_OFF" << endl;
                    // ������� ��������� �� ��������� � ������� �����������..
                    pxDataContainer =
                        (CDataContainerDataBase*)GetExecutorDataContainerPointer();
                    // ��������� �������� ������ ����� ������������ �����. ���� 0, ���������� ���������.
                    // � DO-D2 - � ����� � ������ (0-��� ������ ����������).
                    (pxDataContainer -> m_puiDataPointer[0]) = 0;
                    // ����� � ������� ��������� ��� ���������� �� ��������� ���������� �����.
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

    // ���������� ������ ����� - ���.
    case DEVICE_CONTROL_SET_BOTTOM_OF_SCALE:
        // �������� ������ ������ � ������� ���������, � �������� ��������� �������.
        nucIndexNumber = (((((GetResources() -> GetDeviceConfigSearchPointer()) -> uiLastAnalogueInputModuleIndex) + 1) -
                           ((GetResources() -> GetDeviceConfigSearchPointer()) -> uiServiceAnalogueInputModuleQuantity))  +
                          (((uiAddress -
                             COILS_ARRAY_MODBUS_BEGIN_ADDRESS) & CALIBRATION_ANALOG_MODULE_NUMBER_MASK) >> 4));
        // �� ������� - nucIndexNumber ���� ������?
        if (nucIndexNumber <=
                (((GetResources() -> GetDeviceConfigSearchPointer()) ->
                  uiLastAnalogueInputModuleIndex) + 1))
        {
            // ������� ��������� �� ��������� � ������� ���������.
            CDataContainerDataBase* pxDataContainer =
                (CDataContainerDataBase*)GetCustomerDataContainerPointer();
            // ��� ����������?
            if ((pxDataContainer -> m_puiDataPointer[0]))
            {
                // ���� ���������� - �������.
                (pxDataContainer -> m_puiDataPointer[0]) = 0;
                // ������� ��������� �� ��������� � ������� �����������..
                pxDataContainer =
                    (CDataContainerDataBase*)GetExecutorDataContainerPointer();
                // ��������� �������� ������ ������� - ���������� ������ ����� - ���.
                (pxDataContainer -> m_puiDataPointer[1]) = MUVR_COMMAND_CONTROL_SET_BOTTOM_OF_SCALE;
            }
        }

        SetFsmState(MODBUS_FUNCTION_5_HANDLER_INPUT_SET_TOP_OR_BOTTOM_OF_SCALE_COMAND_START);
        return 1;
        break;

    // ���������� ����� ����� - ���.
    case DEVICE_CONTROL_SET_TOP_OF_SCALE:
        // �������� ������ ������ � ������� ���������, � �������� ��������� �������.
        nucIndexNumber = (((((GetResources() -> GetDeviceConfigSearchPointer()) -> uiLastAnalogueInputModuleIndex) + 1) -
                           ((GetResources() -> GetDeviceConfigSearchPointer()) -> uiServiceAnalogueInputModuleQuantity))  +
                          (((uiAddress -
                             COILS_ARRAY_MODBUS_BEGIN_ADDRESS) & CALIBRATION_ANALOG_MODULE_NUMBER_MASK) >> 4));
        // �� ������� - nucIndexNumber ���� ������?
        if (nucIndexNumber <=
                (((GetResources() -> GetDeviceConfigSearchPointer()) ->
                  uiLastAnalogueInputModuleIndex) + 1))
        {
            // ������� ��������� �� ��������� � ������� ���������.
            CDataContainerDataBase* pxDataContainer =
                (CDataContainerDataBase*)GetCustomerDataContainerPointer();
            // ��� ����������?
            if ((pxDataContainer -> m_puiDataPointer[0]))
            {
                // ���� ���������� - �������.
                (pxDataContainer -> m_puiDataPointer[0]) = 0;
                // ������� ��������� �� ��������� � ������� �����������..
                pxDataContainer =
                    (CDataContainerDataBase*)GetExecutorDataContainerPointer();
                // ��������� �������� ������ ������� - ���������� ������ ����� - ���.
                (pxDataContainer -> m_puiDataPointer[1]) = MUVR_COMMAND_CONTROL_SET_TOP_OF_SCALE;
            }
        }

        SetFsmState(MODBUS_FUNCTION_5_HANDLER_INPUT_SET_TOP_OR_BOTTOM_OF_SCALE_COMAND_START);
        return 1;

////        // �������� ������ ������ � ������� ���������, � �������� ��������� �������.
////        nucIndexNumber = (((xPlcConfigService.xPlcConfigServiceData.ucLastAnalogueInputModuleIndex + 1) -
////                           xPlcConfigService.xPlcConfigServiceData.ucServiceAnalogueInputModuleQuantity)  +
////                          ((((uiAddress -
////                              COILS_ARRAY_MODBUS_BEGIN_ADDRESS) & CALIBRATION_ANALOG_MODULE_NUMBER_MASK) >> 4) - 1));
//		// �������� ������ ������ � ������� ���������, � �������� ��������� �������.
//		nucIndexNumber = (((xPlcConfigService.xPlcConfigServiceData.ucLastAnalogueInputModuleIndex + 1) -
//						   xPlcConfigService.xPlcConfigServiceData.ucServiceAnalogueInputModuleQuantity)  +
//						  (((uiAddress -
//							 COILS_ARRAY_MODBUS_BEGIN_ADDRESS) & CALIBRATION_ANALOG_MODULE_NUMBER_MASK) >> 4));
//		// �� ������� - nucIndexNumber ���� ������?
//		if (nucIndexNumber <= (xPlcConfigService.xPlcConfigServiceData.ucLastAnalogueInputModuleIndex))
//		{
//			// ��� ���������� ��� �������?
//			if (pxModbusMapping->tab_bits[uiAddress -
//										  COILS_ARRAY_MODBUS_BEGIN_ADDRESS])
//			{
//				// ���� ���������� - �������.
//				pxModbusMapping->tab_bits[uiAddress -
//										  COILS_ARRAY_MODBUS_BEGIN_ADDRESS] = 0;
//				// ��������� �������� ������ ������� - ���������� ����� ����� - ���.
//				xAllModulesContext.axAllModulesContext[nucIndexNumber].
//				xModuleContextDinamic.
//				ucCommandControl = MVAI5_COMMAND_CONTROL_SET_TOP_OF_SCALE;
//			}
//			else
//			{
//
//			}
//		}

        break;

    default:
        break;
    };

////-----------------------------------------------------------------------------------------------------
//// ��������� ������� - SP, OUT, ����������� ������� �������� ������ - MTVI5.
//	// ����� ������������� ���� ��������� � ��������� ���������� ������� - SP, OUT, ����������� ������� �������� ������?
//	if (((pxModbusMapping -> current_message_address_bits -
//			COILS_ARRAY_MODBUS_BEGIN_ADDRESS) >= INCREMENT_SP_OUT_BIT_ARRAY_OFFSET) &&
//			((pxModbusMapping -> current_message_address_bits -
//			  COILS_ARRAY_MODBUS_BEGIN_ADDRESS) < (INCREMENT_SP_OUT_BIT_ARRAY_OFFSET +
//					  INCREMENT_SP_OUT_BIT_ARRAY_LENGTH)))
//	{
//		// ��� ���������� ��� �������?
//		if (pxModbusMapping->tab_bits[uiAddress -
//									  COILS_ARRAY_MODBUS_BEGIN_ADDRESS])
//		{
//			// ���� ���������� - �������.
//			pxModbusMapping->tab_bits[uiAddress -
//									  COILS_ARRAY_MODBUS_BEGIN_ADDRESS] = 0;
//			cout << "INCREMENT_SP_OUT_1__BIT_ARRAY_OFFSET 1" << endl;
//			// ��������� ���� ����������-���������� ������� - SP, OUT, �����������.
//			aucCoilsArray[(pxModbusMapping -> current_message_address_bits -
//						   COILS_ARRAY_MODBUS_BEGIN_ADDRESS)] = 1; //?
//		}
//		else
//		{
//			cout << "INCREMENT_SP_OUT_1__BIT_ARRAY_OFFSET 0" << endl;
//			// ������� ���� ����������-���������� ������� - SP, OUT, �����������.
//			aucCoilsArray[(pxModbusMapping -> current_message_address_bits -
//						   COILS_ARRAY_MODBUS_BEGIN_ADDRESS)] = 0; //?
//		}
//
//		// �������� ������ ������ � ������� ���������, � �������� ��������� �������.
//		nucIndexNumber =  ((((xPlcConfigService.xPlcConfigServiceData.ucLastAnalogueOutputModuleIndex) + 1) -
//							((xPlcConfigService.xPlcConfigServiceData.ucServiceAnalogueOutputQuantity) /
//							 MTVI5_ANALOG_OUTPUT_QUANTITY)) +
//						   ((unsigned char)(((uiAddress -
//											  COILS_ARRAY_MODBUS_BEGIN_ADDRESS) -
//											 INCREMENT_SP_OUT_BIT_ARRAY_OFFSET) /
//											(MTVI5_ANALOG_OUTPUT_QUANTITY))));
//
//		// �� ������� - nucIndexNumber ���� ������?
//		if (nucIndexNumber <= (xPlcConfigService.xPlcConfigServiceData.ucLastAnalogueOutputModuleIndex))
//		{
////        if (xPlcConfigService.xPlcConfigServiceData.ucLastAnalogueOutputModuleIndex)
////        {
//			// �������� ����� ���������� ������ �������� ������, � ������� ��������������-�������������� �������.
//			xAllModulesContext.axAllModulesContext[nucIndexNumber].
//			xModuleContextDinamic.
//			ucCommonIndex = (unsigned char)((((uiAddress -
//											   COILS_ARRAY_MODBUS_BEGIN_ADDRESS) -
//											  INCREMENT_SP_OUT_BIT_ARRAY_OFFSET) %
//											 MTVI5_ANALOG_OUTPUT_QUANTITY) +
//											CONVERT_INTEGER_TO_NATURAL_NUMBER);
//
//// � �������� ������ �������� ������, ������ ������� - SP, OUT,
//// ��������� �� �������� ������� axAnalogueOutputRegulatorContAv[�������� �� ����� ����������],
//// ����� ��������� ���� �� � ������������. �� ����� �� �������, �� ��������� Modbus,
//// ���������� �������� ������(HMI ������, ������� SCADA, �������������) �������� ������ ��� ����������� �� �������.
////  �� ������� ���� ����� ������� �������� � ������� �� SPI ���������� �������� � ������ ������������� ������-������.
//// ����� ������ ����� ������ �� ������ ������ �������� ������ �� ��������� ���� ������-������.
//// � ���� �� ������� ���� ������ � ������ ������ ������ ������� �������, �� ������� ��������� �� ���������.
////  �������, ����� ������ � ������ �������� �������, ���������� �� �������� � ���������� usAuxiliaryCounter(���������� � ���������),
//// ��� �� ������������ � ������ ��������.
////  ��� ��, ��� ������ ��� ����������� ����������� ������������ �������� ������, ��������� ������.
//// ��� ��� ����� �������� ������ �������� ������ ���������� ���� ��� � �������,
//// � ������ vDeviceControlModbusMessageHandler(), �������� - ����� ��������� ������ ���������� ������,
//// �� �� ��� �����, �������� �������� ������ ����, ����� ����������������-���������������� �������� �������
//// N - ���. �������� - N ���������� � ���������� usAuxiliaryCounter. � ����� ������� ����� ������ �������
//// �������� � �������, ���������� usAuxiliaryCounter - ����������.
//			// ������� �������� ����������, �� ������� � �������� ������ ��������� �������� �������.
//			xAllModulesContext.axAllModulesContext[nucIndexNumber].
//			xModuleContextDinamic.
//			usAuxiliaryCounter += 1;
//
//			// ��������� �������� ������ ������� - ����������������.
//			xAllModulesContext.axAllModulesContext[nucIndexNumber].
//			xModuleContextDinamic.
//			ucCommandControl = DEVICE_CONTROL_MTVI5_INCREMENT_SP_OUT;
//		}
//	}
//
////-----------------------------------------------------------------------------------------------------
//// ��������� ������� - SP, OUT, ����������� ������� �������� ������ - MTVI5.
//// ����� ������������� ���� ��������� � ��������� ���������� ������� - SP, OUT, ����������� ������� �������� ������?
//	if (((pxModbusMapping -> current_message_address_bits -
//			COILS_ARRAY_MODBUS_BEGIN_ADDRESS) >= DECREMENT_SP_OUT_BIT_ARRAY_OFFSET) &&
//			((pxModbusMapping -> current_message_address_bits -
//			  COILS_ARRAY_MODBUS_BEGIN_ADDRESS) < (DECREMENT_SP_OUT_BIT_ARRAY_OFFSET +
//					  DECREMENT_SP_OUT_BIT_ARRAY_LENGTH)))
//	{
//		// ��� ���������� ��� �������?
//		if (pxModbusMapping->tab_bits[uiAddress -
//									  COILS_ARRAY_MODBUS_BEGIN_ADDRESS])
//		{
//			// ���� ���������� - �������.
//			pxModbusMapping->tab_bits[uiAddress -
//									  COILS_ARRAY_MODBUS_BEGIN_ADDRESS] = 0;
//			cout << "DECREMENT_SP_OUT_BIT_ARRAY_OFFSET 1" << endl;
//			// ��������� ���� ����������-���������� ������� - SP, OUT, �����������.
//			aucCoilsArray[(pxModbusMapping -> current_message_address_bits -
//						   COILS_ARRAY_MODBUS_BEGIN_ADDRESS)] = 1; //?
//		}
//		else
//		{
//			cout << "DECREMENT_SP_OUT_BIT_ARRAY_OFFSET 0" << endl;
//			// ������� ���� ����������- ���������� ������� - SP, OUT, �����������.
//			aucCoilsArray[(pxModbusMapping -> current_message_address_bits -
//						   COILS_ARRAY_MODBUS_BEGIN_ADDRESS)] = 0; //?
//		}
//
//		// �������� ������ ������ � ������� ���������, � �������� ��������� �������.
//		nucIndexNumber =  ((((xPlcConfigService.xPlcConfigServiceData.ucLastAnalogueOutputModuleIndex) + 1) -
//							((xPlcConfigService.xPlcConfigServiceData.ucServiceAnalogueOutputQuantity) /
//							 MTVI5_ANALOG_OUTPUT_QUANTITY)) +
//						   ((unsigned char)(((uiAddress -
//											  COILS_ARRAY_MODBUS_BEGIN_ADDRESS) -
//											 DECREMENT_SP_OUT_BIT_ARRAY_OFFSET) /
//											(MTVI5_ANALOG_OUTPUT_QUANTITY))));
//		// �� ������� - nucIndexNumber ���� ������?
//		if (nucIndexNumber <= (xPlcConfigService.xPlcConfigServiceData.ucLastAnalogueOutputModuleIndex))
//		{
//			// �������� ����� ���������� ������ �������� ������, � ������� ��������������-�������������� �������.
//			xAllModulesContext.axAllModulesContext[nucIndexNumber].
//			xModuleContextDinamic.
//			ucCommonIndex = (unsigned char)((((uiAddress -
//											   COILS_ARRAY_MODBUS_BEGIN_ADDRESS) -
//											  DECREMENT_SP_OUT_BIT_ARRAY_OFFSET) %
//											 MTVI5_ANALOG_OUTPUT_QUANTITY) +
//											CONVERT_INTEGER_TO_NATURAL_NUMBER);
//
//// � �������� ������ �������� ������, ������ ������� - SP, OUT,
//// ��������� �� �������� ������� axAnalogueOutputRegulatorContAv[�������� �� ����� ����������],
//// ����� ��������� ���� �� � ������������. �� ����� �� �������, �� ��������� Modbus,
//// ���������� �������� ������(HMI ������, ������� SCADA, �������������) �������� ������ ��� ����������� �� �������.
////  �� ������� ���� ����� ������� �������� � ������� �� SPI ���������� �������� � ������ ������������� ������-������.
//// ����� ������ ����� ������ �� ������ ������ �������� ������ �� ��������� ���� ������-������.
//// � ���� �� ������� ���� ������ � ������ ������ ������ ������� �������, �� ������� ��������� �� ���������.
////  �������, ����� ������ � ������ �������� �������, ���������� �� �������� � ���������� usAuxiliaryCounter(���������� � ���������),
//// ��� �� ������������ � ������ ��������.
////  ��� ��, ��� ������ ��� ����������� ����������� ������������ �������� ������, ��������� ������.
//// ��� ��� ����� �������� ������ �������� ������ ���������� ���� ��� � �������,
//// � ������ vDeviceControlModbusMessageHandler() - �������� ����� ��������� ������ ���������� ������,
//// �� �� ��� �����, �������� �������� ������ ����, ����� ����������������-���������������� �������� �������
//// N - ���. �������� - N ���������� � ���������� usAuxiliaryCounter. � ����� ������� ����� ������ �������
//// �������� � �������, ���������� usAuxiliaryCounter - ����������.
//			// ������� �������� ����������, �� ������� � �������� ������ ��������� �������� �������.
//			xAllModulesContext.axAllModulesContext[nucIndexNumber].
//			xModuleContextDinamic.
//			usAuxiliaryCounter += 1;
//
//			// ��������� �������� ������ ������� - ����������������.
//			xAllModulesContext.axAllModulesContext[nucIndexNumber].
//			xModuleContextDinamic.
//			ucCommandControl = DEVICE_CONTROL_MTVI5_DECREMENT_SP_OUT;
//		}
//	}
//
////-----------------------------------------------------------------------------------------------------
//// ������ ���������� �������� �����������(CONT_ST), ������� �������� ������ - MTVI5.
//	// ����� ������������� ���� ��������� � ��������� ������� ���������� �������� �����������(CONT_ST) ������� �������� ������?
//	if (((pxModbusMapping -> current_message_address_bits -
//			COILS_ARRAY_MODBUS_BEGIN_ADDRESS) >= CONT_ST_BIT_ARRAY_OFFSET) &&
//			((pxModbusMapping -> current_message_address_bits -
//			  COILS_ARRAY_MODBUS_BEGIN_ADDRESS) < (CONT_ST_BIT_ARRAY_OFFSET +
//					  CONT_ST_BIT_ARRAY_LENGTH)))
//	{
//		// ��� ���������� ��� �������?
//		if (pxModbusMapping->tab_bits[uiAddress -
//									  COILS_ARRAY_MODBUS_BEGIN_ADDRESS])
//		{
//			// ���� ���������� - �������.
//			pxModbusMapping->tab_bits[uiAddress -
//									  COILS_ARRAY_MODBUS_BEGIN_ADDRESS] = 0;
//			cout << "CONT_ST_BIT_ARRAY 1" << endl;
//			// ��������� ��� ���������� �������� �����������.
////            aucCoilsArray[(pxModbusMapping -> current_message_address_bits -
////                           COILS_ARRAY_MODBUS_BEGIN_ADDRESS)] = 1; //?
//			aucContStWriteBitArray[(pxModbusMapping -> current_message_address_bits -
//									COILS_ARRAY_MODBUS_BEGIN_ADDRESS) -
//								   CONT_ST_BIT_ARRAY_OFFSET] = 1;
//		}
//		else
//		{
//			cout << "CONT_ST_BIT_ARRAY 0" << endl;
//			// ������� ��� ���������� �������� �����������.
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
//		// �������� ������ ������ � ������� ���������, � �������� ��������� �������.
//		nucIndexNumber =  ((((xPlcConfigService.xPlcConfigServiceData.ucLastAnalogueOutputModuleIndex) + 1) -
//							((xPlcConfigService.xPlcConfigServiceData.ucServiceAnalogueOutputQuantity) /
//							 MTVI5_ANALOG_OUTPUT_QUANTITY)) +
//						   ((unsigned char)(((uiAddress -
//											  COILS_ARRAY_MODBUS_BEGIN_ADDRESS) -
//											 CONT_ST_BIT_ARRAY_OFFSET) /
//											(MTVI5_STAT_BIT_QUANTITY * MTVI5_ANALOG_OUTPUT_QUANTITY))));
//		// �� ������� - nucIndexNumber ���� ������?
//		if (nucIndexNumber <= (xPlcConfigService.xPlcConfigServiceData.ucLastAnalogueOutputModuleIndex))
//		{
////            // ��������� �������� ������ ������� - ������ ���������� �������� �����������.
////            xAllModulesContext.axAllModulesContext[nucIndexNumber].
////            xModuleContextDinamic.
////            ucCommandControl = DEVICE_CONTROL_MTVI5_CONT_ST_WRITE;
//		}
//	}
//
////-----------------------------------------------------------------------------------------------------
//// ������ ������ ���������� �������� ������.
//	// ����� ������������� ���� ��������� � ��������� ������� ������ ���������� �������� ������?
//	if (((pxModbusMapping -> current_message_address_bits -
//			COILS_ARRAY_MODBUS_BEGIN_ADDRESS) >= LOG_COM_BIT_ARRAY_OFFSET) &&
//			((pxModbusMapping -> current_message_address_bits -
//			  COILS_ARRAY_MODBUS_BEGIN_ADDRESS) < (LOG_COM_BIT_ARRAY_OFFSET +
//					  LOG_COM_BIT_ARRAY_LENGTH)))
//	{
//		// ��� ���������� ��� �������?
//		if (pxModbusMapping->tab_bits[uiAddress -
//									  COILS_ARRAY_MODBUS_BEGIN_ADDRESS])
//		{
//			// ���� ���������� - �������.
//			pxModbusMapping->tab_bits[uiAddress -
//									  COILS_ARRAY_MODBUS_BEGIN_ADDRESS] = 0;
//			cout << "LOG_COM_BIT_ARRAY_OFFSET 1" << endl;
//			// ��������� ��� ������ ���������� �������� ������.
//			aucCoilsArray[(pxModbusMapping -> current_message_address_bits -
//						   COILS_ARRAY_MODBUS_BEGIN_ADDRESS)] = 1;
//		}
//		else
//		{
//			cout << "LOG_COM_BIT_ARRAY_OFFSET 0" << endl;
//			// ������� ��� ������ ���������� �������� ������.
//			aucCoilsArray[(pxModbusMapping -> current_message_address_bits -
//						   COILS_ARRAY_MODBUS_BEGIN_ADDRESS)] = 0;
//		}
//	}
//
////-----------------------------------------------------------------------------------------------------
//// ������� �� ��������� ���������� ����.
////    // ��������� ������ � ���������� ModbusRTU HMI?
////    if ((pxModbusMapping -> message_sourse) ==
////            MODBUS_MESSAGE_SOURCE_RTU_HMI)
//	//    {
//	// ����� ������������� ���� ��������� � ��������� ������� ������ �� ��������� ���������� ������?
//	if (((pxModbusMapping -> current_message_address_bits -
//			COILS_ARRAY_MODBUS_BEGIN_ADDRESS) >= AIN_OFF_BIT_ARRAY_OFFSET) &&
//			((pxModbusMapping -> current_message_address_bits -
//			  COILS_ARRAY_MODBUS_BEGIN_ADDRESS) < (AIN_OFF_BIT_ARRAY_OFFSET +
//					  AIN_OFF_BIT_ARRAY_LENGTH)))
//	{
//		// ����� ���� ��� ���� ������ �� ����� �� ��������� ���������� ����?
//		if ((xMainFlagRegister.ui8AinOffProcessOwnerIndex == AIN_OFF_PROCESS_OWNER_IS_NONE) ||
//				(xMainFlagRegister.ui8AinOffProcessOwnerIndex == AIN_OFF_PROCESS_OWNER_IS_HMI))
//		{
//			// �������� ������ ������ � ������� ���������, � �������� ��������� �������.
//			nucIndexNumber =  ((((xPlcConfigService.xPlcConfigServiceData.ucLastAnalogueInputModuleIndex) + 1) -
//								(xPlcConfigService.xPlcConfigServiceData.ucServiceAnalogueInputModuleQuantity)) +
//							   ((unsigned char)(((uiAddress -
//												  COILS_ARRAY_MODBUS_BEGIN_ADDRESS) -
//												 AIN_OFF_BIT_ARRAY_OFFSET) /
//												(MVAI5_ANALOG_INPUT_QUANTITY))));
//			// �� ������� - nucIndexNumber ���� ������?
//			if (nucIndexNumber <= (xPlcConfigService.xPlcConfigServiceData.ucLastAnalogueInputModuleIndex))
//			{
//				// ����� � ������� ��������� ���������� �� ��������� ���������� ����.
//				for (i = 0; i < MAX_HANDLED_ANALOGUE_INPUT; i++)
//				{
//					// ���������� ���� - i ������� �� ���������?
//					if (aucCoilsArray[AIN_OFF_BIT_ARRAY_OFFSET + i])
//					{
//						// ����� � ������� ��������� ���������� �� ��������� ���������� ���� - i.
//						aucCoilsArray[AIN_OFF_BIT_ARRAY_OFFSET + i] = 0;
////                        // �������� ������ ������ � ������� ���������, ������� � ������� ������ ��������� �������.
////                        nucIndexNumber =  ((((xPlcConfigService.xPlcConfigServiceData.ucLastAnalogueInputModuleIndex) + 1) -
////                                            (xPlcConfigService.xPlcConfigServiceData.ucServiceAnalogueInputModuleQuantity)) +
////                                           ((unsigned char)((i) /
////                                                            (MVAI5_ANALOG_INPUT_QUANTITY))));
//						// ��������� �������� ������ ����� ������������ �����. ���� 0, ���������� ���������.
//						// �������� ���������� ���� ������ ������;
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
//				// ��� ���������� ��� �������?
//				if (pxModbusMapping->tab_bits[uiAddress -
//											  COILS_ARRAY_MODBUS_BEGIN_ADDRESS])
//				{
//					// ��������� ������ ����������.
//					cout << "AIN_OFF_BIT_ARRAY_OFFSET 1" << endl;
//					// ���� ���������� - �������.
//					pxModbusMapping->tab_bits[uiAddress -
//											  COILS_ARRAY_MODBUS_BEGIN_ADDRESS] = 0;
//					// �������� � ��������� �������� ������ ����� ������������ �����.
//					xAllModulesContext.axAllModulesContext[nucIndexNumber].
//					xModuleContextDinamic.
//					ucCommonIndex = (unsigned char)((((uiAddress -
//													   COILS_ARRAY_MODBUS_BEGIN_ADDRESS) -
//													  AIN_OFF_BIT_ARRAY_OFFSET) %
//													 MVAI5_ANALOG_INPUT_QUANTITY) +
//													CONVERT_INTEGER_TO_NATURAL_NUMBER);
//					// ������� ���������� ���� �� ���������.
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
//					// ���������� ������ ����������.
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
//// ��������� ������� - ������ ������������.
//	if (((pxModbusMapping -> current_message_address_bits -
//			COILS_ARRAY_MODBUS_BEGIN_ADDRESS) ==
//			CONFIGURATION_SAVE_COMMAND_BIT_ARRAY_OFFSET))
//	{
//		// ��� ���������� ��� �������?
//		if (pxModbusMapping->tab_bits[uiAddress -
//									  COILS_ARRAY_MODBUS_BEGIN_ADDRESS])
//		{
//			// ���� ���������� - �������.
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
//				// ���������� ������������ � ���� ������.
//				iPlcConfigurationSave();
//				iPlcDataBaseSave();
////    vWorkArraysUpdate();
//				iBadModuleBuffClear();
//				// ����� ������ ������ ������������ � ���� ������,
//				// ��� �������� �� ���������� ����������� ������.
//				fbPlcConfigurationError = 0;
//				fbPlcConfigurationFileError = 0;
//				fbPlcDataBaseFileError = 0;
//				// ��������� ���� "������������ ���������".
//				fbPlcErrorReset = 1;
//				xPlcConfigDataTransmit.xConfigData.ucErrorCode = 0;
//
//				// �������������� �������.
//				CEvents::EventRegistration(
//					0,
//					(CEvents::HANDLED_EVENTS_SYSTEM_EVENTS_TYPE |
//					 CEvents::HANDLED_EVENTS_IS_ARCHIVE),
//					0,
//					"����� ����. � ��.");
//
//				cout << "CONFIGURATION_ERROR_NEW_CONFIGURATION_AND_DATA_BASE 2" << endl;
//				break;
//			case CONFIGURATION_ERROR_NEW_CONFIGURATION:
////            iPlcConfigurationCreate();
////            vMainTaskMemoryMapFormation();
//				// ���������� ������������ � ���� ������.
//				iPlcConfigurationSave();
//				//                iPlcDataBaseSave();
////    vWorkArraysUpdate();
//				iBadModuleBuffClear();
//				// ����� ������ ������ ������������ � ���� ������,
//				// ��� �������� �� ���������� ����������� ������.
//				fbPlcConfigurationError = 0;
//				fbPlcConfigurationFileError = 0;
//				fbPlcDataBaseFileError = 0;
//				// ��������� ���� "������������ ���������".
//				fbPlcErrorReset = 1;
//				xPlcConfigDataTransmit.xConfigData.ucErrorCode = 0;
//
//				// �������������� �������.
//				CEvents::EventRegistration(
//					0,
//					(CEvents::HANDLED_EVENTS_SYSTEM_EVENTS_TYPE |
//					 CEvents::HANDLED_EVENTS_IS_ARCHIVE),
//					0,
//					"����� ������������.");
//
//				cout << "CONFIGURATION_ERROR_NEW_CONFIGURATION 3" << endl;
//				break;
//			case CONFIGURATION_ERROR_BAD_MODULE_FINDED_AND_REGISTERED:
//				// ����� ������ ������ ������������ � ���� ������,
//				// ��� �������� �� ���������� ����������� ������.
//				fbPlcConfigurationError = 0;
//				fbPlcConfigurationFileError = 0;
//				fbPlcDataBaseFileError = 0;
//				// ��������� ���� "������������ ���������".
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
//			// ������� ��� ������ - ������������.
//		}
//	}

    return 0;
}

//-------------------------------------------------------------------------------
uint16_t CDeviceControl::DataBaseBlockReadBlockRelatedAction(void)
{
    std::cout << "CDeviceControl::DataBaseBlockReadBlockRelatedAction 1" << std::endl;

    // ����� ��������� ��� ������ ����� ���� ������.
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
        ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
        SetFsmState(DONE_OK);
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

    // ����� ��������� ��� ������ ����� ���� ������.
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
            // ����� �������� ���������� ������� �����������?
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
            // ����� �������� ���������� ������� �����������?
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
            // ����� �������� ���������� ������� �����������?
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

            // �������� �������������� �������� ��������� � ������� ������������ ����� ���� ������.
            // �������� ��� ������ ����� ������ ���������� �������� ��������� ���� �� ������.
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
            // ������� � ���, ��� ������� ������ ����� ������� ���������.
            // ��� ������ ����� � ���� ������ �� ��������� ��������� ��������� ���������� ������.
            // �������� ���������� ���������� ������������ � _FC_PROGRAMMING_COMPLETION_REQUEST 14 �������.
            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
        }
        break;

    case DATA_BASE_BLOCK_WRITE_EXECUTOR_DONE_OK_ANSWER_PROCESSING:
        std::cout << "CDeviceControl::Fsm DATA_BASE_BLOCK_WRITE_EXECUTOR_DONE_OK_ANSWER_PROCESSING"  << std::endl;
        {
            // �������� �������������� �������� ��������� � ������� ������������ ����� ���� ������.
            // �������� ��� ������ ����� ������ ���������� �������� ������� ���� � � ������.
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
            SetFsmNextStateDoneOk(DATA_BASE_BLOCK_NETWORK_ADDRESS_WRITE_EXECUTOR_DONE_OK_ANSWER_PROCESSING);
            SetFsmNextStateReadyWaitingError(DATA_BASE_BLOCK_NETWORK_ADDRESS_WRITE_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
            SetFsmNextStateDoneWaitingError(DATA_BASE_BLOCK_NETWORK_ADDRESS_WRITE_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
            SetFsmNextStateDoneWaitingDoneError(DATA_BASE_BLOCK_NETWORK_ADDRESS_WRITE_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
        }
        break;

    case DATA_BASE_BLOCK_NETWORK_ADDRESS_WRITE_EXECUTOR_DONE_OK_ANSWER_PROCESSING:
        std::cout << "CDeviceControl::Fsm DATA_BASE_BLOCK_NETWORK_ADDRESS_WRITE_EXECUTOR_DONE_OK_ANSWER_PROCESSING"  << std::endl;
        {
            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
            SetFsmState(DATA_BASE_BLOCK_NETWORK_ADDRESS_WRITE_COMPLETE_CHECK_START);
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

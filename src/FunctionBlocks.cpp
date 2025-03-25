
#include "PasNewConfig.h"
#include "FunctionBlocks.h"
#include "Alarm.h"
#include "Events.h"

using namespace std;

//-----------------------------------------------------------------------------------------------------
// ��������������� �� ������ ������� ���� ������, � ������ �������� � RAM.
// ���� ������ � ������� - ��� ������, ������ 100 ������. ������ ���� 256 ����.
// ����� ��� ������ �������, ���������� ��������, �������������� ������ � ��., ����� ��������� �����.
// ���� ������ �������, ���������� ��������, �������������� ������ � ��., ����� �������� ��������� ������.
// ����� ����� ����������� "�������" ��������� � ���������� ��������, �������� ��������������.

void vFunctionBlockDataBaseExstract(void)
{
    unsigned char ucCheck;
    unsigned char nucBlocksInBlockCounter; // ���� ��������� - ���� ����, � ����� �����.
    unsigned char nucBlockCounter;
    unsigned char nucRelayOutCounter;
    unsigned char nucIndexNumber;
    unsigned char *pucSource;
    unsigned char *pucDestination;
    unsigned char ucFlowControl;
    TFunctionBlockDescriptionWork *pxFunctionBlockDataBaseSource;
    TFunctionBlockDescriptionWorkPackOne *pxFunctionBlockDescriptionWork;
    TFunctionBlockDescriptionWorkPackOne xFunctionBlockDescriptionWork;

//    nucBlocksInBlockCounter = 0;
//    nucBlockCounter = 0;
//    nucIndexNumber = 0;
//    pxFunctionBlockDescriptionWork = (TFunctionBlockDescriptionWorkPackOne*)&(xPlcDataBase.
//                                    axPlcDataBaseBlocks[FUNCTION_BLOCK_DATA_BASE_BLOCK_OFFSET].
//                                    aucPlcDataBaseBlockData[0]);
//    // ��������� ���������� �������������������, �������������� �������������� ������.
//    for (int i = 0;
//            i < MAX_HANDLED_FUNCTION_BLOCK;
//            i++)
//    {
//        // ��������� ���� ���� ���� ������?
//        if (nucBlocksInBlockCounter == FUNCTION_BLOCK_DATA_BASE_BLOCKS_IN_BLOCK_QUANTITY)
//        {
//            // ��������� ���� ���� ������.
//            nucBlockCounter++;
//            // ������� ��������� �� ���� � ����� ���� ������ �������.
//            pxFunctionBlockDescriptionWork = (TFunctionBlockDescriptionWorkPackOne*)&(xPlcDataBase.
//                                            axPlcDataBaseBlocks[FUNCTION_BLOCK_DATA_BASE_BLOCK_OFFSET + nucBlockCounter].
//                                            aucPlcDataBaseBlockData[0]);
//            nucIndexNumber = 0;
//        }
//
//        pucSource = (unsigned char*)&pxFunctionBlockDescriptionWork[nucIndexNumber];
//        ucCheck = 0;
//        // ��������� ������ ���������, ����, �� ����� ����, �� �������������� ���� ����������������.
//        for (int j = 0;
//                j < (sizeof(struct TFunctionBlockDescriptionWorkPackOne) - ONE_BYTE_CRC_LENGTH);
//                j++)
//        {
//            ucCheck += pucSource[j];
//        }
//
//        if (ucCheck)
//        {
//            // ��� ���� �������������� ���� ����������������.
//            nucBlocksInBlockCounter++;
//            nucIndexNumber++;
//        }
//        else
//        {
//            break;
//        }
//    }
//
//    xPlcConfigService.xPlcConfigServiceData.
//    uiHandledFunctionBlockQuantity =
//        nucBlocksInBlockCounter;


    // ������� ��������� �� ����� � ��������������� ����� ������ �������.
    pxFunctionBlockDataBaseSource = axFunctionBlockDescriptionWork;
    // ������� ��������� �� ���� � ����� ���� ������ �������.
    pxFunctionBlockDescriptionWork = (TFunctionBlockDescriptionWorkPackOne*)&(xPlcDataBase.
                                     axPlcDataBaseBlocks[FUNCTION_BLOCK_DATA_BASE_BLOCK_OFFSET].
                                     aucPlcDataBaseBlockData[0]);
    nucBlockCounter = 0;
    nucBlocksInBlockCounter = 0;
    // ����������� �� ������ ������� ���� ������, � ������ �������� � RAM.
    for (int i = 0;
            i < MAX_HANDLED_FUNCTION_BLOCK;
            i++)
    {
        // ��������� ���� ���� ���� ������?
        if (nucBlocksInBlockCounter == FUNCTION_BLOCK_DATA_BASE_BLOCKS_IN_BLOCK_QUANTITY)
        {
            // ��������� ���� ���� ������.
            nucBlockCounter++;
            pxFunctionBlockDescriptionWork = (TFunctionBlockDescriptionWorkPackOne*)&(xPlcDataBase.
                                             axPlcDataBaseBlocks[FUNCTION_BLOCK_DATA_BASE_BLOCK_OFFSET + nucBlockCounter].
                                             aucPlcDataBaseBlockData[0]);
            nucBlocksInBlockCounter = 0;
        }
        // ��������� ���� ��������� ��������������� �����.

        memcpy((uint8_t*)&xFunctionBlockDescriptionWork,
               (uint8_t*)&pxFunctionBlockDescriptionWork[nucBlocksInBlockCounter],
               sizeof(struct TFunctionBlockDescriptionWorkPackOne));

        pxFunctionBlockDataBaseSource[i].ucFunctionBlockType =
            xFunctionBlockDescriptionWork.ucFunctionBlockType;

        memcpy(pxFunctionBlockDataBaseSource[i].aucInputAddress,
               xFunctionBlockDescriptionWork.aucInputAddress,
               8);

        pxFunctionBlockDataBaseSource[i].ucTalArhGrp =
            xFunctionBlockDescriptionWork.ucTalArhGrp;

        memcpy(pxFunctionBlockDataBaseSource[i].aucRelayOut,
               xFunctionBlockDescriptionWork.aucRelayOut,
               8);

        // �������� ����������� ����� ���������.
        pucSource = (unsigned char*)&pxFunctionBlockDataBaseSource[i];
        ucCheck = 0xFF;
        for (int j = 0;
                j < (sizeof(struct TFunctionBlockDescriptionWork) - ONE_BYTE_CRC_LENGTH);
                j++)
        {
            ucCheck += pucSource[j];
        }
        pxFunctionBlockDataBaseSource[i].ucCrc = ucCheck;
        // ��������� ���������.
        nucBlocksInBlockCounter++;
    }
}

//-----------------------------------------------------------------------------------------------------
// ��������� ���������� ����������� ����� ������ ��������� ��������������� �����.
// �������� ��������� �� ���� ��������� ��������������� ����� � ��������������� ���� ������,
// ��������������� �������� vFunctionBlockDataBaseExstract().
int iFunctionBlockDataBaseCrcCheck(
    TFunctionBlockDescriptionWork *pxFunctionBlockDescriptionWork)
{
    unsigned char ucCheck;
    unsigned char *pucSource;
    // ������� ��������� �� ��������� � ��������������� ���� ������.
    pucSource = (unsigned char*)pxFunctionBlockDescriptionWork;
    // ��������� �������� CRC. ����� ����� �� ���� �������.
    ucCheck = 0xFF;
    // ��������� ����� ��������� ���������� struct TFunctionBlockDescriptionWork
    // ����� ����� CRC.
    for (int i = 0;
            i < (sizeof(struct TFunctionBlockDescriptionWork) - ONE_BYTE_CRC_LENGTH);
            i++)
    {
        ucCheck += pucSource[i];
    }

    // CRC ���������?
    if (pxFunctionBlockDescriptionWork -> ucCrc == ucCheck)
    {
        return 0;
    }
    else
    {
        return -1;
    }
}

//-----------------------------------------------------------------------------------------------------
// �������� ��������� ����������� ������� �� ������ - �����, �� ��������� - ����.
// ���� ������ ������������, �� ���������� ������(-1).
int iFunctionBlockInputCheck(unsigned char ucInputSource, unsigned short usSourceAddress)
{
    // �� ������ ��������� ���������� ������?
    switch(ucInputSource)
    {
        // ���������� �������� ������� ���������� ��������.
    case FUNCTION_BLOCK_INPUT_SOURCE_DI_VALUE:
        // �������� ����������� ������� ����������������? �
        // ���������� ������ ����������?
        if ((usSourceAddress) && (!(aucCoilsArray[BAD_DI_BIT_ARRAY_OFFSET +
                                    (usSourceAddress - 1)])))
        {
            return aucDiscreteInputsArray[DI_VALUE_BIT_ARRAY_OFFSET +
                                          (usSourceAddress - 1)];
        }
        else
        {
            // ���������� ������ ������������ ��� �� ����������������.
            return -1;
        }
        break;
        // ������ �������������� ������ ��� ������� ������� ���������� �������.
        // ������: 1 - MAX_HANDLED_FUNCTION_BLOCK, - ����� �� (OutFbl).
        // ������: (MAX_HANDLED_FUNCTION_BLOCK + 1) - 184, - ������� ���(LogCom 64 �������).
    case FUNCTION_BLOCK_INPUT_SOURCE_FUNCTION_BLOCK_OUTS:
        // �������� ����������� ������� ����������������?
        if (usSourceAddress)
        {
            // �������� ����������� ������� - ����� ��������������� �����?
            if (usSourceAddress <= MAX_HANDLED_FUNCTION_BLOCK)
            {
                // �������� ����������� ������� - ����� ��������������� �����.
                return aucCoilsArray[CURRENT_STEP_FUNCTION_BLOCK_OUTS_BIT_ARRAY_OFFSET +
                                     (usSourceAddress - 1)];
            }
            // �������� ����������� ������� - ������� ������� ���������� �������?
            else if (usSourceAddress <= (MAX_HANDLED_FUNCTION_BLOCK + LOG_COM_BIT_ARRAY_LENGTH))
            {
                // �������� ����������� ������� - ������� ������� ���������� �������.
                return aucCoilsArray[LOG_COM_BIT_ARRAY_OFFSET +
                                     (usSourceAddress - 1) -
                                     MAX_HANDLED_FUNCTION_BLOCK];
            }
            // �������� ����������� ������� - ������� ������� "������������"?
            else if (usSourceAddress == (MAX_HANDLED_FUNCTION_BLOCK + LOG_COM_BIT_ARRAY_LENGTH +
                                         KVIT_COMMAND_BIT_ARRAY_LENGTH))
            {
                return fbGlobalKvit;//debag//
            }
            // �������� ����������� ������� - ������� ������� "�����"?
            else if (usSourceAddress == (MAX_HANDLED_FUNCTION_BLOCK + LOG_COM_BIT_ARRAY_LENGTH +
                                         KVIT_COMMAND_BIT_ARRAY_LENGTH + RESET_COMMAND_BIT_ARRAY_LENGTH))
            {
                return fbGlobalReset;//debag//
            }

//            // �������� ����������� ������� - ������� ������� ���������� �������?
//            if (usSourceAddress > MAX_HANDLED_FUNCTION_BLOCK)
//            {
//                // �������� ����������� ������� - ������� ������� ���������� �������.
//                return aucCoilsArray[LOG_COM_BIT_ARRAY_OFFSET +
//                                     (usSourceAddress - 1) -
//                                     MAX_HANDLED_FUNCTION_BLOCK];
//            }
//            // �������� ����������� ������� - ������� ������� ���������� �������?
//            else if (usSourceAddress > MAX_HANDLED_FUNCTION_BLOCK)
//            {
//                // �������� ����������� ������� - ����� ��������������� �����.
//                return aucCoilsArray[CURRENT_STEP_FUNCTION_BLOCK_OUTS_BIT_ARRAY_OFFSET +
//                                     (usSourceAddress - 1)];
//            }
        }
        else
        {
            // ���������� ������ �� ����������������.
            return -1;
        }
        break;
        // ���� ������������� ���������� ������� ��������.
    case FUNCTION_BLOCK_INPUT_SOURCE_BAD_AI:
        // �������� ����������� ������� ����������������?
        if (usSourceAddress)
        {
            return aucCoilsArray[BAD_AI_BIT_ARRAY_OFFSET +
                                 (usSourceAddress - 1)];
        }
        else
        {
            // ���������� ������ �� ����������������.
            return -1;
        }
        break;
        // ��������� ������� �������������� ������ CoDeSys.
    case FUNCTION_BLOCK_INPUT_SOURCE_LOG_SOST:
        // �������� ����������� ������� ����������������?
        if (usSourceAddress)
        {
            return aucCoilsArray[LOG_SOST_BIT_ARRAY_OFFSET +
                                 (usSourceAddress - 1)];
        }
        else
        {
            // ���������� ������ �� ����������������.
            return -1;
        }
        break;
        // ���� ������������� ���������� ������� ��������.
    case FUNCTION_BLOCK_INPUT_SOURCE_BAD_DI:
        // �������� ����������� ������� ����������������?
        if (usSourceAddress)
        {
            return aucCoilsArray[BAD_DI_BIT_ARRAY_OFFSET +
                                 (usSourceAddress - 1)];
        }
        else
        {
            // ���������� ������ �� ����������������.
            return -1;
        }
        break;

    default:
        // ����������� �������� ����������� �������.
        return -1;
        break;
    };
}

//-----------------------------------------------------------------------------------------------------
void vFunctionBlockHandler(void)
{
    unsigned char *pucSource;
    float *pfAnalogueInputSource;
    float *pfCalculatedValueSource;
    float fAnalogueData;
    float fSetPoint;
    float fMinOfScale;
    float fMaxOfScale;
    float fDifference;
    float fHysteresisDifference;
    float fHysteresis;
    float fHysteresisScale;
    unsigned char *pucFunctionBlockOuts;
    unsigned char *pucDoValueDestination;
    unsigned char *pucDoValueBlockDestination;
    unsigned char ucFlowControl;
    unsigned char ucFunctionBlockPreviousState;
    unsigned char ucFunctionBlockCurrentState;
    unsigned char ucFunctionBlockIntermediateState;
    int iDiscreteInputState;
    unsigned char ucInputSet;
    unsigned char ucInputReset;
    bool fbInputIsReliable;
    bool fbInputSetIsReliable;
    bool fbInputResetIsReliable;
    TFunctionBlockDescriptionWork *pxFunctionBlockDescriptionWork;
    TFunctionBlockWorkingData *pxFunctionBlockWorkingData;
    TAnalogueInputDescriptionWork *pxAnalogueInputDescriptionWork;
    TAlarmHmi *pxAlarmHmi;

    // ������� ��������� �� ����� � ��������������� ����� ������ �������������� ������ �������.
    pxFunctionBlockDescriptionWork =
        axFunctionBlockDescriptionWork;
    pxFunctionBlockWorkingData = &axFunctionBlockWorkingData[0];
    // ������� ��������� �� ����� � ��������������� ����� ������ ���������� ������.
    pxAnalogueInputDescriptionWork =
        axAnalogueInputDescriptionWork;
    pucFunctionBlockOuts =
        &aucCoilsArray[CURRENT_STEP_FUNCTION_BLOCK_OUTS_BIT_ARRAY_OFFSET];
    pfAnalogueInputSource =
        afAnalogueInputData;
    pfCalculatedValueSource =
        (float*)&aucHoldingRegistersArray[CALC_CDS_BYTE_ARRAY_LENGTH];
    pucDoValueDestination =
        &aucDoValueByteArray[0];
    pucDoValueBlockDestination =
        &aucDoValueBlockByteArray[0];//&aucCoilsArray[DO_VALUE_BIT_ARRAY_OFFSET];
    // ������� ��������� �� ������� ������ ������������ ���������� ����������� ������������ ���������� �������� �� HMI.
    // ������ ��������� �� ����� � ������� ����� ������ ���������� ��������.
    pxAlarmHmi =
        &axAlarmHmi[(xPlcConfigService.xPlcConfigServiceData.
                     uiHandledDiscreteInputQuantity)];

//cout << "uiHandledFunctionBlockQuantity " << (int)(xPlcConfigService.xPlcConfigServiceData.
//                 uiHandledFunctionBlockQuantity) << endl;

    // ���������� ��� ������������������� �������������� �����.
    for (int i = 0;
            i < (xPlcConfigService.xPlcConfigServiceData.
                 uiHandledFunctionBlockQuantity);
            i++)
    {
        // ��������� ��������������� ����� � ���� ������ ��������?
        if (iFunctionBlockDataBaseCrcCheck(pxFunctionBlockDescriptionWork) == -1)
        {
            fbFunctionBlockDataBaseError = 1;
//            cout << "iFunctionBlockDataBaseCrcCheck error" << endl;
            // error

            // �������� ��������� ������� ��� �� ����������������?
            if(xCInternalModuleErrorEvent.EventOnIsNotRegistered(
                        SYSTEM_ERROR_GROUP_OFFSET,
                        SYSTEM_ERROR_FBL_DATA_BASE_ERROR_OFFSET))
            {
                // �������������� �������� ��������� �������.
                CEvents::EventRegistration(
                    SYSTEM_ERROR_GROUP_OFFSET,
                    (CEvents::HANDLED_EVENTS_SYSTEM_ERROR_TYPE |
                     CEvents::HANDLED_EVENTS_IS_POPUP |
                     CEvents::HANDLED_EVENTS_IS_SOUND |
                     CEvents::HANDLED_EVENTS_IS_ARCHIVE),
                    SYSTEM_ERROR_FBL_DATA_BASE_ERROR_OFFSET,
                    "������ �.�. ���");
            }
        }
        else
        {
            // ������� ��������� ��������������� ����� �� ���������� ����.
            ucFunctionBlockPreviousState = *pucFunctionBlockOuts;
            // ������� ���� �������� ��������� ��������������� �����.
            ucFunctionBlockCurrentState = 0;
            // ����� ���  ��������������� �����?
            switch(FUNCTION_BLOCK_TYPE_FBL((pxFunctionBlockDescriptionWork)))
            {
//-----------------------------------------------------------------------------------------------------
                // �������������� ���� "�".
            case FUNCTION_BLOCK_FLOW_AND:
                ucFunctionBlockCurrentState =
                    ucFourInputElementAnd(pxFunctionBlockDescriptionWork);
                // �������� ��������� ��������������� ����� �� ������� ����.
                *pucFunctionBlockOuts = ucFunctionBlockCurrentState;
                break;

//-----------------------------------------------------------------------------------------------------
                // �������������� ���� "�-��".
            case FUNCTION_BLOCK_FLOW_AND_NOT:
                ucFunctionBlockCurrentState =
                    ucFourInputElementAnd(pxFunctionBlockDescriptionWork);
                // ������� ��������� ������ � ������������ � ������� "�-��".
                if (ucFunctionBlockCurrentState)
                {
                    ucFunctionBlockCurrentState = 0;
                }
                else
                {
                    ucFunctionBlockCurrentState = 1;
                }
                // �������� ��������� ��������������� ����� �� ������� ����.
                *pucFunctionBlockOuts = ucFunctionBlockCurrentState;
                break;

//-----------------------------------------------------------------------------------------------------
                // �������������� ���� "���".
            case FUNCTION_BLOCK_FLOW_OR:
                ucFunctionBlockCurrentState =
                    ucFourInputElementOr(pxFunctionBlockDescriptionWork);
                // �������� ��������� ��������������� ����� �� ������� ����.
                *pucFunctionBlockOuts = ucFunctionBlockCurrentState;
                break;

//-----------------------------------------------------------------------------------------------------
                // �������������� ���� "���-��".
            case FUNCTION_BLOCK_FLOW_OR_NOT:
                ucFunctionBlockCurrentState =
                    ucFourInputElementOr(pxFunctionBlockDescriptionWork);
                // ������� ��������� ������ � ������������ � ������� "���-��".
                if (ucFunctionBlockCurrentState)
                {
                    ucFunctionBlockCurrentState = 0;
                }
                else
                {
                    ucFunctionBlockCurrentState = 1;
                }
                // �������� ��������� ��������������� ����� �� ������� ����.
                *pucFunctionBlockOuts = ucFunctionBlockCurrentState;
                break;

//-----------------------------------------------------------------------------------------------------
                // �������������� ���� "����������".
            case FUNCTION_BLOCK_FLOW_HISTERESIS:
                // �� ���������� ���� �������������� ���� ��� �������?
                if (ucFunctionBlockPreviousState)
                {
                    // �� ���������� ���� �������������� ���� ��� �������.
                    // �������� ��� - ��������� ��� "���".
                    ucFunctionBlockCurrentState =
                        ucFourInputElementOr(pxFunctionBlockDescriptionWork);
                    // �������� ��������� ��������������� ����� �� ������� ����.
                    *pucFunctionBlockOuts = ucFunctionBlockCurrentState;
                }
                else
                {
                    // �� ���������� ���� �������������� ���� �� ��� �������.
                    // ������ ��� - ��������� ��� "�".
                    ucFunctionBlockCurrentState =
                        ucFourInputElementAnd(pxFunctionBlockDescriptionWork);
                    // �������� ��������� ��������������� ����� �� ������� ����.
                    *pucFunctionBlockOuts = ucFunctionBlockCurrentState;
                }
                break;

//-----------------------------------------------------------------------------------------------------
// �������������� ���� "������".
// *** ��� �������:
// *** - 0 �������� ���������
// *** - 1 ������
// *** - 2 �������� ����������
// *** - 3 �������� �������
// *** - 4 ���������� �������
// *** �������� ������ ***
// ��� ����� ���-05 � �/� "vos_pred_8" ����� ������� �������� ���. � "1"
// ��� ������������ ��������� �������� ������ ������� � ������ �����:
// 1) ��������� ��������� ������ - "0"
// 2) ����  �� ����� "SET" ��� ��������� �������, �� ����� �������� "0"
//	� ���� ������� ������������ ��� ��� � ������������ � ����� �������
// 3) ����  �� ����� "SET" ���� �������� ������, �� ����� ����� �������� "0"
//	��� ������������� ��������� ������������� ��������� �������
//	���� ������� �������� �� ������������ ����� "SET" � "0"
// ��� ��������� �������� ����������� �������� � ���������� �������,
// �� ������� �������� �������� ����� ����� � �������� ������ *)
            case FUNCTION_BLOCK_FLOW_TIMER:
                iDiscreteInputState =
                    iFunctionBlockInputCheck(INPUT_SOURCE_FBL_SET((pxFunctionBlockDescriptionWork)),
                                             ((((unsigned short)(ADDRESS_HIGH_FBL_SET((pxFunctionBlockDescriptionWork)))) << 8) |
                                              ((unsigned short)(ADDRESS_LOW_FBL_SET((pxFunctionBlockDescriptionWork))))));
                // ���������� ������ "SET" - ���������������� � ����������?
                if (iDiscreteInputState != -1)
                {
                    // ���������� ������ "SET" - "�������"?
                    if (((INPUT_TYPE_FBL_SET((pxFunctionBlockDescriptionWork))) &&
                            (iDiscreteInputState)) ||
                            (((INPUT_TYPE_FBL_SET((pxFunctionBlockDescriptionWork))) == 0) &&
                             (iDiscreteInputState == 0)))
                    {
                        ucInputSet = 1;
                    }
                    else
                    {
                        ucInputSet = 0;
                    }
                }
                else
                {
                    // ������� ������.
                    pxFunctionBlockWorkingData -> ucDelay = 0;
                    ucFunctionBlockCurrentState = 0;
                    break;
                }

//-----------------------------------------------------------------------------------------------------
                iDiscreteInputState =
                    iFunctionBlockInputCheck(INPUT_SOURCE_FBL_RESET((pxFunctionBlockDescriptionWork)),
                                             ((((unsigned short)(ADDRESS_HIGH_FBL_RESET((pxFunctionBlockDescriptionWork)))) << 8) |
                                              ((unsigned short)(ADDRESS_LOW_FBL_RESET((pxFunctionBlockDescriptionWork))))));
                // ���������� ������ "RESET" - ���������������� � ����������?
                if (iDiscreteInputState != -1)
                {
                    // ���������� ������ "RESET" - "�������"?
                    if (((INPUT_TYPE_FBL_RESET((pxFunctionBlockDescriptionWork))) &&
                            (iDiscreteInputState)) ||
                            (((INPUT_TYPE_FBL_RESET((pxFunctionBlockDescriptionWork))) == 0) &&
                             (iDiscreteInputState == 0)))
                    {
                        ucInputReset = 1;
                    }
                    else
                    {
                        ucInputReset = 0;
                    }
                }
                else
                {
                    ucInputReset = 0;
                }

//-----------------------------------------------------------------------------------------------------
                // ����� ��� �������?
                switch(TIMER_TYPE_FBL((pxFunctionBlockDescriptionWork)))
                {
// (*** ��� 0 - �������� ��������� ***
// 1) SET
// ����� ��������������� � 1 ����� ��������� ������� ��������, �����������
// 1 �� ����� SET � ������������ 0 �� ����� SET.
// ���� 1 �� ����� SET ������ T���, �� ����� � 1 �� ���������������
// 2) RESET
// -���������� ������ �������, ���� �� ����
// -���������� ����� � 0, ���� ����� ������� � �� ���������� � 1
// -���� RESET=1 � �� ���� SET �������� 1, ������ �� ����������*)
                case FUNCTION_BLOCK_TIMER_TYPE_ON_DELAY:
                    // ���������� ������ "RESET" - �������?
                    if (ucInputReset)
                    {
                        // ������� ������.
                        pxFunctionBlockWorkingData -> ucDelay = 0;
                        ucFunctionBlockCurrentState = 0;
                        break;
                    }
                    // ���������� ������ "SET" - �������?
                    if (ucInputSet)
                    {
                        // ������ �������?
                        if (pxFunctionBlockWorkingData -> fbFunctionBlockIsActive)
                        {
                            // �� ���������� ���� ����� ��������������� ����� - "������" ��� �������?
                            if (ucFunctionBlockPreviousState)
                            {
                                ucFunctionBlockCurrentState = 1;
                            }
                            else
                            {
                                // ����� �������� �����������������?
                                if (DELAY_TIME_SETPOINT_FBL((pxFunctionBlockDescriptionWork)))
                                {
                                    // ����� �������� �� �������?
                                    if (pxFunctionBlockWorkingData -> ucDelay)
                                    {
                                        // ����� ������������ ������� ��������?
                                        switch(DELAY_TIME_DISCRETENESS_FBL((pxFunctionBlockDescriptionWork)))
                                        {
                                        case FUNCTION_BLOCK_TIMER_DISCRETENESS_100MS:
                                            pxFunctionBlockWorkingData -> ucDelay -= 1;
                                            break;

                                        case FUNCTION_BLOCK_TIMER_DISCRETENESS_1S:
                                            if (fbOneSecondPassed)
                                            {
                                                pxFunctionBlockWorkingData -> ucDelay -= 1;
                                            }
                                            break;

                                        case FUNCTION_BLOCK_TIMER_DISCRETENESS_10S:
                                            if (fbTenSecondsPassed)
                                            {
                                                pxFunctionBlockWorkingData -> ucDelay -= 1;
                                            }
                                            break;

                                        default:
                                            break;
                                        };
                                    }
                                    else
                                    {
                                        ucFunctionBlockCurrentState = 1;
                                    }
                                }
                                else
                                {
                                    ucFunctionBlockCurrentState = 1;
                                }
                            }
                        }
                        else
                        {
                            // ��������� ����� �������� �������.
                            pxFunctionBlockWorkingData -> ucDelay =
                                DELAY_TIME_SETPOINT_FBL((pxFunctionBlockDescriptionWork));
                            // �������� ������.
                            pxFunctionBlockWorkingData -> fbFunctionBlockIsActive = 1;
                            ucFunctionBlockCurrentState = 0;
                        }
                    }
                    else
                    {
                        // ������� ������.
                        pxFunctionBlockWorkingData -> ucDelay = 0;
                        // ��������� ������.
                        pxFunctionBlockWorkingData -> fbFunctionBlockIsActive = 0;
                        ucFunctionBlockCurrentState = 0;
                    }


                    break;

//-----------------------------------------------------------------------------------------------------
                case FUNCTION_BLOCK_TIMER_TYPE_RESERVED:
                    break;

//-----------------------------------------------------------------------------------------------------
// (*** ��� 2 - �������� ���������� ***
// 1) SET
// ����� ��������������� � 1 ����� ��� ����������� 1 �� ���� SET
// � ��������� � 0 ����� ��������� ������� ��������, ����������� ��������� SET � 0
// ���� ���� SET ������� � 0 � ����� ����� ������ T��� �������� � 1, ��
// ������ ������������ � ����� �������� � 1
// 2) RESET
// -���������� ������ �������, ���� �� ���� � ���������� ��� ���� ����� � 0
// -���� ������ ������� �� �������, �������� �� ���������*)
                case FUNCTION_BLOCK_TIMER_TYPE_OFF_DELAY:
                    // ���������� ������ "RESET" - �������?
                    if (ucInputReset)
                    {
                        // ������ �������?
                        if (pxFunctionBlockWorkingData -> fbFunctionBlockIsActive)
                        {
                            // ������� ������.
                            pxFunctionBlockWorkingData -> ucDelay = 0;
                            // ��������� ������.
                            pxFunctionBlockWorkingData -> fbFunctionBlockIsActive = 0;
                            ucFunctionBlockCurrentState = 0;
                            break;
                        }
                    }
                    // ���������� ������ "SET" - �������?
                    if (ucInputSet)
                    {
                        // ������� ������.
                        pxFunctionBlockWorkingData -> ucDelay = 0;
                        // ��������� ������.
                        pxFunctionBlockWorkingData -> fbFunctionBlockIsActive = 0;
                        ucFunctionBlockCurrentState = 1;
                    }
                    else
                    {
                        // �� ���������� ���� ����� ��������������� ����� - "������" ��� �������?
                        if (ucFunctionBlockPreviousState)
                        {
                            ucFunctionBlockCurrentState = 1;
                            // ������ �������?
                            if (pxFunctionBlockWorkingData -> fbFunctionBlockIsActive)
                            {
                                // ����� �������� �����������������?
                                if (DELAY_TIME_SETPOINT_FBL((pxFunctionBlockDescriptionWork)))
                                {
                                    // ����� �������� �� �������?
                                    if (pxFunctionBlockWorkingData -> ucDelay)
                                    {
                                        // ����� ������������ ������� ��������?
                                        switch(DELAY_TIME_DISCRETENESS_FBL((pxFunctionBlockDescriptionWork)))
                                        {
                                        case FUNCTION_BLOCK_TIMER_DISCRETENESS_100MS:
                                            pxFunctionBlockWorkingData -> ucDelay -= 1;
                                            break;

                                        case FUNCTION_BLOCK_TIMER_DISCRETENESS_1S:
                                            if (fbOneSecondPassed)
                                            {
                                                pxFunctionBlockWorkingData -> ucDelay -= 1;
                                            }
                                            break;

                                        case FUNCTION_BLOCK_TIMER_DISCRETENESS_10S:
                                            if (fbTenSecondsPassed)
                                            {
                                                pxFunctionBlockWorkingData -> ucDelay -= 1;
                                            }
                                            break;

                                        default:
                                            break;
                                        };
                                    }
                                    else
                                    {
                                        ucFunctionBlockCurrentState = 0;
                                    }
                                }
                                else
                                {
                                    ucFunctionBlockCurrentState = 0;
                                }
                            }
                            else
                            {
                                // ��������� ����� �������� �������.
                                pxFunctionBlockWorkingData -> ucDelay =
                                    DELAY_TIME_SETPOINT_FBL((pxFunctionBlockDescriptionWork));
                                // �������� ������.
                                pxFunctionBlockWorkingData -> fbFunctionBlockIsActive = 1;
                            }
                        }
                        else
                        {
                            // ������� ������.
                            pxFunctionBlockWorkingData -> ucDelay = 0;
                            // ��������� ������.
                            pxFunctionBlockWorkingData -> fbFunctionBlockIsActive = 0;
                            ucFunctionBlockCurrentState = 0;
                        }
                    }
                    break;

//-----------------------------------------------------------------------------------------------------
// (*** ��� 3 - �������� ������� ***
// 1) SET
// ����� ��������������� � 1 ����� ��� ����������� 1 �� ���� SET
// � �������� ����� �����������.
// ����� ��������� � 0 ����� ��������� ������� ��������.
// ���� 1 �� ����� SET ������ T���, �� ����� ��������� � 0
// � ��������� SET � 0
// 2) RESET
// -���������� ������ ������� � ����� � 0, ���� �� ����
// -���� RESET=1 � �� ���� SET �������� 1, ������ �� ����������*)
                case FUNCTION_BLOCK_TIMER_TYPE_SHORT_PULSE:
                    // ���������� ������ "RESET" - �������?
                    if (ucInputReset)
                    {
                        // ������� ������.
                        pxFunctionBlockWorkingData -> ucDelay = 0;
                        ucFunctionBlockCurrentState = 0;
                        break;
                    }
                    // ���������� ������ "SET" - �������?
                    if (ucInputSet)
                    {
                        ucFunctionBlockCurrentState = 1;
                        // ������ �������?
                        if (pxFunctionBlockWorkingData -> fbFunctionBlockIsActive)
                        {
                            // ����� �������� �����������������?
                            if (DELAY_TIME_SETPOINT_FBL((pxFunctionBlockDescriptionWork)))
                            {
                                // ����� �������� �� �������?
                                if (pxFunctionBlockWorkingData -> ucDelay)
                                {
                                    // ����� ������������ ������� ��������?
                                    switch(DELAY_TIME_DISCRETENESS_FBL((pxFunctionBlockDescriptionWork)))
                                    {
                                    case FUNCTION_BLOCK_TIMER_DISCRETENESS_100MS:
                                        pxFunctionBlockWorkingData -> ucDelay -= 1;
                                        break;

                                    case FUNCTION_BLOCK_TIMER_DISCRETENESS_1S:
                                        if (fbOneSecondPassed)
                                        {
                                            pxFunctionBlockWorkingData -> ucDelay -= 1;
                                        }
                                        break;

                                    case FUNCTION_BLOCK_TIMER_DISCRETENESS_10S:
                                        if (fbTenSecondsPassed)
                                        {
                                            pxFunctionBlockWorkingData -> ucDelay -= 1;
                                        }
                                        break;

                                    default:
                                        break;
                                    };
                                }
                                else
                                {
                                    ucFunctionBlockCurrentState = 0;
                                }
                            }
                            else
                            {
                                ucFunctionBlockCurrentState = 0;
                            }
                        }
                        else
                        {
                            // ��������� ����� �������� �������.
                            pxFunctionBlockWorkingData -> ucDelay =
                                DELAY_TIME_SETPOINT_FBL((pxFunctionBlockDescriptionWork));
                            // �������� ������.
                            pxFunctionBlockWorkingData -> fbFunctionBlockIsActive = 1;
                            ucFunctionBlockCurrentState = 1;
                        }
                    }
                    else
                    {
                        // ������� ������.
                        pxFunctionBlockWorkingData -> ucDelay = 0;
                        // ��������� ������.
                        pxFunctionBlockWorkingData -> fbFunctionBlockIsActive = 0;
                        ucFunctionBlockCurrentState = 0;
                    }
                    break;

//-----------------------------------------------------------------------------------------------------
// (*** ��� 4 - ���������� ������� ***
// 1) SET
// ����� ��������������� � 1 ����� ��� ����������� 1 �� ���� SET
// � �������� ����� �����������. ����� ��������� � 0 ����� ���������
// ������� �������� ���������� �� ������������ �������� �������.
// ���� SET ������� � 0 � ����� ����� ������ T��� �������� � 1, �� �����
// �������� 1 � ������ ���������� �������.
// 2) RESET
// -���������� ������ ������� � ����� � 0, ���� �� ����
// -���� RESET=1 � �� ���� SET �������� 1, ������ �� ����������*)
                case FUNCTION_BLOCK_TIMER_TYPE_LONG_PULSE:
                    // ���������� ������ "RESET" - �������?
                    if (ucInputReset)
                    {
                        // ������� ������.
                        pxFunctionBlockWorkingData -> ucDelay = 0;
                        ucFunctionBlockCurrentState = 0;
                        break;
                    }
                    // ���������� ������ "SET" - �������?
                    if (ucInputSet)
                    {
                        ucFunctionBlockCurrentState = 1;
                        // ������ �������?
                        if (pxFunctionBlockWorkingData -> fbFunctionBlockIsActive)
                        {
                            // ����� �������� �����������������?
                            if (DELAY_TIME_SETPOINT_FBL((pxFunctionBlockDescriptionWork)))
                            {
                                // ����� �������� �� �������?
                                if (pxFunctionBlockWorkingData -> ucDelay)
                                {
                                    // ����� ������������ ������� ��������?
                                    switch(DELAY_TIME_DISCRETENESS_FBL((pxFunctionBlockDescriptionWork)))
                                    {
                                    case FUNCTION_BLOCK_TIMER_DISCRETENESS_100MS:
                                        pxFunctionBlockWorkingData -> ucDelay -= 1;
                                        break;

                                    case FUNCTION_BLOCK_TIMER_DISCRETENESS_1S:
                                        if (fbOneSecondPassed)
                                        {
                                            pxFunctionBlockWorkingData -> ucDelay -= 1;
                                        }
                                        break;

                                    case FUNCTION_BLOCK_TIMER_DISCRETENESS_10S:
                                        if (fbTenSecondsPassed)
                                        {
                                            pxFunctionBlockWorkingData -> ucDelay -= 1;
                                        }
                                        break;

                                    default:
                                        break;
                                    };
                                }
                                else
                                {
                                    ucFunctionBlockCurrentState = 0;
                                }
                            }
                            else
                            {
                                ucFunctionBlockCurrentState = 0;
                            }
                        }
                        else
                        {
                            // ��������� ����� �������� �������.
                            pxFunctionBlockWorkingData -> ucDelay =
                                DELAY_TIME_SETPOINT_FBL((pxFunctionBlockDescriptionWork));
                            // �������� ������.
                            pxFunctionBlockWorkingData -> fbFunctionBlockIsActive = 1;
                            ucFunctionBlockCurrentState = 1;
                        }
                    }
                    else
                    {
                        ucFunctionBlockCurrentState = 1;
                        // ��������� ������.
                        pxFunctionBlockWorkingData -> fbFunctionBlockIsActive = 0;
                        // ����� �������� �����������������?
                        if (DELAY_TIME_SETPOINT_FBL((pxFunctionBlockDescriptionWork)))
                        {
                            // ����� �������� �� �������?
                            if (pxFunctionBlockWorkingData -> ucDelay)
                            {
                                // ����� ������������ ������� ��������?
                                switch(DELAY_TIME_DISCRETENESS_FBL((pxFunctionBlockDescriptionWork)))
                                {
                                case FUNCTION_BLOCK_TIMER_DISCRETENESS_100MS:
                                    pxFunctionBlockWorkingData -> ucDelay -= 1;
                                    break;

                                case FUNCTION_BLOCK_TIMER_DISCRETENESS_1S:
                                    if (fbOneSecondPassed)
                                    {
                                        pxFunctionBlockWorkingData -> ucDelay -= 1;
                                    }
                                    break;

                                case FUNCTION_BLOCK_TIMER_DISCRETENESS_10S:
                                    if (fbTenSecondsPassed)
                                    {
                                        pxFunctionBlockWorkingData -> ucDelay -= 1;
                                    }
                                    break;

                                default:
                                    break;
                                };
                            }
                            else
                            {
                                ucFunctionBlockCurrentState = 0;
                            }
                        }
                        else
                        {
                            ucFunctionBlockCurrentState = 0;
                        }
                    }
                    break;

                default:
                    break;
                };
                // �������� ��������� ��������������� ����� �� ������� ����.
                *pucFunctionBlockOuts = ucFunctionBlockCurrentState;
                break;

//-----------------------------------------------------------------------------------------------------
// �������������� ���� "�������".
// ( *  "�������"
// 		��� ��������  D4=0 - R-S ��������� ������
// 					  D4=1 - S-R ��������� ���������*)
// ( *��������� ������ SET/RESET ��� ��� "�������"
// ����� 1,2 - SET �� ����� "���", ����� 3,4 - RESET �� ����� "���"
// ���� ��� ����� SET ��� ��� ����� RESET ������������ ���
// �� �����������������, �� �� ������ ������ RESET*)
            case FUNCTION_BLOCK_FLOW_TRIGGER:
                // ��������� ��������� - ���� ���������.
                ucInputSet = 0;
                fbInputSetIsReliable = 0;

                iDiscreteInputState =
                    iFunctionBlockInputCheck(INPUT_SOURCE_FBL_SET_1((pxFunctionBlockDescriptionWork)),
                                             ((((unsigned short)(ADDRESS_HIGH_FBL_SET_1((pxFunctionBlockDescriptionWork)))) << 8) |
                                              ((unsigned short)(ADDRESS_LOW_FBL_SET_1((pxFunctionBlockDescriptionWork))))));
                // ���������� ������ "SET1" - ���������������� � ����������?
                if (iDiscreteInputState != -1)
                {
                    fbInputSetIsReliable = 1;
                    // ���������� ������ "SET1" - "�������"?
                    if (((INPUT_TYPE_FBL_SET_1((pxFunctionBlockDescriptionWork))) &&
                            (iDiscreteInputState)) ||
                            (((INPUT_TYPE_FBL_SET_1((pxFunctionBlockDescriptionWork))) == 0) &&
                             (iDiscreteInputState == 0)))
                    {
                        ucInputSet = 1;
                    }
                    else
                    {

                    }
                }
                else
                {

                }

//-----------------------------------------------------------------------------------------------------
                iDiscreteInputState =
                    iFunctionBlockInputCheck(INPUT_SOURCE_FBL_SET_2((pxFunctionBlockDescriptionWork)),
                                             ((((unsigned short)(ADDRESS_HIGH_FBL_SET_2((pxFunctionBlockDescriptionWork)))) << 8) |
                                              ((unsigned short)(ADDRESS_LOW_FBL_SET_2((pxFunctionBlockDescriptionWork))))));
                // ���������� ������ "SET2" - ���������������� � ����������?
                if (iDiscreteInputState != -1)
                {
                    fbInputSetIsReliable = 1;
                    // ���������� ������ "SET2" - "�������"?
                    if (((INPUT_TYPE_FBL_SET_2((pxFunctionBlockDescriptionWork))) &&
                            (iDiscreteInputState)) ||
                            (((INPUT_TYPE_FBL_SET_2((pxFunctionBlockDescriptionWork))) == 0) &&
                             (iDiscreteInputState == 0)))
                    {
                        ucInputSet = 1;
                    }
                    else
                    {

                    }
                }
                else
                {

                }

//-----------------------------------------------------------------------------------------------------
                // ��������� ��������� - ���� ���������.
                ucInputReset = 0;
                fbInputResetIsReliable = 0;

                iDiscreteInputState =
                    iFunctionBlockInputCheck(INPUT_SOURCE_FBL_RESET_1((pxFunctionBlockDescriptionWork)),
                                             ((((unsigned short)(ADDRESS_HIGH_FBL_RESET_1((pxFunctionBlockDescriptionWork)))) << 8) |
                                              ((unsigned short)(ADDRESS_LOW_FBL_RESET_1((pxFunctionBlockDescriptionWork))))));
                // ���������� ������ "RESET1" - ���������������� � ����������?
                if (iDiscreteInputState != -1)
                {
                    fbInputResetIsReliable = 1;
                    // ���������� ������ "RESET1" - "�������"?
                    if (((INPUT_TYPE_FBL_RESET_1((pxFunctionBlockDescriptionWork))) &&
                            (iDiscreteInputState)) ||
                            (((INPUT_TYPE_FBL_RESET_1((pxFunctionBlockDescriptionWork))) == 0) &&
                             (iDiscreteInputState == 0)))
                    {
                        ucInputReset = 1;
                    }
                    else
                    {

                    }
                }
                else
                {

                }

//-----------------------------------------------------------------------------------------------------
                iDiscreteInputState =
                    iFunctionBlockInputCheck(INPUT_SOURCE_FBL_RESET_2((pxFunctionBlockDescriptionWork)),
                                             ((((unsigned short)(ADDRESS_HIGH_FBL_RESET_2((pxFunctionBlockDescriptionWork)))) << 8) |
                                              ((unsigned short)(ADDRESS_LOW_FBL_RESET_2((pxFunctionBlockDescriptionWork))))));
                // ���������� ������ "RESET2" - ���������������� � ����������?
                if (iDiscreteInputState != -1)
                {
                    fbInputResetIsReliable = 1;
                    // ���������� ������ "RESET2" - "�������"?
                    if (((INPUT_TYPE_FBL_RESET_2((pxFunctionBlockDescriptionWork))) &&
                            (iDiscreteInputState)) ||
                            (((INPUT_TYPE_FBL_RESET_2((pxFunctionBlockDescriptionWork))) == 0) &&
                             (iDiscreteInputState == 0)))
                    {
                        ucInputReset = 1;
                    }
                    else
                    {

                    }
                }
                else
                {

                }

//-----------------------------------------------------------------------------------------------------
                // ��� ����� SET ��� ��� ����� RESET ������������ ��� �� �����������������?
                if ((fbInputSetIsReliable == 0) ||
                        (fbInputResetIsReliable == 0))
                {
                    // RESET.
                    ucInputSet = 0;
                    ucInputReset = 1;
                }
                // ��� ����� ���������?
                if ((ucInputSet == 0) &&
                        (ucInputReset == 0))
                {
                    // ��������� ���������� ���������.
                    // �� ���������� ���� ����� ��������������� ����� ��� �������?
                    if (ucFunctionBlockPreviousState)
                    {
                        ucFunctionBlockCurrentState = 1;
                    }
                    else
                    {
                        ucFunctionBlockCurrentState = 0;
                    }
                    break;
                }
                // ����� ��� ��������?
                switch(TRIGGER_TYPE_FBL((pxFunctionBlockDescriptionWork)))
                {
                case FUNCTION_BLOCK_TRIGGER_TYPE_R_S:
                    if (ucInputReset)
                    {
                        ucFunctionBlockCurrentState = 0;
                    }
                    else
                    {
                        ucFunctionBlockCurrentState = 1;
                    }
                    break;

                case FUNCTION_BLOCK_TRIGGER_TYPE_S_R:
                    if (ucInputSet)
                    {
                        ucFunctionBlockCurrentState = 1;
                    }
                    else
                    {
                        ucFunctionBlockCurrentState = 0;
                    }
                    break;

                default:
                    break;
                };
                // �������� ��������� ��������������� ����� �� ������� ����.
                *pucFunctionBlockOuts = ucFunctionBlockCurrentState;
                break;

//-----------------------------------------------------------------------------------------------------
// �������������� ���� "���������� ����������".
// (*  "���������� ����������"
//		Inp_Addr [1] - D6-D0-����� ����������� ������� , D7=0  ������ �� ������� ���������� ���������
//						D7 = 1 ������ �� ������� ��������� �������
//		Inp_Addr [6] - D7 = 0 ��������� �� ������, D7 = 1 ��������� �� ������, D6-D2  ���������� 0-31%
//		Inp_Addr [7], Inp_Addr [8]  - ����� ����� ����������� ������� ����������
//		Inp_Addr [2] - Inp_Addr [5] - ������� � ������� IEEE 754 -  ��� �� �����. ���. ��������� !!!
//		��������� ��� � � ���� ��� *)
// (*����������� ��������� ����������� ������� ����������
// ������� �������� �������: ���� �� ���������, �� ���������, ��������� "0"
// ���� ������� ������ � ����� � �������, ��� �� ����������������, ��� ������������ - ��� ��������
// ���� ������� ������ � ����� � ��������� - ��� �� ��������*)
            case FUNCTION_BLOCK_FLOW_ANALOGUE_COMPARATOR:
                iDiscreteInputState =
                    iFunctionBlockInputCheck(DISCRETE_INPUT_ENABLE_SOURCE_FBL((pxFunctionBlockDescriptionWork)),
                                             ((((unsigned short)(DISCRETE_INPUT_ENABLE_ADDRESS_HIGH_FBL((pxFunctionBlockDescriptionWork)))) << 8) |
                                              ((unsigned short)(DISCRETE_INPUT_ENABLE_ADDRESS_LOW_FBL((pxFunctionBlockDescriptionWork))))));
                // ���������� ������ "ENABLE" - ���������������� � ����������?
                if (iDiscreteInputState != -1)
                {
                    fbInputIsReliable = 1;
                    // ���������� ������ "ENABLE" - "�������"?
                    if (((DISCRETE_INPUT_ENABLE_TYPE_FBL((pxFunctionBlockDescriptionWork))) &&
                            (iDiscreteInputState)) ||
                            (((DISCRETE_INPUT_ENABLE_TYPE_FBL((pxFunctionBlockDescriptionWork))) == 0) &&
                             (iDiscreteInputState == 0)))
                    {
                        // ���������� ������ "ENABLE" - "�������" - �������������� ���� ��������.
                    }
                    else
                    {
                        // ���������� ������ "ENABLE" - "���������" - �������������� ���� �� ��������.
                        ucFunctionBlockCurrentState = 0;
                        break;
                    }
                }
                else
                {
                    fbInputIsReliable = 0;
                }
                // ������� ���������� ������ �� ����������������?
                if (!(ANALOGUE_INPUT_FBL_ADDRESS((pxFunctionBlockDescriptionWork))))
                {
                    ucFunctionBlockCurrentState = 0;
                    break;
                }
                // ������� ������ �� ������� ��������� �������?
                if (ANALOGUE_INPUT_FBL_TYPE_FBL((pxFunctionBlockDescriptionWork)))
                {
                    // ������� ������ �� ������� ��������� �������.
                    // ������� �������� ���������� �������� FLOAT.
                    fAnalogueData =
                        pfCalculatedValueSource[ANALOGUE_INPUT_FBL_ADDRESS((pxFunctionBlockDescriptionWork))];
                }
                else
                {
                    // ������� ������ �� ������� ���������� ���������.
                    // ������� �������� ���������� �������� FLOAT.
                    fAnalogueData =
                        pfAnalogueInputSource[ANALOGUE_INPUT_FBL_ADDRESS((pxFunctionBlockDescriptionWork))];
                }
                // ������� �������� ������� FLOAT.
                fSetPoint =
                    SETPOINT_FBL((pxFunctionBlockDescriptionWork));
                // ������� �������� ����� ��������� ���������� �������� � ��������� �������.
                fDifference = fAnalogueData - fSetPoint;
                // ������� ��������?
                // (�������� ���������� �������� ������ ������� ��� ������� ���������
                // (��� cond = 0 ����� = 1, ���� ���������� ������ ������ �������)? ���
                // �������� ���������� �������� ������ ������� ��� ������� ���������
                // (��� cond = 1 ����� = 1, ���� ���������� ������ ������ �������)?)
                if (((fDifference < 0) &&
                        ((COMPARISON_CONDITION_FBL((pxFunctionBlockDescriptionWork))) == 0)) ||
                        ((fDifference >= 0) &&
                         (COMPARISON_CONDITION_FBL((pxFunctionBlockDescriptionWork)))))
                {
                    ucFunctionBlockCurrentState = 1;
                }
                else
                {
                    // ������� ������ �� ������� ��������� �������? ���
                    // �� ���������� ���� ��������� �� ����? ���
                    // ���������� �� ����������������?
                    if ((ANALOGUE_INPUT_FBL_TYPE_FBL((pxFunctionBlockDescriptionWork))) ||
                            (!(ucFunctionBlockPreviousState)) ||
                            (HYSTERESIS_ANALOGUE_SCALE_RANGE_FBL((pxFunctionBlockDescriptionWork)) == 0))
                    {
                        ucFunctionBlockCurrentState = 0;
                        break;
                    }
                    else
                    {
                        // ��������� �������� ����������� - ���������� ������ ���� ����������� � �������� �����.
                        // ������� �������� ��� - ������ ������ ����� �������, �� ���������
                        // ����������� ����� ���� ������.
                        fMinOfScale =
                            pxAnalogueInputDescriptionWork[ANALOGUE_INPUT_FBL_ADDRESS((pxFunctionBlockDescriptionWork))].
                            fMinOfScale;
                        // ������� �������� ��� - ������� ������ ����� �������, �� ���������
                        // ����������� ����� ���� ������.
                        fMaxOfScale =
                            pxAnalogueInputDescriptionWork[ANALOGUE_INPUT_FBL_ADDRESS((pxFunctionBlockDescriptionWork))].
                            fMaxOfScale;
                        // ����������� �������� ����������� �� (*���� ����������� � % ����� 0-31% FLOAT*) �
                        // (*���� ����������� � �������� �����*).
                        fHysteresisScale =
                            (float)(((fMaxOfScale - fMinOfScale) / 100) *
                                    HYSTERESIS_ANALOGUE_SCALE_RANGE_FBL((pxFunctionBlockDescriptionWork)));
                        // �������� ���������� �������� ������ �������?
                        if (fDifference < 0)
                        {
                            // �������� ���������� �������� ������ �������.
                            // �������� ������ ���� �����������.
                            // ������� �������� ����� ��������� ����������� � ���������
                            // ����� ���������� ��������� � ��������
                            // (���������� + (�������� ���������� �������� - �������)).
                            // ���� �������� �������������, ������ ���������� �������� � ���� �����������.
                            fHysteresisDifference = fHysteresisScale + fDifference;
                        }
                        else
                        {
                            // �������� ���������� �������� ������ �������.
                            // �������� ������� ���� �����������.
                            // ������� �������� ����� ��������� ����������� � ���������
                            // ����� ���������� ��������� � ��������
                            // (���������� - (�������� ���������� �������� - �������)).
                            // ���� �������� �������������, ������ ���������� �������� � ���� �����������.
                            fHysteresisDifference = fHysteresisScale - fDifference;
                        }
                        // ��� � ���� �����������?
                        if (fHysteresisDifference >= 0)
                        {
                            // ��� � ���� �����������, ��������� �����������.
                            ucFunctionBlockCurrentState = 1;
                            break;
                        }
                        else
                        {
                            // ����� �� ���� �����������, ��������� ���.
                            ucFunctionBlockCurrentState = 0;
                            break;
                        }
                    }
                }
                // �������� ��������� ��������������� ����� �� ������� ����.
                *pucFunctionBlockOuts = ucFunctionBlockCurrentState;
                break;

            default:
                break;
            };
        };


//-----------------------------------------------------------------------------------------------------
        // �������������� ���� �������?
        if (ucFunctionBlockCurrentState)
        {
            // �������������� ���� ��� ������� �� ���������� ����?
            if (ucFunctionBlockPreviousState)
            {
                // �� ����� ���������.
                // ��������� ����� - ���������� ���������, ��� ������������������� ����,
                // � ������ ������� ���������� ��������� ���� - ����������.
                // ���� ������ ������.
                for (int j = 0;
                        j < (FUNCTION_BLOCK_RELAY_OUTPUT_QUANTITY /
                             MRXX_DISCRETE_OUTPUT_QUANTITY);
                        j++)
                {
                    // ���� ������ ����.
                    for (int k = 0;
                            k < (MRXX_DISCRETE_OUTPUT_QUANTITY);
                            k++)
                    {
                        // ��������� ���� - ���������� ��������� ����.
                        pucDoValueBlockDestination[(j * MRXX_DISCRETE_OUTPUT_QUANTITY) + k] |=
                            (((pxFunctionBlockDescriptionWork -> aucRelayOut[j]) >> k) & 0x01);
                    }
                }
            }
            else
            {
                // ����� ���������.
                // ��������� ����� - ����� ���������, ��� ������������������� ����
                // � ������ ������� ���������� ���� - ����� ���������.
                // ���� ������ ������.
                for (int j = 0;
                        j < (FUNCTION_BLOCK_RELAY_OUTPUT_QUANTITY /
                             MRXX_DISCRETE_OUTPUT_QUANTITY);
                        j++)
                {
                    // ���� ������ ����.
                    for (int k = 0;
                            k < (MRXX_DISCRETE_OUTPUT_QUANTITY);
                            k++)
                    {
                        // ��������� ���� - ����� ���������.
                        pucDoValueDestination[(j * MRXX_DISCRETE_OUTPUT_QUANTITY) + k] |=
                            (((pxFunctionBlockDescriptionWork -> aucRelayOut[j]) >> k) & 0x01);
                    }
                }
                // ��������� ����� - ���������� ���������, ��� ������������������� ����,
                // � ������ ������� ���������� ��������� ���� - ����������.
                // ���� ������ ������.
                for (int j = 0;
                        j < (FUNCTION_BLOCK_RELAY_OUTPUT_QUANTITY /
                             MRXX_DISCRETE_OUTPUT_QUANTITY);
                        j++)
                {
                    // ���� ������ ����.
                    for (int k = 0;
                            k < (MRXX_DISCRETE_OUTPUT_QUANTITY);
                            k++)
                    {
                        // ��������� ���� - ���������� ��������� ����.
                        pucDoValueBlockDestination[(j * MRXX_DISCRETE_OUTPUT_QUANTITY) + k] |=
                            (((pxFunctionBlockDescriptionWork -> aucRelayOut[j]) >> k) & 0x01);
                    }
                }
            }

//            // ������������?
//            if (ARCHIVE_FBL((pxFunctionBlockDescriptionWork)))
//            {
//                // arch
//                pxAlarmHmi -> ui8Archive = 1;
//            }
//            else
//            {
//                pxAlarmHmi -> ui8Archive = 0;
//            }

//            // ��������� ��� ����� ������������.
//            // ��� ��������� ������������?
//            if (!((ucAlarmStateCommon >> ALARM_TYPE_ALARM) & 0x01))
//            {
//                // ��� �������� ��������������� ����� ����������������� ������������ ��� ���������?
//                if ((SIGNALING_TYPE_2_FBL((pxFunctionBlockDescriptionWork))) ||
//                        (SIGNALING_TYPE_1_FBL((pxFunctionBlockDescriptionWork))))
//                {
//                    // ��� �������� ��������������� ����� ����������������� ������������?
//                    if (((!SIGNALING_TYPE_2_FBL((pxFunctionBlockDescriptionWork))) &&
//                            (SIGNALING_TYPE_1_FBL((pxFunctionBlockDescriptionWork)))) ||
//                            ((SIGNALING_TYPE_2_FBL((pxFunctionBlockDescriptionWork))) &&
//                             (!SIGNALING_TYPE_1_FBL((pxFunctionBlockDescriptionWork)))))
//                    {
//                        // ������� ����� ����������� ��������� ������������.
//                        ucAlarmStateCommon = 0;
//                        // ��� �������� ��������������� ����� ����������������� ����������������� ������������?
//                        if (SIGNALING_TYPE_1_FBL((pxFunctionBlockDescriptionWork)))
//                        {
//                            // ��������� ���� ����������������� ������������.
//                            ucAlarmStateCommon |= (0x01 << ALARM_TYPE_PREVENT);
//                        }
//                        else
//                        {
//                            // ������� ���� ����������������� ������������.
//                            ucAlarmStateCommon &= ~(0x01 << ALARM_TYPE_PREVENT);
//                        }
//                        // ��� �������� ��������������� ����� ����������������� ��������� ������������?
//                        if (SIGNALING_TYPE_2_FBL((pxFunctionBlockDescriptionWork)))
//                        {
//                            // ��������� ���� ��������� ������������.
//                            ucAlarmStateCommon |= (0x01 << ALARM_TYPE_ALARM);
//                        }
//                        else
//                        {
//                            // ������� ���� ��������� ������������.
//                            ucAlarmStateCommon &= ~(0x01 << ALARM_TYPE_ALARM);
//                        }
//                    }
//                }
//            }
//
//            // ������� ����� ����������� ��������� ������������.
//            pxAlarmHmi -> ui8AlarmState = 0;
//            // ��������� ��� ������������ ��� �������� ��������������� �����.
//            // ��� �������� ��������������� ����� ����������������� ����������������� ������������?
//            if (SIGNALING_TYPE_1_FBL((pxFunctionBlockDescriptionWork)))
//            {
//                // ��������� ���� ����������������� ������������.
//                pxAlarmHmi -> ui8AlarmState |= (0x01 << ALARM_TYPE_PREVENT);
//            }
//            else
//            {
//                // ������� ���� ����������������� ������������.
//                pxAlarmHmi -> ui8AlarmState &= ~(0x01 << ALARM_TYPE_PREVENT);
//            }
//            // ��� �������� ��������������� ����� ����������������� ��������� ������������?
//            if (SIGNALING_TYPE_2_FBL((pxFunctionBlockDescriptionWork)))
//            {
//                // ��������� ���� ��������� ������������.
//                pxAlarmHmi -> ui8AlarmState |= (0x01 << ALARM_TYPE_ALARM);
//            }
//            else
//            {
//                // ������� ���� ��������� ������������.
//                pxAlarmHmi -> ui8AlarmState &= ~(0x01 << ALARM_TYPE_ALARM);
//            }

//
//            // ��� �������� ��������������� ����� ����������������� ������������ ��� ���������?
//            if ((SIGNALING_TYPE_2_FBL((pxFunctionBlockDescriptionWork))) ||
//                    (SIGNALING_TYPE_1_FBL((pxFunctionBlockDescriptionWork))))
//            {
//                // ��� �������� ��������������� ����� ����������������� ������������?
//                if (((!SIGNALING_TYPE_2_FBL((pxFunctionBlockDescriptionWork))) &&
//                        (SIGNALING_TYPE_1_FBL((pxFunctionBlockDescriptionWork)))) ||
//                        ((SIGNALING_TYPE_2_FBL((pxFunctionBlockDescriptionWork))) &&
//                         (!SIGNALING_TYPE_1_FBL((pxFunctionBlockDescriptionWork)))))
//                {
//                    // ��� �������� ��������������� ����� ����������������� ����������������� ������������?
//                    if (SIGNALING_TYPE_1_FBL((pxFunctionBlockDescriptionWork)))
//                    {
//                        // ��������� ���� ����������������� ������������.
//                        pxAlarmHmi -> ui8AlarmState |= (0x01 << ALARM_TYPE_PREVENT);
//                    }
//                    else
//                    {
//                        // ������� ���� ����������������� ������������.
//                        pxAlarmHmi -> ui8AlarmState &= ~(0x01 << ALARM_TYPE_PREVENT);
//                    }
//                    // ��� �������� ��������������� ����� ����������������� ��������� ������������?
//                    if (SIGNALING_TYPE_2_FBL((pxFunctionBlockDescriptionWork)))
//                    {
//                        // ��������� ���� ��������� ������������.
//                        pxAlarmHmi -> ui8AlarmState |= (0x01 << ALARM_TYPE_ALARM);
//                    }
//                    else
//                    {
//                        // ������� ���� ��������� ������������.
//                        pxAlarmHmi -> ui8AlarmState &= ~(0x01 << ALARM_TYPE_ALARM);
//                    }
//                }
//            }
        }
        else
        {
            // �������������� ���� ��� ������� �� ���������� ����?
            if (ucFunctionBlockPreviousState)
            {
//                // ������������?
//                if (ARCHIVE_FBL((pxFunctionBlockDescriptionWork)))
//                {
//                    // arch
//                    pxAlarmHmi -> ui8Archive = 1;
//                }
//                else
//                {
//                    pxAlarmHmi -> ui8Archive = 0;
//                }

                if (ARCHIVE_FBL((pxFunctionBlockDescriptionWork)))
                {
                    // ���������� ��������� ������� ��� �� ����������������?
                    if(xCAlarmEvent.EventOffIsNotRegistered(
                        i,
                        0))
                    {
                        // �������������� ���������� ��������� �������.
                        CEvents::EventRegistration(
                            i,
                            (CEvents::HANDLED_EVENTS_FUNCTION_BLOCKS_TYPE |
                        CEvents::HANDLED_EVENTS_IS_ARCHIVE),
                            0,
                            0);
                    }
                }
                else
                {
                    // ���������� ��������� ������� ��� �� ����������������?
                    if(xCAlarmEvent.EventOffIsNotRegistered(
                        i,
                        0))
                    {
                        // �������������� ���������� ��������� �������.
                        CEvents::EventRegistration(
                            i,
                            (CEvents::HANDLED_EVENTS_FUNCTION_BLOCKS_TYPE),
                            0,
                            0);
                    }
                }
            }
        }

//-----------------------------------------------------------------------------------------------------
//        // ������������ �� �����������������? ���, ����������������� ���������?
//        if (((SIGNALING_TYPE_2_FBL((pxFunctionBlockDescriptionWork))) &&
//                (SIGNALING_TYPE_1_FBL((pxFunctionBlockDescriptionWork)))) ||
//                ((!(SIGNALING_TYPE_2_FBL((pxFunctionBlockDescriptionWork)))) &&
//                 (!(SIGNALING_TYPE_1_FBL((pxFunctionBlockDescriptionWork))))))
        // ������������ �� �����������������?
        if ((!(SIGNALING_TYPE_2_FBL((pxFunctionBlockDescriptionWork)))) &&
                (!(SIGNALING_TYPE_1_FBL((pxFunctionBlockDescriptionWork)))))
        {
            // ������� ����� ���������� ����������� ������������ �� HMI - �� � �����.
            *pxAlarmHmi = xAlarmHmiNull;
        }
        else
        {
            // ����������������� ������������.
            // �������������� ���� �������?
            if (ucFunctionBlockCurrentState)
            {
                // ��� �������� ����������� ������� ����������� ���������� ����������� ������������ �� HMI?
                if (pxAlarmHmi -> ui8AlarmState)
                {

                }
                else
                {
                    // ������ ������ ��� ����������� ������������ �� HMI.
                    pxAlarmHmi -> ui8AlarmState =
                        LIGHT_BOARD_CELL_NUMBER_FBL((pxFunctionBlockDescriptionWork));
                    // ��������� ��� ������������ ��� �������� ��������������� �����.
                    // ��� �������� ��������������� ����� ����������������� ����������������� ������������?
                    if (SIGNALING_TYPE_1_FBL((pxFunctionBlockDescriptionWork)))
                    {
                        // ��������� ���� ����������������� ������������.
                        pxAlarmHmi -> ui8AlarmState |= (0x01 << ALARM_TYPE_PREVENT);
                    }
                    else
                    {
                        // ������� ���� ����������������� ������������.
                        pxAlarmHmi -> ui8AlarmState &= ~(0x01 << ALARM_TYPE_PREVENT);
                    }
                    // ��� �������� ��������������� ����� ����������������� ��������� ������������?
                    if (SIGNALING_TYPE_2_FBL((pxFunctionBlockDescriptionWork)))
                    {
                        // ��������� ���� ��������� ������������.
                        pxAlarmHmi -> ui8AlarmState |= (0x01 << ALARM_TYPE_ALARM);
                    }
                    else
                    {
                        // ������� ���� ��������� ������������.
                        pxAlarmHmi -> ui8AlarmState &= ~(0x01 << ALARM_TYPE_ALARM);
                    }

                    if (ARCHIVE_FBL((pxFunctionBlockDescriptionWork)))
                    {
                        // �������� ��������� ������� ��� �� ����������������?
                        if(xCAlarmEvent.EventOnIsNotRegistered(
                                    i,
                                    0))
                        {
                            // �������������� �������� ��������� �������.
                            CEvents::EventRegistration(
                                i,
                                (CEvents::HANDLED_EVENTS_FUNCTION_BLOCKS_TYPE |
                                 CEvents::HANDLED_EVENTS_IS_POPUP |
                                 CEvents::HANDLED_EVENTS_IS_SOUND |
                                 CEvents::HANDLED_EVENTS_IS_ARCHIVE),
                                (pxAlarmHmi -> ui8AlarmState),
                                0);
                        }
                    }
                    else
                    {
                        // �������� ��������� ������� ��� �� ����������������?
                        if(xCAlarmEvent.EventOnIsNotRegistered(
                                    i,
                                    0))
                        {
                            // �������������� �������� ��������� �������.
                            CEvents::EventRegistration(
                                i,
                                (CEvents::HANDLED_EVENTS_FUNCTION_BLOCKS_TYPE |
                                 CEvents::HANDLED_EVENTS_IS_POPUP |
                                 CEvents::HANDLED_EVENTS_IS_SOUND),
                                (pxAlarmHmi -> ui8AlarmState),
                                0);
                        }
                    }
                }
            }
            else
            {
                // ��� �������� ����������� ������� ����������� ���������� ����������� ������������ �� HMI?
                if (pxAlarmHmi -> ui8AlarmState)
                {
                    // ���������� ����������� ������������ �� HMI ��������?
                    if (fbGlobalReset)
                    {
                        // ������� ����� ���������� ����������� ������������ �� HMI - �� � �����.
                        *pxAlarmHmi = xAlarmHmiNull;
                    }
                    else
                    {

                    }
                }
                else
                {

                }
            }
        }
        pucFunctionBlockOuts++;
        pxFunctionBlockDescriptionWork++;
        pxFunctionBlockWorkingData++;
        pxAnalogueInputDescriptionWork++;
        pxAlarmHmi++;

//-----------------------------------------------------------------------------------------------------
    }
}

//-----------------------------------------------------------------------------------------------------
// �������������� ���������� ������� "�".
// �������� ��������� �� ��������� ��������������� ����� � ��������������� ���� ������.
// ���������� ��������� ������ ����������� ��������.
unsigned char ucFourInputElementAnd(TFunctionBlockDescriptionWork *pxSource)
{
    unsigned char ucCurrentState;
    int iDiscreteInputState;
    bool fbInputIsReliable;
    // ��������� ��������� ������ ��������������� �������� "�" - 1.
    ucCurrentState = 1;
    fbInputIsReliable = 0;

    iDiscreteInputState =
        iFunctionBlockInputCheck(INPUT_SOURCE_FBL_1(pxSource),
                                 ((((unsigned short)(ADDRESS_HIGH_FBL_1(pxSource))) << 8) |
                                  ((unsigned short)(ADDRESS_LOW_FBL_1(pxSource)))));
    // ���������� ������ - ���������������� � ����������?
    if (iDiscreteInputState != -1)
    {
        fbInputIsReliable = 1;
        // ���������� ������ - "�������"?
        if (((INPUT_TYPE_FBL_1(pxSource)) &&
                (iDiscreteInputState)) ||
                (((INPUT_TYPE_FBL_1(pxSource)) == 0) &&
                 (iDiscreteInputState == 0)))
        {

        }
        else
        {
            // ���������� ������ - "���������".
            // ��������� ������ ��������������� �������� "�" - 0.
            ucCurrentState = 0;
        }
    }

//-----------------------------------------------------------------------------------------------------
    iDiscreteInputState =
        iFunctionBlockInputCheck(INPUT_SOURCE_FBL_2(pxSource),
                                 ((((unsigned short)(ADDRESS_HIGH_FBL_2(pxSource))) << 8) |
                                  ((unsigned short)(ADDRESS_LOW_FBL_2(pxSource)))));
    // ���������� ������ - ���������������� � ����������?
    if (iDiscreteInputState != -1)
    {
        fbInputIsReliable = 1;
        // ���������� ������ - "�������"?
        if (((INPUT_TYPE_FBL_2(pxSource)) &&
                (iDiscreteInputState)) ||
                (((INPUT_TYPE_FBL_2(pxSource)) == 0) &&
                 (iDiscreteInputState == 0)))
        {

        }
        else
        {
            // ���������� ������ - "���������".
            // ��������� ������ ��������������� �������� "�" - 0.
            ucCurrentState = 0;
        }
    }

//-----------------------------------------------------------------------------------------------------
    iDiscreteInputState =
        iFunctionBlockInputCheck(INPUT_SOURCE_FBL_3(pxSource),
                                 ((((unsigned short)(ADDRESS_HIGH_FBL_3(pxSource))) << 8) |
                                  ((unsigned short)(ADDRESS_LOW_FBL_3(pxSource)))));
    // ���������� ������ - ���������������� � ����������?
    if (iDiscreteInputState != -1)
    {
        fbInputIsReliable = 1;
        // ���������� ������ - "�������"?
        if (((INPUT_TYPE_FBL_3(pxSource)) &&
                (iDiscreteInputState)) ||
                (((INPUT_TYPE_FBL_3(pxSource)) == 0) &&
                 (iDiscreteInputState == 0)))
        {

        }
        else
        {
            // ���������� ������ - "���������".
            // ��������� ������ ��������������� �������� "�" - 0.
            ucCurrentState = 0;
        }
    }

//-----------------------------------------------------------------------------------------------------
    iDiscreteInputState =
        iFunctionBlockInputCheck(INPUT_SOURCE_FBL_4(pxSource),
                                 ((((unsigned short)(ADDRESS_HIGH_FBL_4(pxSource))) << 8) |
                                  ((unsigned short)(ADDRESS_LOW_FBL_4(pxSource)))));
    // ���������� ������ - ���������������� � ����������?
    if (iDiscreteInputState != -1)
    {
        fbInputIsReliable = 1;
        // ���������� ������ - "�������"?
        if (((INPUT_TYPE_FBL_4(pxSource)) &&
                (iDiscreteInputState)) ||
                (((INPUT_TYPE_FBL_4(pxSource)) == 0) &&
                 (iDiscreteInputState == 0)))
        {

        }
        else
        {
            // ���������� ������ - "���������".
            // ��������� ������ ��������������� �������� "�" - 0.
            ucCurrentState = 0;
        }
    }

    // ���������� ������ - ���������������� � ����������?
    if (!(fbInputIsReliable))
    {
        // ���������� ������ - ������������������ ��� ������������.
        // ��������� ������ ��������������� �������� "�" - 0.
        ucCurrentState = 0;
    }
    return ucCurrentState;
}

//-----------------------------------------------------------------------------------------------------
// �������������� ���������� ������� "���".
// �������� ��������� �� ��������� ��������������� ����� � ��������������� ���� ������.
// ���������� ��������� ������ ����������� ��������.
unsigned char ucFourInputElementOr(TFunctionBlockDescriptionWork *pxSource)
{
    unsigned char ucCurrentState;
    int iDiscreteInputState;
    bool fbInputIsReliable;
    // ��������� ��������� ������ ��������������� �������� "���" - 0.
    ucCurrentState = 0;
    fbInputIsReliable = 0;

    iDiscreteInputState =
        iFunctionBlockInputCheck(INPUT_SOURCE_FBL_1(pxSource),
                                 ((((unsigned short)(ADDRESS_HIGH_FBL_1(pxSource))) << 8) |
                                  ((unsigned short)(ADDRESS_LOW_FBL_1(pxSource)))));
    // ���������� ������ - ���������������� � ����������?
    if (iDiscreteInputState != -1)
    {
        fbInputIsReliable = 1;
        // ���������� ������ - "�������"?
        if (((INPUT_TYPE_FBL_1(pxSource)) &&
                (iDiscreteInputState)) ||
                (((INPUT_TYPE_FBL_1(pxSource)) == 0) &&
                 (iDiscreteInputState == 0)))
        {
            // ���������� ������ - "�������".
            // ��������� ������ ��������������� �������� "���" - 1.
            ucCurrentState = 1;
        }
        else
        {

        }
    }

//-----------------------------------------------------------------------------------------------------
    iDiscreteInputState =
        iFunctionBlockInputCheck(INPUT_SOURCE_FBL_2(pxSource),
                                 ((((unsigned short)(ADDRESS_HIGH_FBL_2(pxSource))) << 8) |
                                  ((unsigned short)(ADDRESS_LOW_FBL_2(pxSource)))));
    // ���������� ������ - ���������������� � ����������?
    if (iDiscreteInputState != -1)
    {
        fbInputIsReliable = 1;
        // ���������� ������ - "�������"?
        if (((INPUT_TYPE_FBL_2(pxSource)) &&
                (iDiscreteInputState)) ||
                (((INPUT_TYPE_FBL_2(pxSource)) == 0) &&
                 (iDiscreteInputState == 0)))
        {
            // ���������� ������ - "�������".
            // ��������� ������ ��������������� �������� "���" - 1.
            ucCurrentState = 1;
        }
        else
        {

        }
    }

//-----------------------------------------------------------------------------------------------------
    iDiscreteInputState =
        iFunctionBlockInputCheck(INPUT_SOURCE_FBL_3(pxSource),
                                 ((((unsigned short)(ADDRESS_HIGH_FBL_3(pxSource))) << 8) |
                                  ((unsigned short)(ADDRESS_LOW_FBL_3(pxSource)))));
    // ���������� ������ - ���������������� � ����������?
    if (iDiscreteInputState != -1)
    {
        fbInputIsReliable = 1;
        // ���������� ������ - "�������"?
        if (((INPUT_TYPE_FBL_3(pxSource)) &&
                (iDiscreteInputState)) ||
                (((INPUT_TYPE_FBL_3(pxSource)) == 0) &&
                 (iDiscreteInputState == 0)))
        {
            // ���������� ������ - "�������".
            // ��������� ������ ��������������� �������� "���" - 1.
            ucCurrentState = 1;
        }
        else
        {

        }
    }

//-----------------------------------------------------------------------------------------------------
    iDiscreteInputState =
        iFunctionBlockInputCheck(INPUT_SOURCE_FBL_4(pxSource),
                                 ((((unsigned short)(ADDRESS_HIGH_FBL_4(pxSource))) << 8) |
                                  ((unsigned short)(ADDRESS_LOW_FBL_4(pxSource)))));
    // ���������� ������ - ���������������� � ����������?
    if (iDiscreteInputState != -1)
    {
        fbInputIsReliable = 1;
        // ���������� ������ - "�������"?
        if (((INPUT_TYPE_FBL_4(pxSource)) &&
                (iDiscreteInputState)) ||
                (((INPUT_TYPE_FBL_4(pxSource)) == 0) &&
                 (iDiscreteInputState == 0)))
        {
            // ���������� ������ - "�������".
            // ��������� ������ ��������������� �������� "���" - 1.
            ucCurrentState = 1;
        }
        else
        {

        }
    }
    return ucCurrentState;
}






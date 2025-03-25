
#include "PasNewConfig.h"
#include "ModuleMtvi5.h"
#include "Events.h"

using namespace std;

void iModuleMtvi5DataHandler(TModuleContext *pxModuleContext)
{
    switch(pxModuleContext ->
            xModuleContextDinamic.
            ucFsmState)
    {
    case MTVI5_FSM_STATE_TRANSFER:
        iModuleMtvi5DataExchange(pxModuleContext);
        break;

    case MTVI5_FSM_STATE_WRITE_REGULATOR_DATABASE:
        // ��������� �������� ������ ������� - ������ ���� ������.
        pxModuleContext ->
        xModuleContextDinamic.
        ucCommandControl = DEVICE_CONTROL_MTVI5_DATA_BASE_WRITE_BY_HMI;

        if (iModuleMtvi5RegulatorDataBaseBlockWrite(pxModuleContext) == 0)
        {

            iPlcDataBaseBlockWrite(pxModuleContext ->
                                   xModuleContextDinamic.
                                   ucCommonIndex);
//            vWorkArraysUpdate();

            // ������� �������������, ��� ������ ���, �� 14 ������� Modbus - ������ � ����������� ����������������.
            ucDataBaseError = PROGRAMMING_DATA_BASE_WRITE_OK;
            iPlcDataBaseBlockWrite(CURRENT_OUTPUT_MODULE_REGULATOR_DATA_BASE_BLOCK_OFFSET +
                                   pxModuleContext ->
                                   xModuleContextStatic.
                                   ucInsideTypeIndex);
        }
        else
        {
            // ������� ��� ������, ��� �������� �������������, �� 14 ������� Modbus - ������ � ����������� ����������������.
            ucDataBaseError = PROGRAMMING_DATA_BASE_READ_WRITE_BLOCK_PSP_EEPROM_ERROR;
            // ��������� �������� ������ �������.
            pxModuleContext ->
            xModuleContextDinamic.
            ucCommandControl = 0;
            cout << "CURRENT_OUTPUT_DATA_BASE_BIG_ENDIAN_BYTE_ARRAY ERROR" << endl;
            // ��� ������ � ������ ��������� ������.
            // ����������� ���� ������.
            iPlcDataBaseCheck();
        }

        vWorkArraysUpdate();

        pxModuleContext ->
        xModuleContextDinamic.
        ucCommonIndex = 0;

        // ������� ��������� � ����� - ����� ������� � �������.
        pxModuleContext ->
        xModuleContextDinamic.
        ucFsmState = MTVI5_FSM_STATE_TRANSFER;
        break;

    case MTVI5_FSM_STATE_WRITE_REGULATOR_AND_PSP_DATABASE_REGULATOR:
        // ��������� �������� ������ ������� - ������ ���� ������.
        pxModuleContext ->
        xModuleContextDinamic.
        ucCommandControl = DEVICE_CONTROL_MTVI5_DATA_BASE_WRITE_BY_PROGRAMMER;

        if (iModuleMtvi5RegulatorDataBaseBlockWrite(pxModuleContext) == 0)
        {
            // �������� �������� �� 1.5 �������, ��� ������ � EEPROM ������.
            pxModuleContext ->
            xModuleContextDinamic.
            nucLoopSkipCounter = 0;
            pxModuleContext ->
            xModuleContextDinamic.
            ucFsmState = MTVI5_FSM_STATE_WRITE_REGULATOR_AND_PSP_DATABASE_PSP;
        }
        else
        {
            // ������� ��� ������, ��� �������� �������������, �� 14 ������� Modbus - ������ � ����������� ����������������.
            ucDataBaseError = PROGRAMMING_DATA_BASE_READ_WRITE_BLOCK_PSP_EEPROM_ERROR;
            // ��������� �������� ������ �������.
            pxModuleContext ->
            xModuleContextDinamic.
            ucCommandControl = 0;
            cout << "MTVI5_FSM_STATE_WRITE_REGULATOR_AND_PSP_DATABASE_REGULATOR ERROR" << endl;
            // ��� ������ � ������ ��������� ������.
            // ����������� ���� ������.
            iPlcDataBaseCheck();
            vWorkArraysUpdate();

            pxModuleContext ->
            xModuleContextDinamic.
            ucCommonIndex = 0;

            // ������� ��������� � ����� - ����� ������� � �������.
            pxModuleContext ->
            xModuleContextDinamic.
            ucFsmState = MTVI5_FSM_STATE_TRANSFER;
        }
        break;

    case MTVI5_FSM_STATE_WRITE_REGULATOR_AND_PSP_DATABASE_PSP:
        // ����������� �������� �� 1.5 �������, ��� ������ � EEPROM ������?
        if ((pxModuleContext ->
                xModuleContextDinamic.
                nucLoopSkipCounter) == MTVI5_WRITE_WAIT_QUANTITY)
        {
            if (iModuleMtvi5PspDataBaseBlockWrite(pxModuleContext) == 0)
            {

                iPlcDataBaseBlockWrite(pxModuleContext ->
                                       xModuleContextDinamic.
                                       ucCommonIndex);
                //                vWorkArraysUpdate();

                // ������� �������������, ��� ������ ���, �� 14 ������� Modbus - ������ � ����������� ����������������.
                ucDataBaseError = PROGRAMMING_DATA_BASE_WRITE_OK;
                iPlcDataBaseBlockWrite(CURRENT_OUTPUT_MODULE_REGULATOR_DATA_BASE_BLOCK_OFFSET +
                                       pxModuleContext ->
                                       xModuleContextStatic.
                                       ucInsideTypeIndex);
                // �������� �������� �� 1.5 �������, ��� ������ � EEPROM ������.
                pxModuleContext ->
                xModuleContextDinamic.
                nucLoopSkipCounter = 0;
                pxModuleContext ->
                xModuleContextDinamic.
                ucFsmState = MTVI5_FSM_STATE_WRITE_REGULATOR_AND_PSP_DATABASE_PSP_WRITE_WAITING;
            }
            else
            {
                // ������� ��� ������, ��� �������� �������������, �� 14 ������� Modbus - ������ � ����������� ����������������.
                ucDataBaseError = PROGRAMMING_DATA_BASE_READ_WRITE_BLOCK_PSP_EEPROM_ERROR;
                // ��������� �������� ������ �������.
                pxModuleContext ->
                xModuleContextDinamic.
                ucCommandControl = 0;
                // ������� ��������� � ����� - ����� ������� � �������.
                pxModuleContext ->
                xModuleContextDinamic.
                ucFsmState = MTVI5_FSM_STATE_TRANSFER;
                cout << "MTVI5_FSM_STATE_WRITE_REGULATOR_AND_PSP_DATABASE_PSP ERROR" << endl;
                // ��� ������ � ������ ��������� ������.
                // ����������� ���� ������.
                iPlcDataBaseCheck();
            }

            vWorkArraysUpdate();

            pxModuleContext ->
            xModuleContextDinamic.
            ucCommonIndex = 0;

        }
        else
        {
            (pxModuleContext ->
             xModuleContextDinamic.
             nucLoopSkipCounter)++;
//            iModuleMtvi5DataExchange(pxModuleContext);
        }
        break;

    case MTVI5_FSM_STATE_WRITE_REGULATOR_AND_PSP_DATABASE_PSP_WRITE_WAITING:
        // ����������� �������� �� 1.5 �������, ��� ������ � EEPROM ������?
        if ((pxModuleContext ->
                xModuleContextDinamic.
                nucLoopSkipCounter) == MTVI5_WRITE_WAIT_QUANTITY)
        {
            // ������� ��������� � ����� - ����� ������� � �������.
            pxModuleContext ->
            xModuleContextDinamic.
            ucFsmState = MTVI5_FSM_STATE_TRANSFER;
        }
        else
        {
            (pxModuleContext ->
             xModuleContextDinamic.
             nucLoopSkipCounter)++;
//            iModuleMtvi5DataExchange(pxModuleContext);
        }
        break;

    default:
        break;
    };
}

//-----------------------------------------------------------------------------------------------------
// ���������� ����� ������� � �������� ����������� ������.
int iModuleMtvi5DataExchange(TModuleContext *pxModuleContext)
{
    int i;
    float fData;
    float fResult;
    unsigned short usData;
    float fMinOfScale;
    float fMaxOfScale;
    unsigned char ucRegulatorMode;
    unsigned short usMaxCode;
    unsigned char nucRegulatorCounter;
    unsigned char nucFrameLength;
    unsigned char *pucSource;
    unsigned char *pucDestination;
    TAnalogueInputDescriptionWork *pxAnalogueInputDescriptionWork;
    TAnalogueOutputRegulatorDescriptionWork *pxAnalogueOutputRegulatorDescriptionWork;
    TMtvi5ContAvData *pxMtvi5ContAvDataPointer;
    float *pfCalcValueOriginBufferPointer;
    // ������ ��� �������������� ������ ����������� �� �������
    // ��������� � ������ ��������.
    TMtvi5TransmitData axMtvi5TransmitData[MAX_HANDLED_ANALOGUE_OUTPUT];
    // ������ ��� �������������� ��������� ����������� �� �������
    // ��������� � ������ ��������.
    unsigned char aucMtvi5TransmitState[MAX_HANDLED_ANALOGUE_OUTPUT];
    unsigned char ucRegulatorNumber;
    unsigned char auiSpiTxBuffer[TX_RX_BUFF_SIZE];
    unsigned char auiSpiRxBuffer[TX_RX_BUFF_SIZE];

    float fTempResult;

//-----------------------------------------------------------------------------------------------------
//    // ������ ����������� ������ ������������ ��� � - (������������ ��������� ����� ������� 100mc * MTVI5_LOOP_SKIP_QUANTITY).
//    // ������ - (MTVI5_LOOP_SKIP_QUANTITY) ������ 100mc?
//    if ((pxModuleContext ->
//            xModuleContextDinamic.
//            nucLoopSkipCounter) == MTVI5_LOOP_SKIP_QUANTITY)
//    {
//        // ������� �������� �� - MTVI5_LOOP_SKIP_QUANTITY.
//        pxModuleContext ->
//        xModuleContextDinamic.
//        nucLoopSkipCounter = 0;
    // ������� ��������� �� ��������������� ���� ���� ������ ���������� ���������� ������.
    pxAnalogueInputDescriptionWork = (TAnalogueInputDescriptionWork*)(pxModuleContext ->
                                     xModuleContextStatic.
                                     pucAnalogueInputDescriptionWork);
    // ������� ��������� �� ���� ���� ������ ���������� ���������� ������� � ����� �������.
    pxAnalogueOutputRegulatorDescriptionWork = (TAnalogueOutputRegulatorDescriptionWork*)(pxModuleContext ->
            xModuleContextStatic.
            pucDataBaseExtractPointer);
    // ������� ��������� �� ����� � ����������� ��������� �����������.
    pxMtvi5ContAvDataPointer = (TMtvi5ContAvData*)(pxModuleContext ->
                               xModuleContextStatic.
                               pucContAvBufferPointer);
    nucRegulatorCounter = 0;
    // ��������� ���������� ������������������� �����������.
    // �� �� ���������� ������� ����� ������ ������ ��� ������ � �������.
    for (i = 0; i < MTVI5_ANALOG_OUTPUT_QUANTITY; i++)
    {
        if (pxAnalogueOutputRegulatorDescriptionWork[i].ucPvAddress)
        {
            nucRegulatorCounter++;
        }
    }

    // ���������� ������������������� ����������� ����������� ����������� ���������?
    if (nucRegulatorCounter > MAX_HANDLED_ANALOGUE_OUTPUT)
    {
        // ������.
        return -1;
    }

    // �������� ����� ��� ������ � ������� ������� - ���� ��������� SP, OUT 1 ����.
    // ���� ����� ����������, �������� �������� ����������?
    if (pxModuleContext ->
            xModuleContextDinamic.
            ucCommonIndex)
    {
        printf("iModuleMtvi5DataHandler ucModuleContextIndex %d\n\r", (pxModuleContext ->
                xModuleContextStatic.
                ucModuleContextIndex));
        printf("iModuleMtvi5DataHandler ucCommonIndex %d\n\r", (pxModuleContext ->
                xModuleContextDinamic.
                ucCommonIndex));
        printf("iModuleMtvi5DataHandler ucCommandControl %d\n\r", (pxModuleContext ->
                xModuleContextDinamic.
                ucCommandControl));
        // ������� ����� ����������, �������� �������� ����������.
        ucRegulatorNumber = pxModuleContext ->
                            xModuleContextDinamic.
                            ucCommonIndex;
    }
    else
    {
        // �������� ����������� �� ����������.
        ucRegulatorNumber = 0x00;
    }

//-----------------------------------------------------------------------------------------------------
    // ���������� ������ ����������� ��� �������� � ������.
    for (i = 0; i < nucRegulatorCounter; i++)
    {
//-----------------------------------------------------------------------------------------------------

//            cout << "pucLogSostOriginBufferPointer" << endl;
//            unsigned char *pucSourceTemp;
//            pucSourceTemp = (unsigned char*)pxModuleContext ->
//                            xModuleContextStatic.
//                            pucLogSostOriginBufferPointer;
//            for(int i=0; i<64; )
//            {
//                for(int j=0; j<8; j++)
//                {
//                    cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
//                }
//                cout << endl;
//                i += 8;
//            }

//            cout << "pucCalcValueOriginBufferPointer" << endl;
//            unsigned char *pucSourceTemp;
//            pucSourceTemp = (unsigned char*)pxModuleContext ->
//                            xModuleContextStatic.
//                            pucCalcValueOriginBufferPointer;
//            for(int i=0; i<64; )
//            {
//                for(int j=0; j<8; j++)
//                {
//                    cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
//                }
//                cout << endl;
//                i += 8;
//            }

//-----------------------------------------------------------------------------------------------------
        // ��������� ����� ������ ����������.
        // ��������� ��������� � �������������� ������?
        if ((pxModuleContext ->
                xModuleContextStatic.
                pucContStWriteBufferPointer[i]) &
                (0x01 << MTVI5_STAT_AUTO_OR_MANUAL_BIT))
        {
            // ��������� ���������������� ��� ������ � ������ - ������������� ����������(fc)?
            if (pxAnalogueOutputRegulatorDescriptionWork[i].
                    ucForceControlFcOnAddress)
            {
                // �� CodeSys ���� ������� - ������������� ����������(fc)?
                if (pxModuleContext ->
                        xModuleContextStatic.
                        pucLogSostOriginBufferPointer[(pxAnalogueOutputRegulatorDescriptionWork[i].
                                                       ucForceControlFcOnAddress) -
                                                      CONVERT_NATURAL_NUMBER_TO_INTEGER])
                {
                    ucRegulatorMode = REGULATOR_MODE_AUTO_FORCE_CONTROL;
                    // ������� ����� ����������, �������� �������� ����������.
                    ucRegulatorNumber = (i % MTVI5_REGULATOR_QUANTITY) +
                                        CONVERT_INTEGER_TO_NATURAL_NUMBER;
                }
                else
                {
                    ucRegulatorMode = REGULATOR_MODE_AUTO_NORMAL;
                }
            }
            // ��������� ��������� � ������ - ���������� ��������?
            else if  ((pxModuleContext ->
                       xModuleContextStatic.
                       pucContStWriteBufferPointer[i]) &
                      (0x01 << MTVI5_STAT_MODE_PROGRAMM_BIT))
            {
                ucRegulatorMode = REGULATOR_MODE_AUTO_PROGRAMM_SET_POINT;
                // ������� ����� ����������, �������� �������� ����������.
                ucRegulatorNumber = (i % MTVI5_REGULATOR_QUANTITY) +
                                    CONVERT_INTEGER_TO_NATURAL_NUMBER;
            }
            else
            {
                ucRegulatorMode = REGULATOR_MODE_AUTO_NORMAL;
            }
        }
        else
        {
            ucRegulatorMode = REGULATOR_MODE_MANUAL;
        }

//-----------------------------------------------------------------------------------------------------
        switch(ucRegulatorMode)
        {
//-----------------------------------------------------------------------------------------------------
        case REGULATOR_MODE_MANUAL:
//                cout << "REGULATOR_MODE_MANUAL" << endl;
            // ������� �������� ������ ����� ��� �� ��������������� ���� ������ ���������� ���������� ������.
            fMinOfScale = pxAnalogueInputDescriptionWork[(pxAnalogueOutputRegulatorDescriptionWork[i].
                          ucPvAddress) -
                          CONVERT_NATURAL_NUMBER_TO_INTEGER].
                          fMinOfScale;
            // ������� �������� ����� ����� ��� �� ��������������� ���� ������ ���������� ���������� ������.
            fMaxOfScale = pxAnalogueInputDescriptionWork[(pxAnalogueOutputRegulatorDescriptionWork[i].
                          ucPvAddress) -
                          CONVERT_NATURAL_NUMBER_TO_INTEGER].
                          fMaxOfScale;
            // �������� �� ��������� �������� CoDeSys?
            if ((pxAnalogueOutputRegulatorDescriptionWork[i].ucControl) &
                    (0x01 << REGULATOR_CONTROL_PV_SOURSE_IS_CALC_CDS))
            {
                // ������� ���������� �������� �� ������ (ucPvAddress) �� ��������� �������� CoDeSys.
                memcpy((unsigned char*)&fData,
                       &(pxModuleContext ->
                         xModuleContextStatic.
                         pucCalcValueOriginBufferPointer[((pxAnalogueOutputRegulatorDescriptionWork[i].
                                 ucPvAddress) -
                                 CONVERT_NATURAL_NUMBER_TO_INTEGER) * sizeof(float)]),
                       sizeof(float));

                pxMtvi5ContAvDataPointer[i].fPv = fData;
                // ����������� ���������� �������� �� ������ ����� ������������� ���������
                // � ������ �������������� ����.
                // ����������� �������� ������ OUT � ����� 0-100%.
                usData = (unsigned short)(((float)MTVI5_MAX_CODE) * (fData / (float)PERCENT_100));
            }
            else
            {
                // ���������� �������� �� ������ ����� ������������� ��������� ������������?
                if (pxModuleContext ->
                        xModuleContextStatic.
                        pucBadAiOriginBufferPointer[((pxAnalogueOutputRegulatorDescriptionWork[i].
                                                      ucPvAddress) -
                                                     CONVERT_NATURAL_NUMBER_TO_INTEGER)])
                {
                    // ���������� �������� �� ������ ����� ������������� ��������� - ������������.
                    // ������� �� ������ ������.
                    usData = (unsigned short)MTVI5_PV_ERROR;
                }
                else
                {
                    // ������� ���������� �������� �� ������ ����� ������������� ���������.
                    memcpy((unsigned char*)&fData,
                           &(pxModuleContext ->
                             xModuleContextStatic.
                             pucAiValueOriginBufferPointer[((pxAnalogueOutputRegulatorDescriptionWork[i].
                                                            ucPvAddress) -
                                                            CONVERT_NATURAL_NUMBER_TO_INTEGER) * sizeof(float)]),
                           sizeof(float));

                    pxMtvi5ContAvDataPointer[i].fPv = fData;
                    // ����������� ���������� �������� �� ������ ����� ������������� ���������
                    // � ������ �������������� ����.
                    usData = (unsigned short)(((float)MTVI5_MAX_CODE) * ((fData - fMinOfScale) / (fMaxOfScale - fMinOfScale)));
                }
            }

            // �������� �������� � ������ ��� �������� � ������.
            axMtvi5TransmitData[i].usPv = usData;

//-----------------------------------------------------------------------------------------------------
            // ���������� ������� ���������-��������� ������� - SP, OUT,
            // ���� ��������� �� ������� vDeviceControlModbusFunction5Handler().
            switch(pxModuleContext ->
                    xModuleContextDinamic.
                    ucCommandControl)
            {
                // ��������� ������� ��������� ������� - SP, OUT.
            case DEVICE_CONTROL_MTVI5_INCREMENT_SP_OUT:
//                // ������� �������.
//                pxModuleContext ->
//                xModuleContextDinamic.
//                ucCommandControl = 0;
                // ��������� ��������� � ������ ������, �������������� �������� ������ - OUT.
                fData = pxMtvi5ContAvDataPointer[i].fOut;
                // ����������� �������� ������ OUT � ����� 0-100%.
                usData = (unsigned short)(((float)MTVI5_MAX_CODE) * (fData / (float)PERCENT_100));
                // �������� �������� �� ���������� ��������� � - usAuxiliaryCounter,
                // ����������� �� ������� vDeviceControlModbusFunction5Handler().
                usData += (unsigned short)((pxModuleContext ->
                                            xModuleContextDinamic.
                                            usAuxiliaryCounter) * MTVI5_MAX_CODE_ONE_PERCENT);
                // ����������� �������� OUT
                // �� ������� �������������� ����, �� float IEEE754.
                fData = (float)(((float)PERCENT_100) * ((float)usData / (float)MTVI5_MAX_CODE));
                // �������� � ������ ���������� ���������� ����������� �CONT_AV�.
                pxMtvi5ContAvDataPointer[i].fOut = fData;
                break;
                // ��������� ������� ��������� ������� - SP, OUT.
            case DEVICE_CONTROL_MTVI5_DECREMENT_SP_OUT:
//                // ������� �������.
//                pxModuleContext ->
//                xModuleContextDinamic.
//                ucCommandControl = 0;
                // ��������� ��������� � ������ ������, �������������� �������� ������ - OUT.
                fData = pxMtvi5ContAvDataPointer[i].fOut;
                // ����������� �������� ������ OUT � ����� 0-100%.
                usData = (unsigned short)(((float)MTVI5_MAX_CODE) * (fData / (float)PERCENT_100));
                // �������� �������� �� ���������� ��������� � - usAuxiliaryCounter,
                // ����������� �� ������� vDeviceControlModbusFunction5Handler().
                usData -= (unsigned short)((pxModuleContext ->
                                            xModuleContextDinamic.
                                            usAuxiliaryCounter) * MTVI5_MAX_CODE_ONE_PERCENT);
                // ����������� �������� OUT
                // �� ������� �������������� ����, �� float IEEE754.
                fData = (float)(((float)PERCENT_100) * ((float)usData / (float)MTVI5_MAX_CODE));
                // �������� � ������ ���������� ���������� ����������� �CONT_AV�.
                pxMtvi5ContAvDataPointer[i].fOut = fData;
                break;
                // ������� ���������-��������� ������� - SP, OUT - �� ���������.
            default:
                // ��������� ��������� � ������ ������, �������� ������ �������� ������ - OUT.
                fData = pxMtvi5ContAvDataPointer[i].fOut;
                // ����������� �������� ������ OUT � ����� 0-100%.
                usData = (unsigned short)(((float)MTVI5_MAX_CODE) * (fData / (float)PERCENT_100));
                break;
            };
            // �������� �������� � ������ ��� �������� � ������.
            axMtvi5TransmitData[i].usSpOut = usData;

//-----------------------------------------------------------------------------------------------------
            aucMtvi5TransmitState[i] = (pxModuleContext ->
                                        xModuleContextStatic.
                                        pucContStWriteBufferPointer[i]);

//                printf("index %d\n", (i)); // MTVI TempRepair.
//                printf("ucPvAddress %d\n", (pxAnalogueOutputRegulatorDescriptionWork[i].
//                                            ucPvAddress));
//                printf("fPv %f\n", (pxMtvi5ContAvDataPointer[i].fPv));
//                printf("fOut %f\n", (pxMtvi5ContAvDataPointer[i].fOut));
//                printf("fSp %f\n", (pxMtvi5ContAvDataPointer[i].fSp));
//                // printf("pxMtvi5ContAvDataPointer %d\n", (pxMtvi5ContAvDataPointer));
////                printf("ucModuleContextIndex %d\n", (pxModuleContext ->
////                                                     xModuleContextStatic.
////                                                     ucModuleContextIndex));
            break;
//-----------------------------------------------------------------------------------------------------
        case REGULATOR_MODE_AUTO_NORMAL:
//                cout << "REGULATOR_MODE_AUTO_NORMAL" << endl;
            // ������� �������� ������ ����� ��� �� ��������������� ���� ������ ���������� ���������� ������.
            fMinOfScale = pxAnalogueInputDescriptionWork[(pxAnalogueOutputRegulatorDescriptionWork[i].
                          ucPvAddress) -
                          CONVERT_NATURAL_NUMBER_TO_INTEGER].
                          fMinOfScale;
            // ������� �������� ����� ����� ��� �� ��������������� ���� ������ ���������� ���������� ������.
            fMaxOfScale = pxAnalogueInputDescriptionWork[(pxAnalogueOutputRegulatorDescriptionWork[i].
                          ucPvAddress) -
                          CONVERT_NATURAL_NUMBER_TO_INTEGER].
                          fMaxOfScale;
            // �������� �� ��������� �������� CoDeSys?
            if ((pxAnalogueOutputRegulatorDescriptionWork[i].ucControl) &
                    (0x01 << REGULATOR_CONTROL_PV_SOURSE_IS_CALC_CDS))
            {
                // ������� ���������� �������� �� ������ (ucPvAddress) �� ��������� �������� CoDeSys.
                memcpy((unsigned char*)&fData,
                       &(pxModuleContext ->
                         xModuleContextStatic.
                         pucCalcValueOriginBufferPointer[((pxAnalogueOutputRegulatorDescriptionWork[i].
                                 ucPvAddress) -
                                 CONVERT_NATURAL_NUMBER_TO_INTEGER) * sizeof(float)]),
                       sizeof(float));

                pxMtvi5ContAvDataPointer[i].fPv = fData;
                // ����������� ���������� �������� �� ������ ����� ������������� ���������
                // � ������ �������������� ����.
                // ����������� �������� ������ OUT � ����� 0-100%.
                usData = (unsigned short)(((float)MTVI5_MAX_CODE) * (fData / (float)PERCENT_100));
            }
            else
            {
                // ���������� �������� �� ������ ����� ������������� ��������� ������������?
                if (pxModuleContext ->
                        xModuleContextStatic.
                        pucBadAiOriginBufferPointer[((pxAnalogueOutputRegulatorDescriptionWork[i].
                                                      ucPvAddress) -
                                                     CONVERT_NATURAL_NUMBER_TO_INTEGER)])
                {
                    // ���������� �������� �� ������ ����� ������������� ��������� - ������������.
                    // ������� �� ������ ������.
                    usData = (unsigned short)MTVI5_PV_ERROR;
                }
                else
                {
                    // ������� ���������� �������� �� ������ ����� ������������� ���������.
                    memcpy((unsigned char*)&fData,
                           &(pxModuleContext ->
                             xModuleContextStatic.
                             pucAiValueOriginBufferPointer[((pxAnalogueOutputRegulatorDescriptionWork[i].
                                                            ucPvAddress) -
                                                            CONVERT_NATURAL_NUMBER_TO_INTEGER) * sizeof(float)]),
                           sizeof(float));

                    pxMtvi5ContAvDataPointer[i].fPv = fData;
                    // ����������� ���������� �������� �� ������ ����� ������������� ���������
                    // � ������ �������������� ����.
                    usData = (unsigned short)(((float)MTVI5_MAX_CODE) * ((fData - fMinOfScale) / (fMaxOfScale - fMinOfScale)));
                }
            }

            // �������� �������� � ������ ��� �������� � ������.
            axMtvi5TransmitData[i].usPv = usData;

//-----------------------------------------------------------------------------------------------------
            // ���������� ������� ���������-��������� ������� - SP, OUT,
            // ���� ��������� �� ������� vDeviceControlModbusFunction5Handler().
            switch(pxModuleContext ->
                    xModuleContextDinamic.
                    ucCommandControl)
            {
                // ��������� ������� ��������� ������� - SP, OUT.
            case DEVICE_CONTROL_MTVI5_INCREMENT_SP_OUT:
//                // ������� �������.
//                pxModuleContext ->
//                xModuleContextDinamic.
//                ucCommandControl = 0;
                // ��������� ��������� � �������������� ������, �������������� ������� - SP.
                fData = pxMtvi5ContAvDataPointer[i].fSp;
                // ����������� �������� ������� SP � ������ �������������� ����.
                usData = (unsigned short)(((float)MTVI5_MAX_CODE) * ((fData - fMinOfScale) / (fMaxOfScale - fMinOfScale)));
                // �������� �������� �� ���������� ��������� � - usAuxiliaryCounter,
                // ����������� �� ������� vDeviceControlModbusFunction5Handler().
                usData += (unsigned short)((pxModuleContext ->
                                            xModuleContextDinamic.
                                            usAuxiliaryCounter) * MTVI5_MAX_CODE_ONE_PERCENT);
                // ����������� �������� SP
                // �� ������� �������������� ����, �� float IEEE754.
                fData = (float)((((float)usData * (fMaxOfScale - fMinOfScale)) / (float)MTVI5_MAX_CODE) + fMinOfScale);
                // �������� � ������ ���������� ���������� ����������� �CONT_AV�.
                pxMtvi5ContAvDataPointer[i].fSp = fData;
                break;
                // ��������� ������� ��������� ������� - SP, OUT.
            case DEVICE_CONTROL_MTVI5_DECREMENT_SP_OUT:
//                // ������� �������.
//                pxModuleContext ->
//                xModuleContextDinamic.
//                ucCommandControl = 0;
                // ��������� ��������� � �������������� ������, �������������� ������� - SP.
                fData = pxMtvi5ContAvDataPointer[i].fSp;
                // ����������� �������� ������� SP � ������ �������������� ����.
                usData = (unsigned short)(((float)MTVI5_MAX_CODE) * ((fData - fMinOfScale) / (fMaxOfScale - fMinOfScale)));
                // �������� �������� �� ���������� ��������� � - usAuxiliaryCounter,
                // ����������� �� ������� vDeviceControlModbusFunction5Handler().
                usData -= (unsigned short)((pxModuleContext ->
                                            xModuleContextDinamic.
                                            usAuxiliaryCounter) * MTVI5_MAX_CODE_ONE_PERCENT);
                // ����������� �������� SP
                // �� ������� �������������� ����, �� float IEEE754.
                fData = (float)((((float)usData * (fMaxOfScale - fMinOfScale)) / (float)MTVI5_MAX_CODE) + fMinOfScale);
                // �������� � ������ ���������� ���������� ����������� �CONT_AV�.
                pxMtvi5ContAvDataPointer[i].fSp = fData;
                break;
                // ������� ���������-��������� ������� - SP, OUT - �� ���������.
            default:
                // ��������� ��������� � �������������� ������, �������� ������ �������� SP.
                fData = pxMtvi5ContAvDataPointer[i].fSp;//34;//
                // ����������� �������� ������� SP � ������ �������������� ����.
                usData = (unsigned short)(((float)MTVI5_MAX_CODE) * ((fData - fMinOfScale) / (fMaxOfScale - fMinOfScale)));
                break;
            };

            // �������� �������� � ������ ��� �������� � ������.
            axMtvi5TransmitData[i].usSpOut = usData;

//-----------------------------------------------------------------------------------------------------
            aucMtvi5TransmitState[i] = (pxModuleContext ->
                                        xModuleContextStatic.
                                        pucContStWriteBufferPointer[i]);
            break;

//-----------------------------------------------------------------------------------------------------
        case REGULATOR_MODE_AUTO_PROGRAMM_SET_POINT:
//                cout << "REGULATOR_MODE_AUTO_PROGRAMM_SET_POINT" << endl;
            // ������� �������� ������ ����� ��� �� ��������������� ���� ������ ���������� ���������� ������.
            fMinOfScale = pxAnalogueInputDescriptionWork[(pxAnalogueOutputRegulatorDescriptionWork[i].
                          ucPvAddress) -
                          CONVERT_NATURAL_NUMBER_TO_INTEGER].
                          fMinOfScale;
            // ������� �������� ����� ����� ��� �� ��������������� ���� ������ ���������� ���������� ������.
            fMaxOfScale = pxAnalogueInputDescriptionWork[(pxAnalogueOutputRegulatorDescriptionWork[i].
                          ucPvAddress) -
                          CONVERT_NATURAL_NUMBER_TO_INTEGER].
                          fMaxOfScale;
            // �������� �� ��������� �������� CoDeSys?
            if ((pxAnalogueOutputRegulatorDescriptionWork[i].ucControl) &
                    (0x01 << REGULATOR_CONTROL_PV_SOURSE_IS_CALC_CDS))
            {
                // ������� ���������� �������� �� ������ (ucPvAddress) �� ��������� �������� CoDeSys.
                memcpy((unsigned char*)&fData,
                       &(pxModuleContext ->
                         xModuleContextStatic.
                         pucCalcValueOriginBufferPointer[((pxAnalogueOutputRegulatorDescriptionWork[i].
                                 ucPvAddress) -
                                 CONVERT_NATURAL_NUMBER_TO_INTEGER) * sizeof(float)]),
                       sizeof(float));

                pxMtvi5ContAvDataPointer[i].fPv = fData;
                // ����������� ���������� �������� �� ������ ����� ������������� ���������
                // � ������ �������������� ����.
                // ����������� �������� ������ OUT � ����� 0-100%.
                usData = (unsigned short)(((float)MTVI5_MAX_CODE) * (fData / (float)PERCENT_100));
            }
            else
            {
                // ���������� �������� �� ������ ����� ������������� ��������� ������������?
                if (pxModuleContext ->
                        xModuleContextStatic.
                        pucBadAiOriginBufferPointer[((pxAnalogueOutputRegulatorDescriptionWork[i].
                                                      ucPvAddress) -
                                                     CONVERT_NATURAL_NUMBER_TO_INTEGER)])
                {
                    // ���������� �������� �� ������ ����� ������������� ��������� - ������������.
                    // ������� �� ������ ������.
                    usData = (unsigned short)MTVI5_PV_ERROR;
                }
                else
                {
                    // ������� ���������� �������� �� ������ ����� ������������� ���������.
                    memcpy((unsigned char*)&fData,
                           &(pxModuleContext ->
                             xModuleContextStatic.
                             pucAiValueOriginBufferPointer[((pxAnalogueOutputRegulatorDescriptionWork[i].
                                                            ucPvAddress) -
                                                            CONVERT_NATURAL_NUMBER_TO_INTEGER) * sizeof(float)]),
                           sizeof(float));

                    pxMtvi5ContAvDataPointer[i].fPv = fData;
                    // ����������� ���������� �������� �� ������ ����� ������������� ���������
                    // � ������ �������������� ����.
                    usData = (unsigned short)(((float)MTVI5_MAX_CODE) * ((fData - fMinOfScale) / (fMaxOfScale - fMinOfScale)));
                }
            }

            // �������� �������� � ������ ��� �������� � ������.
            axMtvi5TransmitData[i].usPv = usData;

//-----------------------------------------------------------------------------------------------------
            // ��������� ��������� � ������ - ���������� ��������.
            // �������� ������ �������� SP ���������� �� CodeSys.
            // ������� �������� ������� SP �� ������ (ucProgrammSPAddress) �� ��������� �������� CoDeSys.
            memcpy((unsigned char*)&fData,
                   &(pxModuleContext ->
                     xModuleContextStatic.
                     pucCalcValueOriginBufferPointer[((pxAnalogueOutputRegulatorDescriptionWork[i].
                             ucProgrammSPAddress) -
                             CONVERT_NATURAL_NUMBER_TO_INTEGER) * sizeof(float)]),
                   sizeof(float));
            // ����������� �������� ������� SP � ������ �������������� ����.
            // ����������� �������� ������ OUT � ����� 0-100%.
            usData = (unsigned short)(((float)MTVI5_MAX_CODE) * (fData / (float)PERCENT_100));
            // �������� �������� ������� SP � ������ ��� �������� � ������.
            axMtvi5TransmitData[i].usSpOut = usData;
            // �������� � ������ ���������� ���������� ����������� �CONT_AV�.
            pxMtvi5ContAvDataPointer[i].fSp = fData;

//-----------------------------------------------------------------------------------------------------
            aucMtvi5TransmitState[i] = (pxModuleContext ->
                                        xModuleContextStatic.
                                        pucContStWriteBufferPointer[i]);
            break;

//-----------------------------------------------------------------------------------------------------
        case REGULATOR_MODE_AUTO_FORCE_CONTROL:
//                cout << "REGULATOR_MODE_AUTO_FORCE_CONTROL" << endl;
            // ������� �������� ������ ����� ��� �� ��������������� ���� ������ ���������� ���������� ������.
            fMinOfScale = pxAnalogueInputDescriptionWork[(pxAnalogueOutputRegulatorDescriptionWork[i].
                          ucPvAddress) -
                          CONVERT_NATURAL_NUMBER_TO_INTEGER].
                          fMinOfScale;
            // ������� �������� ����� ����� ��� �� ��������������� ���� ������ ���������� ���������� ������.
            fMaxOfScale = pxAnalogueInputDescriptionWork[(pxAnalogueOutputRegulatorDescriptionWork[i].
                          ucPvAddress) -
                          CONVERT_NATURAL_NUMBER_TO_INTEGER].
                          fMaxOfScale;
            // �������� �� ��������� �������� CoDeSys?
            if ((pxAnalogueOutputRegulatorDescriptionWork[i].ucControl) &
                    (0x01 << REGULATOR_CONTROL_PV_SOURSE_IS_CALC_CDS))
            {
                // ������� ���������� �������� �� ������ (ucPvAddress) �� ��������� �������� CoDeSys.
                memcpy((unsigned char*)&fData,
                       &(pxModuleContext ->
                         xModuleContextStatic.
                         pucCalcValueOriginBufferPointer[((pxAnalogueOutputRegulatorDescriptionWork[i].
                                 ucPvAddress) -
                                 CONVERT_NATURAL_NUMBER_TO_INTEGER) * sizeof(float)]),
                       sizeof(float));

                pxMtvi5ContAvDataPointer[i].fPv = fData;
                // ����������� ���������� �������� �� ������ ����� ������������� ���������
                // � ������ �������������� ����.
                // ����������� �������� ������ OUT � ����� 0-100%.
                usData = (unsigned short)(((float)MTVI5_MAX_CODE) * (fData / (float)PERCENT_100));
            }
            else
            {
                // ���������� �������� �� ������ ����� ������������� ��������� ������������?
                if (pxModuleContext ->
                        xModuleContextStatic.
                        pucBadAiOriginBufferPointer[((pxAnalogueOutputRegulatorDescriptionWork[i].
                                                      ucPvAddress) -
                                                     CONVERT_NATURAL_NUMBER_TO_INTEGER)])
                {
                    // ���������� �������� �� ������ ����� ������������� ��������� - ������������.
                    // ������� �� ������ ������.
                    usData = (unsigned short)MTVI5_PV_ERROR;
                }
                else
                {
                    // ������� ���������� �������� �� ������ ����� ������������� ���������.
                    memcpy((unsigned char*)&fData,
                           &(pxModuleContext ->
                             xModuleContextStatic.
                             pucAiValueOriginBufferPointer[((pxAnalogueOutputRegulatorDescriptionWork[i].
                                                            ucPvAddress) -
                                                            CONVERT_NATURAL_NUMBER_TO_INTEGER) * sizeof(float)]),
                           sizeof(float));

                    pxMtvi5ContAvDataPointer[i].fPv = fData;
                    // ����������� ���������� �������� �� ������ ����� ������������� ���������
                    // � ������ �������������� ����.
                    usData = (unsigned short)(((float)MTVI5_MAX_CODE) * ((fData - fMinOfScale) / (fMaxOfScale - fMinOfScale)));
                }
            }

            // �������� �������� � ������ ��� �������� � ������.
            axMtvi5TransmitData[i].usPv = usData;

//-----------------------------------------------------------------------------------------------------
            // �� CodeSys ���� ������� - �������������� �����?
            if (pxModuleContext ->
                    xModuleContextStatic.
                    pucLogSostOriginBufferPointer[(pxAnalogueOutputRegulatorDescriptionWork[i].
                                                   ucForceControlHaOnAddress) -
                                                  CONVERT_NATURAL_NUMBER_TO_INTEGER])
            {
                // ��������� ��������� � �������������� ������ - �������������� ���������� � fc.
                // �������� ������ �������� SP ���������� �� CodeSys.
                // ������� �������� ������� SP �� ������ (ucForceControlSpOutAddress) �� ��������� �������� CoDeSys.
                memcpy((unsigned char*)&fData,
                       &(pxModuleContext ->
                         xModuleContextStatic.
                         pucCalcValueOriginBufferPointer[((pxAnalogueOutputRegulatorDescriptionWork[i].
                                 ucForceControlSpOutAddress) -
                                 CONVERT_NATURAL_NUMBER_TO_INTEGER) * sizeof(float)]),
                       sizeof(float));
                // ����������� �������� ������� SP � ������ �������������� ����.
                // ����������� �������� ������ OUT � ����� 0-100%.
                usData = (unsigned short)(((float)MTVI5_MAX_CODE) * (fData / (float)PERCENT_100));
                // �������� �������� ������� SP � ������ ��� �������� � ������.
                axMtvi5TransmitData[i].usSpOut = usData;
                // �������� � ������ ���������� ���������� ����������� �CONT_AV�.
                pxMtvi5ContAvDataPointer[i].fSp = fData;
            }
            else
            {
                // ��������� ��������� � �������������� ������ - �������������� ���������� � fc.
                // �������� ������ �������� SP ���������� �� CodeSys.
                // ������� �������� ������� SP �� ������ (ucForceControlSpOutAddress) �� ��������� �������� CoDeSys.
                memcpy((unsigned char*)&fData,
                       &(pxModuleContext ->
                         xModuleContextStatic.
                         pucCalcValueOriginBufferPointer[((pxAnalogueOutputRegulatorDescriptionWork[i].
                                 ucForceControlSpOutAddress) -
                                 CONVERT_NATURAL_NUMBER_TO_INTEGER) * sizeof(float)]),
                       sizeof(float));
                // ����������� �������� ������� SP � ������ �������������� ����.
                // ����������� �������� ������ OUT � ����� 0-100%.
                usData = (unsigned short)(((float)MTVI5_MAX_CODE) * (fData / (float)PERCENT_100));
                // �������� �������� ������� SP � ������ ��� �������� � ������.
                axMtvi5TransmitData[i].usSpOut = usData;
                // �������� � ������ ���������� ���������� ����������� �CONT_AV�.
                pxMtvi5ContAvDataPointer[i].fOut = fData;
            }

//-----------------------------------------------------------------------------------------------------
            // �� CodeSys ���� ������� - �������������� �����?
            if (pxModuleContext ->
                    xModuleContextStatic.
                    pucLogSostOriginBufferPointer[(pxAnalogueOutputRegulatorDescriptionWork[i].
                                                   ucForceControlHaOnAddress) -
                                                  CONVERT_NATURAL_NUMBER_TO_INTEGER])
            {
                // ��������� ���� - �������������� �����.
                aucMtvi5TransmitState[i] = ((pxModuleContext ->
                                             xModuleContextStatic.
                                             pucContStWriteBufferPointer[i]) |
                                            (0x01 << MTVI5_STAT_AUTO_OR_MANUAL_BIT));
            }
            else
            {
                // ��������� ���� - ������ �����.
                aucMtvi5TransmitState[i] = ((pxModuleContext ->
                                             xModuleContextStatic.
                                             pucContStWriteBufferPointer[i]) &
                                            (~(0x01 << MTVI5_STAT_AUTO_OR_MANUAL_BIT)));
            }

            break;

//-----------------------------------------------------------------------------------------------------
        default:
            break;
        };
    }

//-----------------------------------------------------------------------------------------------------
    pucDestination = &auiSpiTxBuffer[SPI_DATA_BYTE_OFFSET];
    // �������� ����� ������ ��� ������ ������� � �������.
    nucFrameLength = ((MTVI5_ONE_REGULATOR_FRAME_BYTE_QUANTITY * nucRegulatorCounter) +
                      MTVI5_SP_OUT_SET_BYTE_QUANTITY);
    // ������� ������ ����� ������ ��� ������ �������.
    *pucDestination++ = nucFrameLength;

    // �������� ����� ��� ������ ������� � ������� ���������� Pv
    // � ������� �������������� ����.
    for (i = 0; i < nucRegulatorCounter; i++)
    {
        *pucDestination++ = (unsigned char)axMtvi5TransmitData[i].usPv;
        *pucDestination++ = (unsigned char)((axMtvi5TransmitData[i].usPv) >> 8);
    }
    // �������� ����� ��� ������ ������� � ������� ���������� SpOut
    // � ������� �������������� ����.
    for (i = 0; i < nucRegulatorCounter; i++)
    {
        *pucDestination++ = (unsigned char)axMtvi5TransmitData[i].usSpOut;
        *pucDestination++ = (unsigned char)((axMtvi5TransmitData[i].usSpOut) >> 8);
    }

    // �������� ����� ��� ������ � ������� ������� ��������� �����������.
    for (i = 0; i < nucRegulatorCounter; i++)
    {
        *pucDestination++ = (aucMtvi5TransmitState[i]);
    }

    // �������� ����� ��� ������ � ������� ������� - ���� ��������� SP, OUT 1 ����.
    // ������� ����� ����������, �������� �������� ����������.
    *pucDestination++ = ucRegulatorNumber;

    // ������� ��������� �� ������ ������ ������ ������� � �������
    // ��� ���������� CRC(���� � ������ ������ �� ������ � ����������� CRC).
    pucSource = &auiSpiTxBuffer[SPI_DATA_BYTE_OFFSET + MTVI5_FRAME_LENGTH_BYTE_QUANTITY];
    usData = usCrcSummTwoByteCalculation(pucSource,
                                         nucFrameLength);
    *pucDestination++ = (unsigned char)usData;
    *pucDestination++ = (unsigned char)(usData >> 8);
    *pucDestination++ = DATA_EXCHANGE_OK;

//-----------------------------------------------------------------------------------------------------
    auiSpiTxBuffer[0] = MTVI5_DATA_EXCHANGE_COMMAND;



//        cout << "MTVI5_DATA_EXCHANGE_COMMAND auiSpiTxBuffer[0]" << endl;
////        unsigned char *pucSourceTemp;
//        pucSourceTemp = (unsigned char*)&auiSpiTxBuffer[0];
//        for(int i=0; i<32; )
//        {
//            for(int j=0; j<8; j++)
//            {
//                cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
//            }
//            cout << endl;
//            i += 8;
//        }


//        usleep(100);
    xSpiCommunicationDevice.Exchange(pxModuleContext ->
                                     xModuleContextStatic.
                                     ucModuleAddress,
                                     auiSpiTxBuffer,
                                     auiSpiRxBuffer,
                                     SPI_PREAMBLE_LENGTH +
                                     MTVI5_FRAME_LENGTH_BYTE_QUANTITY +
                                     nucFrameLength +
                                     TWO_BYTE_CRC_LENGTH +
                                     TAIL_ANSWER_LENGTH,
                                     MTVI5_EXCHANGE_SPEED_IN_HZ
                                    );


//                printf("iModuleMtvi5DataHandler ucModuleContextIndex %d\n\r", (pxModuleContext ->
//                        xModuleContextStatic.
//                        ucModuleContextIndex));
//
//            cout << "auiSpiTxBuffer[0]" << endl;
//            unsigned char *pucSourceTemp;
//            pucSourceTemp = (unsigned char*)&auiSpiTxBuffer[0];
//            for(int i=0; i<64; )
//            {
//                for(int j=0; j<8; j++)
//                {
//                    cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
//                }
//                cout << endl;
//                i += 8;
//            }
//
//            cout << "auiSpiRxBuffer[0]" << endl;
////            unsigned char *pucSourceTemp;
//            pucSourceTemp = (unsigned char*)&auiSpiRxBuffer[0];
//            for(int i=0; i<64; )
//            {
//                for(int j=0; j<8; j++)
//                {
//                    cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
//                }
//                cout << endl;
//                i += 8;
//            }

//static uint8_t ui8Counter = 0;
//
//    if (ui8Counter < 30)
//    {
//        ui8Counter++;
//        auiSpiRxBuffer[SPI_COMMAND_BYTE_OFFSET] = MTVI5_DAC_LINK_ERROR;
//    }
//    else
//    {
////        ui8Counter = 0;
//    }
//    auiSpiRxBuffer[SPI_COMMAND_BYTE_OFFSET] = 45;

//                printf("iModuleMtvi5DataExchange usAuxiliaryCounter %d\n\r", (ui8Counter));
//-----------------------------------------------------------------------------------------------------
    // ��� ������� ������?
    switch(auiSpiRxBuffer[SPI_COMMAND_BYTE_OFFSET])
    {
        // ���������� ����� �������.
    case MTVI5_DATA_EXCHANGE_COMMAND:

//            cout << "MTVI5_DATA_EXCHANGE_COMMAND command" << endl;

        // ������ �� ����������?
        if (iCrcSummTwoByteCompare(&auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET + MTVI5_FRAME_LENGTH_BYTE_QUANTITY],
                                   nucFrameLength) > 0)
        {
//            cout << "auiSpiTxBuffer[0]" << endl;
//            unsigned char *pucSourceTemp;
//            pucSourceTemp = (unsigned char*)&auiSpiTxBuffer[0];
//            for(int i=0; i<64; )
//            {
//                for(int j=0; j<8; j++)
//                {
//                    cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
//                }
//                cout << endl;
//                i += 8;
//            }
//
//            cout << "auiSpiRxBuffer[0]" << endl;
////            unsigned char *pucSourceTemp;
//            pucSourceTemp = (unsigned char*)&auiSpiRxBuffer[0];
//            for(int i=0; i<64; )
//            {
//                for(int j=0; j<8; j++)
//                {
//                    cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
//                }
//                cout << endl;
//                i += 8;
//            }

            // ������ ��������.
            pxModuleContext ->
            xModuleContextDinamic.
            nucBadAnswerCounter = BAD_MODULE_CYCLE_COUNT_DEFAULT;
            // ������ ��������.
            pxModuleContext ->
            xModuleContextDinamic.
            usAuxiliaryCounter = 0;

            // ������� ���� ������ ������.
            *(pxModuleContext ->
              xModuleContextStatic.
              pucModuleBadStateBufferPointer) = BAD_MODULE_RESPONDED_OK;

//                printf("iModuleMtvi5DataExchange MTVI5_DATA_EXCHANGE_COMMAND %d\n\r", (MTVI5_BAD_ANSWER_ERROR_OFFSET));
            // ���������� ��������� ������� ��� �� ����������������?
            if(xCInternalModuleErrorEvent.EventOffIsNotRegistered(
                        pxModuleContext ->
                        xModuleContextStatic.
                        ucModuleContextIndex,
                        MTVI5_BAD_ANSWER_ERROR_OFFSET))
            {
                // �������������� ���������� ��������� �������.
                // ����� ������ �������.
                CEvents::EventRegistration(
                    pxModuleContext ->
                    xModuleContextStatic.
                    ucModuleContextIndex,
                    (CEvents::HANDLED_EVENTS_INTERNAL_MODULES_BAD_TYPE |
                     CEvents::HANDLED_EVENTS_IS_ARCHIVE),
                    MTVI5_BAD_ANSWER_ERROR_OFFSET,
                    "����. ������");
            }

            // �������� ��������� ������� ��� �� ����������������?
            if(xCInternalModuleErrorEvent.EventOffIsNotRegistered(
                        pxModuleContext ->
                        xModuleContextStatic.
                        ucModuleContextIndex,
                        MTVI5_DAC_LINK_ERROR_OFFSET))
            {
                // �������������� �������� ��������� �������.
                // ������ ������� ���.
                CEvents::EventRegistration(
                    pxModuleContext ->
                    xModuleContextStatic.
                    ucModuleContextIndex,
                    (CEvents::HANDLED_EVENTS_INTERNAL_MODULES_BAD_TYPE |
                     CEvents::HANDLED_EVENTS_IS_ARCHIVE),
                    MTVI5_DAC_LINK_ERROR_OFFSET,
                    "����. �. ���");
            }

            // ������ ������ ����������.
            memset(pxModuleContext ->
                   xModuleContextStatic.
                   pucBadAoBufferPointer,
                   0,
                   xPlcConfigService.xPlcConfigServiceData.ucServiceAnalogueOutputQuantity);
            //iBadModuleBuffUpdate();
            // ������� ��������� �� ������ ������ ������ ������� � �������.
            pucSource = &auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET];

            // ��������� ���� � ������ ������(��� ����� ������ �� ������ �� ������������).
            pucSource++;

// � �������� ������ �������� ������, ������ ������� - SP, OUT,
// ��������� �� �������� ������� axAnalogueOutputRegulatorContAv[�������� �� ����� ����������],
// ����� ��������� ���� �� � ������������. �� ����� �� �������, �� ��������� Modbus,
// ���������� �������� ������(HMI ������, ������� SCADA, �������������) �������� ������ ��� ����������� �� �������.
//  �� ������� ���� ����� ������� �������� � ������� �� SPI ���������� �������� � ������ ������������� ������-������.
// ����� ������ ����� ������ �� ������ ������ �������� ������ �� ��������� ���� ������-������.
// � ���� �� ������� ���� ������ � ������ ������ ������ ������� �������, �� ������� ��������� �� ���������.
//  �������, ����� ������ � ������ �������� �������, ���������� �� �������� � ���������� usAuxiliaryCounter(���������� � ���������),
// ��� �� ������������ � ������ ��������.
// � ���������� �� ������ ������ �� ������� ���� ������������.
//  ��� ��, ��� ������ ��� ����������� ����������� ������������ �������� ������, ��������� ������.
// ��� ��� ����� �������� ������ �������� ������ ���������� ���� ��� � �������,
// � ������ vDeviceControlModbusMessageHandler(), �������� - ����� ��������� ������ ���������� ������,
// �� �� ��� �����, �������� �������� ������ ����, ����� ����������������-���������������� �������� �������
// N - ���. �������� - N ���������� � ���������� usAuxiliaryCounter. � ����� ������� ����� ������ �������
// �������� � �������, ���������� usAuxiliaryCounter - ����������.

//-----------------------------------------------------------------------------------------------------
            // ������� �� ������ � ������� �������� SP � OUT � ������� ���������� ���������� ����������� �CONT_AV�.
            // ���� ������� - ��������� ���������� ���������� ����������� �CONT_AV�?
//            if ((pxModuleContext ->
//                    xModuleContextDinamic.
//                    ucCommandControl) ==
//                    (DEVICE_CONTROL_MTVI5_CONT_AV_WRITE |
//                     DEVICE_CONTROL_MTVI5_DATA_BASE_WRITE_BY_HMI |
//                     DEVICE_CONTROL_MTVI5_DATA_BASE_WRITE_BY_PROGRAMMER))
            if ((pxModuleContext ->
                    xModuleContextDinamic.
                    ucCommandControl) != 0)
            {
//                // ������� �������.
//                pxModuleContext ->
//                xModuleContextDinamic.
//                ucCommandControl = 0;
                // �������� ����������� OUT � ������� ���������� ���������� ����������� �CONT_AV� �� ����������.
                for (i = 0; i < nucRegulatorCounter; i++)
                {
                    // ������� � ��������� ����������� ������.
                    pucSource += 2;
                }
                // �������� ����������� SP � ������� ���������� ���������� ����������� �CONT_AV� �� ����������.
                for (i = 0; i < nucRegulatorCounter; i++)
                {
                    // ������� � ��������� ����������� ������.
                    pucSource += 2;
                }
            }
            else
            {
                // ������� �� ������ � ������� �������� OUT � ������� ���������� ���������� ����������� �CONT_AV�.
                for (i = 0; i < nucRegulatorCounter; i++)
                {
                    // ������� �������� OUT �� ������.
                    usData = (unsigned short)(*pucSource++);
                    usData |= ((unsigned short)(*pucSource++) << 8);
                    // ����������� �������� OUT
                    // �� ������� �������������� ����, �� float IEEE754.
                    fData = (float)(((float)PERCENT_100) * ((float)usData / (float)MTVI5_MAX_CODE));
                    // �������� � ������ ���������� ���������� ����������� �CONT_AV�.
                    pxMtvi5ContAvDataPointer[i].fOut = fData;
                }
                // ������� �� ������ � ������� �������� SP � ������� ���������� ���������� ����������� �CONT_AV�.
                for (i = 0; i < nucRegulatorCounter; i++)
                {
                    // ������� �������� ������ ����� ��� �� ��������������� ���� ������ ���������� ���������� ������.
                    fMinOfScale = pxAnalogueInputDescriptionWork[(pxAnalogueOutputRegulatorDescriptionWork[i].
                                  ucPvAddress) -
                                  CONVERT_NATURAL_NUMBER_TO_INTEGER].
                                  fMinOfScale;
                    // ������� �������� ����� ����� ��� �� ��������������� ���� ������ ���������� ���������� ������.
                    fMaxOfScale = pxAnalogueInputDescriptionWork[(pxAnalogueOutputRegulatorDescriptionWork[i].
                                  ucPvAddress) -
                                  CONVERT_NATURAL_NUMBER_TO_INTEGER].
                                  fMaxOfScale;
                    // ������� �������� SP �� ������.
                    usData = (unsigned short)(*pucSource++);
                    usData |= ((unsigned short)(*pucSource++) << 8);
                    // ����������� �������� SP
                    // �� ������� �������������� ����, �� float IEEE754.
                    fData = (float)((((float)usData * (fMaxOfScale - fMinOfScale)) / (float)MTVI5_MAX_CODE) + fMinOfScale);
                    // �������� � ������ ���������� ���������� ����������� �CONT_AV�.
                    pxMtvi5ContAvDataPointer[i].fSp = fData;
                }
            }

            // ������� �� ������ ����� ��������� ����������� - STAT:
            // ���� ������� - ��������� ���������� ���������� ����������� �CONT_AV�?
//            if ((pxModuleContext ->
//                    xModuleContextDinamic.
//                    ucCommandControl) ==
//                    (DEVICE_CONTROL_MTVI5_CONT_AV_WRITE |
//                     DEVICE_CONTROL_MTVI5_DATA_BASE_WRITE_BY_HMI |
//                     DEVICE_CONTROL_MTVI5_DATA_BASE_WRITE_BY_PROGRAMMER))
            if ((pxModuleContext ->
                    xModuleContextDinamic.
                    ucCommandControl) != 0)
            {
//                // ������� �������.
//                pxModuleContext ->
//                xModuleContextDinamic.
//                ucCommandControl = 0;
                // ������� �� ������ ����� ��������� ����������� - STAT:
                for (i = 0; i < nucRegulatorCounter; i++)
                {
                    uint8_t ui8Data;
                    ui8Data = *pucSource++;

//                    // �������� ����� ������.
//                    // ����� ������-���� ���������?
//                    if ((ui8Data & (0x01 << MTVI5_STAT_AUTO_OR_MANUAL_BIT)) !=
//                            (pxModuleContext ->
//                             xModuleContextStatic.
//                             pucContStWriteBufferPointer[i]) |=
//                                (0x01 << MTVI5_STAT_AUTO_OR_MANUAL_BIT))
//                    {
//                        // �������� ����� ������.
//                        // �� ������ ���� ������� - �������������� �����?
//                        if (ui8Data & (0x01 << MTVI5_STAT_AUTO_OR_MANUAL_BIT))
//                        {
//                            // ��������� ���� - �������������� �����.
//                            (pxModuleContext ->
//                             xModuleContextStatic.
//                             pucContStWriteBufferPointer[i]) |=
//                                 (0x01 << MTVI5_STAT_AUTO_OR_MANUAL_BIT);
//                        }
//                        else
//                        {
//                            // ��������� ���� - ������ �����.
//                            (pxModuleContext ->
//                             xModuleContextStatic.
//                             pucContStWriteBufferPointer[i]) &=
//                                 (~(0x01 << MTVI5_STAT_AUTO_OR_MANUAL_BIT));
//                        }
//                    }

                    // �������� ����� ������.
                    // �� ������ ���� ������� - �������������� �����?
                    if (ui8Data & (0x01 << MTVI5_STAT_AUTO_OR_MANUAL_BIT))
                    {
                        // ��������� ���� - �������������� �����.
                        (pxModuleContext ->
                         xModuleContextStatic.
                         pucContStWriteBufferPointer[i]) |=
                             (0x01 << MTVI5_STAT_AUTO_OR_MANUAL_BIT);
                    }
                    else
                    {
                        // ��������� ���� - ������ �����.
                        (pxModuleContext ->
                         xModuleContextStatic.
                         pucContStWriteBufferPointer[i]) &=
                             (~(0x01 << MTVI5_STAT_AUTO_OR_MANUAL_BIT));
                    }
                }
            }
            else
            {
                // ������� �� ������ ����� ��������� ����������� - STAT:
                for (i = 0; i < nucRegulatorCounter; i++)
                {
                    uint8_t ui8Data;
                    ui8Data = *pucSource++;
                    //
                    // ������� �� ������ ���� ��������� ����������� - STAT:
                    // D0-�/�; D1-��/�� ��; D2,3-����.2-�����.; D4-6-������; D7-����/����,
                    // � �������� ��� � ������ ��������� ����������� �CONT_ST�.
                    (pxModuleContext ->
                     xModuleContextStatic.
                     pucContStBufferPointer[i]) = ui8Data;

                    // �������� ����� ������.
                    // ����� ������-���� ���������?
                    if ((ui8Data & (0x01 << MTVI5_STAT_AUTO_OR_MANUAL_BIT)) !=
                            ((pxModuleContext ->
                              xModuleContextStatic.
                              pucContStWriteBufferPointer[i]) &
                             (0x01 << MTVI5_STAT_AUTO_OR_MANUAL_BIT)))
                    {
//                        // �������� ������� - ��������� ��������� ������-�������.
//                        pxModuleContext ->
//                        xModuleContextDinamic.
//                        ucCommandControl = DEVICE_CONTROL_MTVI5_CONT_ST_CHANGE;
//    printf("iModuleMtvi5DataExchange ui8Data %d\n\r", (ui8Data & (0x01 << MTVI5_STAT_AUTO_OR_MANUAL_BIT)));
//    printf("iModuleMtvi5DataExchange pucContStWriteBufferPointer %d\n\r", ((pxModuleContext ->
//                             xModuleContextStatic.
//                             pucContStWriteBufferPointer[i]) &
//                            (0x01 << MTVI5_STAT_AUTO_OR_MANUAL_BIT)));
//                        // �������� ����� ������.
//                        // �� ������ ���� ������� - �������������� �����?
//                        if (ui8Data & (0x01 << MTVI5_STAT_AUTO_OR_MANUAL_BIT))
//                        {
//                            // ��������� ���� - �������������� �����.
//                            (pxModuleContext ->
//                             xModuleContextStatic.
//                             pucContStWriteBufferPointer[i]) |=
//                                 (0x01 << MTVI5_STAT_AUTO_OR_MANUAL_BIT);
//                        }
//                        else
//                        {
//                            // ��������� ���� - ������ �����.
//                            (pxModuleContext ->
//                             xModuleContextStatic.
//                             pucContStWriteBufferPointer[i]) &=
//                                 (~(0x01 << MTVI5_STAT_AUTO_OR_MANUAL_BIT));
//                        }
                    }

                    // ���� ������������� ��� �������� ������?
                    if (ui8Data & (0x01 << MTVI5_STAT_DAC_ERROR_BIT))
                    {
//                        // ������ ������� �����������.
//                        *(pxModuleContext ->
//                          xModuleContextStatic.
//                          pucModuleBadStateBufferPointer) = BAD_MODULE_NOT_RESPONDED;
//                        // ������� ��� ������;
//                        (pxModuleContext ->
//                         xModuleContextDinamic.
//                         ucErrorCode) = MTVI5_OUT_DAC_ERROR;
                        // ��������� ���� ��������������� - ����� ������������.
                        pxModuleContext ->
                        xModuleContextStatic.
                        pucBadAoBufferPointer[i] = 1;
                        // �������� ��������� ������� ��� �� ����������������?
                        if(xCInternalModuleErrorEvent.EventOnIsNotRegistered(
                                    pxModuleContext ->
                                    xModuleContextStatic.
                                    ucModuleContextIndex,
                                    MTVI5_OUT_DAC_ERROR_OFFSET + i))
                        {
                            // �������������� �������� ��������� �������.
                            // ������ ��� ������ �� ������ - i.
                            CEvents::EventRegistration(
                                pxModuleContext ->
                                xModuleContextStatic.
                                ucModuleContextIndex,
                                (CEvents::HANDLED_EVENTS_INTERNAL_MODULES_BAD_TYPE |
//                                 CEvents::HANDLED_EVENTS_IS_POPUP |
                                 CEvents::HANDLED_EVENTS_IS_ARCHIVE),
                                MTVI5_OUT_DAC_ERROR_OFFSET + i,
                                "������ ���");
                        }
                    }
                    else
                    {
                        // ������� ���� ��������������� - ����� ����������.
                        pxModuleContext ->
                        xModuleContextStatic.
                        pucBadAoBufferPointer[i] = 0;
                        // ���������� ��������� ������� ��� �� ����������������?
                        if(xCInternalModuleErrorEvent.EventOffIsNotRegistered(
                                    pxModuleContext ->
                                    xModuleContextStatic.
                                    ucModuleContextIndex,
                                    MTVI5_OUT_DAC_ERROR_OFFSET + i))
                        {
                            // �������������� ���������� ��������� �������.
                            // ����� ��� ������ �� ������ - i.
                            CEvents::EventRegistration(
                                pxModuleContext ->
                                xModuleContextStatic.
                                ucModuleContextIndex,
                                (CEvents::HANDLED_EVENTS_INTERNAL_MODULES_BAD_TYPE |
                                 CEvents::HANDLED_EVENTS_IS_ARCHIVE),
                                MTVI5_OUT_DAC_ERROR_OFFSET + i,
                                "����� ���");
                        }
                    }
                }
            }

            // ������� ����� ����������.
            pxModuleContext ->
            xModuleContextDinamic.
            ucCommonIndex = 0;
            // ������� �������.
            pxModuleContext ->
            xModuleContextDinamic.
            ucCommandControl = 0;
            // ������� ������� ������ ����.
            // �� �������� � ���������� usAuxiliaryCounter ����������������-����������������
            // �������� ������� SP, OUT.
            pxModuleContext ->
            xModuleContextDinamic.
            usAuxiliaryCounter = 0;

////            cout << "pucContStBufferPointer" << endl;
////            unsigned char *pucSourceTemp;
////            pucSourceTemp = (unsigned char*)pxModuleContext ->
////                            xModuleContextStatic.
////                            pucContStBufferPointer;
//            cout << "pucContStBufferPointer" << endl;
//            unsigned char *pucSourceTemp;
//            pucSourceTemp = (unsigned char*)aucContStByteArray;
//            for(int i=0; i<12; )
//            {
//                for(int j=0; j<8; j++)
//                {
//                    cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
//                }
//                cout << endl;
//                i += 8;
//            }
        }
        else
        {
            cout << "MTVI5_DATA_EXCHANGE_COMMAND crc error" << endl;
            printf("iModuleMtvi5DataHandler ucModuleContextIndex %d\n\r", (pxModuleContext ->
                    xModuleContextStatic.
                    ucModuleContextIndex));



//            cout << "auiSpiTxBuffer[0]" << endl;
//            unsigned char *pucSourceTemp;
//            pucSourceTemp = (unsigned char*)&auiSpiTxBuffer[0];
//            for(int i=0; i<64; )
//            {
//                for(int j=0; j<8; j++)
//                {
//                    cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
//                }
//                cout << endl;
//                i += 8;
//            }
//
//            cout << "auiSpiRxBuffer[0]" << endl;
////            unsigned char *pucSourceTemp;
//            pucSourceTemp = (unsigned char*)&auiSpiRxBuffer[0];
//            for(int i=0; i<64; )
//            {
//                for(int j=0; j<8; j++)
//                {
//                    cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
//                }
//                cout << endl;
//                i += 8;
//            }


            // ������ ������� �����������?
            if ((pxModuleContext ->
                    xModuleContextDinamic.
                    nucBadAnswerCounter) == 0)
            {
                // ������ ������� �����������.
                *(pxModuleContext ->
                  xModuleContextStatic.
                  pucModuleBadStateBufferPointer) = BAD_MODULE_NOT_RESPONDED;

                // ������ �� �������� � ������� 60 ������?
                if ((pxModuleContext ->
                        xModuleContextDinamic.
                        usAuxiliaryCounter) >= BAD_MODULE_60_SECOND_CYCLE_COUNT_DEFAULT)
                {
                    // �������� ������� ������ ����5 - ������������ ������ ��� ����� ���� ������.
                    // ��������� OSF � ������� � ����� ������ɻ.
                    pxModuleContext ->
                    xModuleContextDinamic.
                    ucCommandControl = DEVICE_CONTROL_MTVI5_LOAD_OUTS_OSF;
                }
                else
                {
                    // �������� �������� �������� - "���������� ������� ����� � ������� ��� ������".
                    (pxModuleContext ->
                     xModuleContextDinamic.
                     usAuxiliaryCounter)++;
                }

                // ������� ��� ������;
                (pxModuleContext ->
                 xModuleContextDinamic.
                 ucErrorCode) = DATA_EXCHANGE_CRC_ERROR;

                // �������� ��������� ������� ��� �� ����������������?
                if(xCInternalModuleErrorEvent.EventOnIsNotRegistered(
                            pxModuleContext ->
                            xModuleContextStatic.
                            ucModuleContextIndex,
                            MTVI5_BAD_ANSWER_ERROR_OFFSET))
                {
                    // �������������� �������� ��������� �������.
                    // ������ ������ �������.
                    CEvents::EventRegistration(
                        pxModuleContext ->
                        xModuleContextStatic.
                        ucModuleContextIndex,
                        (CEvents::HANDLED_EVENTS_INTERNAL_MODULES_BAD_TYPE |
                         CEvents::HANDLED_EVENTS_IS_POPUP |
                         CEvents::HANDLED_EVENTS_IS_SOUND |
                         CEvents::HANDLED_EVENTS_IS_ARCHIVE),
                        MTVI5_BAD_ANSWER_ERROR_OFFSET,
                        "����. ������");
                }

                // ��������� ����� ��������������� - ������ ������������.
                memset(pxModuleContext ->
                       xModuleContextStatic.
                       pucBadAoBufferPointer,
                       1,
                       xPlcConfigService.xPlcConfigServiceData.ucServiceAnalogueOutputQuantity);
                //iBadModuleBuffUpdate();
            }
            else
            {
                (pxModuleContext ->
                 xModuleContextDinamic.
                 nucBadAnswerCounter)--;
            }
        }

//            cout << "iModuleMtvi5DataHandler pucContStBufferPointer" << endl;
////        unsigned char *pucSourceTemp;
//            pucSourceTemp = (unsigned char*)pxModuleContext ->
//                            xModuleContextStatic.
//                            pucContStBufferPointer;
//            for(int i=0; i<16; )
//            {
//                for(int j=0; j<8; j++)
//                {
//                    cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
//                }
//                cout << endl;
//                i += 8;
//            }
        break;
        // ������ �� ������.
    case MTVI5_DATA_NOT_READY:
        cout << "MTVI5_DATA_NOT_READY" << endl;
//        // ������ ������� �����������?
//        if ((pxModuleContext ->
//                xModuleContextDinamic.
//                nucBadAnswerCounter) == 0)
//        {
//            // ������ ������� �����������.
//            *(pxModuleContext ->
//              xModuleContextStatic.
//              pucModuleBadStateBufferPointer) = BAD_MODULE_NOT_RESPONDED;
//
//            // ������� ��� ������;
//            (pxModuleContext ->
//             xModuleContextDinamic.
//             ucErrorCode) = MTVI5_DATA_NOT_READY;
//
//            // ��������� ����� ��������������� - ������ ������������.
//            memset(pxModuleContext ->
//                   xModuleContextStatic.
//                   pucBadAoBufferPointer,
//                   1,
//                   xPlcConfigService.xPlcConfigServiceData.ucServiceAnalogueOutputQuantity);
//            //iBadModuleBuffUpdate();
//        }
//        else
//        {
//            (pxModuleContext ->
//             xModuleContextDinamic.
//             nucBadAnswerCounter)--;
//        }
        break;
        // ������ ���� ������.
    case MTVI5_DATA_BASE_ERROR:
        cout << "MTVI5_DATA_BASE_ERROR" << endl;
        // �������� ������� ������ � ����� ������ ���� ������ ����������� � ����������.
        pxModuleContext ->
        xModuleContextDinamic.
        ucFsmState = MTVI5_FSM_STATE_WRITE_REGULATOR_AND_PSP_DATABASE_REGULATOR;

        // �������� ��������� ������� ��� �� ����������������?
        if(xCInternalModuleErrorEvent.EventOnIsNotRegistered(
                    pxModuleContext ->
                    xModuleContextStatic.
                    ucModuleContextIndex,
                    MTVI5_DATA_BASE_ERROR_OFFSET))
        {
            // �������������� �������� ��������� �������.
            CEvents::EventRegistration(
                pxModuleContext ->
                xModuleContextStatic.
                ucModuleContextIndex,
                (CEvents::HANDLED_EVENTS_INTERNAL_MODULES_BAD_TYPE |
                 CEvents::HANDLED_EVENTS_IS_ARCHIVE),
                MTVI5_DATA_BASE_ERROR_OFFSET,
                "����. ���. �.");
        }

//        // ������ ������� �����������?
//        if ((pxModuleContext ->
//                xModuleContextDinamic.
//                nucBadAnswerCounter) == 0)
//        {
//            // ������ ������� �����������.
//            *(pxModuleContext ->
//              xModuleContextStatic.
//              pucModuleBadStateBufferPointer) = BAD_MODULE_NOT_RESPONDED;
//
//            // ������� ��� ������;
//            (pxModuleContext ->
//             xModuleContextDinamic.
//             ucErrorCode) = MTVI5_DATA_BASE_ERROR;
//
//            // ��������� ����� ��������������� - ������ ������������.
//            memset(pxModuleContext ->
//                   xModuleContextStatic.
//                   pucBadAoBufferPointer,
//                   1,
//                   xPlcConfigService.xPlcConfigServiceData.ucServiceAnalogueOutputQuantity);
//            //iBadModuleBuffUpdate();
//        }
//        else
//        {
//            (pxModuleContext ->
//             xModuleContextDinamic.
//             nucBadAnswerCounter)--;
//        }
        break;
        // ��������� ��������� �������� �������.
    case MTVI5_LOAD_OUTS:
        cout << "MTVI5_LOAD_OUTS" << endl;

        printf("iModuleMtvi5DataHandler ucModuleContextIndex %d\n\r", (pxModuleContext ->
                xModuleContextStatic.
                ucModuleContextIndex));


//            cout << "auiSpiRxBuffer[0]" << endl;
//            unsigned char *pucSourceTemp;
//            pucSourceTemp = (unsigned char*)&auiSpiRxBuffer[0];
//            for(int i=0; i<64; )
//            {
//                for(int j=0; j<8; j++)
//                {
//                    cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
//                }
//                cout << endl;
//                i += 8;
//            }

        usleep(1000);
        // (SPI_DATA_BYTE_OFFSET - 1) - ����� ������� "��������� �����" �� ��������� ������������� � ���� ���,
        // � ����� ���� ������.
        pucDestination = &auiSpiTxBuffer[SPI_DATA_BYTE_OFFSET - 1];
        // �������v �������� ������� - 50%.
        usData = (unsigned short)8192;

        *pucDestination++ = (unsigned char)usData;
        *pucDestination++ = (unsigned char)(usData >> 8);
        usData = (unsigned short)8192;
        *pucDestination++ = (unsigned char)usData;
        *pucDestination++ = (unsigned char)(usData >> 8);
        usData = (unsigned short)8192;
        *pucDestination++ = (unsigned char)usData;
        *pucDestination++ = (unsigned char)(usData >> 8);
        usData = (unsigned short)8192;
        *pucDestination++ = (unsigned char)usData;
        *pucDestination++ = (unsigned char)(usData >> 8);
        usData = (unsigned short)8192;
        *pucDestination++ = (unsigned char)usData;
        *pucDestination++ = (unsigned char)(usData >> 8);
        usData = (unsigned short)8192;
        *pucDestination++ = (unsigned char)usData;
        *pucDestination++ = (unsigned char)(usData >> 8);

        // ���� ������� - ������������ ������ ��� ����� ���� ������?
        if ((pxModuleContext ->
                xModuleContextDinamic.
                ucCommandControl) == DEVICE_CONTROL_MTVI5_LOAD_OUTS_OSF)
        {
            // ������� �������.
            pxModuleContext ->
            xModuleContextDinamic.
            ucCommandControl = 0;
            // ��������� OSF � ������� � ����� ������ɻ.
            *pucDestination++ = 0x01;
        }
        else
        {
            // ��������� ��������� �������� OUT.
            *pucDestination++ = 0x00;
        }

        pucSource = &auiSpiTxBuffer[SPI_DATA_BYTE_OFFSET - 1];
        usData = usCrcSummTwoByteCalculation(pucSource,
                                             (MTVI5_RESTORE_OUTS_BUFF_LENGTH +
                                              MTVI5_RESTORE_OUTS_FLAG_BYTE_QUANTITY));
        *pucDestination++ = (unsigned char)usData;
        *pucDestination++ = (unsigned char)(usData >> 8);

        auiSpiTxBuffer[0] = MTVI5_RESTORE_OUTS_COMMAND;
        xSpiCommunicationDevice.Exchange(pxModuleContext ->
                                         xModuleContextStatic.
                                         ucModuleAddress,
                                         auiSpiTxBuffer,
                                         auiSpiRxBuffer,
                                         // (SPI_PREAMBLE_LENGTH - 1) - ����� ������� "��������� �����" �� ��������� ������������� � ���� ���.
                                         // � ����� ���� ������.
                                         (SPI_PREAMBLE_LENGTH - 1) +
                                         MTVI5_RESTORE_OUTS_BUFF_LENGTH +
                                         MTVI5_RESTORE_OUTS_FLAG_BYTE_QUANTITY +
                                         TWO_BYTE_CRC_LENGTH +
                                         TAIL_ANSWER_LENGTH,
                                         MTVI5_EXCHANGE_SPEED_IN_HZ
                                        );

        // �������� ������� - ��������� ��������� ������-�������.
        pxModuleContext ->
        xModuleContextDinamic.
        ucCommandControl = DEVICE_CONTROL_MTVI5_CONT_ST_CHANGE;

//            cout << "auiSpiTxBuffer[0]" << endl;
//            unsigned char *pucSourceTemp;
//            pucSourceTemp = (unsigned char*)&auiSpiTxBuffer[0];
//            for(int i=0; i<64; )
//            {
//                for(int j=0; j<8; j++)
//                {
//                    cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
//                }
//                cout << endl;
//                i += 8;
//            }

//
//            cout << "auiSpiRxBuffer[0]" << endl;
////            unsigned char *pucSourceTemp;
//            pucSourceTemp = (unsigned char*)&auiSpiRxBuffer[0];
//            for(int i=0; i<64; )
//            {
//                for(int j=0; j<8; j++)
//                {
//                    cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
//                }
//                cout << endl;
//                i += 8;
//            }


        break;
        // ������ �� ������.
    case MTVI5_DAC_LINK_ERROR:
        cout << "MTVI5_DAC_LINK_ERROR" << endl;
//        printf("iModuleMtvi5DataHandler ucModuleContextIndex %d\n\r", (pxModuleContext ->
//                xModuleContextStatic.
//                ucModuleContextIndex));

        // ������ ������� �����������?
        if ((pxModuleContext ->
                xModuleContextDinamic.
                nucBadAnswerCounter) == 0)
        {
            // ������ ������� �����������.
            *(pxModuleContext ->
              xModuleContextStatic.
              pucModuleBadStateBufferPointer) = BAD_MODULE_NOT_RESPONDED;

            // �������� ������� - ��������� ��������� ������-�������.
            pxModuleContext ->
            xModuleContextDinamic.
            ucCommandControl = DEVICE_CONTROL_MTVI5_CONT_ST_CHANGE;

//            // ������� ��� ������;
//            (pxModuleContext ->
//             xModuleContextDinamic.
//             ucErrorCode) = MTVI5_DAC_LINK_ERROR;

            // �������� ��������� ������� ��� �� ����������������?
            if(xCInternalModuleErrorEvent.EventOnIsNotRegistered(
                        pxModuleContext ->
                        xModuleContextStatic.
                        ucModuleContextIndex,
                        MTVI5_DAC_LINK_ERROR_OFFSET))
            {
//        printf("iModuleMtvi5DataHandler nucBadAnswerCounter %d\n\r", (pxModuleContext ->
//                xModuleContextDinamic.
//                nucBadAnswerCounter));
                // �������������� �������� ��������� �������.
                // ������ ������� ���.
                CEvents::EventRegistration(
                    pxModuleContext ->
                    xModuleContextStatic.
                    ucModuleContextIndex,
                    (CEvents::HANDLED_EVENTS_INTERNAL_MODULES_BAD_TYPE |
                     CEvents::HANDLED_EVENTS_IS_POPUP |
                     CEvents::HANDLED_EVENTS_IS_ARCHIVE),
                    MTVI5_DAC_LINK_ERROR_OFFSET,
                    "����. �. ���");
            }

            // ��������� ����� ��������������� - ������ ������������.
            memset(pxModuleContext ->
                   xModuleContextStatic.
                   pucBadAoBufferPointer,
                   1,
                   xPlcConfigService.xPlcConfigServiceData.ucServiceAnalogueOutputQuantity);

            //iBadModuleBuffUpdate();
        }
        else
        {
            (pxModuleContext ->
             xModuleContextDinamic.
             nucBadAnswerCounter)--;
        }
        break;
        // ������������ ����� - ������.
    default:
//        cout << "MTVI5 ERROR" << endl;
//
//        printf("iModuleMtvi5DataHandler ucModuleContextIndex %d\n\r", (pxModuleContext ->
//                xModuleContextStatic.
//                ucModuleContextIndex));
////
////            cout << "auiSpiRxBuffer[0]" << endl;
////            unsigned char *pucSourceTemp;
////            pucSourceTemp = (unsigned char*)&auiSpiRxBuffer[0];
////            for(int i=0; i<64; )
////            {
////                for(int j=0; j<8; j++)
////                {
////                    cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
////                }
////                cout << endl;
////                i += 8;
////            }

        // ������ ������� �����������?
        if ((pxModuleContext ->
                xModuleContextDinamic.
                nucBadAnswerCounter) == 0)
        {
            // ������ ������� �����������.
            *(pxModuleContext ->
              xModuleContextStatic.
              pucModuleBadStateBufferPointer) = BAD_MODULE_NOT_RESPONDED;

            // �������� ������� - ��������� ��������� ������-�������.
            pxModuleContext ->
            xModuleContextDinamic.
            ucCommandControl = DEVICE_CONTROL_MTVI5_CONT_ST_CHANGE;

            // ������ �� �������� � ������� 60 ������?
            if ((pxModuleContext ->
                    xModuleContextDinamic.
                    usAuxiliaryCounter) >= BAD_MODULE_60_SECOND_CYCLE_COUNT_DEFAULT)
            {
                // �������� ������� ������ ����5 - ������������ ������ ��� ����� ���� ������.
                // ��������� OSF � ������� � ����� ������ɻ.
                pxModuleContext ->
                xModuleContextDinamic.
                ucCommandControl = DEVICE_CONTROL_MTVI5_LOAD_OUTS_OSF;
            }
            else
            {
                // �������� �������� �������� - "���������� ������� ����� � ������� ��� ������".
                (pxModuleContext ->
                 xModuleContextDinamic.
                 usAuxiliaryCounter)++;
            }

//            // ������� ��� ������;
//            (pxModuleContext ->
//             xModuleContextDinamic.
//             ucErrorCode) = BAD_MODULE_NOT_RESPONDED;

            // �������� ��������� ������� ��� �� ����������������?
            if(xCInternalModuleErrorEvent.EventOnIsNotRegistered(
                        pxModuleContext ->
                        xModuleContextStatic.
                        ucModuleContextIndex,
                        MTVI5_BAD_ANSWER_ERROR_OFFSET))
            {
                // �������������� �������� ��������� �������.
                // ������ ������ �������.
                CEvents::EventRegistration(
                    pxModuleContext ->
                    xModuleContextStatic.
                    ucModuleContextIndex,
                    (CEvents::HANDLED_EVENTS_INTERNAL_MODULES_BAD_TYPE |
                     CEvents::HANDLED_EVENTS_IS_POPUP |
                     CEvents::HANDLED_EVENTS_IS_SOUND |
                     CEvents::HANDLED_EVENTS_IS_ARCHIVE),
                    MTVI5_BAD_ANSWER_ERROR_OFFSET,
                    "����. ������");
            }


            // ��������� ����� ��������������� - ������ ������������.
            memset(pxModuleContext ->
                   xModuleContextStatic.
                   pucBadAoBufferPointer,
                   1,
                   xPlcConfigService.xPlcConfigServiceData.ucServiceAnalogueOutputQuantity);
            //iBadModuleBuffUpdate();
        }
        else
        {
            // �������� �������� �������� - "���������� ������� ����� � ������� ��� ������".
            (pxModuleContext ->
             xModuleContextDinamic.
             nucBadAnswerCounter)--;
        }
        break;
    };
//    // ������� ����� ����������.
//    pxModuleContext ->
//    xModuleContextDinamic.
//    ucCommonIndex = 0;
//    // ������� �������.
//    pxModuleContext ->
//    xModuleContextDinamic.
//    ucCommandControl = 0;
//    // ������� ������� ������ ����.
//    // �� �������� � ���������� usAuxiliaryCounter ����������������-����������������
//    // �������� ������� SP, OUT.
//    pxModuleContext ->
//    xModuleContextDinamic.
//    usAuxiliaryCounter = 0;

//    }
//    // �������������� �������� ������� ��������� � ������.
//    (pxModuleContext ->
//     xModuleContextDinamic.
//     nucLoopSkipCounter)++;
    return 0;
}

//-----------------------------------------------------------------------------------------------------
// ��������, ���������� ���� ������ ������ ��������� � ����� � ������ �������?
// ���� �� ���������, ������� ���� �� ������ � ������.
int iModuleMtvi5RegulatorPlcDataBaseCheck(TModuleContext *pxModuleContext)
{
//    unsigned short usData;
//    int i;
//    unsigned char *pucSource;
//    unsigned char *pucDestination;
//    unsigned char aucTempArray[PLC_DATA_BASE_BLOCK_LENGTH];
//
//    memset(&aucTempArray[0],
//           0,
//           sizeof(aucTempArray));
//
//    xSpiCommunicationDevice.ChipSelectAddressSet(pxModuleContext ->
//                             xModuleContextStatic.
//                             ucModuleAddress);
//
//    auiSpiTxBuffer[0] = MTVI5_GET_REGULATOR_DATA_BASE_COMMAND;
//    // ���������� ������� ������.
//    xSpiCommunicationDevice.Exchange(iSpiDeviceHandle,
//                 auiSpiTxBuffer,
//                 auiSpiRxBuffer,
//                 1,
//                 LOW_SPEED_IN_HZ);
//    usleep(100);
//    auiSpiTxBuffer[0] = 0;
//    // ���������� ������� ������.
//    xSpiCommunicationDevice.Exchange(iSpiDeviceHandle,
//                 auiSpiTxBuffer,
//                 auiSpiRxBuffer,
//                 1,
//                 LOW_SPEED_IN_HZ);
//    usleep(100);
//
//    //  (SPI_COMMAND_BYTE_OFFSET - 1) - �� ���� ���� ������, ������, ��� ����� ���� ������� �������� ��������� �����������.
//    if (auiSpiRxBuffer[(0)] != MTVI5_GET_REGULATOR_DATA_BASE_COMMAND) // if echo answer is ok.
//    {
//        cout << "MTVI5_GET_REGULATOR_DATA_BASE_COMMAND no" << endl;
//        return -1;
//    }
//
//    // ������-������ ������.
//    xSpiCommunicationDevice.Exchange(iSpiDeviceHandle,
//                 auiSpiTxBuffer,
//                 auiSpiRxBuffer,
//// ���������� ���� ��� ������ ������� � ������� �� SPI: (���� ���� - �������) + (������ ���� - "���"(����� ������)) +
//// (����� �������������-������������ ���������(�������� ����� ����� ���� ������)) + (���� ��� ��� ����� ����������� �����).
//// SPI_PREAMBLE_LENGTH = 2, ������ ���� - �������, ������ - �����.
////  � ������� ������ ������� �� SPI xSpiCommunicationDevice.Exchange() � ����� ������ ����� ���������� - ������� � ������.
//// ����� �������� ������� ������ ����������� ��������� ����� �� ������������ ������,
//// ����� ���������������� ������ � ���� ���������. ���� ����� �� ������������ ������ ������������ ������,
//// �� ������ ��������� ��������� ���� - �������� ������ ����� �����������.
//// ����� ����� �� ���������, ��� ������ ������� � "���������� ��������", ��������� ������� � ��������-���� ������
//// ���������� �� ��� ����������.
////  (SPI_PREAMBLE_LENGTH - 1) - �� ���� ���� ������, ������, ��� ����� ���� ������� �������� ��������� �����������.
////                 (SPI_PREAMBLE_LENGTH - 1) +
//                 CURRENT_OUTPUT_MODULE_REGULATOR_DATA_BASE_BLOCK_LENGTH +
//                 TWO_BYTE_CRC_LENGTH,
//                 LOW_SPEED_IN_HZ);
//    CGpio::ClearPin(SPI_CHIP_ENABLE_PIN_PORT, SPI_CHIP_ENABLE_PIN);
////    //  (SPI_COMMAND_BYTE_OFFSET - 1) - �� ���� ���� ������, ������, ��� ����� ���� ������� �������� ��������� �����������.
////    if (auiSpiRxBuffer[(SPI_COMMAND_BYTE_OFFSET - 1)] == MTVI5_GET_REGULATOR_DATA_BASE_COMMAND) // if echo answer is ok.
////    {
//    //  (SPI_DATA_BYTE_OFFSET - 1) - �� ���� ���� ������, ������, ��� ����� ���� ������� �������� ��������� �����������.
//    if (iCrcSummTwoByteCompare(&auiSpiRxBuffer[(SPI_DATA_BYTE_OFFSET - 2)],
//                               CURRENT_OUTPUT_MODULE_REGULATOR_DATA_BASE_BLOCK_LENGTH) > 0)
//    {
//        pucSource = &auiSpiRxBuffer[(SPI_DATA_BYTE_OFFSET - 2)];
//        // ������� ��������� �� ���� � ���� ������.
//        pucDestination = (unsigned char*)(pxModuleContext ->
//                                          xModuleContextStatic.
//                                          pucDataBasePointer);
//        // ���� ������ ������ � ������� �� ���������?
//        if (memcmp((const void*)pucDestination,
//                   (const void*)pucSource,
//                   CURRENT_OUTPUT_MODULE_REGULATOR_DATA_BASE_BLOCK_LENGTH) != 0)
//        {
//            cout << "iModuleMtvi5RegulatorPlcDataBaseCheck pucSource" << endl;
//            unsigned char *pucSourceTemp;
//            pucSourceTemp = (unsigned char*)pucSource;
//            for(int i=0; i<256; )
//            {
//                for(int j=0; j<8; j++)
//                {
//                    cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
//                }
//                cout << endl;
//                i += 8;
//            }
//            cout << "iModuleMtvi5RegulatorPlcDataBaseCheck pucDestination" << endl;
////        unsigned char *pucSourceTemp;
//            pucSourceTemp = (unsigned char*)pucDestination;
//            for(int i=0; i<256; )
//            {
//                for(int j=0; j<8; j++)
//                {
//                    cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
//                }
//                cout << endl;
//                i += 8;
//            }
//            // ���� ������ ������ � ������� �� ���������.
//            // ��������� ���� ������ ������ � ������.
//            memcpy(pucDestination,
//                   pucSource,
//                   CURRENT_OUTPUT_MODULE_REGULATOR_DATA_BASE_BLOCK_LENGTH);
//
////             cout << "iModuleMtvi5RegulatorPlcDataBaseCheck pucDestination 2" << endl;
//////        unsigned char *pucSourceTemp;
////        pucSourceTemp = (unsigned char*)pucDestination;
////        for(int i=0; i<256; )
////        {
////            for(int j=0; j<8; j++)
////            {
////                cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
////            }
////            cout << endl;
////            i += 8;
////        }
//            cout << "iModuleMtvi5RegulatorPlcDataBaseCheck NO" << endl;
//            return -1;
//        }
//        else
//        {
//            cout << "iModuleMtvi5RegulatorPlcDataBaseCheck OK" << endl;
//            return 0;
//        }
//    }
//    cout << "iModuleMtvi5RegulatorPlcDataBaseCheck iCrcSummTwoByteCompare error" << endl;
//
//            cout << "iModuleMtvi5RegulatorPlcDataBaseCheck pucDestination" << endl;
////        unsigned char *pucSourceTemp;
//            pucSourceTemp = (unsigned char*)pucDestination;
//            for(int i=0; i<256; )
//            {
//                for(int j=0; j<8; j++)
//                {
//                    cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
//                }
//                cout << endl;
//                i += 8;
//            }
////    }
//    return -1;



    unsigned short usData;
    int i;
    unsigned char *pucSource;
    unsigned char *pucDestination;
    unsigned char auiSpiTxBuffer[TX_RX_BUFF_SIZE];
    unsigned char auiSpiRxBuffer[TX_RX_BUFF_SIZE];

//    usleep(1200);

    auiSpiTxBuffer[0] = MTVI5_GET_REGULATOR_DATA_BASE_COMMAND;

    // ������-������ ������.
    xSpiCommunicationDevice.Exchange(pxModuleContext ->
                                     xModuleContextStatic.
                                     ucModuleAddress,
                                     auiSpiTxBuffer,
                                     auiSpiRxBuffer,
                                     (SPI_PREAMBLE_LENGTH +
                                      CURRENT_OUTPUT_MODULE_REGULATOR_DATA_BASE_BLOCK_LENGTH +
                                      TWO_BYTE_CRC_LENGTH),
                                     MTVI5_EXCHANGE_SPEED_IN_HZ);//LOW_SPEED_IN_HZ

    pucSource = &auiSpiRxBuffer[(SPI_DATA_BYTE_OFFSET)];
    // ������� ��������� �� ���� � ���� ������.
    pucDestination = (unsigned char*)(pxModuleContext ->
                                      xModuleContextStatic.
                                      pucDataBasePointer);

//    cout << "iModuleMtvi5RegulatorPlcDataBaseCheck pucSource" << endl;
//    unsigned char *pucSourceTemp;
//    pucSourceTemp = (unsigned char*)pucSource;
//    for(int i=0; i<256; )
//    {
//        for(int j=0; j<8; j++)
//        {
//            cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
//        }
//        cout << endl;
//        i += 8;
//    }
//    cout << "iModuleMtvi5RegulatorPlcDataBaseCheck pucDestination" << endl;
////        unsigned char *pucSourceTemp;
//    pucSourceTemp = (unsigned char*)pucDestination;
//    for(int i=0; i<256; )
//    {
//        for(int j=0; j<8; j++)
//        {
//            cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
//        }
//        cout << endl;
//        i += 8;
//    }

    if (iCrcSummTwoByteCompare(&auiSpiRxBuffer[(SPI_DATA_BYTE_OFFSET)],
                               CURRENT_OUTPUT_MODULE_REGULATOR_DATA_BASE_BLOCK_LENGTH) > 0)
    {
//        pucSource = &auiSpiRxBuffer[(SPI_DATA_BYTE_OFFSET)];
//        // ������� ��������� �� ���� � ���� ������.
//        pucDestination = (unsigned char*)(pxModuleContext ->
//                                          xModuleContextStatic.
//                                          pucDataBasePointer);
        // ���� ������ ������ � ������� �� ���������?
        if (memcmp((const void*)pucDestination,
                   (const void*)pucSource,
                   CURRENT_OUTPUT_MODULE_REGULATOR_DATA_BASE_BLOCK_LENGTH) != 0)
        {
            // ���� ������ ������ � ������� �� ���������.
            // ��������� ���� ������ ������ � ������.
            memcpy(pucDestination,
                   pucSource,
                   CURRENT_OUTPUT_MODULE_REGULATOR_DATA_BASE_BLOCK_LENGTH);

//             cout << "iModuleMtvi5RegulatorPlcDataBaseCheck pucDestination 2" << endl;
////        unsigned char *pucSourceTemp;
//        pucSourceTemp = (unsigned char*)pucDestination;
//        for(int i=0; i<256; )
//        {
//            for(int j=0; j<8; j++)
//            {
//                cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
//            }
//            cout << endl;
//            i += 8;
//        }
            cout << "iModuleMtvi5RegulatorPlcDataBaseCheck NO" << endl;
            return -1;
        }
        else
        {
            cout << "iModuleMtvi5RegulatorPlcDataBaseCheck OK" << endl;
            return 0;
        }
    }
    cout << "iModuleMtvi5RegulatorPlcDataBaseCheck iCrcSummTwoByteCompare error" << endl;

//    cout << "iModuleMtvi5RegulatorPlcDataBaseCheck ucSpiRxBuffer[(0)]" << endl;
////        unsigned char *pucSourceTemp;
//    pucSourceTemp = (unsigned char*)&auiSpiRxBuffer[(0)];
//    for(int i=0; i<256; )
//    {
//        for(int j=0; j<8; j++)
//        {
//            cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
//        }
//        cout << endl;
//        i += 8;
//    }
    return -1;
}

//-----------------------------------------------------------------------------------------------------
// ���������� ���������� ���� ������ ������ � ����� � ������ �������.
// ���� �� ���������, ������� ���� �� ������� � ������.
int iModuleMtvi5RegulatorModuleDataBaseCheck(TModuleContext *pxModuleContext)
{
    unsigned short usData;
    int i;
    unsigned char *pucSource;
    unsigned char *pucDestination;
//    unsigned char aucTempArray[PLC_DATA_BASE_BLOCK_LENGTH];
    unsigned char auiSpiTxBuffer[TX_RX_BUFF_SIZE];
    unsigned char auiSpiRxBuffer[TX_RX_BUFF_SIZE];

//    memset(&aucTempArray[0],
//           0,
//           sizeof(aucTempArray));

//    usleep(600);

    auiSpiTxBuffer[0] = MTVI5_GET_REGULATOR_DATA_BASE_COMMAND;
    // ���������� ������� ������.
    xSpiCommunicationDevice.Exchange(pxModuleContext ->
                                     xModuleContextStatic.
                                     ucModuleAddress,
                                     auiSpiTxBuffer,
                                     auiSpiRxBuffer,
                                     1,
                                     MTVI5_EXCHANGE_SPEED_IN_HZ);//LOW_SPEED_IN_HZ
    usleep(100);
    auiSpiTxBuffer[0] = 0;
    // ���������� ������� ������.
    xSpiCommunicationDevice.Exchange(pxModuleContext ->
                                     xModuleContextStatic.
                                     ucModuleAddress,
                                     auiSpiTxBuffer,
                                     auiSpiRxBuffer,
                                     1,
                                     MTVI5_EXCHANGE_SPEED_IN_HZ);//LOW_SPEED_IN_HZ
    usleep(100);

    //  (SPI_COMMAND_BYTE_OFFSET - 1) - �� ���� ���� ������, ������, ��� ����� ���� ������� �������� ��������� �����������.
    if (auiSpiRxBuffer[(0)] != MTVI5_GET_REGULATOR_DATA_BASE_COMMAND) // if echo answer is ok.
    {
        cout << "MTVI5_GET_REGULATOR_DATA_BASE_COMMAND no" << endl;
        return -1;
    }

    auiSpiTxBuffer[0] = 0;
    // ������-������ ������.
    xSpiCommunicationDevice.Exchange(pxModuleContext ->
                                     xModuleContextStatic.
                                     ucModuleAddress,
                                     auiSpiTxBuffer,
                                     auiSpiRxBuffer,
// ���������� ���� ��� ������ ������� � ������� �� SPI: (���� ���� - �������) + (������ ���� - "���"(����� ������)) +
// (����� �������������-������������ ���������(�������� ����� ����� ���� ������)) + (���� ��� ��� ����� ����������� �����).
// SPI_PREAMBLE_LENGTH = 2, ������ ���� - �������, ������ - �����.
//  � ������� ������ ������� �� SPI xSpiCommunicationDevice.Exchange() � ����� ������ ����� ���������� - ������� � ������.
// ����� �������� ������� ������ ����������� ��������� ����� �� ������������ ������,
// ����� ���������������� ������ � ���� ���������. ���� ����� �� ������������ ������ ������������ ������,
// �� ������ ��������� ��������� ���� - �������� ������ ����� �����������.
// ����� ����� �� ���������, ��� ������ ������� � "���������� ��������", ��������� ������� � ��������-���� ������
// ���������� �� ��� ����������.
//  (SPI_PREAMBLE_LENGTH - 1) - �� ���� ���� ������, ������, ��� ����� ���� ������� �������� ��������� �����������.
                                     //(SPI_PREAMBLE_LENGTH - 2) +
                                     CURRENT_OUTPUT_MODULE_REGULATOR_DATA_BASE_BLOCK_LENGTH +
                                     TWO_BYTE_CRC_LENGTH,
                                     MTVI5_EXCHANGE_SPEED_IN_HZ);//LOW_SPEED_IN_HZ
//    //  (SPI_COMMAND_BYTE_OFFSET - 1) - �� ���� ���� ������, ������, ��� ����� ���� ������� �������� ��������� �����������.
//    if (auiSpiRxBuffer[(SPI_COMMAND_BYTE_OFFSET - 1)] == MTVI5_GET_REGULATOR_DATA_BASE_COMMAND) // if echo answer is ok.
//    {
    //  (SPI_DATA_BYTE_OFFSET - 1) - �� ���� ���� ������, ������, ��� ����� ���� ������� �������� ��������� �����������.
    if (iCrcSummTwoByteCompare(&auiSpiRxBuffer[(SPI_DATA_BYTE_OFFSET - 2)],
                               CURRENT_OUTPUT_MODULE_REGULATOR_DATA_BASE_BLOCK_LENGTH) > 0)
    {
        pucSource = &auiSpiRxBuffer[(SPI_DATA_BYTE_OFFSET - 2)];
        // ������� ��������� �� ���� � ���� ������.
        pucDestination = (unsigned char*)(pxModuleContext ->
                                          xModuleContextStatic.
                                          pucDataBasePointer);
        // ���� ������ ������ � ������� �� ���������?
        if (memcmp((const void*)pucDestination,
                   (const void*)pucSource,
                   CURRENT_OUTPUT_MODULE_REGULATOR_DATA_BASE_BLOCK_LENGTH) != 0)
        {
            // ���� ������ ������? ��� ����������� ����� �� ���������?
            if ((iBufferIsZero(pucDestination,
                               (PLC_DATA_BASE_BLOCK_LENGTH -
                                TWO_BYTE_CRC_LENGTH))) ||
                    (iCrc16Check((pxModuleContext ->
                                  xModuleContextStatic.
                                  pucDataBasePointer),
                                 (PLC_DATA_BASE_BLOCK_LENGTH -
                                  TWO_BYTE_CRC_LENGTH))) == -1)
            {
                // ���� ������ ������� ������ ��� ����������� ����� �� ���������.
                // �� ����� ���������� ���� ������ �� ������� � ������.
                cout << "Data base is empty or damaged." << endl;
            }
            else
            {
                // ���� ������ ������ � ������� �� ���������.
                // �������� � ������ ������ ���� ������ ����������� �� �������.
                iModuleMtvi5RegulatorDataBaseBlockWrite(pxModuleContext);
                cout << "New data base saved in module" << endl;
            }
            cout << "iModuleMtvi5RegulatorModuleDataBaseCheck NO" << endl;
            return -1;
        }
        else
        {
            cout << "iModuleMtvi5RegulatorModuleDataBaseCheck OK" << endl;
            return 0;
        }
    }
    cout << "iModuleMtvi5RegulatorModuleDataBaseCheck crc no" << endl;
    return -1;
//    }
//    cout << "MTVI5_GET_REGULATOR_DATA_BASE_COMMAND no" << endl;
//    return -1;




//    unsigned short usData;
//    int i;
//    unsigned char *pucSource;
//    unsigned char *pucDestination;
//
//    xSpiCommunicationDevice.ChipSelectAddressSet(pxModuleContext ->
//                             xModuleContextStatic.
//                             ucModuleAddress);
//
//    auiSpiTxBuffer[0] = MTVI5_GET_REGULATOR_DATA_BASE_COMMAND;
//    // ������-������ ������.
//    xSpiCommunicationDevice.Exchange(iSpiDeviceHandle,
//                 auiSpiTxBuffer,
//                 auiSpiRxBuffer,
//                 (SPI_PREAMBLE_LENGTH +
//                  CURRENT_OUTPUT_MODULE_REGULATOR_DATA_BASE_BLOCK_LENGTH +
//                  TWO_BYTE_CRC_LENGTH),
//                 LOW_SPEED_IN_HZ);
//    CGpio::ClearPin(SPI_CHIP_ENABLE_PIN_PORT, SPI_CHIP_ENABLE_PIN);
//
//    if (iCrcSummTwoByteCompare(&auiSpiRxBuffer[(SPI_DATA_BYTE_OFFSET)],
//                               CURRENT_OUTPUT_MODULE_REGULATOR_DATA_BASE_BLOCK_LENGTH) > 0)
//    {
//        pucSource = &auiSpiRxBuffer[(SPI_DATA_BYTE_OFFSET)];
//        // ������� ��������� �� ���� � ���� ������.
//        pucDestination = (unsigned char*)(pxModuleContext ->
//                                          xModuleContextStatic.
//                                          pucDataBasePointer);
//        // ���� ������ ������ � ������� �� ���������?
//        if (memcmp((const void*)pucDestination,
//                   (const void*)pucSource,
//                   CURRENT_OUTPUT_MODULE_REGULATOR_DATA_BASE_BLOCK_LENGTH) != 0)
//        {
//            // ���� ������ ������? ��� ����������� ����� �� ���������?
//            if ((iBufferIsZero(pucDestination,
//                               (PLC_DATA_BASE_BLOCK_LENGTH -
//                                TWO_BYTE_CRC_LENGTH))) ||
//                    (iCrc16Check((pxModuleContext ->
//                                  xModuleContextStatic.
//                                  pucDataBasePointer),
//                                 (PLC_DATA_BASE_BLOCK_LENGTH -
//                                  TWO_BYTE_CRC_LENGTH))) == -1)
//            {
//                // ���� ������ ������� ������ ��� ����������� ����� �� ���������.
//                // �� ����� ���������� ���� ������ �� ������� � ������.
//                cout << "Data base is empty or damaged." << endl;
//            }
//            else
//            {
//                // ���� ������ ������ � ������� �� ���������.
//                // �������� � ������ ������ ���� ������ ����������� �� �������.
//                iModuleMtvi5RegulatorDataBaseBlockWrite(pxModuleContext);
//                cout << "New data base saved in module" << endl;
//            }
//            cout << "iModuleMtvi5RegulatorModuleDataBaseCheck NO" << endl;
//            return -1;
//        }
//        else
//        {
//            cout << "iModuleMtvi5RegulatorModuleDataBaseCheck OK" << endl;
//            return 0;
//        }
//    }
//    cout << "iModuleMtvi5RegulatorModuleDataBaseCheck crc no" << endl;
//    return -1;
}

//-----------------------------------------------------------------------------------------------------
// ���������� ���� ������ �� RAM ������� � EEPROM ������.
int iModuleMtvi5RegulatorDataBaseBlockWrite(TModuleContext *pxModuleContext)
{
    unsigned short usData;
    int i;
    unsigned char *pucSource;
    unsigned char *pucDestination;
    unsigned char auiSpiTxBuffer[TX_RX_BUFF_SIZE];
    unsigned char auiSpiRxBuffer[TX_RX_BUFF_SIZE];

    // ������� ��������� �� ���� ���� ������, �������� �� Modbus �� ��������� �����.
    pucSource =
        ((pxModuleContext ->
          xModuleContextStatic.
          pucDataBasePointer) +
         (pxModuleContext ->
          xModuleContextStatic.
          ucDataBaseDataOffset));

    // ��������� � ����� TX SPI ���� ���� ������.
    memcpy(&auiSpiTxBuffer[SPI_DATA_BYTE_OFFSET],
           pucSource,
           CURRENT_OUTPUT_MODULE_REGULATOR_DATA_BASE_BLOCK_LENGTH);
    usData = usCrcSummTwoByteCalculation(pucSource,
                                         CURRENT_OUTPUT_MODULE_REGULATOR_DATA_BASE_BLOCK_LENGTH);
    // ��������� � ������ �� ����� CRC.
    pucDestination = &auiSpiTxBuffer[SPI_DATA_BYTE_OFFSET + CURRENT_OUTPUT_MODULE_REGULATOR_DATA_BASE_BLOCK_LENGTH];
    *pucDestination++ = (unsigned char)usData;
    *pucDestination = (unsigned char)(usData >> 8);

    auiSpiTxBuffer[0] = MTVI5_SET_REGULATOR_DATA_BASE_COMMAND;
//    usleep(600);
    xSpiCommunicationDevice.Exchange(pxModuleContext ->
                                     xModuleContextStatic.
                                     ucModuleAddress,
                                     auiSpiTxBuffer,
                                     auiSpiRxBuffer,
                                     SPI_PREAMBLE_LENGTH +
                                     CURRENT_OUTPUT_MODULE_REGULATOR_DATA_BASE_BLOCK_LENGTH +
                                     TWO_BYTE_CRC_LENGTH +
                                     TAIL_ANSWER_LENGTH,
                                     MTVI5_EXCHANGE_SPEED_IN_HZ);//LOW_SPEED_IN_HZ

    // ���� ������������� ������ ���� ������ � EEPROM ������?
    if ((auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET +
                        CURRENT_OUTPUT_MODULE_REGULATOR_DATA_BASE_BLOCK_LENGTH +
                        TWO_BYTE_CRC_LENGTH]) ==
            DATA_EXCHANGE_OK)
    {
//        usleep(1500000);
        // ���� ������������� ������ ���� ������ � EEPROM ������.
        cout << "iModuleMtvi5RegulatorDataBaseBlockWrite OK" << endl;
//        // ��������� ���� ������ � �������������?
//        if (pxModuleContext ->
//                xModuleContextDinamic.
//                ucCommandControl == DEVICE_CONTROL_MTVI5_DATA_BASE_WRITE_BY_PROGRAMMER)
//        {
//            // ���� ������������� ������ ���� ������ � EEPROM ������?
//            if ((iModuleMtvi5PspDataBaseBlockWrite(pxModuleContext)) != -1)
//            {
////                usleep(1500000);
//                return 0;
//            }
//            else
//            {
//                return -1;
//            }
//        }
        return 0;
    }
    else
    {
        cout << "iModuleMtvi5RegulatorDataBaseBlockWrite NO" << endl;
        return -1;
    }
}

//-----------------------------------------------------------------------------------------------------
int iModuleMtvi5PspDataBaseBlockWrite(TModuleContext *pxModuleContext)
{
    unsigned short usData;
    int i;
    unsigned char *pucSource;
    unsigned char *pucDestination;
    unsigned char auiSpiTxBuffer[TX_RX_BUFF_SIZE];
    unsigned char auiSpiRxBuffer[TX_RX_BUFF_SIZE];

    // ������� ��������� �� ���� ���� ������, �������� �� Modbus �� ��������� �����.
//    pucSource =
//        ((pxModuleContext ->
//          xModuleContextStatic.
//          pucDataBasePointer) +
//         (pxModuleContext ->
//          xModuleContextStatic.
//          ucDataBaseDataOffset));

//    memcpy(&auiSpiTxBuffer[SPI_DATA_BYTE_OFFSET],
//           pucSource,
//           CURRENT_OUTPUT_MODULE_PSP_DATA_BASE_BLOCK_LENGTH);

    pucSource = &auiSpiTxBuffer[SPI_DATA_BYTE_OFFSET];
    memset(pucSource,
           0,
           CURRENT_OUTPUT_MODULE_PSP_DATA_BASE_BLOCK_LENGTH);
    usData = usCrcSummTwoByteCalculation(pucSource,
                                         CURRENT_OUTPUT_MODULE_PSP_DATA_BASE_BLOCK_LENGTH);
    pucDestination = &auiSpiTxBuffer[SPI_DATA_BYTE_OFFSET + CURRENT_OUTPUT_MODULE_PSP_DATA_BASE_BLOCK_LENGTH];
    *pucDestination++ = (unsigned char)usData;
    *pucDestination = (unsigned char)(usData >> 8);

    auiSpiTxBuffer[0] = MTVI5_SET_PSP_DATA_BASE_COMMAND;
//    usleep(600);
    xSpiCommunicationDevice.Exchange(pxModuleContext ->
                                     xModuleContextStatic.
                                     ucModuleAddress,
                                     auiSpiTxBuffer,
                                     auiSpiRxBuffer,
                                     SPI_PREAMBLE_LENGTH +
                                     CURRENT_OUTPUT_MODULE_PSP_DATA_BASE_BLOCK_LENGTH +
                                     TWO_BYTE_CRC_LENGTH +
                                     TAIL_ANSWER_LENGTH,
                                     MTVI5_EXCHANGE_SPEED_IN_HZ);//LOW_SPEED_IN_HZ

    if ((auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET +
                        CURRENT_OUTPUT_MODULE_PSP_DATA_BASE_BLOCK_LENGTH +
                        TWO_BYTE_CRC_LENGTH]) ==
            DATA_EXCHANGE_OK)
    {
        // ���� ������������� ������ ���� ������ � EEPROM ������.
        cout << "iModuleMtvi5PspDataBaseBlockWrite OK" << endl;
        return 0;
    }
    else
    {
        cout << "iModuleMtvi5PspDataBaseBlockWrite NO" << endl;
        return -1;
    }
}

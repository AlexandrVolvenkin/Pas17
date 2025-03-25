
#include "PasNewConfig.h"
#include "ModuleMvai5.h"
#include "Events.h"

using namespace std;

static unsigned char auiSpiTxBuffer[TX_RX_BUFF_SIZE];
static  unsigned char auiSpiRxBuffer[TX_RX_BUFF_SIZE];



//-----------------------------------------------------------------------------------------------------
// ���������� ����� ������� � �������� ����������� �����.
int iModuleMvai5DataHandler(TModuleContext *pxModuleContext)
{
    int i;
    int j;
    float fData;
    unsigned char ucCalibrPlus;
    unsigned char ucCalibrMinus;
//    unsigned char auiSpiTxBuffer[TX_RX_BUFF_SIZE];
//    unsigned char auiSpiRxBuffer[TX_RX_BUFF_SIZE];

    ucCalibrPlus = 0;
    ucCalibrMinus = 0;
    uint8_t ui8CommonIndex = (pxModuleContext ->
                              xModuleContextDinamic.
                              ucCommonIndex);
    // ��������� ����?(���� ucCommonIndex �� ����� 0, �� �������� ����� ������������ �����)
    if ((ui8CommonIndex > 0) && (ui8CommonIndex < (MVAI5_TXS_INPUT_NUMBER + 1)))
    {
        // ������� ����� ������������ �����.
        ucCalibrPlus = ui8CommonIndex;
        // ��������� ������ ����� ���?
        if (pxModuleContext ->
                xModuleContextDinamic.
                ucCommandControl == MVAI5_COMMAND_CONTROL_SET_BOTTOM_OF_SCALE)
        {
            pxModuleContext ->
            xModuleContextDinamic.
            ucCommandControl = 0;
            // ������� ������� ������.
            ucCalibrPlus |= MVAI5_SET_BOTTOM_OF_SCALE;
        }
        // ��������� ����� ����� ���?
        else if (pxModuleContext ->
                 xModuleContextDinamic.
                 ucCommandControl == MVAI5_COMMAND_CONTROL_SET_TOP_OF_SCALE)
        {
            pxModuleContext ->
            xModuleContextDinamic.
            ucCommandControl = 0;
            // ������� ������� ������.
            ucCalibrPlus |= MVAI5_SET_TOP_OF_SCALE;
        }
        // ������ ���������� ���������� ������, � ���������� ���� � �����(ucCalibrPlus) �
        // � ���������������(!ucCalibrPlus + 1) � �����(ucCalibrMinus).
        // ��� ��������, ����� (ucCalibrPlus + ucCalibrMinus) ������ ���� 0.
        ucCalibrMinus = 0x00 - ucCalibrPlus;
    }

    auiSpiTxBuffer[MVAI5_CALIBRATION_DATA_OFFSET] = ucCalibrPlus;
    auiSpiTxBuffer[MVAI5_CALIBRATION_DATA_OFFSET + 1] = ucCalibrMinus;
    auiSpiTxBuffer[0] = MVAI5_GET_MEASURE_DATA_COMMAND;

    xSpiCommunicationDevice.Exchange(pxModuleContext ->
                                     xModuleContextStatic.
                                     ucModuleAddress,
                                     auiSpiTxBuffer,
                                     auiSpiRxBuffer,
                                     SPI_PREAMBLE_LENGTH +
                                     MVAI5_GET_MEASURE_DATA_COMMAND_ANSWER_LENGTH +
                                     TWO_BYTE_CRC_LENGTH +
                                     TAIL_ANSWER_LENGTH,
                                     SPEED_IN_HZ
                                    );

    // ��� ������� ������?
    switch(auiSpiRxBuffer[SPI_COMMAND_BYTE_OFFSET])
    {
    case MVAI5_ANSWER_REPER_POINTS_ADC_DATABASE_ERROR:
        // ������ �� �������� �����, �� ����� ����������� ������.
        fbAnalogueInputModuleReperPointsAdcDataBaseError = 1;
        // ������� ��� ������;
        (pxModuleContext ->
         xModuleContextDinamic.
         ucErrorCode) = INTERNAL_MODULE_ERROR_REPER_POINTS;

        // �������� ��������� ������� ��� �� ����������������?
        if(xCInternalModuleErrorEvent.EventOnIsNotRegistered(
                    pxModuleContext ->
                    xModuleContextStatic.
                    ucModuleContextIndex,
                    MVAI_REPER_POINTS_DATA_BASE_ERROR_OFFSET))
        {
            // �������������� �������� ��������� �������.
            CEvents::EventRegistration(
                pxModuleContext ->
                xModuleContextStatic.
                ucModuleContextIndex,
                (CEvents::HANDLED_EVENTS_INTERNAL_MODULES_BAD_TYPE |
                 CEvents::HANDLED_EVENTS_IS_POPUP |
                 CEvents::HANDLED_EVENTS_IS_SOUND |
                 CEvents::HANDLED_EVENTS_IS_ARCHIVE),
                MVAI_REPER_POINTS_DATA_BASE_ERROR_OFFSET,
                "�������� �.");
        }


    case MVAI5_GET_MEASURE_DATA_COMMAND:
        // ������ �� ����������?
        if (iCrcSummTwoByteCompare(&auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET],
                                   MVAI5_GET_MEASURE_DATA_COMMAND_ANSWER_LENGTH) > 0)
        {
            // ������ ��������.
            pxModuleContext ->
            xModuleContextDinamic.
            nucBadAnswerCounter = BAD_MODULE_CYCLE_COUNT_DEFAULT;
            // ������� ���� ������ ������.
            *(pxModuleContext ->
              xModuleContextStatic.
              pucModuleBadStateBufferPointer) = BAD_MODULE_RESPONDED_OK;

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

            //iBadModuleBuffUpdate();
            // ������� ������ ��������� ������� ���������� ������.
            memcpy(pxModuleContext ->
                   xModuleContextStatic.
                   pucStatAiBufferPointer,
                   &auiSpiRxBuffer[MVAI5_STATE_DATA_OFFSET],
                   MVAI5_ANALOG_INPUT_QUANTITY);
//            cout << "MVAI5_GET_MEASURE_DATA_COMMAND ucCalibrMinus" << (int)ucCalibrMinus << endl;
//            unsigned char *pucSourceTemp;
//            pucSourceTemp = (unsigned char*)&auiSpiRxBuffer[MVAI5_STATE_DATA_OFFSET];//pxModbusMapping -> buffer_pointer;//
//            for(int i=0; i<8; )
//            {
//                for(int j=0; j<8; j++)
//                {
//                    cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
//                }
//                cout << endl;
//                i += 8;
//            }
            // ������� ���������� �������� ���� ���������� ������ ������.
            for (i = 0; i < MVAI5_ANALOG_INPUT_QUANTITY; i++)
            {
                // ���� ����� ����� ��� ������������ ��� �������� �������� �����?
                if ((auiSpiRxBuffer[MVAI5_STATE_DATA_OFFSET + i] & ANALOGUE_INPUT_LINE_BREAK) ||
                        (auiSpiRxBuffer[MVAI5_STATE_DATA_OFFSET + i] & ANALOGUE_INPUT_CALCULATION_OVERFLOW))
                {
                    // ������ ����� ������������, ������� ��.
                    memset(&(pxModuleContext ->
                             xModuleContextStatic.
                             pucAiValueBufferPointer[i * sizeof(float)]),
                           0,
                           sizeof(float));
                    // ��������� ���� ��������������� - ���� ������������.
                    pxModuleContext ->
                    xModuleContextStatic.
                    pucBadAiBufferPointer[i] = 1;
                    // ���������� ������ ����� ������������, ������� ��.
                    memset(&(pxModuleContext ->
                             xModuleContextStatic.
                             pucDiValueBufferPointer[i * MVAI5_DI_VALUE_ONE_CHANNEL_LENGTH]),
                           0,
                           MVAI5_DI_VALUE_ONE_CHANNEL_LENGTH);
                    // ��������� ����� ��������������� ������� LL, L, H, HH - ������������.
                    memset(&(pxModuleContext ->
                             xModuleContextStatic.
                             pucBadDiBufferPointer[i * MVAI5_DI_VALUE_ONE_CHANNEL_LENGTH]),
                           1,
                           MVAI5_DI_VALUE_ONE_CHANNEL_LENGTH);
                }
                // ������� ����� ���������� �������� �����?
                else if(
                    (((auiSpiRxBuffer[MVAI5_STATE_DATA_OFFSET + i] & ANALOGUE_INPUT_LINE_BREAK) == 0) &&
                     ((auiSpiRxBuffer[MVAI5_STATE_DATA_OFFSET + i] & ANALOGUE_INPUT_CALCULATION_OVERFLOW) == 0)) &&
                    ((auiSpiRxBuffer[MVAI5_STATE_DATA_OFFSET + i] & ANALOGUE_INPUT_CHANNEL_CALIBRATION)))
                {
                    // ������� ���������� �������� � �����������.
                    fData = fStep5ToFloat(&auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET +
                                                          (i * MVAI5_ONE_ANALOG_INPUT_DATA_BYTE_QUANTITY)]);
                    // �������� ��� � ������� ������.
                    memcpy(&(pxModuleContext ->
                             xModuleContextStatic.
                             pucAiValueBufferPointer[i * sizeof(float)]),
                           (unsigned char*)&fData,
                           sizeof(float));
                    // ��������� ���� ��������������� - ���� ������������.
                    pxModuleContext ->
                    xModuleContextStatic.
                    pucBadAiBufferPointer[i] = 1;
                    // ���������� ������ ����� ������������, ������� ��.
                    memset(&(pxModuleContext ->
                             xModuleContextStatic.
                             pucDiValueBufferPointer[i * MVAI5_DI_VALUE_ONE_CHANNEL_LENGTH]),
                           0,
                           MVAI5_DI_VALUE_ONE_CHANNEL_LENGTH);
                    // ��������� ����� ��������������� ������� LL, L, H, HH - ������������.
                    memset(&(pxModuleContext ->
                             xModuleContextStatic.
                             pucBadDiBufferPointer[i * MVAI5_DI_VALUE_ONE_CHANNEL_LENGTH]),
                           1,
                           MVAI5_DI_VALUE_ONE_CHANNEL_LENGTH);
                }
                else
                {
                    // ���������� ���� ������� �� ���������?
                    if (pxModuleContext ->
                            xModuleContextStatic.
                            pucAinOffBufferPointer[i])
                    {
                        // ������ ������ ������ �������� �� ���������, ������� ��.
                        memset(&(pxModuleContext ->
                                 xModuleContextStatic.
                                 pucAiValueBufferPointer[i * sizeof(float)]),
                               0,
                               sizeof(float));
                        // ��������� ���� ��������������� - ���� ������������.
                        pxModuleContext ->
                        xModuleContextStatic.
                        pucBadAiBufferPointer[i] = 1;
                        // ���������� ������ ����� ������������, ������� ��.
                        memset(&(pxModuleContext ->
                                 xModuleContextStatic.
                                 pucDiValueBufferPointer[i * MVAI5_DI_VALUE_ONE_CHANNEL_LENGTH]),
                               0,
                               MVAI5_DI_VALUE_ONE_CHANNEL_LENGTH);
                        // ��������� ����� ��������������� ������� LL, L, H, HH - ������������.
                        memset(&(pxModuleContext ->
                                 xModuleContextStatic.
                                 pucBadDiBufferPointer[i * MVAI5_DI_VALUE_ONE_CHANNEL_LENGTH]),
                               1,
                               MVAI5_DI_VALUE_ONE_CHANNEL_LENGTH);
                    }
                    else
                    {
                        // ������� ���������� �������� � �����������.
                        fData = fStep5ToFloat(&auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET +
                                                              (i * MVAI5_ONE_ANALOG_INPUT_DATA_BYTE_QUANTITY)]);
                        // �������� ��� � ������� ������.
                        memcpy(&(pxModuleContext ->
                                 xModuleContextStatic.
                                 pucAiValueBufferPointer[i * sizeof(float)]),
                               (unsigned char*)&fData,
                               sizeof(float));
                        // ������� ���� ��������������� - ���� ����������.
                        pxModuleContext ->
                        xModuleContextStatic.
                        pucBadAiBufferPointer[i] = 0;
                        // ������� ����� ������� LL, L, H, HH.
                        memset(&(pxModuleContext ->
                                 xModuleContextStatic.
                                 pucDiValueBufferPointer[i * MVAI5_DI_VALUE_ONE_CHANNEL_LENGTH]),
                               0,
                               MVAI5_DI_VALUE_ONE_CHANNEL_LENGTH);
                        // ������� ����� ��������������� ������� LL, L, H, HH - ����������.
                        memset(&(pxModuleContext ->
                                 xModuleContextStatic.
                                 pucBadDiBufferPointer[i * MVAI5_DI_VALUE_ONE_CHANNEL_LENGTH]),
                               0,
                               MVAI5_DI_VALUE_ONE_CHANNEL_LENGTH);
                        // �������� ������� LL + L?
                        if ((auiSpiRxBuffer[MVAI5_STATE_DATA_OFFSET + i] & ANALOGUE_INPUT_SET_POINT_VIOLATION_MASK) ==
                                ANALOGUE_INPUT_SET_POINT_VIOLATION_LL_L)
                        {
                            // ��������� ���� ��������� ������� LL.
                            (pxModuleContext ->
                             xModuleContextStatic.
                             pucDiValueBufferPointer[(i * MVAI5_DI_VALUE_ONE_CHANNEL_LENGTH) +
                                                     ANALOGUE_INPUT_SET_POINT_VIOLATION_LL_L_OFFSET]) = 1;

                            // ��������� ���� ��������� ������� L.
                            (pxModuleContext ->
                             xModuleContextStatic.
                             pucDiValueBufferPointer[(i * MVAI5_DI_VALUE_ONE_CHANNEL_LENGTH) +
                                                     ANALOGUE_INPUT_SET_POINT_VIOLATION_L_OFFSET]) = 1;
                        }
                        // �������� ������� L?
                        if ((auiSpiRxBuffer[MVAI5_STATE_DATA_OFFSET + i] & ANALOGUE_INPUT_SET_POINT_VIOLATION_MASK) ==
                                ANALOGUE_INPUT_SET_POINT_VIOLATION_L)
                        {
                            // ��������� ���� ��������� ������� L.
                            (pxModuleContext ->
                             xModuleContextStatic.
                             pucDiValueBufferPointer[(i * MVAI5_DI_VALUE_ONE_CHANNEL_LENGTH) +
                                                     ANALOGUE_INPUT_SET_POINT_VIOLATION_L_OFFSET]) = 1;
                        }
                        // �������� ������� H?
                        if ((auiSpiRxBuffer[MVAI5_STATE_DATA_OFFSET + i] & ANALOGUE_INPUT_SET_POINT_VIOLATION_MASK) ==
                                ANALOGUE_INPUT_SET_POINT_VIOLATION_H)
                        {
                            // ��������� ���� ��������� ������� H.
                            (pxModuleContext ->
                             xModuleContextStatic.
                             pucDiValueBufferPointer[(i * MVAI5_DI_VALUE_ONE_CHANNEL_LENGTH) +
                                                     ANALOGUE_INPUT_SET_POINT_VIOLATION_H_OFFSET]) = 1;
                        }
                        // �������� ������� HH + H?
                        if ((auiSpiRxBuffer[MVAI5_STATE_DATA_OFFSET + i] & ANALOGUE_INPUT_SET_POINT_VIOLATION_MASK) ==
                                ANALOGUE_INPUT_SET_POINT_VIOLATION_HH_H)
                        {
                            // ��������� ���� ��������� ������� HH.
                            (pxModuleContext ->
                             xModuleContextStatic.
                             pucDiValueBufferPointer[(i * MVAI5_DI_VALUE_ONE_CHANNEL_LENGTH) +
                                                     ANALOGUE_INPUT_SET_POINT_VIOLATION_HH_H_OFFSET]) = 1;

                            // ��������� ���� ��������� ������� H.
                            (pxModuleContext ->
                             xModuleContextStatic.
                             pucDiValueBufferPointer[(i * MVAI5_DI_VALUE_ONE_CHANNEL_LENGTH) +
                                                     ANALOGUE_INPUT_SET_POINT_VIOLATION_H_OFFSET]) = 1;
                        }
                    }
                }
            }
            return 0;
        }
        break;

    case MVAI5_ANSWER_DATA_NOT_READY:
        // ������ �� ������.
        return 0;
        break;

    case MVAI5_ANSWER_DATABASE_ERROR:
        // ������ �� ���������, ����� �����.
        fbAnalogueInputModuleDataBaseError = 1;
        // ������� ��� ������;
        (pxModuleContext ->
         xModuleContextDinamic.
         ucErrorCode) = INTERNAL_MODULE_ERROR_DATA_BASE;

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
                 CEvents::HANDLED_EVENTS_IS_POPUP |
                 CEvents::HANDLED_EVENTS_IS_SOUND |
                 CEvents::HANDLED_EVENTS_IS_ARCHIVE),
                MTVI5_DATA_BASE_ERROR_OFFSET,
                "����. �. ���");
        }

        return 0;
        break;

    default:
        break;
    };


    // ������ ������� �����������?
    if ((pxModuleContext ->
            xModuleContextDinamic.
            nucBadAnswerCounter) == 0)
    {
        // ������ ������� �����������.
        *(pxModuleContext ->
          xModuleContextStatic.
          pucModuleBadStateBufferPointer) = BAD_MODULE_NOT_RESPONDED;

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

        // ������ ������ ������ ������������, ������� ��.
        memset(pxModuleContext ->
               xModuleContextStatic.
               pucAiValueBufferPointer,
               0,
               MVAI5_ANALOG_INPUT_QUANTITY * sizeof(float));
        // ��������� ����� ��������������� - ����� ������������.
        memset(pxModuleContext ->
               xModuleContextStatic.
               pucBadAiBufferPointer,
               1,
               MVAI5_ANALOG_INPUT_QUANTITY);
        // ���������� ������ ������ ������ ������������, ������� ��.
        memset(pxModuleContext ->
               xModuleContextStatic.
               pucDiValueBufferPointer,
               0,
               MVAI5_ANALOG_INPUT_QUANTITY * MVAI5_DI_VALUE_ONE_CHANNEL_LENGTH);
        // ��������� ����� ��������������� ���� ���������� �������� ������.
        memset(pxModuleContext ->
               xModuleContextStatic.
               pucBadDiBufferPointer,
               1,
               MVAI5_ANALOG_INPUT_QUANTITY * MVAI5_DI_VALUE_ONE_CHANNEL_LENGTH);
        //iBadModuleBuffUpdate();
    }
    else
    {
        // �������� �������� �������� - "���������� ������� ����� � ������� ��� ������".
        (pxModuleContext ->
         xModuleContextDinamic.
         nucBadAnswerCounter)--;
    }
    return 0;
}

//-----------------------------------------------------------------------------------------------------
// ������ �� ������ ������� �������� ����� � ���� ���.
int iModuleMvai5ReperPointsAdcRead(TModuleContext *pxModuleContext)
{
    unsigned short usData;
    int i;
    unsigned char *pucSource;
    unsigned char *pucDestination;
//    unsigned char aucTempArray[PLC_DATA_BASE_BLOCK_LENGTH];
//    unsigned char auiSpiTxBuffer[TX_RX_BUFF_SIZE];
//    unsigned char auiSpiRxBuffer[TX_RX_BUFF_SIZE];

//    memset(&aucTempArray[0],
//           0,
//           sizeof(aucTempArray));

    auiSpiTxBuffer[0] = MVAI5_GET_REPER_POINTS_ADC_DATA_COMMAND;
    xSpiCommunicationDevice.Exchange(pxModuleContext ->
                                     xModuleContextStatic.
                                     ucModuleAddress,
                                     auiSpiTxBuffer,
                                     auiSpiRxBuffer,
                                     SPI_PREAMBLE_LENGTH +
                                     ANALOGUE_INPUT_MODULE_REPER_POINTS_ADC_DATA_BASE_BLOCK_LENGTH +
                                     TWO_BYTE_CRC_LENGTH,
                                     SPEED_IN_HZ);
    // ������ ��������?
    if (auiSpiRxBuffer[SPI_COMMAND_BYTE_OFFSET] == MVAI5_GET_REPER_POINTS_ADC_DATA_COMMAND) // if echo answer is ok.
    {
        // ������ �� ����������?
        if (iCrcSummTwoByteCompare(&auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET],
                                   ANALOGUE_INPUT_MODULE_REPER_POINTS_ADC_DATA_BASE_BLOCK_LENGTH) > 0)
        {
            // ������� ��������� �� ������ �������� ����� � ����� ��� �������� �� ������.
            pucSource = &auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET];
//            // ���������� �� ������ ������� �������� ����� � ���� ��� �������� � ������ ���� ������ ������� �
//            // ������(ANALOGUE_INPUT_MODULE_REPER_POINTS_ADC_DATA_BASE_BLOCK_OFFSET) ������������.
//            // � ����������(MAX_MVAI5_MODULES_QUANTITY).
//            // � ��������� pucDataBasePointer ���������� ����� ����� � ���� ������, ������� ����������� �����.
//            // ���������� ������ ��� ������ ������� ����������� ����� �����
//            // ���������� ������ � ������� �������� ����� ��� + ���� ���.
//            // �������� ������� ����� ���������� �� ���� �������� ����� ��� + ���� ��� � ���� ������� ����������� �����.
//            // � ������� ����� ����� �������� ����� ��� + ���� ��� � ���� ������.
//            pucDestination = (unsigned char*)(pxModuleContext ->
//                                              xModuleContextStatic.
//                                              pucDataBasePointer +
//                                              ((ANALOGUE_INPUT_MODULE_REPER_POINTS_ADC_DATA_BASE_BLOCK_OFFSET -
//                                                ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_OFFSET)  * PLC_DATA_BASE_BLOCK_LENGTH));
//
            pucDestination = pxModuleContext ->
                             xModuleContextStatic.
                             pucReperPointsAdcBufferPointer;

            // ��������� ������ �������� ����� � ����� ��� �� ���������� ������ � ������� ����� �������.
            memcpy(pucDestination,
                   pucSource,
                   ANALOGUE_INPUT_MODULE_REPER_POINTS_ADC_DATA_BASE_BLOCK_LENGTH);
            // �� OK.
            //ucModuleError = 0;
        }
        else
        {
            // ������ ������ �������.
            cout << "MVAI5_GET_REPER_POINTS_ADC_CRC error" << endl;
            ucModuleError = MVAI5_GET_REPER_POINTS_ADC_DATA_COMMUNICATION_ERROR;
        }
    }
    else
    {
        // ������ �� ��������.
        cout << "MVAI5_GET_REPER_POINTS_ADC_DATA_COMMAND error" << endl;
        ucModuleError = MVAI5_GET_REPER_POINTS_ADC_DATA_COMMAND_ERROR;
    }
    return 0;
}

//-----------------------------------------------------------------------------------------------------
// ������ �� ������, ������� ����������� ��������� ���� ��� (������ ��� ����3, ��� 8)
int iModuleMvai5TxsRead(TModuleContext *pxModuleContext)
{
    unsigned short usData;
    float fData;
    int i;
    unsigned char *pucSource;
    unsigned char *pucDestination;
    unsigned char ucCalibrPlus;
    unsigned char ucCalibrMinus;
//    unsigned char auiSpiTxBuffer[TX_RX_BUFF_SIZE];
//    unsigned char auiSpiRxBuffer[TX_RX_BUFF_SIZE];

//    cout << "iModuleMvai5TxsRead ucCommonIndex" << (int)(pxModuleContext ->
//            xModuleContextDinamic.
//            ucCommonIndex) << endl;
    ucCalibrPlus = 0;
    ucCalibrMinus = 0;
    uint8_t ui8CommonIndex = (pxModuleContext ->
                              xModuleContextDinamic.
                              ucCommonIndex);
    // ��������� ����?(���� ucCommonIndex �� ����� 0, �� �������� ����� ������������ �����)
    if ((ui8CommonIndex > 0) && (ui8CommonIndex < (MVAI5_TXS_INPUT_NUMBER + 1)))
//    // ����� ��� �����������?
//    if (pxModuleContext ->
//            xModuleContextDinamic.
//            ucCommonIndex == MVAI5_TXS_INPUT_NUMBER)
    {
        // ������� ����� ������������ �����.
        ucCalibrPlus = ui8CommonIndex;
        // ��������� ������ ����� ���?
        if (pxModuleContext ->
                xModuleContextDinamic.
                ucCommandControl == MVAI5_COMMAND_CONTROL_SET_BOTTOM_OF_SCALE)
        {
            pxModuleContext ->
            xModuleContextDinamic.
            ucCommandControl = 0;
            // ������� ������� ������.
            ucCalibrPlus |= MVAI5_SET_BOTTOM_OF_SCALE;
        }
        // ��������� ����� ����� ���?
        else if (pxModuleContext ->
                 xModuleContextDinamic.
                 ucCommandControl == MVAI5_COMMAND_CONTROL_SET_TOP_OF_SCALE)
        {
            pxModuleContext ->
            xModuleContextDinamic.
            ucCommandControl = 0;
            // ������� ������� ������.
            ucCalibrPlus |= MVAI5_SET_TOP_OF_SCALE;
        }
        // ������ ���������� ���������� ������, � ���������� ���� � �����(ucCalibrPlus) �
        // � ���������������(!ucCalibrPlus + 1) � �����(ucCalibrMinus).
        // ��� ��������, ����� (ucCalibrPlus + ucCalibrMinus) ������ ���� 0.
        ucCalibrMinus = 0x00 - ucCalibrPlus;
    }
//    cout << "iModuleMvai5TxsRead ucCalibrPlus" << (int)ucCalibrPlus << endl;
//    cout << "iModuleMvai5TxsRead ucCalibrMinus" << (int)ucCalibrMinus << endl;
    // ���������� ���������� ���������� � ������ ����� �������,
    // �� ����� ����� �� ������ ���� ���� ����������� �����.
    auiSpiTxBuffer[SPI_PREAMBLE_LENGTH + MVAI5_TXS_CALIBRATION_DATA_OFFSET] = ucCalibrPlus;
    auiSpiTxBuffer[SPI_PREAMBLE_LENGTH + MVAI5_TXS_CALIBRATION_DATA_OFFSET + 1] = ucCalibrMinus;
    auiSpiTxBuffer[0] = MVAI5_GET_TXS_DATA_COMMAND;

    xSpiCommunicationDevice.Exchange(pxModuleContext ->
                                     xModuleContextStatic.
                                     ucModuleAddress,
                                     auiSpiTxBuffer,
                                     auiSpiRxBuffer,
                                     SPI_PREAMBLE_LENGTH +
                                     MVAI5_GET_TXS_DATA_COMMAND_ANSWER_LENGTH +
                                     TWO_BYTE_CRC_LENGTH +
                                     TAIL_ANSWER_LENGTH,
                                     SPEED_IN_HZ);
    // ������ ��������?
    if (auiSpiRxBuffer[SPI_COMMAND_BYTE_OFFSET] == MVAI5_GET_TXS_DATA_COMMAND) // if echo answer is ok.
    {
        // ������ �� ����������?
        if (iCrcSummTwoByteCompare(&auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET],
                                   MVAI5_GET_TXS_DATA_COMMAND_ANSWER_LENGTH) > 0)
        {
            // �������, ����������� � �������� � ������� ������, ���������� �������� ���.
            ((TTxsData*)(pxModuleContext ->
                         xModuleContextStatic.
                         pucTxsBufferPointer)) -> fTxsValue =
                             fStep5ToFloat(&auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET + 1]);
            // ������� � �������� � ������� ������, ��������� ������ ���.
            ((TTxsData*)(pxModuleContext ->
                         xModuleContextStatic.
                         pucTxsBufferPointer)) -> ucTxsStat =
                             auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET + 4];
        }
        else
        {
            // ������ ������ �������.
            cout << "MVAI5_GET_TXS_CRC error" << endl;
            ucModuleError = PROGRAMMING_DATA_BASE_WRITE_BLOCK_MVA_TXS_READ_ERROR;
        }
    }
    else
    {
        // ������ �� ��������.
        cout << "MVAI5_GET_TXS_DATA_COMMAND error" << endl;
        ucModuleError = PROGRAMMING_DATA_BASE_WRITE_BLOCK_MVA_TXS_READ_ERROR;
    }
    return 0;
}

//-----------------------------------------------------------------------------------------------------
// ���������� ���������� ���� ������ ������ � ����� � ������ �������.
// ���� �� ���������, ������� ���� �� ������ � ������.
int iModuleMvai5PlcDataBaseCheck(TModuleContext *pxModuleContext)
{
    unsigned short usData;
    int i;
    unsigned char *pucSource;
    unsigned char *pucDestination;
    unsigned char aucTempArray[PLC_DATA_BASE_BLOCK_LENGTH];
//    unsigned char auiSpiTxBuffer[TX_RX_BUFF_SIZE];
//    unsigned char auiSpiRxBuffer[TX_RX_BUFF_SIZE];

    memset(&aucTempArray[0],
           0,
           sizeof(aucTempArray));

    auiSpiTxBuffer[0] = MVAI5_GET_DATA_BASE_COMMAND;
    xSpiCommunicationDevice.Exchange(pxModuleContext ->
                                     xModuleContextStatic.
                                     ucModuleAddress,
                                     auiSpiTxBuffer,
                                     auiSpiRxBuffer,
                                     SPI_PREAMBLE_LENGTH +
                                     ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_LENGTH +
                                     TWO_BYTE_CRC_LENGTH,
                                     LOW_SPEED_IN_HZ);
    // ������ ��������?
    if (auiSpiRxBuffer[SPI_COMMAND_BYTE_OFFSET] == MVAI5_GET_DATA_BASE_COMMAND) // if echo answer is ok.
    {
        // ������ �� ����������?
        if (iCrcSummTwoByteCompare(&auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET],
                                   ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_LENGTH) > 0)
        {
// � ������� ����������� ����� � � ������ �������, ���� ������ �������� � ������ ��������.
//  � ������� ����������� ����� ������� ���� ������ ���������������� ������(6 ������, �� 20 ����) 120 ����, �����
// ��������� ��������� ������(6 ������, �� 8 ����) 48 ����. 6 ������(120 + 48 = 168) ����.
//  � ������ ������� ������ ���������������� � ��������� ��������� ���� ���������� ������� ��� ������� �����
// (���� ����: 20 ���� - ������ ���������������� � 8 ���� - ��������� ���������). ���� ���� - 28 ����. 6 ������(6 * 28 = 168) ����.
// ����� ������������ �������������� ������� ���� ������, �� ������ � �������.
            // ������� ��������� �� ������ ���������������� ������� �����, �������� �� ������.
            pucSource = &auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET];
            pucDestination = &aucTempArray[0];
            for (i = 0; i < MVAI5_ANALOG_INPUT_QUANTITY; i++)
            {
                // ��������� �� ��������� ����� ����� ����� - ������ ���������������� ������ �����(20 ����).
                memcpy(pucDestination, pucSource, 20);
                // ������� � ������ ���������������� ���������� ����� � ������(+20 ����).
                pucSource += 20;
                // ������� � ���������� ����� ������ ������ ����� � �������(+28 ����).
                pucDestination += 28;
            }
            // ������� ��������� �� ������ ���������� ��������� ������� �����, �������� �� ������.
            pucSource = &auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET + (20 * 6)];
            pucDestination = &aucTempArray[20];
            for (i = 0; i < MVAI5_ANALOG_INPUT_QUANTITY; i++)
            {
                // ��������� �� ��������� ����� ����� ����� - ��������� ��������� ������ �����(8 ����).
                memcpy(pucDestination, pucSource, 8);
                // ������� � ������ ���������� ��������� ���������� ����� � ������(+8 ����).
                pucSource += 8;
                // ������� � ���������� ����� ������ ������ ����� � �������(+28 ����).
                pucDestination += 28;
            }

            pucSource = &aucTempArray[0];
            // ������� ��������� �� ���� � ���� ������ �������.
            pucDestination = (unsigned char*)(pxModuleContext ->
                                              xModuleContextStatic.
                                              pucDataBasePointer);
            // ���� ������ �� ���������?
            if (memcmp((const void*)pucDestination,
                       (const void*)pucSource,
                       ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_LENGTH) != 0)
            {

                // ��������� ���� ������ �� ������ � ������.
                memcpy(pucDestination,
                       pucSource,
                       ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_LENGTH);
                cout << "iModuleMvai5PlcDataBaseCheck NO" << endl;
                return -1;
            }
            else
            {
                // ���� ������ ���������.
                cout << "iModuleMvai5PlcDataBaseCheck OK" << endl;
                return 0;
            }
        }
        else
        {
            // ������ ������ �������.
            cout << "iModuleMvai5PlcDataBaseCheck CRC error" << endl;
            //cout << "MVAI5_GET_DATA_BASE_CRC error" << endl;
        }
    }
    else
    {
        // ������ �� ��������.
        cout << "iModuleMvai5PlcDataBaseCheck COMMAND error" << endl;
    }
    return -1;
}

//-----------------------------------------------------------------------------------------------------
// ���������� ���������� ���� ������ ������ � ����� � ������ �������.
// ���� �� ���������, ������� ���� �� ������� � ������.
int iModuleMvai5ModuleDataBaseCheck(TModuleContext *pxModuleContext)
{
    unsigned short usData;
    int i;
    unsigned char *pucSource;
    unsigned char *pucDestination;
    unsigned char aucTempArray[PLC_DATA_BASE_BLOCK_LENGTH];
//    unsigned char auiSpiTxBuffer[TX_RX_BUFF_SIZE];
//    unsigned char auiSpiRxBuffer[TX_RX_BUFF_SIZE];

    memset(&aucTempArray[0],
           0,
           sizeof(aucTempArray));

    auiSpiTxBuffer[0] = MVAI5_GET_DATA_BASE_COMMAND;
    xSpiCommunicationDevice.Exchange(pxModuleContext ->
                                     xModuleContextStatic.
                                     ucModuleAddress,
                                     auiSpiTxBuffer,
                                     auiSpiRxBuffer,
                                     SPI_PREAMBLE_LENGTH +
                                     ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_LENGTH +
                                     TWO_BYTE_CRC_LENGTH,
                                     LOW_SPEED_IN_HZ);
    // ������ ��������?
    if (auiSpiRxBuffer[SPI_COMMAND_BYTE_OFFSET] == MVAI5_GET_DATA_BASE_COMMAND) // if echo answer is ok.
    {
        // ������ �� ����������?
        if (iCrcSummTwoByteCompare(&auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET],
                                   ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_LENGTH) > 0)
        {
// � ������� ����������� ����� � � ������ �������, ���� ������ �������� � ������ ��������.
//  � ������� ����������� ����� ������� ���� ������ ���������������� ������(6 ������, �� 20 ����) 120 ����, �����
// ��������� ��������� ������(6 ������, �� 8 ����) 48 ����. 6 ������(120 + 48 = 168) ����.
//  � ������ ������� ������ ���������������� � ��������� ��������� ���� ���������� ������� ��� ������� �����
// (���� ����: 20 ���� - ������ ���������������� � 8 ���� - ��������� ���������). ���� ���� - 28 ����. 6 ������(6 * 28 = 168) ����.
// ����� ������������ �������������� ������� ���� ������, �� ������ � �������.
            // ������� ��������� �� ������ ���������������� ������� �����, �������� �� ������.
            pucSource = &auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET];

            pucDestination = &aucTempArray[0];
            for (i = 0; i < MVAI5_ANALOG_INPUT_QUANTITY; i++)
            {
                // ��������� �� ��������� ����� ����� ����� - ������ ���������������� ������ �����(20 ����).
                memcpy(pucDestination, pucSource, 20);
                // ������� � ������ ���������������� ���������� ����� � ������(+20 ����).
                pucSource += 20;
                // ������� � ���������� ����� ������ ������ ����� � �������(+28 ����).
                pucDestination += 28;
            }
            // ������� ��������� �� ������ ���������� ��������� ������� �����, �������� �� ������.
            pucSource = &auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET + (20 * 6)];
            pucDestination = &aucTempArray[20];
            for (i = 0; i < MVAI5_ANALOG_INPUT_QUANTITY; i++)
            {
                // ��������� �� ��������� ����� ����� ����� - ��������� ��������� ������ �����(8 ����).
                memcpy(pucDestination, pucSource, 8);
                // ������� � ������ ���������� ��������� ���������� ����� � ������(+8 ����).
                pucSource += 8;
                // ������� � ���������� ����� ������ ������ ����� � �������(+28 ����).
                pucDestination += 28;
            }

            pucSource = &aucTempArray[0];
            // ������� ��������� �� ���� � ���� ������ �������.
            pucDestination = (unsigned char*)(pxModuleContext ->
                                              xModuleContextStatic.
                                              pucDataBasePointer);
            // ���� ������ �� ���������?
            if (memcmp((const void*)pucDestination,
                       (const void*)pucSource,
                       ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_LENGTH) != 0)
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
                    iModuleMvai5DataBaseBlockWrite(pxModuleContext);
                    cout << "New data base saved in module" << endl;
                }
                cout << "iModuleMvai5ModuleDataBaseCheck NO" << endl;
                return -1;
            }
            else
            {
                // ���� ������ ���������.
                cout << "iModuleMvai5ModuleDataBaseCheck OK" << endl;
                return 0;
            }
        }
        else
        {
            // ������ ������ �������.
            cout << "iModuleMvai5ModuleDataBaseCheck CRC error" << endl;
        }
    }
    else
    {
        // ������ �� ��������.
        cout << "iModuleMvai5ModuleDataBaseCheck COMMAND error" << endl;
    }
    return -1;
}

////-----------------------------------------------------------------------------------------------------
//// ���������� ���� ������ �� RAM ������� � EEPROM ������.
//int iModuleMvai5DataBaseBlockWrite(TModuleContext *pxModuleContext)
//{
//    unsigned short usData;
//    int i;
//    unsigned char *pucSource;
//    unsigned char *pucDestination;
//    unsigned int nuiBusyTimeCounter;
//    unsigned char aucTempArray[PLC_DATA_BASE_BLOCK_LENGTH];
////    unsigned char auiSpiTxBuffer[TX_RX_BUFF_SIZE];
////    unsigned char auiSpiRxBuffer[TX_RX_BUFF_SIZE];
//
//// � ������� ����������� ����� � � ������ �������, ���� ������ �������� � ������ ��������.
////  � ������� ����������� ����� ������� ���� ������ ���������������� ������(6 ������, �� 20 ����) 120 ����, �����
//// ��������� ��������� ������(6 ������, �� 8 ����) 48 ����. 6 ������(120 + 48 = 168) ����.
////  � ������ ������� ������ ���������������� � ��������� ��������� ���� ���������� ������� ��� ������� �����
//// (���� ����: 20 ���� - ������ ���������������� � 8 ���� - ��������� ���������). ���� ���� - 28 ����. 6 ������(6 * 28 = 168) ����.
//// ����� ������������ �������������� ������� ���� ������, �� ������� � ������.
//    memset(&aucTempArray[0],
//           0,
//           sizeof(aucTempArray));
//    // ������� ��������� �� ���� ���� ������, �������� �� Modbus �� ��������� �����.
//
//    pucSource =
//        ((pxModuleContext ->
//          xModuleContextStatic.
//          pucDataBasePointer) +
//         (pxModuleContext ->
//          xModuleContextStatic.
//          ucDataBaseDataOffset));
//
//    // ������� ��������� �� ������ ���������������� ������� �����, ��� ������ � ������.
//    pucDestination = &aucTempArray[SPI_DATA_BYTE_OFFSET];
//    for (i = 0; i < MVAI5_ANALOG_INPUT_QUANTITY; i++)
//    {
//        // ��������� �� ��������� ����� ����� ����� - ������ ���������������� ������ �����(20 ����).
//        memcpy(pucDestination, pucSource, 20);
//        // ������� � ������ ���������������� ���������� ����� � ������(+20 ����).
//        pucDestination += 20;
//        // ������� � ���������� ����� ������ ������ ����� � �������(+28 ����).
//        pucSource += 28;
//    }
//    // ������� ��������� �� ���� ���� ������, �������� �� Modbus �� ��������� �����.
//
//    pucSource =
//        ((pxModuleContext ->
//          xModuleContextStatic.
//          pucDataBasePointer) +
//         (pxModuleContext ->
//          xModuleContextStatic.
//          ucDataBaseDataOffset));
//
//    // ������� � ������ ���������� ��������� ������� �����(+20 ����).
//    pucSource += 20;
//    // ������� ��������� �� ������ ���������� ��������� ������� �����, ��� ������ � ������.
//    pucDestination = &aucTempArray[SPI_DATA_BYTE_OFFSET + (20 * 6)];
//    for (i = 0; i < MVAI5_ANALOG_INPUT_QUANTITY; i++)
//    {
//        // ��������� �� ��������� ����� ����� ����� - ��������� ��������� ������ �����(8 ����).
//        memcpy(pucDestination, pucSource, 8);
//        // ������� � ������ ���������� ��������� ���������� ����� � ������(+8 ����).
//        pucDestination += 8;
//        // ������� � ���������� ����� ������ ������ ����� � �������(+28 ����).
//        pucSource += 28;
//    }
//    // ������� ��������� �� ��������������� ���� ������ �� ������� � ������.
//    pucSource = &aucTempArray[SPI_DATA_BYTE_OFFSET];
//    usData = usCrcSummTwoByteCalculation(pucSource,
//                                         ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_LENGTH);
//    // ������� ��������� �� ����� CRC � ������.
//    pucDestination = &aucTempArray[SPI_DATA_BYTE_OFFSET +
//                                   ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_LENGTH];
//    *pucDestination++ = (unsigned char)usData;
//    *pucDestination = (unsigned char)(usData >> 8);
//    // ��������� ��������������� ���� ������ �� ���������� � ����� TX SPI.
//    memcpy(&auiSpiTxBuffer[SPI_DATA_BYTE_OFFSET],
//           pucSource,
//           ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_LENGTH +
//           TWO_BYTE_CRC_LENGTH);
//    // �������� ������ � ������.
//    auiSpiTxBuffer[0] = MVAI5_SET_DATA_BASE_COMMAND;
//    xSpiCommunicationDevice.Exchange(pxModuleContext ->
//                                     xModuleContextStatic.
//                                     ucModuleAddress,
//                                     auiSpiTxBuffer,
//                                     auiSpiRxBuffer,
//                                     SPI_PREAMBLE_LENGTH +
//                                     ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_LENGTH +
//                                     TWO_BYTE_CRC_LENGTH +
//                                     TAIL_ANSWER_LENGTH,
//                                     LOW_SPEED_IN_HZ);
//    // ������ ��������?
//    if (auiSpiRxBuffer[SPI_COMMAND_BYTE_OFFSET] == MVAI5_SET_DATA_BASE_COMMAND) // if echo answer is ok.
//    {
//        // ������ ������� ��� ������?
//        if ((auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET +
//                            ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_LENGTH +
//                            TWO_BYTE_CRC_LENGTH]) ==
//                DATA_EXCHANGE_OK)
//        {
//            // ���� ������������� ������ ���� ������ � EEPROM ������.
//            cout << "iModuleMvai5DataBaseBlockWrite OK" << endl;
//            i = 0;
////            nuiBusyTimeCounter = MVAI5_CHECK_DATA_BASE_WRITE_BUSY_WAITING_TIME;
////            // ��� ������������� ������ ���� ������ � EEPROM ������.
////            // ���� ������� - ������.
////            while (1)
////            {
////                // ���, ���� ������ �� ������� ���� ������ � EEPROM.
////                usleep(MVAI5_CHECK_DATA_BASE_WRITE_BUSY_DELAY_TIME);
////                // ������ ������ � ����������� ����������������.
////                auiSpiTxBuffer[0] = MVAI5_CHECK_DATA_BASE_WRITE_COMMAND;
////                xSpiCommunicationDevice.Exchange(pxModuleContext ->
////                                                 xModuleContextStatic.
////                                                 ucModuleAddress,
////                                                 auiSpiTxBuffer,
////                                                 auiSpiRxBuffer,
////                                                 SPI_PREAMBLE_LENGTH +
////                                                 TAIL_ANSWER_LENGTH,
////                                                 LOW_SPEED_IN_HZ);
////                // ���� ������������� ������ ���� ������ � EEPROM ������?
////                if((auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET]) == DATA_EXCHANGE_OK)
////                {
////                    // ���� ������������� ������ ���� ������ � EEPROM ������.
////                    cout << "iModuleMvai5DataBaseBlockWrite OK" << endl;
////                    i = 0;
////                    break;
////                }
////                // ������ ����� ������� ������ � EEPROM?
////                else if ((auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET]) == MVAI5_CHECK_DATA_BASE_WRITE_ANSWER_BUSY)
////                {
////                    // ������ ����� ������� ������ � EEPROM.
////                }
////                // ��������� ������ ��� ������ ���� ������ � EEPROM ������?
////                else if ((auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET]) == DATA_EXCHANGE_ERROR)
////                {
////                    // ��������� ������ ��� ������ ���� ������ � EEPROM ������.
////                    ucDataBaseError = PROGRAMMING_DATA_BASE_WRITE_BLOCK_MVA_EEPROM_ERROR;
////                    cout << "iModuleMvai5DataBaseBlockWrite NO" << endl;
////                    i = -1;
////                    break;
////                }
////                else
////                {
////
////                }
////                // ������ �� �������� �� ������ � ���������� ���������������� ���� ������?
////                if (!nuiBusyTimeCounter--)
////                {
////                    // ��� ������������� ������ ���� ������ � EEPROM ������.
////                    ucDataBaseError = PROGRAMMING_DATA_BASE_WRITE_BLOCK_MVA_NOT_CONFIRM;
////                    cout << "iModuleMvai5DataBaseBlockWrite NO" << endl;
////                    i = -1;
////                    break;
////                }
////            }
//        }
//        else
//        {
//            // ������ �������� ���� ������ � ������.
//            ucDataBaseError = PROGRAMMING_DATA_BASE_WRITE_BLOCK_MVA_LOAD_ERROR;
//            cout << "iModuleMvai5DataBaseBlockWrite NO" << endl;
//            i = -1;
//        }
//    }
//    else
//    {
//        // ������ �� ��������.
//        ucDataBaseError = PROGRAMMING_DATA_BASE_WRITE_BLOCK_MVA_LOAD_ERROR;
//        cout << "iModuleMvai5DataBaseBlockWrite NO" << endl;
//        i = -1;//debag//
//    }
//
//    return i;
//}




















////-----------------------------------------------------------------------------------------------------
//// ���������� ���� ������ �� RAM ������� � EEPROM ������.
//int iModuleMvai5ChangeProtocol(TModuleContext *pxModuleContext)
//{
////    unsigned char auiSpiTxBuffer[TX_RX_BUFF_SIZE];
////    unsigned char auiSpiRxBuffer[TX_RX_BUFF_SIZE];
//
//    auiSpiTxBuffer[0] = CHANGE_PROTOCOL_COMMAND;
//    xSpiCommunicationDevice.Exchange(pxModuleContext ->
//                                     xModuleContextStatic.
//                                     ucModuleAddress,
//                                     auiSpiTxBuffer,
//                                     auiSpiRxBuffer,
//                                     SPI_PREAMBLE_LENGTH +
//                                     GET_MODULE_TYPE_COMMAND_ANSWER_LENGTH +
//                                     ONE_BYTE_CRC_LENGTH,
//                                     LOW_SPEED_IN_HZ);
//
//
////    cout << "CHANGE_PROTOCOL_COMMAND " << endl;
////    unsigned char *pucSourceTemp;
////    pucSourceTemp = (unsigned char*)auiSpiRxBuffer;
////    for(int i=0; i<16; )
////    {
////        for(int j=0; j<8; j++)
////        {
////            cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
////        }
////        cout << endl;
////        i += 8;
////    }
//
//    // ������ ��������?
//    if (auiSpiRxBuffer[GET_MODULE_TYPE_COMMAND_OFFSET] ==
//            CHANGE_PROTOCOL_COMMAND) // if echo answer is ok.
//    {
//        // ����� ������� ������ ��� ������?
//        if ((iCrcSummOneByteCompare(&auiSpiRxBuffer[GET_MODULE_TYPE_COMMAND_OFFSET],
//                                    GET_MODULE_TYPE_COMMAND_LENGTH +
//                                    GET_MODULE_TYPE_COMMAND_ANSWER_LENGTH) > 0))
//        {
//            // ���� ������������� ������ ���� ������ � EEPROM ������.
//            cout << "iModuleMvai5ChangeProtocol ok" << endl;
//            return 0;
//        }
//    }
//
//    cout << "iModuleMvai5ChangeProtocol error" << endl;
//
//    return -1;
//}

//-----------------------------------------------------------------------------------------------------
// ���������� ����� ������� � ������� ����������� ������ - MR.
int iModuleMvai5TransferData(TModuleContext *pxModuleContext)
{
    unsigned char ucData;
    unsigned short usData;
    unsigned char ucLocalKvit;
    unsigned char ucLocalReset;
    // ucLocalBou ���� ���������� ������� ����.
    // ���� ���-� f_bou: ����� ���������� ��:  D0-"����.�� ��", D1-"����� �� ��"
    // D2-������"����.�", D3-������"����� �", D4-���� "����.���/����".
    // ���� ���������� �� ����� f_bou.
    // ������������ � �������� ������.
    // #define GLOBAL_KVIT_PC_BIT 0
    // ����� � �������� ������.
    // #define GLOBAL_RESET_PC_BIT 1
    // ������������ � ���������� ������ ����.
    // #define GLOBAL_KVIT_BUTTON_BIT 2
    // ����� � ���������� ������ ����.
    // #define GLOBAL_RESET_BUTTON_BIT 3
    // ����� ����������.
    // #define GLOBAL_BLOCK_BIT 4
    unsigned char ucLocalBou;
    int i;
    unsigned char *pucSource;
    unsigned char *pucDestination;
    TMrXXOneChannelDataBase *pxMrXXOneChannelDataBase;
//    unsigned char auiSpiTxBuffer[TX_RX_BUFF_SIZE];
//    unsigned char auiSpiRxBuffer[TX_RX_BUFF_SIZE];

    // ������� ��������� �� ���� � ���� ������.
    pxMrXXOneChannelDataBase =
        (TMrXXOneChannelDataBase*)((pxModuleContext ->
                                    xModuleContextStatic.
                                    pucDataBasePointer) +
                                   (pxModuleContext ->
                                    xModuleContextStatic.
                                    ucDataBaseDataOffset));

    ucLocalKvit = 0;
    // ���� ��������� ������� �������� ������������.
    for (i = 0; i < MRXX_DISCRETE_OUTPUT_QUANTITY; i++)
    {
        // ��� �������� ����� ������� ������������ �����������������?
        if (pxMrXXOneChannelDataBase[i].ucExternalKvitAddress)
        {
            // ��� �������� ����� ���� ������� ������������?
            if (*((pxModuleContext ->
                    xModuleContextStatic.
                    pucDiValueOriginBufferPointer) +
                    (pxMrXXOneChannelDataBase[i].
                     ucExternalKvitAddress) - 1))
            {
                // ��������� ������������ ��� �������� �����.
                ucLocalKvit |= (1 << i);
            }
        }
    }

    ucLocalReset = 0;
    // ���� ��������� ������� �������� ������.
    for (i = 0; i < MRXX_DISCRETE_OUTPUT_QUANTITY; i++)
    {
        // ��� �������� ����� ������� ����� ����������������?
        if (pxMrXXOneChannelDataBase[i].ucExternalResetAddress)
        {
            // ��� �������� ����� ���� ������� �����?
            if (*((pxModuleContext ->
                    xModuleContextStatic.
                    pucDiValueOriginBufferPointer) +
                    (pxMrXXOneChannelDataBase[i].
                     ucExternalResetAddress) - 1))
            {
                // ��������� ����� ��� �������� �����.
                ucLocalReset |= (1 << i);
            }
        }
    }

    ucLocalBou = 0;
    // ���� ������������ Modbus?
    if (pxModuleContext ->
            xModuleContextDinamic.
            fbGlobalModbusKvit)
    {
        cout << "fbGlobalModbusKvit" << endl;
        // ������� ���� ����������� �������.
        pxModuleContext ->
        xModuleContextDinamic.
        fbGlobalModbusKvit = 0;
        // ��������� ���� ���������� - ������������ Modbus.
        ucLocalBou |= (1 << GLOBAL_KVIT_PC_BIT);
    }
    // ���� ����� Modbus?
    if (pxModuleContext ->
            xModuleContextDinamic.
            fbGlobalModbusReset)
    {
        cout << "fbGlobalModbusReset" << endl;
        // ������� ���� ����������� �������.
        pxModuleContext ->
        xModuleContextDinamic.
        fbGlobalModbusReset = 0;
        // ��������� ���� ���������� - ����� Modbus.
        ucLocalBou |= (1 << GLOBAL_RESET_PC_BIT);
    }
    // ���� ������������ � ������ ����������� ������?
    if (pxModuleContext ->
            xModuleContextDinamic.
            fbGlobalButtonKvit)
    {
        // ������� ���� ����������� �������.
        pxModuleContext ->
        xModuleContextDinamic.
        fbGlobalButtonKvit = 0;
        // ��������� ���� ���������� - ������������ � ����������� ������.
        ucLocalBou |= (1 << GLOBAL_KVIT_BUTTON_BIT);
    }
    // ���� ����� � ������ ����������� ������?
    if (pxModuleContext ->
            xModuleContextDinamic.
            fbGlobalButtonReset)
    {
        // ������� ���� ����������� �������.
        pxModuleContext ->
        xModuleContextDinamic.
        fbGlobalButtonReset = 0;
        // ��������� ���� ���������� - ����� � ����������� ������.
        ucLocalBou |= (1 << GLOBAL_RESET_BUTTON_BIT);
    }
    // ������� ����� ����������?
//    if (pxModuleContext ->
//            xModuleContextDinamic.
//            fbGlobalModbusBlock)
    if (fbGlobalBlock)
    {
//        // ������� ���� ����������� �������.
//        pxModuleContext ->
//        xModuleContextDinamic.
//        fbGlobalModbusBlock = 0;
        // ��������� ���� ���������� - ���������� Modbus.
        ucLocalBou |= (1 << GLOBAL_BLOCK_BIT);
    }
    // ��� ����������� ��� ������?
    if ((!fbModuleMrOutOff) || (!fbPlcErrorReset))
    {
        pucDestination = &auiSpiTxBuffer[SPI_DATA_BYTE_OFFSET];
        pucSource = (pxModuleContext ->
                     xModuleContextStatic.
                     pucDoValueBlockBufferPointer);
        ucData = 0;
        // �������� ���������� ��������� ��� ������� ����.
        for (i = 0; i < MRXX_DISCRETE_OUTPUT_QUANTITY; i++)
        {
            if (pucSource[i])
            {
                ucData |= (1 << i);
            }
            else
            {

            }
        }

//        if (ucData)
//        {
//            cout  << "ucData" << " " <<  hex << uppercase << setw(2) << setfill('0') << (unsigned int)ucData << endl;
//        }
        *pucDestination++ = ucData;
//        cout << "iModuleMrXXTransferData ucData 1 " << std::showbase << std::dec << (int)ucData << std::endl;

        pucSource = (pxModuleContext ->
                     xModuleContextStatic.
                     pucDoValueBufferPointer);
        ucData = 0;
        // �������� ����� ��������� ��� ������� ����.
        for (i = 0; i < MRXX_DISCRETE_OUTPUT_QUANTITY; i++)
        {
            if (pucSource[i])
            {
                ucData |= (1 << i);
            }
            else
            {

            }
        }

//        if (ucData)
//        {
//            cout  << "ucData2" << " " <<  hex << uppercase << setw(2) << setfill('0') << (unsigned int)ucData << endl;
//        }
        *pucDestination++ = ucData;
//        cout << "iModuleMrXXTransferData ucData 2 " << std::showbase << std::dec << (int)ucData << std::endl;
    }
    else
    {
        // ���� ����������� ��� ������.
        *pucDestination++ = 0;
        *pucDestination++ = 0;
    }



    auiSpiTxBuffer[0] = MRXX_GET_DISCRETE_OUTPUT_DATA_COMMAND;
    auiSpiTxBuffer[1] = MRXX_GET_DISCRETE_OUTPUT_DATA_COMMAND_ANSWER_LENGTH;

    *pucDestination++ = ucLocalKvit;
    *pucDestination++ = ucLocalReset;

    *pucDestination++ = ucLocalBou;
    *pucDestination = HammingDistanceCrc(&auiSpiTxBuffer[0],
                                         (SPI_PREAMBLE_LENGTH +
                                          MRXX_GET_DISCRETE_OUTPUT_DATA_COMMAND_ANSWER_LENGTH));


    xSpiCommunicationDevice.Exchange(pxModuleContext ->
                                     xModuleContextStatic.
                                     ucModuleAddress,
                                     auiSpiTxBuffer,
                                     auiSpiRxBuffer,
                                     SPI_PREAMBLE_LENGTH +
                                     MRXX_GET_DISCRETE_OUTPUT_DATA_COMMAND_ANSWER_LENGTH +
                                     ONE_BYTE_CRC_LENGTH,
                                     SPEED_IN_HZ
                                    );


    // ������ �� ����������?
    if (auiSpiRxBuffer[(SPI_MT_RX_OFFSET +
                        SPI_MT_PRIAMBLE_LENGTH +
                        auiSpiRxBuffer[SPI_MT_RX_OFFSET + SPI_MT_LENGTH_OFFSET])] ==
            HammingDistanceCrc(&auiSpiRxBuffer[SPI_MT_RX_OFFSET],
                               (SPI_MT_PRIAMBLE_LENGTH +
                                auiSpiRxBuffer[SPI_MT_RX_OFFSET + SPI_MT_LENGTH_OFFSET])))
    {
        return 0;
    }
    else
    {
        return -1;
    }
}

//-----------------------------------------------------------------------------------------------------
// ���������� ���� ������ �� RAM ������� � EEPROM ������.
int iModuleMvai5DataBaseBlockWrite(TModuleContext *pxModuleContext)
{
    unsigned short usData;
    int i;
    unsigned char *pucSource;
    unsigned char *pucDestination;
    unsigned int nuiBusyTimeCounter;
    unsigned char aucTempArray[PLC_DATA_BASE_BLOCK_LENGTH];
//    unsigned char auiSpiTxBuffer[TX_RX_BUFF_SIZE];
//    unsigned char auiSpiRxBuffer[TX_RX_BUFF_SIZE];

// � ������� ����������� ����� � � ������ �������, ���� ������ �������� � ������ ��������.
//  � ������� ����������� ����� ������� ���� ������ ���������������� ������(6 ������, �� 20 ����) 120 ����, �����
// ��������� ��������� ������(6 ������, �� 8 ����) 48 ����. 6 ������(120 + 48 = 168) ����.
//  � ������ ������� ������ ���������������� � ��������� ��������� ���� ���������� ������� ��� ������� �����
// (���� ����: 20 ���� - ������ ���������������� � 8 ���� - ��������� ���������). ���� ���� - 28 ����. 6 ������(6 * 28 = 168) ����.
// ����� ������������ �������������� ������� ���� ������, �� ������� � ������.
    memset(&aucTempArray[0],
           0,
           sizeof(aucTempArray));
    // ������� ��������� �� ���� ���� ������, �������� �� Modbus �� ��������� �����.

    pucSource =
        ((pxModuleContext ->
          xModuleContextStatic.
          pucDataBasePointer) +
         (pxModuleContext ->
          xModuleContextStatic.
          ucDataBaseDataOffset));

    // ������� ��������� �� ������ ���������������� ������� �����, ��� ������ � ������.
    pucDestination = &aucTempArray[SPI_DATA_BYTE_OFFSET];
    for (i = 0; i < MVAI5_ANALOG_INPUT_QUANTITY; i++)
    {
        // ��������� �� ��������� ����� ����� ����� - ������ ���������������� ������ �����(20 ����).
        memcpy(pucDestination, pucSource, 20);
        // ������� � ������ ���������������� ���������� ����� � ������(+20 ����).
        pucDestination += 20;
        // ������� � ���������� ����� ������ ������ ����� � �������(+28 ����).
        pucSource += 28;
    }
    // ������� ��������� �� ���� ���� ������, �������� �� Modbus �� ��������� �����.

    pucSource =
        ((pxModuleContext ->
          xModuleContextStatic.
          pucDataBasePointer) +
         (pxModuleContext ->
          xModuleContextStatic.
          ucDataBaseDataOffset));

    // ������� � ������ ���������� ��������� ������� �����(+20 ����).
    pucSource += 20;
    // ������� ��������� �� ������ ���������� ��������� ������� �����, ��� ������ � ������.
    pucDestination = &aucTempArray[SPI_DATA_BYTE_OFFSET + (20 * 6)];
    for (i = 0; i < MVAI5_ANALOG_INPUT_QUANTITY; i++)
    {
        // ��������� �� ��������� ����� ����� ����� - ��������� ��������� ������ �����(8 ����).
        memcpy(pucDestination, pucSource, 8);
        // ������� � ������ ���������� ��������� ���������� ����� � ������(+8 ����).
        pucDestination += 8;
        // ������� � ���������� ����� ������ ������ ����� � �������(+28 ����).
        pucSource += 28;
    }
    // ������� ��������� �� ��������������� ���� ������ �� ������� � ������.
    pucSource = &aucTempArray[SPI_DATA_BYTE_OFFSET];
    usData = usCrcSummTwoByteCalculation(pucSource,
                                         ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_LENGTH);
    // ������� ��������� �� ����� CRC � ������.
    pucDestination = &aucTempArray[SPI_DATA_BYTE_OFFSET +
                                   ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_LENGTH];
    *pucDestination++ = (unsigned char)usData;
    *pucDestination = (unsigned char)(usData >> 8);
    // ��������� ��������������� ���� ������ �� ���������� � ����� TX SPI.
    memcpy(&auiSpiTxBuffer[SPI_DATA_BYTE_OFFSET],
           pucSource,
           ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_LENGTH +
           TWO_BYTE_CRC_LENGTH);
    // �������� ������ � ������.
    auiSpiTxBuffer[0] = MVAI5_SET_DATA_BASE_COMMAND;
    xSpiCommunicationDevice.Exchange(pxModuleContext ->
                                     xModuleContextStatic.
                                     ucModuleAddress,
                                     auiSpiTxBuffer,
                                     auiSpiRxBuffer,
                                     SPI_PREAMBLE_LENGTH +
                                     ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_LENGTH +
                                     TWO_BYTE_CRC_LENGTH +
                                     TAIL_ANSWER_LENGTH,
                                     LOW_SPEED_IN_HZ);

    pxModuleContext ->
    xModuleContextDinamic.
    usAuxiliaryCounter = 0;

    return 0;
}

//-----------------------------------------------------------------------------------------------------
int iModuleMvai5DataBaseBlockWriteError(TModuleContext *pxModuleContext)
{
    // ���� ������ �� ��������?
    if ((pxModuleContext ->
            xModuleContextDinamic.
            usAuxiliaryCounter) >= 20)
    {
        // ������ �� ���������, ����� �����.
        fbAnalogueInputModuleDataBaseError = 1;
        // ������� ��� ������;
        (pxModuleContext ->
         xModuleContextDinamic.
         ucErrorCode) = INTERNAL_MODULE_ERROR_DATA_BASE;

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
                 CEvents::HANDLED_EVENTS_IS_POPUP |
                 CEvents::HANDLED_EVENTS_IS_SOUND |
                 CEvents::HANDLED_EVENTS_IS_ARCHIVE),
                MTVI5_DATA_BASE_ERROR_OFFSET,
                "����. �. ���");
        }

        return -1;
    }
    else
    {
        // �������� �������� �������� - "���������� ������� ����� � ������� ��� ������".
        (pxModuleContext ->
         xModuleContextDinamic.
         usAuxiliaryCounter)++;

        return 0;
    }
}

//-----------------------------------------------------------------------------------------------------
// ���������� ���� ������ �� RAM ������� � EEPROM ������.
int iModuleMvai5DataBaseBlockWriteCheck(TModuleContext *pxModuleContext)
{
    unsigned short usData;
    int i;
    unsigned char *pucSource;
    unsigned char *pucDestination;
    unsigned int nuiBusyTimeCounter;
    unsigned char aucTempArray[PLC_DATA_BASE_BLOCK_LENGTH];
//    unsigned char auiSpiTxBuffer[TX_RX_BUFF_SIZE];
//    unsigned char auiSpiRxBuffer[TX_RX_BUFF_SIZE];

    // ������ ������ � ����������� ����������������.
    auiSpiTxBuffer[0] = MVAI5_CHECK_DATA_BASE_WRITE_COMMAND;
    xSpiCommunicationDevice.Exchange(pxModuleContext ->
                                     xModuleContextStatic.
                                     ucModuleAddress,
                                     auiSpiTxBuffer,
                                     auiSpiRxBuffer,
                                     SPI_PREAMBLE_LENGTH +
                                     TAIL_ANSWER_LENGTH,
                                     LOW_SPEED_IN_HZ);

    return 0;

//    // ���� ������������� ������ ���� ������ � EEPROM ������?
//    if((auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET]) == DATA_EXCHANGE_OK)
//    {
//        // ���� ������������� ������ ���� ������ � EEPROM ������.
////        ucDataBaseError = PROGRAMMING_DATA_BASE_WRITE_OK;
//        cout << "iModuleMvai5DataBaseBlockWrite OK" << endl;
//        return 0;
//    }
//    // ������ ����� ������� ������ � EEPROM?
//    else if ((auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET]) == MVAI5_CHECK_DATA_BASE_WRITE_ANSWER_BUSY)
//    {
//        // ������ ����� ������� ������ � EEPROM.
////        ucDataBaseError = PROGRAMMING_DATA_BASE_WRITE_BLOCK_BUSY;
//        cout << "iModuleMvai5DataBaseBlockWrite BUSY" << endl;
//        return -1;
//    }
//    // ��������� ������ ��� ������ ���� ������ � EEPROM ������?
//    else if ((auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET]) == DATA_EXCHANGE_ERROR)
//    {
//        // ��������� ������ ��� ������ ���� ������ � EEPROM ������.
//        ucDataBaseError = PROGRAMMING_DATA_BASE_WRITE_BLOCK_MVA_EEPROM_ERROR;
//        cout << "iModuleMvai5DataBaseBlockWrite ERROR" << endl;
//        return -1;
//    }
//    else
//    {
//        // ��������� ������ ��� ������ ���� ������ � EEPROM ������.
//        ucDataBaseError = PROGRAMMING_DATA_BASE_WRITE_BLOCK_MVA_EEPROM_ERROR;
//        cout << "iModuleMvai5DataBaseBlockWrite ERROR" << endl;
//        return -1;
//    }
}

//-----------------------------------------------------------------------------------------------------
// ������� �������.
void vModuleMvai5DataHandlerNew(TModuleContext *pxModuleContext)
{
    switch (pxModuleContext ->
            xModuleContextDinamic.
            ucFsmState)
    {
    case MVAI5_IDDLE:
        break;

//    case MVAI5_CHANGE_PROTOCOL:
//        cout << "MVAI5_CHANGE_PROTOCOL" << endl;
//        if (iModuleMvai5ChangeProtocol(pxModuleContext) != -1)
//        {
//            pxModuleContext ->
//            xModuleContextDinamic.
//            ucFsmState = MVAI5_DATA_EXCHANGE;
//        }
//        break;

    case MVAI5_WRITE_DATA_BASE:
        cout << "MVAI5_WRITE_DATA_BASE" << endl;
        iModuleMvai5DataBaseBlockWrite(pxModuleContext);
        pxModuleContext ->
        xModuleContextDinamic.
        ucFsmState = MVAI5_WRITE_DATA_BASE_CHECK;
        break;

    case MVAI5_WRITE_DATA_BASE_CHECK:
        cout << "MVAI5_WRITE_DATA_BASE_CHECK" << endl;
        iModuleMvai5DataBaseBlockWriteCheck(pxModuleContext);

        // �������� ����� ������ �� ���������� ������� - ������ ���� ������.
        switch (auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET])
        {
            // ���� ������ �������� �������?
        case DATA_EXCHANGE_OK:
            cout << "DATA_EXCHANGE_OK" << endl;

            iPlcDataBaseBlockWrite(pxModuleContext ->
                                   xModuleContextDinamic.
                                   ucCommonIndex);

            vWorkArraysUpdate();

            ucDataBaseError = PROGRAMMING_DATA_BASE_WRITE_OK;
            cout << "iModuleMvai5DataBaseBlockWrite OK" << endl
                 ;
            pxModuleContext ->
            xModuleContextDinamic.
            ucCommonIndex = 0;

            pxModuleContext ->
            xModuleContextDinamic.
            ucFsmState = MVAI5_DATA_EXCHANGE;
            break;
            // �� ����� ������ ���� ������ ��������� ������?
        case MVAI5_CHECK_DATA_BASE_WRITE_ANSWER_BUSY:
            cout << "MVAI5_CHECK_DATA_BASE_WRITE_ANSWER_BUSY" << endl;
            // ������ ����� ������� ������ � EEPROM.
            ucDataBaseError = PROGRAMMING_DATA_BASE_WRITE_BLOCK_BUSY;
            cout << "iModuleMvai5DataBaseBlockWrite BUSY" << endl;
            if (iModuleMvai5DataBaseBlockWriteError(pxModuleContext) == -1)
            {

                // ��� ������ � ������ ��������� ������.
                // ����������� ���� ������.
                iPlcDataBaseCheck();
                vWorkArraysUpdate();

                // ��������� ������ ��� ������ ���� ������ � EEPROM ������.
                ucDataBaseError = PROGRAMMING_DATA_BASE_WRITE_BLOCK_MVA_EEPROM_ERROR;
                cout << "iModuleMvai5DataBaseBlockWrite ERROR" << endl;
                ;
                pxModuleContext ->
                xModuleContextDinamic.
                ucCommonIndex = 0;

                pxModuleContext ->
                xModuleContextDinamic.
                ucFsmState = MVAI5_DATA_EXCHANGE;
            }
            break;

            // �� ����� ������ ���� ������ ��������� ������?
        case DATA_EXCHANGE_ERROR:
            cout << "DATA_EXCHANGE_ERROR" << endl;

            // ��� ������ � ������ ��������� ������.
            // ����������� ���� ������.
            iPlcDataBaseCheck();
            vWorkArraysUpdate();

            // ��������� ������ ��� ������ ���� ������ � EEPROM ������.
            ucDataBaseError = PROGRAMMING_DATA_BASE_WRITE_BLOCK_MVA_EEPROM_ERROR;
            cout << "iModuleMvai5DataBaseBlockWrite ERROR" << endl;
            ;
            pxModuleContext ->
            xModuleContextDinamic.
            ucCommonIndex = 0;

            pxModuleContext ->
            xModuleContextDinamic.
            ucFsmState = MVAI5_DATA_EXCHANGE;
            break;

            // ��������� ������ ������?
        default:
            cout << "DATA_EXCHANGE_ERROR default" << endl;

            if (iModuleMvai5DataBaseBlockWriteError(pxModuleContext) == -1)
            {
                // ��� ������ � ������ ��������� ������.
                // ����������� ���� ������.
                iPlcDataBaseCheck();
                vWorkArraysUpdate();

                // ��������� ������ ��� ������ ���� ������ � EEPROM ������.
                ucDataBaseError = PROGRAMMING_DATA_BASE_WRITE_BLOCK_MVA_EEPROM_ERROR;
                cout << "iModuleMvai5DataBaseBlockWrite ERROR" << endl;
                ;
                pxModuleContext ->
                xModuleContextDinamic.
                ucCommonIndex = 0;

                pxModuleContext ->
                xModuleContextDinamic.
                ucFsmState = MVAI5_DATA_EXCHANGE;
            }

            break;
        }

        break;

    case MVAI5_DATA_EXCHANGE:
////        cout << "DATA_EXCHANGE" << endl;
        iModuleMvai5DataHandler(pxModuleContext);

//        if (iModuleMvai5TransferData(pxModuleContext) != -1)
//        {
//            // �������� ����� ������ �� ���������� ������� - ������ ���� ������.
//            switch (auiSpiRxBuffer[(SPI_MT_RX_OFFSET + SPI_MT_FUNCTION_CODE_OFFSET)])
//            {
//                // ����� ������� ������ ��� ������?
//            case MVAI5_DATA_READY:
////                cout << "MVAI5_DATA_READY" << endl;
////                iModuleMvai5TransferDataReceive(pxModuleContext);
//                break;
//                // ��������� �������� ���� ������?
//            case MVAI5_DATA_BASE_ERROR:
//                cout << "MVAI5_DATA_BASE_ERROR" << endl;
//                pxModuleContext ->
//                xModuleContextDinamic.
//                ucFsmState = MVAI5_WRITE_DATA_BASE;
//                break;
//                // ��������� ������ ������?
//            default:
//                cout << "MVAI5_SET_DATA_BASE_COMMAND_ERROR default" << endl;
////                iModuleMvai5TransferDataReceiveError(pxModuleContext);
//                break;
//            }
//        }
//        else
//        {
//            cout << "MVAI5_DATA_EXCHANGE  error" << endl;
////            iModuleMvai5TransferDataReceiveError(pxModuleContext);
//        }
        break;

    default:
        break;
    }
}


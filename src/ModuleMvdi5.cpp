
#include "PasNewConfig.h"
#include "ModuleMvdi5.h"
#include "Events.h"

using namespace std;

//-----------------------------------------------------------------------------------------------------
// ���������� ����� ������� � ������� ����������� ����� - MVDI5.
int iModuleMvdiDataHandler(TModuleContext *pxModuleContext)
{
    unsigned char ucData;
    int i;
    int j;
    unsigned char *pucDestination;
    unsigned char auiSpiTxBuffer[TX_RX_BUFF_SIZE];
    unsigned char auiSpiRxBuffer[TX_RX_BUFF_SIZE];

    // ����� ������� �� SPI.
    auiSpiTxBuffer[0] = MVDI5_GET_DISCRETE_INPUT_DATA_COMMAND;
    xSpiCommunicationDevice.Exchange(pxModuleContext ->
                                     xModuleContextStatic.
                                     ucModuleAddress,
                                     auiSpiTxBuffer,
                                     auiSpiRxBuffer,
                                     SPI_PREAMBLE_LENGTH +
                                     MVDI5_GET_DISCRETE_INPUT_DATA_COMMAND_ANSWER_LENGTH +
                                     ONE_BYTE_CRC_LENGTH,
                                     SPEED_IN_HZ
                                    );

    // ������ ��������?
    if (auiSpiRxBuffer[SPI_COMMAND_BYTE_OFFSET] == MVDI5_GET_DISCRETE_INPUT_DATA_COMMAND) // if echo answer is ok.
    {
        // ������ �� ����������?
        if ((iCrcSummOneByteCompare(&auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET],
                                    MVDI5_GET_DISCRETE_INPUT_DATA_COMMAND_ANSWER_LENGTH) > 0) &&
                ((auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET +
                                 MVDI5_GET_DISCRETE_INPUT_DATA_COMMAND_ANSWER_LENGTH]) != 0))
        {
            // ������ ��������.
            pxModuleContext ->
            xModuleContextDinamic.
            nucBadAnswerCounter = BAD_MODULE_CYCLE_COUNT_DEFAULT;
            // ������� ���� ������ ������.
            *(pxModuleContext ->
              xModuleContextStatic.
              pucModuleBadStateBufferPointer) = BAD_MODULE_RESPONDED_OK;
            //iBadModuleBuffUpdate();

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

            // ��� ������� ������?
            switch(auiSpiRxBuffer[SPI_COMMAND_BYTE_OFFSET])
            {
            case MVDI5_DATA_READY:
                // ������ ������ ����������.
                memset(pxModuleContext ->
                       xModuleContextStatic.
                       pucBadDiBufferPointer,
                       0,
                       MVDI5_DISCRETE_INPUT_QUANTITY);
                // �������� ���������� �� ������ ������ � ������� ������ Modbus �������.
                pucDestination = pxModuleContext ->
                                 xModuleContextStatic.
                                 pucDiValueBufferPointer;
                for (i = 0; i < MVDI5_DISCRETE_INPUT_BYTE_QUANTITY; i++)
                {
                    ucData = (auiSpiRxBuffer[i + SPI_DATA_BYTE_OFFSET]);

                    for (j = 0; j < MVDI5_DISCRETE_INPUT_BITS_IN_BYTE_QUANTITY; j++)
                    {
                        // ����������� ��������. � ������ ��������� ����������� ������� - 1, ��������� ��������� - 0.
                        *pucDestination++ = !((ucData & (1 << j)) ? 1 : 0);
                    }
                }
                break;

            case MVDI5_DATA_NOT_READY:
                break;

            default:
                break;
            };
            return 0;
        }
    }

    // ������ ������� �����������?
    if ((pxModuleContext ->
            xModuleContextDinamic.
            nucBadAnswerCounter) == 0)
    {
        // ������ �� ��������.
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

        // ������ ������ ������������.
        memset(pxModuleContext ->
               xModuleContextStatic.
               pucBadDiBufferPointer,
               1,
               MVDI5_DISCRETE_INPUT_QUANTITY);
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


#include "PasNewConfig.h"
#include "ModuleMvds9.h"
#include "Events.h"

using namespace std;

//-----------------------------------------------------------------------------------------------------
// ���������� ����� ������� � ������� ����������� ����� - MVDS9.
int iModuleMvds9DataHandler(TModuleContext *pxModuleContext)
{
    unsigned char ucData;
    int i;
    int j;
    unsigned char *pucDestination;
    unsigned char *pucDestination2;
    unsigned char auiSpiTxBuffer[TX_RX_BUFF_SIZE];
    unsigned char auiSpiRxBuffer[TX_RX_BUFF_SIZE];

    // ����� ������� �� SPI.
    auiSpiTxBuffer[0] = MVDS9_GET_DISCRETE_INPUT_DATA_COMMAND;
    xSpiCommunicationDevice.Exchange(pxModuleContext ->
            xModuleContextStatic.
            ucModuleAddress,
                                     auiSpiTxBuffer,
                                     auiSpiRxBuffer,
                                     SPI_PREAMBLE_LENGTH +
                                     MVDS9_GET_DISCRETE_INPUT_DATA_COMMAND_ANSWER_LENGTH +
                                     ONE_BYTE_CRC_LENGTH,
                                     SPEED_IN_HZ
                                    );
    // ������ ��������?
    if (auiSpiRxBuffer[SPI_COMMAND_BYTE_OFFSET] == MVDS9_GET_DISCRETE_INPUT_DATA_COMMAND) // if echo answer is ok.
    {
        // ������ �� ����������?
        if ((iCrcSummOneByteCompare(&auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET],
                                    MVDS9_GET_DISCRETE_INPUT_DATA_COMMAND_ANSWER_LENGTH) > 0) &&
                ((auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET +
                                 MVDS9_GET_DISCRETE_INPUT_DATA_COMMAND_ANSWER_LENGTH]) != 0))
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
            // ��� ������� ������?
            switch(auiSpiRxBuffer[SPI_COMMAND_BYTE_OFFSET])
            {
            case MVDS9_DATA_READY:
                // ������ ������ ����������.
                memset(pxModuleContext ->
                       xModuleContextStatic.
                       pucBadDiBufferPointer,
                       0,
                       MVDS9_DISCRETE_INPUT_QUANTITY);
                // �������� ���������� �� ������ ������ � ������� ������ Modbus �������.
                pucDestination = pxModuleContext ->
                                 xModuleContextStatic.
                                 pucDiValueBufferPointer;
                pucDestination2 = pxModuleContext ->
                                  xModuleContextStatic.
                                  pucBadDiBufferPointer;

                for (i = 0; i < MVDS9_DISCRETE_INPUT_BYTE_QUANTITY; i++)
                {
                    ucData = (auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET + i]);

                    for (j = 0; j < MVDS9_DISCRETE_INPUT_BITS_IN_BYTE_QUANTITY; j++)
                    {
                        // ������ ���� (D0,D2,D4,D6) = 0 - ���������� (OFF), = 1 - �������� (ON)
                        // �������� ���� (D1,D3,D5,D7) = 0 - ����������, = 1 - ������������
                        // ��� ��������?
                        if (j & 0x01)
                        {
                            // ��� ����������? 0 - ����������, 1 - ������������.
                            *pucDestination2++ = ((ucData & (1 << j)) ? 1 : 0);
                        }
                        else
                        {
                            //�������� ��������?  0 - ���������� (OFF), 1 - �������� (ON)
                            *pucDestination++ = ((ucData & (1 << j)) ? 1 : 0);
                        }
                    }
                }
                break;

            case MVDS9_DATA_NOT_READY:
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
        // ������ ����������.
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
               MVDS9_DISCRETE_INPUT_QUANTITY);
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
// ���������� ���������� ���� ������ ������ � ����� � ������ �������.
// ���� �� ���������, ������� ���� �� ������ � ������.
int iModuleMvds9PlcDataBaseCheck(TModuleContext *pxModuleContext)
{
    unsigned short usData;
    int i;
    unsigned char *pucSource;
    unsigned char *pucDestination;
    unsigned char auiSpiTxBuffer[TX_RX_BUFF_SIZE];
    unsigned char auiSpiRxBuffer[TX_RX_BUFF_SIZE];

    // ����� ������� �� SPI.
    auiSpiTxBuffer[0] = MVDS9_GET_DATA_BASE_COMMAND;
    xSpiCommunicationDevice.Exchange(pxModuleContext ->
            xModuleContextStatic.
            ucModuleAddress,
                                     auiSpiTxBuffer,
                                     auiSpiRxBuffer,
                                     SPI_PREAMBLE_LENGTH +
                                     MVDS9_DATA_BASE_BLOCK_LENGTH +
                                     TWO_BYTE_CRC_LENGTH,
                                     LOW_SPEED_IN_HZ);

    // ������ ��������?
    if (auiSpiRxBuffer[SPI_COMMAND_BYTE_OFFSET] == MVDS9_GET_DATA_BASE_COMMAND) // if echo answer is ok.
    {
        // ������ �� ����������?
        if (iCrcSummTwoByteCompare(&auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET],
                                   MVDS9_DATA_BASE_BLOCK_LENGTH) > 0)
        {
            pucSource = &auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET];
            // ������� ��������� �� ���� � ���� ������.
            pucDestination = (unsigned char*)((pxModuleContext ->
                                               xModuleContextStatic.
                                               pucDataBasePointer) +
                                              (pxModuleContext ->
                                               xModuleContextStatic.
                                               ucDataBaseDataOffset));
            // ���� ������ ������ ��������� � ����� � ������ �������?
            if (memcmp((const void*)pucDestination,
                       (const void*)pucSource,
                       MVDS9_DATA_BASE_BLOCK_LENGTH) != 0)
            {
                // ���� ������ ������ �� ��������� � ����� � ������ �������.
                // �������� � ������ ������� ���� ������ ����������� �� ������.
                memcpy(pucDestination,
                       pucSource,
                       MVDS9_DATA_BASE_BLOCK_LENGTH);
                cout << "iModuleMvds9PlcDataBaseCheck NO" << endl;
                return -1;
            }
            else
            {
                cout << "iModuleMvds9PlcDataBaseCheck OK" << endl;
                return 0;
            }
        }
    }
    return -1;
}

//-----------------------------------------------------------------------------------------------------
// ���������� ���������� ���� ������ ������ � ����� � ������ �������.
// ���� �� ���������, ������� ���� �� ������� � ������.
int iModuleMvds9ModuleDataBaseCheck(TModuleContext *pxModuleContext)
{
    unsigned short usData;
    unsigned short usCrc;
    int i;
    unsigned char *pucSource;
    unsigned char *pucDestination;
    unsigned char auiSpiTxBuffer[TX_RX_BUFF_SIZE];
    unsigned char auiSpiRxBuffer[TX_RX_BUFF_SIZE];

    // ����� ������� �� SPI.
    auiSpiTxBuffer[0] = MVDS9_GET_DATA_BASE_COMMAND;
    xSpiCommunicationDevice.Exchange(pxModuleContext ->
            xModuleContextStatic.
            ucModuleAddress,
                                     auiSpiTxBuffer,
                                     auiSpiRxBuffer,
                                     SPI_PREAMBLE_LENGTH +
                                     MVDS9_DATA_BASE_BLOCK_LENGTH +
                                     TWO_BYTE_CRC_LENGTH,
                                     LOW_SPEED_IN_HZ);

    // ������ ��������?
    if (auiSpiRxBuffer[SPI_COMMAND_BYTE_OFFSET] == MVDS9_GET_DATA_BASE_COMMAND) // if echo answer is ok.
    {
        // ������ �� ����������?
        if (iCrcSummTwoByteCompare(&auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET],
                                   MVDS9_DATA_BASE_BLOCK_LENGTH) > 0)
        {
            pucSource = &auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET];
            // ������� ��������� �� ���� � ���� ������.
            pucDestination = (unsigned char*)((pxModuleContext ->
                                               xModuleContextStatic.
                                               pucDataBasePointer) +
                                              (pxModuleContext ->
                                               xModuleContextStatic.
                                               ucDataBaseDataOffset));
            // ���� ������ ������ ��������� � ����� � ������ �������?
            if (memcmp((const void*)pucDestination,
                       (const void*)pucSource,
                       MVDS9_DATA_BASE_BLOCK_LENGTH) != 0)
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
                    // ���� ������ ������ �� ��������� � ����� � ������ �������.
                    // �������� � ������ ������ ���� ������ ����������� �� �������.
                    iModuleMvds9DataBaseBlockWrite(pxModuleContext);
                    cout << "New data base saved in module" << endl;
                }
                cout << "iModuleMvds9ModuleDataBaseCheck NO" << endl;
                return -1;
            }
            else
            {
                cout << "iModuleMvds9ModuleDataBaseCheck OK" << endl;
                return 0;
            }
        }
    }
    return -1;
}

//-----------------------------------------------------------------------------------------------------
// ���������� ���� ������ �� RAM ������� � EEPROM ������.
int iModuleMvds9DataBaseBlockWrite(TModuleContext *pxModuleContext)
{
    unsigned short usData;
    unsigned char *pucSource;
    unsigned char *pucDestination;
    unsigned char auiSpiTxBuffer[TX_RX_BUFF_SIZE];
    unsigned char auiSpiRxBuffer[TX_RX_BUFF_SIZE];

    // ������� ��������� �� ���� � ���� ������.
    pucSource =
        ((pxModuleContext ->
          xModuleContextStatic.
          pucDataBasePointer) +
         (pxModuleContext ->
          xModuleContextStatic.
          ucDataBaseDataOffset));

    pucDestination = &auiSpiTxBuffer[0];
    *pucDestination++ = MVDS9_SET_DATA_BASE_COMMAND;
    *pucDestination++ = 0;
    *pucDestination++ = *pucSource++;
    *pucDestination++ = *pucSource;
    usData = usCrcSummTwoByteCalculation(&auiSpiTxBuffer[SPI_DATA_BYTE_OFFSET],
                                         MVDS9_DATA_BASE_BLOCK_LENGTH);
    *pucDestination++ = (unsigned char)usData;
    *pucDestination = (unsigned char)(usData >> 8);

    // ����� ������� �� SPI.
    xSpiCommunicationDevice.Exchange(pxModuleContext ->
            xModuleContextStatic.
            ucModuleAddress,
                                     auiSpiTxBuffer,
                                     auiSpiRxBuffer,
                                     SPI_PREAMBLE_LENGTH +
                                     MVDS9_DATA_BASE_BLOCK_LENGTH +
                                     TWO_BYTE_CRC_LENGTH +
                                     TAIL_ANSWER_LENGTH,
                                     LOW_SPEED_IN_HZ);
    // ���� ������ �������� � ������ ������?
    if ((auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET +
                        MVDS9_DATA_BASE_BLOCK_LENGTH +
                        TWO_BYTE_CRC_LENGTH]) ==
            MVDS9_CHECK_DATA_BASE_WRITE_ANSWER_OK)
    {
        return 0;
    }
    return -1;
}

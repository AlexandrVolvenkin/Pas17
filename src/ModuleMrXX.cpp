

//-----------------------------------------------------------------------------------------------------
#include "PasNewConfig.h"
#include "Platform.h"
#include "ModuleMrXX.h"
#include "Events.h"
#include "SystemComponent.h"

using namespace std;

static unsigned char auiSpiTxBuffer[TX_RX_BUFF_SIZE];
static  unsigned char auiSpiRxBuffer[TX_RX_BUFF_SIZE];

//void vModuleMrXXDataHandlerNew(TModuleContext *pxModuleContext)
//{
//    switch(pxModuleContext ->
//            xModuleContextDinamic.
//            ucFsmState)
//    {
//    case MRXX_FSM_STATE_CHANGE_PROTOCOL:
//        iModuleMrXXChangeProtocol(pxModuleContext);
//        pxModuleContext ->
//        xModuleContextDinamic.
//        ucFsmState = MRXX_FSM_STATE_WRITE_DATABASE;
//        break;
//
//    case MRXX_FSM_STATE_TRANSFER:
//        iModuleMrXXTransferData(pxModuleContext);
//        break;
//        // ��� ������ ���� ������ ������������ �� ������� � ������ ����.
//    case MRXX_FSM_STATE_WRITE_DATABASE:
//        iModuleMrXXDataBaseBlockWrite(pxModuleContext);
//        // �����, ������� ��������� � ����� - ����� ������� � �������(MRXX_FSM_STATE_TRANSFER).
//        pxModuleContext ->
//        xModuleContextDinamic.
//        ucFsmState = MRXX_FSM_STATE_TRANSFER;
//        break;
//
//    default:
//        break;
//    };
//}

//-----------------------------------------------------------------------------------------------------
// ���������� ����� ������� � ������� ����������� ������ - MR.
int iModuleMrXXTransferData(TModuleContext *pxModuleContext)
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
        cout << "fbGlobalModbusKvit ok" << endl;
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
        cout << "fbGlobalModbusReset ok" << endl;
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

//    if (ucLocalBou)
//    {
//        cout  << "ucLocalBou" << " " <<  hex << uppercase << setw(2) << setfill('0') << (unsigned int)ucLocalBou << endl;
//    }

    *pucDestination++ = ucLocalBou;
    *pucDestination = HammingDistanceCrc(&auiSpiTxBuffer[0],
                                         (SPI_PREAMBLE_LENGTH +
                                          MRXX_GET_DISCRETE_OUTPUT_DATA_COMMAND_ANSWER_LENGTH));

//    cout  << "HammingDistanceCrc" << " " <<  hex << uppercase << setw(2) << setfill('0') << (unsigned int)*pucDestination << endl;

//    *pucDestination++ = (unsigned char)usData;
//    *pucDestination++ = (unsigned char)(usData >> 8);
//    *pucDestination++ = DATA_EXCHANGE_OK;
//
//if (auiSpiTxBuffer[SPI_DATA_BYTE_OFFSET])
//{
//    cout << "SPI_DATA_BYTE_OFFSET" << endl;
//    cout << "auiSpiTxBuffer" << endl;
//    unsigned char *pucSourceTemp;
//    pucSourceTemp = (unsigned char*)auiSpiTxBuffer;
//    for(int i=0; i<16; )
//    {
//        for(int j=0; j<8; j++)
//        {
//            cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
//        }
//        cout << endl;
//        i += 8;
//    }
//}
//
//if (auiSpiTxBuffer[SPI_DATA_BYTE_OFFSET + 1])
//    {
//        cout << "SPI_DATA_BYTE_OFFSET + 1" << endl;
//        cout << "auiSpiTxBuffer" << endl;
//        unsigned char *pucSourceTemp;
//        pucSourceTemp = (unsigned char*)auiSpiTxBuffer;
//        for(int i=0; i<32; )
//        {
//            for(int j=0; j<8; j++)
//            {
//                cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
//            }
//            cout << endl;
//            i += 8;
//        }
//    }


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


//if (auiSpiTxBuffer[SPI_DATA_BYTE_OFFSET])
//    {
//        cout << "SPI_DATA_BYTE_OFFSET" << endl;
//        cout << "auiSpiRxBuffer" << endl;
//        unsigned char *pucSourceTemp;
//        pucSourceTemp = (unsigned char*)auiSpiRxBuffer;
//        for(int i=0; i<16; )
//        {
//            for(int j=0; j<8; j++)
//            {
//                cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
//            }
//            cout << endl;
//            i += 8;
//        }
//    }
//
//if (auiSpiTxBuffer[SPI_DATA_BYTE_OFFSET + 1])
//{
//        cout << "SPI_DATA_BYTE_OFFSET + 1" << endl;
//        cout << "auiSpiRxBuffer" << endl;
//        unsigned char *pucSourceTemp;
//        pucSourceTemp = (unsigned char*)auiSpiRxBuffer;
//        for(int i=0; i<32; )
//        {
//            for(int j=0; j<8; j++)
//            {
//                cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
//            }
//            cout << endl;
//            i += 8;
//        }
//}


    // ������ �� ����������?
    if (auiSpiRxBuffer[(SPI_MT_RX_OFFSET +
                        SPI_MT_PRIAMBLE_LENGTH +
                        auiSpiRxBuffer[SPI_MT_RX_OFFSET + SPI_MT_LENGTH_OFFSET])] ==
            HammingDistanceCrc(&auiSpiRxBuffer[SPI_MT_RX_OFFSET],
                               (SPI_MT_PRIAMBLE_LENGTH +
                                auiSpiRxBuffer[SPI_MT_RX_OFFSET + SPI_MT_LENGTH_OFFSET])))
    {
//        cout << "HammingDistanceCrc ok" << endl;
        return 0;
    }
    else
    {
//        cout << "HammingDistanceCrc error" << endl;
        return -1;
    }
}

//-----------------------------------------------------------------------------------------------------
int iModuleMrXXTransferDataReceive(TModuleContext *pxModuleContext)
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

    // ������ ��������.
    pxModuleContext ->
    xModuleContextDinamic.
    usAuxiliaryCounter = BAD_MODULE_CYCLE_COUNT_DEFAULT; //MRXX database error Debug

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

    // ������� ����� ��������������� ��������� ������� ����.
    memset(pxModuleContext ->
           xModuleContextStatic.
           pucBadDoBufferPointer,
           0,
           MRXX_BAD_DO_BIT_ARRAY_LENGTH);
    //iBadModuleBuffUpdate();
    // ������� ��������� �� ����� - ��������� ������� ������� ���� �� �1 � MP �8.
    pucDestination = pxModuleContext ->
                     xModuleContextStatic.
                     pucSostDoBufferPointer;
    ucData = (auiSpiRxBuffer[(SPI_MT_RX_OFFSET + MRXX_RELAY_STATE_OFFSET)]);
    // �������� ������ - ��������� ������� ������� ���� �� �1 � MP �8.
    for (int i = 0; i < MRXX_DISCRETE_OUTPUT_QUANTITY; i++)
    {
        if (ucData & (1 << i))
        {
            *pucDestination++ = 1;
            // ������������ �������?
            if (pxMrXXOneChannelDataBase[i].ucControl & ARCHIVE_MRXX_MASK)
            {
                // �������� ��������� ������� ��� �� ����������������?
                if(xCInternalModuleErrorEvent.EventOnIsNotRegistered(
                            pxModuleContext ->
                            xModuleContextStatic.
                            ucModuleContextIndex,
                            MTVI5_OUT_DAC_ERROR_OFFSET + i,
                            MRXX_ERROR_CODE_RELAY_ON_OFF))
                {
                    // �������������� �������� ��������� �������.
                    // ��������� ���� - i.
                    CEvents::EventRegistration(
                        pxModuleContext ->
                        xModuleContextStatic.
                        ucModuleContextIndex,
                        (CEvents::HANDLED_EVENTS_MODULES_EVENTS_TYPE |
                         CEvents::HANDLED_EVENTS_IS_ARCHIVE),
                        MTVI5_OUT_DAC_ERROR_OFFSET + i,
                        "���. ����");
                }
            }
        }
        else
        {
            *pucDestination++ = 0;
            // ������������ �������?
            if (pxMrXXOneChannelDataBase[i].ucControl & ARCHIVE_MRXX_MASK)
            {
                // ���������� ��������� ������� ��� �� ����������������?
                if(xCInternalModuleErrorEvent.EventOffIsNotRegistered(
                            pxModuleContext ->
                            xModuleContextStatic.
                            ucModuleContextIndex,
                            MTVI5_OUT_DAC_ERROR_OFFSET + i,
                            MRXX_ERROR_CODE_RELAY_ON_OFF))
                {
                    // �������������� ���������� ��������� �������.
                    // ���������� ���� - i.
                    CEvents::EventRegistration(
                        pxModuleContext ->
                        xModuleContextStatic.
                        ucModuleContextIndex,
                        (CEvents::HANDLED_EVENTS_MODULES_EVENTS_TYPE |
                         CEvents::HANDLED_EVENTS_IS_ARCHIVE),
                        MTVI5_OUT_DAC_ERROR_OFFSET + i,
                        "����. ����");
                }
            }
        }
    }

    // ��������� ���� ������� ����� �������������� ����������� ��������:
    // ������������� ��������� ������� ���� - MR51;
    // ����� ������� ���� � �������� ��������� ������� ���� - MR53.
    switch(pxModuleContext ->
            xModuleContextStatic.
            ucModuleType)
    {
    case MODULE_TYPE_MR51:
        // ������ ��� ��-51:
        // ���� ����. ��� �� �� rel_OS: D0 - ���. ����� ���� �1 ���., D7 - ���. ����� ���� �8 ���.
        // ������ ��������� � ������ ���. ��� r_out, ����� ������, ������������� ���-����������� ����
        for (int i = 0; i < MRXX_DISCRETE_OUTPUT_QUANTITY; i++)
        {
            // ���� ��������� ������� �� �������� ����� � ������� ��������� ���� ���������?
            if (((auiSpiRxBuffer[(SPI_MT_RX_OFFSET + MRXX_RELAY_STATE_OFFSET)]) >> i) ==
                    ((auiSpiRxBuffer[(SPI_MT_RX_OFFSET + MRXX_RELAY_STATE_OFFSET)]) >> i))
            {
                // ���� ��������� ������� �� �������� ����� � ������� ��������� ���� ���������.
                // ������� ���� ��������������� ������.
                pxModuleContext ->
                xModuleContextStatic.
                pucBadDoBufferPointer[i] = 0;
            }
            else
            {
                // ���� ��������� ������� �� �������� ����� � ������� ��������� ���� �� ���������.
                // ��������� ���� ��������������� ������.
                pxModuleContext ->
                xModuleContextStatic.
                pucBadDoBufferPointer[i] = 1;
            }
        }
//                return 0;
        break;
    case MODULE_TYPE_MR53:
        // ����� ��������� ������� 8-�� ���� ������. ������ ��53.
        // ���� - 2: ��������� ������� ����, ���� ��� = 1 - �������� ���������.
        // #define MRXX_STAT_DO_SHORT_CIRCUIT_BIT_OFFSET 0
        // ���� - 3: ��������� ������� ����, ���� ��� = 1 - �����.
        // #define MRXX_STAT_DO_BREACK_BIT_OFFSET 1
        for (int i = 0; i < MRXX_DISCRETE_OUTPUT_QUANTITY; i++)
        {
            // ���� ����� ��� �������� ��������� ������� ����?
            if (((((auiSpiRxBuffer[(SPI_MT_RX_OFFSET + MRXX_STAT_DO_SHORT_CIRCUIT_BIT_OFFSET)]) >> i) & 0x01) != 0) ||
                    (((auiSpiRxBuffer[(SPI_MT_RX_OFFSET + MRXX_STAT_DO_BREACK_BIT_OFFSET)]) >> i) & 0x01) != 0)
            {
                // ���� ����� ��� �������� ��������� ������� ����.
                // ��������� ���� ��������������� ������.
                pxModuleContext ->
                xModuleContextStatic.
                pucBadDoBufferPointer[i] = 1;
            }
            else
            {
                // ��� ������ ��� ��������� ��������� ������� ����.
                // ������� ���� ��������������� ������.
                pxModuleContext ->
                xModuleContextStatic.
                pucBadDoBufferPointer[i] = 0;
            }
        }

        for (int i = 0; i < MRXX_DISCRETE_OUTPUT_QUANTITY; i++)
        {
            // ���� �������� ��������� ������� ����?
            if (((auiSpiRxBuffer[(SPI_MT_RX_OFFSET + MRXX_STAT_DO_SHORT_CIRCUIT_BIT_OFFSET)]) >> i) & 0x01)
            {
                // ���� �������� ��������� ������� ����.
                // ��������� ���� ��������� ��������� ������� ����.
                (pxModuleContext ->
                 xModuleContextStatic.
                 pucStatDoBufferPointer[(i * MRXX_STAT_DO_ONE_OUTPUT_LENGTH) +
                                        MRXX_STAT_DO_SHORT_CIRCUIT_BIT_OFFSET]) = 1;

                // �������� ��������� ������� ��� �� ����������������?
                if(xCInternalModuleErrorEvent.EventOnIsNotRegistered(
                            pxModuleContext ->
                            xModuleContextStatic.
                            ucModuleContextIndex,
                            MTVI5_OUT_DAC_ERROR_OFFSET + i,
                            MRXX_ERROR_CODE_RELAY_SHORT_CIRCUIT))
                {
                    // �������������� �������� ��������� �������.
                    // ��������� ���� - i.
                    CEvents::EventRegistration(
                        pxModuleContext ->
                        xModuleContextStatic.
                        ucModuleContextIndex,
                        (CEvents::HANDLED_EVENTS_INTERNAL_MODULES_BAD_TYPE |
                         CEvents::HANDLED_EVENTS_IS_ARCHIVE),
                        MTVI5_OUT_DAC_ERROR_OFFSET + i,
                        "��. �������");
                }
            }
            else
            {
                // ��� ��������� ��������� ������� ����.
                // ������� ���� ��������� ��������� ������� ����.
                (pxModuleContext ->
                 xModuleContextStatic.
                 pucStatDoBufferPointer[(i * MRXX_STAT_DO_ONE_OUTPUT_LENGTH) +
                                        MRXX_STAT_DO_SHORT_CIRCUIT_BIT_OFFSET]) = 0;

                // ���������� ��������� ������� ��� �� ����������������?
                if(xCInternalModuleErrorEvent.EventOffIsNotRegistered(
                            pxModuleContext ->
                            xModuleContextStatic.
                            ucModuleContextIndex,
                            MTVI5_OUT_DAC_ERROR_OFFSET + i,
                            MRXX_ERROR_CODE_RELAY_SHORT_CIRCUIT))
                {
                    // �������������� ���������� ��������� �������.
                    // ���������� ���� - i.
                    CEvents::EventRegistration(
                        pxModuleContext ->
                        xModuleContextStatic.
                        ucModuleContextIndex,
                        (CEvents::HANDLED_EVENTS_INTERNAL_MODULES_BAD_TYPE |
                         CEvents::HANDLED_EVENTS_IS_ARCHIVE),
                        MTVI5_OUT_DAC_ERROR_OFFSET + i,
                        "����. ��. ��");
                }
            }

            // ���� ����� ������� ����?
            if (((auiSpiRxBuffer[(SPI_MT_RX_OFFSET + MRXX_STAT_DO_BREACK_BIT_OFFSET)]) >> i) & 0x01)
            {
                // ���� ����� ������� ����.
                // ��������� ���� ������ ������� ����.
                (pxModuleContext ->
                 xModuleContextStatic.
                 pucStatDoBufferPointer[(i * MRXX_STAT_DO_ONE_OUTPUT_LENGTH) +
                                        MRXX_STAT_DO_BREACK_BIT_OFFSET]) = 1;

                // �������� ��������� ������� ��� �� ����������������?
                if(xCInternalModuleErrorEvent.EventOnIsNotRegistered(
                            pxModuleContext ->
                            xModuleContextStatic.
                            ucModuleContextIndex,
                            MTVI5_OUT_DAC_ERROR_OFFSET + i,
                            MRXX_ERROR_CODE_RELAY_BREACK))
                {
                    // �������������� �������� ��������� �������.
                    // ��������� ���� - i.
                    CEvents::EventRegistration(
                        pxModuleContext ->
                        xModuleContextStatic.
                        ucModuleContextIndex,
                        (CEvents::HANDLED_EVENTS_INTERNAL_MODULES_BAD_TYPE |
                         CEvents::HANDLED_EVENTS_IS_ARCHIVE),
                        MTVI5_OUT_DAC_ERROR_OFFSET + i,
                        "���. �������");
                }
            }
            else
            {
                // ��� ������ ������� ����.
                // ������� ���� ������ ������� ����.
                (pxModuleContext ->
                 xModuleContextStatic.
                 pucStatDoBufferPointer[(i * MRXX_STAT_DO_ONE_OUTPUT_LENGTH) +
                                        MRXX_STAT_DO_BREACK_BIT_OFFSET]) = 0;

                // ���������� ��������� ������� ��� �� ����������������?
                if(xCInternalModuleErrorEvent.EventOffIsNotRegistered(
                            pxModuleContext ->
                            xModuleContextStatic.
                            ucModuleContextIndex,
                            MTVI5_OUT_DAC_ERROR_OFFSET + i,
                            MRXX_ERROR_CODE_RELAY_BREACK))
                {
                    // �������������� ���������� ��������� �������.
                    // ���������� ���� - i.
                    CEvents::EventRegistration(
                        pxModuleContext ->
                        xModuleContextStatic.
                        ucModuleContextIndex,
                        (CEvents::HANDLED_EVENTS_INTERNAL_MODULES_BAD_TYPE |
                         CEvents::HANDLED_EVENTS_IS_ARCHIVE),
                        MTVI5_OUT_DAC_ERROR_OFFSET + i,
                        "���� ���. ��");
                }
            }
        }

        return 0;
        break;

    default:
        break;
    };

    return 0;
}

//-----------------------------------------------------------------------------------------------------
int iModuleMrXXTransferDataReceiveError(TModuleContext *pxModuleContext)
{
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

        // ��������� ����� ��������������� ��������� ������� ����.
        memset(pxModuleContext ->
               xModuleContextStatic.
               pucBadDoBufferPointer,
               1,
               MRXX_BAD_DO_BIT_ARRAY_LENGTH);
        //iBadModuleBuffUpdate();
        return -1;
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


////-----------------------------------------------------------------------------------------------------
//// ���������� ����� ������� � ������� ����������� ������ - MR.
//int iModuleMrXXTransferData(TModuleContext *pxModuleContext)
//{
//    unsigned char ucData;
//    unsigned short usData;
//    unsigned char ucLocalKvit;
//    unsigned char ucLocalReset;
//    // ucLocalBou ���� ���������� ������� ����.
//    // ���� ���-� f_bou: ����� ���������� ��:  D0-"����.�� ��", D1-"����� �� ��"
//    // D2-������"����.�", D3-������"����� �", D4-���� "����.���/����".
//    // ���� ���������� �� ����� f_bou.
//    // ������������ � �������� ������.
//    // #define GLOBAL_KVIT_PC_BIT 0
//    // ����� � �������� ������.
//    // #define GLOBAL_RESET_PC_BIT 1
//    // ������������ � ���������� ������ ����.
//    // #define GLOBAL_KVIT_BUTTON_BIT 2
//    // ����� � ���������� ������ ����.
//    // #define GLOBAL_RESET_BUTTON_BIT 3
//    // ����� ����������.
//    // #define GLOBAL_BLOCK_BIT 4
//    unsigned char ucLocalBou;
//    int i;
//    unsigned char *pucSource;
//    unsigned char *pucDestination;
//    TMrXXOneChannelDataBase *pxMrXXOneChannelDataBase;
//
//    // ������� ��������� �� ���� � ���� ������.
//    pxMrXXOneChannelDataBase =
//        (TMrXXOneChannelDataBase*)((pxModuleContext ->
//                                    xModuleContextStatic.
//                                    pucDataBasePointer) +
//                                   (pxModuleContext ->
//                                    xModuleContextStatic.
//                                    ucDataBaseDataOffset));
//
//    ucLocalKvit = 0;
//    // ���� ��������� ������� �������� ������������.
//    for (i = 0; i < MRXX_DISCRETE_OUTPUT_QUANTITY; i++)
//    {
//        // ��� �������� ����� ������� ������������ �����������������?
//        if (pxMrXXOneChannelDataBase[i].ucExternalKvitAddress)
//        {
//            // ��� �������� ����� ���� ������� ������������?
//            if (*((pxModuleContext ->
//                    xModuleContextStatic.
//                    pucDiValueOriginBufferPointer) +
//                    (pxMrXXOneChannelDataBase[i].
//                     ucExternalKvitAddress) - 1))
//            {
//                // ��������� ������������ ��� �������� �����.
//                ucLocalKvit |= (1 << i);
//            }
//        }
//    }
//
//    ucLocalReset = 0;
//    // ���� ��������� ������� �������� ������.
//    for (i = 0; i < MRXX_DISCRETE_OUTPUT_QUANTITY; i++)
//    {
//        // ��� �������� ����� ������� ����� ����������������?
//        if (pxMrXXOneChannelDataBase[i].ucExternalResetAddress)
//        {
//            // ��� �������� ����� ���� ������� �����?
//            if (*((pxModuleContext ->
//                    xModuleContextStatic.
//                    pucDiValueOriginBufferPointer) +
//                    (pxMrXXOneChannelDataBase[i].
//                     ucExternalResetAddress) - 1))
//            {
//                // ��������� ����� ��� �������� �����.
//                ucLocalReset |= (1 << i);
//            }
//        }
//    }
//
//    ucLocalBou = 0;
//    // ���� ������������ Modbus?
//    if (pxModuleContext ->
//            xModuleContextDinamic.
//            fbGlobalModbusKvit)
//    {
//        cout << "fbGlobalModbusKvit" << endl;
//        // ������� ���� ����������� �������.
//        pxModuleContext ->
//        xModuleContextDinamic.
//        fbGlobalModbusKvit = 0;
//        // ��������� ���� ���������� - ������������ Modbus.
//        ucLocalBou |= (1 << GLOBAL_KVIT_PC_BIT);
//    }
//    // ���� ����� Modbus?
//    if (pxModuleContext ->
//            xModuleContextDinamic.
//            fbGlobalModbusReset)
//    {
//        cout << "fbGlobalModbusReset" << endl;
//        // ������� ���� ����������� �������.
//        pxModuleContext ->
//        xModuleContextDinamic.
//        fbGlobalModbusReset = 0;
//        // ��������� ���� ���������� - ����� Modbus.
//        ucLocalBou |= (1 << GLOBAL_RESET_PC_BIT);
//    }
//    // ���� ������������ � ������ ����������� ������?
//    if (pxModuleContext ->
//            xModuleContextDinamic.
//            fbGlobalButtonKvit)
//    {
//        // ������� ���� ����������� �������.
//        pxModuleContext ->
//        xModuleContextDinamic.
//        fbGlobalButtonKvit = 0;
//        // ��������� ���� ���������� - ������������ � ����������� ������.
//        ucLocalBou |= (1 << GLOBAL_KVIT_BUTTON_BIT);
//    }
//    // ���� ����� � ������ ����������� ������?
//    if (pxModuleContext ->
//            xModuleContextDinamic.
//            fbGlobalButtonReset)
//    {
//        // ������� ���� ����������� �������.
//        pxModuleContext ->
//        xModuleContextDinamic.
//        fbGlobalButtonReset = 0;
//        // ��������� ���� ���������� - ����� � ����������� ������.
//        ucLocalBou |= (1 << GLOBAL_RESET_BUTTON_BIT);
//    }
//    // ������� ����� ����������?
////    if (pxModuleContext ->
////            xModuleContextDinamic.
////            fbGlobalModbusBlock)
//    if (fbGlobalBlock)
//    {
////        // ������� ���� ����������� �������.
////        pxModuleContext ->
////        xModuleContextDinamic.
////        fbGlobalModbusBlock = 0;
//        // ��������� ���� ���������� - ���������� Modbus.
//        ucLocalBou |= (1 << GLOBAL_BLOCK_BIT);
//    }
//    // ��� ����������� ��� ������?
//    if ((!fbModuleMrOutOff) || (!fbPlcErrorReset))
//    {
//        pucDestination = &auiSpiTxBuffer[SPI_DATA_BYTE_OFFSET];
//        pucSource = (pxModuleContext ->
//                     xModuleContextStatic.
//                     pucDoValueBlockBufferPointer);
//        ucData = 0;
//        // �������� ���������� ��������� ��� ������� ����.
//        for (i = 0; i < MRXX_DISCRETE_OUTPUT_QUANTITY; i++)
//        {
//            if (pucSource[i])
//            {
//                ucData |= (1 << i);
//            }
//            else
//            {
//
//            }
//        }
//        *pucDestination++ = ucData;
////        cout << "iModuleMrXXTransferData ucData 1 " << std::showbase << std::dec << (int)ucData << std::endl;
//
//        pucSource = (pxModuleContext ->
//                     xModuleContextStatic.
//                     pucDoValueBufferPointer);
//        ucData = 0;
//        // �������� ����� ��������� ��� ������� ����.
//        for (i = 0; i < MRXX_DISCRETE_OUTPUT_QUANTITY; i++)
//        {
//            if (pucSource[i])
//            {
//                ucData |= (1 << i);
//            }
//            else
//            {
//
//            }
//        }
//        *pucDestination++ = ucData;
////        cout << "iModuleMrXXTransferData ucData 2 " << std::showbase << std::dec << (int)ucData << std::endl;
//    }
//    else
//    {
//        // ���� ����������� ��� ������.
//        *pucDestination++ = 0;
//        *pucDestination++ = 0;
//    }
//
//
//    *pucDestination++ = ucLocalKvit;
//    *pucDestination++ = ucLocalReset;
//    *pucDestination++ = ucLocalBou;
//    usData = usCrcSummTwoByteCalculation(&auiSpiTxBuffer[SPI_DATA_BYTE_OFFSET],
//                                         MRXX_GET_DISCRETE_OUTPUT_DATA_COMMAND_ANSWER_LENGTH);
//    *pucDestination++ = (unsigned char)usData;
////    *pucDestination++ = (unsigned char)(usData >> 8);
////    *pucDestination++ = DATA_EXCHANGE_OK;
//
//    auiSpiTxBuffer[0] = MRXX_GET_DISCRETE_OUTPUT_DATA_COMMAND;
//    auiSpiTxBuffer[1] = 7;
////
////if (auiSpiTxBuffer[SPI_DATA_BYTE_OFFSET])
////{
////        cout << "SPI_DATA_BYTE_OFFSET" << endl;
////        cout << "auiSpiTxBuffer" << endl;
////        unsigned char *pucSourceTemp;
////        pucSourceTemp = (unsigned char*)auiSpiTxBuffer;
////        for(int i=0; i<32; )
////        {
////            for(int j=0; j<8; j++)
////            {
////                cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
////            }
////            cout << endl;
////            i += 8;
////        }
////}
////
////if (auiSpiTxBuffer[SPI_DATA_BYTE_OFFSET + 1])
////{
////        cout << "SPI_DATA_BYTE_OFFSET + 1" << endl;
////        cout << "auiSpiTxBuffer" << endl;
////        unsigned char *pucSourceTemp;
////        pucSourceTemp = (unsigned char*)auiSpiTxBuffer;
////        for(int i=0; i<32; )
////        {
////            for(int j=0; j<8; j++)
////            {
////                cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
////            }
////            cout << endl;
////            i += 8;
////        }
////}
//
//
//    xSpiCommunicationDevice.Exchange(pxModuleContext ->
//xModuleContextStatic.
//ucModuleAddress,
//                 auiSpiTxBuffer,
//                 auiSpiRxBuffer,
//                 SPI_PREAMBLE_LENGTH +
//                 MRXX_GET_DISCRETE_OUTPUT_DATA_COMMAND_ANSWER_LENGTH +
//                 ONE_BYTE_CRC_LENGTH,
//                 SPEED_IN_HZ
//                );
//
//
////if (auiSpiTxBuffer[SPI_DATA_BYTE_OFFSET])
////{
////        cout << "SPI_DATA_BYTE_OFFSET" << endl;
////        cout << "auiSpiRxBuffer" << endl;
////        unsigned char *pucSourceTemp;
////        pucSourceTemp = (unsigned char*)auiSpiRxBuffer;
////        for(int i=0; i<32; )
////        {
////            for(int j=0; j<8; j++)
////            {
////                cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
////            }
////            cout << endl;
////            i += 8;
////        }
////}
////
////if (auiSpiTxBuffer[SPI_DATA_BYTE_OFFSET + 1])
////{
////        cout << "SPI_DATA_BYTE_OFFSET + 1" << endl;
////        cout << "auiSpiRxBuffer" << endl;
////        unsigned char *pucSourceTemp;
////        pucSourceTemp = (unsigned char*)auiSpiRxBuffer;
////        for(int i=0; i<32; )
////        {
////            for(int j=0; j<8; j++)
////            {
////                cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
////            }
////            cout << endl;
////            i += 8;
////        }
////}
//
//
//    // ��� ������� ������?
//    switch(auiSpiRxBuffer[SPI_COMMAND_BYTE_OFFSET])
//    {
//    case MRXX_DATA_READY:
//        // ������ �� ����������?
//        if (iCrcSummTwoByteCompare(&auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET],
//                                   MRXX_GET_DISCRETE_OUTPUT_DATA_COMMAND_ANSWER_LENGTH) > 0)
//        {
//            // ������ ��������.
//            pxModuleContext ->
//            xModuleContextDinamic.
//            usAuxiliaryCounter = BAD_MODULE_CYCLE_COUNT_DEFAULT; //MRXX database error Debug
//
//            // ������ ��������.
//            pxModuleContext ->
//            xModuleContextDinamic.
//            nucBadAnswerCounter = BAD_MODULE_CYCLE_COUNT_DEFAULT;
//            // ������� ���� ������ ������.
//            *(pxModuleContext ->
//              xModuleContextStatic.
//              pucModuleBadStateBufferPointer) = BAD_MODULE_RESPONDED_OK;
//
//            // ���������� ��������� ������� ��� �� ����������������?
//            if(xCInternalModuleErrorEvent.EventOffIsNotRegistered(
//                        pxModuleContext ->
//                        xModuleContextStatic.
//                        ucModuleContextIndex,
//                        MTVI5_BAD_ANSWER_ERROR_OFFSET))
//            {
//                // �������������� ���������� ��������� �������.
//                // ����� ������ �������.
//                CEvents::EventRegistration(
//                    pxModuleContext ->
//                    xModuleContextStatic.
//                    ucModuleContextIndex,
//                    (CEvents::HANDLED_EVENTS_INTERNAL_MODULES_BAD_TYPE |
//                     CEvents::HANDLED_EVENTS_IS_ARCHIVE),
//                    MTVI5_BAD_ANSWER_ERROR_OFFSET,
//                    "����. ������");
//            }
//
//            // ������� ����� ��������������� ��������� ������� ����.
//            memset(pxModuleContext ->
//                   xModuleContextStatic.
//                   pucBadDoBufferPointer,
//                   0,
//                   MRXX_BAD_DO_BIT_ARRAY_LENGTH);
//            //iBadModuleBuffUpdate();
//            // ������� ��������� �� ����� - ��������� ������� ������� ���� �� �1 � MP �8.
//            pucDestination = pxModuleContext ->
//                             xModuleContextStatic.
//                             pucSostDoBufferPointer;
//            ucData = (auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET]);
//            // �������� ������ - ��������� ������� ������� ���� �� �1 � MP �8.
//            for (i = 0; i < MRXX_DISCRETE_OUTPUT_QUANTITY; i++)
//            {
//                if (ucData & (1 << i))
//                {
//                    *pucDestination++ = 1;
//                    // ������������ �������?
//                    if (pxMrXXOneChannelDataBase[i].ucControl & ARCHIVE_MRXX_MASK)
//                    {
//                        // �������� ��������� ������� ��� �� ����������������?
//                        if(xCInternalModuleErrorEvent.EventOnIsNotRegistered(
//                                    pxModuleContext ->
//                                    xModuleContextStatic.
//                                    ucModuleContextIndex,
//                                    MTVI5_OUT_DAC_ERROR_OFFSET + i,
//                                    MRXX_ERROR_CODE_RELAY_ON_OFF))
//                        {
//                            // �������������� �������� ��������� �������.
//                            // ��������� ���� - i.
//                            CEvents::EventRegistration(
//                                pxModuleContext ->
//                                xModuleContextStatic.
//                                ucModuleContextIndex,
//                                (CEvents::HANDLED_EVENTS_MODULES_EVENTS_TYPE |
//                                 CEvents::HANDLED_EVENTS_IS_ARCHIVE),
//                                MTVI5_OUT_DAC_ERROR_OFFSET + i,
//                                "���. ����");
//                        }
//                    }
//                }
//                else
//                {
//                    *pucDestination++ = 0;
//                    // ������������ �������?
//                    if (pxMrXXOneChannelDataBase[i].ucControl & ARCHIVE_MRXX_MASK)
//                    {
//                        // ���������� ��������� ������� ��� �� ����������������?
//                        if(xCInternalModuleErrorEvent.EventOffIsNotRegistered(
//                                    pxModuleContext ->
//                                    xModuleContextStatic.
//                                    ucModuleContextIndex,
//                                    MTVI5_OUT_DAC_ERROR_OFFSET + i,
//                                    MRXX_ERROR_CODE_RELAY_ON_OFF))
//                        {
//                            // �������������� ���������� ��������� �������.
//                            // ���������� ���� - i.
//                            CEvents::EventRegistration(
//                                pxModuleContext ->
//                                xModuleContextStatic.
//                                ucModuleContextIndex,
//                                (CEvents::HANDLED_EVENTS_MODULES_EVENTS_TYPE |
//                                 CEvents::HANDLED_EVENTS_IS_ARCHIVE),
//                                MTVI5_OUT_DAC_ERROR_OFFSET + i,
//                                "����. ����");
//                        }
//                    }
//                }
//            }
//            // ��������� ���� ������� ����� �������������� ����������� ��������:
//            // ������������� ��������� ������� ���� - MR51;
//            // ����� ������� ���� � �������� ��������� ������� ���� - MR53.
//            switch(pxModuleContext ->
//                    xModuleContextStatic.
//                    ucModuleType)
//            {
//            case MODULE_TYPE_MR51:
//                // ������ ��� ��-51:
//                // ���� ����. ��� �� �� rel_OS: D0 - ���. ����� ���� �1 ���., D7 - ���. ����� ���� �8 ���.
//                // ������ ��������� � ������ ���. ��� r_out, ����� ������, ������������� ���-����������� ����
//                for (i = 0; i < MRXX_DISCRETE_OUTPUT_QUANTITY; i++)
//                {
//                    // ���� ��������� ������� �� �������� ����� � ������� ��������� ���� ���������?
//                    if (((auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET]) >> i) ==
//                            ((auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET + 1]) >> i))
//                    {
//                        // ���� ��������� ������� �� �������� ����� � ������� ��������� ���� ���������.
//                        // ������� ���� ��������������� ������.
//                        pxModuleContext ->
//                        xModuleContextStatic.
//                        pucBadDoBufferPointer[i] = 0;
//                    }
//                    else
//                    {
//                        // ���� ��������� ������� �� �������� ����� � ������� ��������� ���� �� ���������.
//                        // ��������� ���� ��������������� ������.
//                        pxModuleContext ->
//                        xModuleContextStatic.
//                        pucBadDoBufferPointer[i] = 1;
//                    }
//
//                    if (i == 3)
//                    {
//                        // ���� ��������� ������� �� �������� ����� � ������� ��������� ���� �� ���������.
//                        // ��������� ���� ��������������� ������.
//                        pxModuleContext ->
//                        xModuleContextStatic.
//                        pucBadDoBufferPointer[i] = 1;
//                    }
//                }
////                return 0;
//                break;
//            case MODULE_TYPE_MR53:
//                // ����� ��������� ������� 8-�� ���� ������. ������ ��53.
//                // ���� - 2: ��������� ������� ����, ���� ��� = 1 - �������� ���������.
//                // #define MRXX_STAT_DO_SHORT_CIRCUIT_BIT_OFFSET 0
//                // ���� - 3: ��������� ������� ����, ���� ��� = 1 - �����.
//                // #define MRXX_STAT_DO_BREACK_BIT_OFFSET 1
////                (auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET + 1]) = 4;
////                (auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET + 2]) = 4;
//                for (i = 0; i < MRXX_DISCRETE_OUTPUT_QUANTITY; i++)
//                {
//                    // ���� ����� ��� �������� ��������� ������� ����?
//                    if (((((auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET + 1]) >> i) & 0x01) != 0) ||
//                            (((auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET + 2]) >> i) & 0x01) != 0)
//                    {
//                        // ���� ����� ��� �������� ��������� ������� ����.
//                        // ��������� ���� ��������������� ������.
//                        pxModuleContext ->
//                        xModuleContextStatic.
//                        pucBadDoBufferPointer[i] = 1;
//                    }
//                    else
//                    {
//                        // ��� ������ ��� ��������� ��������� ������� ����.
//                        // ������� ���� ��������������� ������.
//                        pxModuleContext ->
//                        xModuleContextStatic.
//                        pucBadDoBufferPointer[i] = 0;
//                    }
//
////                    if (i == 3)
////                    {
////                        // ���� ��������� ������� �� �������� ����� � ������� ��������� ���� �� ���������.
////                        // ��������� ���� ��������������� ������.
////                        pxModuleContext ->
////                        xModuleContextStatic.
////                        pucBadDoBufferPointer[i] = 1;
////                    }
//                }
//
//                for (i = 0; i < MRXX_DISCRETE_OUTPUT_QUANTITY; i++)
//                {
//                    // ���� �������� ��������� ������� ����?
//                    if (((auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET + 1]) >> i) & 0x01)
//                    {
//                        // ���� �������� ��������� ������� ����.
//                        // ��������� ���� ��������� ��������� ������� ����.
//                        (pxModuleContext ->
//                         xModuleContextStatic.
//                         pucStatDoBufferPointer[(i * MRXX_STAT_DO_ONE_OUTPUT_LENGTH) +
//                                                MRXX_STAT_DO_SHORT_CIRCUIT_BIT_OFFSET]) = 1;
//
//                        // �������� ��������� ������� ��� �� ����������������?
//                        if(xCInternalModuleErrorEvent.EventOnIsNotRegistered(
//                                    pxModuleContext ->
//                                    xModuleContextStatic.
//                                    ucModuleContextIndex,
//                                    MTVI5_OUT_DAC_ERROR_OFFSET + i,
//                                    MRXX_ERROR_CODE_RELAY_SHORT_CIRCUIT))
//                        {
//                            // �������������� �������� ��������� �������.
//                            // ��������� ���� - i.
//                            CEvents::EventRegistration(
//                                pxModuleContext ->
//                                xModuleContextStatic.
//                                ucModuleContextIndex,
//                                (CEvents::HANDLED_EVENTS_INTERNAL_MODULES_BAD_TYPE |
//                                 CEvents::HANDLED_EVENTS_IS_ARCHIVE),
//                                MTVI5_OUT_DAC_ERROR_OFFSET + i,
//                                "��. �������");
//                        }
//                    }
//                    else
//                    {
//                        // ��� ��������� ��������� ������� ����.
//                        // ������� ���� ��������� ��������� ������� ����.
//                        (pxModuleContext ->
//                         xModuleContextStatic.
//                         pucStatDoBufferPointer[(i * MRXX_STAT_DO_ONE_OUTPUT_LENGTH) +
//                                                MRXX_STAT_DO_SHORT_CIRCUIT_BIT_OFFSET]) = 0;
//
//                        // ���������� ��������� ������� ��� �� ����������������?
//                        if(xCInternalModuleErrorEvent.EventOffIsNotRegistered(
//                                    pxModuleContext ->
//                                    xModuleContextStatic.
//                                    ucModuleContextIndex,
//                                    MTVI5_OUT_DAC_ERROR_OFFSET + i,
//                                    MRXX_ERROR_CODE_RELAY_SHORT_CIRCUIT))
//                        {
//                            // �������������� ���������� ��������� �������.
//                            // ���������� ���� - i.
//                            CEvents::EventRegistration(
//                                pxModuleContext ->
//                                xModuleContextStatic.
//                                ucModuleContextIndex,
//                                (CEvents::HANDLED_EVENTS_INTERNAL_MODULES_BAD_TYPE |
//                                 CEvents::HANDLED_EVENTS_IS_ARCHIVE),
//                                MTVI5_OUT_DAC_ERROR_OFFSET + i,
//                                "����. ��. ��");
//                        }
//                    }
//
//
////                    if (i == 3)
////                    {
////                        // ���� �������� ��������� ������� ����.
////                        // ��������� ���� ��������� ��������� ������� ����.
////                        (pxModuleContext ->
////                         xModuleContextStatic.
////                         pucStatDoBufferPointer[(i * MRXX_STAT_DO_ONE_OUTPUT_LENGTH) +
////                                                MRXX_STAT_DO_SHORT_CIRCUIT_BIT_OFFSET]) = 1;
////                    }
//
//                    // ���� ����� ������� ����?
////                    if ((ui8TempCommand) && (ui8TempCommand - 1) == i)
//                    if (((auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET + 2]) >> i) & 0x01)
//                    {
//                        // ���� ����� ������� ����.
//                        // ��������� ���� ������ ������� ����.
//                        (pxModuleContext ->
//                         xModuleContextStatic.
//                         pucStatDoBufferPointer[(i * MRXX_STAT_DO_ONE_OUTPUT_LENGTH) +
//                                                MRXX_STAT_DO_BREACK_BIT_OFFSET]) = 1;
//
//                        // �������� ��������� ������� ��� �� ����������������?
//                        if(xCInternalModuleErrorEvent.EventOnIsNotRegistered(
//                                    pxModuleContext ->
//                                    xModuleContextStatic.
//                                    ucModuleContextIndex,
//                                    MTVI5_OUT_DAC_ERROR_OFFSET + i,
//                                    MRXX_ERROR_CODE_RELAY_BREACK))
//                        {
//                            // �������������� �������� ��������� �������.
//                            // ��������� ���� - i.
//                            CEvents::EventRegistration(
//                                pxModuleContext ->
//                                xModuleContextStatic.
//                                ucModuleContextIndex,
//                                (CEvents::HANDLED_EVENTS_INTERNAL_MODULES_BAD_TYPE |
//                                 CEvents::HANDLED_EVENTS_IS_ARCHIVE),
//                                MTVI5_OUT_DAC_ERROR_OFFSET + i,
//                                "���. �������");
//                        }
//                    }
//                    else
//                    {
//                        // ��� ������ ������� ����.
//                        // ������� ���� ������ ������� ����.
//                        (pxModuleContext ->
//                         xModuleContextStatic.
//                         pucStatDoBufferPointer[(i * MRXX_STAT_DO_ONE_OUTPUT_LENGTH) +
//                                                MRXX_STAT_DO_BREACK_BIT_OFFSET]) = 0;
//
//                        // ���������� ��������� ������� ��� �� ����������������?
//                        if(xCInternalModuleErrorEvent.EventOffIsNotRegistered(
//                                    pxModuleContext ->
//                                    xModuleContextStatic.
//                                    ucModuleContextIndex,
//                                    MTVI5_OUT_DAC_ERROR_OFFSET + i,
//                                    MRXX_ERROR_CODE_RELAY_BREACK))
//                        {
//                            // �������������� ���������� ��������� �������.
//                            // ���������� ���� - i.
//                            CEvents::EventRegistration(
//                                pxModuleContext ->
//                                xModuleContextStatic.
//                                ucModuleContextIndex,
//                                (CEvents::HANDLED_EVENTS_INTERNAL_MODULES_BAD_TYPE |
//                                 CEvents::HANDLED_EVENTS_IS_ARCHIVE),
//                                MTVI5_OUT_DAC_ERROR_OFFSET + i,
//                                "���� ���. ��");
//                        }
//                    }
//
////                    if (i == 3)
////                    {
////                        // ���� ����� ������� ����.
////                        // ��������� ���� ������ ������� ����.
////                        (pxModuleContext ->
////                         xModuleContextStatic.
////                         pucStatDoBufferPointer[(i * MRXX_STAT_DO_ONE_OUTPUT_LENGTH) +
////                                                MRXX_STAT_DO_BREACK_BIT_OFFSET]) = 1;
////                    }
//
//                }
//
//                return 0;
//                break;
//
//            default:
//                break;
//            };
//            break;
//        }
//
//    case MRXX_DATA_BASE_ERROR:
//        // ���� ������ ����������?
//        if ((pxModuleContext ->
//                xModuleContextDinamic.
//                usAuxiliaryCounter) == 0) //MRXX database error Debug
//        {
//            cout << "MRXX_DATA_BASE_ERROR" << endl;
//            fbDiscreteOutputModuleDataBaseError = 1;
//            // ������� ��� ������;
//            (pxModuleContext ->
//             xModuleContextDinamic.
//             ucErrorCode) = INTERNAL_MODULE_ERROR_DATA_BASE;
//
//            // �������� ��������� ������� ��� �� ����������������?
//            if(xCInternalModuleErrorEvent.EventOnIsNotRegistered(
//                        pxModuleContext ->
//                        xModuleContextStatic.
//                        ucModuleContextIndex,
//                        MTVI5_DATA_BASE_ERROR_OFFSET))
//            {
//                // �������������� �������� ��������� �������.
//                CEvents::EventRegistration(
//                    pxModuleContext ->
//                    xModuleContextStatic.
//                    ucModuleContextIndex,
//                    (CEvents::HANDLED_EVENTS_INTERNAL_MODULES_BAD_TYPE |
//                     CEvents::HANDLED_EVENTS_IS_POPUP |
//                     CEvents::HANDLED_EVENTS_IS_SOUND |
//                     CEvents::HANDLED_EVENTS_IS_ARCHIVE),
//                    MTVI5_DATA_BASE_ERROR_OFFSET,
//                    "������ �. ��");
//            }
//        }
//        else
//        {
//            // �������� �������� �������� - "���������� ������� ����� � ������� ��� ������".
//            (pxModuleContext ->
//             xModuleContextDinamic.
//             usAuxiliaryCounter)--;
//
//            cout << "MRXX_DATA_BASE_ERROR MRXX_FSM_STATE_WRITE_DATABASE" << endl;
//            // ���� ������ ����������. ���������� �������� � ��������.
//            pxModuleContext ->
//            xModuleContextDinamic.
//            ucFsmState = MRXX_FSM_STATE_WRITE_DATABASE;
//        }
//
//        break;
//
//    default:
////        // ������ ��������.
////        pxModuleContext ->
////        xModuleContextDinamic.
////        usAuxiliaryCounter = BAD_MODULE_CYCLE_COUNT_DEFAULT; //MRXX database error Debug
//        break;
//    };
//
//    // ������ ������� �����������?
//    if ((pxModuleContext ->
//            xModuleContextDinamic.
//            nucBadAnswerCounter) == 0)
//    {
//        // ������ ������� �����������.
//        *(pxModuleContext ->
//          xModuleContextStatic.
//          pucModuleBadStateBufferPointer) = BAD_MODULE_NOT_RESPONDED;
//
//        // �������� ��������� ������� ��� �� ����������������?
//        if(xCInternalModuleErrorEvent.EventOnIsNotRegistered(
//                    pxModuleContext ->
//                    xModuleContextStatic.
//                    ucModuleContextIndex,
//                    MTVI5_BAD_ANSWER_ERROR_OFFSET))
//        {
//            // �������������� �������� ��������� �������.
//            // ������ ������ �������.
//            CEvents::EventRegistration(
//                pxModuleContext ->
//                xModuleContextStatic.
//                ucModuleContextIndex,
//                (CEvents::HANDLED_EVENTS_INTERNAL_MODULES_BAD_TYPE |
//                 CEvents::HANDLED_EVENTS_IS_POPUP |
//                 CEvents::HANDLED_EVENTS_IS_SOUND |
//                 CEvents::HANDLED_EVENTS_IS_ARCHIVE),
//                MTVI5_BAD_ANSWER_ERROR_OFFSET,
//                "����. ������");
//        }
//
//        // ��������� ����� ��������������� ��������� ������� ����.
//        memset(pxModuleContext ->
//               xModuleContextStatic.
//               pucBadDoBufferPointer,
//               1,
//               MRXX_BAD_DO_BIT_ARRAY_LENGTH);
//        //iBadModuleBuffUpdate();
//        return -1;
//    }
//    else
//    {
//        // �������� �������� �������� - "���������� ������� ����� � ������� ��� ������".
//        (pxModuleContext ->
//         xModuleContextDinamic.
//         nucBadAnswerCounter)--;
//    }
//    return 0;
//}

////-----------------------------------------------------------------------------------------------------
//// ���������� ���� ������ �� RAM ������� � EEPROM ������.
//int iModuleMrXXDataBaseBlockWrite(TModuleContext *pxModuleContext)
//{
//    unsigned char ucData;
//    unsigned short usData;
//    int i;
//    unsigned char *pucSource;
//    unsigned char *pucDestination;
//    unsigned char *pucTempArray;
//    TMrXXOneChannelDataBase *pxMrXXOneChannelDataBaseSource;
//
//    pucDestination = &auiSpiTxBuffer[SPI_DATA_BYTE_OFFSET];
//    // ������� ��������� �� ���� � ���� ������.
//    pxMrXXOneChannelDataBaseSource =
//        (TMrXXOneChannelDataBase*)((pxModuleContext ->
//                                    xModuleContextStatic.
//                                    pucDataBasePointer) +
//                                   (pxModuleContext ->
//                                    xModuleContextStatic.
//                                    ucDataBaseDataOffset));
//
//    // ��� �������� �� � �� ���������� ������ ���� 0 ��������� ������� ������.
//    for (i = 0; i < MRXX_DISCRETE_OUTPUT_QUANTITY; i++)
//    {
//
//        pucDestination[i] =
//            pxMrXXOneChannelDataBaseSource[i].ucControl;
//    }
//
//    pucSource = &auiSpiTxBuffer[SPI_DATA_BYTE_OFFSET];
//    usData = usCrcSummTwoByteCalculation(pucSource,
//                                         MRXX_DISCRETE_OUTPUT_QUANTITY);
//    pucDestination = &auiSpiTxBuffer[SPI_DATA_BYTE_OFFSET +
//                                     MRXX_DISCRETE_OUTPUT_QUANTITY];
//    *pucDestination++ = (unsigned char)usData;
//    *pucDestination = (unsigned char)(usData >> 8);
//
//    auiSpiTxBuffer[0] = MRXX_SET_DATA_BASE_COMMAND;
//    xSpiCommunicationDevice.Exchange(pxModuleContext ->
//            xModuleContextStatic.
//            ucModuleAddress,
//                 auiSpiTxBuffer,
//                 auiSpiRxBuffer,
//                 SPI_PREAMBLE_LENGTH +
//                 MRXX_SET_DATA_BASE_COMMAND_ANSWER_LENGTH +
//                 TWO_BYTE_CRC_LENGTH +
//                 TAIL_ANSWER_LENGTH,
//                 LOW_SPEED_IN_HZ
//                );
//    // ������ ��������?
//    if (auiSpiRxBuffer[SPI_COMMAND_BYTE_OFFSET] == MRXX_SET_DATA_BASE_COMMAND) // if echo answer is ok.
//    {
//        // ���� ������������� ������ ���� ������ � EEPROM ������?
//        if (auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET +
//                           MRXX_DISCRETE_OUTPUT_QUANTITY +
//                           TWO_BYTE_CRC_LENGTH] == DATA_EXCHANGE_OK)
//        {
//            // ���� ������������� ������ ���� ������ � EEPROM ������.
//            cout << "iModuleMrXXDataBaseBlockWrite ok" << endl;
//            return 0;
//        }
//    }
//    cout << "iModuleMrXXDataBaseBlockWrite error" << endl;
//
//    return -1;
//}

//-----------------------------------------------------------------------------------------------------
// ���������� ���� ������ �� RAM ������� � EEPROM ������.
int iModuleMrXXDataBaseBlockWrite(TModuleContext *pxModuleContext)
{
    unsigned char ucData;
    unsigned short usData;
    int i;
    unsigned char *pucSource;
    unsigned char *pucDestination;
    unsigned char *pucTempArray;
    TMrXXOneChannelDataBase *pxMrXXOneChannelDataBaseSource;
//    unsigned char auiSpiTxBuffer[TX_RX_BUFF_SIZE];
//    unsigned char auiSpiRxBuffer[TX_RX_BUFF_SIZE];

    pucDestination = &auiSpiTxBuffer[SPI_DATA_BYTE_OFFSET];
    // ������� ��������� �� ���� � ���� ������.
    pxMrXXOneChannelDataBaseSource =
        (TMrXXOneChannelDataBase*)((pxModuleContext ->
                                    xModuleContextStatic.
                                    pucDataBasePointer) +
                                   (pxModuleContext ->
                                    xModuleContextStatic.
                                    ucDataBaseDataOffset));


    auiSpiTxBuffer[0] = MRXX_SET_DATA_BASE_COMMAND;
    auiSpiTxBuffer[1] = 8;
    // ��� �������� �� � �� ���������� ������ ���� 0 ��������� ������� ������.
    for (i = 0; i < MRXX_DISCRETE_OUTPUT_QUANTITY; i++)
    {

        pucDestination[i] =
            pxMrXXOneChannelDataBaseSource[i].ucControl;
    }

    pucSource = &auiSpiTxBuffer[0];
    usData = HammingDistanceCrc(pucSource,
                                MRXX_DISCRETE_OUTPUT_QUANTITY + 2);
    pucDestination = &auiSpiTxBuffer[SPI_DATA_BYTE_OFFSET +
                                     MRXX_DISCRETE_OUTPUT_QUANTITY];
    *pucDestination = (unsigned char)usData;

//    {
//        cout << "MRXX_SET_DATA_BASE_COMMAND " << endl;
//        unsigned char *pucSourceTemp;
//        pucSourceTemp = (unsigned char*)auiSpiTxBuffer;
//        for(int i=0; i<16; )
//        {
//            for(int j=0; j<8; j++)
//            {
//                cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
//            }
//            cout << endl;
//            i += 8;
//        }
//    }

    xSpiCommunicationDevice.Exchange(pxModuleContext ->
                                     xModuleContextStatic.
                                     ucModuleAddress,
                                     auiSpiTxBuffer,
                                     auiSpiRxBuffer,
                                     SPI_PREAMBLE_LENGTH +
                                     MRXX_SET_DATA_BASE_COMMAND_ANSWER_LENGTH +
                                     ONE_BYTE_CRC_LENGTH,
                                     LOW_SPEED_IN_HZ
                                    );

    return 0;
}

//-----------------------------------------------------------------------------------------------------
int iModuleMrXXDataBaseBlockWriteError(TModuleContext *pxModuleContext)
{
    // ���� ������ ����������?
    if ((pxModuleContext ->
            xModuleContextDinamic.
            usAuxiliaryCounter) == 0) //MRXX database error Debug
    {
//        cout << "MRXX_DATA_BASE_ERROR" << endl;
        fbDiscreteOutputModuleDataBaseError = 1;
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
                "������ �. ��");
        }
    }
    else
    {
        // �������� �������� �������� - "���������� ������� ����� � ������� ��� ������".
        (pxModuleContext ->
         xModuleContextDinamic.
         usAuxiliaryCounter)--;
    }
}

//-----------------------------------------------------------------------------------------------------
// ���������� ���� ������ �� RAM ������� � EEPROM ������.
int iModuleMrXXChangeProtocol(TModuleContext *pxModuleContext)
{
//    unsigned char auiSpiTxBuffer[TX_RX_BUFF_SIZE];
//    unsigned char auiSpiRxBuffer[TX_RX_BUFF_SIZE];

    auiSpiTxBuffer[0] = CHANGE_PROTOCOL_COMMAND;
    xSpiCommunicationDevice.Exchange(pxModuleContext ->
                                     xModuleContextStatic.
                                     ucModuleAddress,
                                     auiSpiTxBuffer,
                                     auiSpiRxBuffer,
                                     SPI_PREAMBLE_LENGTH +
                                     GET_MODULE_TYPE_COMMAND_ANSWER_LENGTH +
                                     ONE_BYTE_CRC_LENGTH,
                                     LOW_SPEED_IN_HZ);


//    cout << "CHANGE_PROTOCOL_COMMAND " << endl;
//    unsigned char *pucSourceTemp;
//    pucSourceTemp = (unsigned char*)auiSpiRxBuffer;
//    for(int i=0; i<16; )
//    {
//        for(int j=0; j<8; j++)
//        {
//            cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
//        }
//        cout << endl;
//        i += 8;
//    }

    // ������ ��������?
    if (auiSpiRxBuffer[GET_MODULE_TYPE_COMMAND_OFFSET] ==
            CHANGE_PROTOCOL_COMMAND) // if echo answer is ok.
    {
        // ����� ������� ������ ��� ������?
        if ((iCrcSummOneByteCompare(&auiSpiRxBuffer[GET_MODULE_TYPE_COMMAND_OFFSET],
                                    GET_MODULE_TYPE_COMMAND_LENGTH +
                                    GET_MODULE_TYPE_COMMAND_ANSWER_LENGTH) > 0))
        {
            // ���� ������������� ������ ���� ������ � EEPROM ������.
            cout << "iModuleMrXXChangeProtocol ok" << endl;
            return 0;
        }
    }

    cout << "iModuleMrXXChangeProtocol error" << endl;

    return -1;
}

//-----------------------------------------------------------------------------------------------------
// ������ ��������� ���� ������ ������� ����������� ������.
int iModuleMrXXStartDataBaseCreate(void)
{
    int i;
    unsigned char ucCheck;
    unsigned char nucBlockCounter;
    unsigned char nucBlocksInBlockCounter; // ���� ��������� - ���� ����, � ����� �����.
    unsigned char nucInputCounter;
    unsigned char nucModuleCounter;
    unsigned char *pucSource;
    unsigned char ucFlowControl;
    unsigned char aucTempArray[PLC_DATA_BASE_BLOCK_LENGTH];
    TMrXXOneChannelDataBase *pxMrXXOneChannelDataBaseSource;
    TMrXXOneChannelDataBase *pxMrXXOneChannelDataBaseDestination;

    memset(&aucTempArray[0],
           0,
           sizeof(aucTempArray));
    pxMrXXOneChannelDataBaseDestination = (TMrXXOneChannelDataBase*)&aucTempArray[0];
    // �������� ��������� ���� ������ ���������� �������.
    for (int i = 0;
            i < (xPlcConfigService.
                 xPlcConfigServiceData.
                 ucServiceDiscreteOutputModuleQuantity *
                 MRXX_DISCRETE_OUTPUT_QUANTITY);
            i++)
    {
        // ��� � ��� ������ 1 � �����������.
        pxMrXXOneChannelDataBaseDestination[i].ucControl |= OUTPUT_TYPE_MASK;
        // ��/��: ��� ������������ ������: 0 � ��� ����������.
        pxMrXXOneChannelDataBaseDestination[i].ucControl &= ~BLOCK_ALARM_TYPE_MASK;
        // ��� � ����� � �������� ������: 0 � ���.
        pxMrXXOneChannelDataBaseDestination[i].ucControl &= ~PC_RESET_MASK;
        // ��� � ������������ � �������� ������: 0 � ���.
        pxMrXXOneChannelDataBaseDestination[i].ucControl &= ~PC_KVIT_MASK;
        // ��� � ����� � ����������� ������ ����: 0 � ���.
        pxMrXXOneChannelDataBaseDestination[i].ucControl &= ~BUTTON_RESET_MASK;
        // ��� � ������������ � ����������� ������ ����: 0 � ���.
        pxMrXXOneChannelDataBaseDestination[i].ucControl &= ~BUTTON_KVIT_MASK;
        // D7 - ��� =1 ������������, 0 � �� ������������.
        pxMrXXOneChannelDataBaseDestination[i].ucControl &= ~ARCHIVE_MRXX_MASK;
        // ��_��� - ����� �������� ������� �������� ������������. ���.
        pxMrXXOneChannelDataBaseDestination[i].ucExternalKvitAddress = 0;
        // ��_��� - ����� �������� ������� �������� ������. ���.
        pxMrXXOneChannelDataBaseDestination[i].ucExternalResetAddress = 0;
    }

    // ������� ��������� �� ����� � ����� ��������� ��������� ����� ������ �������.
    pxMrXXOneChannelDataBaseSource = (TMrXXOneChannelDataBase*)&aucTempArray[0];
    // ������� ��������� �� ���� ������ ������� � ����� �������.
    pxMrXXOneChannelDataBaseDestination = (TMrXXOneChannelDataBase*)&(xPlcDataBase.
                                          axPlcDataBaseBlocks[DISCRETE_OUTPUT_MODULE_DATA_BASE_BLOCK_OFFSET].
                                          aucPlcDataBaseBlockData[0]);
    nucBlockCounter = 0;
    nucBlocksInBlockCounter = 0;
    // ����������� ��������� ���� ������ � ����� ������.
    for (int i = 0;
            i < MAX_HANDLED_DISCRETE_OUTPUT ;
            i++)
    {
        // ��������� ���� ���� ���� ������?
        if (nucBlocksInBlockCounter == DISCRETE_OUTPUT_MODULE_DATA_BASE_BLOCKS_IN_BLOCK_QUANTITY)
        {
            // ��������� ���� ���� ������.
            nucBlockCounter++;
            // ������� ��������� �� ��������� ���� � ����� ���� ������ �������.
            pxMrXXOneChannelDataBaseDestination = (TMrXXOneChannelDataBase*)&(xPlcDataBase.
                                                  axPlcDataBaseBlocks[DISCRETE_OUTPUT_MODULE_DATA_BASE_BLOCK_OFFSET + nucBlockCounter].
                                                  aucPlcDataBaseBlockData[0]);
            nucBlocksInBlockCounter = 0;
        }
        // ��������� ���� ��������� ����������� ������, � ����� ����� ���� ������ �������.
        pxMrXXOneChannelDataBaseDestination[i] =
            pxMrXXOneChannelDataBaseSource[i];
        // ��������� ���������.
        nucBlocksInBlockCounter++;
    }

    return 0;
}

//-----------------------------------------------------------------------------------------------------
// ������� �������.
void vModuleMrXXDataHandler(TModuleContext *pxModuleContext)
{
    switch (pxModuleContext ->
            xModuleContextDinamic.
            ucFsmState)
    {
    case MRXX_IDDLE:
        break;

    case MRXX_CHANGE_PROTOCOL:
        cout << "MRXX_CHANGE_PROTOCOL" << endl;
        if (iModuleMrXXChangeProtocol(pxModuleContext) != -1)
        {
            pxModuleContext ->
            xModuleContextDinamic.
            ucFsmState = MRXX_WRITE_DATA_BASE;
        }
        break;

    case MRXX_WRITE_DATA_BASE:
        cout << "MRXX_WRITE_DATA_BASE" << endl;
        iModuleMrXXDataBaseBlockWrite(pxModuleContext);
        pxModuleContext ->
        xModuleContextDinamic.
        ucFsmState = MRXX_WRITE_DATA_BASE_CHECK;
        break;

    case MRXX_WRITE_DATA_BASE_CHECK:
        cout << "MRXX_WRITE_DATA_BASE_CHECK" << endl;
        if (iModuleMrXXTransferData(pxModuleContext) != -1)
        {
            // �������� ����� ������ �� ���������� ������� - ������ ���� ������.
            switch (auiSpiRxBuffer[(SPI_MT_RX_OFFSET + SPI_MT_FUNCTION_CODE_OFFSET)])
            {
                // ���� ������ �������� �������?
            case MRXX_SET_DATA_BASE_COMMAND:
                cout << "MRXX_SET_DATA_BASE_COMMAND" << endl;
//                if (ucDataBaseError != )
//                {
                    // ������� �������������, ��� ������ ���, �� 14 ������� Modbus - ������ � ����������� ����������������.
                    ucDataBaseError = PROGRAMMING_DATA_BASE_WRITE_OK;
//                }
                pxModuleContext ->
                xModuleContextDinamic.
                ucFsmState = MRXX_DATA_EXCHANGE;
                break;
                // �� ����� ������ ���� ������ ��������� ������?
            case MRXX_SET_DATA_BASE_COMMAND_ERROR:
                cout << "MRXX_SET_DATA_BASE_COMMAND_ERROR" << endl;
//                ucDataBaseError = PROGRAMMING_MR_DATA_BASE_ERROR;
                iModuleMrXXDataBaseBlockWriteError(pxModuleContext);
                pxModuleContext ->
                xModuleContextDinamic.
                ucFsmState = MRXX_WRITE_DATA_BASE;
                break;

                // ��������� ������ ������?
            default:
                cout << "MRXX_SET_DATA_BASE_COMMAND_ERROR default" << endl;
                iModuleMrXXTransferDataReceiveError(pxModuleContext);
                pxModuleContext ->
                xModuleContextDinamic.
                ucFsmState = MRXX_WRITE_DATA_BASE;
                break;
            }
        }
        else
        {
            cout << "WRITE_DATA_BASE_CHECK  error" << endl;
            iModuleMrXXTransferDataReceiveError(pxModuleContext);
        }

        break;

    case MRXX_DATA_EXCHANGE:
//        cout << "DATA_EXCHANGE" << endl;
        if (iModuleMrXXTransferData(pxModuleContext) != -1)
        {
            // �������� ����� ������ �� ���������� ������� - ������ ���� ������.
            switch (auiSpiRxBuffer[(SPI_MT_RX_OFFSET + SPI_MT_FUNCTION_CODE_OFFSET)])
            {
                // ����� ������� ������ ��� ������?
            case MRXX_DATA_READY:
//                cout << "MRXX_DATA_READY" << endl;
                iModuleMrXXTransferDataReceive(pxModuleContext);
                break;
                // ��������� �������� ���� ������?
            case MRXX_DATA_BASE_ERROR:
                cout << "MRXX_DATA_BASE_ERROR" << endl;
                pxModuleContext ->
                xModuleContextDinamic.
                ucFsmState = MRXX_WRITE_DATA_BASE;
                break;
                // ��������� ������ ������?
            default:
                cout << "MRXX_SET_DATA_BASE_COMMAND_ERROR default" << endl;
                iModuleMrXXTransferDataReceiveError(pxModuleContext);
                break;
            }
        }
        else
        {
            cout << "MRXX_DATA_EXCHANGE  error" << endl;
            iModuleMrXXTransferDataReceiveError(pxModuleContext);
        }
        break;

    default:
        break;
    }
}


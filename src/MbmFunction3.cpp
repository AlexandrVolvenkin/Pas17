
#include "PasNewConfig.h"
#include "MbmFunction3.h"
#include "Error.h"
#include "Events.h"

using namespace std;

//-----------------------------------------------------------------------------------------------------
// ���������� ����� ������� � �������� ��������������� ����� ���̻ ������� Modbus - 3.
// ������������ ��� ��� "���������������".
int iMbmFunction3DataHandler(TModuleContext *pxModuleContext)
{
    unsigned char ucTempData;
    int i;
    float fResult;
    unsigned short usMaxCode;
    float fMinOfScale;
    float fMaxOfScale;
    short sCurrentMeasure;
    unsigned char *pucSource;
    unsigned char *pucDestination;
    TMbmFunction1234 *pxFunction1234Pointer;
    TAnalogueInputDescriptionWork *pxAnalogueInputDescriptionWork;
    unsigned char aucTempArray[PLC_DATA_BASE_BLOCK_LENGTH];

    memset(&aucTempArray[0],
           0,
           sizeof(aucTempArray));
    // ������� ��������� �� ��������� ��������������� ����� ���̻ � ���� ������.
    pxFunction1234Pointer = (TMbmFunction1234*)((pxModuleContext ->
                            xModuleContextStatic.
                            pucDataBasePointer) +
                            (pxModuleContext ->
                             xModuleContextStatic.
                             ucDataBaseDataOffset));

    // ������� ��������� �� ��������������� ���� ���� ������ ���������� ���������� ������.
    pxAnalogueInputDescriptionWork = (TAnalogueInputDescriptionWork*)(pxModuleContext ->
                                     xModuleContextStatic.
                                     pucAnalogueInputDescriptionWork);
    // ��������� ����� ������ ��������� ���� ������� ������.
    modbus_set_slave(pxMasterRtuCtx,
                     (pxFunction1234Pointer -> ucModbusSlaveAddress));
    // ������ ������ �� ������ ��������� ���� ������� ������ �� ��������� �����.
    i = modbus_read_registers(pxMasterRtuCtx,
                              (((unsigned short)(pxFunction1234Pointer -> ucAddressH) << 8) |
                               (unsigned short)(pxFunction1234Pointer -> ucAddressL)),
                              (((unsigned short)(pxFunction1234Pointer -> ucQuantityH) << 8) |
                               (unsigned short)(pxFunction1234Pointer -> ucQuantityL)),
                              (unsigned short*)&aucTempArray[0]);

    pthread_mutex_lock(&xMainDataAccessMutex);
    // ������ ��������?
    if (i != -1)
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
                ucModuleAddress,
                (CEvents::HANDLED_EVENTS_EXTERNAL_MODULES_BAD_TYPE |
                 CEvents::HANDLED_EVENTS_IS_ARCHIVE),
                0,
                "����� SL");
        }

        //iBadModuleBuffUpdate();
        // ������ ������ ����������.
        memset(pxModuleContext ->
               xModuleContextStatic.
               pucBadDiBufferPointer,
               0,
               ((pxFunction1234Pointer -> ucIOQuantity) *
                ANALOGUE_INPUT_DI_VALUE_QUANTITY));
        // ������� ������ - ��������� ���������� ������������� �������.
        for (i = 0; i < (pxFunction1234Pointer -> ucIOQuantity); i++)
        {
            pxModuleContext ->
            xModuleContextStatic.
            pucStatAiBufferPointer[i] &=
                ~(ANALOGUE_INPUT_CHANNEL_CALIBRATION |
                  ANALOGUE_INPUT_LINE_BREAK |
                  ANALOGUE_INPUT_CALCULATION_OVERFLOW);
        }
        // ����� ������ ������, ���������� �� ������ ��������� ���� ������� ������?
        switch(((pxFunction1234Pointer ->
                 ucTust) & TUST_DATA_FORMAT_MASK) >> TUST_DATA_FORMAT_OFFSET)
        {
        case SCALABLE_CODE_16_BIT:
            // ���������� ������������������� ���������� ������ ��� ������� ��������������� �����.
            for (i = 0; i < (pxFunction1234Pointer -> ucIOQuantity); i++)
            {
                // ������� ���������� �������� �������� ����������� �����.
                // big endian to little endian.
                ((unsigned char*)&sCurrentMeasure)[0] = aucTempArray[(i * sizeof(unsigned short))];
                ((unsigned char*)&sCurrentMeasure)[1] = aucTempArray[(i * sizeof(unsigned short)) + 1];

                // ������� ��� ��� ����������� �������.
                usMaxCode = (((unsigned short)(pxFunction1234Pointer ->
                                               ucMaxCodeH) << 8) |
                             (unsigned short)(pxFunction1234Pointer ->
                                              ucMaxCodeL));
                // ���������� ���� ������� �� ���������?
                if ((pxModuleContext ->
                        xModuleContextStatic.
                        pucAinOffBufferPointer[i]))
                {
                    // ������ ������ ������ �������� �� ���������, ������� ��.
                    memset(&(pxModuleContext ->
                             xModuleContextStatic.
                             pucAiValueBufferPointer[i * sizeof(float)]),
                           0,
                           sizeof(float));
                    // ��������� ����� ��������������� - ����� ������������.
                    pxModuleContext ->
                    xModuleContextStatic.
                    pucBadAiBufferPointer[i] = 1;
                    // ������ ������ ������ �������� �� ���������, ������� ��.
                    memset(&(pxModuleContext ->
                             xModuleContextStatic.
                             pucDiValueBufferPointer[i * ANALOGUE_INPUT_DI_VALUE_QUANTITY]),
                           0,
                           ANALOGUE_INPUT_DI_VALUE_QUANTITY);
                    // ��������� ����� ��������������� - ����� ������������.
                    memset(&(pxModuleContext ->
                             xModuleContextStatic.
                             pucBadDiBufferPointer[i * ANALOGUE_INPUT_DI_VALUE_QUANTITY]),
                           1,
                           ANALOGUE_INPUT_DI_VALUE_QUANTITY);
                    // ������� ���� - ��������� ���������� ������������� �������, �������� ������.
                    pxModuleContext ->
                    xModuleContextStatic.
                    pucStatAiBufferPointer[i] = 0;
                }
                //  ��� ���������� ��������� ������ ������ ������ (0 - 3 �������� MaxCode)?
                //  ��� ���������� ��������� ������ ������ ������ (MaxCode + 3 �������� MaxCode)?
                else if ((sCurrentMeasure <
                          ((float)0 - (((float)usMaxCode / PERCENT_100) * MEASURE_ERROR_THRESHOLD))) ||
                         (sCurrentMeasure >
                          ((float)usMaxCode + (((float)usMaxCode / PERCENT_100) * MEASURE_ERROR_THRESHOLD))))
                {
                    // ������ ������ ������ �������� �� ���������, ������� ��.
                    memset(&(pxModuleContext ->
                             xModuleContextStatic.
                             pucAiValueBufferPointer[i * sizeof(float)]),
                           0,
                           sizeof(float));
                    // ��������� ����� ��������������� - ����� ������������.
                    pxModuleContext ->
                    xModuleContextStatic.
                    pucBadAiBufferPointer[i] = 1;
                    // ������ ������ ������ �������� �� ���������, ������� ��.
                    memset(&(pxModuleContext ->
                             xModuleContextStatic.
                             pucDiValueBufferPointer[i * ANALOGUE_INPUT_DI_VALUE_QUANTITY]),
                           0,
                           ANALOGUE_INPUT_DI_VALUE_QUANTITY);
                    // ��������� ����� ��������������� - ����� ������������.
                    memset(&(pxModuleContext ->
                             xModuleContextStatic.
                             pucBadDiBufferPointer[i * ANALOGUE_INPUT_DI_VALUE_QUANTITY]),
                           1,
                           ANALOGUE_INPUT_DI_VALUE_QUANTITY);
                    // ������� ���� - ��������� ���������� ������������� �������, �������� ������.
                    pxModuleContext ->
                    xModuleContextStatic.
                    pucStatAiBufferPointer[i] |=
                        ANALOGUE_INPUT_LINE_BREAK;
                }
                else
                {
                    // ������� �������� ��� - ������ ������ ����� �������, �� ���������
                    // ����������� ����� ���� ������.
                    fMinOfScale = pxAnalogueInputDescriptionWork[(pxFunction1234Pointer ->
                                  ucAiValueArrayBegin) + i].
                                  fMinOfScale;
                    // ������� �������� ��� - ������� ������ ����� �������, �� ���������
                    // ����������� ����� ���� ������.
                    fMaxOfScale = pxAnalogueInputDescriptionWork[(pxFunction1234Pointer ->
                                  ucAiValueArrayBegin) + i].
                                  fMaxOfScale;
//                    // ������� ��� ��� ����������� �������.
//                    usMaxCode = (((unsigned short)(pxFunction1234Pointer ->
//                                                   ucMaxCodeH) << 8) |
//                                 (unsigned short)(pxFunction1234Pointer ->
//                                                  ucMaxCodeL));

                    // ������ �� ������ ���������� � ���� ������ ����� � � ��������� �� 0 ��
                    // 8191 (0x1FFF), ��������������� ����� ��������������� ���, ����������������  ��-
                    // ������� �������� ������� (4-20 ��). ��� ��������� ����������� �������� � ������-
                    // ���� �������� ����������� ��������� �� ������� MASTER ������ ���� ���������
                    // ��������������� �� �������:
                    // Y = (X / 8191) (MAX � MIN) + MIN,
                    // ���  ��� � �������� �����, MIN � ������� ����� ������� � ���������� ���������
                    // ����������� ���������.
                    // � ���-�� ��, ���-�� �� �������� ������ ������� �� ������� ��� �� �����-
                    // ��� �����.
                    // ��������� ��������������, ������� ���������� �������� � ������-
                    // ���� �������� ����������� ���������.
                    fResult = ((((float)sCurrentMeasure /
                                 (float)usMaxCode) *
                                (fMaxOfScale - fMinOfScale)) +
                               fMinOfScale);

                    // �������� ��� � ������� ������ - ���������� �������� ������� ���������� ��������.
                    memcpy(&(pxModuleContext ->
                             xModuleContextStatic.
                             pucAiValueBufferPointer[i * sizeof(float)]),
                           (unsigned char*)&fResult,
                           sizeof(float));
                    // ������� ���� ��������������� - ���� ����������.
                    pxModuleContext ->
                    xModuleContextStatic.
                    pucBadAiBufferPointer[i] = 0;
                    // ������� ����� ������� LL, L, H, HH.
                    memset(&(pxModuleContext ->
                             xModuleContextStatic.
                             pucDiValueBufferPointer[i *
                                                     ANALOGUE_INPUT_DI_VALUE_QUANTITY]),
                           0,
                           ANALOGUE_INPUT_DI_VALUE_QUANTITY);
                    // ������� ���� ��������������� - ���������� ������� ������� LL, L, H, HH ����������.
                    memset(&(pxModuleContext ->
                             xModuleContextStatic.
                             pucBadDiBufferPointer[i * ANALOGUE_INPUT_DI_VALUE_QUANTITY]),
                           0,
                           ANALOGUE_INPUT_DI_VALUE_QUANTITY);
                    // ������� ���� - ��������� ���������� ������������� �������, �������� ������.
                    pxModuleContext ->
                    xModuleContextStatic.
                    pucStatAiBufferPointer[i] &=
                        ~(ANALOGUE_INPUT_SET_POINT_VIOLATION_LL_L |
                          ANALOGUE_INPUT_SET_POINT_VIOLATION_L |
                          ANALOGUE_INPUT_SET_POINT_VIOLATION_H |
                          ANALOGUE_INPUT_SET_POINT_VIOLATION_HH_H);
                    // �������� ������� LL + L?
                    if (fResult < (pxAnalogueInputDescriptionWork[(pxFunction1234Pointer ->
                                   ucAiValueArrayBegin) + i].
                                   fSetPointLL))
                    {
                        // ��������� ���� ��������� ������� LL + L.
                        pxModuleContext ->
                        xModuleContextStatic.
                        pucStatAiBufferPointer[i] |=
                            ANALOGUE_INPUT_SET_POINT_VIOLATION_LL_L;
                        // ��������� ���� ��������� ������� LL.
                        (pxModuleContext ->
                         xModuleContextStatic.
                         pucDiValueBufferPointer[(i *
                                                  ANALOGUE_INPUT_DI_VALUE_QUANTITY) +
                                                 ANALOGUE_INPUT_SET_POINT_VIOLATION_LL_L_OFFSET]) = 1;
                        // ��������� ���� ��������� ������� L.
                        (pxModuleContext ->
                         xModuleContextStatic.
                         pucDiValueBufferPointer[(i *
                                                  ANALOGUE_INPUT_DI_VALUE_QUANTITY) +
                                                 ANALOGUE_INPUT_SET_POINT_VIOLATION_L_OFFSET]) = 1;
                    }
                    // �������� ������� L?
                    if (fResult < (pxAnalogueInputDescriptionWork[(pxFunction1234Pointer ->
                                   ucAiValueArrayBegin) + i].
                                   fSetPointL))
                    {
                        // ��������� ���� ��������� ������� L.
                        pxModuleContext ->
                        xModuleContextStatic.
                        pucStatAiBufferPointer[i] |=
                            ANALOGUE_INPUT_SET_POINT_VIOLATION_L;
                        // ��������� ���� ��������� ������� L.
                        (pxModuleContext ->
                         xModuleContextStatic.
                         pucDiValueBufferPointer[(i *
                                                  ANALOGUE_INPUT_DI_VALUE_QUANTITY) +
                                                 ANALOGUE_INPUT_SET_POINT_VIOLATION_L_OFFSET]) = 1;
                    }
                    // �������� ������� H?
                    if (fResult > (pxAnalogueInputDescriptionWork[(pxFunction1234Pointer ->
                                   ucAiValueArrayBegin) + i].
                                   fSetPointH))
                    {
                        // ��������� ���� ��������� ������� H.
                        pxModuleContext ->
                        xModuleContextStatic.
                        pucStatAiBufferPointer[i] |=
                            ANALOGUE_INPUT_SET_POINT_VIOLATION_H;
                        // ��������� ���� ��������� ������� H.
                        (pxModuleContext ->
                         xModuleContextStatic.
                         pucDiValueBufferPointer[(i *
                                                  ANALOGUE_INPUT_DI_VALUE_QUANTITY) +
                                                 ANALOGUE_INPUT_SET_POINT_VIOLATION_H_OFFSET]) = 1;
                    }
                    // �������� ������� HH + H?
                    if (fResult > (pxAnalogueInputDescriptionWork[(pxFunction1234Pointer ->
                                   ucAiValueArrayBegin) + i].
                                   fSetPointHH))
                    {
                        // ��������� ���� ��������� ������� HH + H.
                        pxModuleContext ->
                        xModuleContextStatic.
                        pucStatAiBufferPointer[i] |=
                            ANALOGUE_INPUT_SET_POINT_VIOLATION_HH_H;
                        // ��������� ���� ��������� ������� H.
                        (pxModuleContext ->
                         xModuleContextStatic.
                         pucDiValueBufferPointer[(i *
                                                  ANALOGUE_INPUT_DI_VALUE_QUANTITY) +
                                                 ANALOGUE_INPUT_SET_POINT_VIOLATION_H_OFFSET]) = 1;
                        // ��������� ���� ��������� ������� HH.
                        (pxModuleContext ->
                         xModuleContextStatic.
                         pucDiValueBufferPointer[(i *
                                                  ANALOGUE_INPUT_DI_VALUE_QUANTITY) +
                                                 ANALOGUE_INPUT_SET_POINT_VIOLATION_HH_H_OFFSET]) = 1;
                    }
                }
            }
            break;

        case IEEE754_BYTE_ORDER_MSB_LSB:
            // ���������� ������������������� ���������� ������ ��� ������� ��������������� �����.
            for (i = 0; i < (pxFunction1234Pointer -> ucIOQuantity); i++)
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
                    // ��������� ����� ��������������� - ����� ������������.
                    pxModuleContext ->
                    xModuleContextStatic.
                    pucBadAiBufferPointer[i] = 1;
                    // ������ ������ ������ �������� �� ���������, ������� ��.
                    memset(&(pxModuleContext ->
                             xModuleContextStatic.
                             pucDiValueBufferPointer[i * ANALOGUE_INPUT_DI_VALUE_QUANTITY]),
                           0,
                           ANALOGUE_INPUT_DI_VALUE_QUANTITY);
                    // ��������� ����� ��������������� - ����� ������������.
                    memset(&(pxModuleContext ->
                             xModuleContextStatic.
                             pucBadDiBufferPointer[i * ANALOGUE_INPUT_DI_VALUE_QUANTITY]),
                           1,
                           ANALOGUE_INPUT_DI_VALUE_QUANTITY);
                    // ������� ���� - ��������� ���������� ������������� �������, �������� ������.
                    pxModuleContext ->
                    xModuleContextStatic.
                    pucStatAiBufferPointer[i] = 0;
                }
                else
                {
                    fResult = 0;
                    // ������� ���������� �������� �������� ����������� �����.
                    // big endian to little endian.
                    ((unsigned char*)&fResult)[0] = aucTempArray[(i * sizeof(float)) + 2];
                    ((unsigned char*)&fResult)[1] = aucTempArray[(i * sizeof(float)) + 3];
                    ((unsigned char*)&fResult)[2] = aucTempArray[(i * sizeof(float)) + 0];
                    ((unsigned char*)&fResult)[3] = aucTempArray[(i * sizeof(float)) + 1];
                    // �������� ��� � ������� ������ - ���������� �������� ������� ���������� ��������.
                    memcpy(&(pxModuleContext ->
                             xModuleContextStatic.
                             pucAiValueBufferPointer[i * sizeof(float)]),
                           (unsigned char*)&fResult,
                           sizeof(float));
                    // ������� ���� ��������������� - ���� ����������.
                    pxModuleContext ->
                    xModuleContextStatic.
                    pucBadAiBufferPointer[i] = 0;
                    // ������� ����� ������� LL, L, H, HH.
                    memset(&(pxModuleContext ->
                             xModuleContextStatic.
                             pucDiValueBufferPointer[i *
                                                     ANALOGUE_INPUT_DI_VALUE_QUANTITY]),
                           0,
                           ANALOGUE_INPUT_DI_VALUE_QUANTITY);
                    // ������� ���� ��������������� - ���������� ������� ������� LL, L, H, HH ����������.
                    memset(&(pxModuleContext ->
                             xModuleContextStatic.
                             pucBadDiBufferPointer[i * ANALOGUE_INPUT_DI_VALUE_QUANTITY]),
                           0,
                           ANALOGUE_INPUT_DI_VALUE_QUANTITY);
                    // ������� ���� - ��������� ���������� ������������� �������, �������� ������.
                    pxModuleContext ->
                    xModuleContextStatic.
                    pucStatAiBufferPointer[i] &=
                        ~(ANALOGUE_INPUT_SET_POINT_VIOLATION_LL_L |
                          ANALOGUE_INPUT_SET_POINT_VIOLATION_L |
                          ANALOGUE_INPUT_SET_POINT_VIOLATION_H |
                          ANALOGUE_INPUT_SET_POINT_VIOLATION_HH_H);

                    // �������� ������� LL + L?
                    if (fResult < (pxAnalogueInputDescriptionWork[(pxFunction1234Pointer ->
                                   ucAiValueArrayBegin) + i].
                                   fSetPointLL))
                    {
                        // ��������� ���� ��������� ������� LL + L.
                        pxModuleContext ->
                        xModuleContextStatic.
                        pucStatAiBufferPointer[i] |=
                            ANALOGUE_INPUT_SET_POINT_VIOLATION_LL_L;
                        // ��������� ���� ��������� ������� LL.
                        (pxModuleContext ->
                         xModuleContextStatic.
                         pucDiValueBufferPointer[(i *
                                                  ANALOGUE_INPUT_DI_VALUE_QUANTITY) +
                                                 ANALOGUE_INPUT_SET_POINT_VIOLATION_LL_L_OFFSET]) = 1;
                        // ��������� ���� ��������� ������� L.
                        (pxModuleContext ->
                         xModuleContextStatic.
                         pucDiValueBufferPointer[(i *
                                                  ANALOGUE_INPUT_DI_VALUE_QUANTITY) +
                                                 ANALOGUE_INPUT_SET_POINT_VIOLATION_L_OFFSET]) = 1;
                    }
                    // �������� ������� L?
                    if (fResult < (pxAnalogueInputDescriptionWork[(pxFunction1234Pointer ->
                                   ucAiValueArrayBegin) + i].
                                   fSetPointL))
                    {
                        // ��������� ���� ��������� ������� L.
                        pxModuleContext ->
                        xModuleContextStatic.
                        pucStatAiBufferPointer[i] |=
                            ANALOGUE_INPUT_SET_POINT_VIOLATION_L;
                        // ��������� ���� ��������� ������� L.
                        (pxModuleContext ->
                         xModuleContextStatic.
                         pucDiValueBufferPointer[(i *
                                                  ANALOGUE_INPUT_DI_VALUE_QUANTITY) +
                                                 ANALOGUE_INPUT_SET_POINT_VIOLATION_L_OFFSET]) = 1;
                    }
                    // �������� ������� H?
                    if (fResult > (pxAnalogueInputDescriptionWork[(pxFunction1234Pointer ->
                                   ucAiValueArrayBegin) + i].
                                   fSetPointH))
                    {
                        // ��������� ���� ��������� ������� H.
                        pxModuleContext ->
                        xModuleContextStatic.
                        pucStatAiBufferPointer[i] |=
                            ANALOGUE_INPUT_SET_POINT_VIOLATION_H;
                        // ��������� ���� ��������� ������� H.
                        (pxModuleContext ->
                         xModuleContextStatic.
                         pucDiValueBufferPointer[(i *
                                                  ANALOGUE_INPUT_DI_VALUE_QUANTITY) +
                                                 ANALOGUE_INPUT_SET_POINT_VIOLATION_H_OFFSET]) = 1;
                    }
                    // �������� ������� HH + H?
                    if (fResult > (pxAnalogueInputDescriptionWork[(pxFunction1234Pointer ->
                                   ucAiValueArrayBegin) + i].
                                   fSetPointHH))
                    {
                        // ��������� ���� ��������� ������� HH + H.
                        pxModuleContext ->
                        xModuleContextStatic.
                        pucStatAiBufferPointer[i] |=
                            ANALOGUE_INPUT_SET_POINT_VIOLATION_HH_H;
                        // ��������� ���� ��������� ������� H.
                        (pxModuleContext ->
                         xModuleContextStatic.
                         pucDiValueBufferPointer[(i *
                                                  ANALOGUE_INPUT_DI_VALUE_QUANTITY) +
                                                 ANALOGUE_INPUT_SET_POINT_VIOLATION_H_OFFSET]) = 1;
                        // ��������� ���� ��������� ������� HH.
                        (pxModuleContext ->
                         xModuleContextStatic.
                         pucDiValueBufferPointer[(i *
                                                  ANALOGUE_INPUT_DI_VALUE_QUANTITY) +
                                                 ANALOGUE_INPUT_SET_POINT_VIOLATION_HH_H_OFFSET]) = 1;
                    }
                }
            }
            break;

        case IEEE754_BYTE_ORDER_LSB_MSB:
            // ���������� ������������������� ���������� ������ ��� ������� ��������������� �����.
            for (i = 0; i < (pxFunction1234Pointer -> ucIOQuantity); i++)
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
                    // ��������� ����� ��������������� - ����� ������������.
                    pxModuleContext ->
                    xModuleContextStatic.
                    pucBadAiBufferPointer[i] = 1;
                    // ������ ������ ������ �������� �� ���������, ������� ��.
                    memset(&(pxModuleContext ->
                             xModuleContextStatic.
                             pucDiValueBufferPointer[i * ANALOGUE_INPUT_DI_VALUE_QUANTITY]),
                           0,
                           ANALOGUE_INPUT_DI_VALUE_QUANTITY);
                    // ��������� ����� ��������������� - ����� ������������.
                    memset(&(pxModuleContext ->
                             xModuleContextStatic.
                             pucBadDiBufferPointer[i * ANALOGUE_INPUT_DI_VALUE_QUANTITY]),
                           1,
                           ANALOGUE_INPUT_DI_VALUE_QUANTITY);
                    // ������� ���� - ��������� ���������� ������������� �������, �������� ������.
                    pxModuleContext ->
                    xModuleContextStatic.
                    pucStatAiBufferPointer[i] = 0;
                }
                else
                {
                    fResult = 0;
                    // ������� ���������� �������� �������� ����������� �����.
                    ((unsigned char*)&fResult)[0] = aucTempArray[(i * sizeof(float)) + 2];
                    ((unsigned char*)&fResult)[1] = aucTempArray[(i * sizeof(float)) + 3];
                    ((unsigned char*)&fResult)[2] = aucTempArray[(i * sizeof(float)) + 0];
                    ((unsigned char*)&fResult)[3] = aucTempArray[(i * sizeof(float)) + 1];
                    // �������� ��� � ������� ������ - ���������� �������� ������� ���������� ��������.
                    memcpy(&(pxModuleContext ->
                             xModuleContextStatic.
                             pucAiValueBufferPointer[i * sizeof(float)]),
                           (unsigned char*)&fResult,
                           sizeof(float));
                    // ������� ���� ��������������� - ���� ����������.
                    pxModuleContext ->
                    xModuleContextStatic.
                    pucBadAiBufferPointer[i] = 0;
                    // ������� ����� ������� LL, L, H, HH.
                    memset(&(pxModuleContext ->
                             xModuleContextStatic.
                             pucDiValueBufferPointer[i *
                                                     ANALOGUE_INPUT_DI_VALUE_QUANTITY]),
                           0,
                           ANALOGUE_INPUT_DI_VALUE_QUANTITY);
                    // ������� ���� - ��������� ���������� ������������� �������, �������� ������.
                    pxModuleContext ->
                    xModuleContextStatic.
                    pucStatAiBufferPointer[i] &=
                        ~(ANALOGUE_INPUT_SET_POINT_VIOLATION_LL_L |
                          ANALOGUE_INPUT_SET_POINT_VIOLATION_L |
                          ANALOGUE_INPUT_SET_POINT_VIOLATION_H |
                          ANALOGUE_INPUT_SET_POINT_VIOLATION_HH_H);

                    // �������� ������� LL + L?
                    if (fResult < (pxAnalogueInputDescriptionWork[(pxFunction1234Pointer ->
                                   ucAiValueArrayBegin) + i].
                                   fSetPointLL))
                    {
                        // ��������� ���� ��������� ������� LL + L.
                        pxModuleContext ->
                        xModuleContextStatic.
                        pucStatAiBufferPointer[i] |=
                            ANALOGUE_INPUT_SET_POINT_VIOLATION_LL_L;
                        // ��������� ���� ��������� ������� LL.
                        (pxModuleContext ->
                         xModuleContextStatic.
                         pucDiValueBufferPointer[(i *
                                                  ANALOGUE_INPUT_DI_VALUE_QUANTITY) +
                                                 ANALOGUE_INPUT_SET_POINT_VIOLATION_LL_L_OFFSET]) = 1;
                        // ��������� ���� ��������� ������� L.
                        (pxModuleContext ->
                         xModuleContextStatic.
                         pucDiValueBufferPointer[(i *
                                                  ANALOGUE_INPUT_DI_VALUE_QUANTITY) +
                                                 ANALOGUE_INPUT_SET_POINT_VIOLATION_L_OFFSET]) = 1;
                    }
                    // �������� ������� L?
                    if (fResult < (pxAnalogueInputDescriptionWork[(pxFunction1234Pointer ->
                                   ucAiValueArrayBegin) + i].
                                   fSetPointL))
                    {
                        // ��������� ���� ��������� ������� L.
                        pxModuleContext ->
                        xModuleContextStatic.
                        pucStatAiBufferPointer[i] |=
                            ANALOGUE_INPUT_SET_POINT_VIOLATION_L;
                        // ��������� ���� ��������� ������� L.
                        (pxModuleContext ->
                         xModuleContextStatic.
                         pucDiValueBufferPointer[(i *
                                                  ANALOGUE_INPUT_DI_VALUE_QUANTITY) +
                                                 ANALOGUE_INPUT_SET_POINT_VIOLATION_L_OFFSET]) = 1;
                    }
                    // �������� ������� H?
                    if (fResult > (pxAnalogueInputDescriptionWork[(pxFunction1234Pointer ->
                                   ucAiValueArrayBegin) + i].
                                   fSetPointH))
                    {
                        // ��������� ���� ��������� ������� H.
                        pxModuleContext ->
                        xModuleContextStatic.
                        pucStatAiBufferPointer[i] |=
                            ANALOGUE_INPUT_SET_POINT_VIOLATION_H;
                        // ��������� ���� ��������� ������� H.
                        (pxModuleContext ->
                         xModuleContextStatic.
                         pucDiValueBufferPointer[(i *
                                                  ANALOGUE_INPUT_DI_VALUE_QUANTITY) +
                                                 ANALOGUE_INPUT_SET_POINT_VIOLATION_H_OFFSET]) = 1;
                    }
                    // �������� ������� HH + H?
                    if (fResult > (pxAnalogueInputDescriptionWork[(pxFunction1234Pointer ->
                                   ucAiValueArrayBegin) + i].
                                   fSetPointHH))
                    {
                        // ��������� ���� ��������� ������� HH + H.
                        pxModuleContext ->
                        xModuleContextStatic.
                        pucStatAiBufferPointer[i] |=
                            ANALOGUE_INPUT_SET_POINT_VIOLATION_HH_H;
                        // ��������� ���� ��������� ������� H.
                        (pxModuleContext ->
                         xModuleContextStatic.
                         pucDiValueBufferPointer[(i *
                                                  ANALOGUE_INPUT_DI_VALUE_QUANTITY) +
                                                 ANALOGUE_INPUT_SET_POINT_VIOLATION_H_OFFSET]) = 1;
                        // ��������� ���� ��������� ������� HH.
                        (pxModuleContext ->
                         xModuleContextStatic.
                         pucDiValueBufferPointer[(i *
                                                  ANALOGUE_INPUT_DI_VALUE_QUANTITY) +
                                                 ANALOGUE_INPUT_SET_POINT_VIOLATION_HH_H_OFFSET]) = 1;
                    }
                }
            }
            break;

        case IEEE754_BYTE_ORDER_LSR_MSR:
            // ���������� ������������������� ���������� ������ ��� ������� ��������������� �����.
            for (i = 0; i < (pxFunction1234Pointer -> ucIOQuantity); i++)
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
                    // ��������� ����� ��������������� - ����� ������������.
                    pxModuleContext ->
                    xModuleContextStatic.
                    pucBadAiBufferPointer[i] = 1;
                    // ������ ������ ������ �������� �� ���������, ������� ��.
                    memset(&(pxModuleContext ->
                             xModuleContextStatic.
                             pucDiValueBufferPointer[i * ANALOGUE_INPUT_DI_VALUE_QUANTITY]),
                           0,
                           ANALOGUE_INPUT_DI_VALUE_QUANTITY);
                    // ��������� ����� ��������������� - ����� ������������.
                    memset(&(pxModuleContext ->
                             xModuleContextStatic.
                             pucBadDiBufferPointer[i * ANALOGUE_INPUT_DI_VALUE_QUANTITY]),
                           1,
                           ANALOGUE_INPUT_DI_VALUE_QUANTITY);
                    // ������� ���� - ��������� ���������� ������������� �������, �������� ������.
                    pxModuleContext ->
                    xModuleContextStatic.
                    pucStatAiBufferPointer[i] = 0;
                }
                else
                {
                    fResult = 0;
                    // ������� ���������� �������� �������� ����������� �����.
                    ((unsigned char*)&fResult)[0] = aucTempArray[(i * sizeof(float)) + 0];
                    ((unsigned char*)&fResult)[1] = aucTempArray[(i * sizeof(float)) + 1];
                    ((unsigned char*)&fResult)[2] = aucTempArray[(i * sizeof(float)) + 2];
                    ((unsigned char*)&fResult)[3] = aucTempArray[(i * sizeof(float)) + 3];
                    // �������� ��� � ������� ������ - ���������� �������� ������� ���������� ��������.
                    memcpy(&(pxModuleContext ->
                             xModuleContextStatic.
                             pucAiValueBufferPointer[i * sizeof(float)]),
                           (unsigned char*)&fResult,
                           sizeof(float));
                    // ������� ���� ��������������� - ���� ����������.
                    pxModuleContext ->
                    xModuleContextStatic.
                    pucBadAiBufferPointer[i] = 0;
                    // ������� ����� ������� LL, L, H, HH.
                    memset(&(pxModuleContext ->
                             xModuleContextStatic.
                             pucDiValueBufferPointer[i *
                                                     ANALOGUE_INPUT_DI_VALUE_QUANTITY]),
                           0,
                           ANALOGUE_INPUT_DI_VALUE_QUANTITY);
                    // ������� ���� - ��������� ���������� ������������� �������, �������� ������.
                    pxModuleContext ->
                    xModuleContextStatic.
                    pucStatAiBufferPointer[i] &=
                        ~(ANALOGUE_INPUT_SET_POINT_VIOLATION_LL_L |
                          ANALOGUE_INPUT_SET_POINT_VIOLATION_L |
                          ANALOGUE_INPUT_SET_POINT_VIOLATION_H |
                          ANALOGUE_INPUT_SET_POINT_VIOLATION_HH_H);

                    // �������� ������� LL + L?
                    if (fResult < (pxAnalogueInputDescriptionWork[(pxFunction1234Pointer ->
                                   ucAiValueArrayBegin) + i].
                                   fSetPointLL))
                    {
                        // ��������� ���� ��������� ������� LL + L.
                        pxModuleContext ->
                        xModuleContextStatic.
                        pucStatAiBufferPointer[i] |=
                            ANALOGUE_INPUT_SET_POINT_VIOLATION_LL_L;
                        // ��������� ���� ��������� ������� LL.
                        (pxModuleContext ->
                         xModuleContextStatic.
                         pucDiValueBufferPointer[(i *
                                                  ANALOGUE_INPUT_DI_VALUE_QUANTITY) +
                                                 ANALOGUE_INPUT_SET_POINT_VIOLATION_LL_L_OFFSET]) = 1;
                        // ��������� ���� ��������� ������� L.
                        (pxModuleContext ->
                         xModuleContextStatic.
                         pucDiValueBufferPointer[(i *
                                                  ANALOGUE_INPUT_DI_VALUE_QUANTITY) +
                                                 ANALOGUE_INPUT_SET_POINT_VIOLATION_L_OFFSET]) = 1;
                    }
                    // �������� ������� L?
                    if (fResult < (pxAnalogueInputDescriptionWork[(pxFunction1234Pointer ->
                                   ucAiValueArrayBegin) + i].
                                   fSetPointL))
                    {
                        // ��������� ���� ��������� ������� L.
                        pxModuleContext ->
                        xModuleContextStatic.
                        pucStatAiBufferPointer[i] |=
                            ANALOGUE_INPUT_SET_POINT_VIOLATION_L;
                        // ��������� ���� ��������� ������� L.
                        (pxModuleContext ->
                         xModuleContextStatic.
                         pucDiValueBufferPointer[(i *
                                                  ANALOGUE_INPUT_DI_VALUE_QUANTITY) +
                                                 ANALOGUE_INPUT_SET_POINT_VIOLATION_L_OFFSET]) = 1;
                    }
                    // �������� ������� H?
                    if (fResult > (pxAnalogueInputDescriptionWork[(pxFunction1234Pointer ->
                                   ucAiValueArrayBegin) + i].
                                   fSetPointH))
                    {
                        // ��������� ���� ��������� ������� H.
                        pxModuleContext ->
                        xModuleContextStatic.
                        pucStatAiBufferPointer[i] |=
                            ANALOGUE_INPUT_SET_POINT_VIOLATION_H;
                        // ��������� ���� ��������� ������� H.
                        (pxModuleContext ->
                         xModuleContextStatic.
                         pucDiValueBufferPointer[(i *
                                                  ANALOGUE_INPUT_DI_VALUE_QUANTITY) +
                                                 ANALOGUE_INPUT_SET_POINT_VIOLATION_H_OFFSET]) = 1;
                    }
                    // �������� ������� HH + H?
                    if (fResult > (pxAnalogueInputDescriptionWork[(pxFunction1234Pointer ->
                                   ucAiValueArrayBegin) + i].
                                   fSetPointHH))
                    {
                        // ��������� ���� ��������� ������� HH + H.
                        pxModuleContext ->
                        xModuleContextStatic.
                        pucStatAiBufferPointer[i] |=
                            ANALOGUE_INPUT_SET_POINT_VIOLATION_HH_H;
                        // ��������� ���� ��������� ������� H.
                        (pxModuleContext ->
                         xModuleContextStatic.
                         pucDiValueBufferPointer[(i *
                                                  ANALOGUE_INPUT_DI_VALUE_QUANTITY) +
                                                 ANALOGUE_INPUT_SET_POINT_VIOLATION_H_OFFSET]) = 1;
                        // ��������� ���� ��������� ������� HH.
                        (pxModuleContext ->
                         xModuleContextStatic.
                         pucDiValueBufferPointer[(i *
                                                  ANALOGUE_INPUT_DI_VALUE_QUANTITY) +
                                                 ANALOGUE_INPUT_SET_POINT_VIOLATION_HH_H_OFFSET]) = 1;
                    }
                }
            }
            break;

        default:
            break;
        };
    }
    else
    {
        // ������ ������� �����������?
        if ((pxModuleContext ->
                xModuleContextDinamic.
                nucBadAnswerCounter) == 0)
        {
            // ������� ��� ������;
            (pxModuleContext ->
             xModuleContextDinamic.
             ucErrorCode) = CError::ModbusErrorToPlcError(errno);

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
                    ucModuleAddress,
                    (CEvents::HANDLED_EVENTS_EXTERNAL_MODULES_BAD_TYPE |
//                     CEvents::HANDLED_EVENTS_IS_POPUP |
//                     CEvents::HANDLED_EVENTS_IS_SOUND |
                     CEvents::HANDLED_EVENTS_IS_ARCHIVE),
                    CError::ModbusErrorToPlcError(errno),
                    "������ SL");
            }

            // ��������� ����� ��������������� - ����� ������������.
            memset(pxModuleContext ->
                   xModuleContextStatic.
                   pucBadAiBufferPointer,
                   1,
                   (pxFunction1234Pointer -> ucIOQuantity));
            // ��������� ����� ��������������� ���� ���������� �������� ������.
            memset(pxModuleContext ->
                   xModuleContextStatic.
                   pucBadDiBufferPointer,
                   1,
                   ((pxFunction1234Pointer -> ucIOQuantity) *
                    ANALOGUE_INPUT_DI_VALUE_QUANTITY));
            // ������ ������ ������ ������������, ������� ��.
            memset(pxModuleContext ->
                   xModuleContextStatic.
                   pucAiValueBufferPointer,
                   0,
                   (pxFunction1234Pointer -> ucIOQuantity) * sizeof(float));
            // ���������� ������ ������ ������ ������������, ������� ��.
            memset(pxModuleContext ->
                   xModuleContextStatic.
                   pucDiValueBufferPointer,
                   0,
                   (pxFunction1234Pointer -> ucIOQuantity) *
                   ANALOGUE_INPUT_DI_VALUE_QUANTITY);
            // �������� � ����� - ��������� ���������� ������������� ������� - ����� ����� ������.
            for (i = 0; i < (pxFunction1234Pointer -> ucIOQuantity); i++)
            {
                pxModuleContext ->
                xModuleContextStatic.
                pucStatAiBufferPointer[i] |=
                    ANALOGUE_INPUT_LINE_BREAK;
            }
            //iBadModuleBuffUpdate();
        }
        else
        {
            // �������� �������� �������� - "���������� ������� ����� � ������� ��� ������".
            (pxModuleContext ->
             xModuleContextDinamic.
             nucBadAnswerCounter)--;
        }
    }

    pthread_mutex_unlock(&xMainDataAccessMutex);
    return 0;
}

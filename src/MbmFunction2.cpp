
#include "PasNewConfig.h"
#include "MbmFunction2.h"
#include "Error.h"
#include "Events.h"

using namespace std;

//-----------------------------------------------------------------------------------------------------
// ���������� ����� ������� � �������� ��������������� ����� ���̻ ������� Modbus - 2.
int iMbmFunction2DataHandler(TModuleContext *pxModuleContext)
{
    int i;
    unsigned char *pucSource;
    unsigned char *pucDestination;
    TMbmFunction1234 *pxFunction1234Pointer;
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
    // ��������� ����� ������ ��������� ���� ������� ������.
    modbus_set_slave(pxMasterRtuCtx,
                     (pxFunction1234Pointer -> ucModbusSlaveAddress));
    // ������ ������ �� ������ ��������� ���� ������� ������ �� ��������� �����.
    i = modbus_read_input_bits(pxMasterRtuCtx,
                               (((unsigned short)(pxFunction1234Pointer -> ucAddressH) << 8) |
                                (unsigned short)(pxFunction1234Pointer -> ucAddressL)),
                               (((unsigned short)(pxFunction1234Pointer -> ucQuantityH) << 8) |
                                (unsigned short)(pxFunction1234Pointer -> ucQuantityL)),
                               (unsigned char*)&aucTempArray[0]);

    pthread_mutex_lock(&xMainDataAccessMutex);

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
               (pxFunction1234Pointer -> ucIOQuantity));
        // �������� �������� ���������� ������� � ������� ������ - ��������� ������� ���������� ��������.
        memcpy(pxModuleContext ->
               xModuleContextStatic.
               pucDiValueBufferPointer,
               (unsigned char*)&aucTempArray[0],
               (unsigned int)(pxFunction1234Pointer -> ucIOQuantity));
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
                   pucBadDiBufferPointer,
                   1,
                   (pxFunction1234Pointer -> ucIOQuantity));
            // ���������� ������ ������ ������ ������������, ������� ��.
            memset(pxModuleContext ->
                   xModuleContextStatic.
                   pucDiValueBufferPointer,
                   0,
                   (pxFunction1234Pointer -> ucIOQuantity));
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

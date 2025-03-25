
#include "PasNewConfig.h"
#include "MbmFunction15.h"
#include "Error.h"
#include "Events.h"

using namespace std;

//-----------------------------------------------------------------------------------------------------
// ���������� ����� ������� � �������� ��������� � ������������ PSS7 ������� Modbus - 15.
void vMbmFunction15DataHandler(TModuleContext *pxModuleContext)
{
    int i;
    unsigned char *pucSource;
    unsigned char *pucDestination;
    unsigned char ucLocalBou;
    TMbmFunction15 *pxFunction15Pointer;
    unsigned char aucTempArray[MBM_FUNCTION_15_SOURCE_BIT_ADDRESS_ARRAY_LENGTH];

    memset(&aucTempArray[0],
           0,
           sizeof(aucTempArray));

    // ������� ��������� �� ��������� ��������������� ����� ���̻ � ���� ������.
    pxFunction15Pointer = (TMbmFunction15*)((pxModuleContext ->
                                            xModuleContextStatic.
                                            pucDataBasePointer) +
                                            (pxModuleContext ->
                                                    xModuleContextStatic.
                                                    ucDataBaseDataOffset));
    // ���������� ������������������� ���������� �������� �� ������� �� ������� ������� �������?
    if (((pxFunction15Pointer -> ucIOQuantity) >= 0) &&
            ((pxFunction15Pointer -> ucIOQuantity) <= MBM_FUNCTION_15_SOURCE_BIT_ADDRESS_ARRAY_LENGTH))
    {
        pthread_mutex_lock(&xMainDataAccessMutex);

        // ���������� ������������������� ���������� �������.
        for (i = 0; i < (pxFunction15Pointer -> ucIOQuantity); i++)
        {
            // ������� ���������� ������ ����������������?
            if ((pxFunction15Pointer ->
                    aucBitSourceAddress[i]) != 0)
            {
                // �������� ����������� ������� ������ SOST_DO_BIT_ARRAY_OFFSET?
                if ((pxFunction15Pointer ->
                        aucBitSourceAddress[i]) >= TUST_DI_VALUE_OR_SOST_DO_SOURCE_OFFSET)
                {
                    // �������� ����������� ������� ������ SOST_DO_BIT_ARRAY_OFFSET.
                    pucSource = pxModuleContext ->
                                xModuleContextStatic.
                                pucSostDoBufferPointer;
                    // ���������� ������ � �������-��������� - 1?
                    if (pucSource[((pxFunction15Pointer ->
                                    aucBitSourceAddress[i]) -
                                   TUST_DI_VALUE_OR_SOST_DO_SOURCE_OFFSET) - 1])
                    {
                        // ���������� ������ � �������-��������� - 1.
                        // �������� �������� ����������� ������� �� ��������� ������� ��� �������� �� Modbus.
                        aucTempArray[i] = 1;
                    }
                    else
                    {
                        // ���������� ������ � �������-��������� - 0.
                        // �������� �������� ����������� ������� �� ��������� ������� ��� �������� �� Modbus.
                        aucTempArray[i] = 0;
                    }
                }
                else
                {
                    // �������� ����������� ������� ������ DI_VALUE_BIT_ARRAY_OFFSET.
                    pucSource = pxModuleContext ->
                                xModuleContextStatic.
                                pucDiValueOriginBufferPointer;
                    // ���������� ������ � �������-��������� - 1?
                    if (pucSource[(pxFunction15Pointer ->
                                   aucBitSourceAddress[i]) - 1])
                    {
                        // ���������� ������ � �������-��������� - 1.
                        // �������� �������� ����������� ������� �� ��������� ������� ��� �������� �� Modbus.
                        aucTempArray[i] = 1;
                    }
                    else
                    {
                        // ���������� ������ � �������-��������� - 0.
                        // �������� �������� ����������� ������� �� ��������� ������� ��� �������� �� Modbus.
                        aucTempArray[i] = 0;
                    }
                }
            }
        }

        pthread_mutex_unlock(&xMainDataAccessMutex);

        // ��������� ����� ������ ��������� ���� ������� ������.
        modbus_set_slave(pxMasterRtuCtx,
                         (pxFunction15Pointer -> ucModbusSlaveAddress));
        // �������� ������ ������ ��������� ���� ������� ������ �� ���������� ������.
        modbus_write_bits(pxMasterRtuCtx,
                          (((int)(pxFunction15Pointer -> ucAddressH) << 8) |
                           (int)(pxFunction15Pointer -> ucAddressL)),
                          (pxFunction15Pointer -> ucIOQuantity),
                          &aucTempArray[0]);
    }

    // ������������ c �������� ������ ���������?
    if ((pxFunction15Pointer -> ucTust) & TUST_KVIT_PC_ENABLE_BIT_MASK)
    {
        // ���� ������� - "������������" c �������� ������?
        if (pxModuleContext ->
                xModuleContextDinamic.
                fbGlobalModbusKvit)
        {
            // ������� �������.
            pxModuleContext ->
            xModuleContextDinamic.
            fbGlobalModbusKvit = 0;
            // ��������� ����� ������ ��������� ���� ������� ������.
            modbus_set_slave(pxMasterRtuCtx,
                             (pxFunction15Pointer -> ucModbusSlaveAddress));
            // �������� ������� - "������������" ������ ��������� ���� ������� ������.
            modbus_write_bit(pxMasterRtuCtx, MBM_FUNCTION_15_KVIT_BIT_FUNCTION_5_ADDRESS, 1);
        }
    }
    // ����� c �������� ������ ��������?
    if ((pxFunction15Pointer -> ucTust) & TUST_RESET_PC_ENABLE_BIT_MASK)
    {
        // ���� ������� - "�����" c �������� ������?
        if (pxModuleContext ->
                xModuleContextDinamic.
                fbGlobalModbusReset)
        {
            // ������� �������.
            pxModuleContext ->
            xModuleContextDinamic.
            fbGlobalModbusReset = 0;
            // ��������� ����� ������ ��������� ���� ������� ������.
            modbus_set_slave(pxMasterRtuCtx,
                             (pxFunction15Pointer -> ucModbusSlaveAddress));
            // �������� ������� - "�����" ������ ��������� ���� ������� ������.
            modbus_write_bit(pxMasterRtuCtx, MBM_FUNCTION_15_RESET_BIT_FUNCTION_5_ADDRESS, 1);
        }
    }
}

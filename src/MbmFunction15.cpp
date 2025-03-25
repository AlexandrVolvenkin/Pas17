
#include "PasNewConfig.h"
#include "MbmFunction15.h"
#include "Error.h"
#include "Events.h"

using namespace std;

//-----------------------------------------------------------------------------------------------------
// производит обмен данными с модулями индикации и сигнализации PSS7 функция Modbus - 15.
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

    // получим указатель на описатель функционального блока «МВМ» в базе данных.
    pxFunction15Pointer = (TMbmFunction15*)((pxModuleContext ->
                                            xModuleContextStatic.
                                            pucDataBasePointer) +
                                            (pxModuleContext ->
                                                    xModuleContextStatic.
                                                    ucDataBaseDataOffset));
    // количество запрограммированных дискретных сигналов не выходит за границы размера массива?
    if (((pxFunction15Pointer -> ucIOQuantity) >= 0) &&
            ((pxFunction15Pointer -> ucIOQuantity) <= MBM_FUNCTION_15_SOURCE_BIT_ADDRESS_ARRAY_LENGTH))
    {
        pthread_mutex_lock(&xMainDataAccessMutex);

        // обработаем запрограммированные дискретные сигналы.
        for (i = 0; i < (pxFunction15Pointer -> ucIOQuantity); i++)
        {
            // текущий дискретный сигнал запрограммирован?
            if ((pxFunction15Pointer ->
                    aucBitSourceAddress[i]) != 0)
            {
                // источник дискретного сигнала массив SOST_DO_BIT_ARRAY_OFFSET?
                if ((pxFunction15Pointer ->
                        aucBitSourceAddress[i]) >= TUST_DI_VALUE_OR_SOST_DO_SOURCE_OFFSET)
                {
                    // источник дискретного сигнала массив SOST_DO_BIT_ARRAY_OFFSET.
                    pucSource = pxModuleContext ->
                                xModuleContextStatic.
                                pucSostDoBufferPointer;
                    // дискретный сигнал в массиве-источнике - 1?
                    if (pucSource[((pxFunction15Pointer ->
                                    aucBitSourceAddress[i]) -
                                   TUST_DI_VALUE_OR_SOST_DO_SOURCE_OFFSET) - 1])
                    {
                        // дискретный сигнал в массиве-источнике - 1.
                        // сохраним значение дискретного сигнала во временном массиве для передачи по Modbus.
                        aucTempArray[i] = 1;
                    }
                    else
                    {
                        // дискретный сигнал в массиве-источнике - 0.
                        // сохраним значение дискретного сигнала во временном массиве для передачи по Modbus.
                        aucTempArray[i] = 0;
                    }
                }
                else
                {
                    // источник дискретного сигнала массив DI_VALUE_BIT_ARRAY_OFFSET.
                    pucSource = pxModuleContext ->
                                xModuleContextStatic.
                                pucDiValueOriginBufferPointer;
                    // дискретный сигнал в массиве-источнике - 1?
                    if (pucSource[(pxFunction15Pointer ->
                                   aucBitSourceAddress[i]) - 1])
                    {
                        // дискретный сигнал в массиве-источнике - 1.
                        // сохраним значение дискретного сигнала во временном массиве для передачи по Modbus.
                        aucTempArray[i] = 1;
                    }
                    else
                    {
                        // дискретный сигнал в массиве-источнике - 0.
                        // сохраним значение дискретного сигнала во временном массиве для передачи по Modbus.
                        aucTempArray[i] = 0;
                    }
                }
            }
        }

        pthread_mutex_unlock(&xMainDataAccessMutex);

        // установим адрес модуля локальной сети нижнего уровня.
        modbus_set_slave(pxMasterRtuCtx,
                         (pxFunction15Pointer -> ucModbusSlaveAddress));
        // отправим данные модулю локальной сети нижнего уровня из временного буфера.
        modbus_write_bits(pxMasterRtuCtx,
                          (((int)(pxFunction15Pointer -> ucAddressH) << 8) |
                           (int)(pxFunction15Pointer -> ucAddressL)),
                          (pxFunction15Pointer -> ucIOQuantity),
                          &aucTempArray[0]);
    }

    // квитирование c верхнего уровня разрешено?
    if ((pxFunction15Pointer -> ucTust) & TUST_KVIT_PC_ENABLE_BIT_MASK)
    {
        // есть команда - "квитирование" c верхнего уровня?
        if (pxModuleContext ->
                xModuleContextDinamic.
                fbGlobalModbusKvit)
        {
            // команду приняли.
            pxModuleContext ->
            xModuleContextDinamic.
            fbGlobalModbusKvit = 0;
            // установим адрес модуля локальной сети нижнего уровня.
            modbus_set_slave(pxMasterRtuCtx,
                             (pxFunction15Pointer -> ucModbusSlaveAddress));
            // отправим команду - "квитирование" модулю локальной сети нижнего уровня.
            modbus_write_bit(pxMasterRtuCtx, MBM_FUNCTION_15_KVIT_BIT_FUNCTION_5_ADDRESS, 1);
        }
    }
    // сброс c верхнего уровня разрешён?
    if ((pxFunction15Pointer -> ucTust) & TUST_RESET_PC_ENABLE_BIT_MASK)
    {
        // есть команда - "сброс" c верхнего уровня?
        if (pxModuleContext ->
                xModuleContextDinamic.
                fbGlobalModbusReset)
        {
            // команду приняли.
            pxModuleContext ->
            xModuleContextDinamic.
            fbGlobalModbusReset = 0;
            // установим адрес модуля локальной сети нижнего уровня.
            modbus_set_slave(pxMasterRtuCtx,
                             (pxFunction15Pointer -> ucModbusSlaveAddress));
            // отправим команду - "сброс" модулю локальной сети нижнего уровня.
            modbus_write_bit(pxMasterRtuCtx, MBM_FUNCTION_15_RESET_BIT_FUNCTION_5_ADDRESS, 1);
        }
    }
}

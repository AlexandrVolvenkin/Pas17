
#include "PasNewConfig.h"
#include "MbmFunction2.h"
#include "Error.h"
#include "Events.h"

using namespace std;

//-----------------------------------------------------------------------------------------------------
// производит обмен данными с модулями функционального блока «МВМ» функция Modbus - 2.
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
    // получим указатель на описатель функционального блока «МВМ» в базе данных.
    pxFunction1234Pointer = (TMbmFunction1234*)((pxModuleContext ->
                            xModuleContextStatic.
                            pucDataBasePointer) +
                            (pxModuleContext ->
                             xModuleContextStatic.
                             ucDataBaseDataOffset));
    // установим адрес модуля локальной сети нижнего уровня.
    modbus_set_slave(pxMasterRtuCtx,
                     (pxFunction1234Pointer -> ucModbusSlaveAddress));
    // примем данные от модуля локальной сети нижнего уровня во временный буфер.
    i = modbus_read_input_bits(pxMasterRtuCtx,
                               (((unsigned short)(pxFunction1234Pointer -> ucAddressH) << 8) |
                                (unsigned short)(pxFunction1234Pointer -> ucAddressL)),
                               (((unsigned short)(pxFunction1234Pointer -> ucQuantityH) << 8) |
                                (unsigned short)(pxFunction1234Pointer -> ucQuantityL)),
                               (unsigned char*)&aucTempArray[0]);

    pthread_mutex_lock(&xMainDataAccessMutex);

    if (i != -1)
    {
        // модуль исправен.
        pxModuleContext ->
        xModuleContextDinamic.
        nucBadAnswerCounter = BAD_MODULE_CYCLE_COUNT_DEFAULT;
        // сбросим флаг отказа модуля.
        *(pxModuleContext ->
          xModuleContextStatic.
          pucModuleBadStateBufferPointer) = BAD_MODULE_RESPONDED_OK;
        //iBadModuleBuffUpdate();

        // неактивное состояние события ещё не зарегистрировано?
        if(xCInternalModuleErrorEvent.EventOffIsNotRegistered(
                    pxModuleContext ->
                    xModuleContextStatic.
                    ucModuleContextIndex,
                    MTVI5_BAD_ANSWER_ERROR_OFFSET))
        {
            // зарегистрируем неактивное состояние события.
            // норма обмена данными.
            CEvents::EventRegistration(
                pxModuleContext ->
                xModuleContextStatic.
                ucModuleAddress,
                (CEvents::HANDLED_EVENTS_EXTERNAL_MODULES_BAD_TYPE |
                 CEvents::HANDLED_EVENTS_IS_ARCHIVE),
                0,
                "Норма SL");
        }

        //iBadModuleBuffUpdate();
        // данные модуля достоверны.
        memset(pxModuleContext ->
               xModuleContextStatic.
               pucBadDiBufferPointer,
               0,
               (pxFunction1234Pointer -> ucIOQuantity));
        // поместим принятые дискретные сигналы в рабочий массив - состояние входных дискретных сигналов.
        memcpy(pxModuleContext ->
               xModuleContextStatic.
               pucDiValueBufferPointer,
               (unsigned char*)&aucTempArray[0],
               (unsigned int)(pxFunction1234Pointer -> ucIOQuantity));
    }
    else
    {
        // модуль признан неисправным?
        if ((pxModuleContext ->
                xModuleContextDinamic.
                nucBadAnswerCounter) == 0)
        {
            // получим код ошибки;
            (pxModuleContext ->
             xModuleContextDinamic.
             ucErrorCode) = CError::ModbusErrorToPlcError(errno);

            // модуль признан неисправным.
            *(pxModuleContext ->
              xModuleContextStatic.
              pucModuleBadStateBufferPointer) = BAD_MODULE_NOT_RESPONDED;

            // активное состояние события ещё не зарегистрировано?
            if(xCInternalModuleErrorEvent.EventOnIsNotRegistered(
                        pxModuleContext ->
                        xModuleContextStatic.
                        ucModuleContextIndex,
                        MTVI5_BAD_ANSWER_ERROR_OFFSET))
            {
                // зарегистрируем активное состояние события.
                // ошибка обмена данными.
                CEvents::EventRegistration(
                    pxModuleContext ->
                    xModuleContextStatic.
                    ucModuleAddress,
                    (CEvents::HANDLED_EVENTS_EXTERNAL_MODULES_BAD_TYPE |
//                     CEvents::HANDLED_EVENTS_IS_POPUP |
//                     CEvents::HANDLED_EVENTS_IS_SOUND |
                     CEvents::HANDLED_EVENTS_IS_ARCHIVE),
                    CError::ModbusErrorToPlcError(errno),
                    "Ошибка SL");
            }

            // установим флаги недостоверности - входы недостоверны.
            memset(pxModuleContext ->
                   xModuleContextStatic.
                   pucBadDiBufferPointer,
                   1,
                   (pxFunction1234Pointer -> ucIOQuantity));
            // дискретные данные входов модуля недостоверны, обнулим их.
            memset(pxModuleContext ->
                   xModuleContextStatic.
                   pucDiValueBufferPointer,
                   0,
                   (pxFunction1234Pointer -> ucIOQuantity));
            //iBadModuleBuffUpdate();
        }
        else
        {
            // уменьшим значение счётчика - "количество сеансов связи с модулем без ответа".
            (pxModuleContext ->
             xModuleContextDinamic.
             nucBadAnswerCounter)--;
        }
    }

    pthread_mutex_unlock(&xMainDataAccessMutex);

    return 0;
}

#ifndef CRESOURCES_H
#define CRESOURCES_H
//-------------------------------------------------------------------------------------------
//  Source      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-------------------------------------------------------------------------------------------

#include <stdint.h>

#include "Dfa.h"

class CDeviceControl;
class CDataStore;

//-------------------------------------------------------------------------------------------
class CResourcesInterface : public CDfa
{
public:

};

//-------------------------------------------------------------------------------------------










//-------------------------------------------------------------------------------------------
class CResources : public CResourcesInterface
{
public:
    CResources();
    virtual ~CResources();

    void SetCoils(uint8_t* puiPointer);
    uint8_t* GetCoils(void);
    void SetCoilsNumber(uint16_t uiData);
    uint16_t GetCoilsNumber(void);

    void SetDiscreteInputs(uint8_t* puiPointer);
    uint8_t* GetDiscreteInputs(void);
    void SetDiscreteInputsNumber(uint16_t uiData);
    uint16_t GetDiscreteInputsNumber(void);

    void SetHoldingRegisters(uint16_t* puiPointer);
    uint16_t* GetHoldingRegisters(void);
    void SetHoldingRegistersNumber(uint16_t uiData);
    uint16_t GetHoldingRegistersNumber(void);

    void SetInputRegisters(uint16_t* puiPointer);
    uint16_t* GetInputRegisters(void);
    void SetInputRegistersNumber(uint16_t uiData);
    uint16_t GetInputRegistersNumber(void);

//protected:
//
//private:
    uint8_t m_uiAddress;
    uint8_t *m_puiRxBuffer;
    uint8_t *m_puiTxBuffer;
    uint8_t *m_puiErrorCode;
//    uint8_t *m_puiDiscreteOutputState;
//    uint8_t m_uiUsedDiscreteOutputState;
//    TDiscreteOutputControl *m_pxDiscreteOutputControl;
//    uint8_t m_uiUsedDiscreteOutputs;
//    TOutputData *m_pxDiscreteOutputDataBase;
////    uint8_t *m_puiExternalReceiptAddress;
////    uint8_t *m_puiExternalResetAddress;
//    uint8_t m_uiUsedDiscreteOutputDataBase;
//    uint8_t* m_puiErrorAlarmDataArray;
//    uint8_t m_uiUsedErrorAlarmDataArray;

    uint8_t *m_puiCoils;

    uint8_t *m_puiDiscreteInputs;
    uint8_t m_uiUsedDiscreteInputs;

    uint16_t *m_puiHoldingRegisters;
    uint16_t *m_puiInputRegisters;
    uint16_t m_uiCoilsNumber;
    uint16_t m_uiDiscreteInputsNumber;
    uint16_t m_uiHoldingRegistersNumber;
    uint16_t m_uiInputRegistersNumber;

    CDeviceControl* m_pxDeviceControl;
    CDataStore* m_pxDataStore;
};

//-------------------------------------------------------------------------------------------
#endif // CRESOURCES_H

//-------------------------------------------------------------------------------
//  Source      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-------------------------------------------------------------------------------
#ifndef CMAINPRODUCTIONCYCLE_H
#define CMAINPRODUCTIONCYCLE_H

#include <iostream>
#include <string.h>

#include "Platform.h"
#include "Resources.h"
#include "DeviceControl.h"
#include "Task.h"
#include "Production.h"
//#include "ModbusTcp.h"
#include "ModbusSlave.h"
#include "ModbusSlaveLinkLayer.h"
#include "ModbusTcpSlaveLinkLayer.h"
#include "ModbusRtuSlaveLinkLayer.h"

class CLedBlinker;

//-------------------------------------------------------------------------------
class CMainProductionCycle : public CTask//,public CDfa
{
public:

    enum
    {
        MAIN_CYCLE_SHOW_STATISTICS_PERIOD_TIME = 1000,
    };

    enum
    {
        START = 0,
        READY,
        IDDLE,
        STOP,
        MAIN_CYCLE_MODBUS_SLAVE,
        LED_BLINK_ON,
        LED_BLINK_OFF,

    };

    CMainProductionCycle();
    virtual ~CMainProductionCycle();

    std::list<CTaskInterface*>* GetCommonTasksListPointer(void)
    {
        return &m_lpxCommonTasksList;
    };

    std::list<CTaskInterface*>* GetCurrentlyRunningTasksListPointer(void)
    {
        return &m_lpxCurrentlyRunningTasksList;
    };

    uint8_t Init(void);
    uint8_t Fsm(void);

private:
    std::list<CTaskInterface*> m_lpxCommonTasksList;
    std::list<CTaskInterface*>::iterator m_xCommonTasksListIterator;
    std::list<CTaskInterface*> m_lpxCurrentlyRunningTasksList;
    std::list<CTaskInterface*>::iterator m_xCurrentlyRunningTasksListIterator;

    CResources m_xResources;
    CDeviceControl m_xDeviceControl;

    CLedBlinker* m_pxLedBlinker;
    // создадим указатель на объект
    // "производственная площадка задачи ожидания событий"
    CProductionInterface* m_pxFileDescriptorEventsWaitingProduction;
    CModbusTcpSlaveTopLevelProduction* m_pxModusTcpSlaveTopLevelProduction;
    CModbusTcpSlaveLinkLayer* m_pxModbusTcpSlaveLinkLayerUpperLevel;
    CModbusSlave* m_pxModbusTcpSlaveUpperLevel;

    CModbusRtuSlaveTopLevelProduction* m_pxModusRtuSlaveTopLevelProduction;
    CModbusRtuSlaveLinkLayer* m_pxModbusRtuSlaveLinkLayerUpperLevel;
    CModbusSlave* m_pxModbusRtuSlaveUpperLevel;

    uint8_t *m_puiCoils;
    uint8_t *m_puiDiscreteInputs;
    uint16_t *m_puiHoldingRegisters;
    uint16_t *m_puiInputRegisters;
    uint16_t m_uiCoilsNumber;
    uint16_t m_uiDiscreteInputsNumber;
    uint16_t m_uiHoldingRegistersNumber;
    uint16_t m_uiInputRegistersNumber;
};
//-------------------------------------------------------------------------------





//-------------------------------------------------------------------------------
class CLedBlinker : public CTask//,public CDfa
{
public:

    enum
    {
        BLINK_PERIOD_TIME = 1000,
    };

    enum
    {
        START = 0,
        READY,
        IDDLE,
        STOP,
        LED_ON,
        LED_ON_PERIOD_END_WAITING,
        LED_OFF,
        LED_OFF_PERIOD_END_WAITING,
    };

    CLedBlinker();
    virtual ~CLedBlinker();

    uint8_t Init(void);
    uint8_t Fsm(void);

private:

};

//-------------------------------------------------------------------------------
#endif // CMAINPRODUCTIONCYCLE_H

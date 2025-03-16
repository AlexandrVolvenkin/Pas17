#ifndef CLINK_H
#define CLINK_H
//-------------------------------------------------------------------------------
//  Source      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-------------------------------------------------------------------------------

#include <stdint.h>
#include <list>
#include <memory>

//#include "Timer.h"
//#include "Dfa.h"
class CTaskInterface;
class CTimer;
class CDfaLite;
class CDfa;
class CResourcesInterface;
class CResources;
class CDataContainerInterface;
class CMessageBoxInterface;
class CMessageBoxGeneral;

//-------------------------------------------------------------------------------
class CLinkInterface : public CTask
{
public:
//        CLinkInterface();
//        virtual ~CLinkInterface();

    virtual size_t GetObjectLength(void) {};

    virtual void SetTaskPerformerName(std::string sName) {};
    virtual void SetTaskPerformer(CTaskInterface* pxTaskPerformer) {};
    virtual CTaskInterface* GetTaskPerformerPointer(void) {};

    virtual void SetDataContainer(CDataContainerInterface* pxDataContainer) {};
    virtual CDataContainerInterface* GetDataContainerPointer(void) {};


protected:

private:
};

//-------------------------------------------------------------------------------





//-------------------------------------------------------------------------------
class CLink// : public CLinkInterface
{
public:
//    CLink();
//    virtual ~CLink();
//
//    size_t GetObjectLength(void);

    void SetTaskPerformerName(std::string sName);
    void SetTaskPerformer(CTaskInterface* pxTaskPerformer);
    CTaskInterface* GetTaskPerformerPointer(void);

    void SetDataContainer(CDataContainerInterface* pxDataContainer);
    CDataContainerInterface* GetDataContainerPointer(void);

//    uint8_t Fsm(void);

protected:

private:
    std::string m_sTaskPerformerName;
    CTaskInterface* m_pxTaskPerformer;
    CDataContainerInterface* m_pxDataContainer;
};

//-------------------------------------------------------------------------------
#endif // CLINK_H

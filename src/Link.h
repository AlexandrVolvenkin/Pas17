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
class CLink : public CLinkInterface
{
public:
    CLink();
    virtual ~CLink();

    void SetTaskPerformerName(std::string sName);
    void SetTaskPerformer(CTaskInterface* pxTaskPerformer);
    CTaskInterface* GetTaskPerformerPointer(void);

    void SetDataContainer(CDataContainerInterface* pxDataContainer);
    CDataContainerInterface* GetDataContainerPointer(void);

    uint8_t Fsm(void);

protected:

private:
    std::string m_sTaskPerformerName;
    CTaskInterface* m_pxTaskPerformer;
    CDataContainerInterface* m_pxDataContainer;
};

//-------------------------------------------------------------------------------
#endif // CLINK_H




//// CLink.h
//#ifndef __CLINK_H__
//#define __CLINK_H__
//
//#include <string>
//
//class CTaskInterface {
//    // Define the interface for task performers
//};
//
//class CDataContainerInterface {
//    // Define the interface for data container
//};
//
//class CLink {
//public:
//    // Constructors and destructor
//    CLink(const std::string& performerName, CTaskInterface* performer, CDataContainerInterface* dataContainer);
//    ~CLink();
//
//    // Setters and getters for the fields
//    void setTaskPerformerName(const std::string& performerName);
//    std::string getTaskPerformerName() const;
//
//    void setTaskPerformer(CTaskInterface* performer);
//    CTaskInterface* getTaskPerformer() const;
//
//    void setDataContainer(CDataContainerInterface* dataContainer);
//    CDataContainerInterface* getDataContainer() const;
//
//private:
//    std::string m_sTaskPerformerName;
//    CTaskInterface* m_pxTaskPerformer;
//    CDataContainerInterface* m_pxDataContainer;
//};
//
//#endif // __CLINK_H__
//

//-------------------------------------------------------------------------------
//  Source      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-------------------------------------------------------------------------------
#ifndef CMESSAGEBOX_H
#define CMESSAGEBOX_H

#include <stdint.h>
#include <memory>

class CDataContainerInterface;

//-------------------------------------------------------------------------------
class CMessageBoxInterface
{
public:
    virtual void SetDataContainer(CDataContainerInterface* pxDataContainer) {};
    virtual CDataContainerInterface* GetDataContainerPointer(void) {};

protected:

private:
};

//-------------------------------------------------------------------------------





//-------------------------------------------------------------------------------
class CMessageBoxGeneral : public CMessageBoxInterface
{
public:
    CMessageBoxGeneral();
    virtual ~CMessageBoxGeneral();

    void SetDataContainer(CDataContainerInterface* pxDataContainer);
    CDataContainerInterface* GetDataContainerPointer(void);

public:
    CDataContainerInterface* m_pxDataContainer;
};

//-------------------------------------------------------------------------------
#endif // CMESSAGEBOX_H

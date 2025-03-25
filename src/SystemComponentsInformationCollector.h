//-----------------------------------------------------------------------------------------------------
//  Source      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-----------------------------------------------------------------------------------------------------
#ifndef CSYSTEMCOMPONENTSINFORMATIONCOLLECTOR_H
#define CSYSTEMCOMPONENTSINFORMATIONCOLLECTOR_H

#include <stdint.h>

//-----------------------------------------------------------------------------------------------------
class CSystemComponentsInformationCollectorInterface
{
    virtual bool IsReadyToStartWork(void){};
};

//-----------------------------------------------------------------------------------------------------








//-----------------------------------------------------------------------------------------------------
class CSystemComponentsInformationCollector : public CSystemComponentsInformationCollectorInterface
{
public:
    CSystemComponentsInformationCollector();
    virtual ~CSystemComponentsInformationCollector();
    bool IsReadyToStartWork(void);

protected:
private:
};

//-----------------------------------------------------------------------------------------------------
#endif // CSYSTEMCOMPONENTSINFORMATIONCOLLECTOR_H

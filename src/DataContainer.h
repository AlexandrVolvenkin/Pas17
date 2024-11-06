//-------------------------------------------------------------------------------
//  Source      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-------------------------------------------------------------------------------
#ifndef CDATACONTAINER_H
#define CDATACONTAINER_H

#include <stdint.h>
#include <iostream>

//-------------------------------------------------------------------------------
class CDataContainerInterface
{
public:

protected:

private:
};

//-------------------------------------------------------------------------------





//-------------------------------------------------------------------------------
class CDataContainerDataBase
{
public:
    CDataContainerDataBase();
    virtual ~CDataContainerDataBase();

protected:

private:
    uint16_t m_uiDataIndex;
    uint8_t* m_uiDataPointer;
    uint32_t m_uiDataOffset;
    uint32_t m_uiDataLength;
};

//-------------------------------------------------------------------------------
#endif // CDATACONTAINER_H

#ifndef CSEMAPHORE_H
#define CSEMAPHORE_H
//-------------------------------------------------------------------------------
//  Sourse      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
class CSemaphore
{
public:
    CSemaphore();
    CSemaphore(const char* key, int value);
    CSemaphore(key_t keyVal, int value);
    virtual ~CSemaphore();

    bool Acquire();
    bool Release();

private:
    int semId;
};

//-------------------------------------------------------------------------------
#endif // CSEMAPHORE_H

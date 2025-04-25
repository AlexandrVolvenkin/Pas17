//-------------------------------------------------------------------------------
//  Source      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-------------------------------------------------------------------------------
#ifndef CDATASTORECHECK_H
#define CDATASTORECHECK_H

#include <stdint.h>
#include <iostream>
#include <fstream>

class Timer;
class CTask;
class CResources;
class CStorageDeviceInterface;
class CStorageDeviceFileSystem;
class CDataStore;

using namespace std;

//-------------------------------------------------------------------------------
class CDataStoreCheck : public CDataStore
{
public:

    enum
    {
        DATA_STORE_CHECK_START = NEXT_STEP,
        TEMPORARY_SERVICE_SECTION_DATA_CHECK,
        TEMPORARY_SERVICE_SECTION_LINKED_BLOCKS_CHECK,

        CORRUPTED_BLOCK_RECOVERY_WRITE_END_WAITING,
        SERVICE_SECTION_DATA_WRITE_START,
        SERVICE_SECTION_DATA_WRITE_END_WAITING,

        SERVICE_SECTION_DATA_CHECK,
        SERVICE_SECTION_LINKED_BLOCKS_CHECK,

        DATA_STORE_NEW_VERSION_ACCEPTED,
        DATA_STORE_OLD_VERSION_ACCEPTED,

        DATA_STORE_CHECK_OK,
        DATA_STORE_CHECK_ERROR,
        DATA_STORE_CHECK_REPEAT,
    };

    enum
    {
        //  оличество попыток восстановлени€ хранилища.
        RECOVERY_ATTEMPTS_NUMBER = 3,
    };

    CDataStoreCheck();
    virtual ~CDataStoreCheck();

    uint8_t Init(void);
    size_t GetObjectLength(void);

    uint8_t Check(void);
    uint8_t Fsm(void);

protected:

private:
    uint8_t m_uiRecoveryAttemptCounter;
};

#endif // CDATASTORECHECK_H

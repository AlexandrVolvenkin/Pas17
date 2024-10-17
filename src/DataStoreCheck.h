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

//#include "Dfa.h"
#include "Timer.h"
//#include "StorageDevice.h"
//#include "DataStore.h"

class CTask;
class CResources;
class CStorageDeviceInterface;
class CStorageDeviceFileSystem;
class CDataStore;

using namespace std;

//-------------------------------------------------------------------------------
class CDataStoreCheck : public CTask//, public CDfa
{
public:

    enum
    {
        START = 0,
        INIT,
        READY,
        STOP,
        IDDLE,

        DATA_STORE_CHECK_START,
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
        // Количество попыток восстановления хранилища.
        RECOVERY_ATTEMPTS_NUMBER = 3,
    };

    CDataStoreCheck();
    CDataStoreCheck(CDataStore* pxDataStore);
    virtual ~CDataStoreCheck();

    void SetDataStoreName(std::string sName);
    void SetDataStore(CDataStore* pxDataStore);
    uint8_t Check(void);
    uint8_t Fsm(void);

protected:

private:
    std::string m_sDataStoreName;
    CDataStore* m_pxDataStore;
    uint8_t m_uiRecoveryAttemptCounter;
};

#endif // CDATASTORECHECK_H

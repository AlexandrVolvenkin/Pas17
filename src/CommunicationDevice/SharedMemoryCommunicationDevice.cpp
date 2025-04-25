
//-------------------------------------------------------------------------------
//  Sourse      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-------------------------------------------------------------------------------
#include <typeinfo>

#include "Task.h"
#include "Platform.h"
#include "Resources.h"
#include "CommunicationDevice.h"
#include "SharedMemoryCommunicationDevice.h"

using namespace std;

//-------------------------------------------------------------------------------
CSharedMemoryCommunicationDevice::CSharedMemoryCommunicationDevice()
{
    std::cout << "CSharedMemoryCommunicationDevice constructor"  << std::endl;
//    // получим имя класса.
//    sprintf(GetTaskNamePointer(),
//            "%s",
//            typeid(*this).name());

}
//-------------------------------------------------------------------------------
CSharedMemoryCommunicationDevice::~CSharedMemoryCommunicationDevice()
{
    Close();
}

//-------------------------------------------------------------------------------
void CSharedMemoryCommunicationDevice::Init(void)
{
    std::cout << "CSharedMemoryCommunicationDevice::Init 1"  << std::endl;

    SetFsmState(READY);
}

//-------------------------------------------------------------------------------
void CSharedMemoryCommunicationDevice::SetDeviceName(const char* pccDeviceName)
{
    m_pccDeviceName = pccDeviceName;
}

//-------------------------------------------------------------------------------
const char* CSharedMemoryCommunicationDevice::GetDeviceName(void)
{
    return m_pccDeviceName;
}

//-------------------------------------------------------------------------------
int8_t CSharedMemoryCommunicationDevice::Open(void)
{
    std::cout << "CSharedMemoryCommunicationDevice::Open 1"  << std::endl;

//-------------------------------------------------------------------------------
    int fd;

    fd = shm_open("SharedMemoryServerToClientBuffer",
                  O_CREAT | O_RDWR,
                  S_IRWXU | S_IRWXG);
    if(fd < 0)
    {
        printf("%s\n", strerror(errno));
    }
    printf("Shared Memory Open: %s %d\n","SharedMemoryServerToClientBuffer", fd);
    ftruncate(fd, MODBUS_SM_MAX_ADU_LENGTH);
    m_puiServerToClientBuffer = (uint8_t*)mmap(0,
                                MODBUS_SM_MAX_ADU_LENGTH,
                                PROT_READ | PROT_WRITE, MAP_SHARED,
                                fd,
                                0);
    close(fd);

//-------------------------------------------------------------------------------
    fd = shm_open("SharedMemoryClientToServerBuffer",
                  O_CREAT | O_RDWR,
                  S_IRWXU | S_IRWXG);
    if(fd < 0)
    {
        printf("%s\n", strerror(errno));
    }
    printf("Shared Memory Open: %s %d\n","SharedMemoryClientToServerBuffer", fd);
    ftruncate(fd, MODBUS_SM_MAX_ADU_LENGTH);
    m_puiClientToServerBuffer = (uint8_t*)mmap(0,
                                MODBUS_SM_MAX_ADU_LENGTH,
                                PROT_READ | PROT_WRITE, MAP_SHARED,
                                fd,
                                0);
    close(fd);
}

//-------------------------------------------------------------------------------
int8_t CSharedMemoryCommunicationDevice::Close(void)
{
    std::cout << "CSharedMemoryCommunicationDevice::Close 1"  << std::endl;
    munmap(m_puiServerToClientBuffer,
           MODBUS_SM_MAX_ADU_LENGTH);
    munmap(m_puiClientToServerBuffer,
           MODBUS_SM_MAX_ADU_LENGTH);
}

//-------------------------------------------------------------------------------
int16_t CSharedMemoryCommunicationDevice::Read(uint8_t *puiDestination, uint16_t uiLength)
{
    std::cout << "CSharedMemoryCommunicationDevice::Read 1"  << std::endl;
}

//-------------------------------------------------------------------------------
int16_t CSharedMemoryCommunicationDevice::ReceiveStart(uint8_t *puiDestination,
        uint16_t uiLength,
        uint32_t uiReceiveTimeout)
{
//    std::cout << "CSharedMemoryCommunicationDevice::ReceiveStart 1"  << std::endl;

    uint16_t uiLengthLocal = m_puiClientToServerBuffer[0];

    if (uiLengthLocal)
    {
        cout << "CSharedMemoryCommunicationDevice::ReceiveStart 2" << endl;


        {
            cout << "CSharedMemoryCommunicationDevice::ReceiveStart m_puiClientToServerBuffer" << endl;
            unsigned char *pucSourceTemp;
            pucSourceTemp = (unsigned char*)m_puiClientToServerBuffer;
            for(int i=0; i<32; )
            {
                for(int j=0; j<8; j++)
                {
                    cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
                }
                cout << endl;
                i += 8;
            }
        }

        m_puiClientToServerBuffer[0] = 0;

        memcpy(puiDestination,
               &m_puiClientToServerBuffer[1],
               uiLengthLocal);

        return uiLengthLocal;
    }
    else
    {
//        cout << "CSharedMemoryCommunicationDevice::ReceiveStart 3" << endl;
        return 0;
    }
}

//-------------------------------------------------------------------------------
int16_t CSharedMemoryCommunicationDevice::ReceiveContinue(uint8_t *puiDestination,
        uint16_t uiLength,
        uint32_t uiReceiveTimeout)
{
//    std::cout << "CSharedMemoryCommunicationDevice::ReceiveContinue 1"  << std::endl;

    uint16_t uiLengthLocal = m_puiClientToServerBuffer[0];

    if (uiLengthLocal)
    {
        cout << "CSharedMemoryCommunicationDevice::ReceiveContinue 2" << endl;
        {
            cout << "CSharedMemoryCommunicationDevice::ReceiveContinue m_puiClientToServerBuffer" << endl;
            unsigned char *pucSourceTemp;
            pucSourceTemp = (unsigned char*)m_puiClientToServerBuffer;
            for(int i=0; i<32; )
            {
                for(int j=0; j<8; j++)
                {
                    cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
                }
                cout << endl;
                i += 8;
            }
        }

        m_puiClientToServerBuffer[0] = 0;

        memcpy(puiDestination,
               &m_puiClientToServerBuffer[1],
               uiLengthLocal);

        return uiLengthLocal;
    }
    else
    {
//        cout << "CSharedMemoryCommunicationDevice::ReceiveContinue 3" << endl;
        return 0;
    }
}

//-------------------------------------------------------------------------------
int16_t CSharedMemoryCommunicationDevice::Write(uint8_t *puiSource, uint16_t uiLength)
{
    std::cout << "CSharedMemoryCommunicationDevice::Write"  << std::endl;

    memcpy(&m_puiServerToClientBuffer[1],
           puiSource,
           uiLength);

    m_puiServerToClientBuffer[0]  = uiLength;

    {
        cout << "CSharedMemoryCommunicationDevice::ReceiveContinue m_puiServerToClientBuffer" << endl;
        unsigned char *pucSourceTemp;
        pucSourceTemp = (unsigned char*)m_puiServerToClientBuffer;
        for(int i=0; i<32; )
        {
            for(int j=0; j<8; j++)
            {
                cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
            }
            cout << endl;
            i += 8;
        }
    }

    return uiLength;
}

//-------------------------------------------------------------------------------

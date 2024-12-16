
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
    // получим имя класса.
    sprintf(GetTaskNamePointer(),
            "%s",
            typeid(*this).name());

}
//-------------------------------------------------------------------------------
CSharedMemoryCommunicationDevice::~CSharedMemoryCommunicationDevice()
{

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
//    cout << "CSharedMemoryCommunicationDevice::Open m_pccDeviceName " << m_pccDeviceName << endl;

//-------------------------------------------------------------------------------
    int fd;

    fd = shm_open("SharedMemoryTxBuffer",
                  O_CREAT | O_RDWR,
                  S_IRWXU | S_IRWXG);
    if(fd < 0)
    {
        printf("%s\n", strerror(errno));
    }
    printf("Shared Memory Open: %s %d\n","SharedMemoryTxBuffer", fd);
    ftruncate(fd, MODBUS_SM_MAX_ADU_LENGTH);
    m_puiTxBuffer = (uint8_t*)mmap(0,
                                   MODBUS_SM_MAX_ADU_LENGTH,
                                   PROT_READ | PROT_WRITE, MAP_SHARED,
                                   fd,
                                   0);
    close(fd);

//-------------------------------------------------------------------------------
    fd = shm_open("SharedMemoryRxBuffer",
                  O_CREAT | O_RDWR,
                  S_IRWXU | S_IRWXG);
    if(fd < 0)
    {
        printf("%s\n", strerror(errno));
    }
    printf("Shared Memory Open: %s %d\n","SharedMemoryRxBuffer", fd);
    ftruncate(fd, MODBUS_SM_MAX_ADU_LENGTH);
    m_puiRxBuffer = (uint8_t*)mmap(0,
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
    munmap(m_puiTxBuffer,
           MODBUS_SM_MAX_ADU_LENGTH);
    munmap(m_puiRxBuffer,
           MODBUS_SM_MAX_ADU_LENGTH);
}

//-------------------------------------------------------------------------------
int16_t CSharedMemoryCommunicationDevice::Read(uint8_t *puiDestination, uint16_t uiLength)
{
    std::cout << "CSharedMemoryCommunicationDevice::Read 1"  << std::endl;
//    return read(m_iDeviceDescriptorServer, puiDestination, uiLength);
}

//-------------------------------------------------------------------------------
int16_t CSharedMemoryCommunicationDevice::ReceiveStart(uint8_t *puiDestination,
        uint16_t uiLength,
        uint32_t uiReceiveTimeout)
{
//    std::cout << "CSharedMemoryCommunicationDevice::ReceiveStart 1"  << std::endl;

    uint16_t uiLengthLocal = m_puiRxBuffer[0];

    if (uiLengthLocal)
    {
        cout << "CSharedMemoryCommunicationDevice::ReceiveStart 2" << endl;
        m_puiRxBuffer[0] = 0;

        memcpy(puiDestination,
               &m_puiRxBuffer[1],
               uiLengthLocal);

        return uiLengthLocal;
    }
    else
    {
//        cout << "CSharedMemoryCommunicationDevice::ReceiveStart 3" << endl;
        return 0;
    }
//        return 0;


//    int rc;
//    fd_set rfds;
//
//    /* Add a file descriptor to the set */
//    FD_ZERO(&rfds);
//    FD_SET(m_iDeviceDescriptorServer, &rfds);
//
//    struct timeval tv;
//    tv.tv_sec = 0;
//    tv.tv_usec = (uiReceiveTimeout * 1000);
//
//    rc = select(m_iDeviceDescriptorServer + 1, &rfds, NULL, NULL, &tv);
//
//    if (rc < 0)
//    {
//        if (errno == ETIMEDOUT)
//        {
//            cout << "CSharedMemoryCommunicationDevice::ReceiveStart ETIMEDOUT" << endl;
//            return 0;
//        }
//
//        std::cout << "CSharedMemoryCommunicationDevice::ReceiveStart timeout"  << std::endl;
//        return rc;
//    }
//    else if( FD_ISSET( m_iDeviceDescriptorServer, &rfds ) )
//    {
//        std::cout << "CSharedMemoryCommunicationDevice::ReceiveStart FD_ISSET"  << std::endl;
//        rc = read(m_iDeviceDescriptorServer, (char*)puiDestination, uiLength);
////        rc = recv(m_iDeviceDescriptorServer, (char*)puiDestination, uiLength, 0);
//
//        if (rc < 0)
//        {
//            std::cout << "CSharedMemoryCommunicationDevice::ReceiveStart recv error"  << std::endl;
//            return rc;
//        }
//        else
//        {
//            if (rc)
//            {
//
////                cout << "puiDestination" << endl;
////                unsigned char *pucSourceTemp;
////                pucSourceTemp = (unsigned char*)puiDestination;
////                for(int i=0; i<32; )
////                {
////                    for(int j=0; j<8; j++)
////                    {
////                        cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
////                    }
////                    cout << endl;
////                    i += 8;
////                }
//
//                std::cout << "CSharedMemoryCommunicationDevice::ReceiveStart recv rc "  << (int)rc  << std::endl;
//                return rc;
//            }
//            else
//            {
//                std::cout << "CSharedMemoryCommunicationDevice::ReceiveStart recv 0 " << std::endl;
//                return -1;
//            }
//        }
//    }
//    else
//    {
//        cout << "CSharedMemoryCommunicationDevice::ReceiveStart 6" << endl;
//        return 0;
//    }
}

//-------------------------------------------------------------------------------
int16_t CSharedMemoryCommunicationDevice::ReceiveContinue(uint8_t *puiDestination,
        uint16_t uiLength,
        uint32_t uiReceiveTimeout)
{
    std::cout << "CSharedMemoryCommunicationDevice::ReceiveContinue 1"  << std::endl;

    uint16_t uiLengthLocal = m_puiRxBuffer[0];

    if (uiLengthLocal)
    {
        cout << "CSharedMemoryCommunicationDevice::ReceiveContinue 2" << endl;
        m_puiRxBuffer[0] = 0;

        memcpy(puiDestination,
               &m_puiRxBuffer[1],
               uiLengthLocal);

        return uiLengthLocal;
    }
    else
    {
        cout << "CSharedMemoryCommunicationDevice::ReceiveContinue 3" << endl;
        return 0;
    }

    //    int rc;
//    fd_set rfds;
//
//    /* Add a file descriptor to the set */
//    FD_ZERO(&rfds);
//    FD_SET(m_iDeviceDescriptorServer, &rfds);
//
//    struct timeval tv;
//    tv.tv_sec = 0;
//    tv.tv_usec = (uiReceiveTimeout * 1000);
//
//    rc = select(m_iDeviceDescriptorServer + 1, &rfds, NULL, NULL, &tv);
//
//    if (rc < 0)
//    {
//        if (errno == ETIMEDOUT)
//        {
//            cout << "CSharedMemoryCommunicationDevice::ReceiveContinue ETIMEDOUT" << endl;
//            return 0;
//        }
//
//        std::cout << "CSharedMemoryCommunicationDevice::ReceiveContinue timeout"  << std::endl;
//        return rc;
//    }
//    else if( FD_ISSET( m_iDeviceDescriptorServer, &rfds ) )
//    {
//        std::cout << "CSharedMemoryCommunicationDevice::ReceiveContinue FD_ISSET"  << std::endl;
//        rc = read(m_iDeviceDescriptorServer, (char*)puiDestination, uiLength);
////        rc = recv(m_iDeviceDescriptorServer, (char*)puiDestination, uiLength, 0);
//
//        if (rc < 0)
//        {
//            std::cout << "CSharedMemoryCommunicationDevice::ReceiveContinue recv error"  << std::endl;
//            return rc;
//        }
//        else
//        {
//            if (rc)
//            {
//
////                cout << "puiDestination" << endl;
////                unsigned char *pucSourceTemp;
////                pucSourceTemp = (unsigned char*)puiDestination;
////                for(int i=0; i<32; )
////                {
////                    for(int j=0; j<8; j++)
////                    {
////                        cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
////                    }
////                    cout << endl;
////                    i += 8;
////                }
//
//                std::cout << "CSharedMemoryCommunicationDevice::ReceiveContinue recv rc "  << (int)rc  << std::endl;
//                return rc;
//            }
//            else
//            {
//                std::cout << "CSharedMemoryCommunicationDevice::ReceiveContinue recv 0 " << std::endl;
//                return -1;
//            }
//        }
//    }
//    else
//    {
//        cout << "CSharedMemoryCommunicationDevice::ReceiveContinue 6" << endl;
//        return 0;
//    }
}

//-------------------------------------------------------------------------------
int16_t CSharedMemoryCommunicationDevice::Write(uint8_t *puiSource, uint16_t uiLength)
{
    std::cout << "CSharedMemoryCommunicationDevice::Write"  << std::endl;

    memcpy(&m_puiTxBuffer[1],
           puiSource,
           uiLength);

    m_puiTxBuffer[0]  = uiLength;

    return uiLength;

//    return write(m_iDeviceDescriptorServer, puiSource, uiLength);
}

//-------------------------------------------------------------------------------

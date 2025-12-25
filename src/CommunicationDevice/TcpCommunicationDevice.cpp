//-------------------------------------------------------------------------------
//  Source      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-------------------------------------------------------------------------------
#include <typeinfo>

#include <iostream>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>


#include "Task.h"
#include "Platform.h"
#include "Resources.h"
#include "CommunicationDevice.h"
#include "TcpCommunicationDevice.h"

using namespace std;

//-------------------------------------------------------------------------------
CTcpCommunicationDevice::CTcpCommunicationDevice()
{
    m_iDeviceDescriptorServer = 0;
}

//-------------------------------------------------------------------------------
CTcpCommunicationDevice::~CTcpCommunicationDevice()
{

}

//-------------------------------------------------------------------------------
void CTcpCommunicationDevice::Init(void)
{
#if defined(OS_BSD)
    /* MSG_NOSIGNAL is unsupported on *BSD so we install an ignore
       handler for SIGPIPE. */
    struct sigaction sa;

    sa.sa_handler = SIG_IGN;
    if (sigaction(SIGPIPE, &sa, NULL) < 0)
    {
        /* The debug flag can't be set here... */
        fprintf(stderr, "Coud not install SIGPIPE handler.\n");
        return NULL;
    }
#endif

    memset(&m_Address, 0, sizeof(m_Address));
    m_Address.sin_family = AF_INET;

    SetFsmState(READY);
}

//-------------------------------------------------------------------------------
void CTcpCommunicationDevice::SetIpAddress(const char* pccIpAddress)
{
    m_uiIpAddress = inet_addr(pccIpAddress);
}

//-------------------------------------------------------------------------------
const char* CTcpCommunicationDevice::GetIpAddress(void)
{

}

//-------------------------------------------------------------------------------
void CTcpCommunicationDevice::SetPort(uint16_t uiPort)
{
    m_uiPort = uiPort;
}

//-------------------------------------------------------------------------------
int8_t CTcpCommunicationDevice::Open(void)
{
    cout << "CTcpCommunicationDevice::Open 1" << endl;
    return Listen();
}

//-------------------------------------------------------------------------------
int8_t CTcpCommunicationDevice::Listen(void)
{
    cout << "CTcpCommunicationDevice::Listen 1" << endl;
    int yes;
    struct sockaddr_in addr;

    m_iDeviceDescriptorServer = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_iDeviceDescriptorServer == -1)
    {
        cout << "CTcpCommunicationDevice::Listen 2" << endl;
        return -1;
    }

    yes = 1;
    if (setsockopt(m_iDeviceDescriptorServer, SOL_SOCKET, SO_REUSEADDR,
                   (char *) &yes, sizeof(yes)) == -1)
    {
        cout << "CTcpCommunicationDevice::Listen 3" << endl;
        close(m_iDeviceDescriptorServer);
        return -1;
    }



    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    /* If the modbus port is < to 1024, we need the setuid root. */
    addr.sin_port = htons(m_uiPort);
    addr.sin_addr.s_addr = INADDR_ANY;//htonl(INADDR_ANY);//

    printf("CTcpCommunicationDevice::Listen m_uiPort %d\n", m_uiPort);

    if (bind(m_iDeviceDescriptorServer, (struct sockaddr *)&addr, sizeof(addr)) == -1)
    {
        cout << "CTcpCommunicationDevice::Listen errno " << errno << endl;
        cout << "CTcpCommunicationDevice::Listen 4" << endl;
//        fprintf(stderr, "Connection failed tcp bind: %s\n",
//                CModbus::ModbusStringError(errno));
        close(m_iDeviceDescriptorServer);
        return -1;
    }

    int nb_connection = 1;
    if (listen(m_iDeviceDescriptorServer, nb_connection) == -1)
    {
        cout << "CTcpCommunicationDevice::Listen 5" << endl;
        close(m_iDeviceDescriptorServer);
        return -1;
    }

    // Сделаем не блокирующим.
    int flags = fcntl(m_iDeviceDescriptorServer, F_GETFL, 0);
    fcntl(m_iDeviceDescriptorServer, F_SETFL, flags | O_NONBLOCK);

    cout << "CTcpCommunicationDevice::Listen 6" << endl;
    return 0;
}

//-------------------------------------------------------------------------------
int8_t CTcpCommunicationDevice::Accept(void)
{
//    cout << "CTcpCommunicationDevice::Accept 1" << endl;

    struct sockaddr_in addr;
    socklen_t addrlen;
    addrlen = sizeof(addr);
    memset(&addr, 0, sizeof(addr));
    m_iDeviceDescriptorClient = accept(m_iDeviceDescriptorServer, (struct sockaddr *)&addr, &addrlen);
    if (m_iDeviceDescriptorClient == -1)
    {
//        cout << "CTcpCommunicationDevice::Accept 2" << endl;
//            fprintf(stderr, "Connection failed tcp bind: %s\n",
//                    CModbus::ModbusStringError(errno));
        close(m_iDeviceDescriptorServer);
        return 0;
    }

    printf("The client connection from %s is accepted\n",
           inet_ntoa(addr.sin_addr));

//        // Сделаем не блокирующим.
//        int flags = fcntl(m_iDeviceDescriptorClient, F_GETFL, 0);
//        fcntl(m_iDeviceDescriptorClient, F_SETFL, flags | O_NONBLOCK);

//    cout << "CTcpCommunicationDevice::Accept 3" << endl;
    return 1;
}

////-------------------------------------------------------------------------------
//int8_t CTcpCommunicationDevice::Accept(uint32_t uiBlockingTime)
//{
//    cout << "CTcpCommunicationDevice::Accept 1" << endl;
//    // Сделаем не блокирующим.
//    int flags = fcntl(m_iDeviceDescriptorServer, F_GETFL, 0);
//    fcntl(m_iDeviceDescriptorServer, F_SETFL, flags | O_NONBLOCK);
//
//    fd_set readfds, writefds;
//    FD_ZERO(&readfds);
//    FD_SET(m_iDeviceDescriptorServer, &readfds);
//    FD_SET(STDIN_FILENO, &readfds);
//
//    tv.tv_sec = 0;
//    tv.tv_usec = 5000000;
//    p_tv = &tv;
//
//    int ready = select(m_iDeviceDescriptorServer + 1, &readfds, NULL, NULL, &tv);
//
//    cout << "CTcpCommunicationDevice::Accept 2" << endl;
//    if (ready == -1)
//    {
//        cout << "CTcpCommunicationDevice::Accept 3" << endl;
//        return ready;
//    }
//    else if (FD_ISSET(m_iDeviceDescriptorServer, &readfds))
//    {
//        cout << "CTcpCommunicationDevice::Accept 4" << endl;
//        struct sockaddr_in addr;
//        socklen_t addrlen;
//        addrlen = sizeof(addr);
//        memset(&addr, 0, sizeof(addr));
//        m_iDeviceDescriptorClient = accept(m_iDeviceDescriptorServer, (struct sockaddr *)&addr, &addrlen);
//        if (m_iDeviceDescriptorClient == -1)
//        {
//            cout << "CTcpCommunicationDevice::Accept 5" << endl;
////            fprintf(stderr, "Connection failed tcp bind: %s\n",
////                    CModbus::ModbusStringError(errno));
//            close(m_iDeviceDescriptorServer);
//            return 0;
//        }
//
//        printf("The client connection from %s is accepted\n",
//               inet_ntoa(addr.sin_addr));
//
//        // Сделаем не блокирующим.
//        int flags = fcntl(m_iDeviceDescriptorClient, F_GETFL, 0);
//        fcntl(m_iDeviceDescriptorClient, F_SETFL, flags | O_NONBLOCK);
//
//        return 1;
//    }
//    else
//    {
//        cout << "CTcpCommunicationDevice::Accept 6" << endl;
//        return 0;
//    }
//}

//-------------------------------------------------------------------------------
int8_t CTcpCommunicationDevice::Connect(void)
{
    /* Establishes a modbus TCP connection with a Modbus server. */
    int rc;

    m_iDeviceDescriptorClient = socket(PF_INET, SOCK_STREAM, 0);
    if (m_iDeviceDescriptorClient == -1)
    {
        return -1;
    }

    int option;

    /* Set the TCP no delay flag */
    /* SOL_TCP = IPPROTO_TCP */
    option = 1;
    rc = setsockopt(m_iDeviceDescriptorClient, IPPROTO_TCP, TCP_NODELAY,
                    (const void *)&option, sizeof(int));
    if (rc == -1)
    {
        return -1;
    }

    rc = connect(m_iDeviceDescriptorClient, (struct sockaddr *)&m_Address,
                 sizeof(struct sockaddr_in));
    if (rc == -1)
    {
//        fprintf(stderr, "Connection failed tcp: %s\n",
//                CModbus::ModbusStringError(errno));
        cout << "Connect connect error" << endl;
        close(m_iDeviceDescriptorClient);
        return -1;
    }

    // Сделаем не блокирующим.
    int flags = fcntl(m_iDeviceDescriptorClient, F_GETFL, 0);
    fcntl(m_iDeviceDescriptorClient, F_SETFL, flags | O_NONBLOCK);

    return 0;
}

////-------------------------------------------------------------------------------
//bool CTcpCommunicationDevice::IsDataAvailable(void)
//{
//    int ready;
//    ready = select((m_iDeviceDescriptorServer + 1), &readfds, &writefds, NULL, pto);
//
//    if (ready == -1)
//    {
//        return false;
//    }
//    else
//    {
//        return true;
//    }
//}

//-------------------------------------------------------------------------------
void CTcpCommunicationDevice::CloseClient(void)
{
    //std::cout << "CTcpCommunicationDevice::CloseClient 1"  << std::endl;
    shutdown(m_iDeviceDescriptorClient, SHUT_RDWR);
    close(m_iDeviceDescriptorClient);
}

//-------------------------------------------------------------------------------
int8_t CTcpCommunicationDevice::Close(void)
{
    std::cout << "CTcpCommunicationDevice::Close 1"  << std::endl;
    if (m_iDeviceDescriptorServer != 0)
    {
        close(m_iDeviceDescriptorServer);
    }
    close(m_iDeviceDescriptorClient);
    usleep(100000);
}

//-------------------------------------------------------------------------------
int16_t CTcpCommunicationDevice::Read(uint8_t *puiDestination, uint16_t uiLength)
{
//    //std::cout << "CTcpCommunicationDevice::Read 1"  << std::endl;
//    return read(m_iDeviceDescriptorClient, puiDestination, uiLength);
    return recv(m_iDeviceDescriptorClient, (char*)puiDestination, uiLength, 0);
}

//-------------------------------------------------------------------------------
int16_t CTcpCommunicationDevice::ReceiveStart(uint8_t *puiDestination,
        uint16_t uiLength,
        uint32_t uiReceiveTimeout)
{
//    //std::cout << "CTcpCommunicationDevice::ReceiveStart 1"  << std::endl;

//    // Сделаем не блокирующим.
//    int flags = fcntl(m_iDeviceDescriptorServer, F_GETFL, 0);
//    fcntl(m_iDeviceDescriptorServer, F_SETFL, flags | O_NONBLOCK);

    int rc;
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(m_iDeviceDescriptorServer, &readfds);

    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = (uiReceiveTimeout * 1000);

    int ready = select(m_iDeviceDescriptorServer + 1, &readfds, NULL, NULL, &tv);

//    cout << "CTcpCommunicationDevice::ReceiveStart 2" << endl;
    if (ready < 0)
    {
        cout << "CTcpCommunicationDevice::ReceiveStart errno " << errno << endl;
        if (errno == ETIMEDOUT)
        {
            cout << "CTcpCommunicationDevice::ReceiveStart ETIMEDOUT" << endl;
            return 0;
        }

        cout << "CTcpCommunicationDevice::ReceiveStart 3" << endl;
        return ready;
    }
    else if (FD_ISSET(m_iDeviceDescriptorServer, &readfds))
    {
        cout << "CTcpCommunicationDevice::ReceiveStart 4" << endl;
        struct sockaddr_in addr;
        socklen_t addrlen;
        addrlen = sizeof(addr);
        memset(&addr, 0, sizeof(addr));

        m_iDeviceDescriptorClient = accept(m_iDeviceDescriptorServer, (struct sockaddr *)&addr, &addrlen);

        if (m_iDeviceDescriptorClient < 0)
        {
            cout << "CTcpCommunicationDevice::ReceiveStart 5" << endl;
//            fprintf(stderr, "Connection failed tcp bind: %s\n",
//                    CModbus::ModbusStringError(errno));
            close(m_iDeviceDescriptorServer);
            return -1;
        }

        printf("The client connection from %s is accepted\n",
               inet_ntoa(addr.sin_addr));

//        // Сделаем не блокирующим.
//        int flags = fcntl(m_iDeviceDescriptorClient, F_GETFL, 0);
//        fcntl(m_iDeviceDescriptorClient, F_SETFL, flags | O_NONBLOCK);

        rc = read(m_iDeviceDescriptorClient, (char*)puiDestination, uiLength);
//        rc = recv(m_iDeviceDescriptorClient, (char*)puiDestination, uiLength, 0);

        if (rc < 0)
        {
            //std::cout << "CTcpCommunicationDevice::ReceiveStart recv error"  << std::endl;
            return rc;
        }
        else
        {
            if (rc)
            {

//                cout << "ReceiveStart" << endl;
//                uint8_t *pucSourceTemp;
//                pucSourceTemp = (uint8_t*)puiDestination;
//                for(int i=0; i<32; )
//                {
//                    for(int j=0; j<8; j++)
//                    {
//                        cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
//                    }
//                    cout << endl;
//                    i += 8;
//                }

                //std::cout << "CTcpCommunicationDevice::ReceiveStart recv rc "  << (int)rc  << std::endl;
                return rc;
            }
            else
            {
                //std::cout << "CTcpCommunicationDevice::ReceiveStart recv 0 " << std::endl;
                return -1;
            }
        }
    }
    else
    {
//        cout << "CTcpCommunicationDevice::ReceiveStart 6" << endl;
        return 0;
    }
}

//-------------------------------------------------------------------------------
int16_t CTcpCommunicationDevice::ReceiveContinue(uint8_t *puiDestination,
        uint16_t uiLength,
        uint32_t uiReceiveTimeout)
{
//    //std::cout << "CTcpCommunicationDevice::ReceiveContinue 1"  << std::endl;

    int rc;
    fd_set rfds;

    /* Add a file descriptor to the set */
    FD_ZERO(&rfds);
    FD_SET(m_iDeviceDescriptorClient, &rfds);

    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = (uiReceiveTimeout * 1000);

    rc = select(m_iDeviceDescriptorClient + 1, &rfds, NULL, NULL, &tv);

    if (rc < 0)
    {
        if (errno == ETIMEDOUT)
        {
            //std::cout << "CTcpCommunicationDevice::ReceiveStart ETIMEDOUT" << std::endl;
            return 0;
        }

        //std::cout << "CTcpCommunicationDevice::ReceiveContinue timeout"  << std::endl;
        return rc;
    }
    else if( FD_ISSET( m_iDeviceDescriptorClient, &rfds ) )
    {
        //std::cout << "CTcpCommunicationDevice::ReceiveContinue FD_ISSET"  << std::endl;
        rc = read(m_iDeviceDescriptorClient, (char*)puiDestination, uiLength);
//        rc = recv(m_iDeviceDescriptorClient, (char*)puiDestination, uiLength, 0);

        if (rc < 0)
        {
            //std::cout << "CTcpCommunicationDevice::ReceiveContinue recv error"  << std::endl;
            return rc;
        }
        else
        {
            if (rc)
            {

//                cout << "ReceiveContinue" << endl;
//                uint8_t *pucSourceTemp;
//                pucSourceTemp = (uint8_t*)puiDestination;
//                for(int i=0; i<32; )
//                {
//                    for(int j=0; j<8; j++)
//                    {
//                        cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
//                    }
//                    cout << endl;
//                    i += 8;
//                }

                //std::cout << "CTcpCommunicationDevice::ReceiveContinue recv rc "  << (int)rc  << std::endl;
                return rc;
            }
            else
            {
                //std::cout << "CTcpCommunicationDevice::ReceiveContinue recv 0 " << std::endl;
                return -1;
            }
        }
    }

    //std::cout << "CTcpCommunicationDevice::ReceiveContinue return 0"  << std::endl;
    return 0;
}

//-------------------------------------------------------------------------------
int16_t CTcpCommunicationDevice::Write(uint8_t *puiSource, uint16_t uiLength)
{
//    SetDataIsWrited(true);
    return write(m_iDeviceDescriptorClient, puiSource, uiLength);
}

//-------------------------------------------------------------------------------
// производит обмен данными по SPI.
int CTcpCommunicationDevice::Exchange(uint8_t uiAddress,
                                      uint8_t *pucTxBuff,
                                      uint8_t *pucRxBuff,
                                      int iLength,
                                      int iSpeed)
{

}

//-------------------------------------------------------------------------------

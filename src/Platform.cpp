//-------------------------------------------------------------------------------
//  Source      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-------------------------------------------------------------------------------
#include <iostream>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>

#include "Platform.h"

using namespace std;

//-------------------------------------------------------------------------------
CCommunicationDevice::CCommunicationDevice()
{
    SetDataIsWrited(false);
}

//-------------------------------------------------------------------------------
CCommunicationDevice::~CCommunicationDevice()
{

}

//-------------------------------------------------------------------------------















//-------------------------------------------------------------------------------
CSerialPort::CSerialPort()
{

}
//-------------------------------------------------------------------------------
CSerialPort::~CSerialPort()
{

}

//-------------------------------------------------------------------------------
void CSerialPort::Init(void)
{
    memset(&m_xTios, 0, sizeof(struct termios));

    /* C_CFLAG      Control options
       CLOCAL       Local line - do not change "owner" of port
       CREAD        Enable receiver
    */
    m_xTios.c_cflag |= (CREAD | CLOCAL);
    /* CSIZE, HUPCL, CRTSCTS (hardware flow control) */


    /* Read the man page of termios if you need more information. */

    /* This field isn't used on POSIX systems
       m_xTios.c_line = 0;
    */

    /* C_LFLAG      Line options

       ISIG Enable SIGINTR, SIGSUSP, SIGDSUSP, and SIGQUIT signals
       ICANON       Enable canonical input (else raw)
       XCASE        Map uppercase \lowercase (obsolete)
       ECHO Enable echoing of input characters
       ECHOE        Echo erase character as BS-SP-BS
       ECHOK        Echo NL after kill character
       ECHONL       Echo NL
       NOFLSH       Disable flushing of input buffers after
       interrupt or quit characters
       IEXTEN       Enable extended functions
       ECHOCTL      Echo control characters as ^char and delete as ~?
       ECHOPRT      Echo erased character as character erased
       ECHOKE       BS-SP-BS entire line on line kill
       FLUSHO       Output being flushed
       PENDIN       Retype pending input at next read or input char
       TOSTOP       Send SIGTTOU for background output

       Canonical input is line-oriented. Input characters are put
       into a buffer which can be edited interactively by the user
       until a CR (carriage return) or LF (line feed) character is
       received.

       Raw input is unprocessed. Input characters are passed
       through exactly as they are received, when they are
       received. Generally you'll deselect the ICANON, ECHO,
       ECHOE, and ISIG options when using raw input
    */

    /* Raw input */
    m_xTios.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);

    /* Software flow control is disabled */
    m_xTios.c_iflag &= ~(IXON | IXOFF | IXANY);

    /* C_OFLAG      Output options
       OPOST        Postprocess output (not set = raw output)
       ONLCR        Map NL to CR-NL

       ONCLR ant others needs OPOST to be enabled
    */

    /* Raw ouput */
    m_xTios.c_oflag &=~ OPOST;

    /* C_CC         Control characters
       VMIN         Minimum number of characters to read
       VTIME        Time to wait for data (tenths of seconds)

       UNIX serial interface drivers provide the ability to
       specify character and packet timeouts. Two elements of the
       c_cc array are used for timeouts: VMIN and VTIME. Timeouts
       are ignored in canonical input mode or when the NDELAY
       option is set on the file via open or fcntl.

       VMIN specifies the minimum number of characters to read. If
       it is set to 0, then the VTIME value specifies the time to
       wait for every character read. Note that this does not mean
       that a read call for N bytes will wait for N characters to
       come in. Rather, the timeout will apply to the first
       character and the read call will return the number of
       characters immediately available (up to the number you
       request).

       If VMIN is non-zero, VTIME specifies the time to wait for
       the first character read. If a character is read within the
       time given, any read will block (wait) until all VMIN
       characters are read. That is, once the first character is
       read, the serial interface driver expects to receive an
       entire packet of characters (VMIN bytes total). If no
       character is read within the time allowed, then the call to
       read returns 0. This method allows you to tell the serial
       driver you need exactly N bytes and any read call will
       return 0 or N bytes. However, the timeout only applies to
       the first character read, so if for some reason the driver
       misses one character inside the N byte packet then the read
       call could block forever waiting for additional input
       characters.

       VTIME specifies the amount of time to wait for incoming
       characters in tenths of seconds. If VTIME is set to 0 (the
       default), reads will block (wait) indefinitely unless the
       NDELAY option is set on the port with open or fcntl.
    */
    /* Unused because we use open with the NDELAY option */
    m_xTios.c_cc[VMIN] = 0;
    m_xTios.c_cc[VTIME] = 0;


    memset(&m_xRs485Conf, 0x0, sizeof(struct serial_rs485));

    m_xRs485Conf.flags |= SER_RS485_ENABLED;
    m_xRs485Conf.flags |= SER_RS485_RTS_ON_SEND;
    //m_xRs485Conf.flags &= ~(SER_RS485_RTS_ON_SEND);
    m_xRs485Conf.flags &= ~SER_RS485_RTS_AFTER_SEND;
    //m_xRs485Conf.flags |= SER_RS485_RTS_AFTER_SEND;
    m_xRs485Conf.delay_rts_before_send = 0;
    m_xRs485Conf.delay_rts_after_send = 0;
}

//-------------------------------------------------------------------------------
void CSerialPort::SetDeviceName(const char* pccDeviceName)
{
    m_pccDeviceName = pccDeviceName;
}

//-------------------------------------------------------------------------------
const char* CSerialPort::GetDeviceName(void)
{
    return m_pccDeviceName;
}

//-------------------------------------------------------------------------------
void CSerialPort::SetBaudRate(uint32_t uiBaudRate)
{
    speed_t speed;

    /* C_ISPEED     Input baud (new interface)
       C_OSPEED     Output baud (new interface)
    */
    switch (uiBaudRate)
    {
    case 110:
        speed = B110;
        break;
    case 300:
        speed = B300;
        break;
    case 600:
        speed = B600;
        break;
    case 1200:
        speed = B1200;
        break;
    case 2400:
        speed = B2400;
        break;
    case 4800:
        speed = B4800;
        break;
    case 9600:
        speed = B9600;
        break;
    case 19200:
        speed = B19200;
        break;
    case 38400:
        speed = B38400;
        break;
    case 57600:
        speed = B57600;
        break;
    case 115200:
        speed = B115200;
        break;
    default:
        speed = B9600;
        break;
    }

    /* Set the baud rate */
    if ((cfsetispeed(&m_xTios, speed) < 0) ||
            (cfsetospeed(&m_xTios, speed) < 0))
    {
//        close(m_iDeviceDescriptorServer);
//        m_iDeviceDescriptorServer = -1;
//        return -1;
    }
}

//-------------------------------------------------------------------------------
void CSerialPort::SetDataBits(uint8_t uiDataBits)
{
    /* Set data bits (5, 6, 7, 8 bits)
       CSIZE        Bit mask for data bits
    */
    m_xTios.c_cflag &= ~CSIZE;
    switch (uiDataBits)
    {
    case 5:
        m_xTios.c_cflag |= CS5;
        break;
    case 6:
        m_xTios.c_cflag |= CS6;
        break;
    case 7:
        m_xTios.c_cflag |= CS7;
        break;
    case 8:
    default:
        m_xTios.c_cflag |= CS8;
        break;
    }
}

//-------------------------------------------------------------------------------
void CSerialPort::SetParity(char cParity)
{
    /* PARENB       Enable parity bit
       PARODD       Use odd parity instead of even */
    if (cParity == 'N')
    {
        /* None */
        m_xTios.c_cflag &=~ PARENB;
    }
    else if (cParity == 'E')
    {
        /* Even */
        m_xTios.c_cflag |= PARENB;
        m_xTios.c_cflag &=~ PARODD;
    }
    else
    {
        /* Odd */
        m_xTios.c_cflag |= PARENB;
        m_xTios.c_cflag |= PARODD;
    }

    /* C_IFLAG      Input options

       Constant     Description
       INPCK        Enable parity check
       IGNPAR       Ignore parity errors
       PARMRK       Mark parity errors
       ISTRIP       Strip parity bits
       IXON Enable software flow control (outgoing)
       IXOFF        Enable software flow control (incoming)
       IXANY        Allow any character to start flow again
       IGNBRK       Ignore break condition
       BRKINT       Send a SIGINT when a break condition is detected
       INLCR        Map NL to CR
       IGNCR        Ignore CR
       ICRNL        Map CR to NL
       IUCLC        Map uppercase to lowercase
       IMAXBEL      Echo BEL on input line too long
    */
    if (cParity == 'N')
    {
        /* None */
        m_xTios.c_iflag &= ~INPCK;
    }
    else
    {
        m_xTios.c_iflag |= INPCK;
    }
}

//-------------------------------------------------------------------------------
void CSerialPort::SetStopBit(uint8_t uiStopBit)
{
    /* Stop bit (1 or 2) */
    if (uiStopBit == 1)
    {
        m_xTios.c_cflag &=~ CSTOPB;
    }
    else /* 2 */
    {
        m_xTios.c_cflag |= CSTOPB;
    }
}

//-------------------------------------------------------------------------------
int8_t CSerialPort::Open(void)
{
    cout << "CSerialPort::Open m_pccDeviceName " << m_pccDeviceName << endl;
    /* The O_NOCTTY flag tells UNIX that this program doesn't want
       to be the "controlling terminal" for that port. If you
       don't specify this then any input (such as keyboard abort
       signals and so forth) will affect your process

       Timeouts are ignored in canonical input mode or when the
       NDELAY option is set on the file via open or fcntl */
    m_iDeviceDescriptorServer = open(m_pccDeviceName, O_RDWR | O_NOCTTY | O_NDELAY | O_EXCL);
    if (m_iDeviceDescriptorServer == -1)
    {
        fprintf(stderr, "ERROR Can't open the device %s (%s)\n",
                m_pccDeviceName, strerror(errno));
        return -1;
    }

    if (tcsetattr(m_iDeviceDescriptorServer, TCSANOW, &m_xTios) < 0)
    {
        close(m_iDeviceDescriptorServer);
        m_iDeviceDescriptorServer = -1;
        return -1;
    }

    if (ioctl(m_iDeviceDescriptorServer, TIOCSRS485, &m_xRs485Conf) < 0)
    {
        printf("Error! set rs485 ioctl: %d %s\n", errno, strerror(errno));
        return -1;
    }

    // Сделаем не блокирующим.
    int flags = fcntl(m_iDeviceDescriptorServer, F_GETFL, 0);
    fcntl(m_iDeviceDescriptorServer, F_SETFL, flags | O_NONBLOCK);

//    pto = NULL;
//    FD_ZERO(&readfds);
//    FD_ZERO(&writefds);
//    FD_SET(m_iDeviceDescriptorServer, &readfds);
}

//-------------------------------------------------------------------------------
int8_t CSerialPort::Close(void)
{
    close(m_iDeviceDescriptorServer);
}

////-------------------------------------------------------------------------------
//void CSerialPort::Reset(void)
//{
//
//}

////-------------------------------------------------------------------------------
//bool CSerialPort::IsDataAvailable(void)
//{
//        int ready;
//        ready = select((m_iDeviceDescriptorServer + 1), &readfds, &writefds, NULL, pto);
//
//        if (ready == -1)
//        {
//            return false;
//        }
//        else
//        {
//            return true;
//        }
//}

//-------------------------------------------------------------------------------
int16_t CSerialPort::Read(uint8_t *puiDestination, uint16_t uiLength)
{
    return read(m_iDeviceDescriptorServer, puiDestination, uiLength);
}

//-------------------------------------------------------------------------------
int16_t CSerialPort::Write(uint8_t *puiSource, uint16_t uiLength)
{
//    std::cout << "CSerialPort::Write"  << std::endl;
    SetDataIsWrited(true);
    return write(m_iDeviceDescriptorServer, puiSource, uiLength);
}

//-------------------------------------------------------------------------------
// производит обмен данными по SPI.
int CSerialPort::Exchange(uint8_t uiAddress,
                          unsigned char *pucTxBuff,
                          unsigned char *pucRxBuff,
                          int iLength,
                          int iSpeed)
{

}

//-------------------------------------------------------------------------------













//-------------------------------------------------------------------------------
CTcpCommunicationDevice::CTcpCommunicationDevice()
{

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

//    int sock, listener;
//    struct sockaddr_in addr;
//    char buf[1024];
//    int bytes_read;
//
//    listener = socket(AF_INET, SOCK_STREAM, 0);
//    if(listener < 0)
//    {
//        perror("socket");
//        exit(1);
//    }
//
//    int yes;
//    yes = 1;
//    if (setsockopt(listener, SOL_SOCKET, SO_REUSEADDR,
//                   (char *) &yes, sizeof(yes)) == -1)
//    {
//        cout << "CTcpCommunicationDevice::Open error 8" << endl;
//        close(listener);
//        return -1;
//    }
//
//    addr.sin_family = AF_INET;
//    addr.sin_port = htons(5);
//    addr.sin_addr.s_addr = htonl(INADDR_ANY);
//    if(bind(listener, (struct sockaddr *)&addr, sizeof(addr)) < 0)
//    {
//        perror("bind");
//        exit(2);
//    }
//
//        cout << "bind ok" << endl;
//    return 0;


    /* Establishes a modbus TCP connection with a Modbus server. */
    int rc;

    m_iDeviceDescriptorServer = socket(PF_INET, SOCK_STREAM, 0);
    if (m_iDeviceDescriptorServer == -1)
    {
        cout << "CTcpCommunicationDevice::Open error" << endl;
        return -1;
    }

    cout << "CTcpCommunicationDevice::Open ok" << endl;

    int option;

    /* Set the TCP no delay flag */
    /* SOL_TCP = IPPROTO_TCP */
    option = 1;
    rc = setsockopt(m_iDeviceDescriptorServer, IPPROTO_TCP, TCP_NODELAY,
                    (const void *)&option, sizeof(int));
    if (rc == -1)
    {
        cout << "CTcpCommunicationDevice::Open error 2" << endl;
        return -1;
    }

    rc = connect(m_iDeviceDescriptorServer, (struct sockaddr *)&m_Address,
                 sizeof(struct sockaddr_in));
    if (rc == -1)
    {
//        fprintf(stderr, "Connection failed tcp: %s\n",
//                CModbus::ModbusStringError(errno));
        cout << "CTcpCommunicationDevice::Open error 3" << endl;
        close(m_iDeviceDescriptorServer);
//        return -1;
    }



    int new_socket;
    int yes;
    struct sockaddr_in addr;

    new_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (new_socket == -1)
    {
        cout << "CTcpCommunicationDevice::Open error 4" << endl;
        return -1;
    }

    yes = 1;
    if (setsockopt(new_socket, SOL_SOCKET, SO_REUSEADDR,
                   (char *) &yes, sizeof(yes)) == -1)
    {
        cout << "CTcpCommunicationDevice::Open error 8" << endl;
        close(new_socket);
        return -1;
    }



    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    /* If the modbus port is < to 1024, we need the setuid root. */
//    addr.sin_port = m_Address.sin_port;
    addr.sin_port = htons(502);
    addr.sin_addr.s_addr = INADDR_ANY;//htonl(INADDR_ANY);//

    cout << "CStorageDeviceFileSystem::Write pccIpAddress" << " " << addr.sin_addr.s_addr << endl;
    cout << "CStorageDeviceFileSystem::Write sin_port" << " " << (int)addr.sin_port << endl;
    cout << "CStorageDeviceFileSystem::Write sin_port" << " " << (int)htons(502) << endl;

    if (bind(new_socket, (struct sockaddr *)&addr, sizeof(addr)) == -1)
    {
//        fprintf(stderr, "Connection failed tcp bind: %s\n",
//                CModbus::ModbusStringError(errno));
        cout << "CTcpCommunicationDevice::Open error 5" << endl;
        close(new_socket);
        return -1;
    }

    int nb_connection = 1;
    if (listen(new_socket, nb_connection) == -1)
    {
        cout << "CTcpCommunicationDevice::Open error 6" << endl;
        close(new_socket);
        return -1;
    }



//    struct sockaddr_in addr;
    socklen_t addrlen;

    addrlen = sizeof(addr);
    m_iDeviceDescriptorServer = accept(new_socket, (struct sockaddr *)&addr, &addrlen);
    if (m_iDeviceDescriptorServer == -1)
    {
        cout << "CTcpCommunicationDevice::Open error 7" << endl;
        close(new_socket);
        return -1;
    }

//    if (ctx->debug)
//    {
    printf("The client connection from %s is accepted\n",
           inet_ntoa(m_Address.sin_addr));
//    }



    int flags = fcntl(m_iDeviceDescriptorServer, F_GETFL, 0);
    fcntl(m_iDeviceDescriptorServer, F_SETFL, flags | O_NONBLOCK);


    return 0;
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


//    cout << "CTcpCommunicationDevice::Accept 1" << endl;
////    // Сделаем не блокирующим.
////    int flags = fcntl(m_iDeviceDescriptorServer, F_GETFL, 0);
////    fcntl(m_iDeviceDescriptorServer, F_SETFL, flags | O_NONBLOCK);
//
//    fd_set readfds, writefds;
//    FD_ZERO(&readfds);
////    FD_ZERO(&writefds);
//    FD_SET(m_iDeviceDescriptorServer, &readfds);
////    FD_SET(m_iDeviceDescriptorServer, &writefds);
//    FD_SET(STDIN_FILENO, &readfds);
////    FD_SET(STDIN_FILENO, &writefds);
//
//    int ready = select(m_iDeviceDescriptorServer + 1, &readfds, NULL, NULL, NULL);
//
//    cout << "CTcpCommunicationDevice::Accept 2" << endl;
//    if (ready == -1)
//    {
//        cout << "CTcpCommunicationDevice::Accept 3" << endl;
//        return 0;
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
////        // Сделаем не блокирующим.
////        int flags = fcntl(m_iDeviceDescriptorClient, F_GETFL, 0);
////        fcntl(m_iDeviceDescriptorClient, F_SETFL, flags | O_NONBLOCK);
//
//        return 1;
//    }
//    else
//    {
//        cout << "CTcpCommunicationDevice::Accept 6" << endl;
//        return 0;
//    }
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
    shutdown(m_iDeviceDescriptorClient, SHUT_RDWR);
    close(m_iDeviceDescriptorClient);
}

//-------------------------------------------------------------------------------
int8_t CTcpCommunicationDevice::Close(void)
{
    close(m_iDeviceDescriptorServer);
    close(m_iDeviceDescriptorClient);
}

//-------------------------------------------------------------------------------
int16_t CTcpCommunicationDevice::Read(uint8_t *puiDestination, uint16_t uiLength)
{
//    std::cout << "CTcpCommunicationDevice::Read 1"  << std::endl;
//    return read(m_iDeviceDescriptorClient, puiDestination, uiLength);
    return recv(m_iDeviceDescriptorClient, (char*)puiDestination, uiLength, 0);



//    int rc;
//    fd_set rfds;
//    struct timeval tv;
//    struct timeval *p_tv;
//
//
//    tv.tv_sec = 0;
//    tv.tv_usec = 15000000;
//    p_tv = &tv;
//
//    /* Add a file descriptor to the set */
//    FD_ZERO(&rfds);
//    FD_SET(m_iDeviceDescriptorServer, &rfds);
//
//    if( select(m_iDeviceDescriptorServer + 1, &rfds, NULL, NULL, &tv) == -1 )
//    {
//        std::cout << "CTcpCommunicationDevice::Read timeout"  << std::endl;
//        return -1;
//    }
//    else if( FD_ISSET( m_iDeviceDescriptorClient, &rfds ) )
//    {
//        std::cout << "CTcpCommunicationDevice::Read FD_ISSET"  << std::endl;
////        if( ( rc = read(m_iDeviceDescriptorClient, (char*)puiDestination, uiLength) ) == -1 )
//        if( ( rc = recv(m_iDeviceDescriptorClient, (char*)puiDestination, uiLength, 0) ) == -1 )
//        {
//            std::cout << "CTcpCommunicationDevice::Read recv error"  << std::endl;
//            return -1;
//        }
//        else
//        {
//            if (rc)
//            {
//
////                cout << "Read" << endl;
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
//                std::cout << "CTcpCommunicationDevice::Read recv rc "  << (int)rc  << std::endl;
//                return rc;
//            }
//            else
//            {
//                std::cout << "CTcpCommunicationDevice::Read recv 0 " << std::endl;
//                return 0;
//            }
//        }
//    }
//
//    std::cout << "CTcpCommunicationDevice::Read return 0"  << std::endl;
//    return 0;
}

//-------------------------------------------------------------------------------
int16_t CTcpCommunicationDevice::ReceiveStart(uint8_t *puiDestination,
        uint16_t uiLength,
        uint32_t uiReceiveTimeout)
{
//    std::cout << "CTcpCommunicationDevice::ReceiveStart 1"  << std::endl;
////    return read(m_iDeviceDescriptorClient, puiDestination, uiLength);
//    return recv(m_iDeviceDescriptorClient, (char*)puiDestination, uiLength, 0);

//    // Сделаем не блокирующим.
//    int flags = fcntl(m_iDeviceDescriptorServer, F_GETFL, 0);
//    fcntl(m_iDeviceDescriptorServer, F_SETFL, flags | O_NONBLOCK);

    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(m_iDeviceDescriptorServer, &readfds);

    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = uiReceiveTimeout;

    int ready = select(m_iDeviceDescriptorServer + 1, &readfds, NULL, NULL, &tv);

    cout << "CTcpCommunicationDevice::ReceiveStart 2" << endl;
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

        return read(m_iDeviceDescriptorClient, puiDestination, uiLength);
//    return recv(m_iDeviceDescriptorClient, (char*)puiDestination, uiLength, 0);
//        return 1;
    }
    else
    {
        cout << "CTcpCommunicationDevice::ReceiveStart 6" << endl;
        return 0;
    }


//    int rc;
//    fd_set rfds;
//    struct timeval tv;
//    struct timeval *p_tv;
//
//
//    tv.tv_sec = 0;
//    tv.tv_usec = 15000000;
//    p_tv = &tv;
//
//    /* Add a file descriptor to the set */
//    FD_ZERO(&rfds);
//    FD_SET(m_iDeviceDescriptorServer, &rfds);
//
//    if( select(m_iDeviceDescriptorServer + 1, &rfds, NULL, NULL, &tv) == -1 )
//    {
//        std::cout << "CTcpCommunicationDevice::Read timeout"  << std::endl;
//        return -1;
//    }
//    else if( FD_ISSET( m_iDeviceDescriptorClient, &rfds ) )
//    {
//        std::cout << "CTcpCommunicationDevice::Read FD_ISSET"  << std::endl;
////        if( ( rc = read(m_iDeviceDescriptorClient, (char*)puiDestination, uiLength) ) == -1 )
//        if( ( rc = recv(m_iDeviceDescriptorClient, (char*)puiDestination, uiLength, 0) ) == -1 )
//        {
//            std::cout << "CTcpCommunicationDevice::Read recv error"  << std::endl;
//            return -1;
//        }
//        else
//        {
//            if (rc)
//            {
//
////                cout << "Read" << endl;
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
//                std::cout << "CTcpCommunicationDevice::Read recv rc "  << (int)rc  << std::endl;
//                return rc;
//            }
//            else
//            {
//                std::cout << "CTcpCommunicationDevice::Read recv 0 " << std::endl;
//                return 0;
//            }
//        }
//    }
//
//    std::cout << "CTcpCommunicationDevice::Read return 0"  << std::endl;
//    return 0;
}

//-------------------------------------------------------------------------------
int16_t CTcpCommunicationDevice::ReceiveContinue(uint8_t *puiDestination,
        uint16_t uiLength,
        uint32_t uiReceiveTimeout)
{
//    std::cout << "CTcpCommunicationDevice::ReceiveContinue 1"  << std::endl;
////    return read(m_iDeviceDescriptorClient, puiDestination, uiLength);
//    return recv(m_iDeviceDescriptorClient, (char*)puiDestination, uiLength, 0);



    int rc;
    fd_set rfds;

    /* Add a file descriptor to the set */
    FD_ZERO(&rfds);
    FD_SET(m_iDeviceDescriptorClient, &rfds);

    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = uiReceiveTimeout;

    rc = select(m_iDeviceDescriptorClient + 1, &rfds, NULL, NULL, &tv);

    if (rc < 0)
    {
        if (errno == ETIMEDOUT)
        {
        cout << "CTcpCommunicationDevice::ReceiveStart ETIMEDOUT" << endl;
            return 0;
        }

        std::cout << "CTcpCommunicationDevice::ReceiveContinue timeout"  << std::endl;
        return rc;
    }
    else if( FD_ISSET( m_iDeviceDescriptorClient, &rfds ) )
    {
        std::cout << "CTcpCommunicationDevice::ReceiveContinue FD_ISSET"  << std::endl;
        rc = read(m_iDeviceDescriptorClient, (char*)puiDestination, uiLength);
//        rc = recv(m_iDeviceDescriptorClient, (char*)puiDestination, uiLength, 0);

        if (rc < 0)
        {
            std::cout << "CTcpCommunicationDevice::ReceiveContinue recv error"  << std::endl;
            return rc;
        }
        else
        {
            if (rc)
            {

//                cout << "ReceiveContinue" << endl;
//                unsigned char *pucSourceTemp;
//                pucSourceTemp = (unsigned char*)puiDestination;
//                for(int i=0; i<32; )
//                {
//                    for(int j=0; j<8; j++)
//                    {
//                        cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
//                    }
//                    cout << endl;
//                    i += 8;
//                }

                std::cout << "CTcpCommunicationDevice::ReceiveContinue recv rc "  << (int)rc  << std::endl;
                return rc;
            }
            else
            {
                std::cout << "CTcpCommunicationDevice::ReceiveContinue recv 0 " << std::endl;
                return 0;
            }
        }
    }

    std::cout << "CTcpCommunicationDevice::ReceiveContinue return 0"  << std::endl;
    return 0;
}

//-------------------------------------------------------------------------------
int16_t CTcpCommunicationDevice::Write(uint8_t *puiSource, uint16_t uiLength)
{
    SetDataIsWrited(true);
    return write(m_iDeviceDescriptorClient, puiSource, uiLength);
}

//-------------------------------------------------------------------------------
// производит обмен данными по SPI.
int CTcpCommunicationDevice::Exchange(uint8_t uiAddress,
                                      unsigned char *pucTxBuff,
                                      unsigned char *pucRxBuff,
                                      int iLength,
                                      int iSpeed)
{

}

//-------------------------------------------------------------------------------













//-------------------------------------------------------------------------------
CEthernetCommunicationDevice::CEthernetCommunicationDevice()
{
    m_pxSocketLowAddress = new struct sockaddr_ll;
}

//-------------------------------------------------------------------------------
CEthernetCommunicationDevice::~CEthernetCommunicationDevice()
{

}

//-------------------------------------------------------------------------------
void CEthernetCommunicationDevice::Init(void)
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
}

//-------------------------------------------------------------------------------
void CEthernetCommunicationDevice::SetDeviceName(const char* pccDeviceName)
{
    m_pccDeviceName = pccDeviceName;
}

//-------------------------------------------------------------------------------
const char* CEthernetCommunicationDevice::GetDeviceName(void)
{
    return m_pccDeviceName;
}

//-------------------------------------------------------------------------------
void CEthernetCommunicationDevice::SetIpAddress(const char* pccIpAddress)
{
    m_uiIpAddress = inet_addr(pccIpAddress);
}

//-------------------------------------------------------------------------------
const char* CEthernetCommunicationDevice::GetIpAddress(void)
{

}

//-------------------------------------------------------------------------------
void CEthernetCommunicationDevice::SetPort(uint16_t uiPort)
{
    m_uiPort = uiPort;
}

//-------------------------------------------------------------------------------
void CEthernetCommunicationDevice::SetDestinationMacAddress(uint8_t* puiMacAddress)
{
    memcpy(m_auiDestinationMacAddress, puiMacAddress, ETHER_ADDR_LEN);
}

//-------------------------------------------------------------------------------
uint8_t* CEthernetCommunicationDevice::GetDestnationMacAddress(void)
{
    return m_auiDestinationMacAddress;
}

//-------------------------------------------------------------------------------
void CEthernetCommunicationDevice::SetSourseMacAddress(uint8_t* puiMacAddress)
{
    memcpy(m_auiSourseMacAddress, puiMacAddress, ETHER_ADDR_LEN);
}

//-------------------------------------------------------------------------------
uint8_t* CEthernetCommunicationDevice::GetSourseMacAddress(void)
{
    return m_auiSourseMacAddress;
}

//-------------------------------------------------------------------------------
int8_t CEthernetCommunicationDevice::Open(void)
{
    /* Open PF_PACKET socket, listening for EtherType ETHER_TYPE */
    if ((m_iDeviceDescriptorServer = socket(PF_PACKET, SOCK_RAW, htons(ETHERNET_TYPE))) == -1)
    {
        perror("listener: socket");
        return -1;
    }

//    cout << "socket open ok." << endl;
//    cout << "CEthernetCommunicationDevice m_pccDeviceName " << m_pccDeviceName << endl;

    struct ifreq ifopts;	/* set promiscuous mode */
    /* Set interface to promiscuous mode - do we need to do this every time? */
    // установим имя интерфейса.
    strncpy(ifopts.ifr_name, m_pccDeviceName, strlen(m_pccDeviceName));
    // получим флаги интерфейса.
    ioctl(m_iDeviceDescriptorServer, SIOCGIFFLAGS, &ifopts);
    ifopts.ifr_flags |= IFF_PROMISC;
    // установим флаги интерфейса.
    ioctl(m_iDeviceDescriptorServer, SIOCSIFFLAGS, &ifopts);

//    // Сделаем не блокирующим.
//    int flags = fcntl(m_iDeviceDescriptorServer, F_GETFL, 0);
//    fcntl(m_iDeviceDescriptorServer, F_SETFL, flags | O_NONBLOCK);

    int sockopt;
    /* Allow the socket to be reused - incase connection is closed prematurely */
    if (setsockopt(m_iDeviceDescriptorServer, SOL_SOCKET, SO_REUSEADDR, &sockopt, sizeof sockopt) == -1)
    {
        perror("setsockopt");
        close(m_iDeviceDescriptorServer);
        return -1;
    }
//    cout << "setsockopt SO_REUSEADDR ok." << endl;
    /* Bind to device */
    if (setsockopt(m_iDeviceDescriptorServer, SOL_SOCKET, SO_BINDTODEVICE, m_pccDeviceName, strlen(m_pccDeviceName)) == -1)
    {
        perror("SO_BINDTODEVICE");
        close(m_iDeviceDescriptorServer);
        return -1;
    }
//    cout << "setsockopt SO_REUSEADDR ok." << endl;

    /* Get the MAC address of the interface to send on */
    struct ifreq if_mac;
    memset(&if_mac, 0, sizeof(struct ifreq));
    strncpy(if_mac.ifr_name, m_pccDeviceName, strlen(m_pccDeviceName));
    if (ioctl(m_iDeviceDescriptorServer, SIOCGIFHWADDR, &if_mac) < 0)
    {
        perror("SIOCGIFHWADDR");
        return -1;
    }

    SetSourseMacAddress(((uint8_t *)&if_mac.ifr_hwaddr.sa_data));
    printf("Own MAC: %x:%x:%x:%x:%x:%x\n",
           GetSourseMacAddress()[0],
           GetSourseMacAddress()[1],
           GetSourseMacAddress()[2],
           GetSourseMacAddress()[3],
           GetSourseMacAddress()[4],
           GetSourseMacAddress()[5]);

    return 0;
}

//-------------------------------------------------------------------------------
int8_t CEthernetCommunicationDevice::Listen(void)
{

    return 0;
}

//-------------------------------------------------------------------------------
int8_t CEthernetCommunicationDevice::Accept(void)
{

}

//-------------------------------------------------------------------------------
int8_t CEthernetCommunicationDevice::Connect(void)
{

    return 0;
}

////-------------------------------------------------------------------------------
//bool CEthernetCommunicationDevice::IsDataAvailable(void)
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
void CEthernetCommunicationDevice::CloseClient(void)
{
    shutdown(m_iDeviceDescriptorClient, SHUT_RDWR);
    close(m_iDeviceDescriptorClient);
}

//-------------------------------------------------------------------------------
int8_t CEthernetCommunicationDevice::Close(void)
{
    close(m_iDeviceDescriptorServer);
//    close(m_iDeviceDescriptorClient);
}

//---------------------------------------------------------------------------------
int16_t CEthernetCommunicationDevice::Read(uint8_t *puiDestination, uint16_t uiLength)
{
    int rc;
    fd_set rfds;
    struct timeval tv;
    struct timeval *p_tv;


    tv.tv_sec = 0;
    tv.tv_usec = 100000;
    p_tv = &tv;

    /* Add a file descriptor to the set */
    FD_ZERO(&rfds);
    FD_SET(m_iDeviceDescriptorServer, &rfds);

    if( select(m_iDeviceDescriptorServer + 1, &rfds, NULL, NULL, &tv) == -1 )
    {
//        std::cout << "CTcpCommunicationDevice::Read timeout"  << std::endl;
        return -1;
    }
    else if( FD_ISSET( m_iDeviceDescriptorClient, &rfds ) )
    {
//        std::cout << "CTcpCommunicationDevice::Read FD_ISSET"  << std::endl;
        if( ( rc = recv(m_iDeviceDescriptorClient, (char*)puiDestination, uiLength, 0) ) == -1 )
        {
//            std::cout << "CTcpCommunicationDevice::Read recv error"  << std::endl;
            return -1;
        }
        else
        {
            if (rc)
            {

//                cout << "Read" << endl;
//                unsigned char *pucSourceTemp;
//                pucSourceTemp = (unsigned char*)puiDestination;
//                for(int i=0; i<32; )
//                {
//                    for(int j=0; j<8; j++)
//                    {
//                        cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
//                    }
//                    cout << endl;
//                    i += 8;
//                }

//            std::cout << "CTcpCommunicationDevice::Read recv rc "  << (int)rc  << std::endl;
                return rc;
            }
            else
            {
//            std::cout << "CTcpCommunicationDevice::Read recv 0 " << std::endl;
                return 0;
            }
        }
    }

//        std::cout << "CTcpCommunicationDevice::Read return 0"  << std::endl;
    return 0;
}

////-------------------------------------------------------------------------------
//int16_t CEthernetCommunicationDevice::Read(uint8_t *puiDestination, uint16_t uiLength)
//{
//    return read(m_iDeviceDescriptorServer, puiDestination, uiLength);
//}

//-------------------------------------------------------------------------------
int16_t CEthernetCommunicationDevice::Write(uint8_t *puiSource, uint16_t uiLength)
{

//    cout << "Write" << endl;
//    unsigned char *pucSourceTemp;
//    pucSourceTemp = (unsigned char*)puiSource;
//    for(int i=0; i<32; )
//    {
//        for(int j=0; j<8; j++)
//        {
//            cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
//        }
//        cout << endl;
//        i += 8;
//    }

    SetDataIsWrited(true);
//    return write(m_iDeviceDescriptorServer, puiSource, uiLength);

    /* Send packet */
    if (sendto( m_iDeviceDescriptorServer,
                puiSource,
                uiLength,
                0,
                (struct sockaddr*)GetSocketLowAddress(),
                sizeof(struct sockaddr_ll)) < 0)
//    if (write(sockfd, buf, tx_len) < 0)
    {
//        printf("Send failed\n");
        return 1;
    }
    else
    {
//        printf("Send ok\n");
        return 1;
    }
}

//-------------------------------------------------------------------------------
// производит обмен данными по SPI.
int CEthernetCommunicationDevice::Exchange(uint8_t uiAddress,
        unsigned char *pucTxBuff,
        unsigned char *pucRxBuff,
        int iLength,
        int iSpeed)
{

}

//-------------------------------------------------------------------------------









//-------------------------------------------------------------------------------
CSpi::CSpi()
{

}

//-------------------------------------------------------------------------------
CSpi::~CSpi()
{

}

//-------------------------------------------------------------------------------
void CSpi::Init(void)
{
};

//-------------------------------------------------------------------------------
int8_t CSpi::Open(void)
{
    int iBits = BITS_PER_WORD;
    int  iMode = SPI_MODE;             //8-bits per word, SPI mode 3
//    ULONG  ulMode32 = SPI_MODE32;
    unsigned long ulSpeed;
    unsigned int nuiBusyTimeCounter;

    ulSpeed = SPEED_IN_HZ;               //Speed is 1 MHz

    // The following calls set up the SPI bus properties
    nuiBusyTimeCounter = 20;
    while (1)
    {
        if ((m_iDeviceDescriptorServer = open(SPI_PATH, O_RDWR | O_SYNC)) < 0)
        {
            perror("SPI: Can't open device.");
            CGpio::SetPin(PRD_EN_PIN_PORT, PRD_EN_PIN);
            usleep(500000);
            CGpio::ClearPin(PRD_EN_PIN_PORT, PRD_EN_PIN);
            usleep(500000);
            if (!nuiBusyTimeCounter--)
            {
                return -1;
            }
        }
        else
        {
            break;
        }
    }

    if (ioctl(m_iDeviceDescriptorServer, SPI_IOC_WR_MODE, &iMode) == -1)
    {
        perror("SPI: Can't set SPI mode.");
        return -1;
    }
    if (ioctl(m_iDeviceDescriptorServer, SPI_IOC_RD_MODE, &iMode) == -1)
    {
        perror("SPI: Can't get SPI mode.");
        return -1;
    }
//    if (ioctl(m_iDeviceDescriptorServer, SPI_IOC_WR_MODE32, &ulMode32) == -1)
//    {
//        perror("SPI: Can't set SPI mode32.");
//        return -1;
//    }
//    if (ioctl(m_iDeviceDescriptorServer, SPI_IOC_RD_MODE32, &ulMode32) == -1)
//    {
//        perror("SPI: Can't get SPI mode32.");
//        return -1;
//    }
    if (ioctl(m_iDeviceDescriptorServer, SPI_IOC_WR_BITS_PER_WORD, &iBits) == -1)
    {
        perror("SPI: Can't set bits per word.");
        return -1;
    }
    if (ioctl(m_iDeviceDescriptorServer, SPI_IOC_RD_BITS_PER_WORD, &iBits) == -1)
    {
        perror("SPI: Can't get bits per word.");
        return -1;
    }
    if (ioctl(m_iDeviceDescriptorServer, SPI_IOC_WR_MAX_SPEED_HZ, &ulSpeed) == -1)
    {
        perror("SPI: Can't set max speed HZ");
        return -1;
    }
    if (ioctl(m_iDeviceDescriptorServer, SPI_IOC_RD_MAX_SPEED_HZ, &ulSpeed) == -1)
    {
        perror("SPI: Can't get max speed HZ.");
        return -1;
    }

    // Check that the properties have been set
    printf("SPI Mode is: %d\n", iMode);
    printf("SPI Bits is: %d\n", iBits);
    printf("SPI Speed is: %d\n", (int)ulSpeed);
    return 0;
}

//-------------------------------------------------------------------------------
int8_t CSpi::Close(void)
{
    close(m_iDeviceDescriptorServer);
}

//-------------------------------------------------------------------------------
void CSpi::ChipSelectPinSet(void)
{
    CGpio::SetPinOutput(SPI_CHIP_SELECT_PIN_0_PORT, SPI_CHIP_SELECT_PIN_0);
    CGpio::SetPinOutput(SPI_CHIP_SELECT_PIN_1_PORT, SPI_CHIP_SELECT_PIN_1);
    CGpio::SetPinOutput(SPI_CHIP_SELECT_PIN_2_PORT, SPI_CHIP_SELECT_PIN_2);
    CGpio::SetPinOutput(SPI_CHIP_SELECT_PIN_3_PORT, SPI_CHIP_SELECT_PIN_3);
    CGpio::SetPinOutput(SPI_CHIP_ENABLE_PIN_PORT, SPI_CHIP_ENABLE_PIN);
    CGpio::SetPinOutput(SPI_CHIP_MIND_ENABLE_PIN_PORT, SPI_CHIP_MIND_ENABLE_PIN);

    CGpio::SetPin(SPI_CHIP_SELECT_PIN_0_PORT, SPI_CHIP_SELECT_PIN_0);
    CGpio::SetPin(SPI_CHIP_SELECT_PIN_1_PORT, SPI_CHIP_SELECT_PIN_1);
    CGpio::SetPin(SPI_CHIP_SELECT_PIN_2_PORT, SPI_CHIP_SELECT_PIN_2);
    CGpio::SetPin(SPI_CHIP_SELECT_PIN_3_PORT, SPI_CHIP_SELECT_PIN_3);
    CGpio::ClearPin(SPI_CHIP_ENABLE_PIN_PORT, SPI_CHIP_ENABLE_PIN);
    CGpio::SetPin(SPI_CHIP_MIND_ENABLE_PIN_PORT, SPI_CHIP_MIND_ENABLE_PIN);
}

//-------------------------------------------------------------------------------
void CSpi::ChipSelectPinDelete(void)
{
    CGpio::SetPinInput(SPI_CHIP_SELECT_PIN_0_PORT, SPI_CHIP_SELECT_PIN_0);
    CGpio::SetPinInput(SPI_CHIP_SELECT_PIN_1_PORT, SPI_CHIP_SELECT_PIN_1);
    CGpio::SetPinInput(SPI_CHIP_SELECT_PIN_2_PORT, SPI_CHIP_SELECT_PIN_2);
    CGpio::SetPinInput(SPI_CHIP_SELECT_PIN_3_PORT, SPI_CHIP_SELECT_PIN_3);
    CGpio::SetPinInput(SPI_CHIP_ENABLE_PIN_PORT, SPI_CHIP_ENABLE_PIN);
    CGpio::SetPinInput(SPI_CHIP_MIND_ENABLE_PIN_PORT, SPI_CHIP_MIND_ENABLE_PIN);

    CGpio::ClearPin(SPI_CHIP_SELECT_PIN_0_PORT, SPI_CHIP_SELECT_PIN_0);
    CGpio::ClearPin(SPI_CHIP_SELECT_PIN_1_PORT, SPI_CHIP_SELECT_PIN_1);
    CGpio::ClearPin(SPI_CHIP_SELECT_PIN_2_PORT, SPI_CHIP_SELECT_PIN_2);
    CGpio::ClearPin(SPI_CHIP_SELECT_PIN_3_PORT, SPI_CHIP_SELECT_PIN_3);
    CGpio::ClearPin(SPI_CHIP_ENABLE_PIN_PORT, SPI_CHIP_ENABLE_PIN);
    CGpio::ClearPin(SPI_CHIP_MIND_ENABLE_PIN_PORT, SPI_CHIP_MIND_ENABLE_PIN);
}

//-------------------------------------------------------------------------------
// массив сопоставления номера посадочного слота модуля с адресом на шине SPI.
const uint8_t CSpi::aui8ModuleSlotNumberToSpiAddressMatching[] =
{
    0x0F,
    0x07,
    0x0B,
    0x03,
    0x0D,
    0x05,
    0x09,
    0x01,
    0x0E,
    0x06,
    0x0A,
    0x02,
    0x0C,
    0x04,
    0x08,
    0x00
};

//-------------------------------------------------------------------------------
// устанавливает адрес на шине SPI. зажигает и гасит светодиод контроля обмена данными.
void CSpi::ChipSelectAddressSet(unsigned char ucAddress)
{
    unsigned char ucAddressLocal;

    CGpio::ClearPin(SPI_CHIP_ENABLE_PIN_PORT, SPI_CHIP_ENABLE_PIN);
    CGpio::SetPin(PRD_EN_PIN_PORT, PRD_EN_PIN);

    ucAddressLocal = aui8ModuleSlotNumberToSpiAddressMatching[ucAddress];

    if (ucAddressLocal & 0x01)
    {
        CGpio::SetPin(SPI_CHIP_SELECT_PIN_0_PORT, SPI_CHIP_SELECT_PIN_0);
    }
    else
    {
        CGpio::ClearPin(SPI_CHIP_SELECT_PIN_0_PORT, SPI_CHIP_SELECT_PIN_0);
    }

    ucAddressLocal = (ucAddressLocal >> 1);
    if (ucAddressLocal & 0x01)
    {
        CGpio::SetPin(SPI_CHIP_SELECT_PIN_1_PORT, SPI_CHIP_SELECT_PIN_1);
    }
    else
    {
        CGpio::ClearPin(SPI_CHIP_SELECT_PIN_1_PORT, SPI_CHIP_SELECT_PIN_1);
    }

    ucAddressLocal = (ucAddressLocal >> 1);
    if (ucAddressLocal & 0x01)
    {
        CGpio::SetPin(SPI_CHIP_SELECT_PIN_2_PORT, SPI_CHIP_SELECT_PIN_2);
    }
    else
    {
        CGpio::ClearPin(SPI_CHIP_SELECT_PIN_2_PORT, SPI_CHIP_SELECT_PIN_2);
    }

    ucAddressLocal = (ucAddressLocal >> 1);
    if (ucAddressLocal & 0x01)
    {
        CGpio::SetPin(SPI_CHIP_SELECT_PIN_3_PORT, SPI_CHIP_SELECT_PIN_3);
    }
    else
    {
        CGpio::ClearPin(SPI_CHIP_SELECT_PIN_3_PORT, SPI_CHIP_SELECT_PIN_3);
    }

    usleep(100);
    CGpio::SetPin(SPI_CHIP_ENABLE_PIN_PORT, SPI_CHIP_ENABLE_PIN);
    CGpio::ClearPin(PRD_EN_PIN_PORT, PRD_EN_PIN);
    usleep(380);
}

//-------------------------------------------------------------------------------
// производит обмен данными по SPI.
int CSpi::Exchange(uint8_t uiAddress,
                   unsigned char *pucTxBuff,
                   unsigned char *pucRxBuff,
                   int iLength,
                   int iSpeed)
{
    ChipSelectAddressSet(uiAddress);
//    memset(aucSpiTxBuffer, 0, iLength);
//    memset(aucSpiRxBuffer, 0, iLength);
//    memset(aucSpiRxBuffer, 0, 4);

    struct spi_ioc_transfer xTransfer =
    {
        //the xTransfer structure
        (unsigned long) pucTxBuff,	    // tx_buf;        //the buffer for sending data
        (unsigned long) pucRxBuff,    // rx_buf;         //the buffer for receiving data
        (unsigned long) iLength,      // len;           //the length of buffer
        (unsigned long) iSpeed,       // speed_hz;      //the speed in Hz
        (unsigned int) 0,             // delay_usecs;   //delay in us
        (unsigned char) 8             // bits_per_word; //bits per word
        //(unsigned char)               // cs_change;
        //(unsigned char)               // tx_nbits;
        //(unsigned char)               // rx_nbits;
        //(unsigned int)                // pad;

    };
    // send the SPI message (all of the above fields, inc. buffers)
    int iStatus = ioctl(m_iDeviceDescriptorServer, SPI_IOC_MESSAGE(1), &xTransfer);

    CGpio::ClearPin(SPI_CHIP_ENABLE_PIN_PORT, SPI_CHIP_ENABLE_PIN);

    if (iStatus < 0)
    {
        perror("SPI: SPI_IOC_MESSAGE Failed");
        return -1;
    }
    return iStatus;
}

//-------------------------------------------------------------------------------








////-------------------------------------------------------------------------------
int32_t CGpio::m_iDeviceDescriptorServer;
TGpioControl CGpio::m_xGpioControl;

////-------------------------------------------------------------------------------
//CGpio::CGpio()
//{
//
//}
//
////-------------------------------------------------------------------------------
//CGpio::~CGpio()
//{
//
//}

//-------------------------------------------------------------------------------
void CGpio::Init(void)
{
};

//-------------------------------------------------------------------------------
int8_t CGpio::Open(void)
{
    m_iDeviceDescriptorServer = open("/dev/mem", O_RDWR | O_SYNC);
    if(m_iDeviceDescriptorServer < 0)
    {
        printf("%s\n", strerror(errno));
        close(m_iDeviceDescriptorServer);
        return -1;
    }

    m_xGpioControl.aulGpioMmapAddresses[GPIO_PORT_0] = GPIO0_ADDR;
    m_xGpioControl.aulGpioMmapAddresses[GPIO_PORT_1] = GPIO1_ADDR;
    m_xGpioControl.aulGpioMmapAddresses[GPIO_PORT_2] = GPIO2_ADDR;
    m_xGpioControl.aulGpioMmapAddresses[GPIO_PORT_3] = GPIO3_ADDR;

    int i;
    for(i = 0; i < GPIO_PORT_QUANTITY; i++)
    {
        m_xGpioControl.apulGpioMmapPointers[i] =  (ulong*) mmap(NULL,
                0x1000,
                PROT_READ | PROT_WRITE,
                MAP_SHARED,
                m_iDeviceDescriptorServer,
                m_xGpioControl.aulGpioMmapAddresses[i]);
    }

// GPIO clock configuration
    m_xGpioControl.pulCmPerMap =  (ulong*) mmap(NULL,
                                  0x400,
                                  PROT_READ | PROT_WRITE,
                                  MAP_SHARED,
                                  m_iDeviceDescriptorServer,
                                  CM_PER);

//    *(m_xGpioControl.pulCmPerMap + (CM_PER_L4LS_CLKSTCTRL_OFFSET / 4)) |= (1 << CLKACTIVITY_GPIO_1_GDBCLK) |
//            (1 << CLKACTIVITY_GPIO_2_GDBCLK) |
//            (1 << CLKACTIVITY_GPIO_3_GDBCLK);
//
    *(m_xGpioControl.pulCmPerMap + (CM_PER_GPIO1_CLKCTRL_OFFSET / 4)) = (1 << OPTFCLKEN_GPIO_X_GDBCLK) | (1 << MODULEMODE_BIT_1);
    *(m_xGpioControl.pulCmPerMap + (CM_PER_GPIO2_CLKCTRL_OFFSET / 4)) = (1 << OPTFCLKEN_GPIO_X_GDBCLK) | (1 << MODULEMODE_BIT_1);
    *(m_xGpioControl.pulCmPerMap + (CM_PER_GPIO3_CLKCTRL_OFFSET / 4)) = (1 << OPTFCLKEN_GPIO_X_GDBCLK) | (1 << MODULEMODE_BIT_1);

    return 0;
}

//-------------------------------------------------------------------------------
int8_t CGpio::Close(void)
{
    close(m_iDeviceDescriptorServer);
}

//-------------------------------------------------------------------------------
void CGpio::SetPinOutput(unsigned char ucPortN, unsigned char ucPinN)
{
    *(m_xGpioControl.apulGpioMmapPointers[ucPortN] + (GPIO_OE_OFFSET / 4)) &= ~(1 << ucPinN);
}

//-------------------------------------------------------------------------------
void CGpio::SetPinInput(unsigned char ucPortN, unsigned char ucPinN)
{
    *(m_xGpioControl.apulGpioMmapPointers[ucPortN] + (GPIO_OE_OFFSET / 4)) |= (1 << ucPinN);
}

//-------------------------------------------------------------------------------
void CGpio::SetPin(unsigned char ucPortN, unsigned char ucPinN)
{
    *(m_xGpioControl.apulGpioMmapPointers[ucPortN] + (GPIO_DATAOUT_OFFSET / 4)) |= (1 << ucPinN);
}

//-------------------------------------------------------------------------------
void CGpio::ClearPin(unsigned char ucPortN, unsigned char ucPinN)
{
    *(m_xGpioControl.apulGpioMmapPointers[ucPortN] + (GPIO_DATAOUT_OFFSET / 4)) &= ~(1 << ucPinN);
}
//-------------------------------------------------------------------------------













//-------------------------------------------------------------------------------
CPlatform::CPlatform()
{

}

//-------------------------------------------------------------------------------
CPlatform::~CPlatform()
{

}

//-------------------------------------------------------------------------------
void CPlatform::Init(void)
{

}

//-------------------------------------------------------------------------------
uint16_t CPlatform::GetCurrentTime(void)
{
    struct timeval xCurrentTime;

    gettimeofday( &xCurrentTime, NULL );

    return static_cast<uint16_t>(((xCurrentTime.tv_sec * 1000) +
                                  (xCurrentTime.tv_usec / 1000)));
}

//-------------------------------------------------------------------------------

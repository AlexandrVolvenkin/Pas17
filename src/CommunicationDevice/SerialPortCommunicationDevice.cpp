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
#include "SerialPortCommunicationDevice.h"

using namespace std;

//-------------------------------------------------------------------------------
CSerialPortCommunicationDevice::CSerialPortCommunicationDevice()
{
    std::cout << "CSerialPortCommunicationDevice constructor"  << std::endl;
//    // получим имя класса.
//    sprintf(GetTaskNamePointer(),
//            "%s",
//            typeid(*this).name());

}
//-------------------------------------------------------------------------------
CSerialPortCommunicationDevice::~CSerialPortCommunicationDevice()
{

}

//-------------------------------------------------------------------------------
void CSerialPortCommunicationDevice::Init(void)
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

    SetFsmState(READY);
}

//-------------------------------------------------------------------------------
void CSerialPortCommunicationDevice::SetDeviceName(const char* pccDeviceName)
{
    m_pccDeviceName = pccDeviceName;
}

//-------------------------------------------------------------------------------
const char* CSerialPortCommunicationDevice::GetDeviceName(void)
{
    return m_pccDeviceName;
}

//-------------------------------------------------------------------------------
void CSerialPortCommunicationDevice::SetBaudRate(uint8_t uiBaudRate)
{
    speed_t speed;

    /* C_ISPEED     Input baud (new interface)
       C_OSPEED     Output baud (new interface)
    */
    switch (uiBaudRate)
    {
    case BIT_RATE_9600:
        speed = B9600;
        break;
    case BIT_RATE_19200:
        speed = B19200;
        break;
    case BIT_RATE_38400:
        speed = B38400;
        break;
    case BIT_RATE_57600:
        speed = B57600;
        break;
    case BIT_RATE_115200:
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
void CSerialPortCommunicationDevice::SetDataBits(uint8_t uiDataBits)
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
void CSerialPortCommunicationDevice::SetParity(uint8_t uiParity)
{
    /* PARENB       Enable parity bit
       PARODD       Use odd parity instead of even */
    if (uiParity == PARITY_NO)
    {
        /* None */
        m_xTios.c_cflag &=~ PARENB;
    }
    else if (uiParity == PARITY_EVEN)
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
    if (uiParity == PARITY_NO)
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
void CSerialPortCommunicationDevice::SetStopBit(uint8_t uiStopBit)
{
    /* Stop bit (1 or 2) */
    if (uiStopBit == 1)
    {
        m_xTios.c_cflag &= ~CSTOPB;
    }
    else /* 2 */
    {
        m_xTios.c_cflag |= CSTOPB;
    }
}

//-------------------------------------------------------------------------------
int8_t CSerialPortCommunicationDevice::Open(void)
{
    cout << "CSerialPortCommunicationDevice::Open m_pccDeviceName " << m_pccDeviceName << endl;
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

//    // Сделаем не блокирующим.
//    int flags = fcntl(m_iDeviceDescriptorServer, F_GETFL, 0);
//    fcntl(m_iDeviceDescriptorServer, F_SETFL, flags | O_NONBLOCK);

//    pto = NULL;
//    FD_ZERO(&readfds);
//    FD_ZERO(&writefds);
//    FD_SET(m_iDeviceDescriptorServer, &readfds);
}

//-------------------------------------------------------------------------------
int8_t CSerialPortCommunicationDevice::Close(void)
{
    close(m_iDeviceDescriptorServer);
}

////-------------------------------------------------------------------------------
//void CSerialPortCommunicationDevice::Reset(void)
//{
//
//}

////-------------------------------------------------------------------------------
//bool CSerialPortCommunicationDevice::IsDataAvailable(void)
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
int16_t CSerialPortCommunicationDevice::Read(uint8_t *puiDestination, uint16_t uiLength)
{
    return read(m_iDeviceDescriptorServer, puiDestination, uiLength);
}

//-------------------------------------------------------------------------------
int16_t CSerialPortCommunicationDevice::ReceiveStart(uint8_t *puiDestination,
        uint16_t uiLength,
        uint32_t uiReceiveTimeout)
{
//    std::cout << "CSerialPortCommunicationDevice::ReceiveStart 1"  << std::endl;

    int rc;
    fd_set rfds;

    /* Add a file descriptor to the set */
    FD_ZERO(&rfds);
    FD_SET(m_iDeviceDescriptorServer, &rfds);

    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = (uiReceiveTimeout * 1000);

    rc = select(m_iDeviceDescriptorServer + 1, &rfds, NULL, NULL, &tv);

    if (rc < 0)
    {
        if (errno == ETIMEDOUT)
        {
//            cout << "CSerialPortCommunicationDevice::ReceiveStart ETIMEDOUT" << endl;
            return 0;
        }

//        std::cout << "CSerialPortCommunicationDevice::ReceiveStart timeout"  << std::endl;
        return rc;
    }
    else if( FD_ISSET( m_iDeviceDescriptorServer, &rfds ) )
    {
//        std::cout << "CSerialPortCommunicationDevice::ReceiveStart FD_ISSET"  << std::endl;
        rc = read(m_iDeviceDescriptorServer, (char*)puiDestination, uiLength);
//        rc = recv(m_iDeviceDescriptorServer, (char*)puiDestination, uiLength, 0);

        if (rc < 0)
        {
//            std::cout << "CSerialPortCommunicationDevice::ReceiveStart recv error"  << std::endl;
            return rc;
        }
        else
        {
            if (rc)
            {

//                cout << "puiDestination" << endl;
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

//                std::cout << "CSerialPortCommunicationDevice::ReceiveStart recv rc "  << (int)rc  << std::endl;
                return rc;
            }
            else
            {
//                std::cout << "CSerialPortCommunicationDevice::ReceiveStart recv 0 " << std::endl;
                return -1;
            }
        }
    }
    else
    {
//        cout << "CSerialPortCommunicationDevice::ReceiveStart 6" << endl;
        return 0;
    }
}

//-------------------------------------------------------------------------------
int16_t CSerialPortCommunicationDevice::ReceiveContinue(uint8_t *puiDestination,
        uint16_t uiLength,
        uint32_t uiReceiveTimeout)
{
//    std::cout << "CSerialPortCommunicationDevice::ReceiveContinue 1"  << std::endl;

    int rc;
    fd_set rfds;

    /* Add a file descriptor to the set */
    FD_ZERO(&rfds);
    FD_SET(m_iDeviceDescriptorServer, &rfds);

    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = (uiReceiveTimeout * 1000);

    rc = select(m_iDeviceDescriptorServer + 1, &rfds, NULL, NULL, &tv);

    if (rc < 0)
    {
        if (errno == ETIMEDOUT)
        {
//            cout << "CSerialPortCommunicationDevice::ReceiveContinue ETIMEDOUT" << endl;
            return 0;
        }

//        std::cout << "CSerialPortCommunicationDevice::ReceiveContinue timeout"  << std::endl;
        return rc;
    }
    else if( FD_ISSET( m_iDeviceDescriptorServer, &rfds ) )
    {
//        std::cout << "CSerialPortCommunicationDevice::ReceiveContinue FD_ISSET"  << std::endl;
        rc = read(m_iDeviceDescriptorServer, (char*)puiDestination, uiLength);
//        rc = recv(m_iDeviceDescriptorServer, (char*)puiDestination, uiLength, 0);

        if (rc < 0)
        {
//            std::cout << "CSerialPortCommunicationDevice::ReceiveContinue recv error"  << std::endl;
            return rc;
        }
        else
        {
            if (rc)
            {

//                cout << "puiDestination" << endl;
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

//                std::cout << "CSerialPortCommunicationDevice::ReceiveContinue recv rc "  << (int)rc  << std::endl;
                return rc;
            }
            else
            {
//                std::cout << "CSerialPortCommunicationDevice::ReceiveContinue recv 0 " << std::endl;
                return -1;
            }
        }
    }
    else
    {
//        cout << "CSerialPortCommunicationDevice::ReceiveContinue 6" << endl;
        return 0;
    }
}

//-------------------------------------------------------------------------------
int16_t CSerialPortCommunicationDevice::Write(uint8_t *puiSource, uint16_t uiLength)
{
//    std::cout << "CSerialPortCommunicationDevice::Write"  << std::endl;
    return write(m_iDeviceDescriptorServer, puiSource, uiLength);
}

//-------------------------------------------------------------------------------
// производит обмен данными по SPI.
int CSerialPortCommunicationDevice::Exchange(uint8_t uiAddress,
        uint8_t *pucTxBuff,
        uint8_t *pucRxBuff,
        int iLength,
        int iSpeed)
{

}

//-------------------------------------------------------------------------------

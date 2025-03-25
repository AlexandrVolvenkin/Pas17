//-------------------------------------------------------------------------------
//  Source      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-------------------------------------------------------------------------------
#ifndef PLATFORM_H
#define PLATFORM_H

//-------------------------------------------------------------------------------
#include <iostream>
#include <random>
#include <iomanip>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>
#include <termios.h>
#include <linux/serial.h>
#include <sys/types.h>
#include <sys/select.h>
#include <unistd.h>
#include <sys/mman.h>
#include <linux/spi/spidev.h>
#include <time.h>
#include <sys/time.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>

#include <sys/socket.h>
#include <linux/if_packet.h>
#include <net/ethernet.h> /* the L2 protocols */

#if defined(__OpenBSD__) || (defined(__FreeBSD__) && __FreeBSD__ < 5)
# define OS_BSD
# include <netinet/in_systm.h>
#endif

# include <netinet/in.h>
# include <netinet/ip.h>
# include <netinet/tcp.h>
# include <arpa/inet.h>
# include <poll.h>
# include <netdb.h>
#include <net/if.h>
#include <netinet/if_ether.h>
#include <netinet/ether.h>


#include <unistd.h>

/* Include definition for RS485 ioctls: TIOCGRS485 and TIOCSRS485 */
#include <sys/ioctl.h>

#include "Configuration.h"

//#define SOCKET uint32_t

//class CTimer;

//-------------------------------------------------------------------------------
#define CM_PER 0x44E00000
#define CM_PER_L4LS_CLKSTCTRL_OFFSET 0x00
#define CLKACTIVITY_GPIO_3_GDBCLK 21
#define CLKACTIVITY_GPIO_2_GDBCLK 20
#define CLKACTIVITY_GPIO_1_GDBCLK 19

#define CM_PER_GPIO1_CLKCTRL_OFFSET 0xAC
#define CM_PER_GPIO2_CLKCTRL_OFFSET 0xB0
#define CM_PER_GPIO3_CLKCTRL_OFFSET 0xB4
#define MODULEMODE_BIT_0 0
#define MODULEMODE_BIT_1 1
#define OPTFCLKEN_GPIO_X_GDBCLK 18

#define GPIO0_ADDR 0x44E07000
#define GPIO1_ADDR 0x4804C000
#define GPIO2_ADDR 0x481AC000
#define GPIO3_ADDR 0x481AE000
#define GPIO_OE_OFFSET 0x134
#define GPIO_DATAIN_OFFSET 0x138
#define GPIO_DATAOUT_OFFSET 0x13C

#define GPIO_PORT_0 0
#define GPIO_PORT_1 1
#define GPIO_PORT_2 2
#define GPIO_PORT_3 3
#define GPIO_PORT_QUANTITY 4

#define SYSFS_GPIO_DIR "/sys/class/gpio"
//#define POLL_TIMEOUT (3 * 1000) /* 3 seconds */
#define MAX_BUF 64

enum PIN_DIRECTION
{
    INPUT_PIN=0,
    OUTPUT_PIN=1
};

enum PIN_VALUE
{
    LOW=0,
    HIGH=1
};

struct TGpioControl
{
    unsigned long *pulCmPerMap;
    unsigned long *apulGpioMmapPointers[GPIO_PORT_QUANTITY];
    unsigned long aulGpioMmapAddresses[GPIO_PORT_QUANTITY];
};

////-------------------------------------------------------------------------------
//class CGpio
//{
//public:
////    CGpio();
////    virtual ~CGpio();
//
//    static void Init(void);
//    static int8_t Open(void);
//    static int8_t Close(void);
//    static void SetPinOutput(unsigned char ucPortN, unsigned char ucPinN);
//    static void SetPinInput(unsigned char ucPortN, unsigned char ucPinN);
//    static void SetPin(unsigned char ucPortN, unsigned char ucPinN);
//    static void ClearPin(unsigned char ucPortN, unsigned char ucPinN);
//
//private:
////    const char *m_pccDeviceName;
//    /* Socket or file descriptor */
//    static int32_t m_iDeviceDescriptorServer;
//    static TGpioControl m_xGpioControl;
//
//};
////-------------------------------------------------------------------------------













////-------------------------------------------------------------------------------
//class CPlatform
//{
//public:
//    CPlatform();
//    virtual ~CPlatform();
//    static void Init(void);
//    static uint16_t GetCurrentTime(void);
//
//private:
//
//protected:
//};

//-------------------------------------------------------------------------------
#endif // PLATFORM_H

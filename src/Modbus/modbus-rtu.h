/*
 * Copyright © 2001-2011 St?phane Raimbault <stephane.raimbault@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef _MODBUS_RTU_H_
#define _MODBUS_RTU_H_

#ifndef _MSC_VER
#include <stdint.h>
#else
#include "stdint.h"
#endif

#if defined(_WIN32)
#include <windows.h>
#else
#include <termios.h>
#endif

#include "modbus.h"

//#include "../PasNewConfig.h"

//MODBUS_BEGIN_DECLS

/* Modbus_Application_Protocol_V1_1b.pdf Chapter 4 Section 1 Page 5
 * RS232 / RS485 ADU = 253 bytes + slave (1 byte) + CRC (2 bytes) = 256 bytes
 */
#define MODBUS_RTU_MAX_ADU_LENGTH  256

//#define MODBUS_RTU_35_TIMEOUT        (1000000UL / MODBUS_BAUD_RATE) * 8UL * 4UL //
#define _MODBUS_RTU_35_TIMEOUT        (1000000 / MODBUS_BAUD_RATE) * 8 * 4 //

#define _MODBUS_RTU_HEADER_LENGTH      1
#define _MODBUS_RTU_PRESET_REQ_LENGTH  6
#define _MODBUS_RTU_PRESET_RSP_LENGTH  2

#define _MODBUS_RTU_CHECKSUM_LENGTH    2


#if defined(_WIN32)
#define ENOTSUP WSAEOPNOTSUPP

/* WIN32: struct containing serial handle and a receive buffer */
#define PY_BUF_SIZE 512
struct win32_ser
{
    /* File handle */
    HANDLE fd;
    /* Receive buffer */
    unsigned char buf[PY_BUF_SIZE];
    /* Received chars */
    DWORD n_bytes;
};
#endif /* _WIN32 */

//typedef struct _modbus_rtu
//{
//    /* Device: "/dev/ttyS0", "/dev/ttyUSB0" or "/dev/tty.USA19*" on Mac OS X for
//       KeySpan USB<->Serial adapters this string had to be made bigger on OS X
//       as the directory+file name was bigger than 19 bytes. Making it 67 bytes
//       for now, but OS X does support 256 byte file names. May become a problem
//       in the future. */
//#if defined(__APPLE_CC__)
//    char device[64];
//#else
//    char device[16];
//#endif
//    /* Bauds: 9600, 19200, 57600, 115200, etc */
//    int baud;
//    /* Data bit */
//    unsigned char data_bit;
//    /* Stop bit */
//    unsigned char stop_bit;
//    /* Parity: 'N', 'O', 'E' */
//    char parity;
//#if defined(_WIN32)
//    struct win32_ser w_ser;
//    DCB old_dcb;
//#else
//    /* Save old termios settings */
//    struct termios old_tios;
//#endif
//#if HAVE_DECL_TIOCSRS485
//    int serial_mode;
//#endif
//} modbus_rtu_t;

static int _modbus_rtu_build_response_basis(sft_t *sft, unsigned char *rsp);
int _modbus_rtu_send_msg_pre(unsigned char *req, int req_length);

modbus_t* modbus_new_rtu(modbus_t *ctx, const char *device, int baud, char parity,
                         int data_bit, int stop_bit);


#define MODBUS_RTU_RS232 0
#define MODBUS_RTU_RS485 1

int modbus_rtu_set_serial_mode(modbus_t *ctx, int mode);
int modbus_rtu_get_serial_mode(modbus_t *ctx);


//MODBUS_END_DECLS

#endif /* _MODBUS_RTU_H_ */

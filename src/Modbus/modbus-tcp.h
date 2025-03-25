/*
 * Copyright © 2001-2010 St?phane Raimbault <stephane.raimbault@gmail.com>
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

#ifndef _MODBUS_TCP_H_
#define _MODBUS_TCP_H_

#include "modbus.h"

//#include "../PasNewConfig.h"

//MODBUS_BEGIN_DECLS

#if defined(_WIN32) && !defined(__CYGWIN__)
/* Win32 with MinGW, supplement to <errno.h> */
#include <winsock2.h>
#define ECONNRESET   WSAECONNRESET
#define ECONNREFUSED WSAECONNREFUSED
#define ETIMEDOUT    WSAETIMEDOUT
#define ENOPROTOOPT  WSAENOPROTOOPT
#endif

#define MODBUS_TCP_DEFAULT_PORT   502
#define MODBUS_TCP_SLAVE         0x11

/* Modbus_Application_Protocol_V1_1b.pdf Chapter 4 Section 1 Page 5
 * TCP MODBUS ADU = 253 bytes + MBAP (7 bytes) = 260 bytes
 */
#define MODBUS_TCP_MAX_ADU_LENGTH  260

#define _MODBUS_TCP_HEADER_LENGTH      7
#define _MODBUS_TCP_PRESET_REQ_LENGTH 12
#define _MODBUS_TCP_PRESET_RSP_LENGTH  8

#define _MODBUS_TCP_CHECKSUM_LENGTH    0

//typedef struct _modbus_tcp {
//    /* TCP port */
//    int port;
//    /* IP address */
//    char ip[16];
//} modbus_tcp_t;

//#define _MODBUS_TCP_PI_NODE_LENGTH    1025
//#define _MODBUS_TCP_PI_SERVICE_LENGTH   32

//typedef struct _modbus_tcp_pi {
//    /* TCP port */
//    int port;
//    /* Node */
//    char node[_MODBUS_TCP_PI_NODE_LENGTH];
//    /* Service */
//    char service[_MODBUS_TCP_PI_SERVICE_LENGTH];
//} modbus_tcp_pi_t;

modbus_t* modbus_new_tcp_rtu(modbus_t *ctx, const char *ip_address, int port); //test tcp
modbus_t* modbus_new_tcp(modbus_t *ctx, const char *ip_address, int port);
int modbus_tcp_listen(modbus_t *ctx, int nb_connection);
int modbus_tcp_accept(modbus_t *ctx, int *socket);

modbus_t* modbus_new_tcp_pi(modbus_t *ctx, const char *node, const char *service);
int modbus_tcp_pi_listen(modbus_t *ctx, int nb_connection);
int modbus_tcp_pi_accept(modbus_t *ctx, int *socket);

//MODBUS_END_DECLS

#endif /* _MODBUS_TCP_H_ */

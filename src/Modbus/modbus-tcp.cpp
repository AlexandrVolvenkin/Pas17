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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#ifndef _MSC_VER
#include <unistd.h>
#endif
#include <signal.h>
#include <sys/types.h>

#if defined(_WIN32)
# define OS_WIN32
/* ws2_32.dll has getaddrinfo and freeaddrinfo on Windows XP and later.
 * minwg32 headers check WINVER before allowing the use of these */
# ifndef WINVER
# define WINVER 0x0501
# endif
# include <ws2tcpip.h>
# define SHUT_RDWR 2
# define close closesocket
#else
# include <sys/socket.h>
# include <sys/ioctl.h>

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
#endif

#if !defined(MSG_NOSIGNAL)
#define MSG_NOSIGNAL 0
#endif

#include "modbus.h"
//#include "modbus-private.h"

#include "modbus-tcp.h"
//#include "modbus-tcp-private.h"

#include "modbus-rtu.h"
#include "modbus-rtu-private.h" //test tcp

#include "../PasNewConfig.h"

using namespace std;

#ifdef OS_WIN32
static int _modbus_tcp_init_win32(void)
{
    /* Initialise Windows Socket API */
    WSADATA wsaData;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        fprintf(stderr, "WSAStartup() returned error code %d\n",
                (unsigned int)GetLastError());
        errno = EIO;
        return -1;
    }
    return 0;
}
#endif

/* Table of CRC values for high-order byte */ //test tcp
static const unsigned char table_crc_hi[] =
{
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40
};

/* Table of CRC values for low-order byte */
static const unsigned char table_crc_lo[] =
{
    0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06,
    0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD,
    0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
    0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A,
    0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4,
    0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
    0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3,
    0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4,
    0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
    0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29,
    0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED,
    0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
    0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60,
    0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67,
    0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
    0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68,
    0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E,
    0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
    0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71,
    0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92,
    0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
    0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B,
    0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B,
    0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
    0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42,
    0x43, 0x83, 0x41, 0x81, 0x80, 0x40
};

static unsigned short crc16(unsigned char *buffer, unsigned short buffer_length) //test tcp
{
    unsigned char crc_hi = 0xFF; /* high CRC byte initialized */
    unsigned char crc_lo = 0xFF; /* low CRC byte initialized */
    unsigned int i; /* will index into CRC lookup */

    /* pass through message buffer */
    while (buffer_length--)
    {
        i = crc_hi ^ *buffer++; /* calculate the CRC  */
        crc_hi = crc_lo ^ table_crc_hi[i];
        crc_lo = table_crc_lo[i];
    }

    return (crc_hi << 8 | crc_lo);
}


static int _modbus_set_slave(modbus_t *ctx, int slave)
{
    /* Broadcast address is 0 (MODBUS_BROADCAST_ADDRESS) */
    if (slave >= 0 && slave <= 247)
    {
        ctx->slave = slave;
    }
    else if (slave == MODBUS_TCP_SLAVE)
    {
        /* The special value MODBUS_TCP_SLAVE (0xFF) can be used in TCP mode to
         * restore the default value. */
        ctx->slave = slave;
    }
    else
    {
        errno = EINVAL;
        return -1;
    }

    return 0;
}

/* Builds a TCP request header */
int _modbus_tcp_build_request_basis(modbus_t *ctx, int function,
                                    int addr, int nb,
                                    unsigned char *req)
{

    /* Extract from MODBUS Messaging on TCP/IP Implementation Guide V1.0b
       (page 23/46):
       The transaction identifier is used to associate the future response
       with the request. So, at a time, on a TCP connection, this identifier
       must be unique. */
    static unsigned short t_id = 0;

    /* Transaction ID */
    if (t_id < UINT16_MAX)
        t_id++;
    else
        t_id = 0;
    req[0] = t_id >> 8;
    req[1] = t_id & 0x00ff;

    /* Protocol Modbus */
    req[2] = 0;
    req[3] = 0;

    /* Length will be defined later by set_req_length_tcp at offsets 4
       and 5 */

    req[6] = ctx->slave;
    req[7] = function;
    req[8] = addr >> 8;
    req[9] = addr & 0x00ff;
    req[10] = nb >> 8;
    req[11] = nb & 0x00ff;

    return _MODBUS_TCP_PRESET_REQ_LENGTH;
}

/* Builds a TCP response header */
int _modbus_tcp_build_response_basis(sft_t *sft, unsigned char *rsp) //test tcp
{
    /* Extract from MODBUS Messaging on TCP/IP Implementation
       Guide V1.0b (page 23/46):
       The transaction identifier is used to associate the future
       response with the request. */
    rsp[0] = sft->t_id >> 8;
    rsp[1] = sft->t_id & 0x00ff;

    /* Protocol Modbus */
    rsp[2] = 0;
    rsp[3] = 0;

    /* Length will be set later by send_msg (4 and 5) */

    /* The slave ID is copied from the indication */
    rsp[6] = sft->slave;
    rsp[7] = sft->function;

    return _MODBUS_TCP_PRESET_RSP_LENGTH;

    /* In this case, the slave is certainly valid because a check is already
     * done in _modbus_rtu_listen */
//    rsp[0] = sft->slave;
//    rsp[1] = sft->function;
//
//    return _MODBUS_RTU_PRESET_RSP_LENGTH;
}

/* Builds a RTU response header */
static int _modbus_tcp_rtu_build_response_basis(sft_t *sft, unsigned char *rsp) //test tcp
{
    /* In this case, the slave is certainly valid because a check is already
     * done in _modbus_rtu_listen */
    rsp[0] = sft->slave;
    rsp[1] = sft->function;

    return _MODBUS_RTU_PRESET_RSP_LENGTH;
}


int _modbus_tcp_prepare_response_tid(const unsigned char *req, int *req_length)
{
    return (req[0] << 8) + req[1];
}

int _modbus_tcp_send_msg_pre(unsigned char *req, int req_length)
{
    /* Substract the header length to the message length */
    int mbap_length = req_length - 6;

    req[4] = mbap_length >> 8;
    req[5] = mbap_length & 0x00FF;

    return req_length;

//    unsigned short crc = crc16(req, req_length); //test tcp
//    req[req_length++] = crc >> 8;
//    req[req_length++] = crc & 0x00FF;
//
//    return req_length;
}

int _modbus_tcp_rtu_send_msg_pre(unsigned char *req, int req_length) //test tcp
{
    unsigned short crc = crc16(req, req_length);
    req[req_length++] = crc >> 8;
    req[req_length++] = crc & 0x00FF;

    return req_length;
}

ssize_t _modbus_tcp_send(modbus_t *ctx, const unsigned char *req, int req_length)
{
    /* MSG_NOSIGNAL
       Requests not to send SIGPIPE on errors on stream oriented
       sockets when the other end breaks the connection.  The EPIPE
       error is still returned. */
    return send(ctx->s, (const char*)req, req_length, MSG_NOSIGNAL);
}

ssize_t _modbus_tcp_recv(modbus_t *ctx, unsigned char *rsp, int rsp_length)
{
    return recv(ctx->s, (char *)rsp, rsp_length, 0);
}

int _modbus_tcp_check_integrity(modbus_t *ctx, unsigned char *msg, const int msg_length)
{

//    cout << "_modbus_tcp_check_integrity msg" << endl;
//    unsigned char *pucSourceTemp;
//    pucSourceTemp = (unsigned char*)msg;
//    for(int i=0; i<64; )
//    {
//        for(int j=0; j<8; j++)
//        {
//            cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
//        }
//        cout << endl;
//        i += 8;
//    }

    return msg_length;
}

int _modbus_tcp_pre_check_confirmation(modbus_t *ctx, const unsigned char *req,
                                       const unsigned char *rsp, int rsp_length)
{
    /* Check TID */
    if (req[0] != rsp[0] || req[1] != rsp[1])
    {
        if (ctx->debug)
        {
            fprintf(stderr, "Invalid TID received 0x%X (not 0x%X)\n",
                    (rsp[0] << 8) + rsp[1], (req[0] << 8) + req[1]);
        }
        errno = EMBBADDATA;
        return -1;
    }
    else
    {
        return 0;
    }
}

static int _modbus_tcp_set_ipv4_options(int s)
{
    int rc;
    int option;

    /* Set the TCP no delay flag */
    /* SOL_TCP = IPPROTO_TCP */
    option = 1;
    rc = setsockopt(s, IPPROTO_TCP, TCP_NODELAY,
                    (const void *)&option, sizeof(int));
    if (rc == -1)
    {
        return -1;
    }

#ifndef OS_WIN32
    /**
     * Cygwin defines IPTOS_LOWDELAY but can't handle that flag so it's
     * necessary to workaround that problem.
     **/
    /* Set the IP low delay option */
    option = IPTOS_LOWDELAY;
    rc = setsockopt(s, IPPROTO_IP, IP_TOS,
                    (const void *)&option, sizeof(int));
    if (rc == -1)
    {
        return -1;
    }
#endif

    return 0;
}

/* Establishes a modbus TCP connection with a Modbus server. */
static int _modbus_tcp_connect(modbus_t *ctx)
{
    int rc;
    struct sockaddr_in addr;
    //ctx_ modbus_tcp_t *ctx_tcp = (modbus_tcp_t*)ctx->backend_data;

#ifdef OS_WIN32
    if (_modbus_tcp_init_win32() == -1)
    {
        return -1;
    }
#endif

    ctx->s = socket(PF_INET, SOCK_STREAM, 0);
    if (ctx->s == -1)
    {
        return -1;
    }

    rc = _modbus_tcp_set_ipv4_options(ctx->s);
    if (rc == -1)
    {
        close(ctx->s);
        return -1;
    }

    if (ctx->debug)
    {
        printf("Connecting to %s\n", ctx->ip);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(ctx->port);
    addr.sin_addr.s_addr = inet_addr(ctx->ip);
//    cout << "CStorageDeviceFileSystem::Write pccIpAddress" << " " << ctx->ip << endl;
//    cout << "CStorageDeviceFileSystem::Write address" << " " << (int)inet_addr(ctx->ip) << endl;
    rc = connect(ctx->s, (struct sockaddr *)&addr,
                 sizeof(struct sockaddr_in));
    if (rc == -1)
    {
        close(ctx->s);
        return -1;
    }

    return 0;
}

/* Establishes a modbus TCP PI connection with a Modbus server. */
static int _modbus_tcp_pi_connect(modbus_t *ctx)
{
    int rc;
    struct addrinfo *ai_list;
    struct addrinfo *ai_ptr;
    struct addrinfo ai_hints;
    //ctx_ modbus_tcp_pi_t *ctx_tcp_pi = (modbus_tcp_pi_t*)ctx->backend_data;

#ifdef OS_WIN32
    if (_modbus_tcp_init_win32() == -1)
    {
        return -1;
    }
#endif

    memset(&ai_hints, 0, sizeof(ai_hints));
#ifdef AI_ADDRCONFIG
    ai_hints.ai_flags |= AI_ADDRCONFIG;
#endif
    ai_hints.ai_family = AF_UNSPEC;
    ai_hints.ai_socktype = SOCK_STREAM;
    ai_hints.ai_addr = NULL;
    ai_hints.ai_canonname = NULL;
    ai_hints.ai_next = NULL;

    ai_list = NULL;
    rc = getaddrinfo(ctx->node, ctx->service,
                     &ai_hints, &ai_list);
    if (rc != 0)
        return rc;

    for (ai_ptr = ai_list; ai_ptr != NULL; ai_ptr = ai_ptr->ai_next)
    {
        int s;

        s = socket(ai_ptr->ai_family, ai_ptr->ai_socktype, ai_ptr->ai_protocol);
        if (s < 0)
            continue;

        if (ai_ptr->ai_family == AF_INET)
            _modbus_tcp_set_ipv4_options(s);

        rc = connect(s, ai_ptr->ai_addr, ai_ptr->ai_addrlen);
        if (rc != 0)
        {
            close(s);
            continue;
        }

        ctx->s = s;
        break;
    }

    freeaddrinfo(ai_list);

    if (ctx->s < 0)
    {
        return -1;
    }

    return 0;
}

/* Closes the network connection and socket in TCP mode */
void _modbus_tcp_close(modbus_t *ctx)
{
    shutdown(ctx->s, SHUT_RDWR);
    close(ctx->s);
}

int _modbus_tcp_flush(modbus_t *ctx)
{
    int rc;
    int rc_sum = 0;

    do
    {
        /* Extract the garbage from the socket */
        char devnull[MODBUS_TCP_MAX_ADU_LENGTH];
#ifndef OS_WIN32
        rc = recv(ctx->s, devnull, MODBUS_TCP_MAX_ADU_LENGTH, MSG_DONTWAIT);
#else
        /* On Win32, it's a bit more complicated to not wait */
        fd_set rfds;
        struct timeval tv;

        tv.tv_sec = 0;
        tv.tv_usec = 0;
        FD_ZERO(&rfds);
        FD_SET(ctx->s, &rfds);
        rc = select(ctx->s+1, &rfds, NULL, NULL, &tv);
        if (rc == -1)
        {
            return -1;
        }

        if (rc == 1)
        {
            /* There is data to flush */
            rc = recv(ctx->s, devnull, MODBUS_TCP_MAX_ADU_LENGTH, 0);
        }
#endif
        if (rc > 0)
        {
            rc_sum += rc;
        }
    }
    while (rc == MODBUS_TCP_MAX_ADU_LENGTH);

    return rc_sum;
}

/* Listens for any request from one or many modbus masters in TCP */
int modbus_tcp_listen(modbus_t *ctx, int nb_connection)
{
    int new_socket;
    int yes;
    struct sockaddr_in addr;
    //ctx_ modbus_tcp_t *ctx_tcp = (modbus_tcp_t*)ctx->backend_data;

#ifdef OS_WIN32
    if (_modbus_tcp_init_win32() == -1)
    {
        return -1;
    }
#endif

    new_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (new_socket == -1)
    {
        return -1;
    }

    yes = 1;
    if (setsockopt(new_socket, SOL_SOCKET, SO_REUSEADDR,
                   (char *) &yes, sizeof(yes)) == -1)
    {
        close(new_socket);
        return -1;
    }

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    /* If the modbus port is < to 1024, we need the setuid root. */
    addr.sin_port = htons(ctx->port);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(new_socket, (struct sockaddr *)&addr, sizeof(addr)) == -1)
    {
        close(new_socket);
        return -1;
    }

    if (listen(new_socket, nb_connection) == -1)
    {
        close(new_socket);
        return -1;
    }

    return new_socket;
}

int modbus_tcp_pi_listen(modbus_t *ctx, int nb_connection)
{
    int rc;
    struct addrinfo *ai_list;
    struct addrinfo *ai_ptr;
    struct addrinfo ai_hints;
    const char *node;
    const char *service;
    int new_socket;
    //ctx_ modbus_tcp_pi_t *ctx_tcp_pi = (modbus_tcp_pi_t*)ctx->backend_data;

    if (ctx->node[0] == 0)
        node = NULL; /* == any */
    else
        node = ctx->node;

    if (ctx->service[0] == 0)
        service = "502";
    else
        service = ctx->service;

    memset(&ai_hints, 0, sizeof (ai_hints));
    ai_hints.ai_flags |= AI_PASSIVE;
#ifdef AI_ADDRCONFIG
    ai_hints.ai_flags |= AI_ADDRCONFIG;
#endif
    ai_hints.ai_family = AF_UNSPEC;
    ai_hints.ai_socktype = SOCK_STREAM;
    ai_hints.ai_addr = NULL;
    ai_hints.ai_canonname = NULL;
    ai_hints.ai_next = NULL;

    ai_list = NULL;
    rc = getaddrinfo(node, service, &ai_hints, &ai_list);
    if (rc != 0)
        return -1;

    new_socket = -1;
    for (ai_ptr = ai_list; ai_ptr != NULL; ai_ptr = ai_ptr->ai_next)
    {
        int s;

        s = socket(ai_ptr->ai_family, ai_ptr->ai_socktype,
                   ai_ptr->ai_protocol);
        if (s < 0)
        {
            if (ctx->debug)
            {
                perror("socket");
            }
            continue;
        }
        else
        {
            int yes = 1;
            rc = setsockopt(s, SOL_SOCKET, SO_REUSEADDR,
                            (void *) &yes, sizeof (yes));
            if (rc != 0)
            {
                close(s);
                if (ctx->debug)
                {
                    perror("setsockopt");
                }
                continue;
            }
        }

        rc = bind(s, ai_ptr->ai_addr, ai_ptr->ai_addrlen);
        if (rc != 0)
        {
            close(s);
            if (ctx->debug)
            {
                perror("bind");
            }
            continue;
        }

        rc = listen(s, nb_connection);
        if (rc != 0)
        {
            close(s);
            if (ctx->debug)
            {
                perror("listen");
            }
            continue;
        }

        new_socket = s;
        break;
    }
    freeaddrinfo(ai_list);

    if (new_socket < 0)
    {
        return -1;
    }

    return new_socket;
}

/* On success, the function return a non-negative integer that is a descriptor
   for the accepted socket. On error, -1 is returned, and errno is set
   appropriately. */
int modbus_tcp_accept(modbus_t *ctx, int *socket)
{
    struct sockaddr_in addr;
    socklen_t addrlen;

    addrlen = sizeof(addr);
    ctx->s = accept(*socket, (struct sockaddr *)&addr, &addrlen);
    if (ctx->s == -1)
    {
        close(*socket);
        *socket = 0;
        return -1;
    }

    if (ctx->debug)
    {
        printf("The client connection from %s is accepted\n",
               inet_ntoa(addr.sin_addr));
    }

    return ctx->s;
}

int modbus_tcp_pi_accept(modbus_t *ctx, int *socket)
{
    struct sockaddr_storage addr;
    socklen_t addrlen;

    addrlen = sizeof(addr);
    ctx->s = (int)accept(*socket, (struct sockaddr *)&addr, &addrlen);
    if (ctx->s == -1)
    {
        close(*socket);
        *socket = 0;
    }

    if (ctx->debug)
    {
        printf("The client connection is accepted.\n");
    }

    return ctx->s;
}

int _modbus_tcp_select(modbus_t *ctx, fd_set *rfds, struct timeval *tv, int length_to_read)
{
    int s_rc;
    while ((s_rc = select(ctx->s+1, rfds, NULL, NULL, tv)) == -1)
    {
        cout << "while modbus_receive tcp select errno == EINTR" << endl;//?
        if (errno == EINTR)
        {
            if (ctx->debug)
            {
                fprintf(stderr, "A non blocked signal was caught\n");
            }
            /* Necessary after an error */
            FD_ZERO(rfds);
            FD_SET(ctx->s, rfds);
            if (pxSlaveTcpCtx == ctx)
            {
                cout << "modbus_receive tcp select errno == EINTR" << endl;//?
            }
            return -1;
        }
        else
        {
            return -1;
        }
    }

    if (s_rc == 0)
    {
        errno = ETIMEDOUT;
        return -1;
    }

    return s_rc;
}

int _modbus_tcp_filter_request(modbus_t *ctx, int slave)
{
    return 0;
}

const modbus_backend_t _modbus_tcp_backend =
{
    _MODBUS_BACKEND_TYPE_TCP,
    _MODBUS_TCP_HEADER_LENGTH,
    _MODBUS_TCP_CHECKSUM_LENGTH,
    MODBUS_TCP_MAX_ADU_LENGTH,
    _modbus_set_slave,
    _modbus_tcp_build_request_basis,
    _modbus_tcp_build_response_basis,
    _modbus_tcp_prepare_response_tid,
    _modbus_tcp_send_msg_pre,
    _modbus_tcp_send,
    _modbus_tcp_recv,
    _modbus_tcp_check_integrity,
    _modbus_tcp_pre_check_confirmation,
    _modbus_tcp_connect,
    _modbus_tcp_close,
    _modbus_tcp_flush,
    _modbus_tcp_select,
    _modbus_tcp_filter_request
};

const modbus_backend_t _modbus_tcp_pi_backend =
{
    _MODBUS_BACKEND_TYPE_TCP,
    _MODBUS_TCP_HEADER_LENGTH,
    _MODBUS_TCP_CHECKSUM_LENGTH,
    MODBUS_TCP_MAX_ADU_LENGTH,
    _modbus_set_slave,
    _modbus_tcp_build_request_basis,
    _modbus_tcp_build_response_basis,
    _modbus_tcp_prepare_response_tid,
    _modbus_tcp_send_msg_pre,
    _modbus_tcp_send,
    _modbus_tcp_recv,
    _modbus_tcp_check_integrity,
    _modbus_tcp_pre_check_confirmation,
    _modbus_tcp_pi_connect,
    _modbus_tcp_close,
    _modbus_tcp_flush,
    _modbus_tcp_select,
    _modbus_tcp_filter_request
};

const modbus_backend_t _modbus_tcp_rtu_backend =
{
    _MODBUS_BACKEND_TYPE_TCP,
    _MODBUS_RTU_HEADER_LENGTH,
    _MODBUS_RTU_CHECKSUM_LENGTH,
    MODBUS_TCP_MAX_ADU_LENGTH,
    _modbus_set_slave,
    _modbus_tcp_build_request_basis,
    _modbus_tcp_rtu_build_response_basis,
    _modbus_tcp_prepare_response_tid,
    _modbus_tcp_rtu_send_msg_pre,
    _modbus_tcp_send,
    _modbus_tcp_recv,
    _modbus_tcp_check_integrity,
    _modbus_tcp_pre_check_confirmation,
    _modbus_tcp_connect,
    _modbus_tcp_close,
    _modbus_tcp_flush,
    _modbus_tcp_select,
    _modbus_tcp_filter_request

//
//    _MODBUS_BACKEND_TYPE_TCP,
//    _MODBUS_RTU_HEADER_LENGTH,
//    _MODBUS_RTU_CHECKSUM_LENGTH,
//    MODBUS_RTU_MAX_ADU_LENGTH,
//    _modbus_set_slave,
//    _modbus_rtu_build_request_basis,
//    _modbus_rtu_build_response_basis,
//    _modbus_rtu_prepare_response_tid,
//    _modbus_rtu_send_msg_pre,
//    _modbus_rtu_send,
//    _modbus_rtu_recv,
//    _modbus_rtu_check_integrity,
//    NULL,
//    _modbus_rtu_connect,
//    _modbus_rtu_close,
//    _modbus_rtu_flush,
//    _modbus_rtu_select,
//    _modbus_rtu_filter_request
};

modbus_t* modbus_new_tcp_rtu(modbus_t *ctx, const char *ip, int port) //test tcp
{
    //ctx_ modbus_t *ctx;
    //ctx_ modbus_tcp_t *ctx_tcp;
    size_t dest_size;
    size_t ret_size;

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

    //ctx = (modbus_t *) malloc(sizeof(modbus_t));

    _modbus_init_common(ctx);

    /* Could be changed after to reach a remote serial Modbus device */
    ctx->slave = MODBUS_TCP_SLAVE;

//    ctx->backend = &(_modbus_tcp_backend);
    ctx->backend = &(_modbus_tcp_rtu_backend); //test tcp

    //ctx_ ctx->backend_data = (modbus_tcp_t *) malloc(sizeof(modbus_tcp_t));
    //ctx_ ctx_tcp = (modbus_tcp_t *)ctx->backend_data;

    dest_size = sizeof(char) * 16;
    ret_size = strlcpy(ctx->ip, ip, dest_size);
    if (ret_size == 0)
    {
        fprintf(stderr, "The IP string is empty\n");
        modbus_free(ctx);
        errno = EINVAL;
        return NULL;
    }

    if (ret_size >= dest_size)
    {
        fprintf(stderr, "The IP string has been truncated\n");
        modbus_free(ctx);
        errno = EINVAL;
        return NULL;
    }

    ctx->port = port;

    return ctx;
}

modbus_t* modbus_new_tcp(modbus_t *ctx, const char *ip, int port)
{
    //ctx_ modbus_t *ctx;
    //ctx_ modbus_tcp_t *ctx_tcp;
    size_t dest_size;
    size_t ret_size;

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

    //ctx = (modbus_t *) malloc(sizeof(modbus_t));

    _modbus_init_common(ctx);

    /* Could be changed after to reach a remote serial Modbus device */
    ctx->slave = MODBUS_TCP_SLAVE;

    ctx->backend = &(_modbus_tcp_backend);


    //ctx_ ctx->backend_data = (modbus_tcp_t *) malloc(sizeof(modbus_tcp_t));
    //ctx_ ctx_tcp = (modbus_tcp_t *)ctx->backend_data;

    dest_size = sizeof(char) * 16;
    ret_size = strlcpy(ctx->ip, ip, dest_size);
    if (ret_size == 0)
    {
        fprintf(stderr, "The IP string is empty\n");
        modbus_free(ctx);
        errno = EINVAL;
        return NULL;
    }

    if (ret_size >= dest_size)
    {
        fprintf(stderr, "The IP string has been truncated\n");
        modbus_free(ctx);
        errno = EINVAL;
        return NULL;
    }

    ctx->port = port;

    return ctx;
}


modbus_t* modbus_new_tcp_pi(modbus_t *ctx, const char *node, const char *service)
{
    //ctx_ modbus_t *ctx;
    //ctx_ modbus_tcp_pi_t *ctx_tcp_pi;
    size_t dest_size;
    size_t ret_size;

    //ctx = (modbus_t *) malloc(sizeof(modbus_t));

    _modbus_init_common(ctx);

    /* Could be changed after to reach a remote serial Modbus device */
    ctx->slave = MODBUS_TCP_SLAVE;

    ctx->backend = &(_modbus_tcp_pi_backend);

    //ctx_ ctx->backend_data = (modbus_tcp_pi_t *) malloc(sizeof(modbus_tcp_pi_t));
    //ctx_ ctx_tcp_pi = (modbus_tcp_pi_t *)ctx->backend_data;

    dest_size = sizeof(char) * _MODBUS_TCP_PI_NODE_LENGTH;
    ret_size = strlcpy(ctx->node, node, dest_size);
    if (ret_size == 0)
    {
        fprintf(stderr, "The node string is empty\n");
        modbus_free(ctx);
        errno = EINVAL;
        return NULL;
    }

    if (ret_size >= dest_size)
    {
        fprintf(stderr, "The node string has been truncated\n");
        modbus_free(ctx);
        errno = EINVAL;
        return NULL;
    }

    dest_size = sizeof(char) * _MODBUS_TCP_PI_SERVICE_LENGTH;
    ret_size = strlcpy(ctx->service, service, dest_size);
    if (ret_size == 0)
    {
        fprintf(stderr, "The service string is empty\n");
        modbus_free(ctx);
        errno = EINVAL;
        return NULL;
    }

    if (ret_size >= dest_size)
    {
        fprintf(stderr, "The service string has been truncated\n");
        modbus_free(ctx);
        errno = EINVAL;
        return NULL;
    }

    return ctx;
}

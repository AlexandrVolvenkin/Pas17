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
 *
 *
 * This library implements the Modbus protocol.
 * http://libmodbus.org/
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <time.h>
//#include <config.h>
#include <sys/time.h>

#include "modbus.h"
//#include "modbus-private.h"

using namespace std;

uint8_t nui8FrameCounter = 0; // modbus mind.

/* Internal use */
#define MSG_LENGTH_UNDEFINED -1

/* Exported version */
const unsigned int libmodbus_version_major = LIBMODBUS_VERSION_MAJOR;
const unsigned int libmodbus_version_minor = LIBMODBUS_VERSION_MINOR;
const unsigned int libmodbus_version_micro = LIBMODBUS_VERSION_MICRO;

/* 3 steps are used to parse the query */
typedef enum
{
    _STEP_FUNCTION,
    _STEP_META,
    _STEP_DATA
} _step_t;

const char accErrorString[10] = "SPerror.";

const char *modbus_strerror(int errnum)
{
    switch (errnum)
    {
    case EMBXILFUN:
        return "Illegal function";
    case EMBXILADD:
        return "Illegal data address";
    case EMBXILVAL:
        return "Illegal data value";
    case EMBXSFAIL:
        return "Slave device or server failure";
    case EMBXACK:
        return "Acknowledge";
    case EMBXSBUSY:
        return "Slave device or server is busy";
    case EMBXNACK:
        return "Negative acknowledge";
    case EMBXMEMPAR:
        return "Memory parity error";
    case EMBXGPATH:
        return "Gateway path unavailable";
    case EMBXGTAR:
        return "Target device failed to respond";
    case EMBBADCRC:
        return "Invalid CRC";
    case EMBBADDATA:
        return "Invalid data";
    case EMBBADEXC:
        return "Invalid exception code";
    case EMBMDATA:
        return "Too many data";
    default:
        return strerror(errnum);
    }
}

void _error_print(modbus_t *ctx, const char *context)
{
    if (ctx->debug)
    {
        fprintf(stderr, "ERROR %s", modbus_strerror(errno));
        if (context != NULL)
        {
            fprintf(stderr, ": %s\n", context);
        }
        else
        {
            fprintf(stderr, "\n");
        }
    }
}

int _sleep_and_flush(modbus_t *ctx)
{
#ifdef _WIN32
    /* usleep doesn't exist on Windows */
    Sleep((ctx->response_timeout.tv_sec * 1000) +
          (ctx->response_timeout.tv_usec / 1000));
#else
    /* usleep Source code */
    struct timespec request, remaining;
    request.tv_sec = ctx->response_timeout.tv_sec;
    request.tv_nsec = ((long int)ctx->response_timeout.tv_usec % 1000000)
                      * 1000;
    while (nanosleep(&request, &remaining) == -1 && errno == EINTR)
        request = remaining;
#endif
    return modbus_flush(ctx);
}

int modbus_flush(modbus_t *ctx)
{
    int rc = ctx->backend->flush(ctx);
    if (rc != -1 && ctx->debug)
    {
        printf("%d bytes flushed\n", rc);
    }
    return rc;
}

/* Computes the length of the expected response */
static unsigned int compute_response_length_from_request(modbus_t *ctx, unsigned char *req, unsigned char *rsp)
{
    int length;
    const int offset = ctx->backend->header_length;

    switch (req[offset])
    {
    case _FC_READ_COILS:
    case _FC_READ_DISCRETE_INPUTS:
    {
        /* Header + nb values (code from write_bits) */
        int nb = (req[offset + 3] << 8) | req[offset + 4];
        length = 2 + (nb / 8) + ((nb % 8) ? 1 : 0);
    }
    break;
    case _FC_WRITE_AND_READ_REGISTERS:
    case _FC_READ_HOLDING_REGISTERS:
    case _FC_READ_INPUT_REGISTERS:
        /* Header + 2 * nb values */
        length = 2 + 2 * (req[offset + 3] << 8 | req[offset + 4]);
        break;
    case _FC_READ_EXCEPTION_STATUS:
        length = 3;
        break;
    case _FC_REPORT_SLAVE_ID:
        /* The response is device specific (the header provides the
           length) */
        return MSG_LENGTH_UNDEFINED;
    case _FC_DATA_EXCHANGE:
        /* Header + 2 * nb values */
        length = 2 + (rsp[offset + 1]);
        break;
    default:
        length = 5;
    }

    return offset + length + ctx->backend->checksum_length;
}

/* Sends a request/response */
static int send_msg(modbus_t *ctx, unsigned char *msg, int msg_length)
{
    int rc;
    int i;

    CGpio::SetPin(ctx->led_port, ctx->led_pin);
    usleep(5000);
    CGpio::ClearPin(ctx->led_port, ctx->led_pin);

//    CGpio::SetPin(ctx->led_port, ctx->led_pin);


//    cout << "modbus rsp" << endl;
//    unsigned char *pucSourceTemp;
//    pucSourceTemp = (unsigned char*)msg;
//    for(int i=0; i<32; )
//    {
//        for(int j=0; j<8; j++)
//        {
//            cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
//        }
//        cout << endl;
//        i += 8;
//    }

    msg_length = ctx->backend->send_msg_pre(msg, msg_length);

    if (ctx->debug)
    {
        for (i = 0; i < msg_length; i++)
            printf("[%.2X]", msg[i]);
        printf("\n");
    }


//
//    if (pxSlaveTcpCtx == ctx)
////    if (pxSlaveRtuCtx == ctx)
//    {
//        cout << "msg_length" << endl;
//        cout << "0x" << hex << uppercase << setw(2) << setfill('0') << (unsigned int)(msg_length) << " " << endl;
//
//        cout << "send_msg msg" << endl;
//        unsigned char *pucSourceTemp;
//        pucSourceTemp = (unsigned char*)msg;
//        for(int i=0; i<32; )
//        {
//            for(int j=0; j<8; j++)
//            {
//                cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
//            }
//            cout << endl;
//            i += 8;
//        }
//    }


    /* In recovery mode, the write command will be issued until to be
       successful! Disabled by default. */
    do
    {
        rc = ctx->backend->send(ctx, msg, msg_length);
        if (rc == -1)
        {
//            cout << "_error_print" << endl;
            _error_print(ctx, NULL);
            if (ctx->error_recovery & MODBUS_ERROR_RECOVERY_LINK)
            {
                int saved_errno = errno;

                if ((errno == EBADF || errno == ECONNRESET || errno == EPIPE))
                {
                    modbus_close(ctx);
                    modbus_connect(ctx);
                }
                else
                {
                    _sleep_and_flush(ctx);
                }
                errno = saved_errno;
            }
        }
    }
    while ((ctx->error_recovery & MODBUS_ERROR_RECOVERY_LINK) &&
            rc == -1);
//    CGpio::ClearPin(ctx->led_port, ctx->led_pin);

    if (rc > 0 && rc != msg_length)
    {
//        cout << "EMBBADDATA" << endl;
        errno = EMBBADDATA;
        return -1;
    }
//cout << "send ok" << endl;
    return rc;
}

int modbus_send_raw_request(modbus_t *ctx, unsigned char *raw_req, int raw_req_length)
{
    sft_t sft;
    unsigned char req[MAX_MESSAGE_LENGTH];
    int req_length;

    if (raw_req_length < 2)
    {
        /* The raw request must contain function and slave at least */
        errno = EINVAL;
        return -1;
    }

    sft.slave = raw_req[0];
    sft.function = raw_req[1];
    /* The t_id is left to zero */
    sft.t_id = 0;
    /* This response function only set the header so it's convenient here */
    req_length = ctx->backend->build_response_basis(&sft, req);

    if (raw_req_length > 2)
    {
        /* Copy data after function code */
        memcpy(req + req_length, raw_req + 2, raw_req_length - 2);
        req_length += raw_req_length - 2;
    }

    return send_msg(ctx, req, req_length);
}

/*
    ---------- Request     Indication ----------
    | Client | ---------------------->| Server |
    ---------- Confirmation  Response ----------
*/

typedef enum
{
    /* Request message on the server side */
    MSG_INDICATION,
    /* Request message on the client side */
    MSG_CONFIRMATION
} msg_type_t;

/* Computes the length to read after the function received */
static unsigned char compute_meta_length_after_function(int function,
        msg_type_t msg_type)
{
    int length;

    if (msg_type == MSG_INDICATION)
    {
        if (function <= _FC_WRITE_SINGLE_REGISTER)
        {
            length = 4;
        }
        else if (function == _FC_WRITE_MULTIPLE_COILS ||
                 function == _FC_WRITE_MULTIPLE_REGISTERS)
        {
            length = 5;
        }
        else if (function == _FC_WRITE_AND_READ_REGISTERS)
        {
            length = 9;
        }
//        else if (function == _FC_DATA_BASE_READ ||  //?
//                         function == _FC_DATA_BASE_WRITE)
        else if (function == _FC_DATA_BASE_READ)//?
        {
            length = 1;
        }
        else
        {
            /* _FC_READ_EXCEPTION_STATUS, _FC_REPORT_SLAVE_ID */
            length = 0;
        }
    }
    else
    {
        /* MSG_CONFIRMATION */
        switch (function)
        {
        case _FC_WRITE_SINGLE_COIL:
        case _FC_WRITE_SINGLE_REGISTER:
        case _FC_WRITE_MULTIPLE_COILS:
        case _FC_WRITE_MULTIPLE_REGISTERS:
            length = 4;
            break;
        default:
            length = 1;
        }
    }

    return length;
}

/* Computes the length to read after the meta information (address, count, etc) */
static int compute_data_length_after_meta(modbus_t *ctx, unsigned char *msg,
        msg_type_t msg_type)
{
    int function = msg[ctx->backend->header_length];
    int length;

    if (msg_type == MSG_INDICATION)
    {
        switch (function)
        {
        case _FC_WRITE_MULTIPLE_COILS:
        case _FC_WRITE_MULTIPLE_REGISTERS:
            length = msg[ctx->backend->header_length + 5];
            break;
        case _FC_WRITE_AND_READ_REGISTERS:
            length = msg[ctx->backend->header_length + 9];
            break;
        default:
            length = 0;
        }
    }
    else
    {
        /* MSG_CONFIRMATION */
        if (function <= _FC_READ_INPUT_REGISTERS ||
                function == _FC_REPORT_SLAVE_ID ||
                function == _FC_WRITE_AND_READ_REGISTERS)
        {
            length = msg[ctx->backend->header_length + 1];
        }
        else
        {
            length = 0;
        }
    }

    length += ctx->backend->checksum_length;

    return length;
}

///* Waits a response from a modbus server or a request from a modbus client.
//   This function blocks if there is no replies (3 timeouts).
//
//   The function shall return the number of received characters and the received
//   message in an array of unsigned char if successful. Otherwise it shall return -1
//   and errno is set to one of the values defined below:
//   - ECONNRESET
//   - EMBBADDATA
//   - EMBUNKEXC
//   - ETIMEDOUT
//   - read() or recv() error codes
//*/
static int receive_msg(modbus_t *ctx, unsigned char *msg, msg_type_t msg_type)
{
    int rc;
    fd_set rfds;
    struct timeval tv;
    struct timeval *p_tv;
    int length_to_read;
    int msg_length = 0;
    _step_t step;

    struct timeval  xTimeCur;
    struct timeval xTimeLast;

    if (ctx->debug)
    {
        if (msg_type == MSG_INDICATION)
        {
            printf("Waiting for a indication...\n");
        }
        else
        {
            printf("Waiting for a confirmation...\n");
        }
    }

    /* We need to analyse the message step by step.  At the first step, we want
     * to reach the function code because all packets contain this
     * information. */
    step = _STEP_FUNCTION;
    length_to_read = ctx->backend->header_length + 1;

//    cout << "backend_type " << ctx->backend->backend_type <<endl;
    switch(ctx->backend->backend_type)
    {
    case _MODBUS_BACKEND_TYPE_TCP:
//        cout << "_MODBUS_BACKEND_TYPE_TCP" << endl;
        if (msg_type == MSG_INDICATION)
        {
            /* Wait for a message, we don't know when the message will be
             * received */
//            tv.tv_sec = 0;//ctx->receive_timeout.tv_sec;
//            tv.tv_usec = 15000000;//ctx->receive_timeout.tv_usec;
            tv.tv_sec = ctx->receive_timeout.tv_sec;
            tv.tv_usec = ctx->receive_timeout.tv_usec;
            p_tv = &tv;//debag//
//            p_tv = NULL;
        }
        else
        {
            tv.tv_sec = ctx->response_timeout.tv_sec;
            tv.tv_usec = ctx->response_timeout.tv_usec;
            p_tv = &tv;
        }

        /* Add a file descriptor to the set */
        FD_ZERO(&rfds);
        FD_SET(ctx->s, &rfds);
//        FD_SET(STDIN_FILENO, &rfds);

        if( ctx->backend->select(ctx, &rfds, p_tv, NULL) == -1 )
        {
            _error_print(ctx, "select");
            if (ctx->error_recovery & MODBUS_ERROR_RECOVERY_LINK)
            {
                int saved_errno = errno;

                if (errno == ETIMEDOUT)
                {
                    _sleep_and_flush(ctx);
                }
                else if (errno == EBADF)
                {
                    modbus_close(ctx);
                    modbus_connect(ctx);
                }
                errno = saved_errno;
            }
            cout << "select error" << endl;
            return -1;
        }
        else if( FD_ISSET( ctx -> s, &rfds ) )
        {
            if( ( rc = read( ctx -> s, (msg + msg_length), (MODBUS_RTU_MAX_ADU_LENGTH - msg_length) ) ) == -1 )
            {
                _error_print(ctx, "read");
                if ((ctx->error_recovery & MODBUS_ERROR_RECOVERY_LINK) &&
                        (errno == ECONNRESET || errno == ECONNREFUSED ||
                         errno == EBADF))
                {
                    int saved_errno = errno;
                    modbus_close(ctx);
                    modbus_connect(ctx);
                    /* Could be removed by previous calls */
                    errno = saved_errno;
                }
                cout << "FD_ISSET error" << endl;
                return -1;
            }
            else
            {
                if (rc)
                {
                    msg_length += rc;
                    if ((msg_length > ctx->backend->max_adu_length) ||
                            (msg_length < _MIN_MESSAGE_LENGTH))
                    {
                        errno = EMBBADDATA;
                        _error_print(ctx, "too many data");
                        cout << "rc error" << endl;
                        return -1;
                    }


//                    cout << "modbus res" << endl;
//                    unsigned char *pucSourceTemp;
//                    pucSourceTemp = (unsigned char*)msg;
//                    for(int i=0; i<256; )
//                    {
//                        for(int j=0; j<8; j++)
//                        {
//                            cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
//                        }
//                        cout << endl;
//                        i += 8;
//                    }

//                    cout << "modbus check_integrity" << endl;
                    return ctx->backend->check_integrity(ctx, msg, msg_length);
                }
                else
                {
                    cout << "modbus errno" << endl;
                    errno = ECONNRESET;
                    return -1;
                }
            }
        }
        else
        {

            cout << "modbus else" << endl;
        }
        break;

    case _MODBUS_BACKEND_TYPE_RTU:
//        cout << "_MODBUS_BACKEND_TYPE_RTU" << endl;
        if (msg_type == MSG_INDICATION)
        {
            /* Wait for a message, we don't know when the message will be
             * received */
            tv.tv_sec = ctx->receive_timeout.tv_sec;
            tv.tv_usec = ctx->receive_timeout.tv_usec;
            p_tv = &tv;
//            p_tv = NULL;
        }
        else
        {
            tv.tv_sec = ctx->response_timeout.tv_sec;
            tv.tv_usec = ctx->response_timeout.tv_usec;
            p_tv = &tv;
        }
        while (1)
        {

            /* Add a file descriptor to the set */
            FD_ZERO(&rfds);
            FD_SET(ctx->s, &rfds);
//            FD_SET(STDIN_FILENO, &rfds);

            if( ctx->backend->select(ctx, &rfds, p_tv, NULL) == -1 )
            {
                if (errno == ETIMEDOUT)
                {
                    if ((msg_length > ctx->backend->max_adu_length) ||
                            (msg_length < _MIN_MESSAGE_LENGTH))
                    {
                        errno = EMBBADDATA;
                        _error_print(ctx, "too many data");
                        return -1;
                    }
                    return ctx->backend->check_integrity(ctx, msg, msg_length);
                }


//                if (pxSlaveRtuCtx == ctx)
//                {
//                    cout << "modbus_receive select end return -1" << endl;//?
//                }
                return -1;
            }

            if(( ( rc = read( ctx -> s, (msg + msg_length), (MODBUS_RTU_MAX_ADU_LENGTH - msg_length) ) ) ) == -1 )
            {
//                if (pxSlaveRtuCtx == ctx)
//                {
//                    cout << "modbus_receive read return -1" << endl;//?
//                }
                _error_print(ctx, "read");
                if ((ctx->error_recovery & MODBUS_ERROR_RECOVERY_LINK) &&
                        (errno == ECONNRESET || errno == ECONNREFUSED ||
                         errno == EBADF))
                {
                    int saved_errno = errno;
                    modbus_close(ctx);
                    modbus_connect(ctx);
                    /* Could be removed by previous calls */
                    errno = saved_errno;
                }
                return -1;
            }
            else
            {
                if (rc)
                {
                    msg_length += rc;

                    if (msg_length > ctx->backend->max_adu_length)
                    {
//                        if (pxSlaveRtuCtx == ctx)
//                        {
//                            cout << "modbus_receive read return EMBBADDATA" << endl;//?
//                        }
                        errno = EMBBADDATA;
                        _error_print(ctx, "too many data");
                        return -1;
                    }

                    /* If there is no character in the buffer, the allowed timeout
                    interval between two consecutive bytes is defined by
                    byte_timeout */
                    tv.tv_sec = ctx->modbus_timeout.tv_sec;
                    tv.tv_usec = ctx->modbus_timeout.tv_usec;
                    p_tv = &tv;
                }
                else
                {
//                    if (pxSlaveRtuCtx == ctx)
//                    {
//                        cout << "modbus_receive read ECONNRESET" << endl;//?
//                    }
                    errno = ECONNRESET;
                    return -1;
                }
            }
        }
        break;

    default:
        cout << "backend_type default" << endl;
        break;
    };

    return 0;
}

/* Receive the request from a modbus master */
int modbus_receive(modbus_t *ctx, unsigned char *req)
{
    return receive_msg(ctx, req, MSG_INDICATION);
}

/* Receives the confirmation.

   The function shall store the read response in rsp and return the number of
   values (bits or words). Otherwise, its shall return -1 and errno is set.

   The function doesn't check the confirmation is the expected response to the
   initial request.
*/
int modbus_receive_confirmation(modbus_t *ctx, unsigned char *rsp)
{
    return receive_msg(ctx, rsp, MSG_CONFIRMATION);
}

static int check_confirmation(modbus_t *ctx, unsigned char *req,
                              unsigned char *rsp, int rsp_length)
{
    int rc;
    int rsp_length_computed;
    const int offset = ctx->backend->header_length;

    if (ctx->backend->pre_check_confirmation)
    {
        rc = ctx->backend->pre_check_confirmation(ctx, req, rsp, rsp_length);
        if (rc == -1)
        {
            if (ctx->error_recovery & MODBUS_ERROR_RECOVERY_PROTOCOL)
            {
                _sleep_and_flush(ctx);
            }
            return -1;
        }
    }

    rsp_length_computed = compute_response_length_from_request(ctx, req, rsp);

    /* Check length */
    if (rsp_length == rsp_length_computed ||
            rsp_length_computed == MSG_LENGTH_UNDEFINED)
    {
        int req_nb_value;
        int rsp_nb_value;
        const int function = rsp[offset];

        /* Check function code */
        if (function != req[offset])
        {
            if (ctx->debug)
            {
                fprintf(stderr,
                        "Received function not corresponding to the request (%d != %d)\n",
                        function, req[offset]);
            }
            if (ctx->error_recovery & MODBUS_ERROR_RECOVERY_PROTOCOL)
            {
                _sleep_and_flush(ctx);
            }
            errno = EMBBADDATA;
            return -1;
        }

        /* Check the number of values is corresponding to the request */
        switch (function)
        {
        case _FC_READ_COILS:
        case _FC_READ_DISCRETE_INPUTS:
            /* Read functions, 8 values in a byte (nb
             * of values in the request and byte count in
             * the response. */
            req_nb_value = (req[offset + 3] << 8) + req[offset + 4];
            req_nb_value = (req_nb_value / 8) + ((req_nb_value % 8) ? 1 : 0);
            rsp_nb_value = rsp[offset + 1];
            break;
        case _FC_WRITE_AND_READ_REGISTERS:
        case _FC_READ_HOLDING_REGISTERS:
        case _FC_READ_INPUT_REGISTERS:
            /* Read functions 1 value = 2 bytes */
            req_nb_value = (req[offset + 3] << 8) + req[offset + 4];
            rsp_nb_value = (rsp[offset + 1] / 2);
            break;
        case _FC_WRITE_MULTIPLE_COILS:
        case _FC_WRITE_MULTIPLE_REGISTERS:
            /* N Write functions */
            req_nb_value = (req[offset + 3] << 8) + req[offset + 4];
            rsp_nb_value = (rsp[offset + 3] << 8) | rsp[offset + 4];
            break;
        case _FC_REPORT_SLAVE_ID:
            /* Report slave ID (bytes received) */
            req_nb_value = rsp_nb_value = rsp[offset + 1];
            break;
        case _FC_DATA_EXCHANGE:
            /* DataExchange (bytes received) */
            req_nb_value = rsp_nb_value = rsp[offset + 1];
            break;
        default:
            /* 1 Write functions & others */
            req_nb_value = rsp_nb_value = 1;
        }

        if (req_nb_value == rsp_nb_value)
        {
            rc = rsp_nb_value;
        }
        else
        {
            if (ctx->debug)
            {
                fprintf(stderr,
                        "Quantity not corresponding to the request (%d != %d)\n",
                        rsp_nb_value, req_nb_value);
            }

            if (ctx->error_recovery & MODBUS_ERROR_RECOVERY_PROTOCOL)
            {
                _sleep_and_flush(ctx);
            }

            errno = EMBBADDATA;
            rc = -1;
        }
    }
    else if (rsp_length == (offset + 2 + ctx->backend->checksum_length) &&
             req[offset] == (rsp[offset] - 0x80))
    {
        /* EXCEPTION CODE RECEIVED */

        int exception_code = rsp[offset + 1];
        if (exception_code < MODBUS_EXCEPTION_MAX)
        {
            errno = MODBUS_ENOBASE + exception_code;
        }
        else
        {
            errno = EMBBADEXC;
        }
        _error_print(ctx, NULL);
        rc = -1;
    }
    else
    {
        if (ctx->debug)
        {
            fprintf(stderr,
                    "Message length not corresponding to the computed length (%d != %d)\n",
                    rsp_length, rsp_length_computed);
        }
        if (ctx->error_recovery & MODBUS_ERROR_RECOVERY_PROTOCOL)
        {
            _sleep_and_flush(ctx);
        }
        errno = EMBBADDATA;
        rc = -1;
    }

    return rc;
}

static int response_io_status(int address, int nb,
                              unsigned char *tab_io_status,
                              unsigned char *rsp, int offset)
{
    int shift = 0;
    int byte = 0;
    int i;

    for (i = address; i < address+nb; i++)
    {
        byte |= tab_io_status[i] << shift;
        if (shift == 7)
        {
            /* Byte is full */
            rsp[offset++] = byte;
            byte = shift = 0;
        }
        else
        {
            shift++;
        }
    }

    if (shift != 0)
        rsp[offset++] = byte;

    return offset;
}

/* Build the exception response */
static int response_exception(modbus_t *ctx, sft_t *sft,
                              int exception_code, unsigned char *rsp)
{
    int rsp_length;

    sft->function = sft->function + 0x80;
    rsp_length = ctx->backend->build_response_basis(sft, rsp);

    /* Positive exception code */
    rsp[rsp_length++] = exception_code;

    return rsp_length;
}

/* Send a response to the received request.
   Analyses the request and constructs a response.

   If an error occurs, this function construct the response
   accordingly.
*/
int modbus_reply(modbus_t *ctx, const unsigned char *req,
                 int req_length, modbus_mapping_t *mb_mapping)
{
    int offset = ctx->backend->header_length;
    int slave = req[offset - 1];
    int function = req[offset];
    unsigned short address = (req[offset + 1] << 8) + req[offset + 2];
    unsigned char rsp[MAX_MESSAGE_LENGTH];
    int rsp_length = 0;
    sft_t sft;
    float fTemp;
    float *pfSource;
    unsigned char *pucSource;
    unsigned char *pucSource2;
    unsigned char *pucDestination;
    unsigned char *pucTempArray;
    unsigned int nuiBusyTimeCounter;
    unsigned char nucIndexNumber;
    unsigned char ucTempData;
    unsigned short usCrc;
    unsigned char ucFlowControl;
    int i;
    int j;
    int nb;


//    cout << "function" << endl;
//    cout << "0x" << hex << uppercase << setw(2) << setfill('0') << (unsigned int)(function) << " " << endl;
//
//    cout << "nb" << endl;
//    cout << "0x" << hex << uppercase << setw(2) << setfill('0') << (unsigned int)((req[offset + 1] << 8) + req[offset + 2]) << " " << endl;
//    cout << "nb" << endl;
//    cout << "0x" << hex << uppercase << setw(2) << setfill('0') << (unsigned int)((req[offset + 3] << 8) + req[offset + 4]) << " " << endl;

//    if (_FC_TIME_SET != function)
//    {
    if (ctx->backend->filter_request(ctx, slave) == 1)
    {
        /* Filtered */
        cout << "filter_request" << endl;
        return 0;
    }
//    }


//    CGpio::SetPin(ctx->led_port, ctx->led_pin);

//    if (pxSlaveTcpCtx == ctx)
//    {
//        cout << "modbus_reply req" << endl;
//        unsigned char *pucSourceTemp;
//        pucSourceTemp = (unsigned char*)req;
//        for(int i=0; i<32; )
//        {
//            for(int j=0; j<8; j++)
//            {
//                cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
//            }
//            cout << endl;
//            i += 8;
//        }
//    }


    sft.slave = slave;
    sft.function = function;
    sft.t_id = ctx->backend->prepare_response_tid(req, &req_length);
    switch (function)
    {
    case _FC_READ_COILS:
    {
        nb = (req[offset + 3] << 8) + req[offset + 4];

        if (nb < 1 || MODBUS_MAX_READ_BITS < nb)
        {
            if (ctx->debug)
            {
                fprintf(stderr,
                        "Illegal nb of values %d in read_bits (max %d)\n",
                        nb, MODBUS_MAX_READ_BITS);
            }
            cout << "MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE" << endl;
            rsp_length = response_exception(
                             ctx, &sft,
                             MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE, rsp);
        }
        else if ((address + nb) > mb_mapping->nb_bits)
        {
            if (ctx->debug)
            {
                fprintf(stderr, "Illegal data address %0X in read_bits\n",
                        address + nb);
            }
            cout << "MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS" << endl;
            rsp_length = response_exception(
                             ctx, &sft,
                             MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS, rsp);
        }
        else
        {
            nuiBusyTimeCounter = MAX_MODBUS_BUFFER_BUSY_WAITING_TIME;

            while (1)
            {
                if (mb_mapping->message_ready)
                {
                    usleep(COMMON_DELAY_TIME);
                    if (!nuiBusyTimeCounter--)
                    {
                        cout << "MODBUS_EXCEPTION_SLAVE_OR_SERVER_BUSY" << endl;
                        rsp_length = response_exception(
                                         ctx, &sft,
                                         MODBUS_EXCEPTION_SLAVE_OR_SERVER_BUSY, rsp);
                        break;
                    }
                }
                else
                {
                    mb_mapping->message_ready = 1;

                    rsp_length = ctx->backend->build_response_basis(&sft, rsp);
                    rsp[rsp_length++] = (nb / 8) + ((nb % 8) ? 1 : 0);
                    rsp_length = response_io_status(address, nb,
                                                    mb_mapping->tab_bits,
                                                    rsp, rsp_length);

                    mb_mapping->message_ready = 0;

                    break;
                }
            }
        }
    }
    break;

    case _FC_READ_DISCRETE_INPUTS:
    {
        /* Similar to coil status (but too many arguments to use a
         * function) */
        nb = (req[offset + 3] << 8) + req[offset + 4];

        if (nb < 1 || MODBUS_MAX_READ_BITS < nb)
        {
            if (ctx->debug)
            {

                fprintf(stderr,
                        "Illegal nb of values %d in read_input_bits (max %d)\n",
                        nb, MODBUS_MAX_READ_BITS);
            }
            cout << "MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE" << endl;
            rsp_length = response_exception(
                             ctx, &sft,
                             MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE, rsp);
        }
        else if ((address + nb) > mb_mapping->nb_input_bits)
        {
            if (ctx->debug)
            {
                fprintf(stderr, "Illegal data address %0X in read_input_bits\n",
                        address + nb);
            }
            cout << "MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS" << endl;
            rsp_length = response_exception(
                             ctx, &sft,
                             MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS, rsp);
        }
        else
        {
            nuiBusyTimeCounter = MAX_MODBUS_BUFFER_BUSY_WAITING_TIME;

            while (1)
            {
                if (mb_mapping->message_ready)
                {
                    usleep(COMMON_DELAY_TIME);
                    if (!nuiBusyTimeCounter--)
                    {
                        cout << "MODBUS_EXCEPTION_SLAVE_OR_SERVER_BUSY" << endl;
                        rsp_length = response_exception(
                                         ctx, &sft,
                                         MODBUS_EXCEPTION_SLAVE_OR_SERVER_BUSY, rsp);
                        break;
                    }
                }
                else
                {
                    mb_mapping->message_ready = 1;

                    rsp_length = ctx->backend->build_response_basis(&sft, rsp);
                    rsp[rsp_length++] = (nb / 8) + ((nb % 8) ? 1 : 0);
                    rsp_length = response_io_status(address, nb,
                                                    mb_mapping->tab_input_bits,
                                                    rsp, rsp_length);
                    mb_mapping->message_ready = 0;

                    break;
                }
            }
        }
    }
    break;

    case _FC_READ_HOLDING_REGISTERS:
    {
        nb = (req[offset + 3] << 8) + req[offset + 4];

//        if (pxSlaveTcpCtx == ctx)
//        {
//            printf("address %d\n\r", address);
//        }


        if (nb < 1 || MODBUS_MAX_READ_REGISTERS < nb)
        {
            if (ctx->debug)
            {

                fprintf(stderr,
                        "Illegal nb of values %d in read_holding_registers (max %d)\n",
                        nb, MODBUS_MAX_READ_REGISTERS);
            }
            cout << "MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE" << endl;
            rsp_length = response_exception(
                             ctx, &sft,
                             MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE, rsp);
        }
        else if ((address + nb) > mb_mapping->nb_registers)
        {
            if (ctx->debug)
            {
                fprintf(stderr, "Illegal data address %0X in read_registers\n",
                        address + nb);
            }
            cout << "MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS" << endl;
            rsp_length = response_exception(
                             ctx, &sft,
                             MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS, rsp);
        }
        else
        {
            nuiBusyTimeCounter = MAX_MODBUS_BUFFER_BUSY_WAITING_TIME;

            while (1)
            {
                if (mb_mapping->message_ready)
                {
                    usleep(COMMON_DELAY_TIME);
                    if (!nuiBusyTimeCounter--)
                    {
                        cout << "MODBUS_EXCEPTION_SLAVE_OR_SERVER_BUSY" << endl;
                        rsp_length = response_exception(
                                         ctx, &sft,
                                         MODBUS_EXCEPTION_SLAVE_OR_SERVER_BUSY, rsp);
                        break;
                    }
                }
                else
                {
                    mb_mapping->message_ready = 1;

                    rsp_length = ctx->backend->build_response_basis(&sft, rsp);

                    rsp[rsp_length++] = nb << 1;
                    for (i = address; i < address + nb; i++)
                    {
                        rsp[rsp_length++] = mb_mapping->tab_registers[i] >> 8;
                        rsp[rsp_length++] = mb_mapping->tab_registers[i] & 0xFF;
                    }
                    mb_mapping->message_ready = 0;

                    break;
                }
            }
        }
    }
    break;

    case _FC_READ_INPUT_REGISTERS:
    {
        /* Similar to holding registers (but too many arguments to use a
         * function) */
        nb = (req[offset + 3] << 8) + req[offset + 4];

        if (nb < 1 || MODBUS_MAX_READ_REGISTERS < nb)
        {
            if (ctx->debug)
            {

                fprintf(stderr,
                        "Illegal number of values %d in read_input_registers (max %d)\n",
                        nb, MODBUS_MAX_READ_REGISTERS);
            }
            cout << "MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE" << endl;
            rsp_length = response_exception(
                             ctx, &sft,
                             MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE, rsp);
        }
        else if ((address + nb) > mb_mapping->nb_input_registers)
        {
            if (ctx->debug)
            {

                fprintf(stderr, "Illegal data address %0X in read_input_registers\n",
                        address + nb);
            }
            cout << "MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS" << endl;
            rsp_length = response_exception(
                             ctx, &sft,
                             MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS, rsp);
        }
        else
        {
            nuiBusyTimeCounter = MAX_MODBUS_BUFFER_BUSY_WAITING_TIME;

            while (1)
            {
                if (mb_mapping->message_ready)
                {
                    usleep(COMMON_DELAY_TIME);
                    if (!nuiBusyTimeCounter--)
                    {
                        cout << "MODBUS_EXCEPTION_SLAVE_OR_SERVER_BUSY" << endl;
                        rsp_length = response_exception(
                                         ctx, &sft,
                                         MODBUS_EXCEPTION_SLAVE_OR_SERVER_BUSY, rsp);
                        break;
                    }
                }
                else
                {
                    mb_mapping->message_ready = 1;

                    rsp_length = ctx->backend->build_response_basis(&sft, rsp);
                    rsp[rsp_length++] = nb << 1;
                    for (i = address; i < address + nb; i++)
                    {
                        rsp[rsp_length++] = mb_mapping->tab_input_registers[i] >> 8;
                        rsp[rsp_length++] = mb_mapping->tab_input_registers[i] & 0xFF;
                    }

                    mb_mapping->message_ready = 0;
                    break;
                }
            }
        }
    }
    break;

    case _FC_WRITE_SINGLE_COIL:
        if (address >= mb_mapping->nb_bits)
        {
            if (ctx->debug)
            {
                fprintf(stderr, "Illegal data address %0X in write_bit\n",
                        address);
            }
            cout << "MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS" << endl;
            rsp_length = response_exception(
                             ctx, &sft,
                             MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS, rsp);
        }
        else
        {
            nuiBusyTimeCounter = MAX_MODBUS_BUFFER_BUSY_WAITING_TIME;
            while (mb_mapping->message_ready)
            {
                usleep(COMMON_DELAY_TIME);
                if (!nuiBusyTimeCounter--)
                {
                    cout << "MODBUS_EXCEPTION_SLAVE_OR_SERVER_BUSY 2" << endl;
                    rsp_length = response_exception(
                                     ctx, &sft,
                                     MODBUS_EXCEPTION_SLAVE_OR_SERVER_BUSY, rsp);
                    break;
                }
            }

            int data = (req[offset + 3] << 8) + req[offset + 4];

            if (data == 0xFF00 || data == 0x0)
            {
                mb_mapping->message_ready = 1;
                mb_mapping->current_message_address_bits = address;
                mb_mapping->current_message_nb_bits = 1;
                mb_mapping->function_code = _FC_WRITE_SINGLE_COIL;

                mb_mapping->tab_bits[address] = (data) ? ON : OFF;
                memcpy(rsp, req, req_length);
                rsp_length = req_length;

                nuiBusyTimeCounter = MAX_MODBUS_BUFFER_BUSY_WAITING_TIME;
                while (mb_mapping->message_ready)
                {
                    usleep(COMMON_DELAY_TIME);
                    if (!nuiBusyTimeCounter--)
                    {
                        cout << "MODBUS_EXCEPTION_SLAVE_OR_SERVER_BUSY 2" << endl;
                        rsp_length = response_exception(
                                         ctx, &sft,
                                         MODBUS_EXCEPTION_SLAVE_OR_SERVER_BUSY, rsp);
                        break;
                    }
                }

                mb_mapping->message_ready = 0;
                break;
            }
            else
            {
                if (ctx->debug)
                {
                    fprintf(stderr,
                            "Illegal data value %0X in write_bit request at address %0X\n",
                            data, address);
                }
                cout << "MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE" << endl;
                rsp_length = response_exception(
                                 ctx, &sft,
                                 MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE, rsp);
            }
        }
        break;

    case _FC_WRITE_SINGLE_REGISTER:
        if (address >= mb_mapping->nb_registers)
        {
            if (ctx->debug)
            {
                fprintf(stderr, "Illegal data address %0X in write_register\n",
                        address);
            }
            cout << "MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS" << endl;
            rsp_length = response_exception(
                             ctx, &sft,
                             MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS, rsp);
        }
        else
        {
            nuiBusyTimeCounter = MAX_MODBUS_BUFFER_BUSY_WAITING_TIME;
            while (mb_mapping->message_ready)
            {
                usleep(COMMON_DELAY_TIME);
                if (!nuiBusyTimeCounter--)
                {
                    cout << "MODBUS_EXCEPTION_SLAVE_OR_SERVER_BUSY 2" << endl;
                    rsp_length = response_exception(
                                     ctx, &sft,
                                     MODBUS_EXCEPTION_SLAVE_OR_SERVER_BUSY, rsp);
                    break;
                }
            }

            int data = (req[offset + 3] << 8) + req[offset + 4];

            mb_mapping->message_ready = 1;
            mb_mapping->current_message_address_registers = address;
            mb_mapping->current_message_nb_registers = 1;
            mb_mapping->function_code = _FC_WRITE_SINGLE_REGISTER;

            mb_mapping->tab_registers[address] = data;
            memcpy(rsp, req, req_length);
            rsp_length = req_length;

            nuiBusyTimeCounter = MAX_MODBUS_BUFFER_BUSY_WAITING_TIME;
            while (mb_mapping->message_ready)
            {
                usleep(COMMON_DELAY_TIME);
                if (!nuiBusyTimeCounter--)
                {
                    cout << "MODBUS_EXCEPTION_SLAVE_OR_SERVER_BUSY 2" << endl;
                    rsp_length = response_exception(
                                     ctx, &sft,
                                     MODBUS_EXCEPTION_SLAVE_OR_SERVER_BUSY, rsp);
                    break;
                }
            }

            mb_mapping->message_ready = 0;
        }
        break;

    case _FC_WRITE_MULTIPLE_COILS:
    {
        nb = (req[offset + 3] << 8) + req[offset + 4];

        if (nb < 1 || MODBUS_MAX_WRITE_BITS < nb)
        {
            if (ctx->debug)
            {
                fprintf(stderr,
                        "Illegal number of values %d in write_bits (max %d)\n",
                        nb, MODBUS_MAX_WRITE_BITS);
            }
            cout << "MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE" << endl;
            rsp_length = response_exception(
                             ctx, &sft,
                             MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE, rsp);
        }
        else if ((address + nb) > mb_mapping->nb_bits)
        {
            if (ctx->debug)
            {
                fprintf(stderr, "Illegal data address %0X in write_bits\n",
                        address + nb);
            }
            cout << "MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS" << endl;
            rsp_length = response_exception(
                             ctx, &sft,
                             MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS, rsp);
        }
        else
        {
            nuiBusyTimeCounter = MAX_MODBUS_BUFFER_BUSY_WAITING_TIME;
            while (mb_mapping->message_ready)
            {
                usleep(COMMON_DELAY_TIME);
                if (!nuiBusyTimeCounter--)
                {
                    cout << "MODBUS_EXCEPTION_SLAVE_OR_SERVER_BUSY 2" << endl;
                    rsp_length = response_exception(
                                     ctx, &sft,
                                     MODBUS_EXCEPTION_SLAVE_OR_SERVER_BUSY, rsp);
                    break;
                }
            }

            /* 6 = byte count */
            mb_mapping->message_ready = 1;
            mb_mapping->current_message_address_bits = address;
            mb_mapping->current_message_nb_bits = nb;
            mb_mapping->function_code = _FC_WRITE_MULTIPLE_COILS;

            modbus_set_bits_from_bytes(mb_mapping->tab_bits, address, nb, &req[offset + 6]);

            rsp_length = ctx->backend->build_response_basis(&sft, rsp);
            /* 4 to copy the bit address (2) and the quantity of bits */
            memcpy(rsp + rsp_length, req + rsp_length, 4);
            rsp_length += 4;

            nuiBusyTimeCounter = MAX_MODBUS_BUFFER_BUSY_WAITING_TIME;
            while (mb_mapping->message_ready)
            {
                usleep(COMMON_DELAY_TIME);
                if (!nuiBusyTimeCounter--)
                {
                    cout << "MODBUS_EXCEPTION_SLAVE_OR_SERVER_BUSY 2" << endl;
                    rsp_length = response_exception(
                                     ctx, &sft,
                                     MODBUS_EXCEPTION_SLAVE_OR_SERVER_BUSY, rsp);
                    break;
                }
            }

            mb_mapping->message_ready = 0;
        }
    }
    break;

    case _FC_WRITE_MULTIPLE_REGISTERS:
    {
        nb = (req[offset + 3] << 8) + req[offset + 4];

        if (nb < 1 || MODBUS_MAX_WRITE_REGISTERS < nb)
        {
            if (ctx->debug)
            {
                fprintf(stderr,
                        "Illegal number of values %d in write_registers (max %d)\n",
                        nb, MODBUS_MAX_WRITE_REGISTERS);
            }
            cout << "MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE" << endl;
            rsp_length = response_exception(
                             ctx, &sft,
                             MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE, rsp);
        }
        else if ((address + nb) > mb_mapping->nb_registers)
        {
            if (ctx->debug)
            {
                fprintf(stderr, "Illegal data address %0X in write_registers\n",
                        address + nb);
            }
            cout << "MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS" << endl;
            rsp_length = response_exception(
                             ctx, &sft,
                             MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS, rsp);
        }
        else
        {
            nuiBusyTimeCounter = MAX_MODBUS_BUFFER_BUSY_WAITING_TIME;
            while (mb_mapping->message_ready)
            {
                usleep(COMMON_DELAY_TIME);
                if (!nuiBusyTimeCounter--)
                {
                    cout << "MODBUS_EXCEPTION_SLAVE_OR_SERVER_BUSY 2" << endl;
                    rsp_length = response_exception(
                                     ctx, &sft,
                                     MODBUS_EXCEPTION_SLAVE_OR_SERVER_BUSY, rsp);
                    break;
                }
            }

            mb_mapping->message_ready = 1;
            mb_mapping->current_message_address_registers = address;
            mb_mapping->current_message_nb_registers = nb;
            mb_mapping->function_code = _FC_WRITE_MULTIPLE_REGISTERS;

            for (i = address, j = 6; i < address + nb; i++, j += 2)
            {
                /* 6 and 7 = first value */
                mb_mapping->tab_registers[i] =
                    (req[offset + j] << 8) + req[offset + j + 1];
            }

            rsp_length = ctx->backend->build_response_basis(&sft, rsp);
            /* 4 to copy the address (2) and the no. of registers */
            memcpy(rsp + rsp_length, req + rsp_length, 4);
            rsp_length += 4;

            nuiBusyTimeCounter = MAX_MODBUS_BUFFER_BUSY_WAITING_TIME;
            while (mb_mapping->message_ready)
            {
                usleep(COMMON_DELAY_TIME);
                if (!nuiBusyTimeCounter--)
                {
                    cout << "MODBUS_EXCEPTION_SLAVE_OR_SERVER_BUSY 2" << endl;
                    rsp_length = response_exception(
                                     ctx, &sft,
                                     MODBUS_EXCEPTION_SLAVE_OR_SERVER_BUSY, rsp);
                    break;
                }
            }

            mb_mapping->message_ready = 0;
        }
    }
    break;

//    case _FC_REPORT_SLAVE_ID:
//    {
//        int str_len;
//        int byte_count_pos;
//
//        rsp_length = ctx->backend->build_response_basis(&sft, rsp);
//        /* Skip byte count for now */
//        byte_count_pos = rsp_length++;
//        rsp[rsp_length++] = _REPORT_SLAVE_ID;
//        /* Run indicator status to ON */
//        rsp[rsp_length++] = 0xFF;
//        /* LMB + length of LIBMODBUS_VERSION_STRING */
//        str_len = 3 + strlen(LIBMODBUS_VERSION_STRING);
//        memcpy(rsp + rsp_length, "LMB" LIBMODBUS_VERSION_STRING, str_len);
//        rsp_length += str_len;
//        rsp[byte_count_pos] = rsp_length - byte_count_pos - 1;
//    }
//    break;

    case _FC_READ_EXCEPTION_STATUS:
        if (ctx->debug)
        {
            fprintf(stderr, "FIXME Not implemented\n");
        }
        errno = ENOPROTOOPT;
        return -1;
        break;

    case _FC_WRITE_AND_READ_REGISTERS:
    {
        nb = (req[offset + 3] << 8) + req[offset + 4];
        unsigned short address_write = (req[offset + 5] << 8) + req[offset + 6];
        int nb_write = (req[offset + 7] << 8) + req[offset + 8];
        int nb_write_bytes = req[offset + 9];

        if (nb_write < 1 || MODBUS_MAX_RW_WRITE_REGISTERS < nb_write ||
                nb < 1 || MODBUS_MAX_READ_REGISTERS < nb ||
                nb_write_bytes != nb_write * 2)
        {
            if (ctx->debug)
            {
                fprintf(stderr,
                        "Illegal nb of values (W%d, R%d) in write_and_read_registers (max W%d, R%d)\n",
                        nb_write, nb,
                        MODBUS_MAX_RW_WRITE_REGISTERS, MODBUS_MAX_READ_REGISTERS);
            }
            cout << "MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE" << endl;
            rsp_length = response_exception(
                             ctx, &sft,
                             MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE, rsp);
        }
        else if ((address + nb) > mb_mapping->nb_registers ||
                 (address_write + nb_write) > mb_mapping->nb_registers)
        {
            if (ctx->debug)
            {
                fprintf(stderr,
                        "Illegal data read address %0X or write address %0X write_and_read_registers\n",
                        address + nb, address_write + nb_write);
            }
            rsp_length = response_exception(ctx, &sft,
                                            MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS, rsp);
        }
        else
        {
            nuiBusyTimeCounter = MAX_MODBUS_BUFFER_BUSY_WAITING_TIME;
            while (mb_mapping->message_ready)
            {
                usleep(COMMON_DELAY_TIME);
                if (!nuiBusyTimeCounter--)
                {
                    cout << "MODBUS_EXCEPTION_SLAVE_OR_SERVER_BUSY 2" << endl;
                    rsp_length = response_exception(
                                     ctx, &sft,
                                     MODBUS_EXCEPTION_SLAVE_OR_SERVER_BUSY, rsp);
                    break;
                }
            }

            mb_mapping->message_ready = 1;
            mb_mapping->current_message_address_registers = address_write;
            mb_mapping->current_message_nb_registers = nb_write;
            mb_mapping->function_code = _FC_WRITE_AND_READ_REGISTERS;

            rsp_length = ctx->backend->build_response_basis(&sft, rsp);
            rsp[rsp_length++] = nb << 1;

            /* Write first.
               10 and 11 are the offset of the first values to write */
            for (i = address_write, j = 10; i < address_write + nb_write; i++, j += 2)
            {
                mb_mapping->tab_registers[i] =
                    (req[offset + j] << 8) + req[offset + j + 1];
            }

            /* and read the data for the response */
            for (i = address; i < address + nb; i++)
            {
                rsp[rsp_length++] = mb_mapping->tab_registers[i] >> 8;
                rsp[rsp_length++] = mb_mapping->tab_registers[i] & 0xFF;
            }

            nuiBusyTimeCounter = MAX_MODBUS_BUFFER_BUSY_WAITING_TIME;
            while (mb_mapping->message_ready)
            {
                usleep(COMMON_DELAY_TIME);
                if (!nuiBusyTimeCounter--)
                {
                    cout << "MODBUS_EXCEPTION_SLAVE_OR_SERVER_BUSY 2" << endl;
                    rsp_length = response_exception(
                                     ctx, &sft,
                                     MODBUS_EXCEPTION_SLAVE_OR_SERVER_BUSY, rsp);
                    break;
                }
            }

            mb_mapping->message_ready = 0;
        }
    }
    break;

    case _FC_CONFIGURATION_REQUEST:
    {
        nuiBusyTimeCounter = MAX_MODBUS_BUFFER_BUSY_WAITING_TIME;
        while (mb_mapping->message_ready)
        {
            usleep(COMMON_DELAY_TIME);
            if (!nuiBusyTimeCounter--)
            {
                cout << "MODBUS_EXCEPTION_SLAVE_OR_SERVER_BUSY 2" << endl;
                rsp_length = response_exception(
                                 ctx, &sft,
                                 MODBUS_EXCEPTION_SLAVE_OR_SERVER_BUSY, rsp);
                break;
            }
        }

        mb_mapping->message_ready = 1;
        mb_mapping->function_code = _FC_CONFIGURATION_REQUEST;
        rsp_length = ctx->backend->build_response_basis(&sft, rsp);
        // (rsp_length - 2) - адрес slave.
        // (rsp_length - 1) - функция.
        // (rsp_length) - количество байт в ответе.
        // (rsp_length + 1) - начало данных в ответе.
        mb_mapping->buffer_pointer = mb_mapping->tab_auxiliary;

        nuiBusyTimeCounter = MAX_MODBUS_BUFFER_BUSY_WAITING_TIME;
        while (mb_mapping->message_ready)
        {
            usleep(COMMON_DELAY_TIME);
            if (!nuiBusyTimeCounter--)
            {
                cout << "MODBUS_EXCEPTION_SLAVE_OR_SERVER_BUSY 2" << endl;
                rsp_length = response_exception(
                                 ctx, &sft,
                                 MODBUS_EXCEPTION_SLAVE_OR_SERVER_BUSY, rsp);
                break;
            }
        }

        memcpy((&(rsp[rsp_length + 1])),
               mb_mapping->buffer_pointer,
               mb_mapping -> current_message_nb_common);


//                    printf("mb_mapping -> current_message_nb_common %d\n\r", (mb_mapping -> current_message_nb_common));
//
//        cout << "send_msg msg" << endl;
//        unsigned char *pucSourceTemp;
//        pucSourceTemp = (unsigned char*)mb_mapping->buffer_pointer;
//        for(int i=0; i<64; )
//        {
//            for(int j=0; j<8; j++)
//            {
//                cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
//            }
//            cout << endl;
//            i += 8;
//        }
//
//        cout << "send_msg msg" << endl;
////        unsigned char *pucSourceTemp;
//        pucSourceTemp = (unsigned char*)&rsp[rsp_length + 1];
//        for(int i=0; i<64; )
//        {
//            for(int j=0; j<8; j++)
//            {
//                cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
//            }
//            cout << endl;
//            i += 8;
//        }
        rsp[rsp_length++] = mb_mapping->current_message_nb_common;
        rsp_length += mb_mapping->current_message_nb_common;

        mb_mapping->message_ready = 0;
    }
    break;

    case _FC_DATA_BASE_ARCHIVE_BLOCK_READ: //?
    {

    }
    break;

    case _FC_DATA_BASE_READ:
//        cout << "_FC_DATA_BASE_READ" << endl;
    {
        if (((req[offset + 1]) < 0) ||
                ((req[offset + 1]) > (PLC_DATA_BASE_BLOCK_QUANTITY - 1)))
        {
            if (ctx->debug)
            {
                fprintf(stderr, "Illegal data address %0X in read_bits\n",
                        address + nb);
            }
            cout << "MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS" << endl;
            rsp_length = response_exception(
                             ctx, &sft,
                             MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE, rsp);
        }
        else
        {
            nuiBusyTimeCounter = MAX_MODBUS_BUFFER_BUSY_WAITING_TIME;
            while (mb_mapping->message_ready)
            {
                usleep(COMMON_DELAY_TIME);
                if (!nuiBusyTimeCounter--)
                {
                    cout << "MODBUS_EXCEPTION_SLAVE_OR_SERVER_BUSY 2" << endl;
                    rsp_length = response_exception(
                                     ctx, &sft,
                                     MODBUS_EXCEPTION_SLAVE_OR_SERVER_BUSY, rsp);
                    break;
                }
            }

            mb_mapping->message_ready = 1;
            mb_mapping->current_message_address_common = req[offset + 1];
            mb_mapping->function_code = _FC_DATA_BASE_READ;
            rsp_length = ctx->backend->build_response_basis(&sft, rsp);
            // (rsp_length - 2) - адрес slave.
            // (rsp_length - 1) - функция.
            // (rsp_length) - количество байт в ответе.
            // (rsp_length + 1) - номер блока.
            // (rsp_length + 2) - начало данных в ответе.
            mb_mapping->buffer_pointer = mb_mapping->tab_auxiliary;

            nuiBusyTimeCounter = MAX_MODBUS_BUFFER_BUSY_WAITING_TIME;
            while (mb_mapping->message_ready)
            {
                usleep(COMMON_DELAY_TIME);
                if (!nuiBusyTimeCounter--)
                {
                    cout << "MODBUS_EXCEPTION_SLAVE_OR_SERVER_BUSY 2" << endl;
                    rsp_length = response_exception(
                                     ctx, &sft,
                                     MODBUS_EXCEPTION_SLAVE_OR_SERVER_BUSY, rsp);
                    break;
                }
            }
//                    // при чтении базы данных из модуля произошла ошибка?
//                    if (mb_mapping->current_message_address_common)
//                    {
//                        rsp_length = response_exception(
//                                         ctx, &sft,
//                                         (mb_mapping->current_message_address_common), rsp);
//
//                    mb_mapping->message_ready = 0;
//                        break;
//                    }

            memcpy((&(rsp[rsp_length + 2])),
                   mb_mapping->buffer_pointer,
                   mb_mapping -> current_message_nb_common);

            rsp[rsp_length++] = mb_mapping->current_message_nb_common +
                                DATA_BASE_BLOCK_NUMBER_DATA_LENGTH;
            rsp[rsp_length++] = req[offset + 1];
            rsp_length += mb_mapping->current_message_nb_common;

            mb_mapping->message_ready = 0;
        }
    }
    break;

    case _FC_DATA_BASE_WRITE:
    {
        if (((req[offset + 1]) < 0) ||
                ((req[offset + 1]) > (PLC_DATA_BASE_BLOCK_QUANTITY - 1)))
        {
            if (ctx->debug)
            {
                fprintf(stderr, "Illegal data address %0X in read_bits\n",
                        address + nb);
            }
            rsp_length = response_exception(
                             ctx, &sft,
                             MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS, rsp);
            cout << "MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS" << endl;
        }
        else
        {
            nuiBusyTimeCounter = MAX_MODBUS_BUFFER_BUSY_WAITING_TIME;
            while (mb_mapping->message_ready)
            {
                usleep(COMMON_DELAY_TIME);
                if (!nuiBusyTimeCounter--)
                {
                    cout << "MODBUS_EXCEPTION_SLAVE_OR_SERVER_BUSY 2" << endl;
                    rsp_length = response_exception(
                                     ctx, &sft,
                                     MODBUS_EXCEPTION_SLAVE_OR_SERVER_BUSY, rsp);
                    break;
                }
            }

            mb_mapping->message_ready = 1;
            mb_mapping->current_message_address_registers = req[offset + 1];
            mb_mapping->function_code = _FC_DATA_BASE_WRITE;
            mb_mapping->buffer_pointer = mb_mapping->tab_auxiliary;
            memcpy((unsigned char*)(mb_mapping -> buffer_pointer),
                   (unsigned char*)&(req[offset + 2]),
                   PLC_DATA_BASE_BLOCK_LENGTH);

            rsp_length = ctx->backend->build_response_basis(&sft, rsp);
            rsp[rsp_length++] = DATA_BASE_BLOCK_NUMBER_DATA_LENGTH;
            rsp[rsp_length++] = req[offset + 1];

            nuiBusyTimeCounter = MAX_MODBUS_BUFFER_BUSY_WAITING_TIME;
            while (mb_mapping->message_ready)
            {
                usleep(COMMON_DELAY_TIME);
                if (!nuiBusyTimeCounter--)
                {
                    cout << "MODBUS_EXCEPTION_SLAVE_OR_SERVER_BUSY 2" << endl;
                    rsp_length = response_exception(
                                     ctx, &sft,
                                     MODBUS_EXCEPTION_SLAVE_OR_SERVER_BUSY, rsp);
                    break;
                }
            }

            mb_mapping->message_ready = 0;
        }
    }
    break;

    case _FC_PROGRAMMING_COMPLETION_REQUEST:
        if (ucDataBaseError == PREVIOUS_REQUES_WAS_NOT_A_BLOCK_TRANSFER)
        {
            cout << "PREVIOUS_REQUES_WAS_NOT_A_BLOCK_TRANSFER" << endl;
//            rsp_length = ctx->backend->build_response_basis(&sft, rsp);
            rsp_length = response_exception(
                             ctx, &sft,
                             PREVIOUS_REQUES_WAS_NOT_A_BLOCK_TRANSFER_ERROR, rsp);
//            rsp[1] =  0x86;//|= 0x80;
//            rsp[2] = PREVIOUS_REQUES_WAS_NOT_A_BLOCK_TRANSFER_ERROR;
//            rsp_length++;
        }
        else if (ucDataBaseError == PROGRAMMING_DATA_BASE_WRITE_OK)
        {
            cout << "FC_PROGRAMMING_COMPLETION_REQUEST_OK" << endl;
            rsp_length = ctx->backend->build_response_basis(&sft, rsp);
            ucDataBaseError = PREVIOUS_REQUES_WAS_NOT_A_BLOCK_TRANSFER;
        }
        else if (ucDataBaseError == PROGRAMMING_DATA_BASE_WRITE_BLOCK_BUSY)
        {
            cout << "FC_PROGRAMMING_COMPLETION_REQUEST_BUSY" << endl;
            rsp_length = response_exception(
                             ctx, &sft,
                             ucDataBaseError, rsp);
//            rsp_length = ctx->backend->build_response_basis(&sft, rsp);
//            rsp[1] =  0x86;// |= 0x80;
//            rsp[2] = PROGRAMMING_DATA_BASE_WRITE_BLOCK_BUSY;
//            rsp_length++;
        }
        else
        {
            cout << "FC_PROGRAMMING_COMPLETION_REQUEST_ERROR" << endl;
            rsp_length = response_exception(
                             ctx, &sft,
                             ucDataBaseError, rsp);
//            rsp_length = ctx->backend->build_response_basis(&sft, rsp);
//            rsp[1] =  0x86;// |= 0x80;
//            rsp[2] = ucDataBaseError;
//            rsp_length++;
            ucDataBaseError = PREVIOUS_REQUES_WAS_NOT_A_BLOCK_TRANSFER;
        }
        break;

    case _FC_TIME_SET:
        cout << "_FC_TIME_SET" << endl;

        nuiBusyTimeCounter = MAX_MODBUS_BUFFER_BUSY_WAITING_TIME;
        while (mb_mapping->message_ready)
        {
            usleep(COMMON_DELAY_TIME);
            if (!nuiBusyTimeCounter--)
            {
                cout << "MODBUS_EXCEPTION_SLAVE_OR_SERVER_BUSY 2" << endl;
                rsp_length = response_exception(
                                 ctx, &sft,
                                 MODBUS_EXCEPTION_SLAVE_OR_SERVER_BUSY, rsp);
                break;
            }
        }

        mb_mapping->message_ready = 1;
        mb_mapping->function_code = _FC_TIME_SET;
        // (req[offset + 1]) - начало данных в сообщении.

        mb_mapping->buffer_pointer = mb_mapping->tab_auxiliary;
        memcpy((unsigned char*)(mb_mapping -> buffer_pointer),
               (unsigned char*)&(req[offset + 1]),
               PLC_DATA_BASE_BLOCK_LENGTH);

        rsp_length = ctx->backend->build_response_basis(&sft, rsp);

        nuiBusyTimeCounter = MAX_MODBUS_BUFFER_BUSY_WAITING_TIME;
        while (mb_mapping->message_ready)
        {
            usleep(COMMON_DELAY_TIME);
            if (!nuiBusyTimeCounter--)
            {
                cout << "MODBUS_EXCEPTION_SLAVE_OR_SERVER_BUSY 2" << endl;
                rsp_length = response_exception(
                                 ctx, &sft,
                                 MODBUS_EXCEPTION_SLAVE_OR_SERVER_BUSY, rsp);
                break;
            }
        }

        rsp[rsp_length++] = mb_mapping->current_message_nb_common;
        rsp_length += mb_mapping->current_message_nb_common;

        mb_mapping->message_ready = 0;
        // если широковещательный запрос, то не отвечаем.
        if (ctx->backend->filter_request(ctx, slave) == 1)
        {
            /* Filtered */
            cout << "filter_request" << endl;
            return 0;
        }
        break;

    case _FC_ONLINE_DATA_READ:
        // req[offset + 1] -
        // если бит7 = 0, то запрашиваются реперные точки - (бит0 - бит6) - адрес аналогового входа.
        // если бит7 = 1, то запрашивается ТХС и (бит0 - бит2) - относительный адрес модуля МВСТ3.
        // req[offset + 2] - требуемое количество аналоговых входов.
        //        cout << "_FC_ONLINE_DATA_READ" << endl;

        if ((((req[offset + 1]) & ANALOGUE_INPUT_ADDRESS_MASK) < MAX_HANDLED_ANALOGUE_INPUT) &&
                ((req[offset + 2]) <= (MVAI5_TXS_INPUT_QUANTITY + MVAI5_ANALOG_INPUT_QUANTITY)))
        {
            nuiBusyTimeCounter = MAX_MODBUS_BUFFER_BUSY_WAITING_TIME;
            while (mb_mapping->message_ready)
            {
                usleep(COMMON_DELAY_TIME);
                if (!nuiBusyTimeCounter--)
                {
                    cout << "MODBUS_EXCEPTION_SLAVE_OR_SERVER_BUSY 2" << endl;
                    rsp_length = response_exception(
                                     ctx, &sft,
                                     MODBUS_EXCEPTION_SLAVE_OR_SERVER_BUSY, rsp);
                    break;
                }
            }

            mb_mapping->message_ready = 1;
            mb_mapping->current_message_address_common = address;
            mb_mapping->current_message_nb_common = 0;
            mb_mapping->function_code = _FC_ONLINE_DATA_READ;
            rsp_length = ctx->backend->build_response_basis(&sft, rsp);
            // (rsp_length - 2) - адрес slave.
            // (rsp_length - 1) - функция.
            // (rsp_length) - количество байт в ответе.
            // (rsp_length + 1) - начало данных в ответе.
            mb_mapping->buffer_pointer = mb_mapping->tab_auxiliary;

            nuiBusyTimeCounter = MAX_MODBUS_BUFFER_BUSY_WAITING_TIME;
            while (mb_mapping->message_ready)
            {
                usleep(COMMON_DELAY_TIME);
                if (!nuiBusyTimeCounter--)
                {
                    cout << "MODBUS_EXCEPTION_SLAVE_OR_SERVER_BUSY 2" << endl;
                    rsp_length = response_exception(
                                     ctx, &sft,
                                     MODBUS_EXCEPTION_SLAVE_OR_SERVER_BUSY, rsp);
                    break;
                }
            }

            memcpy((&(rsp[rsp_length + 1])),
                   mb_mapping->buffer_pointer,
                   mb_mapping -> current_message_nb_common);

            rsp[rsp_length++] = mb_mapping->current_message_nb_common;
            rsp_length += mb_mapping->current_message_nb_common;

            if (mb_mapping->current_message_address_common)
            {
                rsp_length = response_exception(
                                 ctx, &sft,
                                 (mb_mapping->current_message_address_common), rsp);
            }

            mb_mapping->message_ready = 0;
        }
        else
        {
            rsp_length = response_exception(
                             ctx, &sft,
                             MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS, rsp);
        }
        break;

    case _FC_DATA_EXCHANGE:
        nb = (req[offset + 1]);

        if ((nb < 1) || (MAX_DATA_EXCHANGE_BYTE < nb))
        {
            if (ctx->debug)
            {

                fprintf(stderr,
                        "Illegal nb of values %d in modbus_data_exchange (max %d)\n",
                        nb, MODBUS_MAX_READ_REGISTERS);
            }
            cout << "MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE" << endl;
            rsp_length = response_exception(
                             ctx, &sft,
                             MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE, rsp);
            break;
        }
        else
        {
            nuiBusyTimeCounter = MAX_MODBUS_BUFFER_BUSY_WAITING_TIME;
            while (mb_mapping->message_ready)
            {
                usleep(COMMON_DELAY_TIME);
                if (!nuiBusyTimeCounter--)
                {
                    cout << "MODBUS_EXCEPTION_SLAVE_OR_SERVER_BUSY 2" << endl;
                    rsp_length = response_exception(
                                     ctx, &sft,
                                     MODBUS_EXCEPTION_SLAVE_OR_SERVER_BUSY, rsp);
                    break;
                }
            }

            mb_mapping->message_ready = 1;
            mb_mapping->current_message_address_common = 0;
            mb_mapping->current_message_nb_common = nb;
            mb_mapping->function_code = _FC_DATA_EXCHANGE;
            rsp_length = ctx->backend->build_response_basis(&sft, rsp);
            // (offset - 1) - адрес slave.
            // (offset) - функция.
            // (offset + 1) - количество байт в ответе.
            // (offset + 2) - начало данных в ответе.
            // (rsp_length - 2) - адрес slave.
            // (rsp_length - 1) - функция.
            // (rsp_length) - количество байт в ответе.
            // (rsp_length + 1) - начало данных в ответе.
            mb_mapping->buffer_pointer = mb_mapping->tab_auxiliary;
            memcpy((unsigned char*)(mb_mapping -> buffer_pointer),
                   (unsigned char*)&(req[offset + 2]),
                   nb);

            nuiBusyTimeCounter = MAX_MODBUS_BUFFER_BUSY_WAITING_TIME;
            while (mb_mapping->message_ready)
            {
                usleep(COMMON_DELAY_TIME);
                if (!nuiBusyTimeCounter--)
                {
                    cout << "MODBUS_EXCEPTION_SLAVE_OR_SERVER_BUSY 2" << endl;
                    rsp_length = response_exception(
                                     ctx, &sft,
                                     MODBUS_EXCEPTION_SLAVE_OR_SERVER_BUSY, rsp);
                    break;
                }
            }

            rsp[rsp_length++] = mb_mapping->current_message_nb_common;
            memcpy((&(rsp[rsp_length])),
                   mb_mapping->buffer_pointer,
                   mb_mapping -> current_message_nb_common);

            rsp_length += mb_mapping->current_message_nb_common;

            if (mb_mapping->current_message_address_common)
            {
                rsp_length = response_exception(
                                 ctx, &sft,
                                 (mb_mapping->current_message_address_common), rsp);
            }

            mb_mapping->message_ready = 0;
        }
        break;

    default:
        rsp_length = response_exception(ctx, &sft,
                                        MODBUS_EXCEPTION_ILLEGAL_FUNCTION,
                                        rsp);
        break;
    }
//
//    CGpio::SetPin(ctx->led_port, ctx->led_pin);
//    usleep(5000);
//    CGpio::ClearPin(ctx->led_port, ctx->led_pin);

//while (1)
//{
//  send_msg(ctx, rsp, rsp_length);
//  usleep(250000);
//}
    return send_msg(ctx, rsp, rsp_length);
}

int modbus_reply_exception(modbus_t *ctx, const unsigned char *req,
                           unsigned int exception_code)
{
    int offset = ctx->backend->header_length;
    int slave = req[offset - 1];
    int function = req[offset];
    unsigned char rsp[MAX_MESSAGE_LENGTH];
    int rsp_length;
    int dummy_length = 99;
    sft_t sft;

    if (ctx->backend->filter_request(ctx, slave) == 1)
    {
        /* Filtered */
        return 0;
    }

    sft.slave = slave;
    sft.function = function + 0x80;
    sft.t_id = ctx->backend->prepare_response_tid(req, &dummy_length);
    rsp_length = ctx->backend->build_response_basis(&sft, rsp);

    /* Positive exception code */
    if (exception_code < MODBUS_EXCEPTION_MAX)
    {
        rsp[rsp_length++] = exception_code;
        return send_msg(ctx, rsp, rsp_length);
    }
    else
    {
        errno = EINVAL;
        return -1;
    }
}

/* Reads IO status */
static int read_io_status(modbus_t *ctx, int function,
                          int addr, int nb, unsigned char *dest)
{
    int rc;
    int req_length;

    unsigned char req[_MIN_REQ_LENGTH];
    unsigned char rsp[MAX_MESSAGE_LENGTH];

    req_length = ctx->backend->build_request_basis(ctx, function, addr, nb, req);

    rc = send_msg(ctx, req, req_length);
    if (rc > 0)
    {
        int i, temp, bit;
        int pos = 0;
        int offset;
        int offset_end;

        rc = receive_msg(ctx, rsp, MSG_CONFIRMATION);
        if (rc == -1)
            return -1;

        rc = check_confirmation(ctx, req, rsp, rc);
        if (rc == -1)
            return -1;

        offset = ctx->backend->header_length + 2;
        offset_end = offset + rc;
        for (i = offset; i < offset_end; i++)
        {
            /* Shift reg hi_byte to temp */
            temp = rsp[i];

            for (bit = 0x01; (bit & 0xff) && (pos < nb);)
            {
                dest[pos++] = (temp & bit) ? TRUE : FALSE;
                bit = bit << 1;
            }

        }
    }

    return rc;
}

/* Reads the boolean status of bits and sets the array elements
   in the destination to TRUE or FALSE (single bits). */
int modbus_read_bits(modbus_t *ctx, int addr, int nb, unsigned char *dest)
{
    int rc;

    if (nb > MODBUS_MAX_READ_BITS)
    {
        if (ctx->debug)
        {
            fprintf(stderr,
                    "ERROR Too many bits requested (%d > %d)\n",
                    nb, MODBUS_MAX_READ_BITS);
        }
        errno = EMBMDATA;
        return -1;
    }

    rc = read_io_status(ctx, _FC_READ_COILS, addr, nb, dest);

    if (rc == -1)
        return -1;
    else
        return nb;
}


/* Same as modbus_read_bits but reads the remote device input table */
int modbus_read_input_bits(modbus_t *ctx, int addr, int nb, unsigned char *dest)
{
    int rc;

    if (nb > MODBUS_MAX_READ_BITS)
    {
        if (ctx->debug)
        {
            fprintf(stderr,
                    "ERROR Too many discrete inputs requested (%d > %d)\n",
                    nb, MODBUS_MAX_READ_BITS);
        }
        errno = EMBMDATA;
        return -1;
    }

    rc = read_io_status(ctx, _FC_READ_DISCRETE_INPUTS, addr, nb, dest);

    if (rc == -1)
        return -1;
    else
        return nb;
}

/* Reads the data from a remove device and put that data into an array */
static int read_registers(modbus_t *ctx, int function, int addr, int nb,
                          unsigned short *dest)
{
    int rc;
    int req_length;
    unsigned char req[_MIN_REQ_LENGTH];
    unsigned char rsp[MAX_MESSAGE_LENGTH];

    if (nb > MODBUS_MAX_READ_REGISTERS)
    {
        if (ctx->debug)
        {
            fprintf(stderr,
                    "ERROR Too many registers requested (%d > %d)\n",
                    nb, MODBUS_MAX_READ_REGISTERS);
        }
        errno = EMBMDATA;
        return -1;
    }

    req_length = ctx->backend->build_request_basis(ctx, function, addr, nb, req);
    rc = send_msg(ctx, req, req_length);
    if (rc > 0)
    {
        int offset;
        int i;

        rc = receive_msg(ctx, rsp, MSG_CONFIRMATION);
        if (rc == -1)
        {
            return -1;
        }

        rc = check_confirmation(ctx, req, rsp, rc);
        if (rc == -1)
        {
            return -1;
        }

        offset = ctx->backend->header_length;

        for (i = 0; i < rc; i++)
        {
            /* shift reg hi_byte to temp OR with lo_byte */
            dest[i] = (rsp[offset + 2 + (i << 1)] << 8) |
                      rsp[offset + 3 + (i << 1)];
        }
    }
    return rc;
}

/* Reads the holding registers of remote device and put the data into an
   array */
int modbus_read_registers(modbus_t *ctx, int addr, int nb, unsigned short *dest)
{
    int status;

    if (nb > MODBUS_MAX_READ_REGISTERS)
    {
        if (ctx->debug)
        {
            fprintf(stderr,
                    "ERROR Too many registers requested (%d > %d)\n",
                    nb, MODBUS_MAX_READ_REGISTERS);
        }
        errno = EMBMDATA;
        return -1;
    }

    status = read_registers(ctx, _FC_READ_HOLDING_REGISTERS,
                            addr, nb, dest);
    return status;
}

/* Reads the input registers of remote device and put the data into an array */
int modbus_read_input_registers(modbus_t *ctx, int addr, int nb,
                                unsigned short *dest)
{
    int status;

    if (nb > MODBUS_MAX_READ_REGISTERS)
    {
        fprintf(stderr,
                "ERROR Too many input registers requested (%d > %d)\n",
                nb, MODBUS_MAX_READ_REGISTERS);
        errno = EMBMDATA;
        return -1;
    }

    status = read_registers(ctx, _FC_READ_INPUT_REGISTERS,
                            addr, nb, dest);

    return status;
}

/* Write a value to the specified register of the remote device.
   Used by write_bit and write_register */
static int write_single(modbus_t *ctx, int function, int addr, int value)
{
    int rc;
    int req_length;
    unsigned char req[_MIN_REQ_LENGTH];

    req_length = ctx->backend->build_request_basis(ctx, function, addr, value, req);

    rc = send_msg(ctx, req, req_length);
    if (rc > 0)
    {
        /* Used by write_bit and write_register */
        unsigned char rsp[MAX_MESSAGE_LENGTH];

        rc = receive_msg(ctx, rsp, MSG_CONFIRMATION);
        if (rc == -1)
            return -1;

        rc = check_confirmation(ctx, req, rsp, rc);
    }

    return rc;
}

/* Turns ON or OFF a single bit of the remote device */
int modbus_write_bit(modbus_t *ctx, int addr, int status)
{
    return write_single(ctx, _FC_WRITE_SINGLE_COIL, addr,
                        status ? 0xFF00 : 0);
}

/* Writes a value in one register of the remote device */
int modbus_write_register(modbus_t *ctx, int addr, int value)
{
    return write_single(ctx, _FC_WRITE_SINGLE_REGISTER, addr, value);
}

/* Write the bits of the array in the remote device */
int modbus_write_bits(modbus_t *ctx, int addr, int nb, const unsigned char *src)
{
    int rc;
    int i;
    int byte_count;
    int req_length;
    int bit_check = 0;
    int pos = 0;

    unsigned char req[MAX_MESSAGE_LENGTH];

    if (nb > MODBUS_MAX_WRITE_BITS)
    {
        if (ctx->debug)
        {
            fprintf(stderr, "ERROR Writing too many bits (%d > %d)\n",
                    nb, MODBUS_MAX_WRITE_BITS);
        }
        errno = EMBMDATA;
        return -1;
    }

    req_length = ctx->backend->build_request_basis(ctx,
                 _FC_WRITE_MULTIPLE_COILS,
                 addr, nb, req);
    byte_count = (nb / 8) + ((nb % 8) ? 1 : 0);
    req[req_length++] = byte_count;

    for (i = 0; i < byte_count; i++)
    {
        int bit;

        bit = 0x01;
        req[req_length] = 0;

        while ((bit & 0xFF) && (bit_check++ < nb))
        {
            if (src[pos++])
                req[req_length] |= bit;
            else
                req[req_length] &=~ bit;

            bit = bit << 1;
        }
        req_length++;
    }

    rc = send_msg(ctx, req, req_length);
    if (rc > 0)
    {
        unsigned char rsp[MAX_MESSAGE_LENGTH];

        rc = receive_msg(ctx, rsp, MSG_CONFIRMATION);
        if (rc == -1)
            return -1;

        rc = check_confirmation(ctx, req, rsp, rc);
    }


    return rc;
}

/* Write the values from the array to the registers of the remote device */
int modbus_write_registers(modbus_t *ctx, int addr, int nb, const unsigned short *src)
{
    int rc;
    int i;
    int req_length;
    int byte_count;

    unsigned char req[MAX_MESSAGE_LENGTH];

    if (nb > MODBUS_MAX_WRITE_REGISTERS)
    {
        if (ctx->debug)
        {
            fprintf(stderr,
                    "ERROR Trying to write to too many registers (%d > %d)\n",
                    nb, MODBUS_MAX_WRITE_REGISTERS);
        }
        errno = EMBMDATA;
        return -1;
    }

    req_length = ctx->backend->build_request_basis(ctx,
                 _FC_WRITE_MULTIPLE_REGISTERS,
                 addr, nb, req);
    byte_count = nb * 2;
    req[req_length++] = byte_count;

    for (i = 0; i < nb; i++)
    {
        req[req_length++] = src[i] >> 8;
        req[req_length++] = src[i] & 0x00FF;
    }

    rc = send_msg(ctx, req, req_length);
    if (rc > 0)
    {
        unsigned char rsp[MAX_MESSAGE_LENGTH];

        rc = receive_msg(ctx, rsp, MSG_CONFIRMATION);
        if (rc == -1)
            return -1;

        rc = check_confirmation(ctx, req, rsp, rc);
    }

    return rc;
}

/* Write multiple registers from src array to remote device and read multiple
   registers from remote device to dest array. */
int modbus_write_and_read_registers(modbus_t *ctx,
                                    int write_addr, int write_nb, const unsigned short *src,
                                    int read_addr, int read_nb, unsigned short *dest)

{
    int rc;
    int req_length;
    int i;
    int byte_count;
    unsigned char req[MAX_MESSAGE_LENGTH];
    unsigned char rsp[MAX_MESSAGE_LENGTH];

    if (write_nb > MODBUS_MAX_RW_WRITE_REGISTERS)
    {
        if (ctx->debug)
        {
            fprintf(stderr,
                    "ERROR Too many registers to write (%d > %d)\n",
                    write_nb, MODBUS_MAX_RW_WRITE_REGISTERS);
        }
        errno = EMBMDATA;
        return -1;
    }

    if (read_nb > MODBUS_MAX_READ_REGISTERS)
    {
        if (ctx->debug)
        {
            fprintf(stderr,
                    "ERROR Too many registers requested (%d > %d)\n",
                    read_nb, MODBUS_MAX_READ_REGISTERS);
        }
        errno = EMBMDATA;
        return -1;
    }
    req_length = ctx->backend->build_request_basis(ctx,
                 _FC_WRITE_AND_READ_REGISTERS,
                 read_addr, read_nb, req);

    req[req_length++] = write_addr >> 8;
    req[req_length++] = write_addr & 0x00ff;
    req[req_length++] = write_nb >> 8;
    req[req_length++] = write_nb & 0x00ff;
    byte_count = write_nb * 2;
    req[req_length++] = byte_count;

    for (i = 0; i < write_nb; i++)
    {
        req[req_length++] = src[i] >> 8;
        req[req_length++] = src[i] & 0x00FF;
    }

    rc = send_msg(ctx, req, req_length);
    if (rc > 0)
    {
        int offset;

        rc = receive_msg(ctx, rsp, MSG_CONFIRMATION);
        if (rc == -1)
            return -1;

        rc = check_confirmation(ctx, req, rsp, rc);
        if (rc == -1)
            return -1;

        offset = ctx->backend->header_length;

        /* If rc is negative, the loop is jumped ! */
        for (i = 0; i < rc; i++)
        {
            /* shift reg hi_byte to temp OR with lo_byte */
            dest[i] = (rsp[offset + 2 + (i << 1)] << 8) |
                      rsp[offset + 3 + (i << 1)];
        }
    }

    return rc;
}

/* Send a request to get the slave ID of the device (only available in serial
   communication). */
int modbus_report_slave_id(modbus_t *ctx, unsigned char *dest)
{
    int rc;
    int req_length;
    unsigned char req[_MIN_REQ_LENGTH];

    req_length = ctx->backend->build_request_basis(ctx, _FC_REPORT_SLAVE_ID,
                 0, 0, req);

    /* HACKISH, addr and count are not used */
    req_length -= 4;

    rc = send_msg(ctx, req, req_length);
    if (rc > 0)
    {
        int i;
        int offset;
        unsigned char rsp[MAX_MESSAGE_LENGTH];

        rc = receive_msg(ctx, rsp, MSG_CONFIRMATION);
        if (rc == -1)
            return -1;

        rc = check_confirmation(ctx, req, rsp, rc);
        if (rc == -1)
            return -1;

        offset = ctx->backend->header_length + 2;

        /* Byte count, slave id, run indicator status,
           additional data */
        for (i=0; i < rc; i++)
        {
            dest[i] = rsp[offset + i];
        }
    }

    return rc;
}

/* Data exchange */
int16_t modbus_data_exchange(modbus_t *ctx, int16_t nb, uint8_t *src, uint8_t *dest)
{
    int16_t rc;
    int16_t iOffset;
    int16_t req_length;

    uint8_t req[MODBUS_RTU_MAX_ADU_LENGTH];

    if (nb > MAX_DATA_EXCHANGE_BYTE)
    {
        if (ctx->debug)
        {
            fprintf(stderr,
                    "ERROR Trying to write to too many bytes (%d > %d)\n",
                    nb, MODBUS_MAX_WRITE_REGISTERS);
        }
        errno = EMBMDATA;
        return -1;
    }

    req_length = ctx->backend->build_request_basis(ctx,
                 _FC_DATA_EXCHANGE,
                 0, 0, req);
//    req[0] = ctx->slave;
//    // function code.
//    req[1] = _FC_DATA_EXCHANGE;
//    // message length.
//    req[2] = nb;
    // функции build_request_basis готовит адрес устройства, код функции, адрес и количество регистров.
    // и возвращает количество байт в заголовке.
    // в функции modbus_data_exchange не передаётся адрес и количество регистров.
    req_length = req_length -
                 (MODBUS_FUNCTION_REGISTER_ADDRESS_LENGTH +
                  MODBUS_FUNCTION_REGISTER_QUANTITY_LENGTH);
    req[req_length++] = nb;

    for (int16_t i = 0; i < nb; i++)
    {
        req[req_length++] = src[i];
    }

//    cout << "modbus_data_exchange req" << endl;
//    unsigned char *pucSourceTemp;
//    pucSourceTemp = (unsigned char*)req;
//    for(int i=0; i<64; )
//    {
//        for(int j=0; j<8; j++)
//        {
//            cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
//        }
//        cout << endl;
//        i += 8;
//    }

    rc = send_msg(ctx, req, req_length);
    if (rc > 0)
    {
        uint8_t rsp[MODBUS_RTU_MAX_ADU_LENGTH];

        rc = receive_msg(ctx, rsp, MSG_CONFIRMATION);

//        cout << "modbus_data_exchange rsp" << endl;
//        unsigned char *pucSourceTemp;
//        pucSourceTemp = (unsigned char*)rsp;
//        for(int i=0; i<64; )
//        {
//            for(int j=0; j<8; j++)
//            {
//                cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
//            }
//            cout << endl;
//            i += 8;
//        }

        if (rc == -1)
        {
//            cout << "receive_msg == -1" << endl;
            return -1;
        }

        rc = check_confirmation(ctx, req, rsp, rc);
        if (rc == -1)
        {
//            cout << "check_confirmation == -1" << endl;
            return -1;
        }

        iOffset = ctx->backend->header_length;

//        if (req[iOffset - 1] != rsp[iOffset - 1] || req[iOffset] != rsp[iOffset])
//        {
////        if (ctx->debug)
////        {
////            fprintf(stderr, "Invalid TID received 0x%X (not 0x%X)\n",
////                    (rsp[0] << 8) + rsp[1], (req[0] << 8) + req[1]);
////        }
//            errno = EMBBADDATA;
////            cout << "errno = EMBBADDATA" << endl;
//            return -1;
//        }

        // message length.
        nb = rsp[iOffset + 1];
        req_length = (iOffset +
                      //MODBUS_ADDRESS_LENGTH +
                      MODBUS_FUNCTION_CODE_LENGTH +
                      MODBUS_BYTE_QUANTITY_LENGTH);
        for (int16_t i = 0; i < nb; i++)
        {
            dest[i] = rsp[req_length++];
        }
    }
    return nb;
}

void _modbus_init_common(modbus_t *ctx)
{
    /* Slave and socket are initialized to -1 */
    ctx->slave = -1;
    ctx->s = -1;

    ctx->debug = FALSE;
//    ctx->error_recovery = MODBUS_ERROR_RECOVERY_NONE;
    ctx->error_recovery = MODBUS_ERROR_RECOVERY_LINK;


    ctx->response_timeout.tv_sec = 0;
    ctx->response_timeout.tv_usec = _RESPONSE_TIMEOUT;

//    ctx->receive_timeout.tv_sec = 0;
//    ctx->receive_timeout.tv_usec = _MODBUS_RTU_35_TIMEOUT;

    ctx->receive_timeout.tv_sec = 0;
    ctx->receive_timeout.tv_usec = _RECEIVE_TIMEOUT;

    ctx->modbus_timeout.tv_sec = 0;
    ctx->modbus_timeout.tv_usec = _MODBUS_RTU_35_TIMEOUT;

    ctx->byte_timeout.tv_sec = 0;
    ctx->byte_timeout.tv_usec = _BYTE_TIMEOUT;
}

/* Define the slave number */
int modbus_set_slave(modbus_t *ctx, int slave)
{
    return ctx->backend->set_slave(ctx, slave);
}

int modbus_set_error_recovery(modbus_t *ctx,
                              modbus_error_recovery_mode error_recovery)
{
    /* The type of modbus_error_recovery_mode is unsigned enum */
    ctx->error_recovery = (unsigned char) error_recovery;
    return 0;
}

void modbus_set_socket(modbus_t *ctx, int socket)
{
    ctx->s = socket;
}

int modbus_get_socket(modbus_t *ctx)
{
    return ctx->s;
}

/* Get the timeout interval used to wait for a response */
void modbus_get_response_timeout(modbus_t *ctx, struct timeval *timeout)
{
    *timeout = ctx->response_timeout;
}

void modbus_set_response_timeout(modbus_t *ctx, const struct timeval *timeout)
{
    ctx->response_timeout = *timeout;
}

/* Get the timeout interval between two consecutive bytes of a message */
void modbus_get_byte_timeout(modbus_t *ctx, struct timeval *timeout)
{
    *timeout = ctx->byte_timeout;
}

void modbus_set_byte_timeout(modbus_t *ctx, const struct timeval *timeout)
{
    ctx->byte_timeout = *timeout;
}

int modbus_get_header_length(modbus_t *ctx)
{
    return ctx->backend->header_length;
}

int modbus_connect(modbus_t *ctx)
{
    return ctx->backend->connect(ctx);
}

void modbus_close(modbus_t *ctx)
{
    if (ctx == NULL)
        return;

    ctx->backend->close(ctx);
}

void modbus_free(modbus_t *ctx)
{
    if (ctx == NULL)
        return;

    free(ctx->backend_data);
    free(ctx);
}

void modbus_set_debug(modbus_t *ctx, int boolean)
{
    ctx->debug = boolean;
}

/* Allocates 4 arrays to store bits, input bits, registers and inputs
   registers. The pointers are stored in modbus_mapping structure.

   The modbus_mapping_new() function shall return the new allocated structure if
   successful. Otherwise it shall return NULL and set errno to ENOMEM. */
modbus_mapping_t* modbus_mapping_new(int nb_bits, int nb_input_bits,
                                     int nb_registers, int nb_input_registers)
{
    modbus_mapping_t *mb_mapping;

    mb_mapping = (modbus_mapping_t *)malloc(sizeof(modbus_mapping_t));
    if (mb_mapping == NULL)
    {
        return NULL;
    }

    /* 0X */
    mb_mapping->nb_bits = nb_bits;
    if (nb_bits == 0)
    {
        mb_mapping->tab_bits = NULL;
    }
    else
    {
        /* Negative number raises a POSIX error */
        mb_mapping->tab_bits =
            (unsigned char *) malloc(nb_bits * sizeof(unsigned char));
        if (mb_mapping->tab_bits == NULL)
        {
            free(mb_mapping);
            return NULL;
        }
        memset(mb_mapping->tab_bits, 0, nb_bits * sizeof(unsigned char));
    }

    /* 1X */
    mb_mapping->nb_input_bits = nb_input_bits;
    if (nb_input_bits == 0)
    {
        mb_mapping->tab_input_bits = NULL;
    }
    else
    {
        mb_mapping->tab_input_bits =
            (unsigned char *) malloc(nb_input_bits * sizeof(unsigned char));
        if (mb_mapping->tab_input_bits == NULL)
        {
            free(mb_mapping->tab_bits);
            free(mb_mapping);
            return NULL;
        }
        memset(mb_mapping->tab_input_bits, 0, nb_input_bits * sizeof(unsigned char));
    }

    /* 4X */
    mb_mapping->nb_registers = nb_registers;
    if (nb_registers == 0)
    {
        mb_mapping->tab_registers = NULL;
    }
    else
    {
        mb_mapping->tab_registers =
            (unsigned short *) malloc(nb_registers * sizeof(unsigned short));
        if (mb_mapping->tab_registers == NULL)
        {
            free(mb_mapping->tab_input_bits);
            free(mb_mapping->tab_bits);
            free(mb_mapping);
            return NULL;
        }
        memset(mb_mapping->tab_registers, 0, nb_registers * sizeof(unsigned short));
    }

    /* 3X */
    mb_mapping->nb_input_registers = nb_input_registers;
    if (nb_input_registers == 0)
    {
        mb_mapping->tab_input_registers = NULL;
    }
    else
    {
        mb_mapping->tab_input_registers =
            (unsigned short *) malloc(nb_input_registers * sizeof(unsigned short));
        if (mb_mapping->tab_input_registers == NULL)
        {
            free(mb_mapping->tab_registers);
            free(mb_mapping->tab_input_bits);
            free(mb_mapping->tab_bits);
            free(mb_mapping);
            return NULL;
        }
        memset(mb_mapping->tab_input_registers, 0,
               nb_input_registers * sizeof(unsigned short));
    }

    return mb_mapping;
}

/* Frees the 4 arrays */
void modbus_mapping_free(modbus_mapping_t *mb_mapping)
{
    if (mb_mapping == NULL)
    {
        return;
    }

    free(mb_mapping->tab_input_registers);
    free(mb_mapping->tab_registers);
    free(mb_mapping->tab_input_bits);
    free(mb_mapping->tab_bits);
    free(mb_mapping);
}

#ifndef HAVE_STRLCPY
/*
 * Function strlcpy was originally developed by
 * Todd C. Miller <Todd.Miller@courtesan.com> to simplify writing secure code.
 * See ftp://ftp.openbsd.org/pub/OpenBSD/src/lib/libc/string/strlcpy.3
 * for more information.
 *
 * Thank you Ulrich Drepper... not!
 *
 * Copy src to string dest of size dest_size.  At most dest_size-1 characters
 * will be copied.  Always NUL terminates (unless dest_size == 0).  Returns
 * strlen(src); if retval >= dest_size, truncation occurred.
 */
size_t strlcpy(char *dest, const char *src, size_t dest_size)
{
    register char *d = dest;
    register const char *s = src;
    register size_t n = dest_size;

    /* Copy as many bytes as will fit */
    if (n != 0 && --n != 0)
    {
        do
        {
            if ((*d++ = *s++) == 0)
                break;
        }
        while (--n != 0);
    }

    /* Not enough room in dest, add NUL and traverse rest of src */
    if (n == 0)
    {
        if (dest_size != 0)
            *d = '\0'; /* NUL-terminate dest */
        while (*s++)
            ;
    }

    return (s - src - 1); /* count does not include NUL */
}
#endif

/* Sets many bits from a single byte value (all 8 bits of the byte value are
   set) */
void modbus_set_bits_from_byte(unsigned char *dest, int index, const unsigned char value)
{
    int i;

    for (i=0; i<8; i++)
    {
        dest[index+i] = (value & (1 << i)) ? 1 : 0;
    }
}

/* Sets many bits from a table of bytes (only the bits between index and
   index + nb_bits are set) */
void modbus_set_bits_from_bytes(unsigned char *dest, int index, unsigned int nb_bits,
                                const unsigned char *tab_byte)
{
    int i;
    int shift = 0;

    for (i = index; i < index + nb_bits; i++)
    {
        dest[i] = tab_byte[(i - index) / 8] & (1 << shift) ? 1 : 0;
        /* gcc doesn't like: shift = (++shift) % 8; */
        shift++;
        shift %= 8;
    }
}

/* Gets the byte value from many bits.
   To obtain a full byte, set nb_bits to 8. */
unsigned char modbus_get_byte_from_bits(const unsigned char *src, int index,
                                        unsigned int nb_bits)
{
    int i;
    unsigned char value = 0;

    if (nb_bits > 8)
    {
        /* Assert is ignored if NDEBUG is set */
        assert(nb_bits < 8);
        nb_bits = 8;
    }

    for (i=0; i < nb_bits; i++)
    {
        value |= (src[index+i] << i);
    }

    return value;
}

/* Get a float from 4 bytes in Modbus format */
float modbus_get_float(const unsigned short *src)
{
    float f = 0.0f;
    uint32_t i;

    i = (((uint32_t)src[1]) << 16) + src[0];
    memcpy(&f, &i, sizeof(float));

    return f;
}

/* Set a float to 4 bytes in Modbus format */
void modbus_set_float(float f, unsigned short *dest)
{
    uint32_t i = 0;

    memcpy(&i, &f, sizeof(uint32_t));
    dest[0] = (unsigned short)i;
    dest[1] = (unsigned short)(i >> 16);
}


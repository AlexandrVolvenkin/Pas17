/*
 * Copyright © 2001-2011 Stephane Raimbault <stephane.raimbault@gmail.com>
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

#ifndef _MODBUS_H_
#define _MODBUS_H_

/* Add this for macros that defined unix flavor */
#if (defined(__unix__) || defined(unix)) && !defined(USG)
#include <sys/param.h>
#endif

#ifndef _MSC_VER
#include <stdint.h>
#include <sys/time.h>
#else
#include "stdint.h"
#include <time.h>
#endif

#if defined(_WIN32)
#include <windows.h>
#else
#include <termios.h>
#endif

#include "modbus-version.h"
//#include "modbus-private.h"

//#include "modbus-tcp.h"
//#include "modbus-rtu.h"

typedef int ssize_t;

//#ifdef  __cplusplus
//# define MODBUS_BEGIN_DECLS  extern "C" {
//# define MODBUS_END_DECLS    }
//#else
//# define MODBUS_BEGIN_DECLS
//# define MODBUS_END_DECLS
//#endif

#include "../PasNewConfig.h"
#include "../Platform.h"
//
//#ifdef __cplusplus
//extern "C" {
//#endif

//MODBUS_BEGIN_DECLS

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef OFF
#define OFF 0
#endif

#ifndef ON
#define ON 1
#endif

#define HAVE_DECL_TIOCSRS485 1

/* Max between RTU and TCP max adu length (so TCP) */
#define MAX_MESSAGE_LENGTH 260

#define COMMON_DELAY_TIME    100
#define MAX_MODBUS_BUFFER_BUSY_WAITING_TIME 5000 //COMMON_DELAY_TIME * 5000 = 500ms
#define MAX_MAIN_BUFFER_BUSY_WAITING_TIME 10 //COMMON_DELAY_TIME * 20 = 2ms

#define MODBUS_BROADCAST_ADDRESS    0

/* Modbus_Application_Protocol_V1_1b.pdf (chapter 6 section 1 page 12)
 * Quantity of Coils to read (2 bytes): 1 to 2000 (0x7D0)
 * (chapter 6 section 11 page 29)
 * Quantity of Coils to write (2 bytes): 1 to 1968 (0x7B0)
 */
#define MODBUS_MAX_READ_BITS              2000
#define MODBUS_MAX_WRITE_BITS             1968

/* Modbus_Application_Protocol_V1_1b.pdf (chapter 6 section 3 page 15)
 * Quantity of Registers to read (2 bytes): 1 to 125 (0x7D)
 * (chapter 6 section 12 page 31)
 * Quantity of Registers to write (2 bytes) 1 to 123 (0x7B)
 * (chapter 6 section 17 page 38)
 * Quantity of Registers to write in R/W registers (2 bytes) 1 to 121 (0x79)
 */
#define MODBUS_MAX_READ_REGISTERS          125
#define MODBUS_MAX_WRITE_REGISTERS         123
#define MODBUS_MAX_RW_WRITE_REGISTERS      121

#define MODBUS_ADDRESS_LENGTH 1
#define MODBUS_FUNCTION_CODE_LENGTH 1
#define MODBUS_BYTE_QUANTITY_LENGTH 1
#define MODBUS_FUNCTION_REGISTER_ADDRESS_LENGTH 2
#define MODBUS_FUNCTION_REGISTER_QUANTITY_LENGTH 2
#define MODBUS_CRC_LENGTH 2
#define MAX_DATA_EXCHANGE_BYTE      (MODBUS_RTU_MAX_ADU_LENGTH - \
                                     MODBUS_ADDRESS_LENGTH - \
                                      MODBUS_FUNCTION_CODE_LENGTH - \
                                       MODBUS_BYTE_QUANTITY_LENGTH - \
                                       MODBUS_CRC_LENGTH)

/* Random number to avoid errno conflicts */
#define MODBUS_ENOBASE 112345678

/* It's not really the minimal length (the real one is report slave ID
 * in RTU (4 bytes)) but it's a convenient size to use in RTU or TCP
 * communications to read many values or write a single one.
 * Maximum between :
 * - HEADER_LENGTH_TCP (7) + function (1) + address (2) + number (2)
 * - HEADER_LENGTH_RTU (1) + function (1) + address (2) + number (2) + CRC (2)
 */
#define _MIN_REQ_LENGTH 12
#define _MIN_MESSAGE_LENGTH 4

#define _REPORT_SLAVE_ID SERVER_ID//180

#define _MODBUS_EXCEPTION_RSP_LENGTH 5

/* Timeouts in microsecond (0.5 s) */
#define _RESPONSE_TIMEOUT    1000000UL
#define _BYTE_TIMEOUT        1000000UL
#define _RECEIVE_TIMEOUT    15000000UL
#define _MODBUS_RTU_35_TIMEOUT 5000UL

/* Function codes */
#define _FC_READ_COILS                0x01
#define _FC_READ_DISCRETE_INPUTS      0x02
#define _FC_READ_HOLDING_REGISTERS    0x03
#define _FC_READ_INPUT_REGISTERS      0x04
#define _FC_WRITE_SINGLE_COIL         0x05
#define _FC_WRITE_SINGLE_REGISTER     0x06
#define _FC_READ_EXCEPTION_STATUS     0x07
#define _FC_WRITE_MULTIPLE_COILS      0x0F
#define _FC_WRITE_MULTIPLE_REGISTERS  0x10
#define _FC_REPORT_SLAVE_ID           0x11
#define _FC_WRITE_AND_READ_REGISTERS  0x17

#define _FC_PROGRAMMING_COMPLETION_REQUEST  0x0E
#define _FC_CONFIGURATION_REQUEST  0x11
#define _FC_DATA_BASE_ARCHIVE_BLOCK_READ  0x43
#define _FC_DATA_BASE_READ  0x44
#define _FC_DATA_BASE_WRITE  0x45
#define _FC_TIME_SET  0x46
#define _FC_ONLINE_DATA_READ  0x47

#define _FC_WRITE_DATA  0x41
#define _FC_READ_DATA  0x42
#define _FC_DATA_EXCHANGE  0x41

#define _MODBUS_TCP_PI_NODE_LENGTH    1025
#define _MODBUS_TCP_PI_SERVICE_LENGTH   32

typedef enum
{
    _MODBUS_BACKEND_TYPE_RTU=1,
    _MODBUS_BACKEND_TYPE_TCP
} modbus_bakend_type_t;

/* Protocol exceptions */
enum
{
    MODBUS_EXCEPTION_ILLEGAL_FUNCTION = 0x01,
    MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS,
    MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE,
    MODBUS_EXCEPTION_SLAVE_OR_SERVER_FAILURE,
    MODBUS_EXCEPTION_ACKNOWLEDGE,
    MODBUS_EXCEPTION_SLAVE_OR_SERVER_BUSY,
    MODBUS_EXCEPTION_NEGATIVE_ACKNOWLEDGE,
    MODBUS_EXCEPTION_MEMORY_PARITY,
    MODBUS_EXCEPTION_NOT_DEFINED,
    MODBUS_EXCEPTION_GATEWAY_PATH,
    MODBUS_EXCEPTION_GATEWAY_TARGET,
    MODBUS_EXCEPTION_MAX
};

#define EMBXILFUN  (MODBUS_ENOBASE + MODBUS_EXCEPTION_ILLEGAL_FUNCTION)
#define EMBXILADD  (MODBUS_ENOBASE + MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS)
#define EMBXILVAL  (MODBUS_ENOBASE + MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE)
#define EMBXSFAIL  (MODBUS_ENOBASE + MODBUS_EXCEPTION_SLAVE_OR_SERVER_FAILURE)
#define EMBXACK    (MODBUS_ENOBASE + MODBUS_EXCEPTION_ACKNOWLEDGE)
#define EMBXSBUSY  (MODBUS_ENOBASE + MODBUS_EXCEPTION_SLAVE_OR_SERVER_BUSY)
#define EMBXNACK   (MODBUS_ENOBASE + MODBUS_EXCEPTION_NEGATIVE_ACKNOWLEDGE)
#define EMBXMEMPAR (MODBUS_ENOBASE + MODBUS_EXCEPTION_MEMORY_PARITY)
#define EMBXGPATH  (MODBUS_ENOBASE + MODBUS_EXCEPTION_GATEWAY_PATH)
#define EMBXGTAR   (MODBUS_ENOBASE + MODBUS_EXCEPTION_GATEWAY_TARGET)

/* Native libmodbus error codes */
#define EMBBADCRC  (EMBXGTAR + 1)
#define EMBBADDATA (EMBXGTAR + 2)
#define EMBBADEXC  (EMBXGTAR + 3)
#define EMBUNKEXC  (EMBXGTAR + 4)
#define EMBMDATA   (EMBXGTAR + 5)

enum
{
    TCP,
    TCP_PI,
    RTU
};

extern const unsigned int libmodbus_version_major;
extern const unsigned int libmodbus_version_minor;
extern const unsigned int libmodbus_version_micro;

struct modbus_t
{
// general
    /* Slave address */
    int slave;
    /* Socket or file descriptor */
    int s;
    int debug;
    int error_recovery;
    struct timeval response_timeout;
    struct timeval receive_timeout;
    struct timeval modbus_timeout;
    struct timeval byte_timeout;
    const struct _modbus_backend *backend;
    void *backend_data;
    unsigned char led_port;
    unsigned char led_pin;
// _modbus_rtu
    /* Device: "/dev/ttyS0", "/dev/ttyUSB0" or "/dev/tty.USA19*" on Mac OS X for
       KeySpan USB<->Serial adapters this string had to be made bigger on OS X
       as the directory+file name was bigger than 19 bytes. Making it 67 bytes
       for now, but OS X does support 256 byte file names. May become a problem
       in the future. */
#if defined(__APPLE_CC__)
    char device[64];
#else
    char device[16];
#endif
    /* Bauds: 9600, 19200, 57600, 115200, etc */
    int baud;
    /* Data bit */
    unsigned char data_bit;
    /* Stop bit */
    unsigned char stop_bit;
    /* Parity: 'N', 'O', 'E' */
    char parity;
#if defined(_WIN32)
    struct win32_ser w_ser;
    DCB old_dcb;
#else
    /* Save old termios settings */
    struct termios old_tios;
#endif
#if HAVE_DECL_TIOCSRS485
    int serial_mode;
#endif
// _modbus_tcp
    /* TCP port */
    int port;
    /* IP address */
    char ip[16];
// _modbus_tcp_pi
///* TCP port */
//int port;
    /* Node */
    char node[_MODBUS_TCP_PI_NODE_LENGTH];
    /* Service */
    char service[_MODBUS_TCP_PI_SERVICE_LENGTH];
};
typedef struct modbus_t _modbus_t;
typedef struct modbus_t modbus_t;

/* This structure reduces the number of params in functions and so
 * optimizes the speed of execution (~ 37%). */
//typedef struct _sft
struct _sft_t
{
    int slave;
    int function;
    int t_id;
};// sft_t;
typedef struct _sft_t _sft_t;
typedef struct _sft_t sft_t;

//typedef struct _modbus_backend
struct _modbus_backend
{
    unsigned int backend_type;
    unsigned int header_length;
    unsigned int checksum_length;
    unsigned int max_adu_length;
    int (*set_slave) (modbus_t *ctx, int slave);
    int (*build_request_basis) (modbus_t *ctx, int function, int addr,
                                int nb, unsigned char *req);
    int (*build_response_basis) (sft_t *sft, unsigned char *rsp);
    int (*prepare_response_tid) (const unsigned char *req, int *req_length);
    int (*send_msg_pre) (unsigned char *req, int req_length);
    ssize_t (*send) (modbus_t *ctx, const unsigned char *req, int req_length);
    ssize_t (*recv) (modbus_t *ctx, unsigned char *rsp, int rsp_length);
    int (*check_integrity) (modbus_t *ctx, unsigned char *msg,
                            const int msg_length);
    int (*pre_check_confirmation) (modbus_t *ctx, const unsigned char *req,
                                   const unsigned char *rsp, int rsp_length);
    int (*connect) (modbus_t *ctx);
    void (*close) (modbus_t *ctx);
    int (*flush) (modbus_t *ctx);
    int (*select) (modbus_t *ctx, fd_set *rfds, struct timeval *tv, int msg_length);
    int (*filter_request) (modbus_t *ctx, int slave);
};// modbus_backend_t;
typedef struct _modbus_backend _modbus_backend;
typedef struct _modbus_backend modbus_backend_t;

// контекст Modbus интерфейсов(память под массивы Modbus, служебные переменные, флаги, и т.д.).
struct modbus_mapping_t
//struct _modbus_mapping_t
{
    int nb_bits; // количество Coils.
    int nb_input_bits; // количество DiscreteInput.
    int nb_input_registers; // количество InputRegisters.
    int nb_registers; // количество HoldingRegisters.
    unsigned char *tab_bits; // указатель на буфер Coils.
    unsigned char *tab_input_bits; // указатель на буфер DiscreteInput.
    unsigned short *tab_input_registers; // указатель на буфер InputRegisters.
    unsigned short *tab_registers; // указатель на буфер HoldingRegisters.
    unsigned char *tab_auxiliary; // указатель на вспомогательный буфер.

    unsigned short current_message_nb_bits; // длина поступившего сообщения Coils.
    unsigned short current_message_nb_input_bits; // длина поступившего сообщения DiscreteInput.
    unsigned short current_message_nb_input_registers; // длина поступившего сообщения InputRegisters.
    unsigned short current_message_nb_registers; // длина поступившего сообщения HoldingRegisters.
    unsigned short current_message_address_bits; // адрес в поступившем сообщениии Coils.
    unsigned short current_message_address_input_bits; // адрес в поступившем сообщениии DiscreteInput.
    unsigned short current_message_address_input_registers; // адрес в поступившем сообщениии InputRegisters.
    unsigned short current_message_address_registers; // адрес в поступившем сообщениии HoldingRegisters.

    unsigned short current_message_nb_common; // длина поступившего сообщения, общий случай.
    unsigned short current_message_address_common; // адрес в поступившем сообщениии, общий случай.
    unsigned char message_sourse; // с какого интерфейса поступило сообщение(ModbusTCP, ModbusRTU, ModbusRTU-HMI).
    unsigned char function_code; // код функции Modbus, в поступившем сообщении.
    unsigned char *buffer_pointer; // указатель на буфер для ответного сообщения Modbus.
    bool message_ready; // поступило сообщение, буфер занят.

    bool bits_buffer_busy;
    bool input_bits_buffer_busy;
    bool input_registers_buffer_busy;
    bool registers_buffer_busy;

    bool bits_buffer_data_ready;
    bool input_bits_data_ready;
    bool input_registers_data_ready;
    bool registers_data_ready;
};
//typedef struct _modbus_mapping_t _modbus_mapping_t;
//typedef struct _modbus_mapping_t modbus_mapping_t;

typedef enum
{
    MODBUS_ERROR_RECOVERY_NONE          = 0,
    MODBUS_ERROR_RECOVERY_LINK          = (1<<1),
    MODBUS_ERROR_RECOVERY_PROTOCOL      = (1<<2),
} modbus_error_recovery_mode;


extern uint8_t nui8FrameCounter; // modbus mind.

int modbus_set_slave(modbus_t* ctx, int slave);
int modbus_set_error_recovery(modbus_t *ctx, modbus_error_recovery_mode error_recovery);
void modbus_set_socket(modbus_t *ctx, int socket);
int modbus_get_socket(modbus_t *ctx);

void modbus_get_response_timeout(modbus_t *ctx, struct timeval *timeout);
void modbus_set_response_timeout(modbus_t *ctx, const struct timeval *timeout);

void modbus_get_byte_timeout(modbus_t *ctx, struct timeval *timeout);
void modbus_set_byte_timeout(modbus_t *ctx, const struct timeval *timeout);

int modbus_get_header_length(modbus_t *ctx);

int modbus_connect(modbus_t *ctx);
void modbus_close(modbus_t *ctx);

void modbus_free(modbus_t *ctx);

int modbus_flush(modbus_t *ctx);
void modbus_set_debug(modbus_t *ctx, int boolean);

const char *modbus_strerror(int errnum);

int modbus_read_bits(modbus_t *ctx, int addr, int nb, unsigned char *dest);
int modbus_read_input_bits(modbus_t *ctx, int addr, int nb, unsigned char *dest);
int modbus_read_registers(modbus_t *ctx, int addr, int nb, unsigned short *dest);
int modbus_read_input_registers(modbus_t *ctx, int addr, int nb, unsigned short *dest);
int modbus_write_bit(modbus_t *ctx, int coil_addr, int status);
int modbus_write_register(modbus_t *ctx, int reg_addr, int value);
int modbus_write_bits(modbus_t *ctx, int addr, int nb, const unsigned char *data);
int modbus_write_registers(modbus_t *ctx, int addr, int nb, const unsigned short *data);
int modbus_write_and_read_registers(modbus_t *ctx, int write_addr, int write_nb,
                                    const unsigned short *src, int read_addr, int read_nb,
                                    unsigned short *dest);
int modbus_report_slave_id(modbus_t *ctx, unsigned char *dest);

int16_t modbus_data_exchange(modbus_t *ctx, int16_t nb, uint8_t *src, uint8_t *dest);

modbus_mapping_t* modbus_mapping_new(int nb_coil_status, int nb_input_status,
                                     int nb_holding_registers, int nb_input_registers);
void modbus_mapping_free(modbus_mapping_t *mb_mapping);

int modbus_send_raw_request(modbus_t *ctx, unsigned char *raw_req, int raw_req_length);

int modbus_receive(modbus_t *ctx, unsigned char *req);
int modbus_receive_from(modbus_t *ctx, int sockfd, unsigned char *req);

int modbus_receive_confirmation(modbus_t *ctx, unsigned char *rsp);

int modbus_reply(modbus_t *ctx, const unsigned char *req,
                 int req_length, modbus_mapping_t *mb_mapping);
int modbus_reply_exception(modbus_t *ctx, const unsigned char *req,
                           unsigned int exception_code);

/**
 * UTILS FUNCTIONS
 **/

#define MODBUS_GET_HIGH_BYTE(data) (((data) >> 8) & 0xFF)
#define MODBUS_GET_LOW_BYTE(data) ((data) & 0xFF)
#define MODBUS_GET_INT32_FROM_INT16(tab_int16, index) ((tab_int16[(index)] << 16) + tab_int16[(index) + 1])
#define MODBUS_GET_INT16_FROM_INT8(tab_int8, index) ((tab_int8[(index)] << 8) + tab_int8[(index) + 1])
#define MODBUS_SET_INT16_TO_INT8(tab_int8, index, value) \
    do { \
        tab_int8[(index)] = (value) >> 8;  \
        tab_int8[(index) + 1] = (value) & 0xFF; \
    } while (0)

void modbus_set_bits_from_byte(unsigned char *dest, int index, const unsigned char value);
void modbus_set_bits_from_bytes(unsigned char *dest, int index, unsigned int nb_bits,
                                const unsigned char *tab_byte);
unsigned char modbus_get_byte_from_bits(const unsigned char *src, int index, unsigned int nb_bits);
float modbus_get_float(const unsigned short *src);
void modbus_set_float(float f, unsigned short *dest);

void _modbus_init_common(modbus_t *ctx);
void _error_print(modbus_t *ctx, const char *context);

#ifndef HAVE_STRLCPY
size_t strlcpy(char *dest, const char *src, size_t dest_size);
#endif

//-----------------------------------------------------------------------------------------------------

#include "modbus-tcp.h"
#include "modbus-rtu.h"

//MODBUS_END_DECLS

//#ifdef __cplusplus
//}
//#endif


#endif  /* _MODBUS_H_ */

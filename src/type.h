#ifndef _TYPE_H
#define _TYPE_H

//#define NULL (0)

// true/false defines
#define FALSE	0
#define TRUE	1

// datatype definitions macros
typedef unsigned char  u08;
typedef   signed char  s08;
typedef unsigned short u16;
typedef   signed short s16;
typedef unsigned long  u32;
typedef   signed long  s32;
typedef unsigned long long u64;
typedef   signed long long s64;

/* use inttypes.h instead
// C99 standard integer type definitions
typedef unsigned char	unsigned char;
typedef   signed char	int8_t;
typedef unsigned short	unsigned short;
typedef   signed short	int16_t;
typedef unsigned long	uint32_t;
typedef   signed long	int32_t;
typedef unsigned long	uint64_t;
typedef   signed long	int64_t;
*/

// maximum value that can be held
// by unsigned data types (8,16,32bits)
#define MAX_U08	255
#define MAX_U16	65535
#define MAX_U32	4294967295

// maximum values that can be held
// by signed data types (8,16,32bits)
#define MIN_S08	-128
#define MAX_S08	127
#define MIN_S16	-32768
#define MAX_S16	32767
#define MIN_S32	-2147483648
#define MAX_S32	2147483647

typedef unsigned char   BOOL;

typedef signed char CHAR;
typedef unsigned char UCHAR;

typedef signed int INT;
typedef unsigned int UINT;

typedef unsigned short USHORT;
typedef short   SHORT;

typedef signed long LONG;
typedef unsigned long ULONG;

typedef signed char SBYTE;
//typedef unsigned char BYTE;

typedef signed int SWORD;
typedef unsigned int WORD;

typedef signed long SDWORD;
typedef unsigned long DWORD;


typedef signed char sbyte;
typedef unsigned char byte;
typedef unsigned char uchar;

typedef unsigned int word;
typedef unsigned int uint;

typedef unsigned long ulong;
typedef unsigned long dword;

typedef unsigned char uch;



#endif


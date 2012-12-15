#ifndef __DATATYPES_H
#define __DATATYPES_H

#include "GenericTypeDefs.h"

/* Specify an extension for GCC based compilers */
#if defined(__GNUC__)
#define __EXTENSION __extension__
#else
#define __EXTENSION
#endif

/* Constants */
#define kHz                     1000ul
#define MHz                     1000000ul

typedef float               FLOAT;  // 32-Bit
typedef double             DOUBLE;  // 32-Bit

typedef union
{
    UINT8 Val;
    struct __PACKED
    {
        __EXTENSION UINT8 B0:1;
        __EXTENSION UINT8 B1:1;
        __EXTENSION UINT8 B2:1;
        __EXTENSION UINT8 B3:1;
        __EXTENSION UINT8 B4:1;
        __EXTENSION UINT8 B5:1;
        __EXTENSION UINT8 B6:1;
        __EXTENSION UINT8 B7:1;
    } Bits;
} UINT8_BIT, BYTE_BIT;

typedef union
{
    INT8 Val;
    struct __PACKED
    {
        __EXTENSION UINT8 B0:1;
        __EXTENSION UINT8 B1:1;
        __EXTENSION UINT8 B2:1;
        __EXTENSION UINT8 B3:1;
        __EXTENSION UINT8 B4:1;
        __EXTENSION UINT8 B5:1;
        __EXTENSION UINT8 B6:1;
        __EXTENSION UINT8 B7:1;
    } Bits;
} INT8_BIT;

typedef union
{
    UINT16 Val;
    struct __PACKED
    {
        UINT8 B0;
        UINT8 B1;
    } Byte;
    struct __PACKED
    {
        __EXTENSION UINT8 B0:1;
        __EXTENSION UINT8 B1:1;
        __EXTENSION UINT8 B2:1;
        __EXTENSION UINT8 B3:1;
        __EXTENSION UINT8 B4:1;
        __EXTENSION UINT8 B5:1;
        __EXTENSION UINT8 B6:1;
        __EXTENSION UINT8 B7:1;
        __EXTENSION UINT8 B8:1;
        __EXTENSION UINT8 B9:1;
        __EXTENSION UINT8 B10:1;
        __EXTENSION UINT8 B11:1;
        __EXTENSION UINT8 B12:1;
        __EXTENSION UINT8 B13:1;
        __EXTENSION UINT8 B14:1;
        __EXTENSION UINT8 B15:1;
    } Bits;
} UINT16_BIT;

typedef union
{
    INT16 Val;
    struct __PACKED
    {
        UINT8 B0;
        UINT8 B1;
    } Byte;
    struct __PACKED
    {
        __EXTENSION UINT8 B0:1;
        __EXTENSION UINT8 B1:1;
        __EXTENSION UINT8 B2:1;
        __EXTENSION UINT8 B3:1;
        __EXTENSION UINT8 B4:1;
        __EXTENSION UINT8 B5:1;
        __EXTENSION UINT8 B6:1;
        __EXTENSION UINT8 B7:1;
        __EXTENSION UINT8 B8:1;
        __EXTENSION UINT8 B9:1;
        __EXTENSION UINT8 B10:1;
        __EXTENSION UINT8 B11:1;
        __EXTENSION UINT8 B12:1;
        __EXTENSION UINT8 B13:1;
        __EXTENSION UINT8 B14:1;
        __EXTENSION UINT8 B15:1;
    } Bits;
} INT16_BIT;

typedef union
{
    UINT32 Val;
    struct __PACKED
    {
        UINT16 W0;
        UINT16 W1;
    } Word;
    struct __PACKED
    {
        UINT16_BIT W0;
        UINT16_BIT W1;
    } WordUnion;
    struct __PACKED
    {
        UINT8 B0;
        UINT8 B1;
        UINT8 B2;
        UINT8 B3;
    } Byte;
    struct __PACKED
    {
        __EXTENSION UINT8 B0:1;
        __EXTENSION UINT8 B1:1;
        __EXTENSION UINT8 B2:1;
        __EXTENSION UINT8 B3:1;
        __EXTENSION UINT8 B4:1;
        __EXTENSION UINT8 B5:1;
        __EXTENSION UINT8 B6:1;
        __EXTENSION UINT8 B7:1;
        __EXTENSION UINT8 B8:1;
        __EXTENSION UINT8 B9:1;
        __EXTENSION UINT8 B10:1;
        __EXTENSION UINT8 B11:1;
        __EXTENSION UINT8 B12:1;
        __EXTENSION UINT8 B13:1;
        __EXTENSION UINT8 B14:1;
        __EXTENSION UINT8 B15:1;
        __EXTENSION UINT8 B16:1;
        __EXTENSION UINT8 B17:1;
        __EXTENSION UINT8 B18:1;
        __EXTENSION UINT8 B19:1;
        __EXTENSION UINT8 B20:1;
        __EXTENSION UINT8 B21:1;
        __EXTENSION UINT8 B22:1;
        __EXTENSION UINT8 B23:1;
        __EXTENSION UINT8 B24:1;
        __EXTENSION UINT8 B25:1;
        __EXTENSION UINT8 B26:1;
        __EXTENSION UINT8 B27:1;
        __EXTENSION UINT8 B28:1;
        __EXTENSION UINT8 B29:1;
        __EXTENSION UINT8 B30:1;
        __EXTENSION UINT8 B31:1;
    } Bits;
} UINT32_BIT;

typedef union
{
    INT32 Val;
    struct __PACKED
    {
        UINT16 W0;
        UINT16 W1;
    } Word;
    struct __PACKED
    {
        UINT16_BIT W0;
        UINT16_BIT W1;
    } WordUnion;
    struct __PACKED
    {
        UINT8 B0;
        UINT8 B1;
        UINT8 B2;
        UINT8 B3;
    } Byte;
    struct __PACKED
    {
        __EXTENSION UINT8 B0:1;
        __EXTENSION UINT8 B1:1;
        __EXTENSION UINT8 B2:1;
        __EXTENSION UINT8 B3:1;
        __EXTENSION UINT8 B4:1;
        __EXTENSION UINT8 B5:1;
        __EXTENSION UINT8 B6:1;
        __EXTENSION UINT8 B7:1;
        __EXTENSION UINT8 B8:1;
        __EXTENSION UINT8 B9:1;
        __EXTENSION UINT8 B10:1;
        __EXTENSION UINT8 B11:1;
        __EXTENSION UINT8 B12:1;
        __EXTENSION UINT8 B13:1;
        __EXTENSION UINT8 B14:1;
        __EXTENSION UINT8 B15:1;
        __EXTENSION UINT8 B16:1;
        __EXTENSION UINT8 B17:1;
        __EXTENSION UINT8 B18:1;
        __EXTENSION UINT8 B19:1;
        __EXTENSION UINT8 B20:1;
        __EXTENSION UINT8 B21:1;
        __EXTENSION UINT8 B22:1;
        __EXTENSION UINT8 B23:1;
        __EXTENSION UINT8 B24:1;
        __EXTENSION UINT8 B25:1;
        __EXTENSION UINT8 B26:1;
        __EXTENSION UINT8 B27:1;
        __EXTENSION UINT8 B28:1;
        __EXTENSION UINT8 B29:1;
        __EXTENSION UINT8 B30:1;
        __EXTENSION UINT8 B31:1;
    } Bits;
} INT32_BIT;

typedef union
{
    float  Val;
    UINT32 Data;
    struct __PACKED
    {
        UINT8 B0;
        UINT8 B1;
        UINT8 B2;
        UINT8 B3;
    } Byte;
} FLOAT_BIT;

#endif

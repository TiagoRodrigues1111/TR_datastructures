/**
 * @file tr_types.h
 * @brief Portable type definitions for C89/C99/C11 compatibility
 *
 * Provides fixed width integer types, bool, and NULL for use across
 * all C standards. On C99 and later the standard headers are used
 * directly. On C89 portable fallbacks are provided with compile time
 * size verification.
 */

/*******************************************************************************************************
 * NAME: tr_types.h
 *
 * PURPOSE: Portable type definitions for C89/C99/C11 compatibility
 *
 * GLOBAL VARIABLES: None
 *
 * DEVELOPMENT HISTORY:
 *
 * Date          Author          Change Id       Release         Description Of Change
 * ----------    --------------- ---------       -------         -----------------------------------
 * 24-05-2026    Tiago Rodrigues                       1         File preparation
 *
 *******************************************************************************************************/
#ifndef TR_TYPES_H
#define TR_TYPES_H

/* 0 copyright/licensing */
/*******************************************************************************************************
 *
 * This is free and unencumbered software released into the public domain (Unlicense).
 *
 ********************************************************************************************************/

#ifdef __cplusplus
extern "C"
{
#endif

/* 1 includes */
/*****************************************************/
#include <limits.h> /* UCHAR_MAX, USHRT_MAX, UINT_MAX, ULONG_MAX - available in C89 */
#include <stddef.h> /* size_t - available in C89 */
/*****************************************************/

/* 2 defines */
/*****************************************************/

/* ------------------------------------------------------------------
 * 2.1 Compile-time assert
 * ------------------------------------------------------------------ */

/**
 * @brief Compile time assertion macro
 *
 * Triggers a compile error if @p condition is false.
 * On C11 and later uses @c _Static_assert. On C89/C99 uses
 * the array size trick.
 *
 * @param condition Expression to evaluate at compile time
 * @param message   Token used as the error identifier — no spaces allowed
 *
 * Example usage:
 * @code
 * TR_STATIC_ASSERT(sizeof(int) == 4, int_must_be_4_bytes);
 * @endcode
 */
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
/* C11 - native static assert */
#define TR_STATIC_ASSERT(condition, message) _Static_assert((condition), #message)
#else
/* C89/C99 */
#define TR_STATIC_ASSERT(condition, message)                                                       \
        typedef char tr_static_assert_##message[(condition) ? 1 : -1]
#endif

/* ------------------------------------------------------------------
 * 2.2 bool
 * ------------------------------------------------------------------ */

/**
 * @brief Boolean type
 *
 * On C++ uses the native bool type.
 * On C99 and later includes @c stdbool.h.
 * On C89 provides a portable enum fallback with values
 * @c false = 0 and @c true = 1, matching C99 exactly.
 */
#if defined(__cplusplus)
/* C++ has native bool */
#elif defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
/* C99 or later - use standard stdbool.h */
#include <stdbool.h>
#else
/* C89 fallback - matches C99 stdbool.h values exactly */
typedef enum
{
        false = 0, /**< Boolean false value - evaluates to 0 */
        true = 1   /**< Boolean true value  - evaluates to 1 */
} bool;
#endif

/* ------------------------------------------------------------------
 * 2.3 Fixed width integers
 * ------------------------------------------------------------------ */

/**
 * @defgroup tr_types_integers Fixed Width Integer Types
 * @{
 *
 * @brief Portable fixed width integer types
 *
 * On C99 and later these are provided by @c stdint.h directly.
 * On C89 portable fallbacks are detected at compile time using
 * @c limits.h constants. A compile time assertion verifies the
 * correct size on each platform.
 *
 * | Type      | Size   | Range                        |
 * |-----------|--------|------------------------------|
 * | uint8_t   | 1 byte | 0 to 255                     |
 * | int8_t    | 1 byte | -128 to 127                  |
 * | uint16_t  | 2 byte | 0 to 65535                   |
 * | int16_t   | 2 byte | -32768 to 32767              |
 * | uint32_t  | 4 byte | 0 to 4294967295              |
 * | int32_t   | 4 byte | -2147483648 to 2147483647    |
 * | uint64_t  | 8 byte | 0 to 18446744073709551615    |
 * | int64_t   | 8 byte | -9223372036854775808 to max  |
 */
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
/* C99 or later - use standard stdint.h */
#include <stdint.h>
#else
/* C89 fallback - auto detect correct types using limits.h constants */

/* uint8_t / int8_t - find a type that is exactly 1 byte */
#if defined(UCHAR_MAX) && (UCHAR_MAX == 0xFF)
typedef unsigned char uint8_t;
typedef signed char int8_t;
#else
#error "Cannot find a suitable 8-bit type on this platform"
#endif

/* uint16_t / int16_t - find a type that is exactly 2 bytes */
#if defined(USHRT_MAX) && (USHRT_MAX == 0xFFFF)
typedef unsigned short uint16_t;
typedef signed short int16_t;
#elif defined(UINT_MAX) && (UINT_MAX == 0xFFFF)
typedef unsigned int uint16_t;
typedef signed int int16_t;
#else
#error "Cannot find a suitable 16-bit type on this platform"
#endif

/* uint32_t / int32_t - find a type that is exactly 4 bytes */
#if defined(UINT_MAX) && (UINT_MAX == 0xFFFFFFFF)
typedef unsigned int uint32_t;
typedef signed int int32_t;
#elif defined(ULONG_MAX) && (ULONG_MAX == 0xFFFFFFFF)
typedef unsigned long uint32_t;
typedef signed long int32_t;
#else
#error "Cannot find a suitable 32-bit type on this platform"
#endif

/* uint64_t / int64_t - find a type that is exactly 8 bytes */
#if defined(_MSC_VER)
/* Microsoft Visual C++ compiler - use __int64 extension */
typedef unsigned __int64 uint64_t;
typedef signed __int64 int64_t;
#elif defined(__GNUC__) || defined(__clang__)
/* GCC or Clang - long long is supported as an extension in C89 */
typedef unsigned long long uint64_t;
typedef signed long long int64_t;
#elif defined(ULONG_MAX) && (ULONG_MAX == 0xFFFFFFFFFFFFFFFF)
/* generic fallback - unsigned long happens to be 64 bit */
typedef unsigned long uint64_t;
typedef signed long int64_t;
#else
#error "Cannot find a suitable 64-bit type on this platform"
#endif

#endif /* __STDC_VERSION__ >= 199901L */

/* ------------------------------------------------------------------
 * 2.4 NULL
 * ------------------------------------------------------------------ */

/**
 * @brief Null pointer constant
 *
 * Defined as @c 0 for C++ (where void* does not implicitly convert)
 * and as @c ((void*)0) for C. Only defined if not already provided
 * by an included system header.
 */
#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void *) 0)
#endif
#endif

// clang-format off

/*****************************************************/

/* 3 external declarations */
/*****************************************************/
/*****************************************************/

/* 4 typedefs */
/*****************************************************/
/*****************************************************/

/* 5 global variable declarations */
/*****************************************************/
/*****************************************************/

/* 6 function prototypes */
/*****************************************************/
/*****************************************************/

// clang-format on

#ifdef __cplusplus
}
#endif

#endif /* TR_TYPES_H */
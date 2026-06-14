/*******************************************************************************************************
 * NAME: tr_internal.h
 *
 * PURPOSE: Internal utility macros for the tr_datastructures library
 *          This file is NOT part of the public API - do not include it in public headers
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
#ifndef TR_INTERNAL_H
#define TR_INTERNAL_H

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
#include <stddef.h> /* size_t */
#include <stdio.h>  /* fprintf, stderr */
#include <stdlib.h> /* abort */

#include "tr_datastructures/tr_types.h"
#include "tr_datastructures/tr_export.h"
/*****************************************************/

/* 2 defines */
/*****************************************************/

/* ------------------------------------------------------------------
 * 2.1 Debug mode detection
 * Debug assertions are active when EITHER:
 *   - CMake builds in Debug mode (defines TR_CMAKE_DEBUG)
 *   - The user explicitly defines TR_DEBUG
 * ------------------------------------------------------------------ */
#if defined(TR_CMAKE_DEBUG) || defined(TR_DEBUG)
#define TR_IS_DEBUG 1
#else
#define TR_IS_DEBUG 0
#endif

/* ------------------------------------------------------------------
 * 2.2 Internal assertion macro
 * Only active in debug builds - compiles to nothing in release
 * Prints file, line, function and condition before aborting
 * ------------------------------------------------------------------ */
#if TR_IS_DEBUG

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
/* C99 or later - __func__ is available */
#define TR_ASSERT(condition)                                                                       \
        do                                                                                         \
        {                                                                                          \
                if (!(condition))                                                                  \
                {                                                                                  \
                        fprintf(stderr,                                                            \
                                "[TR_ASSERT] Condition failed: %s\n"                               \
                                "            Function : %s\n"                                      \
                                "            File     : %s\n"                                      \
                                "            Line     : %d\n",                                     \
                                #condition,                                                        \
                                __func__,                                                          \
                                __FILE__,                                                          \
                                __LINE__);                                                         \
                        abort();                                                                   \
                }                                                                                  \
        } while (0)

#else
/* C89 - __func__ is not available */
#define TR_ASSERT(condition)                                                                       \
        do                                                                                         \
        {                                                                                          \
                if (!(condition))                                                                  \
                {                                                                                  \
                        fprintf(stderr,                                                            \
                                "[TR_ASSERT] Condition failed: %s\n"                               \
                                "            File     : %s\n"                                      \
                                "            Line     : %d\n",                                     \
                                #condition,                                                        \
                                __FILE__,                                                          \
                                __LINE__);                                                         \
                        abort();                                                                   \
                }                                                                                  \
        } while (0)
#endif

#else
/* release build - assert compiles to nothing */
#define TR_ASSERT(condition) ((void) 0)
#endif

/* ------------------------------------------------------------------
 * 2.3 Unused variable/parameter macro
 * Suppresses compiler warnings for intentionally unused variables
 * Usage: TR_UNUSED(variable);
 * ------------------------------------------------------------------ */
#define TR_UNUSED(x) ((void) (x))

/* ------------------------------------------------------------------
 * 2.4 Array size macro
 * Returns the number of elements in a stack allocated array
 * NOTE: Do NOT use on pointers - will give wrong results
 * Usage: size_t n = TR_ARRAY_SIZE(my_array);
 * ------------------------------------------------------------------ */
#define TR_ARRAY_SIZE(array) (sizeof(array) / sizeof((array)[0]))

/* ------------------------------------------------------------------
 * 2.5 Min / Max macros
 * NOTE: arguments are evaluated twice - avoid side effects
 * e.g. TR_MAX(i++, j++) is undefined behaviour
 * ------------------------------------------------------------------ */
#define TR_MIN(a, b) ((a) < (b) ? (a) : (b))
#define TR_MAX(a, b) ((a) > (b) ? (a) : (b))

/* ------------------------------------------------------------------
 * 2.6 Compile time assertion
 * Reuses TR_STATIC_ASSERT from tr_types.h for internal use
 * ------------------------------------------------------------------ */
#define TR_INTERNAL_STATIC_ASSERT(condition, message) TR_STATIC_ASSERT(condition, message)

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

#endif /* TR_INTERNAL_H */
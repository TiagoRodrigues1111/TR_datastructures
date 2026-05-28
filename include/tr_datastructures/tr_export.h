/*******************************************************************************************************
 * NAME: tr_export.h
 *
 * PURPOSE: Defines macros for controlling symbol visibility across platforms and build types
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
#ifndef TR_EXPORT_H
#define TR_EXPORT_H

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
/*****************************************************/

/* 2 defines */
/*****************************************************/

/* ------------------------------------------------------------------
 * 2.1 Compiler visibility support detection
 * ------------------------------------------------------------------ */
#if defined(_MSC_VER)
/* Microsoft Visual C++ compiler */
#define TR_EXPORT_SYMBOL __declspec(dllexport)
#define TR_IMPORT_SYMBOL __declspec(dllimport)
#define TR_LOCAL_SYMBOL /* not supported on MSVC */

#elif defined(__GNUC__) && (__GNUC__ >= 4)
/* GCC version 4 or later - visibility attributes supported */
#define TR_EXPORT_SYMBOL __attribute__((visibility("default")))
#define TR_IMPORT_SYMBOL __attribute__((visibility("default")))
#define TR_LOCAL_SYMBOL __attribute__((visibility("hidden")))

#elif defined(__clang__)
/* Clang - visibility attributes supported */
#define TR_EXPORT_SYMBOL __attribute__((visibility("default")))
#define TR_IMPORT_SYMBOL __attribute__((visibility("default")))
#define TR_LOCAL_SYMBOL __attribute__((visibility("hidden")))

#else
/* unknown compiler / GCC less than 4 - no visibility support, degrade gracefully */
#define TR_EXPORT_SYMBOL
#define TR_IMPORT_SYMBOL
#define TR_LOCAL_SYMBOL
#endif

/* ------------------------------------------------------------------
 * 2.2 Public API macro
 * TR_DATASTRUCTURES_SHARED  - defined by CMake when building shared library
 * TR_DATASTRUCTURES_EXPORTS - defined by CMake when building the library itself
 *                             (not defined when a user is consuming the library)
 * ------------------------------------------------------------------ */
#if defined(TR_DATASTRUCTURES_SHARED)
/* building or consuming as a shared library */
#if defined(TR_DATASTRUCTURES_EXPORTS)
/* we are building the library - export symbols */
#define TR_API TR_EXPORT_SYMBOL
#else
/* we are consuming the library - import symbols */
#define TR_API TR_IMPORT_SYMBOL
#endif
#else
/* building or consuming as a static library - no decoration needed */
#define TR_API
#endif

/* ------------------------------------------------------------------
 * 2.3 Internal symbol macro
 * Marks symbols that are internal to the library and should not
 * be visible to consumers even when building shared
 * ------------------------------------------------------------------ */
#define TR_INTERNAL TR_LOCAL_SYMBOL

/* ------------------------------------------------------------------
 * 2.4 Deprecated symbol macro
 * Marks public API functions as deprecated with a message
 * ------------------------------------------------------------------ */
#if defined(_MSC_VER)
/* Microsoft Visual C++ compiler */
#define TR_DEPRECATED(message) __declspec(deprecated(message))

#elif defined(__GNUC__) && (__GNUC__ >= 4) || defined(__clang__)
/* GCC version 4 or later, or Clang */
#define TR_DEPRECATED(message) __attribute__((deprecated(message)))

#else
/* unknown compiler - degrade gracefully */
#define TR_DEPRECATED(message)
#endif


/* ------------------------------------------------------------------
 * 2.5 No discard macro
 * Warns if the return value of a function is ignored
 * Apply to all functions returning tr_result_t
 * ------------------------------------------------------------------ */
#if defined(__GNUC__) && (__GNUC__ >= 4) || defined(__clang__)
/* GCC version 4 or later, or Clang */
#define TR_NODISCARD __attribute__((warn_unused_result))
#elif defined(_MSC_VER) && (_MSC_VER >= 1700)
/* Microsoft Visual C++ compiler version 2012 or later */
#define TR_NODISCARD _Check_return_
#else
/* unknown compiler - degrade gracefully */
#define TR_NODISCARD
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

#endif /* TR_EXPORT_H */
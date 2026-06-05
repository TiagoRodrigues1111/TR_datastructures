/**
 * @file tr_export.h
 * @brief Symbol visibility and compiler annotation macros
 *
 * Provides macros for controlling symbol visibility across platforms
 * and build types. Supports MSVC, GCC, Clang and unknown compilers
 * with graceful degradation.
 *
 * The following macros are defined:
 * - @ref TR_API       — marks a public API symbol
 * - @ref TR_INTERNAL  — marks an internal symbol
 * - @ref TR_DEPRECATED — marks a deprecated symbol
 * - @ref TR_NODISCARD  — warns if return value is ignored
 */
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
/**
 * @defgroup tr_export_visibility Compiler Detection Macros
 * @{
 *
 * @brief Internal macros for raw compiler visibility attributes
 *
 * These are internal building blocks for @ref TR_API and @ref TR_INTERNAL.
 * Do not use these directly in consuming code — use @ref TR_API instead.
 *
 * | Macro              | MSVC                    | GCC/Clang                          | Unknown |
 * |--------------------|-------------------------|------------------------------------|---------|
 * | TR_EXPORT_SYMBOL   | __declspec(dllexport)   | __attribute__((visibility("default"))) | (empty)
 * | | TR_IMPORT_SYMBOL   | __declspec(dllimport)   | __attribute__((visibility("default"))) |
 * (empty) | | TR_LOCAL_SYMBOL    | (not supported)         | __attribute__((visibility("hidden")))
 * | (empty) |
 */
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

/** @} */ /* end of tr_export_visibility group */

/* ------------------------------------------------------------------
 * 2.2 Public API macro
 * TR_DATASTRUCTURES_SHARED  - defined by CMake when building shared library
 * TR_DATASTRUCTURES_EXPORTS - defined by CMake when building the library itself
 *                             (not defined when a user is consuming the library)
 * ------------------------------------------------------------------ */
/**
 * @brief Marks a public API symbol for export or import
 *
 * When building the library as a shared library:
 * - Expands to @c __declspec(dllexport) when building the library (MSVC)
 * - Expands to @c __declspec(dllimport) when consuming the library (MSVC)
 * - Expands to @c __attribute__((visibility("default"))) on GCC/Clang
 *
 * When building as a static library expands to nothing.
 *
 * Apply to all public API function declarations:
 * @code
 * TR_NODISCARD TR_API tr_result_t tr_stack_create(...);
 * @endcode
 */
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
/**
 * @brief Marks a symbol as internal to the library
 *
 * Prevents the symbol from being visible to consumers of the library
 * even when building as a shared library.
 *
 * - Expands to @c __attribute__((visibility("hidden"))) on GCC/Clang
 * - Not supported on MSVC, expands to nothing
 *
 * Apply to internal functions that must be visible across translation
 * units within the library but not to consumers:
 * @code
 * TR_INTERNAL void tr_internal_helper(void);
 * @endcode
 */
#define TR_INTERNAL TR_LOCAL_SYMBOL

/* ------------------------------------------------------------------
 * 2.4 Deprecated symbol macro
 * Marks public API functions as deprecated with a message
 * ------------------------------------------------------------------ */
/**
 * @brief Marks a public API function as deprecated
 *
 * Emits a compiler warning when the marked function is used,
 * with a message explaining the deprecation.
 *
 * - Expands to @c __declspec(deprecated(message)) on MSVC
 * - Expands to @c __attribute__((deprecated(message))) on GCC/Clang
 * - Expands to nothing on unknown compilers
 *
 * Example usage:
 * @code
 * TR_DEPRECATED("Use tr_stack_create instead")
 * TR_API tr_result_t tr_stack_new(...);
 * @endcode
 *
 * @param message String literal describing the deprecation reason
 */
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
/**
 * @brief Warns if the return value of a function is ignored
 *
 * Apply to all functions returning @ref tr_result_t to enforce
 * error checking at the call site.
 *
 * - Expands to @c __attribute__((warn_unused_result)) on GCC/Clang
 * - Expands to @c _Check_return_ on MSVC 2012 and later
 * - Expands to nothing on unknown compilers
 *
 * Example usage:
 * @code
 * TR_NODISCARD TR_API tr_result_t tr_stack_push(...);
 * @endcode
 */
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
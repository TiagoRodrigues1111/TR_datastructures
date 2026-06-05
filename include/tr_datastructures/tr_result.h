/**
 * @file tr_result.h
 * @brief Library wide error code definitions for the tr_datastructures library
 */

/*******************************************************************************************************
 * NAME: tr_result.h
 *
 * PURPOSE: Defines the tr_result_t error code enum for the tr_datastructures library
 *
 * GLOBAL VARIABLES: None
 *
 * DEVELOPMENT HISTORY:
 *
 * Date          Author          Change Id       Release         Description Of Change
 * ----------    --------------- ---------       -------         -----------------------------------
 * 28-05-2026    Tiago Rodrigues                       1         File preparation
 *
 *******************************************************************************************************/
#ifndef TR_RESULT_H
#define TR_RESULT_H

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
/*****************************************************/

/* 3 external declarations */
/*****************************************************/
/*****************************************************/

/* 4 typedefs */
/*****************************************************/

/**
 * @brief Library wide error code returned by all API functions
 *
 * Every public API function returns a value of this type.
 * @c TR_OK indicates success. Any other value indicates a specific
 * failure condition that the caller should handle.
 *
 * Example usage:
 * @code
 * struct stack  *p_stack = NULL;
 * tr_result_t    res     = TR_OK;
 *
 * res = tr_stack_create(sizeof(int), 10, TR_STACK_ARRAY_DYNAMIC, &p_stack);
 * if (TR_OK != res)
 * {
 *     // handle error
 * }
 * @endcode
 */
typedef enum tr_result
{
        TR_OK = 0,              /**< Operation completed successfully                   */
        TR_ERR_NULL = 1,        /**< NULL pointer passed where a valid pointer expected */
        TR_ERR_ALLOC = 2,       /**< Memory allocation failed                           */
        TR_ERR_EMPTY = 3,       /**< Operation failed because structure is empty        */
        TR_ERR_FULL = 4,        /**< Operation failed because structure is full         */
        TR_ERR_INVALID = 5,     /**< Invalid argument (e.g. size of 0)                  */
        TR_ERR_NOT_FOUND = 6,   /**< Element not found in the structure                 */
        TR_ERR_DUPLICATE = 7,   /**< Duplicate element detected                         */
        TR_ERR_OUT_OF_RANGE = 8 /**< Index or position is out of valid range            */
} tr_result_t;

/*****************************************************/

/* 5 global variable declarations */
/*****************************************************/
/*****************************************************/

/* 6 function prototypes */
/*****************************************************/
/*****************************************************/

#ifdef __cplusplus
}
#endif

#endif /* TR_RESULT_H */
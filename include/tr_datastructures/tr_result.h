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

/*******************************************************************************************************
 *
 * TYPE NAME: tr_result_t
 *
 * PURPOSE: Library wide error code type returned by all API functions
 *
 * VALUES:
 *
 * VALUE                   DESCRIPTION
 * -----                   -----------
 * TR_OK                   Operation completed successfully
 * TR_ERR_NULL             NULL pointer passed where a valid pointer was expected
 * TR_ERR_ALLOC            Memory allocation failed
 * TR_ERR_EMPTY            Operation failed because the structure is empty
 * TR_ERR_FULL             Operation failed because the structure is full (might never happen, depending on implementation)
 * TR_ERR_INVALID          Invalid argument passed (e.g. size of 0)
 * TR_ERR_NOT_FOUND        Element not found in the structure
 * TR_ERR_DUPLICATE        Duplicate element detected
 * TR_ERR_OUT_OF_RANGE     Index or position is out of valid range
 *
 *******************************************************************************************************/
typedef enum tr_result
{
        TR_OK = 0,
        TR_ERR_NULL = 1,
        TR_ERR_ALLOC = 2,
        TR_ERR_EMPTY = 3,
        TR_ERR_FULL = 4,
        TR_ERR_INVALID = 5,
        TR_ERR_NOT_FOUND = 6,
        TR_ERR_DUPLICATE = 7,
        TR_ERR_OUT_OF_RANGE = 8
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
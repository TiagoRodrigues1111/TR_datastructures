/*******************************************************************************
 * NAME: tr_stack.h
 *
 * PURPOSE: Declaration of the stack functions
 *
 * GLOBAL VARIABLES:
 *
 * Variable              Type                    Description
 * --------              ----                    -----------
 *
 * DEVELOPMENT HISTORY:
 *
 * Date         Author          Change Id       Release         Description Of Change
 * 31-05-2026   Tiago Rodrigues                 1               File preparation
 *
 *******************************************************************************/
#ifndef TR_STACK_H
#define TR_STACK_H

/* 0 copyright/licensing */
/*******************************************************************************************************
 *
 * This is free and unencumbered software released into the public domain (Unlicense).
 *
 ********************************************************************************************************/

/* Ensure C++ compatibility */
#ifdef __cplusplus
extern "C"
{
#endif

/* 1 includes */
/*****************************************************/
#include "tr_datastructures/tr_export.h"
#include "tr_datastructures/tr_result.h"
#include "tr_datastructures/tr_types.h"
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
 * TYPE NAME: struct stack
 *
 * PURPOSE: Opaque handle to a stack instance
 *
 *******************************************************************************************************/
struct stack;

/*****************************************************/

/* 5 global variable declarations */
/*****************************************************/

/* 6 function prototypes */
/*****************************************************/

/******************************************************************
 *
 * FUNCTION NAME: stack_create
 *
 * PURPOSE: Allocates the needed memory for the stack wanted
 *
 * ARGUMENTS:
 *
 * ARGUMENT                TYPE            I/O     DESCRIPTION
 * --------                ----            ---     ------------
 * size_of_datatype        size_t          I       Byte size of the datatype to store in the
 * stack elements_to_allocate    size_t          I       Initial number of elements to
 * allocate id_of_stack             stack_t **      O       Pointer to pointer to receive
 * the created stack
 *
 * RETURNS: tr_result_t
 *   TR_OK               - Stack created successfully
 *   TR_ERR_INVALID      - size_of_datatype or elements_to_allocate is 0
 *   TR_ERR_ALLOC        - Memory allocation failed
 *
 *****************************************************************/
TR_NODISCARD TR_API tr_result_t stack_create(const size_t size_of_datatype,
                                             const size_t elements_to_allocate,
                                             struct stack **id_of_stack);

/******************************************************************
 *
 * FUNCTION NAME: stack_destroy
 *
 * PURPOSE: Frees all memory associated with the stack instance
 *
 * ARGUMENTS:
 *
 * ARGUMENT                TYPE            I/O     DESCRIPTION
 * --------                ----            ---     ------------
 * id_of_stack             stack_t **      I/O     Pointer to pointer to the stack to
 * destroy Set to NULL after destruction
 *
 * RETURNS: tr_result_t
 *   TR_OK               - Stack destroyed successfully
 *   TR_ERR_NULL         - id_of_stack or *id_of_stack is NULL
 *
 *****************************************************************/
TR_NODISCARD TR_API tr_result_t stack_destroy(struct stack **id_of_stack);

/******************************************************************
 *
 * FUNCTION NAME: stack_push
 *
 * PURPOSE: Pushes a element onto the top of the stack
 *
 * ARGUMENTS:
 *
 * ARGUMENT                TYPE            I/O     DESCRIPTION
 * --------                ----            ---     ------------
 * id_of_stack             struct stack *  I/O     Pointer to the stack to push onto
 * data_to_push            const void *    I       Pointer to the data to copy onto the
 * stack
 *
 * RETURNS: tr_result_t
 *   TR_OK               - Data pushed successfully
 *   TR_ERR_NULL         - id_of_stack or data_to_push is NULL
 *   TR_ERR_ALLOC        - Memory allocation failed (linked list only)
 *   TR_ERR_FULL         - Stack is full (array based only)
 *
 *****************************************************************/
TR_NODISCARD TR_API tr_result_t stack_push(struct stack *id_of_stack, const void *data_to_push);

/******************************************************************
 *
 * FUNCTION NAME: stack_pop
 *
 * PURPOSE: Removes the element at the top of the stack
 *
 * ARGUMENTS:
 *
 * ARGUMENT                TYPE            I/O     DESCRIPTION
 * --------                ----            ---     ------------
 * id_of_stack             struct stack *  I/O     Pointer to the stack to pop from
 *
 * RETURNS: tr_result_t
 *   TR_OK               - Element popped successfully
 *   TR_ERR_NULL         - id_of_stack is NULL
 *   TR_ERR_EMPTY        - Stack is empty
 *
 *****************************************************************/
TR_NODISCARD TR_API tr_result_t stack_pop(struct stack *id_of_stack);

/******************************************************************
 *
 * FUNCTION NAME: stack_top
 *
 * PURPOSE: Copies the element at the top of the stack into the provided buffer
 *          Does not remove the element
 *
 * ARGUMENTS:
 *
 * ARGUMENT                TYPE                  I/O     DESCRIPTION
 * --------                ----                  ---     ------------
 * id_of_stack             const struct stack *  I       Pointer to the stack to peek at
 * data_at_top             void *                O       Pointer to buffer to copy the top
 * element into Must be at least size_of_datatype bytes
 *
 * RETURNS: tr_result_t
 *   TR_OK               - Data copied successfully
 *   TR_ERR_NULL         - id_of_stack or data_at_top is NULL
 *   TR_ERR_EMPTY        - Stack is empty
 *
 *****************************************************************/
TR_NODISCARD TR_API tr_result_t stack_top(const struct stack *id_of_stack, void *data_at_top);

/******************************************************************
 *
 * FUNCTION NAME: stack_size
 *
 * PURPOSE: Returns the current number of elements in the stack
 *
 * ARGUMENTS:
 *
 * ARGUMENT                TYPE            I/O     DESCRIPTION
 * --------                ----            ---     ------------
 * id_of_stack             const struct stack * I       Pointer to the stack to query
 * size                    size_t *        O       Pointer to receive the current element
 * count
 *
 * RETURNS: tr_result_t
 *   TR_OK               - Size retrieved successfully
 *   TR_ERR_NULL         - id_of_stack or size is NULL
 *
 *****************************************************************/
TR_NODISCARD TR_API tr_result_t stack_size(const struct stack *id_of_stack, size_t *size);

/******************************************************************
 *
 * FUNCTION NAME: stack_is_empty
 *
 * PURPOSE: Checks whether the stack contains no elements
 *
 * ARGUMENTS:
 *
 * ARGUMENT                TYPE            I/O     DESCRIPTION
 * --------                ----            ---     ------------
 * id_of_stack             const struct stack * I       Pointer to the stack to check
 * is_empty                bool *          O       Pointer to receive the result
 *                                                 Set to true if empty, false otherwise
 *
 * RETURNS: tr_result_t
 *   TR_OK               - Check completed successfully
 *   TR_ERR_NULL         - id_of_stack or is_empty is NULL
 *
 *****************************************************************/
TR_NODISCARD TR_API tr_result_t stack_is_empty(const struct stack *id_of_stack, bool *is_empty);

/******************************************************************
 *
 * FUNCTION NAME: stack_capacity
 *
 * PURPOSE: Returns the total allocated capacity of the stack
 *          For linked list based stacks this is the same as stack_size
 *
 * ARGUMENTS:
 *
 * ARGUMENT                TYPE            I/O     DESCRIPTION
 * --------                ----            ---     ------------
 * id_of_stack             const struct stack * I       Pointer to the stack to query
 * capacity                size_t *        O       Pointer to receive the capacity
 *
 * RETURNS: tr_result_t
 *   TR_OK               - Capacity retrieved successfully
 *   TR_ERR_NULL         - id_of_stack or capacity is NULL
 *
 *****************************************************************/
TR_NODISCARD TR_API tr_result_t stack_capacity(const struct stack *id_of_stack, size_t *capacity);

/*****************************************************/

#ifdef __cplusplus
}
#endif

#endif /* TR_STACK_H */

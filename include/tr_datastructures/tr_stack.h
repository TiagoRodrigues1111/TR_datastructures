
/**
 * @file tr_stack.h
 * @brief Public API for the stack data structure
 *
 * Provides a generic stack implementation supporting multiple
 * underlying storage strategies selectable at runtime via
 * @ref tr_stack_type_t.
 *
 * Typical usage:
 * @code
 * struct stack *p_stack = NULL;
 * tr_result_t   res     = TR_OK;
 * int           val     = 42;
 * int           out     = 0;
 *
 * res = tr_stack_create(sizeof(int), 10, TR_STACK_ARRAY_DYNAMIC, &p_stack);
 * if (TR_OK != res) { return res; }
 *
 * res = tr_stack_push(p_stack, &val);
 * if (TR_OK != res) { return res; }
 *
 * res = tr_stack_top(p_stack, &out);
 * if (TR_OK != res) { return res; }
 *
 * res = tr_stack_pop(p_stack);
 * if (TR_OK != res) { return res; }
 *
 * tr_stack_destroy(&p_stack);
 * @endcode
 */
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
/**
 * @brief Opaque handle to a stack instance
 *
 * Users interact with the stack exclusively through the API functions.
 * The internal implementation is hidden.
 */
struct stack;

/*******************************************************************************************************
 *
 * TYPE NAME: tr_stack_type_t
 *
 * PURPOSE: Selects the underlying implementation used by the stack
 *
 * VALUES:
 *
 * VALUE                   DESCRIPTION
 * -----                   -----------
 * TR_STACK_ARRAY          Array based implementation - fixed capacity, fast access
 * TR_STACK_ARRAY_FIXED    Array based implementation - fixed capacity, returns FULL when capacity
 * is reached TR_STACK_LL             Linked list based implementation - dynamic growth, unbounded
 *
 *******************************************************************************************************/
/**
 * @brief Selects the underlying implementation used by the stack
 *
 * Passed to @ref tr_stack_create to select which implementation
 * to use. The choice affects memory layout, growth behaviour
 * and performance characteristics.
 *
 * | Type                    | Memory    | Growth   | Best for              |
 * |-------------------------|-----------|----------|-----------------------|
 * | TR_STACK_ARRAY_DYNAMIC  | Contiguous| Automatic| General purpose       |
 * | TR_STACK_ARRAY_FIXED    | Contiguous| None     | Bounded/embedded use  |
 * | TR_STACK_LL             | Per node  | Automatic| Unpredictable size    |
 */
typedef enum tr_stack_type
{
        TR_STACK_ARRAY_DYNAMIC = 0, /**< Array based - grows automatically by factor of 2  */
        TR_STACK_ARRAY_FIXED = 1,   /**< Array based - fixed capacity, returns TR_ERR_FULL */
        TR_STACK_LL = 2             /**< Linked list based - dynamic node allocation       */
} tr_stack_type_t;

/*****************************************************/

/* 5 global variable declarations */
/*****************************************************/

/* 6 function prototypes */
/*****************************************************/

/******************************************************************
 *
 * FUNCTION NAME: tr_stack_create
 *
 * PURPOSE: Allocates the needed memory for the stack wanted
 *
 * ARGUMENTS:
 *
 * ARGUMENT                     TYPE            I/O     DESCRIPTION
 * --------                     ----            ---     ------------
 * size_of_datatype             size_t          I       Byte size of the datatype to store in the
 * elements_to_allocate         size_t          I       Initial number of elements to
 * stack_type                   tr_stack_type_t I       Type of stack to create (array or linked
 *                                                          list)
 * allocate id_of_stack         stack_t **      O       Pointer to pointer to receive the
 *                                                          created stack
 *
 * RETURNS: tr_result_t
 *   TR_OK               - Stack created successfully
 *   TR_ERR_NULL         - id_of_stack is NULL
 *   TR_ERR_INVALID      - size_of_datatype or elements_to_allocate is 0, or unknown stack_type
 *   TR_ERR_ALLOC        - Memory allocation failed
 *
 *****************************************************************/
/**
 * @brief Allocates and initialises a new stack instance
 *
 * @param[in]  size_of_datatype     Byte size of the datatype to store
 * @param[in]  elements_to_allocate Initial number of elements to allocate
 * @param[in]  stack_type           Implementation type to use
 * @param[out] id_of_stack          Pointer to receive the created stack
 *
 * @return TR_OK          Stack created successfully
 * @return TR_ERR_NULL    id_of_stack is NULL
 * @return TR_ERR_INVALID size_of_datatype or elements_to_allocate is 0
 * @return TR_ERR_ALLOC   Memory allocation failed
 */
TR_NODISCARD TR_API tr_result_t tr_stack_create(size_t size_of_datatype,
                                                size_t elements_to_allocate,
                                                tr_stack_type_t stack_type,
                                                struct stack **id_of_stack);

/******************************************************************
 *
 * FUNCTION NAME: tr_stack_destroy
 *
 * PURPOSE: Frees all memory associated with the stack instance
 *
 * ARGUMENTS:
 *
 * ARGUMENT     TYPE                    I/O     DESCRIPTION
 * --------     ----                    ---     ------------
 * id_of_stack  struct stack **         I/O     Pointer to pointer to the stack to
 *                                               destroy. Set to NULL after destruction
 *
 * RETURNS: tr_result_t
 *   TR_OK               - Stack destroyed successfully
 *   TR_ERR_NULL         - id_of_stack or *id_of_stack is NULL
 *
 *****************************************************************/
/**
 * @brief Frees all memory associated with the stack instance
 *
 * Destroys the stack and sets the pointer to @c NULL to prevent
 * use after free. Both the implementation data and the stack
 * handle are freed.
 *
 * @param[in,out] id_of_stack Pointer to pointer to the stack to destroy.
 *                            Set to @c NULL after destruction
 *
 * @return TR_OK       Stack destroyed successfully
 * @return TR_ERR_NULL id_of_stack or *id_of_stack is NULL
 *
 * Example:
 * @code
 * struct stack *p_stack = NULL;
 *
 * tr_stack_create(sizeof(int), 10, TR_STACK_ARRAY_DYNAMIC, &p_stack);
 *
 * tr_stack_destroy(&p_stack);
 * // p_stack is now NULL
 * @endcode
 */
TR_NODISCARD TR_API tr_result_t tr_stack_destroy(struct stack **id_of_stack);

/******************************************************************
 *
 * FUNCTION NAME: tr_stack_push
 *
 * PURPOSE: Pushes a element onto the top of the stack
 *
 * ARGUMENTS:
 *
 * ARGUMENT                TYPE            I/O     DESCRIPTION
 * --------                ----            ---     ------------
 * id_of_stack             struct stack *  I/O     Pointer to the stack to push onto
 * data_to_push            const void *    I       Pointer to the data to copy onto the stack
 *
 * RETURNS: tr_result_t
 *   TR_OK               - Data pushed successfully
 *   TR_ERR_NULL         - id_of_stack or data_to_push is NULL
 *   TR_ERR_ALLOC        - Memory allocation failed (linked list only)
 *   TR_ERR_FULL         - Stack is full (array based only)
 *
 *****************************************************************/
/**
 * @brief Pushes a deep copy of the data onto the top of the stack
 *
 * Copies @c size_of_datatype bytes from @p data_to_push into the
 * stack. The caller retains ownership of the original data.
 *
 * For @ref TR_STACK_ARRAY_DYNAMIC stacks the array grows automatically
 * by a factor of 2 when full. For @ref TR_STACK_ARRAY_FIXED stacks
 * @ref TR_ERR_FULL is returned when capacity is reached.
 *
 * @param[in,out] id_of_stack  Pointer to the stack to push onto
 * @param[in]     data_to_push Pointer to the data to copy onto the stack
 *
 * @return TR_OK        Data pushed successfully
 * @return TR_ERR_NULL  id_of_stack or data_to_push is NULL
 * @return TR_ERR_ALLOC Memory reallocation failed (dynamic only)
 * @return TR_ERR_FULL  Stack is at capacity (fixed only)
 *
 * Example:
 * @code
 * int         val = 42;
 * tr_result_t res = TR_OK;
 *
 * res = tr_stack_push(p_stack, &val);
 * if (TR_OK != res)
 * {
 *     // handle error
 * }
 * @endcode
 */
TR_NODISCARD TR_API tr_result_t tr_stack_push(struct stack *id_of_stack, const void *data_to_push);

/******************************************************************
 *
 * FUNCTION NAME: tr_stack_pop
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
/**
 * @brief Removes the element at the top of the stack
 *
 * Decrements the stack size by one. The data is not returned —
 * call @ref tr_stack_top first if you need the value before removing it.
 *
 * @param[in,out] id_of_stack Pointer to the stack to pop from
 *
 * @return TR_OK        Element removed successfully
 * @return TR_ERR_NULL  id_of_stack is NULL
 * @return TR_ERR_EMPTY Stack is empty
 *
 * Example:
 * @code
 * int         out = 0;
 * tr_result_t res = TR_OK;
 *
 * res = tr_stack_top(p_stack, &out);
 * if (TR_OK == res)
 * {
 *     tr_stack_pop(p_stack);
 * }
 * @endcode
 */
TR_NODISCARD TR_API tr_result_t tr_stack_pop(struct stack *id_of_stack);

/******************************************************************
 *
 * FUNCTION NAME: tr_stack_top
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
 *                                                        element into. Must be at least
 *                                                        size_of_datatype bytes
 *
 * RETURNS: tr_result_t
 *   TR_OK               - Data copied successfully
 *   TR_ERR_NULL         - id_of_stack or data_at_top is NULL
 *   TR_ERR_EMPTY        - Stack is empty
 *
 *****************************************************************/
/**
 * @brief Copies the element at the top of the stack into the provided buffer
 *
 * Copies @c size_of_datatype bytes from the top of the stack into
 * @p data_at_top. The element is not removed — call @ref tr_stack_pop
 * afterwards if removal is needed.
 *
 * The buffer pointed to by @p data_at_top must be at least
 * @c size_of_datatype bytes large.
 *
 * @param[in]  id_of_stack Pointer to the stack to peek at
 * @param[out] data_at_top Buffer to copy the top element into
 *
 * @return TR_OK        Data copied successfully
 * @return TR_ERR_NULL  id_of_stack or data_at_top is NULL
 * @return TR_ERR_EMPTY Stack is empty
 *
 * Example:
 * @code
 * int         out = 0;
 * tr_result_t res = TR_OK;
 *
 * res = tr_stack_top(p_stack, &out);
 * if (TR_OK == res)
 * {
 *     printf("Top value: %d\n", out);
 * }
 * @endcode
 */
TR_NODISCARD TR_API tr_result_t tr_stack_top(const struct stack *id_of_stack, void *data_at_top);

/******************************************************************
 *
 * FUNCTION NAME: tr_stack_size
 *
 * PURPOSE: Returns the current number of elements in the stack
 *
 * ARGUMENTS:
 *
 * ARGUMENT                TYPE                 I/O     DESCRIPTION
 * --------                ----                 ---     ------------
 * id_of_stack             const struct stack * I       Pointer to the stack to query
 * size                    size_t *             O       Pointer to receive the current element
 *                                                        count
 *
 * RETURNS: tr_result_t
 *   TR_OK               - Size retrieved successfully
 *   TR_ERR_NULL         - id_of_stack or size is NULL
 *
 *****************************************************************/
/**
 * @brief Returns the current number of elements in the stack
 *
 * @param[in]  id_of_stack Pointer to the stack to query
 * @param[out] size        Pointer to receive the current element count
 *
 * @return TR_OK       Size retrieved successfully
 * @return TR_ERR_NULL id_of_stack or size is NULL
 *
 * Example:
 * @code
 * size_t      size = 0u;
 * tr_result_t res  = TR_OK;
 *
 * res = tr_stack_size(p_stack, &size);
 * if (TR_OK == res)
 * {
 *     printf("Stack has %zu elements\n", size);
 * }
 * @endcode
 */
TR_NODISCARD TR_API tr_result_t tr_stack_size(const struct stack *id_of_stack, size_t *size);

/******************************************************************
 *
 * FUNCTION NAME: tr_stack_is_empty
 *
 * PURPOSE: Checks whether the stack contains no elements
 *
 * ARGUMENTS:
 *
 * ARGUMENT                TYPE                 I/O     DESCRIPTION
 * --------                ----                 ---     ------------
 * id_of_stack             const struct stack * I       Pointer to the stack to check
 * is_empty                bool *               O       Pointer to receive the result
 *                                                       Set to true if empty, false otherwise
 *
 * RETURNS: tr_result_t
 *   TR_OK               - Check completed successfully
 *   TR_ERR_NULL         - id_of_stack or is_empty is NULL
 *
 *****************************************************************/
/**
 * @brief Checks whether the stack contains no elements
 *
 * @param[in]  id_of_stack Pointer to the stack to check
 * @param[out] is_empty    Set to @c true if the stack is empty,
 *                         @c false otherwise
 *
 * @return TR_OK       Check completed successfully
 * @return TR_ERR_NULL id_of_stack or is_empty is NULL
 *
 * Example:
 * @code
 * bool        is_empty = false;
 * tr_result_t res      = TR_OK;
 *
 * res = tr_stack_is_empty(p_stack, &is_empty);
 * if (TR_OK == res && is_empty)
 * {
 *     printf("Stack is empty\n");
 * }
 * @endcode
 */
TR_NODISCARD TR_API tr_result_t tr_stack_is_empty(const struct stack *id_of_stack, bool *is_empty);

/******************************************************************
 *
 * FUNCTION NAME: tr_stack_capacity
 *
 * PURPOSE: Returns the total allocated capacity of the stack
 *          For linked list based stacks this is the same as stack_size
 *
 * ARGUMENTS:
 *
 * ARGUMENT                TYPE                 I/O     DESCRIPTION
 * --------                ----                 ---     ------------
 * id_of_stack             const struct stack * I       Pointer to the stack to query
 * capacity                size_t *             O       Pointer to receive the capacity
 *
 * RETURNS: tr_result_t
 *   TR_OK               - Capacity retrieved successfully
 *   TR_ERR_NULL         - id_of_stack or capacity is NULL
 *
 *****************************************************************/
/**
 * @brief Returns the total allocated capacity of the stack
 *
 * For @ref TR_STACK_ARRAY_DYNAMIC stacks capacity grows automatically
 * and may be larger than the current size. For @ref TR_STACK_ARRAY_FIXED
 * stacks capacity is fixed at creation time and never changes.
 * For @ref TR_STACK_LL stacks capacity equals the current size since
 * nodes are allocated individually.
 *
 * @param[in]  id_of_stack Pointer to the stack to query
 * @param[out] capacity    Pointer to receive the capacity
 *
 * @return TR_OK       Capacity retrieved successfully
 * @return TR_ERR_NULL id_of_stack or capacity is NULL
 *
 * Example:
 * @code
 * size_t      capacity = 0u;
 * size_t      size     = 0u;
 * tr_result_t res      = TR_OK;
 *
 * tr_stack_size(p_stack, &size);
 * res = tr_stack_capacity(p_stack, &capacity);
 * if (TR_OK == res)
 * {
 *     printf("Using %zu of %zu slots\n", size, capacity);
 * }
 * @endcode
 */
TR_NODISCARD TR_API tr_result_t tr_stack_capacity(const struct stack *id_of_stack,
                                                  size_t *capacity);

/*****************************************************/

#ifdef __cplusplus
}
#endif

#endif /* TR_STACK_H */

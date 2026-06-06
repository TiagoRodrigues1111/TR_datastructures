/*******************************************************************************************************
 * NAME: stack.c
 *
 * PURPOSE: Public API implementation for the stack data structure
 *          Dispatches to the appropriate implementation via function pointers
 *
 * GLOBAL VARIABLES: None
 *
 * DEVELOPMENT HISTORY:
 *
 * Date          Author          Change Id       Release         Description Of Change
 * ----------    --------------- ---------       -------         -----------------------------------
 * 06-06-2026    Tiago Rodrigues                       1         File preparation
 *
 *******************************************************************************************************/

/* 0 copyright/licensing */
/*******************************************************************************************************
 *
 * This is free and unencumbered software released into the public domain (Unlicense).
 *
 ********************************************************************************************************/

/* 1 includes */
/*****************************************************/

/* 1.1 Related header */
#include "tr_datastructures/tr_stack.h"

/* 1.2 C system headers */

/* 1.3 C++ standard library headers*/

/* 1.4 other libraries' headers*/

/* 1.5 project's headers */
#include "tr_internal.h"
#include "stack_shared.h"

/*****************************************************/

/* 2 defines */
/*****************************************************/
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

/*******************************************************************************************************
 *
 * FUNCTION NAME: tr_stack_create
 *
 * PURPOSE: Allocates and initialises a new stack instance
 *          Delegates to the appropriate implementation based on stack_type
 *
 * ARGUMENTS:
 *
 * ARGUMENT                TYPE            I/O     DESCRIPTION
 * --------                ----            ---     ------------
 * size_of_datatype        size_t          I       Byte size of the datatype to store
 * elements_to_allocate    size_t          I       Initial number of elements to allocate
 * stack_type              tr_stack_type_t I       Implementation type to use
 * id_of_stack             struct stack ** O       Pointer to pointer to receive the created stack
 *
 * RETURNS: tr_result_t
 *   TR_OK               - Stack created successfully
 *   TR_ERR_NULL         - id_of_stack is NULL
 *   TR_ERR_INVALID      - size_of_datatype or elements_to_allocate is 0, or unknown stack_type
 *   TR_ERR_ALLOC        - Memory allocation failed
 *
 *******************************************************************************************************/
tr_result_t tr_stack_create(size_t size_of_datatype,
                            size_t elements_to_allocate,
                            tr_stack_type_t stack_type,
                            struct stack **id_of_stack)
{
        /* local variables */
        /* none */

        if (NULL == id_of_stack)
        {
                return (TR_ERR_NULL);
        }

        if ((0u == size_of_datatype) || (0u == elements_to_allocate))
        {
                return (TR_ERR_INVALID);
        }

        if (TR_STACK_ARRAY_DYNAMIC == stack_type || TR_STACK_ARRAY_FIXED == stack_type)
        {
                return (tr_stack_array_create(
                        size_of_datatype, elements_to_allocate, stack_type, id_of_stack));
        }
        else if (TR_STACK_LL == stack_type)
        {
                return (tr_stack_ll_create(size_of_datatype, elements_to_allocate, id_of_stack));
        }

        return (TR_ERR_INVALID);
}

/*******************************************************************************************************
 *
 * FUNCTION NAME: tr_stack_destroy
 *
 * PURPOSE: Frees all memory associated with the stack instance
 *
 * ARGUMENTS:
 *
 * ARGUMENT             TYPE                I/O     DESCRIPTION
 * --------             ----                ---     ------------
 * id_of_stack          struct stack **     I/O     Pointer to pointer to the stack to destroy
 *
 * RETURNS: tr_result_t
 *   TR_OK               - Stack destroyed successfully
 *   TR_ERR_NULL         - id_of_stack or *id_of_stack is NULL
 *
 *******************************************************************************************************/
tr_result_t tr_stack_destroy(struct stack **id_of_stack)
{
        /* local variables */
        /* none */

        if (NULL == id_of_stack || NULL == *id_of_stack)
        {
                return (TR_ERR_NULL);
        }

        return ((*id_of_stack)->ops->destroy(id_of_stack));
}

/*******************************************************************************************************
 *
 * FUNCTION NAME: tr_stack_push
 *
 * PURPOSE: Pushes a deep copy of the data onto the top of the stack
 *
 * ARGUMENTS:
 *
 * ARGUMENT             TYPE                I/O     DESCRIPTION
 * --------             ----                ---     ------------
 * id_of_stack          struct stack *      I/O     Pointer to the stack
 * data_to_push         const void *        I       Pointer to the data to copy
 *
 * RETURNS: tr_result_t
 *   TR_OK               - Data pushed successfully
 *   TR_ERR_NULL         - id_of_stack or data_to_push is NULL
 *   TR_ERR_ALLOC        - Memory allocation failed (linked list only)
 *   TR_ERR_FULL         - Stack is full (fixed only)
 *
 *******************************************************************************************************/
tr_result_t tr_stack_push(struct stack *id_of_stack, const void *data_to_push)
{
        /* local variables */
        /* none */

        if (NULL == id_of_stack || NULL == data_to_push)
        {
                return (TR_ERR_NULL);
        }

        return (id_of_stack->ops->push(id_of_stack, data_to_push));
}

/*******************************************************************************************************
 *
 * FUNCTION NAME: tr_stack_pop
 *
 * PURPOSE: Removes the element at the top of the stack
 *
 * ARGUMENTS:
 *
 * ARGUMENT             TYPE                I/O     DESCRIPTION
 * --------             ----                ---     ------------
 * id_of_stack          struct stack *      I/O     Pointer to the stack
 *
 * RETURNS: tr_result_t
 *   TR_OK               - Element removed successfully
 *   TR_ERR_NULL         - id_of_stack is NULL
 *   TR_ERR_EMPTY        - Stack is empty
 *
 *******************************************************************************************************/
tr_result_t tr_stack_pop(struct stack *id_of_stack)
{
        /* local variables */
        /* none */

        if (NULL == id_of_stack)
        {
                return (TR_ERR_NULL);
        }

        return (id_of_stack->ops->pop(id_of_stack));
}

/*******************************************************************************************************
 *
 * FUNCTION NAME: tr_stack_top
 *
 * PURPOSE: Copies the element at the top of the stack into the provided buffer
 *
 * ARGUMENTS:
 *
 * ARGUMENT             TYPE                    I/O     DESCRIPTION
 * --------             ----                    ---     ------------
 * id_of_stack          const struct stack *    I       Pointer to the stack
 * data_at_top          void *                  O       Buffer to copy the top element into
 *
 * RETURNS: tr_result_t
 *   TR_OK               - Data copied successfully
 *   TR_ERR_NULL         - id_of_stack or data_at_top is NULL
 *   TR_ERR_EMPTY        - Stack is empty
 *
 *******************************************************************************************************/
tr_result_t tr_stack_top(const struct stack *id_of_stack, void *data_at_top)
{
        /* local variables */
        /* none */

        if (NULL == id_of_stack || NULL == data_at_top)
        {
                return (TR_ERR_NULL);
        }

        return (id_of_stack->ops->top(id_of_stack, data_at_top));
}

/*******************************************************************************************************
 *
 * FUNCTION NAME: tr_stack_size
 *
 * PURPOSE: Returns the current number of elements in the stack
 *
 * ARGUMENTS:
 *
 * ARGUMENT             TYPE                    I/O     DESCRIPTION
 * --------             ----                    ---     ------------
 * id_of_stack          const struct stack *    I       Pointer to the stack
 * size                 size_t *                O       Pointer to receive the current element count
 *
 * RETURNS: tr_result_t
 *   TR_OK               - Size retrieved successfully
 *   TR_ERR_NULL         - id_of_stack or size is NULL
 *
 *******************************************************************************************************/
tr_result_t tr_stack_size(const struct stack *id_of_stack, size_t *size)
{
        /* local variables */
        /* none */

        if (NULL == id_of_stack || NULL == size)
        {
                return (TR_ERR_NULL);
        }

        return (id_of_stack->ops->size(id_of_stack, size));
}

/*******************************************************************************************************
 *
 * FUNCTION NAME: tr_stack_is_empty
 *
 * PURPOSE: Checks whether the stack contains no elements
 *
 * ARGUMENTS:
 *
 * ARGUMENT             TYPE                    I/O     DESCRIPTION
 * --------             ----                    ---     ------------
 * id_of_stack          const struct stack *    I       Pointer to the stack
 * is_empty             bool *                  O       Set to true if empty, false otherwise
 *
 * RETURNS: tr_result_t
 *   TR_OK               - Check completed successfully
 *   TR_ERR_NULL         - id_of_stack or is_empty is NULL
 *
 *******************************************************************************************************/
tr_result_t tr_stack_is_empty(const struct stack *id_of_stack, bool *is_empty)
{
        /* local variables */
        /* none */

        if (NULL == id_of_stack || NULL == is_empty)
        {
                return (TR_ERR_NULL);
        }

        return (id_of_stack->ops->is_empty(id_of_stack, is_empty));
}

/*******************************************************************************************************
 *
 * FUNCTION NAME: tr_stack_capacity
 *
 * PURPOSE: Returns the total allocated capacity of the stack
 *
 * ARGUMENTS:
 *
 * ARGUMENT             TYPE                    I/O     DESCRIPTION
 * --------             ----                    ---     ------------
 * id_of_stack          const struct stack *    I       Pointer to the stack
 * capacity             size_t *                O       Pointer to receive the capacity
 *
 * RETURNS: tr_result_t
 *   TR_OK               - Capacity retrieved successfully
 *   TR_ERR_NULL         - id_of_stack or capacity is NULL
 *
 *******************************************************************************************************/
tr_result_t tr_stack_capacity(const struct stack *id_of_stack, size_t *capacity)
{
        /* local variables */
        /* none */

        if (NULL == id_of_stack || NULL == capacity)
        {
                return (TR_ERR_NULL);
        }

        return (id_of_stack->ops->capacity(id_of_stack, capacity));
}
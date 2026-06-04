/*******************************************************************************************************
 * NAME: stack.c
 *
 * PURPOSE: Array based implementation of the stack data structure
 *
 * GLOBAL VARIABLES: None
 *
 * DEVELOPMENT HISTORY:
 *
 * Date          Author          Change Id       Release         Description Of Change
 * ----------    --------------- ---------       -------         -----------------------------------
 * 31-05-2026    Tiago Rodrigues                       1         File preparation
 *
 *
 * ASSUMPTIONS, CONSTRAINTS, RESTRICTIONS:
 *
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
#include <stdlib.h> /* malloc, realloc, free    */
#include <string.h> /* memcpy                   */
#include <stdio.h>  /* fprintf, stderr          */

/* 1.3 C++ standard library headers*/

/* 1.4 other libraries' headers*/

/* 1.5 project's headers */
#include "tr_internal.h"

/*****************************************************/

/* 2 defines */
/*****************************************************/
#define GROWTH_FACTOR 2u

/*****************************************************/

/* 3 external declarations */
/*****************************************************/
/*****************************************************/

/* 4 typedefs */
/*****************************************************/

/*******************************************************************************************************
 *
 * TYPE NAME: struct tr_stack_array_data
 *
 * PURPOSE: Internal data for the array based stack implementation
 *
 * MEMBERS:
 *
 * MEMBER               TYPE        DESCRIPTION
 * ------               ----        -----------
 * stack_data           void *      Pointer to the allocated array of elements
 * stack_size           size_t      Current number of elements in the stack
 * capacity             size_t      Total number of elements allocated
 * size_of_datatype     size_t      Byte size of each element
 *
 *******************************************************************************************************/
struct tr_stack_array_data
{
        void *stack_data;
        size_t stack_size;
        size_t capacity;
        size_t size_of_datatype;
};

/*******************************************************************************************************
 *
 * TYPE NAME: struct stack
 *
 * PURPOSE: Internal definition of the opaque stack handle
 *          Contains the dispatch table and implementation specific data
 *
 * MEMBERS:
 *
 * MEMBER               TYPE                    DESCRIPTION
 * ------               ----                    -----------
 * ops                  struct tr_stack_ops *       Pointer to the dispatch table
 * impl                 void *                  Pointer to implementation specific data
 *
 *******************************************************************************************************/
struct stack
{
        const struct tr_stack_ops *ops;
        void *impl;
};

/*******************************************************************************************************
 *
 * TYPE NAME: struct tr_stack_ops
 *
 * PURPOSE: Dispatch table for stack operations
 *          Function pointers are set at creation time based on implementation type
 *
 * MEMBERS:
 *
 * MEMBER               TYPE                                            DESCRIPTION
 * ------               ----                                            -----------
 * push                 tr_result_t (*)(struct stack *, const void *)   Push operation
 * pop                  tr_result_t (*)(struct stack *)                 Pop operation
 * top                  tr_result_t (*)(const struct stack *, void *)   Top operation
 * size                 tr_result_t (*)(const struct stack *, size_t *) Size operation
 * is_empty             tr_result_t (*)(const struct stack *, bool *)   Is empty operation
 * capacity             tr_result_t (*)(const struct stack *, size_t *) Capacity operation
 * destroy              tr_result_t (*)(struct stack **)                Destroy operation
 *
 *******************************************************************************************************/
struct tr_stack_ops
{
        tr_result_t (*push)(struct stack *id_of_stack, const void *data_to_push);
        tr_result_t (*pop)(struct stack *id_of_stack);
        tr_result_t (*top)(const struct stack *id_of_stack, void *data_at_top);
        tr_result_t (*size)(const struct stack *id_of_stack, size_t *size);
        tr_result_t (*is_empty)(const struct stack *id_of_stack, bool *is_empty);
        tr_result_t (*capacity)(const struct stack *id_of_stack, size_t *capacity);
        tr_result_t (*destroy)(struct stack **id_of_stack);
};

/*****************************************************/

/* 5 global variable declarations */
/*****************************************************/
/*****************************************************/

/* 6 function prototypes */
/*****************************************************/

static tr_result_t stack_array_dynamic_push(struct stack *id_of_stack, const void *data_to_push);
static tr_result_t stack_array_fixed_push(struct stack *id_of_stack, const void *data_to_push);
static tr_result_t stack_array_pop(struct stack *id_of_stack);
static tr_result_t stack_array_top(const struct stack *id_of_stack, void *data_at_top);
static tr_result_t stack_array_size(const struct stack *id_of_stack, size_t *size);
static tr_result_t stack_array_is_empty(const struct stack *id_of_stack, bool *is_empty);
static tr_result_t stack_array_capacity(const struct stack *id_of_stack, size_t *capacity);
static tr_result_t stack_array_destroy(struct stack **id_of_stack);

/*****************************************************/

/*******************************************************************************************************
 *
 * FUNCTION NAME: stack_array_dynamic_push
 *
 * PURPOSE: Pushes a deep copy of the data onto the top of the array stack
 *          Grows the array by GROWTH_FACTOR if the stack is full
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
 *   TR_ERR_ALLOC        - Memory reallocation failed
 *
 *******************************************************************************************************/
static tr_result_t stack_array_dynamic_push(struct stack *id_of_stack, const void *data_to_push)
{
        /* local variables */
        struct tr_stack_array_data *p_data = NULL; /* pointer to implementation data  */
        void *p_new_data = NULL;                   /* pointer for realloc             */
        size_t new_cap = 0u;                       /* new capacity after growth       */

        p_data = (struct tr_stack_array_data *) id_of_stack->impl;

        /* grow array if full */
        if (p_data->stack_size == p_data->capacity)
        {
                new_cap = p_data->capacity * GROWTH_FACTOR;
                p_new_data = realloc(p_data->stack_data, new_cap * p_data->size_of_datatype);

                if (NULL == p_new_data)
                {
                        fprintf(stderr, "[TR] stack_array_push: realloc failed\n");
                        return (TR_ERR_ALLOC);
                }

                p_data->stack_data = p_new_data;
                p_data->capacity = new_cap;
        }

        /* copy data to top of stack */
        memcpy((uint8_t *) p_data->stack_data + (p_data->stack_size * p_data->size_of_datatype),
               data_to_push,
               p_data->size_of_datatype);

        p_data->stack_size++;

        return (TR_OK);
}

/*******************************************************************************************************
 *
 * FUNCTION NAME: stack_array_fixed_push
 *
 * PURPOSE: Pushes a deep copy of the data onto the top of the array stack
 *          Returns TR_ERR_FULL if the stack is full
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
 *   TR_ERR_ALLOC        - Memory reallocation failed
 *   TR_ERR_FULL         - Stack is full
 *
 *******************************************************************************************************/
static tr_result_t stack_array_fixed_push(struct stack *id_of_stack, const void *data_to_push)
{
        /* local variables */
        struct tr_stack_array_data *p_data = NULL;

        p_data = (struct tr_stack_array_data *) id_of_stack->impl;

        /* Return error if full */
        if (p_data->stack_size == p_data->capacity)
        {
                return (TR_ERR_FULL);
        }

        /* copy data to top of stack */
        memcpy((uint8_t *) p_data->stack_data + (p_data->stack_size * p_data->size_of_datatype),
               data_to_push,
               p_data->size_of_datatype);

        p_data->stack_size++;

        return (TR_OK);
}

/*******************************************************************************************************
 *
 * FUNCTION NAME: stack_array_pop
 *
 * PURPOSE: Removes the element at the top of the array stack
 *
 * ARGUMENTS:
 *
 * ARGUMENT             TYPE                I/O     DESCRIPTION
 * --------             ----                ---     ------------
 * id_of_stack          struct stack *      I/O     Pointer to the stack
 *
 * RETURNS: tr_result_t
 *   TR_OK               - Element removed successfully
 *   TR_ERR_EMPTY        - Stack is empty
 *
 *******************************************************************************************************/
static tr_result_t stack_array_pop(struct stack *id_of_stack)
{
        /* local variables */
        struct tr_stack_array_data *p_data = NULL; /* pointer to implementation data */

        p_data = (struct tr_stack_array_data *) id_of_stack->impl;

        if (0u == p_data->stack_size)
        {
                return (TR_ERR_EMPTY);
        }

        p_data->stack_size--;

        return (TR_OK);
}

/*******************************************************************************************************
 *
 * FUNCTION NAME: stack_array_top
 *
 * PURPOSE: Copies the element at the top of the array stack into the provided buffer
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
 *   TR_ERR_EMPTY        - Stack is empty
 *
 *******************************************************************************************************/
static tr_result_t stack_array_top(const struct stack *id_of_stack, void *data_at_top)
{
        /* local variables */
        const struct tr_stack_array_data *p_data = NULL; /* pointer to implementation data */

        p_data = (const struct tr_stack_array_data *) id_of_stack->impl;

        if (0u == p_data->stack_size)
        {
                return (TR_ERR_EMPTY);
        }

        memcpy(data_at_top,
               (const uint8_t *) p_data->stack_data +
                       ((p_data->stack_size - 1u) * p_data->size_of_datatype),
               p_data->size_of_datatype);

        return (TR_OK);
}

/*******************************************************************************************************
 *
 * FUNCTION NAME: stack_array_size
 *
 * PURPOSE: Returns the current number of elements in the array stack
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
 *
 *******************************************************************************************************/
static tr_result_t stack_array_size(const struct stack *id_of_stack, size_t *size)
{
        /* local variables */
        const struct tr_stack_array_data *p_data = NULL; /* pointer to implementation data */

        p_data = (const struct tr_stack_array_data *) id_of_stack->impl;
        *size = p_data->stack_size;

        return (TR_OK);
}

/*******************************************************************************************************
 *
 * FUNCTION NAME: stack_array_is_empty
 *
 * PURPOSE: Checks whether the array stack contains no elements
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
 *
 *******************************************************************************************************/
static tr_result_t stack_array_is_empty(const struct stack *id_of_stack, bool *is_empty)
{
        /* local variables */
        const struct tr_stack_array_data *p_data = NULL; /* pointer to implementation data */

        p_data = (const struct tr_stack_array_data *) id_of_stack->impl;
        *is_empty = (0u == p_data->stack_size);

        return (TR_OK);
}

/*******************************************************************************************************
 *
 * FUNCTION NAME: stack_array_capacity
 *
 * PURPOSE: Returns the total allocated capacity of the array stack
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
 *
 *******************************************************************************************************/
static tr_result_t stack_array_capacity(const struct stack *id_of_stack, size_t *capacity)
{
        /* local variables */
        const struct tr_stack_array_data *p_data = NULL; /* pointer to implementation data */

        p_data = (const struct tr_stack_array_data *) id_of_stack->impl;
        *capacity = p_data->capacity;

        return (TR_OK);
}

/*******************************************************************************************************
 *
 * FUNCTION NAME: stack_array_destroy
 *
 * PURPOSE: Frees all memory associated with the array stack instance
 *
 * ARGUMENTS:
 *
 * ARGUMENT             TYPE                I/O     DESCRIPTION
 * --------             ----                ---     ------------
 * id_of_stack          struct stack **     I/O     Pointer to pointer to the stack to destroy
 *
 * RETURNS: tr_result_t
 *   TR_OK               - Stack destroyed successfully
 *
 *******************************************************************************************************/
static tr_result_t stack_array_destroy(struct stack **id_of_stack)
{
        /* local variables */
        struct tr_stack_array_data *p_data = NULL; /* pointer to implementation data */

        p_data = (struct tr_stack_array_data *) (*id_of_stack)->impl;

        free(p_data->stack_data);
        p_data->stack_data = NULL;
        p_data->stack_size = 0u;
        p_data->capacity = 0u;
        p_data->size_of_datatype = 0u;

        free(p_data);
        free(*id_of_stack);
        *id_of_stack = NULL;

        return (TR_OK);
}

/*******************************************************************************************************
 *
 * Static dispatch tables for array based stack
 *
 *******************************************************************************************************/
static const struct tr_stack_ops k_stack_array_dynamic_ops = {
        stack_array_dynamic_push, stack_array_pop,      stack_array_top,    stack_array_size,
        stack_array_is_empty,     stack_array_capacity, stack_array_destroy
};

static const struct tr_stack_ops k_stack_array_fixed_ops = {
        stack_array_fixed_push, stack_array_pop,      stack_array_top,    stack_array_size,
        stack_array_is_empty,   stack_array_capacity, stack_array_destroy
};

/*******************************************************************************************************
 *
 * FUNCTION NAME: tr_stack_create
 *
 * PURPOSE: Allocates and initialises a new stack instance
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
        struct stack *p_stack = NULL;              /* pointer to new stack handle     */
        struct tr_stack_array_data *p_data = NULL; /* pointer to implementation data  */

        /* validate arguments */
        if (NULL == id_of_stack)
        {
                return (TR_ERR_NULL);
        }

        if ((0u == size_of_datatype) || (0u == elements_to_allocate))
        {
                return (TR_ERR_INVALID);
        }

        if ((TR_STACK_ARRAY_DYNAMIC != stack_type) && (TR_STACK_ARRAY_FIXED != stack_type))
        {
                return (TR_ERR_INVALID);
        }

        /* Allocation of a stack struct */
        p_stack = (struct stack *) malloc(sizeof(struct stack));
        if (NULL == p_stack)
        {
                fprintf(stderr, "[TR] stack_create: malloc failed for stack handle\n");
                return (TR_ERR_ALLOC);
        }

        /* Allocate implementation data */
        p_data = (struct tr_stack_array_data *) malloc(sizeof(struct tr_stack_array_data));
        if (NULL == p_data)
        {
                fprintf(stderr, "[TR] stack_create: malloc failed for implementation data\n");
                free(p_stack);
                return (TR_ERR_ALLOC);
        }

        /* Allocate data array */
        p_data->stack_data = malloc(elements_to_allocate * size_of_datatype);
        if (NULL == p_data->stack_data)
        {
                fprintf(stderr, "[TR] stack_create: malloc failed for data array\n");
                free(p_data);
                free(p_stack);
                return (TR_ERR_ALLOC);
        }

        /* Initialise implementation data */
        p_data->stack_size = 0u;
        p_data->capacity = elements_to_allocate;
        p_data->size_of_datatype = size_of_datatype;

        /* Wire up dispatch table and implementation data */
        if (TR_STACK_ARRAY_DYNAMIC == stack_type)
        {
                p_stack->ops = &k_stack_array_dynamic_ops;
        }
        else if (TR_STACK_ARRAY_FIXED == stack_type)
        {
                p_stack->ops = &k_stack_array_fixed_ops;
        }
        else
        {
                /* TR_STACK_LL handled in stack_ll.c */
                free(p_data->stack_data);
                free(p_data);
                free(p_stack);
                return (TR_ERR_INVALID);
        }
        p_stack->impl = p_data;

        *id_of_stack = p_stack;

        return (TR_OK);
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
 *   TR_ERR_FULL         - Stack is full (array based only)
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

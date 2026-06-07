/*******************************************************************************************************
 * NAME: stack_ll.c
 *
 * PURPOSE: Linked list based implementation of the stack data structure
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
#include <stdlib.h> /* malloc, free, abort      */
#include <string.h> /* memcpy                   */
#include <stdio.h>  /* fprintf, stderr          */

/* 1.3 C++ standard library headers*/

/* 1.4 other libraries' headers*/

/* 1.5 project's headers */
#include "tr_internal.h"
#include "stack_shared.h"

/*****************************************************/

/* 2 defines */
/*****************************************************/

/*
 * STACK_LL_NODE_SIZE - computes the total allocation size for a node
 * On C99 and later the flexible array member data[] has zero size
 * On C89 data[1] contributes 1 byte to sizeof so we subtract it
 */
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
#define STACK_LL_NODE_SIZE(datatype_size) (sizeof(struct tr_stack_ll_node) + (datatype_size))
#else
#define STACK_LL_NODE_SIZE(datatype_size) (sizeof(struct tr_stack_ll_node) - 1u + (datatype_size))
#endif
/*****************************************************/

/* 3 external declarations */
/*****************************************************/
/*****************************************************/

/* 4 typedefs */
/*****************************************************/

/*******************************************************************************************************
 *
 * TYPE NAME: struct tr_stack_ll_node
 *
 * PURPOSE: A single node in the linked list stack
 *          Data is stored inline at the end of the node using a flexible
 *          array member on C99 or later, or a single element array on C89
 *
 * MEMBERS:
 *
 * MEMBER               TYPE                    DESCRIPTION
 * ------               ----                    -----------
 * next                 struct tr_stack_ll_node *  Pointer to the next node down the stack
 * data                 uint8_t[]               Inline data storage
 *
 *******************************************************************************************************/
struct tr_stack_ll_node
{
        struct tr_stack_ll_node *next;

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
        /* C99 or later - use flexible array member */
        uint8_t data[];
#else
        /* C89 fallback - single element array */
        uint8_t data[1];
#endif
};

/*******************************************************************************************************
 *
 * TYPE NAME: struct tr_stack_ll_data
 *
 * PURPOSE: Internal data for the linked list stack implementation
 *
 * MEMBERS:
 *
 * MEMBER               TYPE                    DESCRIPTION
 * ------               ----                    -----------
 * top                  struct tr_stack_ll_node *  Pointer to the top node
 * stack_size           size_t                  Current number of elements
 * size_of_datatype     size_t                  Byte size of each element
 *
 *******************************************************************************************************/
struct tr_stack_ll_data
{
        struct tr_stack_ll_node *top;
        size_t stack_size;
        size_t size_of_datatype;
};

/*****************************************************/

/* 5 global variable declarations */
/*****************************************************/
/*****************************************************/

/* 6 function prototypes */
/*****************************************************/
static tr_result_t stack_ll_push(struct stack *id_of_stack, const void *data_to_push);
static tr_result_t stack_ll_pop(struct stack *id_of_stack);
static tr_result_t stack_ll_top(const struct stack *id_of_stack, void *data_at_top);
static tr_result_t stack_ll_size(const struct stack *id_of_stack, size_t *size);
static tr_result_t stack_ll_is_empty(const struct stack *id_of_stack, bool *is_empty);
static tr_result_t stack_ll_capacity(const struct stack *id_of_stack, size_t *capacity);
static tr_result_t stack_ll_destroy(struct stack **id_of_stack);

/*****************************************************/

/*******************************************************************************************************
 *
 * FUNCTION NAME: stack_ll_push
 *
 * PURPOSE: Pushes a deep copy of the data onto the top of the linked list stack
 *          Allocates a new node with inline data storage
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
 *   TR_ERR_ALLOC        - Memory allocation failed
 *
 *******************************************************************************************************/
static tr_result_t stack_ll_push(struct stack *id_of_stack, const void *data_to_push)
{
        /* local variables */
        struct tr_stack_ll_data *p_data = NULL; /* pointer to implementation data   */
        struct tr_stack_ll_node *p_node = NULL; /* pointer to new node              */

        TR_ASSERT(NULL != id_of_stack);

        p_data = (struct tr_stack_ll_data *) id_of_stack->impl;

        TR_ASSERT(NULL != p_data);
        TR_ASSERT(p_data->size_of_datatype > 0u);
        TR_ASSERT((0u == p_data->stack_size) || (NULL != p_data->top));
        TR_ASSERT((0u != p_data->stack_size) || (NULL == p_data->top));

        /* allocate node with inline data */
        p_node = (struct tr_stack_ll_node *) malloc(STACK_LL_NODE_SIZE(p_data->size_of_datatype));

        if (NULL == p_node) 
        {
                /* LCOV_EXCL_START */
                fprintf(stderr, "[TR] stack_ll_push: malloc failed\n");
                return (TR_ERR_ALLOC);
                /* LCOV_EXCL_STOP */
        } 

        /* copy data inline into node */
        memcpy(p_node->data, data_to_push, p_data->size_of_datatype);

        /* push node onto top of stack */
        p_node->next = p_data->top;
        p_data->top = p_node;

        p_data->stack_size++;

        TR_ASSERT(NULL != p_data->top);
        TR_ASSERT(p_data->stack_size > 0u);
        TR_ASSERT((0u == p_data->stack_size) || (NULL != p_data->top));
        TR_ASSERT((0u != p_data->stack_size) || (NULL == p_data->top));

        return (TR_OK);
}

/*******************************************************************************************************
 *
 * FUNCTION NAME: stack_ll_pop
 *
 * PURPOSE: Removes the node at the top of the linked list stack and frees it
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
static tr_result_t stack_ll_pop(struct stack *id_of_stack)
{
        /* local variables */
        struct tr_stack_ll_data *p_data = NULL;    /* pointer to implementation data   */
        struct tr_stack_ll_node *p_old_top = NULL; /* pointer to node being removed    */

        TR_ASSERT(NULL != id_of_stack);

        p_data = (struct tr_stack_ll_data *) id_of_stack->impl;

        TR_ASSERT(NULL != p_data);
        TR_ASSERT((0u == p_data->stack_size) || (NULL != p_data->top));
        TR_ASSERT((0u != p_data->stack_size) || (NULL == p_data->top));

        if (0u == p_data->stack_size)
        {
                return (TR_ERR_EMPTY);
        }

        /* unlink top node */
        p_old_top = p_data->top;
        p_data->top = p_old_top->next;

        /* zero and free the node */
        memset(p_old_top, 0, STACK_LL_NODE_SIZE(p_data->size_of_datatype));
        free(p_old_top);

        p_data->stack_size--;

        TR_ASSERT((0u == p_data->stack_size) || (NULL != p_data->top));
        TR_ASSERT((0u != p_data->stack_size) || (NULL == p_data->top));

        return (TR_OK);
}

/*******************************************************************************************************
 *
 * FUNCTION NAME: stack_ll_top
 *
 * PURPOSE: Copies the data from the top node into the provided buffer
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
static tr_result_t stack_ll_top(const struct stack *id_of_stack, void *data_at_top)
{
        /* local variables */
        const struct tr_stack_ll_data *p_data = NULL; /* pointer to implementation data */

        TR_ASSERT(NULL != id_of_stack);

        p_data = (const struct tr_stack_ll_data *) id_of_stack->impl;

        TR_ASSERT(NULL != p_data);
        TR_ASSERT(p_data->size_of_datatype > 0u);
        TR_ASSERT((0u == p_data->stack_size) || (NULL != p_data->top));
        TR_ASSERT((0u != p_data->stack_size) || (NULL == p_data->top));

        if (0u == p_data->stack_size)
        {
                return (TR_ERR_EMPTY);
        }

        memcpy(data_at_top, p_data->top->data, p_data->size_of_datatype);

        return (TR_OK);
}

/*******************************************************************************************************
 *
 * FUNCTION NAME: stack_ll_size
 *
 * PURPOSE: Returns the current number of elements in the linked list stack
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
static tr_result_t stack_ll_size(const struct stack *id_of_stack, size_t *size)
{
        /* local variables */
        const struct tr_stack_ll_data *p_data = NULL; /* pointer to implementation data */

        TR_ASSERT(NULL != id_of_stack);

        p_data = (const struct tr_stack_ll_data *) id_of_stack->impl;

        TR_ASSERT(NULL != p_data);

        *size = p_data->stack_size;

        return (TR_OK);
}

/*******************************************************************************************************
 *
 * FUNCTION NAME: stack_ll_is_empty
 *
 * PURPOSE: Checks whether the linked list stack contains no elements
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
static tr_result_t stack_ll_is_empty(const struct stack *id_of_stack, bool *is_empty)
{
        /* local variables */
        const struct tr_stack_ll_data *p_data = NULL; /* pointer to implementation data */

        TR_ASSERT(NULL != id_of_stack);

        p_data = (const struct tr_stack_ll_data *) id_of_stack->impl;

        TR_ASSERT(NULL != p_data);
        TR_ASSERT((0u == p_data->stack_size) || (NULL != p_data->top));
        TR_ASSERT((0u != p_data->stack_size) || (NULL == p_data->top));

        *is_empty = (0u == p_data->stack_size);

        return (TR_OK);
}

/*******************************************************************************************************
 *
 * FUNCTION NAME: stack_ll_capacity
 *
 * PURPOSE: Returns the capacity of the linked list stack
 *          For linked list stacks capacity always equals current size
 *          since nodes are allocated individually on demand
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
static tr_result_t stack_ll_capacity(const struct stack *id_of_stack, size_t *capacity)
{
        /* local variables */
        const struct tr_stack_ll_data *p_data = NULL; /* pointer to implementation data */

        TR_ASSERT(NULL != id_of_stack);

        p_data = (const struct tr_stack_ll_data *) id_of_stack->impl;

        TR_ASSERT(NULL != p_data);

        *capacity = p_data->stack_size;

        return (TR_OK);
}

/*******************************************************************************************************
 *
 * FUNCTION NAME: stack_ll_destroy
 *
 * PURPOSE: Frees all nodes and implementation data for the linked list stack
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
static tr_result_t stack_ll_destroy(struct stack **id_of_stack)
{
        /* local variables */
        struct tr_stack_ll_data *p_data = NULL;    /* pointer to implementation data   */
        struct tr_stack_ll_node *p_current = NULL; /* pointer to current node          */
        struct tr_stack_ll_node *p_next = NULL;    /* pointer to next node             */

        TR_ASSERT(NULL != id_of_stack);
        TR_ASSERT(NULL != (*id_of_stack));

        p_data = (struct tr_stack_ll_data *) (*id_of_stack)->impl;

        TR_ASSERT(NULL != p_data);

        /* free all nodes */
        p_current = p_data->top;
        p_data->top = NULL;

        while (NULL != p_current)
        {
                p_next = p_current->next;
                memset(p_current, 0, STACK_LL_NODE_SIZE(p_data->size_of_datatype));
                free(p_current);
                p_current = p_next;
        }

        TR_ASSERT(NULL == p_data->top);
        TR_ASSERT(NULL == p_current);

        /* zero and free implementation data */
        p_data->stack_size = 0u;
        p_data->size_of_datatype = 0u;

        free(p_data);
        free(*id_of_stack);
        *id_of_stack = NULL;

        TR_ASSERT(NULL == *id_of_stack);
        return (TR_OK);
}

/*******************************************************************************************************
 *
 * Static dispatch table for linked list stack
 *
 *******************************************************************************************************/
static const struct tr_stack_ops k_stack_ll_ops = { stack_ll_push,     stack_ll_pop,
                                                    stack_ll_top,      stack_ll_size,
                                                    stack_ll_is_empty, stack_ll_capacity,
                                                    stack_ll_destroy };

/*******************************************************************************************************
 *
 * FUNCTION NAME: tr_stack_ll_create
 *
 * PURPOSE: Internal function called by tr_stack_create when TR_STACK_LL is selected
 *          Allocates and initialises the linked list stack implementation
 *
 * ARGUMENTS:
 *
 * ARGUMENT                TYPE            I/O     DESCRIPTION
 * --------                ----            ---     ------------
 * size_of_datatype        size_t          I       Byte size of the datatype to store
 * elements_to_allocate    size_t          I       Ignored for linked list stack, can be set to 0
 * id_of_stack             struct stack ** O       Pointer to pointer to receive the created stack
 *
 * RETURNS: tr_result_t
 *   TR_OK               - Stack created successfully
 *   TR_ERR_ALLOC        - Memory allocation failed
 *
 *******************************************************************************************************/
tr_result_t
tr_stack_ll_create(size_t size_of_datatype, size_t elements_to_allocate, struct stack **id_of_stack)
{
        TR_UNUSED(elements_to_allocate);
        /* local variables */
        struct stack *p_stack = NULL;           /* pointer to new stack handle      */
        struct tr_stack_ll_data *p_data = NULL; /* pointer to implementation data   */

        /* allocate stack handle */
        p_stack = (struct stack *) malloc(sizeof(struct stack));
        if (NULL == p_stack)
        {
                /* LCOV_EXCL_START */
                fprintf(stderr, "[TR] tr_stack_ll_create: malloc failed for stack handle\n");
                return (TR_ERR_ALLOC);
                /* LCOV_EXCL_STOP */
        }

        /* allocate implementation data */
        p_data = (struct tr_stack_ll_data *) malloc(sizeof(struct tr_stack_ll_data));
        if (NULL == p_data)
        {
                /* LCOV_EXCL_START */
                fprintf(stderr, "[TR] tr_stack_ll_create: malloc failed for implementation data\n");
                free(p_stack);
                return (TR_ERR_ALLOC);
                /* LCOV_EXCL_STOP */
        }

        /* initialise implementation data */
        p_data->top = NULL;
        p_data->stack_size = 0u;
        p_data->size_of_datatype = size_of_datatype;

        /* wire up dispatch table and implementation data */
        p_stack->ops = &k_stack_ll_ops;
        p_stack->impl = p_data;

        TR_ASSERT(NULL != p_stack->ops);
        TR_ASSERT(NULL != p_stack->impl);
        TR_ASSERT(NULL == p_data->top);
        TR_ASSERT(0u == p_data->stack_size);
        TR_ASSERT(p_data->size_of_datatype == size_of_datatype);

        *id_of_stack = p_stack;

        return (TR_OK);
}
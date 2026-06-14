/*******************************************************************************************************
 * NAME: stack_shared.h
 *
 * PURPOSE: Internal shared definitions for the stack implementations
 *          Not part of the public API - do not include outside of src/stack/
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
#ifndef STACK_SHARED_H
#define STACK_SHARED_H

/* 0 copyright/licensing */
/*******************************************************************************************************
 *
 * This is free and unencumbered software released into the public domain (Unlicense).
 *
 ********************************************************************************************************/

/* 1 includes */
/*****************************************************/
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
 * TYPE NAME: struct tr_stack_ops
 *
 * PURPOSE: Dispatch table for stack operations
 *          Function pointers are set at creation time based on implementation type
 *          All implementation files must use this exact definition
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

/*******************************************************************************************************
 *
 * TYPE NAME: struct stack
 *
 * PURPOSE: Internal definition of the opaque stack handle
 *          Users only see the forward declaration in tr_stack.h
 *
 * MEMBERS:
 *
 * MEMBER               TYPE                    DESCRIPTION
 * ------               ----                    -----------
 * ops                  struct tr_stack_ops *      Pointer to the dispatch table
 * impl                 void *                  Pointer to implementation specific data
 *
 *******************************************************************************************************/
struct stack
{
        const struct tr_stack_ops *ops;
        void *impl;
};

/*****************************************************/

/* 5 global variable declarations */
/*****************************************************/
/*****************************************************/

/* 6 function prototypes */
/*****************************************************/

/* forward declarations of implementation create functions */
/* called by tr_stack_create in stack.c */
tr_result_t tr_stack_array_create(size_t size_of_datatype,
                                  size_t elements_to_allocate,
                                  tr_stack_type_t stack_type,
                                  struct stack **id_of_stack);

tr_result_t tr_stack_ll_create(size_t size_of_datatype,
                               size_t elements_to_allocate,
                               struct stack **id_of_stack);
/*****************************************************/

#endif /* STACK_SHARED_H */
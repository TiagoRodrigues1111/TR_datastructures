/*******************************************************************************************************
 * NAME: queue_shared.h
 *
 * PURPOSE: Internal shared definitions for the queue implementations
 *          Not part of the public API - do not include outside of src/queue/
 *
 * GLOBAL VARIABLES: None
 *
 * DEVELOPMENT HISTORY:
 *
 * Date          Author          Change Id       Release         Description Of Change
 * ----------    --------------- ---------       -------         -----------------------------------
 * 14-06-2026    Tiago Rodrigues                       1         File preparation
 *
 *******************************************************************************************************/
#ifndef QUEUE_SHARED_H
#define QUEUE_SHARED_H

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
 * TYPE NAME: struct tr_queue_ops
 *
 * PURPOSE: Dispatch table for queue operations
 *          Function pointers are set at creation time based on implementation type
 *          All implementation files must use this exact definition
 *
 * MEMBERS:
 *
 * MEMBER               TYPE                                             DESCRIPTION
 * ------               ----                                             -----------
 * enqueue              tr_result_t (*)(struct queue *, const void *)    Enqueue operation
 * dequeue              tr_result_t (*)(struct queue *)                  Dequeue operation
 * front                tr_result_t (*)(const struct queue *, void *)    Front operation
 * size                 tr_result_t (*)(const struct queue *, size_t *)  Size operation
 * is_empty             tr_result_t (*)(const struct queue *, bool *)    Is empty operation
 * capacity             tr_result_t (*)(const struct queue *, size_t *)  Capacity operation
 * destroy              tr_result_t (*)(struct queue **)                 Destroy operation
 *
 *******************************************************************************************************/
struct tr_queue_ops
{
        tr_result_t (*enqueue)(struct queue *id_of_queue, const void *data_to_enqueue);
        tr_result_t (*dequeue)(struct queue *id_of_queue);
        tr_result_t (*front)(const struct queue *id_of_queue, void *data_at_front);
        tr_result_t (*size)(const struct queue *id_of_queue, size_t *size);
        tr_result_t (*is_empty)(const struct queue *id_of_queue, bool *is_empty);
        tr_result_t (*capacity)(const struct queue *id_of_queue, size_t *capacity);
        tr_result_t (*destroy)(struct queue **id_of_queue);
};

/*******************************************************************************************************
 *
 * TYPE NAME: struct queue
 *
 * PURPOSE: Internal definition of the opaque queue handle
 *          Users only see the forward declaration in tr_queue.h
 *
 * MEMBERS:
 *
 * MEMBER               TYPE                     DESCRIPTION
 * ------               ----                     -----------
 * ops                  struct tr_queue_ops *    Pointer to the dispatch table
 * impl                 void *                   Pointer to implementation specific data
 *
 *******************************************************************************************************/
struct queue
{
        const struct tr_queue_ops *ops;
        void *impl;
};

/*****************************************************/

/* 5 global variable declarations */
/*****************************************************/
/*****************************************************/

/* 6 function prototypes */
/*****************************************************/

/* forward declarations of implementation create functions */
/* called by tr_queue_create in queue.c */
tr_result_t tr_queue_array_create(size_t size_of_datatype,
                                  size_t elements_to_allocate,
                                  tr_queue_type_t queue_type,
                                  struct queue **id_of_queue);

tr_result_t tr_queue_ll_create(size_t size_of_datatype,
                               size_t elements_to_allocate,
                               struct queue **id_of_queue);

/*****************************************************/

#endif /* QUEUE_SHARED_H */
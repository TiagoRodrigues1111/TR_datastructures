/*******************************************************************************************************
* NAME: queue.c
*
* PURPOSE: Public API implementation for the queue data structure
*          Dispatches to the appropriate implementation via function pointers
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

/* 0 copyright/licensing */
/*******************************************************************************************************
*
* This is free and unencumbered software released into the public domain (Unlicense).
*
********************************************************************************************************/


/* 1 includes */
/*****************************************************/

/* 1.1 Related header */
#include "tr_datastructures/tr_queue.h"

/* 1.2 C system headers */
#include <stdio.h>      /* fprintf — used by TR_ASSERT */
#include <stdlib.h>     /* abort   — used by TR_ASSERT */

/* 1.3 C++ standard library headers */

/* 1.4 other libraries' headers */

/* 1.5 project's headers */
#include "tr_internal.h"
#include "queue_shared.h"

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
* FUNCTION NAME: tr_queue_create
*
* PURPOSE: Allocates and initialises a new queue instance
*          Delegates to the appropriate implementation based on queue_type
*
* ARGUMENTS:
*
* ARGUMENT                TYPE             I/O     DESCRIPTION
* --------                ----             ---     ------------
* size_of_datatype        size_t           I       Byte size of the datatype to store
* elements_to_allocate    size_t           I       Initial number of elements to allocate
* queue_type              tr_queue_type_t  I       Implementation type to use
* id_of_queue             struct queue **  O       Pointer to pointer to receive the created queue
*
* RETURNS: tr_result_t
*   TR_OK               - Queue created successfully
*   TR_ERR_NULL         - id_of_queue is NULL
*   TR_ERR_INVALID      - size_of_datatype or elements_to_allocate is 0, or unknown queue_type
*   TR_ERR_ALLOC        - Memory allocation failed
*
*******************************************************************************************************/
tr_result_t tr_queue_create(size_t           size_of_datatype,
                             size_t           elements_to_allocate,
                             tr_queue_type_t  queue_type,
                             struct queue   **id_of_queue)
{
    /* local variables */
    /* none */

    if (NULL == id_of_queue)
    {
        return (TR_ERR_NULL);
    }

    if ((0u == size_of_datatype) || (0u == elements_to_allocate))
    {
        return (TR_ERR_INVALID);
    }

    if (TR_QUEUE_ARRAY_DYNAMIC == queue_type || TR_QUEUE_ARRAY_FIXED == queue_type)
    {
        return (tr_queue_array_create(size_of_datatype,
                                      elements_to_allocate,
                                      queue_type,
                                      id_of_queue));
    }
    else if (TR_QUEUE_LL == queue_type)
    {
        return (tr_queue_ll_create(size_of_datatype,
                                   elements_to_allocate,
                                   id_of_queue));
    }

    return (TR_ERR_INVALID);
}


/*******************************************************************************************************
*
* FUNCTION NAME: tr_queue_destroy
*
* PURPOSE: Frees all memory associated with the queue instance
*
* ARGUMENTS:
*
* ARGUMENT             TYPE                I/O     DESCRIPTION
* --------             ----                ---     ------------
* id_of_queue          struct queue **     I/O     Pointer to pointer to the queue to destroy
*
* RETURNS: tr_result_t
*   TR_OK               - Queue destroyed successfully
*   TR_ERR_NULL         - id_of_queue or *id_of_queue is NULL
*
*******************************************************************************************************/
tr_result_t tr_queue_destroy(struct queue **id_of_queue)
{
    /* local variables */
    /* none */

    if (NULL == id_of_queue || NULL == *id_of_queue)
    {
        return (TR_ERR_NULL);
    }

    TR_ASSERT(NULL != (*id_of_queue)->ops);
    TR_ASSERT(NULL != (*id_of_queue)->ops->destroy);

    return ((*id_of_queue)->ops->destroy(id_of_queue));
}


/*******************************************************************************************************
*
* FUNCTION NAME: tr_queue_enqueue
*
* PURPOSE: Adds a deep copy of the data to the back of the queue
*
* ARGUMENTS:
*
* ARGUMENT             TYPE                I/O     DESCRIPTION
* --------             ----                ---     ------------
* id_of_queue          struct queue *      I/O     Pointer to the queue
* data_to_enqueue      const void *        I       Pointer to the data to copy
*
* RETURNS: tr_result_t
*   TR_OK               - Data enqueued successfully
*   TR_ERR_NULL         - id_of_queue or data_to_enqueue is NULL
*   TR_ERR_ALLOC        - Memory allocation failed (linked list only)
*   TR_ERR_FULL         - Queue is full (fixed only)
*
*******************************************************************************************************/
tr_result_t tr_queue_enqueue(struct queue *id_of_queue, const void *data_to_enqueue)
{
    /* local variables */
    /* none */

    if (NULL == id_of_queue || NULL == data_to_enqueue)
    {
        return (TR_ERR_NULL);
    }

    TR_ASSERT(NULL != id_of_queue->ops);
    TR_ASSERT(NULL != id_of_queue->ops->enqueue);

    return (id_of_queue->ops->enqueue(id_of_queue, data_to_enqueue));
}


/*******************************************************************************************************
*
* FUNCTION NAME: tr_queue_dequeue
*
* PURPOSE: Removes the element at the front of the queue
*
* ARGUMENTS:
*
* ARGUMENT             TYPE                I/O     DESCRIPTION
* --------             ----                ---     ------------
* id_of_queue          struct queue *      I/O     Pointer to the queue
*
* RETURNS: tr_result_t
*   TR_OK               - Element removed successfully
*   TR_ERR_NULL         - id_of_queue is NULL
*   TR_ERR_EMPTY        - Queue is empty
*
*******************************************************************************************************/
tr_result_t tr_queue_dequeue(struct queue *id_of_queue)
{
    /* local variables */
    /* none */

    if (NULL == id_of_queue)
    {
        return (TR_ERR_NULL);
    }

    TR_ASSERT(NULL != id_of_queue->ops);
    TR_ASSERT(NULL != id_of_queue->ops->dequeue);

    return (id_of_queue->ops->dequeue(id_of_queue));
}


/*******************************************************************************************************
*
* FUNCTION NAME: tr_queue_front
*
* PURPOSE: Copies the element at the front of the queue into the provided buffer
*
* ARGUMENTS:
*
* ARGUMENT             TYPE                    I/O     DESCRIPTION
* --------             ----                    ---     ------------
* id_of_queue          const struct queue *    I       Pointer to the queue
* data_at_front        void *                  O       Buffer to copy the front element into
*
* RETURNS: tr_result_t
*   TR_OK               - Data copied successfully
*   TR_ERR_NULL         - id_of_queue or data_at_front is NULL
*   TR_ERR_EMPTY        - Queue is empty
*
*******************************************************************************************************/
tr_result_t tr_queue_front(const struct queue *id_of_queue, void *data_at_front)
{
    /* local variables */
    /* none */

    if (NULL == id_of_queue || NULL == data_at_front)
    {
        return (TR_ERR_NULL);
    }

    TR_ASSERT(NULL != id_of_queue->ops);
    TR_ASSERT(NULL != id_of_queue->ops->front);

    return (id_of_queue->ops->front(id_of_queue, data_at_front));
}


/*******************************************************************************************************
*
* FUNCTION NAME: tr_queue_size
*
* PURPOSE: Returns the current number of elements in the queue
*
* ARGUMENTS:
*
* ARGUMENT             TYPE                    I/O     DESCRIPTION
* --------             ----                    ---     ------------
* id_of_queue          const struct queue *    I       Pointer to the queue
* size                 size_t *                O       Pointer to receive the current element count
*
* RETURNS: tr_result_t
*   TR_OK               - Size retrieved successfully
*   TR_ERR_NULL         - id_of_queue or size is NULL
*
*******************************************************************************************************/
tr_result_t tr_queue_size(const struct queue *id_of_queue, size_t *size)
{
    /* local variables */
    /* none */

    if (NULL == id_of_queue || NULL == size)
    {
        return (TR_ERR_NULL);
    }

    TR_ASSERT(NULL != id_of_queue->ops);
    TR_ASSERT(NULL != id_of_queue->ops->size);

    return (id_of_queue->ops->size(id_of_queue, size));
}


/*******************************************************************************************************
*
* FUNCTION NAME: tr_queue_is_empty
*
* PURPOSE: Checks whether the queue contains no elements
*
* ARGUMENTS:
*
* ARGUMENT             TYPE                    I/O     DESCRIPTION
* --------             ----                    ---     ------------
* id_of_queue          const struct queue *    I       Pointer to the queue
* is_empty             bool *                  O       Set to true if empty, false otherwise
*
* RETURNS: tr_result_t
*   TR_OK               - Check completed successfully
*   TR_ERR_NULL         - id_of_queue or is_empty is NULL
*
*******************************************************************************************************/
tr_result_t tr_queue_is_empty(const struct queue *id_of_queue, bool *is_empty)
{
    /* local variables */
    /* none */

    if (NULL == id_of_queue || NULL == is_empty)
    {
        return (TR_ERR_NULL);
    }

    TR_ASSERT(NULL != id_of_queue->ops);
    TR_ASSERT(NULL != id_of_queue->ops->is_empty);

    return (id_of_queue->ops->is_empty(id_of_queue, is_empty));
}


/*******************************************************************************************************
*
* FUNCTION NAME: tr_queue_capacity
*
* PURPOSE: Returns the total allocated capacity of the queue
*
* ARGUMENTS:
*
* ARGUMENT             TYPE                    I/O     DESCRIPTION
* --------             ----                    ---     ------------
* id_of_queue          const struct queue *    I       Pointer to the queue
* capacity             size_t *                O       Pointer to receive the capacity
*
* RETURNS: tr_result_t
*   TR_OK               - Capacity retrieved successfully
*   TR_ERR_NULL         - id_of_queue or capacity is NULL
*
*******************************************************************************************************/
tr_result_t tr_queue_capacity(const struct queue *id_of_queue, size_t *capacity)
{
    /* local variables */
    /* none */

    if (NULL == id_of_queue || NULL == capacity)
    {
        return (TR_ERR_NULL);
    }

    TR_ASSERT(NULL != id_of_queue->ops);
    TR_ASSERT(NULL != id_of_queue->ops->capacity);

    return (id_of_queue->ops->capacity(id_of_queue, capacity));
}
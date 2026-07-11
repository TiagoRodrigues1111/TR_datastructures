/*******************************************************************************************************
* NAME: queue_ll.c
*
* PURPOSE: Linked list based implementation of the queue data structure
*          Uses a singly linked list with head and tail pointers for O(1)
*          enqueue and dequeue operations
*
* GLOBAL VARIABLES: None
*
* DEVELOPMENT HISTORY:
*
* Date          Author          Change Id       Release         Description Of Change
* ----------    --------------- ---------       -------         -----------------------------------
* 04-07-2026    Tiago Rodrigues                       1         File preparation
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
#include <stdlib.h>     /* malloc, free             */
#include <string.h>     /* memcpy, memset           */
#include <stdio.h>      /* fprintf, stderr          */

#include "tr_datastructures/tr_queue.h"
#include "queue_shared.h"
#include "tr_internal.h"
/*****************************************************/


/* 2 defines */
/*****************************************************/

/*
 * QUEUE_LL_NODE_SIZE - computes the total allocation size for a node
 * On C99 and later the flexible array member data[] has zero size
 * On C89 data[1] contributes 1 byte to sizeof so we subtract it
 */
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
#define QUEUE_LL_NODE_SIZE(datatype_size) \
    (sizeof(struct tr_queue_ll_node) + (datatype_size))
#else
#define QUEUE_LL_NODE_SIZE(datatype_size) \
    (sizeof(struct tr_queue_ll_node) - 1u + (datatype_size))
#endif

/*****************************************************/


/* 3 external declarations */
/*****************************************************/
/*****************************************************/


/* 4 typedefs */
/*****************************************************/

/*******************************************************************************************************
*
* TYPE NAME: struct tr_queue_ll_node
*
* PURPOSE: A single node in the linked list queue
*          Data is stored inline at the end of the node using a flexible
*          array member on C99 or later, or a single element array on C89
*
* MEMBERS:
*
* MEMBER               TYPE                     DESCRIPTION
* ------               ----                     -----------
* next                 struct tr_queue_ll_node * Pointer to the next node toward the tail
* data                 uint8_t[]                Inline data storage
*
*******************************************************************************************************/
struct tr_queue_ll_node
{
    struct tr_queue_ll_node *next;

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
    /* C99 or later - flexible array member */
    uint8_t data[];
#else
    /* C89 fallback - single element array trick
     * allocation size adjusted by QUEUE_LL_NODE_SIZE macro */
    uint8_t data[1];
#endif
};


/*******************************************************************************************************
*
* TYPE NAME: struct tr_queue_ll_data
*
* PURPOSE: Internal data for the linked list queue implementation
*
* MEMBERS:
*
* MEMBER               TYPE                     DESCRIPTION
* ------               ----                     -----------
* head                 struct tr_queue_ll_node * Pointer to the front node (dequeue from here)
* tail                 struct tr_queue_ll_node * Pointer to the back node (enqueue here)
* queue_size           size_t                   Current number of elements
* size_of_datatype     size_t                   Byte size of each element
*
*******************************************************************************************************/
struct tr_queue_ll_data
{
    struct tr_queue_ll_node *head;
    struct tr_queue_ll_node *tail;
    size_t                   queue_size;
    size_t                   size_of_datatype;
};

/*****************************************************/


/* 5 global variable declarations */
/*****************************************************/
/*****************************************************/


/* 6 function prototypes */
/*****************************************************/
static tr_result_t queue_ll_enqueue  (struct queue *id_of_queue, const void *data_to_enqueue);
static tr_result_t queue_ll_dequeue  (struct queue *id_of_queue);
static tr_result_t queue_ll_front    (const struct queue *id_of_queue, void *data_at_front);
static tr_result_t queue_ll_size     (const struct queue *id_of_queue, size_t *size);
static tr_result_t queue_ll_is_empty (const struct queue *id_of_queue, bool *is_empty);
static tr_result_t queue_ll_capacity (const struct queue *id_of_queue, size_t *capacity);
static tr_result_t queue_ll_destroy  (struct queue **id_of_queue);
/*****************************************************/


/*******************************************************************************************************
*
* FUNCTION NAME: queue_ll_enqueue
*
* PURPOSE: Adds a deep copy of the data to the back of the linked list queue
*          Allocates a new node with inline data storage
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
*   TR_ERR_ALLOC        - Memory allocation failed
*
*******************************************************************************************************/
static tr_result_t queue_ll_enqueue(struct queue *id_of_queue, const void *data_to_enqueue)
{
    /* local variables */
    struct tr_queue_ll_data *p_data = NULL;     /* pointer to implementation data   */
    struct tr_queue_ll_node *p_node = NULL;     /* pointer to new node              */

    TR_ASSERT(NULL != id_of_queue);
    TR_ASSERT(NULL != id_of_queue->impl);

    p_data = (struct tr_queue_ll_data *)id_of_queue->impl;

    TR_ASSERT(NULL != p_data);
    TR_ASSERT(p_data->size_of_datatype > 0u);
    TR_ASSERT((0u == p_data->queue_size) || (NULL != p_data->head));
    TR_ASSERT((0u == p_data->queue_size) || (NULL != p_data->tail));
    TR_ASSERT((0u != p_data->queue_size) || (NULL == p_data->head));
    TR_ASSERT((0u != p_data->queue_size) || (NULL == p_data->tail));

    /* allocate node with inline data */
    p_node = (struct tr_queue_ll_node *)malloc(
        QUEUE_LL_NODE_SIZE(p_data->size_of_datatype));

    if (NULL == p_node)
    {
        /* LCOV_EXCL_START */
        fprintf(stderr, "[TR] queue_ll_enqueue: malloc failed\n");
        return (TR_ERR_ALLOC);
        /* LCOV_EXCL_STOP */
    }

    /* copy data inline into node */
    memcpy(p_node->data, data_to_enqueue, p_data->size_of_datatype);
    p_node->next = NULL;

    /* attach node to tail */
    if (NULL == p_data->tail)
    {
        /* queue was empty - node is both head and tail */
        p_data->head = p_node;
        p_data->tail = p_node;
    }
    else
    {
        /* attach to existing tail */
        p_data->tail->next = p_node;
        p_data->tail       = p_node;
    }

    p_data->queue_size++;

    /* post conditions */
    TR_ASSERT(NULL != p_data->head);
    TR_ASSERT(NULL != p_data->tail);
    TR_ASSERT(p_data->queue_size > 0u);

    return (TR_OK);
}


/*******************************************************************************************************
*
* FUNCTION NAME: queue_ll_dequeue
*
* PURPOSE: Removes the node at the front of the linked list queue and frees it
*
* ARGUMENTS:
*
* ARGUMENT             TYPE                I/O     DESCRIPTION
* --------             ----                ---     ------------
* id_of_queue          struct queue *      I/O     Pointer to the queue
*
* RETURNS: tr_result_t
*   TR_OK               - Element removed successfully
*   TR_ERR_EMPTY        - Queue is empty
*
*******************************************************************************************************/
static tr_result_t queue_ll_dequeue(struct queue *id_of_queue)
{
    /* local variables */
    struct tr_queue_ll_data *p_data    = NULL;  /* pointer to implementation data   */
    struct tr_queue_ll_node *p_old_head = NULL; /* pointer to node being removed    */

    TR_ASSERT(NULL != id_of_queue);
    TR_ASSERT(NULL != id_of_queue->impl);

    p_data = (struct tr_queue_ll_data *)id_of_queue->impl;

    TR_ASSERT(NULL != p_data);
    TR_ASSERT((0u == p_data->queue_size) || (NULL != p_data->head));
    TR_ASSERT((0u == p_data->queue_size) || (NULL != p_data->tail));
    TR_ASSERT((0u != p_data->queue_size) || (NULL == p_data->head));
    TR_ASSERT((0u != p_data->queue_size) || (NULL == p_data->tail));

    if (0u == p_data->queue_size)
    {
        return (TR_ERR_EMPTY);
    }

    /* unlink head node */
    p_old_head   = p_data->head;
    p_data->head = p_old_head->next;

    /* if queue is now empty, tail must also be NULL */
    if (NULL == p_data->head)
    {
        p_data->tail = NULL;
    }

    /* zero and free the node */
    memset(p_old_head, 0, QUEUE_LL_NODE_SIZE(p_data->size_of_datatype));
    free(p_old_head);

    p_data->queue_size--;

    /* post conditions */
    TR_ASSERT((0u == p_data->queue_size) || (NULL != p_data->head));
    TR_ASSERT((0u != p_data->queue_size) || (NULL == p_data->head));
    TR_ASSERT((0u != p_data->queue_size) || (NULL == p_data->tail));

    return (TR_OK);
}


/*******************************************************************************************************
*
* FUNCTION NAME: queue_ll_front
*
* PURPOSE: Copies the data from the front node into the provided buffer
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
*   TR_ERR_EMPTY        - Queue is empty
*
*******************************************************************************************************/
static tr_result_t queue_ll_front(const struct queue *id_of_queue, void *data_at_front)
{
    /* local variables */
    const struct tr_queue_ll_data *p_data = NULL; /* pointer to implementation data */

    TR_ASSERT(NULL != id_of_queue);
    TR_ASSERT(NULL != id_of_queue->impl);

    p_data = (const struct tr_queue_ll_data *)id_of_queue->impl;

    TR_ASSERT(NULL != p_data);
    TR_ASSERT(p_data->size_of_datatype > 0u);
    TR_ASSERT((0u == p_data->queue_size) || (NULL != p_data->head));
    TR_ASSERT((0u == p_data->queue_size) || (NULL != p_data->tail));
    TR_ASSERT((0u != p_data->queue_size) || (NULL == p_data->head));
    TR_ASSERT((0u != p_data->queue_size) || (NULL == p_data->tail));

    if (0u == p_data->queue_size)
    {
        return (TR_ERR_EMPTY);
    }

    memcpy(data_at_front, p_data->head->data, p_data->size_of_datatype);

    return (TR_OK);
}


/*******************************************************************************************************
*
* FUNCTION NAME: queue_ll_size
*
* PURPOSE: Returns the current number of elements in the linked list queue
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
*
*******************************************************************************************************/
static tr_result_t queue_ll_size(const struct queue *id_of_queue, size_t *size)
{
    /* local variables */
    const struct tr_queue_ll_data *p_data = NULL; /* pointer to implementation data */

    TR_ASSERT(NULL != id_of_queue);
    TR_ASSERT(NULL != id_of_queue->impl);

    p_data = (const struct tr_queue_ll_data *)id_of_queue->impl;

    TR_ASSERT(NULL != p_data);

    *size = p_data->queue_size;

    return (TR_OK);
}


/*******************************************************************************************************
*
* FUNCTION NAME: queue_ll_is_empty
*
* PURPOSE: Checks whether the linked list queue contains no elements
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
*
*******************************************************************************************************/
static tr_result_t queue_ll_is_empty(const struct queue *id_of_queue, bool *is_empty)
{
    /* local variables */
    const struct tr_queue_ll_data *p_data = NULL; /* pointer to implementation data */

    TR_ASSERT(NULL != id_of_queue);
    TR_ASSERT(NULL != id_of_queue->impl);

    p_data    = (const struct tr_queue_ll_data *)id_of_queue->impl;

    TR_ASSERT(NULL != p_data);
    TR_ASSERT((0u == p_data->queue_size) || (NULL != p_data->head));
    TR_ASSERT((0u != p_data->queue_size) || (NULL == p_data->head));

    *is_empty = (0u == p_data->queue_size);

    return (TR_OK);
}


/*******************************************************************************************************
*
* FUNCTION NAME: queue_ll_capacity
*
* PURPOSE: Returns the capacity of the linked list queue
*          For linked list queues capacity always equals current size
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
*
*******************************************************************************************************/
static tr_result_t queue_ll_capacity(const struct queue *id_of_queue, size_t *capacity)
{
    /* local variables */
    const struct tr_queue_ll_data *p_data = NULL; /* pointer to implementation data */

    TR_ASSERT(NULL != id_of_queue);
    TR_ASSERT(NULL != id_of_queue->impl);

    p_data    = (const struct tr_queue_ll_data *)id_of_queue->impl;

    TR_ASSERT(NULL != p_data);

    *capacity = p_data->queue_size;

    return (TR_OK);
}


/*******************************************************************************************************
*
* FUNCTION NAME: queue_ll_destroy
*
* PURPOSE: Frees all nodes and implementation data for the linked list queue
*
* ARGUMENTS:
*
* ARGUMENT             TYPE                I/O     DESCRIPTION
* --------             ----                ---     ------------
* id_of_queue          struct queue **     I/O     Pointer to pointer to the queue to destroy
*
* RETURNS: tr_result_t
*   TR_OK               - Queue destroyed successfully
*
*******************************************************************************************************/
static tr_result_t queue_ll_destroy(struct queue **id_of_queue)
{
    /* local variables */
    struct tr_queue_ll_data *p_data    = NULL;  /* pointer to implementation data   */
    struct tr_queue_ll_node *p_current = NULL;  /* pointer to current node          */
    struct tr_queue_ll_node *p_next    = NULL;  /* pointer to next node             */

    TR_ASSERT(NULL != id_of_queue);
    TR_ASSERT(NULL != *id_of_queue);

    p_data = (struct tr_queue_ll_data *)(*id_of_queue)->impl;

    TR_ASSERT(NULL != p_data);

    /* free all nodes */
    p_current    = p_data->head;
    p_data->head = NULL;
    p_data->tail = NULL;

    while (NULL != p_current)
    {
        p_next = p_current->next;
        memset(p_current, 0, QUEUE_LL_NODE_SIZE(p_data->size_of_datatype));
        free(p_current);
        p_current = p_next;
    }

    TR_ASSERT(NULL == p_data->head);
    TR_ASSERT(NULL == p_data->tail);

    /* zero and free implementation data */
    p_data->queue_size       = 0u;
    p_data->size_of_datatype = 0u;

    free(p_data);
    free(*id_of_queue);
    *id_of_queue = NULL;

    TR_ASSERT(NULL == *id_of_queue);

    return (TR_OK);
}


/*******************************************************************************************************
*
* Static dispatch table for linked list queue
*
*******************************************************************************************************/
static const struct tr_queue_ops k_queue_ll_ops =
{
    queue_ll_enqueue,
    queue_ll_dequeue,
    queue_ll_front,
    queue_ll_size,
    queue_ll_is_empty,
    queue_ll_capacity,
    queue_ll_destroy
};


/*******************************************************************************************************
*
* FUNCTION NAME: tr_queue_ll_create
*
* PURPOSE: Allocates and initialises a new linked list queue instance
*          Called by tr_queue_create for TR_QUEUE_LL
*
* ARGUMENTS:
*
* ARGUMENT                TYPE             I/O     DESCRIPTION
* --------                ----             ---     ------------
* size_of_datatype        size_t           I       Byte size of the datatype to store
* elements_to_allocate    size_t           I       Ignored - nodes allocated on demand
* id_of_queue             struct queue **  O       Pointer to pointer to receive the created queue
*
* RETURNS: tr_result_t
*   TR_OK               - Queue created successfully
*   TR_ERR_ALLOC        - Memory allocation failed
*
*******************************************************************************************************/
tr_result_t tr_queue_ll_create(size_t           size_of_datatype,
                                size_t           elements_to_allocate,
                                struct queue   **id_of_queue)
{
    /* local variables */
    struct queue            *p_queue = NULL;    /* pointer to new queue handle      */
    struct tr_queue_ll_data *p_data  = NULL;    /* pointer to implementation data   */

    TR_UNUSED(elements_to_allocate);            /* not used for linked list         */

    /* allocate queue handle */
    p_queue = (struct queue *)malloc(sizeof(struct queue));
    if (NULL == p_queue)
    {
        /* LCOV_EXCL_START */
        fprintf(stderr, "[TR] tr_queue_ll_create: malloc failed for queue handle\n");
        return (TR_ERR_ALLOC);
        /* LCOV_EXCL_STOP */
    }

    /* allocate implementation data */
    p_data = (struct tr_queue_ll_data *)malloc(sizeof(struct tr_queue_ll_data));
    if (NULL == p_data)
    {
        /* LCOV_EXCL_START */
        fprintf(stderr, "[TR] tr_queue_ll_create: malloc failed for implementation data\n");
        free(p_queue);
        return (TR_ERR_ALLOC);
        /* LCOV_EXCL_STOP */
    }

    /* initialise implementation data */
    p_data->head             = NULL;
    p_data->tail             = NULL;
    p_data->queue_size       = 0u;
    p_data->size_of_datatype = size_of_datatype;

    /* wire up dispatch table and implementation data */
    p_queue->ops  = &k_queue_ll_ops;
    p_queue->impl = p_data;

    *id_of_queue = p_queue;

    TR_ASSERT(NULL != p_queue->ops);
    TR_ASSERT(NULL != p_queue->impl);
    TR_ASSERT(NULL == p_data->head);
    TR_ASSERT(NULL == p_data->tail);
    TR_ASSERT(0u == p_data->queue_size);
    TR_ASSERT(p_data->size_of_datatype == size_of_datatype);

    return (TR_OK);
}
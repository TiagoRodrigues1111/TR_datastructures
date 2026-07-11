/*******************************************************************************************************
 * NAME: queue_array.c
 *
 * PURPOSE: Array based implementation of the queue data structure
 *          Uses a circular buffer for O(1) enqueue and dequeue operations
 *          Supports both dynamic (auto-growing) and fixed capacity variants
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
#include <stdlib.h> /* malloc, realloc, free, abort    */
#include <string.h> /* memcpy                          */
#include <stdio.h>  /* fprintf, stderr                 */

/* 1.3 C++ standard library headers*/

/* 1.4 other libraries' headers*/

/* 1.5 project's headers */
#include "tr_internal.h"
#include "queue_shared.h"

/*****************************************************/

/* 2 defines */
/*****************************************************/
#define GROWTH_FACTOR 2u

TR_INTERNAL_STATIC_ASSERT(GROWTH_FACTOR >= 2u, growth_factor_must_be_at_least_2);

/*****************************************************/

/* 3 external declarations */
/*****************************************************/
/*****************************************************/

/* 4 typedefs */
/*****************************************************/

/*******************************************************************************************************
 *
 * TYPE NAME: struct tr_queue_array_data
 *
 * PURPOSE: Internal data for the array based queue implementation
 *          Uses a circular buffer for O(1) enqueue and dequeue
 *
 * MEMBERS:
 *
 * MEMBER               TYPE        DESCRIPTION
 * ------               ----        -----------
 * queue_data           void *      Pointer to the allocated circular buffer
 * head                 size_t      Index of the front element (dequeue from here)
 * tail                 size_t      Index of the next free slot (enqueue here)
 * queue_size           size_t      Current number of elements in the queue
 * capacity             size_t      Total number of elements allocated
 * size_of_datatype     size_t      Byte size of each element
 *
 *******************************************************************************************************/
struct tr_queue_array_data
{
        void *queue_data;
        size_t head;
        size_t tail;
        size_t queue_size;
        size_t capacity;
        size_t size_of_datatype;
};

/*****************************************************/

/* 5 global variable declarations */
/*****************************************************/
/*****************************************************/

/* 6 function prototypes */
/*****************************************************/
static tr_result_t queue_array_dynamic_enqueue(struct queue *id_of_queue,
                                               const void *data_to_enqueue);
static tr_result_t queue_array_fixed_enqueue(struct queue *id_of_queue,
                                             const void *data_to_enqueue);
static tr_result_t queue_array_dequeue(struct queue *id_of_queue);
static tr_result_t queue_array_front(const struct queue *id_of_queue, void *data_at_front);
static tr_result_t queue_array_size(const struct queue *id_of_queue, size_t *size);
static tr_result_t queue_array_is_empty(const struct queue *id_of_queue, bool *is_empty);
static tr_result_t queue_array_capacity(const struct queue *id_of_queue, size_t *capacity);
static tr_result_t queue_array_destroy(struct queue **id_of_queue);

/*****************************************************/

/*******************************************************************************************************
 *
 * FUNCTION NAME: queue_array_dynamic_enqueue
 *
 * PURPOSE: Adds data to the back of the dynamic array queue
 *          Grows the circular buffer by GROWTH_FACTOR if full
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
 *   TR_ERR_ALLOC        - Memory reallocation failed
 *
 *******************************************************************************************************/
static tr_result_t queue_array_dynamic_enqueue(struct queue *id_of_queue,
                                               const void *data_to_enqueue)
{
        /* local variables */
        struct tr_queue_array_data *p_data = NULL; /* pointer to implementation data   */
        void *p_new_data = NULL;                   /* pointer for realloc              */
        size_t new_cap = 0u;                       /* new capacity after growth        */
        size_t i = 0u;                             /* loop counter for reorganisation  */

        TR_ASSERT(NULL != id_of_queue);
        TR_ASSERT(NULL != id_of_queue->impl);

        p_data = (struct tr_queue_array_data *) id_of_queue->impl;

        TR_ASSERT(NULL != p_data);
        TR_ASSERT(NULL != p_data->queue_data);
        TR_ASSERT(p_data->size_of_datatype > 0u);
        TR_ASSERT(p_data->capacity > 0u);
        TR_ASSERT(p_data->queue_size <= p_data->capacity);

        /* grow circular buffer if full */
        if (p_data->queue_size == p_data->capacity)
        {
                new_cap = p_data->capacity * GROWTH_FACTOR;
                p_new_data = malloc(new_cap * p_data->size_of_datatype);

                if (NULL == p_new_data)
                {
                        /* LCOV_EXCL_START */
                        fprintf(stderr, "[TR] queue_array_dynamic_enqueue: malloc failed\n");
                        return (TR_ERR_ALLOC);
                        /* LCOV_EXCL_STOP */
                }

                /* reorganise elements into new linear buffer starting from index 0 */
                for (i = 0u; i < p_data->queue_size; i++)
                {
                        memcpy((uint8_t *) p_new_data + (i * p_data->size_of_datatype),
                               (uint8_t *) p_data->queue_data +
                                       (((p_data->head + i) % p_data->capacity) *
                                        p_data->size_of_datatype),
                               p_data->size_of_datatype);
                }

                free(p_data->queue_data);
                p_data->queue_data = p_new_data;
                p_data->head = 0u;
                p_data->tail = p_data->queue_size;
                p_data->capacity = new_cap;
        }

        /* copy data into tail slot */
        memcpy((uint8_t *) p_data->queue_data + (p_data->tail * p_data->size_of_datatype),
               data_to_enqueue,
               p_data->size_of_datatype);

        /* advance tail with wrap around */
        p_data->tail = (p_data->tail + 1u) % p_data->capacity;
        p_data->queue_size++;

        TR_ASSERT(p_data->queue_size <= p_data->capacity);

        return (TR_OK);
}

/*******************************************************************************************************
 *
 * FUNCTION NAME: queue_array_fixed_enqueue
 *
 * PURPOSE: Adds data to the back of the fixed array queue
 *          Returns TR_ERR_FULL if capacity is reached
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
 *   TR_ERR_FULL         - Queue is at capacity
 *
 *******************************************************************************************************/
static tr_result_t queue_array_fixed_enqueue(struct queue *id_of_queue, const void *data_to_enqueue)
{
        /* local variables */
        struct tr_queue_array_data *p_data = NULL; /* pointer to implementation data */

        TR_ASSERT(NULL != id_of_queue);
        TR_ASSERT(NULL != id_of_queue->impl);

        p_data = (struct tr_queue_array_data *) id_of_queue->impl;

        TR_ASSERT(NULL != p_data);
        TR_ASSERT(NULL != p_data->queue_data);
        TR_ASSERT(p_data->size_of_datatype > 0u);
        TR_ASSERT(p_data->capacity > 0u);
        TR_ASSERT(p_data->queue_size <= p_data->capacity);

        if (p_data->queue_size == p_data->capacity)
        {
                return (TR_ERR_FULL);
        }

        /* copy data into tail slot */
        memcpy((uint8_t *) p_data->queue_data + (p_data->tail * p_data->size_of_datatype),
               data_to_enqueue,
               p_data->size_of_datatype);

        /* advance tail with wrap around */
        p_data->tail = (p_data->tail + 1u) % p_data->capacity;
        p_data->queue_size++;

        TR_ASSERT(p_data->queue_size <= p_data->capacity);

        return (TR_OK);
}

/*******************************************************************************************************
 *
 * FUNCTION NAME: queue_array_dequeue
 *
 * PURPOSE: Removes the element at the front of the array queue
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
static tr_result_t queue_array_dequeue(struct queue *id_of_queue)
{
        /* local variables */
        struct tr_queue_array_data *p_data = NULL; /* pointer to implementation data */

        TR_ASSERT(NULL != id_of_queue);
        TR_ASSERT(NULL != id_of_queue->impl);

        p_data = (struct tr_queue_array_data *) id_of_queue->impl;

        TR_ASSERT(NULL != p_data);
        TR_ASSERT(NULL != p_data->queue_data);
        TR_ASSERT(p_data->queue_size <= p_data->capacity);

        if (0u == p_data->queue_size)
        {
                return (TR_ERR_EMPTY);
        }

        /* zero the slot being vacated */
        memset((uint8_t *) p_data->queue_data + (p_data->head * p_data->size_of_datatype),
               0,
               p_data->size_of_datatype);

        /* advance head with wrap around */
        p_data->head = (p_data->head + 1u) % p_data->capacity;
        p_data->queue_size--;

        return (TR_OK);
}

/*******************************************************************************************************
 *
 * FUNCTION NAME: queue_array_front
 *
 * PURPOSE: Copies the element at the front of the array queue into the provided buffer
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
static tr_result_t queue_array_front(const struct queue *id_of_queue, void *data_at_front)
{
        /* local variables */
        const struct tr_queue_array_data *p_data = NULL; /* pointer to implementation data */

        TR_ASSERT(NULL != id_of_queue);
        TR_ASSERT(NULL != id_of_queue->impl);

        p_data = (const struct tr_queue_array_data *) id_of_queue->impl;

        TR_ASSERT(NULL != p_data);
        TR_ASSERT(NULL != p_data->queue_data);
        TR_ASSERT(p_data->size_of_datatype > 0u);
        TR_ASSERT(p_data->queue_size <= p_data->capacity);

        if (0u == p_data->queue_size)
        {
                return (TR_ERR_EMPTY);
        }

        /* copy front element — head points to oldest element */
        memcpy(data_at_front,
               (const uint8_t *) p_data->queue_data + (p_data->head * p_data->size_of_datatype),
               p_data->size_of_datatype);

        return (TR_OK);
}

/*******************************************************************************************************
 *
 * FUNCTION NAME: queue_array_size
 *
 * PURPOSE: Returns the current number of elements in the array queue
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
static tr_result_t queue_array_size(const struct queue *id_of_queue, size_t *size)
{
        /* local variables */
        const struct tr_queue_array_data *p_data = NULL; /* pointer to implementation data */

        TR_ASSERT(NULL != id_of_queue);
        TR_ASSERT(NULL != id_of_queue->impl);

        p_data = (const struct tr_queue_array_data *) id_of_queue->impl;

        TR_ASSERT(NULL != p_data);
        TR_ASSERT(p_data->queue_size <= p_data->capacity);

        *size = p_data->queue_size;

        return (TR_OK);
}

/*******************************************************************************************************
 *
 * FUNCTION NAME: queue_array_is_empty
 *
 * PURPOSE: Checks whether the array queue contains no elements
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
static tr_result_t queue_array_is_empty(const struct queue *id_of_queue, bool *is_empty)
{
        /* local variables */
        const struct tr_queue_array_data *p_data = NULL; /* pointer to implementation data */

        TR_ASSERT(NULL != id_of_queue);
        TR_ASSERT(NULL != id_of_queue->impl);

        p_data = (const struct tr_queue_array_data *) id_of_queue->impl;

        TR_ASSERT(NULL != p_data);
        TR_ASSERT(p_data->queue_size <= p_data->capacity);

        *is_empty = (0u == p_data->queue_size);

        return (TR_OK);
}

/*******************************************************************************************************
 *
 * FUNCTION NAME: queue_array_capacity
 *
 * PURPOSE: Returns the total allocated capacity of the array queue
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
static tr_result_t queue_array_capacity(const struct queue *id_of_queue, size_t *capacity)
{
        /* local variables */
        const struct tr_queue_array_data *p_data = NULL; /* pointer to implementation data */

        TR_ASSERT(NULL != id_of_queue);
        TR_ASSERT(NULL != id_of_queue->impl);

        p_data = (const struct tr_queue_array_data *) id_of_queue->impl;

        TR_ASSERT(NULL != p_data);
        TR_ASSERT(p_data->capacity > 0u);

        *capacity = p_data->capacity;

        return (TR_OK);
}

/*******************************************************************************************************
 *
 * FUNCTION NAME: queue_array_destroy
 *
 * PURPOSE: Frees all memory associated with the array queue instance
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
static tr_result_t queue_array_destroy(struct queue **id_of_queue)
{
        /* local variables */
        struct tr_queue_array_data *p_data = NULL; /* pointer to implementation data */

        TR_ASSERT(NULL != id_of_queue);
        TR_ASSERT(NULL != *id_of_queue);

        p_data = (struct tr_queue_array_data *) (*id_of_queue)->impl;

        TR_ASSERT(NULL != p_data);
        TR_ASSERT(NULL != p_data->queue_data);

        free(p_data->queue_data);
        p_data->queue_data = NULL;
        p_data->head = 0u;
        p_data->tail = 0u;
        p_data->queue_size = 0u;
        p_data->capacity = 0u;
        p_data->size_of_datatype = 0u;

        free(p_data);
        free(*id_of_queue);
        *id_of_queue = NULL;

        TR_ASSERT(NULL == *id_of_queue);

        return (TR_OK);
}

/*******************************************************************************************************
 *
 * Static dispatch tables
 *
 *******************************************************************************************************/
static const struct tr_queue_ops k_queue_array_dynamic_ops = {
        queue_array_dynamic_enqueue, queue_array_dequeue,  queue_array_front,  queue_array_size,
        queue_array_is_empty,        queue_array_capacity, queue_array_destroy
};

static const struct tr_queue_ops k_queue_array_fixed_ops = {
        queue_array_fixed_enqueue, queue_array_dequeue,  queue_array_front,  queue_array_size,
        queue_array_is_empty,      queue_array_capacity, queue_array_destroy
};

/*******************************************************************************************************
 *
 * FUNCTION NAME: tr_queue_array_create
 *
 * PURPOSE: Allocates and initialises a new array based queue instance
 *          Called by tr_queue_create for TR_QUEUE_ARRAY_DYNAMIC and TR_QUEUE_ARRAY_FIXED
 *
 * ARGUMENTS:
 *
 * ARGUMENT                TYPE             I/O     DESCRIPTION
 * --------                ----             ---     ------------
 * size_of_datatype        size_t           I       Byte size of the datatype to store
 * elements_to_allocate    size_t           I       Initial number of elements to allocate
 * queue_type              tr_queue_type_t  I       TR_QUEUE_ARRAY_DYNAMIC or TR_QUEUE_ARRAY_FIXED
 * id_of_queue             struct queue **  O       Pointer to pointer to receive the created queue
 *
 * RETURNS: tr_result_t
 *   TR_OK               - Queue created successfully
 *   TR_ERR_ALLOC        - Memory allocation failed
 *
 *******************************************************************************************************/
tr_result_t tr_queue_array_create(size_t size_of_datatype,
                                  size_t elements_to_allocate,
                                  tr_queue_type_t queue_type,
                                  struct queue **id_of_queue)
{
        /* local variables */
        struct queue *p_queue = NULL;              /* pointer to new queue handle     */
        struct tr_queue_array_data *p_data = NULL; /* pointer to implementation data  */

        /* allocate queue handle */
        p_queue = (struct queue *) malloc(sizeof(struct queue));
        if (NULL == p_queue)
        {
                /* LCOV_EXCL_START */
                fprintf(stderr, "[TR] tr_queue_array_create: malloc failed for queue handle\n");
                return (TR_ERR_ALLOC);
                /* LCOV_EXCL_STOP */
        }

        /* allocate implementation data */
        p_data = (struct tr_queue_array_data *) malloc(sizeof(struct tr_queue_array_data));
        if (NULL == p_data)
        {
                /* LCOV_EXCL_START */
                fprintf(stderr,
                        "[TR] tr_queue_array_create: malloc failed for implementation data\n");
                free(p_queue);
                return (TR_ERR_ALLOC);
                /* LCOV_EXCL_STOP */
        }

        /* allocate circular buffer */
        p_data->queue_data = malloc(elements_to_allocate * size_of_datatype);
        if (NULL == p_data->queue_data)
        {
                /* LCOV_EXCL_START */
                fprintf(stderr, "[TR] tr_queue_array_create: malloc failed for queue data\n");
                free(p_data);
                free(p_queue);
                return (TR_ERR_ALLOC);
                /* LCOV_EXCL_STOP */
        }

        /* initialise implementation data */
        p_data->head = 0u;
        p_data->tail = 0u;
        p_data->queue_size = 0u;
        p_data->capacity = elements_to_allocate;
        p_data->size_of_datatype = size_of_datatype;

        /* wire up correct dispatch table */
        if (TR_QUEUE_ARRAY_DYNAMIC == queue_type)
        {
                p_queue->ops = &k_queue_array_dynamic_ops;
        }
        else
        {
                p_queue->ops = &k_queue_array_fixed_ops;
        }

        p_queue->impl = p_data;
        *id_of_queue = p_queue;

        TR_ASSERT(NULL != p_queue->ops);
        TR_ASSERT(NULL != p_queue->impl);
        TR_ASSERT(0u == p_data->head);
        TR_ASSERT(0u == p_data->tail);
        TR_ASSERT(0u == p_data->queue_size);
        TR_ASSERT(p_data->capacity == elements_to_allocate);
        TR_ASSERT(p_data->size_of_datatype == size_of_datatype);

        return (TR_OK);
}
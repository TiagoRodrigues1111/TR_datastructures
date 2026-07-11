
/**
 * @file tr_queue.h
 * @brief Public API for the queue data structure
 *
 * Provides a generic FIFO queue implementation supporting multiple
 * underlying storage strategies selectable at runtime via
 * @ref tr_queue_type_t.
 *
 * Typical usage:
 * @code
 * struct queue *p_queue = NULL;
 * tr_result_t   res     = TR_OK;
 * int           val     = 42;
 * int           out     = 0;
 *
 * res = tr_queue_create(sizeof(int), 10, TR_QUEUE_ARRAY_DYNAMIC, &p_queue);
 * if (TR_OK != res) { return res; }
 *
 * res = tr_queue_enqueue(p_queue, &val);
 * if (TR_OK != res) { return res; }
 *
 * res = tr_queue_front(p_queue, &out);
 * if (TR_OK != res) { return res; }
 *
 * res = tr_queue_dequeue(p_queue);
 * if (TR_OK != res) { return res; }
 *
 * tr_queue_destroy(&p_queue);
 * @endcode
 */
/*******************************************************************************************************
 * NAME: tr_queue.h
 *
 * PURPOSE: Declaration of the queue functions
 *
 * GLOBAL VARIABLES:
 *
 * Variable              Type                    Description
 * --------              ----                    -----------
 *
 * DEVELOPMENT HISTORY:
 *
 * Date         Author          Change Id       Release         Description Of Change
 * ----------   --------------- ---------       -------         -----------------------------------
 * 14-06-2026   Tiago Rodrigues                       1         File preparation
 *
 *******************************************************************************************************/
#ifndef TR_QUEUE_H
#define TR_QUEUE_H

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
 * TYPE NAME: struct queue
 *
 * PURPOSE: Opaque handle to a queue instance
 *
 *******************************************************************************************************/
/**
 * @brief Opaque handle to a queue instance
 *
 * Users interact with the queue exclusively through the API functions.
 * The internal implementation is hidden.
 */
struct queue;

/*******************************************************************************************************
 *
 * TYPE NAME: tr_queue_type_t
 *
 * PURPOSE: Selects the underlying implementation used by the queue
 *
 * VALUES:
 *
 * VALUE                    DESCRIPTION
 * -----                    -----------
 * TR_QUEUE_ARRAY_DYNAMIC   Array based implementation - grows automatically
 * TR_QUEUE_ARRAY_FIXED     Array based implementation - fixed capacity
 * TR_QUEUE_LL              Linked list based implementation - dynamic growth
 *
 *******************************************************************************************************/
/**
 * @brief Selects the underlying implementation used by the queue
 *
 * Passed to @ref tr_queue_create to select which implementation
 * to use. The choice affects memory layout, growth behaviour
 * and performance characteristics.
 *
 * | Type                    | Memory     | Growth    | Best for              |
 * |-------------------------|------------|-----------|-----------------------|
 * | TR_QUEUE_ARRAY_DYNAMIC  | Contiguous | Automatic | General purpose       |
 * | TR_QUEUE_ARRAY_FIXED    | Contiguous | None      | Bounded/embedded use  |
 * | TR_QUEUE_LL             | Per node   | Automatic | Unpredictable size    |
 */
typedef enum tr_queue_type
{
        TR_QUEUE_ARRAY_DYNAMIC = 0, /**< Array based - grows automatically by factor of 2  */
        TR_QUEUE_ARRAY_FIXED = 1,   /**< Array based - fixed capacity, returns TR_ERR_FULL */
        TR_QUEUE_LL = 2             /**< Linked list based - dynamic node allocation       */
} tr_queue_type_t;

/*****************************************************/

/* 5 global variable declarations */
/*****************************************************/
/*****************************************************/

/* 6 function prototypes */
/*****************************************************/

/******************************************************************
 *
 * FUNCTION NAME: tr_queue_create
 *
 * PURPOSE: Allocates and initialises a new queue instance
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
 *****************************************************************/
/**
 * @brief Allocates and initialises a new queue instance
 *
 * @param[in]  size_of_datatype     Byte size of the datatype to store
 * @param[in]  elements_to_allocate Initial number of elements to allocate
 * @param[in]  queue_type           Implementation type to use
 * @param[out] id_of_queue          Pointer to receive the created queue
 *
 * @return TR_OK          Queue created successfully
 * @return TR_ERR_NULL    id_of_queue is NULL
 * @return TR_ERR_INVALID size_of_datatype or elements_to_allocate is 0
 * @return TR_ERR_ALLOC   Memory allocation failed
 */
TR_NODISCARD TR_API tr_result_t tr_queue_create(size_t size_of_datatype,
                                                size_t elements_to_allocate,
                                                tr_queue_type_t queue_type,
                                                struct queue **id_of_queue);

/******************************************************************
 *
 * FUNCTION NAME: tr_queue_destroy
 *
 * PURPOSE: Frees all memory associated with the queue instance
 *
 * ARGUMENTS:
 *
 * ARGUMENT                TYPE             I/O     DESCRIPTION
 * --------                ----             ---     ------------
 * id_of_queue             struct queue **  I/O     Pointer to pointer to the queue to destroy
 *                                                   Set to NULL after destruction
 *
 * RETURNS: tr_result_t
 *   TR_OK               - Queue destroyed successfully
 *   TR_ERR_NULL         - id_of_queue or *id_of_queue is NULL
 *
 *****************************************************************/
/**
 * @brief Frees all memory associated with the queue instance
 *
 * Destroys the queue and sets the pointer to @c NULL to prevent
 * use after free.
 *
 * @param[in,out] id_of_queue Pointer to pointer to the queue to destroy.
 *                            Set to @c NULL after destruction
 *
 * @return TR_OK       Queue destroyed successfully
 * @return TR_ERR_NULL id_of_queue or *id_of_queue is NULL
 *
 * Example:
 * @code
 * struct queue *p_queue = NULL;
 *
 * tr_queue_create(sizeof(int), 10, TR_QUEUE_ARRAY_DYNAMIC, &p_queue);
 *
 * tr_queue_destroy(&p_queue);
 * // p_queue is now NULL
 * @endcode
 */
TR_NODISCARD TR_API tr_result_t tr_queue_destroy(struct queue **id_of_queue);

/******************************************************************
 *
 * FUNCTION NAME: tr_queue_enqueue
 *
 * PURPOSE: Adds a deep copy of the data to the back of the queue
 *
 * ARGUMENTS:
 *
 * ARGUMENT                TYPE             I/O     DESCRIPTION
 * --------                ----             ---     ------------
 * id_of_queue             struct queue *   I/O     Pointer to the queue to enqueue onto
 * data_to_enqueue         const void *     I       Pointer to the data to copy into the queue
 *
 * RETURNS: tr_result_t
 *   TR_OK               - Data enqueued successfully
 *   TR_ERR_NULL         - id_of_queue or data_to_enqueue is NULL
 *   TR_ERR_ALLOC        - Memory allocation failed (linked list only)
 *   TR_ERR_FULL         - Queue is full (array fixed only)
 *
 *****************************************************************/
/**
 * @brief Adds a deep copy of the data to the back of the queue
 *
 * Copies @c size_of_datatype bytes from @p data_to_enqueue into
 * the queue. The caller retains ownership of the original data.
 *
 * For @ref TR_QUEUE_ARRAY_DYNAMIC queues the array grows automatically
 * by a factor of 2 when full. For @ref TR_QUEUE_ARRAY_FIXED queues
 * @ref TR_ERR_FULL is returned when capacity is reached.
 *
 * @param[in,out] id_of_queue      Pointer to the queue to enqueue onto
 * @param[in]     data_to_enqueue  Pointer to the data to copy into the queue
 *
 * @return TR_OK        Data enqueued successfully
 * @return TR_ERR_NULL  id_of_queue or data_to_enqueue is NULL
 * @return TR_ERR_ALLOC Memory reallocation failed (dynamic only)
 * @return TR_ERR_FULL  Queue is at capacity (fixed only)
 *
 * Example:
 * @code
 * int         val = 42;
 * tr_result_t res = TR_OK;
 *
 * res = tr_queue_enqueue(p_queue, &val);
 * if (TR_OK != res)
 * {
 *     // handle error
 * }
 * @endcode
 */
TR_NODISCARD TR_API tr_result_t tr_queue_enqueue(struct queue *id_of_queue,
                                                 const void *data_to_enqueue);

/******************************************************************
 *
 * FUNCTION NAME: tr_queue_dequeue
 *
 * PURPOSE: Removes the element at the front of the queue
 *
 * ARGUMENTS:
 *
 * ARGUMENT                TYPE             I/O     DESCRIPTION
 * --------                ----             ---     ------------
 * id_of_queue             struct queue *   I/O     Pointer to the queue to dequeue from
 *
 * RETURNS: tr_result_t
 *   TR_OK               - Element removed successfully
 *   TR_ERR_NULL         - id_of_queue is NULL
 *   TR_ERR_EMPTY        - Queue is empty
 *
 *****************************************************************/
/**
 * @brief Removes the element at the front of the queue
 *
 * Removes the front element without returning its value — call
 * @ref tr_queue_front first if you need the value before removing it.
 *
 * @param[in,out] id_of_queue Pointer to the queue to dequeue from
 *
 * @return TR_OK        Element removed successfully
 * @return TR_ERR_NULL  id_of_queue is NULL
 * @return TR_ERR_EMPTY Queue is empty
 *
 * Example:
 * @code
 * int         out = 0;
 * tr_result_t res = TR_OK;
 *
 * res = tr_queue_front(p_queue, &out);
 * if (TR_OK == res)
 * {
 *     tr_queue_dequeue(p_queue);
 * }
 * @endcode
 */
TR_NODISCARD TR_API tr_result_t tr_queue_dequeue(struct queue *id_of_queue);

/******************************************************************
 *
 * FUNCTION NAME: tr_queue_front
 *
 * PURPOSE: Copies the element at the front of the queue into the provided buffer
 *          Does not remove the element
 *
 * ARGUMENTS:
 *
 * ARGUMENT                TYPE                  I/O     DESCRIPTION
 * --------                ----                  ---     ------------
 * id_of_queue             const struct queue *  I       Pointer to the queue to peek at
 * data_at_front           void *                O       Pointer to buffer to copy the front
 *                                                        element into. Must be at least
 *                                                        size_of_datatype bytes
 *
 * RETURNS: tr_result_t
 *   TR_OK               - Data copied successfully
 *   TR_ERR_NULL         - id_of_queue or data_at_front is NULL
 *   TR_ERR_EMPTY        - Queue is empty
 *
 *****************************************************************/
/**
 * @brief Copies the element at the front of the queue into the provided buffer
 *
 * Copies @c size_of_datatype bytes from the front of the queue into
 * @p data_at_front. The element is not removed — call @ref tr_queue_dequeue
 * afterwards if removal is needed.
 *
 * The buffer pointed to by @p data_at_front must be at least
 * @c size_of_datatype bytes large.
 *
 * @param[in]  id_of_queue   Pointer to the queue to peek at
 * @param[out] data_at_front Buffer to copy the front element into
 *
 * @return TR_OK        Data copied successfully
 * @return TR_ERR_NULL  id_of_queue or data_at_front is NULL
 * @return TR_ERR_EMPTY Queue is empty
 *
 * Example:
 * @code
 * int         out = 0;
 * tr_result_t res = TR_OK;
 *
 * res = tr_queue_front(p_queue, &out);
 * if (TR_OK == res)
 * {
 *     printf("Front value: %d\n", out);
 * }
 * @endcode
 */
TR_NODISCARD TR_API tr_result_t tr_queue_front(const struct queue *id_of_queue,
                                               void *data_at_front);

/******************************************************************
 *
 * FUNCTION NAME: tr_queue_size
 *
 * PURPOSE: Returns the current number of elements in the queue
 *
 * ARGUMENTS:
 *
 * ARGUMENT                TYPE                  I/O     DESCRIPTION
 * --------                ----                  ---     ------------
 * id_of_queue             const struct queue *  I       Pointer to the queue to query
 * size                    size_t *              O       Pointer to receive the current element
 * count
 *
 * RETURNS: tr_result_t
 *   TR_OK               - Size retrieved successfully
 *   TR_ERR_NULL         - id_of_queue or size is NULL
 *
 *****************************************************************/
/**
 * @brief Returns the current number of elements in the queue
 *
 * @param[in]  id_of_queue Pointer to the queue to query
 * @param[out] size        Pointer to receive the current element count
 *
 * @return TR_OK       Size retrieved successfully
 * @return TR_ERR_NULL id_of_queue or size is NULL
 *
 * Example:
 * @code
 * size_t      size = 0u;
 * tr_result_t res  = TR_OK;
 *
 * res = tr_queue_size(p_queue, &size);
 * if (TR_OK == res)
 * {
 *     printf("Queue has %lu elements\n", (unsigned long)size);
 * }
 * @endcode
 */
TR_NODISCARD TR_API tr_result_t tr_queue_size(const struct queue *id_of_queue, size_t *size);

/******************************************************************
 *
 * FUNCTION NAME: tr_queue_is_empty
 *
 * PURPOSE: Checks whether the queue contains no elements
 *
 * ARGUMENTS:
 *
 * ARGUMENT                TYPE                  I/O     DESCRIPTION
 * --------                ----                  ---     ------------
 * id_of_queue             const struct queue *  I       Pointer to the queue to check
 * is_empty                bool *                O       Pointer to receive the result
 *                                                        Set to true if empty, false otherwise
 *
 * RETURNS: tr_result_t
 *   TR_OK               - Check completed successfully
 *   TR_ERR_NULL         - id_of_queue or is_empty is NULL
 *
 *****************************************************************/
/**
 * @brief Checks whether the queue contains no elements
 *
 * @param[in]  id_of_queue Pointer to the queue to check
 * @param[out] is_empty    Set to @c true if the queue is empty,
 *                         @c false otherwise
 *
 * @return TR_OK       Check completed successfully
 * @return TR_ERR_NULL id_of_queue or is_empty is NULL
 *
 * Example:
 * @code
 * bool        is_empty = false;
 * tr_result_t res      = TR_OK;
 *
 * res = tr_queue_is_empty(p_queue, &is_empty);
 * if (TR_OK == res && is_empty)
 * {
 *     printf("Queue is empty\n");
 * }
 * @endcode
 */
TR_NODISCARD TR_API tr_result_t tr_queue_is_empty(const struct queue *id_of_queue, bool *is_empty);

/******************************************************************
 *
 * FUNCTION NAME: tr_queue_capacity
 *
 * PURPOSE: Returns the total allocated capacity of the queue
 *          For linked list based queues this is the same as queue_size
 *
 * ARGUMENTS:
 *
 * ARGUMENT                TYPE                  I/O     DESCRIPTION
 * --------                ----                  ---     ------------
 * id_of_queue             const struct queue *  I       Pointer to the queue to query
 * capacity                size_t *              O       Pointer to receive the capacity
 *
 * RETURNS: tr_result_t
 *   TR_OK               - Capacity retrieved successfully
 *   TR_ERR_NULL         - id_of_queue or capacity is NULL
 *
 *****************************************************************/
/**
 * @brief Returns the total allocated capacity of the queue
 *
 * For @ref TR_QUEUE_ARRAY_DYNAMIC queues capacity grows automatically
 * and may be larger than the current size. For @ref TR_QUEUE_ARRAY_FIXED
 * queues capacity is fixed at creation time and never changes.
 * For @ref TR_QUEUE_LL queues capacity equals the current size since
 * nodes are allocated individually.
 *
 * @param[in]  id_of_queue Pointer to the queue to query
 * @param[out] capacity    Pointer to receive the capacity
 *
 * @return TR_OK       Capacity retrieved successfully
 * @return TR_ERR_NULL id_of_queue or capacity is NULL
 *
 * Example:
 * @code
 * size_t      capacity = 0u;
 * size_t      size     = 0u;
 * tr_result_t res      = TR_OK;
 *
 * tr_queue_size(p_queue, &size);
 * res = tr_queue_capacity(p_queue, &capacity);
 * if (TR_OK == res)
 * {
 *     printf("Using %lu of %lu slots\n",
 *            (unsigned long)size,
 *            (unsigned long)capacity);
 * }
 * @endcode
 */
TR_NODISCARD TR_API tr_result_t tr_queue_capacity(const struct queue *id_of_queue,
                                                  size_t *capacity);

/*****************************************************/

#ifdef __cplusplus
}
#endif

#endif /* TR_QUEUE_H */
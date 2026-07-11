/*******************************************************************************************************
* NAME: example_queue.c
*
* PURPOSE: Demonstrates usage of the tr_datastructures queue API
*
* GLOBAL VARIABLES: None
*
* DEVELOPMENT HISTORY:
*
* Date          Author          Change Id       Release         Description Of Change
* ----------    --------------- ---------       -------         -----------------------------------
* 05-07-2026    Tiago Rodrigues                       1         File preparation
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

/* 1.1 related header */
/* none */

/* 1.2 C system headers */
#include <stdio.h>      /* printf, fprintf  */

/* 1.3 C++ standard library headers */
/* none */

/* 1.4 other libraries' headers */
/* none */

/* 1.5 project headers */
#include "tr_datastructures/tr_version.h"
#include "tr_datastructures.h"

/*****************************************************/


/* 2 defines */
/*****************************************************/
#define EXAMPLE_CAPACITY    8u
#define EXAMPLE_FIXED_CAP   4u
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
static void example_basic_dynamic(void);
static void example_basic_fixed(void);
static void example_print_queue(void);
static void print_separator(const char *p_title);
/*****************************************************/


/*******************************************************************************************************
*
* FUNCTION NAME: print_separator
*
* PURPOSE: Prints a formatted section separator to stdout
*
* ARGUMENTS:
*
* ARGUMENT             TYPE            I/O     DESCRIPTION
* --------             ----            ---     ------------
* p_title              const char *    I       Title to display in the separator
*
* RETURNS: void
*
*******************************************************************************************************/
static void print_separator(const char *p_title)
{
    /* local variables */
    /* none */

    printf("\n");
    printf("==================================================\n");
    printf(" %s\n", p_title);
    printf("==================================================\n");
}


/*******************************************************************************************************
*
* FUNCTION NAME: example_basic_dynamic
*
* PURPOSE: Demonstrates basic usage of a dynamic array queue
*          Shows create, enqueue, front, dequeue, size, is_empty, destroy
*
* ARGUMENTS: none
*
* RETURNS: void
*
*******************************************************************************************************/
static void example_basic_dynamic(void)
{
    /* local variables */
    struct queue *p_queue  = NULL;  /* queue handle             */
    tr_result_t   res      = TR_OK; /* operation result         */
    int           val      = 0;     /* value to enqueue         */
    int           front    = 0;     /* value at front of queue  */
    size_t        size     = 0u;    /* current queue size       */
    bool          empty    = false; /* is queue empty           */
    int           i        = 0;     /* loop counter             */

    print_separator("Basic Dynamic Queue");

    /* --- create --- */
    res = tr_queue_create(sizeof(int), EXAMPLE_CAPACITY, TR_QUEUE_ARRAY_DYNAMIC, &p_queue);
    if (TR_OK != res)
    {
        fprintf(stderr, "Failed to create queue: %d\n", res);
        return ;
    }
    printf("Created dynamic queue with initial capacity %u\n", EXAMPLE_CAPACITY);

    /* --- enqueue --- */
    printf("\nEnqueueing values 1 to 5:\n");
    for (i = 1; i <= 5; i++)
    {
        val = i;
        res = tr_queue_enqueue(p_queue, &val);
        if (TR_OK != res)
        {
            fprintf(stderr, "Enqueue failed: %d\n", res);
            (void)tr_queue_destroy(&p_queue);
            return ;
        }
        printf("  enqueued: %d\n", val);
    }

    /* --- size --- */
    (void)tr_queue_size(p_queue, &size);
    printf("\nQueue size: %lu\n", (unsigned long)size);

    /* --- front --- */
    (void)tr_queue_front(p_queue, &front);
    printf("Front element: %d\n", front);

    /* --- dequeue --- */
    printf("\nDequeueing all elements:\n");
    while (true)
    {
        (void)tr_queue_is_empty(p_queue, &empty);
        if (empty)
        {
            break;
        }

        (void)tr_queue_front(p_queue, &front);
        (void)tr_queue_dequeue(p_queue);
        printf("  dequeued: %d\n", front);
    }

    /* --- is_empty --- */
    (void)tr_queue_is_empty(p_queue, &empty);
    printf("\nQueue is empty: %s\n", empty ? "true" : "false");

    /* --- destroy --- */
    (void)tr_queue_destroy(&p_queue);
    printf("Queue destroyed. Handle is now: %s\n", (NULL == p_queue) ? "NULL" : "not NULL");
}


/*******************************************************************************************************
*
* FUNCTION NAME: example_basic_fixed
*
* PURPOSE: Demonstrates basic usage of a fixed capacity array queue
*          Shows TR_ERR_FULL behaviour when capacity is exceeded
*
* ARGUMENTS: none
*
* RETURNS: void
*
*******************************************************************************************************/
static void example_basic_fixed(void)
{
    /* local variables */
    struct queue *p_queue  = NULL;  /* queue handle             */
    tr_result_t   res      = TR_OK; /* operation result         */
    int           val      = 0;     /* value to enqueue         */
    size_t        capacity = 0u;    /* queue capacity           */
    int           i        = 0;     /* loop counter             */

    print_separator("Basic Fixed Queue");

    /* --- create --- */
    res = tr_queue_create(sizeof(int), EXAMPLE_FIXED_CAP, TR_QUEUE_ARRAY_FIXED, &p_queue);
    if (TR_OK != res)
    {
        fprintf(stderr, "Failed to create queue: %d\n", res);
        return ;
    }

    (void)tr_queue_capacity(p_queue, &capacity);
    printf("Created fixed queue with capacity %lu\n", (unsigned long)capacity);

    /* --- enqueue to capacity --- */
    printf("\nFilling queue to capacity:\n");
    for (i = 1; i <= (int)EXAMPLE_FIXED_CAP; i++)
    {
        val = i * 10;
        res = tr_queue_enqueue(p_queue, &val);
        printf("  enqueue %d: %s\n", val, (TR_OK == res) ? "OK" : "FAILED");
    }

    /* --- enqueue beyond capacity --- */
    printf("\nAttempting to enqueue beyond capacity:\n");
    val = 999;
    res = tr_queue_enqueue(p_queue, &val);
    printf("  enqueue %d: %s (expected TR_ERR_FULL)\n",
           val,
           (TR_ERR_FULL == res) ? "TR_ERR_FULL" : "unexpected result");

    /* --- destroy --- */
    (void)tr_queue_destroy(&p_queue);
    printf("\nQueue destroyed\n");
}


/*******************************************************************************************************
*
* FUNCTION NAME: example_print_queue
*
* PURPOSE: Demonstrates a real use case - using a queue to process items in order
*          Simulates a simple print queue where jobs are processed FIFO
*
* ARGUMENTS: none
*
* RETURNS: void
*
*******************************************************************************************************/
static void example_print_queue(void)
{
    /* local variables */
    struct queue *p_queue           = NULL;     /* queue handle             */
    tr_result_t   res               = TR_OK;    /* operation result         */
    int           jobs[]            = { 101, 205, 307, 412, 518 };
    size_t        length            = 0u;       /* number of jobs           */
    size_t        i                 = 0u;       /* loop counter             */
    int           job               = 0;        /* current job              */

    print_separator("Real Use Case: Print Queue");

    length = sizeof(jobs) / sizeof(jobs[0]);

    /* --- create queue --- */
    res = tr_queue_create(sizeof(int), length, TR_QUEUE_ARRAY_DYNAMIC, &p_queue);
    if (TR_OK != res)
    {
        fprintf(stderr, "Failed to create queue: %d\n", res);
        return ;
    }

    /* --- submit print jobs --- */
    printf("Submitting print jobs:\n");
    for (i = 0u; i < length; i++)
    {
        res = tr_queue_enqueue(p_queue, &jobs[i]);
        if (TR_OK != res)
        {
            fprintf(stderr, "Failed to enqueue job %d: %d\n", jobs[i], res);
            (void)tr_queue_destroy(&p_queue);
            return ;
        }
        printf("  submitted job #%d\n", jobs[i]);
    }

    /* --- process print jobs in FIFO order --- */
    printf("\nProcessing print jobs (FIFO order):\n");
    while (true)
    {
        bool is_empty = false;

        (void)tr_queue_is_empty(p_queue, &is_empty);
        if (is_empty)
        {
            break;
        }

        (void)tr_queue_front(p_queue, &job);
        (void)tr_queue_dequeue(p_queue);
        printf("  processing job #%d... done\n", job);
    }

    /* --- destroy --- */
    (void)tr_queue_destroy(&p_queue);
    printf("Queue destroyed\n");
}


/*******************************************************************************************************
*
* FUNCTION NAME: main
*
* PURPOSE: Entry point - runs all queue examples
*
* ARGUMENTS: none
*
* RETURNS: int (0 on success)
*
*******************************************************************************************************/
int main(void)
{
    /* local variables */
    /* none */

    printf("TR_datastructures v%s - Queue Examples\n", TR_VERSION_STRING);

    example_basic_dynamic();
    example_basic_fixed();
    example_print_queue();

    printf("\n==================================================\n");
    printf(" Done\n");
    printf("==================================================\n");

    return (0);
}
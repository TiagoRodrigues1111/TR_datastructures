/*******************************************************************************************************
 * NAME: test_queue.c
 *
 * PURPOSE: Unit tests for the queue data structure
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

/* 1.1 related header */
/* none */

/* 1.2 C system headers */
#include <stddef.h> /* size_t   */

/* 1.3 C++ standard library headers */
/* none */

/* 1.4 other libraries' headers */
#include "unity.h"

/* 1.5 project headers */
#include "tr_datastructures/tr_queue.h"

/*****************************************************/

/* 2 defines */
/*****************************************************/
#define TEST_CAPACITY 10u
#define TEST_VALUE_A 42
#define TEST_VALUE_B 99
#define TEST_VALUE_C 7
/*****************************************************/

/* 3 external declarations */
/*****************************************************/
/*****************************************************/

/* 4 typedefs */
/*****************************************************/
/*****************************************************/

/* 5 global variable declarations */
/*****************************************************/
static struct queue *gp_queue_dynamic = NULL;
static struct queue *gp_queue_fixed = NULL;
static struct queue *gp_queue_ll = NULL;
/*****************************************************/

/* 6 function prototypes */
/*****************************************************/
void setUp(void);
void tearDown(void);

/* tr_queue_create */
void test_tr_queue_create_dynamic_returns_ok(void);
void test_tr_queue_create_fixed_returns_ok(void);
void test_tr_queue_create_returns_err_null_on_null_output(void);
void test_tr_queue_create_returns_err_invalid_on_zero_datatype_size(void);
void test_tr_queue_create_returns_err_invalid_on_zero_capacity(void);
void test_tr_queue_create_returns_err_invalid_on_unknown_type(void);

/* tr_queue_destroy */
void test_tr_queue_destroy_sets_pointer_to_null(void);
void test_tr_queue_destroy_returns_err_null_on_null_pointer(void);
void test_tr_queue_destroy_returns_err_null_on_null_queue(void);

/* tr_queue_enqueue */
void test_tr_queue_enqueue_returns_ok_on_valid_enqueue(void);
void test_tr_queue_enqueue_returns_err_null_on_null_queue(void);
void test_tr_queue_enqueue_returns_err_null_on_null_data(void);
void test_tr_queue_enqueue_fixed_returns_err_full_when_at_capacity(void);
void test_tr_queue_enqueue_dynamic_grows_beyond_initial_capacity(void);

/* tr_queue_dequeue */
void test_tr_queue_dequeue_returns_ok_on_valid_dequeue(void);
void test_tr_queue_dequeue_returns_err_null_on_null_queue(void);
void test_tr_queue_dequeue_returns_err_empty_on_empty_queue(void);
void test_tr_queue_dequeue_decrements_size(void);

/* tr_queue_front */
void test_tr_queue_front_returns_correct_value(void);
void test_tr_queue_front_does_not_remove_element(void);
void test_tr_queue_front_returns_first_enqueued_value(void);
void test_tr_queue_front_returns_err_null_on_null_queue(void);
void test_tr_queue_front_returns_err_null_on_null_output(void);
void test_tr_queue_front_returns_err_empty_on_empty_queue(void);

/* tr_queue_size */
void test_tr_queue_size_returns_zero_on_empty_queue(void);
void test_tr_queue_size_increments_on_enqueue(void);
void test_tr_queue_size_returns_err_null_on_null_queue(void);
void test_tr_queue_size_returns_err_null_on_null_output(void);

/* tr_queue_is_empty */
void test_tr_queue_is_empty_returns_true_on_empty_queue(void);
void test_tr_queue_is_empty_returns_false_after_enqueue(void);
void test_tr_queue_is_empty_returns_true_after_enqueue_and_dequeue(void);
void test_tr_queue_is_empty_returns_err_null_on_null_queue(void);
void test_tr_queue_is_empty_returns_err_null_on_null_output(void);

/* tr_queue_capacity */
void test_tr_queue_capacity_returns_initial_capacity(void);
void test_tr_queue_capacity_grows_on_dynamic_queue(void);
void test_tr_queue_capacity_fixed_does_not_grow(void);
void test_tr_queue_capacity_returns_err_null_on_null_queue(void);
void test_tr_queue_capacity_returns_err_null_on_null_output(void);

/* circular buffer specific */
void test_tr_queue_fifo_order_is_correct(void);
void test_tr_queue_circular_buffer_wraps_correctly(void);
void test_tr_queue_enqueue_dequeue_multiple_types(void);

/* tr_queue_ll specific */
void test_tr_queue_create_ll_returns_ok(void);
void test_tr_queue_ll_enqueue_returns_ok(void);
void test_tr_queue_ll_enqueue_grows_unbounded(void);
void test_tr_queue_ll_dequeue_returns_ok(void);
void test_tr_queue_ll_dequeue_returns_err_empty(void);
void test_tr_queue_ll_front_returns_correct_value(void);
void test_tr_queue_ll_front_returns_err_empty(void);
void test_tr_queue_ll_front_returns_first_enqueued_value(void);
void test_tr_queue_ll_size_equals_enqueue_count(void);
void test_tr_queue_ll_capacity_equals_size(void);
void test_tr_queue_ll_is_empty_returns_true_on_empty(void);
void test_tr_queue_ll_is_empty_returns_false_after_enqueue(void);
void test_tr_queue_ll_destroy_sets_pointer_to_null(void);
void test_tr_queue_ll_destroy_with_elements(void);
void test_tr_queue_ll_fifo_order_is_correct(void);

/*****************************************************/

/*******************************************************************************************************
 *
 * FUNCTION NAME: setUp
 *
 * PURPOSE: Creates fresh queues before each test
 *          Called automatically by Unity before every test function
 *
 * ARGUMENTS: none
 *
 * RETURNS: void
 *
 *******************************************************************************************************/
void setUp(void)
{
        (void) tr_queue_create(
                sizeof(int), TEST_CAPACITY, TR_QUEUE_ARRAY_DYNAMIC, &gp_queue_dynamic);
        (void) tr_queue_create(sizeof(int), TEST_CAPACITY, TR_QUEUE_ARRAY_FIXED, &gp_queue_fixed);
        (void) tr_queue_create(sizeof(int), TEST_CAPACITY, TR_QUEUE_LL, &gp_queue_ll);
}

/*******************************************************************************************************
 *
 * FUNCTION NAME: tearDown
 *
 * PURPOSE: Destroys queues after each test
 *          Called automatically by Unity after every test function
 *
 * ARGUMENTS: none
 *
 * RETURNS: void
 *
 *******************************************************************************************************/
void tearDown(void)
{
        if (NULL != gp_queue_dynamic)
        {
                (void) tr_queue_destroy(&gp_queue_dynamic);
                gp_queue_dynamic = NULL;
        }

        if (NULL != gp_queue_fixed)
        {
                (void) tr_queue_destroy(&gp_queue_fixed);
                gp_queue_fixed = NULL;
        }

        if (NULL != gp_queue_ll)
        {
                (void) tr_queue_destroy(&gp_queue_ll);
                gp_queue_ll = NULL;
        }
}

/* ======================================================================
 * tr_queue_create tests
 * ====================================================================== */

void test_tr_queue_create_dynamic_returns_ok(void)
{
        struct queue *p_queue = NULL;
        tr_result_t res = TR_OK;

        res = tr_queue_create(sizeof(int), TEST_CAPACITY, TR_QUEUE_ARRAY_DYNAMIC, &p_queue);

        TEST_ASSERT_EQUAL(TR_OK, res);
        TEST_ASSERT_NOT_NULL(p_queue);

        (void) tr_queue_destroy(&p_queue);
}

void test_tr_queue_create_fixed_returns_ok(void)
{
        struct queue *p_queue = NULL;
        tr_result_t res = TR_OK;

        res = tr_queue_create(sizeof(int), TEST_CAPACITY, TR_QUEUE_ARRAY_FIXED, &p_queue);

        TEST_ASSERT_EQUAL(TR_OK, res);
        TEST_ASSERT_NOT_NULL(p_queue);

        (void) tr_queue_destroy(&p_queue);
}

void test_tr_queue_create_returns_err_null_on_null_output(void)
{
        tr_result_t res = TR_OK;

        res = tr_queue_create(sizeof(int), TEST_CAPACITY, TR_QUEUE_ARRAY_DYNAMIC, NULL);

        TEST_ASSERT_EQUAL(TR_ERR_NULL, res);
}

void test_tr_queue_create_returns_err_invalid_on_zero_datatype_size(void)
{
        struct queue *p_queue = NULL;
        tr_result_t res = TR_OK;

        res = tr_queue_create(0u, TEST_CAPACITY, TR_QUEUE_ARRAY_DYNAMIC, &p_queue);

        TEST_ASSERT_EQUAL(TR_ERR_INVALID, res);
        TEST_ASSERT_NULL(p_queue);
}

void test_tr_queue_create_returns_err_invalid_on_zero_capacity(void)
{
        struct queue *p_queue = NULL;
        tr_result_t res = TR_OK;

        res = tr_queue_create(sizeof(int), 0u, TR_QUEUE_ARRAY_DYNAMIC, &p_queue);

        TEST_ASSERT_EQUAL(TR_ERR_INVALID, res);
        TEST_ASSERT_NULL(p_queue);
}

void test_tr_queue_create_returns_err_invalid_on_unknown_type(void)
{
        struct queue *p_queue = NULL;
        tr_result_t res = TR_OK;

        res = tr_queue_create(sizeof(int), TEST_CAPACITY, (tr_queue_type_t) 99, &p_queue);

        TEST_ASSERT_EQUAL(TR_ERR_INVALID, res);
        TEST_ASSERT_NULL(p_queue);
}

/* ======================================================================
 * tr_queue_destroy tests
 * ====================================================================== */

void test_tr_queue_destroy_sets_pointer_to_null(void)
{
        struct queue *p_queue = NULL;
        tr_result_t res = TR_OK;

        (void) tr_queue_create(sizeof(int), TEST_CAPACITY, TR_QUEUE_ARRAY_DYNAMIC, &p_queue);

        res = tr_queue_destroy(&p_queue);

        TEST_ASSERT_EQUAL(TR_OK, res);
        TEST_ASSERT_NULL(p_queue);
}

void test_tr_queue_destroy_returns_err_null_on_null_pointer(void)
{
        tr_result_t res = TR_OK;

        res = tr_queue_destroy(NULL);

        TEST_ASSERT_EQUAL(TR_ERR_NULL, res);
}

void test_tr_queue_destroy_returns_err_null_on_null_queue(void)
{
        struct queue *p_queue = NULL;
        tr_result_t res = TR_OK;

        res = tr_queue_destroy(&p_queue);

        TEST_ASSERT_EQUAL(TR_ERR_NULL, res);
}

/* ======================================================================
 * tr_queue_enqueue tests
 * ====================================================================== */

void test_tr_queue_enqueue_returns_ok_on_valid_enqueue(void)
{
        tr_result_t res = TR_OK;
        int val = TEST_VALUE_A;

        res = tr_queue_enqueue(gp_queue_dynamic, &val);

        TEST_ASSERT_EQUAL(TR_OK, res);
}

void test_tr_queue_enqueue_returns_err_null_on_null_queue(void)
{
        tr_result_t res = TR_OK;
        int val = TEST_VALUE_A;

        res = tr_queue_enqueue(NULL, &val);

        TEST_ASSERT_EQUAL(TR_ERR_NULL, res);
}

void test_tr_queue_enqueue_returns_err_null_on_null_data(void)
{
        tr_result_t res = TR_OK;

        res = tr_queue_enqueue(gp_queue_dynamic, NULL);

        TEST_ASSERT_EQUAL(TR_ERR_NULL, res);
}

void test_tr_queue_enqueue_fixed_returns_err_full_when_at_capacity(void)
{
        /* local variables */
        tr_result_t res = TR_OK;
        int val = TEST_VALUE_A;
        size_t i = 0u;

        /* fill to capacity */
        for (i = 0u; i < TEST_CAPACITY; i++)
        {
                res = tr_queue_enqueue(gp_queue_fixed, &val);
                TEST_ASSERT_EQUAL(TR_OK, res);
        }

        /* one more should fail */
        res = tr_queue_enqueue(gp_queue_fixed, &val);

        TEST_ASSERT_EQUAL(TR_ERR_FULL, res);
}

void test_tr_queue_enqueue_dynamic_grows_beyond_initial_capacity(void)
{
        /* local variables */
        tr_result_t res = TR_OK;
        int val = TEST_VALUE_A;
        size_t i = 0u;

        /* push beyond initial capacity */
        for (i = 0u; i < (TEST_CAPACITY * 2u); i++)
        {
                res = tr_queue_enqueue(gp_queue_dynamic, &val);
                TEST_ASSERT_EQUAL(TR_OK, res);
        }
}

/* ======================================================================
 * tr_queue_dequeue tests
 * ====================================================================== */

void test_tr_queue_dequeue_returns_ok_on_valid_dequeue(void)
{
        tr_result_t res = TR_OK;
        int val = TEST_VALUE_A;

        (void) tr_queue_enqueue(gp_queue_dynamic, &val);

        res = tr_queue_dequeue(gp_queue_dynamic);

        TEST_ASSERT_EQUAL(TR_OK, res);
}

void test_tr_queue_dequeue_returns_err_null_on_null_queue(void)
{
        tr_result_t res = TR_OK;

        res = tr_queue_dequeue(NULL);

        TEST_ASSERT_EQUAL(TR_ERR_NULL, res);
}

void test_tr_queue_dequeue_returns_err_empty_on_empty_queue(void)
{
        tr_result_t res = TR_OK;

        res = tr_queue_dequeue(gp_queue_dynamic);

        TEST_ASSERT_EQUAL(TR_ERR_EMPTY, res);
}

void test_tr_queue_dequeue_decrements_size(void)
{
        /* local variables */
        tr_result_t res = TR_OK;
        int val = TEST_VALUE_A;
        size_t size = 0u;

        (void) tr_queue_enqueue(gp_queue_dynamic, &val);
        (void) tr_queue_enqueue(gp_queue_dynamic, &val);

        (void) tr_queue_dequeue(gp_queue_dynamic);

        res = tr_queue_size(gp_queue_dynamic, &size);

        TEST_ASSERT_EQUAL(TR_OK, res);
        TEST_ASSERT_EQUAL_size_t(1u, size);
}

/* ======================================================================
 * tr_queue_front tests
 * ====================================================================== */

void test_tr_queue_front_returns_correct_value(void)
{
        tr_result_t res = TR_OK;
        int val = TEST_VALUE_A;
        int out = 0;

        (void) tr_queue_enqueue(gp_queue_dynamic, &val);

        res = tr_queue_front(gp_queue_dynamic, &out);

        TEST_ASSERT_EQUAL(TR_OK, res);
        TEST_ASSERT_EQUAL_INT(TEST_VALUE_A, out);
}

void test_tr_queue_front_does_not_remove_element(void)
{
        /* local variables */
        tr_result_t res = TR_OK;
        int val = TEST_VALUE_A;
        int out = 0;
        size_t size = 0u;

        (void) tr_queue_enqueue(gp_queue_dynamic, &val);
        (void) tr_queue_front(gp_queue_dynamic, &out);

        res = tr_queue_size(gp_queue_dynamic, &size);

        TEST_ASSERT_EQUAL(TR_OK, res);
        TEST_ASSERT_EQUAL_size_t(1u, size);
}

void test_tr_queue_front_returns_first_enqueued_value(void)
{
        /* local variables */
        int val_a = TEST_VALUE_A;
        int val_b = TEST_VALUE_B;
        int out = 0;

        /* enqueue A then B — front should return A (FIFO) */
        (void) tr_queue_enqueue(gp_queue_dynamic, &val_a);
        (void) tr_queue_enqueue(gp_queue_dynamic, &val_b);

        (void) tr_queue_front(gp_queue_dynamic, &out);

        TEST_ASSERT_EQUAL_INT(TEST_VALUE_A, out);
}

void test_tr_queue_front_returns_err_null_on_null_queue(void)
{
        tr_result_t res = TR_OK;
        int out = 0;

        res = tr_queue_front(NULL, &out);

        TEST_ASSERT_EQUAL(TR_ERR_NULL, res);
}

void test_tr_queue_front_returns_err_null_on_null_output(void)
{
        tr_result_t res = TR_OK;
        int val = TEST_VALUE_A;

        (void) tr_queue_enqueue(gp_queue_dynamic, &val);

        res = tr_queue_front(gp_queue_dynamic, NULL);

        TEST_ASSERT_EQUAL(TR_ERR_NULL, res);
}

void test_tr_queue_front_returns_err_empty_on_empty_queue(void)
{
        tr_result_t res = TR_OK;
        int out = 0;

        res = tr_queue_front(gp_queue_dynamic, &out);

        TEST_ASSERT_EQUAL(TR_ERR_EMPTY, res);
}

/* ======================================================================
 * tr_queue_size tests
 * ====================================================================== */

void test_tr_queue_size_returns_zero_on_empty_queue(void)
{
        /* local variables */
        tr_result_t res = TR_OK;
        size_t size = 0u;

        res = tr_queue_size(gp_queue_dynamic, &size);

        TEST_ASSERT_EQUAL(TR_OK, res);
        TEST_ASSERT_EQUAL_size_t(0u, size);
}

void test_tr_queue_size_increments_on_enqueue(void)
{
        /* local variables */
        tr_result_t res = TR_OK;
        int val = TEST_VALUE_A;
        size_t size = 0u;

        (void) tr_queue_enqueue(gp_queue_dynamic, &val);
        (void) tr_queue_enqueue(gp_queue_dynamic, &val);
        (void) tr_queue_enqueue(gp_queue_dynamic, &val);

        res = tr_queue_size(gp_queue_dynamic, &size);

        TEST_ASSERT_EQUAL(TR_OK, res);
        TEST_ASSERT_EQUAL_size_t(3u, size);
}

void test_tr_queue_size_returns_err_null_on_null_queue(void)
{
        tr_result_t res = TR_OK;
        size_t size = 0u;

        res = tr_queue_size(NULL, &size);

        TEST_ASSERT_EQUAL(TR_ERR_NULL, res);
}

void test_tr_queue_size_returns_err_null_on_null_output(void)
{
        tr_result_t res = TR_OK;

        res = tr_queue_size(gp_queue_dynamic, NULL);

        TEST_ASSERT_EQUAL(TR_ERR_NULL, res);
}

/* ======================================================================
 * tr_queue_is_empty tests
 * ====================================================================== */

void test_tr_queue_is_empty_returns_true_on_empty_queue(void)
{
        /* local variables */
        tr_result_t res = TR_OK;
        bool is_empty = false;

        res = tr_queue_is_empty(gp_queue_dynamic, &is_empty);

        TEST_ASSERT_EQUAL(TR_OK, res);
        TEST_ASSERT_TRUE(is_empty);
}

void test_tr_queue_is_empty_returns_false_after_enqueue(void)
{
        /* local variables */
        tr_result_t res = TR_OK;
        int val = TEST_VALUE_A;
        bool is_empty = true;

        (void) tr_queue_enqueue(gp_queue_dynamic, &val);

        res = tr_queue_is_empty(gp_queue_dynamic, &is_empty);

        TEST_ASSERT_EQUAL(TR_OK, res);
        TEST_ASSERT_FALSE(is_empty);
}

void test_tr_queue_is_empty_returns_true_after_enqueue_and_dequeue(void)
{
        /* local variables */
        tr_result_t res = TR_OK;
        int val = TEST_VALUE_A;
        bool is_empty = false;

        (void) tr_queue_enqueue(gp_queue_dynamic, &val);
        (void) tr_queue_dequeue(gp_queue_dynamic);

        res = tr_queue_is_empty(gp_queue_dynamic, &is_empty);

        TEST_ASSERT_EQUAL(TR_OK, res);
        TEST_ASSERT_TRUE(is_empty);
}

void test_tr_queue_is_empty_returns_err_null_on_null_queue(void)
{
        tr_result_t res = TR_OK;
        bool is_empty = false;

        res = tr_queue_is_empty(NULL, &is_empty);

        TEST_ASSERT_EQUAL(TR_ERR_NULL, res);
}

void test_tr_queue_is_empty_returns_err_null_on_null_output(void)
{
        tr_result_t res = TR_OK;

        res = tr_queue_is_empty(gp_queue_dynamic, NULL);

        TEST_ASSERT_EQUAL(TR_ERR_NULL, res);
}

/* ======================================================================
 * tr_queue_capacity tests
 * ====================================================================== */

void test_tr_queue_capacity_returns_initial_capacity(void)
{
        /* local variables */
        tr_result_t res = TR_OK;
        size_t capacity = 0u;

        res = tr_queue_capacity(gp_queue_dynamic, &capacity);

        TEST_ASSERT_EQUAL(TR_OK, res);
        TEST_ASSERT_EQUAL_size_t(TEST_CAPACITY, capacity);
}

void test_tr_queue_capacity_grows_on_dynamic_queue(void)
{
        /* local variables */
        tr_result_t res = TR_OK;
        int val = TEST_VALUE_A;
        size_t capacity = 0u;
        size_t i = 0u;

        /* fill beyond capacity to trigger growth */
        for (i = 0u; i <= TEST_CAPACITY; i++)
        {
                (void) tr_queue_enqueue(gp_queue_dynamic, &val);
        }

        res = tr_queue_capacity(gp_queue_dynamic, &capacity);

        TEST_ASSERT_EQUAL(TR_OK, res);
        TEST_ASSERT_GREATER_THAN_size_t(TEST_CAPACITY, capacity);
}

void test_tr_queue_capacity_fixed_does_not_grow(void)
{
        /* local variables */
        tr_result_t res = TR_OK;
        int val = TEST_VALUE_A;
        size_t capacity = 0u;
        size_t i = 0u;

        /* fill to capacity */
        for (i = 0u; i < TEST_CAPACITY; i++)
        {
                (void) tr_queue_enqueue(gp_queue_fixed, &val);
        }

        res = tr_queue_capacity(gp_queue_fixed, &capacity);

        TEST_ASSERT_EQUAL(TR_OK, res);
        TEST_ASSERT_EQUAL_size_t(TEST_CAPACITY, capacity);
}

void test_tr_queue_capacity_returns_err_null_on_null_queue(void)
{
        tr_result_t res = TR_OK;
        size_t capacity = 0u;

        res = tr_queue_capacity(NULL, &capacity);

        TEST_ASSERT_EQUAL(TR_ERR_NULL, res);
}

void test_tr_queue_capacity_returns_err_null_on_null_output(void)
{
        tr_result_t res = TR_OK;

        res = tr_queue_capacity(gp_queue_dynamic, NULL);

        TEST_ASSERT_EQUAL(TR_ERR_NULL, res);
}

/* ======================================================================
 * circular buffer and integration tests
 * ====================================================================== */

void test_tr_queue_fifo_order_is_correct(void)
{
        /* local variables */
        int val_a = TEST_VALUE_A;
        int val_b = TEST_VALUE_B;
        int val_c = TEST_VALUE_C;
        int out = 0;

        (void) tr_queue_enqueue(gp_queue_dynamic, &val_a);
        (void) tr_queue_enqueue(gp_queue_dynamic, &val_b);
        (void) tr_queue_enqueue(gp_queue_dynamic, &val_c);

        /* dequeue should return A, B, C in order */
        (void) tr_queue_front(gp_queue_dynamic, &out);
        TEST_ASSERT_EQUAL_INT(TEST_VALUE_A, out);
        (void) tr_queue_dequeue(gp_queue_dynamic);

        (void) tr_queue_front(gp_queue_dynamic, &out);
        TEST_ASSERT_EQUAL_INT(TEST_VALUE_B, out);
        (void) tr_queue_dequeue(gp_queue_dynamic);

        (void) tr_queue_front(gp_queue_dynamic, &out);
        TEST_ASSERT_EQUAL_INT(TEST_VALUE_C, out);
        (void) tr_queue_dequeue(gp_queue_dynamic);
}

void test_tr_queue_circular_buffer_wraps_correctly(void)
{
        /* local variables */
        int val = 0;
        int out = 0;
        size_t i = 0u;

        /* fill queue to capacity */
        for (i = 0u; i < TEST_CAPACITY; i++)
        {
                val = (int) i;
                (void) tr_queue_enqueue(gp_queue_fixed, &val);
        }

        /* dequeue half — head advances */
        for (i = 0u; i < (TEST_CAPACITY / 2u); i++)
        {
                (void) tr_queue_dequeue(gp_queue_fixed);
        }

        /* enqueue more — tail should wrap around */
        for (i = 0u; i < (TEST_CAPACITY / 2u); i++)
        {
                val = (int) (TEST_CAPACITY + i);
                (void) tr_queue_enqueue(gp_queue_fixed, &val);
        }

        /* verify front is the first undequeued element */
        (void) tr_queue_front(gp_queue_fixed, &out);
        TEST_ASSERT_EQUAL_INT((int) (TEST_CAPACITY / 2u), out);
}

void test_tr_queue_enqueue_dequeue_multiple_types(void)
{
        /* local variables */
        struct queue *p_queue = NULL;
        double val_push = 3.14;
        double val_pop = 0.0;

        (void) tr_queue_create(sizeof(double), TEST_CAPACITY, TR_QUEUE_ARRAY_DYNAMIC, &p_queue);

        (void) tr_queue_enqueue(p_queue, &val_push);
        (void) tr_queue_front(p_queue, &val_pop);

        TEST_ASSERT_EQUAL_DOUBLE(val_push, val_pop);

        (void) tr_queue_destroy(&p_queue);
}

/* ======================================================================
 * tr_queue_ll specific tests
 * ====================================================================== */

void test_tr_queue_create_ll_returns_ok(void)
{
        struct queue *p_queue = NULL;
        tr_result_t res = TR_OK;

        res = tr_queue_create(sizeof(int), TEST_CAPACITY, TR_QUEUE_LL, &p_queue);

        TEST_ASSERT_EQUAL(TR_OK, res);
        TEST_ASSERT_NOT_NULL(p_queue);

        (void) tr_queue_destroy(&p_queue);
}

void test_tr_queue_ll_enqueue_returns_ok(void)
{
        tr_result_t res = TR_OK;
        int val = TEST_VALUE_A;

        res = tr_queue_enqueue(gp_queue_ll, &val);

        TEST_ASSERT_EQUAL(TR_OK, res);
}

void test_tr_queue_ll_enqueue_grows_unbounded(void)
{
        /* local variables */
        tr_result_t res = TR_OK;
        int val = TEST_VALUE_A;
        size_t i = 0u;

        /* push far beyond initial capacity */
        for (i = 0u; i < (TEST_CAPACITY * 4u); i++)
        {
                res = tr_queue_enqueue(gp_queue_ll, &val);
                TEST_ASSERT_EQUAL(TR_OK, res);
        }
}

void test_tr_queue_ll_dequeue_returns_ok(void)
{
        tr_result_t res = TR_OK;
        int val = TEST_VALUE_A;

        (void) tr_queue_enqueue(gp_queue_ll, &val);

        res = tr_queue_dequeue(gp_queue_ll);

        TEST_ASSERT_EQUAL(TR_OK, res);
}

void test_tr_queue_ll_dequeue_returns_err_empty(void)
{
        tr_result_t res = TR_OK;

        res = tr_queue_dequeue(gp_queue_ll);

        TEST_ASSERT_EQUAL(TR_ERR_EMPTY, res);
}

void test_tr_queue_ll_front_returns_correct_value(void)
{
        tr_result_t res = TR_OK;
        int val = TEST_VALUE_A;
        int out = 0;

        (void) tr_queue_enqueue(gp_queue_ll, &val);

        res = tr_queue_front(gp_queue_ll, &out);

        TEST_ASSERT_EQUAL(TR_OK, res);
        TEST_ASSERT_EQUAL_INT(TEST_VALUE_A, out);
}

void test_tr_queue_ll_front_returns_err_empty(void)
{
        tr_result_t res = TR_OK;
        int out = 0;

        res = tr_queue_front(gp_queue_ll, &out);

        TEST_ASSERT_EQUAL(TR_ERR_EMPTY, res);
}

void test_tr_queue_ll_front_returns_first_enqueued_value(void)
{
        /* local variables */
        int val_a = TEST_VALUE_A;
        int val_b = TEST_VALUE_B;
        int out = 0;

        /* enqueue A then B - front should return A (FIFO) */
        (void) tr_queue_enqueue(gp_queue_ll, &val_a);
        (void) tr_queue_enqueue(gp_queue_ll, &val_b);

        (void) tr_queue_front(gp_queue_ll, &out);

        TEST_ASSERT_EQUAL_INT(TEST_VALUE_A, out);
}

void test_tr_queue_ll_size_equals_enqueue_count(void)
{
        /* local variables */
        tr_result_t res = TR_OK;
        int val = TEST_VALUE_A;
        size_t size = 0u;
        size_t i = 0u;

        for (i = 0u; i < TEST_CAPACITY; i++)
        {
                (void) tr_queue_enqueue(gp_queue_ll, &val);
        }

        res = tr_queue_size(gp_queue_ll, &size);

        TEST_ASSERT_EQUAL(TR_OK, res);
        TEST_ASSERT_EQUAL_size_t(TEST_CAPACITY, size);
}

void test_tr_queue_ll_capacity_equals_size(void)
{
        /* local variables */
        tr_result_t res = TR_OK;
        int val = TEST_VALUE_A;
        size_t size = 0u;
        size_t capacity = 0u;
        size_t i = 0u;

        for (i = 0u; i < TEST_CAPACITY; i++)
        {
                (void) tr_queue_enqueue(gp_queue_ll, &val);
        }

        (void) tr_queue_size(gp_queue_ll, &size);
        res = tr_queue_capacity(gp_queue_ll, &capacity);

        TEST_ASSERT_EQUAL(TR_OK, res);
        TEST_ASSERT_EQUAL_size_t(size, capacity);
}

void test_tr_queue_ll_is_empty_returns_true_on_empty(void)
{
        tr_result_t res = TR_OK;
        bool is_empty = false;

        res = tr_queue_is_empty(gp_queue_ll, &is_empty);

        TEST_ASSERT_EQUAL(TR_OK, res);
        TEST_ASSERT_TRUE(is_empty);
}

void test_tr_queue_ll_is_empty_returns_false_after_enqueue(void)
{
        tr_result_t res = TR_OK;
        int val = TEST_VALUE_A;
        bool is_empty = true;

        (void) tr_queue_enqueue(gp_queue_ll, &val);

        res = tr_queue_is_empty(gp_queue_ll, &is_empty);

        TEST_ASSERT_EQUAL(TR_OK, res);
        TEST_ASSERT_FALSE(is_empty);
}

void test_tr_queue_ll_destroy_sets_pointer_to_null(void)
{
        struct queue *p_queue = NULL;
        tr_result_t res = TR_OK;

        (void) tr_queue_create(sizeof(int), TEST_CAPACITY, TR_QUEUE_LL, &p_queue);

        res = tr_queue_destroy(&p_queue);

        TEST_ASSERT_EQUAL(TR_OK, res);
        TEST_ASSERT_NULL(p_queue);
}

void test_tr_queue_ll_destroy_with_elements(void)
{
        /* local variables */
        struct queue *p_queue = NULL;
        tr_result_t res = TR_OK;
        int val = TEST_VALUE_A;
        size_t i = 0u;

        (void) tr_queue_create(sizeof(int), TEST_CAPACITY, TR_QUEUE_LL, &p_queue);

        for (i = 0u; i < TEST_CAPACITY; i++)
        {
                (void) tr_queue_enqueue(p_queue, &val);
        }

        res = tr_queue_destroy(&p_queue);

        TEST_ASSERT_EQUAL(TR_OK, res);
        TEST_ASSERT_NULL(p_queue);
}

void test_tr_queue_ll_fifo_order_is_correct(void)
{
        /* local variables */
        int val_a = TEST_VALUE_A;
        int val_b = TEST_VALUE_B;
        int val_c = TEST_VALUE_C;
        int out = 0;

        (void) tr_queue_enqueue(gp_queue_ll, &val_a);
        (void) tr_queue_enqueue(gp_queue_ll, &val_b);
        (void) tr_queue_enqueue(gp_queue_ll, &val_c);

        /* dequeue should return A, B, C in order */
        (void) tr_queue_front(gp_queue_ll, &out);
        TEST_ASSERT_EQUAL_INT(TEST_VALUE_A, out);
        (void) tr_queue_dequeue(gp_queue_ll);

        (void) tr_queue_front(gp_queue_ll, &out);
        TEST_ASSERT_EQUAL_INT(TEST_VALUE_B, out);
        (void) tr_queue_dequeue(gp_queue_ll);

        (void) tr_queue_front(gp_queue_ll, &out);
        TEST_ASSERT_EQUAL_INT(TEST_VALUE_C, out);
        (void) tr_queue_dequeue(gp_queue_ll);
}

/* ======================================================================
 * main - test runner
 * ====================================================================== */

int main(void)
{
        UNITY_BEGIN();

        /* tr_queue_create */
        RUN_TEST(test_tr_queue_create_dynamic_returns_ok);
        RUN_TEST(test_tr_queue_create_fixed_returns_ok);
        RUN_TEST(test_tr_queue_create_returns_err_null_on_null_output);
        RUN_TEST(test_tr_queue_create_returns_err_invalid_on_zero_datatype_size);
        RUN_TEST(test_tr_queue_create_returns_err_invalid_on_zero_capacity);
        RUN_TEST(test_tr_queue_create_returns_err_invalid_on_unknown_type);

        /* tr_queue_destroy */
        RUN_TEST(test_tr_queue_destroy_sets_pointer_to_null);
        RUN_TEST(test_tr_queue_destroy_returns_err_null_on_null_pointer);
        RUN_TEST(test_tr_queue_destroy_returns_err_null_on_null_queue);

        /* tr_queue_enqueue */
        RUN_TEST(test_tr_queue_enqueue_returns_ok_on_valid_enqueue);
        RUN_TEST(test_tr_queue_enqueue_returns_err_null_on_null_queue);
        RUN_TEST(test_tr_queue_enqueue_returns_err_null_on_null_data);
        RUN_TEST(test_tr_queue_enqueue_fixed_returns_err_full_when_at_capacity);
        RUN_TEST(test_tr_queue_enqueue_dynamic_grows_beyond_initial_capacity);

        /* tr_queue_dequeue */
        RUN_TEST(test_tr_queue_dequeue_returns_ok_on_valid_dequeue);
        RUN_TEST(test_tr_queue_dequeue_returns_err_null_on_null_queue);
        RUN_TEST(test_tr_queue_dequeue_returns_err_empty_on_empty_queue);
        RUN_TEST(test_tr_queue_dequeue_decrements_size);

        /* tr_queue_front */
        RUN_TEST(test_tr_queue_front_returns_correct_value);
        RUN_TEST(test_tr_queue_front_does_not_remove_element);
        RUN_TEST(test_tr_queue_front_returns_first_enqueued_value);
        RUN_TEST(test_tr_queue_front_returns_err_null_on_null_queue);
        RUN_TEST(test_tr_queue_front_returns_err_null_on_null_output);
        RUN_TEST(test_tr_queue_front_returns_err_empty_on_empty_queue);

        /* tr_queue_size */
        RUN_TEST(test_tr_queue_size_returns_zero_on_empty_queue);
        RUN_TEST(test_tr_queue_size_increments_on_enqueue);
        RUN_TEST(test_tr_queue_size_returns_err_null_on_null_queue);
        RUN_TEST(test_tr_queue_size_returns_err_null_on_null_output);

        /* tr_queue_is_empty */
        RUN_TEST(test_tr_queue_is_empty_returns_true_on_empty_queue);
        RUN_TEST(test_tr_queue_is_empty_returns_false_after_enqueue);
        RUN_TEST(test_tr_queue_is_empty_returns_true_after_enqueue_and_dequeue);
        RUN_TEST(test_tr_queue_is_empty_returns_err_null_on_null_queue);
        RUN_TEST(test_tr_queue_is_empty_returns_err_null_on_null_output);

        /* tr_queue_capacity */
        RUN_TEST(test_tr_queue_capacity_returns_initial_capacity);
        RUN_TEST(test_tr_queue_capacity_grows_on_dynamic_queue);
        RUN_TEST(test_tr_queue_capacity_fixed_does_not_grow);
        RUN_TEST(test_tr_queue_capacity_returns_err_null_on_null_queue);
        RUN_TEST(test_tr_queue_capacity_returns_err_null_on_null_output);

        /* circular buffer and integration */
        RUN_TEST(test_tr_queue_fifo_order_is_correct);
        RUN_TEST(test_tr_queue_circular_buffer_wraps_correctly);
        RUN_TEST(test_tr_queue_enqueue_dequeue_multiple_types);

        /* tr_queue_ll specific */
        RUN_TEST(test_tr_queue_create_ll_returns_ok);
        RUN_TEST(test_tr_queue_ll_enqueue_returns_ok);
        RUN_TEST(test_tr_queue_ll_enqueue_grows_unbounded);
        RUN_TEST(test_tr_queue_ll_dequeue_returns_ok);
        RUN_TEST(test_tr_queue_ll_dequeue_returns_err_empty);
        RUN_TEST(test_tr_queue_ll_front_returns_correct_value);
        RUN_TEST(test_tr_queue_ll_front_returns_err_empty);
        RUN_TEST(test_tr_queue_ll_front_returns_first_enqueued_value);
        RUN_TEST(test_tr_queue_ll_size_equals_enqueue_count);
        RUN_TEST(test_tr_queue_ll_capacity_equals_size);
        RUN_TEST(test_tr_queue_ll_is_empty_returns_true_on_empty);
        RUN_TEST(test_tr_queue_ll_is_empty_returns_false_after_enqueue);
        RUN_TEST(test_tr_queue_ll_destroy_sets_pointer_to_null);
        RUN_TEST(test_tr_queue_ll_destroy_with_elements);
        RUN_TEST(test_tr_queue_ll_fifo_order_is_correct);

        return UNITY_END();
}
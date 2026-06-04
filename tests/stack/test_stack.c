/*******************************************************************************************************
 * NAME: test_stack.c
 *
 * PURPOSE: Unit tests for the stack data structure
 *
 * GLOBAL VARIABLES: None
 *
 * DEVELOPMENT HISTORY:
 *
 * Date          Author          Change Id       Release         Description Of Change
 * ----------    --------------- ---------       -------         -----------------------------------
 * 04-06-2026    Tiago Rodrigues                       1         File preparation
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

/* 1.2 C system headers */
#include <stddef.h> /* size_t   */

/* 1.3 C++ standard library headers */

/* 1.4 other libraries' headers */
#include "unity.h"

/* 1.5 project headers */
#include "tr_datastructures/tr_stack.h"

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
static struct stack *gp_stack_dynamic = NULL;
static struct stack *gp_stack_fixed = NULL;
/*****************************************************/

/* 6 function prototypes */
/*****************************************************/
void setUp(void);
void tearDown(void);


void test_tr_stack_create_dynamic_returns_ok(void);
void test_tr_stack_create_fixed_returns_ok(void);
void test_tr_stack_create_returns_err_null_on_null_output(void);
void test_tr_stack_create_returns_err_invalid_on_zero_datatype_size(void);
void test_tr_stack_create_returns_err_invalid_on_zero_capacity(void);
void test_tr_stack_create_returns_err_invalid_on_unknown_type(void);
void test_tr_stack_destroy_sets_pointer_to_null(void);
void test_tr_stack_destroy_returns_err_null_on_null_pointer(void);
void test_tr_stack_destroy_returns_err_null_on_null_stack(void);
void test_tr_stack_push_returns_ok_on_valid_push(void);
void test_tr_stack_push_returns_err_null_on_null_stack(void);
void test_tr_stack_push_returns_err_null_on_null_data(void);
void test_tr_stack_push_fixed_returns_err_full_when_at_capacity(void);
void test_tr_stack_push_dynamic_grows_beyond_initial_capacity(void);
void test_tr_stack_pop_returns_ok_on_valid_pop(void);
void test_tr_stack_pop_returns_err_null_on_null_stack(void);
void test_tr_stack_pop_returns_err_empty_on_empty_stack(void);
void test_tr_stack_pop_decrements_size(void);
void test_tr_stack_top_returns_correct_value(void);
void test_tr_stack_top_does_not_remove_element(void);
void test_tr_stack_top_returns_last_pushed_value(void);
void test_tr_stack_top_returns_err_null_on_null_stack(void);
void test_tr_stack_top_returns_err_null_on_null_output(void);
void test_tr_stack_top_returns_err_empty_on_empty_stack(void);
void test_tr_stack_size_returns_zero_on_empty_stack(void);
void test_tr_stack_size_increments_on_push(void);
void test_tr_stack_size_returns_err_null_on_null_stack(void);
void test_tr_stack_size_returns_err_null_on_null_output(void);
void test_tr_stack_is_empty_returns_true_on_empty_stack(void);
void test_tr_stack_is_empty_returns_false_after_push(void);
void test_tr_stack_is_empty_returns_true_after_push_and_pop(void);
void test_tr_stack_is_empty_returns_err_null_on_null_stack(void);
void test_tr_stack_is_empty_returns_err_null_on_null_output(void);
void test_tr_stack_capacity_returns_initial_capacity(void);
void test_tr_stack_capacity_grows_on_dynamic_stack(void);
void test_tr_stack_capacity_fixed_does_not_grow(void);
void test_tr_stack_capacity_returns_err_null_on_null_stack(void);
void test_tr_stack_capacity_returns_err_null_on_null_output(void);
void test_tr_stack_lifo_order_is_correct(void);
void test_tr_stack_push_pop_multiple_types(void);



/*****************************************************/

/*******************************************************************************************************
 *
 * FUNCTION NAME: setUp
 *
 * PURPOSE: Creates a fresh stack before each test
 *          Called automatically by Unity before every test function
 *
 * ARGUMENTS: none
 *
 * RETURNS: void
 *
 *******************************************************************************************************/
void setUp(void)
{
        tr_stack_create(sizeof(int), TEST_CAPACITY, TR_STACK_ARRAY_DYNAMIC, &gp_stack_dynamic);
        tr_stack_create(sizeof(int), TEST_CAPACITY, TR_STACK_ARRAY_FIXED, &gp_stack_fixed);
}

/*******************************************************************************************************
 *
 * FUNCTION NAME: tearDown
 *
 * PURPOSE: Destroys stacks after each test
 *          Called automatically by Unity after every test function
 *
 * ARGUMENTS: none
 *
 * RETURNS: void
 *
 *******************************************************************************************************/
void tearDown(void)
{
        if (NULL != gp_stack_dynamic)
        {
                tr_stack_destroy(&gp_stack_dynamic);
                gp_stack_dynamic = NULL;
        }

        if (NULL != gp_stack_fixed)
        {
                tr_stack_destroy(&gp_stack_fixed);
                gp_stack_fixed = NULL;
        }
}

/* ======================================================================
 * tr_stack_create tests
 * ====================================================================== */

void test_tr_stack_create_dynamic_returns_ok(void)
{
        struct stack *p_stack = NULL;
        tr_result_t res = TR_OK;

        res = tr_stack_create(sizeof(int), TEST_CAPACITY, TR_STACK_ARRAY_DYNAMIC, &p_stack);

        TEST_ASSERT_EQUAL(TR_OK, res);
        TEST_ASSERT_NOT_NULL(p_stack);

        tr_stack_destroy(&p_stack);
}

void test_tr_stack_create_fixed_returns_ok(void)
{
        struct stack *p_stack = NULL;
        tr_result_t res = TR_OK;

        res = tr_stack_create(sizeof(int), TEST_CAPACITY, TR_STACK_ARRAY_FIXED, &p_stack);

        TEST_ASSERT_EQUAL(TR_OK, res);
        TEST_ASSERT_NOT_NULL(p_stack);

        tr_stack_destroy(&p_stack);
}

void test_tr_stack_create_returns_err_null_on_null_output(void)
{
        tr_result_t res = TR_OK;

        res = tr_stack_create(sizeof(int), TEST_CAPACITY, TR_STACK_ARRAY_DYNAMIC, NULL);

        TEST_ASSERT_EQUAL(TR_ERR_NULL, res);
}

void test_tr_stack_create_returns_err_invalid_on_zero_datatype_size(void)
{
        struct stack *p_stack = NULL;
        tr_result_t res = TR_OK;

        res = tr_stack_create(0u, TEST_CAPACITY, TR_STACK_ARRAY_DYNAMIC, &p_stack);

        TEST_ASSERT_EQUAL(TR_ERR_INVALID, res);
        TEST_ASSERT_NULL(p_stack);
}

void test_tr_stack_create_returns_err_invalid_on_zero_capacity(void)
{
        struct stack *p_stack = NULL;
        tr_result_t res = TR_OK;

        res = tr_stack_create(sizeof(int), 0u, TR_STACK_ARRAY_DYNAMIC, &p_stack);

        TEST_ASSERT_EQUAL(TR_ERR_INVALID, res);
        TEST_ASSERT_NULL(p_stack);
}

void test_tr_stack_create_returns_err_invalid_on_unknown_type(void)
{
        struct stack *p_stack = NULL;
        tr_result_t res = TR_OK;

        res = tr_stack_create(sizeof(int), TEST_CAPACITY, (tr_stack_type_t) 99, &p_stack);

        TEST_ASSERT_EQUAL(TR_ERR_INVALID, res);
        TEST_ASSERT_NULL(p_stack);
}

/* ======================================================================
 * tr_stack_destroy tests
 * ====================================================================== */

void test_tr_stack_destroy_sets_pointer_to_null(void)
{
        struct stack *p_stack = NULL;
        tr_result_t res = TR_OK;

        tr_stack_create(sizeof(int), TEST_CAPACITY, TR_STACK_ARRAY_DYNAMIC, &p_stack);

        res = tr_stack_destroy(&p_stack);

        TEST_ASSERT_EQUAL(TR_OK, res);
        TEST_ASSERT_NULL(p_stack);
}

void test_tr_stack_destroy_returns_err_null_on_null_pointer(void)
{
        tr_result_t res = TR_OK;

        res = tr_stack_destroy(NULL);

        TEST_ASSERT_EQUAL(TR_ERR_NULL, res);
}

void test_tr_stack_destroy_returns_err_null_on_null_stack(void)
{
        struct stack *p_stack = NULL;
        tr_result_t res = TR_OK;

        res = tr_stack_destroy(&p_stack);

        TEST_ASSERT_EQUAL(TR_ERR_NULL, res);
}

/* ======================================================================
 * tr_stack_push tests
 * ====================================================================== */

void test_tr_stack_push_returns_ok_on_valid_push(void)
{
        tr_result_t res = TR_OK;
        int val = TEST_VALUE_A;

        res = tr_stack_push(gp_stack_dynamic, &val);

        TEST_ASSERT_EQUAL(TR_OK, res);
}

void test_tr_stack_push_returns_err_null_on_null_stack(void)
{
        tr_result_t res = TR_OK;
        int val = TEST_VALUE_A;

        res = tr_stack_push(NULL, &val);

        TEST_ASSERT_EQUAL(TR_ERR_NULL, res);
}

void test_tr_stack_push_returns_err_null_on_null_data(void)
{
        tr_result_t res = TR_OK;

        res = tr_stack_push(gp_stack_dynamic, NULL);

        TEST_ASSERT_EQUAL(TR_ERR_NULL, res);
}

void test_tr_stack_push_fixed_returns_err_full_when_at_capacity(void)
{
        /* local variables */
        tr_result_t res = TR_OK;
        int val = TEST_VALUE_A;
        size_t i = 0u;

        /* fill stack to capacity */
        for (i = 0u; i < TEST_CAPACITY; i++)
        {
                res = tr_stack_push(gp_stack_fixed, &val);
                TEST_ASSERT_EQUAL(TR_OK, res);
        }

        /* one more push should fail */
        res = tr_stack_push(gp_stack_fixed, &val);

        TEST_ASSERT_EQUAL(TR_ERR_FULL, res);
}

void test_tr_stack_push_dynamic_grows_beyond_initial_capacity(void)
{
        /* local variables */
        tr_result_t res = TR_OK;
        int val = TEST_VALUE_A;
        size_t i = 0u;

        /* push beyond initial capacity */
        for (i = 0u; i < (TEST_CAPACITY * 2u); i++)
        {
                res = tr_stack_push(gp_stack_dynamic, &val);
                TEST_ASSERT_EQUAL(TR_OK, res);
        }
}

/* ======================================================================
 * tr_stack_pop tests
 * ====================================================================== */

void test_tr_stack_pop_returns_ok_on_valid_pop(void)
{
        tr_result_t res = TR_OK;
        int val = TEST_VALUE_A;

        tr_stack_push(gp_stack_dynamic, &val);

        res = tr_stack_pop(gp_stack_dynamic);

        TEST_ASSERT_EQUAL(TR_OK, res);
}

void test_tr_stack_pop_returns_err_null_on_null_stack(void)
{
        tr_result_t res = TR_OK;

        res = tr_stack_pop(NULL);

        TEST_ASSERT_EQUAL(TR_ERR_NULL, res);
}

void test_tr_stack_pop_returns_err_empty_on_empty_stack(void)
{
        tr_result_t res = TR_OK;

        res = tr_stack_pop(gp_stack_dynamic);

        TEST_ASSERT_EQUAL(TR_ERR_EMPTY, res);
}

void test_tr_stack_pop_decrements_size(void)
{
        /* local variables */
        tr_result_t res = TR_OK;
        int val = TEST_VALUE_A;
        size_t size = 0u;

        tr_stack_push(gp_stack_dynamic, &val);
        tr_stack_push(gp_stack_dynamic, &val);

        tr_stack_pop(gp_stack_dynamic);

        res = tr_stack_size(gp_stack_dynamic, &size);

        TEST_ASSERT_EQUAL(TR_OK, res);
        TEST_ASSERT_EQUAL_size_t(1u, size);
}

/* ======================================================================
 * tr_stack_top tests
 * ====================================================================== */

void test_tr_stack_top_returns_correct_value(void)
{
        /* local variables */
        tr_result_t res = TR_OK;
        int val = TEST_VALUE_A;
        int out = 0;

        tr_stack_push(gp_stack_dynamic, &val);

        res = tr_stack_top(gp_stack_dynamic, &out);

        TEST_ASSERT_EQUAL(TR_OK, res);
        TEST_ASSERT_EQUAL_INT(TEST_VALUE_A, out);
}

void test_tr_stack_top_does_not_remove_element(void)
{
        /* local variables */
        tr_result_t res = TR_OK;
        int val = TEST_VALUE_A;
        int out = 0;
        size_t size = 0u;

        tr_stack_push(gp_stack_dynamic, &val);
        tr_stack_top(gp_stack_dynamic, &out);

        res = tr_stack_size(gp_stack_dynamic, &size);

        TEST_ASSERT_EQUAL(TR_OK, res);
        TEST_ASSERT_EQUAL_size_t(1u, size);
}

void test_tr_stack_top_returns_last_pushed_value(void)
{
        /* local variables */
        int val_a = TEST_VALUE_A;
        int val_b = TEST_VALUE_B;
        int out = 0;

        tr_stack_push(gp_stack_dynamic, &val_a);
        tr_stack_push(gp_stack_dynamic, &val_b);

        tr_stack_top(gp_stack_dynamic, &out);

        TEST_ASSERT_EQUAL_INT(TEST_VALUE_B, out);
}

void test_tr_stack_top_returns_err_null_on_null_stack(void)
{
        tr_result_t res = TR_OK;
        int out = 0;

        res = tr_stack_top(NULL, &out);

        TEST_ASSERT_EQUAL(TR_ERR_NULL, res);
}

void test_tr_stack_top_returns_err_null_on_null_output(void)
{
        tr_result_t res = TR_OK;
        int val = TEST_VALUE_A;

        tr_stack_push(gp_stack_dynamic, &val);

        res = tr_stack_top(gp_stack_dynamic, NULL);

        TEST_ASSERT_EQUAL(TR_ERR_NULL, res);
}

void test_tr_stack_top_returns_err_empty_on_empty_stack(void)
{
        tr_result_t res = TR_OK;
        int out = 0;

        res = tr_stack_top(gp_stack_dynamic, &out);

        TEST_ASSERT_EQUAL(TR_ERR_EMPTY, res);
}

/* ======================================================================
 * tr_stack_size tests
 * ====================================================================== */

void test_tr_stack_size_returns_zero_on_empty_stack(void)
{
        /* local variables */
        tr_result_t res = TR_OK;
        size_t size = 0u;

        res = tr_stack_size(gp_stack_dynamic, &size);

        TEST_ASSERT_EQUAL(TR_OK, res);
        TEST_ASSERT_EQUAL_size_t(0u, size);
}

void test_tr_stack_size_increments_on_push(void)
{
        /* local variables */
        tr_result_t res = TR_OK;
        int val = TEST_VALUE_A;
        size_t size = 0u;

        tr_stack_push(gp_stack_dynamic, &val);
        tr_stack_push(gp_stack_dynamic, &val);
        tr_stack_push(gp_stack_dynamic, &val);

        res = tr_stack_size(gp_stack_dynamic, &size);

        TEST_ASSERT_EQUAL(TR_OK, res);
        TEST_ASSERT_EQUAL_size_t(3u, size);
}

void test_tr_stack_size_returns_err_null_on_null_stack(void)
{
        tr_result_t res = TR_OK;
        size_t size = 0u;

        res = tr_stack_size(NULL, &size);

        TEST_ASSERT_EQUAL(TR_ERR_NULL, res);
}

void test_tr_stack_size_returns_err_null_on_null_output(void)
{
        tr_result_t res = TR_OK;

        res = tr_stack_size(gp_stack_dynamic, NULL);

        TEST_ASSERT_EQUAL(TR_ERR_NULL, res);
}

/* ======================================================================
 * tr_stack_is_empty tests
 * ====================================================================== */

void test_tr_stack_is_empty_returns_true_on_empty_stack(void)
{
        /* local variables */
        tr_result_t res = TR_OK;
        bool is_empty = false;

        res = tr_stack_is_empty(gp_stack_dynamic, &is_empty);

        TEST_ASSERT_EQUAL(TR_OK, res);
        TEST_ASSERT_TRUE(is_empty);
}

void test_tr_stack_is_empty_returns_false_after_push(void)
{
        /* local variables */
        tr_result_t res = TR_OK;
        int val = TEST_VALUE_A;
        bool is_empty = true;

        tr_stack_push(gp_stack_dynamic, &val);

        res = tr_stack_is_empty(gp_stack_dynamic, &is_empty);

        TEST_ASSERT_EQUAL(TR_OK, res);
        TEST_ASSERT_FALSE(is_empty);
}

void test_tr_stack_is_empty_returns_true_after_push_and_pop(void)
{
        /* local variables */
        tr_result_t res = TR_OK;
        int val = TEST_VALUE_A;
        bool is_empty = false;

        tr_stack_push(gp_stack_dynamic, &val);
        tr_stack_pop(gp_stack_dynamic);

        res = tr_stack_is_empty(gp_stack_dynamic, &is_empty);

        TEST_ASSERT_EQUAL(TR_OK, res);
        TEST_ASSERT_TRUE(is_empty);
}

void test_tr_stack_is_empty_returns_err_null_on_null_stack(void)
{
        tr_result_t res = TR_OK;
        bool is_empty = false;

        res = tr_stack_is_empty(NULL, &is_empty);

        TEST_ASSERT_EQUAL(TR_ERR_NULL, res);
}

void test_tr_stack_is_empty_returns_err_null_on_null_output(void)
{
        tr_result_t res = TR_OK;

        res = tr_stack_is_empty(gp_stack_dynamic, NULL);

        TEST_ASSERT_EQUAL(TR_ERR_NULL, res);
}

/* ======================================================================
 * tr_stack_capacity tests
 * ====================================================================== */

void test_tr_stack_capacity_returns_initial_capacity(void)
{
        /* local variables */
        tr_result_t res = TR_OK;
        size_t capacity = 0u;

        res = tr_stack_capacity(gp_stack_dynamic, &capacity);

        TEST_ASSERT_EQUAL(TR_OK, res);
        TEST_ASSERT_EQUAL_size_t(TEST_CAPACITY, capacity);
}

void test_tr_stack_capacity_grows_on_dynamic_stack(void)
{
        /* local variables */
        tr_result_t res = TR_OK;
        int val = TEST_VALUE_A;
        size_t capacity = 0u;
        size_t i = 0u;

        /* fill beyond capacity to trigger growth */
        for (i = 0u; i <= TEST_CAPACITY; i++)
        {
                tr_stack_push(gp_stack_dynamic, &val);
        }

        res = tr_stack_capacity(gp_stack_dynamic, &capacity);

        TEST_ASSERT_EQUAL(TR_OK, res);
        TEST_ASSERT_GREATER_THAN_size_t(TEST_CAPACITY, capacity);
}

void test_tr_stack_capacity_fixed_does_not_grow(void)
{
        /* local variables */
        tr_result_t res = TR_OK;
        int val = TEST_VALUE_A;
        size_t capacity = 0u;
        size_t i = 0u;

        /* fill to capacity */
        for (i = 0u; i < TEST_CAPACITY; i++)
        {
                tr_stack_push(gp_stack_fixed, &val);
        }

        res = tr_stack_capacity(gp_stack_fixed, &capacity);

        TEST_ASSERT_EQUAL(TR_OK, res);
        TEST_ASSERT_EQUAL_size_t(TEST_CAPACITY, capacity);
}

void test_tr_stack_capacity_returns_err_null_on_null_stack(void)
{
        tr_result_t res = TR_OK;
        size_t capacity = 0u;

        res = tr_stack_capacity(NULL, &capacity);

        TEST_ASSERT_EQUAL(TR_ERR_NULL, res);
}

void test_tr_stack_capacity_returns_err_null_on_null_output(void)
{
        tr_result_t res = TR_OK;

        res = tr_stack_capacity(gp_stack_dynamic, NULL);

        TEST_ASSERT_EQUAL(TR_ERR_NULL, res);
}

/* ======================================================================
 * integration tests
 * ====================================================================== */

void test_tr_stack_lifo_order_is_correct(void)
{
        /* local variables */
        int val_a = TEST_VALUE_A;
        int val_b = TEST_VALUE_B;
        int val_c = TEST_VALUE_C;
        int out = 0;

        tr_stack_push(gp_stack_dynamic, &val_a);
        tr_stack_push(gp_stack_dynamic, &val_b);
        tr_stack_push(gp_stack_dynamic, &val_c);

        tr_stack_top(gp_stack_dynamic, &out);
        TEST_ASSERT_EQUAL_INT(TEST_VALUE_C, out);
        tr_stack_pop(gp_stack_dynamic);

        tr_stack_top(gp_stack_dynamic, &out);
        TEST_ASSERT_EQUAL_INT(TEST_VALUE_B, out);
        tr_stack_pop(gp_stack_dynamic);

        tr_stack_top(gp_stack_dynamic, &out);
        TEST_ASSERT_EQUAL_INT(TEST_VALUE_A, out);
        tr_stack_pop(gp_stack_dynamic);
}

void test_tr_stack_push_pop_multiple_types(void)
{
        /* local variables */
        struct stack *p_stack = NULL;
        double val_push = 3.14;
        double val_pop = 0.0;

        tr_stack_create(sizeof(double), TEST_CAPACITY, TR_STACK_ARRAY_DYNAMIC, &p_stack);

        tr_stack_push(p_stack, &val_push);
        tr_stack_top(p_stack, &val_pop);

        TEST_ASSERT_EQUAL_DOUBLE(val_push, val_pop);

        tr_stack_destroy(&p_stack);
}

/* ======================================================================
 * main - test runner
 * ====================================================================== */

int main(void)
{
        UNITY_BEGIN();

        /* tr_stack_create */
        RUN_TEST(test_tr_stack_create_dynamic_returns_ok);
        RUN_TEST(test_tr_stack_create_fixed_returns_ok);
        RUN_TEST(test_tr_stack_create_returns_err_null_on_null_output);
        RUN_TEST(test_tr_stack_create_returns_err_invalid_on_zero_datatype_size);
        RUN_TEST(test_tr_stack_create_returns_err_invalid_on_zero_capacity);
        RUN_TEST(test_tr_stack_create_returns_err_invalid_on_unknown_type);

        /* tr_stack_destroy */
        RUN_TEST(test_tr_stack_destroy_sets_pointer_to_null);
        RUN_TEST(test_tr_stack_destroy_returns_err_null_on_null_pointer);
        RUN_TEST(test_tr_stack_destroy_returns_err_null_on_null_stack);

        /* tr_stack_push */
        RUN_TEST(test_tr_stack_push_returns_ok_on_valid_push);
        RUN_TEST(test_tr_stack_push_returns_err_null_on_null_stack);
        RUN_TEST(test_tr_stack_push_returns_err_null_on_null_data);
        RUN_TEST(test_tr_stack_push_fixed_returns_err_full_when_at_capacity);
        RUN_TEST(test_tr_stack_push_dynamic_grows_beyond_initial_capacity);

        /* tr_stack_pop */
        RUN_TEST(test_tr_stack_pop_returns_ok_on_valid_pop);
        RUN_TEST(test_tr_stack_pop_returns_err_null_on_null_stack);
        RUN_TEST(test_tr_stack_pop_returns_err_empty_on_empty_stack);
        RUN_TEST(test_tr_stack_pop_decrements_size);

        /* tr_stack_top */
        RUN_TEST(test_tr_stack_top_returns_correct_value);
        RUN_TEST(test_tr_stack_top_does_not_remove_element);
        RUN_TEST(test_tr_stack_top_returns_last_pushed_value);
        RUN_TEST(test_tr_stack_top_returns_err_null_on_null_stack);
        RUN_TEST(test_tr_stack_top_returns_err_null_on_null_output);
        RUN_TEST(test_tr_stack_top_returns_err_empty_on_empty_stack);

        /* tr_stack_size */
        RUN_TEST(test_tr_stack_size_returns_zero_on_empty_stack);
        RUN_TEST(test_tr_stack_size_increments_on_push);
        RUN_TEST(test_tr_stack_size_returns_err_null_on_null_stack);
        RUN_TEST(test_tr_stack_size_returns_err_null_on_null_output);

        /* tr_stack_is_empty */
        RUN_TEST(test_tr_stack_is_empty_returns_true_on_empty_stack);
        RUN_TEST(test_tr_stack_is_empty_returns_false_after_push);
        RUN_TEST(test_tr_stack_is_empty_returns_true_after_push_and_pop);
        RUN_TEST(test_tr_stack_is_empty_returns_err_null_on_null_stack);
        RUN_TEST(test_tr_stack_is_empty_returns_err_null_on_null_output);

        /* tr_stack_capacity */
        RUN_TEST(test_tr_stack_capacity_returns_initial_capacity);
        RUN_TEST(test_tr_stack_capacity_grows_on_dynamic_stack);
        RUN_TEST(test_tr_stack_capacity_fixed_does_not_grow);
        RUN_TEST(test_tr_stack_capacity_returns_err_null_on_null_stack);
        RUN_TEST(test_tr_stack_capacity_returns_err_null_on_null_output);

        /* integration */
        RUN_TEST(test_tr_stack_lifo_order_is_correct);
        RUN_TEST(test_tr_stack_push_pop_multiple_types);

        return UNITY_END();
}
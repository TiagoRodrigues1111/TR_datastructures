/*******************************************************************************************************
 * NAME: example_stack.c
 *
 * PURPOSE: Demonstrates usage of the tr_datastructures stack API
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

/* 1.1 related header */
/* none */

/* 1.2 C system headers */
#include <stdio.h>  /* printf, fprintf  */
#include <string.h> /* strlen           */

/* 1.3 C++ standard library headers */
/* none */

/* 1.4 other libraries' headers */
/* none */

/* 1.5 project headers */
#include "tr_datastructures.h"
//#include "tr_datastructures/tr_version.h"

/*****************************************************/

/* 2 defines */
/*****************************************************/
#define EXAMPLE_CAPACITY 8u
#define EXAMPLE_FIXED_CAP 4u
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
static void example_reverse_array(void);
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
 * PURPOSE: Demonstrates basic usage of a dynamic array stack
 *          Shows create, push, top, pop, size, is_empty, destroy
 *
 * ARGUMENTS: none
 *
 * RETURNS: void
 *
 *******************************************************************************************************/
static void example_basic_dynamic(void)
{
        /* local variables */
        struct stack *p_stack = NULL; /* stack handle             */
        tr_result_t res = TR_OK;      /* operation result         */
        int val = 0;                  /* value to push/pop        */
        int top = 0;                  /* value at top of stack    */
        size_t size = 0u;             /* current stack size       */
        bool empty = false;           /* is stack empty           */
        int i = 0;                    /* loop counter             */

        print_separator("Basic Dynamic Stack");

        /* --- create --- */
        res = tr_stack_create(sizeof(int), EXAMPLE_CAPACITY, TR_STACK_ARRAY_DYNAMIC, &p_stack);
        if (TR_OK != res)
        {
                fprintf(stderr, "Failed to create stack: %d\n", res);
                return;
        }
        printf("Created dynamic stack with initial capacity %u\n", EXAMPLE_CAPACITY);

        /* --- push --- */
        printf("\nPushing values 1 to 5:\n");
        for (i = 1; i <= 5; i++)
        {
                val = i;
                res = tr_stack_push(p_stack, &val);
                if (TR_OK != res)
                {
                        fprintf(stderr, "Push failed: %d\n", res);
                        (void) tr_stack_destroy(&p_stack);
                        return;
                }
                printf("  pushed: %d\n", val);
        }

        /* --- size --- */
        (void) tr_stack_size(p_stack, &size);
        printf("\nStack size: %lu\n", (unsigned long)size);

        /* --- top --- */
        (void) tr_stack_top(p_stack, &top);
        printf("Top element: %d\n", top);

        /* --- pop --- */
        printf("\nPopping all elements:\n");
        while (true)
        {
                (void) tr_stack_is_empty(p_stack, &empty);
                if (empty)
                {
                        break;
                }

                (void) tr_stack_top(p_stack, &top);
                (void) tr_stack_pop(p_stack);
                printf("  popped: %d\n", top);
        }

        /* --- is_empty --- */
        (void) tr_stack_is_empty(p_stack, &empty);
        printf("\nStack is empty: %s\n", empty ? "true" : "false");

        /* --- destroy --- */
        (void) tr_stack_destroy(&p_stack);
        printf("Stack destroyed. Handle is now: %s\n", (NULL == p_stack) ? "NULL" : "not NULL");
}

/*******************************************************************************************************
 *
 * FUNCTION NAME: example_basic_fixed
 *
 * PURPOSE: Demonstrates basic usage of a fixed capacity array stack
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
        struct stack *p_stack = NULL; /* stack handle             */
        tr_result_t res = TR_OK;      /* operation result         */
        int val = 0;                  /* value to push            */
        size_t capacity = 0u;         /* stack capacity           */
        int i = 0;                    /* loop counter             */

        print_separator("Basic Fixed Stack");

        /* --- create --- */
        res = tr_stack_create(sizeof(int), EXAMPLE_FIXED_CAP, TR_STACK_ARRAY_FIXED, &p_stack);
        if (TR_OK != res)
        {
                fprintf(stderr, "Failed to create stack: %d\n", res);
                return;
        }

        (void) tr_stack_capacity(p_stack, &capacity);
        printf("Created fixed stack with capacity %lu\n", (unsigned long)capacity);

        /* --- push to capacity --- */
        printf("\nFilling stack to capacity:\n");
        for (i = 1; i <= (int) EXAMPLE_FIXED_CAP; i++)
        {
                val = i * 10;
                res = tr_stack_push(p_stack, &val);
                printf("  push %d: %s\n", val, (TR_OK == res) ? "OK" : "FAILED");
        }

        /* --- push beyond capacity --- */
        printf("\nAttempting to push beyond capacity:\n");
        val = 999;
        res = tr_stack_push(p_stack, &val);
        printf("  push %d: %s (expected TR_ERR_FULL)\n",
               val,
               (TR_ERR_FULL == res) ? "TR_ERR_FULL" : "unexpected result");

        /* --- destroy --- */
        (void) tr_stack_destroy(&p_stack);
        printf("\nStack destroyed\n");
}

/*******************************************************************************************************
 *
 * FUNCTION NAME: example_reverse_array
 *
 * PURPOSE: Demonstrates a real use case - reversing an array using a stack
 *          Pushes all elements then pops them into a new array
 *
 * ARGUMENTS: none
 *
 * RETURNS: void
 *
 *******************************************************************************************************/
static void example_reverse_array(void)
{
        /* local variables */
        struct stack *p_stack = NULL; /* stack handle             */
        tr_result_t res = TR_OK;      /* operation result         */
        int original[] = { 1, 2, 3, 4, 5, 6, 7, 8 };
        int reversed[8] = { 0 }; /* reversed output          */
        size_t length = 0u;      /* array length             */
        size_t i = 0u;           /* loop counter             */
        int val = 0;             /* current value            */

        print_separator("Real Use Case: Reversing an Array");

        length = sizeof(original) / sizeof(original[0]);

        /* --- print original --- */
        printf("Original array: ");
        for (i = 0u; i < length; i++)
        {
                printf("%d ", original[i]);
        }
        printf("\n");

        /* --- create stack --- */
        res = tr_stack_create(sizeof(int), length, TR_STACK_ARRAY_DYNAMIC, &p_stack);
        if (TR_OK != res)
        {
                fprintf(stderr, "Failed to create stack: %d\n", res);
                return;
        }

        /* --- push all elements --- */
        for (i = 0u; i < length; i++)
        {
                res = tr_stack_push(p_stack, &original[i]);
                if (TR_OK != res)
                {
                        fprintf(stderr, "Push failed at index %lu: %d\n", (unsigned long)i, res);
                        (void) tr_stack_destroy(&p_stack);
                        return;
                }
        }

        /* --- pop into reversed array --- */
        for (i = 0u; i < length; i++)
        {
                (void) tr_stack_top(p_stack, &val);
                (void) tr_stack_pop(p_stack);
                reversed[i] = val;
        }

        /* --- print reversed --- */
        printf("Reversed array: ");
        for (i = 0u; i < length; i++)
        {
                printf("%d ", reversed[i]);
        }
        printf("\n");

        /* --- destroy --- */
        (void) tr_stack_destroy(&p_stack);
        printf("Stack destroyed\n");
}

/*******************************************************************************************************
 *
 * FUNCTION NAME: main
 *
 * PURPOSE: Entry point - runs all stack examples
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

        printf("TR_datastructures v%s - Stack Examples\n", TR_VERSION_STRING);

        example_basic_dynamic();
        example_basic_fixed();
        example_reverse_array();

        printf("\n==================================================\n");
        printf(" Done\n");
        printf("==================================================\n");

        return (0);
}
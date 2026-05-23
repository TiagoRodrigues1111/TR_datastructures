/*******************************************************************************
 * NAME: tr_stack.h
 *
 * PURPOSE: declaration of the stack functions
 *
 * GLOBAL VARIABLES:
 *
 * Variable              Type                    Description
 * --------              ----                    -----------
 *
 * DEVELOPMENT HISTORY:
 *
 * Date  Author  Change Id       Release         Description Of Change
 * ----  ------  ---------       -------         ---------------------
 *
 *******************************************************************************/

#ifndef TR_STACK_H
#define TR_STACK_H

/* 0 copyright/licensing */
/*******************************************************************************************************
 *
 * 2026 Tiago Filipe Rodrigues tiagorodrigues1590@hotmail.com
 *
 ********************************************************************************************************/

/* Ensure C++ compatibility */
#ifdef __cplusplus
extern "C"
{
#endif

        /* 1 includes */
        /*****************************************************/

        /* 2 defines */
        /*****************************************************/

        /* 3 external declarations */
        /*****************************************************/

        /* 4 typedefs */
        /*****************************************************/

        /* 5 global variable declarations */
        /*****************************************************/

        /* 6 function prototypes */
        /*****************************************************/

        struct tr_datastructures_stack;

        /******************************************************************
         *
         * FUNCTION NAME: tr_datastructures_stack_create
         *
         * PURPOSE: Allocates the needed memory for the stack wanted
         *
         * ARGUMENTS:
         *
         * ARGUMENT 	        TYPE	                I/O	DESCRIPTION
         * --------              ----                    ---     ------------
         * size_of_elements      const size_t            I       byte size of datatype to place in
         * the stack elements_to_allocate  const size_t            I       number of elements to
         * allocate for the stack
         *
         * RETURNS: struct tr_datastructures_stack*
         *
         *
         *
         *****************************************************************/
        struct tr_datastructures_stack *
        tr_datastructures_stack_create(const size_t size_of_elements,
                                       const size_t elements_to_allocate);

        /******************************************************************
         *
         * FUNCTION NAME: tr_datastructures_stack_free
         *
         * PURPOSE: Frees the memory allocated for the stack
         *
         * ARGUMENTS:
         *
         * ARGUMENT 	TYPE	                                I/O	DESCRIPTION
         * --------	-------------	                        ---	--------------------------
         * stack_id      struct tr_datastructures_stack *	I	pointer to the memory
         * position of the stack to free
         *
         *
         * RETURNS: void
         *
         *
         *
         *****************************************************************/
        bool tr_datastructures_stack_free(struct tr_datastructures_stack *stack_id);

        /******************************************************************
         *
         * FUNCTION NAME: stack_push
         *
         * PURPOSE: Pushes an element to the top of the stack
         *
         * ARGUMENTS:
         *
         * ARGUMENT 	        TYPE	                                I/O	DESCRIPTION
         * --------	        -------------	                        ---
         *	-------------------------- stack_id              struct tr_datastructures_stack *
         *	I	pointer to the memory position of the stack to push to element_to_push
         * const void*	                        I	pointer to the memory position of the data
         * to push into the stack
         *
         *
         * RETURNS: bool
         *
         *
         *
         *****************************************************************/
        bool tr_datastructures_stack_push(struct tr_datastructures_stack *stack_id,
                                          const void *element_to_push);

        /******************************************************************
         *
         * FUNCTION NAME: tr_datastructures_stack_pop
         *
         * PURPOSE: Removes one position from the top of the stack
         *
         * ARGUMENTS:
         *
         * ARGUMENT 	TYPE	                                I/O	DESCRIPTION
         * --------	-------------	                        ---	--------------------------
         * stack_id      struct tr_datastructures_stack *	I	pointer to the memory
         * position of the stack to pop from
         *
         *
         * RETURNS: bool
         *
         *
         *
         *****************************************************************/
        bool tr_datastructures_stack_pop(struct tr_datastructures_stack *stack_id);

        /******************************************************************
         *
         * FUNCTION NAME: tr_datastructures_stack_top
         *
         * PURPOSE: Returns the memory position of the element that is currently on the top of the
         * stack
         *
         * ARGUMENTS:
         *
         * ARGUMENT 	        TYPE	                                I/O	DESCRIPTION
         * --------              ----                                    ---     ------------
         * stack_id              const struct tr_datastructures_stack*	I	pointer to the
         * memory position of the stack to check element_at_top        void*             O
         *	pointer to the memory position where to copy the data at the top of the stack
         *
         *
         * RETURNS: bool ( true if successful, false otherwise)
         *
         *
         *
         *****************************************************************/
        bool tr_datastructures_stack_top(const struct tr_datastructures_stack *stack_id,
                                         void *element_at_top);

        /******************************************************************
         *
         * FUNCTION NAME: tr_datastructures_stack_is_empty
         *
         * PURPOSE: Checks if the stack is empty or not
         *
         * ARGUMENTS:
         *
         * ARGUMENT 	TYPE	                                I/O	DESCRIPTION
         * --------	-------------	                        ---	--------------------------
         * stack_id      const struct tr_datastructures_stack*	I	pointer to the memory
         * position of the stack
         *
         *
         * RETURNS: bool ( true or false)
         *
         *
         *
         *****************************************************************/
        bool tr_datastructures_stack_is_empty(const struct tr_datastructures_stack *stack_id);

        /******************************************************************
         *
         * FUNCTION NAME: tr_datastructures_stack_size
         *
         * PURPOSE: Will return the current element count in the stack
         *
         * ARGUMENTS:
         *
         * ARGUMENT 	TYPE	                                I/O	DESCRIPTION
         * --------	-------------	                        ---	--------------------------
         * stack_id      const struct tr_datastructures_stack*	I	pointer to the memory
         * position of the stack
         *
         *
         * RETURNS: size_t (size of the stack)
         *
         *
         *
         *****************************************************************/
        size_t tr_datastructures_stack_size(const struct tr_datastructures_stack *stack_id);

#ifdef __cplusplus
}
#endif

#endif /* TR_STACK_H */

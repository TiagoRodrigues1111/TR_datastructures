/*******************************************************************************
 * NAME: tr_queue.h
 *
 * PURPOSE: declaration of the queue functions
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

#ifndef TR_QUEUE_H
#define TR_QUEUE_H

/* 0 copyright/licensing */
/************************************
 *
 * 2026 Tiago Filipe Rodrigues tiagorodrigues1590@hotmail.com
 *
 *************************************/

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

        struct tr_datastructures_queue *
        tr_datastructures_queue_create(const size_t size_of_elements,
                                       const size_t elements_to_allocate);
                                       
        bool tr_datastructures_queue_free();

        bool tr_datastructures_queue_push();
        bool tr_datastructures_queue_pop();

        bool tr_datastructures_queue_front();
        bool tr_datastructures_queue_back();

        bool tr_datastructures_queue_is_empty();
        size_t tr_datastructures_queue_size();

#ifdef __cplusplus
}
#endif

#endif /* TR_QUEUE_H */
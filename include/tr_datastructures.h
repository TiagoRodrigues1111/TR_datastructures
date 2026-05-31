/*******************************************************************************************************
 * NAME: tr_datastructures.h
 *
 * PURPOSE: Umbrella header for the tr_datastructures library
 *          Including this file provides access to the full public API
 *
 * GLOBAL VARIABLES: None
 *
 * DEVELOPMENT HISTORY:
 *
 * Date          Author          Change Id       Release         Description Of Change
 * ----------    --------------- ---------       -------         -----------------------------------
 * 24-05-2026    Tiago Rodrigues                       1         File preparation
 *
 *******************************************************************************************************/
#ifndef TR_DATASTRUCTURES_H
#define TR_DATASTRUCTURES_H

/* 0 copyright/licensing */
/*******************************************************************************************************
 *
 * This is free and unencumbered software released into the public domain (Unlicense).
 *
 ********************************************************************************************************/

#ifdef __cplusplus
extern "C"
{
#endif

/* 1 includes */
/*****************************************************/

/* -- version & types -- */
#include "tr_datastructures/tr_version.h"
#include "tr_datastructures/tr_types.h"
#include "tr_datastructures/tr_export.h"
#include "tr_datastructures/tr_result.h"  


/* -- data structures -- */
#include "tr_datastructures/tr_stack.h"
/* #include "tr_datastructures/tr_queue.h" */


#ifdef __cplusplus
}
#endif

#endif /* TR_DATASTRUCTURES_H */
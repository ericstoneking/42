/*    This file is distributed with 42,                               */
/*    the (mostly harmless) spacecraft dynamics simulation            */
/*    created by Eric Stoneking of NASA Goddard Space Flight Center   */

/*    Copyright 2010 United States Government                         */
/*    as represented by the Administrator                             */
/*    of the National Aeronautics and Space Administration.           */

/*    No copyright is claimed in the United States                    */
/*    under Title 17, U.S. Code.                                      */

/*    All Other Rights Reserved.                                      */


#ifndef __42FSW_H__
#define __42FSW_H__

#include <stdlib.h>
#include <math.h>
#include "fswdefines.h"
#include "fswtypes.h"
#include "mathkit.h"
#include "dcmkit.h"
#include "fswkit.h"
#include "iokit.h"

void AcFsw(struct AcType *AC);

/*
** #ifdef __cplusplus
** namespace _42 {
** using namespace Kit;
** #endif
*/

/* Disable extern keyword to declare globals */
#ifdef DECLARE_GLOBALS
   #define extern
#endif

#undef extern

/*
** #ifdef __cplusplus
** }
** #endif
*/

#endif /* __42FSW_H__ */

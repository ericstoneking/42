/*    This file is distributed with 42,                               */
/*    the (mostly harmless) spacecraft dynamics simulation            */
/*    created by Eric Stoneking of NASA Goddard Space Flight Center   */

/*    Copyright 2010 United States Government                         */
/*    as represented by the Administrator                             */
/*    of the National Aeronautics and Space Administration.           */

/*    No copyright is claimed in the United States                    */
/*    under Title 17, U.S. Code.                                      */

/*    All Other Rights Reserved.                                      */


#ifndef __AC_H__
#define __AC_H__

#include <stdlib.h>
#include <math.h>
#include "AcTypes.h"
#include "mathkit.h"
#include "dcmkit.h"
#include "fswkit.h"
#include "iokit.h"
#include "timekit.h"

#ifndef TRUE
   #define TRUE (1)
#endif

#ifndef FALSE
   #define FALSE (0)
#endif

#ifndef PI
   #define PI (3.141592653589793)
#endif

#ifndef TWOPI
   #define TWOPI (6.283185307179586)
#endif

#ifndef HALFPI
   #define HALFPI (1.570796326794897)
#endif

#ifndef D2R
   #define D2R (1.74532925199433E-2)
#endif


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

#endif /* __AC_H__ */

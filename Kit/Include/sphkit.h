/*    This file is distributed with 42,                               */
/*    the (mostly harmless) spacecraft dynamics simulation            */
/*    created by Eric Stoneking of NASA Goddard Space Flight Center   */

/*    Copyright 2010 United States Government                         */
/*    as represented by the Administrator                             */
/*    of the National Aeronautics and Space Administration.           */

/*    No copyright is claimed in the United States                    */
/*    under Title 17, U.S. Code.                                      */

/*    All Other Rights Reserved.                                      */


#ifndef __SPHKIT_H__
#define __SPHKIT_H__

#include <math.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
/* #include <GLUT/glut.h> */

#ifdef __cplusplus
   extern "C" {
#endif

/* GLuint KernelToTexTag(void); */
double CubicKernel(double r, double h, long Ndim);
double CubicGradKernel(double r, double h, long Ndim);
double Poly6Kernel(double r, double h, long Ndim);
double Poly6GradKernel(double r, double h, long Ndim);
double SpikyKernel(double r, double h, long Ndim);
double SpikyGradKernel(double r, double h, long Ndim);
double Unwrap(double dx, double Span);

#ifdef __cplusplus
   } /* extern "C" */
#endif

#endif /* __SPHKIT_H__ */

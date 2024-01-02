/*    This file is distributed with 42,                               */
/*    the (mostly harmless) spacecraft dynamics simulation            */
/*    created by Eric Stoneking of NASA Goddard Space Flight Center   */

/*    Copyright 2010 United States Government                         */
/*    as represented by the Administrator                             */
/*    of the National Aeronautics and Space Administration.           */

/*    No copyright is claimed in the United States                    */
/*    under Title 17, U.S. Code.                                      */

/*    All Other Rights Reserved.                                      */


#ifndef __SIGKIT_H__
#define __SIGKIT_H__

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

/*
** #ifdef __cplusplus
** namespace Kit {
** #endif
*/

struct RandomProcessType {
   /* For UniformRandom */
   long Index;
   long PreviousValue;
   long LookupTable[32];

   /* For GaussianRandom */
   long HaveSavedValue;
   double SavedValue;
};

struct FilterType {
   long Ns;
   double *A,*B,*x,*y;
   double dxmax;
   double ymin;
};

struct DelayType {
   long N;
   double *CircBuffer;
   long Idx;
};

struct RandomProcessType *CreateRandomProcess(long Seed);
void DestroyRandomProcess(struct RandomProcessType *RP);
double UniformRandom(struct RandomProcessType *RP);
double GaussianRandom(struct RandomProcessType *RP);
double PRN2D(long x, long y);
double PRN3D(long x, long y, long z);
double Step(double a,double x);
double Clamp(double a,double b,double x);
double RampStep(double a,double b,double x);
double CubicStep(double a, double b, double x);
struct FilterType *CreateGeneralFilter(long Ns, double *A, double *B,
   double dxmax, double ymin);
struct FilterType *CreateFirstOrderLowpassFilter(double w, double T,
   double dxmax, double ymin);
struct FilterType *CreateFirstOrderHighpassFilter(double w, double T,
   double dxmax, double ymin);
struct FilterType *CreateSecondOrderLowpassFilter(double w, double z,
   double T, double dxmax, double ymin);
struct FilterType *CreateSecondOrderHighpassFilter(double w, double z,
   double T, double dxmax, double ymin);
void DestroyFilter(struct FilterType *F);
double GeneralFilter(struct FilterType *F, double x);
double FirstOrderLowpassFilter(struct FilterType *F, double x);
double FirstOrderHighpassFilter(struct FilterType *F, double x);
double SecondOrderLowpassFilter(struct FilterType *F, double x);
double SecondOrderHighpassFilter(struct FilterType *F, double x);
struct DelayType *CreateDelay(double DelayTime, double DT);
struct DelayType *ResizeDelay(struct DelayType *OldD, double DelayTime, double DT);
double Delay(struct DelayType *D, double x);

/*
** #ifdef __cplusplus
** }
** #endif
*/
#endif /* __SIGKIT_H__ */


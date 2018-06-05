/*    This file is distributed with 42,                               */
/*    the (mostly harmless) spacecraft dynamics simulation            */
/*    created by Eric Stoneking of NASA Goddard Space Flight Center   */

/*    Copyright 2010 United States Government                         */
/*    as represented by the Administrator                             */
/*    of the National Aeronautics and Space Administration.           */

/*    No copyright is claimed in the United States                    */
/*    under Title 17, U.S. Code.                                      */

/*    All Other Rights Reserved.                                      */


#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "mathkit.h"
#include "dcmkit.h"

#ifndef __FSWKIT_H__
#define __FSWKIT_H__


/*
** #ifdef __cplusplus
** namespace Kit {
** #endif
*/

struct KFMeasType {
   long Ny; /* 1, 2, or 3 */

   double *y;   /* Ny by 1 */
   double **Rv; /* Ny by Ny */
   double **H;  /* Ny by Nx */
   double **L;  /* Nx by Ny */
   
   /* Workspace variables */
   double **HP; /* Ny by Nx */
   double *Hx; /* Ny by 1 */
   double **HPHtRv; /* Ny by Ny */
   double **HPHtRvInv; /* Ny by Ny */
};

struct KalmanFilterType {
   long Nx;
   long Nu;
   long Nw;
   long Nm;

   double *x; /* Nx by 1 */
   double *u; /* Nu by 1 */
   double **Phi; /* Nx by Nx */
   double **Gam; /* Nx by Nu */
   double **Gamw; /* Nx by Nw */

   double **P; /* Covariance after measurement (P is for Plus), Nx by Nx */
   double **M; /* Covariance before measurement (M is for Minus), Nx by Nx */

   double **Rw; /* Nw by Nw */
   
   /* Measurement structures, Nm by 1 */
   struct KFMeasType *Meas;

   /* Workspace variables */
   double *PhiX; /* Nx by 1 */
   double *GamU; /* Nx by 1 */
   double **PhiP; /* Nx by Nx */
   double **GRwGt; /* Gamw*Rw*GamwT, Nx by Nx */
};


void FindPDGains(double I, double w, double z, double *Kr, double *Kp);
double Limit(double x,double min, double max);
double SpinGainCostFunction(double p[2],double CostParm[2]);
void FindSpinnerGains(double J, double It, double Tc, double OrbPer,
                      double alpha,
                      double *SpinRate, double *Knute, double *Kprec);
void TRIAD(double Va[3], double Wa[3], double Vb[3], double Wb[3],
           double CBA[3][3]);
void Quest(long n, double *Weight,double **Ref,double **Meas,
   double qmr[4]);
void FilterQuest(long n, double *Weight, double **Ref, double **Meas,
                 double dt, double memory,double wbn[3], double qmr[4]);
void PointGimbalToTarget(long Seq, double CGiBi[3][3],
                         double CBoGo[3][3],
                         double tvi[3], double bvo[3],
                         double GimAngCmd[3]);
void CollisionAvoidanceLaw(double x[3], double v[3],
   double xg[3], double xa[3], double Ra, double vmax, double amax,
   double wc, double zc, double a[3]);
double BangBangSettle(double x, double v, double w0,
                      double amax, double vmax);
double RampCoastGlide(double x, double v, double w0,
                      double amax, double vmax);
double RateControl(double v, double amax, double w0);
void VectorRampCoastGlide(double Xvec[3], double Vvec[3], double w0,
                      double amax, double vmax, double Avec[3]);
double SolarBeta(double svn[3], double psn[3], double vsn[3]);
double ThrusterSelection(double **A, double *f, double *t,
   double tmax, long m, long n, long OffPulse);
void StateEstimator(double **PHI, double **GAMMA, double **H,
   double **L, double *u, double *y, double *x,
   long Nx, long Nu, long Ny);
void UDUFactor(double **P, double **U, long N);
void UDMeasUpdate(double *x, double **U, double y, double *H,
   double Rv, long Ns);
void UDTimeUpdate(double *x, double **U, double **phi, double **gam,
   double *y, double *Rw,long Ns, long Nw);
void AllocKalmanFilterMeasurement(struct KFMeasType *M,long Nx, long Ny);
struct KalmanFilterType *CreateKalmanFilter(long Nx, long Nu, long Nw, long Nm);
void PopulateKalmanFilterWorkspace(struct KalmanFilterType *KF);
void KalmanFilterMeasUpdate(struct KalmanFilterType *KF, struct KFMeasType *M);
void KalmanFilterTimeUpdate(struct KalmanFilterType *KF);
double CMGLaw4x1DOF(double Tcmd[3], double Axis[4][3], double Gim[4][3],
   double h[4], double AngRateCmd[4]);

/*
** #ifdef __cplusplus
** }
** #endif
*/

#endif /* __FSWKIT_H__ */

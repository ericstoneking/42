/*    This file is distributed with 42,                               */
/*    the (mostly harmless) spacecraft dynamics simulation            */
/*    created by Eric Stoneking of NASA Goddard Space Flight Center   */

/*    Copyright 2010 United States Government                         */
/*    as represented by the Administrator                             */
/*    of the National Aeronautics and Space Administration.           */

/*    No copyright is claimed in the United States                    */
/*    under Title 17, U.S. Code.                                      */

/*    All Other Rights Reserved.                                      */


#ifndef __MATHKIT_H__
#define __MATHKIT_H__

#include <stdlib.h>
#include <math.h>
#include <stdio.h>

/*
** #ifdef __cplusplus
** namespace Kit {
** #endif
*/

#ifndef __MINGW32__
   #ifdef WIN32
      #ifndef isnan(x)
         #define isnan(x) ((x) != (x))
      #endif
   #endif
#endif

double signum(double x);
double sinc(double x);
void MxM(double A[3][3], double B[3][3], double C[3][3]);
void MxMT(double A[3][3], double B[3][3], double C[3][3]);
void MTxM (double A[3][3], double B[3][3], double C[3][3]);
void MTxMT (double A[3][3], double B[3][3], double C[3][3]);
void VxM (double V[3], double M[3][3], double W[3]);
void MxV (double M[3][3], double V[3], double W[3]);
void VxMT (double V[3], double M[3][3], double W[3]);
void MTxV (double M[3][3], double V[3], double W[3]);
void SxV(double S, double V[3], double W[3]);
void SxM(double S, double A[3][3], double B[3][3]);
void MINV4(double A[4][4], double B[4][4]);
void MINV3(double A[3][3], double B[3][3]);
void MINV2(double A[2][2], double B[2][2]);
void PINV4x3(double A[4][3], double Aplus[3][4]);
void MT(double A[3][3], double B[3][3]);
double VoV(double A[3], double B[3]);
void VxV(double A[3], double B[3], double C[3]);
void vxMov(double w[3], double M[3][3], double wxMow[3]);
double MAGV(double V[3]);
double UNITV(double V[3]);
double CopyUnitV(double V[3], double W[3]);
void V2CrossM(double V[3], double M[3][3]);
void V2DoubleCrossM(double V[3], double M[3][3]);
void VcrossM(double V[3], double M[3][3], double A[3][3]);
void VcrossMT(double V[3], double M[3][3], double A[3][3]);
void QxQ(double A[4], double B[4], double C[4]);
void QTxQ(double A[4], double B[4], double C[4]);
void QxQT(double A[4], double B[4], double C[4]);
void VxQ(double Va[3],double QAB[4],double Vb[3]);
void QxV(double QAB[4],double Vb[3],double Va[3]);
void QTxV(double QAB[4],double Va[3],double Vb[3]);
void UNITQ(double Q[4]);
void RECTIFYQ(double Q[4]);
void PerpBasis(double A[3], double B[3], double C[3]);
double fact(long n);
double oddfact(long n);
void Legendre(long N, long M, double x,
              double P[19][19], double sdP[19][19]);
void SphericalHarmonics(long N, long M, double r, double phi,
        double theta, double Re, double K,
        double C[19][19], double S[19][19], double gradV[3]);
void MxMG(double **A, double **B, double **C,
          long N, long K, long M);
void MxMTG(double **A, double **B, double **C,
           long N, long K, long M);
void MTxMG(double **A, double **B, double **C,
          long N, long K, long M);
void MxVG(double **M, double *v, double *w, long n, long m);
void SxMG(double s, double **A, double **B,
          long N, long M);
void MINVG(double **A, double **AI, long N);
void FastMINV6(double A[6][6], double AI[6][6], long N);
void PINVG(double **A, double **Ai, long n, long m);
double **CreateMatrix(long n, long m);
void DestroyMatrix(double **A, long n);
void LINSOLVE(double **A, double *x, double *b, long n);
void CholeskySolve(double **A, double *x, double *b, long n);
void ConjGradSolve(double **A, double *x, double *b, long n,
   double errtol, long maxiter);
void Bairstow(long n, double *a, double Tol, double *Real, double *Imag);
double Amoeba(long N, double *P,
            double CostFunction(double *p, double *Parm),
            double *CostParm, double scale, double Tol);
void FindNormal(double V1[3], double V2[3], double V3[3], double N[3]);
double LinInterp(double *X, double *Y, double x, long n);
void SphereInterp(double q1[4], double q2[4], double u, double q[4]);
double CubicInterp1D(double f0, double f1, double x);
double CubicInterp2D(double f00, double f10, double f01, double f11,
                    double x, double y);
double CubicInterp3D(double f000, double f100, double f010, double f110,
                    double f001, double f101, double f011, double f111,
                    double x, double y, double z);
double DistanceToLine(double LineEnd1[3], double LineEnd2[3],
   double Point[3], double VecToLine[3]);
long ProjectPointOntoPoly(double Point[3], double DirVec[3],
   double **Vtx, long Nvtx, double ProjPoint[3], double *Distance);
long ProjectPointOntoTriangle(double A[3], double B[3], double C[3],
      double DirVec[3], double Pt[3], double ProjPt[3], double Bary[4]);
double CubicSpline(double x, double X[4], double Y[4]);
void ChebyPolys(double u, long n, double T[20], double U[20]); 
void ChebyInterp(double T[20], double U[20], double Coef[20], long n, 
   double *P, double *dPdu);
void FindChebyCoefs(double *u, double *P, long Nu, long Nc, double Coef[20]);
void VecToLngLat(double A[3], double *lng, double *lat);

/*
** #ifdef __cplusplus
** }
** #endif
*/

#endif /* __MATHKIT_H__ */

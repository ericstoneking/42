/*    This file is distributed with 42,                               */
/*    the (mostly harmless) spacecraft dynamics simulation            */
/*    created by Eric Stoneking of NASA Goddard Space Flight Center   */

/*    Copyright 2010 United States Government                         */
/*    as represented by the Administrator                             */
/*    of the National Aeronautics and Space Administration.           */

/*    No copyright is claimed in the United States                    */
/*    under Title 17, U.S. Code.                                      */

/*    All Other Rights Reserved.                                      */


#include "mathkit.h"

/* #ifdef __cplusplus
** namespace Kit {
** #endif
*/

/**********************************************************************/
double signum(double x)
{
      return(x>=0 ? 1.0 : -1.0);
}
/**********************************************************************/
/* sinc(x) = sin(x)/x                                                 */
/*  Series expansion: sinc(x) = 1 - x^2/3! + x^4/5! - x^6/7!...       */
/*  Enough terms kept to be within 2E-10 for x in [-pi:pi]            */
double sinc(double x)
{
      double x2;

      if(x < -3.14159265358979 || x > 3.14159265358979) {
         return(sin(x)/x);
      }
      else {
         x2 = x*x;
         return(1.0-x2/6.0*(1.0-x2/20.0*(1.0-x2/42.0*(1.0-x2/72.0*
               (1.0-x2/110.0*(1.0-x2/156.0*(1.0-x2/210.0*
               (1.0-x2/272.0*(1.0-x2/342.0)))))))));
      }
}
/**********************************************************************/
/*   3x3 Matrix Product                                               */
void MxM(double A[3][3], double B[3][3], double C[3][3])
{

      C[0][0]=A[0][0]*B[0][0]+A[0][1]*B[1][0]+A[0][2]*B[2][0];
      C[0][1]=A[0][0]*B[0][1]+A[0][1]*B[1][1]+A[0][2]*B[2][1];
      C[0][2]=A[0][0]*B[0][2]+A[0][1]*B[1][2]+A[0][2]*B[2][2];
      C[1][0]=A[1][0]*B[0][0]+A[1][1]*B[1][0]+A[1][2]*B[2][0];
      C[1][1]=A[1][0]*B[0][1]+A[1][1]*B[1][1]+A[1][2]*B[2][1];
      C[1][2]=A[1][0]*B[0][2]+A[1][1]*B[1][2]+A[1][2]*B[2][2];
      C[2][0]=A[2][0]*B[0][0]+A[2][1]*B[1][0]+A[2][2]*B[2][0];
      C[2][1]=A[2][0]*B[0][1]+A[2][1]*B[1][1]+A[2][2]*B[2][1];
      C[2][2]=A[2][0]*B[0][2]+A[2][1]*B[1][2]+A[2][2]*B[2][2];
}
/**********************************************************************/
/* 3x3 Matrix times Transpose of Matrix                               */
void MxMT(double A[3][3], double B[3][3], double C[3][3])
{
      C[0][0]=A[0][0]*B[0][0]+A[0][1]*B[0][1]+A[0][2]*B[0][2];
      C[0][1]=A[0][0]*B[1][0]+A[0][1]*B[1][1]+A[0][2]*B[1][2];
      C[0][2]=A[0][0]*B[2][0]+A[0][1]*B[2][1]+A[0][2]*B[2][2];
      C[1][0]=A[1][0]*B[0][0]+A[1][1]*B[0][1]+A[1][2]*B[0][2];
      C[1][1]=A[1][0]*B[1][0]+A[1][1]*B[1][1]+A[1][2]*B[1][2];
      C[1][2]=A[1][0]*B[2][0]+A[1][1]*B[2][1]+A[1][2]*B[2][2];
      C[2][0]=A[2][0]*B[0][0]+A[2][1]*B[0][1]+A[2][2]*B[0][2];
      C[2][1]=A[2][0]*B[1][0]+A[2][1]*B[1][1]+A[2][2]*B[1][2];
      C[2][2]=A[2][0]*B[2][0]+A[2][1]*B[2][1]+A[2][2]*B[2][2];
}
/**********************************************************************/
/*  3x3 Transpose of Matrix times Matrix                              */
void  MTxM (double A[3][3], double B[3][3], double C[3][3])
{
      C[0][0]=A[0][0]*B[0][0]+A[1][0]*B[1][0]+A[2][0]*B[2][0];
      C[0][1]=A[0][0]*B[0][1]+A[1][0]*B[1][1]+A[2][0]*B[2][1];
      C[0][2]=A[0][0]*B[0][2]+A[1][0]*B[1][2]+A[2][0]*B[2][2];
      C[1][0]=A[0][1]*B[0][0]+A[1][1]*B[1][0]+A[2][1]*B[2][0];
      C[1][1]=A[0][1]*B[0][1]+A[1][1]*B[1][1]+A[2][1]*B[2][1];
      C[1][2]=A[0][1]*B[0][2]+A[1][1]*B[1][2]+A[2][1]*B[2][2];
      C[2][0]=A[0][2]*B[0][0]+A[1][2]*B[1][0]+A[2][2]*B[2][0];
      C[2][1]=A[0][2]*B[0][1]+A[1][2]*B[1][1]+A[2][2]*B[2][1];
      C[2][2]=A[0][2]*B[0][2]+A[1][2]*B[1][2]+A[2][2]*B[2][2];
}
/**********************************************************************/
/*  3x3 Transpose of Matrix times Transpose of Matrix                 */
void MTxMT (double A[3][3], double B[3][3], double C[3][3])
{
   C[0][0]=A[0][0]*B[0][0]+A[1][0]*B[0][1]+A[2][0]*B[0][2];
   C[0][1]=A[0][0]*B[1][0]+A[1][0]*B[1][1]+A[2][0]*B[1][2];
   C[0][2]=A[0][0]*B[2][0]+A[1][0]*B[2][1]+A[2][0]*B[2][2];
   C[1][0]=A[0][1]*B[0][0]+A[1][1]*B[0][1]+A[2][1]*B[0][2];
   C[1][1]=A[0][1]*B[1][0]+A[1][1]*B[1][1]+A[2][1]*B[1][2];
   C[1][2]=A[0][1]*B[2][0]+A[1][1]*B[2][1]+A[2][1]*B[2][2];
   C[2][0]=A[0][2]*B[0][0]+A[1][2]*B[0][1]+A[2][2]*B[0][2];
   C[2][1]=A[0][2]*B[1][0]+A[1][2]*B[1][1]+A[2][2]*B[1][2];
   C[2][2]=A[0][2]*B[2][0]+A[1][2]*B[2][1]+A[2][2]*B[2][2];
}
/**********************************************************************/
/*  1x3 Vector times 3x3 Matrix                                       */
void VxM (double V[3], double M[3][3], double W[3])
{
      W[0]=V[0]*M[0][0]+V[1]*M[1][0]+V[2]*M[2][0];
      W[1]=V[0]*M[0][1]+V[1]*M[1][1]+V[2]*M[2][1];
      W[2]=V[0]*M[0][2]+V[1]*M[1][2]+V[2]*M[2][2];
}
/**********************************************************************/
/*  3x3 Matrix times 3x1 Vector                                       */
void MxV (double M[3][3], double V[3], double W[3])
{
      W[0]=V[0]*M[0][0]+V[1]*M[0][1]+V[2]*M[0][2];
      W[1]=V[0]*M[1][0]+V[1]*M[1][1]+V[2]*M[1][2];
      W[2]=V[0]*M[2][0]+V[1]*M[2][1]+V[2]*M[2][2];
}
/**********************************************************************/
/*  1x3 Vector times transpose of 3x3 Matrix                          */
void VxMT (double V[3], double M[3][3], double W[3])
{
      W[0]=V[0]*M[0][0]+V[1]*M[0][1]+V[2]*M[0][2];
      W[1]=V[0]*M[1][0]+V[1]*M[1][1]+V[2]*M[1][2];
      W[2]=V[0]*M[2][0]+V[1]*M[2][1]+V[2]*M[2][2];
}
/**********************************************************************/
/*  Transpose of 3x3 Matrix times 3x1 Vector                          */
void MTxV (double M[3][3], double V[3], double W[3])
{
      W[0] = M[0][0]*V[0] + M[1][0]*V[1] + M[2][0]*V[2];
      W[1] = M[0][1]*V[0] + M[1][1]*V[1] + M[2][1]*V[2];
      W[2] = M[0][2]*V[0] + M[1][2]*V[1] + M[2][2]*V[2];
}
/**********************************************************************/
/*  Scalar times 3x1 Vector                                           */
void SxV(double S, double V[3], double W[3])
{
      W[0] = S*V[0];
      W[1] = S*V[1];
      W[2] = S*V[2];
}
/**********************************************************************/
/*  Scalar times 3x3 Matrix                                           */
void SxM(double S, double A[3][3], double B[3][3])
{
      B[0][0] = S*A[0][0];
      B[0][1] = S*A[0][1];
      B[0][2] = S*A[0][2];
      B[1][0] = S*A[1][0];
      B[1][1] = S*A[1][1];
      B[1][2] = S*A[1][2];
      B[2][0] = S*A[2][0];
      B[2][1] = S*A[2][1];
      B[2][2] = S*A[2][2];
}
/******************************************************************************/
/* Inverse of a 4x4 Matrix                                                    */
void MINV4(double A[4][4],double B[4][4])
{
      double DET = 0.0;
      long r,s,i,j,k,x,y,z;
      
      for(r=0;r<4;r++) {
         for(s=0;s<4;s++) {
            i = (r+1)%4;
            j = (r+2)%4;
            k = (r+3)%4;
            x = (s+1)%4;
            y = (s+2)%4;
            z = (s+3)%4;
            B[s][r] =  A[i][x]*(A[j][y]*A[k][z]-A[j][z]*A[k][y])
                      +A[i][y]*(A[j][z]*A[k][x]-A[j][x]*A[k][z])
                      +A[i][z]*(A[j][x]*A[k][y]-A[j][y]*A[k][x]);
            if ((r+s)%2 == 1) B[s][r] = -B[s][r];
         }
      }
      for(r=0;r<4;r++) DET += A[0][r]*B[r][0];

      if (DET == 0.0) {
         printf("Attempted inversion of singular matrix in MINV4.  Bailing out.\n");
         exit(1);
      }
      else {
         for(r=0;r<4;r++) {
            for(s=0;s<4;s++) {
               B[r][s] /= DET;
            }
         }
      }
}
/******************************************************************************/
/*  Inverse of a 3x3 Matrix                                                   */
void MINV3(double A[3][3], double B[3][3])
{
      double DET;

      DET=A[0][0]*A[1][1]*A[2][2]+A[0][1]*A[1][2]*A[2][0]
         +A[0][2]*A[1][0]*A[2][1]-A[2][0]*A[1][1]*A[0][2]
         -A[2][1]*A[1][2]*A[0][0]-A[2][2]*A[1][0]*A[0][1];

      if (DET == 0.0) {
         printf("Attempted inversion of singular matrix in MINV3.  Bailing out.\n");
         exit(1);
      }
      else {
         B[0][0]=(A[1][1]*A[2][2]-A[2][1]*A[1][2])/DET;
         B[0][1]=(A[2][1]*A[0][2]-A[0][1]*A[2][2])/DET;
         B[0][2]=(A[0][1]*A[1][2]-A[1][1]*A[0][2])/DET;
         B[1][0]=(A[2][0]*A[1][2]-A[1][0]*A[2][2])/DET;
         B[1][1]=(A[0][0]*A[2][2]-A[2][0]*A[0][2])/DET;
         B[1][2]=(A[1][0]*A[0][2]-A[0][0]*A[1][2])/DET;
         B[2][0]=(A[1][0]*A[2][1]-A[2][0]*A[1][1])/DET;
         B[2][1]=(A[2][0]*A[0][1]-A[0][0]*A[2][1])/DET;
         B[2][2]=(A[0][0]*A[1][1]-A[1][0]*A[0][1])/DET;
      }
}
/******************************************************************************/
/*  Inverse of a 2x2 Matrix                                                   */
void MINV2(double A[2][2], double B[2][2])
{
      double DET;
      
      DET = A[0][0]*A[1][1] - A[1][0]*A[0][1];
      
      if (DET == 0.0) {
         printf("Attempted inversion of singular matrix in MINV2.  Bailing out.\n");
         exit(1);
      }
      else {
         B[0][0]= A[1][1]/DET;
         B[0][1]=-A[0][1]/DET;
         B[1][0]=-A[1][0]/DET;
         B[1][1]= A[0][0]/DET;
      }
}
/**********************************************************************/
/*  Pseudo-inverse of a 4x3 matrix                                    */
void PINV4x3(double A[4][3], double Aplus[3][4])
{
      double AtA[3][3],AtAi[3][3];

      AtA[0][0]=A[0][0]*A[0][0]+A[1][0]*A[1][0]
               +A[2][0]*A[2][0]+A[3][0]*A[3][0];
      AtA[0][1]=A[0][0]*A[0][1]+A[1][0]*A[1][1]
               +A[2][0]*A[2][1]+A[3][0]*A[3][1];
      AtA[0][2]=A[0][0]*A[0][2]+A[1][0]*A[1][2]
               +A[2][0]*A[2][2]+A[3][0]*A[3][2];
      AtA[1][0]=A[0][1]*A[0][0]+A[1][1]*A[1][0]
               +A[2][1]*A[2][0]+A[3][1]*A[3][0];
      AtA[1][1]=A[0][1]*A[0][1]+A[1][1]*A[1][1]
               +A[2][1]*A[2][1]+A[3][1]*A[3][1];
      AtA[1][2]=A[0][1]*A[0][2]+A[1][1]*A[1][2]
               +A[2][1]*A[2][2]+A[3][1]*A[3][2];
      AtA[2][0]=A[0][2]*A[0][0]+A[1][2]*A[1][0]
               +A[2][2]*A[2][0]+A[3][2]*A[3][0];
      AtA[2][1]=A[0][2]*A[0][1]+A[1][2]*A[1][1]
               +A[2][2]*A[2][1]+A[3][2]*A[3][1];
      AtA[2][2]=A[0][2]*A[0][2]+A[1][2]*A[1][2]
               +A[2][2]*A[2][2]+A[3][2]*A[3][2];

      MINV3(AtA,AtAi);

      Aplus[0][0]=AtAi[0][0]*A[0][0]+AtAi[0][1]*A[0][1]+AtAi[0][2]*A[0][2];
      Aplus[0][1]=AtAi[0][0]*A[1][0]+AtAi[0][1]*A[1][1]+AtAi[0][2]*A[1][2];
      Aplus[0][2]=AtAi[0][0]*A[2][0]+AtAi[0][1]*A[2][1]+AtAi[0][2]*A[2][2];
      Aplus[0][3]=AtAi[0][0]*A[3][0]+AtAi[0][1]*A[3][1]+AtAi[0][2]*A[3][2];
      Aplus[1][0]=AtAi[1][0]*A[0][0]+AtAi[1][1]*A[0][1]+AtAi[1][2]*A[0][2];
      Aplus[1][1]=AtAi[1][0]*A[1][0]+AtAi[1][1]*A[1][1]+AtAi[1][2]*A[1][2];
      Aplus[1][2]=AtAi[1][0]*A[2][0]+AtAi[1][1]*A[2][1]+AtAi[1][2]*A[2][2];
      Aplus[1][3]=AtAi[1][0]*A[3][0]+AtAi[1][1]*A[3][1]+AtAi[1][2]*A[3][2];
      Aplus[2][0]=AtAi[2][0]*A[0][0]+AtAi[2][1]*A[0][1]+AtAi[2][2]*A[0][2];
      Aplus[2][1]=AtAi[2][0]*A[1][0]+AtAi[2][1]*A[1][1]+AtAi[2][2]*A[1][2];
      Aplus[2][2]=AtAi[2][0]*A[2][0]+AtAi[2][1]*A[2][1]+AtAi[2][2]*A[2][2];
      Aplus[2][3]=AtAi[2][0]*A[3][0]+AtAi[2][1]*A[3][1]+AtAi[2][2]*A[3][2];
}
/**********************************************************************/
/*  Transpose of a 3x3 Matrix                                         */
void MT(double A[3][3], double B[3][3])
{
      B[0][0]=A[0][0];
      B[0][1]=A[1][0];
      B[0][2]=A[2][0];
      B[1][0]=A[0][1];
      B[1][1]=A[1][1];
      B[1][2]=A[2][1];
      B[2][0]=A[0][2];
      B[2][1]=A[1][2];
      B[2][2]=A[2][2];
}
/**********************************************************************/
/*  Vector Dot Product                                                */
double VoV(double A[3], double B[3])
{
      return(A[0]*B[0]+A[1]*B[1]+A[2]*B[2]);
}
/**********************************************************************/
/*  Vector Cross Product                                              */
void  VxV(double A[3], double B[3], double C[3])
{
      C[0]=A[1]*B[2]-A[2]*B[1];
      C[1]=A[2]*B[0]-A[0]*B[2];
      C[2]=A[0]*B[1]-A[1]*B[0];
}
/**********************************************************************/
/*  Vector cross Matrix dot Vector                                    */
void vxMov(double w[3], double M[3][3], double wxMow[3])
{
      double Mow[3];

      Mow[0]=M[0][0]*w[0]+M[0][1]*w[1]+M[0][2]*w[2];
      Mow[1]=M[1][0]*w[0]+M[1][1]*w[1]+M[1][2]*w[2];
      Mow[2]=M[2][0]*w[0]+M[2][1]*w[1]+M[2][2]*w[2];

      wxMow[0]=w[1]*Mow[2]-w[2]*Mow[1];
      wxMow[1]=w[2]*Mow[0]-w[0]*Mow[2];
      wxMow[2]=w[0]*Mow[1]-w[1]*Mow[0];
}
/**********************************************************************/
/*  Magnitude of a 3-vector                                           */
double MAGV(double V[3])
{
      return(sqrt(V[0]*V[0]+V[1]*V[1]+V[2]*V[2]));
}
/**********************************************************************/
/*  Normalize a 3-vector.  Return its (pre-normalization) magnitude   */
double UNITV(double V[3])
{
      double A;

      A=sqrt(V[0]*V[0]+V[1]*V[1]+V[2]*V[2]);
      if (A > 0.0) {
         V[0]/=A;
         V[1]/=A;
         V[2]/=A;
      }
      else {
         printf("Attempted divide by zero in UNITV (Line %d of mathkit.c)\n",__LINE__);
         V[0] = 0.0;
         V[1] = 0.0;
         V[2] = 0.0;
      }
      return(A);
}
/**********************************************************************/
/*  Copy and normalize a 3-vector.  Return its magnitude              */
double CopyUnitV(double V[3], double W[3])
{
      double A;

      A=sqrt(V[0]*V[0]+V[1]*V[1]+V[2]*V[2]);
      if (A > 0.0) {
         W[0] = V[0]/A;
         W[1] = V[1]/A;
         W[2] = V[2]/A;
      }
      else {
         printf("Attempted divide by zero in COPYUNITV (Line %d of mathkit.c)\n",__LINE__);
         W[0] = 0.0;
         W[1] = 0.0;
         W[2] = 0.0;
      }
      return(A);
}
/**********************************************************************/
/*  Form a skew-symmetric matrix M from a vector V such that the      */
/*  product MxA equals the cross product VxA for any vector A.        */
void V2CrossM(double V[3], double M[3][3])
{
      M[0][0] =   0.0;
      M[1][1] =   0.0;
      M[2][2] =   0.0;
      M[2][1] =  V[0];
      M[0][2] =  V[1];
      M[1][0] =  V[2];
      M[1][2] = -V[0];
      M[2][0] = -V[1];
      M[0][1] = -V[2];
}
/**********************************************************************/
/*  Form a symmetric matrix M from a vector V such that the           */
/*  product M*A equals the product Vx(VxA) for any vector A.          */
void V2DoubleCrossM(double V[3], double M[3][3])
{
      M[0][0] = -V[1]*V[1]-V[2]*V[2];
      M[1][1] = -V[2]*V[2]-V[0]*V[0];
      M[2][2] = -V[0]*V[0]-V[1]*V[1];
      M[2][1] =  V[2]*V[1];
      M[0][2] =  V[0]*V[2];
      M[1][0] =  V[1]*V[0];
      M[1][2] =  V[1]*V[2];
      M[2][0] =  V[2]*V[0];
      M[0][1] =  V[0]*V[1];
}
/**********************************************************************/
/*  Save a step.  Form a skew matrix from V, then multiply by M       */
void VcrossM(double V[3], double M[3][3], double A[3][3])
{

      A[0][0] =  V[1]*M[2][0]-V[2]*M[1][0];
      A[0][1] =  V[1]*M[2][1]-V[2]*M[1][1];
      A[0][2] =  V[1]*M[2][2]-V[2]*M[1][2];
      A[1][0] =  V[2]*M[0][0]-V[0]*M[2][0];
      A[1][1] =  V[2]*M[0][1]-V[0]*M[2][1];
      A[1][2] =  V[2]*M[0][2]-V[0]*M[2][2];
      A[2][0] =  V[0]*M[1][0]-V[1]*M[0][0];
      A[2][1] =  V[0]*M[1][1]-V[1]*M[0][1];
      A[2][2] =  V[0]*M[1][2]-V[1]*M[0][2];

}
/**********************************************************************/
/*  Save a step.  Form a skew matrix from V, then multiply by MT      */
void VcrossMT(double V[3], double M[3][3], double A[3][3])
{

      A[0][0] =  V[1]*M[0][2]-V[2]*M[0][1];
      A[0][1] =  V[1]*M[1][2]-V[2]*M[1][1];
      A[0][2] =  V[1]*M[2][2]-V[2]*M[2][1];
      A[1][0] =  V[2]*M[0][0]-V[0]*M[0][2];
      A[1][1] =  V[2]*M[1][0]-V[0]*M[1][2];
      A[1][2] =  V[2]*M[2][0]-V[0]*M[2][2];
      A[2][0] =  V[0]*M[0][1]-V[1]*M[0][0];
      A[2][1] =  V[0]*M[1][1]-V[1]*M[1][0];
      A[2][2] =  V[0]*M[2][1]-V[1]*M[2][0];

}
/**********************************************************************/
/*  Quaternion product                                                */
void QxQ(double A[4], double B[4], double C[4])
{
      C[0]= A[3]*B[0]+A[2]*B[1]-A[1]*B[2]+A[0]*B[3];
      C[1]=-A[2]*B[0]+A[3]*B[1]+A[0]*B[2]+A[1]*B[3];
      C[2]= A[1]*B[0]-A[0]*B[1]+A[3]*B[2]+A[2]*B[3];
      C[3]=-A[0]*B[0]-A[1]*B[1]-A[2]*B[2]+A[3]*B[3];
}
/**********************************************************************/
/* Product of the Complement of a Quaternion (A) with a Quaternion (B)*/
void QTxQ(double A[4], double B[4], double C[4])
{
      C[0]= A[3]*B[0]-A[2]*B[1]+A[1]*B[2]-A[0]*B[3];
      C[1]= A[2]*B[0]+A[3]*B[1]-A[0]*B[2]-A[1]*B[3];
      C[2]=-A[1]*B[0]+A[0]*B[1]+A[3]*B[2]-A[2]*B[3];
      C[3]= A[0]*B[0]+A[1]*B[1]+A[2]*B[2]+A[3]*B[3];
}
/**********************************************************************/
/* Product of a Quaternion (A) with the Complement of a Quaternion (B)*/
void QxQT(double A[4], double B[4], double C[4])
{
      C[0]=-A[3]*B[0]-A[2]*B[1]+A[1]*B[2]+A[0]*B[3];
      C[1]= A[2]*B[0]-A[3]*B[1]-A[0]*B[2]+A[1]*B[3];
      C[2]=-A[1]*B[0]+A[0]*B[1]-A[3]*B[2]+A[2]*B[3];
      C[3]= A[0]*B[0]+A[1]*B[1]+A[2]*B[2]+A[3]*B[3];
}
/**********************************************************************/
/* Find components of V in B, given components of V in A, and qab     */
void VxQ(double Va[3],double QAB[4],double Vb[3])
{
      double qq[4][4];
      long i,j;

      for(i=0;i<4;i++) {
         for(j=i;j<4;j++) qq[i][j] = QAB[i]*QAB[j];
      }

      Vb[0] = ( qq[0][0]-qq[1][1]-qq[2][2]+qq[3][3])*Va[0]
                          + 2.0*((qq[0][1]-qq[2][3])*Va[1]
                                +(qq[0][2]+qq[1][3])*Va[2]);
      Vb[1] = (-qq[0][0]+qq[1][1]-qq[2][2]+qq[3][3])*Va[1]
                          + 2.0*((qq[1][2]-qq[0][3])*Va[2]
                                +(qq[0][1]+qq[2][3])*Va[0]);
      Vb[2] = (-qq[0][0]-qq[1][1]+qq[2][2]+qq[3][3])*Va[2]
                          + 2.0*((qq[0][2]-qq[1][3])*Va[0]
                                +(qq[1][2]+qq[0][3])*Va[1]);
}
/**********************************************************************/
/* Find components of V in A, given components of V in B, and qab     */
void QxV(double QAB[4],double Vb[3],double Va[3])
{
      double qq[4][4];
      long i,j;

      for(i=0;i<4;i++) {
         for(j=i;j<4;j++) qq[i][j] = QAB[i]*QAB[j];
      }

      Va[0] = ( qq[0][0]-qq[1][1]-qq[2][2]+qq[3][3])*Vb[0]
                          + 2.0*((qq[0][1]+qq[2][3])*Vb[1]
                                +(qq[0][2]-qq[1][3])*Vb[2]);
      Va[1] = (-qq[0][0]+qq[1][1]-qq[2][2]+qq[3][3])*Vb[1]
                          + 2.0*((qq[1][2]+qq[0][3])*Vb[2]
                                +(qq[0][1]-qq[2][3])*Vb[0]);
      Va[2] = (-qq[0][0]-qq[1][1]+qq[2][2]+qq[3][3])*Vb[2]
                          + 2.0*((qq[0][2]+qq[1][3])*Vb[0]
                                +(qq[1][2]-qq[0][3])*Vb[1]);
}
/**********************************************************************/
/* Find components of V in B, given components of V in A, and qab     */
void QTxV(double QAB[4],double Va[3],double Vb[3])
{
      double qq[4][4];
      long i,j;

      for(i=0;i<4;i++) {
         for(j=i;j<4;j++) qq[i][j] = QAB[i]*QAB[j];
      }

      Vb[0] = ( qq[0][0]-qq[1][1]-qq[2][2]+qq[3][3])*Va[0]
                          + 2.0*((qq[0][1]-qq[2][3])*Va[1]
                                +(qq[0][2]+qq[1][3])*Va[2]);
      Vb[1] = (-qq[0][0]+qq[1][1]-qq[2][2]+qq[3][3])*Va[1]
                          + 2.0*((qq[1][2]-qq[0][3])*Va[2]
                                +(qq[0][1]+qq[2][3])*Va[0]);
      Vb[2] = (-qq[0][0]-qq[1][1]+qq[2][2]+qq[3][3])*Va[2]
                          + 2.0*((qq[0][2]-qq[1][3])*Va[0]
                                +(qq[1][2]+qq[0][3])*Va[1]);
}
/**********************************************************************/
/*  Normalize a quaternion                                            */
void UNITQ(double Q[4])
{
      double A;

      A=sqrt(Q[0]*Q[0]+Q[1]*Q[1]+Q[2]*Q[2]+Q[3]*Q[3]);
      if (A == 0.0) {
         printf("Divide by zero in UNITQ (Line %d of mathkit.c).  You'll want to fix that.\n",__LINE__);
         exit(1);
      }
      else {
         Q[0]/=A;
         Q[1]/=A;
         Q[2]/=A;
         Q[3]/=A;
      }
}
/**********************************************************************/
/*  Rectify a quaternion, forcing q[3] to be positive                 */
void RECTIFYQ(double Q[4])
{
      if(Q[3] < 0.0) {
         Q[0] = -Q[0];
         Q[1] = -Q[1];
         Q[2] = -Q[2];
         Q[3] = -Q[3];
      }
}
/*********************************************************************/
/* Given vector A, find vectors B, C to form orthogonal basis        */
void PerpBasis(double A[3], double B[3], double C[3])
{
      long i;
      double V[3] = {0.0,0.0,0.0};
      double Amin;

      Amin = fabs(A[0]);
      i=0;
      if (fabs(A[1]) < Amin) {
         Amin = A[1];
         i=1;
      }
      if (fabs(A[2]) < Amin) {
         i=2;
      }

      V[i] = 1.0;
      VxV(A,V,B);
      UNITV(B);
      VxV(A,B,C);
      UNITV(C);
}
/**********************************************************************/
double fact(long n)
{
      double F=1.0;
      long i;

      for(i=1;i<=n;i++) F *= (double) i;

      return F;
}
/**********************************************************************/
double oddfact(long n)
{
      double F=1.0;
      long i;

      for(i=1;i<=n;i+=2) F *= (double) i;

      return F;
}
/**********************************************************************/
/*  Legendre Functions P(x) and sdP(x), up to Degree N and Order M    */
/*  Neumann normalization (see Battin p.390+, Wertz App. G)           */
/*  Note that dP[n][1] are singular at x = +/- 1.0.                   */
/*  sdP = sqrt(1-x^2)*dP are not singular, and are how dP's are used  */
/*  in SphericalHarmonics.                                            */
#define NMAX 18
void Legendre(long N, long M, double x,
              double P[NMAX+1][NMAX+1], double sdP[NMAX+1][NMAX+1])
{

      double Ps[NMAX+1][NMAX+1];
      long n,m;
      double s;

/* .. Order can't be greater than Degree */
      if (M > N) {
         printf("Order %ld can't be greater than Degree %ld\n",M,N);
         exit(1);
      }

      s = sqrt(1.0-x*x);

/* .. Some terms are easy */
      P[0][0] = 1.0;
      P[1][0] = x;
      sdP[0][0] = 0.0;
      sdP[1][0] = s;

/* .. m=0 terms are not too bad */
      for(n=2;n<=N;n++) {
         P[n][0]  = ((2.0*n-1.0)/n)*x*P[n-1][0]-((n-1.0)/n)*P[n-2][0];
         sdP[n][0] = x*sdP[n-1][0]+n*s*P[n-1][0];
      }

/* .. Then there are the rest... */
      for(m=1;m<=M;m++) {
         P[m][m] = oddfact(2*m-1)*pow(s,m);
         Ps[m][m] = oddfact(2*m-1)*pow(s,m-1);
         sdP[m][m] = m*(x*Ps[m][m]-2.0*P[m][m-1]);
         if (m < N) {
            P[m+1][m] = x*(2.0*m+1.0)*P[m][m];
            Ps[m+1][m] = x*(2.0*m+1.0)*Ps[m][m];
            sdP[m+1][m] = m*x*Ps[m+1][m]-2.0*(2.0*m+1.0)*P[m+1][m-1];
         }
         for(n=m+2;n<=N;n++) {
            P[n][m] = (x*(2.0*n-1.0)*P[n-1][m]-(n+m-1.0)*P[n-2][m])/(n-m);
            Ps[n][m] = (x*(2.0*n-1.0)*Ps[n-1][m]-(n+m-1.0)*Ps[n-2][m])/(n-m);
            sdP[n][m] = m*x*Ps[n][m]-(n+m)*(n-m+1.0)*P[n][m-1];
         }
      }

}
/**********************************************************************/
/* Finds gradient of the potential V, which is parameterized by       */
/* Legendre coefficient matrices C and S, using the Neumann           */
/* ("conventional") normalization.                                    */
/* gradV[0] = Radial (positive outward)                               */
/* gradV[1] = Latitudinal (positive south)                            */
/* gradV[2] = Longitudinal (positive east)                            */
void SphericalHarmonics(long N, long M, double r, double phi,
        double theta, double Re, double K,
        double C[NMAX+1][NMAX+1], double S[NMAX+1][NMAX+1],double gradV[3])
{

      double P[NMAX+1][NMAX+1],sdP[NMAX+1][NMAX+1];
      long n,m;
      double cphi[NMAX+1],sphi[NMAX+1];
      double Rer,Rern1,CcSs,ScCs,sth;
      double dVdr,dVdphi,dVdtheta;

/* .. Order can't be greater than Degree */
      if (M > N) {
         printf("Order %ld can't be greater than Degree %ld\n",M,N);
         exit(1);
      }

/* .. Find Legendre functions */
      Legendre(N,M,cos(theta),P,sdP);

/* .. Build cos(m*phi) and sin(m*phi) */
      cphi[0] = 1.0;
      sphi[0] = 0.0;
      cphi[1] = cos(phi);
      sphi[1] = sin(phi);
      for(m=2;m<=M;m++){
         cphi[m] = cphi[m-1]*cphi[1]-sphi[m-1]*sphi[1];
         sphi[m] = sphi[m-1]*cphi[1]+cphi[m-1]*sphi[1];
      }

/* .. Find gradient of V */
      dVdr = 0.0;
      dVdphi = 0.0;
      dVdtheta = 0.0;
      Rer = Re/r;
      Rern1 = Rer;
      sth = sin(theta);
      for(n=1;n<=N;n++){
         Rern1 *= Rer;
         for(m=0;m<=n && m<=M;m++){
            CcSs = C[n][m]*cphi[m]+S[n][m]*sphi[m];
            ScCs = S[n][m]*cphi[m]-C[n][m]*sphi[m];
            dVdr     += -(n+1) * Rern1 * CcSs *   P[n][m];
            dVdphi   +=      m * Rern1 * ScCs *   P[n][m];
            dVdtheta +=         -Rern1 * CcSs * sdP[n][m];
         }
      }
      dVdr *= K/r;
      dVdphi *= K;
      dVdtheta *= K;

      gradV[0] = dVdr;
      gradV[1] = dVdtheta/r;
      if (sth == 0.0)
         gradV[2] = 0.0;
      else
         gradV[2] = dVdphi/(r*sth);

      /*printf("N,M,n,m: %ld %ld %ld %ld\n",N,M,n,m);
      **printf("Rern1,CcSs,ScCs: %lf %lf %lf \n",Rern1,CcSs,ScCs);
      **printf("gradV: %lf %lf %lf\n",gradV[0],gradV[1],gradV[2]);
      */
}
#undef NMAX
/**********************************************************************/
/*  A is NxK, B is KxM, C is NxM                                      */
void MxMG(double **A, double **B, double **C,
          long N, long K, long M)
{
      long i,j,k;

      for(i=0;i<N;i++) {
         for(j=0;j<M;j++) {
            C[i][j] = 0.0;
            for(k=0;k<K;k++) {
               C[i][j] += A[i][k]*B[k][j];
            }
         }
      }
}
/**********************************************************************/
/*  A is NxK, B is MxK, C is NxM                                      */
void MxMTG(double **A, double **B, double **C,
          long N, long K, long M)
{
      long i,j,k;

      for(i=0;i<N;i++) {
         for(j=0;j<M;j++) {
            C[i][j] = 0.0;
            for(k=0;k<K;k++) {
               C[i][j] += A[i][k]*B[j][k];
            }
         }
      }
}
/**********************************************************************/
/*  A is KxN, B is KxM, C is NxM                                      */
void MTxMG(double **A, double **B, double **C,
          long N, long K, long M)
{
      long i,j,k;

      for(i=0;i<N;i++) {
         for(j=0;j<M;j++) {
            C[i][j] = 0.0;
            for(k=0;k<K;k++) {
               C[i][j] += A[k][i]*B[k][j];
            }
         }
      }
}
/**********************************************************************/
void MxVG(double **M, double *v, double *w, long n, long m)
{
      long i,j;

      for(i=0;i<n;i++) {
         w[i] = 0.0;
         for(j=0;j<m;j++) {
            w[i] += M[i][j]*v[j];
         }
      }
}
/**********************************************************************/
/*  Product of scalar S with NxM matrix A                             */
void SxMG(double s, double **A, double **B,
          long N, long M)
{
      long i,j;

      for(i=0;i<N;i++) {
         for(j=0;j<M;j++) {
            B[i][j] = s*A[i][j];
         }
      }
}
/**********************************************************************/
/*                  GENERAL MATRIX INVERSE                            */
/* Inverse of an NxN matrix                                           */
void MINVG(double **A, double **AI, long N)
{
      long I,J,ROW;
      long IPIVOT = 0;
      double **M;
      double PIVOT,K,*TA,*TB;

      M = CreateMatrix(N,N);
      TA = (double *) calloc(N,sizeof(double));
      TB = (double *) calloc(N,sizeof(double));

      for(I=0;I<N;I++){
         for(J=0;J<N;J++){
            M[I][J] = A[I][J];
            AI[I][J] = 0.0;
         }
         AI[I][I] = 1.0;
      }

      for(ROW=0;ROW<N;ROW++){
         PIVOT = M[ROW][ROW];
         IPIVOT = ROW;
         for(I=ROW+1;I<N;I++){
            if (fabs(M[I][ROW]) > fabs(PIVOT)){
               PIVOT = M[I][ROW];
               IPIVOT = I;
            }
         }
         if (PIVOT == 0.0){
            printf("Matrix is singular in MINVG\n");
            exit(1);
         }

         for(J=0;J<N;J++){
            TA[J] = M[IPIVOT][J];
            TB[J] = AI[IPIVOT][J];
            M[IPIVOT][J] = M[ROW][J];
            AI[IPIVOT][J] = AI[ROW][J];
            M[ROW][J] = TA[J] / PIVOT;
            AI[ROW][J] = TB[J] / PIVOT;
         }
         for(I=ROW+1;I<N;I++){
            K = M[I][ROW];
            for(J=0;J<N;J++){
               M[I][J] = M[I][J] - K * M[ROW][J];
               AI[I][J] = AI[I][J] - K * AI[ROW][J];
            }
         }
      }

/*    M is now upper diagonal */

      for(ROW=N-1;ROW>0;ROW--){
         for(I=0;I<ROW;I++){
            K = M[I][ROW];
            for(J=0;J<N;J++){
               M[I][J] = M[I][J] - K * M[ROW][J];
               AI[I][J] = AI[I][J] - K * AI[ROW][J];
            }
         }
      }

      DestroyMatrix(M,N);
      free(TA);
      free(TB);
}
/******************************************************************************/
/* For Order-N dynamics, we need to invert matrices of size 1 <= N <= 6       */
/* This specialized function avoids mallocs to save time                      */
void FastMINV6(double A[6][6], double AI[6][6], long N)
{
      long I,J,ROW;
      long IPIVOT = 0;
      double M[6][6];
      double PIVOT,K,TA[6],TB[6];


      for(I=0;I<N;I++){
         for(J=0;J<N;J++){
            M[I][J] = A[I][J];
            AI[I][J] = 0.0;
         }
         AI[I][I] = 1.0;
      }

      for(ROW=0;ROW<N;ROW++){
         PIVOT = M[ROW][ROW];
         IPIVOT = ROW;
         for(I=ROW+1;I<N;I++){
            if (fabs(M[I][ROW]) > fabs(PIVOT)){
               PIVOT = M[I][ROW];
               IPIVOT = I;
            }
         }
         if (PIVOT == 0.0){
            printf("Matrix is singular in FastMINV6\n");
            exit(1);
         }

         for(J=0;J<N;J++){
            TA[J] = M[IPIVOT][J];
            TB[J] = AI[IPIVOT][J];
            M[IPIVOT][J] = M[ROW][J];
            AI[IPIVOT][J] = AI[ROW][J];
            M[ROW][J] = TA[J] / PIVOT;
            AI[ROW][J] = TB[J] / PIVOT;
         }
         for(I=ROW+1;I<N;I++){
            K = M[I][ROW];
            for(J=0;J<N;J++){
               M[I][J] = M[I][J] - K * M[ROW][J];
               AI[I][J] = AI[I][J] - K * AI[ROW][J];
            }
         }
      }

/*    M is now upper diagonal */

      for(ROW=N-1;ROW>0;ROW--){
         for(I=0;I<ROW;I++){
            K = M[I][ROW];
            for(J=0;J<N;J++){
               M[I][J] = M[I][J] - K * M[ROW][J];
               AI[I][J] = AI[I][J] - K * AI[ROW][J];
            }
         }
      }
}
/**********************************************************************/
/*  Find the pseudo-inverse of an n-by-m matrix A                     */
void PINVG(double **A, double **Ai, long n, long m)
{
      double **AtA, **AtAi;
      double **AAt, **AAti;

      if (n == m) {
         MINVG(A,Ai,n);
      }
      else if (n > m) {
         AtA = CreateMatrix(m,m);
         AtAi = CreateMatrix(m,m);
         MTxMG(A,A,AtA,m,n,m);
         MINVG(AtA,AtAi,m);
         MxMTG(AtAi,A,Ai,m,m,n);
         DestroyMatrix(AtA,m);
         DestroyMatrix(AtAi,m);
      }
      else {
         AAt = CreateMatrix(n,n);
         AAti = CreateMatrix(n,n);
         MxMTG(A,A,AAt,n,m,n);
         MINVG(AAt,AAti,n);
         MTxMG(A,AAti,Ai,m,n,n);
         DestroyMatrix(AAt,n);
         DestroyMatrix(AAti,n);
      }
}
/**********************************************************************/
double **CreateMatrix(long n, long m)
{
      double **A;
      long i;

      A = (double **) calloc(n,sizeof(double *));
      if (A == NULL) {
         printf("calloc failed in CreateMatrix.  Bailing out.\n");
         exit(1);
      }
      for(i=0;i<n;i++) {
         A[i] = (double *) calloc(m,sizeof(double));
         if (A[i] == NULL) {
            printf("calloc failed in CreateMatrix.  Bailing out.\n");
            exit(1);
         }
      }
      return(A);
}
/**********************************************************************/
void DestroyMatrix(double **A, long n)
{
      long i;
      for(i=0;i<n;i++) free(A[i]);
      free(A);
}
/**********************************************************************/
/*   Solution of NxN system      A * x = b                            */
/*   by Gaussian Elimination and Back Substitution, with pivoting     */
void LINSOLVE(double **A, double *x, double *b, long n)
{
      long i,j,k,l,m;
      double mm,*a1,b1;

      if (n == 1) {
         x[0] = b[0]/A[0][0];
         return;
      }

      a1 = (double *) calloc(n,sizeof(double));

      for(j=0;j<n-1;j++){
         mm = fabs(A[j][j]);
         l=j;
         for(i=j+1;i<n;i++){
            if (fabs(A[i][j]) >= mm){
               l=i;
               mm=fabs(A[i][j]);
            }
         }
         if (l != j){
            for(i=0;i<n;i++){
               a1[i]=A[j][i];
            }
            b1=b[j];
            for(i=j;i<n;i++){
               A[j][i]=A[l][i]/A[l][j];
            }
            b[j]=b[l]/A[l][j];
            for(i=0;i<n;i++){
               A[l][i]=a1[i];
            }
            b[l]=b1;
         }
         else {
            b[j]=b[j]/A[j][j];
            for(i=n-1;i>=j;i--){
               A[j][i]=A[j][i]/A[j][j];
            }
         }
         for(k=j+1;k<n;k++){
            b[k]-=A[k][j]*b[j];
         }
         for(k=j+1;k<n;k++){
            for(m=n-1;m>=j;m--){
               A[k][m]-=A[k][j]*A[j][m];
            }
         }
      }
      x[n-1]=b[n-1]/A[n-1][n-1];
      for(i=n-2;i>=0;i--){
         x[i]=b[i];
         for(k=i+1;k<n;k++){
            x[i]-=A[i][k]*x[k];
         }
      }

      free(a1);
}
/**********************************************************************/
/*  Solution of the linear equations A*x=b by Cholesky Decomposition  */
/*  This method can only be used if A is positive definite            */
/*  (symmetric), but it is roughly twice as fast as Gaussian          */
/*  Elimination.                                                      */
/*  In testing, this didn't live up to the hype, being slightly       */
/*  slower than LINSOLVE.  I must have an inefficiency.               */               
void CholeskySolve(double **A, double *x, double *b, long n)
{
      double **L, *D, **LD;
      double *y;
      long i,j,k;

      L = CreateMatrix(n,n);
      D = (double *) calloc(n,sizeof(double));
      LD = CreateMatrix(n,n);
      y = (double *) calloc(n,sizeof(double));

/* .. Find L, D */
      for(j=0;j<n;j++) {
         D[j] = A[j][j];
         for(k=0;k<j;k++) D[j] -= L[j][k]*L[j][k]*D[k];
         for(i=0;i<n;i++) {
            L[i][j] = A[i][j];
            for(k=0;k<j;k++) L[i][j] -= L[i][k]*L[j][k]*D[k];
            L[i][j] /= D[j];
         }
      }

/* .. Find LD */
      for(i=0;i<n;i++) {
         LD[i][i] = D[i];
         for(j=i+1;j<n;j++) LD[j][i] = L[j][i]*D[i];
      }

/* .. Solve LD*y = b */
      for(i=0;i<n;i++) {
         y[i] = b[i];
         for(k=0;k<i;k++) y[i] -= LD[i][k]*y[k];
         y[i] /= LD[i][i];
      }
/* .. Solve Lt*x = y */
      for(i=n-1;i>=0;i--) {
         x[i] = y[i];
         for(k=n-1;k>i;k--) x[i] -= L[k][i]*x[k];
      }

      DestroyMatrix(L,n);
      DestroyMatrix(LD,n);
      free(D);
      free(y);
}
/**********************************************************************/
/* Solution of linear equations by Conjugate Gradient method.         */
/* See "An Introduction to the Conjugate Gradient Method              */
/* Without the Agonizing Pain", by Jonathan Richard Shewchuk          */
void ConjGradSolve(double **A, double *x, double *b, long n,
   double errtol, long maxiter)
{
      double *r,*d,*q;
      double DeltaNew,DeltaOld,Err2D0,alpha,Beta,dq;
      long i,j,Iter;

      r = (double *) calloc(n,sizeof(double));
      d = (double *) calloc(n,sizeof(double));
      q = (double *) calloc(n,sizeof(double));

/* .. r = b - A*x, d = r, DeltaNew = r'*r */
      DeltaNew = 0.0;
      for(i=0;i<n;i++) {
         r[i] = b[i];
         for(j=0;j<n;j++) r[i] -= A[i][j]*x[j];
         d[i] = r[i];
         DeltaNew += r[i]*r[i];
      }

/* .. Error tolerance end condition */
      Err2D0 = errtol*errtol*DeltaNew;

      Iter = 0;
      while (Iter < maxiter && DeltaNew > Err2D0) {
         /* q = A*d */
         dq = 0.0;
         for(i=0;i<n;i++) {
            q[i] = 0.0;
            for(j=0;j<n;j++) q[i] += A[i][j]*d[j];
            dq += d[i]*q[i];
         }
         /* alpha = DeltaNew/d'*q */
         alpha = DeltaNew/dq;

         /* x += alpha*d */
         for(i=0;i<n;i++) x[i] += alpha*d[i];

         if (Iter%50 == 0) {
            for(i=0;i<n;i++) {
               r[i] = b[i];
               for(j=0;j<n;j++) r[i] -= A[i][j]*x[j];
            }
         }
         else {
            for(i=0;i<n;i++) r[i] -= alpha*q[i];
         }
         /* DeltaOld = DeltaNew */
         DeltaOld = DeltaNew;

         /* DeltaNew = r'*r */
         DeltaNew = 0.0;
         for(i=0;i<n;i++) DeltaNew += r[i]*r[i];

         /* Beta = DeltaNew/DeltaOld */
         Beta = DeltaNew/DeltaOld;

         /* d = r + beta*d */
         for(i=0;i<n;i++) d[i] = r[i] + Beta*d[i];

         Iter++;
      }

      free(r);
      free(d);
      free(q);
}
/************************************************************************/
/*  Find Roots of a Polynomial using Bairstow's Method                  */
/*  a = Coefficients of polynomial (length n+1)                         */
/*  Real = Real parts of roots (length n)                               */
/*  Imag = Imaginary parts of roots (length n)                          */
void Bairstow(long n, double *a, double Tol, double *Real, double *Imag)
{
#define MAX(x,y) (x>y?x:y)

      double *b,*c;
      double r,s,dr,ds,Disc,Det;
      long Done;
      long i;

      b = (double *) calloc(n+1,sizeof(double));
      c = (double *) calloc(n+1,sizeof(double));

      while (n > 2) {

         /* Initial Guesses */
         if (a[n-2] != 0.0) {
            r=-a[n-1]/a[n-2];
            s=-a[n]/a[n-2];
         }
         else {
            r=-a[1]/a[0];
            s=-a[2]/a[0];
         }

         b[0]=a[0];
         c[0]=b[0];

         /* Search for quadratic factor */
         Done = 0;
         while (!Done) {
            b[1]=a[1]+b[0]*r;
            c[1]=b[1]+c[0]*r;
            for (i=2;i<n+1;i++) {
               b[i]=a[i]+r*b[i-1]+s*b[i-2];
               c[i]=b[i]+r*c[i-1]+s*c[i-2];
            }
            Det = c[n-1]*c[n-3]-c[n-2]*c[n-2];
            dr = (b[n-1]*c[n-2]-b[n]*c[n-3])/Det;
            ds = (b[n]*c[n-2]-b[n-1]*c[n-1])/Det;
            r = r + dr;
            s = s + ds;

            if (fabs(dr) < Tol*MAX(1.0,r) && fabs(ds) < Tol*MAX(1.0,s))
               Done = 1;
         }

         /* Store roots of quadratic factor */
         Disc=r*r+4.0*s;
         if (Disc < 0.0) {
            Real[n-1]=0.5*r;
            Imag[n-1]=0.5*sqrt(-Disc);
            Real[n-2]=Real[n-1];
            Imag[n-2]=-Imag[n-1];
         }
         else {
            Real[n-1]=0.5*(r+sqrt(Disc));
            Imag[n-1]=0.0;
            Real[n-2]=0.5*(r-sqrt(Disc));
            Imag[n-2]=0.0;
         }

         /* Deflate polynomial */
         n-=2;
         for (i=0;i<n+1;i++) a[i] = b[i];

      } /* End deflation */

      /* Find roots of remaining first/second order polynomial */
      if (n==1) {
         Real[0] = -a[1]/a[0];
         Imag[0] = 0.0;
      }
      else {
         r = -a[1]/a[0];
         s = -a[2]/a[0];
         Disc=r*r+4*s;
         if (Disc < 0.0) {
            Real[1]=0.5*r;
            Imag[1]=0.5*sqrt(-Disc);
            Real[0]=Real[1];
            Imag[0]=-Imag[1];
         }
         else {
            Real[1]=0.5*(r+sqrt(Disc));
            Imag[1]=0.0;
            Real[0]=0.5*(r-sqrt(Disc));
            Imag[0]=0.0;
         }
      }
      free(b);
      free(c);
#undef MAX
}
/**********************************************************************/
/*  Minimize a cost function by Downhill Simplex Method               */
/*  See Numerical Recipes 10.4 for description of method              */
/*                                                                    */
/*  N = Number of dimensions                                          */
/*  P = Vector which minimizes cost function                          */
/*      (Initial guess in, result out)                                */
/*  CostFunction must take an N-vector as argument,                   */
/*               must return a double                                 */
/*  scale = Size of initial amoeba                                    */
/*  Tol = Tolerance on cost function to declare convergence           */
double Amoeba(long N, double *P,
            double CostFunction(double *p, double *Parm),
            double *CostParm, double scale, double Tol)
{

      long Converged = 0;
      double **p,*pc,*f;
      long high,nexthigh,low;
      double Coef1,Coef2,*pn,fn,StepSize;
      double MinCost;
      long i,j;

      p = (double **) calloc(N+1,sizeof(double *));
      for(i=0;i<N+1;i++) p[i] = (double *) calloc(N,sizeof(double));

      pc = (double *) calloc(N,sizeof(double));
      f = (double *) calloc(N+1,sizeof(double));
      pn = (double *) calloc(N,sizeof(double));

      /* Simplex */
      for(j=0;j<N;j++) p[0][j] = P[j];
      for(i=1;i<N+1;i++) {
         for(j=0;j<N;j++) {
            p[i][j] = p[0][j];
         }
         p[i][i-1] += scale;
      }

      /* Simplex Centroid */
      for(j=0;j<N;j++) {
         pc[j] = 0.0;
         for(i=0;i<N+1;i++) {
            pc[j] += p[i][j];
         }
         pc[j] /= (N+1.0);
      }

      /* Evaluate cost function */
      for(i=0;i<N+1;i++) {
         f[i] = CostFunction(p[i],CostParm);
      }

      /* Find high, next-to-high, low cost */
      low = 0;
      high = 0;
      for(i=1;i<N+1;i++) {
         if (f[i] < f[low])  low = i;
         if (f[i] > f[high]) high = i;
      }
      nexthigh = low;
      for(i=0;i<N+1;i++) {
         if (f[i] > f[nexthigh] && f[i] < f[high])  nexthigh = i;
      }

      while (!Converged) {

         /* Try Reflection */
         StepSize = -1.0;
         Coef1 = (N+1.0)/N*(1.0-StepSize);
         Coef2 = -(1.0-(N+1.0)*StepSize)/N;
         for (j=0;j<N;j++) {
            pn[j] = Coef1*pc[j] + Coef2*p[high][j];
         }
         fn = CostFunction(pn,CostParm);
         if (fn < f[high]) {
            f[high] = fn;
            for(j=0;j<N;j++) {
               pc[j] += (pn[j] - p[high][j])/(N+1);
               p[high][j] = pn[j];
            }
         }
         if (f[high] < f[low]) { /* Worked so well, try longer step */
            StepSize = 2.0;
            Coef1 = (N+1.0)/N*(1.0-StepSize);
            Coef2 = -(1.0-(N+1.0)*StepSize)/N;
            for (j=0;j<N;j++) {
               pn[j] = Coef1*pc[j] + Coef2*p[high][j];
            }
            fn = CostFunction(pn,CostParm);
            if (fn < f[high]) {
               f[high] = fn;
               for(j=0;j<N;j++) {
                  pc[j] += (pn[j] - p[high][j])/(N+1);
                  p[high][j] = pn[j];
               }
            }
         }
         else if (f[high] > f[nexthigh]) { /* Worked not so well, so try contraction */
            StepSize = 0.5;
            Coef1 = (N+1.0)/N*(1.0-StepSize);
            Coef2 = -(1.0-(N+1.0)*StepSize)/N;
            for (j=0;j<N;j++) {
               pn[j] = Coef1*pc[j] + Coef2*p[high][j];
            }
            fn = CostFunction(pn,CostParm);
            if (fn < f[high]) {
               f[high] = fn;
               for(j=0;j<N;j++) {
                  pc[j] += (pn[j] - p[high][j])/(N+1);
                  p[high][j] = pn[j];
               }
            }
            if (f[high] > f[nexthigh]) { /* Stuck.  Contract about lowest point */
               for(i=0;i<N+1;i++) {
                  if (i != low) {
                     for(j=0;j<N;j++) {
                        p[i][j] = 0.5*(p[i][j] + p[low][j]);
                     }
                     f[i] = CostFunction(p[i],CostParm);
                  }
               }
               /* Find centroid */
               for(j=0;j<N;j++) {
                  pc[j] = 0.0;
                  for(i=0;i<N+1;i++) {
                     pc[j] += p[i][j];
                  }
                  pc[j] /= (N+1.0);
               }
            }
         }

         /* Find high, next-to-high, low cost */
         low = 0;
         high = 0;
         for(i=1;i<N+1;i++) {
            if (f[i] < f[low])  low = i;
            if (f[i] > f[high]) high = i;
         }
         nexthigh = low;
         for(i=0;i<N+1;i++) {
            if (f[i] > f[nexthigh] && f[i] < f[high])  nexthigh = i;
         }

         /* Termination Condition */
         if ((fabs(f[low]/f[high]-1.0) < Tol) ||
             (fabs(f[low]-f[high]) < Tol)) {
            Converged = 1;
         }

      }
      for(j=0;j<N;j++) P[j] = p[low][j];
      MinCost = f[low];

      for(i=0;i<N+1;i++) free(p[i]);
      free(p);
      free(pc);
      free(f);
      free(pn);

      return(MinCost);
}
/**********************************************************************/
/*  Find unit normal vector to plane defined by points V1, V2, V3     */
void FindNormal(double V1[3], double V2[3], double V3[3], double N[3])
{
      long i;
      double D1[3],D2[3];

      for(i=0;i<3;i++) {
         D1[i] = V2[i] - V1[i];
         D2[i] = V3[i] - V2[i];
      }
      VxV(D1,D2,N);
      UNITV(N);
}
/**********************************************************************/
/*  Output clamped at ends of interval                                */
double LinInterp(double *X, double *Y, double x, long n)
{
      double dx,dxn,y;
      long i,i1,i2;

      dx = x-X[0];
      dxn = X[n-1]-X[0];
      if (fabs(dxn) < fabs(dx)) {
         printf("LinInterp clamped to 'right' end of interval\n");
         y = Y[n-1];
      }
      else if (dx*dxn < 0.0) {
         printf("LinInterp clamped to 'left' end of interval\n");
         y = Y[0];
      }
      else {
         /* Binary Search */
         i1 = 0;
         i2 = n-1;
         while(i1+1 < i2) {
            i = (i1+i2)/2;
            if (fabs(X[i]-X[0]) < fabs(dx)) i1 = i;
            else i2 = i;
         }
         y = (Y[i2]-Y[i1])/(X[i2]-X[i1])*(x-X[i1])+Y[i1];
      }
      return(y);
}
/**********************************************************************/
/*  SLERP = Spherical Linear Interpolation                            */
/*  A constant-rate interpolation for quaternions                     */
/*  Ref: Ken Shoemake, "Animating Rotation with Quaternion Curves"    */
/*  q(u=0.0) = q1, q(u=1.0) = q2                                      */
void SphereInterp(double q1[4], double q2[4], double u, double q[4])
{
      double Theta,CosTheta,SinTheta;
      double SinU,Sin1mU;
      long k;

      CosTheta = q1[0]*q2[0]+q1[1]*q2[1]+q1[2]*q2[2]+q1[3]*q2[3];
      if (CosTheta >= 1.0) {
         for(k=0;k<4;k++) q[k] = q1[k];
      }
      else {
         SinTheta = sqrt(1.0-CosTheta*CosTheta);
         Theta = asin(SinTheta);
         SinU = sin(u*Theta);
         Sin1mU = sin((1.0-u)*Theta);
         for(k=0;k<4;k++) q[k] = (SinU*q2[k] + Sin1mU*q1[k])/SinTheta;
      }
}
/**********************************************************************/
double CubicInterp1D(double f0, double f1, double x)
{
      double x1 = 1.0-x;
      return((3.0-2.0*x1)*x1*x1*f0+(3.0-2.0*x)*x*x*f1);
}
/**********************************************************************/
double CubicInterp2D(double f00, double f10, double f01, double f11,
                    double x, double y)
{
      double f0 = CubicInterp1D(f00, f10, x);
      double f1 = CubicInterp1D(f01, f11, x);
      return(CubicInterp1D(f0, f1, y));
}
/**********************************************************************/
double CubicInterp3D(double f000, double f100, double f010, double f110,
                    double f001, double f101, double f011, double f111,
                    double x, double y, double z)
{
      double f0 = CubicInterp2D(f000, f100, f010, f110, x, y);
      double f1 = CubicInterp2D(f001, f101, f011, f111, x, y);
      return(CubicInterp1D(f0,f1,z));
}
/**********************************************************************/
double DistanceToLine(double LineEnd1[3], double LineEnd2[3],
   double Point[3], double VecToLine[3])
{
      double Axis[3],Vec[3],VoA;
      long i;

      for(i=0;i<3;i++) {
         Axis[i] = LineEnd2[i] - LineEnd1[i];
         Vec[i] = Point[i] - LineEnd1[i];
      }
      UNITV(Axis);
      VoA = VoV(Vec,Axis);

      for(i=0;i<3;i++) VecToLine[i] = VoA*Axis[i] - Vec[i];
      return(MAGV(VecToLine));
}
/**********************************************************************/
long ProjectPointOntoPoly(double Point[3], double DirVec[3],
   double **Vtx, long Nvtx, double ProjPoint[3], double *Distance)
{
      double Axis[3],a1[3],a2[3];
      static double **COEF,*RHS,*x;
      double SumAng,s1[3],s2[3],S1xS2[3],Norm[3],SinAng,CosAng;
      long i,j,Iv,Nwrap;
      static long First = 1;
      long OnEdge;

      if (First) {
         First = 0;
         COEF = CreateMatrix(4,4);
         RHS = (double *) calloc(4,sizeof(double));
         x = (double *) calloc(4,sizeof(double));
      }

      CopyUnitV(DirVec,Axis);
      for(i=0;i<3;i++) {
         a1[i] = Vtx[1][i] - Vtx[0][i];
         a2[i] = Vtx[2][i] - Vtx[0][i];
      }
      COEF[0][0] = a1[1]*a2[2]-a1[2]*a2[1];
      COEF[0][1] = a1[2]*a2[0]-a1[0]*a2[2];
      COEF[0][2] = a1[0]*a2[1]-a1[1]*a2[0];
      COEF[0][3] = 0.0;
      RHS[0] = COEF[0][0]*Vtx[0][0]
              +COEF[0][1]*Vtx[0][1]
              +COEF[0][2]*Vtx[0][2];
      COEF[1][0] = 1.0;
      COEF[1][1] = 0.0;
      COEF[1][2] = 0.0;
      COEF[1][3] = -Axis[0];
      RHS[1] = Point[0];
      COEF[2][0] = 0.0;
      COEF[2][1] = 1.0;
      COEF[2][2] = 0.0;
      COEF[2][3] = -Axis[1];
      RHS[2] = Point[1];
      COEF[3][0] = 0.0;
      COEF[3][1] = 0.0;
      COEF[3][2] = 1.0;
      COEF[3][3] = -Axis[2];
      RHS[3] = Point[2];
      LINSOLVE(COEF,x,RHS,4);
      for(i=0;i<3;i++) ProjPoint[i] = x[i];
      *Distance = x[3];

      /* Find whether ProjPoint lies in polygon */
      VxV(a1,a2,Norm);
      UNITV(Norm);
      SumAng = 0.0;
      OnEdge = 0;
      for(Iv=0;Iv<Nvtx;Iv++) {
         for(j=0;j<3;j++) {
            s1[j] = Vtx[Iv][j]-ProjPoint[j];
            s2[j] = Vtx[(Iv+1)%Nvtx][j]-ProjPoint[j];
         }
         UNITV(s1);
         UNITV(s2);
         VxV(s1,s2,S1xS2);
         SinAng = VoV(S1xS2,Norm);
         CosAng = VoV(s1,s2);
         if (fabs(SinAng) < 1.0E-6 && CosAng < -0.9) OnEdge = 1;
         SumAng += atan2(SinAng,CosAng);
      }
      Nwrap = (long) (SumAng/6.283+0.5);

      /* If Nwrap is odd, then ProjPoint lies within polygon */
      return(Nwrap%2 || OnEdge);
}
/*********************************************************************/
/* Given Triangle ABC, a point Pt, and a direction vector,           */
/* find the projection of Pt onto ABC.  Barycentric coords have      */
/* fourth element, so that                                           */
/* Pt = Bary[0]*A + Bary[1]*B + Bary[2]*C + Bary[3]*DirVec           */
long ProjectPointOntoTriangle(double A[3], double B[3], double C[3],
      double DirVec[3], double Pt[3], double ProjPt[3], double Bary[4])
{
      double Den,NumA,NumB,NumC,NumD;
      double AxB[3],CxD[3],PxB[3],AxP[3],CxP[3],PxD[3];
      double M[4][3],Mplus[3][4];
      long InPoly,i;

      VxV(A,B,AxB);
      VxV(C,DirVec,CxD);

      Den =  (A[0]-B[0])*CxD[0]
            +(A[1]-B[1])*CxD[1]
            +(A[2]-B[2])*CxD[2]
            -DirVec[0]*AxB[0]
            -DirVec[1]*AxB[1]
            -DirVec[2]*AxB[2];

      if (fabs(Den) < 1.0E-12) {
         /* If DirVec is in plane of ABC, then problem reduces to... */
         for(i=0;i<3;i++) {
            M[i][0] = A[i];
            M[i][1] = B[i];
            M[i][2] = C[i];
            M[3][i] = 1.0;
         }
         PINV4x3(M,Mplus);
         Bary[0] = Mplus[0][0]*Pt[0]+Mplus[0][1]*Pt[1]+Mplus[0][2]*Pt[2];
         Bary[1] = Mplus[1][0]*Pt[0]+Mplus[1][1]*Pt[1]+Mplus[1][2]*Pt[2];
         Bary[2] = Mplus[2][0]*Pt[0]+Mplus[2][1]*Pt[1]+Mplus[2][2]*Pt[2];
         Bary[3] = 0.0;
      }
      else {
         VxV(Pt,B,PxB);
         VxV(A,Pt,AxP);
         VxV(C,Pt,CxP);
         VxV(Pt,DirVec,PxD);
      
         NumA =  (Pt[0]-B[0])*CxD[0]
                +(Pt[1]-B[1])*CxD[1]
                +(Pt[2]-B[2])*CxD[2]
                -DirVec[0]*PxB[0]
                -DirVec[1]*PxB[1]
                -DirVec[2]*PxB[2];

         NumB =  (A[0]-Pt[0])*CxD[0]
                +(A[1]-Pt[1])*CxD[1]
                +(A[2]-Pt[2])*CxD[2]
                -DirVec[0]*AxP[0]
                -DirVec[1]*AxP[1]
                -DirVec[2]*AxP[2];

         NumC =  (A[0]-B[0])*PxD[0]
                +(A[1]-B[1])*PxD[1]
                +(A[2]-B[2])*PxD[2]
                -DirVec[0]*AxB[0]
                -DirVec[1]*AxB[1]
                -DirVec[2]*AxB[2];

         NumD =  (A[0]-B[0])*CxP[0]
                +(A[1]-B[1])*CxP[1]
                +(A[2]-B[2])*CxP[2]
                -(Pt[0]-C[0])*AxB[0]
                -(Pt[1]-C[1])*AxB[1]
                -(Pt[2]-C[2])*AxB[2];

         Bary[0] = NumA/Den;
         Bary[1] = NumB/Den;
         Bary[2] = NumC/Den;
         Bary[3] = NumD/Den;
      }

      ProjPt[0] = Bary[0]*A[0]+Bary[1]*B[0]+Bary[2]*C[0];
      ProjPt[1] = Bary[0]*A[1]+Bary[1]*B[1]+Bary[2]*C[1];
      ProjPt[2] = Bary[0]*A[2]+Bary[1]*B[2]+Bary[2]*C[2];

      InPoly  = (Bary[0] >= 0.0 && Bary[1] >= 0.0 && Bary[2] >= 0.0 ? 1 : 0);

      return(InPoly);
}
/**********************************************************************/
double CubicSpline(double x, double X[4], double Y[4])
{
      double DY0,DY2,DY3;
      double Det,u0,u3,u;
      double z0,z3,u02,u32;
      double a,b,c,d;

      u = (x-X[1])/(X[2]-X[1]);

      if (isnan(u)) {
         printf("Bad spline interval in CubicSpline.\n");
         exit(1);
      }
      if (u < 0.0 || u > 1.0) {
         printf("Interpolant out of range in CubicSpline.\n");
         exit(1);
      }

      DY0 = Y[0]-Y[1];
      DY2 = Y[2]-Y[1];
      DY3 = Y[3]-Y[1];

      u0 = (X[0]-X[1])/(X[2]-X[1]);
      u3 = (X[3]-X[1])/(X[2]-X[1]);

      z0 = u0 - 1.0;
      z3 = u3 - 1.0;
      u02 = u0*u0;
      u32 = u3*u3;

      Det = (u3-1.0)*(u0-1.0)*(u3-u0)*u0*u3;
      if (fabs(Det) < 1.0E-9) {
         printf("Matrix is close to singular in CubicSpline.\n");
         exit(1);
      }
      a = Y[1];
      b = (-z3*u32*DY0 + (u3-u0)*u02*u32*DY2 + z0*u02*DY3)/Det;
      c = (z3*(u3+1.0)*u3*DY0 - (u3-u0)*(u3+u0)*u0*u3*DY2 - z0*(u0+1.0)*u0*DY3)/Det;
      d = (-z3*u3*DY0 + (u3-u0)*u0*u3*DY2 + z0*u0*DY3)/Det;

      return(a+u*(b+u*(c+u*d)));
}
/******************************************************************************/
/* Compute Chebyshev polynomials of first kind (T) and second kind (U)        */
void ChebyPolys(double u, long n, double T[20], double U[20]) 
{
      long k;
      
      if (u < -1.0 || u > 1.0) {
         printf("u out of range in ChebPolys.  Bailing out.\n");
         exit(1);
      }
      if (n > 20) {
         printf("n out of range in ChebPolys.  Bailing out.\n");
         exit(1);
      }
      
      T[0] = 1.0;
      T[1] = u;
      U[0] = 1.0;
      U[1] = 2.0*u;
      for(k=1;k<n-1;k++) {
         T[k+1] = 2.0*u*T[k] - T[k-1];
         U[k+1] = 2.0*u*U[k] - U[k-1];
      }
}
/******************************************************************************/
/* Using ChebyPolys, find "position" (P) and scaled velocity (dPdu)           */
void ChebyInterp(double T[20], double U[20], double Coef[20], long n, 
   double *P, double *dPdu)
{
      long k;
      
      if (n > 20) {
         printf("n out of range in ChebyInterp.  Bailing out.\n");
         exit(1);
      }
      
      *P = Coef[0]*T[0];
      *dPdu = 0.0;
      for(k=1;k<n;k++) {
         *P += Coef[k]*T[k];
         *dPdu += Coef[k]*((double) k)*U[k-1];
      }
}
/******************************************************************************/
void FindChebyCoefs(double *u, double *P, long Nu, long Nc, double Coef[20])
{
      long i,j,k;
      double T[20],U[20];
      double **AtA, *x, *Atb;
      
      if (Nc > 20) {
         printf("Nc out of range in FindChebyCoefs.  Bailing out.\n");
         exit(1);
      }
      
      AtA = CreateMatrix(Nc,Nc);
      x = (double *) calloc(Nc,sizeof(double));
      Atb = (double *) calloc(Nc,sizeof(double));
      
      for(k=0;k<Nu;k++) {
         ChebyPolys(u[k],Nc,T,U);
         for(i=0;i<Nc;i++) {
            for(j=0;j<Nc;j++) {
               AtA[i][j] += T[i]*T[j];
            }
            Atb[i] += T[i]*P[k];
         }
      }
      LINSOLVE(AtA,x,Atb,Nc);
      for(i=0;i<Nc;i++) Coef[i] = x[i];
      for(i=Nc;i<20;i++) Coef[i] = 0.0;
      
      DestroyMatrix(AtA,Nc);
      free(x);
      free(Atb);
      
}
/******************************************************************************/
void VecToLngLat(double A[3], double *lng, double *lat)
{
      double B[3];
      
      if (MAGV(A) > 0.0) {
         CopyUnitV(A,B);
      
         *lng = atan2(B[1],B[0]);
      
         if (fabs(B[2]) < 1.0) *lat = asin(B[2]);
         else if (B[2] > 0.0) *lat = 2.0*atan(1.0);
         else *lat = -2.0*atan(1.0);
      }
      else {
         *lng = 0.0;
         *lat = 0.0;
      }
}
/* #ifdef __cplusplus
** }
** #endif
*/

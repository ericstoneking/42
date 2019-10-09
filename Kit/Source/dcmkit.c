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

/* #ifdef __cplusplus
** namespace Kit {
** #endif
*/

/**********************************************************************/
/*    Convert direction cosine matrix to quaternion.  Bulletproof.    */

void C2Q(double C[3][3], double Q[4])
{
      double K1,K2,K3,K4,K;

      K1=1.0+C[0][0]-C[1][1]-C[2][2];
      K2=1.0-C[0][0]+C[1][1]-C[2][2];
      K3=1.0-C[0][0]-C[1][1]+C[2][2];
      K4=1.0+C[0][0]+C[1][1]+C[2][2];

      K=K1;
      if (K2 > K) K=K2;
      if (K3 > K) K=K3;
      if (K4 > K) K=K4;

      if (K == K1) {
         Q[0]=0.5*sqrt(K1);
         Q[1]=0.25*(C[0][1]+C[1][0])/Q[0];
         Q[2]=0.25*(C[2][0]+C[0][2])/Q[0];
         Q[3]=0.25*(C[1][2]-C[2][1])/Q[0];
      }
      else if (K == K2) {
         Q[1]=0.5*sqrt(K2);
         Q[0]=0.25*(C[1][0]+C[0][1])/Q[1];
         Q[2]=0.25*(C[2][1]+C[1][2])/Q[1];
         Q[3]=0.25*(C[2][0]-C[0][2])/Q[1];
      }
      else if (K == K3) {
         Q[2]=0.5*sqrt(K3);
         Q[0]=0.25*(C[2][0]+C[0][2])/Q[2];
         Q[1]=0.25*(C[1][2]+C[2][1])/Q[2];
         Q[3]=0.25*(C[0][1]-C[1][0])/Q[2];
      }
      else {
         Q[3]=0.5*sqrt(K4);
         Q[0]=0.25*(C[1][2]-C[2][1])/Q[3];
         Q[1]=0.25*(C[2][0]-C[0][2])/Q[3];
         Q[2]=0.25*(C[0][1]-C[1][0])/Q[3];
      }
}
/**********************************************************************/
/*  Convert quaternion to direction cosine matrix                     */

void Q2C(double Q[4], double C[3][3])
{
      double TwoQ00,TwoQ11,TwoQ22;
      double TwoQ01,TwoQ02,TwoQ03;
      double TwoQ12,TwoQ13,TwoQ23;

      TwoQ00 = 2.0*Q[0]*Q[0];
      TwoQ11 = 2.0*Q[1]*Q[1];
      TwoQ22 = 2.0*Q[2]*Q[2];
      TwoQ01 = 2.0*Q[0]*Q[1];
      TwoQ02 = 2.0*Q[0]*Q[2];
      TwoQ03 = 2.0*Q[0]*Q[3];
      TwoQ12 = 2.0*Q[1]*Q[2];
      TwoQ13 = 2.0*Q[1]*Q[3];
      TwoQ23 = 2.0*Q[2]*Q[3];

      C[0][0] = 1.0-(TwoQ11+TwoQ22);
      C[0][1] = TwoQ01+TwoQ23;
      C[0][2] = TwoQ02-TwoQ13;
      C[1][0] = TwoQ01-TwoQ23;
      C[1][1] = 1.0-(TwoQ22+TwoQ00);
      C[1][2] = TwoQ12+TwoQ03;
      C[2][0] = TwoQ02+TwoQ13;
      C[2][1] = TwoQ12-TwoQ03;
      C[2][2] = 1.0-(TwoQ00+TwoQ11);
}
/**********************************************************************/
/*   Convert Euler angle sequence to direction cosine matrix          */

void A2C(long SEQ, double TH1, double TH2, double TH3, double C[3][3])
{
      double S1,C1;
      double S2 = 0.0;
      double C2 = 1.0;
      double S3 = 0.0;
      double C3 = 1.0;

      S1=sin(TH1);
      C1=cos(TH1);
      if (SEQ > 10) {  /* Two digits or more */
         S2=sin(TH2);
         C2=cos(TH2);
      }
      if (SEQ > 100) { /* Three digits */
         S3=sin(TH3);
         C3=cos(TH3);
      }

      if (SEQ == 1) {
         C[0][0]=1.0;
         C[1][0]=0.0;
         C[2][0]=0.0;
         C[0][1]=0.0;
         C[1][1]=C1;
         C[2][1]=-S1;
         C[0][2]=0.0;
         C[1][2]=S1;
         C[2][2]=C1;
      }
      else if (SEQ == 2) {
         C[0][0]=C1;
         C[1][0]=0.0;
         C[2][0]=S1;
         C[0][1]=0.0;
         C[1][1]=1.0;
         C[2][1]=0.0;
         C[0][2]=-S1;
         C[1][2]=0.0;
         C[2][2]=C1;
      }
      else if (SEQ == 3) {
         C[0][0]=C1;
         C[1][0]=-S1;
         C[2][0]=0.0;
         C[0][1]=S1;
         C[1][1]=C1;
         C[2][1]=0.0;
         C[0][2]=0.0;
         C[1][2]=0.0;
         C[2][2]=1.0;
      }
      else if (SEQ == 12) {
         C[0][0]=C2;
         C[1][0]=0.0;
         C[2][0]=S2;
         C[0][1]=S1*S2;
         C[1][1]=C1;
         C[2][1]=-S1*C2;
         C[0][2]=-C1*S2;
         C[1][2]=S1;
         C[2][2]=C1*C2;
      }
      else if (SEQ == 13) {
         C[0][0]=C2;
         C[1][0]=-S2;
         C[2][0]=0.0;
         C[0][1]=C1*S2;
         C[1][1]=C1*C2;
         C[2][1]=-S1;
         C[0][2]=S1*S2;
         C[1][2]=S1*C2;
         C[2][2]=C1;
      }
      else if (SEQ == 21) {
         C[0][0]=C1;
         C[1][0]=S1*S2;
         C[2][0]=S1*C2;
         C[0][1]=0.0;
         C[1][1]=C2;
         C[2][1]=-S2;
         C[0][2]=-S1;
         C[1][2]=C1*S2;
         C[2][2]=C1*C2;
      }
      else if (SEQ == 23) {
         C[0][0]=C1*C2;
         C[1][0]=-C1*S2;
         C[2][0]=S1;
         C[0][1]=S2;
         C[1][1]=C2;
         C[2][1]=0.0;
         C[0][2]=-S1*C2;
         C[1][2]=S1*S2;
         C[2][2]=C1;
      }
      else if (SEQ == 31) {
         C[0][0]=C1;
         C[1][0]=-S1*C2;
         C[2][0]=S1*S2;
         C[0][1]=S1;
         C[1][1]=C1*C2;
         C[2][1]=-C1*S2;
         C[0][2]=0.0;
         C[1][2]=S2;
         C[2][2]=C2;
      }
      else if (SEQ == 32) {
         C[0][0]=C1*C2;
         C[1][0]=-S1;
         C[2][0]=C1*S2;
         C[0][1]=S1*C2;
         C[1][1]=C1;
         C[2][1]=S1*S2;
         C[0][2]=-S2;
         C[1][2]=0.0;
         C[2][2]=C2;
      }
      else if (SEQ == 123) {
         C[0][0]=C2*C3;
         C[1][0]=-C2*S3;
         C[2][0]=S2;
         C[0][1]=S1*S2*C3+S3*C1;
         C[1][1]=-S1*S2*S3+C3*C1;
         C[2][1]=-S1*C2;
         C[0][2]=-C1*S2*C3+S3*S1;
         C[1][2]=C1*S2*S3+C3*S1;
         C[2][2]=C1*C2;
      }
      else if (SEQ == 231) {
         C[0][0]=C1*C2;
         C[1][0]=-C1*S2*C3+S3*S1;
         C[2][0]=C1*S2*S3+C3*S1;
         C[0][1]=S2;
         C[1][1]=C2*C3;
         C[2][1]=-C2*S3;
         C[0][2]=-S1*C2;
         C[1][2]=S1*S2*C3+S3*C1;
         C[2][2]=-S1*S2*S3+C3*C1;
      }
      else if (SEQ == 312) {
         C[0][0]=-S1*S2*S3+C3*C1;
         C[1][0]=-S1*C2;
         C[2][0]=S1*S2*C3+S3*C1;
         C[0][1]=C1*S2*S3+C3*S1;
         C[1][1]=C1*C2;
         C[2][1]=-C1*S2*C3+S3*S1;
         C[0][2]=-C2*S3;
         C[1][2]=S2;
         C[2][2]=C2*C3;
      }
      else if (SEQ == 132) {
         C[0][0]=C2*C3;
         C[1][0]=-S2;
         C[2][0]=C2*S3;
         C[0][1]=C1*S2*C3+S3*S1;
         C[1][1]=C1*C2;
         C[2][1]=C1*S2*S3-C3*S1;
         C[0][2]=S1*S2*C3-S3*C1;
         C[1][2]=S1*C2;
         C[2][2]=S1*S2*S3+C3*C1;
      }
      else if (SEQ == 213) {
         C[0][0]=S1*S2*S3+C3*C1;
         C[1][0]=S1*S2*C3-S3*C1;
         C[2][0]=S1*C2;
         C[0][1]=C2*S3;
         C[1][1]=C2*C3;
         C[2][1]=-S2;
         C[0][2]=C1*S2*S3-C3*S1;
         C[1][2]=C1*S2*C3+S3*S1;
         C[2][2]=C1*C2;
      }
      else if (SEQ == 321) {
         C[0][0]=C1*C2;
         C[1][0]=C1*S2*S3-C3*S1;
         C[2][0]=C1*S2*C3+S3*S1;
         C[0][1]=S1*C2;
         C[1][1]=S1*S2*S3+C3*C1;
         C[2][1]=S1*S2*C3-S3*C1;
         C[0][2]=-S2;
         C[1][2]=C2*S3;
         C[2][2]=C2*C3;
      }
      else if (SEQ == 121) {
         C[0][0]=C2;
         C[1][0]=S2*S3;
         C[2][0]=S2*C3;
         C[0][1]=S1*S2;
         C[1][1]=-S1*C2*S3+C3*C1;
         C[2][1]=-S1*C2*C3-S3*C1;
         C[0][2]=-C1*S2;
         C[1][2]=C1*C2*S3+C3*S1;
         C[2][2]=C1*C2*C3-S3*S1;
      }
      else if (SEQ == 131) {
         C[0][0]=C2;
         C[1][0]=-S2*C3;
         C[2][0]=S2*S3;
         C[0][1]=C1*S2;
         C[1][1]=C1*C2*C3-S3*S1;
         C[2][1]=-C1*C2*S3-C3*S1;
         C[0][2]=S1*S2;
         C[1][2]=S1*C2*C3+S3*C1;
         C[2][2]=-S1*C2*S3+C3*C1;
      }
      else if (SEQ == 212) {
         C[0][0]=-S1*C2*S3+C3*C1;
         C[1][0]=S1*S2;
         C[2][0]=S1*C2*C3+S3*C1;
         C[0][1]=S2*S3;
         C[1][1]=C2;
         C[2][1]=-S2*C3;
         C[0][2]=-C1*C2*S3-C3*S1;
         C[1][2]=C1*S2;
         C[2][2]=C1*C2*C3-S3*S1;
      }
      else if (SEQ == 232) {
         C[0][0]=C1*C2*C3-S1*S3;
         C[1][0]=-C1*S2;
         C[2][0]=C1*C2*S3+S1*C3;
         C[0][1]=S2*C3;
         C[1][1]=C2;
         C[2][1]=S2*S3;
         C[0][2]=-S1*C2*C3-C1*S3;
         C[1][2]=S1*S2;
         C[2][2]=-S1*C2*S3+C1*C3;
      }
      else if (SEQ == 313) {
         C[0][0]=-S1*C2*S3+C3*C1;
         C[1][0]=-S1*C2*C3-S3*C1;
         C[2][0]=S1*S2;
         C[0][1]=C1*C2*S3+C3*S1;
         C[1][1]=C1*C2*C3-S3*S1;
         C[2][1]=-C1*S2;
         C[0][2]=S2*S3;
         C[1][2]=S2*C3;
         C[2][2]=C2;
      }
      else if (SEQ == 323) {
         C[0][0]=C1*C2*C3-S3*S1;
         C[1][0]=-C1*C2*S3-C3*S1;
         C[2][0]=C1*S2;
         C[0][1]=S1*C2*C3+S3*C1;
         C[1][1]=-S1*C2*S3+C3*C1;
         C[2][1]=S1*S2;
         C[0][2]=-S2*C3;
         C[1][2]=S2*S3;
         C[2][2]=C2;
      }
      else {
         printf("Bogus Euler Sequence %ld in A2C\n",SEQ);
         exit(1);
      }
}
/**********************************************************************/
/*  Convert direction cosine matrix to Euler angles                   */

void C2A(long SEQ, double C[3][3],double *TH1, double *TH2, double *TH3)
{

      if (SEQ == 123) {
         *TH1=atan2(-C[2][1],C[2][2]);
         *TH2=asin(C[2][0]);
         *TH3=atan2(-C[1][0],C[0][0]);
      }
      else if (SEQ == 231) {
         *TH1=atan2(-C[0][2],C[0][0]);
         *TH2=asin(C[0][1]);
         *TH3=atan2(-C[2][1],C[1][1]);
      }
      else if (SEQ == 312) {
         *TH1=atan2(-C[1][0],C[1][1]);
         *TH2=asin(C[1][2]);
         *TH3=atan2(-C[0][2],C[2][2]);
      }
      else if (SEQ == 132) {
         *TH1=atan2(C[1][2],C[1][1]);
         *TH2=asin(-C[1][0]);
         *TH3=atan2(C[2][0],C[0][0]);
      }
      else if (SEQ == 213) {
         *TH1=atan2(C[2][0],C[2][2]);
         *TH2=asin(-C[2][1]);
         *TH3=atan2(C[0][1],C[1][1]);
      }
      else if (SEQ == 321) {
         *TH1=atan2(C[0][1],C[0][0]);
         *TH2=asin(-C[0][2]);
         *TH3=atan2(C[1][2],C[2][2]);
      }
      else if (SEQ == 121) {
         *TH1=atan2(C[0][1],-C[0][2]);
         *TH2=acos(C[0][0]);
         *TH3=atan2(C[1][0],C[2][0]);
      }
      else if (SEQ == 131) {
         *TH1=atan2(C[0][2],C[0][1]);
         *TH2=acos(C[0][0]);
         *TH3=atan2(C[2][0],-C[1][0]);
      }
      else if (SEQ == 212) {
         *TH1=atan2(C[1][0],C[1][2]);
         *TH2=acos(C[1][1]);
         *TH3=atan2(C[0][1],-C[2][1]);
      }
      else if (SEQ == 232) {
         *TH1=atan2(C[1][2],-C[1][0]);
         *TH2=acos(C[1][1]);
         *TH3=atan2(C[2][1],C[0][1]);
      }
      else if (SEQ == 313) {
         *TH1=atan2(C[2][0],-C[2][1]);
         *TH2=acos(C[2][2]);
         *TH3=atan2(C[0][2],C[1][2]);
      }
      else if (SEQ == 323) {
         *TH1=atan2(C[2][1],C[2][0]);
         *TH2=acos(C[2][2]);
         *TH3=atan2(C[1][2],-C[0][2]);
      }
      else {
         printf("Bogus Euler Sequence %ld in C2A\n",SEQ);
         exit(1);
      }
}
/**********************************************************************/
/* Compute direction cosine matrix corresponding to a                 */
/* simple rotation of THETA radians about a unit vector               */
/* parallel to AXIS                                                   */

void SimpRot(double AXIS[3], double THETA, double C[3][3])
{
      double CTH,STH,CTH1,AX[3];

      CTH=cos(THETA);
      STH=sin(THETA);
      CTH1=1.0-CTH;
      CopyUnitV(AXIS,AX);

      C[0][0]= CTH + AX[0]*AX[0]*CTH1;
      C[1][0]= -AX[2]*STH+AX[0]*AX[1]*CTH1;
      C[2][0]=  AX[1]*STH+AX[2]*AX[0]*CTH1;
      C[0][1]=  AX[2]*STH+AX[0]*AX[1]*CTH1;
      C[1][1]= CTH + AX[1]*AX[1]*CTH1;
      C[2][1]= -AX[0]*STH+AX[1]*AX[2]*CTH1;
      C[0][2]= -AX[1]*STH+AX[2]*AX[0]*CTH1;
      C[1][2]=  AX[0]*STH+AX[1]*AX[2]*CTH1;
      C[2][2]= CTH + AX[2]*AX[2]*CTH1;

}
/**********************************************************************/
void Q2AngleVec(double Q[4], double AngleVec[3])
{
      double s;

      UNITQ(Q);

      if (Q[3] > 0.0) s = 2.0/sinc(acos(Q[3]));
      else s = -2.0/sinc(acos(-Q[3]));

      AngleVec[0] = Q[0]*s;
      AngleVec[1] = Q[1]*s;
      AngleVec[2] = Q[2]*s;
}
/**********************************************************************/
/*  Given body rates and quaternion, find qdot.  Ref Kane, 1.13       */
void QW2QDOT(double Q[4],double W[3],double QDOT[4])
{

      QDOT[0] = 0.5*( W[0]*Q[3]-W[1]*Q[2]+W[2]*Q[1]);
      QDOT[1] = 0.5*( W[0]*Q[2]+W[1]*Q[3]-W[2]*Q[0]);
      QDOT[2] = 0.5*(-W[0]*Q[1]+W[1]*Q[0]+W[2]*Q[3]);
      QDOT[3] = 0.5*(-W[0]*Q[0]-W[1]*Q[1]-W[2]*Q[2]);

}

/****************************************************************************/
/*  Given body rates and quaternion,find qdot. ref Zimmerman, GSFC, 1969    */
/*void MQW2QDOT(double Q[4], double W[3], double DT, double QDOT[4])        */
/*{                                                                         */
/*    QDOT[0] = 0.5*( W[0]*Q[3]-W[1]*Q[2]+W[2]*Q[1])+DT*(1-(Q[0]*Q[0]+Q[1]*Q[1]+Q[2]*Q[2]+Q[3]*Q[3]))*Q[0];*/
/*    QDOT[1] = 0.5*( W[0]*Q[2]+W[1]*Q[3]-W[2]*Q[0])+DT*(1-(Q[0]*Q[0]+Q[1]*Q[1]+Q[2]*Q[2]+Q[3]*Q[3]))*Q[1];*/
/*    QDOT[2] = 0.5*(-W[0]*Q[1]+W[1]*Q[0]+W[2]*Q[3])+DT*(1-(Q[0]*Q[0]+Q[1]*Q[1]+Q[2]*Q[2]+Q[3]*Q[3]))*Q[2];*/
/*    QDOT[3] = 0.5*(-W[0]*Q[0]-W[1]*Q[1]-W[2]*Q[2])+DT*(1-(Q[0]*Q[0]+Q[1]*Q[1]+Q[2]*Q[2]+Q[3]*Q[3]))*Q[3];*/

/*}*/

/**********************************************************************/
/*  Parallel axis theorem.                                            */
/*     IB:  Central inertia matrix of a body B                        */
/*     CBA: Dircos from frame A to frame B                            */
/*     m:   Mass of B                                                 */
/*     pba: Location of mass center of B, wrt origin of A             */
/*     IBA: Inertia of B about the origin of A, expressed in A        */
void PARAXIS(double IB[3][3], double CBA[3][3], double m, double pba[3],
             double IBA[3][3])
{

      double CI[3][3],CIC[3][3],p2,pp[3][3];
      long i,j,k;


      for(i=0;i<3;i++){
         for(j=0;j<3;j++){
            CI[i][j] = 0.0;
            for(k=0;k<3;k++){
               CI[i][j] += CBA[k][i]*IB[k][j];
            }
         }
      }
      for(i=0;i<3;i++){
         for(j=0;j<3;j++){
            CIC[i][j] = 0.0;
            for(k=0;k<3;k++){
               CIC[i][j] += CI[i][k]*CBA[k][j];
            }
         }
      }
      p2=pba[0]*pba[0]+pba[1]*pba[1]+pba[2]*pba[2];

      for(i=0;i<3;i++){
         for(j=0;j<3;j++){
            pp[i][j]=-pba[i]*pba[j];
         }
         pp[i][i]+=p2;
      }

      for(i=0;i<3;i++){
         for(j=0;j<3;j++){
            IBA[i][j]=CIC[i][j]+m*pp[i][j];
         }
      }

}
/******************************************************************************/
void PrincipalMOI(double Ib[3][3], double Ip[3], double CPB[3][3])
{
      double Tol = 1.0E-12;
      long MaxK = 100;
      double I[3][3];
      double C[3][3] = {{1.0,0.0,0.0},{0.0,1.0,0.0},{0.0,0.0,1.0}};
      double CI[3][3],CICT[3][3],CCPB[3][3];
      double MaxOffDiag,th,MaxEl,Swap;
      long i,j,k;
      long id,jd;
      long Jmax;
      long Done;

      for(i=0;i<3;i++) {
         for(j=0;j<3;j++) {
            I[i][j] = Ib[i][j];
            CPB[i][j] = 0.0;
         }
         CPB[i][i] = 1.0;
      }
      k = 1;
      Done = 0;

      while(!Done) {
         MaxOffDiag = fabs(I[0][1]);
         id = 0;
         jd = 1;
         if (fabs(I[0][2]) > MaxOffDiag) {
            MaxOffDiag = fabs(I[0][2]);
            id = 0;
            jd = 2;
         }
         if (fabs(I[1][2]) > MaxOffDiag) {
            MaxOffDiag = fabs(I[1][2]);
            id = 1;
            jd = 2;
         }
      
         if (I[id][id] == I[jd][jd]) {
            th = 0.78540; /* pi/4 */
         }
         else {
            th = 0.5*atan2(2.0*I[id][jd],I[id][id]-I[jd][jd]);
         }
      
         for(i=0;i<3;i++) {
            for(j=0;j<3;j++) {
               C[i][j] = 0.0;
            }
            C[i][i] = 1.0;
         }
         C[id][id] =  cos(th);
         C[jd][jd] =  cos(th);
         C[id][jd] =  sin(th);
         C[jd][id] = -sin(th);
      
         MxM(C,I,CI);
         MxMT(CI,C,CICT);
         MxM(C,CPB,CCPB);
         for(i=0;i<3;i++) {
            for(j=0;j<3;j++) {
               I[i][j] = CICT[i][j];
               CPB[i][j] = CCPB[i][j];
            }
         }
      
         k++;
         if (MaxOffDiag < Tol || k > MaxK) Done = 1;
      }
      
      for(i=0;i<3;i++) Ip[i] = I[i][i];
      
      /* Flip Signs to make max elements in each row positive */
      for(i=0;i<3;i++) {
         MaxEl = fabs(CPB[i][0]);
         Jmax = 0;
         if (fabs(CPB[i][1]) > MaxEl) {
            MaxEl = fabs(CPB[i][1]);
            Jmax = 1;
         }
         if (fabs(CPB[i][2]) > MaxEl) {
            MaxEl = fabs(CPB[i][2]);
            Jmax = 2;
         }
         if (CPB[i][Jmax] < 0.0) {
            for(j=0;j<3;j++) CPB[i][j] *= -1.0;
         }
      }
      /* Permute rows to make CPB as diagonal as possible */
      Done = 0;
      while(!Done) {
         Done = 1;
         for(i=0;i<3;i++) {
            for(j=0;j<3;j++) {
               if (fabs(CPB[j][i]) > fabs(CPB[i][i])) {
                  Done = 0;
                  for(k=0;k<3;k++) {
                     Swap = CPB[j][k];
                     CPB[j][k] = CPB[i][k];
                     CPB[i][k] = Swap;
                  }
                  Swap = Ip[j];
                  Ip[j] = Ip[i];
                  Ip[i] = Swap;
               }
            }
         }
      }
}
/**********************************************************************/
/*  Given quaternion measurements, find body rates.  Ref Kane, 1.13   */
void Q2W(double q[4], double qdot[4], double w[3])
{
      w[0] = 2.0*( qdot[0]*q[3]+qdot[1]*q[2]
                  -qdot[2]*q[1]-qdot[3]*q[0]);

      w[1] = 2.0*(-qdot[0]*q[2]+qdot[1]*q[3]
                  +qdot[2]*q[0]-qdot[3]*q[1]);

      w[2] = 2.0*( qdot[0]*q[1]-qdot[1]*q[0]
                  +qdot[2]*q[3]-qdot[3]*q[2]);
}
/**********************************************************************/
/*  Finds rotational and translational joint partials                 */
/*  On Init, populate all matrix elements.  Else, only populate       */
/*  variable ones.                                                    */
void JointPartials(long Init, long IsSpherical,
   long RotSeq, long TrnSeq, double ang[3], double sig[3],
   double Gamma[3][3], double Gs[3], double Gds[3],
   double s[3], double Delta[3][3], double Ds[3], double Dds[3])
{
      double s2,c2,s3,c3;
      long i1,i2,i3,Cyclic,i,j;

      if (Init) {
         if (IsSpherical) {
            Gamma[0][0] = 1.0;
            Gamma[0][1] = 0.0;
            Gamma[0][2] = 0.0;
            Gamma[1][0] = 0.0;
            Gamma[1][1] = 1.0;
            Gamma[1][2] = 0.0;
            Gamma[2][0] = 0.0;
            Gamma[2][1] = 0.0;
            Gamma[2][2] = 1.0;
         }
         else {
            for(i=0;i<3;i++) {
               for(j=0;j<3;j++) {
                  Gamma[i][j] = 0.0;
               }
            }
         }
         for(i=0;i<3;i++) {
            for(j=0;j<3;j++) {
               Delta[i][j] = 0.0;
            }
            Gs[i] = 0.0;
            Gds[i] = 0.0;
            Ds[i] = 0.0;
            Dds[i] = 0.0;
         }
         i3 = RotSeq % 10;         /* Pick off third digit */
         i2 = (RotSeq % 100)/10;   /* Extract second digit */
         i1 = RotSeq/100;          /* Pick off first digit */
         Gamma[i3-1][2] = 1.0;

         i3 = TrnSeq % 10;         /* Pick off third digit */
         i2 = (TrnSeq % 100)/10;   /* Extract second digit */
         i1 = TrnSeq/100;          /* Pick off first digit */

         Delta[i1-1][0] = 1.0;
         Delta[i2-1][1] = 1.0;
         Delta[i3-1][2] = 1.0;

      }

      if (IsSpherical) {
         Gs[0] = sig[0];
         Gs[1] = sig[1];
         Gs[2] = sig[2];
      }
      else {
         i3 = RotSeq % 10;         /* Pick off third digit */
         i2 = (RotSeq % 100)/10;   /* Extract second digit */
         i1 = RotSeq/100;          /* Pick off first digit */

         s2 = sin(ang[1]);
         c2 = cos(ang[1]);
         s3 = sin(ang[2]);
         c3 = cos(ang[2]);

         Cyclic = (i2-i1)*(i3-i2)*(i3-i1);
         /* Convert (123) style to [012] subscripts */
         i1--;
         i2--;
         i3--;
         if (Cyclic > 0) {  /* 123, 231, 312 */
            Gamma[i1][0] = c2*c3;
            Gamma[i1][1] = s3;
            Gamma[i2][0] = -c2*s3;
            Gamma[i2][1] = c3;
            Gamma[i3][0] = s2;
            Gds[i1] = -sig[0]*(sig[1]*s2*c3+sig[2]*c2*s3)+sig[1]*sig[2]*c3;
            Gds[i2] = sig[0]*(sig[1]*s2*s3-sig[2]*c2*c3)-sig[1]*sig[2]*s3;
            Gds[i3] = sig[0]*sig[1]*c2;
         }
         else if (Cyclic < 0) { /* 321, 132, 213 */
            Gamma[i1][0] = c2*c3;
            Gamma[i1][1] = -s3;
            Gamma[i2][0] = c2*s3;
            Gamma[i2][1] = c3;
            Gamma[i3][0] = -s2;
            Gds[i1] = -sig[0]*(sig[1]*s2*c3+sig[2]*c2*s3)-sig[1]*sig[2]*c3;
            Gds[i2] = sig[0]*(-sig[1]*s2*s3+sig[2]*c2*c3)-sig[1]*sig[2]*s3;
            Gds[i3] = -sig[0]*sig[1]*c2;
         }
         else {
            printf("Bogus RotSeq %ld in JointPartials\n",RotSeq);
            exit(1);
         }
         for(i=0;i<3;i++) Gs[i] = Gamma[i][0]*sig[0]
                                + Gamma[i][1]*sig[1]
                                + Gamma[i][2]*sig[2];
      }

      i3 = TrnSeq % 10;         /* Pick off third digit */
      i2 = (TrnSeq % 100)/10;   /* Extract second digit */
      i1 = TrnSeq/100;          /* Pick off first digit */
      i1--;
      i2--;
      i3--;

      Ds[i1] = s[0];
      Ds[i2] = s[1];
      Ds[i3] = s[2];
}
/**********************************************************************/
void ADOT2W(long IsSpherical, long Seq, double ang[3], double u[3], double w[3])
{
      double s2,c2,s3,c3;
      long k,i1,i2,i3,Cyclic;

      if (IsSpherical) {
         for(k=0;k<3;k++) w[k] = u[k];
      }
      else {
         i3 = Seq % 10;         /* Pick off third digit */
         i2 = (Seq % 100)/10;   /* Extract second digit */
         i1 = Seq/100;          /* Pick off first digit */

         s2 = sin(ang[1]);
         c2 = cos(ang[1]);
         s3 = sin(ang[2]);
         c3 = cos(ang[2]);

         Cyclic = (i2-i1)*(i3-i2)*(i3-i1);
         /* Convert (123) style to [012] subscripts */
         i1--;
         i2--;
         i3--;
         if (Cyclic > 0) {  /* 123, 231, 312 */
            w[i1] = c2*c3*u[0] + s3*u[1];
            w[i2] = -c2*s3*u[0] + c3*u[1];
            w[i3] = s2*u[0] + u[2];
         }
         else if (Cyclic < 0) { /* 321, 132, 213 */
            w[i1] = c2*c3*u[0] - s3*u[1];
            w[i2] = c2*s3*u[0] + c3*u[1];
            w[i3] = -s2*u[0] + u[2];
         }
         else if ((i2-i1+3)%3 == 1) { /* 121, 232, 313 */
            i3 = (i2+4)%3;
            w[i1] = c2*u[0] + u[2];
            w[i2] = s2*s3*u[0] + c3*u[1];
            w[i3] = s2*c3*u[0] - s3*u[1];
         }
         else if ((i2-i1+3)%3 == 2) {  /* 212, 323, 131 */
            i3 = (i2+2)%3;
            w[i1] = c2*u[0] + u[2];
            w[i2] = s2*s3*u[0] + c3*u[1];
            w[i3] = -s2*c3*u[0] + s3*u[1];
         }
         else {
            printf("Bogus Seq %ld in ADOT2W\n",Seq);
            exit(1);
         }
      }
}
/**********************************************************************/
void W2ADOT(long Seq, double ang[3], double w[3], double adot[3])
{
      double s2,c2,s3,c3;
      long i1,i2,i3,Cyclic;

      i3 = Seq % 10;         /* Pick off third digit */
      i2 = (Seq % 100)/10;   /* Extract second digit */
      i1 = Seq/100;          /* Pick off first digit */

      s2 = sin(ang[1]);
      c2 = cos(ang[1]);
      s3 = sin(ang[2]);
      c3 = cos(ang[2]);


      Cyclic = (i2-i1)*(i3-i2)*(i3-i1);
      /* Convert (123) style to [012] subscripts */
      i1--;
      i2--;
      i3--;
      if (Cyclic > 0) {  /* 123, 231, 312 */
         if (fabs(c2) < 1.0E-6)
            printf("Joint near gimbal lock in W2ADOT\n");
         adot[0] = (w[i1]*c3-w[i2]*s3)/c2;
         adot[1] = w[i1]*s3+w[i2]*c3;
         adot[2] = (-w[i1]*c3+w[i2]*s3)*s2/c2 + w[i3];
      }
      else if (Cyclic < 0) { /* 321, 132, 213 */
         if (fabs(c2) < 1.0E-6)
            printf("Joint near gimbal lock in W2ADOT\n");
         adot[0] = (w[i2]*s3+w[i1]*c3)/c2;
         adot[1] = w[i2]*c3-w[i1]*s3;
         adot[2] = w[i3]+(w[i2]*s3+w[i1]*c3)*s2/c2;
      }
      else if ((i2-i1+3)%3 == 1) { /* 121, 232, 313 */
         i3 = (i2+4)%3;
         if (fabs(s2) < 1.0E-6)
            printf("Joint near indeterminate in W2ADOT\n");
         adot[0] = (w[i2]*s3+w[i3]*c3)/s2;
         adot[1] = w[i2]*c3-w[i3]*s3;
         adot[2] = w[i1]-(w[i2]*s3+w[i3]*c3)*c2/s2;
      }
      else if ((i2-i1+3)%3 == 2) {  /* 212, 323, 131 */
         i3 = (i2+2)%3;
         if (fabs(s2) < 1.0E-6)
            printf("Joint near indeterminate in W2ADOT\n");
         adot[0] = (w[i2]*s3-w[i3]*c3)/s2;
         adot[1] = w[i2]*c3+w[i3]*s3;
         adot[2] = w[i1]+(w[i3]*c3-w[i2]*s3)*c2/s2;
      }
      else {
         printf("Bogus Seq %ld in W2ADOT\n",Seq);
         exit(1);
      }
}
/**********************************************************************/
void W2CDOT(double w[3], double C[3][3], double Cdot[3][3])
{
      Cdot[0][0] = C[1][0]*w[2]-C[2][0]*w[1];
      Cdot[1][0] = C[2][0]*w[0]-C[0][0]*w[2];
      Cdot[2][0] = C[0][0]*w[1]-C[1][0]*w[0];
      Cdot[0][1] = C[1][1]*w[2]-C[2][1]*w[1];
      Cdot[1][1] = C[2][1]*w[0]-C[0][1]*w[2];
      Cdot[2][1] = C[0][1]*w[1]-C[1][1]*w[0];
      Cdot[0][2] = C[1][2]*w[2]-C[2][2]*w[1];
      Cdot[1][2] = C[2][2]*w[0]-C[0][2]*w[2];
      Cdot[2][2] = C[0][2]*w[1]-C[1][2]*w[0];
}
/**********************************************************************/
void CDOT2W(double C[3][3], double Cdot[3][3], double w[3])
{
      w[0] = C[2][0]*Cdot[1][0]+C[2][1]*Cdot[1][1]+C[2][2]*Cdot[1][2];
      w[1] = C[0][1]*Cdot[2][1]+C[0][2]*Cdot[2][2]+C[0][0]*Cdot[2][0];
      w[2] = C[1][2]*Cdot[0][2]+C[1][0]*Cdot[0][0]+C[1][1]*Cdot[0][1];
}
/**********************************************************************/



/* #ifdef __cplusplus
** }
** #endif
*/

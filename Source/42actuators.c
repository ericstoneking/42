/*    This file is distributed with 42,                               */
/*    the (mostly harmless) spacecraft dynamics simulation            */
/*    created by Eric Stoneking of NASA Goddard Space Flight Center   */

/*    Copyright 2010 United States Government                         */
/*    as represented by the Administrator                             */
/*    of the National Aeronautics and Space Administration.           */

/*    No copyright is claimed in the United States                    */
/*    under Title 17, U.S. Code.                                      */

/*    All Other Rights Reserved.                                      */


#include "42.h"

/* #ifdef __cplusplus
** namespace _42 {
** using namespace Kit;
** #endif
*/

/**********************************************************************/
void ThrModel(double ImpulseCmd,double DT,double cm[3],
              struct ThrusterType *Thr)
{

      double r[3];
      long i;

      Thr->F = ImpulseCmd/DT;
      if (Thr->F < 0.0) Thr->F = 0.0;
      if (Thr->F > Thr->Fmax) Thr->F = Thr->Fmax;

      Thr->Frc[0] = Thr->F*Thr->A[0];
      Thr->Frc[1] = Thr->F*Thr->A[1];
      Thr->Frc[2] = Thr->F*Thr->A[2];

      for(i=0;i<3;i++) r[i] = Thr->r[i] - cm[i];
      VxV(r,Thr->Frc,Thr->Trq);

}
/**********************************************************************/
void WhlModel(double Tcmd,double H,double Hmax,double Tmax, double *Trq)
{

      *Trq = Tcmd;
      if (*Trq < -Tmax) *Trq = -Tmax;
      if (*Trq >  Tmax) *Trq =  Tmax;
      if (*Trq < 0.0 && H <= -Hmax) *Trq = 0.0;
      if (*Trq > 0.0 && H >=  Hmax) *Trq = 0.0;


}
/**********************************************************************/
void MTBModel(double bvb[3],double Mcmd, double Mmax, double A[3],
              double *M, double Trq[3])
{

      *M = Mcmd;
      if(*M < -Mmax) *M = -Mmax;
      if(*M >  Mmax) *M =  Mmax;

      Trq[0] = *M * (A[1]*bvb[2]-A[2]*bvb[1]);
      Trq[1] = *M * (A[2]*bvb[0]-A[0]*bvb[2]);
      Trq[2] = *M * (A[0]*bvb[1]-A[1]*bvb[0]);

}
/**********************************************************************/
void GimbalModel(long DOF,double Rate[3],double Ang[3],
                 double RateCmd[3],double AngCmd[3],
                 double RateGain[3],double AngGain[3],
                 double MaxRate[3], double MaxTrq[3], double Trq[3])
{
      double DesiredRate,AngErr;
      long i;

      for(i=0;i<DOF;i++) {
         AngErr = fmod(Ang[i] - AngCmd[i],TwoPi);
         if (AngErr > Pi) AngErr -= TwoPi;
         if (AngErr < -Pi) AngErr += TwoPi;
         DesiredRate = Limit(RateCmd[i] - AngGain[i]*AngErr,
                             -MaxRate[i],MaxRate[i]);
         Trq[i] = Limit(-RateGain[i]*(Rate[i]-DesiredRate),
                        -MaxTrq[i],MaxTrq[i]);
      }
}
/**********************************************************************/
void TranslationalModel(long DOF, double Rate[3], double Pos[3],
                        double RateCmd[3], double PosCmd[3],
                        double RateGain[3], double PosGain[3],
                        double MaxRate[3], double MaxFrc[3], double Frc[3])
{
   double DesiredRate,PosErr;
   long i;

   for(i=0;i<DOF;i++) {
      PosErr = Pos[i] - PosCmd[i];
      DesiredRate = Limit(RateCmd[i] - PosGain[i]*PosErr,
                          -MaxRate[i],MaxRate[i]);
      Frc[i] = Limit(-RateGain[i]*(Rate[i]-DesiredRate),
                     -MaxFrc[i],MaxFrc[i]);
   }
}
/**********************************************************************/
void ThrusterPlumeFrcTrq(struct SCType *S)
{
      /* Plume Parameters */
      double Temp = 100.0; /* WAG */
      double R = 8.134; /* J/(K*mol) */
      double Beta = 1.0/sqrt(2.0*R*Temp);
      double Ve = 100.0; /* WAG */
      double s = Beta*Ve;
      double y = s;
      double A1 = exp(-y*y)+sqrt(Pi)*y*(1.0+erf(y));
      double mdot = 0.1; /* WAG */
      double Coef = mdot/(Beta*A1*Pi);
      /* Other variables */
      struct ThrusterType *T;
      struct BodyType *B;
      struct GeomType *G;
      struct PolyType *P;
      double PosThrN[3],PosThrB[3],AxisN[3],CPB[3][3];
      double AoN,PosB[3],PosP[3],MagPos,Phat[3],cosphi;
      double w,w2,Wpoly,TotalCoef,FrcP[3],FrcB[3],r[3],TrqB[3],FrcN[3];
      long Ithr,Ipoly,Ib,i;

      for(Ithr=0;Ithr<S->Nthr;Ithr++) {
         T = &S->Thr[Ithr];
         if (T->F > 0.0) { /* Check that this is legit */

            /* Find Force and Torque on each Body */
            for(Ib=0;Ib<S->Nb;Ib++) {
               B = &S->B[Ib];
               G = &Geom[B->GeomTag];

               /* Find thruster location, axis in B */
               MTxV(S->B[0].CN,T->r,PosThrN);
               for(i=0;i<3;i++) PosThrN[i] += S->B[0].pn[i] - B->pn[i];
               MxV(B->CN,PosThrN,PosThrB);
               /* Note that plume axis is opposite T->A */
               /* CPB is DCM from B to Plume (P) frame */
               MTxV(S->B[0].CN,T->A,AxisN);
               MxV(B->CN,AxisN,CPB[0]);
               for(i=0;i<3;i++) CPB[0][i] = -CPB[0][i];
               PerpBasis(CPB[0],CPB[1],CPB[2]);

               /* Find force and torque on each illuminated polygon */
               for(Ipoly=0;Ipoly<G->Npoly;Ipoly++) {
                  P = &G->Poly[Ipoly];
                  if (strcmp(Matl[P->Matl].Label,"INTERIOR")) { /* Plume doesn't see interior polys */
                     AoN = VoV(CPB[0],P->Norm);
                     if (AoN < 0.0) {  /* Plume doesn't see polys facing away */
                        /* Find plume pressure (momentum flux) at poly centroid */
                        for(i=0;i<3;i++) PosB[i] = P->Centroid[i] - PosThrB[i];
                        MxV(CPB,PosB,PosP);
                        if (PosP[0] > 0.0) { /* Ignore backflow */
                           MagPos = CopyUnitV(PosP,Phat);
                           cosphi = Phat[0];
                           w = s*cosphi;
                           w2 = w*w;
                           Wpoly = (w2+2.5)*w*exp(-w2);
                           Wpoly += (0.75+3.0*w2+w2*w2)*sqrt(Pi)*(1.0+erf(w));
                           Wpoly *= exp(w2-s*s);
                           TotalCoef = P->Area*Coef*cosphi*Wpoly/(MagPos*MagPos);

                           for(i=0;i<3;i++) FrcP[i] = TotalCoef*Phat[i];
                           MTxV(CPB,FrcP,FrcB);

                           /* Find plume force in B frame */
                           for(i=0;i<3;i++) r[i] = P->Centroid[i]-B->cm[i];
                           VxV(r,FrcB,TrqB);
                           MTxV(B->CN,FrcB,FrcN);
                           for(i=0;i<3;i++) {
                              B->Frc[i] += FrcN[i];
                              B->Trq[i] += TrqB[i];
                           }
                        }
                     }
                  }
               }
            }
         }
      }
}
/**********************************************************************/
/*  This function is called at the simulation rate.  Sub-sampling of  */
/*  actuators should be done on a case-by-case basis.                 */

void Actuators(struct SCType *S)
{

      long i,j;
      double FrcN[3];
      struct FSWType *FSW;
      struct JointType *G;
      struct FswGimType *FG;

      FSW = &S->FSW;

      /* Ideal Actuators */
      for(j=0;j<3;j++) {
         S->B[0].Frc[j] += FSW->IdealFrc[j];
         S->B[0].Trq[j] += FSW->IdealTrq[j];
      }
      /* Wheels */
      for(i=0;i<S->Nw;i++) {
         WhlModel(FSW->Twhlcmd[i],
                  S->Whl[i].H,
                  S->Whl[i].Hmax,
                  S->Whl[i].Tmax,
                 &S->Whl[i].Trq);
      }
      /* MTBs */
      for(i=0;i<S->Nmtb;i++){
         MTBModel(S->bvb,FSW->Mmtbcmd[i],S->MTB[i].Mmax,
                  S->MTB[i].A,&S->MTB[i].M,
                  S->MTB[i].Trq);
         for(j=0;j<3;j++){
            S->B[0].Trq[j] += S->MTB[i].Trq[j];
         }
      }

      /* Gimbal Drives */
      for(i=0;i<FSW->Ngim;i++) {
         G = &S->G[i];
         FG = &FSW->Gim[i];
         if (FG->IsUnderActiveControl) {
            /* PD Gimbal Control */
            GimbalModel(G->RotDOF,FG->Rate,FG->Ang,
                        FSW->GimCmd[i].Rate,FSW->GimCmd[i].Ang,
                        FG->RateGain,FG->AngGain,
                        FG->MaxRate,FG->MaxTrq,G->Trq);
            /* Ideal Kinematic Gimbal Control */
            for(j=0;j<G->RotDOF;j++) {
               G->RateCmd[j] = FSW->GimCmd[i].Rate[j];
               G->AngCmd[j] = FSW->GimCmd[i].Ang[j];
            }
         }
      }

      /* Thrusters */
      for(i=0;i<S->Nthr;i++) {
         ThrModel(FSW->Thrcmd[i],DTSIM,S->B[0].cm,&S->Thr[i]);
         MTxV(S->B[0].CN,S->Thr[i].Frc,FrcN);
         for(j=0;j<3;j++) {
            S->B[0].Trq[j] += S->Thr[i].Trq[j];
            S->B[0].Frc[j] += FrcN[j];
         }
      }
      if (ThrusterPlumesActive) {
         ThrusterPlumeFrcTrq(S);
      }

}

/* #ifdef __cplusplus
** }
** #endif
*/


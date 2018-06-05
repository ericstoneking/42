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
/*  Acceleration of a point A fixed in SC[Isc].B[0], expressed in     */
/*  B[0].  Due to 42's accounting of forces (esp. gravity), the       */
/*  gravity-gradient force accounting for the offset from             */
/*  SC[Isc].B[0].cm to A must be explicitly accounted for.  All other */
/*  gravity terms apply equally to A and B[0].  (Assuming gravity-    */
/*  gradient from non-spherical primary and 3rd-body forces is        */
/*  negligible.)  Surface forces are included in S->         */
void Accelerometer(struct SCType *S,struct AccelType *A)
{
      double p[3],wxp[3],wxwxp[3],axp[3],ab[3];
      double r,Coef,rhatn[3],rhat[3],rhatop;
      double accb[3],asnb[3];
      long j;
      struct BodyType *B;

      B = &S->B[0];

/* .. Vector from cm of B0 to A */
      for(j=0;j<3;j++) p[j] = A->PosB[j] - B->cm[j];

/* .. abn and alfbn are byproducts of NbodyAttitudeRK4 */
      VxV(B->wn,p,wxp);
      VxV(B->wn,wxp,wxwxp);
      VxV(S->alfbn,p,axp);
      MxV(B->CN,S->abs,ab);

/* .. Acceleration of a point fixed in an accelerating, rotating body */
      for(j=0;j<3;j++) accb[j] = ab[j]+axp[j]+wxwxp[j];

/* .. Grav-grad force (see Hughes, p.246, eq (56)) */
      if (GGActive) {
         r = MAGV(S->PosN);
         Coef = -3.0*Orb[S->RefOrb].mu/(r*r*r);
         CopyUnitV(S->PosN,rhatn);
         MxV(B->CN,rhatn,rhat);
         rhatop = VoV(rhat,p);
         for(j=0;j<3;j++) {
            accb[j] += Coef*(p[j]-3.0*rhat[j]*rhatop);
         }
      }

/* .. Add acceleration of SC cm from external surface forces */
      MxV(B->CN,S->asn,asnb);
      for(j=0;j<3;j++) accb[j] += asnb[j];

/* .. Find component along Accel axis */
      A->acc = VoV(accb,A->Axis);

/* .. Add noise, etc. */

}
/**********************************************************************/
void GenericGyroModel(struct SCType *S)
{
      struct GyroType *G;
      long Ig;
      double PrevBias,RateError,PrevAngle;
      long Counts,PrevCounts;
      
      for(Ig=0;Ig<S->Ngyro;Ig++) {
         G = &S->Gyro[Ig];
         
         G->SampleCounter++;
         if (G->SampleCounter >= G->MaxCounter) {
            G->SampleCounter = 0;
         
            G->TrueRate = VoV(S->B[0].wn,G->Axis);
         
            PrevBias = G->CorrCoef*G->Bias;
            G->Bias = PrevBias + G->BiasStabCoef*GaussianRandom(RNG);
            RateError = 0.5*(G->Bias+PrevBias) + G->ARWCoef*GaussianRandom(RNG);
         
            G->MeasRate = Limit(G->Scale*G->TrueRate + RateError,
               -G->MaxRate,G->MaxRate);
         
            PrevAngle = G->Angle;
            G->Angle = PrevAngle + G->MeasRate*G->SampleTime 
               + G->AngNoiseCoef*GaussianRandom(RNG);
         
            PrevCounts = (long) (PrevAngle/G->Quant+0.5);
            Counts = (long) (G->Angle/G->Quant+0.5);

            G->MeasRate = ((double) (Counts - PrevCounts))*G->Quant/G->SampleTime;
         }
      }
}
/**********************************************************************/
void GenericMagnetometerModel(struct SCType *S)
{
      struct MagnetometerType *MAG;
      long Counts,Imag;
      double Signal;
      
      for(Imag=0;Imag<S->Nmag;Imag++) {
         MAG = &S->MAG[Imag];
         
         MAG->SampleCounter++;
         if (MAG->SampleCounter >= MAG->MaxCounter) {
            MAG->SampleCounter = 0;
            
            Signal = MAG->Scale*VoV(S->bvb,MAG->Axis) 
               + MAG->Noise*GaussianRandom(RNG);
            Signal = Limit(Signal,-MAG->Saturation,MAG->Saturation); 
            Counts = (long) (Signal/MAG->Quant+0.5);
            MAG->Field = ((double) Counts)*MAG->Quant;
         }
      }
}
/**********************************************************************/
void GenericCssModel(struct SCType *S)
{
}
/**********************************************************************/
void GenericStarTrackerModel(struct SCType *S)
{
}
/**********************************************************************/
void GenericGpsModel(struct SCType *S)
{
}
/**********************************************************************/
void GenericFssModel(struct SCType *S)
{
}
/**********************************************************************/
void GenericAccelModel(struct SCType *S)
{
}
/**********************************************************************/
void GenericGimbalPotModel(struct SCType *S)
{
}
/**********************************************************************/
void GenericEarthSensorModel(struct SCType *S)
{
}
/**********************************************************************/
void GenericWheelTachModel(struct SCType *S)
{
}
/**********************************************************************/
/*  This function is called at the simulation rate.  Sub-sampling of  */
/*  sensors should be done on a case-by-case basis.                   */

void Sensors(struct SCType *S)
{

      double evn[3],evb[3];
      long i,j,k,DOF;
      struct AcsType *AC;
      struct JointType *G;

      AC = &S->AC;

      /* Sun Sensor */
      if (S->Eclipse){
         AC->SunValid = FALSE;
      }
      else {
         AC->SunValid = TRUE;
         MxV(S->B[0].CN,S->svn,AC->svb);
      }
      
      /* Magnetometer */
      if (Orb[S->RefOrb].World == EARTH) {
         AC->MagValid = TRUE;
         if (S->Nmag == 0) {
            for(i=0;i<3;i++) AC->bvb[i] = S->bvb[i];
         }
         else {
            GenericMagnetometerModel(S);
            for(i=0;i<3;i++) {
               AC->bvb[i] = 0.0;
            }
            for(i=0;i<S->Nmag;i++) {
               AC->MAG[i].Field = S->MAG[i].Field;
               for(j=0;j<3;j++) { 
                  AC->bvb[j] += S->MAG[i].Field*S->MAG[i].Axis[j];
               }
            }
         }
      }
      else {
         AC->MagValid = FALSE;
      }
      
      /* Star Tracker */
      for (i=0;i<4;i++) {
         AC->qbn[i] = S->B[0].qn[i];
      }

      /* IMU */
      if (S->Ngyro == 0) {
         for (i=0;i<3;i++) AC->wbn[i] = S->B[0].wn[i];
      }
      else {
         GenericGyroModel(S);
         for(i=0;i<3;i++) {
            AC->wbn[i] = 0.0;
         }
         for(i=0;i<S->Ngyro;i++) {
            AC->Gyro[i].Rate = S->Gyro[i].MeasRate;
            for(j=0;j<3;j++) {
               AC->wbn[j] += S->Gyro[i].MeasRate*S->Gyro[i].Axis[j];
            }
         }
      }

      /* Earth Sensor */
      for (i=0;i<3;i++) evn[i] = -S->PosN[i];
      UNITV(evn);
      MxV(S->B[0].CN,evn,evb);
      if (evb[2] > 0.866) {
         AC->ES.Valid = TRUE;
         AC->ES.Roll = evb[1];
         AC->ES.Pitch = -evb[0];
      }
      else {
         AC->ES.Valid = FALSE;
         AC->ES.Roll = 0.0;
         AC->ES.Pitch = 0.0;
      }

      /* Ephemeris */
      AC->EphValid = TRUE;
      for (i=0;i<3;i++) {
         AC->PosN[i] = S->PosN[i];
         AC->VelN[i] = S->VelN[i];
         AC->svn[i] = S->svn[i];
         AC->bvn[i] = S->bvn[i];
      }

      /* Gimbal Angles */
      for (i=0;i<AC->Ng;i++) {
         G = &S->G[i];
         DOF = AC->G[i].RotDOF;
         for (j=0;j<DOF;j++) {
            AC->G[i].Ang[j] = G->Ang[j];
            AC->G[i].AngRate[j] = G->AngRate[j];
         }
         for(j=0;j<3;j++) {
            for(k=0;k<3;k++) {
               AC->G[i].COI[j][k] = G->COI[j][k];
            }
         }
         DOF = AC->G[i].TrnDOF;
         for (j=0;j<DOF;j++) {
            AC->G[i].Pos[j] = G->Pos[j];
            AC->G[i].PosRate[j] = G->PosRate[j];
         }
      }

      /* Wheel Tachs */
      for (i=0;i<S->Nw;i++) {
         AC->Whl[i].H = S->Whl[i].H;
         AC->Whl[i].w = S->Whl[i].w;
      }

//      /* Formation Sensors */
//      for (i=0;i<3;i++) {
//         for (j=0;j<3;j++) FSW->CSF[i][j] = S->CF[i][j];
//         FSW->PosF[i] = S->PosF[i];
//         FSW->VelF[i] = S->VelF[i];
//      }

}

/* #ifdef __cplusplus
** }
** #endif
*/

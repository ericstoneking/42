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
      for(j=0;j<3;j++) p[j] = A->pb[j] - B->cm[j];

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

/* .. Express in Accel frame */
      MxV(A->CB,accb,A->acc);

/* .. Add noise, etc. */

}
/**********************************************************************/
/*  This function is called at the simulation rate.  Sub-sampling of  */
/*  sensors should be done on a case-by-case basis.                   */

void Sensors(struct SCType *S)
{

      double evn[3],evb[3];
      long i,j,k,DOF;
      struct FSWType *FSW;
      struct JointType *G;

      FSW = &S->FSW;

      /* Enable telemetry on OutFlag */
      if (OutFlag) FSW->Tlm = TRUE;
      else FSW->Tlm = FALSE;

      /* Sun Sensor */
      if (S->Eclipse){
         FSW->SunValid = FALSE;
      }
      else {
         FSW->SunValid = TRUE;
         MxV(S->B[0].CN,S->svn,FSW->svb);
      }
      /* TAM */
      if (Orb[S->RefOrb].World == EARTH) {
         FSW->MagValid = TRUE;
         for(i=0;i<3;i++) FSW->bvb[i] = S->bvb[i];
      }
      else {
         FSW->MagValid = FALSE;
      }
      /* Star Tracker */
      for (i=0;i<4;i++) FSW->qbn[i] = S->B[0].qn[i];

      /* IMU */
      for (i=0;i<3;i++) FSW->wbn[i] = S->B[0].wn[i];

      /* Earth Sensor */
      for (i=0;i<3;i++) evn[i] = -S->PosN[i];
      UNITV(evn);
      MxV(S->B[0].CN,evn,evb);
      if (evb[2] > 0.866) {
         FSW->EarthValid = TRUE;
         FSW->ESroll = evb[1];
         FSW->ESpitch = -evb[0];
      }
      else {
         FSW->EarthValid = FALSE;
         FSW->ESroll = 0.0;
         FSW->ESpitch = 0.0;
      }

      /* Ephemeris */
      FSW->EphValid = TRUE;
      for (i=0;i<3;i++) {
         FSW->PosN[i] = S->PosN[i];
         FSW->VelN[i] = S->VelN[i];
         FSW->svn[i] = S->svn[i];
         FSW->bvn[i] = S->bvn[i];
      }

      /* Gimbal Angles */
      for (i=0;i<FSW->Ngim;i++) {
         G = &S->G[i];
         DOF = FSW->Gim[i].RotDOF;
         for (j=0;j<DOF;j++) {
            FSW->Gim[i].Ang[j] = G->ang[j];
            FSW->Gim[i].Rate[j] = G->rate[j];
         }
         for(j=0;j<3;j++) {
            for(k=0;k<3;k++) {
               FSW->Gim[i].COI[j][k] = G->COI[j][k];
            }
         }
      }

      /* Wheel Tachs */
      for (i=0;i<S->Nw;i++) FSW->Hw[i] = S->Whl[i].H;

      /* Formation Sensors */
      for (i=0;i<3;i++) {
         for (j=0;j<3;j++) FSW->CSF[i][j] = S->CF[i][j];
         FSW->PosF[i] = S->PosF[i];
         FSW->VelF[i] = S->VelF[i];
      }

}

/* #ifdef __cplusplus
** }
** #endif
*/

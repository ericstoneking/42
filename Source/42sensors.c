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
/*  Substantial contributions to this model provided                  */
/*  by Jeffrey Calixto, 2019 summer intern.                           */
/*                                                                    */
/*  Acceleration of a point A fixed in SC[Isc].B[0], expressed in     */
/*  B[0].  Due to 42's accounting of forces (esp. gravity), the       */
/*  gravity-gradient force accounting for the offset from             */
/*  SC[Isc].B[0].cm to A must be explicitly accounted for.  All other */
/*  gravity terms apply equally to A and B[0].  (Assuming gravity-    */
/*  gradient from non-spherical primary and 3rd-body forces is        */
/*  negligible.)  Surface forces are included in S->         */
void AccelerometerModel(struct SCType *S)
{
      struct AccelType *A;
      double p[3],wxp[3],wxwxp[3],axp[3],ab[3];
      double r,Coef,rhatn[3],rhat[3],rhatop;
      double accb[3],asnb[3];
      long j;
      struct BodyType *B; 
      struct FlexNodeType *FN;
      long Ia;
      double PrevBias,PrevDV,AccError;
      long Counts,PrevCounts;

      B = &S->B[0];

     for(Ia=0;Ia<S->Nacc;Ia++) { 
         A = &S->Accel[Ia];
         A->SampleCounter++; 
         if (A->SampleCounter >= A->MaxCounter) {
            A->SampleCounter = 0;

            /* Vector from cm of B0 to A */
            for(j=0;j<3;j++) 
               p[j] = A->PosB[j] - B->cm[j];

            /* abs and alfbn are byproducts of NbodyAttitudeRK4 */
            VxV(B->wn,p,wxp);
            VxV(B->wn,wxp,wxwxp);
            VxV(S->alfbn,p,axp);
            MxV(B->CN,S->abs,ab);

            /* Acceleration of a point fixed in an accelerating, rotating body */
            for(j=0;j<3;j++) accb[j] = ab[j]+axp[j]+wxwxp[j];

            /* Grav-grad force (see Hughes, p.246, eq (56)) */
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

            /* Add acceleration of SC cm from external surface forces */
            MxV(B->CN,S->asn,asnb);
            for(j=0;j<3;j++) accb[j] += asnb[j];


            /* .. Add noise, etc. */
            /* this is the noise added for the accelerometer ( the Edited ) */

             
            if (S->FlexActive) {
               FN = &S->B[0].FlexNode[A->FlexNode];  
               A->TrueAcc = VoV(FN->TotTrnVel,A->Axis); /* TODO: Fix this */
            }
            else {
               A->TrueAcc = VoV(accb,A->Axis); 
            }                                        
            
            PrevBias = A->CorrCoef*A->Bias;
            A->Bias = PrevBias + A->BiasStabCoef*GaussianRandom(RNG);
            AccError = 0.5*(A->Bias+PrevBias) + A->DVRWCoef*GaussianRandom(RNG);
         
            A->MeasAcc = Limit(A->Scale*A->TrueAcc + AccError,
               -A->MaxAcc,A->MaxAcc); 
         
            PrevDV = A->DV; 
            A->DV = PrevDV + A->MeasAcc*A->SampleTime 
               + A->DVNoiseCoef*GaussianRandom(RNG);
         
            PrevCounts = (long) (PrevDV/A->Quant+0.5);
            Counts = (long) (A->DV/A->Quant+0.5);

            A->MeasAcc = ((double) (Counts - PrevCounts))*A->Quant/A->SampleTime;
            
            S->AC.Accel[Ia].Acc = A->MeasAcc;
         }
      }
}
/**********************************************************************/
void GyroModel(struct SCType *S)
{
      struct GyroType *G;
      struct FlexNodeType *FN;
      long Ig;
      double PrevBias,RateError,PrevAngle;
      long Counts,PrevCounts;
      
      for(Ig=0;Ig<S->Ngyro;Ig++) {
         G = &S->Gyro[Ig];
         
         G->SampleCounter++;
         if (G->SampleCounter >= G->MaxCounter) {
            G->SampleCounter = 0;
            
            if (S->FlexActive) {
               FN = &S->B[0].FlexNode[G->FlexNode];
               G->TrueRate = VoV(FN->TotAngVel,G->Axis);
            }
            else {
               G->TrueRate = VoV(S->B[0].wn,G->Axis);
            }
            
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
            
            S->AC.Gyro[Ig].Rate = G->MeasRate;
         }
      }
}
/**********************************************************************/
void MagnetometerModel(struct SCType *S)
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
            
            S->AC.MAG[Imag].Field = MAG->Field;
         }
      }
}
/**********************************************************************/
/* This model credit Paul McKee, summer intern 2018                   */
void CssModel(struct SCType *S)
{
      struct CssType *CSS;
      long Counts, Icss; 
      double Signal; 
      double SoA; 
      double svb[3];
         
      for(Icss=0;Icss<S->Ncss;Icss++) {
         CSS = &S->CSS[Icss];
         
         CSS->SampleCounter++;
         if (CSS->SampleCounter >= CSS->MaxCounter) {
            CSS->SampleCounter = 0;
        
            if (S->Eclipse) {
               CSS->Valid = FALSE; 
               CSS->Illum = 0.0; 
            }
            else {
               MxV(S->B[CSS->Body].CN,S->svn,svb);
               SoA = VoV(svb,CSS->Axis);
               if (SoA > CSS->CosFov) {
                  /* Sun within FOV */
                  CSS->Valid = TRUE;
                  Signal = CSS->Scale*SoA;
                  Counts = (long) (Signal/CSS->Quant+0.5);
                  CSS->Illum = ((double) Counts)*CSS->Quant;
               }        
               else {
                  /* Sun not in FOV */
                  CSS->Valid = FALSE;
                  CSS->Illum = 0.0;   
               }
            }

            #ifdef _ENABLE_GUI_  
            CSS->Albedo = 0.0;          
            if (AlbedoActive) {
               FindAlbedo(S,CSS);
               Signal = CSS->Scale*CSS->Albedo;
               Counts = (long) (Signal/CSS->Quant+0.5);
               CSS->Illum += ((double) Counts)*CSS->Quant;
               CSS->Illum = Limit(CSS->Illum,0.0,CSS->Scale);
            }
            #endif
         
            /* Copy into AC structure */
            S->AC.CSS[Icss].Valid = CSS->Valid;       
            S->AC.CSS[Icss].Illum = CSS->Illum;
         }       
      }
}
/**********************************************************************/
void FssModel(struct SCType *S)
{
      struct FssType *FSS;
      static struct RandomProcessType *FssNoise;
      double svs[3],SunAng[2],Signal;
      long Counts;
      static long First = 1;
      long Ifss,i;
      
      if (First) {
         First = 0;
         FssNoise = CreateRandomProcess(10);
      }
      
      for(Ifss=0;Ifss<S->Nfss;Ifss++) {
         FSS = &S->FSS[Ifss];
         
         FSS->SampleCounter++;
         if (FSS->SampleCounter >= FSS->MaxCounter) {
            FSS->SampleCounter = 0;
         
            if (S->Eclipse) {
               FSS->Valid = FALSE;
            }
            else {
               MxV(FSS->CB,S->svb,svs);
               SunAng[0] = atan2(svs[FSS->H_Axis],svs[FSS->BoreAxis]);
               SunAng[1] = atan2(svs[FSS->V_Axis],svs[FSS->BoreAxis]);
               if (fabs(SunAng[0]) < FSS->FovHalfAng[0] && 
                   fabs(SunAng[1]) < FSS->FovHalfAng[1] &&
                   svs[FSS->BoreAxis] > 0.0) {
                  FSS->Valid = TRUE;
               }
               else {
                  FSS->Valid = FALSE;
               }
            }
            
            if (FSS->Valid) {
               for(i=0;i<2;i++) {
                  Signal = SunAng[i] + FSS->NEA*GaussianRandom(FssNoise);
                  Counts = (long) (Signal/FSS->Quant+0.5);
                  FSS->SunAng[i] = ((double) Counts)*FSS->Quant;
               }
            }
            else {
               FSS->SunAng[0] = 0.0;
               FSS->SunAng[1] = 0.0;
            }
            
            S->AC.FSS[Ifss].Valid = FSS->Valid;
            for(i=0;i<2;i++) S->AC.FSS[Ifss].SunAng[i] = FSS->SunAng[i];
         }
      }
}
/**********************************************************************/
void StarTrackerModel(struct SCType *S)
{
      struct StarTrackerType *ST;
      struct FlexNodeType *FN;
      static struct RandomProcessType *StNoise;
      struct WorldType *W;
      double qsn[4],Qnoise[4];
      double BoS,OrbRad,LimbAng,NadirVecB[3],BoN;
      double mvn[3],MoonDist,mvb[3],BoM;
      double qfb[4],qfn[4];
      static long First = 1;
      long Ist,i;
      
      if (First) {
         First = 0;
         StNoise = CreateRandomProcess(1);
      }
      
      for(Ist=0;Ist<S->Nst;Ist++) {
         ST = &S->ST[Ist];
         
         ST->SampleCounter++;
         if (ST->SampleCounter >= ST->MaxCounter) {
            ST->SampleCounter = 0;
         
            ST->Valid = TRUE;
            /* Sun Occultation? */
            BoS = VoV(ST->CB[ST->BoreAxis],S->svb);
            if (BoS > ST->CosSunExclAng) ST->Valid = FALSE;
            /* Earth Occultation? (Generalized to whatever world we're orbiting) */
            W = &World[Orb[S->RefOrb].World];
            OrbRad = MAGV(S->PosN);
            LimbAng = asin(W->rad/OrbRad);
            MxV(S->B[0].CN,S->CLN[2],NadirVecB);
            BoN = VoV(ST->CB[ST->BoreAxis],NadirVecB);
            if (BoN > cos(LimbAng + ST->EarthExclAng)) ST->Valid = FALSE;
            /* Moon Occultation? (Only worked out if orbiting Earth.  Customize as needed)*/
            if (Orb[S->RefOrb].World == EARTH) {
               for(i=0;i<3;i++) mvn[i] = World[LUNA].eph.PosN[i] - S->PosN[i];
               MoonDist = UNITV(mvn);
               LimbAng = asin(World[LUNA].rad/MoonDist);
               MxV(S->B[0].CN,mvn,mvb);
               BoM = VoV(ST->CB[ST->BoreAxis],mvb);
               if (BoM > cos(LimbAng+ST->MoonExclAng)) ST->Valid = FALSE;
            }
            if (ST->Valid) {
               if (S->FlexActive) {
                  FN = &S->B[0].FlexNode[ST->FlexNode];
                  for(i=0;i<3;i++) qfb[i] = 0.5*FN->ang[i];
                  qfb[3] = sqrt(1.0-qfb[0]*qfb[0]-qfb[1]*qfb[1]-qfb[2]*qfb[2]);
                  QxQ(qfb,S->B[0].qn,qfn);
                  QxQ(ST->qb,qfn,qsn);
               }
               else {
                  QxQ(ST->qb,S->B[0].qn,qsn);
               }
               /* Add Noise in ST frame */
               for(i=0;i<3;i++) Qnoise[i] = 0.5*ST->NEA[i]*GaussianRandom(StNoise);
               Qnoise[3] = 1.0;
               UNITQ(Qnoise);
               QxQ(Qnoise,qsn,ST->qn);
            }
            
            S->AC.ST[Ist].Valid = ST->Valid;
            for(i=0;i<4;i++) {
               S->AC.ST[Ist].qn[i] = ST->qn[i];
            }
         }
      }
}
/**********************************************************************/
void GpsModel(struct SCType *S)
{
      struct GpsType *GPS;
      static struct RandomProcessType *GpsNoise;
      double PosW[3],MagPosW;
      long Ig,i;
      static long First = 1;
      
      if (First) {
         First = 0;
         GpsNoise = CreateRandomProcess(2);
      }
      
      if (Orb[S->RefOrb].World == EARTH) {
         for(Ig=0;Ig<S->Ngps;Ig++) {
            GPS = &S->GPS[Ig];
            
            GPS->SampleCounter++;
            if (GPS->SampleCounter >= GPS->MaxCounter) {
               GPS->SampleCounter = 0;
               
               GPS->Valid = TRUE;
         
               GPS->Rollover = GpsRollover;
               GPS->Week = GpsWeek;
               GPS->Sec = GpsSecond + GPS->TimeNoise*GaussianRandom(GpsNoise);
      
               for(i=0;i<3;i++) {
                  GPS->PosN[i] = S->PosN[i] + GPS->PosNoise*GaussianRandom(GpsNoise);
                  GPS->VelN[i] = S->VelN[i] + GPS->VelNoise*GaussianRandom(GpsNoise);
               }
               MxV(World[EARTH].CWN,S->PosN,PosW);
               MxV(World[EARTH].CWN,GPS->PosN,GPS->PosW);
               MxV(World[EARTH].CWN,GPS->VelN,GPS->VelW);
               /* Subtract Earth rotation velocity */
               GPS->VelW[0] -= -World[EARTH].w*PosW[1];
               GPS->VelW[1] -=  World[EARTH].w*PosW[0];
         
               MagPosW = MAGV(GPS->PosW);
               GPS->Lng = atan2(GPS->PosW[1],GPS->PosW[0]);
               GPS->Lat = asin(GPS->PosW[2]/MagPosW);
               GPS->Alt = MagPosW - World[EARTH].rad;
               ECEFToWGS84(GPS->PosW,&GPS->WgsLat,&GPS->WgsLng,&GPS->WgsAlt);
               
               S->AC.GPS[Ig].Valid = GPS->Valid;
               S->AC.GPS[Ig].Rollover = GPS->Rollover;
               S->AC.GPS[Ig].Week = GPS->Week;
               S->AC.GPS[Ig].Sec = GPS->Sec;
               
               for(i=0;i<3;i++) {
                  S->AC.GPS[Ig].PosN[i] = GPS->PosN[i];
                  S->AC.GPS[Ig].VelN[i] = GPS->VelN[i];
                  S->AC.GPS[Ig].PosW[i] = GPS->PosW[i];
                  S->AC.GPS[Ig].VelW[i] = GPS->VelW[i];
               }
               S->AC.GPS[Ig].Lng = GPS->Lng;
               S->AC.GPS[Ig].Lat = GPS->Lat;
               S->AC.GPS[Ig].Alt = GPS->Alt;
               S->AC.GPS[Ig].WgsLng = GPS->WgsLng;
               S->AC.GPS[Ig].WgsLat = GPS->WgsLat;
               S->AC.GPS[Ig].WgsAlt = GPS->WgsAlt;
               
            }
         }
      }
      else {
         for(Ig=0;Ig<S->Ngps;Ig++) S->GPS[Ig].Valid = FALSE;
      }
}
/**********************************************************************/
/*  This function is called at the simulation rate.  Sub-sampling of  */
/*  sensors should be done on a case-by-case basis.                   */
void Sensors(struct SCType *S)
{

      double evn[3],evb[3],qfb[4];
      long i,j,k,DOF;
      struct AcType *AC;
      struct JointType *G;
      struct FlexNodeType *FN;

      AC = &S->AC;
      
      /* Ephemeris */
      AC->EphValid = 1;
      for(i=0;i<3;i++) {
         AC->svn[i] = S->svn[i];
         AC->bvn[i] = S->bvn[i];
      }

      /* Accelerometer */
      if (S->Nacc > 0) {
         AccelerometerModel(S);
      }

      /* Gyro */
      if (S->Ngyro == 0) {
         for (i=0;i<3;i++) AC->wbn[i] = S->B[0].wn[i];
      }
      else {
         GyroModel(S);
      }
      
      /* Magnetometer */
      if (Orb[S->RefOrb].World == EARTH) {
         AC->MagValid = TRUE;
         if (S->Nmag == 0) {
            for(i=0;i<3;i++) AC->bvb[i] = S->bvb[i];
         }
         else {
            MagnetometerModel(S);
         }
      }
      else {
         AC->MagValid = FALSE;
      }
      
      /* Sun Sensors */
      if (S->Ncss == 0 && S->Nfss == 0) {
         if (S->Eclipse){
            AC->SunValid = FALSE;
         }
         else {
            AC->SunValid = TRUE;
            MxV(S->B[0].CN,S->svn,AC->svb);
         }
      }
      if (S->Ncss > 0) {
         CssModel(S);
      }
      if (S->Nfss > 0) {
         FssModel(S);
      }
      
      /* Star Tracker */
      if (S->Nst == 0) {
         if (S->FlexActive) {
            FN = &S->B[0].FlexNode[0]; /* Arbitrarily use FlexNode[0] */
            for(i=0;i<3;i++) qfb[i] = 0.5*FN->ang[i];
            qfb[3] = sqrt(1.0-qfb[0]*qfb[0]-qfb[1]*qfb[1]-qfb[2]*qfb[2]);
            QxQ(qfb,S->B[0].qn,AC->qbn);
         }
         else {
            for (i=0;i<4;i++) AC->qbn[i] = S->B[0].qn[i];
         }
      }
      else {
         StarTrackerModel(S);
      }

      /* GPS Receiver (or ephem model) */
      if (S->Ngps == 0) {
         AC->Time = DynTime;
         for(i=0;i<3;i++) {
            AC->PosN[i] = S->PosN[i];
            AC->VelN[i] = S->VelN[i];
         }
      }
      else {
         GpsModel(S);
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

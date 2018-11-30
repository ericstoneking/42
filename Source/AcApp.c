/*    This file is distributed with 42,                               */
/*    the (mostly harmless) spacecraft dynamics simulation            */
/*    created by Eric Stoneking of NASA Goddard Space Flight Center   */

/*    Copyright 2010 United States Government                         */
/*    as represented by the Administrator                             */
/*    of the National Aeronautics and Space Administration.           */

/*    No copyright is claimed in the United States                    */
/*    under Title 17, U.S. Code.                                      */

/*    All Other Rights Reserved.                                      */

#ifdef _AC_STANDALONE_
   #include "42fsw.h" 
#else
   #include "42.h"
#endif

/* #ifdef __cplusplus
** namespace _42 {
** using namespace Kit;
** #endif
*/
#ifdef _AC_STANDALONE_

#if 1
/**********************************************************************/
/* This function copies needed parameters from the SC structure to    */
/* the AC structure.  This is a crude first pass.  It only allocates  */
/* memory for the structures, and counts on the data to be filled in  */
/* via messages.                                                      */
void InitAC(struct AcType *AC)
{
      long Ig;

      AC->Init = 1;

      /* Joints */
      AC->Ng = 1;
      if (AC->Ng > 0) {
         AC->G = (struct AcJointType *) calloc(AC->Ng,sizeof(struct AcJointType));
         for(Ig=0;Ig<AC->Ng;Ig++) {
            AC->G[Ig].IsUnderActiveControl = 1;
         }
      }
      
      /* Wheels */
      AC->Nwhl = 4;
      if (AC->Nwhl > 0) {
         AC->Whl = (struct AcWhlType *) calloc(AC->Nwhl,sizeof(struct AcWhlType));
      }

      /* Magnetic Torquer Bars */
      AC->Nmtb = 3;
      if (AC->Nmtb > 0) {
         AC->MTB = (struct AcMtbType *) calloc(AC->Nmtb,sizeof(struct AcMtbType));
      }

      /* Thrusters */
      AC->Nthr = 0;
      if (AC->Nthr > 0) {
         AC->Thr = (struct AcThrType *) calloc(AC->Nthr,sizeof(struct AcThrType));
      }
      
      /* Control Moment Gyros */

      /* Gyro Axes */
      AC->Ngyro = 3;
      if (AC->Ngyro > 0) {
         AC->Gyro = (struct AcGyroType *) calloc(AC->Ngyro,sizeof(struct AcGyroType));
      }

      /* Magnetometer Axes */
      AC->Nmag = 3;
      if (AC->Nmag > 0) {
         AC->MAG = (struct AcMagnetometerType *) calloc(AC->Nmag,sizeof(struct AcMagnetometerType));
      }

      /* Coarse Sun Sensors */
      AC->Ncss = 8;
      if (AC->Ncss > 0) {
         AC->CSS = (struct AcCssType *) calloc(AC->Ncss,sizeof(struct AcCssType));
      }
      
      /* Fine Sun Sensors */
      AC->Nfss = 1;
      if (AC->Nfss > 0) {
         AC->FSS = (struct AcFssType *) calloc(AC->Nfss,sizeof(struct AcFssType));
      }

      /* Star Trackers */
      AC->Nst = 1;
      if (AC->Nst > 0) {
         AC->ST = (struct AcStarTrackerType *) calloc(AC->Nst,sizeof(struct AcStarTrackerType));
      }

      /* GPS */
      AC->Ngps = 1;
      if (AC->Ngps > 0) {
         AC->GPS = (struct AcGpsType *) calloc(AC->Ngps,sizeof(struct AcGpsType)); 
      }     
      
      /* Accelerometer Axes */

      
      /* Controllers */
      AC->CfsCtrl.Init = 1;      
}
#else
/**********************************************************************/
/* This function copies needed parameters from the SC structure to    */
/* the AC structure.                                                  */
/* This version is copy/pasted from 42fsw.c.  Obviously won't work    */
/* as is.  Needs work.                                                */
void InitAC(struct AcType *AC)
{
      long Ig,i,j,k;
      double **A,**Aplus;

      AC->Init = 1;

      /* Joints */
      AC->Ng = S->Ng;
      if (S->Ng > 0) {
         AC->G = (struct AcJointType *) calloc(S->Ng,sizeof(struct AcJointType));
         for(Ig=0;Ig<AC->Ng;Ig++) {
            AC->G[Ig].IsUnderActiveControl = TRUE;
            AC->G[Ig].IsSpherical = S->G[Ig].IsSpherical;
            AC->G[Ig].RotDOF = S->G[Ig].RotDOF;
            AC->G[Ig].TrnDOF = S->G[Ig].TrnDOF;
            for(i=0;i<3;i++) {
               for(j=0;j<3;j++) {
                  AC->G[Ig].CGiBi[i][j] = S->G[Ig].CGiBi[i][j];
                  AC->G[Ig].CBoGo[i][j] = S->G[Ig].CBoGo[i][j];
               }
            }
            AC->G[Ig].RotSeq = S->G[Ig].RotSeq;
            AC->G[Ig].TrnSeq = S->G[Ig].TrnSeq;
         }
      }
      
      /* Gyro Axes */
      AC->Ngyro = S->Ngyro;
      if (S->Ngyro > 0) {
         AC->Gyro = (struct AcGyroType *) calloc(S->Ngyro,sizeof(struct AcGyroType));
         for(i=0;i<S->Ngyro;i++) {
            for(j=0;j<3;j++) {
               AC->Gyro[i].Axis[j] = S->Gyro[i].Axis[j];
            }
         }
      }

      /* Magnetometer Axes */
      AC->Nmag = S->Nmag;
      if (S->Nmag > 0) {
         AC->MAG = (struct AcMagnetometerType *) calloc(S->Nmag,sizeof(struct AcMagnetometerType));
         for(i=0;i<S->Nmag;i++) {
            for(j=0;j<3;j++) {
               AC->MAG[i].Axis[j] = S->MAG[i].Axis[j];
            }
         }
      }

      /* Coarse Sun Sensors */
      AC->Ncss = S->Ncss;
      AC->CSS = (struct AcCssType *) calloc(S->Ncss,sizeof(struct AcCssType));
      for(i=0;i<S->Ncss;i++) {
         for(j=0;j<3;j++) AC->CSS[i].Axis[j] = S->CSS[i].Axis[j];
         AC->CSS[i].Scale = S->CSS[i].Scale;
      }
      
      /* Fine Sun Sensors */
      AC->Nfss = S->Nfss;
      AC->FSS = (struct AcFssType *) calloc(S->Nfss,sizeof(struct AcFssType));
      for(k=0;k<S->Nfss;k++) {
         for(i=0;i<3;i++) {
            for(j=0;j<3;j++) AC->FSS[k].CB[i][j] = S->FSS[k].CB[i][j];
         }
         for(i=0;i<4;i++) AC->FSS[k].qb[i] = S->FSS[k].qb[i];
      }

      /* Star Trackers */
      AC->Nst = S->Nst;
      AC->ST = (struct AcStarTrackerType *) calloc(S->Nst,sizeof(struct AcStarTrackerType));
      for(k=0;k<S->Nst;k++) {
         for(i=0;i<3;i++) {
            for(j=0;j<3;j++) AC->ST[k].CB[i][j] = S->ST[k].CB[i][j];
         }
         for(i=0;i<4;i++) AC->ST[k].qb[i] = S->ST[k].qb[i];
      }

      /* GPS */
      AC->Ngps = S->Ngps;
      AC->GPS = (struct AcGpsType *) calloc(S->Ngps,sizeof(struct AcGpsType));      
      
      /* Accelerometer Axes */

      /* Wheels */
      AC->Nwhl = S->Nw;
      if (S->Nw > 0) {
         AC->Whl = (struct AcWhlType *) calloc(AC->Nwhl,sizeof(struct AcWhlType));
         A = CreateMatrix(3,AC->Nwhl);
         Aplus = CreateMatrix(AC->Nwhl,3);
         for (i=0;i<S->Nw;i++) {
            for (j=0;j<3;j++) {
               AC->Whl[i].Axis[j] = S->Whl[i].A[j];
               A[j][i] = S->Whl[i].A[j];
            }
         }
         if (S->Nw == 1) {
            for(i=0;i<3;i++) AC->Whl[0].DistVec[i] = AC->Whl[0].Axis[i]; 
         }
         else if (S->Nw >= 2) {
            PINVG(A,Aplus,3,S->Nw);
            for(i=0;i<AC->Nwhl;i++) {
               for(j=0;j<3;j++) {
                  AC->Whl[i].DistVec[j] = Aplus[i][j];
               }
            }
         }
         DestroyMatrix(A,3);
         DestroyMatrix(Aplus,AC->Nwhl);
         for(i=0;i<S->Nw;i++) {
            AC->Whl[i].J = S->Whl[i].J;
            AC->Whl[i].Tmax = S->Whl[i].Tmax;
            AC->Whl[i].Hmax = S->Whl[i].Hmax;
         }
      }

      /* Magnetic Torquer Bars */
      AC->Nmtb = S->Nmtb;
      if (S->Nmtb > 0) {
         AC->MTB = (struct AcMtbType *) calloc(AC->Nmtb,sizeof(struct AcMtbType));
         A = CreateMatrix(3,AC->Nmtb);
         Aplus = CreateMatrix(AC->Nmtb,3);
         for (i=0;i<S->Nmtb;i++) {
            for (j=0;j<3;j++) {
               AC->MTB[i].Axis[j] = S->MTB[i].A[j];
               A[j][i] = S->MTB[i].A[j];
            }
         }
         if (S->Nmtb == 1) {
            for(i=0;i<3;i++) AC->MTB[0].DistVec[i] = AC->MTB[0].Axis[i]; 
         }
         else if (S->Nmtb >= 2) {
            PINVG(A,Aplus,3,S->Nmtb);
            for(i=0;i<AC->Nmtb;i++) {
               for(j=0;j<3;j++) {
                  AC->MTB[i].DistVec[j] = Aplus[i][j];
               }
            }
         }
         DestroyMatrix(A,3);
         DestroyMatrix(Aplus,AC->Nmtb);
         for(i=0;i<S->Nmtb;i++) {
            AC->MTB[i].Mmax = S->MTB[i].Mmax;
         }
      }

      /* Thrusters */
      AC->Nthr = S->Nthr;
      if (S->Nthr > 0) {
         AC->Thr = (struct AcThrType *) calloc(AC->Nthr,sizeof(struct AcThrType));
         for(i=0;i<S->Nthr;i++) {
            AC->Thr[i].Fmax = S->Thr[i].Fmax;
            for(j=0;j<3;j++) {
               AC->Thr[i].Axis[j] = S->Thr[i].A[j];
               AC->Thr[i].PosB[j] = S->Thr[i].PosB[j];
            }
         }
      }
      
      /* Control Moment Gyros */

      AC->DT = DTSIM;
      AC->mass = S->mass;
      for (i=0;i<3;i++) {
         for(j=0;j<3;j++) {
            AC->MOI[i][j] = S->I[i][j];
         }
      }
      
      /* Controllers */
      AC->PrototypeCtrl.Init = 1;
      AC->AdHocCtrl.Init = 1;
      AC->SpinnerCtrl.Init = 1;
      AC->MomBiasCtrl.Init = 1;
      AC->ThreeAxisCtrl.Init = 1;
      AC->IssCtrl.Init = 1;
      AC->CmgCtrl.Init = 1;
      AC->CfsCtrl.Init = 1;
      
      AC->PrototypeCtrl.wc = 0.05*TwoPi;
      AC->PrototypeCtrl.amax = 0.01;
      AC->PrototypeCtrl.vmax = 0.5*D2R;
}
#endif
#endif
/**********************************************************************/
/*  Some Simple Sensor Processing Functions                           */
/*  corresponding to the Sensor Models in 42sensors.c                 */
/*  Note!  These are simple, sometimes naive.  Use with care.         */
/**********************************************************************/
void GyroProcessing(struct AcType *AC)
{
      struct AcGyroType *G;
      double A0xA1[3];
      double A[3][3],b[3],Ai[3][3];
      double AtA[3][3] = {{0.0,0.0,0.0},{0.0,0.0,0.0},{0.0,0.0,0.0}};
      double Atb[3] = {0.0,0.0,0.0};
      double AtAi[3][3];
      long Ig,i,j;

      if (AC->Ngyro == 0) {
         /* AC->wbn populated by true S->B[0].wn in 42sensors.c */
      }
      else if (AC->Ngyro == 1) {
         G = &AC->Gyro[0];
         for(i=0;i<3;i++) AC->wbn[i] = G->Rate*G->Axis[i];
      }
      else if (AC->Ngyro == 2) {
         VxV(AC->Gyro[0].Axis,AC->Gyro[1].Axis,A0xA1);
         for(i=0;i<3;i++) {
            A[0][i] = AC->Gyro[0].Axis[i];
            A[1][i] = AC->Gyro[1].Axis[i];
            A[2][i] = A0xA1[i]; 
         }
         b[0] = AC->Gyro[0].Rate;
         b[1] = AC->Gyro[1].Rate;
         b[2] = 0.0;
         MINV(A,Ai);
         MxV(Ai,b,AC->wbn);
      }
      else if (AC->Ngyro > 2) {
         /* Normal Equations */
         for(Ig=0;Ig<AC->Ngyro;Ig++) {
            G = &AC->Gyro[Ig];
            for(i=0;i<3;i++) {
               Atb[i] += G->Rate*G->Axis[i];
               for(j=0;j<3;j++) {
                  AtA[i][j] += G->Axis[i]*G->Axis[j];
               }
            }
         }
         MINV(AtA,AtAi);
         MxV(AtAi,Atb,AC->wbn);
      }
}
/**********************************************************************/
void MagnetometerProcessing(struct AcType *AC)
{
      struct AcMagnetometerType *M;
      double A0xA1[3];
      double A[3][3],b[3],Ai[3][3];
      double AtA[3][3] = {{0.0,0.0,0.0},{0.0,0.0,0.0},{0.0,0.0,0.0}};
      double Atb[3] = {0.0,0.0,0.0};
      double AtAi[3][3];
      long Im,i,j;

      if (AC->Nmag == 0) {
         /* AC->bvb populated by true S->bvb in 42sensors.c */
      }
      else if (AC->Nmag == 1) {
         M = &AC->MAG[0];
         for(i=0;i<3;i++) AC->bvb[i] = M->Field*M->Axis[i];
      }
      else if (AC->Nmag == 2) {
         VxV(AC->MAG[0].Axis,AC->MAG[1].Axis,A0xA1);
         for(i=0;i<3;i++) {
            A[0][i] = AC->MAG[0].Axis[i];
            A[1][i] = AC->MAG[1].Axis[i];
            A[2][i] = A0xA1[i]; 
         }
         b[0] = AC->MAG[0].Field;
         b[1] = AC->MAG[1].Field;
         b[2] = 0.0;
         MINV(A,Ai);
         MxV(Ai,b,AC->bvb);
      }
      else if (AC->Nmag > 2) {
         /* Normal Equations */
         for(Im=0;Im<AC->Nmag;Im++) {
            M = &AC->MAG[Im];
            for(i=0;i<3;i++) {
               Atb[i] += M->Field*M->Axis[i];
               for(j=0;j<3;j++) {
                  AtA[i][j] += M->Axis[i]*M->Axis[j];
               }
            }
         }
         MINV(AtA,AtAi);
         MxV(AtAi,Atb,AC->bvb);
      }
}
/**********************************************************************/
void CssProcessing(struct AcType *AC)
{
      struct AcCssType *Css;
      double AtA[3][3] = {{0.0,0.0,0.0},{0.0,0.0,0.0},{0.0,0.0,0.0}};
      double Atb[3] = {0.0,0.0,0.0};
      double AtAi[3][3];
      double A[2][3],b[2];
      long Ic,i,j;
      long Nvalid = 0;
      double InvalidSVB[3] = {1.0,0.0,0.0}; /* Safe vector if SunValid == FALSE */

      if (AC->Ncss == 0) {
         /* AC->svb populated by true S->svb in 42sensors.c */
      }
      else {
         for(Ic=0;Ic<AC->Ncss;Ic++) {
            Css = &AC->CSS[Ic];
            if (Css->Valid) {
               Nvalid++;
               /* Normal equations, assuming Nvalid will end up > 2 */
               for(i=0;i<3;i++) {
                  Atb[i] += Css->Axis[i]*Css->Illum/Css->Scale;
                  
                  for(j=0;j<3;j++) {
                     AtA[i][j] += Css->Axis[i]*Css->Axis[j];
                  }
               }
               /* In case Nvalid ends up == 2 */
               for(i=0;i<3;i++) {
                  A[0][i] = A[1][i];
                  A[1][i] = Css->Axis[i];
               }
               b[0] = b[1];
               b[1] = Css->Illum/Css->Scale;
            }
         }
         if (Nvalid > 2) {
            AC->SunValid = TRUE;
            MINV(AtA,AtAi);
            MxV(AtAi,Atb,AC->svb);
            UNITV(AC->svb);
         }
         else if (Nvalid == 2) {
            AC->SunValid = TRUE;
            for(i=0;i<3;i++) AC->svb[i] = b[0]*A[0][i] + b[1]*A[1][i];
            UNITV(AC->svb);
         }
         else if (Nvalid == 1) {
            AC->SunValid = TRUE;
            for(i=0;i<3;i++) AC->svb[i] = Atb[i];
            UNITV(AC->svb);
         }
         else {
            AC->SunValid = FALSE;
            for(i=0;i<3;i++) AC->svb[i] = InvalidSVB[i];
         }
      }
}
/******************************************************************************/
/* This function assumes FSS FOVs don't overlap, and FSS overwrites CSS */
void FssProcessing(struct AcType *AC)
{
      struct AcFssType *FSS;
      long Ifss,i;
      
      for(Ifss=0;Ifss<AC->Nfss;Ifss++) {
         FSS = &AC->FSS[Ifss];
         if (FSS->Valid) {
            AC->SunValid = 1;
            FSS->SunVecS[0] = sin(FSS->SunAng[0]);
            FSS->SunVecS[1] = sin(FSS->SunAng[1]);
            FSS->SunVecS[2] = 
               sqrt(1.0 - FSS->SunVecS[0]*FSS->SunVecS[0]
                        - FSS->SunVecS[1]*FSS->SunVecS[1]);
            MTxV(FSS->CB,FSS->SunVecS,FSS->SunVecB);
            for(i=0;i<3;i++) AC->svb[i] = FSS->SunVecB[i];
         }
      }
}
/**********************************************************************/
/* TODO: Weight measurements to reduce impact of "weak" axis */
void StarTrackerProcessing(struct AcType *AC)
{
      long Ist,i;
      struct AcStarTrackerType *ST;
      long Nvalid = 0;
      double qbn[4];
      
      if (AC->Nst == 0) {
         /* AC->qbn populated by true S->B[0].qn in 42sensors.c */
         AC->StValid = TRUE;
      }
      else {
         /* Naive averaging */
         for(i=0;i<4;i++) AC->qbn[i] = 0.0;
         for(Ist=0;Ist<AC->Nst;Ist++) {
            ST = &AC->ST[Ist];
            if (ST->Valid) {
               Nvalid++;
               QTxQ(ST->qb,ST->qn,qbn);
               RECTIFYQ(qbn);
               for(i=0;i<4;i++) AC->qbn[i] += qbn[i];
            }
         }
         if (Nvalid > 0) {
            AC->StValid = TRUE;
            UNITQ(AC->qbn);
         }
         else {
            AC->StValid = FALSE;
            AC->qbn[3] = 1.0;
         }
      }
}
/**********************************************************************/
void GpsProcessing(struct AcType *AC)
{
      struct AcGpsType *G;
      double DaysSinceWeek,DaysSinceRollover,DaysSinceEpoch,JD;
      long i;

      if (AC->Ngps == 0) {
         /* AC->Time, AC->PosN, AC->VelN */
         /* populated in 42sensors.c */
      }
      else {
         G = &AC->GPS[0];
         /* GPS Time is seconds since 6 Jan 1980 00:00:00.0, which is JD = 2444244.5 */
         DaysSinceWeek = G->Sec/86400.0;
         DaysSinceRollover = DaysSinceWeek + 7.0*G->Week;
         DaysSinceEpoch = DaysSinceRollover + 7168.0*G->Rollover;
         JD = DaysSinceEpoch + 2444244.5;
         /* AC->Time is seconds since J2000, which is JD = 2451545.0 */
         AC->Time = (JD-2451545.0)*86400.0;

         /* Position, Velocity */
         for(i=0;i<3;i++) {
            AC->PosN[i] = AC->GPS[0].PosN[i];
            AC->VelN[i] = AC->GPS[0].VelN[i];
         }
      }
}
/**********************************************************************/
void AccelProcessing(struct AcType *AC)
{
}
/**********************************************************************/
/*  End Sensor Processing Functions                                   */
/**********************************************************************/
/*  Some Actuator Processing Functions                                */
/**********************************************************************/
void WheelProcessing(struct AcType *AC)
{
      struct AcWhlType *W;
      long Iw;
      
      for(Iw=0;Iw<AC->Nwhl;Iw++) {
         W = &AC->Whl[Iw];
         W->Tcmd = Limit(-VoV(AC->Tcmd,W->DistVec),-W->Tmax,W->Tmax);
      }
}
/**********************************************************************/
void MtbProcessing(struct AcType *AC)
{
      struct AcMtbType *M;
      long Im;
      
      for(Im=0;Im<AC->Nmtb;Im++) {
         M = &AC->MTB[Im];
         M->Mcmd = Limit(VoV(AC->Mcmd,M->DistVec),-M->Mmax,M->Mmax);
      }
}
/**********************************************************************/
/*  End Actuator Processing Functions                                 */
/**********************************************************************/
void AcFsw(struct AcType *AC)
{
      struct AcCfsCtrlType *C;
      struct AcJointType *G;
      double L1[3],L2[3],L3[3];
      double Hb[3],HxB[3];
      long i;
      
      C = &AC->CfsCtrl;
      G = &AC->G[0];

      if (C->Init) {
         C->Init = 0;
         for(i=0;i<3;i++) FindPDGains(AC->MOI[i][i],0.1,0.7,&C->Kr[i],&C->Kp[i]);
         C->Kunl = 1.0E6;
         FindPDGains(100.0,0.2,1.0,&G->AngRateGain[0],&G->AngGain[0]);
         G->MaxAngRate[0] = 1.0*D2R;
         G->MaxTrq[0] = 10.0;
      }

/* .. Sensor Processing */
      GyroProcessing(AC);
      MagnetometerProcessing(AC);
      CssProcessing(AC);
      FssProcessing(AC);
      StarTrackerProcessing(AC);
      GpsProcessing(AC);
      
/* .. Commanded Attitude */
      CopyUnitV(AC->PosN,L3);
      VxV(AC->PosN,AC->VelN,L2);
      UNITV(L2);
      UNITV(L3);
      for(i=0;i<3;i++) {
         L2[i] = -L2[i];
         L3[i] = -L3[i];
      }
      VxV(L2,L3,L1);
      UNITV(L1);
      for(i=0;i<3;i++) {
         AC->CLN[0][i] = L1[i];
         AC->CLN[1][i] = L2[i];
         AC->CLN[2][i] = L3[i];
      }
      C2Q(AC->CLN,AC->qln);
      AC->wln[1] = -MAGV(AC->VelN)/MAGV(AC->PosN);
            
/* .. Attitude Control */
      QxQT(AC->qbn,AC->qln,AC->qbr);
      RECTIFYQ(AC->qbr);
      for(i=0;i<3;i++) {
         C->therr[i] = Limit(2.0*AC->qbr[i],-0.05,0.05);
         C->werr[i] = AC->wbn[i] - AC->wln[i];
         AC->Tcmd[i] = Limit(-C->Kr[i]*C->werr[i] - C->Kp[i]*C->therr[i],-0.1,0.1);
      }
/* .. Momentum Management */
      for(i=0;i<3;i++) Hb[i] = AC->MOI[i][i]*AC->wbn[i] + AC->Whl[i].H;
      VxV(Hb,AC->bvb,HxB);
      for(i=0;i<3;i++) AC->Mcmd[i] = C->Kunl*HxB[i];
      
/* .. Solar Array Steering */
      G->Cmd.Ang[0] = atan2(AC->svb[0],AC->svb[2]);
      
/* .. Actuator Processing */
      WheelProcessing(AC);
      MtbProcessing(AC);
}
#ifdef _AC_STANDALONE_
/**********************************************************************/
/* Send/receive messages from 42 */
//#include "../Database/AcMessages.c"
/**********************************************************************/
int main(int argc, char **argv)
{
      struct AcType AC;
      SOCKET Socket;
      char hostname[20] = "localhost";
      int Port = 101010;
      
      
      InitAC(&AC);
      
      Socket = InitSocketClient(hostname,Port,TRUE);
      
      while(1) {
         //ReceiveMessageFromSim(Socket);
         AcFsw(&AC);
         //SendMessageToSim(Socket);
      }
      
      return(0);
}
#endif
/* #ifdef __cplusplus
** }
** #endif
*/


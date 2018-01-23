/*    This file is distributed with 42,                               */
/*    the (mostly harmless) spacecraft dynamics simulation            */
/*    created by Eric Stoneking of NASA Goddard Space Flight Center   */

/*    Copyright 2010 United States Government                         */
/*    as represented by the Administrator                             */
/*    of the National Aeronautics and Space Administration.           */

/*    No copyright is claimed in the United States                    */
/*    under Title 17, U.S. Code.                                      */

/*    All Other Rights Reserved.                                      */


#define DECLARE_GLOBALS
#include "42.h"
#undef DECLARE_GLOBALS

/* #ifdef __cplusplus
** namespace _42 {
** using namespace Kit;
** #endif
*/

#ifdef _USE_GUI_
   extern int HandoffToGui(int argc, char **argv);
#endif

/**********************************************************************/
void ReportProgress(void)
{
#define PROGRESSPERCENT 10

      static long ProgressPercent = 0;
      static long ProgressCtr = 0;
      static double ProgressTime = 0.0;

      if (TimeMode == FAST_TIME) {

         if (SimTime >= ProgressTime) {
            ProgressCtr++;
            ProgressTime = (double) (ProgressCtr*PROGRESSPERCENT)/100.0*STOPTIME;
            printf("    42 Case %s is %3.1li%% Complete at Time = %12.3f\n",
               InOutPath,ProgressPercent,SimTime);
            ProgressPercent += PROGRESSPERCENT;
         }
      }
}
/**********************************************************************/
void ManageFlags(void)
{
      long nout,GLnout;
      static long iout = 1000000;
      static long GLiout = 1000000;

      nout = ((long) (DTOUT/DTSIM + 0.5));
      GLnout = ((long) (DTOUTGL/DTSIM + 0.5));

      iout++;
      if(iout >= nout) {
         iout=0;
         OutFlag = TRUE;
      }
      else OutFlag = FALSE;

      GLiout++;
      if(GLiout >= GLnout) {
         GLiout=0;
         GLOutFlag = TRUE;
      }
      else GLOutFlag = FALSE;

}
/**********************************************************************/
long AdvanceTime(void)
{
      static long itime = 0;
      long Done;

      /* Advance time to next Timestep */
      #if defined _USE_SYSTEM_TIME_
         switch (TimeMode) {
            case FAST_TIME :
               SimTime += DTSIM;
               itime = (long) ((SimTime+0.5*DTSIM)/(DTSIM));
               SimTime = ((double) itime)*DTSIM;
               AbsTime = AbsTime0 + SimTime;
               break;
            case REAL_TIME :
               usleep(1.0E6*DTSIM);
               SimTime += DTSIM;
               itime = (long) ((SimTime+0.5*DTSIM)/(DTSIM));
               SimTime = ((double) itime)*DTSIM;
               AbsTime = AbsTime0 + SimTime;
               break;
            case EXTERNAL_TIME :
               usleep(1.0E6*DTSIM);
               SimTime += DTSIM;
               itime = (long) ((SimTime+0.5*DTSIM)/(DTSIM));
               SimTime = ((double) itime)*DTSIM;
               RealSystemTime(&Year,&doy,&Month,&Day,&Hour,&Minute,&Second,DTSIM);
               AbsTime = DateToAbsTime(Year,Month,Day,Hour,Minute,Second+AbsTimeOffset);
               JulDay = AbsTimeToJD(AbsTime);
               JDToGpsTime(JulDay,&GpsRollover,&GpsWeek,&GpsSecond);
               AbsTime0 = AbsTime - SimTime;
               break;
            /* case SSUP_TIME:
            **   RealSystemTime(&Year,&doy,&Month,&Day,&Hour,&Minute,&Second);
            **   AbsTime = DateToAbsTime(Year,Month,Day,Hour,Minute,Second+AbsTimeOffset);
            **   JulDay = AbsTimeToJD(AbsTime);
            **   SimTime = AbsTime - AbsTime0;
            **   break;
            */
         }
      #else
         SimTime += DTSIM;
         itime = (long) ((SimTime+0.5*DTSIM)/(DTSIM));
         SimTime = ((double) itime)*DTSIM;
         AbsTime = AbsTime0 + SimTime;
      #endif

      /* Check for end of run */
      if (SimTime > STOPTIME) Done = 1;
      else Done = 0;

      return(Done);
}
/*********************************************************************/
/* The SC Bounding Box is referred to the origin of B0,              */
/* and expressed in B0                                               */
void UpdateScBoundingBox(struct SCType *S)
{
#define REFPT_CM 0
      struct BodyType *B, *B0;
      struct BoundingBoxType *BBox;
      struct GeomType *G;
      double ctrB[3],ctrN[3],ctrB0[3],maxB0,minB0,r[3];
      long Ib,i;

      B0 = &S->B[0];
      BBox = &S->BBox;

      for(Ib=0;Ib<S->Nb;Ib++) {
         B = &S->B[Ib];
         G = &Geom[B->GeomTag];
         for(i=0;i<3;i++) {
            ctrB[i] = G->BBox.center[i];
            if (S->RefPt == REFPT_CM) {
               ctrB[i] -= B->cm[i];
            }
         }
         MTxV(B->CN,ctrB,ctrN);
         for(i=0;i<3;i++) {
            ctrN[i] += (B->pn[i]-B0->pn[i]);
         }
         MxV(B0->CN,ctrN,ctrB0);
         for(i=0;i<3;i++) {
            if (S->RefPt == REFPT_CM) {
               ctrB0[i] += B0->cm[i];
            }
            maxB0 = ctrB0[i] + G->BBox.radius;
            minB0 = ctrB0[i] - G->BBox.radius;
            if(BBox->max[i] < maxB0) BBox->max[i] = maxB0;
            if(BBox->min[i] > minB0) BBox->min[i] = minB0;
         }
      }
      for(i=0;i<3;i++) {
         BBox->center[i] = 0.5*(BBox->max[i]+BBox->min[i]);
         r[i] = BBox->max[i]-BBox->center[i];
      }
      BBox->radius = MAGV(r);
#undef REFPT_CM
}
/**********************************************************************/
void ManageBoundingBoxes(void)
{
      static long BBoxCtr = 100;
      long Isc;
      struct SCType *S;

      BBoxCtr++;
      if (BBoxCtr > 100) {
         BBoxCtr = 0;
         for(Isc=0;Isc<Nsc;Isc++) {
            S = &SC[Isc];
            if (S->Exists) {
               UpdateScBoundingBox(S);
            }
         }
      }
}
/**********************************************************************/
/* Zero forces and torques                                            */
void ZeroFrcTrq(void)
{
      struct SCType *S;
      long Isc,Ib;

      for(Isc=0;Isc<Nsc;Isc++) {
         S = &SC[Isc];
         S->Frc[0] = 0.0;
         S->Frc[1] = 0.0;
         S->Frc[2] = 0.0;
         for(Ib=0;Ib<S->Nb;Ib++) {
            S->B[Ib].Frc[0] = 0.0;
            S->B[Ib].Frc[1] = 0.0;
            S->B[Ib].Frc[2] = 0.0;
            S->B[Ib].Trq[0] = 0.0;
            S->B[Ib].Trq[1] = 0.0;
            S->B[Ib].Trq[2] = 0.0;
         }
      }
}
/**********************************************************************/
long SimStep(void)
{
      long Isc;
      static long First = 1;
      struct SCType *S;
      long SimComplete;
      double TotalRunTime;

      if (First) {
         First = 0;
         SimTime = 0.0;
         #if defined _USE_SYSTEM_TIME_
            /* First call just initializes timer */
            RealRunTime(&TotalRunTime,DTSIM);
         #endif
         ManageFlags();

         Ephemerides(); /* Sun, Moon, Planets, Spacecraft, Useful Auxiliary Frames */

         ZeroFrcTrq();
         for(Isc=0;Isc<Nsc;Isc++) {
            S = &SC[Isc];
            if (S->Exists) {
               Environment(S);    /* Magnetic Field, Atmospheric Density */
               Perturbations(S);  /* Environmental Forces and Torques */
               Sensors(S);
               FlightSoftWare(S);
               Actuators(S);
               PartitionForces(S); /* Orbit-affecting and "internal" */
            }
         }
         Report();  /* File Output */
      }

      ReportProgress();
      ManageFlags();

      /* Read and Interpret Command Script File */
      CmdInterpreter();

      /* Update Dynamics to next Timestep */
      for(Isc=0;Isc<Nsc;Isc++) {
         if (SC[Isc].Exists) Dynamics(&SC[Isc]);
      }
      OrbitMotion();
      SimComplete = AdvanceTime();

      /* Update SC Bounding Boxes occasionally */
      ManageBoundingBoxes();

      #ifdef _ENABLE_SOCKETS_
         InterProcessComm(); /* Send and receive from external processes */
      #endif
      Ephemerides(); /* Sun, Moon, Planets, Spacecraft, Useful Auxiliary Frames */
      ZeroFrcTrq();
      for(Isc=0;Isc<Nsc;Isc++) {
         S = &SC[Isc];
         if (S->Exists) {
            Environment(S);    /* Magnetic Field, Atmospheric Density */
            Perturbations(S);  /* Environmental Forces and Torques */
            Sensors(S);
            FlightSoftWare(S);
            Actuators(S);
            PartitionForces(S); /* Orbit-affecting and "internal" */
         }
      }
      Report();  /* File Output */

      /* Exit when Stoptime is reached */
      if (SimComplete) {
         #if defined _USE_SYSTEM_TIME_
            if (TimeMode == FAST_TIME) {
               RealRunTime(&TotalRunTime,DTSIM);
               printf("     Total Run Time = %9.2lf sec\n", TotalRunTime);
               printf("     Sim Speed = %8.2lf x Real\n",
                  STOPTIME/TotalRunTime);
            }
         #endif
      }
      return(SimComplete);

}
/**********************************************************************/
int exec(int argc,char **argv)
{
      long Isc;
      long Done = 0;

      InitSim(argc,argv);
      for (Isc=0;Isc<Nsc;Isc++) {
         if (SC[Isc].Exists) {
            InitSpacecraft(&SC[Isc]);
            InitFSW(&SC[Isc]);
         }
      }
      CmdInterpreter();
      #ifdef _ENABLE_SOCKETS_
         InitInterProcessComm();
      #endif
      #ifdef _USE_GUI_
         if (GLEnable) HandoffToGui(argc,argv);
         else {
            while(!Done) {
               Done = SimStep();
            }
         }
      #else
         /* Crunch numbers till done */
         while(!Done) {
            Done = SimStep();
         }
      #endif

      return(0);
}

/* #ifdef __cplusplus
** }
** #endif
*/

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
/* #include "42fsw.h" */

/* #ifdef __cplusplus
** namespace _42 {
** using namespace Kit;
** #endif
*/

/**********************************************************************/
long FswCmdInterpreter(char CmdLine[512],double *CmdTime)
{
      long NewCmdProcessed = FALSE;
      long Isc,Ib,Ig,Iw,It,i,Isct,Ibt,Ithr;
      char response[80];
      char FrameChar;
      long Frame;
      struct FSWType *Fsw;
      struct CmdType *Cmd;
      struct CmdVecType *CV;
      double q[4],Ang[3],C[3][3],VecR[3],Vec[3],VecH[3];
      double RA,Dec;
      double Lng,Lat,Alt;
      double wc,amax,vmax;
      long RotSeq;
      char VecString[20],TargetString[20];

      if (sscanf(CmdLine,"%lf SC[%ld] qrn = [%lf %lf %lf %lf]",
         CmdTime,&Isc,&q[0],&q[1],&q[2],&q[3]) == 6) {
         NewCmdProcessed = TRUE;
         Fsw = &SC[Isc].FSW;
         Cmd = &Fsw->Cmd;
         Cmd->Parm = PARM_QUATERNION;
         Cmd->Frame = FRAME_N;
         for(i=0;i<4;i++) Cmd->qrn[i] = q[i];
      }

      else if (sscanf(CmdLine,"%lf SC[%ld] qrl = [%lf %lf %lf %lf]",
         CmdTime,&Isc,&q[0],&q[1],&q[2],&q[3]) == 6) {
         NewCmdProcessed = TRUE;
         Fsw = &SC[Isc].FSW;
         Cmd = &Fsw->Cmd;
         Cmd->Parm = PARM_QUATERNION;
         Cmd->Frame = FRAME_L;
         for(i=0;i<4;i++) Cmd->qrl[i] = q[i];
      }

      else if (sscanf(CmdLine,"%lf SC[%ld] FswTag = %s",
         CmdTime,&Isc,response) == 3) {
         NewCmdProcessed = TRUE;
         SC[Isc].FswTag = DecodeString(response);
      }

      else if (sscanf(CmdLine,"%lf SC[%ld] Cmd Angles = [%lf %lf %lf] deg, Seq = %ld wrt %c Frame",
         CmdTime,&Isc,&Ang[0],&Ang[1],&Ang[2],&RotSeq,&FrameChar) == 7) {
         NewCmdProcessed = TRUE;
         Fsw = &SC[Isc].FSW;
         Cmd = &Fsw->Cmd;
         Cmd->Parm = PARM_EULER_ANGLES;
         if (FrameChar == 'L') Cmd->Frame = FRAME_L;
         else Cmd->Frame = FRAME_N;
         for(i=0;i<3;i++) Cmd->Ang[i] = Ang[i]*D2R;
         Cmd->RotSeq = RotSeq;
         A2C(RotSeq,Ang[0]*D2R,Ang[1]*D2R,Ang[2]*D2R,C);
         if (Cmd->Frame == FRAME_L) C2Q(C,Cmd->qrl);
         else C2Q(C,Cmd->qrn);
      }

      else if (sscanf(CmdLine,"%lf SC[%ld].G[%ld] Cmd Angles = [%lf %lf %lf] deg",
         CmdTime,&Isc,&Ig,&Ang[0],&Ang[1],&Ang[2]) == 6) {
         NewCmdProcessed = TRUE;
         Fsw = &SC[Isc].FSW;
         Cmd = &Fsw->GimCmd[Ig];
         Cmd->Parm = PARM_EULER_ANGLES;
         for(i=0;i<3;i++) Cmd->Ang[i] = Ang[i]*D2R;
      }

      else if (sscanf(CmdLine,"%lf Point SC[%ld].B[%ld] %s Vector [%lf %lf %lf] at RA = %lf deg, Dec = %lf deg",
         CmdTime,&Isc,&Ib,VecString,&VecR[0],&VecR[1],&VecR[2],&RA,&Dec) == 9) {
         NewCmdProcessed = TRUE;
         Fsw = &SC[Isc].FSW;
         if (Ib == 0) {
            Cmd = &Fsw->Cmd;
         }
         else {
            Ig = SC[Isc].B[Ib].Gin;
            Cmd = &Fsw->GimCmd[Ig];
         }
         Cmd->Parm = PARM_VECTORS;
         if (!strcmp(VecString,"Primary")) CV = &Cmd->PriVec;
         else CV = &Cmd->SecVec;
         CV->Mode = CMD_DIRECTION;
         CV->Frame = FRAME_N;
         UNITV(VecR);
         for(i=0;i<3;i++) CV->R[i] = VecR[i];
         CV->N[0] = cos(RA*D2R)*cos(Dec*D2R);
         CV->N[1] = sin(RA*D2R)*cos(Dec*D2R);
         CV->N[2] = sin(Dec*D2R);
      }

      else if (sscanf(CmdLine,
         "%lf Point SC[%ld].B[%ld] %s Vector [%lf %lf %lf] at World[%ld] Lng = %lf deg, Lat = %lf deg, Alt = %lf km",
         CmdTime,&Isc,&Ib,VecString,&VecR[0],&VecR[1],&VecR[2],&Iw,&Lng,&Lat,&Alt) == 11) {
         NewCmdProcessed = TRUE;
         Fsw = &SC[Isc].FSW;
         if (Ib == 0) {
            Cmd = &Fsw->Cmd;
         }
         else {
            Ig = SC[Isc].B[Ib].Gin;
            Cmd = &Fsw->GimCmd[Ig];
         }
         Cmd->Parm = PARM_VECTORS;
         Cmd->Frame = FRAME_N;
         if (!strcmp(VecString,"Primary")) CV = &Cmd->PriVec;
         else CV = &Cmd->SecVec;
         CV->Mode = CMD_TARGET;
         CV->Frame = FRAME_N;
         CV->TrgType = TARGET_WORLD;
         CV->TrgWorld = Iw;
         UNITV(VecR);
         for(i=0;i<3;i++) CV->R[i] = VecR[i];
         CV->W[0] = (World[Iw].rad+1000.0*Alt)*cos(Lng*D2R)*cos(Lat*D2R);
         CV->W[1] = (World[Iw].rad+1000.0*Alt)*sin(Lng*D2R)*cos(Lat*D2R);
         CV->W[2] = (World[Iw].rad+1000.0*Alt)*sin(Lat*D2R);
      }

      else if (sscanf(CmdLine,"%lf Point SC[%ld].B[%ld] %s Vector [%lf %lf %lf] at World[%ld]",
         CmdTime,&Isc,&Ib,VecString,&VecR[0],&VecR[1],&VecR[2],&Iw) == 8) {
         NewCmdProcessed = TRUE;
         Fsw = &SC[Isc].FSW;
         if (Ib == 0) {
            Cmd = &Fsw->Cmd;
         }
         else {
            Ig = SC[Isc].B[Ib].Gin;
            Cmd = &Fsw->GimCmd[Ig];
         }
         Cmd->Parm = PARM_VECTORS;
         Cmd->Frame = FRAME_N;
         if (!strcmp(VecString,"Primary")) CV = &Cmd->PriVec;
         else CV = &Cmd->SecVec;
         CV->Mode = CMD_TARGET;
         CV->Frame = FRAME_N;
         CV->TrgType = TARGET_WORLD;
         CV->TrgWorld = Iw;
         UNITV(VecR);
         for(i=0;i<3;i++) CV->R[i] = VecR[i];
         for(i=0;i<3;i++) CV->W[i] = 0.0;
      }

      else if (sscanf(CmdLine,"%lf Point SC[%ld].B[%ld] %s Vector [%lf %lf %lf] at GroundStation[%ld]",
         CmdTime,&Isc,&Ib,VecString,&VecR[0],&VecR[1],&VecR[2],&It) == 8) {
         NewCmdProcessed = TRUE;
         Fsw = &SC[Isc].FSW;
         if (Ib == 0) {
            Cmd = &Fsw->Cmd;
         }
         else {
            Ig = SC[Isc].B[Ib].Gin;
            Cmd = &Fsw->GimCmd[Ig];
         }
         Cmd->Parm = PARM_VECTORS;
         Cmd->Frame = FRAME_N;
         if (!strcmp(VecString,"Primary")) CV = &Cmd->PriVec;
         else CV = &Cmd->SecVec;
         CV->Mode = CMD_TARGET;
         CV->Frame = FRAME_N;
         CV->TrgType = TARGET_WORLD;
         CV->TrgWorld = GroundStation[It].World;
         UNITV(VecR);
         for(i=0;i<3;i++) CV->R[i] = VecR[i];
         for(i=0;i<3;i++) CV->W[i] = GroundStation[It].PosW[i];
      }

      else if (sscanf(CmdLine,"%lf Point SC[%ld].B[%ld] %s Vector [%lf %lf %lf] at SC[%ld].B[%ld] point [%lf %lf %lf]",
         CmdTime,&Isc,&Ib,VecString,&VecR[0],&VecR[1],&VecR[2],&Isct,&Ibt,&Vec[0],&Vec[1],&Vec[2]) == 12) {
         NewCmdProcessed = TRUE;
         Fsw = &SC[Isc].FSW;
         if (Ib == 0) {
            Cmd = &Fsw->Cmd;
         }
         else {
            Ig = SC[Isc].B[Ib].Gin;
            Cmd = &Fsw->GimCmd[Ig];
         }
         Cmd->Parm = PARM_VECTORS;
         Cmd->Frame = FRAME_N;
         if (!strcmp(VecString,"Primary")) CV = &Cmd->PriVec;
         else CV = &Cmd->SecVec;
         CV->Mode = CMD_TARGET;
         CV->Frame = FRAME_N;
         CV->TrgType = TARGET_BODY;
         CV->TrgSC = Isct;
         CV->TrgBody = Ibt;
         CopyUnitV(VecR,CV->R);
         for(i=0;i<3;i++) CV->T[i] = Vec[i];
      }

      else if (sscanf(CmdLine,"%lf Point SC[%ld].B[%ld] %s Vector [%lf %lf %lf] at SC[%ld]",
         CmdTime,&Isc,&Ib,VecString,&VecR[0],&VecR[1],&VecR[2],&Isct) == 8) {
         NewCmdProcessed = TRUE;
         Fsw = &SC[Isc].FSW;
         if (Ib == 0) {
            Cmd = &Fsw->Cmd;
         }
         else {
            Ig = SC[Isc].B[Ib].Gin;
            Cmd = &Fsw->GimCmd[Ig];
         }
         Cmd->Parm = PARM_VECTORS;
         Cmd->Frame = FRAME_N;
         if (!strcmp(VecString,"Primary")) CV = &Cmd->PriVec;
         else CV = &Cmd->SecVec;
         CV->Mode = CMD_TARGET;
         CV->Frame = FRAME_N;
         CV->TrgType = TARGET_SC;
         CV->TrgSC = Isct;
         CopyUnitV(VecR,CV->R);
      }

      else if (sscanf(CmdLine,"%lf Point SC[%ld].B[%ld] %s Vector [%lf %lf %lf] at %s",
         CmdTime,&Isc,&Ib,VecString,&VecR[0],&VecR[1],&VecR[2],TargetString) == 8) {
         NewCmdProcessed = TRUE;
         Fsw = &SC[Isc].FSW;
         if (Ib == 0) {
            Cmd = &Fsw->Cmd;
         }
         else {
            Ig = SC[Isc].B[Ib].Gin;
            Cmd = &Fsw->GimCmd[Ig];
         }
         Cmd->Parm = PARM_VECTORS;
         Cmd->Frame = FRAME_N;
         if (!strcmp(VecString,"Primary")) CV = &Cmd->PriVec;
         else CV = &Cmd->SecVec;
         CV->Mode = CMD_TARGET;
         CV->Frame = FRAME_N;
         if (!strcmp(TargetString,"EARTH")) {
            CV->TrgType = TARGET_WORLD;
            CV->TrgWorld = EARTH;
         }
         else if (!strcmp(TargetString,"MOON")) {
            CV->TrgType = TARGET_WORLD;
            CV->TrgWorld = LUNA;
         }
         else if (!strcmp(TargetString,"LUNA")) {
            CV->TrgType = TARGET_WORLD;
            CV->TrgWorld = LUNA;
         }
         else if (!strcmp(TargetString,"MERCURY")) {
            CV->TrgType = TARGET_WORLD;
            CV->TrgWorld = MERCURY;
         }
         else if (!strcmp(TargetString,"VENUS")) {
            CV->TrgType = TARGET_WORLD;
            CV->TrgWorld = VENUS;
         }
         else if (!strcmp(TargetString,"MARS")) {
            CV->TrgType = TARGET_WORLD;
            CV->TrgWorld = MARS;
         }
         else if (!strcmp(TargetString,"JUPITER")) {
            CV->TrgType = TARGET_WORLD;
            CV->TrgWorld = JUPITER;
         }
         else if (!strcmp(TargetString,"SATURN")) {
            CV->TrgType = TARGET_WORLD;
            CV->TrgWorld = SATURN;
         }
         else if (!strcmp(TargetString,"URANUS")) {
            CV->TrgType = TARGET_WORLD;
            CV->TrgWorld = URANUS;
         }
         else if (!strcmp(TargetString,"NEPTUNE")) {
            CV->TrgType = TARGET_WORLD;
            CV->TrgWorld = NEPTUNE;
         }
         else if (!strcmp(TargetString,"PLUTO")) {
            CV->TrgType = TARGET_WORLD;
            CV->TrgWorld = PLUTO;
         }
         else if (!strcmp(TargetString,"VELOCITY")) {
            CV->TrgType = TARGET_VELOCITY;
         }
         else if (!strcmp(TargetString,"MAGFIELD")) {
            CV->TrgType = TARGET_MAGFIELD;
         }
         else if (!strcmp(TargetString,"TDRS")) {
            CV->TrgType = TARGET_TDRS;
         }
         else {
            CV->TrgType = TARGET_WORLD;
            CV->TrgWorld = SOL;
         }
         UNITV(VecR);
         for(i=0;i<3;i++) CV->R[i] = VecR[i];
      }

      else if (sscanf(CmdLine,"%lf Align SC[%ld].B[%ld] %s Vector [%lf %lf %lf] with SC[%ld].B[%ld] vector [%lf %lf %lf]",
         CmdTime,&Isc,&Ib,VecString,&VecR[0],&VecR[1],&VecR[2],&Isct,&Ibt,&Vec[0],&Vec[1],&Vec[2]) == 12) {
         NewCmdProcessed = TRUE;
         Fsw = &SC[Isc].FSW;
         if (Ib == 0) {
            Cmd = &Fsw->Cmd;
         }
         else {
            Ig = SC[Isc].B[Ib].Gin;
            Cmd = &Fsw->GimCmd[Ig];
         }
         Cmd->Parm = PARM_VECTORS;
         Cmd->Frame = FRAME_N;
         if (!strcmp(VecString,"Primary")) CV = &Cmd->PriVec;
         else CV = &Cmd->SecVec;
         CV->Mode = CMD_DIRECTION;
         CV->Frame = FRAME_B;
         CV->TrgType = TARGET_BODY;
         CV->TrgSC = Isct;
         CV->TrgBody = Ibt;
         CopyUnitV(VecR,CV->R);
         for(i=0;i<3;i++) CV->T[i] = Vec[i];
      }

      else if (sscanf(CmdLine,"%lf Align SC[%ld].B[%ld] %s Vector [%lf %lf %lf] with %c-frame Vector [%lf %lf %lf]",
         CmdTime,&Isc,&Ib,VecString,&VecR[0],&VecR[1],&VecR[2],&FrameChar,&Vec[0],&Vec[1],&Vec[2]) == 11) {
         NewCmdProcessed = TRUE;
         Fsw = &SC[Isc].FSW;
         if (FrameChar == 'L') Frame = FRAME_L;
         else if (FrameChar == 'H') {
            Frame = FRAME_N;
            for(i=0;i<3;i++) VecH[i] = Vec[i];
            MxV(World[Orb[SC[Isc].RefOrb].World].CNH,VecH,Vec);
         }
         else Frame = FRAME_N;
         if (Ib == 0) {
            Cmd = &Fsw->Cmd;
         }
         else {
            Ig = SC[Isc].B[Ib].Gin;
            Cmd = &Fsw->GimCmd[Ig];
         }
         Cmd->Parm = PARM_VECTORS;
         if (!strcmp(VecString,"Primary")) CV = &Cmd->PriVec;
         else CV = &Cmd->SecVec;
         CV->Mode = CMD_DIRECTION;
         CV->Frame = Frame;
         UNITV(VecR);
         UNITV(Vec);
         for(i=0;i<3;i++) CV->R[i] = VecR[i];
         if (Frame == FRAME_L) {
            for(i=0;i<3;i++) CV->L[i] = Vec[i];
         }
         else {
            for(i=0;i<3;i++) CV->N[i] = Vec[i];
         }
      }

      else if (sscanf(CmdLine,"%lf SC[%ld].Thr[%ld] %s",
         CmdTime,&Isc,&Ithr,response) == 4) {
         NewCmdProcessed = TRUE;
         if (DecodeString(response))
            SC[Isc].FSW.Thrcmd[Ithr] = SC[Isc].Thr[Ithr].Fmax*DTSIM;
         else
            SC[Isc].FSW.Thrcmd[Ithr] = 0.0;
      }

      else if (sscanf(CmdLine,"Event Eclipse Entry SC[%ld] qrl = [%lf %lf %lf %lf]",
         &Isc,&q[0],&q[1],&q[2],&q[3]) == 5) {
         *CmdTime = SimTime+DTSIM; /* Allows exiting while loop in CmdInterpreter */
         if (SC[Isc].Eclipse) { /* Will pend on this command until this condition is true */
            NewCmdProcessed = TRUE;
            Fsw = &SC[Isc].FSW;
            Cmd = &Fsw->Cmd;
            Cmd->Parm = PARM_QUATERNION;
            Cmd->Frame = FRAME_L;
            for(i=0;i<4;i++) Cmd->qrl[i] = q[i];
         }
      }
      else if (sscanf(CmdLine,"Event Eclipse Exit SC[%ld] qrl = [%lf %lf %lf %lf]",
         &Isc,&q[0],&q[1],&q[2],&q[3]) == 5) {
         *CmdTime = SimTime+DTSIM; /* Allows exiting while loop in CmdInterpreter */
         if (!SC[Isc].Eclipse) { /* Will pend on this command until this condition is true */
            NewCmdProcessed = TRUE;
            Fsw = &SC[Isc].FSW;
            Cmd = &Fsw->Cmd;
            Cmd->Parm = PARM_QUATERNION;
            Cmd->Frame = FRAME_L;
            for(i=0;i<4;i++) Cmd->qrl[i] = q[i];
         }
      }

      else if (sscanf(CmdLine,
         "Event Eclipse Entry SC[%ld] Cmd Angles = [%lf %lf %lf] deg, Seq = %ld wrt %c Frame",
         &Isc,&Ang[0],&Ang[1],&Ang[2],&RotSeq,&FrameChar) == 6) {
         *CmdTime = SimTime+DTSIM; /* Allows exiting while loop in CmdInterpreter */
         if (SC[Isc].Eclipse) { /* Will pend on this command until this condition is true */
            NewCmdProcessed = TRUE;
            Fsw = &SC[Isc].FSW;
            Cmd = &Fsw->Cmd;
            Cmd->Parm = PARM_EULER_ANGLES;
            if (FrameChar == 'L') Cmd->Frame = FRAME_L;
            else Cmd->Frame = FRAME_N;
            for(i=0;i<3;i++) Cmd->Ang[i] = Ang[i]*D2R;
            Cmd->RotSeq = RotSeq;
            A2C(RotSeq,Ang[0]*D2R,Ang[1]*D2R,Ang[2]*D2R,C);
            if (Cmd->Frame == FRAME_L) C2Q(C,Cmd->qrl);
            else C2Q(C,Cmd->qrn);
         }
      }

      else if (sscanf(CmdLine,
         "Event Eclipse Exit SC[%ld] Cmd Angles = [%lf %lf %lf] deg, Seq = %ld wrt %c Frame",
         &Isc,&Ang[0],&Ang[1],&Ang[2],&RotSeq,&FrameChar) == 6) {
         *CmdTime = SimTime+DTSIM; /* Allows exiting while loop in CmdInterpreter */
         if (!SC[Isc].Eclipse) { /* Will pend on this command until this condition is true */
            NewCmdProcessed = TRUE;
            Fsw = &SC[Isc].FSW;
            Cmd = &Fsw->Cmd;
            Cmd->Parm = PARM_EULER_ANGLES;
            if (FrameChar == 'L') Cmd->Frame = FRAME_L;
            else Cmd->Frame = FRAME_N;
            for(i=0;i<3;i++) Cmd->Ang[i] = Ang[i]*D2R;
            Cmd->RotSeq = RotSeq;
            A2C(RotSeq,Ang[0]*D2R,Ang[1]*D2R,Ang[2]*D2R,C);
            if (Cmd->Frame == FRAME_L) C2Q(C,Cmd->qrl);
            else C2Q(C,Cmd->qrn);
         }
      }

      else if (sscanf(CmdLine,"%lf Set SC[%ld] RampCoastGlide wc = %lf Hz, amax = %lf, vmax = %lf",
         CmdTime,&Isc,&wc,&amax,&vmax) == 5) {
         NewCmdProcessed = TRUE;
         Fsw = &SC[Isc].FSW;
         Fsw->RcgWC = wc*TwoPi;
         Fsw->RcgAmax = amax;
         Fsw->RcgVmax = vmax;
      }

      else if (sscanf(CmdLine,"%lf Spin SC[%ld] about Primary Vector at %lf deg/sec",
         CmdTime,&Isc,&wc) == 3) {
         NewCmdProcessed = TRUE;
         Fsw = &SC[Isc].FSW;
         
         Fsw->Cmd.Parm = PARM_AXIS_SPIN;
         Fsw->Cmd.SpinRate = wc*D2R;
      }

      return(NewCmdProcessed);
}
/**********************************************************************/
/* Given a relative position and velocity vector, find the angular    */
/* velocity at which the relative position vector is rotating.        */
void RelMotionToAngRate(double RelPosN[3], double RelVelN[3],
                        double wn[3])
{
      double magp,phat[3],Axis[3],Vpar,Vperp[3],magvp;
      long i;

      magp = CopyUnitV(RelPosN,phat);

      VxV(RelPosN,RelVelN,Axis);
      UNITV(Axis);

      Vpar = VoV(RelVelN,phat);
      for(i=0;i<3;i++) Vperp[i] = RelVelN[i]-Vpar*phat[i];
      magvp = MAGV(Vperp);
      for(i=0;i<3;i++) wn[i] = magvp/magp*Axis[i];
}
/**********************************************************************/
void FindCmdVecN(struct SCType *S, struct CmdVecType *CV)
{
      struct WorldType *W;
      double RelPosB[3],vb[3];
      double RelPosN[3],RelPosH[3],RelVelN[3],RelVelH[3];
      double pcmn[3],pn[3],vn[3],ph[3],vh[3];
      double CosPriMerAng,SinPriMerAng;
      double MaxToS,Rhat[3],ToS;
      long It,i;

      switch (CV->TrgType) {
         case TARGET_WORLD:
            W = &World[CV->TrgWorld];
            CosPriMerAng = cos(W->PriMerAng);
            SinPriMerAng = sin(W->PriMerAng);
            pn[0] =  CV->W[0]*CosPriMerAng - CV->W[1]*SinPriMerAng;
            pn[1] =  CV->W[0]*SinPriMerAng + CV->W[1]*CosPriMerAng;
            pn[2] =  CV->W[2];
            vn[0] = -CV->W[0]*SinPriMerAng - CV->W[1]*CosPriMerAng;
            vn[1] =  CV->W[0]*CosPriMerAng - CV->W[1]*SinPriMerAng;
            vn[2] = 0.0;
            if (CV->TrgWorld == Orb[SC->RefOrb].World) {
               for(i=0;i<3;i++) {
                  RelPosN[i] = pn[i] - S->PosN[i];
                  RelVelN[i] = vn[i] - S->VelN[i];
               }
            }
            else {
               MTxV(W->CNH,pn,ph);
               MTxV(W->CNH,vn,vh);
               for(i=0;i<3;i++) {
                  RelPosH[i] = (W->PosH[i]+ph[i])-S->PosH[i];
                  RelVelH[i] = (W->VelH[i]+vh[i])-S->VelH[i];
               }
               MxV(World[Orb[S->RefOrb].World].CNH,RelPosH,RelPosN);
               MxV(World[Orb[S->RefOrb].World].CNH,RelVelH,RelVelN);
            }
            CopyUnitV(RelPosN,CV->N);
            RelMotionToAngRate(RelPosN,RelVelN,CV->wn);
            break;
         case TARGET_SC:
            if (SC[CV->TrgSC].RefOrb == S->RefOrb) {
               for(i=0;i<3;i++) {
                  RelPosN[i] = SC[CV->TrgSC].PosR[i]-S->PosR[i];
                  RelVelN[i] = SC[CV->TrgSC].VelR[i]-S->VelR[i];
               }
            }
            else if (Orb[SC[CV->TrgSC].RefOrb].World == Orb[S->RefOrb].World) {
               for(i=0;i<3;i++) {
                  RelPosN[i] = SC[CV->TrgSC].PosN[i]-S->PosN[i];
                  RelVelN[i] = SC[CV->TrgSC].VelN[i]-S->VelN[i];
               }
            }
            else {
               for(i=0;i<3;i++) {
                  RelPosH[i] = SC[CV->TrgSC].PosH[i]-S->PosH[i];
                  RelVelH[i] = SC[CV->TrgSC].VelH[i]-S->VelH[i];
               }
               MxV(World[Orb[S->RefOrb].World].CNH,RelPosH,RelPosN);
               MxV(World[Orb[S->RefOrb].World].CNH,RelVelH,RelVelN);
            }
            CopyUnitV(RelPosN,CV->N);
            RelMotionToAngRate(RelPosN,RelVelN,CV->wn);
            break;
         case TARGET_BODY:
            MTxV(SC[CV->TrgSC].B[0].CN,SC[CV->TrgSC].cm,pcmn);
            MTxV(SC[CV->TrgSC].B[CV->TrgBody].CN,CV->T,pn);
            for(i=0;i<3;i++) RelPosB[i] = CV->T[i] - SC[CV->TrgSC].B[CV->TrgBody].cm[i];
            VxV(SC[CV->TrgSC].B[CV->TrgBody].wn,RelPosB,vb);
            MTxV(SC[CV->TrgSC].B[CV->TrgBody].CN,vb,vn);
            for(i=0;i<3;i++) {
               pn[i] += SC[CV->TrgSC].B[CV->TrgBody].pn[i]-pcmn[i];
               vn[i] += SC[CV->TrgSC].B[CV->TrgBody].vn[i];
            }
            if (SC[CV->TrgSC].RefOrb == S->RefOrb) {
               for(i=0;i<3;i++) {
                  RelPosN[i] = SC[CV->TrgSC].PosR[i] + pn[i] - S->PosR[i];
                  RelVelN[i] = SC[CV->TrgSC].VelR[i] + vn[i] - S->VelR[i];
               }
            }
            else if (Orb[SC[CV->TrgSC].RefOrb].World == Orb[S->RefOrb].World) {
               for(i=0;i<3;i++) {
                  RelPosN[i] = SC[CV->TrgSC].PosN[i] + pn[i] - S->PosN[i];
                  RelVelN[i] = SC[CV->TrgSC].VelN[i] + vn[i] - S->VelN[i];
               }
            }
            else {
               MTxV(World[Orb[SC[CV->TrgSC].RefOrb].World].CNH,pn,ph);
               MTxV(World[Orb[SC[CV->TrgSC].RefOrb].World].CNH,vn,vh);
               for(i=0;i<3;i++) {
                  RelPosH[i] = SC[CV->TrgSC].PosH[i] + ph[i] - S->PosH[i];
                  RelVelH[i] = SC[CV->TrgSC].VelH[i] + vh[i] - S->VelH[i];
               }
               MxV(World[Orb[S->RefOrb].World].CNH,RelPosH,RelPosN);
               MxV(World[Orb[S->RefOrb].World].CNH,RelVelH,RelVelN);
            }
            CopyUnitV(RelPosN,CV->N);
            RelMotionToAngRate(RelPosN,RelVelN,CV->wn);
            break;
         case TARGET_VELOCITY:
            for(i=0;i<3;i++) CV->N[i] = S->VelN[i];
            UNITV(CV->N);
            break;
         case TARGET_MAGFIELD:
            for(i=0;i<3;i++) CV->N[i] = S->bvn[i];
            UNITV(CV->N);
            break;
         case TARGET_TDRS:
            CV->N[0] = 0.0;
            CV->N[1] = 0.0;
            CV->N[2] = 1.0;
            for(i=0;i<3;i++) CV->wn[i] = 0.0;
            MaxToS = -2.0; /* Bogus */
            CopyUnitV(S->PosN,Rhat);
            /* Aim at TDRS closest to Zenith */
            for(It=0;It<10;It++) {
               if (Tdrs[It].Exists) {
                  for(i=0;i<3;i++)
                     RelPosN[i] = Tdrs[It].PosN[i] - S->PosN[i];
                  UNITV(RelPosN);
                  ToS = VoV(RelPosN,Rhat);
                  if (ToS > MaxToS) {
                     MaxToS = ToS;
                     for(i=0;i<3;i++) CV->N[i] = RelPosN[i];
                  }
               }
            }
            break;
         default:
            break;
      }
}
/**********************************************************************/
void ThreeAxisAttitudeCommand(struct SCType *S)
{
      struct JointType *G;
      struct BodyType *B;
      struct FSWType *Fsw;
      struct CmdType *Cmd;
      struct CmdVecType *PV, *SV;
      double CRN[3][3],C[3][3],qln[4],Cdot[3][3];
      double PriVecBi[3],SecVecBi[3],PriVecGi[3],SecVecGi[3];
      double PriVecGo[3],SecVecGo[3],CGoGi[3][3];
      long Ig,Bi,i,j;

      Fsw = &S->FSW;

      Cmd = &Fsw->Cmd;
      PV = &Cmd->PriVec;
      SV = &Cmd->SecVec;

      switch (Cmd->Parm) {
         case PARM_EULER_ANGLES:
            A2C(Cmd->RotSeq,Cmd->Ang[0],Cmd->Ang[1],Cmd->Ang[2],C);
            if (Cmd->Frame == FRAME_L) C2Q(C,Cmd->qrl);
            else C2Q(C,Cmd->qrn);
         case PARM_QUATERNION:
            C2Q(S->CLN,qln);
            if (Cmd->Frame == FRAME_L) {
               QxQ(Cmd->qrl,qln,Cmd->qrn);
               QxV(Cmd->qrn,S->wln,Cmd->wrn);
            }
            break;
         case PARM_VECTORS:
            if (PV->Mode == CMD_TARGET) FindCmdVecN(S,PV);
            else if (PV->Frame == FRAME_N) {
               for(i=0;i<3;i++) PV->wn[i] = 0.0;
            }
            else if (PV->Frame == FRAME_L) {
               MTxV(S->CLN,PV->L,PV->N);
               for(i=0;i<3;i++) PV->wn[i] = S->wln[i];
            }
            else if (PV->Frame == FRAME_B) {
               MTxV(SC[PV->TrgSC].B[PV->TrgBody].CN,PV->T,PV->N);
               MTxV(SC[PV->TrgSC].B[PV->TrgBody].CN,
                  SC[PV->TrgSC].B[PV->TrgBody].wn,PV->wn);
            }

            if (SV->Mode == CMD_TARGET) FindCmdVecN(S,SV);
            else if (SV->Frame == FRAME_N) {
               for(i=0;i<3;i++) SV->wn[i] = 0.0;
            }
            else if (SV->Frame == FRAME_L) {
               MTxV(S->CLN,SV->L,SV->N);
               for(i=0;i<3;i++) SV->wn[i] = S->wln[i];
            }
            else if (SV->Frame == FRAME_B) {
               MTxV(SC[SV->TrgSC].B[SV->TrgBody].CN,SV->T,SV->N);
               MTxV(SC[SV->TrgSC].B[SV->TrgBody].CN,
                  SC[SV->TrgSC].B[SV->TrgBody].wn,SV->wn);
            }
            if (MAGV(PV->N) == 0.0 || MAGV(PV->R) == 0.0)
               printf("Warning: Primary Vector not defined for SC[%ld]\n",S->Tag);
            if (MAGV(SV->N) == 0.0 || MAGV(SV->R) == 0.0)
               printf("Warning: Secondary Vector not defined for SC[%ld]\n",S->Tag);
            TRIAD(PV->N,SV->N,PV->R,SV->R,CRN);
            C2Q(CRN,Cmd->qrn);
            for(i=0;i<3;i++) {
               for(j=0;j<3;j++) {
                  Cdot[i][j] = (CRN[i][j]-Cmd->OldCRN[i][j])/Fsw->DT;
               }
            }
            CDOT2W(CRN,Cdot,Cmd->wrn);
            for(i=0;i<3;i++) {
               for(j=0;j<3;j++) {
                  Cmd->OldCRN[i][j] = CRN[i][j];
               }
            }
            break;
         default:
            break;
      }

      for(Ig=0;Ig<S->Ng;Ig++) {
         G = &S->G[Ig];
         Bi = G->Bin;
         B = &S->B[Bi];
         Cmd = &Fsw->GimCmd[Ig];
         PV = &Cmd->PriVec;
         SV = &Cmd->SecVec;

         if (Cmd->Parm == PARM_VECTORS) {
            if (PV->Mode == CMD_TARGET) FindCmdVecN(S,PV);
            else if (PV->Frame == FRAME_L) MTxV(S->CLN,PV->L,PV->N);
            if (SV->Mode == CMD_TARGET) FindCmdVecN(S,SV);
            else if (SV->Frame == FRAME_L) MTxV(S->CLN,SV->L,SV->N);


            if (G->RotDOF == 3) {
               MxV(B->CN,PV->N,PriVecBi);
               MxV(B->CN,SV->N,SecVecBi);
               MxV(G->CGiBi,PriVecBi,PriVecGi);
               MxV(G->CGiBi,SecVecBi,SecVecGi);
               MTxV(G->CBoGo,PV->R,PriVecGo);
               MTxV(G->CBoGo,SV->R,SecVecGo);
               TRIAD(PriVecGi,SecVecGi,PriVecGo,SecVecGo,CGoGi);
               C2A(G->RotSeq,CGoGi,&Cmd->Ang[0],
                  &Cmd->Ang[1],&Cmd->Ang[2]);
            }
            else {
               MxV(B->CN,PV->N,PriVecBi);
               PointGimbalToTarget(G->RotSeq,G->CGiBi,G->CBoGo,PriVecBi,
                  PV->R,Cmd->Ang);
            }
         }
      }
}
/**********************************************************************/
void SpinnerCommand(struct SCType *S)
{
      struct FSWType *Fsw;
      struct CmdType *Cmd;
      struct CmdVecType *PV;
      double MagH;
      long i;
      
      Fsw = &S->FSW;
      Cmd = &Fsw->Cmd;
      PV = &Cmd->PriVec;
      
      if (PV->Frame != FRAME_N) {
         printf("SpinnerCommand requires that Primary Vector be fixed in N\n");
         exit(1);
      }
      
      FindCmdVecN(S,PV);
      for(i=0;i<3;i++) {
         Cmd->wrn[i] = PV->R[i]*Cmd->SpinRate;
      }
      MxV(S->I,Cmd->wrn,Cmd->Hvr);
      MagH = MAGV(Cmd->Hvr);
      for(i=0;i<3;i++) {
         Cmd->Hvn[i] = PV->N[i]*MagH;
      }
      
}
/**********************************************************************/
/* This function copies needed parameters from the SC structure to    */
/* the FSW structure.                                                 */
void InitFSW(struct SCType *S)
{
      long Ig,i,j;
      struct FSWType *FSW;

      FSW = &S->FSW;

      FSW->Init = 1;

      if (S->Ng > 0) {
         FSW->Ngim = S->Ng;
         FSW->Gim = (struct FswGimType *) calloc(S->Ng,sizeof(struct FswGimType));
         FSW->GimCmd = (struct CmdType *) calloc(S->Ng,sizeof(struct CmdType));
         for(Ig=0;Ig<FSW->Ngim;Ig++) {
            FSW->Gim[Ig].IsSpherical = S->G[Ig].IsSpherical;
            FSW->Gim[Ig].RotDOF = S->G[Ig].RotDOF;
            FSW->Gim[Ig].TrnDOF = S->G[Ig].TrnDOF;
            FSW->Gim[Ig].IsUnderActiveControl = TRUE;
            FSW->Gim[Ig].RotSeq = S->G[Ig].RotSeq;
            FSW->GimCmd[Ig].RotSeq = S->G[Ig].RotSeq;
            for(i=0;i<3;i++) {
               for(j=0;j<3;j++) {
                  FSW->Gim[Ig].CGiBi[i][j] = S->G[Ig].CGiBi[i][j];
                  FSW->Gim[Ig].CBoGo[i][j] = S->G[Ig].CBoGo[i][j];
               }
            }
         }
      }

      if (S->Nw > 0) {
         FSW->Nwhl = S->Nw;
         FSW->Hw = (double *) calloc(FSW->Nwhl,sizeof(double));
         FSW->Hwcmd = (double *) calloc(FSW->Nwhl,sizeof(double));
         FSW->Awhl = CreateMatrix(3,FSW->Nwhl);
         FSW->AwhlPlus = CreateMatrix(FSW->Nwhl,3);
         FSW->Twhlcmd = (double *) calloc(FSW->Nwhl,sizeof(double));
         for (i=0;i<S->Nw;i++) {
            for (j=0;j<3;j++) {
               FSW->Awhl[j][i] = S->Whl[i].A[j];
            }
         }
         if (S->Nw >= 3) PINVG(FSW->Awhl,FSW->AwhlPlus,3,FSW->Nwhl);
      }

      if (S->Nmtb > 0) {
         FSW->Nmtb = S->Nmtb;
         FSW->Amtb = CreateMatrix(3,FSW->Nmtb);
         FSW->AmtbPlus = CreateMatrix(FSW->Nmtb,3);
         FSW->Mmtbcmd = (double *) calloc(FSW->Nmtb,sizeof(double));
         FSW->Mmtbmax = (double *) calloc(FSW->Nmtb,sizeof(double));
         for (i=0;i<S->Nmtb;i++) {
            for (j=0;j<3;j++) {
               FSW->Amtb[j][i] = S->MTB[i].A[j];
            }
            FSW->Mmtbmax[i] = S->MTB[i].Mmax;
         }
         PINVG(FSW->Amtb,FSW->AmtbPlus,3,FSW->Nmtb);
      }

      if (S->Nthr > 0) {
         FSW->Nthr = S->Nthr;
         FSW->Athr = CreateMatrix(3,FSW->Nthr);
         FSW->AthrPlus = CreateMatrix(FSW->Nthr,3);
         FSW->Thrcmd = (double *) calloc(FSW->Nthr,sizeof(double));
         /* for(i=0;i<S->Nthr;i++) {
         **    for(j=0;j<3;j++) {
         **       FSW->Athr[j][i] = S->Thr[i].A[j];
         **    }
         ** }
         ** PINVG(FSW->Athr,FSW->AthrPlus,3,FSW->Nthr);
         */
      }

      /* Crude MOI estimate */
      for (i=0;i<3;i++) {
            FSW->MOI[i] = S->I[i][i];
      }
      FSW->mass = S->mass;

      /* For RampCoastGlide.  See Inp_Cmd.txt for easy modification. */
      FSW->RcgWC = 0.05*TwoPi;
      FSW->RcgAmax = 0.01;
      FSW->RcgVmax = 0.5*D2R;

}
/**********************************************************************/
/* The effective inertia for a gimbal is assumed to be the moment of  */
/* inertia of the appendage depending from the joint (that is, all    */
/* bodies for which that joint is in the JointPathTable) about that   */
/* joint, with all joints undeflected.                                */
void FindAppendageInertia(long Ig, struct SCType *S,double Iapp[3])
{
      struct DynType *D;
      struct JointType *G;
      double rho[3],CBoBi[3][3],Coi[3][3],Cr[3],rhog[3],Csofar[3][3];
      double CBoG[3][3],IBoG[3][3];
      long Ib,Jg,j,k;

      D = &S->Dyn;

      for(k=0;k<3;k++) Iapp[k] = 0.0;
      for (Ib=1;Ib<S->Nb;Ib++) {
         if (D->JointPathTable[Ib][Ig].InPath) {
            /* Build undeflected rho */
            Jg = S->B[Ib].Gin;
            for(k=0;k<3;k++) rho[k] = 0.0;
            for(j=0;j<3;j++) {
               for(k=0;k<3;k++) CBoBi[j][k] = 0.0;
               CBoBi[j][j] = 1.0;
            }
            while (Jg > Ig) {
               G = &S->G[Jg];
               MxM(G->CBoGo,G->CGiBi,Coi);
               for(k=0;k<3;k++) rho[k] -= G->rout[k];
               MTxV(Coi,rho,Cr);
               for(k=0;k<3;k++) rho[k] = Cr[k] + G->rin[k];
               for(j=0;j<3;j++) {
                  for(k=0;k<3;k++) Csofar[j][k] = CBoBi[j][k];
               }
               MxM(Csofar,Coi,CBoBi);
               Jg = S->B[G->Bin].Gin;
            }
            G = &S->G[Ig];
            for(k=0;k<3;k++) rho[k] -= G->rout[k];
            MTxV(G->CBoGo,rho,rhog);
            MTxM(CBoBi,G->CBoGo,CBoG);
            /* Parallel axis theorem */
            PARAXIS(S->B[Ib].I,CBoG,S->B[Ib].mass,rhog,IBoG);
            /* Accumulate */
            for(k=0;k<3;k++) Iapp[k] += IBoG[k][k];
         }
      }

}
/**********************************************************************/
/*  This simple control law is suitable for rapid prototyping.        */
void PrototypeFSW(struct SCType *S)
{
      struct FSWType *FSW;
      struct BodyType *B;
      struct CmdType *Cmd;
      double alpha[3],Iapp[3];
      double Hvnb[3],Herr[3],werr[3];
      long Ig,i,j;

      FSW = &S->FSW;
      
      if (FSW->Cmd.Parm == PARM_AXIS_SPIN) {
         if (FSW->Init) {
            FSW->Init = 0;
            FSW->DT = DTSIM;
            FSW->Kprec = 3.0E-2;
            FSW->Knute = 1.0;
         }
         
         SpinnerCommand(S);
         
         B = &S->B[0];
         Cmd = &FSW->Cmd;
         
         MxV(B->CN,Cmd->Hvn,Hvnb);
         
         for(i=0;i<3;i++) {
            Herr[i] = S->Hvb[i] - Hvnb[i];
            werr[i] = FSW->wbn[i] - Cmd->wrn[i];
            FSW->Tcmd[i] = -FSW->Knute*werr[i];
            if (MAGV(Herr) < 0.5*MAGV(Cmd->Hvn)) {
               FSW->Tcmd[i] -= FSW->Kprec*Herr[i];
            }
            FSW->IdealTrq[i] = Limit(FSW->Tcmd[i],-0.1,0.1); 
         }
         
      }
      else {
         if (FSW->Init) {
            FSW->Init = 0;
            FSW->DT = DTSIM;
            for(Ig=0;Ig<FSW->Ngim;Ig++) {
               FindAppendageInertia(Ig,S,Iapp);
               for(j=0;j<3;j++) {
                  FindPDGains(Iapp[j],0.05,1.0,
                     &FSW->Gim[Ig].RateGain[j],
                     &FSW->Gim[Ig].AngGain[j]);
                  FSW->Gim[Ig].MaxRate[j] = 0.5*D2R;
                  FSW->Gim[Ig].MaxTrq[j] = 0.1;
               }
            }
         }

         /* Find qrn, wrn and joint angle commands */
         ThreeAxisAttitudeCommand(S);

         /* Form attitude error signals */
         QxQT(FSW->qbn,FSW->Cmd.qrn,FSW->qbr);
         Q2AngleVec(FSW->qbr,FSW->therr);
         for(i=0;i<3;i++) FSW->werr[i] = FSW->wbn[i] - FSW->Cmd.wrn[i];

         /* Closed-loop attitude control */
         VectorRampCoastGlide(FSW->therr,FSW->werr,
            FSW->RcgWC,FSW->RcgAmax,FSW->RcgVmax,alpha);
         for(i=0;i<3;i++) FSW->IdealTrq[i] = FSW->MOI[i]*alpha[i];
      }

}
/**********************************************************************/
/*  SC_Spinner is a one-body spin-stabilized inertial pointer         */
void SpinnerFSW(struct SCType *S)
{

      double B1,B2,magb,magb2;
      double x=0.0;
      double y=0.0;
      double w1,w2,w3;
      double CyclicTorque,OrbPeriod,MaxPtgErr;
      static double I3,It;
      long i;
      struct FSWType *FSW;

      FSW = &S->FSW;

      if (FSW->Init) {
         FSW->Init = 0;
         FSW->DT = 0.1;
         FSW->Bold1 = 0.0;
         FSW->Bold2 = 0.0;
         FSW->xold = 0.0;
         FSW->yold = 0.0;

         CyclicTorque = 3.0E-4;
         MaxPtgErr = 1.0*D2R;
         OrbPeriod = TwoPi/sqrt(Orb[S->RefOrb].mu/(pow(Orb[S->RefOrb].SMA,3)));
         FindSpinnerGains(FSW->MOI[2],sqrt(FSW->MOI[0]*FSW->MOI[1]),
                          CyclicTorque,OrbPeriod,MaxPtgErr,
                          &FSW->wrn[2],&FSW->Knute,&FSW->Kprec);

         I3 = FSW->MOI[2];
         It = sqrt(FSW->MOI[0]*FSW->MOI[1]);
      }


      /* Sun-TAM Attitude Determination */
      if (FSW->SunValid) {
         TRIAD(FSW->svn,FSW->bvn,FSW->svb,FSW->bvb,FSW->CBN);
         for(i=0;i<3;i++) FSW->rvn[i] = FSW->svn[i];
         MxV(FSW->CBN,FSW->rvn,FSW->rvb);
         x = FSW->rvb[0];
         y = FSW->rvb[1];
      }

      /* Spin rate control */
      B1=FSW->bvb[0];
      B2=FSW->bvb[1];
      magb=sqrt(B1*B1+B2*B2);
      B1 /= magb;
      B2 /= magb;
      w3=(B1*FSW->Bold2-B2*FSW->Bold1)/FSW->DT-FSW->wrn[2];
/*      w3 = FSW->wbn[2]-FSW->wrn[2]; */
      FSW->Bold1=B1;
      FSW->Bold2=B2;
      FSW->Tcmd[2] = -FSW->Kprec*w3;

      /* Precession/nutation control */
      if (FSW->SunValid && fabs(w3) < 0.5*FSW->wrn[2]){
/*         w1 = FSW->wbn[0]; */
/*         w2 = FSW->wbn[1]; */
         w1= (y-FSW->yold)/FSW->DT + FSW->wrn[2]*x;
         w2=-(x-FSW->xold)/FSW->DT + FSW->wrn[2]*y;
         FSW->Tcmd[0] = -FSW->Knute*w1-FSW->Kprec*(It*w1-I3*FSW->wrn[2]*x);
         FSW->Tcmd[1] = -FSW->Knute*w2-FSW->Kprec*(It*w2-I3*FSW->wrn[2]*y);
         FSW->xold = x;
         FSW->yold = y;
      }
      else {
         FSW->Tcmd[0] = 0.0;
         FSW->Tcmd[1] = 0.0;
      }

      VxV(FSW->bvb,FSW->Tcmd,FSW->Mmtbcmd);
      magb2 = VoV(FSW->bvb,FSW->bvb);
      for (i=0;i<3;i++) {
         FSW->Mmtbcmd[i] /= magb2;
         FSW->Mmtbcmd[i] = Limit(FSW->Mmtbcmd[i],
                                    -FSW->Mmtbmax[i],
                                     FSW->Mmtbmax[i]);
/*         FSW->IdealFrc[i] = 0.0; */
/*         FSW->IdealTrq[i] = FSW->Tcmd[i]; */
      }

}
/**********************************************************************/
/* SC_EOS is a two-body momentum-biased Earth pointer                 */
void MomBiasFSW(struct SCType *S)
{

      double PitchRateError,PitchTcmd;
      double Tcmd[3],magb2;
      double Bdot[3];
      static double bvbold[3];
      double PitchRateCmd = -0.001059;
      double Kry = 5.0;
      double Kpy = 0.1;
      double Krx = 0.5;
      double Kpx = 0.05;
      double Kunl = 1.0E-4;
      double Kbdot = 3.0E8;
      double Hwcmd = -50.0;
      double Zvec[3] = {0.0,0.0,1.0};
      long i;
      struct FSWType *FSW;

      FSW = &S->FSW;

      if (FSW->Init) {
         FSW->Init = 0;
         FSW->DT = 0.1;

         FSW->Gim[0].IsSpherical = FALSE;
         FSW->Gim[0].RotDOF = 1;
         FSW->Gim[0].RotSeq = 231;
      }

      if (!FSW->EarthValid) {  /* Bdot Acquisition */

         FSW->Twhlcmd[0] = -Kry*(FSW->Hw[0]-Hwcmd);
         for(i=0;i<3;i++) {
            Bdot[i] = (FSW->bvb[i]-bvbold[i])/FSW->DT;
            bvbold[i] = FSW->bvb[i];
            FSW->Mmtbcmd[i] = -Kbdot*Bdot[i];

            FSW->GimCmd[0].Ang[i] = 0.0;
            FSW->GimCmd[0].Rate[i] = 0.0;

         }

      }
      else { /* Nadir Point */

         /* Pitch Loop */
         PitchRateError = FSW->wbn[1]-PitchRateCmd;
         PitchTcmd = -Kry*PitchRateError-Kpy*FSW->ESpitch;
         FSW->Twhlcmd[0] = -PitchTcmd-Kunl*(FSW->Hw[0]-Hwcmd);

         /* Roll-Yaw Loop */
         Tcmd[0] = -Krx*FSW->wbn[0]-Kpx*FSW->ESroll;
         Tcmd[2] = -0.5*Tcmd[0];

         /* Wheel Unload */
         Tcmd[1] = -Kunl*(FSW->Hw[0]-Hwcmd);

         /* M = BxT/B^2 */
         VxV(FSW->bvb,Tcmd,FSW->Mmtbcmd);
         magb2 = VoV(FSW->bvb,FSW->bvb);
         FSW->Mmtbcmd[0] /= magb2;
         FSW->Mmtbcmd[1] /= magb2;
         FSW->Mmtbcmd[2] /= magb2;

         /* Gimbal */
         FSW->GimCmd[0].Rate[0] = -PitchRateCmd;
         if (FSW->SunValid) {
            PointGimbalToTarget(FSW->Gim[0].RotSeq, FSW->Gim[0].CGiBi,
               FSW->Gim[0].CBoGo, FSW->svb, Zvec, FSW->GimCmd[0].Ang);
         }
         else {
            FSW->GimCmd[0].Ang[0] += PitchRateCmd*FSW->DT;
         }
         if (FSW->Gim[0].Ang[0] - FSW->GimCmd[0].Ang[0] > Pi)
            FSW->GimCmd[0].Ang[0] += TwoPi;
         if (FSW->Gim[0].Ang[0] - FSW->GimCmd[0].Ang[0] < -Pi)
            FSW->GimCmd[0].Ang[0] -= TwoPi;
      }
}
/**********************************************************************/
/* SC_Aura is a three-body three-axis stabilized S/C                */
void ThreeAxisFSW(struct SCType *S)
{
      double wln[3],CRN[3][3];
      double qrn[4],qbr[4],svr[3];
      double Herr[3],HxB[3];
      double Zvec[3] = {0.0,0.0,1.0};
      long i,j,Bout;
      struct FSWType *FSW;

      FSW = &S->FSW;

      if (FSW->Init) {
         FSW->Init = 0;
         FSW->DT = 0.1;
         FSW->Gim[0].IsSpherical = FALSE;
         FSW->Gim[0].RotDOF = 1;
         FSW->Gim[0].RotSeq = 231;
			for(j=0;j<3;j++) {
				FSW->GimCmd[0].Rate[j] = 0.0;
				FSW->GimCmd[0].Ang[j] = 0.0;
				FSW->Gim[0].MaxRate[j] = 0.2*D2R;
				FSW->Gim[0].MaxTrq[j] = 100.0;
				FindPDGains(S->B[1].I[1][1],0.02*TwoPi,1.0,
					&FSW->Gim[0].RateGain[j],&FSW->Gim[0].AngGain[j]);
			}

         for(i=0;i<3;i++) {
            FSW->wc[i] = 0.1;
            FSW->zc[i] = 0.7;
            FindPDGains(FSW->MOI[i],FSW->wc[i],FSW->zc[i],
                        &FSW->Kr[i],&FSW->Kp[i]);
         }
         FSW->Kunl = 1.0E6;
      }

      /* Find Attitude Command */
      FindCLN(FSW->PosN,FSW->VelN,CRN,wln);
      C2Q(CRN,qrn);
      MxV(CRN,FSW->svn,svr);

      /* Form Error Signals */
      QxQT(FSW->qbn,qrn,qbr);
      RECTIFYQ(qbr);

      /* PD Control */
      for(i=0;i<3;i++) {
         FSW->Tcmd[i] = -FSW->Kr[i]*FSW->wbn[i]-FSW->Kp[i]*(2.0*qbr[i]);
         FSW->Twhlcmd[i] = -FSW->Tcmd[i];
      }

      /* Momentum Management */
      for(i=0;i<3;i++) {
         Herr[i]=FSW->Hw[i]-FSW->Hwcmd[i];
      }
      VxV(Herr,FSW->bvb,HxB);
      for(i=0;i<3;i++) FSW->Mmtbcmd[i] = FSW->Kunl*HxB[i];

      /* Gimbals */
      FSW->GimCmd[0].Rate[0] = wln[1];
      if (FSW->SunValid) {
         PointGimbalToTarget(FSW->Gim[0].RotSeq, FSW->Gim[0].CGiBi,
               FSW->Gim[0].CBoGo, FSW->svb, Zvec,FSW->GimCmd[0].Ang);
      }
      else {
         FSW->GimCmd[0].Ang[0] += wln[1]*FSW->DT;
      }
		if (FSW->Gim[0].Ang[0] - FSW->GimCmd[0].Ang[0] > Pi)
			FSW->GimCmd[0].Ang[0] += TwoPi;
		if (FSW->Gim[0].Ang[0] - FSW->GimCmd[0].Ang[0] < -Pi)
			FSW->GimCmd[0].Ang[0] -= TwoPi;
}
/**********************************************************************/
void Thr3DOFFSW(struct SCType *S)
{

      double MOI[3] = {100.0,200.0,300.0};
      static double Kr[3],Kp[3];
      double CLN[3][3],qrn[4],qbr[4];
      double Fcmd[3]={0.0,0.0,0.0};
      double Tcmd[3],Thrcmd[12];
      double Aplus[12][6] = {{-0.25, 0.0 , 0.0 , 0.0  ,-0.125, 0.125},
                             { 0.0 ,-0.25, 0.0 , 0.125, 0.0  ,-0.125},
                             { 0.0 , 0.0 ,-0.25,-0.125, 0.125, 0.0  },
                             { 0.25, 0.0 , 0.0 , 0.0  , 0.125, 0.125},
                             { 0.0 , 0.25, 0.0 ,-0.125, 0.0  ,-0.125},
                             { 0.0 , 0.0 ,-0.25, 0.125,-0.125, 0.0  },
                             {-0.25, 0.0 , 0.0 , 0.0  , 0.125,-0.125},
                             { 0.0 , 0.25, 0.0 , 0.125, 0.0  , 0.125},
                             { 0.0 , 0.0 , 0.25,-0.125,-0.125, 0.0  },
                             { 0.25, 0.0 , 0.0 , 0.0  ,-0.125,-0.125},
                             { 0.0 ,-0.25, 0.0 ,-0.125, 0.0  , 0.125},
                             { 0.0 , 0.0 , 0.25, 0.125, 0.125, 0.0  }};
      double Null[12][3] = {{1.0,0.0,0.0},{0.0,1.0,0.0},{0.0,0.0,1.0},
                            {1.0,0.0,0.0},{0.0,1.0,0.0},{0.0,0.0,1.0},
                            {1.0,0.0,0.0},{0.0,1.0,0.0},{0.0,0.0,1.0},
                            {1.0,0.0,0.0},{0.0,1.0,0.0},{0.0,0.0,1.0}};
      double M, k[3],wln[3];
      long i,j;
      static long First = 1;
      struct FSWType *FSW;

      FSW = &S->FSW;

      if (First) {
         First = 0;
         for (i=0;i<3;i++) FindPDGains(MOI[i],0.05,1.0,&Kr[i],&Kp[i]);
      }

      /* Align to LVLH */
      FindCLN(FSW->PosN,FSW->VelN,CLN,wln);
      C2Q(CLN,qrn);
      QxQT(FSW->qbn,qrn,qbr);
      RECTIFYQ(qbr);
      for(i=0;i<3;i++) {
         Tcmd[i] = -Kr[i]*FSW->wbn[i]-Kp[i]*(2.0*qbr[i]);
         Tcmd[i] = Limit(Tcmd[i],-0.1,0.1);
      }

      /* Thruster Selection Logic */
      for(i=0;i<12;i++) {
         Thrcmd[i] = 0.0;
         for(j=0;j<3;j++) {
            Thrcmd[i] += Aplus[i][j]*Fcmd[j]+Aplus[i][j+3]*Tcmd[j];
         }
      }
      for(i=0;i<3;i++) {
         M=Thrcmd[i];
         k[i] = 0.0;
         if (M > Thrcmd[3+i]) M = Thrcmd[3+i];
         if (M > Thrcmd[6+i]) M = Thrcmd[6+i];
         if (M > Thrcmd[9+i]) M = Thrcmd[9+i];
         if (M < 0) k[i] = -M;
      }
      for(i=0;i<12;i++) {
         for(j=0;j<3;j++) Thrcmd[i] += Null[i][j]*k[j];
         if (Thrcmd[i] > 1.0) Thrcmd[i] = 1.0;
         if (Thrcmd[i] < 0.0) Thrcmd[i] = 0.0;
         FSW->Thrcmd[i] = Thrcmd[i];
      }

}
/**********************************************************************/
void Thr6DOFFSW(struct SCType *S)
{

      double Aplus[12][6] = {{-0.25, 0.0 , 0.0 , 0.0  ,-0.125, 0.125},
                             { 0.0 ,-0.25, 0.0 , 0.125, 0.0  ,-0.125},
                             { 0.0 , 0.0 ,-0.25,-0.125, 0.125, 0.0  },
                             { 0.25, 0.0 , 0.0 , 0.0  , 0.125, 0.125},
                             { 0.0 , 0.25, 0.0 ,-0.125, 0.0  ,-0.125},
                             { 0.0 , 0.0 ,-0.25, 0.125,-0.125, 0.0  },
                             {-0.25, 0.0 , 0.0 , 0.0  , 0.125,-0.125},
                             { 0.0 , 0.25, 0.0 , 0.125, 0.0  , 0.125},
                             { 0.0 , 0.0 , 0.25,-0.125,-0.125, 0.0  },
                             { 0.25, 0.0 , 0.0 , 0.0  ,-0.125,-0.125},
                             { 0.0 ,-0.25, 0.0 ,-0.125, 0.0  , 0.125},
                             { 0.0 , 0.0 , 0.25, 0.125, 0.125, 0.0  }};
      double Null[12][3] = {{1.0,0.0,0.0},{0.0,1.0,0.0},{0.0,0.0,1.0},
                            {1.0,0.0,0.0},{0.0,1.0,0.0},{0.0,0.0,1.0},
                            {1.0,0.0,0.0},{0.0,1.0,0.0},{0.0,0.0,1.0},
                            {1.0,0.0,0.0},{0.0,1.0,0.0},{0.0,0.0,1.0}};
      double M, k[3];
      long i,j;
      double Cerr[3][3],therr[3],Tcmd[3];
      double perr[3],F[3],Fcmd[3];
      double Thrcmd[12];
      struct FSWType *FSW;

      FSW = &S->FSW;

      if (FSW->Init) {
         FSW->Init = 0;

         for(i=0;i<3;i++) {
            FSW->wc[i] = 0.1;
            FSW->zc[i] = 1.0;
         }
         FSW->wct = 0.005;
         FSW->zct = 1.0;
         FSW->thsat = 0.1;
         FSW->psat = 5.0;
         if (S->Tag == 3) {
            A2C(123,90.0*D2R,0.0*D2R,0.0*D2R,FSW->CSFcmd);
            FSW->PosFcmd[0] = 0.0;
            FSW->PosFcmd[1] = 0.0;
            FSW->PosFcmd[2] = 0.0;
         }
         else {
            A2C(213,-90.0*D2R,0.0*D2R,0.0*D2R,FSW->CSFcmd);
            FSW->PosFcmd[0] = 50.0;
            FSW->PosFcmd[1] = 0.0;
            FSW->PosFcmd[2] = 0.0;
         }
         FSW->Tmax = 0.1;
         FSW->Fmax = 1.0;

         for (i=0;i<3;i++) FindPDGains(FSW->MOI[i],
             FSW->wc[i],FSW->zc[i],
            &FSW->Kr[i],&FSW->Kp[i]);
         FindPDGains(FSW->mass,FSW->wct,FSW->zct,
                     &FSW->Krt,&FSW->Kpt);
      }

/* .. Attitude Loop */

      MxMT(FSW->CSF,FSW->CSFcmd,Cerr);
      therr[0] = Cerr[1][2];
      therr[1] = Cerr[2][0];
      therr[2] = Cerr[0][1];
      for(i=0;i<3;i++) {
         therr[i] = Limit(therr[i],-FSW->thsat,FSW->thsat);
         Tcmd[i] = -FSW->Kr[i]*FSW->wbn[i]
                   -FSW->Kp[i]*therr[i];
         Tcmd[i] = Limit(Tcmd[i],-FSW->Tmax,FSW->Tmax);
      }


/* .. Translation Loop */

      for(i=0;i<3;i++) {
         perr[i] = FSW->PosF[i] - FSW->PosFcmd[i];
         perr[i] = Limit(perr[i],-FSW->psat,FSW->psat);
         /* F[i] = -FSW->Krt*FSW->vsf[i]
         **        -FSW->Kpt*perr[i]; */
         F[i] = 0.0;
      }
      MxV(FSW->CSF,F,Fcmd);
      for(i=0;i<3;i++)
         Fcmd[i] = Limit(Fcmd[i],-FSW->Fmax,FSW->Fmax);


/* .. Thruster Selection Logic */
      for(i=0;i<12;i++) {
         Thrcmd[i] = 0.0;
         for(j=0;j<3;j++) {
            Thrcmd[i] += Aplus[i][j]*Fcmd[j]+Aplus[i][j+3]*Tcmd[j];
         }
      }
      for(i=0;i<3;i++) {
         M=Thrcmd[i];
         k[i] = 0.0;
         if (M > Thrcmd[3+i]) M = Thrcmd[3+i];
         if (M > Thrcmd[6+i]) M = Thrcmd[6+i];
         if (M > Thrcmd[9+i]) M = Thrcmd[9+i];
         if (M < 0.0) k[i] = -M;
      }
      for(i=0;i<12;i++) {
         for(j=0;j<3;j++) Thrcmd[i] += Null[i][j]*k[j];
         if (Thrcmd[i] > 1.0) Thrcmd[i] = 1.0;
         if (Thrcmd[i] < 0.0) Thrcmd[i] = 0.0;
         FSW->Thrcmd[i] = Thrcmd[i];
      }

}
/**********************************************************************/
void MmsFSW(struct SCType *S)
{
      struct FSWType *FSW;

      FSW = &S->FSW;

      if (FSW->Init) {
         FSW->Init = 0;
         FSW->DT = 0.1;
         FSW->Ngim = 0;
      }
}
/**********************************************************************/
void SolarSailFSW(struct SCType *S)
{
      static FILE *FswFile;
      double acmdn[3],acmd[3];
      double Zvec[3] = {0.0,0.0,1.0};
      double NegZvec[3] = {0.0,0.0,-1.0};
      double Xvec[3] = {1.0,0.0,0.0};
      double CRN[3][3],CBR[3][3],qbr[4],qrn[4];
      double CmdTrq[3];
      long i,j;
      struct JointType *G;
      struct FSWType *FSW;

      FSW = &S->FSW;

      if (FSW->Init) {
         FSW->Init = 0;
         FSW->Ngim = S->Ng;
         for(i=0;i<FSW->Ngim;i++) {
            FSW->Gim[i].IsSpherical = FALSE;
            FSW->Gim[i].RotDOF = 1;
            FSW->Gim[i].RotSeq = 123;
            for(j=0;j<3;j++) {
               FSW->GimCmd[i].Rate[j] = 0.0;
               FSW->Gim[i].MaxRate[j] = 1.0*D2R;
            }
         }
         for(i=0;i<8;i++) {
            for(j=0;j<3;j++) {
               FindPDGains(53.5,0.05,2.0,
                  &FSW->Gim[i].RateGain[j],&FSW->Gim[i].AngGain[j]);
               FSW->Gim[i].MaxTrq[j] = 1.0;
            }
         }
         FswFile = FileOpen(InOutPath,"Fsw.42","w");
      }

/* .. Attitude control torque */
      TRIAD(S->svn,Zvec,NegZvec,Xvec,CRN);
      C2Q(CRN,qrn);
      MxMT(S->B[0].CN,CRN,CBR);
      C2Q(CBR,qbr);
      RECTIFYQ(qbr);
      for(i=0;i<3;i++) {
         CmdTrq[i] = Limit(-10.0*S->B[0].wn[i]-0.2*2.0*qbr[i],-1.0,1.0);
         FSW->Twhlcmd[i] = -CmdTrq[i];
      }

/* .. Desired thrust direction */
      /* For escape, thrust parallel to velocity */
      CopyUnitV(S->VelN,acmdn);
      MxV(S->B[0].CN,acmdn,acmd);

      if (VoV(FSW->svb,acmd) > 0.0) { /* Going upwind */
         /* Feather panels parallel to Sun */
         for(i=0;i<8;i++) {
            if (FSW->Gim[i].Ang[0] > 0.0)
               FSW->GimCmd[i].Ang[0] = 90.0*D2R;
            else
               FSW->GimCmd[i].Ang[0] = -90.0*D2R;
         }
      }
      else {
         /* Put panels to acmd */
         for(i=0;i<8;i++) {
            G = &S->G[i];
            PointGimbalToTarget(G->RotSeq,G->CGiBi,G->CBoGo,acmd,Zvec,FSW->GimCmd[i].Ang);
            /* FSW->GimAngCmd[i][0] = 0.0*D2R; */
         }
      }

      if (OutFlag) {
         fprintf(FswFile,"%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf\n",
            qrn[0],qrn[1],qrn[2],qrn[3],qbr[0],qbr[1],qbr[2],qbr[3],
            CmdTrq[0],CmdTrq[1],CmdTrq[2]);
      }
}
/**********************************************************************/
void EarthToMoonFSW(struct SCType *S)
{
      double Zvec[3] = {0.0,0.0,1.0};
      double Yvec[3] = {0.0,1.0,0.0};
      double CRN[3][3],CBR[3][3],qbr[4];
      long i;
      struct FSWType *FSW;

      double amax = 1.0;
      double mu,rc[3],vc[3],magrc,rchat[3],hc[3],hchat[3],thchat[3],n;
      double rhat[3],thhat[3],hhat[3];
      double CLN[3][3],x[3],v[3],magr;
      double h[3],ucmd,wcmd,vcirc;
      double PosR[3],VelR[3];
      double a[3];
      double Kv,Kr,Kvy,Krc,Kvc;
      double xmax,zmax;
      double u,w;
      double axcc,azcc;

      static FILE *Afile;

      FSW = &S->FSW;

      if (FSW->Init) {
         FSW->Init = 0;
         Afile = FileOpen(InOutPath,"Acc.42","w");
      }

/* .. Rendezvous with Luna */
      if (Orb[S->RefOrb].World == EARTH) {
         zmax = 500.0E6;
         xmax = 500.0E6;
         mu = World[EARTH].mu;
         for(i=0;i<3;i++) {
            rc[i] = World[LUNA].eph.PosN[i];
            vc[i] = World[LUNA].eph.VelN[i];
         }
      }
      else {
         zmax =   50.0E3;
         xmax = 100.0E3;
         mu = Orb[1].mu;
         for(i=0;i<3;i++) {
            rc[i] = Orb[1].PosN[i];
            vc[i] = Orb[1].VelN[i];
         }
      }
      for(i=0;i<3;i++) {
         PosR[i] = S->PosN[i]-rc[i];
         VelR[i] = S->VelN[i]-vc[i];
      }
      magrc = CopyUnitV(rc,rchat);
      VxV(rc,vc,hc);
      CopyUnitV(hc,hchat);
      VxV(hchat,rchat,thchat);
      n = sqrt(mu/magrc/magrc/magrc);
      for(i=0;i<3;i++) {
         CLN[0][i] = thchat[i];
         CLN[1][i] = -hchat[i];
         CLN[2][i] = -rchat[i];
      }
      FindPDGains(1.0,5.0*n,1.0,&Kv,&Kr);
      FindPDGains(1.0,5.0*n,0.5,&Kvc,&Krc);
      Kvy = 5.0*(2.0*n);

      /* SC state */
      magr = CopyUnitV(S->PosN,rhat);
      MAGV(S->VelN);
      VxV(S->PosN,S->VelN,h);
      CopyUnitV(h,hhat);
      VxV(hhat,rhat,thhat);

      vcirc = sqrt(mu/magr);
      RelRV2EHRV(magrc,n,CLN,PosR,VelR,x,v);

      u = VoV(S->VelN,thhat);
      w = -VoV(S->VelN,rhat);

/* .. Cancel out natural (nonlinear) coupling, then add in as desired */
      axcc = 2.0*(n+v[0]/magrc)*v[2];
      azcc = magr*(n+v[0]/magrc)*(n+v[0]/magrc) - mu/(magr*magr);
      if (magr < 0.2*magrc) {
         ucmd = 0.2*magrc/magr*vcirc;
         if (u < 0.5*vcirc) wcmd = 0.0;
         else wcmd = -2.0*vcirc;
         a[0] = -Kv*(u-ucmd);
         a[2] = -Kv*(w-wcmd);
      }
      else if (magr > 1.2*magrc) {
         ucmd = vcirc;
         if (u < 0.5*ucmd) wcmd = 0.0;
         else wcmd = vcirc*Limit(magr/magrc-1.0,0.0,3.0);
         a[0] = -Kv*(u-ucmd);
         a[2] = -Kv*(w-wcmd);
      }
      else {
         x[0] = Limit(x[0],-xmax,xmax);
         x[2] = Limit(x[2],-zmax,zmax);
         if (u < 0.5*vcirc) {
            a[0] = -Kv*(u-0.5*vcirc);
            a[2] = -Kv*v[2];
         }
         else {
            a[0] =  -Kv*v[0] + Kvc*v[2] -  Kr*x[0] + Krc*x[2] + axcc;
            a[2] = -Kvc*v[0] -  Kv*v[2] - Krc*x[0] -  Kr*x[2] + azcc;
         }
      }
      a[1] = (fabs(VoV(hchat,rhat)) < 0.1*fabs(VoV(hchat,thhat)) ? -Kvy*v[1] : 0.0);

      for(i=0;i<3;i++) {
         a[i] = Limit(a[i],-amax,amax);
      }
      MTxV(S->CLN,a,FSW->IdealFrc);
      for(i=0;i<3;i++) FSW->IdealFrc[i] *= S->mass;

/* .. Attitude Control */
      TRIAD(S->CLN[0],S->CLN[1],Zvec,Yvec,CRN);
      MxMT(S->B[0].CN,CRN,CBR);
      C2Q(CBR,qbr);
      RECTIFYQ(qbr);
      for(i=0;i<3;i++)
         FSW->IdealTrq[i] = FSW->MOI[i]*RampCoastGlide(2.0*qbr[i],S->B[0].wn[i],0.05*TwoPi,1.0E-2,1.0*D2R);

      if (OutFlag) {
         fprintf(Afile,"%lf %lf %lf  %lf %lf %lf  %lf %lf %lf  %lf %lf\n",
            x[0],x[1],x[2],
            v[0],v[1],v[2],
            a[0],a[1],a[2],
            axcc,azcc);
      }

}
/**********************************************************************/
void IssFSW(struct SCType *S)
{
      long Ig,i,j;
      struct FSWType *FSW;
      double Identity[3][3] = {{1.0,0.0,0.0},{0.0,1.0,0.0},{0.0,0.0,1.0}};
      double Zvec[3] = {0.0,0.0,1.0};
      double GimCmd[3];
      double svb[3];
      double Iapp[3];
      double r[3],rb[3],tvb[3],MinRoZ,RoZ;
      double CRL[3][3],CBL[3][3];

      FSW = &S->FSW;

      if (FSW->Init) {
         FSW->Init = 0;
         for(Ig=0;Ig<FSW->Ngim;Ig++) {
            for(j=0;j<3;j++) {
               FSW->GimCmd[Ig].Rate[j] = 0.0;
               FSW->GimCmd[Ig].Ang[j] = 0.0;
               FSW->Gim[Ig].MaxRate[j] = 0.5*D2R;
            }
            FindAppendageInertia(Ig,S,Iapp);
            for(j=0;j<FSW->Gim[Ig].RotDOF;j++) {
               FindPDGains(Iapp[j],0.02*TwoPi,1.0,
                  &FSW->Gim[Ig].RateGain[j],
                  &FSW->Gim[Ig].AngGain[j]);
               FSW->Gim[Ig].MaxTrq[j] = 0.1*FSW->Gim[Ig].AngGain[j];
            }
         }
         for(i=0;i<3;i++)
            FindPDGains(S->I[i][i],0.02*TwoPi,0.7,
               &FSW->Kr[i],&FSW->Kp[i]);
         FSW->Tmax = 0.1*MAX(FSW->Kp[0],MAX(FSW->Kp[1],FSW->Kp[2]));
      }

/* .. Hold LVLH */
      A2C(213,0.0*D2R,0.0,0.0,CRL);
      MxMT(S->B[0].CN,S->CLN,CBL);
      MxMT(CBL,CRL,FSW->CBR);
      /* XVV */
      C2A(321,FSW->CBR,&FSW->therr[2],&FSW->therr[1],&FSW->therr[0]);
      for(i=0;i<3;i++) {
         FSW->werr[i] = FSW->wbn[i] - S->wln[i];
         FSW->IdealTrq[i] = -FSW->Kp[i]*FSW->therr[i]
                            -FSW->Kr[i]*FSW->werr[i];
       }

/* .. Point Main Solar Arrays */
      MxV(S->B[0].CN,FSW->svn,svb);
      PointGimbalToTarget(21,Identity,Identity,svb,Zvec,GimCmd);
      GimCmd[0] += 5.0*D2R; /* Avoid lighting artifacts from on-edge polys */
      FSW->GimCmd[0].Ang[0] = GimCmd[0];
      FSW->GimCmd[1].Ang[0] = -GimCmd[0];
      FSW->GimCmd[0].Rate[0] = -S->wln[1];
      FSW->GimCmd[1].Rate[0] = S->wln[1];

      FSW->GimCmd[2].Ang[0] = -GimCmd[1];
      FSW->GimCmd[3].Ang[0] =  GimCmd[1];
      FSW->GimCmd[4].Ang[0] = -GimCmd[1];
      FSW->GimCmd[5].Ang[0] =  GimCmd[1];

      FSW->GimCmd[6].Ang[0] =  GimCmd[1];
      FSW->GimCmd[7].Ang[0] = -GimCmd[1];
      FSW->GimCmd[8].Ang[0] =  GimCmd[1];
      FSW->GimCmd[9].Ang[0] = -GimCmd[1];

/* .. Point SM Solar Array */
      FSW->GimCmd[12].Ang[0] = GimCmd[0];
      FSW->GimCmd[13].Ang[0] = -GimCmd[0];

/* .. Point Radiators */
      PointGimbalToTarget(1,Identity,Identity,svb,Zvec,GimCmd);
      FSW->GimCmd[10].Ang[0] =  GimCmd[0] + 90.0*D2R;
      FSW->GimCmd[11].Ang[0] =  GimCmd[0] + 90.0*D2R;

/* .. Point HGA */
      /* Select TDRS nearest Zenith */
      MinRoZ = 2.0;
      for(i=0;i<10;i++) {
         if (Tdrs[i].Exists) {
            for(j=0;j<3;j++) r[j] = Tdrs[i].PosN[j]-S->PosN[j];
            UNITV(r);
            MxV(S->B[0].CN,r,rb);
            RoZ = VoV(rb,Zvec);
            if (RoZ < MinRoZ) {
               MinRoZ = RoZ;
               for(j=0;j<3;j++) tvb[j] = rb[j];
            }
         }
      }
      PointGimbalToTarget(21,S->G[14].CGiBi,Identity,tvb,Zvec,GimCmd);

      FSW->GimCmd[14].Ang[0] = Limit(GimCmd[0],-120.0*D2R,120.0*D2R);
      FSW->GimCmd[14].Ang[1] = Limit(GimCmd[1],-65.0*D2R,65.0*D2R);

}
/**********************************************************************/
void ManualControlFSW(struct SCType *S)
{
      struct FSWType *FSW;
      long i;

      FSW = &S->FSW;

      if (FSW->Init) {
         FSW->Init = 0;
         for(i=0;i<3;i++) {
            FSW->wmax[i] = 0.3;
         }
      }

      for(i=0;i<3;i++) {
         FSW->IdealTrq[i] =
            RateControl(FSW->wbn[i]-FSW->wrn[i],0.1,1.0*TwoPi)*FSW->MOI[i];
      }
}
/**********************************************************************/
/* Put Chaser on Safety Ellipse                                       */
/* Align with LVLH                                                    */
/* Point Solar Arrays at Sun                                          */
/* Hold Arm in Neutral Position                                       */
void ArcEllipseMode(struct SCType *S)
{
      struct FSWType *FSW;
      double CBR[3][3],qbr[4];
      double Zvec[3] = {0.0,0.0,1.0};
      static double OldZ;
      long i;

      FSW = &S->FSW;

      /* Translation Control */
      /* Attitude Control */
      MxMT(S->B[0].CN,S->CLN,CBR);
      C2Q(CBR,qbr);
      RECTIFYQ(qbr);
      for(i=0;i<3;i++) {
         FSW->IdealTrq[i] = FSW->MOI[i]*
            RampCoastGlide(2.0*qbr[i],S->B[0].wn[i],0.05*TwoPi,1.0E-2,1.0*D2R);
      }
      /* Solar Arrays */
      for(i=0;i<2;i++) {
         PointGimbalToTarget(FSW->Gim[i].RotSeq, FSW->Gim[i].CGiBi,
            FSW->Gim[i].CBoGo, S->svb, Zvec,
            FSW->GimCmd[i].Ang);
      }
      /* Arm */
      FSW->GimCmd[2].Ang[1] = -30.0*D2R;
      FSW->GimCmd[3].Ang[0] = 60.0*D2R;
      FSW->GimCmd[4].Ang[0] = -30.0*D2R;

      if (S->PosEH[2] > 0.0 && OldZ < 0.0) {
         FSW->Mode = 2;  /* Approach */
         printf("Mode Transition to Approach\n");
      }
      OldZ = S->PosEH[2];
}
/**********************************************************************/
void ArcAcqAxis(struct SCType *S)
{
}
/**********************************************************************/
void ArcApproach(struct SCType *S)
{
      struct FSWType *FSW;
      struct OrbitType *O;
      double CBR[3][3],qbr[4];
      double Zvec[3] = {0.0,0.0,1.0};
      double ucmd;
      double Feh[3];
      double n,n2;
      long i;

      FSW = &S->FSW;
      O = &Orb[S->RefOrb];

      n = O->MeanMotion;
      n2 = n*n;

      if (S->PosEH[0] > -10.0) {
         ucmd = 0.01;
      }
      else {
         ucmd = 0.05;
      }

      /* Translation Control */
      /* Feedforward cancellation of EH coupling */
      Feh[0] = FSW->mass*(-2.0*n*S->VelEH[2]);
      Feh[1] = FSW->mass*(n2*S->PosEH[1]);
      Feh[2] = FSW->mass*(2.0*n*S->VelEH[0] - 3.0*n2*S->PosEH[2]);

      /* Add Closed-loop Control */

      Feh[0] += FSW->mass*
         RateControl(S->VelEH[0]-ucmd,1.0E-2,0.05*TwoPi);
      Feh[1] += FSW->mass*
         RampCoastGlide(S->PosEH[1],S->VelEH[1],0.05*TwoPi,1.0E-2,1.0);
      Feh[2] += FSW->mass*
         RampCoastGlide(S->PosEH[2],S->VelEH[2],0.05*TwoPi,1.0E-2,1.0);
      MTxV(S->CLN,Feh,FSW->IdealFrc);
      /* Attitude Control */
      MxMT(S->B[0].CN,S->CLN,CBR);
      C2Q(CBR,qbr);
      RECTIFYQ(qbr);
      for(i=0;i<3;i++) {
         FSW->IdealTrq[i] = FSW->MOI[i]*
            RampCoastGlide(2.0*qbr[i],S->B[0].wn[i],0.05*TwoPi,1.0E-2,1.0*D2R);
      }
      /* Solar Arrays */
      for(i=0;i<2;i++) {
         PointGimbalToTarget(FSW->Gim[i].RotSeq, FSW->Gim[i].CGiBi,
            FSW->Gim[i].CBoGo, S->svb, Zvec,
            FSW->GimCmd[i].Ang);
      }
      /* Arm */
      FSW->GimCmd[2].Ang[1] = -30.0*D2R;
      FSW->GimCmd[3].Ang[0] = 60.0*D2R;
      FSW->GimCmd[4].Ang[0] = -30.0*D2R;

      /* Mode Transition */
      if (S->PosEH[0] > -5.5) {
         FSW->Mode = 3;  /* Capture */
         printf("Mode Transition to Capture\n");
      }
}
/**********************************************************************/
void ArcCapture(struct SCType *S)
{
      struct FSWType *FSW;
      struct OrbitType *O;
      double CBR[3][3],qbr[4];
      double PosCmd[3] = {-5.0,0.0,0.0};
      double Feh[3];
      double n,n2;
      long i;

      FSW = &S->FSW;
      O = &Orb[S->RefOrb];

      n = O->MeanMotion;
      n2 = n*n;

      /* Translation Control */
      Feh[0] = FSW->mass*(-2.0*n*S->VelEH[2]);
      Feh[1] = FSW->mass*(n2*S->PosEH[1]);
      Feh[2] = FSW->mass*(2.0*n*S->VelEH[0] - 3.0*n2*S->PosEH[2]);
      for(i=0;i<3;i++) {
         Feh[i] += FSW->mass*
            RampCoastGlide(S->PosEH[i]-PosCmd[i],S->VelEH[i],0.05*TwoPi,1.0E-2,1.0);
      }
      MTxV(S->CLN,Feh,FSW->IdealFrc);
      /* Attitude Control */
      MxMT(S->B[0].CN,S->CLN,CBR);
      C2Q(CBR,qbr);
      RECTIFYQ(qbr);
      for(i=0;i<3;i++) {
         FSW->IdealTrq[i] = FSW->MOI[i]*
            RampCoastGlide(2.0*qbr[i],S->B[0].wn[i],0.05*TwoPi,1.0E-2,1.0*D2R);
      }
      /* Solar Arrays */
      FSW->GimCmd[0].Ang[0] = 0.0;
      FSW->GimCmd[1].Ang[0] = 0.0;
      /* Arm */
      FSW->GimCmd[2].Ang[1] = 0.0;
      FSW->GimCmd[3].Ang[0] = 0.0;
      FSW->GimCmd[4].Ang[0] = 0.0;

}
/**********************************************************************/
void ArcChaserFSW(struct SCType *S)
{
#define SAFETY_ELLIPSE 0
#define ACQ_APPROACH_AXIS 1
#define APPROACH 2
#define CAPTURE 3

      struct FSWType *FSW;
      struct FswGimType *FG;
      long Ig,i;

      FSW = &S->FSW;
      if (FSW->Init) {
         FSW->Init = 0;
         for(Ig=0;Ig<2;Ig++) {
            FG = &FSW->Gim[Ig];
            FG->RateGain[0] = 50.0;
            FG->AngGain[0] = 10.0;
            FG->MaxRate[0] = 1.0*D2R;
            FG->MaxTrq[0] = 10.0;
         }
         /* Shoulder */
         FG = &FSW->Gim[2];
         for(i=0;i<3;i++) {
            FG->RateGain[i] = 3.0;
            FG->AngGain[i] = 0.1;
            FG->MaxRate[i] = 0.1*D2R;
            FG->MaxTrq[i] = 0.1;
         }
         /* Elbow */
         FG = &FSW->Gim[3];
         for(i=0;i<3;i++) {
            FG->RateGain[i] = 3.0;
            FG->AngGain[i] = 0.1;
            FG->MaxRate[i] = 0.2*D2R;
            FG->MaxTrq[i] = 0.1;
         }
         /* Wrist */
         FG = &FSW->Gim[4];
         for(i=0;i<3;i++) {
            FG->RateGain[i] = 3.0;
            FG->AngGain[i] = 0.1;
            FG->MaxRate[i] = 0.1*D2R;
            FG->MaxTrq[i] = 0.1;
         }
      }

      switch(S->FSW.Mode) {
         case SAFETY_ELLIPSE:
            ArcEllipseMode(S);
         break;
         case ACQ_APPROACH_AXIS:
            ArcAcqAxis(S);
         break;
         case APPROACH:
            ArcApproach(S);
         break;
         case CAPTURE:
            ArcCapture(S);
         break;
      }

#undef SAFETY_ELLIPSE
#undef ACQ_APPROACH_AXIS
#undef APPROACH
#undef CAPTURE
}
/**********************************************************************/
/* Hexapod rover walks                                                */
void RoverFSW(struct SCType *S)
{

}
/**********************************************************************/
void CmgFSW(struct SCType *S)
{
      double wcmd[3],Tcmd[3],Axis[4][3],Gim[4][3],H[4],AngRateCmd[4];
      double Gain;
      long i,j;
      static FILE *outfile;
      static long First = 1;

      if (First) {
         First = 0;
         outfile = FileOpen(InOutPath,"CmgGain.42","w");
      }

      wcmd[0] = 5.0*D2R*sin(0.01*SimTime);
      wcmd[1] = 2.0*D2R*sin(0.02*SimTime);
      wcmd[2] = 1.0*D2R*sin(0.05*SimTime);
      /* wcmd[0] = 0.0;
      ** wcmd[1] = 0.0*D2R;
      ** wcmd[2] = 1.0*D2R; */

      for(i=0;i<3;i++) Tcmd[i] = -100.0*(S->B[0].wn[i]-wcmd[i]);

      /* Tcmd[0] = 5.0*sin(0.01*SimTime);
      ** Tcmd[1] = 2.0*sin(0.02*SimTime);
      ** Tcmd[2] = 1.0*sin(0.05*SimTime); */

      for(i=0;i<4;i++) {
         for(j=0;j<3;j++) {
            Axis[i][j] = S->CMG[i].A[j];
            Gim[i][j] = S->CMG[i].CGB[0][j];  /* Because Seq = 1 */
         }
         H[i] = S->CMG[i].H;
      }

      Gain = CMGLaw4x1DOF(Tcmd,Axis,Gim,H,AngRateCmd);

      for(i=0;i<4;i++)
         S->CMG[i].angrate[0] = AngRateCmd[i];

      if (OutFlag) {
         fprintf(outfile,"%lf\n",Gain);
      }
}
/**********************************************************************/
/* Put your custom controller here                                    */
void AdHocFSW(struct SCType *S)
{
      struct FSWType *FSW;
      long i;

      FSW = &S->FSW;

      if (FSW->Init) {
         FSW->Init = 0;
         FSW->DT = DTSIM;
         for(i=0;i<3;i++)
            FindPDGains(FSW->MOI[i],0.1,0.7,
                        &FSW->Kr[i],&FSW->Kp[i]);
      }

/* .. Form attitude error signals */
      RECTIFYQ(FSW->qbn);
      for(i=0;i<3;i++) {
         FSW->therr[i] = 2.0*FSW->qbn[i];
         FSW->werr[i] = FSW->wbn[i];
      }

/* .. Closed-loop attitude control */
      for(i=0;i<3;i++) {
         FSW->Tcmd[i] =
         -FSW->Kr[i]*FSW->werr[i]-FSW->Kp[i]*FSW->therr[i];
      }

      for(i=0;i<3;i++) FSW->IdealTrq[i] = FSW->Tcmd[i];
}
/**********************************************************************/
void CmdHandler(void)
{

}
/**********************************************************************/
long ModeTransition(long *TransitionRequested,long *Mode, long *RequestedMode,
                  long **TransitionIsAllowed)
{
     long TransitionExecuted = FALSE;

     if (*TransitionRequested &&
          TransitionIsAllowed[*Mode][*RequestedMode]) {
        *Mode = *RequestedMode;
        TransitionExecuted = TRUE;
     }
     return(TransitionExecuted);
}
/**********************************************************************/
/*  This function is called at the simulation rate.  Sub-sampling of  */
/*  control loops should be done on a case-by-case basis.             */
/*  Mode handling, command generation, error determination, feedback  */
/*  and failure detection and correction all fall within the scope of */
/*  this file.                                                        */
/**********************************************************************/
void FlightSoftWare(struct SCType *S)
{

      switch(S->FswTag){
         case PASSIVE_FSW:
            break;
         case PROTOTYPE_FSW:
            PrototypeFSW(S);
            break;
         case AD_HOC_FSW:
            AdHocFSW(S);
            break;
         case MANUAL_CONTROL_FSW:
            ManualControlFSW(S);
            break;
         case SPINNER_FSW:
            SpinnerFSW(S);
            break;
         case MOMBIAS_FSW:
            MomBiasFSW(S);
            break;
         case THREE_AXIS_FSW:
            ThreeAxisFSW(S);
            break;
         case IONCRUISER_FSW:
            EarthToMoonFSW(S);
            break;
         case ISS_FSW:
            IssFSW(S);
            break;
         case ARC_CHASER_FSW:
            ArcChaserFSW(S);
            break;
         case CMG_FSW:
            CmgFSW(S);
            break;
      }


}

/* #ifdef __cplusplus
** }
** #endif
*/


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

void AcFsw(struct AcType *AC);
void WriteToSocket(SOCKET Socket, char **Prefix, long Nprefix, long EchoEnabled);
void ReadFromSocket(SOCKET Socket, long EchoEnabled);


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
      struct CmdType *Cmd;
      struct CmdVecType *CV;
      double q[4],Ang[3],C[3][3],VecR[3],Vec[3],VecH[3];
      double RA,Dec;
      double Lng,Lat,Alt;
      double wc,amax,vmax;
      long RotSeq;
      char VecString[20],TargetString[20];
      double ThrPulseCmd;

      if (sscanf(CmdLine,"%lf SC[%ld] qrn = [%lf %lf %lf %lf]",
         CmdTime,&Isc,&q[0],&q[1],&q[2],&q[3]) == 6) {
         NewCmdProcessed = TRUE;
         Cmd = &SC[Isc].AC.Cmd;
         Cmd->Parm = PARM_QUATERNION;
         Cmd->Frame = FRAME_N;
         for(i=0;i<4;i++) Cmd->qrn[i] = q[i];
      }

      else if (sscanf(CmdLine,"%lf SC[%ld] qrl = [%lf %lf %lf %lf]",
         CmdTime,&Isc,&q[0],&q[1],&q[2],&q[3]) == 6) {
         NewCmdProcessed = TRUE;
         Cmd = &SC[Isc].AC.Cmd;
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
         Cmd = &SC[Isc].AC.Cmd;
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
         for(i=0;i<3;i++) SC[Isc].AC.G[Ig].Cmd.Ang[i] = Ang[i]*D2R;
      }

      else if (sscanf(CmdLine,"%lf Point SC[%ld].B[%ld] %s Vector [%lf %lf %lf] at RA = %lf deg, Dec = %lf deg",
         CmdTime,&Isc,&Ib,VecString,&VecR[0],&VecR[1],&VecR[2],&RA,&Dec) == 9) {
         NewCmdProcessed = TRUE;
         if (Ib == 0) {
            Cmd = &SC[Isc].AC.Cmd;
         }
         else {
            Ig = SC[Isc].B[Ib].Gin;
            Cmd = &SC[Isc].AC.G[Ig].Cmd;
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
         if (Ib == 0) {
            Cmd = &SC[Isc].AC.Cmd;
         }
         else {
            Ig = SC[Isc].B[Ib].Gin;
            Cmd = &SC[Isc].AC.G[Ig].Cmd;
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
         if (Ib == 0) {
            Cmd = &SC[Isc].AC.Cmd;
         }
         else {
            Ig = SC[Isc].B[Ib].Gin;
            Cmd = &SC[Isc].AC.G[Ig].Cmd;
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
         if (Ib == 0) {
            Cmd = &SC[Isc].AC.Cmd;
         }
         else {
            Ig = SC[Isc].B[Ib].Gin;
            Cmd = &SC[Isc].AC.G[Ig].Cmd;
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
         if (Ib == 0) {
            Cmd = &SC[Isc].AC.Cmd;
         }
         else {
            Ig = SC[Isc].B[Ib].Gin;
            Cmd = &SC[Isc].AC.G[Ig].Cmd;
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
         if (Ib == 0) {
            Cmd = &SC[Isc].AC.Cmd;
         }
         else {
            Ig = SC[Isc].B[Ib].Gin;
            Cmd = &SC[Isc].AC.G[Ig].Cmd;
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
         if (Ib == 0) {
            Cmd = &SC[Isc].AC.Cmd;
         }
         else {
            Ig = SC[Isc].B[Ib].Gin;
            Cmd = &SC[Isc].AC.G[Ig].Cmd;
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
         for(i=0;i<3;i++) CV->W[i] = 0.0;
      }

      else if (sscanf(CmdLine,"%lf Align SC[%ld].B[%ld] %s Vector [%lf %lf %lf] with SC[%ld].B[%ld] vector [%lf %lf %lf]",
         CmdTime,&Isc,&Ib,VecString,&VecR[0],&VecR[1],&VecR[2],&Isct,&Ibt,&Vec[0],&Vec[1],&Vec[2]) == 12) {
         NewCmdProcessed = TRUE;
         if (Ib == 0) {
            Cmd = &SC[Isc].AC.Cmd;
         }
         else {
            Ig = SC[Isc].B[Ib].Gin;
            Cmd = &SC[Isc].AC.G[Ig].Cmd;
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
         if (FrameChar == 'L') Frame = FRAME_L;
         else if (FrameChar == 'H') {
            Frame = FRAME_N;
            for(i=0;i<3;i++) VecH[i] = Vec[i];
            MxV(World[Orb[SC[Isc].RefOrb].World].CNH,VecH,Vec);
         }
         else Frame = FRAME_N;
         if (Ib == 0) {
            Cmd = &SC[Isc].AC.Cmd;
         }
         else {
            Ig = SC[Isc].B[Ib].Gin;
            Cmd = &SC[Isc].AC.G[Ig].Cmd;
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

      else if (sscanf(CmdLine,"%lf SC[%ld].Thr[%ld].PulseWidthCmd = %lf",
         CmdTime,&Isc,&Ithr,&ThrPulseCmd) == 4) {
         NewCmdProcessed = TRUE;
         SC[Isc].AC.Thr[Ithr].PulseWidthCmd = ThrPulseCmd;
      }

      else if (sscanf(CmdLine,"Event Eclipse Entry SC[%ld] qrl = [%lf %lf %lf %lf]",
         &Isc,&q[0],&q[1],&q[2],&q[3]) == 5) {
         *CmdTime = SimTime+DTSIM; /* Allows exiting while loop in CmdInterpreter */
         if (SC[Isc].Eclipse) { /* Will pend on this command until this condition is true */
            NewCmdProcessed = TRUE;
            Cmd = &SC[Isc].AC.Cmd;
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
            Cmd = &SC[Isc].AC.Cmd;
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
            Cmd = &SC[Isc].AC.Cmd;
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
            Cmd = &SC[Isc].AC.Cmd;
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
         SC[Isc].AC.PrototypeCtrl.wc = wc*TwoPi;
         SC[Isc].AC.PrototypeCtrl.amax = amax;
         SC[Isc].AC.PrototypeCtrl.vmax = vmax;
      }

      else if (sscanf(CmdLine,"%lf Spin SC[%ld] about Primary Vector at %lf deg/sec",
         CmdTime,&Isc,&wc) == 3) {
         NewCmdProcessed = TRUE;
         Cmd = &SC[Isc].AC.Cmd;
         
         Cmd->Parm = PARM_AXIS_SPIN;
         Cmd->SpinRate = wc*D2R;
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
      struct CmdType *Cmd;
      struct CmdVecType *PV, *SV;
      double CRN[3][3],C[3][3],qln[4],Cdot[3][3];
      double PriVecBi[3],SecVecBi[3],PriVecGi[3],SecVecGi[3];
      double PriVecGo[3],SecVecGo[3],CGoGi[3][3];
      long Ig,Bi,i,j;


      Cmd = &S->AC.Cmd;
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
               printf("Warning: Primary Vector not defined for SC[%ld]\n",S->ID);
            if (MAGV(SV->N) == 0.0 || MAGV(SV->R) == 0.0)
               printf("Warning: Secondary Vector not defined for SC[%ld]\n",S->ID);
            TRIAD(PV->N,SV->N,PV->R,SV->R,CRN);
            C2Q(CRN,Cmd->qrn);
            for(i=0;i<3;i++) {
               for(j=0;j<3;j++) {
                  Cdot[i][j] = (CRN[i][j]-Cmd->OldCRN[i][j])/S->AC.DT;
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
         Cmd = &S->AC.G[Ig].Cmd;
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
      struct CmdType *Cmd;
      struct CmdVecType *PV;
      double MagH;
      long i;
      
      Cmd = &S->AC.Cmd;
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
/* the AC structure.                                                 */
void InitAC(struct SCType *S)
{
      long Ib,Ig,i,j,k;
      struct AcType *AC;
      double **A,**Aplus;
      double r[3];

      AC = &S->AC;

      S->InitAC = 0;
      AC->Init = 1;
      
      AC->ID = S->ID;
      
      /* Time, Mass */
      AC->DT = S->FswSampleTime;
      AC->mass = S->mass;
      for (i=0;i<3;i++) {
         AC->cm[i] = S->cm[i];
         for(j=0;j<3;j++) {
            AC->MOI[i][j] = S->I[i][j];
         }
      }
      
      /* Bodies */
      AC->Nb = S->Nb;
      if (AC->Nb > 0) {
         AC->B = (struct AcBodyType *) calloc(AC->Nb,sizeof(struct AcBodyType));
         for (Ib=0;Ib<AC->Nb;Ib++) {
            AC->B[Ib].mass = S->B[Ib].mass;
            for(i=0;i<3;i++) {
               AC->B[Ib].cm[i] = S->B[Ib].cm[i];
               for(j=0;j<3;j++) {
                  AC->B[Ib].MOI[i][j] = S->B[Ib].I[i][j];
               }
            }
         }
      }
      
      /* Joints */
      AC->Ng = S->Ng;
      if (AC->Ng > 0) {
         AC->G = (struct AcJointType *) calloc(AC->Ng,sizeof(struct AcJointType));
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
      if (AC->Ngyro > 0) {
         AC->Gyro = (struct AcGyroType *) calloc(AC->Ngyro,sizeof(struct AcGyroType));
         for(i=0;i<S->Ngyro;i++) {
            for(j=0;j<3;j++) {
               AC->Gyro[i].Axis[j] = S->Gyro[i].Axis[j];
            }
         }
      }

      /* Magnetometer Axes */
      AC->Nmag = S->Nmag;
      if (AC->Nmag > 0) {
         AC->MAG = (struct AcMagnetometerType *) calloc(AC->Nmag,sizeof(struct AcMagnetometerType));
         for(i=0;i<S->Nmag;i++) {
            for(j=0;j<3;j++) {
               AC->MAG[i].Axis[j] = S->MAG[i].Axis[j];
            }
         }
      }

      /* Coarse Sun Sensors */
      AC->Ncss = S->Ncss;
      if (AC->Ncss > 0) {
         AC->CSS = (struct AcCssType *) calloc(AC->Ncss,sizeof(struct AcCssType));
         for(i=0;i<S->Ncss;i++) {
            AC->CSS[i].Body = S->CSS[i].Body;
            for(j=0;j<3;j++) AC->CSS[i].Axis[j] = S->CSS[i].Axis[j];
            AC->CSS[i].Scale = S->CSS[i].Scale;
         }
      }
      
      /* Fine Sun Sensors */
      AC->Nfss = S->Nfss;
      if (AC->Nfss > 0) {
         AC->FSS = (struct AcFssType *) calloc(AC->Nfss,sizeof(struct AcFssType));
         for(k=0;k<S->Nfss;k++) {
            for(i=0;i<3;i++) {
               for(j=0;j<3;j++) AC->FSS[k].CB[i][j] = S->FSS[k].CB[i][j];
            }
            for(i=0;i<4;i++) AC->FSS[k].qb[i] = S->FSS[k].qb[i];
         }
      }

      /* Star Trackers */
      AC->Nst = S->Nst;
      if (AC->Nst > 0) {
         AC->ST = (struct AcStarTrackerType *) calloc(AC->Nst,sizeof(struct AcStarTrackerType));
         for(k=0;k<S->Nst;k++) {
            for(i=0;i<3;i++) {
               for(j=0;j<3;j++) AC->ST[k].CB[i][j] = S->ST[k].CB[i][j];
            }
            for(i=0;i<4;i++) AC->ST[k].qb[i] = S->ST[k].qb[i];
         }
      }

      /* GPS */
      AC->Ngps = S->Ngps;
      if (AC->Ngps > 0) {
         AC->GPS = (struct AcGpsType *) calloc(AC->Ngps,sizeof(struct AcGpsType)); 
      }     
      
      /* Accelerometer Axes */
      AC->Nacc = S->Nacc;
      if (AC->Nacc > 0) {
         AC->Accel = (struct AcAccelType *) calloc(AC->Nacc,sizeof(struct AcAccelType));
         for(i=0;i<S->Nacc;i++) {
            for(j=0;j<3;j++) {
               AC->Accel[i].Axis[j] = S->Accel[i].Axis[j];
            }
         }
      }

      /* Wheels */
      AC->Nwhl = S->Nw;
      if (AC->Nwhl > 0) {
         AC->Whl = (struct AcWhlType *) calloc(AC->Nwhl,sizeof(struct AcWhlType));
         A = CreateMatrix(3,AC->Nwhl);
         Aplus = CreateMatrix(AC->Nwhl,3);
         for (i=0;i<S->Nw;i++) {
            AC->Whl[i].Body = S->Whl[i].Body;
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
      if (AC->Nmtb > 0) {
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
      if (AC->Nthr > 0) {
         AC->Thr = (struct AcThrType *) calloc(AC->Nthr,sizeof(struct AcThrType));
         for(i=0;i<S->Nthr;i++) {
            AC->Thr[i].Body = S->Thr[i].Body;
            AC->Thr[i].Fmax = S->Thr[i].Fmax;
            for(j=0;j<3;j++) {
               AC->Thr[i].Axis[j] = S->Thr[i].A[j];
               AC->Thr[i].PosB[j] = S->Thr[i].PosB[j];
               r[j] = AC->Thr[i].PosB[j] - AC->cm[j];
            }
            VxV(r,AC->Thr[i].Axis,AC->Thr[i].rxA);
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
      AC->ThrCtrl.Init = 1;
      AC->CfsCtrl.Init = 1;
      AC->ThrSteerCtrl.Init = 1;
      
      AC->PrototypeCtrl.wc = 0.05*TwoPi;
      AC->PrototypeCtrl.amax = 0.01;
      AC->PrototypeCtrl.vmax = 0.5*D2R;
      
      /* Initialize variables to avoid divide-by-zero before first sensor measurements */
      AC->qbn[3] = 1.0;
      AC->svb[0] = 1.0;
      AC->bvb[0] = 1.0E-4;
      
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
               for(k=0;k<3;k++) rho[k] -= G->ro[k];
               MTxV(Coi,rho,Cr);
               for(k=0;k<3;k++) rho[k] = Cr[k] + G->ri[k];
               for(j=0;j<3;j++) {
                  for(k=0;k<3;k++) Csofar[j][k] = CBoBi[j][k];
               }
               MxM(Csofar,Coi,CBoBi);
               Jg = S->B[G->Bin].Gin;
            }
            G = &S->G[Ig];
            for(k=0;k<3;k++) rho[k] -= G->ro[k];
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
void MapCmdsToActuators(struct SCType *S)
{
      struct AcType *AC;
      long i,Iw,Im,It;

      AC = &S->AC;
      
      if (S->GainAndDelayActive) {
         for(i=0;i<3;i++) {
            S->IdealAct[i].Fcmd = Delay(S->IdealAct[i].FrcDelay,S->LoopGain*AC->IdealFrc[i]);
            S->IdealAct[i].Tcmd = Delay(S->IdealAct[i].TrqDelay,S->LoopGain*AC->IdealTrq[i]);
         }
            
         for(Iw=0;Iw<AC->Nwhl;Iw++) {
            S->Whl[Iw].Tcmd = Delay(S->Whl[Iw].Delay,S->LoopGain*AC->Whl[Iw].Tcmd);
         }
         for(Im=0;Im<AC->Nmtb;Im++) {
            S->MTB[Im].Mcmd = Delay(S->MTB[Im].Delay,S->LoopGain*AC->MTB[Im].Mcmd);
         }
         for(It=0;It<AC->Nthr;It++) {
            S->Thr[It].PulseWidthCmd = Delay(S->Thr[It].Delay,S->LoopGain*AC->Thr[It].PulseWidthCmd);
         }         
      }
      else {      
         for(i=0;i<3;i++) {
            S->IdealAct[i].Fcmd = AC->IdealFrc[i];
            S->IdealAct[i].Tcmd = AC->IdealTrq[i];
         }
      
         for(Iw=0;Iw<AC->Nwhl;Iw++) {
            S->Whl[Iw].Tcmd = AC->Whl[Iw].Tcmd;
         }
         for(Im=0;Im<AC->Nmtb;Im++) {
            S->MTB[Im].Mcmd = AC->MTB[Im].Mcmd;
         }
         for(It=0;It<AC->Nthr;It++) {
            S->Thr[It].PulseWidthCmd = AC->Thr[It].PulseWidthCmd;
         }
      } 
              
}
/**********************************************************************/
/*  This simple control law is suitable for rapid prototyping.        */
void PrototypeFSW(struct SCType *S)
{
      struct AcType *AC;
      struct AcPrototypeCtrlType *C;
      struct BodyType *B;
      struct CmdType *Cmd;
      double alpha[3],Iapp[3];
      double Hvnb[3],Herr[3],werr[3];
      double qbr[4];
      long Ig,i,j;

      AC = &S->AC;
      C = &AC->PrototypeCtrl;
      Cmd = &AC->Cmd;
            
      if (Cmd->Parm == PARM_AXIS_SPIN) {
         if (C->Init) {
            C->Init = 0;
            C->Kprec = 3.0E-2;
            C->Knute = 1.0;
         }
         
         SpinnerCommand(S);
         
         B = &S->B[0];
         
         MxV(B->CN,Cmd->Hvn,Hvnb);
         
         for(i=0;i<3;i++) {
            Herr[i] = S->Hvb[i] - Hvnb[i];
            werr[i] = AC->wbn[i] - Cmd->wrn[i];
            C->Tcmd[i] = -C->Knute*werr[i];
            if (MAGV(Herr) < 0.5*MAGV(Cmd->Hvn)) {
               C->Tcmd[i] -= C->Kprec*Herr[i];
            }
            AC->IdealTrq[i] = Limit(C->Tcmd[i],-0.1,0.1); 
         }
         
      }
      else {
         if (C->Init) {
            C->Init = 0;
            
            for(Ig=0;Ig<AC->Ng;Ig++) {
               FindAppendageInertia(Ig,S,Iapp);
               for(j=0;j<3;j++) {
                  FindPDGains(Iapp[j],0.05,1.0,
                     &AC->G[Ig].AngRateGain[j],
                     &AC->G[Ig].AngGain[j]);
                  AC->G[Ig].MaxAngRate[j] = 0.5*D2R;
                  AC->G[Ig].MaxTrq[j] = 0.1;
               }
            }
         }

         /* Find qrn, wrn and joint angle commands */
         ThreeAxisAttitudeCommand(S);

         /* Form attitude error signals */
         QxQT(AC->qbn,Cmd->qrn,qbr);
         Q2AngleVec(qbr,C->therr);
         for(i=0;i<3;i++) C->werr[i] = AC->wbn[i] - Cmd->wrn[i];

         /* Closed-loop attitude control */
         VectorRampCoastGlide(C->therr,C->werr,
            C->wc,C->amax,C->vmax,alpha);
         for(i=0;i<3;i++) AC->IdealTrq[i] = AC->MOI[i][i]*alpha[i];
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
      long i,Imtb;
      struct AcType *AC;
      struct AcSpinnerCtrlType *C;
      struct AcMtbType *M;

      AC = &S->AC;
      C = &AC->SpinnerCtrl;

      if (AC->Init) {
         AC->Init = 0;
         AC->DT = 0.1;
         C->Bold1 = 0.0;
         C->Bold2 = 0.0;
         C->xold = 0.0;
         C->yold = 0.0;

         CyclicTorque = 3.0E-4;
         MaxPtgErr = 1.0*D2R;
         OrbPeriod = TwoPi/sqrt(Orb[S->RefOrb].mu/(pow(Orb[S->RefOrb].SMA,3)));
         FindSpinnerGains(AC->MOI[2][2],sqrt(AC->MOI[0][0]*AC->MOI[1][1]),
                          CyclicTorque,OrbPeriod,MaxPtgErr,
                          &C->SpinRate,&C->Knute,&C->Kprec);

         C->Ispin = AC->MOI[2][2];
         C->Itrans = sqrt(AC->MOI[0][0]*AC->MOI[1][1]);
      }

      /* Sun-TAM Attitude Determination */
      if (AC->SunValid) {
         TRIAD(AC->svn,AC->bvn,AC->svb,AC->bvb,AC->CBN);
         for(i=0;i<3;i++) C->rvn[i] = AC->svn[i];
         MxV(AC->CBN,C->rvn,C->rvb);
         x = C->rvb[0];
         y = C->rvb[1];
      }

      /* Spin rate control */
      B1=AC->bvb[0];
      B2=AC->bvb[1];
      magb=sqrt(B1*B1+B2*B2);
      B1 /= magb;
      B2 /= magb;
      w3=(B1*C->Bold2-B2*C->Bold1)/AC->DT - C->SpinRate;
/*      w3 = AC->wbn[2] - C->SpinRate; */
      C->Bold1=B1;
      C->Bold2=B2;
      C->Tcmd[2] = -C->Kprec*w3;

      /* Precession/nutation control */
      if (AC->SunValid && fabs(w3) < 0.5*C->SpinRate){
/*         w1 = AC->wbn[0]; */
/*         w2 = AC->wbn[1]; */
         w1= (y-C->yold)/AC->DT + C->SpinRate*x;
         w2=-(x-C->xold)/AC->DT + C->SpinRate*y;
         C->Tcmd[0] = -C->Knute*w1-C->Kprec*(C->Itrans*w1-C->Ispin*C->SpinRate*x);
         C->Tcmd[1] = -C->Knute*w2-C->Kprec*(C->Itrans*w2-C->Ispin*C->SpinRate*y);
         C->xold = x;
         C->yold = y;
      }
      else {
         C->Tcmd[0] = 0.0;
         C->Tcmd[1] = 0.0;
      }

      VxV(AC->bvb,C->Tcmd,C->Mcmd);
      magb2 = VoV(AC->bvb,AC->bvb);
      for (i=0;i<3;i++) C->Mcmd[i] /= magb2;
      
      for(Imtb=0;Imtb<AC->Nmtb;Imtb++) {
         M = &AC->MTB[Imtb];
         M->Mcmd = VoV(M->DistVec,C->Mcmd);
         M->Mcmd = Limit(M->Mcmd,-M->Mmax,M->Mmax);
      }
/*      for(i=0;i<3;i++) { */
/*         AC->IdealFrc[i] = 0.0; */
/*         AC->IdealTrq[i] = M->Tcmd[i]; */
/*      } */

}
/**********************************************************************/
/* Notional two-body momentum-biased Earth pointer                    */
void MomBiasFSW(struct SCType *S)
{

      double PitchRateError,PitchTcmd;
      double Tcmd[3],magb2,Mcmd[3];
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
      struct AcType *AC;
      struct AcMomBiasCtrlType *C;

      AC = &S->AC;
      C = &AC->MomBiasCtrl;
      
      if (C->Init) {
         C->Init = 0;
      }

      if (!AC->ES.Valid) {  /* Bdot Acquisition */

         AC->Whl[0].Tcmd = -Kry*(AC->Whl[0].H-Hwcmd);
         for(i=0;i<3;i++) {
            Bdot[i] = (AC->bvb[i]-bvbold[i])/AC->DT;
            bvbold[i] = AC->bvb[i];
            AC->MTB[i].Mcmd = -Kbdot*Bdot[i];

            AC->G[0].Cmd.Ang[i] = 0.0;
            AC->G[0].Cmd.AngRate[i] = 0.0;
         }

      }
      else { /* Nadir Point */

         /* Pitch Loop */
         PitchRateError = AC->wbn[1]-PitchRateCmd;
         PitchTcmd = -Kry*PitchRateError-Kpy*AC->ES.Pitch;
         AC->Whl[0].Tcmd = -PitchTcmd-Kunl*(AC->Whl[0].H-Hwcmd);

         /* Roll-Yaw Loop */
         Tcmd[0] = -Krx*AC->wbn[0]-Kpx*AC->ES.Roll;
         Tcmd[2] = -0.5*Tcmd[0];

         /* Wheel Unload */
         Tcmd[1] = -Kunl*(AC->Whl[0].H-Hwcmd);

         /* M = BxT/B^2 */
         VxV(AC->bvb,Tcmd,Mcmd);
         magb2 = VoV(AC->bvb,AC->bvb);
         Mcmd[0] /= magb2;
         Mcmd[1] /= magb2;
         Mcmd[2] /= magb2;
         for(i=0;i<3;i++) AC->MTB[i].Mcmd = Mcmd[i];

         /* Solar Array Gimbal */
         AC->G[0].Cmd.AngRate[0] = -PitchRateCmd;
         if (AC->SunValid) {
            PointGimbalToTarget(AC->G[0].RotSeq, AC->G[0].CGiBi,
               AC->G[0].CBoGo, AC->svb, Zvec, AC->G[0].Cmd.Ang);
         }
         else {
            AC->G[0].Cmd.Ang[0] += PitchRateCmd*AC->DT;
         }
         if (AC->G[0].Ang[0] - AC->G[0].Cmd.Ang[0] > Pi)
            AC->G[0].Cmd.Ang[0] += TwoPi;
         if (AC->G[0].Ang[0] - AC->G[0].Cmd.Ang[0] < -Pi)
            AC->G[0].Cmd.Ang[0] -= TwoPi;
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
      long i,j;
      struct AcType *AC;
      struct AcThreeAxisCtrlType *C;
      
      AC = &S->AC;
      C = &AC->ThreeAxisCtrl;

      if (C->Init) {
         C->Init = 0;
         for(j=0;j<3;j++) {
            AC->G[0].Cmd.AngRate[j] = 0.0;
            AC->G[0].Cmd.Ang[j] = 0.0;
            AC->G[0].MaxAngRate[j] = 0.2*D2R;
            AC->G[0].MaxTrq[j] = 100.0;
            FindPDGains(S->B[1].I[1][1],0.02*TwoPi,1.0,
               &AC->G[0].AngRateGain[j],&AC->G[0].AngGain[j]);
         }

         for(i=0;i<3;i++) {
            FindPDGains(AC->MOI[i][i],0.1,0.7,
                        &C->Kr[i],&C->Kp[i]);
            C->Hwcmd[i] = 0.0;
         }
         C->Kunl = 1.0E6;
      }

      /* Find Attitude Command */
      FindCLN(AC->PosN,AC->VelN,CRN,wln);
      C2Q(CRN,qrn);
      MxV(CRN,AC->svn,svr);

      /* Form Error Signals */
      QxQT(AC->qbn,qrn,qbr);
      RECTIFYQ(qbr);

      /* PD Control */
      for(i=0;i<3;i++) {
         C->Tcmd[i] = -C->Kr[i]*AC->wbn[i]-C->Kp[i]*(2.0*qbr[i]);
         AC->Whl[i].Tcmd = -C->Tcmd[i];
      }

      /* Momentum Management */
      for(i=0;i<3;i++) {
         Herr[i] = AC->Whl[i].H - C->Hwcmd[i];
      }
      VxV(Herr,AC->bvb,HxB);
      for(i=0;i<3;i++) AC->MTB[i].Mcmd = C->Kunl*HxB[i];

      /* Solar Array Gimbal */
      AC->G[0].Cmd.AngRate[0] = wln[1];
      if (AC->SunValid) {
         PointGimbalToTarget(AC->G[0].RotSeq, AC->G[0].CGiBi,
               AC->G[0].CBoGo, AC->svb, Zvec,AC->G[0].Cmd.Ang);
      }
      else {
         AC->G[0].Cmd.Ang[0] += wln[1]*AC->DT;
      }
      if (AC->G[0].Ang[0] - AC->G[0].Cmd.Ang[0] > Pi)
         AC->G[0].Cmd.Ang[0] += TwoPi;
      if (AC->G[0].Ang[0] - AC->G[0].Cmd.Ang[0] < -Pi)
         AC->G[0].Cmd.Ang[0] -= TwoPi;
}
/**********************************************************************/
void IssFSW(struct SCType *S)
{
      long Ig,i,j;
      struct AcType *AC;
      struct AcIssCtrlType *C;
      double Identity[3][3] = {{1.0,0.0,0.0},{0.0,1.0,0.0},{0.0,0.0,1.0}};
      double Zvec[3] = {0.0,0.0,1.0};
      double GimCmd[3];
      double svb[3];
      double Iapp[3];
      double r[3],rb[3],tvb[3],MinRoZ,RoZ;
      double CRL[3][3],CBL[3][3],CBR[3][3];

      AC = &S->AC;
      C = &AC->IssCtrl;

      if (C->Init) {
         C->Init = 0;
         for(Ig=0;Ig<AC->Ng;Ig++) {
            for(j=0;j<3;j++) {
               AC->G[Ig].Cmd.AngRate[j] = 0.0;
               AC->G[Ig].Cmd.Ang[j] = 0.0;
               AC->G[Ig].MaxAngRate[j] = 0.5*D2R;
            }
            FindAppendageInertia(Ig,S,Iapp);
            for(j=0;j<AC->G[Ig].RotDOF;j++) {
               FindPDGains(Iapp[j],0.02*TwoPi,1.0,
                  &AC->G[Ig].AngRateGain[j],
                  &AC->G[Ig].AngGain[j]);
               AC->G[Ig].MaxTrq[j] = 0.1*AC->G[Ig].AngGain[j];
            }
         }
         for(i=0;i<3;i++)
            FindPDGains(S->I[i][i],0.02*TwoPi,0.7,
               &C->Kr[i],&C->Kp[i]);
         C->Tmax = 0.1*MAX(C->Kp[0],MAX(C->Kp[1],C->Kp[2]));
      }

/* .. Hold LVLH */
      A2C(213,0.0*D2R,0.0,0.0,CRL);
      MxMT(S->B[0].CN,S->CLN,CBL);
      MxMT(CBL,CRL,CBR);
      /* XVV */
      C2A(321,CBR,&C->therr[2],&C->therr[1],&C->therr[0]);
      for(i=0;i<3;i++) {
         C->werr[i] = AC->wbn[i] - S->wln[i];
         AC->IdealTrq[i] = -C->Kp[i]*C->therr[i]
                            -C->Kr[i]*C->werr[i];
       }

/* .. Point Main Solar Arrays */
      MxV(S->B[0].CN,AC->svn,svb);
      PointGimbalToTarget(21,Identity,Identity,svb,Zvec,GimCmd);
      GimCmd[0] += 5.0*D2R; /* Avoid lighting artifacts from on-edge polys */
      AC->G[0].Cmd.Ang[0] = GimCmd[0];
      AC->G[1].Cmd.Ang[0] = -GimCmd[0];
      AC->G[0].Cmd.AngRate[0] = -S->wln[1];
      AC->G[1].Cmd.AngRate[0] = S->wln[1];

      AC->G[2].Cmd.Ang[0] = -GimCmd[1];
      AC->G[3].Cmd.Ang[0] =  GimCmd[1];
      AC->G[4].Cmd.Ang[0] = -GimCmd[1];
      AC->G[5].Cmd.Ang[0] =  GimCmd[1];

      AC->G[6].Cmd.Ang[0] =  GimCmd[1];
      AC->G[7].Cmd.Ang[0] = -GimCmd[1];
      AC->G[8].Cmd.Ang[0] =  GimCmd[1];
      AC->G[9].Cmd.Ang[0] = -GimCmd[1];

/* .. Point SM Solar Array */
      AC->G[12].Cmd.Ang[0] = GimCmd[0];
      AC->G[13].Cmd.Ang[0] = -GimCmd[0];

/* .. Point Radiators */
      PointGimbalToTarget(1,Identity,Identity,svb,Zvec,GimCmd);
      AC->G[10].Cmd.Ang[0] =  GimCmd[0] + 90.0*D2R;
      AC->G[11].Cmd.Ang[0] =  GimCmd[0] + 90.0*D2R;

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

      AC->G[14].Cmd.Ang[0] = Limit(GimCmd[0],-120.0*D2R,120.0*D2R);
      AC->G[14].Cmd.Ang[1] = Limit(GimCmd[1],-65.0*D2R,65.0*D2R);

}
/**********************************************************************/
void CmgFSW(struct SCType *S)
{
      struct AcType *AC;
      struct AcCmgCtrlType *C;
      double CBL[3][3],qbl[4],qbr[4];
      double CRL[3][3];
      double Axis[4][3],Gim[4][3],H[4];
      double Gain;
      static double MoveTime = 200.0;
      static double RPYCmd[3] = {1.0,1.0,1.0};
      static double qrl[4];
      static long Idx = 0;
      long i,j;

      AC = &S->AC;
      C = &AC->CmgCtrl;

      if (C->Init) {
         C->Init = 0;
         for(i=0;i<3;i++) FindPDGains(AC->MOI[i][i],0.5,0.7,&C->Kr[i],&C->Kp[i]);
         for(i=0;i<4;i++) {
            AC->G[i].IsUnderActiveControl = TRUE;
            AC->G[i].Cmd.Ang[0] = 0.0;
            AC->G[i].AngGain[0] = 0.0;
            AC->G[i].AngRateGain[0] = 100.0;
            AC->G[i].MaxAngRate[0] = 1.0*D2R;
            AC->G[i].MaxTrq[0] = 5.0;
         }
      }

      MoveTime -= AC->DT;
      if (MoveTime < 0.0) {
         MoveTime = 200.0;
         Idx = (Idx+1)%3;
         if (RPYCmd[Idx] > 0.0) RPYCmd[Idx] = -60.0*D2R;
         else RPYCmd[Idx] = 60.0*D2R;
         A2C(123,RPYCmd[0],RPYCmd[1],RPYCmd[2],CRL);
         C2Q(CRL,qrl);
      }

      MxMT(S->B[0].CN,S->CLN,CBL);
      C2Q(CBL,qbl);
      QxQT(qbl,qrl,qbr);
      RECTIFYQ(qbr);
      for(i=0;i<3;i++) {
         C->therr[i] = 2.0*qbr[i];
         C->werr[i] = S->B[0].wn[i];
         C->Tcmd[i] = -C->Kr[i]*C->werr[i] - C->Kp[i]*C->therr[i]; 
      }

      for(i=0;i<4;i++) {
         for(j=0;j<3;j++) {
            Axis[i][j] = AC->G[i].COI[2][j];
            Gim[i][j] = AC->G[i].COI[0][j];  
         }
         H[i] = 75.0;
      }

      Gain = CMGLaw4x1DOF(C->Tcmd,Axis,Gim,H,C->AngRateCmd);

      for(i=0;i<4;i++) {
         AC->G[i].Cmd.AngRate[0] = C->AngRateCmd[i];
      }
}
/**********************************************************************/
void ThrFSW(struct SCType *S)
{
      struct AcType *AC;
      struct AcThrType *T;
      struct AcThrCtrlType *C;
      static double MoveTime = 0.0;
      double RollCmd[4] = {30.0,0.0,-30.0,0.0};
      double PitchCmd[4] = {0.0,30.0,0.0,-30.0};
      double YawCmd[4] = {0.0,0.0,0.0,0.0};
      double PosXcmd[4] = {0.0,0.0,0.0,0.0};
      double PosYcmd[4] = {24.0,0.0,-24.0,0.0};
      double PosZcmd[4] = {0.0,24.0,0.0,-24.0};
      static double CRL[3][3],PosRL[3];
      double CRN[3][3],qrn[4],PosRN[3];
      double FcmdB[3];
      double FoA,TorxA;
      static long Idx = 0;
      long i;
      
      AC = &S->AC;
      C = &AC->ThrCtrl;
      
      if (C->Init) {
         C->Init = 0;
         for(i=0;i<3;i++) FindPDGains(AC->MOI[i][i],0.1,0.7,&C->Kw[i],&C->Kth[i]);
         FindPDGains(AC->mass,0.05,1.0,&C->Kv,&C->Kp);
      }
      
/* .. Commanded Attitude and Position */
      MoveTime -= AC->DT;
      if (MoveTime < 0.0) {
         MoveTime = 1000.0;
         Idx = (Idx+1)%4;
         A2C(123,RollCmd[Idx]*D2R,PitchCmd[Idx]*D2R,YawCmd[Idx]*D2R,CRL);
         PosRL[0] = PosXcmd[Idx];
         PosRL[1] = PosYcmd[Idx];
         PosRL[2] = PosZcmd[Idx];
      }
      MxM(CRL,S->CLN,CRN);
      C2Q(CRN,qrn);
      QxQT(AC->qbn,qrn,AC->qbr);
      RECTIFYQ(AC->qbr);
      MTxV(S->CLN,PosRL,PosRN);
      
/* .. Force and Torque Commands */
      for(i=0;i<3;i++) {
         AC->Tcmd[i] = -C->Kw[i]*S->B[0].wn[i] - C->Kth[i]*2.0*AC->qbr[i];
         AC->Fcmd[i] = -C->Kv*S->VelR[i] - C->Kp*(S->PosR[i] - PosRN[i]);
         AC->Tcmd[i] = Limit(AC->Tcmd[i],-4.0,4.0);
      }
      MxV(S->B[0].CN,AC->Fcmd,FcmdB);
      for(i=0;i<3;i++)  FcmdB[i] = Limit(FcmdB[i],-2.0,2.0);
      MTxV(S->B[0].CN,FcmdB,AC->Fcmd);
      
#if 0
/* .. Ideal Actuators to check out controller before tackling thruster logic */
      for(i=0;i<3;i++) {
         AC->IdealTrq[i] = AC->Tcmd[i];
         AC->IdealFrc[i] = AC->Fcmd[i];
      }
#else
/* .. Distribute to Thrusters */
      for(i=0;i<AC->Nthr;i++) {
         T = &AC->Thr[i];
         T->PulseWidthCmd = 0.0;
         
         FoA = VoV(FcmdB,T->Axis);
         TorxA = VoV(AC->Tcmd,T->rxA);
         if ( FoA > 0.0 && TorxA > 0.0) {
            T->PulseWidthCmd = (0.25*FoA + TorxA)/T->Fmax*AC->DT;
         }
         
         T->PulseWidthCmd = Limit(T->PulseWidthCmd,0.0,AC->DT);
      }
         
#endif
}
#if 0
/**********************************************************************/
/* CFS_FSW: A test case to work out interfaces between 42 and a       */
/* CFS flight software configuration.                                 */
void CfsFSW(struct AcType *AC)
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
         for(i=0;i<3;i++) FindPDGains(AC->MOI[i][i],0.1*TwoPi,0.7,&C->Kr[i],&C->Kp[i]);
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
#endif
/**********************************************************************/
/* Put your custom controller here                                    */
void AdHocFSW(struct SCType *S)
{
      struct AcType *AC;
      struct AcAdHocCtrlType *C;
      double CLN[3][3],CRN[3][3],qrn[4],wln[3];
      double CRL[3][3] = {{ 1.0, 0.0, 0.0}, 
                          { 0.0, 1.0, 0.0},
                          { 0.0, 0.0, 1.0}}; /* Point +Z to antivelocity */
      long i;

      AC = &S->AC;
      C = &AC->AdHocCtrl;

      if (C->Init) {
         C->Init = 0;
         for(i=0;i<3;i++) {
            FindPDGains(AC->MOI[i][i],0.1*TwoPi,0.7,&C->Kr[i],&C->Kp[i]);
            //C->Kp[i] *= 0.5;
            //C->Kr[i] *= 0.5;
         }
      }

/* .. Form attitude error signals */
      FindCLN(AC->PosN,AC->VelN,CLN,wln);
      MxM(CRL,CLN,CRN);
      C2Q(CRN,qrn);
      //QxQT(AC->qbn,qrn,AC->qbr);
      for(i=0;i<4;i++) AC->qbr[i] = AC->qbn[i];
      RECTIFYQ(AC->qbr);
      for(i=0;i<3;i++) {
         C->therr[i] = 2.0*AC->qbr[i];
         C->werr[i] = AC->wbn[i]; // - wln[i];
      }

/* .. Closed-loop attitude control */
      for(i=0;i<3;i++) {
         C->Tcmd[i] =
         -C->Kr[i]*C->werr[i]-C->Kp[i]*C->therr[i];
      }

      for(i=0;i<3;i++) AC->IdealTrq[i] = C->Tcmd[i];
      //for(i=0;i<3;i++) AC->Whl[i].Tcmd = -C->Tcmd[i];
}
/**********************************************************************/
/* Learning about finding frequency response in time domain           */
void FreqRespFSW(struct SCType *S)
{
      struct AcType *AC;
      struct AcAdHocCtrlType *C;
      long i;

      AC = &S->AC;
      C = &AC->AdHocCtrl;

      if (C->Init) {
         C->Init = 0;
         for(i=0;i<3;i++) {
            FindPDGains(AC->MOI[i][i],0.1*TwoPi,0.7,&C->Kr[i],&C->Kp[i]);
            AC->qrn[i] = 0.0;
            AC->wrn[i] = 0.0;
         }
         AC->qrn[3] = 1.0;
      }

/* .. Form attitude error signals */
      QxQT(AC->qbn,AC->qrn,AC->qbr);
      RECTIFYQ(AC->qbr);
      for(i=0;i<3;i++) {
         C->therr[i] = 2.0*AC->qbr[i];
         C->werr[i] = AC->wbn[i]-AC->wrn[i];
      }

/* .. Closed-loop attitude control */
      for(i=0;i<3;i++) {
         C->Tcmd[i] =
         -C->Kr[i]*C->werr[i]-C->Kp[i]*C->therr[i];
      }

      for(i=0;i<3;i++) AC->IdealTrq[i] = C->Tcmd[i];
}
/**********************************************************************/
#define FREQ_INIT 0
#define FREQ_WARMUP 1
#define FREQ_SWEEP 2
#define FREQ_DONE 3
void FreqResp(struct SCType *S)
{
      struct AcType *AC;
      struct FreqRespType *F;
      struct FreqNormEqType *NE;
      double dec,t,c,s,Mag,dB[3],Phase[3];
      double AtAi[4][4],Soln[4];
      double A1,B1;
      double TotTime,f,P;
      long Ia,i,j;
      char str[80];
      
      AC = &S->AC;
      F = &S->FreqResp;
      
      if (F->State == FREQ_INIT) {
         sprintf(str,"FreqResp%02ld.42",S->ID);
         F->outfile = FileOpen(InOutPath,str,"w");
         
         F->MinDecade = -1.0;
         F->MaxDecade =  2.0;
         F->Nf = 201;
         F->RefAmp = 1.0/3600.0*D2R;
               
         f = pow(10.0,F->MaxDecade);
         P = 1.0/f;
         if (DTSIM > 0.1*P) {
            printf("Warning: DTSIM is too large for FreqResp max freq.  Suggest DTSIM < %lf\n",0.1*P);
         }
         for(i=0;i<3;i++) {
            AC->qrn[i] = 0.0;
         }
         AC->qrn[3] = 1.0;
         
         F->Np = (double *) calloc(F->Nf,sizeof(double));
         TotTime = 0.0;
         for(i=0;i<F->Nf;i++) {
            dec = F->MinDecade+(F->MaxDecade-F->MinDecade)*
               ((double) i)/((double) (F->Nf-1));
            f = pow(10.0,dec);            
            P = 1.0/f;
            F->SettleTime = ceil(30.0/P)*P;
            F->ReadTime = F->SettleTime + F->SettleTime;
            F->EndTime = F->ReadTime + 10.0*F->RefPeriod;
            TotTime += F->EndTime;
         }
         printf("Expected FreqResp End Time: %lf sec\n",SimTime+TotTime);
         
         F->If = 0;
         F->InitFreq = 1;
         F->Time = 0.0;
         F->State = FREQ_SWEEP;
      }
      else if (F->State == FREQ_SWEEP) {
         /* Sweep over frequencies */
         if (F->If < F->Nf) {
            if (F->InitFreq) {
               F->InitFreq = 0;
               F->Time = 0.0;
               dec = F->MinDecade+(F->MaxDecade-F->MinDecade)*
                  ((double) F->If)/((double) (F->Nf-1));
               F->RefFreq = TwoPi*pow(10.0,dec);            
               F->RefPeriod = TwoPi/F->RefFreq;
               F->SettleTime = ceil(30.0/F->RefPeriod)*F->RefPeriod;
               F->ReadTime = F->SettleTime + F->SettleTime;
               F->EndTime = F->ReadTime + 10.0*F->RefPeriod;
            
               for(Ia=0;Ia<3;Ia++) {
                  for(i=0;i<4;i++) {
                     for(j=0;j<4;j++) F->NormEq[Ia].AtA[i][j] = 0.0;
                     F->NormEq[Ia].Atb[i] = 0.0;
                  }
               }
               
            }
            else {            
               /* Fit response to sinusoid */
               Q2AngleVec(AC->qbn,F->OutAng);
               if (F->Time > F->ReadTime) {
                  t = F->Time;
                  c = cos(F->RefFreq*F->Time);
                  s = sin(F->RefFreq*F->Time);
                  for(Ia=0;Ia<3;Ia++) {
                     NE = &F->NormEq[Ia];
                     NE->AtA[0][0] += 1.0;
                     NE->AtA[0][1] += t;
                     NE->AtA[0][2] += c;
                     NE->AtA[0][3] += s;
                     NE->AtA[1][0] += t;
                     NE->AtA[1][1] += t*t;
                     NE->AtA[1][2] += t*c;
                     NE->AtA[1][3] += t*s;
                     NE->AtA[2][0] += c;
                     NE->AtA[2][1] += c*t;
                     NE->AtA[2][2] += c*c;
                     NE->AtA[2][3] += c*s;
                     NE->AtA[3][0] += s;
                     NE->AtA[3][1] += s*t;
                     NE->AtA[3][2] += s*c;
                     NE->AtA[3][3] += s*s;
                     NE->Atb[0] +=   F->OutAng[Ia];
                     NE->Atb[1] += t*F->OutAng[Ia];
                     NE->Atb[2] += c*F->OutAng[Ia];
                     NE->Atb[3] += s*F->OutAng[Ia];
                  }
               }
            
               if (F->Time < F->SettleTime) {
                  for(i=0;i<3;i++) {
                     F->RefAng[i] = 0.0;
                     F->RefRate[i] = 0.0;
                     AC->qrn[i] = 0.0;
                     AC->wrn[i] = 0.0;
                  }
                  AC->qrn[3] = 1.0;
               }
               else if (F->Time < F->ReadTime) {
                  /* Generate Reference Signal */
                  for(i=0;i<3;i++) {
                     F->RefAng[i] = F->RefAmp;
                     F->RefRate[i] = 0.0;
                     AC->qrn[i] = 0.5*F->RefAng[i];
                     AC->wrn[i] = F->RefRate[i];
                  }
                  AC->qrn[3] = 1.0;
                  UNITQ(AC->qrn);
               }
               else {
                  /* Generate Reference Signal */
                  for(i=0;i<3;i++) {
                     F->RefAng[i] = F->RefAmp*cos(F->RefFreq*F->Time);
                     F->RefRate[i] = -F->RefAmp*F->RefFreq*sin(F->RefFreq*F->Time);
                     AC->qrn[i] = 0.5*F->RefAng[i];
                     AC->wrn[i] = F->RefRate[i];
                  }
                  AC->qrn[3] = 1.0;
                  UNITQ(AC->qrn);
               
                  /* Overwrite qbn, wbn */
                  for(i=0;i<3;i++) {
                     AC->qbn[i] = 0.0;
                     AC->wbn[i] = 0.0;
                  }
                  AC->qbn[3] = 1.0;
               }

               /* Record magnitude, phase */
               F->Time += DTSIM;
               if (F->Time > F->EndTime) {
                  F->Time = 0.0;
                  F->If++;
                  F->InitFreq = 1;
               
                  for(Ia=0;Ia<3;Ia++) {
                     MINV4(NE->AtA,AtAi);
                     for(i=0;i<4;i++) {
                        Soln[i] = 0.0;
                        for(j=0;j<4;j++) {
                           Soln[i] += AtAi[i][j]*NE->Atb[j];
                        }
                     }
                     A1 = Soln[2];
                     B1 = Soln[3];
                     Mag = sqrt(A1*A1+B1*B1)/F->RefAmp;
                     dB[Ia] = 20.0*log10(Mag);
                     Phase[Ia] = -atan2(B1,A1)*R2D;
                  }
                  fprintf(F->outfile,"%le %lf %lf %lf %lf %lf %lf\n",
                     F->RefFreq/TwoPi,dB[0],dB[1],dB[2],Phase[0],Phase[1],Phase[2]);
               }
            }
         }
         else {
            F->State = FREQ_DONE;
         }
      }
      else { /* FREQ_DONE */
         fclose(F->outfile);
         for(i=0;i<3;i++) {
            F->RefAng[i] = 0.0;
            F->RefRate[i] = 0.0;
            AC->qrn[i] = 0.0;
            AC->wrn[i] = 0.0;
         }
         AC->qrn[3] = 1.0;
         S->FreqRespActive = FALSE;
         printf("Frequency Response Complete at Time = %lf\n",SimTime);
      }

      
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
      #ifdef _AC_STANDALONE_
      struct IpcType *I;
      long Iipc;
      #endif

      if (S->FreqRespActive) {
         FreqResp(S);
      }
            
      S->FswSampleCounter++;
      if (S->FswSampleCounter >= S->FswMaxCounter) {
         S->FswSampleCounter = 0;
         
         switch(S->FswTag){
            case PASSIVE_FSW:
               break;
            case PROTOTYPE_FSW:
               PrototypeFSW(S);
               break;
            case AD_HOC_FSW:
               AdHocFSW(S);
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
            case ISS_FSW:
               IssFSW(S);
               break;
            case CMG_FSW:
               CmgFSW(S);
               break;
            case THR_FSW:
               ThrFSW(S);
               break;
            case CFS_FSW:
               #ifdef _AC_STANDALONE_
               for(Iipc=0;Iipc<Nipc;Iipc++) {
                  I = &IPC[Iipc];
                  if (I->Mode == IPC_ACS && I->AcsID == S->AC.ID) {
                     if (I->Init) {
                        I->Init = 0;
                        S->AC.ParmLoadEnabled = 1;
                        S->AC.ParmDumpEnabled = 1;
                        S->AC.EchoEnabled = 1;

                        WriteToSocket(I->Socket,I->Prefix,I->Nprefix,I->EchoEnabled);
                        ReadFromSocket(I->Socket,I->EchoEnabled);

                        S->AC.ParmLoadEnabled = 0;
                        S->AC.ParmDumpEnabled = 0;
                     }
                     else {
                        WriteToSocket(I->Socket,I->Prefix,I->Nprefix,I->EchoEnabled);
                        ReadFromSocket(I->Socket,I->EchoEnabled);
                     }
                  }
               }
               #else
                  AcFsw(&S->AC);
               #endif
               break;
            case FREQRESP_FSW:
               FreqRespFSW(S);
               break;
         }
         
      }
      
      MapCmdsToActuators(S);
}

/* #ifdef __cplusplus
** }
** #endif
*/


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

/******************************************************************************/
/* References:                                                                */
/*                                                                            */
/* [1]  Louis J. Ippolito, Jr.                                                */
/*   "Satellite Communications Systems Engineering:                           */
/*   Atmospheric Effects, Satellite Link Design and System Performance"       */
/*   Wiley 2008.                                                              */
/*                                                                            */
/* [2]  Wertz et al. "Space Mission Engineering: The New SMAD",               */
/*   Microcosm 2011.                                                          */

/******************************************************************************/
long IsOcculted(double Pos[3], double Dir[3], double Range, double rad)
{
      double P,PosDir[3],PoD,RbyP;
      
      P = CopyUnitV(Pos,PosDir);
      PoD = VoV(PosDir,Dir);
      RbyP = rad/P;

      if (PoD < 0.0 && RbyP*RbyP > 1.0-PoD*PoD && Range > -P*PoD) return(1);
      else return(0);
}
/******************************************************************************/
void PosToENU(double PosW[3],double CPW[3][3])
{
      double Zaxis[3] = {0.0,0.0,1.0};
      double Up[3],North[3],East[3];
      long i;

      CopyUnitV(PosW,Up);
      VxV(Zaxis,Up,East);
      UNITV(East);
      VxV(Up,East,North);
      UNITV(North);
      for(i=0;i<3;i++) {
         CPW[0][i] = East[i];
         CPW[1][i] = North[i];
         CPW[2][i] = Up[i];
      }
}
/******************************************************************************/
void GndPosVel(struct WorldType *W, struct GroundStationType *G, double Delay, 
   double PosN[3], double VelN[3], double PosH[3], double VelH[3])
{
      double CDW[3][3]; /* Delay */
      double CDN[3][3];
      double Zaxis[3] = {0.0,0.0,1.0};
      long i;

      SimpRot(Zaxis,-Delay*W->w,CDW);
      MxM(CDW,W->CWN,CDN);
      MTxV(CDN,G->PosW,PosN);
      VelN[0] = -W->w*PosN[1];
      VelN[1] =  W->w*PosN[0];
      VelN[2] = 0.0; 

      MTxV(W->CNH,PosN,PosH);
      MTxV(W->CNH,VelN,VelH);
      for(i=0;i<3;i++) {
         PosH[i] += W->PosH[i];
         VelH[i] += W->VelH[i];
      }
}
/******************************************************************************/
void ScPosVel(struct WorldType *W, struct OrbitType *O, double Delay, 
   double PosN[3], double VelN[3], double PosH[3], double VelH[3])
{
      double anom;
      long i;
      
      //if (O->Regime == ORB_THREE_BODY) {
      //   if (O->LagDOF == LAGDOF_MODES) {
      //      LagModes2RV(Time,&LagSys[O->Sys],
      //         O,O->PosN,O->VelN);
      //   }
      //   else if (O->LagDOF == LAGDOF_COWELL) {
      //      ThreeBodyOrbitRK4(O);
      //      RV2LagModes(Time,&LagSys[O->Sys],O);
      //      O->Epoch = Time;
      //   }
      //   else if (O->LagDOF == LAGDOF_SPLINE) {
      //      SplineToPosVel(O);
      //   }
      //}
      //else if (O->Regime == ORB_CENTRAL) {
         //if (O->SplineActive) SplineToPosVel(O);
         //else if (O->J2DriftEnabled) MeanEph2RV(O,Time);
         //else {
            Eph2RV(O->mu,O->SLR,O->ecc,O->inc,O->RAAN,O->ArgP,
                   DynTime-O->tp-Delay,PosN,VelN,&anom);
         //}
      //}
      ///* ORB_FLIGHT is handled in Ephemerides() */
      ///* ORB_ZERO requires no action */

      MTxV(W->CNH,PosN,PosH);
      MTxV(W->CNH,VelN,VelH);
      for(i=0;i<3;i++) {
         PosH[i] += W->PosH[i];
         VelH[i] += W->VelH[i];
      }
}
/******************************************************************************/
/* Tx is Gnd, Rx is S/C                                                       */
void UplinkGeometry(struct CommLinkType *L)
{
      struct GroundStationType *G;
      struct SCType *S;
      struct WorldType *Wtx,*Wrx;
      struct OrbitType *O;
      struct BodyType *B;
      double CAW[3][3];

      G = &GroundStation[L->TxID];
      S = &SC[L->RxID];
      B = &S->B[L->RxBody];
      O = &Orb[S->RefOrb];
      L->TxWorld = G->World;
      L->RxWorld = O->World;
      Wtx = &World[L->TxWorld];
      Wrx = &World[L->RxWorld];

      GndPosVel(Wtx,G,L->Delay,L->TxPosN,L->TxVelN,L->TxPosH,L->TxVelH);
      /* Assume ground station "A" frame is ENU */
      PosToENU(G->PosW,CAW);
      MxM(CAW,Wtx->CWN,L->TxCAN);
      MxM(L->TxCAN,Wtx->CNH,L->TxCAH);

      ScPosVel(Wrx,O,L->Delay,L->RxPosN,L->RxVelN,L->RxPosH,L->RxVelH);
      MxM(L->RxCAB,B->CN,L->RxCAN);
      MxM(L->RxCAN,Wrx->CNH,L->RxCAH);
}
/******************************************************************************/
/* Tx is S/C, Rx is Gnd                                                       */
void DownlinkGeometry(struct CommLinkType *L)
{
      struct GroundStationType *G;
      struct SCType *S;
      struct WorldType *Wtx,*Wrx;
      struct OrbitType *O;
      struct BodyType *B;
      double CAW[3][3];

      G = &GroundStation[L->RxID];
      S = &SC[L->TxID];
      B = &S->B[L->TxBody];
      O = &Orb[S->RefOrb];
      L->TxWorld = O->World;
      L->RxWorld = G->World;
      Wtx = &World[L->TxWorld];
      Wrx = &World[L->RxWorld];

      GndPosVel(Wrx,G,L->Delay,L->RxPosN,L->RxVelN,L->RxPosH,L->RxVelH);
      /* Assume ground station "A" frame is ENU */
      PosToENU(G->PosW,CAW);
      MxM(CAW,Wrx->CWN,L->RxCAN);
      MxM(L->RxCAN,Wrx->CNH,L->RxCAH);

      ScPosVel(Wtx,O,L->Delay,L->TxPosN,L->TxVelN,L->TxPosH,L->TxVelH);
      MxM(L->TxCAB,B->CN,L->TxCAN);
      MxM(L->TxCAN,Wtx->CNH,L->TxCAH);
}
/******************************************************************************/
/* Both Tx and Rx are S/C                                                     */
void CrosslinkGeometry(struct CommLinkType *L)
{
      struct SCType *Stx,*Srx;
      struct WorldType *Wtx,*Wrx;
      struct OrbitType *Otx,*Orx;
      struct BodyType *Btx,*Brx;

      Stx = &SC[L->TxID];
      Btx = &Stx->B[L->TxBody];
      Otx = &Orb[Stx->RefOrb];
      Srx = &SC[L->RxID];
      Brx = &Srx->B[L->RxBody];
      Orx = &Orb[Srx->RefOrb];
      L->RxWorld = Orx->World;
      L->TxWorld = Otx->World;
      Wtx = &World[L->TxWorld];
      Wrx = &World[L->RxWorld];

      MxM(L->TxCAB,Btx->CN,L->TxCAN);
      MxM(L->TxCAN,Wtx->CNH,L->TxCAH);

      MxM(L->RxCAB,Brx->CN,L->RxCAN);
      MxM(L->RxCAN,Wrx->CNH,L->RxCAH);

      ScPosVel(Wtx,Otx,L->Delay,L->TxPosN,L->TxVelN,L->TxPosH,L->TxVelH);
      ScPosVel(Wrx,Orx,L->Delay,L->RxPosN,L->RxVelN,L->RxPosH,L->RxVelH);
}
/******************************************************************************/
void LinkGeometry(struct CommLinkType *L)
{
      struct WorldType *Wtx,*Wrx;
      double RelVelN[3],RelVelH[3];
      double dt;
      double PrevDelay = 0.0;
      long i;

      Wtx = &World[L->TxWorld];
      Wrx = &World[L->RxWorld];

      L->Delay = 0.0;
      do {
         if (L->LinkType == UPLINK) UplinkGeometry(L);
         else if (L->LinkType == DOWNLINK) DownlinkGeometry(L);
         else if (L->LinkType == CROSSLINK) CrosslinkGeometry(L);
         else {
            printf("Error: Unknown link type.\n");
            exit(1);
         }
   
         /* Find Range, Delay */
         if (L->TxWorld == L->RxWorld) {
            for(i=0;i<3;i++) {
               L->TxPathDirN[i] = L->RxPosN[i] - L->TxPosN[i];
            }
            L->Range = UNITV(L->TxPathDirN);
            L->Delay = L->Range/SPEED_OF_LIGHT;
         }
         else {
            for(i=0;i<3;i++) {
               L->TxPathDirH[i] = L->RxPosH[i] - L->TxPosH[i];
            }
            L->Range = UNITV(L->TxPathDirH);
            L->Delay = L->Range/SPEED_OF_LIGHT;
         }
         dt = L->Delay - PrevDelay;
         PrevDelay = L->Delay;
      } while(L->PosAdjustEnabled && fabs(dt) > L->DelayTol);
      
      /* Find RangeRate, Doppler */
      if (L->TxWorld == L->RxWorld) {
         MTxV(Wtx->CNH,L->TxPathDirN,L->TxPathDirH);
         for(i=0;i<3;i++) {
            L->RxPathDirN[i] = -L->TxPathDirN[i];
            L->RxPathDirH[i] = -L->TxPathDirH[i];
            RelVelN[i] = L->TxVelN[i] - L->RxVelN[i];
         }
         L->RangeRate = VoV(RelVelN,L->RxPathDirN);
         L->Doppler = -L->RangeRate/SPEED_OF_LIGHT*L->Freq;
      }
      else {
         MxV(Wtx->CNH,L->TxPathDirH,L->TxPathDirN);
         for(i=0;i<3;i++) {
            L->RxPathDirH[i] = -L->TxPathDirH[i];
            RelVelH[i] = L->TxVelH[i] - L->RxVelH[i];
         }
         MxV(Wrx->CNH,L->RxPathDirH,L->RxPathDirN);
         L->RangeRate = VoV(RelVelH,L->RxPathDirH);
         L->Doppler = -L->RangeRate/SPEED_OF_LIGHT*L->Freq;
      }

      /* Check for occultation */
      if (L->LinkType == UPLINK) {
         if (VoV(L->TxPosN,L->TxPathDirN) > 0.0) L->TxOcculted = 0;
         else L->TxOcculted = 1;
         L->RxOcculted = IsOcculted(L->RxPosN,L->RxPathDirN,L->Range,Wrx->rad);
      }
      else if (L->LinkType == DOWNLINK) {
         L->TxOcculted = IsOcculted(L->TxPosN,L->TxPathDirN,L->Range,Wtx->rad);
         if (VoV(L->RxPosN,L->RxPathDirN) > 0.0) L->RxOcculted = 0;
         else L->RxOcculted = 1;
      }
      else {
         L->TxOcculted = IsOcculted(L->TxPosN,L->TxPathDirN,L->Range,Wtx->rad);
         L->RxOcculted = IsOcculted(L->RxPosN,L->RxPathDirN,L->Range,Wrx->rad);
      }
      if (L->TxOcculted || L->RxOcculted) L->PathIsOcculted = 1;
      else L->PathIsOcculted = 0;

}
/******************************************************************************/
/*  This is a simple stochastic model of losses through an atmosphere.        */
/*  Feel free to replace with your own loss model.                            */
void AtmoLossModel(struct CommLinkType *L)
{
      if (L->LinkType == CROSSLINK) L->AtmoPathLoss = 0.0;
      else {
         L->AtmoPathLoss = L->AtmoMean 
            + (1.0-DTSIM/L->AtmoCorrTime)*(L->AtmoPathLoss-L->AtmoMean) 
            + L->AtmoStd*GaussianRandom(L->AtmoNoise);
      }
}
/******************************************************************************/
double AntGain(struct AntPattType *A, double CAN[3][3], double PathDirN[3])
{
      struct MeshType *M;
      double ViewPt[3],ViewDir[3];
      double PathDirA[3],ProjPt[3];
      double AntGain;
      long Ip,FoundPoly;
      long i;

      M = &Mesh[A->MeshTag];

/* .. Antenna Gain */
      MxV(CAN,PathDirN,PathDirA);
      for(i=0;i<3;i++) {
         ViewPt[i] = 2.0*M->BBox.radius*PathDirA[i];
         ViewDir[i] = -PathDirA[i];
      }
      FoundPoly = OCProjectRayOntoMesh(ViewPt,ViewDir,
         M,ProjPt,&Ip);
      if (FoundPoly) {
         AntGain = A->MeshScl*(MAGV(ProjPt)-A->MinRad)+A->FloorGain;
      }
      else {
         AntGain = A->FloorGain;
      }
      
      return(AntGain);
}
/******************************************************************************/
void LoadLinkFile(void)
{
      FILE *infile;
      long Il,Iv,Seq;
      long EdgesEnabled = 1;
      double ang1,ang2,ang3;
      double dt,r;
      double MaxRad;
      char response[81];
      struct CommLinkType *L;
      struct AntPattType *A;
      struct MeshType *M;
      long Seed;
      long OldNmesh;

      infile = FileOpen(InOutPath,"Inp_CommLink.txt","r");
      
      ScanLine(infile,"",0,NULL);
      ScanLine(infile,"%ld",1,&Nlink);
      
      CommLink = (struct CommLinkType *) calloc(Nlink,sizeof(struct CommLinkType));
      for(Il=0;Il<Nlink;Il++) {
         L = &CommLink[Il];
         ScanLine(infile,"",0,NULL);  
         ScanLine(infile,"",0,NULL); 

         ScanLine(infile,"%80s",1,response);
         L->Exists = DecodeString(response);
         ScanLine(infile,"%80s %lf",2,response,&dt);
         L->OutEnabled = DecodeString(response);
         if (dt < DTSIM) {
            printf("Link[%ld] output timestep < DTSIM.  You'll want to fix that.\n",Il);
            exit(1);
         }
         else L->MaxOutCtr = (long) (dt/DTSIM+0.5);

         ScanLine(infile,"%80s %lf",2,response,&L->DelayTol); 
         L->PosAdjustEnabled = DecodeString(response);

         ScanLine(infile,"%80s",1,response); 
         L->LinkType = DecodeString(response);
         ScanLine(infile,"%lf",1,&L->Freq);
         L->Wavelength = SPEED_OF_LIGHT/L->Freq;
         ScanLine(infile,"%lf",1,&L->NoiseFloor);         
         ScanLine(infile,"%ld %ld",2,&L->TxID,&L->TxBody);
         ScanLine(infile,"%lf %lf %lf %ld",4,&ang1,&ang2,&ang3,&Seq);   
         A2C(Seq,ang1*D2R,ang2*D2R,ang3*D2R,L->TxCAB);      
         ScanLine(infile,"%lf",1,&L->TxPower);
         ScanLine(infile,"%lf %lf",2,
            &L->TxAntPatt.PeakGain,&L->TxAntPatt.FloorGain);
         ScanLine(infile,"\"%80[^\"]\"",1,L->TxAntFileName);
         ScanLine(infile,"%ld %ld",2,&L->RxID,&L->RxBody);
         ScanLine(infile,"%lf %lf %lf %ld",4,&ang1,&ang2,&ang3,&Seq);   
         A2C(Seq,ang1*D2R,ang2*D2R,ang3*D2R,L->RxCAB);
         ScanLine(infile,"%lf %lf",2,
            &L->RxAntPatt.PeakGain,&L->RxAntPatt.FloorGain);
         ScanLine(infile,"\"%80[^\"]\"",1,L->RxAntFileName);
         ScanLine(infile,"%lf",1,&L->RxNoisePower);
         ScanLine(infile,"%lf",1,&L->AtmoMean);
         ScanLine(infile,"%lf %lf %ld",3,
            &L->AtmoStd,&L->AtmoCorrTime,&Seed);
         L->AtmoNoise = CreateRandomProcess(Seed);
         
         OldNmesh = Nmesh;
         Mesh = LoadWingsObjFile(ModelPath,L->TxAntFileName,
            &Matl,&Nmatl,Mesh,&Nmesh,&L->TxAntPatt.MeshTag,EdgesEnabled);
         if (OldNmesh != Nmesh) LoadOctree(&Mesh[Nmesh-1]);
         
         A = &L->TxAntPatt;
         M = &Mesh[A->MeshTag];
         A->MinRad = 1.0E9;
         MaxRad = 0.0;
         for(Iv=0;Iv<M->Nv;Iv++) {
            r = MAGV(M->V[Iv]);
            if (r > MaxRad) MaxRad = r;
            if (r < A->MinRad) A->MinRad = r;
         }
         if (fabs(MaxRad-A->MinRad) < 1.0E-3*MaxRad) { /* Trap isotropic pattern */
            A->MeshScl = 0.0;
         }
         else {
            A->MeshScl = (A->PeakGain-A->FloorGain)/(MaxRad-A->MinRad);
         }
         
         OldNmesh = Nmesh;
         Mesh = LoadWingsObjFile(ModelPath,L->RxAntFileName,
            &Matl,&Nmatl,Mesh,&Nmesh,&L->RxAntPatt.MeshTag,EdgesEnabled);
         if (OldNmesh != Nmesh) LoadOctree(&Mesh[Nmesh-1]);

         A = &L->RxAntPatt;
         M = &Mesh[A->MeshTag];
         A->MinRad = 1.0E9;
         MaxRad = 0.0;
         for(Iv=0;Iv<M->Nv;Iv++) {
            r = MAGV(M->V[Iv]);
            if (r > MaxRad) MaxRad = r;
            if (r < A->MinRad) A->MinRad = r;
         }
         if (fabs(MaxRad-A->MinRad) < 1.0E-3*MaxRad) { /* Trap isotropic pattern */
            A->MeshScl = 0.0;
         }
         else {
            A->MeshScl = (A->PeakGain-A->FloorGain)/(MaxRad-A->MinRad);
         }
      }
      
      fclose(infile);
}
/******************************************************************************/
void CommLinkPerformance(void)
{
      struct CommLinkType *L;
      long Il;
      static long First = 1;

      if (First) {
         First = 0;
         LoadLinkFile();
      }

      for(Il=0;Il<Nlink;Il++) {
         L = &CommLink[Il];
         
         LinkGeometry(L);
         
         L->TxAntGain = AntGain(&L->TxAntPatt,L->TxCAN,L->TxPathDirN);
         L->RxAntGain = AntGain(&L->RxAntPatt,L->RxCAN,L->RxPathDirN);

         AtmoLossModel(L);
                  
         /* [1](4.6) */
         L->EIRP = L->TxPower + L->TxAntGain;
         
         /* [1](4.9) */
         L->PowerFluxDensity = L->EIRP - 20.0*log10(L->Range) - 10.99;
         
         /* [1](4.27) */
         L->FreeSpacePathLoss = 20.0*log10(TwoPi*L->Range/L->Wavelength);
         
         L->Loss = L->FreeSpacePathLoss + L->AtmoPathLoss;

         /* [1](4.31),(4.55) */
         if (L->PathIsOcculted) L->Carrier = L->NoiseFloor;
         else L->Carrier = L->EIRP + L->RxAntGain - L->Loss;
         
         L->Noise = L->RxNoisePower;
                  
         L->CNR = L->Carrier - L->Noise;
      }
}

/* #ifdef __cplusplus
** }
** #endif
*/

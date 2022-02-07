/*    This file is distributed with 42,                               */
/*    the (mostly harmless) spacecraft dynamics simulation            */
/*    created by Eric Stoneking of NASA Goddard Space Flight World   */

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
void AssignScToOrbit(struct SCType *S,long Iorb)
{
      struct OrbitType *OldOrb,*NewOrb;
      long i;

      if (Iorb < 0 || Iorb >= Norb) {
         printf("Desired orbit is out of bounds.\n");
         exit(1);
      }

      OldOrb = &Orb[S->RefOrb];
      NewOrb = &Orb[Iorb];
      NewOrb->Exists = TRUE;

      if (NewOrb->World != OldOrb->World) {
         printf("New Orbit and Old Orbit must (for the present) be in same gravitational system.\n");
         exit(1);
      }

/* .. Update PosR, VelR, PosEH, VelEH */
      for(i=0;i<3;i++) {
         S->PosR[i] = S->PosN[i] - NewOrb->PosN[i];
         S->VelR[i] = S->PosN[i] - NewOrb->PosN[i];
      }
      RelRV2EHRV(MAGV(NewOrb->PosN),MAGV(NewOrb->wln),NewOrb->CLN,
         S->PosR,S->VelR,S->PosEH,S->VelEH);

/* .. Update RefOrb tag */
      S->RefOrb = Iorb;
}
/**********************************************************************/
void FindSCinFormation(struct SCType *S)
{

      double psn[3],pcmn[3];
      double wxr[3],wxrn[3],vsn[3];
      long j;
      struct FormationType *F;

      F = &Frm[S->RefOrb];

/* .. Find CSF */
      MxMT(S->B[0].CN,F->CN,S->CF);

      if (S->OrbDOF) {
         /* Find PosF */
         MTxV(S->B[0].CN,S->cm,pcmn);
         for(j=0;j<3;j++) {
            psn[j] = S->PosR[j] - F->PosR[j] - pcmn[j];
         }
         MxV(F->CN,psn,S->PosF);

         /* Find VelF */
         VxV(S->B[0].wn,S->cm,wxr);
         MTxV(S->B[0].CN,wxr,wxrn);
         for(j=0;j<3;j++) {
            vsn[j] = S->VelR[j] - wxrn[j];
         }
         MxV(F->CN,vsn,S->VelF);
      }
#if 0
      else {
         /* Find PosR */
         MTxV(F->CN,S->PosF,psn);
         MTxV(S->B[0].CN,S->cm,pcmn);
         for(j=0;j<3;j++)
            S->PosR[j] = psn[j] + F->PosR[j] + pcmn[j];
         /* Find VelR */
         MTxV(F->CN,S->VelF,vsn);
         VxV(S->B[0].wn,S->cm,wxr);
         MTxV(S->B[0].CN,wxr,wxrn);
         for(j=0;j<3;j++) S->VelR[j] = vsn[j] + wxrn[j];
      }
#endif

}
/**********************************************************************/
void CheckOrbitRectification(struct OrbitType *O)
{
      long Isc,i;
      double m = 0.0;
      double mr[3] = {0.0,0.0,0.0};
      double mv[3] = {0.0,0.0,0.0};
      double PosR[3],VelR[3];
      double a,n;
      struct SCType *S;

      for(Isc=0;Isc<Nsc;Isc++) {
         if (SC[Isc].Exists && SC[Isc].RefOrb == O->Tag) {
            m += SC[Isc].mass;
            for(i=0;i<3;i++) {
               mr[i] += SC[Isc].mass*SC[Isc].PosR[i];
               mv[i] += SC[Isc].mass*SC[Isc].VelR[i];
            }
         }
      }
      for(i=0;i<3;i++) {
         PosR[i] = mr[i]/m;
         VelR[i] = mv[i]/m;
      }
      if (MAGV(PosR) > 50.0E3) { /* Visualization gets jittery at about 50 km */
         for(i=0;i<3;i++) {      /* due to SC.PosR-POV.rr being difference of */
            O->PosN[i] += PosR[i];  /* large quantities */
            O->VelN[i] += VelR[i];
         }
         if (O->Regime == ORB_CENTRAL)
            RV2Eph(DynTime,O->mu,O->PosN,O->VelN,
                   &O->SMA,&O->ecc,&O->inc,
                   &O->RAAN,&O->ArgP,&O->anom,
                   &O->tp,&O->SLR,&O->alpha,&O->rmin,
                   &O->MeanMotion,&O->Period);
         FindCLN(O->PosN,O->VelN,O->CLN,O->wln);
         a = MAGV(O->PosN);
         n = sqrt(O->mu/(a*a*a));
         for(Isc=0;Isc<Nsc;Isc++) {
            if (SC[Isc].Exists && SC[Isc].RefOrb == O->Tag) {
               S = &SC[Isc];
               for(i=0;i<3;i++) {
                  S->PosR[i] -= PosR[i];
                  S->VelR[i] -= VelR[i];
               }
               RelRV2EHRV(a,n,O->CLN,S->PosR,S->VelR,S->PosEH,S->VelEH);
            }
         }
         printf("Orb[%ld] rectified at Time = %12.3f\n",O->Tag,SimTime);
      }
}
/**********************************************************************/
/*  The N frame is the reference frame to which most dynamical        */
/*  variables are referenced.  Changing orbit Worlds changes the N   */
/*  frame being used.  To avoid discontinuities in actual positions   */
/*  or attitudes, several dynamical variables must be adjusted.       */
void ChangeNFrame(struct OrbitType *O,long OldWorld, long NewWorld)
{
      double CN1H[3][3],CN2H[3][3],CL1H[3][3],CL2H[3][3],CH[3][3],VH[3];
      double CBN1[3][3],CBN2[3][3];
      struct FormationType *F;
      struct SCType *S;
      struct BodyType *B;
      struct DynType *D;
      long Isc,Ib,i,j;

      for(i=0;i<3;i++) {
         for(j=0;j<3;j++) {
            CN1H[i][j] = World[OldWorld].CNH[i][j];
            CN2H[i][j] = World[NewWorld].CNH[i][j];
         }
      }

/* .. Orb */
      MxM(O->CLN,CN1H,CL1H);
      FindCLN(O->PosN,O->VelN,O->CLN,O->wln);
      MxM(O->CLN,CN2H,CL2H);
      /* Update Formation Frame */
      F = &Frm[O->Tag];
      if (F->FixedInFrame == 'L') {
         MxM(F->CL,CL1H,CH);
         MxMT(CH,CL2H,F->CL);
         MxM(F->CL,O->CLN,F->CN);
      }
      else {
         MxM(F->CN,CN1H,CH);
         MxMT(CH,CN2H,F->CN);
         MxMT(F->CN,O->CLN,F->CL);
      }

/* .. SC */
      for(Isc=0;Isc<Nsc;Isc++) {
         S = &SC[Isc];
         if (S->Exists && S->RefOrb == O->Tag) {
            MxM(S->CLN,CN1H,CL1H);

            MTxV(CN1H,S->PosR,VH);
            MxV(CN2H,VH,S->PosR);
            MTxV(CN1H,S->VelR,VH);
            MxV(CN2H,VH,S->VelR);
            for(i=0;i<3;i++) {
               S->PosN[i] = O->PosN[i]+S->PosR[i];
               S->VelN[i] = O->VelN[i]+S->VelR[i];
            }

            FindCLN(S->PosN,S->VelN,S->CLN,S->wln);
            MxM(S->CLN,CN2H,CL2H);
            MTxV(CL1H,S->PosEH,VH);
            MxV(CL2H,VH,S->PosEH);
            MTxV(CL1H,S->VelEH,VH);
            MxV(CL2H,VH,S->VelEH);

            /* Bodies */
            for(Ib=0;Ib<S->Nb;Ib++) {
               B = &S->B[Ib];
               MxM(B->CN,CN1H,CH);
               MxMT(CH,CN2H,B->CN);
               C2Q(B->CN,B->qn);
               MTxV(CN1H,B->vn,VH);
               MxV(CN2H,VH,B->vn);
               MTxV(CN1H,B->pn,VH);
               MxV(CN2H,VH,B->pn);
            }
            /* Dyn */
            D = &S->Dyn;
            Q2C(&D->x[0],CBN1);
            MxM(CBN1,CN1H,CH);
            MxMT(CH,CN2H,CBN2);
            C2Q(CBN2,&D->x[0]);
            MTxV(CN1H,&D->u[D->Nu-3],VH);
            MxV(CN2H,VH,&D->u[D->Nu-3]);
         }
      }

/* .. POV */
      if (POV.Host.RefOrb == O->Tag) {
         POV.Host.World = NewWorld;
         MxM(POV.CN,CN1H,CH);
         MxMT(CH,CN2H,POV.CN);
         MxMT(POV.CN,SC[POV.Host.SC].CLN,POV.CL);

         if (POV.Frame == FRAME_N) {
            for(i=0;i<3;i++) {
               for(j=0;j<3;j++) POV.C[i][j] = POV.CN[i][j];
            }
         }
         else if (POV.Frame == FRAME_L) {
            for(i=0;i<3;i++) {
               for(j=0;j<3;j++) POV.C[i][j] = POV.CL[i][j];
            }
         }
         else if (POV.Frame == FRAME_F) {
            /* Still needs work */
            for(i=0;i<3;i++) {
               for(j=0;j<3;j++) POV.C[i][j] = POV.CF[i][j];
            }
         }
         else if (POV.Frame == FRAME_S || POV.Frame == FRAME_B) {
            for(i=0;i<3;i++) {
               for(j=0;j<3;j++) POV.C[i][j] = POV.CB[i][j];
            }
         }
         C2Q(POV.C,POV.q);
      }
}
/**********************************************************************/
void CheckChangeOfOrbitWorld(struct OrbitType *O)
{
#define NO_TRANSITION 0
#define CENTRAL1_TO_THREEBODY 1
#define CENTRAL2_TO_THREEBODY 2
#define THREEBODY_TO_CENTRAL1 3
#define THREEBODY_TO_CENTRAL2 4

      long i,Im,Iw;
      double dr[3],rh[3],vh[3];
      struct WorldType *P;
      long Transition = NO_TRANSITION;
      long Body1 = 0,Body2 = 1;

      if (O->Regime == ORB_CENTRAL) {
         /* Falling "in" from Body 1-centered to 3-body */
         P = &World[O->World];
         for(Im=0;Im<P->Nsat;Im++) {
            Iw = P->Sat[Im];
            for(i=0;i<3;i++) dr[i] = O->PosN[i]-World[Iw].eph.PosN[i];
            if (MAGV(dr) < 1.99*World[Iw].RadOfInfluence) {
               Transition = CENTRAL1_TO_THREEBODY;
               Body1 = O->World;
               Body2 = Iw;
            }
         }
         if (MAGV(O->PosN) > 0.51*World[O->World].RadOfInfluence) {
            /* Falling "out" from Body 2-centered to 3-body */
            Transition = CENTRAL2_TO_THREEBODY;
            Body1 = World[O->World].Parent;
            Body2 = O->World;
         }
      }
      else { /* ORB_THREE_BODY */
         /* Falling "in" from 3-body to Body 2-centered */
         Iw = O->Body2;
         for(i=0;i<3;i++) dr[i] = O->PosN[i]-World[Iw].eph.PosN[i];
         if (MAGV(dr) < 0.49*World[Iw].RadOfInfluence) {
            Transition = THREEBODY_TO_CENTRAL2;
            Body1 = O->Body1;
            Body2 = O->Body2;
         }
         else {
            /* Falling "out" from 3-body to Body 1-centered */
            for(i=0;i<3;i++)
               dr[i] = O->PosN[i]-World[Iw].eph.PosN[i];
            if (MAGV(dr) > 2.01*World[Iw].RadOfInfluence) {
               Transition = THREEBODY_TO_CENTRAL1;
               Body1 = O->Body1;
               Body2 = O->Body2;
            }
         }
      }

      switch(Transition) {
         case NO_TRANSITION :
            break;
         case CENTRAL1_TO_THREEBODY :
            O->Regime = ORB_THREE_BODY;
            O->Body1 = Body1;
            O->Body2 = Body2;
            O->mu1 = World[Body1].mu;
            O->mu2 = World[Body2].mu;
            printf("Orbit %ld transitioned from Central-1 to 3-body orbit at Time = %lf\n",O->Tag,SimTime);
            break;
         case CENTRAL2_TO_THREEBODY :
            O->Regime = ORB_THREE_BODY;
            O->Body1 = Body1;
            O->Body2 = Body2;
            O->World = Body1;
            O->mu1 = World[Body1].mu;
            O->mu2 = World[Body2].mu;
            O->mu = O->mu1;
            MTxV(World[Body2].CNH,O->PosN,rh);
            MTxV(World[Body2].CNH,O->VelN,vh);
            MxV(World[Body1].CNH,rh,O->PosN);
            MxV(World[Body1].CNH,vh,O->VelN);
            for(i=0;i<3;i++) {
               O->PosN[i] += World[Body2].eph.PosN[i];
               O->VelN[i] += World[Body2].eph.VelN[i];
            }
            RV2Eph(DynTime,O->mu,O->PosN,O->VelN,
                   &O->SMA,&O->ecc,&O->inc,
                   &O->RAAN,&O->ArgP,&O->anom,
                   &O->tp,&O->SLR,&O->alpha,&O->rmin,
                   &O->MeanMotion,&O->Period);
            printf("Orbit %ld transitioned to 3-body orbit at Time = %lf\n",O->Tag,SimTime);
            /* Change of N frame has far-ranging effects */
            ChangeNFrame(O,Body2,Body1);
            break;
         case THREEBODY_TO_CENTRAL1 :
            O->Regime = ORB_CENTRAL;
            O->World = Body1;
            O->mu = World[O->World].mu;
            printf("Orbit %ld transitioned to central orbit at Time = %lf\n",O->Tag,SimTime);
            break;
         case THREEBODY_TO_CENTRAL2 :
            O->Regime = ORB_CENTRAL;
            O->World = Body2;
            O->mu = World[O->World].mu;
            for(i=0;i<3;i++) {
               O->PosN[i] -= World[Body2].eph.PosN[i];
               O->VelN[i] -= World[Body2].eph.VelN[i];
            }
            MTxV(World[Body1].CNH,O->PosN,rh);
            MTxV(World[Body1].CNH,O->VelN,vh);
            MxV(World[Body2].CNH,rh,O->PosN);
            MxV(World[Body2].CNH,vh,O->VelN);
            RV2Eph(DynTime,O->mu,O->PosN,O->VelN,
                   &O->SMA,&O->ecc,&O->inc,
                   &O->RAAN,&O->ArgP,&O->anom,
                   &O->tp,&O->SLR,&O->alpha,&O->rmin,
                   &O->MeanMotion,&O->Period);
            printf("Orbit %ld transitioned to central orbit at Time = %lf\n",O->Tag,SimTime);
            /* Change of N frame has far-ranging effects */
            ChangeNFrame(O,Body1,Body2);
            break;
      }

#undef NO_TRANSITION
#undef CENTRAL1_TO_THREEBODY
#undef CENTRAL2_TO_THREEBODY
#undef THREEBODY_TO_CENTRAL1
#undef THREEBODY_TO_CENTRAL2
}
/**********************************************************************/
void SplineToPosVel(struct OrbitType *O)
{
      long NodeYear,NodeMonth,NodeDay,NodeHour,NodeMin;
      double NodeSec;
      char newline;
      long i,j,k;
      double X[4],Y[4];
      double x[3],v[3],xn[3],vn[3];

/* .. Get nodes from O->SplineFile */
      while(DynTime > O->NodeDynTime[2]) {
         for(i=0;i<3;i++) {
            O->NodeDynTime[i] = O->NodeDynTime[i+1];
            for(j=0;j<3;j++) {
               O->NodePos[i][j] = O->NodePos[i+1][j];
               O->NodeVel[i][j] = O->NodeVel[i+1][j];
            }
         }
         fscanf(O->SplineFile," %ld:%ld:%ld:%ld:%ld:%lf %lf %lf %lf %lf %lf %lf %[\n]",
                  &NodeYear,&NodeMonth,&NodeDay,&NodeHour,&NodeMin,&NodeSec,
                  &O->NodePos[3][0],&O->NodePos[3][1],&O->NodePos[3][2],
                  &O->NodeVel[3][0],&O->NodeVel[3][1],&O->NodeVel[3][2],
                  &newline);
               O->NodeDynTime[3] = DateToTime(NodeYear,NodeMonth,NodeDay,
                  NodeHour,NodeMin,NodeSec);
               for(j=0;j<3;j++) {
                  O->NodePos[3][j] *= 1000.0;
                  O->NodeVel[3][j] *= 1000.0;
               }
         if (feof(O->SplineFile)) {
            printf("Oops.  Reached end of Spline file.\n");
            exit(1);
         }
      }

/* .. Interpolate Spline */
      for(k=0;k<4;k++) X[k] = O->NodeDynTime[k];
      for(j=0;j<3;j++) {
         for(k=0;k<4;k++) Y[k] = O->NodePos[k][j];
         x[j] = CubicSpline(DynTime,X,Y);
         for(k=0;k<4;k++) Y[k] = O->NodeVel[k][j];
         v[j] = CubicSpline(DynTime,X,Y);
      }

      if (O->Regime == ORB_CENTRAL) {
         for(j=0;j<3;j++) {
            O->PosN[j] = x[j];
            O->VelN[j] = v[j];
         }
         RV2Eph(O->Epoch,O->mu,O->PosN,O->VelN,
            &O->SMA,&O->ecc,&O->inc,&O->RAAN,
            &O->ArgP,&O->anom,&O->tp,
            &O->SLR,&O->alpha,&O->rmin,
            &O->MeanMotion,&O->Period);
      }
      else if (O->Regime == ORB_THREE_BODY) {
         MTxV(LagSys[O->Sys].CLN,x,xn);
         MTxV(LagSys[O->Sys].CLN,v,vn);
         for(j=0;j<3;j++) {
            O->PosN[j] = xn[j] + LagSys[O->Sys].LP[O->LP].PosN[j];
            O->VelN[j] = vn[j] + LagSys[O->Sys].LP[O->LP].VelN[j];
         }
      }
      else {
         printf("Invalid Orbit Regime in SplineToPosVel.\n");
         exit(1);
      }
}
/**********************************************************************/
void OrbitMotion(double Time)
{
      long Iorb,i,j;
      struct OrbitType *O;
      struct RegionType *R;

#if 0
      static long RectCtr = 0;
      RectCtr++;
      if (RectCtr > 100) {
         RectCtr = 0;
         for(Iorb=0;Iorb<Norb;Iorb++) {
            if (Orb[Iorb].Exists) {
               CheckOrbitRectification(&Orb[Iorb]);
               CheckChangeOfOrbitWorld(&Orb[Iorb]);
            }
         }
      }
#endif

      for(Iorb=0;Iorb<Norb;Iorb++) {
         O = &Orb[Iorb];
         if (O->Exists) {
            if (O->Regime == ORB_THREE_BODY) {
               if (O->LagDOF == LAGDOF_MODES) {
                  LagModes2RV(Time,&LagSys[O->Sys],
                     O,O->PosN,O->VelN);
               }
               else if (O->LagDOF == LAGDOF_COWELL) {
                  ThreeBodyOrbitRK4(O);
                  RV2LagModes(Time,&LagSys[O->Sys],O);
                  O->Epoch = Time;
               }
               else if (O->LagDOF == LAGDOF_SPLINE) {
                  SplineToPosVel(O);
               }
            }
            else if (O->Regime == ORB_CENTRAL) {
               if (O->J2DriftEnabled) MeanEph2RV(O,Time);
               else {
                  Eph2RV(O->mu,O->SLR,O->ecc,
                         O->inc,O->RAAN,O->ArgP,
                         Time-O->tp,
                         O->PosN,O->VelN,&O->anom);
               }
            }
            /* Else is ORB_ZERO or ORB_FLIGHT, and no action required */
            
            /* Update CLN */
            switch (O->Regime) {
               case ORB_ZERO :
                  /* L is aligned with N, wln is zero */
                  for(i=0;i<3;i++) {
                     for(j=0;j<3;j++) O->CLN[i][j] = 0.0;
                     O->CLN[i][i] = 1.0;
                     O->wln[i] = 0.0;
                  }
                  break;
               case ORB_FLIGHT :
                  /* L is East-North-Up */
                  R = &Rgn[O->Region];
                  for(i=0;i<3;i++) {
                     O->PosN[i] = R->PosN[i];
                     O->VelN[i] = R->VelN[i];
                  }
                  FindENU(O->PosN,World[O->World].w,O->CLN,O->wln);
                  break;
               case ORB_CENTRAL :
                  /* L is LVLH */
                  FindCLN(O->PosN,O->VelN,O->CLN,O->wln);
                  break;
               case ORB_THREE_BODY :
                  /* L is Rotating Frame XYZ? */
                  FindCLN(O->PosN,O->VelN,O->CLN,O->wln);
                  break;
               default :
                  printf("Unknown Orbit Regime in Ephemerides.  Bailing out.\n");
                  exit(1);
            }
            
            /* Update Formation Frame */
            if (Frm[Iorb].FixedInFrame == 'L') {
               MxM(Frm[Iorb].CL,O->CLN,Frm[Iorb].CN);
            }
            else {
               MxMT(Frm[Iorb].CN,O->CLN,Frm[Iorb].CL);
            }
         }
      }
}
/**********************************************************************/
void Ephemerides(void)
{

      struct OrbitType *Eph;
      struct OrbitType *O;
      struct WorldType *W;
      struct RegionType *R;
      struct SCType *S;
      double svh[3],p,pvn[3],SoP,Rp,GMST;
      double r1[3],rh[3],vh[3];
      double ptn[10][3],vtn[10][3],ptw[3];
      double ZAxis[3] = {0.0,0.0,1.0};
      double EarthMoonBaryPosH[3],EarthMoonBaryVelH[3];
      struct LagrangeSystemType *LS;
      long i,j,Ip,Im,Iw,Imb,Ir,Isc;
      long Ic;
      struct Cheb3DType *C;
      double u,dudJD,T[20],U[20],P,dPdu;
      double EMRAT = 81.30056907419062; /* Earth-Moon mass ratio */
      double PosJ[3],VelJ[3];
      double C_W_TETE[3][3],C_TEME_TETE[3][3],C_TETE_J2000[3][3];

/* .. Locate Planets and Luna */
      if (EphemOption == EPH_MEAN) {
         for(Ip=MERCURY;Ip<=PLUTO;Ip++){
            if(World[Ip].Exists) {
               W = &World[Ip];
               /* Call PlanetEphemerides again only for
                  ridiculously high accuracy or rather long sims (years) */
               /*PlanetEphemerides(i,JulDay,... */
               Eph = &W->eph;
               Eph2RV(Eph->mu,Eph->SLR,Eph->ecc,Eph->inc,Eph->RAAN,Eph->ArgP,
                      DynTime-Eph->tp,Eph->PosN,Eph->VelN,&Eph->anom);
               for(j=0;j<3;j++) {
                  W->PosH[j] = Eph->PosN[j];
                  W->VelH[j] = Eph->VelN[j];
               }
               W->PriMerAng = fmod(W->PriMerAngJ2000+W->w*DynTime,TwoPi);
               SimpRot(ZAxis,W->PriMerAng,W->CWN);
            }
         }
         if (World[LUNA].Exists){
            Eph = &World[LUNA].eph;
            /* Meeus computes Luna Position in geocentric ecliptic */
            LunaPosition(TT.JulDay,rh);
            LunaPosition(TT.JulDay+0.01,r1);
            for(j=0;j<3;j++) vh[j] = (r1[j]-rh[j])/(864.0);
            /* Convert to Earth's N frame */
            MxV(World[EARTH].CNH,rh,Eph->PosN);
            MxV(World[EARTH].CNH,vh,Eph->VelN);
            /* Find Luna's osculating elements */
            RV2Eph(DynTime,Eph->mu,Eph->PosN,Eph->VelN,
               &Eph->SMA,&Eph->ecc,&Eph->inc,&Eph->RAAN,
               &Eph->ArgP,&Eph->anom,&Eph->tp,&Eph->SLR,&Eph->alpha,
               &Eph->rmin,&Eph->MeanMotion,&Eph->Period);
            World[LUNA].PriMerAng = LunaPriMerAng(TT.JulDay);
            SimpRot(ZAxis,World[LUNA].PriMerAng,World[LUNA].CWN);
            for(j=0;j<3;j++) {
               World[LUNA].PosH[j] = rh[j] + World[EARTH].PosH[j];
               World[LUNA].VelH[j] = vh[j] + World[EARTH].VelH[j];
            }
         }
      }
      else if (EphemOption == EPH_DE430) {
         /* Update DE430 block if needed */
         if (TT.JulDay > World[SOL].eph.Cheb[1].JD2) LoadDE430(ModelPath,TT.JulDay);
         for(Iw=SOL;Iw<=LUNA;Iw++) {
            W = &World[Iw];
            Eph = &W->eph;
            /* Determine segment */
            Ic=0;
            while(TT.JulDay > Eph->Cheb[Ic].JD2) Ic++;
            /* Apply Chebyshev polynomials */
            C = &Eph->Cheb[Ic];
            dudJD = 2.0/(C->JD2-C->JD1);
            u = (TT.JulDay-C->JD1)*dudJD - 1.0;
            ChebyPolys(u,C->N,T,U);
            for(i=0;i<3;i++) {
               ChebyInterp(T,U,C->Coef[i],C->N,&P,&dPdu);
               PosJ[i] = 1000.0*P;
               VelJ[i] = 1000.0*dPdu*dudJD/86400.0;
            }
            QTxV(qJ2000H,PosJ,Eph->PosN);
            QTxV(qJ2000H,VelJ,Eph->VelN);
         }
         /* Adjust for barycenters */
         /* Move planets from barycentric to Sun-centered */
         for(Iw=MERCURY;Iw<=PLUTO;Iw++) {
            W = &World[Iw];
            for(i=0;i<3;i++) {
               W->eph.PosN[i] -= World[SOL].eph.PosN[i];
               W->eph.VelN[i] -= World[SOL].eph.VelN[i];
               W->PosH[i] = W->eph.PosN[i];
               W->VelH[i] = W->eph.VelN[i];
            }
            W->PriMerAng = fmod(W->w*DynTime,TwoPi);
            SimpRot(ZAxis,W->PriMerAng,W->CWN);
         }
         /* Move Sun to origin */
         for(i=0;i<3;i++) {
            World[SOL].PosH[i] = 0.0;
            World[SOL].VelH[i] = 0.0;
            World[SOL].eph.PosN[i] = 0.0;
            World[SOL].eph.VelN[i] = 0.0;
         }
         /* Adjust Earth from Earth-Moon barycenter */
         /* (Moon PosVel is geocentric, not from barycenter) */
         for(i=0;i<3;i++) {
            EarthMoonBaryPosH[i] = World[EARTH].eph.PosN[i];
            EarthMoonBaryVelH[i] = World[EARTH].eph.VelN[i];
            World[EARTH].eph.PosN[i] -= World[LUNA].eph.PosN[i]/(1.0+EMRAT);
            World[EARTH].eph.VelN[i] -= World[LUNA].eph.VelN[i]/(1.0+EMRAT);
            World[EARTH].PosH[i] = World[EARTH].eph.PosN[i];
            World[EARTH].VelH[i] = World[EARTH].eph.VelN[i];
         }
         for(i=0;i<3;i++) {
            rh[i] = World[LUNA].eph.PosN[i];
            vh[i] = World[LUNA].eph.VelN[i];
            World[LUNA].PosH[i] = World[EARTH].PosH[i] + World[LUNA].eph.PosN[i];
            World[LUNA].VelH[i] = World[EARTH].VelH[i] + World[LUNA].eph.VelN[i];
         }
         /* Rotate Moon into ECI */
         QxV(qJ2000H,rh,World[LUNA].eph.PosN);
         QxV(qJ2000H,vh,World[LUNA].eph.VelN);
         World[LUNA].PriMerAng = LunaPriMerAng(TT.JulDay);
         SimpRot(ZAxis,World[LUNA].PriMerAng,World[LUNA].CWN);
      }
      else {
         printf("Bogus Ephem Option in Ephemerides.  Bailing out.\n");
         exit(1);
      }
      
/* .. Locate Asteroids and Comets */
      for(Imb=0;Imb<Nmb;Imb++) {
         if(World[55+Imb].Exists){
            W = &World[55+Imb];
            Eph = &W->eph;
            Eph2RV(Eph->mu,Eph->SLR,Eph->ecc,Eph->inc,Eph->RAAN,Eph->ArgP,
                   DynTime-Eph->tp,Eph->PosN,Eph->VelN,&Eph->anom);
            for(j=0;j<3;j++) {
               W->PosH[j] = Eph->PosN[j];
               W->VelH[j] = Eph->VelN[j];
            }
            W->PriMerAng = fmod(W->w*DynTime,TwoPi);
            SimpRot(ZAxis,W->PriMerAng,W->CWN);
         }
      }

/* .. Earth rotation is a special case */
      GMST = JD2GMST(UTC.JulDay);
      World[EARTH].PriMerAng = TwoPi*GMST;
      HiFiEarthPrecNute(UTC.JulDay,C_TEME_TETE,C_TETE_J2000);
      SimpRot(ZAxis,World[EARTH].PriMerAng,C_W_TETE);
      MxM(C_W_TETE,C_TETE_J2000,World[EARTH].CWN);

/* .. Other planets' moons */
      for(Ip=MARS;Ip<=PLUTO;Ip++) {
         if (World[Ip].Exists) {
            for(Im=0;Im<World[Ip].Nsat;Im++) {
               Iw = World[Ip].Sat[Im];
               Eph = &World[Iw].eph;
               Eph2RV(Eph->mu, Eph->SLR,Eph->ecc,Eph->inc,Eph->RAAN,
                      Eph->ArgP,DynTime-Eph->tp,Eph->PosN,Eph->VelN,&Eph->anom);
               World[Iw].PriMerAng = fmod(World[Iw].w*DynTime,TwoPi);
               SimpRot(ZAxis,World[Iw].PriMerAng,World[Iw].CWN);
               MTxV(World[Ip].CNH,Eph->PosN,rh);
               MTxV(World[Ip].CNH,Eph->VelN,vh);
               for(i=0;i<3;i++) {
                  World[Iw].PosH[i] = rh[i] + World[Ip].PosH[i];
                  World[Iw].VelH[i] = vh[i] + World[Ip].VelH[i];
               }
            }
         }
      }

/* .. Locate Lagrange Points in N of LagSys Body 1 */
      for(i=0;i<3;i++) {
         LS = &LagSys[i];
         if (LS->Exists) {
            for(j=0;j<5;j++) {
               FindLagPtPosVel(DynTime,LS,j,
                  LS->LP[j].PosN,LS->LP[j].VelN,LS->CLN);
            }
         }
      }

/* .. Regions */
      for(Ir=0;Ir<Nrgn;Ir++) {
         R = &Rgn[Ir];
         W = &World[R->World];
         MTxV(W->CWN,R->PosW,R->PosN);
         R->VelN[0] = -W->w*R->PosN[1];
         R->VelN[1] = W->w*R->PosN[0];
         R->VelN[2] = 0.0;
         MxM(R->CW,W->CWN,R->CN);
      }

/* .. TDRS Spacecraft */
      TDRSPosVel(World[EARTH].PriMerAng,DynTime,ptn,vtn);
      for(i=0;i<10;i++) {
         MxV(World[EARTH].CWN,ptn[i],Tdrs[i].rw);
         for(j=0;j<3;j++) {
            Tdrs[i].PosN[j] = ptn[i][j];
            Tdrs[i].VelN[j] = vtn[i][j];
         }
         CopyUnitV(Tdrs[i].rw,ptw);
         Tdrs[i].lat = asin(ptw[2]);
         Tdrs[i].lng = atan2(ptw[1],ptw[0]);
      }

/* .. SC */
      for(Isc=0;Isc<Nsc;Isc++){
         if (SC[Isc].Exists) {
            S = &SC[Isc];
            O = &Orb[S->RefOrb];
            W = &World[O->World];
            
            /* Local-vertical frame tied to SC */
            if (O->Regime == ORB_ZERO) {
               for(i=0;i<3;i++) {
                  S->PosR[i] = S->PosN[i] - O->PosN[i];
                  S->VelR[i] = S->VelN[i] - O->VelN[i];
                  for(j=0;j<3;j++) S->CLN[i][j] = 0.0;
                  S->CLN[i][i] = 1.0;
                  S->wln[i] = 0.0;
               }
            }
            else if (O->Regime == ORB_FLIGHT) {
               for(j=0;j<3;j++) {
                  S->PosR[j] = S->PosN[j] - O->PosN[j];
                  S->VelR[j] = S->VelN[j] - O->VelN[j];
               }
               FindENU(S->PosN,W->w,S->CLN,S->wln);
            }
            else if (O->Regime == ORB_CENTRAL) {
               for(j=0;j<3;j++) {
                  S->PosN[j] = O->PosN[j] + S->PosR[j];
                  S->VelN[j] = O->VelN[j] + S->VelR[j];
               }
               FindCLN(S->PosN,S->VelN,S->CLN,S->wln);
            }
            else {
               for(j=0;j<3;j++) {
                  S->PosN[j] = O->PosN[j] + S->PosR[j];
                  S->VelN[j] = O->VelN[j] + S->VelR[j];
               }
               FindCLN(S->PosN,S->VelN,S->CLN,S->wln);
            }

            /* Locate Spacecraft in H frame */
            MTxV(W->CNH,S->PosN,S->PosH);
            MTxV(W->CNH,S->VelN,S->VelH);
            for(j=0;j<3;j++) {
               S->PosH[j] += W->PosH[j];
               S->VelH[j] += W->VelH[j];
            }

            /* Sun unit vector */
            for(j=0;j<3;j++)
               svh[j]=-W->PosH[j];
            MxV(W->CNH,svh,S->svn);
            for(j=0;j<3;j++) S->svn[j] -= S->PosN[j];
            UNITV(S->svn);
            MxV(S->B[0].CN,S->svn,S->svb);

            /* Eclipse Flag */
            if (W->Type == SUN) S->Eclipse = FALSE;
            else {
               p = MAGV(S->PosN);
               for(j=0;j<3;j++) pvn[j]= -S->PosN[j];
               UNITV(pvn);
               SoP = VoV(S->svn,pvn);
               S->Eclipse = FALSE;
               if (SoP > 0.0) {
                  Rp = W->rad/p;
                  if (Rp*Rp > 1.0-SoP*SoP) {
                     S->Eclipse = TRUE;
                  }
               }
            }

            /* S/C relationship to its Formation */
            FindSCinFormation(S);

         }
      }
}

/* #ifdef __cplusplus
** }
** #endif
*/


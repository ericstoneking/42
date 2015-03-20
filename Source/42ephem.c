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

//#ifdef __cplusplus
//namespace _42 {
//using namespace Kit;
//#endif

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

      if (NewOrb->center != OldOrb->center) {
         printf("New Orbit and Old Orbit must (for the present) be in same gravitational system.\n");
         exit(1);
      }

/* .. Update Rrel, Vrel, Reh, Veh */
      for(i=0;i<3;i++) {
         S->Rrel[i] = S->PosN[i] - NewOrb->PosN[i];
         S->Vrel[i] = S->PosN[i] - NewOrb->PosN[i];
      }
      RelRV2EHRV(MAGV(NewOrb->PosN),MAGV(NewOrb->wln),NewOrb->CLN,
         S->Rrel,S->Vrel,S->Reh,S->Veh);

/* .. Update RefOrb tag */
      S->RefOrb = Iorb;
}
/**********************************************************************/
void FindSCinFormation(struct SCType *S)
{

      double psn[3],pcmn[3];
      double wxr[3],wxrn[3],vsn[3];
      long j,RefOrb;

      RefOrb = S->RefOrb;

/* .. Find CSF */
      MxMT(S->B[0].CN,Frm[RefOrb].CN,S->CF);

      if (S->OrbDOF) {
         /* Find psf */
         MTxV(S->B[0].CN,S->cm,pcmn);
         for(j=0;j<3;j++) {
            psn[j] = S->Rrel[j] - Frm[RefOrb].PosR[j] - pcmn[j];
         }
         MxV(Frm[RefOrb].CN,psn,S->pf);

         /* Find vsf */
         VxV(S->B[0].wn,S->cm,wxr);
         MTxV(S->B[0].CN,wxr,wxrn);
         for(j=0;j<3;j++) {
            vsn[j] = S->Vrel[j] - wxrn[j];
         }
         MxV(Frm[RefOrb].CN,vsn,S->vf);
      }
#if 0
      else {
         /* Find Rrel */
         MTxV(Frm[RefOrb].CN,S->pf,psn);
         MTxV(S->B[0].CN,S->cm,pcmn);
         for(j=0;j<3;j++)
            S->Rrel[j] = psn[j] + Frm[RefOrb].pr[j] + pcmn[j];
         /* Find Vrel */
         MTxV(Frm[RefOrb].CN,S->vf,vsn);
         VxV(S->B[0].wn,S->cm,wxr);
         MTxV(S->B[0].CN,wxr,wxrn);
         for(j=0;j<3;j++) S->Vrel[j] = vsn[j] + wxrn[j];
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
      double Rrel[3],Vrel[3];
      double a,n;
      struct SCType *S;

      for(Isc=0;Isc<Nsc;Isc++) {
         if (SC[Isc].Exists && SC[Isc].RefOrb == O->Tag) {
            m += SC[Isc].mass;
            for(i=0;i<3;i++) {
               mr[i] += SC[Isc].mass*SC[Isc].Rrel[i];
               mv[i] += SC[Isc].mass*SC[Isc].Vrel[i];
            }
         }
      }
      for(i=0;i<3;i++) {
         Rrel[i] = mr[i]/m;
         Vrel[i] = mv[i]/m;
      }
      if (MAGV(Rrel) > 50.0E3) { /* Visualization gets jittery at about 50 km */
         for(i=0;i<3;i++) {      /* due to SC.Rrel-POV.rr being difference of */
            O->PosN[i] += Rrel[i];  /* large quantities */
            O->VelN[i] += Vrel[i];
         }
         if (O->Type == ORB_CENTRAL)
            RV2Eph(AbsTime,O->mu,O->PosN,O->VelN,
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
                  S->Rrel[i] -= Rrel[i];
                  S->Vrel[i] -= Vrel[i];
               }
               RelRV2EHRV(a,n,O->CLN,S->Rrel,S->Vrel,S->Reh,S->Veh);
            }
         }
         printf("Orb[%ld] rectified at Time = %12.3f\n",O->Tag,SimTime);
      }
}
/**********************************************************************/
/*  The N frame is the reference frame to which most dynamical        */
/*  variables are referenced.  Changing orbit centers changes the N   */
/*  frame being used.  To avoid discontinuities in actual positions   */
/*  or attitudes, several dynamical variables must be adjusted.       */
void ChangeNFrame(struct OrbitType *O,long OldCenter, long NewCenter)
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
            CN1H[i][j] = World[OldCenter].CNH[i][j];
            CN2H[i][j] = World[NewCenter].CNH[i][j];
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

            MTxV(CN1H,S->Rrel,VH);
            MxV(CN2H,VH,S->Rrel);
            MTxV(CN1H,S->Vrel,VH);
            MxV(CN2H,VH,S->Vrel);
            for(i=0;i<3;i++) {
               S->PosN[i] = O->PosN[i]+S->Rrel[i];
               S->VelN[i] = O->VelN[i]+S->Vrel[i];
            }

            FindCLN(S->PosN,S->VelN,S->CLN,S->wln);
            MxM(S->CLN,CN2H,CL2H);
            MTxV(CL1H,S->Reh,VH);
            MxV(CL2H,VH,S->Reh);
            MTxV(CL1H,S->Veh,VH);
            MxV(CL2H,VH,S->Veh);

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
         POV.Host.World = NewCenter;
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
void CheckChangeOfOrbitCenter(struct OrbitType *O)
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

      if (O->Type == ORB_CENTRAL) {
         /* Falling "in" from Body 1-centered to 3-body */
         P = &World[O->center];
         for(Im=0;Im<P->Nsat;Im++) {
            Iw = P->Sat[Im];
            for(i=0;i<3;i++) dr[i] = O->PosN[i]-World[Iw].eph.PosN[i];
            if (MAGV(dr) < 1.99*World[Iw].RadOfInfluence) {
               Transition = CENTRAL1_TO_THREEBODY;
               Body1 = O->center;
               Body2 = Iw;
            }
         }
         if (MAGV(O->PosN) > 0.51*World[O->center].RadOfInfluence) {
            /* Falling "out" from Body 2-centered to 3-body */
            Transition = CENTRAL2_TO_THREEBODY;
            Body1 = World[O->center].Parent;
            Body2 = O->center;
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
            O->Type = ORB_THREE_BODY;
            O->Body1 = Body1;
            O->Body2 = Body2;
            O->mu1 = World[Body1].mu;
            O->mu2 = World[Body2].mu;
            printf("Orbit %ld transitioned from Central-1 to 3-body orbit at Time = %lf\n",O->Tag,SimTime);
            break;
         case CENTRAL2_TO_THREEBODY :
            O->Type = ORB_THREE_BODY;
            O->Body1 = Body1;
            O->Body2 = Body2;
            O->center = Body1;
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
            RV2Eph(AbsTime,O->mu,O->PosN,O->VelN,
                   &O->SMA,&O->ecc,&O->inc,
                   &O->RAAN,&O->ArgP,&O->anom,
                   &O->tp,&O->SLR,&O->alpha,&O->rmin,
                   &O->MeanMotion,&O->Period);
            printf("Orbit %ld transitioned to 3-body orbit at Time = %lf\n",O->Tag,SimTime);
            /* Change of N frame has far-ranging effects */
            ChangeNFrame(O,Body2,Body1);
            break;
         case THREEBODY_TO_CENTRAL1 :
            O->Type = ORB_CENTRAL;
            O->center = Body1;
            O->mu = World[O->center].mu;
            printf("Orbit %ld transitioned to central orbit at Time = %lf\n",O->Tag,SimTime);
            break;
         case THREEBODY_TO_CENTRAL2 :
            O->Type = ORB_CENTRAL;
            O->center = Body2;
            O->mu = World[O->center].mu;
            for(i=0;i<3;i++) {
               O->PosN[i] -= World[Body2].eph.PosN[i];
               O->VelN[i] -= World[Body2].eph.VelN[i];
            }
            MTxV(World[Body1].CNH,O->PosN,rh);
            MTxV(World[Body1].CNH,O->VelN,vh);
            MxV(World[Body2].CNH,rh,O->PosN);
            MxV(World[Body2].CNH,vh,O->VelN);
            RV2Eph(AbsTime,O->mu,O->PosN,O->VelN,
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
void OrbitMotion(void)
{
      //static long RectCtr = 0;
      long Iorb;
      struct OrbitType *O;

#if 0
      RectCtr++;
      if (RectCtr > 100) {
         RectCtr = 0;
         for(Iorb=0;Iorb<Norb;Iorb++) {
            if (Orb[Iorb].Exists) {
               CheckOrbitRectification(&Orb[Iorb]);
               CheckChangeOfOrbitCenter(&Orb[Iorb]);
            }
         }
      }
#endif

      for(Iorb=0;Iorb<Norb;Iorb++) {
         O = &Orb[Iorb];
         if (O->Exists) {
            if (O->Type == ORB_THREE_BODY) {
               if (O->LagDOF == LAGDOF_MODES) {
                  LagModes2RV(AbsTime,&LagSys[O->Sys],
                     O,O->PosN,O->VelN);
               }
               else { /* LAGDOF_COWELL */
                  ThreeBodyOrbitRK4(O);
                  RV2LagModes(AbsTime,&LagSys[O->Sys],O);
                  O->Epoch = AbsTime;
               }
            }
            else if (O->Type == ORB_CENTRAL) {
               Eph2RV(O->mu,O->SLR,O->ecc,
                      O->inc,O->RAAN,O->ArgP,
                      AbsTime+DTSIM-O->tp,
                      O->PosN,O->VelN,&O->anom);
            }
            /* Else is ORB_ZERO, and no action required */
         }
      }
}
/**********************************************************************/
void Ephemerides(void)
{

      struct OrbitType *Eph;
      struct OrbitType *O;
      struct WorldType *P;
      struct MinorBodyType *MB;
      struct SCType *S;
      double svh[3],p,pvn[3],SoP,Rp,GMST;
      double r1[3],rh[3],vh[3];
      double ptn[10][3],vtn[10][3];
      double ZAxis[3] = {0.0,0.0,1.0};
      struct LagrangeSystemType *LS;
      long i,j,Ip,Im,Iw;
      long RefOrb,center;

/* .. Update Julian Day, etc */
      JulDay = AbsTimeToJD(AbsTime);
      JD2YMDHMS(JulDay,&Year,&Month,&Day,&Hour,&Minute,&Second);
      doy = MD2DOY(Year,Month,Day);

/* .. Locate Planets */
      for(i=MERCURY;i<=PLUTO;i++){
         if(World[i].Exists){
            /* Call PlanetEphemerides again only for
               ridiculously high accuracy or rather long sims (years) */
            /*PlanetEphemerides(i,JulDay,... */
            Eph = &World[i].eph;
            Eph2RV(Eph->mu,Eph->SLR,Eph->ecc,Eph->inc,Eph->RAAN,Eph->ArgP,
                   AbsTime-Eph->tp,Eph->PosN,Eph->VelN,&Eph->anom);
            for(j=0;j<3;j++) {
               World[i].PosH[j] = Eph->PosN[j];
               World[i].VelH[j] = Eph->VelN[j];
            }
            World[i].PriMerAng = fmod(World[i].w*AbsTime,TwoPi);
            SimpRot(ZAxis,World[i].PriMerAng,World[i].CWN);
         }
      }

/* .. Locate Asteroids and Comets */
      for(i=0;i<Nmb;i++) {
         if(MinorBody[i].Exists){
            MB = &MinorBody[i];
            Eph = &MB->eph;
            Eph2RV(Eph->mu,Eph->SLR,Eph->ecc,Eph->inc,Eph->RAAN,Eph->ArgP,
                   AbsTime-Eph->tp,Eph->PosN,Eph->VelN,&Eph->anom);
            for(j=0;j<3;j++) {
               MB->PosH[j] = Eph->PosN[j];
               MB->VelH[j] = Eph->VelN[j];
            }
            MB->PriMerAng = fmod(MB->w*AbsTime,TwoPi);
            SimpRot(ZAxis,MB->PriMerAng,MB->CWN);
         }
      }

/* .. Earth rotation is a special case */
      GMST = JD2GMST(JulDay);
      World[EARTH].PriMerAng = TwoPi*GMST;
      SimpRot(ZAxis,World[EARTH].PriMerAng,World[EARTH].CWN);

/* .. Locate Luna */
      if (World[LUNA].Exists){
         Eph = &World[LUNA].eph;
         /* Meeus computes Luna Position in geocentric ecliptic */
         LunaPosition(JulDay,rh);
         LunaPosition(JulDay+0.01,r1);
         for(j=0;j<3;j++) vh[j] = (r1[j]-rh[j])/(864.0);
         /* Convert to Earth's N frame */
         MxV(World[EARTH].CNH,rh,Eph->PosN);
         MxV(World[EARTH].CNH,vh,Eph->VelN);
         /* Find Luna's osculating elements */
         RV2Eph(AbsTime,Eph->mu,Eph->PosN,Eph->VelN,
            &Eph->SMA,&Eph->ecc,&Eph->inc,&Eph->RAAN,
            &Eph->ArgP,&Eph->anom,&Eph->tp,&Eph->SLR,&Eph->alpha,
            &Eph->rmin,&Eph->MeanMotion,&Eph->Period);
         World[LUNA].PriMerAng = LunaPriMerAng(JulDay);
         SimpRot(ZAxis,World[LUNA].PriMerAng,World[LUNA].CWN);
         for(j=0;j<3;j++) {
            World[LUNA].PosH[j] = rh[j] + World[EARTH].PosH[j];
            World[LUNA].VelH[j] = vh[j] + World[EARTH].VelH[j];
         }
      }

/* .. Other planets' moons */
      for(Ip=MARS;Ip<=PLUTO;Ip++) {
         if (World[Ip].Exists) {
            for(Im=0;Im<World[Ip].Nsat;Im++) {
               Iw = World[Ip].Sat[Im];
               Eph = &World[Iw].eph;
               Eph2RV(Eph->mu, Eph->SLR,Eph->ecc,Eph->inc,Eph->RAAN,
                      Eph->ArgP,AbsTime-Eph->tp,Eph->PosN,Eph->VelN,&Eph->anom);
               World[Iw].PriMerAng = fmod(World[i].w*AbsTime,TwoPi);
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
               FindLagPtPosVel(AbsTime,LS,j,
                  LS->LP[j].PosN,LS->LP[j].VelN,LS->CLN);
            }
         }
      }

/* .. Local Vertical frame tied to Reference Orbit */
      for(i=0;i<Norb;i++){
         if (Orb[i].Exists && Orb[i].Type != ORB_ZERO) {
            FindCLN(Orb[i].PosN,Orb[i].VelN,Orb[i].CLN,Orb[i].wln);
            /* Update Formation Frame */
            if (Frm[i].FixedInFrame == 'L') {
               MxM(Frm[i].CL,Orb[i].CLN,Frm[i].CN);
            }
            else {
               MxMT(Frm[i].CN,Orb[i].CLN,Frm[i].CL);
            }
         }
      }

/* .. TDRS Spacecraft */
      TDRSPosVel(World[EARTH].PriMerAng,AbsTime,ptn,vtn);
      for(i=0;i<10;i++) {
         MxV(World[EARTH].CWN,ptn[i],Tdrs[i].rw);
         for(j=0;j<3;j++) {
            Tdrs[i].PosN[j] = ptn[i][j];
            Tdrs[i].VelN[j] = vtn[i][j];
         }
         UNITV(ptn[i]);
         Tdrs[i].lat = asin(ptn[i][2]);
         Tdrs[i].lng = atan2(ptn[i][1],ptn[i][0]);
      }


/* .. SC */
      for(i=0;i<Nsc;i++){
         if (SC[i].Exists) {
            RefOrb = SC[i].RefOrb;
            center = Orb[RefOrb].center;
            S = &SC[i];
            O = &Orb[RefOrb];
            if (O->CenterType == MINORBODY) MB = &MinorBody[center];
            else P = &World[center];

            /* Locate Spacecraft in N frame */
            for(j=0;j<3;j++) {
               S->PosN[j] = O->PosN[j] + S->Rrel[j];
               S->VelN[j] = O->VelN[j] + S->Vrel[j];
            }

            /* Local-vertical frame tied to SC */
            FindCLN(S->PosN,S->VelN,S->CLN,S->wln);

            /* Locate Spacecraft in H frame */
            if (O->CenterType == MINORBODY) {
               MTxV(MB->CNH,S->PosN,S->PosH);
               MTxV(MB->CNH,S->VelN,S->VelH);
               for(j=0;j<3;j++) {
                  S->PosH[j] += MB->PosH[j];
                  S->VelH[j] += MB->VelH[j];
               }
            }
            else {
               MTxV(P->CNH,S->PosN,S->PosH);
               MTxV(P->CNH,S->VelN,S->VelH);
               for(j=0;j<3;j++) {
                  S->PosH[j] += P->PosH[j];
                  S->VelH[j] += P->VelH[j];
               }
            }

            /* Sun unit vector */
            if (O->CenterType == MINORBODY) {
               for(j=0;j<3;j++)
                  svh[j]=-MB->PosH[j];
               MxV(MB->CNH,svh,S->svn);
            }
            else {
               for(j=0;j<3;j++)
                  svh[j]=-P->PosH[j];
               MxV(P->CNH,svh,S->svn);
            }
            for(j=0;j<3;j++) S->svn[j] -= S->PosN[j];
            UNITV(S->svn);
            MxV(S->B[0].CN,S->svn,S->svb);

            /* Eclipse Flag */
            if (World[center].Type == SUN) S->Eclipse = FALSE;
            else {
               p = MAGV(S->PosN);
               for(j=0;j<3;j++) pvn[j]= -S->PosN[j];
               UNITV(pvn);
               SoP = VoV(S->svn,pvn);
               S->Eclipse = FALSE;
               if (SoP > 0.0) {
                  if (O->CenterType == MINORBODY) Rp = MB->rad/p;
                  else Rp = P->rad/p;
                  if (Rp*Rp > 1.0-SoP*SoP) {
                     S->Eclipse = TRUE;
                  }
               }
            }

            /* S/C relationship to its Formation */
            FindSCinFormation(&SC[i]);

         }
      }
}

//#ifdef __cplusplus
//}
//#endif


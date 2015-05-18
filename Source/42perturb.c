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
void GravGradFrcTrq(struct SCType *S)
{
      double r,rb[3],Coef,axIoa[3];
      double rhat[3],c[3],rhatoc;
      long Ib,i;
      struct BodyType *B;

      r = CopyUnitV(S->PosN,rhat);
      Coef = Orb[S->RefOrb].mu/(r*r*r);

      if (S->Nb == 1) {
         B = &S->B[0];
         /* GG torque */
         MxV(B->CN,rhat,rb);
         vxMov(rb,B->I,axIoa);
         for(i=0;i<3;i++) B->Trq[i] += 3.0*Coef*axIoa[i];
      }
      else {
         CopyUnitV(S->PosN,rhat);
         for(Ib=0;Ib<S->Nb;Ib++) {
            B = &S->B[Ib];
            /* GG torque */
            MxV(B->CN,rhat,rb);
            vxMov(rb,B->I,axIoa);
            for(i=0;i<3;i++) B->Trq[i] += 3.0*Coef*axIoa[i];

            /* GG force from Hughes, p. 246, eq. (56) */
            for(i=0;i<3;i++) c[i] = B->mass*B->pn[i];
            rhatoc = VoV(rhat,c);
            for(i=0;i<3;i++) {
               B->Frc[i] -= Coef*(c[i]-3.0*rhat[i]*rhatoc);
            }
         }
      }
}
/**********************************************************************/
void ThirdBodyGravForce(double p[3],double s[3],double mu, double mass,
   double Frc[3])
{
      double magp,mags,p3,s3;
      long j;

      magp = MAGV(p);
      mags = MAGV(s);
      p3 = magp*magp*magp;
      s3 = mags*mags*mags;
      for(j=0;j<3;j++) Frc[j] = mu*mass*(s[j]/s3-p[j]/p3);
}
/**********************************************************************/
void GravPertForce(struct SCType *S)
{
      struct OrbitType *O;
      double FgeoN[3],ph[3],p[3],s[3],Frc[3];
      long Iw,Im,j;
      long OrbCenter,SecCenter;

      O = &Orb[S->RefOrb];
      if (O->Regime == ORB_CENTRAL) {
         OrbCenter = O->World;
         SecCenter = -1; /* Nonsense value */
      }
      else {
         OrbCenter = O->Body1;
         SecCenter = O->Body2;
      }

      /* Sun and all existing planets */
      for(Iw=SOL;Iw<=PLUTO;Iw++) {
         if (World[Iw].Exists && !(Iw == OrbCenter || Iw == SecCenter)) {
            for(j=0;j<3;j++)
               ph[j] = World[Iw].eph.PosN[j]
                      -World[OrbCenter].eph.PosN[j];
            MxV(World[OrbCenter].CNH,ph,p);
            for(j=0;j<3;j++) s[j] = p[j]-S->PosN[j];
            ThirdBodyGravForce(p,s,World[Iw].mu,S->mass,Frc);
            for(j=0;j<3;j++) S->Frc[j] += Frc[j];
         }
      }
      /* Moons of OrbCenter (but not SecCenter) */
      if (OrbCenter != SOL) {
         for(Im=0;Im<World[OrbCenter].Nsat;Im++) {
            Iw = World[OrbCenter].Sat[Im];
            if (Iw != SecCenter) {
               for(j=0;j<3;j++) {
                  p[j] = World[Iw].eph.PosN[j];
                  s[j] = p[j]-S->PosN[j];
               }
               ThirdBodyGravForce(p,s,World[Iw].mu,S->mass,Frc);
               for(j=0;j<3;j++) S->Frc[j] += Frc[j];
            }
         }
      }
      /* Moons of SecCenter */
      if (O->Regime == ORB_THREE_BODY) {
         for(Im=0;Im<World[SecCenter].Nsat;Im++) {
            Iw = World[SecCenter].Sat[Im];
            for(j=0;j<3;j++) p[j] = World[Iw].eph.PosN[j];
            MTxV(World[SecCenter].CNH,p,ph);
            MxV(World[OrbCenter].CNH,ph,p);
            for(j=0;j<3;j++) {
               p[j] += World[SecCenter].eph.PosN[j];
               s[j] = p[j]-S->PosN[j];
            }
            ThirdBodyGravForce(p,s,World[Iw].mu,S->mass,Frc);
            for(j=0;j<3;j++) S->Frc[j] += Frc[j];
         }
      }

      /* Perturbations due to non-spherical gravity potential */
      if (OrbCenter == EARTH) {
         EGM96(ModelPath,EarthGravModel.N,EarthGravModel.M,S->mass,S->PosN,
               World[EARTH].PriMerAng,FgeoN);
         for(j=0;j<3;j++) S->Frc[j] += FgeoN[j];
      }
      else if (OrbCenter == MARS) {
         GMM2B(ModelPath,MarsGravModel.N,MarsGravModel.M,S->mass,S->PosN,
               World[MARS].PriMerAng,FgeoN);
         for(j=0;j<3;j++) S->Frc[j] += FgeoN[j];
      }
      else if (OrbCenter == LUNA) {
         GLGM2(ModelPath,LunaGravModel.N,LunaGravModel.M,S->mass,S->PosN,
               World[LUNA].PriMerAng,FgeoN);
         for(j=0;j<3;j++) S->Frc[j] += FgeoN[j];
      }
      /* else if O->CenterType == MINORBODY, use provided gravity model */
}
/**********************************************************************/
void AeroFrcTrq(struct SCType *S)
{

      double VrelN[3],WindSpeed,VrelB[3],Area,PolyArea,cp[3];
      double WoN,Coef,Fb[3],Fn[3],Trq[3];
      long Ib,i;
      long Ipoly;
      long OrbCenter;
      struct BodyType *B;
      struct GeomType *G;
      struct PolyType *P;

      OrbCenter = Orb[S->RefOrb].World;

/* .. Find Velocity Relative to Atmosphere, expressed in N */
      VrelN[0] = S->VelN[0]
                 +World[OrbCenter].w*S->PosN[1];
      VrelN[1] = S->VelN[1]
                 -World[OrbCenter].w*S->PosN[0];
      VrelN[2] = S->VelN[2];
      WindSpeed = UNITV(VrelN);

/* .. Find Force and Torque on each Body, in that body's frame */
      for(Ib=0;Ib<S->Nb;Ib++) {
         B = &S->B[Ib];

         /* Transform Rel Wind to B */
         MxV(B->CN,VrelN,VrelB);

         /* Find total projected area and cp for Body */
         Area = 0.0;
         for(i=0;i<3;i++) cp[i] = 0.0;
         G = &Geom[B->GeomTag];
         for(Ipoly=0;Ipoly<G->Npoly;Ipoly++) {
            P = &G->Poly[Ipoly];
            if (strcmp(Matl[P->Matl].Label,"INTERIOR")) { /* Aero doesn't see interior polys */
               WoN = VoV(VrelB,P->Norm);
               if (WoN > 0.0) {
                  PolyArea = WoN*P->Area;
                  Area += PolyArea;
                  for(i=0;i<3;i++)
                     cp[i] += PolyArea*(P->Centroid[i]-B->cm[i]);
               }
            }
         }
         if (Area > 0.0) {for(i=0;i<3;i++) cp[i] /= Area;}

         /* Compute force and torque exerted on B */
         Coef = -0.5*S->AtmoDensity*S->DragCoef
               *WindSpeed*WindSpeed*Area;
         for(i=0;i<3;i++) Fb[i] = Coef*VrelB[i];
         MTxV(B->CN,Fb,Fn);
         for(i=0;i<3;i++) B->Frc[i] += Fn[i];
         VxV(cp,Fb,Trq);
         for(i=0;i<3;i++) B->Trq[i] += Trq[i];
      }
}
/**********************************************************************/
void SolPressFrcTrq(struct SCType *S)
{
      long Ib,i;
      long Ipoly;
      double svb[3],SoN,Coef,r[3],Fb[3],Fn[3],Tb[3],SolarPressure;
      struct BodyType *B;
      struct GeomType *G;
      struct PolyType *P;
      struct MatlType *M;

      if (!S->Eclipse) {
         /* Solar pressure is 4.5E-6 N/m^2 at Earth orbit radius, */
         /* and falls off as R^2                                  */
         SolarPressure = 4.5E-6*2.238E22/VoV(S->PosH,S->PosH);

/* .. Find Force and Torque on each Body */
         for(Ib=0;Ib<S->Nb;Ib++) {
            B = &S->B[Ib];
            G = &Geom[B->GeomTag];

            /* Find force and torque on each illuminated polygon */
            for(Ipoly=0;Ipoly<G->Npoly;Ipoly++) {
               P = &G->Poly[Ipoly];
               if (strcmp(Matl[P->Matl].Label,"INTERIOR")) { /* SRP doesn't see interior polys */
                  MxV(B->CN,S->svn,svb);
                  SoN = VoV(svb,P->Norm);
                  if (SoN > 0.0) {
                     M = &Matl[P->Matl];
                     Coef = -SolarPressure*P->Area*SoN;
                     for(i=0;i<3;i++) {
                        Fb[i] = Coef*((1.0-M->SpecFrac)*svb[i] +
                        2.0*(M->SpecFrac*SoN +M->DiffFrac/3.0) * P->Norm[i]);
                     }
                     for(i=0;i<3;i++) r[i] = P->Centroid[i]-B->cm[i];
                     VxV(r,Fb,Tb);
                     MTxV(B->CN,Fb,Fn);
                     for(i=0;i<3;i++) {
                        B->Frc[i] += Fn[i];
                        B->Trq[i] += Tb[i];
                     }
                  }
               }
            }
         }
      }

}
/**********************************************************************/
/*  Static and dynamic imbalance may have any relative phasing.       */
/*  This model makes the dynamic imbalance torque be in phase         */
/*  with the torque due to static imbalance.                          */
void RwaImbalance(struct SCType *S)
{
      struct BodyType *B;
      struct WhlType *W;
      struct FlexNodeType *FN;
      double c,s,Coef,PosB[3],Fb[3],Fn[3],Tb[3],PoA;
      long Iw,i;

      B = &S->B[0];

      for(Iw=0;Iw<S->Nw;Iw++) {
         W = &S->Whl[Iw];
         FN = &B->FlexNode[W->FlexNode];
         c = cos(W->ang);
         s = sin(W->ang);

         /* Position of Wheel wrt cm of Body */
         for(i=0;i<3;i++) PosB[i] = FN->PosB[i] - B->cm[i];

         /* Static Imbalance Force */
         Coef = W->Ks*W->w*W->w;
         for(i=0;i<3;i++) {
            Fb[i] = Coef*(c*W->Uhat[i] + s*W->Vhat[i]);
         }
         MTxV(B->CN,Fb,Fn);
         VxV(PosB,Fb,Tb);
         for(i=0;i<3;i++) {
            B->Frc[i] += Fn[i];
            B->Trq[i] += Tb[i];
            FN->Frc[i] += Fb[i];
            FN->Trq[i] += Tb[i];
         }

         /* Dynamic Imbalance Torque */
         Coef = W->Kd*W->w*W->w;
         /* This sign makes sure dyn imbalance trq is in phase with */
         /* trq from static imbalance */
         PoA = (VoV(PosB,W->A) > 0.0 ? 1.0 : -1.0);
         for(i=0;i<3;i++) {
            Tb[i] = Coef*PoA*(-s*W->Uhat[i]+c*W->Vhat[i]);
            B->Trq[i] += Tb[i];
            FN->Trq[i] += Tb[i];
         }
      }
}
/**********************************************************************/
/* For each Poly in Body B, find force and torque due to contact with */
/* each poly in Region R.                                             */
void BodyRgnContactFrcTrq(struct SCType *S, long Ibody,
   struct RegionType *R)
{
      struct GeomType *Gb,*Gr;
      struct BodyType *B;
      struct PolyType *Pb,*Pr;
      struct EdgeType *E;
      double FrcN[3] = {0.0,0.0,0.0};
      double TrqB[3] = {0.0,0.0,0.0};
      double pbn[3],rb[3],wxrb[3],wxrn[3],vbn[3];
      double prn[3],vrn[3],pbrn[3],vbrn[3],CPR[3][3],CPN[3][3];
      double PosP[3],VelP[3];
      double FrcP[3];
      double ContactArea;
      double Dist,MinDist;
      double PosR[3],RelPosR[3];
      static long HitPoly = 0;
      long OtherPoly;
      long Ib,Ie,i,Done;
      double Fn[3],Fb[3],Tb[3];

      B = &S->B[Ibody];
      Gb = &Geom[B->GeomTag];
      Gr = &Geom[R->GeomTag];

      for(i=0;i<3;i++) {
         FrcN[i] = 0.0;
         TrqB[i] = 0.0;
      }

      /* Loop through all Polys (Pb) in Gb */
      for(Ib=0;Ib<Gb->Npoly;Ib++) {
         Pb = &Gb->Poly[Ib];
         /* Use Centroid for proximity */
         /* Find position and velocity of Centroid wrt origin of R */
         MTxV(B->CN,Pb->Centroid,pbn);
         for(i=0;i<3;i++) pbn[i] += S->PosR[i] + B->pn[i];
         for(i=0;i<3;i++) rb[i] = Pb->Centroid[i] - B->cm[i];
         VxV(B->wn,rb,wxrb);
         MTxV(B->CN,wxrb,wxrn);
         for(i=0;i<3;i++) vbn[i] = S->VelR[i] + B->vn[i] + wxrn[i];
         MxV(R->CN,pbn,PosR);

         /* Find poly (Pr) in Gr closest to Pb */
         Done = 0;
         while(!Done) {
            Done = 1;
            for(i=0;i<3;i++) RelPosR[i] = PosR[i] - Gr->Poly[HitPoly].Centroid[i];
            MinDist = MAGV(RelPosR);
            /* Check neighboring polys */
            for(Ie=0;Ie<3;Ie++) {
               E = &Gr->Edge[Gr->Poly[HitPoly].E[Ie]];
               if (E->Poly1 >= 0 && E->Poly2 >= 0) { /* Screen edges of region */
                  OtherPoly = (E->Poly1 == HitPoly ? E->Poly2 : E->Poly1);
                  for(i=0;i<3;i++) RelPosR[i] = PosR[i] - Gr->Poly[OtherPoly].Centroid[i];
                  Dist = MAGV(RelPosR);
                  if (Dist < MinDist) {
                     MinDist = Dist;
                     HitPoly = OtherPoly;
                     Done = 0;
                     break;
                  }
               }
            }
         }

         /* Interact with selected poly */
         Pr = &Gr->Poly[HitPoly];
         MTxV(R->CN,Pr->Centroid,prn);
         VxV(R->wn,Pr->Centroid,wxrb);
         MTxV(R->CN,wxrb,vrn);

         for(i=0;i<3;i++) {
            pbrn[i] = pbn[i] - prn[i];
            vbrn[i] = vbn[i] - vrn[i];
            CPR[0][i] = Pr->Uhat[i];
            CPR[1][i] = Pr->Vhat[i];
            CPR[2][i] = Pr->Norm[i];
         }
         MxM(CPR,R->CN,CPN);
         MxV(CPN,pbrn,PosP);
         MxV(CPN,vbrn,VelP);

         /* Find contact force */
         //printf("Body %ld Poly %ld h = %lf\n",
         //   Ibody,Ib,h);
         FrcP[2] = 0.0;
         FrcP[0] = 0.0;
         FrcP[1] = 0.0;
         if (PosP[2] < Pb->radius) {
            if (PosP[2] > 0.0) {
               ContactArea = (1.0-PosP[2]/Pb->radius)*Pb->Area;
               FrcP[2] = -R->DampCoef*VelP[2]*ContactArea;
               FrcP[0] = 0.0;
               FrcP[1] = 0.0;
            }
            else {
               FrcP[2] = -(R->ElastCoef*PosP[2] + R->DampCoef*VelP[2])*Pb->Area;
               FrcP[0] = -R->FricCoef*FrcP[2]*VelP[0];
               FrcP[1] = -R->FricCoef*FrcP[2]*VelP[1];
            }
         }

         /* Transform into N, B frames */
         MTxV(CPN,FrcP,Fn);
         MxV(B->CN,Fn,Fb);
         VxV(rb,Fb,Tb);
         for(i=0;i<3;i++) {
            FrcN[i] += Fn[i];
            TrqB[i] += Tb[i];
         }
      }

/* .. Limit Force and Torque to avoid adding Kinetic Energy */


      for(i=0;i<3;i++) {
         B->Frc[i] += FrcN[i];
         B->Trq[i] += TrqB[i];
      }
}
/**********************************************************************/
void ContactFrcTrq(struct SCType *S)
{
      struct OrbitType *O;
      struct RegionType *R;
      double dx[3];
      long Ir,i,Ib;

      O = &Orb[S->RefOrb];

/* .. Contact with Regions */
      for(Ir=0;Ir<Nrgn;Ir++) {
         R = &Rgn[Ir];
         /* Cheap proximity checks */
         if (!R->Exists) continue;
         if (R->World != O->World) continue;
         for(i=0;i<3;i++) dx[i] = S->PosN[i] - R->PosN[i];
         if (MAGV(dx) > S->BBox.radius + Geom[R->GeomTag].BBox.radius) continue;

         /* Check each body vs Region */
         for(Ib=0;Ib<S->Nb;Ib++) {
            BodyRgnContactFrcTrq(S,Ib,R);
         }
      }

/* .. Contact with other S/C will follow a similar template */

}
/**********************************************************************/
/* .. Resolve perturbation torque and force system to torques about   */
/* .. the S/C cm.  Express these in a special Sun-orbit frame to      */
/* .. determine actuator capacity requirements.                       */
void EnvTrq(struct SCType *S)
{
      long Ib,i;
      double S1[3],S2[3],S3[3],CSN[3][3];
      double rxF[3],TrqN[3],SumTrqN[3],TrqS[3];
      double TrqB[3],Hn[3],Hb[3];
      struct EnvTrqType *E;
      char envfilename[40];

      E = &S->EnvTrq;

      if (E->First) {
         E->First = 0;
         sprintf(envfilename,"EnvTrq%02li.42",S->Tag);
         E->envfile = FileOpen(InOutPath,envfilename,"w");
      }

      /* Define S frame: s3 is orbit normal, s1 is orbit noon */
      VxV(S->PosN,S->VelN,S3);
      UNITV(S3);
      VxV(S3,S->svn,S2);
      UNITV(S2);
      VxV(S2,S3,S1);
      for (i=0;i<3;i++) {
         CSN[0][i] = S1[i];
         CSN[1][i] = S2[i];
         CSN[2][i] = S3[i];
         SumTrqN[i] = 0.0;
      }

      for(Ib=0;Ib<S->Nb;Ib++) {
         MTxV(S->B[Ib].CN,S->B[Ib].Trq,TrqN);
         VxV(S->B[Ib].pn,S->B[Ib].Frc,rxF);
         for(i=0;i<3;i++) SumTrqN[i] += TrqN[i] + rxF[i];
      }
      MxV(CSN,SumTrqN,TrqS);

      for (i=0;i<3;i++) E->Hs[i] += TrqS[i]*DTSIM;

      if (OutFlag) {
         /* Express Trq, H in B0 frame */
         MxV(S->B[0].CN,SumTrqN,TrqB);
         MTxV(CSN,E->Hs,Hn);
         MxV(S->B[0].CN,Hn,Hb);
         fprintf(E->envfile,"%le %le %le %le %le %le %le %le %le %le %le %le\n",
                 TrqS[0],TrqS[1],TrqS[2],
                 E->Hs[0],E->Hs[1],E->Hs[2],
                 TrqB[0],TrqB[1],TrqB[2],
                 Hb[0],Hb[1],Hb[2]);
      }

}
/**********************************************************************/
/*  This file contains perturbation torque and force models to apply  */
/*  as desired to each spacecraft.                                    */
/*  Remember that torques are expressed in the Body frame, but forces */
/*  are expressed in the N frame.                                     */

void Perturbations(struct SCType *S)
{
      long Ib;


/* .. Zero forces and torques */
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

/* .. Gravity-Gradient Torques */
      if (GGActive) GravGradFrcTrq(S);

/* .. Gravity Perturbation Forces */
      if (GravPertActive) GravPertForce(S);

/* .. Aerodynamic Forces and Torques */
      if (AeroActive) AeroFrcTrq(S);

/* .. Solar Radiation Pressure Forces and Torques */
      if (SolPressActive) SolPressFrcTrq(S);

/* .. Reaction Wheel Static and Dynamic Imbalance Torques */
      if (RwaImbalanceActive) RwaImbalance(S);

/* .. Contact Forces and Torques */
      if (ContactActive) ContactFrcTrq(S);

/* .. Find Momentum Accumulation for Actuator Sizing */
      if (ComputeEnvTrq) EnvTrq(S);

}

//#ifdef __cplusplus
//}
//#endif

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
      if (O->Type == ORB_CENTRAL) {
         OrbCenter = O->center;
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
      if (O->Type == ORB_THREE_BODY) {
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

      OrbCenter = Orb[S->RefOrb].center;

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

/* .. Find Momentum Accumulation for Actuator Sizing */
      if (ComputeEnvTrq) EnvTrq(S);

}

//#ifdef __cplusplus
//}
//#endif

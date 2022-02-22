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
void PassiveJointFrcTrq(double *u, double *x, struct SCType *S)
{
      long Ig,i;
      struct JointType *G;
      double a[3];

      for(Ig=0;Ig<S->Ng;Ig++) {
         G = &S->G[Ig];
         if (G->IsSpherical) {
            Q2AngleVec(&x[G->Rotx0],a);
            for(i=0;i<3;i++) {
               G->PassiveTrq[i] = -G->RotDampCoef[i]*u[G->Rotu0+i]
                                 - G->RotSpringCoef[i]*a[i];
            }
         }
         else {
            for(i=0;i<G->RotDOF;i++) {
               G->PassiveTrq[i] = -G->RotDampCoef[i]*u[G->Rotu0+i]
                                  -G->RotSpringCoef[i]*x[G->Rotx0+i];
            }
         }

         for(i=0;i<G->TrnDOF;i++) {
            G->PassiveFrc[i] = -G->TrnDampCoef[i]*u[G->Trnu0+i]
                               -G->TrnSpringCoef[i]*x[G->Trnx0+i];
         }

      }
}
/**********************************************************************/
/*  Adjust body positions and velocities so that they are related to  */
/*  those of the spacecraft center of mass.                           */
void MotionConstraints(struct SCType *S)
{
      struct BodyType *B;
      double pcm[3],vcm[3];
      long Ib,i;

      /* Constrain Sum(mass*pn = 0.0), Sum(mass*vn = 0.0) */
      for(i=0;i<3;i++) {
         pcm[i] = 0.0;
         vcm[i] = 0.0;
      }
      S->mass = 0.0;
      for(Ib=0;Ib<S->Nb;Ib++) {
         B = &S->B[Ib];
         for(i=0;i<3;i++) {
            pcm[i] += B->mass*B->pn[i];
            vcm[i] += B->mass*B->vn[i];
         }
         S->mass += B->mass;
      }
      for(i=0;i<3;i++) {
         pcm[i] /= S->mass;
         vcm[i] /= S->mass;
      }
      for(Ib=0;Ib<S->Nb;Ib++) {
         B = &S->B[Ib];
         for(i=0;i<3;i++) {
            B->pn[i] -= pcm[i];
            B->vn[i] -= vcm[i];
         }
      }
      /* Adjust Dyn States corresponding to B[0].vn, B[0].pn */
      for(i=0;i<3;i++) {
         S->Dyn.u[S->Dyn.Nu-3+i] = S->B[0].vn[i];
         S->Dyn.x[S->Dyn.Nx-3+i] = S->B[0].pn[i];
      }
}
/**********************************************************************/
/*  Given body locations, attitudes, and mass properties,             */
/*  find SC mass center and inertia matrix.                           */
void SCMassProps(struct SCType *S)
{
      struct BodyType *B0,*B;
      double pnb[3];
      double p2,p[3],pp[3][3],CI0[3][3],MOI[3][3];
      long i,j,Ib;

      B0 = &S->B[0];

      /* Locate SC.cm wrt B0 origin */
      MxV(B0->CN,B0->pn,pnb);
      for(i=0;i<3;i++) S->cm[i] = B0->cm[i]-pnb[i];

      /* Compute composite inertia matrix, SC.I */
      for(i=0;i<3;i++) {
         for(j=0;j<3;j++) S->I[i][j] = B0->I[i][j];
      }
      MxV(B0->CN,B0->pn,p);
      p2 = VoV(p,p);
      for(i=0;i<3;i++) {
         for(j=0;j<3;j++) {
            pp[i][j]=-p[i]*p[j];
         }
         pp[i][i]+=p2;
      }
      for(i=0;i<3;i++) {
         for(j=0;j<3;j++) S->I[i][j] += B0->mass*pp[i][j];
      }
      for(Ib=1;Ib<S->Nb;Ib++) {
         B = &S->B[Ib];
         MxMT(B->CN,B0->CN,CI0);
         MxV(B0->CN,B->pn,p);
         PARAXIS(B->I,CI0,B->mass,p,MOI);
         for(i=0;i<3;i++) {
            for(j=0;j<3;j++) S->I[i][j] += MOI[i][j];
         }
      }
}
/**********************************************************************/
void MapJointStatesToStateVector(struct SCType *S)
{
      double CGoGi[3][3],qgogi[4];
      long i,j,Ig;
      struct JointType *G;
      struct DynType *D;

      D = &S->Dyn;

      /* Map in state variables */
      for(i=0;i<3;i++) {
         D->u[i] = S->B[0].wn[i];
         D->u[D->Nu-3+i] = S->B[0].vn[i];
         D->x[D->Nx-3+i] = S->B[0].pn[i];
      }
      for(i=0;i<4;i++) D->x[i] = S->B[0].qn[i];

      for(Ig=0;Ig<S->Ng;Ig++) {
         G = &S->G[Ig];
         if (G->IsSpherical) {
            A2C(G->RotSeq,G->Ang[0],G->Ang[1],G->Ang[2],CGoGi);
            C2Q(CGoGi,qgogi);
            for(i=0;i<3;i++) D->u[G->Rotu0+i] = G->AngRate[i];
            for(i=0;i<4;i++) D->x[G->Rotx0+i] = qgogi[i];
         }
         else {
            for(i=0;i<G->RotDOF;i++) {
               D->u[G->Rotu0+i] = G->AngRate[i];
               D->x[G->Rotx0+i] = G->Ang[i];
            }
         }
         for (i=0; i<G->TrnDOF;i++) {
            D->u[G->Trnu0+i] = G->PosRate[i];
            D->x[G->Trnx0+i] = G->Pos[i];
         }
         A2C(G->RotSeq,G->Ang[0],G->Ang[1],G->Ang[2],G->CGoGi);
         JointPartials(TRUE,G->IsSpherical,G->RotSeq,G->TrnSeq,
            G->Ang,G->AngRate,G->Gamma,G->Gs,G->Gds,
            G->PosRate,G->Delta,G->Ds,G->Dds);
         for(i=0;i<3;i++) {
            G->PassiveTrq[i] = 0.0;
            G->PassiveFrc[i] = 0.0;
         }
         /* CTrqBo is constant for rigid body dynamics */
         /* It gets overwritten for flex */
         for(i=0;i<3;i++) {
            for(j=0;j<3;j++) {
               G->CTrqBo[i][j] = G->CBoGo[j][i];
            }
         }
      }

      for(i=0;i<S->Nw;i++) D->h[i] = S->Whl[i].H;


}
/**********************************************************************/
void MapStateVectorToBodyStates(double *u, double *x, double *uf,
   double *xf, struct SCType *S)
{
      double wi[3],ri[3],ro[3],wxr[3],wxri[3],wxro[3];
      double xg[3];
      double CBfiBi[3][3],CBfoBo[3][3],CGoBfi[3][3];
      double wgon[3],wo[3],fvi[3],fvo[3];
      double qfi[4],qfo[4];
      double vg[3],vgb[3],vgn[3];
      struct BodyType *Bi, *Bo, *B;
      struct JointType *G;
      struct FlexNodeType *FN;
      long Ig,i,j,Nu,Nx,Ng,If,Ib,In;

      Nu = S->Dyn.Nu;
      Nx = S->Dyn.Nx;
      Ng = S->Ng;
      UNITQ(&x[0]);
      for(i=0;i<3;i++) {
         S->B[0].wn[i] = u[i];
         S->B[0].qn[i] = x[i];
         S->B[0].vn[i] = u[Nu-3+i];
         S->B[0].pn[i] = x[Nx-3+i];
      }
      S->B[0].qn[3] = x[3];
      Q2C(S->B[0].qn,S->B[0].CN);

      if (S->FlexActive) {
         for(Ib=0;Ib<S->Nb;Ib++) {
            B = &S->B[Ib];
            for(If=0;If<B->Nf;If++) {
               B->xi[If] = uf[B->f0+If];
               B->eta[If] = xf[B->f0+If];
            }
            for(In=0;In<B->NumFlexNodes;In++) {
               FN = &B->FlexNode[In];
               for(i=0;i<3;i++) {
                  FN->pos[i] = 0.0;
                  FN->vel[i] = 0.0;
                  FN->ang[i] = 0.0;
                  FN->angrate[i] = 0.0;
                  for(If=0;If<B->Nf;If++) {
                     FN->pos[i] += FN->PSI[i][If]*B->eta[If];
                     FN->vel[i] += FN->PSI[i][If]*B->xi[If];
                     FN->ang[i] += FN->THETA[i][If]*B->eta[If];
                     FN->angrate[i] += FN->THETA[i][If]*B->xi[If];
                  }
                  FN->TotAngVel[i] = B->wn[i] + FN->angrate[i];
                  /* TODO: FN->TotTrnAcc = ... */
               }
            }
         }
      }

      for(Ig=0;Ig<Ng;Ig++) {
         G = &S->G[Ig];
         Bi = &S->B[G->Bin];
         Bo = &S->B[G->Bout];
         if (G->IsSpherical) {
            UNITQ(&x[G->Rotx0]);
            for(i=0;i<3;i++) {
               G->AngRate[i] = u[G->Rotu0+i];
            }
            Q2C(&x[G->Rotx0],G->CGoGi);
            C2A(G->RotSeq,G->CGoGi,&G->Ang[0],&G->Ang[1],&G->Ang[2]);
         }
         else {
            for(i=0;i<G->RotDOF;i++) {
               G->AngRate[i] = u[G->Rotu0+i];
               while (x[G->Rotx0+i] >  Pi) x[G->Rotx0+i] -= TwoPi;
               while (x[G->Rotx0+i] < -Pi) x[G->Rotx0+i] += TwoPi;
               G->Ang[i] = x[G->Rotx0+i];
            }
            if (G->RotDOF == 3) {
               if (fabs(G->Ang[1]) > 1.5533) {
                  printf("Warning:  Joint %ld is near gimbal lock.\n",Ig);
               }
            }
            A2C(G->RotSeq,G->Ang[0],G->Ang[1],G->Ang[2],G->CGoGi);
         }
         for(i=0;i<G->TrnDOF;i++) {
            G->PosRate[i] = u[G->Trnu0+i];
            G->Pos[i] = x[G->Trnx0+i];
         }
         if (S->FlexActive) {
            /* Flex */
            for(i=0;i<3;i++) {
               G->FlexPosi[i] = 0.0;
               G->FlexVeli[i] = 0.0;
               G->FlexAngi[i] = 0.0;
               G->FlexAngVeli[i] = 0.0;
               for(If=0;If<Bi->Nf;If++) {
                  G->FlexPosi[i] += G->PSIi[i][If]*Bi->eta[If];
                  G->FlexVeli[i] += G->PSIi[i][If]*Bi->xi[If];
                  G->FlexAngi[i] += G->THETAi[i][If]*Bi->eta[If];
                  G->FlexAngVeli[i] += G->THETAi[i][If]*Bi->xi[If];
               }
               G->FlexPoso[i] = 0.0;
               G->FlexVelo[i] = 0.0;
               G->FlexAngo[i] = 0.0;
               G->FlexAngVelo[i] = 0.0;
               for(If=0;If<Bo->Nf;If++) {
                  G->FlexPoso[i] += G->PSIo[i][If]*Bo->eta[If];
                  G->FlexVelo[i] += G->PSIo[i][If]*Bo->xi[If];
                  G->FlexAngo[i] += G->THETAo[i][If]*Bo->eta[If];
                  G->FlexAngVelo[i] += G->THETAo[i][If]*Bo->xi[If];
               }
            }
            /* CN, qn */
            qfi[3] = 1.0;
            qfo[3] = 1.0;
            for(i=0;i<3;i++) {
               qfi[i] = sin(0.5*G->FlexAngi[i]);
               qfo[i] = sin(0.5*G->FlexAngo[i]);
               qfi[3] -= qfi[i]*qfi[i];
               qfo[3] -= qfo[i]*qfo[i];
            }
            qfi[3] = sqrt(qfi[3]);
            qfo[3] = sqrt(qfo[3]);
            Q2C(qfi,CBfiBi);
            Q2C(qfo,CBfoBo);
            MTxM(G->CBoGo,CBfoBo,G->CTrqBo);
            MxM(G->CGoGi,G->CGiBi,CGoBfi);
            MxM(CGoBfi,CBfiBi,G->CTrqBi);
         }
         else {
            MxM(G->CGoGi,G->CGiBi,G->CTrqBi);
         }
         MTxM(G->CTrqBo,G->CTrqBi,G->COI);
         MxM(G->COI,Bi->CN,Bo->CN);
         C2Q(Bo->CN,Bo->qn);

         /* wn */
         ADOT2W(G->IsSpherical,G->RotSeq,G->Ang,G->AngRate,wgon);
         MxV(G->CBoGo,wgon,Bo->wn);
         if (S->FlexActive) {
            for(i=0;i<3;i++) {
               Bo->wn[i] -= G->FlexAngVelo[i];
               wi[i] = Bi->wn[i] + G->FlexAngVeli[i];
            }
            MxV(G->COI,wi,wo);
         }
         else {
            MxV(G->COI,Bi->wn,wo);
         }
         for(i=0;i<3;i++)  Bo->wn[i] += wo[i];

         /* pn, vn */
         for (i=0;i<3;i++) {
            xg[i] = 0.0;
            vg[i] = 0.0;
            for (j=0;j<G->TrnDOF;j++) {
               xg[i] += G->Delta[i][j]*G->Pos[j];
               vg[i] += G->Delta[i][j]*G->PosRate[j];
            }
         }
         MTxV(G->CGiBi,xg,G->xb);
         for(i=0;i<3;i++) {
            G->ri[i] = G->RigidRin[i] + G->xb[i];
            G->ro[i] = G->RigidRout[i];
         }
         MTxV(Bi->CN,G->xb,G->xn);
         MTxV(Bi->CN,G->ri,ri);
         MTxV(Bo->CN,G->ro,ro);
         for(i=0;i<3;i++) Bo->pn[i] = Bi->pn[i] + ri[i] - ro[i];
         /* vn */
         VxV(Bi->wn,G->ri,wxr);
         MTxV(Bi->CN,wxr,wxri);
         VxV(Bo->wn,G->ro,wxr);
         MTxV(Bo->CN,wxr,wxro);
         MTxV(G->CGiBi,vg,vgb);
         MTxV(Bi->CN,vgb,vgn);
         for(i=0;i<3;i++)
            Bo->vn[i] = Bi->vn[i] + wxri[i] + vgn[i] - wxro[i];
         if (S->FlexActive) {
            for(i=0;i<3;i++) {
               G->ri[i] += G->FlexPosi[i];
               G->ro[i] += G->FlexPoso[i];
            }
            MTxV(Bi->CN,G->FlexVeli,fvi);
            MTxV(Bo->CN,G->FlexVelo,fvo);
            for(i=0;i<3;i++)
               Bo->vn[i] += fvi[i] - fvo[i];
         }
      }
}
/**********************************************************************/
void FindTotalAngMom(struct SCType *S) {

      struct BodyType *B;
      struct WhlType *W;
      double Hb[3],Hn[3],mv[3],rxmv[3],Hwn[3];
      double Hwb[3] = {0.0,0.0,0.0};
      long Ib,i,Iwhl;

      /* Zero */
      for(i=0;i<3;i++) {
         S->Hvn[i] = 0.0;
         S->Hvb[i] = 0.0;
      }

      /* Bodies */
      for(Ib=0;Ib<S->Nb;Ib++) {
         B = &S->B[Ib];
         MxV(B->I,B->wn,Hb);
         for(i=0;i<3;i++) Hb[i] += B->EmbeddedMom[i];
         MTxV(B->CN,Hb,Hn);
         SxV(B->mass,B->vn,mv);
         VxV(B->pn,mv,rxmv);
         for(i=0;i<3;i++) S->Hvn[i] += Hn[i] + rxmv[i];
      }

      /* Wheels */
      for(Iwhl=0;Iwhl<S->Nw;Iwhl++) {
         W = &S->Whl[Iwhl];
         for(i=0;i<3;i++) Hwb[i] += W->A[i]*W->H;
         MTxV(S->B[W->Body].CN,Hwb,Hwn);
         for(i=0;i<3;i++) S->Hvn[i] += Hwn[i];
      }

      /* Express in B[0] frame */
      MxV(S->B[0].CN,S->Hvn,S->Hvb);
}
/**********************************************************************/
double FindTotalKineticEnergy(struct SCType *S)
{
      struct BodyType *B;
      struct WhlType *W;
      double Iw[3],mv[3];
      double KE = 0.0;
      long Ib,Iwhl;

      for(Ib=0;Ib<S->Nb;Ib++) {
         B = &S->B[Ib];
         MxV(B->I,B->wn,Iw);
         SxV(B->mass,B->vn,mv);
         KE += 0.5*(VoV(B->wn,Iw)+VoV(B->vn,mv));
      }

      for(Iwhl=0;Iwhl<S->Nw;Iwhl++) {
         W = &S->Whl[Iwhl];
         KE += 0.5*W->w*W->J*W->w;
      }

      if (Orb[S->RefOrb].Regime == ORB_ZERO) {
          KE += 0.5*S->mass*VoV(S->VelN,S->VelN);
      }
      else if (Orb[S->RefOrb].Regime == ORB_FLIGHT) {
          KE += 0.5*S->mass*VoV(S->VelR,S->VelR);
      }

      return(KE);
}
/**********************************************************************/
void FindBodyPathDCMs(struct SCType *S)
{
      struct DynType *D;
      struct JointType *G;
      long Ig,i,j,Bo,Bi,Gi;

      D = &S->Dyn;

      for(Ig=0;Ig<S->Ng;Ig++) {
         G = &S->G[Ig];
         Bo = G->Bout;
         Bi = G->Bin;
         for(i=0;i<3;i++) {
            for(j=0;j<3;j++)
               D->BodyPathTable[Bo][Bi].Coi[i][j] = G->COI[i][j];
         }
         while (Bi != 0) {
            Gi = S->B[Bi].Gin;
            Bi = S->G[Gi].Bin;
            MxMT(S->B[Bo].CN,S->B[Bi].CN,D->BodyPathTable[Bo][Bi].Coi);
         }
      }
}
/**********************************************************************/
/* TODO:  Could probably be optimized */
void FindPathVectors(struct SCType *S)
{
      struct DynType *D;
      struct JointType *G;
      double ri[3],ro[3];
      long Ig,Jg,Ia,i,Bi,Bo;

      D = &S->Dyn;

      for(Ig=0;Ig<S->Ng;Ig++) {
         G = &S->G[Ig];
         Bi = G->Bin;
         Bo = G->Bout;
         MTxV(S->B[Bi].CN,G->ri,ri);
         MTxV(S->B[Bo].CN,G->ro,ro);
         for(i=0;i<3;i++) {
            S->B[Bo].beta[i] = S->B[Bi].beta[i] + ro[i] - ri[i];
         }
         #if 0
         for(Jg=0;Jg<Ig;Jg++) {
            if (D->JointPathTable[Bo][Jg].InPath) {
               for(i=0;i<3;i++)
                  D->JointPathTable[Bo][Jg].rho[i] =
                  D->JointPathTable[Bi][Jg].rho[i] + ro[i] - ri[i];
            }
         }
         #else
         for(Ia=0;Ia<G->Nanc;Ia++) {
            Jg = G->Anc[Ia];
            for(i=0;i<3;i++)
               D->JointPathTable[Bo][Jg].rho[i] =
               D->JointPathTable[Bi][Jg].rho[i] + ro[i] - ri[i];
         }
         #endif
         for(i=0;i<3;i++) D->JointPathTable[Bo][Ig].rho[i] = ro[i];
      }
}
/**********************************************************************/
/*  PAngVel and IPAngVel                                              */
void FindPAngVel(struct SCType *S)
{
      struct DynType *D;
      struct BodyType *Bib;
      struct JointType *G;
      double CGo[3][3],CG[3][3],IC[3][3];
      long Ib,i,j,k,i0,j0;
      long Jb,Ig;

      D = &S->Dyn;

      /* PAngVel and IPAngVel */
      for(Ib=1;Ib<S->Nb;Ib++) {
         Ig = S->B[Ib].Gin;
         Bib = &S->B[Ib];
         G = &S->G[Ig];
         i0 = 3*Ib;
         j0 = G->Rotu0;
         for(i=0;i<3;i++) {
            for(j=0;j<G->RotDOF;j++) {
               CGo[i][j] = 0.0;
               for(k=0;k<3;k++) {
                  CGo[i][j] += G->CTrqBo[k][i]*G->Gamma[k][j];
               }
            }
         }
         for(i=0;i<3;i++) {
            for(j=0;j<G->RotDOF;j++) {
               D->PAngVel[i0+i][j0+j] = CGo[i][j];
               D->IPAngVel[i0+i][j0+j] = 0.0;
               for(k=0;k<3;k++) {
                  D->IPAngVel[i0+i][j0+j] += Bib->I[i][k]*CGo[k][j];
               }
            }
         }
         Jb = G->Bin;
         while(Jb > 0) {
            Ig = S->B[Jb].Gin;
            G = &S->G[Ig];
            j0 = G->Rotu0;
            for(i=0;i<3;i++) {
               for(j=0;j<G->RotDOF;j++) {
                  CGo[i][j] = 0.0;
                  for(k=0;k<3;k++) {
                     CGo[i][j] += G->CTrqBo[k][i]*G->Gamma[k][j];
                  }
               }
            }
            for(i=0;i<3;i++) {
               for(j=0;j<G->RotDOF;j++) {
                  CG[i][j] = 0.0;
                  for(k=0;k<3;k++) {
                     CG[i][j] += D->BodyPathTable[Ib][Jb].Coi[i][k]*CGo[k][j];
                  }
               }
            }
            for(i=0;i<3;i++) {
               for(j=0;j<G->RotDOF;j++) {
                  D->PAngVel[i0+i][j0+j] = CG[i][j];
                  D->IPAngVel[i0+i][j0+j] = 0.0;
                  for(k=0;k<3;k++) {
                     D->IPAngVel[i0+i][j0+j] += Bib->I[i][k]*CG[k][j];
                  }
               }
            }
            Jb = G->Bin;
         }
         MxM(Bib->I,D->BodyPathTable[Ib][0].Coi,IC);
         for(i=0;i<3;i++) {
            for(j=0;j<3;j++) {
               D->PAngVel[i0+i][j] = D->BodyPathTable[Ib][0].Coi[i][j];
               D->IPAngVel[i0+i][j] = IC[i][j];
            }
         }
      }
}
/**********************************************************************/
/* PVel and mPVel                                                     */
void FindPVel(struct SCType *S)
{
      struct DynType *D;
      struct BodyType *Bib,*Bjb;
      struct JointType *G;
      double RC[3][3],BC[3][3];
      double RCB[3][3],RCG[3][3];
      double CNG[3][3],CD[3][3];
      double m;
      long Ib,Jb,Ig,i,j,k,i0,j0;

      D = &S->Dyn;

      /* PVel and mPVel */
      for(Ib=1;Ib<S->Nb;Ib++) {
         Bib = &S->B[Ib];
         i0 = 3*Ib;
         m = Bib->mass;
         Jb = Ib;
         while (Jb > 0) {
            Bjb = &S->B[Jb];
            Ig = Bjb->Gin;
            G = &S->G[Ig];
            /* Rotation */
            j0 = G->Rotu0;
            VcrossMT(D->JointPathTable[Ib][Ig].rho,Bjb->CN,RCB);
            MxMT(RCB,G->CTrqBo,RC);
            for(i=0;i<3;i++) {
               for(j=0;j<G->RotDOF;j++) {
                  RCG[i][j] = 0.0;
                  for(k=0;k<3;k++) {
                     RCG[i][j] += RC[i][k]*G->Gamma[k][j];
                  }
               }
            }
            for(i=0;i<3;i++) {
               for(j=0;j<G->RotDOF;j++) {
                  D->PVel[i0+i][j0+j] = RCG[i][j];
                  D->mPVel[i0+i][j0+j] = m*RCG[i][j];
               }
            }
            /* Translation */
            j0 = G->Trnu0;
            MTxMT(S->B[G->Bin].CN,G->CGiBi,CNG);
            for(i=0;i<3;i++) {
               for(j=0;j<3;j++) CD[i][j] = 0.0;
               for(j=0;j<G->TrnDOF;j++) {
                  for(k=0;k<3;k++) {
                     CD[i][j] += CNG[i][k]*G->Delta[k][j];
                  }
               }
            }
            for(i=0;i<3;i++) {
               for(j=0;j<G->TrnDOF;j++) {
                  D->PVel[i0+i][j0+j] = CD[i][j];
                  D->mPVel[i0+i][j0+j] = m*CD[i][j];
               }
            }
            Jb = G->Bin;
         }
         /* First Column */
         VcrossMT(Bib->beta,S->B[0].CN,BC);
         for(i=0;i<3;i++) {
            for(j=0;j<3;j++) {
               D->PVel[i0+i][j] = BC[i][j];
               D->mPVel[i0+i][j] = m*BC[i][j];
            }
         }
      }
      /* Last Column populated in InitRigidDyn */
}
/**********************************************************************/
/*  PAngVelf and IPAngVelf                                            */
void FindPAngVelf(struct SCType *S)
{
      struct DynType *D;
      struct BodyType *Bib,*Bjb;
      struct JointType *Gi,*Go;
      long Ib,i,j,k,i0,j0;
      long Jb,Gin;

      D = &S->Dyn;

      /* PAngVelf and IPAngVelf */
      for(Ib=1;Ib<S->Nb;Ib++) {
         i0 = 3*Ib;
         Bib = &S->B[Ib];
         Gin = Bib->Gin;
         Gi = &S->G[Gin];
         j0 = Bib->f0;
         for(i=0;i<3;i++) {
            for(j=0;j<Bib->Nf;j++) {
               D->PAngVelf[i0+i][j0+j] = -Gi->THETAo[i][j];
            }
         }
         for(i=0;i<3;i++) {
            for(j=0;j<Bib->Nf;j++) {
               D->IPAngVelf[i0+i][j0+j] = 0.0;
               for(k=0;k<3;k++)
                  D->IPAngVelf[i0+i][j0+j] += Bib->I[i][k]*D->PAngVelf[i0+k][j0+j];
            }
         }
         Jb = Gi->Bin;
         while (Jb > 0) {
            Go = Gi;
            Bjb = &S->B[Jb];
            Gin = Bjb->Gin;
            Gi = &S->G[Gin];
            j0 = Bjb->f0;
            for(i=0;i<3;i++) {
               for(j=0;j<Bjb->Nf;j++) {
                  D->PAngVelf[i0+i][j0+j] = 0.0;
                  for(k=0;k<3;k++) {
                     D->PAngVelf[i0+i][j0+j] +=
                        D->BodyPathTable[Ib][Jb].Coi[i][k]*
                        (Go->THETAi[k][j]-Gi->THETAo[k][j]);
                  }
               }
            }
            for(i=0;i<3;i++) {
               for(j=0;j<Bjb->Nf;j++) {
                  D->IPAngVelf[i0+i][j0+j] = 0.0;
                  for(k=0;k<3;k++) {
                     D->IPAngVelf[i0+i][j0+j] +=
                        Bib->I[i][k]*D->PAngVelf[i0+k][j0+j];
                  }
               }
            }
            Jb = Gi->Bin;
         }
         Bjb = &S->B[0];
         for(i=0;i<3;i++) {
            for(j=0;j<Bjb->Nf;j++) {
               D->PAngVelf[i0+i][j] = 0.0;
               for(k=0;k<3;k++) {
                  D->PAngVelf[i0+i][j] +=
                     D->BodyPathTable[Ib][0].Coi[i][k]*Gi->THETAi[k][j];
               }
            }
         }
         for(i=0;i<3;i++) {
            for(j=0;j<Bjb->Nf;j++) {
               D->IPAngVelf[i0+i][j] = 0.0;
               for(k=0;k<3;k++) {
                  D->IPAngVelf[i0+i][j] +=
                     Bib->I[i][k]*D->PAngVelf[i0+k][j];
               }
            }
         }
      }
}
/**********************************************************************/
/* PVelf and mPVelf                                                   */
void FindPVelf(struct SCType *S)
{
      struct DynType *D;
      struct BodyType *Bib,*Bjb;
      struct JointType *Gi,*Go;
      double RCi[3][3],RCo[3][3],m;
      long Ib,Jb,Gin,Gout,i,j,k,i0,j0;

      D = &S->Dyn;

      /* PVelf */
      for(Ib=1;Ib<S->Nb;Ib++) {
         i0 = 3*Ib;
         Bib = &S->B[Ib];
         j0 = Bib->f0;
         m = Bib->mass;
         Gin = Bib->Gin;
         Gi = &S->G[Gin];
         VcrossMT(D->JointPathTable[Ib][Gin].rho,Bib->CN,RCi);
         for(i=0;i<3;i++) {
            for(j=0;j<Bib->Nf;j++) {
               D->PVelf[i0+i][j0+j] = 0.0;
               for(k=0;k<3;k++) {
                  D->PVelf[i0+i][j0+j] -=
                     Bib->CN[k][i]*Gi->PSIo[k][j] +
                     RCi[i][k]*Gi->THETAo[k][j];
               }
               D->mPVelf[i0+i][j0+j] = m*D->PVelf[i0+i][j0+j];
            }
         }
         Jb = Gi->Bin;

         while (Jb > 0) {
            Bjb = &S->B[Jb];
            j0 = Bjb->f0;
            Gout = Gin;
            Go = Gi;
            Gin = Bjb->Gin;
            Gi = &S->G[Gin];
            VcrossMT(D->JointPathTable[Ib][Gin].rho,Bjb->CN,RCi);
            VcrossMT(D->JointPathTable[Ib][Gout].rho,Bjb->CN,RCo);
            for(i=0;i<3;i++) {
               for(j=0;j<Bjb->Nf;j++) {
                  D->PVelf[i0+i][j0+j] = 0.0;
                  for(k=0;k<3;k++) {
                     D->PVelf[i0+i][j0+j] +=
                        Bjb->CN[k][i]*(Go->PSIi[k][j]-Gi->PSIo[k][j])
                        + RCo[i][k]*Go->THETAi[k][j]
                        - RCi[i][k]*Gi->THETAo[k][j];
                  }
                  D->mPVelf[i0+i][j0+j] = m*D->PVelf[i0+i][j0+j];
               }
            }
            Jb = Gi->Bin;
         }

         Bjb = &S->B[0];
         VcrossMT(D->JointPathTable[Ib][Gin].rho,Bjb->CN,RCi);
         for(i=0;i<3;i++) {
            for(j=0;j<Bjb->Nf;j++) {
               D->PVelf[i0+i][j] = 0.0;
               for(k=0;k<3;k++) {
                  D->PVelf[i0+i][j] +=
                     Bjb->CN[k][i]*Gi->PSIi[k][j]
                     + RCi[i][k]*Gi->THETAi[k][j];
               }
               D->mPVelf[i0+i][j] = m*D->PVelf[i0+i][j];
            }
         }

      }
}
/**********************************************************************/
/* Add (c+Pf*eta)xPVel to IPAngVel                                    */
void AugmentIPAngVel(struct SCType *S)
{
      struct DynType *D;
      struct BodyType *Bib,*Bjb;
      struct JointType *G;
      double cplusPetaN[3][3];
      long Ib,i,j,k,i0,j0;
      long Jb,Ig;

      D = &S->Dyn;

      /* IPAngVel */
      for(Ib=1;Ib<S->Nb;Ib++) {
         Ig = S->B[Ib].Gin;
         Bib = &S->B[Ib];
         G = &S->G[Ig];
         i0 = 3*Ib;
         j0 = G->Rotu0;
         MxM(Bib->cplusPeta,Bib->CN,cplusPetaN);
         for(i=0;i<3;i++) {
            for(j=0;j<G->RotDOF;j++) {
               for(k=0;k<3;k++) {
                  D->IPAngVel[i0+i][j0+j] +=
                     cplusPetaN[i][k]*D->PVel[i0+k][j0+j];
               }
            }
         }
         Jb = G->Bin;
         while(Jb > 0) {
            Bjb = &S->B[Jb];
            Ig = Bjb->Gin;
            G = &S->G[Ig];
            j0 = G->Rotu0;
            for(i=0;i<3;i++) {
               for(j=0;j<G->RotDOF;j++) {
                  for(k=0;k<3;k++) {
                     D->IPAngVel[i0+i][j0+j] +=
                        cplusPetaN[i][k]*D->PVel[i0+k][j0+j];
                  }
               }
            }
            j0 = G->Trnu0;
            for(i=0;i<3;i++) {
               for(j=0;j<G->TrnDOF;j++) {
                  for(k=0;k<3;k++) {
                     D->IPAngVel[i0+i][j0+j] +=
                        cplusPetaN[i][k]*D->PVel[i0+k][j0+j];
                  }
               }
            }
            Jb = G->Bin;
         }
         for(i=0;i<3;i++) {
            for(j=0;j<3;j++) {
               for(k=0;k<3;k++) {
                  D->IPAngVel[i0+i][j] +=
                     cplusPetaN[i][k]*D->PVel[i0+k][j];
               }
            }
         }
         /* Last column */
         j0 = D->Nu-3;
         for(i=0;i<3;i++) {
            for(j=0;j<3;j++) {
               D->IPAngVel[i0+i][j0+j] = cplusPetaN[i][j];
            }
         }
      }
}
/**********************************************************************/
/* Add -(c+Pf*eta)xPAngVel to mPVel                                   */
void AugmentMPVel(struct SCType *S)
{
      struct DynType *D;
      struct BodyType *Bib,*Bjb;
      struct JointType *G;
      double CcplusPeta[3][3];
      long Ib,Jb,Ig,i,j,k,i0,j0;

      D = &S->Dyn;

      /* mPVel */
      for(Ib=1;Ib<S->Nb;Ib++) {
         Bib = &S->B[Ib];
         i0 = 3*Ib;
         Jb = Ib;
         MTxM(Bib->CN,Bib->cplusPeta,CcplusPeta);
         while (Jb > 0) {
            Bjb = &S->B[Jb];
            Ig = Bjb->Gin;
            G = &S->G[Ig];
            j0 = G->Rotu0;
            for(i=0;i<3;i++) {
               for(j=0;j<G->RotDOF;j++) {
                  for(k=0;k<3;k++)
                     D->mPVel[i0+i][j0+j] -=
                        CcplusPeta[i][k]*D->PAngVel[i0+k][j0+j];
               }
            }
            Jb = G->Bin;
         }
         /* First Column */
         for(i=0;i<3;i++) {
            for(j=0;j<3;j++) {
               for(k=0;k<3;k++)
                  D->mPVel[i0+i][j] -= CcplusPeta[i][k]*D->PAngVel[i0+k][j];
            }
         }
      }
}
/**********************************************************************/
/* Add (c+Pf*eta)xPVelf + (Hf+Qf*eta) to IPAngVelf                    */
void AugmentIPAngVelf(struct SCType *S)
{
      struct DynType *D;
      struct BodyType *Bib,*Bjb;
      struct JointType *Gi;
      double cplusPetaN[3][3];
      long Ib,i,j,k,i0,j0;
      long Jb,Gin;

      D = &S->Dyn;

      /* Add (Hf+Qf*eta) */
      for(Ib=0;Ib<S->Nb;Ib++) {
         i0 = 3*Ib;
         Bib = &S->B[Ib];
         j0 = Bib->f0;
         for(i=0;i<3;i++) {
            for(j=0;j<Bib->Nf;j++) {
               D->IPAngVelf[i0+i][j0+j] += Bib->HplusQeta[i][j];
            }
         }
      }

      if (S->RefPt == REFPT_JOINT) {
         /* Add (c+Pf*eta)xPVelf */
         for(Ib=1;Ib<S->Nb;Ib++) {
            i0 = 3*Ib;
            Bib = &S->B[Ib];
            Gin = Bib->Gin;
            Gi = &S->G[Gin];
            j0 = Bib->f0;
            MxM(Bib->cplusPeta,Bib->CN,cplusPetaN);
            for(i=0;i<3;i++) {
               for(j=0;j<Bib->Nf;j++) {
                  D->IPAngVelf[i0+i][j0+j] += Bib->HplusQeta[i][j];
                  for(k=0;k<3;k++) {
                     D->IPAngVelf[i0+i][j0+j] +=
                        cplusPetaN[i][k]*D->PVelf[i0+k][j0+j];
                  }
               }
            }

            Jb = Gi->Bin;
            while (Jb > 0) {
               Bjb = &S->B[Jb];
               Gin = Bjb->Gin;
               Gi = &S->G[Gin];
               j0 = Bjb->f0;
               for(i=0;i<3;i++) {
                  for(j=0;j<Bjb->Nf;j++) {
                     for(k=0;k<3;k++) {
                        D->IPAngVelf[i0+i][j0+j] +=
                           cplusPetaN[i][k]*D->PVelf[i0+k][j0+j];
                     }
                  }
               }
               Jb = Gi->Bin;
            }

            Bjb = &S->B[0];
            for(i=0;i<3;i++) {
               for(j=0;j<Bjb->Nf;j++) {
                  for(k=0;k<3;k++) {
                     D->IPAngVelf[i0+i][j] +=
                        cplusPetaN[i][k]*D->PVelf[i0+k][j];
                  }
               }
            }
         }
      }
}
/**********************************************************************/
/* Add -[(c+Pf*eta)xPAngVelf - Pf] to mPVelf                          */
void AugmentMPVelf(struct SCType *S)
{
      struct DynType *D;
      struct BodyType *Bib,*Bjb;
      struct JointType *Gi;
      double CcplusPeta[3][3];
      long Ib,Jb,Gin,i,j,k,i0,j0;
      long Nfi,Nfj;

      D = &S->Dyn;

      /* B[0] sees only Pf term */
      Bib = &S->B[0];
      Nfi = Bib->Nf;
      for(i=0;i<3;i++) {
         for(j=0;j<Nfi;j++) {
            D->mPVelf[i][j] += Bib->CnbP[i][j];
         }
      }

      /* Other bodies see both terms */
      for(Ib=1;Ib<S->Nb;Ib++) {
         i0 = 3*Ib;
         Bib = &S->B[Ib];
         j0 = Bib->f0;
         Gin = Bib->Gin;
         Gi = &S->G[Gin];
         Nfi = Bib->Nf;
         MTxM(Bib->CN,Bib->cplusPeta,CcplusPeta);
         for(i=0;i<3;i++) {
            for(j=0;j<Nfi;j++) {
               D->mPVelf[i0+i][j0+j] += Bib->CnbP[i][j];
               for(k=0;k<3;k++) {
                 D->mPVelf[i0+i][j0+j] -=
                    CcplusPeta[i][k]*D->PAngVelf[i0+k][j0+j];
               }
            }
         }
         Jb = Gi->Bin;

         while (Jb > 0) {
            Bjb = &S->B[Jb];
            j0 = Bjb->f0;
            Gin = Bjb->Gin;
            Gi = &S->G[Gin];
            Nfj = Bjb->Nf;
            for(i=0;i<3;i++) {
               for(j=0;j<Nfj;j++) {
                  for(k=0;k<3;k++) {
                    D->mPVelf[i0+i][j0+j] -=
                       CcplusPeta[i][k]*D->PAngVelf[i0+k][j0+j];
                  }
               }
            }
            Jb = Gi->Bin;
         }

         Bjb = &S->B[0];
         Nfj = Bjb->Nf;
         for(i=0;i<3;i++) {
            for(j=0;j<Nfj;j++) {
               for(k=0;k<3;k++) {
                 D->mPVelf[i0+i][j] -=
                    CcplusPeta[i][k]*D->PAngVelf[i0+k][j];
               }
            }
         }

      }
}
/**********************************************************************/
void FindHplusQetaPAngVelf(struct SCType *S)
{
      struct DynType *D;
      struct BodyType *Bib,*Bjb;
      struct JointType *Gi;
      double **HpQe;
      long Ib,i,j,i0,j0;
      long Jb,Gin;
      long Nfi,Nfj;

      D = &S->Dyn;

      /* (HplusQeta)^T*PAngVelf */
      for(Ib=1;Ib<S->Nb;Ib++) {
         Bib = &S->B[Ib];
         Nfi = Bib->Nf;
         Gin = Bib->Gin;
         Gi = &S->G[Gin];
         i0 = Bib->f0;
         HpQe = Bib->HplusQeta;
         for(i=0;i<Nfi;i++) {
            for(j=0;j<Nfi;j++) {
               D->HplusQetaPAngVelf[i0+i][i0+j] =
                  HpQe[0][i]*D->PAngVelf[3*Ib][i0+j] +
                  HpQe[1][i]*D->PAngVelf[3*Ib+1][i0+j] +
                  HpQe[2][i]*D->PAngVelf[3*Ib+2][i0+j];
            }
         }
         Jb = Gi->Bin;
         while (Jb > 0) {
            Bjb = &S->B[Jb];
            Gin = Bjb->Gin;
            Gi = &S->G[Gin];
            j0 = Bjb->f0;
            Nfj = Bjb->Nf;
            for(i=0;i<Nfi;i++) {
               for(j=0;j<Nfj;j++) {
                  D->HplusQetaPAngVelf[i0+i][j0+j] =
                     HpQe[0][i]*D->PAngVelf[3*Ib][j0+j] +
                     HpQe[1][i]*D->PAngVelf[3*Ib+1][j0+j] +
                     HpQe[2][i]*D->PAngVelf[3*Ib+2][j0+j];
               }
            }
            Jb = Gi->Bin;
         }

         Bjb = &S->B[0];
         Nfj = Bjb->Nf;
         for(i=0;i<Nfi;i++) {
            for(j=0;j<Nfj;j++) {
               D->HplusQetaPAngVelf[i0+i][j] =
                  HpQe[0][i]*D->PAngVelf[3*Ib][j] +
                  HpQe[1][i]*D->PAngVelf[3*Ib+1][j] +
                  HpQe[2][i]*D->PAngVelf[3*Ib+2][j];
            }
         }
      }
}
/**********************************************************************/
/* Compute Pf^T*CBN*PVelf                                             */
void FindPCPVelf(struct SCType *S)
{
      struct DynType *D;
      struct BodyType *Bib,*Bjb;
      struct JointType *Gi;
      double **CP;
      long Ib,Jb,Gin,i,j,i0,j0;
      long Nfi,Nfj;

      D = &S->Dyn;

      /* PCPVelf */
      for(Ib=1;Ib<S->Nb;Ib++) {
         Bib = &S->B[Ib];
         CP = Bib->CnbP;
         i0 = Bib->f0;
         Nfi = Bib->Nf;
         for(i=0;i<Nfi;i++) {
            for(j=0;j<Nfi;j++) {
               D->PCPVelf[i0+i][i0+j] =
                  CP[0][i]*D->PVelf[3*Ib][i0+j] +
                  CP[1][i]*D->PVelf[3*Ib+1][i0+j] +
                  CP[2][i]*D->PVelf[3*Ib+2][i0+j];
            }
         }
         Gin = Bib->Gin;
         Gi = &S->G[Gin];
         Jb = Gi->Bin;

         while (Jb > 0) {
            Bjb = &S->B[Jb];
            Gin = Bjb->Gin;
            Gi = &S->G[Gin];
            j0 = Bjb->f0;
            Nfj = Bjb->Nf;
            for(i=0;i<Nfi;i++) {
               for(j=0;j<Nfj;j++) {
                  D->PCPVelf[i0+i][j0+j] =
                     CP[0][i]*D->PVelf[3*Ib][j0+j] +
                     CP[1][i]*D->PVelf[3*Ib+1][j0+j] +
                     CP[2][i]*D->PVelf[3*Ib+2][j0+j];
               }
            }
            Jb = Gi->Bin;
         }
         Bjb = &S->B[0];
         Nfj = Bjb->Nf;
         for(i=0;i<Nfi;i++) {
            for(j=0;j<Nfj;j++) {
               D->PCPVelf[i0+i][j] =
                  CP[0][i]*D->PVelf[3*Ib][j] +
                  CP[1][i]*D->PVelf[3*Ib+1][j] +
                  CP[2][i]*D->PVelf[3*Ib+2][j];
            }
         }
      }
}
/**********************************************************************/
void FindAlphaR(struct SCType *S)
{
      struct JointType *G;
      struct BodyType *Bi,*Bo;
      double CGs[3],CGds[3];
      double wxGs[3],wxFo[3],wxFi[3],CwxFi[3],CAlphaR[3];
      long Ig,i;

      for(Ig=0;Ig<S->Ng;Ig++) {
         G = &S->G[Ig];
         Bi = &S->B[G->Bin];
         Bo = &S->B[G->Bout];
         MTxV(G->CTrqBo,G->Gs,CGs);
         MTxV(G->CTrqBo,G->Gds,CGds);
         VxV(Bo->wn,CGs,wxGs);
         MxV(G->COI,Bi->AlphaR,CAlphaR);
         for(i=0;i<3;i++)
            Bo->AlphaR[i] = CAlphaR[i] + CGds[i] + wxGs[i];
      }

      if (S->FlexActive) {
         for(Ig=0;Ig<S->Ng;Ig++) {
            G = &S->G[Ig];
            Bi = &S->B[G->Bin];
            Bo = &S->B[G->Bout];
            VxV(Bo->wn,G->FlexAngVelo,wxFo);
            VxV(Bi->wn,G->FlexAngVeli,wxFi);
            MxV(G->COI,wxFi,CwxFi);
            for(i=0;i<3;i++)
               Bo->AlphaR[i] += CwxFi[i] - wxFo[i];
         }
      }
}
/**********************************************************************/
void FindAccR(struct SCType *S)
{
      struct JointType *G;
      struct BodyType *Bi, *Bo;
      double wxr[3],wxwxr[3],Cwri[3],Cwro[3];
      double Dsb[3],wxDsb[3],wxDsn[3];
      double axr[3],Caxri[3],Caxro[3];
      double wxv[3],Cwxvi[3],Cwxvo[3];
      long Ig,i;

      for(Ig=0;Ig<S->Ng;Ig++) {
         G = &S->G[Ig];
         Bi = &S->B[G->Bin];
         Bo = &S->B[G->Bout];

         VxV(Bi->wn,G->ri,wxr);
         VxV(Bi->wn,wxr,wxwxr);
         MTxV(Bi->CN,wxwxr,Cwri);

         VxV(Bo->wn,G->ro,wxr);
         VxV(Bo->wn,wxr,wxwxr);
         MTxV(Bo->CN,wxwxr,Cwro);

         VxV(Bo->AlphaR,G->ro,axr);
         MTxV(Bo->CN,axr,Caxro);

         VxV(Bi->AlphaR,G->ri,axr);
         MTxV(Bi->CN,axr,Caxri);

         MTxV(G->CGiBi,G->Ds,Dsb);
         VxV(Bi->wn,Dsb,wxDsb);
         MTxV(Bi->CN,wxDsb,wxDsn);

         for(i=0;i<3;i++)
            Bo->AccR[i] = Bi->AccR[i] + Cwri[i]  - Cwro[i]
            + Caxri[i] - Caxro[i] + 2.0*wxDsn[i];
      }

      if (S->FlexActive) {
         for(Ig=0;Ig<S->Ng;Ig++) {
            G = &S->G[Ig];
            Bi = &S->B[G->Bin];
            Bo = &S->B[G->Bout];

            VxV(Bo->wn,G->FlexVelo,wxv);
            MTxV(Bo->CN,wxv,Cwxvo);

            VxV(Bi->wn,G->FlexVeli,wxv);
            MTxV(Bi->CN,wxv,Cwxvi);

            for(i=0;i<3;i++)
               Bo->AccR[i] += 2.0*(Cwxvi[i] - Cwxvo[i]);
         }
      }
}
/**********************************************************************/
/*  Find Peta, cplusPeta, HplusQeta, CnbP for each body          */
void FindFlexTerms(struct SCType *S)
{
      struct BodyType *B;
      long Nf,Ib,i,j,k;
      double cPe[3];

      for(Ib=0;Ib<S->Nb;Ib++) {
         B = &S->B[Ib];
         Nf = B->Nf;
         for(i=0;i<3;i++) {
            B->Peta[i] = 0.0;
            for(k=0;k<Nf;k++) {
               B->Peta[i] += B->Pf[i][k]*B->eta[k];
            }
            cPe[i] = B->c[i] + B->Peta[i];
            for(j=0;j<Nf;j++) {
               B->CnbP[i][j] = 0.0;
               for(k=0;k<3;k++)
                  B->CnbP[i][j] += B->CN[k][i]*B->Pf[k][j];
            }
         }
         V2CrossM(cPe,B->cplusPeta);
      }

      for(Ib=0;Ib<S->Nb;Ib++) {
         B = &S->B[Ib];
         Nf = B->Nf;
         for(i=0;i<3;i++) {
            for(j=0;j<Nf;j++) {
               B->HplusQeta[i][j] = B->Hf[i][j];
            }
         }
      }
      if (S->IncludeSecondOrderFlexTerms) {
         for(Ib=0;Ib<S->Nb;Ib++) {
            B = &S->B[Ib];
            Nf = B->Nf;
            for(i=0;i<3;i++) {
               for(j=0;j<Nf;j++) {
                  for(k=0;k<Nf;k++) {
                     B->HplusQeta[i][j] += B->Qf[i][j][k]*B->eta[k];
                  }
               }
            }
         }
      }
}
/**********************************************************************/
void FindInertiaTrq(struct SCType *S)
{
      struct BodyType *B;
      struct WhlType *W;
      double H[3],wxH[3],Ia[3];
      double cPexa[3];
      double CAccR[3];
      long Ib,Iw,i,Nf;

      /* -I*AlphaR - wxH for all bodies */
      for(Ib=0;Ib<S->Nb;Ib++) {
         B = &S->B[Ib];
         MxV(B->I,B->wn,H);
         for(i=0;i<3;i++) H[i] += B->EmbeddedMom[i];
         VxV(B->wn,H,wxH);
         MxV(B->I,B->AlphaR,Ia);
         for(i=0;i<3;i++) B->InertiaTrq[i] = -Ia[i] - wxH[i];
      }
      
      for(Iw=0;Iw<S->Nw;Iw++) {
         W = &S->Whl[Iw];
         B = &S->B[W->Body];
         for(i=0;i<3;i++) {
            H[i] = W->H*W->A[i];
         }
         VxV(B->wn,H,wxH);
         for(i=0;i<3;i++) B->InertiaTrq[i] +=  -wxH[i];
      }

      if (S->FlexActive && S->RefPt == REFPT_JOINT) {
         /* -(c + Pf*eta) x AccR */
         for(Ib=0;Ib<S->Nb;Ib++) {
            B = &S->B[Ib];
            Nf = B->Nf;
            MxV(B->CN,B->AccR,CAccR);
            MxV(B->cplusPeta,CAccR,cPexa);
            for(i=0;i<3;i++) B->InertiaTrq[i] += -cPexa[i];
         }
      }

}
/**********************************************************************/
void FindInertiaFrc(struct SCType *S)
{
      struct BodyType *B;
      double cPexa[3],cPexw[3],cPexwxw[3],Pxi[3],wxPxi[3];
      double FlexInertiaFrc[3],FlexInertiaFrcN[3];
      long Ib,i,j,Nf;

      for(Ib=0;Ib<S->Nb;Ib++) {
         B = &S->B[Ib];
         for(i=0;i<3;i++) B->InertiaFrc[i] = -B->mass*B->AccR[i];
      }

      if (S->FlexActive && S->RefPt == REFPT_JOINT) {
         /* -AlphaR x (c+Pf*eta) - wx(wx(c+Pf*eta)) - 2wxPf*xi */
         for(Ib=0;Ib<S->Nb;Ib++) {
            B = &S->B[Ib];
            Nf = B->Nf;
            MxV(B->cplusPeta,B->AlphaR,cPexa);
            MxV(B->cplusPeta,B->wn,cPexw);
            VxV(cPexw,B->wn,cPexwxw);
            for(i=0;i<3;i++) {
               Pxi[i] = 0.0;
               for(j=0;j<Nf;j++) Pxi[i] += B->Pf[i][j]*B->xi[j];
            }
            VxV(B->wn,Pxi,wxPxi);
            for(i=0;i<3;i++)
               FlexInertiaFrc[i] = cPexa[i] - cPexwxw[i] - 2.0*wxPxi[i];
            MTxV(B->CN,FlexInertiaFrc,FlexInertiaFrcN);
            for(i=0;i<3;i++) B->InertiaFrc[i] += FlexInertiaFrcN[i];
         }
      }
}
/**********************************************************************/
void FindFlexInertiaFrc(struct SCType *S)
{
      long Ib,Nf,f0;
      struct BodyType *B;
      struct DynType *D;
      long i,j,k;

      D = &S->Dyn;

      if (S->RefPt == REFPT_JOINT) {
         /* -Pf*AccR - Hf*AlphaR */
         for(Ib=0;Ib<S->Nb;Ib++) {
            B = &S->B[Ib];
            Nf = B->Nf;
            f0 = B->f0;
            for(i=0;i<Nf;i++) {
               for(k=0;k<3;k++)
                  D->FlexFrc[f0+i] -= B->CnbP[k][i]*B->AccR[k]
                                    + B->HplusQeta[k][i]*B->AlphaR[k];
            }
         }
      }

      if (S->IncludeSecondOrderFlexTerms) {
         /*  -w*R*w - w*S*w*eta - 2*xi*Q*w */
         for(Ib=0;Ib<S->Nb;Ib++) {
            B = &S->B[Ib];
            Nf = B->Nf;
            f0 = B->f0;

            /* Qxi */
            for(i=0;i<3;i++) {
               for(j=0;j<Nf;j++) {
                  B->Qxi[i][j] = 0.0;
                  for(k=0;k<Nf;k++) {
                     B->Qxi[i][j] += B->Qf[i][j][k]*B->xi[k];
                  }
               }
            }

            /* Rw */
            for(i=0;i<3;i++) {
               for(j=0;j<Nf;j++) {
                  B->Rw[i][j] = 0.0;
                  for(k=0;k<3;k++)
                     B->Rw[i][j] += B->Rf[i][j][k]*B->wn[k];
               }
            }

            /* Swe */
            for(i=0;i<3;i++) {
               for(j=0;j<Nf;j++) {
                  for(k=0;k<Nf;k++) {
                     B->Sw[i][j][k] = B->Sf[i][j][k][0]*B->wn[0]+
                                      B->Sf[i][j][k][1]*B->wn[1]+
                                      B->Sf[i][j][k][2]*B->wn[2];
                  }
               }
            }
            for(i=0;i<3;i++) {
               for(j=0;j<Nf;j++) {
                  B->Swe[i][j] = 0.0;
                  for(k=0;k<Nf;k++)
                     B->Swe[i][j] += B->Sw[i][j][k]*B->eta[k];
               }
            }

            /* Add them up */
            for(i=0;i<Nf;i++) {
               for(j=0;j<3;j++)
                  D->FlexFrc[f0+i] -=
                     (B->Rw[j][i]+B->Swe[j][i]+2.0*B->Qxi[j][i])*B->wn[j];
            }
         }
      }
}
/**********************************************************************/
void FindFlexFrc(struct SCType *S)
{
      long Ib,In,i,k,Nf,f0;
      struct BodyType *B;
      struct DynType *D;
      struct FlexNodeType *FN;

      D = &S->Dyn;

      for(Ib=0;Ib<S->Nb;Ib++) {
         B = &S->B[Ib];
         Nf = B->Nf;
         f0 = B->f0;
         for(i=0;i<Nf;i++) {
            D->FlexFrc[f0+i] = 0.0;
            for(k=0;k<Nf;k++)
               D->FlexFrc[f0+i] -= B->Cf[i][k]*B->xi[k]
                                 + B->Kf[i][k]*B->eta[k];
         }
         for(In=0;In<B->NumFlexNodes;In++) {
            FN = &B->FlexNode[In];
            for(i=0;i<Nf;i++) {
               D->FlexFrc[f0+i] +=
                  FN->PSI[0][i]*FN->Frc[0] +
                  FN->PSI[1][i]*FN->Frc[1] +
                  FN->PSI[2][i]*FN->Frc[2] +
                  FN->THETA[0][i]*FN->Trq[0] +
                  FN->THETA[1][i]*FN->Trq[1] +
                  FN->THETA[2][i]*FN->Trq[2];
            }
         }
      }
}
/**********************************************************************/
void EchoPVel(struct SCType *S)
{
      FILE *outfile;
      long i,j,Nb,Nu,Nf;
      struct DynType *D;

      D = &S->Dyn;

      Nb = S->Nb;
      Nu = D->Nu;
      Nf = D->Nf;

      outfile = FileOpen(InOutPath,"PVel.42","w");
      for(i=0;i<3*Nb;i++) {
         for(j=0;j<Nu;j++) fprintf(outfile," %24.16le",D->PVel[i][j]);
         fprintf(outfile,"\n");
      }
      fclose(outfile);

      outfile = FileOpen(InOutPath,"PAngVel.42","w");
      for(i=0;i<3*Nb;i++) {
         for(j=0;j<Nu;j++) fprintf(outfile," %24.16le",D->PAngVel[i][j]);
         fprintf(outfile,"\n");
      }
      fclose(outfile);

      outfile = FileOpen(InOutPath,"PVelf.42","w");
      for(i=0;i<3*Nb;i++) {
         for(j=0;j<Nf;j++) fprintf(outfile," %24.16le",D->PVelf[i][j]);
         fprintf(outfile,"\n");
      }
      fclose(outfile);

      outfile = FileOpen(InOutPath,"PAngVelf.42","w");
      for(i=0;i<3*Nb;i++) {
         for(j=0;j<Nf;j++) fprintf(outfile," %24.16le",D->PAngVelf[i][j]);
         fprintf(outfile,"\n");
      }
      fclose(outfile);

      outfile = FileOpen(InOutPath,"mPVel.42","w");
      for(i=0;i<3*Nb;i++) {
         for(j=0;j<Nu;j++) fprintf(outfile," %24.16le",D->mPVel[i][j]);
         fprintf(outfile,"\n");
      }
      fclose(outfile);

      outfile = FileOpen(InOutPath,"IPAngVel.42","w");
      for(i=0;i<3*Nb;i++) {
         for(j=0;j<Nu;j++) fprintf(outfile," %24.16le",D->IPAngVel[i][j]);
         fprintf(outfile,"\n");
      }
      fclose(outfile);

      outfile = FileOpen(InOutPath,"mPVelf.42","w");
      for(i=0;i<3*Nb;i++) {
         for(j=0;j<Nf;j++) fprintf(outfile," %24.16le",D->mPVelf[i][j]);
         fprintf(outfile,"\n");
      }
      fclose(outfile);

      outfile = FileOpen(InOutPath,"IPAngVelf.42","w");
      for(i=0;i<3*Nb;i++) {
         for(j=0;j<Nf;j++) fprintf(outfile," %24.16le",D->IPAngVelf[i][j]);
         fprintf(outfile,"\n");
      }
      fclose(outfile);

      outfile = FileOpen(InOutPath,"Mf.42","w");
      for(i=0;i<Nf;i++) {
         for(j=0;j<Nf;j++) fprintf(outfile," %24.16le",D->Mf[i][j]);
         fprintf(outfile,"\n");
      }
      fclose(outfile);

      outfile = FileOpen(InOutPath,"HQeOMf.42","w");
      for(i=0;i<Nf;i++) {
         for(j=0;j<Nf;j++) fprintf(outfile," %24.16le",D->HplusQetaPAngVelf[i][j]);
         fprintf(outfile,"\n");
      }
      fclose(outfile);

      outfile = FileOpen(InOutPath,"BodyTrq.42","w");
      for(i=0;i<3*Nb;i++) {
         fprintf(outfile," %24.16le\n",D->BodyTrq[i]);
      }
      fclose(outfile);

      outfile = FileOpen(InOutPath,"BodyFrc.42","w");
      for(i=0;i<3*Nb;i++) {
         fprintf(outfile," %24.16le\n",D->BodyFrc[i]);
      }
      fclose(outfile);

      outfile = FileOpen(InOutPath,"FlexFrc.42","w");
      for(i=0;i<Nf;i++) {
         fprintf(outfile," %24.16le\n",D->FlexFrc[i]);
      }
      fclose(outfile);

      outfile = FileOpen(InOutPath,"FlexInertiaFrc.42","w");
      for(i=0;i<Nf;i++) {
         fprintf(outfile," %24.16le\n",D->FlexInertiaFrc[i]);
      }
      fclose(outfile);


}
/**********************************************************************/
void EchoEOM(double **COEF, double *State, double *RHS,long Ns)
{
      FILE *outfile;
      long i,j;

      outfile = FileOpen(InOutPath,"COEF.42","w");

      for(i=0;i<Ns;i++) {
         for(j=0;j<Ns;j++) fprintf(outfile," %24.16le",COEF[i][j]);
         fprintf(outfile,"\n");
      }
      fclose(outfile);

      outfile = FileOpen(InOutPath,"x.42","w");
      for(i=0;i<Ns;i++) fprintf(outfile,"%24.16le\n",State[i]);
      fclose(outfile);

      outfile = FileOpen(InOutPath,"RHS.42","w");
      for(i=0;i<Ns;i++) fprintf(outfile,"%24.16le\n",RHS[i]);
      fclose(outfile);
}
/**********************************************************************/
void EchoUdot(double *State, long Ns)
{
      static FILE *outfile;
      long i;
      static long First = 1;
      
      if (First) {
         First = 0;
         outfile = FileOpen(InOutPath,"udot.42","w");
      }   
      
      for(i=0;i<Ns;i++) fprintf(outfile,"%24.16le ",State[i]);
      fprintf(outfile,"\n");
}
/********************************************************************/
void EchoRemAcc(struct SCType *S)
{
   FILE *outfile;
   long i,Ib;
   struct BodyType *B;

   outfile = FileOpen(InOutPath,"AccR.42","w");
   for(Ib=0;Ib<S->Nb;Ib++) {
      B = &S->B[Ib];
      for(i=0;i<3;i++) fprintf(outfile," %24.16le\n",B->AccR[i]);
   }
   fclose(outfile);

   outfile = FileOpen(InOutPath,"AlphaR.42","w");
   for(Ib=0;Ib<S->Nb;Ib++) {
      B = &S->B[Ib];
      for(i=0;i<3;i++) fprintf(outfile," %24.16le\n",B->AlphaR[i]);
   }
   fclose(outfile);

   outfile = FileOpen(InOutPath,"wn.42","w");
   for(Ib=0;Ib<S->Nb;Ib++) {
      B = &S->B[Ib];
      for(i=0;i<3;i++) fprintf(outfile," %24.16le\n",B->wn[i]);
   }
   fclose(outfile);

   outfile = FileOpen(InOutPath,"InertiaTrq.42","w");
   for(Ib=0;Ib<S->Nb;Ib++) {
      B = &S->B[Ib];
      for(i=0;i<3;i++) fprintf(outfile," %24.16le\n",B->InertiaTrq[i]);
   }
   fclose(outfile);

   outfile = FileOpen(InOutPath,"InertiaFrc.42","w");
   for(Ib=0;Ib<S->Nb;Ib++) {
      B = &S->B[Ib];
      for(i=0;i<3;i++) fprintf(outfile," %24.16le\n",B->InertiaFrc[i]);
   }
   fclose(outfile);

}
/**********************************************************************/
void KaneNBodyEOM(double *u, double *x, double *h,
                  double *uf, double *xf,
                  double *udot, double *xdot, double *hdot,
                  double *ufdot, double *xfdot,
                  struct SCType *S)
{
      long i,j,k,Nk,Ig,Ib,Iw,N,ii,jj;
      struct DynType *D;
      struct JointType *G;
      struct BodyType *B;
      struct WhlType *W;
      double TrqBo[3],TrqGo[3],TrqBi[3];
      double FrcBo[3],FrcGo[3],FrcBi[3],FrcGi[3];
      double FrcBiN[3], FrcBoN[3];
      double rxFi[3],rxFo[3];
      D = &S->Dyn;
      //static long First = 1;

/* .. Dynamics */

      MapStateVectorToBodyStates(u,x,uf,xf,S);

      /* Joint Partials */
      for(Ig=0;Ig<S->Ng;Ig++) {
         G = &S->G[Ig];
         JointPartials(FALSE,G->IsSpherical,G->RotSeq,G->TrnSeq,
            G->Ang,G->AngRate,G->Gamma,G->Gs,G->Gds,
            G->PosRate,G->Delta,G->Ds,G->Dds);
      }

      /* Path vectors, beta and rho */
      FindBodyPathDCMs(S);
      FindPathVectors(S);

      /* Find Peta, cplusPeta, HplusQeta, CnbP for each body */
      FindFlexTerms(S);

      /* Partial Angular Velocity Matrix (PAngVel) and I*PAngVel */
      FindPAngVel(S);
      /* Partial Velocity Matrix (PVel) and m*PVel */
      FindPVel(S);
      if (S->FlexActive) {
         /* Flex Partial Angular Velocity Matrix (PAngVelf) and I*PAngVelf */
         FindPAngVelf(S);
         /* Flex Partial Velocity Matrix (PVelf) and m*PVelf */
         FindPVelf(S);
         /* Add (c+Pf*eta)xPVel, etc to IPAngVelf and mPVelf */
         AugmentIPAngVelf(S);
         FindHplusQetaPAngVelf(S);
         if (S->RefPt == REFPT_JOINT) {
            AugmentIPAngVel(S);
            AugmentMPVel(S);
            AugmentMPVelf(S);
            FindPCPVelf(S);
         }
      }

      /* Remainder Accelerations, AlphaR and AccR */
      FindAlphaR(S);
      FindAccR(S);

      FindInertiaTrq(S);
      FindInertiaFrc(S);

      /* Non-actuator-induced joint torques */
      if(S->PassiveJointFrcTrqEnabled) PassiveJointFrcTrq(u,x,S);

      /* "F-bendy" and "T-bendy", Spring/Damping, and nonlinear terms */
      if (S->FlexActive) {
         FindFlexFrc(S);
         FindFlexInertiaFrc(S);
      }

      /* Assemble BodyTrq, BodyFrc, and FlexFrc Terms */
      for(Ib=0;Ib<S->Nb;Ib++) {
         B = &S->B[Ib];
         for(i=0;i<3;i++) {
            D->BodyTrq[3*Ib+i] = B->Trq[i] + B->InertiaTrq[i];
            D->BodyFrc[3*Ib+i] = B->FrcN[i] + B->InertiaFrc[i];
         }
      }
      /* Add Wheel Torques */
      for(Iw=0;Iw<S->Nw;Iw++) {
         W = &S->Whl[Iw];
         Ib = W->Body;
         for(i=0;i<3;i++) {
            D->BodyTrq[3*Ib+i] -= W->Trq*W->A[i];
         }
      }
      /* Applied Joint Torques and Forces */
      for(Ig=0;Ig<S->Ng;Ig++) {
         G = &S->G[Ig];
         for(i=0;i<3;i++) {
            FrcGi[i] = 0.0;
            TrqGo[i] = 0.0;
            for(j=0;j<G->RotDOF;j++) {
               TrqGo[i] += G->Gamma[i][j]*(G->Trq[j]+G->PassiveTrq[j]);
            }
            for(j=0;j<G->TrnDOF;j++) {
               FrcGi[i] += G->Delta[i][j]*(G->Frc[j]+G->PassiveFrc[j]);
            }
         }

         /* Force Transformations*/
         MxV(G->CGoGi,FrcGi,FrcGo);
         MTxV(G->CTrqBi,FrcGo,FrcBi);
         MTxV(G->CTrqBo,FrcGo,FrcBo);
         MTxV(S->B[G->Bin].CN,FrcBi,FrcBiN);
         MTxV(S->B[G->Bout].CN,FrcBo,FrcBoN);
         VxV(G->ri,FrcBi,rxFi);
         VxV(G->ro,FrcBo,rxFo);

         /* Torque Transformations */
         MTxV(G->CTrqBi,TrqGo,TrqBi);
         MTxV(G->CTrqBo,TrqGo,TrqBo);

         for(i=0;i<3;i++) {
            D->BodyTrq[3*G->Bin+i]  -= TrqBi[i] + rxFi[i];
            D->BodyTrq[3*G->Bout+i] += TrqBo[i] + rxFo[i];
            D->BodyFrc[3*G->Bin+i]  -= FrcBiN[i];
            D->BodyFrc[3*G->Bout+i] += FrcBoN[i];
         }
      }

      /* Assemble COEF and RHS */
      Nk = 3*S->Nb;
      for(i=0;i<D->Nu;i++) {
         /* Upper Left of COEF */
         for(j=i;j<D->Nu;j++) {
            D->COEF[i][j] = 0.0;
            for(k=0;k<Nk;k++) {
               D->COEF[i][j] += D->PAngVel[k][i]*D->IPAngVel[k][j]
                              + D->PVel[k][i]*D->mPVel[k][j];
            }
            D->COEF[j][i] = D->COEF[i][j];
         }
         /* Upper Right of COEF */
         for(j=0;j<D->Nf;j++) {
            D->COEF[i][D->Nu+j] = 0.0;
            for(k=0;k<Nk;k++) {
               D->COEF[i][D->Nu+j] += D->PAngVel[k][i]*D->IPAngVelf[k][j]
                                    + D->PVel[k][i]*D->mPVelf[k][j];
            }
         }
         /* Upper RHS */
         D->RHS[i] = 0.0;
         for(k=0;k<Nk;k++) {
            D->RHS[i] += D->PAngVel[k][i]*D->BodyTrq[k]
                       + D->PVel[k][i]*D->BodyFrc[k];
         }
      }
      for(i=0;i<D->Nf;i++) {
         /* Lower Left of COEF is Transpose of Upper Right */
         for(j=0;j<D->Nu;j++) D->COEF[D->Nu+i][j] = D->COEF[j][D->Nu+i];
         /* Lower Right of COEF */
         for(j=0;j<D->Nf;j++) {
            D->COEF[D->Nu+i][D->Nu+j] = D->Mf[i][j];
            for(k=0;k<Nk;k++) {
               D->COEF[D->Nu+i][D->Nu+j] += D->PAngVelf[k][i]*D->IPAngVelf[k][j]
                                    + D->PVelf[k][i]*D->mPVelf[k][j];
            }
         }
         /* Lower RHS */
         D->RHS[D->Nu+i] =  D->FlexFrc[i] + D->FlexInertiaFrc[i];
         for(k=0;k<Nk;k++)
            D->RHS[D->Nu+i] += D->PAngVelf[k][i]*D->BodyTrq[k]
                             + D->PVelf[k][i]*D->BodyFrc[k];
      }
      if (S->RefPt == REFPT_JOINT) {
         for(i=0;i<D->Nf;i++) {
            for(j=0;j<D->Nf;j++) {
               D->COEF[D->Nu+i][D->Nu+j] +=
                  D->PCPVelf[i][j]+D->HplusQetaPAngVelf[i][j];
            }
         }
      }

/* .. Eliminate locked DOF */
      if (D->SomeJointsLocked) {
         N = D->Nu + D->Nf;
         /* Eliminate Rows */
         for(i=0;i<D->Ns;i++) {
            ii = D->ActiveStateIdx[i];
            for(j=0;j<N;j++) {
               D->COEF[i][j] = D->COEF[ii][j];
            }
            D->RHS[i] = D->RHS[ii];
         }
         /* Eliminate Columns */
         for(j=0;j<D->Ns;j++) {
            jj = D->ActiveStateIdx[j];
            for(i=0;i<D->Ns;i++) {
               D->COEF[i][j] = D->COEF[i][jj];
            }
         }
      }

/* .. Solve EOM */
      //EchoPVel(S);
      //EchoRemAcc(S);
      //if (First) {
      //   First = 0;
      //   EchoEOM(D->COEF,D->ActiveState,D->RHS,D->Ns); 
      //}
      LINSOLVE(D->COEF,D->ActiveState,D->RHS,D->Ns);
      //OrderNSolveEOM(S);
      //EchoUdot(D->ActiveState,D->Ns);

/* .. Map out result */
      if(D->SomeJointsLocked) {
         N = D->Ns - D->Nf;
         for(i=0;i<N;i++) udot[D->ActiveStateIdx[i]] = D->ActiveState[i];
         for(i=0;i<D->Nf;i++) ufdot[i] = D->ActiveState[N+i];
      }
      else {
         for(i=0;i<D->Nu;i++) udot[i] = D->ActiveState[i];
         for(i=0;i<D->Nf;i++) ufdot[i] = D->ActiveState[D->Nu+i];
      }

/* .. Kinematics */
      /* B[0].qn */
      QW2QDOT(&x[0],&u[0],&xdot[0]);
      /* Joints, rotation and translation */
      for(Ig=0;Ig<S->Ng;Ig++) {
         G = &S->G[Ig];
         if (G->IsSpherical) {
            QW2QDOT(&x[G->Rotx0],&u[G->Rotu0],&xdot[G->Rotx0]);
         }
         else {
            for(i=0;i<G->RotDOF;i++) xdot[G->Rotx0+i] = u[G->Rotu0+i];
         }
         for(i=0;i<G->TrnDOF;i++) xdot[G->Trnx0+i] = u[G->Trnu0+i];
      }
      /* B[0].pn */
      for(i=0;i<3;i++) xdot[D->Nx-3+i] = u[D->Nu-3+i];
      /* Flex Modes */
      for(i=0;i<D->Nf;i++) xfdot[i] = uf[i];

/* .. Wheel-body interaction  */
      for (i=0;i<S->Nw;i++) {
         hdot[i] = S->Whl[i].Trq;
      }
}
/**********************************************************************/
/*  PAngVelc (for Constraints)                                        */
void FindPAngVelc(struct SCType *S)
{
      struct DynType *D;
      struct JointType *G;
      double CGo[3][3],CG[3][3];
      long Ib,i,j,k,i0,j0;
      long Jb,Ig,Nc;

      D = &S->Dyn;

      for(Ib=1;Ib<S->Nb;Ib++) {
         Ig = S->B[Ib].Gin;
         G = &S->G[Ig];
         i0 = 3*Ib;
         j0 = G->Rotc0;
         Nc = 3-G->RotDOF;
         for(i=0;i<3;i++) {
            for(j=0;j<Nc;j++) {
               CGo[i][j] = 0.0;
               for(k=0;k<3;k++) {
                  CGo[i][j] += G->CTrqBo[k][i]*G->Gamma[k][G->RotDOF+j];
               }
               D->PAngVelc[i0+i][j0+j] = CGo[i][j];
            }
         }
         Jb = G->Bin;
         while(Jb > 0) {
            Ig = S->B[Jb].Gin;
            G = &S->G[Ig];
            j0 = G->Rotc0;
            Nc = 3-G->RotDOF;
            for(i=0;i<3;i++) {
               for(j=0;j<Nc;j++) {
                  CG[i][j] = 0.0;
                  for(k=0;k<3;k++) {
                     CG[i][j] += D->BodyPathTable[Ib][Jb].Coi[i][k]*CGo[k][j];
                  }
               }
            }
            for(i=0;i<3;i++) {
               for(j=0;j<Nc;j++) {
                  D->PAngVelc[i0+i][j0+j] = CG[i][j];
               }
            }
            Jb = G->Bin;
         }
      }
}
/**********************************************************************/
/* PVelc (for Constraints)                                            */
void FindPVelc(struct SCType *S)
{
      struct DynType *D;
      struct BodyType *Bjb;
      struct JointType *G;
      double RC[3][3];
      double RCB[3][3],RCG[3][3];
      double CNG[3][3],CD[3][3];
      long Ib,Jb,Ig,i,j,k,i0,j0,Nc;

      D = &S->Dyn;

      for(Ib=1;Ib<S->Nb;Ib++) {
         i0 = 3*Ib;
         Jb = Ib;
         while (Jb > 0) {
            Bjb = &S->B[Jb];
            Ig = Bjb->Gin;
            G = &S->G[Ig];
            /* Rotation */
            j0 = G->Rotc0;
            Nc = 3-G->RotDOF;
            VcrossMT(D->JointPathTable[Ib][Ig].rho,Bjb->CN,RCB);
            MxMT(RCB,G->CTrqBo,RC);
            for(i=0;i<3;i++) {
               for(j=0;j<Nc;j++) {
                  RCG[i][j] = 0.0;
                  for(k=0;k<3;k++) {
                     RCG[i][j] += RC[i][k]*G->Gamma[k][G->RotDOF+j];
                  }
               }
            }
            for(i=0;i<3;i++) {
               for(j=0;j<Nc;j++) {
                  D->PVelc[i0+i][j0+j] = RCG[i][j];
               }
            }
            /* Translation */
            j0 = G->Trnc0;
            Nc = 3-G->TrnDOF;
            MTxMT(S->B[G->Bin].CN,G->CGiBi,CNG);
            for(i=0;i<3;i++) {
               for(j=0;j<Nc;j++) {
                  CD[i][j] = 0.0;
                  for(k=0;k<3;k++) {
                     CD[i][j] += CNG[i][k]*G->Delta[k][G->TrnDOF+j];
                  }
                  D->PVelc[i0+i][j0+j] = CD[i][j];
               }
            }
            Jb = G->Bin;
         }
      }
}
/**********************************************************************/
void KaneNBodyConstraints(struct SCType *S)
{
      struct DynType *D;
      struct BodyType *B;
      struct JointType *G;
      struct WhlType *W;
      long Ig,Ib,Iw,i,j;
      double TrqBo[3],TrqGo[3],TrqBi[3];
      double FrcBo[3],FrcGo[3],FrcBi[3],FrcGi[3];
      double FrcBiN[3], FrcBoN[3];
      double rxFi[3],rxFo[3];

      D = &S->Dyn;

      MapStateVectorToBodyStates(D->u,D->x,D->uf,D->xf,S);

      for(Ig=0;Ig<S->Ng;Ig++) {
         G = &S->G[Ig];
         JointPartials(FALSE,G->IsSpherical,G->RotSeq,G->TrnSeq,
            G->Ang,G->AngRate,G->Gamma,G->Gs,G->Gds,
            G->PosRate,G->Delta,G->Ds,G->Dds);
      }
      FindBodyPathDCMs(S);
      FindPathVectors(S);

      FindPAngVel(S);
      FindPVel(S);

      FindPAngVelc(S);
      FindPVelc(S);

      FindAlphaR(S);
      FindAccR(S);
      FindInertiaTrq(S);
      FindInertiaFrc(S);

      /* Non-actuator-induced joint torques */
      if(S->PassiveJointFrcTrqEnabled) PassiveJointFrcTrq(D->u,D->x,S);

      /* "F-bendy" and "T-bendy", Spring/Damping, and nonlinear terms */
      if (S->FlexActive) {
         FindFlexFrc(S);
         FindFlexInertiaFrc(S);
      }

      /* Assemble BodyTrq, BodyFrc, and FlexFrc Terms */
      for(Ib=0;Ib<S->Nb;Ib++) {
         B = &S->B[Ib];
         for(i=0;i<3;i++) {
            D->BodyTrq[3*Ib+i] = B->Trq[i] + B->InertiaTrq[i];
            D->BodyFrc[3*Ib+i] = B->FrcN[i] + B->InertiaFrc[i];
         }
      }
      /* Add Wheel Torques */
      for(Iw=0;Iw<S->Nw;Iw++) {
         W = &S->Whl[Iw];
         Ib = W->Body;
         for(i=0;i<3;i++) {
            D->BodyTrq[3*Ib+i] -= W->Trq*W->A[i];
         }
      }
      /* Applied Joint Torques and Forces */
      for(Ig=0;Ig<S->Ng;Ig++) {
         G = &S->G[Ig];
         for(i=0;i<3;i++) {
            FrcGi[i] = 0.0;
            TrqGo[i] = 0.0;
            for(j=0;j<G->RotDOF;j++) {
               TrqGo[i] += G->Gamma[i][j]*(G->Trq[j]+G->PassiveTrq[j]);
            }
            for(j=0;j<G->TrnDOF;j++) {
               FrcGi[i] += G->Delta[i][j]*(G->Frc[j]+G->PassiveFrc[j]);
            }
         }

         /* Force Transformations*/
         MxV(G->CGoGi,FrcGi,FrcGo);
         MTxV(G->CTrqBi,FrcGo,FrcBi);
         MTxV(G->CTrqBo,FrcGo,FrcBo);
         MTxV(S->B[G->Bin].CN,FrcBi,FrcBiN);
         MTxV(S->B[G->Bout].CN,FrcBo,FrcBoN);
         VxV(G->ri,FrcBi,rxFi);
         VxV(G->ro,FrcBo,rxFo);

         /* Torque Transformations */
         MTxV(G->CTrqBi,TrqGo,TrqBi);
         MTxV(G->CTrqBo,TrqGo,TrqBo);

         for(i=0;i<3;i++) {
            D->BodyTrq[3*G->Bin+i]  -= TrqBi[i] + rxFi[i];
            D->BodyTrq[3*G->Bout+i] += TrqBo[i] + rxFo[i];
            D->BodyFrc[3*G->Bin+i]  -= FrcBiN[i];
            D->BodyFrc[3*G->Bout+i] += FrcBoN[i];
         }
      }

      /* Assemble Total Trq, Total Frc */
      for(i=0;i<3*S->Nb;i++) {
         D->TotalTrq[i] = D->BodyTrq[i];
         D->TotalFrc[i] = D->BodyFrc[i];
         for(j=0;j<D->Nu;j++) {
            D->TotalTrq[i] -= D->IPAngVel[i][j]*D->du[j];
            D->TotalFrc[i] -= D->mPVel[i][j]*D->du[j];
         }
      }
      for(Ib=0;Ib<S->Nb;Ib++) {
         B = &S->B[Ib];
         for(i=0;i<3;i++) {
            D->TotalTrq[3*Ib+i] += B->InertiaTrq[i];
            D->TotalFrc[3*Ib+i] += B->InertiaFrc[i];
         }
      }

      /* Find Generalized Constraint Forces */
      for(i=0;i<D->Nc;i++) {
         D->GenConstraintFrc[i] = 0.0;
         for(j=0;j<3*S->Nb;j++) {
            D->GenConstraintFrc[i] -=
               D->PAngVelc[j][i]*D->TotalTrq[j]
             + D->PVelc[j][i]*D->TotalFrc[j];
         }
      }
}
/**********************************************************************/
void FindBodyAccelerations(struct SCType *S)
{
      struct DynType *D;
      struct BodyType *B;
      long Ib,i,j;

      D = &S->Dyn;

      for(Ib=0;Ib<S->Nb;Ib++) {
         B = &S->B[Ib];
         for(i=0;i<3;i++) {
            B->alpha[i] = B->AlphaR[i];
            B->accel[i] = S->FrcN[i]/S->mass + B->AccR[i];
            for(j=0;j<D->Nu;j++) {
               B->alpha[i] += D->PAngVel[3*Ib+i][j]*D->du[j];
               B->accel[i] += D->PVel[3*Ib+i][j]*D->du[j];
            }
         }
      }
}
/**********************************************************************/
void KaneNBodyRK4(struct SCType *S)
{
      struct DynType *D;
      struct JointType *G;
      struct WhlType *W;
      double *u,*uu,*du,*udot;
      double *x,*xx,*dx,*xdot;
      double *h,*hh,*dh,*hdot;
      double *uf,*uuf,*duf,*ufdot;
      double *xf,*xxf,*dxf,*xfdot;
      long i,iu,Ig;
      long Nu,Nx,Nw,Nf;

      /* Save some typing (and dereferencing) */
      D = &S->Dyn;
      Nu = D->Nu;
      Nx = D->Nx;
      Nw = S->Nw;
      Nf = D->Nf;

      u = D->u;
      x = D->x;
      h = D->h;
      uf = D->uf;
      xf = D->xf;

      uu = D->uu;
      xx = D->xx;
      hh = D->hh;
      uuf = D->uuf;
      xxf = D->xxf;

      du = D->du;
      dx = D->dx;
      dh = D->dh;
      duf = D->duf;
      dxf = D->dxf;

      udot = D->udot;
      xdot = D->xdot;
      hdot = D->hdot;
      ufdot = D->ufdot;
      xfdot = D->xfdot;

      /* State vector initialized in InitKaneNBody() */

/* .. Check for Locked Joint DOFs */
      for(i=0;i<3;i++) D->ActiveStateIdx[i] = i;  /* Body 0 angular DOF never locked */
      D->Ns = 3;
      iu = 3;
      for(Ig=0;Ig<S->Ng;Ig++) {
         G = &S->G[Ig];
         G->ActiveRotu0 = D->Ns;
         G->ActiveRotDOF = 0;
         for(i=0;i<G->RotDOF;i++) {
            if (!G->RotLocked[i]) {
               G->ActiveRotDOF++;
               D->ActiveStateIdx[D->Ns] = iu;
               D->Ns++;
            }
            else {
               u[iu] = 0.0;
            }
            iu++;
         }
         G->ActiveTrnu0 = D->Ns;
         G->ActiveTrnDOF = 0;
         for(i=0;i<G->TrnDOF;i++) {
            if (!G->TrnLocked[i]) {
               G->ActiveTrnDOF++;
               D->ActiveStateIdx[D->Ns] = iu;
               D->Ns++;
            }
            else {
               u[iu] = 0.0;
            }
            iu++;
         }
      }
      for(i=0;i<3;i++) {  /* Body 0 translational DOF never locked */
         D->ActiveStateIdx[D->Ns] = iu;
         D->Ns++;
         iu++;
      }
      D->SomeJointsLocked = ((D->Ns == D->Nu) ? 0 : 1);
      D->Ns += D->Nf;

/* .. 4th order Runge Kutta */

      /* First Call */
      KaneNBodyEOM(u,x,h,uf,xf,du,dx,dh,duf,dxf,S);
      for(i=0;i<Nu;i++)  {
         uu[i] = u[i] + 0.5*DTSIM*du[i];
         udot[i] = du[i]/6.0;
      }
      for(i=0;i<Nx;i++)  {
         xx[i] = x[i] + 0.5*DTSIM*dx[i];
         xdot[i] = dx[i]/6.0;
      }
      for(i=0;i<Nw;i++)  {
         hh[i] = h[i] + 0.5*DTSIM*dh[i];
         hdot[i] = dh[i]/6.0;
      }
      for(i=0;i<Nf;i++)  {
         uuf[i] = uf[i] + 0.5*DTSIM*duf[i];
         ufdot[i] = duf[i]/6.0;
         xxf[i] = xf[i] + 0.5*DTSIM*dxf[i];
         xfdot[i] = dxf[i]/6.0;
      }

      /* This call must be made here, so that du is taken at the */
      /* same instant as all the other configuration variables */
      if (S->ConstraintsRequested) {
         KaneNBodyConstraints(S);
      }
      FindBodyAccelerations(S);
      
      /* Second Call */
      KaneNBodyEOM(uu,xx,hh,uuf,xxf,du,dx,dh,duf,dxf,S);

      for(i=0;i<Nu;i++) {
         uu[i] = u[i] + 0.5*DTSIM*du[i];
         udot[i] += du[i]/3.0;
      }
      for(i=0;i<Nx;i++) {
         xx[i] = x[i] + 0.5*DTSIM*dx[i];
         xdot[i] += dx[i]/3.0;
      }
      for(i=0;i<Nw;i++) {
         hh[i] = h[i] + 0.5*DTSIM*dh[i];
         hdot[i] += dh[i]/3.0;
      }
      for(i=0;i<Nf;i++)  {
         uuf[i] = uf[i] + 0.5*DTSIM*duf[i];
         ufdot[i] += duf[i]/3.0;
         xxf[i] = xf[i] + 0.5*DTSIM*dxf[i];
         xfdot[i] += dxf[i]/3.0;
      }

      /* Third Call */
      KaneNBodyEOM(uu,xx,hh,uuf,xxf,du,dx,dh,duf,dxf,S);

      for(i=0;i<Nu;i++) {
         uu[i] = u[i] + DTSIM*du[i];
         udot[i] += du[i]/3.0;
      }
      for(i=0;i<Nx;i++) {
         xx[i] = x[i] + DTSIM*dx[i];
         xdot[i] += dx[i]/3.0;
      }
      for(i=0;i<Nw;i++) {
         hh[i] = h[i] + DTSIM*dh[i];
         hdot[i] += dh[i]/3.0;
      }
      for(i=0;i<Nf;i++)  {
         uuf[i] = uf[i] + DTSIM*duf[i];
         ufdot[i] += duf[i]/3.0;
         xxf[i] = xf[i] + DTSIM*dxf[i];
         xfdot[i] += dxf[i]/3.0;
      }

      /* Fourth Call */
      KaneNBodyEOM(uu,xx,hh,uuf,xxf,du,dx,dh,duf,dxf,S);

      for(i=0;i<Nu;i++) udot[i] += du[i]/6.0;
      for(i=0;i<Nx;i++) xdot[i] += dx[i]/6.0;
      for(i=0;i<Nw;i++) hdot[i] += dh[i]/6.0;
      for(i=0;i<Nf;i++)  {
         ufdot[i] += duf[i]/6.0;
         xfdot[i] += dxf[i]/6.0;
      }

      for(i=0;i<Nu;i++) u[i] += udot[i]*DTSIM;
      for(i=0;i<Nx;i++) x[i] += xdot[i]*DTSIM;
      for(i=0;i<Nw;i++) h[i] += hdot[i]*DTSIM;
      for(i=0;i<Nf;i++)  {
         uf[i] += ufdot[i]*DTSIM;
         xf[i] += xfdot[i]*DTSIM;
      }

/* .. NaN Check */
      for(i=0;i<Nu;i++) {
         if (isnan(u[i])) {
            printf("Oops!  u[%ld] is NaN in KaneNBodyRK4 at SimTime = %lf.  Better fix that.\n",i,SimTime);
            exit(1);
         }
      }
      for(i=0;i<Nx;i++) {
         if (isnan(x[i])) {
            printf("Oops!  x[%ld] is NaN in KaneNBodyRK4 at SimTime = %lf.  Better fix that.\n",i,SimTime);
            exit(1);
         }
      }
      for(i=0;i<Nf;i++) {
         if (isnan(uf[i])) {
            printf("Oops!  uf[%ld] is NaN in KaneNBodyRK4 at SimTime = %lf.  Better fix that.\n",i,SimTime);
            exit(1);
         }
         if (isnan(xf[i])) {
            printf("Oops!  xf[%ld] is NaN in KaneNBodyRK4 at SimTime = %lf.  Better fix that.\n",i,SimTime);
            exit(1);
         }
      }

/* .. Map out state variables */
      MapStateVectorToBodyStates(u,x,uf,xf,S);
      MotionConstraints(S);
      SCMassProps(S);

/* .. Wheels */
      for(i=0;i<Nw;i++) {
         W = &S->Whl[i];
         W->H = h[i];
         W->w = h[i]/W->J;
         /* Hi-fi isn't needed for wheel rotor angle */
         W->ang += W->w*DTSIM;
         W->ang = fmod(W->ang,TwoPi);
      }

/* .. For Accelerometers */
      KaneNBodyEOM(u,x,h,uf,xf,du,dx,dh,duf,dxf,S);
      for(i=0;i<3;i++) S->alfbn[i] = du[i];
      MxV(S->B[0].CN,&du[Nu-3],S->abs);

      FindTotalAngMom(S);

}
/**********************************************************************/
/* For spacecraft composed of a single body, with the reference point */
/* placed at the CM, the EOM are vastly simplified.  KaneNBodyEOM     */
/* will work, but this should be faster.                              */
void OneBodyEOM(double *u, double *x, double *h,
                double *uf, double *xf,
                double *udot, double *xdot, double *hdot,
                double *ufdot, double *xfdot,
                struct SCType *S)
{
      struct BodyType *B;
      struct DynType *D;
      struct FlexNodeType *FN;
      double Hb[3],WhlTorq[3],wxH[3],Trq[3];
      double Iinv[3][3];
      long i,j,k;
      long If,Nf,In;


      D = &S->Dyn;
      B = &S->B[0];
      Nf = B->Nf;

/* .. Build H's */
      MxV(B->I,u,Hb);
      for (i=0;i<3;i++) {
         Hb[i] += B->EmbeddedMom[i];
         for (j=0;j<S->Nw;j++){
            Hb[i]+= h[j]*S->Whl[j].A[i];
         }
      }

/* .. Build wheel torque in B0 frame */
      for (i=0;i<3;i++) {
         WhlTorq[i]=0.0;
         for (j=0;j<S->Nw;j++){
            WhlTorq[i]-=S->Whl[j].Trq*S->Whl[j].A[i];
         }
      }

/* .. Angular Rates */
      VxV(u,Hb,wxH);
      Trq[0] = B->Trq[0]-wxH[0]+WhlTorq[0];
      Trq[1] = B->Trq[1]-wxH[1]+WhlTorq[1];
      Trq[2] = B->Trq[2]-wxH[2]+WhlTorq[2];

/* .. Rigid Body EOM */
      MINV3(B->I,Iinv);
      MxV(Iinv,Trq,udot);

/* .. Wheel-body interaction  */
      for (i=0;i<S->Nw;i++){
         hdot[i] = S->Whl[i].Trq;
      }

/* .. Quaternion kinematics */
      xdot[0] = 0.5*( u[0]*x[3]-u[1]*x[2]+u[2]*x[1]);
      xdot[1] = 0.5*( u[0]*x[2]+u[1]*x[3]-u[2]*x[0]);
      xdot[2] = 0.5*(-u[0]*x[1]+u[1]*x[0]+u[2]*x[3]);
      xdot[3] = 0.5*(-u[0]*x[0]-u[1]*x[1]-u[2]*x[2]);

/* .. Flex EOM */
      if (S->FlexActive) {
         for(If=0;If<Nf;If++) {
            D->FlexFrc[If] = 0.0;
            for(k=0;k<Nf;k++)
               D->FlexFrc[If] -= B->Cf[If][k]*uf[k]
                                 + B->Kf[If][k]*xf[k];
            for(In=0;In<B->NumFlexNodes;In++) {
               FN = &B->FlexNode[In];
               D->FlexFrc[If] +=
                  FN->PSI[0][If]*FN->Frc[0] +
                  FN->PSI[1][If]*FN->Frc[1] +
                  FN->PSI[2][If]*FN->Frc[2] +
                  FN->THETA[0][If]*FN->Trq[0] +
                  FN->THETA[1][If]*FN->Trq[1] +
                  FN->THETA[2][If]*FN->Trq[2];
            }
         }
         if (B->MfIsDiagonal) {
            for(If=0;If<Nf;If++) {
               ufdot[If] = D->FlexFrc[If]/B->Mf[If][If];
            }
         }
         else {
            LINSOLVE(B->Mf,ufdot,D->FlexFrc,Nf);
         }
         /* Flex Kinematics */
         for(If=0;If<Nf;If++) {
            xfdot[If] = uf[If];
         }
      }

}
/**********************************************************************/
void OneBodyRK4(struct SCType *S)
{
      struct BodyType *B;
      struct DynType *D;
      struct FlexNodeType *FN;
      struct WhlType *W;
      double *u,*uu,*du,*udot;
      double *x,*xx,*dx,*xdot;
      double *h,*hh,*dh,*hdot;
      double *uf,*uuf,*duf,*ufdot;
      double *xf,*xxf,*dxf,*xfdot;
      long i,In,If;
      long Nf,Nw;

      /* Save some typing (and dereferencing) */
      D = &S->Dyn;
      B = &S->B[0];
      Nw = S->Nw;
      Nf = D->Nf;

      u = D->u;
      x = D->x;
      h = D->h;
      uf = D->uf;
      xf = D->xf;

      uu = D->uu;
      xx = D->xx;
      hh = D->hh;
      uuf = D->uuf;
      xxf = D->xxf;

      du = D->du;
      dx = D->dx;
      dh = D->dh;
      duf = D->duf;
      dxf = D->dxf;

      udot = D->udot;
      xdot = D->xdot;
      hdot = D->hdot;
      ufdot = D->ufdot;
      xfdot = D->xfdot;

      /* State vector initialized in InitRigidDyn() */

/* .. 4th order Runge Kutta */
      /* First Call */
      OneBodyEOM(u,x,h,uf,xf,du,dx,dh,duf,dxf,S);

      for(i=0;i<3;i++)  {
         uu[i] = u[i] + 0.5*DTSIM*du[i];
         udot[i] = du[i]/6.0;
      }
      for(i=0;i<4;i++)  {
         xx[i] = x[i] + 0.5*DTSIM*dx[i];
         xdot[i] = dx[i]/6.0;
      }
      for(i=0;i<Nw;i++)  {
         hh[i] = h[i] + 0.5*DTSIM*dh[i];
         hdot[i] = dh[i]/6.0;
      }
      for(i=0;i<Nf;i++)  {
         uuf[i] = uf[i] + 0.5*DTSIM*duf[i];
         ufdot[i] = duf[i]/6.0;
         xxf[i] = xf[i] + 0.5*DTSIM*dxf[i];
         xfdot[i] = dxf[i]/6.0;
      }

      /* Second Call */
      OneBodyEOM(uu,xx,hh,uuf,xxf,du,dx,dh,duf,dxf,S);

      for(i=0;i<3;i++) {
         uu[i] = u[i] + 0.5*DTSIM*du[i];
         udot[i] += du[i]/3.0;
      }
      for(i=0;i<4;i++) {
         xx[i] = x[i] + 0.5*DTSIM*dx[i];
         xdot[i] += dx[i]/3.0;
      }
      for(i=0;i<Nw;i++) {
         hh[i] = h[i] + 0.5*DTSIM*dh[i];
         hdot[i] += dh[i]/3.0;
      }
      for(i=0;i<Nf;i++)  {
         uuf[i] = uf[i] + 0.5*DTSIM*duf[i];
         ufdot[i] += duf[i]/3.0;
         xxf[i] = xf[i] + 0.5*DTSIM*dxf[i];
         xfdot[i] += dxf[i]/3.0;
      }

      /* Third Call */
      OneBodyEOM(uu,xx,hh,uuf,xxf,du,dx,dh,duf,dxf,S);

      for(i=0;i<3;i++) {
         uu[i] = u[i] + DTSIM*du[i];
         udot[i] += du[i]/3.0;
      }
      for(i=0;i<4;i++) {
         xx[i] = x[i] + DTSIM*dx[i];
         xdot[i] += dx[i]/3.0;
      }
      for(i=0;i<Nw;i++) {
         hh[i] = h[i] + DTSIM*dh[i];
         hdot[i] += dh[i]/3.0;
      }
      for(i=0;i<Nf;i++)  {
         uuf[i] = uf[i] + DTSIM*duf[i];
         ufdot[i] += duf[i]/3.0;
         xxf[i] = xf[i] + DTSIM*dxf[i];
         xfdot[i] += dxf[i]/3.0;
      }

      /* Fourth Call */
      OneBodyEOM(uu,xx,hh,uuf,xxf,du,dx,dh,duf,dxf,S);

      for(i=0;i<3;i++) {
         udot[i] += du[i]/6.0;
         u[i] += udot[i]*DTSIM;
      }
      for(i=0;i<4;i++) {
         xdot[i] += dx[i]/6.0;
         x[i] += xdot[i]*DTSIM;
      }
      for(i=0;i<Nw;i++) {
         hdot[i] += dh[i]/6.0;
         h[i] += hdot[i]*DTSIM;
      }
      for(i=0;i<Nf;i++)  {
         ufdot[i] += duf[i]/6.0;
         uf[i] += ufdot[i]*DTSIM;
         xfdot[i] += dxf[i]/6.0;
         xf[i] += xfdot[i]*DTSIM;
      }

/* .. Map out state variables */
      UNITQ(x);
      for(i=0;i<3;i++) {
         S->B[0].wn[i] = u[i];
         S->B[0].qn[i] = x[i];
      }
      S->B[0].qn[3] = x[3];
      Q2C(S->B[0].qn,S->B[0].CN);

/* .. Wheels */
      for(i=0;i<Nw;i++) {
         W = &S->Whl[i];
         W->H = h[i];
         W->w = h[i]/W->J;
         /* Hi-fi isn't needed for wheel rotor angle */
         W->ang += W->w*DTSIM;
         W->ang = fmod(W->ang,TwoPi);
      }

/* .. Flex */
      if (S->FlexActive) {
         for(If=0;If<B->Nf;If++) {
            B->xi[If] = uf[B->f0+If];
            B->eta[If] = xf[B->f0+If];
         }
         for(In=0;In<B->NumFlexNodes;In++) {
            FN = &B->FlexNode[In];
            for(i=0;i<3;i++) {
               FN->pos[i] = 0.0;
               FN->vel[i] = 0.0;
               FN->ang[i] = 0.0;
               FN->angrate[i] = 0.0;
               for(If=0;If<B->Nf;If++) {
                  FN->pos[i] += FN->PSI[i][If]*B->eta[If];
                  FN->vel[i] += FN->PSI[i][If]*B->xi[If];
                  FN->ang[i] += FN->THETA[i][If]*B->eta[If];
                  FN->angrate[i] += FN->THETA[i][If]*B->xi[If];
               }
               FN->TotAngVel[i] = B->wn[i] + FN->angrate[i];
            }
         }
      }

/* .. Find Total Angular Momentum */
      FindTotalAngMom(S);

/* .. For Accelerometers */
      for(i=0;i<3;i++) {
         S->alfbn[i] = udot[i];
         S->abs[i] = 0.0;
      }
}
/******************************************************************************/
/*  Finds rotational and translational joint partials                         */
/*  On Init, populate all matrix elements.  Else, only populate               */
/*  variable ones.                                                            */
void OrderNJointPartials(struct JointType *G)
{
      double s2,c2,s3,c3;
      double Pw[3][3] = {{0.0,0.0,0.0},{0.0,0.0,0.0},{0.0,0.0,0.0}};
      double Pwdot[3][3] = {{0.0,0.0,0.0},{0.0,0.0,0.0},{0.0,0.0,0.0}};
      double Pv[3][3] = {{0.0,0.0,0.0},{0.0,0.0,0.0},{0.0,0.0,0.0}};
      double CPv[3][3];
      long i1,i2,i3,Cyclic,i,j,k;
      struct BodyType *Bi;
      
      Bi = G->Bi;

      if (G->Init) {
         G->Init = 0;
         
         for(i=0;i<3;i++) {
            for(j=0;j<3;j++) {
               G->Pw[i][j] = 0.0;
               G->Pv[i][j] = 0.0;
               G->Pwdot[i][j] = 0.0;
            }
         }
         for(i=0;i<6;i++) {
            for(j=0;j<6;j++) G->P[i][j] = 0.0;
         }
         
         if (G->IsSpherical) {
            for(i=0;i<3;i++) {
               for(j=0;j<3;j++) G->Pw[i][j] = G->CBoGo[i][j];
            }
         }

         i3 = G->TrnSeq % 10;         /* Pick off third digit */
         i2 = (G->TrnSeq % 100)/10;   /* Extract second digit */
         i1 = G->TrnSeq/100;          /* Pick off first digit */

         Pv[i1-1][0] = 1.0;
         Pv[i2-1][1] = 1.0;
         Pv[i3-1][2] = 1.0;
         MTxM(G->CGiBi,Pv,G->Pv);
      }

      if (!G->IsSpherical) {
         i3 = G->RotSeq % 10;         /* Pick off third digit */
         i2 = (G->RotSeq % 100)/10;   /* Extract second digit */
         i1 = G->RotSeq/100;          /* Pick off first digit */

         s2 = sin(G->Ang[1]);
         c2 = cos(G->Ang[1]);
         s3 = sin(G->Ang[2]);
         c3 = cos(G->Ang[2]);

         Cyclic = (i2-i1)*(i3-i2)*(i3-i1);
         /* Convert (123) style to [012] subscripts */
         i1--;
         i2--;
         i3--;
         if (Cyclic > 0) {  /* 123, 231, 312 */
            Pw[i1][0] = c2*c3;
            Pw[i1][1] = s3;
            Pw[i2][0] = -c2*s3;
            Pw[i2][1] = c3;
            Pw[i3][0] = s2;
            Pw[i3][2] = 1.0;
            
            Pwdot[i1][0] = -s2*c3*G->AngRate[1]-c2*s3*G->AngRate[2];
            Pwdot[i1][1] = c3*G->AngRate[2];
            Pwdot[i2][0] = s2*s3*G->AngRate[1]-c2*c3*G->AngRate[2];
            Pwdot[i2][1] = -s3*G->AngRate[2];
            Pwdot[i3][0] = c2*G->AngRate[1];
         }
         else if (Cyclic < 0) {  /* 321, 132, 213 */
            Pw[i1][0] = c2*c3;
            Pw[i1][1] = -s3;
            Pw[i2][0] = c2*s3;
            Pw[i2][1] = c3;
            Pw[i3][0] = -s2;
            Pw[i3][2] = 1.0;

            Pwdot[i1][0] = -s2*c3*G->AngRate[1]-c2*s3*G->AngRate[2];
            Pwdot[i1][1] = -c3*G->AngRate[2];
            Pwdot[i2][0] = -s2*s3*G->AngRate[1]+c2*c3*G->AngRate[2];
            Pwdot[i2][1] = -s3*G->AngRate[2];
            Pwdot[i3][0] = -c2*G->AngRate[1];
         }
         else {
            printf("RotSeq %ld is not a Body-3 Sequence, so is not supported.\n",G->RotSeq);
            exit(1);
         }
         MxM(G->CBoGo,Pw,G->Pw);
         MxM(G->CBoGo,Pwdot,G->Pwdot);
      }
      
      /* Pw is expressed in Bo, Pv is expressed in Bi */
      /* Express P in Bo */
      for(i=0;i<3;i++) {
         for(j=0;j<G->TrnDOF;j++) {
            CPv[i][j] = 0.0;
            for(k=0;k<3;k++) CPv[i][j] += G->COI[i][k]*G->Pv[k][j];
         }
      }
      for(i=0;i<3;i++) {
         for(j=0;j<G->RotDOF;j++) G->P[i][j] = G->Pw[i][j];
         for(j=0;j<G->TrnDOF;j++) G->P[3+i][G->RotDOF+j] = CPv[i][j];
      }
}
/******************************************************************************/
void MINV1to6(double A[6][6], double AI[6][6], long N)
{
      long I,J,ROW;
      long IPIVOT = 0;
      double M[6][6];
      double PIVOT,K,TA[6],TB[6];

      for(I=0;I<N;I++){
         for(J=0;J<N;J++){
            M[I][J] = A[I][J];
            AI[I][J] = 0.0;
         }
         AI[I][I] = 1.0;
      }

      for(ROW=0;ROW<N;ROW++){
         PIVOT = M[ROW][ROW];
         IPIVOT = ROW;
         for(I=ROW+1;I<N;I++){
            if (fabs(M[I][ROW]) > fabs(PIVOT)){
               PIVOT = M[I][ROW];
               IPIVOT = I;
            }
         }
         if (PIVOT == 0.0){
            printf("Matrix is singular in MINV1to6\n");
            exit(1);
         }

         for(J=0;J<N;J++){
            TA[J] = M[IPIVOT][J];
            TB[J] = AI[IPIVOT][J];
            M[IPIVOT][J] = M[ROW][J];
            AI[IPIVOT][J] = AI[ROW][J];
            M[ROW][J] = TA[J] / PIVOT;
            AI[ROW][J] = TB[J] / PIVOT;
         }
         for(I=ROW+1;I<N;I++){
            K = M[I][ROW];
            for(J=0;J<N;J++){
               M[I][J] = M[I][J] - K * M[ROW][J];
               AI[I][J] = AI[I][J] - K * AI[ROW][J];
            }
         }
      }

/*    M is now upper diagonal */

      for(ROW=N-1;ROW>0;ROW--){
         for(I=0;I<ROW;I++){
            K = M[I][ROW];
            for(J=0;J<N;J++){
               M[I][J] = M[I][J] - K * M[ROW][J];
               AI[I][J] = AI[I][J] - K * AI[ROW][J];
            }
         }
      }
}
/******************************************************************************/
void ShiftArtFrc(double F[6], double r[3], double Fbar[6])
{
      double rxF[3];
      
      VxV(r,&F[3],rxF);

      Fbar[0] = F[0] - rxF[0];
      Fbar[1] = F[1] - rxF[1];
      Fbar[2] = F[2] - rxF[2];
      Fbar[3] = F[3];
      Fbar[4] = F[4];
      Fbar[5] = F[5];
}
/******************************************************************************/
void ShiftSpatAcc(double a[6], double r[3], double abar[6])
{
      double axr[3];
      
      VxV(&a[0],r,axr);

      abar[0] = a[0];
      abar[1] = a[1];
      abar[2] = a[2];
      abar[3] = a[3] + axr[0];
      abar[4] = a[4] + axr[1];
      abar[5] = a[5] + axr[2];
}
/******************************************************************************/
void ShiftArtMass(double A[6][6], double r[3], double B[6][6])
{
      double rx[3][3],rxA21[3][3],A12xr[3][3],rxA22[3][3],A22xr[3][3],rxA22xr[3][3];
      long i,j,k;

      rx[0][0] =  0.0;
      rx[1][1] =  0.0;
      rx[2][2] =  0.0;
      rx[2][1] =  r[0];
      rx[0][2] =  r[1];
      rx[1][0] =  r[2];
      rx[1][2] = -r[0];
      rx[2][0] = -r[1];
      rx[0][1] = -r[2];

      for(i=0;i<3;i++) {
         for(j=0;j<3;j++) {
            rxA21[i][j] = 0.0;
            A12xr[i][j] = 0.0;
            rxA22[i][j] = 0.0;
            A22xr[i][j] = 0.0;
            for(k=0;k<3;k++) {
               rxA21[i][j] += rx[i][k]*A[3+k][j];
               A12xr[i][j] += A[i][3+k]*rx[k][j];
               rxA22[i][j] += rx[i][k]*A[3+k][3+j];
               A22xr[i][j] += A[3+i][3+k]*rx[k][j];
            }
         }
      }
      MxM(rxA22,rx,rxA22xr);
      
      for(i=0;i<3;i++) {
         for(j=0;j<3;j++) {
            for(k=0;k<3;k++) {
               B[  i][  j] = A[  i][  j] - rxA21[i][j] + A12xr[i][j] - rxA22xr[i][j];
               B[  i][3+j] = A[  i][3+j] - rxA22[i][j];
               B[3+i][  j] = A[3+i][  j] + A22xr[i][j];
            }
            B[3+i][3+j] = A[3+i][3+j];
         }
      }
      
}
/******************************************************************************/
void RotateSpatVec(double CBA[3][3],double Va[6],double Vb[6])
{
      long i,j;
      
      for(i=0;i<3;i++) {
         Vb[i] = 0.0;
         Vb[3+i] = 0.0;
         for(j=0;j<3;j++) {
            Vb[i] += CBA[i][j]*Va[j];
            Vb[3+i] += CBA[i][j]*Va[3+j];
         }
      }
}
/******************************************************************************/
void RotateSpatMat(double CBA[3][3],double Ma[6][6],double Mb[6][6])
{
      double CM11[3][3],CM12[3][3],CM21[3][3],CM22[3][3];
      long i,j,k;
      
      for(i=0;i<3;i++) {
         for(j=0;j<3;j++) {
            CM11[i][j] = 0.0;
            CM12[i][j] = 0.0;
            CM21[i][j] = 0.0;
            CM22[i][j] = 0.0;
            for(k=0;k<3;k++) {
               CM11[i][j] += CBA[i][k]*Ma[  k][  j];
               CM12[i][j] += CBA[i][k]*Ma[  k][3+j];
               CM21[i][j] += CBA[i][k]*Ma[3+k][  j];
               CM22[i][j] += CBA[i][k]*Ma[3+k][3+j];
            }
         }
      }
      
      for(i=0;i<3;i++) {
         for(j=0;j<3;j++) {
            Mb[  i][  j] = 0.0;
            Mb[  i][3+j] = 0.0;
            Mb[3+i][  j] = 0.0;
            Mb[3+i][3+j] = 0.0;
            for(k=0;k<3;k++) {
               Mb[  i][  j] += CM11[i][k]*CBA[j][k];
               Mb[  i][3+j] += CM12[i][k]*CBA[j][k];
               Mb[3+i][  j] += CM21[i][k]*CBA[j][k];
               Mb[3+i][3+j] += CM22[i][k]*CBA[j][k];
            }
         }
      }
}
/******************************************************************************/
void OrderNJointCOI(struct JointType *G)
{
      double CBoGi[3][3];
      
      if (G->IsSpherical) {
         Q2C(G->q,G->CGoGi);
         C2A(G->RotSeq,G->CGoGi,&G->Ang[0],&G->Ang[1],&G->Ang[2]);
      }
      else A2C(G->RotSeq,G->Ang[0],G->Ang[1],G->Ang[2],G->CGoGi);
      
      MxM(G->CBoGo,G->CGoGi,CBoGi);
      MxM(CBoGi,G->CGiBi,G->COI);
}
/**********************************************************************/
void OrderNPassiveJointFrcTrq(struct SCType *S)
{
      long Ig,i;
      struct JointType *G;
      double a[3];

      for(Ig=0;Ig<S->Ng;Ig++) {
         G = &S->G[Ig];
         if (G->IsSpherical) {
            Q2AngleVec(G->q,a);
            for(i=0;i<3;i++) {
               G->PassiveTrq[i] = -G->RotDampCoef[i]*G->AngRate[i]
                                 - G->RotSpringCoef[i]*a[i];
            }
         }
         else {
            for(i=0;i<G->RotDOF;i++) {
               G->PassiveTrq[i] = -G->RotDampCoef[i]*G->AngRate[i]
                                  -G->RotSpringCoef[i]*G->Ang[i];
            }
         }

         for(i=0;i<G->TrnDOF;i++) {
            G->PassiveFrc[i] = -G->TrnDampCoef[i]*G->PosRate[i]
                               -G->TrnSpringCoef[i]*G->Pos[i];
         }

      }
}
/******************************************************************************/
void ScatterStates(struct JointType *G)
{
      struct BodyType *Bi,*Bo;
      double Pwu[3],Pvu[3],Pdwu[3];
      double pni[3],vi[3],Cvi[3],wxPvu[3],ai[3],Cai[3];
      double wxri[3],wxro[3],Calfri[3],wxPwu[3];
      double axri[3],axro[3],wxwxri[3];
      double wxwxro[3],Cwi[3];
      double Iw[3],Ialfr[3],wxH[3];
      long i,j;

      Bi = G->Bi;
      Bo = G->Bo;
      
      for(i=0;i<3;i++) {
         Pwu[i] = 0.0;
         Pvu[i] = 0.0;
         Pdwu[i] = 0.0;
         for(j=0;j<G->RotDOF;j++) {
            Pwu[i] += G->Pw[i][j]*G->AngRate[j];
            Pdwu[i] += G->Pwdot[i][j]*G->AngRate[j];
         }
         for(j=0;j<G->TrnDOF;j++) {         
            Pvu[i] += G->Pv[i][j]*G->PosRate[j];
         }
      }
      
      MxM(G->COI,Bi->CN,Bo->CN);
      
      for(i=0;i<3;i++) pni[i] = Bi->pn[i] + G->riplusPx[i];
      MxV(G->COI,pni,Bo->pn);
      for(i=0;i<3;i++) Bo->pn[i] -= G->RigidRout[i]; 
      
      /* Velocities */
      MxV(G->COI,Bi->wn,Cwi);
      for(i=0;i<3;i++) Bo->wn[i] = Cwi[i] + Pwu[i];
      
      VxV(Bi->wn,G->riplusPx,wxri);
      VxV(Bo->wn,G->RigidRout,wxro);
      for(i=0;i<3;i++) vi[i] = Bi->vn[i] + Pvu[i] + wxri[i];
      MxV(G->COI,vi,Cvi);
      for(i=0;i<3;i++) {
         Bo->vn[i] = Cvi[i] - wxro[i];
      }

      /* Remainder Accelerations */
      MxV(G->COI,Bi->RemAlf,Calfri);
      VxV(Bo->wn,Pwu,wxPwu);
      for(i=0;i<3;i++) {
         Bo->RemAlf[i] = Calfri[i] + Pdwu[i] + wxPwu[i];
      }
      
      VxV(Bi->RemAlf,G->riplusPx,axri);
      VxV(Bi->wn,wxri,wxwxri);
      VxV(Bi->wn,Pvu,wxPvu);
      for(i=0;i<3;i++) ai[i] = Bi->RemAcc[i] + 2.0*wxPvu[i] + axri[i] + wxwxri[i] ;
      MxV(G->COI,ai,Cai);
      VxV(Bo->RemAlf,G->RigidRout,axro);
      VxV(Bo->wn,wxro,wxwxro);
      for(i=0;i<3;i++) {  
         Bo->RemAcc[i] = Cai[i] - axro[i] - wxwxro[i];
      }

      MxV(Bo->I,Bo->wn,Iw);
      for(i=0;i<3;i++) Bo->H[i] = Iw[i] + Bo->WhlMom[i] + Bo->EmbeddedMom[i];

      MxV(Bo->I,Bo->RemAlf,Ialfr);
      VxV(Bo->wn,Bo->H,wxH);
      for(i=0;i<3;i++) {
         Bo->RemInertiaFrc[i] = -Ialfr[i] - wxH[i];
         Bo->RemInertiaFrc[3+i] = -Bo->mass*Bo->RemAcc[i];
      }
}
/******************************************************************************/
void GatherMassAndForce(struct JointType *G,struct SCType *S)
{
      struct BodyType *Bo;
      struct JointType *Gd;
      double F[6],TF[6],CTF[6],rdk[3],SCTF[6];
      double M[6][6],TM[6][6],CTMC[6][6],SCTMCS[6][6];
      double Coc[3][3];
      long i,Id,j,k;
      
      Bo = G->Bo;
      
      /* Articulated-Body Force */
      for(i=0;i<6;i++) {
         F[i] = Bo->SpatFrc[i] + Bo->RemInertiaFrc[i];
      }
      ShiftArtFrc(F,G->RigidRout,G->ArtFrc);
      for(Id=0;Id<Bo->Nd;Id++) {
         Gd = &S->G[Bo->Gd[Id]];
         for(i=0;i<6;i++) {
            TF[i] = 0.0;
            for(j=0;j<6;j++) TF[i] += Gd->TransMtx[i][j]*Gd->ArtFrc[j];
         }
         MT(Gd->COI,Coc);
         RotateSpatVec(Coc,TF,CTF);
         for(i=0;i<3;i++) rdk[i] = G->RigidRout[i] - Gd->riplusPx[i]; 
         ShiftArtFrc(CTF,rdk,SCTF);
         for(i=0;i<6;i++) G->ArtFrc[i] += SCTF[i];
      }
      
      /* Articulated-Body Mass */
      for(i=0;i<3;i++) {
         for(j=0;j<3;j++) {
            M[i][j] = Bo->I[i][j];
            M[i][3+j] = 0.0;
            M[3+i][j] = 0.0;
            M[3+i][3+j] = 0.0;
         }
         M[3+i][3+i] = Bo->mass;
      }
      ShiftArtMass(M,G->RigidRout,G->ArtMass);
      for(Id=0;Id<Bo->Nd;Id++) {
         Gd = &S->G[Bo->Gd[Id]];
         for(i=0;i<6;i++) {
            for(j=0;j<6;j++) {
               TM[i][j] = 0.0;
               for(k=0;k<6;k++) {
                  TM[i][j] += Gd->TransMtx[i][k]*Gd->ArtMass[k][j];
               }
            }
         }
         MT(Gd->COI,Coc);
         RotateSpatMat(Coc,TM,CTMC);
         for(i=0;i<3;i++) rdk[i] = G->ro[i] - Gd->riplusPx[i]; 
         ShiftArtMass(CTMC,rdk,SCTMCS);
         for(i=0;i<6;i++) {
            for(j=0;j<6;j++) G->ArtMass[i][j] += SCTMCS[i][j];
         }
      }
}
/******************************************************************************/
void GatherDynMtx(struct JointType *G,struct SCType *S)
{
      double MP[6][6];
      long i,j,k;

      /* Dynamic Matrix, D */
      for(i=0;i<6;i++) {
         for(j=0;j<G->Nu;j++) {
            MP[i][j] = 0.0;
            for(k=0;k<6;k++) MP[i][j] += G->ArtMass[i][k]*G->P[k][j];
         }
      }
      for(i=0;i<G->Nu;i++) {
         for(j=0;j<G->Nu;j++) {
            G->DynMtx[i][j] = 0.0;
            for(k=0;k<6;k++) G->DynMtx[i][j] += G->P[k][i]*MP[k][j];
         }
      }
      MINV1to6(G->DynMtx,G->InvDynMtx,G->Nu);

      /* Absorption Matrix, A */
      for(i=0;i<G->Nu;i++) {
         for(j=0;j<6;j++) {
            G->InvDynPT[i][j] = 0.0;
            for(k=0;k<G->Nu;k++) 
               G->InvDynPT[i][j] += G->InvDynMtx[i][k]*G->P[j][k];
         }
      }
      /* TODO: Save a few cycles by not computing A and T for GN (A = U, T = 0) */
      for(i=0;i<6;i++) {
         for(j=0;j<6;j++) {
            G->AbsorpMtx[i][j] = 0.0;
            for(k=0;k<G->Nu;k++) G->AbsorpMtx[i][j] += MP[i][k]*G->InvDynPT[k][j];
         }
      }
      
      /* Transmission Matrix, T */
      for(i=0;i<6;i++) {
         for(j=0;j<6;j++) G->TransMtx[i][j] = -G->AbsorpMtx[i][j];
         G->TransMtx[i][i] += 1.0;
      }
}
/******************************************************************************/
void ScatterStateDerivatives(struct JointType *G)
{
      struct BodyType *Bi,*Bo;
      double Ma[6],Saui[6],CSaui[6],F[6],CSauiPudot[6],rko[3];
      long i,j;
      
      Bi = G->Bi;
      Bo = G->Bo;
      
      ShiftSpatAcc(Bi->AccU,G->riplusPx,Saui); 
      RotateSpatVec(G->COI,Saui,CSaui);
            
      for(i=0;i<6;i++) {
         Ma[i] = 0.0;
         for(j=0;j<6;j++) Ma[i] += G->ArtMass[i][j]*CSaui[j];
         F[i] = G->ArtFrc[i] - Ma[i];
      }
      for(i=0;i<G->Nu;i++) {
         G->udot[i] = 0.0;
         for(j=0;j<6;j++) G->udot[i] += G->InvDynPT[i][j]*F[j];
      }
      for(i=0;i<6;i++) {
         CSauiPudot[i] = CSaui[i];
         for(j=0;j<G->Nu;j++) CSauiPudot[i] += G->P[i][j]*G->udot[j];
      }
      for(i=0;i<3;i++) rko[i] = -G->RigidRout[i];
      ShiftSpatAcc(CSauiPudot,rko,Bo->AccU);
}
/******************************************************************************/
void OrderNMultiBodyEOM(struct SCType *S) 
{
      struct BodyType *B,*Bi,*Bo;
      struct JointType *G;
      struct WhlType *W;
      double Iow[3],wxH[3];
      double TrqBo[3],TrqGo[3],TrqBi[3];
      double FrcBo[3],FrcBi[3];
      double rxFi[3],rxFo[3];
      long i,j,Ib,Ig,Iw;
      
      for(Ib=0;Ib<S->Nb;Ib++) {
         B = &S->B[Ib];
         for(i=0;i<3;i++) B->WhlMom[i] = 0.0;
      }
      for(Iw=0;Iw<S->Nw;Iw++) {
         W = &S->Whl[Iw];
         B = &S->B[W->Body];
         for(i=0;i<3;i++) B->WhlMom[i] += W->H*W->A[i];
      }
            
      /* First Pass: Root to tips */
      B = &S->B[0];
      G = &S->GN;
      OrderNJointCOI(G);
      OrderNJointPartials(G);
      for(i=0;i<3;i++) {
         B->wn[i] = G->AngRate[i];
         for(j=0;j<3;j++) B->CN[i][j] = G->COI[i][j];
      }
      MxV(B->CN,G->Pos,B->pn);
      MxV(B->CN,G->PosRate,B->vn);
      MxV(B->I,B->wn,Iow);
      for(i=0;i<3;i++) B->H[i] = Iow[i] + B->WhlMom[i] + B->EmbeddedMom[i];
      VxV(B->wn,B->H,wxH);
      for(i=0;i<3;i++) {
         B->RemInertiaFrc[i] = -wxH[i];
         B->RemInertiaFrc[3+i] = 0.0;
      }
      for(Ig=0;Ig<S->Ng;Ig++) {
         G = &S->G[Ig];
         OrderNJointCOI(G);
         OrderNJointPartials(G);
         for(i=0;i<3;i++) {
            G->riplusPx[i] = G->RigidRin[i];
            for(j=0;j<G->TrnDOF;j++) {
               G->riplusPx[i] += G->Pv[i][j]*G->Pos[j];
            }
         }
         ScatterStates(G);
      }
      
      if (S->PassiveJointFrcTrqEnabled) {
         OrderNPassiveJointFrcTrq(S);
      }
      /* Apply joint torques/forces to bodies */
      for(Ig=0;Ig<S->Ng;Ig++) {
         G = &S->G[Ig];
         Bi = G->Bi;
         Bo = G->Bo;
         for(i=0;i<3;i++) {
            FrcBi[i] = 0.0;
            TrqGo[i] = 0.0;
            for(j=0;j<G->RotDOF;j++) {
               TrqGo[i] += G->Pw[i][j]*(G->Trq[j]+G->PassiveTrq[j]);
            }
            for(j=0;j<G->TrnDOF;j++) {
               FrcBi[i] += G->Pv[i][j]*(G->Frc[j]+G->PassiveFrc[j]);
            }
         }
         /* Force Transformations*/
         MxV(G->COI,FrcBi,FrcBo);
         VxV(G->riplusPx,FrcBi,rxFi); 
         VxV(G->RigidRout,FrcBo,rxFo);

         /* Torque Transformations */
         MTxV(G->CTrqBi,TrqGo,TrqBi);
         MTxV(G->CTrqBo,TrqGo,TrqBo);

         for(i=0;i<3;i++) {
            Bi->SpatFrc[  i] -= TrqBi[i] + rxFi[i];
            Bo->SpatFrc[  i] += TrqBo[i] + rxFo[i];
            Bi->SpatFrc[3+i] -= FrcBi[i];
            Bo->SpatFrc[3+i] += FrcBo[i];
         }
      }
      
      /* Second Pass: Tips to Root */
      for(Ig=S->Ng-1;Ig>=0;Ig--) {
         G = &S->G[Ig];
         GatherMassAndForce(G,S);
         GatherDynMtx(G,S);
      }
      G = &S->GN;
      GatherMassAndForce(G,S);
      GatherDynMtx(G,S);
      
      /* Third Pass: Root to tips */
      B = &S->B[0];
      G = &S->GN;
      for(i=0;i<6;i++) {
         G->udot[i] = 0.0;
         for(j=0;j<6;j++) G->udot[i] += G->InvDynPT[i][j]*G->ArtFrc[j];
      }
      for(i=0;i<3;i++) {
         B->AccU[i] = G->udot[i];
         B->AccU[3+i] = B->CN[i][0]*G->udot[3]
                       +B->CN[i][1]*G->udot[4]
                       +B->CN[i][2]*G->udot[5];
      }
      
      for(Ig=0;Ig<S->Ng;Ig++) ScatterStateDerivatives(&S->G[Ig]);
      
      /* Kinematic EOM */
      G = &S->GN;
      QW2QDOT(G->q,G->AngRate,G->qdot);
      for(i=0;i<3;i++) G->xdot[i] = G->PosRate[i];
      for(Ig=0;Ig<S->Ng;Ig++) {
         G = &S->G[Ig];
         if (G->IsSpherical) QW2QDOT(G->q,G->AngRate,G->qdot);
         else for(i=0;i<G->RotDOF;i++) G->qdot[i] = G->AngRate[i];
         for(i=0;i<G->TrnDOF;i++) G->xdot[i] = G->PosRate[i];
      }
      
      /* Wheel EOM */
      for(Iw=0;Iw<S->Nw;Iw++) {
         W = &S->Whl[Iw];
         W->Hdot = W->Trq;
      }
      
}
/******************************************************************************/
void OrderNMultiBodyRK4(struct SCType *S)
{
      struct DynType *D; /* Copy to/from D->u, D->x */
      struct BodyType *B;
      struct JointType *G;
      struct WhlType *W;
      double f,dt;
      long i,Ig,Iw,Ib;
      
      /* Copy states from Dyn */
      D = &S->Dyn;
      G = &S->GN;
      for(i=0;i<3;i++) {
         G->RKum[i] = D->u[i];
         G->RKum[3+i] = D->u[D->Nu-3+i];
         G->RKxm[i] = D->x[D->Nx-3+i];
      }
      for(i=0;i<4;i++) {
         G->RKqm[i] = D->x[i];
      }
      
      for(Ig=0;Ig<S->Ng;Ig++) {
         G = &S->G[Ig];
         for(i=0;i<G->RotDOF;i++) {
            G->RKum[i] = D->u[G->Rotu0+i];
         }
         for(i=0;i<G->TrnDOF;i++) {
            G->RKum[G->RotDOF+i] = D->u[G->Trnu0+i];
            G->RKxm[i] = D->x[G->Trnx0+i];
         }
         if (G->IsSpherical) {
            for(i=0;i<4;i++) {
               G->RKqm[i] = D->x[G->Rotx0+i];
            }
         }
         else {
            for(i=0;i<G->RotDOF;i++) {
               G->RKqm[i] = D->x[G->Rotx0+i];
            }
         }
      }
      
      for(Iw=0;Iw<S->Nw;Iw++) {
         W = &S->Whl[Iw];
         W->RKHm = D->h[Iw];
      }
      
      /* Set up for First Call */
      for(Ib=0;Ib<S->Nb;Ib++) {
         B = &S->B[Ib];
         for(i=0;i<3;i++) {
            B->SpatFrc[  i] = B->Trq[i];
            B->SpatFrc[3+i] = B->FrcB[i];
         }
      }
      G = &S->GN;
      for(i=0;i<3;i++) {
         G->AngRate[i] = G->RKum[i];
         G->PosRate[i] = G->RKum[3+i];
         G->Pos[i] = G->RKxm[i];
      }
      for(i=0;i<4;i++) {
         G->q[i] = G->RKqm[i];
      }
      
      for(Ig=0;Ig<S->Ng;Ig++) {
         G = &S->G[Ig];
         for(i=0;i<G->RotDOF;i++) {
            G->AngRate[i] = G->RKum[i];
         }
         for(i=0;i<G->TrnDOF;i++) {
            G->PosRate[i] = G->RKum[G->RotDOF+i];
            G->Pos[i] = G->RKxm[i];
         }
         if (G->IsSpherical) {
            for(i=0;i<4;i++) {
               G->q[i] = G->RKqm[i];
            }
         }
         else {
            for(i=0;i<G->RotDOF;i++) {
               G->Ang[i] = G->RKqm[i];
            }
         }
      }
      
      for(Iw=0;Iw<S->Nw;Iw++) {
         W = &S->Whl[Iw];
         W->H = W->RKHm;
      }
      
      /* First Call */
      OrderNMultiBodyEOM(S);
      
      /* Digest First Call */
      f = 1.0/6.0;
      G = &S->GN;
      for(i=0;i<3;i++) {
         G->RKdu[  i] = f*G->udot[i];
         G->RKdu[3+i] = f*G->udot[3+i];
         G->RKdx[  i] = f*G->xdot[i];         
      }
      for(i=0;i<4;i++) {
         G->RKdq[  i] = f*G->qdot[i];
      }
      
      for(Ig=0;Ig<S->Ng;Ig++) {
         G = &S->G[Ig];
         for(i=0;i<G->RotDOF;i++) {
            G->RKdu[i] = f*G->udot[i];
         }
         for(i=0;i<G->TrnDOF;i++) {
            G->RKdu[G->RotDOF+i] = f*G->udot[G->RotDOF+i];
            G->RKdx[i] = f*G->xdot[i];
         }
         if (G->IsSpherical) {
            for(i=0;i<4;i++) {
               G->RKdq[i] = f*G->qdot[i];
            }
         }
         else {
            for(i=0;i<G->RotDOF;i++) {
               G->RKdq[i] = f*G->qdot[i];
            }
         }
      }
      
      for(Iw=0;Iw<S->Nw;Iw++) {
         W = &S->Whl[Iw];
         W->RKdH = f*W->Hdot;
      }

      /* Set up for Second Call */
      dt = 0.5*DTSIM;
      for(Ib=0;Ib<S->Nb;Ib++) {
         B = &S->B[Ib];
         for(i=0;i<3;i++) {
            B->SpatFrc[  i] = B->Trq[i];
            B->SpatFrc[3+i] = B->FrcB[i];
         }
      }
      G = &S->GN;
      for(i=0;i<3;i++) {
         G->AngRate[i] = G->RKum[i] + dt*G->udot[i];
         G->PosRate[i] = G->RKum[3+i]+dt*G->udot[3+i];
         G->Pos[i] = G->RKxm[i] + dt*G->xdot[i];
      }
      for(i=0;i<4;i++) {
         G->q[i] = G->RKqm[i] + dt*G->qdot[i]; 
      }
      
      for(Ig=0;Ig<S->Ng;Ig++) {
         G = &S->G[Ig];
         for(i=0;i<G->RotDOF;i++) {
            G->AngRate[i] = G->RKum[i] + dt*G->udot[i];
         }
         for(i=0;i<G->TrnDOF;i++) {
            G->PosRate[i] = G->RKum[G->RotDOF+i]+dt*G->udot[G->RotDOF+i];
            G->Pos[i] = G->RKxm[i] + dt*G->xdot[i];
         }
         if (G->IsSpherical) {
            for(i=0;i<4;i++) {
               G->q[i] = G->RKqm[i] + dt*G->qdot[i]; 
            }
         }
         else {
            for(i=0;i<G->RotDOF;i++) {
               G->Ang[i] = G->RKqm[i] + dt*G->qdot[i]; /* "q" here used for angle states */
            }
         }
      }
      
      for(Iw=0;Iw<S->Nw;Iw++) {
         W = &S->Whl[Iw];
         W->H = W->RKHm + dt*W->Hdot;
      }
      
      /* Second Call */
      OrderNMultiBodyEOM(S);
      
      /* Digest Second Call */
      f = 1.0/3.0;
      G = &S->GN;
      for(i=0;i<3;i++) {
         G->RKdu[  i] += f*G->udot[i];
         G->RKdu[3+i] += f*G->udot[3+i];
         G->RKdx[  i] += f*G->xdot[i];         
      }
      for(i=0;i<4;i++) {
         G->RKdq[i] += f*G->qdot[i];
      }
      
      for(Ig=0;Ig<S->Ng;Ig++) {
         G = &S->G[Ig];
         for(i=0;i<G->RotDOF;i++) {
            G->RKdu[i] += f*G->udot[i];
         }
         for(i=0;i<G->TrnDOF;i++) {
            G->RKdu[G->RotDOF+i] += f*G->udot[G->RotDOF+i];
            G->RKdx[i] += f*G->xdot[i];
         }
         if (G->IsSpherical) {
            for(i=0;i<4;i++) {
               G->RKdq[i] += f*G->qdot[i];
            }
         }
         else {
            for(i=0;i<G->RotDOF;i++) {
               G->RKdq[i] += f*G->qdot[i];
            }
         }
      }
      
      for(Iw=0;Iw<S->Nw;Iw++) {
         W = &S->Whl[Iw];
         W->RKdH += f*W->Hdot;
      }
            
      /* Set up for Third Call */
      dt = 0.5*DTSIM;
      for(Ib=0;Ib<S->Nb;Ib++) {
         B = &S->B[Ib];
         for(i=0;i<3;i++) {
            B->SpatFrc[  i] = B->Trq[i];
            B->SpatFrc[3+i] = B->FrcB[i];
         }
      }
      G = &S->GN;
      for(i=0;i<3;i++) {
         G->AngRate[i] = G->RKum[i] + dt*G->udot[i];
         G->PosRate[i] = G->RKum[3+i] + dt*G->udot[3+i];
         G->Pos[i] = G->RKxm[i] + dt*G->xdot[i];
      }
      for(i=0;i<4;i++) {
         G->q[i] = G->RKqm[i] + dt*G->qdot[i]; 
      }
      
      for(Ig=0;Ig<S->Ng;Ig++) {
         G = &S->G[Ig];
         for(i=0;i<G->RotDOF;i++) {
            G->AngRate[i] = G->RKum[i] + dt*G->udot[i];
         }
         for(i=0;i<G->TrnDOF;i++) {
            G->PosRate[i] = G->RKum[G->RotDOF+i]+dt*G->udot[G->RotDOF+i];
            G->Pos[i] = G->RKxm[i] + dt*G->xdot[i];
         }
         if (G->IsSpherical) {
            for(i=0;i<4;i++) {
               G->q[i] = G->RKqm[i] + dt*G->qdot[i]; 
            }
         }
         else {
            for(i=0;i<G->RotDOF;i++) {
               G->Ang[i] = G->RKqm[i] + dt*G->qdot[i]; /* "q" here used for angle states */
            }
         }
      }
      
      for(Iw=0;Iw<S->Nw;Iw++) {
         W = &S->Whl[Iw];
         W->H = W->RKHm + dt*W->Hdot;
      }

      /* Third Call */
      OrderNMultiBodyEOM(S);

      /* Digest Third Call */
      f = 1.0/3.0;
      G = &S->GN;
      for(i=0;i<3;i++) {
         G->RKdu[  i] += f*G->udot[i];
         G->RKdu[3+i] += f*G->udot[3+i];
         G->RKdx[  i] += f*G->xdot[i];         
      }
      for(i=0;i<4;i++) {
         G->RKdq[i] += f*G->qdot[i];
      }
      
      for(Ig=0;Ig<S->Ng;Ig++) {
         G = &S->G[Ig];
         for(i=0;i<G->RotDOF;i++) {
            G->RKdu[i] += f*G->udot[i];
         }
         for(i=0;i<G->TrnDOF;i++) {
            G->RKdu[G->RotDOF+i] += f*G->udot[G->RotDOF+i];
            G->RKdx[i] += f*G->xdot[i];
         }
         if (G->IsSpherical) {
            for(i=0;i<4;i++) {
               G->RKdq[i] += f*G->qdot[i];
            }
         }
         else {
            for(i=0;i<G->RotDOF;i++) {
               G->RKdq[i] += f*G->qdot[i];
            }
         }
      }
      
      for(Iw=0;Iw<S->Nw;Iw++) {
         W = &S->Whl[Iw];
         W->RKdH += f*W->Hdot;
      }
            
      /* Set up for Fourth Call */
      dt = DTSIM;
      for(Ib=0;Ib<S->Nb;Ib++) {
         B = &S->B[Ib];
         for(i=0;i<3;i++) {
            B->SpatFrc[  i] = B->Trq[i];
            B->SpatFrc[3+i] = B->FrcB[i];
         }
      }
      G = &S->GN;
      for(i=0;i<3;i++) {
         G->AngRate[i] = G->RKum[i] + dt*G->udot[i];
         G->PosRate[i] = G->RKum[3+i] + dt*G->udot[3+i];
         G->Pos[i] = G->RKxm[i] + dt*G->xdot[i];
      }
      for(i=0;i<4;i++) {
         G->q[i] = G->RKqm[i] + dt*G->qdot[i]; 
      }
      
      for(Ig=0;Ig<S->Ng;Ig++) {
         G = &S->G[Ig];
         for(i=0;i<G->RotDOF;i++) {
            G->AngRate[i] = G->RKum[i] + dt*G->udot[i];
         }
         for(i=0;i<G->TrnDOF;i++) {
            G->PosRate[i] = G->RKum[G->RotDOF+i] + dt*G->udot[G->RotDOF+i];
            G->Pos[i] = G->RKxm[i] + dt*G->xdot[i];
         }
         if (G->IsSpherical) {
            for(i=0;i<4;i++) {
               G->q[i] = G->RKqm[i] + dt*G->qdot[i]; 
            }
         }
         else {
            for(i=0;i<G->RotDOF;i++) {
               G->Ang[i] = G->RKqm[i] + dt*G->qdot[i]; /* "q" here used for angle states */
            }
         }
      }
      
      for(Iw=0;Iw<S->Nw;Iw++) {
         W = &S->Whl[Iw];
         W->H = W->RKHm + dt*W->Hdot;
      }

      /* Fourth Call */
      OrderNMultiBodyEOM(S);
      
      /* Digest Fourth Call */
      f = 1.0/6.0;
      G = &S->GN;
      for(i=0;i<3;i++) {
         G->RKdu[  i] += f*G->udot[i];
         G->RKdu[3+i] += f*G->udot[3+i];
         G->RKdx[  i] += f*G->xdot[i];         
      }
      for(i=0;i<4;i++) {
         G->RKdq[i] += f*G->qdot[i];
      }
      
      for(Ig=0;Ig<S->Ng;Ig++) {
         G = &S->G[Ig];
         for(i=0;i<G->RotDOF;i++) {
            G->RKdu[i] += f*G->udot[i];
         }
         for(i=0;i<G->TrnDOF;i++) {
            G->RKdu[G->RotDOF+i] += f*G->udot[G->RotDOF+i];
            G->RKdx[i] += f*G->xdot[i];
         }
         if (G->IsSpherical) {
            for(i=0;i<4;i++) {
               G->RKdq[i] += f*G->qdot[i];
            }
         }
         else {
            for(i=0;i<G->RotDOF;i++) {
               G->RKdq[i] += f*G->qdot[i];
            }
         }
      }

      for(Iw=0;Iw<S->Nw;Iw++) {
         W = &S->Whl[Iw];
         W->RKdH += f*W->Hdot;
      }
      
      /* Update States */
      dt = DTSIM;
      G = &S->GN;
      for(i=0;i<3;i++) {
         D->u[i] = G->RKum[i] + dt*G->RKdu[i];
         D->u[D->Nu-3+i] = G->RKum[3+i] + dt*G->RKdu[3+i];
         D->x[D->Nx-3+i] = G->RKxm[  i] + dt*G->RKdx[i];
      }
      for(i=0;i<4;i++) {
         D->x[i] = G->RKqm[i] + dt*G->RKdq[i];
      } 
      UNITQ(&D->x[0]);
      
      for(Ig=0;Ig<S->Ng;Ig++) {
         G = &S->G[Ig];
         for(i=0;i<G->RotDOF;i++) {
            D->u[G->Rotu0+i] = G->RKum[i] + dt*G->RKdu[i];
         }
         for(i=0;i<G->TrnDOF;i++) {
            D->u[G->Trnu0+i] = G->RKum[G->RotDOF+i] + dt*G->RKdu[G->RotDOF+i];
            D->x[G->Trnx0+i] = G->RKxm[i] + dt*G->RKdx[i];
         }
         if (G->IsSpherical) {
            for(i=0;i<4;i++) {
               D->x[G->Rotx0+i] = G->RKqm[i] + dt*G->RKdq[i]; 
            }
            UNITQ(&D->x[G->Rotx0]);
         }
         else {
            for(i=0;i<G->RotDOF;i++) {
               D->x[G->Rotx0+i] = G->RKqm[i] + dt*G->RKdq[i]; /* "q" here used for angle states */
            }
         }
      }

      MapStateVectorToBodyStates(D->u,D->x,D->uf,D->xf,S); 
      MotionConstraints(S);
      SCMassProps(S);

      for(Iw=0;Iw<S->Nw;Iw++) {
         W = &S->Whl[Iw];
         D->h[Iw] = W->RKHm + dt*W->RKdH;
         W->H = D->h[Iw];
         W->w = W->H/W->J;
         W->ang += W->w*DTSIM;
         W->ang = fmod(W->ang,TwoPi);
      }      

      FindTotalAngMom(S);
}
/**********************************************************************/
/* Utility function for Encke's method.  Computes f(q).               */
/* See Battin, p. 449                                                 */
double EnckeFQ(double r[3], double delta[3])
{
      double q,q1;

      q = ( delta[0]*(delta[0] - 2.0*r[0])
           +delta[1]*(delta[1] - 2.0*r[1])
           +delta[2]*(delta[2] - 2.0*r[2]))/
           (r[0]*r[0]+r[1]*r[1]+r[2]*r[2]);

      q1 = 1.0+q;

      return(q*(3.0+q*(3.0+q))/(1.0+sqrt(q1*q1*q1)));
}
/**********************************************************************/
/*  Orbit dynamics using Encke's method                               */
/*  See Battin, p. 449                                                */
/*   u[0-2] is Rrel(1-3)                                              */
/*   u[3-5] is Vrel(1-3)                                              */

void EnckeEOM(double u[6], double udot[6],
              double R[3], double muR3, double a[3])
{
      double r[3],fq;

      udot[0] = u[3];
      udot[1] = u[4];
      udot[2] = u[5];

      r[0] = R[0] + u[0];
      r[1] = R[1] + u[1];
      r[2] = R[2] + u[2];

      fq = EnckeFQ(r,u);

      udot[3] = a[0] - muR3*(u[0]+fq*r[0]);
      udot[4] = a[1] - muR3*(u[1]+fq*r[1]);
      udot[5] = a[2] - muR3*(u[2]+fq*r[2]);
}
/**********************************************************************/
/* Integration of orbital equations of motion                         */
/* by 4th order Runge-Kutta                                           */
void EnckeRK4(struct SCType *S)
{
      double accel[3],R[3], magr,muR3;
      double u[6],uu[6],m1[6],m2[6],m3[6],m4[6];
      long j;
      struct OrbitType *O;

      O = &Orb[S->RefOrb];

      accel[0] = S->FrcN[0]/S->mass;
      accel[1] = S->FrcN[1]/S->mass;
      accel[2] = S->FrcN[2]/S->mass;
      R[0] = O->PosN[0];
      R[1] = O->PosN[1];
      R[2] = O->PosN[2];

      magr = sqrt(R[0]*R[0]+R[1]*R[1]+R[2]*R[2]);
      muR3 = O->mu/(magr*magr*magr);

      u[0] = S->PosR[0];
      u[1] = S->PosR[1];
      u[2] = S->PosR[2];
      u[3] = S->VelR[0];
      u[4] = S->VelR[1];
      u[5] = S->VelR[2];

/* .. 4th Order Runga-Kutta Integration */
      EnckeEOM( u, m1, R, muR3, accel);
      for(j=0;j<6;j++) uu[j] = u[j] + 0.5*DTSIM*m1[j];
      EnckeEOM(uu, m2, R, muR3, accel);
      for(j=0;j<6;j++) uu[j] = u[j] + 0.5*DTSIM*m2[j];
      EnckeEOM(uu, m3, R, muR3, accel);
      for(j=0;j<6;j++) uu[j] = u[j] + DTSIM*m3[j];
      EnckeEOM(uu, m4, R, muR3, accel);
      for(j=0;j<6;j++) u[j]+=DTSIM/6.0*(m1[j]+2.0*(m2[j]+m3[j])+m4[j]);

      S->PosR[0] = u[0];
      S->PosR[1] = u[1];
      S->PosR[2] = u[2];
      S->VelR[0] = u[3];
      S->VelR[1] = u[4];
      S->VelR[2] = u[5];

      RelRV2EHRV(O->SMA,O->MeanMotion,O->CLN,
         S->PosR,S->VelR,S->PosEH,S->VelEH);
}
/**********************************************************************/
void CowellEOM(double u[6], double udot[6], double mu,
   double mass, double Frc[3])
{
      double r,muR3;

      r = MAGV(u);
      muR3 = mu/(r*r*r);

      udot[0] = u[3];
      udot[1] = u[4];
      udot[2] = u[5];
      udot[3] = Frc[0]/mass - muR3*u[0];
      udot[4] = Frc[1]/mass - muR3*u[1];
      udot[5] = Frc[2]/mass - muR3*u[2];
}
/**********************************************************************/
/* Integration of orbital equations of motion using Cowell's method   */
/* by 4th order Runge-Kutta                                           */
void CowellRK4(struct SCType *S)
{
      double u[6],uu[6],m1[6],m2[6],m3[6],m4[6];
      long j;
      struct OrbitType *O;

      O = &Orb[S->RefOrb];

      u[0] = S->PosN[0];
      u[1] = S->PosN[1];
      u[2] = S->PosN[2];
      u[3] = S->VelN[0];
      u[4] = S->VelN[1];
      u[5] = S->VelN[2];

/* .. 4th Order Runga-Kutta Integration */
      CowellEOM( u, m1, O->mu, S->mass, S->FrcN);
      for(j=0;j<6;j++) uu[j] = u[j] + 0.5*DTSIM*m1[j];
      CowellEOM(uu, m2, O->mu, S->mass, S->FrcN);
      for(j=0;j<6;j++) uu[j] = u[j] + 0.5*DTSIM*m2[j];
      CowellEOM(uu, m3, O->mu, S->mass, S->FrcN);
      for(j=0;j<6;j++) uu[j] = u[j] + DTSIM*m3[j];
      CowellEOM(uu, m4, O->mu, S->mass, S->FrcN);
      for(j=0;j<6;j++) u[j]+=DTSIM/6.0*(m1[j]+2.0*(m2[j]+m3[j])+m4[j]);

      S->PosN[0] = u[0];
      S->PosN[1] = u[1];
      S->PosN[2] = u[2];
      S->VelN[0] = u[3];
      S->VelN[1] = u[4];
      S->VelN[2] = u[5];

      if (O->Regime == ORB_CENTRAL) {
         for(j=0;j<3;j++) {
            S->PosR[j] = S->PosN[j] - O->PosN[j];
            S->VelR[j] = S->VelN[j] - O->VelN[j];
         }
         RelRV2EHRV(O->SMA,O->MeanMotion,O->CLN,
            S->PosR,S->VelR,S->PosEH,S->VelEH);
      }
      else {
         for(j=0;j<3;j++) {
            S->PosEH[j] = 0.0;
            S->VelEH[j] = 0.0;
         }
      }
}
/**********************************************************************/
void PolyhedronCowellEOM(double u[6], double udot[6],
   double mass, double GravAcc[3], double Frc[3])
{
      udot[0] = u[3];
      udot[1] = u[4];
      udot[2] = u[5];
      udot[3] = GravAcc[0]+Frc[0]/mass;
      udot[4] = GravAcc[1]+Frc[1]/mass;
      udot[5] = GravAcc[2]+Frc[2]/mass;
}
/**********************************************************************/
/* Integration of orbital equations of motion using Cowell's method   */
/* by 4th order Runge-Kutta                                           */
void PolyhedronCowellRK4(struct SCType *S)
{
      double u[6],uu[6],m1[6],m2[6],m3[6],m4[6];
      long j;
      struct OrbitType *O;
      struct WorldType *W;
      struct GeomType *G;
      double GravAccN[3];

      O = &Orb[S->RefOrb];
      W = &World[O->World];
      G = &Geom[W->GeomTag];

      u[0] = S->PosN[0];
      u[1] = S->PosN[1];
      u[2] = S->PosN[2];
      u[3] = S->VelN[0];
      u[4] = S->VelN[1];
      u[5] = S->VelN[2];

/* .. 4th Order Runga-Kutta Integration */
      PolyhedronGravAcc(G,W->Density,u,W->CWN,GravAccN);
      PolyhedronCowellEOM( u, m1, S->mass, GravAccN, S->FrcN);
      for(j=0;j<6;j++) uu[j] = u[j] + 0.5*DTSIM*m1[j];
      
      PolyhedronGravAcc(G,W->Density,uu,W->CWN,GravAccN);
      PolyhedronCowellEOM(uu, m2, S->mass, GravAccN, S->FrcN);
      for(j=0;j<6;j++) uu[j] = u[j] + 0.5*DTSIM*m2[j];
    
      PolyhedronGravAcc(G,W->Density,uu,W->CWN,GravAccN);
      PolyhedronCowellEOM(uu, m3, S->mass, GravAccN, S->FrcN);
      for(j=0;j<6;j++) uu[j] = u[j] + DTSIM*m3[j];
      
      PolyhedronGravAcc(G,W->Density,uu,W->CWN,GravAccN);
      PolyhedronCowellEOM(uu, m4, S->mass, GravAccN, S->FrcN);
      for(j=0;j<6;j++) u[j]+=DTSIM/6.0*(m1[j]+2.0*(m2[j]+m3[j])+m4[j]);

      S->PosN[0] = u[0];
      S->PosN[1] = u[1];
      S->PosN[2] = u[2];
      S->VelN[0] = u[3];
      S->VelN[1] = u[4];
      S->VelN[2] = u[5];

}
/**********************************************************************/
/*  Orbit dynamics using Encke's method                               */
/*  Perturbation from Three-Body trajectory                           */
/*  See Battin, p. 449                                                */
/*   u[0-2] is Rrel(1-3)                                              */
/*   u[3-5] is Vrel(1-3)                                              */

void ThreeBodyEnckeEOM(double u[6], double udot[6],
      double R1[3], double muR13, double R2[3], double muR23,
      double a[3])
{
      double r1[3],r2[3],fq1,fq2;

      udot[0] = u[3];
      udot[1] = u[4];
      udot[2] = u[5];

      r1[0] = R1[0] + u[0];
      r1[1] = R1[1] + u[1];
      r1[2] = R1[2] + u[2];

      r2[0] = R2[0] + u[0];
      r2[1] = R2[1] + u[1];
      r2[2] = R2[2] + u[2];

      fq1 = EnckeFQ(r1,u);
      fq2 = EnckeFQ(r2,u);

      udot[3] = a[0] - muR13*(u[0]+fq1*r1[0]) - muR23*(u[0]+fq2*r2[0]);
      udot[4] = a[1] - muR13*(u[1]+fq1*r1[1]) - muR23*(u[1]+fq2*r2[1]);
      udot[5] = a[2] - muR13*(u[2]+fq1*r1[2]) - muR23*(u[2]+fq2*r2[2]);
}
/**********************************************************************/
/* Integration of equations of perturbed motion from three-body orbit */
/* by 4th order Runge-Kutta                                           */
void ThreeBodyEnckeRK4(struct SCType *S)
{
      double accel[3],R1[3],MagR1,muR13,R2[3],MagR2,muR23;
      double u[6],uu[6],m1[6],m2[6],m3[6],m4[6];
      long j;
      struct OrbitType *O,*E;

      O = &Orb[S->RefOrb];
      E = &World[O->Body2].eph;

      accel[0] = S->FrcN[0]/S->mass;
      accel[1] = S->FrcN[1]/S->mass;
      accel[2] = S->FrcN[2]/S->mass;
      R1[0] = O->PosN[0];
      R1[1] = O->PosN[1];
      R1[2] = O->PosN[2];
      R2[0] = R1[0] - E->PosN[0];
      R2[1] = R1[1] - E->PosN[1];
      R2[2] = R1[2] - E->PosN[2];

      MagR1 = sqrt(R1[0]*R1[0]+R1[1]*R1[1]+R1[2]*R1[2]);
      muR13 = O->mu1/(MagR1*MagR1*MagR1);
      MagR2 = sqrt(R2[0]*R2[0]+R2[1]*R2[1]+R2[2]*R2[2]);
      muR23 = O->mu2/(MagR2*MagR2*MagR2);

      u[0] = S->PosR[0];
      u[1] = S->PosR[1];
      u[2] = S->PosR[2];
      u[3] = S->VelR[0];
      u[4] = S->VelR[1];
      u[5] = S->VelR[2];

/* .. 4th Order Runga-Kutta Integration */
      ThreeBodyEnckeEOM( u, m1, R1, muR13, R2, muR23, accel);
      for(j=0;j<6;j++) uu[j] = u[j] + 0.5*DTSIM*m1[j];
      ThreeBodyEnckeEOM(uu, m2, R1, muR13, R2, muR23, accel);
      for(j=0;j<6;j++) uu[j] = u[j] + 0.5*DTSIM*m2[j];
      ThreeBodyEnckeEOM(uu, m3, R1, muR13, R2, muR23, accel);
      for(j=0;j<6;j++) uu[j] = u[j] + DTSIM*m3[j];
      ThreeBodyEnckeEOM(uu, m4, R1, muR13, R2, muR23, accel);
      for(j=0;j<6;j++) u[j]+=DTSIM/6.0*(m1[j]+2.0*(m2[j]+m3[j])+m4[j]);

      S->PosR[0] = u[0];
      S->PosR[1] = u[1];
      S->PosR[2] = u[2];
      S->VelR[0] = u[3];
      S->VelR[1] = u[4];
      S->VelR[2] = u[5];

      RelRV2EHRV(MagR1,sqrt(muR13),O->CLN,
         S->PosR,S->VelR,S->PosEH,S->VelEH);
}
/************************************************************/
/*  Euler-Hill linearized EOM for near-circular orbits.     */

void EulHillEOM(double u[6], double udot[6],
                     double n, double a[3])
{
   udot[0] = u[3];
   udot[1] = u[4];
   udot[2] = u[5];
   udot[3] = a[0] + 2.0*n*u[5];
   udot[4] = a[1] - n*n*u[1];
   udot[5] = a[2] - 2.0*n*u[3] + 3.0*n*n*u[2];

}
/**********************************************************************/
/* Integration of orbital equations of motion                         */
/* by 4th order Runge-Kutta                                           */
/* State u[0:2] = r, u[3:5] = v                                       */
void EulHillRK4(struct SCType *S)
{
      double accelN[3],accel[3],R;
      double u[6],uu[6],m1[6],m2[6],m3[6],m4[6];
      long j;
      struct OrbitType *O;

      O = &Orb[S->RefOrb];

      accelN[0] = S->FrcN[0]/S->mass;
      accelN[1] = S->FrcN[1]/S->mass;
      accelN[2] = S->FrcN[2]/S->mass;
      for(j=0;j<3;j++) {
         u[j] = S->PosEH[j];
         u[3+j] = S->VelEH[j];
      }
      MxV(O->CLN,accelN,accel);

      R = MAGV(O->PosN);

/* .. 4th Order Runga-Kutta Integration */
      EulHillEOM( u, m1, O->MeanMotion, accel);
      for(j=0;j<6;j++) uu[j] = u[j] + 0.5*DTSIM*m1[j];
      EulHillEOM(uu, m2, O->MeanMotion, accel);
      for(j=0;j<6;j++) uu[j] = u[j] + 0.5*DTSIM*m2[j];
      EulHillEOM(uu, m3, O->MeanMotion, accel);
      for(j=0;j<6;j++) uu[j] = u[j] + DTSIM*m3[j];
      EulHillEOM(uu, m4, O->MeanMotion, accel);
      for(j=0;j<6;j++) u[j]+=DTSIM/6.0*(m1[j]+2.0*(m2[j]+m3[j])+m4[j]);

      for(j=0;j<3;j++) {
         S->PosEH[j] = u[j];
         S->VelEH[j] = u[3+j];
      }

      EHRV2RelRV(R,O->MeanMotion,O->CLN,S->PosEH,S->VelEH,
         S->PosR,S->VelR);
}
/**********************************************************************/
void ThreeBodyOrbitEOM(double mu1, double mu2, double p[3],
                       double u[6], double udot[6])
{

      double r2[3],r13,r23,p3,c1,c2,c3;

      r2[0] = u[0] - p[0];
      r2[1] = u[1] - p[1];
      r2[2] = u[2] - p[2];

      r13 = sqrt(u[0]*u[0]+u[1]*u[1]+u[2]*u[2]);
      r13 = r13*r13*r13;
      r23 = sqrt(r2[0]*r2[0]+r2[1]*r2[1]+r2[2]*r2[2]);
      r23 = r23*r23*r23;
      p3  = sqrt(p[0]*p[0]+p[1]*p[1]+p[2]*p[2]);
      p3  = p3*p3*p3;

      c1 = -mu1/r13;
      c2 = -mu2/r23;
      c3 =  mu2/p3;

      udot[0] = u[3];
      udot[1] = u[4];
      udot[2] = u[5];
      udot[3] = c1*u[0] + c2*r2[0] + c3*p[0];
      udot[4] = c1*u[1] + c2*r2[1] + c3*p[1];
      udot[5] = c1*u[2] + c2*r2[2] + c3*p[2];
}
/************************************************************/
/*  Propagates motion of Reference Orbit under              */
/*  gravitational attraction of two large bodies.           */
void ThreeBodyOrbitRK4(struct OrbitType *O)
{
      double u[6],uu[6],m1[6],m2[6],m3[6],m4[6];
      long j;

      u[0] = O->PosN[0];
      u[1] = O->PosN[1];
      u[2] = O->PosN[2];
      u[3] = O->VelN[0];
      u[4] = O->VelN[1];
      u[5] = O->VelN[2];

/* .. 4th Order Runga-Kutta Integration */
      ThreeBodyOrbitEOM(O->mu1, O->mu2, World[O->Body2].eph.PosN, u, m1);
      for(j=0;j<6;j++) uu[j] = u[j] + 0.5*DTSIM*m1[j];
      ThreeBodyOrbitEOM(O->mu1, O->mu2, World[O->Body2].eph.PosN, uu, m2);
      for(j=0;j<6;j++) uu[j] = u[j] + 0.5*DTSIM*m2[j];
      ThreeBodyOrbitEOM(O->mu1, O->mu2, World[O->Body2].eph.PosN, uu, m3);
      for(j=0;j<6;j++) uu[j] = u[j] + DTSIM*m3[j];
      ThreeBodyOrbitEOM(O->mu1, O->mu2, World[O->Body2].eph.PosN, uu, m4);
      for(j=0;j<6;j++) u[j]+=DTSIM/6.0*(m1[j]+2.0*(m2[j]+m3[j])+m4[j]);

      O->PosN[0] = u[0];
      O->PosN[1] = u[1];
      O->PosN[2] = u[2];
      O->VelN[0] = u[3];
      O->VelN[1] = u[4];
      O->VelN[2] = u[5];

}
/**********************************************************************/
void FixedOrbitPosition(struct SCType *S)
{
      struct OrbitType *O;
      struct FormationType *Fr;

      O = &Orb[S->RefOrb];
      Fr = &Frm[S->RefOrb];
      if (Fr->FixedInFrame == 'L') {
         MxV(O->CLN,S->PosEH,S->PosR);
      }
      else {
         MTxV(O->CLN,S->PosR,S->PosEH);
      }
}
/**********************************************************************/
/*   Divide acting forces into two components:                        */
/*   The external component perturbs the orbit and the internal       */
/*   (differential) component affects only attitude motion (for       */
/*   multi-body S/C).                                                 */
/*   Thus, S->Frc = External component                                */
/*   and   S->B[j].Frc = Internal component                           */
void PartitionForces(struct SCType *S)
{
      long i,Ib;
      double FextN[3] = {0.0,0.0,0.0};
      double FextB[3];
      long Nb;

      Nb = S->Nb;
      for(Ib=0;Ib<Nb;Ib++) {
         for(i=0;i<3;i++) FextN[i] += S->B[Ib].FrcN[i];
      }

      /* Non-grav acceleration of SC.cm, for accelerometer */
      for(i=0;i<3;i++) S->asn[i] = FextN[i]/S->mass;

      S->FrcN[0] += FextN[0];
      S->FrcN[1] += FextN[1];
      S->FrcN[2] += FextN[2];
      for(Ib=0;Ib<Nb;Ib++) {
         MxV(S->B[Ib].CN,FextN,FextB);
         for(i=0;i<3;i++) {
            S->B[Ib].FrcN[i] -= FextN[i]*S->B[Ib].mass/S->mass;
            S->B[Ib].FrcB[i] -= FextB[i]*S->B[Ib].mass/S->mass;
         }
      }
}
/**********************************************************************/
void Dynamics(struct SCType *S)
{
      struct OrbitType *O;

      O = &Orb[S->RefOrb];


      if (S->Nb > 1) {
         switch(S->DynMethod) {
            case DYN_GAUSS_ELIM :
               KaneNBodyRK4(S);
               break;
            case DYN_ORDER_N :
               OrderNMultiBodyRK4(S);
               break;
            default :
               printf("Unknown Dynamics Solution option.  Bailing out.\n");
               exit(1);
            
         }
      }
      else OneBodyRK4(S);

      switch(O->Regime) {
         case ORB_ZERO :
         case ORB_FLIGHT :
            if (O->PolyhedronGravityEnabled) {
               PolyhedronCowellRK4(S);
            }
            else CowellRK4(S);
            break;
         case ORB_CENTRAL :
            switch(S->OrbDOF) {
               case ORBDOF_FIXED :
                  FixedOrbitPosition(S);
                  break;
               case ORBDOF_EULER_HILL :
                  EulHillRK4(S);
                  break;
               case ORBDOF_COWELL :
                  CowellRK4(S);
                  break;
               default :
                  EnckeRK4(S);
            }
            break;
         case ORB_THREE_BODY :
            switch(S->OrbDOF) {
               case ORBDOF_FIXED :
                  FixedOrbitPosition(S);
                  break;
               case ORBDOF_EULER_HILL :
                  EulHillRK4(S);
                  break;
               case ORBDOF_COWELL :
                  CowellRK4(S);
                  break;
               default :
                  ThreeBodyEnckeRK4(S);
            }
            break;
         default :
            printf("Unknown Orbit Regime in Dynamics.  Bailing out.\n");
            exit(1);
      }

}

/* #ifdef __cplusplus
** }
** #endif
*/

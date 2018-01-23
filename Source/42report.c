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

/*********************************************************************/
double FindTotalProjectedArea(struct SCType *S,double VecN[3])
{
      struct BodyType *B;
      struct GeomType *G;
      struct PolyType *P;
      double ProjArea = 0.0;
      double VecB[3],VoN;
      long Ib,Ipoly;

      for(Ib=0;Ib<S->Nb;Ib++) {
         B = &S->B[Ib];

         /* Transform Direction Vector from N to B */
         MxV(B->CN,VecN,VecB);

         G = &Geom[B->GeomTag];
         for(Ipoly=0;Ipoly<G->Npoly;Ipoly++) {
            P = &G->Poly[Ipoly];
            VoN = VoV(VecB,P->Norm);
            if (VoN > 0.0) {
               ProjArea += VoN*P->Area;
            }
         }
      }
      return(ProjArea);
}
/*********************************************************************/
double FindTotalUnshadedProjectedArea(struct SCType *S,double VecN[3])
{
      struct BodyType *B;
      struct GeomType *G;
      struct PolyType *P;
      double ProjArea = 0.0;
      double VecB[3],VoN;
      long Ib,Ipoly;

      FindUnshadedAreas(S,VecN);

      for(Ib=0;Ib<S->Nb;Ib++) {
         B = &S->B[Ib];

         /* Transform Direction Vector from N to B */
         MxV(B->CN,VecN,VecB);

         G = &Geom[B->GeomTag];
         for(Ipoly=0;Ipoly<G->Npoly;Ipoly++) {
            P = &G->Poly[Ipoly];
            VoN = VoV(VecB,P->Norm);
            if (VoN > 0.0) {
               ProjArea += VoN*P->UnshadedArea;
            }
         }
      }
      return(ProjArea);
}
/*********************************************************************/
void CmgReport(void)
{
      struct SCType *S;
      static FILE *Hvnfile;
      static FILE *Hvbfile;
      static FILE *CmgAngfile;
      static FILE *Tcmgfile;
      static long First = 1;
      double Tcmg[3];
      long i;

      if (First) {
         First = 0;
         Hvnfile = FileOpen(InOutPath,"Hvn.42","w");
         Hvbfile = FileOpen(InOutPath,"Hvb.42","w");
         CmgAngfile = FileOpen(InOutPath,"ang.42","w");
         Tcmgfile = FileOpen(InOutPath,"Tcmg.42","w");
      }

      if (OutFlag) {
         S = &SC[0];
         fprintf(Hvnfile,"%lf %lf %lf\n",S->Hvn[0],S->Hvn[1],S->Hvn[2]);
         fprintf(Hvbfile,"%lf %lf %lf\n",S->Hvb[0],S->Hvb[1],S->Hvb[2]);
         fprintf(CmgAngfile,"%lf %lf %lf %lf\n",
            S->CMG[0].ang[0]*R2D,S->CMG[1].ang[0]*R2D,
            S->CMG[2].ang[0]*R2D,S->CMG[3].ang[0]*R2D);
         Tcmg[0] = 0.0;
         Tcmg[1] = 0.0;
         Tcmg[2] = 0.0;
         for(i=0;i<S->Ncmg;i++) {
            Tcmg[0] += S->CMG[i].Trq[0];
            Tcmg[1] += S->CMG[i].Trq[1];
            Tcmg[2] += S->CMG[i].Trq[2];
         }
         fprintf(Tcmgfile,"%lf %lf %lf\n",Tcmg[0],Tcmg[1],Tcmg[2]);
      }
}
/*********************************************************************/
void PotatoReport(void)
{
      static FILE *Bodywnfile,*Bodyqnfile;
      static FILE *Bodyvnfile,*Bodypnfile;
      struct SCType *S;
      struct BodyType *B;
      long Ib;
      static long First = 1;

      if (First) {
         First = 0;
         Bodywnfile = FileOpen(InOutPath,"Bodywn.42","w");
         Bodyqnfile = FileOpen(InOutPath,"Bodyqn.42","w");
         Bodyvnfile = FileOpen(InOutPath,"Bodyvn.42","w");
         Bodypnfile = FileOpen(InOutPath,"Bodypn.42","w");
      }

      if (OutFlag) {
         S = &SC[0];
         for(Ib=0;Ib<S->Nb;Ib++) {
            B = &S->B[Ib];
            fprintf(Bodywnfile,"%lf %lf %lf ",
               B->wn[0],B->wn[1],B->wn[2]);
            fprintf(Bodyqnfile,"%lf %lf %lf %lf ",
               B->qn[0],B->qn[1],B->qn[2],B->qn[3]);
            fprintf(Bodyvnfile,"%lf %lf %lf ",
               B->vn[0],B->vn[1],B->vn[2]);
            fprintf(Bodypnfile,"%lf %lf %lf ",
               B->pn[0],B->pn[1],B->pn[2]);
         }
         fprintf(Bodywnfile,"\n");
         fprintf(Bodyqnfile,"\n");
         fprintf(Bodyvnfile,"\n");
         fprintf(Bodypnfile,"\n");
      }
}
/*********************************************************************/
void Report(void)
{
      static FILE *timefile,*AbsTimeFile;
      static FILE **xfile, **ufile, **xffile, **uffile;
      static FILE **ConstraintFile;
      static FILE *PosNfile,*VelNfile,*qbnfile,*wbnfile;
      static FILE *PosWfile,*VelWfile;
      static FILE *PosRfile,*VelRfile;
      static FILE *Hvnfile,*KEfile;
      static FILE *RPYfile;
      static FILE *ProjAreaFile;
      static char First = TRUE;
      long Isc,i;
      struct DynType *D;
      double CBL[3][3],Roll,Pitch,Yaw;
      double PosW[3],VelW[3],PosR[3],VelR[3];
      char s[40];
      double ZAxis[3] = {0.0,0.0,1.0};

      if (First) {
         First = FALSE;
         timefile = FileOpen(InOutPath,"time.42","w");
         AbsTimeFile = FileOpen(InOutPath,"AbsTime.42","w");

         ufile = (FILE **) calloc(Nsc,sizeof(FILE *));
         xfile = (FILE **) calloc(Nsc,sizeof(FILE *));
         uffile = (FILE **) calloc(Nsc,sizeof(FILE *));
         xffile = (FILE **) calloc(Nsc,sizeof(FILE *));
         ConstraintFile = (FILE **) calloc(Nsc,sizeof(FILE *));
         for(Isc=0;Isc<Nsc;Isc++) {
            if (SC[Isc].Exists) {
               sprintf(s,"u%02li.42",Isc);
               ufile[Isc] = FileOpen(InOutPath,s,"w");
               sprintf(s,"x%02li.42",Isc);
               xfile[Isc] = FileOpen(InOutPath,s,"w");
               if (SC[Isc].FlexActive) {
                  sprintf(s,"uf%02li.42",Isc);
                  uffile[Isc] = FileOpen(InOutPath,s,"w");
                  sprintf(s,"xf%02li.42",Isc);
                  xffile[Isc] = FileOpen(InOutPath,s,"w");
               }
               if (SC[Isc].ConstraintsRequested) {
                  sprintf(s,"Constraint%02li.42",Isc);
                  ConstraintFile[Isc] = FileOpen(InOutPath,s,"w");
               }
            }
         }
         PosNfile = FileOpen(InOutPath,"PosN.42","w");
         VelNfile = FileOpen(InOutPath,"VelN.42","w");
         PosWfile = FileOpen(InOutPath,"PosW.42","w");
         VelWfile = FileOpen(InOutPath,"VelW.42","w");
         PosRfile = FileOpen(InOutPath,"PosR.42","w");
         VelRfile = FileOpen(InOutPath,"VelR.42","w");
         qbnfile = FileOpen(InOutPath,"qbn.42","w");
         wbnfile = FileOpen(InOutPath,"wbn.42","w");
         Hvnfile = FileOpen(InOutPath,"Hvn.42","w");
         KEfile = FileOpen(InOutPath,"KE.42","w");
         ProjAreaFile = FileOpen(InOutPath,"ProjArea.42","w");
         RPYfile = FileOpen(InOutPath,"RPY.42","w");
      }

      if (OutFlag) {
         fprintf(timefile,"%lf\n",SimTime);
         fprintf(AbsTimeFile,"%lf\n",AbsTime);
         for(Isc=0;Isc<Nsc;Isc++) {
            if (SC[Isc].Exists) {
               D = &SC[Isc].Dyn;
               for(i=0;i<D->Nu;i++) fprintf(ufile[Isc],"% le ",D->u[i]);
               fprintf(ufile[Isc],"\n");
               for(i=0;i<D->Nx;i++) fprintf(xfile[Isc],"% le ",D->x[i]);
               fprintf(xfile[Isc],"\n");
               if (SC[Isc].FlexActive) {
                  for(i=0;i<D->Nf;i++) fprintf(uffile[Isc],"% le ",D->uf[i]);
                  fprintf(uffile[Isc],"\n");
                  for(i=0;i<D->Nf;i++) fprintf(xffile[Isc],"% le ",D->xf[i]);
                  fprintf(xffile[Isc],"\n");
               }
               if (SC[Isc].ConstraintsRequested) {
                  for(i=0;i<D->Nc;i++)
                     fprintf(ConstraintFile[Isc],"% le ",
                             D->GenConstraintFrc[i]);
                  fprintf(ConstraintFile[Isc],"\n");
               }
            }
         }
         if (SC[0].Exists) {
            fprintf(PosNfile,"%le %le %le\n",
               SC[0].PosN[0],SC[0].PosN[1],SC[0].PosN[2]);
            fprintf(VelNfile,"%le %le %le\n",
               SC[0].VelN[0],SC[0].VelN[1],SC[0].VelN[2]);
            MxV(World[EARTH].CWN,SC[0].PosN,PosW);
            MxV(World[EARTH].CWN,SC[0].VelN,VelW);
            fprintf(PosWfile,"%18.12le %18.12le %18.12le ",
               PosW[0],PosW[1],PosW[2]);
            fprintf(VelWfile,"%18.12le %18.12le %18.12le ",
               VelW[0],VelW[1],VelW[2]);
            MxV(Rgn[Orb[SC[0].RefOrb].Region].CN,SC[0].PosR,PosR);
            MxV(Rgn[Orb[SC[0].RefOrb].Region].CN,SC[0].VelR,VelR);
            fprintf(PosRfile,"%le %le %le\n",
               PosR[0],PosR[1],PosR[2]);
            fprintf(VelRfile,"%le %le %le\n",
               VelR[0],VelR[1],VelR[2]);
            fprintf(qbnfile,"%le %le %le %le\n",
               SC[0].B[0].qn[0],SC[0].B[0].qn[1],SC[0].B[0].qn[2],SC[0].B[0].qn[3]);
            fprintf(wbnfile,"%le %le %le\n",
               SC[0].B[0].wn[0],SC[0].B[0].wn[1],SC[0].B[0].wn[2]);
            fprintf(Hvnfile,"%18.12le %18.12le %18.12le\n",
               SC[0].Hvn[0],SC[0].Hvn[1],SC[0].Hvn[2]);
            fprintf(KEfile,"%18.12le\n",FindTotalKineticEnergy(&SC[0]));
            //fprintf(ProjAreaFile,"%18.12le %18.12le\n",
            //   FindTotalProjectedArea(&SC[0],ZAxis),
            //   FindTotalUnshadedProjectedArea(&SC[0],ZAxis));
            MxMT(SC[0].B[0].CN,SC[0].CLN,CBL);
            C2A(123,CBL,&Roll,&Pitch,&Yaw);
            fprintf(RPYfile,"%lf %lf %lf\n",Roll*R2D,Pitch*R2D,Yaw*R2D);
         }

      }

      if (!strcmp(InOutPath,"./Potato/")) PotatoReport();
      if (!strcmp(InOutPath,"./CMG/")) CmgReport();

      if (CleanUpFlag) {
         fclose(timefile);
      }

}

/* #ifdef __cplusplus
** }
** #endif
*/

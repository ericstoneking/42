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
void MagReport(void)
{
      static FILE *magfile;
      static long First = 1;
      
      if (First) {
         First = 0;
         magfile = FileOpen(InOutPath,"MagBVB.42","wt");
      }
      
      fprintf(magfile,"%le %le %le %le %le %le %le %le %le \n",
         SC[0].bvb[0],SC[0].bvb[1],SC[0].bvb[2],
         SC[0].MAG[0].Field,SC[0].MAG[1].Field,SC[0].MAG[2].Field,
         SC[0].AC.bvb[0],SC[0].AC.bvb[1],SC[0].AC.bvb[2]);
      
}
/*********************************************************************/
void GyroReport(void)
{
      static FILE *gyrofile;
      static long First = 1;
      
      if (First) {
         First = 0;
         gyrofile = FileOpen(InOutPath,"Gyro.42","wt");
      }
      
      fprintf(gyrofile,"%le %le %le %le %le %le %le %le %le %le %le %le %le %le %le %le %le %le \n",
         SC[0].B[0].wn[0],SC[0].B[0].wn[1],SC[0].B[0].wn[2],
         SC[0].Gyro[0].TrueRate,SC[0].Gyro[1].TrueRate,SC[0].Gyro[2].TrueRate,
         SC[0].Gyro[0].Bias,SC[0].Gyro[1].Bias,SC[0].Gyro[2].Bias,
         SC[0].Gyro[0].Angle,SC[0].Gyro[1].Angle,SC[0].Gyro[2].Angle,
         SC[0].Gyro[0].MeasRate,SC[0].Gyro[1].MeasRate,SC[0].Gyro[2].MeasRate,
         SC[0].AC.wbn[0],SC[0].AC.wbn[1],SC[0].AC.wbn[2]);
      
}
/*********************************************************************/
void OrbPropReport(void)
{
      static FILE *FixedFile;
      static FILE *EnckeFile;
      static FILE *CowellFile;
      static FILE *EulHillFile;
      static long First = 1;
      
      if (First) {
         First = 0;
         FixedFile = FileOpen(InOutPath,"PosVelNfixed.42","w");
         EnckeFile = FileOpen(InOutPath,"PosVelNencke.42","w");
         CowellFile = FileOpen(InOutPath,"PosVelNcowell.42","w");
         EulHillFile = FileOpen(InOutPath,"PosVelNeulhill.42","w");
      }
      
      if (OutFlag) {
         fprintf(FixedFile,"%24.18le %24.18le %24.18le %24.18le %24.18le %24.18le\n",
            SC[0].PosN[0],SC[0].PosN[1],SC[0].PosN[2],
            SC[0].VelN[0],SC[0].VelN[1],SC[0].VelN[2]);
         fprintf(EnckeFile,"%24.18le %24.18le %24.18le %24.18le %24.18le %24.18le\n",
            SC[1].PosN[0],SC[1].PosN[1],SC[1].PosN[2],
            SC[1].VelN[0],SC[1].VelN[1],SC[1].VelN[2]);
         fprintf(CowellFile,"%24.18le %24.18le %24.18le %24.18le %24.18le %24.18le\n",
            SC[2].PosN[0],SC[2].PosN[1],SC[2].PosN[2],
            SC[2].VelN[0],SC[2].VelN[1],SC[2].VelN[2]);
         fprintf(EulHillFile,"%24.18le %24.18le %24.18le %24.18le %24.18le %24.18le\n",
            SC[3].PosN[0],SC[3].PosN[1],SC[3].PosN[2],
            SC[3].VelN[0],SC[3].VelN[1],SC[3].VelN[2]);
      }
}
/*********************************************************************/
void GmatReport(void)
{
      static FILE *outfile;
      static long First = 1;
      long i;
      
      if (First) {
         First = 0;
         outfile = FileOpen(InOutPath,"PosN9sc.42","w");
      }
      
      if (OutFlag) {
         for(i=0;i<9;i++) {
            fprintf(outfile,"%24.18le %24.18le %24.18le ",
            SC[i].PosN[0],SC[i].PosN[1],SC[i].PosN[2]);
         }
         fprintf(outfile,"\n");
      }
}
/*********************************************************************/
void Report(void)
{
      static FILE *timefile,*DynTimeFile,*UtcDateFile;
      static FILE **xfile, **ufile, **xffile, **uffile;
      static FILE **ConstraintFile;
      static FILE *PosNfile,*VelNfile,*qbnfile,*wbnfile;
      static FILE *PosWfile,*VelWfile;
      static FILE *PosRfile,*VelRfile;
      static FILE *Hvnfile,*KEfile;
      static FILE *Hvbfile;
      static FILE *svnfile,*svbfile;
      static FILE *RPYfile;
      static FILE *Hwhlfile;
      static FILE *MTBfile;
      static FILE *Thrfile;
      static FILE *AlbedoFile;
      static FILE *IllumFile;
      //static FILE *ProjAreaFile;
      static FILE *AccFile;
      //static FILE *Kepfile;
      //static FILE *EHfile;
      static char First = TRUE;
      long Isc,i;
      struct DynType *D;
      double CBR[3][3],CRN[3][3],Roll,Pitch,Yaw;
      struct WorldType *W;
      double WorldAngVel[3],wxR[3],VelN[3];
      double PosW[3],VelW[3],PosR[3],VelR[3];
      double CRL[3][3] = {{1.0,0.0,0.0},{0.0,1.0,0.0},{0.0,0.0,1.0}};
      //double SMA,ecc,inc,RAAN,ArgP,anom,tp,SLR,alpha,rmin,MeanMotion,Period;
      char s[40];
      //double ZAxis[3] = {0.0,0.0,1.0};

      if (First) {
         First = FALSE;
         timefile = FileOpen(InOutPath,"time.42","w");
         DynTimeFile = FileOpen(InOutPath,"DynTime.42","w");
         UtcDateFile = FileOpen(InOutPath,"UTC.42","w");

         ufile = (FILE **) calloc(Nsc,sizeof(FILE *));
         xfile = (FILE **) calloc(Nsc,sizeof(FILE *));
         uffile = (FILE **) calloc(Nsc,sizeof(FILE *));
         xffile = (FILE **) calloc(Nsc,sizeof(FILE *));
         ConstraintFile = (FILE **) calloc(Nsc,sizeof(FILE *));
         for(Isc=0;Isc<Nsc;Isc++) {
            if (SC[Isc].Exists) {
               sprintf(s,"u%02ld.42",Isc);
               ufile[Isc] = FileOpen(InOutPath,s,"w");
               sprintf(s,"x%02ld.42",Isc);
               xfile[Isc] = FileOpen(InOutPath,s,"w");
               if (SC[Isc].FlexActive) {
                  sprintf(s,"uf%02ld.42",Isc);
                  uffile[Isc] = FileOpen(InOutPath,s,"w");
                  sprintf(s,"xf%02ld.42",Isc);
                  xffile[Isc] = FileOpen(InOutPath,s,"w");
               }
               if (SC[Isc].ConstraintsRequested) {
                  sprintf(s,"Constraint%02ld.42",Isc);
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
         Hvbfile = FileOpen(InOutPath,"Hvb.42","w");
         svnfile = FileOpen(InOutPath,"svn.42","w");
         svbfile = FileOpen(InOutPath,"svb.42","w");
         KEfile = FileOpen(InOutPath,"KE.42","w");
         //ProjAreaFile = FileOpen(InOutPath,"ProjArea.42","w");
         RPYfile = FileOpen(InOutPath,"RPY.42","w");
         Hwhlfile = FileOpen(InOutPath,"Hwhl.42","w");
         //Kepfile = FileOpen(InOutPath,"KepElem.42","w");
         //EHfile = FileOpen(InOutPath,"EH.42","w");

         if (SC[0].Nmtb > 0) {
            MTBfile = FileOpen(InOutPath,"MTB.42","w");
         }
         
         if (SC[0].Nthr > 0) {
            Thrfile = FileOpen(InOutPath,"Thr.42","w");
         }
         
         if (SC[0].Nacc > 0) {
            AccFile = FileOpen(InOutPath,"Acc.42","w");
         }
         
         if (SC[0].Ncss > 0) {
            AlbedoFile = FileOpen(InOutPath,"Albedo.42","w");
            IllumFile = FileOpen(InOutPath,"Illum.42","w");
         }
      }

      if (OutFlag) {
         fprintf(timefile,"%lf\n",SimTime);
         fprintf(DynTimeFile,"%lf\n",DynTime);
         fprintf(UtcDateFile," %ld:%02ld:%02ld:%02ld:%02ld:%09.6lf\n",
            UTC.Year,UTC.Month,UTC.Day,UTC.Hour,UTC.Minute,UTC.Second);
         for(Isc=0;Isc<Nsc;Isc++) {
            if (SC[Isc].Exists) {
               D = &SC[Isc].Dyn;
               for(i=0;i<D->Nu;i++) fprintf(ufile[Isc],"%18.12le ",D->u[i]);
               fprintf(ufile[Isc],"\n");
               for(i=0;i<D->Nx;i++) fprintf(xfile[Isc],"%18.12le ",D->x[i]);
               fprintf(xfile[Isc],"\n");
               if (SC[Isc].FlexActive) {
                  for(i=0;i<D->Nf;i++) fprintf(uffile[Isc],"%18.12le ",D->uf[i]);
                  fprintf(uffile[Isc],"\n");
                  for(i=0;i<D->Nf;i++) fprintf(xffile[Isc],"%18.12le ",D->xf[i]);
                  fprintf(xffile[Isc],"\n");
               }
               if (SC[Isc].ConstraintsRequested) {
                  for(i=0;i<D->Nc;i++)
                     fprintf(ConstraintFile[Isc],"%18.12le ",
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
            W = &World[Orb[SC[0].RefOrb].World];
            WorldAngVel[0] = 0.0;
            WorldAngVel[1] = 0.0;
            WorldAngVel[2] = W->w;
            VxV(WorldAngVel,SC[0].PosN,wxR);
            for(i=0;i<3;i++) VelN[i] = SC[0].VelN[i] - wxR[i];
            MxV(W->CWN,SC[0].PosN,PosW);
            MxV(W->CWN,VelN,VelW);
            fprintf(PosWfile,"%18.12le %18.12le %18.12le\n",
               PosW[0],PosW[1],PosW[2]);
            fprintf(VelWfile,"%18.12le %18.12le %18.12le\n",
               VelW[0],VelW[1],VelW[2]);  
            if (Orb[SC[0].RefOrb].Regime == ORB_FLIGHT) {
               MxV(Rgn[Orb[SC[0].RefOrb].Region].CN,SC[0].PosR,PosR);
               MxV(Rgn[Orb[SC[0].RefOrb].Region].CN,SC[0].VelR,VelR);
               fprintf(PosRfile,"%le %le %le\n",
                  PosR[0],PosR[1],PosR[2]);
               fprintf(VelRfile,"%le %le %le\n",
                  VelR[0],VelR[1],VelR[2]);
            }
            else {
               fprintf(PosRfile,"%le %le %le\n",
                  SC[0].PosR[0],SC[0].PosR[1],SC[0].PosR[2]);
               fprintf(VelRfile,"%le %le %le\n",
                  SC[0].VelR[0],SC[0].VelR[1],SC[0].VelR[2]);
            }
            fprintf(qbnfile,"%le %le %le %le\n",
               SC[0].B[0].qn[0],SC[0].B[0].qn[1],SC[0].B[0].qn[2],SC[0].B[0].qn[3]);
            fprintf(wbnfile,"%le %le %le\n",
               SC[0].B[0].wn[0],SC[0].B[0].wn[1],SC[0].B[0].wn[2]);
            fprintf(Hvnfile,"%18.12le %18.12le %18.12le\n",
               SC[0].Hvn[0],SC[0].Hvn[1],SC[0].Hvn[2]);
            fprintf(Hvbfile,"%18.12le %18.12le %18.12le\n",
               SC[0].Hvb[0],SC[0].Hvb[1],SC[0].Hvb[2]);
            fprintf(svnfile,"%18.12le %18.12le %18.12le\n",
               SC[0].svn[0],SC[0].svn[1],SC[0].svn[2]);
            fprintf(svbfile,"%18.12le %18.12le %18.12le\n",
               SC[0].svb[0],SC[0].svb[1],SC[0].svb[2]);
            fprintf(KEfile,"%18.12le\n",FindTotalKineticEnergy(&SC[0]));
            //fprintf(ProjAreaFile,"%18.12le %18.12le\n",
            //   FindTotalProjectedArea(&SC[0],ZAxis),
            //   FindTotalUnshadedProjectedArea(&SC[0],ZAxis));
            MxM(CRL,SC[0].CLN,CRN);
            MxMT(SC[0].B[0].CN,CRN,CBR);
            C2A(123,CBR,&Roll,&Pitch,&Yaw);
            fprintf(RPYfile,"%le %le %le\n",Roll*R2D,Pitch*R2D,Yaw*R2D);
            if (SC[0].Nw > 0) {
               for(i=0;i<SC[0].Nw;i++) {
                  fprintf(Hwhlfile,"%lf ",SC[0].Whl[i].H);
               }
               fprintf(Hwhlfile,"\n");
            }
            if (SC[0].Nmtb > 0) {
               for(i=0;i<SC[0].Nmtb;i++) fprintf(MTBfile,"%lf ",SC[0].MTB[i].M);
               fprintf(MTBfile,"\n");
            }
            if (SC[0].Nthr > 0) {
               for(i=0;i<SC[0].Nthr;i++) fprintf(Thrfile,"%lf ",SC[0].Thr[i].F);
               fprintf(Thrfile,"\n");
            }
            if (SC[0].Nacc > 0) {
               for(i=0;i<SC[0].Nacc;i++) 
                  fprintf(AccFile,"%le %le ",SC[0].Accel[i].TrueAcc,SC[0].Accel[i].MeasAcc);
               fprintf(AccFile,"\n");
            }
            if (SC[0].Ncss > 0) {
               for(i=0;i<SC[0].Ncss;i++) {
                  fprintf(IllumFile,"%le ",SC[0].CSS[i].Illum);
                  fprintf(AlbedoFile,"%le ",SC[0].CSS[i].Albedo);
               }
               fprintf(IllumFile,"\n");
               fprintf(AlbedoFile,"\n");
            }
            
            //RV2Eph(DynTime,Orb[0].mu,SC[0].PosN,SC[0].VelN,
            //   &SMA,&ecc,&inc,&RAAN,&ArgP,&anom,&tp,&SLR,&alpha,&rmin,
            //   &MeanMotion,&Period);
            //fprintf(Kepfile,"%le %le %le %le %le %le\n",
            //   SMA*1.0E-3,ecc,inc*R2D,RAAN*R2D,ArgP*R2D,anom*R2D);
            
            //OrbPropReport();
            //GmatReport();
            
            //MagReport();
            //GyroReport();
            
            //fprintf(EHfile,"%le %le %le %le %le %le\n",
            //   SC[0].PosEH[0],SC[0].PosEH[1],SC[0].PosEH[2],
            //   SC[0].VelEH[0],SC[0].VelEH[1],SC[0].VelEH[2]);
         }

      }
      
      /* An example how to call specialized reporting based on sim case */
      /* if (!strcmp(InOutPath,"./Potato/")) PotatoReport(); */
      

      if (CleanUpFlag) {
         fclose(timefile);
      }

}

/* #ifdef __cplusplus
** }
** #endif
*/

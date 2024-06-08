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
long Aperture(double FldPnt[3], double FldDir[3], double ctr[3], double axis[3],
   double ApRad, double IntPnt[3], double OutDir[3])
{
      double eps = 1.0E-12;
      double dl = 1.0E6;
      double K = 1.0;
      
      double PoA,rvec[3],r,rhat[3],a;
      double cq[3],l,cp[3];
      long k = 0;
      long i;
      
      long InAperture = TRUE;
      
      for(i=0;i<3;i++) cq[i] = FldPnt[i] - ctr[i];
      l = VoV(cq,axis);
      while (fabs(dl) > eps && k < 10) {
         for(i=0;i<3;i++) IntPnt[i] = FldPnt[i] + l*FldDir[i];
         for(i=0;i<3;i++) cp[i] = IntPnt[i] - ctr[i];
         PoA = VoV(cp,axis);
         for(i=0;i<3;i++) rvec[i] = cp[i] - PoA*axis[i];
         r = CopyUnitV(rvec,rhat);
         
         /* Surface Equation */
         a = 0.0;
         
         dl = -K*(a - PoA);
         l += dl;
         k++;
      }
      for(i=0;i<3;i++) IntPnt[i] = FldPnt[i] + l*FldDir[i];
      if (r > ApRad) InAperture = FALSE;
      
      for(i=0;i<3;i++) OutDir[i] = FldDir[i];

      return(InAperture);
}
/**********************************************************************/
long PlanarMirror(double FldPnt[3], double FldDir[3], double ctr[3], double axis[3],
   double ApRad, double IntPnt[3], double ReflDir[3])
{
      double eps = 1.0E-12;
      double dl = 1.0E6;
      double K = 1.0;
      
      double PoA,rvec[3],r,rhat[3],a;
      double cq[3],l,cp[3];
      double nhat[3],LoN;
      long k = 0;
      long i;
      
      long InAperture = TRUE;
      
      for(i=0;i<3;i++) cq[i] = FldPnt[i] - ctr[i];
      l = VoV(cq,axis);
      while (fabs(dl) > eps && k < 10) {
         for(i=0;i<3;i++) IntPnt[i] = FldPnt[i] + l*FldDir[i];
         for(i=0;i<3;i++) cp[i] = IntPnt[i] - ctr[i];
         PoA = VoV(cp,axis);
         for(i=0;i<3;i++) rvec[i] = cp[i] - PoA*axis[i];
         r = CopyUnitV(rvec,rhat);
         
         /* Mirror Equation */
         a = 0.0;
         
         dl = -K*(a - PoA);
         l += dl;
         k++;
      }
      for(i=0;i<3;i++) IntPnt[i] = FldPnt[i] + l*FldDir[i];
      if (r > ApRad) InAperture = FALSE;
      
      /* Mirror Normal */
      for(i=0;i<3;i++) nhat[i] = axis[i];
      LoN = VoV(FldDir,nhat);
      for(i=0;i<3;i++) ReflDir[i] = FldDir[i] - 2.0*LoN*nhat[i];

      return(InAperture);
}
/**********************************************************************/
long ConicMirror(double FldPnt[3], double FldDir[3], double ctr[3], double axis[3],
   double foclen, double ConicConst, double ConicSign, double ApRad, 
   double IntPnt[3], double ReflDir[3])
{
      double eps = 1.0E-12;
      double dl = 1.0E6;
      double G = 1.0;
      
      double R,D;
      
      double PoA,rvec[3],r,rhat[3],a;
      double cq[3],l,cp[3];
      double nhat[3],LoN;
      double Den;
      long k = 0;
      long i;
      
      long InAperture = TRUE;
      
      R = 2.0*foclen;
      
      for(i=0;i<3;i++) cq[i] = FldPnt[i] - ctr[i];
      l = VoV(cq,axis);
      for(i=0;i<3;i++) IntPnt[i] = FldPnt[i] + l*FldDir[i];
      while (fabs(dl) > eps && k < 10) {
         for(i=0;i<3;i++) cp[i] = IntPnt[i] - ctr[i];
         PoA = VoV(cp,axis);
         for(i=0;i<3;i++) rvec[i] = cp[i] - PoA*axis[i];
         r = CopyUnitV(rvec,rhat);
         
         /* Mirror Equation */
         D = sqrt(R*R-(1.0+ConicConst)*r*r);
         a = ConicSign*r*r/(R+D);
         
         dl = -G*(a - PoA);
         l += dl;
         for(i=0;i<3;i++) IntPnt[i] = FldPnt[i] + l*FldDir[i];
         k++;
      }
      if (r > ApRad) InAperture = FALSE;
      
      /* Mirror Normal */
      Den = sqrt(R*R-ConicConst*r*r);
      for(i=0;i<3;i++) nhat[i] = (-ConicSign*rvec[i]+D*axis[i])/Den;
      LoN = VoV(FldDir,nhat);
      for(i=0;i<3;i++) ReflDir[i] = FldDir[i] - 2.0*LoN*nhat[i];

      return(InAperture);
}
/**********************************************************************/
long ThinLens(double FldPnt[3], double FldDir[3], double ctr[3], double axis[3],
   double foclen, double ApRad, double IntPnt[3], double RefrDir[3])
{
      long RayOnAxis = FALSE;
      long InAperture = TRUE;
      long i;
      double dp[3],binorm[3];
      double rhat[3],r0,a0,r1;
      double TanTheta0,TanTheta1;
      double theta1,CosTheta1,SinTheta1;
      double eps = 1.0E-6;
      
      for(i=0;i<3;i++) dp[i] = FldPnt[i] - ctr[i];
      VxV(dp,axis,binorm);
      if (MAGV(binorm) < eps) {
         VxV(FldDir,axis,binorm);
         if (MAGV(binorm) < eps) {
            RayOnAxis = TRUE;
         }
      }
      
      if (RayOnAxis) {
         for(i=0;i<3;i++) {
            IntPnt[i] = ctr[i];
            RefrDir[i] = -axis[i];
         }
      }
      else {
         UNITV(binorm);
         VxV(axis,binorm,rhat);
         
         a0 = VoV(dp,axis);
         r0 = VoV(dp,rhat);
         
         TanTheta0 = VoV(FldDir,rhat)/VoV(FldDir,axis);
         
         r1 = r0 - a0*TanTheta0;
         if (fabs(r1) > ApRad) InAperture = FALSE;
         TanTheta1 = (r0+(a0-foclen)/foclen*r1);
         theta1 = atan(TanTheta1);
         CosTheta1 = cos(theta1);
         SinTheta1 = sin(theta1);
         
         for(i=0;i<3;i++) {
            IntPnt[i] = ctr[i] + r1*rhat[i];
            RefrDir[i] = -CosTheta1*axis[i] - SinTheta1*rhat[i];
         }
      }

      return(InAperture);
}
/**********************************************************************/
long Detector(double FldPnt[3], double FldDir[3], double ctr[3], double axis[3],
   double ApRad, double IntPnt[3])
{
      double eps = 1.0E-12;
      double dl = 1.0E6;
      double K = 1.0;
      
      double PoA,rvec[3],r,rhat[3],a;
      double cq[3],l,cp[3];
      long k = 0;
      long i;
      
      long InAperture = TRUE;
      
      for(i=0;i<3;i++) cq[i] = FldPnt[i] - ctr[i];
      l = VoV(cq,axis);
      while (fabs(dl) > eps && k < 10) {
         for(i=0;i<3;i++) IntPnt[i] = FldPnt[i] + l*FldDir[i];
         for(i=0;i<3;i++) cp[i] = IntPnt[i] - ctr[i];
         PoA = VoV(cp,axis);
         for(i=0;i<3;i++) rvec[i] = cp[i] - PoA*axis[i];
         r = CopyUnitV(rvec,rhat);
         
         /* Surface Equation */
         a = 0.0;
         
         dl = -K*(a - PoA);
         l += dl;
         k++;
      }
      for(i=0;i<3;i++) IntPnt[i] = FldPnt[i] + l*FldDir[i];
      if (r > ApRad) InAperture = FALSE;
      
      return(InAperture);
}
/**********************************************************************/
long OpticalFieldPoint(double StarVecB[3], struct OpticsType *O, 
   double FldPntB[3], double FldDirB[3])
{
      struct NodeType *N;
      double InPntB[3],InDirB[3],OutPntB[3],OutDirB[3];
      long InAp,i;
      
      N = &SC[O->SC].B[O->Body].Node[O->Node];

      /* Send ray upstream from focal point of ApFocus */
      for(i=0;i<3;i++) {
         InPntB[i] = N->NomPosB[i] - O->FocLen*O->Axis[i];
         InDirB[i] = StarVecB[i];
      }
      InAp = Aperture(InPntB,InDirB,N->NomPosB,O->Axis,
                O->ApRad,OutPntB,OutDirB);
      for(i=0;i<3;i++) {
         FldPntB[i] =  OutPntB[i];
         FldDirB[i] = -O->Axis[i];
      }
      return(InAp);
}
/**********************************************************************/
/* Returns number of elements successfully passed [0:Nopt]            */
long OpticalTrain(long FldSC, long FldBody, double FldPntB[3], double FldDirB[3], 
   long Nopt, struct OpticsType *Opt,
   long *OutSC, long *OutBody, double OutPntB[3], double OutDirB[3])
{
      struct SCType *S;
      struct BodyType *B;
      struct NodeType *N;
      struct OpticsType *O;
      double InPntN[3],InDirN[3],InPntB[3],InDirB[3];
      double axis[3];
      long Io,i;
      long InAp;

      S = &SC[FldSC];
      B = &S->B[FldBody];
      MTxV(B->CN,FldPntB,InPntN);
      MTxV(B->CN,FldDirB,InDirN);
      for(i=0;i<3;i++) InPntN[i] += B->pn[i] + S->PosR[i];

/* .. For each element in optical train */
      for(Io=0;Io<Nopt;Io++) {
         O = &Opt[Io];
         S = &SC[O->SC];
         B = &S->B[O->Body];
         N = &B->Node[O->Node];
         QxV(N->qb,O->Axis,axis);
         /* Transform incoming ray */
         for(i=0;i<3;i++) InPntN[i] -= B->pn[i] + S->PosR[i];
         MxV(B->CN,InPntN,InPntB);
         MxV(B->CN,InDirN,InDirB);
         printf("Opt[%ld]:  InPnt: %lf %lf %lf   InDirB: %lf %lf %lf\n",
            Io,InPntB[0],InPntB[1],InPntB[2],InDirB[0],InDirB[1],InDirB[2]);
         /* Find Reflection */
         switch(O->Type) {
            case OPT_APERTURE:
               InAp = Aperture(InPntB,InDirB,N->PosB,axis,
                  O->ApRad,OutPntB,OutDirB);
               break;
            case OPT_CONIC:
               InAp = ConicMirror(InPntB,InDirB,N->PosB,axis,
                         O->FocLen,O->ConicConst,O->ConicSign,O->ApRad,
                         OutPntB,OutDirB);
               break;
            case OPT_THINLENS:
               InAp = ThinLens(InPntB,InDirB,N->PosB,axis,
                         O->FocLen,O->ApRad,OutPntB,OutDirB);
               break;
            case OPT_DETECTOR:
               InAp = Detector(InPntB,InDirB,N->PosB,axis,
                         O->ApRad,OutPntB);
               for(i=0;i<3;i++) OutDirB[i] = 0.0;
               break;
         }
         if (!InAp) return(Io);
         else {
            *OutSC = O->SC;
            *OutBody = O->Body;
            /* Copy and transform outgoing ray for next element */
            MTxV(B->CN,OutPntB,InPntN);
            MTxV(B->CN,OutDirB,InDirN);
            for(i=0;i<3;i++) InPntN[i] += B->pn[i] + S->PosR[i];
         }
      }
      
      return(Nopt); 
}

/* #ifdef __cplusplus
** }
** #endif
*/


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
/* Generic force/torque source, with harmonics or spread-spectrum    */
double ShakerFrcTrq(struct ShakerType *Sh, struct SCType *S)
{
      long It;
      double Signal;

      Sh->Output = 0.0;
      for(It=0;It<Sh->Ntone;It++) {
         Sh->Output += Sh->ToneAmp[It]*cos(Sh->ToneFreq[It]*SimTime+Sh->TonePhase[It]);
      }
      
      if (Sh->RandomActive) {
         Signal = GaussianRandom(Sh->RandomProc);
         Signal = SecondOrderLowpassFilter(Sh->Lowpass,Signal);
         if (Sh->LowBandLimit > 0.0) 
            Signal = SecondOrderHighpassFilter(Sh->Highpass,Signal);
         Sh->Output += Sh->RandStd*Signal;
      }
      return(Sh->Output);      
}
/*********************************************************************/
/* Ref "Wheel Jitter.ipynb" */
/* But note that here Frc and Trq are expressed in Go frame, so      */
/* wheel spin phase is a little different than in the reference      */
void WheelJitter(struct JointType *G, struct SCType *S)
{
      struct WhlHarmType *H;
      struct WhlType *W;
      double OM,OM2,nOM,nOM2;
      double Fx,Fy,Tx,Ty;
      double CosTerm,SinTerm,rho;
      double mR,nsigma;
      double JA,ndelta;
      long Ih;
      
      /* Fundamental Harmonic */
      G->Frc[0] = -G->TrnSpringCoef[0]*G->Pos[0]
                  -G->TrnDampCoef[0]*G->PosRate[0];
      G->Frc[1] = -G->TrnSpringCoef[1]*G->Pos[1]
                  -G->TrnDampCoef[1]*G->PosRate[1];
      G->Trq[0] = -G->RotSpringCoef[0]*G->Ang[0]
                  -G->RotDampCoef[0]*G->AngRate[0];
      G->Trq[1] = -G->RotSpringCoef[1]*G->Ang[1]
                  -G->RotDampCoef[1]*G->AngRate[1];

      /* Other harmonics */
      W = G->W;
      
      OM = G->AngRate[2];
      OM2 = OM*OM;
      
      Fx = 0.0;
      Fy = 0.0;
      Tx = 0.0;
      Ty = 0.0;

      for(Ih=0;Ih<W->NumHarm;Ih++) {
         H = &W->Harm[Ih];
         nOM = H->n*OM;
         nOM2 = nOM*nOM;
         
         /* Radial Force */
         CosTerm = W->LatFreq*W->LatFreq-nOM2;
         SinTerm = 2.0*W->LatDamp*W->LatFreq*nOM;
         mR = H->Ks*nOM2/sqrt(CosTerm*CosTerm+SinTerm*SinTerm);
         nsigma = atan2(-SinTerm,-CosTerm);
         rho = H->n*(G->Ang[2]+W->ImbPhase+H->phase);
         Fx += nOM2*(-H->Ks*cos(rho)+mR*cos(rho-nsigma));
         Fy += nOM2*(-H->Ks*sin(rho)+mR*sin(rho-nsigma));
         
         /* Radial Torque */
         CosTerm = W->RockFreq*W->RockFreq+(2.0*W->gamma-H->n)*H->n*OM2;
         SinTerm = 2.0*W->RockDamp*W->RockFreq*nOM;
         JA = H->Kd*nOM2/sqrt(CosTerm*CosTerm+SinTerm*SinTerm);
         ndelta = atan2(-SinTerm,-CosTerm);
         rho = H->n*(G->Ang[2]+H->phase);
         Tx += nOM2*(-H->Kd*cos(rho)+JA*cos(rho-ndelta));
         Ty += nOM2*(-H->Kd*sin(rho)+JA*sin(rho-ndelta));
      }
      G->Frc[0] += Fx;
      G->Frc[1] += Fy;
      G->Trq[0] += Tx;
      G->Trq[1] += Ty;

}

/* #ifdef __cplusplus
** }
** #endif
*/

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
/* Generic force/torque source, with tones or spread-spectrum    */
void ShakerJitter(struct ShakerType *Sh, struct SCType *S)
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
}
/*********************************************************************/
/* Ref "Wheel Jitter.ipynb" */
void WheelJitter(struct WhlType *W, struct SCType *S)
{
      struct WhlHarmType *H;
      double OM,OM2,nOM,nOM2;
      double Fx,Fy,Tx,Ty;
      double CosTerm,SinTerm,rho;
      double mR,nsigma;
      double JA,ndelta;
      long Ih,i;
            
      OM = W->w;
      OM2 = OM*OM;
      
      Fx = 0.0;
      Fy = 0.0;
      Tx = 0.0;
      Ty = 0.0;

      for(Ih=0;Ih<W->NumHarm;Ih++) {
         H = &W->Harm[Ih];
         nOM = H->n*OM;
         
         if (nOM < 0.1*TwoPi/DTSIM) {
            nOM2 = nOM*nOM;
         
            /* Radial Force */
            CosTerm = W->LatFreq*W->LatFreq-nOM2;
            SinTerm = 2.0*W->LatDamp*W->LatFreq*nOM;
            mR = H->Ks*nOM2/sqrt(CosTerm*CosTerm+SinTerm*SinTerm);
            nsigma = atan2(-SinTerm,-CosTerm);
            rho = H->n*(W->Ang+W->ImbPhase+H->phase);
            Fx += nOM2*(-H->Ks*cos(rho)+mR*cos(rho-nsigma));
            Fy += nOM2*(-H->Ks*sin(rho)+mR*sin(rho-nsigma));
         
            /* Radial Torque */
            CosTerm = W->RockFreq*W->RockFreq+(2.0*W->gamma-H->n)*H->n*OM2;
            SinTerm = 2.0*W->RockDamp*W->RockFreq*nOM;
            JA = H->Kd*nOM2/sqrt(CosTerm*CosTerm+SinTerm*SinTerm);
            ndelta = atan2(-SinTerm,-CosTerm);
            rho = H->n*(W->Ang+H->phase);
            Tx += nOM2*(-H->Kd*cos(rho)+JA*cos(rho-ndelta));
            Ty += nOM2*(-H->Kd*sin(rho)+JA*sin(rho-ndelta));
         }
      }
      for(i=0;i<3;i++) {
         W->JitFrc[i] = Fx*W->Uaxis[i]+Fy*W->Vaxis[i];
         W->JitTrq[i] = Tx*W->Uaxis[i]+Ty*W->Vaxis[i];
      }

}

/* #ifdef __cplusplus
** }
** #endif
*/

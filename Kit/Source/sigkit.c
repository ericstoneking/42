/*    This file is distributed with 42,                               */
/*    the (mostly harmless) spacecraft dynamics simulation            */
/*    created by Eric Stoneking of NASA Goddard Space Flight Center   */

/*    Copyright 2010 United States Government                         */
/*    as represented by the Administrator                             */
/*    of the National Aeronautics and Space Administration.           */

/*    No copyright is claimed in the United States                    */
/*    under Title 17, U.S. Code.                                      */

/*    All Other Rights Reserved.                                      */


#include "sigkit.h"

/* #ifdef __cplusplus
** namespace Kit {
** #endif
*/

#define IA 16807
#define IM 2147483647
#define AM (1.0/IM)
#define IQ 127773
#define IR 2836
#define NTAB 32
#define NDIV (1+(IM-1)/NTAB)
#define EPS 1.2E-7
#define RNMX (1.0-EPS)


/**********************************************************************/
struct RandomProcessType *CreateRandomProcess(long Seed)
{
      long j;
      long k;
      long Value;
      struct RandomProcessType *RP;

      RP = (struct RandomProcessType *)
         calloc(1,sizeof(struct RandomProcessType));

      Value = Seed;

      /* Make sure Value is in valid range */
      if (Value <= 0) {
         if (-Value < 1) Value = 1;
         else Value = -Value;
      }

      /* Do eight warmup cycles */
      for(j=0;j<8;j++) {
         /* Multiplicative Congruential Algorithm */
         k = Value/IQ;
         Value = IA*(Value-k*IQ)-IR*k;
         if (Value < 0) Value += IM;
      }
      /* Fill Lookup Table */
      for(j=0;j<NTAB;j++) {
         /* Multiplicative Congruential Algorithm */
         k = Value/IQ;
         Value = IA*(Value-k*IQ)-IR*k;
         if (Value < 0) Value += IM;

         /* Place in Table */
         RP->LookupTable[j] = Value;
      }
      RP->Index = (long) (RP->LookupTable[0]/NDIV);
      RP->PreviousValue = Value;

      return(RP);
}
/**********************************************************************/
void DestroyRandomProcess(struct RandomProcessType *RP)
{
      free(RP);
}
/**********************************************************************/
/* This portable random number generator is taken from Numerical      */
/* Recipes in C, where it is called ran1.  It has been rewritten for  */
/* readability.                                                       */
/* It returns a uniform random deviate between 0.0 and 1.0.           */
double UniformRandom(struct RandomProcessType *RP)
{
      long k;
      long Value;
      long ReadValue;
      double Output;

      Value = RP->PreviousValue;

      /* Make sure Value is in valid range */
      if (Value <= 0) {
         if (-Value < 1) Value = 1;
         else Value = -Value;
      }

      /* Multiplicative Congruential Algorithm */
      k = Value/IQ;
      Value = IA*(Value-k*IQ)-IR*k;
      if (Value < 0) Value += IM;

      /* Read from Table */
      ReadValue = RP->LookupTable[RP->Index];

      /* Remember value for next time */
      RP->PreviousValue = Value;

      /* Replace read-out value with fresh one */
      RP->LookupTable[RP->Index] = Value;
      /* Compute index for next table lookup */
      RP->Index = (long) (ReadValue/NDIV);

      /* Scale output  to (0:1) */
      Output = (double) (ReadValue*AM);
      if (Output > RNMX) Output = RNMX;
      return(Output);

}

#undef IA
#undef IM
#undef AM
#undef IQ
#undef IR
#undef NTAB
#undef NDIV
#undef EPS
#undef RNMX

/**********************************************************************/
/*  This function returns a Gaussian random deviate with zero mean,   */
/*  unit variance, using the "UniformRandom" function above to obtain */
/*  uniform deviates.  Algorithm based on Numerical Recipes Sec. 7-2  */
double GaussianRandom(struct RandomProcessType *RP)
{
      double x,y,r2,a;

      if (RP->HaveSavedValue) {
         RP->HaveSavedValue = 0;
         return(RP->SavedValue);
      }
      else {
         /* Throw (uniform) darts until hit in unit circle */
         do {
            x = 2.0*UniformRandom(RP)-1.0;
            y = 2.0*UniformRandom(RP)-1.0;
            r2 = x*x+y*y;
         } while (r2 >= 1.0 || r2 == 0.0);
         a = sqrt(-2.0*log(r2)/r2);
         /* Use one now, save one for next time */
         RP->HaveSavedValue = 1;
         RP->SavedValue=x*a;
         return(y*a);
      }
}
/**********************************************************************/
/* Returns a uniform deviate in range [-1:1].  No internal states,    */
/* so suitable for procedural textures and other deterministic        */
/* applications.                                                      */
double PRN2D(long x, long y)
{
      int n;
      n = x + 57*y;
      n = (n<<13)^n;
      return((double) (1.0-((n*(n*n*15731+789221)+1376312589)
                      & 0x7fffffff)/1073741824.0));
}
/**********************************************************************/
/* Returns a uniform deviate in range [-1:1].  No internal states,    */
/* so suitable for procedural textures and other deterministic        */
/* applications.                                                      */
double PRN3D(long x, long y, long z)
{
      long n;
      n = x + 57*(y+57*z);
      n = (n<<13)^n;
      return((double) (1.0-((n*(n*n*15731+789221)+1376312589)
                      & 0x7fffffff)/1073741824.0));
}
/**********************************************************************/
/* 0 if x < a, 1.0 otherwise                                          */
double Step(double a, double x)
{
      return((double) (x >= a));
}
/**********************************************************************/
/*  a if x < a, b if x > b, else x                                    */
double Clamp(double a, double b, double x)
{
      return( x < a ? a : (x > b ? b : x ));
}
/**********************************************************************/
/*  0 if x < a, 1 if x > b, linear in between                         */
double RampStep(double a, double b, double x)
{
      if (x <= a) return(0.0);
      if (x >= b) return(1.0);
      return((x-a)/(b-a));
}
/**********************************************************************/
/*  0.0 if x < a, 1.0 if x > b, cubic in between                      */
double CubicStep(double a, double b, double x)
{
      if (x <= a) return(0.0);
      if (x >= b) return(1.0);
      x = (x-a)/(b-a);
      return(x*x*(3.0-2.0*x));
}
/**********************************************************************/
/*        (B[0] + B[1]*z^-1 + B[2]*z^-2...)                           */
/* Y/X = -----------------------------------                          */
/*        (A[0] + A[1]*z^-1 + A[2]*z^-2...)                           */
struct FilterType *CreateGeneralFilter(long Ns, double *A, double *B,
   double dxmax, double ymin)
{
      struct FilterType *F;
      long i;

      F = (struct FilterType *) calloc(1,sizeof(struct FilterType));

      F->A = (double *) calloc(Ns,sizeof(double));
      F->B = (double *) calloc(Ns,sizeof(double));
      F->x = (double *) calloc(Ns,sizeof(double));
      F->y = (double *) calloc(Ns,sizeof(double));

      F->Ns = Ns;
      for(i=0;i<Ns;i++) {
         F->A[i] = A[i];
         F->B[i] = B[i];
      }
      F->dxmax = dxmax;
      F->ymin = ymin;

      return(F);
}
/**********************************************************************/
/*  Discrete Filter by Pole-Zero Mapping                              */
/*  See Franklin and Powell, sec 3.3                                  */
struct FilterType *CreateFirstOrderLowpassFilter(double w, double T,
   double dxmax, double ymin)
{
      struct FilterType *F;

      if (6.28/w < 10.0*T) {
         printf("Error in CreateSecondeOrderLowpassFilter.\n");
         printf("Filter break frequency is too high to be sampled at given sample rate.\n");
         printf("Recommend sample interval be reduced to %lf sec or less.\n",0.628/w);
         exit(1);
      }

      F = (struct FilterType *) calloc(1,sizeof(struct FilterType));

      F->Ns = 2;
      F->A = (double *) calloc(F->Ns,sizeof(double));
      F->B = (double *) calloc(F->Ns,sizeof(double));
      F->x = (double *) calloc(F->Ns,sizeof(double));
      F->y = (double *) calloc(F->Ns,sizeof(double));
      F->A[0] = exp(-w*T);
      F->B[0] = 0.5*(1.0-F->A[0]);
      F->dxmax = dxmax;
      F->ymin = ymin;

      return(F);
}
/**********************************************************************/
/*  Discrete Filter by Pole-Zero Mapping                              */
/*  See Franklin and Powell, sec 3.3                                  */
struct FilterType *CreateFirstOrderHighpassFilter(double w, double T,
   double dxmax, double ymin)
{
      struct FilterType *F;

      if (6.28/w < 10.0*T) {
         printf("Error in CreateSecondeOrderLowpassFilter.\n");
         printf("Filter break frequency is too high to be sampled at given sample rate.\n");
         printf("Recommend sample interval be reduced to %lf sec or less.\n",0.628/w);
         exit(1);
      }

      F = (struct FilterType *) calloc(1,sizeof(struct FilterType));

      F->Ns = 2;
      F->A = (double *) calloc(F->Ns,sizeof(double));
      F->B = (double *) calloc(F->Ns,sizeof(double));
      F->x = (double *) calloc(F->Ns,sizeof(double));
      F->y = (double *) calloc(F->Ns,sizeof(double));
      F->A[0] = exp(-w*T);
      F->B[0] = 0.5*(1.0+F->A[0]);
      F->dxmax = dxmax;
      F->ymin = ymin;

      return(F);
}
/**********************************************************************/
/*  Discrete Filter by Pole-Zero Mapping                              */
/*  See Franklin and Powell, sec 3.3                                  */
struct FilterType *CreateSecondOrderLowpassFilter(double w, double z,
   double T, double dxmax, double ymin)
{
      struct FilterType *F;
      double a;

      if (6.28/w < 10.0*T) {
         printf("Error in CreateSecondeOrderLowpassFilter.\n");
         printf("Filter break frequency is too high to be sampled at given sample rate.\n");
         printf("Recommend sample interval be reduced to %lf sec or less.\n",0.628/w);
         exit(1);
      }

      F = (struct FilterType *) calloc(1,sizeof(struct FilterType));

      F->Ns = 3;
      F->A = (double *) calloc(F->Ns,sizeof(double));
      F->B = (double *) calloc(F->Ns,sizeof(double));
      F->x = (double *) calloc(F->Ns,sizeof(double));
      F->y = (double *) calloc(F->Ns,sizeof(double));

      a = exp(-z*w*T);
      F->A[0] = 2.0*a*cos(w*T);
      F->A[1] = -a*a;
      F->B[0] = 0.25*(1.0-2.0*a*cos(w*T)+a*a);
      F->dxmax = dxmax;
      F->ymin = ymin;

      return(F);
}
/**********************************************************************/
/*  Discrete Filter by Pole-Zero Mapping                              */
/*  See Franklin and Powell, sec 3.3                                  */
struct FilterType *CreateSecondOrderHighpassFilter(double w, double z,
   double T, double dxmax, double ymin)
{
      struct FilterType *F;
      double a;

      if (6.28/w < 10.0*T) {
         printf("Error in CreateSecondeOrderLowpassFilter.\n");
         printf("Filter break frequency is too high to be sampled at given sample rate.\n");
         printf("Recommend sample interval be reduced to %lf sec or less.\n",0.628/w);
         exit(1);
      }

      F = (struct FilterType *) calloc(1,sizeof(struct FilterType));

      F->Ns = 3;
      F->A = (double *) calloc(F->Ns,sizeof(double));
      F->B = (double *) calloc(F->Ns,sizeof(double));
      F->x = (double *) calloc(F->Ns,sizeof(double));
      F->y = (double *) calloc(F->Ns,sizeof(double));

      a = exp(-z*w*T);
      F->A[0] = 2.0*a*cos(w*T);
      F->A[1] = -a*a;
      F->B[0] = 0.25*(1.0+a*cos(w*T));
      F->dxmax = dxmax;
      F->ymin = ymin;

      return(F);
}
/**********************************************************************/
void DestroyFilter(struct FilterType *F)
{
      free(F->A);
      free(F->B);
      free(F->x);
      free(F->y);
      free(F);
}
/**********************************************************************/
/*  y(k) = (B[0]*x(k)+B[1]*x(k-1)+...+B[Ns-1]*x(k-Ns+1)               */
/*                  - A[1]*y(k-1)-...-A[Ns-1]*y(k-Ns+1))/A[0]         */
double GeneralFilter(struct FilterType *F, double x)
{
      double Xmax,Xmin;
      double Bx,Ay;
      long k;

      /* Clamp spikes */
      F->x[0] = x;
      Xmax = F->x[1] + F->dxmax;
      if (F->x[0] > Xmax) F->x[0] = Xmax;
      Xmin = F->x[1] - F->dxmax;
      if (F->x[0] < Xmin) F->x[0] = Xmin;

      /* Filter */
      Bx = 0.0;
      Ay = 0.0;
      for(k=0;k<F->Ns;k++) Bx += F->B[k]*F->x[k];
      for(k=1;k<F->Ns;k++) Ay += F->A[k]*F->y[k];
      F->y[0] = (Bx-Ay)/F->A[0];

      /* Trap underflow */
      if (fabs(F->y[0]) < F->ymin) F->y[0] = 0.0;

      for(k=F->Ns-1;k>0;k--) {
         F->x[k] = F->x[k-1];
         F->y[k] = F->y[k-1];
      }

      return(F->y[0]);
}
/**********************************************************************/
/*  Discrete Filter by Pole-Zero Mapping                              */
/*  See Franklin and Powell, sec 3.3                                  */
double FirstOrderLowpassFilter(struct FilterType *F, double x)
{
      double Xmax,Xmin;
      long k;

      /* Clamp spikes */
      F->x[0] = x;
      Xmax = F->x[1] + F->dxmax;
      if (F->x[0] > Xmax) F->x[0] = Xmax;
      Xmin = F->x[1] - F->dxmax;
      if (F->x[0] < Xmin) F->x[0] = Xmin;

      /* Filter */
      F->y[0] = F->A[0]*F->y[1] + F->B[0]*(F->x[0]+F->x[1]);

      /* Trap underflow */
      if (fabs(F->y[0]) < F->ymin) F->y[0] = 0.0;

      for(k=F->Ns-1;k>0;k--) {
         F->x[k] = F->x[k-1];
         F->y[k] = F->y[k-1];
      }

      return(F->y[0]);
}
/**********************************************************************/
/*  Discrete Filter by Pole-Zero Mapping                              */
/*  See Franklin and Powell, sec 3.3                                  */
double FirstOrderHighpassFilter(struct FilterType *F, double x)
{
      double Xmax,Xmin;
      long k;

      /* Clamp spikes */
      F->x[0] = x;
      Xmax = F->x[1] + F->dxmax;
      if (F->x[0] > Xmax) F->x[0] = Xmax;
      Xmin = F->x[1] - F->dxmax;
      if (F->x[0] < Xmin) F->x[0] = Xmin;

      /* Filter */
      F->y[0] = F->A[0]*F->y[1] + F->B[0]*(F->x[0] - F->x[1]);

      /* Trap underflow */
      if (fabs(F->y[0]) < F->ymin) F->y[0] = 0.0;

      for(k=F->Ns-1;k>0;k--) {
         F->x[k] = F->x[k-1];
         F->y[k] = F->y[k-1];
      }

      return(F->y[0]);
}
/**********************************************************************/
/*  Discrete Filter by Pole-Zero Mapping                              */
/*  See Franklin and Powell, sec 3.3                                  */
double SecondOrderLowpassFilter(struct FilterType *F, double x)
{
      double Xmax,Xmin;
      long k;

      /* Clamp spikes */
      F->x[0] = x;
      Xmax = F->x[1] + F->dxmax;
      if (F->x[0] > Xmax) F->x[0] = Xmax;
      Xmin = F->x[1] - F->dxmax;
      if (F->x[0] < Xmin) F->x[0] = Xmin;

      /* Filter */
      F->y[0] = F->A[0]*F->y[1]+F->A[1]*F->y[2]
               +F->B[0]*(F->x[0]+2.0*F->x[1]+F->x[2]);

      /* Trap underflow */
      if (fabs(F->y[0]) < F->ymin) F->y[0] = 0.0;

      for(k=F->Ns-1;k>0;k--) {
         F->x[k] = F->x[k-1];
         F->y[k] = F->y[k-1];
      }

      return(F->y[0]);
}
/**********************************************************************/
/*  Discrete Filter by Pole-Zero Mapping                              */
/*  See Franklin and Powell, sec 3.3                                  */
double SecondOrderHighpassFilter(struct FilterType *F, double x)
{
      double Xmax,Xmin;
      long k;

      /* Clamp spikes */
      F->x[0] = x;
      Xmax = F->x[1] + F->dxmax;
      if (F->x[0] > Xmax) F->x[0] = Xmax;
      Xmin = F->x[1] - F->dxmax;
      if (F->x[0] < Xmin) F->x[0] = Xmin;

      /* Filter */
      F->y[0] = F->A[0]*F->y[1]+F->A[1]*F->y[2]+F->B[0]*(F->x[0]-2.0*F->x[1]+F->x[2]);

      /* Trap underflow */
      if (fabs(F->y[0]) < F->ymin) F->y[0] = 0.0;

      for(k=F->Ns-1;k>0;k--) {
         F->x[k] = F->x[k-1];
         F->y[k] = F->y[k-1];
      }

      return(F->y[0]);
}
/**********************************************************************/
struct DelayType *CreateDelay(double DelayTime, double DT)
{
      struct DelayType *D;
      
      D = (struct DelayType *) calloc(1,sizeof(struct DelayType));
      
      D->N = ((long) (DelayTime/DT+0.5));
      
      D->CircBuffer = (double *) calloc(D->N,sizeof(double));
      
      D->Idx = 0;
      
      return(D);
}
/**********************************************************************/
struct DelayType *ResizeDelay(struct DelayType *OldD, double DelayTime, double DT)
{
      struct DelayType *D;
      long N,i;
      
      N = ((long) (DelayTime/DT+0.5));
      
      if (N == OldD->N) return(OldD);
      
      D = CreateDelay(DelayTime,DT);
      N = (OldD->N > D->N ? D->N : OldD->N);
      for(i=0;i<N;i++) {
         D->CircBuffer[i] = OldD->CircBuffer[i];
      }
      D->Idx = OldD->Idx % D->N;
      
      /* TODO: Free OldD */
      
      return(D);
}
/**********************************************************************/
double Delay(struct DelayType *D, double x)
{
      double y; 
      
      if (D->N == 0) {
         y = x;
      }
      else {      
         y = D->CircBuffer[D->Idx];      
         D->CircBuffer[D->Idx] = x;
         D->Idx = (D->Idx + 1) % D->N;
      }
      
      return(y);
}
/* #ifdef __cplusplus
** }
** #endif
*/

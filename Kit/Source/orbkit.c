/*    This file is distributed with 42,                               */
/*    the (mostly harmless) spacecraft dynamics simulation            */
/*    created by Eric Stoneking of NASA Goddard Space Flight Center   */

/*    Copyright 2010 United States Government                         */
/*    as represented by the Administrator                             */
/*    of the National Aeronautics and Space Administration.           */

/*    No copyright is claimed in the United States                    */
/*    under Title 17, U.S. Code.                                      */

/*    All Other Rights Reserved.                                      */


#include "orbkit.h"

/* #ifdef __cplusplus
** namespace Kit {
** #endif
*/

/**********************************************************************/
struct OrbitType *CloneOrbit(struct OrbitType *OldOrb, long *Norb,
   long Iorb)
{
      struct OrbitType *NewOrb;

      (*Norb)++;
      NewOrb = (struct OrbitType *)
         realloc(OldOrb,(*Norb)*sizeof(struct OrbitType));
      if (NewOrb == NULL) {
         printf("Realloc failed in CloneOrbit\n");
         exit(1);
      }
      memcpy(&NewOrb[(*Norb)-1],
             &NewOrb[Iorb],sizeof(struct OrbitType));
      return(NewOrb);
}
/**********************************************************************/
double MeanAnomToTrueAnom(double MeanAnom, double ecc)
{
      #define EPS (1.0E-12)
      double E,f,fp,dE;
      long i = 0;

      E = MeanAnom;
      do {
         i++;
         f = E - ecc*sin(E) - MeanAnom;
         fp = 1.0 - ecc*cos(E);
         dE = f/fp;
         if (dE >  0.1) dE =  0.1;
         if (dE < -0.1) dE = -0.1;
         E -= dE;
      } while (fabs(f) > EPS && fabs(dE) > EPS && i < 100);
      return(2.0*atan(sqrt((1.0+ecc)/(1.0-ecc))*tan(0.5*E)));
      #undef EPS
}
/**********************************************************************/
double TrueAnomaly(double mu, double p, double e, double t)
{
#define EPS (1.0E-12)
#define PI    (3.141592653589793)
#define TWOPI (6.283185307179586)

      double p3,B,x,f,fp,e1,N,H,M,a;
      double Ne,dx,dH,Anom;
      long i;

      p3 = p*p*p;

      if (e ==1.0) {
         i = 0;
         B = 3.0*sqrt(mu/p3)*t;
         x = 0.0;
         do {
            i++;
            f = x*(x*x+3.0)-2.0*B;
            fp = 3.0*x*x+3.0;
            dx = f/fp;
            x -= dx;
         } while (fabs(f) > EPS && fabs(dx) > EPS && i < 100);
         Anom = 2.0*atan(x);
      }
      else if (e > 1.0) {
         i = 0;
         e1 = e*e-1.0;
         N = sqrt(mu*e1*e1*e1/p3)*t;
         Ne = N/e;
         H = log(Ne+sqrt(Ne*Ne+1.0)); /* H = arcsinh(N/e); */
         do {
            i++;
            f = e*sinh(H) - H - N;
            fp = e*cosh(H) - 1.0;
            dH = f/fp;
            if (dH >  0.1) dH =  0.1;
            if (dH < -0.1) dH = -0.1;
            H -= dH;
         } while (fabs(f) > EPS && fabs(dH) > EPS && i < 100);
         Anom = 2.0*atan(sqrt((e+1.0)/(e-1.0))*tanh(0.5*H));
      }
      else {
         a = p/(1.0-e*e);
         M = sqrt(mu/(a*a*a))*t;
         M = fmod(M+PI, TWOPI) - PI;
         Anom = MeanAnomToTrueAnom(M,e);
      }

      return(Anom);
#undef EPS
#undef PI
#undef TWOPI
}
/**********************************************************************/
/* As a hyperbolic trajectory approaches its asymptotes, it's more    */
/* precise to find the radius rather than the true anomaly as a       */
/* function of time since periapsis passage.                          */
/* Sensitivity crossover happens when velocity is more radial than    */
/* tangential, which happens at about N = e-1 (approximation improves */
/* as e->inf.                                                         */
void FindHyperbolicRadius(double mu, double p, double e, double dt,
                          double *R)
{

      double a,q,sqma,T,Den,alpha,r,sqX,f,rold,fold,dr;

      a = p/(1.0-e*e);
      q = p/(1.0+e);
      sqma = sqrt(-a);
      T = -sqrt(mu)/a*fabs(dt);
      Den = q/sqma+sqma;
      alpha = 1.0/a;

      r=p;
      sqX=sqrt((2.0-p/r)/r-alpha);
      f=r*sqX-sqma*log(((sqX+1.0/sqma)*r+sqma)/Den)-T;
      rold=r;
      fold=f;

      r=1.1*p;
      do {
         sqX=sqrt((2.0-p/r)/r-alpha);
         f=r*sqX-sqma*log(((sqX+1.0/sqma)*r+sqma)/Den)-T;
         dr = -f*(r-rold)/(f-fold);
         rold=r;
         fold=f;
         r+=dr;

      } while (fabs(dr) > 1.0E-3);

      *R = r;
}
/**********************************************************************/
double atanh(double x)
{
   if (fabs(x) < 1.0)
      return 0.5*log((1.0+x)/(1.0-x));
   else
      return 0.0;
}
/**********************************************************************/
double TimeSincePeriapsis(double mu, double p, double e, double th)
{
      double x,a,B,E,H,dt;

      x = tan(0.5*th);

      if (e == 1.0) {
         B = 0.5*x*(x*x+3.0);
         dt = sqrt(p*p*p/mu)/3.0*B;
      }
      else if (e < 1.0) {
         a = p/(1.0-e*e);
         E=2.0*atan(sqrt((1.0-e)/(1.0+e))*x);
         dt=(E-e*sin(E))*sqrt(a*a*a/mu);
      }
      else {
         a = p/(1.0-e*e);
         H=2.0*atanh(sqrt((e-1.0)/(e+1.0))*x);
         dt=(e*sinh(H)-H)*sqrt(-a*a*a/mu);
      }
      return(dt);
}
/**********************************************************************/
/* Find position and velocity, given initial position, velocity, and  */
/* true anomaly difference.  This routine good for all orbits.        */
/* See Battin, p.130                                                  */
void RV02RV(double mu, double xr0[3], double xv0[3], double anom,
            double xr[3], double xv[3])
{
      double sqmu,cth,sth,cth1,s0,R0xV0[3],p,r0,sqp,r,F,Ft,G,Gt;
      long i;

      sqmu=sqrt(mu);
      cth=cos(anom);
      sth=sin(anom);
      cth1=1.0-cth;

      s0 = (xr0[0]*xv0[0]+xr0[1]*xv0[1]+xr0[2]*xv0[2])/sqmu;
      R0xV0[0] = xr0[1]*xv0[2]-xr0[2]*xv0[1];
      R0xV0[1] = xr0[2]*xv0[0]-xr0[0]*xv0[2];
      R0xV0[2] = xr0[0]*xv0[1]-xr0[1]*xv0[0];
      p=(R0xV0[0]*R0xV0[0]+R0xV0[1]*R0xV0[1]+R0xV0[2]*R0xV0[2])/mu;
      r0=sqrt(xr0[0]*xr0[0]+xr0[1]*xr0[1]+xr0[2]*xr0[2]);
      sqp=sqrt(p);

      r=p*r0/(r0+(p-r0)*cth-sqp*s0*sth);

      F=1.0-r/p*cth1;

      G=r*r0*sth/(sqmu*sqp);

      Ft=sqmu/(r0*p)*(s0*cth1-sqp*sth);

      Gt=1.0-r0/p*cth1;

      for(i=0;i<3;i++){
         xr[i] = F *xr0[i] + G *xv0[i];
         xv[i] = Ft*xr0[i] + Gt*xv0[i];
      }

}
/**********************************************************************/
/* Compute position and velocity given orbital elements.  Works for   */
/* circular, elliptical, parabolic and hyperbolic orbits.             */

void Eph2RV(double mu, double p, double e, double i, double RAAN,
            double ArgP, double dt, double r[3], double v[3],
            double *anom)
{
      double R,th,CPN[3][3],cth,sth,pr[3],pv[3],c2;
      double C1,S1,C2,S2,C3,S3;

      th = TrueAnomaly(mu,p,e,dt);
      sth = sin(th);
      cth = cos(th);
      R = p/(1.0+e*cth);

      c2 = sqrt(mu/p);
      pr[0] = R*cth;
      pr[1] = R*sth;
      pr[2] = 0.0;
      pv[0] = -c2*sth;
      pv[1] = c2*(e+cth);
      pv[2] = 0.0;

      C1=cos(RAAN);S1=sin(RAAN);
      C2=cos(i);S2=sin(i);
      C3=cos(ArgP);S3=sin(ArgP);

      CPN[0][0]=-S1*C2*S3+C3*C1;
      CPN[1][0]=-S1*C2*C3-S3*C1;
      CPN[2][0]=S1*S2;
      CPN[0][1]=C1*C2*S3+C3*S1;
      CPN[1][1]=C1*C2*C3-S3*S1;
      CPN[2][1]=-C1*S2;
      CPN[0][2]=S2*S3;
      CPN[1][2]=S2*C3;
      CPN[2][2]=C2;

      r[0] = pr[0]*CPN[0][0]+pr[1]*CPN[1][0];
      r[1] = pr[0]*CPN[0][1]+pr[1]*CPN[1][1];
      r[2] = pr[0]*CPN[0][2]+pr[1]*CPN[1][2];

      v[0] = pv[0]*CPN[0][0]+pv[1]*CPN[1][0];
      v[1] = pv[0]*CPN[0][1]+pv[1]*CPN[1][1];
      v[2] = pv[0]*CPN[0][2]+pv[1]*CPN[1][2];

      *anom = th;

}
/**********************************************************************/
/* Compute orbital elements, given position and velocity.  Works for  */
/* for all eccentricities.                                            */

void  RV2Eph(double time, double mu, double xr[3], double xv[3],
             double *SMA, double *e, double *i, double *RAAN,
             double *ArgP, double *th, double *tp,
             double *SLR, double *alpha, double *rmin,
             double *MeanMotion, double *Period)
{
#define EPS (1.0E-12)
#define PI    (3.141592653589793)
#define TWOPI (6.283185307179586)

      double xh[3],xe[3],rhat[3],r,v,vxh[3],cth,hxe[3];
      double rohxe,h,dt;
      double xn[3],hxn[3],cosw,sinw;

      r=sqrt(xr[0]*xr[0]+xr[1]*xr[1]+xr[2]*xr[2]);
      v=sqrt(xv[0]*xv[0]+xv[1]*xv[1]+xv[2]*xv[2]);

      *alpha = 2.0*(mu/r-0.5*v*v)/mu;
      *SMA = 1.0/(*alpha);
      if (*alpha > 0.0) {
         /* Elliptic orbit */
         *MeanMotion = sqrt(mu*(*alpha)*(*alpha)*(*alpha));
         *Period = TWOPI/(*MeanMotion);
      }
      else {
         /* For hyperbolic orbits, these need special interpretation */
         *MeanMotion = sqrt(-mu*(*alpha)*(*alpha)*(*alpha));
         *Period = TWOPI/(*MeanMotion);
      }

      xh[0] = xr[1]*xv[2]-xr[2]*xv[1];
      xh[1] = xr[2]*xv[0]-xr[0]*xv[2];
      xh[2] = xr[0]*xv[1]-xr[1]*xv[0];
      h=sqrt(xh[0]*xh[0]+xh[1]*xh[1]+xh[2]*xh[2]); /* 3D mag */

      /* Semi-Latus Rectum */
      *SLR = (xh[0]*xh[0]+xh[1]*xh[1]+xh[2]*xh[2])/mu;

      rhat[0] = xr[0]/r;
      rhat[1] = xr[1]/r;
      rhat[2] = xr[2]/r;

      vxh[0] = xv[1]*xh[2]-xv[2]*xh[1];
      vxh[1] = xv[2]*xh[0]-xv[0]*xh[2];
      vxh[2] = xv[0]*xh[1]-xv[1]*xh[0];

      xe[0] = vxh[0]/mu-rhat[0];
      xe[1] = vxh[1]/mu-rhat[1];
      xe[2] = vxh[2]/mu-rhat[2];
      *e=sqrt(xe[0]*xe[0]+xe[1]*xe[1]+xe[2]*xe[2]);

      *rmin = *SLR/(1.0 + *e);

      if (h > EPS) {
         xh[0] /= h;
         xh[1] /= h;
         xh[2] /= h;
      }

      h = sqrt(xh[0]*xh[0]+xh[1]*xh[1]);  /* 2D mag */
      if (*e < EPS) {              /* Circular */
         if (h < EPS) {            /* Equatorial */
            /* Arbitrarily set RAAN, omg = 0 */
            *RAAN = 0.0;
            *i = (xh[2]>0.0 ? 0.0 : PI );
            *ArgP = 0.0;
            xe[0] = 1.0;
            xe[1] = 0.0;
            xe[2] = 0.0;
         }
         else {                     /* Inclined */
            /* Arbitrarily set omg = 0 */
            *RAAN = atan2(xh[0],-xh[1]);
            *i   =  acos(xh[2]);
            *ArgP = 0.0;
            xe[0] = -xh[1]/h;
            xe[1] =  xh[0]/h;
            xe[2] = 0.0;
         }
      }
      else {                        /* Eccentric */
         xe[0] /= *e;
         xe[1] /= *e;
         xe[2] /= *e;
         if (h < EPS) {             /* Equatorial */
            /* Arbitrarily set RAAN = 0 */
            xn[0] = 1.0;
            xn[1] = 0.0;
            xn[2] = 0.0;
            *RAAN = 0.0;
            *i = (xh[2]>0.0 ? 0.0 : PI );
            /* Find omg */
            hxn[0] = -xh[2]*xn[1];
            hxn[1] = xh[2]*xn[0];
            hxn[2] = xh[0]*xn[1]-xh[1]*xn[0];
            cosw = xe[0]*xn[0]+xe[1]*xn[1];
            sinw = xe[0]*hxn[0]+xe[1]*hxn[1]+xe[2]*hxn[2];
            *ArgP = atan2(sinw,cosw);
         }
         else {                     /* Inclined */
            /* RAAN, omg both well defined */
            xn[0] = -xh[1]/h;
            xn[1] = xh[0]/h;
            xn[2] = 0.0;
            *RAAN = atan2(xn[1],xn[0]);
            *i   =  acos(xh[2]);
            /* Find omg */
            hxn[0] = -xh[2]*xn[1];
            hxn[1] = xh[2]*xn[0];
            hxn[2] = xh[0]*xn[1]-xh[1]*xn[0];
            cosw = xe[0]*xn[0]+xe[1]*xn[1];
            sinw = xe[0]*hxn[0]+xe[1]*hxn[1]+xe[2]*hxn[2];
            *ArgP = atan2(sinw,cosw);
         }
      }

      cth = rhat[0]*xe[0]+rhat[1]*xe[1]+rhat[2]*xe[2];
      *th = acos(cth);

      hxe[0] = xh[1]*xe[2]-xh[2]*xe[1];
      hxe[1] = xh[2]*xe[0]-xh[0]*xe[2];
      hxe[2] = xh[0]*xe[1]-xh[1]*xe[0];

      rohxe = rhat[0]*hxe[0]+rhat[1]*hxe[1]+rhat[2]*hxe[2];
      if (rohxe < 0.0) *th = TWOPI - *th;

      dt = TimeSincePeriapsis(mu,*SLR,*e,*th);
      *tp = time - dt;
#undef EPS
#undef PI
#undef TWOPI
}
/**********************************************************************/
void TLE2MeanEph(const char Line1[80], const char Line2[80], double JD, 
   double LeapSec, struct OrbitType *O)
{
#define EPS (1.0E-12)
#define TWOPI (6.283185307179586)
#define D2R (1.74532925199E-2)

      char YearString[3];
      char DOYstring[13];
      char IncString[9];
      char RAANstring[10];
      char EccString[8];
      char omgstring[9];
      char MeanAnomString[9];
      char MeanMotionString[12];
      long year,DOY,Month,Day;
      double FloatDOY,FracDay,JDepoch;
      double DynTime;
      /* Parameters quoted from SatelliteToolbox.jl's sgp4_model.jl */
      double mu = 3.986005E14;
      double Re = 6378.137E3;
      double J2 = 1.08262998905E-3;
      double Coef;

      strncpy(YearString,&Line1[18],2);
      YearString[2] = 0;
      year = (long) atoi(YearString);
      if (year < 57) year += 2000;
      else year += 1900;
      strncpy(DOYstring,&Line1[20],12);
      DOYstring[12] = 0;
      FloatDOY = (double) atof(DOYstring);
      DOY = (long) FloatDOY;
      FracDay = FloatDOY - ((double) DOY);
      DOY2MD(year,DOY,&Month,&Day);
      JDepoch = DateToJD(year,Month,Day,0,0,0.0);
      JDepoch += FracDay;
      O->Epoch = JDToTime(JDepoch);
      /* Shift Epoch from UTC to TT */
      O->Epoch += LeapSec + 32.184;
      DynTime = JDToTime(JD);

      strncpy(IncString,&Line2[8],8);
      IncString[8] = 0;
      O->inc = ((double) atof(IncString))*D2R;

      strncpy(RAANstring,&Line2[17],9);
      RAANstring[9] = 0;
      O->RAAN = ((double) atof(RAANstring))*D2R;

      strncpy(EccString,&Line2[26],7);
      EccString[7] = 0;
      O->ecc = ((double) atof(EccString))*1.0E-7;

      strncpy(omgstring,&Line2[34],8);
      omgstring[8] = 0;
      O->ArgP = ((double) atof(omgstring))*D2R;

      strncpy(MeanAnomString,&Line2[43],8);
      MeanAnomString[8] = 0;
      O->MeanAnom = ((double) atof(MeanAnomString))*D2R;

      strncpy(MeanMotionString,&Line2[52],11);
      MeanMotionString[11] = 0;
      O->MeanMotion = ((double) atof(MeanMotionString))*TWOPI/86400.0;
      O->Period = TWOPI/(O->MeanMotion);

      /* Time of Periapsis passage given in seconds since J2000 */
      O->tp = O->Epoch - O->MeanAnom/(O->MeanMotion);
      while ((O->tp-DynTime) < -(O->Period)) O->tp += O->Period;
      while ((O->tp-DynTime) >   O->Period ) O->tp -= O->Period;

      O->MeanSMA = pow(mu/(O->MeanMotion*O->MeanMotion),1.0/3.0);
      O->SMA = O->MeanSMA;
      O->alpha = 1.0/(O->SMA);
      O->SLR = O->SMA*(1.0 - O->ecc*O->ecc);
      O->rmin = O->SLR/(1.0 + O->ecc);

      O->anom = MeanAnomToTrueAnom(O->MeanAnom,O->ecc);
            
      /* Initialize J2 Drift Parameters (ref Markley and Crassidis, Ch. 10) */
      /* 10.121 */
      if (O->J2DriftEnabled) {
         Coef = 1.5*J2*Re*Re/(O->SLR*O->SLR)*O->MeanMotion;
         O->RAANdot = -Coef*cos(O->inc);
         O->ArgPdot =  Coef*(2.0-2.5*sin(O->inc)*sin(O->inc));
         O->RAAN0 = O->RAAN - O->RAANdot*(DynTime-O->Epoch);
         O->ArgP0 = O->ArgP - O->ArgPdot*(DynTime-O->Epoch);
         /* 10.122 */
         O->MeanAnom0 = O->MeanAnom - O->MeanMotion*(DynTime-O->Epoch);
         /* 10.126 */
         O->J2Rw2bya = J2*Re*Re/O->MeanSMA;   
      }
      else {
         O->RAANdot = 0.0;
         O->ArgPdot = 0.0;
         O->RAAN0 = O->RAAN;
         O->ArgP0 = O->ArgP;
         O->J2Rw2bya = 0.0;
         /* 10.122 */
         O->MeanAnom0 = O->MeanAnom - O->MeanMotion*(DynTime-O->Epoch);
      }   

#undef TWOPI
#undef D2R
}
/**********************************************************************/
/* Ref: Markley and Crassidis, 10.4.3                                 */
/* Osculating elements drift from initial conditions due to J2        */
void MeanEph2RV(struct OrbitType *O, double DynTime)
{
      double e,e2,sin2i,sinw,sin2w,cosnu,g;
      double CPN[3][3],cth,sth,R,c2,pr[3],pv[3];
      double C1,S1,C2,S2,C3,S3;
      long i;

      /* 10.121a,b */
      if (O->J2DriftEnabled) {
         O->ArgP = O->ArgP0 + O->ArgPdot*(DynTime-O->Epoch);
         O->RAAN = O->RAAN0 + O->RAANdot*(DynTime-O->Epoch);
      }

      /* 10.122 */
      O->MeanAnom = O->MeanAnom0 + O->MeanMotion*(DynTime - O->Epoch);
      
      O->anom = MeanAnomToTrueAnom(O->MeanAnom,O->ecc);
      
      e = O->ecc;
      e2 = e*e;
      sin2i = sin(O->inc)*sin(O->inc);
      
      /* 10.127 */
      sinw = sin(O->ArgP+O->anom);
      sin2w = sinw*sinw;
      cosnu = cos(O->anom);
      g = pow((1.0+e*cosnu)/(1.0-e2),3.0)*(1.0-3.0*sin2i*sin2w);

      /* 10.126 */
      O->SMA = O->MeanSMA + O->J2Rw2bya*g;
      
      O->SLR = O->SMA*(1.0-e2);
      O->alpha = 1.0/O->SMA;
      O->rmin = O->SLR/(1.0 + O->ecc);

      sth = sin(O->anom);
      cth = cos(O->anom);
      R = O->SLR/(1.0+e*cth);

      c2 = sqrt(O->mu/O->SLR);
      pr[0] = R*cth;
      pr[1] = R*sth;
      pr[2] = 0.0;
      pv[0] = -c2*sth;
      pv[1] = c2*(e+cth);
      pv[2] = 0.0;

      C1=cos(O->RAAN);S1=sin(O->RAAN);
      C2=cos(O->inc);S2=sin(O->inc);
      C3=cos(O->ArgP);S3=sin(O->ArgP);

      CPN[0][0]=-S1*C2*S3+C3*C1;
      CPN[1][0]=-S1*C2*C3-S3*C1;
      CPN[2][0]=S1*S2;
      CPN[0][1]=C1*C2*S3+C3*S1;
      CPN[1][1]=C1*C2*C3-S3*S1;
      CPN[2][1]=-C1*S2;
      CPN[0][2]=S2*S3;
      CPN[1][2]=S2*C3;
      CPN[2][2]=C2;

      for(i=0;i<3;i++) {
         O->PosN[i] = pr[0]*CPN[0][i] + pr[1]*CPN[1][i];
         O->VelN[i] = pv[0]*CPN[0][i] + pv[1]*CPN[1][i];
      }

}
/**********************************************************************/
/* TLEs use UTC.  42 orbits use TT.  So LeapSec are needed.           */
long LoadTleFromFile(const char *Path, const char *TleFileName,
   const char *TleLabel, double DynTime, double JD, double LeapSec, 
   struct OrbitType *O)
{
      FILE *infile;
      char line[80],line1[80],line2[80];
      char Label[25];
      long i,Nchar;
      long Success = 0;

      infile=FileOpen(Path,TleFileName,"r");

      Nchar = strlen(TleLabel);
      if (Nchar > 24) Nchar = 24;
      /* Pad label to 24 characters to assure unique match */
      for(i=0;i<Nchar;i++) Label[i] = TleLabel[i];
      for(i=Nchar;i<24;i++) Label[i] = ' ';
      Label[24] = '\0';
      while(!feof(infile) && !Success) {
         fgets(line,80,infile);
         if (!strncmp(line,Label,Nchar)) {
            Success = 1;
            fgets(line1,80,infile);
            fgets(line2,80,infile);
            TLE2MeanEph(line1,line2,JD,LeapSec,O);
            MeanEph2RV(O,DynTime);
         }
      }
      fclose(infile);

      return(Success);
}
/**********************************************************************/
/* Periapsis position and velocity might make a useful orbit element  */
/* set for some applications.  This function finds them from the      */
/* given position and velocity.                                       */
double RV2RVp(double mu, double r[3], double v[3], double rp[3], double vp[3])
{
      double magr,magv;
      double E,rxv[3],h,p,e,magrp,magvp,cth,sth;
      double Cer,Cev,Cpr,Cpv,ie[3],ip[3],anom;
      long i;

      magr = MAGV(r);
      magv = MAGV(v);

      E = 0.5*magv*magv-mu/magr;
      VxV(r,v,rxv);
      h = MAGV(rxv);
      p = h*h/mu;
      e = sqrt(1.0+2.0*E*p/mu);
      magrp = p/(1.0+e);
      magvp = h/magrp;
      if (e == 0.0) {
         cth = 1.0;
         sth = 0.0;
      }
      else {
         cth = (p/magr-1.0)/e;
         sth = sqrt(1.0-cth*cth);
         if (VoV(r,v) < 0.0) sth = -sth;
      }

      Cer = (e+cth)/p;
      Cev = -magr/h*sth;
      Cpr = sth/p;
      Cpv = magr/h*cth;
      for(i=0;i<3;i++) {
         ie[i] = Cer*r[i] + Cev*v[i];
         ip[i] = Cpr*r[i] + Cpv*v[i];
      }
      UNITV(ie);
      UNITV(ip);
      for(i=0;i<3;i++) {
         rp[i] = magrp*ie[i];
         vp[i] = magvp*ip[i];
      }
      anom = atan2(sth,cth);

      return(anom);
}
/**********************************************************************/
/*  This function finds the mean orbit of planet "i" with respect to  */
/*  the mean-equinox-of-date frame.  Ref. Chap 31 of Meeus,           */
/*  "Astronomical Algorithms", second edition, QB51.3.E43, M42, 1998. */
/*  Index 1=Mercury, 2=Venus, ... 9=Pluto.  0=Sun is not used.        */
/*  Note that the elements for Pluto are not from Meeus, but from a   */
/*  lower-fidelity data set from JPL.                                 */
void PlanetEphemerides(long i, double JD, double mu,
       double *SMA, double *ecc, double *inc, double *RAAN, double *ArgP,
       double *tp, double *anom, double *SLR, double *alpha, double *rmin,
       double *MeanMotion, double *Period)
{
#define TWOPI (6.283185307179586)

      double La0[10]={0.0,252.250906,181.979801,100.466457,355.433,
                      34.351519,50.077444,314.055005,304.348665,
                      238.92881};
      double La1[10]={0.0,149474.0722491,58519.2130302,36000.7698278,
                      19141.6964471,3036.3027748,1223.5110686,
                      429.8640561,219.8833092,145.20775};
      double La2[10]={0.0,0.0003035,0.00031014,0.00030322,0.00031052,
                      0.0002233,0.00051908,0.00030390,0.00030882,0.0};
      double La3[10]={0.0,18.0E-9,15.0E-9,20.0E-9,16.0E-9,37.0E-9,
                      -3.0E-8,26.0E-9,18.0E-9,0.0};

      double aa0[10]={0.0,0.387098310,0.723329820,1.000001018,
                      1.523679342,5.202603209,9.554909192,
                      19.218446062,30.110386869,39.48168677};
      double aa1[10]={0.0,0.0,0.0,0.0,0.0,1.913E-7,-2.139E-6,-3.72E-8,
                      -1.663E-7,-7.6912E-4};
      double aa2[10]={0.0,0.0,0.0,0.0,0.0,0.0,4E-9,9.8E-10,6.9E-10,0.0};

      double ea0[10]={0.0,0.20563175,0.00677192,0.01670863,0.09340065,
                      0.04849793,0.05554814,0.04638122,0.00945575,
                      0.24880766};
      double ea1[10]={0.0,2.0407E-5,-4.7765E-5,-4.2037E-5,9.0484E-5,
                      1.63225E-4,-3.46641E-4,-2.7293E-5,6.033E-6,
                      6.465E-5};
      double ea2[10]={0.0,-2.83E-8,9.81E-8,-1.267E-7,-8.06E-8,
                      -4.714E-7,-6.436E-7,7.89E-8,0.0,0.0};
      double ea3[10]={0.0,-1.8E-10,4.6E-10,1.4E-10,-2.5E-10,
                      -2.01E-9,3.4E-9,2.4E-10,-5.0E-11,0.0};

      double ia0[10]={0.0,7.004986,3.394662,0.0,1.849726,1.303267,
                      2.488879,0.773197,1.769953,17.14175};
      double ia1[10]={0.0,0.0018215,0.0010037,0.0,-0.0006011,-0.0054965,
                      -0.0037362,0.0007744,-0.0093082,0.003075};
      double ia2[10]={0.0,-1.81E-5,-8.8E-7,0.0,1.276E-5,4.66E-5,
                      -1.519E-5,3.749E-5,-7.08E-6,0.0};
      double ia3[10]={0.0,5.6E-8,-7.0E-9,0.0,-7.0E-9,-2.0E-9,8.7E-8,
                      -9.2E-8,2.7E-8,0.0};

      double Oa0[10]={0.0,48.330893,76.67992,0.0,49.558093,100.464407,
                      113.665503,74.005957,131.784057,110.30347};
      double Oa1[10]={0.0,1.1861883,0.9011206,0.0,0.7720959,1.0209774,
                      0.877088,0.5211278,1.1022039,-0.01037};
      double Oa2[10]={0.0,1.7542E-4,4.0618E-4,0.0,1.557E-5,4.0315E-4,
                      -1.2176E-4,1.33947E-3,2.5952E-4,0.0};
      double Oa3[10]={0.0,2.15E-7,-9.3E-8,0.0,2.267E-6,4.04E-7,
                      -2.249E-6,1.8484E-5,-6.37E-7,0.0};

      double pa0[10]={0.0,77.456119,131.563703,102.937348,336.060234,
                      14.331207,93.057237,173.005291,48.120276,
                      224.06676};
      double pa1[10]={0.0,1.5564776,1.4022288,1.7195366,1.8410449,
                      1.6126352,1.9637613,1.486379,1.4262957,-0.036736};
      double pa2[10]={0.0,2.9544E-4,-1.07618E-3,4.5688E-4,1.3477E-4,
                      1.03042E-3,8.3753E-4,2.1406E-4,3.8434E-4,0.0};
      double pa3[10]={0.0,9.0E-9,-5.678E-6,-1.8E-8,5.36E-7,-4.464E-6,
                      4.928E-6,4.34E-7,2.0E-8,0.0};
      double T,L,Pi,M,dt,SecSinceJ2000;

      double D2R = atan(1.0)/45.0;
      double AU2m = 149597870000.0;

/* .. Time since J2000, in Julian centuries */
      T = (JD - 2451545.0)/36525.0;

/* .. Time since J2000, in seconds */
      SecSinceJ2000 = (JD - 2451545.0)*86400.0;

/* .. Mean Longitude */
      L = (La0[i]+T*(La1[i]+T*(La2[i]+T*La3[i])))*D2R;
/* .. Semi-major axis */
      *SMA = (aa0[i]+T*(aa1[i]+T*aa2[i]))*AU2m;
/* .. Eccentricity */
      *ecc  = ea0[i]+T*(ea1[i]+T*(ea2[i]+T*ea3[i]));
/* .. Inclination to ecliptic */
      *inc  = (ia0[i]+T*(ia1[i]+T*(ia2[i]+T*ia3[i])))*D2R;
/* .. Right ascension of ascending node */
      *RAAN  = (Oa0[i]+T*(Oa1[i]+T*(Oa2[i]+T*Oa3[i])))*D2R;
/* .. Right ascension of perihelion */
      Pi    = (pa0[i]+T*(pa1[i]+T*(pa2[i]+T*pa3[i])))*D2R;

/* .. Argument of perihelion */
      *ArgP = Pi - *RAAN;
/* .. Mean anomaly */
      M = L - Pi;
/* .. Time since perihelion passage */
      dt = M*sqrt((*SMA)*(*SMA)*(*SMA)/mu);
/* .. Time of perihelion passage */
      *tp = SecSinceJ2000 - dt;

/* .. Semilatus rectum */
      *SLR = *SMA * (1.0- (*ecc)*(*ecc));
/* .. Alpha is inverse of axis */
      *alpha = 1.0/ (*SMA);
/* .. Periapsis radius */
      *rmin = *SMA * (1.0 - (*ecc) );

/* .. True anomaly */
      *anom = TrueAnomaly(mu, *SLR, *ecc, dt);

      *MeanMotion = sqrt(mu/(*SMA)/(*SMA)/(*SMA));
      *Period = TWOPI/(*MeanMotion);

#undef TWOPI
}
/*********************************************************************/
/*  This function gives the location of Luna, with respect to the    */
/*  geocentric ecliptic frame.  Refer to Chap 47 of Meeus,           */
/*  "Astronomical Algorithms" QB51.3.E43 M42, 1998.                  */
void LunaPosition(double JD, double r[3])
{

      double T,Lp,D,M,Mp,F,A1,A2,A3,E,E2,SumL,SumR,SumB,arg;
      double Lat,Lng,Delta;
      double D2R = atan(1.0)/45.0;

      T=(JD-2451545.0)/36525.0;

      Lp=(218.3164477 + T*(481267.88123421 + T*
          (-1.5786E-3 + T*(1.855835024E-6 - T/65194000.0))))*D2R;
      D = (297.8501921 + T*(445267.1114034 + T*
          (-1.8819E-3 + T*(1.831944719E-6 - T/113065000.0))))*D2R;
      M = (357.5291092 + T*(35999.0502909 + T*
          (-1.536E-4 + T/24490000.0)))*D2R;
      Mp= (134.9633964 + T*(477198.8675055 + T*
          (8.7414E-3 + T*(1.434740814E-5 - T/14712000.0))))*D2R;
      F = (93.272095 + T*(483202.0175233 + T*
          (-3.6539E-3 +T*(-2.836074872E-7 + T/863310000.0))))*D2R;
      A1 = (119.75 + 131.849*T)*D2R;
      A2 = (53.09 + 479264.29*T)*D2R;
      A3 = (313.45 + 481266.484*T)*D2R;
      E = 1.0-2.516E-3*T-7.4E-6*T*T;
      E2=E*E;

      SumL = 0.0;
      SumR = 0.0;
      SumB = 0.0;

      arg=Mp;
      SumL+=6288774.0*sin(arg);
      SumR+=-20905355.0*cos(arg);

      arg=2.0*D-Mp;
      SumL+=1274027.0*sin(arg);
      SumR+=-3699111.0*cos(arg);

      arg=2.0*D;
      SumL+=658314.0*sin(arg);
      SumR+=-2955968.0*cos(arg);

      arg=2.0*Mp;
      SumL+=213618.0*sin(arg);
      SumR+=-569925.0*cos(arg);

      arg=M;
      SumL+=-185116.0*E*sin(arg);
      SumR+=48888.0*E*cos(arg);

      arg=2.0*F;
      SumL+=-114332.0*sin(arg);
      SumR+=-3149.0*cos(arg);

      arg=2.0*(D-Mp);
      SumL+=58793.0*sin(arg);
      SumR+=246158.0*cos(arg);

      arg=2.0*D-M-Mp;
      SumL+=57066.0*E*sin(arg);
      SumR+=-152138.0*E*cos(arg);

      arg=2.0*D+Mp;
      SumL+=53322.0*sin(arg);
      SumR+=-170733.0*cos(arg);

      arg=2.0*D-M;
      SumL+=45758.0*E*sin(arg);
      SumR+=-204586.0*E*cos(arg);

      arg=M-Mp;
      SumL+=-40923.0*E*sin(arg);
      SumR+=-129620.0*E*cos(arg);

      arg=2.0*D;
      SumL+=-34720.0*sin(arg);
      SumR+=108743.0*cos(arg);

      arg=M+Mp;
      SumL+=-30383.0*E*sin(arg);
      SumR+=104755.0*E*cos(arg);

      arg=2.0*(D-F);
      SumL+=15327.0*sin(arg);
      SumR+=10321.0*cos(arg);

      arg=Mp+2.0*F;
      SumL+=-12528.0*sin(arg);

      arg=Mp-2.0*F;
      SumL+=10980.0*sin(arg);
      SumR+=79661.0*cos(arg);

      arg=4.0*D-Mp;
      SumL+=10675.0*sin(arg);
      SumR+=-34782.0*cos(arg);

      arg=3.0*Mp;
      SumL+=10034.0*sin(arg);
      SumR+=-23210.0*cos(arg);

      arg=4.0*D-2.0*Mp;
      SumL+=8548.0*sin(arg);
      SumR+=-21636.0*cos(arg);

      arg=2.0*D+M-Mp;
      SumL+=-7888.0*E*sin(arg);
      SumR+=24208.0*E*cos(arg);

      arg=2.0*D+M;
      SumL+=-6766.0*E*sin(arg);
      SumR+=30824.0*E*cos(arg);

      arg=D-Mp;
      SumL+=-5163.0*sin(arg);
      SumR+=-8379.0*cos(arg);

      arg=D+M;
      SumL+=4987.0*E*sin(arg);
      SumR+=-16675.0*E*cos(arg);

      arg=2.0*D-M+Mp;
      SumL+=4036.0*E*sin(arg);
      SumR+=-12831.0*E*cos(arg);

      arg=2.0*(D+Mp);
      SumL+=3994.0*sin(arg);
      SumR+=-10445.0*cos(arg);

      arg=4.0*D;
      SumL+=3861.0*sin(arg);
      SumR+=-11650.0*cos(arg);

      arg=2.0*D-3.0*Mp;
      SumL+=3665.0*sin(arg);
      SumR+=14403.0*cos(arg);

      arg=M-2.0*Mp;
      SumL+=-2689.0*E*sin(arg);
      SumR+=-7003.0*E*cos(arg);

      arg=2.0*(D+F)-Mp;
      SumL+=-2602.0*sin(arg);

      arg=2.0*(D-Mp)-M;
      SumL+=2390.0*E*sin(arg);
      SumR+=10056.0*E*cos(arg);

      arg=D+Mp;
      SumL+=-2348.0*sin(arg);
      SumR+=6322.0*cos(arg);

      arg=2.0*(D-M);
      SumL+=2236.0*E2*sin(arg);
      SumR+=-9884.0*E2*cos(arg);

      arg=M+2.0*Mp;
      SumL+=-2120.0*E*sin(arg);
      SumR+=5751.0*E*cos(arg);

      arg=2.0*M;
      SumL+=-2069.0*E2*sin(arg);

      arg=2.0*(D-M)-Mp;
      SumL+=2048.0*E2*sin(arg);
      SumR+=-4950.0*E2*cos(arg);

      arg=2.0*(D-F)+Mp;
      SumL+=-1773.0*sin(arg);
      SumR+=4130.0*cos(arg);

      arg=2.0*(D+F);
      SumL+=-1595.0*sin(arg);

      arg=4.0*D-M-Mp;
      SumL+=1215.0*E*sin(arg);
      SumR+=-3958.0*E*cos(arg);

      arg=2.0*(Mp+F);
      SumL+=-1110.0*sin(arg);

      arg=3.0*D-Mp;
      SumL+=-892.0*sin(arg);
      SumR+=3258.0*cos(arg);

      arg=2.0*D+M+Mp;
      SumL+=-810.0*E*sin(arg);
      SumR+=2616.0*E*cos(arg);

      arg=4.0*D-M-2.0*Mp;
      SumL+=759.0*E*sin(arg);
      SumR+=-1897.0*E*cos(arg);

      arg=2.0*M-Mp;
      SumL+=-713.0*E2*sin(arg);
      SumR+=-2117.0*E2*cos(arg);

      arg=2.0*(D+M)-Mp;
      SumL+=-700.0*E2*sin(arg);
      SumR+=2354.0*E2*cos(arg);

      arg=2.0*(D-Mp)+M;
      SumL+=691.0*E*sin(arg);

      arg=2.0*(D-F)-M;
      SumL+=596.0*E*sin(arg);

      arg=4.0*D+Mp;
      SumL+=549.0*sin(arg);
      SumR+=-1423.0*cos(arg);

      arg=4.0*Mp;
      SumL+=537.0*sin(arg);
      SumR+=-1117.0*cos(arg);

      arg=4.0*D-M;
      SumL+=520.0*E*sin(arg);
      SumR+=-1571.0*E*cos(arg);

      arg=D-2.0*Mp;
      SumL+=-487.0*sin(arg);
      SumR+=-1739.0*cos(arg);

      arg=2.0*(D-F)+M;
      SumL+=-399.0*E*sin(arg);

      arg=2.0*(Mp-F);
      SumL+=-381.0*sin(arg);
      SumR+=-4421.0*cos(arg);

      arg=D+M+Mp;
      SumL+=351.0*E*sin(arg);

      arg=3.0*D-2.0*Mp;
      SumL+=-340.0*sin(arg);

      arg=4.0*D-3.0*Mp;
      SumL+=330.0*sin(arg);

      arg=2.0*(D+Mp)-M;
      SumL+=327.0*E*sin(arg);

      arg=2.0*M+Mp;
      SumL+=-323.0*E2*sin(arg);
      SumR+=1165.0*E2*cos(arg);

      arg=D+M-Mp;
      SumL+=299.0*E*sin(arg);

      arg=2.0*D+3.0*Mp;
      SumL+=294.0*sin(arg);

      arg=2.0*(D-F)-Mp;
      SumR+=8752.0*cos(arg);

      SumB+=5128122.0*sin(F);
      SumB+=280602.0*sin(Mp+F);
      SumB+=277693.0*sin(Mp-F);
      SumB+=173237.0*sin(2.0*D-F);
      SumB+=55413.0*sin(2.0*D-Mp+F);
      SumB+=46271.0*sin(2.0*D-Mp-F);
      SumB+=32573.0*sin(2.0*D+F);
      SumB+=17198.0*sin(2.0*Mp+F);
      SumB+=9266.0*sin(2.0*D+Mp-F);
      SumB+=8822.0*sin(2.0*Mp-F);
      SumB+=8216.0*E*sin(2.0*D-M-F);
      SumB+=4324.0*sin(2.0*(D-Mp)-F);
      SumB+=4200.0*sin(2.0*D+Mp+F);
      SumB+=-3359.0*E*sin(2.0*D+M-F);
      SumB+=2463.0*E*sin(2.0*D-M-Mp+F);
      SumB+=2211.0*E*sin(2.0*D-M+F);
      SumB+=2065.0*E*sin(2.0*D-M-Mp-F);
      SumB+=-1870.0*E*sin(M-Mp-F);
      SumB+=1828.0*sin(4.0*D-Mp-F);
      SumB+=-1794.0*E*sin(M+F);
      SumB+=-1749.0*sin(3.0*F);
      SumB+=-1565.0*E*sin(M-Mp+F);
      SumB+=-1491.0*sin(D+F);
      SumB+=-1475.0*E*sin(M+Mp+F);
      SumB+=-1410.0*E*sin(M+Mp-F);
      SumB+=-1344.0*E*sin(M-F);
      SumB+=-1335.0*sin(D-F);
      SumB+=1107.0*sin(3.0*Mp+F);
      SumB+=1021.0*sin(4.0*D-F);
      SumB+=833.0*sin(4.0*D-Mp+F);
      SumB+=777.0*sin(Mp-3.0*F);
      SumB+=671.0*sin(4.0*D-2.0*Mp+F);
      SumB+=607.0*sin(2.0*D-3.0*F);
      SumB+=596.0*sin(2.0*(D+Mp)-F);
      SumB+=491.0*E*sin(2.0*D-M+Mp-F);
      SumB+=-451.0*sin(2.0*(D-Mp)+F);
      SumB+=439.0*sin(3.0*Mp-F);
      SumB+=422.0*sin(2.0*(D+Mp)+F);
      SumB+=421.0*sin(2.0*D-3.0*Mp-F);
      SumB+=-366.0*E*sin(2.0*D+M-Mp+F);
      SumB+=-351.0*E*sin(2.0*D+M+F);
      SumB+=331.0*sin(4.0*D+F);
      SumB+=315.0*E*sin(2.0*D-M+Mp+F);
      SumB+=302.0*E2*sin(2.0*(D-M)-F);
      SumB+=-283.0*sin(Mp+3.0*F);
      SumB+=-229.0*E*sin(2.0*D+M+Mp-F);
      SumB+=223.0*E*(sin(D+M-F)+sin(D+M+F));
      SumB+=-220.0*E*(sin(M-2.0*Mp-F)+sin(2.0*D+M-Mp-F));
      SumB+=-185.0*sin(D+Mp+F);
      SumB+=181.0*E*sin(2.0*(D-Mp)-M-F);
      SumB+=-177.0*E*sin(M+2.0*Mp+F);
      SumB+=176.0*sin(4.0*D-2.0*Mp-F);
      SumB+=166.0*E*sin(4.0*D-M-Mp-F);
      SumB+=-164.0*sin(D+Mp-F);
      SumB+=132.0*sin(4.0*D+Mp-F);
      SumB+=-119.0*sin(D-Mp-F);
      SumB+=115.0*E*sin(4.0*D-M-F);
      SumB+=107.0*E2*sin(2.0*(D-M)+F);

      SumL += 3958.0*sin(A1)+1962.0*sin(Lp-F)+318.0*sin(A2);
      SumB += -2235.0*sin(Lp)+382.0*sin(A3)+175.0*sin(A1-F)
              +175.0*sin(A1+F)+127.0*sin(Lp-Mp)-115.0*sin(Lp+Mp);

      Lng = Lp + 1.0E-6*SumL*D2R;
      Lat = 1.0E-6*SumB*D2R;
      Delta = 385000.56E3 + SumR;

      r[0] = Delta*cos(Lng)*cos(Lat);
      r[1] = Delta*sin(Lng)*cos(Lat);
      r[2] = Delta*sin(Lat);

}
/**********************************************************************/
/*  Ref JPL D-32296, "Lunar Constants and Models Document"            */
/*  http://ssd.jpl.nasa.gov/?lunar_doc                                */
/*  Finds Lunar Inertial Frame wrt J2000                              */
void LunaInertialFrame(double JulDay, double CNJ[3][3])
{
#define D2R (1.74532925199E-2)
      double D,T;
      double E1,E2,E3,E4,E6,E7,E10,E12,E13;
      double SinE1,SinE2,SinE3,SinE4,SinE6;
      double SinE10,SinE13;
      double CosE1,CosE2,CosE3,CosE4,CosE6,CosE7,CosE10,CosE13;
      double PoleRA,PoleDec,PoleVec[3],NodeVec[3],YVec[3];
      long i;

      D = JulDay - 2451545.0;
      T = D/36525.0;

      E1 = fmod(125.045 - 0.0529921*D,360.0)*D2R;
      E2 = fmod(250.089 - 0.1059842*D,360.0)*D2R;
      E3 = fmod(260.008 + 13.0120009*D,360.0)*D2R;
      E4 = fmod(176.625 + 13.3407154*D,360.0)*D2R;
      E6 = fmod(311.589 + 26.4057084*D,360.0)*D2R;
      E7 = fmod(134.963 + 13.0649930*D,360.0)*D2R;
      E10 = fmod(15.134 - 0.1589763*D,360.0)*D2R;
      E12 = fmod(239.961 + 0.1643573*D,360.0)*D2R;
      E13 = fmod(25.053 + 12.9590088*D,360.0)*D2R;

      SinE1 = sin(E1);
      SinE2 = sin(E2);
      SinE3 = sin(E3);
      SinE4 = sin(E4);
      SinE6 = sin(E6);
      SinE10 = sin(E10);
      SinE13 = sin(E13);

      CosE1 = cos(E1);
      CosE2 = cos(E2);
      CosE3 = cos(E3);
      CosE4 = cos(E4);
      CosE6 = cos(E6);
      CosE7 = cos(E7);
      CosE10 = cos(E10);
      CosE13 = cos(E13);

      PoleRA = 269.9949 + 0.0031*T - 3.8787*SinE1 - 0.1204*SinE2
         + 0.0700*SinE3 -0.0172*SinE4 + 0.0072*SinE6
         - 0.0052*SinE10 + 0.0043*SinE13;

      PoleDec = 66.5392 + 0.0130*T + 1.5419*CosE1 + 0.0239*CosE2
         -0.0278*CosE3 + 0.0068*CosE4 - 0.0029*CosE6
        + 0.0009*CosE7 + 0.0008*CosE10 - 0.0009*CosE13;

      PoleRA *= D2R;
      PoleDec *= D2R;

      /* Derive Pole Vector in J2000 */
      PoleVec[0] = cos(PoleRA)*cos(PoleDec);
      PoleVec[1] = sin(PoleRA)*cos(PoleDec);
      PoleVec[2] = sin(PoleDec);

      /* IAU convention puts the X axis at Z(J2000) x PoleVec */
      NodeVec[0] = -PoleVec[1];
      NodeVec[1] =  PoleVec[0];
      NodeVec[2] = 0.0;
      UNITV(NodeVec);

      VxV(PoleVec,NodeVec,YVec);
      UNITV(YVec);

      /* Luna's N frame wrt J2000 */
      for(i=0;i<3;i++) {
         CNJ[0][i] = NodeVec[i];
         CNJ[1][i] = YVec[i];
         CNJ[2][i] = PoleVec[i];
      }
#undef D2R
}
/**********************************************************************/
/*  Ref JPL D-32296, "Lunar Constants and Models Document"            */
/*  http://ssd.jpl.nasa.gov/?lunar_doc                                */
double LunaPriMerAng(double JulDay)
{
#define D2R (1.74532925199E-2)
      double D;
      double E1,E2,E3,E4,E5,E6,E7,E8,E9,E10,E11,E12,E13;
      double SinE1,SinE2,SinE3,SinE4,SinE5,SinE6,SinE7;
      double SinE8,SinE9,SinE10,SinE11,SinE12,SinE13;
      double PriMerAng;

      D = JulDay - 2451545.0;

      E1 = fmod(125.045 - 0.0529921*D,360.0)*D2R;
      E2 = fmod(250.089 - 0.1059842*D,360.0)*D2R;
      E3 = fmod(260.008 + 13.0120009*D,360.0)*D2R;
      E4 = fmod(176.625 + 13.3407154*D,360.0)*D2R;
      E5 = fmod(357.529 + 0.9856003*D,360.0)*D2R;
      E6 = fmod(311.589 + 26.4057084*D,360.0)*D2R;
      E7 = fmod(134.963 + 13.0649930*D,360.0)*D2R;
      E8 = fmod(276.617 + 0.3287146*D,360.0)*D2R;
      E9 = fmod(34.226 + 1.7484877*D,360.0)*D2R;
      E10 = fmod(15.134 - 0.1589763*D,360.0)*D2R;
      E11 = fmod(119.743 + 0.0036096*D,360.0)*D2R;
      E12 = fmod(239.961 + 0.1643573*D,360.0)*D2R;
      E13 = fmod(25.053 + 12.9590088*D,360.0)*D2R;

      SinE1 = sin(E1);
      SinE2 = sin(E2);
      SinE3 = sin(E3);
      SinE4 = sin(E4);
      SinE5 = sin(E5);
      SinE6 = sin(E6);
      SinE7 = sin(E7);
      SinE8 = sin(E8);
      SinE9 = sin(E9);
      SinE10 = sin(E10);
      SinE11 = sin(E11);
      SinE12 = sin(E12);
      SinE13 = sin(E13);

      PriMerAng = 38.3213 + 13.17635815*D - 1.4E-12*D*D
         + 3.5610*SinE1 + 0.1208*SinE2 - 0.0642*SinE3
         + 0.0158*SinE4 + 0.0252*SinE5 - 0.0066*SinE6
         - 0.0047*SinE7 - 0.0046*SinE8 + 0.0028*SinE9
         + 0.0052*SinE10 + 0.0040*SinE11 + 0.0019*SinE12
         - 0.0044*SinE13;

      return(PriMerAng*D2R);
#undef D2R
}
/**********************************************************************/
void FindCLN(double r[3], double v[3], double CLN[3][3], double wln[3])
{
      double L1[3],L2[3],L3[3],m,h[3],rr,hh;
      long i;

      h[0] = r[1]*v[2]-r[2]*v[1];
      h[1] = r[2]*v[0]-r[0]*v[2];
      h[2] = r[0]*v[1]-r[1]*v[0];
      rr = r[0]*r[0]+r[1]*r[1]+r[2]*r[2];
      hh = h[0]*h[0]+h[1]*h[1]+h[2]*h[2];

      for(i=0;i<3;i++) {
         wln[i] = h[i]/rr;
         L3[i] = -r[i];
         L2[i] = -h[i];
      }

      m=sqrt(rr);
      L3[0] /= m;
      L3[1] /= m;
      L3[2] /= m;

      if (hh == 0.0) { /* Rectlinear Motion */
         PerpBasis(L3,L1,L2);
      }
      else {
         m=sqrt(L2[0]*L2[0]+L2[1]*L2[1]+L2[2]*L2[2]);
         L2[0] /= m;
         L2[1] /= m;
         L2[2] /= m;

         L1[0] = L2[1]*L3[2]-L2[2]*L3[1];
         L1[1] = L2[2]*L3[0]-L2[0]*L3[2];
         L1[2] = L2[0]*L3[1]-L2[1]*L3[0];

         m=sqrt(L1[0]*L1[0]+L1[1]*L1[1]+L1[2]*L1[2]);
         L1[0] /= m;
         L1[1] /= m;
         L1[2] /= m;
      }

      for(i=0;i<3;i++){
         CLN[0][i] = L1[i];
         CLN[1][i] = L2[i];
         CLN[2][i] = L3[i];
      }

}
/**********************************************************************/
void FindENU(double PosN[3],double WorldW, double CLN[3][3], double wln[3])
{
      double Zaxis[3] = {0.0,0.0,1.0};
      double East[3],North[3],Up[3];
      long i;

      CopyUnitV(PosN,Up);
      VxV(Zaxis,Up,East);
      UNITV(East);
      VxV(Up,East,North);
      UNITV(North);
      for(i=0;i<3;i++) {
         CLN[0][i] = East[i];
         CLN[1][i] = North[i];
         CLN[2][i] = Up[i];
      }
      wln[0] = 0.0;
      wln[1] = 0.0;
      wln[2] = WorldW;
}
/**********************************************************************/
/*  Consider the Circular Restricted Three-Body Problem, with two     */
/*  massive bodies (masses m1 and m2, m2 < m1) and a body of          */
/*  negligible mass.  The locations of the Lagrange points are        */
/*  functions of the mass ratio rho = m2/(m1+m2).                     */
/*                                                                    */
/*  Reference Bong Wie, "Space Vehicle Dynamics and Control"          */
/*  (TL1050.W52)Sec 3.7.3                                             */
/*  Also see LagModes.pdf for dimensioned derivations.                */
void FindLagPtParms(struct LagrangeSystemType *LS)
{
      struct LagrangePointType *LP;
      double rho,x,dx,f,fx;
      double eps = 2.0E-16;
      double rho1,xp,xp1;
      double n,D,X0rD,X0r1D,MuSum,a,b,c,s2;
      double R13,R15;
      double R23,R25;
      double alpha;

      rho = LS->rho;
      rho1 = 1.0 - rho;
      MuSum = LS->mu1+LS->mu2;
      n = LS->MeanRate;
      D = LS->SMA;

/* .. L1 */
      LP = &LS->LP[0];
      x = -1.0;
      do {
         xp = x - rho;
         xp1 = xp + 1.0;
         f = x + rho1/(xp*xp) - rho/(xp1*xp1);
         fx = 1.0 - 2.0*rho1/(xp*xp*xp) + 2.0*rho/(xp1*xp1*xp1);
         dx = -f/fx;
         x += dx;
      } while(fabs(dx) > eps);
      LP->X0 = x*D;
      LP->Y0 = 0.0;

      X0rD = LP->X0 - rho*D;
      X0r1D = LP->X0 + rho1*D;

      LP->R1 = sqrt(X0rD*X0rD+LP->Y0*LP->Y0);
      LP->R2 = sqrt(X0r1D*X0r1D+LP->Y0*LP->Y0);
      R13 = 1.0/(LP->R1*LP->R1*LP->R1);
      R15 = R13/(LP->R1*LP->R1);
      R23 = 1.0/(LP->R2*LP->R2*LP->R2);
      R25 = R23/(LP->R2*LP->R2);

      LP->Kxx = MuSum*(rho1*(R13-3.0*R15*X0rD*X0rD)+rho*(R23-3.0*R25*X0r1D*X0r1D));
      LP->Kxy = 0.0;
      LP->Kyy = MuSum*(rho1*(R13-3.0*R15*LP->Y0*LP->Y0)+rho*(R23-3.0*R25*LP->Y0*LP->Y0));

      a = 1.0;
      b = LP->Kxx+LP->Kyy+2.0*n*n;
      c = (LP->Kxx-n*n)*(LP->Kyy-n*n)-LP->Kxy*LP->Kxy;
      s2 = (-b - sqrt(b*b-4*a*c))/(2.0*a);
      LP->w1 = sqrt(-s2);
      LP->w2 = 0.0;
      s2 = (-b + sqrt(b*b-4*a*c))/(2.0*a);
      LP->sigma = sqrt(s2);
      LP->wz = sqrt(MuSum*(rho1*R13+rho*R23));

      LP->Zw1 = LP->Kyy-n*n-LP->w1*LP->w1;
      LP->Zw2 = 0.0;
      LP->Zs = LP->Kyy-n*n-LP->sigma*LP->sigma;

      LP->ca1 = 1.0;
      LP->sa1 = 0.0;
      LP->ca2 = 1.0;
      LP->sa2 = 0.0;

      LP->AR1 = -(LP->Zw1)/(2.0*n*LP->w1);
      LP->AR2 = 0.0;

/* .. L2 */
      LP = &LS->LP[1];
      x = -1.0;
      do {
         xp = x-rho;
         xp1 = xp + 1.0;
         f = x + rho1/(xp*xp) + rho/(xp1*xp1);
         fx = 1.0 - 2.0*rho1/(xp*xp*xp) - 2.0*rho/(xp1*xp1*xp1);
         dx = -f/fx;
         x += dx;
      } while(fabs(dx) > eps);
      LP->X0 = x*D;
      LP->Y0 = 0.0;

      X0rD = LP->X0 - rho*D;
      X0r1D = LP->X0 + rho1*D;

      LP->R1 = sqrt(X0rD*X0rD+LP->Y0*LP->Y0);
      LP->R2 = sqrt(X0r1D*X0r1D+LP->Y0*LP->Y0);
      R13 = 1.0/(LP->R1*LP->R1*LP->R1);
      R15 = R13/(LP->R1*LP->R1);
      R23 = 1.0/(LP->R2*LP->R2*LP->R2);
      R25 = R23/(LP->R2*LP->R2);

      LP->Kxx = MuSum*(rho1*(R13-3.0*R15*X0rD*X0rD)+rho*(R23-3.0*R25*X0r1D*X0r1D));
      LP->Kxy = 0.0;
      LP->Kyy = MuSum*(rho1*(R13-3.0*R15*LP->Y0*LP->Y0)+rho*(R23-3.0*R25*LP->Y0*LP->Y0));

      a = 1.0;
      b = LP->Kxx+LP->Kyy+2.0*n*n;
      c = (LP->Kxx-n*n)*(LP->Kyy-n*n)-LP->Kxy*LP->Kxy;
      s2 = (-b - sqrt(b*b-4*a*c))/(2.0*a);
      LP->w1 = sqrt(-s2);
      LP->w2 = 0.0;
      s2 = (-b + sqrt(b*b-4*a*c))/(2.0*a);
      LP->sigma = sqrt(s2);
      LP->wz = sqrt(MuSum*(rho1*R13+rho*R23));

      LP->Zw1 = LP->Kyy-n*n-LP->w1*LP->w1;
      LP->Zw2 = 0.0;
      LP->Zs = LP->Kyy-n*n-LP->sigma*LP->sigma;

      LP->ca1 = 1.0;
      LP->sa1 = 0.0;
      LP->ca2 = 1.0;
      LP->sa2 = 0.0;

      LP->AR1 = -(LP->Zw1)/(2.0*n*LP->w1);
      LP->AR2 = 0.0;

/* .. L3 */
      LP = &LS->LP[2];
      x = 1.0;
      do {
         xp = x-rho;
         xp1 = xp + 1.0;
         f = x - rho1/(xp*xp) - rho/(xp1*xp1);
         fx = 1.0 + 2.0*rho1/(xp*xp*xp) + 2.0*rho/(xp1*xp1*xp1);
         dx = -f/fx;
         x += dx;
      } while(fabs(dx) > eps);
      LP->X0 = x*D;
      LP->Y0 = 0.0;

      X0rD = LP->X0 - rho*D;
      X0r1D = LP->X0 + rho1*D;

      LP->R1 = sqrt(X0rD*X0rD+LP->Y0*LP->Y0);
      LP->R2 = sqrt(X0r1D*X0r1D+LP->Y0*LP->Y0);
      R13 = 1.0/(LP->R1*LP->R1*LP->R1);
      R15 = R13/(LP->R1*LP->R1);
      R23 = 1.0/(LP->R2*LP->R2*LP->R2);
      R25 = R23/(LP->R2*LP->R2);

      LP->Kxx = MuSum*(rho1*(R13-3.0*R15*X0rD*X0rD)+rho*(R23-3.0*R25*X0r1D*X0r1D));
      LP->Kxy = 0.0;
      LP->Kyy = MuSum*(rho1*(R13-3.0*R15*LP->Y0*LP->Y0)+rho*(R23-3.0*R25*LP->Y0*LP->Y0));

      a = 1.0;
      b = LP->Kxx+LP->Kyy+2.0*n*n;
      c = (LP->Kxx-n*n)*(LP->Kyy-n*n)-LP->Kxy*LP->Kxy;
      s2 = (-b - sqrt(b*b-4*a*c))/(2.0*a);
      LP->w1 = sqrt(-s2);
      LP->w2 = 0.0;
      s2 = (-b + sqrt(b*b-4*a*c))/(2.0*a);
      LP->sigma = sqrt(s2);
      LP->wz = sqrt(MuSum*(rho1*R13+rho*R23));

      LP->Zw1 = LP->Kyy-n*n-LP->w1*LP->w1;
      LP->Zw2 = 0.0;
      LP->Zs = LP->Kyy-n*n-LP->sigma*LP->sigma;

      LP->ca1 = 1.0;
      LP->sa1 = 0.0;
      LP->ca2 = 1.0;
      LP->sa2 = 0.0;

      LP->AR1 = -(LP->Zw1)/(2.0*n*LP->w1);
      LP->AR2 = 0.0;

/* .. L4 */
      LP = &LS->LP[3];
      LP->X0 = -(0.5-rho)*D;
      LP->Y0 = 0.5*sqrt(3.0)*D;

      X0rD = LP->X0 - rho*D;
      X0r1D = LP->X0 + rho1*D;

      LP->R1 = sqrt(X0rD*X0rD+LP->Y0*LP->Y0);
      LP->R2 = sqrt(X0r1D*X0r1D+LP->Y0*LP->Y0);
      R13 = 1.0/(LP->R1*LP->R1*LP->R1);
      R15 = R13/(LP->R1*LP->R1);
      R23 = 1.0/(LP->R2*LP->R2*LP->R2);
      R25 = R23/(LP->R2*LP->R2);

      LP->Kxx = MuSum*(rho1*(R13-3.0*R15*X0rD*X0rD)+rho*(R23-3.0*R25*X0r1D*X0r1D));
      LP->Kxy = 3.0*MuSum*LP->Y0*(rho1*R15*X0rD+rho*R25*X0r1D);
      LP->Kyy = MuSum*(rho1*(R13-3.0*R15*LP->Y0*LP->Y0)+rho*(R23-3.0*R25*LP->Y0*LP->Y0));

      a = 1.0;
      b = LP->Kxx+LP->Kyy+2.0*n*n;
      c = (LP->Kxx-n*n)*(LP->Kyy-n*n)-LP->Kxy*LP->Kxy;
      s2 = (-b - sqrt(b*b-4*a*c))/(2.0*a);
      LP->w1 = sqrt(-s2);
      s2 = (-b + sqrt(b*b-4*a*c))/(2.0*a);
      LP->w2 = sqrt(-s2);
      LP->sigma = 0.0;
      LP->wz = sqrt(MuSum*(rho1*R13+rho*R23));

      LP->Zw1 = LP->Kyy-n*n-LP->w1*LP->w1;
      LP->Zw2 = LP->Kyy-n*n-LP->w2*LP->w2;
      LP->Zs = 0.0;

      alpha = 0.5*atan2(-2.0*LP->Kxy*LP->Zw1,
         -(LP->Zw1*LP->Zw1-LP->Kxy*LP->Kxy-4.0*n*n*LP->w1*LP->w1));
      LP->ca1 = cos(alpha);
      LP->sa1 = sin(alpha);
      alpha = 0.5*atan2(-2.0*LP->Kxy*LP->Zw2,
         -(LP->Zw2*LP->Zw2-LP->Kxy*LP->Kxy-4.0*n*n*LP->w2*LP->w2));
      LP->ca2 = cos(alpha);
      LP->sa2 = sin(alpha);

      LP->AR1 = -(LP->Zw1*LP->ca1+LP->Kxy*LP->sa1)/(2.0*n*LP->w1*LP->ca1);
      LP->AR2 = -(LP->Zw2*LP->ca2+LP->Kxy*LP->sa2)/(2.0*n*LP->w2*LP->ca2);

/* .. L5 */
      LP = &LS->LP[4];
      LP->X0 = -(0.5-rho)*D;
      LP->Y0 = -0.5*sqrt(3.0)*D;

      X0rD = LP->X0 - rho*D;
      X0r1D = LP->X0 + rho1*D;

      LP->R1 = sqrt(X0rD*X0rD+LP->Y0*LP->Y0);
      LP->R2 = sqrt(X0r1D*X0r1D+LP->Y0*LP->Y0);
      R13 = 1.0/(LP->R1*LP->R1*LP->R1);
      R15 = R13/(LP->R1*LP->R1);
      R23 = 1.0/(LP->R2*LP->R2*LP->R2);
      R25 = R23/(LP->R2*LP->R2);

      LP->Kxx = MuSum*(rho1*(R13-3.0*R15*X0rD*X0rD)+rho*(R23-3.0*R25*X0r1D*X0r1D));
      LP->Kxy = 3.0*MuSum*LP->Y0*(rho1*R15*X0rD+rho*R25*X0r1D);
      LP->Kyy = MuSum*(rho1*(R13-3.0*R15*LP->Y0*LP->Y0)+rho*(R23-3.0*R25*LP->Y0*LP->Y0));

      a = 1.0;
      b = LP->Kxx+LP->Kyy+2.0*n*n;
      c = (LP->Kxx-n*n)*(LP->Kyy-n*n)-LP->Kxy*LP->Kxy;
      s2 = (-b - sqrt(b*b-4*a*c))/(2.0*a);
      LP->w1 = sqrt(-s2);
      s2 = (-b + sqrt(b*b-4*a*c))/(2.0*a);
      LP->w2 = sqrt(-s2);
      LP->sigma = 0.0;
      LP->wz = sqrt(MuSum*(rho1*R13+rho*R23));

      LP->Zw1 = LP->Kyy-n*n-LP->w1*LP->w1;
      LP->Zw2 = LP->Kyy-n*n-LP->w2*LP->w2;
      LP->Zs = 0.0;

      alpha = 0.5*atan2(-2.0*LP->Kxy*LP->Zw1,
         -(LP->Zw1*LP->Zw1-LP->Kxy*LP->Kxy-4.0*n*n*LP->w1*LP->w1));
      LP->ca1 = cos(alpha);
      LP->sa1 = sin(alpha);
      alpha = 0.5*atan2(-2.0*LP->Kxy*LP->Zw2,
         -(LP->Zw2*LP->Zw2-LP->Kxy*LP->Kxy-4.0*n*n*LP->w2*LP->w2));
      LP->ca2 = cos(alpha);
      LP->sa2 = sin(alpha);

      LP->AR1 = -(LP->Zw1*LP->ca1+LP->Kxy*LP->sa1)/(2.0*n*LP->w1*LP->ca1);
      LP->AR2 = -(LP->Zw2*LP->ca2+LP->Kxy*LP->sa2)/(2.0*n*LP->w2*LP->ca2);

}
/**********************************************************************/
/*  Find the instantaneous locations of Lagrange Points, assuming the */
/*  elliptic restricted three-body problem.                           */
/*  Reference Bong Wie, "Space Vehicle Dynamics and Control"          */
/*  (TL1050.W52)Sec 3.7.3                                             */
void FindLagPtPosVel(double SecSinceJ2000, struct LagrangeSystemType *S,
   long Ilp, double PosN[3], double VelN[3], double CLN[3][3])
{

      double OnePlusEcosTH,OneMinusE2,R2[3],V2[3],L1[3],L2[3],L3[3];
      double rp[3],rhat[3],magr,thhat[3],vp[3];
      double sth,cth;
      long i;

      Eph2RV(S->mu1,S->SLR,S->ecc,S->inc,S->RAAN,S->ArgP,
         SecSinceJ2000-S->tp,R2,V2,&S->th);

      sth = sin(S->th);
      cth = cos(S->th);

      /* Find D, Ddot, Ddotdot, thdot, thdotdot */
      OnePlusEcosTH = 1.0+S->ecc*cth;
      OneMinusE2 = 1.0-S->ecc*S->ecc;
      S->D = OneMinusE2/OnePlusEcosTH;
      S->Ddot = S->ecc/sqrt(OneMinusE2)*sth;
      S->Ddotdot = S->ecc/OneMinusE2*cth*OnePlusEcosTH;
      S->thdot = OnePlusEcosTH*OnePlusEcosTH/sqrt(OneMinusE2);
      S->thdotdot = -2.0*S->ecc*sth/OneMinusE2*
                     OnePlusEcosTH*OnePlusEcosTH;

      /* L is rotating ("synodic") frame */
      /* L1 points from Body 2 to Body 1 */
      /* L2 is in plane of rotation */
      for(i=0;i<3;i++) {
         L1[i] = -R2[i];
         L2[i] = -V2[i];
      }
      UNITV(L1);
      UNITV(L2);
      VxV(L1,L2,L3);
      UNITV(L3);
      VxV(L3,L1,L2);
      UNITV(L2);
      for(i=0;i<3;i++) {
         CLN[0][i] = L1[i];
         CLN[1][i] = L2[i];
         CLN[2][i] = L3[i];
      }

      rp[0] = S->LP[Ilp].X0*S->D;
      rp[1] = S->LP[Ilp].Y0*S->D;
      rp[2] = 0.0;
      magr = CopyUnitV(rp,rhat);
      thhat[0] = -rhat[1];
      thhat[1] =  rhat[0];
      thhat[2] = 0.0;
      vp[0] = (S->Ddot*S->SMA*rhat[0]
            + magr*S->thdot*thhat[0])*S->MeanRate;
      vp[1] = (S->Ddot*S->SMA*rhat[1]
            + magr*S->thdot*thhat[1])*S->MeanRate;
      vp[2] = 0.0;
      MTxV(CLN,rp,PosN);
      MTxV(CLN,vp,VelN);
}
/**********************************************************************/
/*  From Lagrange System "modal" description, find position, velocity */
/*  (m, m/sec) wrt N frame of LagSys Body 1                           */
void LagModes2RV(double SecSinceJ2000, struct LagrangeSystemType *LS,
                 struct OrbitType *O, double r[3], double v[3])
{
      struct LagrangePointType *LP;
      double cw1t,sw1t,cw2t,sw2t,ep,em,cwzt,swzt;
      double TimeSinceEpoch;
      double rl[3], vl[3];
      long i;

      LP = &LS->LP[O->LP];

      TimeSinceEpoch = SecSinceJ2000 - O->Epoch;

      cw1t = cos(LP->w1*TimeSinceEpoch);
      sw1t = sin(LP->w1*TimeSinceEpoch);
      if (O->LP < 3) {  /* Collinear LP */
         ep = exp(LP->sigma*TimeSinceEpoch);
         em = exp(-LP->sigma*TimeSinceEpoch);
         rl[0] = O->Ax*cw1t + O->Bx*sw1t + O->Cx*ep + O->Dx*em;
         rl[1] = O->Ay*cw1t + O->By*sw1t + O->Cy*ep + O->Dy*em;
         vl[0] = LP->w1*(-O->Ax*sw1t+O->Bx*cw1t)
                +LP->sigma*(O->Cx*ep-O->Dx*em);
         vl[1] = LP->w1*(-O->Ay*sw1t+O->By*cw1t)
                +LP->sigma*(O->Cy*ep-O->Dy*em);
      }
      else {  /* Triangular LP */
         cw2t = cos(LP->w2*TimeSinceEpoch);
         sw2t = sin(LP->w2*TimeSinceEpoch);
         rl[0] = O->Ax*cw1t + O->Bx*sw1t + O->Cx*cw2t + O->Dx*sw2t;
         rl[1] = O->Ay*cw1t + O->By*sw1t + O->Cy*cw2t + O->Dy*sw2t;
         vl[0] = LP->w1*(-O->Ax*sw1t+O->Bx*cw1t)
                +LP->w2*(-O->Cx*sw1t+O->Dx*cw1t);
         vl[1] = LP->w1*(-O->Ay*sw1t+O->By*cw1t)
                +LP->w2*(-O->Cy*sw1t+O->Dy*cw1t);
      }
      cwzt = cos(LP->wz*TimeSinceEpoch);
      swzt = sin(LP->wz*TimeSinceEpoch);
      rl[2] = O->Az*cwzt + O->Bz*swzt;
      vl[2] = LP->wz*(-O->Az*swzt+O->Bz*cwzt);

      /* Do we need to keep x,y,z,xdot,ydot,zdot? */
      O->x = rl[0];
      O->y = rl[1];
      O->z = rl[2];
      O->xdot = vl[0];
      O->ydot = vl[1];
      O->zdot = vl[2];

      VxM(rl,LS->CLN,r);
      VxM(vl,LS->CLN,v);
      for(i=0;i<3;i++) r[i] += LP->PosN[i];

      /* Add velocity of rotating frame, at r, not at LP */
      v[0] -= LS->MeanRate*r[1];
      v[1] += LS->MeanRate*r[0];
}
/**********************************************************************/
/*  From position, velocity (m, m/sec) wrt N frame of LagSys Body 1,  */
/*  find Lagrange System "modal" description                          */
void RV2LagModes(double SecSinceJ2000, struct LagrangeSystemType *LS,
                 struct OrbitType *O)
{
      struct LagrangePointType *LP;
      double TimeSinceEpoch;
      double R,Rmin;
      double cw1t,sw1t,cw2t,sw2t,ep,em,cwzt,swzt;
      double rn[3],rl[3],vn[3],vl[3];
      double wvec[3],wxr[3];
      double LpPosN[3],LpVelN[3],CLN[3][3];
      double **COEF,*RHS,*ParmVec;
      long i,j;

      COEF = CreateMatrix(8,8);
      RHS = (double *) calloc(8,sizeof(double));
      ParmVec = (double *) calloc(8,sizeof(double));

      /* Find which LP is closest.  Assume motion is about that LP */
      O->LP = 0;
      Rmin = 1.0E15; /* Absurdly large */
      for(i=0;i<5;i++) {
         FindLagPtPosVel(SecSinceJ2000,LS,i,LpPosN,LpVelN,CLN);
         for(j=0;j<3;j++) rn[j] = O->PosN[j] - LpPosN[j];
         R = MAGV(rn);
         if (R < Rmin) {
            O->LP = i;
            Rmin = R;
         }
      }

      LP = &LS->LP[O->LP];
      FindLagPtPosVel(SecSinceJ2000,LS,O->LP,LpPosN,LpVelN,CLN);

      TimeSinceEpoch = SecSinceJ2000 - O->Epoch;

      cw1t = cos(LP->w1*TimeSinceEpoch);
      sw1t = sin(LP->w1*TimeSinceEpoch);
      cw2t = cos(LP->w2*TimeSinceEpoch);
      sw2t = sin(LP->w2*TimeSinceEpoch);

      if (O->LP < 3) { /* Collinear LPs */
         ep = exp(LP->sigma*TimeSinceEpoch);
         em = exp(-LP->sigma*TimeSinceEpoch);
         COEF[0][0] = cw1t;
         COEF[0][1] = sw1t;
         COEF[0][2] = ep;
         COEF[0][3] = em;
         COEF[1][4] = cw1t;
         COEF[1][5] = sw1t;
         COEF[1][6] = ep;
         COEF[1][7] = em;
         COEF[2][0] = -LP->w1*sw1t;
         COEF[2][1] =  LP->w1*cw1t;
         COEF[2][2] =  LP->sigma*ep;
         COEF[2][3] = -LP->sigma*em;
         COEF[3][4] = -LP->w1*sw1t;
         COEF[3][5] =  LP->w1*cw1t;
         COEF[3][6] =  LP->sigma*ep;
         COEF[3][7] = -LP->sigma*em;
         COEF[4][1] =  2.0*LS->MeanRate*LP->w1;
         COEF[5][0] = -2.0*LS->MeanRate*LP->w1;
         COEF[6][2] =  2.0*LS->MeanRate*LP->sigma;
         COEF[7][3] = -2.0*LS->MeanRate*LP->sigma;
         COEF[4][4] = LP->Zw1;
         COEF[5][5] = LP->Zw1;
         COEF[6][6] = LP->Zs;
         COEF[7][7] = LP->Zs;
      }
      else {
         COEF[0][0] = cw1t;
         COEF[0][1] = sw1t;
         COEF[0][2] = cw2t;
         COEF[0][3] = sw2t;
         COEF[1][4] = cw1t;
         COEF[1][5] = sw1t;
         COEF[1][6] = cw2t;
         COEF[1][7] = sw2t;
         COEF[2][0] = -LP->w1*sw1t;
         COEF[2][1] =  LP->w1*cw1t;
         COEF[2][2] = -LP->w2*sw2t;
         COEF[2][3] =  LP->w2*cw2t;
         COEF[3][4] = -LP->w1*sw1t;
         COEF[3][5] =  LP->w1*cw1t;
         COEF[3][6] = -LP->w2*sw2t;
         COEF[3][7] =  LP->w2*cw2t;
         COEF[4][0] = -LP->Kxy;
         COEF[4][1] =  2.0*LS->MeanRate*LP->w1;
         COEF[5][0] = -2.0*LS->MeanRate*LP->w1;
         COEF[5][1] = -LP->Kxy;
         COEF[6][2] = -LP->Kxy;
         COEF[6][3] =  2.0*LS->MeanRate*LP->w2;
         COEF[7][2] = -2.0*LS->MeanRate*LP->w2;
         COEF[7][3] = -LP->Kxy;
         COEF[4][4] = LP->Zw1;
         COEF[5][5] = LP->Zw1;
         COEF[6][6] = LP->Zw2;
         COEF[7][7] = LP->Zw2;
      }
      for(i=0;i<3;i++) rn[i] = O->PosN[i] - LpPosN[i];
      MxV(CLN,rn,rl);

      /* O's velocity wrt rotating frame, expressed in N */
      for(i=0;i<3;i++) wvec[i] = LS->MeanRate*CLN[2][i];
      VxV(wvec,O->PosN,wxr);
      for(i=0;i<3;i++) vn[i] = O->VelN[i] - wxr[i];
      /* Now transform to L frame */
      MxV(CLN,vn,vl);

      RHS[0] = rl[0];
      RHS[1] = rl[1];
      RHS[2] = vl[0];
      RHS[3] = vl[1];
      LINSOLVE(COEF,ParmVec,RHS,8);
      O->Ax = ParmVec[0];
      O->Bx = ParmVec[1];
      O->Cx = ParmVec[2];
      O->Dx = ParmVec[3];
      O->Ay = ParmVec[4];
      O->By = ParmVec[5];
      O->Cy = ParmVec[6];
      O->Dy = ParmVec[7];

      cwzt = cos(LP->wz*TimeSinceEpoch);
      swzt = sin(LP->wz*TimeSinceEpoch);

      O->Az = cwzt*rl[2] - swzt*vl[2]/LP->wz;
      O->Bz = swzt*rl[2] + cwzt*vl[2]/LP->wz;

      DestroyMatrix(COEF,8);
      free(RHS);
      free(ParmVec);
}
/**********************************************************************/
/*  From position (m) wrt N frame of LagSys Body 1,                   */
/*  find Lagrange System "modal" description, stable mode only        */
void R2StableLagMode(double SecSinceJ2000, struct LagrangeSystemType *LS,
                 struct OrbitType *O)
{
      struct LagrangePointType *LP;
      double TimeSinceEpoch;
      double R,Rmin;
      double cw1t,sw1t,cwzt,swzt;
      double rn[3],rl[3],vn[3],vl[3];
      double wvec[3],wxr[3];
      double LpPosN[3],LpVelN[3],CLN[3][3];
      double **COEF,*RHS,*ParmVec;
      long i,j;

      COEF = CreateMatrix(4,4);
      RHS = (double *) calloc(4,sizeof(double));
      ParmVec = (double *) calloc(4,sizeof(double));

      /* Find which LP is closest.  Assume motion is about that LP */
      O->LP = 0;
      Rmin = 1.0E15; /* Absurdly large */
      for(i=0;i<5;i++) {
         FindLagPtPosVel(SecSinceJ2000,LS,i,LpPosN,LpVelN,CLN);
         for(j=0;j<3;j++) rn[j] = O->PosN[j] - LpPosN[j];
         R = MAGV(rn);
         if (R < Rmin) {
            O->LP = i;
            Rmin = R;
         }
      }

      LP = &LS->LP[O->LP];
      FindLagPtPosVel(SecSinceJ2000,LS,O->LP,LpPosN,LpVelN,CLN);

      TimeSinceEpoch = SecSinceJ2000 - O->Epoch;

      cw1t = cos(LP->w1*TimeSinceEpoch);
      sw1t = sin(LP->w1*TimeSinceEpoch);

      if (O->LP < 3) { /* Collinear LPs */
         COEF[0][0] = cw1t;
         COEF[0][1] = sw1t;
         COEF[1][2] = cw1t;
         COEF[1][3] = sw1t;
         COEF[2][1] =  2.0*LS->MeanRate*LP->w1;
         COEF[3][0] = -2.0*LS->MeanRate*LP->w1;
         COEF[2][2] = LP->Zw1;
         COEF[3][3] = LP->Zw1;
      }
      else {
         COEF[0][0] = cw1t;
         COEF[0][1] = sw1t;
         COEF[1][2] = cw1t;
         COEF[1][3] = sw1t;
         COEF[2][0] = -LP->Kxy;
         COEF[2][1] =  2.0*LS->MeanRate*LP->w1;
         COEF[3][0] = -2.0*LS->MeanRate*LP->w1;
         COEF[3][1] = -LP->Kxy;
         COEF[2][2] = LP->Zw1;
         COEF[3][3] = LP->Zw1;
      }
      for(i=0;i<3;i++) rn[i] = O->PosN[i] - LpPosN[i];
      MxV(CLN,rn,rl);

      /* O's velocity wrt rotating frame, expressed in N */
      for(i=0;i<3;i++) wvec[i] = LS->MeanRate*CLN[2][i];
      VxV(wvec,O->PosN,wxr);
      for(i=0;i<3;i++) vn[i] = O->VelN[i] - wxr[i];
      /* Now transform to L frame */
      MxV(CLN,vn,vl);

      RHS[0] = rl[0];
      RHS[1] = rl[1];
      LINSOLVE(COEF,ParmVec,RHS,4);
      O->Ax = ParmVec[0];
      O->Bx = ParmVec[1];
      O->Cx = 0.0;
      O->Dx = 0.0;
      O->Ay = ParmVec[2];
      O->By = ParmVec[3];
      O->Cy = 0.0;
      O->Dy = 0.0;

      cwzt = cos(LP->wz*TimeSinceEpoch);
      swzt = sin(LP->wz*TimeSinceEpoch);

      O->Az = cwzt*rl[2] - swzt*vl[2]/LP->wz;
      O->Bz = swzt*rl[2] + cwzt*vl[2]/LP->wz;

      DestroyMatrix(COEF,4);
      free(RHS);
      free(ParmVec);
}
/**********************************************************************/
/*  From position, velocity (m, m/sec) wrt N frame of LagSys Body 1,  */
/*  find Lagrange System "modal" description                          */
void XYZ2LagModes(double TimeSinceEpoch, struct LagrangeSystemType *LS,
                 struct OrbitType *O)
{
      struct LagrangePointType *LP;
      double cw1t,sw1t,cw2t,sw2t,ep,em,cwzt,swzt;
      double **COEF,*RHS,*ParmVec;

      COEF = CreateMatrix(8,8);
      RHS = (double *) calloc(8,sizeof(double));
      ParmVec = (double *) calloc(8,sizeof(double));

      LP = &LS->LP[O->LP];

      cw1t = cos(LP->w1*TimeSinceEpoch);
      sw1t = sin(LP->w1*TimeSinceEpoch);
      cw2t = cos(LP->w2*TimeSinceEpoch);
      sw2t = sin(LP->w2*TimeSinceEpoch);

      if (O->LP < 3) { /* Collinear LPs */
         ep = exp(LP->sigma*TimeSinceEpoch);
         em = exp(-LP->sigma*TimeSinceEpoch);
         COEF[0][0] = cw1t;
         COEF[0][1] = sw1t;
         COEF[0][2] = ep;
         COEF[0][3] = em;
         COEF[1][4] = cw1t;
         COEF[1][5] = sw1t;
         COEF[1][6] = ep;
         COEF[1][7] = em;
         COEF[2][0] = -LP->w1*sw1t;
         COEF[2][1] =  LP->w1*cw1t;
         COEF[2][2] =  LP->sigma*ep;
         COEF[2][3] = -LP->sigma*em;
         COEF[3][4] = -LP->w1*sw1t;
         COEF[3][5] =  LP->w1*cw1t;
         COEF[3][6] =  LP->sigma*ep;
         COEF[3][7] = -LP->sigma*em;
         COEF[4][1] =  2.0*LS->MeanRate*LP->w1;
         COEF[5][0] = -2.0*LS->MeanRate*LP->w1;
         COEF[6][2] =  2.0*LS->MeanRate*LP->sigma;
         COEF[7][3] = -2.0*LS->MeanRate*LP->sigma;
         COEF[4][4] = LP->Zw1;
         COEF[5][5] = LP->Zw1;
         COEF[6][6] = LP->Zs;
         COEF[7][7] = LP->Zs;
      }
      else {
         COEF[0][0] = cw1t;
         COEF[0][1] = sw1t;
         COEF[0][2] = cw2t;
         COEF[0][3] = sw2t;
         COEF[1][4] = cw1t;
         COEF[1][5] = sw1t;
         COEF[1][6] = cw2t;
         COEF[1][7] = sw2t;
         COEF[2][0] = -LP->w1*sw1t;
         COEF[2][1] =  LP->w1*cw1t;
         COEF[2][2] = -LP->w2*sw2t;
         COEF[2][3] =  LP->w2*cw2t;
         COEF[3][4] = -LP->w1*sw1t;
         COEF[3][5] =  LP->w1*cw1t;
         COEF[3][6] = -LP->w2*sw2t;
         COEF[3][7] =  LP->w2*cw2t;
         COEF[4][0] = -LP->Kxy;
         COEF[4][1] =  2.0*LS->MeanRate*LP->w1;
         COEF[5][0] = -2.0*LS->MeanRate*LP->w1;
         COEF[5][1] = -LP->Kxy;
         COEF[6][2] = -LP->Kxy;
         COEF[6][3] =  2.0*LS->MeanRate*LP->w2;
         COEF[7][2] = -2.0*LS->MeanRate*LP->w2;
         COEF[7][3] = -LP->Kxy;
         COEF[4][4] = LP->Zw1;
         COEF[5][5] = LP->Zw1;
         COEF[6][6] = LP->Zw2;
         COEF[7][7] = LP->Zw2;
      }


      RHS[0] = O->x;
      RHS[1] = O->y;
      RHS[2] = O->xdot;
      RHS[3] = O->ydot;
      LINSOLVE(COEF,ParmVec,RHS,8);
      O->Ax = ParmVec[0];
      O->Bx = ParmVec[1];
      O->Cx = ParmVec[2];
      O->Dx = ParmVec[3];
      O->Ay = ParmVec[4];
      O->By = ParmVec[5];
      O->Cy = ParmVec[6];
      O->Dy = ParmVec[7];

      cwzt = cos(LP->wz*TimeSinceEpoch);
      swzt = sin(LP->wz*TimeSinceEpoch);

      O->Az = cwzt*O->z - swzt*O->zdot/LP->wz;
      O->Bz = swzt*O->z + cwzt*O->zdot/LP->wz;

      DestroyMatrix(COEF,8);
      free(RHS);
      free(ParmVec);
}
/**********************************************************************/
void AmpPhase2LagModes(double TimeSinceEpoch, double AmpXY1,
   double PhiXY1, double SenseXY1, double AmpXY2, double PhiXY2,
   double SenseXY2, double AmpZ, double PhiZ,
   struct LagrangeSystemType *S, struct OrbitType *O)
{
      double A,B,ca,sa,cphi,sphi;
      struct LagrangePointType *LP;

      LP = &S->LP[O->LP];

      PhiXY1 -= LP->w1*TimeSinceEpoch;
      PhiXY2 -= LP->w2*TimeSinceEpoch;
      PhiZ   -= LP->wz*TimeSinceEpoch;

      if (SenseXY1 < 0.0) PhiXY1 = -PhiXY1;
      if (SenseXY2 < 0.0) PhiXY2 = -PhiXY2;

      if (O->LP < 3) { /* Collinear, only one stable mode */
         A = AmpXY1;
         B = SenseXY1*LP->AR1*A;
         sphi = sin(PhiXY1);
         cphi = cos(PhiXY1);
         O->Ax = -B*sphi;
         O->Bx = -B*cphi;
         O->Ay =  A*cphi;
         O->By = -A*sphi;
         O->Cx = 0.0;
         O->Dx = 0.0;
         O->Cy = 0.0;
         O->Dy = 0.0;
      }
      else { /* Triangular, two stable modes */
         A = AmpXY1;
         B = SenseXY1*LP->AR1*A;
         sa = LP->sa1;
         ca = LP->ca1;
         sphi = sin(PhiXY1);
         cphi = cos(PhiXY1);
         O->Ax = -A*sa*cphi-B*sa*sphi;
         O->Bx =  A*sa*sphi-B*ca*cphi;
         O->Ay =  A*ca*cphi-B*sa*sphi;
         O->By = -A*ca*sphi-B*sa*cphi;

         A = AmpXY2;
         B = SenseXY2*LP->AR2*A;
         sa = LP->sa2;
         ca = LP->ca2;
         sphi = sin(PhiXY2);
         cphi = cos(PhiXY2);
         O->Cx = -A*sa*cphi-B*sa*sphi;
         O->Dx =  A*sa*sphi-B*ca*cphi;
         O->Cy =  A*ca*cphi-B*sa*sphi;
         O->Dy = -A*ca*sphi-B*sa*cphi;
      }

      O->Az = AmpZ*cos(PhiZ);
      O->Bz = AmpZ*sin(PhiZ);
}
/**********************************************************************/
/*   Notional position and velocities for TDRS satellites             */
/*   Note that TDRS[1] (TDRS-2) was lost at launch                    */
void TDRSPosVel(double PriMerAng,double time,
                double ptn[10][3], double vtn[10][3])
{
#define D2R (1.74532925199E-2)

      double Lng[10] = {-49.0,0.0,-275.0,-46.0,-171.4,
                        -173.7,-150.0,-271.0,-62.4,-40.9}; /* deg */
      double a[10] = {42.241E6,42.241E6,42.241E6,42.241E6,42.241E6,
                      42.241E6,42.241E6,42.241E6,42.241E6,42.241E6}; /* m */
      double e[10] = {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};
      double i[10] = {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0}; /* deg */
      double LANdrift[10] = {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0}; /* r/s */
      double omdrift[10] = {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};  /* r/s */
      double anom,Li,LAN,om;
      double S1,C1,C2,S3,C3;
      long j;

      static double p[10];
      static double LAN0[10] = {0.0,36.0,72.0,108.0,144.0,
                                180.0,216.0,252.0,288.0,324.0}; /* deg */
      static double om0[10];
      static long First = 1;

      if (First) {
         First = 0;
         for(j=0;j<10;j++) {
            p[j] = a[j]*(1.0-e[j]*e[j]);

            Li = Lng[j]*D2R+PriMerAng;
            S1=sin(LAN0[j]*D2R);
            C1=cos(LAN0[j]*D2R);
            C2=cos(i[j]*D2R);
            S3=sin(Li);
            C3=cos(Li);
            om0[j] = atan2(-(C1*S3-S1*C3),(-S1*C2*S3-C1*C2*C3));
         }
      }

      for(j=0;j<10;j++) {
         om = om0[j] + omdrift[j]*time;
         LAN = LAN0[j]*D2R + LANdrift[j]*time;

         Eph2RV(3.986004E14,p[j],e[j],i[j]*D2R,LAN,
            om,time,ptn[j],vtn[j],&anom);
      }
#undef D2R
}
/**********************************************************************/
/* Find coordinate transformation from True Equator True Equinox      */
/* (TETE) frame to J2000 frame.  Ref "The Astronomical Almanac",      */
/* QB8.U5, 2003, p. B18,B20.                                          */
void TETE2J2000(double JD,double CTJ[3][3])
{
#define D2R (1.74532925199E-2)

      double d,arg1,arg2,dpsi,deps,eps,CTM[3][3];
      double T,z,theta,zeta,CMJ[3][3];
      double c1,s1,c2,s2,c3,s3;

      /* TETE to MEME */
      d = JD - 2452639.5;
      arg1 = (67.1 - 0.053*d)*D2R;
      arg2 = (198.5 + 1.971*d)*D2R;
      dpsi = (-0.0048*sin(arg1)-0.0004*sin(arg2))*D2R;
      deps = (0.0026*cos(arg1)+0.0002*cos(arg2))*D2R;
      eps = 23.44*D2R;

      CTM[0][0] = 1.0;
      CTM[1][1] = 1.0;
      CTM[2][2] = 1.0;
      CTM[2][1] = deps;
      CTM[2][0] = dpsi*sin(eps);
      CTM[1][0] = dpsi*cos(eps);
      CTM[1][2] = -CTM[2][1];
      CTM[0][2] = -CTM[2][0];
      CTM[0][1] = -CTM[1][0];

      /* MEME to J2000 */
      T = (JD - 2451545.0)/36525.0;
      z =     D2R*((0.6406161+(3.041E-4+5.10E-6*T)*T)*T);
      theta = D2R*((0.5567530-(1.185E-4+1.16E-5*T)*T)*T);
      zeta =  D2R*((0.6406161+(8.390E-5+5.00E-6*T)*T)*T);

      c1=cos(-zeta);
      s1=sin(-zeta);
      c2=cos(theta);
      s2=sin(theta);
      c3=cos(-z);
      s3=sin(-z);
      CMJ[0][0] =  c1*c2*c3-s3*s1;
      CMJ[1][0] = -c1*c2*s3-c3*s1;
      CMJ[2][0] =  c1*s2;
      CMJ[0][1] =  s1*c2*c3+s3*c1;
      CMJ[1][1] = -s1*c2*s3+c3*c1;
      CMJ[2][1] =  s1*s2;
      CMJ[0][2] = -s2*c3;
      CMJ[1][2] =  s2*s3;
      CMJ[2][2] =  c2;

      /* Concatenate */
      CTJ[0][0] = CTM[0][0]*CMJ[0][0]+CTM[0][1]*CMJ[1][0]+CTM[0][2]*CMJ[2][0];
      CTJ[0][1] = CTM[0][0]*CMJ[0][1]+CTM[0][1]*CMJ[1][1]+CTM[0][2]*CMJ[2][1];
      CTJ[0][2] = CTM[0][0]*CMJ[0][2]+CTM[0][1]*CMJ[1][2]+CTM[0][2]*CMJ[2][2];
      CTJ[1][0] = CTM[1][0]*CMJ[0][0]+CTM[1][1]*CMJ[1][0]+CTM[1][2]*CMJ[2][0];
      CTJ[1][1] = CTM[1][0]*CMJ[0][1]+CTM[1][1]*CMJ[1][1]+CTM[1][2]*CMJ[2][1];
      CTJ[1][2] = CTM[1][0]*CMJ[0][2]+CTM[1][1]*CMJ[1][2]+CTM[1][2]*CMJ[2][2];
      CTJ[2][0] = CTM[2][0]*CMJ[0][0]+CTM[2][1]*CMJ[1][0]+CTM[2][2]*CMJ[2][0];
      CTJ[2][1] = CTM[2][0]*CMJ[0][1]+CTM[2][1]*CMJ[1][1]+CTM[2][2]*CMJ[2][1];
      CTJ[2][2] = CTM[2][0]*CMJ[0][2]+CTM[2][1]*CMJ[1][2]+CTM[2][2]*CMJ[2][2];

#undef D2R
}
/**********************************************************************/
/*  See Battin                                                        */
double RadiusOfInfluence(double mu1, double mu2, double r)
{
      return(r*pow(mu2/mu1,0.4));
}
/**********************************************************************/
/*  Given Rrel and Vrel, find the Euler-Hill state vector [re, ve]    */
/*  E-H usually assumes small departures from LVLH.  I'm using        */
/*  spherical coordinates here to ensure valid solution anywhere.     */
void RelRV2EHRV(double OrbRadius, double OrbRate, double OrbCLN[3][3],
             double Rrel[3], double Vrel[3], double re[3], double ve[3])
{
      double p[3],magp,b3[3],alpha,beta,CBL[3][3],CBN[3][3],vn[3],vb[3];
      double C1,S1,C2,S2;
      long i;

      for(i=0;i<3;i++) p[i] = Rrel[i] - OrbRadius*OrbCLN[2][i];
      magp = CopyUnitV(p,b3);
      for(i=0;i<3;i++) b3[i] *= -1.0;
      alpha = atan2(-VoV(OrbCLN[0],b3),VoV(OrbCLN[2],b3));
      beta = asin(-VoV(b3,OrbCLN[1]));

      re[0] = OrbRadius*alpha;
      re[1] = OrbRadius*beta;
      re[2] = OrbRadius-magp;

      C1 = cos(alpha);
      S1 = -sin(alpha);
      C2 = cos(beta);
      S2 = sin(beta);
      CBL[0][0]=C1;
      CBL[1][0]=S1*S2;
      CBL[2][0]=S1*C2;
      CBL[0][1]=0.0;
      CBL[1][1]=C2;
      CBL[2][1]=-S2;
      CBL[0][2]=-S1;
      CBL[1][2]=C1*S2;
      CBL[2][2]=C1*C2;
      MxM(CBL,OrbCLN,CBN);
      for(i=0;i<3;i++) vn[i] = Vrel[i] + OrbRadius*OrbRate*OrbCLN[0][i];
      MxV(CBN,vn,vb);
      ve[0] = vb[0] - OrbRate*magp;
      ve[1] = vb[1];
      ve[2] = vb[2];

}
/**********************************************************************/
/*  Given a circular reference, and the Euler-Hill state vector       */
/*  find the relative position and velocity (expressed in N)          */
/*  E-H usually assumes small departures from LVLH.  I'm using        */
/*  spherical coordinates here to ensure valid solution anywhere.     */
void EHRV2RelRV(double OrbRadius, double OrbRate, double OrbCLN[3][3],
   double re[3], double ve[3], double Rrel[3], double Vrel[3])
{
      double alpha,beta,magp,CBL[3][3],CBN[3][3],vb[3],vn[3];
      double C1,S1,C2,S2;
      long i;

      alpha = re[0]/OrbRadius;
      beta = re[1]/OrbRadius;
      magp = OrbRadius-re[2];

      C1 = cos(alpha);
      S1 = -sin(alpha);
      C2 = cos(beta);
      S2 = sin(beta);
      CBL[0][0]=C1;
      CBL[1][0]=S1*S2;
      CBL[2][0]=S1*C2;
      CBL[0][1]=0.0;
      CBL[1][1]=C2;
      CBL[2][1]=-S2;
      CBL[0][2]=-S1;
      CBL[1][2]=C1*S2;
      CBL[2][2]=C1*C2;
      MxM(CBL,OrbCLN,CBN);
      for(i=0;i<3;i++)
         Rrel[i] = OrbRadius*OrbCLN[2][i] - magp*CBN[2][i];

      vb[0] = ve[0] + OrbRate*magp;
      vb[1] = ve[1];
      vb[2] = ve[2];
      MTxV(CBN,vb,vn);
      for(i=0;i<3;i++) Vrel[i] = vn[i] - OrbRadius*OrbRate*OrbCLN[0][i];
}
/**********************************************************************/
/*  Given Euler-Hill position and velocity, find parameters of        */
/*  Drift, Ellipse, Static, and Cross-Track modes                     */
void EHRV2EHModes(double r[3], double v[3], double n, double nt,
                  double *A, double *Bc, double *Bs, double *C,
                  double *Dc, double *Ds)
{
      double s,c,zuterm;

      s = sin(nt);
      c = cos(nt);
      zuterm = 2.0*v[0]/n - 3.0*r[2];

      /* Drift */
      *A =   4.0*r[2] - 2.0*v[0]/n;

      /* Ellipse */
      *Bc = zuterm*c - v[2]/n*s;
      *Bs = v[2]/n*c + zuterm*s;

      /* Static */
      *C = r[0] - 6.0*nt*r[2] + 3.0*nt*v[0]/n + 2.0*v[2]/n;

      /* Cross-Track */
      *Dc = v[1]/n*c + r[1]*s;
      *Ds = v[1]/n*s - r[1]*c;
}
/**********************************************************************/
void EHModes2EHRV(double A, double Bc, double Bs, double C,
                  double Dc, double Ds, double n, double nt,
                  double r[3], double v[3])
{
      double c,s,BCosTheta,BSinTheta,DCosTheta,DSinTheta;

      c = cos(nt);
      s = sin(nt);
      /* B*cos(nt-thp) */
      BCosTheta = Bc*c + Bs*s;
      /* B*sin(nt-thp) */
      BSinTheta = Bc*s - Bs*c;
      /* D*cos(nt-thn) */
      DCosTheta = Dc*c + Ds*s;
      /* D*sin(nt-thn) */
      DSinTheta = Dc*s - Ds*c;

      r[0] = 1.5*A*nt + 2.0*BSinTheta + C;
      r[1] = DSinTheta;
      r[2] = A + BCosTheta;
      v[0] = 1.5*A*n + 2.0*n*BCosTheta;
      v[1] = n*DCosTheta;
      v[2] = -n*BSinTheta;
}
/**********************************************************************/
/* See Battin 7.1                                                     */
double LambertTOF(double mu, double amin, double lambda, double x)
{
      double y,eta,S1,Q,T;
      double delta = 1.0;
      double u = 1.0;
      double Sigma = 1.0;
      long n = 1;
      double gamma;

      y = sqrt(1.0-lambda*lambda*(1.0-x*x));
      eta = y-lambda*x;
      S1 = 0.5*(1.0-lambda-x*eta);

      /* Q = HyperQ(S1) */
      while(fabs(u) > 1.0E-8 && n < 1000) {
         if (n%2) { /* n odd */
            gamma = ((double) ((n+2)*(n+5)))/((double) ((2*n+1)*(2*n+3)));
         }
         else { /* n even */
            gamma = ((double) (n*(n-3)))/((double) ((2*n+1)*(2*n+3)));
         }
         delta = 1.0/(1.0-gamma*delta*S1);
         u *= (delta - 1.0);
         Sigma += u;
         n++;
      }
      Q = 4.0/3.0*Sigma;

      T = (eta*eta*Q + 4.0*lambda)*eta*sqrt(amin*amin*amin/mu);
      return(T);
}
/**********************************************************************/
/*  See Battin 7.1                                                    */
/*  TransferType =  1.0 for Type I  (1H, 1A, 1B) transfers            */
/*  TransferType = -1.0 for Type II (2H, 2A, 2B) transfers            */
void LambertProblem(double t0, double mu, double xr1[3], double xr2[3],
   double TOF,double TransferType,double *SLR, double *e, double *inc,
   double *RAAN, double *ArgP, double *tp)
{
      double r1,r2,ir1[3],ir2[3],ih[3],th,dr[3],c,s,amin,lambda;
      double xold,Told,x,dx,T;
      double y,eta,ihxir1[3],xv1[3],Coef0,Coef1,Coef2;
      double a,anom,alpha,MeanMotion,Period,rmin;
      long i;

      r1 = CopyUnitV(xr1,ir1);
      r2 = CopyUnitV(xr2,ir2);
      VxV(xr1,xr2,ih);
      UNITV(ih);
      th = acos(VoV(ir1,ir2));
      for(i=0;i<3;i++) dr[i] = xr2[i]-xr1[i];
      c = MAGV(dr);
      s = 0.5*(r1+r2+c);
      amin = 0.5*s;
      lambda = TransferType*sqrt(r1*r2)*cos(0.5*th)/s;

      /* Secant Search for x */
      xold = 0.0;
      Told = LambertTOF(mu,amin,lambda,xold);
      x = 0.1;
      dx = x - xold;
      while(fabs(dx) > 1.0E-6) {
         T = LambertTOF(mu,amin,lambda,x);
         dx = (TOF-T)/(T-Told)*(x-xold);
         xold = x;
         Told = T;
         if (dx < -0.1) dx = -0.1;
         if (dx > 0.1) dx = 0.1;
         x += dx;
         if (x < -1.0) x = -1.0;
      }

      y = sqrt(1.0-lambda*lambda*(1.0-x*x));
      eta = y - lambda*x;
      VxV(ih,ir1,ihxir1);
      Coef0 = sqrt(mu/amin)/eta;
      Coef1 = (2.0*lambda*amin/r1 - (lambda + x*eta))*Coef0;
      Coef2 = sqrt(r2/r1)*sin(0.5*th)*Coef0;
      for(i=0;i<3;i++) xv1[i] = Coef1*ir1[i] + Coef2*TransferType*ihxir1[i];
      RV2Eph(t0,mu,xr1,xv1,&a,e,inc,RAAN,ArgP,&anom,tp,SLR,&alpha,&rmin,
         &MeanMotion,&Period);
}
/**********************************************************************/
double RendezvousCostFunction(double *InVec, double *AuxVec)
{
      double t0,TOF,mu,r1e[3],v1e[3],r2e[3],v2e[3];
      double tf,SMA,ecc,inc,RAAN,ArgP,anom,tp,p,alpha,MeanMotion,rmin;
      double r1[3],v1[3],r2[3],v2[3];
      double r1t[3],v1t[3],r2t[3],v2t[3];
      double DV1I[3],DV2I[3],DeltaVI;
      double DV1II[3],DV2II[3],DeltaVII;
      double DV1[3],DV2[3],DeltaV;
      double Per1,Per2;
      long i;

      t0 = InVec[0];
      TOF = InVec[1];

      mu = AuxVec[0];
      for(i=0;i<3;i++) {
         r1e[i] = AuxVec[1+i];
         v1e[i] = AuxVec[4+i];
         r2e[i] = AuxVec[7+i];
         v2e[i] = AuxVec[10+i];
      }

      tf = t0 + TOF;

/* .. Find r1(t0), r2(tf) */
      RV2Eph(0.0,mu,r1e,v1e,&SMA,&ecc,&inc,&RAAN,&ArgP,&anom,&tp,&p,&alpha,
         &rmin,&MeanMotion,&Per1);
      Eph2RV(mu,p,ecc,inc,RAAN,ArgP,t0-tp,r1,v1,&anom);

      RV2Eph(0.0,mu,r2e,v2e,&SMA,&ecc,&inc,&RAAN,&ArgP,&anom,&tp,&p,&alpha,
         &rmin,&MeanMotion,&Per2);
      Eph2RV(mu,p,ecc,inc,RAAN,ArgP,tf-tp,r2,v2,&anom);

/* .. Solve Lambert problem to find transfer orbit, transfer angle < Pi */
      LambertProblem(t0,mu,r1,r2,TOF,1.0,&p,&ecc,&inc,&RAAN,&ArgP,&tp);

/* .. Compute delta-V */
      Eph2RV(mu,p,ecc,inc,RAAN,ArgP,t0-tp,r1t,v1t,&anom);
      Eph2RV(mu,p,ecc,inc,RAAN,ArgP,tf-tp,r2t,v2t,&anom);

      for(i=0;i<3;i++) {
         DV1I[i] = v1t[i]-v1[i];
         DV2I[i] = v2[i]-v2t[i];
      }
      DeltaVI = MAGV(DV1I) + MAGV(DV2I);

/* .. Solve Lambert problem to find transfer orbit, transfer angle > Pi */
      LambertProblem(t0,mu,r1,r2,TOF,-1.0,&p,&ecc,&inc,&RAAN,&ArgP,&tp);

/* .. Compute delta-V */
      Eph2RV(mu,p,ecc,inc,RAAN,ArgP,t0-tp,r1t,v1t,&anom);
      Eph2RV(mu,p,ecc,inc,RAAN,ArgP,tf-tp,r2t,v2t,&anom);

      for(i=0;i<3;i++) {
         DV1II[i] = v1t[i]-v1[i];
         DV2II[i] = v2[i]-v2t[i];
      }
      DeltaVII = MAGV(DV1II) + MAGV(DV2II);

      if (DeltaVI < DeltaVII) {
         DeltaV = DeltaVI;
         for(i=0;i<3;i++) {
            DV1[i] = DV1I[i];
            DV2[i] = DV2I[i];
         }
      }
      else {
         DeltaV = DeltaVII;
         for(i=0;i<3;i++) {
            DV1[i] = DV1II[i];
            DV2[i] = DV2II[i];
         }
      }
      for(i=0;i<3;i++) {
         AuxVec[13+i] = DV1[i];
         AuxVec[16+i] = DV2[i];
      }

      /* Ramp penalties to constrain solution */
      if (t0 < 0.0) DeltaV += 10.0*(-t0/Per1);
      if (t0 > Per1) DeltaV += 10.0*((t0-Per1)/Per1);
      if (TOF < 0.0) DeltaV += 10.0*(-TOF/Per1);
      if (TOF > Per1+Per2) DeltaV += 10.0*((TOF-Per1-Per2)/(Per1+Per2));

      return(DeltaV);
}
/**********************************************************************/
/*  Given starting state (r1e, v1e) and target state (r2e, v2e) at    */
/*  epoch t=0, find times (t1, t2) and DVs (DV1, DV2) to perform      */
/*  rendezvous.                                                       */
void PlanTwoImpulseRendezvous(double mu, double r1e[3], double v1e[3],
   double r2e[3], double v2e[3], double *t1, double *t2,
   double DV1[3], double DV2[3])
{
      double SMA1,SMA2,ecc,inc,RAAN,ArgP,anom1,anom2,tp,p,alpha;
      double AmP[2],AmParm[19],DeltaV;
      double MeanMotion,Period,rmin;
      long i;

      RV2Eph(0.0,mu,r1e,v1e,&SMA1,&ecc,&inc,&RAAN,&ArgP,&anom1,&tp,&p,
         &alpha,&rmin,&MeanMotion,&Period);
      RV2Eph(0.0,mu,r2e,v2e,&SMA2,&ecc,&inc,&RAAN,&ArgP,&anom2,&tp,&p,
         &alpha,&rmin,&MeanMotion,&Period);
      AmP[0] = 0.0;
      AmP[1] = 3.1416*sqrt(sqrt(fabs(SMA1*SMA1*SMA1*SMA2*SMA2*SMA2))/mu);
      AmParm[0] = mu;
      for(i=0;i<3;i++) {
         AmParm[1+i] = r1e[i];
         AmParm[4+i] = v1e[i];
         AmParm[7+i] = r2e[i];
         AmParm[10+i] = v2e[i];
      }
      DeltaV = Amoeba(2,AmP,RendezvousCostFunction,AmParm,0.1*SMA1,1.0E-5);
      *t1 = AmP[0];
      *t2 = AmP[0]+AmP[1];
      for(i=0;i<3;i++) {
         DV1[i] = AmParm[13+i];
         DV2[i] = AmParm[16+i];
      }
      printf("t1 = %7.2lf   TOF = %7.2lf  DV = %7.2lf\n",*t1,*t2-*t1,DeltaV);
      printf("DV1: %7.2lf %7.2lf %7.2lf\n",DV1[0],DV1[1],DV1[2]);
      printf("DV2: %7.2lf %7.2lf %7.2lf\n",DV2[0],DV2[1],DV2[2]);
}
/**********************************************************************/
/*  Given the orbits of an Observer and a Target, find:               */
/*     PastPos: Location of Target when it sent the photons that      */
/*              are reaching the Observer at the present instant.     */
/*     FuturePos: Location of Target when the photons that are        */
/*                leaving the Observer at the present instant         */
/*                will arrive.                                        */
/*  Two iterations gives < mm accuracy for GEO-LEO distances.         */
/*  Will need more iterations for interplanetary-scale applications.  */
void FindLightLagOffsets(double DynTime, struct OrbitType *Observer,
   struct OrbitType *Target, double PastPos[3], double FuturePos[3])
{
#define SPEED_OF_LIGHT 299792458.0
      double Vel[3],anom;
      double RelPos[3],dt;
      long i;

/* .. Past */
      for(i=0;i<3;i++) RelPos[i] = Target->PosN[i] - Observer->PosN[i];
      dt = MAGV(RelPos)/SPEED_OF_LIGHT;
      Eph2RV(Target->mu,Target->SLR,Target->ecc,Target->inc,Target->RAAN,
         Target->ArgP,DynTime-dt-Target->tp,PastPos,Vel,&anom);

      for(i=0;i<3;i++) RelPos[i] = PastPos[i] - Observer->PosN[i];
      dt = MAGV(RelPos)/SPEED_OF_LIGHT;
      Eph2RV(Target->mu,Target->SLR,Target->ecc,Target->inc,Target->RAAN,
         Target->ArgP,DynTime-dt-Target->tp,PastPos,Vel,&anom);

/* .. Future */
      for(i=0;i<3;i++) RelPos[i] = Target->PosN[i] - Observer->PosN[i];
      dt = MAGV(RelPos)/SPEED_OF_LIGHT;
      Eph2RV(Target->mu,Target->SLR,Target->ecc,Target->inc,Target->RAAN,
         Target->ArgP,DynTime+dt-Target->tp,PastPos,Vel,&anom);

      for(i=0;i<3;i++) RelPos[i] = PastPos[i] - Observer->PosN[i];
      dt = MAGV(RelPos)/SPEED_OF_LIGHT;
      Eph2RV(Target->mu,Target->SLR,Target->ecc,Target->inc,Target->RAAN,
         Target->ArgP,DynTime+dt-Target->tp,PastPos,Vel,&anom);

#undef SPEED_OF_LIGHT
}
/**********************************************************************/
/* Ref: Markley and Crassidis, 10.4.3                                 */
/* Osculating elements drift from initial conditions due to J2        */
/* Use this function to initialize mean eph at sim start              */
void OscEphToMeanEph(double mu, double J2, double Rw, double DynTime,
   struct OrbitType *O)
{
      #define TWOPI (6.283185307179586)
      double e,e2,sin2i,sinw,sin2w,cosnu,g;
      double a,p,p2,Coef;
      
      sin2i = sin(O->inc)*sin(O->inc);
      
      /* 10.127 */
      e = O->ecc;
      e2 = e*e;
      sinw = sin(O->ArgP+O->anom);
      sin2w = sinw*sinw;
      cosnu = cos(O->anom);
      g = pow((1.0+e*cosnu)/(1.0-e2),3.0)*(1.0-3.0*sin2i*sin2w);
 
      /* 10.128 */
      a = O->SMA;
      O->MeanSMA = 0.5*(a + sqrt(a*a-4.0*J2*Rw*Rw*g));

      /* 10.123 */
      O->MeanMotion = sqrt(mu/O->MeanSMA)/O->MeanSMA;
      O->Period = TWOPI/O->MeanMotion;

      /* 10.121 */
      p = O->MeanSMA*(1.0-O->ecc*O->ecc);
      p2 = p*p;
      Coef = 1.5*J2*Rw*Rw/p2*O->MeanMotion;
      O->RAANdot = -Coef*cos(O->inc);
      O->ArgPdot =  Coef*(2.0-2.5*sin2i);

      O->RAAN0 = O->RAAN - O->RAANdot*(DynTime-O->Epoch);
      O->ArgP0 = O->ArgP - O->ArgPdot*(DynTime-O->Epoch);
      
      /* 10.126 */
      O->J2Rw2bya = J2*Rw*Rw/O->MeanSMA;      

      #undef TWOPI
}
/* #ifdef __cplusplus
** }
** #endif
*/

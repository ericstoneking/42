#include <stdlib.h>
#include <stdio.h>
#include <math.h>
/**********************************************************************/
/*  Vector Dot Product                                                */
double VoV(double A[3], double B[3])
{
      return(A[0]*B[0]+A[1]*B[1]+A[2]*B[2]);
}
/**********************************************************************/
/*  Vector Cross Product                                              */
void  VxV(double A[3], double B[3], double C[3])
{
      C[0]=A[1]*B[2]-A[2]*B[1];
      C[1]=A[2]*B[0]-A[0]*B[2];
      C[2]=A[0]*B[1]-A[1]*B[0];
}
/**********************************************************************/
/*  Magnitude of a 3-vector                                           */
double MAGV(double V[3])
{
      return(sqrt(V[0]*V[0]+V[1]*V[1]+V[2]*V[2]));
}
/**********************************************************************/
/*  Normalize a 3-vector.  Return its (pre-normalization) magnitude   */
double UNITV(double V[3])
{
      double A;

      A=sqrt(V[0]*V[0]+V[1]*V[1]+V[2]*V[2]);
      if (A > 0.0) {
         V[0]/=A;
         V[1]/=A;
         V[2]/=A;
      }
      else {
         printf("Attempted divide by zero in UNITV\n");
         V[0] = 0.0;
         V[1] = 0.0;
         V[2] = 0.0;
      }
      return(A);
}
/**********************************************************************/
/*  Copy and normalize a 3-vector.  Return its magnitude              */
double CopyUnitV(double V[3], double W[3])
{
      double A;

      A=sqrt(V[0]*V[0]+V[1]*V[1]+V[2]*V[2]);
      if (A > 0.0) {
         W[0] = V[0]/A;
         W[1] = V[1]/A;
         W[2] = V[2]/A;
      }
      else {
         printf("Attempted divide by zero in COPYUNITV\n");
         W[0] = 0.0;
         W[1] = 0.0;
         W[2] = 0.0;
      }
      return(A);
}
/**********************************************************************/
double TrueAnomaly(double mu, double p, double e, double t)
{
#define EPS 1.0E-12
#define PI    3.141592653589793
#define TWOPI 6.283185307179586

      double p3,B,x,f,fp,e1,N,H,M,E;
      double Ne,dx,dH,dE,Anom;
      short i;
      
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
         i = 0;
         e1 = 1.0-e*e;
         M = sqrt(mu*e1*e1*e1/p3)*t;
         M = fmod(M+PI, TWOPI) - PI;
         E = M;
         do {
            i++;
            f = E - e*sin(E) - M;
            fp = 1.0 - e*cos(E);
            dE = f/fp;
            if (dE >  0.1) dE =  0.1;
            if (dE < -0.1) dE = -0.1;
            E -= dE;
         } while (fabs(f) > EPS && fabs(dE) > EPS && i < 100);
         Anom = 2.0*atan(sqrt((1.0+e)/(1.0-e))*tan(0.5*E));
      }
      
      return(Anom);
#undef EPS
#undef PI
#undef TWOPI
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
/* Compute orbital elements, given position and velocity.  Works for  */
/* for all eccentricities.                                            */

void  RV2Eph(double time, double mu, double xr[3], double xv[3],
             double *SMA, double *e, double *i, double *RAAN,
             double *omg, double *th, double *tp, 
             double *p, double *alpha)
{
#define EPS 1.0E-12
#define PI    3.141592653589793
#define TWOPI 6.283185307179586

      double xh[3],xe[3],rhat[3],r,v,vxh[3],cth,hxe[3];
      double rohxe,h,dt;
      double xn[3],hxn[3],cosw,sinw;

      r=sqrt(xr[0]*xr[0]+xr[1]*xr[1]+xr[2]*xr[2]);
      v=sqrt(xv[0]*xv[0]+xv[1]*xv[1]+xv[2]*xv[2]);
      *alpha = 2.0*(mu/r-0.5*v*v)/mu;
      *SMA = 1.0/(*alpha);

      xh[0] = xr[1]*xv[2]-xr[2]*xv[1];
      xh[1] = xr[2]*xv[0]-xr[0]*xv[2];
      xh[2] = xr[0]*xv[1]-xr[1]*xv[0];

      *p = (xh[0]*xh[0]+xh[1]*xh[1]+xh[2]*xh[2])/mu;

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

      h=sqrt(xh[0]*xh[0]+xh[1]*xh[1]+xh[2]*xh[2]); /* 3D mag */
      xh[0] /= h;
      xh[1] /= h;
      xh[2] /= h;

      h = sqrt(xh[0]*xh[0]+xh[1]*xh[1]);  /* 2D mag */
      if (*e < EPS) {              /* Circular */
         if (h < EPS) {            /* Equatorial */
            /* Arbitrarily set RAAN, omg = 0 */
            *RAAN = 0.0;
            *i = (xh[2]>0.0 ? 0.0 : PI );
            *omg = 0.0;
            xe[0] = 1.0;
            xe[1] = 0.0;
            xe[2] = 0.0;
         }
         else {                     /* Inclined */
            /* Arbitrarily set omg = 0 */
            *RAAN = atan2(xh[0],-xh[1]);
            *i   =  acos(xh[2]);
            *omg = 0.0;
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
            *omg = atan2(sinw,cosw);
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
            *omg = atan2(sinw,cosw);
         }
      }

      cth = rhat[0]*xe[0]+rhat[1]*xe[1]+rhat[2]*xe[2];
      *th = acos(cth);

      hxe[0] = xh[1]*xe[2]-xh[2]*xe[1];
      hxe[1] = xh[2]*xe[0]-xh[0]*xe[2];
      hxe[2] = xh[0]*xe[1]-xh[1]*xe[0];

      rohxe = rhat[0]*hxe[0]+rhat[1]*hxe[1]+rhat[2]*hxe[2];
      if (rohxe < 0.0) *th = TWOPI - *th;

      dt = TimeSincePeriapsis(mu,*p,*e,*th);
      *tp = time - dt;
}
/**********************************************************************/
/* Compute position and velocity given orbital elements.  Works for   */
/* circular, elliptical, parabolic and hyperbolic orbits.             */

void Eph2RV(double mu, double p, double e, double i, double RAAN,
            double omg, double dt, double r[3], double v[3], 
            double *anom)
{
      double R,th,CPN[3][3],cth,sth,pr[3],pv[3],c2,a;
      double C1,S1,C2,S2,C3,S3;

//      if (e > 1.0) {
//         a=p/(1.0-e*e);
//         if (fabs(sqrt(-mu/a/a/a)*dt) > e-1.0) { 
//            FindHyperbolicRadius(mu,p,e,dt,&R);
//            cth = (p/R-1.0)/e;
//            th = acos(cth);
//            if (dt < 0.0) th = -th;
//            sth = sin(th);
//         }
//         else {
//            FindTrueAnomaly(mu,p,e,dt,&th);
//            sth = sin(th);
//            cth = cos(th);
//            R = p/(1.0+e*cth);
//         }
//      }
//      else {
         th = TrueAnomaly(mu,p,e,dt);
         sth = sin(th);
         cth = cos(th);
         R = p/(1.0+e*cth);
//      }

      c2 = sqrt(mu/p);
      pr[0] = R*cth;
      pr[1] = R*sth;
      pr[2] = 0.0;
      pv[0] = -c2*sth;
      pv[1] = c2*(e+cth);
      pv[2] = 0.0;

      C1=cos(RAAN);S1=sin(RAAN);
      C2=cos(i);S2=sin(i);
      C3=cos(omg);S3=sin(omg);

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
/* Q(S1) = 4/3*F(3,1;5/2;S1) is a hypergeometric function.            */
/* See Battin                                                         */
double HyperQ(double S1)
{
      double delta = 1.0;
      double u = 1.0;
      double Sigma = 1.0;
      short n = 1;
      double gamma;
      
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
      return(4.0/3.0*Sigma);
}
/**********************************************************************/
/* See Battin 7.1                                                     */
double LambertTOF(double mu, double amin, double lambda, double x)
{
      double y,eta,S1,Q,T;
      double delta = 1.0;
      double u = 1.0;
      double Sigma = 1.0;
      short n = 1;
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
   double TOF,double TransferType,double *p, double *e, double *inc, 
   double *RAAN, double *ArgP, double *tp)
{
      double r1,r2,ir1[3],ir2[3],ih[3],th,dr[3],c,s,amin,lambda;
      double xold,Told,x,dx,T;
      double y,eta,ihxir1[3],xv1[3],Coef0,Coef1,Coef2;
      double a,anom,alpha;
      short i;
      
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
      RV2Eph(t0,mu,xr1,xv1,&a,e,inc,RAAN,ArgP,&anom,tp,p,&alpha);
}
/**********************************************************************/
double RendezvousCostFunction(double *InVec, double *AuxVec)
{
      double t0,TOF,mu,r1e[3],v1e[3],r2e[3],v2e[3];
      double tf,SMA,ecc,inc,RAAN,ArgP,anom,tp,p,alpha;
      double r1[3],v1[3],r2[3],v2[3];
      double r1t[3],v1t[3],r2t[3],v2t[3];
      double DV1I[3],DV2I[3],DeltaVI;
      double DV1II[3],DV2II[3],DeltaVII;
      double DV1[3],DV2[3],DeltaV;
      double Per1,Per2;
      short i;
      
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
      RV2Eph(0.0,mu,r1e,v1e,&SMA,&ecc,&inc,&RAAN,&ArgP,&anom,&tp,&p,&alpha);
      Eph2RV(mu,p,ecc,inc,RAAN,ArgP,t0-tp,r1,v1,&anom);
      Per1 = 6.28*sqrt(SMA*SMA*SMA/mu);
      
      RV2Eph(0.0,mu,r2e,v2e,&SMA,&ecc,&inc,&RAAN,&ArgP,&anom,&tp,&p,&alpha);
      Eph2RV(mu,p,ecc,inc,RAAN,ArgP,tf-tp,r2,v2,&anom);
      Per2 = 6.28*sqrt(SMA*SMA*SMA/mu);

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
/*  Minimize a cost function by Downhill Simplex Method               */
/*  See Numerical Recipes 10.4 for description of method              */
/*                                                                    */
/*  N = Number of dimensions                                          */
/*  P = Vector which minimizes cost function                          */
/*      (Initial guess in, result out)                                */
/*  CostFunction must take an N-vector as argument,                   */
/*               must return a double                                 */
/*  scale = Size of initial amoeba                                    */
/*  Tol = Tolerance on cost function to declare convergence           */
double Amoeba(short N, double *P, 
            double CostFunction(double *p, double *Parm), 
            double *CostParm, double scale, double Tol)
{

      short Converged = 0;
      double **p,*pc,*f;
      short high,nexthigh,low;
      double Coef1,Coef2,*pn,fn,StepSize;
      double MinCost;
      short i,j;

      p = (double **) calloc(N+1,sizeof(double *));
      for(i=0;i<N+1;i++) p[i] = (double *) calloc(N,sizeof(double));
      
      pc = (double *) calloc(N,sizeof(double));
      f = (double *) calloc(N+1,sizeof(double));
      pn = (double *) calloc(N,sizeof(double));

      /* Simplex */
      for(j=0;j<N;j++) p[0][j] = P[j];
      for(i=1;i<N+1;i++) {
         for(j=0;j<N;j++) {
            p[i][j] = p[0][j];
         }
         p[i][i-1] += scale;
      }
         
      /* Simplex Centroid */
      for(j=0;j<N;j++) {          
         pc[j] = 0.0;
         for(i=0;i<N+1;i++) {
            pc[j] += p[i][j];
         }
         pc[j] /= (N+1.0);
      }
         
      /* Evaluate cost function */
      for(i=0;i<N+1;i++) {
         f[i] = CostFunction(p[i],CostParm);
      }
         
      /* Find high, next-to-high, low cost */
      low = 0;
      high = 0;
      for(i=1;i<N+1;i++) {
         if (f[i] < f[low])  low = i;
         if (f[i] > f[high]) high = i;
      }
      nexthigh = low;
      for(i=0;i<N+1;i++) {
         if (f[i] > f[nexthigh] && f[i] < f[high])  nexthigh = i;
      }
      
      while (!Converged) {
      
         /* Try Reflection */
         StepSize = -1.0;
         Coef1 = (N+1.0)/N*(1.0-StepSize);
         Coef2 = -(1.0-(N+1.0)*StepSize)/N;
         for (j=0;j<N;j++) {
            pn[j] = Coef1*pc[j] + Coef2*p[high][j];
         }
         fn = CostFunction(pn,CostParm);      
         if (fn < f[high]) {
            f[high] = fn;
            for(j=0;j<N;j++) {
               pc[j] += (pn[j] - p[high][j])/(N+1);
               p[high][j] = pn[j];
            }
         }
         if (f[high] < f[low]) { /* Worked so well, try longer step */
            StepSize = 2.0;
            Coef1 = (N+1.0)/N*(1.0-StepSize);
            Coef2 = -(1.0-(N+1.0)*StepSize)/N;
            for (j=0;j<N;j++) {
               pn[j] = Coef1*pc[j] + Coef2*p[high][j];
            }
            fn = CostFunction(pn,CostParm);      
            if (fn < f[high]) {
               f[high] = fn;
               for(j=0;j<N;j++) {
                  pc[j] += (pn[j] - p[high][j])/(N+1);
                  p[high][j] = pn[j];
               }
            }
         }
         else if (f[high] > f[nexthigh]) { /* Worked not so well, so try contraction */
            StepSize = 0.5;
            Coef1 = (N+1.0)/N*(1.0-StepSize);
            Coef2 = -(1.0-(N+1.0)*StepSize)/N;
            for (j=0;j<N;j++) {
               pn[j] = Coef1*pc[j] + Coef2*p[high][j];
            }
            fn = CostFunction(pn,CostParm);      
            if (fn < f[high]) {
               f[high] = fn;
               for(j=0;j<N;j++) {
                  pc[j] += (pn[j] - p[high][j])/(N+1);
                  p[high][j] = pn[j];
               }
            }
            if (f[high] > f[nexthigh]) { /* Stuck.  Contract about lowest point */
               for(i=0;i<N+1;i++) {
                  if (i != low) {
                     for(j=0;j<N;j++) {
                        p[i][j] = 0.5*(p[i][j] + p[low][j]);
                     }
                     f[i] = CostFunction(p[i],CostParm);
                  }
               }            
               /* Find centroid */
               for(j=0;j<N;j++) {          
                  pc[j] = 0.0;
                  for(i=0;i<N+1;i++) {
                     pc[j] += p[i][j];
                  }
                  pc[j] /= (N+1.0);
               }
            }
         }

         /* Find high, next-to-high, low cost */
         low = 0;
         high = 0;
         for(i=1;i<N+1;i++) {
            if (f[i] < f[low])  low = i;
            if (f[i] > f[high]) high = i;
         }
         nexthigh = low;
         for(i=0;i<N+1;i++) {
            if (f[i] > f[nexthigh] && f[i] < f[high])  nexthigh = i;
         }
         
         /* Termination Condition */
         if ((fabs(f[low]/f[high]-1.0) < Tol) || 
             (fabs(f[low]-f[high]) < Tol)) {
            Converged = 1;
         }
         
      }
      for(j=0;j<N;j++) P[j] = p[low][j];
      MinCost = f[low];
            
      for(i=0;i<N+1;i++) free(p[i]);
      free(p);
      free(pc);
      free(f);
      free(pn);
      
      return(MinCost);
}
/**********************************************************************/
void PlanTwoImpulseRendezvous(double mu, double r1e[3], double v1e[3],
   double r2e[3], double v2e[3], double *t1, double *t2,
   double DV1[3], double DV2[3])      
{
      double SMA1,SMA2,ecc,inc,RAAN,ArgP,anom1,anom2,tp,p,alpha;
      double AmP[2],AmParm[19],DeltaV;
      short i;

      RV2Eph(0.0,mu,r1e,v1e,&SMA1,&ecc,&inc,&RAAN,&ArgP,&anom1,&tp,&p,&alpha);
      RV2Eph(0.0,mu,r2e,v2e,&SMA2,&ecc,&inc,&RAAN,&ArgP,&anom2,&tp,&p,&alpha);
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
/*  Find optimal 2-impulse rendezvous from (r1,v1) to (r2,v2), both   */
/*  known at epoch time te.  Find time of first impulse, t0, and      */
/*  time of second impulse, tf.  Minimize total delta-V.              */
int main(int argc, char **argv)
{
#define TYPE_I (1.0)
#define TYPE_II (-1.0)

      double mu, r1[3],r2[3],v1[3],v2[3],t0,tf,TOF,t;
      double SMA1,SMA2,ecc,inc,RAAN,ArgP,tp,p,alpha;
      double r1t[3],v1t[3],r2t[3],v2t[3],anom1,anom2;
      double DV1[3],DV2[3],DeltaVI,DeltaVII,DeltaV;
      double r1e[3],v1e[3],r2e[3],v2e[3];
      double x1,x2,lambda1,lambda2;
      double AmP[2],AmParm[19];
      short i;
      
      FILE *outfile;
      
      outfile=fopen("ti.txt","wt");
            
      mu = 1.0;
      
/* .. Positions, velocities at epoch time */
      r1e[0] = 1.0;
      r1e[1] = 0.0;
      r1e[2] = 0.0;
      v1e[0] = 0.0;
      v1e[1] = 1.0;
      v1e[2] = 0.0;
      r2e[0] = 0.0;
      r2e[1] = 2.0;
      r2e[2] = 0.0;
      v2e[0] = -sqrt(0.5);
      v2e[1] = -0.1;
      v2e[2] = 0.2;
            
/* .. Guess t0, tf */
      for(t0=0.0;t0<2.0;t0+=0.01) {
         for(TOF=5.0;TOF<9.0;TOF+=0.05) {
      
            tf = t0 + TOF;
             
      /* .. Find r1(t0), r2(tf) */
            RV2Eph(0.0,mu,r1e,v1e,&SMA1,&ecc,&inc,&RAAN,&ArgP,&anom1,&tp,&p,&alpha);
            Eph2RV(mu,p,ecc,inc,RAAN,ArgP,t0-tp,r1,v1,&anom1);
            
            RV2Eph(0.0,mu,r2e,v2e,&SMA2,&ecc,&inc,&RAAN,&ArgP,&anom2,&tp,&p,&alpha);
            Eph2RV(mu,p,ecc,inc,RAAN,ArgP,tf-tp,r2,v2,&anom2);

      /* .. Solve Lambert problem to find transfer orbit, transfer angle < Pi */
            LambertProblem(t0,mu,r1,r2,TOF,TYPE_I,&p,&ecc,&inc,&RAAN,&ArgP,&tp);
         
      /* .. Compute delta-V */
            Eph2RV(mu,p,ecc,inc,RAAN,ArgP,t0-tp,r1t,v1t,&anom1);
            Eph2RV(mu,p,ecc,inc,RAAN,ArgP,tf-tp,r2t,v2t,&anom2);
         
            for(i=0;i<3;i++) {
               DV1[i] = v1t[i]-v1[i];
               DV2[i] = v2[i]-v2t[i];
            }
            DeltaVI = MAGV(DV1) + MAGV(DV2);
            
      /* .. Solve Lambert problem to find transfer orbit, transfer angle > Pi */
            LambertProblem(t0,mu,r1,r2,TOF,TYPE_II,&p,&ecc,&inc,&RAAN,&ArgP,&tp);
               
      /* .. Compute delta-V */
            Eph2RV(mu,p,ecc,inc,RAAN,ArgP,t0-tp,r1t,v1t,&anom1);
            Eph2RV(mu,p,ecc,inc,RAAN,ArgP,tf-tp,r2t,v2t,&anom2);
            
            for(i=0;i<3;i++) {
               DV1[i] = v1t[i]-v1[i];
               DV2[i] = v2[i]-v2t[i];
            }
            DeltaVII = MAGV(DV1) + MAGV(DV2);

            if (DeltaVI < DeltaVII) DeltaV = DeltaVI;
            else DeltaV = DeltaVII;
            
            fprintf(outfile,"%lf %lf %lf\n",t0,TOF,DeltaV);
         }
      }      
      fclose(outfile);

/* .. Now use Amoeba to search for optimum */
      PlanTwoImpulseRendezvous(mu,r1e,v1e,r2e,v2e,&t0,&tf,DV1,DV2);      

      
      return(0);
      
#undef TYPE_I
#undef TYPE_II
}
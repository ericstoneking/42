/*    This file is distributed with 42,                               */
/*    the (mostly harmless) spacecraft dynamics simulation            */
/*    created by Eric Stoneking of NASA Goddard Space Flight Center   */

/*    Copyright 2010 United States Government                         */
/*    as represented by the Administrator                             */
/*    of the National Aeronautics and Space Administration.           */

/*    No copyright is claimed in the United States                    */
/*    under Title 17, U.S. Code.                                      */

/*    All Other Rights Reserved.                                      */


#include "fswkit.h"

/* #ifdef __cplusplus
** namespace Kit {
** #endif
*/

/**********************************************************************/
void FindPDGains(double I, double w, double z, double *Kr, double *Kp)
{

   *Kr = 2.0*I*z*w;
   *Kp = I*w*w;
}
/**********************************************************************/
double Limit(double x,double min, double max)
{
      return(x < min ? min : (x > max ? max : x));
}
/**********************************************************************/
double SpinGainCostFunction(double p[2],double CostParm[2])
{
      double Coef[5],Real[4],Imag[4],Cost;
      double Om,K1;
      long i;

      Om = CostParm[0];
      K1 = CostParm[1];

      Coef[0] = 1.0;
      Coef[1] = 2.0*(p[0]+p[1]);
      Coef[2] = (K1*K1+1.0)*Om*Om+(p[0]+p[1])*(p[0]+p[1]);
      Coef[3] = 2.0*(p[0]+K1*K1*p[1])*Om*Om;
      Coef[4] = (p[0]-K1*p[1])*(p[0]-K1*p[1])*Om*Om+K1*K1*Om*Om*Om*Om;

      Bairstow(4,Coef,1.0E-8,Real,Imag);

      Cost = Real[0];
      for(i=1;i<4;i++) if (Real[i] > Cost) Cost = Real[i];

      return(Cost);

}
/**********************************************************************/
/*  This function finds the required spin rate, precession gain and   */
/*  nutation gain for an inertial spinner.                            */
/*  Inputs:                                                           */
/*     J      : Moment of inertia of spin axis [kg-m^2]               */
/*     It     : Moment of inertia of transverse axes [=sqrt(I1*I2)]   */
/*     Tc     : Expected magnitude of cyclic disturbance torque [N-m] */
/*     OrbPer : Orbit period [sec]                                    */
/*     alpha  : Allowable cyclic pointing offset [rad]                */
/*  Outputs:                                                          */
/*     Om     : Required spin rate [rad/sec]                          */
/*     Kn     : Nutation gain [N-m/(rad/sec)]                         */
/*     Kp     : Precession gain [N-m/rad]                             */
/**********************************************************************/
void FindSpinnerGains(double J, double It, double Tc, double OrbPer,
                      double alpha,
                      double *SpinRate, double *Knute, double *Kprec)
{
      double Ki,K1,Om,p[2];
      double SpinGainCostParm[2];

      Ki = J/It;
      K1 = Ki-1.0;

/* .. Spin rate gives gyroscopic stiffness against disturbance torques */
      Om = Tc*(0.25*OrbPer)/(J*alpha);

/* .. Find Kprec, Knute by numerical search */
      /* minmax(Real(roots of CE)) through choice of Kp, Kn */
      SpinGainCostParm[0] = Om;
      SpinGainCostParm[1] = K1;
      p[0] = 0.1;
      p[1] = 10.0;
      Amoeba(2,p,SpinGainCostFunction,SpinGainCostParm,Om,1.0E-8);

      *SpinRate = Om;
      *Knute = p[0]*It;
      *Kprec = p[1];

}
/**********************************************************************/
/*                                                                    */
/*  TRIAD Attitude Determination                                      */
/*                                                                    */
/*  Given components of two vectors (V and W) defined in A frame and  */
/*  in B frame, find the direction cosine matrix CBA.                 */
void TRIAD(double Va[3], double Wa[3], double Vb[3], double Wb[3],
           double CBA[3][3])
{
      double u[3],v[3],q[3],r[3],s[3],Ma[3][3],Mb[3][3];
      long i;

      for(i=0;i<3;i++) {
         u[i] = Va[i];
         v[i] = Wa[i];
         q[i] = u[i];
      }
      UNITV(u);
      UNITV(v);
      UNITV(q);
      VxV(u,v,r);
      UNITV(r);
      VxV(q,r,s);
      UNITV(s);
      for(i=0;i<3;i++) {
         Ma[i][0]=q[i];
         Ma[i][1]=r[i];
         Ma[i][2]=s[i];
      }

      for(i=0;i<3;i++) {
         u[i] = Vb[i];
         v[i] = Wb[i];
         q[i] = u[i];
      }
      UNITV(u);
      UNITV(v);
      UNITV(q);
      VxV(u,v,r);
      UNITV(r);
      VxV(q,r,s);
      UNITV(s);
      for(i=0;i<3;i++) {
         Mb[i][0]=q[i];
         Mb[i][1]=r[i];
         Mb[i][2]=s[i];
      }

      MxMT(Mb,Ma,CBA);
}
/**********************************************************************/
/*                                                                    */
/*              QUEST attitude determination algorithm                */
/*                                                                    */
/* Given N reference vectors Ref(i) and the corresponding measurement */
/* vectors Meas(i) in the body, along with weights Weight(i), this    */
/* routine finds the optimal estimate of q, the quaternion expressing */
/* the rotation from the reference frame (where Ref's are given) to   */
/* the body frame (where Meas's are given).                           */
void Quest(long n, double *Weight,double **Ref,double **Meas,
   double qmr[4])
{
      long i,j,k;

      double B[3][3],S[3][3];
      double Z[3] = {0.0,0.0,0.0};
      double sigma,kappa,delta;
      double aa,bb,cc,dd;
      double lam,f0,f1,f2,f,fp,lam2;
      double alpha,beta,gamma;
      double WxV[3],SZ[3],XX[3][3],X[3],mag,SS[3][3],SSZ[3];

/* .. Normalize weights and measurements */
      mag = 0.0;
      for(i=0;i<n;i++) mag += Weight[i];
      for(i=0;i<n;i++) Weight[i] /= mag;

      for(i=0;i<n;i++) {
         UNITV(Ref[i]);
         UNITV(Meas[i]);
      }

      for(j=0;j<3;j++) {
         for(k=0;k<3;k++) {
            B[j][k] = 0.0;
            for(i=0;i<n;i++) {
               B[j][k]=B[j][k]+Weight[i]*Meas[i][j]*Ref[i][k];
            }
         }
      }

      for(j=0;j<3;j++) {
         for(k=0;k<3;k++) {
            S[j][k] = B[j][k] + B[k][j];
         }
      }

      for(j=0;j<3;j++) {
         for(k=0;k<3;k++) {
            SS[j][k] = S[j][0]*S[0][k]
                     + S[j][1]*S[1][k]
                     + S[j][2]*S[2][k];
         }
      }

      for(i=0;i<n;i++) {
         VxV(Meas[i],Ref[i],WxV);
         for(j=0;j<3;j++) Z[j] += Weight[i]*WxV[j];
      }

      sigma = B[0][0]+B[1][1]+B[2][2];
      kappa = S[1][1]*S[2][2]-S[1][2]*S[2][1]
             +S[0][0]*S[2][2]-S[0][2]*S[2][0]
             +S[0][0]*S[1][1]-S[0][1]*S[1][0];
      delta = S[0][0]*S[1][1]*S[2][2]+S[0][1]*S[1][2]*S[2][0]
             +S[1][0]*S[2][1]*S[0][2]-S[0][0]*S[2][1]*S[1][2]
             -S[0][1]*S[1][0]*S[2][2]-S[0][2]*S[1][1]*S[2][0];

      aa = sigma*sigma - kappa;

      bb = sigma*sigma + VoV(Z,Z);

      MxV(S,Z,SZ);
      MxV(SS,Z,SSZ);

      cc = delta + VoV(Z,SZ);

      dd = VoV(Z,SSZ);

      f0 = aa*bb+cc*sigma-dd;
      f1 = -cc;
      f2 = -(aa+bb);

      lam = 1.0;
      for(i=0;i<4;i++) {  /* Assumes 4 iterations sufficient for convergence */
         lam2 = lam*lam;
         f = f0 + f1*lam + f2*lam2 + lam2*lam2;
         fp = f1 + 2.0*f2*lam + 4.0*lam2*lam;
         lam -= f/fp;
      }

      alpha = lam*lam - sigma*sigma + kappa;
      beta = lam - sigma;
      gamma = (lam + sigma)*alpha - delta;

      for(j=0;j<3;j++) {
         for(k=0;k<3;k++) {
            XX[j][k] = beta*S[j][k]+SS[j][k];
         }
         XX[j][j] += alpha;
      }

      MxV(XX,Z,X);

      mag = sqrt(X[0]*X[0]+X[1]*X[1]+X[2]*X[2]+gamma*gamma);

      qmr[0] = X[0]/mag;
      qmr[1] = X[1]/mag;
      qmr[2] = X[2]/mag;
      qmr[3] = gamma/mag;
}
/**********************************************************************/
/*                                                                    */
/*  FILTER-QUEST attitude determination algorithm                     */
/*                                                                    */
/*  This is an embellishment of QUEST.  The B matrix is propagated    */
/*  forward in time using wbn.  The memory parameter mem              */
/*  (0 < mem < 1) is the relative weight given to propagation vs.     */
/*  measurements.  Updates are done as for QUEST, with as many vector */
/*  pairs as are available at any one time (thus it is possible to    */
/*  update with only one measurement at a time).                      */
/*  Given N reference vectors Ref(i) and the corresponding measure-   */
/*  ment vectors Meas(i) in the body, along with weights Weight(i),   */
/*  this routine finds the optimal estimate of qmr, the quaternion    */
/*  expressing the rotation from the reference frame (where Ref's are */
/*  given) to the body frame (where Meas's are given).                */
void FilterQuest(long n, double *Weight, double **Ref, double **Meas,
                 double dt, double memory,double wbn[3], double qmr[4])
{
      double *a,**W,**V,rho;
      long i,j,k;

      double phi[3][3],aVW[3][3],phiB[3][3];
      double th,U[3];
      double B[3][3],S[3][3],Z[3];
      double sigma,kappa,delta;
      double aa,bb,cc,dd;
      double lam,f0,f1,f2,f,fp,lam2;
      double alpha,beta,gamma;
      double SZ[3],XX[3][3],X[3],mag,SS[3][3],SSZ[3];

      a = (double *) calloc(n,sizeof(double));
      if (a == NULL) {
         printf("calloc returned null pointer in FilterQuest.  Bailing out!\n");
         exit(1);
      }
      W = CreateMatrix(n,3);
      V = CreateMatrix(n,3);

/*.. Normalize weights and measurements */
      mag = 0.0;
      for(i=0;i<n;i++) mag += Weight[i];
      for(i=0;i<n;i++) a[i] = Weight[i]/mag;

      mag = memory;
      for(i=0;i<n;i++) mag += (1.0-memory)*a[i];
      rho = memory/mag;
      for(i=0;i<n;i++) a[i] *= (1.0-memory)/mag;

      for(i=0;i<n;i++) {
         CopyUnitV(Ref[i],V[i]);
         CopyUnitV(Meas[i],W[i]);
      }

/* .. Build transition matrix, phi */
      th = CopyUnitV(wbn,U);
      SimpRot(U,th,phi);

/* .. Debug this.  Where does B come from? */
      MxM(phi,B,phiB);

/* .. Update B with measurements */
      for(j=0;j<3;j++) {
         for(k=0;k<3;k++) {
            aVW[j][k]=0.0;
            for(i=0;i<n;i++) aVW[j][k] += a[i]*V[i][k]*W[i][j];
         }
      }

      for(j=0;j<3;j++) {
         for(k=0;k<3;k++) {
            B[j][k] = rho*phiB[j][k]+aVW[j][k];
         }
      }

/* .. Find quaternion */
      for(j=0;j<3;j++) {
         for(k=0;k<3;k++) {
            S[j][k] = B[j][k] + B[k][j];
         }
      }

      for(j=0;j<3;j++) {
         for(k=0;k<3;k++) {
            SS[j][k] = S[j][0]*S[0][k]
                     + S[j][1]*S[1][k]
                     + S[j][2]*S[2][k];
         }
      }

      Z[0] = B[1][2]-B[2][1];
      Z[1] = B[2][0]-B[0][2];
      Z[2] = B[0][1]-B[1][0];

      sigma = B[0][0]+B[1][1]+B[2][2];
      kappa = S[1][1]*S[2][2]-S[1][2]*S[2][1]
             +S[0][0]*S[2][2]-S[0][2]*S[2][0]
             +S[0][0]*S[1][1]-S[0][1]*S[1][0];
      delta = S[0][0]*S[1][1]*S[2][2]+S[0][1]*S[1][2]*S[2][0]
             +S[1][0]*S[2][1]*S[0][2]-S[0][0]*S[2][1]*S[1][2]
             -S[0][1]*S[1][0]*S[2][2]-S[0][2]*S[1][1]*S[2][0];

      aa = sigma*sigma - kappa;

      bb = sigma*sigma + VoV(Z,Z);

      MxV(S,Z,SZ);
      MxV(SS,Z,SSZ);

      cc = delta + VoV(Z,SZ);

      dd = VoV(Z,SSZ);

      f0 = aa*bb+cc*sigma-dd;
      f1 = -cc;
      f2 = -(aa+bb);

      lam = 1.0;
      for(i=0;i<4;i++) {  /* Assumes 4 iterations sufficient for convergence */
         lam2 = lam*lam;
         f = f0 + f1*lam + f2*lam2 + lam2*lam2;
         fp = f1 + 2.0*f2*lam + 4.0*lam2*lam;
         lam -= f/fp;
      }

      alpha = lam*lam - sigma*sigma + kappa;
      beta = lam - sigma;
      gamma = (lam + sigma)*alpha - delta;

      for(j=0;j<3;j++) {
         for(k=0;k<3;k++) {
            XX[j][k] = beta*S[j][k]+SS[j][k];
         }
         XX[j][j] += alpha;
      }

      MxV(XX,Z,X);

      mag = sqrt(X[0]*X[0]+X[1]*X[1]+X[2]*X[2]+gamma*gamma);

      qmr[0] = X[0]/mag;
      qmr[1] = X[1]/mag;
      qmr[2] = X[2]/mag;
      qmr[3] = gamma/mag;

      free(a);
      DestroyMatrix(W,n);
      DestroyMatrix(V,n);
}
/**********************************************************************/
/* Find the Euler Angles to point a given boresight vector fixed in   */
/* a joint's outer body (bvo) parallel to a target vector fixed in    */
/* the joint's inner body (tvi).                                      */
void PointGimbalToTarget(long Seq, double CGiBi[3][3],
                         double CBoGo[3][3],
                         double tvi[3], double bvo[3],
                         double GimAngCmd[3])
{
#define PI (3.141592653589793)
#define TWOPI (6.283185307179586)

      double *a1,*a2;
      double a3[3];
      double Axis[3][3] = {{1.0,0.0,0.0},{0.0,1.0,0.0},{0.0,0.0,1.0}};
      double TargVec[3],BoreVec[3];
      double TargAng1,BoreAng1,TargAng2,BoreAng2;
      double t1,t2,t3,b1,b2,b3;
      double Cycle;

      /* Form perpendicular basis for unrotated gimbal */
      /* Call it the A frame */
      switch (Seq) {
         case 1   :
         case 12  :
         case 123 :
         case 121 :
            a1 = Axis[0];
            a2 = Axis[1];
            Cycle = 1.0;
         break;
         case 13  :
         case 132 :
         case 131 :
            a1 = Axis[0];
            a2 = Axis[2];
            Cycle = -1.0;
         break;
         case 2   :
         case 23  :
         case 231 :
         case 232 :
            a1 = Axis[1];
            a2 = Axis[2];
            Cycle = 1.0;
         break;
         case 21  :
         case 213 :
         case 212 :
            a1 = Axis[1];
            a2 = Axis[0];
            Cycle = -1.0;
         break;
         case 3   :
         case 31  :
         case 312 :
         case 313 :
            a1 = Axis[2];
            a2 = Axis[0];
            Cycle = 1.0;
         break;
         case 32  :
         case 321 :
         case 323 :
            a1 = Axis[2];
            a2 = Axis[1];
            Cycle = -1.0;
         break;
         default :
            a1 = Axis[0];
            a2 = Axis[1];
            Cycle = 1.0;
      }
      VxV(a1,a2,a3);

      /* Transform target and boresight into unrotated Go (=Gi) frame */
      MxV(CGiBi,tvi,TargVec);
      MTxV(CBoGo,bvo,BoreVec);

      /* Find components of TargVec in A */
      t1 = VoV(TargVec,a1);
      t2 = VoV(TargVec,a2);
      t3 = VoV(TargVec,a3);

      /* Find components of BoreVec in A */
      b1 = VoV(BoreVec,a1);
      b2 = VoV(BoreVec,a2);
      b3 = VoV(BoreVec,a3);

      /* Find rotation about a1 to move BoreVec into TargVec */
      TargAng1 = atan2(-t2,t3);
      BoreAng1 = atan2(-b2,b3);

      GimAngCmd[0] = TargAng1 - BoreAng1;
      if (GimAngCmd[0] < -PI) GimAngCmd[0] += TWOPI;
      if (GimAngCmd[0] >  PI) GimAngCmd[0] -= TWOPI;

      /* Find rotation about a2 to move BoreVec into TargVec */
      TargAng2 = Cycle*asin(t1);
      BoreAng2 = Cycle*asin(b1);

      GimAngCmd[1] =  TargAng2 - BoreAng2;
      if (GimAngCmd[1] < -PI) GimAngCmd[1] += TWOPI;
      if (GimAngCmd[1] >  PI) GimAngCmd[1] -= TWOPI;

      /* Always */
      GimAngCmd[2] = 0.0;
#undef PI
#undef TWOPI
}
/**********************************************************************/
/*   Find control acceleration to seek goal at xg while staying at    */
/*   least Ra away from obstacle at xa.                               */
void CollisionAvoidanceLaw(double x[3], double v[3],
   double xg[3], double xa[3], double Ra, double vmax, double amax,
   double wc, double zc, double a[3])
{

      double Kx,Kv,d[3],xhat[3],xga[3],magx,dox,zhat[3],magz;
      double yhat[3],cosa,sina,lam1[3],lam2[3],magvcmd,maga,vcmd[3];
      long i;

      Kx = wc*wc;
      Kv = 2.0*zc*wc;

      for(i=0;i<3;i++) {
         d[i] = xg[i]-x[i];
         xhat[i] = x[i]-xa[i];
         xga[i] = xg[i]-xa[i];
      }
      UNITV(d);
      magx = UNITV(xhat);
      dox = VoV(d,xhat);
      VxV(xga,xhat,zhat);
      magz = UNITV(zhat);
      if (magz < 1.0E-3) {
         yhat[0] = 0.0;
         yhat[1] = 0.0;
         yhat[2] = 1.0;
         VxV(xhat,yhat,zhat);
      }
      VxV(zhat,xhat,yhat);
      cosa = Ra/magx;
      sina = sqrt(1.0-cosa*cosa);
      for(i=0;i<3;i++) {
         lam1[i] = -xhat[i]*sina-yhat[i]*cosa;
         lam2[i] = -yhat[i];
      }

      if (magx < 0.95*Ra) {
         /* Get out! */
         for(i=0;i<3;i++) a[i] = -Kv*(v[i]-vmax*xhat[i]);
      }
      else if (dox > 0.0 || ((Ra/magx*Ra/magx) + dox*dox < 1.0)) {
         /* Path is clear */
         for(i=0;i<3;i++) vcmd[i] = -Kx/Kv*(x[i]-xg[i]);
         magvcmd = MAGV(vcmd);
         if (magvcmd > vmax) {
            for(i=0;i<3;i++) vcmd[i] *= vmax/magvcmd;
         }
         for(i=0;i<3;i++) a[i] = -Kv*(v[i]-vcmd[i]);
      }
      else if (fabs(magx/Ra-1.0) < 0.01) {
         /* Turn on avoidance circle */
         for(i=0;i<3;i++)
            a[i] = -vmax*vmax/Ra*xhat[i] - Kv*(v[i]-vmax*lam2[i]);
      }
      else {
         /* Go to tangent on avoidance circle */
         for(i=0;i<3;i++) a[i] = -Kv*(v[i]-vmax*lam1[i]);
      }

      /* Limit control */
      maga = MAGV(a);
      if (maga > amax) {
         for(i=0;i<3;i++) a[i] *= amax/maga;
      }
}
/**********************************************************************/
double BangBangSettle(double x, double v, double w0,
                      double amax, double vmax)
{
      double a,vcmd,aswitch;
      double xc,x0,vc;
      double Kr,Kp;

      aswitch = 0.9*amax; /* Reserve some authority */
      xc = amax/w0/w0;
      x0 = 0.5*xc;
      vc = xc*w0;
      Kr = 2.0*w0;
      Kp = w0*w0;

      if (fabs(x) < xc && fabs(v) < vc) {
         a = -Kr*v-Kp*x;
      }
      else if (x > x0) {
         vcmd = -sqrt(2.0*aswitch*(x-x0));
         if (vcmd < -vmax) vcmd = -vmax;
         if (v > vcmd) {
            a = -(v - vcmd)*w0;
            if (a < -amax) a = -amax;
         }
         else {
            a = aswitch - (v - vcmd)*w0;
            if (a > amax) a = amax;
         }
      }
      else if (x < -x0) {
         vcmd = sqrt(2.0*aswitch*(-x-x0));
         if (vcmd > vmax) vcmd = vmax;
         if (v < vcmd) {
            a = -(v - vcmd)*w0;
            if (a > amax) a = amax;
         }
         else {
            a = -aswitch - (v - vcmd)*w0;
            if (a < -amax) a = -amax;
         }
      }
      else {
         if (v > 0.0) a = -amax;
         else a = amax;
      }

      return(a);
}
/**********************************************************************/
/* Near (x=0,v=0) this is a critically damped PD law with time        */
/* constant tau.  Far away from (0,0) this is a max-effort slew       */
/* with rate limiting.                                                */
/* Original algorithm by Eric Stoneking.                              */
/* Corrections by Blair Carter, 2011.                                 */
double RampCoastGlide(double x, double v, double w0,
                      double amax, double vmax)
{
      double a,vcmd,aswitch;
      double xc,x0;

      aswitch = 0.9*amax; /* Reserve some authority */
      xc = aswitch/w0/w0;
      x0 = 0.5*xc;

      if (x > xc) {
         vcmd = -sqrt(2.0*aswitch*(x-x0));
         if (v < vcmd) {
            a = amax;
         }
         else {
            if (vcmd < -vmax) vcmd = -vmax;
            a =  -(v-vcmd)*w0;
            a = Limit(a,-amax,amax);
         }
      }
      else if (x < -xc) {
         vcmd = sqrt(2.0*aswitch*(-x-x0));
         if (v > vcmd) {
            a = -amax;
         }
         else {
            if (vcmd > vmax) vcmd = vmax;
            a =  -(v-vcmd)*w0;
            a = Limit(a,-amax,amax);
         }
      }
      else {
         /* PD, Kx = w0*w0, Kv = 2.0*w0 */
         a = -(x*w0+2.0*v)*w0;
         a = Limit(a,-amax,amax);
      }

      return(a);
}
/**********************************************************************/
double RateControl(double v, double amax, double w0)
{
      double a = -v*w0;
      return(Limit(a,-amax,amax));
}
/**********************************************************************/
void VectorRampCoastGlide(double Xvec[3], double Vvec[3], double w0,
                      double amax, double vmax, double Avec[3])
{
      double Axis[3],Xaxis[3],Yaxis[3];
      double x,v,a,avx,avy;
      long i;

      if (MAGV(Xvec) > 0.0) {
         /* RampCoastGlide on line parallel to Xvec */
         x=CopyUnitV(Xvec,Axis);
         v=VoV(Vvec,Axis);
         a=RampCoastGlide(x,v,w0,amax,vmax);
         /* RateControl on transverse axes */
         PerpBasis(Axis,Xaxis,Yaxis);
         avx = RateControl(VoV(Vvec,Xaxis),amax,w0);
         avy = RateControl(VoV(Vvec,Yaxis),amax,w0);

         for(i=0;i<3;i++)
            Avec[i] = a*Axis[i] + avx*Xaxis[i] + avy*Yaxis[i];
      }
      else {
         for(i=0;i<3;i++) Avec[i] = RateControl(Vvec[i],amax,w0);
      }
}
/**********************************************************************/
/* Beta is angle between Sun vector and the orbit plane.              */
/* It is positive toward the positive orbit normal.                   */
double SolarBeta(double svn[3], double psn[3], double vsn[3])
{
      double h[3];

      VxV(psn,vsn,h);
      UNITV(h);
      return(asin(VoV(svn,h)));
}
/**********************************************************************/
/* These functions implement linear programming for thruster          */
/* selection logic.  Onpulsing or offpulsing.                         */
/* Ref Strang, Ch. 8                                                  */
/*    Let t = Thrust commands, n x 1 (to be solved for)               */
/*    Let f = Force and torque command, m x 1 (given)                 */
/*        A = Thruster coefficient matrix, m x n                      */
/*        T = Tableau matrix, (m+2) x (n+1)                           */
/* For onpulsing, the problem is to minimize sum(t),                  */
/*    subject to t>=0, A*t=f                                          */
/* For offpulsing, the problem is to minimize sum(tmax-t)             */
/*    subject to (tmax-t)>=0, A*(tmax-t)=A*tmax-f                     */
/**********************************************************************/
void MakeTableau(double **A, double *f, double tmax,
   long m, long n, long OffPulse, double **T)
{
      double At;
      long i,j;


      /* Indices keep track of basis-vs-free variables */
      for(j=0;j<n;j++) T[0][j] = j;

      /* This corner isn't used */
      T[0][n] = 0.0;

      /* A */
      for(i=0;i<m;i++) {
         for(j=0;j<n;j++) {
            T[i+1][j] = A[i][j];
         }
      }

      /* b */
      if (OffPulse) {
         for(i=0;i<m;i++) {
            At = 0.0;
            for(j=0;j<n;j++) At += A[i][j];
            T[i+1][n] = At*tmax-f[i];
         }
      }
      else {
         for(i=0;i<m;i++) T[i+1][n] = f[i];
      }

      /* c */
      for(j=0;j<n;j++) T[m+1][j] = 1.0;

      /* This corner will end up with -Cost */
      T[m+1][n] = 0.0;

}
/**********************************************************************/
void TableauGaussElim(double **T, long m, long n)
{
      double pivot,temp,K;
      long row,ip,i,j;

      for(row=0;row<m;row++) {
         pivot = T[row+1][row];
         ip = row;
         for(i=row+1;i<m;i++) {
            if (fabs(T[i+1][row]) > fabs(pivot)) {
               pivot = T[i+1][row];
               ip = i;
            }
         }
         if (pivot == 0.0) {
            printf("Singular Matrix in TableauGaussElim\n");
            exit(1);
         }
         for(j=0;j<n+1;j++) {
            temp = T[ip+1][j];
            T[ip+1][j] = T[row+1][j];
            T[row+1][j] = temp/pivot;
         }
         for(i=row+1;i<m+1;i++) {
            K = T[i+1][row];
            for(j=0;j<n+1;j++) T[i+1][j] -= K*T[row+1][j];
         }
      }
      for(row=m-1;row>0;row--) {
         for(i=0;i<row;i++) {
            K = T[i+1][row];
            for(j=0;j<n+1;j++) T[i+1][j] -= K*T[row+1][j];
         }
      }
}
/**********************************************************************/
void FindSwap(double **T, long *incoming, long *outgoing,
   long m, long n)
{
      double minratio = 1.0E20; /* Arbitrary really big number */
      double ratio;
      long i,j;

      for(i=0;i<m;i++) {
         if (T[i+1][n] < 0.0) {
            for(j=m;j<n;j++) {
               if (T[i+1][j] < 0.0) {
                  ratio = T[i+1][n]/T[i+1][j];
                  if (ratio < minratio) {
                     minratio = ratio;
                     *incoming = j;
                     *outgoing = i;
                  }
               }
            }
         }
      }
}
/**********************************************************************/
void SwapCol(double **T, long j1, long j2, long m)
{
      long i;
      double temp;

      long M = m+2;

      for(i=0;i<M;i++) {
         temp = T[i][j1];
         T[i][j1] = T[i][j2];
         T[i][j2] = temp;
      }
}
/**********************************************************************/
double InterpretTableau(double **T, double tmax, double *t,
   long m, long n, long OffPulse)
{
      long i,idx;

      for(i=0;i<n;i++) t[i] = 0.0;
      for(i=0;i<m;i++) {
         idx = (long) T[0][i];
         t[idx] = T[i+1][n];
      }
      if (OffPulse) {
         for(i=0;i<n;i++) t[i] = tmax - t[i];
      }

      return(-T[m+1][n]);
}
/**********************************************************************/
double ThrusterSelection(double **A, double *f, double *t,
   double tmax, long m, long n, long OffPulse)
{
      double **T;
      long Done,i;
      double Cost;
      long incoming = 0,outgoing = 0;

      T = CreateMatrix(m+2,n+1);

      MakeTableau(A,f,tmax,m,n,OffPulse,T);
      TableauGaussElim(T,m,n);

      Done = 1;
      for(i=1;i<=m;i++) {
         if (T[i][n] < 0.0) Done = 0;
      }
      while(!Done) {
         FindSwap(T,&incoming,&outgoing,m,n);
         SwapCol(T,incoming,outgoing,m);
         TableauGaussElim(T,m,n);
         Done = 1;
         for(i=1;i<=m;i++) {
            if (T[i][n] < 0.0) Done = 0;
         }
      }
      Cost = InterpretTableau(T,tmax,t,m,n,OffPulse);
      DestroyMatrix(T,m+2);
      return(Cost);
}

/**********************************************************************/
/*     Fixed-Gain Predictor Estimator                                 */
/*     x(k+1) = PHI*x(k) + GAMMA*u(k) + L*(y(k) - H*x(k))             */
/*                                                                    */
/*     x[Nx] is state estimate (x(k) in, x(k+1) out)                  */
/*     y[Ny] is measurement (in)                                      */
/*     u[Nu] is control (or other input)                              */

void StateEstimator(double **PHI, double **GAMMA, double **H,
   double **L, double *u, double *y, double *x,
   long Nx, long Nu, long Ny)
{
      double *Hx,*Lz,*PHIx,*GAMMAu;
      long i,j;

      Hx = (double *) calloc(Ny,sizeof(double));
      Lz = (double *) calloc(Nx,sizeof(double));
      PHIx = (double *) calloc(Nx,sizeof(double));
      GAMMAu = (double *) calloc(Nx,sizeof(double));

      for(i=0;i<Ny;i++) {
         for(j=0;j<Nx;j++) {
            Hx[i] += H[i][j]*x[j];
         }
      }
      for(i=0;i<Nx;i++) {
         for(j=0;j<Ny;j++) {
            Lz[i] += L[i][j]*(y[j]-Hx[j]);
         }
      }
      for(i=0;i<Nx;i++) {
         for(j=0;j<Nx;j++) {
            PHIx[i] += PHI[i][j]*x[j];
         }
      }
      for(i=0;i<Nx;i++) {
         for(j=0;j<Nu;j++) {
            GAMMAu[i] += GAMMA[i][j]*u[j];
         }
      }

      for(i=0;i<Nx;i++) {
         x[i] = PHIx[i] + GAMMAu[i] + Lz[i];
      }

      free(Hx);
      free(Lz);
      free(PHIx);
      free(GAMMAu);
}
/**********************************************************************/
/* The following subroutines implement a U-D estimator, following     */
/* Bierman, Gerald J., "Factorization Methods for Discrete            */
/* Sequential Estimation", Academic Press, 1977.                      */

/**********************************************************************/
/* Ref Bierman, p. 53                                                 */
/*  Matrix P is input.  Output U is upper triangular, with elements   */
/*  of D stored on its diagonal.  (Bierman is big on saving space.)   */
void UDUFactor(double **P, double **U, long N)
{
      double alpha,beta;
      long i,j,k;

      for(j=N-1;j>0;j--) {
         U[j][j]=P[j][j];
         alpha=1.0/U[j][j];
         for(k=0;k<j;k++) {
            beta=P[k][j];
            U[k][j]=alpha*beta;
            for(i=0;i<=k;i++) {
               P[i][k]-= beta*U[i][j];
            }
         }
      }
      U[0][0]=P[0][0];
}
/**********************************************************************/
/* Ref Bierman, p. 100-101                                            */
/*   x = State vector (in/out)                                        */
/*   U = Covariance matrix (in/out)                                   */
/*   y = Measurement scalar (in)                                      */
/*   H = Observation vector (in)                                      */
/*   Rv = Measurement noise variance (in)                             */
/*   Ns = Number of states (in)                                       */
/*                                                                    */
/*   x = x + K*(y-H*x)                                                */
void UDMeasUpdate(double *x, double **U, double y, double *H,
   double Rv, long Ns)
{
      double *a,*b,z,alpha,beta,gamma,lambda;
      long i,j,k;

      a = (double *) calloc(Ns,sizeof(double));
      b = (double *) calloc(Ns,sizeof(double));

      z = y;
      for(j=0;j<Ns;j++) {
         a[j] = H[j];
         z -= a[j]*x[j];
      }

      for(j=Ns-1;j>0;j--) {
         for(k=0;k<j;k++) {
            a[j] += U[k][j]*a[k];
         }
         b[j] = U[j][j]*a[j];
      }
      b[0] = U[0][0]*a[0];

      alpha = Rv+b[0]*a[0];
      gamma = 1.0/alpha;
      U[0][0] *= Rv*gamma;

      for(j=1;j<Ns;j++) {
         beta = alpha;
         alpha += b[j]*a[j];
         lambda = -a[j]*gamma;
         gamma = 1.0/alpha;
         U[j][j] *= beta*gamma;
         for(i=0;i<j;i++) {
            beta = U[i][j];
            U[i][j] = beta + b[i]*lambda;
            b[i] += b[j]*beta;
         }
      }
      z *= gamma;

      for(j=0;j<Ns;j++) {
         x[j] += b[j]*z;
      }

      free(a);
      free(b);
}
/**********************************************************************/
/* Ref Bierman, p. 132-133                                            */
/*  x(k+1)=phi*x(k) + gam*y(k)                                        */
/*  U(k+1)=Something messy                                            */
void UDTimeUpdate(double *x, double **U, double **phi, double **gam,
   double *y, double *Rw,long Ns, long Nw)
{
      double **Uaug,*D,*v,*a,*Gy;
      double sigma,Dinv;
      long i,j,k;
      long Naug = Ns + Nw;

      Uaug = CreateMatrix(Naug,Naug);
      D = (double *) calloc(Naug,sizeof(double));
      v = (double *) calloc(Naug,sizeof(double));
      a = (double *) calloc(Naug,sizeof(double));
      Gy = (double *) calloc(Ns,sizeof(double));

      for(i=0;i<Ns;i++) {
         for(j=0;j<Ns;j++) {
            Uaug[i][j]=U[i][j];
         }
      }
      for(i=0;i<Nw;i++) {
         D[Ns+i]=Rw[i];
         Uaug[i][Ns+i]=gam[i][i];
      }

      v[0]=0.0;
      for(j=Ns-1;j>0;j--) {
         for(i=0;i<j;i++) {
            D[i]=Uaug[i][j];
         }
         for(i=0;i<Ns;i++) {
            Uaug[i][j]=phi[i][j];
            for(k=0;k<j;k++) {
               Uaug[i][j] += phi[i][k]*D[k];
            }
         }
      }
      D[0]=Uaug[0][0];

      for(j=0;j<Ns;j++) {
         v[j]=0.0;
         for(k=0;k<Ns;k++) {
            v[j] += phi[j][k]*x[k];
         }
         Gy[j]=0.0;
         for(k=0;k<Nw;k++) {
            Gy[j] += gam[j][k]*y[k];
         }
      }

      for(j=0;j<Ns;j++) {
         x[j] = v[j]+Gy[j];
         Uaug[j][0]=phi[j][0];
      }

      for(j=Ns-1;j>=0;j--) {
         sigma=0.0;
         for(k=0;k<Naug;k++) {
            v[k]=Uaug[j][k];
            a[k]=D[k]*v[k];
            sigma += v[k]*a[k];
         }
         Uaug[j][j]=sigma;
         Dinv=1.0/sigma;
         for(k=0;k<j;k++) {
            sigma=0.0;
            for(i=0;i<Naug;i++) {
               sigma += Uaug[k][i]*a[i];
            }
            sigma *= Dinv;
            for(i=0;i<Naug;i++) {
               Uaug[k][i] -= sigma*v[i];
            }
            Uaug[j][k]=sigma;
         }
      }
      sigma=0.0;
      for(k=0;k<Naug;k++) {
         v[k]=Uaug[0][k];
         a[k]=D[k]*v[k];
         sigma += v[k]*a[k];
      }

      for(j=1;j<Ns;j++) {
         for(i=0;i<j;i++) {
            U[i][j] = Uaug[j][i];
            U[j][i] = 0.0;
         }
      }

      DestroyMatrix(Uaug,Naug);
      free(D);
      free(v);
      free(a);
      free(Gy);
}
/******************************************************************************/
/*  The following functions implement a "sequential" Kalman Filter, with      */
/*  measurement updates having 1, 2, or 3 dimensions.                         */
/*  These functions use a KalmanFilterType struct to hold parameters and      */
/*  internal states.                                                          */ 

/******************************************************************************/
void AllocKalmanFilterMeasurement(struct KFMeasType *M,long Nx, long Ny)
{
      long i;
      
      if (Ny < 1 || Ny > 3) {
         printf("Hmm.  This Kalman Filter implementation assumes measurements are of \n");
         printf("dimension 1, 2, or 3.  Bailing out.\n");
         exit(1);
      }
      M->Ny = Ny;
      M->y = (double *) calloc(Ny,sizeof(double));
      M->Rv = (double **) calloc(Ny,sizeof(double *));
      for(i=0;i<Ny;i++) {
         M->Rv[i] = (double *) calloc(Ny,sizeof(double));
      }
      M->H = (double **) calloc(Ny,sizeof(double *));
      for(i=0;i<Ny;i++) {
         M->H[i] = (double *) calloc(Nx,sizeof(double));
      }
      M->L = (double **) calloc(Nx,sizeof(double *));
      for(i=0;i<Nx;i++) M->L[i] = (double *) calloc(Ny,sizeof(double));
      
      M->HP = (double **) calloc(Ny,sizeof(double *));
      for(i=0;i<Ny;i++) M->HP[i] = (double *) calloc(Nx,sizeof(double));
      
      M->Hx = (double *) calloc(Ny,sizeof(double));
      
      M->HPHtRv = (double **) calloc(Ny,sizeof(double *));
      for(i=0;i<Ny;i++) M->HPHtRv[i] = (double *) calloc(Ny,sizeof(double));

      M->HPHtRvInv = (double **) calloc(Ny,sizeof(double *));
      for(i=0;i<Ny;i++) M->HPHtRvInv[i] = (double *) calloc(Ny,sizeof(double));
}
/******************************************************************************/
struct KalmanFilterType *CreateKalmanFilter(long Nx, long Nu, long Nw, long Nm)
{
      long i;

      struct KalmanFilterType *KF;
      
      KF = (struct KalmanFilterType *) calloc(1,sizeof(struct KalmanFilterType));
      
      KF->Nx = Nx;
      KF->Nu = Nu;
      KF->Nw = Nw;
      KF->Nm = Nm;
      
      KF->x = (double *) calloc(Nx,sizeof(double));
      KF->u = (double *) calloc(Nu,sizeof(double));

      KF->Phi = (double **) calloc(Nx,sizeof(double *));
      for(i=0;i<Nx;i++) {
         KF->Phi[i] = (double *) calloc(Nx,sizeof(double *));
      }

      KF->Gam = (double **) calloc(Nx,sizeof(double *));
      for(i=0;i<Nx;i++) {
         KF->Gam[i] = (double *) calloc(Nu,sizeof(double *));
      }

      KF->Gamw = (double **) calloc(Nx,sizeof(double *));
      for(i=0;i<Nx;i++) {
         KF->Gamw[i] = (double *) calloc(Nw,sizeof(double *));
      }
      
      KF->P = (double **) calloc(Nx,sizeof(double *));
      for(i=0;i<Nx;i++) KF->P[i] = (double *) calloc(Nx,sizeof(double));
      
      KF->Rw = (double **) calloc(Nw,sizeof(double *));
      for(i=0;i<Nw;i++) {
         KF->Rw[i] = (double *) calloc(Nw,sizeof(double *));
      }
      
      KF->Meas = (struct KFMeasType *) calloc(Nm,sizeof(struct KFMeasType));
      
      KF->PhiX = (double *) calloc(Nx,sizeof(double));
      KF->GamU = (double *) calloc(Nx,sizeof(double));
      
      KF->PhiP = (double **) calloc(Nx,sizeof(double *));
      for(i=0;i<Nx;i++) KF->PhiP[i] = (double *) calloc(Nx,sizeof(double));
      
      KF->GRwGt = (double **) calloc(Nx,sizeof(double *));
      for(i=0;i<Nx;i++) KF->GRwGt[i] = (double *) calloc(Nx,sizeof(double));
      
      return(KF);
}
/******************************************************************************/
void PopulateKalmanFilterWorkspace(struct KalmanFilterType *KF)
{
      double **GRw;
      long i,j,k;

      GRw = (double **) calloc(KF->Nx,sizeof(double *));
      for(i=0;i<KF->Nx;i++) GRw[i] = (double *) calloc(KF->Nw,sizeof(double));
      for(i=0;i<KF->Nx;i++) {
         for(j=0;j<KF->Nw;j++) {
            for(k=0;k<KF->Nw;k++) {
               GRw[i][j] += KF->Gamw[i][k]*KF->Rw[k][j];
            }
         }
      }
      KF->GRwGt = (double **) calloc(KF->Nx,sizeof(double *));
      for(i=0;i<KF->Nx;i++) KF->GRwGt[i] = (double *) calloc(KF->Nx,sizeof(double));
      for(i=0;i<KF->Nx;i++) {
         for(j=0;j<KF->Nx;j++) {
            for(k=0;k<KF->Nw;k++) {
               KF->GRwGt[i][j] += GRw[i][k]*KF->Gamw[j][k];
            }
         }
      }
      for(i=0;i<KF->Nx;i++) free(GRw[i]);
      free(GRw);
}
/******************************************************************************/
void KalmanFilterMeasUpdate(struct KalmanFilterType *KF, struct KFMeasType *M)
{
      long i,j,k;
      double Det;
      
      /* H*P*HT + Rv */
      for(i=0;i<M->Ny;i++) {
         for(j=0;j<KF->Nx;j++) {
            M->HP[i][j] = 0.0;
            for(k=0;k<KF->Nx;k++) {
               M->HP[i][j] += M->H[i][k]*KF->P[k][j];
            }
         }
      }
      for(i=0;i<M->Ny;i++) {
         for(j=i;j<M->Ny;j++) {
            M->HPHtRv[i][j] = M->Rv[i][j];
            for(k=0;k<KF->Nx;k++) {
               M->HPHtRv[i][j] += M->HP[i][k]*M->H[j][k];
            }
            M->HPHtRv[j][i] = M->HPHtRv[i][j];
         }
      }
      
      /* Invert (HPHt+Rv) */
      if (M->Ny == 1) {
         if (M->HPHtRv[0][0] == 0.0) {
            printf("Attempted divide by zero in KalmanFilterMeasUpdate.\n");
            exit(1);
         }
         M->HPHtRvInv[0][0] = 1.0/M->HPHtRv[0][0];
      }
      else if (M->Ny == 2) {
         Det = M->HPHtRv[0][0]*M->HPHtRv[1][1]
             - M->HPHtRv[1][0]*M->HPHtRv[0][1];
         if (Det == 0.0) {
            printf("Singular matrix HPHtRv in KalmanFilterMeasUpdate.\n");
            exit(1);
         }
         M->HPHtRvInv[0][0] =  M->HPHtRv[1][1]/Det; 
         M->HPHtRvInv[0][1] = -M->HPHtRv[0][1]/Det; 
         M->HPHtRvInv[1][1] =  M->HPHtRv[0][0]/Det; 

         M->HPHtRvInv[1][0] =  M->HPHtRvInv[0][1]; 
      }
      else if (M->Ny == 3) {
         Det=M->HPHtRv[0][0]*(M->HPHtRv[1][1]*M->HPHtRv[2][2]-M->HPHtRv[2][1]*M->HPHtRv[1][2])
            +M->HPHtRv[0][1]*(M->HPHtRv[1][2]*M->HPHtRv[2][0]-M->HPHtRv[2][2]*M->HPHtRv[1][0])
            +M->HPHtRv[0][2]*(M->HPHtRv[1][0]*M->HPHtRv[2][1]-M->HPHtRv[2][0]*M->HPHtRv[1][1]);

         if (Det == 0.0) {
            printf("Singular matrix HPHtRv in KalmanFilterMeasUpdate.\n");
            exit(1);
         }

         M->HPHtRvInv[0][0]=(M->HPHtRv[1][1]*M->HPHtRv[2][2]
                            -M->HPHtRv[2][1]*M->HPHtRv[1][2])/Det;
         M->HPHtRvInv[0][1]=(M->HPHtRv[2][1]*M->HPHtRv[0][2]
                            -M->HPHtRv[0][1]*M->HPHtRv[2][2])/Det;
         M->HPHtRvInv[0][2]=(M->HPHtRv[0][1]*M->HPHtRv[1][2]
                            -M->HPHtRv[1][1]*M->HPHtRv[0][2])/Det;
         M->HPHtRvInv[1][1]=(M->HPHtRv[0][0]*M->HPHtRv[2][2]
                            -M->HPHtRv[2][0]*M->HPHtRv[0][2])/Det;
         M->HPHtRvInv[1][2]=(M->HPHtRv[1][0]*M->HPHtRv[0][2]
                            -M->HPHtRv[0][0]*M->HPHtRv[1][2])/Det;
         M->HPHtRvInv[2][2]=(M->HPHtRv[0][0]*M->HPHtRv[1][1]
                            -M->HPHtRv[1][0]*M->HPHtRv[0][1])/Det;         

         M->HPHtRvInv[1][0]=M->HPHtRvInv[0][1];
         M->HPHtRvInv[2][0]=M->HPHtRvInv[0][2];
         M->HPHtRvInv[2][1]=M->HPHtRvInv[1][2];
      }
      else {
         printf("Oops.  This Kalman Filter implementation assumes that \n");
         printf("measurements are of dimension 1, 2, or 3.  Bailing out.\n");
         exit(1);
      }
      
      /* L = (HP)^T*inv(HPHt+Rv) */
      for(i=0;i<KF->Nx;i++) {
         for(j=0;j<M->Ny;j++) {
            M->L[i][j] = 0.0;
            for(k=0;k<M->Ny;k++) {
               M->L[i][j] += M->HP[k][i]*M->HPHtRvInv[k][j];
            }
         }
      }
            
      /* Update x */
      for(i=0;i<M->Ny;i++) {
         M->Hx[i] = 0.0;
         for(j=0;j<KF->Nx;j++) M->Hx[i] += M->H[i][j]*KF->x[j];
      }
      for(i=0;i<KF->Nx;i++) {
         for(j=0;j<M->Ny;j++) {
            KF->x[i] += M->L[i][j]*(M->y[j] - M->Hx[j]);
         } 
      }
      
      /* Update P */
      for(i=0;i<KF->Nx;i++) {
         for(j=i;j<KF->Nx;j++) {
            for(k=0;k<M->Ny;k++) {
               KF->P[i][j] -= M->L[i][k]*M->HP[k][j];
            }
            KF->P[j][i] = KF->P[i][j];
         }
      }
}
/******************************************************************************/
void KalmanFilterTimeUpdate(struct KalmanFilterType *KF)
{
      long i,j,k;
      
      /* x(k+1) = Phi*x(k) + Gam*u(k) */
      for(i=0;i<KF->Nx;i++) {
         KF->PhiX[i] = 0.0;
         KF->GamU[i] = 0.0;
         for(j=0;j<KF->Nx;j++) {
            KF->PhiX[i] += KF->Phi[i][j]*KF->x[j];
         }
         for(j=0;j<KF->Nu;j++) {
            KF->GamU[i] += KF->Gam[i][j]*KF->u[j];
         }
      }
      for(i=0;i<KF->Nx;i++) KF->x[i] = KF->PhiX[i]+KF->GamU[i];

      /* P = Phi*P*PhiT + Gamw*Rw*GamwT */
      for(i=0;i<KF->Nx;i++) {
         for(j=0;j<KF->Nx;j++) {
            KF->PhiP[i][j] = 0.0;
            for(k=0;k<KF->Nx;k++) {
               KF->PhiP[i][j] += KF->Phi[i][k]*KF->P[k][j];
            }
         }
      }
      for(i=0;i<KF->Nx;i++) {
         for(j=i;j<KF->Nx;j++) {
            KF->P[i][j] = KF->GRwGt[i][j];
            for(k=0;k<KF->Nx;k++) {
               KF->P[i][j] += KF->PhiP[i][k]*KF->Phi[j][k];
            }
            KF->P[j][i] = KF->P[i][j];
         }
      }
}
/**********************************************************************/
/*  Ref Wie, "Singularity Escape/Avoidance Steering Logic for         */
/*  Control Moment Gyro Systems", JGCD, Sep-Oct 2005                  */
double CMGLaw4x1DOF(double Tcmd[3], double Axis[4][3], double Gim[4][3],
   double h[4], double AngRateCmd[4])
{
      double eps0 = 0.1;
      double lam0 = 0.01;
      double mu = 10.0;
      double w = 1.0;
      double dt = 0.1;
      double TwoPi = 6.2831853072;
      double A[3][4],AAt[3][3],Asharp[4][3],Gain;
      static double wt = 0.0;
      double lam,eps;
      double V[3][3],W[4][4],AW[3][4],Den[3][3],InvDen[3][3];

      long i,j;

      /* Output axis A = (gxa) */
      for(i=0;i<4;i++) {
         A[0][i] = (Gim[i][1]*Axis[i][2]-Gim[i][2]*Axis[i][1]);
         A[1][i] = (Gim[i][2]*Axis[i][0]-Gim[i][0]*Axis[i][2]);
         A[2][i] = (Gim[i][0]*Axis[i][1]-Gim[i][1]*Axis[i][0]);
      }

      for(i=0;i<3;i++) {
         for(j=0;j<3;j++) {
            AAt[i][j] = A[i][0]*A[j][0] + A[i][1]*A[j][1] +
                        A[i][2]*A[j][2] + A[i][3]*A[j][3];
         }
      }
      /* Gain measures proximity to singularities */
      Gain = AAt[0][0]*AAt[1][1]*AAt[2][2]
             + AAt[0][1]*AAt[1][2]*AAt[2][0]
             + AAt[0][2]*AAt[1][0]*AAt[2][1]
             - AAt[0][2]*AAt[1][1]*AAt[2][0]
             - AAt[0][1]*AAt[1][0]*AAt[2][2]
             - AAt[0][0]*AAt[1][2]*AAt[2][1];

      /* Singularity Avoidance */
      lam = lam0*exp(-mu*Gain);
      eps = eps0*sin(wt);
      wt += w*dt;
      wt = fmod(wt,TwoPi);

      /* V */
      for(i=0;i<3;i++) {
         for(j=0;j<i;j++) {
            V[i][j] = lam*eps;
            V[j][i] = V[i][j];
         }
         V[i][i] = lam;
      }
      /* W */
      for(i=0;i<4;i++) {
         for(j=0;j<i;j++) {
            W[i][j] = lam;
            W[j][i] = lam;
         }
         W[i][i] = 1.0;
      }

      /* AW */
      for(i=0;i<3;i++) {
         for(j=0;j<4;j++) {
            AW[i][j] = A[i][0]*W[0][j]+A[i][1]*W[1][j]+
                       A[i][2]*W[2][j]+A[i][3]*W[3][j];
         }
      }
      /* Den = AWAt + V*/
      for(i=0;i<3;i++) {
         for(j=0;j<3;j++) {
            Den[i][j] = AW[i][0]*A[j][0]+AW[i][1]*A[j][1]+
                         AW[i][2]*A[j][2]+AW[i][3]*A[j][3] + V[i][j];
         }
      }
      MINV3(Den,InvDen);

      /* Asharp = (AW)^T*inv(Den) */
      for(i=0;i<4;i++) {
         for(j=0;j<3;j++) {
            Asharp[i][j] = AW[0][i]*InvDen[0][j]+
                           AW[1][i]*InvDen[1][j]+
                           AW[2][i]*InvDen[2][j];
         }
      }

      for(i=0;i<4;i++) AngRateCmd[i] = -(Asharp[i][0]*Tcmd[0]
                                        +Asharp[i][1]*Tcmd[1]
                                        +Asharp[i][2]*Tcmd[2])/h[i];

      return(Gain);
}
/* #ifdef __cplusplus
** }
** #endif
*/

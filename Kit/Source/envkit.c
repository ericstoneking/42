/*    This file is distributed with 42,                               */
/*    the (mostly harmless) spacecraft dynamics simulation            */
/*    created by Eric Stoneking of NASA Goddard Space Flight Center   */

/*    Copyright 2010 United States Government                         */
/*    as represented by the Administrator                             */
/*    of the National Aeronautics and Space Administration.           */

/*    No copyright is claimed in the United States                    */
/*    under Title 17, U.S. Code.                                      */

/*    All Other Rights Reserved.                                      */


#include "envkit.h"

/* #ifdef __cplusplus
** namespace Kit {
** #endif
*/

#define D2R 1.74532925199E-2
#define PI 3.14159265358979323846
#define TWOPI 6.28318530718
/**********************************************************************/
/*  Earth Gravity Model 96, published by NIMA as part of the WGS84    */
void EGM96(const char *ModelPath, long N, long M, double mass, double pbn[3],
           double PriMerAng, double FgeoN[3])
{
      double Cbar[19][19],Sbar[19][19];
      double ReNorm;
      static double C[19][19],S[19][19];
      double CEN[3][3],cth,sth,cph,sph,pbe[3],gradV[3];
      double r,theta,phi,Fr,Fth,Fph,Fe[3];
      double dum1,dum2;
      double Re = 6378.145E3;
      double mu = 3.986004E14;
      long i,n,m;
      static long First = 1;
      FILE *EGM96file;

      if (First) {
         First = 0;
         /* Get data from EGM96.txt */
         EGM96file = FileOpen(ModelPath,"EGM96.txt","r");
         for(i=1;i<=187;i++){
             fscanf(EGM96file,"%ld %ld %lf %lf",&n,&m,&dum1,&dum2);
             Cbar[n][m]=dum1;
             Sbar[n][m]=dum2;
         }
         fclose(EGM96file);
         /* Transform from EGM96 normalization to Neumann normalization */
         for(n=2;n<=18;n++){
            for(m=0;m<=n;m++){
               ReNorm = sqrt((2*n+1)*fact(n-m)/fact(n+m));
               if (m!=0) ReNorm *= sqrt(2.0);
               C[n][m] = Cbar[n][m]*ReNorm;
               S[n][m] = Sbar[n][m]*ReNorm;
            }
         }
      }

/*    Transform p to spherical coords in Earth frame */
      CEN[0][0] = cos(PriMerAng);
      CEN[1][1] = CEN[0][0];
      CEN[2][2] = 1.0;
      CEN[0][1] = sin(PriMerAng);
      CEN[1][0] = -CEN[0][1];
      CEN[0][2] = 0.0;
      CEN[1][2] = 0.0;
      CEN[2][0] = 0.0;
      CEN[2][1] = 0.0;
      MxV(CEN,pbn,pbe);
      r = MAGV(pbe);
      theta = acos(pbe[2]/r);
      phi = atan2(pbe[1],pbe[0]);
      sth = sin(theta);
      cth = cos(theta);
      sph = sin(phi);
      cph = cos(phi);

/*    Find Fr, Fth, Fph */
      SphericalHarmonics(N,M,r,phi,theta,Re,mu/Re,C,S,gradV);
      Fr  = mass*gradV[0];
      Fth = mass*gradV[1];
      Fph = mass*gradV[2];
      
/*    Transform back to cartesian coords in Newtonian frame */
      Fe[0]=(Fr*sth+Fth*cth)*cph-Fph*sph;
      Fe[1]=(Fr*sth+Fth*cth)*sph+Fph*cph;
      Fe[2]=Fr*cth-Fth*sth;

      MTxV(CEN,Fe,FgeoN);
}
/**********************************************************************/
/*  Goddard Mars Model 2B.  Truncated to 18x18                        */
/*  Ref:  http://bowie.gsfc.nasa.gov/926/MARS/GMM2B.html              */
void GMM2B(const char *ModelPath, long N, long M, double mass, double pbn[3],
           double PriMerAng, double FgeoN[3])
{
      double Cbar[19][19],Sbar[19][19];
      double ReNorm;
      static double C[19][19],S[19][19];
      double CEN[3][3],cth,sth,cph,sph,pbe[3],gradV[3];
      double r,theta,phi,Fr,Fth,Fph,Fe[3];
      double dum1,dum2;
      double Re = 3397.0E3;
      double mu = 4.28283719E13;
      long i,n,m;
      static long First = 1;
      FILE *GMM2Bfile;

      if (First) {
         First = 0;
         /* Get data from GMM2B.txt */
         GMM2Bfile = FileOpen(ModelPath,"GMM2B.txt","r");
         for(i=1;i<=187;i++){
             fscanf(GMM2Bfile,"%ld %ld %lf %lf",&n,&m,&dum1,&dum2);
             Cbar[n][m]=dum1;
             Sbar[n][m]=dum2;
         }
         fclose(GMM2Bfile);
         /* Transform from GMM2B normalization to Neumann normalization */
         for(n=2;n<=18;n++){
            for(m=0;m<=n;m++){
               ReNorm = sqrt((2*n+1)*fact(n-m)/fact(n+m));
               if (m!=0) ReNorm *= sqrt(2.0);
               C[n][m] = Cbar[n][m]*ReNorm;
               S[n][m] = Sbar[n][m]*ReNorm;
            }
         }
      }

/*    Transform p to spherical coords in Mars N frame */
      CEN[0][0] = cos(PriMerAng);
      CEN[1][1] = CEN[0][0];
      CEN[2][2] = 1.0;
      CEN[0][1] = sin(PriMerAng);
      CEN[1][0] = -CEN[0][1];
      CEN[0][2] = 0.0;
      CEN[1][2] = 0.0;
      CEN[2][0] = 0.0;
      CEN[2][1] = 0.0;
      MxV(CEN,pbn,pbe);
      r = MAGV(pbe);
      theta = acos(pbe[2]/r);
      phi = atan2(pbe[1],pbe[0]);
      sth = sin(theta);
      cth = cos(theta);
      sph = sin(phi);
      cph = cos(phi);

/*    Find Fr, Fth, Fph */
      SphericalHarmonics(N,M,r,phi,theta,Re,mu/Re,C,S,gradV);
      Fr  = mass*gradV[0];
      Fth = mass*gradV[1];
      Fph = mass*gradV[2];

/*    Transform back to cartesian coords in Newtonian frame */
      Fe[0]=(Fr*sth+Fth*cth)*cph-Fph*sph;
      Fe[1]=(Fr*sth+Fth*cth)*sph+Fph*cph;
      Fe[2]=Fr*cth-Fth*sth;

      MTxV(CEN,Fe,FgeoN);
}
/**********************************************************************/
/*  Goddard Luna Gravity Model 2.  Truncated to 18x18                 */
/*  Ref:  http://pds-geosciences.wustl.edu/geodata/clem1-gravity-topo-v1/cl_xxxx/gravity/glgm2sh.tab            */
void GLGM2(const char *ModelPath, long N, long M, double mass, double pbn[3],
           double PriMerAng, double FgeoN[3])
{
      double Cbar[19][19],Sbar[19][19];
      double ReNorm;
      static double C[19][19],S[19][19];
      double CEN[3][3],cth,sth,cph,sph,pbe[3],gradV[3];
      double r,theta,phi,Fr,Fth,Fph,Fe[3];
      double dum1,dum2;
      double Re = 1738.0E3;
      double mu = 4.903E12;
      long i,n,m;
      static long First = 1;
      FILE *GLGM2file;

      if (First) {
         First = 0;
         /* Get data from GLGM2.txt */
         GLGM2file = FileOpen(ModelPath,"GLGM2.txt","r");
         for(i=1;i<=187;i++){
             fscanf(GLGM2file,"%ld %ld %lf %lf",&n,&m,&dum1,&dum2);
             Cbar[n][m]=dum1;
             Sbar[n][m]=dum2;
         }
         fclose(GLGM2file);
         /* Transform from GLGM2 normalization to Neumann normalization */
         for(n=2;n<=18;n++){
            for(m=0;m<=n;m++){
               ReNorm = sqrt((2*n+1)*fact(n-m)/fact(n+m));
               if (m!=0) ReNorm *= sqrt(2.0);
               C[n][m] = Cbar[n][m]*ReNorm;
               S[n][m] = Sbar[n][m]*ReNorm;
            }
         }
      }

/*    Transform p to spherical coords in Luna N frame */
      CEN[0][0] = cos(PriMerAng);
      CEN[1][1] = CEN[0][0];
      CEN[2][2] = 1.0;
      CEN[0][1] = sin(PriMerAng);
      CEN[1][0] = -CEN[0][1];
      CEN[0][2] = 0.0;
      CEN[1][2] = 0.0;
      CEN[2][0] = 0.0;
      CEN[2][1] = 0.0;
      MxV(CEN,pbn,pbe);
      r = MAGV(pbe);
      theta = acos(pbe[2]/r);
      phi = atan2(pbe[1],pbe[0]);
      sth = sin(theta);
      cth = cos(theta);
      sph = sin(phi);
      cph = cos(phi);

/*    Find Fr, Fth, Fph */
      SphericalHarmonics(N,M,r,phi,theta,Re,mu/Re,C,S,gradV);
      Fr  = mass*gradV[0];
      Fth = mass*gradV[1];
      Fph = mass*gradV[2];

/*    Transform back to cartesian coords in Newtonian frame */
      Fe[0]=(Fr*sth+Fth*cth)*cph-Fph*sph;
      Fe[1]=(Fr*sth+Fth*cth)*sph+Fph*cph;
      Fe[2]=Fr*cth-Fth*sth;

      MTxV(CEN,Fe,FgeoN);
}
/**********************************************************************/
/*  IGRF Magnetic field model                                      *  */
void IGRFMagField(const char *ModelPath, long N, long M, double pbn[3],double PriMerAng,
             double MagVecN[3])
{
      double g[11][11],h[11][11];
      double ReNorm;
      static double C[19][19],S[19][19];
      double cth,sth,cph,sph,pbe[3],gradV[3];
      double r,theta,phi,Br,Bth,Bph,BVE[3];
      double dum1,dum2;
      double AXIS[3] = {0.0,0.0,1.0};
      double CEN[3][3];
      static double Re;
      long i,n,m;
      static long First = 1;
      FILE *IGRFfile;
      char junk[120],newline;

      if (First) {
         First = 0;
         /* Get data from IGRF20.txt */
         IGRFfile = FileOpen(ModelPath,"igrf20.txt","r");
         fscanf(IGRFfile,"%[^\n] %[\n]",junk,&newline);
         fscanf(IGRFfile,"%lf %lf %lf",
                &dum1,&Re,&dum2);
         Re *= 1.0E3;  /* Convert from km to m */
         for(i=1;i<=65;i++){
             fscanf(IGRFfile,"%ld %ld %lf %lf",
                     &n,&m,&dum1,&dum2);
             g[n][m]=dum1;
             h[n][m]=dum2;
         }
         fclose(IGRFfile);
         /* Transform from Schmidt normalization to Neumann normalization */
         for(n=1;n<=10;n++){
            for(m=0;m<=n;m++){
               ReNorm = sqrt(fact(n-m)/fact(n+m));
               if (m!=0) ReNorm *= sqrt(2.0);
               C[n][m] = g[n][m]*ReNorm;
               S[n][m] = h[n][m]*ReNorm;
            }
         }
      }

      SimpRot(AXIS,PriMerAng,CEN);

/*    Transform p to spherical coords in Earth frame */
      MxV(CEN,pbn,pbe);
      r = MAGV(pbe);
      theta = acos(pbe[2]/r);
      phi = atan2(pbe[1],pbe[0]);
      sth = sin(theta);
      cth = cos(theta);
      sph = sin(phi);
      cph = cos(phi);

/*    Find Br, Bth, Bph */
      SphericalHarmonics(N,M,r,phi,theta,Re,Re,C,S,gradV);
      Br  = -gradV[0];
      Bth = -gradV[1];
      Bph = -gradV[2];

/*    Transform back to cartesian coords in Newtonian frame */
/*    and convert from nanoTesla to Tesla */
      BVE[0]=1.0E-9*((Br*sth+Bth*cth)*cph-Bph*sph);
      BVE[1]=1.0E-9*((Br*sth+Bth*cth)*sph+Bph*cph);
      BVE[2]=1.0E-9*(Br*cth-Bth*sth);

      MTxV(CEN,BVE,MagVecN);

      /*printf("r,phi,theta: %lf %lf %lf\n",r,phi,theta);
      **printf("Br,Bth,Bph: %lf %lf %lf\n",Br,Bth,Bph);
      **printf("BVE: %lf %lf %lf\n\n",BVE[0],BVE[1],BVE[2]);
      */
}
/**********************************************************************/
/*  Computes planetary dipole magnetic field vector at S/C position.  */
void DipoleMagField(double DipoleMoment, double DipoleAxis[3],
                    double DipoleOffset[3],double p[3],
                    double PriMerAng,double MagVecN[3])
{
      double R[3],PCN[3],R3,MN[3],MoR,CEN[3][3];
      double AXIS[3] = {0.0,0.0,1.0};
      long i;

      SimpRot(AXIS,PriMerAng,CEN);

      MTxV(CEN,DipoleOffset,PCN);
      for(i=0;i<3;i++) R[i]=p[i]-PCN[i];
      R3 = UNITV(R);
      R3 =R3*R3*R3;
      MTxV(CEN,DipoleAxis,MN);
      MoR = VoV(MN,R);
      for(i=0;i<3;i++) MagVecN[i]=DipoleMoment/R3*(3.0*MoR*R[i]-MN[i]);

}
/**********************************************************************/
/* Ref: Rostoker, "Geomagnetic Indices", Rev. of Geophysics and       */
/* Space Physics, Vol 10, No. 4, pp. 935-950, Nov 1972.               */
/* Kp is in the scale [0o 0+ 1- 1o 1+ ... 8+ 9- 9o].                  */
/* We map it to [0.0 0.33 0.67 1.0 1.33 ... 8.33 8.67 9.0] for ease   */
/* of table lookup.                                                   */
double KpToAp(double Kp)
{
      long k;
      double Ap[28] = {  0,  2,  3,  4,  5,  6,  7,
                         9, 12, 15, 18, 22, 27, 32,
                        39, 48, 56, 67, 80, 94,111,
                       132,154,179,207,236,300,400};
      
      k = (long) (3.0*Kp+0.5); /* Round to the nearest 1/3 */
      if (k < 0) k = 0;
      if (k > 27) k = 27;
      
      return(Ap[k]);
}/**********************************************************************/
/*                                                                    */
/* This is an atmospheric density model, described in  "Models of     */
/* Earth's Atmosphere", NASA SP-8021, May 1969.  It is a modification */
/* of the Jacchia model.  Range of validity is from 120 km to 1000 km */
/* altitude.                                                          */
double JacchiaRoberts(double pbn[3],double svn[3],double F10p7,double Ap)
{
      #define ERAD 6378.145E3
      double N[5],Fbar,F,logT,sinth25;
      double W[5] = {1.6731E-24,6.6435E-24,4.6496E-23,
                     5.3104E-23,2.6552E-23};
      double Lat,RAP,z,DS,RAS,HRA,TAU,T[5],fDD,th,A,X,S,dH,Q,P,B,TD,c;
      double T4_800,p;
      double density;

/* Fbar should properly be an 81-day average.  Here it's assumed to   */
/* equal to the instantaneous F10p7                                   */
      Fbar=F10p7;
      F=F10p7;

      p = MAGV(pbn);
      Lat=asin(pbn[2]/p);
      if (pbn[1]==0.0 && pbn[0]==0.0)
         RAP = 0.0;
      else
         RAP=atan2(pbn[1],pbn[0]);

/*    Find Geometric Altitude, z, in km */
      z = 1.0E-3*(p - ERAD);
      if (z < 120.0) {
         printf("Altitude %f km too low for Jacchia-Roberts\n",z);
         exit(1);
      }
      else if (z > 1000.0) density = 0.0; /* Beyond range of model */
      else {

/*       Find E-W separation of sun and computation point */
         DS=asin(svn[2]);
         RAS=atan2(svn[1],svn[0]);
         HRA = RAP - RAS;

/*       Temperature Computation */
         TAU = HRA - 0.25*PI + PI/15.0*sin(HRA+0.25*PI);
         if (TAU > PI) TAU -= TWOPI;
         if (TAU < -PI) TAU += TWOPI;

         T[0] = 362.0+3.6*Fbar;
         T[1] = T[0]+1.8*(F-Fbar);
         fDD=(0.37+0.14*sin(RAS-1.222))*sin(2.0*RAS+0.723);
         T[2] = T[1]+fDD*Fbar;
         th=0.5*fabs(Lat+DS);
         sinth25=pow(sin(th),2.5);
         A=0.28*(pow(cos(0.5*(Lat-DS)),2.5)-sinth25)
              /(1.0+0.28*sinth25);
         c=cos(0.5*TAU);
         T[3]=T[2]*(1.0+0.28*sinth25)*(1.0+A*c*c*sqrt(c));
         T[4]=T[3]+Ap+100.0*(1.0-exp(-0.08*Ap));

         T4_800 = T[4] - 800.0;
         X=(T4_800)/(750.0+1.722E-4*T4_800*T4_800);
         S=1.5E-4+0.0291*exp(-0.5*X*X);
         dH=(z-120.0)*(ERAD+120.0E3)/p;

/*       Number Density Computations */
         Q = 1.13619033/(S*T[4]);
         P = (T[4]-355.0)/T[4];
         B = (1.0-P)/(1.0-P*exp(-S*dH));
/*       Hydrogen */
         TD =  ((((9.753963073E-16*T[4]-7.577509214E-12)*T[4]
               +2.341193059E-8)*T[4]-3.62095821E-5)*T[4]
               +2.844291123E-2)*T[4]-10.48947029;
         logT=log10(T[4]);
         N[0] = pow(10.0,73.13-39.4*logT+5.5*logT*logT);
         if (z > 500.0)
            N[0] *= pow(B,1.0+TD+1.008*Q)*exp(-1.008*S*Q*dH);
/*       Helium */
         N[1]=3.4E7*pow(B,0.63+4.002*Q)*exp(-4.002*S*Q*dH);
/*       Nitrogen */
         N[2]=4.0E11*pow(B,1.0+28.0134*Q)*exp(-28.0134*S*Q*dH);
/*       Oxygen */
         N[3]=7.5E10*pow(B,1.0+31.9988*Q)*exp(-31.9988*S*Q*dH);
/*       Atomic Oxygen */
         N[4]=7.6E10*pow(B,1.0+15.9990*Q)*exp(-15.9990*S*Q*dH);

/*       Mass Density */
         density = 1.0E3*(N[0]*W[0]+N[1]*W[1]+N[2]*W[2]+N[3]*W[3]+N[4]*W[4]);

      }
      return(density);
}
/**********************************************************************/
/*  The tabulated data is taken from a table in the back of           */
/*  Wertz and Larson, "SMAD", Third Edition.  The data is based       */
/*  on MSIS-86, averaged to be a function of altitude only, and       */
/*  F10.7 chosen so that the density is according to the column.      */
/*  Col = 0 (Min), 1, (Mean), or 2 (Max) as defined in table.         */
double SimpleMSIS(double pbn[3], long Col)
{
      double AltTable[22] = {0.0,100.0,150.0,200.0,250.0,300.0,350.0,
                             400.0,450.0,500.0,550.0,600.0,650.0,
                             700.0,750.0,800.0,850.0,900.0,950.0,
                             1000.0,1250.0,1500.0};
      double RhoTable[22][3] = { {1.2,1.2,1.2},
                                 {4.61E-7,4.79E-7,5.10E-7},
                                 {1.65E-9,1.81E-9,2.04E-9},
                                 {1.78E-10,2.53E-10,3.52E-10},
                                 {3.35E-11,6.24E-11,1.06E-10},
                                 {8.19E-12,1.95E-11,3.96E-11},
                                 {2.34E-12,6.98E-12,1.66E-11},
                                 {7.32E-13,2.72E-12,7.55E-12},
                                 {2.47E-13,1.13E-12,3.61E-12},
                                 {8.98E-14,4.89E-13,1.80E-12},
                                 {3.63E-14,2.21E-13,9.25E-13},
                                 {1.68E-14,1.04E-13,4.89E-13},
                                 {9.14E-15,5.15E-14,2.64E-13},
                                 {5.74E-15,2.72E-14,1.47E-13},
                                 {3.99E-15,1.55E-14,8.37E-14},
                                 {2.96E-15,9.63E-15,4.39E-14},
                                 {2.28E-15,6.47E-15,3.00E-14},
                                 {1.80E-15,4.66E-15,1.91E-14},
                                 {1.44E-15,3.54E-15,1.27E-14},
                                 {1.17E-15,2.79E-15,8.84E-15},
                                 {4.67E-16,1.11E-15,2.59E-15},
                                 {2.30E-16,5.21E-16,1.22E-15}};

      double EarthRad = 6378.145E3;
      double Alt,a;
      long i1,i2;
      double density;

      if (Col < 0 || Col > 2) {
         printf("Column %ld out of range in SimpleMSIS.  Bailing out.\n",Col);
         exit(1);
      }

      Alt = 1.0E-3*(MAGV(pbn)-EarthRad);
      if (Alt < 0.0) density = 1.2;
      else if (Alt >= 1500.0) density = 0.0;
      else {
         i2 = 1;
         while (Alt > AltTable[i2]) i2++;
         i1 = i2-1;
         /* Interpolate density logarithmically */
         a = (Alt-AltTable[i1])/(AltTable[i2]-AltTable[i1]);
         density = exp((1.0-a)*log(RhoTable[i1][Col])+a*log(RhoTable[i2][Col]));
      }
      return(density);
}
/**********************************************************************/
/* This simple model is taken from                                    */
/* http://www.grc.nasa.gov/WWW/K-12/airplane/atmosmrm.html            */
double MarsAtmosphereModel(double r[3])
{
      double Alt,T,p;
      double MarsRad = 3410.0E3;
      double Density;

      Alt = MAGV(r) - MarsRad;
      if (Alt < 150.0E3) {
         if (Alt < 7000.0) {
            T = -23.4 - 0.00222*Alt;
         }
         else {
            T = -31.0 - 9.98E-4*Alt;
         }
         p = 1.0E3*0.699*exp(-9.0E-5*Alt); /* p in Pa */
         Density = p/(0.1921*(T+273.1));
      }
      else Density = 0.0;

      return(Density);
}
/**********************************************************************/
/* Find coordinate transformation from True Equator True Equinox      */
/* (TETE) frame to J2000 frame.  Ref "The Astronomical Almanac",      */
/* QB8.U5, 2003, p. B18,B20.                                          */
/* TEME to TETE rotation from Montenbruck adn Gill (TL1080.M66)       */
void SimpleEarthPrecNute(double JD,double C_TEME_TETE[3][3],
                                   double C_TETE_J2000[3][3])
{
      double d,arg1,arg2,dpsi,deps,eps,N[3][3];
      double T,z,theta,zeta,P[3][3];
      double c1,s1,c2,s2,c3,s3;
      double dR;

      /* TETE to MEME (Nutation) */
      d = JD - 2452639.5;
      arg1 = (67.1 - 0.053*d)*D2R;
      arg2 = (198.5 + 1.971*d)*D2R;
      dpsi = (-0.0048*sin(arg1)-0.0004*sin(arg2))*D2R;
      deps = (0.0026*cos(arg1)+0.0002*cos(arg2))*D2R;
      eps = 23.44*D2R;

      N[0][0] = 1.0;
      N[1][1] = 1.0;
      N[2][2] = 1.0;
      N[2][1] = deps;
      N[2][0] = dpsi*sin(eps);
      N[1][0] = dpsi*cos(eps);
      N[1][2] = -N[2][1];
      N[0][2] = -N[2][0];
      N[0][1] = -N[1][0];

      /* MEME to J2000 (Precession) */
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
      P[0][0] =  c1*c2*c3-s3*s1;
      P[1][0] = -c1*c2*s3-c3*s1;
      P[2][0] =  c1*s2;
      P[0][1] =  s1*c2*c3+s3*c1;
      P[1][1] = -s1*c2*s3+c3*c1;
      P[2][1] =  s1*s2;
      P[0][2] = -s2*c3;
      P[1][2] =  s2*s3;
      P[2][2] =  c2;

      /* TETE to J2000 (Precession, then Nutation) */
      C_TETE_J2000[0][0] = N[0][0]*P[0][0]+N[0][1]*P[1][0]+N[0][2]*P[2][0];
      C_TETE_J2000[0][1] = N[0][0]*P[0][1]+N[0][1]*P[1][1]+N[0][2]*P[2][1];
      C_TETE_J2000[0][2] = N[0][0]*P[0][2]+N[0][1]*P[1][2]+N[0][2]*P[2][2];
      C_TETE_J2000[1][0] = N[1][0]*P[0][0]+N[1][1]*P[1][0]+N[1][2]*P[2][0];
      C_TETE_J2000[1][1] = N[1][0]*P[0][1]+N[1][1]*P[1][1]+N[1][2]*P[2][1];
      C_TETE_J2000[1][2] = N[1][0]*P[0][2]+N[1][1]*P[1][2]+N[1][2]*P[2][2];
      C_TETE_J2000[2][0] = N[2][0]*P[0][0]+N[2][1]*P[1][0]+N[2][2]*P[2][0];
      C_TETE_J2000[2][1] = N[2][0]*P[0][1]+N[2][1]*P[1][1]+N[2][2]*P[2][1];
      C_TETE_J2000[2][2] = N[2][0]*P[0][2]+N[2][1]*P[1][2]+N[2][2]*P[2][2];

      /* TEME to TETE (Projection) */
      dR = atan(tan(dpsi)*cos(eps));
      c1 = cos(dR);
      s1 = sin(dR);
      C_TEME_TETE[0][0] = c1;
      C_TEME_TETE[1][0] = -s1;
      C_TEME_TETE[2][0] = 0.0;
      C_TEME_TETE[0][1] = s1;
      C_TEME_TETE[1][1] = c1;
      C_TEME_TETE[2][1] = 0.0;
      C_TEME_TETE[0][2] = 0.0;
      C_TEME_TETE[1][2] = 0.0;
      C_TEME_TETE[2][2] = 1.0;

}
/**********************************************************************/
/* Ref Montenbruck and Gill, "Satellite Orbits: Models, Methods,      */
/* Applications", TL1080.M66                                          */
void HiFiEarthPrecNute(double JD,double C_TEME_TETE[3][3],
                                 double C_TETE_J2000[3][3])
{

      double P[3][3],N[3][3];
      static long First = 1;
      static double Al,Bl,Alp,Blp,AF,BF,AD,BD,AOm,BOm;
      static double A2R;
      long i;
      double T,zeta,z,theta;
      double cos_zeta,sin_zeta,cos_theta,sin_theta,cos_z,sin_z;
      double dpsi,de,l,lp,F,D,Om,phi,e,ep;
      double cos_e,sin_e,cos_ep,sin_ep,cos_dpsi,sin_dpsi;
      double dR,cos_dR,sin_dR;

      static double pl[106] = { 0, 0,-2, 2,-2, 1, 0, 2, 0, 0,
                                0, 0, 0, 2, 0, 0, 0, 0, 0,-2,
                                0, 2, 0, 1, 2, 0, 0, 0,-1, 0,
                                0, 1, 0, 1, 1,-1, 0, 1,-1,-1,
                                1, 0, 2, 1, 2, 0,-1,-1, 1,-1,
                                1, 0, 0, 1, 1, 2, 0, 0, 1, 0,
                                1, 2, 0, 1, 0, 1, 1, 1,-1,-2,
                                3, 0, 1,-1, 2, 1, 3, 0,-1, 1,
                               -2,-1, 2, 1, 1,-2,-1, 1, 2, 2,
                                1, 0, 3, 1, 0,-1, 0, 0, 0, 1,
                                0, 1, 1, 2, 0, 0};
      static double plp[106] = { 0, 0, 0, 0, 0,-1,-2, 0, 0, 1,
                                 1,-1, 0, 0, 0, 2, 1, 2,-1, 0,
                                -1, 0, 1, 0, 1, 0, 1, 1, 0, 1,
                                 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                 1, 1,-1, 0, 0, 0, 0, 0, 0, 0,
                                -1, 0, 1, 0, 0, 1, 0,-1,-1, 0,
                                 0,-1, 1, 0, 0, 0, 0, 0, 0, 0,
                                 0, 0, 0, 1, 0, 0, 0,-1, 0, 0,
                                 0, 0, 0, 0, 1,-1, 0, 0, 1, 0,
                                -1, 1, 0, 0, 0, 1};
      static double pD[106] = { 0, 0, 2,-2, 2, 0, 2,-2, 2, 0,
                                2, 2, 2, 0, 2, 0, 0, 2, 0, 0,
                                2, 0, 2, 0, 0,-2,-2, 0, 0, 2,
                                2, 0, 2, 2, 0, 2, 0, 0, 0, 2,
                                2, 2, 0, 2, 2, 2, 2, 0, 0, 2,
                                0, 2, 2, 2, 0, 2, 0, 2, 2, 0,
                                0, 2, 0,-2, 0, 0, 2, 2, 2, 0,
                                2, 2, 2, 2, 0, 0, 0, 2, 0, 0,
                                2, 2, 0, 2, 2, 2, 4, 0, 2, 2,
                                0, 4, 2, 2, 2, 0,-2, 2, 0,-2,
                                2, 0,-2, 0, 2, 0};
      static double pF[106] = { 0, 0, 0, 0, 0,-1,-2, 0,-2, 0,
                               -2,-2,-2,-2,-2, 0, 0,-2, 0, 2,
                               -2,-2,-2,-1,-2, 2, 2, 0, 1,-2,
                                0, 0, 0, 0,-2, 0, 2, 0, 0, 2,
                                0, 2, 0,-2, 0, 0, 0, 2,-2, 2,
                               -2, 0, 0, 2, 0,-2, 2, 2,-2,-2,
                                0, 0,-2, 0, 1, 0, 0, 0, 2, 0,
                                0, 2, 0,-2, 0, 0, 0, 1, 0,-4,
                                2, 4,-4,-2, 2, 4, 0,-2,-2, 2,
                                2,-2,-2,-2, 0, 2, 0,-1, 2,-2,
                                0,-2, 2, 2, 4, 1};
      static double pOm[106] = { 1, 2, 1, 0, 2, 0, 1, 1, 2, 0,
                                 2, 2, 1, 0, 0, 0, 1, 2, 1, 1,
                                 1, 1, 1, 0, 0, 1, 0, 2, 1, 0,
                                 2, 0, 1, 2, 0, 2, 0, 1, 1, 2,
                                 1, 2, 0, 2, 2, 0, 1, 1, 1, 1,
                                 0, 2, 2, 2, 0, 2, 1, 1, 1, 1,
                                 0, 1, 0, 0, 0, 0, 0, 2, 2, 1,
                                 2, 2, 2, 1, 1, 2, 0, 2, 2, 0,
                                 2, 2, 0, 2, 1, 2, 2, 0, 1, 2,
                                 1, 2, 2, 0, 1, 1, 1, 2, 0, 0,
                                 1, 1, 0, 0, 2, 0};
      static double dp0[106] = {-171996,2062,  46,    11,  -3,
                                     -3,  -2,   1,-13187,1426,
                                   -517, 217, 129,    48, -22,
                                     17, -15, -16,   -12,  -6,
                                     -5,   4,   4,    -4,   1,
                                      1,  -1,   1,     1,  -1,
                                  -2274, 712,-386,  -301,-158,
                                    123,  63,  63,   -58, -59,
                                    -51, -38,  29,    29, -31,
                                     26,  21,  16,   -13, -10,
                                     -7,   7,  -7,    -8,   6,
                                      6,  -6,  -7,     6,  -5,
                                      5,  -5,  -4,     4,  -4,
                                     -3,   3,  -3,    -3,  -2,
                                     -3,  -3,   2,    -2,   2,
                                     -2,   2,   2,     1,  -1,
                                      1,  -2,  -1,     1,  -1,
                                     -1,   1,   1,     1,  -1,
                                     -1,   1,   1,    -1,   1,
                                      1,  -1,  -1,    -1,  -1,
                                     -1,  -1,  -1,     1,  -1, 1};
      static double dp1[40] = {-174.2, 0.2,   0,   0,   0,
                                    0,   0,   0,-1.6,-3.4,
                                  1.2,-0.5, 0.1,   0,   0,
                                 -0.1,   0, 0.1,   0,   0,
                                    0,   0,   0,   0,   0,
                                    0,   0,   0,   0,   0,
                                 -0.2, 0.1,-0.4,   0,   0,
                                    0,   0, 0.1,-0.1,   0};
      static double de0[106] = {92025,-895,-24,   0, 1,
                                    0,   1,  0,5736,54,
                                  224, -95,-70,   1, 0,
                                    0,   9,  7,   6, 3,
                                    3,  -2, -2,   0, 0,
                                    0,   0,  0,   0, 0,
                                  977,  -7,200, 129,-1,
                                  -53,  -2,-33,  32,26,
                                   27,  16, -1, -12,13,
                                   -1, -10, -8,   7, 5,
                                    0,  -3,  3,   3, 0,
                                   -3,   3,  3,  -3, 3,
                                    0,   3,  0,   0, 0,
                                    0,   0,  1,   1, 1,
                                    1,   1, -1,   1,-1,
                                    1,   0, -1,  -1, 0,
                                   -1,   1,  0,  -1, 1,
                                    1,   0,  0,  -1, 0,
                                    0,   0,  0,   0, 0,
                                    0,   0,  0,   0, 0,
                                    0,   0,  0,   0, 0, 0};
      static double de1[40] = {8.9, 0.5, 0,  0,    0,
                                 0,   0, 0,-3.1,-0.1,
                              -0.6, 0.3, 0,   0,   0,
                                 0, 0,   0,   0,   0,
                                 0, 0,   0,   0,   0,
                                 0, 0,   0,   0,   0,
                              -0.5, 0,   0,-0.1,   0,
                                 0, 0,   0,   0,   0};

      if (First) {
         First = 0;
         A2R = D2R/3600.0;
         Al = 134.0*3600.0+57.0*60.0+46.733;
         Bl = 477198.0*3600.0+52*60.0+2.633;
         Alp = 357.0*3600.0+31.0*60.0+39.804;
         Blp = 35999.0*3600.0+3.0*60.0+1.224;
         AF = 93.0*3600.0+16.0*60.0+18.877;
         BF = 483202.0*3600.0+1.0*60.0+3.137;
         AD = 297.0*3600.0+51.0*60.0+1.307;
         BD = 445267.0*3600.0+6.0*60.0+41.328;
         AOm = 125.0*3600.0+2.0*60+40.280;
         BOm = -(1934.0*3600.0+8.0*60.0+10.539);
      }

      T = (JD - 2451545.0)/36525.0;
      zeta  = (2306.2181+(0.30188+0.017998*T)*T)*T*A2R;
      theta = (2004.3109-(0.42665+0.041833*T)*T)*T*A2R;
      z     = zeta + (0.79280+0.000205*T)*T*T*A2R;

      cos_zeta = cos(zeta);
      sin_zeta = sin(zeta);
      cos_theta = cos(theta);
      sin_theta = sin(theta);
      cos_z = cos(z);
      sin_z = sin(z);
      P[0][0] = -sin_z*sin_zeta+cos_zeta*cos_theta*cos_z;
      P[1][0] = cos_z*sin_zeta+sin_z*cos_theta*cos_zeta;
      P[2][0] = sin_theta*cos_zeta;
      P[0][1] = -sin_z*cos_zeta-cos_z*cos_theta*sin_zeta;
      P[1][1] = cos_z*cos_zeta-sin_z*cos_theta*sin_zeta;
      P[2][1] = -sin_theta*sin_zeta;
      P[0][2] = -cos_z*sin_theta;
      P[1][2] = -sin_z*sin_theta;
      P[2][2] = cos_theta;

      dpsi = 0.0;
      de = 0.0;
      l =  Al+(Bl+(31.310+0.064*T)*T)*T;
      lp = Alp+(Blp+(-0.577-0.012*T)*T)*T;
      F =  AF+(BF+(-13.257+0.011*T)*T)*T;
      D =  AD+(BD+(-6.891+0.019*T)*T)*T;
      Om = AOm+(BOm+(7.455+0.008*T)*T)*T;
      for(i=0;i<40;i++) {
         phi = (pl[i]*l+plp[i]*lp+pD[i]*D+pF[i]*F+pOm[i]*Om)*A2R;
         dpsi += (dp0[i]+dp1[i]*T)*sin(phi);
         de   += (de0[i]+de1[i]*T)*cos(phi);
      }
      for(i=40;i<106;i++) {
         phi = (pl[i]*l+plp[i]*lp+pF[i]*F+pD[i]*D+pOm[i]*Om)*A2R;
         dpsi += dp0[i]*sin(phi);
         de   += de0[i]*cos(phi);
      }
      dpsi *= 1.0E-4*A2R;
      de *= 1.0E-4*A2R;

      e = (23.43929111+(-46.8150+(-0.00059+0.001813*T)*T)*T/3600.0)*D2R;
      ep = e + de;

      cos_e = cos(e);
      sin_e = sin(e);
      cos_ep = cos(ep);
      sin_ep = sin(ep);
      cos_dpsi = cos(dpsi);
      sin_dpsi = sin(dpsi);
      N[0][0] = cos_dpsi;
      N[1][0] = cos_ep*sin_dpsi;
      N[2][0] = sin_ep*sin_dpsi;
      N[0][1] = -cos_e*sin_dpsi;
      N[1][1] = cos_e*cos_ep*cos_dpsi+sin_e*sin_ep;
      N[2][1] = cos_e*sin_ep*cos_dpsi-sin_e*cos_ep;
      N[0][2] = -sin_e*sin_dpsi;
      N[1][2] = sin_e*cos_ep*cos_dpsi-cos_e*sin_ep;
      N[2][2] = sin_e*sin_ep*cos_dpsi+cos_e*cos_ep;

      C_TETE_J2000[0][0] = N[0][0]*P[0][0]+N[0][1]*P[1][0]+N[0][2]*P[2][0];
      C_TETE_J2000[0][1] = N[0][0]*P[0][1]+N[0][1]*P[1][1]+N[0][2]*P[2][1];
      C_TETE_J2000[0][2] = N[0][0]*P[0][2]+N[0][1]*P[1][2]+N[0][2]*P[2][2];
      C_TETE_J2000[1][0] = N[1][0]*P[0][0]+N[1][1]*P[1][0]+N[1][2]*P[2][0];
      C_TETE_J2000[1][1] = N[1][0]*P[0][1]+N[1][1]*P[1][1]+N[1][2]*P[2][1];
      C_TETE_J2000[1][2] = N[1][0]*P[0][2]+N[1][1]*P[1][2]+N[1][2]*P[2][2];
      C_TETE_J2000[2][0] = N[2][0]*P[0][0]+N[2][1]*P[1][0]+N[2][2]*P[2][0];
      C_TETE_J2000[2][1] = N[2][0]*P[0][1]+N[2][1]*P[1][1]+N[2][2]*P[2][1];
      C_TETE_J2000[2][2] = N[2][0]*P[0][2]+N[2][1]*P[1][2]+N[2][2]*P[2][2];

      /* TEME to TETE (Projection) */
      dR = atan(sin_dpsi/cos_dpsi*cos_ep);
      cos_dR = cos(dR);
      sin_dR = sin(dR);
      C_TEME_TETE[0][0] = cos_dR;
      C_TEME_TETE[1][0] = -sin_dR;
      C_TEME_TETE[2][0] = 0.0;
      C_TEME_TETE[0][1] = sin_dR;
      C_TEME_TETE[1][1] = cos_dR;
      C_TEME_TETE[2][1] = 0.0;
      C_TEME_TETE[0][2] = 0.0;
      C_TEME_TETE[1][2] = 0.0;
      C_TEME_TETE[2][2] = 1.0;
}
/**********************************************************************/
/* http://en.wikipedia.org/wiki/Geodetic_system#Geodetic_versus_geocentric_latitude */
void WGS84ToECEF(double glat, double glong, double alt, double p[3])
{
      double a = 6378137.0;
      double f = 1.0/298.257222101;
      double e2 = f*(2.0-f);
      double X;
      double CosLat,SinLat,CosLng,SinLng;

      CosLat = cos(glat);
      SinLat = sin(glat);
      CosLng = cos(glong);
      SinLng = sin(glong);

      X = sqrt(1.0-e2*SinLat*SinLat);

      p[0] = (a/X+alt)*CosLat*CosLng;
      p[1] = (a/X+alt)*CosLat*SinLng;
      p[2] = (a/X*(1.0-e2)+alt)*SinLat;
}
/**********************************************************************/
void ECEFToWGS84(double p[3], double *glat, double *glong, double *alt)
{
      double a = 6378137.0;
      double f = 1.0/298.257222101;
      double b = a*(1.0-f);
      double e2 = f*(2.0-f);
      double ep2 = f*(2.0-f)/(1.0-f)/(1.0-f);
      double r,E2,F,G,C,S,P,Q,r0,U,V,Z0;

      double OneMinusE2,Z1,SpolyG,Qpoly;

      OneMinusE2 = 1.0-e2;

      r = sqrt(p[0]*p[0]+p[1]*p[1]);

      E2 = a*a-b*b;

      Z1 = b*p[2];

      F = 54.0*Z1*Z1;

      G = r*r+OneMinusE2*p[2]*p[2]-e2*E2;

      Z1 = e2*r/G;
      C = Z1*Z1*F/G;

      S = pow(1.0+C+sqrt(C*C+2.0*C),1.0/3.0);

      SpolyG = (S+1.0/S+1.0)*G;
      P = F/(3.0*SpolyG*SpolyG);

      Q = sqrt(1.0+2.0*e2*e2*P);

      Qpoly = 1.0+Q;
      r0 = -P*e2*r/Qpoly+sqrt(0.5*a*a*Qpoly/Q-P*OneMinusE2*p[2]*p[2]/(Q*Qpoly)-0.5*P*r*r);

      Z1 = r-e2*r0;
      Z1 *= Z1;

      U = sqrt(Z1+p[2]*p[2]);

      V = sqrt(Z1+OneMinusE2*p[2]*p[2]);


      Z1 = b*b/a/V;
      Z0 = Z1*p[2];

      *alt = U*(1.0-Z1);
      *glat = atan((p[2]+ep2*Z0)/r);
      *glong = atan2(p[1],p[0]);
}
/**********************************************************************/
/* Ref Werner and Scheeres, "Exterior Gravitation of a Polyhedron ..." */
/* Returns 1 if PosN is outside polyhedron, 0 if inside */
long PolyhedronGravAcc(struct GeomType *G, double Density, 
   double PosN[3], double CWN[3][3], double GravAccN[3])
{
      struct EdgeType *E;
      struct PolyType *P;
      double *V1,*V2,*V3;
      double PosW[3],GravAccW[3];
      double re1[3],re2[3],rf1[3],rf2[3],rf3[3],r1,r2,r3,r2xr3[3];
      double Num,Den,Er[3],Fr[3],Le,wf,SumWf,Gsig;
      long PosIsOutside;
      long Ie,Ip,i;
      
      for(i=0;i<3;i++) {
         GravAccW[i] = 0.0;
      }
      SumWf = 0.0;
      
      MxV(CWN,PosN,PosW);
      
      for(Ie=0;Ie<G->Nedge;Ie++) {
         E = &G->Edge[Ie];
         V1 = G->V[E->Vtx1];
         V2 = G->V[E->Vtx2];
         for(i=0;i<3;i++) {
            re1[i] = V1[i] - PosW[i];
            re2[i] = V2[i] - PosW[i];
         }
         r1 = MAGV(re1);
         r2 = MAGV(re2);
         Le = log((r1+r2+E->Length)/(r1+r2-E->Length));
         MxV(E->Dyad,re1,Er);
         for(i=0;i<3;i++) {
            GravAccW[i] -= Er[i]*Le;
         }
      }
      
      for(Ip=0;Ip<G->Npoly;Ip++) {
         P = &G->Poly[Ip];
         V1 = G->V[P->V[0]];
         V2 = G->V[P->V[1]];
         V3 = G->V[P->V[2]];
         for(i=0;i<3;i++) {
            rf1[i] = V1[i] - PosW[i];
            rf2[i] = V2[i] - PosW[i];
            rf3[i] = V3[i] - PosW[i];
         }
         r1 = MAGV(rf1);
         r2 = MAGV(rf2);
         r3 = MAGV(rf3);
         VxV(rf2,rf3,r2xr3);
         Num = VoV(rf1,r2xr3);
         Den = r1*r2*r3 + r1*VoV(rf2,rf3) + r2*VoV(rf3,rf1) + r3*VoV(rf1,rf2);
         wf = 2.0*atan2(Num,Den);
         MxV(P->Dyad,rf1,Fr);
         for(i=0;i<3;i++) {
            GravAccW[i] += Fr[i]*wf;
         }
         SumWf += wf;
      }
      
      Gsig = 6.67408E-11*Density;
      for(i=0;i<3;i++) {
         GravAccW[i] *= Gsig;
      }
      
      MTxV(CWN,GravAccW,GravAccN);
      
      /* SumWf should be zero if Pos Is Outside, or -4*pi if Pos is Inside */
      PosIsOutside = (SumWf > -6.28 ? 1 : 0);
      
      return(PosIsOutside);
}
/**********************************************************************/
/* Ref Werner and Scheeres, "Exterior Gravitation of a Polyhedron ..." */
/* Returns 1 if PosN is outside polyhedron, 0 if inside */
long PolyhedronGravGrad(struct GeomType *G, double Density, double PosN[3],
   double CWN[3][3], double GravGradN[3][3])
{
      struct EdgeType *E;
      struct PolyType *P;
      double *V1,*V2,*V3;
      double PosW[3],GravGradW[3][3],GC[3][3];
      double re1[3],re2[3],rf1[3],rf2[3],rf3[3],r1,r2,r3,r2xr3[3];
      double Num,Den,Le,wf,SumWf,Gsig;
      long PosIsOutside;
      long Ie,Ip,i,j;
      
      for(i=0;i<3;i++) {
         for(j=0;j<3;j++) GravGradW[i][j] = 0.0;
      }
      SumWf = 0.0;
      
      MxV(CWN,PosN,PosW);
      
      for(Ie=0;Ie<G->Nedge;Ie++) {
         E = &G->Edge[Ie];
         V1 = G->V[E->Vtx1];
         V2 = G->V[E->Vtx2];
         for(i=0;i<3;i++) {
            re1[i] = V1[i] - PosW[i];
            re2[i] = V2[i] - PosW[i];
         }
         r1 = MAGV(re1);
         r2 = MAGV(re2);
         Le = log((r1+r2+E->Length)/(r1+r2-E->Length));
         for(i=0;i<3;i++) {
            for(j=0;j<3;j++) GravGradW[i][j] += E->Dyad[i][j]*Le;
         }
      }
      
      for(Ip=0;Ip<G->Npoly;Ip++) {
         P = &G->Poly[Ip];
         V1 = G->V[P->V[0]];
         V2 = G->V[P->V[1]];
         V3 = G->V[P->V[2]];
         for(i=0;i<3;i++) {
            rf1[i] = V1[i] - PosW[i];
            rf2[i] = V2[i] - PosW[i];
            rf3[i] = V3[i] - PosW[i];
         }
         r1 = MAGV(rf1);
         r2 = MAGV(rf2);
         r3 = MAGV(rf3);
         VxV(rf2,rf3,r2xr3);
         Num = VoV(rf1,r2xr3);
         Den = r1*r2*r3 + r1*VoV(rf2,rf3) + r2*VoV(rf3,rf1) + r3*VoV(rf1,rf2);
         wf = 2.0*atan2(Num,Den);
         for(i=0;i<3;i++) {
            for(j=0;j<3;j++) GravGradW[i][j] -= P->Dyad[i][j]*wf;
         }
         SumWf += wf;
      }
      
      Gsig = 6.67408E-11*Density;
      for(i=0;i<3;i++) {
         for(j=0;j<3;j++) GravGradW[i][j] *= Gsig;
      }
      
      MxM(GravGradW,CWN,GC);
      MTxM(CWN,GC,GravGradN);
      
      /* SumWf should be zero if Pos Is Outside, or -4*pi if Pos is Inside */
      PosIsOutside = (SumWf > -6.28 ? 1 : 0);
      
      return(PosIsOutside);
}
/**********************************************************************/
void GravGradTimesInertia(double g[3][3], double I[3][3], double GGxI[3])
{
      GGxI[0] = (I[2][2]-I[1][1])*g[1][2] + (g[1][1]-g[2][2])*I[1][2] + I[0][2]*g[1][0] - I[0][1]*g[2][0];
      GGxI[1] = (I[0][0]-I[2][2])*g[2][0] + (g[2][2]-g[0][0])*I[2][0] + I[0][1]*g[1][2] - I[1][2]*g[0][1];
      GGxI[2] = (I[1][1]-I[0][0])*g[0][1] + (g[0][0]-g[1][1])*I[0][1] + I[1][2]*g[0][2] - I[0][2]*g[1][2];
}
/* #ifdef __cplusplus
** }
** #endif
*/

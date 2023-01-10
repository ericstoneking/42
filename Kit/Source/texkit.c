/*    This file is distributed with 42,                               */
/*    the (mostly harmless) spacecraft dynamics simulation            */
/*    created by Eric Stoneking of NASA Goddard Space Flight Center   */

/*    Copyright 2010 United States Government                         */
/*    as represented by the Administrator                             */
/*    of the National Aeronautics and Space Administration.           */

/*    No copyright is claimed in the United States                    */
/*    under Title 17, U.S. Code.                                      */

/*    All Other Rights Reserved.                                      */


/*  Procedural Texturing functions cribbed from:                      */
/*  Ebert, Musgrave, Peachey, Perlin, Worley.                         */
/*  "Texturing & Modeling: A Procedural Approach", 3rd edition        */

#include "texkit.h"

/* Ref p. 69-70 */

#define TABSIZE 256
#define TABMASK (TABSIZE-1)
#define PERM(x) perm[(x) & TABMASK]
#define INDEX(ix,iy,iz) PERM((ix)+PERM((iy)+PERM(iz)))
static unsigned char perm[TABSIZE] = {
   225,155,210,108, 175,119,221,144, 203,116, 70,213,  69,158, 33,252,
     5, 82,173,133, 222,139,174, 27,   9, 71, 90,246,  75,130, 91,191,
   169,138,  2,151, 194,235, 81,  7,   5,113,228,159, 205,253,134,142,
   248, 65,224,217,  22,121,229, 63,  89,103, 96,104, 156, 17,201,129,
    36,  8,165,110, 237,117,231, 56, 132,211,152, 20, 181,111,239,218,
   170,163, 51,172, 157, 47, 80,212, 176,250, 87, 49,  99,242,136,189,
   162,115, 44, 43, 124, 94,150, 16, 141,247, 32, 10, 198,223,255, 72,
    53,131, 84, 57, 220,197, 58, 50, 208, 11,241, 28,   3,192, 62,202,
    18,215,153, 24,  76, 41, 15,179,  39, 46, 55,  6, 128,167, 23,188,
   106, 34,187,140, 164, 73,112,182, 244,195,227, 13,  35, 77,196,185,
    26,200,226,119,  31,123,168,125, 249, 68,183,230, 177,135,160,180,
    12,  1,243,148, 102,166, 38,238, 251, 37,240,126,  64, 74,161, 40,
   184,149,171,178, 101, 66, 29, 59, 146, 61,254,107,  42, 86,154,  4,
   236,232,120, 21, 233,209, 45, 98, 193,114, 78, 19, 206, 14,118,127,
    48, 79,147, 85,  30,207,219, 54,  88,234,190,122,  95, 67,143,109,
   137,214,145,93,92,100,245,0,216,186,60,83,105,97,204,52};
#define RANDMASK 0x7fffffff
#define RANDNUMBER ((random() & RANDMASK)/(double) RANDMASK)
double ValRandomTable[TABSIZE];
double GradRandomTable[3*TABSIZE];

/* Ref p. 33 */
#define FLOOR(x) ((int)(x) - ((x) < 0 && (x) != (int)(x)))
#define CEIL(x)  ((int)(x) + ((x) > 0 && (x) != (int)(x)))

/* Ref p. 76 */
#define LERP(t,x0,x1) ((x0)+(t)*((x1)-(x0)))
#define SMOOTHSTEP(x) ((x)*(x)*(3.0-2.0*(x)))

/**********************************************************************/
/* Ref p. 25                                                          */
void MixColor(float C1[3], float C2[3], double f, float C[3])
{
   C[0] = (1.0-f)*C1[0] + f*C2[0];
   C[1] = (1.0-f)*C1[1] + f*C2[1];
   C[2] = (1.0-f)*C1[2] + f*C2[2];
}
/**********************************************************************/
/* Ref p. 34-35, 72                                                   */
/* Cubic spline interpolation of domain [0:1] equally subdivided by   */
/* four knots.                                                        */
double Spline4(double x,double knot[4])
{
      double c1,c2,c3;
      x = Clamp(x,0.0,1.0);

      c3 = -0.5*knot[0] + 1.5*knot[1] - 1.5*knot[2] + 0.5*knot[3];
      c2 =      knot[0] - 2.5*knot[1] + 2.0*knot[2] - 0.5*knot[3];
      c1 = -0.5*knot[0]               + 0.5*knot[2];

      return ((c3*x + c2)*x + c1)*x + knot[1];
}
/**********************************************************************/
/* Ref p. 38                                                          */
double PerlinBias(double b, double x)
{
      return pow(x, log(b)/-0.693147180559945);
}
/**********************************************************************/
/* Ref p. 38                                                          */
double PerlinGain(double g, double x)
{
      if (x < 0.5) return 0.5*PerlinBias(1.0-g,2.0*x);
      else return 1.0-0.5*PerlinBias(1.0-g,2.0-2.0*x);
}
/**********************************************************************/
/* Ref p. 71                                                          */
/* ValRandomTable gets packed with uniformly distributed PRNs in [-1:1] */
void InitValRandomTable(int seed)
{
      double *T = ValRandomTable;
      long i;
      srandom(seed);
      for(i=0;i<TABSIZE;i++)
         *T++ = 1.0-2.0*RANDNUMBER;
}
/**********************************************************************/
/* Ref p. 71                                                          */
double vlattice(long ix, long iy, long iz)
{
      return ValRandomTable[INDEX(ix,iy,iz)];
}
/**********************************************************************/
/* Ref p. 71-72                                                       */
double vnoise(double x, double y, double z)
{
      long ix, iy, iz;
      long i,j,k;
      double fx,fy,fz;
      double xknots[4],yknots[4],zknots[4];
      static long First = 1;

      if (First) {
         First = 0;
         InitValRandomTable(665);
      }
      ix = FLOOR(x);
      fx = x - ix;
      iy = FLOOR(y);
      fy = y - iy;
      iz = FLOOR(z);
      fz = z - iz;

      for(k=-1;k<=2;k++) {
         for(j=-1;j<=2;j++) {
            for(i=-1;i<=2;i++) {
               xknots[i+1] = vlattice(ix+i,iy+j,iz+k);
            }
            yknots[j+1] = Spline4(fx,xknots);
         }
         zknots[k+1] = Spline4(fy,yknots);
      }
      return Spline4(fz,zknots);
}
/**********************************************************************/
/* Ref p. 74-75                                                       */
/* GradRandomTable gets packed with uniformly distributed PRNs in [-1:1] */
void InitGradRandomTable(int seed)
{
      double *T = GradRandomTable;
      double z,r,theta;
      long i;
      srandom(seed);
      for(i=0;i<TABSIZE;i++)
         z = 1.0-2.0*RANDNUMBER;
         r = sqrt(1.0-z*z);
         theta = 2.0*M_PI*RANDNUMBER;
         *T++ = r*cos(theta);
         *T++ = r*sin(theta);
         *T++ = z;
}
/**********************************************************************/
/* Ref p. 76                                                          */
double glattice(long ix, long iy, long iz, double fx, double fy, double fz)
{
      double *g = &GradRandomTable[INDEX(ix,iy,iz)*3];
      return g[0]*fx+g[1]*fy+g[2]*fz;
}
/**********************************************************************/
/* Ref p. 76                                                          */
double gnoise(double x, double y, double z)
{
      long ix, iy, iz;
      double fx0,fx1,fy0,fy1,fz0,fz1;
      double wx, wy, wz;
      double vx0,vx1,vy0,vy1,vz0,vz1;
      static long First = 1;

      if (First) {
         First = 0;
         InitGradRandomTable(665);
      }
      ix = FLOOR(x);
      fx0 = x-ix;
      fx1 = fx0-1.0;
      wx = SMOOTHSTEP(fx0);
      iy = FLOOR(y);
      fy0 = y-iy;
      fy1 = fy0-1.0;
      wy = SMOOTHSTEP(fy0);
      iz = FLOOR(z);
      fz0 = z-iz;
      fz1 = fz0-1.0;
      wz = SMOOTHSTEP(fz0);

      vx0 = glattice(ix,iy,iz,fx0,fy0,fz0);
      vx1 = glattice(ix+1,iy,iz,fx1,fy0,fz0);
      vy0 = LERP(wx,vx0,vx1);
      vx0 = glattice(ix,iy+1,iz,fx0,fy1,fz0);
      vx1 = glattice(ix+1,iy+1,iz,fx1,fy1,fz0);
      vy1 = LERP(wx,vx0,vx1);
      vz0 = LERP(wy,vy0,vy1);
      vx0 = glattice(ix,iy,iz+1,fx0,fy0,fz1);
      vx1 = glattice(ix+1,iy,iz+1,fx1,fy0,fz1);
      vy0 = LERP(wx,vx0,vx1);
      vx0 = glattice(ix,iy+1,iz+1,fx0,fy1,fz1);
      vx1 = glattice(ix+1,iy+1,iz+1,fx1,fy1,fz1);
      vy1 = LERP(wx,vx0,vx1);
      vz1 = LERP(wy,vy0,vy1);

      return LERP(wz,vz0,vz1);
}
/**********************************************************************/
/* Ref p. 86                                                          */
double turbulence(double x, double y, double z,
                 double MinFreq, double MaxFreq)
{
      double t = 0.0;
      double f;
      for(f=MinFreq;f<MaxFreq;f*=2.0)
         t += fabs(gnoise(f*x,f*y,f*z))/f;
      return t;
}
/**********************************************************************/
double FractalWorley(double p[3], long octaves, double lacunarity,
                    long DistanceType)
{
      long i,k;
      double G = 0.0;
      double scale = 1.0;
      double at[3],F,delta[3];
      unsigned long ID;

      for(k=0;k<3;k++) at[k] = p[k];
      for(i=0;i<octaves;i++) {
         Worley(at,1,&F,&delta,&ID,DistanceType);
         G += scale*F;
         scale /= lacunarity;
         for(k=0;k<3;k++) at[k] *= lacunarity;
      }
      return(G);
}
/**********************************************************************/
double FractalWorley2(double p[3], long octaves, double lacunarity,
                     long DistanceType)
{
      long i,k;
      double G = 0.0;
      double scale = 1.0;
      double at[3],F[2],delta[2][3];
      unsigned long ID[2];

      for(k=0;k<3;k++) at[k] = p[k];
      for(i=0;i<octaves;i++) {
         Worley(at,2,F,delta,ID,DistanceType);
         G += scale*fabs(F[1]-F[0]);
         scale /= lacunarity;
         for(k=0;k<3;k++) at[k] *= lacunarity;
      }
      return(G);
}
/*********************************************************************/
double ProcTex2D(double x, double y, double Xunit, double Yunit, long Noct)
{
      long k;
      long ix,iy;
      double kx,ky,fx,fy;
      double z = 0.0;

      for(k=1;k<=Noct;k++) {
         kx = ((double) k)*x/Xunit;
         ky = ((double) k)*y/Yunit;
         ix = (long) kx;
         iy = (long) ky;
         fx = kx-ix;
         fy = ky-iy;
         z += CubicInterp2D(PRN2D(ix,iy),PRN2D(ix+1,iy),
                            PRN2D(ix,iy+1),PRN2D(ix+1,iy+1),
                            fx,fy)/((double) k);
      }
      z = 0.5+0.5*z;
      if (z > 1.0) z = 1.0;
      if (z < 0.0) z = 0.0;
      return(z);
}
/*********************************************************************/
double ProcTex3D(double x, double y, double z,
                double Xunit, double Yunit, double Zunit,
                long Noct, double Persist)
{
      long k;
      long ix,iy,iz;
      double kx,ky,kz,fx,fy,fz;
      double PT3D = 0.0;
      double Scale = Persist;
      double SumScale = 0.0;

      for(k=1;k<=Noct;k++) {
         kx = ((double) k)*x/Xunit;
         ky = ((double) k)*y/Yunit;
         kz = ((double) k)*z/Zunit;
         ix = (long) kx;
         iy = (long) ky;
         iz = (long) kz;
         fx = kx-ix;
         fy = ky-iy;
         fz = kz-iz;
         PT3D += Scale*CubicInterp3D(
            PRN3D(ix  ,iy  ,iz  ),PRN3D(ix+1,iy  ,iz  ),
            PRN3D(ix  ,iy+1,iz  ),PRN3D(ix+1,iy+1,iz  ),
            PRN3D(ix  ,iy  ,iz+1),PRN3D(ix+1,iy  ,iz+1),
            PRN3D(ix  ,iy+1,iz+1),PRN3D(ix+1,iy+1,iz+1),
            fx,fy,fz);
         SumScale += Scale;
         Scale *= Persist;
      }
      PT3D = 0.5+0.5*PT3D/SumScale;
      if (PT3D > 1.0) PT3D = 1.0;
      if (PT3D < 0.0) PT3D = 0.0;
      return(PT3D);
}
/**********************************************************************/
double SphereTex(double lng, double lat,
   double Xunit, double Yunit, double Zunit, long Noct, double Persist)
{
      double clat = cos(lat);
      double x = cos(lng)*clat;
      double y = sin(lng)*clat;
      double z = sin(lat);
      double f = ProcTex3D(x+1.0,y+1.0,z+1.0,Xunit,Yunit,Zunit,Noct,Persist);
      return(f);
}

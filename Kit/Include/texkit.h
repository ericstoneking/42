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

void Worley(double at[3], long max_order,
       double *F, double (*delta)[3], unsigned long *ID,
       long DistanceType);

void MixColor(float C1[3], float C2[3], double f, float C[3]);
double Spline4(double x,double knot[4]);
double PerlinBias(double b, double x);
double PerlinGain(double g, double x);
void InitValRandomTable(int seed);
double vlattice(long ix, long iy, long iz);
double vnoise(double x, double y, double z);
void InitGradRandomTable(int seed);
double glattice(long ix, long iy, long iz, double fx, double fy, double fz);
double gnoise(double x, double y, double z);
double turbulence(double x, double y, double z,
                 double MinFreq, double MaxFreq);
double FractalWorley(double p[3], long octaves, double lacunarity,
                    long DistanceType);
double FractalWorley2(double p[3], long octaves, double lacunarity,
                     long DistanceType);
double ProcTex2D(double x, double y, double Xunit, double Yunit, long Noct);
double ProcTex3D(double x, double y, double z,
                double Xunit, double Yunit, double Zunit,
                long Noct, double Persist);
double SphereTex(double lng, double lat,
   double Xunit, double Yunit, double Zunit, long Noct, double Persist);

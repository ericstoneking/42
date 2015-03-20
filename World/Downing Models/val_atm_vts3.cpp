#include "val_atm_vts3.h"
#include "val_solarflux.h"
#include "val_atm.h"
#include "val_universe.h"
#include "val_unit_convert.h"
#include "val_userio.h"
//====================================================================
//  VAL -- Virtual Almanac Library
//====================================================================
//  Purpose:  Calculate acceleration due to Venus atmospheric drag
//====================================================================
// Model Coefficients

//  CO2 density
static double pa1[] = {
   7.76049e-01,    2.93750e0,      -1.47210e-01,   -1.57306e-01,   -6.69599e-02,
   -8.18055e-01,   -1.06697e-02,   3.00201e-01,    7.96075e-04,    3.24607e-01,
   9.23450e-05,    0.0      ,      0.0      ,      -2.37584e-03,   -1.34238e-04,
   1.00000e0,      0.0      ,      0.0      ,      0.0      ,      0.0      ,
   0.0      ,      0.0      ,      0.0      ,      0.0      ,      0.0      ,
   0.0      ,      0.0      ,      0.0      ,      0.0      ,      0.0      ,
   1.13277e0,      -1.87582e0,     -3.38029e-01,   -9.31956e-01,   9.04382e-02,
   1.67238e0,      7.32745e-03,    8.28310e-01,    1.69341e-03,    -6.84008e-01,
   -1.00458e-04,   0.0      ,      0.0      ,      0.0      ,      0.0      ,
   0.0      ,      0.0      ,      0.0      ,      2.01296e-02,    0.0 };

//  O density
static double pb1[] = {
   1.07134e0,      7.13595e-01,    -3.61877e-02,   2.82843e-01,    4.85210e-03,
   -1.95412e-01,   -1.76002e-03,   -3.34167e-01,   -9.68110e-04,   3.87223e-01,
   3.88084e-05,    0.0      ,      0.0      ,      2.84044e-03,    1.20219e-03,
   3.00713e-02,    0.0      ,      0.0      ,      0.0      ,      0.0      ,
   0.0      ,      0.0      ,      0.0      ,      0.0      ,      0.0      ,
   0.0      ,      0.0      ,      0.0      ,      0.0      ,      0.0      ,
   3.90127e0,      0.0      ,      0.0      ,      0.0      ,      0.0      ,
   0.0      ,      0.0      ,      0.0      ,      0.0      ,      0.0      ,
   0.0      ,      0.0      ,      0.0      ,      0.0      ,      0.0      ,
   1.12140e0,      1.30508e0,      0.0      ,      0.0      ,      9.80888e-04 };

// CO density & tlb
static double pc1[] = {
   9.84586e-01,    1.92394e0,      -8.16346e-02,   1.85261e-01,    -4.62701e-03,
   -4.08268e-01,   -1.68582e-03,   -2.05573e-01,   -1.15921e-03,   4.93592e-01,
   -2.59753e-05,   0.0      ,      0.0      ,      1.39529e-03,    5.53068e-04,
   1.00000e0,      0.0      ,      0.0      ,      0.0      ,      0.0      ,
   0.0      ,      0.0      ,      0.0      ,      0.0      ,      0.0      ,
   0.0      ,      0.0      ,      0.0      ,      0.0      ,      0.0      ,
   1.00000e00,     0.0      ,      0.0      ,      0.0      ,      0.0      ,
   0.0      ,      0.0      ,      0.0      ,      0.0      ,      0.0      ,
   0.0      ,      0.0      ,      0.0      ,      0.0      ,      0.0      ,
   0.0      ,      0.0      ,      0.0      ,      0.0      ,      0.0 };

//  He density
static double pd1[] = {
   9.61871e-01,    -1.42734e00,    5.93447e-01,    9.36320e-02,    1.39517e-01,
   8.39837e-01,    -3.60608e-03,   -3.57368e-01,   -1.38972e-03,   -1.96184e-02,
   8.86656e-05,    0.0      ,      0.0      ,      2.15513e-03,    -7.68169e-04,
   3.03416e-02,    0.0      ,      0.0      ,      0.0      ,      0.0      ,
   0.0      ,      0.0      ,      0.0      ,      0.0      ,      0.0      ,
   0.0      ,      0.0      ,      0.0      ,      0.0      ,      0.0      ,
   1.00000e00,     0.0      ,      0.0      ,      0.0      ,      0.0      ,
   0.0      ,      0.0      ,      0.0      ,      0.0      ,      0.0      ,
   0.0      ,      0.0      ,      0.0      ,      0.0      ,      0.0      ,
   0.0      ,      0.0      ,      0.0      ,      0.0      ,      0.0 };

//  N density
static double pe1[] = {
   1.07218e00,     1.07061e00,     -1.92920e-01,   1.72877e-01,    -4.19517e-02,
   -2.37737e-01,   -3.55792e-04,   -2.46647e-01,   -8.06493e-04,   4.72518e-01,
   8.04218e-06,    0.0      ,      0.0      ,      4.85444e-03,    1.24067e-03,
   1.00000e00,     0.0      ,      0.0      ,      0.0      ,      0.0      ,
   0.0      ,      0.0      ,      0.0      ,      0.0      ,      0.0      ,
   0.0      ,      0.0      ,      0.0      ,      0.0      ,      0.0      ,
   1.00000e00,     0.0      ,      0.0      ,      0.0      ,      0.0      ,
   0.0      ,      0.0      ,      0.0      ,      0.0      ,      0.0      ,
   0.0      ,      0.0      ,      0.0      ,      0.0      ,      0.0      ,
   0.0      ,      0.0      ,      0.0      ,      0.0      ,      0.0 };

//  N2 density
static double pf1[] = {
   8.11373e-01,    1.15335e00,     1.26869e-01,    2.25488e-01,    4.20755e-02,
   -2.21562e-01,   6.93173e-03,    -4.49676e-01,   -2.56643e-04,   5.91909e-01,
   1.22099e-05,    0.0      ,      0.0      ,      -1.42414e-03,   -6.52404e-04,
   1.00000e00,     0.0      ,      0.0      ,      0.0      ,      0.0      ,
   0.0      ,      0.0      ,      0.0      ,      0.0      ,      0.0      ,
   0.0      ,      0.0      ,      0.0      ,      0.0      ,      0.0      ,
   1.00000e00,     0.0      ,      0.0      ,      0.0      ,      0.0      ,
   0.0      ,      0.0      ,      0.0      ,      0.0      ,      0.0      ,
   0.0      ,      0.0      ,      0.0      ,      0.0      ,      0.0      ,
   0.0      ,      0.0      ,      0.0      ,      1.88000,        0.0 };
//====================================================================
// Some more global variables
static double  gsurfv,rev;
static double  swv[15];
static int     iswv;
static double  ptv[50];
static double  ptmv[8],pdmv[8][6];
//====================================================================
static double ccorv(double alt, double r, double h1, double zh);

static double denssv(double alt, double dlb, double tinf, double *tlb,
                     double xm, double alpha, double *tz, double zlb,
                     double s2, double t0, double za, double *ta, double zm,
                     double am, double sp);

static double dnetv(double dd, double dm, double zhm, double xmm, double xm);

static double globvv(double slat, double clat, double tloc,
                     double f107a, double f107, double p[]);

static short parmsv();

static short tselev(double sv[]);

static short turbov(double dd, double dm, double zb, double *zh, double xm,
                    double xmm, double tz);

static short vts3(double alt, double slat, double clat,
                  double tloc, double f107a, double f107, double d[7],
                  double t[2]);
//====================================================================
bool GetVenusDensity (const ZSmjd& smjd,
   const ZVector& sc, const ZVector& unitsun, real& rho)
//  Purpose:  Calculate acceleration due to Venus atmospheric drag
//  Comments:
//
//      ....Venus atmosphere outputs from vts3()
//
//      d[0] - total mass density (gm/cm3)
//        1  - co2 number density (cm-3)
//        2  - o
//        3  - co
//        4  - he
//        5  - n
//        6  - n2
//      t[0] - exospheric temperature
//        1  - temperature at altitude
   {
   real alt = (sc.Length() - VENUS->EquatorialRadius());

   real latitude = atan(sc.Z/(sqrt(sqr(sc.X) + sqr(sc.Y))));
   real sinpsi = sin(latitude);
   real cospsi = cos(latitude);

   // The Venus atmosphere model comes from "Global Empirical
   // Model of the Venus Thermosphere", by Hedin, et al., jgr,
   // vol.88, no.a1, pp. 73-83, jan. 1, 1983

   // Get daily and average solar flux */
   real f107;
   SolarFlux->GetF107(0,smjd,f107);
   double avgflux = f107;

   // Local solar time = solar hour angle + 12 hours
   real xynorm = sqrt(sqr(sc.X) + sqr(sc.Y));
   real xx = sc.X/xynorm;
   real yy = sc.Y/xynorm;
   real coshl = xx*unitsun.X + yy*unitsun.Y;
   real sinhl = yy*unitsun.X - xx*unitsun.Y;
   coshl = -coshl;
   sinhl = -sinhl;
   real tloc = atan2 (sinhl,coshl) * 24.0 / KReal::PI / 2;

   // Compute Hedin Venus model
   double d[7],t[2];
   vts3 ( alt, sinpsi, cospsi, tloc, avgflux, f107, d, t );
   rho = d[0];
   // Convert density from gm/cm3 to kg/km3
   rho *= 1.0e12;
   // Convert density in units of kg/m3
   rho *= 1.0e-9;

   return true;
   }
//--------------------------------------------------------------------
static double ccorv(double alt, double r, double h1, double zh)
   {
   //  chemistry/dissociation correction
   double ccorv_ret;
   static double e,ex;
   e = (zh - alt)/h1;
   if (e >  170.0)
      ccorv_ret = r;
   else if( e < -170.0 )
      ccorv_ret = 1.0;
   else
      {
      ex=exp(e);
      ccorv_ret = (1.0 + r * ex)/(1.0 + ex);
      }
   return ccorv_ret;
   }
//--------------------------------------------------------------------
#define  ZETA(zz) ((zz)-zlb)*(rev+zlb)/(rev+(zz))
static double denssv(double alt, double dlb, double tinf, double *tlb,
                     double xm, double alpha, double *tz, double zlb,
                     double s2, double t0, double za, double *ta, double zm,
                     double am, double sp)
//  calculate density (12/12/80)
//  tlb calculated from ta
   {
   double denssv_ret;
   static double taf;
   static double rgas = 831.40;
   static double z,zg2,zga,zg,t2,tt,s1,zg1,t1,tzl,zgm,exz,dit,exm,ditl,
           glb,gamma1,gamma2,dens2,dens1,gammap,densm;

   denssv_ret = dlb;
   taf = (*ta - t0)/(tinf - t0);

    if ( taf <= 0.0)
        *ta = t0 + 1.0e-4 * (tinf - t0);
    else if ( taf >= 1.0)
        *ta = tinf - 1.0e-4 * (tinf - t0);

    z = (alt > za) ? alt:za;
    zg2 = ZETA(z);
    zga = ZETA(za);
    zg = ZETA(alt);

    *tlb = tinf - (tinf - (*ta)) * exp(s2 * zga);
    t2 = tinf - (tinf - (*tlb)) * exp(-s2 * zg2);
    tt = t2;

    if (alt < za)
    {
        s1 = -s2 * (tinf - (*ta))/((*ta) - t0);
        zg1 = zg - zga;

//      Calculate temperature below za

        if (s1 * zg1 >= 10.0)
        {
            t1 = t0;
        }
        else
        {
            t1 = t0 - (t0 - (*ta)) * exp(-s1 * zg1);
        }
        tt = t1;

    }

    *tz = tt;
    tzl = *tlb;

    if (am != 0.0)
    {
        zgm = ZETA(zm);
        exz = exp(-(sp * (zg - zgm)));
        dit = 4.0 * am * exz/((1.0 + exz) * (1.0 + exz))/tinf;
        *tz = tt/(1.0 + tt * dit);
        exm = exp(sp * zgm);
        ditl = 4.0 * am * exm/((1.0 + exm) * (1.0 + exm))/tinf;
        tzl = *tlb/(1.0 + (*tlb) * ditl);
    }

    if (xm != 0.0)
    {
//
//      Calculate density above za
//
        glb = gsurfv/sqr(1.0+zlb/rev);
        gamma2 = xm * glb/(s2 * rgas * tinf);
        dens2 = dlb * pow((*tlb)/t2 , gamma2) * exp(-s2 * gamma2 * zg2);
        denssv_ret = dens2;

        if (alt < za)
        {
//          Calculate density below za;

            gamma1 = xm * glb/(s1 * rgas * t0);
            dens1 = dens2 * pow((*ta)/tt , gamma1) * exp(-s1 * gamma1 * zg1);
            denssv_ret = dens1;
        }
        if (am != 0.0)
        {
            gammap = xm * glb/(sp * rgas * tinf);
            densm = exp(gammap * 4.0 * am * (exz/(1.0 + exz) - exm/(1.0 + exm)));
            denssv_ret *= densm;
        }
        denssv_ret *= pow(tzl/(*tz) , 1.0+alpha);

    }

    return denssv_ret;
}
//--------------------------------------------------------------------
static double dnetv(double dd, double dm, double zhm, double xmm, double xm)
// Purpose: Turbopause correction
   {
    double dnetv_ret;

    static double a,adm,add,ylog;

    a = zhm/(xmm-xm);
    adm = 0.0;
    if (dm > 0.0) adm = log(dm);
    add = 0.0;
    if (dd > 0.0) add = log(dd);
    ylog = a*(adm-add);

    if (ylog < -10.0)
        dnetv_ret = dd;
    else if (ylog > 10.0)
        dnetv_ret = dm;
    else
        dnetv_ret = dd * pow(1.0+exp(ylog) , 1/a);

    return dnetv_ret;
}
//--------------------------------------------------------------------
static double globvv(double slat, double clat, double tloc,
                     double f107a, double f107, double p[])
//  Purpose: Calculate global variations (12/12/80)
//           Uses solar zenith angle for symmetrical terms
   {

    double globvv_ret;

    static double plg[6][6],t[15];

    static double xl = -1000.0;

    static double sv[15] = {1.0, 1.0, 1.0, 1.0, 1.0,
                            1.0, 1.0, 1.0, 1.0, 1.0,
                            1.0, 1.0, 1.0, 1.0, 1.0};

    static double tll = 1000.0;
    static double hr = 0.26180;

    static double c,s,s2,c2,c4,tlochr,stloc,ctloc,stloc2,s2tloc,s3tloc,
           s4tloc,s5tloc,f,g;
    short i;

    if (iswv != 64999) tselev(sv);

    if (xl != slat || tll != tloc)
    {
//
//      Calculate legendre polynomials
//
        c = clat;
        s = slat;

//      Cosine of solar zenith angle

        c = clat * cos(hr * (tloc - 12.0));
        s2 = s * s;
        c2 = c * c;
        c4 = c2 * c2;
        plg[1][0] = c;
        plg[2][0] = 0.5 * (3.0 * c2 - 1.0);
        plg[3][0] = 0.5 * (5.0 * c2 - 3.0) * c;
        plg[4][0] = (35 * c4 - 30 * c2 + 3.0)/8.0;
        plg[5][0] = (63 * c4 - 70 * c2 + 15) * c/8.0;
        plg[1][1] = s;
        plg[2][2] = 3.0 * s2;
        plg[3][3] = 15.0 * s2 * s;
        plg[4][4] = 105 * s2 * s2;
        plg[5][5] = 945 * s2 * s2 * s;
        xl = slat;
    }

    if (tll != tloc)
    {

        tlochr = hr * tloc;
        stloc = sin( tlochr );
        ctloc = cos( tlochr );
        stloc2 = stloc * stloc;
        s2tloc = 2.0 * stloc * ctloc;
        s3tloc = ( 3.0 - 4.0 * stloc2 ) * stloc;
        s4tloc = ctloc * stloc * ( 4.0 - 8.0 * stloc2 );
        s5tloc = stloc * ( 5.0 - 20.0 * stloc2 + 16.0 * stloc2 * stloc2 );

    }

    tll = tloc;
    t[0] = p[13] * (f107a - 200.0) + p[14] * (f107 - f107a);
//
//  The following test inadvertently treats helium differently than the
//  other species with regard to the f107 effect.
//  the '>' should be '!=' but is not changed here to be consisTENt with
//  the coefficients and tables as originally derived and published.
//  7/25/83
//

    if (p[1] <= 0.0)
        f = 1.0;
    else
        f = 1.0 + t[0]/(p[1] - p[3] + p[5] - p[7] + p[9]);

    t[1] = (p[1] * plg[1][0] + p[2]  * plg[1][1] * stloc)  * f;
    t[2] = (p[3] * plg[2][0] + p[4]  * plg[2][2] * s2tloc) * f;
    t[3] = (p[5] * plg[3][0] + p[6]  * plg[3][3] * s3tloc) * f;
    t[4] = (p[7] * plg[4][0] + p[8]  * plg[4][4] * s4tloc) * f;
    t[5] = (p[9] * plg[5][0] + p[10] * plg[5][5] * s5tloc) * f;

    g=0.0;

    for (i = 0; i < 6; i++)
       g += swv[i] * t[i];

    globvv_ret = g;

    return globvv_ret;
}
//--------------------------------------------------------------------
static short parmsv()
// Purpose: Set initial parameters (04/24/81)
   {
   static const double gsurf0 = 887.40;
   short j,k;

//  Model Coefficients
//
//  Temperature
//
    static double pt1[50] =
    {
        1.14575e0,      5.73938e-01,    -1.05623e-01,   -1.53896e-01,   -7.11596e-03,
        -1.82894e-01,   4.41052e-03,    1.22197e-01,    3.20351e-04,    -9.28368e-03,
        -2.32161e-05,   0.0      ,      0.0      ,      1.00000e-03,    6.00000e-04,
        7.93416e-01,    1.30399e-01,    8.82217e-02,    -4.98274e-01,   -2.05990e-02,
        0.0      ,      0.0      ,      0.0      ,      0.0      ,      0.0      ,
        0.0      ,      0.0      ,      0.0      ,      0.0      ,      0.0      ,
        1.12000e0,      0.0      ,      0.0      ,      0.0      ,      0.0      ,
        0.0      ,      0.0      ,      0.0      ,      0.0      ,      0.0      ,
        0.0      ,      0.0      ,      0.0      ,      0.0      ,      0.0      ,
        7.23933e-01,    9.74046e-01,    1.00000e0,      7.24953e-02,    0.0
    };

//
//  Lower boundary
//
    static double pm[8][7] =
    {

        { 1.99171e02,     5.00530e08,     1.04912e09,     2.95223e08,
          5.64985e06,     9.30114e06,     1.39749e08  },
        { 1.87600e02,     0.0      ,      1.00000e-01,    0.0      ,
          2.00000e-05,    0.0      ,      3.50000e-02 },
        { 1.77000e02,     1.30000e02,     1.12862e02,     1.26638e02,
          1.23157e02,     1.12862e02,     1.29748e02  },
        { 9.21400e-02,    4.00000e01,     0.0,            0.0      ,
          1.00000e01,     0.0      ,      0.0         },
        { 1.25000e02,     4.34577e01,     0.0      ,      0.0      ,
          0.0      ,      0.0      ,      0.0         },
        { 1.50000e02,     1.00000e02,     1.20000e02,     0.0      ,
          0.0      ,      1.24000e02,     0.0         },
        { 1.40000e02,     0.0      ,      5.80000e00,     0.0      ,
          0.0      ,      1.45000e00,     0.0         },
        { 1.00000e-01,    1.63000e00,     1.00000e00,     0.0      ,
          0.0      ,      0.0      ,      0.0         }
    };

    gsurfv = gsurf0;

//  Venus equatorial radius in km.
    rev = VENUS->EquatorialRadius();

    for (j=0; j<50; j++)
      ptv[j] = pt1[j];

    for (j=0; j<8; j++)
      ptmv[j] = pm[j][0];

    for (k=0; k<6; k++)
      for (j=0; j<8; j++)
          pdmv[j][k] = pm[j][k+1];

    return 1;
   }
//--------------------------------------------------------------------
static short tselev(double sv[])
   {
   short i;
   for (i=0; i<15; i++)
        swv[i] = sv[i];

   iswv = 64999;
   return 1;
   }
//--------------------------------------------------------------------
static short turbov(double dd, double dm, double zb, double *zh, double xm,
                    double xmm, double tz)
// Purpose: Estimate turbopause height
   {
   static double rgas = 831.40;
   static double gzb;

    gzb = gsurfv/sqr(1.0 + zb / rev);
    *zh = zb + rgas * tz/gzb/(xm - xmm) * log(dd/dm);

    return 1;
   }
//--------------------------------------------------------------------
static short vts3(double alt, double slat, double clat,
                  double tloc, double f107a, double f107, double d[7],
                  double t[2])
//  Purpose: Venus atmosphere model (03/05/81).
//           Returns density and temperature
//
//           Uses local time and latitude to estimate solar zenith
//           angle for symmetrical terms
//
//  I/O parameters:
//
//    name    i/o  a/s   description of parameters
//    ------  ---  ---   ------------------------------------------------
//    alt      i    s    satellite altitude in km
//    slat     i    s    sine of satellite latitude
//    clat     i    s    cosine of satellite latitude
//    tloc     i    s    local time in hours
//    f107a    i    s    average solar flux  (81 day centered average)
//    f107     i    s    solar flux for venus at current time
//    d        o    a    array of mass and number densitites
//    t        o    a    temperature at altitude and exospheric temp.
//
//  Comments
//
//   Hedin's original comments:
//
//   Input variables
//
//       alt   - altitude (km)
//       slat  - sine of latitude
//       clat  - cosine of latitude
//       tloc  - local hour angle (hrs)
//       f107a - 3 month avg 10.7
//       f107  - daily 10.7 shifted in phase to venus (but not correcting
//               for change in distance)
// 
//   Output variables
// 
//       d[0] - total mass density (gm/cm3)
//         1  - co2 number density (cm-3)
//         2  - o
//         3  - co
//         4  - he
//         5  - n
//         6  - n2
//       t[0] - exospheric temperature
//         1  - temperature at altitude
// 
//   Reference:
// 
//      Hedin, a. e., et al., "Global Empirical Model of the Venus
//             Thermosphere", JGR, Vol. 88, no.a1, pp 73-83, Jan. 1, 1983
   {
   static double tlb,s,db44,db16,db29,db04,db14,db28;
   static bool ifl = true;
   static double tinfv,tinf,za,t0,sp,fr,ta,zm,am,xmr,h1,zho,ymr,xmm,zhm,zh44,xmd,
           dm16,dD16,dm44,dmz44,zh16,dm28,dm29,dm04,zh14,b44,b14,dm14;
   static double tz;
   short i;

   if (ifl)
      {
      parmsv();
      ifl = false;
      }

    /* Temperature structure */

    tinfv = globvv(slat,clat,tloc,f107a,f107,&ptv[0]);

    tinf = ptmv[0] * (1.0 + tinfv) * ptv[0];
    t[0] = tinf;
    za = ptmv[4] * ptv[30];
    t0 = ptmv[2] * ptv[46];
    s = ptmv[3] * ptv[45];
    sp = ptv[48];

    /* Set ta between t0 and tinf */

    fr = ptv[15] * (1.0 + globvv(slat,clat,tloc,f107a,f107,&ptv[15]));

    fr = 1.0/(1.0 + exp(-4.0 * (fr - 0.5)));
    ta = tinf + fr * (t0 - tinf);
    zm = ptmv[6] * ptv[47];
    am = ptmv[7] * pa1[30] *
                   (1.0+globvv(slat,clat,tloc,f107a,f107,&pa1[30]));

    xmr = pdmv[1][1] * pb1[15] * (1.0 + globvv(slat,clat,tloc,f107a,f107,
                                               &pb1[15]) );
    if (xmr < 1e-3) xmr = 1e-3;
    h1 = pdmv[6][1] * pb1[46];
    zho = pdmv[5][1] * pb1[45];
    ymr = xmr * ccorv(pdmv[5][0],pdmv[4][1],h1,zho);

    /* Calculate mean mass */

    xmm = (44.0 + 44.0 * ymr + pdmv[1][5] * 28.0)/(1.0 + 2.0 * ymr + pdmv[1][5]);
    pdmv[4][0] = xmm;
    zhm = pdmv[3][0]/pb1[30];

//
//  CO2 density
//
    db44 = pdmv[0][0] *
           exp(globvv(slat,clat,tloc,f107a,f107,pa1))*pa1[0];
    d[1] = denssv(alt,db44,tinf,&tlb,44.0,0.0,&t[1],ptmv[5],s,t0,za,
                  &ta, zm, am, sp);
    zh44 = pdmv[2][0] * pa1[15];

    /* Get mixing density at zlb */

    xmd = 44.0 - xmm;
    b44 = denssv(zh44,db44,tinf,&tlb,xmd,-1.0,&tz,ptmv[5],s,t0,za,&ta,zm,
                 am,sp);
    dm44 = denssv(alt,b44,tinf,&tlb,xmm,0.0,&tz,ptmv[5],s,t0,za,&ta,zm,
                  am,sp);
    d[1] = dnetv(d[1],dm44,zhm,xmm,44.0);

//
//  O density
//
    db16 = pdmv[0][1] *
           exp(globvv(slat,clat,tloc,f107a,f107,pb1))*pb1[0];
    d[2] = denssv(alt,db16,tinf,&tlb,16.0,0.0,&t[1],ptmv[5],s,t0,za,&ta,zm,
                  am, sp);
    dm16 = dm44 * xmr;
    d[2] = dnetv(d[2],dm16,zhm,xmm,16.0);
    d[2] *= ccorv(alt,pdmv[4][1],h1,zho);

//
//  Get O turbopause estimate
//
    dD16 = denssv(zh44,db16,tinf,&tlb,16.0,0.0,&tz,ptmv[5],s,t0,za,&ta,zm,
                  am, sp);
    dmz44 = denssv(zh44,b44,tinf,&tlb,xmm,0.0,&tz,ptmv[5],s,t0,za,&ta,zm,
                   am, sp);
    turbov(dD16,dmz44*xmr,zh44,&zh16,16.0,xmm,tz);

    pdmv[2][1] = zh16;

//
//  CO density
//
    db29 = pdmv[0][2] *
           exp(globvv(slat,clat,tloc,f107a,f107,pc1))*pc1[0];
    d[3] = denssv(alt,db29,tinf,&tlb,28.0,0.0,&t[1],ptmv[5],s,t0,za,&ta,zm,
                  am, sp);
    dm29 = dm44 * xmr;
    d[3] = dnetv(d[3],dm29,zhm,xmm,28.0);
    d[3] *= ccorv(alt,pdmv[4][1],h1,zho);

//
//  He density
//
    db04 = pdmv[0][3] *
           exp(globvv(slat,clat,tloc,f107a,f107,pd1))*pd1[0];
    d[4] = denssv(alt,db04,tinf,&tlb,4.0,-0.60,&t[1],ptmv[5],s,t0,za,&ta,
                  zm,am, sp);
    dm04 = dm44 * pdmv[1][3] * pd1[15];
    d[4] = dnetv(d[4],dm04,zhm,xmm,4.0);

//
//  N density
//
    db14 = pdmv[0][4] *
           exp(globvv(slat,clat,tloc,f107a,f107,pe1))*pe1[0];
    d[5] = denssv(alt,db14,tinf,&tlb,14.0,0.0,&t[1],ptmv[5],s,t0,za,&ta,
                  zm,am, sp);
    zh14 = zh16;
    pdmv[2][4] = zh14;
    xmd = 14.0 - xmm;
    b14 = denssv(zh14,db14,tinf,&tlb,xmd,-1.0,&tz,ptmv[5],s,t0,za,&ta,zm,
                 am,sp);
    dm14 = denssv(alt,b14,tinf,&tlb,xmm,0.0,&tz,ptmv[5],s,t0,za,&ta,zm,
                  am,sp);
    d[5] = dnetv(d[5],dm14,zhm,xmm,14.0);
    d[5] *= ccorv(alt,pdmv[4][4],pdmv[6][4],pdmv[5][4]);

//
//  N2 density
//
    db28 = pdmv[0][5] *
           exp(globvv(slat,clat,tloc,f107a,f107,pf1))*pf1[0];
    d[6] = denssv(alt,db28,tinf,&tlb,28.0,0.0,&t[1],ptmv[5],s,t0,za,&ta,zm,
                  am, sp);
    dm28 = dm44 * pdmv[1][5];
    d[6] = dnetv(d[6],dm28,zhm,xmm,28.0);

//
//  Total mass density
//
    d[0] = 1.66e-24 * (44*d[1]+16*d[2]+28*d[3]+4*d[4]+14*d[5]+28*d[6]);

//
//  Multiplication factor for onms densities
//
    for (i=0; i<7; i++)
      d[i] *= pdmv[7][0];

    return 1;

}
//====================================================================


#include "val_atm_jr.h"
#include <math.h>
#include <stdio.h>
//====================================================================
//  VAL -- Virtual Almanac Library
//====================================================================
// Purpose: A collection of routines to support atmospheric drag, including
//          the Jacchia-Roberts drag model
//====================================================================
static void deflate_polynomial (const double c[], const short& n,
   const double& root, double c_new[])
// Purpose:   Given a polynomial P(z) of order n-1 specified by its real
//            coefficients c[n-1] to c[0], along with a single real root   
//            of the polynomial, this function divides the polynomial      
//            by (z - root) to arrive at a new set of coefficients,
//            c_new[n-2] to c_new[0].                                      
//                                                                         
// Notes:     This routine is designed to allow calls where c[] and
//            c_new[] are the same array, e.g.,
//                                                                         
//                 deflate_polynomial(c, 5, 3.5, c);                       
//                                                                         
//            Note however that only c_new[0] to c_new[n-2] are touched,   
//            c_new[n-1] is not, so that if c[] and c_new[] are the same,
//            the highest order coefficient of c[] is not zeroed out.
//
// Reference: Numerical Recipes in C, section 5.3.
   {
   double sum = c[n-1];

   for (int i = n - 2; i >= 0; i--)
      {
      double save = c[i];
      c_new[i] = sum;
      sum = save + sum * root;
      }
   }
//====================================================================
static void roots (const double a[], const short& na,
   double croots[][2], const short& irl)
// Purpose  -  To compute any desired complex roots (zeroes) of a given
//         polynomial if approximations to the desired roots are known.
//
// Return Value Type - void
//
// Calling Description - roots(double a[], short na, double croots[][2], short irl);
//
// Argument      Type        Description                  Use (I/O)
// --------      ----        -----------                  ------------
//   a           double      An array of non-imaginary         I
//                       polynomial coefficients,
//                      starting with the lowest
//                        power.
//
//   na          short       The number of coefficients,       I
//                        one greater than the degree
//                       of the polynomial.
//
//   croots      double      A 2-dimensional array of 'na'    I/O
//                       rows by 2 columns, containing
//                        the starting approximations to
//                       the desired roots on input and
//                        the solutions on output.
//   irl         short       The number of roots (zeroes)      I
//                        desired to be solved for.
//
// Notes:  This module was converted from preexisting Fortran code
//        written by J. P. Molineaux of CSC.  Mr. Molineaux based
//        the original code on Newton's method described in the
//        following reference:
//
//        Henrici, P. "ELEMENTS OF NUMERICAL ANALYSIS", NEW YORK,
//        WILEY, 1965, P. 84.
//
//        Curt Anderson of CSC provided mathematical formulas for
//        conversion of Fortran COMPLEX*16 variables to C formats.
//
   {
   double z[2], zs[2], cb[2], cc[2], dif, denom, temp;

   int ir = 0;                       /* ir counts roots solved to this point */
   int n1 = na - 1;
   int n2 = n1 - 1;
   dif = 0.0l;

    /* While the number of roots solved to this point (ir) has not exceeded */
    /* the number passed to this function (irl), perform the following */
    /* iterations. */

   while (ir < irl)
      {
      z[0] = croots[ir][0];       /* z is the currently iterated value */
      z[1] = croots[ir][1];       /* of the root */

      do                          /* Iterate while the convergence criterion */
         {                          /* is greater than the defined limit */
         cb[0] = a[n1];            /* Real component of complex number */
         cb[1] = 0;                /* Imaginary component of complex number */
         cc[0] = a[n1];            /* Real component of complex number */
         cc[1] = 0;                /* Imaginary component of complex number */

            /* cc and cb are the values of the polynomial (P(z)) */
            /* and its' derivative (P'(z)). */

         for (int i = 0; i <= n2; ++i)
            {
            int j = n2 - i;
            temp = (z[0]*cb[0] - z[1]*cb[1]) + a[j];
            cb[1] =  z[0]*cb[1] + z[1]*cb[0];
            cb[0] = temp;
            if (j != 0)
               {
               temp = (z[0]*cc[0] - z[1]*cc[1]) + cb[0];
               cc[1] = (z[0]*cc[1] + z[1]*cc[0]) + cb[1];
               cc[0] = temp;
               }
            }
         zs[0] = z[0];             /* Result of last iteration saved in zs */
         zs[1] = z[1];

         /* Newton's Method: Z(I) = Z(I-1) - P(Z(I-1)) / P'(Z(I-1)) */
         /* tends towards a root of the polynomial P(Z). */
         denom =   cc[0]*cc[0] + cc[1]*cc[1];
         z[0] -= ((cb[0]*cc[0] + cb[1]*cc[1]) / denom);
         z[1] += ((cb[0]*cc[1] - cb[1]*cc[0]) / denom);

         /* The convergence criterion (dif) is the sum of the relative */
         /* changes in the real and imaginary parts of the root (z). */

         dif = fabs( (zs[0] - z[0]) / zs[0]);
         if(zs[1] != 0.0E0)
            {
            dif += fabs( (zs[1] - z[1]) / zs[1]);
            }
         }
      while (dif > 1.0E-14);  /* If convergence criterion is too large */
                              /* iterate again */

      /* Convergence criterion is complete.  Store the answer in the */
      /* output array 'croots' and start on the next root. */

      croots[ir][0] = z[0];
      croots[ir][1] = z[1];
      ++ir;                   /* Increment roots solved counter */
      }
   }
//====================================================================
static double rho_cor (const double& height, const double& a1_time,
   const double& geo_lat, const double& tkp, const double& xtemp)
   {
   /* Compute geomagnetic activity correction */
   double geo_cor;
   if (height < 200.0)
      geo_cor = 0.012 * tkp + 0.000012 * exp(tkp);
   else
      geo_cor = 0.0;

   /* Compute semiannual variation correction */
   double f = (5.876e-7 * pow(height, 2.331) + 0.06328) *
      exp(-0.002868 * height);
   double day_58 = (a1_time - 6204.5)/365.2422;
   double tausa = day_58 + 0.09544 * (
                   pow( 0.5*(1.0 + sin(2*KReal::PI*day_58 +6.035)), 1.65 )  - 0.5);
   double alpha = sin(4.0*KReal::PI*tausa + 4.259);
   double g = 0.02835 + (0.3817 + 0.17829 * sin(2*KReal::PI*tausa + 4.137)) * alpha;
   double semian_cor = f * g;

   /* Compute seasonal latitudinal variation */
   double sin_lat = sin(geo_lat);
   double eta_lat = sin(2.0*KReal::PI*day_58 + 1.72) * sin_lat * fabs(sin_lat);
   double slat_cor = 0.014 * (height - 90.0) * eta_lat *
             exp(-0.0013 * (height - 90.0) * (height - 90.0));

   return pow(10.0, geo_cor + semian_cor + slat_cor);
   }
//====================================================================
static const double ra         = 6356.766;        // Average radius of the earth (km)
static const double flat       = 3.3528133297e-3; // Earth "flattening factor"
static const double avogadro   = 6.022045e23;     // Avogadro's number
static const double g_zero     = 9.80665;         // earth gravitational constant m/sec**2
static const double gas_con    = 8.31432;         // gas constant (joules/(degK-mole))
static const double rho_zero   = 3.46e-9;         // low altitude density in g/cm**2
static const double tzero      = 183.0;           // Temperature at height of 90km in degrees kelvin
//====================================================================
//  Auxiliary temperature related quantities
static const double con_c[5]  = {  // Constants for series expansion
      -89284375.0,             // below 125 km. The function it defines
       3542400.0,              // interpolates uniformly between 90 km  
       -52687.5,               // and 125km, being -1 at height 90km
          340.5,               // and zero at height 125km.
           -0.8
};
static const double con_l[5]  = {  // Constants for series expansion
       0.1031445e5,            // above 125 km.
       0.2341230e1,
       0.1579202e-2,
      -0.1252487e-5,
       0.2462708e-9
};

// Constants required between 90 km  and 100 km
static const double mzero = 28.82678;
static const double m_con[7] = {   // Constants for series expansion
      -435093.363387,        // of M(z) function
       28275.5646391,        // (1/km)
       -765.33466108,        // (1/km**2)
         11.043387545,       // (1/km**3)
         -0.08958790995,     // (1/km**4)
          0.00038737586,     // (1/km**5)
         -0.000000697444     // (1/km**6)
};

static const double s_con[6] = {      /* Constants for series expansion */
    3144902516.672729,          /* of S(z) function */
    -123774885.4832917,         /* (1/km)    */
       1816141.096520398,       /* (1/km**2) */
       -11403.31079489267,      /* (1/km**3) */
          24.36498612105595,    /* (1/km**4) */
           0.008957502869707995 /* (1/km**5) */
};

static const double s_beta[6] = {    /* Constants for series expansion */
   -52864482.17910969,         /* of S(z) function - temperature part */
      -16632.50847336828,      /* (1/km)    */
        -1.308252378125,       /* (1/km**2) */
         0.0,                  /* (1/km**3) */
         0.0,                  /* (1/km**4) */
         0.0                   /* (1/km**5) */
};

/* Constants required for attitudes between 100 km and 125 km */

static const double omega = -0.94585589;

static const double zeta_con[7] = { /* Constants for series expansion */
      0.1985549e-10,            /* of 100 to 125 km density function */
     -0.1833490e-14,            /* (1/deg)    */
      0.1711735e-17,            /* (1/deg**2) */
     -0.1021474e-20,            /* (1/deg**3) */
      0.3727894e-24,            /* (1/deg**4) */
     -0.7734110e-28,            /* (1/deg**5) */
      0.7026942e-32             /* (1/deg**6) */
};

static    const double mol_mass[6] = { /* Molecular masses of atmospheric
                               constituents in grams/mole */
      28.0134,                  /* Nitrogen  */
      39.948,                   /* Argon     */
      4.0026,                   /* Helium    */
      31.9988,                  /* Molecular Oxygen */
      15.9994,                  /* Atomic Oxygen  */
      1.00797                   /* Hydrogen */
};

static    const double num_dens[5] = { /* Number density divided by Avogadro's
                              number of atmospheric constituents */
      0.78110,                  /* Nitrogen  */
      0.93432e-2,               /* Argon     */
      0.61471e-5,               /* Helium    */
      0.161778,                 /* Molecular Oxygen */
      0.95544e-1                /* Atomic Oxygen  */
};

   /* Constants required for altitude greater than 125 km */

static    const double con_den[5][7] = { /* Polynomial coefficients for constituent
                                 densities of each atmospheric gas */

                               /* Nitrogen */
      {0.1093155e2,
       0.1186783e-2,              /* (1/deg)    */
      -0.1677341e-5,              /* (1/deg**2) */
       0.1420228e-8,              /* (1/deg**3) */
      -0.7139785e-12,             /* (1/deg**4) */
       0.1969715e-15,             /* (1/deg**5) */
      -0.2296182e-19},            /* (1/deg**6) */
                               /* Argon */
      {0.8049405e1,
       0.2382822e-2,              /* (1/deg)    */
      -0.3391366e-5,              /* (1/deg**2) */
       0.2909714e-8,              /* (1/deg**3) */
      -0.1481702e-11,             /* (1/deg**4) */
       0.4127600e-15,             /* (1/deg**5) */
      -0.4837461e-19},            /* (1/deg**6) */
                              /* Helium */
      {0.7646886e1,
      -0.4383486e-3,              /* (1/deg)    */
       0.4694319e-6,              /* (1/deg**2) */
      -0.2894886e-9,              /* (1/deg**3) */
       0.9451989e-13,             /* (1/deg**4) */
      -0.1270838e-16,             /* (1/deg**5) */
       0.0},                      /* (1/deg**6) */
                              /* Molecular Oxygen */
      {0.9924237e1,
       0.1600311e-2,              /* (1/deg)    */
      -0.2274761e-5,              /* (1/deg**2) */
       0.1938454e-8,              /* (1/deg**3) */
      -0.9782183e-12,             /* (1/deg**4) */
       0.2698450e-15,             /* (1/deg**5) */
      -0.3131808e-19},            /* (1/deg**6) */
                              /* Atomic Oxygen */
      {0.1097083e2,
       0.6118742e-4,              /* (1/deg)    */
      -0.1165003e-6,              /* (1/deg**2) */
       0.9239354e-10,             /* (1/deg**3) */
      -0.3490739e-13,             /* (1/deg**4) */
       0.5116298e-17,             /* (1/deg**5) */
       0.0}                       /* (1/deg**6) */
};







//====================================================================
static double calcpoly (const double coef[],
   const double& x, const int& order)
   {
   double out = coef[order];
   for (int i=order-1; i>=0; i--)
      out = out * x + coef[i];
   return out;
   }
//====================================================================
// Structure used to pass output of exotherm function
class ExoTemps {
public:
   double temp;
   double t_infinity;
   double tx;
   double sum;
   double root1;
   double root2;
   double x_root;
   double y_root;
   };
//--------------------------------------------------------------------
static ExoTemps exotherm (const double space_craft[2],
   const double sun[3], const double& tkp, const double& xtemp,
   const double& height, const double& sun_dec, const double& geo_lat)
// purpose - Compute the temperature of earth's atmosphere and auxiliary
//       temperature related quantities at a given altitude.
//
// type - double
//
// calling sequence -
//
//    space_craft[2]           Spacecraft position (km)
//    sun                      Sun unit vector
//    tkp                      double   Kp value
//    xtemp                    double   Exospehric temperature
//    height                   Spacecraft height (km)
//
// global variables -
//
//    con_c                    Series expansion coefficients for height < 125 km
//                            (in jr_drag header file)
//    con_l                    Series expansion coefficients for height > 125 km
//                            (in jr_drag header file)
   {
   ExoTemps exotemps;
   double c_star[5], aux[4][2];
   const short int na=5;

   /* Compute hour angle of the sun */
   double sun_denom = sqrt(sun[0]*sun[0] + sun[1]*sun[1]);
   double hour_angle = ( (sun[0]*space_craft[1]-sun[1]*space_craft[0])/
                fabs(sun[0]*space_craft[1]-sun[1]*space_craft[0]))
                *acos((sun[0]*space_craft[0]+sun[1]*space_craft[1])/
                (sun_denom * sqrt(space_craft[0]*space_craft[0] +
                space_craft[1]*space_craft[1])));

   /* Compute sun and spacecraft position dependent part of temperature */
   double theta = 0.5 * fabs(geo_lat + sun_dec);
   double eta = 0.5 * fabs(geo_lat - sun_dec);
   double tau = hour_angle - 0.64577182325 + 0.10471975512 *
         sin(hour_angle + 0.75049157836);
   if (tau < -KReal::PI)
     tau += 2 * KReal::PI;
   else if (tau > KReal::PI)
     tau -= 2 * KReal::PI;
   double th22 = pow(sin(theta), 2.2);
   double t1 = xtemp * (1.0 + 0.3*(th22 +pow(cos(0.5*tau),3.0) *
                      (pow(cos(eta),2.2) - th22)));
   double expkp = exp(tkp);

/* Compute t infinity based on altitude  */
   if (height < 200.0)
      exotemps.t_infinity =   t1 + 14.0 * tkp + 0.02 * expkp;
   else
      exotemps.t_infinity = t1 + 28.0 * tkp + 0.03 * expkp;
   exotemps.tx = 371.6678 + 0.0518806*exotemps.t_infinity
      -294.3505*exp(-0.0021622*exotemps.t_infinity);

/* If the spacecraft altitude is below 125 km then */
   if (height < 125.0)
      {
      /* Compute height dependent polynomial */
      exotemps.sum = calcpoly (con_c,height,4);
      /* Compute temperature */
      exotemps.temp = exotemps.tx + (exotemps.tx - tzero)*exotemps.sum/1.500625e6;
      }
/* else if spacecraft altitude is above 125 km then */
   else if (height > 125.0)
      {
      /* Compute temperature dependent polynomial */
      exotemps.sum = calcpoly (con_l,exotemps.t_infinity,4);
      /* Compute temperature */
      exotemps.temp = exotemps.t_infinity - (exotemps.t_infinity - exotemps.tx) *
                   exp(-(exotemps.tx-tzero)/(exotemps.t_infinity - exotemps.tx) *
                       (height - 125.0)/35.0 *
                       exotemps.sum/(ra + height) );
      }
   else
      exotemps.temp = exotemps.tx;

   if (height <= 125.0)
      {
      /* Obtain coefficients of polynomial for auxiliary quantities
         required for heights less than 125 km  */
      c_star[0]=con_c[0] + 1500625.0 * exotemps.tx/(exotemps.tx-tzero);
      for (int i=1; i<=4; i++)
         c_star[i] = con_c[i];

      /* Set approximate values for solution of auxiliary quantities */
      //      aux[0][0] = 125.0;
      //      aux[0][1] = 0.0;
      //      aux[1][0] = 200.0;
      //      aux[1][1] = 0.0;
      //      aux[2][0] = 10.0;
      //      aux[2][1] = 125.0;
      //      aux[3][0] = 10.0;
      //      aux[3][1] = -25.0;

      /* Solve polynomial equation for auxiliary quantities */
      //      roots(c_star, na, aux, irl);

      aux[0][0] = 125.0;            // get 1st real root
      aux[0][1] = 0.0;
      roots(c_star, na, aux, 1);
      exotemps.root1 = aux[0][0];

      deflate_polynomial(c_star, na, exotemps.root1, c_star);

      aux[0][0] = 200.0;            // get 2nd real root
      aux[0][1] = 0.0;
      roots(c_star, na-1, aux, 1);
      exotemps.root2 = aux[0][0];

      deflate_polynomial(c_star, na-1, exotemps.root2, c_star);

      aux[0][0] = 10.0;            // get remaining roots
      aux[0][1] = 125.0;
      roots(c_star, na-2, aux, 1);
      exotemps.x_root = aux[0][0];
      exotemps.y_root = fabs(aux[0][1]);

      /* Extract needed quantities from polynomial solutions */
//      root1 = aux[0][0];
//      root2 = aux[1][0];
//      x_root = aux[2][0];
//      y_root = fabs(aux[2][1]);
      }
   return exotemps;
   }
//--------------------------------------------------------------------
static double rho_125(const double& height, const ExoTemps& exotemps)
// Purpose: Compute density of the atmosphere between 100 and 125 km
//          using the Jacchia-Roberts density model.
//
// Return Value Type - double
//
// Calling Sequence -
// Argument     Type     Description                            Use
// --------     ------   -----------------------------------    ----
// height       double   Spacecraft altitude (km)                 I
// temperature  double   Exospheric temperature at spacecraft     I
//                       altitude (degrees K)
   {
   // local copies for readability
   double root1 = exotemps.root1;
   double root2 = exotemps.root2;
   double x_root = exotemps.x_root;
   double y_root = exotemps.y_root;
   // Compute base density polynomial
   double rho_prime = calcpoly (zeta_con,exotemps.t_infinity,6);
   // Compute base temperature
   double t_100 = exotemps.tx + omega * (exotemps.tx - tzero);

   // Compute functions of auxiliary temperature values
   double roots_2 = x_root*x_root + y_root*y_root;
   double x_star = -2.0* root1 * root2 * ra * (ra*ra + 2.0*ra*x_root + roots_2);
   double v = (ra + root1)*(ra +root2) * (ra*ra + 2.0*ra*x_root + roots_2);
   double u[2];
   u[0] = (root1 - root2) * (root1 + ra) * (root1 + ra) *
       (root1*root1 - 2.0*root1*x_root + roots_2);
   u[1] = (root1 - root2) * (root2 + ra) * (root2 + ra) *
       (root2*root2 - 2.0*root2*x_root + roots_2);
   double w[2];
   w[0] = root1 * root2 * ra * (ra + root1) * (ra + roots_2/root1);
   w[1] = root1 * root2 * ra * (ra + root2) * (ra + roots_2/root2);

   /* Compute power of second quantity in f3 function */
   double q2 = 1.0/u[0];

   /* Compute power of third quantity in f3 function */
   double q3 = -1.0/u[1];

   /* Compute q5 factor in f4 function */
   double q5 = 1.0/v;

   /* Compute power of fourth quantity in f3 function */
   double q4 = (1.0 + w[0]*q2 + w[1]*q3
       + root1*root2*(ra*ra - roots_2)*q5)/x_star;

   /* Compute power of first quantity in f3 function */
   double q1 = - 2*q4 - q3 - q2;

   /* Compute q6 factor in f4 function */
   double q6 = - q5 - 2.0*(x_root + ra)*q4 - (root2 + ra)*q3 - (root1 +ra)*q2;

   /* Compute log of f3 function */
   double log_f3 =  q1 * log( (height + ra)/(100.0 + ra))
        +  q2 * log( (height - root1)/(100.0 - root1))
        +  q3 * log( (height - root2)/(100.0 - root2))
        +  q4 * log( (height*height - 2.0*x_root*height + roots_2)
            /(1.0e4 - 200.0*x_root + roots_2));

   /* Compute f4 function */
   double f4 = (height - 100.0) * q5 / ((height + ra) * (100.0 + ra))
       + q6 * atan(y_root * (height - 100.0)/(
       y_root*y_root + (height - x_root)*(100.0 - x_root)))/ y_root;

   /* Compute f3 power */
   double factor_k = -1500625.0*g_zero*ra*ra/(gas_con*con_c[4]*(exotemps.tx-tzero));

   /* Compute mass-dependent sum */
   double rho_sum = 0.0;
   for (int i=0; i<=4; i++)
      {
      double rhoi = mol_mass[i] * num_dens[i] *
          exp(mol_mass[i] * factor_k * (f4 + log_f3));

      if (i == 2)
         {
         rhoi *= pow(t_100/exotemps.temp, -0.38);
         }
      rho_sum += rhoi;
      }
   return rho_sum * rho_prime * t_100 / exotemps.temp;
   }
//--------------------------------------------------------------------
static double rho_high (const double& height, const ExoTemps& exotemps,
   const double& t_500, const double& sun_dec, const double& geo_lat)
// Purpose: Compute density of the atmosphere between 125 and 2500 km
//          using the Jacchia-Roberts density model.
//
// Return Value Type - double
//
// Calling Sequence -
// Argument     Type     Description                            Use
// --------     ------   -----------------------------------    ----
// height       double   Spacecraft altitude (km)                 I
// temperature  double   Exospheric temperature at spacecraft     I
//                       altitude (degrees K)
// t_500        double   Exospheric temperature at altitude       I
//                       of 500 km (degrees K)
// sun_dec      double   Declination of the sun in TOD            I
//                       coordinates (radians)
// geo_lat      double   Geodetic latitude of spacecraft          I
//                       (radians)
   {
   double rho_out = 0.0;
   for (int i = 0; i <= 5; i++)
      {
      // Compute constituent density sum for this atmospheric component
      double di;
      if (i <= 4)       // (skip 5 => hydrogen)
         {
         double log_di=con_den[i][6];
         for (int j=5; j>=0; j--)
            {
            log_di = log_di*exotemps.t_infinity + con_den[i][j];
            }
         di = pow(10.0, log_di)/avogadro;
         }

      // Compute second exponent in density expression for this component
      double gamma =   35.0 * mol_mass[i] * g_zero * ra * ra *
         (exotemps.t_infinity - exotemps.tx) /
         ( gas_con * exotemps.sum * exotemps.t_infinity *
         (exotemps.tx - tzero) * 6481.766);

      // Compute first exponent in density expression for this component
      double exp1 = 1.0 + gamma;

      // A factor which is non-unity only for helium
      double f = 1.0;

      // Compute corrections for helium
      if (i == 2)
         {
         exp1 -= 0.38;
         f = 4.9914 * fabs(sun_dec) * (
         power(sin(0.25 * KReal::PI - 0.5 * geo_lat * sun_dec / fabs(sun_dec)), 3)
            - 0.35355) / KReal::PI;
         f = pow(10.0, f);
         }

      // Add corrections to rho_out, skip hydrogen unless above 500 km
      double r;
      if (height > 500.0 && i == 5)
         {
         r = mol_mass[5] * pow(10.0, 73.13 - (39.4 - 5.5 * log10(t_500))
                                  * log10(t_500))
                   * pow(t_500/exotemps.temp, exp1)
                   * pow((exotemps.t_infinity - exotemps.temp) / (exotemps.t_infinity - t_500),
                                                                        gamma)
                    /avogadro;
         rho_out += r;
         }
      else if (i <= 4)
         {
         r = f * mol_mass[i] * di * pow(exotemps.tx/exotemps.temp, exp1)
                * pow((exotemps.t_infinity - exotemps.temp)/
                (exotemps.t_infinity - exotemps.tx), gamma);
         rho_out += r;
         }
      }
   return rho_out;
   }
//--------------------------------------------------------------------
static double rho_100 (const double& height, const ExoTemps& exotemps)
// Purpose: Compute density of the atmosphere between 90 and 100 km
//          using the Jacchia-Roberts density model.
//
// Return Value Type - double
//
// Calling Sequence -
// Argument     Type     Description                            Use
// --------     ------   -----------------------------------    ----
// height       double   Spacecraft altitude (km)                 I
// temperature  double   Exospheric temperature at spacecraft     I
//                       altitude (degrees K)
   {
   // local copies for readability
   double root1 = exotemps.root1;
   double root2 = exotemps.root2;
   double x_root = exotemps.x_root;
   double y_root = exotemps.y_root;
   /* Compute M(z) polynomial */
   double m_poly = calcpoly (m_con,height,6);

   /* Compute temperature dependent coefficients */
   double b[6];
   for (int i=0; i<=5; i++)
      b[i] = s_con[i] + s_beta[i] * exotemps.tx/(exotemps.tx-tzero);

   /* Compute functions of auxiliary temperature values */
   double roots_2 = x_root*x_root + y_root*y_root;
   double x_star = -2.0* root1 * root2 * ra * (ra*ra + 2.0*ra*x_root + roots_2);
   double v = (ra + root1)*(ra +root2) * (ra*ra + 2.0*ra*x_root + roots_2);
   double u[2];
   u[0] = (root1 - root2) * (root1 + ra) * (root1 + ra) *
        (root1*root1 - 2.0*root1*x_root + roots_2);
   u[1] = (root1 - root2) * (root2 + ra) * (root2 + ra) *
        (root2*root2 - 2.0*root2*x_root + roots_2);
   double w[2];
   w[0] = root1 * root2 * ra * (ra + root1) * (ra + roots_2/root1);
   w[1] = root1 * root2 * ra * (ra + root2) * (ra + roots_2/root2);

   /* Compute S(z) polynomial for z = root1 */
   /* Compute power of second quantity in f1 function */
   double p2 = calcpoly(b,root1,5)/u[0];
   /* Compute S(z) polynomial for z = root2 */
   /* Compute power of third quantity in f1 function */
   double p3 = -calcpoly(b,root2,5)/u[1];
   /* Compute S(z) polynomial for z = negative earth radius */
   /* Compute p5 factor in f2 function */
   double p5 = calcpoly (b,-ra,5)/v;

   /* Compute power of fourth quantity in f1 function */
   double p4 = (b[0] - root1*root2*ra*ra*(b[4] + b[5]*
       (2.0*x_root + root1 + root2 - ra)) + w[0]*p2 + w[1]*p3
       - root1*root2*b[5]*ra*roots_2
       + root1*root2*(ra*ra - roots_2)*p5)/x_star;

   /* Compute power of first quantity in f1 function */
   double p1 = b[5] - 2*p4 - p3 - p2;

   /* Compute p6 factor in f2 function */
   double p6 = b[4] + b[5]*(2.0*x_root + root1 + root2 - ra) - p5
       - 2.0*(x_root +ra)*p4 - (root2 + ra)*p3 - (root1 +ra)*p2;

   /* Compute natural log of f1 function */
   double log_f1 =   p1 * log( (height + ra)/(90.0 + ra))
         +   p2 * log( (height - root1)/(90.0 - root1))
         +   p3 * log( (height - root2)/(90.0 - root2))
         +   p4 * log( (height*height - 2.0*x_root*height + roots_2)
             /(8100.0 - 180.0*x_root + roots_2));

   /* Compute f2 function */
   double f2 = (height - 90.0) * (m_con[6] + p5/((height + ra)*(90.0 + ra)))
       + p6 * atan(y_root * (height - 90.0)/(
        y_root*y_root + (height - x_root)*(90.0 - x_root)))/ y_root;

   /* Compute f1 power */
   double factor_k = -1500625.0*g_zero*ra*ra/(gas_con*con_c[4]*(exotemps.tx-tzero));

   return rho_zero * tzero * m_poly * exp(factor_k*(log_f1 + f2))
       /(mzero * exotemps.temp);
   }
//--------------------------------------------------------------------
double jac_rob (const double& height,
   const double space_craft[3], const double sun[3],
   const double& a1_time, const double& tkp, const double& xtemp)
// Purpose: Obtain atmospheric density using the Jacchia-Roberts model
//
// Return Value Type - double
//
// Calling Sequence -
// Argument     Type     Description                            Use
// --------     ------   -----------------------------------    ----
// height       double   Spacecraft height (km)                  I
// space_craft  double   Spacecraft position (km, TOD GCI)       I
// sun          double   Sun unit vector (TOD GCI)               I
// a1_time      double   Reduced julian date (days)              I
// tkptr        FILE     Pointer to Jacchia-Roberts exospheric   I
//                       temperature and geomagnetic index file
// new_file     boolean* if true, flush static data for file     I
// istat        short    Return code from attempts to read       O
//                       above file
//
// Functions Called - jaccwf, rho_100, rho_125, rho_high, exotherm
//
// File References - none (but see jaccwf)
//
// External (Global) Variables -
// Variable   Description                        Include File
// --------   --------------------------------   --------------------------
// flat       "Flattening" factor for earth's    jac_rob.h
//        oblateness
// rho_zero   Standard density of earth's        jac_rob.h
//        atmosphere
//
   {
   /* Compute declination of the sun */
   double sun_dec = atan2(sun[2], sqrt(sun[0]*sun[0] + sun[1]*sun[1]));

   /* Compute geodetic latitude of spacecraft */
   double geo_lat = atan2(space_craft[2],
                   sqrt(space_craft[0]*space_craft[0] +
                        space_craft[1]*space_craft[1]) *
                   ((1.0-flat)*(1.0-flat)));

   /* Compute height dependent density */
   double density;
   ExoTemps exotemps;
   if (height<=90.0)
      {
      density = rho_zero;
      }
   else if (height < (double) 100.0)
      {
      exotemps = exotherm(space_craft, sun, tkp,xtemp, height,sun_dec, geo_lat);
      density = rho_100(height, exotemps);
      }
   else if (height <= (double) 125.0)
      {
      exotemps = exotherm(space_craft, sun, tkp,xtemp, height,sun_dec, geo_lat);
      density = rho_125(height, exotemps);
      }
   else if (height <= (double) 2500.0)
      {
      ExoTemps t_500 = exotherm(space_craft, sun, tkp,xtemp, 500.0, sun_dec,geo_lat);
      exotemps = exotherm(space_craft, sun, tkp,xtemp, height,sun_dec, geo_lat);
      density = rho_high(height, exotemps, t_500.temp, sun_dec, geo_lat);
      }
   else
      {
      density = 0.0;
      }
   return density * rho_cor(height, a1_time, geo_lat, tkp,xtemp);
   }
//====================================================================


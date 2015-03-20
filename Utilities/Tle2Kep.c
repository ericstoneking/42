#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/**********************************************************************/
/*  Find Day of Year, given Month, Day                                */
/*  Ref. Jean Meeus, 'Astronomical Algorithms', QB51.3.E43M42, 1991.  */

long MD2DOY(long Year, long Month, long Day)
{
      long K;

      if (Year % 4 == 0) {
         K = 1;
      }
      else {
         K = 2;
      }

      return(275*Month/9 - K*((Month+9)/12) + Day - 30);
}
/**********************************************************************/
/* AbsTime is elapsed seconds since J2000 epoch                       */
double JDToAbsTime(double JD)
{
      return((JD-2451545.0)*86400.0);
}
/**********************************************************************/
/*  Convert Year, Month, Day, Hour, Minute and Second to Julian Day   */
/*  Valid for any positive JD.                                        */
/*  Year, Month, Day assumed in Gregorian calendar. (Not true < 1582) */
/*  Ref. Jean Meeus, 'Astronomical Algorithms', QB51.3.E43M42, 1991.  */

double YMDHMS2JD(long Year, long Month, long Day, 
               long Hour, long Minute, double Second)
{
      long A,B;
      double JD;

      if (Month < 3) {
         Year--;
         Month+=12;
      }

      A = Year/100;
      B = 2 - A + A/4;

      JD = floor(365.25*(Year+4716)) + floor(30.6001*(Month+1))
           + Day + B - 1524.5;

      JD += ((double) Hour) / 24.0 +
            ((double) Minute) / 1440.0 +
            Second / 86400.0;

      return(JD);
}
/**********************************************************************/
/*  Find Month, Day, given Day of Year                                */
/*  Ref. Jean Meeus, 'Astronomical Algorithms', QB51.3.E43M42, 1991.  */

void DOY2MD(long Year, long DayOfYear, long *Month, long *Day)
{
      long K;

      if (Year % 4 == 0) {
         K = 1;
      }
      else {
         K = 2;
      }

      if (DayOfYear < 32) {
         *Month = 1;
      }
      else {
         *Month = (long) (9.0*(K+DayOfYear)/275.0+0.98);
      }

      *Day = DayOfYear - 275*(*Month)/9 + K*(((*Month)+9)/12) + 30;

}
/**********************************************************************/
double TrueAnomaly(double mu, double p, double e, double t)
{
#define EPS (1.0E-12)
#define PI    (3.141592653589793)
#define TWOPI (6.283185307179586)

      double p3,B,x,f,fp,e1,N,H,M,E,a;
      double Ne,dx,dH,dE,Anom;
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
         i = 0;
         a = p/(1.0-e*e);
         M = sqrt(mu/(a*a*a))*t;
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
void TLE2Eph(const char Line1[80], const char Line2[80], double JD,
   double mu, double *SMA, double *e, double *i, double *RAAN,
   double *ArgP, double *th, double *tp, double *SLR, 
   double *alpha, double *rmin, double *Period, double *MeanMotion)
{
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
      double FloatDOY,FracDay,MeanAnom,JDepoch;
      double Epoch,AbsTime;

      strncpy(YearString,&Line1[18],2);
      year = (long) atoi(YearString);
      if (year < 57) year += 2000;
      else year += 1900;
      strncpy(DOYstring,&Line1[20],12);
      FloatDOY = (double) atof(DOYstring);
      DOY = (long) FloatDOY;
      FracDay = FloatDOY - ((double) DOY);
      DOY2MD(year,DOY,&Month,&Day);
      JDepoch = YMDHMS2JD(year,Month,Day,0,0,0.0);
      JDepoch += FracDay;
      Epoch = JDToAbsTime(JDepoch);
      AbsTime = JDToAbsTime(JD);
      
      strncpy(IncString,&Line2[8],8);
      *i = ((double) atof(IncString))*D2R;

      strncpy(RAANstring,&Line2[17],9);
      *RAAN = ((double) atof(RAANstring))*D2R;

      strncpy(EccString,&Line2[26],7);
      *e = ((double) atof(EccString))*1.0E-7;

      strncpy(omgstring,&Line2[34],8);
      *ArgP = ((double) atof(omgstring))*D2R;

      strncpy(MeanAnomString,&Line2[43],8);
      MeanAnom = ((double) atof(MeanAnomString))*D2R;

      strncpy(MeanMotionString,&Line2[52],11);
      *MeanMotion = ((double) atof(MeanMotionString))*TWOPI/86400.0;
      *Period = TWOPI/(*MeanMotion);
      
      /* Time of Periapsis passage given in seconds since J2000 */
      *tp = Epoch - MeanAnom/(*MeanMotion);
      while ((*tp-AbsTime) < -(*Period)) *tp += *Period;
      while ((*tp-AbsTime) >   *Period ) *tp -= *Period;
      
      *SMA = pow(mu/(*MeanMotion)/(*MeanMotion),1.0/3.0);
      *alpha = 1.0/(*SMA);
      *SLR = (*SMA)*(1.0-(*e)*(*e));
      *rmin = *SLR/(1.0 + *e);
      
      *th = TrueAnomaly(mu, *SLR, *e, AbsTime-(*tp));

#undef TWOPI
#undef D2R
}
/**********************************************************************/
int main(int argc, char **argv)
{
#define D2R (1.74532925199E-2)

      char Line1[80] = "1 33053U 08029A   13295.49203983  .00002156  00000-0  12025-3 0  4331";
      char Line2[80] = "2 33053  25.5837  63.0469 0014097   5.4700  58.4448 15.07392089295554";
      double JD;
      double mu = 3.98604E14;
      double Re = 6378.145E3;
      double SMA,ecc,inc,RAAN,ArgP,Anom,tp,SLR,alpha,rmin,Period,MeanMotion;
      long DOY;
      
      DOY = MD2DOY(2013,10,22);
     
      JD = YMDHMS2JD(2013,10,22,5,0,0.0);

      TLE2Eph(Line1,Line2,JD,mu,&SMA,&ecc,&inc,&RAAN,&ArgP,&Anom,
              &tp,&SLR,&alpha,&rmin,&Period,&MeanMotion);

      printf("JD:  %lf\n",JD);
      printf("DOY: %li\n",DOY);
      printf("Avg Alt: %lf km\n",(SMA-Re)/1000.0);
      printf("Ecc: %lf\n",ecc);
      printf("Inc: %lf deg\n",inc/D2R);
      printf("RAAN: %lf deg\n",RAAN/D2R);
      printf("ArgP: %lf deg\n",ArgP/D2R);
      printf("Anom: %lf deg\n",Anom/D2R);


      return(0);
}

/*    This file is distributed with 42,                               */
/*    the (mostly harmless) spacecraft dynamics simulation            */
/*    created by Eric Stoneking of NASA Goddard Space Flight Center   */

/*    Copyright 2010 United States Government                         */
/*    as represented by the Administrator                             */
/*    of the National Aeronautics and Space Administration.           */

/*    No copyright is claimed in the United States                    */
/*    under Title 17, U.S. Code.                                      */

/*    All Other Rights Reserved.                                      */


#include "timekit.h"

//#ifdef __cplusplus
//namespace Kit {
//#endif

/**********************************************************************/
/* AbsTime is elapsed seconds since J2000 epoch                       */
double AbsTimeToJD(double AbsTime)
{
      return(AbsTime/86400.0 + 2451545.0);
}
/**********************************************************************/
/* AbsTime is elapsed seconds since J2000 epoch                       */
double JDToAbsTime(double JD)
{
      return((JD-2451545.0)*86400.0);
}
/**********************************************************************/
/*  Convert Year, Month, Day, Hour, Minute and Second to              */
/*  "Absolute Time", i.e. seconds elapsed since J2000 epoch.          */
/*  J2000 = 2451545.0 TT  =  01 Jan 2000 12:00:00.00 TT               */
/*  Year, Month, Day assumed in Gregorian calendar. (Not true < 1582) */
/*  Ref. Jean Meeus, 'Astronomical Algorithms', QB51.3.E43M42, 1991.  */

double DateToAbsTime(long Year, long Month, long Day, long Hour,
   long Minute, double Second)
{
      long A,B;
      double Days;

      if (Month < 3) {
         Year--;
         Month+=12;
      }

      A = Year/100;
      B = 2 - A + A/4;

      /* Days since J2000 Epoch (01 Jan 2000 12:00:00.0) */
      Days = floor(365.25*(Year+4716))
                  + floor(30.6001*(Month+1))
                  + Day + B - 1524.5 - 2451545.0;

      /* Add fractional day */
      return(86400.0*Days + 3600.0*((double) Hour)
         + 60.0*((double) Minute) + Second);
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
/*   Convert Julian Day to Year, Month, Day, Hour, Minute, and Second */
/*   Ref. Jean Meeus, 'Astronomical Algorithms', QB51.3.E43M42, 1991. */

void JD2YMDHMS(double JD,long *Year, long *Month, long *Day,
                         long *Hour, long *Minute, double *Second)
{
      double Z,F,A,B,C,D,E,alpha;
		double FD;

      Z= floor(JD+0.5);
      F=(JD+0.5)-Z;

      if (Z < 2299161.0) {
         A = Z;
      }
      else {
         alpha = floor((Z-1867216.25)/36524.25);
         A = Z+1.0+alpha - floor(alpha/4.0);
      }

      B = A + 1524.0;
      C = floor((B-122.1)/365.25);
      D = floor(365.25*C);
      E = floor((B-D)/30.6001);

      FD = B - D - floor(30.6001*E) + F;
      *Day = (long) FD;

      if (E < 14.0) {
         *Month = (long) (E - 1.0);
         *Year = (long) (C - 4716.0);
      }
      else {
         *Month = (long) (E - 13.0);
         *Year = (long) (C - 4715.0);
      }

		FD = FD - floor(FD);
		FD = FD * 24.0;
		*Hour = (long) FD;
		FD = FD - floor(FD);
		FD = FD * 60.0;
		*Minute = (long) FD;
		FD = FD - floor(FD);
		*Second = FD * 60.0;

}
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
/*  Find Greenwich Mean Sidereal Time (GMST)                          */
/*  Ref. Jean Meeus, 'Astronomical Algorithms', QB51.3.E43M42, 1991.  */
/*  GMST is output in units of days.                                  */

double JD2GMST(double JD)
{
      double T,JD0,GMST0,GMST;

      JD0 = floor(JD)+0.5;

      T = (JD0 - 2451545.0)/36525.0;

/* .. GMST at UT=0h, in deg */
      GMST0 = 100.46061837+T*(36000.770053608+T*(3.87933E-4 - T/3.871E7));

/* .. Convert to days */
      GMST0 /= 360.0;

      GMST = GMST0+1.00273790935*(JD-JD0);

      GMST -= (int) (GMST);

      return(GMST);

}
/**********************************************************************/
/*  Separate non-integer day into integer day, hour, minute, second.  */

void DAY2HMS(double *DAY, double *HOUR, double *MINUTE, double *SECOND,
             double DTSIM)
{
      double TEMP;

      TEMP=*DAY;
      *DAY=floor(TEMP);
      TEMP=(TEMP-*DAY)*24.0;
      *HOUR=floor(TEMP);
      TEMP=(TEMP-*HOUR)*60.0;
      *MINUTE=floor(TEMP);
      *SECOND=(TEMP-*MINUTE)*60.0;
      *SECOND=floor(*SECOND/DTSIM+0.5)*DTSIM;
      if (*SECOND == 60.0) {
         *SECOND = 0.0;
         *MINUTE += 1.0;
         if (*MINUTE == 60.0) {
            *MINUTE = 0.0;
            *HOUR += 1.0;
            if (*HOUR == 24.0) {
               *HOUR = 0.0;
               *DAY += 1.0;
            }
         }
      }
}
#if defined _USE_SYSTEM_TIME_
/**********************************************************************/
/* This function returns the number of microseconds since the Unix    */
/* epoch, 00:00:00.0 Jan 1 1970.  Typically used as a tick/tock       */
/* duration measurement.                                              */
double usec(void)
{
      struct timeval now;

      gettimeofday(&now,NULL);
      return(1.0E6*now.tv_sec+now.tv_usec);
}
/**********************************************************************/
/* Get time from operating system, and convert to compatible format.  */
void RealSystemTime(long *Year, long *DOY, long *Month, long *Day,
                   long *Hour, long *Minute, double *Second)
{
      struct timeval now;
      double UnixTime,AbsTime,JD;

      /* Unix Time is since 00:00:00.0 Jan 1 1970 */
      gettimeofday(&now,NULL);
      UnixTime = now.tv_sec + 1.0E-6*now.tv_usec;

      /* AbsTime is since J2000 */
      AbsTime = UnixTime - 946728000.0;

      JD = AbsTimeToJD(AbsTime);
      JD2YMDHMS(JD,Year,Month,Day,Hour,Minute,Second);
      *DOY = MD2DOY(*Year,*Month,*Day);
}
/**********************************************************************/
double RealRunTime(double *RealTimeDT)
{
      static double RunTime = 0.0;
      static long First = 1;

#if defined(_WIN32)

      static LARGE_INTEGER SysFreq;
      static LARGE_INTEGER OldSysCtr;
      LARGE_INTEGER SysCtr;

      if (First) {
         First = 0;
         QueryPerformanceFrequency(&SysFreq);
         QueryPerformanceCounter(&OldSysCtr);
      }

      QueryPerformanceCounter(&SysCtr);
      *RealTimeDT = ((double) (SysCtr.QuadPart - OldSysCtr.QuadPart)) /
         ((double) SysFreq.QuadPart);
      OldSysCtr = SysCtr;

#elif defined(__APPLE__)

      static double OldSysTime;
      double SysTime;
      struct timeval now;

      if (First) {
         First = 0;
         gettimeofday(&now,NULL);
         OldSysTime = now.tv_sec + 1.0E-6*now.tv_usec;
      }

      gettimeofday(&now,NULL);
      SysTime = now.tv_sec + 1.0E-6*now.tv_usec;
      *RealTimeDT = SysTime-OldSysTime;
      OldSysTime = SysTime;

#elif defined(__linux__)

      static double OldSysTime;
      double SysTime;
      long Year,DOY,Month,Day,Hour,Minute;
      double Second;

      if (First) {
         First = 0;
         RealSystemTime(&Year,&DOY,&Month,&Day,&Hour,&Minute,&Second);
         OldSysTime = DateToAbsTime(Year,Month,Day,Hour,Minute,Second);
      }

      RealSystemTime(&Year,&DOY,&Month,&Day,&Hour,&Minute,&Second);
      SysTime = DateToAbsTime(Year,Month,Day,Hour,Minute,Second);
      *RealTimeDT = SysTime-OldSysTime;
      OldSysTime = SysTime;
#else
      #error "Unknown operating system in RealRunTime.  Fix that!"
      printf("Unknown operating system in RealRunTime.  Bailing out.\n");
      exit(1);
#endif

      if (*RealTimeDT < 0.0) *RealTimeDT = 0.001;
      //if (*RealTimeDT > 1.0) *RealTimeDT = 1.0;
      RunTime += *RealTimeDT;

      return(RunTime);
}
#endif /* USE_SYSTEM_TIME */

//#ifdef __cplusplus
//}
//#endif


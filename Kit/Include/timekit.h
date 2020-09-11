/*    This file is distributed with 42,                               */
/*    the (mostly harmless) spacecraft dynamics simulation            */
/*    created by Eric Stoneking of NASA Goddard Space Flight Center   */

/*    Copyright 2010 United States Government                         */
/*    as represented by the Administrator                             */
/*    of the National Aeronautics and Space Administration.           */

/*    No copyright is claimed in the United States                    */
/*    under Title 17, U.S. Code.                                      */

/*    All Other Rights Reserved.                                      */


#ifndef __TIMEKIT_H__
#define __TIMEKIT_H__

#include <math.h>
#if defined(_WIN32)
   #include <Windows.h>
#elif defined(__APPLE__)
/*      #include <CoreServices/Timer.h> */ /* For Microseconds high-precision timer */
#endif
#include <time.h>
#ifndef _WIN32
   #include <sys/time.h>
#endif

/* #ifdef __cplusplus
** namespace Kit {
** #endif
*/

struct DateType {
   double JulDay;
   long Year;
   long Month;
   long Day;
   long doy;
   long Hour;
   long Minute;
   double Second;
};

double TimeToJD(double Time);
double JDToTime(double JD);
double DateToTime(long Year, long Month, long Day, 
   long Hour, long Minute, double Second);
double DateToJD(long Year, long Month, long Day,
   long Hour, long Minute, double Second);
void JDToDate(double JD,long *Year, long *Month, long *Day,
   long *Hour, long *Minute, double *Second);
void TimeToDate(double Time, long *Year, long *Month, long *Day,
   long *Hour, long *Minute, double *Second, double LSB);
long MD2DOY(long Year, long Month, long Day);
void DOY2MD(long Year, long DayOfYear, long *Month, long *Day);
double JD2GMST(double JD);
double GpsTimeToJD(long GpsRollover, long GpsWeek, double GpsSecond);
void JDToGpsTime(double JD, long *GpsRollover, long *GpsWeek, double *GpsSecond);
double usec(void);
void RealSystemTime(long *Year, long *DOY, long *Month, long *Day,
                   long *Hour, long *Minute, double *Second, double LSB);
double RealRunTime(double *RealTimeDT, double LSB);

/*
** #ifdef __cplusplus
** }
** #endif
*/

#endif /* __TIMEKIT_H__ */

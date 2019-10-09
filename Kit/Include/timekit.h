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
#if defined _USE_SYSTEM_TIME_
   #if defined(_WIN32)
      #include <Windows.h>
   #elif defined(__APPLE__)
/*      #include <CoreServices/Timer.h> */ /* For Microseconds high-precision timer */
   #endif
   #include <time.h>
   #ifndef _WIN32
      #include <sys/time.h>
   #endif
#endif

/* #ifdef __cplusplus
** namespace Kit {
** #endif
*/

double AbsTimeToJD(double AbsTime);
double JDToAbsTime(double JD);
double DateToAbsTime(long Year, long Month, long Day, long Hour,
   long Minute, double Second);
double YMDHMS2JD(long Year, long Month, long Day,
               long Hour, long Minute, double Second);
void JD2YMDHMS(double JD,long *Year, long *Month, long *Day,
                         long *Hour, long *Minute, double *Second);
void AbsTimeToDate(double AbsTime, long *Year, long *Month, long *Day,
                  long *Hour, long *Minute, double *Second, double LSB);
long MD2DOY(long Year, long Month, long Day);
void DOY2MD(long Year, long DayOfYear, long *Month, long *Day);
double JD2GMST(double JD);
void DAY2HMS(double *DAY, double *HOUR, double *MINUTE, double *SECOND,
             double DTSIM);
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

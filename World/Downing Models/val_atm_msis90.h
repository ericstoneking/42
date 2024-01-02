#ifndef val_atm_msis90_h
#define val_atm_msis90_h
//====================================================================
#include "val_always.h"
#include "val_f2c.h"
//====================================================================
int gtd6_(integer *iyd, real *sec, real *alt, real *glat,
	real *glong, real *stl, real *f107a, real *f107, real *ap, integer *
	mass, real *d__, real *t);
int tselec_90(real *);
int tretrv_90(real *svv);
//====================================================================
//        Neutral Atmosphere Empirical Model from the surface to lower 
//          exosphere  MSISE90 (JGR, 96, 1159-1172, 1991) 
//         A.E.Hedin 4/24/90;6/3/91(add SAVE) 
//         2/11/93 correct switch initialization and mks calculation 
//       2/11/97 [AEH] CORRECT ERROR IN GHP6 WHEN USING METER6(.TRUE.) 
//           See subroutine GHP6 to specify a pressure rather than 
//           altitude. 
//     INPUT: 
//        IYD - YEAR AND DAY AS YYDDD or DDD (day of year from 1 to 365) 
//        SEC - UT(SEC) 
//        ALT - ALTITUDE(KM) 
//        GLAT - GEODETIC LATITUDE(DEG) 
//        GLONG - GEODETIC LONGITUDE(DEG) 
//        STL - LOCAL APPARENT SOLAR TIME(HRS) 
//        F107A - 3 MONTH AVERAGE OF F10.7 FLUX 
//        F107 - DAILY F10.7 FLUX FOR PREVIOUS DAY 
//        AP - MAGNETIC INDEX(DAILY) OR WHEN SW(9)=-1. : 
//           - ARRAY CONTAINING: 
//             (1) DAILY AP 
//             (2) 3 HR AP INDEX FOR CURRENT TIME 
//             (3) 3 HR AP INDEX FOR 3 HRS BEFORE CURRENT TIME 
//             (4) 3 HR AP INDEX FOR 6 HRS BEFORE CURRENT TIME 
//             (5) 3 HR AP INDEX FOR 9 HRS BEFORE CURRENT TIME 
//             (6) AVERAGE OF EIGHT 3 HR AP INDICIES FROM 12 TO 33 HRS PRIOR 
//                    TO CURRENT TIME 
//             (7) AVERAGE OF EIGHT 3 HR AP INDICIES FROM 36 TO 59 HRS PRIOR 
//                    TO CURRENT TIME 
//        MASS - MASS NUMBER (ONLY DENSITY FOR SELECTED GAS IS 
//                 CALCULATED.  MASS 0 IS TEMPERATURE.  MASS 48 FOR ALL. 
//     Note:  Ut, Local Time, and Longitude are used independently in the 
//            model and are not of equal importance for every situation. 
//            For the most physically realistic calculation these three 
//            variables should be consistent (STL=SEC/3600+GLONG/15). 
//            F107, F107A, and AP effects are not large below 80 km 
//            and these can be set to 150., 150., and 4. respectively. 
//     OUTPUT: 
//        D(1) - HE NUMBER DENSITY(CM-3) 
//        D(2) - O NUMBER DENSITY(CM-3) 
//        D(3) - N2 NUMBER DENSITY(CM-3) 
//        D(4) - O2 NUMBER DENSITY(CM-3) 
//        D(5) - AR NUMBER DENSITY(CM-3) 
//        D(6) - TOTAL MASS DENSITY(GM/CM3) 
//        D(7) - H NUMBER DENSITY(CM-3) 
//        D(8) - N NUMBER DENSITY(CM-3) 
//        T(1) - EXOSPHERIC TEMPERATURE 
//        T(2) - TEMPERATURE AT ALT 

//      TO GET OUTPUT IN M-3 and KG/M3:   CALL METER6(.TRUE.) 

//      O, H, and N set to zero below 72.5 km 
//      Exospheric temperature set to average for altitudes below 120 km. 

//           The following is for test and special purposes: 
//            TO TURN ON AND OFF PARTICULAR VARIATIONS CALL TSELEC(SW) 
//               WHERE SW IS A 25 ELEMENT ARRAY CONTAINING 0. FOR OFF, 1. 
//               FOR ON, OR 2. FOR MAIN EFFECTS OFF BUT CROSS TERMS ON 
//               FOR THE FOLLOWING VARIATIONS 
//               1 - F10.7 EFFECT ON MEAN  2 - TIME INDEPENDENT 
//               3 - SYMMETRICAL ANNUAL    4 - SYMMETRICAL SEMIANNUAL 
//               5 - ASYMMETRICAL ANNUAL   6 - ASYMMETRICAL SEMIANNUAL 
//               7 - DIURNAL               8 - SEMIDIURNAL 
//               9 - DAILY AP             10 - ALL UT/LONG EFFECTS 
//              11 - LONGITUDINAL         12 - UT AND MIXED UT/LONG 
//              13 - MIXED AP/UT/LONG     14 - TERDIURNAL 
//              15 - DEPARTURES FROM DIFFUSIVE EQUILIBRIUM 
//              16 - ALL TINF VAR         17 - ALL TLB VAR 
//              18 - ALL TN1 VAR           19 - ALL S VAR 
//              20 - ALL TN2 VAR           21 - ALL NLB VAR 
//              22 - ALL TN3 VAR           23 - TURBO SCALE HEIGHT VAR 

//              To get current values of SW: CALL TRETRV(SW)
//====================================================================
#endif

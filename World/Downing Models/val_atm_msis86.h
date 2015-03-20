#ifndef val_atm_msis86_h
#define val_atm_msis86_h
//====================================================================
#include "val_always.h"
#include "val_f2c.h"
//====================================================================
int gts5_(integer *iyd, real *sec, real *alt, real *glat,
	real *glong, real *stl, real *f107a, real *f107, real *ap, integer *
	mass, real *d__, real *t);
int tselec_86(real *);
int tretrv_86(real *svv);
//====================================================================
//        MSIS-86/CIRA 1986 Neutral Thermosphere Model 
//         A.E.Hedin 3/15/85;2/26/87 (Variable Names Shortened) 
//         10/14/87 Increase altitude limit of O mixing calculation 
//             ALTL(2) FROM 300. TO 400. 
//         8/10/88 CHANGE HANDLING OF SW(9) IN GLOBE 
//     INPUT: 
//        IYD - YEAR AND DAY AS YYDDD 
//        SEC - UT(SEC) 
//        ALT - ALTITUDE(KM) (GREATER THAN 85 KM) 
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

//      TO GET OUTPUT IN M-3 and KG/M3:   CALL METERS(.TRUE.) 

//          ADDITIONAL COMMENTS 
//           (1) LOWER BOUND QUANTITIES IN COMMON/GTS3C/ 
//           (2) TO TURN ON AND OFF PARTICULAR VARIATIONS CALL TSELEC(SW) 
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
//              18 - ALL T0 VAR           19 - ALL S VAR 
//              20 - ALL Z0 VAR           21 - ALL NLB VAR 
//              22 - ALL TR12 VAR         23 - TURBO SCALE HEIGHT VAR 

//              To get current values of SW: CALL TRETRV(SW)
//====================================================================
#endif

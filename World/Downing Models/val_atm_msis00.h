#ifndef val_atm_msis00_h
#define val_atm_msis00_h
//====================================================================
#include "val_always.h"
#include "val_f2c.h"
//====================================================================
int gtd7_(integer *iyd, real *sec, real *alt, real *glat,
	real *glong, real *stl, real *f107a, real *f107, real *ap, integer *
	mass, real *d__, real *t);
int tselec_00(real *);
int tretrv_00(real *svv);
//====================================================================
//     NRLMSISE-00 
//     ----------- 
//        Neutral Atmosphere Empirical Model from the surface to lower 
//        exosphere 

//        NEW FEATURES: 
//          *Extensive satellite drag database used in model generation 
//          *Revised O2 (and O) in lower thermosphere 
//          *Additional nonlinear solar activity term 
//          *"ANOMALOUS OXYGEN" NUMBER DENSITY, OUTPUT D(9) 
//           At high altitudes (> 500 km), hot atomic oxygen or ionized 
//           oxygen can become appreciable for some ranges of subroutine 
//           inputs, thereby affecting drag on satellites and debris. We 
//           group these species under the term "anomalous oxygen," since 
//           their individual variations are not presently separable with 
//           the drag data used to define this model component. 

//        SUBROUTINES FOR SPECIAL OUTPUTS: 

//        HIGH ALTITUDE DRAG: EFFECTIVE TOTAL MASS DENSITY 
//        (SUBROUTINE GTD7D, OUTPUT D(6)) 
//           For atmospheric drag calculations at altitudes above 500 km, 
//           call SUBROUTINE GTD7D to compute the "effective total mass 
//           density" by including contributions from "anomalous oxygen." 
//           See "NOTES ON OUTPUT VARIABLES" below on D(6). 

//        PRESSURE GRID (SUBROUTINE GHP7) 
//          See subroutine GHP7 to specify outputs at a pressure level 
//          rather than at an altitude. 

//        OUTPUT IN M-3 and KG/M3:   CALL METERS(.TRUE.) 

//     INPUT VARIABLES: 
//        IYD - YEAR AND DAY AS YYDDD (day of year from 1 to 365 (or 366)) 
//              (Year ignored in current model) 
//        SEC - UT(SEC) 
//        ALT - ALTITUDE(KM) 
//        GLAT - GEODETIC LATITUDE(DEG) 
//        GLONG - GEODETIC LONGITUDE(DEG) 
//        STL - LOCAL APPARENT SOLAR TIME(HRS; see Note below) 
//        F107A - 81 day AVERAGE OF F10.7 FLUX (centered on day DDD) 
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
//             (7) AVERAGE OF EIGHT 3 HR AP INDICIES FROM 36 TO 57 HRS PRIOR 
//                    TO CURRENT TIME 
//        MASS - MASS NUMBER (ONLY DENSITY FOR SELECTED GAS IS 
//                 CALCULATED.  MASS 0 IS TEMPERATURE.  MASS 48 FOR ALL. 
//                 MASS 17 IS Anomalous O ONLY.) 

//     NOTES ON INPUT VARIABLES: 
//        UT, Local Time, and Longitude are used independently in the 
//        model and are not of equal importance for every situation. 
//        For the most physically realistic calculation these three 
//        variables should be consistent (STL=SEC/3600+GLONG/15). 
//        The Equation of Time departures from the above formula 
//        for apparent local time can be included if available but 
//        are of minor importance. 

//        F107 and F107A values used to generate the model correspond 
//        to the 10.7 cm radio flux at the actual distance of the Earth 
//        from the Sun rather than the radio flux at 1 AU. The following 
//        site provides both classes of values: 
//        ftp://ftp.ngdc.noaa.gov/STP/SOLAR_DATA/SOLAR_RADIO/FLUX/ 

//        F107, F107A, and AP effects are neither large nor well 
//        established below 80 km and these parameters should be set to 
//        150., 150., and 4. respectively. 

//     OUTPUT VARIABLES: 
//        D(1) - HE NUMBER DENSITY(CM-3) 
//        D(2) - O NUMBER DENSITY(CM-3) 
//        D(3) - N2 NUMBER DENSITY(CM-3) 
//        D(4) - O2 NUMBER DENSITY(CM-3) 
//        D(5) - AR NUMBER DENSITY(CM-3) 
//        D(6) - TOTAL MASS DENSITY(GM/CM3) 
//        D(7) - H NUMBER DENSITY(CM-3) 
//        D(8) - N NUMBER DENSITY(CM-3) 
//        D(9) - Anomalous oxygen NUMBER DENSITY(CM-3) 
//        T(1) - EXOSPHERIC TEMPERATURE 
//        T(2) - TEMPERATURE AT ALT 

//     NOTES ON OUTPUT VARIABLES: 
//        TO GET OUTPUT IN M-3 and KG/M3:   CALL METERS(.TRUE.) 

//        O, H, and N are set to zero below 72.5 km 

//        T(1), Exospheric temperature, is set to global average for 
//        altitudes below 120 km. The 120 km gradient is left at global 
//        average value for altitudes below 72 km. 

//        D(6), TOTAL MASS DENSITY, is NOT the same for subroutines GTD7 
//        and GTD7D 

//          SUBROUTINE GTD7 -- D(6) is the sum of the mass densities of the 
//          species labeled by indices 1-5 and 7-8 in output variable D. 
//          This includes He, O, N2, O2, Ar, H, and N but does NOT include 
//          anomalous oxygen (species index 9). 

//          SUBROUTINE GTD7D -- D(6) is the "effective total mass density 
//          for drag" and is the sum of the mass densities of all species 
//          in this model, INCLUDING anomalous oxygen. 

//     SWITCHES: The following is for test and special purposes: 

//        TO TURN ON AND OFF PARTICULAR VARIATIONS CALL TSELEC(SW), 
//        WHERE SW IS A 25 ELEMENT ARRAY CONTAINING 0. FOR OFF, 1. 
//        FOR ON, OR 2. FOR MAIN EFFECTS OFF BUT CROSS TERMS ON 
//        FOR THE FOLLOWING VARIATIONS 
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

//        To get current values of SW: CALL TRETRV(SW)
//====================================================================
#endif

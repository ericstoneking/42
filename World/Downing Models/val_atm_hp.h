#ifndef val_atm_hp_h
#define val_atm_hp_h
//====================================================================
#include "val_always.h"
#include "val_rotation.h"
//====================================================================
bool Harris (const ZVector& pos, const ZVector& sunpos,
   const real& altitude, const real& f107, real& density);
// PURPOSE TO COMPUTE THE DENSITY USING HARRIS-PRIESTER DENSITY MODEL
// METHOD
//     1ND--FIND THE HEIGHT ON THE ATMOSPHERIC DENSITY TABLE
//          BETWEEN IT'S TWO BRACKETING HEIGHTS. (EACH HEIGHT ON THE
//          TABLE HAS A MAXIMUM AND A MINIMUM DENSITY ASSOCIATED
//          WITH IT.  THE MAXIMUM DENSITY IS THE DENSITY AT THAT
//          HEIGHT AT THE APEX OF THE DIURNAL BULGE.  THE MINIMUM
//          DENSITY IS THE DENSITY AT THAT HEIGHT AT THE ANTAPEX.
//     2ND--USE EXPONENTIAL INTERPOLATION TO CALCULATE THE MAXIMUM AND
//          MINIMUM DENSITYS (RHOMAX AND RHOMIN) FOR THE SPACECRAFT
//          HEIGHT.
//     3RD--USE PSI (THE ANGULAR DIFFERENCE BETWEEN THE SPACECRAFT
//          AND THE DIURNAL BLUGE) AS A COSINE VARIATION FUNCTION
//          TO DETERMIN RHO (THE ACTUAL DENSITY).
// ARGUMENT LIST
//     pos      SPACECRAFT POSITION VECTOR (KM) in MJ2000eq
//     sunpos   SUN POSITION VECTOR (KM) in MJ2000eq
//     altitude SPACECRAFT HEIGHT (km)
//     f107     F10.7 solar flux
//     density  ATMOSPHERIC DENSITY (kg/km^3
//
//     returns   true if successful
// ORIGINATOR
//     J.ERICKSON - MODIFIED FROM EPHGEN PROGRAM SUBROUTINE ATMOS
//     J.DOWNING  = CONVERTED FROM GMAN CODE 6-9-04
//====================================================================
#endif

#ifndef val_atm_jr_h
#define val_atm_jr_h
//====================================================================
#include "val_always.h"
#include <stdio.h>
//====================================================================
double jac_rob (const double& height,
   const double space_craft[3], const double sun[3],
   const double& a1_time, const double& tkp, const double& xtemp);
// Return Value Type - double
//
//Calling Sequence -
//Argument     Type     Description                            Use
// --------     ------   -----------------------------------    ----
// height       double   Spacecraft height (km)                  I
// space_craft  double   Spacecraft position (km, TOD GCI)       I
// sun          double   Sun unit vector (TOD GCI)               I
// a1_time      double   Reduced julian date (days)              I
// tkp          double   Kp value (geomagnetic index             I
// xtemp        double   Exospehric temperature                  I
//====================================================================
#endif


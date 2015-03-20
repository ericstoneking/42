#ifndef val_atm_vts3_h
#define val_atm_vts3_h
//====================================================================
//  VAL -- Virtual Almanac Library
//  ALL ELEMENTS DEFINED IN ASSOCIATED *.c file
//====================================================================
#include "val_always.h"
#include "val_rotation.h"
#include "val_smjd.h"
//====================================================================
bool GetVenusDensity (const ZSmjd& smjd,
   const ZVector& sc, const ZVector& unitsun, real& rho);
//====================================================================
#endif


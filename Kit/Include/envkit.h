/*    This file is distributed with 42,                               */
/*    the (mostly harmless) spacecraft dynamics simulation            */
/*    created by Eric Stoneking of NASA Goddard Space Flight Center   */

/*    Copyright 2010 United States Government                         */
/*    as represented by the Administrator                             */
/*    of the National Aeronautics and Space Administration.           */

/*    No copyright is claimed in the United States                    */
/*    under Title 17, U.S. Code.                                      */

/*    All Other Rights Reserved.                                      */


#ifndef __ENVKIT_H__
#define __ENVKIT_H__

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "mathkit.h"
#include "dcmkit.h"
#include "iokit.h"
#include "geomkit.h"

/*
** #ifdef __cplusplus
** namespace Kit {
** #endif
*/

void EGM96(const char *ModelPath, long N, long M, double mass, double pbn[3],
           double PriMerAng, double FgeoN[3]);
void GMM2B(const char *ModelPath, long N, long M, double mass, double pbn[3],
           double PriMerAng, double FgeoN[3]);
void GLGM2(const char *ModelPath, long N, long M, double mass, double pbn[3],
           double PriMerAng, double FgeoN[3]);
void IGRFMagField(const char *ModelPath, long N, long M, double pbn[3],double PriMerAng,
             double MagVecN[3]);
void DipoleMagField(double DipoleMoment, double DipoleAxis[3],
                    double DipoleOffset[3],double p[3],
                    double PriMerAng,double MagVecN[3]);
double KpToAp(double Kp);
double JacchiaRoberts(double pbn[3],double svn[3],double F10p7,double Ap);
double SimpleMSIS(double pbn[3], long Col);
double NRLMSISE00(long Year, long DOY, long Hour, long Minute,
   double Second, double PosW[3], double F10p7, double AP);
double MarsAtmosphereModel(double r[3]);
void SimpleEarthPrecNute(double JD,double C_TEME_TETE[3][3],
                                   double C_TETE_J2000[3][3]);
void HiFiEarthPrecNute(double JD,double C_TEME_TETE[3][3],
                                 double C_TETE_J2000[3][3]);
void WGS84ToECEF(double glat, double glong, double alt, double p[3]);
void ECEFToWGS84(double p[3], double *glat, double *glong, double *alt);
long PolyhedronGravAcc(struct GeomType *G, double Density, double PosN[3],
   double CWN[3][3], double GravAccN[3]);
long PolyhedronGravGrad(struct GeomType *G, double Density, double PosN[3],
   double CWN[3][3], double GravGradN[3][3]);
void GravGradTimesInertia(double g[3][3], double I[3][3], double GGxI[3]);

/*
** #ifdef __cplusplus
** }
** #endif
*/

#endif /* __ENVKIT_H__ */

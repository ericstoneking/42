/*    This file is distributed with 42,                               */
/*    the (mostly harmless) spacecraft dynamics simulation            */
/*    created by Eric Stoneking of NASA Goddard Space Flight Center   */

/*    Copyright 2010 United States Government                         */
/*    as represented by the Administrator                             */
/*    of the National Aeronautics and Space Administration.           */

/*    No copyright is claimed in the United States                    */
/*    under Title 17, U.S. Code.                                      */

/*    All Other Rights Reserved.                                      */


#ifndef __DCMKIT_H__
#define __DCMKIT_H__

/*
** #ifdef __cplusplus
** namespace Kit {
** #endif
*/

void C2Q(double C[3][3], double Q[4]);
void Q2C(double Q[4], double C[3][3]);
void A2C(long SEQ, double TH1, double TH2, double TH3, double C[3][3]);
void C2A(long SEQ, double C[3][3],double *TH1, double *TH2, double *TH3);
void SimpRot(double AXIS[3], double THETA, double C[3][3]);
void Q2AngleVec(double Q[4], double AngleVec[3]);
void QW2QDOT(double Q[4],double W[3],double QDOT[4]);
void PARAXIS(double IB[3][3], double CBA[3][3], double m, double pba[3],
             double IBA[3][3]);
void PrincipalMOI(double Ib[3][3], double Ip[3], double CPB[3][3]);
void Q2W(double q[4], double qdot[4], double w[3]);
void JointPartials(long Init, long IsSpherical,
   long RotSeq, long TrnSeq, double ang[3], double sig[3],
   double Gamma[3][3], double Gs[3], double Gds[3],
   double s[3], double Delta[3][3], double Ds[3], double Dds[3]);
void ADOT2W(long IsSpherical, long Seq, double ang[3], double u[3], double w[3]);
void W2ADOT(long Seq, double ang[3], double w[3], double adot[3]);
void W2CDOT(double w[3], double C[3][3], double Cdot[3][3]);
void CDOT2W(double C[3][3], double Cdot[3][3], double w[3]);

/*
** #ifdef __cplusplus
** }
** #endif
*/

#endif /* __DCMKIT_H__ */

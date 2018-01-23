/*    This file is distributed with 42,                               */
/*    the (mostly harmless) spacecraft dynamics simulation            */
/*    created by Eric Stoneking of NASA Goddard Space Flight Center   */

/*    Copyright 2010 United States Government                         */
/*    as represented by the Administrator                             */
/*    of the National Aeronautics and Space Administration.           */

/*    No copyright is claimed in the United States                    */
/*    under Title 17, U.S. Code.                                      */

/*    All Other Rights Reserved.                                      */


#ifndef __FSWTYPES_H__
#define __FSWTYPES_H__

/*
** #ifdef __cplusplus
** namespace _42 {
** #endif
*/

struct CmdVecType {
   long Mode;
   long Frame;
   long TrgType;
   long TrgWorld;
   long TrgSC;
   long TrgBody;
   double N[3]; /* Components in N */
   double W[3]; /* Components in W */
   double L[3]; /* Components in L */
   double R[3]; /* Components in R */
   double T[3]; /* Components in T */
   double wn[3]; /* Angular velocity in N, expressed in N */
};

struct CmdType {
   long Parm;
   long Frame;
   double Rate[3];
   double Ang[3];
   long RotSeq;
   double qrl[4];
   double qrn[4];
   double wrn[3];
   double SpinRate;
   double Hvr[3];
   double Hvn[3];
   double OldCRN[3][3];
   struct CmdVecType PriVec;
   struct CmdVecType SecVec;
};

struct FswGimType {
   long IsUnderActiveControl;
   long IsSpherical;
   long RotDOF;
   long TrnDOF;
   double CGiBi[3][3];
   double CBoGo[3][3];
   double COI[3][3];
   long RotSeq;
   double Rate[3];
   double Ang[3];
   double RateGain[3];
   double AngGain[3];
   double MaxRate[3];
   double MaxTrq[3];
};

struct FSWType {
   long Init;
   long Tlm;

   long Mode;
   long ReqMode;
   double DT;

   /* Cmd Variables */
   struct CmdType Cmd;

   /* Sensor Inputs */
   long SunValid;
   double svb[3];
   long MagValid;
   double bvb[3];
   double qbn[4];
   double wbn[3];
   long EarthValid;
   double ESroll, ESpitch;
   long EphValid;
   double PosN[3],VelN[3];
   double svn[3],bvn[3];

   /* Gimbals */
   long Ngim;
   struct FswGimType *Gim;
   struct CmdType *GimCmd;

   /* Wheels */
   long Nwhl;
   double *Hw;
   double *Hwcmd;
   double **Awhl;  /* 3 x Nwhl */
   double **AwhlPlus; /* Nwhl x 3 */
   double *Twhlcmd;

   /* MTBs */
   long Nmtb;
   double **Amtb;  /* 3 x Nmtb */
   double **AmtbPlus;  /* Nmtb x 3 */
   double *Mmtbcmd;
   double *Mmtbmax;

   /* Thrusters */
   long Nthr;
   double **Athr; /* 3 x Nthr, or 6 x Nthr */
   double **AthrPlus; /* Nthr x 3  or Nthr x 6 */
   double *Thrcmd;

   /* Ideal Actuators */
   double IdealFrc[3];
   double IdealTrq[3];

   /* Formation Control */
   double CSF[3][3];
   double PosF[3];  /* Expressed in F */
   double VelF[3];  /* Expressed in F */
   double CSFcmd[3][3];
   double PosFcmd[3];

   /* Control Parameters */
   double MOI[3];
   double mass;
   double wc[3],zc[3];
   double Kr[3],Kp[3],Ki[3];
   double wct,zct;
   double Krt,Kpt,Kit;
   double thsat, psat; /* Error Saturation Limits */
   double Kunl;
   double Kprec,Knute;
   double MaxSlewRate;
   double wrn[3];
   double rvn[3];

   /* Limits */
   double Tmax;
   double Fmax;
   double wmax[3];

   /* For RampCoastGlide */
   double RcgWC;
   double RcgAmax;
   double RcgVmax;

   /* Intermediate Variables */
   double CRN[3][3],CBR[3][3],CBN[3][3];
   double qbr[4];
   double therr[3],werr[3],ierr[3];
   double pbr[3],vbr[3];
   double poserr[3],velerr[3];
   double Tcmd[3];
   double rvb[3];
   double Hvb[3];

   double Bold1,Bold2,xold,yold;
};

/*
** #ifdef __cplusplus
** }
** #endif
*/

#endif /* __FSWTYPES_H__ */

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
   double AngRate[3];
   double Ang[3];
   double PosRate[3];
   double Pos[3];
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

struct AcBodyType {
};

struct AcJointType {
   long IsUnderActiveControl;
   long IsSpherical;
   long RotDOF;
   long TrnDOF;
   double CGiBi[3][3];
   double CBoGo[3][3];
   double COI[3][3];
   long RotSeq;
   long TrnSeq;
   double Ang[3];
   double AngRate[3];
   double Pos[3];
   double PosRate[3];
   double AngGain[3];
   double AngRateGain[3];
   double PosGain[3];
   double PosRateGain[3];
   double MaxAngRate[3];
   double MaxPosRate[3];
   double MaxTrq[3];
   double MaxFrc[3];
   struct CmdType Cmd;
};

struct AcAccelType {
   double PosB[3];
   double Axis[3];
   double Acc;
};

struct AcGyroType {
   double Axis[3];
   double Rate;
};

struct AcMagnetometerType {
   double Axis[3];
   double Field;
};

struct AcCssType {
   double Axis[3];
   double Scale;
   long Valid;
   double Illum;
};

struct AcStarTrackerType {
   double qb[4];
   double CB[3][3];
   long Valid;
   double qn[4];
   double qbn[4];
};

struct AcFssType {
   double qb[4];
   double CB[3][3];
   long Valid;
   double SunAng[2];
   double SunVecS[3];
   double SunVecB[3];
};

struct AcEarthSensorType {
   long Valid;
   double Roll;
   double Pitch;
};

struct AcGpsType {
   long Valid;
   long Rollover;
   long Week;
   double Sec;
   double PosN[3];
   double VelN[3];
   double PosW[3];
   double VelW[3];
   double Lng,Lat,Alt;
};

struct AcWhlType {
   double Axis[3];
   double DistVec[3];
   double J;
   double Tmax;
   double Hmax;
   double w;
   double H;
   double Tcmd;
};

struct AcMtbType {
   double Axis[3];
   double DistVec[3];
   double Mmax;
   double Mcmd;
};

struct AcThrType {
   double PosB[3];
   double Axis[3];
   double Fmax;
   double Fcmd;
   double PulseWidthCmd;
};

struct AcCmgType {
};

struct AcPrototypeCtrlType {
   long Init;
   double wc;
   double amax;
   double vmax;
   double Kprec;
   double Knute;
   double Tcmd[3];
   double qbr[4];
   double therr[3];
   double werr[3];
};

struct AcAdHocCtrlType {
   long Init;
   double Kr[3],Kp[3];
   double therr[3],werr[3];
   double Tcmd[3];
};

struct AcSpinnerCtrlType {
   long Init;
   double Ispin,Itrans; 
   double SpinRate;
   double Knute;
   double Kprec;

   double Bold1,Bold2;
   double xold,yold;
   double rvn[3],rvb[3];
   double Tcmd[3];
   double Mcmd[3];
};

struct AcMomBiasCtrlType {
   long Init;
};

struct AcThreeAxisCtrlType {
   long Init;
   double Kr[3];
   double Kp[3];
   double Kunl;
   
   double Tcmd[3];
   double Hwcmd[3];
};

struct AcIssCtrlType {
   long Init;
   double Kr[3];
   double Kp[3];
   double Tmax;
   double therr[3];
   double werr[3];
};

struct AcCmgCtrlType {
   long Init;
};


struct AcsType {
   long Nb;
   long Ng;
   long Nwhl;
   long Nmtb;
   long Nthr;
   long Ncmg;
   long Nacc;
   long Ngyro;
   long Nmag;
   long Ncss;
   long Nst;
   long Nfss;
   
   /* Dynamics */
   struct AcBodyType *B;
   struct AcJointType *G;
   
   /* Sensors */
   struct AcAccelType *Accel;
   struct AcGyroType *Gyro;
   struct AcMagnetometerType *MAG;
   struct AcCssType *CSS;
   struct AcStarTrackerType *ST;
   struct AcFssType *FSS;
   struct AcGpsType *GPS;
   struct AcEarthSensorType ES;
   
   /* Actuators */
   struct AcWhlType *Whl;
   struct AcMtbType *MTB;
   struct AcThrType *Thr;
   struct AcCmgType *CMG;
   
   /* Control Modes */
   struct AcPrototypeCtrlType PrototypeCtrl;
   struct AcAdHocCtrlType AdHocCtrl;
   struct AcSpinnerCtrlType SpinnerCtrl;
   struct AcMomBiasCtrlType MomBiasCtrl;
   struct AcThreeAxisCtrlType ThreeAxisCtrl;
   struct AcIssCtrlType IssCtrl;
   struct AcCmgCtrlType CmgCtrl;
   
   /* Common Parameters */
   double DT;
   double mass;
   double MOI[3][3];
   
   /* Common Variables */
   long Init;
   long Mode;
   long ReqMode;
   struct CmdType Cmd;
   
   double wbn[3];
   double qbn[4];
   double CBN[3][3];
   double Hvb[3];
   double svn[3],svb[3];
   double bvn[3],bvb[3];
   double PosN[3];
   double VelN[3];
   long SunValid;
   long MagValid;
   long EphValid;
   
   double IdealTrq[3];
   double IdealFrc[3];
};

/*
** #ifdef __cplusplus
** }
** #endif
*/

#endif /* __FSWTYPES_H__ */

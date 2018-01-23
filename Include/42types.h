/*    This file is distributed with 42,                               */
/*    the (mostly harmless) spacecraft dynamics simulation            */
/*    created by Eric Stoneking of NASA Goddard Space Flight Center   */

/*    Copyright 2010 United States Government                         */
/*    as represented by the Administrator                             */
/*    of the National Aeronautics and Space Administration.           */

/*    No copyright is claimed in the United States                    */
/*    under Title 17, U.S. Code.                                      */

/*    All Other Rights Reserved.                                      */


#include "geomkit.h"
#include "orbkit.h"
#include "fswtypes.h"

#ifndef __42TYPES_H__
#define __42TYPES_H__

/*
** #ifdef __cplusplus
** namespace _42 {
** #endif
*/

struct SphereHarmType {
   long Type;
   long N;
   long M;
};

struct FormationType {
   char FixedInFrame;
   double CN[3][3];
   double CL[3][3];
   double PosR[3]; /* Position of F wrt R, expressed in N */
};

/* "Analysis" flex nodes, ("Force" nodes and "Measurement" nodes) */
struct FlexNodeType {
   long ID; /* For referring back to Nastran */
   double PosB[3];
   double **PSI, **THETA; /* Mode shapes, 3 x B.Nf */
   double Frc[3],Trq[3]; /* Both expressed in B */
   double *FlexFrc;  /* "Fbendy + Tbendy", B.Nf x 1 */
   double pos[3],vel[3],ang[3],angrate[3]; /* Deflection variables */
   double TotAngVel[3];
};

struct BodyType {
   double mass;
   double cm[3]; /* wrt origin of convenience, expressed in B frame */
   double c[3]; /* First mass moment about ref pt, expressed in B */
   double I[3][3]; /* Moment of Inertia, about ref pt, expressed in B frame */
   double Hgyro[3];  /* Constant embedded momentum, for CMGs and rotating instruments */
   double wn[3]; /* Angular Velocity of B expressed in B frame */
   double qn[4];
   double vn[3]; /* velocity of B ref pt expressed in N frame */
   double pn[3]; /* position of B ref pt in N frame expressed in N frame */
   double CN[3][3]; /* Direction Cosine of B frame in N frame */
   double Trq[3]; /* expressed in B */
   double Frc[3]; /* expressed in B */
   double alpha[3]; /* Angular acceleration of B wrt N, expressed in B */
   double accel[3]; /* Linear acceleration of B wrt N, expressed in N */
   char GeomFileName[40];
   char FlexFileName[40];
   float ModelMatrix[16]; /* For OpenGL */
   long GeomTag;
   /* For KaneNBody Dynamics */
   long Gin;  /* Joint that B is Bout of */
   double beta[3];  /* Vector from B ref pt to B[0] ref pt, expressed in N */
   double AlphaR[3];  /* Remainder alpha, expressed in B */
   double AccR[3];    /* Remainder acc, expressed in N */
   double InertiaTrq[3];
   double InertiaFrc[3];
   double JointTrq[3]; /* From all joints exerting trq on this body */
   /* For Flex Formulation */
   long Nf;  /* Number of flex modes superimposed on this body */
   double *xi;    /* Flex speed coordinate, Nf x 1 */
   double *eta;   /* Flex position coordinate, Nf x 1 */
   double **Mf;   /* Flex Mass Matrix, Nf x Nf */
   double **Kf;   /* Flex Stiffness Matrix, Nf x Nf */
   double **Cf;   /* Flex Damping Matrix, Nf x Nf */
   double **Pf;   /* Flex tensor, 3 x Nf */
   double **Hf;   /* Flex tensor, 3 x Nf */
   double ***Qf;  /* Flex tensor, 3 x Nf x Nf */
   double ***Rf;  /* Flex tensor, 3 x Nf x 3 */
   double ****Sf; /* flex tensor, 3 x Nf x Nf x 3 */
   long f0;      /* Index of first element in uf */
   double Peta[3]; /* Pf*eta */
   double cplusPeta[3][3]; /* SkewMatrix of (c + Pf*eta) */
   double **CnbP; /* CNB*Pf, 3 x Nf */
   double **HplusQeta; /* Hf + Qf*eta, 3 x Nf */
   double **Qxi;  /* Qf*xi, 3 x Nf */
   double **Rw;   /* Rf*w, 3 x Nf */
   double ***Sw;  /* Sf*w, 3 x Nf * Nf */
   double **Swe;  /* Sf*w*eta, 3 x Nf */
   long NumFlexNodes;  /* Number of flex "analysis" nodes on Body */
   struct FlexNodeType *FlexNode;
   long MfIsDiagonal;  /* Simpler EOM for One-body case if Mf is diagonal */
};

struct JointType {
   long IsSpherical;      /* TRUE or FALSE */
   long RotDOF;           /* 0,1,2,3 */
   long TrnDOF;           /* 0,1,2,3 */
   long Bin;              /* Index of inner body */
   long Bout;             /* Index of outer body */
   double RigidRin[3];    /* Position wrt inner body ref pt (rigid) */
   double RigidRout[3];   /* Position wrt outer body ref pt (rigid) */
   double rin[3];         /* Position wrt inner body ref pt (incl flex & TrnDOF) */
   double rout[3];        /* Position wrt outer body ref pt (incl flex) */
   long RotSeq;           /* Joint Euler sequence */
   long RotLocked[3];     /* Set TRUE if individual DOF is to be locked in place */
   long TrnSeq;           /* Translational joint sequence */
   long TrnLocked[3];
   double d[3];           /* translational kinematic state variables */
   double s[3];           /* translational dynamic state variables */
   double xb[3];          /* translational displacement in the Bi frame */
   double xn[3];          /* translational displacement in the N frame */
   double ang[3];         /* Joint Euler angles */
   double rate[3];        /* Euler angle rates about gim axes */
   double AngCmd[3];      /* Euler angle commands, for kinematic joints */
   double RateCmd[3];     /* Euler angle rate commands, for kinematic joints */
   double RotSpringCoef[3];  /* For passive joint torques */
   double RotDampCoef[3];    /* For passive joint torques */
   double TrnSpringCoef[3];  /* For passive joint forces */
   double TrnDampCoef[3];    /* For passive joint forces */
   double PassiveTrq[3];  /* Trq from passive spring, damper */
   double PassiveFrc[3];  /* Frc from passive spring, damper in translational joint...still uses SpringCoef and DampCoef */
   /* Frames involved in a joint: Bo <-> (Bfo) <-> Go <-> Gi <-> (Bfi) <-> Bi */
   double CGiBi[3][3];    /* Constant orientation of joint in Bi (Bfi) */
   double CBoGo[3][3];    /* Constant orientation of joint in Bo (Bfo) */
   double CGoGi[3][3];    /* Euler rot through ang[0], ang[1], ang[2] */
   double CTrqBo[3][3];   /* Used for transforming joint torques or forces from Go to Bo */
   double CTrqBi[3][3];   /* Used for transforming joint torques from Go to Bi */
   double COI[3][3];      /* DCM from inner body to outer body, (incl flex) */
   double Trq[3];         /* Exerted on Bout, components along gimbal axes */
   double Frc[3];         /* Force exerted on Bout, components along translational axes */
   double Gamma[3][3];        /* w = Gamma*sigma */
   double Delta[3][3];        /* v = Delta*s -- matrix of joint partials for translational joints */
   double Gs[3];          /* Gamma*sigma */
   double Gds[3];         /* Gammadot*sigma */
   double Ds[3];          /* Delta*s */
   double Dds[3];         /* Deltadot*s */
   long Rotu0;              /* Index of first Rot element in u */
   long Rotx0;              /* Index of first Rot element in x */
   long Trnu0;              /* Index of first Trn element in u */
   long Trnx0;              /* Index of first Trn element in x */
   /* For Flex */
   double **PSIi;         /* Translation Mode Shapes, 3 x Bi.Nf */
   double **THETAi;       /* Rotational Mode Shapes, 3 x Bi.Nf */
   double **PSIo;         /* Translation Mode Shapes, 3 x Bo.Nf */
   double **THETAo;       /* Rotational Mode Shapes, 3 x Bo.Nf */
   double FlexPosi[3];    /* Translational Flex Deflection of Bi at G (d) */
   double FlexVeli[3];    /* Translational Flex Velocity of Bi at G   (e) */
   double FlexAngi[3];    /* Rotational Flex Deflection of Bi at G    (delta) */
   double FlexAngVeli[3]; /* Rotational Flex Velocity of Bi at G      (eta) */
   double FlexPoso[3];    /* Translational Flex Deflection of Bo at G (d) */
   double FlexVelo[3];    /* Translational Flex Velocity of Bo at G   (e) */
   double FlexAngo[3];    /* Rotational Flex Deflection of Bo at G    (delta) */
   double FlexAngVelo[3]; /* Rotational Flex Velocity of Bo at G      (eta) */
   /* For Constraints */
   long Rotc0;
   long Trnc0;
};

struct WhlType {
   double H;  /* Angular Momentum, Nms */
   double J;  /* Rotary inertia, kg-m^2 */
   double w;  /* Angular speed, rad/sec */
   double A[3]; /* Axis vector wrt Body 0 */
   double Tmax;
   double Hmax;
   double Trq;  /* Exerted on wheel, expressed along wheel axis */
   long FlexNode;
   double Uhat[3],Vhat[3]; /* Transverse basis vectors wrt Body 0 */
   double ang; /* Spin angle, rad */
   double Ks;  /* Static imbalance coefficient, [kg-m] */
   double Kd;  /* Dynamic imbalance coefficient, [kg-m^2] */
};

struct MTBType {
   double M;
   double A[3]; /* Axis vector wrt Body 0 */
   double Mmax;
   double Trq[3]; /* Exerted on Body 0, expressed in B[0] frame */
   long FlexNode;
};

struct ThrusterType {
   double Fmax;
   double F;
   double A[3]; /* Axis vector wrt Body 0 */
   double r[3]; /* Position vector in Body 0 */
   double Frc[3]; /* Force exerted */
   double Trq[3]; /* Torque exerted */
   long FlexNode;
};

struct CMGType {
   /* CMG Rotor Axis (A) is the Z axis of the frame defined by gimbal rotations */
   long DOF;     /* 1, 2, or 3 */
   long Seq; /* Euler Sequence */
   double CG[3][3]; /* Dynamic orientation */
   double CGB[3][3]; /* Static Mounting DCM */
   double CB[3][3]; /* Total orientation */
   double ang[3]; /* Typically only 1 or 2 used */
   double angrate[3]; /* Typically only 1 or 2 used */
   double MaxAngRate[3];
   double MaxAng[3];
   double J; /* Rotor MOI, kg-m^2 */
   double H; /* Momentum, constant, Nms */
   double A[3]; /* Axis, expressed in B[0] */
   double Trq[3]; /* Exerted on B[0], expressed in B[0] */
   long FlexNode;
};

struct AccelType {
   double pb[3];  /* Position in B[0] */
   double CB[3][3]; /* Mounting matrix */
   double acc[3];  /* Measured acceleration, expressed in A */
   long FlexNode;
};

struct JointPathTableType { /* tells if joint is in path of body*/
   long InPath;
   double rho[3];
};

struct BodyPathTableType { /* tells if inner body is in path of outer body*/
   long InPath;
   double Coi[3][3];
};

struct DynType {
   long Nu;   /* 6 + Sum(Joint DOFs) */
   long Nx;   /* 7 + Sum(Joint DOFs), (spherical gives 4) */
   long **ConnectTable;  /* Nb x Ng */
   struct JointPathTableType **JointPathTable;  /* Nb x Ng */
   struct BodyPathTableType **BodyPathTable;  /* Nb x Nb */
   double **PAngVel; /* 3*Nb x Nu */
   double **IPAngVel; /* 3*Nb x Nu */
   double **PVel;   /* 3*Nb x Nu */
   double **mPVel;   /* 3*Nb x Nu */
   double *BodyTrq;  /* 3*Nb x 1 */
   double *BodyFrc;  /* 3*Nb x 1 */
   double **COEF;  /* (Nu+Nf) x (Nu+Nf) */
   double *RHS;    /* (Nu+Nf) x 1 */
   long SomeJointsLocked; /* 1 if any DOFs are locked */
   long Ns;  /* Number of active states (joint + flex), <= (Nu+Nf) */
   double *DynState; /* u and uf concatenated, (Nu+Nf) x 1 */
   long *DynStateIdx;  /* Keeps track of active states, (Nu+Nf) x 1 */
   double *u,*uu,*du,*udot;  /* Nu  (Dynamic States) */
   double *x,*xx,*dx,*xdot;  /* Nx  (Kinematic States) */
   double *h,*hh,*dh,*hdot;  /* Nw  (Wheel Momentum States) */
   /* For Flex */
   long Nf;  /* Total Number of Flex Modes, Sum(B.Nf) */
   double **PAngVelf; /* 3*Nb x Nf */
   double **IPAngVelf; /* 3*Nb x Nf */
   double **PVelf; /* 3*Nb x Nf */
   double **mPVelf; /* 3*Nb x Nf */
   double **Mf; /* Nf x Nf */
   double **PCPVelf; /* Nf x Nf */
   double **HplusQetaPAngVelf; /* Nf x Nf */
   double *uf,*uuf,*duf,*ufdot; /* Nf (Dynamic Flex States) */
   double *xf,*xxf,*dxf,*xfdot; /* Nf (Kinematic Flex States) */
   double *FlexAcc;  /* Nf x 1 */
   double *FlexFrc;  /* Nf x 1 */
   double *FlexInertiaFrc; /* Nf x 1 */
   /* For Constraints */
   long Nc;  /* 6*Nb - Nu */
   double **PAngVelc; /* 3*Nb x Nc */
   double **PVelc; /* 3*Nb x Nc */
   double *TotalTrq; /* 3*Nb x 1 */
   double *TotalFrc; /* 3*Nb x 1 */
   double *GenConstraintFrc;  /* Nc x 1 */
};

struct EnvTrqType {
   long First;
   FILE *envfile;
   double Hs[3];
};

struct SCType {
   long Tag;     /* SC[Tag].Tag = Tag */
   long Exists;
   char Label[40];
   long RotDOF;  /* STEADY, KIN_JOINT, DYN_JOINT */
   long OrbDOF;  /* FIXED, EULER_HILL, ENCKE, COWELL */
   long RefOrb;
   long FswTag;  /* Tag for FSW function */
   long Nb;   /* Number of bodies */
   long Ng;   /* Number of joints, = Nb-1 */
   long Nw;   /* Number of wheels */
   long Nmtb; /* Number of MTB's  [0 - 4] */
   long Nthr; /* Number of thrusters [0 - 12] */
   long Ncmg; /* Number of Control Moment Gyros [0 - 4] */
   long Nacc; /* Number of accelerometers [0 - 4] */
   struct BodyType *B;
   struct JointType *G;
   struct WhlType *Whl;
   struct MTBType *MTB;
   struct ThrusterType *Thr;
   struct CMGType *CMG;
   struct AccelType Accel[4];
   double mass;
   double cm[3]; /* wrt B0 origin, expressed in B0 frame */
   double I[3][3]; /* Inertia matrix, wrt SC.cm, expressed in B0 frame */
   double PosR[3]; /* Position of cm wrt Reference Orbit (Orb[RefOrb], or R), m, expressed in N */
   double VelR[3]; /* Velocity of cm wrt R, m/sec, expressed in N */
   double PosEH[3];  /* Position of cm wrt R, m, in Euler-Hill coords */
   double VelEH[3];  /* Velocity of cm wrt R, m, in Euler-Hill coords */
   double PosN[3];   /* Position of cm wrt origin of N, m, expressed in N */
   double VelN[3];   /* Velocity of cm wrt origin of N, m/sec, expressed in N */
   double CLN[3][3]; /* Note that SC.CLN != Orb[RefOrb].CLN if SC.Rrel != 0 */
   double wln[3]; /* Expressed in N */
   double PosH[3];  /* Position of cm wrt H frame, expressed in H */
   double VelH[3];  /* Velocity of cm wrt H frame, expressed in H */
   double Frc[3]; /* Force, N, expressed in N */
   double svn[3]; /* Sun-pointing unit vector, expressed in N */
   double svb[3]; /* Sun-pointing unit vector, expressed in SC.B[0] */
   double bvn[3]; /* Magfield, Tesla, expressed in N */
   double bvb[3]; /* Magfield, Tesla, expressed in SC.B[0] */
   double Hvn[3]; /* Total SC angular momentum, Nms, expressed in N */
   double Hvb[3]; /* Total SC angular momentum, Nms, expressed in SC.B[0] */
   long Eclipse;
   double AtmoDensity;
   double DragCoef;
   char FileName[20];
   char SpriteFileName[40];
   unsigned int SpriteTexTag;
   /* The following are for OSCAR */
   double PosF[3]; /* Position of B0 origin wrt F, expressed in F */
   double VelF[3]; /* Velocity of B0 origin wrt F, expressed in F */
   double CF[3][3];  /* Attitude of B0 wrt F */
   /* These are computed in dynamics, used in accelerometer models */
   double asn[3];  /* Non-gravitational accel of SC.cm in N */
   double alfbn[3]; /* Angular accel of B wrt N, expressed in B */
   double abs[3];  /* Non-grav accel of B[0].cm, wrt SC.cm, in N */
   /* For dynamics model construction */
   long MassIsConstant;
   /* Enable/Disable Passive Joint Forces or Torques (i.e. spring/damper) */
   long PassiveJointFrcTrqEnabled;
   /* Constraint forces and torques are computed if requested */
   long ConstraintsRequested;
   /* Mass and flex properties referred to REFPT_CM or REFPT_JOINT */
   long RefPt;
   /* Flexible Dynamics Active */
   long FlexActive;
   /* Include higher-order coupling terms in rigid-flex dynamics */
   long IncludeSecondOrderFlexTerms;
   /* Workspace for KaneNBody */
   struct DynType Dyn;
   /* Workspace for Actuator Sizing */
   struct EnvTrqType EnvTrq;
   /* Should FSW be part of SC, or its own structure? */
   struct FSWType FSW;
   /* Bounding Box used for shadowmap */
   struct BoundingBoxType BBox;
   /* See ReadStatesFromSocket */
   long RequestStateRefresh;
};

struct TargetType {
   long Type;
   long World;
   long RefOrb;
   long SC;
   long Body;
   double PosR[3];
   double PosN[3];
   double PosH[3];
   double CN[3][3];
};

struct POVType {
   long Mode; /* Track Host, Track Target, or Fixed in Host */
   struct TargetType Host;
   struct TargetType Target;
   long View;
   long Frame;  /* Which frame is POV frame oriented in? (N=0,L=1,F=2,B=3) */
   long BoreAxis;  /* POV boresight axis (out of screen): POS_X, POS_Y, ..., NEG_Z */
   long UpAxis;    /* POV axis pointing to top of window: POS_X, ... NEG_Z */
   double Width; /* Width of POV Field of View */
   double Height; /* Height of POV Field of View */
   double Near, Far;  /* Near and Far limits of POV FOV */
   double CosFov, SinFov;
   double Angle; /* Angle subtended in vertical, deg */
   double AR;    /* Aspect ratio of POV FOV */
   double PosLeftEye[3]; /* in POV frame, expressed in POV */
   double PosRightEye[3]; /* in POV frame, expressed in POV */
   double w[3];  /* Angular velocity */
   double q[4];  /* Quaternion */
   double PosB[3];  /* Position wrt Host, expressed in Host B[0] Frame */
   double Range;
   double GridSpacing; /* For ProxOps Grid */
   double wmax[3];
   double C[3][3];
   double CN[3][3];
   double CH[3][3];
   double CL[3][3];
   double CF[3][3];
   double CB[3][3];
   double PosR[3]; /* Position vector in R, expressed in N */
   double PosN[3]; /* Position vector in N, expressed in N */
   double PosH[3]; /* Position vector in H, expressed in H */
   float ViewMatrix[16];
   /* For PanZoomPOV */
   double TimeToGo;
   long CmdSeq;
   double CmdAngle[3];
   double CmdRange;
   double CmdPermute[3][3];
};

struct RegionType {
   long Exists;
   long World;
   double Lng,Lat,Alt; /* Origin location */
   double PosW[3];
   double CW[3][3]; /* Region frame is East-North-Up */
   double PosN[3];
   double VelN[3];
   double CN[3][3];
   double wn[3]; /* Expressed in R frame */
   double ElastCoef,DampCoef,FricCoef;
   char Name[20];
   char GeomFileName[40];
   long GeomTag;
   float ModelMatrix[16]; /* For OpenGL */
};

struct AtmoType {
   long Exists;
   float GasColor[3];
   float DustColor[3];
   double ScaleHt;
   double MaxHt;
   double rad;
};

struct WorldType {
   /* Relationships */
   long Exists;
   long Type; /* STAR, PLANET, MOON, ASTEROID, COMET */
   long Parent;
   long Nsat;
   long *Sat;

   /* Physical Properties */
   double mu; /* Gravitation constant  */
   double rad; /* Radius */
   double w; /* Spin Rate */
   double RadOfInfluence; /* Radius of Sphere of Influence */
   double DipoleMoment;  /* Magnetic Field Dipole Moment, Wb-m */
   double DipoleAxis[3]; /* Magnetic Field Dipole Axis */
   double DipoleOffset[3]; /* Dipole Offset, m */
   double RingInner, RingOuter;

   /* Ephemeris */
   struct OrbitType eph; /* Ephemeris Parameters */

   /* Graphical Properties */
   long HasRing;
   char Name[20];
   char MapFileName[40];
   char GeomFileName[40];
   char ColTexFileName[40];
   char BumpTexFileName[40];
   float Color[4];
   unsigned char Glyph[14];
   unsigned int TexTag;
   unsigned int MapTexTag;
   unsigned int ColTexTag;
   unsigned int BumpTexTag;
   unsigned int ColCubeTag;
   unsigned int BumpCubeTag;
   unsigned int CloudGlossCubeTag;
   long GeomTag;
   unsigned int RingTexTag;
   unsigned int RingList;
   struct AtmoType Atmo;

   /* State Variables */
   double PosH[3];         /* Position in H frame */
   double VelH[3];         /* Velocity in H frame */
   double CNH[3][3]; /* DCM from heliocentric ecliptic frame
                        to world-centric equatorial inertial frame */
   double PriMerAng; /* Angle from N1 to prime meridian */
   double CWN[3][3]; /* DCM from world-centric inertial frame
                        to world-centric rotating frame */
   long Visibility; /* Too small to see, point-sized, or shows disk */
   float ModelMatrix[16];
};

struct SpotType {
   long xmin,ymin;
   long xmax,ymax;
   long Visible;
   long Selected;
};

struct WidgetType {
   long xmin,ymin;
   long xmax,ymax;
   float BorderColor[4];
   float TextColor[4];
   float SelectedColor[4];
   float UnselectedColor[4];
   long Nspot;
   struct SpotType *Spot;
};

struct FovType {
   long Type; /* WIREFRAME, SOLID, or VECTOR */
   long NearExists;
   long FarExists;
   long RefOrb;
   long SC;
   long Body;
   char Label[40];
   long Nv;
   double Width; /* X angular dimension, rad */
   double Height; /* Y angular dimension, rad */
   double Length;
   double pb[3];
   double CB[3][3];
   float Color[4];
};

struct TdrsType {
   long Exists;
   double JD;
   double rw[3]; /* Position vector in ECEF frame */
   double PosN[3];  /* Position vector in N frame */
   double VelN[3];  /* Velocity vector in N frame (never used?) */
   double lat;
   double lng;
   char Designation[40];
};

struct GroundStationType {
   long Exists;
   long World;
   long Show;
   double lng, lat;
   double PosW[3];  /* Position vector in World frame */
   char Label[40];
};

/* Framebuffer Objects for Spacecraft Shadows */
struct FBOType {
   unsigned int FrameTag;
   unsigned int Height, Width;
   unsigned int RenderTag;
   unsigned int TexTag;
};

/* Orrery POV is different from POV */
struct OrreryPOVType {
   long Regime;  /* CENTRAL or THREE_BODY */
   long CenterType;
   long World;
   long LagSys;
   long MinorBody;
   long LP;
   double PosN[3]; /* Position wrt World, expressed in World N */
   double Radius;
   long Zoom;
   double Scale[30];
   char ScaleLabel[30][8];
   double Angle;
   double CNH[3][3];
   double CN[3][3];
   double CH[3][3];
   double CL[3][3];
};

/*
** #ifdef __cplusplus
** }
** #endif
*/

#endif /* __42TYPES_H__ */

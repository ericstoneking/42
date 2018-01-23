/*    This file is distributed with 42,                               */
/*    the (mostly harmless) spacecraft dynamics simulation            */
/*    created by Eric Stoneking of NASA Goddard Space Flight Center   */

/*    Copyright 2010 United States Government                         */
/*    as represented by the Administrator                             */
/*    of the National Aeronautics and Space Administration.           */

/*    No copyright is claimed in the United States                    */
/*    under Title 17, U.S. Code.                                      */

/*    All Other Rights Reserved.                                      */


/* Disable extern keyword to declare globals */
#ifdef DECLARE_GLOBALS
   #define EXTERN
#else
   #define EXTERN extern
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "42types.h"
#include "42defines.h"
#include "dcmkit.h"
#include "envkit.h"
#include "geomkit.h"
#include "iokit.h"
#include "mathkit.h"
#include "orbkit.h"
#include "sigkit.h"
#include "sphkit.h"
#include "timekit.h"
#include "fswkit.h"
#include "msis86kit.h"

/*
** #ifdef __cplusplus
** namespace _42 {
** using namespace Kit;
** #endif
*/

/* Number of Reference Orbits */
EXTERN long Norb;
/* Number of spacecraft */
EXTERN long Nsc;

/* Number of materials */
EXTERN long Nmatl;
/* Number of geometric objects */
EXTERN long Ngeom;

EXTERN char InOutPath[80];
EXTERN char ModelPath[80];
EXTERN char CmdFileName[80];

/* Math Basics */
EXTERN double Pi, TwoPi, HalfPi, SqrtTwo, SqrtHalf, D2R, R2D;

/* Simulation Control */
EXTERN long TimeMode; /* FAST_TIME, REAL_TIME, EXTERNAL_SYNCH */
EXTERN long IpcMode; /* IPC_OFF, IPC_TX, IPC_RX */
EXTERN long SocketRole; /* IPC_SERVER or IPC_CLIENT */
EXTERN double SimTime,STOPTIME,DTSIM,DTOUT,DTOUTGL;
EXTERN long OutFlag,GLOutFlag,GLEnable,CleanUpFlag;
EXTERN double AbsTime; /* Absolute Time, sec since J2000 Epoch */
EXTERN double AbsTimeOffset; /* Added to AbsTime to account for offsets between TAI, TDB, etc */

/* Environment */
EXTERN struct SphereHarmType MagModel;  /* -3,...,10 */
EXTERN long SurfaceModel;  /* 0=Brick, 1=CylPlate */
EXTERN struct SphereHarmType EarthGravModel;  /* Degree and Order of Earth gravitational model */
EXTERN struct SphereHarmType MarsGravModel;  /* Degree and Order of Mars gravitational model */
EXTERN struct SphereHarmType LunaGravModel;  /* Degree and Order of Luna gravitational model */
EXTERN long AeroActive;
EXTERN long AeroShadowsActive;
EXTERN long GGActive;
EXTERN long SolPressActive;
EXTERN long SolPressShadowsActive;
EXTERN long GravPertActive;
EXTERN long JointTrqActive;
EXTERN long ThrusterPlumesActive;
EXTERN long RwaImbalanceActive;
EXTERN long ContactActive;
EXTERN long SloshActive;
EXTERN long ComputeEnvTrq;

/* Calendar Time */
EXTERN double AbsTime0; /* Time in sec since J2000 Epoch at Sim Start */
EXTERN double JulDay;
EXTERN long doy,Year,Month,Day,Hour,Minute;
EXTERN double Second;
EXTERN long GpsRollover,GpsWeek;
EXTERN double GpsSecond;

/* Parameters for environmental models  */
EXTERN long UseFileForInterpolation; /*TWOSIGMA_KP, NOMINAL, USER_DEFINED*/
EXTERN double Flux10p7, GeomagIndex;

EXTERN struct WorldType World[NWORLD];
EXTERN struct LagrangeSystemType LagSys[3];

/* Galactic Coordinate Frame */
EXTERN double CGH[3][3];

/* SC structure manages attitude and translation wrt Reference Orbit */
EXTERN struct SCType *SC;
/* Orb structure manages Reference Orbits */
EXTERN struct OrbitType *Orb;
/* Frm structure describes a Formation of S/C's */
EXTERN struct FormationType *Frm;

/* Geom structure manages geometric objects, used for display and    */
/* for surface force computation (e.g. aerodynamic)                  */
EXTERN struct GeomType *Geom;

EXTERN struct POVType POV;

EXTERN struct TdrsType Tdrs[10];
EXTERN struct GroundStationType *GroundStation;
EXTERN long Ngnd;

/* For drawing Fields of View */
EXTERN long Nfov;
EXTERN struct FovType *FOV;

/* Materials for graphics and solar pressure forces.                  */
EXTERN struct MatlType *Matl;

/* Framebuffer Objects for Shadows and Surface Forces */
EXTERN struct FBOType ShadowMap;

/* Minor Bodies (Asteroids and Comets) */
EXTERN long Nmb;

EXTERN long Nrgn;
EXTERN struct RegionType *Rgn;

/* CFD execution control */
EXTERN long ExecuteCFDStep;
EXTERN long EndCFD;

EXTERN SOCKET TxSocket,RxSocket;
EXTERN long EchoEnabled;

long SimStep(void);
void Ephemerides(void);
void OrbitMotion(void);
void Environment(struct SCType *S);
void Perturbations(struct SCType *S);
void Sensors(struct SCType *S);
void SensorDriver(struct SCType *S);
void FlightSoftWare(struct SCType *S);
void ActuatorDriver(struct SCType *S);
void Actuators(struct SCType *S);
void CmdInterpreter(void);
void Report(void);
void DrawScene(void);
void ThreeBodyOrbitRK4(struct OrbitType *O);
void MotionConstraints(struct SCType *S);
void SCMassProps(struct SCType *S);
void MapJointStatesToStateVector(struct SCType *S);
void MapStateVectorToBodyStates(double *u, double *x, double *uf,
   double *xf, struct SCType *S);
void PartitionForces(struct SCType *S);
void Dynamics(struct SCType *S);
void Cleanup(void);
void FindInterBodyDCMs(struct SCType *S);
void FindPathVectors(struct SCType *S);
void FindTotalAngMom(struct SCType *S);
double FindTotalKineticEnergy(struct SCType *S);
void UpdateScBoundingBox(struct SCType *S);
void FindCmgAxisAndTrq(struct CMGType *C);
void FindUnshadedAreas(struct SCType *S, double DirVecN[3]);
void RadBelt(float RadiusKm, float MagLatDeg, int NumEnergies, 
      float *ElectronEnergy, float *ProtonEnergy, double **Flux); 

/* Debug Function Prototypes */
void EchoPVel(struct SCType *S);
void EchoEOM(double **COEF, double *State, double *RHS, long Ns);
void EchoStates(int Nx, double *x, int Nu, double *u);
void EchoRemAcc(struct SCType *S);

void InitSim(int argc, char **argv);
void InitOrbits(void);
void InitSpacecraft(struct SCType *S);
void LoadPlanets(void);
long DecodeString(char *s);
void InitFSW(struct SCType *S);
void InitLagrangePoints(void);

long LoadTRVfromFile(const char *Path, const char *TrvFileName,
   const char *ElemLabel, double AbsTime, struct OrbitType *O);
void SplineToPosVel(struct OrbitType *O);

void CfdSlosh(struct SCType *S);
void FakeCfdSlosh(struct SCType *S);
void SendStatesToSpirent(void);

#ifdef _ENABLE_SOCKETS_
   void InterProcessComm(void);
   void InitInterProcessComm(void);
#endif

#undef EXTERN

/*
** #ifdef __cplusplus
** }
** #endif
*/






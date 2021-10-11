/*    This file is distributed with 42,                               */
/*    the (mostly harmless) spacecraft dynamics simulation            */
/*    created by Eric Stoneking of NASA Goddard Space Flight Center   */

/*    Copyright 2010 United States Government                         */
/*    as represented by the Administrator                             */
/*    of the National Aeronautics and Space Administration.           */

/*    No copyright is claimed in the United States                    */
/*    under Title 17, U.S. Code.                                      */

/*    All Other Rights Reserved.                                      */


#ifndef __42DEFINES_H__
#define __42DEFINES_H__

#define TRUE 1
#define FALSE 0

#define ON 1
#define OFF 0

#define AU2m 149597870000.0

#define ABS(X) ((X)>0 ? (X) : (-X))
#define MIN(X,Y) ((X)<(Y) ? (X) : (Y))
#define MAX(X,Y) ((X)>(Y) ? (X) : (Y))

#define IDX2(x,y,Ny)             ((x)*(Ny)+(y))
#define IDX3(x,y,z,Ny,Nz)       (((x)*(Ny)+(y))*(Nz)+(z))
#define IDX4(x,y,z,w,Ny,Nz,Nw) ((((x)*(Ny)+(y))*(Nz)+(z))*(Nw)+(w))

#define SOL 0
#define MERCURY 1
#define VENUS 2
#define EARTH 3
#define MARS 4
#define JUPITER 5
#define SATURN 6
#define URANUS 7
#define NEPTUNE 8
#define PLUTO 9
#define LUNA 10
/* Moons of Mars */
#define PHOBOS 11
#define DEIMOS 12
/* Major Moons of Jupiter */
#define IO 13
#define EUROPA 14
#define GANYMEDE 15
#define CALLISTO 16
#define AMALTHEA 17
#define HIMALIA 18
#define ELARA 19
#define PASIPHAE 20
#define SINOPE 21
#define LYSITHEA 22
#define CARME 23
#define ANANKE 24
#define LEDA 25
#define THEBE 26
#define ADRASTEA 27
#define METIS 28
/* Major Moons of Saturn */
#define MIMAS 29
#define ENCELADUS 30
#define TETHYS 31
#define DIONE 32
#define RHEA 33
#define TITAN 34
#define HYPERION 35
#define IAPETUS 36
#define PHOEBE 37
#define JANUS 38
#define EPIMETHEUS 39
#define HELENE 40
#define TELESTO 41
#define CALYPSO 42
#define ATLAS 43
#define PROMETHEUS 44
#define PANDORA 45
#define PAN 46
/* Major Moons of Uranus */
#define ARIEL 47
#define UMBRIEL 48
#define TITANIA 49
#define OBERON 50
#define MIRANDA 51
/* Major Moons of Neptune */
#define TRITON 52
#define NERIED 53
/* Pluto's moon */
#define CHARON 54

/* Minor Bodies */
#define MINORBODY_0 55
#define MINORBODY_1 56
#define MINORBODY_2 57
#define MINORBODY_3 58
#define MINORBODY_4 59
#define MINORBODY_5 60
#define MINORBODY_6 61
#define MINORBODY_7 62
#define MINORBODY_8 63
#define MINORBODY_9 64

#define NWORLD 65

#define POSITIVE 1
#define NEGATIVE 0

#define DIR_CW 0
#define DIR_CCW 1

#define ORB_ZERO 0
#define ORB_FLIGHT 1
#define ORB_CENTRAL 2
#define ORB_THREE_BODY 3

#define INP_KEPLER 0
#define INP_POSVEL 1
#define INP_FILE 2
#define INP_TLE 3
#define INP_TRV 4
#define INP_MODES 5
#define INP_XYZ 6
#define INP_SPLINE 7

#define EARTHMOON  0
#define SUNEARTH 1
#define SUNJUPITER 2

#define LAGPT_L1 0
#define LAGPT_L2 1
#define LAGPT_L3 2
#define LAGPT_L4 3
#define LAGPT_L5 4

#define NONE 0
#define DIPOLE 1
#define IGRF 2

#define X_AXIS 0
#define Y_AXIS 1
#define Z_AXIS 2

#define POS_X 0
#define POS_Y 1
#define POS_Z 2
#define NEG_X 3
#define NEG_Y 4
#define NEG_Z 5

/* POV Frames */
#define FRAME_N 0
#define FRAME_L 1
#define FRAME_F 2
#define FRAME_S 3
#define FRAME_B 4

/* POV (Host and) Target Types */
#define TARGET_WORLD 0
#define TARGET_REFORB 1
#define TARGET_FRM 2
#define TARGET_SC 3
#define TARGET_BODY 4
#define TARGET_TDRS 5
#define TARGET_VELOCITY 7
#define TARGET_LAGPT 8
#define TARGET_MAGFIELD 9

/* POV Modes */
#define TRACK_HOST 0
#define TRACK_TARGET 1
#define FIXED_IN_HOST 2

/* POV Views */
#define VIEW_FRONT 8
#define VIEW_FRONT_RIGHT 9
#define VIEW_RIGHT 6
#define VIEW_REAR_RIGHT 3
#define VIEW_REAR 2
#define VIEW_REAR_LEFT 1
#define VIEW_LEFT 4
#define VIEW_FRONT_LEFT 7
#define VIEW_UP 5
#define VIEW_DOWN 0

#define FOV_WIREFRAME 0
#define FOV_SOLID 1
#define FOV_VECTOR 2
#define FOV_PLANE 3

#define LAGDOF_MODES 0
#define LAGDOF_COWELL 1
#define LAGDOF_SPLINE 2

#define DYN_GAUSS_ELIM 0
#define DYN_ORDER_N 1

#define ORBDOF_FIXED 0
#define ORBDOF_EULER_HILL 1
#define ORBDOF_ENCKE 2
#define ORBDOF_COWELL 3

#define EPH_MEAN 0
#define EPH_DE430 1

#define REFPT_CM 0
#define REFPT_JOINT 1

/* World Visibility */
#define WORLD_IS_TOO_SMALL_TO_SEE 0
#define WORLD_IS_POINT_SIZED 1
#define WORLD_SHOWS_DISK 2
#define WORLD_IS_SUN 3
#define WORLD_IS_OUT_OF_POV 4
#define WORLD_IS_NEAR 5

/* Time Modes */
#define FAST_TIME 0
#define REAL_TIME 1
#define EXTERNAL_TIME 2
#define NOS3_TIME 3

/* World Types */
#define SUN 0
#define PLANET 1
#define MOON 2
#define ASTEROID 3
#define COMET 4

/* FSW Tags */
#define PASSIVE_FSW 0
#define PROTOTYPE_FSW 1
#define AD_HOC_FSW 2
#define SPINNER_FSW 3
#define MOMBIAS_FSW 4
#define THREE_AXIS_FSW 5
#define ISS_FSW 6
#define CMG_FSW 7
#define THR_FSW 8
#define CFS_FSW 9
#define FREQRESP_FSW 10

/* Command Types */
#define CMD_DIRECTION 0
#define CMD_TARGET 1

/* Command Parameter Types */
#define PARM_EULER_ANGLES 0
#define PARM_QUATERNION 1
#define PARM_VECTORS 2
#define PARM_AXIS_SPIN 3

/* ProxOps View */
#define VIEW_SIDE 6
#define VIEW_TOP 7

/* Solar Activity Modes */
#define TWOSIGMA_ATMO 0
#define NOMINAL_ATMO 1
#define USER_ATMO 2

/* Modes for InterProcess Comm */
#define IPC_OFF 0
#define IPC_TX 1
#define IPC_RX 2
#define IPC_TXRX 3
#define IPC_ACS 4
#define IPC_WRITEFILE 5
#define IPC_READFILE 6
#define IPC_SPIRENT 7
#define IPC_FFTB 8

/* Socket Roles for InterProcess Comm */
#define IPC_SERVER 0
#define IPC_CLIENT 1
#define IPC_GMSEC_CLIENT 2

/* Secs from J2000 to the Unix epoch of 1 Jan 1970 */
#define UNIX_EPOCH (-946728000.0)
/* Secs from J2000 to the GPS epoch of 6 Jan 1980 */
#define GPS_EPOCH  (-630763200.0)

/* Constellation Classes */
#define NUM_CONSTELL 88
#define MAJOR_CONSTELL 0
#define ZODIAC_CONSTELL 1
#define MINOR_CONSTELL 2

#endif /* __42DEFINES_H__ */

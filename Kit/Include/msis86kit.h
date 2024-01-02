/* This file provided by John Downing                                 */
/* f2c.h copy/pasted, and other minor formatting tweaks by Eric Stoneking */

#ifndef val_atm_msis86_h
#define val_atm_msis86_h

/* #include <math.h> */

/*
** #ifdef __cplusplus
** namespace Kit {
** #endif
*/

/*====================================================================*/
/* Begin f2c.h */
/* f2c.h  --  Standard Fortran to C header file */

/**  barf  [ba:rf]  2.  "He suggested using FORTRAN, and everybody barfed."

   - From The Shogakukan DICTIONARY OF NEW ENGLISH (Second edition) */

#ifndef F2C_INCLUDE
#define F2C_INCLUDE
typedef long int integer;
typedef char *address;
typedef short int shortint;
/* typedef float real; */
typedef double real;
typedef real doublereal;
/* typedef struct { real r, i; } complex; */
typedef struct { doublereal r, i; } doublecomplex;
typedef long int logical;
typedef short int shortlogical;
typedef char logical1;
typedef char integer1;
/* typedef long long longint; */ /* system-dependent */

#define TRUE_ (1)
#define FALSE_ (0)

/* Extern is for use with -E */
#ifndef Extern
#define Extern extern
#endif

/* I/O stuff */

#ifdef f2c_i2
/* for -i2 */
typedef short flag;
typedef short ftnlen;
typedef short ftnint;
#else
typedef long flag;
typedef long ftnlen;
typedef long ftnint;
#endif

/*external read, write*/
typedef struct
{  flag cierr;
   ftnint ciunit;
   flag ciend;
   char *cifmt;
   ftnint cirec;
} cilist;

/*internal read, write*/
typedef struct
{  flag icierr;
   char *iciunit;
   flag iciend;
   char *icifmt;
   ftnint icirlen;
   ftnint icirnum;
} icilist;

/*open*/
typedef struct
{  flag oerr;
   ftnint ounit;
   char *ofnm;
   ftnlen ofnmlen;
   char *osta;
   char *oacc;
   char *ofm;
   ftnint orl;
   char *oblnk;
} olist;

/*close*/
typedef struct
{  flag cerr;
   ftnint cunit;
   char *csta;
} cllist;

/*rewind, backspace, endfile*/
typedef struct
{  flag aerr;
   ftnint aunit;
} alist;

/* inquire */
typedef struct
{  flag inerr;
   ftnint inunit;
   char *infile;
   ftnlen infilen;
   ftnint   *inex;   /*parameters in standard's order*/
   ftnint   *inopen;
   ftnint   *innum;
   ftnint   *innamed;
   char  *inname;
   ftnlen   innamlen;
   char  *inacc;
   ftnlen   inacclen;
   char  *inseq;
   ftnlen   inseqlen;
   char  *indir;
   ftnlen   indirlen;
   char  *infmt;
   ftnlen   infmtlen;
   char  *inform;
   ftnint   informlen;
   char  *inunf;
   ftnlen   inunflen;
   ftnint   *inrecl;
   ftnint   *innrec;
   char  *inblank;
   ftnlen   inblanklen;
} inlist;

#define VOID void

union Multitype { /* for multiple entry points */
   integer1 g;
   shortint h;
   integer i;
   /* longint j; */
   real r;
   doublereal d;
   doublecomplex c;
   doublecomplex z;
};

typedef union Multitype Multitype;

typedef long Long;   /* No longer used; formerly in Namelist */

struct Vardesc {  /* for Namelist */
   char *name;
   char *addr;
   ftnlen *dims;
   int  type;
   };
typedef struct Vardesc Vardesc;

struct Namelist {
   char *name;
   Vardesc **vars;
   int nvars;
   };
typedef struct Namelist Namelist;

#define f2c_abs(x) ((x) >= 0 ? (x) : -(x))
#define dabs(x) (doublereal) f2c_abs(x)
/* These defines might trip up a Raspberry Pi, and are only used once.  
   Inlining.  See msis86kit.c:570
#ifndef min
#define min(a,b) ((a) <= (b) ? (a) : (b))
#endif
#ifndef max
#define max(a,b) ((a) >= (b) ? (a) : (b))
#endif
#define dmin(a,b) (doublereal) min(a,b)
#define dmax(a,b) (doublereal) max(a,b)
*/

/* procedure parameter types for -A and -C++ */

#define F2C_proc_par_types 1
#ifdef __cplusplus
typedef int /* Unknown procedure type */ (*U_fp)(...);
typedef shortint (*J_fp)(...);
typedef integer (*I_fp)(...);
typedef real (*R_fp)(...);
typedef doublereal (*D_fp)(...), (*E_fp)(...);
typedef /* Complex */ VOID (*C_fp)(...);
typedef /* Double Complex */ VOID (*Z_fp)(...);
typedef logical (*L_fp)(...);
typedef shortlogical (*K_fp)(...);
typedef /* Character */ VOID (*H_fp)(...);
typedef /* Subroutine */ int (*S_fp)(...);
#else
typedef int /* Unknown procedure type */ (*U_fp)();
typedef shortint (*J_fp)();
typedef integer (*I_fp)();
typedef real (*R_fp)();
typedef doublereal (*D_fp)(), (*E_fp)();
typedef /* Complex */ VOID (*C_fp)();
typedef /* Double Complex */ VOID (*Z_fp)();
typedef logical (*L_fp)();
typedef shortlogical (*K_fp)();
typedef /* Character */ VOID (*H_fp)();
typedef /* Subroutine */ int (*S_fp)();
#endif
/* E_fp is for real functions when -R is not specified */
typedef VOID C_f; /* complex function */
typedef VOID H_f; /* character function */
typedef VOID Z_f; /* double complex function */
typedef doublereal E_f; /* real function with -R not specified */

/* undef any lower-case symbols that your C compiler predefines, e.g.: */

#ifndef Skip_f2c_Undefs
#undef cray
#undef gcos
#undef mc68010
#undef mc68020
#undef mips
#undef pdp11
#undef sgi
#undef sparc
#undef sun
#undef sun2
#undef sun3
#undef sun4
#undef u370
#undef u3b
#undef u3b2
#undef u3b5
#undef unix
#undef vax
#endif
#endif
/* End f2c.h */
#include "math.h"

/* Common Block Declarations */

struct gts3c_86Type {
    real tlb, s, db04, db16, db28, db32, db40, db48, db01, za, t0, z0, g0, rl,
        dd, db14;
};

EXTERN struct gts3c_86Type gts3c_86;

#define gts3c_1 gts3c_86

struct lower5_1_ {
    real ptm[8], pdm[56]   /* was [8][7] */;
};

#define lower5_1 (*(struct lower5_1_ *) &lower5_)

struct parm5_1_ {
    real pt[150], pd[1050] /* was [150][7] */, ps[150], pdl[50]   /*
       was [25][2] */;
};
struct parm5_2_ {
    real pt1[50], pt2[50], pt3[50], pa1[50], pa2[50], pa3[50], pb1[50], pb2[
       50], pb3[50], pc1[50], pc2[50], pc3[50], pd1[50], pd2[50], pd3[50]
       , pe1[50], pe2[50], pe3[50], pf1[50], pf2[50], pf3[50], pg1[50],
       pg2[50], pg3[50], ph1[50], ph2[50], ph3[50], pi1[50];
};

#define parm5_1 (*(struct parm5_1_ *) &parm5_)
#define parm5_2 (*(struct parm5_2_ *) &parm5_)

struct csw_86Type {
    real sw[25];
    integer isw;
    real swc[25];
};

EXTERN struct csw_86Type csw_86;

#define csw_1 csw_86


struct parmb_1_ {
    real gsurf, re;
};

#define parmb_1 (*(struct parmb_1_ *) &parmb_86)

struct fit_86Type {
    real taf;
};

EXTERN struct fit_86Type fit_86;

#define fit_1 fit_86

struct lsqv_86Type {
    integer mp, ii, jg, lt;
    real qpb[50];
    integer ierr, ifun, n, j;
    real dv[60];
};

EXTERN struct lsqv_86Type lsqv_86;

#define lsqv_1 lsqv_86

struct ipoly_86Type {
    real plg[36]  /* was [9][4] */, ctloc, stloc, c2tloc, s2tloc,
       c3tloc, s3tloc;
    integer iyr;
    real day, df, dfa, apd, apdf, apt[4];
};

EXTERN struct ipoly_86Type lpoly_86;

#define lpoly_1 lpoly_86

/* Initialized data */

struct parmb_86Type {
    real e_1[2];
};

EXTERN struct parmb_86Type parmb_86;

struct lower5_Type {
    real e_1[64];
};
EXTERN struct lower5_Type lower5_;

struct parm5_Type {
    real e_1[1400];
};
EXTERN struct parm5_Type parm5_;


/*====================================================================*/
int gts5_(integer *iyd, real *sec, real *alt, real *glat,
   real *glong, real *stl, real *f107a, real *f107, real *ap, integer *
   mass, real *d__, real *t);
int tselec_86(real *);
int tretrv_86(real *svv);
/*====================================================================*/
/*        MSIS-86/CIRA 1986 Neutral Thermosphere Model                */
/*         A.E.Hedin 3/15/85;2/26/87 (Variable Names Shortened)       */
/*         10/14/87 Increase altitude limit of O mixing calculation   */
/*             ALTL(2) FROM 300. TO 400.                              */
/*         8/10/88 CHANGE HANDLING OF SW(9) IN GLOBE                  */
/*     INPUT:                                                         */
/*        IYD - YEAR AND DAY AS YYDDD                                 */
/*        SEC - UT(SEC)                                               */
/*        ALT - ALTITUDE(KM) (GREATER THAN 85 KM)                     */
/*        GLAT - GEODETIC LATITUDE(DEG)                               */
/*        GLONG - GEODETIC LONGITUDE(DEG)                             */
/*        STL - LOCAL APPARENT SOLAR TIME(HRS)                        */
/*        F107A - 3 MONTH AVERAGE OF F10.7 FLUX                       */
/*        F107 - DAILY F10.7 FLUX FOR PREVIOUS DAY                    */
/*        AP - MAGNETIC INDEX(DAILY) OR WHEN SW(9)=-1. :              */
/*           - ARRAY CONTAINING:                                      */
/*             (1) DAILY AP                                           */
/*             (2) 3 HR AP INDEX FOR CURRENT TIME                     */
/*             (3) 3 HR AP INDEX FOR 3 HRS BEFORE CURRENT TIME        */
/*             (4) 3 HR AP INDEX FOR 6 HRS BEFORE CURRENT TIME        */
/*             (5) 3 HR AP INDEX FOR 9 HRS BEFORE CURRENT TIME        */
/*             (6) AVERAGE OF EIGHT 3 HR AP INDICIES FROM             */
/*                    12 TO 33 HRS PRIOR TO CURRENT TIME              */
/*             (7) AVERAGE OF EIGHT 3 HR AP INDICIES FROM             */
/*                    36 TO 59 HRS PRIOR TO CURRENT TIME              */
/*        MASS - MASS NUMBER (ONLY DENSITY FOR SELECTED GAS IS        */
/*                 CALCULATED.  MASS 0 IS TEMPERATURE.                */
/*                 MASS 48 FOR ALL.)                                  */
/*     Note:  Ut, Local Time, and Longitude are used independently    */
/*            in the model and are not of equal importance for every  */
/*            situation.  For the most physically realistic           */
/*            calculation these three variables should be             */
/*            consistent (STL=SEC/3600+GLONG/15).                     */
/*     OUTPUT:                                                        */
/*        D(1) - HE NUMBER DENSITY(CM-3)                              */
/*        D(2) - O NUMBER DENSITY(CM-3)                               */
/*        D(3) - N2 NUMBER DENSITY(CM-3)                              */
/*        D(4) - O2 NUMBER DENSITY(CM-3)                              */
/*        D(5) - AR NUMBER DENSITY(CM-3)                              */
/*        D(6) - TOTAL MASS DENSITY(GM/CM3)                           */
/*        D(7) - H NUMBER DENSITY(CM-3)                               */
/*        D(8) - N NUMBER DENSITY(CM-3)                               */
/*        T(1) - EXOSPHERIC TEMPERATURE                               */
/*        T(2) - TEMPERATURE AT ALT                                   */

/*      TO GET OUTPUT IN M-3 and KG/M3:   CALL METERS(.TRUE.)         */

/*          ADDITIONAL COMMENTS                                       */
/*           (1) LOWER BOUND QUANTITIES IN COMMON/GTS3C/              */
/*           (2) TO TURN ON AND OFF PARTICULAR VARIATIONS             */
/*               CALL TSELEC(SW), WHERE SW IS A 25 ELEMENT ARRAY      */
/*               CONTAINING 0. FOR OFF, 1. FOR ON, OR 2. FOR MAIN     */
/*               EFFECTS OFF BUT CROSS TERMS ON FOR THE FOLLOWING     */
/*               VARIATIONS:                                          */
/*               1 - F10.7 EFFECT ON MEAN  2 - TIME INDEPENDENT       */
/*               3 - SYMMETRICAL ANNUAL    4 - SYMMETRICAL SEMIANNUAL */
/*               5 - ASYMMETRICAL ANNUAL   6 - ASYMMETRICAL SEMIANNUAL*/
/*               7 - DIURNAL               8 - SEMIDIURNAL            */
/*               9 - DAILY AP             10 - ALL UT/LONG EFFECTS    */
/*              11 - LONGITUDINAL         12 - UT AND MIXED UT/LONG   */
/*              13 - MIXED AP/UT/LONG     14 - TERDIURNAL             */
/*              15 - DEPARTURES FROM DIFFUSIVE EQUILIBRIUM            */
/*              16 - ALL TINF VAR         17 - ALL TLB VAR            */
/*              18 - ALL T0 VAR           19 - ALL S VAR              */
/*              20 - ALL Z0 VAR           21 - ALL NLB VAR            */
/*              22 - ALL TR12 VAR         23 - TURBO SCALE HEIGHT VAR */

/*              To get current values of SW: CALL TRETRV(SW)          */
/*====================================================================*/

double MSIS86(long Year, long DOY, long Hour, long Minute,
              double Second, double pbn[3],
              double PriMerAng, double F107, double AP);


/*
** #ifdef __cplusplus
** }
** #endif
*/

#endif

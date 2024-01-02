/* This file provided by John Downing                                 */
/* Minor formatting tweaks by Eric Stoneking                          */
/* Driver functions for 42 interface added by Eric Stoneking          */

/*  -- translated by f2c (version 20030306).
   You must link the resulting object file with the libraries:
   -lf2c -lm   (in that order)  [Not so for this installation -ETS]
*/

#define EXTERN extern
#include "msis86kit.h"
#undef EXTERN

/* #ifdef __cplusplus
** namespace Kit {
** #endif
*/

union ttest_86Type {
    struct {
   real tinfg, gb, rout, tt[15];
    } _1;
    struct {
   real tinf, gb, rout, t[15];
    } _2;
};
union ttest_86Type ttest_86;

#define ttest_1 (ttest_86._1)
#define ttest_2 (ttest_86._2)

/* Table of constant values */

static real c_b8 = 28.f;
static real c_b9 = 0.f;
static real c_b10 = -1.f;
static real c_b19 = 4.f;
static real c_b20 = -.4f;
static real c_b22 = -1.4f;
static real c_b25 = 16.f;
static real c_b31 = 32.f;
static real c_b37 = 40.f;
static real c_b43 = 1.f;
static real c_b49 = 14.f;
static doublereal c_b71 = .5;
static real c_b79 = 2.f;

/**********************************************************************/
/* Subroutine */ int gts5_0_(int n__, integer *iyd, real *sec, real *alt,
   real *glat, real *glong, real *stl, real *f107a, real *f107, real *ap,
    integer *mass, real *d__, real *t, logical *meter)
{
    /* Initialized data */

    static integer mt[10] = { 48,0,4,16,28,32,40,1,49,14 };
    static integer ifl = 0;
    static real altl[8] = { 200.f,400.f,150.f,200.f,240.f,450.f,320.f,450.f };
    static integer imr = 0;

    /* System generated locals */
    real r__1;

    /* Builtin functions */
    /* double exp(doublereal), log(doublereal); */

    /* Local variables */
    static integer i__, j;
    static real g1, g4, b01, b04, b32, b16, g40, b28, g32, g16, b40, g14, g28,
        b14, tz, hc04, hc32, hc16, hc40, dm04, dm32, dm16, dm40, dm01,
       dm28, rc16, hc01, zc04, zc16, zh04, tr12, xmd, zh16, zh28, yrd,
       zh32, xmm, zc32, zh40, zc40, zh01, zc01, rc01, zh14, dm14, hc14,
       zc14, rc14, hcc01, hcc14, hcc16, zcc01, zcc14;
    extern doublereal ccor_86(real *, real *, real *, real *);
    static real zcc16;
    extern doublereal dnet_86(real *, real *, real *, real *, real *);
    static real zhm01, tinf, zhm04, zhm32, zhm16, zhm40, zhm14, zhm28;
    extern doublereal denss_(real *, real *, real *, real *, real *, real *,
       real *, real *, real *, real *, real *, real *, real *), globe5_(
       real *, real *, real *, real *, real *, real *, real *, real *,
       real *), glob5l_(real *);
    extern /* Subroutine */ int prmsg5_(void);

/*        MSIS-86/CIRA 1986 Neutral Thermosphere Model */
/*         A.E.Hedin 3/15/85;2/26/87 (Variable Names Shortened) */
/*         10/14/87 Increase altitude limit of O mixing calculation */
/*             ALTL(2) FROM 300. TO 400. */
/*         8/10/88 CHANGE HANDLING OF SW(9) IN GLOBE */
/*     INPUT: */
/*        IYD - YEAR AND DAY AS YYDDD */
/*        SEC - UT(SEC) */
/*        ALT - ALTITUDE(KM) (GREATER THAN 85 KM) */
/*        GLAT - GEODETIC LATITUDE(DEG) */
/*        GLONG - GEODETIC LONGITUDE(DEG) */
/*        STL - LOCAL APPARENT SOLAR TIME(HRS) */
/*        F107A - 3 MONTH AVERAGE OF F10.7 FLUX */
/*        F107 - DAILY F10.7 FLUX FOR PREVIOUS DAY */
/*        AP - MAGNETIC INDEX(DAILY) OR WHEN SW(9)=-1. : */
/*           - ARRAY CONTAINING: */
/*             (1) DAILY AP */
/*             (2) 3 HR AP INDEX FOR CURRENT TIME */
/*             (3) 3 HR AP INDEX FOR 3 HRS BEFORE CURRENT TIME */
/*             (4) 3 HR AP INDEX FOR 6 HRS BEFORE CURRENT TIME */
/*             (5) 3 HR AP INDEX FOR 9 HRS BEFORE CURRENT TIME */
/*             (6) AVERAGE OF EIGHT 3 HR AP INDICIES FROM 12 TO 33 HRS PRIOR */
/*                    TO CURRENT TIME */
/*             (7) AVERAGE OF EIGHT 3 HR AP INDICIES FROM 36 TO 59 HRS PRIOR */
/*                    TO CURRENT TIME */
/*        MASS - MASS NUMBER (ONLY DENSITY FOR SELECTED GAS IS */
/*                 CALCULATED.  MASS 0 IS TEMPERATURE.  MASS 48 FOR ALL. */
/*     Note:  Ut, Local Time, and Longitude are used independently in the */
/*            model and are not of equal importance for every situation. */
/*            For the most physically realistic calculation these three */
/*            variables should be consistent (STL=SEC/3600+GLONG/15). */
/*     OUTPUT: */
/*        D(1) - HE NUMBER DENSITY(CM-3) */
/*        D(2) - O NUMBER DENSITY(CM-3) */
/*        D(3) - N2 NUMBER DENSITY(CM-3) */
/*        D(4) - O2 NUMBER DENSITY(CM-3) */
/*        D(5) - AR NUMBER DENSITY(CM-3) */
/*        D(6) - TOTAL MASS DENSITY(GM/CM3) */
/*        D(7) - H NUMBER DENSITY(CM-3) */
/*        D(8) - N NUMBER DENSITY(CM-3) */
/*        T(1) - EXOSPHERIC TEMPERATURE */
/*        T(2) - TEMPERATURE AT ALT */

/*      TO GET OUTPUT IN M-3 and KG/M3:   CALL METERS(.TRUE.) */

/*          ADDITIONAL COMMENTS */
/*           (1) LOWER BOUND QUANTITIES IN COMMON/GTS3C/ */
/*           (2) TO TURN ON AND OFF PARTICULAR VARIATIONS CALL TSELEC(SW) */
/*               WHERE SW IS A 25 ELEMENT ARRAY CONTAINING 0. FOR OFF, 1. */
/*               FOR ON, OR 2. FOR MAIN EFFECTS OFF BUT CROSS TERMS ON */
/*               FOR THE FOLLOWING VARIATIONS */
/*               1 - F10.7 EFFECT ON MEAN  2 - TIME INDEPENDENT */
/*               3 - SYMMETRICAL ANNUAL    4 - SYMMETRICAL SEMIANNUAL */
/*               5 - ASYMMETRICAL ANNUAL   6 - ASYMMETRICAL SEMIANNUAL */
/*               7 - DIURNAL               8 - SEMIDIURNAL */
/*               9 - DAILY AP             10 - ALL UT/LONG EFFECTS */
/*              11 - LONGITUDINAL         12 - UT AND MIXED UT/LONG */
/*              13 - MIXED AP/UT/LONG     14 - TERDIURNAL */
/*              15 - DEPARTURES FROM DIFFUSIVE EQUILIBRIUM */
/*              16 - ALL TINF VAR         17 - ALL TLB VAR */
/*              18 - ALL T0 VAR           19 - ALL S VAR */
/*              20 - ALL Z0 VAR           21 - ALL NLB VAR */
/*              22 - ALL TR12 VAR         23 - TURBO SCALE HEIGHT VAR */

/*              To get current values of SW: CALL TRETRV(SW) */

/* XX      COMMON/DATIME/ISDATE(3),ISTIME(2),NAME(2) */
    /* Parameter adjustments */
    if (ap) {
   --ap;
   }
    if (d__) {
   --d__;
   }
    if (t) {
   --t;
   }

    /* Function Body */
    switch(n__) {
   case 1: goto L_meters;
   }

    if (ifl == 1) {
   goto L5;
    }
    prmsg5_();
    ifl = 1;
L5:
    yrd = (real) (*iyd);
/*       Eq. A7 */
    tinf = lower5_1.ptm[0] * (csw_1.sw[15] * globe5_(&yrd, sec, glat, glong,
       stl, f107a, f107, &ap[1], parm5_1.pt) + 1.f) * parm5_1.pt[0];
    gts3c_1.za = lower5_1.ptm[4] * parm5_1.pdl[40];
/*       Eq. A9 */
    gts3c_1.t0 = lower5_1.ptm[2] * parm5_1.pd[375] * (csw_1.sw[17] * glob5l_(&
       parm5_1.pd[375]) + 1.f);
/*       Eq. A8 */
    gts3c_1.tlb = lower5_1.ptm[1] * (csw_1.sw[16] * glob5l_(&parm5_1.pd[325])
       + 1.f) * parm5_1.pd[325];
/*       Eq. A10 */
    gts3c_1.z0 = lower5_1.ptm[6] * (csw_1.sw[19] * glob5l_(&parm5_1.pd[350])
       + 1.f) * parm5_1.pd[350];
/*       Eq. A6 */
    gts3c_1.g0 = lower5_1.ptm[3] * parm5_1.ps[0] * (csw_1.sw[18] * globe5_(&
       yrd, sec, glat, glong, stl, f107a, f107, &ap[1], parm5_1.ps) +
       1.f);
/*       Eq. A5 */
    gts3c_1.s = gts3c_1.g0 / (tinf - gts3c_1.tlb);
/*       Eq. A11 */
    tr12 = parm5_1.pd[400] * (csw_1.sw[21] * glob5l_(&parm5_1.pd[400]) + 1.f);
    t[1] = tinf;
    if (*mass == 0) {
   goto L50;
    }
/*       Eq. A18  N2 */
    g28 = csw_1.sw[20] * glob5l_(&parm5_1.pd[300]);
    yrd = (real) (*iyd);
    t[1] = tinf;
    xmm = lower5_1.pdm[20];
    for (j = 1; j <= 10; ++j) {
   if (*mass == mt[j - 1]) {
       goto L15;
   }
/* L10: */
    }
/* XX      WRITE(6,100) MASS */
    goto L90;
L15:
    if (*alt > altl[5] && *mass != 28 && *mass != 48) {
   goto L17;
    }

/*       **** N2 DENSITY **** */

/*       Eq. A18 */
    gts3c_1.db28 = lower5_1.pdm[16] * exp(g28) * parm5_1.pd[300];
/*       Eq. A13 - A17 */
    d__[3] = denss_(alt, &gts3c_1.db28, &tinf, &gts3c_1.tlb, &c_b8, &c_b9, &t[
       2], &lower5_1.ptm[5], &gts3c_1.s, &gts3c_1.t0, &gts3c_1.za, &
       gts3c_1.z0, &tr12);
    gts3c_1.dd = d__[3];
/*       Eq. A19 */
    zh28 = lower5_1.pdm[18];
    zhm28 = lower5_1.pdm[19] * parm5_1.pdl[30];
    xmd = 28.f - xmm;
    b28 = denss_(&zh28, &gts3c_1.db28, &tinf, &gts3c_1.tlb, &xmd, &c_b10, &tz,
        &lower5_1.ptm[5], &gts3c_1.s, &gts3c_1.t0, &gts3c_1.za, &
       gts3c_1.z0, &tr12);
    if (*alt > altl[2] || csw_1.sw[14] == 0.f) {
   goto L17;
    }
    dm28 = denss_(alt, &b28, &tinf, &gts3c_1.tlb, &xmm, &c_b9, &tz, &
       lower5_1.ptm[5], &gts3c_1.s, &gts3c_1.t0, &gts3c_1.za, &
       gts3c_1.z0, &tr12);
/*       Eq. A12 */
    d__[3] = dnet_86(&d__[3], &dm28, &zhm28, &xmm, &c_b8);
L17:
    switch (j) {
   case 1:  goto L20;
   case 2:  goto L50;
   case 3:  goto L20;
   case 4:  goto L25;
   case 5:  goto L90;
   case 6:  goto L35;
   case 7:  goto L40;
   case 8:  goto L45;
   case 9:  goto L25;
   case 10:  goto L48;
    }
L20:

/*       **** HE DENSITY **** */

/*       Eq. A18 */
    g4 = csw_1.sw[20] * globe5_(&yrd, sec, glat, glong, stl, f107a, f107, &ap[
       1], parm5_1.pd);
    gts3c_1.db04 = lower5_1.pdm[0] * exp(g4) * parm5_1.pd[0];
/*       Eq. A13 - A17 */
    d__[1] = denss_(alt, &gts3c_1.db04, &tinf, &gts3c_1.tlb, &c_b19, &c_b20, &
       t[2], &lower5_1.ptm[5], &gts3c_1.s, &gts3c_1.t0, &gts3c_1.za, &
       gts3c_1.z0, &tr12);
    gts3c_1.dd = d__[1];
    if (*alt > altl[0] || csw_1.sw[14] == 0.f) {
   goto L24;
    }
/*       Eq. A19 */
    zh04 = lower5_1.pdm[2];
    r__1 = 4.f - xmm;
    b04 = denss_(&zh04, &gts3c_1.db04, &tinf, &gts3c_1.tlb, &r__1, &c_b22, &t[
       2], &lower5_1.ptm[5], &gts3c_1.s, &gts3c_1.t0, &gts3c_1.za, &
       gts3c_1.z0, &tr12);
    dm04 = denss_(alt, &b04, &tinf, &gts3c_1.tlb, &xmm, &c_b9, &t[2], &
       lower5_1.ptm[5], &gts3c_1.s, &gts3c_1.t0, &gts3c_1.za, &
       gts3c_1.z0, &tr12);
/*       Eq. A12 */
    zhm04 = zhm28;
    d__[1] = dnet_86(&d__[1], &dm04, &zhm04, &xmm, &c_b19);
/*       Eq. A20b */
    gts3c_1.rl = log(b28 * lower5_1.pdm[1] / b04);
/*       Eq. A20a */
    zc04 = lower5_1.pdm[4] * parm5_1.pdl[25];
    hc04 = lower5_1.pdm[5] * parm5_1.pdl[26];
    d__[1] *= ccor_86(alt, &gts3c_1.rl, &hc04, &zc04);
L24:
    if (*mass != 48) {
   goto L90;
    }
L25:

/*      **** O DENSITY **** */

/*       Eq. A18 */
    g16 = csw_1.sw[20] * globe5_(&yrd, sec, glat, glong, stl, f107a, f107, &
       ap[1], &parm5_1.pd[150]);
    gts3c_1.db16 = lower5_1.pdm[8] * exp(g16) * parm5_1.pd[150];
/*       Eq. A13 - A17 */
    d__[2] = denss_(alt, &gts3c_1.db16, &tinf, &gts3c_1.tlb, &c_b25, &c_b9, &
       t[2], &lower5_1.ptm[5], &gts3c_1.s, &gts3c_1.t0, &gts3c_1.za, &
       gts3c_1.z0, &tr12);
    gts3c_1.dd = d__[2];
    if (*alt > altl[1] || csw_1.sw[14] == 0.f) {
   goto L34;
    }
/*  Corrected from PDM(3,1) to PDM(3,2)  12/2/85 */
/*       Eq. A19 */
    zh16 = lower5_1.pdm[10];
    r__1 = 16 - xmm;
    b16 = denss_(&zh16, &gts3c_1.db16, &tinf, &gts3c_1.tlb, &r__1, &c_b10, &t[
       2], &lower5_1.ptm[5], &gts3c_1.s, &gts3c_1.t0, &gts3c_1.za, &
       gts3c_1.z0, &tr12);
    dm16 = denss_(alt, &b16, &tinf, &gts3c_1.tlb, &xmm, &c_b9, &t[2], &
       lower5_1.ptm[5], &gts3c_1.s, &gts3c_1.t0, &gts3c_1.za, &
       gts3c_1.z0, &tr12);
/*       Eq. A12 */
    zhm16 = zhm28;
    d__[2] = dnet_86(&d__[2], &dm16, &zhm16, &xmm, &c_b25);
/*       Eq. A20b */
    gts3c_1.rl = log(b28 * lower5_1.pdm[9] * dabs(parm5_1.pdl[41]) / b16);
/*       Eq. A20a */
    hc16 = lower5_1.pdm[13] * parm5_1.pdl[28];
    zc16 = lower5_1.pdm[12] * parm5_1.pdl[27];
    d__[2] *= ccor_86(alt, &gts3c_1.rl, &hc16, &zc16);
/*       Eq. A21 */
    hcc16 = lower5_1.pdm[15] * parm5_1.pdl[38];
    zcc16 = lower5_1.pdm[14] * parm5_1.pdl[37];
    rc16 = lower5_1.pdm[11] * parm5_1.pdl[39];
    d__[2] *= ccor_86(alt, &rc16, &hcc16, &zcc16);
L34:
    if (*mass != 48 && *mass != 49) {
   goto L90;
    }
L35:

/*       **** O2 DENSITY **** */

/*       Eq. A18 */
    g32 = csw_1.sw[20] * globe5_(&yrd, sec, glat, glong, stl, f107a, f107, &
       ap[1], &parm5_1.pd[450]);
    gts3c_1.db32 = lower5_1.pdm[24] * exp(g32) * parm5_1.pd[450];
/*       Eq. A13 - A17 */
    d__[4] = denss_(alt, &gts3c_1.db32, &tinf, &gts3c_1.tlb, &c_b31, &c_b9, &
       t[2], &lower5_1.ptm[5], &gts3c_1.s, &gts3c_1.t0, &gts3c_1.za, &
       gts3c_1.z0, &tr12);
    if (*mass == 49) {
   gts3c_1.dd += d__[4] * 2.f;
    } else {
   gts3c_1.dd = d__[4];
    }
    if (*alt > altl[3] || csw_1.sw[14] == 0.f) {
   goto L39;
    }
/*       Eq. A19 */
    zh32 = lower5_1.pdm[26];
    r__1 = 32.f - xmm;
    b32 = denss_(&zh32, &gts3c_1.db32, &tinf, &gts3c_1.tlb, &r__1, &c_b10, &t[
       2], &lower5_1.ptm[5], &gts3c_1.s, &gts3c_1.t0, &gts3c_1.za, &
       gts3c_1.z0, &tr12);
    dm32 = denss_(alt, &b32, &tinf, &gts3c_1.tlb, &xmm, &c_b9, &t[2], &
       lower5_1.ptm[5], &gts3c_1.s, &gts3c_1.t0, &gts3c_1.za, &
       gts3c_1.z0, &tr12);
/*       Eq. A12 */
    zhm32 = zhm28;
    d__[4] = dnet_86(&d__[4], &dm32, &zhm32, &xmm, &c_b31);
/*       Eq. A20b */
    gts3c_1.rl = log(b28 * lower5_1.pdm[25] / b32);
/*       Eq. A20a */
    hc32 = lower5_1.pdm[29] * parm5_1.pdl[32];
    zc32 = lower5_1.pdm[28] * parm5_1.pdl[31];
    d__[4] *= ccor_86(alt, &gts3c_1.rl, &hc32, &zc32);
L39:
    if (*mass != 48) {
   goto L90;
    }
L40:

/*       **** AR DENSITY **** */

/*       Eq. A18 */
    g40 = csw_1.sw[20] * globe5_(&yrd, sec, glat, glong, stl, f107a, f107, &
       ap[1], &parm5_1.pd[600]);
    gts3c_1.db40 = lower5_1.pdm[32] * exp(g40) * parm5_1.pd[600];
/*       Eq. A13 - A17 */
    d__[5] = denss_(alt, &gts3c_1.db40, &tinf, &gts3c_1.tlb, &c_b37, &c_b9, &
       t[2], &lower5_1.ptm[5], &gts3c_1.s, &gts3c_1.t0, &gts3c_1.za, &
       gts3c_1.z0, &tr12);
    gts3c_1.dd = d__[5];
    if (*alt > altl[4] || csw_1.sw[14] == 0.f) {
   goto L44;
    }
/*       Eq. A19 */
    zh40 = lower5_1.pdm[34];
    r__1 = 40.f - xmm;
    b40 = denss_(&zh40, &gts3c_1.db40, &tinf, &gts3c_1.tlb, &r__1, &c_b10, &t[
       2], &lower5_1.ptm[5], &gts3c_1.s, &gts3c_1.t0, &gts3c_1.za, &
       gts3c_1.z0, &tr12);
    dm40 = denss_(alt, &b40, &tinf, &gts3c_1.tlb, &xmm, &c_b9, &t[2], &
       lower5_1.ptm[5], &gts3c_1.s, &gts3c_1.t0, &gts3c_1.za, &
       gts3c_1.z0, &tr12);
/*       Eq. A12 */
    zhm40 = zhm28;
    d__[5] = dnet_86(&d__[5], &dm40, &zhm40, &xmm, &c_b37);
/*       Eq. A20b */
    gts3c_1.rl = log(b28 * lower5_1.pdm[33] / b40);
/*       Eq. A20a */
    hc40 = lower5_1.pdm[37] * parm5_1.pdl[34];
    zc40 = lower5_1.pdm[36] * parm5_1.pdl[33];
    d__[5] *= ccor_86(alt, &gts3c_1.rl, &hc40, &zc40);
L44:
    if (*mass != 48) {
   goto L90;
    }
L45:

/*        **** HYDROGEN DENSITY **** */

/*       Eq. A18 */
    g1 = csw_1.sw[20] * globe5_(&yrd, sec, glat, glong, stl, f107a, f107, &ap[
       1], &parm5_1.pd[750]);
    gts3c_1.db01 = lower5_1.pdm[40] * exp(g1) * parm5_1.pd[750];
/*       Eq. A13 - A17 */
    d__[7] = denss_(alt, &gts3c_1.db01, &tinf, &gts3c_1.tlb, &c_b43, &c_b20, &
       t[2], &lower5_1.ptm[5], &gts3c_1.s, &gts3c_1.t0, &gts3c_1.za, &
       gts3c_1.z0, &tr12);
    gts3c_1.dd = d__[7];
    if (*alt > altl[6] || csw_1.sw[14] == 0.f) {
   goto L47;
    }
/*       Eq. A19 */
    zh01 = lower5_1.pdm[42];
    r__1 = 1.f - xmm;
    b01 = denss_(&zh01, &gts3c_1.db01, &tinf, &gts3c_1.tlb, &r__1, &c_b22, &t[
       2], &lower5_1.ptm[5], &gts3c_1.s, &gts3c_1.t0, &gts3c_1.za, &
       gts3c_1.z0, &tr12);
    dm01 = denss_(alt, &b01, &tinf, &gts3c_1.tlb, &xmm, &c_b9, &t[2], &
       lower5_1.ptm[5], &gts3c_1.s, &gts3c_1.t0, &gts3c_1.za, &
       gts3c_1.z0, &tr12);
/*       Eq. A12 */
    zhm01 = zhm28;
    d__[7] = dnet_86(&d__[7], &dm01, &zhm01, &xmm, &c_b43);
/*       Eq. A20b */
    gts3c_1.rl = log(b28 * lower5_1.pdm[41] * dabs(parm5_1.pdl[42]) / b01);
/*       Eq. A20a */
    hc01 = lower5_1.pdm[45] * parm5_1.pdl[36];
    zc01 = lower5_1.pdm[44] * parm5_1.pdl[35];
    d__[7] *= ccor_86(alt, &gts3c_1.rl, &hc01, &zc01);
/*       Eq. A21 */
    hcc01 = lower5_1.pdm[47] * parm5_1.pdl[44];
    zcc01 = lower5_1.pdm[46] * parm5_1.pdl[43];
    rc01 = lower5_1.pdm[43] * parm5_1.pdl[45];
    d__[7] *= ccor_86(alt, &rc01, &hcc01, &zcc01);
L47:
L48:

/*        **** ATOMIC NITROGEN DENSITY **** */

/*       Eq. A18 */
    g14 = csw_1.sw[20] * globe5_(&yrd, sec, glat, glong, stl, f107a, f107, &
       ap[1], &parm5_1.pd[900]);
    gts3c_1.db14 = lower5_1.pdm[48] * exp(g14) * parm5_1.pd[900];
/*       Eq. A13 - A17 */
    d__[8] = denss_(alt, &gts3c_1.db14, &tinf, &gts3c_1.tlb, &c_b49, &c_b9, &
       t[2], &lower5_1.ptm[5], &gts3c_1.s, &gts3c_1.t0, &gts3c_1.za, &
       gts3c_1.z0, &tr12);
    gts3c_1.dd = d__[8];
    if (*alt > altl[7] || csw_1.sw[14] == 0.f) {
   goto L49;
    }
/*       Eq. A19 */
    zh14 = lower5_1.pdm[50];
    r__1 = 14.f - xmm;
    b14 = denss_(&zh14, &gts3c_1.db14, &tinf, &gts3c_1.tlb, &r__1, &c_b10, &t[
       2], &lower5_1.ptm[5], &gts3c_1.s, &gts3c_1.t0, &gts3c_1.za, &
       gts3c_1.z0, &tr12);
    dm14 = denss_(alt, &b14, &tinf, &gts3c_1.tlb, &xmm, &c_b9, &t[2], &
       lower5_1.ptm[5], &gts3c_1.s, &gts3c_1.t0, &gts3c_1.za, &
       gts3c_1.z0, &tr12);
/*       Eq. A12 */
    zhm14 = zhm28;
    d__[8] = dnet_86(&d__[8], &dm14, &zhm14, &xmm, &c_b49);
/*       Eq. A20b */
    gts3c_1.rl = log(b28 * lower5_1.pdm[49] * dabs(parm5_1.pdl[2]) / b14);
/*       Eq. A20a */
    hc14 = lower5_1.pdm[53] * parm5_1.pdl[1];
    zc14 = lower5_1.pdm[52] * parm5_1.pdl[0];
    d__[8] *= ccor_86(alt, &gts3c_1.rl, &hc14, &zc14);
/*       Eq. A21 */
    hcc14 = lower5_1.pdm[55] * parm5_1.pdl[4];
    zcc14 = lower5_1.pdm[54] * parm5_1.pdl[3];
    rc14 = lower5_1.pdm[51] * parm5_1.pdl[5];
    d__[8] *= ccor_86(alt, &rc14, &hcc14, &zcc14);
L49:
    if (*mass != 48) {
   goto L90;
    }

/*       TOTAL MASS DENSITY */

    d__[6] = (d__[1] * 4.f + d__[2] * 16.f + d__[3] * 28.f + d__[4] * 32.f +
       d__[5] * 40.f + d__[7] + d__[8] * 14.f) * 1.66e-24f;
    gts3c_1.db48 = (gts3c_1.db04 * 4.f + gts3c_1.db16 * 16.f + gts3c_1.db28 *
       28.f + gts3c_1.db32 * 32.f + gts3c_1.db40 * 40.f + gts3c_1.db01 +
       gts3c_1.db14 * 14.f) * 1.66e-24f;
    goto L90;
L50:
    denss_(alt, &c_b43, &tinf, &gts3c_1.tlb, &c_b9, &c_b9, &t[2], &
       lower5_1.ptm[5], &gts3c_1.s, &gts3c_1.t0, &gts3c_1.za, &
       gts3c_1.z0, &tr12);
    goto L90;
L90:
    if (imr == 1) {
   for (i__ = 1; i__ <= 8; ++i__) {
       d__[i__] *= 1e6f;
/* L95: */
   }
   d__[6] /= 1e3f;
    }
    return 0;
/* L100: */

L_meters:
    imr = 0;
    if (*meter) {
   imr = 1;
    }
    return 0;
} /* gts5_ */

/**********************************************************************/
/* Subroutine */ int gts5_(integer *iyd, real *sec, real *alt, real *glat,
   real *glong, real *stl, real *f107a, real *f107, real *ap, integer *
   mass, real *d__, real *t)
{
    return gts5_0_(0, iyd, sec, alt, glat, glong, stl, f107a, f107, ap, mass,
       d__, t, (logical *)0);
    }

/**********************************************************************/
/* Subroutine */ int meters_86(logical *meter)
{
    return gts5_0_(1, (integer *)0, (real *)0, (real *)0, (real *)0, (real *)
       0, (real *)0, (real *)0, (real *)0, (real *)0, (integer *)0, (
       real *)0, (real *)0, meter);
    }

/**********************************************************************/
doublereal denss_(real *alt, real *dlb, real *tinf, real *tlb, real *xm, real
   *alpha, real *tz, real *zlb, real *s2, real *t0, real *za, real *z0,
   real *tr12)
{
    /* Initialized data */

    static real rgas = 831.4f;

    /* System generated locals */
    real ret_val, r__1;
    doublereal d__1, d__2;

    /* Builtin functions */
    /* double exp(doublereal), pow_dd(doublereal *, doublereal *); */

    /* Local variables */
    static real x, z__, x2, bb, cc, dd, ta, t12, tt, zg0, zg1, zg2, glb, dta,
       gamm, gamma, densa;

/*       Calculate Temperature and Density Profiles for MSIS models */
    ret_val = 1.f;
    z__ = (*alt >= *za ? *alt : *za);
/*      Eq. A4a */
    zg2 = (z__ - *zlb) * (parmb_1.re + *zlb) / (parmb_1.re + z__);
/*      Eq. A1a */
    tt = *tinf - (*tinf - *tlb) * exp(-(*s2) * zg2);
    ta = tt;
    *tz = tt;
    ret_val = *tz;
    if (*alt >= *za) {
   goto L10;
    }
/*      Eq. A4b */
    zg0 = (*z0 - *za) * (parmb_1.re + *za) / (parmb_1.re + *z0);
/*      Eq. A2b */
/* Computing 2nd power */
    r__1 = (parmb_1.re + *zlb) / (parmb_1.re + *za);
    dta = (*tinf - ta) * *s2 * (r__1 * r__1);
/*      Eq. A3e */
    t12 = *t0 + *tr12 * (ta - *t0);
/*      Eq. A4b */
    zg1 = (*alt - *za) * (parmb_1.re + *za) / (parmb_1.re + *alt);
/*       CALCULATE TEMPERATURE BELOW ZA */
/*      Eq. A3a */
/* Computing 2nd power */
    r__1 = ta;
    dd = zg0 * .666666f * dta / (r__1 * r__1) - (1.f / ta - 1.f / *t0) *
       3.11111f + (1.f / t12 - 1.f / *t0) * 7.11111f;
/*      Eq. A3b */
    cc = zg0 * dta / (ta * 2.f * ta) - (1.f / ta - 1.f / *t0) - dd * 2.f;
/*      Eq. A3c */
    bb = 1.f / ta - 1.f / *t0 - cc - dd;
/*      Eq. A3d */
    x = -(zg1 - zg0) / zg0;
/*      Eq. A1b */
    x2 = x * x;
    *tz = 1.f / (1.f / *t0 + bb * x2 + cc * x2 * x2 + dd * x2 * x2 * x2);
    ret_val = *tz;
    fit_1.taf = (t12 - *t0) / (ta - *t0);
L10:
    if (*xm == 0.f) {
   goto L50;
    }
    if (ta > 0.f && *tz > 0.f) {
   goto L20;
    }
/* XX         WRITE(6,*)ALT,XM,TINF,TLB,T0,TA,II,JG,N,DV(J),IFUN,S2,ZG0,TZ */
    tt = *tlb;
    ta = *tlb;
    *tz = *tlb;
L20:
/*      CALCULATE DENSITY ABOVE ZA */
/*      Eq. A17a */
/* Computing 2nd power */
    r__1 = *zlb / parmb_1.re + 1.f;
    glb = parmb_1.gsurf / (r__1 * r__1);
/*      Eq. A16a */
    gamma = *xm * glb / (*s2 * rgas * *tinf);
/*      Eq. A13, A14a, & A15 */
    d__1 = (doublereal) (*tlb / tt);
    d__2 = (doublereal) (*alpha + 1.f + gamma);
    densa = *dlb * pow(d__1, d__2) * exp(-(*s2) * gamma * zg2);
    ret_val = densa;
    if (*alt >= *za) {
   goto L50;
    }
/*      CALCULATE DENSITY BELOW ZA */
/*      Eq. A17b */
/* Computing 2nd power */
    r__1 = *za / parmb_1.re + 1.f;
    glb = parmb_1.gsurf / (r__1 * r__1);
/*      Eq. A16b */
    gamm = *xm * glb * zg0 / rgas;
/*      Eq. A13, A14b, & A15 */
    d__1 = (doublereal) (ta / *tz);
    d__2 = (doublereal) (*alpha + 1.f);
    ret_val = densa * pow(d__1, d__2) * exp(gamm * ((x - 1) / *t0 + bb *
       (x * x2 - 1.f) / 3.f + cc * (x2 * x2 * x - 1.f) / 5.f + dd * (x2 *
        x2 * x2 * x - 1.f) / 7.f));
L50:
/* CCCCCWRITE(6,100)CXM,ALT,ZA,TINF,TLB,S2,T0,S1,TA,TZ,DLB,DENSA,DENSS */
/* C100 FORMAT(' D',1P13E10.2) */
    return ret_val;
} /* denss_ */

/**********************************************************************/
doublereal globe5_(real *yrd, real *sec, real *lat, real *long__, real *tloc,
   real *f107a, real *f107, real *ap, real *p)
{
    /* Initialized data */

    static integer nsw = 14;
    static real p39 = -1e3f;
    static real dgtr = .0174533f;
    static real dr = .0172142f;
    static real xl = 1e3f;
    static real tll = 1e3f;
    static real sw9 = 1.f;
    static real dayl = -1.f;
    static real p14 = -1e3f;
    static real p18 = -1e3f;
    static real p32 = -1e3f;
    static real hr = .2618f;
    static real sr = 7.2722e-5f;
    static real sv[25] = { 1.f,1.f,1.f,1.f,1.f,1.f,1.f,1.f,1.f,1.f,1.f,1.f,
       1.f,1.f,1.f,1.f,1.f,1.f,1.f,1.f,1.f,1.f,1.f,1.f,1.f };

    /* System generated locals */
    integer i__1;
    real ret_val, r__1, r__2, r__3, r__4, r__5, r__6;
    doublereal d__1;

    /* Builtin functions */
    /* double sin(doublereal), cos(doublereal), exp(doublereal), pow_dd(
          doublereal *, doublereal *); */

    /* Local variables */
    static real c__;
    static integer i__;
    static real s, c2, c4, f1, f2, s2, p44, p45, t71, t72, t81, t82, cd14,
       cd32, cd18, cd39, c2d14, exp1, exp2;
    /* Subroutine */ int tselec_86(real *);

/*       CALCULATE G(L) FUNCTION FOR MSIS-86/CIRA 1986 */
/*       Upper Thermosphere Parameters */
    /* Parameter adjustments */
    --p;
    --ap;

    /* Function Body */
/*      Eq. A24d */
/*       Eq. A24c */
/*       Eq. A24a */
    if (csw_1.isw != 64999) {
   tselec_86(sv);
    }
    ttest_2.t[8] = 0.f;
    ttest_2.t[9] = 0.f;
    ttest_2.t[10] = 0.f;
    ttest_2.t[11] = 0.f;
    ttest_2.t[12] = 0.f;
    if (csw_1.sw[8] > 0.f) {
   sw9 = 1.f;
    }
    if (csw_1.sw[8] < 0.f) {
   sw9 = -1.f;
    }
/* L10: */
    lpoly_1.iyr = (int) (*yrd / 1e3f);
    lpoly_1.day = *yrd - lpoly_1.iyr * 1e3f;
/*      Eq. A22 (remainder of code) */
    if (xl == *lat) {
   goto L15;
    }
/*          CALCULATE LEGENDRE POLYNOMIALS */
    c__ = sin(*lat * dgtr);
    s = cos(*lat * dgtr);
    c2 = c__ * c__;
    c4 = c2 * c2;
    s2 = s * s;
    lpoly_1.plg[1] = c__;
    lpoly_1.plg[2] = (c2 * 3.f - 1.f) * .5f;
    lpoly_1.plg[3] = (c__ * 5.f * c2 - c__ * 3.f) * .5f;
    lpoly_1.plg[4] = (c4 * 35.f - c2 * 30.f + 3.f) / 8.f;
    lpoly_1.plg[5] = (c2 * 63.f * c2 * c__ - c2 * 70.f * c__ + c__ * 15.f) /
       8.f;
    lpoly_1.plg[6] = (c__ * 11.f * lpoly_1.plg[5] - lpoly_1.plg[4] * 5.f) /
       6.f;
    lpoly_1.plg[10] = s;
    lpoly_1.plg[11] = c__ * 3.f * s;
    lpoly_1.plg[12] = (c2 * 5.f - 1.f) * 1.5f * s;
    lpoly_1.plg[13] = (c2 * 7.f * c__ - c__ * 3.f) * 2.5f * s;
    lpoly_1.plg[14] = (c4 * 21.f - c2 * 14.f + 1.f) * 1.875f * s;
    lpoly_1.plg[15] = (c__ * 11.f * lpoly_1.plg[14] - lpoly_1.plg[13] * 6.f) /
        5.f;
    lpoly_1.plg[20] = s2 * 3.f;
    lpoly_1.plg[21] = s2 * 15.f * c__;
    lpoly_1.plg[22] = (c2 * 7.f - 1.f) * 7.5f * s2;
    lpoly_1.plg[23] = c__ * 3.f * lpoly_1.plg[22] - lpoly_1.plg[21] * 2.f;
    lpoly_1.plg[24] = (c__ * 11.f * lpoly_1.plg[23] - lpoly_1.plg[22] * 7.f) /
        4.f;
    lpoly_1.plg[25] = (c__ * 13.f * lpoly_1.plg[24] - lpoly_1.plg[23] * 8.f) /
        5.f;
    lpoly_1.plg[30] = s2 * 15.f * s;
    lpoly_1.plg[31] = s2 * 105.f * s * c__;
    lpoly_1.plg[32] = (c__ * 9.f * lpoly_1.plg[31] - lpoly_1.plg[30] * 7.f) /
       2.f;
    lpoly_1.plg[33] = (c__ * 11.f * lpoly_1.plg[32] - lpoly_1.plg[31] * 8.f) /
        3.f;
    xl = *lat;
L15:
    if (tll == *tloc) {
   goto L16;
    }
    lpoly_1.stloc = sin(hr * *tloc);
    lpoly_1.ctloc = cos(hr * *tloc);
    lpoly_1.s2tloc = sin(hr * 2.f * *tloc);
    lpoly_1.c2tloc = cos(hr * 2.f * *tloc);
    lpoly_1.s3tloc = sin(hr * 3.f * *tloc);
    lpoly_1.c3tloc = cos(hr * 3.f * *tloc);
    tll = *tloc;
L16:
    if (lpoly_1.day != dayl || p[14] != p14) {
   cd14 = cos(dr * (lpoly_1.day - p[14]));
    }
    if (lpoly_1.day != dayl || p[14] != p14) {
   c2d14 = cos(dr * 2 * (lpoly_1.day - p[14]));
    }
    if (lpoly_1.day != dayl || p[18] != p18) {
   cd18 = cos(dr * 2.f * (lpoly_1.day - p[18]));
    }
    if (lpoly_1.day != dayl || p[32] != p32) {
   cd32 = cos(dr * (lpoly_1.day - p[32]));
    }
    if (lpoly_1.day != dayl || p[39] != p39) {
   cd39 = cos(dr * 2.f * (lpoly_1.day - p[39]));
    }
    dayl = lpoly_1.day;
    p14 = p[14];
    p18 = p[18];
    p32 = p[32];
    p39 = p[39];
/*         F10.7 EFFECT */
    lpoly_1.df = *f107 - *f107a;
    lpoly_1.dfa = *f107a - 150.f;
/* Computing 2nd power */
    r__1 = lpoly_1.dfa;
    ttest_2.t[0] = p[20] * lpoly_1.df + p[21] * lpoly_1.df * lpoly_1.df + p[
       22] * lpoly_1.dfa + p[30] * (r__1 * r__1);
    f1 = (p[48] * lpoly_1.dfa + p[20] * lpoly_1.df + p[21] * lpoly_1.df *
       lpoly_1.df) * csw_1.swc[0] + 1.f;
    f2 = (p[50] * lpoly_1.dfa + p[20] * lpoly_1.df + p[21] * lpoly_1.df *
       lpoly_1.df) * csw_1.swc[0] + 1.f;
/*        TIME INDEPENDENT */
    ttest_2.t[1] = p[2] * lpoly_1.plg[2] + p[3] * lpoly_1.plg[4] + p[23] *
       lpoly_1.plg[6] + p[15] * lpoly_1.plg[2] * lpoly_1.dfa * csw_1.swc[
       0] + p[27] * lpoly_1.plg[1];
/*        SYMMETRICAL ANNUAL */
    ttest_2.t[2] = p[19] * cd32;
/*        SYMMETRICAL SEMIANNUAL */
    ttest_2.t[3] = (p[16] + p[17] * lpoly_1.plg[2]) * cd18;
/*        ASYMMETRICAL ANNUAL */
    ttest_2.t[4] = f1 * (p[10] * lpoly_1.plg[1] + p[11] * lpoly_1.plg[3]) *
       cd14;
/*         ASYMMETRICAL SEMIANNUAL */
    ttest_2.t[5] = p[38] * lpoly_1.plg[1] * cd39;
/*        DIURNAL */
    t71 = (p[12] * lpoly_1.plg[11] + p[36] * lpoly_1.plg[10]) * cd14 *
       csw_1.swc[4];
    t72 = (p[13] * lpoly_1.plg[11] + p[37] * lpoly_1.plg[10]) * cd14 *
       csw_1.swc[4];
    ttest_2.t[6] = f2 * ((p[4] * lpoly_1.plg[10] + p[5] * lpoly_1.plg[12] + p[
       28] * lpoly_1.plg[14] + t71) * lpoly_1.ctloc + (p[7] *
       lpoly_1.plg[10] + p[8] * lpoly_1.plg[12] + p[29] * lpoly_1.plg[14]
        + t72) * lpoly_1.stloc);
/*        SEMIDIURNAL */
    t81 = p[24] * lpoly_1.plg[21] * cd14 * csw_1.swc[4];
    t82 = p[34] * lpoly_1.plg[21] * cd14 * csw_1.swc[4];
    ttest_2.t[7] = f2 * ((p[6] * lpoly_1.plg[20] + p[42] * lpoly_1.plg[22] +
       t81) * lpoly_1.c2tloc + (p[9] * lpoly_1.plg[20] + p[43] *
       lpoly_1.plg[22] + t82) * lpoly_1.s2tloc);
/*        TERDIURNAL */
    ttest_2.t[13] = f2 * ((p[40] * lpoly_1.plg[30] + (p[94] * lpoly_1.plg[31]
       + p[47] * lpoly_1.plg[33]) * cd14 * csw_1.swc[4]) *
       lpoly_1.s3tloc + (p[41] * lpoly_1.plg[30] + (p[95] * lpoly_1.plg[
       31] + p[49] * lpoly_1.plg[33]) * cd14 * csw_1.swc[4]) *
       lpoly_1.c3tloc);
/*          MAGNETIC ACTIVITY BASED ON DAILY AP */
    if (sw9 == -1.f) {
   goto L30;
    }
    lpoly_1.apd = ap[1] - 4.f;
    p44 = p[44];
    p45 = p[45];
    if (p44 < 0.f) {
   p44 = 1e-5f;
    }
    lpoly_1.apdf = lpoly_1.apd + (p45 - 1.f) * (lpoly_1.apd + (exp(-p44 *
       lpoly_1.apd) - 1.f) / p44);
    ttest_2.t[8] = lpoly_1.apdf * (p[33] + p[46] * lpoly_1.plg[2] + p[35] *
       lpoly_1.plg[4] + (p[101] * lpoly_1.plg[1] + p[102] * lpoly_1.plg[
       3] + p[103] * lpoly_1.plg[5]) * cd14 * csw_1.swc[4] + (p[122] *
       lpoly_1.plg[10] + p[123] * lpoly_1.plg[12] + p[124] * lpoly_1.plg[
       14]) * csw_1.swc[6] * cos(hr * (*tloc - p[125])));
    goto L40;
L30:
    if (p[52] == 0.f) {
   goto L40;
    }
    exp1 = exp(dabs(p[52]) * -10800.f / (p[139] * (45.f - dabs(*lat)) + 1.f));
    if (exp1 > .99999f) {
   exp1 = .99999f;
    }
    exp2 = exp(dabs(p[54]) * -10800.f);
    if (exp2 > .99999f) {
   exp2 = .99999f;
    }
    if (p[25] < 1e-4f) {
   p[25] = 1e-4f;
    }
/* Computing 3rd power */
    r__1 = exp1;
/* Computing 4th power */
    r__2 = exp1, r__2 *= r__2;
/* Computing 12th power */
    r__3 = exp1, r__3 *= r__3, r__3 *= r__3;
/* Computing 8th power */
    r__4 = exp1, r__4 *= r__4, r__4 *= r__4;
/* Computing 19th power */
    r__5 = exp1, r__6 = r__5, r__5 *= r__5, r__6 *= r__5, r__5 *= r__5, r__5
       *= r__5;
    d__1 = (doublereal) exp1;
    lpoly_1.apt[0] = (ap[2] - 4.f + (p[26] - 1.f) * (ap[2] - 4.f + (exp(-dabs(
       p[25]) * (ap[2] - 4.f)) - 1.f) / dabs(p[25])) + ((ap[3] - 4.f + (
       p[26] - 1.f) * (ap[3] - 4.f + (exp(-dabs(p[25]) * (ap[3] - 4.f))
       - 1.f) / dabs(p[25]))) * exp1 + (ap[4] - 4.f + (p[26] - 1.f) * (
       ap[4] - 4.f + (exp(-dabs(p[25]) * (ap[4] - 4.f)) - 1.f) / dabs(p[
       25]))) * exp1 * exp1 + (ap[5] - 4.f + (p[26] - 1.f) * (ap[5] -
       4.f + (exp(-dabs(p[25]) * (ap[5] - 4.f)) - 1.f) / dabs(p[25]))) *
       (r__1 * (r__1 * r__1)) + ((ap[6] - 4.f + (p[26] - 1.f) * (ap[6] -
       4.f + (exp(-dabs(p[25]) * (ap[6] - 4.f)) - 1.f) / dabs(p[25]))) *
       (r__2 * r__2) + (ap[7] - 4.f + (p[26] - 1.f) * (ap[7] - 4.f + (
       exp(-dabs(p[25]) * (ap[7] - 4.f)) - 1.f) / dabs(p[25]))) * (r__3 *
        (r__3 * r__3))) * (1.f - r__4 * r__4) / (1.f - exp1))) / (1.f + (
       1.f - r__6 * (r__5 * r__5)) / (1.f - exp1) * pow(d__1, c_b71)
       );
/* Computing 3rd power */
    r__1 = exp2;
/* Computing 4th power */
    r__2 = exp2, r__2 *= r__2;
/* Computing 12th power */
    r__3 = exp2, r__3 *= r__3, r__3 *= r__3;
/* Computing 8th power */
    r__4 = exp2, r__4 *= r__4, r__4 *= r__4;
/* Computing 19th power */
    r__5 = exp2, r__6 = r__5, r__5 *= r__5, r__6 *= r__5, r__5 *= r__5, r__5
       *= r__5;
    d__1 = (doublereal) exp2;
    lpoly_1.apt[2] = (ap[2] - 4.f + (p[26] - 1.f) * (ap[2] - 4.f + (exp(-dabs(
       p[25]) * (ap[2] - 4.f)) - 1.f) / dabs(p[25])) + ((ap[3] - 4.f + (
       p[26] - 1.f) * (ap[3] - 4.f + (exp(-dabs(p[25]) * (ap[3] - 4.f))
       - 1.f) / dabs(p[25]))) * exp2 + (ap[4] - 4.f + (p[26] - 1.f) * (
       ap[4] - 4.f + (exp(-dabs(p[25]) * (ap[4] - 4.f)) - 1.f) / dabs(p[
       25]))) * exp2 * exp2 + (ap[5] - 4.f + (p[26] - 1.f) * (ap[5] -
       4.f + (exp(-dabs(p[25]) * (ap[5] - 4.f)) - 1.f) / dabs(p[25]))) *
       (r__1 * (r__1 * r__1)) + ((ap[6] - 4.f + (p[26] - 1.f) * (ap[6] -
       4.f + (exp(-dabs(p[25]) * (ap[6] - 4.f)) - 1.f) / dabs(p[25]))) *
       (r__2 * r__2) + (ap[7] - 4.f + (p[26] - 1.f) * (ap[7] - 4.f + (
       exp(-dabs(p[25]) * (ap[7] - 4.f)) - 1.f) / dabs(p[25]))) * (r__3 *
        (r__3 * r__3))) * (1.f - r__4 * r__4) / (1.f - exp2))) / (1.f + (
       1.f - r__6 * (r__5 * r__5)) / (1.f - exp2) * pow(d__1, c_b71)
       );
    ttest_2.t[8] = lpoly_1.apt[0] * (p[51] + p[97] * lpoly_1.plg[2] + p[55] *
       lpoly_1.plg[4] + (p[126] * lpoly_1.plg[1] + p[127] * lpoly_1.plg[
       3] + p[128] * lpoly_1.plg[5]) * cd14 * csw_1.swc[4] + (p[129] *
       lpoly_1.plg[10] + p[130] * lpoly_1.plg[12] + p[131] * lpoly_1.plg[
       14]) * csw_1.swc[6] * cos(hr * (*tloc - p[132])));
L40:
    if (csw_1.sw[9] == 0.f || *long__ <= -1e3f) {
   goto L49;
    }
/*        LONGITUDINAL */
    ttest_2.t[10] = (p[90] * lpoly_1.plg[1] + 1.f) * (p[81] * lpoly_1.dfa *
       csw_1.swc[0] + 1.f) * ((p[65] * lpoly_1.plg[11] + p[66] *
       lpoly_1.plg[13] + p[67] * lpoly_1.plg[15] + p[104] * lpoly_1.plg[
       10] + p[105] * lpoly_1.plg[12] + p[106] * lpoly_1.plg[14] +
       csw_1.swc[4] * (p[110] * lpoly_1.plg[10] + p[111] * lpoly_1.plg[
       12] + p[112] * lpoly_1.plg[14]) * cd14) * cos(dgtr * *long__) + (
       p[91] * lpoly_1.plg[11] + p[92] * lpoly_1.plg[13] + p[93] *
       lpoly_1.plg[15] + p[107] * lpoly_1.plg[10] + p[108] * lpoly_1.plg[
       12] + p[109] * lpoly_1.plg[14] + csw_1.swc[4] * (p[113] *
       lpoly_1.plg[10] + p[114] * lpoly_1.plg[12] + p[115] * lpoly_1.plg[
       14]) * cd14) * sin(dgtr * *long__));
/*        UT AND MIXED UT,LONGITUDE */
    ttest_2.t[11] = (p[96] * lpoly_1.plg[1] + 1.f) * (p[82] * lpoly_1.dfa *
       csw_1.swc[0] + 1.f) * (p[120] * lpoly_1.plg[1] * csw_1.swc[4] *
       cd14 + 1.f) * ((p[69] * lpoly_1.plg[1] + p[70] * lpoly_1.plg[3] +
       p[71] * lpoly_1.plg[5]) * cos(sr * (*sec - p[72])));
    ttest_2.t[11] += csw_1.swc[10] * (p[77] * lpoly_1.plg[21] + p[78] *
       lpoly_1.plg[23] + p[79] * lpoly_1.plg[25]) * cos(sr * (*sec - p[
       80]) + dgtr * 2.f * *long__) * (p[138] * lpoly_1.dfa * csw_1.swc[
       0] + 1.f);
/*        UT,LONGITUDE MAGNETIC ACTIVITY */
    if (sw9 == -1.f) {
   goto L45;
    }
    ttest_2.t[12] = lpoly_1.apdf * csw_1.swc[10] * (p[121] * lpoly_1.plg[1] +
       1.f) * ((p[61] * lpoly_1.plg[11] + p[62] * lpoly_1.plg[13] + p[63]
        * lpoly_1.plg[15]) * cos(dgtr * (*long__ - p[64]))) +
       lpoly_1.apdf * csw_1.swc[10] * csw_1.swc[4] * (p[116] *
       lpoly_1.plg[10] + p[117] * lpoly_1.plg[12] + p[118] * lpoly_1.plg[
       14]) * cd14 * cos(dgtr * (*long__ - p[119])) + lpoly_1.apdf *
       csw_1.swc[11] * (p[84] * lpoly_1.plg[1] + p[85] * lpoly_1.plg[3]
       + p[86] * lpoly_1.plg[5]) * cos(sr * (*sec - p[76]));
    goto L48;
L45:
    if (p[52] == 0.f) {
   goto L48;
    }
    ttest_2.t[12] = lpoly_1.apt[0] * csw_1.swc[10] * (p[133] * lpoly_1.plg[1]
       + 1.f) * ((p[53] * lpoly_1.plg[11] + p[99] * lpoly_1.plg[13] + p[
       68] * lpoly_1.plg[15]) * cos(dgtr * (*long__ - p[98]))) +
       lpoly_1.apt[0] * csw_1.swc[10] * csw_1.swc[4] * (p[134] *
       lpoly_1.plg[10] + p[135] * lpoly_1.plg[12] + p[136] * lpoly_1.plg[
       14]) * cd14 * cos(dgtr * (*long__ - p[137])) + lpoly_1.apt[0] *
       csw_1.swc[11] * (p[56] * lpoly_1.plg[1] + p[57] * lpoly_1.plg[3]
       + p[58] * lpoly_1.plg[5]) * cos(sr * (*sec - p[59]));
L48:
/*  PARMS NOT USED: 60,83,100,140-150 */
L49:
    ttest_2.tinf = 0.f;
    if (csw_1.sw[8] == -1.f) {
   ttest_2.tinf = p[31];
    }
    i__1 = nsw;
    for (i__ = 1; i__ <= i__1; ++i__) {
/* L50: */
   ttest_2.tinf += (r__1 = csw_1.sw[i__ - 1], dabs(r__1)) * ttest_2.t[
      i__ - 1];
    }
    ret_val = ttest_2.tinf;
    return ret_val;
} /* globe5_ */

/**********************************************************************/
/* Subroutine */ int tselec_0_86(int n__, real *sv, real *svv)
{
    /* System generated locals */
    real r__1;

    /* Builtin functions */
    /* double r_mod(real *, real *); */

    /* Local variables */
    static integer i__;
    static real sav[25];

/*        SET SWITCHES */
    /* Parameter adjustments */
    if (sv) {
   --sv;
   }
    if (svv) {
   --svv;
   }

    /* Function Body */
    switch(n__) {
   case 1: goto L_tretrv;
   }

    for (i__ = 1; i__ <= 25; ++i__) {
   sav[i__ - 1] = sv[i__];
   csw_1.sw[i__ - 1] = fmod(sv[i__], c_b79);
   if ((r__1 = sv[i__], dabs(r__1)) > 0.f) {
       csw_1.swc[i__ - 1] = 1.f;
   } else {
       csw_1.swc[i__ - 1] = 0.f;
   }
/* L100: */
    }
    csw_1.isw = 64999;
    return 0;

L_tretrv:
    for (i__ = 1; i__ <= 25; ++i__) {
   svv[i__] = sav[i__ - 1];
/* L200: */
    }
    return 0;
} /* tselec_ */

/**********************************************************************/
/* Subroutine */ int tselec_86(real *sv)
{
    return tselec_0_86(0, sv, (real *)0);
    }

/**********************************************************************/
/* Subroutine */ int tretrv_86(real *svv)
{
    return tselec_0_86(1, (real *)0, svv);
    }

/**********************************************************************/
doublereal glob5l_(real *p)
{
    /* Initialized data */

    static real dr = .0172142f;
    static real t[15] = { 0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,
       0.f,0.f };
    static real dayl = -1.f;
    static real p7 = -1e3f;
    static real p9 = -1e3f;
    static real p11 = -1e3f;

    /* System generated locals */
    real ret_val, r__1;

    /* Builtin functions */
    /* double cos(doublereal); */

    /* Local variables */
    static integer i__;
    static real tt, cd7, cd9, cd11;

/*      LIMITED PARAMETER VERSION OF GLOBE 9/2/82 */
/*       CALCULATE G(L) FUNCTION FOR MSIS-86/CIRA 1986 */
/*       Lower Thermosphere Parameters */
    /* Parameter adjustments */
    --p;

    /* Function Body */
    if (lpoly_1.day != dayl || p7 != p[7]) {
   cd7 = cos(dr * (lpoly_1.day - p[7]));
    }
    if (lpoly_1.day != dayl || p9 != p[9]) {
   cd9 = cos(dr * 2.f * (lpoly_1.day - p[9]));
    }
    if (lpoly_1.day != dayl || p11 != p[11]) {
   cd11 = cos(dr * (lpoly_1.day - p[11]));
    }
    dayl = lpoly_1.day;
    p7 = p[7];
    p9 = p[9];
    p11 = p[11];

    t[0] = p[2] * lpoly_1.dfa;
    t[1] = p[4] * lpoly_1.plg[2];
    t[2] = p[6] * cd7;
    t[3] = p[8] * cd9;
    t[4] = (p[10] * lpoly_1.plg[1] + p[22] * lpoly_1.plg[3]) * cd11;
    t[5] = 0.f;
    t[6] = p[14] * lpoly_1.plg[10] * lpoly_1.ctloc + p[15] * lpoly_1.plg[10] *
        lpoly_1.stloc;
    t[7] = (p[16] * lpoly_1.plg[20] + p[18] * lpoly_1.plg[22] + p[20] *
       lpoly_1.plg[23] * cd11 * csw_1.swc[4]) * lpoly_1.c2tloc + (p[17] *
        lpoly_1.plg[20] + p[19] * lpoly_1.plg[22] + p[21] * lpoly_1.plg[
       23] * cd11 * csw_1.swc[4]) * lpoly_1.s2tloc;
    t[13] = p[12] * lpoly_1.plg[30] * lpoly_1.c3tloc + p[25] * lpoly_1.plg[30]
        * lpoly_1.s3tloc;
    if (csw_1.sw[8] == 1.f) {
   t[8] = lpoly_1.apdf * (p[23] + p[24] * lpoly_1.plg[2] * csw_1.swc[1]);
    }
    if (csw_1.sw[8] == -1.f) {
   t[8] = p[3] * lpoly_1.apt[2] + p[5] * lpoly_1.plg[2] * lpoly_1.apt[2]
      * csw_1.swc[1];
    }
/*       PARMS NOT USED: 13 */
    tt = 0.f;
    for (i__ = 1; i__ <= 14; ++i__) {
/* L50: */
   tt += (r__1 = csw_1.sw[i__ - 1], dabs(r__1)) * t[i__ - 1];
    }
    ret_val = tt;
    return ret_val;
} /* glob5l_ */

/**********************************************************************/
doublereal dnet_86(real *dd, real *dm, real *zhm, real *xmm, real *xm)
{
    /* System generated locals */
    real ret_val;
    doublereal d__1, d__2;

    /* Builtin functions */
    /* double log(doublereal), exp(doublereal), pow_dd(doublereal *, doublereal *);*/

    /* Local variables */
    static real a, ylog;

/*       8/20/80 */
/*       TURBOPAUSE CORRECTION FOR MSIS MODELS */
/*       Eq. A12b */
    a = *zhm / (*xmm - *xm);
/*       Eq. A12a */
    ylog = a * log(*dm / *dd);
    if (ylog < -10.f) {
   goto L10;
    }
    if (ylog > 10.f) {
   goto L20;
    }
    d__1 = (doublereal) (exp(ylog) + 1.f);
    d__2 = (doublereal) (1 / a);
    ret_val = *dd * pow(d__1, d__2);
    goto L50;
L10:
    ret_val = *dd;
    goto L50;
L20:
    ret_val = *dm;
    goto L50;
L50:
    return ret_val;
} /* dnet_ */

/**********************************************************************/
doublereal ccor_86(real *alt, real *r__, real *h1, real *zh)
{
    /* System generated locals */
    real ret_val;

    /* Builtin functions */
    /* double exp(doublereal); */

    /* Local variables */
    static real e, ex;

/*        CHEMISTRY/DISSOCIATION CORRECTION FOR MSIS MODELS */
/*     Eq. A20a or Eq. A21 */
    e = (*alt - *zh) / *h1;
    if (e > 70.f) {
   goto L20;
    }
    if (e < -70.f) {
   goto L10;
    }
    ex = exp(e);
    ret_val = *r__ / (ex + 1.f);
    goto L50;
L10:
    ret_val = *r__;
    goto L50;
L20:
    ret_val = 0.f;
    goto L50;
L50:
    ret_val = exp(ret_val);
    return ret_val;
} /* ccor_86 */

/**********************************************************************/
/* Subroutine */ int prmsg5_(void)
{
    static integer i__;

/*          CIRA     11-FEB-86 */
/* XX      DIMENSION ISD(3),IST(2) */
/* XX      COMMON/DATIME/ISDATE(3),ISTIME(2),NAME(2) */
/* XX      DATA ISD/'11-F','EB-8','6   '/,IST/'18:2','3:31'/ */
/*         TEMPERATURE */
/*         HE DENSITY */
/*         O DENSITY */
/*         N2 DENSITY & TLB */
/*         Z0 & T0 */
/*         TR */
/*         O2 DENSITY */
/*         AR DENSITY */
/*          H DENSITY */
/*          N DENSITY */
/*          S PARAM */
/*          TURBO */
/*         LOWER BOUNDARY */
    for (i__ = 1; i__ <= 3; ++i__) {
/* XX        ISDATE(I)=ISD(I) */
/* L10: */
    }
    for (i__ = 1; i__ <= 2; ++i__) {
/* XX        ISTIME(I)=IST(I) */
/* L20: */
    }
/* XX      NAME(1)='CIRA' */
/* XX      NAME(2)='-86 ' */
    return 0;
} /* prmsg5_ */


/**********************************************************************/
/*  f2c implementation generated some compiler warnings in declaring  */
/*  and initializing these structures in the preamble.  This function */
/*  circumvents the warnings.                                         */
void InitMSIS86(void)
{
real parmb_86_e1[2] = { 980.665f, 6356.77f };

real lower5_e1[64] = { 1041.3f, 386.f, 190.f, 16.6728f, 115.f, 120.f, 94.5537f,
       0.f, 2.456e7f, 6.71072e-6f, 100.f, 0.f, 110.f, 10.f, 0.f, 0.f,
       8.594e10f, .54f, 105.f, -8.f, 110.f, 10.f, 90.f, 2.f, 2.81e11f,
       0.f, 105.f, 28.f, 28.95f, 0.f, 0.f, 0.f, 3.3e10f, .26827f, 105.f,
       0.f, 110.f, 10.f, 0.f, 0.f, 1.33e9f, .0119615f, 105.f, 0.f, 110.f,
        10.f, 0.f, 0.f, 176100.f, 1.f, 95.f, -8.f, 110.f, 10.f, 90.f,
       2.f, 1e7f, 1.f, 105.f, -8.f, 110.f, 10.f, 90.f, 2.f };

real parm5_e1[1400] = { .99604f, .0385528f, .00303445f, -.105531f, -.00607134f,
       -5.16278e-4f, -.115622f, .0020224f, .00990156f, -.127371f,
       -.0302449f, .0123512f, -.00526277f, -8.45398f, 0.f, .014237f, 0.f,
        125.818f, .00805486f, .00164419f, -6.21452e-6f, .00311701f, 0.f,
       .00386578f, .132397f, .213315f, 0.f, 0.f, 0.f, -6.4111e-6f, 0.f,
       30.015f, .00533297f, .00389146f, .00204725f, 0.f, 0.f, -.0192645f,
        2.75905f, .00147284f, 3.41345e-4f, -.00117388f, -3.54589e-4f,
       .113139f, .169134f, .00508295f, 3.65016e-5f, .00426385f,
       1.15102e-4f, .00511819f, .00609108f, 4.04995e-5f, .00153049f,
       2.4147e-5f, .00230764f, .00155267f, .00133722f, -.00182318f,
       -263.007f, 0.f, .00137337f, 9.95774e-4f, 0.f, -108.983f,
       .00562606f, .00594053f, .00109358f, 0.f, -.013341f, -.0243409f,
       -.0135688f, 31137.f, 0.f, 0.f, 0.f, -2830.23f, 8.45583e-4f,
       5.38706e-4f, 0.f, 247.956f, .00292246f, 0.f, 0.f, 7.47703e-5f,
       8.87993e-4f, 0.f, 0.f, 0.f, 0.f, 0.f, -.011654f, -.00449173f,
       -3.53189e-4f, -1.73933e-4f, -1.53218e-4f, -.565411f, .00777272f,
       -91.1784f, 6.45187e-4f, 0.f, -8.37685e-4f, .00242318f, .00473796f,
        -.00301801f, -.00423564f, -.00248289f, 9.19286e-4f, .00216372f,
       8.63968e-4f, .00189689f, .00415654f, 0.f, .0118068f, .0033119f,
       0.f, .00120222f, 0.f, 0.f, -3.07246f, 0.f, 0.f, 6.72403e-4f,
       .0010893f, 9.72278e-4f, 4.68242f, -3.15034e-4f, .00400059f,
       .00515036f, .00162989f, .00108824f, 9.95261e-4f, 4.18955f,
       -.364059f, .00170182f, 0.f, 0.f, -3.2012f, 0.f, .00580206f, 0.f,
       0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 1.04934f,
       -.0288362f, -.207095f, -.103314f, -.00702373f, .0129664f,
       .408853f, -.00919895f, -.018866f, 1.40927f, .175033f, .0187351f,
       .110979f, -7.42871f, 0.f, .267143f, -.0595979f, 105.038f,
       -.0840963f, -6.97632e-4f, 2.06521e-6f, 7.65306e-4f, 0.f, 0.f,
       .126762f, .128876f, -.0504479f, -.0130735f, -.0224348f, 0.f, 0.f,
       -150.832f, -.00629928f, 0.f, -.0040776f, 0.f, 0.f, .0525725f,
       -31.1486f, -.00313351f, .00275838f, 0.f, 0.f, .111247f, .108815f,
       -.0466713f, 0.f, -.00329329f, 0.f, .00167838f, -.00916691f,
       3.45044e-5f, -.00971806f, 0.f, -.00204672f, -.00786899f,
       -.00798285f, .00536515f, -5311.72f, 0.f, -.00642781f, -.0017169f,
       0.f, -67.9131f, -.0179912f, -.0158305f, -.00712313f, 0.f,
       .0253477f, .085296f, .102163f, 29500.9f, 0.f, 0.f, 0.f, -6846.25f,
        -.00619098f, -.00269289f, 0.f, -520.231f, -.00633463f, 0.f, 0.f,
       -.00602428f, -.00407077f, .00542264f, 0.f, 0.f, 0.f, 0.f,
       .040756f, .0282288f, .00908088f, 0.f, 0.f, -.405204f, -.0597931f,
       -73.1823f, -.0020662f, 0.f, -.00372723f, -.0188146f, -.0101794f,
       .00804633f, .010109f, .00873253f, .0238268f, .00480444f,
       .00171088f, .0396369f, -.0213809f, 0.f, -.102588f, -.00591702f,
       0.f, .00270923f, 0.f, 0.f, -175.043f, .603489f, -.617589f,
       .00838098f, .00183871f, -7.05329e-4f, -4.06644f, -.00509347f,
       -.0284344f, -.012416f, .0133665f, .0039341f, -5.03723e-4f,
       -4.57683f, -.529542f, -.00425812f, 0.f, 0.f, 19.1541f, 0.f,
       .00323247f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
        .931113f, -.138721f, -.133457f, -.0529542f, -.00444983f,
       .0135264f, .0598075f, -.036288f, -.0312798f, .372068f, .0295974f,
       .0120509f, .0521995f, -7.78888f, 0.f, .118634f, -.0204495f,
       103.28f, .0982432f, 4.77694e-4f, 0.f, .00274372f, 0.f, 0.f,
       .0757809f, .171403f, -.0105205f, 0.f, 0.f, 0.f, 0.f, -8.73348f,
       -.00581094f, 0.f, -.00814944f, 0.f, 0.f, .0517255f, -15.3028f,
       -.00348932f, 9.61771e-4f, .00557732f, -4.5418e-4f, .0988213f,
       .0940456f, -.0318797f, 0.f, 0.f, 0.f, .00232122f, -.0060022f,
       2.77654e-5f, -.00322019f, 0.f, -.00378551f, -.00334809f,
       -.00170668f, 0.f, 6361.84f, 0.f, .00159986f, -.00388204f,
       -.00164825f, -74.7955f, -.010536f, -.00945723f, -.00159824f,
       -7.0673e-4f, -.0168513f, -.113023f, -.0636637f, -13770.9f, 0.f,
       0.f, 0.f, -15236.8f, -.00586061f, -.00253108f, 0.f, -2548.37f,
       -.00328988f, 0.f, 0.f, -.00276364f, .00967923f, 0.f, 0.f, 0.f,
       0.f, 0.f, .0434255f, .011402f, -.00618447f, 0.f, 0.f, -.302568f,
       -.0327694f, -67.1589f, -.0022834f, 0.f, .0030623f, -.00465113f,
       -.00973421f, .0128326f, .00788553f, .00797197f, -.012076f,
       -.00767547f, -.00120755f, -.0298523f, -.012656f, 0.f, -.056835f,
       -.0153039f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, .00242911f,
       -.00401347f, -.00219074f, 3.11281f, .00323251f, -.00639523f,
       -.00663069f, -3.04403e-4f, -.0040192f, -.00118708f, 4.15211f,
       -.201896f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
       0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 1.06903f, 3.77113e-4f, 0.f, 0.f,
       0.f, .0898481f, -23.6325f, .020818f, 139.638f, -.119444f,
       -8.45398f, -3.99776e-6f, 0.f, .0036621f, -.00178929f, .0190412f,
       -.0392257f, .00632343f, .00548144f, 0.f, 0.f, 0.f, 0.f, 0.f,
       -.00243022f, .976619f, 5.68478e-4f, .00582026f, 0.f, .00621998f,
       0.f, 0.f, .0107674f, 89.382f, -.0192414f, -8.45398f, 0.f, 0.f,
       -.02002f, -.00195833f, -.00938391f, .013148f, -.00260147f,
       -8.08556e-4f, 5.11651e-5f, .00255717f, 0.f, .00466814f,
       .00664196f, 0.f, .998594f, 1.90038e-4f, 0.f, -.0243825f, 0.f, 0.f,
        0.f, 0.f, 0.f, .0522105f, -8.45398f, 0.f, 0.f, 0.f, 0.f,
       .00767271f, .00564539f, -.00270623f, -5.26454e-4f, .00137075f,
       .0013306f, 0.f, 0.f, 0.f, 0.f, .949197f, 0.f, 0.f, -.0768008f,
       0.f, 0.f, 0.f, -.0137993f, -1.40136f, .120481f, -8.45398f, 0.f,
       0.f, 0.f, 0.f, .00987746f, .0017533f, -6.88835e-4f, .00287022f,
       0.f, 0.f, .0744513f, 0.f, 0.f, 0.f, .15284f, 0.f, 0.f, 1.16252f,
       0.f, 0.f, 0.f, 0.f, 0.f, -.64919f, -8.45398f, 0.f, 0.f, 0.f, 0.f,
       -.0584949f, -.102105f, .0299153f, -.0486227f, 0.f, 0.f, 0.f, 0.f,
       0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
       0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
       0.f, .931402f, .137976f, 0.f, 3.23736e-4f, 0.f, -.00910906f,
       .0707506f, 0.f, -.051665f, .0689755f, 0.f, 0.f, 0.f, -8.45398f,
       0.f, .028114f, 0.f, 73.6009f, .0596604f, 0.f, 0.f, -.00151792f,
       0.f, 0.f, .132397f, .213315f, 0.f, 0.f, 0.f, 0.f, 0.f, 9.48758f,
       .00884541f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
       .113139f, .169134f, .0145192f, 0.f, 0.f, 0.f, 0.f, .0107906f,
       2.99942e-5f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
       0.f, 0.f, -.014893f, -.00787184f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
       0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
       0.f, 0.f, 0.f, 0.f, 0.f, -.068342f, -.0441778f, 0.f, 0.f, 0.f,
       0.f, .022973f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
       0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
       0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
       0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
       0.f, 0.f, 0.f, 0.f, .868053f, .236364f, .134306f, .0103086f, 0.f,
       -.00379164f, -.157806f, 0.f, -.0587644f, -.312508f, 0.f,
       .0437387f, -.0354091f, -22.3636f, 0.f, -.0533976f, 0.f, 114.091f,
       .0517497f, 0.f, 0.f, 0.f, 0.f, 0.f, .132397f, .213315f, 0.f, 0.f,
       0.f, 0.f, 0.f, 342.702f, .0157033f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
       -.00366278f, -.00116193f, 0.f, 0.f, .113139f, .169134f, .0178431f,
        0.f, 0.f, 0.f, 0.f, .0162864f, 3.16963e-5f, .0127968f, 0.f, 0.f,
       -.00704599f, .00207921f, .0063666f, 22994.f, 0.f, .0127833f,
       -.00208036f, -.0046182f, -62.9391f, -.0120745f, .0136675f,
       .0136011f, -.00537162f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
       19250.9f, .00835522f, .00419439f, 0.f, 12036.6f, 0.f, 0.f, 0.f,
       -.0100034f, -.00233267f, .00972374f, 0.f, 0.f, 0.f, 0.f,
       -.0265079f, -.0209125f, -.0109465f, 0.f, 0.f, 0.f, .0217252f,
       -71.2385f, -.00189428f, 0.f, -.00602006f, .0169058f, 0.f, 0.f,
       0.f, 0.f, 0.f, 0.f, 0.f, .0290646f, .00348971f, 0.f, .0501174f,
       .0550595f, 0.f, -.00955897f, 0.f, 0.f, -1516.93f, 0.f, 0.f,
       .0129306f, .00269567f, 0.f, 3.92243f, -.0084769f, .0116896f, 0.f,
       .0148967f, .00544521f, 0.f, 5.64918f, 0.f, -.00772178f, 0.f, 0.f,
       -73.4042f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
       0.f, 0.f, 1.27515f, -.210472f, -.177924f, .2189f, .0288436f,
       .0190077f, .291001f, .0217437f, -.0105186f, .436141f, .107605f,
       .0330755f, .0400581f, -9.58051f, 0.f, .0154028f, 0.f, 73.4194f,
       .049654f, -.00595906f, 3.84512e-5f, -.0136f, 0.f, 0.f, .132397f,
       .213315f, -.041661f, 0.f, 0.f, 0.f, 0.f, 146.276f, -.0198408f,
       0.f, .013253f, 0.f, 0.f, 0.f, 0.f, -1.04687e-4f, -.00147562f, 0.f,
        0.f, .113139f, .169134f, -.0126913f, 0.f, 0.f, 0.f, -.0060837f,
       -.0257587f, 3.19022e-5f, 0.f, 0.f, .0156644f, .010364f,
       .00105771f, 0.f, 3579.49f, 0.f, -.00125672f, .00152783f,
       .00130518f, 7.55558f, -.00920341f, -.0209142f, -.0134106f, 0.f,
       -.0483312f, .08309f, .0988009f, -14114.8f, 0.f, 0.f, 0.f,
       -1055.13f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, .00673442f,
       .00201691f, 0.f, 0.f, 0.f, 0.f, 0.f, .0598019f, .00633298f,
       -.00112871f, 0.f, 0.f, 0.f, -.0128604f, 0.f, 0.f, 0.f, -.0049496f,
        -.0136415f, -.0115039f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
       0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, -.0058686f,
       -.00141732f, .00213697f, 2.63845f, -.00834186f, -.0187336f,
       -.019087f, -.0080381f, -.00284279f, .00256722f, 1.71429f, 0.f,
       0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
       0.f, 0.f, 0.f, 0.f, 57.3587f, -.398747f, 0.f, -.529554f,
       -.00582186f, .0714177f, -.679279f, -.167715f, -.0642434f,
       -.211569f, -.159922f, -1.71024e-4f, -.115885f, 6.51603f, 0.f,
       -.176683f, .0650395f, 1.43504f, .0928208f, .00511662f, 0.f,
       .00995121f, 0.f, 0.f, .132397f, .213315f, .101451f, 0.f, 0.f, 0.f,
        0.f, 56.7667f, .00238192f, 0.f, -.018824f, 0.f, 0.f, .0476218f,
       23.5206f, .00475901f, .00576162f, .0151815f, -.019273f, .113139f,
       .169134f, -.0288771f, 0.f, 0.f, 0.f, .00118418f, -.00368927f,
       3.14704e-5f, .00882198f, 0.f, -.0192562f, -.00258674f, -.0219913f,
        0.f, 4386.55f, 0.f, .00760126f, .00259438f, .0017231f, 77.9204f,
       7.97786e-4f, -.0077051f, .00190982f, .00272707f, .0101016f,
       .116537f, -.00312236f, 13978.3f, 0.f, 0.f, 0.f, -1307.12f, 0.f,
       0.f, 0.f, 0.f, 0.f, 0.f, 0.f, -.00320544f, -.020697f, 0.f, 0.f,
       0.f, 0.f, 0.f, .015901f, -.00191427f, -.0342829f, 0.f, 0.f, 0.f,
       -.0345379f, 89.4518f, .00171556f, 0.f, -.00765278f, -2.08987e-4f,
       -.0157393f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
        0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, -.00860673f, -.0119922f,
       -.00646356f, -3.00107f, -.00932511f, -.0150205f, -.00867835f,
       -.00764801f, -.0131495f, -.0067672f, -1.82396f, 0.f, 0.f, 0.f,
       0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
       0.f, 0.f, .951363f, -.0467542f, .12026f, 0.f, 0.f, .0191357f, 0.f,
        0.f, .00125429f, -.13324f, 0.f, 0.f, 0.f, -8.45398f, 0.f, 0.f,
       0.f, 0.f, 0.f, 0.f, 0.f, .00252317f, 0.f, -.00973404f, .132397f,
       .213315f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, -7.18482e-4f, 0.f,
       0.f, 0.f, 0.f, 0.f, 0.f, 0.f, .00787683f, -.00233698f, .113139f,
       .169134f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
       0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
       0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
       0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
       0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
       0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
       0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
       0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
       0.f, 0.f, 0.f, .933804f, 5.47446f, .153263f, .919303f, 16.4109f,
       4.27083f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
       0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 1.15897f, .471094f,
       1.09459f, 5.25012f, 1.f, 1.f, 1.03999f, .767132f, 1.10514f,
       1.75636f, 1.10845f, 2.33439f, .796532f, 4.3152f, 4.073f, 1.01885f,
        .239547f, 2.53791e-6f, .842931f, 1.04192f, 2.00202f, 1.f, 1.f,
       1.f, 1.f };

      short i;

      for(i=0;i<2;i++) parmb_86.e_1[i] = parmb_86_e1[i];
      for(i=0;i<64;i++) lower5_.e_1[i] = lower5_e1[i];
      for(i=0;i<1400;i++) parm5_.e_1[i] = parm5_e1[i];

}
/**********************************************************************/
void MSIS_ECEFToWGS84(double p[3], double *glat, double *glong, double *alt)
{
      double a = 6378137.0;
      double f = 1.0/298.257222101;
      double b = a*(1.0-f);
      double e2 = f*(2.0-f);
      double ep2 = f*(2.0-f)/(1.0-f)/(1.0-f);
      double r,E2,F,G,C,S,P,Q,r0,U,V,Z0;

      double OneMinusE2,Z1,SpolyG,Qpoly;

      OneMinusE2 = 1.0-e2;

      r = sqrt(p[0]*p[0]+p[1]*p[1]);

      E2 = a*a-b*b;

      Z1 = b*p[2];

      F = 54.0*Z1*Z1;

      G = r*r+OneMinusE2*p[2]*p[2]-e2*E2;

      Z1 = e2*r/G;
      C = Z1*Z1*F/G;

      S = pow(1.0+C+sqrt(C*C+2.0*C),1.0/3.0);

      SpolyG = (S+1.0/S+1.0)*G;
      P = F/(3.0*SpolyG*SpolyG);

      Q = sqrt(1.0+2.0*e2*e2*P);

      Qpoly = 1.0+Q;
      r0 = -P*e2*r/Qpoly+sqrt(0.5*a*a*Qpoly/Q-P*OneMinusE2*p[2]*p[2]/(Q*Qpoly)-0.5*P*r*r);

      Z1 = r-e2*r0;
      Z1 *= Z1;

      U = sqrt(Z1+p[2]*p[2]);

      V = sqrt(Z1+OneMinusE2*p[2]*p[2]);


      Z1 = b*b/a/V;
      Z0 = Z1*p[2];

      *alt = U*(1.0-Z1);
      *glat = atan((p[2]+ep2*Z0)/r);
      *glong = atan2(p[1],p[0]);
}
/**********************************************************************/
/*
C***********************************************************************
      SUBROUTINE GTS5(IYD,SEC,ALT,GLAT,GLONG,STL,F107A,F107,AP,MASS,D,T)
C        MSIS-86/CIRA 1986 Neutral Thermosphere Model
C         A.E.Hedin 3/15/85;2/26/87 (Variable Names Shortened)
C         10/14/87 increase altitude limit of O mixing calculation
C             ALTL(2) from 300.0 to 400.0 km .
C     INPUT:
C        IYD - YEAR AND DAY AS YYYYDDD
C        SEC - UT(SEC)
C        ALT - ALTITUDE(KM) (GREATER THAN 85 KM)
C        GLAT - GEODETIC LATITUDE(DEG)
C        GLONG - GEODETIC LONGITUDE(DEG)
C        STL - LOCAL APPARENT SOLAR TIME(HRS)
C        F107A - 3 MONTH AVERAGE OF F10.7 FLUX
C        F107 - DAILY F10.7 FLUX FOR PREVIOUS DAY
C        AP - MAGNETIC INDEX(DAILY) OR WHEN SW(9)=-1. :
C           - ARRAY CONTAINING:
C             (1) DAILY AP
C             (2) 3 HR AP INDEX FOR CURRENT TIME
C             (3) 3 HR AP INDEX FOR 3 HRS BEFORE CURRENT TIME
C             (4) 3 HR AP INDEX FOR 6 HRS BEFORE CURRENT TIME
C             (5) 3 HR AP INDEX FOR 9 HRS BEFORE CURRENT TIME
C             (6) AVERAGE OF EIGHT 3 HR AP INDICIES FROM 12 TO 33 HRS PRIOR
C                    TO CURRENT TIME
C             (7) AVERAGE OF EIGHT 3 HR AP INDICIES FROM 36 TO 59 HRS PRIOR
C                    TO CURRENT TIME
C        MASS - MASS NUMBER (ONLY DENSITY FOR SELECTED GAS IS
C                 CALCULATED.  MASS 0 IS TEMPERATURE.  MASS 48 FOR ALL.
C     OUTPUT:
C        D(1) - HE NUMBER DENSITY(CM-3)
C        D(2) - O NUMBER DENSITY(CM-3)
C        D(3) - N2 NUMBER DENSITY(CM-3)
C        D(4) - O2 NUMBER DENSITY(CM-3)
C        D(5) - AR NUMBER DENSITY(CM-3)
C        D(6) - TOTAL MASS DENSITY(GM/CM3)
C        D(7) - H NUMBER DENSITY(CM-3)
C        D(8) - N NUMBER DENSITY(CM-3)
C        T(1) - EXOSPHERIC TEMPERATURE
C        T(2) - TEMPERATURE AT ALT
C
C      TO GET OUTPUT IN M-3 and KG/M3:   CALL METERS(.TRUE.)
C
C          ADDITIONAL COMMENTS
C           (1) LOWER BOUND QUANTITIES IN COMMON/GTS3C/
C           (2) TO TURN ON AND OFF PARTICULAR VARIATIONS CALL TSELEC(SW)
C               WHERE SW IS A 25 ELEMENT ARRAY CONTAINING 0. FOR OFF, 1.
C               FOR ON, OR 2. FOR MAIN EFFECTS OFF BUT CROSS TERMS ON
C               FOR THE FOLLOWING VARIATIONS
C               1 - F10.7 EFFECT ON MEAN  2 - TIME INDEPENDENT
C               3 - SYMMETRICAL ANNUAL    4 - SYMMETRICAL SEMIANNUAL
C               5 - ASYMMETRICAL ANNUAL   6 - ASYMMETRICAL SEMIANNUAL
C               7 - DIURNAL               8 - SEMIDIURNAL
C               9 - DAILY AP             10 - ALL UT/LONG EFFECTS
C              11 - LONGITUDINAL         12 - UT AND MIXED UT/LONG
C              13 - MIXED AP/UT/LONG     14 - TERDIURNAL
C              15 - DEPARTURES FROM DIFFUSIVE EQUILIBRIUM
C              16 - ALL TINF VAR         17 - ALL TLB VAR
C              18 - ALL T0 VAR           19 - ALL S VAR
C              20 - ALL Z0 VAR           21 - ALL NLB VAR
C              22 - ALL TR12 VAR         23 - TURBO SCALE HEIGHT VAR
C
C              To get current values of SW: CALL TRETRV(SW)
C
C !!!!!!!!!!!!!!!!!!!!!!!!!!! CHANGES !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
C
C  - NAME,ISD,IST,ISDATE, and ISTIME were changed to character variables
C    in GTS5 and PRMSG5
C
C  - The variable dimension of P and AP in GLOBE5 and GLOBE5L was
C    indicted by *, rather than 1; if this does not work on your system
C    you may want to use P(150) and AP(7).
C
C  - The large data statement in PRMSG5 is now read in from file
C    MSIS86.DAT; some compilers do not allow named commons to be
C    initialized in a data statement.
C
C  - The first call to GLOBE5 should come before the common array SW(25)
C    is used in GTS5.
C
C Dieter Bilitza !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! March 87
C **********************************************************************
*/
double MSIS86(long Year, long DOY, long Hour, long Minute,
              double Second, double pbn[3],
              double PriMerAng, double F107, double AP)
{
      integer IYD;
      real SEC,ALT,GLAT,GLONG,STL,F107A;
      real D[8],T[2];
      double TwoPi = 6.28318530717959;
      double cth,sth,pbe[3],density;
      static integer MASS = 48;  /* All species */
      static long First = 1;

      if (First) {
         First = 0;
         InitMSIS86();
      }

      IYD = 1000*Year+DOY;
      SEC = 3600.0*((double) Hour) + 60.0*((double) Minute) + Second;
      F107A = F107;

      cth = cos(PriMerAng);
      sth = sin(PriMerAng);

      pbe[0] = cth*pbn[0]+sth*pbn[1];
      pbe[1] = -sth*pbn[0]+cth*pbn[1];
      pbe[2] = pbn[2];

      MSIS_ECEFToWGS84(pbe,&GLAT,&GLONG,&ALT);

      GLAT *= 360.0/TwoPi;
      GLONG *= 360.0/TwoPi;
      ALT /= 1.0E3;
      STL = SEC/3600.0 + GLONG/15.0;

      gts5_(&IYD,&SEC,&ALT,&GLAT,&GLONG,&STL,&F107A,&F107,&AP,&MASS,
         D,T);

      density = 1.0E3*D[5];  /* Convert from g/cm^3 to kg/m^3 */

      return(density);
}

/* #ifdef __cplusplus
** }
** #endif
*/

/*  -- translated by f2c (version 20030306).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#include "val_atm_msis90.h"

/* Common Block Declarations */

struct {
    real tlb, s, db04, db16, db28, db32, db40, db48, db01, za, t0, z0, g0, rl,
	     dd, db14, tr12;
} gts3c_90;

#define gts3c_1 gts3c_90

struct {
    real tn1[5], tn2[4], tn3[5], tgn1[2], tgn2[2], tgn3[2];
} meso6_;

#define meso6_1 meso6_

struct lower6_1_ {
    real ptm[10], pdm[80]	/* was [10][8] */;
};

#define lower6_1 (*(struct lower6_1_ *) &lower6_)

struct parm6_1_ {
    real pt[150], pd[1350]	/* was [150][9] */, ps[150], pdl[50]	/* 
	    was [25][2] */, ptl[400]	/* was [100][4] */, pma[1000]	/* 
	    was [100][10] */;
};
struct parm6_2_ {
    real pt1[50], pt2[50], pt3[50], pa1[50], pa2[50], pa3[50], pb1[50], pb2[
	    50], pb3[50], pc1[50], pc2[50], pc3[50], pd1[50], pd2[50], pd3[50]
	    , pe1[50], pe2[50], pe3[50], pf1[50], pf2[50], pf3[50], pg1[50], 
	    pg2[50], pg3[50], ph1[50], ph2[50], ph3[50], pi1[50], pi2[50], 
	    pi3[50], pj1[50], pj2[50], pj3[50], pk1[50], pl1[50], pl2[50], 
	    pm1[50], pm2[50], pn1[50], pn2[50], po1[50], po2[50], pp1[50], 
	    pp2[50], pq1[50], pq2[50], pr1[50], pr2[50], ps1[50], ps2[50], 
	    pu1[50], pu2[50], pv1[50], pv2[50], pw1[50], pw2[50], px1[50], 
	    px2[50], py1[50], py2[50], pz1[50], pz2[50];
};

#define parm6_1 (*(struct parm6_1_ *) &parm6_)
#define parm6_2 (*(struct parm6_2_ *) &parm6_)

struct {
    real sw[25];
    integer isw;
    real swc[25];
} csw_90;

#define csw_1 csw_90

struct mavg6_1_ {
    real pavgm[10];
};

#define mavg6_1 (*(struct mavg6_1_ *) &mavg6_)

struct {
    real dm04, dm16, dm28, dm32, dm40, dm01, dm14;
} dmix_90;

#define dmix_1 dmix_90

struct {
    real gsurf, re;
} parmb_90;

#define parmb_1 parmb_90

struct metsel_1_ {
    integer imr;
};

#define metsel_1 (*(struct metsel_1_ *) &metsel_90)

union {
    struct {
	real tinfg, gb, rout, tt[15];
    } _1;
    struct {
	real tinf, gb, rout, t[15];
    } _2;
} ttest_90;

#define ttest_1 (ttest_90._1)
#define ttest_2 (ttest_90._2)

union {
    struct {
	real plg[36]	/* was [9][4] */, ctloc, stloc, c2tloc, s2tloc, 
		c3tloc, s3tloc;
	integer iyr;
	real day, df, dfa, apd, apdf, apt[4], xlong, clong, slong;
    } _1;
    struct {
	real plg[36]	/* was [9][4] */, ctloc, stloc, c2tloc, s2tloc, 
		c3tloc, s3tloc;
	integer iyr;
	real day, df, dfa, apd, apdf, apt[4], long__, clong, slong;
    } _2;
} lpoly_90;

#define lpoly_1 (lpoly_90._1)
#define lpoly_2 (lpoly_90._2)

struct {
    integer mp, ii, jg, lt;
    real qpb[50];
    integer ierr, ifun, n, j;
    real dv[60];
} lsqv_90;

#define lsqv_1 lsqv_90

struct {
    real taf;
} fit_90;

#define fit_1 fit_90

/* Initialized data */

struct {
    real e_1[90];
    } lower6_ = { 1041.3f, 386.f, 195.f, 16.6728f, 213.f, 120.f, 240.f, 187.f,
	     -2.f, 0.f, 2.456e7f, 6.71072e-6f, 100.f, 0.f, 110.f, 10.f, 0.f, 
	    0.f, 0.f, 0.f, 8.594e10f, .54f, 105.f, -8.f, 110.f, 10.f, 90.f, 
	    2.f, 0.f, 0.f, 2.81e11f, 0.f, 105.f, 28.f, 28.95f, 0.f, 0.f, 0.f,
	    0.f, 0.f, 3.3e10f, .26827f, 105.f, 0.f, 110.f, 10.f, 0.f, 0.f, 
	    0.f, 0.f, 1.33e9f, .0119615f, 105.f, 0.f, 110.f, 10.f, 0.f, 0.f, 
	    0.f, 0.f, 176100.f, 1.f, 95.f, -8.f, 110.f, 10.f, 90.f, 2.f, 0.f, 
	    0.f, 1e7f, 1.f, 105.f, -8.f, 110.f, 10.f, 90.f, 2.f, 0.f, 0.f, 
	    1e7f, 1.f, 105.f, -8.f, 110.f, 10.f, 90.f, 2.f, 0.f, 0.f };

struct {
    real e_1[3100];
    } parm6_ = { .99604f, .0385528f, .00303445f, -.105531f, -.00607134f, 
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
	    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 1.06903f, 0.f, 0.f, .0036621f, 0.f, 
	    .0190412f, -.00178929f, 0.f, -.0392257f, -.119444f, 0.f, 0.f, 0.f,
	     -8.45398f, 0.f, .020818f, 0.f, 139.638f, .0898481f, 0.f, 0.f, 
	    3.77113e-4f, 0.f, 0.f, .132397f, .213315f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, -23.6325f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, -.00243022f, 
	    -3.99776e-6f, .00632343f, .00548144f, .113139f, .169134f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, 2.4147e-5f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
	     0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, .976619f, 0.f, 0.f, -.02002f, 0.f, -.00938391f, -.00195833f, 
	    0.f, .013148f, -.0192414f, 0.f, 0.f, 0.f, -8.45398f, 0.f, 
	    .0107674f, 0.f, 89.382f, 0.f, 0.f, 0.f, 5.68478e-4f, 0.f, 0.f, 
	    .132397f, .213315f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, .00466814f, 0.f,
	     0.f, 5.11651e-5f, .00255717f, 0.f, 0.f, 0.f, 0.f, -.00260147f, 
	    -8.08556e-4f, .113139f, .169134f, .00664196f, 0.f, 0.f, 0.f, 0.f, 
	    .00582026f, 2.4147e-5f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, .00621998f, 0.f,
	     0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    .931402f, .137976f, 0.f, 3.23736e-4f, 0.f, -.00910906f, .0707506f,
	     0.f, -.051665f, .0689755f, 0.f, 0.f, 0.f, -8.45398f, 0.f, 
	    .028114f, 0.f, 73.6009f, .0596604f, 0.f, 0.f, -.00151792f, 0.f, 
	    0.f, .132397f, .213315f, 0.f, 0.f, 0.f, 0.f, 0.f, 9.48758f, 
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
	    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, -8.45398f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, .132397f, .213315f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, .113139f, .169134f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, .951363f, -.0467542f, .12026f, 0.f, 0.f, .0191357f, 0.f, 0.f,
	     .00125429f, -.13324f, 0.f, 0.f, 0.f, -8.45398f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, .00252317f, 0.f, -.00973404f, .132397f, 
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
	    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, .140925f, 1.15897f, .471094f, 
	    1.09459f, 5.25012f, 1.f, 1.f, 1.03999f, .767132f, 1.10514f, 
	    1.75636f, 1.10845f, 2.33439f, .796532f, 4.3152f, 4.073f, 122.807f,
	     .239547f, 2.53791e-6f, .842931f, 1.04192f, 2.00202f, 1.f, 1.f, 
	    1.f, .962736f, 1.02083f, .0408449f, -.0234582f, 4.38274e-4f, 
	    -.015238f, -.0209089f, .00446355f, -.0034125f, -.0112961f, 
	    -.0703277f, -.0482724f, 0.f, 0.f, -6.20496f, 0.f, -.00980197f, 
	    -.0145065f, -113.226f, .0228455f, 0.f, 0.f, 4.93658e-4f, 0.f, 
	    .00379078f, .132397f, .213315f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    -8890.51f, .002259f, .00176142f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    -2.55015e-4f, .00221388f, -5.99073e-4f, -.00352331f, .113139f, 
	    .169134f, .00779156f, -.00193458f, -.0108596f, -4.39285e-4f, 0.f, 
	    .00383994f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
	     0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, .00676608f, 0.f, 0.f, 0.f,
	     .92488f, .0741986f, -.00637629f, .00600575f, .00129382f, 
	    .0069755f, -.00170782f, .00280584f, -.00887214f, -.0435703f, 0.f, 
	    0.f, 0.f, 4.31515f, 0.f, -.0181474f, -.0606627f, -84.3503f, 
	    .00846944f, 0.f, 0.f, 0.f, -.0217081f, -.002195f, .132397f, 
	    .213315f, 0.f, 0.f, 0.f, 0.f, 0.f, 247.58f, .00441585f, 
	    .00780466f, 0.f, 0.f, 0.f, 0.f, 0.f, 6.44155e-4f, -.00249166f, 
	    .00290482f, -3.40501e-4f, .113139f, .169134f, -.0060146f, 
	    -.00163368f, 0.f, -.0043134f, 0.f, .00453979f, 0.f, 0.f, 0.f, 0.f,
	     0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, -.0054366f, 0.f, 0.f, 0.f, .972669f, -.0426748f, 
	    .0112876f, -.00844951f, .00704114f, .0126036f, -.00388164f, 
	    -5.20509e-4f, -6.0971e-4f, .131603f, .113804f, 0.f, 0.f, -6.1597f,
	     0.f, -.0214214f, -.0662913f, -.202884f, .023535f, 0.f, 0.f, 0.f, 
	    .0113573f, -.00184905f, .132397f, .213315f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 1.42645f, -.00264405f, -5.57771e-4f, 0.f, 0.f, 0.f, 0.f, 
	    -22.0621f, -.00110313f, 3.97063e-5f, 5.47632e-5f, .00357577f, 
	    .113139f, .169134f, 0.f, .00118897f, 0.f, 7.62305e-4f, 0.f, 
	    -.00352015f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, -9.5255e-4f, 8.56253e-4f, 4.33114e-4f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, .00121223f, 2.38694e-4f, 9.15245e-4f, 
	    .00128385f, 8.67668e-4f, -5.61425e-6f, 1.04445f, 34.1112f, 0.f, 
	    -.840704f, -239.639f, .706668f, -20.5873f, -.363696f, 23.9245f, 
	    10.f, -.00106657f, -7.67292e-4f, 1.54534e-4f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, .999368f, .0433893f, -.00207009f, .00109617f, 
	    .0010544f, 4.83408e-4f, 9.7704e-4f, 9.24791e-4f, 4.80247e-4f, 
	    .0494737f, .00105985f, 0.f, 0.f, 2.74409f, 0.f, -.00496656f, 
	    -.0151684f, 46.5158f, -.00751133f, 0.f, 0.f, 0.f, 0.f, 
	    6.63808e-4f, .132397f, .213315f, -.00206652f, -.00632046f, 0.f, 
	    0.f, .00594545f, -190.958f, 0.f, -.00416892f, 0.f, -.0167499f, 
	    0.f, .00258987f, 597.781f, 0.f, 0.f, 4.4489e-4f, 4.66444e-4f, 
	    .113139f, .169134f, 0.f, 7.1136e-4f, .0132186f, .00223948f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, .00160571f, 6.28078e-4f, 5.05469e-5f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, -.00157829f, -4.00855e-4f, 5.04077e-5f, 
	    -.00139001f, -.00233406f, -4.81197e-4f, 1.46758f, 6.20332f, 0.f, 
	    .366476f, -61.976f, .309198f, -19.8999f, 0.f, -329.933f, 0.f, 
	    -.0011008f, -9.3931e-5f, 1.39638e-4f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, .981637f, -.00141317f, .0387323f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, -.0358707f, -.00863658f, 0.f, 0.f, -2.02226f, 0.f, 
	    -.00869424f, -.0191397f, 87.6779f, .00452188f, 0.f, 0.f, 0.f, 0.f,
	     0.f, 0.f, 0.f, -.00707572f, 0.f, 0.f, 0.f, -.0041121f, 35.006f, 
	    0.f, 0.f, 0.f, .022376f, 0.f, -.00836657f, 16.1347f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, -.014513f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, .00124152f,
	     6.43365e-4f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, .00133255f, 
	    .00242657f, .00160666f, -.00185728f, -.00146874f, -4.79163e-6f, 
	    1.22464f, 35.351f, 0.f, .449223f, -47.7466f, .470681f, 8.41861f, 
	    -.288198f, 167.854f, 0.f, 7.11493e-4f, 6.05601e-4f, 0.f, 0.f, 0.f,
	     0.f, 0.f, 0.f, 0.f, 0.f, 1.00422f, -.00711212f, .0052448f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, -.0528914f, -.0241301f, 0.f, 0.f, 
	    -21.2219f, 0.f, -.00328077f, .0165727f, 1.68564f, -.00668154f, 
	    0.f, 0.f, 0.f, .00842365f, 0.f, 0.f, 0.f, -.00434645f, -.010383f, 
	    -.00808279f, .021678f, 0.f, -138.459f, 0.f, 0.f, 0.f, .0145155f, 
	    0.f, .00704573f, -47.3204f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, .0108767f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 5.21769e-4f, -2.27387e-4f, 0.f, 0.f,
	     0.f, 0.f, 0.f, 0.f, 0.f, 0.f, .00326769f, .00316901f, 
	    4.60316e-4f, -1.01431e-4f, .00102131f, 9.96601e-4f, 1.25707f, 
	    25.0114f, 0.f, .424472f, -27.7655f, .344625f, 27.5412f, 0.f, 
	    794.251f, 0.f, .00245835f, .00138871f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, 1.0189f, -.0246603f, .0100078f, 0.f, 0.f, 0.f, 0.f,
	     0.f, 0.f, -.0670977f, -.0402286f, 0.f, 0.f, -22.9466f, 0.f, 
	    .0022658f, .0263931f, 37.2625f, -.00639041f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, -.00185291f, -.00747019f, -.00707265f, 0.f, 0.f, 
	    139.717f, 0.f, 0.f, 0.f, .00958383f, 0.f, .00919771f, -369.121f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, -.0157067f, 0.f, 0.f, 0.f,
	     0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    -.00292953f, -.00277739f, -4.40092e-4f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, .0024728f, 2.95035e-4f, -.00181246f, .00281945f, 
	    .00427296f, 9.78863e-4f, 1.40545f, -6.19173f, 0.f, 0.f, -79.3632f,
	     .444643f, -403.085f, 0.f, 11.5603f, 0.f, .00225068f, 8.48557e-4f,
	     -2.98493e-4f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, .975801f, 
	    .038068f, -.0305198f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, .0385575f, 
	    .0504057f, 0.f, 0.f, -176.046f, 0.f, -.00148297f, -.0036856f, 
	    30.2185f, -.00323338f, 0.f, 0.f, 0.f, -.0115558f, 0.f, 0.f, 0.f, 
	    .0048962f, .0144594f, .00991215f, -.0100616f, -.00821324f, 
	    -157.757f, 0.f, 0.f, 0.f, .0153569f, 0.f, .00663564f, 45.841f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, -.025128f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    -8.73148e-4f, -.00129648f, -7.32026e-5f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, -.0046811f, -.00466003f, -.00131567f, -7.3939e-4f, 
	    6.32499e-4f, -4.65588e-4f, -1.29785f, -157.139f, 0.f, .25835f, 
	    -36.9453f, .410672f, 9.78196f, -.152064f, -3850.84f, 0.f, 
	    -8.52706e-4f, -.00140945f, -7.26786e-4f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, .960722f, .0703757f, -.0300266f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, .0222671f, .0410423f, 0.f, 0.f, -163.07f, 0.f, 
	    5.40747e-4f, .00779481f, 144.908f, 1.51484e-4f, 0.f, 0.f, 0.f, 
	    -.0141844f, 0.f, 0.f, 0.f, .00577884f, .0106073f, .00536685f, 
	    .00974319f, 0.f, -2880.15f, 0.f, 0.f, 0.f, .0197547f, 0.f, 
	    -.00444902f, -29.276f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    .0234419f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, -4.65325e-4f, -5.50628e-4f, 3.31465e-4f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, -.00206179f, -.00308575f, 
	    -7.93589e-4f, -1.08629e-4f, 5.95511e-4f, -9.0505e-4f, 1.18997f, 
	    41.5924f, 0.f, -.472064f, -947.15f, .398723f, 19.8304f, 0.f, 
	    3732.19f, 0.f, -.0015004f, -.00114933f, -1.56769e-4f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, 1.03123f, -.0705124f, .00871615f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, -.0382621f, -.00980975f, 0.f, 0.f, 
	    28.9286f, 0.f, 0.f, 0.f, 86.6153f, 7.91938e-4f, 0.f, 0.f, 0.f, 
	    .00468917f, 0.f, 0.f, 0.f, .00786638f, .00957341f, .00572268f, 
	    .00990827f, 0.f, 65.5573f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, -40.02f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, .00707457f, 0.f, 0.f, 0.f,
	     0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    -2.0497e-4f, .0012156f, -8.05579e-6f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, -.00249941f, -4.57256e-4f, -1.59311e-4f, 2.96481e-4f, 
	    -.00177318f, -6.37918e-4f, 1.02395f, 12.8172f, 0.f, .149903f, 
	    -26.3818f, 0.f, 47.0628f, -.222139f, .0482292f, 0.f, -8.67075e-4f,
	     -5.86479e-4f, 5.32462e-4f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    1.00828f, -.0910404f, -.0226549f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    -.023242f, -.00908925f, 0.f, 0.f, 33.6105f, 0.f, 0.f, 0.f, 
	    -12.4957f, -.00587939f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, 27.9765f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    2012.37f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, -.0175553f, 0.f,
	     0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, .00329699f, .00126659f, 2.68402e-4f, 0.f, 0.f, 0.f, 0.f,
	     0.f, 0.f, 0.f, .00117894f, .00148746f, 1.06478e-4f, 1.34743e-4f, 
	    -.00220939f, -6.23523e-4f, .636539f, 11.3621f, 0.f, -.393777f, 
	    2386.87f, 0.f, 661.865f, -.121434f, 9.27608f, 0.f, 1.68478e-4f, 
	    .00124892f, .00171345f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    1.57293f, -.6784f, .6475f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    -.0762974f, -.360423f, 0.f, 0.f, 128.358f, 0.f, 0.f, 0.f, 
	    46.8038f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, -.167898f, 0.f, 
	    0.f, 0.f, 0.f, 29099.4f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 31.5706f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, .866492f, .355807f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, -1.12111f, 0.f, 0.f, 0.f, 
	    1.82458f, 0.f, 0.f, 0.f, 101.024f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 654.251f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    -.0156959f, .0191001f, .0315971f, .0100982f, -.00671565f, 
	    .00257693f, 1.38692f, .282132f, 0.f, 0.f, 381.511f, 0.f, 0.f, 0.f,
	     0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    1.06029f, -.0525231f, .373034f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    .0331072f, -.388409f, 0.f, 0.f, -165.295f, 0.f, -.0438916f, 
	    -.322716f, -88.2393f, .118458f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
	     -.119782f, -.213801f, 0.f, 0.f, 0.f, 26.2229f, 0.f, 0.f, 0.f, 
	    -.435863f, 0.f, 0.f, -53.7443f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
	     0.f, -.455788f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
	     0.f, 0.f, 0.f, 0.f, 0.f, 0.f, .0384009f, .0396733f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, .0505494f, .0739617f, .01922f, 
	    -.00846151f, -.0134244f, .0196338f, 1.50421f, 18.8368f, 0.f, 0.f, 
	    -51.3114f, 0.f, 0.f, 0.f, 0.f, 0.f, .0511923f, .0361225f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f };

struct {
    real e_1[10];
    } mavg6_ = { 261.f, 264.f, 229.f, 217.f, 217.f, 223.f, 286.76f, -2.9394f, 
	    2.5f, 0.f };

struct {
    integer e_1;
    } metsel_90 = { 0 };


/* Table of constant values */

static integer c__1 = 1;
static real c_b14 = 1.f;
static real c_b15 = 0.f;
static integer c__48 = 48;
static integer c__2 = 2;
static real c_b32 = 1e3f;
static real c_b37 = 28.f;
static real c_b39 = -1.f;
static real c_b48 = 4.f;
static real c_b49 = -.4f;
static real c_b51 = -1.4f;
static real c_b54 = 16.f;
static real c_b60 = 32.f;
static real c_b66 = 40.f;
static real c_b78 = 14.f;
static doublereal c_b99 = .5;
static real c_b109 = 2.f;

/* Subroutine */ int gtd6_(integer *iyd, real *sec, real *alt, real *glat, 
	real *glong, real *stl, real *f107a, real *f107, real *ap, integer *
	mass, real *d__, real *t)
{
    /* Initialized data */

    static integer mn3 = 5;
    static real zn3[5] = { 32.5f,20.f,15.f,10.f,0.f };
    static integer mn2 = 4;
    static real zn2[4] = { 72.5f,55.f,45.f,32.5f };
    static real zmix = 62.5f;
    static real alast = 99999.f;
    static integer mssl = -999;
    static real sv[25] = { 1.f,1.f,1.f,1.f,1.f,1.f,1.f,1.f,1.f,1.f,1.f,1.f,
	    1.f,1.f,1.f,1.f,1.f,1.f,1.f,1.f,1.f,1.f,1.f,1.f,1.f };

    /* System generated locals */
    real r__1;

    /* Local variables */
    static integer i__, j;
    static real d6[8], v1, t6[2], tz, dmc, dmr, dz28, xmm;
    static integer mss;
    extern /* Subroutine */ int gts6_(integer *, real *, real *, real *, real 
	    *, real *, real *, real *, real *, integer *, real *, real *);
    static real dm28m, altt, xlat;
    extern doublereal vtst_(integer *, real *, real *, real *, real *, real *,
	     real *, real *, integer *);
    extern /* Subroutine */ int glatf_90(real *, real *, real *);
    extern doublereal densm_90(real *, real *, real *, real *, integer *, real *
	    , real *, real *, integer *, real *, real *, real *), glob6s_(
	    real *);
    /* Subroutine */ int tselec_90(real *);

/*        Neutral Atmosphere Empirical Model from the surface to lower */
/*          exosphere  MSISE90 (JGR, 96, 1159-1172, 1991) */
/*         A.E.Hedin 4/24/90;6/3/91(add SAVE) */
/*         2/11/93 correct switch initialization and mks calculation */
/*       2/11/97 [AEH] CORRECT ERROR IN GHP6 WHEN USING METER6(.TRUE.) */
/*           See subroutine GHP6 to specify a pressure rather than */
/*           altitude. */
/*     INPUT: */
/*        IYD - YEAR AND DAY AS YYDDD or DDD (day of year from 1 to 365) */
/*        SEC - UT(SEC) */
/*        ALT - ALTITUDE(KM) */
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
/*            F107, F107A, and AP effects are not large below 80 km */
/*            and these can be set to 150., 150., and 4. respectively. */
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

/*      TO GET OUTPUT IN M-3 and KG/M3:   CALL METER6(.TRUE.) */

/*      O, H, and N set to zero below 72.5 km */
/*      Exospheric temperature set to average for altitudes below 120 km. */

/*           The following is for test and special purposes: */
/*            TO TURN ON AND OFF PARTICULAR VARIATIONS CALL TSELEC(SW) */
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
/*              18 - ALL TN1 VAR           19 - ALL S VAR */
/*              20 - ALL TN2 VAR           21 - ALL NLB VAR */
/*              22 - ALL TN3 VAR           23 - TURBO SCALE HEIGHT VAR */

/*              To get current values of SW: CALL TRETRV(SW) */

/* XX      COMMON/DATIM6/ISD(3),IST(2),NAM(2) */
/* XX      COMMON/DATIME/ISDATE(3),ISTIME(2),NAME(2) */
    /* Parameter adjustments */
    --t;
    --d__;
    --ap;

    /* Function Body */
    if (csw_1.isw != 64999) {
	tselec_90(sv);
    }
/*      Put identification data into common/datime/ */
    for (i__ = 1; i__ <= 3; ++i__) {
/* XX        ISDATE(I)=ISD(I) */
/* L1: */
    }
    for (i__ = 1; i__ <= 2; ++i__) {
/* XX        ISTIME(I)=IST(I) */
/* XX        NAME(I)=NAM(I) */
/* L2: */
    }

/* e        Test for changed input */
    v1 = vtst_(iyd, sec, glat, glong, stl, f107a, f107, &ap[1], &c__1);
/*       Latitude variation of gravity (none for SW(2)=0) */
    xlat = *glat;
    if (csw_1.sw[1] == 0.f) {
	xlat = 45.f;
    }
    glatf_90(&xlat, &parmb_1.gsurf, &parmb_1.re);

    xmm = lower6_1.pdm[24];

/*       THERMOSPHERE/UPPER MESOSPHERE [above ZN2(1)] */
    altt = dmax(*alt,zn2[0]);
    mss = *mass;
/* e       Only calculate N2 in thermosphere if alt in mixed region */
    if (*alt < zmix && *mass > 0) {
	mss = 28;
    }
/* e       Only calculate thermosphere if input parameters changed */
/* e         or altitude above ZN2(1) in mesosphere */
    if (v1 == 1.f || *alt > zn2[0] || alast > zn2[0] || mss != mssl) {
	gts6_(iyd, sec, &altt, glat, glong, stl, f107a, f107, &ap[1], &mss, 
		d6, t6);
	dm28m = dmix_1.dm28;
/*         metric adjustment */
	if (metsel_1.imr == 1) {
	    dm28m = dmix_1.dm28 * 1e6f;
	}
	mssl = mss;
    }
    t[1] = t6[0];
    t[2] = t6[1];
    if (*alt >= zn2[0]) {
	for (j = 1; j <= 8; ++j) {
	    d__[j] = d6[j - 1];
/* L5: */
	}
	goto L10;
    }

/*       LOWER MESOSPHERE/UPPER STRATOSPHERE [between ZN3(1) and ZN2(1)] */
/*         Temperature at nodes and gradients at end nodes */
/*         Inverse temperature a linear function of spherical harmonics */
/* e        Only calculate nodes if input changed */
    if (v1 == 1.f || alast >= zn2[0]) {
	meso6_1.tgn2[0] = meso6_1.tgn1[1];
	meso6_1.tn2[0] = meso6_1.tn1[4];
	meso6_1.tn2[1] = parm6_1.pma[0] * mavg6_1.pavgm[0] / (1.f - csw_1.sw[
		19] * glob6s_(parm6_1.pma));
	meso6_1.tn2[2] = parm6_1.pma[100] * mavg6_1.pavgm[1] / (1.f - 
		csw_1.sw[19] * glob6s_(&parm6_1.pma[100]));
	meso6_1.tn2[3] = parm6_1.pma[200] * mavg6_1.pavgm[2] / (1.f - 
		csw_1.sw[19] * csw_1.sw[21] * glob6s_(&parm6_1.pma[200]));
/* Computing 2nd power */
	r__1 = parm6_1.pma[200] * mavg6_1.pavgm[2];
	meso6_1.tgn2[1] = mavg6_1.pavgm[8] * parm6_1.pma[900] * (csw_1.sw[19] 
		* csw_1.sw[21] * glob6s_(&parm6_1.pma[900]) + 1.f) * 
		meso6_1.tn2[3] * meso6_1.tn2[3] / (r__1 * r__1);
	meso6_1.tn3[0] = meso6_1.tn2[3];
    }
    if (*alt >= zn3[0]) {
	goto L6;
    }

/*       LOWER STRATOSPHERE AND TROPOSPHERE [below ZN3(1)] */
/*         Temperature at nodes and gradients at end nodes */
/*         Inverse temperature a linear function of spherical harmonics */
/* e        Only calculate nodes if input changed */
    if (v1 == 1.f || alast >= zn3[0]) {
	meso6_1.tgn3[0] = meso6_1.tgn2[1];
	meso6_1.tn3[1] = parm6_1.pma[300] * mavg6_1.pavgm[3] / (1.f - 
		csw_1.sw[21] * glob6s_(&parm6_1.pma[300]));
	meso6_1.tn3[2] = parm6_1.pma[400] * mavg6_1.pavgm[4] / (1.f - 
		csw_1.sw[21] * glob6s_(&parm6_1.pma[400]));
	meso6_1.tn3[3] = parm6_1.pma[500] * mavg6_1.pavgm[5] / (1.f - 
		csw_1.sw[21] * glob6s_(&parm6_1.pma[500]));
	meso6_1.tn3[4] = parm6_1.pma[600] * mavg6_1.pavgm[6] / (1.f - 
		csw_1.sw[21] * glob6s_(&parm6_1.pma[600]));
/* Computing 2nd power */
	r__1 = parm6_1.pma[600] * mavg6_1.pavgm[6];
	meso6_1.tgn3[1] = parm6_1.pma[700] * mavg6_1.pavgm[7] * (csw_1.sw[21] 
		* glob6s_(&parm6_1.pma[700]) + 1.f) * meso6_1.tn3[4] * 
		meso6_1.tn3[4] / (r__1 * r__1);
    }
L6:
    if (*mass == 0) {
	goto L50;
    }
/* e          Linear transition to full mixing at ZMIX from almost */
/* e            full mixing at ZN2(1) to improve efficiency */
    dmc = 0.f;
    if (*alt > zmix) {
	dmc = 1.f - (zn2[0] - *alt) / (zn2[0] - zmix);
    }
    dz28 = d6[2];
/*      ***** N2 DENSITY **** */
    dmr = d6[2] / dm28m - 1.f;
    d__[3] = densm_90(alt, &dm28m, &xmm, &tz, &mn3, zn3, meso6_1.tn3, 
	    meso6_1.tgn3, &mn2, zn2, meso6_1.tn2, meso6_1.tgn2);
    d__[3] *= dmr * dmc + 1.f;
/*      ***** HE DENSITY **** */
    d__[1] = 0.f;
    if (*mass != 4 && *mass != 48) {
	goto L204;
    }
    dmr = d6[0] / (dz28 * lower6_1.pdm[1]) - 1.f;
    d__[1] = d__[3] * lower6_1.pdm[1] * (dmr * dmc + 1.f);
L204:
/*      **** O DENSITY **** */
    d__[2] = 0.f;
/* L216: */
/*      ***** O2 DENSITY **** */
    d__[4] = 0.f;
    if (*mass != 32 && *mass != 48) {
	goto L232;
    }
    dmr = d6[3] / (dz28 * lower6_1.pdm[31]) - 1.f;
    d__[4] = d__[3] * lower6_1.pdm[31] * (dmr * dmc + 1.f);
L232:
/*      ***** AR DENSITY **** */
    d__[5] = 0.f;
    if (*mass != 40 && *mass != 48) {
	goto L240;
    }
    dmr = d6[4] / (dz28 * lower6_1.pdm[41]) - 1.f;
    d__[5] = d__[3] * lower6_1.pdm[41] * (dmr * dmc + 1.f);
L240:
/*      ***** HYDROGEN DENSITY **** */
    d__[7] = 0.f;
/*      ***** ATOMIC NITROGEN DENSITY **** */
    d__[8] = 0.f;

/*       TOTAL MASS DENSITY */

    if (*mass == 48) {
	d__[6] = (d__[1] * 4.f + d__[2] * 16.f + d__[3] * 28.f + d__[4] * 
		32.f + d__[5] * 40.f + d__[7] + d__[8] * 14.f) * 1.66e-24f;
	if (metsel_1.imr == 1) {
	    d__[6] /= 1e3f;
	}
    }
    t[2] = tz;
L10:
    goto L90;
L50:
    gts3c_1.dd = densm_90(alt, &c_b14, &c_b15, &tz, &mn3, zn3, meso6_1.tn3, 
	    meso6_1.tgn3, &mn2, zn2, meso6_1.tn2, meso6_1.tgn2);
    t[2] = tz;
L90:
    alast = *alt;
    return 0;
} /* gtd6_ */

/* ----------------------------------------------------------------------- */
/* Subroutine */ int ghp6_(integer *iyd, real *sec, real *alt, real *glat, 
	real *glong, real *stl, real *f107a, real *f107, real *ap, real *d__, 
	real *t, real *press)
{
    /* Initialized data */

    static real bm = 1.3806e-19f;
    static real rgas = 831.4f;
    static real test = 4.3e-4f;

    /* System generated locals */
    real r__1;

    /* Builtin functions */
//    double r_lg10(real *);

    /* Local variables */
    static real g;
    static integer l;
    static real p, z__, ca, cd, cl, sh, pl, zi, xm, xn, cl2;
    extern /* Subroutine */ int gtd6_(integer *, real *, real *, real *, real 
	    *, real *, real *, real *, real *, integer *, real *, real *);
    static real diff;
    static integer iday;

/*       FIND ALTITUDE OF PRESSURE SURFACE (PRESS) FROM GTD6 */
/*       2/11/97 [AEH] CORRECT ERROR IN GHP6 WHEN USING METER6(.TRUE.) */
/*     INPUT: */
/*        IYD - YEAR AND DAY AS YYDDD */
/*        SEC - UT(SEC) */
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
/*        PRESS - PRESSURE LEVEL(MB) */
/*     OUTPUT: */
/*        ALT - ALTITUDE(KM) */
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

    /* Parameter adjustments */
    --t;
    --d__;
    --ap;

    /* Function Body */
    pl = log10(*press);
/*      Initial altitude estimate */
    if (pl >= -5.f) {
	if (pl > 2.5f) {
	    zi = (3.f - pl) * 18.06f;
	}
	if (pl > .75f && pl <= 2.5f) {
	    zi = (3.08f - pl) * 14.98f;
	}
	if (pl > -1.f && pl <= .75f) {
	    zi = (2.72f - pl) * 17.8f;
	}
	if (pl > -2.f && pl <= -1.f) {
	    zi = (3.64f - pl) * 14.28f;
	}
	if (pl > -4.f && pl <= -2.f) {
	    zi = (4.32f - pl) * 12.72f;
	}
	if (pl <= -4.f) {
	    zi = (.11f - pl) * 25.3f;
	}
	iday = *iyd % 1000;
	cl = *glat / 90.f;
	cl2 = cl * cl;
	if (iday < 182) {
	    cd = 1.f - iday / 91.25f;
	}
	if (iday >= 182) {
	    cd = iday / 91.25f - 3.f;
	}
	ca = 0.f;
	if (pl > -1.11f && pl <= -.23f) {
	    ca = 1.f;
	}
	if (pl > -.23f) {
	    ca = (2.79f - pl) / 3.02f;
	}
	if (pl <= -1.11f && pl > -3.f) {
	    ca = (-2.93f - pl) / -1.8200000000000001f;
	}
	z__ = zi - cl * 4.87f * cd * ca - cl2 * 1.64f * ca + ca * .31f * cl;
    }
    if (pl < -5.f) {
/* Computing 2nd power */
	r__1 = pl + 4.f;
	z__ = r__1 * r__1 * 22.f + 110;
    }
    l = 0;
/*      ITERATION LOOP */
L10:
    ++l;
    gtd6_(iyd, sec, &z__, glat, glong, stl, f107a, f107, &ap[1], &c__48, &d__[
	    1], &t[1]);
    xn = d__[1] + d__[2] + d__[3] + d__[4] + d__[5] + d__[7] + d__[8];
    p = bm * xn * t[2];
    if (metsel_1.imr == 1) {
	p *= 1e-6f;
    }
    diff = pl - log10(p);
    if (dabs(diff) < test || l == 6) {
	goto L20;
    }
    xm = d__[6] / xn / 1.66e-24f;
    if (metsel_1.imr == 1) {
	xm *= 1e3f;
    }
/* Computing 2nd power */
    r__1 = z__ / parmb_1.re + 1.f;
    g = parmb_1.gsurf / (r__1 * r__1);
    sh = rgas * t[2] / (xm * g);
/*         New altitude estimate using scale height */
    z__ -= sh * diff * 2.302f;
    goto L10;
L20:
/* XX      IF(L.EQ.6) WRITE(6,100) PRESS,DIFF */
/* XX  100 FORMAT(1X,29HGHP6 NOT CONVERGING FOR PRESS,1PE12.2,E12.2) */
    *alt = z__;
    return 0;
} /* ghp6_ */

/* ----------------------------------------------------------------------- */
/* Subroutine */ int glatf_90(real *lat, real *gv, real *reff)
{
    /* Initialized data */

    static real dgtr = .0174533f;
    static real latl = -999.f;

    /* Builtin functions */
//    double cos(doublereal);

    /* Local variables */
    static real c2;

/*      CALCULATE LATITUDE VARIABLE GRAVITY (GV) AND EFFECTIVE */
/*      RADIUS (REFF) */
    if (*lat != latl) {
	c2 = cos(2.f * dgtr * *lat);
    }
    latl = *lat;
    *gv = (1.f - c2 * .0026373f) * 980.616f;
    *reff = *gv * 2.f / (c2 * 2.27e-9f + 3.085462e-6f) * 1e-5f;
    return 0;
} /* glatf_90 */

/* ----------------------------------------------------------------------- */
doublereal vtst_(integer *iyd, real *sec, real *glat, real *glong, real *stl, 
	real *f107a, real *f107, real *ap, integer *ic)
{
    /* Initialized data */

    static integer iydl[2] = { -999,-999 };
    static real secl[2] = { -999.f,-999.f };
    static real glatl[2] = { -999.f,-999.f };
    static real gll[2] = { -999.f,-999.f };
    static real stll[2] = { -999.f,-999.f };
    static real fal[2] = { -999.f,-999.f };
    static real fl[2] = { -999.f,-999.f };
    static real apl[14]	/* was [7][2] */ = { -999.f,-999.f,-999.f,-999.f,
	    -999.f,-999.f,-999.f,-999.f,-999.f,-999.f,-999.f,-999.f,-999.f,
	    -999.f };
    static real swl[50]	/* was [25][2] */ = { -999.f,-999.f,-999.f,-999.f,
	    -999.f,-999.f,-999.f,-999.f,-999.f,-999.f,-999.f,-999.f,-999.f,
	    -999.f,-999.f,-999.f,-999.f,-999.f,-999.f,-999.f,-999.f,-999.f,
	    -999.f,-999.f,-999.f,-999.f,-999.f,-999.f,-999.f,-999.f,-999.f,
	    -999.f,-999.f,-999.f,-999.f,-999.f,-999.f,-999.f,-999.f,-999.f,
	    -999.f,-999.f,-999.f,-999.f,-999.f,-999.f,-999.f,-999.f,-999.f,
	    -999.f };
    static real swcl[50]	/* was [25][2] */ = { -999.f,-999.f,-999.f,
	    -999.f,-999.f,-999.f,-999.f,-999.f,-999.f,-999.f,-999.f,-999.f,
	    -999.f,-999.f,-999.f,-999.f,-999.f,-999.f,-999.f,-999.f,-999.f,
	    -999.f,-999.f,-999.f,-999.f,-999.f,-999.f,-999.f,-999.f,-999.f,
	    -999.f,-999.f,-999.f,-999.f,-999.f,-999.f,-999.f,-999.f,-999.f,
	    -999.f,-999.f,-999.f,-999.f,-999.f,-999.f,-999.f,-999.f,-999.f,
	    -999.f,-999.f };

    /* System generated locals */
    real ret_val;

    /* Local variables */
    static integer i__;

/*       Test if geophysical variables or switches changed and save */
/*       Return 0 if unchanged and 1 if changed */
    /* Parameter adjustments */
    --ap;

    /* Function Body */
    ret_val = 0.f;
    if (*iyd != iydl[*ic - 1]) {
	goto L10;
    }
    if (*sec != secl[*ic - 1]) {
	goto L10;
    }
    if (*glat != glatl[*ic - 1]) {
	goto L10;
    }
    if (*glong != gll[*ic - 1]) {
	goto L10;
    }
    if (*stl != stll[*ic - 1]) {
	goto L10;
    }
    if (*f107a != fal[*ic - 1]) {
	goto L10;
    }
    if (*f107 != fl[*ic - 1]) {
	goto L10;
    }
    for (i__ = 1; i__ <= 7; ++i__) {
	if (ap[i__] != apl[i__ + *ic * 7 - 8]) {
	    goto L10;
	}
/* L5: */
    }
    for (i__ = 1; i__ <= 25; ++i__) {
	if (csw_1.sw[i__ - 1] != swl[i__ + *ic * 25 - 26]) {
	    goto L10;
	}
	if (csw_1.swc[i__ - 1] != swcl[i__ + *ic * 25 - 26]) {
	    goto L10;
	}
/* L7: */
    }
    goto L20;
L10:
    ret_val = 1.f;
    iydl[*ic - 1] = *iyd;
    secl[*ic - 1] = *sec;
    glatl[*ic - 1] = *glat;
    gll[*ic - 1] = *glong;
    stll[*ic - 1] = *stl;
    fal[*ic - 1] = *f107a;
    fl[*ic - 1] = *f107;
    for (i__ = 1; i__ <= 7; ++i__) {
	apl[i__ + *ic * 7 - 8] = ap[i__];
/* L15: */
    }
    for (i__ = 1; i__ <= 25; ++i__) {
	swl[i__ + *ic * 25 - 26] = csw_1.sw[i__ - 1];
	swcl[i__ + *ic * 25 - 26] = csw_1.swc[i__ - 1];
/* L16: */
    }
L20:
    return ret_val;
} /* vtst_ */

/* ----------------------------------------------------------------------- */
/* Subroutine */ int gts6_0_(int n__, integer *iyd, real *sec, real *alt, 
	real *glat, real *glong, real *stl, real *f107a, real *f107, real *ap,
	 integer *mass, real *d__, real *t, logical *meter)
{
    /* Initialized data */

    static integer mt[10] = { 48,0,4,16,28,32,40,1,49,14 };
    static real altl[8] = { 200.f,400.f,160.f,200.f,240.f,450.f,320.f,450.f };
    static integer mn1 = 5;
    static real zn1[5] = { 120.f,110.f,100.f,90.f,72.5f };
    static real dgtr = .0174533f;
    static real dr = .0172142f;
    static real alast = -999.f;

    /* System generated locals */
    real r__1;

    /* Builtin functions */
//    double r_mod(real *, real *), sin(doublereal), cos(doublereal), exp(
//	    doublereal), log(doublereal);

    /* Local variables */
    static integer i__, j;
    static real g1, g4, v2, b01, b04, b32, b16, g40, b28, g32, g16, b40, g14, 
	    g28, b14, tz, hc04, hc32, hc16, rc16, day, zc04, zh04, zlb, zhf, 
	    yrd, xmm, zh28, xmd, zh16, zc16, zh32, zc32, zh40, hc40, zc40, 
	    zh01, hc01, zc01, rc01, zh14, hc14, zc14, rc14, hcc01, hcc14, 
	    hcc16, zcc01, zcc14;
    extern doublereal ccor_90(real *, real *, real *, real *);
    static real zcc16, ddum;
    doublereal dnet_90(real *, real *, real *, real *, real *);
    static real zhm01, tinf, zhm04, zhm32, zhm16, zhm40, zhm14, zhm28;
    extern doublereal vtst_(integer *, real *, real *, real *, real *, real *,
	     real *, real *, integer *), densu_90(real *, real *, real *, real *
	    , real *, real *, real *, real *, real *, integer *, real *, real 
	    *, real *), globe6_(real *, real *, real *, real *, real *, real *
	    , real *, real *, real *), glob6s_(real *);

/*        Neutral Thermosphere Model above 72.5 km for MSISE-90 */
/*         A.E.Hedin 3/9/90 */
/*         Coefficients not changed for 120km and above, but results may differ */
/*        by a few percent from MSIS-86 (GTS5) with introduction of a */
/*        latitude dependent accel. of gravity. */
/*         Lower thermosphere reformulated for better continuation into */
/*        lower atmosphere. */
/*        For efficiency: */
/*         Exospheric temperature left at average value for alt below 120km; */
/*         120 km gradient left at average value for alt below 72 km; */
/*     INPUT: */
/*        IYD - YEAR AND DAY AS YYDDD */
/*        SEC - UT(SEC) */
/*        ALT - ALTITUDE(KM) (GREATER THAN 72.5 KM) */
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

/*           The following is for test and special purposes: */
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
/*              18 - ALL TN1 VAR           19 - ALL S VAR */
/*              20 - ALL TN2 VAR           21 - ALL NLB VAR */
/*              22 - ALL TN3 VAR           23 - TURBO SCALE HEIGHT VAR */

/*              To get current values of SW: CALL TRETRV(SW) */

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
	case 1: goto L_meter6;
	}

/* e        Test for changed input */
    v2 = vtst_(iyd, sec, glat, glong, stl, f107a, f107, &ap[1], &c__2);

    yrd = (real) (*iyd);
    gts3c_1.za = parm6_1.pdl[40];
    zn1[0] = gts3c_1.za;
    for (j = 1; j <= 8; ++j) {
	d__[j] = 0.f;
/* L2: */
    }
/* e       TINF VARIATIONS NOT IMPORTANT BELOW ZA OR ZN1(1) */
    if (*alt > zn1[0]) {
	if (v2 == 1.f || alast <= zn1[0]) {
	    tinf = lower6_1.ptm[0] * parm6_1.pt[0] * (csw_1.sw[15] * globe6_(&
		    yrd, sec, glat, glong, stl, f107a, f107, &ap[1], 
		    parm6_1.pt) + 1.f);
	}
    } else {
	tinf = lower6_1.ptm[0] * parm6_1.pt[0];
    }
    t[1] = tinf;
/* e         GRADIENT VARIATIONS NOT IMPORTANT BELOW ZN1(5) */
    if (*alt > zn1[4]) {
	if (v2 == 1.f || alast <= zn1[4]) {
	    gts3c_1.g0 = lower6_1.ptm[3] * parm6_1.ps[0] * (csw_1.sw[18] * 
		    globe6_(&yrd, sec, glat, glong, stl, f107a, f107, &ap[1], 
		    parm6_1.ps) + 1.f);
	}
    } else {
	gts3c_1.g0 = lower6_1.ptm[3] * parm6_1.ps[0];
    }
/* e      Calculate these temperatures only if input changed */
    if (v2 == 1.f) {
	gts3c_1.tlb = lower6_1.ptm[1] * (csw_1.sw[16] * globe6_(&yrd, sec, 
		glat, glong, stl, f107a, f107, &ap[1], &parm6_1.pd[450]) + 
		1.f) * parm6_1.pd[450];
    }
    gts3c_1.s = gts3c_1.g0 / (tinf - gts3c_1.tlb);
/* e       Lower thermosphere temp variations not significant for */
/* e        density above 300 km */
    if (*alt < 300.f) {
	if (v2 == 1.f || alast >= 300.f) {
	    meso6_1.tn1[1] = lower6_1.ptm[6] * parm6_1.ptl[0] / (1.f - 
		    csw_1.sw[17] * glob6s_(parm6_1.ptl));
	    meso6_1.tn1[2] = lower6_1.ptm[2] * parm6_1.ptl[100] / (1.f - 
		    csw_1.sw[17] * glob6s_(&parm6_1.ptl[100]));
	    meso6_1.tn1[3] = lower6_1.ptm[7] * parm6_1.ptl[200] / (1.f - 
		    csw_1.sw[17] * glob6s_(&parm6_1.ptl[200]));
	    meso6_1.tn1[4] = lower6_1.ptm[4] * parm6_1.ptl[300] / (1.f - 
		    csw_1.sw[17] * csw_1.sw[19] * glob6s_(&parm6_1.ptl[300]));
/* Computing 2nd power */
	    r__1 = lower6_1.ptm[4] * parm6_1.ptl[300];
	    meso6_1.tgn1[1] = lower6_1.ptm[8] * parm6_1.pma[800] * (csw_1.sw[
		    17] * csw_1.sw[19] * glob6s_(&parm6_1.pma[800]) + 1.f) * 
		    meso6_1.tn1[4] * meso6_1.tn1[4] / (r__1 * r__1);
	}
    } else {
	meso6_1.tn1[1] = lower6_1.ptm[6] * parm6_1.ptl[0];
	meso6_1.tn1[2] = lower6_1.ptm[2] * parm6_1.ptl[100];
	meso6_1.tn1[3] = lower6_1.ptm[7] * parm6_1.ptl[200];
	meso6_1.tn1[4] = lower6_1.ptm[4] * parm6_1.ptl[300];
/* Computing 2nd power */
	r__1 = lower6_1.ptm[4] * parm6_1.ptl[300];
	meso6_1.tgn1[1] = lower6_1.ptm[8] * parm6_1.pma[800] * meso6_1.tn1[4] 
		* meso6_1.tn1[4] / (r__1 * r__1);
    }

    gts3c_1.z0 = zn1[3];
    gts3c_1.t0 = meso6_1.tn1[3];
    zlb = lower6_1.ptm[5];
    gts3c_1.tr12 = 1.f;

    if (*mass == 0) {
	goto L50;
    }
/*       N2 variation factor at Zlb */
    g28 = csw_1.sw[20] * globe6_(&yrd, sec, glat, glong, stl, f107a, f107, &
	    ap[1], &parm6_1.pd[300]);
/*        Variation of turbopause height */
    day = mod(yrd, c_b32);
    zhf = parm6_1.pdl[49] * (csw_1.sw[4] * parm6_1.pdl[24] * sin(dgtr * *glat)
	     * cos(dr * (day - parm6_1.pt[13])) + 1.f);

    yrd = (real) (*iyd);
    t[1] = tinf;
    xmm = lower6_1.pdm[24];

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

/*      Diffusive density at Zlb */
    gts3c_1.db28 = lower6_1.pdm[20] * exp(g28) * parm6_1.pd[300];
/*      Diffusive density at Alt */
    d__[3] = densu_90(alt, &gts3c_1.db28, &tinf, &gts3c_1.tlb, &c_b37, &c_b15, &
	    t[2], &zlb, &gts3c_1.s, &mn1, zn1, meso6_1.tn1, meso6_1.tgn1);
    gts3c_1.dd = d__[3];
/*      Turbopause */
    zh28 = lower6_1.pdm[22] * zhf;
    zhm28 = lower6_1.pdm[23] * parm6_1.pdl[30];
    xmd = 28.f - xmm;
/*      Mixed density at Zlb */
    b28 = densu_90(&zh28, &gts3c_1.db28, &tinf, &gts3c_1.tlb, &xmd, &c_b39, &tz,
	     &zlb, &gts3c_1.s, &mn1, zn1, meso6_1.tn1, meso6_1.tgn1);
    if (*alt > altl[2] || csw_1.sw[14] == 0.f) {
	goto L17;
    }
/*      Mixed density at Alt */
    dmix_1.dm28 = densu_90(alt, &b28, &tinf, &gts3c_1.tlb, &xmm, &c_b15, &tz, &
	    zlb, &gts3c_1.s, &mn1, zn1, meso6_1.tn1, meso6_1.tgn1);
/*      Net density at Alt */
    d__[3] = dnet_90(&d__[3], &dmix_1.dm28, &zhm28, &xmm, &c_b37);
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

/*       Density variation factor at Zlb */
    g4 = csw_1.sw[20] * globe6_(&yrd, sec, glat, glong, stl, f107a, f107, &ap[
	    1], parm6_1.pd);
/*      Diffusive density at Zlb */
    gts3c_1.db04 = lower6_1.pdm[0] * exp(g4) * parm6_1.pd[0];
/*      Diffusive density at Alt */
    d__[1] = densu_90(alt, &gts3c_1.db04, &tinf, &gts3c_1.tlb, &c_b48, &c_b49, &
	    t[2], &zlb, &gts3c_1.s, &mn1, zn1, meso6_1.tn1, meso6_1.tgn1);
    gts3c_1.dd = d__[1];
    if (*alt > altl[0] || csw_1.sw[14] == 0.f) {
	goto L24;
    }
/*      Turbopause */
    zh04 = lower6_1.pdm[2];
    zhm04 = zhm28;
/*      Mixed density at Zlb */
    r__1 = 4.f - xmm;
    b04 = densu_90(&zh04, &gts3c_1.db04, &tinf, &gts3c_1.tlb, &r__1, &c_b51, &t[
	    2], &zlb, &gts3c_1.s, &mn1, zn1, meso6_1.tn1, meso6_1.tgn1);
/*      Mixed density at Alt */
    dmix_1.dm04 = densu_90(alt, &b04, &tinf, &gts3c_1.tlb, &xmm, &c_b15, &t[2], 
	    &zlb, &gts3c_1.s, &mn1, zn1, meso6_1.tn1, meso6_1.tgn1);
/*      Net density at Alt */
    d__[1] = dnet_90(&d__[1], &dmix_1.dm04, &zhm04, &xmm, &c_b48);
/*      Correction to specified mixing ratio at ground */
    gts3c_1.rl = log(b28 * lower6_1.pdm[1] / b04);
    zc04 = lower6_1.pdm[4] * parm6_1.pdl[25];
    hc04 = lower6_1.pdm[5] * parm6_1.pdl[26];
/*      Net density corrected at Alt */
    d__[1] *= ccor_90(alt, &gts3c_1.rl, &hc04, &zc04);
L24:
    if (*mass != 48) {
	goto L90;
    }
L25:

/*      **** O DENSITY **** */

/*       Density variation factor at Zlb */
    g16 = csw_1.sw[20] * globe6_(&yrd, sec, glat, glong, stl, f107a, f107, &
	    ap[1], &parm6_1.pd[150]);
/*      Diffusive density at Zlb */
    gts3c_1.db16 = lower6_1.pdm[10] * exp(g16) * parm6_1.pd[150];
/*       Diffusive density at Alt */
    d__[2] = densu_90(alt, &gts3c_1.db16, &tinf, &gts3c_1.tlb, &c_b54, &c_b15, &
	    t[2], &zlb, &gts3c_1.s, &mn1, zn1, meso6_1.tn1, meso6_1.tgn1);
    gts3c_1.dd = d__[2];
    if (*alt > altl[1] || csw_1.sw[14] == 0.f) {
	goto L34;
    }
/*  Corrected from PDM(3,1) to PDM(3,2)  12/2/85 */
/*       Turbopause */
    zh16 = lower6_1.pdm[12];
    zhm16 = zhm28;
/*      Mixed density at Zlb */
    r__1 = 16 - xmm;
    b16 = densu_90(&zh16, &gts3c_1.db16, &tinf, &gts3c_1.tlb, &r__1, &c_b39, &t[
	    2], &zlb, &gts3c_1.s, &mn1, zn1, meso6_1.tn1, meso6_1.tgn1);
/*      Mixed density at Alt */
    dmix_1.dm16 = densu_90(alt, &b16, &tinf, &gts3c_1.tlb, &xmm, &c_b15, &t[2], 
	    &zlb, &gts3c_1.s, &mn1, zn1, meso6_1.tn1, meso6_1.tgn1);
/*      Net density at Alt */
    d__[2] = dnet_90(&d__[2], &dmix_1.dm16, &zhm16, &xmm, &c_b54);
/*       Correction to specified mixing ratio at ground */
    gts3c_1.rl = log(b28 * lower6_1.pdm[11] * dabs(parm6_1.pdl[41]) / b16);
    hc16 = lower6_1.pdm[15] * parm6_1.pdl[28];
    zc16 = lower6_1.pdm[14] * parm6_1.pdl[27];
    d__[2] *= ccor_90(alt, &gts3c_1.rl, &hc16, &zc16);
/*       Chemistry correction */
    hcc16 = lower6_1.pdm[17] * parm6_1.pdl[38];
    zcc16 = lower6_1.pdm[16] * parm6_1.pdl[37];
    rc16 = lower6_1.pdm[13] * parm6_1.pdl[39];
/*      Net density corrected at Alt */
    d__[2] *= ccor_90(alt, &rc16, &hcc16, &zcc16);
L34:
    if (*mass != 48 && *mass != 49) {
	goto L90;
    }
L35:

/*       **** O2 DENSITY **** */

/*       Density variation factor at Zlb */
    g32 = csw_1.sw[20] * globe6_(&yrd, sec, glat, glong, stl, f107a, f107, &
	    ap[1], &parm6_1.pd[600]);
/*      Diffusive density at Zlb */
    gts3c_1.db32 = lower6_1.pdm[30] * exp(g32) * parm6_1.pd[600];
/*       Diffusive density at Alt */
    d__[4] = densu_90(alt, &gts3c_1.db32, &tinf, &gts3c_1.tlb, &c_b60, &c_b15, &
	    t[2], &zlb, &gts3c_1.s, &mn1, zn1, meso6_1.tn1, meso6_1.tgn1);
    if (*mass == 49) {
	gts3c_1.dd += d__[4] * 2.f;
    } else {
	gts3c_1.dd = d__[4];
    }
    if (*alt > altl[3] || csw_1.sw[14] == 0.f) {
	goto L39;
    }
/*       Turbopause */
    zh32 = lower6_1.pdm[32];
    zhm32 = zhm28;
/*      Mixed density at Zlb */
    r__1 = 32.f - xmm;
    b32 = densu_90(&zh32, &gts3c_1.db32, &tinf, &gts3c_1.tlb, &r__1, &c_b39, &t[
	    2], &zlb, &gts3c_1.s, &mn1, zn1, meso6_1.tn1, meso6_1.tgn1);
/*      Mixed density at Alt */
    dmix_1.dm32 = densu_90(alt, &b32, &tinf, &gts3c_1.tlb, &xmm, &c_b15, &t[2], 
	    &zlb, &gts3c_1.s, &mn1, zn1, meso6_1.tn1, meso6_1.tgn1);
/*      Net density at Alt */
    d__[4] = dnet_90(&d__[4], &dmix_1.dm32, &zhm32, &xmm, &c_b60);
/*       Correction to specified mixing ratio at ground */
    gts3c_1.rl = log(b28 * lower6_1.pdm[31] / b32);
    hc32 = lower6_1.pdm[35] * parm6_1.pdl[32];
    zc32 = lower6_1.pdm[34] * parm6_1.pdl[31];
/*      Net density corrected at Alt */
    d__[4] *= ccor_90(alt, &gts3c_1.rl, &hc32, &zc32);
L39:
    if (*mass != 48) {
	goto L90;
    }
L40:

/*       **** AR DENSITY **** */

/*       Density variation factor at Zlb */
    g40 = csw_1.sw[20] * globe6_(&yrd, sec, glat, glong, stl, f107a, f107, &
	    ap[1], &parm6_1.pd[750]);
/*      Diffusive density at Zlb */
    gts3c_1.db40 = lower6_1.pdm[40] * exp(g40) * parm6_1.pd[750];
/*       Diffusive density at Alt */
    d__[5] = densu_90(alt, &gts3c_1.db40, &tinf, &gts3c_1.tlb, &c_b66, &c_b15, &
	    t[2], &zlb, &gts3c_1.s, &mn1, zn1, meso6_1.tn1, meso6_1.tgn1);
    gts3c_1.dd = d__[5];
    if (*alt > altl[4] || csw_1.sw[14] == 0.f) {
	goto L44;
    }
/*       Turbopause */
    zh40 = lower6_1.pdm[42];
    zhm40 = zhm28;
/*      Mixed density at Zlb */
    r__1 = 40.f - xmm;
    b40 = densu_90(&zh40, &gts3c_1.db40, &tinf, &gts3c_1.tlb, &r__1, &c_b39, &t[
	    2], &zlb, &gts3c_1.s, &mn1, zn1, meso6_1.tn1, meso6_1.tgn1);
/*      Mixed density at Alt */
    dmix_1.dm40 = densu_90(alt, &b40, &tinf, &gts3c_1.tlb, &xmm, &c_b15, &t[2], 
	    &zlb, &gts3c_1.s, &mn1, zn1, meso6_1.tn1, meso6_1.tgn1);
/*      Net density at Alt */
    d__[5] = dnet_90(&d__[5], &dmix_1.dm40, &zhm40, &xmm, &c_b66);
/*       Correction to specified mixing ratio at ground */
    gts3c_1.rl = log(b28 * lower6_1.pdm[41] / b40);
    hc40 = lower6_1.pdm[45] * parm6_1.pdl[34];
    zc40 = lower6_1.pdm[44] * parm6_1.pdl[33];
/*      Net density corrected at Alt */
    d__[5] *= ccor_90(alt, &gts3c_1.rl, &hc40, &zc40);
L44:
    if (*mass != 48) {
	goto L90;
    }
L45:

/*        **** HYDROGEN DENSITY **** */

/*       Density variation factor at Zlb */
    g1 = csw_1.sw[20] * globe6_(&yrd, sec, glat, glong, stl, f107a, f107, &ap[
	    1], &parm6_1.pd[900]);
/*      Diffusive density at Zlb */
    gts3c_1.db01 = lower6_1.pdm[50] * exp(g1) * parm6_1.pd[900];
/*       Diffusive density at Alt */
    d__[7] = densu_90(alt, &gts3c_1.db01, &tinf, &gts3c_1.tlb, &c_b14, &c_b49, &
	    t[2], &zlb, &gts3c_1.s, &mn1, zn1, meso6_1.tn1, meso6_1.tgn1);
    gts3c_1.dd = d__[7];
    if (*alt > altl[6] || csw_1.sw[14] == 0.f) {
	goto L47;
    }
/*       Turbopause */
    zh01 = lower6_1.pdm[52];
    zhm01 = zhm28;
/*      Mixed density at Zlb */
    r__1 = 1.f - xmm;
    b01 = densu_90(&zh01, &gts3c_1.db01, &tinf, &gts3c_1.tlb, &r__1, &c_b51, &t[
	    2], &zlb, &gts3c_1.s, &mn1, zn1, meso6_1.tn1, meso6_1.tgn1);
/*      Mixed density at Alt */
    dmix_1.dm01 = densu_90(alt, &b01, &tinf, &gts3c_1.tlb, &xmm, &c_b15, &t[2], 
	    &zlb, &gts3c_1.s, &mn1, zn1, meso6_1.tn1, meso6_1.tgn1);
/*      Net density at Alt */
    d__[7] = dnet_90(&d__[7], &dmix_1.dm01, &zhm01, &xmm, &c_b14);
/*       Correction to specified mixing ratio at ground */
    gts3c_1.rl = log(b28 * lower6_1.pdm[51] * dabs(parm6_1.pdl[42]) / b01);
    hc01 = lower6_1.pdm[55] * parm6_1.pdl[36];
    zc01 = lower6_1.pdm[54] * parm6_1.pdl[35];
    d__[7] *= ccor_90(alt, &gts3c_1.rl, &hc01, &zc01);
/*       Chemistry correction */
    hcc01 = lower6_1.pdm[57] * parm6_1.pdl[44];
    zcc01 = lower6_1.pdm[56] * parm6_1.pdl[43];
    rc01 = lower6_1.pdm[53] * parm6_1.pdl[45];
/*      Net density corrected at Alt */
    d__[7] *= ccor_90(alt, &rc01, &hcc01, &zcc01);
L47:
L48:

/*        **** ATOMIC NITROGEN DENSITY **** */

/*       Density variation factor at Zlb */
    g14 = csw_1.sw[20] * globe6_(&yrd, sec, glat, glong, stl, f107a, f107, &
	    ap[1], &parm6_1.pd[1050]);
/*      Diffusive density at Zlb */
    gts3c_1.db14 = lower6_1.pdm[60] * exp(g14) * parm6_1.pd[1050];
/*       Diffusive density at Alt */
    d__[8] = densu_90(alt, &gts3c_1.db14, &tinf, &gts3c_1.tlb, &c_b78, &c_b15, &
	    t[2], &zlb, &gts3c_1.s, &mn1, zn1, meso6_1.tn1, meso6_1.tgn1);
    gts3c_1.dd = d__[8];
    if (*alt > altl[7] || csw_1.sw[14] == 0.f) {
	goto L49;
    }
/*       Turbopause */
    zh14 = lower6_1.pdm[62];
    zhm14 = zhm28;
/*      Mixed density at Zlb */
    r__1 = 14.f - xmm;
    b14 = densu_90(&zh14, &gts3c_1.db14, &tinf, &gts3c_1.tlb, &r__1, &c_b39, &t[
	    2], &zlb, &gts3c_1.s, &mn1, zn1, meso6_1.tn1, meso6_1.tgn1);
/*      Mixed density at Alt */
    dmix_1.dm14 = densu_90(alt, &b14, &tinf, &gts3c_1.tlb, &xmm, &c_b15, &t[2], 
	    &zlb, &gts3c_1.s, &mn1, zn1, meso6_1.tn1, meso6_1.tgn1);
/*      Net density at Alt */
    d__[8] = dnet_90(&d__[8], &dmix_1.dm14, &zhm14, &xmm, &c_b78);
/*       Correction to specified mixing ratio at ground */
    gts3c_1.rl = log(b28 * lower6_1.pdm[61] * dabs(parm6_1.pdl[2]) / b14);
    hc14 = lower6_1.pdm[65] * parm6_1.pdl[1];
    zc14 = lower6_1.pdm[64] * parm6_1.pdl[0];
    d__[8] *= ccor_90(alt, &gts3c_1.rl, &hc14, &zc14);
/*       Chemistry correction */
    hcc14 = lower6_1.pdm[67] * parm6_1.pdl[4];
    zcc14 = lower6_1.pdm[66] * parm6_1.pdl[3];
    rc14 = lower6_1.pdm[63] * parm6_1.pdl[5];
/*      Net density corrected at Alt */
    d__[8] *= ccor_90(alt, &rc14, &hcc14, &zcc14);
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
/*       TEMPERATURE AT ALTITUDE */
L50:
    ddum = densu_90(alt, &c_b14, &tinf, &gts3c_1.tlb, &c_b15, &c_b15, &t[2], &
	    zlb, &gts3c_1.s, &mn1, zn1, meso6_1.tn1, meso6_1.tgn1);
    goto L90;
L90:
/*       ADJUST DENSITIES FROM CGS TO KGM */
    if (metsel_1.imr == 1) {
	for (i__ = 1; i__ <= 8; ++i__) {
	    d__[i__] *= 1e6f;
/* L95: */
	}
	d__[6] /= 1e3f;
    }
    alast = *alt;
    return 0;
/* L100: */

L_meter6:
    metsel_1.imr = 0;
    if (*meter) {
	metsel_1.imr = 1;
    }
    return 0;
} /* gts6_ */

/* Subroutine */ int gts6_(integer *iyd, real *sec, real *alt, real *glat, 
	real *glong, real *stl, real *f107a, real *f107, real *ap, integer *
	mass, real *d__, real *t)
{
    return gts6_0_(0, iyd, sec, alt, glat, glong, stl, f107a, f107, ap, mass, 
	    d__, t, (logical *)0);
    }

/* Subroutine */ int meter6_(logical *meter)
{
    return gts6_0_(1, (integer *)0, (real *)0, (real *)0, (real *)0, (real *)
	    0, (real *)0, (real *)0, (real *)0, (real *)0, (integer *)0, (
	    real *)0, (real *)0, meter);
    }

/* ----------------------------------------------------------------------- */
doublereal globe6_(real *yrd, real *sec, real *lat, real *long__, real *tloc, 
	real *f107a, real *f107, real *ap, real *p)
{
    /* Initialized data */

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
    static integer nsw = 14;
    static real p39 = -1e3f;
    static real longl = -999.f;

    /* System generated locals */
    integer i__1;
    real ret_val, r__1, r__2, r__3, r__4, r__5, r__6;
    doublereal d__1;

    /* Builtin functions */
//    double sin(doublereal), cos(doublereal), exp(doublereal), pow_dd(
//	    doublereal *, doublereal *);

    /* Local variables */
    static real c__;
    static integer i__, j;
    static real s, c2, c4, f1, f2, s2, p44, p45, t71, t72, t81, t82, cd14, 
	    cd32, cd18, cd39, exp1, exp2;
    /* Subroutine */ int tselec_90(real *);

/*       CALCULATE G(L) FUNCTION */
/*       Upper Thermosphere Parameters */
    /* Parameter adjustments */
    --p;
    --ap;

    /* Function Body */
/*       3hr Magnetica activity functions */

    if (csw_1.isw != 64999) {
	tselec_90(sv);
    }
    for (j = 1; j <= 14; ++j) {
	ttest_2.t[j - 1] = 0.f;
/* L10: */
    }
    if (csw_1.sw[8] > 0.f) {
	sw9 = 1.f;
    }
    if (csw_1.sw[8] < 0.f) {
	sw9 = -1.f;
    }
    lpoly_1.iyr = *yrd / 1e3f;
    lpoly_1.day = *yrd - lpoly_1.iyr * 1e3f;
    lpoly_1.xlong = *long__;
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
/*     PLG(8,1) = (13.*C*PLG(7,1) - 6.*PLG(6,1))/7. */
    lpoly_1.plg[10] = s;
    lpoly_1.plg[11] = c__ * 3.f * s;
    lpoly_1.plg[12] = (c2 * 5.f - 1.f) * 1.5f * s;
    lpoly_1.plg[13] = (c2 * 7.f * c__ - c__ * 3.f) * 2.5f * s;
    lpoly_1.plg[14] = (c4 * 21.f - c2 * 14.f + 1.f) * 1.875f * s;
    lpoly_1.plg[15] = (c__ * 11.f * lpoly_1.plg[14] - lpoly_1.plg[13] * 6.f) /
	     5.f;
/*     PLG(8,2) = (13.*C*PLG(7,2)-7.*PLG(6,2))/6. */
/*     PLG(9,2) = (15.*C*PLG(8,2)-8.*PLG(7,2))/7. */
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
    if (csw_1.sw[6] == 0.f && csw_1.sw[7] == 0.f && csw_1.sw[13] == 0.f) {
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
    if (*long__ != longl) {
	lpoly_1.clong = cos(dgtr * *long__);
	lpoly_1.slong = sin(dgtr * *long__);
    }
    longl = *long__;
    if (lpoly_1.day != dayl || p[14] != p14) {
	cd14 = cos(dr * (lpoly_1.day - p[14]));
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
    if (csw_1.sw[6] == 0.f) {
	goto L200;
    }
    t71 = p[12] * lpoly_1.plg[11] * cd14 * csw_1.swc[4];
    t72 = p[13] * lpoly_1.plg[11] * cd14 * csw_1.swc[4];
    ttest_2.t[6] = f2 * ((p[4] * lpoly_1.plg[10] + p[5] * lpoly_1.plg[12] + p[
	    28] * lpoly_1.plg[14] + t71) * lpoly_1.ctloc + (p[7] * 
	    lpoly_1.plg[10] + p[8] * lpoly_1.plg[12] + p[29] * lpoly_1.plg[14]
	     + t72) * lpoly_1.stloc);
L200:
/*        SEMIDIURNAL */
    if (csw_1.sw[7] == 0.f) {
	goto L210;
    }
    t81 = (p[24] * lpoly_1.plg[21] + p[36] * lpoly_1.plg[23]) * cd14 * 
	    csw_1.swc[4];
    t82 = (p[34] * lpoly_1.plg[21] + p[37] * lpoly_1.plg[23]) * cd14 * 
	    csw_1.swc[4];
    ttest_2.t[7] = f2 * ((p[6] * lpoly_1.plg[20] + p[42] * lpoly_1.plg[22] + 
	    t81) * lpoly_1.c2tloc + (p[9] * lpoly_1.plg[20] + p[43] * 
	    lpoly_1.plg[22] + t82) * lpoly_1.s2tloc);
L210:
/*        TERDIURNAL */
    if (csw_1.sw[13] == 0.f) {
	goto L220;
    }
    ttest_2.t[13] = f2 * ((p[40] * lpoly_1.plg[30] + (p[94] * lpoly_1.plg[31] 
	    + p[47] * lpoly_1.plg[33]) * cd14 * csw_1.swc[4]) * 
	    lpoly_1.s3tloc + (p[41] * lpoly_1.plg[30] + (p[95] * lpoly_1.plg[
	    31] + p[49] * lpoly_1.plg[33]) * cd14 * csw_1.swc[4]) * 
	    lpoly_1.c3tloc);
L220:
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
    if (csw_1.sw[8] == 0.f) {
	goto L40;
    }
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
	    1.f - r__6 * (r__5 * r__5)) / (1.f - exp1) * pow(d__1, c_b99)
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
	    1.f - r__6 * (r__5 * r__5)) / (1.f - exp2) * pow(d__1, c_b99)
	    );
    if (csw_1.sw[8] == 0.f) {
	goto L40;
    }
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
    if (csw_1.sw[10] == 0.f) {
	goto L230;
    }
    ttest_2.t[10] = (p[81] * lpoly_1.dfa * csw_1.swc[0] + 1.f) * ((p[65] * 
	    lpoly_1.plg[11] + p[66] * lpoly_1.plg[13] + p[67] * lpoly_1.plg[
	    15] + p[104] * lpoly_1.plg[10] + p[105] * lpoly_1.plg[12] + p[106]
	     * lpoly_1.plg[14] + csw_1.swc[4] * (p[110] * lpoly_1.plg[10] + p[
	    111] * lpoly_1.plg[12] + p[112] * lpoly_1.plg[14]) * cd14) * 
	    lpoly_1.clong + (p[91] * lpoly_1.plg[11] + p[92] * lpoly_1.plg[13]
	     + p[93] * lpoly_1.plg[15] + p[107] * lpoly_1.plg[10] + p[108] * 
	    lpoly_1.plg[12] + p[109] * lpoly_1.plg[14] + csw_1.swc[4] * (p[
	    113] * lpoly_1.plg[10] + p[114] * lpoly_1.plg[12] + p[115] * 
	    lpoly_1.plg[14]) * cd14) * lpoly_1.slong);
L230:
/*        UT AND MIXED UT,LONGITUDE */
    if (csw_1.sw[11] == 0.f) {
	goto L240;
    }
    ttest_2.t[11] = (p[96] * lpoly_1.plg[1] + 1.f) * (p[82] * lpoly_1.dfa * 
	    csw_1.swc[0] + 1.f) * (p[120] * lpoly_1.plg[1] * csw_1.swc[4] * 
	    cd14 + 1.f) * ((p[69] * lpoly_1.plg[1] + p[70] * lpoly_1.plg[3] + 
	    p[71] * lpoly_1.plg[5]) * cos(sr * (*sec - p[72])));
    ttest_2.t[11] += csw_1.swc[10] * (p[77] * lpoly_1.plg[21] + p[78] * 
	    lpoly_1.plg[23] + p[79] * lpoly_1.plg[25]) * cos(sr * (*sec - p[
	    80]) + dgtr * 2.f * *long__) * (p[138] * lpoly_1.dfa * csw_1.swc[
	    0] + 1.f);
L240:
/*        UT,LONGITUDE MAGNETIC ACTIVITY */
    if (csw_1.sw[12] == 0.f) {
	goto L48;
    }
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
L49:
    ttest_2.tinf = p[31];
    i__1 = nsw;
    for (i__ = 1; i__ <= i__1; ++i__) {
/* L50: */
	ttest_2.tinf += (r__1 = csw_1.sw[i__ - 1], dabs(r__1)) * ttest_2.t[
		i__ - 1];
    }
    ret_val = ttest_2.tinf;
    return ret_val;
} /* globe6_ */

/* ----------------------------------------------------------------------- */
/* Subroutine */ int tselec_0_90(int n__, real *sv, real *svv)
{
    /* System generated locals */
    real r__1, r__2;

    /* Builtin functions */
//    double r_mod(real *, real *);

    /* Local variables */
    static integer i__;
    static real sav[25];

/*        SET SWITCHES */
/*        SW FOR MAIN TERMS, SWC FOR CROSS TERMS */
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
	csw_1.sw[i__ - 1] = mod(sv[i__], c_b109);
	if ((r__1 = sv[i__], dabs(r__1)) == 1.f || (r__2 = sv[i__], dabs(r__2)
		) == 2.f) {
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

/* Subroutine */ int tselec_90(real *sv)
{
    return tselec_0_90(0, sv, (real *)0);
    }

/* Subroutine */ int tretrv_90(real *svv)
{
    return tselec_0_90(1, (real *)0, svv);
    }

/* ----------------------------------------------------------------------- */
doublereal glob6s_(real *p)
{
    /* Initialized data */

    static real dr = .0172142f;
    static real dayl = -1.f;
    static real p32 = -1e3f;
    static real p18 = -1e3f;
    static real p14 = -1e3f;
    static real p39 = -1e3f;

    /* System generated locals */
    real ret_val, r__1;

    /* Builtin functions */
//    double cos(doublereal);

    /* Local variables */
    static integer i__, j;
    static real t[14], t71, t72, t81, t82, tt, cd32, cd14, cd18, cd39;

/*      VERSION OF GLOBE FOR LOWER ATMOSPHERE 1/17/90 */
    /* Parameter adjustments */
    --p;

    /* Function Body */
    for (j = 1; j <= 14; ++j) {
	t[j - 1] = 0.f;
/* L10: */
    }
    if (lpoly_2.day != dayl || p32 != p[32]) {
	cd32 = cos(dr * (lpoly_2.day - p[32]));
    }
    if (lpoly_2.day != dayl || p18 != p[18]) {
	cd18 = cos(dr * 2.f * (lpoly_2.day - p[18]));
    }
    if (lpoly_2.day != dayl || p14 != p[14]) {
	cd14 = cos(dr * (lpoly_2.day - p[14]));
    }
    if (lpoly_2.day != dayl || p39 != p[39]) {
	cd39 = cos(dr * 2.f * (lpoly_2.day - p[39]));
    }
    dayl = lpoly_2.day;
    p32 = p[32];
    p18 = p[18];
    p14 = p[14];
    p39 = p[39];

/*       F10.7 */
    t[0] = p[22] * lpoly_2.dfa;
/*       TIME INDEPENDENT */
    t[1] = p[2] * lpoly_2.plg[2] + p[3] * lpoly_2.plg[4] + p[23] * 
	    lpoly_2.plg[6] + p[27] * lpoly_2.plg[1] + p[28] * lpoly_2.plg[3] 
	    + p[29] * lpoly_2.plg[5];
/*       SYMMETRICAL ANNUAL */
    t[2] = (p[19] + p[48] * lpoly_2.plg[2] + p[30] * lpoly_2.plg[4]) * cd32;
/*       SYMMETRICAL SEMIANNUAL */
    t[3] = (p[16] + p[17] * lpoly_2.plg[2] + p[31] * lpoly_2.plg[4]) * cd18;
/*       ASYMMETRICAL ANNUAL */
    t[4] = (p[10] * lpoly_2.plg[1] + p[11] * lpoly_2.plg[3] + p[36] * 
	    lpoly_2.plg[5]) * cd14;
/*       ASYMMETRICAL SEMIANNUAL */
    t[5] = p[38] * lpoly_2.plg[1] * cd39;
/*        DIURNAL */
    if (csw_1.sw[6] == 0.f) {
	goto L200;
    }
    t71 = p[12] * lpoly_2.plg[11] * cd14 * csw_1.swc[4];
    t72 = p[13] * lpoly_2.plg[11] * cd14 * csw_1.swc[4];
    t[6] = (p[4] * lpoly_2.plg[10] + p[5] * lpoly_2.plg[12] + t71) * 
	    lpoly_2.ctloc + (p[7] * lpoly_2.plg[10] + p[8] * lpoly_2.plg[12] 
	    + t72) * lpoly_2.stloc;
L200:
/*        SEMIDIURNAL */
    if (csw_1.sw[7] == 0.f) {
	goto L210;
    }
    t81 = (p[24] * lpoly_2.plg[21] + p[47] * lpoly_2.plg[23]) * cd14 * 
	    csw_1.swc[4];
    t82 = (p[34] * lpoly_2.plg[21] + p[49] * lpoly_2.plg[23]) * cd14 * 
	    csw_1.swc[4];
    t[7] = (p[6] * lpoly_2.plg[20] + p[42] * lpoly_2.plg[22] + t81) * 
	    lpoly_2.c2tloc + (p[9] * lpoly_2.plg[20] + p[43] * lpoly_2.plg[22]
	     + t82) * lpoly_2.s2tloc;
L210:
/*        TERDIURNAL */
    if (csw_1.sw[13] == 0.f) {
	goto L220;
    }
    t[13] = p[40] * lpoly_2.plg[30] * lpoly_2.s3tloc + p[41] * lpoly_2.plg[30]
	     * lpoly_2.c3tloc;
L220:
/*       MAGNETIC ACTIVITY */
    if (csw_1.sw[8] == 0.f) {
	goto L40;
    }
    if (csw_1.sw[8] == 1.f) {
	t[8] = lpoly_2.apdf * (p[33] + p[46] * lpoly_2.plg[2] * csw_1.swc[1]);
    }
    if (csw_1.sw[8] == -1.f) {
	t[8] = p[51] * lpoly_2.apt[2] + p[97] * lpoly_2.plg[2] * lpoly_2.apt[
		2] * csw_1.swc[1];
    }
L40:
    if (csw_1.sw[9] == 0.f || csw_1.sw[10] == 0.f || lpoly_2.long__ <= -1e3f) 
	    {
	goto L49;
    }
/*        LONGITUDINAL */
    t[10] = (lpoly_2.plg[1] * (p[81] * csw_1.swc[4] * cos(dr * (lpoly_2.day - 
	    p[82])) + p[86] * csw_1.swc[5] * cos(dr * 2.f * (lpoly_2.day - p[
	    87]))) + 1.f + p[84] * csw_1.swc[2] * cos(dr * (lpoly_2.day - p[
	    85])) + p[88] * csw_1.swc[3] * cos(dr * 2.f * (lpoly_2.day - p[89]
	    ))) * ((p[65] * lpoly_2.plg[11] + p[66] * lpoly_2.plg[13] + p[67] 
	    * lpoly_2.plg[15] + p[75] * lpoly_2.plg[10] + p[76] * lpoly_2.plg[
	    12] + p[77] * lpoly_2.plg[14]) * lpoly_2.clong + (p[91] * 
	    lpoly_2.plg[11] + p[92] * lpoly_2.plg[13] + p[93] * lpoly_2.plg[
	    15] + p[78] * lpoly_2.plg[10] + p[79] * lpoly_2.plg[12] + p[80] * 
	    lpoly_2.plg[14]) * lpoly_2.slong);
L49:
    tt = 0.f;
    for (i__ = 1; i__ <= 14; ++i__) {
/* L50: */
	tt += (r__1 = csw_1.sw[i__ - 1], dabs(r__1)) * t[i__ - 1];
    }
    ret_val = tt;
    return ret_val;
} /* glob6s_ */

/* -------------------------------------------------------------------- */
doublereal densu_90(real *alt, real *dlb, real *tinf, real *tlb, real *xm, real 
	*alpha, real *tz, real *zlb, real *s2, integer *mn1, real *zn1, real *
	tn1, real *tgn1)
{
    /* Initialized data */

    static real rgas = 831.4f;

    /* System generated locals */
    integer i__1;
    real ret_val, r__1, r__2;
    doublereal d__1, d__2;

    /* Builtin functions */
//    double exp(doublereal), pow_dd(doublereal *, doublereal *);

    /* Local variables */
    static integer k;
    static real x, y, z__, t1, t2, z1, z2, ta, za;
    static integer mn;
    static real zg, yi, tt, xs[5], ys[5], yd1, yd2, zg2, glb, dta, gamm, expl,
	     y2out[5], gamma, densa, zgdif;
    extern /* Subroutine */ int spline_90(real *, real *, integer *, real *, 
	    real *, real *), splini_90(real *, real *, real *, integer *, real *
	    , real *), splint_90(real *, real *, real *, integer *, real *,
	    real *);

/*       Calculate Temperature and Density Profiles for MSIS models */
/*       New lower thermo polynomial 10/30/89 */
    /* Parameter adjustments */
    --tn1;
    --zn1;
    --tgn1;

    /* Function Body */
/* CCCCCWRITE(6,*) 'DB',ALT,DLB,TINF,TLB,XM,ALPHA,ZLB,S2,MN1,ZN1,TN1 */
    ret_val = 1.f;
/*        Joining altitude of Bates and spline */
    za = zn1[1];
    z__ = dmax(*alt,za);
/*      Geopotential altitude difference from ZLB */
    zg2 = (z__ - *zlb) * (parmb_1.re + *zlb) / (parmb_1.re + z__);
/*      Bates temperature */
    tt = *tinf - (*tinf - *tlb) * exp(-(*s2) * zg2);
    ta = tt;
    *tz = tt;
    ret_val = *tz;
    if (*alt >= za) {
	goto L10;
    }

/*       CALCULATE TEMPERATURE BELOW ZA */
/*      Temperature gradient at ZA from Bates profile */
/* Computing 2nd power */
    r__1 = (parmb_1.re + *zlb) / (parmb_1.re + za);
    dta = (*tinf - ta) * *s2 * (r__1 * r__1);
    tgn1[1] = dta;
    tn1[1] = ta;
/* Computing MAX */
    r__1 = *alt, r__2 = zn1[*mn1];
    z__ = dmax(r__1,r__2);
    mn = *mn1;
    z1 = zn1[1];
    z2 = zn1[mn];
    t1 = tn1[1];
    t2 = tn1[mn];
/*      Geopotental difference from Z1 */
    zg = (z__ - z1) * (parmb_1.re + z1) / (parmb_1.re + z__);
    zgdif = (z2 - z1) * (parmb_1.re + z1) / (parmb_1.re + z2);
/*       Set up spline nodes */
    i__1 = mn;
    for (k = 1; k <= i__1; ++k) {
	xs[k - 1] = (zn1[k] - z1) * (parmb_1.re + z1) / (parmb_1.re + zn1[k]) 
		/ zgdif;
	ys[k - 1] = 1.f / tn1[k];
/* L20: */
    }
/*        End node derivatives */
    yd1 = -tgn1[1] / (t1 * t1) * zgdif;
/* Computing 2nd power */
    r__1 = (parmb_1.re + z2) / (parmb_1.re + z1);
    yd2 = -tgn1[2] / (t2 * t2) * zgdif * (r__1 * r__1);
/*       Calculate spline coefficients */
    spline_90(xs, ys, &mn, &yd1, &yd2, y2out);
    x = zg / zgdif;
    splint_90(xs, ys, y2out, &mn, &x, &y);
/*       temperature at altitude */
    *tz = 1.f / y;
    ret_val = *tz;
L10:
    if (*xm == 0.f) {
	goto L50;
    }

/*      CALCULATE DENSITY ABOVE ZA */
/* Computing 2nd power */
    r__1 = *zlb / parmb_1.re + 1.f;
    glb = parmb_1.gsurf / (r__1 * r__1);
    gamma = *xm * glb / (*s2 * rgas * *tinf);
    expl = exp(-(*s2) * gamma * zg2);
    if (expl > 50.f || tt <= 0.f) {
	expl = 50.f;
    }
/*       Density at altitude */
    d__1 = (doublereal) (*tlb / tt);
    d__2 = (doublereal) (*alpha + 1.f + gamma);
    densa = *dlb * pow(d__1, d__2) * expl;
    ret_val = densa;
    if (*alt >= za) {
	goto L50;
    }

/*      CALCULATE DENSITY BELOW ZA */
/* Computing 2nd power */
    r__1 = z1 / parmb_1.re + 1.f;
    glb = parmb_1.gsurf / (r__1 * r__1);
    gamm = *xm * glb * zgdif / rgas;
/*       integrate spline temperatures */
    splini_90(xs, ys, y2out, &mn, &x, &yi);
    expl = gamm * yi;
    if (expl > 50.f || *tz <= 0.f) {
	expl = 50.f;
    }
/*       Density at altitude */
    d__1 = (doublereal) (t1 / *tz);
    d__2 = (doublereal) (*alpha + 1.f);
    ret_val = ret_val * pow(d__1, d__2) * exp(-expl);
L50:
    return ret_val;
} /* densu_90 */

/* -------------------------------------------------------------------- */
doublereal densm_90(real *alt, real *d0, real *xm, real *tz, integer *mn3, real 
	*zn3, real *tn3, real *tgn3, integer *mn2, real *zn2, real *tn2, real 
	*tgn2)
{
    /* Initialized data */

    static real rgas = 831.4f;

    /* System generated locals */
    integer i__1;
    real ret_val, r__1, r__2;

    /* Builtin functions */
//    double exp(doublereal);

    /* Local variables */
    static integer k;
    static real x, y, z__, t1, t2, z1, z2;
    static integer mn;
    static real zg, yi, xs[10], ys[10], yd1, yd2, glb, gamm, expl, y2out[10], 
	    zgdif;
    extern /* Subroutine */ int spline_90(real *, real *, integer *, real *, 
	    real *, real *), splini_90(real *, real *, real *, integer *, real *
	    , real *), splint_90(real *, real *, real *, integer *, real *, 
	    real *);

/*       Calculate Temperature and Density Profiles for lower atmos. */
    /* Parameter adjustments */
    --tn3;
    --zn3;
    --tgn3;
    --tn2;
    --zn2;
    --tgn2;

    /* Function Body */
    ret_val = *d0;
    if (*alt > zn2[1]) {
	goto L50;
    }
/*      STRATOSPHERE/MESOSPHERE TEMPERATURE */
/* Computing MAX */
    r__1 = *alt, r__2 = zn2[*mn2];
    z__ = dmax(r__1,r__2);
    mn = *mn2;
    z1 = zn2[1];
    z2 = zn2[mn];
    t1 = tn2[1];
    t2 = tn2[mn];
    zg = (z__ - z1) * (parmb_1.re + z1) / (parmb_1.re + z__);
    zgdif = (z2 - z1) * (parmb_1.re + z1) / (parmb_1.re + z2);
/*       Set up spline nodes */
    i__1 = mn;
    for (k = 1; k <= i__1; ++k) {
	xs[k - 1] = (zn2[k] - z1) * (parmb_1.re + z1) / (parmb_1.re + zn2[k]) 
		/ zgdif;
	ys[k - 1] = 1.f / tn2[k];
/* L210: */
    }
    yd1 = -tgn2[1] / (t1 * t1) * zgdif;
/* Computing 2nd power */
    r__1 = (parmb_1.re + z2) / (parmb_1.re + z1);
    yd2 = -tgn2[2] / (t2 * t2) * zgdif * (r__1 * r__1);
/*       Calculate spline coefficients */
    spline_90(xs, ys, &mn, &yd1, &yd2, y2out);
    x = zg / zgdif;
    splint_90(xs, ys, y2out, &mn, &x, &y);
/*       Temperature at altitude */
    *tz = 1.f / y;
    if (*xm == 0.f) {
	goto L20;
    }

/*      CALCULATE STRATOSPHERE/MESOSPHERE DENSITY */
/* Computing 2nd power */
    r__1 = z1 / parmb_1.re + 1.f;
    glb = parmb_1.gsurf / (r__1 * r__1);
    gamm = *xm * glb * zgdif / rgas;
/*       Integrate temperature profile */
    splini_90(xs, ys, y2out, &mn, &x, &yi);
    expl = gamm * yi;
    if (expl > 50.f) {
	expl = 50.f;
    }
/*       Density at altitude */
    ret_val = ret_val * (t1 / *tz) * exp(-expl);
L20:
    if (*alt > zn3[1]) {
	goto L50;
    }

/*      TROPOSPHERE/STRATOSPHERE TEMPERATURE */
    z__ = *alt;
    mn = *mn3;
    z1 = zn3[1];
    z2 = zn3[mn];
    t1 = tn3[1];
    t2 = tn3[mn];
    zg = (z__ - z1) * (parmb_1.re + z1) / (parmb_1.re + z__);
    zgdif = (z2 - z1) * (parmb_1.re + z1) / (parmb_1.re + z2);
/*       Set up spline nodes */
    i__1 = mn;
    for (k = 1; k <= i__1; ++k) {
	xs[k - 1] = (zn3[k] - z1) * (parmb_1.re + z1) / (parmb_1.re + zn3[k]) 
		/ zgdif;
	ys[k - 1] = 1.f / tn3[k];
/* L220: */
    }
    yd1 = -tgn3[1] / (t1 * t1) * zgdif;
/* Computing 2nd power */
    r__1 = (parmb_1.re + z2) / (parmb_1.re + z1);
    yd2 = -tgn3[2] / (t2 * t2) * zgdif * (r__1 * r__1);
/*       Calculate spline coefficients */
    spline_90(xs, ys, &mn, &yd1, &yd2, y2out);
    x = zg / zgdif;
    splint_90(xs, ys, y2out, &mn, &x, &y);
/*       temperature at altitude */
    *tz = 1.f / y;
    if (*xm == 0.f) {
	goto L30;
    }

/*      CALCULATE TROPOSPHERIC/STRATOSPHERE DENSITY */

/* Computing 2nd power */
    r__1 = z1 / parmb_1.re + 1.f;
    glb = parmb_1.gsurf / (r__1 * r__1);
    gamm = *xm * glb * zgdif / rgas;
/*        Integrate temperature profile */
    splini_90(xs, ys, y2out, &mn, &x, &yi);
    expl = gamm * yi;
    if (expl > 50.f) {
	expl = 50.f;
    }
/*        Density at altitude */
    ret_val = ret_val * (t1 / *tz) * exp(-expl);
L30:
L50:
    if (*xm == 0.f) {
	ret_val = *tz;
    }
    return ret_val;
} /* densm_90 */

/* ----------------------------------------------------------------------- */
/* Subroutine */ int spline_90(real *x, real *y, integer *n, real *yp1, real *
	ypn, real *y2)
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    static integer i__, k;
    static real p, u[100], qn, un, sig;

/*        CALCULATE 2ND DERIVATIVES OF CUBIC SPLINE INTERP FUNCTION */
/*        ADAPTED FROM NUMERICAL RECIPES BY PRESS ET AL */
/*        X,Y: ARRAYS OF TABULATED FUNCTION IN ASCENDING ORDER BY X */
/*        N: SIZE OF ARRAYS X,Y */
/*        YP1,YPN: SPECIFIED DERIVATIVES AT X(1) AND X(N); VALUES */
/*                 >= 1E30 SIGNAL SIGNAL SECOND DERIVATIVE ZERO */
/*        Y2: OUTPUT ARRAY OF SECOND DERIVATIVES */
    /* Parameter adjustments */
    --y2;
    --y;
    --x;

    /* Function Body */
    if (*yp1 > 9.9e29f) {
	y2[1] = 0.f;
	u[0] = 0.f;
    } else {
	y2[1] = -.5f;
	u[0] = 3.f / (x[2] - x[1]) * ((y[2] - y[1]) / (x[2] - x[1]) - *yp1);
    }
    i__1 = *n - 1;
    for (i__ = 2; i__ <= i__1; ++i__) {
	sig = (x[i__] - x[i__ - 1]) / (x[i__ + 1] - x[i__ - 1]);
	p = sig * y2[i__ - 1] + 2.f;
	y2[i__] = (sig - 1.f) / p;
	u[i__ - 1] = (((y[i__ + 1] - y[i__]) / (x[i__ + 1] - x[i__]) - (y[i__]
		 - y[i__ - 1]) / (x[i__] - x[i__ - 1])) * 6.f / (x[i__ + 1] - 
		x[i__ - 1]) - sig * u[i__ - 2]) / p;
/* L11: */
    }
    if (*ypn > 9.9e29f) {
	qn = 0.f;
	un = 0.f;
    } else {
	qn = .5f;
	un = 3.f / (x[*n] - x[*n - 1]) * (*ypn - (y[*n] - y[*n - 1]) / (x[*n] 
		- x[*n - 1]));
    }
    y2[*n] = (un - qn * u[*n - 2]) / (qn * y2[*n - 1] + 1.f);
    for (k = *n - 1; k >= 1; --k) {
	y2[k] = y2[k] * y2[k + 1] + u[k - 1];
/* L12: */
    }
    return 0;
} /* spline_90 */

/* ----------------------------------------------------------------------- */
/* Subroutine */ int splint_90(real *xa, real *ya, real *y2a, integer *n, real *
	x, real *y)
{
    static real a, b, h__;
    static integer k, khi, klo;

/*        CALCULATE CUBIC SPLINE INTERP VALUE */
/*        ADAPTED FROM NUMBERICAL RECIPES BY PRESS ET AL. */
/*        XA,YA: ARRAYS OF TABULATED FUNCTION IN ASCENDING ORDER BY X */
/*        Y2A: ARRAY OF SECOND DERIVATIVES */
/*        N: SIZE OF ARRAYS XA,YA,Y2A */
/*        X: ABSCISSA FOR INTERPOLATION */
/*        Y: OUTPUT VALUE */
    /* Parameter adjustments */
    --y2a;
    --ya;
    --xa;

    /* Function Body */
    klo = 1;
    khi = *n;
L1:
    if (khi - klo > 1) {
	k = (khi + klo) / 2;
	if (xa[k] > *x) {
	    khi = k;
	} else {
	    klo = k;
	}
	goto L1;
    }
    h__ = xa[khi] - xa[klo];
/* XX      IF(H.EQ.0) WRITE(6,*) 'BAD XA INPUT TO SPLINT' */
    a = (xa[khi] - *x) / h__;
    b = (*x - xa[klo]) / h__;
    *y = a * ya[klo] + b * ya[khi] + ((a * a * a - a) * y2a[klo] + (b * b * b
	    - b) * y2a[khi]) * h__ * h__ / 6.f;
    return 0;
} /* splint_90 */

/* ----------------------------------------------------------------------- */
/* Subroutine */ int splini_90(real *xa, real *ya, real *y2a, integer *n, real *
	x, real *yi)
{
    /* System generated locals */
    real r__1, r__2;

    /* Local variables */
    static real a, b, h__, a2, b2, xx;
    static integer khi, klo;

/*       INTEGRATE CUBIC SPLINE FUNCTION FROM XA(1) TO X */
/*        XA,YA: ARRAYS OF TABULATED FUNCTION IN ASCENDING ORDER BY X */
/*        Y2A: ARRAY OF SECOND DERIVATIVES */
/*        N: SIZE OF ARRAYS XA,YA,Y2A */
/*        X: ABSCISSA ENDPOINT FOR INTEGRATION */
/*        Y: OUTPUT VALUE */
    /* Parameter adjustments */
    --y2a;
    --ya;
    --xa;

    /* Function Body */
    *yi = 0.f;
    klo = 1;
    khi = 2;
L1:
    if (*x > xa[klo] && khi <= *n) {
	xx = *x;
	if (khi < *n) {
/* Computing MIN */
	    r__1 = *x, r__2 = xa[khi];
	    xx = dmin(r__1,r__2);
	}
	h__ = xa[khi] - xa[klo];
	a = (xa[khi] - xx) / h__;
	b = (xx - xa[klo]) / h__;
	a2 = a * a;
	b2 = b * b;
	*yi += ((1.f - a2) * ya[klo] / 2.f + b2 * ya[khi] / 2.f + ((-(a2 * a2 
		+ 1.f) / 4.f + a2 / 2.f) * y2a[klo] + (b2 * b2 / 4.f - b2 / 
		2.f) * y2a[khi]) * h__ * h__ / 6.f) * h__;
	++klo;
	++khi;
	goto L1;
    }
    return 0;
} /* splini_90 */

/* ----------------------------------------------------------------------- */
doublereal dnet_90(real *dd, real *dm, real *zhm, real *xmm, real *xm)
{
    /* System generated locals */
    real ret_val;
    doublereal d__1, d__2;

    /* Builtin functions */
//    double log(doublereal), exp(doublereal), pow_dd(doublereal *, doublereal *
//	    );

    /* Local variables */
    static real a, ylog;

/*       TURBOPAUSE CORRECTION FOR MSIS MODELS */
/*         Root mean density */
/*       8/20/80 */
/*          DD - diffusive density */
/*          DM - full mixed density */
/*          ZHM - transition scale length */
/*          XMM - full mixed molecular weight */
/*          XM  - species molecular weight */
/*          DNET - combined density */
    a = *zhm / (*xmm - *xm);
    if (*dm > 0.f && *dd > 0.f) {
	goto L5;
    }
/* XX        WRITE(6,*) 'DNET LOG ERROR',DM,DD,XM */
    if (*dd == 0.f && *dm == 0.f) {
	*dd = 1.f;
    }
    if (*dm == 0.f) {
	goto L10;
    }
    if (*dd == 0.f) {
	goto L20;
    }
L5:
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

/* ----------------------------------------------------------------------- */
doublereal ccor_90(real *alt, real *r__, real *h1, real *zh)
{
    /* System generated locals */
    real ret_val;

    /* Builtin functions */
//    double exp(doublereal);

    /* Local variables */
    static real e, ex;

/*        CHEMISTRY/DISSOCIATION CORRECTION FOR MSIS MODELS */
/*        ALT - altitude */
/*        R - target ratio */
/*        H1 - transition scale length */
/*        ZH - altitude of 1/2 R */
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
} /* ccor_90 */

/* ----------------------------------------------------------------------- */
/* Subroutine */ int gtd6bk_(void)
{
    return 0;
} /* gtd6bk_ */

/*          MSISE 90 12-MAR-90 */
/* XX      COMMON/DATIM6/ISDATE(3),ISTIME(2),NAME(2) */
/* XX      DATA ISDATE/'12-M','AR-9','0   '/,ISTIME/'15:0','9:04'/ */
/* XX      DATA NAME/'MSIS','E 90'/ */
/*         TEMPERATURE */
/*         HE DENSITY */
/*         O DENSITY */
/*         N2 DENSITY */
/*         TLB */
/*         O2 DENSITY */
/*         AR DENSITY */
/*          H DENSITY */
/*          N DENSITY */
/*        SPARE */
/*          S PARAM */
/*          TURBO */
/*         LOWER BOUNDARY */







/*         TN1(2) */
/*         TN1(3) */
/*         TN1(4) */
/*         TN1(5) TN2(1) */
/*          TN2(2) */
/*          TN2(3) */
/*          TN2(4) TN3(1) */
/*          TN3(2) */
/*          TN3(3) */
/*          TN3(4) */
/*          TN3(5) SURFACE TEMP TSL */
/*          TGN3(2) SURFACE GRAD TSLG */
/*          TGN2(1) TGN1(2) */
/*          TGN3(1) TGN2(2) */
/*         MIDDLE ATMOSPHERE AVERAGES */


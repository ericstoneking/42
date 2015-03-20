/*  -- translated by f2c (version 20030306).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#include "val_atm_msis00.h"

/* Common Block Declarations */

struct {
    real tlb, s, db04, db16, db28, db32, db40, db48, db01, za, t0, z0, g0, rl,
	     dd, db14, tr12;
} gts3c_00;

#define gts3c_1 gts3c_00

struct {
    real tn1[5], tn2[4], tn3[5], tgn1[2], tgn2[2], tgn3[2];
} meso7_;

#define meso7_1 meso7_

struct lower7_1_ {
    real ptm[10], pdm[80]	/* was [10][8] */;
};

#define lower7_1 (*(struct lower7_1_ *) &lower7_)

struct parm7_1_ {
    real pt[150], pd[1350]	/* was [150][9] */, ps[150], pdl[50]	/* 
	    was [25][2] */, ptl[400]	/* was [100][4] */, pma[1000]	/* 
	    was [100][10] */, sam[100];
};
struct parm7_2_ {
    real pt1[50], pt2[50], pt3[50], pa1[50], pa2[50], pa3[50], pb1[50], pb2[
	    50], pb3[50], pc1[50], pc2[50], pc3[50], pd1[50], pd2[50], pd3[50]
	    , pe1[50], pe2[50], pe3[50], pf1[50], pf2[50], pf3[50], pg1[50], 
	    pg2[50], pg3[50], ph1[50], ph2[50], ph3[50], pi1[50], pi2[50], 
	    pi3[50], pj1[50], pj2[50], pj3[50], pk1[50], pl1[50], pl2[50], 
	    pm1[50], pm2[50], pn1[50], pn2[50], po1[50], po2[50], pp1[50], 
	    pp2[50], pq1[50], pq2[50], pr1[50], pr2[50], ps1[50], ps2[50], 
	    pu1[50], pu2[50], pv1[50], pv2[50], pw1[50], pw2[50], px1[50], 
	    px2[50], py1[50], py2[50], pz1[50], pz2[50], paa1[50], paa2[50];
};

#define parm7_1 (*(struct parm7_1_ *) &parm7_)
#define parm7_2 (*(struct parm7_2_ *) &parm7_)

struct {
    real sw[25];
    integer isw;
    real swc[25];
} csw_;

#define csw_1 csw_

struct mavg7_1_ {
    real pavgm[10];
};

#define mavg7_1 (*(struct mavg7_1_ *) &mavg7_)

struct {
    real dm04, dm16, dm28, dm32, dm40, dm01, dm14;
} dmix_00;

#define dmix_1 dmix_00

struct {
    real gsurf, re;
} parmb_00;

#define parmb_1 parmb_00

struct metsel_1_ {
    integer imr;
};

#define metsel_1 (*(struct metsel_1_ *) &metsel_00)

union {
    struct {
	real tinfg, gb, rout, tt[15];
    } _1;
    struct {
	real tinf, gb, rout, t[15];
    } _2;
} ttest_00;

#define ttest_1 (ttest_00._1)
#define ttest_2 (ttest_00._2)

union {
    struct {
	real plg[36]	/* was [9][4] */, ctloc, stloc, c2tloc, s2tloc, 
		c3tloc, s3tloc;
	integer iyr;
	real day, df, dfa, apd, apdf, apt[4], xlong;
    } _1;
    struct {
	real plg[36]	/* was [9][4] */, ctloc, stloc, c2tloc, s2tloc, 
		c3tloc, s3tloc;
	integer iyr;
	real day, df, dfa, apd, apdf, apt[4], long__;
    } _2;
} lpoly_00;

#define lpoly_1 (lpoly_00._1)
#define lpoly_2 (lpoly_00._2)

struct {
    integer mp, ii, jg, lt;
    real qpb[50];
    integer ierr, ifun, n, j;
    real dv[60];
} lsqv_00;

#define lsqv_1 lsqv_00

struct {
    real taf;
} fit_00;

#define fit_1 fit_00

/* Initialized data */

struct {
    real e_1[90];
    } lower7_ = { 1041.3f, 386.f, 195.f, 16.6728f, 213.f, 120.f, 240.f, 187.f,
	     -2.f, 0.f, 2.456e7f, 6.71072e-6f, 100.f, 0.f, 110.f, 10.f, 0.f, 
	    0.f, 0.f, 0.f, 8.594e10f, 1.f, 105.f, -8.f, 110.f, 10.f, 90.f, 
	    2.f, 0.f, 0.f, 2.81e11f, 0.f, 105.f, 28.f, 28.95f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, 3.3e10f, .26827f, 105.f, 1.f, 110.f, 10.f, 110.f, -10.f,
	     0.f, 0.f, 1.33e9f, .0119615f, 105.f, 0.f, 110.f, 10.f, 0.f, 0.f, 
	    0.f, 0.f, 176100.f, 1.f, 95.f, -8.f, 110.f, 10.f, 90.f, 2.f, 0.f, 
	    0.f, 1e7f, 1.f, 105.f, -8.f, 110.f, 10.f, 90.f, 2.f, 0.f, 0.f, 
	    1e6f, 1.f, 105.f, -8.f, 550.f, 76.f, 90.f, 2.f, 0.f, 4e3f };

struct {
    real e_1[3200];
    } parm7_ = { .986573f, .0162228f, .015527f, -.104323f, -.00375801f, 
	    -.00118538f, -.124043f, .0045682f, .00876018f, -.136235f, 
	    -.0352427f, .00884181f, -.00592127f, -8.6165f, 0.f, .0128492f, 
	    0.f, 130.096f, .0104567f, .00165686f, -5.53887e-6f, .0029781f, 
	    0.f, .00513122f, .0866784f, .158727f, 0.f, 0.f, 0.f, -7.27026e-6f,
	     0.f, 6.74494f, .00493933f, .00221656f, .00250802f, 0.f, 0.f, 
	    -.0208841f, -1.79873f, .00145103f, 2.81769e-4f, -.00144703f, 
	    -5.16394e-5f, .0847001f, .170147f, .00572562f, 5.07493e-5f, 
	    .00436148f, 1.17863e-4f, .00474364f, .00661278f, 4.34292e-5f, 
	    .00144373f, 2.4147e-5f, .00284426f, 8.5656e-4f, .00204028f, 0.f, 
	    -3159.94f, -.00246423f, .00113843f, 4.20512e-4f, 0.f, -97.7214f, 
	    .00677794f, .00527499f, .00114936f, 0.f, -.00661311f, -.0184255f, 
	    -.0196259f, 29861.8f, 0.f, 0.f, 0.f, 644.574f, 8.84668e-4f, 
	    5.05066e-4f, 0.f, 4028.81f, -.00189503f, 0.f, 0.f, 8.21407e-4f, 
	    .0020678f, 0.f, 0.f, 0.f, 0.f, 0.f, -.012041f, -.00363963f, 
	    9.9207e-5f, -1.15284e-4f, -6.33059e-5f, -.605545f, .00834218f, 
	    -91.3036f, 3.71042e-4f, 0.f, 4.19e-4f, .00270928f, .00331507f, 
	    -.00444508f, -.00496334f, -.00160449f, .00395119f, .00248924f, 
	    5.09815e-4f, .00405302f, .00224076f, 0.f, .00684256f, 4.66354e-4f,
	     0.f, -3.68328e-4f, 0.f, 0.f, -146.87f, 0.f, 0.f, .00109501f, 
	    4.65156e-4f, 5.62583e-4f, 3.21596f, 6.43168e-4f, .0031486f, 
	    .00340738f, .00178481f, 9.62532e-4f, 5.58171e-4f, 3.43731f, 
	    -.233195f, 5.10289e-4f, 0.f, 0.f, -92534.7f, 0.f, -.00199639f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 1.09979f, 
	    -.048806f, -.197501f, -.091028f, -.00696558f, .0242136f, .391333f,
	     -.00720068f, -.0322718f, 1.41508f, .168194f, .0185282f, .109384f,
	     -7.24282f, 0.f, .296377f, -.049721f, 104.114f, -.0861108f, 
	    -7.29177e-4f, 1.48998e-6f, .00108629f, 0.f, 0.f, .083109f, 
	    .112818f, -.0575005f, -.0129919f, -.0178849f, -2.86343e-6f, 0.f, 
	    -151.187f, -.00665902f, 0.f, -.00202069f, 0.f, 0.f, .0432264f, 
	    -28.0444f, -.00326789f, .00247461f, 0.f, 0.f, .09821f, .122714f, 
	    -.039645f, 0.f, -.00276489f, 0.f, .00187723f, -.00809813f, 
	    4.34428e-5f, -.00770932f, 0.f, -.00228894f, -.0056907f, 
	    -.00522193f, .00600692f, -7804.34f, -.00348336f, -.00638362f, 
	    -.0018219f, 0.f, -75.8976f, -.0217875f, -.0172524f, -.00906287f, 
	    0.f, .0244725f, .086604f, .105712f, 30254.3f, 0.f, 0.f, 0.f, 
	    -6013.64f, -.00564668f, -.00254157f, 0.f, 315.611f, -.00569158f, 
	    0.f, 0.f, -.00447216f, -.00449523f, .00464428f, 0.f, 0.f, 0.f, 
	    0.f, .0451236f, .024652f, .00617794f, 0.f, 0.f, -.362944f, 
	    -.0480022f, -75.723f, -.00199656f, 0.f, -.0051878f, -.017399f, 
	    -.00903485f, .00748465f, .0153267f, .0106296f, .0118655f, 
	    .00255569f, .0016902f, .0351936f, -.0181242f, 0.f, -.100529f, 
	    -.00510574f, 0.f, .00210228f, 0.f, 0.f, -173.255f, .507833f, 
	    -.241408f, .00875414f, .00277527f, -8.90353e-5f, -5.25148f, 
	    -.00583899f, -.0209122f, -.0096353f, .00977164f, .00407051f, 
	    2.53555e-4f, -5.52875f, -.355993f, -.00249231f, 0.f, 0.f, 
	    28.6026f, 0.f, 3.42722e-4f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, 1.02315f, -.15971f, -.10663f, -.0177074f, 
	    -.00442726f, .0344803f, .0445613f, -.0333751f, -.0573598f, 
	    .35036f, .0633053f, .0216221f, .0542577f, -5.74193f, 0.f, 
	    .190891f, -.0139194f, 101.102f, .0816363f, 1.33717e-4f, 
	    6.54403e-6f, .00310295f, 0.f, 0.f, .0538205f, .12391f, -.0139831f,
	     0.f, 0.f, -3.95915e-6f, 0.f, -.714651f, -.00501027f, 0.f, 
	    -.00324756f, 0.f, 0.f, .0442173f, -13.1598f, -.00315626f, 
	    .00124574f, -.00147626f, -.00155461f, .0640682f, .134898f, 
	    -.0242415f, 0.f, 0.f, 0.f, 6.13666e-4f, -.00540373f, 2.61635e-5f, 
	    -.00333012f, 0.f, -.00308101f, -.00242679f, -.00336086f, 0.f, 
	    -1189.79f, -.0504738f, -.00261547f, -.00103132f, 1.91583e-4f, 
	    -83.8132f, -.0140517f, -.0114167f, -.00408012f, 1.73522e-4f, 
	    -.0139644f, -.0664128f, -.0685152f, -13441.4f, 0.f, 0.f, 0.f, 
	    607.916f, -.0041222f, -.00220996f, 0.f, 1702.77f, -.00463015f, 
	    0.f, 0.f, -.0022536f, -.00296204f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    .0392786f, .0131186f, -.00178086f, 0.f, 0.f, -.390083f, 
	    -.0284741f, -77.84f, -.00102601f, 0.f, -7.26485e-4f, -.00542181f, 
	    -.00559305f, .0122825f, .0123868f, .00668835f, -.0103303f, 
	    -.00951903f, 2.70021e-4f, -.0257084f, -.013243f, 0.f, -.0381f, 
	    -.0031681f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, -9.05762e-4f, 
	    -.0021459f, -.00117824f, 3.66732f, -3.79729e-4f, -.00613966f, 
	    -.00509082f, -.00196332f, -.0030828f, -9.75222e-4f, 4.03315f, 
	    -.25271f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 1.16112f, 0.f, 0.f, .0333725f, 0.f, 
	    .0348637f, -.00544368f, 0.f, -.067394f, .174754f, 0.f, 0.f, 0.f, 
	    174.712f, 0.f, .126733f, 0.f, 103.154f, .0552075f, 0.f, 0.f, 
	    8.13525e-4f, 0.f, 0.f, .0866784f, .158727f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, -25.0482f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, -.00248894f, 
	    6.16053e-4f, -5.79716e-4f, .00295482f, .0847001f, .170147f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, 2.47425e-5f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, .944846f, 0.f, 0.f, -.0308617f, 0.f, -.0244019f, 
	    .00648607f, 0.f, .0308181f, .0459392f, 0.f, 0.f, 0.f, 174.712f, 
	    0.f, .021326f, 0.f, -356.958f, 0.f, 1.82278e-4f, 0.f, 3.07472e-4f,
	     0.f, 0.f, .0866784f, .158727f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    .00383054f, 0.f, 0.f, -.00193065f, -.0014509f, 0.f, 0.f, 0.f, 0.f,
	     -.00123493f, .00136736f, .0847001f, .170147f, .00371469f, 0.f, 
	    0.f, 0.f, 0.f, .0051025f, 2.47425e-5f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    .00368756f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
	     0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, 1.3558f, .144816f, 0.f, .0607767f, 0.f, .0294777f, 
	    .07469f, 0.f, -.0923822f, .0857342f, 0.f, 0.f, 0.f, 23.8636f, 0.f,
	     .0771653f, 0.f, 81.8751f, .0187736f, 0.f, 0.f, .0149667f, 0.f, 
	    0.f, .0866784f, .158727f, 0.f, 0.f, 0.f, 0.f, 0.f, -367.874f, 
	    .00548158f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    .0847001f, .170147f, .0122631f, 0.f, 0.f, 0.f, 0.f, .00817187f, 
	    3.71617e-5f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, -.00210826f, -.0031364f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, -.0735742f, -.0500266f, 0.f, 0.f, 0.f, 
	    0.f, .0194965f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, 1.04761f, .200165f, .237697f, .0368552f, 0.f, 
	    .0357202f, -.214075f, 0.f, -.108018f, -.373981f, 0.f, .0310022f, 
	    -.00116305f, -20.7596f, 0.f, .0864502f, 0.f, 97.4908f, .0516707f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, .0866784f, .158727f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 346.193f, .0134297f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    -.00348509f, -1.54689e-4f, 0.f, 0.f, .0847001f, .170147f, 
	    .0147753f, 0.f, 0.f, 0.f, 0.f, .018932f, 3.68181e-5f, .013257f, 
	    0.f, 0.f, .00359719f, .00744328f, -.00100023f, -6505.28f, 0.f, 
	    .0103485f, -.00100983f, -.00406916f, -66.0864f, -.0171533f, 
	    .0110605f, .01203f, -.00520034f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, -2627.69f, .00713755f, .00417999f, 0.f, 12591.f, 0.f, 0.f, 
	    0.f, -.00223595f, .00460217f, .00571794f, 0.f, 0.f, 0.f, 0.f, 
	    -.0318353f, -.0235526f, -.0136189f, 0.f, 0.f, 0.f, .0203522f, 
	    -66.7837f, -.00109724f, 0.f, -.0138821f, .0160468f, 0.f, 0.f, 0.f,
	     0.f, 0.f, 0.f, 0.f, .0151574f, -5.4447e-4f, 0.f, .0728224f, 
	    .0659413f, 0.f, -.00515692f, 0.f, 0.f, -3703.67f, 0.f, 0.f, 
	    .0136131f, .00538153f, 0.f, 4.76285f, -.0175677f, .0226301f, 0.f, 
	    .0176631f, .00477162f, 0.f, 5.39354f, 0.f, -.0075171f, 0.f, 0.f, 
	    -88.2736f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, 1.26376f, -.214304f, -.149984f, .230404f, .0298237f, 
	    .0268673f, .296228f, .02219f, -.0207655f, .452506f, .120105f, 
	    .032442f, .0424816f, -9.14313f, 0.f, .0247178f, -.0288229f, 
	    81.2805f, .051038f, -.00580611f, 2.51236e-5f, -.0124083f, 0.f, 
	    0.f, .0866784f, .158727f, -.034819f, 0.f, 0.f, 2.89885e-5f, 0.f, 
	    153.595f, -.0168604f, 0.f, .0101015f, 0.f, 0.f, 0.f, 0.f, 
	    2.84552e-4f, -.00122181f, 0.f, 0.f, .0847001f, .170147f, 
	    -.0104927f, 0.f, 0.f, 0.f, -.00591313f, -.0230501f, 3.14758e-5f, 
	    0.f, 0.f, .0126956f, .00835489f, 3.10513e-4f, 0.f, 3421.19f, 
	    -.00245017f, -4.27154e-4f, 5.45152e-4f, .00189896f, 28.9121f, 
	    -.00649973f, -.0193855f, -.0148492f, 0.f, -.0510576f, .0787306f, 
	    .0951981f, -14942.2f, 0.f, 0.f, 0.f, 265.503f, 0.f, 0.f, 0.f, 0.f,
	     0.f, 0.f, 0.f, .0063711f, 3.24789e-4f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    .0614274f, .0100376f, -8.41083e-4f, 0.f, 0.f, 0.f, -.0127099f, 
	    0.f, 0.f, 0.f, -.00394077f, -.0128601f, -.00797616f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, -.00671465f, -.00169799f, .00193772f, 3.8114f, 
	    -.0077929f, -.0182589f, -.012586f, -.0104311f, -.00302465f, 
	    .00243063f, 3.63237f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
	     0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 70.9557f, -.32674f, 
	    0.f, -.516829f, -.00171664f, .090931f, -.6715f, -.147771f, 
	    -.0927471f, -.230862f, -.15641f, .0134455f, -.119717f, 2.52151f, 
	    0.f, -.241582f, .0592939f, 4.39756f, .091528f, .00441292f, 0.f, 
	    .00866807f, 0.f, 0.f, .0866784f, .158727f, .0974701f, 0.f, 0.f, 
	    0.f, 0.f, 67.0217f, -.0013166f, 0.f, -.0165317f, 0.f, 0.f, 
	    .0850247f, 27.7428f, .00498658f, .00615115f, .00950156f, 
	    -.0212723f, .0847001f, .170147f, -.0238645f, 0.f, 0.f, 0.f, 
	    .0013738f, -.00841918f, 2.80145e-5f, .00712383f, 0.f, -.0166209f, 
	    1.03533e-4f, -.0168898f, 0.f, 3645.26f, 0.f, .00654077f, 
	    3.6913e-4f, 9.94419e-4f, 84.2803f, -.0116124f, -.00774414f, 
	    -.00168844f, .00142809f, -.00192955f, .117225f, -.0241512f, 
	    15052.1f, 0.f, 0.f, 0.f, 1602.61f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, -3.54403e-4f, -.018727f, 0.f, 0.f, 0.f, 0.f, 0.f, .0276439f, 
	    .00643207f, -.03543f, 0.f, 0.f, 0.f, -.0280221f, 81.1228f, 
	    -6.75255e-4f, 0.f, -.0105162f, -.00348292f, -.00697321f, 0.f, 0.f,
	     0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, -.00145546f, -.013197f, -.00357751f, -1.09021f, 
	    -.0150181f, -.00712841f, -.0066459f, -.0035261f, -.0187773f, 
	    -.00222432f, -.393895f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, .060405f, 
	    1.57034f, .0299387f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, -1.51018f, 0.f,
	     0.f, 0.f, -8.6165f, .0126454f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    .00550878f, 0.f, 0.f, .0866784f, .158727f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, .0623881f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, .0847001f, .170147f, -.0945934f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, .956827f, .0620637f, 
	    .0318433f, 0.f, 0.f, .03949f, 0.f, 0.f, -.00924882f, -.00794023f, 
	    0.f, 0.f, 0.f, 174.712f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    .00274677f, 0.f, .0154951f, .0866784f, .158727f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, -6.99007e-4f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, .0124362f, -.00528756f, .0847001f, .170147f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, 2.47425e-5f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    1.0993f, 3.90631f, 3.07165f, .986161f, 16.3536f, 4.6383f, 1.f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, 1.2884f, .0310302f, .118339f, 1.f, .7f, 1.1502f, 
	    3.44689f, 1.2884f, 1.f, 1.08738f, 1.22947f, 1.10016f, .734129f, 
	    1.15241f, 2.22784f, .795046f, 4.01612f, 4.47749f, 123.435f, 
	    -.0760535f, 1.68986e-6f, .744294f, 1.03604f, 172.783f, 1.1502f, 
	    3.44689f, -.74623f, .949154f, 1.00858f, .0456011f, -.0222972f, 
	    -.0544388f, 5.23136e-4f, -.0188849f, .0523707f, -.00943646f, 
	    .00631707f, -.078046f, -.048843f, 0.f, 0.f, -7.6025f, 0.f, 
	    -.0144635f, -.0176843f, -121.517f, .0285647f, 0.f, 0.f, 
	    6.31792e-4f, 0.f, .00577197f, .0866784f, .158727f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, -8902.72f, .00330611f, .00302172f, 0.f, -.00213673f, 
	    -3.2091e-4f, 0.f, 0.f, .00276034f, .00282487f, -2.97592e-4f, 
	    -.00421534f, .0847001f, .170147f, .00896456f, 0.f, -.0108596f, 
	    0.f, 0.f, .00557917f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
	     0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, .00965405f, 0.f,
	     0.f, 2.f, .939664f, .0856514f, -.00679989f, .0265929f, 
	    -.00474283f, .0121855f, -.0214905f, .00649651f, -.0205477f, 
	    -.0424952f, 0.f, 0.f, 0.f, 11.9148f, 0.f, .0118777f, -.072823f, 
	    -81.5965f, .0173887f, 0.f, 0.f, 0.f, -.0144691f, 2.80259e-4f, 
	    .0866784f, .158727f, 0.f, 0.f, 0.f, 0.f, 0.f, 216.584f, 
	    .00318713f, .00737479f, 0.f, -.00255018f, -.00392806f, 0.f, 0.f, 
	    -.00289757f, -.00133549f, .00102661f, 3.53775e-4f, .0847001f, 
	    .170147f, -.00917497f, 0.f, 0.f, 0.f, 0.f, .00356082f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, -.0100902f, 0.f, 0.f, 2.f, .985982f, 
	    -.0455435f, .0121106f, .0204127f, -.00240836f, .0111383f, 
	    -.0451926f, .0135074f, -.00654139f, .115275f, .128247f, 0.f, 0.f, 
	    -5.30705f, 0.f, -.0379332f, -.0624741f, .771062f, .0296315f, 0.f, 
	    0.f, 0.f, .00681051f, -.00434767f, .0866784f, .158727f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, 10.7003f, -.00276907f, 4.32474e-4f, 0.f, 
	    .00131497f, -6.47517e-4f, 0.f, -22.0621f, -.00110804f, 
	    -8.09338e-4f, 4.18184e-4f, .0042965f, .0847001f, .170147f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, -.00404337f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, -9.5255e-4f, 8.56253e-4f, 
	    4.33114e-4f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, .00121223f, 
	    2.38694e-4f, 9.15245e-4f, .00128385f, 8.67668e-4f, -5.61425e-6f, 
	    1.04445f, 34.1112f, 0.f, -.840704f, -239.639f, .706668f, 
	    -20.5873f, -.363696f, 23.9245f, 0.f, -.00106657f, -7.67292e-4f, 
	    1.54534e-4f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 2.f, 1.0032f, 
	    .0383501f, -.00238983f, .0028395f, .00420956f, 5.86619e-4f, 
	    .0219054f, -.0100946f, -.00350259f, .0417392f, -.00844404f, 0.f, 
	    0.f, 4.96949f, 0.f, -.00706478f, -.0146494f, 31.3258f, 
	    -.00186493f, 0.f, -.0167499f, 0.f, 0.f, 5.12686e-4f, .0866784f, 
	    .158727f, -.00464167f, 0.f, 0.f, 0.f, .00437353f, -199.069f, 0.f, 
	    -.00534884f, 0.f, .00162458f, .00293016f, .00267926f, 590.449f, 
	    0.f, 0.f, -.00117266f, -3.5889e-4f, .0847001f, .170147f, 0.f, 0.f,
	     .0138673f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
	     0.f, 0.f, 0.f, 0.f, 0.f, .00160571f, 6.28078e-4f, 5.05469e-5f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, -.00157829f, -4.00855e-4f, 
	    5.04077e-5f, -.00139001f, -.00233406f, -4.81197e-4f, 1.46758f, 
	    6.20332f, 0.f, .366476f, -61.976f, .309198f, -19.8999f, 0.f, 
	    -329.933f, 0.f, -.0011008f, -9.3931e-5f, 1.39638e-4f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, 2.f, .981637f, -.00141317f, .0387323f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, -.0358707f, -.00863658f, 0.f, 0.f, 
	    -2.02226f, 0.f, -.00869424f, -.0191397f, 87.6779f, .00452188f, 
	    0.f, .022376f, 0.f, 0.f, 0.f, 0.f, 0.f, -.00707572f, 0.f, 0.f, 
	    0.f, -.0041121f, 35.006f, 0.f, 0.f, 0.f, 0.f, 0.f, -.00836657f, 
	    16.1347f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, -.014513f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, .00124152f, 6.43365e-4f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, .00133255f, .00242657f, .00160666f, -.00185728f, 
	    -.00146874f, -4.79163e-6f, 1.22464f, 35.351f, 0.f, .449223f, 
	    -47.7466f, .470681f, 8.41861f, -.288198f, 167.854f, 0.f, 
	    7.11493e-4f, 6.05601e-4f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 2.f, 
	    1.00422f, -.00711212f, .0052448f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    -.0528914f, -.0241301f, 0.f, 0.f, -21.2219f, -.010383f, 
	    -.00328077f, .0165727f, 1.68564f, -.00668154f, 0.f, .0145155f, 
	    0.f, .00842365f, 0.f, 0.f, 0.f, -.00434645f, 0.f, 0.f, .021678f, 
	    0.f, -138.459f, 0.f, 0.f, 0.f, 0.f, 0.f, .00704573f, -47.3204f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, .0108767f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, -.00808279f, 0.f, 0.f, 
	    0.f, 0.f, 5.21769e-4f, -2.27387e-4f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
	     0.f, 0.f, .00326769f, .00316901f, 4.60316e-4f, -1.01431e-4f, 
	    .00102131f, 9.96601e-4f, 1.25707f, 25.0114f, 0.f, .424472f, 
	    -27.7655f, .344625f, 27.5412f, 0.f, 794.251f, 0.f, .00245835f, 
	    .00138871f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 2.f, 1.0189f, 
	    -.0246603f, .0100078f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, -.0670977f, 
	    -.0402286f, 0.f, 0.f, -22.9466f, -.00747019f, .0022658f, 
	    .0263931f, 37.2625f, -.00639041f, 0.f, .00958383f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, -.00185291f, 0.f, 0.f, 0.f, 0.f, 139.717f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, .00919771f, -369.121f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, -.0157067f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
	     0.f, 0.f, 0.f, -.00707265f, 0.f, 0.f, 0.f, 0.f, -.00292953f, 
	    -.00277739f, -4.40092e-4f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    .0024728f, 2.95035e-4f, -.00181246f, .00281945f, .00427296f, 
	    9.78863e-4f, 1.40545f, -6.19173f, 0.f, 0.f, -79.3632f, .444643f, 
	    -403.085f, 0.f, 11.5603f, 0.f, .00225068f, 8.48557e-4f, 
	    -2.98493e-4f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 2.f, .975801f, 
	    .038068f, -.0305198f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, .0385575f, 
	    .0504057f, 0.f, 0.f, -176.046f, .0144594f, -.00148297f, 
	    -.0036856f, 30.2185f, -.00323338f, 0.f, .0153569f, 0.f, 
	    -.0115558f, 0.f, 0.f, 0.f, .0048962f, 0.f, 0.f, -.0100616f, 
	    -.00821324f, -157.757f, 0.f, 0.f, 0.f, 0.f, 0.f, .00663564f, 
	    45.841f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, -.025128f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, .00991215f, 0.f,
	     0.f, 0.f, 0.f, -8.73148e-4f, -.00129648f, -7.32026e-5f, 0.f, 0.f,
	     0.f, 0.f, 0.f, 0.f, 0.f, -.0046811f, -.00466003f, -.00131567f, 
	    -7.3939e-4f, 6.32499e-4f, -4.65588e-4f, -1.29785f, -157.139f, 0.f,
	     .25835f, -36.9453f, .410672f, 9.78196f, -.152064f, -3850.84f, 
	    0.f, -8.52706e-4f, -.00140945f, -7.26786e-4f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, 2.f, .960722f, .0703757f, -.0300266f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, .0222671f, .0410423f, 0.f, 0.f, -163.07f, 
	    .0106073f, 5.40747e-4f, .00779481f, 144.908f, 1.51484e-4f, 0.f, 
	    .0197547f, 0.f, -.0141844f, 0.f, 0.f, 0.f, .00577884f, 0.f, 0.f, 
	    .00974319f, 0.f, -2880.15f, 0.f, 0.f, 0.f, 0.f, 0.f, -.00444902f, 
	    -29.276f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, .0234419f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, .00536685f, 0.f,
	     0.f, 0.f, 0.f, -4.65325e-4f, -5.50628e-4f, 3.31465e-4f, 0.f, 0.f,
	     0.f, 0.f, 0.f, 0.f, 0.f, -.00206179f, -.00308575f, -7.93589e-4f, 
	    -1.08629e-4f, 5.95511e-4f, -9.0505e-4f, 1.18997f, 41.5924f, 0.f, 
	    -.472064f, -947.15f, .398723f, 19.8304f, 0.f, 3732.19f, 0.f, 
	    -.0015004f, -.00114933f, -1.56769e-4f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 2.f, 1.03123f, -.0705124f, .00871615f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, -.0382621f, -.00980975f, 0.f, 0.f, 28.9286f, .00957341f,
	     0.f, 0.f, 86.6153f, 7.91938e-4f, 0.f, 0.f, 0.f, .00468917f, 0.f, 
	    0.f, 0.f, .00786638f, 0.f, 0.f, .00990827f, 0.f, 65.5573f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, -40.02f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, .00707457f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
	     0.f, 0.f, .00572268f, 0.f, 0.f, 0.f, 0.f, -2.0497e-4f, .0012156f,
	     -8.05579e-6f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, -.00249941f, 
	    -4.57256e-4f, -1.59311e-4f, 2.96481e-4f, -.00177318f, 
	    -6.37918e-4f, 1.02395f, 12.8172f, 0.f, .149903f, -26.3818f, 0.f, 
	    47.0628f, -.222139f, .0482292f, 0.f, -8.67075e-4f, -5.86479e-4f, 
	    5.32462e-4f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 2.f, 1.00828f, 
	    -.0910404f, -.0226549f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, -.023242f, 
	    -.00908925f, 0.f, 0.f, 33.6105f, 0.f, 0.f, 0.f, -12.4957f, 
	    -.00587939f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, 27.9765f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 2012.37f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, -.0175553f, 0.f, 0.f, 0.f, 0.f,
	     0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    .00329699f, .00126659f, 2.68402e-4f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
	     0.f, .00117894f, .00148746f, 1.06478e-4f, 1.34743e-4f, 
	    -.00220939f, -6.23523e-4f, .636539f, 11.3621f, 0.f, -.393777f, 
	    2386.87f, 0.f, 661.865f, -.121434f, 9.27608f, 0.f, 1.68478e-4f, 
	    .00124892f, .00171345f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 2.f, 
	    1.57293f, -.6784f, .6475f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    -.0762974f, -.360423f, 0.f, 0.f, 128.358f, 0.f, 0.f, 0.f, 
	    46.8038f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, -.167898f, 0.f, 
	    0.f, 0.f, 0.f, 29099.4f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 31.5706f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 2.f, .860028f, .377052f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, -1.1757f, 0.f, 0.f, 0.f, 
	    .00777757f, 0.f, 0.f, 0.f, 101.024f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
	     0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 654.251f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    -.0156959f, .0191001f, .0315971f, .0100982f, -.00671565f, 
	    .00257693f, 1.38692f, .282132f, 0.f, 0.f, 381.511f, 0.f, 0.f, 0.f,
	     0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 2.f, 
	    1.06029f, -.0525231f, .373034f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    .0331072f, -.388409f, 0.f, 0.f, -165.295f, -.213801f, -.0438916f, 
	    -.322716f, -88.2393f, .118458f, 0.f, -.435863f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, -.119782f, 0.f, 0.f, 0.f, 0.f, 26.2229f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, -53.7443f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    -.455788f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, .0384009f, .0396733f, 0.f, 0.f, 0.f, 0.f,
	     0.f, 0.f, 0.f, 0.f, .0505494f, .0739617f, .01922f, -.00846151f, 
	    -.0134244f, .0196338f, 1.50421f, 18.8368f, 0.f, 0.f, -51.3114f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, .0511923f, .0361225f, 0.f, 0.f, 0.f, 0.f,
	     0.f, 0.f, 0.f, 2.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 
	    1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 
	    1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 
	    1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 
	    1.f, 1.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 
	    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f };

struct {
    real e_1[10];
    } mavg7_ = { 261.f, 264.f, 229.f, 217.f, 217.f, 223.f, 286.76f, -2.9394f, 
	    2.5f, 0.f };

struct {
    integer e_1;
    } metsel_00 = { 0 };


/* Table of constant values */

static integer c__1 = 1;
static real c_b14 = 1.f;
static real c_b15 = 0.f;
static integer c__48 = 48;
static integer c__2 = 2;
static real c_b33 = 1e3f;
static real c_b38 = 28.f;
static real c_b47 = 4.f;
static real c_b51 = 16.f;
static real c_b55 = 32.f;
static real c_b60 = 40.f;
static real c_b68 = 14.f;
static doublereal c_b89 = .5;
static real c_b98 = 2.f;

/* ----------------------------------------------------------------------- */
/* Subroutine */ int gtd7_(integer *iyd, real *sec, real *alt, real *glat, 
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
    static real v1, ds[9], ts[2], tz, dmc, dmr, dz28, xmm;
    static integer mss;
    extern /* Subroutine */ int gts7_(integer *, real *, real *, real *, real 
	    *, real *, real *, real *, real *, integer *, real *, real *);
    static real dm28m, altt, xlat;
    extern doublereal vtst7_(integer *, real *, real *, real *, real *, real *
	    , real *, real *, integer *);
    extern /* Subroutine */ int glatf_00(real *, real *, real *);
    extern doublereal densm_00(real *, real *, real *, real *, integer *, real *
	    , real *, real *, integer *, real *, real *, real *), glob7s_(
	    real *);
    /* Subroutine */ int tselec_00(real *);


/*     NRLMSISE-00 */
/*     ----------- */
/*        Neutral Atmosphere Empirical Model from the surface to lower */
/*        exosphere */

/*        NEW FEATURES: */
/*          *Extensive satellite drag database used in model generation */
/*          *Revised O2 (and O) in lower thermosphere */
/*          *Additional nonlinear solar activity term */
/*          *"ANOMALOUS OXYGEN" NUMBER DENSITY, OUTPUT D(9) */
/*           At high altitudes (> 500 km), hot atomic oxygen or ionized */
/*           oxygen can become appreciable for some ranges of subroutine */
/*           inputs, thereby affecting drag on satellites and debris. We */
/*           group these species under the term "anomalous oxygen," since */
/*           their individual variations are not presently separable with */
/*           the drag data used to define this model component. */

/*        SUBROUTINES FOR SPECIAL OUTPUTS: */

/*        HIGH ALTITUDE DRAG: EFFECTIVE TOTAL MASS DENSITY */
/*        (SUBROUTINE GTD7D, OUTPUT D(6)) */
/*           For atmospheric drag calculations at altitudes above 500 km, */
/*           call SUBROUTINE GTD7D to compute the "effective total mass */
/*           density" by including contributions from "anomalous oxygen." */
/*           See "NOTES ON OUTPUT VARIABLES" below on D(6). */

/*        PRESSURE GRID (SUBROUTINE GHP7) */
/*          See subroutine GHP7 to specify outputs at a pressure level */
/*          rather than at an altitude. */

/*        OUTPUT IN M-3 and KG/M3:   CALL METERS(.TRUE.) */

/*     INPUT VARIABLES: */
/*        IYD - YEAR AND DAY AS YYDDD (day of year from 1 to 365 (or 366)) */
/*              (Year ignored in current model) */
/*        SEC - UT(SEC) */
/*        ALT - ALTITUDE(KM) */
/*        GLAT - GEODETIC LATITUDE(DEG) */
/*        GLONG - GEODETIC LONGITUDE(DEG) */
/*        STL - LOCAL APPARENT SOLAR TIME(HRS; see Note below) */
/*        F107A - 81 day AVERAGE OF F10.7 FLUX (centered on day DDD) */
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
/*             (7) AVERAGE OF EIGHT 3 HR AP INDICIES FROM 36 TO 57 HRS PRIOR */
/*                    TO CURRENT TIME */
/*        MASS - MASS NUMBER (ONLY DENSITY FOR SELECTED GAS IS */
/*                 CALCULATED.  MASS 0 IS TEMPERATURE.  MASS 48 FOR ALL. */
/*                 MASS 17 IS Anomalous O ONLY.) */

/*     NOTES ON INPUT VARIABLES: */
/*        UT, Local Time, and Longitude are used independently in the */
/*        model and are not of equal importance for every situation. */
/*        For the most physically realistic calculation these three */
/*        variables should be consistent (STL=SEC/3600+GLONG/15). */
/*        The Equation of Time departures from the above formula */
/*        for apparent local time can be included if available but */
/*        are of minor importance. */

/*        F107 and F107A values used to generate the model correspond */
/*        to the 10.7 cm radio flux at the actual distance of the Earth */
/*        from the Sun rather than the radio flux at 1 AU. The following */
/*        site provides both classes of values: */
/*        ftp://ftp.ngdc.noaa.gov/STP/SOLAR_DATA/SOLAR_RADIO/FLUX/ */

/*        F107, F107A, and AP effects are neither large nor well */
/*        established below 80 km and these parameters should be set to */
/*        150., 150., and 4. respectively. */

/*     OUTPUT VARIABLES: */
/*        D(1) - HE NUMBER DENSITY(CM-3) */
/*        D(2) - O NUMBER DENSITY(CM-3) */
/*        D(3) - N2 NUMBER DENSITY(CM-3) */
/*        D(4) - O2 NUMBER DENSITY(CM-3) */
/*        D(5) - AR NUMBER DENSITY(CM-3) */
/*        D(6) - TOTAL MASS DENSITY(GM/CM3) */
/*        D(7) - H NUMBER DENSITY(CM-3) */
/*        D(8) - N NUMBER DENSITY(CM-3) */
/*        D(9) - Anomalous oxygen NUMBER DENSITY(CM-3) */
/*        T(1) - EXOSPHERIC TEMPERATURE */
/*        T(2) - TEMPERATURE AT ALT */

/*     NOTES ON OUTPUT VARIABLES: */
/*        TO GET OUTPUT IN M-3 and KG/M3:   CALL METERS(.TRUE.) */

/*        O, H, and N are set to zero below 72.5 km */

/*        T(1), Exospheric temperature, is set to global average for */
/*        altitudes below 120 km. The 120 km gradient is left at global */
/*        average value for altitudes below 72 km. */

/*        D(6), TOTAL MASS DENSITY, is NOT the same for subroutines GTD7 */
/*        and GTD7D */

/*          SUBROUTINE GTD7 -- D(6) is the sum of the mass densities of the */
/*          species labeled by indices 1-5 and 7-8 in output variable D. */
/*          This includes He, O, N2, O2, Ar, H, and N but does NOT include */
/*          anomalous oxygen (species index 9). */

/*          SUBROUTINE GTD7D -- D(6) is the "effective total mass density */
/*          for drag" and is the sum of the mass densities of all species */
/*          in this model, INCLUDING anomalous oxygen. */

/*     SWITCHES: The following is for test and special purposes: */

/*        TO TURN ON AND OFF PARTICULAR VARIATIONS CALL TSELEC(SW), */
/*        WHERE SW IS A 25 ELEMENT ARRAY CONTAINING 0. FOR OFF, 1. */
/*        FOR ON, OR 2. FOR MAIN EFFECTS OFF BUT CROSS TERMS ON */
/*        FOR THE FOLLOWING VARIATIONS */
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

/*        To get current values of SW: CALL TRETRV(SW) */

/* XX      COMMON/DATIM7/ISD(3),IST(2),NAM(2) */
/* XX      COMMON/DATIME/ISDATE(3),ISTIME(2),NAME(2) */
    /* Parameter adjustments */
    --t;
    --d__;
    --ap;

    /* Function Body */
    if (csw_1.isw != 64999) {
	tselec_00(sv);
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

/*        Test for changed input */
    v1 = vtst7_(iyd, sec, glat, glong, stl, f107a, f107, &ap[1], &c__1);
/*       Latitude variation of gravity (none for SW(2)=0) */
    xlat = *glat;
    if (csw_1.sw[1] == 0.f) {
	xlat = 45.f;
    }
    glatf_00(&xlat, &parmb_1.gsurf, &parmb_1.re);

    xmm = lower7_1.pdm[24];

/*       THERMOSPHERE/MESOSPHERE (above ZN2(1)) */
    altt = dmax(*alt,zn2[0]);
    mss = *mass;
/*       Only calculate N2 in thermosphere if alt in mixed region */
    if (*alt < zmix && *mass > 0) {
	mss = 28;
    }
/*       Only calculate thermosphere if input parameters changed */
/*         or altitude above ZN2(1) in mesosphere */
    if (v1 == 1.f || *alt > zn2[0] || alast > zn2[0] || mss != mssl) {
	gts7_(iyd, sec, &altt, glat, glong, stl, f107a, f107, &ap[1], &mss, 
		ds, ts);
	dm28m = dmix_1.dm28;
/*         metric adjustment */
	if (metsel_1.imr == 1) {
	    dm28m = dmix_1.dm28 * 1e6f;
	}
	mssl = mss;
    }
    t[1] = ts[0];
    t[2] = ts[1];
    if (*alt >= zn2[0]) {
	for (j = 1; j <= 9; ++j) {
	    d__[j] = ds[j - 1];
/* L5: */
	}
	goto L10;
    }

/*       LOWER MESOSPHERE/UPPER STRATOSPHERE [between ZN3(1) and ZN2(1)] */
/*         Temperature at nodes and gradients at end nodes */
/*         Inverse temperature a linear function of spherical harmonics */
/*         Only calculate nodes if input changed */
    if (v1 == 1.f || alast >= zn2[0]) {
	meso7_1.tgn2[0] = meso7_1.tgn1[1];
	meso7_1.tn2[0] = meso7_1.tn1[4];
	meso7_1.tn2[1] = parm7_1.pma[0] * mavg7_1.pavgm[0] / (1.f - csw_1.sw[
		19] * glob7s_(parm7_1.pma));
	meso7_1.tn2[2] = parm7_1.pma[100] * mavg7_1.pavgm[1] / (1.f - 
		csw_1.sw[19] * glob7s_(&parm7_1.pma[100]));
	meso7_1.tn2[3] = parm7_1.pma[200] * mavg7_1.pavgm[2] / (1.f - 
		csw_1.sw[19] * csw_1.sw[21] * glob7s_(&parm7_1.pma[200]));
/* Computing 2nd power */
	r__1 = parm7_1.pma[200] * mavg7_1.pavgm[2];
	meso7_1.tgn2[1] = mavg7_1.pavgm[8] * parm7_1.pma[900] * (csw_1.sw[19] 
		* csw_1.sw[21] * glob7s_(&parm7_1.pma[900]) + 1.f) * 
		meso7_1.tn2[3] * meso7_1.tn2[3] / (r__1 * r__1);
	meso7_1.tn3[0] = meso7_1.tn2[3];
    }
    if (*alt >= zn3[0]) {
	goto L6;
    }

/*       LOWER STRATOSPHERE AND TROPOSPHERE [below ZN3(1)] */
/*         Temperature at nodes and gradients at end nodes */
/*         Inverse temperature a linear function of spherical harmonics */
/*         Only calculate nodes if input changed */
    if (v1 == 1.f || alast >= zn3[0]) {
	meso7_1.tgn3[0] = meso7_1.tgn2[1];
	meso7_1.tn3[1] = parm7_1.pma[300] * mavg7_1.pavgm[3] / (1.f - 
		csw_1.sw[21] * glob7s_(&parm7_1.pma[300]));
	meso7_1.tn3[2] = parm7_1.pma[400] * mavg7_1.pavgm[4] / (1.f - 
		csw_1.sw[21] * glob7s_(&parm7_1.pma[400]));
	meso7_1.tn3[3] = parm7_1.pma[500] * mavg7_1.pavgm[5] / (1.f - 
		csw_1.sw[21] * glob7s_(&parm7_1.pma[500]));
	meso7_1.tn3[4] = parm7_1.pma[600] * mavg7_1.pavgm[6] / (1.f - 
		csw_1.sw[21] * glob7s_(&parm7_1.pma[600]));
/* Computing 2nd power */
	r__1 = parm7_1.pma[600] * mavg7_1.pavgm[6];
	meso7_1.tgn3[1] = parm7_1.pma[700] * mavg7_1.pavgm[7] * (csw_1.sw[21] 
		* glob7s_(&parm7_1.pma[700]) + 1.f) * meso7_1.tn3[4] * 
		meso7_1.tn3[4] / (r__1 * r__1);
    }
L6:
    if (*mass == 0) {
	goto L50;
    }
/*          LINEAR TRANSITION TO FULL MIXING BELOW ZN2(1) */
    dmc = 0.f;
    if (*alt > zmix) {
	dmc = 1.f - (zn2[0] - *alt) / (zn2[0] - zmix);
    }
    dz28 = ds[2];
/*      ***** N2 DENSITY **** */
    dmr = ds[2] / dm28m - 1.f;
    d__[3] = densm_00(alt, &dm28m, &xmm, &tz, &mn3, zn3, meso7_1.tn3, 
	    meso7_1.tgn3, &mn2, zn2, meso7_1.tn2, meso7_1.tgn2);
    d__[3] *= dmr * dmc + 1.f;
/*      ***** HE DENSITY **** */
    d__[1] = 0.f;
    if (*mass != 4 && *mass != 48) {
	goto L204;
    }
    dmr = ds[0] / (dz28 * lower7_1.pdm[1]) - 1.f;
    d__[1] = d__[3] * lower7_1.pdm[1] * (dmr * dmc + 1.f);
L204:
/*      **** O DENSITY **** */
    d__[2] = 0.f;
    d__[9] = 0.f;
/* L216: */
/*      ***** O2 DENSITY **** */
    d__[4] = 0.f;
    if (*mass != 32 && *mass != 48) {
	goto L232;
    }
    dmr = ds[3] / (dz28 * lower7_1.pdm[31]) - 1.f;
    d__[4] = d__[3] * lower7_1.pdm[31] * (dmr * dmc + 1.f);
L232:
/*      ***** AR DENSITY **** */
    d__[5] = 0.f;
    if (*mass != 40 && *mass != 48) {
	goto L240;
    }
    dmr = ds[4] / (dz28 * lower7_1.pdm[41]) - 1.f;
    d__[5] = d__[3] * lower7_1.pdm[41] * (dmr * dmc + 1.f);
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
    gts3c_1.dd = densm_00(alt, &c_b14, &c_b15, &tz, &mn3, zn3, meso7_1.tn3, 
	    meso7_1.tgn3, &mn2, zn2, meso7_1.tn2, meso7_1.tgn2);
    t[2] = tz;
L90:
    alast = *alt;
    return 0;
} /* gtd7_ */

/* ----------------------------------------------------------------------- */
/* Subroutine */ int gtd7d_(integer *iyd, real *sec, real *alt, real *glat, 
	real *glong, real *stl, real *f107a, real *f107, real *ap, integer *
	mass, real *d__, real *t)
{
    extern /* Subroutine */ int gtd7_(integer *, real *, real *, real *, real 
	    *, real *, real *, real *, real *, integer *, real *, real *);


/*     NRLMSISE-00 */
/*     ----------- */
/*        This subroutine provides Effective Total Mass Density for */
/*        output D(6) which includes contributions from "anomalous */
/*        oxygen" which can affect satellite drag above 500 km.  This */
/*        subroutine is part of the distribution package for the */
/*        Neutral Atmosphere Empirical Model from the surface to lower */
/*        exosphere.  See subroutine GTD7 for more extensive comments. */

/*     INPUT VARIABLES: */
/*        IYD - YEAR AND DAY AS YYDDD (day of year from 1 to 365 (or 366)) */
/*              (Year ignored in current model) */
/*        SEC - UT(SEC) */
/*        ALT - ALTITUDE(KM) */
/*        GLAT - GEODETIC LATITUDE(DEG) */
/*        GLONG - GEODETIC LONGITUDE(DEG) */
/*        STL - LOCAL APPARENT SOLAR TIME(HRS; see Note below) */
/*        F107A - 81 day AVERAGE OF F10.7 FLUX (centered on day DDD) */
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
/*             (7) AVERAGE OF EIGHT 3 HR AP INDICIES FROM 36 TO 57 HRS PRIOR */
/*                    TO CURRENT TIME */
/*        MASS - MASS NUMBER (ONLY DENSITY FOR SELECTED GAS IS */
/*                 CALCULATED.  MASS 0 IS TEMPERATURE.  MASS 48 FOR ALL. */
/*                 MASS 17 IS Anomalous O ONLY.) */

/*     NOTES ON INPUT VARIABLES: */
/*        UT, Local Time, and Longitude are used independently in the */
/*        model and are not of equal importance for every situation. */
/*        For the most physically realistic calculation these three */
/*        variables should be consistent (STL=SEC/3600+GLONG/15). */
/*        The Equation of Time departures from the above formula */
/*        for apparent local time can be included if available but */
/*        are of minor importance. */

/*        F107 and F107A values used to generate the model correspond */
/*        to the 10.7 cm radio flux at the actual distance of the Earth */
/*        from the Sun rather than the radio flux at 1 AU. */

/*     OUTPUT VARIABLES: */
/*        D(1) - HE NUMBER DENSITY(CM-3) */
/*        D(2) - O NUMBER DENSITY(CM-3) */
/*        D(3) - N2 NUMBER DENSITY(CM-3) */
/*        D(4) - O2 NUMBER DENSITY(CM-3) */
/*        D(5) - AR NUMBER DENSITY(CM-3) */
/*        D(6) - TOTAL MASS DENSITY(GM/CM3) [includes anomalous oxygen] */
/*        D(7) - H NUMBER DENSITY(CM-3) */
/*        D(8) - N NUMBER DENSITY(CM-3) */
/*        D(9) - Anomalous oxygen NUMBER DENSITY(CM-3) */
/*        T(1) - EXOSPHERIC TEMPERATURE */
/*        T(2) - TEMPERATURE AT ALT */

    /* Parameter adjustments */
    --t;
    --d__;
    --ap;

    /* Function Body */
    gtd7_(iyd, sec, alt, glat, glong, stl, f107a, f107, &ap[1], mass, &d__[1],
	     &t[1]);
/*       TOTAL MASS DENSITY */

    if (*mass == 48) {
	d__[6] = (d__[1] * 4.f + d__[2] * 16.f + d__[3] * 28.f + d__[4] * 
		32.f + d__[5] * 40.f + d__[7] + d__[8] * 14.f + d__[9] * 16.f)
		 * 1.66e-24f;
	if (metsel_1.imr == 1) {
	    d__[6] /= 1e3f;
	}
    }
    return 0;
} /* gtd7d_ */

/* ----------------------------------------------------------------------- */
/* Subroutine */ int ghp7_(integer *iyd, real *sec, real *alt, real *glat, 
	real *glong, real *stl, real *f107a, real *f107, real *ap, real *d__, 
	real *t, real *press)
{
    /* Initialized data */

    static real bm = 1.3806e-19f;
    static real rgas = 831.4f;
    static real test = 4.3e-4f;
    static integer ltest = 12;

    /* System generated locals */
    real r__1;

    /* Builtin functions */
//    double r_lg10(real *);

    /* Local variables */
    static real g;
    static integer l;
    static real p, z__, ca, cd, cl, sh, pl, zi, xm, xn, cl2;
    extern /* Subroutine */ int gtd7_(integer *, real *, real *, real *, real 
	    *, real *, real *, real *, real *, integer *, real *, real *);
    static real diff;
    static integer iday;

/*       FIND ALTITUDE OF PRESSURE SURFACE (PRESS) FROM GTD7 */
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
/*        D(9) - HOT O NUMBER DENSITY(CM-3) */
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
/*      ITERATION LOOP */
    l = 0;
L10:
    ++l;
    gtd7_(iyd, sec, &z__, glat, glong, stl, f107a, f107, &ap[1], &c__48, &d__[
	    1], &t[1]);
    xn = d__[1] + d__[2] + d__[3] + d__[4] + d__[5] + d__[7] + d__[8];
    p = bm * xn * t[2];
    if (metsel_1.imr == 1) {
	p *= 1e-6f;
    }
    diff = pl - log10(p);
    if (dabs(diff) < test || l == ltest) {
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
    if (l < 6) {
	z__ -= sh * diff * 2.302f;
    } else {
	z__ -= sh * diff;
    }
    goto L10;
L20:
/* XX      IF(L.EQ.LTEST) WRITE(6,100) PRESS,DIFF */
/* XX  100 FORMAT(1X,29HGHP7 NOT CONVERGING FOR PRESS, 1PE12.2,E12.2) */
    *alt = z__;
    return 0;
} /* ghp7_ */

/* ----------------------------------------------------------------------- */
/* Subroutine */ int glatf_00(real *lat, real *gv, real *reff)
{
    /* Initialized data */

    static real dgtr = .0174533f;

    /* Builtin functions */
//    double cos(doublereal);

    /* Local variables */
    static real c2;

/*      CALCULATE LATITUDE VARIABLE GRAVITY (GV) AND EFFECTIVE */
/*      RADIUS (REFF) */
    c2 = cos(2.f * dgtr * *lat);
    *gv = (1.f - c2 * .0026373f) * 980.616f;
    *reff = *gv * 2.f / (c2 * 2.27e-9f + 3.085462e-6f) * 1e-5f;
    return 0;
} /* glatf_00 */

/* ----------------------------------------------------------------------- */
doublereal vtst7_(integer *iyd, real *sec, real *glat, real *glong, real *stl,
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
} /* vtst7_ */

/* ----------------------------------------------------------------------- */
/* Subroutine */ int gts7_(integer *iyd, real *sec, real *alt, real *glat, 
	real *glong, real *stl, real *f107a, real *f107, real *ap, integer *
	mass, real *d__, real *t)
{
    /* Initialized data */

    static integer mt[11] = { 48,0,4,16,28,32,40,1,49,14,17 };
    static real altl[8] = { 200.f,300.f,160.f,250.f,240.f,450.f,320.f,450.f };
    static integer mn1 = 5;
    static real zn1[5] = { 120.f,110.f,100.f,90.f,72.5f };
    static real dgtr = .0174533f;
    static real dr = .0172142f;
    static real alast = -999.f;
    static real alpha[9] = { -.38f,0.f,0.f,0.f,.17f,0.f,-.38f,0.f,0.f };

    /* System generated locals */
    real r__1, r__2;

    /* Builtin functions */
//    double r_mod(real *, real *), sin(doublereal), cos(doublereal), exp(
//	    doublereal), log(doublereal);

    /* Local variables */
    static integer i__, j;
    static real z__, g1, g4, t2, v2, b01, b04, b32, b16, g40, b28, g32, g16, 
	    b40, g14, g28, b14, tz, hc04, hc32, hc16, rc32, rc16, day, zh04, 
	    zhf, zh28, yrd, xmm, xmd, zc04, zh16, zc16, zh32, zc32, zh40, 
	    hc40, zc40, zh01, hc01, zc01, rc01, hc216, zh14, hc14, zc14, rc14,
	     g16h, tho, hcc01, hcc32, hcc14, hcc16, db16h, zcc01, zcc32, 
	    zcc14;
    extern doublereal ccor_00(real *, real *, real *, real *);
    static real zcc16, ddum;
    extern doublereal dnet_00(real *, real *, real *, real *, real *);
    static real zhm01, tinf, zhm04, zhm32, zhm16, zhm40, zhm14, zhm28, zmho, 
	    zsho, hcc232, zsht;
    extern doublereal ccor2_(real *, real *, real *, real *, real *), vtst7_(
	    integer *, real *, real *, real *, real *, real *, real *, real *,
	     integer *), scalh_(real *, real *, real *), densu_00(real *, real *
	    , real *, real *, real *, real *, real *, real *, real *, integer 
	    *, real *, real *, real *), globe7_(real *, real *, real *, real *
	    , real *, real *, real *, real *, real *), glob7s_(real *);


/*     Thermospheric portion of NRLMSISE-00 */
/*     See GTD7 for more extensive comments */

/*        OUTPUT IN M-3 and KG/M3:   CALL METERS(.TRUE.) */

/*     INPUT VARIABLES: */
/*        IYD - YEAR AND DAY AS YYDDD (day of year from 1 to 365 (or 366)) */
/*              (Year ignored in current model) */
/*        SEC - UT(SEC) */
/*        ALT - ALTITUDE(KM) (>72.5 km) */
/*        GLAT - GEODETIC LATITUDE(DEG) */
/*        GLONG - GEODETIC LONGITUDE(DEG) */
/*        STL - LOCAL APPARENT SOLAR TIME(HRS; see Note below) */
/*        F107A - 81 day AVERAGE OF F10.7 FLUX (centered on day DDD) */
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
/*             (7) AVERAGE OF EIGHT 3 HR AP INDICIES FROM 36 TO 57 HRS PRIOR */
/*                    TO CURRENT TIME */
/*        MASS - MASS NUMBER (ONLY DENSITY FOR SELECTED GAS IS */
/*                 CALCULATED.  MASS 0 IS TEMPERATURE.  MASS 48 FOR ALL. */
/*                 MASS 17 IS Anomalous O ONLY.) */

/*     NOTES ON INPUT VARIABLES: */
/*        UT, Local Time, and Longitude are used independently in the */
/*        model and are not of equal importance for every situation. */
/*        For the most physically realistic calculation these three */
/*        variables should be consistent (STL=SEC/3600+GLONG/15). */
/*        The Equation of Time departures from the above formula */
/*        for apparent local time can be included if available but */
/*        are of minor importance. */

/*        F107 and F107A values used to generate the model correspond */
/*        to the 10.7 cm radio flux at the actual distance of the Earth */
/*        from the Sun rather than the radio flux at 1 AU. The following */
/*        site provides both classes of values: */
/*        ftp://ftp.ngdc.noaa.gov/STP/SOLAR_DATA/SOLAR_RADIO/FLUX/ */

/*        F107, F107A, and AP effects are neither large nor well */
/*        established below 80 km and these parameters should be set to */
/*        150., 150., and 4. respectively. */

/*     OUTPUT VARIABLES: */
/*        D(1) - HE NUMBER DENSITY(CM-3) */
/*        D(2) - O NUMBER DENSITY(CM-3) */
/*        D(3) - N2 NUMBER DENSITY(CM-3) */
/*        D(4) - O2 NUMBER DENSITY(CM-3) */
/*        D(5) - AR NUMBER DENSITY(CM-3) */
/*        D(6) - TOTAL MASS DENSITY(GM/CM3) [Anomalous O NOT included] */
/*        D(7) - H NUMBER DENSITY(CM-3) */
/*        D(8) - N NUMBER DENSITY(CM-3) */
/*        D(9) - Anomalous oxygen NUMBER DENSITY(CM-3) */
/*        T(1) - EXOSPHERIC TEMPERATURE */
/*        T(2) - TEMPERATURE AT ALT */

    /* Parameter adjustments */
    --t;
    --d__;
    --ap;

    /* Function Body */
/*        Test for changed input */
    v2 = vtst7_(iyd, sec, glat, glong, stl, f107a, f107, &ap[1], &c__2);

    yrd = (real) (*iyd);
    gts3c_1.za = parm7_1.pdl[40];
    zn1[0] = gts3c_1.za;
    for (j = 1; j <= 9; ++j) {
	d__[j] = 0.f;
/* L2: */
    }
/*        TINF VARIATIONS NOT IMPORTANT BELOW ZA OR ZN1(1) */
    if (*alt > zn1[0]) {
	if (v2 == 1.f || alast <= zn1[0]) {
	    tinf = lower7_1.ptm[0] * parm7_1.pt[0] * (csw_1.sw[15] * globe7_(&
		    yrd, sec, glat, glong, stl, f107a, f107, &ap[1], 
		    parm7_1.pt) + 1.f);
	}
    } else {
	tinf = lower7_1.ptm[0] * parm7_1.pt[0];
    }
    t[1] = tinf;
/*          GRADIENT VARIATIONS NOT IMPORTANT BELOW ZN1(5) */
    if (*alt > zn1[4]) {
	if (v2 == 1.f || alast <= zn1[4]) {
	    gts3c_1.g0 = lower7_1.ptm[3] * parm7_1.ps[0] * (csw_1.sw[18] * 
		    globe7_(&yrd, sec, glat, glong, stl, f107a, f107, &ap[1], 
		    parm7_1.ps) + 1.f);
	}
    } else {
	gts3c_1.g0 = lower7_1.ptm[3] * parm7_1.ps[0];
    }
/*      Calculate these temperatures only if input changed */
    if (v2 == 1.f || *alt < 300.f) {
	gts3c_1.tlb = lower7_1.ptm[1] * (csw_1.sw[16] * globe7_(&yrd, sec, 
		glat, glong, stl, f107a, f107, &ap[1], &parm7_1.pd[450]) + 
		1.f) * parm7_1.pd[450];
    }
    gts3c_1.s = gts3c_1.g0 / (tinf - gts3c_1.tlb);
/*       Lower thermosphere temp variations not significant for */
/*        density above 300 km */
    if (*alt < 300.f) {
	if (v2 == 1.f || alast >= 300.f) {
	    meso7_1.tn1[1] = lower7_1.ptm[6] * parm7_1.ptl[0] / (1.f - 
		    csw_1.sw[17] * glob7s_(parm7_1.ptl));
	    meso7_1.tn1[2] = lower7_1.ptm[2] * parm7_1.ptl[100] / (1.f - 
		    csw_1.sw[17] * glob7s_(&parm7_1.ptl[100]));
	    meso7_1.tn1[3] = lower7_1.ptm[7] * parm7_1.ptl[200] / (1.f - 
		    csw_1.sw[17] * glob7s_(&parm7_1.ptl[200]));
	    meso7_1.tn1[4] = lower7_1.ptm[4] * parm7_1.ptl[300] / (1.f - 
		    csw_1.sw[17] * csw_1.sw[19] * glob7s_(&parm7_1.ptl[300]));
/* Computing 2nd power */
	    r__1 = lower7_1.ptm[4] * parm7_1.ptl[300];
	    meso7_1.tgn1[1] = lower7_1.ptm[8] * parm7_1.pma[800] * (csw_1.sw[
		    17] * csw_1.sw[19] * glob7s_(&parm7_1.pma[800]) + 1.f) * 
		    meso7_1.tn1[4] * meso7_1.tn1[4] / (r__1 * r__1);
	}
    } else {
	meso7_1.tn1[1] = lower7_1.ptm[6] * parm7_1.ptl[0];
	meso7_1.tn1[2] = lower7_1.ptm[2] * parm7_1.ptl[100];
	meso7_1.tn1[3] = lower7_1.ptm[7] * parm7_1.ptl[200];
	meso7_1.tn1[4] = lower7_1.ptm[4] * parm7_1.ptl[300];
/* Computing 2nd power */
	r__1 = lower7_1.ptm[4] * parm7_1.ptl[300];
	meso7_1.tgn1[1] = lower7_1.ptm[8] * parm7_1.pma[800] * meso7_1.tn1[4] 
		* meso7_1.tn1[4] / (r__1 * r__1);
    }

    gts3c_1.z0 = zn1[3];
    gts3c_1.t0 = meso7_1.tn1[3];
    gts3c_1.tr12 = 1.f;

    if (*mass == 0) {
	goto L50;
    }
/*       N2 variation factor at Zlb */
    g28 = csw_1.sw[20] * globe7_(&yrd, sec, glat, glong, stl, f107a, f107, &
	    ap[1], &parm7_1.pd[300]);
    day = mod(yrd, c_b33);
/*        VARIATION OF TURBOPAUSE HEIGHT */
    zhf = parm7_1.pdl[49] * (csw_1.sw[4] * parm7_1.pdl[24] * sin(dgtr * *glat)
	     * cos(dr * (day - parm7_1.pt[13])) + 1.f);
    yrd = (real) (*iyd);
    t[1] = tinf;
    xmm = lower7_1.pdm[24];
    z__ = *alt;

    for (j = 1; j <= 11; ++j) {
	if (*mass == mt[j - 1]) {
	    goto L15;
	}
/* L10: */
    }
/* XX      WRITE(6,100) MASS */
    goto L90;
L15:
    if (z__ > altl[5] && *mass != 28 && *mass != 48) {
	goto L17;
    }

/*       **** N2 DENSITY **** */

/*      Diffusive density at Zlb */
    gts3c_1.db28 = lower7_1.pdm[20] * exp(g28) * parm7_1.pd[300];
/*      Diffusive density at Alt */
    d__[3] = densu_00(&z__, &gts3c_1.db28, &tinf, &gts3c_1.tlb, &c_b38, &alpha[
	    2], &t[2], &lower7_1.ptm[5], &gts3c_1.s, &mn1, zn1, meso7_1.tn1, 
	    meso7_1.tgn1);
    gts3c_1.dd = d__[3];
/*      Turbopause */
    zh28 = lower7_1.pdm[22] * zhf;
    zhm28 = lower7_1.pdm[23] * parm7_1.pdl[30];
    xmd = 28.f - xmm;
/*      Mixed density at Zlb */
    r__1 = alpha[2] - 1.f;
    b28 = densu_00(&zh28, &gts3c_1.db28, &tinf, &gts3c_1.tlb, &xmd, &r__1, &tz, 
	    &lower7_1.ptm[5], &gts3c_1.s, &mn1, zn1, meso7_1.tn1, 
	    meso7_1.tgn1);
    if (z__ > altl[2] || csw_1.sw[14] == 0.f) {
	goto L17;
    }
/*      Mixed density at Alt */
    dmix_1.dm28 = densu_00(&z__, &b28, &tinf, &gts3c_1.tlb, &xmm, &alpha[2], &
	    tz, &lower7_1.ptm[5], &gts3c_1.s, &mn1, zn1, meso7_1.tn1, 
	    meso7_1.tgn1);
/*      Net density at Alt */
    d__[3] = dnet_00(&d__[3], &dmix_1.dm28, &zhm28, &xmm, &c_b38);
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
	case 11:  goto L46;
    }
L20:

/*       **** HE DENSITY **** */

/*       Density variation factor at Zlb */
    g4 = csw_1.sw[20] * globe7_(&yrd, sec, glat, glong, stl, f107a, f107, &ap[
	    1], parm7_1.pd);
/*      Diffusive density at Zlb */
    gts3c_1.db04 = lower7_1.pdm[0] * exp(g4) * parm7_1.pd[0];
/*      Diffusive density at Alt */
    d__[1] = densu_00(&z__, &gts3c_1.db04, &tinf, &gts3c_1.tlb, &c_b47, alpha, &
	    t[2], &lower7_1.ptm[5], &gts3c_1.s, &mn1, zn1, meso7_1.tn1, 
	    meso7_1.tgn1);
    gts3c_1.dd = d__[1];
    if (z__ > altl[0] || csw_1.sw[14] == 0.f) {
	goto L24;
    }
/*      Turbopause */
    zh04 = lower7_1.pdm[2];
/*      Mixed density at Zlb */
    r__1 = 4.f - xmm;
    r__2 = alpha[0] - 1.f;
    b04 = densu_00(&zh04, &gts3c_1.db04, &tinf, &gts3c_1.tlb, &r__1, &r__2, &t[
	    2], &lower7_1.ptm[5], &gts3c_1.s, &mn1, zn1, meso7_1.tn1, 
	    meso7_1.tgn1);
/*      Mixed density at Alt */
    dmix_1.dm04 = densu_00(&z__, &b04, &tinf, &gts3c_1.tlb, &xmm, &c_b15, &t[2],
	     &lower7_1.ptm[5], &gts3c_1.s, &mn1, zn1, meso7_1.tn1, 
	    meso7_1.tgn1);
    zhm04 = zhm28;
/*      Net density at Alt */
    d__[1] = dnet_00(&d__[1], &dmix_1.dm04, &zhm04, &xmm, &c_b47);
/*      Correction to specified mixing ratio at ground */
    gts3c_1.rl = log(b28 * lower7_1.pdm[1] / b04);
    zc04 = lower7_1.pdm[4] * parm7_1.pdl[25];
    hc04 = lower7_1.pdm[5] * parm7_1.pdl[26];
/*      Net density corrected at Alt */
    d__[1] *= ccor_00(&z__, &gts3c_1.rl, &hc04, &zc04);
L24:
    if (*mass != 48) {
	goto L90;
    }
L25:

/*      **** O DENSITY **** */

/*       Density variation factor at Zlb */
    g16 = csw_1.sw[20] * globe7_(&yrd, sec, glat, glong, stl, f107a, f107, &
	    ap[1], &parm7_1.pd[150]);
/*      Diffusive density at Zlb */
    gts3c_1.db16 = lower7_1.pdm[10] * exp(g16) * parm7_1.pd[150];
/*       Diffusive density at Alt */
    d__[2] = densu_00(&z__, &gts3c_1.db16, &tinf, &gts3c_1.tlb, &c_b51, &alpha[
	    1], &t[2], &lower7_1.ptm[5], &gts3c_1.s, &mn1, zn1, meso7_1.tn1, 
	    meso7_1.tgn1);
    gts3c_1.dd = d__[2];
    if (z__ > altl[1] || csw_1.sw[14] == 0.f) {
	goto L34;
    }
/*  Corrected from PDM(3,1) to PDM(3,2)  12/2/85 */
/*       Turbopause */
    zh16 = lower7_1.pdm[12];
/*      Mixed density at Zlb */
    r__1 = 16 - xmm;
    r__2 = alpha[1] - 1.f;
    b16 = densu_00(&zh16, &gts3c_1.db16, &tinf, &gts3c_1.tlb, &r__1, &r__2, &t[
	    2], &lower7_1.ptm[5], &gts3c_1.s, &mn1, zn1, meso7_1.tn1, 
	    meso7_1.tgn1);
/*      Mixed density at Alt */
    dmix_1.dm16 = densu_00(&z__, &b16, &tinf, &gts3c_1.tlb, &xmm, &c_b15, &t[2],
	     &lower7_1.ptm[5], &gts3c_1.s, &mn1, zn1, meso7_1.tn1, 
	    meso7_1.tgn1);
    zhm16 = zhm28;
/*      Net density at Alt */
    d__[2] = dnet_00(&d__[2], &dmix_1.dm16, &zhm16, &xmm, &c_b51);
/*   3/16/99 Change form to match O2 departure from diff equil near 150 */
/*   km and add dependence on F10.7 */
/*      RL=ALOG(B28*PDM(2,2)*ABS(PDL(17,2))/B16) */
    gts3c_1.rl = lower7_1.pdm[11] * parm7_1.pdl[41] * (csw_1.sw[0] * 
	    parm7_1.pdl[23] * (*f107a - 150.f) + 1.f);
    hc16 = lower7_1.pdm[15] * parm7_1.pdl[28];
    zc16 = lower7_1.pdm[14] * parm7_1.pdl[27];
    hc216 = lower7_1.pdm[15] * parm7_1.pdl[29];
    d__[2] *= ccor2_(&z__, &gts3c_1.rl, &hc16, &zc16, &hc216);
/*       Chemistry correction */
    hcc16 = lower7_1.pdm[17] * parm7_1.pdl[38];
    zcc16 = lower7_1.pdm[16] * parm7_1.pdl[37];
    rc16 = lower7_1.pdm[13] * parm7_1.pdl[39];
/*      Net density corrected at Alt */
    d__[2] *= ccor_00(&z__, &rc16, &hcc16, &zcc16);
L34:
    if (*mass != 48 && *mass != 49) {
	goto L90;
    }
L35:

/*       **** O2 DENSITY **** */

/*       Density variation factor at Zlb */
    g32 = csw_1.sw[20] * globe7_(&yrd, sec, glat, glong, stl, f107a, f107, &
	    ap[1], &parm7_1.pd[600]);
/*      Diffusive density at Zlb */
    gts3c_1.db32 = lower7_1.pdm[30] * exp(g32) * parm7_1.pd[600];
/*       Diffusive density at Alt */
    d__[4] = densu_00(&z__, &gts3c_1.db32, &tinf, &gts3c_1.tlb, &c_b55, &alpha[
	    3], &t[2], &lower7_1.ptm[5], &gts3c_1.s, &mn1, zn1, meso7_1.tn1, 
	    meso7_1.tgn1);
    if (*mass == 49) {
	gts3c_1.dd += d__[4] * 2.f;
    } else {
	gts3c_1.dd = d__[4];
    }
    if (csw_1.sw[14] == 0.f) {
	goto L39;
    }
    if (z__ > altl[3]) {
	goto L38;
    }
/*       Turbopause */
    zh32 = lower7_1.pdm[32];
/*      Mixed density at Zlb */
    r__1 = 32.f - xmm;
    r__2 = alpha[3] - 1.f;
    b32 = densu_00(&zh32, &gts3c_1.db32, &tinf, &gts3c_1.tlb, &r__1, &r__2, &t[
	    2], &lower7_1.ptm[5], &gts3c_1.s, &mn1, zn1, meso7_1.tn1, 
	    meso7_1.tgn1);
/*      Mixed density at Alt */
    dmix_1.dm32 = densu_00(&z__, &b32, &tinf, &gts3c_1.tlb, &xmm, &c_b15, &t[2],
	     &lower7_1.ptm[5], &gts3c_1.s, &mn1, zn1, meso7_1.tn1, 
	    meso7_1.tgn1);
    zhm32 = zhm28;
/*      Net density at Alt */
    d__[4] = dnet_00(&d__[4], &dmix_1.dm32, &zhm32, &xmm, &c_b55);
/*       Correction to specified mixing ratio at ground */
    gts3c_1.rl = log(b28 * lower7_1.pdm[31] / b32);
    hc32 = lower7_1.pdm[35] * parm7_1.pdl[32];
    zc32 = lower7_1.pdm[34] * parm7_1.pdl[31];
    d__[4] *= ccor_00(&z__, &gts3c_1.rl, &hc32, &zc32);
L38:
/*      Correction for general departure from diffusive equilibrium above Zlb */
    hcc32 = lower7_1.pdm[37] * parm7_1.pdl[47];
    hcc232 = lower7_1.pdm[37] * parm7_1.pdl[22];
    zcc32 = lower7_1.pdm[36] * parm7_1.pdl[46];
    rc32 = lower7_1.pdm[33] * parm7_1.pdl[48] * (csw_1.sw[0] * parm7_1.pdl[23]
	     * (*f107a - 150.f) + 1.f);
/*      Net density corrected at Alt */
    d__[4] *= ccor2_(&z__, &rc32, &hcc32, &zcc32, &hcc232);
L39:
    if (*mass != 48) {
	goto L90;
    }
L40:

/*       **** AR DENSITY **** */

/*       Density variation factor at Zlb */
    g40 = csw_1.sw[20] * globe7_(&yrd, sec, glat, glong, stl, f107a, f107, &
	    ap[1], &parm7_1.pd[750]);
/*      Diffusive density at Zlb */
    gts3c_1.db40 = lower7_1.pdm[40] * exp(g40) * parm7_1.pd[750];
/*       Diffusive density at Alt */
    d__[5] = densu_00(&z__, &gts3c_1.db40, &tinf, &gts3c_1.tlb, &c_b60, &alpha[
	    4], &t[2], &lower7_1.ptm[5], &gts3c_1.s, &mn1, zn1, meso7_1.tn1, 
	    meso7_1.tgn1);
    gts3c_1.dd = d__[5];
    if (z__ > altl[4] || csw_1.sw[14] == 0.f) {
	goto L44;
    }
/*       Turbopause */
    zh40 = lower7_1.pdm[42];
/*      Mixed density at Zlb */
    r__1 = 40.f - xmm;
    r__2 = alpha[4] - 1.f;
    b40 = densu_00(&zh40, &gts3c_1.db40, &tinf, &gts3c_1.tlb, &r__1, &r__2, &t[
	    2], &lower7_1.ptm[5], &gts3c_1.s, &mn1, zn1, meso7_1.tn1, 
	    meso7_1.tgn1);
/*      Mixed density at Alt */
    dmix_1.dm40 = densu_00(&z__, &b40, &tinf, &gts3c_1.tlb, &xmm, &c_b15, &t[2],
	     &lower7_1.ptm[5], &gts3c_1.s, &mn1, zn1, meso7_1.tn1, 
	    meso7_1.tgn1);
    zhm40 = zhm28;
/*      Net density at Alt */
    d__[5] = dnet_00(&d__[5], &dmix_1.dm40, &zhm40, &xmm, &c_b60);
/*       Correction to specified mixing ratio at ground */
    gts3c_1.rl = log(b28 * lower7_1.pdm[41] / b40);
    hc40 = lower7_1.pdm[45] * parm7_1.pdl[34];
    zc40 = lower7_1.pdm[44] * parm7_1.pdl[33];
/*      Net density corrected at Alt */
    d__[5] *= ccor_00(&z__, &gts3c_1.rl, &hc40, &zc40);
L44:
    if (*mass != 48) {
	goto L90;
    }
L45:

/*        **** HYDROGEN DENSITY **** */

/*       Density variation factor at Zlb */
    g1 = csw_1.sw[20] * globe7_(&yrd, sec, glat, glong, stl, f107a, f107, &ap[
	    1], &parm7_1.pd[900]);
/*      Diffusive density at Zlb */
    gts3c_1.db01 = lower7_1.pdm[50] * exp(g1) * parm7_1.pd[900];
/*       Diffusive density at Alt */
    d__[7] = densu_00(&z__, &gts3c_1.db01, &tinf, &gts3c_1.tlb, &c_b14, &alpha[
	    6], &t[2], &lower7_1.ptm[5], &gts3c_1.s, &mn1, zn1, meso7_1.tn1, 
	    meso7_1.tgn1);
    gts3c_1.dd = d__[7];
    if (z__ > altl[6] || csw_1.sw[14] == 0.f) {
	goto L47;
    }
/*       Turbopause */
    zh01 = lower7_1.pdm[52];
/*      Mixed density at Zlb */
    r__1 = 1.f - xmm;
    r__2 = alpha[6] - 1.f;
    b01 = densu_00(&zh01, &gts3c_1.db01, &tinf, &gts3c_1.tlb, &r__1, &r__2, &t[
	    2], &lower7_1.ptm[5], &gts3c_1.s, &mn1, zn1, meso7_1.tn1, 
	    meso7_1.tgn1);
/*      Mixed density at Alt */
    dmix_1.dm01 = densu_00(&z__, &b01, &tinf, &gts3c_1.tlb, &xmm, &c_b15, &t[2],
	     &lower7_1.ptm[5], &gts3c_1.s, &mn1, zn1, meso7_1.tn1, 
	    meso7_1.tgn1);
    zhm01 = zhm28;
/*      Net density at Alt */
    d__[7] = dnet_00(&d__[7], &dmix_1.dm01, &zhm01, &xmm, &c_b14);
/*       Correction to specified mixing ratio at ground */
    gts3c_1.rl = log(b28 * lower7_1.pdm[51] * dabs(parm7_1.pdl[42]) / b01);
    hc01 = lower7_1.pdm[55] * parm7_1.pdl[36];
    zc01 = lower7_1.pdm[54] * parm7_1.pdl[35];
    d__[7] *= ccor_00(&z__, &gts3c_1.rl, &hc01, &zc01);
/*       Chemistry correction */
    hcc01 = lower7_1.pdm[57] * parm7_1.pdl[44];
    zcc01 = lower7_1.pdm[56] * parm7_1.pdl[43];
    rc01 = lower7_1.pdm[53] * parm7_1.pdl[45];
/*      Net density corrected at Alt */
    d__[7] *= ccor_00(&z__, &rc01, &hcc01, &zcc01);
L47:
    if (*mass != 48) {
	goto L90;
    }
L48:

/*        **** ATOMIC NITROGEN DENSITY **** */

/*       Density variation factor at Zlb */
    g14 = csw_1.sw[20] * globe7_(&yrd, sec, glat, glong, stl, f107a, f107, &
	    ap[1], &parm7_1.pd[1050]);
/*      Diffusive density at Zlb */
    gts3c_1.db14 = lower7_1.pdm[60] * exp(g14) * parm7_1.pd[1050];
/*       Diffusive density at Alt */
    d__[8] = densu_00(&z__, &gts3c_1.db14, &tinf, &gts3c_1.tlb, &c_b68, &alpha[
	    7], &t[2], &lower7_1.ptm[5], &gts3c_1.s, &mn1, zn1, meso7_1.tn1, 
	    meso7_1.tgn1);
    gts3c_1.dd = d__[8];
    if (z__ > altl[7] || csw_1.sw[14] == 0.f) {
	goto L49;
    }
/*       Turbopause */
    zh14 = lower7_1.pdm[62];
/*      Mixed density at Zlb */
    r__1 = 14.f - xmm;
    r__2 = alpha[7] - 1.f;
    b14 = densu_00(&zh14, &gts3c_1.db14, &tinf, &gts3c_1.tlb, &r__1, &r__2, &t[
	    2], &lower7_1.ptm[5], &gts3c_1.s, &mn1, zn1, meso7_1.tn1, 
	    meso7_1.tgn1);
/*      Mixed density at Alt */
    dmix_1.dm14 = densu_00(&z__, &b14, &tinf, &gts3c_1.tlb, &xmm, &c_b15, &t[2],
	     &lower7_1.ptm[5], &gts3c_1.s, &mn1, zn1, meso7_1.tn1, 
	    meso7_1.tgn1);
    zhm14 = zhm28;
/*      Net density at Alt */
    d__[8] = dnet_00(&d__[8], &dmix_1.dm14, &zhm14, &xmm, &c_b68);
/*       Correction to specified mixing ratio at ground */
    gts3c_1.rl = log(b28 * lower7_1.pdm[61] * dabs(parm7_1.pdl[2]) / b14);
    hc14 = lower7_1.pdm[65] * parm7_1.pdl[1];
    zc14 = lower7_1.pdm[64] * parm7_1.pdl[0];
    d__[8] *= ccor_00(&z__, &gts3c_1.rl, &hc14, &zc14);
/*       Chemistry correction */
    hcc14 = lower7_1.pdm[67] * parm7_1.pdl[4];
    zcc14 = lower7_1.pdm[66] * parm7_1.pdl[3];
    rc14 = lower7_1.pdm[63] * parm7_1.pdl[5];
/*      Net density corrected at Alt */
    d__[8] *= ccor_00(&z__, &rc14, &hcc14, &zcc14);
L49:
    if (*mass != 48) {
	goto L90;
    }
L46:

/*        **** Anomalous OXYGEN DENSITY **** */

    g16h = csw_1.sw[20] * globe7_(&yrd, sec, glat, glong, stl, f107a, f107, &
	    ap[1], &parm7_1.pd[1200]);
    db16h = lower7_1.pdm[70] * exp(g16h) * parm7_1.pd[1200];
    tho = lower7_1.pdm[79] * parm7_1.pdl[6];
    gts3c_1.dd = densu_00(&z__, &db16h, &tho, &tho, &c_b51, &alpha[8], &t2, &
	    lower7_1.ptm[5], &gts3c_1.s, &mn1, zn1, meso7_1.tn1, meso7_1.tgn1)
	    ;
    zsht = lower7_1.pdm[75];
    zmho = lower7_1.pdm[74];
    zsho = scalh_(&zmho, &c_b51, &tho);
    d__[9] = gts3c_1.dd * exp(-zsht / zsho * (exp(-(z__ - zmho) / zsht) - 1.f)
	    );
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
    z__ = dabs(*alt);
    ddum = densu_00(&z__, &c_b14, &tinf, &gts3c_1.tlb, &c_b15, &c_b15, &t[2], &
	    lower7_1.ptm[5], &gts3c_1.s, &mn1, zn1, meso7_1.tn1, meso7_1.tgn1)
	    ;
L90:
/*       ADJUST DENSITIES FROM CGS TO KGM */
    if (metsel_1.imr == 1) {
	for (i__ = 1; i__ <= 9; ++i__) {
	    d__[i__] *= 1e6f;
/* L95: */
	}
	d__[6] /= 1e3f;
    }
    alast = *alt;
    return 0;
/* XX  100 FORMAT(1X,'MASS', I5, '  NOT VALID') */
} /* gts7_ */

/* ----------------------------------------------------------------------- */
/* Subroutine */ int meters_00(logical *meter)
{
/*      Convert outputs to Kg & Meters if METER true */
    metsel_1.imr = 0;
    if (*meter) {
	metsel_1.imr = 1;
    }
    return 0;
} /* meters_00 */

/* ----------------------------------------------------------------------- */
doublereal scalh_(real *alt, real *xm, real *temp)
{
    /* Initialized data */

    static real rgas = 831.4f;

    /* System generated locals */
    real ret_val, r__1;

    /* Local variables */
    static real g;

/*      Calculate scale height (km) */
/* Computing 2nd power */
    r__1 = 1.f + *alt / parmb_1.re;
    g = parmb_1.gsurf / (r__1 * r__1);
    ret_val = rgas * *temp / (g * *xm);
    return ret_val;
} /* scalh_ */

/* ----------------------------------------------------------------------- */
doublereal globe7_(real *yrd, real *sec, real *lat, real *long__, real *tloc, 
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
	    cd32, cd18, cd39, exp1;
    /* Subroutine */ int tselec_00(real *);

/*       CALCULATE G(L) FUNCTION */
/*       Upper Thermosphere Parameters */
    /* Parameter adjustments */
    --p;
    --ap;

    /* Function Body */
/*       3hr Magnetic activity functions */
/*      Eq. A24d */
/*       Eq. A24c */
/*       Eq. A24a */
    if (csw_1.isw != 64999) {
	tselec_00(sv);
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
    ttest_2.t[0] = p[20] * lpoly_1.df * (p[60] * lpoly_1.dfa + 1.f) + p[21] * 
	    lpoly_1.df * lpoly_1.df + p[22] * lpoly_1.dfa + p[30] * (r__1 * 
	    r__1);
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
	    1.f - r__6 * (r__5 * r__5)) / (1.f - exp1) * pow(d__1, c_b89)
	    );
/*      APT(2)=SG2(EXP1) */
/*      APT(3)=SG0(EXP2) */
/*      APT(4)=SG2(EXP2) */
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
	    111] * lpoly_1.plg[12] + p[112] * lpoly_1.plg[14]) * cd14) * cos(
	    dgtr * *long__) + (p[91] * lpoly_1.plg[11] + p[92] * lpoly_1.plg[
	    13] + p[93] * lpoly_1.plg[15] + p[107] * lpoly_1.plg[10] + p[108] 
	    * lpoly_1.plg[12] + p[109] * lpoly_1.plg[14] + csw_1.swc[4] * (p[
	    113] * lpoly_1.plg[10] + p[114] * lpoly_1.plg[12] + p[115] * 
	    lpoly_1.plg[14]) * cd14) * sin(dgtr * *long__));
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
/*  PARMS NOT USED: 83, 90,100,140-150 */
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
} /* globe7_ */

/* ----------------------------------------------------------------------- */
/* Subroutine */ int tselec_0_00(int n__, real *sv, real *svv)
{
    /* System generated locals */
    real r__1, r__2;

    /* Builtin functions */
//    double r_mod(real *, real *);

    /* Local variables */
    static integer i__;
    static real sav[25];

/*        SET SWITCHES */
/*        Output in  COMMON/CSW/SW(25),ISW,SWC(25) */
/*        SW FOR MAIN TERMS, SWC FOR CROSS TERMS */

/*        TO TURN ON AND OFF PARTICULAR VARIATIONS CALL TSELEC(SV), */
/*        WHERE SV IS A 25 ELEMENT ARRAY CONTAINING 0. FOR OFF, 1. */
/*        FOR ON, OR 2. FOR MAIN EFFECTS OFF BUT CROSS TERMS ON */

/*        To get current values of SW: CALL TRETRV(SW) */

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
	csw_1.sw[i__ - 1] = mod(sv[i__], c_b98);
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

/* Subroutine */ int tselec_00(real *sv)
{
    return tselec_0_00(0, sv, (real *)0);
    }

/* Subroutine */ int tretrv_00(real *svv)
{
    return tselec_0_00(1, (real *)0, svv);
    }

/* ----------------------------------------------------------------------- */
doublereal glob7s_(real *p)
{
    /* Initialized data */

    static real dr = .0172142f;
    static real dgtr = .0174533f;
    static real pset = 2.f;
    static real dayl = -1.f;
    static real p32 = -1e3f;
    static real p18 = -1e3f;
    static real p14 = -1e3f;
    static real p39 = -1e3f;

    /* System generated locals */
    real ret_val, r__1;

    /* Builtin functions */
//    /* Subroutine */ int s_stop(char *, ftnlen);
//    double cos(doublereal), sin(doublereal);

    /* Local variables */
    static integer i__, j;
    static real t[14], t71, t72, t81, t82, tt, cd32, cd14, cd18, cd39;

/*      VERSION OF GLOBE FOR LOWER ATMOSPHERE 10/26/99 */
    /* Parameter adjustments */
    --p;

    /* Function Body */
/*       CONFIRM PARAMETER SET */
    if (p[100] == 0.f) {
	p[100] = pset;
    }
    if (p[100] != pset) {
/* XX        WRITE(6,900) PSET,P(100) */
/* XX  900   FORMAT(1X,'WRONG PARAMETER SET FOR GLOB7S',3F10.1) */
   return 0;
//	s_stop("", (ftnlen)0);
    }
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
	    lpoly_2.plg[6] + p[27] * lpoly_2.plg[1] + p[15] * lpoly_2.plg[3] 
	    + p[60] * lpoly_2.plg[5];
/*       SYMMETRICAL ANNUAL */
    t[2] = (p[19] + p[48] * lpoly_2.plg[2] + p[30] * lpoly_2.plg[4]) * cd32;
/*       SYMMETRICAL SEMIANNUAL */
    t[3] = (p[16] + p[17] * lpoly_2.plg[2] + p[31] * lpoly_2.plg[4]) * cd18;
/*       ASYMMETRICAL ANNUAL */
    t[4] = (p[10] * lpoly_2.plg[1] + p[11] * lpoly_2.plg[3] + p[21] * 
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
    t81 = (p[24] * lpoly_2.plg[21] + p[36] * lpoly_2.plg[23]) * cd14 * 
	    csw_1.swc[4];
    t82 = (p[34] * lpoly_2.plg[21] + p[37] * lpoly_2.plg[23]) * cd14 * 
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
	t[8] = p[51] * lpoly_2.apt[0] + p[97] * lpoly_2.plg[2] * lpoly_2.apt[
		0] * csw_1.swc[1];
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
	    12] + p[77] * lpoly_2.plg[14]) * cos(dgtr * lpoly_2.long__) + (p[
	    91] * lpoly_2.plg[11] + p[92] * lpoly_2.plg[13] + p[93] * 
	    lpoly_2.plg[15] + p[78] * lpoly_2.plg[10] + p[79] * lpoly_2.plg[
	    12] + p[80] * lpoly_2.plg[14]) * sin(dgtr * lpoly_2.long__));
L49:
    tt = 0.f;
    for (i__ = 1; i__ <= 14; ++i__) {
/* L50: */
	tt += (r__1 = csw_1.sw[i__ - 1], dabs(r__1)) * t[i__ - 1];
    }
    ret_val = tt;
    return ret_val;
} /* glob7s_ */

/* -------------------------------------------------------------------- */
doublereal densu_00(real *alt, real *dlb, real *tinf, real *tlb, real *xm, real 
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
    extern /* Subroutine */ int spline_00(real *, real *, integer *, real *, 
	    real *, real *), splini_00(real *, real *, real *, integer *, real *
	    , real *), splint_00(real *, real *, real *, integer *, real *, 
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
    spline_00(xs, ys, &mn, &yd1, &yd2, y2out);
    x = zg / zgdif;
    splint_00(xs, ys, y2out, &mn, &x, &y);
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
    splini_00(xs, ys, y2out, &mn, &x, &yi);
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
} /* densu_00 */

/* -------------------------------------------------------------------- */
doublereal densm_00(real *alt, real *d0, real *xm, real *tz, integer *mn3, real 
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
    extern /* Subroutine */ int spline_00(real *, real *, integer *, real *, 
	    real *, real *), splini_00(real *, real *, real *, integer *, real *
	    , real *), splint_00(real *, real *, real *, integer *, real *, 
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
    spline_00(xs, ys, &mn, &yd1, &yd2, y2out);
    x = zg / zgdif;
    splint_00(xs, ys, y2out, &mn, &x, &y);
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
    splini_00(xs, ys, y2out, &mn, &x, &yi);
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
    spline_00(xs, ys, &mn, &yd1, &yd2, y2out);
    x = zg / zgdif;
    splint_00(xs, ys, y2out, &mn, &x, &y);
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
    splini_00(xs, ys, y2out, &mn, &x, &yi);
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
} /* densm_00 */

/* ----------------------------------------------------------------------- */
/* Subroutine */ int spline_00(real *x, real *y, integer *n, real *yp1, real *
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
} /* spline_00 */

/* ----------------------------------------------------------------------- */
/* Subroutine */ int splint_00(real *xa, real *ya, real *y2a, integer *n, real *
	x, real *y)
{
    static real a, b, h__;
    static integer k, khi, klo;

/*        CALCULATE CUBIC SPLINE INTERP VALUE */
/*        ADAPTED FROM NUMERICAL RECIPES BY PRESS ET AL. */
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
} /* splint_00 */

/* ----------------------------------------------------------------------- */
/* Subroutine */ int splini_00(real *xa, real *ya, real *y2a, integer *n, real *
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
} /* splini_00 */

/* ----------------------------------------------------------------------- */
doublereal dnet_00(real *dd, real *dm, real *zhm, real *xmm, real *xm)
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
} /* dnet_00 */

/* ----------------------------------------------------------------------- */
doublereal ccor_00(real *alt, real *r__, real *h1, real *zh)
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
} /* ccor_00 */

/* ----------------------------------------------------------------------- */
doublereal ccor2_(real *alt, real *r__, real *h1, real *zh, real *h2)
{
    /* System generated locals */
    real ret_val;

    /* Builtin functions */
//    double exp(doublereal);

    /* Local variables */
    static real e1, e2, ex1, ex2;

/*       O&O2 CHEMISTRY/DISSOCIATION CORRECTION FOR MSIS MODELS */
    e1 = (*alt - *zh) / *h1;
    e2 = (*alt - *zh) / *h2;
    if (e1 > 70.f || e2 > 70.f) {
	goto L20;
    }
    if (e1 < -70.f && e2 < -70.f) {
	goto L10;
    }
    ex1 = exp(e1);
    ex2 = exp(e2);
    ret_val = *r__ / ((ex1 + ex2) * .5f + 1.f);
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
} /* ccor2_ */

/* ----------------------------------------------------------------------- */
/* Subroutine */ int gtd7bk_(void)
{
    return 0;
} /* gtd7bk_ */

/*          MSISE-00 01-FEB-02 */
/* XX      COMMON/DATIM7/ISDATE(3),ISTIME(2),NAME(2) */
/* XX      DATA ISDATE/'01-F','EB-0','2   '/,ISTIME/'15:4','9:27'/ */
/* XX      DATA NAME/'MSIS','E-00'/ */
/*         TEMPERATURE */
/*         HE DENSITY */
/*         O DENSITY */
/*         N2 DENSITY */
/*         TLB */
/*         O2 DENSITY */
/*         AR DENSITY */
/*          H DENSITY */
/*          N DENSITY */
/*        HOT O DENSITY */
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
/*          SEMIANNUAL MULT SAM */
/*         MIDDLE ATMOSPHERE AVERAGES */


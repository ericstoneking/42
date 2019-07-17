/**********************************************************************/
/*    This file is distributed with 42,                               */
/*    the (mostly harmless) spacecraft dynamics simulation            */
/*    created by Eric Stoneking of NASA Goddard Space Flight Center   */

/*    Copyright 2010 United States Government                         */
/*    as represented by the Administrator                             */
/*    of the National Aeronautics and Space Administration.           */

/*    No copyright is claimed in the United States                    */
/*    under Title 17, U.S. Code.                                      */

/*    All Other Rights Reserved.                                      */


/* This file is modified from Dan Leonard's code described in the comments 
** below.  It was modified by Eric Stoneking to be called from other code, and to
** read in the data files concurrently (and some formatting).
**
** Dan Leonard's code and the coefficients were obtained from
** https://ccmc.gsfc.nasa.gov/pub/modelweb/radiation_belt/radbelt/c_code/
** and
** https://ccmc.gsfc.nasa.gov/pub/modelweb/radiation_belt/radbelt/fortran_code/
** 
** There is a report at:
** https://ccmc.gsfc.nasa.gov/modelweb/magnetos/AP-8-min-max-76-6.pdf
** 
** This URL seems more recent, and says that the old AP and AE models are still 
** the de facto standard for engineering because of coverage: 
** https://www.spenvis.oma.be/help/background/traprad/traprad.html
*/

/* Begin Dan Leonard's comments: */
/**============================================================================
//|External Files:
//|   Name           Type        Size           Description
//|   AE8MIN.ASCt  ascii data   80K       Coefficients of electron model 
//|                                              (one line shorter than AE8MIN.ASC)
//|   AP8MIN.ASCt  ascii data   80K       Coefficients of proton model
//|                                              (one line shorter than AP8MIN.ASC)
//|Notes:
//|   
//|History:
//|     August 10, 1998 Dan Leonard,
//|        dleonard@cfa.harvard.edu,
//|        (617) 496-7075 
//|        (617) 496-7049 fax
//|
//|        This is a C version of NASA's main radiation modelling program "radbelt.for"
//|        which basically makes calls to the primary algorithim model, "trmfun.for".
//|        Here, the main function is analogous to the code in "radbelt.for", 
//|        in the sense that they are both examples of basic user interfaces.
//|        The functions TRARA1() and TRARA2() are analogous to those found in the
//|        trmfun.for original module.  In porting the code to C, however, I found it
//|        necessary to add the funtions trara3(), trara4(), and finally trara5() which
//|        carry the "GOTO" parts of the original FORTRAN TRARA1() and TRARA2() functions.
//|
//|        Here I only use two of the eight available data input files, AE8MIN.ASCt,
//|        and AP8MIN.ASCt.  The program reads them in assuming ascii not binary
//|        format.  The first line of the files is deleted, thus the extension "t"
//|        for truncated.  The line is instead carried below in the static arrays:
//|
//|        static int descrelns[] = {  8,  4,  1964,  6400,  2100,  1024,  1024, 13168 };
//|        static int descrprtns[] = {  2,     4,  1964,   100,  2048,  2048,  1024, 16584 };
//|  
//|        The program should work equally as well for the MAX models, AP8MAX.ASC or
//|        AE8MAX.ASC, after truncating the first line and using its coeffiecients instead
//|        for the descrprtns[] or descrelns[] arrays instead.
//|
//|        One website at which radbelt.for and trmfun.for can be found, as well as
//|        as the 18K integer data files is at:
//|        http://nssdc.gsfc.nasa.gov/space/model/trap.html.  An older paper report
//|        with plots is the "AP-8 Trapped Proton Environment for Solar
//|        Maximum and Solar Minimum".  At least one newer one is known to
//|        exist by J.I. Vette, "The AE-8 Trapped Electron Model Environment, 
//|        NSSDC Report 91-24, Nov 1991".  One for protons is assumed to exist.
//|
//|        The main() program below finds BB0 and L given magnetic latitude and
//|        range of a point in space.  Dipole assumptions are made to do this.
//|        The dipole appoximation to the earth's magnetic field is quite good 
//|        except when very close to the earth or far from it.  TRARA1() and
//|        TRARA2() only need BB0 and L as inputs however -- the reader may wish
//|        to calculate these values in some other way.
//|
//|============================================================================*/
#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <math.h>

#define AE_MIN 0
#define AP_MIN 1
#define AE_MAX 2
#define AP_MAX 3

int *MAP;
float FISTEP;
int I1;
float FKB1, FKB2, FINCR2, FINCR1;
float FKBM, FLOGM, SL2, FNB, DFL;
int J1, J2, ITIME, L1, L2;
int FLOG1, FLOG2;
float FKBJ1, FKBJ2, SL1;
float FKB, FLOG;

int Descriptors[4][8];
int *MapAeMin,*MapApMin,*MapAeMax,*MapApMax;

/******************************************************************************/
void LoadMapsFromFiles(void)
{
      FILE *infile;
      int i;
            
      /* AEMIN */
      infile = fopen("./Model/ae8min.txt","rt");
      fscanf(infile," %d %d %d %d %d %d %d %d",
         &Descriptors[AE_MIN][0],&Descriptors[AE_MIN][1],
         &Descriptors[AE_MIN][2],&Descriptors[AE_MIN][3],
         &Descriptors[AE_MIN][4],&Descriptors[AE_MIN][5],
         &Descriptors[AE_MIN][6],&Descriptors[AE_MIN][7]);
      MapAeMin = (int *) calloc(Descriptors[AE_MIN][7],sizeof(int));
      for(i=0;i<Descriptors[AE_MIN][7];i++) 
         fscanf(infile,"%6d",&MapAeMin[i]);
      fclose(infile);
         
      /* APMIN */
      infile = fopen("./Model/ap8min.txt","rt");
      fscanf(infile," %d %d %d %d %d %d %d %d",
         &Descriptors[AP_MIN][0],&Descriptors[AP_MIN][1],
         &Descriptors[AP_MIN][2],&Descriptors[AP_MIN][3],
         &Descriptors[AP_MIN][4],&Descriptors[AP_MIN][5],
         &Descriptors[AP_MIN][6],&Descriptors[AP_MIN][7]);
      MapApMin = (int *) calloc(Descriptors[AP_MIN][7],sizeof(int));
      for(i=0;i<Descriptors[AP_MIN][7];i++) 
         fscanf(infile,"%6d",&MapApMin[i]);
      fclose(infile);
         
      /* AEMAX */
      infile = fopen("./Model/ae8max.txt","rt");
      fscanf(infile," %d %d %d %d %d %d %d %d",
         &Descriptors[AE_MAX][0],&Descriptors[AE_MAX][1],
         &Descriptors[AE_MAX][2],&Descriptors[AE_MAX][3],
         &Descriptors[AE_MAX][4],&Descriptors[AE_MAX][5],
         &Descriptors[AE_MAX][6],&Descriptors[AE_MAX][7]);
      MapAeMax = (int *) calloc(Descriptors[AE_MAX][7],sizeof(int));
      for(i=0;i<Descriptors[AE_MAX][7];i++) 
         fscanf(infile,"%6d",&MapAeMax[i]);
      fclose(infile);
         
      /* APMAX */
      infile = fopen("./Model/ap8max.txt","rt");
      fscanf(infile," %d %d %d %d %d %d %d %d",
         &Descriptors[AP_MAX][0],&Descriptors[AP_MAX][1],
         &Descriptors[AP_MAX][2],&Descriptors[AP_MAX][3],
         &Descriptors[AP_MAX][4],&Descriptors[AP_MAX][5],
         &Descriptors[AP_MAX][6],&Descriptors[AP_MAX][7]);
      MapApMax = (int *) calloc(Descriptors[AP_MAX][7],sizeof(int));
      for(i=0;i<Descriptors[AP_MAX][7];i++) 
         fscanf(infile,"%6d",&MapApMax[i]);
      fclose(infile);
         
}

/******************************************************************************/
float trara5(void)
{
      float retval;
      
      if ( FKB < (FKBM+1.E-10)) return (0.0);
      
      retval=FLOGM+(FLOG-FLOGM)*((FNB-FKBM)/(FKB-FKBM));
      retval= retval > 0.0 ? retval : 0.0;
      
      return(retval);
}
/******************************************************************************/
float trara4(int *SUBMAP, int start_psn) 
{ 
      int I2 = 0;
      int bypassto20;
      bypassto20=0.0;

      /* reset statics */
      FKB=FLOG=0.0;

      if ( start_psn == 15 ) {
         ;
      } 
      else if ( start_psn == 35 ) {
         /*    35  FINCR1=0.
          SL1=-900000.
          GOTO 20  */
         FINCR1=0;
         SL1=-900000;
         bypassto20=1; /* like going to 20 */
      } 
      else {
         /* 29 FKBM=FKBJ1+(FKB2-FKBJ1)*DFL */
         FKBM=FKBJ1+(FKB2-FKBJ1)*DFL;
         FLOGM=FKBM*SL2;
         FLOG2=FLOG2-FISTEP;
         FKB2=FKB2+FINCR2;
         SL1=FLOG1/FKB1;
         SL2=FLOG2/FKB2;
      }                                       
      while ( 1 ) {
        if ( SL1 >= SL2 && !bypassto20 ) {
            FKBJ2=((FLOG2/FISTEP)*FINCR2+FKB2)/((FINCR2/FISTEP)*SL1+1.);
            FKB=FKB1+(FKBJ2-FKB1)*DFL;
            FLOG=FKB*SL1;
            if(FKB >= FNB) return(trara5());
            FKBM=FKB;
            FLOGM=FLOG;
            if(J1 >= L1) return(0);
            J1=J1+1;
            FINCR1=SUBMAP[I1+J1-1];
            FLOG1=FLOG1-FISTEP;
            FKB1=FKB1+FINCR1;
            SL1=FLOG1/FKB1;
         }
         bypassto20=0; /* only once occurs via entry at 35 */
         FKBJ1=((FLOG1/FISTEP)*FINCR1+FKB1)/((FINCR1/FISTEP)*SL2+1.);
         FKB=FKBJ1+(FKB2-FKBJ1)*DFL;
         FLOG=FKB*SL2;
         if(FKB >= FNB) return(trara5());
         FKBM=FKB;
         FLOGM=FLOG;
         if(J2 >= L2) return(0);
         J2=J2+1;
         FINCR2=SUBMAP[I2+J2-1];
         FLOG2=FLOG2-FISTEP;
         FKB2=FKB2+FINCR2;
         SL2=FLOG2/FKB2;
      }  /* GOTO 15  (or possibly 20??) */
} /* end of TRARA4 */
/*
 *  incorporates old fortran gotos 23, 32, 28, 30
 */
/******************************************************************************/
float trara3(int *SUBMAP, int position) 
{
      int I2 = 0;

      /* reset statics */
      FKBJ1=0;
      FKBJ1=0;
      SL1=0;

      if ( position == 23 && (ITIME != 1) && (J2 != 4) ) {
         SL2=FLOG2/FKB2;
           for ( J1=4; J1<=L1; J1++) { 
              FINCR1=SUBMAP[I1+J1-1];
               FKB1=FKB1+FINCR1;
              FLOG1=FLOG1-FISTEP;
              FKBJ1=((FLOG1/FISTEP)*FINCR1+FKB1)/((FINCR1/FISTEP)*SL2+1.);
              /* IF(FKBJ1.LE.FKB1) GOTO 31 */
              if (FKBJ1 <= FKB1) return(trara4(SUBMAP, 0)); /*go to 31 with sure true;
                                              same as go to 29 or trara4(0) */
           }  
         if (FKBJ1 <= FKB2) return(trara5());
         if ( FKBJ1 <= FKB2) return(trara4(SUBMAP, 0)); /* start at the top of trara4() */
         FKB1=0.;
      }
      FKB2=0.; 
      if ( position == 32 ) {
         J2=4; 
         FINCR2=SUBMAP[I2+J2-1];
         FLOG2=SUBMAP[I2+3-1];
         FLOG1=SUBMAP[I1+3-1];
      } 
      FLOGM=FLOG1+(FLOG2-FLOG1)*DFL;
      FKBM=0.;
      FKB2=FKB2+FINCR2; 
      FLOG2=FLOG2-FISTEP; 
      SL2=FLOG2/FKB2;
      if ( L1 < 4 ) return(trara4(SUBMAP, 35));
      J1=4;
      FINCR1=SUBMAP[I1+J1 +1];                                                
      FKB1=FKB1+FINCR1;
      FLOG1=FLOG1-FISTEP;
      SL1=FLOG1/FKB1;
      return(trara4(SUBMAP,15));
}  /* end of function trara3() */


/*****************************************************************
 ***  TRARA2 INTERPOLATES LINEARLY IN L-B/B0-MAP TO OBTAIN     ***
 ***  THE LOGARITHM OF INTEGRAL FLUX AT GIVEN L AND B/B0.      ***
 ***    INPUT: MAP[] IS SUB-MAP (FOR SPECIFIC ENERGY) OF     ***
 ***                   TRAPPED RADIATION MODEL MAP             ***
 ***           IL      SCALED L-VALUE                          ***
 ***           IB      SCALED B/B0-1                           ***
 ***   OUTPUT: TRARA2  SCALED LOGARITHM OF PARTICLE FLUX       ***
 *****************************************************************
 ***  SEE MAIN PROGRAM 'MODEL' FOR EXPLANATION OF MAP FORMAT   ***
 ***  SCALING FACTORS.                                         ***
 ***  THE STEPSIZE FOR THE PARAMETERIZATION OF THE LOGARITHM   ***
 ***  OF FLUX IS OBTAINED FROM 'COMMON/TRA2/'.                 ***
 *****************************************************************/
 /*     FUNCTION TRARA2(int MAP,IL,IB) */


/******************************************************************************/
float TRARA2(int *SUBMAP,float IL,float IB)  
{                                   
      float FNL, FLL1, FLL2;
      int I2, KT;
      /* TRARA2 may becalled multiple times --want to reset these statics */
      I2=0;
      FLOG1=0;
      FLOG2=0;
      J1=0;
      J2=0;
      ITIME=0;
      L1=0;
      L2=0;
      FKB1=0;
      FKB2=0;
      FINCR2=0;
      FINCR1=0;
      FKBM=0;
      FLOGM=0;
      SL2=0;
      FNB=0;
      DFL=0;

      FNB=IB;
      FNL=IL; 
      /*
       * FIND CONSECUTIVE SUB-SUB-MAPS FOR SCALED L-VALUES LS1,LS2, 
       * WITH IL LESS OR EQUAL LS2.  L1,L2 ARE LENGTHS OF SUB-SUB-MAPS. 
       * I1,I2 ARE INDECES OF FIRST ELEMENTS MINUS 1.
       */
      L2 = SUBMAP[I2+1-1];
      while ( SUBMAP[I2+2-1] <= IL ) {
         I1=I2;                                                            
         L1=L2;
         I2=I2+L2;
         L2 = SUBMAP[I2+1-1];
      } 

      /*  
      * IF SUB-SUB-MAPS ARE EMPTY, I. E. LENGTH LESS 4, THAN TRARA2=0
      */       
      if (( L1 < 4 ) && (L2 < 4 )) return(trara5());
      /*
      * IF FLOG2 LESS FLOG1, THAN LS2 FIRST MAP AND LS1 SECOND MAP
      */
      if ( SUBMAP[I2+3-1] <= SUBMAP[I1+3-1] ) {
         KT=I1; /* 5 KT=I1   */
         I1=I2;
         I2=KT;
         KT=L1;
         L1=L2;
         L2=KT;
      }
      do {   /* major 5 loop of type while is here */
      /*
      * DETERMINE INTERPOLATE IN SCALED L-VALUE
      */ 
         FLL1=SUBMAP[I1+2-1];
         FLL2=SUBMAP[I2+2-1];
         DFL=(FNL-FLL1)/(FLL2-FLL1);
         FLOG1=SUBMAP[I1+3-1];
         FLOG2=SUBMAP[I2+3-1];
         FKB1=0.;
         FKB2=0.;
         if ( L1 < 4 ) return(trara3(SUBMAP, 32));
         /*
          * B/B0 LOOP
          */                                               
         for(J2=4; J2<=L2; J2++) {
            FINCR2=SUBMAP[I2+J2-1];
            /* IF(FKB2+FINCR2.GT.FNB) GOTO 23 */
            if ((FKB2+FINCR2) > FNB) return(trara3(SUBMAP, 23));
            FKB2=FKB2+FINCR2;
            /* 17 FLOG2=FLOG2-FISTEP */
            FLOG2=FLOG2-FISTEP;
         }  /* closing 17 here */
         ITIME=ITIME+1;
         /* IF(ITIME.EQ.1)GO TO 5  */
         KT=I1;
         I1=I2;
         I2=KT;
         KT=L1;
         L1=L2;
         L2=KT;
      } while ( ITIME == 1 );  /* this is the 5 continue */
      return(0); /*GO TO 50, equiv to TRARA2=0 */
} /* end of trara2() here */

/* 
  based upon TRMFUN.FOR 1987
******************* TRARA1, TRARA2 *********************************
********************************************************************                         
***********************************************************************
*** TRARA1 FINDS PARTICLE FLUXES FOR GIVEN ENERGIES, MAGNETIC FIELD *** 
*** STRENGTH AND L-VALUE. FUNCTION TRARA2 IS USED TO INTERPOLATE IN ***
*** B-L-SPACE.                                                      ***
***   INPUT: DESCR(8)   HEADER OF SPECIFIED TRAPPED RADITION MODEL  ***
***          MAP(...)   MAP OF TRAPPED RADITION MODEL               ***
***                     (DESCR AND MAP ARE EXPLAINED AT THE BEGIN   ***
***                     OF THE MAIN PROGRAM MODEL)                  ***
 (these are currectly in the big static array MAP and DESCR)
***          N          NUMBER OF ENERGIES                          ***
***          E(N)       ARRAY OF ENERGIES IN MEV                    ***
***          FL         L-VALUE                                     ***
***          BB0        =B/B0  MAGNETIC FIELD STRENGTH NORMALIZED   ***
***                     TO FIELD STRENGTH AT MAGNETIC EQUATOR       ***
***  OUTPUT: F(N)       DECADIC LOGARITHM OF INTEGRAL FLUXES IN     ***
***                     PARTICLES/(CM*CM*SEC)                       ***
***********************************************************************
*/

/******************************************************************************/
void TRARA1(float FL, float BB0, float *E,float *F,int N, int *DESCR)
{
      int S0, S1, S2;   
      int I2, I3, L3, IE, I0;
      float  ESCALE, FSCALE;
      float XNL, NL, NB, E0, E1, E2, F0, F1, F2;
      F1=1.001; 
      F2=1.002;  

      FISTEP=DESCR[6]/DESCR[1];
      ESCALE=DESCR[3];
      FSCALE=DESCR[6];
      FL = FL > 0 ? FL : -FL;
      XNL = 15.6 < FL ? 15.6 : FL;
      
      NL=XNL*DESCR[4];                             
      if (BB0 < 1.0 ) BB0=1.0;                                             
      NB = ( BB0-1.0)*DESCR[6-1];  
      /*                                                                       
       * I2 IS THE NUMBER OF ELEMENTS IN THE FLUX MAP FOR THE FIRST ENERGY.  
       * I3 IS THE INDEX OF THE LAST ELEMENT OF THE SECOND ENERGY MAP.       
       * L3 IS THE LENGTH OF THE MAP FOR THE THIRD ENERGY.                   
       * E1 IS THE ENERGY OF THE FIRST ENERGY MAP (UNSCALED)                 
       * E2 IS THE ENERGY OF THE SECOND ENERGY MAP (UNSCALED)                
       */
      I1 =0;                                                             
      I2=MAP[1-1];                                                        
      I3=I2+MAP[I2+1-1]; 
      L3=MAP[I3+1-1];                                                      
      E1=MAP[I1+2-1]/ESCALE;
      E2=MAP[I2+2-1]/ESCALE;
      /*
       * S0, S1, S2 ARE LOGICAL VARIABLES WHICH INDICATE WHETHER THE FLUX FOR 
       * A PARTICULAR E, B, L POINT HAS ALREADY BEEN FOUND IN A PREVIOUS CALL  
       * TO FUNCTION TRARA2. IF NOT, S.. =.TRUE.
       */
      S1 = 1;
      S2 = 1;             
/* 
 *       ENERGY LOOP
 */ 
      /* DO 3 IE=1,N */
      for(IE=1; IE<=N; IE++) {                                                      
         /*
          * FOR EACH ENERGY E(I) FIND THE SUCCESSIVE ENERGIES E0,E1,E2 IN 
          * MODEL MAP, WHICH OBEY  E0 < E1 < E(I) < E2 . 
          */
         while (!( (E[IE-1] <= E2) || L3==0 ) ) {                                  
            I0=I1;                                                             
            I1=I2;                                                            
            I2=I3;                                                             
            I3=I3+L3;                                                          
            /* L3=MAP(I3-1) */
            L3 = MAP[I3+1-1];                                                      
            E0=E1;                                                             
            E1=E2;                                                             
            /* E2=MAP[I2+2]/ESCALE */
            E2=MAP[I2+2-1]/ESCALE;
            S0=S1;                                                            
            S1=S2;                                                            
            /* S2=.TRUE. */
            S2=1;                                                         
            F0=F1;                                                             
            F1=F2;                                
         } /* while continues here */

         /*
          * CALL TRARA2 TO INTERPOLATE THE FLUX-MAPS FOR E1,E2 IN L-B/B0-
          * SPACE TO FIND FLUXES F1,F2 [IF THEY HAVE NOT ALREADY BEEN 
          * CALCULATED FOR A PREVIOUS E(I)].
          */
         if(S1) F1=TRARA2(&MAP[I1+3-1], NL, NB)/FSCALE;
         if(S2) F2=TRARA2(&MAP[I2+3-1],NL,NB)/FSCALE;
         S1 = 0;
         S2 = 0;                                                        
         /*
          * FINALLY, INTERPOLATE IN ENERGY.
          */
         F[IE-1]=F1+(F2-F1)*(E[IE-1]-E1)/(E2-E1);
         if (F2 <= 0.0 || I1 == 0 ) {
            /*                                                                       
             * --------- SPECIAL INTERPOLATION ---------------------------------
             * IF THE FLUX FOR THE SECOND ENERGY CANNOT BE FOUND (I.E. F2=0.0),
             * AND THE ZEROTH ENERGY MAP HAS BEEN DEFINED (I.E. I1 NOT EQUAL 0), 
             * THEN INTERPOLATE USING THE FLUX MAPS FOR THE ZEROTH AND FIRST 
             * ENERGY AND CHOOSE THE MINIMUM OF THIS INTERPOLATIONS AND THE
             * INTERPOLATION THAT WAS DONE WITH F2=0. 
             */                                                                       
            /* IF(S0) F0=TRARA2(&MAP(I0+3),NL,NB)/FSCALE  */
            if (S0) F0=TRARA2(&MAP[I0+3-1],NL,NB)/FSCALE;
            /* S0=.FALSE. */
            S0=0;                                                        
            /* F(IE)=AMIN1(F(IE),F0+(F1-F0)*(E(IE)-E0)/(E1-E0)) */
            if ( F[IE-1] < F0+(F1-F0)*(E[IE-1]-E0)/(E1-E0) )
               F[IE-1] =  F0+(F1-F0)*(E[IE-1]-E0)/(E1-E0);
          }    
      }  /* major for loop continues here */
  
      F[IE-1] = F[IE-1] > 0 ? F[IE-1] : 0;                                           
      return;
} /* trara1() ends */
                                                          
/******************************************************************************/
void RadBelt(float RadiusKm, float MagLatDeg, int NumEnergies, 
      float *ElectronEnergy, float *ProtonEnergy, double **Flux) 
{
      float Lvalue, BB0;
      float bottomF,altrad, RE, BetaValueF;
      static int First = 1;
      static float *LogFlux;
      int Ie;
      
      if (First) {
         First = 0;
         LoadMapsFromFiles();
         LogFlux = (float *) calloc(NumEnergies,sizeof(float));
      }

      RE = RadiusKm/6378.137;

      altrad = (float) MagLatDeg*3.14159/180.0;  /* magnetic latitude, not latitude ( +/- 11 deg ) */

      /* get the field by simple dipole, value in nanoteslas */
      BetaValueF = 30610.0*sqrt(cos(1.57-altrad)*cos(1.57-altrad)*3.0+1.0)/(RE*RE*RE);
 
      /* compute L value */
      bottomF = RE*RE*RE*RE*RE*RE;
      bottomF = 4 - (BetaValueF*BetaValueF*bottomF/(3.06e4*3.06e4));
      Lvalue = 3*sqrt(RE*RE)/bottomF ;

      /*
       *  From page 8 of the AP-8 Trapped Proton Documnent;
       *  also easily derivable from dipole approximation
       */
      BB0 = (BetaValueF*Lvalue*Lvalue)*(Lvalue/3.06e4);

      MAP = MapAeMin;
      TRARA1(Lvalue, BB0, ElectronEnergy, LogFlux, NumEnergies, Descriptors[AE_MIN]);
      for(Ie=0;Ie<NumEnergies;Ie++) {
         Flux[AE_MIN][Ie] = (double) pow((double) 10.0,(double) LogFlux[Ie]);
      }

      MAP = MapApMin;
      TRARA1(Lvalue, BB0, ProtonEnergy, LogFlux, NumEnergies, Descriptors[AP_MIN]);
      for(Ie=0;Ie<NumEnergies;Ie++) {
         Flux[AP_MIN][Ie] = (double) pow((double) 10.0,(double) LogFlux[Ie]);
      }
 
      MAP = MapAeMax;
      TRARA1(Lvalue, BB0, ElectronEnergy, LogFlux, NumEnergies, Descriptors[AE_MAX]);
      for(Ie=0;Ie<NumEnergies;Ie++) {
         Flux[AE_MAX][Ie] = (double) pow((double) 10.0,(double) LogFlux[Ie]);
      }

      MAP = MapApMax;
      TRARA1(Lvalue, BB0, ProtonEnergy, LogFlux, NumEnergies, Descriptors[AP_MAX]);
      for(Ie=0;Ie<NumEnergies;Ie++) {
         Flux[AP_MAX][Ie] = (double) pow((double) 10.0,(double) LogFlux[Ie]);
      }
 }
/******************************************************************************/
/* Driver for unit-testing purposes                                           */
#if 0
int main(int argc, char **argv)
{
      int NumEnergies = 5;
      float ElectronEnergy[5] = {0.15,0.5,1.0,3.0,4.0}; /* MeV */
      float ProtonEnergy[5] = {4.0,10.0,20.0,30.0,50.0}; /* MeV */
      double **Flux;
      float RadiusKm,MagLatDeg;
      int Im,Ie;
      
      Flux = (double **) calloc(4,sizeof(double *));
      for(Im=0;Im<4;Im++) Flux[Im] = (double *) calloc(NumEnergies,sizeof(double));

      RadiusKm = 1.999957*6378.137;
      MagLatDeg = 60.0;
      RadBelt(RadiusKm,MagLatDeg,NumEnergies,ElectronEnergy,ProtonEnergy,Flux);
      
      printf("Solar Min\n");
      printf("RE is: %f   MagLat is: %f\n",RadiusKm/6378.137,MagLatDeg);
      for(Ie=0;Ie<NumEnergies;Ie++) {
         printf("Electron Flux, Energy: %f MeV is : %f electrons/(cm^2-s)\n",
            ElectronEnergy[Ie],Flux[AE_MIN][Ie]);
      } 
      for(Ie=0;Ie<NumEnergies;Ie++) {
         printf("Proton Flux, Energy: %f MeV is : %f protons/(cm^2-s)\n",
            ProtonEnergy[Ie],Flux[AP_MIN][Ie]);
      } 
      printf("\n");
      
      printf("Solar Max\n");
      printf("RE is: %f   MagLat is: %f\n",RadiusKm/6378.137,MagLatDeg);
      for(Ie=0;Ie<NumEnergies;Ie++) {
         printf("Electron Flux, Energy: %f MeV is : %f electrons/(cm^2-s)\n",
            ElectronEnergy[Ie],Flux[AE_MAX][Ie]);
      } 
      for(Ie=0;Ie<NumEnergies;Ie++) {
         printf("Proton Flux, Energy: %f MeV is : %f protons/(cm^2-s)\n",
            ProtonEnergy[Ie],Flux[AP_MAX][Ie]);
      } 
      printf("\n");
      
      return(0);
}
#endif

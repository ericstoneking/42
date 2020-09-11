/*    This file is distributed with 42,                               */
/*    the (mostly harmless) spacecraft dynamics simulation            */
/*    created by Eric Stoneking of NASA Goddard Space Flight Center   */

/*    Copyright 2010 United States Government                         */
/*    as represented by the Administrator                             */
/*    of the National Aeronautics and Space Administration.           */

/*    No copyright is claimed in the United States                    */
/*    under Title 17, U.S. Code.                                      */

/*    All Other Rights Reserved.                                      */


#include "42.h"
#include <stdio.h>
#include <mathkit.h>
#include <timekit.h>

/* #ifdef __cplusplus
** namespace _42 {
** using namespace Kit;
** #endif
*/

/**********************************************************************/
/*  Nominal Schatten Model Solar Flux KP and AP data   */
void NominalAtmoParam(void)
{

      char junk[120],newline;
      double KPnomvalue;
      double nomF10,twosigF10,twosigKP;
      static double JD[410],KPnom1[410], JD2[59535],SolFlux[59535];
      static long First=1;
      double fileyear,filemonth;
      double Solyear,Solmonth,Solday;
      long n, j;
      FILE *NOAA_Flux;
      FILE *KPAP_Schattenfile;


      /* Get KP data from SolFlx0908_Schatten.txt */
      if (First){
         First=0;
         /**********Finding KP value**********/
         KPAP_Schattenfile = FileOpen(ModelPath,"SolFlx0908_Schatten.txt","r");
         if (KPAP_Schattenfile==NULL){
            fprintf(KPAP_Schattenfile,"Unable to open Schatten file.");
         }
         else{
            /* Save Date and KP info for use in interpolation */
            fscanf(KPAP_Schattenfile,"%[^\n] %[\n]",junk,&newline);
            fscanf(KPAP_Schattenfile,"%[^\n] %[\n]",junk,&newline);
            for(n=0;n<410;n++){
               fscanf(KPAP_Schattenfile,"%lf %lf %lf %lf %lf %lf,%[^\n] %[\n]",
                  &fileyear,&filemonth,&twosigF10,&nomF10,&twosigKP,
                  &KPnom1[n],junk,&newline);
               JD[n] = DateToJD(fileyear, filemonth, 01, 12, 00, 00);
            }
         }
         fclose(KPAP_Schattenfile);

         /* Finding F10.7 Flux value */
         NOAA_Flux = FileOpen(ModelPath,"NOAA_solarflux_table.txt","r");
         if (NOAA_Flux==NULL){
            fprintf(NOAA_Flux,"Unable to open NOAA file.");
         }
         else{
            /* Save Date and F10.7 info for use in interpolation */
            fscanf(KPAP_Schattenfile,"%[^\n] %[\n]",junk,&newline);
            fscanf(KPAP_Schattenfile,"%[^\n] %[\n]",junk,&newline);
            fscanf(KPAP_Schattenfile,"%[^\n] %[\n]",junk,&newline);
            fscanf(KPAP_Schattenfile,"%[^\n] %[\n]",junk,&newline);
            for(j=0;j<59535;j++){
               fscanf(NOAA_Flux,"%lf %lf %lf %lf %lf, %[^\n] %[\n]",
                  &JD2[j],&SolFlux[j],&Solyear,&Solmonth,&Solday,
                  junk,&newline);
            }
         }
         fclose(NOAA_Flux);
      }

      KPnomvalue=LinInterp(JD, KPnom1, TT.JulDay, 410);

      /* Use KP data to get AP values */
      if (KPnomvalue<=0.0)
         GeomagIndex= 0;
      else if(0.0<KPnomvalue && KPnomvalue<=0.1666)
         GeomagIndex= 2;
      else if(0.1666<KPnomvalue && KPnomvalue<=0.833)
         GeomagIndex= 3;
      else if(0.833<KPnomvalue && KPnomvalue<=1.1666)
         GeomagIndex= 4;
      else if(1.166<KPnomvalue && KPnomvalue<=1.833)
         GeomagIndex= 5;
      else if(1.833<KPnomvalue && KPnomvalue<=2.166)
         GeomagIndex= 6;
      else if(2.166<KPnomvalue && KPnomvalue<=2.5)
         GeomagIndex= 7;
      else if(2.5<KPnomvalue && KPnomvalue<=2.833)
         GeomagIndex= 9;
      else if(2.833<KPnomvalue && KPnomvalue<=3.166)
         GeomagIndex= 12;
      else if(3.166<KPnomvalue && KPnomvalue<=3.5)
         GeomagIndex= 15;
      else if(3.5<KPnomvalue && KPnomvalue<=3.833)
         GeomagIndex= 18;
      else if (3.833<KPnomvalue && KPnomvalue<=4.166)
         GeomagIndex= 22;
      else if (4.166<KPnomvalue && KPnomvalue<=4.5)
         GeomagIndex= 27;
      else if (4.5<KPnomvalue && KPnomvalue<=4.833)
         GeomagIndex= 32;
      else if (4.833<KPnomvalue && KPnomvalue<=5.166)
         GeomagIndex= 39;
      else if (5.166<KPnomvalue && KPnomvalue<=5.5)
         GeomagIndex= 48;
      else if (5.5<KPnomvalue && KPnomvalue<=5.833)
         GeomagIndex= 56;
      else if (5.833<KPnomvalue && KPnomvalue<=6.166)
         GeomagIndex= 67;
      else if (6.166<KPnomvalue && KPnomvalue<=6.5)
         GeomagIndex= 80;
      else if (6.5<KPnomvalue && KPnomvalue<=6.833)
         GeomagIndex= 94;
      else if (6.833<KPnomvalue && KPnomvalue<=7.166)
         GeomagIndex= 111;
      else if (7.166<KPnomvalue && KPnomvalue<=7.5)
         GeomagIndex= 132;
      else if (7.5<KPnomvalue && KPnomvalue<=7.833)
         GeomagIndex= 154;
      else if (7.833<KPnomvalue && KPnomvalue<=8.166)
         GeomagIndex= 179;
      else if (8.166<KPnomvalue && KPnomvalue<=8.5)
         GeomagIndex= 207;
      else if (8.5<KPnomvalue && KPnomvalue<=8.833)
         GeomagIndex= 236;
      else if (8.833<KPnomvalue && KPnomvalue<=9.166)
         GeomagIndex= 300;
      else if (9.166<KPnomvalue)
         GeomagIndex= 400;

      /*Interpolate date and solar flux info to find current F10.7 value*/
      Flux10p7=LinInterp(JD2, SolFlux, TT.JulDay, 59535);
}

/**********************************************************************/
/*  2 Sigma Schatten Model Solar Flux KP and AP data   */
void TwoSigmaAtmoParam(void)
{

      char junk[120],newline;
      double KPtwosig;
      double twosigF10,nomF10,nomKP;
      static double JD1[410],twosigmaKP[410],JD2[59535],SolFlux[59535];
      static long First=1;
      static double Solyear,Solmonth,Solday;
      double fileyear,filemonth;
      long n, j;
      FILE *KPAP_Schattenfile;
      FILE *NOAA_Flux;

      /* Get KP data from SolFlx0908_Schatten.txt */
      if (First){
         First=0;
         KPAP_Schattenfile = FileOpen(ModelPath,"SolFlx0908_Schatten.txt","r");
         if (KPAP_Schattenfile==NULL){
            fprintf(KPAP_Schattenfile,"Unable to open Schatten file.");
         }
         else{
            /* Save Date and KP info for use in interpolation */
            fscanf(KPAP_Schattenfile,"%[^\n] %[\n]",junk,&newline);
            fscanf(KPAP_Schattenfile,"%[^\n] %[\n]",junk,&newline);
            for(n=0;n<410;n++){
               fscanf(KPAP_Schattenfile,"%lf %lf %lf %lf %lf %lf,%[^\n] %[\n]",
                  &fileyear,&filemonth,&twosigF10,&nomF10,&twosigmaKP[n],
                  &nomKP,junk,&newline);
               JD1[n] = DateToJD(fileyear, filemonth, 01, 12, 00, 00);
            }
         }
         fclose(KPAP_Schattenfile);

          /**********Finding F10.7 Flux value**********/
          NOAA_Flux = FileOpen(ModelPath,"NOAA_solarflux_table.txt","r");
          if (NOAA_Flux==NULL){
             fprintf(NOAA_Flux,"Unable to open NOAA file.");
          }
         else{
            /* Save Date and F10.7 info for use in interpolation */
            for(j=0;j<59535;j++){
               fscanf(KPAP_Schattenfile,"%[^\n] %[\n]",junk,&newline);
               fscanf(KPAP_Schattenfile,"%[^\n] %[\n]",junk,&newline);
               fscanf(KPAP_Schattenfile,"%[^\n] %[\n]",junk,&newline);
               fscanf(KPAP_Schattenfile,"%[^\n] %[\n]",junk,&newline);
               fscanf(NOAA_Flux,"%lf %lf %lf %lf %lf, %[^\n] %[\n]",
                  &JD2[j],&SolFlux[j],&Solyear,&Solmonth,&Solday,
                  junk,&newline);
            }
         }
         fclose(NOAA_Flux);
      }

      /*Interpolate date and solar flux info to find current KP value*/
      KPtwosig = LinInterp(JD1, twosigmaKP, TT.JulDay, 410);

      /* Use KP data to get AP values */
      if (KPtwosig<=0.0)
         GeomagIndex=0;
      else if(0.0<KPtwosig && KPtwosig<=0.1666)
         GeomagIndex= 2;
      else if(0.1666<KPtwosig && KPtwosig<=0.833)
         GeomagIndex= 3;
      else if(0.833<KPtwosig && KPtwosig<=1.1666)
         GeomagIndex= 4;
      else if(1.166<KPtwosig && KPtwosig<=1.833)
         GeomagIndex= 5;
      else if(1.833<KPtwosig && KPtwosig<=2.166)
         GeomagIndex= 6;
      else if(2.166<KPtwosig && KPtwosig<=2.5)
         GeomagIndex= 7;
      else if(2.5<KPtwosig && KPtwosig<=2.833)
         GeomagIndex= 9;
      else if(2.833<KPtwosig && KPtwosig<=3.166)
         GeomagIndex= 12;
      else if(3.166<KPtwosig && KPtwosig<=3.5)
         GeomagIndex= 15;
      else if(3.5<KPtwosig && KPtwosig<=3.833)
         GeomagIndex= 18;
      else if (3.833<KPtwosig && KPtwosig<=4.166)
         GeomagIndex= 22;
      else if (4.166<KPtwosig && KPtwosig<=4.5)
         GeomagIndex= 27;
      else if (4.5<KPtwosig && KPtwosig<=4.833)
         GeomagIndex= 32;
      else if (4.833<KPtwosig && KPtwosig<=5.166)
         GeomagIndex= 39;
      else if (5.166<KPtwosig && KPtwosig<=5.5)
         GeomagIndex= 48;
      else if (5.5<KPtwosig && KPtwosig<=5.833)
         GeomagIndex= 56;
      else if (5.833<KPtwosig && KPtwosig<=6.166)
         GeomagIndex= 67;
      else if (6.166<KPtwosig && KPtwosig<=6.5)
         GeomagIndex= 80;
      else if (6.5<KPtwosig && KPtwosig<=6.833)
         GeomagIndex= 94;
      else if (6.833<KPtwosig && KPtwosig<=7.166)
         GeomagIndex= 111;
      else if (7.166<KPtwosig && KPtwosig<=7.5)
         GeomagIndex= 132;
      else if (7.5<KPtwosig && KPtwosig<=7.833)
         GeomagIndex= 154;
      else if (7.833<KPtwosig && KPtwosig<=8.166)
         GeomagIndex= 179;
      else if (8.166<KPtwosig && KPtwosig<=8.5)
         GeomagIndex= 207;
      else if (8.5<KPtwosig && KPtwosig<=8.833)
         GeomagIndex= 236;
      else if (8.833<KPtwosig && KPtwosig<=9.166)
         GeomagIndex= 300;
      else if (9.166<KPtwosig)
         GeomagIndex= 400;

      /*Interpolate date and solar flux info to find current F10.7 value*/
      Flux10p7=LinInterp(JD2, SolFlux, TT.JulDay, 59535);
}

/**********************************************************************/
/* #define _RADBELT_ */
void Environment(struct SCType *S)
{
      struct OrbitType *O;
      struct WorldType *P;
      double Alt;
      double PosW[3];
      #ifdef _RADBELT_
      int NumEnergies = 5; */
      float ElectronEnergy[5] = {0.15,0.5,1.0,3.0,4.0}; /* MeV */
      float ProtonEnergy[5] = {4.0,10.0,20.0,30.0,50.0}; /* MeV */
      static double **Flux;
      double MagLat;
      static long First = 1;
      
      if (First) {
         First = 0;
         Flux = CreateMatrix(4,NumEnergies);
      }
      #endif

      O = &Orb[S->RefOrb];
      P = &World[O->World];

/* .. Magnetic Field */
      if (MagModel.Type == DIPOLE) {
         DipoleMagField(P->DipoleMoment,P->DipoleAxis,
         P->DipoleOffset,S->PosN,P->PriMerAng,S->bvn);
      }
      else if (MagModel.Type == IGRF && O->World == EARTH) {
         IGRFMagField(ModelPath,MagModel.N,MagModel.M,S->PosN,
         P->PriMerAng,S->bvn);
      }
      else {
         S->bvn[0] = 0.0;
         S->bvn[1] = 0.0;
         S->bvn[2] = 0.0;
      }

      MxV(S->B[0].CN,S->bvn,S->bvb);

/* .. Atmospheric Density */
      /* .. Use of NOAA and Schatten files for Solar Flux and AP values */
      if (UseFileForInterpolation == TWOSIGMA_KP){
         TwoSigmaAtmoParam();
      printf("AP 2Sigma value is %lf \n", GeomagIndex);
      printf("Flux 2Sigma value is %lf \n", Flux10p7);
      }
      else if (UseFileForInterpolation == NOMINAL){
         NominalAtmoParam();
      printf("AP 2Sigma value is %lf \n", GeomagIndex);
      printf("Flux 2Sigma value is %lf \n", Flux10p7);
      }
      /* .. Atmospheric Density */
      if (O->World == EARTH) {
         MxV(World[EARTH].CWN,S->PosN,PosW);
         Alt = MAGV(PosW)-World[EARTH].rad;
         if (Alt < 1000.0E3) { /* What is max alt of MSISE00 validity? */
            S->AtmoDensity = NRLMSISE00(TT.Year,TT.doy,TT.Hour,TT.Minute,TT.Second,PosW,
                                        Flux10p7,GeomagIndex);
         }
         else S->AtmoDensity = 0.0;
      }

      else if (O->World == MARS) {
         S->AtmoDensity = MarsAtmosphereModel(S->PosN);
      }

      else S->AtmoDensity = 0.0;
      
/* .. Radiation Belt Electron and Proton Fluxes, particles/cm^2/sec */
      #ifdef _RADBELT_
      if (O->World == EARTH) {
         MxV(World[EARTH].CWN,S->PosN,PosW);
         UNITV(PosW);
         MagLat = asin(VoV(PosW,World[EARTH].DipoleAxis));
         RadBelt(MAGV(S->PosN)/1000.0,fabs(MagLat)*R2D,NumEnergies,
            ElectronEnergy,ProtonEnergy,Flux);
      }
      #endif

}

/* #ifdef __cplusplus
** }
** #endif
*/

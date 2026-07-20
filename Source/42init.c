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
#include <ctype.h>

/* #ifdef __cplusplus
** namespace _42 {
** using namespace Kit;
** #endif
*/

/**********************************************************************/
long DecodeString(char *s)
{

      unsigned long i;

      for(i=0;i<strlen(s);i++) s[i] = toupper(s[i]);

      if (!strcmp(s,"FALSE")) return FALSE;
      else if (!strcmp(s,"TRUE")) return TRUE;
      else if (!strcmp(s,"OFF")) return OFF;
      else if (!strcmp(s,"ON")) return ON;
      else if (!strcmp(s,"FAST")) return FAST_TIME;
      else if (!strcmp(s,"REAL")) return REAL_TIME;
      else if (!strcmp(s,"EXTERNAL")) return EXTERNAL_TIME;
      else if (!strcmp(s,"NOS3")) return NOS3_TIME;
      else if (!strcmp(s,"SOL")) return SOL;
      else if (!strcmp(s,"MERCURY")) return MERCURY;
      else if (!strcmp(s,"VENUS")) return VENUS;
      else if (!strcmp(s,"EARTH")) return EARTH;
      else if (!strcmp(s,"MARS")) return MARS;
      else if (!strcmp(s,"JUPITER")) return JUPITER;
      else if (!strcmp(s,"SATURN")) return SATURN;
      else if (!strcmp(s,"URANUS")) return URANUS;
      else if (!strcmp(s,"NEPTUNE")) return NEPTUNE;
      else if (!strcmp(s,"PLUTO")) return PLUTO;
      else if (!strcmp(s,"LUNA")) return LUNA;
      else if (!strcmp(s,"KEP")) return INP_KEPLER;
      else if (!strcmp(s,"RV")) return INP_POSVEL;
      else if (!strcmp(s,"FILE")) return INP_FILE;
      else if (!strcmp(s,"TLE")) return INP_TLE;
      else if (!strcmp(s,"TRV")) return INP_TRV;
      else if (!strcmp(s,"PA")) return TRUE;
      else if (!strcmp(s,"AE")) return FALSE;
      else if (!strcmp(s,"POSW")) return TRUE;
      else if (!strcmp(s,"LLA")) return FALSE;
      else if (!strcmp(s,"POS")) return POSITIVE;
      else if (!strcmp(s,"NEG")) return NEGATIVE;
      else if (!strcmp(s,"CM")) return TRUE;
      else if (!strcmp(s,"ORIGIN")) return FALSE;
      else if (!strcmp(s,"CENTRAL")) return ORB_CENTRAL;
      else if (!strcmp(s,"THREE_BODY")) return ORB_THREE_BODY;
      else if (!strcmp(s,"ZERO")) return ORB_ZERO;
      else if (!strcmp(s,"FLIGHT")) return ORB_FLIGHT;
      else if (!strcmp(s,"EARTHMOON")) return EARTHMOON;
      else if (!strcmp(s,"SUNEARTH")) return SUNEARTH;
      else if (!strcmp(s,"SUNJUPITER")) return SUNJUPITER;
      else if (!strcmp(s,"MODES")) return INP_MODES;
      else if (!strcmp(s,"XYZ")) return INP_XYZ;
      else if (!strcmp(s,"SPLINE")) return INP_SPLINE;
      else if (!strcmp(s,"L1")) return LAGPT_L1;
      else if (!strcmp(s,"L2")) return LAGPT_L2;
      else if (!strcmp(s,"L3")) return LAGPT_L3;
      else if (!strcmp(s,"L4")) return LAGPT_L4;
      else if (!strcmp(s,"L5")) return LAGPT_L5;
      else if (!strcmp(s,"CW")) return DIR_CW;
      else if (!strcmp(s,"CCW")) return DIR_CCW;
      else if (!strcmp(s,"NONE")) return NONE;
      else if (!strcmp(s,"DIPOLE")) return DIPOLE;
      else if (!strcmp(s,"IGRF")) return IGRF;
      else if (!strcmp(s,"SPHERICAL")) return 1;
      else if (!strcmp(s,"GIMBAL")) return 0;
      else if (!strcmp(s,"X_AXIS")) return X_AXIS;
      else if (!strcmp(s,"Y_AXIS")) return Y_AXIS;
      else if (!strcmp(s,"Z_AXIS")) return Z_AXIS;
      else if (!strcmp(s,"POS_X")) return POS_X;
      else if (!strcmp(s,"POS_Y")) return POS_Y;
      else if (!strcmp(s,"POS_Z")) return POS_Z;
      else if (!strcmp(s,"NEG_X")) return NEG_X;
      else if (!strcmp(s,"NEG_Y")) return NEG_Y;
      else if (!strcmp(s,"NEG_Z")) return NEG_Z;
      else if (!strcmp(s,"REFORB")) return TARGET_REFORB;
      else if (!strcmp(s,"FRM")) return TARGET_FRM;
      else if (!strcmp(s,"SC")) return TARGET_SC;
      else if (!strcmp(s,"BODY")) return TARGET_BODY;

      else if (!strcmp(s,"TRACK_HOST")) return TRACK_HOST;
      else if (!strcmp(s,"TRACK_TARGET")) return TRACK_TARGET;
      else if (!strcmp(s,"FIXED_IN_HOST")) return FIXED_IN_HOST;

      else if (!strcmp(s,"DOWN")) return VIEW_DOWN;
      else if (!strcmp(s,"REAR_LEFT")) return VIEW_REAR_LEFT;
      else if (!strcmp(s,"REAR")) return VIEW_REAR;
      else if (!strcmp(s,"REAR_RIGHT")) return VIEW_REAR_RIGHT;
      else if (!strcmp(s,"LEFT")) return VIEW_LEFT;
      else if (!strcmp(s,"UP")) return VIEW_UP;
      else if (!strcmp(s,"RIGHT")) return VIEW_RIGHT;
      else if (!strcmp(s,"FRONT_LEFT")) return VIEW_FRONT_LEFT;
      else if (!strcmp(s,"FRONT")) return VIEW_FRONT;
      else if (!strcmp(s,"FRONT_RIGHT")) return VIEW_FRONT_RIGHT;

      else if (!strcmp(s,"WIREFRAME")) return FOV_WIREFRAME;
      else if (!strcmp(s,"SOLID")) return FOV_SOLID;
      else if (!strcmp(s,"VECTOR")) return FOV_VECTOR;
      else if (!strcmp(s,"PLANE")) return FOV_PLANE;

      else if (!strcmp(s,"REFPT_CM")) return REFPT_CM;
      else if (!strcmp(s,"REFPT_JOINT")) return REFPT_JOINT;

      else if (!strcmp(s,"LAGDOF_MODES")) return LAGDOF_MODES;
      else if (!strcmp(s,"LAGDOF_COWELL")) return LAGDOF_COWELL;
      else if (!strcmp(s,"LAGDOF_SPLINE")) return LAGDOF_SPLINE;

      else if (!strcmp(s,"GAUSS_ELIM")) return DYN_GAUSS_ELIM;
      else if (!strcmp(s,"ORDER_N")) return DYN_ORDER_N;

      else if (!strcmp(s,"FIXED")) return ORBDOF_FIXED;
      else if (!strcmp(s,"EULER_HILL")) return ORBDOF_EULER_HILL;
      else if (!strcmp(s,"ENCKE")) return ORBDOF_ENCKE;
      else if (!strcmp(s,"COWELL")) return ORBDOF_COWELL;

      else if (!strcmp(s,"PASSIVE_FSW")) return PASSIVE_FSW;
      else if (!strcmp(s,"INSTANT_FSW")) return INSTANT_FSW;
      else if (!strcmp(s,"SANDBOX_FSW")) return SANDBOX_FSW;
      else if (!strcmp(s,"SPINNER_FSW")) return SPINNER_FSW;
      else if (!strcmp(s,"MOMBIAS_FSW")) return MOMBIAS_FSW;
      else if (!strcmp(s,"THREE_AXIS_FSW")) return THREE_AXIS_FSW;
      else if (!strcmp(s,"ISS_FSW")) return ISS_FSW;
      else if (!strcmp(s,"CMG_FSW")) return CMG_FSW;
      else if (!strcmp(s,"THR_FSW")) return THR_FSW;
      else if (!strcmp(s,"CFS_FSW")) return CFS_FSW;
      else if (!strcmp(s,"HEXAPOD_FSW")) return HEXAPOD_FSW;
      else if (!strcmp(s,"LUNARCOMM_FSW")) return LUNARCOMM_FSW;

      else if (!strcmp(s,"PHOBOS")) return PHOBOS;
      else if (!strcmp(s,"DEIMOS")) return DEIMOS;
      else if (!strcmp(s,"IO")) return IO;
      else if (!strcmp(s,"EUROPA")) return EUROPA;
      else if (!strcmp(s,"GANYMEDE")) return GANYMEDE;
      else if (!strcmp(s,"CALLISTO")) return CALLISTO;
      else if (!strcmp(s,"AMALTHEA")) return AMALTHEA;
      else if (!strcmp(s,"HIMALIA")) return HIMALIA;
      else if (!strcmp(s,"ELARA")) return ELARA;
      else if (!strcmp(s,"PASIPHAE")) return PASIPHAE;
      else if (!strcmp(s,"SINOPE")) return SINOPE;
      else if (!strcmp(s,"LYSITHEA")) return LYSITHEA;
      else if (!strcmp(s,"CARME")) return CARME;
      else if (!strcmp(s,"ANANKE")) return ANANKE;
      else if (!strcmp(s,"LEDA")) return LEDA;
      else if (!strcmp(s,"THEBE")) return THEBE;
      else if (!strcmp(s,"ADRASTEA")) return ADRASTEA;
      else if (!strcmp(s,"METIS")) return METIS;
      else if (!strcmp(s,"MIMAS")) return MIMAS;
      else if (!strcmp(s,"ENCELADUS")) return ENCELADUS;
      else if (!strcmp(s,"TETHYS")) return TETHYS;
      else if (!strcmp(s,"DIONE")) return DIONE;
      else if (!strcmp(s,"RHEA")) return RHEA;
      else if (!strcmp(s,"TITAN")) return TITAN;
      else if (!strcmp(s,"HYPERION")) return HYPERION;
      else if (!strcmp(s,"IAPETUS")) return IAPETUS;
      else if (!strcmp(s,"PHOEBE")) return PHOEBE;
      else if (!strcmp(s,"JANUS")) return JANUS;
      else if (!strcmp(s,"EPIMETHEUS")) return EPIMETHEUS;
      else if (!strcmp(s,"HELENE")) return HELENE;
      else if (!strcmp(s,"TELESTO")) return TELESTO;
      else if (!strcmp(s,"CALYPSO")) return CALYPSO;
      else if (!strcmp(s,"ATLAS")) return ATLAS;
      else if (!strcmp(s,"PROMETHEUS")) return PROMETHEUS;
      else if (!strcmp(s,"PANDORA")) return PANDORA;
      else if (!strcmp(s,"PAN")) return PAN;
      else if (!strcmp(s,"ARIEL")) return ARIEL;
      else if (!strcmp(s,"UMBRIEL")) return UMBRIEL;
      else if (!strcmp(s,"TITANIA")) return TITANIA;
      else if (!strcmp(s,"OBERON")) return OBERON;
      else if (!strcmp(s,"MIRANDA")) return MIRANDA;
      else if (!strcmp(s,"TRITON")) return TRITON;
      else if (!strcmp(s,"NERIED")) return NERIED;
      else if (!strcmp(s,"CHARON")) return CHARON;
      else if (sscanf(s,"MINORBODY_%ld",&i) == 1) return(55+i);

      else if (!strcmp(s,"SUN")) return SUN;
      else if (!strcmp(s,"PLANET")) return PLANET;
      else if (!strcmp(s,"MOON")) return MOON;
      else if (!strcmp(s,"ASTEROID")) return ASTEROID;
      else if (!strcmp(s,"COMET")) return COMET;

      else if (!strcmp(s,"SIDE")) return VIEW_SIDE;
      else if (!strcmp(s,"TOP")) return VIEW_TOP;
      else if (!strcmp(s,"TWOSIGMA")) return TWOSIGMA_ATMO;
      else if (!strcmp(s,"NOMINAL")) return NOMINAL_ATMO;
      else if (!strcmp(s,"USER")) return USER_ATMO;
      else if (!strcmp(s,"TX")) return IPC_TX;
      else if (!strcmp(s,"RX")) return IPC_RX;
      else if (!strcmp(s,"TXRX")) return IPC_TXRX;
      else if (!strcmp(s,"WRITEFILE")) return IPC_WRITEFILE;
      else if (!strcmp(s,"READFILE")) return IPC_READFILE;
      else if (!strcmp(s,"SPIRENT")) return IPC_SPIRENT;
      else if (!strcmp(s,"FFTB")) return IPC_FFTB;
      else if (!strcmp(s,"SERVER")) return IPC_SERVER;
      else if (!strcmp(s,"CLIENT")) return IPC_CLIENT;
      else if (!strcmp(s,"GMSEC_CLIENT")) return IPC_GMSEC_CLIENT;
      
      else if (!strcmp(s,"MEAN")) return EPH_MEAN;
      else if (!strcmp(s,"DE430")) return EPH_DE430;
      else if (!strcmp(s,"DE440")) return EPH_DE440;

      else if (!strcmp(s,"MAJOR")) return MAJOR_CONSTELL;
      else if (!strcmp(s,"ZODIAC")) return ZODIAC_CONSTELL;
      else if (!strcmp(s,"MINOR")) return MINOR_CONSTELL;
      
      else if (!strcmp(s,"PASSIVE")) return PASSIVE_JOINT;
      else if (!strcmp(s,"ACTUATED")) return ACTUATED_JOINT;
      else if (!strcmp(s,"STEPPER_MOTOR")) return STEPPER_MOTOR_JOINT;
      else if (!strcmp(s,"TVC_JOINT")) return TVC_JOINT;
      else if (!strcmp(s,"VIBRATION_ISOLATOR")) return VIBRATION_ISOLATOR_JOINT;
      else if (!strcmp(s,"SLOSH")) return SLOSH_JOINT;
      else if (!strcmp(s,"STEERING_MIRROR")) return STEERING_MIRROR_JOINT;
      else if (!strcmp(s,"AD_HOC_JOINT")) return AD_HOC_JOINT;

      else if (!strcmp(s,"FORCE")) return FORCE;
      else if (!strcmp(s,"TORQUE")) return TORQUE;

      else if (!strcmp(s,"PULSED")) return THR_PULSED;
      else if (!strcmp(s,"PROPORTIONAL")) return THR_PROPORTIONAL;
      
      else if (!strcmp(s,"APERTURE")) return OPT_APERTURE;
      else if (!strcmp(s,"CONIC")) return OPT_CONIC;
      else if (!strcmp(s,"DETECTOR")) return OPT_DETECTOR;
      else if (!strcmp(s,"THINLENS")) return OPT_THINLENS;
      
      else if (!strcmp(s,"CONCAVE")) return OPT_CONCAVE;
      else if (!strcmp(s,"CONVEX")) return OPT_CONVEX;
      
      else if (!strcmp(s,"UPLINK")) return UPLINK;
      else if (!strcmp(s,"DOWNLINK")) return DOWNLINK;
      else if (!strcmp(s,"CROSSLINK")) return CROSSLINK;

      else {
         printf("Bogus input %s in DecodeString (42init.c:%d)\n",s,__LINE__);
         exit(1);
      }
}
/**********************************************************************/
void EchoDyn(struct SCType *S)
{
      FILE *outfile;
      char OutFileName[80];
      char Fmt[40];
      struct DynType *D;
      struct BodyType *B;
      struct JointType *G;
      long i,j,Ib,Ig,Nf;

      if (Nsc == 1) strcpy(Fmt,"\0");
      else if (Nsc <= 10) sprintf(Fmt,"%1ld",S->ID);
      else sprintf(Fmt,"%02ld",S->ID);
      sprintf(OutFileName,"Dyn%s.42",Fmt);
      outfile = FileOpen(InOutPath,OutFileName,"w");

/* .. SC Structure */
      fprintf(outfile,"Dynamics Check for SC[%ld]\n\n",S->ID);
      fprintf(outfile,"Nb: %2ld   Ng: %2ld\n",S->Nb,S->Ng);
      fprintf(outfile,"Mass:  %lf\n",S->mass);
      fprintf(outfile,"cm:  %lf %lf %lf\n",S->cm[0],S->cm[1],S->cm[2]);
      fprintf(outfile,"I  :  %lf  %lf  %lf \n     %lf  %lf  %lf \n      %lf %lf  %lf\n",
         S->I[0][0],S->I[0][1],S->I[0][2],
         S->I[1][0],S->I[1][1],S->I[1][2],
         S->I[2][0],S->I[2][1],S->I[2][2]);
      fprintf(outfile,"PosR:  %lf %lf %lf\n",
         S->PosR[0],S->PosR[1],S->PosR[2]);
      fprintf(outfile,"VelR:  %lf %lf %lf\n\n",
         S->VelR[0],S->VelR[1],S->VelR[2]);

/* .. Dyn Structure */
      D = &S->Dyn;
      fprintf(outfile,"Dyn Structure\n-------------\n");
      fprintf(outfile,"Nu:  %ld   Nx:  %ld  Nf:  %ld\n",D->Nu,D->Nx,D->Nf);
      for(i=0;i<D->Nu;i++) fprintf(outfile,"u[%02ld]: %lf\n",i,D->u[i]);
      fprintf(outfile,"\n");
      for(i=0;i<D->Nx;i++) fprintf(outfile,"x[%02ld]: %lf\n",i,D->x[i]);
      fprintf(outfile,"\n");
      for(i=0;i<D->Nf;i++) fprintf(outfile,"[%02ld] uf: %lf   xf: %lf\n",
         i,D->uf[i],D->xf[i]);
      fprintf(outfile,"\n\n");

/* .. Body Structures */
      for(Ib=0;Ib<S->Nb;Ib++) {
         B = &S->B[Ib];
         fprintf(outfile,"Body Structure [%02ld]\n-------------------\n",Ib);
         fprintf(outfile,"Mass:  %lf\n",B->mass);
         fprintf(outfile,"cm:  %lf %lf %lf\n",B->cm[0],B->cm[1],B->cm[2]);
         fprintf(outfile,"c:   %lf %lf %lf\n",B->c[0],B->c[1],B->c[2]);
         fprintf(outfile,"I:  %lf %lf %lf\n     %lf %lf %lf\n   %lf %lf %lf\n",
            B->I[0][0],B->I[0][1],B->I[0][2],
            B->I[1][0],B->I[1][1],B->I[1][2],
            B->I[2][0],B->I[2][1],B->I[2][2]);
         fprintf(outfile,"wn:  %lf %lf %lf\n",
            B->wn[0],B->wn[1],B->wn[2]);
         fprintf(outfile,"qn:  %lf %lf %lf %lf\n",
            B->qn[0],B->qn[1],B->qn[2],B->qn[3]);
         fprintf(outfile,"vn:  %lf %lf %lf\n",
            B->vn[0],B->vn[1],B->vn[2]);
         fprintf(outfile,"pn:  %lf %lf %lf\n\n",
            B->pn[0],B->pn[1],B->pn[2]);
         fprintf(outfile,"CN:\n");
         fprintf(outfile,"%lf %lf %lf\n",
            B->CN[0][0],B->CN[0][1],B->CN[0][2]);
         fprintf(outfile,"%lf %lf %lf\n",
            B->CN[1][0],B->CN[1][1],B->CN[1][2]);
         fprintf(outfile,"%lf %lf %lf\n",
            B->CN[2][0],B->CN[2][1],B->CN[2][2]);
         fprintf(outfile,"Nf: %ld   f0: %ld\n",B->Nf,B->f0);
         fprintf(outfile,"Mf:\n");
         for(i=0;i<B->Nf;i++) {
            for(j=0;j<B->Nf;j++) fprintf(outfile,"  %lf",B->Mf[i][j]);
            fprintf(outfile,"\n");
         }
         fprintf(outfile,"\n");
         fprintf(outfile,"Kf:\n");
         for(i=0;i<B->Nf;i++) {
            for(j=0;j<B->Nf;j++) fprintf(outfile,"  %lf",B->Kf[i][j]);
            fprintf(outfile,"\n");
         }
         fprintf(outfile,"\n");
         fprintf(outfile,"Cf:\n");
         for(i=0;i<B->Nf;i++) {
            for(j=0;j<B->Nf;j++) fprintf(outfile,"  %lf",B->Cf[i][j]);
            fprintf(outfile,"\n");
         }
         fprintf(outfile,"\n");
         fprintf(outfile,"Pf:\n");
         for(i=0;i<3;i++) {
            for(j=0;j<B->Nf;j++) fprintf(outfile,"  %lf",B->Pf[i][j]);
            fprintf(outfile,"\n");
         }
         fprintf(outfile,"\n");
         fprintf(outfile,"Hf:\n");
         for(i=0;i<3;i++) {
            for(j=0;j<B->Nf;j++) fprintf(outfile,"  %lf",B->Hf[i][j]);
            fprintf(outfile,"\n");
         }
         fprintf(outfile,"\n");
      }
      fprintf(outfile,"\n");

/* .. Joint Structures */
      for(Ig=0;Ig<S->Ng;Ig++) {
         G = &S->G[Ig];
         fprintf(outfile,"Joint Structure [%02ld]\n---------------\n",Ig);
         fprintf(outfile,"Rotu0: %ld   Rotx0: %ld\n",G->Rotu0,G->Rotx0);
         fprintf(outfile,"Trnu0: %ld   Trnx0: %ld\n",G->Trnu0,G->Trnx0);
         fprintf(outfile,"ang:  %lf %lf %lf\n",
            G->Ang[0],G->Ang[1],G->Ang[2]);
         fprintf(outfile,"rate:  %lf %lf %lf\n",
            G->AngRate[0],G->AngRate[1],G->AngRate[2]);
         fprintf(outfile,"COI:\n");
         fprintf(outfile,"%lf %lf %lf\n",
            G->COI[0][0],G->COI[0][1],G->COI[0][2]);
         fprintf(outfile,"%lf %lf %lf\n",
            G->COI[1][0],G->COI[1][1],G->COI[1][2]);
         fprintf(outfile,"%lf %lf %lf\n",
            G->COI[2][0],G->COI[2][1],G->COI[2][2]);
         fprintf(outfile,"SpringCoef: %lf %lf %lf\n",
            G->RotSpringCoef[0],G->RotSpringCoef[1],G->RotSpringCoef[2]);
         fprintf(outfile,"DampCoef: %lf %lf %lf\n",
            G->RotDampCoef[0],G->RotDampCoef[1],G->RotDampCoef[2]);
         Nf = S->B[G->Bin].Nf;
         fprintf(outfile,"PSIi:\n");
         for(i=0;i<3;i++) {
            for(j=0;j<Nf;j++) fprintf(outfile," %lf",G->PSIi[i][j]);
            fprintf(outfile,"\n");
         }
         fprintf(outfile,"THETAi:\n");
         for(i=0;i<3;i++) {
            for(j=0;j<Nf;j++) fprintf(outfile," %lf",G->THETAi[i][j]);
            fprintf(outfile,"\n");
         }
         Nf = S->B[G->Bout].Nf;
         fprintf(outfile,"PSIo:\n");
         for(i=0;i<3;i++) {
            for(j=0;j<Nf;j++) fprintf(outfile," %lf",G->PSIo[i][j]);
            fprintf(outfile,"\n");
         }
         fprintf(outfile,"THETAo:\n");
         for(i=0;i<3;i++) {
            for(j=0;j<Nf;j++) fprintf(outfile," %lf",G->THETAo[i][j]);
            fprintf(outfile,"\n");
         }
         fprintf(outfile,"\n\n");
      }

      fclose(outfile);
}
/**********************************************************************/
long LoadTRVfromFile(const char *Path, const char *TrvFileName,
   const char *ElemLabel, double Time, struct OrbitType *O)
{
      FILE *infile;
      char line[121],response1[121],response2[121];
      char Label[25];
      long i,Nchar;
      long Success = 0;
      double EpochJD,R[3],V[3];
      long EpochYear,EpochMonth,EpochDay,EpochHour,EpochMinute;
      double EpochSecond;

      infile=FileOpen(Path,TrvFileName,"r");

      Nchar = strlen(ElemLabel);
      /* Pad label to 24 characters to assure unique match */
      while(!feof(infile) && !Success) {
         fgets(line,79,infile);
         if (sscanf(line,"\"%[^\"]\"",Label) == 1) {
            if (!strncmp(Label,ElemLabel,Nchar)) {
               Success = 1;
               ScanLine(infile,"%120s %120s %ld-%ld-%ld %ld:%ld:%lf",8,
                  response1,response2,
                  &EpochYear,&EpochMonth,&EpochDay,
                  &EpochHour,&EpochMinute,&EpochSecond);
               ScanLine(infile,"%lf %lf %lf",3,&R[0],&R[1],&R[2]);
               ScanLine(infile,"%lf %lf %lf",3,&V[0],&V[1],&V[2]);
            }
         }
      }
      fclose(infile);

      if (Success) {
         /* Epoch is in UTC */
         EpochJD = DateToJD(EpochYear,EpochMonth,EpochDay,
            EpochHour,EpochMinute,EpochSecond);
         O->Epoch = JDToTime(EpochJD);
         O->Epoch += DynTime-CivilTime;
         O->Regime = DecodeString(response1);
         if (O->Regime == ORB_CENTRAL) {
            O->World = DecodeString(response2);
            O->mu = World[O->World].mu;
            RV2Eph(O->Epoch,O->mu,R,V,&O->SMA,&O->ecc,&O->inc,&O->RAAN,
               &O->ArgP,&O->anom,&O->tp,&O->SLR,&O->alpha,&O->rmin,
               &O->MeanMotion,&O->Period);
            Eph2RV(O->mu,O->SLR,O->ecc,O->inc,O->RAAN,O->ArgP,Time-O->Epoch,
               O->PosN,O->VelN,&O->anom);
         }
         else {
            O->Sys = DecodeString(response2);
            O->Body1 = LagSys[O->Sys].Body1;
            O->Body2 = LagSys[O->Sys].Body2;
            O->mu1 = World[O->Body1].mu;
            O->mu2 = World[O->Body2].mu;
            O->World = O->Body1;
            O->mu = O->mu1;
            for(i=0;i<3;i++) {
               O->PosN[i] = R[i];
               O->VelN[i] = V[i];
            }
            /* RV2LagModes(O->Epoch,&LagSys[O->Sys],O); */
            R2StableLagMode(O->Epoch,&LagSys[O->Sys],O);
            LagModes2RV(Time,&LagSys[O->Sys],O,O->PosN,O->VelN);
         }
      }

      return(Success);
}
/*********************************************************************/
void InitOrbit(struct OrbitType *O)
{
      FILE *infile;

      char response[121];
      double Alt1,Alt2,MaxAnom;
      double mu,rad,J2;
      double p[3],Ang1,Ang2,Ang3;
      char FrmExpressedIn;
      long Ir,i,j,k,Seq;
      struct FormationType *F;
      double AmpXY1,PhiXY1,SenseXY1;
      double AmpXY2,PhiXY2,SenseXY2;
      double AmpZ,PhiZ;
      struct RegionType *R;
      long InputType,UsePA,ElementType;
      char ElementLabel[41];
      char ElementFileName[41];
      long Success;
      long NodeYear,NodeDOY,NodeMonth,NodeDay,NodeHour,NodeMin;
      double NodeSec;
      char SplineLine[256];
      long NumMonDay,NumDOY;
      char sep;
      char MonDayFmt[121] = " %4ld%[ ,/:-]%ld%[ ,/:-]%ld%[ ,/:T-]%ld:%ld:%lf %lf %lf %lf %lf %lf %lf";
      char DOYFmt[121] =    " %4ld%[ ,/:-]%ld%[ ,/:T-]%ld:%ld:%lf %lf %lf %lf %lf %lf %lf";

      infile = FileOpen(InOutPath,O->FileName,"r");

/* .. Orbit Parameters */
      O->Epoch = DynTime;
      O->SplineActive = FALSE;
      ScanLine(infile,"",0,NULL);
      ScanLine(infile,"",0,NULL);
      ScanLine(infile,"%120s",1,response);
      O->Regime = DecodeString(response);

      if (O->Regime == ORB_ZERO) {
         ScanLine(infile,"",0,NULL);
         ScanLine(infile,"%120s",1,response);
         O->World = DecodeString(response);
         if (!World[O->World].Exists) {
            printf("Oops.  Orbit %ld depends on a World that doesn't exist.\n",O->Tag);
            exit(1);
         }
         O->mu = World[O->World].mu;
         for(j=0;j<3;j++) {
            O->PosN[j] = 0.0;
            O->VelN[j] = 0.0;
            for(k=0;k<3;k++) O->CLN[j][k] = 0.0;
            O->CLN[j][j] = 1.0;
            O->wln[j] = 0.0;
         }
         ScanLine(infile,"%120s",1,response);
         O->PolyhedronGravityEnabled = DecodeString(response);
         /* Skip FLIGHT, CENTRAL, THREE_BODY sections */
         for(j=0;j<36;j++) ScanLine(infile,"",0,NULL);
      }
      else if (O->Regime == ORB_FLIGHT) {
         /* Skip ZERO section */
         for(j=0;j<3;j++) ScanLine(infile,"",0,NULL);
         ScanLine(infile,"",0,NULL);
         ScanLine(infile,"%ld",1,&Ir);
         if (!Rgn[Ir].Exists) {
            printf("Oops.  Orbit %ld depends on a Region that doesn't exist.\n",O->Tag);
            exit(1);
         }
         O->Region = Ir;
         R = &Rgn[Ir];
         O->World = R->World;
         O->mu = World[O->World].mu;
         for(j=0;j<3;j++) {
            O->PosN[j] = R->PosN[j];
            O->VelN[j] = R->VelN[j];
            for(k=0;k<3;k++) O->CLN[j][k] = R->CN[j][k];
         }
         O->wln[0] = 0.0;
         O->wln[1] = 0.0;
         O->wln[2] = World[O->World].w;
         ScanLine(infile,"%120s",1,response);
         O->PolyhedronGravityEnabled = DecodeString(response);

         /* Skip CENTRAL and THREE_BODY sections */
         for(j=0;j<33;j++) ScanLine(infile,"",0,NULL);
      }
      else if (O->Regime == ORB_CENTRAL) {
         /* Skip ZERO and FLIGHT sections */
         for(j=0;j<6;j++) ScanLine(infile,"",0,NULL);

         ScanLine(infile,"",0,NULL);
         ScanLine(infile,"%120s",1,response);
         O->World = DecodeString(response);
         if (!World[O->World].Exists) {
            printf("Oops.  Orbit %ld depends on a World that doesn't exist.\n",O->Tag);
            exit(1);
         }
         ScanLine(infile,"%120s",1,response);
         O->J2DriftEnabled = DecodeString(response);
         mu = World[O->World].mu;
         rad = World[O->World].rad;
         J2 = World[O->World].J2;
         O->mu=mu;
         ScanLine(infile,"%120s",1,response);
         InputType = DecodeString(response);
         if (InputType == INP_KEPLER) {
            ScanLine(infile,"%120s",1,response);
            UsePA = DecodeString(response);
            if (UsePA) {
               ScanLine(infile,"%lf %lf",2,&Alt1,&Alt2);
               if (Alt2 < Alt1) {
                  printf("Apoapsis below Periapsis for Orbit %ld\n",O->Tag);
                  exit(1);
               }
               ScanLine(infile,"",0,NULL);
               O->SMA=rad+0.5*(Alt1+Alt2)*1.0E3;
               O->ecc=1.0E3*fabs(Alt1-Alt2)/(2.0*O->SMA);
               O->SLR = O->SMA*(1.0-O->ecc*O->ecc);
               O->alpha = 1.0/O->SMA;
               O->rmin = rad+Alt1*1.0E3;
               O->MeanMotion = sqrt(O->mu*O->alpha)*O->alpha;
            }
            else {
               ScanLine(infile,"",0,NULL);
               ScanLine(infile,"%lf %lf",2,&Alt1,&O->ecc);
               O->rmin = rad+Alt1*1.0E3;
               O->SLR = O->rmin*(1.0+O->ecc);
               O->alpha = (1.0-O->ecc)/O->rmin;
               if (O->alpha != 0.0) O->SMA = 1.0/O->alpha;
               if (O->alpha > 0.0) O->MeanMotion = sqrt(O->mu*O->alpha)*O->alpha;
               else O->MeanMotion = sqrt(-O->mu*O->alpha)*O->alpha;
            }
            ScanLine(infile,"%lf",1,&O->inc);
            ScanLine(infile,"%lf",1,&O->RAAN);
            ScanLine(infile,"%lf",1,&O->ArgP);
            ScanLine(infile,"%lf",1,&O->anom);
            O->inc *= D2R;
            O->RAAN *= D2R;
            O->ArgP *= D2R;
            O->anom *= D2R;
            O->tp = O->Epoch - TimeSincePeriapsis(O->mu,O->SLR,O->ecc,O->anom);

            /* Some anomalies are unreachable for hyperbolic trajectories */
            if (O->ecc > 1.0) {
               MaxAnom = Pi - acos(1.0/O->ecc);
               if (fabs(O->anom) > MaxAnom){
                  printf("True Anomaly out of range for Orbit %ld\n",O->Tag);
                  exit(1);
               }
            }

            if (O->J2DriftEnabled) {
               OscEphToMeanEph(mu,J2,rad,DynTime0,O);
            }
            Eph2RV(O->mu,O->SLR,O->ecc,O->inc,
                   O->RAAN,O->ArgP,O->Epoch-O->tp,
                   O->PosN,O->VelN,&O->anom);

            /* Skip RV and FILE */
            for(j=0;j<5;j++) ScanLine(infile,"",0,NULL);
         }
         else if (InputType == INP_POSVEL) {
            /* Skip KEPLER section */
            for(j=0;j<7;j++) ScanLine(infile,"",0,NULL);
            ScanLine(infile,"%lf %lf %lf",3,
               &O->PosN[0],&O->PosN[1],&O->PosN[2]);
            ScanLine(infile,"%lf %lf %lf",3,
                  &O->VelN[0],&O->VelN[1],&O->VelN[2]);
            for(j=0;j<3;j++){
               O->PosN[j] *= 1.0E3;
               O->VelN[j] *= 1.0E3;
            }            
            RV2Eph(O->Epoch,O->mu,O->PosN,O->VelN,
               &O->SMA,&O->ecc,&O->inc,&O->RAAN,
               &O->ArgP,&O->anom,&O->tp,
               &O->SLR,&O->alpha,&O->rmin,
               &O->MeanMotion,&O->Period);
            if (O->J2DriftEnabled) {
               OscEphToMeanEph(mu,J2,rad,DynTime0,O);
            }
            /* Skip FILE section */
            for(j=0;j<3;j++) ScanLine(infile,"",0,NULL);
         }
         else if (InputType == INP_FILE) {
            /* Skip KEP and RV sections */
            for(j=0;j<9;j++) ScanLine(infile,"",0,NULL);
            ScanLine(infile,"%120s",1,response);
            ElementType = DecodeString(response);
            ScanLine(infile,"\"%40[^\"]\"",1,ElementFileName);
            ScanLine(infile,"\"%40[^\"]\"",1,ElementLabel);
            if (ElementType == INP_TLE) {
               if (O->World != EARTH) {
                  printf("TLEs are only defined for Earth-orbiting S/C.\n");
                  exit(1);
               }
               Success = LoadTleFromFile(InOutPath,ElementFileName,
                  ElementLabel,DynTime,TT.JulDay,LeapSec,O);
               if (!Success) {
                  printf("Error loading TLE %s from file %s.\n",
                     ElementLabel,ElementFileName);
                  exit(1);
               }
               MeanEph2RV(O,DynTime);
            }
            else if (ElementType == INP_TRV) {
               Success = LoadTRVfromFile(InOutPath, ElementFileName,
                  ElementLabel, CivilTime, O);
               if (!Success) {
                  printf("Error loading TRV %s from file %s.\n",
                     ElementLabel,ElementFileName);
                  exit(1);
               }
               //O->tp = O->Epoch - TimeSincePeriapsis(O->mu,O->SLR,O->ecc,O->anom);
               //if (O->J2DriftEnabled) {
               //   OscEphToMeanEph(O->mu,World[O->World].J2,World[O->World].rad,DynTime,O);
               //}
               //O->MeanMotion = sqrt(O->mu/(O->SMA*O->SMA*O->SMA));
               //O->Period = TwoPi/O->MeanMotion;
               //Eph2RV(O->mu,O->SLR,O->ecc,O->inc,
               //       O->RAAN,O->ArgP,O->Epoch-O->tp,
               //       O->PosN,O->VelN,&O->anom);
            }
            else if (ElementType == INP_SPLINE) {
               O->SplineFile = FileOpen(InOutPath,ElementFileName,"rt");
               O->SplineActive = TRUE;
               i = 0;
               NumMonDay = 0;
               NumDOY = 0;
               while (NumMonDay != 15 && NumDOY != 13) { /* Discard any header lines */
                  fgets(SplineLine,255,O->SplineFile);
                  NumMonDay = sscanf(SplineLine,MonDayFmt,
                     &NodeYear,&sep,&NodeMonth,&sep,&NodeDay,&sep,
                     &NodeHour,&NodeMin,&NodeSec,
                     &O->NodePos[i][0],&O->NodePos[i][1],&O->NodePos[i][2],
                     &O->NodeVel[i][0],&O->NodeVel[i][1],&O->NodeVel[i][2]);
                  NumDOY = sscanf(SplineLine,DOYFmt,
                     &NodeYear,&sep,&NodeDOY,&sep,
                     &NodeHour,&NodeMin,&NodeSec,
                     &O->NodePos[i][0],&O->NodePos[i][1],&O->NodePos[i][2],
                     &O->NodeVel[i][0],&O->NodeVel[i][1],&O->NodeVel[i][2]);
               }
               if (NumMonDay == 15) {
                  O->SplineDateFormat = DATE_MON_DAY;
                  strcpy(O->SplineFmt,MonDayFmt);
               }
               else {
                  O->SplineDateFormat = DATE_DOY;
                  strcpy(O->SplineFmt,DOYFmt);
                  DOY2MD(NodeYear,NodeDOY,&NodeMonth,&NodeDay);                  
               }
               for(i=1;i<4;i++) {
                  fgets(SplineLine,255,O->SplineFile);
                  if (O->SplineDateFormat == DATE_MON_DAY) {
                     sscanf(SplineLine,O->SplineFmt,
                        &NodeYear,&sep,&NodeMonth,&sep,&NodeDay,&sep,
                        &NodeHour,&NodeMin,&NodeSec,
                        &O->NodePos[i][0],&O->NodePos[i][1],&O->NodePos[i][2],
                        &O->NodeVel[i][0],&O->NodeVel[i][1],&O->NodeVel[i][2]);
                  }
                  else {
                     sscanf(SplineLine,O->SplineFmt,
                        &NodeYear,&sep,&NodeDOY,&sep,
                        &NodeHour,&NodeMin,&NodeSec,
                        &O->NodePos[i][0],&O->NodePos[i][1],&O->NodePos[i][2],
                        &O->NodeVel[i][0],&O->NodeVel[i][1],&O->NodeVel[i][2]);
                     DOY2MD(NodeYear,NodeDOY,&NodeMonth,&NodeDay);                  
                  }
                  O->NodeDynTime[i] = DateToTime(NodeYear,NodeMonth,NodeDay,
                     NodeHour,NodeMin,NodeSec);
                  O->NodeDynTime[i] += DynTime-CivilTime; /* Adjust from UTC to TT */
                  for(j=0;j<3;j++) {
                     O->NodePos[i][j] *= 1000.0;
                     O->NodeVel[i][j] *= 1000.0;
                  }
                  if (DynTime < O->NodeDynTime[1]) {
                     printf("Oops.  Spline file beginning is in the future.\n");
                     exit(1);
                  }
               }
               SplineToPosVel(O);
            }
            else {
               printf("Oops.  Unknown ElementType in InitOrbit.\n");
               exit(1);
            }
         }
         else {
            printf("Oops.  Unknown InputType in InitOrbit.\n");
            exit(1);
         }
         FindCLN(O->PosN,O->VelN,O->CLN,O->wln);

         /* Skip THREE_BODY section */
         for(j=0;j<17;j++) ScanLine(infile,"",0,NULL);

      }
      else if (O->Regime == ORB_THREE_BODY) {
         /* Skip ZERO, FLIGHT, and CENTRAL sections */
         for(j=0;j<22;j++) ScanLine(infile,"",0,NULL);

         ScanLine(infile,"",0,NULL);
         ScanLine(infile,"%120s",1,response);
         O->Sys = DecodeString(response);
         if (!LagSys[O->Sys].Exists) {
            printf("Oops.  Orbit %ld depends on a Lagrange System that doesn't exist.\n",O->Tag);
            exit(1);
         }
         O->Body1 = LagSys[O->Sys].Body1;
         O->Body2 = LagSys[O->Sys].Body2;
         O->mu1 =  LagSys[O->Sys].mu1;
         O->mu2 =  LagSys[O->Sys].mu2;
         ScanLine(infile,"%120s",1,response);
         O->LagDOF = DecodeString(response);
         ScanLine(infile,"%120s",1,response);
         InputType = DecodeString(response);
         if (InputType == INP_MODES) { /* Initial modes given */
            ScanLine(infile,"%120s",1,response);
            O->LP = DecodeString(response);
            ScanLine(infile,"%lf",1,&AmpXY1);
            ScanLine(infile,"%lf",1,&PhiXY1);
            ScanLine(infile,"%120s",1,response);
            if (DecodeString(response) == DIR_CCW) SenseXY1 = 1.0;
            else SenseXY1 = -1.0;
            ScanLine(infile,"%lf",1,&AmpXY2);
            ScanLine(infile,"%lf",1,&PhiXY2);
            ScanLine(infile,"%120s",1,response);
            if (DecodeString(response) == DIR_CCW) SenseXY2 = 1.0;
            else SenseXY2 = -1.0;
            ScanLine(infile,"%lf",1,&AmpZ);
            ScanLine(infile,"%lf",1,&PhiZ);
            AmpXY1  *= 1000.0;
            AmpXY2  *= 1000.0;
            AmpZ    *= 1000.0;
            PhiXY1 *= D2R;
            PhiXY2 *= D2R;
            PhiZ   *= D2R;
            AmpPhase2LagModes(0.0,AmpXY1,PhiXY1,SenseXY1,
               AmpXY2,PhiXY2,SenseXY2,AmpZ,PhiZ,
               &LagSys[O->Sys],O);
            /* Find r,v from modal description */
            LagModes2RV(DynTime,&LagSys[O->Sys],O,O->PosN,O->VelN);
            /* Skip XYZ and FILE sections */
            for(j=0;j<4;j++) ScanLine(infile,"",0,NULL);
         }
         else if (InputType == INP_XYZ) {
            /* Skip MODES section */
            for(j=0;j<9;j++) ScanLine(infile,"",0,NULL);
            ScanLine(infile,"%lf %lf %lf",3,&O->x,&O->y,&O->z);
            ScanLine(infile,"%lf %lf %lf",3,&O->xdot,&O->ydot,&O->zdot);
            XYZ2LagModes(0.0,&LagSys[O->Sys],O);
            LagModes2RV(DynTime,&LagSys[O->Sys],O,O->PosN,O->VelN);
            /* Skip FILE section */
            for(j=0;j<2;j++) ScanLine(infile,"",0,NULL);
         }
         else if (InputType == INP_FILE) {
            ScanLine(infile,"%120s",1,response);
            O->LP = DecodeString(response);
            /* Skip MODES and XYZ sections */
            for(j=0;j<10;j++) ScanLine(infile,"",0,NULL);
            ScanLine(infile,"%120s \"%40[^\"]\"",2,response,ElementLabel);
            ElementType = DecodeString(response);
            ScanLine(infile,"\"%40[^\"]\"",1,ElementFileName);
            if (ElementType == INP_TRV) {
               Success = LoadTRVfromFile(InOutPath, ElementFileName,
                  ElementLabel, CivilTime, O);
               if (!Success) {
                  printf("Error loading TRV %s from file %s.\n",
                     ElementLabel,ElementFileName);
                  exit(1);
               }
            }
            else if (ElementType == INP_SPLINE) {
               O->SplineFile = FileOpen(InOutPath,ElementFileName,"rt");
               O->SplineActive = TRUE;
               i = 0;
               NumMonDay = 0;
               NumDOY = 0;
               while (NumMonDay != 15 && NumDOY != 13) { /* Discard any header lines */
                  fgets(SplineLine,255,O->SplineFile);
                  NumMonDay = sscanf(SplineLine,MonDayFmt,
                     &NodeYear,&sep,&NodeMonth,&sep,&NodeDay,&sep,
                     &NodeHour,&NodeMin,&NodeSec,
                     &O->NodePos[i][0],&O->NodePos[i][1],&O->NodePos[i][2],
                     &O->NodeVel[i][0],&O->NodeVel[i][1],&O->NodeVel[i][2]);
                  NumDOY = sscanf(SplineLine,DOYFmt,
                     &NodeYear,&sep,&NodeDOY,&sep,
                     &NodeHour,&NodeMin,&NodeSec,
                     &O->NodePos[i][0],&O->NodePos[i][1],&O->NodePos[i][2],
                     &O->NodeVel[i][0],&O->NodeVel[i][1],&O->NodeVel[i][2]);
               }
               if (NumMonDay == 15) {
                  O->SplineDateFormat = DATE_MON_DAY;
                  strcpy(O->SplineFmt,MonDayFmt);
               }
               else {
                  O->SplineDateFormat = DATE_DOY;
                  strcpy(O->SplineFmt,DOYFmt);
                  DOY2MD(NodeYear,NodeDOY,&NodeMonth,&NodeDay);                  
               }
               for(i=1;i<4;i++) {
                  fgets(SplineLine,255,O->SplineFile);
                  if (O->SplineDateFormat == DATE_MON_DAY) {
                     sscanf(SplineLine,O->SplineFmt,
                        &NodeYear,&sep,&NodeMonth,&sep,&NodeDay,&sep,
                        &NodeHour,&NodeMin,&NodeSec,
                        &O->NodePos[i][0],&O->NodePos[i][1],&O->NodePos[i][2],
                        &O->NodeVel[i][0],&O->NodeVel[i][1],&O->NodeVel[i][2]);
                  }
                  else {
                     sscanf(SplineLine,O->SplineFmt,
                        &NodeYear,&sep,&NodeDOY,&sep,
                        &NodeHour,&NodeMin,&NodeSec,
                        &O->NodePos[i][0],&O->NodePos[i][1],&O->NodePos[i][2],
                        &O->NodeVel[i][0],&O->NodeVel[i][1],&O->NodeVel[i][2]);
                     DOY2MD(NodeYear,NodeDOY,&NodeMonth,&NodeDay);                  
                  }
                  O->NodeDynTime[i] = DateToTime(NodeYear,NodeMonth,NodeDay,
                     NodeHour,NodeMin,NodeSec);
                  for(j=0;j<3;j++) {
                     O->NodePos[i][j] *= 1000.0;
                     O->NodeVel[i][j] *= 1000.0;
                  }
                  if (DynTime < O->NodeDynTime[1]) {
                     printf("Oops.  Spline file beginning is in the future.\n");
                     exit(1);
                  }
               }
               SplineToPosVel(O);
            }
            else {
               printf("Oops.  Unknown ElementType in InitOrbit.\n");
               exit(1);
            }
         }
         else {
            printf("Oops.  Unknown Input Type in InitOrbit.\n");
            exit(1);
         }
         O->World = O->Body1;
         O->mu = O->mu1;
         O->SMA = MAGV(O->PosN); /* For sake of EH */
         FindCLN(O->PosN,O->VelN,O->CLN,O->wln);
         O->MeanMotion = LagSys[O->Sys].MeanRate;
         O->Period = TwoPi/O->MeanMotion;
      }
      else {
         printf("Bogus Orbit Regime in file %s\n",O->FileName);
         exit(1);
      }

/* .. Formation Frame Parameters */
      F = &Frm[O->Tag];
      ScanLine(infile,"",0,NULL);
      ScanLine(infile,"%120s",1,response);
      F->FixedInFrame = response[0];
      ScanLine(infile,"%lf %lf %lf %ld",4,&Ang1,&Ang2,&Ang3,&Seq);
      A2C(Seq,Ang1*D2R,Ang2*D2R,Ang3*D2R,F->CN);
      if(F->FixedInFrame == 'L') {
         /* Adjust CFN */
         for(j=0;j<3;j++){
            for(k=0;k<3;k++) F->CL[j][k] = F->CN[j][k];
         }
         MxM(F->CL,O->CLN,F->CN);
      }
      ScanLine(infile,"%120s",1,response);
      FrmExpressedIn = response[0];
      ScanLine(infile,"%lf %lf %lf",3,&F->PosR[0],&F->PosR[1],&F->PosR[2]);
      if (FrmExpressedIn == 'L') {
         for(j=0;j<3;j++) p[j] = F->PosR[j];
         MTxV(O->CLN,p,F->PosR);
      }

      fclose(infile);

}
/**********************************************************************/
void InitRigidDyn(struct SCType *S)
{
      long i,j,Ig,Ia,Jg,Jb,Ibody,Ib,u0,x0,c0,Nu,Nx;
      struct JointType *G;
      struct DynType *D;
      FILE *outfile;
      char filename[80];
      char Fmt[40];

      D = &S->Dyn;

/* .. Tree Tables */
      /* Connectivity Table */
      /* Inner body -> -1, Outer body -> +1, else 0 */
      D->ConnectTable = (long **) calloc(S->Nb,sizeof(long *));
      for(i=0;i<S->Nb;i++)
         D->ConnectTable[i] = (long *) calloc(S->Ng,sizeof(long));
      for(Ib=0;Ib<S->Nb;Ib++) {
         for(Ig=0;Ig<S->Ng;Ig++) {
            D->ConnectTable[Ib][Ig] = 0;
         }
      }
      for(Ig=0;Ig<S->Ng;Ig++) {
         D->ConnectTable[S->G[Ig].Bin][Ig] = -1;
         D->ConnectTable[S->G[Ig].Bout][Ig] =  1;
      }

      /* Joint Path Table */
      /* Joint in path -> 1, else 0 */
      D->JointPathTable = (struct JointPathTableType **)
         calloc(S->Nb,sizeof(struct JointPathTableType *));
      for(i=0;i<S->Nb;i++)
         D->JointPathTable[i] = (struct JointPathTableType *)
            calloc(S->Ng,sizeof(struct JointPathTableType));
      for(Ibody=0;Ibody<S->Nb;Ibody++) {
         Ib = Ibody;
         while(Ib > 0) {
            Ig = S->B[Ib].Gin;
            D->JointPathTable[Ibody][Ig].InPath = 1;
            Ib = S->G[Ig].Bin;
         }
      }
      /* Body Path Table */
      /* Body in path -> 1, else 0 */
      D->BodyPathTable = (struct BodyPathTableType **)
         calloc(S->Nb,sizeof(struct BodyPathTableType *));
      for(i=0;i<S->Nb;i++)
         D->BodyPathTable[i] = (struct BodyPathTableType *)
            calloc(S->Nb,sizeof(struct BodyPathTableType));
      for(Ibody=0;Ibody<S->Nb;Ibody++) {
         Ib = Ibody;
         while(Ib > 0) {
            Ig = 0;
            while(S->G[Ig].Bout != Ib) Ig++;
            D->BodyPathTable[Ibody][Ib].InPath = 1;
            Ib = S->G[Ig].Bin;
         }
         D->BodyPathTable[Ibody][0].InPath = 1;
      }
      
      /* Find ancestor joint list for each joint */
      for(Ig=0;Ig<S->Ng;Ig++) {
         G = &S->G[Ig];
         Ib = S->G[Ig].Bin;
         G->Nanc = 0;
         while(Ib > 0) {
            G->Nanc++;
            Jg = S->B[Ib].Gin;
            Ib = S->G[Jg].Bin;
         }
         G->Anc = (long *) calloc(G->Nanc,sizeof(long));
         Ib = S->G[Ig].Bin;
         Ia = 0;
         while(Ib > 0) {
            Jg = S->B[Ib].Gin;
            G->Anc[Ia] = Jg;
            Ib = S->G[Jg].Bin;
            Ia++;
         }
      }

      /* Determine sizes of state vectors */
      D->Nu = 6;
      D->Nx = 7;
      u0 = 3;
      x0 = 4;
      D->Nc = 0;
      c0 = 0;
      for(Ig=0;Ig<S->Ng;Ig++) {
         G = &S->G[Ig];
         if (G->IsSpherical) {
            G->RotDOF = 3;
            G->RotSeq = 123;
            D->Nu += 3;
            D->Nx += 4;
            G->Rotu0 = u0;
            G->Rotx0 = x0;
            u0 += 3;
            x0 += 4;
         }
         else { /* Is Gimbal */
            D->Nu += G->RotDOF;
            D->Nx += G->RotDOF;
            G->Rotu0 = u0;
            G->Rotx0 = x0;
            u0 += G->RotDOF;
            x0 += G->RotDOF;
            G->Rotc0 = c0;
            c0 += 3-G->RotDOF;
         }
         D->Nu += G->TrnDOF;
         D->Nx += G->TrnDOF;
         G->Trnu0 = u0;
         G->Trnx0 = x0;
         u0 += G->TrnDOF;
         x0 += G->TrnDOF;
         G->Trnc0 = c0;
         c0 += 3-G->TrnDOF;
      }

/* .. Allocate Workspace */
      D->PAngVel = CreateMatrix(3*S->Nb,D->Nu);
      D->IPAngVel = CreateMatrix(3*S->Nb,D->Nu);
      D->PVel = CreateMatrix(3*S->Nb,D->Nu);
      D->mPVel = CreateMatrix(3*S->Nb,D->Nu);
      D->BodyTrq = (double *) calloc(3*S->Nb,sizeof(double));
      D->BodyFrc = (double *) calloc(3*S->Nb,sizeof(double));

      D->u = (double *) calloc(D->Nu,sizeof(double));
      D->uu = (double *) calloc(D->Nu,sizeof(double));
      D->du = (double *) calloc(D->Nu,sizeof(double));
      D->udot = (double *) calloc(D->Nu,sizeof(double));
      D->x = (double *) calloc(D->Nx,sizeof(double));
      D->xx = (double *) calloc(D->Nx,sizeof(double));
      D->dx = (double *) calloc(D->Nx,sizeof(double));
      D->xdot = (double *) calloc(D->Nx,sizeof(double));
      D->h = (double *) calloc(S->Nw,sizeof(double));
      D->hh = (double *) calloc(S->Nw,sizeof(double));
      D->dh = (double *) calloc(S->Nw,sizeof(double));
      D->hdot = (double *) calloc(S->Nw,sizeof(double));
      D->a = (double *) calloc(S->Nw,sizeof(double));
      D->aa = (double *) calloc(S->Nw,sizeof(double));
      D->da = (double *) calloc(S->Nw,sizeof(double));
      D->adot = (double *) calloc(S->Nw,sizeof(double));

      for(i=0;i<3;i++) {
         D->PAngVel[i][i] = 1.0;
         for(j=0;j<3;j++) D->IPAngVel[i][j] = S->B[0].I[i][j];
      }
      for(Ib=0;Ib<S->Nb;Ib++) {
         for(i=0;i<3;i++) {
            D->PVel[3*Ib+i][D->Nu-3+i] = 1.0;
            D->mPVel[3*Ib+i][D->Nu-3+i] = S->B[Ib].mass;
         }
      }

      if (S->ConstraintsRequested) {
         D->Nc = 6*S->Nb - D->Nu;
         D->PAngVelc = CreateMatrix(3*S->Nb,D->Nc);
         D->PVelc = CreateMatrix(3*S->Nb,D->Nc);
         D->TotalTrq = (double *) calloc(3*S->Nb,sizeof(double));
         D->TotalFrc = (double *) calloc(3*S->Nb,sizeof(double));
         D->GenConstraintFrc = (double *) calloc(D->Nc,sizeof(double));
      }

      MapJointStatesToStateVector(S);

/* .. Echo tree tables */
      if (Nsc == 1) strcpy(Fmt,"\0");
      else if (Nsc <= 10) sprintf(Fmt,"%1ld",S->ID);
      else sprintf(Fmt,"%02ld",S->ID);
      sprintf(filename,"Tree%s.42",Fmt);
      outfile = FileOpen(InOutPath,filename,"w");
      fprintf(outfile,"SC %2ld:  Nb = %2ld  Ng = %2ld\n\n",S->ID,S->Nb,S->Ng);
      fprintf(outfile,"Connect Table:\n\n");
      fprintf(outfile,"     ");
      for(Ig=0;Ig<S->Ng;Ig++) fprintf(outfile,"  G[%02ld]",Ig);
      fprintf(outfile,"\n");
      for(Ib=0;Ib<S->Nb;Ib++) {
         fprintf(outfile,"B[%02ld]:",Ib);
         for(Ig=0;Ig<S->Ng;Ig++) {
            fprintf(outfile,"  %3ld  ",D->ConnectTable[Ib][Ig]);
         }
         fprintf(outfile,"\n");
      }
      fprintf(outfile,"\n\n\nJoint Path Table:\n\n");
      fprintf(outfile,"     ");
      for(Ig=0;Ig<S->Ng;Ig++) fprintf(outfile,"  G[%02ld]",Ig);
      fprintf(outfile,"\n");
      for(Ib=0;Ib<S->Nb;Ib++) {
         fprintf(outfile,"B[%02ld]:",Ib);
         for(Ig=0;Ig<S->Ng;Ig++) {
            fprintf(outfile,"  %3ld  ",D->JointPathTable[Ib][Ig].InPath);
         }
         fprintf(outfile,"\n");
      }
      fprintf(outfile,"\n\n\nBody Path Table:\n\n");
      fprintf(outfile,"     ");
      for(Jb=0;Jb<S->Nb;Jb++) fprintf(outfile,"  B[%02ld]",Jb);
      fprintf(outfile,"\n");
      for(Ib=0;Ib<S->Nb;Ib++) {
         fprintf(outfile,"B[%02ld]:",Ib);
         for(Jb=0;Jb<S->Nb;Jb++) {
            fprintf(outfile,"  %3ld  ",D->BodyPathTable[Ib][Jb].InPath);
         }
         fprintf(outfile,"\n");
      }

/* .. Echo State Vector */
      fprintf(outfile,"\n\n\nState Vector Map:\n\n");
      fprintf(outfile,"Body/Joint   RotDOF   TrnDOF   RotSeq   TrnSeq       u[]      x[]\n");
      fprintf(outfile,"  B[00]        wn       ---      123      ---       00-02    00-03\n");
      for(Ig=0;Ig<S->Ng;Ig++) {
         G = &S->G[Ig];
         Nu = G->RotDOF+G->TrnDOF;
         Nx = (G->IsSpherical ? Nu+1 : Nu);
         fprintf(outfile,"  G[%02ld]       %3ld      %3ld       %3ld      %3ld       %02ld-%02ld    %02ld-%02ld\n",
            Ig,G->RotDOF,G->TrnDOF,G->RotSeq,G->TrnSeq,G->Rotu0,G->Rotu0+Nu-1,G->Rotx0,G->Rotx0+Nx-1);
      }
      fprintf(outfile,"  B[00]       ---       vn       ---      123       %02ld-%02ld    %02ld-%02ld\n",
      D->Nu-3,D->Nu-1,D->Nx-3,D->Nx-1);

/* .. DOF/Constraint Map */
      fprintf(outfile,"\n\n\nMap all 6*Nb potential DOF Axes into DOFs or Constraints\n\n");
      fprintf(outfile,"*****************************************************************\n");
      fprintf(outfile,"Body 00:   RotSeq = 123   TrnSeq = 123\n");
      fprintf(outfile,"                                Col in   Col in       Col in\n");
      fprintf(outfile,"Axis      F/C    u[]  x[]       u%02ld.42   x%02ld.42   Constraint%02ld.42\n",S->ID,S->ID,S->ID);
      fprintf(outfile,"-----------------------------------------------------------------\n");
      fprintf(outfile,"Rot1       F      00   00         01       01           --\n");
      fprintf(outfile,"Rot2       F      01   01         02       02           --\n");
      fprintf(outfile,"Rot3       F      02   02         03       03           --\n");
      fprintf(outfile,"(Sph)      -      --   03         --       04           --\n\n");
      fprintf(outfile,"Trn1       F      %02ld   %02ld         %02ld       %02ld           --\n",D->Nu-3,D->Nx-3,D->Nu-2,D->Nx-2);
      fprintf(outfile,"Trn2       F      %02ld   %02ld         %02ld       %02ld           --\n",D->Nu-2,D->Nx-2,D->Nu-1,D->Nx-1);
      fprintf(outfile,"Trn3       F      %02ld   %02ld         %02ld       %02ld           --\n",D->Nu-1,D->Nx-1,D->Nu,D->Nx);
      for(Ig=0;Ig<S->Ng;Ig++) {
         G = &S->G[Ig];
         fprintf(outfile,"*****************************************************************\n");
         fprintf(outfile,"Joint %02ld:   RotSeq = %3ld   TrnSeq = %3ld\n",Ig,G->RotSeq,G->TrnSeq);
         fprintf(outfile,"                                Col in   Col in       Col in\n");
         fprintf(outfile,"Axis      F/C    u[]  x[]       u%02ld.42   x%02ld.42   Constraint%02ld.42\n",S->ID,S->ID,S->ID);
         fprintf(outfile,"-----------------------------------------------------------------\n");
         for(i=0;i<G->RotDOF;i++) {
            fprintf(outfile,"Rot%ld       F      %02ld   %02ld         %02ld       %02ld           --\n",
               i+1,G->Rotu0+i,G->Rotx0+i,G->Rotu0+i+1,G->Rotx0+i+1);
         }
         for(i=0;i<3-G->RotDOF;i++) {
            fprintf(outfile,"Rot%ld       C      --   --         --       --           %02ld\n",G->RotDOF+i+1,G->Rotc0+i+1);
         }
         if (G->IsSpherical) {
            fprintf(outfile,"(Sph)      -      --   %02ld         --       %02ld           --\n",G->Rotx0+3,G->Rotx0+4);
         }
         fprintf(outfile,"\n");
         for(i=0;i<G->TrnDOF;i++) {
            fprintf(outfile,"Trn%ld       F      %02ld   %02ld         %02ld       %02ld           --\n",
               i,G->Trnu0+i+1,G->Trnx0+i,G->Trnu0+i+1,G->Trnx0+i+1);
         }
         for(i=0;i<3-G->TrnDOF;i++) {
            fprintf(outfile,"Trn%ld       C      --   --         --       --           %02ld\n",G->TrnDOF+i+1,G->Trnc0+i+1);
         }
      }
      fclose(outfile);
}
/**********************************************************************/
void InitFlexModes(struct SCType *S)
{
      FILE *infile;
      struct DynType *D;
      struct BodyType *B;
      struct JointType *G;
      long Ib,Ig,If,Im,Jm,Ia,In;
      long Nnonzero,Iz;
      long i,j;
      double value,wf;
      double ***L,****N;
      struct NodeType *FN;

      D = &S->Dyn;
      D->Nf = 0;

/* .. First pass through all flex input files to allocate matrices */
      for(Ib=0;Ib<S->Nb;Ib++) {
         B = &S->B[Ib];
         B->Nf = 0;
         if (strcmp(B->FlexFileName,"NONE")) {
            infile = FileOpen(InOutPath,B->FlexFileName,"r");
            ScanLine(infile,"",0,NULL);
            ScanLine(infile,"",0,NULL);
            /* Number of Flex Modes */
            ScanLine(infile,"%ld",1,&B->Nf);
            B->f0 = D->Nf;
            D->Nf += B->Nf;
            /* Allocate matrices */
            B->xi = (double *) calloc(B->Nf,sizeof(double));
            B->eta = (double *) calloc(B->Nf,sizeof(double));
            B->Mf = CreateMatrix(B->Nf,B->Nf);
            B->Kf = CreateMatrix(B->Nf,B->Nf);
            B->Cf = CreateMatrix(B->Nf,B->Nf);
            B->Pf = CreateMatrix(3,B->Nf);
            B->Hf = CreateMatrix(3,B->Nf);
            B->CnbP = CreateMatrix(3,B->Nf);
            B->HplusQeta = CreateMatrix(3,B->Nf);
            B->Qxi = CreateMatrix(3,B->Nf);
            B->Rw = CreateMatrix(3,B->Nf);
            B->Swe = CreateMatrix(3,B->Nf);
            /* Allocate higher-order tensors */
            B->Qf = (double *) calloc(3*B->Nf*B->Nf,sizeof(double));
            B->Rf = (double *) calloc(3*B->Nf*3,sizeof(double));
            B->Sf = (double *) calloc(3*B->Nf*B->Nf*3,sizeof(double));
            B->Sw = (double *) calloc(3*B->Nf*B->Nf,sizeof(double));
            fclose(infile);
         }
      }
      /* For Joint Nodes */
      for(Ig=0;Ig<S->Ng;Ig++) {
         G = &S->G[Ig];
         B = &S->B[G->Bin];
         if (B->Nf > 0) {
            G->PSIi = CreateMatrix(3,B->Nf);
            G->THETAi = CreateMatrix(3,B->Nf);
         }
         B = &S->B[G->Bout];
         if (B->Nf > 0) {
            G->PSIo = CreateMatrix(3,B->Nf);
            G->THETAo = CreateMatrix(3,B->Nf);
         }
      }
      D->PAngVelf = CreateMatrix(3*S->Nb,D->Nf);
      D->IPAngVelf = CreateMatrix(3*S->Nb,D->Nf);
      D->PVelf = CreateMatrix(3*S->Nb,D->Nf);
      D->mPVelf = CreateMatrix(3*S->Nb,D->Nf);
      D->uf = (double *) calloc(D->Nf,sizeof(double));
      D->uuf = (double *) calloc(D->Nf,sizeof(double));
      D->duf = (double *) calloc(D->Nf,sizeof(double));
      D->ufdot = (double *) calloc(D->Nf,sizeof(double));
      D->xf = (double *) calloc(D->Nf,sizeof(double));
      D->xxf = (double *) calloc(D->Nf,sizeof(double));
      D->dxf = (double *) calloc(D->Nf,sizeof(double));
      D->xfdot = (double *) calloc(D->Nf,sizeof(double));
      D->FlexAcc = (double *) calloc(D->Nf,sizeof(double));
      D->FlexFrc = (double *) calloc(D->Nf,sizeof(double));
      D->FlexInertiaFrc = (double *) calloc(D->Nf,sizeof(double));
      D->Mf = CreateMatrix(D->Nf,D->Nf);
      D->PCPVelf = CreateMatrix(D->Nf,D->Nf);
      D->HplusQetaPAngVelf = CreateMatrix(D->Nf,D->Nf);

/* .. Second pass through all flex files to read in values */
      for(Ib=0;Ib<S->Nb;Ib++) {
         B = &S->B[Ib];
         if (strcmp(B->FlexFileName,"NONE")) {
            infile = FileOpen(InOutPath,B->FlexFileName,"r");
            ScanLine(infile,"",0,NULL);
            ScanLine(infile,"",0,NULL);
            ScanLine(infile,"",0,NULL);

            /* Initial Modal States x, u */
            ScanLine(infile,"",0,NULL);
            for(If=0;If<B->Nf;If++) {
               ScanLine(infile,"%lf %lf",2,&B->eta[If],&B->xi[If]);
               D->xf[B->f0+If] = B->eta[If];
               D->uf[B->f0+If] = B->xi[If];
            }

            /* Node-related matrices */
            for(In=0;In<B->NumNodes;In++) {
               FN = &B->Node[In];
               FN->PSI = CreateMatrix(3,B->Nf);
               FN->THETA = CreateMatrix(3,B->Nf);
               for(i=0;i<3;i++) {
                  FN->Frc[i] = 0.0;
                  FN->Trq[i] = 0.0;
               }
               FN->FlexFrc = (double *) calloc(B->Nf,sizeof(double));
            }

            /**** Joint Node Mode Shapes ****/
            ScanLine(infile,"",0,NULL);
            /* Non-zero Translation Mode Shape (PSI) Elements */
            ScanLine(infile,"",0,NULL);
            ScanLine(infile,"%ld",1,&Nnonzero);
            for(Iz=0;Iz<Nnonzero;Iz++) {
               ScanLine(infile,"%ld %ld %ld %lf",4,&Im,&Ig,&Ia,&value);
               if (Ig >= S->Ng) {
                  printf("Error in InitFlexModes: Joint %ld out of range\n",Ig);
                  exit(1);
               }
               if (Ia > 2) {
                  printf("Error in InitFlexModes (PSI): Axis %ld out of range\n",Ia);
                  exit(1);
               }
               if (Im >= B->Nf) {
                  printf("Error in InitFlexModes (PSI): Flex Mode %ld out of range\n",Im);
                  exit(1);
               }
               G = &S->G[Ig];
               if (Ib == G->Bin) G->PSIi[Ia][Im] = value;
               else if (Ib == G->Bout) G->PSIo[Ia][Im] = value;
               else {
                  printf("Error in InitFlexModes (PSI): Body %ld not connected to Joint %ld\n",Ib,Ig);
                  exit(1);
               }
            }
            /* Non-zero Rotation Mode Shape (THETA) Elements */
            ScanLine(infile,"",0,NULL);
            ScanLine(infile,"%ld",1,&Nnonzero);
            for(Iz=0;Iz<Nnonzero;Iz++) {
               ScanLine(infile,"%ld %ld %ld %lf",4,&Im,&Ig,&Ia,&value);
               if (Ig >= S->Ng) {
                  printf("Error in InitFlexModes: Joint %ld out of range\n",Ig);
                  exit(1);
               }
               if (Ia > 2) {
                  printf("Error in InitFlexModes (THETA): Axis %ld out of range\n",Ia);
                  exit(1);
               }
               if (Im >= B->Nf) {
                  printf("Error in InitFlexModes (THETA): Flex Mode %ld out of range\n",Im);
                  exit(1);
               }
               G = &S->G[Ig];
               if (Ib == G->Bin) G->THETAi[Ia][Im] = value;
               else if (Ib == G->Bout) G->THETAo[Ia][Im] = value;
               else {
                  printf("Error in InitFlexModes (THETA): Body %ld not connected to Joint %ld\n",Ib,Ig);
                  exit(1);
               }
            }

            /**** Analysis Node Mode Shapes ****/
            ScanLine(infile,"",0,NULL);
            /* Non-zero Translation Mode Shape (PSI) Elements */
            ScanLine(infile,"",0,NULL);
            ScanLine(infile,"%ld",1,&Nnonzero);
            for(Iz=0;Iz<Nnonzero;Iz++) {
               ScanLine(infile,"%ld %ld %ld %lf",4,&Im,&In,&Ia,&value);
               if (In > B->NumNodes-1) {
                  printf("Error in InitFlexModes (PSI):  Node %ld out of range\n",In);
                  exit(1);
               }
               FN = &B->Node[In];
               if (Ia > 2) {
                  printf("Error in InitFlexModes (PSI): Axis %ld out of range\n",Ia);
                  exit(1);
               }
               if (Im >= B->Nf) {
                  printf("Error in InitFlexModes (PSI): Flex Mode %ld out of range\n",Im);
                  exit(1);
               }
               FN->PSI[Ia][Im] = value;
            }

            /* Non-zero Rotation Mode Shape (THETA) Elements */
            ScanLine(infile,"",0,NULL);
            ScanLine(infile,"%ld",1,&Nnonzero);
            for(Iz=0;Iz<Nnonzero;Iz++) {
               ScanLine(infile,"%ld %ld %ld %lf",4,&Im,&In,&Ia,&value);
               FN = &B->Node[In];
               if (In > B->NumNodes-1) {
                  printf("Error in InitFlexModes (THETA):  Node %ld out of range\n",In);
                  exit(1);
               }
               if (Ia > 2) {
                  printf("Error in InitFlexModes (THETA): Axis %ld out of range\n",Ia);
                  exit(1);
               }
               if (Im >= B->Nf) {
                  printf("Error in InitFlexModes (THETA): Flex Mode %ld out of range\n",Im);
                  exit(1);
               }
               FN->THETA[Ia][Im] = value;
            }

            /* Non-zero Mass Matrix Elements */
            ScanLine(infile,"",0,NULL);
            ScanLine(infile,"%ld",1,&Nnonzero);
            for(Iz=0;Iz<Nnonzero;Iz++) {
               ScanLine(infile,"%ld %ld %lf",3,&i,&j,&value);
               if (i >= B->Nf || j >= B->Nf) {
                  printf("Error in InitFlexModes: Mass Matrix index [%ld][%ld] out of range\n",i,j);
                  exit(1);
               }
               B->Mf[i][j] = value;
            }
            /* Non-zero Stiffness Matrix Elements */
            ScanLine(infile,"",0,NULL);
            ScanLine(infile,"%ld",1,&Nnonzero);
            for(Iz=0;Iz<Nnonzero;Iz++) {
               ScanLine(infile,"%ld %ld %lf",3,&i,&j,&value);
               if (i >= B->Nf || j >= B->Nf) {
                  printf("Error in InitFlexModes: Stiffness Matrix index [%ld][%ld] out of range\n",i,j);
                  exit(1);
               }
               B->Kf[i][j] = value;
            }
            /* Non-zero Damping Matrix Elements */
            ScanLine(infile,"",0,NULL);
            ScanLine(infile,"%ld",1,&Nnonzero);
            for(Iz=0;Iz<Nnonzero;Iz++) {
               ScanLine(infile,"%ld %ld %lf",3,&i,&j,&value);
               if (i >= B->Nf || j >= B->Nf) {
                  printf("Error in InitFlexModes: Damping Matrix index [%ld][%ld] out of range\n",i,j);
                  exit(1);
               }
               B->Cf[i][j] = value;
            }

            /* Check modal frequencies to make sure DTSIM is small enough */
            for(i=0;i<B->Nf;i++) {
               wf = sqrt(B->Kf[i][i]/B->Mf[i][i]);
               if (Pi/wf < DTSIM) {
                  printf("Oops.  Natural frequency of Flex Mode %ld of Body %ld of SC %ld is too high to be sampled at time step of %lf.\n",i,Ib,S->ID,DTSIM);
                  printf("Suggest setting DTSIM < %lf sec\n",0.2*TwoPi/wf); /* 5 samples/cycle */
                  exit(1);
               }
            }

            /* Linear Momentum Modal Integral, Pf, 3 x Nf */
            ScanLine(infile,"",0,NULL);
            for(Im=0;Im<B->Nf;Im++) {
               ScanLine(infile,"%lf %lf %lf",3,
                  &B->Pf[0][Im],&B->Pf[1][Im],&B->Pf[2][Im]);
            }
            /* Angular Momentum Modal Integral, Hf, 3 x Nf */
            ScanLine(infile,"",0,NULL);
            for(Im=0;Im<B->Nf;Im++) {
               ScanLine(infile,"%lf %lf %lf",3,
                  &B->Hf[0][Im],&B->Hf[1][Im],&B->Hf[2][Im]);
            }
            /* Don't trust input file to make Hf, and Pf be zero */
            if (S->RefPt == REFPT_CM) {
               for(Im=0;Im<B->Nf;Im++) {
                  B->Pf[0][Im] = 0.0;
                  B->Pf[1][Im] = 0.0;
                  B->Pf[2][Im] = 0.0;
                  B->Hf[0][Im] = 0.0;
                  B->Hf[1][Im] = 0.0;
                  B->Hf[2][Im] = 0.0;
               }
            }

            /* Non-zero Elements of Linear Modal Integral, L, 3 x 3 x Nf */
            L = (double ***) calloc(3,sizeof(double **));
            if (L==NULL) {
               printf("Allocation failed in %s:%d\n",__FILE__,__LINE__);
               exit(1);
            }
            for(i=0;i<3;i++) L[i] = CreateMatrix(3,B->Nf);
            ScanLine(infile,"",0,NULL);
            ScanLine(infile,"%ld",1,&Nnonzero);
            for(Iz=0;Iz<Nnonzero;Iz++) {
               ScanLine(infile,"%ld %ld %ld %lf",4,&i,&j,&Im,&value);
               if (i >= 3 || j >= 3 || Im >= B->Nf) {
                  printf("Error in InitFlexModes: L index [%ld][%ld][%ld] out of range\n",i,j,Im);
                  exit(1);
               }
               L[i][j][Im] = value;
            }
            /* B->Rf, 3 x Nf x 3 */
            for(i=0;i<B->Nf;i++) {
               B->Rf[IDX3(0,i,0,B->Nf,3)] = -L[1][1][i]-L[2][2][i];
               B->Rf[IDX3(1,i,1,B->Nf,3)] = -L[2][2][i]-L[0][0][i];
               B->Rf[IDX3(2,i,2,B->Nf,3)] = -L[0][0][i]-L[1][1][i];
               B->Rf[IDX3(0,i,1,B->Nf,3)] = L[1][0][i];
               B->Rf[IDX3(0,i,2,B->Nf,3)] = L[2][0][i];
               B->Rf[IDX3(1,i,0,B->Nf,3)] = L[0][1][i];
               B->Rf[IDX3(1,i,2,B->Nf,3)] = L[2][1][i];
               B->Rf[IDX3(2,i,0,B->Nf,3)] = L[0][2][i];
               B->Rf[IDX3(2,i,1,B->Nf,3)] = L[1][2][i];
            }
            for(i=0;i<3;i++) DestroyMatrix(L[i],3);
            free(L);

            /* Non-zero Elements of Angular Modal Integral, N, 3 x 3 x Nf x Nf*/
            N = (double ****) calloc(3,sizeof(double ***));
            if (N==NULL) {
               printf("Allocation failed in %s:%d\n",__FILE__,__LINE__);
               exit(1);
            }
            for(i=0;i<3;i++) {
               N[i] = (double ***) calloc(3,sizeof(double **));
               for(j=0;j<3;j++) {
                  N[i][j] = CreateMatrix(B->Nf,B->Nf);
               }
            }
            ScanLine(infile,"",0,NULL);
            ScanLine(infile,"%ld",1,&Nnonzero);
            for(Iz=0;Iz<Nnonzero;Iz++) {
               ScanLine(infile,"%ld %ld %ld %ld %lf",5,&i,&j,&Im,&Jm,&value);
               if (i >= 3 || j >= 3 || Im >= B->Nf || Jm >= B->Nf) {
                  printf("Error in InitFlexModes: N index [%ld][%ld][%ld][%ld] out of range\n",i,j,Im,Jm);
                  exit(1);
               }
               N[i][j][Im][Jm] = value;
            }
            for(i=0;i<B->Nf;i++) {
               for(j=0;j<B->Nf;j++) {
                  /* B->Qf, 3 x Nf x Nf */
                  B->Qf[IDX3(0,i,j,B->Nf,B->Nf)] = N[2][1][i][j]-N[1][2][i][j];
                  B->Qf[IDX3(1,i,j,B->Nf,B->Nf)] = N[0][2][i][j]-N[2][0][i][j];
                  B->Qf[IDX3(2,i,j,B->Nf,B->Nf)] = N[1][0][i][j]-N[0][1][i][j];
                  /* B->Sf, 3 x Nf x Nf x 3 */
                  B->Sf[IDX4(0,i,j,0,B->Nf,B->Nf,3)] = -N[1][1][i][j]-N[2][2][i][j];
                  B->Sf[IDX4(1,i,j,1,B->Nf,B->Nf,3)] = -N[2][2][i][j]-N[0][0][i][j];
                  B->Sf[IDX4(2,i,j,2,B->Nf,B->Nf,3)] = -N[0][0][i][j]-N[1][1][i][j];
                  B->Sf[IDX4(0,i,j,1,B->Nf,B->Nf,3)] = N[1][0][i][j];
                  B->Sf[IDX4(0,i,j,2,B->Nf,B->Nf,3)] = N[2][0][i][j];
                  B->Sf[IDX4(1,i,j,0,B->Nf,B->Nf,3)] = N[0][1][i][j];
                  B->Sf[IDX4(1,i,j,2,B->Nf,B->Nf,3)] = N[2][1][i][j];
                  B->Sf[IDX4(2,i,j,0,B->Nf,B->Nf,3)] = N[0][2][i][j];
                  B->Sf[IDX4(2,i,j,1,B->Nf,B->Nf,3)] = N[1][2][i][j];
               }
            }
            for(i=0;i<3;i++) {
               for(j=0;j<3;j++) DestroyMatrix(N[i][j],B->Nf);
               free(N[i]);
            }
            free(N);

            fclose(infile);
         }
      }

/* .. Assemble Dyn.Mf */
      for(Ib=0;Ib<S->Nb;Ib++) {
         B = &S->B[Ib];
         for(i=0;i<B->Nf;i++) {
            for(j=0;j<B->Nf;j++) {
               D->Mf[B->f0+i][B->f0+j] = B->Mf[i][j];
            }
         }
      }

/* .. Find if Mf is Diagonal */
      for(Ib=0;Ib<S->Nb;Ib++) {
         B = &S->B[Ib];
         B->MfIsDiagonal = TRUE;
         for(i=0;i<B->Nf;i++) {
            for(j=0;j<B->Nf;j++) {
               if (i != j && B->Mf[i][j] != 0.0)
                  B->MfIsDiagonal = FALSE;
            }
         }
      }

/* .. Bypass flex computations if FlexActive is FALSE */
      if (!S->FlexActive) D->Nf = 0;
/* .. If FlexActive is TRUE, but no flex modes have been defined... */
      if (S->FlexActive && D->Nf == 0) {
         S->FlexActive = FALSE;
         printf("FlexActive set TRUE, but no flex modes defined.\n");
         printf("   Setting FlexActive to FALSE.\n");
      }
}
/**********************************************************************/
void InitNodes(struct BodyType *B)
{
      struct NodeType *N;
      FILE *infile;
      long In,i;
      
      if (strcmp(B->NodeFileName,"NONE")) {
         infile = FileOpen(InOutPath,B->NodeFileName,"r");
         ScanLine(infile,"",0,NULL);
         ScanLine(infile,"",0,NULL);
         ScanLine(infile,"%ld",1,&B->NumNodes);
         B->Node = (struct NodeType *) calloc(B->NumNodes,sizeof(struct NodeType));
         ScanLine(infile,"",0,NULL);
         for(In=0;In<B->NumNodes;In++) {
            N = &B->Node[In];
            ScanLine(infile,"%lf %lf %lf \"%80[^\"]\"",4,
               &N->NomPosB[0],&N->NomPosB[1],&N->NomPosB[2],N->comment);
         }
         fclose(infile);
      }
      else {
         /* Default to one node at B.cm */
         B->NumNodes = 1;
         B->Node = (struct NodeType *) calloc(1,sizeof(struct NodeType));
         for(i=0;i<3;i++) B->Node[0].PosB[i] = B->cm[i];
         strcpy(B->Node[0].comment,"Mass Center");
      }
}
/**********************************************************************/
void InitPassiveJoint(struct JointType *G, struct SCType *S)
{
      FILE *infile;
      long i;
      
      for(i=0;i<3;i++) {
         G->RotSpringCoef[i] = 0.0;
         G->RotDampCoef[i] = 0.0;
         G->TrnSpringCoef[i] = 0.0;
         G->TrnDampCoef[i] = 0.0;
      }
      if (strcmp(G->ParmFileName,"NONE")) {
         infile = FileOpen(InOutPath,G->ParmFileName,"r");
         ScanLine(infile,"",0,NULL);
         ScanLine(infile,"",0,NULL);
         ScanLine(infile,"",0,NULL);
         ScanLine(infile,"%lf %lf %lf",3,
                &G->RotSpringCoef[0],
                &G->RotSpringCoef[1],
                &G->RotSpringCoef[2]);
         ScanLine(infile,"%lf %lf %lf",3,
                &G->RotDampCoef[0],
                &G->RotDampCoef[1],
                &G->RotDampCoef[2]);
         ScanLine(infile,"%lf %lf %lf",3,
                &G->TrnSpringCoef[0],
                &G->TrnSpringCoef[1],
                &G->TrnSpringCoef[2]);
         ScanLine(infile,"%lf %lf %lf",3,
                &G->TrnDampCoef[0],
                &G->TrnDampCoef[1],
                &G->TrnDampCoef[2]);
         fclose(infile);
      }
}
/**********************************************************************/
void InitActuatedJoint(struct JointType *G, struct SCType *S)
{
      long i;
      
      for(i=0;i<3;i++) {
         G->MaxTrq[i] = 500.0;
         G->MaxAngRate[i] = 60.0*D2R;
         G->AngRateGain[i] = 120.0; //G->MaxTrq[i]/G->MaxAngRate[i];
         G->MaxFrc[i] = 10.0;
         G->MaxPosRate[i] = 0.01;
         G->PosRateGain[i] = G->MaxFrc[i]/G->MaxPosRate[i];
      }
}
/**********************************************************************/
void InitShakers(struct SCType *S)
{
      FILE *infile;
      long Ish,It;
      struct ShakerType *Sh;
      char response[120];
      
      if (strcmp(S->ShakerFileName,"NONE")) {
      
         infile = FileOpen(InOutPath,S->ShakerFileName,"r");

         ScanLine(infile,"",0,NULL);
         ScanLine(infile,"%ld",1,&S->Nsh);
         ScanLine(infile,"",0,NULL);
         
         S->Shaker = (struct ShakerType *) calloc(S->Nsh,sizeof(struct ShakerType)); 
         for(Ish=0;Ish<S->Nsh;Ish++) {
            Sh = &S->Shaker[Ish];
            ScanLine(infile,"",0,NULL);
            ScanLine(infile,"",0,NULL);
            ScanLine(infile,"%ld %ld",2,&Sh->Body,&Sh->Node);
            ScanLine(infile,"%120s",1,response); 
            Sh->FrcTrq = DecodeString(response);
            ScanLine(infile,"%lf %lf %lf",3,
               &Sh->Axis[0],&Sh->Axis[1],&Sh->Axis[2]);
            UNITV(Sh->Axis);   
            ScanLine(infile,"%ld",1,&Sh->Ntone);
            if (Sh->Ntone==0) {
               ScanLine(infile,"",0,NULL);
               ScanLine(infile,"",0,NULL);
               ScanLine(infile,"",0,NULL);
            }
            else {
               Sh->ToneAmp = (double *) calloc(Sh->Ntone,sizeof(double));
               Sh->ToneFreq = (double *) calloc(Sh->Ntone,sizeof(double));
               Sh->TonePhase = (double *) calloc(Sh->Ntone,sizeof(double));
               for(It=0;It<Sh->Ntone;It++) {
                  ScanLine(infile,"%lf",1,&Sh->ToneAmp[It]);
                  ScanLine(infile,"%lf",1,&Sh->ToneFreq[It]);
                  ScanLine(infile,"%lf",1,&Sh->TonePhase[It]);
                  Sh->ToneFreq[It] *= TwoPi;
                  Sh->TonePhase[It] *= D2R;
               }
            }
            ScanLine(infile,"%120s",1,response); 
            Sh->RandomActive = DecodeString(response);
            if (!Sh->RandomActive) {
               ScanLine(infile,"",0,NULL);
               ScanLine(infile,"",0,NULL);
               Sh->RandomProc = NULL;
               Sh->Lowpass = NULL;
               Sh->Highpass = NULL;
            }
            else {
               ScanLine(infile,"%lf %lf",2,
                  &Sh->LowBandLimit,&Sh->HighBandLimit);
               ScanLine(infile,"%lf",1,&Sh->RandStd);
               Sh->RandStd /= Sh->HighBandLimit-Sh->LowBandLimit;
               Sh->HighBandLimit *= TwoPi;
               Sh->LowBandLimit *= TwoPi;
         
               /* Lowpass and Highpass overlap to form Bandpass */
               Sh->RandomProc = CreateRandomProcess(RngSeed+Ish);
               Sh->Lowpass = CreateSecondOrderLowpassFilter(Sh->HighBandLimit,1.0,
                  DTSIM,1.0E6,1.0E-12);
               if (Sh->LowBandLimit > 0.0) 
                  Sh->Highpass = CreateSecondOrderLowpassFilter(Sh->LowBandLimit,1.0,
                     DTSIM,1.0E6,1.0E-12);
            }
         }
         fclose(infile);
      }
}
/**********************************************************************/
void InitWhlDragAndJitter(struct WhlType *W)
{
      FILE *infile;
      struct WhlHarmType *H;
      double Stiction,LugrePeriod,w;
      long Ih;
      
      if (strcmp(W->DragJitterFileName,"NONE")) {
         infile = FileOpen(InOutPath,W->DragJitterFileName,"r");
         ScanLine(infile,"",0,NULL);
         /* Drag Parameters */
         ScanLine(infile,"",0,NULL);
         ScanLine(infile,"",0,NULL);
         ScanLine(infile,"",0,NULL);
         ScanLine(infile,"%lf",1,&W->CoulCoef);
         ScanLine(infile,"%lf",1,&Stiction);
         W->StribeckCoef = Stiction - W->CoulCoef;
         if (W->StribeckCoef < 0.0) {
            printf("Error: Stiction < Coulomb friction in %s.  Better fix that.\n",
               W->DragJitterFileName);
            exit(1);
         }
         ScanLine(infile,"%lf",1,&W->ViscCoef);
         ScanLine(infile,"%lf",1,&W->StribeckZone);
         ScanLine(infile,"%lf",1,&LugrePeriod);
         w = TwoPi/LugrePeriod;
         W->LugreSpringCoef = W->J*w*w;
         W->LugreDampCoef = 2.0*W->J*w; /* Critical damping assumed */
         ScanLine(infile,"%lf",1,&W->LugreDampZone);
         W->z = 0.0;
         
         /* Jitter Parameters */
         ScanLine(infile,"",0,NULL);
         ScanLine(infile,"",0,NULL);
         ScanLine(infile,"",0,NULL);
         ScanLine(infile,"%lf",1,&W->gamma);
         W->Jt = 0.5*W->gamma*W->J;
         ScanLine(infile,"%lf",1,&W->ImbPhase);
         W->ImbPhase *= D2R;
         ScanLine(infile,"%lf %lf",2,&W->LatFreq,&W->LatDamp);
         W->LatFreq *= TwoPi;
         ScanLine(infile,"%lf %lf",2,&W->RockFreq,&W->RockDamp);
         W->RockFreq *= TwoPi;
         ScanLine(infile,"",0,NULL);
         ScanLine(infile,"%ld",1,&W->NumHarm);
         if (W->NumHarm > 0) {
            W->Harm = (struct WhlHarmType *) calloc(W->NumHarm,sizeof(struct WhlHarmType));
            for(Ih=0;Ih<W->NumHarm;Ih++) {
               H = &W->Harm[Ih];
               ScanLine(infile,"",0,NULL);
               ScanLine(infile,"%lf",1,&H->n);
               ScanLine(infile,"%lf",1,&H->Ks);
               H->Ks *= 1.0E-3*0.01;
               ScanLine(infile,"%lf",1,&H->Kd);
               H->Kd *= 1.0E-3*1.0E-4;
               ScanLine(infile,"%lf",1,&H->phase);
               H->phase *= D2R;
            }
         }
         fclose(infile);
      }
}
/**********************************************************************/
void InitOptics(struct FgsType *F)
{
#if 0
      FILE *infile;
      struct SCType *S;
      struct BodyType *B;
      struct NodeType *N;
      struct OpticsType *O;
      struct OpticsType *Ap,*Det;
      char response[120];
      double ApPntN[3],FocPntN[3],DetPntN[3],RelPosN[3];
      long Io,i;
      long HasFocus;

      if (strcmp(F->OpticsFileName,"NONE")) {
         infile = FileOpen(InOutPath,F->OpticsFileName,"r");
         ScanLine(infile,"",0,NULL);
         ScanLine(infile,"",0,NULL);
         ScanLine(infile,"",0,NULL);
         ScanLine(infile,"%ld",1,&F->Nopt);
         F->Opt = (struct OpticsType *) calloc(F->Nopt,sizeof(struct OpticsType));
         for(Io=0;Io<F->Nopt;Io++) {
            O = &F->Opt[Io];
            ScanLine(infile,"",0,NULL);
            ScanLine(infile,"%ld %ld %ld",3,&O->SC,&O->Body,&O->Node);
            ScanLine(infile,"%lf",1,&O->ApRad);
            O->ApRad /= 2.0;
            ScanLine(infile,"%lf %lf %lf",3,
               &O->Axis[0],&O->Axis[1],&O->Axis[2]);
            UNITV(O->Axis);
            ScanLine(infile,"%120s",1,response);
            O->Type = DecodeString(response);
            ScanLine(infile,"%120s",1,response);
            O->ConicSign = (double) DecodeString(response);
            ScanLine(infile,"%lf",1,&O->FocLen);
            ScanLine(infile,"%lf",1,&O->ConicConst);
         }
         fclose(infile);
         F->HasOptics = TRUE;

         /* Check for Aperture, Detector */
         if (F->Opt[0].Type != OPT_APERTURE) {
            printf("Optical Train must have Aperture as first element.\n");
            exit(1);
         }
         if (F->Opt[F->Nopt-1].Type != OPT_DETECTOR) {
            printf("Optical Train must have Detector as last element.\n");
         }
         
         /* TODO: Find erect/inverted, lump in Det->FocLen */
      }
      else {
         F->HasOptics = FALSE;
      }
      
      printf("Exiting InitOptics\n");
      
#endif
}
/**********************************************************************/
void InitOrderNDynamics(struct SCType *S)
{
      struct BodyType *B;
      struct JointType *G;
      long Ib,Ig,Id,i,j;
      
      G = &S->GN;
      G->Init = 1;
      G->IsSpherical = 1;
      G->RotDOF = 3;
      G->TrnDOF = 3;
      G->Bo = &S->B[0];
      G->RotSeq = 123;
      G->TrnSeq = 123;
      for(i=0;i<3;i++) {
         for(j=0;j<3;j++) {
            G->CGiBi[i][j] = 0.0;
            G->CBoGo[i][j] = 0.0;
            G->Pw[i][j] = 0.0;
            G->Pv[i][j] = 0.0;
            G->Pwdot[i][j] = 0.0;
            G->P[i][j] = 0.0;
            G->P[i][3+j] = 0.0;
            G->P[3+i][j] = 0.0;
            G->P[3+i][3+i] = 0.0;
         }
         G->CGiBi[i][i] = 1.0;
         G->CBoGo[i][i] = 1.0;
         G->Pw[i][i] = 1.0;
         G->Pv[i][i] = 1.0;
         G->P[i][i] = 1.0;
         G->P[3+i][3+i] = 1.0;
      }
      G->Nu = 6;
      
      for(Ib=0;Ib<S->Nb;Ib++) {
         B = &S->B[Ib];
         B->Nd = 0;
      }

      for(Ig=0;Ig<S->Ng;Ig++) {
         G = &S->G[Ig];
         G->Init = 1;
         G->Bi = &S->B[G->Bin];
         G->Bo = &S->B[G->Bout];
         G->Bi->Nd++;
         G->Nu = G->RotDOF + G->TrnDOF;
      }
      
      for(Ib=0;Ib<S->Nb;Ib++) {
         B = &S->B[Ib];
         if (B->Nd > 0) {
            B->Gd = (long *) calloc(B->Nd,sizeof(long));
         }
         Id = 0;
         for(Ig=0;Ig<S->Ng;Ig++) {
            G = &S->G[Ig];
            if (G->Bin == Ib) {
               B->Gd[Id] = Ig;
               Id++;
            }
         }
      }
}
/**********************************************************************/
void InitSpacecraft(struct SCType *S)
{
      FILE *infile;
      char response[120];
      char response1[120],response2[120],response3[120];
      double CBL[3][3],CBF[3][3];
      long i,j,k,Ia,Ib,Ig,Iw,Im,It,Bi,Bo,Ic,Ist,Ifss,Ifgs;
      char RateFrame,AttFrame,AttParm;
      double wlnb[3];
      double wbn[3],CBN[3][3],qbn[4];
      double CEN[3][3],CBE[3][3];
      double Ang1,Ang2,Ang3;
      double pIn[3],pOut[3];
      double psn[3],vsn[3],psl[3],vsl[3],pfl[3],pcmn[3],pcml[3];
      double PosVec[3],VelVec[3],wxr[3],wxrn[3],wxrl[3];
      double rh[3],vh[3];
      double BiasTime;
      long Seq;
      long i1,i2,i3;
      long UseCM;
      long SomeJointsLocked;
      struct JointType *G;
      struct BodyType *B;
      struct WhlType *W;
      struct OrbitType *O;
      struct FormationType *Fr;
      struct DynType *D;
      struct GyroType *Gyro;
      struct MagnetometerType *MAG;
      struct CssType *CSS;
      struct FssType *FSS;
      struct StarTrackerType *ST;
      struct GpsType *GPS;
      struct AccelType *Accel;
      struct FgsType *Fgs;
      struct PsfType *PSF;
      long OldNmesh;

      infile=FileOpen(InOutPath,S->FileName,"r");
      ScanLine(infile,"",0,NULL);
      ScanLine(infile,"",0,NULL);
      ScanLine(infile,"\"%40[^\"]\"",1,S->Label);
      ScanLine(infile,"%40s",1,S->SpriteFileName);
      ScanLine(infile,"%120s",1,response);
      S->FswTag = DecodeString(response);
      ScanLine(infile,"%lf",1,&S->FswSampleTime);
      S->FswMaxCounter = (long) (S->FswSampleTime/DTSIM+0.5);
      if (S->FswSampleTime < DTSIM) {
         printf("Error:  FswSampleTime smaller than DTSIM.\n");
         exit(1);
      }
      S->FswSampleCounter = S->FswMaxCounter;
      S->InitAC = 1;

/* .. Orbit Parameters */
      ScanLine(infile,"%120s",1,response);
      if (response[0] != '*') {
         printf("Error:  Malformed SC input file before Orbit Parameter section\n.");
         exit(1);
      }
      ScanLine(infile,"%120s",1,response);
      S->OrbDOF=DecodeString(response);
      ScanLine(infile,"%120s",1,response);
      UseCM=DecodeString(response);
      ScanLine(infile,"%lf %lf %lf",3,&PosVec[0],&PosVec[1],&PosVec[2]);
      ScanLine(infile,"%lf %lf %lf",3,&VelVec[0],&VelVec[1],&VelVec[2]);

/* .. Initial Attitude */
      ScanLine(infile,"%120s",1,response);
      if (response[0] != '*') {
         printf("Error:  Malformed SC input file before Initial Attitude section\n.");
         exit(1);
      }
      ScanLine(infile,"%120s",1,response);
      RateFrame=response[0];
      AttParm=response[1];
      AttFrame=response[2];
      ScanLine(infile,"%lf %lf %lf",3,&wbn[0],&wbn[1],&wbn[2]);
      wbn[0] *= D2R;
      wbn[1] *= D2R;
      wbn[2] *= D2R;
      if (AttParm == 'Q') {
         ScanLine(infile,"%lf %lf %lf %lf",4,&qbn[0],&qbn[1],&qbn[2],&qbn[3]);
         ScanLine(infile,"",0,NULL);
         Q2C(qbn,CBN);
      }
      else {
         ScanLine(infile,"",0,NULL);
         ScanLine(infile,"%lf %lf %lf %ld",4,&Ang1,&Ang2,&Ang3,&Seq);
         A2C(Seq,Ang1*D2R,Ang2*D2R,Ang3*D2R,CBN);
         C2Q(CBN,qbn);
      }
      if(AttFrame == 'L') {
         /* Adjust CBN */
         for(j=0;j<3;j++){
            for(k=0;k<3;k++) CBL[j][k] = CBN[j][k];
         }
         MxM(CBL,Orb[S->RefOrb].CLN,CBN);
         C2Q(CBN,qbn);
      }
      else if (AttFrame == 'F') {
         /* Adjust CBN */
         for(j=0;j<3;j++){
            for(k=0;k<3;k++) CBF[j][k] = CBN[j][k];
         }
         MxM(CBF,Frm[S->RefOrb].CN,CBN);
         C2Q(CBN,qbn);
      }
      else if (AttFrame == 'E') {
         /* Adjust CBN */
         for(j=0;j<3;j++){
            for(k=0;k<3;k++) CBE[j][k] = CBN[j][k];
         }
         FindCEN(Orb[S->RefOrb].PosN,CEN);
         MxM(CBE,CEN,CBN);
         C2Q(CBN,qbn);
      }
      if(RateFrame == 'L') {
         /* Add LVLH rate to wn */
         MxV(CBN,Orb[S->RefOrb].wln,wlnb);
         for(j=0;j<3;j++) wbn[j] += wlnb[j];
      }

      MxMT(CBN,Frm[S->RefOrb].CN,S->CF);

/* .. Dynamics Flags */
      ScanLine(infile,"%120s",1,response);
      if (response[0] != '*') {
         printf("Error:  Malformed SC input file before Dynamics Flags section\n.");
         exit(1);
      }
      ScanLine(infile,"%120s",1,response);
      S->DynMethod=DecodeString(response);
      ScanLine(infile,"%120s",1,response);
      S->ConstraintsRequested=DecodeString(response);
      ScanLine(infile,"%120s",1,response);
      S->RefPt=DecodeString(response);
      ScanLine(infile,"%120s",1,response);
      S->FlexActive=DecodeString(response);
      ScanLine(infile,"%120s",1,response);
      S->IncludeSecondOrderFlexTerms=DecodeString(response);
      ScanLine(infile,"%40s",1,S->ShakerFileName);
      ScanLine(infile,"%lf",1,&S->DragCoef);

/* .. Body parameters */
      ScanLine(infile,"",0,NULL);
      ScanLine(infile,"",0,NULL);
      ScanLine(infile,"%120s",1,response);
      if (response[0] != '*') {
         printf("Error:  Malformed SC input file before Body Parameters section\n.");
         exit(1);
      }
      ScanLine(infile,"%ld",1,&S->Nb);
      S->Ng = S->Nb-1;
      S->B = (struct BodyType *) calloc(S->Nb,sizeof(struct BodyType));
      if (S->B == NULL) {
         printf("S->B calloc returned null pointer.  Bailing out!\n");
         exit(1);
      }
      S->G = (struct JointType *) calloc(S->Ng,sizeof(struct JointType));
      if (S->G == NULL) {
         printf("S->G calloc returned null pointer.  Bailing out!\n");
         exit(1);
      }

      /* Load B[0] initial attitude */
      for(j=0;j<3;j++) {
         S->B[0].wn[j] = wbn[j];
         for(k=0;k<3;k++) S->B[0].CN[j][k] = CBN[j][k];
      }
      for(j=0;j<4;j++) S->B[0].qn[j] = qbn[j];

/* .. Body Ib */
      for(Ib=0;Ib<S->Nb;Ib++) {
         B = &S->B[Ib];
         ScanLine(infile,"",0,NULL);
         ScanLine(infile,"%lf",1,&B->mass);
         ScanLine(infile,"%lf %lf %lf",3,
            &B->I[0][0],&B->I[1][1],&B->I[2][2]);
         ScanLine(infile,"%lf %lf %lf",3,
            &B->I[0][1],&B->I[0][2],&B->I[1][2]);
         B->I[0][1] *= -1.0;
         B->I[0][2] *= -1.0;
         B->I[1][2] *= -1.0;
         B->I[1][0] = B->I[0][1];
         B->I[2][0] = B->I[0][2];
         B->I[2][1] = B->I[1][2];
         if (!InertiasArePhysical(B->I)) {
            printf("SC[%ld].B[%ld] inertias are not physically realizable.\n",
               S->ID,Ib);
         }
         ScanLine(infile,"%lf %lf %lf",3,&B->cm[0],&B->cm[1],&B->cm[2]);
         ScanLine(infile,"%lf %lf %lf",3,
            &B->EmbeddedMom[0],&B->EmbeddedMom[1],&B->EmbeddedMom[2]);
         ScanLine(infile,"%lf %lf %lf",3,
            &B->EmbeddedDipole[0],&B->EmbeddedDipole[1],&B->EmbeddedDipole[2]);
         ScanLine(infile,"%40s",1,B->MeshFileName);
         ScanLine(infile,"%40s",1,B->NodeFileName);
         ScanLine(infile,"%40s",1,B->FlexFileName);
         if (S->RefPt == REFPT_JOINT)
            for(i=0;i<3;i++) B->c[i] = B->mass*B->cm[i];
         else
            for(i=0;i<3;i++) B->c[i] = 0.0;
         
         InitNodes(B);
      }

/* .. Joint Parameters */
      SomeJointsLocked = FALSE;
      ScanLine(infile,"",0,NULL);
      ScanLine(infile,"",0,NULL);
      ScanLine(infile,"%120s",1,response);
      if (response[0] != '*') {
         printf("Error:  Malformed SC input file before Joint Parameters section\n.");
         exit(1);
      }
      ScanLine(infile,"",0,NULL);
      if (S->Ng == 0) {  /* Read and discard template */
         for(i=0;i<16;i++) ScanLine(infile,"",0,NULL);
      }
      else {
         for(Ig=0;Ig<S->Ng;Ig++) {
            G = &S->G[Ig];
            ScanLine(infile,"",0,NULL);
            ScanLine(infile,"%120s",1,response);
            G->Type = DecodeString(response);
            ScanLine(infile,"%ld %ld",2,&G->Bin,&G->Bout);
            if (G->Bin > G->Bout) {
               printf("Yo!  SC[%ld].G[%ld] inner body index (%ld) is greater than outer body index (%ld)\n",S->ID,Ig,G->Bin,G->Bout);
               printf("You must define inner bodies before outer bodies!\n");
               exit(1);
            }
            Bi=G->Bin;
            Bo=G->Bout;
            S->B[Bo].Gin = Ig;

            ScanLine(infile,"%ld %ld %120s",3,&G->RotDOF,&G->RotSeq,response);
            G->IsSpherical = DecodeString(response);
            if (G->IsSpherical && G->RotDOF < 3) {
               printf("SC[%ld].G[%ld].RotDOF = %ld is inconsistent with SPHERICAL option.  Reconcile and try again.\n",S->ID,Ig,G->RotDOF);
               exit(1);
            }
            if (G->RotSeq < 100) {
               printf("Invalid RotSeq %ld for SC[%ld].G[%ld].  All three axes required.\n",
                  G->RotSeq,S->ID,Ig);
               exit(1);
            }
            i3 = G->RotSeq % 10;         /* Pick off third digit */
            i2 = (G->RotSeq % 100)/10;   /* Extract second digit */
            i1 = G->RotSeq/100;          /* Pick off first digit */
            if (i1 == i2 || i1 == i3 || i2 == i3) {
               printf("Invalid RotSeq %ld for SC[%ld].G[%ld].  Repeated indices are not allowed.\n",
                  G->RotSeq,S->ID,Ig);
               exit(1);
            }

            ScanLine(infile,"%ld %ld",2,&G->TrnDOF,&G->TrnSeq);
            if (G->TrnSeq < 100) {
               printf("Invalid TrnSeq %ld for SC[%ld].G[%ld].  All three axes required.\n",
                  G->TrnSeq,S->ID,Ig);
               exit(1);
            }
            i3 = G->TrnSeq % 10;         /* Pick off third digit */
            i2 = (G->TrnSeq % 100)/10;   /* Extract second digit */
            i1 = G->TrnSeq/100;          /* Pick off first digit */
            if (i1 == i2 || i1 == i3 || i2 == i3) {
               printf("Invalid TrnSeq %ld for SC[%ld].G[%ld].  Repeated indices are not allowed.\n",
                  G->TrnSeq,S->ID,Ig);
               exit(1);
            }
            ScanLine(infile,"%120s %120s %120s",3,
               response1,response2,response3);
            G->RotLocked[0] = DecodeString(response1);
            G->RotLocked[1] = DecodeString(response2);
            G->RotLocked[2] = DecodeString(response3);
            ScanLine(infile,"%120s %120s %120s",3,
               response1,response2,response3);
            G->TrnLocked[0] = DecodeString(response1);
            G->TrnLocked[1] = DecodeString(response2);
            G->TrnLocked[2] = DecodeString(response3);
            
            for(i=0;i<3;i++) {
               if (G->RotLocked[i] || G->TrnLocked[i]) SomeJointsLocked = TRUE;
            }

            /* Load in initial angles and angular rates */
            ScanLine(infile,"%lf %lf %lf",3,&G->Ang[0],&G->Ang[1],&G->Ang[2]);
            ScanLine(infile,"%lf %lf %lf",3,
               &G->AngRate[0],&G->AngRate[1],&G->AngRate[2]);
            for(k=0;k<3;k++) {
               G->Ang[k] *= D2R;
               G->AngRate[k] *= D2R;
            }
            /* Protect against more inputs than RotDOF */
            for(k=G->RotDOF;k<3;k++) {
               G->Ang[k] = 0.0;
               G->AngRate[k] = 0.0;
            }
            /* Load in initial displacements and rates */
            ScanLine(infile,"%lf %lf %lf",3,&G->Pos[0],&G->Pos[1],&G->Pos[2]);
            ScanLine(infile,"%lf %lf %lf",3,
                  &G->PosRate[0],&G->PosRate[1],&G->PosRate[2]);
            /* Protect against more inputs than TrnDOF */
            for(k=G->TrnDOF;k<3;k++) {
               G->Pos[k] = 0.0;
               G->PosRate[k] = 0.0;
            }

            ScanLine(infile,"%lf %lf %lf %ld",4,&Ang1,&Ang2,&Ang3,&Seq);
            A2C(Seq,Ang1*D2R,Ang2*D2R,Ang3*D2R,G->CGiBi);
            ScanLine(infile,"%lf %lf %lf %ld",4,&Ang1,&Ang2,&Ang3,&Seq);
            A2C(Seq,Ang1*D2R,Ang2*D2R,Ang3*D2R,G->CBoGo);

            ScanLine(infile,"%lf %lf %lf",3,&pIn[0],&pIn[1],&pIn[2]);
            ScanLine(infile,"%lf %lf %lf",3,&pOut[0],&pOut[1],&pOut[2]);

            if (S->RefPt == REFPT_JOINT) {
               for(j=0;j<3;j++) {
                  G->RigidRin[j] = pIn[j];
                  G->RigidRout[j] = pOut[j];
               }
            }
            else {
               for(j=0;j<3;j++) {
                  G->RigidRin[j] = pIn[j] - S->B[Bi].cm[j];
                  G->RigidRout[j] = pOut[j] - S->B[Bo].cm[j];
               }
            }
            ScanLine(infile,"%40s",1,G->ParmFileName);

            if (G->Type == PASSIVE_JOINT) InitPassiveJoint(G,S);
            if (G->Type == ACTUATED_JOINT) InitActuatedJoint(G,S);
         }
      }

/* .. Wheel parameters */
      ScanLine(infile,"%120s",1,response);
      if (response[0] != '*') {
         printf("Error:  Malformed SC input file before Wheel Parameters section\n.");
         exit(1);
      }
      ScanLine(infile,"%120s",1,response);
      S->WhlDragActive=DecodeString(response);
      ScanLine(infile,"%120s",1,response);
      S->WhlJitterActive=DecodeString(response);
      ScanLine(infile,"%ld",1,&S->Nw);
      S->Whl = (struct WhlType *) calloc(S->Nw,sizeof(struct WhlType));
      if (S->Nw == 0) {
         for(i=0;i<8;i++) ScanLine(infile,"",0,NULL);
      }
      else {
         for(Iw=0;Iw<S->Nw;Iw++) {
            W = &S->Whl[Iw];
            ScanLine(infile,"",0,NULL);
            ScanLine(infile,"%lf",1,&W->H);
            ScanLine(infile,"%lf %lf %lf",3,&W->A[0],&W->A[1],&W->A[2]);
            UNITV(W->A);
            PerpBasis(W->A,W->Uaxis,W->Vaxis);
            ScanLine(infile,"%lf %lf",2,&W->Tmax,&W->Hmax);
            ScanLine(infile,"%lf",1,&W->J);
            ScanLine(infile,"%ld",1,&W->Body);
            ScanLine(infile,"%ld",1,&W->Node);
            if (W->Node >= S->B[W->Body].NumNodes) {
               printf("SC[%ld].Whl[%ld] Node out of range\n",S->ID,Iw);
               exit(1);
            } 
            ScanLine(infile,"%40s",1,W->DragJitterFileName);
            InitWhlDragAndJitter(W);
         }
      }

/* .. MTB parameters */
      ScanLine(infile,"%120s",1,response);
      if (response[0] != '*') {
         printf("Error:  Malformed SC input file before MTB Parameters section\n.");
         exit(1);
      }
      ScanLine(infile,"%ld",1,&S->Nmtb);
      S->MTB = (struct MTBType *) calloc(S->Nmtb,sizeof(struct MTBType));
      if (S->Nmtb == 0) {
         for(i=0;i<4;i++) ScanLine(infile,"",0,NULL);
      }
      else {
         for(Im=0;Im<S->Nmtb;Im++) {
            ScanLine(infile,"",0,NULL);
            ScanLine(infile,"%lf",1,&S->MTB[Im].Mmax);
            ScanLine(infile,"%lf %lf %lf",3,
               &S->MTB[Im].A[0],&S->MTB[Im].A[1],&S->MTB[Im].A[2]);
            UNITV(S->MTB[Im].A);
            ScanLine(infile,"%ld",1,&S->MTB[Im].Node);
            if (S->MTB[Im].Node >= S->B[0].NumNodes) {
               printf("SC[%ld].MTB[%ld] Node out of range\n",S->ID,Im);
               exit(1);
            } 

         }
      }

/* .. Thruster parameters */
      ScanLine(infile,"%120s",1,response);
      if (response[0] != '*') {
         printf("Error:  Malformed SC input file before Thruster Parameters section\n.");
         exit(1);
      }
      ScanLine(infile,"%ld",1,&S->Nthr);
      S->Thr = (struct ThrType *) calloc(S->Nthr,sizeof(struct ThrType));
      if (S->Nthr == 0) {
         for(i=0;i<6;i++) ScanLine(infile,"",0,NULL);
      }
      else {
         for(It=0;It<S->Nthr;It++) {
            ScanLine(infile,"",0,NULL);
            ScanLine(infile,"%120s",1,response);
            S->Thr[It].Mode = DecodeString(response);
            ScanLine(infile,"%lf",1,&S->Thr[It].Fmax);
            ScanLine(infile,"%lf %lf %lf",3,
               &S->Thr[It].A[0],&S->Thr[It].A[1],&S->Thr[It].A[2]);
            UNITV(S->Thr[It].A);
            ScanLine(infile,"%ld",1,&S->Thr[It].Body);
            ScanLine(infile,"%ld",1,&S->Thr[It].Node);
            if (S->Thr[It].Node >= S->B[S->Thr[It].Body].NumNodes) {
               printf("SC[%ld].Thr[%ld] Node out of range\n",S->ID,It);
               exit(1);
            } 

         }
      }

/* .. Gyro Parameters */
      ScanLine(infile,"%120s",1,response);
      if (response[0] != '*') {
         printf("Error:  Malformed SC input file before Gyro section\n.");
         exit(1);
      }
      ScanLine(infile,"%ld",1,&S->Ngyro);
      S->Gyro = (struct GyroType *) calloc(S->Ngyro,sizeof(struct GyroType));
      if (S->Ngyro == 0) {
         for(i=0;i<11;i++) ScanLine(infile,"",0,NULL);
      }
      else {
         for(Ig=0;Ig<S->Ngyro;Ig++) {
            Gyro = &S->Gyro[Ig];
            ScanLine(infile,"",0,NULL);
            ScanLine(infile,"%lf",1,&Gyro->SampleTime);
            Gyro->MaxCounter = (long) (Gyro->SampleTime/DTSIM+0.5);
            if (Gyro->SampleTime < DTSIM) {
               printf("Error:  Gyro[%ld].SampleTime smaller than DTSIM.\n",Ig);
               exit(1);
            }
            Gyro->SampleCounter = Gyro->MaxCounter;
            ScanLine(infile,"%lf %lf %lf",3,
               &Gyro->Axis[0],&Gyro->Axis[1],&Gyro->Axis[2]);
            UNITV(Gyro->Axis);
            ScanLine(infile,"%lf",1,&Gyro->MaxRate);
            Gyro->MaxRate *= D2R;
            ScanLine(infile,"%lf",1,&Gyro->Scale);
            Gyro->Scale = 1.0+1.0E-6*Gyro->Scale;
            ScanLine(infile,"%lf",1,&Gyro->Quant);
            Gyro->Quant *= D2R/3600.0;
            ScanLine(infile,"%lf",1,&Gyro->SigV);
            Gyro->SigV *= D2R/60.0; /* from deg/rt-hr to rad/rt-sec */
            ScanLine(infile,"%lf %lf",2,&Gyro->SigU,&BiasTime);
            Gyro->SigU *= D2R/3600.0/sqrt(BiasTime*3600.0);
            ScanLine(infile,"%lf",1,&Gyro->SigE);
            Gyro->SigE *= D2R/3600.0;
            ScanLine(infile,"%lf",1,&Gyro->Bias);
            Gyro->Bias *= D2R/3600.0;
            ScanLine(infile,"%ld",1,&Gyro->Node);
            if (Gyro->Node >= S->B[0].NumNodes) {
               printf("SC[%ld].Gyro Node out of range\n",S->ID);
               exit(1);
            } 
            
            Gyro->BiasStabCoef = Gyro->SigU*sqrt(Gyro->SampleTime);
            Gyro->ARWCoef = sqrt(Gyro->SigV*Gyro->SigV/Gyro->SampleTime 
                               + Gyro->SigU*Gyro->SigU*Gyro->SampleTime/12.0);
            Gyro->AngNoiseCoef = Gyro->SigE/sqrt(Gyro->SampleTime);
            Gyro->CorrCoef = 1.0-Gyro->SampleTime/(BiasTime*3600.0);
            Gyro->Angle = 0.0;
         }
      }
      
/* .. Magnetometer Parameters */
      ScanLine(infile,"%120s",1,response);
      if (response[0] != '*') {
         printf("Error:  Malformed SC input file before Magnetometer section\n.");
         exit(1);
      }
      ScanLine(infile,"%ld",1,&S->Nmag);
      S->MAG = (struct MagnetometerType *) calloc(S->Nmag,sizeof(struct MagnetometerType));
      if (S->Nmag == 0) {
         for(i=0;i<8;i++) ScanLine(infile,"",0,NULL);
      }
      else {
         for(Im=0;Im<S->Nmag;Im++) {
            MAG = &S->MAG[Im];
            ScanLine(infile,"",0,NULL);
            ScanLine(infile,"%lf",1,&MAG->SampleTime);
            MAG->MaxCounter = (long) (MAG->SampleTime/DTSIM+0.5);
            if (MAG->SampleTime < DTSIM) {
               printf("Error:  MAG[%ld].SampleTime smaller than DTSIM.\n",Im);
               exit(1);
            }
            MAG->SampleCounter = MAG->MaxCounter;
            ScanLine(infile,"%lf %lf %lf",3,
               &MAG->Axis[0],&MAG->Axis[1],&MAG->Axis[2]);
            UNITV(MAG->Axis);
            ScanLine(infile,"%lf",1,&MAG->Saturation);
            ScanLine(infile,"%lf",1,&MAG->Scale);
            MAG->Scale = 1.0+1.0E-6*MAG->Scale;
            ScanLine(infile,"%lf",1,&MAG->Quant);
            ScanLine(infile,"%lf",1,&MAG->Noise);
            ScanLine(infile,"%ld",1,&MAG->Node);
            if (MAG->Node >= S->B[0].NumNodes) {
               printf("SC[%ld].MAG[%ld] Node out of range\n",S->ID,Im);
               exit(1);
            } 
         }
      }
      
/* .. Coarse Sun Sensors */
      ScanLine(infile,"%120s",1,response);
      if (response[0] != '*') {
         printf("Error:  Malformed SC input file before Coarse Sun Sensor section\n.");
         exit(1);
      }
      ScanLine(infile,"%ld",1,&S->Ncss);
      S->CSS = (struct CssType *) calloc(S->Ncss,sizeof(struct CssType));
      if (S->Ncss == 0) {
         for(i=0;i<8;i++) ScanLine(infile,"",0,NULL);
      }
      else {
         for(Ic=0;Ic<S->Ncss;Ic++) {
            CSS = &S->CSS[Ic];
            ScanLine(infile,"",0,NULL);
            ScanLine(infile,"%lf",1,&CSS->SampleTime);
            CSS->MaxCounter = (long) (CSS->SampleTime/DTSIM+0.5);
            if (CSS->SampleTime < DTSIM) {
               printf("Error:  CSS[%ld].SampleTime smaller than DTSIM.\n",Ig);
               exit(1);
            }
            CSS->SampleCounter = CSS->MaxCounter;
            ScanLine(infile,"%lf %lf %lf",3,
               &CSS->Axis[0],&CSS->Axis[1],&CSS->Axis[2]);
            UNITV(CSS->Axis);
            ScanLine(infile,"%lf",1,&CSS->FovHalfAng);
            CSS->FovHalfAng *= D2R;
            CSS->CosFov = cos(CSS->FovHalfAng);
            ScanLine(infile,"%lf",1,&CSS->Scale);
            ScanLine(infile,"%lf",1,&CSS->Quant);
            ScanLine(infile,"%ld",1,&CSS->Body);
            ScanLine(infile,"%ld",1,&CSS->Node);
            if (CSS->Node >= S->B[CSS->Body].NumNodes) {
               printf("SC[%ld].CSS[%ld] Node out of range\n",S->ID,Ic);
               exit(1);
            } 
         }
      }

/* .. Fine Sun Sensors */
      ScanLine(infile,"%120s",1,response);
      if (response[0] != '*') {
         printf("Error:  Malformed SC input file before Fine Sun Sensor section\n.");
         exit(1);
      }
      ScanLine(infile,"%ld",1,&S->Nfss);
      S->FSS = (struct FssType *) calloc(S->Nfss,sizeof(struct FssType));
      if (S->Nfss == 0) {
         for(i=0;i<8;i++) ScanLine(infile,"",0,NULL);
      }
      else {
         for(Ifss=0;Ifss<S->Nfss;Ifss++) {
            FSS = &S->FSS[Ifss];
            ScanLine(infile,"",0,NULL);
            ScanLine(infile,"%lf",1,&FSS->SampleTime);
            FSS->MaxCounter = (long) (FSS->SampleTime/DTSIM+0.5);
            if (FSS->SampleTime < DTSIM) {
               printf("Error:  FSS[%ld].SampleTime smaller than DTSIM.\n",Ig);
               exit(1);
            }
            FSS->SampleCounter = FSS->MaxCounter;
            ScanLine(infile,"%lf %lf %lf %ld",4,&Ang1,&Ang2,&Ang3,&Seq);
            A2C(Seq,Ang1*D2R,Ang2*D2R,Ang3*D2R,FSS->CB);
            C2Q(FSS->CB,FSS->qb);
            ScanLine(infile,"%120s",1,response);
            FSS->BoreAxis = DecodeString(response);
            FSS->H_Axis = (FSS->BoreAxis+1)%3;
            FSS->V_Axis = (FSS->BoreAxis+2)%3;
            ScanLine(infile,"%lf %lf",2,
               &FSS->FovHalfAng[0],&FSS->FovHalfAng[1]);
            for(i=0;i<2;i++) {
               FSS->FovHalfAng[i] *= 0.5*D2R;
            }
            ScanLine(infile,"%lf",1,&FSS->NEA);
            FSS->NEA *= D2R;
            ScanLine(infile,"%lf",1,&FSS->Quant);
            FSS->Quant *= D2R;
            ScanLine(infile,"%ld",1,&FSS->Node);
            if (FSS->Node >= S->B[0].NumNodes) {
               printf("SC[%ld].FSS[%ld] Node out of range\n",S->ID,Ifss);
               exit(1);
            } 
         }
      }
      
/* .. Star Trackers */
      ScanLine(infile,"%120s",1,response);
      if (response[0] != '*') {
         printf("Error:  Malformed SC input file before Star Tracker section\n.");
         exit(1);
      }
      ScanLine(infile,"%ld",1,&S->Nst);
      S->ST = (struct StarTrackerType *) calloc(S->Nst,sizeof(struct StarTrackerType));
      if (S->Nst == 0) {
         for(i=0;i<8;i++) ScanLine(infile,"",0,NULL);
      }
      else {
         for(Ist=0;Ist<S->Nst;Ist++) {
            ST = &S->ST[Ist];
            ScanLine(infile,"",0,NULL);
            ScanLine(infile,"%lf",1,&ST->SampleTime);
            ST->MaxCounter = (long) (ST->SampleTime/DTSIM+0.5);
            if (ST->SampleTime < DTSIM) {
               printf("Error:  ST[%ld].SampleTime smaller than DTSIM.\n",Ig);
               exit(1);
            }
            ST->SampleCounter = ST->MaxCounter;
            ScanLine(infile,"%lf %lf %lf %ld",4,&Ang1,&Ang2,&Ang3,&Seq);
            A2C(Seq,Ang1*D2R,Ang2*D2R,Ang3*D2R,ST->CB);
            C2Q(ST->CB,ST->qb);
            ScanLine(infile,"%120s",1,response);
            ST->BoreAxis = DecodeString(response);
            ST->H_Axis = (ST->BoreAxis+1)%3;
            ST->V_Axis = (ST->BoreAxis+2)%3;
            ScanLine(infile,"%lf %lf",2,
               &ST->FovHalfAng[0],&ST->FovHalfAng[1]);
            for(i=0;i<2;i++) {
               ST->FovHalfAng[i] *= 0.5*D2R;
               ST->CosFov[i] = cos(ST->FovHalfAng[i]);
            }
            ScanLine(infile,"%lf %lf %lf",3,
               &ST->SunExclAng,&ST->EarthExclAng,&ST->MoonExclAng);
            ST->SunExclAng *= D2R;
            ST->EarthExclAng *= D2R;
            ST->MoonExclAng *= D2R;
            ST->CosSunExclAng = cos(ST->SunExclAng);
            ST->CosEarthExclAng = cos(ST->EarthExclAng);
            ST->CosMoonExclAng = cos(ST->MoonExclAng);
            ScanLine(infile,"%lf %lf %lf",3,
               &ST->NEA[0],&ST->NEA[1],&ST->NEA[2]);
            for(i=0;i<3;i++) ST->NEA[i] *= D2R/3600.0;
            ScanLine(infile,"%ld",1,&ST->Node);
            if (ST->Node >= S->B[0].NumNodes) {
               printf("SC[%ld].ST[%ld] Node out of range\n",S->ID,Ist);
               exit(1);
            } 
         }
      }
      
/* .. GPS Sensors */
      ScanLine(infile,"%120s",1,response);
      if (response[0] != '*') {
         printf("Error:  Malformed SC input file before GPS section\n.");
         exit(1);
      }
      ScanLine(infile,"%ld",1,&S->Ngps);
      S->GPS = (struct GpsType *) calloc(S->Ngps,sizeof(struct GpsType));
      if (S->Ngps == 0) {
         for(i=0;i<6;i++) ScanLine(infile,"",0,NULL);
      }
      else {
         for(Ig=0;Ig<S->Ngps;Ig++) {
            GPS = &S->GPS[Ig];
            ScanLine(infile,"",0,NULL);
            ScanLine(infile,"%lf",1,&GPS->SampleTime);
            GPS->MaxCounter = (long) (GPS->SampleTime/DTSIM+0.5);
            if (GPS->SampleTime < DTSIM) {
               printf("Error:  GPS[%ld].SampleTime smaller than DTSIM.\n",Ig);
               exit(1);
            }
            GPS->SampleCounter = GPS->MaxCounter;
            ScanLine(infile,"%lf",1,&GPS->PosNoise);
            ScanLine(infile,"%lf",1,&GPS->VelNoise);
            ScanLine(infile,"%lf",1,&GPS->TimeNoise);
            ScanLine(infile,"%ld",1,&GPS->Node);
            if (GPS->Node >= S->B[0].NumNodes) {
               printf("SC[%ld].GPS[%ld] Node out of range\n",S->ID,Ig);
               exit(1);
            } 
         }
      }
      
/* .. Accelerometers */
      ScanLine(infile,"%120s",1,response);
      if (response[0] != '*') {
         printf("Error:  Malformed SC input file before Accelerometer section\n.");
         exit(1);
      }
      ScanLine(infile,"%ld",1,&S->Nacc);
      S->Accel = (struct AccelType *) calloc(S->Nacc,sizeof(struct AccelType));
      if (S->Nacc == 0) {
         for(i=0;i<11;i++) ScanLine(infile,"",0,NULL);
      }
      else {
         for(Ia=0;Ia<S->Nacc;Ia++) {
            Accel = &S->Accel[Ia];
            ScanLine(infile,"",0,NULL);
            ScanLine(infile,"%lf",1,&Accel->SampleTime);
            Accel->MaxCounter = (long) (Accel->SampleTime/DTSIM+0.5);
            if (Accel->SampleTime < DTSIM) {
               printf("Error:  Accel[%ld].SampleTime smaller than DTSIM.\n",Ia);
               exit(1);
            }
            Accel->SampleCounter = Accel->MaxCounter;
            ScanLine(infile,"%lf %lf %lf",3,
               &Accel->Axis[0],&Accel->Axis[1],&Accel->Axis[2]);
            UNITV(Accel->Axis);
            ScanLine(infile,"%lf",1,&Accel->MaxAcc);
            ScanLine(infile,"%lf",1,&Accel->Scale);
            Accel->Scale = 1.0+1.0E-6*Accel->Scale;
            ScanLine(infile,"%lf",1,&Accel->Quant);
            ScanLine(infile,"%lf",1,&Accel->SigV);
            Accel->SigV /= 60.0; /* from m/s/rt-hr to m/s/rt-sec */
            ScanLine(infile,"%lf %lf",2,&Accel->SigU,&BiasTime);
            Accel->SigU /= sqrt(BiasTime*3600.0);
            ScanLine(infile,"%lf",1,&Accel->SigE);
            ScanLine(infile,"%lf",1,&Accel->Bias);
            ScanLine(infile,"%ld",1,&Accel->Node);
            if (Accel->Node >= S->B[0].NumNodes) {
               printf("SC[%ld].Accel[%ld] Node out of range\n",S->ID,Ia);
               exit(1);
            } 
            Accel->BiasStabCoef = Accel->SigU*sqrt(Accel->SampleTime);
            Accel->DVRWCoef = sqrt(Accel->SigV*Accel->SigV/Accel->SampleTime 
                               + Accel->SigU*Accel->SigU*Accel->SampleTime/12.0);
            Accel->DVNoiseCoef = Accel->SigE/sqrt(Accel->SampleTime);
            Accel->CorrCoef = 1.0-Accel->SampleTime/(BiasTime*3600.0);
            Accel->DV = 0.0;
         }
      }

/* .. Fine Guidance Sensors */
      ScanLine(infile,"%120s",1,response);
      if (response[0] != '*') {
         printf("Error:  Malformed SC input file before Fine Guidance Sensor section\n.");
         exit(1);
      }
      ScanLine(infile,"%ld",1,&S->Nfgs);
      S->Fgs = (struct FgsType *) calloc(S->Nfgs,sizeof(struct FgsType));
      if (S->Nfgs == 0) {
         for(i=0;i<7;i++) ScanLine(infile,"",0,NULL);
      }
      else {
         for(Ifgs=0;Ifgs<S->Nfgs;Ifgs++) {
            Fgs = &S->Fgs[Ifgs];
            ScanLine(infile,"",0,NULL);
            ScanLine(infile,"%lf",1,&Fgs->SampleTime);
            Fgs->MaxCounter = (long) (Fgs->SampleTime/DTSIM+0.5);
            if (Fgs->SampleTime < DTSIM) {
               printf("Error:  Fgs[%ld].SampleTime smaller than DTSIM.\n",Ifgs);
               exit(1);
            }
            Fgs->SampleCounter = Fgs->MaxCounter;
            ScanLine(infile,"%lf %lf %lf %ld",4,&Ang1,&Ang2,&Ang3,&Seq);
            A2C(Seq,Ang1*D2R,Ang2*D2R,Ang3*D2R,Fgs->CB);
            C2Q(Fgs->CB,Fgs->qb);
            ScanLine(infile,"%120s",1,response);
            Fgs->BoreAxis = DecodeString(response);
            Fgs->H_Axis = (Fgs->BoreAxis+1)%3;
            Fgs->V_Axis = (Fgs->BoreAxis+2)%3;
            ScanLine(infile,"%lf %lf",2,
               &Fgs->FovHalfAng[0],&Fgs->FovHalfAng[1]);
            for(i=0;i<2;i++) {
               Fgs->FovHalfAng[i] *= 0.5*A2R;
            }
            ScanLine(infile,"%lf",1,&Fgs->NEA);
            Fgs->NEA *= A2R;
            ScanLine(infile,"%lf",1,&Fgs->Scl);
            Fgs->Scl *= A2R;
            ScanLine(infile,"%ld",1,&Fgs->Body);
            if (Fgs->Body >= S->Nb) {
               printf("SC[%ld].Fgs[%ld] Body out of range\n",S->ID,Ifgs);
               exit(1);
            } 
            ScanLine(infile,"%ld",1,&Fgs->Node);
            if (Fgs->Node >= S->B[Fgs->Body].NumNodes) {
               printf("SC[%ld].Fgs[%ld] Node out of range\n",S->ID,Ifgs);
               exit(1);
            } 
            ScanLine(infile,"%lf %lf %lf %ld",4,&Ang1,&Ang2,&Ang3,&Seq);
            A2C(Seq,Ang1*D2R,Ang2*D2R,Ang3*D2R,Fgs->CR);
            C2Q(Fgs->CR,Fgs->qr);
            ScanLine(infile,"%lf %lf",2,&Ang1,&Ang2);
            Fgs->Hr = Ang1*D2R;
            Fgs->Vr = Ang2*D2R;
            ScanLine(infile,"%40s",1,Fgs->OpticsFileName);
            ScanLine(infile,"%40s",1,Fgs->PsfFileName);
            InitOptics(Fgs);
            if (strcmp(Fgs->PsfFileName,"NONE")) {
               PSF = &Fgs->PSF;
               PSF->Image = PpmToPsf(ModelPath,Fgs->PsfFileName,
                  &PSF->Ncol,&PSF->Nrow,&PSF->BytesPerPixel);
            }
         }
      }
      
/* .. Initialize some Orbit and Formation variables */
      O = &Orb[S->RefOrb];
      Fr = &Frm[S->RefOrb];
      if (UseCM) {
         if (Fr->FixedInFrame == 'L') {
            MTxV(Fr->CL,PosVec,S->PosEH);
            MTxV(Fr->CL,VelVec,S->VelEH);
            if (O->Regime == ORB_ZERO) {
               for(i=0;i<3;i++) {
                  S->PosR[i] = S->PosEH[i];
                  S->VelR[i] = S->VelEH[i];
               }
            }
            else if (O->Regime == ORB_FLIGHT) {
               MTxV(O->CLN,S->PosEH,S->PosR);
               MTxV(O->CLN,S->VelEH,S->VelR);
            }
            else {
               EHRV2RelRV(O->SMA,O->MeanMotion,
                  O->CLN,S->PosEH,S->VelEH,S->PosR,S->VelR);
            }
         }
         else {
            MTxV(Fr->CN,PosVec,S->PosR);
            MTxV(Fr->CN,VelVec,S->VelR);
            if (O->Regime == ORB_ZERO) {
               for(i=0;i<3;i++) {
                  S->PosEH[i] = S->PosR[i];
                  S->VelEH[i] = S->VelR[i];
               }
            }
            else if (O->Regime == ORB_FLIGHT) {
               MxV(O->CLN,S->PosR,S->PosEH);
               MxV(O->CLN,S->VelR,S->VelEH);
            }
            else {
               RelRV2EHRV(O->SMA,MAGV(O->wln),
                  O->CLN,S->PosR,S->VelR,S->PosEH,S->VelEH);
            }
         }
         MTxV(S->B[0].CN,S->cm,pcmn);
         for(j=0;j<3;j++) {
            psn[j] = S->PosR[j] - Fr->PosR[j] - pcmn[j];
         }
         MxV(Fr->CN,psn,S->PosF);
         VxV(S->B[0].wn,S->cm,wxr);
         MTxV(S->B[0].CN,wxr,wxrn);
         for(j=0;j<3;j++) {
            vsn[j] = S->VelR[j] - wxrn[j];
         }
         MxV(Fr->CN,vsn,S->VelF);
      }
      else {
         for(j=0;j<3;j++) {
            S->PosF[j] = PosVec[j];
            S->VelF[j] = VelVec[j];
         }
         MTxV(S->B[0].CN,S->cm,pcmn);
         VxV(S->B[0].wn,S->cm,wxr);
         MTxV(S->B[0].CN,wxr,wxrn);
         if (Fr->FixedInFrame == 'L') {
            MTxV(Fr->CL,S->PosF,psl);
            MTxV(Fr->CL,S->VelF,vsl);
            MxV(O->CLN,Fr->PosR,pfl);
            MxV(O->CLN,pcmn,pcml);
            MxV(O->CLN,wxrn,wxrl);
            for(j=0;j<3;j++) {
               S->PosEH[j] = pcml[j] + psl[j] + pfl[j];
               S->VelEH[j] = wxrl[j] + vsl[j];
            }
            if (O->Regime == ORB_ZERO) {
               for(i=0;i<3;i++) {
                  S->PosR[i] = S->PosEH[i];
                  S->VelR[i] = S->VelEH[i];
               }
            }
            else if (O->Regime == ORB_FLIGHT) {
               MTxV(O->CLN,S->PosEH,S->PosR);
               MTxV(O->CLN,S->VelEH,S->VelR);
            }
            else {
               EHRV2RelRV(O->SMA,MAGV(O->wln),O->CLN,
                  S->PosEH,S->VelEH,S->PosR,S->VelR);
            }
         }
         else {
            MTxV(Fr->CN,S->PosF,psn);
            MTxV(Fr->CN,S->VelF,vsn);
            for(j=0;j<3;j++) {
               S->PosR[j] = pcmn[j] + psn[j] + Fr->PosR[j];
               S->VelR[j] = wxrn[j] + vsn[j];
            }
            if (O->Regime == ORB_ZERO) {
            }
            else if (O->Regime == ORB_FLIGHT) {
            }
            else {
               RelRV2EHRV(O->SMA,MAGV(O->wln),O->CLN,
                  S->PosR,S->VelR,S->PosEH,S->VelEH);
            }
         }
      }
      for(j=0;j<3;j++) {
         S->PosN[j] = O->PosN[j] + S->PosR[j];
         S->VelN[j] = O->VelN[j] + S->VelR[j];
      }
      MTxV(World[O->World].CNH,S->PosN,rh);
      MTxV(World[O->World].CNH,S->VelN,vh);
      for(j=0;j<3;j++) {
         S->PosH[j] = World[O->World].PosH[j] + rh[j];
         S->PosH[j] = World[O->World].VelH[j] + vh[j];
      }

      if (O->Regime == ORB_ZERO) {
         for(i=0;i<3;i++) {
            for(j=0;j<3;j++) S->CLN[i][j] = 0.0;
            S->CLN[i][i] = 1.0;
            S->wln[i] = 0.0;
         }
      }
      else if (O->Regime == ORB_FLIGHT) {
         FindENU(S->PosN,World[O->World].w,S->CLN,S->wln);
      }
      else {
         FindCLN(S->PosN,S->VelN,S->CLN,S->wln);
      }

      fclose(infile);

      if (S->DynMethod == DYN_ORDER_N) {
         if (SomeJointsLocked || S->FlexActive || S->ConstraintsRequested) {
            printf("Order-N dynamics doesn't (yet) support flex modes, constraint computation, or locking joints.\n");
            printf("Switching over to Gaussian Elimination.\n");
            S->DynMethod = DYN_GAUSS_ELIM;
         }
      }

      D = &S->Dyn;
      InitRigidDyn(S);
      InitFlexModes(S);
      InitOrderNDynamics(S);

      D->ActiveState = (double *) calloc(D->Nu+D->Nf,sizeof(double));
      D->ActiveStateIdx = (long *) calloc(D->Nu+D->Nf,sizeof(long));
      D->COEF = CreateMatrix(D->Nu+D->Nf,D->Nu+D->Nf);
      D->RHS = (double *) calloc(D->Nu+D->Nf,sizeof(double));

      MapStateVectorToBodyStates(D->u,D->x,D->h,D->a,D->uf,D->xf,S);
      MotionConstraints(S);
      BodyStatesToNodeStates(S);
      SCMassProps(S);
      FindTotalAngMom(S);
      EchoDyn(S);

/* .. Load geometry */
      for(j=0;j<S->Nb;j++) {
         OldNmesh = Nmesh;
         Mesh = LoadWingsObjFile(ModelPath,S->B[j].MeshFileName,
            &Matl,&Nmatl,Mesh,&Nmesh,&S->B[j].MeshTag,
            AeroShadowsActive || SolPressShadowsActive);
         if (ContactActive && OldNmesh != Nmesh) LoadOctree(&Mesh[Nmesh-1]);
      }

/* .. Initialize Bounding Box */
      memcpy(&S->BBox,&Mesh[S->B[0].MeshTag].BBox,sizeof(struct BoundingBoxType));
      UpdateScBoundingBox(S);

      S->EnvTrq.First = 1;
      
      InitAC(S);

      #if _AC_STANDALONE_
      S->AcIpc.Init = 1;
      S->AcIpc.AllowBlocking = 1;
      #endif
      
      InitShakers(S);
      
/* .. Loop Gain and Delays allow verification of stability margins in the time domain */
      /* Created by commands */
      S->GainAndDelayActive = FALSE;
      S->LoopGain = 1.0;
      S->LoopDelay = 0.0;
      for(i=0;i<3;i++) {
         S->IdealAct[i].FrcDelay = NULL;
         S->IdealAct[i].TrqDelay = NULL;
      }
      for(Iw=0;Iw<S->Nw;Iw++) {
         S->Whl[Iw].Delay = NULL;
      }
      for(Im=0;Im<S->Nmtb;Im++) {
         S->MTB[Im].Delay = NULL;
      }
      for(It=0;It<S->Nthr;It++) {
         S->Thr[It].Delay = NULL;
      }
      
}
/*********************************************************************/
void LoadTdrs(void)
{
      FILE *infile;
      char response[120];
      long i;

/* .. Initialize TDRS */
      infile = FileOpen(InOutPath,"Inp_TDRS.txt","r");
/* .. 42 TDRS Configuration File */
      ScanLine(infile,"",0,NULL);

      for(i=0;i<10;i++) {
         ScanLine(infile,"%120s \"%40[^\"]\"",2,
            response,Tdrs[i].Designation);
         Tdrs[i].Exists = DecodeString(response);
      }

      fclose(infile);
}
/*********************************************************************/
void LoadSun(void)
{
      /* Rumor is, Sun's magfield is highly variable, poorly modeled */
      /* by simple dipole.                                           */
      double DipoleAxis[3] = {0.0,0.0,1.0};
      double SunColor[3] = {1.0,1.0,0.9};
      unsigned char Glyph[14] = {0xc0,0xc0,0x00,0x00,0x18,0x66,0x42,
                                 0x99,0x99,0x42,0x66,0x18,0x00,0x00};
      long i,j;
      struct WorldType *W;

      W = &World[SOL];

      /* Relationships */
      W->Exists = TRUE;
      W->Type = SUN;
      W->Parent = 0;

      W->Nsat = 9;
      W->Sat = (long *) calloc(W->Nsat,sizeof(long));
      if (W->Sat == NULL) {
         printf("W->Sat calloc returned null pointer.  Bailing out!\n");
         exit(1);
      }
      for(i=0;i<W->Nsat;i++) W->Sat[i] = MERCURY+i;

      /* Physical Properties */
      W->mu=1.32715E20;
      W->rad=6.98E8;
      W->w=2.69E-6;
      W->RadOfInfluence = 2.0E13;  /* Beyond Pluto's Orbit */
      W->DipoleMoment = 0.0;
      for(j=0;j<3;j++) {
         W->DipoleAxis[j] = DipoleAxis[j];
         W->DipoleOffset[j] = 0.0;
      }
      W->RingInner = 0.0;
      W->RingOuter = 0.0;

      /* Ephemeris */
      W->eph.World = 0;
      W->eph.mu = W->mu;
      W->eph.SMA=0.0;
      W->eph.ecc=0.0;
      W->eph.inc=0.0;
      W->eph.RAAN=0.0;
      W->eph.ArgP=0.0;
      W->eph.tp=0.0;
      W->eph.anom=0.0;
      W->eph.alpha=0.0;
      W->eph.SLR=0.0;
      W->eph.rmin=0.0;

      /* Graphical Properties */
      W->Atmo.Exists = FALSE;
      W->HasRing = FALSE;
      strcpy(W->Name,"Sun");
      strcpy(W->MapFileName,"NONE");
      strcpy(W->MeshFileName,"NONE");
      strcpy(W->ColTexFileName,"NONE");
      strcpy(W->BumpTexFileName,"NONE");
      for(j=0;j<3;j++) {
         W->Color[j] = (float) SunColor[j];
      }
      W->Color[3] = 1.0;
      for(j=0;j<14;j++) W->Glyph[j] = Glyph[j];

      /* State Variables */
      for(i=0;i<3;i++) {
         W->eph.PosN[i]=0.0;
         W->eph.VelN[i]=0.0;
         for(j=0;j<3;j++) W->CNH[i][j] = 0.0;
         W->CNH[i][i] = 1.0;
         W->qnh[i] = 0.0;
      }
      W->qnh[3] = 1.0;
      QxQT(W->qnh,qjh,W->qnj);
}
/*********************************************************************/
void LoadPlanets(void)
{

      struct OrbitType *Eph;
      double Zaxis[3] = {0.0,0.0,1.0};
      double GMST;
      double C_W_TETE[3][3],C_TEME_TETE[3][3],C_TETE_J2000[3][3];

      char PlanetName[10][20] ={"Sun","Mercury","Venus","Earth","Mars",
                                "Jupiter","Saturn","Uranus","Neptune",
                                "Pluto"};
      char MapFileName[10][20] = {"NONE","Rockball","Venus.ppm","Earth.ppm","Mars.ppm",
         "Jupiter.ppm","Saturn.ppm","Uranus.ppm","Neptune.ppm","Iceball"};
      double Mu[10] = {1.32715E20,2.18E13,3.2485E14,3.986004418E14,
                       4.293E13,1.2761E17,3.792E16,5.788E15,6.8E15,
                       3.2E14};
      double J2[10] = {0.0,0.0,0.0,1.08263E-3,1.96045E-3,0.0,0.0,0.0,0.0,0.0};
      double Rad[10] = {6.98E8,2.42E6,6.1E6,6.378145E6,3.41E6,
                        7.14E7,6.04E7,2.35E7,2.23E7,7.0E6};
      double W[10] = {2.69E-6,1.23E-6,2.94E-7,7.292115E-5,7.0882E-5,
                      1.7659E-4,1.6728E-4,1.631E-4,1.105E-4,0.0};

      double PoleRA[10] = {0.0,281.008,272.758,0.0,317.683,268.057,40.587,257.313,299.333,133.046};
      double PoleDec[10] = {90.0,61.45,67.16,0.0,52.8865,64.496,83.537,-15.175,42.95,-6.145};
      double CNJ[3][3];
      double PriMerAngJ2000[10] = {0.0,329.71,160.26,190.16,176.868,284.95,38.90,203.81,253.18,236.77};
      /* Magnetic Field Dipole Strength, Wb-m */
      double DipoleMoment[10] = {0.0,0.0,0.0,7.943E15,0.0,
                                 0.0,0.0,0.0,0.0,0.0};
      /* Magnetic Field Dipole Axis Unit Vector */
      double DipoleAxis[10][3] = {{0.0,0.0,1.0},
                                  {0.0,0.0,1.0},
                                  {0.0,0.0,1.0},
                                  {-6.53286E-2,0.186549,-0.980271},
                                  {0.0,0.0,1.0},
                                  {0.0,0.0,1.0},
                                  {0.0,0.0,1.0},
                                  {0.0,0.0,1.0},
                                  {0.0,0.0,1.0},
                                  {0.0,0.0,1.0}};
      /* Magnetic Field Dipole Offset from Center, m */
      double DipoleOffset[10][3] = {{0.0,0.0,0.0},
                                    {0.0,0.0,0.0},
                                    {0.0,0.0,0.0},
                                    {-3.74461E5,2.44108E5,-1.58291E5},
                                    {0.0,0.0,0.0},
                                    {0.0,0.0,0.0},
                                    {0.0,0.0,0.0},
                                    {0.0,0.0,0.0},
                                    {0.0,0.0,0.0},
                                    {0.0,0.0,0.0}};
      float Color[11][3] = {{1.0f    , 1.0f     , 0.9f},      /* Sun */
                           {0.400318f, 0.347338f, 0.253973f}, /* Mercury */
                           {0.716824f, 0.676952f, 0.623907f}, /* Venus */
                           {0.212f   , 0.293502f, 0.522072f}, /* Earth */
                           {0.687493f, 0.454481f, 0.365368f}, /* Mars */
                           {0.793131f, 0.627618f, 0.477430f}, /* Jupiter */
                           {0.705187f, 0.677713f, 0.620916f}, /* Saturn */
                           {0.486074f, 0.584573f, 0.769742f}, /* Uranus */
                           {0.187558f, 0.243884f, 0.413025f}, /* Neptune */
                           {0.268063f, 0.268183f, 0.268204f}, /* Pluto */
                           {0.440417f, 0.441343f, 0.441084f}};/* Luna */
      unsigned char Glyph[11][14] = {
{0xc0,0xc0,0x00,0x00,0x18,0x66,0x42,0x99,0x99,0x42,0x66,0x18,0x00,0x00}, /* Sun */
{0xc0,0xc0,0x00,0x10,0x7c,0x10,0x38,0x44,0x82,0x82,0x44,0x38,0x44,0x82}, /* Mercury */
{0xc0,0xc0,0x00,0x10,0x10,0x7c,0x10,0x38,0x44,0x82,0x82,0x44,0x38,0x00}, /* Venus */
{0xc0,0xc0,0x00,0x00,0x38,0x54,0x92,0xfe,0x92,0x54,0x38,0x00,0x00,0x00}, /* Earth */
{0xc0,0xc0,0x00,0x00,0x38,0x44,0x82,0x82,0x82,0x44,0x3c,0x05,0x01,0x07}, /* Mars */
{0xc0,0xc0,0x00,0x04,0x04,0x7f,0x44,0x24,0x14,0x14,0x24,0xc2,0x00,0x00}, /* Jupiter */
{0xc0,0xc0,0x00,0x0c,0x08,0x44,0x42,0x42,0x64,0x58,0x40,0x40,0xe0,0x00}, /* Saturn */
{0xc0,0xc0,0x00,0x38,0x44,0x82,0x92,0x82,0x44,0x38,0x10,0x54,0x38,0x10}, /* Uranus */
{0xc0,0xc0,0x00,0x10,0xfe,0x10,0x38,0x54,0x92,0x92,0xd6,0x92,0x00,0x00}, /* Neptune */
{0xc0,0xc0,0x00,0x00,0xf8,0x80,0x80,0xf0,0x88,0x88,0xf0,0x00,0x00,0x00}, /* Pluto */
{0xc0,0xc0,0x00,0x00,0x18,0x70,0x60,0xe0,0xe0,0x60,0x70,0x18,0x00,0x00}};/* Luna */
      long HasAtmo[11] = {0,0,0,1,1,0,0,0,0,0,0};
      long i,j;

      for(i=MERCURY;i<=PLUTO;i++){
         strcpy(World[i].Name,PlanetName[i]);
         strcpy(World[i].MapFileName,MapFileName[i]);
         strcpy(World[i].ColTexFileName,"NONE");
         strcpy(World[i].BumpTexFileName,"NONE");
         World[i].mu=Mu[i];
         World[i].J2 = J2[i];
         World[i].rad=Rad[i];
         World[i].w=W[i];
         World[i].PriMerAngJ2000 = PriMerAngJ2000[i]*D2R;
         World[i].Parent = SOL;
         World[i].eph.World = SOL;
         World[i].eph.mu = World[SOL].mu;
         World[i].DipoleMoment = DipoleMoment[i];
         for(j=0;j<3;j++) {
            World[i].DipoleAxis[j] = DipoleAxis[i][j];
            World[i].DipoleOffset[j] = DipoleOffset[i][j];
            World[i].Color[j] = Color[i][j];
         }
         World[i].Color[3] = 1.0;
         for(j=0;j<14;j++) World[i].Glyph[j] = Glyph[i][j];
         World[i].Atmo.Exists = HasAtmo[i];
      }

      World[EARTH].Atmo.GasColor[0] = 0.17523;
      World[EARTH].Atmo.GasColor[1] = 0.40785;
      World[EARTH].Atmo.GasColor[2] = 1.0;
      World[EARTH].Atmo.DustColor[0] = 1.0;
      World[EARTH].Atmo.DustColor[1] = 1.0;
      World[EARTH].Atmo.DustColor[2] = 1.0;
      World[EARTH].Atmo.RayScat[0] = 5.8E-6;
      World[EARTH].Atmo.RayScat[1] = 13.5E-6;
      World[EARTH].Atmo.RayScat[2] = 33.1E-6;
      World[EARTH].Atmo.RayScaleHt = 8000.0;
      World[EARTH].Atmo.MieScat = 2.0E-5;
      World[EARTH].Atmo.MieScaleHt = 1200.0;
      World[EARTH].Atmo.MieG = 0.76;
      World[EARTH].Atmo.MaxHt = 8.0*World[EARTH].Atmo.RayScaleHt;
      World[EARTH].Atmo.rad = World[EARTH].rad + World[EARTH].Atmo.MaxHt;

      World[MARS].Atmo.GasColor[0] = 0.70588;
      World[MARS].Atmo.GasColor[1] = 0.50196;
      World[MARS].Atmo.GasColor[2] = 0.19608;
      World[MARS].Atmo.DustColor[0] = 0.5;
      World[MARS].Atmo.DustColor[1] = 0.5;
      World[MARS].Atmo.DustColor[2] = 0.5;
      World[MARS].Atmo.RayScat[0] = 19.918E-6;
      World[MARS].Atmo.RayScat[1] = 13.57E-6;
      World[MARS].Atmo.RayScat[2] = 5.75E-6;
      World[MARS].Atmo.RayScaleHt = 11000.0;
      World[MARS].Atmo.MieScat = 0.0E-6;
      World[MARS].Atmo.MieScaleHt = 11000.0;
      World[MARS].Atmo.MieG = 0.76;
      World[MARS].Atmo.MaxHt = 8.0*World[MARS].Atmo.RayScaleHt;
      World[MARS].Atmo.rad = World[MARS].rad + World[MARS].Atmo.MaxHt;

/* .. Load planetary orbit elements for date of interest */
      for(i=MERCURY;i<=PLUTO;i++){
         PlanetEphemerides(i,TT.JulDay,World[i].eph.mu,
                                &World[i].eph.SMA,
                                &World[i].eph.ecc,
                                &World[i].eph.inc,
                                &World[i].eph.RAAN,
                                &World[i].eph.ArgP,
                                &World[i].eph.tp,
                                &World[i].eph.anom,
                                &World[i].eph.SLR,
                                &World[i].eph.alpha,
                                &World[i].eph.rmin,
                                &World[i].eph.MeanMotion,
                                &World[i].eph.Period);
         /* TODO: These ephems are expressed in mean-equinox-of-date (MEME) */
         /* Would it be worthwhile to transform to J2000? */
      }

      /* Planetocentric Inertial Reference Frames */
      A2C(123,-23.4392911*D2R,0.0,0.0,World[EARTH].CNH);
      C2Q(World[EARTH].CNH,World[EARTH].qnh);
      for(i=0;i<3;i++) World[EARTH].qnj[i] = 0.0;
      World[EARTH].qnj[3] = 1.0;
      for(i=MERCURY;i<=PLUTO;i++) {
         if (i != EARTH) {
            A2C(312,(PoleRA[i]+90.0)*D2R,(90.0-PoleDec[i])*D2R,0.0,CNJ);
            MxM(CNJ,World[EARTH].CNH,World[i].CNH);
            C2Q(World[i].CNH,World[i].qnh);
            QxQT(World[i].qnh,qjh,World[i].qnj);
         }
      }

/* .. Saturn's Rings */
      World[SATURN].HasRing = 1;
      World[SATURN].RingInner = 67258.0E3;
      World[SATURN].RingOuter = 181328.0E3;

      for(i=MERCURY;i<=PLUTO;i++) {
         World[i].RadOfInfluence =
            RadiusOfInfluence(World[i].eph.mu,World[i].mu,World[i].eph.SMA);
         World[i].Type = PLANET;
      }

      for(i=MERCURY;i<=PLUTO;i++){
         if(World[i].Exists){
            Eph = &World[i].eph;
            Eph2RV(Eph->mu,Eph->SLR,Eph->ecc,Eph->inc,Eph->RAAN,Eph->ArgP,
                   DynTime-Eph->tp,Eph->PosN,Eph->VelN,&Eph->anom);
            for(j=0;j<3;j++) World[i].PosH[j] = Eph->PosN[j];
            World[i].PriMerAng = fmod(World[i].PriMerAngJ2000+World[i].w*DynTime,TwoPi);
            SimpRot(Zaxis,World[i].PriMerAng,World[i].CWN);
            C2Q(World[i].CWN,World[i].qwn);
         }
      }
/* .. Earth rotation is a special case */
      GMST = JD2GMST(UTC.JulDay);
      World[EARTH].PriMerAng = TwoPi*GMST;
      /* SimpRot(Zaxis,World[EARTH].PriMerAng,World[EARTH].CWN); */
      HiFiEarthPrecNute(UTC.JulDay,C_TEME_TETE,C_TETE_J2000);
      SimpRot(Zaxis,World[EARTH].PriMerAng,C_W_TETE);
      MxM(C_W_TETE,C_TETE_J2000,World[EARTH].CWN);
      C2Q(World[EARTH].CWN,World[EARTH].qwn);

      strcpy(World[EARTH].BumpTexFileName,"EarthBump.ppm");
}
/*********************************************************************/
void LoadMoonOfEarth(void)
{
#define Nm 1

      double Zaxis[3] = {0.0,0.0,1.0};
      char Name[Nm][40] = {"Luna"};
      char MapFileName[Nm][40] = {"Luna.ppm"};
      float Color[4] = {0.440417f, 0.441343f, 0.441084f,1.0f};
      double mu[Nm] = {4.902801E12};
      double J2[Nm] = {2.027E-4};
      double rad[Nm] = {1.738E6};
      double w[Nm] = {2.66E-6};
      double SMA[Nm] = {384400000.0};
      double ecc[Nm] = {0.0549};
      double inc[Nm] = {0.0};
      double RAAN[Nm] = {0.0};
      double omg[Nm] = {0.0};
      long EpochYear[Nm] = {2000};
      long EpochMon[Nm] = {1};
      long EpochDay[Nm] = {1};
      long EpochHour[Nm] = {12};
      double MeanAnom[Nm] = {0.0};
      double Epoch;
      unsigned char Glyph[14] = {0xc0,0xc0,0x00,0x00,0x18,0x70,0x60,
                                 0xe0,0xe0,0x60,0x70,0x18,0x00,0x00};

      long Ip = EARTH;
      long Iw,Im;
      long i;
      struct WorldType *M,*P;
      struct OrbitType *E;
      double CNJ[3][3];

      P = &World[Ip];
      P->Nsat = 1;
      P->Sat = (long *) calloc(Nm,sizeof(long));
      if (P->Sat == NULL) {
         printf("Earth P->Sat calloc returned null pointer.  Bailing out!\n");
         exit(1);
      }

      for(Im=0;Im<Nm;Im++) {
         Iw = LUNA+Im;
         M = &World[Iw];
         E = &M->eph;
         P->Sat[Im] = Iw;

         M->Exists = TRUE;
         M->Parent = EARTH;
         strcpy(M->Name,Name[Im]);
         strcpy(M->MapFileName,MapFileName[Im]);
         strcpy(M->ColTexFileName,"LunaCol.ppm");
         strcpy(M->BumpTexFileName,"LunaBump.ppm");
         for(i=0;i<4;i++) M->Color[i] = Color[i];
         for(i=0;i<14;i++) M->Glyph[i] = Glyph[i];
         M->mu = mu[Im];
         M->J2 = J2[Im];
         M->rad = rad[Im];
         M->w = w[Im];
         M->PriMerAng = 0.0;
         E->Exists = TRUE;
         E->Regime = ORB_CENTRAL;
         E->World = Ip;
         E->mu = P->mu;
         E->SMA = SMA[Im];
         E->ecc = ecc[Im];
         E->inc = inc[Im];
         E->RAAN = RAAN[Im];
         E->ArgP = omg[Im];

         Epoch = DateToTime(EpochYear[Im],EpochMon[Im],EpochDay[Im],EpochHour[Im],0,0.0);
         E->MeanMotion = sqrt(E->mu/(E->SMA*E->SMA*E->SMA));
         E->Period = TwoPi/E->MeanMotion;
         E->tp = Epoch - MeanAnom[Im]*D2R/E->MeanMotion;
         while ((E->tp-DynTime0) < -E->Period) E->tp += E->Period;
         while ((E->tp-DynTime0) >  E->Period) E->tp -= E->Period;

         E->alpha = 1.0/E->SMA;
         E->SLR = E->SMA*(1.0-E->ecc*E->ecc);
         E->rmin = E->SMA*(1.0-E->ecc);

         E->anom = TrueAnomaly(E->mu,E->SLR,E->ecc,DynTime-E->tp);
         M->RadOfInfluence = RadiusOfInfluence(P->mu,M->mu,E->SMA);

         LunaInertialFrame(TT.JulDay,CNJ);
         MxM(CNJ,World[EARTH].CNH,M->CNH);
         C2Q(M->CNH,M->qnh);
         QxQT(M->qnh,qjh,M->qnj);
         M->PriMerAng = LunaPriMerAng(TT.JulDay);
         SimpRot(Zaxis,M->PriMerAng,M->CWN);
         C2Q(M->CWN,M->qwn);
         M->Type = MOON;
      }
#undef Nm
}
/**********************************************************************/
/*  See JPL web pages MoonEphems and MoonParms in Development folder  */
void LoadMoonsOfMars(void)
{
#define Nm 2

      char Name[Nm][40] = {"Phobos","Deimos"};
      char MapFileName[Nm][40] = {"Rockball","Rockball"};
      double mu[Nm] = {7.158E5,9.8E4};
      double rad[Nm] = {11.1E3,6.2E3};
      double w[Nm] = {0.0,0.0};
      double SMA[Nm] = {9380.0E3,23460.0E3};
      double ecc[Nm] = {0.0151,0.0002};
      double inc[Nm] = {1.075,1.793};
      double RAAN[Nm] = {164.931,339.600};
      double omg[Nm] = {150.247,290.496};
      long EpochYear[Nm] = {1950,1950};
      long EpochMon[Nm] = {1,1};
      long EpochDay[Nm] = {1,1};
      double MeanAnom[Nm] = {92.474,296.230};
      double Epoch;

      long Ip = MARS;
      long Im,Iw;
      long i,j;
      struct WorldType *M,*P;
      struct OrbitType *E;

      P = &World[Ip];
      P->Nsat = Nm;
      P->Sat = (long *) calloc(Nm,sizeof(long));
      if (P->Sat == NULL) {
         printf("Mars P->Sat calloc returned null pointer.  Bailing out!\n");
         exit(1);
      }

      for(Im=0;Im<Nm;Im++) {
         Iw = PHOBOS + Im;
         M = &World[Iw];
         E = &M->eph;
         P->Sat[Im] = Iw;

         M->Exists = TRUE;
         M->Parent = MARS;
         strcpy(M->Name,Name[Im]);
         strcpy(M->MapFileName,MapFileName[Im]);
         strcpy(M->ColTexFileName,"NONE");
         strcpy(M->BumpTexFileName,"NONE");
         M->mu = mu[Im];
         M->rad = rad[Im];
         M->w = w[Im];
         M->PriMerAng = 0.0;
         E->Exists = TRUE;
         E->Regime = ORB_CENTRAL;
         E->World = Ip;
         E->mu = World[Ip].mu;
         E->SMA = SMA[Im];
         E->ecc = ecc[Im];
         E->inc = inc[Im]*D2R;
         E->RAAN = RAAN[Im]*D2R;
         E->ArgP = omg[Im];

         Epoch = DateToTime(EpochYear[Im],EpochMon[Im],EpochDay[Im],0,0,0.0);
         E->MeanMotion = sqrt(E->mu/(E->SMA*E->SMA*E->SMA));
         E->Period = TwoPi/E->MeanMotion;
         E->tp = Epoch - MeanAnom[Im]*D2R/E->MeanMotion;
         while ((E->tp-DynTime0) < -E->Period) E->tp += E->Period;
         while ((E->tp-DynTime0) >  E->Period) E->tp -= E->Period;

         E->alpha = 1.0/E->SMA;
         E->SLR = E->SMA*(1.0-E->ecc*E->ecc);
         E->rmin = E->SMA*(1.0-E->ecc);

         E->anom = TrueAnomaly(E->mu,E->SLR,E->ecc,DynTime-E->tp);
         M->RadOfInfluence = RadiusOfInfluence(P->mu,M->mu,E->SMA);

         /* CNH assumed to be same as parent planet */
         for(i=0;i<3;i++) {
            for(j=0;j<3;j++) M->CNH[i][j] = P->CNH[i][j];
         }
         C2Q(M->CNH,M->qnh);
         QxQT(M->qnh,qjh,M->qnj);
         for(i=0;i<4;i++) M->Color[i] = 1.0;
         M->Type = MOON;
      }
      strcpy(World[PHOBOS].MeshFileName,"Phobos.obj");
      Mesh = LoadWingsObjFile(ModelPath,World[PHOBOS].MeshFileName,
         &Matl,&Nmatl,Mesh,&Nmesh,&World[PHOBOS].MeshTag,FALSE);


#undef Nm
}
/**********************************************************************/
void LoadMoonsOfJupiter(void)
{
#define Nm 16

      char Name[Nm][40] = {"Io","Europa","Ganymede","Callisto","Amalthea",
         "Himalia","Elara","Pasiphae","Sinope","Lysithea","Carme",
         "Ananke","Leda","Thebe","Adrastea","Metis"};
      char MapFileName[Nm][40] = {"NONE","NONE","NONE","NONE",
         "NONE","NONE","NONE","NONE","NONE","NONE","NONE","NONE",
         "NONE","NONE","NONE","NONE"};
      double mu[Nm] = {5.959E9,3202.739E9,9887.834E9,7179.289E9,1.38E8,
         4.5E8,5.8E7,2.0E7,5.0E6,4.2E6,8.8E6,
         2.0E6,7.3E5,1.0E8,5.0E5,8.0E6};
      double rad[Nm] = {1821.6E3,1560.8E3,2631.2E3,2410.3E3,83.45E3,
         85.0E3,43.0E3,30.0E3,19.0E3,18.0E3,23.0E3,
         14.0E3,10.0E3,49.3E3,8.2E3,21.5E3};
      double w[Nm] = {0.0,0.0,0.0,0.0,
         0.0,0.0,0.0,0.0,0.0,
         0.0,0.0,0.0,0.0,};
      double SMA[Nm] = {4.2180E8,6.7110E8,1.07040E9,1.8827E9,1.814E8,
         1.1461E10,1.1741E10,2.3624E10,2.3939E10,1.1717E10,2.3404E10,
         2.1276E10,1.1165E10,2.219E8,1.29E8,1.28E8};
      double ecc[Nm] = {0.0041,0.0094,0.0013,0.0074,0.0032,
         0.1623,0.2174,0.4090,0.2495,0.1124,0.2533,
         0.2435,0.1636,0.0176,0.0018,0.0012};
      double inc[Nm] = {0.036,0.466,0.177,0.192,0.380,
         27.496,26.627,151.431,158.109,28.302,164.907,
         148.889,27.457,1.08,0.054,0.019};
      double RAAN[Nm] = {43.977,219.106,63.552,298.848,108.946,
         57.245,109.373,312.990,303.081,5.528,113.738,
         7.615,217.137,235.694,228.378,146.912};
      double omg[Nm] = {84.129,88.97,192.417,52.643,155.873,
         331.995,143.591,170.45,346.394,49.486,28.199,
         100.619,272.349,234.269,328.047,297.177};
      long EpochYear[Nm] = {1997,1997,1997,1997,1997,
         2000,2000,2000,2000,2000,2000,2000,2000,
         1997,1997,1997};
      long EpochMon[Nm] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
      long EpochDay[Nm] = {16,16,16,16,16,
         1,1,1,1,1,1,1,1,
         16,16,16};
      double MeanAnom[Nm] = {342.021,171.016,317.54,181.408,185.194,
         68.721,332.962,280.193,168.397,329.121,234.027,248.793,228.076,
         135.956,135.673,276.047};
      double Epoch;

      long Ip = JUPITER;
      long Im,Iw;
      long i,j;
      struct WorldType *M,*P;
      struct OrbitType *E;

      P = &World[Ip];
      P->Nsat = Nm;
      P->Sat = (long *) calloc(Nm,sizeof(long));
      if (P->Sat == NULL) {
         printf("Jupiter P->Sat calloc returned null pointer.  Bailing out!\n");
         exit(1);
      }

      for(Im=0;Im<Nm;Im++) {
         Iw = IO + Im;
         M = &World[Iw];
         E = &M->eph;
         P->Sat[Im] = Iw;

         M->Exists = TRUE;
         M->Parent = JUPITER;
         strcpy(M->Name,Name[Im]);
         strcpy(M->MapFileName,MapFileName[Im]);
         strcpy(M->ColTexFileName,"NONE");
         strcpy(M->BumpTexFileName,"NONE");
         M->mu = mu[Im];
         M->rad = rad[Im];
         M->w = w[Im];
         M->PriMerAng = 0.0;
         E->Exists = TRUE;
         E->Regime = ORB_CENTRAL;
         E->World = Ip;
         E->mu = World[Ip].mu;
         E->SMA = SMA[Im];
         E->ecc = ecc[Im];
         E->inc = inc[Im]*D2R;
         E->RAAN = RAAN[Im]*D2R;
         E->ArgP = omg[Im]*D2R;

         Epoch = DateToTime(EpochYear[Im],EpochMon[Im],EpochDay[Im],0,0,0.0);
         E->MeanMotion = sqrt(E->mu/(E->SMA*E->SMA*E->SMA));
         E->Period = TwoPi/E->MeanMotion;
         E->tp = Epoch - MeanAnom[Im]*D2R/E->MeanMotion;
         while ((E->tp-DynTime0) < -E->Period) E->tp += E->Period;
         while ((E->tp-DynTime0) >  E->Period) E->tp -= E->Period;

         E->alpha = 1.0/E->SMA;
         E->SLR = E->SMA*(1.0-E->ecc*E->ecc);
         E->rmin = E->SMA*(1.0-E->ecc);

         E->anom = TrueAnomaly(E->mu,E->SLR,E->ecc,DynTime-E->tp);
         M->RadOfInfluence = RadiusOfInfluence(P->mu,M->mu,E->SMA);

         /* CNH assumed to be same as parent planet */
         for(i=0;i<3;i++) {
            for(j=0;j<3;j++) M->CNH[i][j] = P->CNH[i][j];
         }
         C2Q(M->CNH,M->qnh);
         QxQT(M->qnh,qjh,M->qnj);
         for(i=0;i<4;i++) M->Color[i] = 1.0;
         M->Type = MOON;
      }
#undef Nm
}
/**********************************************************************/
void LoadMoonsOfSaturn(void)
{
#define Nm 18

      char Name[Nm][40] = {"Mimas","Enceladus","Tethys","Dione","Rhea",
         "Titan","Hyperion","Iapetus","Phoebe","Janus","Epimetheus",
         "Helene","Telesto","Calypso","Atlas","Prometheus","Pandora","Pan"};
      char MapFileName[Nm][40] = {"NONE","Iceball","NONE","NONE",
         "NONE","NONE","NONE","NONE","NONE","NONE","NONE","NONE",
         "NONE","NONE","NONE","NONE","NONE","NONE"};
      double mu[Nm] = {2.53E9,7.21E9,4.121E10,7.3113E10,1.5407E11,
         8.97819E12,3.7E8,1.205E11,5.531E8,1.266E8,3.51E7,
         1.7E6,4.8E5,2.4E5,1.4E5,1.246E7,9.95E6,3.3E5};
      double rad[Nm] = {198.8E3,252.3E3,536.3E3,562.5E3,764.5E3,
         2575.5E3,133.0E3,734.5E3,106.6E3,90.4E3,58.3E3,
         16.0E3,12.0E3,9.5E3,10.E3,46.8e3,40.6E3,12.8E3};
      double w[Nm] = {0.0,0.0,0.0,0.0,0.0,0.0,
         0.0,0.0,0.0,0.0,0.0,0.0,
         0.0,0.0,0.0,0.0,0.0,0.0};
      double SMA[Nm] = {1.8554E8,2.3804E8,2.9467E8,3.7742E8,5.2707E8,
         1.22187E9,1.50088E9,3.56084E9,1.294778E10,1.5146E8,1.5141E8,
         3.7742E8,2.9471E8,2.9471E8,1.3767E8,1.3938E8,1.4172E8,1.3358E8};
      double ecc[Nm] = {0.0196,0.0047,0.0001,0.0022,0.001,
         0.0288,0.0274,0.0283,0.1635,0.0068,0.0098,
         0.0071,0.0002,0.0005,0.0012,0.0022,0.0042,0.0};
      double inc[Nm] = {1.572,0.009,1.091,0.028,0.331,
         0.28,0.63,7.489,175.986,0.163,0.351,
         0.213,1.18,1.499,0.003,0.008,0.05,0.001};
      double RAAN[Nm] = {153.152,93.204,330.882,168.909,311.531,
         24.502,264.022,75.831,241.57,46.899,85.244,
         40.039,300.256,25.327,0.5,259.504,327.215,40.557};
      double omg[Nm] = {14.352,211.923,262.845,168.82,256.609,
         185.671,324.183,275.921,345.582,241.778,312.63,
         292.056,341.795,234.788,331.521,164.389,83.461,139.318};
      long EpochYear[Nm] = {2004,2004,2004,2004,2004,
         2004,2004,2004,2004,2004,2004,
         2004,2004,2004,2004,2004,2004,2004};
      long EpochMon[Nm] = {1,1,1,1,1,
         1,1,1,1,1,1,1,1,1,1,1,1,1};
      long EpochDay[Nm] = {1,1,1,1,1,
         1,1,1,1,1,1,1,1,1,1,1,1,1};
      double MeanAnom[Nm] = {255.312,197.047,189.003,65.99,311.551,
         15.154,295.906,356.029,287.593,242.754,308.322,
         134.07,200.143,101.961,157.738,242.224,202.697,246.065};
      double Epoch;

      long Ip = SATURN;
      long Im,Iw;
      long i,j;
      struct WorldType *M,*P;
      struct OrbitType *E;

      P = &World[Ip];
      P->Nsat = Nm;
      P->Sat = (long *) calloc(Nm,sizeof(long));
      if (P->Sat == NULL) {
         printf("Saturn P->Sat calloc returned null pointer.  Bailing out!\n");
         exit(1);
      }

      for(Im=0;Im<Nm;Im++) {
         Iw = MIMAS + Im;
         M = &World[Iw];
         E = &M->eph;
         P->Sat[Im] = Iw;

         M->Exists = TRUE;
         M->Parent = SATURN;
         strcpy(M->Name,Name[Im]);
         strcpy(M->MapFileName,MapFileName[Im]);
         strcpy(M->ColTexFileName,"NONE");
         strcpy(M->BumpTexFileName,"NONE");
         M->mu = mu[Im];
         M->rad = rad[Im];
         M->w = w[Im];
         M->PriMerAng = 0.0;
         E->Exists = TRUE;
         E->Regime = ORB_CENTRAL;
         E->World = Ip;
         E->mu = World[Ip].mu;
         E->SMA = SMA[Im];
         E->ecc = ecc[Im];
         E->inc = inc[Im];
         E->RAAN = RAAN[Im];
         E->ArgP = omg[Im];

         Epoch = DateToTime(EpochYear[Im],EpochMon[Im],EpochDay[Im],0,0,0.0);
         E->MeanMotion = sqrt(E->mu/(E->SMA*E->SMA*E->SMA));
         E->Period = TwoPi/E->MeanMotion;
         E->tp = Epoch - MeanAnom[Im]*D2R/E->MeanMotion;
         while ((E->tp-DynTime0) < -E->Period) E->tp += E->Period;
         while ((E->tp-DynTime0) >  E->Period) E->tp -= E->Period;

         E->alpha = 1.0/E->SMA;
         E->SLR = E->SMA*(1.0-E->ecc*E->ecc);
         E->rmin = E->SMA*(1.0-E->ecc);

         E->anom = TrueAnomaly(E->mu,E->SLR,E->ecc,DynTime-E->tp);
         M->RadOfInfluence = RadiusOfInfluence(P->mu,M->mu,E->SMA);

         /* CNH assumed to be same as parent planet */
         for(i=0;i<3;i++) {
            for(j=0;j<3;j++) M->CNH[i][j] = P->CNH[i][j];
         }
         C2Q(M->CNH,M->qnh);
         QxQT(M->qnh,qjh,M->qnj);
         for(i=0;i<4;i++) M->Color[i] = 1.0;
         M->Type = MOON;
      }
#undef Nm
}
/**********************************************************************/
void LoadMoonsOfUranus(void)
{
#define Nm 5

      char Name[Nm][40] = {"Ariel","Umbriel","Titania","Oberon","Miranda"};
      char MapFileName[Nm][40] = {"NONE","NONE","NONE","NONE","NONE"};
      double mu[Nm] = {90.3E9,78.2E9,235.3E9,201.1E9,4.4E9};
      double rad[Nm] = {578.9E3,584.7E3,788.9E3,761.4E3,235.8E3};
      double w[Nm] = {0.0,0.0,0.0,0.0,0.0};
      double SMA[Nm] = {1.909E8,2.66E8,4.363E8,5.835E8,1.299E8};
      double ecc[Nm] = {0.0012,0.0039,0.0011,0.0014,0.0013};
      double inc[Nm] = {0.041,0.128,0.079,0.068,4.338};
      double RAAN[Nm] = {22.394,33.485,99.771,279.771,326.438};
      double omg[Nm] = {115.349,84.709,284.4,104.4,68.312};
      long EpochYear[Nm] = {1980,1980,1980,1980,1980};
      long EpochMon[Nm] = {1,1,1,1,1};
      long EpochDay[Nm] = {1,1,1,1,1};
      double MeanAnom[Nm] = {39.481,12.469,24.614,283.088,311.33};
      double Epoch;

      long Ip = URANUS;
      long Im,Iw;
      long i,j;
      struct WorldType *M,*P;
      struct OrbitType *E;

      P = &World[Ip];
      P->Nsat = Nm;
      P->Sat = (long *) calloc(Nm,sizeof(long));
      if (P->Sat == NULL) {
         printf("Uranus P->Sat calloc returned null pointer.  Bailing out!\n");
         exit(1);
      }

      for(Im=0;Im<Nm;Im++) {
         Iw = ARIEL + Im;
         M = &World[Iw];
         E = &M->eph;
         P->Sat[Im] = Iw;

         M->Exists = TRUE;
         M->Parent = URANUS;
         strcpy(M->Name,Name[Im]);
         strcpy(M->MapFileName,MapFileName[Im]);
         strcpy(M->ColTexFileName,"NONE");
         strcpy(M->BumpTexFileName,"NONE");
         M->mu = mu[Im];
         M->rad = rad[Im];
         M->w = w[Im];
         M->PriMerAng = 0.0;
         E->Exists = TRUE;
         E->Regime = ORB_CENTRAL;
         E->World = Ip;
         E->mu = World[Ip].mu;
         E->SMA = SMA[Im];
         E->ecc = ecc[Im];
         E->inc = inc[Im];
         E->RAAN = RAAN[Im];
         E->ArgP = omg[Im];

         Epoch = DateToTime(EpochYear[Im],EpochMon[Im],EpochDay[Im],0,0,0.0);
         E->MeanMotion = sqrt(E->mu/(E->SMA*E->SMA*E->SMA));
         E->Period = TwoPi/E->MeanMotion;
         E->tp = Epoch - MeanAnom[Im]*D2R/E->MeanMotion;
         while ((E->tp-DynTime0) < -E->Period) E->tp += E->Period;
         while ((E->tp-DynTime0) >  E->Period) E->tp -= E->Period;

         E->alpha = 1.0/E->SMA;
         E->SLR = E->SMA*(1.0-E->ecc*E->ecc);
         E->rmin = E->SMA*(1.0-E->ecc);

         E->anom = TrueAnomaly(E->mu,E->SLR,E->ecc,DynTime-E->tp);
         M->RadOfInfluence = RadiusOfInfluence(P->mu,M->mu,E->SMA);

         /* CNH assumed to be same as parent planet */
         for(i=0;i<3;i++) {
            for(j=0;j<3;j++) M->CNH[i][j] = P->CNH[i][j];
         }
         C2Q(M->CNH,M->qnh);
         QxQT(M->qnh,qjh,M->qnj);
         for(i=0;i<4;i++) M->Color[i] = 1.0;
         M->Type = MOON;
      }
#undef Nm
}
/**********************************************************************/
void LoadMoonsOfNeptune(void)
{
#define Nm 2

      char Name[Nm][40] = {"Triton","Nereid"};
      char MapFileName[Nm][40] = {"NONE","NONE"};
      double mu[Nm] = {1427.9E9,2.06E9};
      double rad[Nm] = {1353.4E3,170.0E3};
      double w[Nm] = {0.0,0.0};
      double SMA[Nm] = {3.548E8,5.5134E9};
      double ecc[Nm] = {0.0,0.7512};
      double inc[Nm] = {156.834,7.232};
      double RAAN[Nm] = {172.431,334.762};
      double omg[Nm] = {344.046,280.83};
      long EpochYear[Nm] = {1989,1989};
      long EpochMon[Nm] = {8,8};
      long EpochDay[Nm] = {25,25};
      double MeanAnom[Nm] = {264.775,359.341};
      double Epoch;

      long Ip = NEPTUNE;
      long Im,Iw;
      long i,j;
      struct WorldType *M,*P;
      struct OrbitType *E;

      P = &World[Ip];
      P->Nsat = Nm;
      P->Sat = (long *) calloc(Nm,sizeof(long));
      if (P->Sat == NULL) {
         printf("Neptune P->Sat calloc returned null pointer.  Bailing out!\n");
         exit(1);
      }

      for(Im=0;Im<Nm;Im++) {
         Iw = TRITON + Im;
         M = &World[Iw];
         E = &M->eph;
         P->Sat[Im] = Iw;

         M->Exists = TRUE;
         M->Parent = NEPTUNE;
         strcpy(M->Name,Name[Im]);
         strcpy(M->MapFileName,MapFileName[Im]);
         strcpy(M->ColTexFileName,"NONE");
         strcpy(M->BumpTexFileName,"NONE");
         M->mu = mu[Im];
         M->rad = rad[Im];
         M->w = w[Im];
         M->PriMerAng = 0.0;
         E->Exists = TRUE;
         E->Regime = ORB_CENTRAL;
         E->World = Ip;
         E->mu = World[Ip].mu;
         E->SMA = SMA[Im];
         E->ecc = ecc[Im];
         E->inc = inc[Im];
         E->RAAN = RAAN[Im];
         E->ArgP = omg[Im];

         Epoch = DateToTime(EpochYear[Im],EpochMon[Im],EpochDay[Im],0,0,0.0);
         E->MeanMotion = sqrt(E->mu/(E->SMA*E->SMA*E->SMA));
         E->Period = TwoPi/E->MeanMotion;
         E->tp = Epoch - MeanAnom[Im]*D2R/E->MeanMotion;
         while ((E->tp-DynTime0) < -E->Period) E->tp += E->Period;
         while ((E->tp-DynTime0) >  E->Period) E->tp -= E->Period;

         E->alpha = 1.0/E->SMA;
         E->SLR = E->SMA*(1.0-E->ecc*E->ecc);
         E->rmin = E->SMA*(1.0-E->ecc);

         E->anom = TrueAnomaly(E->mu,E->SLR,E->ecc,DynTime-E->tp);
         M->RadOfInfluence = RadiusOfInfluence(P->mu,M->mu,E->SMA);

         /* CNH assumed to be same as parent planet */
         for(i=0;i<3;i++) {
            for(j=0;j<3;j++) M->CNH[i][j] = P->CNH[i][j];
         }
         C2Q(M->CNH,M->qnh);
         QxQT(M->qnh,qjh,M->qnj);
         for(i=0;i<4;i++) M->Color[i] = 1.0;
         M->Type = MOON;
      }
#undef Nm
}
/**********************************************************************/
void LoadMoonsOfPluto(void)
{
#define Nm 1

      char Name[Nm][40] = {"Charon"};
      char MapFileName[Nm][40] = {"Iceball"};
      double mu[Nm] = {108.0E9};
      double rad[Nm] = {593.0E3};
      double w[Nm] = {0.0};
      double SMA[Nm] = {1.7536E7};
      double ecc[Nm] = {0.0022};
      double inc[Nm] = {0.001};
      double RAAN[Nm] = {85.187};
      double omg[Nm] = {71.255};
      long EpochYear[Nm] = {2000};
      long EpochMon[Nm] = {1};
      long EpochDay[Nm] = {1};
      long EpochHour[Nm] = {12};
      double MeanAnom[Nm] = {147.848};
      double Epoch;

      long Ip = PLUTO;
      long Iw,Im;
      long i,j;
      struct WorldType *M,*P;
      struct OrbitType *E;

      P = &World[Ip];
      P->Nsat = 1;
      P->Sat = (long *) calloc(Nm,sizeof(long));
      if (P->Sat == NULL) {
         printf("Pluto P->Sat calloc returned null pointer.  Bailing out!\n");
         exit(1);
      }

      for(Im=0;Im<Nm;Im++) {
         Iw = CHARON+Im;
         M = &World[Iw];
         E = &M->eph;
         P->Sat[Im] = Iw;

         M->Exists = TRUE;
         M->Parent = PLUTO;
         strcpy(M->Name,Name[Im]);
         strcpy(M->MapFileName,MapFileName[Im]);
         strcpy(M->ColTexFileName,"NONE");
         strcpy(M->BumpTexFileName,"NONE");
         M->mu = mu[Im];
         M->rad = rad[Im];
         M->w = w[Im];
         M->PriMerAng = 0.0;
         E->Exists = TRUE;
         E->Regime = ORB_CENTRAL;
         E->World = Ip;
         E->mu = P->mu;
         E->SMA = SMA[Im];
         E->ecc = ecc[Im];
         E->inc = inc[Im];
         E->RAAN = RAAN[Im];
         E->ArgP = omg[Im];

         Epoch = DateToTime(EpochYear[Im],EpochMon[Im],EpochDay[Im],EpochHour[Im],0,0.0);
         E->MeanMotion = sqrt(E->mu/(E->SMA*E->SMA*E->SMA));
         E->Period = TwoPi/E->MeanMotion;
         E->tp = Epoch - MeanAnom[Im]*D2R/E->MeanMotion;
         while ((E->tp-DynTime0) < -E->Period) E->tp += E->Period;
         while ((E->tp-DynTime0) >  E->Period) E->tp -= E->Period;

         E->alpha = 1.0/E->SMA;
         E->SLR = E->SMA*(1.0-E->ecc*E->ecc);
         E->rmin = E->SMA*(1.0-E->ecc);

         E->anom = TrueAnomaly(E->mu,E->SLR,E->ecc,DynTime-E->tp);
         M->RadOfInfluence = RadiusOfInfluence(P->mu,M->mu,E->SMA);

         /* CNH assumed to be same as parent planet */
         for(i=0;i<3;i++) {
            for(j=0;j<3;j++) M->CNH[i][j] = P->CNH[i][j];
         }
         C2Q(M->CNH,M->qnh);
         QxQT(M->qnh,qjh,M->qnj);
         for(i=0;i<4;i++) M->Color[i] = 1.0;
         M->Type = MOON;
      }
#undef Nm
}
/**********************************************************************/
void LoadMinorBodies(void)
{
      FILE *infile;
      struct WorldType *W;
      struct OrbitType *E;
      char response[120];
      long Ib,i;
      long EpochYear,EpochMon,EpochDay,EpochHour;
      double CNJ[3][3],PoleRA,PoleDec,Epoch;
      double ZAxis[3] = {0.0,0.0,1.0};

      infile = FileOpen(ModelPath,"MinorBodies.txt","r");
      ScanLine(infile,"",0,NULL);
      ScanLine(infile,"%ld",1,&Nmb);
      if (Nmb > 10) {
         printf("Only 10 minor bodies are supported.  Adjust NWORLD to suit.\n");
         exit(1);
      }
      for(Ib=0;Ib<Nmb;Ib++) {
         W = &World[55+Ib];
         E = &W->eph;
         ScanLine(infile,"",0,NULL);
         ScanLine(infile,"%120s",1,response);
         W->Exists=DecodeString(response);
         ScanLine(infile,"\"%40[^\"]\"",1,W->Name);
         ScanLine(infile,"%120s",1,response);
         W->Type=DecodeString(response);
         ScanLine(infile,"%40s",1,W->MapFileName);
         ScanLine(infile,"%40s",1,W->MeshFileName);
         ScanLine(infile,"%40s",1,W->ColTexFileName);
         ScanLine(infile,"%40s",1,W->BumpTexFileName);
         ScanLine(infile,"%lf",1,&W->mu);
         ScanLine(infile,"%lf",1,&W->rad);
         ScanLine(infile,"%lf",1,&W->w);
         ScanLine(infile,"%lf %lf",2,&PoleRA,&PoleDec);
         A2C(312,(PoleRA+90.0)*D2R,(90.0-PoleDec)*D2R,0.0,CNJ);
         MxM(CNJ,World[EARTH].CNH,W->CNH);
         C2Q(W->CNH,W->qnh);
         QxQT(W->qnh,qjh,W->qnj);
         E->Exists = TRUE;
         E->Regime = ORB_CENTRAL;
         E->World = SOL;
         E->mu = World[SOL].mu;
         ScanLine(infile,"%lf",1,&E->SMA);
         ScanLine(infile,"%lf",1,&E->ecc);
         ScanLine(infile,"%lf",1,&E->inc);
         ScanLine(infile,"%lf",1,&E->RAAN);
         ScanLine(infile,"%lf",1,&E->ArgP);
         ScanLine(infile,"%ld %ld %ld %ld",4,
            &EpochYear,&EpochMon,&EpochDay,&EpochHour);
         ScanLine(infile,"%lf",1,&E->anom);
         Epoch = DateToTime(EpochYear,EpochMon,EpochDay,EpochHour,0,0.0);
         E->MeanMotion = sqrt(E->mu/(E->SMA*E->SMA*E->SMA));
         E->Period = TwoPi/E->MeanMotion;
         E->alpha = 1.0/E->SMA;
         E->SLR = E->SMA*(1.0-E->ecc*E->ecc);
         E->rmin = E->SMA*(1.0-E->ecc);
         E->tp = Epoch - TimeSincePeriapsis(E->mu,E->SLR,E->ecc,E->anom);
         while ((E->tp-DynTime0) < -E->Period) E->tp += E->Period;
         while ((E->tp-DynTime0) >  E->Period) E->tp -= E->Period;

         Mesh = LoadWingsObjFile(ModelPath,W->MeshFileName,
            &Matl,&Nmatl,Mesh,&Nmesh,&W->MeshTag,TRUE);
         W->Density = W->mu/(6.67408E-11*PolyhedronVolume(&Mesh[W->MeshTag]));

         W->Parent = SOL;
         W->Nsat = 0;
         W->RadOfInfluence = 100.0E3; /* Being generous */
         W->DipoleMoment = 0.0;
         W->DipoleAxis[2] = 1.0;
         W->Atmo.Exists = FALSE;
         W->HasRing = FALSE;
         for(i=0;i<3;i++) W->Color[i] = 0.5;
         W->Color[3] = 1.0;

         Eph2RV(E->mu,E->SLR,E->ecc,E->inc,E->RAAN,E->ArgP,
                DynTime-E->tp,E->PosN,E->VelN,&E->anom);
         for(i=0;i<3;i++) {
            W->PosH[i] = E->PosN[i];
            W->VelH[i] = E->VelN[i];
         }
         W->PriMerAng = fmod(W->w*DynTime,TwoPi);
         SimpRot(ZAxis,W->PriMerAng,W->CWN);
         C2Q(W->CWN,W->qwn);

      }
      fclose(infile);
}
/**********************************************************************/
void LoadRegions(void)
{
      FILE *infile;
      long Ir;
      char Exists[21],WorldID[41],IsPosW[121];
      struct WorldType *W;
      struct RegionType *R;
      double MagR;
      double VelW[3];

      infile = FileOpen(InOutPath,"Inp_Region.txt","rt");

      ScanLine(infile,"",0,NULL);
      ScanLine(infile,"%ld",1,&Nrgn);
      Rgn = (struct RegionType *) calloc(Nrgn,sizeof(struct RegionType));
      for(Ir=0;Ir<Nrgn;Ir++) {
         R = &Rgn[Ir];
         ScanLine(infile,"",0,NULL);
         ScanLine(infile,"%20s",1,Exists);
         R->Exists = DecodeString(Exists);
         ScanLine(infile,"\"%40[^\"]\"",1,R->Name);
         ScanLine(infile,"%40s",1,WorldID);
         R->World = DecodeString(WorldID);
         if (R->World<0 || R->World > NWORLD) {
            printf("Region's World is out of range in LoadRegions.  Bailing out.\n");
            exit(1);
         }
         W = &World[R->World];
         ScanLine(infile,"%120s",1,IsPosW);
         if (DecodeString(IsPosW)) {
            ScanLine(infile,"%lf %lf %lf",3,
               &R->PosW[0],&R->PosW[1],&R->PosW[2]);
            R->Lng = atan2(R->PosW[1],R->PosW[0]);
            MagR = MAGV(R->PosW);
            R->Lat = asin(R->PosW[2]/MagR);
            R->Alt = MagR-W->rad;
            A2C(312,R->Lng+HalfPi,HalfPi-R->Lat,0.0,R->CW);
            /* for(i=0;i<3;i++) R->CRW[i][i] = 1.0; */
            MTxV(W->CWN,R->PosW,R->PosN);
            MxM(R->CW,W->CWN,R->CN);
            ScanLine(infile,"",0,NULL);
         }
         else {
            ScanLine(infile,"",0,NULL);
            ScanLine(infile,"%lf %lf %lf",3,&R->Lng,&R->Lat,&R->Alt);
            R->Lng *= D2R;
            R->Lat *= D2R;
            MagR = W->rad+R->Alt;
            R->PosW[0] = MagR*cos(R->Lng)*cos(R->Lat);
            R->PosW[1] = MagR*sin(R->Lng)*cos(R->Lat);
            R->PosW[2] = MagR*sin(R->Lat);
            A2C(312,R->Lng+HalfPi,HalfPi-R->Lat,0.0,R->CW);
            MTxV(W->CWN,R->PosW,R->PosN);
            MxM(R->CW,W->CWN,R->CN);
         }
         VelW[0] = -W->w*R->PosW[1];
         VelW[1] =  W->w*R->PosW[0];
         VelW[2] = 0.0;
         MTxV(W->CWN,VelW,R->VelN);         
         R->wn[0] = 0.0;
         R->wn[1] = W->w*cos(R->Lat);
         R->wn[2] = W->w*sin(R->Lat);
         ScanLine(infile,"%lf %lf %lf",3,
            &R->ElastCoef,&R->DampCoef,&R->FricCoef);
         ScanLine(infile,"%40s",1,R->MeshFileName);
         Mesh = LoadWingsObjFile(ModelPath,R->MeshFileName,
            &Matl,&Nmatl,Mesh,&Nmesh,&R->MeshTag,TRUE);
      }
      fclose(infile);
}
/**********************************************************************/
void InitLagrangePoints(void)
{
      long i,j;
      char LagsysName[3][20]={"Earth-Luna","Sun-Earth","Sun-Jupiter"};
      struct LagrangeSystemType *LS;
      struct WorldType *W1,*W2;

      LagSys[EARTHMOON].Body1 = EARTH;
      LagSys[EARTHMOON].Body2 = LUNA;
      LagSys[SUNEARTH].Body1 = SOL;
      LagSys[SUNEARTH].Body2 = EARTH;
      LagSys[SUNJUPITER].Body1 = SOL;
      LagSys[SUNJUPITER].Body2 = JUPITER;

      for(i=0;i<3;i++) {
         LS = &LagSys[i];
         W1 = &World[LS->Body1];
         W2 = &World[LS->Body2];
         strcpy(LS->Name,LagsysName[i]);
         if (LS->Exists) {
            if (!(W1->Exists && W2->Exists)) {
               printf("Lagrange System %s depends on worlds that don't exist.  Check Inp_Sim.txt\n",LS->Name);
               exit(1);
            }
            LS->mu1 = W1->mu;
            LS->mu2 = W2->mu;
            LS->rho = LS->mu2/(LS->mu1+LS->mu2);
            LS->SLR = W2->eph.SLR;
            LS->SMA = W2->eph.SMA;
            LS->ecc = W2->eph.ecc;
            LS->inc = W2->eph.inc;
            LS->RAAN = W2->eph.RAAN;
            LS->ArgP = W2->eph.ArgP;
            LS->tp = W2->eph.tp;
            LS->MeanRate = sqrt(LS->mu1/LS->SMA)/LS->SMA;
            LS->Period = TwoPi/LS->MeanRate;

            FindLagPtParms(LS);
            for(j=0;j<5;j++) {
               FindLagPtPosVel(DynTime,LS,j,
                  LS->LP[j].PosN,LS->LP[j].VelN,LS->CLN);
            }
         }
      }
}
/******************************************************************************/
long LoadJplEphems(char EphemPath[80],double JD)
{
      FILE *infile;
      double Block[1020];
      long BlockNum,NumEntries;
      long FoundBlock;
      char line[512];
      double JD1,JD2;
      long i,n,Ic,Iw;
      long Nseg,Start,N;
      struct Cheb3DType *Cheb;
      struct OrbitType *Eph;
      struct WorldType *W;
      double u,dudJD,T[20],U[20],P,dPdu;
      double rh[3],vh[3];
      double EarthMoonBaryPosH[3],EarthMoonBaryVelH[3];
      double EMRAT = 81.30056907419062; /* Earth-Moon mass ratio */
      double ZAxis[3] = {0.0,0.0,1.0}; 
      double PosJ[3],VelJ[3];

/* .. Select input file */
      if (JD < 2433264.5) {
         printf("JD earlier than JPL ephem input files.  Falling back to lower-precision planetary ephemerides.\n");
         return(1);
      }
      else if (JD < 2469808.5) {
         if (EphemOption == EPH_DE430)
            infile = FileOpen(EphemPath,"ascp1950.430","rt");
         else if (EphemOption == EPH_DE440)
            infile = FileOpen(EphemPath,"ascp01950.440","rt");
         else {
            printf("Unknown Ephem Option in LoadJplEphems.\n");
            exit(1);
         }             
      }
      else if (JD < 2506352.5) {
         if (EphemOption == EPH_DE430)
            infile = FileOpen(EphemPath,"ascp2050.430","rt");
         else if (EphemOption == EPH_DE440)
            infile = FileOpen(EphemPath,"ascp02050.440","rt");
         else {
            printf("Unknown Ephem Option in LoadJplEphems.\n");
            exit(1);
         }             
      }
      else if (JD < 2542864.5) {
         if (EphemOption == EPH_DE430)
            infile = FileOpen(EphemPath,"ascp2150.430","rt");
         else if (EphemOption == EPH_DE440)
            infile = FileOpen(EphemPath,"ascp02150.440","rt");
         else {
            printf("Unknown Ephem Option in LoadJplEphems.\n");
            exit(1);
         }             
      }
      else {
         printf("JD later than JPL ephem input files.  Falling back to lower-precision planetary ephemerides.\n");
         return(1);
      }

/* .. Search for block */
      FoundBlock = 0;
      while(!FoundBlock) {
         fgets(line,511,infile);
         if (sscanf(line,"%ld %ld",&BlockNum,&NumEntries) == 2) {
         fgets(line,511,infile);
            if (sscanf(line,"%lf %lf %lf",&Block[0],&Block[1],&Block[2]) == 3) {
               if (JD >= Block[0] && JD < Block[1]) {
                  FoundBlock = 1;
                  JD1 = Block[0];
                  JD2 = Block[1];
               }
            }
         }
      }
      
/* .. Load block */ 
      for(i=1;i<340;i++) {
         fgets(line,511,infile);
         sscanf(line,"%lf %lf %lf",&Block[3*i],&Block[3*i+1],&Block[3*i+2]);
      }
      fclose(infile);
            
/* .. Distribute to Worlds [Starting Entry (1-based), Order, Number of Segments] */
      /* Mercury [3 14 4] */
      Iw = MERCURY;
      Nseg = 4;
      Start = 3-1;
      N = 14;
      World[Iw].eph.Ncheb = Nseg;
      World[Iw].eph.Cheb = (struct Cheb3DType *) calloc(Nseg,sizeof(struct Cheb3DType));
      for(Ic=0;Ic<Nseg;Ic++) {
         Cheb = &World[Iw].eph.Cheb[Ic];
         Cheb->JD1 = JD1 + ((double) Ic)*(JD2-JD1)/((double) Nseg);
         Cheb->JD2 = JD2 - ((double) (Nseg-1-Ic))*(JD2-JD1)/((double) Nseg);
         Cheb->N = N;
         for(n=0;n<N;n++) {
            for(i=0;i<3;i++) {
               Cheb->Coef[i][n] = Block[Start + N*3*Ic + N*i+n];
            }
         }
      }
      /* Venus [171 10 2] */
      Iw = VENUS;
      Nseg = 2;
      Start = 171-1;
      N = 10;
      World[Iw].eph.Ncheb = Nseg;
      World[Iw].eph.Cheb = (struct Cheb3DType *) calloc(Nseg,sizeof(struct Cheb3DType));
      for(Ic=0;Ic<Nseg;Ic++) {
         Cheb = &World[Iw].eph.Cheb[Ic];
         Cheb->JD1 = JD1 + ((double) Ic)*(JD2-JD1)/((double) Nseg);
         Cheb->JD2 = JD2 - ((double) (Nseg-1-Ic))*(JD2-JD1)/((double) Nseg);
         Cheb->N = N;
         for(n=0;n<N;n++) {
            for(i=0;i<3;i++) {
               Cheb->Coef[i][n] = Block[Start + N*3*Ic + N*i+n];
            }
         }
      }
      /* Earth-Moon barycenter [231 13 2] */
      Iw = EARTH;
      Nseg = 2;
      Start = 231-1;
      N = 13;
      World[Iw].eph.Ncheb = Nseg;
      World[Iw].eph.Cheb = (struct Cheb3DType *) calloc(Nseg,sizeof(struct Cheb3DType));
      for(Ic=0;Ic<Nseg;Ic++) {
         Cheb = &World[Iw].eph.Cheb[Ic];
         Cheb->JD1 = JD1 + ((double) Ic)*(JD2-JD1)/((double) Nseg);
         Cheb->JD2 = JD2 - ((double) (Nseg-1-Ic))*(JD2-JD1)/((double) Nseg);
         Cheb->N = N;
         for(n=0;n<N;n++) {
            for(i=0;i<3;i++) {
               Cheb->Coef[i][n] = Block[Start + N*3*Ic + N*i+n];
            }
         }
      }
      /* Mars [309 11 1] */
      Iw = MARS;
      Nseg = 1;
      Start = 309-1;
      N = 11;
      World[Iw].eph.Ncheb = Nseg;
      World[Iw].eph.Cheb = (struct Cheb3DType *) calloc(Nseg,sizeof(struct Cheb3DType));
      for(Ic=0;Ic<Nseg;Ic++) {
         Cheb = &World[Iw].eph.Cheb[Ic];
         Cheb->JD1 = JD1 + ((double) Ic)*(JD2-JD1)/((double) Nseg);
         Cheb->JD2 = JD2 - ((double) (Nseg-1-Ic))*(JD2-JD1)/((double) Nseg);
         Cheb->N = N;
         for(n=0;n<N;n++) {
            for(i=0;i<3;i++) {
               Cheb->Coef[i][n] = Block[Start + N*3*Ic + N*i+n];
            }
         }
      }
      /* Jupiter [342 8 1] */
      Iw = JUPITER;
      Nseg = 1;
      Start = 342-1;
      N = 8;
      World[Iw].eph.Ncheb = Nseg;
      World[Iw].eph.Cheb = (struct Cheb3DType *) calloc(Nseg,sizeof(struct Cheb3DType));
      for(Ic=0;Ic<Nseg;Ic++) {
         Cheb = &World[Iw].eph.Cheb[Ic];
         Cheb->JD1 = JD1 + ((double) Ic)*(JD2-JD1)/((double) Nseg);
         Cheb->JD2 = JD2 - ((double) (Nseg-1-Ic))*(JD2-JD1)/((double) Nseg);
         Cheb->N = N;
         for(n=0;n<N;n++) {
            for(i=0;i<3;i++) {
               Cheb->Coef[i][n] = Block[Start + N*3*Ic + N*i+n];
            }
         }
      }
      /* Saturn [366 7 1] */
      Iw = SATURN;
      Nseg = 1;
      Start = 366-1;
      N = 7;
      World[Iw].eph.Ncheb = Nseg;
      World[Iw].eph.Cheb = (struct Cheb3DType *) calloc(Nseg,sizeof(struct Cheb3DType));
      for(Ic=0;Ic<Nseg;Ic++) {
         Cheb = &World[Iw].eph.Cheb[Ic];
         Cheb->JD1 = JD1 + ((double) Ic)*(JD2-JD1)/((double) Nseg);
         Cheb->JD2 = JD2 - ((double) (Nseg-1-Ic))*(JD2-JD1)/((double) Nseg);
         Cheb->N = N;
         for(n=0;n<N;n++) {
            for(i=0;i<3;i++) {
               Cheb->Coef[i][n] = Block[Start + N*3*Ic + N*i+n];
            }
         }
      }
      /* Uranus [387 6 1] */
      Iw = URANUS;
      Nseg = 1;
      Start = 387-1;
      N = 6;
      World[Iw].eph.Ncheb = Nseg;
      World[Iw].eph.Cheb = (struct Cheb3DType *) calloc(Nseg,sizeof(struct Cheb3DType));
      for(Ic=0;Ic<Nseg;Ic++) {
         Cheb = &World[Iw].eph.Cheb[Ic];
         Cheb->JD1 = JD1 + ((double) Ic)*(JD2-JD1)/((double) Nseg);
         Cheb->JD2 = JD2 - ((double) (Nseg-1-Ic))*(JD2-JD1)/((double) Nseg);
         Cheb->N = N;
         for(n=0;n<N;n++) {
            for(i=0;i<3;i++) {
               Cheb->Coef[i][n] = Block[Start + N*3*Ic + N*i+n];
            }
         }
      }
      /* Neptune [405 6 1] */
      Iw = NEPTUNE;
      Nseg = 1;
      Start = 405-1;
      N = 6;
      World[Iw].eph.Ncheb = Nseg;
      World[Iw].eph.Cheb = (struct Cheb3DType *) calloc(Nseg,sizeof(struct Cheb3DType));
      for(Ic=0;Ic<Nseg;Ic++) {
         Cheb = &World[Iw].eph.Cheb[Ic];
         Cheb->JD1 = JD1 + ((double) Ic)*(JD2-JD1)/((double) Nseg);
         Cheb->JD2 = JD2 - ((double) (Nseg-1-Ic))*(JD2-JD1)/((double) Nseg);
         Cheb->N = N;
         for(n=0;n<N;n++) {
            for(i=0;i<3;i++) {
               Cheb->Coef[i][n] = Block[Start + N*3*Ic + N*i+n];
            }
         }
      }
      /* Pluto [423 6 1] */
      Iw = PLUTO;
      Nseg = 1;
      Start = 423-1;
      N = 6;
      World[Iw].eph.Ncheb = Nseg;
      World[Iw].eph.Cheb = (struct Cheb3DType *) calloc(Nseg,sizeof(struct Cheb3DType));
      for(Ic=0;Ic<Nseg;Ic++) {
         Cheb = &World[Iw].eph.Cheb[Ic];
         Cheb->JD1 = JD1 + ((double) Ic)*(JD2-JD1)/((double) Nseg);
         Cheb->JD2 = JD2 - ((double) (Nseg-1-Ic))*(JD2-JD1)/((double) Nseg);
         Cheb->N = N;
         for(n=0;n<N;n++) {
            for(i=0;i<3;i++) {
               Cheb->Coef[i][n] = Block[Start + N*3*Ic + N*i+n];
            }
         }
      }
      /* Moon (geocentric) [441 13 8] */
      Iw = LUNA;
      Nseg = 8;
      Start = 441-1;
      N = 13;
      World[Iw].eph.Ncheb = Nseg;
      World[Iw].eph.Cheb = (struct Cheb3DType *) calloc(Nseg,sizeof(struct Cheb3DType));
      for(Ic=0;Ic<Nseg;Ic++) {
         Cheb = &World[Iw].eph.Cheb[Ic];
         Cheb->JD1 = JD1 + ((double) Ic)*(JD2-JD1)/((double) Nseg);
         Cheb->JD2 = JD2 - ((double) (Nseg-1-Ic))*(JD2-JD1)/((double) Nseg);
         Cheb->N = N;
         for(n=0;n<N;n++) {
            for(i=0;i<3;i++) {
               Cheb->Coef[i][n] = Block[Start + N*3*Ic + N*i+n];
            }
         }
      }
      /* Sun [753 11 2] */
      Iw = SOL;
      Nseg = 2;
      Start = 753-1;
      N = 11;
      World[Iw].eph.Ncheb = Nseg;
      World[Iw].eph.Cheb = (struct Cheb3DType *) calloc(Nseg,sizeof(struct Cheb3DType));
      for(Ic=0;Ic<Nseg;Ic++) {
         Cheb = &World[Iw].eph.Cheb[Ic];
         Cheb->JD1 = JD1 + ((double) Ic)*(JD2-JD1)/((double) Nseg);
         Cheb->JD2 = JD2 - ((double) (Nseg-1-Ic))*(JD2-JD1)/((double) Nseg);
         Cheb->N = N;
         for(n=0;n<N;n++) {
            for(i=0;i<3;i++) {
               Cheb->Coef[i][n] = Block[Start + N*3*Ic + N*i+n];
            }
         }
      }

/* .. Initialize Planetary Pos/Vel */
      for(Iw=SOL;Iw<=LUNA;Iw++) {
         W = &World[Iw];
         Eph = &W->eph;
         /* Determine segment */
         Ic=0;
         while(TT.JulDay > Eph->Cheb[Ic].JD2) Ic++;
         /* Apply Chebyshev polynomials */
         Cheb = &Eph->Cheb[Ic];
         dudJD = 2.0/(Cheb->JD2-Cheb->JD1);
         u = (TT.JulDay-Cheb->JD1)*dudJD - 1.0;
         ChebyPolys(u,Cheb->N,T,U);
         for(i=0;i<3;i++) {
            ChebyInterp(T,U,Cheb->Coef[i],Cheb->N,&P,&dPdu);
            PosJ[i] = 1000.0*P;
            VelJ[i] = 1000.0*dPdu*dudJD/86400.0;
         }
         QTxV(qjh,PosJ,Eph->PosN);
         QTxV(qjh,VelJ,Eph->VelN);
      }
      /* Adjust for barycenters */
      /* Move planets from barycentric to Sun-centered */
      for(Iw=MERCURY;Iw<=PLUTO;Iw++) {
         W = &World[Iw];
         for(i=0;i<3;i++) {
            W->eph.PosN[i] -= World[SOL].eph.PosN[i];
            W->eph.VelN[i] -= World[SOL].eph.VelN[i];
            W->PosH[i] = W->eph.PosN[i];
            W->VelH[i] = W->eph.VelN[i];
         }
         W->PriMerAng = fmod(W->w*DynTime,TwoPi);
         SimpRot(ZAxis,W->PriMerAng,W->CWN);
      }
      /* Move Sun to origin */
      for(i=0;i<3;i++) {
         World[SOL].PosH[i] = 0.0;
         World[SOL].VelH[i] = 0.0;
         World[SOL].eph.PosN[i] = 0.0;
         World[SOL].eph.VelN[i] = 0.0;
      }
      /* Adjust Earth from Earth-Moon barycenter */
      for(i=0;i<3;i++) {
         EarthMoonBaryPosH[i] = World[EARTH].eph.PosN[i];
         EarthMoonBaryVelH[i] = World[EARTH].eph.VelN[i];
         World[EARTH].eph.PosN[i] -= World[LUNA].eph.PosN[i]/EMRAT;
         World[EARTH].eph.VelN[i] -= World[LUNA].eph.VelN[i]/EMRAT;
         World[EARTH].PosH[i] = World[EARTH].eph.PosN[i];
         World[EARTH].VelH[i] = World[EARTH].eph.VelN[i];
      }
      /* Move Moon from barycentric to Earth-centered */
      for(i=0;i<3;i++) {
         rh[i] = World[LUNA].eph.PosN[i]*(1.0+1.0/EMRAT);
         vh[i] = World[LUNA].eph.VelN[i]*(1.0+1.0/EMRAT);
         World[LUNA].PosH[i] = World[EARTH].PosH[i] + rh[i];
         World[LUNA].VelH[i] = World[EARTH].VelH[i] + vh[i];
      }
      /* Rotate Moon into ECI */
      MxV(World[EARTH].CNH,rh,World[LUNA].eph.PosN);
      MxV(World[EARTH].CNH,vh,World[LUNA].eph.VelN);
      World[LUNA].PriMerAng = LunaPriMerAng(TT.JulDay);
      SimpRot(ZAxis,World[LUNA].PriMerAng,World[LUNA].CWN);
      
      for(Iw=MERCURY;Iw<=LUNA;Iw++) {
         Eph = &World[Iw].eph;
         RV2Eph(DynTime,Eph->mu,Eph->PosN,Eph->VelN,
                &Eph->SMA,&Eph->ecc,&Eph->inc,
                &Eph->RAAN,&Eph->ArgP,&Eph->anom,
                &Eph->tp,&Eph->SLR,&Eph->alpha,&Eph->rmin,
                &Eph->MeanMotion,&Eph->Period);
      }

      return(0);
}
/**********************************************************************/
void LoadConstellations(void) {

      FILE *infile;
      char response[121];
      double RA,Dec;
      long i,j;
      struct ConstellationType *C;

      infile=FileOpen(ModelPath,"Constellations.txt","r");

      for (i=0;i<89;i++) {
         C = &Constell[i];
         ScanLine(infile,"%4s %120s %ld %ld",4,C->Tag,response,&C->Nstars,&C->Nlines);
         C->Class = DecodeString(response);

         C->StarVec = CreateMatrix(C->Nstars,3);

         C->Star1 = (long *) calloc(C->Nlines,sizeof(long));
         C->Star2 = (long *) calloc(C->Nlines,sizeof(long));
         
         for (j=0; j<C->Nstars; j++) {
            ScanLine(infile,"%lf %lf",2,&RA,&Dec);
            RA *= D2R;
            Dec *= D2R;
            C->StarVec[j][0] = cos(RA)*cos(Dec);
            C->StarVec[j][1] = sin(RA)*cos(Dec);
            C->StarVec[j][2] = sin(Dec);
         }

         for (j=0; j<C->Nlines; j++) {
            ScanLine(infile,"%ld %ld",2,&C->Star1[j],&C->Star2[j]);                  
         }
      }
      
      fclose(infile);
}
/**********************************************************************/
void LoadSchatten(void)
{
      FILE *infile;
      long i,fileyear,filemonth;
      
      infile = FileOpen(ModelPath,"SolFlx0908_Schatten.txt","rt");

      ScanLine(infile,"",0,NULL);
      ScanLine(infile,"",0,NULL);
      for(i=0;i<409;i++) {
         ScanLine(infile,"%ld %ld %lf %lf %lf %lf",6,
            &fileyear,&filemonth,
            &SchattenTable[1][i],&SchattenTable[2][i],
            &SchattenTable[3][i],&SchattenTable[4][i]);
         SchattenTable[0][i] = DateToJD(fileyear,filemonth,01,12,00,00);
      }
      fclose(infile);
}
/**********************************************************************/
void InitSim(int argc, char **argv)
{
      FILE *infile;
      struct OrbitType *Eph;
      char response[120],response1[120],response2[120];
      double r1[3],rh[3],vh[3];
      double Zaxis[3] = {0.0,0.0,1.0};
      long Iorb,Isc,i,j,Ip,Im,Iw,Nm;
      long MinorBodiesExist;
      long JunkTag;
      double CGJ[3][3] =
         {{-0.054873956175539,-0.873437182224835,-0.483835031431981},
          { 0.494110775064704,-0.444828614979805, 0.746981957785302},
          {-0.867665382947348,-0.198076649977489, 0.455985113757595}};
      double CJH[3][3];
      
      Pi = 4.0*atan(1.0);
      TwoPi = 2.0*Pi;
      HalfPi = 0.5*Pi;
      SqrtTwo = sqrt(2.0);
      SqrtHalf = sqrt(0.5);
      R2D = 180.0/Pi;
      D2R = Pi/180.0;
      A2R = D2R/3600.0;
      R2A = R2D*3600.0;
      GoldenRatio = (1.0+sqrt(5.0))/2.0;
      
      qjh[0] = -0.203123038887;
      qjh[1] = 0.0;
      qjh[2] = 0.0;
      qjh[3] = 0.979153221449;
      
      sprintf(InOutPath,"InOut/");
      sprintf(ModelPath,"Model/");
      if (argc > 1) sprintf(InOutPath,"%s/",argv[1]);
      if (argc > 2) sprintf(ModelPath,"%s/",argv[2]);

/* .. Read from file Inp_Sim.txt */
      infile=FileOpen(InOutPath,"Inp_Sim.txt","r");

      ScanLine(infile,"",0,NULL);
      ScanLine(infile,"",0,NULL);
/* .. Time Mode */
      ScanLine(infile,"%120s",1,response);
      TimeMode = DecodeString(response);
/* .. Duration, Step size */
      ScanLine(infile,"%lf %lf",2,&STOPTIME,&DTSIM);
/* .. File output interval */
      ScanLine(infile,"%lf",1,&DTOUT);
/* .. RNG Seed */
      ScanLine(infile,"%ld",1,&RngSeed);
/* .. Graphics Front End? */
      ScanLine(infile,"%120s",1,response);
      GLEnable = DecodeString(response);
/* .. Cmd Script File Name */
      ScanLine(infile,"%80s",1,CmdFileName);

/* .. Reference Orbits */
      ScanLine(infile,"",0,NULL);
      ScanLine(infile,"%ld",1,&Norb);
      Orb = NULL;
      Orb = (struct OrbitType *) calloc(Norb,sizeof(struct OrbitType));
      if (Orb == NULL) {
         printf("Orb calloc returned null pointer.  Bailing out!\n");
         exit(1);
      }
      Frm = NULL;
      Frm = (struct FormationType *) calloc(Norb,sizeof(struct FormationType));
      if (Frm == NULL) {
         printf("Frm calloc returned null pointer.  Bailing out!\n");
         exit(1);
      }
      for(Iorb=0;Iorb<Norb;Iorb++){
         ScanLine(infile,"%120s %120s",2,response,Orb[Iorb].FileName);
         Orb[Iorb].Exists = DecodeString(response);
         Orb[Iorb].Tag = Iorb;
      }

/* .. Spacecraft */
      ScanLine(infile,"",0,NULL);
      ScanLine(infile,"%ld",1,&Nsc);
      SC = NULL;
      SC = (struct SCType *) calloc(Nsc,sizeof(struct SCType));
      if (SC == NULL) {
         printf("SC calloc returned null pointer.  Bailing out!\n");
         exit(1);
      }
      for(Isc=0;Isc<Nsc;Isc++){
         ScanLine(infile,"%120s  %ld %40s",3,response,
            &SC[Isc].RefOrb,SC[Isc].FileName);
         SC[Isc].Exists=DecodeString(response);
         SC[Isc].ID = Isc;
         if ((SC[Isc].Exists && !Orb[SC[Isc].RefOrb].Exists) || (SC[Isc].RefOrb > Norb)) {
            printf("Yo!  SC[%ld] is assigned to non-existent Orb[%ld]\n",
               Isc,SC[Isc].RefOrb);
            exit(1);
         }
      }
/* .. Environment */
      ScanLine(infile,"",0,NULL);
/* .. Date and time (UTC) */
      ScanLine(infile,"%ld %ld %ld",3,&UTC.Month,&UTC.Day,&UTC.Year);
      ScanLine(infile,"%ld %ld %lf",3,&UTC.Hour,&UTC.Minute,&UTC.Second);
      ScanLine(infile,"%lf",1,&LeapSec);
/* .. Choices for Modeling Solar Activity */
      ScanLine(infile,"%120s",1,response);
      AtmoOption=DecodeString(response);
      ScanLine(infile,"%lf",1,&Flux10p7);
      ScanLine(infile,"%lf",1,&GeomagIndex);
/* .. Magnetic Field Model */
      ScanLine(infile,"%120s",1,response);
         MagModel.Type = DecodeString(response);
      ScanLine(infile,"%ld %ld",2,&MagModel.N,&MagModel.M);
/* .. Earth Gravity Model */
      ScanLine(infile,"%ld %ld",2,
         &EarthGravModel.N,&EarthGravModel.M);
/* .. Mars Gravity Model */
      ScanLine(infile,"%ld %ld",2,
         &MarsGravModel.N,&MarsGravModel.M);
/* .. Luna Gravity Model */
      ScanLine(infile,"%ld %ld",2,
         &LunaGravModel.N,&LunaGravModel.M);
/* .. Toggle on/off various environmental effects */
      ScanLine(infile,"%120s  %120s",2,response1,response2);
      AeroActive=DecodeString(response1);
      AeroShadowsActive = DecodeString(response2);
      ScanLine(infile,"%120s",1,response);
      GGActive=DecodeString(response);
      ScanLine(infile,"%120s %120s",2,response1,response2);
      SolPressActive=DecodeString(response1);
      SolPressShadowsActive=DecodeString(response2);
      ScanLine(infile,"%120s",1,response);
      ResidualDipoleActive=DecodeString(response);
      ScanLine(infile,"%120s",1,response);
      GravPertActive=DecodeString(response);
      ScanLine(infile,"%120s",1,response);
      ThrusterPlumesActive=DecodeString(response);
      ScanLine(infile,"%120s",1,response);
      ContactActive=DecodeString(response);
      ScanLine(infile,"%120s",1,response);
      SloshActive=DecodeString(response);
      ScanLine(infile,"%120s",1,response);
      AlbedoActive=DecodeString(response);
      ScanLine(infile,"%120s",1,response);
      ComputeEnvTrq=DecodeString(response);
/* .. Celestial Bodies */
      ScanLine(infile,"",0,NULL);
      ScanLine(infile,"%120s",1,response);
      EphemOption = DecodeString(response);
      for(i=MERCURY;i<=PLUTO;i++){
         ScanLine(infile,"%120s",1,response);
         World[i].Exists=DecodeString(response);
      }
      ScanLine(infile,"%120s",1,response);
      MinorBodiesExist=DecodeString(response);

/* .. Lagrange Point Systems */
      ScanLine(infile,"",0,NULL);
      for(i=0;i<3;i++){
         ScanLine(infile,"%120s",1,response);
         LagSys[i].Exists=DecodeString(response);
      }

/* .. Ground Stations */
      ScanLine(infile,"",0,NULL);
      ScanLine(infile,"%ld",1,&Ngnd);
      GroundStation = (struct GroundStationType *) calloc(Ngnd,sizeof(struct GroundStationType));
      for(i=0;i<Ngnd;i++) {
         ScanLine(infile,"%120s %120s %lf %lf \"%40[^\"]\"",5,
            response1,response2,&GroundStation[i].lng,&GroundStation[i].lat,
            GroundStation[i].Label);
         GroundStation[i].Exists = DecodeString(response1);
         GroundStation[i].World = DecodeString(response2);
      }

      fclose(infile);
      
/* .. Load Materials */
      Nmatl = 0;
      Matl = AddMtlLib(ModelPath,"42.mtl",Matl,&Nmatl);
      ScaleSpecDiffFrac(Matl,Nmatl);

      /* Known bug: First Mesh loaded in gets corrupted.
      Kludge fix: Load a sacrificial mesh first.  */
      Mesh = LoadWingsObjFile(ModelPath,"Point.obj",
            &Matl,&Nmatl,Mesh,&Nmesh,&JunkTag,FALSE);

/* .. Time */
      if (TimeMode == EXTERNAL_TIME) {
         printf("Initializing with External Time\n");
         RealSystemTime(&UTC.Year,&UTC.doy,&UTC.Month,&UTC.Day,
            &UTC.Hour,&UTC.Minute,&UTC.Second,DTSIM);
      }         
      CivilTime = DateToTime(UTC.Year,UTC.Month,UTC.Day,
         UTC.Hour,UTC.Minute,UTC.Second);
      AtomicTime = CivilTime + LeapSec;
      DynTime0 = AtomicTime + 32.184;
      GpsTime = AtomicTime - 19.0;
      DynTime = DynTime0;

      TT.JulDay = TimeToJD(DynTime);
      TimeToDate(DynTime,&TT.Year,&TT.Month,&TT.Day,
         &TT.Hour,&TT.Minute,&TT.Second,DTSIM);
      TT.doy = MD2DOY(TT.Year,TT.Month,TT.Day);
      
      UTC.JulDay = TimeToJD(CivilTime);
      UTC.doy = MD2DOY(UTC.Year,UTC.Month,UTC.Day);
      
      GpsTimeToGpsDate(GpsTime,&GpsRollover,&GpsWeek,&GpsSecond);

/* .. Load Sun and Planets */
      LoadSun();
      LoadPlanets();
      /* JPL planetary ephems */
      if (EphemOption == EPH_DE430 || EphemOption == EPH_DE440) 
         LoadJplEphems(ModelPath,TT.JulDay);

/* .. Load Moons */
      if (World[EARTH].Exists) LoadMoonOfEarth();
      if (World[MARS].Exists) LoadMoonsOfMars();
      if (World[JUPITER].Exists) LoadMoonsOfJupiter();
      if (World[SATURN].Exists) LoadMoonsOfSaturn();
      if (World[URANUS].Exists) LoadMoonsOfUranus();
      if (World[NEPTUNE].Exists) LoadMoonsOfNeptune();
      if (World[PLUTO].Exists) LoadMoonsOfPluto();

/* .. Asteroids and Comets */
      if (MinorBodiesExist) LoadMinorBodies();
      else Nmb = 0;

/* .. Regions */
      LoadRegions();

/* .. Galactic Frame */
      Q2C(qjh,CJH);
      MxM(CGJ,CJH,CGH);

/* .. Ground Station Locations */
      for(i=0;i<Ngnd;i++) {
         if (GroundStation[i].Exists && !World[GroundStation[i].World].Exists) {
            printf("Ground Station[%ld].World doesn't exist.\n",i);
         }
         GroundStation[i].PosW[0] = World[GroundStation[i].World].rad*
            cos(GroundStation[i].lng*D2R)*cos(GroundStation[i].lat*D2R);
         GroundStation[i].PosW[1] = World[GroundStation[i].World].rad*
            sin(GroundStation[i].lng*D2R)*cos(GroundStation[i].lat*D2R);
         GroundStation[i].PosW[2] = World[GroundStation[i].World].rad*
            sin(GroundStation[i].lat*D2R);
      }

/* .. Locate Luna */
      if (World[LUNA].Exists){
         Eph = &World[LUNA].eph;
         /* Meeus computes Luna Position in geocentric ecliptic */
         LunaPosition(TT.JulDay,rh);
         LunaPosition(TT.JulDay+0.01,r1);
         for(j=0;j<3;j++) vh[j] = (r1[j]-rh[j])/(864.0);
         /* Convert to Earth's N frame */
         MxV(World[EARTH].CNH,rh,Eph->PosN);
         MxV(World[EARTH].CNH,vh,Eph->VelN);
         /* Find Luna's osculating elements */
         RV2Eph(DynTime,Eph->mu,Eph->PosN,Eph->VelN,
            &Eph->SMA,&Eph->ecc,&Eph->inc,&Eph->RAAN,
            &Eph->ArgP,&Eph->anom,&Eph->tp,&Eph->SLR,&Eph->alpha,
            &Eph->rmin,&Eph->MeanMotion,&Eph->Period);
         World[LUNA].PriMerAng =
            atan2(Eph->PosN[1],Eph->PosN[0])+Pi;
         SimpRot(Zaxis,World[LUNA].PriMerAng,World[LUNA].CWN);
         C2Q(World[LUNA].CWN,World[LUNA].qwn);
         for(j=0;j<3;j++) {
            World[LUNA].PosH[j] = rh[j] + World[EARTH].PosH[j];
            World[LUNA].VelH[j] = vh[j] + World[EARTH].VelH[j];
         }
      }

/* .. Other planets' moons */
      for(Ip=MARS;Ip<=PLUTO;Ip++) {
         if (World[Ip].Exists) {
            Nm = World[Ip].Nsat;
            for(Im=0;Im<Nm;Im++) {
               Iw = World[Ip].Sat[Im];
               Eph = &World[Iw].eph;
               Eph2RV(Eph->mu, Eph->SLR,Eph->ecc,Eph->inc,Eph->RAAN,
                      Eph->ArgP,DynTime-Eph->tp,Eph->PosN,Eph->VelN,&Eph->anom);
               World[Iw].PriMerAng = fmod(World[i].w*DynTime,TwoPi);
               SimpRot(Zaxis,World[Iw].PriMerAng,World[Iw].CWN);
               C2Q(World[Iw].CWN,World[Iw].qwn);
               MTxV(World[Ip].CNH,Eph->PosN,rh);
               MTxV(World[Ip].CNH,Eph->VelN,vh);
               for(i=0;i<3;i++) {
                  World[Iw].PosH[i] = rh[i] + World[Ip].PosH[i];
                  World[Iw].VelH[i] = vh[i] + World[Ip].VelH[i];
               }
            }
         }
      }

/* .. Note that some moons are so dominated by their planet that   */
/*    they don't really have a sphere of influence!                */
      /*
      for(Iw=1;Iw<NWORLD;Iw++) {
         if (0.5*World[Iw].RadOfInfluence < World[Iw].rad)
            printf("World %s is bigger than its inner sphere of influence\n",World[Iw].Name);
         if (2.0*World[Iw].RadOfInfluence < World[Iw].rad)
            printf("World %s is bigger than its outer sphere of influence\n",World[Iw].Name);
      }
      */

      InitLagrangePoints();
      for(Iorb=0;Iorb<Norb;Iorb++) {
         if (Orb[Iorb].Exists) InitOrbit(&Orb[Iorb]);
      }
      OrbitMotion(DynTime);
      for (Isc=0;Isc<Nsc;Isc++) {
         if (SC[Isc].Exists) {
            InitSpacecraft(&SC[Isc]);
         }
      }

      LoadTdrs();
      
      RNG = CreateRandomProcess(RngSeed);

      LoadConstellations();
      
      LoadSchatten();
}

/* #ifdef __cplusplus
** }
** #endif
*/

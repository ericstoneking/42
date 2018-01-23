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

/* #ifdef __cplusplus
** namespace _42 {
** using namespace Kit;
** #endif
*/

long FswCmdInterpreter(char CmdLine[512], double *CmdTime);
#ifdef _USE_GUI_
long GuiCmdInterpreter(char CmdLine[512], double *CmdTime);
#endif

/**********************************************************************/
long SimCmdInterpreter(char CmdLine[512],double *CmdTime)
{
      char response[80];
      long NewCmdProcessed = FALSE;
      long Isc,Ig,Idof;
      double Val;
      long Iorb;
      char DvFrame;
      double Vec[3], DVN[3];
      struct OrbitType *O;

      if (sscanf(CmdLine,"%lf SC[%ld].RotDOF %s",
         CmdTime,&Isc,response) == 3) {
         NewCmdProcessed = TRUE;
         SC[Isc].RotDOF = DecodeString(response);
      }

      if (sscanf(CmdLine,"%lf DTSIM = %lf",
         CmdTime,&Val) == 2) {
         NewCmdProcessed = TRUE;
         DTSIM = Val;
      }

      if (sscanf(CmdLine,"%lf SC[%ld].G[%ld].RotLocked[%ld] %s",
         CmdTime,&Isc,&Ig,&Idof,response) == 5) {
         NewCmdProcessed = TRUE;
         SC[Isc].G[Ig].RotLocked[Idof] = DecodeString(response);
      }

      if (sscanf(CmdLine,"%lf SC[%ld].G[%ld].TrnLocked[%ld] %s",
         CmdTime,&Isc,&Ig,&Idof,response) == 5) {
         NewCmdProcessed = TRUE;
         SC[Isc].G[Ig].TrnLocked[Idof] = DecodeString(response);
      }

      if (sscanf(CmdLine,"%lf Impart Impulsive Delta-V of [%lf %lf %lf] m/s in Frame %c to Orb[%ld]",
         CmdTime,&Vec[0],&Vec[1],&Vec[2],&DvFrame,&Iorb) == 6) {
         NewCmdProcessed = TRUE;
         O = &Orb[Iorb];
         if (DvFrame == 'L') {
            MTxV(O->CLN,Vec,DVN);
            O->VelN[0] += DVN[0];
            O->VelN[1] += DVN[1];
            O->VelN[2] += DVN[2];
         }
         else if (DvFrame == 'N') {
            O->VelN[0] += Vec[0];
            O->VelN[1] += Vec[1];
            O->VelN[2] += Vec[2];
         }
         else {
            printf("Bogus DvFrame %c in SimCmdInterpreter\n",DvFrame);
            exit(1);
         }
         RV2Eph(AbsTime,O->mu,O->PosN,O->VelN,
                &O->SMA,&O->ecc,&O->inc,
                &O->RAAN,&O->ArgP,&O->anom,
                &O->tp,&O->SLR,&O->alpha,&O->rmin,
                &O->MeanMotion,&O->Period);
      }

      return(NewCmdProcessed);
}
/**********************************************************************/
void CmdInterpreter(void)
{
      static FILE *CmdFile;
      static char CmdLine[512];
      static double CmdTime;
      long NewCmdProcessed;

      static long First = 1;
      static long CmdFileActive = 1;

      if (First) {
         First = 0;
         if (!strcmp(CmdFileName,"NONE"))
            CmdFileActive = 0;
         else {
            CmdFile = FileOpen(InOutPath,CmdFileName,"rt");
            fgets(CmdLine,512,CmdFile);
            fgets(CmdLine,512,CmdFile);
            sscanf(CmdLine,"%lf",&CmdTime);
            if (!strncmp(CmdLine,"EOF",3)) {
               fclose(CmdFile);
               printf("Reached CmdScript EOF at Time = %lf\n",SimTime);
               CmdFileActive = 0;
            }
         }
      }

      while (CmdTime <= SimTime && CmdFileActive) {
         NewCmdProcessed = FALSE;
         /* Look for known command patterns */

         /* Skip Comment Lines */
         if (!strncmp(CmdLine," ",1) ||
             !strncmp(CmdLine,"//",2) ||
             !strncmp(CmdLine,"#",1) ||
             !strncmp(CmdLine,"\n",1) ||
             !strncmp(CmdLine,"%",1)) NewCmdProcessed = TRUE;

         /* Sim Commands */
         else if (SimCmdInterpreter(CmdLine,&CmdTime))
            NewCmdProcessed = TRUE;

         /* Visualization Commands */
         #ifdef _USE_GUI_
            else if (GuiCmdInterpreter(CmdLine,&CmdTime))
               NewCmdProcessed = TRUE;
         #endif

         /* FSW Commands */
         else if (FswCmdInterpreter(CmdLine,&CmdTime))
            NewCmdProcessed = TRUE;

         /* If any match found, get next command */
         if (NewCmdProcessed) {
            printf("%s",CmdLine);
            fgets(CmdLine,512,CmdFile);
            sscanf(CmdLine,"%lf",&CmdTime);
            if (!strncmp(CmdLine,"EOF",3)) {
               fclose(CmdFile);
               printf("Reached CmdScript EOF at Time = %lf\n",SimTime);
               CmdFileActive = 0;
            }
            fflush(stdout);
         }
      }
}

/* #ifdef __cplusplus
** }
** #endif
*/


#include "42.h"

/**********************************************************************/
void ReadFromCmd(void)
{

      struct SCType *S;
      struct OrbitType *O;
      struct DynType *D;
      long Isc,Iorb,Iw,i;
      char line[512] = "Blank";
      long RequestTimeRefresh = 0;
      long Done;
      double DbleVal[30];
      long LongVal[30];

      /* Placeholder */
         if (EchoEnabled) printf("%s",line);

         if (sscanf(line,"TIME %ld-%ld-%ld:%ld:%lf\n",
            &Year,&doy,&Hour,&Minute,&Second) == 5)
            RequestTimeRefresh = 1;

         if (sscanf(line,"SC[%ld].PosR = [ %le %le %le]",
            &Isc,
            &DbleVal[0],
            &DbleVal[1],
            &DbleVal[2]) == 4) {
            SC[Isc].PosR[0] = DbleVal[0];
            SC[Isc].PosR[1] = DbleVal[1];
            SC[Isc].PosR[2] = DbleVal[2];
            SC[Isc].RequestStateRefresh = 1;
         }

         if (sscanf(line,"SC[%ld].VelR = [ %le %le %le]",
            &Isc,
            &DbleVal[0],
            &DbleVal[1],
            &DbleVal[2]) == 4) {
            SC[Isc].VelR[0] = DbleVal[0];
            SC[Isc].VelR[1] = DbleVal[1];
            SC[Isc].VelR[2] = DbleVal[2];
            SC[Isc].RequestStateRefresh = 1;
         }

         if (sscanf(line,"SC[%ld].svb = [ %le %le %le]",
            &Isc,
            &DbleVal[0],
            &DbleVal[1],
            &DbleVal[2]) == 4) {
            SC[Isc].svb[0] = DbleVal[0];
            SC[Isc].svb[1] = DbleVal[1];
            SC[Isc].svb[2] = DbleVal[2];
            SC[Isc].RequestStateRefresh = 1;
         }

         if (sscanf(line,"SC[%ld].bvb = [ %le %le %le]",
            &Isc,
            &DbleVal[0],
            &DbleVal[1],
            &DbleVal[2]) == 4) {
            SC[Isc].bvb[0] = DbleVal[0];
            SC[Isc].bvb[1] = DbleVal[1];
            SC[Isc].bvb[2] = DbleVal[2];
            SC[Isc].RequestStateRefresh = 1;
         }

         if (sscanf(line,"SC[%ld].Hvb = [ %le %le %le]",
            &Isc,
            &DbleVal[0],
            &DbleVal[1],
            &DbleVal[2]) == 4) {
            SC[Isc].Hvb[0] = DbleVal[0];
            SC[Isc].Hvb[1] = DbleVal[1];
            SC[Isc].Hvb[2] = DbleVal[2];
            SC[Isc].RequestStateRefresh = 1;
         }

         if (sscanf(line,"World[%ld].PosH = [ %le %le %le]",
            &Iw,
            &DbleVal[0],
            &DbleVal[1],
            &DbleVal[2]) == 4) {
            World[Iw].PosH[0] = DbleVal[0];
            World[Iw].PosH[1] = DbleVal[1];
            World[Iw].PosH[2] = DbleVal[2];
         }

         if (sscanf(line,"Orb[%ld].PosN = [ %le %le %le]",
            &Iorb,
            &DbleVal[0],
            &DbleVal[1],
            &DbleVal[2]) == 4) {
            Orb[Iorb].PosN[0] = DbleVal[0];
            Orb[Iorb].PosN[1] = DbleVal[1];
            Orb[Iorb].PosN[2] = DbleVal[2];
         }

         if (sscanf(line,"Orb[%ld].VelN = [ %le %le %le]",
            &Iorb,
            &DbleVal[0],
            &DbleVal[1],
            &DbleVal[2]) == 4) {
            Orb[Iorb].VelN[0] = DbleVal[0];
            Orb[Iorb].VelN[1] = DbleVal[1];
            Orb[Iorb].VelN[2] = DbleVal[2];
         }

         for(Isc=0;Isc<Nsc;Isc++) {
            if (SC[Isc].Exists) {
               if (SC[Isc].AC.ParmDumpEnabled) {
               }
            }
         }


         if (!strncmp(line,"[EOF]",5)) {
            Done = 1;
            sprintf(line,"[EOF] reached\n");
         }
      }

      if (RequestTimeRefresh) {
         /* Update AbsTime, SimTime, etc */
         DOY2MD(Year,doy,&Month,&Day);
         AbsTime = DateToAbsTime(Year,Month,Day,Hour,Minute,Second);
         JulDay = AbsTimeToJD(AbsTime);
         JDToGpsTime(JulDay,&GpsRollover,&GpsWeek,&GpsSecond);
         SimTime = AbsTime-AbsTime0;
      }


/* .. Refresh SC states that depend on inputs */

      for(Isc=0;Isc<Nsc;Isc++) {
         if (SC[Isc].RequestStateRefresh) {
            S = &SC[Isc];
            S->RequestStateRefresh = 0;
            if (S->Exists) {
               /* Update  RefOrb */
               O = &Orb[S->RefOrb];
               O->Epoch = AbsTime;
               for(i=0;i<3;i++) {
                  S->PosN[i] = O->PosN[i] + S->PosR[i];
                  S->VelN[i] = O->VelN[i] + S->VelR[i];
               }
               RV2Eph(O->Epoch,O->mu,O->PosN,O->VelN,
                  &O->SMA,&O->ecc,&O->inc,&O->RAAN,
                  &O->ArgP,&O->anom,&O->tp,
                  &O->SLR,&O->alpha,&O->rmin,
                  &O->MeanMotion,&O->Period);
               FindCLN(O->PosN,O->VelN,O->CLN,O->wln);

               /* Update Dyn */
               MapJointStatesToStateVector(S);
               D = &S->Dyn;
               MapStateVectorToBodyStates(D->u,D->x,D->uf,D->xf,S);
               MotionConstraints(S);
            }
         }
      }
}

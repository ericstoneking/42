#include "42.h"
#define EXTERN extern
#include "Ac.h"
#undef EXTERN

/******************************************************************************/
void WriteToSocket(SOCKET Socket,  char **Prefix, long Nprefix, long EchoEnabled)
{
      struct SCType *S;
      struct WorldType *W;
      struct OrbitType *O;
      int Success;
      char Ack[4] = "Ack\0";
      long Is,Ipfx;
      char Msg[16384];
      long MsgLen = 0;
      long LineLen,PfxLen;
      char line[512];
      long k;

      sprintf(line,"TIME %ld-%03ld-%02ld:%02ld:%012.9lf\n",
         UTC.Year,UTC.doy,UTC.Hour,UTC.Minute,UTC.Second);
      LineLen = strlen(line);
      memcpy(&Msg[MsgLen],line,LineLen);
      MsgLen += LineLen;
      if (EchoEnabled) printf("%s",line);

      for(Ipfx=0;Ipfx<Nprefix;Ipfx++) {
         PfxLen = strlen(Prefix[Ipfx]);

         for(Is=0;Is<Nsc;Is++) {
            if (SC[Is].Exists) {
               S = &SC[Is];

               sprintf(line,"SC[%ld].qn = [%18.12le %18.12le %18.12le %18.12le]\n",Is,
                  S->qn[0],S->qn[1],S->qn[2],S->qn[3]);
               if (!strncmp(line,Prefix[Ipfx],PfxLen)) {
                  LineLen = strlen(line);
                  memcpy(&Msg[MsgLen],line,LineLen);
                  MsgLen += LineLen;
                  if (EchoEnabled) printf("%s",line);
               }

               sprintf(line,"SC[%ld].wn = [%18.12le %18.12le %18.12le]\n",Is,
                  S->wn[0],S->wn[1],S->wn[2]);
               if (!strncmp(line,Prefix[Ipfx],PfxLen)) {
                  LineLen = strlen(line);
                  memcpy(&Msg[MsgLen],line,LineLen);
                  MsgLen += LineLen;
                  if (EchoEnabled) printf("%s",line);
               }

               sprintf(line,"SC[%ld].PosR = [%18.12le %18.12le %18.12le]\n",Is,
                  S->PosR[0],S->PosR[1],S->PosR[2]);
               if (!strncmp(line,Prefix[Ipfx],PfxLen)) {
                  LineLen = strlen(line);
                  memcpy(&Msg[MsgLen],line,LineLen);
                  MsgLen += LineLen;
                  if (EchoEnabled) printf("%s",line);
               }

               sprintf(line,"SC[%ld].VelR = [%18.12le %18.12le %18.12le]\n",Is,
                  S->VelR[0],S->VelR[1],S->VelR[2]);
               if (!strncmp(line,Prefix[Ipfx],PfxLen)) {
                  LineLen = strlen(line);
                  memcpy(&Msg[MsgLen],line,LineLen);
                  MsgLen += LineLen;
                  if (EchoEnabled) printf("%s",line);
               }

               sprintf(line,"SC[%ld].svb = [%18.12le %18.12le %18.12le]\n",Is,
                  S->svb[0],S->svb[1],S->svb[2]);
               if (!strncmp(line,Prefix[Ipfx],PfxLen)) {
                  LineLen = strlen(line);
                  memcpy(&Msg[MsgLen],line,LineLen);
                  MsgLen += LineLen;
                  if (EchoEnabled) printf("%s",line);
               }

               sprintf(line,"SC[%ld].bvb = [%18.12le %18.12le %18.12le]\n",Is,
                  S->bvb[0],S->bvb[1],S->bvb[2]);
               if (!strncmp(line,Prefix[Ipfx],PfxLen)) {
                  LineLen = strlen(line);
                  memcpy(&Msg[MsgLen],line,LineLen);
                  MsgLen += LineLen;
                  if (EchoEnabled) printf("%s",line);
               }

               sprintf(line,"SC[%ld].Hvb = [%18.12le %18.12le %18.12le]\n",Is,
                  S->Hvb[0],S->Hvb[1],S->Hvb[2]);
               if (!strncmp(line,Prefix[Ipfx],PfxLen)) {
                  LineLen = strlen(line);
                  memcpy(&Msg[MsgLen],line,LineLen);
                  MsgLen += LineLen;
                  if (EchoEnabled) printf("%s",line);
               }

               for(k=0;k<S->Nb;k++) {
                  sprintf(line,"SC[%ld].B[%ld].wn = [%18.12le %18.12le %18.12le]\n",Is,k,
                     S->B[k].wn[0],S->B[k].wn[1],S->B[k].wn[2]);
                  if (!strncmp(line,Prefix[Ipfx],PfxLen)) {
                     LineLen = strlen(line);
                     memcpy(&Msg[MsgLen],line,LineLen);
                     MsgLen += LineLen;
                     if (EchoEnabled) printf("%s",line);
                  }
               }

               for(k=0;k<S->Nb;k++) {
                  sprintf(line,"SC[%ld].B[%ld].qn = [%18.12le %18.12le %18.12le %18.12le]\n",Is,k,
                     S->B[k].qn[0],S->B[k].qn[1],S->B[k].qn[2],S->B[k].qn[3]);
                  if (!strncmp(line,Prefix[Ipfx],PfxLen)) {
                     LineLen = strlen(line);
                     memcpy(&Msg[MsgLen],line,LineLen);
                     MsgLen += LineLen;
                     if (EchoEnabled) printf("%s",line);
                  }
               }

               for(k=0;k<S->Ng;k++) {
                  sprintf(line,"SC[%ld].G[%ld].Pos = [%18.12le %18.12le %18.12le]\n",Is,k,
                     S->G[k].Pos[0],S->G[k].Pos[1],S->G[k].Pos[2]);
                  if (!strncmp(line,Prefix[Ipfx],PfxLen)) {
                     LineLen = strlen(line);
                     memcpy(&Msg[MsgLen],line,LineLen);
                     MsgLen += LineLen;
                     if (EchoEnabled) printf("%s",line);
                  }
               }

               for(k=0;k<S->Ng;k++) {
                  sprintf(line,"SC[%ld].G[%ld].PosRate = [%18.12le %18.12le %18.12le]\n",Is,k,
                     S->G[k].PosRate[0],S->G[k].PosRate[1],S->G[k].PosRate[2]);
                  if (!strncmp(line,Prefix[Ipfx],PfxLen)) {
                     LineLen = strlen(line);
                     memcpy(&Msg[MsgLen],line,LineLen);
                     MsgLen += LineLen;
                     if (EchoEnabled) printf("%s",line);
                  }
               }

               for(k=0;k<S->Ng;k++) {
                  sprintf(line,"SC[%ld].G[%ld].Ang = [%18.12le %18.12le %18.12le]\n",Is,k,
                     S->G[k].Ang[0],S->G[k].Ang[1],S->G[k].Ang[2]);
                  if (!strncmp(line,Prefix[Ipfx],PfxLen)) {
                     LineLen = strlen(line);
                     memcpy(&Msg[MsgLen],line,LineLen);
                     MsgLen += LineLen;
                     if (EchoEnabled) printf("%s",line);
                  }
               }

               for(k=0;k<S->Ng;k++) {
                  sprintf(line,"SC[%ld].G[%ld].AngRate = [%18.12le %18.12le %18.12le]\n",Is,k,
                     S->G[k].AngRate[0],S->G[k].AngRate[1],S->G[k].AngRate[2]);
                  if (!strncmp(line,Prefix[Ipfx],PfxLen)) {
                     LineLen = strlen(line);
                     memcpy(&Msg[MsgLen],line,LineLen);
                     MsgLen += LineLen;
                     if (EchoEnabled) printf("%s",line);
                  }
               }

               for(k=0;k<S->Nw;k++) {
                  sprintf(line,"SC[%ld].Whl[%ld].H = %18.12le\n",Is,k,
                     S->Whl[k].H);
                  if (!strncmp(line,Prefix[Ipfx],PfxLen)) {
                     LineLen = strlen(line);
                     memcpy(&Msg[MsgLen],line,LineLen);
                     MsgLen += LineLen;
                     if (EchoEnabled) printf("%s",line);
                  }
               }

               for(k=0;k<S->Ngyro;k++) {
                  sprintf(line,"SC[%ld].Gyro[%ld].TrueRate = %18.12le\n",Is,k,
                     S->Gyro[k].TrueRate);
                  if (!strncmp(line,Prefix[Ipfx],PfxLen)) {
                     LineLen = strlen(line);
                     memcpy(&Msg[MsgLen],line,LineLen);
                     MsgLen += LineLen;
                     if (EchoEnabled) printf("%s",line);
                  }
               }
            }
         }

         for(Is=0;Is<NWORLD;Is++) {
            if (World[Is].Exists) {
               W = &World[Is];

               sprintf(line,"World[%ld].PosH = [%18.12le %18.12le %18.12le]\n",Is,
                  W->PosH[0],W->PosH[1],W->PosH[2]);
               if (!strncmp(line,Prefix[Ipfx],PfxLen)) {
                  LineLen = strlen(line);
                  memcpy(&Msg[MsgLen],line,LineLen);
                  MsgLen += LineLen;
                  if (EchoEnabled) printf("%s",line);
               }

               sprintf(line,"World[%ld].eph.PosN = [%18.12le %18.12le %18.12le]\n",Is,
                  W->eph.PosN[0],W->eph.PosN[1],W->eph.PosN[2]);
               if (!strncmp(line,Prefix[Ipfx],PfxLen)) {
                  LineLen = strlen(line);
                  memcpy(&Msg[MsgLen],line,LineLen);
                  MsgLen += LineLen;
                  if (EchoEnabled) printf("%s",line);
               }

               sprintf(line,"World[%ld].eph.VelN = [%18.12le %18.12le %18.12le]\n",Is,
                  W->eph.VelN[0],W->eph.VelN[1],W->eph.VelN[2]);
               if (!strncmp(line,Prefix[Ipfx],PfxLen)) {
                  LineLen = strlen(line);
                  memcpy(&Msg[MsgLen],line,LineLen);
                  MsgLen += LineLen;
                  if (EchoEnabled) printf("%s",line);
               }
            }
         }

         for(Is=0;Is<Norb;Is++) {
            if (Orb[Is].Exists) {
               O = &Orb[Is];

               sprintf(line,"Orb[%ld].PosN = [%18.12le %18.12le %18.12le]\n",Is,
                  O->PosN[0],O->PosN[1],O->PosN[2]);
               if (!strncmp(line,Prefix[Ipfx],PfxLen)) {
                  LineLen = strlen(line);
                  memcpy(&Msg[MsgLen],line,LineLen);
                  MsgLen += LineLen;
                  if (EchoEnabled) printf("%s",line);
               }

               sprintf(line,"Orb[%ld].VelN = [%18.12le %18.12le %18.12le]\n",Is,
                  O->VelN[0],O->VelN[1],O->VelN[2]);
               if (!strncmp(line,Prefix[Ipfx],PfxLen)) {
                  LineLen = strlen(line);
                  memcpy(&Msg[MsgLen],line,LineLen);
                  MsgLen += LineLen;
                  if (EchoEnabled) printf("%s",line);
               }
            }
         }

      }

      sprintf(line,"[ENDMSG]\n");
      LineLen = strlen(line);
      memcpy(&Msg[MsgLen],line,LineLen);
      MsgLen += LineLen;
      if (EchoEnabled) printf("%s",line);
      if (EchoEnabled) printf("MsgLen = %ld\n",MsgLen);
      printf("\n");

      Success = write(Socket,Msg,MsgLen);
      if (Success < 0) {
         printf("Error writing to socket in WriteToSocket.\n");
         exit(1);
      }
      read(Socket,Ack,4);

}
/******************************************************************************/
void ReadFromSocket(SOCKET Socket, long EchoEnabled)
{

      struct SCType *S;
      struct OrbitType *O;
      struct DynType *D;
      long Is,i;
      char line[512] = "Blank";
      long RequestTimeRefresh = 0;
      long Done;
      char Msg[16384];
      long Imsg,Iline;
      int NumBytes;
      double DbleVal[30];
      long Year,doy,Hour,Minute;
      double Second;
      char Ack[4] = "Ack\0";
      long k;

      NumBytes = read(Socket,Msg,16384);
      if (NumBytes < 0) {
         printf("Error reading from socket in ReadFromSocket.\n");
         exit(1);
      }
      write(Socket,Ack,4);

      Done = 0;
      Imsg = 0;
      while(!Done) {
         /* Parse lines from Msg, newline-delimited */
         Iline = 0;
         memset(line,'\0',512);
         while((Msg[Imsg] != '\n') && (Iline < 511) && (Imsg < 16383)) {
            line[Iline++] = Msg[Imsg++];
         }
         line[Iline++] = Msg[Imsg++];
         if (EchoEnabled) printf("%s",line);

         if (sscanf(line,"TIME %ld-%ld-%ld:%ld:%lf",
            &Year,&doy,&Hour,&Minute,&Second) == 5) {
            RequestTimeRefresh = 1;
         }


      if (sscanf(line,"SC[%ld].qn = [%le %le %le %le]",
         &Is,
         &DbleVal[0],
         &DbleVal[1],
         &DbleVal[2],
         &DbleVal[3]) == 5)
      {
         SC[Is].qn[0] = DbleVal[0];
         SC[Is].qn[1] = DbleVal[1];
         SC[Is].qn[2] = DbleVal[2];
         SC[Is].qn[3] = DbleVal[3];
         SC[Is].RequestStateRefresh = 1;
      }

      if (sscanf(line,"SC[%ld].wn = [%le %le %le]",
         &Is,
         &DbleVal[0],
         &DbleVal[1],
         &DbleVal[2]) == 4)
      {
         SC[Is].wn[0] = DbleVal[0];
         SC[Is].wn[1] = DbleVal[1];
         SC[Is].wn[2] = DbleVal[2];
         SC[Is].RequestStateRefresh = 1;
      }

      if (sscanf(line,"SC[%ld].PosR = [%le %le %le]",
         &Is,
         &DbleVal[0],
         &DbleVal[1],
         &DbleVal[2]) == 4)
      {
         SC[Is].PosR[0] = DbleVal[0];
         SC[Is].PosR[1] = DbleVal[1];
         SC[Is].PosR[2] = DbleVal[2];
         SC[Is].RequestStateRefresh = 1;
      }

      if (sscanf(line,"SC[%ld].VelR = [%le %le %le]",
         &Is,
         &DbleVal[0],
         &DbleVal[1],
         &DbleVal[2]) == 4)
      {
         SC[Is].VelR[0] = DbleVal[0];
         SC[Is].VelR[1] = DbleVal[1];
         SC[Is].VelR[2] = DbleVal[2];
         SC[Is].RequestStateRefresh = 1;
      }

      if (sscanf(line,"SC[%ld].svb = [%le %le %le]",
         &Is,
         &DbleVal[0],
         &DbleVal[1],
         &DbleVal[2]) == 4)
      {
         SC[Is].svb[0] = DbleVal[0];
         SC[Is].svb[1] = DbleVal[1];
         SC[Is].svb[2] = DbleVal[2];
         SC[Is].RequestStateRefresh = 1;
      }

      if (sscanf(line,"SC[%ld].bvb = [%le %le %le]",
         &Is,
         &DbleVal[0],
         &DbleVal[1],
         &DbleVal[2]) == 4)
      {
         SC[Is].bvb[0] = DbleVal[0];
         SC[Is].bvb[1] = DbleVal[1];
         SC[Is].bvb[2] = DbleVal[2];
         SC[Is].RequestStateRefresh = 1;
      }

      if (sscanf(line,"SC[%ld].Hvb = [%le %le %le]",
         &Is,
         &DbleVal[0],
         &DbleVal[1],
         &DbleVal[2]) == 4)
      {
         SC[Is].Hvb[0] = DbleVal[0];
         SC[Is].Hvb[1] = DbleVal[1];
         SC[Is].Hvb[2] = DbleVal[2];
         SC[Is].RequestStateRefresh = 1;
      }

      if (sscanf(line,"SC[%ld].B[%ld].wn = [%le %le %le]",
         &Is,&k,
         &DbleVal[0],
         &DbleVal[1],
         &DbleVal[2]) == 5)
      {
         SC[Is].B[k].wn[0] = DbleVal[0];
         SC[Is].B[k].wn[1] = DbleVal[1];
         SC[Is].B[k].wn[2] = DbleVal[2];
         SC[Is].RequestStateRefresh = 1;
      }

      if (sscanf(line,"SC[%ld].B[%ld].qn = [%le %le %le %le]",
         &Is,&k,
         &DbleVal[0],
         &DbleVal[1],
         &DbleVal[2],
         &DbleVal[3]) == 6)
      {
         SC[Is].B[k].qn[0] = DbleVal[0];
         SC[Is].B[k].qn[1] = DbleVal[1];
         SC[Is].B[k].qn[2] = DbleVal[2];
         SC[Is].B[k].qn[3] = DbleVal[3];
         SC[Is].RequestStateRefresh = 1;
      }

      if (sscanf(line,"SC[%ld].G[%ld].Pos = [%le %le %le]",
         &Is,&k,
         &DbleVal[0],
         &DbleVal[1],
         &DbleVal[2]) == 5)
      {
         SC[Is].G[k].Pos[0] = DbleVal[0];
         SC[Is].G[k].Pos[1] = DbleVal[1];
         SC[Is].G[k].Pos[2] = DbleVal[2];
         SC[Is].RequestStateRefresh = 1;
      }

      if (sscanf(line,"SC[%ld].G[%ld].PosRate = [%le %le %le]",
         &Is,&k,
         &DbleVal[0],
         &DbleVal[1],
         &DbleVal[2]) == 5)
      {
         SC[Is].G[k].PosRate[0] = DbleVal[0];
         SC[Is].G[k].PosRate[1] = DbleVal[1];
         SC[Is].G[k].PosRate[2] = DbleVal[2];
         SC[Is].RequestStateRefresh = 1;
      }

      if (sscanf(line,"SC[%ld].G[%ld].Ang = [%le %le %le]",
         &Is,&k,
         &DbleVal[0],
         &DbleVal[1],
         &DbleVal[2]) == 5)
      {
         SC[Is].G[k].Ang[0] = DbleVal[0];
         SC[Is].G[k].Ang[1] = DbleVal[1];
         SC[Is].G[k].Ang[2] = DbleVal[2];
         SC[Is].RequestStateRefresh = 1;
      }

      if (sscanf(line,"SC[%ld].G[%ld].AngRate = [%le %le %le]",
         &Is,&k,
         &DbleVal[0],
         &DbleVal[1],
         &DbleVal[2]) == 5)
      {
         SC[Is].G[k].AngRate[0] = DbleVal[0];
         SC[Is].G[k].AngRate[1] = DbleVal[1];
         SC[Is].G[k].AngRate[2] = DbleVal[2];
         SC[Is].RequestStateRefresh = 1;
      }

      if (sscanf(line,"SC[%ld].Whl[%ld].H = %le",
         &Is,&k,
         &DbleVal[0]) == 3)
      {
         SC[Is].Whl[k].H = DbleVal[0];
         SC[Is].RequestStateRefresh = 1;
      }

      if (sscanf(line,"World[%ld].PosH = [%le %le %le]",
         &Is,
         &DbleVal[0],
         &DbleVal[1],
         &DbleVal[2]) == 4)
      {
         World[Is].PosH[0] = DbleVal[0];
         World[Is].PosH[1] = DbleVal[1];
         World[Is].PosH[2] = DbleVal[2];
      }

      if (sscanf(line,"World[%ld].eph.PosN = [%le %le %le]",
         &Is,
         &DbleVal[0],
         &DbleVal[1],
         &DbleVal[2]) == 4)
      {
         World[Is].eph.PosN[0] = DbleVal[0];
         World[Is].eph.PosN[1] = DbleVal[1];
         World[Is].eph.PosN[2] = DbleVal[2];
      }

      if (sscanf(line,"World[%ld].eph.VelN = [%le %le %le]",
         &Is,
         &DbleVal[0],
         &DbleVal[1],
         &DbleVal[2]) == 4)
      {
         World[Is].eph.VelN[0] = DbleVal[0];
         World[Is].eph.VelN[1] = DbleVal[1];
         World[Is].eph.VelN[2] = DbleVal[2];
      }

      if (sscanf(line,"Orb[%ld].PosN = [%le %le %le]",
         &Is,
         &DbleVal[0],
         &DbleVal[1],
         &DbleVal[2]) == 4)
      {
         Orb[Is].PosN[0] = DbleVal[0];
         Orb[Is].PosN[1] = DbleVal[1];
         Orb[Is].PosN[2] = DbleVal[2];
      }

      if (sscanf(line,"Orb[%ld].VelN = [%le %le %le]",
         &Is,
         &DbleVal[0],
         &DbleVal[1],
         &DbleVal[2]) == 4)
      {
         Orb[Is].VelN[0] = DbleVal[0];
         Orb[Is].VelN[1] = DbleVal[1];
         Orb[Is].VelN[2] = DbleVal[2];
      }
         if (!strncmp(line,"[ENDMSG]",8)) {
            Done = 1;
            //sprintf(line,"[ENDMSG] reached\n");
         }
         if (Imsg >= 16383) {
            Done = 1;
            printf("Imsg limit exceeded\n");
         }
      }
      if (EchoEnabled) printf("MsgLen = %ld\n\n",Imsg);

      if (RequestTimeRefresh) {
         /* Update time variables */
         UTC.Year = Year;
         UTC.doy = doy;
         UTC.Hour = Hour;
         UTC.Minute = Minute;
         UTC.Second = Second;
         DOY2MD(UTC.Year,UTC.doy,&UTC.Month,&UTC.Day);
         CivilTime = DateToTime(UTC.Year,UTC.Month,UTC.Day,UTC.Hour,UTC.Minute,UTC.Second);
         AtomicTime = CivilTime + LeapSec;
         GpsTime = AtomicTime - 19.0;
         DynTime = AtomicTime + 32.184;
         TT.JulDay = TimeToJD(DynTime);
         TimeToDate(DynTime,&TT.Year,&TT.Month,&TT.Day,
            &TT.Hour,&TT.Minute,&TT.Second,DTSIM);
         TT.doy = MD2DOY(TT.Year,TT.Month,TT.Day);
         UTC.JulDay = TimeToJD(CivilTime);
         GpsTimeToGpsDate(GpsTime,&GpsRollover,&GpsWeek,&GpsSecond);
         SimTime = DynTime-DynTime0;
      }

/* .. Refresh SC states that depend on inputs */

      for(Is=0;Is<Nsc;Is++) {
         if (SC[Is].RequestStateRefresh) {
            S = &SC[Is];
            S->RequestStateRefresh = 0;
            if (S->Exists) {
               /* Update  RefOrb */
               O = &Orb[S->RefOrb];
               O->Epoch = DynTime;
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
               MapStateVectorToBodyStates(D->u,D->x,D->h,D->a,D->uf,D->xf,S);
               MotionConstraints(S);
            }
         }
      }
}

#include "Ac.h"

/**********************************************************************/
void WriteToSocket(SOCKET Socket, struct AcType *AC)
{

      long Isc,Iorb,Iw,i;
      int Success;
      char AckMsg[8] = "Ack\n";
      char Msg[16384];
      long MsgLen = 0;
      long LineLen;
      char line[512];

      Isc = AC->ID;

      sprintf(line,"SC[%ld].AC.svb = [ %le %le %le]\n",
         Isc,
         AC->svb[0],
         AC->svb[1],
         AC->svb[2]);
      if (AC->EchoEnabled) printf("%s",line);
      LineLen = strlen(line);
      memcpy(&Msg[MsgLen],line,LineLen);
      MsgLen += LineLen;

      sprintf(line,"SC[%ld].AC.bvb = [ %le %le %le]\n",
         Isc,
         AC->bvb[0],
         AC->bvb[1],
         AC->bvb[2]);
      if (AC->EchoEnabled) printf("%s",line);
      LineLen = strlen(line);
      memcpy(&Msg[MsgLen],line,LineLen);
      MsgLen += LineLen;

      sprintf(line,"SC[%ld].AC.Hvb = [ %le %le %le]\n",
         Isc,
         AC->Hvb[0],
         AC->Hvb[1],
         AC->Hvb[2]);
      if (AC->EchoEnabled) printf("%s",line);
      LineLen = strlen(line);
      memcpy(&Msg[MsgLen],line,LineLen);
      MsgLen += LineLen;

      for(i=0;i<AC->Ng;i++) {
         sprintf(line,"SC[%ld].AC.G[%ld].Cmd.Ang = [ %le %le %le]\n",
            Isc,i,
            AC->G[i].Cmd.Ang[0],
            AC->G[i].Cmd.Ang[1],
            AC->G[i].Cmd.Ang[2]);
         if (AC->EchoEnabled) printf("%s",line);
         LineLen = strlen(line);
         memcpy(&Msg[MsgLen],line,LineLen);
         MsgLen += LineLen;

      }

      for(i=0;i<AC->Nwhl;i++) {
         sprintf(line,"SC[%ld].AC.Whl[%ld].Tcmd = [ %le]\n",
            Isc,i,
            AC->Whl[i].Tcmd);
         if (AC->EchoEnabled) printf("%s",line);
         LineLen = strlen(line);
         memcpy(&Msg[MsgLen],line,LineLen);
         MsgLen += LineLen;

      }

      for(i=0;i<AC->Nmtb;i++) {
         sprintf(line,"SC[%ld].AC.MTB[%ld].Mcmd = [ %le]\n",
            Isc,i,
            AC->MTB[i].Mcmd);
         if (AC->EchoEnabled) printf("%s",line);
         LineLen = strlen(line);
         memcpy(&Msg[MsgLen],line,LineLen);
         MsgLen += LineLen;

      }

      for(i=0;i<AC->Nthr;i++) {
         sprintf(line,"SC[%ld].AC.Thr[%ld].PulseWidthCmd = [ %le]\n",
            Isc,i,
            AC->Thr[i].PulseWidthCmd);
         if (AC->EchoEnabled) printf("%s",line);
         LineLen = strlen(line);
         memcpy(&Msg[MsgLen],line,LineLen);
         MsgLen += LineLen;

      }

      sprintf(line,"SC[%ld].AC.Cmd.Ang = [ %le %le %le]\n",
         Isc,
         AC->Cmd.Ang[0],
         AC->Cmd.Ang[1],
         AC->Cmd.Ang[2]);
      if (AC->EchoEnabled) printf("%s",line);
      LineLen = strlen(line);
      memcpy(&Msg[MsgLen],line,LineLen);
      MsgLen += LineLen;

      if (AC->ParmDumpEnabled) {
         sprintf(line,"SC[%ld].AC.ID = [ %ld]\n",
            Isc,
            AC->ID);
         if (AC->EchoEnabled) printf("%s",line);
         LineLen = strlen(line);
         memcpy(&Msg[MsgLen],line,LineLen);
         MsgLen += LineLen;

         sprintf(line,"SC[%ld].AC.EchoEnabled = [ %ld]\n",
            Isc,
            AC->EchoEnabled);
         if (AC->EchoEnabled) printf("%s",line);
         LineLen = strlen(line);
         memcpy(&Msg[MsgLen],line,LineLen);
         MsgLen += LineLen;

         sprintf(line,"SC[%ld].AC.Nb = [ %ld]\n",
            Isc,
            AC->Nb);
         if (AC->EchoEnabled) printf("%s",line);
         LineLen = strlen(line);
         memcpy(&Msg[MsgLen],line,LineLen);
         MsgLen += LineLen;

         sprintf(line,"SC[%ld].AC.Ng = [ %ld]\n",
            Isc,
            AC->Ng);
         if (AC->EchoEnabled) printf("%s",line);
         LineLen = strlen(line);
         memcpy(&Msg[MsgLen],line,LineLen);
         MsgLen += LineLen;

         sprintf(line,"SC[%ld].AC.Nwhl = [ %ld]\n",
            Isc,
            AC->Nwhl);
         if (AC->EchoEnabled) printf("%s",line);
         LineLen = strlen(line);
         memcpy(&Msg[MsgLen],line,LineLen);
         MsgLen += LineLen;

         sprintf(line,"SC[%ld].AC.Nmtb = [ %ld]\n",
            Isc,
            AC->Nmtb);
         if (AC->EchoEnabled) printf("%s",line);
         LineLen = strlen(line);
         memcpy(&Msg[MsgLen],line,LineLen);
         MsgLen += LineLen;

         sprintf(line,"SC[%ld].AC.Nthr = [ %ld]\n",
            Isc,
            AC->Nthr);
         if (AC->EchoEnabled) printf("%s",line);
         LineLen = strlen(line);
         memcpy(&Msg[MsgLen],line,LineLen);
         MsgLen += LineLen;

         sprintf(line,"SC[%ld].AC.Ncmg = [ %ld]\n",
            Isc,
            AC->Ncmg);
         if (AC->EchoEnabled) printf("%s",line);
         LineLen = strlen(line);
         memcpy(&Msg[MsgLen],line,LineLen);
         MsgLen += LineLen;

         sprintf(line,"SC[%ld].AC.Ngyro = [ %ld]\n",
            Isc,
            AC->Ngyro);
         if (AC->EchoEnabled) printf("%s",line);
         LineLen = strlen(line);
         memcpy(&Msg[MsgLen],line,LineLen);
         MsgLen += LineLen;

         sprintf(line,"SC[%ld].AC.Nmag = [ %ld]\n",
            Isc,
            AC->Nmag);
         if (AC->EchoEnabled) printf("%s",line);
         LineLen = strlen(line);
         memcpy(&Msg[MsgLen],line,LineLen);
         MsgLen += LineLen;

         sprintf(line,"SC[%ld].AC.Ncss = [ %ld]\n",
            Isc,
            AC->Ncss);
         if (AC->EchoEnabled) printf("%s",line);
         LineLen = strlen(line);
         memcpy(&Msg[MsgLen],line,LineLen);
         MsgLen += LineLen;

         sprintf(line,"SC[%ld].AC.Nfss = [ %ld]\n",
            Isc,
            AC->Nfss);
         if (AC->EchoEnabled) printf("%s",line);
         LineLen = strlen(line);
         memcpy(&Msg[MsgLen],line,LineLen);
         MsgLen += LineLen;

         sprintf(line,"SC[%ld].AC.Nst = [ %ld]\n",
            Isc,
            AC->Nst);
         if (AC->EchoEnabled) printf("%s",line);
         LineLen = strlen(line);
         memcpy(&Msg[MsgLen],line,LineLen);
         MsgLen += LineLen;

         sprintf(line,"SC[%ld].AC.Ngps = [ %ld]\n",
            Isc,
            AC->Ngps);
         if (AC->EchoEnabled) printf("%s",line);
         LineLen = strlen(line);
         memcpy(&Msg[MsgLen],line,LineLen);
         MsgLen += LineLen;

         sprintf(line,"SC[%ld].AC.Nacc = [ %ld]\n",
            Isc,
            AC->Nacc);
         if (AC->EchoEnabled) printf("%s",line);
         LineLen = strlen(line);
         memcpy(&Msg[MsgLen],line,LineLen);
         MsgLen += LineLen;

         sprintf(line,"SC[%ld].AC.DT = [ %le]\n",
            Isc,
            AC->DT);
         if (AC->EchoEnabled) printf("%s",line);
         LineLen = strlen(line);
         memcpy(&Msg[MsgLen],line,LineLen);
         MsgLen += LineLen;

         sprintf(line,"SC[%ld].AC.mass = [ %le]\n",
            Isc,
            AC->mass);
         if (AC->EchoEnabled) printf("%s",line);
         LineLen = strlen(line);
         memcpy(&Msg[MsgLen],line,LineLen);
         MsgLen += LineLen;

         sprintf(line,"SC[%ld].AC.cm = [ %le %le %le]\n",
            Isc,
            AC->cm[0],
            AC->cm[1],
            AC->cm[2]);
         if (AC->EchoEnabled) printf("%s",line);
         LineLen = strlen(line);
         memcpy(&Msg[MsgLen],line,LineLen);
         MsgLen += LineLen;

         sprintf(line,"SC[%ld].AC.MOI = [ %le %le %le %le %le %le %le %le %le]\n",
            Isc,
            AC->MOI[0][0],
            AC->MOI[0][1],
            AC->MOI[0][2],
            AC->MOI[1][0],
            AC->MOI[1][1],
            AC->MOI[1][2],
            AC->MOI[2][0],
            AC->MOI[2][1],
            AC->MOI[2][2]);
         if (AC->EchoEnabled) printf("%s",line);
         LineLen = strlen(line);
         memcpy(&Msg[MsgLen],line,LineLen);
         MsgLen += LineLen;

         for(i=0;i<AC->Nb;i++) {
            sprintf(line,"SC[%ld].AC.B[%ld].mass = [ %le]\n",
               Isc,i,
               AC->B[i].mass);
            if (AC->EchoEnabled) printf("%s",line);
            LineLen = strlen(line);
            memcpy(&Msg[MsgLen],line,LineLen);
            MsgLen += LineLen;

            sprintf(line,"SC[%ld].AC.B[%ld].cm = [ %le %le %le]\n",
               Isc,i,
               AC->B[i].cm[0],
               AC->B[i].cm[1],
               AC->B[i].cm[2]);
            if (AC->EchoEnabled) printf("%s",line);
            LineLen = strlen(line);
            memcpy(&Msg[MsgLen],line,LineLen);
            MsgLen += LineLen;

            sprintf(line,"SC[%ld].AC.B[%ld].MOI = [ %le %le %le %le %le %le %le %le %le]\n",
               Isc,i,
               AC->B[i].MOI[0][0],
               AC->B[i].MOI[0][1],
               AC->B[i].MOI[0][2],
               AC->B[i].MOI[1][0],
               AC->B[i].MOI[1][1],
               AC->B[i].MOI[1][2],
               AC->B[i].MOI[2][0],
               AC->B[i].MOI[2][1],
               AC->B[i].MOI[2][2]);
            if (AC->EchoEnabled) printf("%s",line);
            LineLen = strlen(line);
            memcpy(&Msg[MsgLen],line,LineLen);
            MsgLen += LineLen;

         }

         for(i=0;i<AC->Ng;i++) {
            sprintf(line,"SC[%ld].AC.G[%ld].IsUnderActiveControl = [ %ld]\n",
               Isc,i,
               AC->G[i].IsUnderActiveControl);
            if (AC->EchoEnabled) printf("%s",line);
            LineLen = strlen(line);
            memcpy(&Msg[MsgLen],line,LineLen);
            MsgLen += LineLen;

            sprintf(line,"SC[%ld].AC.G[%ld].IsSpherical = [ %ld]\n",
               Isc,i,
               AC->G[i].IsSpherical);
            if (AC->EchoEnabled) printf("%s",line);
            LineLen = strlen(line);
            memcpy(&Msg[MsgLen],line,LineLen);
            MsgLen += LineLen;

            sprintf(line,"SC[%ld].AC.G[%ld].RotDOF = [ %ld]\n",
               Isc,i,
               AC->G[i].RotDOF);
            if (AC->EchoEnabled) printf("%s",line);
            LineLen = strlen(line);
            memcpy(&Msg[MsgLen],line,LineLen);
            MsgLen += LineLen;

            sprintf(line,"SC[%ld].AC.G[%ld].TrnDOF = [ %ld]\n",
               Isc,i,
               AC->G[i].TrnDOF);
            if (AC->EchoEnabled) printf("%s",line);
            LineLen = strlen(line);
            memcpy(&Msg[MsgLen],line,LineLen);
            MsgLen += LineLen;

            sprintf(line,"SC[%ld].AC.G[%ld].RotSeq = [ %ld]\n",
               Isc,i,
               AC->G[i].RotSeq);
            if (AC->EchoEnabled) printf("%s",line);
            LineLen = strlen(line);
            memcpy(&Msg[MsgLen],line,LineLen);
            MsgLen += LineLen;

            sprintf(line,"SC[%ld].AC.G[%ld].TrnSeq = [ %ld]\n",
               Isc,i,
               AC->G[i].TrnSeq);
            if (AC->EchoEnabled) printf("%s",line);
            LineLen = strlen(line);
            memcpy(&Msg[MsgLen],line,LineLen);
            MsgLen += LineLen;

            sprintf(line,"SC[%ld].AC.G[%ld].CGiBi = [ %le %le %le %le %le %le %le %le %le]\n",
               Isc,i,
               AC->G[i].CGiBi[0][0],
               AC->G[i].CGiBi[0][1],
               AC->G[i].CGiBi[0][2],
               AC->G[i].CGiBi[1][0],
               AC->G[i].CGiBi[1][1],
               AC->G[i].CGiBi[1][2],
               AC->G[i].CGiBi[2][0],
               AC->G[i].CGiBi[2][1],
               AC->G[i].CGiBi[2][2]);
            if (AC->EchoEnabled) printf("%s",line);
            LineLen = strlen(line);
            memcpy(&Msg[MsgLen],line,LineLen);
            MsgLen += LineLen;

            sprintf(line,"SC[%ld].AC.G[%ld].CBoGo = [ %le %le %le %le %le %le %le %le %le]\n",
               Isc,i,
               AC->G[i].CBoGo[0][0],
               AC->G[i].CBoGo[0][1],
               AC->G[i].CBoGo[0][2],
               AC->G[i].CBoGo[1][0],
               AC->G[i].CBoGo[1][1],
               AC->G[i].CBoGo[1][2],
               AC->G[i].CBoGo[2][0],
               AC->G[i].CBoGo[2][1],
               AC->G[i].CBoGo[2][2]);
            if (AC->EchoEnabled) printf("%s",line);
            LineLen = strlen(line);
            memcpy(&Msg[MsgLen],line,LineLen);
            MsgLen += LineLen;

            sprintf(line,"SC[%ld].AC.G[%ld].AngGain = [ %le %le %le]\n",
               Isc,i,
               AC->G[i].AngGain[0],
               AC->G[i].AngGain[1],
               AC->G[i].AngGain[2]);
            if (AC->EchoEnabled) printf("%s",line);
            LineLen = strlen(line);
            memcpy(&Msg[MsgLen],line,LineLen);
            MsgLen += LineLen;

            sprintf(line,"SC[%ld].AC.G[%ld].AngRateGain = [ %le %le %le]\n",
               Isc,i,
               AC->G[i].AngRateGain[0],
               AC->G[i].AngRateGain[1],
               AC->G[i].AngRateGain[2]);
            if (AC->EchoEnabled) printf("%s",line);
            LineLen = strlen(line);
            memcpy(&Msg[MsgLen],line,LineLen);
            MsgLen += LineLen;

            sprintf(line,"SC[%ld].AC.G[%ld].PosGain = [ %le %le %le]\n",
               Isc,i,
               AC->G[i].PosGain[0],
               AC->G[i].PosGain[1],
               AC->G[i].PosGain[2]);
            if (AC->EchoEnabled) printf("%s",line);
            LineLen = strlen(line);
            memcpy(&Msg[MsgLen],line,LineLen);
            MsgLen += LineLen;

            sprintf(line,"SC[%ld].AC.G[%ld].PosRateGain = [ %le %le %le]\n",
               Isc,i,
               AC->G[i].PosRateGain[0],
               AC->G[i].PosRateGain[1],
               AC->G[i].PosRateGain[2]);
            if (AC->EchoEnabled) printf("%s",line);
            LineLen = strlen(line);
            memcpy(&Msg[MsgLen],line,LineLen);
            MsgLen += LineLen;

            sprintf(line,"SC[%ld].AC.G[%ld].MaxAngRate = [ %le %le %le]\n",
               Isc,i,
               AC->G[i].MaxAngRate[0],
               AC->G[i].MaxAngRate[1],
               AC->G[i].MaxAngRate[2]);
            if (AC->EchoEnabled) printf("%s",line);
            LineLen = strlen(line);
            memcpy(&Msg[MsgLen],line,LineLen);
            MsgLen += LineLen;

            sprintf(line,"SC[%ld].AC.G[%ld].MaxPosRate = [ %le %le %le]\n",
               Isc,i,
               AC->G[i].MaxPosRate[0],
               AC->G[i].MaxPosRate[1],
               AC->G[i].MaxPosRate[2]);
            if (AC->EchoEnabled) printf("%s",line);
            LineLen = strlen(line);
            memcpy(&Msg[MsgLen],line,LineLen);
            MsgLen += LineLen;

            sprintf(line,"SC[%ld].AC.G[%ld].MaxTrq = [ %le %le %le]\n",
               Isc,i,
               AC->G[i].MaxTrq[0],
               AC->G[i].MaxTrq[1],
               AC->G[i].MaxTrq[2]);
            if (AC->EchoEnabled) printf("%s",line);
            LineLen = strlen(line);
            memcpy(&Msg[MsgLen],line,LineLen);
            MsgLen += LineLen;

            sprintf(line,"SC[%ld].AC.G[%ld].MaxFrc = [ %le %le %le]\n",
               Isc,i,
               AC->G[i].MaxFrc[0],
               AC->G[i].MaxFrc[1],
               AC->G[i].MaxFrc[2]);
            if (AC->EchoEnabled) printf("%s",line);
            LineLen = strlen(line);
            memcpy(&Msg[MsgLen],line,LineLen);
            MsgLen += LineLen;

         }

         for(i=0;i<AC->Ngyro;i++) {
            sprintf(line,"SC[%ld].AC.Gyro[%ld].Axis = [ %le %le %le]\n",
               Isc,i,
               AC->Gyro[i].Axis[0],
               AC->Gyro[i].Axis[1],
               AC->Gyro[i].Axis[2]);
            if (AC->EchoEnabled) printf("%s",line);
            LineLen = strlen(line);
            memcpy(&Msg[MsgLen],line,LineLen);
            MsgLen += LineLen;

         }

         for(i=0;i<AC->Nmag;i++) {
            sprintf(line,"SC[%ld].AC.MAG[%ld].Axis = [ %le %le %le]\n",
               Isc,i,
               AC->MAG[i].Axis[0],
               AC->MAG[i].Axis[1],
               AC->MAG[i].Axis[2]);
            if (AC->EchoEnabled) printf("%s",line);
            LineLen = strlen(line);
            memcpy(&Msg[MsgLen],line,LineLen);
            MsgLen += LineLen;

         }

         for(i=0;i<AC->Ncss;i++) {
            sprintf(line,"SC[%ld].AC.CSS[%ld].Body = [ %ld]\n",
               Isc,i,
               AC->CSS[i].Body);
            if (AC->EchoEnabled) printf("%s",line);
            LineLen = strlen(line);
            memcpy(&Msg[MsgLen],line,LineLen);
            MsgLen += LineLen;

            sprintf(line,"SC[%ld].AC.CSS[%ld].Axis = [ %le %le %le]\n",
               Isc,i,
               AC->CSS[i].Axis[0],
               AC->CSS[i].Axis[1],
               AC->CSS[i].Axis[2]);
            if (AC->EchoEnabled) printf("%s",line);
            LineLen = strlen(line);
            memcpy(&Msg[MsgLen],line,LineLen);
            MsgLen += LineLen;

            sprintf(line,"SC[%ld].AC.CSS[%ld].Scale = [ %le]\n",
               Isc,i,
               AC->CSS[i].Scale);
            if (AC->EchoEnabled) printf("%s",line);
            LineLen = strlen(line);
            memcpy(&Msg[MsgLen],line,LineLen);
            MsgLen += LineLen;

         }

         for(i=0;i<AC->Nfss;i++) {
            sprintf(line,"SC[%ld].AC.FSS[%ld].qb = [ %le %le %le %le]\n",
               Isc,i,
               AC->FSS[i].qb[0],
               AC->FSS[i].qb[1],
               AC->FSS[i].qb[2],
               AC->FSS[i].qb[3]);
            if (AC->EchoEnabled) printf("%s",line);
            LineLen = strlen(line);
            memcpy(&Msg[MsgLen],line,LineLen);
            MsgLen += LineLen;

            sprintf(line,"SC[%ld].AC.FSS[%ld].CB = [ %le %le %le %le %le %le %le %le %le]\n",
               Isc,i,
               AC->FSS[i].CB[0][0],
               AC->FSS[i].CB[0][1],
               AC->FSS[i].CB[0][2],
               AC->FSS[i].CB[1][0],
               AC->FSS[i].CB[1][1],
               AC->FSS[i].CB[1][2],
               AC->FSS[i].CB[2][0],
               AC->FSS[i].CB[2][1],
               AC->FSS[i].CB[2][2]);
            if (AC->EchoEnabled) printf("%s",line);
            LineLen = strlen(line);
            memcpy(&Msg[MsgLen],line,LineLen);
            MsgLen += LineLen;

         }

         for(i=0;i<AC->Nst;i++) {
            sprintf(line,"SC[%ld].AC.ST[%ld].qb = [ %le %le %le %le]\n",
               Isc,i,
               AC->ST[i].qb[0],
               AC->ST[i].qb[1],
               AC->ST[i].qb[2],
               AC->ST[i].qb[3]);
            if (AC->EchoEnabled) printf("%s",line);
            LineLen = strlen(line);
            memcpy(&Msg[MsgLen],line,LineLen);
            MsgLen += LineLen;

            sprintf(line,"SC[%ld].AC.ST[%ld].CB = [ %le %le %le %le %le %le %le %le %le]\n",
               Isc,i,
               AC->ST[i].CB[0][0],
               AC->ST[i].CB[0][1],
               AC->ST[i].CB[0][2],
               AC->ST[i].CB[1][0],
               AC->ST[i].CB[1][1],
               AC->ST[i].CB[1][2],
               AC->ST[i].CB[2][0],
               AC->ST[i].CB[2][1],
               AC->ST[i].CB[2][2]);
            if (AC->EchoEnabled) printf("%s",line);
            LineLen = strlen(line);
            memcpy(&Msg[MsgLen],line,LineLen);
            MsgLen += LineLen;

         }

         for(i=0;i<AC->Nacc;i++) {
            sprintf(line,"SC[%ld].AC.Accel[%ld].PosB = [ %le %le %le]\n",
               Isc,i,
               AC->Accel[i].PosB[0],
               AC->Accel[i].PosB[1],
               AC->Accel[i].PosB[2]);
            if (AC->EchoEnabled) printf("%s",line);
            LineLen = strlen(line);
            memcpy(&Msg[MsgLen],line,LineLen);
            MsgLen += LineLen;

            sprintf(line,"SC[%ld].AC.Accel[%ld].Axis = [ %le %le %le]\n",
               Isc,i,
               AC->Accel[i].Axis[0],
               AC->Accel[i].Axis[1],
               AC->Accel[i].Axis[2]);
            if (AC->EchoEnabled) printf("%s",line);
            LineLen = strlen(line);
            memcpy(&Msg[MsgLen],line,LineLen);
            MsgLen += LineLen;

         }

         for(i=0;i<AC->Nwhl;i++) {
            sprintf(line,"SC[%ld].AC.Whl[%ld].Axis = [ %le %le %le]\n",
               Isc,i,
               AC->Whl[i].Axis[0],
               AC->Whl[i].Axis[1],
               AC->Whl[i].Axis[2]);
            if (AC->EchoEnabled) printf("%s",line);
            LineLen = strlen(line);
            memcpy(&Msg[MsgLen],line,LineLen);
            MsgLen += LineLen;

            sprintf(line,"SC[%ld].AC.Whl[%ld].DistVec = [ %le %le %le]\n",
               Isc,i,
               AC->Whl[i].DistVec[0],
               AC->Whl[i].DistVec[1],
               AC->Whl[i].DistVec[2]);
            if (AC->EchoEnabled) printf("%s",line);
            LineLen = strlen(line);
            memcpy(&Msg[MsgLen],line,LineLen);
            MsgLen += LineLen;

            sprintf(line,"SC[%ld].AC.Whl[%ld].J = [ %le]\n",
               Isc,i,
               AC->Whl[i].J);
            if (AC->EchoEnabled) printf("%s",line);
            LineLen = strlen(line);
            memcpy(&Msg[MsgLen],line,LineLen);
            MsgLen += LineLen;

            sprintf(line,"SC[%ld].AC.Whl[%ld].Tmax = [ %le]\n",
               Isc,i,
               AC->Whl[i].Tmax);
            if (AC->EchoEnabled) printf("%s",line);
            LineLen = strlen(line);
            memcpy(&Msg[MsgLen],line,LineLen);
            MsgLen += LineLen;

            sprintf(line,"SC[%ld].AC.Whl[%ld].Hmax = [ %le]\n",
               Isc,i,
               AC->Whl[i].Hmax);
            if (AC->EchoEnabled) printf("%s",line);
            LineLen = strlen(line);
            memcpy(&Msg[MsgLen],line,LineLen);
            MsgLen += LineLen;

         }

         for(i=0;i<AC->Nmtb;i++) {
            sprintf(line,"SC[%ld].AC.MTB[%ld].Axis = [ %le %le %le]\n",
               Isc,i,
               AC->MTB[i].Axis[0],
               AC->MTB[i].Axis[1],
               AC->MTB[i].Axis[2]);
            if (AC->EchoEnabled) printf("%s",line);
            LineLen = strlen(line);
            memcpy(&Msg[MsgLen],line,LineLen);
            MsgLen += LineLen;

            sprintf(line,"SC[%ld].AC.MTB[%ld].DistVec = [ %le %le %le]\n",
               Isc,i,
               AC->MTB[i].DistVec[0],
               AC->MTB[i].DistVec[1],
               AC->MTB[i].DistVec[2]);
            if (AC->EchoEnabled) printf("%s",line);
            LineLen = strlen(line);
            memcpy(&Msg[MsgLen],line,LineLen);
            MsgLen += LineLen;

            sprintf(line,"SC[%ld].AC.MTB[%ld].Mmax = [ %le]\n",
               Isc,i,
               AC->MTB[i].Mmax);
            if (AC->EchoEnabled) printf("%s",line);
            LineLen = strlen(line);
            memcpy(&Msg[MsgLen],line,LineLen);
            MsgLen += LineLen;

         }

         for(i=0;i<AC->Nthr;i++) {
            sprintf(line,"SC[%ld].AC.Thr[%ld].PosB = [ %le %le %le]\n",
               Isc,i,
               AC->Thr[i].PosB[0],
               AC->Thr[i].PosB[1],
               AC->Thr[i].PosB[2]);
            if (AC->EchoEnabled) printf("%s",line);
            LineLen = strlen(line);
            memcpy(&Msg[MsgLen],line,LineLen);
            MsgLen += LineLen;

            sprintf(line,"SC[%ld].AC.Thr[%ld].Axis = [ %le %le %le]\n",
               Isc,i,
               AC->Thr[i].Axis[0],
               AC->Thr[i].Axis[1],
               AC->Thr[i].Axis[2]);
            if (AC->EchoEnabled) printf("%s",line);
            LineLen = strlen(line);
            memcpy(&Msg[MsgLen],line,LineLen);
            MsgLen += LineLen;

            sprintf(line,"SC[%ld].AC.Thr[%ld].rxA = [ %le %le %le]\n",
               Isc,i,
               AC->Thr[i].rxA[0],
               AC->Thr[i].rxA[1],
               AC->Thr[i].rxA[2]);
            if (AC->EchoEnabled) printf("%s",line);
            LineLen = strlen(line);
            memcpy(&Msg[MsgLen],line,LineLen);
            MsgLen += LineLen;

            sprintf(line,"SC[%ld].AC.Thr[%ld].Fmax = [ %le]\n",
               Isc,i,
               AC->Thr[i].Fmax);
            if (AC->EchoEnabled) printf("%s",line);
            LineLen = strlen(line);
            memcpy(&Msg[MsgLen],line,LineLen);
            MsgLen += LineLen;

         }

         sprintf(line,"SC[%ld].AC.PrototypeCtrl.wc = [ %le]\n",
            Isc,
            AC->PrototypeCtrl.wc);
         if (AC->EchoEnabled) printf("%s",line);
         LineLen = strlen(line);
         memcpy(&Msg[MsgLen],line,LineLen);
         MsgLen += LineLen;

         sprintf(line,"SC[%ld].AC.PrototypeCtrl.amax = [ %le]\n",
            Isc,
            AC->PrototypeCtrl.amax);
         if (AC->EchoEnabled) printf("%s",line);
         LineLen = strlen(line);
         memcpy(&Msg[MsgLen],line,LineLen);
         MsgLen += LineLen;

         sprintf(line,"SC[%ld].AC.PrototypeCtrl.vmax = [ %le]\n",
            Isc,
            AC->PrototypeCtrl.vmax);
         if (AC->EchoEnabled) printf("%s",line);
         LineLen = strlen(line);
         memcpy(&Msg[MsgLen],line,LineLen);
         MsgLen += LineLen;

         sprintf(line,"SC[%ld].AC.PrototypeCtrl.Kprec = [ %le]\n",
            Isc,
            AC->PrototypeCtrl.Kprec);
         if (AC->EchoEnabled) printf("%s",line);
         LineLen = strlen(line);
         memcpy(&Msg[MsgLen],line,LineLen);
         MsgLen += LineLen;

         sprintf(line,"SC[%ld].AC.PrototypeCtrl.Knute = [ %le]\n",
            Isc,
            AC->PrototypeCtrl.Knute);
         if (AC->EchoEnabled) printf("%s",line);
         LineLen = strlen(line);
         memcpy(&Msg[MsgLen],line,LineLen);
         MsgLen += LineLen;

         sprintf(line,"SC[%ld].AC.AdHocCtrl.Kr = [ %le %le %le]\n",
            Isc,
            AC->AdHocCtrl.Kr[0],
            AC->AdHocCtrl.Kr[1],
            AC->AdHocCtrl.Kr[2]);
         if (AC->EchoEnabled) printf("%s",line);
         LineLen = strlen(line);
         memcpy(&Msg[MsgLen],line,LineLen);
         MsgLen += LineLen;

         sprintf(line,"SC[%ld].AC.AdHocCtrl.Kp = [ %le %le %le]\n",
            Isc,
            AC->AdHocCtrl.Kp[0],
            AC->AdHocCtrl.Kp[1],
            AC->AdHocCtrl.Kp[2]);
         if (AC->EchoEnabled) printf("%s",line);
         LineLen = strlen(line);
         memcpy(&Msg[MsgLen],line,LineLen);
         MsgLen += LineLen;

         sprintf(line,"SC[%ld].AC.SpinnerCtrl.Ispin = [ %le]\n",
            Isc,
            AC->SpinnerCtrl.Ispin);
         if (AC->EchoEnabled) printf("%s",line);
         LineLen = strlen(line);
         memcpy(&Msg[MsgLen],line,LineLen);
         MsgLen += LineLen;

         sprintf(line,"SC[%ld].AC.SpinnerCtrl.Itrans = [ %le]\n",
            Isc,
            AC->SpinnerCtrl.Itrans);
         if (AC->EchoEnabled) printf("%s",line);
         LineLen = strlen(line);
         memcpy(&Msg[MsgLen],line,LineLen);
         MsgLen += LineLen;

         sprintf(line,"SC[%ld].AC.SpinnerCtrl.SpinRate = [ %le]\n",
            Isc,
            AC->SpinnerCtrl.SpinRate);
         if (AC->EchoEnabled) printf("%s",line);
         LineLen = strlen(line);
         memcpy(&Msg[MsgLen],line,LineLen);
         MsgLen += LineLen;

         sprintf(line,"SC[%ld].AC.SpinnerCtrl.Knute = [ %le]\n",
            Isc,
            AC->SpinnerCtrl.Knute);
         if (AC->EchoEnabled) printf("%s",line);
         LineLen = strlen(line);
         memcpy(&Msg[MsgLen],line,LineLen);
         MsgLen += LineLen;

         sprintf(line,"SC[%ld].AC.SpinnerCtrl.Kprec = [ %le]\n",
            Isc,
            AC->SpinnerCtrl.Kprec);
         if (AC->EchoEnabled) printf("%s",line);
         LineLen = strlen(line);
         memcpy(&Msg[MsgLen],line,LineLen);
         MsgLen += LineLen;

         sprintf(line,"SC[%ld].AC.ThreeAxisCtrl.Kr = [ %le %le %le]\n",
            Isc,
            AC->ThreeAxisCtrl.Kr[0],
            AC->ThreeAxisCtrl.Kr[1],
            AC->ThreeAxisCtrl.Kr[2]);
         if (AC->EchoEnabled) printf("%s",line);
         LineLen = strlen(line);
         memcpy(&Msg[MsgLen],line,LineLen);
         MsgLen += LineLen;

         sprintf(line,"SC[%ld].AC.ThreeAxisCtrl.Kp = [ %le %le %le]\n",
            Isc,
            AC->ThreeAxisCtrl.Kp[0],
            AC->ThreeAxisCtrl.Kp[1],
            AC->ThreeAxisCtrl.Kp[2]);
         if (AC->EchoEnabled) printf("%s",line);
         LineLen = strlen(line);
         memcpy(&Msg[MsgLen],line,LineLen);
         MsgLen += LineLen;

         sprintf(line,"SC[%ld].AC.ThreeAxisCtrl.Kunl = [ %le]\n",
            Isc,
            AC->ThreeAxisCtrl.Kunl);
         if (AC->EchoEnabled) printf("%s",line);
         LineLen = strlen(line);
         memcpy(&Msg[MsgLen],line,LineLen);
         MsgLen += LineLen;

         sprintf(line,"SC[%ld].AC.IssCtrl.Kr = [ %le %le %le]\n",
            Isc,
            AC->IssCtrl.Kr[0],
            AC->IssCtrl.Kr[1],
            AC->IssCtrl.Kr[2]);
         if (AC->EchoEnabled) printf("%s",line);
         LineLen = strlen(line);
         memcpy(&Msg[MsgLen],line,LineLen);
         MsgLen += LineLen;

         sprintf(line,"SC[%ld].AC.IssCtrl.Kp = [ %le %le %le]\n",
            Isc,
            AC->IssCtrl.Kp[0],
            AC->IssCtrl.Kp[1],
            AC->IssCtrl.Kp[2]);
         if (AC->EchoEnabled) printf("%s",line);
         LineLen = strlen(line);
         memcpy(&Msg[MsgLen],line,LineLen);
         MsgLen += LineLen;

         sprintf(line,"SC[%ld].AC.IssCtrl.Tmax = [ %le]\n",
            Isc,
            AC->IssCtrl.Tmax);
         if (AC->EchoEnabled) printf("%s",line);
         LineLen = strlen(line);
         memcpy(&Msg[MsgLen],line,LineLen);
         MsgLen += LineLen;

         sprintf(line,"SC[%ld].AC.CmgCtrl.Kr = [ %le %le %le]\n",
            Isc,
            AC->CmgCtrl.Kr[0],
            AC->CmgCtrl.Kr[1],
            AC->CmgCtrl.Kr[2]);
         if (AC->EchoEnabled) printf("%s",line);
         LineLen = strlen(line);
         memcpy(&Msg[MsgLen],line,LineLen);
         MsgLen += LineLen;

         sprintf(line,"SC[%ld].AC.CmgCtrl.Kp = [ %le %le %le]\n",
            Isc,
            AC->CmgCtrl.Kp[0],
            AC->CmgCtrl.Kp[1],
            AC->CmgCtrl.Kp[2]);
         if (AC->EchoEnabled) printf("%s",line);
         LineLen = strlen(line);
         memcpy(&Msg[MsgLen],line,LineLen);
         MsgLen += LineLen;

         sprintf(line,"SC[%ld].AC.ThrCtrl.Kw = [ %le %le %le]\n",
            Isc,
            AC->ThrCtrl.Kw[0],
            AC->ThrCtrl.Kw[1],
            AC->ThrCtrl.Kw[2]);
         if (AC->EchoEnabled) printf("%s",line);
         LineLen = strlen(line);
         memcpy(&Msg[MsgLen],line,LineLen);
         MsgLen += LineLen;

         sprintf(line,"SC[%ld].AC.ThrCtrl.Kth = [ %le %le %le]\n",
            Isc,
            AC->ThrCtrl.Kth[0],
            AC->ThrCtrl.Kth[1],
            AC->ThrCtrl.Kth[2]);
         if (AC->EchoEnabled) printf("%s",line);
         LineLen = strlen(line);
         memcpy(&Msg[MsgLen],line,LineLen);
         MsgLen += LineLen;

         sprintf(line,"SC[%ld].AC.ThrCtrl.Kv = [ %le]\n",
            Isc,
            AC->ThrCtrl.Kv);
         if (AC->EchoEnabled) printf("%s",line);
         LineLen = strlen(line);
         memcpy(&Msg[MsgLen],line,LineLen);
         MsgLen += LineLen;

         sprintf(line,"SC[%ld].AC.ThrCtrl.Kp = [ %le]\n",
            Isc,
            AC->ThrCtrl.Kp);
         if (AC->EchoEnabled) printf("%s",line);
         LineLen = strlen(line);
         memcpy(&Msg[MsgLen],line,LineLen);
         MsgLen += LineLen;

         sprintf(line,"SC[%ld].AC.CfsCtrl.Kr = [ %le %le %le]\n",
            Isc,
            AC->CfsCtrl.Kr[0],
            AC->CfsCtrl.Kr[1],
            AC->CfsCtrl.Kr[2]);
         if (AC->EchoEnabled) printf("%s",line);
         LineLen = strlen(line);
         memcpy(&Msg[MsgLen],line,LineLen);
         MsgLen += LineLen;

         sprintf(line,"SC[%ld].AC.CfsCtrl.Kp = [ %le %le %le]\n",
            Isc,
            AC->CfsCtrl.Kp[0],
            AC->CfsCtrl.Kp[1],
            AC->CfsCtrl.Kp[2]);
         if (AC->EchoEnabled) printf("%s",line);
         LineLen = strlen(line);
         memcpy(&Msg[MsgLen],line,LineLen);
         MsgLen += LineLen;

         sprintf(line,"SC[%ld].AC.CfsCtrl.Kunl = [ %le]\n",
            Isc,
            AC->CfsCtrl.Kunl);
         if (AC->EchoEnabled) printf("%s",line);
         LineLen = strlen(line);
         memcpy(&Msg[MsgLen],line,LineLen);
         MsgLen += LineLen;

      }

      sprintf(line,"[EOF]\n\n");
      if (AC->EchoEnabled) printf("%s",line);

      LineLen = strlen(line);
      memcpy(&Msg[MsgLen],line,LineLen);
      MsgLen += LineLen;
      Success = send(Socket,Msg,strlen(Msg),0);

      /* Wait for Ack */
      recv(Socket,AckMsg,8,0);
}

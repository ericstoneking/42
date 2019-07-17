#include "42.h"

#include "gmseckit.h"
/**********************************************************************/
void WriteToGmsec(GMSEC_ConnectionMgr ConnMgr,GMSEC_Status status)
{

      long Isc,Iorb,Iw,i;
      char Header[40] = "GMSEC.42.TX.MSG.LOG";
      GMSEC_Message AckMsg;
      char Msg[16384];
      long MsgLen = 0;
      long LineLen;
      char line[512];

      sprintf(line,"TIME %ld-%03ld-%02ld:%02ld:%012.9lf\n",
         Year,doy,Hour,Minute,Second);
      LineLen = strlen(line);
      memcpy(&Msg[MsgLen],line,LineLen);
      MsgLen += LineLen;
      if (EchoEnabled) printf("%s",line);

      for(Isc=0;Isc<Nsc;Isc++) {
         if (SC[Isc].Exists) {
            sprintf(line,"SC[%ld].PosR = %18.12le %18.12le %18.12le\n",
               Isc,
               SC[Isc].PosR[0],
               SC[Isc].PosR[1],
               SC[Isc].PosR[2]);
            if (EchoEnabled) printf("%s",line);
            LineLen = strlen(line);
            memcpy(&Msg[MsgLen],line,LineLen);
            MsgLen += LineLen;

            sprintf(line,"SC[%ld].VelR = %18.12le %18.12le %18.12le\n",
               Isc,
               SC[Isc].VelR[0],
               SC[Isc].VelR[1],
               SC[Isc].VelR[2]);
            if (EchoEnabled) printf("%s",line);
            LineLen = strlen(line);
            memcpy(&Msg[MsgLen],line,LineLen);
            MsgLen += LineLen;

            sprintf(line,"SC[%ld].svb = %18.12le %18.12le %18.12le\n",
               Isc,
               SC[Isc].svb[0],
               SC[Isc].svb[1],
               SC[Isc].svb[2]);
            if (EchoEnabled) printf("%s",line);
            LineLen = strlen(line);
            memcpy(&Msg[MsgLen],line,LineLen);
            MsgLen += LineLen;

            sprintf(line,"SC[%ld].bvb = %18.12le %18.12le %18.12le\n",
               Isc,
               SC[Isc].bvb[0],
               SC[Isc].bvb[1],
               SC[Isc].bvb[2]);
            if (EchoEnabled) printf("%s",line);
            LineLen = strlen(line);
            memcpy(&Msg[MsgLen],line,LineLen);
            MsgLen += LineLen;

            sprintf(line,"SC[%ld].Hvb = %18.12le %18.12le %18.12le\n",
               Isc,
               SC[Isc].Hvb[0],
               SC[Isc].Hvb[1],
               SC[Isc].Hvb[2]);
            if (EchoEnabled) printf("%s",line);
            LineLen = strlen(line);
            memcpy(&Msg[MsgLen],line,LineLen);
            MsgLen += LineLen;

            sprintf(line,"SC[%ld].AC.ParmLoadEnabled = %ld\n",
               Isc,
               SC[Isc].AC.ParmLoadEnabled);
            if (EchoEnabled) printf("%s",line);
            LineLen = strlen(line);
            memcpy(&Msg[MsgLen],line,LineLen);
            MsgLen += LineLen;

            sprintf(line,"SC[%ld].AC.ParmDumpEnabled = %ld\n",
               Isc,
               SC[Isc].AC.ParmDumpEnabled);
            if (EchoEnabled) printf("%s",line);
            LineLen = strlen(line);
            memcpy(&Msg[MsgLen],line,LineLen);
            MsgLen += LineLen;

            for(i=0;i<SC[Isc].AC.Ng;i++) {
               sprintf(line,"SC[%ld].AC.G[%ld].Ang = %18.12le %18.12le %18.12le\n",
                  Isc,i,
                  SC[Isc].AC.G[i].Ang[0],
                  SC[Isc].AC.G[i].Ang[1],
                  SC[Isc].AC.G[i].Ang[2]);
               if (EchoEnabled) printf("%s",line);
               LineLen = strlen(line);
               memcpy(&Msg[MsgLen],line,LineLen);
               MsgLen += LineLen;

            }

            for(i=0;i<SC[Isc].AC.Ngyro;i++) {
               sprintf(line,"SC[%ld].AC.Gyro[%ld].Rate = %18.12le\n",
                  Isc,i,
                  SC[Isc].AC.Gyro[i].Rate);
               if (EchoEnabled) printf("%s",line);
               LineLen = strlen(line);
               memcpy(&Msg[MsgLen],line,LineLen);
               MsgLen += LineLen;

            }

            for(i=0;i<SC[Isc].AC.Nmag;i++) {
               sprintf(line,"SC[%ld].AC.MAG[%ld].Field = %18.12le\n",
                  Isc,i,
                  SC[Isc].AC.MAG[i].Field);
               if (EchoEnabled) printf("%s",line);
               LineLen = strlen(line);
               memcpy(&Msg[MsgLen],line,LineLen);
               MsgLen += LineLen;

            }

            for(i=0;i<SC[Isc].AC.Ncss;i++) {
               sprintf(line,"SC[%ld].AC.CSS[%ld].Valid = %ld\n",
                  Isc,i,
                  SC[Isc].AC.CSS[i].Valid);
               if (EchoEnabled) printf("%s",line);
               LineLen = strlen(line);
               memcpy(&Msg[MsgLen],line,LineLen);
               MsgLen += LineLen;

               sprintf(line,"SC[%ld].AC.CSS[%ld].Illum = %18.12le\n",
                  Isc,i,
                  SC[Isc].AC.CSS[i].Illum);
               if (EchoEnabled) printf("%s",line);
               LineLen = strlen(line);
               memcpy(&Msg[MsgLen],line,LineLen);
               MsgLen += LineLen;

            }

            for(i=0;i<SC[Isc].AC.Nfss;i++) {
               sprintf(line,"SC[%ld].AC.FSS[%ld].Valid = %ld\n",
                  Isc,i,
                  SC[Isc].AC.FSS[i].Valid);
               if (EchoEnabled) printf("%s",line);
               LineLen = strlen(line);
               memcpy(&Msg[MsgLen],line,LineLen);
               MsgLen += LineLen;

               sprintf(line,"SC[%ld].AC.FSS[%ld].SunAng = %18.12le %18.12le\n",
                  Isc,i,
                  SC[Isc].AC.FSS[i].SunAng[0],
                  SC[Isc].AC.FSS[i].SunAng[1]);
               if (EchoEnabled) printf("%s",line);
               LineLen = strlen(line);
               memcpy(&Msg[MsgLen],line,LineLen);
               MsgLen += LineLen;

            }

            for(i=0;i<SC[Isc].AC.Nst;i++) {
               sprintf(line,"SC[%ld].AC.ST[%ld].Valid = %ld\n",
                  Isc,i,
                  SC[Isc].AC.ST[i].Valid);
               if (EchoEnabled) printf("%s",line);
               LineLen = strlen(line);
               memcpy(&Msg[MsgLen],line,LineLen);
               MsgLen += LineLen;

               sprintf(line,"SC[%ld].AC.ST[%ld].qn = %18.12le %18.12le %18.12le %18.12le\n",
                  Isc,i,
                  SC[Isc].AC.ST[i].qn[0],
                  SC[Isc].AC.ST[i].qn[1],
                  SC[Isc].AC.ST[i].qn[2],
                  SC[Isc].AC.ST[i].qn[3]);
               if (EchoEnabled) printf("%s",line);
               LineLen = strlen(line);
               memcpy(&Msg[MsgLen],line,LineLen);
               MsgLen += LineLen;

            }

            for(i=0;i<SC[Isc].AC.Ngps;i++) {
               sprintf(line,"SC[%ld].AC.GPS[%ld].Valid = %ld\n",
                  Isc,i,
                  SC[Isc].AC.GPS[i].Valid);
               if (EchoEnabled) printf("%s",line);
               LineLen = strlen(line);
               memcpy(&Msg[MsgLen],line,LineLen);
               MsgLen += LineLen;

               sprintf(line,"SC[%ld].AC.GPS[%ld].Rollover = %ld\n",
                  Isc,i,
                  SC[Isc].AC.GPS[i].Rollover);
               if (EchoEnabled) printf("%s",line);
               LineLen = strlen(line);
               memcpy(&Msg[MsgLen],line,LineLen);
               MsgLen += LineLen;

               sprintf(line,"SC[%ld].AC.GPS[%ld].Week = %ld\n",
                  Isc,i,
                  SC[Isc].AC.GPS[i].Week);
               if (EchoEnabled) printf("%s",line);
               LineLen = strlen(line);
               memcpy(&Msg[MsgLen],line,LineLen);
               MsgLen += LineLen;

               sprintf(line,"SC[%ld].AC.GPS[%ld].Sec = %18.12le\n",
                  Isc,i,
                  SC[Isc].AC.GPS[i].Sec);
               if (EchoEnabled) printf("%s",line);
               LineLen = strlen(line);
               memcpy(&Msg[MsgLen],line,LineLen);
               MsgLen += LineLen;

               sprintf(line,"SC[%ld].AC.GPS[%ld].PosN = %18.12le %18.12le %18.12le\n",
                  Isc,i,
                  SC[Isc].AC.GPS[i].PosN[0],
                  SC[Isc].AC.GPS[i].PosN[1],
                  SC[Isc].AC.GPS[i].PosN[2]);
               if (EchoEnabled) printf("%s",line);
               LineLen = strlen(line);
               memcpy(&Msg[MsgLen],line,LineLen);
               MsgLen += LineLen;

               sprintf(line,"SC[%ld].AC.GPS[%ld].VelN = %18.12le %18.12le %18.12le\n",
                  Isc,i,
                  SC[Isc].AC.GPS[i].VelN[0],
                  SC[Isc].AC.GPS[i].VelN[1],
                  SC[Isc].AC.GPS[i].VelN[2]);
               if (EchoEnabled) printf("%s",line);
               LineLen = strlen(line);
               memcpy(&Msg[MsgLen],line,LineLen);
               MsgLen += LineLen;

               sprintf(line,"SC[%ld].AC.GPS[%ld].PosW = %18.12le %18.12le %18.12le\n",
                  Isc,i,
                  SC[Isc].AC.GPS[i].PosW[0],
                  SC[Isc].AC.GPS[i].PosW[1],
                  SC[Isc].AC.GPS[i].PosW[2]);
               if (EchoEnabled) printf("%s",line);
               LineLen = strlen(line);
               memcpy(&Msg[MsgLen],line,LineLen);
               MsgLen += LineLen;

               sprintf(line,"SC[%ld].AC.GPS[%ld].VelW = %18.12le %18.12le %18.12le\n",
                  Isc,i,
                  SC[Isc].AC.GPS[i].VelW[0],
                  SC[Isc].AC.GPS[i].VelW[1],
                  SC[Isc].AC.GPS[i].VelW[2]);
               if (EchoEnabled) printf("%s",line);
               LineLen = strlen(line);
               memcpy(&Msg[MsgLen],line,LineLen);
               MsgLen += LineLen;

               sprintf(line,"SC[%ld].AC.GPS[%ld].Lng = %18.12le\n",
                  Isc,i,
                  SC[Isc].AC.GPS[i].Lng);
               if (EchoEnabled) printf("%s",line);
               LineLen = strlen(line);
               memcpy(&Msg[MsgLen],line,LineLen);
               MsgLen += LineLen;

               sprintf(line,"SC[%ld].AC.GPS[%ld].Lat = %18.12le\n",
                  Isc,i,
                  SC[Isc].AC.GPS[i].Lat);
               if (EchoEnabled) printf("%s",line);
               LineLen = strlen(line);
               memcpy(&Msg[MsgLen],line,LineLen);
               MsgLen += LineLen;

               sprintf(line,"SC[%ld].AC.GPS[%ld].Alt = %18.12le\n",
                  Isc,i,
                  SC[Isc].AC.GPS[i].Alt);
               if (EchoEnabled) printf("%s",line);
               LineLen = strlen(line);
               memcpy(&Msg[MsgLen],line,LineLen);
               MsgLen += LineLen;

            }

            for(i=0;i<SC[Isc].AC.Nwhl;i++) {
               sprintf(line,"SC[%ld].AC.Whl[%ld].H = %18.12le\n",
                  Isc,i,
                  SC[Isc].AC.Whl[i].H);
               if (EchoEnabled) printf("%s",line);
               LineLen = strlen(line);
               memcpy(&Msg[MsgLen],line,LineLen);
               MsgLen += LineLen;

            }

            for(i=0;i<SC[Isc].Nb;i++) {
               sprintf(line,"SC[%ld].B[%ld].wn = %18.12le %18.12le %18.12le\n",
                  Isc,i,
                  SC[Isc].B[i].wn[0],
                  SC[Isc].B[i].wn[1],
                  SC[Isc].B[i].wn[2]);
               if (EchoEnabled) printf("%s",line);
               LineLen = strlen(line);
               memcpy(&Msg[MsgLen],line,LineLen);
               MsgLen += LineLen;

               sprintf(line,"SC[%ld].B[%ld].qn = %18.12le %18.12le %18.12le %18.12le\n",
                  Isc,i,
                  SC[Isc].B[i].qn[0],
                  SC[Isc].B[i].qn[1],
                  SC[Isc].B[i].qn[2],
                  SC[Isc].B[i].qn[3]);
               if (EchoEnabled) printf("%s",line);
               LineLen = strlen(line);
               memcpy(&Msg[MsgLen],line,LineLen);
               MsgLen += LineLen;

            }

            for(i=0;i<SC[Isc].Nw;i++) {
               sprintf(line,"SC[%ld].Whl[%ld].H = %18.12le\n",
                  Isc,i,
                  SC[Isc].Whl[i].H);
               if (EchoEnabled) printf("%s",line);
               LineLen = strlen(line);
               memcpy(&Msg[MsgLen],line,LineLen);
               MsgLen += LineLen;

            }

            for(i=0;i<SC[Isc].Ngyro;i++) {
               sprintf(line,"SC[%ld].Gyro[%ld].TrueRate = %18.12le\n",
                  Isc,i,
                  SC[Isc].Gyro[i].TrueRate);
               if (EchoEnabled) printf("%s",line);
               LineLen = strlen(line);
               memcpy(&Msg[MsgLen],line,LineLen);
               MsgLen += LineLen;

            }

         }
      }

      for(Iw=1;Iw<NWORLD;Iw++) {
         if (World[Iw].Exists) {
            sprintf(line,"World[%ld].PosH = %18.12le %18.12le %18.12le\n",
               Iw,
               World[Iw].PosH[0],
               World[Iw].PosH[1],
               World[Iw].PosH[2]);
            if (EchoEnabled) printf("%s",line);
            LineLen = strlen(line);
            memcpy(&Msg[MsgLen],line,LineLen);
            MsgLen += LineLen;

            sprintf(line,"World[%ld].eph.PosN = %18.12le %18.12le %18.12le\n",
               Iw,
               World[Iw].eph.PosN[0],
               World[Iw].eph.PosN[1],
               World[Iw].eph.PosN[2]);
            if (EchoEnabled) printf("%s",line);
            LineLen = strlen(line);
            memcpy(&Msg[MsgLen],line,LineLen);
            MsgLen += LineLen;

            sprintf(line,"World[%ld].eph.VelN = %18.12le %18.12le %18.12le\n",
               Iw,
               World[Iw].eph.VelN[0],
               World[Iw].eph.VelN[1],
               World[Iw].eph.VelN[2]);
            if (EchoEnabled) printf("%s",line);
            LineLen = strlen(line);
            memcpy(&Msg[MsgLen],line,LineLen);
            MsgLen += LineLen;

         }
      }

      for(Iorb=0;Iorb<Norb;Iorb++) {
         if (Orb[Iorb].Exists) {
            sprintf(line,"Orb[%ld].PosN = %18.12le %18.12le %18.12le\n",
               Iorb,
               Orb[Iorb].PosN[0],
               Orb[Iorb].PosN[1],
               Orb[Iorb].PosN[2]);
            if (EchoEnabled) printf("%s",line);
            LineLen = strlen(line);
            memcpy(&Msg[MsgLen],line,LineLen);
            MsgLen += LineLen;

            sprintf(line,"Orb[%ld].VelN = %18.12le %18.12le %18.12le\n",
               Iorb,
               Orb[Iorb].VelN[0],
               Orb[Iorb].VelN[1],
               Orb[Iorb].VelN[2]);
            if (EchoEnabled) printf("%s",line);
            LineLen = strlen(line);
            memcpy(&Msg[MsgLen],line,LineLen);
            MsgLen += LineLen;

         }
      }

      for(Isc=0;Isc<Nsc;Isc++) {
         if (SC[Isc].Exists) {
            if (SC[Isc].AC.ParmLoadEnabled) {
               sprintf(line,"SC[%ld].AC.ID = %ld\n",
                  Isc,
                  SC[Isc].AC.ID);
               if (EchoEnabled) printf("%s",line);
               LineLen = strlen(line);
               memcpy(&Msg[MsgLen],line,LineLen);
               MsgLen += LineLen;

               sprintf(line,"SC[%ld].AC.EchoEnabled = %ld\n",
                  Isc,
                  SC[Isc].AC.EchoEnabled);
               if (EchoEnabled) printf("%s",line);
               LineLen = strlen(line);
               memcpy(&Msg[MsgLen],line,LineLen);
               MsgLen += LineLen;

               sprintf(line,"SC[%ld].AC.Nb = %ld\n",
                  Isc,
                  SC[Isc].AC.Nb);
               if (EchoEnabled) printf("%s",line);
               LineLen = strlen(line);
               memcpy(&Msg[MsgLen],line,LineLen);
               MsgLen += LineLen;

               sprintf(line,"SC[%ld].AC.Ng = %ld\n",
                  Isc,
                  SC[Isc].AC.Ng);
               if (EchoEnabled) printf("%s",line);
               LineLen = strlen(line);
               memcpy(&Msg[MsgLen],line,LineLen);
               MsgLen += LineLen;

               sprintf(line,"SC[%ld].AC.Nwhl = %ld\n",
                  Isc,
                  SC[Isc].AC.Nwhl);
               if (EchoEnabled) printf("%s",line);
               LineLen = strlen(line);
               memcpy(&Msg[MsgLen],line,LineLen);
               MsgLen += LineLen;

               sprintf(line,"SC[%ld].AC.Nmtb = %ld\n",
                  Isc,
                  SC[Isc].AC.Nmtb);
               if (EchoEnabled) printf("%s",line);
               LineLen = strlen(line);
               memcpy(&Msg[MsgLen],line,LineLen);
               MsgLen += LineLen;

               sprintf(line,"SC[%ld].AC.Nthr = %ld\n",
                  Isc,
                  SC[Isc].AC.Nthr);
               if (EchoEnabled) printf("%s",line);
               LineLen = strlen(line);
               memcpy(&Msg[MsgLen],line,LineLen);
               MsgLen += LineLen;

               sprintf(line,"SC[%ld].AC.Ncmg = %ld\n",
                  Isc,
                  SC[Isc].AC.Ncmg);
               if (EchoEnabled) printf("%s",line);
               LineLen = strlen(line);
               memcpy(&Msg[MsgLen],line,LineLen);
               MsgLen += LineLen;

               sprintf(line,"SC[%ld].AC.Ngyro = %ld\n",
                  Isc,
                  SC[Isc].AC.Ngyro);
               if (EchoEnabled) printf("%s",line);
               LineLen = strlen(line);
               memcpy(&Msg[MsgLen],line,LineLen);
               MsgLen += LineLen;

               sprintf(line,"SC[%ld].AC.Nmag = %ld\n",
                  Isc,
                  SC[Isc].AC.Nmag);
               if (EchoEnabled) printf("%s",line);
               LineLen = strlen(line);
               memcpy(&Msg[MsgLen],line,LineLen);
               MsgLen += LineLen;

               sprintf(line,"SC[%ld].AC.Ncss = %ld\n",
                  Isc,
                  SC[Isc].AC.Ncss);
               if (EchoEnabled) printf("%s",line);
               LineLen = strlen(line);
               memcpy(&Msg[MsgLen],line,LineLen);
               MsgLen += LineLen;

               sprintf(line,"SC[%ld].AC.Nfss = %ld\n",
                  Isc,
                  SC[Isc].AC.Nfss);
               if (EchoEnabled) printf("%s",line);
               LineLen = strlen(line);
               memcpy(&Msg[MsgLen],line,LineLen);
               MsgLen += LineLen;

               sprintf(line,"SC[%ld].AC.Nst = %ld\n",
                  Isc,
                  SC[Isc].AC.Nst);
               if (EchoEnabled) printf("%s",line);
               LineLen = strlen(line);
               memcpy(&Msg[MsgLen],line,LineLen);
               MsgLen += LineLen;

               sprintf(line,"SC[%ld].AC.Ngps = %ld\n",
                  Isc,
                  SC[Isc].AC.Ngps);
               if (EchoEnabled) printf("%s",line);
               LineLen = strlen(line);
               memcpy(&Msg[MsgLen],line,LineLen);
               MsgLen += LineLen;

               sprintf(line,"SC[%ld].AC.Nacc = %ld\n",
                  Isc,
                  SC[Isc].AC.Nacc);
               if (EchoEnabled) printf("%s",line);
               LineLen = strlen(line);
               memcpy(&Msg[MsgLen],line,LineLen);
               MsgLen += LineLen;

               sprintf(line,"SC[%ld].AC.DT = %18.12le\n",
                  Isc,
                  SC[Isc].AC.DT);
               if (EchoEnabled) printf("%s",line);
               LineLen = strlen(line);
               memcpy(&Msg[MsgLen],line,LineLen);
               MsgLen += LineLen;

               sprintf(line,"SC[%ld].AC.mass = %18.12le\n",
                  Isc,
                  SC[Isc].AC.mass);
               if (EchoEnabled) printf("%s",line);
               LineLen = strlen(line);
               memcpy(&Msg[MsgLen],line,LineLen);
               MsgLen += LineLen;

               sprintf(line,"SC[%ld].AC.cm = %18.12le %18.12le %18.12le\n",
                  Isc,
                  SC[Isc].AC.cm[0],
                  SC[Isc].AC.cm[1],
                  SC[Isc].AC.cm[2]);
               if (EchoEnabled) printf("%s",line);
               LineLen = strlen(line);
               memcpy(&Msg[MsgLen],line,LineLen);
               MsgLen += LineLen;

               sprintf(line,"SC[%ld].AC.MOI = %18.12le %18.12le %18.12le %18.12le %18.12le %18.12le %18.12le %18.12le %18.12le\n",
                  Isc,
                  SC[Isc].AC.MOI[0][0],
                  SC[Isc].AC.MOI[0][1],
                  SC[Isc].AC.MOI[0][2],
                  SC[Isc].AC.MOI[1][0],
                  SC[Isc].AC.MOI[1][1],
                  SC[Isc].AC.MOI[1][2],
                  SC[Isc].AC.MOI[2][0],
                  SC[Isc].AC.MOI[2][1],
                  SC[Isc].AC.MOI[2][2]);
               if (EchoEnabled) printf("%s",line);
               LineLen = strlen(line);
               memcpy(&Msg[MsgLen],line,LineLen);
               MsgLen += LineLen;

               for(i=0;i<SC[Isc].AC.Nb;i++) {
                  sprintf(line,"SC[%ld].AC.B[%ld].mass = %18.12le\n",
                     Isc,i,
                     SC[Isc].AC.B[i].mass);
                  if (EchoEnabled) printf("%s",line);
                  LineLen = strlen(line);
                  memcpy(&Msg[MsgLen],line,LineLen);
                  MsgLen += LineLen;

                  sprintf(line,"SC[%ld].AC.B[%ld].cm = %18.12le %18.12le %18.12le\n",
                     Isc,i,
                     SC[Isc].AC.B[i].cm[0],
                     SC[Isc].AC.B[i].cm[1],
                     SC[Isc].AC.B[i].cm[2]);
                  if (EchoEnabled) printf("%s",line);
                  LineLen = strlen(line);
                  memcpy(&Msg[MsgLen],line,LineLen);
                  MsgLen += LineLen;

                  sprintf(line,"SC[%ld].AC.B[%ld].MOI = %18.12le %18.12le %18.12le %18.12le %18.12le %18.12le %18.12le %18.12le %18.12le\n",
                     Isc,i,
                     SC[Isc].AC.B[i].MOI[0][0],
                     SC[Isc].AC.B[i].MOI[0][1],
                     SC[Isc].AC.B[i].MOI[0][2],
                     SC[Isc].AC.B[i].MOI[1][0],
                     SC[Isc].AC.B[i].MOI[1][1],
                     SC[Isc].AC.B[i].MOI[1][2],
                     SC[Isc].AC.B[i].MOI[2][0],
                     SC[Isc].AC.B[i].MOI[2][1],
                     SC[Isc].AC.B[i].MOI[2][2]);
                  if (EchoEnabled) printf("%s",line);
                  LineLen = strlen(line);
                  memcpy(&Msg[MsgLen],line,LineLen);
                  MsgLen += LineLen;

               }

               for(i=0;i<SC[Isc].AC.Ng;i++) {
                  sprintf(line,"SC[%ld].AC.G[%ld].IsUnderActiveControl = %ld\n",
                     Isc,i,
                     SC[Isc].AC.G[i].IsUnderActiveControl);
                  if (EchoEnabled) printf("%s",line);
                  LineLen = strlen(line);
                  memcpy(&Msg[MsgLen],line,LineLen);
                  MsgLen += LineLen;

                  sprintf(line,"SC[%ld].AC.G[%ld].IsSpherical = %ld\n",
                     Isc,i,
                     SC[Isc].AC.G[i].IsSpherical);
                  if (EchoEnabled) printf("%s",line);
                  LineLen = strlen(line);
                  memcpy(&Msg[MsgLen],line,LineLen);
                  MsgLen += LineLen;

                  sprintf(line,"SC[%ld].AC.G[%ld].RotDOF = %ld\n",
                     Isc,i,
                     SC[Isc].AC.G[i].RotDOF);
                  if (EchoEnabled) printf("%s",line);
                  LineLen = strlen(line);
                  memcpy(&Msg[MsgLen],line,LineLen);
                  MsgLen += LineLen;

                  sprintf(line,"SC[%ld].AC.G[%ld].TrnDOF = %ld\n",
                     Isc,i,
                     SC[Isc].AC.G[i].TrnDOF);
                  if (EchoEnabled) printf("%s",line);
                  LineLen = strlen(line);
                  memcpy(&Msg[MsgLen],line,LineLen);
                  MsgLen += LineLen;

                  sprintf(line,"SC[%ld].AC.G[%ld].RotSeq = %ld\n",
                     Isc,i,
                     SC[Isc].AC.G[i].RotSeq);
                  if (EchoEnabled) printf("%s",line);
                  LineLen = strlen(line);
                  memcpy(&Msg[MsgLen],line,LineLen);
                  MsgLen += LineLen;

                  sprintf(line,"SC[%ld].AC.G[%ld].TrnSeq = %ld\n",
                     Isc,i,
                     SC[Isc].AC.G[i].TrnSeq);
                  if (EchoEnabled) printf("%s",line);
                  LineLen = strlen(line);
                  memcpy(&Msg[MsgLen],line,LineLen);
                  MsgLen += LineLen;

                  sprintf(line,"SC[%ld].AC.G[%ld].CGiBi = %18.12le %18.12le %18.12le %18.12le %18.12le %18.12le %18.12le %18.12le %18.12le\n",
                     Isc,i,
                     SC[Isc].AC.G[i].CGiBi[0][0],
                     SC[Isc].AC.G[i].CGiBi[0][1],
                     SC[Isc].AC.G[i].CGiBi[0][2],
                     SC[Isc].AC.G[i].CGiBi[1][0],
                     SC[Isc].AC.G[i].CGiBi[1][1],
                     SC[Isc].AC.G[i].CGiBi[1][2],
                     SC[Isc].AC.G[i].CGiBi[2][0],
                     SC[Isc].AC.G[i].CGiBi[2][1],
                     SC[Isc].AC.G[i].CGiBi[2][2]);
                  if (EchoEnabled) printf("%s",line);
                  LineLen = strlen(line);
                  memcpy(&Msg[MsgLen],line,LineLen);
                  MsgLen += LineLen;

                  sprintf(line,"SC[%ld].AC.G[%ld].CBoGo = %18.12le %18.12le %18.12le %18.12le %18.12le %18.12le %18.12le %18.12le %18.12le\n",
                     Isc,i,
                     SC[Isc].AC.G[i].CBoGo[0][0],
                     SC[Isc].AC.G[i].CBoGo[0][1],
                     SC[Isc].AC.G[i].CBoGo[0][2],
                     SC[Isc].AC.G[i].CBoGo[1][0],
                     SC[Isc].AC.G[i].CBoGo[1][1],
                     SC[Isc].AC.G[i].CBoGo[1][2],
                     SC[Isc].AC.G[i].CBoGo[2][0],
                     SC[Isc].AC.G[i].CBoGo[2][1],
                     SC[Isc].AC.G[i].CBoGo[2][2]);
                  if (EchoEnabled) printf("%s",line);
                  LineLen = strlen(line);
                  memcpy(&Msg[MsgLen],line,LineLen);
                  MsgLen += LineLen;

                  sprintf(line,"SC[%ld].AC.G[%ld].AngGain = %18.12le %18.12le %18.12le\n",
                     Isc,i,
                     SC[Isc].AC.G[i].AngGain[0],
                     SC[Isc].AC.G[i].AngGain[1],
                     SC[Isc].AC.G[i].AngGain[2]);
                  if (EchoEnabled) printf("%s",line);
                  LineLen = strlen(line);
                  memcpy(&Msg[MsgLen],line,LineLen);
                  MsgLen += LineLen;

                  sprintf(line,"SC[%ld].AC.G[%ld].AngRateGain = %18.12le %18.12le %18.12le\n",
                     Isc,i,
                     SC[Isc].AC.G[i].AngRateGain[0],
                     SC[Isc].AC.G[i].AngRateGain[1],
                     SC[Isc].AC.G[i].AngRateGain[2]);
                  if (EchoEnabled) printf("%s",line);
                  LineLen = strlen(line);
                  memcpy(&Msg[MsgLen],line,LineLen);
                  MsgLen += LineLen;

                  sprintf(line,"SC[%ld].AC.G[%ld].PosGain = %18.12le %18.12le %18.12le\n",
                     Isc,i,
                     SC[Isc].AC.G[i].PosGain[0],
                     SC[Isc].AC.G[i].PosGain[1],
                     SC[Isc].AC.G[i].PosGain[2]);
                  if (EchoEnabled) printf("%s",line);
                  LineLen = strlen(line);
                  memcpy(&Msg[MsgLen],line,LineLen);
                  MsgLen += LineLen;

                  sprintf(line,"SC[%ld].AC.G[%ld].PosRateGain = %18.12le %18.12le %18.12le\n",
                     Isc,i,
                     SC[Isc].AC.G[i].PosRateGain[0],
                     SC[Isc].AC.G[i].PosRateGain[1],
                     SC[Isc].AC.G[i].PosRateGain[2]);
                  if (EchoEnabled) printf("%s",line);
                  LineLen = strlen(line);
                  memcpy(&Msg[MsgLen],line,LineLen);
                  MsgLen += LineLen;

                  sprintf(line,"SC[%ld].AC.G[%ld].MaxAngRate = %18.12le %18.12le %18.12le\n",
                     Isc,i,
                     SC[Isc].AC.G[i].MaxAngRate[0],
                     SC[Isc].AC.G[i].MaxAngRate[1],
                     SC[Isc].AC.G[i].MaxAngRate[2]);
                  if (EchoEnabled) printf("%s",line);
                  LineLen = strlen(line);
                  memcpy(&Msg[MsgLen],line,LineLen);
                  MsgLen += LineLen;

                  sprintf(line,"SC[%ld].AC.G[%ld].MaxPosRate = %18.12le %18.12le %18.12le\n",
                     Isc,i,
                     SC[Isc].AC.G[i].MaxPosRate[0],
                     SC[Isc].AC.G[i].MaxPosRate[1],
                     SC[Isc].AC.G[i].MaxPosRate[2]);
                  if (EchoEnabled) printf("%s",line);
                  LineLen = strlen(line);
                  memcpy(&Msg[MsgLen],line,LineLen);
                  MsgLen += LineLen;

                  sprintf(line,"SC[%ld].AC.G[%ld].MaxTrq = %18.12le %18.12le %18.12le\n",
                     Isc,i,
                     SC[Isc].AC.G[i].MaxTrq[0],
                     SC[Isc].AC.G[i].MaxTrq[1],
                     SC[Isc].AC.G[i].MaxTrq[2]);
                  if (EchoEnabled) printf("%s",line);
                  LineLen = strlen(line);
                  memcpy(&Msg[MsgLen],line,LineLen);
                  MsgLen += LineLen;

                  sprintf(line,"SC[%ld].AC.G[%ld].MaxFrc = %18.12le %18.12le %18.12le\n",
                     Isc,i,
                     SC[Isc].AC.G[i].MaxFrc[0],
                     SC[Isc].AC.G[i].MaxFrc[1],
                     SC[Isc].AC.G[i].MaxFrc[2]);
                  if (EchoEnabled) printf("%s",line);
                  LineLen = strlen(line);
                  memcpy(&Msg[MsgLen],line,LineLen);
                  MsgLen += LineLen;

               }

               for(i=0;i<SC[Isc].AC.Ngyro;i++) {
                  sprintf(line,"SC[%ld].AC.Gyro[%ld].Axis = %18.12le %18.12le %18.12le\n",
                     Isc,i,
                     SC[Isc].AC.Gyro[i].Axis[0],
                     SC[Isc].AC.Gyro[i].Axis[1],
                     SC[Isc].AC.Gyro[i].Axis[2]);
                  if (EchoEnabled) printf("%s",line);
                  LineLen = strlen(line);
                  memcpy(&Msg[MsgLen],line,LineLen);
                  MsgLen += LineLen;

               }

               for(i=0;i<SC[Isc].AC.Nmag;i++) {
                  sprintf(line,"SC[%ld].AC.MAG[%ld].Axis = %18.12le %18.12le %18.12le\n",
                     Isc,i,
                     SC[Isc].AC.MAG[i].Axis[0],
                     SC[Isc].AC.MAG[i].Axis[1],
                     SC[Isc].AC.MAG[i].Axis[2]);
                  if (EchoEnabled) printf("%s",line);
                  LineLen = strlen(line);
                  memcpy(&Msg[MsgLen],line,LineLen);
                  MsgLen += LineLen;

               }

               for(i=0;i<SC[Isc].AC.Ncss;i++) {
                  sprintf(line,"SC[%ld].AC.CSS[%ld].Body = %ld\n",
                     Isc,i,
                     SC[Isc].AC.CSS[i].Body);
                  if (EchoEnabled) printf("%s",line);
                  LineLen = strlen(line);
                  memcpy(&Msg[MsgLen],line,LineLen);
                  MsgLen += LineLen;

                  sprintf(line,"SC[%ld].AC.CSS[%ld].Axis = %18.12le %18.12le %18.12le\n",
                     Isc,i,
                     SC[Isc].AC.CSS[i].Axis[0],
                     SC[Isc].AC.CSS[i].Axis[1],
                     SC[Isc].AC.CSS[i].Axis[2]);
                  if (EchoEnabled) printf("%s",line);
                  LineLen = strlen(line);
                  memcpy(&Msg[MsgLen],line,LineLen);
                  MsgLen += LineLen;

                  sprintf(line,"SC[%ld].AC.CSS[%ld].Scale = %18.12le\n",
                     Isc,i,
                     SC[Isc].AC.CSS[i].Scale);
                  if (EchoEnabled) printf("%s",line);
                  LineLen = strlen(line);
                  memcpy(&Msg[MsgLen],line,LineLen);
                  MsgLen += LineLen;

               }

               for(i=0;i<SC[Isc].AC.Nfss;i++) {
                  sprintf(line,"SC[%ld].AC.FSS[%ld].qb = %18.12le %18.12le %18.12le %18.12le\n",
                     Isc,i,
                     SC[Isc].AC.FSS[i].qb[0],
                     SC[Isc].AC.FSS[i].qb[1],
                     SC[Isc].AC.FSS[i].qb[2],
                     SC[Isc].AC.FSS[i].qb[3]);
                  if (EchoEnabled) printf("%s",line);
                  LineLen = strlen(line);
                  memcpy(&Msg[MsgLen],line,LineLen);
                  MsgLen += LineLen;

                  sprintf(line,"SC[%ld].AC.FSS[%ld].CB = %18.12le %18.12le %18.12le %18.12le %18.12le %18.12le %18.12le %18.12le %18.12le\n",
                     Isc,i,
                     SC[Isc].AC.FSS[i].CB[0][0],
                     SC[Isc].AC.FSS[i].CB[0][1],
                     SC[Isc].AC.FSS[i].CB[0][2],
                     SC[Isc].AC.FSS[i].CB[1][0],
                     SC[Isc].AC.FSS[i].CB[1][1],
                     SC[Isc].AC.FSS[i].CB[1][2],
                     SC[Isc].AC.FSS[i].CB[2][0],
                     SC[Isc].AC.FSS[i].CB[2][1],
                     SC[Isc].AC.FSS[i].CB[2][2]);
                  if (EchoEnabled) printf("%s",line);
                  LineLen = strlen(line);
                  memcpy(&Msg[MsgLen],line,LineLen);
                  MsgLen += LineLen;

               }

               for(i=0;i<SC[Isc].AC.Nst;i++) {
                  sprintf(line,"SC[%ld].AC.ST[%ld].qb = %18.12le %18.12le %18.12le %18.12le\n",
                     Isc,i,
                     SC[Isc].AC.ST[i].qb[0],
                     SC[Isc].AC.ST[i].qb[1],
                     SC[Isc].AC.ST[i].qb[2],
                     SC[Isc].AC.ST[i].qb[3]);
                  if (EchoEnabled) printf("%s",line);
                  LineLen = strlen(line);
                  memcpy(&Msg[MsgLen],line,LineLen);
                  MsgLen += LineLen;

                  sprintf(line,"SC[%ld].AC.ST[%ld].CB = %18.12le %18.12le %18.12le %18.12le %18.12le %18.12le %18.12le %18.12le %18.12le\n",
                     Isc,i,
                     SC[Isc].AC.ST[i].CB[0][0],
                     SC[Isc].AC.ST[i].CB[0][1],
                     SC[Isc].AC.ST[i].CB[0][2],
                     SC[Isc].AC.ST[i].CB[1][0],
                     SC[Isc].AC.ST[i].CB[1][1],
                     SC[Isc].AC.ST[i].CB[1][2],
                     SC[Isc].AC.ST[i].CB[2][0],
                     SC[Isc].AC.ST[i].CB[2][1],
                     SC[Isc].AC.ST[i].CB[2][2]);
                  if (EchoEnabled) printf("%s",line);
                  LineLen = strlen(line);
                  memcpy(&Msg[MsgLen],line,LineLen);
                  MsgLen += LineLen;

               }

               for(i=0;i<SC[Isc].AC.Nacc;i++) {
                  sprintf(line,"SC[%ld].AC.Accel[%ld].PosB = %18.12le %18.12le %18.12le\n",
                     Isc,i,
                     SC[Isc].AC.Accel[i].PosB[0],
                     SC[Isc].AC.Accel[i].PosB[1],
                     SC[Isc].AC.Accel[i].PosB[2]);
                  if (EchoEnabled) printf("%s",line);
                  LineLen = strlen(line);
                  memcpy(&Msg[MsgLen],line,LineLen);
                  MsgLen += LineLen;

                  sprintf(line,"SC[%ld].AC.Accel[%ld].Axis = %18.12le %18.12le %18.12le\n",
                     Isc,i,
                     SC[Isc].AC.Accel[i].Axis[0],
                     SC[Isc].AC.Accel[i].Axis[1],
                     SC[Isc].AC.Accel[i].Axis[2]);
                  if (EchoEnabled) printf("%s",line);
                  LineLen = strlen(line);
                  memcpy(&Msg[MsgLen],line,LineLen);
                  MsgLen += LineLen;

               }

               for(i=0;i<SC[Isc].AC.Nwhl;i++) {
                  sprintf(line,"SC[%ld].AC.Whl[%ld].Axis = %18.12le %18.12le %18.12le\n",
                     Isc,i,
                     SC[Isc].AC.Whl[i].Axis[0],
                     SC[Isc].AC.Whl[i].Axis[1],
                     SC[Isc].AC.Whl[i].Axis[2]);
                  if (EchoEnabled) printf("%s",line);
                  LineLen = strlen(line);
                  memcpy(&Msg[MsgLen],line,LineLen);
                  MsgLen += LineLen;

                  sprintf(line,"SC[%ld].AC.Whl[%ld].DistVec = %18.12le %18.12le %18.12le\n",
                     Isc,i,
                     SC[Isc].AC.Whl[i].DistVec[0],
                     SC[Isc].AC.Whl[i].DistVec[1],
                     SC[Isc].AC.Whl[i].DistVec[2]);
                  if (EchoEnabled) printf("%s",line);
                  LineLen = strlen(line);
                  memcpy(&Msg[MsgLen],line,LineLen);
                  MsgLen += LineLen;

                  sprintf(line,"SC[%ld].AC.Whl[%ld].J = %18.12le\n",
                     Isc,i,
                     SC[Isc].AC.Whl[i].J);
                  if (EchoEnabled) printf("%s",line);
                  LineLen = strlen(line);
                  memcpy(&Msg[MsgLen],line,LineLen);
                  MsgLen += LineLen;

                  sprintf(line,"SC[%ld].AC.Whl[%ld].Tmax = %18.12le\n",
                     Isc,i,
                     SC[Isc].AC.Whl[i].Tmax);
                  if (EchoEnabled) printf("%s",line);
                  LineLen = strlen(line);
                  memcpy(&Msg[MsgLen],line,LineLen);
                  MsgLen += LineLen;

                  sprintf(line,"SC[%ld].AC.Whl[%ld].Hmax = %18.12le\n",
                     Isc,i,
                     SC[Isc].AC.Whl[i].Hmax);
                  if (EchoEnabled) printf("%s",line);
                  LineLen = strlen(line);
                  memcpy(&Msg[MsgLen],line,LineLen);
                  MsgLen += LineLen;

               }

               for(i=0;i<SC[Isc].AC.Nmtb;i++) {
                  sprintf(line,"SC[%ld].AC.MTB[%ld].Axis = %18.12le %18.12le %18.12le\n",
                     Isc,i,
                     SC[Isc].AC.MTB[i].Axis[0],
                     SC[Isc].AC.MTB[i].Axis[1],
                     SC[Isc].AC.MTB[i].Axis[2]);
                  if (EchoEnabled) printf("%s",line);
                  LineLen = strlen(line);
                  memcpy(&Msg[MsgLen],line,LineLen);
                  MsgLen += LineLen;

                  sprintf(line,"SC[%ld].AC.MTB[%ld].DistVec = %18.12le %18.12le %18.12le\n",
                     Isc,i,
                     SC[Isc].AC.MTB[i].DistVec[0],
                     SC[Isc].AC.MTB[i].DistVec[1],
                     SC[Isc].AC.MTB[i].DistVec[2]);
                  if (EchoEnabled) printf("%s",line);
                  LineLen = strlen(line);
                  memcpy(&Msg[MsgLen],line,LineLen);
                  MsgLen += LineLen;

                  sprintf(line,"SC[%ld].AC.MTB[%ld].Mmax = %18.12le\n",
                     Isc,i,
                     SC[Isc].AC.MTB[i].Mmax);
                  if (EchoEnabled) printf("%s",line);
                  LineLen = strlen(line);
                  memcpy(&Msg[MsgLen],line,LineLen);
                  MsgLen += LineLen;

               }

               for(i=0;i<SC[Isc].AC.Nthr;i++) {
                  sprintf(line,"SC[%ld].AC.Thr[%ld].PosB = %18.12le %18.12le %18.12le\n",
                     Isc,i,
                     SC[Isc].AC.Thr[i].PosB[0],
                     SC[Isc].AC.Thr[i].PosB[1],
                     SC[Isc].AC.Thr[i].PosB[2]);
                  if (EchoEnabled) printf("%s",line);
                  LineLen = strlen(line);
                  memcpy(&Msg[MsgLen],line,LineLen);
                  MsgLen += LineLen;

                  sprintf(line,"SC[%ld].AC.Thr[%ld].Axis = %18.12le %18.12le %18.12le\n",
                     Isc,i,
                     SC[Isc].AC.Thr[i].Axis[0],
                     SC[Isc].AC.Thr[i].Axis[1],
                     SC[Isc].AC.Thr[i].Axis[2]);
                  if (EchoEnabled) printf("%s",line);
                  LineLen = strlen(line);
                  memcpy(&Msg[MsgLen],line,LineLen);
                  MsgLen += LineLen;

                  sprintf(line,"SC[%ld].AC.Thr[%ld].rxA = %18.12le %18.12le %18.12le\n",
                     Isc,i,
                     SC[Isc].AC.Thr[i].rxA[0],
                     SC[Isc].AC.Thr[i].rxA[1],
                     SC[Isc].AC.Thr[i].rxA[2]);
                  if (EchoEnabled) printf("%s",line);
                  LineLen = strlen(line);
                  memcpy(&Msg[MsgLen],line,LineLen);
                  MsgLen += LineLen;

                  sprintf(line,"SC[%ld].AC.Thr[%ld].Fmax = %18.12le\n",
                     Isc,i,
                     SC[Isc].AC.Thr[i].Fmax);
                  if (EchoEnabled) printf("%s",line);
                  LineLen = strlen(line);
                  memcpy(&Msg[MsgLen],line,LineLen);
                  MsgLen += LineLen;

               }

               sprintf(line,"SC[%ld].AC.PrototypeCtrl.wc = %18.12le\n",
                  Isc,
                  SC[Isc].AC.PrototypeCtrl.wc);
               if (EchoEnabled) printf("%s",line);
               LineLen = strlen(line);
               memcpy(&Msg[MsgLen],line,LineLen);
               MsgLen += LineLen;

               sprintf(line,"SC[%ld].AC.PrototypeCtrl.amax = %18.12le\n",
                  Isc,
                  SC[Isc].AC.PrototypeCtrl.amax);
               if (EchoEnabled) printf("%s",line);
               LineLen = strlen(line);
               memcpy(&Msg[MsgLen],line,LineLen);
               MsgLen += LineLen;

               sprintf(line,"SC[%ld].AC.PrototypeCtrl.vmax = %18.12le\n",
                  Isc,
                  SC[Isc].AC.PrototypeCtrl.vmax);
               if (EchoEnabled) printf("%s",line);
               LineLen = strlen(line);
               memcpy(&Msg[MsgLen],line,LineLen);
               MsgLen += LineLen;

               sprintf(line,"SC[%ld].AC.PrototypeCtrl.Kprec = %18.12le\n",
                  Isc,
                  SC[Isc].AC.PrototypeCtrl.Kprec);
               if (EchoEnabled) printf("%s",line);
               LineLen = strlen(line);
               memcpy(&Msg[MsgLen],line,LineLen);
               MsgLen += LineLen;

               sprintf(line,"SC[%ld].AC.PrototypeCtrl.Knute = %18.12le\n",
                  Isc,
                  SC[Isc].AC.PrototypeCtrl.Knute);
               if (EchoEnabled) printf("%s",line);
               LineLen = strlen(line);
               memcpy(&Msg[MsgLen],line,LineLen);
               MsgLen += LineLen;

               sprintf(line,"SC[%ld].AC.AdHocCtrl.Kr = %18.12le %18.12le %18.12le\n",
                  Isc,
                  SC[Isc].AC.AdHocCtrl.Kr[0],
                  SC[Isc].AC.AdHocCtrl.Kr[1],
                  SC[Isc].AC.AdHocCtrl.Kr[2]);
               if (EchoEnabled) printf("%s",line);
               LineLen = strlen(line);
               memcpy(&Msg[MsgLen],line,LineLen);
               MsgLen += LineLen;

               sprintf(line,"SC[%ld].AC.AdHocCtrl.Kp = %18.12le %18.12le %18.12le\n",
                  Isc,
                  SC[Isc].AC.AdHocCtrl.Kp[0],
                  SC[Isc].AC.AdHocCtrl.Kp[1],
                  SC[Isc].AC.AdHocCtrl.Kp[2]);
               if (EchoEnabled) printf("%s",line);
               LineLen = strlen(line);
               memcpy(&Msg[MsgLen],line,LineLen);
               MsgLen += LineLen;

               sprintf(line,"SC[%ld].AC.SpinnerCtrl.Ispin = %18.12le\n",
                  Isc,
                  SC[Isc].AC.SpinnerCtrl.Ispin);
               if (EchoEnabled) printf("%s",line);
               LineLen = strlen(line);
               memcpy(&Msg[MsgLen],line,LineLen);
               MsgLen += LineLen;

               sprintf(line,"SC[%ld].AC.SpinnerCtrl.Itrans = %18.12le\n",
                  Isc,
                  SC[Isc].AC.SpinnerCtrl.Itrans);
               if (EchoEnabled) printf("%s",line);
               LineLen = strlen(line);
               memcpy(&Msg[MsgLen],line,LineLen);
               MsgLen += LineLen;

               sprintf(line,"SC[%ld].AC.SpinnerCtrl.SpinRate = %18.12le\n",
                  Isc,
                  SC[Isc].AC.SpinnerCtrl.SpinRate);
               if (EchoEnabled) printf("%s",line);
               LineLen = strlen(line);
               memcpy(&Msg[MsgLen],line,LineLen);
               MsgLen += LineLen;

               sprintf(line,"SC[%ld].AC.SpinnerCtrl.Knute = %18.12le\n",
                  Isc,
                  SC[Isc].AC.SpinnerCtrl.Knute);
               if (EchoEnabled) printf("%s",line);
               LineLen = strlen(line);
               memcpy(&Msg[MsgLen],line,LineLen);
               MsgLen += LineLen;

               sprintf(line,"SC[%ld].AC.SpinnerCtrl.Kprec = %18.12le\n",
                  Isc,
                  SC[Isc].AC.SpinnerCtrl.Kprec);
               if (EchoEnabled) printf("%s",line);
               LineLen = strlen(line);
               memcpy(&Msg[MsgLen],line,LineLen);
               MsgLen += LineLen;

               sprintf(line,"SC[%ld].AC.ThreeAxisCtrl.Kr = %18.12le %18.12le %18.12le\n",
                  Isc,
                  SC[Isc].AC.ThreeAxisCtrl.Kr[0],
                  SC[Isc].AC.ThreeAxisCtrl.Kr[1],
                  SC[Isc].AC.ThreeAxisCtrl.Kr[2]);
               if (EchoEnabled) printf("%s",line);
               LineLen = strlen(line);
               memcpy(&Msg[MsgLen],line,LineLen);
               MsgLen += LineLen;

               sprintf(line,"SC[%ld].AC.ThreeAxisCtrl.Kp = %18.12le %18.12le %18.12le\n",
                  Isc,
                  SC[Isc].AC.ThreeAxisCtrl.Kp[0],
                  SC[Isc].AC.ThreeAxisCtrl.Kp[1],
                  SC[Isc].AC.ThreeAxisCtrl.Kp[2]);
               if (EchoEnabled) printf("%s",line);
               LineLen = strlen(line);
               memcpy(&Msg[MsgLen],line,LineLen);
               MsgLen += LineLen;

               sprintf(line,"SC[%ld].AC.ThreeAxisCtrl.Kunl = %18.12le\n",
                  Isc,
                  SC[Isc].AC.ThreeAxisCtrl.Kunl);
               if (EchoEnabled) printf("%s",line);
               LineLen = strlen(line);
               memcpy(&Msg[MsgLen],line,LineLen);
               MsgLen += LineLen;

               sprintf(line,"SC[%ld].AC.IssCtrl.Kr = %18.12le %18.12le %18.12le\n",
                  Isc,
                  SC[Isc].AC.IssCtrl.Kr[0],
                  SC[Isc].AC.IssCtrl.Kr[1],
                  SC[Isc].AC.IssCtrl.Kr[2]);
               if (EchoEnabled) printf("%s",line);
               LineLen = strlen(line);
               memcpy(&Msg[MsgLen],line,LineLen);
               MsgLen += LineLen;

               sprintf(line,"SC[%ld].AC.IssCtrl.Kp = %18.12le %18.12le %18.12le\n",
                  Isc,
                  SC[Isc].AC.IssCtrl.Kp[0],
                  SC[Isc].AC.IssCtrl.Kp[1],
                  SC[Isc].AC.IssCtrl.Kp[2]);
               if (EchoEnabled) printf("%s",line);
               LineLen = strlen(line);
               memcpy(&Msg[MsgLen],line,LineLen);
               MsgLen += LineLen;

               sprintf(line,"SC[%ld].AC.IssCtrl.Tmax = %18.12le\n",
                  Isc,
                  SC[Isc].AC.IssCtrl.Tmax);
               if (EchoEnabled) printf("%s",line);
               LineLen = strlen(line);
               memcpy(&Msg[MsgLen],line,LineLen);
               MsgLen += LineLen;

               sprintf(line,"SC[%ld].AC.CmgCtrl.Kr = %18.12le %18.12le %18.12le\n",
                  Isc,
                  SC[Isc].AC.CmgCtrl.Kr[0],
                  SC[Isc].AC.CmgCtrl.Kr[1],
                  SC[Isc].AC.CmgCtrl.Kr[2]);
               if (EchoEnabled) printf("%s",line);
               LineLen = strlen(line);
               memcpy(&Msg[MsgLen],line,LineLen);
               MsgLen += LineLen;

               sprintf(line,"SC[%ld].AC.CmgCtrl.Kp = %18.12le %18.12le %18.12le\n",
                  Isc,
                  SC[Isc].AC.CmgCtrl.Kp[0],
                  SC[Isc].AC.CmgCtrl.Kp[1],
                  SC[Isc].AC.CmgCtrl.Kp[2]);
               if (EchoEnabled) printf("%s",line);
               LineLen = strlen(line);
               memcpy(&Msg[MsgLen],line,LineLen);
               MsgLen += LineLen;

               sprintf(line,"SC[%ld].AC.ThrCtrl.Kw = %18.12le %18.12le %18.12le\n",
                  Isc,
                  SC[Isc].AC.ThrCtrl.Kw[0],
                  SC[Isc].AC.ThrCtrl.Kw[1],
                  SC[Isc].AC.ThrCtrl.Kw[2]);
               if (EchoEnabled) printf("%s",line);
               LineLen = strlen(line);
               memcpy(&Msg[MsgLen],line,LineLen);
               MsgLen += LineLen;

               sprintf(line,"SC[%ld].AC.ThrCtrl.Kth = %18.12le %18.12le %18.12le\n",
                  Isc,
                  SC[Isc].AC.ThrCtrl.Kth[0],
                  SC[Isc].AC.ThrCtrl.Kth[1],
                  SC[Isc].AC.ThrCtrl.Kth[2]);
               if (EchoEnabled) printf("%s",line);
               LineLen = strlen(line);
               memcpy(&Msg[MsgLen],line,LineLen);
               MsgLen += LineLen;

               sprintf(line,"SC[%ld].AC.ThrCtrl.Kv = %18.12le\n",
                  Isc,
                  SC[Isc].AC.ThrCtrl.Kv);
               if (EchoEnabled) printf("%s",line);
               LineLen = strlen(line);
               memcpy(&Msg[MsgLen],line,LineLen);
               MsgLen += LineLen;

               sprintf(line,"SC[%ld].AC.ThrCtrl.Kp = %18.12le\n",
                  Isc,
                  SC[Isc].AC.ThrCtrl.Kp);
               if (EchoEnabled) printf("%s",line);
               LineLen = strlen(line);
               memcpy(&Msg[MsgLen],line,LineLen);
               MsgLen += LineLen;

               sprintf(line,"SC[%ld].AC.CfsCtrl.Kr = %18.12le %18.12le %18.12le\n",
                  Isc,
                  SC[Isc].AC.CfsCtrl.Kr[0],
                  SC[Isc].AC.CfsCtrl.Kr[1],
                  SC[Isc].AC.CfsCtrl.Kr[2]);
               if (EchoEnabled) printf("%s",line);
               LineLen = strlen(line);
               memcpy(&Msg[MsgLen],line,LineLen);
               MsgLen += LineLen;

               sprintf(line,"SC[%ld].AC.CfsCtrl.Kp = %18.12le %18.12le %18.12le\n",
                  Isc,
                  SC[Isc].AC.CfsCtrl.Kp[0],
                  SC[Isc].AC.CfsCtrl.Kp[1],
                  SC[Isc].AC.CfsCtrl.Kp[2]);
               if (EchoEnabled) printf("%s",line);
               LineLen = strlen(line);
               memcpy(&Msg[MsgLen],line,LineLen);
               MsgLen += LineLen;

               sprintf(line,"SC[%ld].AC.CfsCtrl.Kunl = %18.12le\n",
                  Isc,
                  SC[Isc].AC.CfsCtrl.Kunl);
               if (EchoEnabled) printf("%s",line);
               LineLen = strlen(line);
               memcpy(&Msg[MsgLen],line,LineLen);
               MsgLen += LineLen;

            }
         }
      }

      sprintf(line,"[EOF]\n\n");
      if (EchoEnabled) printf("%s",line);

      LineLen = strlen(line);
      memcpy(&Msg[MsgLen],line,LineLen);
      MsgLen += LineLen;
      GmsecSend(Header,Msg,ConnMgr,status);
      /* Wait for ack */
      AckMsg = connectionManagerReceive(ConnMgr,GMSEC_WAIT_FOREVER,status);
      CheckGmsecStatus(status);
      messageDestroy(&AckMsg);
}

#include "42.h"

/**********************************************************************/
void WriteToSocket(SOCKET Socket)
{

      long Isc,Iorb,Iw,i;
      int Success;
      char AckMsg[8] = "Ack\n";
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
            sprintf(line,"SC[%ld].PosR = [ %le %le %le]\n",
               Isc,
               SC[Isc].PosR[0],
               SC[Isc].PosR[1],
               SC[Isc].PosR[2]);
            if (EchoEnabled) printf("%s",line);
            LineLen = strlen(line);
            memcpy(&Msg[MsgLen],line,LineLen);
            MsgLen += LineLen;

            sprintf(line,"SC[%ld].VelR = [ %le %le %le]\n",
               Isc,
               SC[Isc].VelR[0],
               SC[Isc].VelR[1],
               SC[Isc].VelR[2]);
            if (EchoEnabled) printf("%s",line);
            LineLen = strlen(line);
            memcpy(&Msg[MsgLen],line,LineLen);
            MsgLen += LineLen;

            sprintf(line,"SC[%ld].svb = [ %le %le %le]\n",
               Isc,
               SC[Isc].svb[0],
               SC[Isc].svb[1],
               SC[Isc].svb[2]);
            if (EchoEnabled) printf("%s",line);
            LineLen = strlen(line);
            memcpy(&Msg[MsgLen],line,LineLen);
            MsgLen += LineLen;

            sprintf(line,"SC[%ld].bvb = [ %le %le %le]\n",
               Isc,
               SC[Isc].bvb[0],
               SC[Isc].bvb[1],
               SC[Isc].bvb[2]);
            if (EchoEnabled) printf("%s",line);
            LineLen = strlen(line);
            memcpy(&Msg[MsgLen],line,LineLen);
            MsgLen += LineLen;

            sprintf(line,"SC[%ld].Hvb = [ %le %le %le]\n",
               Isc,
               SC[Isc].Hvb[0],
               SC[Isc].Hvb[1],
               SC[Isc].Hvb[2]);
            if (EchoEnabled) printf("%s",line);
            LineLen = strlen(line);
            memcpy(&Msg[MsgLen],line,LineLen);
            MsgLen += LineLen;

         }
      }

      for(Iw=1;Iw<NWORLD;Iw++) {
         if (World[Iw].Exists) {
            sprintf(line,"World[%ld].PosH = [ %le %le %le]\n",
               Iw,
               World[Iw].PosH[0],
               World[Iw].PosH[1],
               World[Iw].PosH[2]);
            if (EchoEnabled) printf("%s",line);
            LineLen = strlen(line);
            memcpy(&Msg[MsgLen],line,LineLen);
            MsgLen += LineLen;

         }
      }

      for(Iorb=0;Iorb<Norb;Iorb++) {
         if (Orb[Iorb].Exists) {
            sprintf(line,"Orb[%ld].PosN = [ %le %le %le]\n",
               Iorb,
               Orb[Iorb].PosN[0],
               Orb[Iorb].PosN[1],
               Orb[Iorb].PosN[2]);
            if (EchoEnabled) printf("%s",line);
            LineLen = strlen(line);
            memcpy(&Msg[MsgLen],line,LineLen);
            MsgLen += LineLen;

            sprintf(line,"Orb[%ld].VelN = [ %le %le %le]\n",
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
            }
         }
      }

      sprintf(line,"[EOF]\n\n");
      if (EchoEnabled) printf("%s",line);

      LineLen = strlen(line);
      memcpy(&Msg[MsgLen],line,LineLen);
      MsgLen += LineLen;
      Success = send(Socket,Msg,strlen(Msg),0);

      /* Wait for Ack */
      recv(Socket,AckMsg,8,0);
}

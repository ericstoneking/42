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

      if (AC->ParmDumpEnabled) {
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

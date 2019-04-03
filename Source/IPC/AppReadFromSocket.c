#include "Ac.h"

/**********************************************************************/
void ReadFromSocket(SOCKET Socket, struct AcType *AC)
{

      struct SCType *S;
      struct OrbitType *O;
      struct DynType *D;
      long Isc,Iorb,Iw,i;
      char line[512] = "Blank";
      long RequestTimeRefresh = 0;
      long Done;
      char Msg[16384];
      char AckMsg[8] = "Ack\n";
      long Imsg,Iline;
      double DbleVal[30];
      long LongVal[30];

      long Year,doy,Month,Day,Hour,Minute;
      double Second;
      long MsgLen;

      memset(Msg,'\0',16384);
      MsgLen = 0;
      recv(Socket,Msg,16384,0);

      Done = 0;
      Imsg = 0;
      while(!Done) {
         /* Parse lines from Msg, newline-delimited */
         Iline = 0;
         memset(line,'\0',512);
         while(Msg[Imsg] != '\n') {
            line[Iline++] = Msg[Imsg++];
         }
         line[Iline++] = Msg[Imsg++];
         if (AC->EchoEnabled) printf("%s",line);

         if (sscanf(line,"TIME %ld-%ld-%ld:%ld:%lf\n",
            &Year,&doy,&Hour,&Minute,&Second) == 5)
            RequestTimeRefresh = 1;

         if (sscanf(line,"SC[%ld].AC.ParmLoadEnabled = [ %ld]",
            &Isc,
            &LongVal[0]) == 2) {
            if (Isc == AC->ID) {
               AC->ParmLoadEnabled = LongVal[0];
            }
         }

         if (sscanf(line,"SC[%ld].AC.ParmDumpEnabled = [ %ld]",
            &Isc,
            &LongVal[0]) == 2) {
            if (Isc == AC->ID) {
               AC->ParmDumpEnabled = LongVal[0];
            }
         }

         if (AC->ParmLoadEnabled) {
         }


         if (!strncmp(line,"[EOF]",5)) {
            Done = 1;
            sprintf(line,"[EOF] reached\n");
         }
         if (Imsg > 16383) {
            Done = 1;
            printf("Imsg limit exceeded\n");
         }
      }

      /* Acknowledge receipt */
      send(Socket,AckMsg,strlen(AckMsg),0);

      if (RequestTimeRefresh) {
         /* Update AC->Time */
         DOY2MD(Year,doy,&Month,&Day);
         AC->Time = DateToAbsTime(Year,Month,Day,Hour,Minute,Second);
      }

}

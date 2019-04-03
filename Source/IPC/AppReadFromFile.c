#include "Ac.h"

/**********************************************************************/
void ReadFromFile(FILE *StateFile, struct AcType *AC)
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

      long Year,doy,Month,Day,Hour,Minute;
      double Second;
      Done = 0;
      while(!Done) {
         fgets(line,511,StateFile);
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
      }

      if (RequestTimeRefresh) {
         /* Update AC->Time */
         DOY2MD(Year,doy,&Month,&Day);
         AC->Time = DateToAbsTime(Year,Month,Day,Hour,Minute,Second);
      }

}

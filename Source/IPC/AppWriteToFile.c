#include "Ac.h"

/**********************************************************************/
void WriteToFile(FILE *StateFile, struct AcType *AC)
{

      long Isc,Iorb,Iw,i;
      char line[512];

      Isc = AC->ID;

      sprintf(line,"SC[%ld].AC.svb = [ %le %le %le]\n",
         Isc,
         AC->svb[0],
         AC->svb[1],
         AC->svb[2]);
      if (AC->EchoEnabled) printf("%s",line);
      fprintf(StateFile,"%s",line);

      sprintf(line,"SC[%ld].AC.bvb = [ %le %le %le]\n",
         Isc,
         AC->bvb[0],
         AC->bvb[1],
         AC->bvb[2]);
      if (AC->EchoEnabled) printf("%s",line);
      fprintf(StateFile,"%s",line);

      sprintf(line,"SC[%ld].AC.Hvb = [ %le %le %le]\n",
         Isc,
         AC->Hvb[0],
         AC->Hvb[1],
         AC->Hvb[2]);
      if (AC->EchoEnabled) printf("%s",line);
      fprintf(StateFile,"%s",line);

      if (AC->ParmDumpEnabled) {
      }

      sprintf(line,"[EOF]\n\n");
      if (AC->EchoEnabled) printf("%s",line);

      fprintf(StateFile,"%s",line);
}

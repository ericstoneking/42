#include "42.h"

/**********************************************************************/
void WriteToFile(FILE *StateFile)
{

      long Isc,Iorb,Iw,i;
      char line[512];

      sprintf(line,"TIME %ld-%03ld-%02ld:%02ld:%012.9lf\n",
         Year,doy,Hour,Minute,Second);
      fprintf(StateFile,"%s",line);
      if (EchoEnabled) printf("%s",line);

      for(Isc=0;Isc<Nsc;Isc++) {
         if (SC[Isc].Exists) {
            sprintf(line,"SC[%ld].PosR = [ %le %le %le]\n",
               Isc,
               SC[Isc].PosR[0],
               SC[Isc].PosR[1],
               SC[Isc].PosR[2]);
            if (EchoEnabled) printf("%s",line);
            fprintf(StateFile,"%s",line);

            sprintf(line,"SC[%ld].VelR = [ %le %le %le]\n",
               Isc,
               SC[Isc].VelR[0],
               SC[Isc].VelR[1],
               SC[Isc].VelR[2]);
            if (EchoEnabled) printf("%s",line);
            fprintf(StateFile,"%s",line);

            sprintf(line,"SC[%ld].svb = [ %le %le %le]\n",
               Isc,
               SC[Isc].svb[0],
               SC[Isc].svb[1],
               SC[Isc].svb[2]);
            if (EchoEnabled) printf("%s",line);
            fprintf(StateFile,"%s",line);

            sprintf(line,"SC[%ld].bvb = [ %le %le %le]\n",
               Isc,
               SC[Isc].bvb[0],
               SC[Isc].bvb[1],
               SC[Isc].bvb[2]);
            if (EchoEnabled) printf("%s",line);
            fprintf(StateFile,"%s",line);

            sprintf(line,"SC[%ld].Hvb = [ %le %le %le]\n",
               Isc,
               SC[Isc].Hvb[0],
               SC[Isc].Hvb[1],
               SC[Isc].Hvb[2]);
            if (EchoEnabled) printf("%s",line);
            fprintf(StateFile,"%s",line);

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
            fprintf(StateFile,"%s",line);

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
            fprintf(StateFile,"%s",line);

            sprintf(line,"Orb[%ld].VelN = [ %le %le %le]\n",
               Iorb,
               Orb[Iorb].VelN[0],
               Orb[Iorb].VelN[1],
               Orb[Iorb].VelN[2]);
            if (EchoEnabled) printf("%s",line);
            fprintf(StateFile,"%s",line);

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

      fprintf(StateFile,"%s",line);
}

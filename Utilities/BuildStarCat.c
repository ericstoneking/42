/* Cull and parse Skymap for 42 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
      FILE *infile,*outfile;
      float MagLimit,r1,r2,r3,Mv,Mvp;
      int Im,Nstar;
      char inputline[600];
      char junk[600];
      char OutFileName[80];
      char R1a[15],R2a[15],R3a[15],Mva[15],Mvpa[15];

      infile = fopen("/Users/estoneki/Wisdom/SkyMap/ATT_sky2kv5.cat","rt");
      for(Im=0;Im<10;Im++) {
         printf("Processing Mag Limit %i\n",Im);
         MagLimit = (float) Im;
         sprintf(OutFileName,"../Model/Skymap%02i.txt",Im);

         outfile = fopen(OutFileName,"wt");
         fprintf(outfile,"00000000\n");
         Nstar = 0;
         while(!feof(infile)) {
            fgets(inputline,600,infile);
            /* Geocentric Inertial Unit Vector Components (ICRS2000) */
            /* Columns 194-220 */
            sscanf(inputline,"%193c%9c%9c%9c",junk,R1a,R2a,R3a);
            r1 = (float) atof(R1a);
            r2 = (float) atof(R2a);
            r3 = (float) atof(R3a);
            /* V Magnitude, Columns 233-238, V' Magnitude, Columns 239-243 */
            sscanf(inputline,"%232c%6c%5c",junk,Mva,Mvpa);
            Mv = (float) atof(Mva);
            Mvp = (float) atof(Mvpa);
            /* One-dimensional Spectral Class, Columns 337-339 */
            //sscanf(inline[336],"%3c",spec);

            if (Mvp != 0.0) Mv = Mvp;
            if (Mv < MagLimit) {
               Nstar++;
               fprintf(outfile,"%9.6f %9.6f %9.6f  %6.3f\n",r1,r2,r3,Mv);
            }
         }
         rewind(outfile);
         fprintf(outfile,"%8i\n",Nstar);
         fclose(outfile);
         rewind(infile);
      }
      fclose(infile);

      return(0);
}

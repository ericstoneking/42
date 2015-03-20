/* Text file-based interprocess communication */

#include <stdlib.h>
#include <stdio.h>

/**********************************************************************/
int main(int argc, char **argv)
{
      FILE *infile = NULL;
      FILE *outfile = NULL;
      char line[512];
      long i,k;
      double RotAxis[3],RotCenter[3],RotRate;
      double CMx,CMy,CMz,Fx,Fy,Fz,Tx,Ty,Tz;

      outfile = fopen("ClientToServer.fifo","w");
      infile = fopen("ServerToClient.fifo","r");

      for(i=1;i<4;i++) {
         for(k=0;k<3;k++) {
            RotAxis[k] = 0.1*((double) i)*((double) k+1);
            RotCenter[k] = 0.01*((double) i)*((double) k+1);
         }
         RotRate = ((double) i);
      
         fprintf(outfile,"%lf %lf %lf %lf %lf %lf %lf\n",
            RotAxis[0],RotAxis[1],RotAxis[2],
            RotCenter[0],RotCenter[1],RotCenter[2],RotRate);
         fflush(outfile);

         fgets(line,512,infile);
         sscanf(line,"%lf %lf %lf %lf %lf %lf %lf %lf %lf",
            &CMx,&CMy,&CMz,&Fx,&Fy,&Fz,&Tx,&Ty,&Tz);
         printf("%li Client: %lf %lf %lf  %lf %lf %lf  %lf %lf %lf\n",i,
            CMx,CMy,CMz,Fx,Fy,Fz,Tx,Ty,Tz);
      }
      fclose(infile);
      fclose(outfile);

      return(0);
}
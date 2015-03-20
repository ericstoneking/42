/* Text file-based interprocess communication */

#include <stdlib.h>
#include <stdio.h>

/***********************************************/
int main(int argc, char **argv)
{
      FILE *infile = NULL;
      FILE *outfile = NULL;
      char line[512];
      double RotAxis[3],RotCenter[3],RotRate;
      double CMx,CMy,CMz,Fx,Fy,Fz,Tx,Ty,Tz;
      long i,k;
      double x;
      
      system("mkfifo -m a+rw ClientToServer.fifo");
      system("mkfifo -m a+rw ServerToClient.fifo");

      infile = fopen("ClientToServer.fifo","r");
      outfile = fopen("ServerToClient.fifo","w");

      for(i=1;i<4;i++) {
         x = ((double) i);
         fgets(line,512,infile);
         sscanf(line,"%lf %lf %lf %lf %lf %lf %lf",
            &RotAxis[0],&RotAxis[1],&RotAxis[2],
            &RotCenter[0],&RotCenter[1],&RotCenter[2],&RotRate);
         printf("%li Server: %lf %lf %lf %lf %lf %lf %lf\n",i,
            RotAxis[0],RotAxis[1],RotAxis[2],
            RotCenter[0],RotCenter[1],RotCenter[2],RotRate);

            CMx = 0.1*x;
            CMy = 0.2*x;
            CMz = 0.3*x;
            Fx = 0.01*x;
            Fy = 0.02*x;
            Fz = 0.03*x;
            Tx = 1.0*x;
            Ty = 2.0*x;
            Tz = 3.0*x;

         fprintf(outfile,"%lf %lf %lf %lf %lf %lf %lf %lf %lf\n",
            CMx,CMy,CMz,Fx,Fy,Fz,Tx,Ty,Tz);
         fflush(outfile);
      }
      fclose(infile);
      fclose(outfile);
      
      system("rm ClientToServer.fifo");
      system("rm ServerToClient.fifo");

      return(0);
}
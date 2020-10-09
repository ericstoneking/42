#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
      FILE *infile, *outfile;
      double g[14][14],h[14][14];
      //long col = 21; /* Year 2000.0 */
      //long col = 22; /* Year 2005.0 */
      //long col = 23; /* Year 2010.0 */
      long col = 25; /* Year 2020.0 */
      double c[26];
      char gh;
      long n,m;

      infile = fopen("igrf Multiyear.txt","rt");

      while(!feof(infile)) {
         if (fscanf(infile,"%c %ld %ld    %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf",
         &gh,&n,&m,
         &c[ 0],&c[ 1],&c[ 2],&c[ 3],&c[ 4],&c[ 5],&c[ 6],&c[ 7],&c[ 8],&c[ 9],
         &c[10],&c[11],&c[12],&c[13],&c[14],&c[15],&c[16],&c[17],&c[18],&c[19],
         &c[20],&c[21],&c[22],&c[23],&c[24],&c[25]) == 28) {
            printf("%ld %ld\n",n,m);
            if (gh == 'g') {
               g[n][m] = c[col-1];
            }
            else if (gh == 'h') {
               h[n][m] = c[col-1];
            }
         }
      }
      fclose(infile);

      outfile = fopen("igrf20.txt","wt");
      fprintf(outfile,"   igrf20\n");
      fprintf(outfile,"   13  6371.2 2020.0\n");
      for(n=1;n<=13;n++) {
         for(m=0;m<=n;m++) {
            fprintf(outfile,"%5ld %5ld %12.2lf %12.2lf\n",n,m,g[n][m],h[n][m]);
         }
      }
      fclose(outfile);

      return(0);
}

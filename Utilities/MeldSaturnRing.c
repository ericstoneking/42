#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv)
{
      FILE *RedFile, *GrnFile, *BluFile, *AlfFile;
      FILE *RGBAFile;
      int N1,N2,i,r[512],g[512],b[512],a[512];
      
      RedFile = fopen("saturnringr.txt","rt");
      GrnFile = fopen("saturnringg.txt","rt");
      BluFile = fopen("saturnringb.txt","rt");
      AlfFile = fopen("saturnringa.txt","rt");

      RGBAFile = fopen("SaturnRing.ppm","w");

      fscanf(RedFile,"%i %i",&N1,&N2);
      fscanf(GrnFile,"%i %i",&N1,&N2);
      fscanf(BluFile,"%i %i",&N1,&N2);
      fscanf(AlfFile,"%i %i",&N1,&N2);

      fprintf(RGBAFile,"P6\n#.\n512 1\n255\n");

      for(i=0;i<512;i++) {
         fscanf(RedFile,"%i",&r[511-i]);
         fscanf(GrnFile,"%i",&g[511-i]);
         fscanf(BluFile,"%i",&b[511-i]);
         fscanf(AlfFile,"%i",&a[511-i]);
      }
      for(i=0;i<512;i++)
         fprintf(RGBAFile,"%c%c%c%c",r[i],g[i],b[i],255-a[i]);

      fclose(RedFile);
      fclose(GrnFile);
      fclose(BluFile);
      fclose(AlfFile);
      fclose(RGBAFile);

      return(0);
}
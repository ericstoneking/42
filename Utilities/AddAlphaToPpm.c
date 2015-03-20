#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
      FILE *infile,*outfile;
      long i,Nc,Nw,Nh,N;
      long junk;
      char comment[80];
      unsigned char *Tex;
      unsigned char c,R,G,B;

      infile = fopen("./42_Watermark.ppm","rb");
      outfile = fopen("./42_WatermarkAlpha.ppm","wb");

      fscanf(infile,"P6\n%[^\n]\n%li %li\n%li\n",comment,&Nw,&Nh,&junk); /* Header */
      N = Nw*Nh;
      Tex = (unsigned char *) malloc(4*N*sizeof(unsigned char));
      if (Tex == NULL) {
         printf("malloc returned null pointer in AddAlpha.  Bailing out!\n");
         exit(1);
      }
      for(i=0;i<N;i++) {
         fscanf(infile,"%c%c%c",&R,&G,&B);
         //Tex[4*i] = (unsigned char) R;
         //Tex[4*i+1] = (unsigned char) G;
         //Tex[4*i+2] = (unsigned char) B;
         //if (R == 0 && G == 0 && B == 0)
         //   Tex[4*i+3] = (unsigned char) 0;
         //else
         //   Tex[4*i+3] = (unsigned char) 127;
            //Tex[4*i+3] = (unsigned char) 255;
         Tex[4*i  ] = (unsigned char) 255;
         Tex[4*i+1] = (unsigned char) 255;
         Tex[4*i+2] = (unsigned char) 255;
         Tex[4*i+3] = (unsigned char) R;
      }
      fclose(infile);

      fprintf(outfile,"P6\n#.\n%li %li\n255\n",Nw,Nh);
      fflush(outfile);
      N = 4*Nh*Nw;
      for(i=0;i<N;i++) {
         fprintf(outfile,"%c",Tex[i]);
      }
      fclose(outfile);

      return(0);
}

/* Convert PPM to Glyph */
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv)
{
      FILE *infile,*outfile;
      long Nin,Nout,i;
      long Nh,Nw,junk;
      char comment[80];
      unsigned char *Tex;
      unsigned char Glyph;
      long Ctr8;


      infile = fopen("Marker32.ppm","rb");
      fscanf(infile,"P6\n%[^\n]\n%li %li\n%li\n",comment,&Nw,&Nh,&junk); /* Header */
      Nin = Nw*Nh;
      Tex = (unsigned char *) calloc(Nin,sizeof(unsigned char));
      if (Tex == NULL) {
         printf("calloc returned null pointer in PpmToGlyph.  Bailing out!\n");
         exit(1);
      }
      for(i=0;i<Nin;i++) {
         Tex[i] = fgetc(infile);
         fgetc(infile);
         fgetc(infile);
      }
      fclose(infile);
      
      Nout = Nin/8;
      Ctr8 = 0;
      outfile = fopen("Glyph.txt","w");
      fprintf(outfile,"      GLubyte Glyph[%li] = {\n         ",Nout);
      for(i=0;i<Nout;i++) {
         Glyph =  (unsigned char) ((Tex[8*i  ] ? 128 : 0)
                                 + (Tex[8*i+1] ?  64 : 0)
                                 + (Tex[8*i+2] ?  32 : 0)
                                 + (Tex[8*i+3] ?  16 : 0)
                                 + (Tex[8*i+4] ?   8 : 0)
                                 + (Tex[8*i+5] ?   4 : 0)
                                 + (Tex[8*i+6] ?   2 : 0)
                                 + (Tex[8*i+7] ?   1 : 0));
         fprintf(outfile,"0x%02x",Glyph);
         if (i<Nout-1) fprintf(outfile,",");
         if (Ctr8++ > 6) {
            Ctr8 = 0;
            fprintf(outfile,"\n         ");
         }
      }
      fprintf(outfile,"\n      };");
      fclose(outfile);
      
      return(0);
}

/* Convert Mercator texture to Cube */

#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#define RES 1024

unsigned char *Tex;
unsigned char Cube[6*RES*RES*3];
unsigned char CloudGlossCube[6*RES*RES*3];

long Nh,Nw;

/**********************************************************************/
FILE *OpenFile(char *Path, char *File, char *CtrlCode)
{
      FILE *FilePtr;
      char FileName[80];

      strcpy(FileName,Path);
      strcat(FileName,File);
      FilePtr=fopen(FileName,CtrlCode);
      if(FilePtr == NULL) {
         printf("Error opening %s\n",FileName);
         exit(1);
      }
      return(FilePtr);
}
/**********************************************************************/
/*  Normalize a 3-vector.  Return its (pre-normalization) magnitude   */
double UNITV(double V[3])
{
      double A;

      A=sqrt(V[0]*V[0]+V[1]*V[1]+V[2]*V[2]);
      if (A > 0.0) {
         V[0]/=A;
         V[1]/=A;
         V[2]/=A;
      }
      else {
         printf("Attempted divide by zero in UNITV\n");
         V[0] = 0.0;
         V[1] = 0.0;
         V[2] = 0.0;
      }
      return(A);
}
/**********************************************************************/
void PpmToTex(char path[40], char filename[40],long BytesPerPixel)
{
      FILE *infile;
      long N,i;
      long junk;
      char comment[80];

      infile = OpenFile(path,filename,"rb");
      fscanf(infile,"P6\n%[^\n]\n%li %li\n%li\n",comment,&Nw,&Nh,&junk); /* Header */
      N = Nw*Nh*BytesPerPixel;
      Tex = (unsigned char *) malloc(N*sizeof(unsigned char));
      if (Tex == NULL) {
         printf("malloc returned null pointer in PpmToTexTag.  Bailing out!\n");
         exit(1);
      }
      if (BytesPerPixel == 4) {
         for(i=0;i<N;i++) {
            fscanf(infile,"%c",&Tex[i]);
         }
      }
      else {
         for(i=0;i<N;i++) {
            fscanf(infile,"%c",&Tex[i]);
         }
      }
      fclose(infile);

}
/**********************************************************************/
void VecToCube(long N, double p[3], long *f, long *i, long *j)
{
      double MaxComponent = 0.0;
      double MidPoint = 0.5*(N-1.0);
      long k;

      for(k=0;k<3;k++) {
         if (fabs(p[k]) > MaxComponent) {
            MaxComponent = fabs(p[k]);
            *f = k;
            if (p[k] < 0.0) *f += 3;
         }
      }

      if (*f == 0) {
         *i = (long) (MidPoint*(1.0-p[1]/p[0]));
         *j = (long) (MidPoint*(1.0-p[2]/p[0]));
      }
      else if (*f == 1) {
         *i = (long) (MidPoint*(1.0+p[2]/p[1]));
         *j = (long) (MidPoint*(1.0+p[0]/p[1]));
      }
      else if (*f == 2) {
         *i = (long) (MidPoint*(1.0-p[0]/p[2]));
         *j = (long) (MidPoint*(1.0+p[1]/p[2]));
      }
      else if (*f == 3) {
         *i = (long) (MidPoint*(1.0+p[1]/p[0]));
         *j = (long) (MidPoint*(1.0-p[2]/p[0]));
      }
      else if (*f == 4) {
         *i = (long) (MidPoint*(1.0+p[2]/p[1]));
         *j = (long) (MidPoint*(1.0+p[0]/p[1]));
      }
      else {
         *i = (long) (MidPoint*(1.0+p[0]/p[2]));
         *j = (long) (MidPoint*(1.0+p[1]/p[2]));
      }
}
/**********************************************************************/
void CubeToVec(long N, long f, long i, long j, double p[3])
{
      double MidPoint = 0.5*(N-1.0);
      double HalfN = 0.5*N;

      if (f == 0) {
         /* +X */
         p[0] = 1.0;
         p[1] = -(((double) i) - MidPoint)/HalfN;
         p[2] = -(((double) j) - MidPoint)/HalfN;
      }
      else if (f == 1) {
         /* +Y */
         p[1] = 1.0;
         p[2] =  (((double) i) - MidPoint)/HalfN;
         p[0] =  (((double) j) - MidPoint)/HalfN;
      }
      else if (f == 2) {
         /* +Z */
         p[2] = 1.0;
         p[1] = -(((double) i) - MidPoint)/HalfN;
         p[0] =  (((double) j) - MidPoint)/HalfN;
      }
      else if (f == 3) {
         /* -X */
         p[0] = -1.0;
         p[1] = -(((double) i) - MidPoint)/HalfN;
         p[2] =  (((double) j) - MidPoint)/HalfN;
      }
      else if (f == 4) {
         /* -Y */
         p[1] = -1.0;
         p[2] = -(((double) i) - MidPoint)/HalfN;
         p[0] =  (((double) j) - MidPoint)/HalfN;
      }
      else {
         /* -Z */
         p[2] = -1.0;
         p[1] = -(((double) i) - MidPoint)/HalfN;
         p[0] = -(((double) j) - MidPoint)/HalfN;
      }
      UNITV(p);
}
/**********************************************************************/
void CubeToPpm(unsigned char Cube[6*RES*RES*3], long N,
   char pathname[40],char filename[40])
{
      FILE *outfile;
      char outfilename[40];
      unsigned char R,G,B;
      long face,i,j;
      long idx = 0;
      char Face[6][20] = {"PX.ppm","PY.ppm","PZ.ppm",
                          "MX.ppm","MY.ppm","MZ.ppm"};

      for(face=0;face<6;face++) {
         strcpy(outfilename,filename);
         strcat(outfilename,Face[face]);
         outfile = OpenFile(pathname,outfilename,"w");
         fprintf(outfile,"P6\n#.\n%li %li\n255\n",N,N);
         fflush(outfile);
         for(i=0;i<N;i++) {
            for(j=0;j<N;j++) {
               R = Cube[idx++];
               G = Cube[idx++];
               B = Cube[idx++];
               fprintf(outfile,"%c%c%c",R,G,B);
            }
         }
         fclose(outfile);
      }
}
/**********************************************************************/
int main(int argc, char **argv)
{

      long i,j;
      double lat;
      double lng;
      double p[3];
      long F,I,J,k;
      double Pi,TwoPi,HalfPi;
      char WorldFileName[80],MercFileName[80],CloudGlossFileName[80];
      double Red,Grn,Blu;
      int Gloss;

      Pi = 4.0*atan(1.0);
      TwoPi = 2.0*Pi;
      HalfPi = 0.5*Pi;

      if (argc != 2) {
         printf("Need World File Name (w/o extension) as argument\n");
         exit(1);
      }
      strcpy(WorldFileName,argv[1]);

      sprintf(MercFileName,"%s.ppm",WorldFileName);
      PpmToTex("./",MercFileName,3);

      for(F=0;F<6;F++) {
         for(I=0;I<RES;I++) {
            for(J=0;J<RES;J++) {
               CubeToVec(RES,F,I,J,p);
               lat = asin(p[2]);
               lng = atan2(p[1],p[0]);
               i = (long) (2047.0*(-lat+HalfPi)/Pi+0.5);
               j = (long) (4095.0*(lng+Pi)/TwoPi+0.5);
               for(k=0;k<3;k++) {
                  Cube[RES*RES*3*F+RES*3*I+3*J+k] = Tex[4096*3*i+3*j+k];
               }
               /* Gloss */
               Red = (double) Tex[4096*3*i+3*j+0];
               Grn = (double) Tex[4096*3*i+3*j+1];
               Blu = (double) Tex[4096*3*i+3*j+2];
               if (Blu > Red+Grn) {
                  Gloss = (char) (255.0*Blu/(Red+Grn+Blu));
               }
               else Gloss = 0;
               CloudGlossCube[RES*RES*3*F+RES*3*I+3*J] = 0;
               CloudGlossCube[RES*RES*3*F+RES*3*I+3*J+1] = Gloss;
               CloudGlossCube[RES*RES*3*F+RES*3*I+3*J+2] = 0;
               //i = (long) (1799.0*(-lat+HalfPi)/Pi+0.5);
               //j = (long) (3599.0*(lng+Pi)/TwoPi+0.5);
               //for(k=0;k<3;k++) Cube[1024*1024*3*F+1024*3*I+3*J+k] = Tex[3600*3*i+3*j+k];
            }
         }
      }

      CubeToPpm(Cube,RES,"./",WorldFileName);

      sprintf(CloudGlossFileName,"%sCloudGloss",WorldFileName);
      CubeToPpm(CloudGlossCube,RES,"./",CloudGlossFileName);

      return(0);
}


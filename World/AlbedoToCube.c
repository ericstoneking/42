/* Convert Mercator texture to Cube */
/* Customized for TOMS_albedo.ppm */

#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#define RES 128

unsigned char *Tex;
unsigned char Cube[6*RES*RES];

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
void CubeToPpm(unsigned char Cube[6*RES*RES], long N,
   char pathname[40],char filename[40])
{
      FILE *outfile;
      char outfilename[40];
      long face,i,j,k;
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
               for(k=0;k<3;k++) fprintf(outfile,"%c",Cube[idx]);
               idx++;
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
      char WorldFileName[80],MercFileName[80];

      Pi = 4.0*atan(1.0);
      TwoPi = 2.0*Pi;
      HalfPi = 0.5*Pi;

      sprintf(WorldFileName,"TOMS_Albedo");

      sprintf(MercFileName,"%s.ppm",WorldFileName);
      PpmToTex("./",MercFileName,3);

      for(F=0;F<6;F++) {
         for(I=0;I<RES;I++) {
            for(J=0;J<RES;J++) {
               CubeToVec(RES,F,I,J,p);
               lat = asin(p[2]);
               lng = atan2(p[1],p[0]);
               i = (long) (179.0*(-lat+HalfPi)/Pi+0.5);
               j = (long) (287.0*(lng+Pi)/TwoPi+0.5);
               Cube[RES*RES*F+RES*I+J] = Tex[288*3*i+3*j];
            }
         }
      }

      CubeToPpm(Cube,RES,"./",WorldFileName);

      return(0);
}


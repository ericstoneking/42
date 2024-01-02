/* Convert Earth DEM to Bump Cube */

#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#define RES 512

#define IDX2(x,y,Ny)             ((x)*(Ny)+(y))
#define IDX3(x,y,z,Ny,Nz)       (((x)*(Ny)+(y))*(Nz)+(z))
#define IDX4(x,y,z,w,Ny,Nz,Nw) ((((x)*(Ny)+(y))*(Nz)+(z))*(Nw)+(w))

unsigned char HeightCube[6*RES*RES*3];
unsigned char BumpCube[6*RES*RES*3];

double Pi,TwoPi,HalfPi;

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
/*  Magnitude of a 3-vector                                           */
double MAGV(double V[3])
{
      return(sqrt(V[0]*V[0]+V[1]*V[1]+V[2]*V[2]));
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
/*  Vector Cross Product                                              */
void  VxV(double A[3], double B[3], double C[3])
{
      C[0]=A[1]*B[2]-A[2]*B[1];
      C[1]=A[2]*B[0]-A[0]*B[2];
      C[2]=A[0]*B[1]-A[1]*B[0];
}
/**********************************************************************/
/*  Transpose of 3x3 Matrix times 3x1 Vector                          */
void MTxV (double M[3][3], double V[3], double W[3])
{
      W[0] = M[0][0]*V[0] + M[1][0]*V[1] + M[2][0]*V[2];
      W[1] = M[0][1]*V[0] + M[1][1]*V[1] + M[2][1]*V[2];
      W[2] = M[0][2]*V[0] + M[1][2]*V[1] + M[2][2]*V[2];
}
/**********************************************************************/
unsigned char *PpmToMap(char path[40], char filename[40],long BytesPerPixel)
{
      FILE *infile;
      long N,i;
      long junk;
      char comment[80];
      unsigned char *Map;

      infile = OpenFile(path,filename,"rb");
      fscanf(infile,"P6\n%[^\n]\n%li %li\n%li\n",comment,&Nw,&Nh,&junk); /* Header */
      N = Nw*Nh*BytesPerPixel;
      Map = (unsigned char *) calloc(N,sizeof(unsigned char));
      if (Map == NULL) {
         printf("calloc returned null pointer in PpmToTex.  Bailing out!\n");
         exit(1);
      }
      for(i=0;i<N;i++) {
         Map[i] = (unsigned char) fgetc(infile);
      }
      fclose(infile);

      return(Map);
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
void FijToNeighbors(long f, long i, long j, long N,
                    long *fim, long *iim, long *jim,
                    long *fip, long *iip, long *jip,
                    long *fjm, long *ijm, long *jjm,
                    long *fjp, long *ijp, long *jjp)
{
      long N1 = N-1;

      /* Interior Points are simple */
      *fim = f;  *iim = i-1;  *jim = j;
      *fip = f;  *iip = i+1;  *jip = j;
      *fjm = f;  *ijm = i  ;  *jjm = j-1;
      *fjp = f;  *ijp = i  ;  *jjp = j+1;

      /* Edges and Corners are Special Cases */
      if (i == 0) {
         if (f == 0) {
            *fim = 1;
            *iim = N1-j;
            *jim = N1;
         }
         else if (f == 1) {
            *fim = 5;
            *iim = 0;
            *jim = N1-j;
         }
         else if (f == 2) {
            *fim = 5;
            *iim = i;
            *jim = N1;
         }
         else if (f == 3) {
            *fim = 1;
            *iim = j;
            *jim = 0;
         }
         else if (f == 4) {
            *fim = 2;
            *iim = N1;
            *jim = j;
         }
         else {
            *fim = 1;
            *iim = 0;
            *jim = N1-j;
         }
      }
      else if (i == N1) {
         if (f == 0) {
            *fip = 4;
            *iip = j;
            *jip = N1;
         }
         else if (f == 1) {
            *fip = 2;
            *iip = 0;
            *jip = j;
         }
         else if (f == 2) {
            *fip = 4;
            *iip = 0;
            *jip = j;
         }
         else if (f == 3) {
            *fip = 4;
            *iip = N1-j;
            *jip = 0;
         }
         else if (f == 4) {
            *fip = 5;
            *iip = N1;
            *jip = N1-j;
         }
         else {
            *fip = 4;
            *iip = N1;
            *jip = N1-j;
         }
      }

      if (j == 0) {
         if (f == 0) {
            *fjm = 2;
            *ijm = i;
            *jjm = N1;
         }
         else if (f == 1) {
            *fjm = 3;
            *ijm = 0;
            *jjm = i;
         }
         else if (f == 2) {
            *fjm = 3;
            *ijm = i;
            *jjm = N1;
         }
         else if (f == 3) {
            *fjm = 5;
            *ijm = i;
            *jjm = N1;
         }
         else if (f == 4) {
            *fjm = 3;
            *ijm = N1;
            *jjm = N1-i;
         }
         else {
            *fjm = 0;
            *ijm = i;
            *jjm = N1;
         }
      }
      else if (j == N1) {
         if (f == 0) {
            *fjp = 5;
            *ijp = i;
            *jjp = 0;
         }
         else if (f == 1) {
            *fjp = 0;
            *ijp = 0;
            *jjp = N1-i;
         }
         else if (f == 2) {
            *fjp = 0;
            *ijp = i;
            *jjp = 0;
         }
         else if (f == 3) {
            *fjp = 2;
            *ijp = i;
            *jjp = 0;
         }
         else if (f == 4) {
            *fjp = 0;
            *ijp = N1;
            *jjp = i;
         }
         else {
            *fjp = 3;
            *ijp = i;
            *jjp = 0;
         }
      }
}
/**********************************************************************/
void CubeToPpm(unsigned char *Cube, long N,
   char pathname[40],char filename[40])
{
      FILE *outfile;
      char outfilename[40];
      unsigned char R,G,B;
      long face,i,j;
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
               R = Cube[IDX4(face,i,j,0,RES,RES,3)];
               G = Cube[IDX4(face,i,j,1,RES,RES,3)];
               B = Cube[IDX4(face,i,j,2,RES,RES,3)];
               fprintf(outfile,"%c%c%c",R,G,B);
            }
         }
         fclose(outfile);
      }
}
/**********************************************************************/
void MapToCube1(unsigned char *Map,long Ny,long Nx,unsigned char *Cube, long Nc)
{
      long f,i,j,I,J;
      double p[3];

      for(f=0;f<6;f++) {
         for(i=0;i<Nc;i++) {
            for(j=0;j<Nc;j++) {
               CubeToVec(Nc,f,i,j,p);
               I = (long) (((double) (Ny-1))*(HalfPi-asin(p[2]))/Pi);
               J = (long) (((double) (Nx-1))*(atan2(p[1],p[0])+Pi)/TwoPi);
               if (I < 0) I += Ny;
               if (I > Ny-1) I -= Ny;
               if (J < 0) J += Nx;
               if (J > Nx-1) J -= Nx;
               Cube[IDX3(f,i,j,Nc,Nc)] = Map[IDX3(I,J,0,Nx,3)];
            }
         }
      }
}
/**********************************************************************/
void PatchToNormal(double p[3], double pim[3], double pip[3],
                   double pjm[3], double pjp[3], double dHi, double dHj,
                   double N[3])
{
      double dpi[3],dpj[3],di,dj;
      double CIJN[3][3];
      double dHdi,dHdj,r,z;
      double Nij[3];
      long k;

      for(k=0;k<3;k++) {
         CIJN[2][k] = p[k];
         dpi[k] = pip[k]-pim[k];
         dpj[k] = pjp[k]-pjm[k];
      }
      di = MAGV(dpi);
      dj = MAGV(dpj);
      VxV(p,dpi,CIJN[1]);
      UNITV(CIJN[1]);
      VxV(CIJN[1],p,CIJN[0]);
      UNITV(CIJN[0]);
      dHdi = dHi/di;
      dHdj = dHj/dj;
      r=sqrt(dHdi*dHdi+dHdj*dHdj);
      if (r > 0.5) {
         dHdi *= 0.5/r;
         dHdj *= 0.5/r;
         r = 0.5;
      }
      z = sqrt(1.0-r*r);
      Nij[0] = -dHdi;
      Nij[1] = -dHdj;
      Nij[2] = z;
      MTxV(CIJN,Nij,N);
      UNITV(N);
}
/**********************************************************************/
int main(int argc, char **argv)
{

      long i,j;
      double lat;
      double lng;
      double p[3];
      long F,I,J,k;
      long fim,fip,fjm,fjp;
      long iim,iip,jim,jip,ijm,ijp,jjm,jjp;
      double pjm[3],pjp[3],pim[3],pip[3];
      double dHi,dHj,dN[3],N[3];
      char WorldFileName[80],MercFileName[80];
      double Red,Grn,Blu;
      int Gloss;
      unsigned char *Map;
      //double Scale = 0.005/255.0; /* Works for Earth */
      double Scale = 0.01/255.0; /* Mars */

      Pi = 4.0*atan(1.0);
      TwoPi = 2.0*Pi;
      HalfPi = 0.5*Pi;

      Map = PpmToMap("./","MarsDEM4096.ppm",3);

      MapToCube1(Map,Nh,Nw,HeightCube,RES);

      for(F=0;F<6;F++) {
         for(I=0;I<RES;I++) {
            for(J=0;J<RES;J++) {
               FijToNeighbors(F,I,J,RES,&fim,&iim,&jim,&fip,&iip,&jip,
                     &fjm,&ijm,&jjm,&fjp,&ijp,&jjp);
               CubeToVec(RES,F,I,J,p);
               CubeToVec(RES,fjm,ijm,jjm,pjm);
               CubeToVec(RES,fjp,ijp,jjp,pjp);
               CubeToVec(RES,fim,iim,jim,pim);
               CubeToVec(RES,fip,iip,jip,pip);
               dHi = Scale*((double) (HeightCube[IDX3(fip,iip,jip,RES,RES)]-HeightCube[IDX3(fim,iim,jim,RES,RES)]));
               dHj = Scale*((double) (HeightCube[IDX3(fjp,ijp,jjp,RES,RES)]-HeightCube[IDX3(fjm,ijm,jjm,RES,RES)]));
               PatchToNormal(p,pim,pip,pjm,pjp,dHi,dHj,N);
               UNITV(p);

               for(k=0;k<3;k++) {
                  dN[k] = N[k]-p[k];
                  if (dN[k] < -1.0) dN[k] = -1.0;
                  if (dN[k] > 1.0) dN[k] = 1.0;
                  BumpCube[IDX4(F,I,J,k,RES,RES,3)] = (unsigned char) (255.0*0.5*(dN[k]+1.0));
               }

            }
         }
      }

      CubeToPpm(BumpCube,RES,"./","MarsBump");

      return(0);
}


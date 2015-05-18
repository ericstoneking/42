/*    This file is distributed with 42,                               */
/*    the (mostly harmless) spacecraft dynamics simulation            */
/*    created by Eric Stoneking of NASA Goddard Space Flight Center   */

/*    Copyright 2010 United States Government                         */
/*    as represented by the Administrator                             */
/*    of the National Aeronautics and Space Administration.           */

/*    No copyright is claimed in the United States                    */
/*    under Title 17, U.S. Code.                                      */

/*    All Other Rights Reserved.                                      */

#include "sphkit.h"

#ifdef __cplusplus
   extern "C" {
#endif

#define Pi (3.141592654)

/**********************************************************************/
GLuint KernelToTexTag(void)
{
      long N,i,j;
      long Nh,Nw;
      GLuint TexTag;
      GLubyte *Tex;
      int BytesPerPixel = 1;
      GLuint wrap = GL_CLAMP;
      float x,y;
      float r,W;

      Nw = 64;
      Nh = 64;
      N = Nw*Nh*BytesPerPixel;
      Tex = (GLubyte *) calloc(N,sizeof(GLubyte));
      if (Tex == NULL) {
         printf("calloc returned null pointer in KernelToTexTag.  Bailing out!\n");
         exit(1);
      }
      for(i=0;i<64;i++) {
         x = ((float) i)/16.0 - 2.0 + 0.5/64.0;
         for(j=0;j<64;j++) {
            y = ((float) j)/16.0 - 2.0 + 0.5/64.0;
            r = sqrt(x*x+y*y);
            W = CubicKernel(r,1.0,2);
            //if (fabs(r-1.0) < 1.0E-2) W = 1.0;
            //if (fabs(r-2.0) < 1.0E-2) W = 0.5;
            Tex[64*j+i] = (GLubyte) (255.0*W);
         }
      }
      glGenTextures(1,&TexTag);
      glBindTexture(GL_TEXTURE_2D,TexTag);
      glTexParameteri(GL_TEXTURE_2D,GL_GENERATE_MIPMAP,GL_TRUE);
      glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,wrap);
      glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,wrap);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexImage2D(GL_TEXTURE_2D,0,GL_LUMINANCE,Nw,Nh,0,GL_LUMINANCE,
            GL_UNSIGNED_BYTE,Tex);

      free(Tex);
      return(TexTag);
}
/**********************************************************************/
/*  This function cribbed from an OpenCL example                      */
/*  on the Apple developer site                                       */
int FileToString(const char *file_name, char **result_string,
                 size_t *string_len)
{
      int fd;
      size_t file_len;
      struct stat file_status;
      int ret;

      *string_len = 0;
      fd = open(file_name, O_RDONLY);
      if (fd == -1) {
          printf("Error opening file %s\n", file_name);
          return -1;
      }
      ret = fstat(fd, &file_status);
      if (ret) {
          printf("Error reading status for file %s\n", file_name);
          return -1;
      }
      file_len = file_status.st_size;

      *result_string = (char *) calloc(file_len + 1, sizeof(char));
      ret = read(fd, *result_string, file_len);
      if (!ret) {
          printf("Error reading from file %s\n", file_name);
          return -1;
      }

      close(fd);

      *string_len = file_len;
      return 0;
}
/**********************************************************************/
/* T = [0:1]                                                          */
void Heat2RGB(float T, float *R, float *G, float *B)
{
      if (T < 0.0) T = 0.0;
      if (T > 1.0) T = 1.0;

      if (T < 0.5) {
         *R = T/0.5;
         *B = 0.0;
      }
      else {
         *R = 1.0;
         *B = (T-0.5)/0.5;
      }

      if (T < 0.25) *G = 0.0;
      else if (T < 0.75) *G = (T-0.25)/0.5;
      else *G = 1.0;
}
/**********************************************************************/
/* H = [0:1] , S = [0:1]                                              */
void HeatMap(float H, float S, float *R, float *G, float *B)
{
      if (H < 0.0) H = 0.0;
      if (H > 1.0) H = 1.0;
      if (S < 0.0) S = 0.0;
      if (S > 1.0) S = 1.0;

      if (H < 0.5) {
         *R = H/0.5;
         *B = 0.0;
      }
      else {
         *R = 1.0;
         *B = (H-0.5)/0.5;
      }

      if (H < 0.25) *G = 0.0;
      else if (H < 0.75) *G = (H-0.25)/0.5;
      else *G = 1.0;

      *R *= S;
      *G *= S;
      *B *= S;
}
/**********************************************************************/
/* H = [0:360], S = [0:1], V = [0:1]                                  */
/* R = [0:1], G = [0:1], B = [0:1]                                    */
void HSV2RGB(float H, float S, float V,
   float *R, float *G, float *B)
{
      double C = V*S;
      double X;

      if (H < 0.0) H = 0.0;
      if (H > 360.0) H = 360.0;
      H = H/60.0;

      if (S < 0.0) S = 0.0;
      if (S > 1.0) S = 1.0;
      if (V < 0.0) V = 0.0;
      if (V > 1.0) V = 1.0;

      X = C*(1.0-fabs(fmod(H,2.0)-1.0));

      if (H < 1.0) {
         *R = C;
         *G = X;
         *B = 0.0;
      }
      else if (H < 2.0) {
         *R = X;
         *G = C;
         *B = 0.0;
      }
      else if (H < 3.0) {
         *R = 0.0;
         *G = C;
         *B = X;
      }
      else if (H < 4.0) {
         *R = 0.0;
         *G = X;
         *B = C;
      }
      else if (H < 5.0) {
         *R = X;
         *G = 0.0;
         *B = C;
      }
      else {
         *R = C;
         *G = 0.0;
         *B = X;
      }
}
/**********************************************************************/
double CubicKernel(double r, double h, long Ndim)
{
      double K;
      double q = fabs(r/h);
      double q1,q2;

      switch(Ndim) {
         case 1:
            K = 1.0/(6.0*h);
         break;
         case 2:
            K = 5.0/(14.0*Pi*h*h);
         break;
         case 3:
            K = 1.0/(4.0*Pi*h*h*h);
         break;
         default:
            printf("Bad Ndim = %ld in CubicGradKernel\n",Ndim);
            exit(1);
      }

      if (q < 1.0) {
         q2 = 2.0-q;
         q1 = 1.0-q;
         return(K*(q2*q2*q2-4.0*q1*q1*q1));
      }
      else if (q < 2.0) {
         q2 = 2.0-q;
         return(K*q2*q2*q2);
      }
      else return(0.0);
}
/**********************************************************************/
double CubicGradKernel(double r, double h, long Ndim)
{
      double K;
      double q = fabs(r/h);
      double q1,q2;

      switch(Ndim) {
         case 1:
            K = -3.0/(6.0*h);
         break;
         case 2:
            K = -15.0/(14.0*Pi*h*h);
         break;
         case 3:
            K = -3.0/(4.0*Pi*h*h*h);
         break;
         default:
            printf("Bad Ndim = %ld in CubicGradKernel\n",Ndim);
            exit(1);
      }

      if (q < 1.0) {
         q2 = 2.0-q;
         q1 = 1.0-q;
         return(K*(q2*q2-4.0*q1*q1));
      }
      else if (q < 2.0) {
         q2 = 2.0-q;
         return(K*q2*q2);
      }
      else return(0.0);
}
/**********************************************************************/
double Poly6Kernel(double r, double h, long Ndim)
{
      double K;
      double q = fabs(r/h);
      double d;

      switch(Ndim) {
         case 1:
            K = 35.0/(4096.0*h);
         break;
         case 2:
            K = 1.0/(64.0*Pi*h*h);
         break;
         case 3:
            K = 315.0/(32768.0*h*h*h);
         break;
         default:
            printf("Bad Ndim = %ld in Poly6Kernel\n",Ndim);
            exit(1);
      }

      if (q < 2.0) {
         d = 4.0-q*q;
         return(K*d*d*d);
      }
      else return(0.0);
}
/**********************************************************************/
double Poly6GradKernel(double r, double h, long Ndim)
{
      double K;
      double q = fabs(r/h);
      double d;

      switch(Ndim) {
         case 1:
            K = -6.0*35.0/(4096.0*h);
         break;
         case 2:
            K = -6.0/(64.0*Pi*h*h);
         break;
         case 3:
            K = -6.0*315.0/(32768.0*h*h*h);
         break;
         default:
            printf("Bad Ndim = %ld in Poly6GradKernel\n",Ndim);
            exit(1);
      }

      if (q < 2.0) {
         d = 4.0-q*q;
         return(K*q*d*d);
      }
      else return(0.0);
}
/**********************************************************************/
double SpikyKernel(double r, double h, long Ndim)
{
      double K;
      double q = fabs(r/h);
      double d = 2.0-q;

      switch(Ndim) {
         case 1:
            K = 1.0/(8.0*h);
         break;
         case 2:
            K = 5.0/(16.0*Pi*h*h);
         break;
         case 3:
            K = 15.0/(64.0*Pi*h*h*h);
         break;
         default:
            printf("Bad Ndim = %ld in SpikyKernel\n",Ndim);
            exit(1);
      }

      if (q < 2.0) return(K*d*d*d);
      else return(0.0);
}
/**********************************************************************/
double SpikyGradKernel(double r, double h, long Ndim)
{
      double K;
      double q = fabs(r/h);
      double d = 2.0-q;

      switch(Ndim) {
         case 1:
            K = -3.0/(8.0*h);
         break;
         case 2:
            K = -15.0/(16.0*Pi*h*h);
         break;
         case 3:
            K = -45.0/(64.0*Pi*h*h*h);
         break;
         default:
            printf("Bad Ndim = %ld in SpikyGradKernel\n",Ndim);
            exit(1);
      }

      if (q < 2.0) return(K*d*d);
      else return(0.0);
}
/**********************************************************************/
double Unwrap(double dx, double Span)
{
      if (dx < -0.5*Span) return(dx + Span);
      else if (dx > 0.5*Span) return(dx - Span);
      else return(dx);
}
/**********************************************************************/
#ifdef __cplusplus
   } /* extern "C" */
#endif

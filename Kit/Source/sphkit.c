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
#if 0
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
            /*if (fabs(r-1.0) < 1.0E-2) W = 1.0; */
            /*if (fabs(r-2.0) < 1.0E-2) W = 0.5; */
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
#endif
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

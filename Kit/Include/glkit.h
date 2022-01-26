/*    This file is distributed with 42,                               */
/*    the (mostly harmless) spacecraft dynamics simulation            */
/*    created by Eric Stoneking of NASA Goddard Space Flight Center   */

/*    Copyright 2010 United States Government                         */
/*    as represented by the Administrator                             */
/*    of the National Aeronautics and Space Administration.           */

/*    No copyright is claimed in the United States                    */
/*    under Title 17, U.S. Code.                                      */

/*    All Other Rights Reserved.                                      */


#ifndef __GLKIT_H__
#define __GLKIT_H__

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#ifdef __linux__
   #define GL_GLEXT_PROTOTYPES
   #include <GL/glut.h>
   #include <GL/glu.h>
   #ifdef _USE_GLFW_
     #include <GLFW/glfw3.h>
   #endif
#elif defined __MINGW32__
   #define GLEW_BUILD
   #include "glew.h"
   #define GL_GLEXT_PROTOTYPES
   #include <glut.h>
   #include <GL/glu.h>
#elif defined _WIN32
   #define GLEW_BUILD
   #include "glew.h"
   #define GL_GLEXT_PROTOTYPES
   #include <glut.h>
   #include <GL/glu.h>
#elif defined _WIN64
   #define GLEW_BUILD
   #include "glew.h"
   #define GL_GLEXT_PROTOTYPES
   #include <glut.h>
   #include <GL/glu.h>
#elif defined __APPLE__
   #include <OpenGL/gl.h>
   #include <OpenGL/glu.h>
   #include <GLUT/glut.h>
   #ifdef _USE_GLFW_
     #include <GLFW/glfw3.h>
   #endif
   /* #include <CGDirectDisplay.h> */ /* For CGDisplayHideCursor */
#else
   #error "Yo!  I don't know where to look for glut.h!"
#endif

#include "dcmkit.h"
#include "mathkit.h"
#include "iokit.h"
#include "sigkit.h"

/*
** #ifdef __cplusplus
** namespace Kit {
** #endif
*/

#define FRONT 1
#define BACK 0

#define CUBE 0
#define MERCATOR 1

#define GLYPH_N 0
#define GLYPH_S 1
#define GLYPH_B 2
#define GLYPH_H 3
#define GLYPH_O 4
#define GLYPH_F 5
#define GLYPH_L 6

struct FacetType {
   double V[55][3];
   double ST[55][2];
};

struct HexType {
   double Pc[3];
   double Pv[6][3];
   struct FacetType F[6];
};

struct PentType {
   double Pc[3];
   double Pv[5][3];
   struct FacetType F[5];
};

EXTERN GLuint Font8x11Offset;
EXTERN struct HexType Hex[20];
EXTERN struct PentType Pent[12];
EXTERN double CosVis[4], SinVis[4];
EXTERN GLuint SunVtxShader,SunFragShader,SunShaderProgram;
EXTERN GLuint WorldVtxShader,WorldFragShader,WorldShaderProgram;
EXTERN GLuint RingVtxShader,RingFragShader,RingShaderProgram;
EXTERN GLuint AtmoVtxShader,AtmoFragShader,AtmoShaderProgram;
EXTERN GLuint MapVtxShader,MapFragShader,MapShaderProgram;
EXTERN GLuint BodyVtxShader,BodyFragShader,BodyShaderProgram;
EXTERN GLuint MoonMapFragShader,MoonMapShaderProgram;
EXTERN GLuint AlbedoVtxShader,AlbedoFragShader,AlbedoShaderProgram;
EXTERN GLuint TexReductionVtxShader,TexReductionFragShader,TexReductionShaderProgram;

void DrawBitmapString(void *font, const char *string);
void DrawStrokeString(void *font, const char *string);
GLuint LoadFont8x11(void);
void DrawString8x11(const char *s);
long ClampColor4fv(GLfloat *Color);
void FindSunColor(double T, GLfloat LightColor[3], GLfloat DiskColor[3]);
void DrawSkyGrid(GLfloat MajColor[4], GLfloat MinColor[4], double C[3][3],
                 GLuint MajList, GLuint MinList);
void LoadSkyGrid(double MajGrid, double MinGrid, double SkyDistance,
                 GLuint *MajList, GLuint *MinList);
void DrawNearFOV(long Nv,double Width,double Height,double Length,
   long BoreAxis, long H_Axis, long V_Axis, long Type,GLfloat Color[4]);
void DrawFarFOV(long Nv,double Width,double Height, 
   long BoreAxis, long H_Axis, long V_Axis, long Type, GLfloat Color[4],
                const char Label[40], double SkyDistance);
void RotateL2R(double C[3][3]);
void RotateR2L(double C[3][3]);
void MxM4f(float A[16], float B[16], float C[16]);
void Minv4f(float A[16], float Ai[16]);
void BuildModelMatrix(double CBN[3][3], double pbn[3],
   float ModelMatrix[16]);
void BuildViewMatrix(double CEN[3][3], double pen[3],
   float ViewMatrix[16]);
void CaptureScreenToPpm(const char path[40], const char filename[40],
                        long Nh, long Nw);
void TexToPpm(const char path[40], const char filename[40],
              long Nh, long Nw, long Nb, float *Data);
GLuint PpmToTexTag(const char path[40], const char filename[40],int BytesPerPixel,
                   GLuint wrap);
GLuint Ppm1DToTexTag(const char path[40], const char filename[40],int BytesPerPixel,
                   GLuint wrap);
GLuint PpmToCubeTag(const char path[40], const char file[40], int BytesPerPixel);
GLuint PpmToRingTexTag(const char path[80], const char filename[80]);
void CubeToPpm(GLubyte *Cube, long N, const char pathname[40],const char filename[40]);
void LoadBucky(double BuckyPf[32][3], long BuckyNeighbor[32][6]);
void LoadStars(const char StarFileName[40],double BuckyPf[32][3],
               long BuckyNeighbor[32][6],GLuint StarList[32],
               double SkyDistance);
void DrawStars(double LineOfSight[3],double BuckyPf[32][3],
               GLuint StarList[32]);
void Draw1FGL(double LineOfSight[3],double BuckyPf[32][3],
               GLuint FermiSourceList[32]);
void DrawEgret(double LineOfSight[3],double BuckyPf[32][3],
               GLuint EgretSourceList[32]);
void DrawPulsars(double LineOfSight[3],double BuckyPf[32][3],
               GLuint PulsarList[32]);
GLuint LoadMilkyWay(const char *PathName,const char *FileName, double CGH[3][3],
   double SkyDistance, double AlphaMask[4]);
GLuint LoadSkyCube(const char *PathName,const char *FileName, double CGH[3][3],
   double SkyDistance);
void LoadEgretCatalog(const char *EgretFileName,double BuckyPf[32][3],
               long BuckyNeighbor[32][6],GLuint EgretSourceList[32],
               double SkyDistance);
void Load1FGL(const char *FileName,double BuckyPf[32][3],
               long BuckyNeighbor[32][6],GLuint FermiSourceList[32],
               double SkyDistance);
void LoadPulsars(const char *FileName,double BuckyPf[32][3],
               long BuckyNeighbor[32][6],GLuint PulsarList[32],
               double SkyDistance);
void DrawUnitCubeSphere(long Ndiv);
void DrawSkySphere(long Ndiv);
void DrawUnitMercatorSphere(GLuint Nlat, GLuint Nlng);
void DrawBullseye(GLfloat Color[4],double p[4]);
void DrawArrowhead(double v[3],double scale);
void DrawVector(double v[3], const char Label[8], const char Units[8],
                GLfloat Color[4],
                double VisScale, double MagScale, long UnitVec);
void DrawAxisLabels(long Iglyph,GLfloat Color[4],
                    GLfloat Xc,GLfloat Xmax,GLfloat Yc,GLfloat Ymax,
                    GLfloat Zc,GLfloat Zmax);
void DrawBodyLabel(long Ib,GLfloat Color[4],double p[3]);
void DrawRollPitchYaw(long xc, long yc, long PixScale,
   double AngScale, double RateScale,
   double Roll, double Pitch, double Yaw,
   double RollRate, double PitchRate, double YawRate,
   double RollCmd, double PitchCmd, double YawCmd,
   GLfloat GaugeColor[4], GLfloat BarColor[4]);
void DrawSmallCircle(double lngc, double latc, double rad);
void DrawMercatorGrid(double CVA[3][3]);
void DrawMercatorLine(double lngA, double latA, double lngB, double latB);
void DrawMercatorSquare(double CCV[3][3], double FOV[2]);
void DrawMercatorVector(double lng, double lat, char *label);
void DrawMercatorAxes(double CAV[3][3], char *label);
void CheckOpenGLProperties(void);
void HammerProjection(double Lng, double Lat, double *x, double *y);
void VecToCube(long N, double p[3], long *f, long *i, long *j);
void CubeToVec(long N, long f, long i, long j, double p[3]);
double ProcTex2D(double x, double y, double Xunit, double Yunit, long Noct);
double ProcTex3D(double x, double y, double z,
                double Xunit, double Yunit, double Zunit,
                long Noct, double Persist);
double SphereTex(double lng, double lat,
   double Xunit, double Yunit, double Zunit, long Noct, double Persist);

#ifdef _USE_SHADERS_
GLuint TextToShader(GLchar *Text,GLuint Type,const char *Name);
GLuint BuildShaderProgram(GLuint VtxShader, GLuint FragShader, const char *Name);
void ValidateShaderProgram(GLuint ShaderProgram, const char *Name);
#endif

/*
** #ifdef __cplusplus
** }
** #endif
*/

#endif /* __GLKIT_H__ */

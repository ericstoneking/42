/*    This file is distributed with 42,                               */
/*    the (mostly harmless) spacecraft dynamics simulation            */
/*    created by Eric Stoneking of NASA Goddard Space Flight Center   */

/*    Copyright 2010 United States Government                         */
/*    as represented by the Administrator                             */
/*    of the National Aeronautics and Space Administration.           */

/*    No copyright is claimed in the United States                    */
/*    under Title 17, U.S. Code.                                      */

/*    All Other Rights Reserved.                                      */


#ifndef __42GLUTGUI_H__
#define __42GLUTGUI_H__

#include <stdlib.h>
#include <math.h>
#include <stdio.h>

#define EXTERN extern
#include "glkit.h"
#undef EXTERN

//#ifdef __cplusplus
//namespace _42 {
//using namespace Kit;
//#endif

#define CAM_MENU_SIZE 17
#define N_AXES 0
#define L_AXES 1
#define F_AXES 2
#define B_AXES 3
#define N_GRID 4
#define L_GRID 5
#define F_GRID 6
#define B_GRID 7
#define G_GRID 8
#define CAM_FOV 9
#define TDRS 10
#define CAM_SHADOWS 11
#define ASTRO 12
#define TRUTH_VECTORS 13
#define FSW_VECTORS 14
#define MILKY_WAY 15
#define FERMI_SKY 16

#define MAP_MENU_SIZE 4
#define MAP_CLOCK 0
#define MAP_TLM_CLOCK 1
#define MAP_CREDITS 2
#define MAP_NIGHT 3

#define MONOCULAR 0
#define LEFTEYE 1
#define RIGHTEYE 2
#define ONSCREEN 0
#define OFFSCREEN 1

long MapWindowExists, OrreryWindowExists;

double RunTime,FrameRate,RealTimeDT;
long TimerHasExpired;
int TimerDuration;

int MouseClickX,MouseClickY;
long PauseFlag;
long PausedByMouse;
long CamWidth, CamHeight;
char CamTitle[80];
long CamShow[CAM_MENU_SIZE];
char CamShowLabel[CAM_MENU_SIZE][40];
long MapWidth, MapHeight;
char MapTitle[80];
long MapShow[MAP_MENU_SIZE];
char MapShowLabel[MAP_MENU_SIZE][40];
long ShowHUD;
GLuint CamWindow;
GLuint MapWindow;
double MouseScaleFactor;
long CaptureCam, CamFrame;
GLuint SunTexTag;
GLuint SunlightTexTag;
GLuint SunlightRingTexTag[3];
GLuint RockballTexTag;
GLuint RockballColCubeTag;
GLuint RockballBumpCubeTag;
GLuint IceballTexTag;
GLuint IceballColCubeTag;
GLuint NullTexTag;
GLuint NullColCubeTag;
GLuint NullBumpCubeTag;
GLuint NullCloudGlossCubeTag;
GLuint NullRingTexTag;
GLuint ShadowTexTag;
GLint MapSunVecLoc;
GLint MapCosEclLoc;
GLint MoonMapSunVecLoc;
GLint MoonMapCosEclLoc;
GLuint NASAWatermarkTexTag;
GLuint GSFCWatermarkTexTag;
GLuint FortyTwoWatermarkTexTag;

GLuint OrreryWindow;
long OrreryWidth;
long OrreryHeight;
char OrreryTitle[40];
struct OrreryPOVType Orrery;
GLuint OrrerySphereList;
GLuint OrreryRingList;

/*ProxOps Variables*/
long ProxOpsWindowExists;
GLuint ProxOpsWindow;
long ProxOpsWidth;
long ProxOpsHeight;
long ProxOpsZoom;
long ProxOpsView;
double ProxOpsRadius;
double ProxOpsTrans[2];
double ProxOpsScale[30];
char ProxOpsTitle[40];
struct WidgetType ProxOpsWidget;

GLuint DayNightTexTag;
GLuint EarthMapTexTag;
GLuint SunSpriteTexTag;
GLuint AntiSunSpriteTexTag;
GLuint MoonSpriteTexTag;
GLuint LogoTexTag;

char StarCatFileName[80];
double BuckyPf[32][3];
long BuckyNeighbor[32][6];

double SkyDistance;
double GammaCorrection;

GLuint MilkyWayList;
GLuint StarList[32];
GLuint FermiSkyList;
GLuint EgretSourceList[32];
GLuint FermiSourceList[32];
GLuint PulsarList[32];
GLuint NSkyGridList;
GLuint LSkyGridList;
GLuint FSkyGridList;
GLuint BSkyGridList;
GLuint PlanetList[11];
GLuint NightList;
GLuint MajSkyGridList;
GLuint MinSkyGridList;
GLuint SphereList;

GLfloat DistantAmbientLightColor[4];
GLfloat LocalAmbientLightColor[4];
GLfloat DistantDiffuseLightColor[4];
GLfloat LocalDiffuseLightColor[4];
GLfloat SpecularLightColor[4];
GLfloat NBrightColor[4];
GLfloat NDimColor[4];
GLfloat LBrightColor[4];
GLfloat LDimColor[4];
GLfloat FBrightColor[4];
GLfloat FDimColor[4];
GLfloat BBrightColor[4];
GLfloat BDimColor[4];
GLfloat GBrightColor[4];
GLfloat GDimColor[4];
GLfloat HUDColor[4];
GLfloat TdrsColor[4];

struct WidgetType PovWidget;
struct WidgetType HostWidget;
struct WidgetType TargetWidget;
struct WidgetType CamShowWidget;

struct WidgetType OrreryWidget;

long TlmIsStatic;
long UseEphFromTlm;
long ShowTdrsVis;
long ShowWatermark;

GLint ColorTexSamplerLoc;
GLint BumpTexSamplerLoc;
GLint EnvMapSamplerLoc;
GLint NoiseTexSamplerLoc;
GLint SpectrumTexSamplerLoc;
GLint ShadowSamplerLoc;
GLint NoiseGainLoc;
GLint NoiseBiasLoc;
GLint NoiseScaleLoc;
GLint NoiseAxisLoc;
GLint NoiseTypeLoc;
GLint ColorTexEnabledLoc;
GLint BumpTexEnabledLoc;
GLint ReflectEnabledLoc;
GLint NoiseColEnabledLoc;
GLint NoiseBumpEnabledLoc;
GLint ShadowsEnabledLoc;
GLint CNELoc;
GLint ShadowMatrixLoc;

GLuint SkyCube;
GLuint NoiseTex;

long ShadowsEnabled;
long RiftEnabled;
long SeeThruPassNeeded;
GLfloat ShadowMatrix[16];

long NearWorld;

GLfloat LightPosN[4];

GLfloat ShadowFromNMatrix[16]; /* 4x4 Matrix transforms from N frame to Shadow Texture space */
GLfloat CNE[9]; /* DCM between N and Eye frame */


char Banner[120];
GLfloat BannerColor[4];

void GeomToDisplayLists(struct GeomType *G);

void DrawWidget(struct WidgetType *W);
void DrawProxOpsHUD(void);
void DrawProxOps(void);
void ProxOpsMouseButtonHandler(int Button, int State, int x, int y);
void InitProxOpsWidget(void);
void ProxOpsReshapeHandler(int width, int height);
void InitProxOpsWindow(void);
void Idle(void);
void AsciiKeyHandler(unsigned char CharCode, int x, int y);


//#ifdef __cplusplus
//}
//#endif

#endif /* __42GLUTGUI_H__ */

/*    This file is distributed with 42,                               */
/*    the (mostly harmless) spacecraft dynamics simulation            */
/*    created by Eric Stoneking of NASA Goddard Space Flight Center   */

/*    Copyright 2010 United States Government                         */
/*    as represented by the Administrator                             */
/*    of the National Aeronautics and Space Administration.           */

/*    No copyright is claimed in the United States                    */
/*    under Title 17, U.S. Code.                                      */

/*    All Other Rights Reserved.                                      */


#include "42.h"
#define EXTERN
#include "42gl.h"
#undef EXTERN

/* #ifdef __cplusplus
** namespace _42 {
** using namespace Kit;
** #endif
*/

/*******************************************************************************/
void DrawWorldAsBackdrop(struct WorldType *W,double PosN[3],double svn[3])
{
      struct AtmoType *A;
      GLint UniLoc;
      double sve[3],PosE[3],UnitWorldVecE[3],WorldDist;
      double CosWorldAng,CosAtmoAng,CosRingAng;
      double CWE[3][3],PosW[3];
      GLfloat LightPos[4] = {0.0,0.0,0.0,0.0};
      GLfloat CWEarray[9];
      long i,j;

      A = &W->Atmo;

      glPushMatrix();
      glLoadIdentity();

      /* Transform and scale into Eye frame */
      MxV(POV.CN,svn,sve);
      for(i=0;i<3;i++) LightPos[i] = sve[i];
      glLightfv(GL_LIGHT0,GL_POSITION,LightPos);
      MxV(POV.CN,PosN,PosE);
      MxV(W->CWN,PosN,PosW);
      MxMT(W->CWN,POV.CN,CWE);
      WorldDist = CopyUnitV(PosE,UnitWorldVecE);
      for(i=0;i<3;i++) UnitWorldVecE[i] = -UnitWorldVecE[i];

      if (WorldDist > W->rad) {
         CosWorldAng = sqrt(1.0-W->rad*W->rad/(WorldDist*WorldDist));
      }
      else CosWorldAng = 0.0;

      if (WorldDist > A->rad) {
         CosAtmoAng = sqrt(1.0-A->rad*A->rad/(WorldDist*WorldDist));
      }
      else CosAtmoAng = -1.0;

      if (WorldDist > W->RingOuter) {
         CosRingAng = sqrt(1.0-W->RingOuter*W->RingOuter/(WorldDist*WorldDist));
      }
      else CosRingAng = -1.0;

      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_CUBE_MAP,W->ColCubeTag);
      glActiveTexture(GL_TEXTURE1);
      glBindTexture(GL_TEXTURE_CUBE_MAP,W->BumpCubeTag);
      glActiveTexture(GL_TEXTURE2);
      glBindTexture(GL_TEXTURE_CUBE_MAP,W->CloudGlossCubeTag);
      glActiveTexture(GL_TEXTURE3);
      glBindTexture(GL_TEXTURE_1D,W->RingTexTag);

      glUseProgram(WorldShaderProgram);

      UniLoc = glGetUniformLocation(WorldShaderProgram,"HasAtmo");
      glUniform1i(UniLoc,A->Exists);

      UniLoc = glGetUniformLocation(WorldShaderProgram,"Br");
      glUniform3f(UniLoc,A->RayScat[0],A->RayScat[1],A->RayScat[2]);

      UniLoc = glGetUniformLocation(WorldShaderProgram,"Bm");
      glUniform1f(UniLoc,A->MieScat);

      UniLoc = glGetUniformLocation(WorldShaderProgram,"Hr");
      glUniform1f(UniLoc,A->RayScaleHt);

      UniLoc = glGetUniformLocation(WorldShaderProgram,"Hm");
      glUniform1f(UniLoc,A->MieScaleHt);

      UniLoc = glGetUniformLocation(WorldShaderProgram,"Gm");
      glUniform1f(UniLoc,A->MieG);

      UniLoc = glGetUniformLocation(WorldShaderProgram,"UnitWorldVecE");
      glUniform3f(UniLoc,UnitWorldVecE[0],UnitWorldVecE[1],UnitWorldVecE[2]);

      UniLoc = glGetUniformLocation(WorldShaderProgram,"CosWorldAng");
      glUniform1f(UniLoc,CosWorldAng);

      UniLoc = glGetUniformLocation(WorldShaderProgram,"CosAtmoAng");
      glUniform1f(UniLoc,CosAtmoAng);

      UniLoc = glGetUniformLocation(WorldShaderProgram,"CosRingAng");
      glUniform1f(UniLoc,CosRingAng);

      UniLoc = glGetUniformLocation(WorldShaderProgram,"WorldRad");
      glUniform1f(UniLoc,W->rad);

      UniLoc = glGetUniformLocation(WorldShaderProgram,"PosEyeW");
      glUniform3f(UniLoc,PosW[0],PosW[1],PosW[2]);

      UniLoc = glGetUniformLocation(WorldShaderProgram,"MagPosEye");
      glUniform1f(UniLoc,MAGV(PosW));

      UniLoc = glGetUniformLocation(WorldShaderProgram,"CWE");
      for(i=0;i<3;i++) {
         for(j=0;j<3;j++) CWEarray[3*i+j] = CWE[i][j];
      }
      glUniformMatrix3fv(UniLoc,1,1,CWEarray);

      UniLoc = glGetUniformLocation(WorldShaderProgram,"HasRing");
      glUniform1i(UniLoc,W->HasRing);

      glBegin(GL_QUADS);
         glVertex3d(-POV.Width,-POV.Height,-1.0);
         glVertex3d( POV.Width,-POV.Height,-1.0);
         glVertex3d( POV.Width, POV.Height,-1.0);
         glVertex3d(-POV.Width, POV.Height,-1.0);
      glEnd();

      glUseProgram(0);

      glPopMatrix();
}
/**********************************************************************/
void DrawSunAsBackdrop(void)
{
      GLint UniLoc;
      double svh[3],RadRatio,RadRatio2;
      double UnitSunVecE[3],SunDist;
      double CosSunAng,CosCoronaAng;
      long i;

      glPushMatrix();
      glLoadIdentity();

      /* Transform and scale into Eye frame */
      for(i=0;i<3;i++) svh[i] = -POV.PosH[i];
      SunDist = UNITV(svh);
      MxV(POV.CH,svh,UnitSunVecE);

      RadRatio = World[0].rad/SunDist;
      RadRatio2 = RadRatio*RadRatio;
      CosSunAng = sqrt(1.0-RadRatio2);
      CosCoronaAng = sqrt(1.0-16.0*RadRatio2);

      glUseProgram(SunShaderProgram);

      UniLoc = glGetUniformLocation(SunShaderProgram,"UnitSunVecE");
      glUniform3f(UniLoc,UnitSunVecE[0],UnitSunVecE[1],UnitSunVecE[2]);

      UniLoc = glGetUniformLocation(SunShaderProgram,"CosSunAng");
      glUniform1f(UniLoc,CosSunAng);

      UniLoc = glGetUniformLocation(SunShaderProgram,"CosCoronaAng");
      glUniform1f(UniLoc,CosCoronaAng);

      glBegin(GL_QUADS);
         glVertex3d(-POV.Width,-POV.Height,-1.0);
         glVertex3d( POV.Width,-POV.Height,-1.0);
         glVertex3d( POV.Width, POV.Height,-1.0);
         glVertex3d(-POV.Width, POV.Height,-1.0);
      glEnd();

      glUseProgram(0);

      glPopMatrix();
}
/**********************************************************************/
void GeomToDisplayLists(struct GeomType *G)
{
      long Im,TexActive,Iv;
      long Ip;
      struct PolyType *P;
      struct MatlType *M;
      double V[3];
      double u,v;

/* .. Depth Pass */
      G->DepthListTag = glGenLists(1);
      glNewList(G->DepthListTag,GL_COMPILE);

      for(Im=0;Im<G->Nmatl;Im++) {
         M = &Matl[G->Matl[Im]];
         /* if (M->Kd[3] == 1.0) { *//* Only opaque polys cast shadows */
            /* Sweep for triangles */
            glBegin(GL_TRIANGLES);
            for(Ip=0;Ip<G->Npoly;Ip++) {
               P = &G->Poly[Ip];
               if (P->Matl == G->Matl[Im]) {
                  if (P->Nv == 3) {
                     for(Iv=0;Iv<3;Iv++) {
                        glVertex3dv(G->V[P->V[Iv]]);
                     }
                  }
               }
            }
            glEnd();
            /* Sweep for quads */
            glBegin(GL_QUADS);
            for(Ip=0;Ip<G->Npoly;Ip++) {
               P = &G->Poly[Ip];
               if (P->Matl == G->Matl[Im]) {
                  if (P->Nv == 4) {
                     for(Iv=0;Iv<4;Iv++) {
                        glVertex3dv(G->V[P->V[Iv]]);
                     }
                  }
               }
            }
            glEnd();
            /* Sweep for all other polygons */
            for(Ip=0;Ip<G->Npoly;Ip++) {
               P = &G->Poly[Ip];
               if (P->Matl == G->Matl[Im]) {
                  if (P->Nv > 4) {
                     glBegin(GL_POLYGON);
                     for(Iv=0;Iv<P->Nv;Iv++) {
                        glVertex3dv(G->V[P->V[Iv]]);
                     }
                     glEnd();
                  }
               }
            }
         /*}*/
      }
      glEndList();

/* .. Opaque Polys Pass */
      G->OpaqueListTag = glGenLists(1);
      glNewList(G->OpaqueListTag,GL_COMPILE);

      CNELoc = glGetUniformLocation(BodyShaderProgram,"CNE");
      ShadowMatrixLoc = glGetUniformLocation(BodyShaderProgram,"ShadowMatrix");
      ColorTexSamplerLoc = glGetUniformLocation(BodyShaderProgram,"ColorTexture");
      BumpTexSamplerLoc = glGetUniformLocation(BodyShaderProgram,"BumpTexture");
      EnvMapSamplerLoc = glGetUniformLocation(BodyShaderProgram,"EnvironmentMap");
      NoiseTexSamplerLoc = glGetUniformLocation(BodyShaderProgram,"NoiseTexture");
      SpectrumTexSamplerLoc = glGetUniformLocation(BodyShaderProgram,"SpectrumTexture");
      ShadowSamplerLoc = glGetUniformLocation(BodyShaderProgram,"ShadowMap");
      NoiseGainLoc = glGetUniformLocation(BodyShaderProgram,"NoiseGain");
      NoiseBiasLoc = glGetUniformLocation(BodyShaderProgram,"NoiseBias");
      NoiseScaleLoc = glGetUniformLocation(BodyShaderProgram,"NoiseScale");
      NoiseAxisLoc = glGetUniformLocation(BodyShaderProgram,"NoiseAxis");
      NoiseTypeLoc = glGetUniformLocation(BodyShaderProgram,"NoiseType");
      ColorTexEnabledLoc = glGetUniformLocation(BodyShaderProgram,"ColorTexEnabled");
      BumpTexEnabledLoc = glGetUniformLocation(BodyShaderProgram,"BumpTexEnabled");
      ReflectEnabledLoc = glGetUniformLocation(BodyShaderProgram,"ReflectEnabled");
      NoiseColEnabledLoc = glGetUniformLocation(BodyShaderProgram,"NoiseColEnabled");
      NoiseBumpEnabledLoc = glGetUniformLocation(BodyShaderProgram,"NoiseBumpEnabled");
      ShadowsEnabledLoc = glGetUniformLocation(BodyShaderProgram,"ShadowsEnabled");
      glUniform1i(ColorTexSamplerLoc,0);
      glUniform1i(BumpTexSamplerLoc,1);
      glUniform1i(EnvMapSamplerLoc,2);
      glUniform1i(NoiseTexSamplerLoc,3);
      glUniform1i(SpectrumTexSamplerLoc,4);
      glUniform1i(ShadowSamplerLoc,5);

      for(Im=0;Im<G->Nmatl;Im++) {
         M = &Matl[G->Matl[Im]];
         if (M->Kd[3] == 1.0) { /* Draw only opaque polys on this pass */

            glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,M->Ka);
            glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,M->Kd);
            glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,M->Ks);
            glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,M->Ke);
            glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,M->Ns);

            TexActive = 0;
            glUniform1i(ColorTexEnabledLoc,FALSE);
            glUniform1i(BumpTexEnabledLoc,FALSE);
            glUniform1i(ReflectEnabledLoc,FALSE);
            glUniform1i(NoiseColEnabledLoc,FALSE);
            glUniform1i(NoiseBumpEnabledLoc,FALSE);
            if (M->ColorTex) {
               TexActive = 1;
               glActiveTexture(GL_TEXTURE0);
               glBindTexture(GL_TEXTURE_2D,M->ColorTex);
               glUniform1i(ColorTexEnabledLoc,TRUE);
            }
            if (M->BumpTex) {
               TexActive = 1;
               glActiveTexture(GL_TEXTURE1);
               glBindTexture(GL_TEXTURE_2D,M->BumpTex);
               glUniform1i(BumpTexEnabledLoc,TRUE);
            }
            if (M->Refl > 0.0) {
               TexActive = 1;
               glActiveTexture(GL_TEXTURE2);
               glEnable(GL_TEXTURE_CUBE_MAP);
               glBindTexture(GL_TEXTURE_CUBE_MAP,SkyCube);
               glUniform1i(ReflectEnabledLoc,TRUE);
            }
            if (M->NoiseColEnabled) {
               glActiveTexture(GL_TEXTURE3);
               glBindTexture(GL_TEXTURE_3D,NoiseTex);
               glActiveTexture(GL_TEXTURE4);
               glBindTexture(GL_TEXTURE_1D,M->SpectrumTex);
               glUniform4f(NoiseGainLoc,M->NoiseGain[0],M->NoiseGain[1],
                  M->NoiseGain[2],M->NoiseGain[3]);
               glUniform1f(NoiseBiasLoc,M->NoiseBias);
               glUniform1f(NoiseScaleLoc,M->NoiseScale);
               glUniform3f(NoiseAxisLoc,M->NoiseAxis[0],
                  M->NoiseAxis[1],M->NoiseAxis[2]);
               glUniform1i(NoiseTypeLoc,M->NoiseType);
               glUniform1i(NoiseColEnabledLoc,TRUE);
            }
            if (M->NoiseBumpEnabled) {
               glActiveTexture(GL_TEXTURE3);
               glBindTexture(GL_TEXTURE_3D,NoiseTex);
               glUniform1i(NoiseBumpEnabledLoc,TRUE);
            }

            /* Sweep for triangles */
            glBegin(GL_TRIANGLES);
            for(Ip=0;Ip<G->Npoly;Ip++) {
               P = &G->Poly[Ip];
               if (P->Matl == G->Matl[Im]) {
                  if (P->Nv == 3) {
                     for(Iv=0;Iv<3;Iv++) {
                        if(TexActive) {
                           if (P->HasTex) {
                              glTexCoord2dv(G->Vt[P->Vt[Iv]]);
                           }
                           else {
                              V[0] = G->V[P->V[Iv]][0];
                              V[1] = G->V[P->V[Iv]][1];
                              V[2] = G->V[P->V[Iv]][2];
                              u = VoV(V,P->Uhat);
                              v = VoV(V,P->Vhat);
                              glTexCoord2d(u,v);
                           }
                        }
                        if (P->HasNorm)
                           glNormal3dv(G->Vn[P->Vn[Iv]]);
                        glVertex3dv(G->V[P->V[Iv]]);
                     }
                  }
               }
            }
            glEnd();
            /* Sweep for quads */
            glBegin(GL_QUADS);
            for(Ip=0;Ip<G->Npoly;Ip++) {
               P = &G->Poly[Ip];
               if (P->Matl == G->Matl[Im]) {
                  if (P->Nv == 4) {
                     for(Iv=0;Iv<4;Iv++) {
                        if(TexActive) {
                           if (P->HasTex) {
                              glTexCoord2dv(G->Vt[P->Vt[Iv]]);
                           }
                           else {
                              V[0] = G->V[P->V[Iv]][0];
                              V[1] = G->V[P->V[Iv]][1];
                              V[2] = G->V[P->V[Iv]][2];
                              u = VoV(V,P->Uhat);
                              v = VoV(V,P->Vhat);
                              glTexCoord2d(u,v);
                           }
                        }
                        if (P->HasNorm)
                           glNormal3dv(G->Vn[P->Vn[Iv]]);
                        glVertex3dv(G->V[P->V[Iv]]);
                     }
                  }
               }
            }
            glEnd();
            /* Sweep for all other polygons */
            for(Ip=0;Ip<G->Npoly;Ip++) {
               P = &G->Poly[Ip];
               if (P->Matl == G->Matl[Im]) {
                  if (P->Nv > 4) {
                     glBegin(GL_POLYGON);
                     for(Iv=0;Iv<P->Nv;Iv++) {
                        if(TexActive) {
                           if (P->HasTex) {
                              glTexCoord2dv(G->Vt[P->Vt[Iv]]);
                           }
                           else {
                              V[0] = G->V[P->V[Iv]][0];
                              V[1] = G->V[P->V[Iv]][1];
                              V[2] = G->V[P->V[Iv]][2];
                              u = VoV(V,P->Uhat);
                              v = VoV(V,P->Vhat);
                              glTexCoord2d(u,v);
                           }
                        }
                        if (P->HasNorm)
                           glNormal3dv(G->Vn[P->Vn[Iv]]);
                        glVertex3dv(G->V[P->V[Iv]]);
                     }
                     glEnd();
                  }
               }
            }

            if (M->Refl > 0.0) {
               glActiveTexture(GL_TEXTURE2);
               glDisable(GL_TEXTURE_CUBE_MAP);
            }
         }
      }
      glEndList();

/* .. SeeThru Polys Pass */
      G->SeeThruListTag = glGenLists(1);
      glNewList(G->SeeThruListTag,GL_COMPILE);

      for(Im=0;Im<G->Nmatl;Im++) {
         M = &Matl[G->Matl[Im]];
         if (M->Kd[3] < 1.0) { /* Draw only seethru polys on this pass */
            SeeThruPassNeeded = TRUE;
            glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,M->Ka);
            glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,M->Kd);
            glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,M->Ks);
            glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,M->Ke);
            glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,M->Ns);
            glUniform1i(ColorTexEnabledLoc,FALSE);
            glUniform1i(BumpTexEnabledLoc,FALSE);
            glUniform1i(ReflectEnabledLoc,FALSE);
            glUniform1i(NoiseColEnabledLoc,FALSE);
            glUniform1i(NoiseBumpEnabledLoc,FALSE);

            /* Sweep for triangles */
            glBegin(GL_TRIANGLES);
            for(Ip=0;Ip<G->Npoly;Ip++) {
               P = &G->Poly[Ip];
               if (P->Matl == G->Matl[Im]) {
                  if (P->Nv == 3) {
                     for(Iv=0;Iv<3;Iv++) {
                        if (P->HasNorm)
                           glNormal3dv(G->Vn[P->Vn[Iv]]);
                        glVertex3dv(G->V[P->V[Iv]]);
                     }
                  }
               }
            }
            glEnd();
            /* Sweep for quads */
            glBegin(GL_QUADS);
            for(Ip=0;Ip<G->Npoly;Ip++) {
               P = &G->Poly[Ip];
               if (P->Matl == G->Matl[Im]) {
                  if (P->Nv == 4) {
                     for(Iv=0;Iv<4;Iv++) {
                        if (P->HasNorm)
                           glNormal3dv(G->Vn[P->Vn[Iv]]);
                        glVertex3dv(G->V[P->V[Iv]]);
                     }
                  }
               }
            }
            glEnd();
            /* Sweep for all other polygons */
            for(Ip=0;Ip<G->Npoly;Ip++) {
               P = &G->Poly[Ip];
               if (P->Matl == G->Matl[Im]) {
                  if (P->Nv > 4) {
                     glBegin(GL_POLYGON);
                     for(Iv=0;Iv<P->Nv;Iv++) {
                        if (P->HasNorm)
                           glNormal3dv(G->Vn[P->Vn[Iv]]);
                        glVertex3dv(G->V[P->V[Iv]]);
                     }
                     glEnd();
                  }
               }
            }
         }
      }
      glEndList();
}
/**********************************************************************/
void SetEye(long Eye)
{
      long i,j;

      if (Eye == LEFTEYE) {
      }
      else if (Eye == RIGHTEYE) {
      }
      else {
         glMatrixMode(GL_PROJECTION);
         glLoadIdentity();
         gluPerspective(POV.Angle, POV.AR, POV.Near, POV.Far);

         /* Transform from POV frame to N frame */
         glMatrixMode(GL_MODELVIEW);
         glLoadIdentity();

         BuildViewMatrix(POV.CN,POV.PosR,POV.ViewMatrix);

         glMultMatrixf(POV.ViewMatrix);
         for(i=0;i<3;i++) {
            for(j=0;j<3;j++) CNE[3*i+j] = POV.CN[i][j];
         }

      }
}
/**********************************************************************/
void SetDestination(long Dest)
{
      if (Dest == OFFSCREEN) {
      }
      else {
      }
}
/*********************************************************************/
void DrawPlanetLabels(GLfloat length)
{
      GLfloat Black[4] = {0.0,0.0,0.0,1.0};
      double Vec[3];
      long i,j;
      struct OrbitType *O;

      glPushMatrix();
      O = &Orb[POV.Host.RefOrb];
      RotateL2R(World[O->World].CNH);

      glDisable(GL_LIGHTING);
      for(i=SOL;i<=LUNA;i++) {
         if (World[i].Exists) {
            glColor4fv(World[i].Color);
            for(j=0;j<3;j++) Vec[j] = World[i].PosH[j]-SC[POV.Host.SC].PosH[j];
            UNITV(Vec);
            glBegin(GL_LINES);
               glVertex3f(0.0,0.0,0.0);
               glVertex3f(length*Vec[0],length*Vec[1],length*Vec[2]);
            glEnd();
            glRasterPos3f((length+0.2)*Vec[0],
                          (length+0.2)*Vec[1],
                          (length+0.2)*Vec[2]);
            glBitmap(8, 14, 0.0, 0.0, 0.0, 0.0, World[i].Glyph);
         }
      }
      glPopMatrix();
      glColor4fv(Black);
      glEnable(GL_LIGHTING);
}
/*********************************************************************/
void DrawThrusterPlume(struct ThrType *Thr)
{
      GLfloat TipColor[4] = {0.918,0.67,0.25,0.3};
      GLfloat CoreColor[4] = {1.0,0.937,0.259,1.0};
      double Rad = 0.05;
      double scl = 5.0;
      double p[3],ang,s,c,f;
      double X[3],Y[3]; /* Basis vectors perpendicular to Axis */
      long j;

      if (Thr->F > 0.0) {

         PerpBasis(Thr->A,X,Y);

         f = sqrt(Thr->F/Thr->Fmax);
         glDisable(GL_LIGHTING);
         glDisable(GL_CULL_FACE);
         glDepthMask(GL_FALSE);

         glColor4fv(CoreColor);
         glBegin(GL_TRIANGLE_FAN);
            glNormal3d(-Thr->A[0],-Thr->A[1],-Thr->A[2]);
            for(j=0;j<3;j++) p[j] = Thr->PosB[j]-0.5*scl*f*Thr->A[j];
            glVertex3dv(p);
            for(ang=0.0;ang<=360.0;ang+=30.0) {
               s=-sin(ang*D2R);
               c=cos(ang*D2R);
               for(j=0;j<3;j++) p[j] = Thr->PosB[j] + Rad*(c*X[j]+s*Y[j]);
               glVertex3dv(p);
            }
         glEnd();

         glColor4fv(TipColor);
         glBegin(GL_TRIANGLE_FAN);
            glNormal3d(-Thr->A[0],-Thr->A[1],-Thr->A[2]);
            for(j=0;j<3;j++) p[j] = Thr->PosB[j]-scl*f*Thr->A[j];
            glVertex3dv(p);
            for(ang=0.0;ang<=360.0;ang+=30.0) {
               s=-sin(ang*D2R);
               c=cos(ang*D2R);
               for(j=0;j<3;j++) p[j] = Thr->PosB[j] + Rad*(c*X[j]+s*Y[j]);
               glVertex3dv(p);
            }
         glEnd();

         glEnable(GL_LIGHTING);
         glEnable(GL_CULL_FACE);
         glDepthMask(GL_TRUE);
      }
}
/*********************************************************************/
void DrawWidget(struct WidgetType *W)
{
      struct SpotType *S;
      long i;

      glColor4fv(W->BorderColor);
      glBegin(GL_LINE_LOOP);
         glVertex2i(W->xmin,W->ymin);
         glVertex2i(W->xmax,W->ymin);
         glVertex2i(W->xmax,W->ymax);
         glVertex2i(W->xmin,W->ymax);
      glEnd();

      glBegin(GL_QUADS);
         for(i=0;i<W->Nspot;i++) {
            S = &W->Spot[i];
            if (S->Visible) {
               if(S->Selected)
                  glColor4fv(W->SelectedColor);
               else
                  glColor4fv(W->UnselectedColor);
               glVertex2i(S->xmin,S->ymin);
               glVertex2i(S->xmin,S->ymax);
               glVertex2i(S->xmax,S->ymax);
               glVertex2i(S->xmax,S->ymin);
            }
         }
      glEnd();
      glColor4fv(W->TextColor);
}
/*********************************************************************/
void DrawBanner(int ScreenWidth, int ScreenHeight)
{
      GLfloat Black[4] = {0.0,0.0,0.0,1.0};

      glDisable(GL_LIGHTING);
      glMatrixMode(GL_PROJECTION);
      glPushMatrix();
      glLoadIdentity();
      gluOrtho2D(0,ScreenWidth,ScreenHeight,0);
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();

      glColor4fv(BannerColor);
      glRasterPos2i(ScreenWidth/2 - 4*((int) strlen(Banner)),75);
      DrawBitmapString(GLUT_BITMAP_HELVETICA_18,Banner);

      glMatrixMode(GL_PROJECTION);
      glPopMatrix();
      glMatrixMode(GL_MODELVIEW);

      glColor4fv(Black);
      glEnable(GL_LIGHTING);
}
/*********************************************************************/
void DrawCamHUD(void)
{
      char s[120];
      char mon[12][4]={"Jan","Feb","Mar","Apr","May","Jun",
                        "Jul","Aug","Sep","Oct","Nov","Dec"};
      char Frame[5][2]={"N","L","F","S","B"};
      char Axis[6][3] = {"+X","+Y","+Z","-X","-Y","-Z"};
      char Mode[3][20] = {"TRACK HOST","TRACK TARGET","FIXED IN HOST"};
      char WorldName[80];
      double RA,Dec;
      GLfloat BoxColor[4] = {0.133,0.545,0.133,1.0};
      GLfloat ClockColor[4] = {0.604,0.804,0.196,1.0};
      GLfloat ProxOpsGridColor[4] = {1.0,1.0,1.0,0.5};
      GLfloat Black[4] = {0.0,0.0,0.0,1.0};
      int i;
      struct WidgetType *W;
      struct OrbitType *O;

      glMatrixMode(GL_PROJECTION);
      glPushMatrix();
      glLoadIdentity();
      gluOrtho2D(0,CamWidth,CamHeight,0);
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
      glMaterialfv(GL_FRONT,GL_AMBIENT,Black);
      glMaterialfv(GL_FRONT,GL_DIFFUSE,Black);
      glMaterialfv(GL_FRONT,GL_SPECULAR,Black);
      glMaterialfv(GL_FRONT,GL_EMISSION,Black);

      glDisable(GL_LIGHTING);

/* .. Clock Box */
      glColor4fv(BoxColor);
      glBegin(GL_LINE_LOOP);
      glVertex2i(2,2);
      glVertex2i(172,2);
      glVertex2i(172,60);
      glVertex2i(2,60);
      glEnd();

      glColor4fv(ClockColor);
      sprintf(s,"Sim Time = % 8.2f",SimTime);
      glRasterPos2i(10,20);
      DrawBitmapString(GLUT_BITMAP_8_BY_13,s);

      sprintf(s,"%2ld %s %4ld",UTC.Day,mon[UTC.Month-1],UTC.Year);
      glRasterPos2i(40,35);
      DrawBitmapString(GLUT_BITMAP_8_BY_13,s);

      sprintf(s,"UTC %03ld-%02ld:%02ld:%05.2lf",UTC.doy,UTC.Hour,UTC.Minute,UTC.Second);
      glRasterPos2i(10,50);
      DrawBitmapString(GLUT_BITMAP_8_BY_13,s);

/* .. POV Box */
      W = &PovWidget;
      DrawWidget(W);

      glRasterPos2i((W->xmin+W->xmax)/2-12,W->ymin+15);
      DrawBitmapString(GLUT_BITMAP_8_BY_13,"POV");

      glRasterPos2i(W->xmin+8,W->ymin+30);
      DrawBitmapString(GLUT_BITMAP_8_BY_13,Mode[POV.Mode]);

      glRasterPos2i(W->xmin+8,W->ymin+45);
      DrawBitmapString(GLUT_BITMAP_8_BY_13,"Fixed in");
      for(i=0;i<5;i++) {
         glRasterPos2i(W->xmin+110+10*i,W->ymin+45);
         DrawBitmapString(GLUT_BITMAP_8_BY_13,Frame[i]);
      }

      sprintf(s,"Boresight: %s",Axis[POV.BoreAxis]);
      glRasterPos2i(W->xmin+8,W->ymin+60);
      DrawBitmapString(GLUT_BITMAP_8_BY_13,s);

      sprintf(s,"  Up Axis: %s",Axis[POV.UpAxis]);
      glRasterPos2i(W->xmin+8,W->ymin+75);
      DrawBitmapString(GLUT_BITMAP_8_BY_13,s);

/* .. Show/Hide Menu */
      W = &CamShowWidget;
      DrawWidget(W);

      glRasterPos2i((W->xmin+W->xmax)/2-16,W->ymin+15);
      DrawBitmapString(GLUT_BITMAP_8_BY_13,"Show");
      for(i=0;i<CAM_MENU_SIZE;i++) {
         if (CamShow[i]) {
            sprintf(s,"x %s",CamShowLabel[i]);
         }
         else {
            sprintf(s,"  %s",CamShowLabel[i]);
         }
         glRasterPos2i(W->xmin+8,W->ymin+30+15*i);
         DrawBitmapString(GLUT_BITMAP_8_BY_13,s);
      }

/* .. Host */
      W = &HostWidget;
      DrawWidget(W);
      O = &Orb[POV.Host.RefOrb];

      glRasterPos2i((W->xmin+W->xmax)/2-16,W->ymin+15);
      DrawBitmapString(GLUT_BITMAP_8_BY_13,"Host");

      if (O->Regime == ORB_THREE_BODY) {
         sprintf(WorldName,"%s-%s",
            World[Orb[POV.Host.RefOrb].Body1].Name,
            World[Orb[POV.Host.RefOrb].Body2].Name);
      }
      else {
         strcpy(WorldName,World[POV.Host.World].Name);
      }
      glRasterPos2i(W->xmin+30,W->ymin+30);
      sprintf(s,"  World: %s",WorldName);
      DrawBitmapString(GLUT_BITMAP_8_BY_13,s);

      glRasterPos2i(W->xmin+30,W->ymin+45);
      sprintf(s,"Ref Orb: %ld",POV.Host.RefOrb);
      DrawBitmapString(GLUT_BITMAP_8_BY_13,s);
      glRasterPos2i(W->xmin+30,W->ymin+60);
      sprintf(s,"    Frm: %ld",POV.Host.RefOrb);
      DrawBitmapString(GLUT_BITMAP_8_BY_13,s);
      glRasterPos2i(W->xmin+30,W->ymin+75);
      sprintf(s,"    S/C: %ld",POV.Host.SC);
      DrawBitmapString(GLUT_BITMAP_8_BY_13,s);
      glRasterPos2i(W->xmin+30,W->ymin+90);
      sprintf(s,"   Body: %ld",POV.Host.Body);
      DrawBitmapString(GLUT_BITMAP_8_BY_13,s);
      glRasterPos2i(W->xmin+10,W->ymin+120);
      sprintf(s,"Range: %7.2lf",POV.Range);
      DrawBitmapString(GLUT_BITMAP_8_BY_13,s);
      glRasterPos2i(W->xmin+10,W->ymin+135);
      if (POV.Host.Type >= TARGET_SC) {
         if (SC[POV.Host.SC].Eclipse)
            DrawBitmapString(GLUT_BITMAP_8_BY_13,"In Eclipse");
         else
            DrawBitmapString(GLUT_BITMAP_8_BY_13,"In Sunlight");
      }
      glRasterPos2i(W->Spot[5].xmin,W->Spot[5].ymax);
      DrawBitmapString(GLUT_BITMAP_8_BY_13,"-");
      glRasterPos2i(W->Spot[6].xmin,W->Spot[6].ymax);
      DrawBitmapString(GLUT_BITMAP_8_BY_13,"+");
      glRasterPos2i(W->Spot[7].xmin,W->Spot[7].ymax);
      DrawBitmapString(GLUT_BITMAP_8_BY_13,"+");
      glRasterPos2i(W->Spot[8].xmin,W->Spot[8].ymax);
      DrawBitmapString(GLUT_BITMAP_8_BY_13,"-");

/* .. Target */
      W = &TargetWidget;
      DrawWidget(W);
      O = &Orb[POV.Host.RefOrb];

      glRasterPos2i((W->xmin+W->xmax)/2-24,W->ymin+15);
      DrawBitmapString(GLUT_BITMAP_8_BY_13,"Target");

      if (O->Regime == ORB_CENTRAL || O->Regime == ORB_ZERO) {
         strcpy(WorldName,World[POV.Host.World].Name);
      }
      else {
         sprintf(WorldName,"%s-%s",
            World[Orb[POV.Host.RefOrb].Body1].Name,
            World[Orb[POV.Host.RefOrb].Body2].Name);
      }
      glRasterPos2i(W->xmin+30,W->ymin+30);
      sprintf(s,"  World: %s",WorldName);
      DrawBitmapString(GLUT_BITMAP_8_BY_13,s);

      glRasterPos2i(W->xmin+30,W->ymin+45);
      sprintf(s,"Ref Orb: %ld",POV.Target.RefOrb);
      DrawBitmapString(GLUT_BITMAP_8_BY_13,s);
      glRasterPos2i(W->xmin+30,W->ymin+60);
      sprintf(s,"    Frm: %ld",POV.Target.RefOrb);
      DrawBitmapString(GLUT_BITMAP_8_BY_13,s);
      glRasterPos2i(W->xmin+30,W->ymin+75);
      sprintf(s,"    S/C: %ld",POV.Target.SC);
      DrawBitmapString(GLUT_BITMAP_8_BY_13,s);
      glRasterPos2i(W->xmin+30,W->ymin+90);
      sprintf(s,"   Body: %ld",POV.Target.Body);
      DrawBitmapString(GLUT_BITMAP_8_BY_13,s);
      glRasterPos2i(W->xmin+10,W->ymin+120);
      sprintf(s,"Range: %7.2lf",POV.Range);
      DrawBitmapString(GLUT_BITMAP_8_BY_13,s);
      glRasterPos2i(W->xmin+10,W->ymin+135);
      if (POV.Target.Type >= TARGET_SC) {
         if (SC[POV.Target.SC].Eclipse)
            DrawBitmapString(GLUT_BITMAP_8_BY_13,"In Eclipse");
         else
            DrawBitmapString(GLUT_BITMAP_8_BY_13,"In Sunlight");
      }
      glRasterPos2i(W->Spot[5].xmin,W->Spot[5].ymax);
      DrawBitmapString(GLUT_BITMAP_8_BY_13,"-");
      glRasterPos2i(W->Spot[6].xmin,W->Spot[6].ymax);
      DrawBitmapString(GLUT_BITMAP_8_BY_13,"+");
      glRasterPos2i(W->Spot[7].xmin,W->Spot[7].ymax);
      DrawBitmapString(GLUT_BITMAP_8_BY_13,"+");
      glRasterPos2i(W->Spot[8].xmin,W->Spot[8].ymax);
      DrawBitmapString(GLUT_BITMAP_8_BY_13,"-");

/* .. Boresight Direction */
      if (CamShow[N_AXES]) {
         glColor4fv(NBrightColor);
         RA = atan2(-POV.CN[2][1],-POV.CN[2][0])*R2D;
         Dec = asin(-POV.CN[2][2])*R2D;
         sprintf(s,"View RA:% 5.2f Dec:% 5.2f",RA,Dec);
         glRasterPos2i(CamWidth-300,20);
         DrawBitmapString(GLUT_BITMAP_8_BY_13,s);
      }
      if (CamShow[L_AXES]) {
         glColor4fv(LBrightColor);
         sprintf(s,"View in L: [% 5.4f % 5.4f % 5.4f]",
            -POV.CL[2][0],-POV.CL[2][1],-POV.CL[2][2]);
         glRasterPos2i(CamWidth-300,35);
         DrawBitmapString(GLUT_BITMAP_8_BY_13,s);
      }
      if (CamShow[F_AXES]) {
         glColor4fv(FBrightColor);
         sprintf(s,"View in F: [% 5.4f % 5.4f % 5.4f]",
            -POV.CF[2][0],-POV.CF[2][1],-POV.CF[2][2]);
         glRasterPos2i(CamWidth-300,50);
         DrawBitmapString(GLUT_BITMAP_8_BY_13,s);
      }
      if (CamShow[B_AXES]) {
         glColor4fv(BBrightColor);
         sprintf(s,"View in B: [% 5.4f % 5.4f % 5.4f]",
            -POV.CB[2][0],-POV.CB[2][1],-POV.CB[2][2]);
         glRasterPos2i(CamWidth-300,65);
         DrawBitmapString(GLUT_BITMAP_8_BY_13,s);
      }

/* .. ProxOps Grid Spacing */
      if (CamShow[PROX_OPS]) {
         glColor4fv(ProxOpsGridColor);
         glLineWidth(2.5);
         glBegin(GL_LINE_STRIP);
            glVertex2i(CamWidth-120,CamHeight-106);
            glVertex2i(CamWidth-120,CamHeight-100);
            glVertex2i(CamWidth-40,CamHeight-100);
            glVertex2i(CamWidth-40,CamHeight-106);
         glEnd();
         if (POV.GridSpacing < 1000.0) {
            sprintf(s,"%ld m",(long) POV.GridSpacing);
         }
         else {
            sprintf(s,"%ld km",(long) (POV.GridSpacing/1000.0));
         }
         glRasterPos2i(CamWidth-80-4*strlen(s),CamHeight-104);
         DrawBitmapString(GLUT_BITMAP_8_BY_13,s);
      }

      glColor4fv(Black);
      glEnable(GL_LIGHTING);

      glMatrixMode(GL_PROJECTION);
      glPopMatrix();
      glMatrixMode(GL_MODELVIEW);
}
/**********************************************************************/
void DrawWatermarks(void)
{
      GLfloat Black[4] = {0.0,0.0,0.0,1.0};

      glMatrixMode(GL_PROJECTION);
      glPushMatrix();
      glLoadIdentity();
      gluOrtho2D(0,CamWidth,CamHeight,0);
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
      glMaterialfv(GL_FRONT,GL_AMBIENT,Black);
      glMaterialfv(GL_FRONT,GL_DIFFUSE,Black);
      glMaterialfv(GL_FRONT,GL_SPECULAR,Black);
      glMaterialfv(GL_FRONT,GL_EMISSION,Black);

      glDisable(GL_LIGHTING);

      glActiveTexture(GL_TEXTURE0);
      glEnable(GL_TEXTURE_2D);
      glColor4f(1.0,1.0,1.0,0.70);
      glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

/* Use of the following watermarks is restricted to NASA and GSFC,    */
/* respectively.  Feel free to substitute your own watermarks.        */
#if _USE_GSFC_WATERMARK_
      glBindTexture(GL_TEXTURE_2D,NASAWatermarkTexTag);
      glBegin(GL_QUADS);
         glTexCoord2f(0.0,1.0);
         glVertex2f(10,CamHeight-10);
         glTexCoord2f(1.0,1.0);
         glVertex2f(60,CamHeight-10);
         glTexCoord2f(1.0,0.0);
         glVertex2f(60,CamHeight-52);
         glTexCoord2f(0.0,0.0);
         glVertex2f(10,CamHeight-52);
      glEnd();

      glBindTexture(GL_TEXTURE_2D,GSFCWatermarkTexTag);
      glBegin(GL_QUADS);
         glTexCoord2f(0.0,1.0);
         glVertex2f(70,CamHeight-10);
         glTexCoord2f(1.0,1.0);
         glVertex2f(170,CamHeight-10);
         glTexCoord2f(1.0,0.0);
         glVertex2f(170,CamHeight-52);
         glTexCoord2f(0.0,0.0);
         glVertex2f(70,CamHeight-52);
      glEnd();
#endif

/* .. This is the 42 watermark.  Use is not restricted.               */
      glColor4f(1.0,1.0,1.0,0.50);

      glBindTexture(GL_TEXTURE_2D,FortyTwoWatermarkTexTag);
      glBegin(GL_QUADS);
         glTexCoord2f(0.0,1.0);
         glVertex2f(CamWidth-50,CamHeight-10);
         glTexCoord2f(1.0,1.0);
         glVertex2f(CamWidth-10,CamHeight-10);
         glTexCoord2f(1.0,0.0);
         glVertex2f(CamWidth-10,CamHeight-50);
         glTexCoord2f(0.0,0.0);
         glVertex2f(CamWidth-50,CamHeight-50);
      glEnd();

      glDisable(GL_TEXTURE_2D);

      glColor4fv(Black);
      glEnable(GL_LIGHTING);

      glMatrixMode(GL_PROJECTION);
      glPopMatrix();
      glMatrixMode(GL_MODELVIEW);
}
/**********************************************************************/
long ScIsVisible(long RefOrb, long Isc, double PosR[3])
{
      struct SCType *S;
      long ScVisible = 0;
      long i;

      S = &SC[Isc];


      if (S->RefOrb == RefOrb) {
         ScVisible = 1;
         for(i=0;i<3;i++) PosR[i] = S->PosR[i];
      }
      else if (Orb[RefOrb].World == Orb[S->RefOrb].World) {
         for(i=0;i<3;i++) PosR[i] = S->PosN[i]-Orb[RefOrb].PosN[i];
         if (MAGV(PosR) < SkyDistance) {
            ScVisible = 1;
         }
         else ScVisible = 0;
      }
      return(ScVisible);
}
/**********************************************************************/
void DrawFarScene(void)
{
      static long Nw;
      long Iw,Isc,i,j;
      double LoS[3];
      GLfloat WorldColor[4] = {1.0,1.0,1.0,1.0};
      double CLpermute[3][3] = {{0.0,0.0,1.0},{1.0,0.0,0.0},{0.0,1.0,0.0}};
      double Identity[3][3] = {{1.0,0.0,0.0},{0.0,1.0,0.0},{0.0,0.0,1.0}};
      double C[3][3];
      static long WorldOrder[NWORLD],TempWO;
      double Zdepth[NWORLD],rh[NWORLD][3],TempZ;
      long Done;
      struct WorldType *W;
      struct SCType *S;
      double svh[3],PosH[3],PosN[3],svn[3];
      double VisCoef,PixRad,r[3],magr;
      double PosR[3];
      GLfloat Black[4] = {0.0,0.0,0.0,1.0};
      GLubyte TdrsGlyph[32] = {0x01,0x80, 0x02,0x40, 0x04,0x20, 0x08,0x10,
                               0x10,0x08, 0x20,0x04, 0x40,0x02, 0x80,0x01,
                               0x80,0x01, 0x40,0x02, 0x20,0x04, 0x10,0x08,
                               0x08,0x10, 0x04,0x20, 0x02,0x40, 0x01,0x80};
      GLubyte WorldGlyph[32] = {0x00,0x00, 0x00,0x00, 0x07,0xe0, 0x08,0x10,
                                0x10,0x08, 0x20,0x04, 0x20,0x04, 0x20,0x04,
                                0x20,0x04, 0x20,0x04, 0x20,0x04, 0x10,0x08,
                                0x08,0x10, 0x07,0xe0, 0x00,0x00, 0x00,0x00};
      GLfloat ScColor[4] = {0.529,0.808,0.922,1.0};
      GLubyte ScGlyph[32] = {0x01,0x80, 0x02,0x40, 0x04,0x20, 0x08,0x10,
                             0x10,0x08, 0x20,0x04, 0x40,0x02, 0x80,0x01,
                             0x80,0x01, 0x40,0x02, 0x20,0x04, 0x10,0x08,
                             0x08,0x10, 0x04,0x20, 0x02,0x40, 0x01,0x80};

      static long First = 1;

      if (First) {
         First = 0;
         Nw = 0;
         for(Iw=0;Iw<NWORLD;Iw++) {
            if (World[Iw].Exists) {
               WorldOrder[Nw] = Iw;
               Nw++;
            }
         }
      }

/* .. For Milky Way, Stars, and Far Planets, set sky at 1.0 */
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      gluPerspective(POV.Angle,POV.AR,0.1,10.0);
      glMatrixMode(GL_MODELVIEW);

/* .. Transform from View frame to H frame */
      glLoadIdentity();
      RotateL2R(POV.CH);

      /* Line of Sight, from POV toward back of view frustum */
      for(i=0;i<3;i++) LoS[i] = -POV.CH[2][i];

      if (CamShow[FERMI_SKY]) glCallList(FermiSkyList);
      else if (CamShow[MILKY_WAY]) glCallList(MilkyWayList);
      DrawStars(LoS,BuckyPf,StarList);
      /*if (CamShow[FERMI_SOURCES]) Draw1FGL(LoS,BuckyPf,FermiSourceList);*/
      /*if (CamShow[EGRET_SOURCES]) DrawEgret(LoS,BuckyPf,EgretSourceList);*/
      /*if (CamShow[PULSAR_SOURCES]) DrawPulsars(LoS,BuckyPf,PulsarList);*/

      glClear(GL_DEPTH_BUFFER_BIT);


/* .. Find depth, visibility for each world */
      VisCoef = 0.5*CamHeight/POV.SinFov;
      for(i=0;i<Nw;i++) {
         Iw = WorldOrder[i];
         for(j=0;j<3;j++) rh[Iw][j] = POV.PosH[j]-World[Iw].PosH[j];
         Zdepth[i] = VoV(rh[Iw],LoS);
         if (World[Iw].HasRing) {
            World[Iw].NearExtent = -Zdepth[i]-4.0*World[Iw].rad;
            World[Iw].FarExtent  = -Zdepth[i]+4.0*World[Iw].rad;
         }
         else {
            World[Iw].NearExtent = -Zdepth[i]-1.1*World[Iw].rad;
            World[Iw].FarExtent  = -Zdepth[i]+1.1*World[Iw].rad;
         }
         if (World[Iw].Type == SUN) World[Iw].Visibility = WORLD_IS_SUN;
         else if (World[Iw].FarExtent < 0.0) World[Iw].Visibility = WORLD_IS_OUT_OF_POV;
         else {
            magr = MAGV(rh[Iw]);
            PixRad = VisCoef*World[Iw].rad/magr;
            if (PixRad < 1.0E-2)
               World[Iw].Visibility = WORLD_IS_TOO_SMALL_TO_SEE;
            else if (PixRad < 4.0)
               World[Iw].Visibility = WORLD_IS_POINT_SIZED;
            else {
               World[Iw].Visibility = WORLD_SHOWS_DISK;
            }
         }
      }

/* .. Sort Worlds from Far Front (Z big -) to Far Rear (Z big +) */
      /* Bubble Sort, though much maligned, is fast enough if */
      /* view has some temporal coherence */
      Done = FALSE;
      while (!Done) {
         Done = TRUE;
         for (i=0;i<Nw-1;i++) {
            if (Zdepth[i] > Zdepth[i+1]) {
               Done = FALSE;
               TempWO = WorldOrder[i];
               WorldOrder[i] = WorldOrder[i+1];
               WorldOrder[i+1] = TempWO;
               TempZ = Zdepth[i];
               Zdepth[i] = Zdepth[i+1];
               Zdepth[i+1] = TempZ;
            }
         }
      }

      glPointSize(4.0);
      for(i=0;i<Nw;i++) {
         Iw = WorldOrder[i];
         W = &World[Iw];
         for(j=0;j<3;j++) rh[Iw][j] = POV.PosH[j]-W->PosH[j];
         if (W->Visibility == WORLD_IS_SUN) {
            DrawSunAsBackdrop();
         }
         else if (W->Visibility == WORLD_IS_POINT_SIZED) {
            CopyUnitV(rh[Iw],r);
            r[0] *= -2.0;
            r[1] *= -2.0;
            r[2] *= -2.0;
            glDisable(GL_LIGHTING);
            glColor4fv(W->Color);
            glBegin(GL_POINTS);
               glVertex3dv(r);
            glEnd();
            glEnable(GL_LIGHTING);
         }
         else if (W->Visibility == WORLD_SHOWS_DISK) {
            for(j=0;j<3;j++) svh[j] = -W->PosH[j];
            UNITV(svh);
            if (W->GeomTag == 0) { /* World is sphere */
               MxV(World[POV.Host.World].CNH,rh[Iw],PosN);
               MxV(World[POV.Host.World].CNH,svh,svn);
               DrawWorldAsBackdrop(W,PosN,svn);
            }
            else {
               glClear(GL_DEPTH_BUFFER_BIT);
               glMatrixMode(GL_PROJECTION);
               glLoadIdentity();
               gluPerspective(POV.Angle,POV.AR,
                  W->NearExtent,W->FarExtent);
               glMatrixMode(GL_MODELVIEW);
               glUseProgram(BodyShaderProgram);
               glUniform1i(ShadowsEnabledLoc,ShadowsEnabled);
               glUniformMatrix3fv(CNELoc,1,0,CNE);
               glPushMatrix();
               glTranslated(-rh[Iw][0],-rh[Iw][1],-rh[Iw][2]);
               RotateR2L(W->CNH);
               RotateR2L(W->CWN);
               glCallList(Geom[W->GeomTag].OpaqueListTag);
               glPopMatrix();
               glUseProgram(0);
            }
         }
      }

      glPointSize(1.0);
      glColor4fv(Black);
      glClear(GL_DEPTH_BUFFER_BIT);

      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      gluPerspective(POV.Angle,POV.AR,POV.Near,POV.Far);
      glMatrixMode(GL_MODELVIEW);

/* .. Draw circle and label for planets */
      if (CamShow[ASTRO]) {
         glDisable(GL_LIGHTING);
         glColor4fv(WorldColor);
         for(Iw=SOL;Iw<=LUNA;Iw++) {
            if(World[Iw].Exists) {
               if (World[Iw].Visibility == WORLD_IS_TOO_SMALL_TO_SEE ||
                   World[Iw].Visibility == WORLD_IS_POINT_SIZED ||
                   Iw == LUNA) {
                  CopyUnitV(rh[Iw],r);
                  for(i=0;i<3;i++) r[i] *= SkyDistance;
                  glRasterPos3d(-r[0],-r[1],-r[2]);
                  glBitmap(16,16,8.0,8.0,10,-8,WorldGlyph);
                  DrawString8x11(World[Iw].Name);
               }
            }
         }
         glColor4fv(Black);
         glEnable(GL_LIGHTING);
      }

      glLoadIdentity();
      RotateL2R(POV.CN);

/* .. Draw Sky Grids */
      if (CamShow[N_GRID]) {
         DrawSkyGrid(NBrightColor,NDimColor,Identity,
                     MajSkyGridList,MinSkyGridList);
      }
      if (CamShow[L_GRID]) {
         MxM(CLpermute,Orb[POV.Host.RefOrb].CLN,C);
         DrawSkyGrid(LBrightColor,LDimColor,C,
                     MajSkyGridList,MinSkyGridList);
      }
      if (CamShow[F_GRID]) {
         DrawSkyGrid(FBrightColor,FDimColor,Frm[POV.Host.RefOrb].CN,
                     MajSkyGridList,MinSkyGridList);
      }
      if (CamShow[B_GRID]) {
         DrawSkyGrid(BBrightColor,BDimColor,SC[POV.Host.SC].B[0].CN,
                     MajSkyGridList,MinSkyGridList);
      }
      if (CamShow[G_GRID]) {
         MxMT(CGH,World[POV.Host.World].CNH,C);
         DrawSkyGrid(GBrightColor,GDimColor,C,
                     MajSkyGridList,MinSkyGridList);
      }

/* .. Draw TDRS */
      if (CamShow[TDRS] && POV.Host.World == EARTH) {
         glDisable(GL_LIGHTING);
         glColor4fv(TdrsColor);
         for(i=0;i<10;i++) {
            if (Tdrs[i].Exists) {
               for(j=0;j<3;j++) PosN[j] = Tdrs[i].PosN[j] - POV.PosN[j];
               UNITV(PosN);
               for(j=0;j<3;j++) PosN[j] *= SkyDistance;
               glRasterPos3d(PosN[0],PosN[1],PosN[2]);
               glBitmap(16,16,8.0,8.0,10,-8,TdrsGlyph);
               DrawString8x11(Tdrs[i].Designation);
            }
         }
         glEnable(GL_LIGHTING);
      }

/* .. Draw SC not in POV.Host.RefOrb */
      glDisable(GL_LIGHTING);
      for(Isc=0;Isc<Nsc;Isc++) {
         S = &SC[Isc];
         //if (S->RefOrb != POV.Host.RefOrb) {
         if (!ScIsVisible(POV.Host.RefOrb,Isc,PosR)) {
            glColor4fv(ScColor);
            for(i=0;i<3;i++) PosH[i] = S->PosH[i] - POV.PosH[i];
            MxV(World[POV.Host.World].CNH,PosH,PosN);
            UNITV(PosN);
            for(i=0;i<3;i++) PosN[i] *= SkyDistance;
            glRasterPos3d(PosN[0],PosN[1],PosN[2]);
            glBitmap(16,16,8.0,8.0,10,-8,ScGlyph);
            DrawString8x11(S->Label);
         }
      }
      glEnable(GL_LIGHTING);
}
/**********************************************************************/
void DrawProxOps(void)
{
      struct SCType *S;
      struct OrbitType *O;
      GLfloat PlaneColor[4] = {1.0,1.0,1.0,0.05};
      GLfloat GridColor[4] = {1.0,1.0,1.0,0.16};
      GLfloat AxisColor[4] = {1.0,1.0,1.0,0.5};
      GLfloat OrbitColor[4] = {1.0,0.8,0.5,0.25};
      GLfloat Black[4] = {0.0,0.0,0.0,1.0};
      double GridSpacing[20] = {1.0,2.0,4.0,8.0,
                              10.0,20.0,40.0,80.0,
                              100.0,200.0,400.0,800.0,
                              1000.0,2000.0,4000.0,8000.0,
                              10000.0,20000.0,40000.0,80000.0};
      long Isc,i;
      double A,Bc,Bs,C,Dc,Ds;
      double t,r[3],v[3];
      double R,dr,AxisLength;

      glDisable(GL_LIGHTING);
      glDepthMask(GL_FALSE);

      O = &Orb[POV.Host.RefOrb];
      glPushMatrix();
      RotateR2L(O->CLN);

/* .. Draw Orbit Traces */
      glLineWidth(4.0);
      glPointSize(4.0);

      for(Isc=0;Isc<Nsc;Isc++) {
         if (SC[Isc].Exists && SC[Isc].RefOrb == POV.Host.RefOrb) {
            S = &SC[Isc];
            EHRV2EHModes(S->PosEH,S->VelEH,O->MeanMotion,0.0,
                         &A,&Bc,&Bs,&C,&Dc,&Ds);

            /* Orbit */
            glColor4fv(OrbitColor);
            glBegin(GL_LINE_STRIP);
               for(t=-3600.0;t<=3600.0;t+=60.0) {
                  EHModes2EHRV(A,Bc,Bs,C,Dc,Ds,O->MeanMotion,
                     O->MeanMotion*t,r,v);
                  glVertex3dv(r);
               }
            glEnd();

            /* Knots */
            #if 0
            for(t=-3600.0;t<=3600.0;t+=600.0) {
               if (t<0.0) {
                  KnotColor[0] = 1.0;
                  KnotColor[1] = 0.5+0.5*(3600.0+t)/3600.0;
                  KnotColor[2] = KnotColor[1];
               }
               else {
                  KnotColor[0] = 0.5+0.5*(3600.0-t)/3600.0;
                  KnotColor[1] = KnotColor[0];
                  KnotColor[2] = 1.0;
               }
               glColor4fv(KnotColor);
               EHModes2EHRV(A,Bc,Bs,C,Dc,Ds,O->MeanMotion,
                  O->MeanMotion*t,r,v);
               glBegin(GL_POINTS);
                  glVertex3dv(r);
               glEnd();
            }
            #endif
         }
      }
      glLineWidth(1.0);
      glPointSize(1.0);

      i = 0;
      while(POV.Range/5.0 > GridSpacing[i] && i<19) i++;
      POV.GridSpacing = GridSpacing[i];
      R = 5.0*POV.GridSpacing;

/* .. Draw Euler-Hill Planes */
      glColor4fv(PlaneColor);
      glBegin(GL_QUADS);
         /* X-Y */
         glVertex3d( R, R,0.0);
         glVertex3d(-R, R,0.0);
         glVertex3d(-R,-R,0.0);
         glVertex3d( R,-R,0.0);
         glVertex3d( R, R,0.0);
         glVertex3d( R,-R,0.0);
         glVertex3d(-R,-R,0.0);
         glVertex3d(-R, R,0.0);
         /* X-Z */
         glVertex3d( R,0.0, R);
         glVertex3d(-R,0.0, R);
         glVertex3d(-R,0.0,-R);
         glVertex3d( R,0.0,-R);
         glVertex3d( R,0.0, R);
         glVertex3d( R,0.0,-R);
         glVertex3d(-R,0.0,-R);
         glVertex3d(-R,0.0, R);
         /* Y-Z */
         glVertex3d(0.0, R, R);
         glVertex3d(0.0,-R, R);
         glVertex3d(0.0,-R,-R);
         glVertex3d(0.0, R,-R);
         glVertex3d(0.0, R, R);
         glVertex3d(0.0, R,-R);
         glVertex3d(0.0,-R,-R);
         glVertex3d(0.0,-R, R);
      glEnd();

/* .. Draw Euler-Hill Grids */
      glColor4fv(GridColor);
      glBegin(GL_LINES);
         for(i=0;i<6;i++) {
            dr = i*POV.GridSpacing;
            /* X */
            glVertex3d( dr, R,0.0);
            glVertex3d( dr,-R,0.0);
            glVertex3d( dr,0.0, R);
            glVertex3d( dr,0.0,-R);
            glVertex3d(-dr, R,0.0);
            glVertex3d(-dr,-R,0.0);
            glVertex3d(-dr,0.0, R);
            glVertex3d(-dr,0.0,-R);
            /* Y */
            glVertex3d( R, dr,0.0);
            glVertex3d(-R, dr,0.0);
            glVertex3d(0.0, dr, R);
            glVertex3d(0.0, dr,-R);
            glVertex3d( R,-dr,0.0);
            glVertex3d(-R,-dr,0.0);
            glVertex3d(0.0,-dr, R);
            glVertex3d(0.0,-dr,-R);
            /* Z */
            glVertex3d( R,0.0, dr);
            glVertex3d(-R,0.0, dr);
            glVertex3d(0.0, R, dr);
            glVertex3d(0.0,-R, dr);
            glVertex3d( R,0.0,-dr);
            glVertex3d(-R,0.0,-dr);
            glVertex3d(0.0, R,-dr);
            glVertex3d(0.0,-R,-dr);
         }
      glEnd();

/* .. Draw L axes */
      AxisLength = 0.4*POV.Height*POV.Range;
      DrawAxisLabels(GLYPH_L,AxisColor,
                     0.0,AxisLength,
                     0.0,AxisLength,
                     0.0,AxisLength);


      glPopMatrix();
      glColor4fv(Black);
      glEnable(GL_LIGHTING);
      glDepthMask(GL_TRUE);
}
/**********************************************************************/
void DrawNearAuxObjects(void)
{
      double AxisLength,r[3],len[3];
      long Isc,Ib,Ithr,i,Ig;
      struct SCType *S;
      struct BodyType *B;
      struct GeomType *G;
      struct FovType *F;
      float SvbColor[4] = {1.0,1.0,0.0,1.0};
      float BvbColor[4] = {1.0,0.0,0.5,1.0};
      float HvbColor[4] = {0.5,0.5,1.0,1.0};
      double PosR[3];

/* .. Draw near-field Auxiliary Objects */
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      gluPerspective(POV.Angle, POV.AR, POV.Near, POV.Far);
      glMatrixMode(GL_MODELVIEW);
      glLightModelfv(GL_LIGHT_MODEL_AMBIENT,LocalAmbientLightColor);
      glLightfv(GL_LIGHT0,GL_DIFFUSE,LocalDiffuseLightColor);

/* .. Reset to POV frame */
      glLoadIdentity();

/* .. Set to N frame */
      RotateL2R(POV.CN);

      glLightfv(GL_LIGHT0,GL_POSITION,LightPosN);

      /* Translate from POV to R */
      glTranslated(-POV.PosR[0],-POV.PosR[1],-POV.PosR[2]);
      AxisLength = 0.4*POV.Height*POV.Range;

      if (CamShow[PROX_OPS]) DrawProxOps();

/* .. Draw Formation Origin and Axes */
      if (CamShow[F_AXES]) {
         glPushMatrix();
         glTranslated(Frm[POV.Host.RefOrb].PosR[0],
                      Frm[POV.Host.RefOrb].PosR[1],
                      Frm[POV.Host.RefOrb].PosR[2]);
         RotateR2L(Frm[POV.Host.RefOrb].CN);
         DrawAxisLabels(GLYPH_F,FBrightColor,
                        0.0,AxisLength,
                        0.0,AxisLength,
                        0.0,AxisLength);
         glPopMatrix();
      }

/* .. Draw Axes fixed in S/C */
      for(Isc=0;Isc<Nsc;Isc++) {
         S = &SC[Isc];
         if (S->Exists) {
            glPushMatrix();

            if (ScIsVisible(POV.Host.RefOrb,Isc,PosR) ) {
               glTranslated(PosR[0],PosR[1],PosR[2]);
               if (CamShow[B_AXES]) {
                  for(Ib=0;Ib<S->Nb;Ib++) {
                     B = &S->B[Ib];
                     G = &Geom[B->GeomTag];
                     glPushMatrix();
                     glTranslated(B->pn[0],B->pn[1],B->pn[2]);
                     RotateR2L(B->CN);
                     for(i=0;i<3;i++) {
                        r[i] = G->BBox.max[i]-B->cm[i];
                        len[i] = 1.5*r[i];
                        if (len[i] < 0.5) len[i] = 0.5;
                        if (len[i] > r[i]+1.0) len[i] = r[i]+1.0;
                     }
                     DrawAxisLabels(GLYPH_B,BBrightColor,
                        0.0,len[0],0.0,len[1],0.0,len[2]);
                     DrawBodyLabel(Ib,BBrightColor,G->BBox.max);

                     /* Draw dots at joints */
                     glDisable(GL_LIGHTING);
                     for(Ig=0;Ig<S->Ng;Ig++) {
                        if (Ib == S->G[Ig].Bin) {
                           glPointSize(20.0);
                           glColor4f(0.0,0.0,1.0,0.5);
                           glBegin(GL_POINTS);
                              glVertex3dv(S->G[Ig].ri);
                           glEnd();
                        }
                        if (Ib == S->G[Ig].Bout) {
                           glPointSize(10.0);
                           glColor4f(1.0,0.0,0.0,0.5);
                           glBegin(GL_POINTS);
                              glVertex3dv(S->G[Ig].ro);
                           glEnd();
                        }
                     }
                     glColor4f(0.0,0.0,0.0,1.0);
                     glPointSize(1.0);
                     glEnable(GL_LIGHTING);

                     glPopMatrix();
                  }
               }
               if (Isc == POV.Host.SC) {
                  if (CamShow[N_AXES]) {
                     DrawAxisLabels(GLYPH_N,NBrightColor,
                                    0.0,AxisLength,
                                    0.0,AxisLength,
                                    0.0,AxisLength);
                  }
                  if (CamShow[L_AXES]) {
                     glPushMatrix();
                     RotateR2L(SC[Isc].CLN);
                     DrawAxisLabels(GLYPH_L,LBrightColor,
                                    0.0,AxisLength,
                                    0.0,AxisLength,
                                    0.0,AxisLength);
                     glPopMatrix();
                  }
                  if (CamShow[ASTRO])
                     DrawPlanetLabels(AxisLength);
                  if (CamShow[TRUTH_VECTORS]) {
                     B = &S->B[0];
                     glPushMatrix();
                     glTranslated(B->pn[0],B->pn[1],B->pn[2]);
                     RotateR2L(B->CN);
                     DrawVector(S->svb,"s"," ",SvbColor,AxisLength,
                        1.0,TRUE);
                     if (MAGV(S->bvb) > 0.0)
                        DrawVector(S->bvb,"B","uT",BvbColor,AxisLength,
                           1.0E6,FALSE);
                     if (MAGV(S->Hvb) > 0.0)
                        DrawVector(S->Hvb,"H","Nms",HvbColor,AxisLength,
                           1.0,FALSE);
                     glPopMatrix();
                  }
                  if (CamShow[FSW_VECTORS]) {
                     B = &S->B[0];
                     glPushMatrix();
                     glTranslated(B->pn[0],B->pn[1],B->pn[2]);
                     RotateR2L(B->CN);
                     if (MAGV(S->AC.svb) > 0.0)
                        DrawVector(S->AC.svb,"Sac"," ",SvbColor,
                           1.15*AxisLength,1.0,TRUE);
                     if (MAGV(S->AC.bvb) > 0.0)
                        DrawVector(S->AC.bvb,"Bac","uT",BvbColor,
                           1.15*AxisLength,1.0E6,FALSE);
                     if (MAGV(S->AC.Hvb) > 0.0)
                        DrawVector(S->AC.Hvb,"Hac","Nms",HvbColor,
                           1.15*AxisLength,
                           1.0,FALSE);
                     glPopMatrix();
                  }
               }
            }

            glPopMatrix();
         }
      }

/* .. Draw Thruster Plumes */
      for(Isc=0;Isc<Nsc;Isc++) {
         S = &SC[Isc];
         if (S->Exists) {
            glPushMatrix();

//            if (S->RefOrb == POV.Host.RefOrb) { /* TODO: Improve this */
//               glTranslated(S->PosR[0],S->PosR[1],S->PosR[2]);
            if (ScIsVisible(POV.Host.RefOrb,Isc,PosR)) {
               glTranslated(PosR[0],PosR[1],PosR[2]);
               for(Ithr=0;Ithr<S->Nthr;Ithr++) {
                  B = &S->B[S->Thr[Ithr].Body];
                  glPushMatrix();
                  glTranslated(B->pn[0],B->pn[1],B->pn[2]);
                  RotateR2L(B->CN);
                  glTranslated(-B->cm[0],-B->cm[1],-B->cm[2]);
                  DrawThrusterPlume(&S->Thr[Ithr]);
                  glPopMatrix();
               }
            }

            glPopMatrix();
         }
      }


/* .. Translucent FOVs are drawn last to get blending right */
      if (CamShow[CAM_FOV]) {
         glDisable(GL_LIGHTING);
         glDepthMask(GL_FALSE);
         for(i=0;i<Nfov;i++) {
            F = &FOV[i];
            if (F->RefOrb == POV.Host.RefOrb) {
               if (SC[F->SC].Exists) {
                  S = &SC[F->SC];
                  B = &S->B[F->Body];
                  if (F->NearExists) {
                     glPushMatrix();
                     glTranslated(S->PosR[0]+B->pn[0],
                                  S->PosR[1]+B->pn[1],
                                  S->PosR[2]+B->pn[2]);
                     RotateR2L(B->CN);
                     glTranslated(F->pb[0]-B->cm[0],
                                  F->pb[1]-B->cm[1],
                                  F->pb[2]-B->cm[2]);
                     RotateR2L(F->CB);
                     DrawNearFOV(F->Nv,F->Width,F->Height,F->Length,
                        F->BoreAxis,F->H_Axis,F->V_Axis,
                        F->Type,F->Color);
                     glPopMatrix();
                  }
                  if (F->FarExists) {
                     glPushMatrix();
                     glLoadIdentity();
                     RotateL2R(POV.CN);
                     RotateR2L(B->CN);
                     RotateR2L(F->CB);
                     DrawFarFOV(F->Nv,F->Width,F->Height,
                        F->BoreAxis,F->H_Axis,F->V_Axis,
                        F->Type,F->Color,F->Label,SkyDistance);
                     glPopMatrix();
                  }
               }
            }
         }
         glDepthMask(GL_TRUE);
         glEnable(GL_LIGHTING);
      }

/* .. Draw HUD in POV frame */
      if (ShowHUD) DrawCamHUD();
      if (strlen(Banner) > 1) DrawBanner(CamWidth,CamHeight);
      if (ShowWatermark) DrawWatermarks();
}
/**********************************************************************/
void DrawContactSpheres(void)
{
      struct SCType *S;
      struct BodyType *B;
      struct GeomType *G;
      struct PolyType *P;
      long Isc,Ib,Ip;
      static long First = 1;
      static GLUquadric *Sphere;
      GLfloat White[4] = {1.0,1.0,1.0,1.0};
      GLfloat Black[4] = {0.0,0.0,0.0,1.0};

      if (First) {
         First = 0;
         Sphere = gluNewQuadric();
      }

      glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,White);
      glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,White);
      glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,White);
      glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Black);
      glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,10.0);

      for(Isc=0;Isc<Nsc;Isc++) {
         S = &SC[Isc];
         if (S->RefOrb == POV.Host.RefOrb) {
            for(Ib=0;Ib<S->Nb;Ib++) {
               B = &S->B[Ib];
               G = &Geom[B->GeomTag];
               glPushMatrix();
               glMultMatrixf(B->ModelMatrix);
               for(Ip=0;Ip<G->Npoly;Ip++) {
                  P = &G->Poly[Ip];
                  glPushMatrix();
                  glTranslated(P->Centroid[0],P->Centroid[1],P->Centroid[2]);
                  glScaled(P->radius,P->radius,P->radius);
                  gluSphere(Sphere,1.0,8,8);
                  glPopMatrix();
               }
               glPopMatrix();
            }
         }
      }

      glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,Black);
      glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,Black);
      glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,Black);
}
/**********************************************************************/
void DepthPass(void)
{
      /* Bias and Scale, in Column Major order */
      GLfloat BS[16] = {0.5,0.0,0.0,0.0 , 0.0,0.5,0.0,0.0 ,
                        0.0,0.0,0.5,0.0 , 0.5,0.5,0.5,1.0 };
      GLfloat MPN[16];
      double CLN[3][3],rb[3],rn[3],rl[3],r;
      struct ShadowFBOType *SM;
      struct SCType *S;
      struct BodyType *B;
      struct GeomType *G;
      struct BoundingBoxType *BB, LB;
      long Isc,Ib,i;
      double PosR[3];

      SM = &ShadowMap;

/* .. Find LightBox extent */
      for(i=0;i<3;i++) {
         LB.min[i] =  1.0E9;
         LB.max[i] = -1.0E9;
      }

      for(i=0;i<3;i++) CLN[2][i] = LightPosN[i];
      UNITV(CLN[2]);
      PerpBasis(CLN[2],CLN[0],CLN[1]);
      for(Isc=0;Isc<Nsc;Isc++) {
         S = &SC[Isc];
         //if (S->RefOrb == POV.Host.RefOrb) { /* TODO:  Improve this */
         if (ScIsVisible(POV.Host.RefOrb,Isc,PosR) ) {
            for(Ib=0;Ib<S->Nb;Ib++) {
               B = &S->B[Ib];
               G = &Geom[B->GeomTag];
               BB = &G->BBox;
               for(i=0;i<3;i++) rb[i] = BB->center[i] - B->cm[i];
               MxV(B->CN,rb,rn);
               for(i=0;i<3;i++) rn[i] += B->pn[i]+ S->PosR[i];
               MxV(CLN,rn,rl);
               for(i=0;i<3;i++) {
                  r = rl[i] + BB->radius;
                  if (r > LB.max[i]) LB.max[i] = r;
                  r = rl[i] - BB->radius;
                  if (r < LB.min[i]) LB.min[i] = r;
               }
            }
         }
      }
      /* Extend LightBox 50 m */
      LB.min[2] -= 50.0;
      /* Find center */
      for(i=0;i<3;i++) {
         LB.center[i] = 0.5*(LB.max[i]+LB.min[i]);
         rb[i] = LB.max[i]-LB.center[i];
      }
      LB.radius = MAGV(rb);

/* .. Draw into ShadowMap */

      glBindFramebuffer(GL_FRAMEBUFFER,SM->FrameTag);
      glBindRenderbuffer(GL_RENDERBUFFER,SM->RenderTag);
      glViewport(0,0,SM->Width,SM->Height);
      glClear(GL_DEPTH_BUFFER_BIT);

      glMatrixMode(GL_PROJECTION);
      glPushMatrix();
      glLoadIdentity();
      glOrtho(LB.min[0]-LB.center[0],
              LB.max[0]-LB.center[0],
              LB.min[1]-LB.center[1],
              LB.max[1]-LB.center[1],
              LB.min[2]-LB.center[2],
              LB.max[2]-LB.center[2]);
      glTranslated(-LB.center[0],-LB.center[1],-LB.center[2]);
      RotateL2R(CLN);
      glGetFloatv(GL_PROJECTION_MATRIX,MPN);
      MxM4f(BS,MPN,ShadowFromNMatrix);
      glMatrixMode(GL_MODELVIEW);

         glShadeModel(GL_FLAT);
         glDisable(GL_LIGHTING);
         glDisable(GL_NORMALIZE);
         glColorMask(0,0,0,0);
         glCullFace(GL_FRONT);

         glPushMatrix();
            glLoadIdentity();
            for(Isc=0;Isc<Nsc;Isc++) {
               S = &SC[Isc];
//               if (S->RefOrb == POV.Host.RefOrb) { /* TODO: Improve this */
               if (ScIsVisible(POV.Host.RefOrb,Isc,PosR) ) {
                  for(Ib=0;Ib<S->Nb;Ib++) {
                     B = &S->B[Ib];
                     G = &Geom[B->GeomTag];
                     glPushMatrix();
                        glMultMatrixf(B->ModelMatrix);
                        glCallList(G->DepthListTag);
                     glPopMatrix();
                  }
               }
            }
         glPopMatrix();

         glCullFace(GL_BACK);
         glColorMask(1,1,1,1);
         glEnable(GL_NORMALIZE);
         glEnable(GL_LIGHTING);
         glShadeModel(GL_SMOOTH);

      glMatrixMode(GL_PROJECTION);
      glPopMatrix();
      glMatrixMode(GL_MODELVIEW);

      glBindFramebuffer(GL_FRAMEBUFFER,0);
      glBindRenderbuffer(GL_RENDERBUFFER,0);
      glViewport(0,0,CamWidth,CamHeight);
}
/**********************************************************************/
void OpaquePass(void)
{
      long Isc,Ib;
      struct SCType *S;
      struct BodyType *B;
      struct RegionType *R;
      struct GeomType *G;
      struct ShadowFBOType *SM;
      long Ir;
      double PosR[3];

      SM = &ShadowMap;

      glUseProgram(BodyShaderProgram);
      glUniform1i(ShadowsEnabledLoc,ShadowsEnabled);
      glUniformMatrix3fv(CNELoc,1,0,CNE);

      if (ShadowsEnabled) {
         glActiveTexture(GL_TEXTURE5);
         glEnable(GL_TEXTURE_2D);
         glBindTexture(GL_TEXTURE_2D,SM->TexTag);
         glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_COMPARE_MODE,
               GL_COMPARE_R_TO_TEXTURE);
      }

      for(Ir=0;Ir<Nrgn;Ir++) {
         R = &Rgn[Ir];
         if (R->Exists && R->World == POV.Host.World) {
            glPushMatrix();
            glMultMatrixf(R->ModelMatrix);
            if (ShadowsEnabled) {
               MxM4f(ShadowFromNMatrix,R->ModelMatrix,ShadowMatrix);
               glUniformMatrix4fv(ShadowMatrixLoc,1,0,ShadowMatrix);
            }
            glCallList(Geom[R->GeomTag].OpaqueListTag);
            glPopMatrix();
         }
      }

      for(Isc=0;Isc<Nsc;Isc++) {
         S = &SC[Isc];
         if (ScIsVisible(POV.Host.RefOrb,Isc,PosR)) {
            for(Ib=0;Ib<S->Nb;Ib++) {
               B = &S->B[Ib];
               G = &Geom[B->GeomTag];
               glPushMatrix();
               glMultMatrixf(B->ModelMatrix);
               if (ShadowsEnabled) {
                  MxM4f(ShadowFromNMatrix,B->ModelMatrix,ShadowMatrix);
                  glUniformMatrix4fv(ShadowMatrixLoc,1,0,ShadowMatrix);
               }
               glCallList(G->OpaqueListTag);
               glPopMatrix();
            }
         }
      }
}
/**********************************************************************/
void SeeThruPass(void)
{
      long Isc,Ib,Ir;
      struct SCType *S;
      struct BodyType *B;
      struct GeomType *G;
      struct RegionType *R;
      struct ShadowFBOType *SM;
      double PosR[3];

      SM = &ShadowMap;

      glUseProgram(BodyShaderProgram);
      glUniform1i(ShadowsEnabledLoc,ShadowsEnabled);
      glUniformMatrix3fv(CNELoc,1,0,CNE);

      if (ShadowsEnabled) {
         glActiveTexture(GL_TEXTURE5);
         glEnable(GL_TEXTURE_2D);
         glBindTexture(GL_TEXTURE_2D,SM->TexTag);
         glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_COMPARE_MODE,
               GL_COMPARE_R_TO_TEXTURE);
      }

      for(Ir=0;Ir<Nrgn;Ir++) {
         R = &Rgn[Ir];
         if (R->Exists && R->World == POV.Host.World) {
            glPushMatrix();
            glMultMatrixf(R->ModelMatrix);
            if (ShadowsEnabled) {
               MxM4f(ShadowFromNMatrix,R->ModelMatrix,ShadowMatrix);
               glUniformMatrix4fv(ShadowMatrixLoc,1,0,ShadowMatrix);
            }
            glCallList(Geom[R->GeomTag].SeeThruListTag);
            glPopMatrix();
         }
      }

      for(Isc=0;Isc<Nsc;Isc++) {
         S = &SC[Isc];
         //if (S->RefOrb == POV.Host.RefOrb) {
         if (ScIsVisible(POV.Host.RefOrb,Isc,PosR)) {
            for(Ib=0;Ib<S->Nb;Ib++) {
               B = &S->B[Ib];
               G = &Geom[B->GeomTag];
               glPushMatrix();
               glMultMatrixf(B->ModelMatrix);
               glCallList(G->SeeThruListTag);
               glPopMatrix();
            }
         }
      }
}
/**********************************************************************/
void DrawBodies(void)
{
      double CNH[3][3],PosN[3];
      GLfloat Black[4] = {0.0,0.0,0.0,1.0};

/* .. Light Source */
      /* Sun */
      MTxM(POV.CN,POV.CH,CNH);
      MxV(CNH,POV.PosH,PosN);
      UNITV(PosN);
      LightPosN[0] = (GLfloat) -PosN[0];
      LightPosN[1] = (GLfloat) -PosN[1];
      LightPosN[2] = (GLfloat) -PosN[2];
      LightPosN[3] = 0.0;
      glLightfv(GL_LIGHT0,GL_POSITION,LightPosN);
      glLightfv(GL_LIGHT0,GL_DIFFUSE,LocalDiffuseLightColor);
      glLightModelfv(GL_LIGHT_MODEL_AMBIENT,LocalAmbientLightColor);

/* .. Depth Pass */
      if (ShadowsEnabled) {
         DepthPass();
      }

/* .. Opaque Pass */
      OpaquePass();

/* .. SeeThru Pass */
      if (SeeThruPassNeeded) SeeThruPass();

/* .. Tidy Up */
      glUseProgram(0);
      glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE,Black);
      glMaterialfv(GL_FRONT,GL_SPECULAR,Black);
      glMaterialfv(GL_FRONT,GL_EMISSION,Black);
}
/**********************************************************************/
void BufferToScreen()
{
}
/**********************************************************************/
void SetPovOrientation(void)
{
      double Axis[6][3] = {{ 1.0,0.0,0.0},{0.0, 1.0,0.0},{0.0,0.0, 1.0},
                           {-1.0,0.0,0.0},{0.0,-1.0,0.0},{0.0,0.0,-1.0}};
      double Qfixed[10][4] = {{0.0,-1.0,0.0,1.0}, /* Down */
                              {0.0,0.0,-0.9239,0.3827}, /* Rear Left */
                              {0.0,0.0,1.0,0.0}, /* Rear */
                              {0.0,0.0,0.9239,0.3827}, /* Rear Right */
                              {0.0,0.0,-1.0,1.0}, /* Left */
                              {0.0,1.0,0.0,1.0}, /* Up */
                              {0.0,0.0,1.0,1.0}, /* Right */
                              {0.0,0.0,-0.3827,0.9239}, /* Front Left */
                              {0.0,0.0,0.0,1.0}, /* Front */
                              {0.0,0.0,0.3827,0.9239} /* Front Right */};
      double Qpermute[4] = {0.5,0.5,-0.5,-0.5};
      long i;

      if (POV.Mode == TRACK_HOST) {
         for(i=0;i<3;i++) {
            POV.C[2][i] =  -Axis[POV.BoreAxis][i];
            POV.C[1][i] =   Axis[POV.UpAxis][i];
         }
         VxV(POV.C[1],POV.C[2],POV.C[0]);
         C2Q(POV.C,POV.q);
      }
      else if (POV.Mode == TRACK_TARGET) {
         for(i=0;i<3;i++) {
            POV.C[2][i] =  -Axis[POS_X][i];
            POV.C[1][i] =   Axis[NEG_Z][i];
         }
         VxV(POV.C[1],POV.C[2],POV.C[0]);
         C2Q(POV.C,POV.q);
      }
      else { /* FIXED_IN_HOST */
         QxQ(Qpermute,Qfixed[POV.View],POV.q);
         Q2C(POV.q,POV.C);
      }
}
/**********************************************************************/
void PovTrackHostMode(void)
{
      double qdot[4],rb[3],rs[3],rf[3],rw[3];
      long i,j,RefOrb,center,PovSC,PovBody;
      struct TargetType *Host;
      double rh[3];

      Host = &POV.Host;

      RefOrb = Host->RefOrb;
      center = Orb[RefOrb].World;
      PovSC = Host->SC;
      PovBody = Host->Body;

      if (POV.Frame == FRAME_N) {
         for(i=0;i<3;i++) {
            for(j=0;j<3;j++) POV.CN[i][j] = POV.C[i][j];
         }
         MxMT(POV.CN,SC[PovSC].CLN,POV.CL);
         MxMT(POV.CN,Frm[RefOrb].CN,POV.CF);
         MxMT(POV.CN,SC[PovSC].B[0].CN,POV.CB);
      }
      else if(POV.Frame == FRAME_L) {
         for(i=0;i<3;i++) {
            for(j=0;j<3;j++) POV.CL[i][j] = POV.C[i][j];
         }
         if (Host->Type == TARGET_SC) {
            MxM(POV.CL,SC[PovSC].CLN,POV.CN);
         }
         else if(Host->Type == TARGET_REFORB) {
            MxM(POV.CL,Orb[RefOrb].CLN,POV.CN);
         }
         MxMT(POV.CN,SC[PovSC].B[0].CN,POV.CB);
         MxMT(POV.CN,Frm[RefOrb].CN,POV.CF);
      }
      else if(POV.Frame == FRAME_F) {
         for(i=0;i<3;i++) {
            for(j=0;j<3;j++) POV.CF[i][j] = POV.C[i][j];
         }
         MxM(POV.CF,Frm[RefOrb].CN,POV.CN);
         MxMT(POV.CN,Orb[RefOrb].CLN,POV.CL);
         MxMT(POV.CN,SC[PovSC].B[0].CN,POV.CB);
      }
      else if(POV.Frame == FRAME_S) {
         for(i=0;i<3;i++) {
            for(j=0;j<3;j++) POV.CB[i][j] = POV.C[i][j];
         }
         MxM(POV.CB,SC[PovSC].B[0].CN,POV.CN);
         MxMT(POV.CN,Orb[RefOrb].CLN,POV.CL);
         MxMT(POV.CN,Frm[RefOrb].CN,POV.CF);
      }
      else if(POV.Frame == FRAME_B) {
         for(i=0;i<3;i++) {
            for(j=0;j<3;j++) POV.CB[i][j] = POV.C[i][j];
         }
         MxM(POV.CB,SC[PovSC].B[PovBody].CN,POV.CN);
         MxMT(POV.CN,Orb[RefOrb].CLN,POV.CL);
         MxMT(POV.CN,Frm[RefOrb].CN,POV.CF);
      }

      if (Host->Type == TARGET_WORLD) {
         for(i=0;i<3;i++) {
            rw[i] = World[Host->World].rad*POV.Range*POV.CN[2][i];
            POV.PosR[i] = rw[i]-Orb[RefOrb].PosN[i];
         }
      }
      else if (Host->Type == TARGET_REFORB) {
         Host->World = center;
         for(i=0;i<3;i++) {
            POV.PosR[i] = POV.Range*POV.CN[2][i];
         }
      }
      else if (Host->Type == TARGET_FRM) {
         Host->World = center;
         for(i=0;i<3;i++) {
            rf[i] = POV.Range*POV.CN[2][i];
            POV.PosR[i] = rf[i] + Frm[RefOrb].PosR[i];
         }
      }
      else if (Host->Type == TARGET_SC) {
         Host->World = center;
         for(i=0;i<3;i++) {
            rs[i] = POV.Range*POV.CN[2][i];
            POV.PosR[i] = rs[i] + SC[PovSC].PosR[i];
         }
      }
      else if (Host->Type == TARGET_BODY) {
         Host->World = center;
         for(i=0;i<3;i++) {
            rb[i] = POV.Range*POV.CN[2][i];
            rs[i] = rb[i] + SC[PovSC].B[PovBody].pn[i];
            POV.PosR[i] = rs[i] + SC[PovSC].PosR[i];
         }
      }

      for(i=0;i<3;i++) POV.PosN[i] = POV.PosR[i] + Orb[RefOrb].PosN[i];
      MxM(POV.CN,World[Host->World].CNH,POV.CH);
      VxM(POV.PosN,World[Host->World].CNH,rh);
      for(i=0;i<3;i++) POV.PosH[i] = World[Host->World].PosH[i]+rh[i];

      QW2QDOT(POV.q,POV.w,qdot);
      for(i=0;i<4;i++) POV.q[i] += qdot[i];
      UNITQ(POV.q);
      Q2C(POV.q,POV.C);
}
/**********************************************************************/
void PovTrackTargetMode(void)
{
      struct TargetType *Host,*Trg;
      double LoS[3],LoSN[3],LoSH[3],pn[3];
      double CosAz,SinAz,CosEl,SinEl,CTH[3][3],CTN[3][3];
      double Identity[3][3] = {{1.0,0.0,0.0},{0.0,1.0,0.0},{0.0,0.0,1.0}};
      double Cos1deg = 0.99985;
      long j,k;

      Host = &POV.Host;
      Trg = &POV.Target;

/* .. Find rr, rn, rh for Host and POV */
      if (Host->Type == TARGET_WORLD) {
         for(j=0;j<3;j++) {
            Host->PosH[j] = World[Host->World].PosH[j];
            for(k=0;k<3;k++) Host->CN[j][k] = Identity[j][k];
            POV.PosH[j] = Host->PosH[j];
         }
      }
      else if (Host->Type == TARGET_REFORB) {
         for(j=0;j<3;j++) {
            Host->PosR[j] = 0.0;
            Host->PosN[j] = Orb[Host->RefOrb].PosN[j];
            for(k=0;k<3;k++)
               Host->CN[j][k] = Orb[Host->RefOrb].CLN[j][k];
         }
         MTxV(World[Host->World].CNH,Host->PosN,Host->PosH);
         for(j=0;j<3;j++) Host->PosH[j] += World[Host->World].PosH[j];
         MTxV(Host->CN,POV.PosB,pn);
         for(j=0;j<3;j++) {
            POV.PosR[j] = Host->PosR[j] + pn[j];
            POV.PosN[j] = POV.PosR[j] + Host->PosN[j];
            for(k=0;k<3;k++) POV.CN[j][k] = Host->CN[j][k];
         }
         MxM(POV.CN,World[Host->World].CNH,POV.CH);
         VxM(POV.PosN,World[Host->World].CNH,POV.PosH);
         for(j=0;j<3;j++) POV.PosH[j] += World[Host->World].PosH[j];
      }
      else if (Host->Type == TARGET_FRM) {
         for(j=0;j<3;j++) {
            Host->PosR[j] = Frm[Host->RefOrb].PosR[j];
            Host->PosN[j] = Host->PosR[j] + Orb[Host->RefOrb].PosN[j];
            for(k=0;k<3;k++)
               Host->CN[j][k] = Frm[Host->RefOrb].CN[j][k];
         }
         MTxV(World[Host->World].CNH,Host->PosN,Host->PosH);
         for(j=0;j<3;j++) Host->PosH[j] += World[Host->World].PosH[j];
         MTxV(Host->CN,POV.PosB,pn);
         for(j=0;j<3;j++) {
            POV.PosR[j] = Host->PosR[j] + pn[j];
            POV.PosN[j] = POV.PosR[j] + Orb[Host->RefOrb].PosN[j];
            for(k=0;k<3;k++) POV.CN[j][k] = Host->CN[j][k];
         }
         MxM(POV.CN,World[Host->World].CNH,POV.CH);
         VxM(POV.PosN,World[Host->World].CNH,POV.PosH);
         for(j=0;j<3;j++) POV.PosH[j] += World[Host->World].PosH[j];
      }
      else if (Host->Type == TARGET_SC) {
         for(j=0;j<3;j++) {
            Host->PosR[j] = SC[Host->SC].PosR[j];
            Host->PosN[j] = Host->PosR[j] + Orb[Host->RefOrb].PosN[j];
            for(k=0;k<3;k++)
               Host->CN[j][k] = SC[Host->SC].B[0].CN[j][k];
         }
         MTxV(World[Host->World].CNH,Host->PosN,Host->PosH);
         for(j=0;j<3;j++) Host->PosH[j] += World[Host->World].PosH[j];
         MTxV(Host->CN,POV.PosB,pn);
         for(j=0;j<3;j++) {
            POV.PosR[j] = Host->PosR[j] + pn[j];
            POV.PosN[j] = POV.PosR[j] + Orb[Host->RefOrb].PosN[j];
            for(k=0;k<3;k++) POV.CN[j][k] = Host->CN[j][k];
         }
         MxM(POV.CN,World[Host->World].CNH,POV.CH);
         VxM(POV.PosN,World[Host->World].CNH,POV.PosH);
         for(j=0;j<3;j++) POV.PosH[j] += World[Host->World].PosH[j];
      }
      else {
         for(j=0;j<3;j++) {
            Host->PosR[j] = SC[Host->SC].B[Host->Body].pn[j]
               + SC[Host->SC].PosR[j];
            Host->PosN[j] = Host->PosR[j] + Orb[Host->RefOrb].PosN[j];
            for(k=0;k<3;k++)
               Host->CN[j][k] = SC[Host->SC].B[Host->Body].CN[j][k];
         }
         MTxV(World[Host->World].CNH,Host->PosN,Host->PosH);
         for(j=0;j<3;j++) Host->PosH[j] += World[Host->World].PosH[j];
         MTxV(Host->CN,POV.PosB,pn);
         for(j=0;j<3;j++) {
            POV.PosR[j] = Host->PosR[j] + pn[j];
            POV.PosN[j] = POV.PosR[j] + Orb[Host->RefOrb].PosN[j];
            for(k=0;k<3;k++) POV.CN[j][k] = Host->CN[j][k];
         }
         MxM(POV.CN,World[Host->World].CNH,POV.CH);
         VxM(POV.PosN,World[Host->World].CNH,POV.PosH);
         for(j=0;j<3;j++) POV.PosH[j] += World[Host->World].PosH[j];
      }

/* .. Find rr, rn, rh for Target */
      if (Trg->Type == TARGET_WORLD) {
         for(j=0;j<3;j++) Trg->PosH[j] = World[Trg->World].PosH[j];
      }
      else if (Trg->Type == TARGET_REFORB) {
         for(j=0;j<3;j++) {
            Trg->PosR[j] = 0.0;
            Trg->PosN[j] = Orb[Trg->RefOrb].PosN[j];
         }
         MTxV(World[Trg->World].CNH,Trg->PosN,Trg->PosH);
         for(j=0;j<3;j++) Trg->PosH[j] += World[Trg->World].PosH[j];
      }
      else if (Trg->Type == TARGET_FRM) {
         for(j=0;j<3;j++) {
            Trg->PosR[j] = Frm[Trg->RefOrb].PosR[j];
            Trg->PosN[j] = Trg->PosR[j] + Orb[Trg->RefOrb].PosN[j];
         }
         MTxV(World[Trg->World].CNH,Trg->PosN,Trg->PosH);
         for(j=0;j<3;j++) Trg->PosH[j] += World[Trg->World].PosH[j];
      }
      else if (Trg->Type == TARGET_SC) {
         for(j=0;j<3;j++) {
            Trg->PosR[j] = SC[Trg->SC].PosR[j];
            Trg->PosN[j] = Trg->PosR[j] + Orb[Trg->RefOrb].PosN[j];
         }
         MTxV(World[Trg->World].CNH,Trg->PosN,Trg->PosH);
         for(j=0;j<3;j++) Trg->PosH[j] += World[Trg->World].PosH[j];
      }
      else {
         for(j=0;j<3;j++) {
            Trg->PosR[j] = SC[Trg->SC].B[Trg->Body].pn[j]
               + SC[Trg->SC].PosR[j];
            Trg->PosN[j] = Trg->PosR[j] + Orb[Trg->RefOrb].PosN[j];
         }
         MTxV(World[Trg->World].CNH,Trg->PosN,Trg->PosH);
         for(j=0;j<3;j++) Trg->PosH[j] += World[Trg->World].PosH[j];
      }

/* .. Find pointing vector */
      if (Trg->RefOrb == Host->RefOrb) {
         for(j=0;j<3;j++) LoSN[j] = Trg->PosR[j] - POV.PosR[j];
      }
      else if (Trg->World == Host->World) {
         for(j=0;j<3;j++) LoSN[j] = Trg->PosN[j] - POV.PosN[j];
      }
      else {
         for(j=0;j<3;j++) LoSH[j] = Trg->PosH[j] - POV.PosH[j];
         MxV(World[Host->World].CNH,LoSH,LoSN);
      }
      MxV(Host->CN,LoSN,LoS);
      if (MAGV(LoS) < 1.0E-2) {
         printf("Target suspiciously close to POV.  POV Mode reverting to TRACK HOST\n");
         POV.Mode = TRACK_HOST;
         PovWidget.Spot[0].Selected = 1;
         PovWidget.Spot[1].Selected = 0;
      }
      else {
        UNITV(LoS);

/* .. Find POV orientation */
         if (LoS[2] > Cos1deg) {
            CosAz = 1.0;
            SinAz = 0.0;
            CosEl = 0.0;
            SinEl = -1.0;
         }
         else if (LoS[2] < -Cos1deg) {
            CosAz = 1.0;
            SinAz = 0.0;
            CosEl = 0.0;
            SinEl = 1.0;
         }
         else {
            SinEl = -LoS[2];
            CosEl = sqrt(LoS[0]*LoS[0]+LoS[1]*LoS[1]);
            SinAz = LoS[1]/CosEl;
            CosAz = LoS[0]/CosEl;
         }
         CTH[0][0] = CosAz*CosEl;
         CTH[0][1] = SinAz*CosEl;
         CTH[0][2] = -SinEl;
         CTH[1][0] = -SinAz;
         CTH[1][1] = CosAz;
         CTH[1][2] = 0.0;
         CTH[2][0] = CosAz*SinEl;
         CTH[2][1] = SinAz*SinEl;
         CTH[2][2] = CosEl;
         MxM(CTH,Host->CN,CTN);
         MxM(POV.C,CTN,POV.CN);
         MxM(POV.CN,World[Host->World].CNH,POV.CH);
         MxMT(POV.CN,Orb[Host->RefOrb].CLN,POV.CL);
         MxMT(POV.CN,Frm[Host->RefOrb].CN,POV.CF);
         MxMT(POV.CN,SC[Host->SC].B[0].CN,POV.CB);
      }
}
/**********************************************************************/
void PovFixedInHostMode(void)
{
      struct TargetType *Host;
      double pn[3];
      double Identity[3][3] = {{1.0,0.0,0.0},{0.0,1.0,0.0},{0.0,0.0,1.0}};
      long j,k;

      Host = &POV.Host;

/* .. Find rr, rn, rh for Host and POV */
      if (Host->Type == TARGET_WORLD) {
         for(j=0;j<3;j++) {
            Host->PosH[j] = World[Host->World].PosH[j];
            for(k=0;k<3;k++) Host->CN[j][k] = Identity[j][k];
            POV.PosH[j] = Host->PosH[j];
         }
      }
      else if (Host->Type == TARGET_REFORB) {
         for(j=0;j<3;j++) {
            Host->PosR[j] = 0.0;
            Host->PosN[j] = Orb[Host->RefOrb].PosN[j];
            for(k=0;k<3;k++)
               Host->CN[j][k] = Orb[Host->RefOrb].CLN[j][k];
         }
         MTxV(World[Host->World].CNH,Host->PosN,Host->PosH);
         for(j=0;j<3;j++) Host->PosH[j] += World[Host->World].PosH[j];
         MTxV(Host->CN,POV.PosB,pn);
         for(j=0;j<3;j++) {
            POV.PosR[j] = Host->PosR[j] + pn[j];
            POV.PosN[j] = POV.PosR[j] + Host->PosN[j];
            for(k=0;k<3;k++) POV.CN[j][k] = Host->CN[j][k];
         }
         MxM(POV.CN,World[Host->World].CNH,POV.CH);
         VxM(POV.PosN,World[Host->World].CNH,POV.PosH);
         for(j=0;j<3;j++) POV.PosH[j] += World[Host->World].PosH[j];
      }
      else if (Host->Type == TARGET_FRM) {
         for(j=0;j<3;j++) {
            Host->PosR[j] = Frm[Host->RefOrb].PosR[j];
            Host->PosN[j] = Host->PosR[j] + Orb[Host->RefOrb].PosN[j];
            for(k=0;k<3;k++)
               Host->CN[j][k] = Frm[Host->RefOrb].CN[j][k];
         }
         MTxV(World[Host->World].CNH,Host->PosN,Host->PosH);
         for(j=0;j<3;j++) Host->PosH[j] += World[Host->World].PosH[j];
         MTxV(Host->CN,POV.PosB,pn);
         for(j=0;j<3;j++) {
            POV.PosR[j] = Host->PosR[j] + pn[j];
            POV.PosN[j] = POV.PosR[j] + Orb[Host->RefOrb].PosN[j];
            for(k=0;k<3;k++) POV.CN[j][k] = Host->CN[j][k];
         }
         MxM(POV.CN,World[Host->World].CNH,POV.CH);
         VxM(POV.PosN,World[Host->World].CNH,POV.PosH);
         for(j=0;j<3;j++) POV.PosH[j] += World[Host->World].PosH[j];
      }
      else if (Host->Type == TARGET_SC) {
         for(j=0;j<3;j++) {
            Host->PosR[j] = SC[Host->SC].PosR[j];
            Host->PosN[j] = Host->PosR[j] + Orb[Host->RefOrb].PosN[j];
            for(k=0;k<3;k++)
               Host->CN[j][k] = SC[Host->SC].B[0].CN[j][k];
         }
         MTxV(World[Host->World].CNH,Host->PosN,Host->PosH);
         for(j=0;j<3;j++) Host->PosH[j] += World[Host->World].PosH[j];
         MTxV(Host->CN,POV.PosB,pn);
         for(j=0;j<3;j++) {
            POV.PosR[j] = Host->PosR[j] + pn[j];
            POV.PosN[j] = POV.PosR[j] + Orb[Host->RefOrb].PosN[j];
            for(k=0;k<3;k++) POV.CN[j][k] = Host->CN[j][k];
         }
         MxM(POV.CN,World[Host->World].CNH,POV.CH);
         VxM(POV.PosN,World[Host->World].CNH,POV.PosH);
         for(j=0;j<3;j++) POV.PosH[j] += World[Host->World].PosH[j];
      }
      else {
         for(j=0;j<3;j++) {
            Host->PosR[j] = SC[Host->SC].B[Host->Body].pn[j]
               + SC[Host->SC].PosR[j];
            Host->PosN[j] = Host->PosR[j] + Orb[Host->RefOrb].PosN[j];
            for(k=0;k<3;k++)
               Host->CN[j][k] = SC[Host->SC].B[Host->Body].CN[j][k];
         }
         MTxV(World[Host->World].CNH,Host->PosN,Host->PosH);
         for(j=0;j<3;j++) Host->PosH[j] += World[Host->World].PosH[j];
         MTxV(Host->CN,POV.PosB,pn);
         for(j=0;j<3;j++) {
            POV.PosR[j] = Host->PosR[j] + pn[j];
            POV.PosN[j] = POV.PosR[j] + Orb[Host->RefOrb].PosN[j];
            for(k=0;k<3;k++) POV.CN[j][k] = Host->CN[j][k];
         }
         MxM(POV.CN,World[Host->World].CNH,POV.CH);
         VxM(POV.PosN,World[Host->World].CNH,POV.PosH);
         for(j=0;j<3;j++) POV.PosH[j] += World[Host->World].PosH[j];
      }


/* .. Find POV orientation */
      MxM(POV.C,Host->CN,POV.CN);
      MxM(POV.CN,World[Host->World].CNH,POV.CH);
      MxMT(POV.CN,Orb[Host->RefOrb].CLN,POV.CL);
      MxMT(POV.CN,Frm[Host->RefOrb].CN,POV.CF);
      MxMT(POV.CN,SC[Host->SC].B[0].CN,POV.CB);
}
/**********************************************************************/
void PanZoomPOV(void)
{
      double C[3][3],Ang[3],f,dAng,dR;
      long i;

      if (POV.TimeToGo > 0.0) {
         f = (POV.TimeToGo-DTOUTGL)/POV.TimeToGo;
         if (f < 0.0) f = 0.0;
         MxMT(POV.C,POV.CmdPermute,C);
         C2A(POV.CmdSeq,C,&Ang[0],&Ang[1],
            &Ang[2]);
         for(i=0;i<3;i++) {
            dAng = Ang[i]-POV.CmdAngle[i];
            Ang[i] = POV.CmdAngle[i] + f*dAng;
         }
         A2C(POV.CmdSeq,Ang[0],Ang[1],Ang[2],C);
         MxM(C,POV.CmdPermute,POV.C);
         C2Q(POV.C,POV.q);

         dR = POV.Range-POV.CmdRange;
         POV.Range = POV.CmdRange + f*dR;

         POV.TimeToGo -= DTOUTGL;
      }
}
/**********************************************************************/
/*  Find POV location, orientation in RefOrb, N, H, etc               */
void UpdatePOV(void)
{
      PanZoomPOV();

      if (POV.Mode == TRACK_HOST) PovTrackHostMode();
      else if (POV.Mode == TRACK_TARGET) PovTrackTargetMode();
      else PovFixedInHostMode();
}
/**********************************************************************/
void FindModelMatrices(void)
{
      long Isc,Ib,i,Ir;
      double pbn[3],pcmn[3],prn[3];
      struct SCType *S;
      struct BodyType *B;
      struct OrbitType *O;
      struct RegionType *R;
      double PosR[3];

      for(Isc=0;Isc<Nsc;Isc++) {
         S = &SC[Isc];
         if (ScIsVisible(POV.Host.RefOrb,Isc,PosR)) {
            for(Ib=0;Ib<S->Nb;Ib++) {
               B = &S->B[Ib];
               if (S->RefPt == REFPT_CM) {
                  MTxV(B->CN,B->cm,pcmn);
               }
               else {
                  for(i=0;i<3;i++) pcmn[i] = 0.0;
               }
               for(i=0;i<3;i++) pbn[i] = S->PosR[i] + B->pn[i] - pcmn[i];
               BuildModelMatrix(B->CN,pbn,B->ModelMatrix);
            }
         }
      }

      for(Ir=0;Ir<Nrgn;Ir++) {
         R = &Rgn[Ir];
         if (R->Exists && R->World == POV.Host.World) {
            O = &Orb[POV.Host.RefOrb];
            for(i=0;i<3;i++) prn[i] = R->PosN[i]-O->PosN[i];
            BuildModelMatrix(R->CN,prn,R->ModelMatrix);
         }
      }
}
/**********************************************************************/
void CamRenderExec(void)
{
      FindModelMatrices();
      UpdatePOV();

      if (VREnabled) {
         SetDestination(OFFSCREEN);
         SetEye(LEFTEYE);
         DrawFarScene();
         DrawBodies();
         DrawNearAuxObjects();
         SetEye(RIGHTEYE);
         DrawFarScene();
         DrawBodies();
         DrawNearAuxObjects();
         BufferToScreen();
      }
      else {
         glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
         SetDestination(ONSCREEN);
         DrawFarScene();
         SetEye(MONOCULAR);
         DrawBodies();
         /* DrawContactSpheres(); */ /* Diagnostic only */
         DrawNearAuxObjects();
      }
}
/*********************************************************************/
void DrawClock(void)
{
      GLfloat Black[4] = {0.0,0.0,0.0,1.0};
      float ClockColor[4] = {0.8,0.8,0.5,1.0};
      char s[200];
      char TlmLabel[20] = "   Tlm Time: ";
      char SysLabel[20] = "System Time: ";

      glBlendFunc (GL_SRC_ALPHA, GL_ONE);
      glEnable(GL_BLEND);
      glMatrixMode(GL_PROJECTION);
      glPushMatrix();
      glLoadIdentity();
      gluOrtho2D(0,MapWidth,MapHeight,0);
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
      glMaterialfv(GL_FRONT,GL_EMISSION,ClockColor);

      if (MapShow[MAP_TLM_CLOCK]) {
         sprintf(s,"%s %04ld-%03ld-%02ld:%02ld:%02ld",TlmLabel,
            UTC.Year,UTC.doy,UTC.Hour,UTC.Minute,(long) UTC.Second);
      }
      else {
         sprintf(s,"%s %04ld-%03ld-%02ld:%02ld:%02ld",SysLabel,
            UTC.Year,UTC.doy,UTC.Hour,UTC.Minute,(long) UTC.Second);
      }
      glRasterPos2i(MapWidth-8*strlen(s),15);
      DrawBitmapString(GLUT_BITMAP_8_BY_13,s);

      glMatrixMode(GL_PROJECTION);
      glPopMatrix();
      glMatrixMode(GL_MODELVIEW);
      glMaterialfv(GL_FRONT,GL_EMISSION,Black);
      glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
/*********************************************************************/
void DrawCredits(void)
{
      GLfloat Black[4] = {0.0,0.0,0.0,1.0};
      float CreditColor[4] = {0.5,0.5,0.5,1.0};
      static double OldJD = 0;
      static long Toggle = 1;

      if (TT.JulDay-OldJD > 60.0/86400.0) {
         OldJD = TT.JulDay;
         if (Toggle) Toggle = 0;
         else Toggle = 1;
      }

      glBlendFunc (GL_SRC_ALPHA, GL_ONE);
      glEnable(GL_BLEND);
      glMatrixMode(GL_PROJECTION);
      glPushMatrix();
      glLoadIdentity();
      gluOrtho2D(0,MapWidth,MapHeight,0);
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
      glMaterialfv(GL_FRONT,GL_EMISSION,CreditColor);
      if (Toggle) {
         glRasterPos2i(5,MapHeight-15);
         DrawBitmapString(GLUT_BITMAP_8_BY_13,
            "42 written by Eric Stoneking");
         glRasterPos2f(5,MapHeight-2);
         DrawBitmapString(GLUT_BITMAP_8_BY_13,
            "Code 591, NASA Goddard Space Flight Center");
      }
      else {
         glRasterPos2i(5,MapHeight-15);
         DrawBitmapString(GLUT_BITMAP_8_BY_13,
            "GlastCam and LroCam contributions from");
         glRasterPos2f(5,MapHeight-2);
         DrawBitmapString(GLUT_BITMAP_8_BY_13,
            "Dean Chai and Neerav Shah.");
      }
      glMatrixMode(GL_PROJECTION);
      glPopMatrix();
      glMatrixMode(GL_MODELVIEW);
      glMaterialfv(GL_FRONT,GL_EMISSION,Black);
      glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
/*********************************************************************/
void DrawTdrsMap(void)
{
      float lng, lat;
      GLfloat Black[4] = {0.0,0.0,0.0,1.0};
      long i;

      /* TDRS from FDS products */
      glDisable(GL_LIGHTING);
      glColor4fv(TdrsColor);
      for(i=0;i<10;i++) {
         if (Tdrs[i].Exists) {
            lng = Tdrs[i].lng*R2D;
            lat = Tdrs[i].lat*R2D;
            glBegin(GL_POINTS);
               glVertex2f(lng,lat);
            glEnd();
            glRasterPos2f(lng,lat);
            glBitmap(0,0,0,0,4,-4,0);
            DrawBitmapString(GLUT_BITMAP_8_BY_13,Tdrs[i].Designation);
         }
      }
      glColor4fv(Black);
      glEnable(GL_LIGHTING);
}
/*********************************************************************/
long OccultedByEarth(double pge[3], double pte[3])
{
      double dp[3],magpg,phat[3],CosVec,CosOcc,SinOcc;
      long i, Occulted;

      for(i=0;i<3;i++) dp[i] = pte[i]-pge[i];
      magpg = MAGV(pge);
      for(i=0;i<3;i++) phat[i] = pge[i]/magpg;
      UNITV(dp);
      CosVec = VoV(dp,phat);

      SinOcc = World[EARTH].rad/magpg;
      CosOcc = -sqrt(1.0-SinOcc*SinOcc);

      if (CosVec > CosOcc) Occulted = 0;
      else Occulted = 1;
      return(Occulted);
}
/*********************************************************************/
void DrawMap(void)
{

      double a,Lng,Lat,x,y;
      double p[3],magr,CosEclipse,rad,lngc,latc,axis[3];
      GLfloat Black[4] = {0.0,0.0,0.0,1.0};
      float MinorLatLngColor[4] = {1.0,1.0,1.0,0.2};
      float MajorLatLngColor[4] = {1.0,1.0,1.0,0.4};
      float OrbitColor[4] = {1.0,1.0,1.0,1.0};
      float OrbitKnotColor[4] = {1.0,1.0,1.0,1.0};
      GLfloat GroundStationColor[4] = {0.7,0.5,0.1,1.0};
      GLfloat NonHostColor[4] = {0.5,0.5,0.5,1.0};
      struct OrbitType *Eph;
      struct SCType *S;
      struct WorldType *W;
      double dt,rn[3],vn[3],anom,re[3],rmw[3];
      double Zaxis[3] = {0.0,0.0,1.0};
      double CEW[3][3],CEN[3][3],CWH[3][3],svh[3],svw[3];
      float OldLng,OldLat;
      long i,k,Im,Isc;
      double rmh[3],rmn[3];

      glClear(GL_COLOR_BUFFER_BIT);
      glMaterialfv(GL_FRONT,GL_DIFFUSE,Black);

      S = &SC[POV.Host.SC];
      if (World[POV.Host.World].Type != SUN) {
         W = &World[Orb[S->RefOrb].World];

         for(i=0;i<3;i++) svh[i] = -W->PosH[i];
         UNITV(svh);
         MxM(W->CWN,W->CNH,CWH);
         MxV(CWH,svh,svw);
         magr = MAGV(S->PosN);
         CosEclipse = -sqrt(1.0-(W->rad*W->rad/magr/magr));


/* .. Map with Day/Night Shading */
         if (POV.Host.World == LUNA) {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, World[LUNA].ColTexTag);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, World[LUNA].BumpTexTag);
            glUseProgram(MoonMapShaderProgram);
            glUniform3f(MoonMapSunVecLoc,svw[0],svw[1],svw[2]);
            glUniform1f(MoonMapCosEclLoc,CosEclipse);
         }
         else {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, World[POV.Host.World].MapTexTag);
            glUseProgram(MapShaderProgram);
            glUniform3f(MapSunVecLoc,svw[0],svw[1],svw[2]);
            glUniform1f(MapCosEclLoc,CosEclipse);
         }

         glBegin(GL_QUADS);
            glVertex2f(-180.0,-90.0);
            glVertex2f(180.0,-90.0);
            glVertex2f(180.0,90.0);
            glVertex2f(-180.0,90.0);
         glEnd();
         glUseProgram(0);
         glActiveTexture(GL_TEXTURE0);

/* .. Latitude/Longitude Grid */
         glDisable(GL_LIGHTING);
         glColor4fv(MinorLatLngColor);
         glBegin(GL_LINES);
            for(a=-75.0;a<90.0;a+=15.0) {
               glVertex2f(-180.0,a);
               glVertex2f( 180.0,a);
            }
            for(a=-165.0;a<180.0;a+=15.0) {
               glVertex2f(a,-90.0);
               glVertex2f(a, 90.0);
            }
         glEnd();
         glColor4fv(MajorLatLngColor);
         glBegin(GL_LINES);
            for(a=-45.0;a<90.0;a+=45.0) {
               glVertex2f(-180.0,a);
               glVertex2f( 180.0,a);
            }
            for(a=-135.0;a<180.0;a+=45.0) {
               glVertex2f(a,-90.0);
               glVertex2f(a, 90.0);
            }
         glEnd();

/* .. Sun Sprite */
         Lng = atan2(svw[1],svw[0])*R2D;
         Lat = asin(svw[2])*R2D;
         glEnable(GL_TEXTURE_2D);
         glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
         glBindTexture(GL_TEXTURE_2D,SunSpriteTexTag);
         x=4.0;
         y=x;
         glBegin(GL_QUADS);
            glTexCoord2f(0.0,1.0);
            glVertex2f(Lng-x,Lat-y);
            glTexCoord2f(1.0,1.0);
            glVertex2f(Lng+x,Lat-y);
            glTexCoord2f(1.0,0.0);
            glVertex2f(Lng+x,Lat+y);
            glTexCoord2f(0.0,0.0);
            glVertex2f(Lng-x,Lat+y);
         glEnd();
         glDisable(GL_TEXTURE_2D);

/* .. Anti-Sun Sprite */
         if (Lng > 0.0) Lng -= 180.0;
         else Lng += 180.0;
         Lat = -Lat;
         glEnable(GL_TEXTURE_2D);
         glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
         glBindTexture(GL_TEXTURE_2D,AntiSunSpriteTexTag);
         x=4.0;
         y=x;
         glBegin(GL_QUADS);
            glTexCoord2f(0.0,1.0);
            glVertex2f(Lng-x,Lat-y);
            glTexCoord2f(1.0,1.0);
            glVertex2f(Lng+x,Lat-y);
            glTexCoord2f(1.0,0.0);
            glVertex2f(Lng+x,Lat+y);
            glTexCoord2f(0.0,0.0);
            glVertex2f(Lng-x,Lat+y);
         glEnd();
         glDisable(GL_TEXTURE_2D);

/* .. Moon Sprite */
         if (W->Nsat > 0) { /* Draw Moon Sprites */
            glEnable(GL_TEXTURE_2D);
            glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
            glBindTexture(GL_TEXTURE_2D,MoonSpriteTexTag);
            for(Im=0;Im<W->Nsat;Im++) {
               MxV(W->CWN,World[W->Sat[Im]].eph.PosN,rmw);
               UNITV(rmw);
               Lng = atan2(rmw[1],rmw[0])*R2D;
               Lat = asin(rmw[2])*R2D;
               x=4.0;
               y=x;
               glBegin(GL_QUADS);
                  glTexCoord2f(0.0,1.0);
                  glVertex2f(Lng-x,Lat-y);
                  glTexCoord2f(1.0,1.0);
                  glVertex2f(Lng+x,Lat-y);
                  glTexCoord2f(1.0,0.0);
                  glVertex2f(Lng+x,Lat+y);
                  glTexCoord2f(0.0,0.0);
                  glVertex2f(Lng-x,Lat+y);
               glEnd();
            }
            glDisable(GL_TEXTURE_2D);
         }
         else if (W->Type == MOON) { /* Draw Parent Sprite */
            glEnable(GL_TEXTURE_2D);
            glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
            glBindTexture(GL_TEXTURE_2D,MoonSpriteTexTag);
            for(i=0;i<3;i++) rmn[i] = -W->eph.PosN[i];
            UNITV(rmn);
            MTxV(World[W->Parent].CNH,rmn,rmh);
            MxV(W->CNH,rmh,rmn);
            MxV(W->CWN,rmn,rmw);
            Lng = atan2(rmw[1],rmw[0])*R2D;
            Lat = asin(rmw[2])*R2D;
            x=4.0;
            y=x;
            glBegin(GL_QUADS);
               glTexCoord2f(0.0,1.0);
               glVertex2f(Lng-x,Lat-y);
               glTexCoord2f(1.0,1.0);
               glVertex2f(Lng+x,Lat-y);
               glTexCoord2f(1.0,0.0);
               glVertex2f(Lng+x,Lat+y);
               glTexCoord2f(0.0,0.0);
               glVertex2f(Lng-x,Lat+y);
            glEnd();
            glDisable(GL_TEXTURE_2D);
         }

/* .. Ground Stations */
         glColor4fv(GroundStationColor);
         glPointSize(5.0);
         for(i=0;i<Ngnd;i++) {
            if (GroundStation[i].Exists && GroundStation[i].World == POV.Host.World) {
               glBegin(GL_POINTS);
                  glVertex2f(GroundStation[i].lng,GroundStation[i].lat);
               glEnd();
               glRasterPos2f(GroundStation[i].lng,GroundStation[i].lat);
               glBitmap(0,0,0,0,5,-4,0);
               DrawBitmapString(GLUT_BITMAP_8_BY_13,GroundStation[i].Label);
            }
         }
         glPointSize(1.0);

         /* Orbits of SC that aren't the POV host */
         for(Isc=0;Isc<Nsc;Isc++) {
            if (SC[Isc].Exists && Orb[SC[Isc].RefOrb].World == Orb[S->RefOrb].World && Isc != S->ID) {

               /* SC Sprite */
               MxV(W->CWN,SC[Isc].PosN,p);
               UNITV(p);
               Lng = atan2(p[1],p[0])*R2D;
               Lat = asin(p[2])*R2D;
               glEnable(GL_TEXTURE_2D);
               glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
               glBindTexture(GL_TEXTURE_2D,SC[Isc].SpriteTexTag);
               x=12.0;
               y=0.5*x;
               glBegin(GL_QUADS);
                  glTexCoord2f(0.0,1.0);
                  glVertex2f(Lng-x,Lat-y);
                  glTexCoord2f(1.0,1.0);
                  glVertex2f(Lng+x,Lat-y);
                  glTexCoord2f(1.0,0.0);
                  glVertex2f(Lng+x,Lat+y);
                  glTexCoord2f(0.0,0.0);
                  glVertex2f(Lng-x,Lat+y);
               glEnd();
               glDisable(GL_TEXTURE_2D);

               glEnable(GL_LINE_SMOOTH);

               /* Horizon Circle */
               glLineWidth(1.5);
               MxV(W->CWN,SC[Isc].PosN,axis);
               UNITV(axis);
               lngc = atan2(axis[1],axis[0]);
               latc = asin(axis[2]);
               rad = acos(W->rad/magr);
               glColor4fv(NonHostColor);
               DrawSmallCircle(lngc,latc,rad);

               /* Projected orbit track */
               glPointSize(3.0);
               glLineWidth(1.0);
               Eph = &Orb[SC[Isc].RefOrb];
               Eph2RV(Eph->mu,Eph->SLR,Eph->ecc,Eph->inc,Eph->RAAN,Eph->ArgP,
                  DynTime-3600.0-Eph->tp,rn,vn,&anom);
               SimpRot(Zaxis,-3600.0*W->w,CEW);
               MxM(CEW,W->CWN,CEN);
               MxV(CEN,rn,re);
               magr = MAGV(re);
               OldLng = atan2(re[1],re[0])*R2D;
               OldLat = asin(re[2]/magr)*R2D;
               for(k=-60;k<61;k++) {
                  dt = ((double) k)*60.0;
                  Eph2RV(Eph->mu,Eph->SLR,Eph->ecc,Eph->inc,Eph->RAAN,Eph->ArgP,
                     DynTime+dt-Eph->tp,rn,vn,&anom);
                  SimpRot(Zaxis,W->w*dt,CEW);
                  MxM(CEW,W->CWN,CEN);
                  MxV(CEN,rn,re);
                  magr = MAGV(re);
                  Lng = atan2(re[1],re[0])*R2D;
                  Lat = asin(re[2]/magr)*R2D;
                  glColor4fv(NonHostColor);
                  glBegin(GL_LINES);
                     if (fabs(Lng-OldLng) < 180.0) {
                        glVertex2f(OldLng,OldLat);
                        glVertex2f(Lng,Lat);
                     }
                     else if(Lng > OldLng) {
                        glVertex2f(OldLng+360.0,OldLat);
                        glVertex2f(Lng,Lat);
                        glVertex2f(OldLng,OldLat);
                        glVertex2f(Lng-360.0,Lat);
                     }
                     else {
                        glVertex2f(OldLng-360.0,OldLat);
                        glVertex2f(Lng,Lat);
                        glVertex2f(OldLng,OldLat);
                        glVertex2f(Lng+360.0,Lat);
                     }
                  glEnd();
                  OldLng = Lng;
                  OldLat = Lat;
               }
               glLineWidth(1.0);
               glDisable(GL_LINE_SMOOTH);
            }
         }

/* .. POV Host SC */
         /* SC Sprite */
         MxV(W->CWN,S->PosN,p);
         UNITV(p);
         Lng = atan2(p[1],p[0])*R2D;
         Lat = asin(p[2])*R2D;
         glEnable(GL_TEXTURE_2D);
         glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
         glBindTexture(GL_TEXTURE_2D,S->SpriteTexTag);
         x=12.0;
         y=0.5*x;
         glBegin(GL_QUADS);
            glTexCoord2f(0.0,1.0);
            glVertex2f(Lng-x,Lat-y);
            glTexCoord2f(1.0,1.0);
            glVertex2f(Lng+x,Lat-y);
            glTexCoord2f(1.0,0.0);
            glVertex2f(Lng+x,Lat+y);
            glTexCoord2f(0.0,0.0);
            glVertex2f(Lng-x,Lat+y);
         glEnd();
         glDisable(GL_TEXTURE_2D);

         glEnable(GL_LINE_SMOOTH);

         /* Horizon Circle */
         glLineWidth(1.5);
         MxV(W->CWN,S->PosN,axis);
         UNITV(axis);
         lngc = atan2(axis[1],axis[0]);
         latc = asin(axis[2]);
         rad = acos(W->rad/magr);
         glColor4fv(GroundStationColor);
         DrawSmallCircle(lngc,latc,rad);

         /* Projected orbit track */
         glPointSize(3.0);
         glLineWidth(1.0);
         Eph = &Orb[POV.Host.RefOrb];
         Eph2RV(Eph->mu,Eph->SLR,Eph->ecc,Eph->inc,Eph->RAAN,Eph->ArgP,
            DynTime-3600.0-Eph->tp,rn,vn,&anom);
         SimpRot(Zaxis,-3600.0*W->w,CEW);
         MxM(CEW,W->CWN,CEN);
         MxV(CEN,rn,re);
         magr = MAGV(re);
         OldLng = atan2(re[1],re[0])*R2D;
         OldLat = asin(re[2]/magr)*R2D;
         for(k=-60;k<61;k++) {
            dt = ((double) k)*60.0;
            Eph2RV(Eph->mu,Eph->SLR,Eph->ecc,Eph->inc,Eph->RAAN,Eph->ArgP,
               DynTime+dt-Eph->tp,rn,vn,&anom);
            SimpRot(Zaxis,W->w*dt,CEW);
            MxM(CEW,W->CWN,CEN);
            MxV(CEN,rn,re);
            magr = MAGV(re);
            Lng = atan2(re[1],re[0])*R2D;
            Lat = asin(re[2]/magr)*R2D;
            glColor4fv(OrbitColor);
            glBegin(GL_LINES);
               if (fabs(Lng-OldLng) < 180.0) {
                  glVertex2f(OldLng,OldLat);
                  glVertex2f(Lng,Lat);
               }
               else if(Lng > OldLng) {
                  glVertex2f(OldLng+360.0,OldLat);
                  glVertex2f(Lng,Lat);
                  glVertex2f(OldLng,OldLat);
                  glVertex2f(Lng-360.0,Lat);
               }
               else {
                  glVertex2f(OldLng-360.0,OldLat);
                  glVertex2f(Lng,Lat);
                  glVertex2f(OldLng,OldLat);
                  glVertex2f(Lng+360.0,Lat);
               }
            glEnd();
            if (k%10 == 0) {
               if (dt < 0.0) {
                  OrbitKnotColor[0] = 1.0;
                  OrbitKnotColor[1] = 0.5+0.5*(3600.0+dt)/3600.0;
                  OrbitKnotColor[2] = 0.5+0.5*(3600.0+dt)/3600.0;
               }
               else {
                  OrbitKnotColor[0] = 0.5+0.5*(3600.0-dt)/3600.0;
                  OrbitKnotColor[1] = 0.5+0.5*(3600.0-dt)/3600.0;
                  OrbitKnotColor[2] = 1.0;
               }
               glColor4fv(OrbitKnotColor);
               glBegin(GL_POINTS);
                  glVertex2f(Lng,Lat);
               glEnd();
            }
            OldLng = Lng;
            OldLat = Lat;
         }
         glLineWidth(1.0);
         glDisable(GL_LINE_SMOOTH);


/* .. TDRS in View */
         if (POV.Host.World == EARTH && CamShow[TDRS]) DrawTdrsMap();

/* .. Clock shows (possibly offset) time */
         if (MapShow[MAP_CLOCK]) DrawClock();

/* .. Credits */
         if (MapShow[MAP_CREDITS]) DrawCredits();

         glEnable(GL_LIGHTING);

      }
}
/**********************************************************************/
void DrawOrreryHUD(struct OrreryPOVType *O)
{
      struct WidgetType *W;
      int x0, y0;
      GLfloat ScaleColor[3] = {1.0,1.0,1.0};
      GLubyte ButtonGlyph[6][32] = {
      {0x00, 0x00,   0x00, 0x00,   0x01, 0x80,   0x01, 0x80,
       0x01, 0x80,   0x01, 0x80,   0x01, 0x80,   0xff, 0xff,
       0xff, 0xff,   0x01, 0x80,   0x01, 0x80,   0x01, 0x80,
       0x01, 0x80,   0x01, 0x80,   0x00, 0x00,   0x00, 0x00}, /* In */

      {0x00, 0x00,   0x00, 0x00,   0x00, 0x00,   0x00, 0x00,
       0x00, 0x00,   0x00, 0x00,   0x00, 0x00,   0xff, 0xff,
       0xff, 0xff,   0x00, 0x00,   0x00, 0x00,   0x00, 0x00,
       0x00, 0x00,   0x00, 0x00,   0x00, 0x00,   0x00, 0x00}, /* Out */

      {0x01, 0x80,   0x01, 0x80,   0x01, 0x80,   0x01, 0x80,
       0x01, 0x80,   0x01, 0x80,   0x01, 0x80,   0x81, 0x81,
       0xc1, 0x83,   0x61, 0x86,   0x31, 0x8c,   0x19, 0x98,
       0x0d, 0xb0,   0x07, 0xe0,   0x03, 0xc0,   0x01, 0x80}, /* Up */

      {0x01, 0x80,   0x03, 0xc0,   0x07, 0xe0,   0x0d, 0xb0,
       0x19, 0x98,   0x31, 0x8c,   0x61, 0x86,   0xc1, 0x83,
       0x81, 0x81,   0x01, 0x80,   0x01, 0x80,   0x01, 0x80,
       0x01, 0x80,   0x01, 0x80,   0x01, 0x80,   0x01, 0x80}, /* Down */

      {0x01, 0x80,   0x03, 0x00,   0x06, 0x00,   0x0c, 0x00,
       0x18, 0x00,   0x30, 0x00,   0x60, 0x00,   0xff, 0xff,
       0xff, 0xff,   0x60, 0x00,   0x30, 0x00,   0x18, 0x00,
       0x0c, 0x00,   0x06, 0x00,   0x03, 0x00,   0x01, 0x80}, /* Left */

      {0x01, 0x80,   0x00, 0xc0,   0x00, 0x60,   0x00, 0x30,
       0x00, 0x18,   0x00, 0x0c,   0x00, 0x06,   0xff, 0xff,
       0xff, 0xff,   0x00, 0x06,   0x00, 0x0c,   0x00, 0x18,
       0x00, 0x30,   0x00, 0x60,   0x00, 0xc0,   0x01, 0x80}  /* Right */
      };

      long i;

      glMatrixMode(GL_PROJECTION);
      glPushMatrix();
      glLoadIdentity();
      gluOrtho2D(0,OrreryWidth,OrreryHeight,0);
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();

      W = &OrreryWidget;
      DrawWidget(W);

      for(i=0;i<6;i++) {
         glRasterPos2i((W->Spot[i].xmin+W->Spot[i].xmax)/2,
                       (W->Spot[i].ymin+W->Spot[i].ymax)/2);
         glBitmap(0,0,0,0,-8,-8,0);
         glBitmap(16,16,0.0,0.0,0.0,0.0,ButtonGlyph[i]);
      }

/* .. Scale Bar and Text */
      x0 = W->xmin + 10;
      y0 = W->ymax - 20;
      glColor3fv(ScaleColor);
      glBegin(GL_LINE_STRIP);
         glVertex2i(x0,y0-10);
         glVertex2i(x0,y0);
         glVertex2i(x0+80,y0);
         glVertex2i(x0+80,y0-10);
      glEnd();
      glRasterPos2i(x0+40-4*strlen(O->ScaleLabel[O->Zoom]),y0+15);
      DrawBitmapString(GLUT_BITMAP_8_BY_13,O->ScaleLabel[O->Zoom]);

      glMatrixMode(GL_PROJECTION);
      glPopMatrix();
      glMatrixMode(GL_MODELVIEW);

}
/**********************************************************************/
void DrawOrrery(void)
{
      struct OrreryPOVType *O;
      double t,dt,t1,t2,r[3],v[3],anom;
      struct WorldType *W,*W1,*W2,*P,*M,*MB;
      struct OrbitType *E;
      struct LagrangeSystemType *LS;
      struct LagrangePointType *LP;
      double MaxRadFactor,MaxAnom;
      GLfloat Black[4] = {0.0,0.0,0.0,1.0};
      long Ip,Iw,Im,Ib,Isc,Is,Iorb;
      GLfloat WorldColor[3] = {1.0,1.0,1.0};
      GLfloat WorldNameColor[3] = {1.0,0.25,1.0};
      GLfloat WorldOrbitColor[3] = {0.576,0.439,0.859};
      GLfloat MinorBodyOrbitColor[3] = {0.45,0.22,0.42};
      GLfloat RefOrbColor[4] = {0.251,0.878,0.816,1.0};
      GLfloat ScColor[3] = {1.0,0.549,0.0};
      GLubyte ScGlyph[8] = {0x18,0x24,0x42,0x81,0x81,0x42,0x24,0x18};
      GLfloat LPLineColor[3] = {0.729,0.333,0.827};
      GLfloat LPColor[3] = {0.847,0.749,0.847};
      GLfloat SaturnRingColor[4] = {0.867,0.627,0.867,0.2};
      GLfloat Color[4] = {0.0,0.0,0.0,1.0};
      double Period,ta,tc,te;
      char LPName[5][3] = {"L1","L2","L3","L4","L5"};
      double PointSize;

      O = &Orrery;

      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      glMaterialfv(GL_FRONT,GL_DIFFUSE,Black);
      glDisable(GL_LIGHTING);

      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      glOrtho(-O->Radius,O->Radius,
              -O->Radius*OrreryHeight/OrreryWidth,
               O->Radius*OrreryHeight/OrreryWidth,
              -60.0*AU2m,60.0*AU2m);
      glMatrixMode(GL_MODELVIEW);

/* .. Gradient fill background */
/*      glBegin(GL_QUADS);
**         glVertex2d(-O->Radius,-O->Radius);
**         glVertex2d( O->Radius,-O->Radius);
**         glVertex2d( O->Radius, O->Radius);
**         glVertex2d(-O->Radius, O->Radius);
**      glEnd();
*/

      if (O->Regime == ORB_CENTRAL) {
         RotateL2R(O->CN);
         glTranslated(-O->PosN[0],-O->PosN[1],-O->PosN[2]);
         W = &World[O->World];
         RotateL2R(W->CNH);
         glTranslated(-W->PosH[0],-W->PosH[1],-W->PosH[2]);
      }
      else {
         RotateL2R(O->CN);
         glTranslated(-O->PosN[0],-O->PosN[1],-O->PosN[2]);
         LS = &LagSys[O->LagSys];
         LP = &LS->LP[O->LP];
         glTranslated(-LP->PosN[0],-LP->PosN[1],-LP->PosN[2]);
         W = &World[LS->Body1];
         glTranslated(-W->PosH[0],-W->PosH[1],-W->PosH[2]);
      }

      glDisable(GL_DEPTH);

      glLineWidth(2.0);

/* .. Draw Orbits of Planets and Moons */
      for(Ip=MERCURY;Ip<=PLUTO;Ip++) {
         if(World[Ip].Exists) {
            P = &World[Ip];
            E = &P->eph;
            if (E->SMA > 0.05*O->Radius) {
               glColor3fv(WorldOrbitColor);
               glBegin(GL_LINE_LOOP);
                  dt = 0.01*E->Period;
                  t1 = DynTime-E->tp - 0.5*E->Period;
                  t2 = DynTime-E->tp + 0.5*E->Period;
                  for(t=t1;t<t2;t+=dt) {
                     Eph2RV(E->mu,E->SLR,E->ecc,E->inc,E->RAAN,E->ArgP,
                        t,r,v,&anom);
                     glVertex3dv(r);
                  }
               glEnd();
               glPushMatrix();
               glTranslatef(E->PosN[0],E->PosN[1],E->PosN[2]);
               RotateR2L(P->CNH);
               /* Moons */
               for(Im=0;Im<P->Nsat;Im++) {
                  Iw = P->Sat[Im];
                  M = &World[Iw];
                  E = &M->eph;
                  if (E->SMA > 0.05*O->Radius) {
                     glColor3fv(WorldOrbitColor);
                     glBegin(GL_LINE_LOOP);
                        dt = 0.01*E->Period;
                        t1 = E->tp - 0.5*E->Period;
                        t2 = E->tp + 0.5*E->Period;
                        for(t=t1;t<t2;t+=dt) {
                           Eph2RV(E->mu,E->SLR,E->ecc,E->inc,E->RAAN,E->ArgP,
                              t,r,v,&anom);
                           glVertex3dv(r);
                        }
                     glEnd();
                  }
               }
               glPopMatrix();
            }
         }
      }

/* .. Draw Orbits for Asteroids and Comets */
      for(Ib=0;Ib<Nmb;Ib++) {
         if(World[55+Ib].Exists) {
            MB = &World[55+Ib];
            E = &MB->eph;
            if (E->SMA > 0.05*O->Radius) {
               glColor3fv(MinorBodyOrbitColor);
               glBegin(GL_LINE_LOOP);
                  dt = 0.01*E->Period;
                  t1 = E->tp - 0.5*E->Period;
                  t2 = E->tp + 0.5*E->Period;
                  for(t=t1;t<t2;t+=dt) {
                     Eph2RV(E->mu,E->SLR,E->ecc,E->inc,E->RAAN,E->ArgP,
                        t,r,v,&anom);
                     glVertex3dv(r);
                  }
               glEnd();
            }
         }
      }

/* .. Draw Lagrange Systems */
      glPointSize(6.0);
      for(Is=0;Is<3;Is++) {
         if (LagSys[Is].Exists) {
            LS = &LagSys[Is];
            W1 = &World[LS->Body1];
            W2 = &World[LS->Body2];
            if (W2->eph.SMA > 0.05*O->Radius) {
               glPushMatrix();
               glTranslated(W1->PosH[0],W1->PosH[1],W1->PosH[2]);
               RotateR2L(W1->CNH);
               /* Draw lines between Lagrange Points */
               glColor3fv(LPLineColor);
               glBegin(GL_LINES);
                  /* L2-L3 */
                  glVertex3dv(LS->LP[1].PosN);
                  glVertex3dv(LS->LP[2].PosN);
                  /* Body1-L4 */
                  glVertex3d(0.0,0.0,0.0);
                  glVertex3dv(LS->LP[3].PosN);
                  /* Body1-L5 */
                  glVertex3d(0.0,0.0,0.0);
                  glVertex3dv(LS->LP[4].PosN);
                  /* Body2-L4 */
                  glVertex3dv(W2->eph.PosN);
                  glVertex3dv(LS->LP[3].PosN);
                  /* Body2-L5 */
                  glVertex3dv(W2->eph.PosN);
                  glVertex3dv(LS->LP[4].PosN);
               glEnd();
               /* Draw and label Lagrange Points */
               glColor3fv(LPColor);
               for(Ip=0;Ip<5;Ip++) {
                  glBegin(GL_POINTS);
                     glVertex3dv(LS->LP[Ip].PosN);
                  glEnd();
                  glRasterPos3dv(LS->LP[Ip].PosN);
                  glBitmap(0,0,0.0,0.0,5,5,NULL);
                  DrawBitmapString(GLUT_BITMAP_8_BY_13,LPName[Ip]);
               }
               glPopMatrix();
            }
         }
      }

      glEnable(GL_DEPTH);

/* .. Draw Sun */
      P = &World[SOL];
      glColor3fv(P->Color);
      PointSize = P->rad/O->Radius*((double) OrreryWidth);
      if (PointSize < 12.0) {
         glPointSize(12.0);
         glBegin(GL_POINTS);
            glVertex3d(0.0,0.0,0.0);
         glEnd();
      }
      else {
         glPushMatrix();
         glScaled(P->rad,P->rad,P->rad);
         glCallList(OrrerySphereList);
         glPopMatrix();
      }

/* .. Draw Planets and Moons */
      for(Ip=MERCURY;Ip<=PLUTO;Ip++) {
         if(World[Ip].Exists) {
            P = &World[Ip];
            E = &P->eph;
            if (E->SMA > 0.05*O->Radius) {
               glColor3fv(WorldColor);
               glPushMatrix();
               glTranslatef(E->PosN[0],E->PosN[1],E->PosN[2]);
               RotateR2L(P->CNH);
               PointSize = P->rad/O->Radius*((double) OrreryWidth);
               if (PointSize < 6.0) {
                  glPointSize(6.0);
                  glBegin(GL_POINTS);
                     glVertex3d(0.0,0.0,0.0);
                  glEnd();
               }
               else {
                  glPushMatrix();
                  glScaled(P->rad,P->rad,P->rad);
                  glCallList(OrrerySphereList);
                  if (Ip == SATURN) {
                     glColor4fv(SaturnRingColor);
                     //glCallList(OrreryRingList);
                  }
                  glPopMatrix();
               }
               glColor3fv(WorldNameColor);
               glRasterPos3d(0.0,0.0,0.0);
               glBitmap(0,0,0.0,0.0,PointSize/2+2,5,NULL);
               DrawBitmapString(GLUT_BITMAP_8_BY_13,P->Name);
               /* Moons */
               for(Im=0;Im<P->Nsat;Im++) {
                  Iw = P->Sat[Im];
                  M = &World[Iw];
                  E = &M->eph;
                  if (E->SMA > 0.05*O->Radius) {
                     glColor3fv(WorldColor);
                     glPushMatrix();
                     glTranslatef(E->PosN[0],E->PosN[1],E->PosN[2]);
                     PointSize = M->rad/O->Radius*((double) OrreryWidth);
                     if (PointSize < 6.0) {
                        glPointSize(6.0);
                        glBegin(GL_POINTS);
                           glVertex3d(0.0,0.0,0.0);
                        glEnd();
                     }
                     else {
                        glPushMatrix();
                        glScaled(M->rad,M->rad,M->rad);
                        glCallList(OrrerySphereList);
                        glPopMatrix();
                     }
                     glColor3fv(WorldNameColor);
                     glRasterPos3d(0.0,0.0,0.0);
                     glBitmap(0,0,0.0,0.0,PointSize/2+2,5,NULL);
                     DrawBitmapString(GLUT_BITMAP_8_BY_13,M->Name);
                     glPopMatrix();
                  }
               }
               glPopMatrix();
            }
         }
      }

/* .. Draw Asteroids and Comets */
      glPointSize(6.0);
      for(Ib=0;Ib<Nmb;Ib++) {
         if(World[55+Ib].Exists) {
            MB = &World[55+Ib];
            E = &MB->eph;
            if (E->SMA > 0.05*O->Radius) {
               glColor3fv(WorldColor);
               glPushMatrix();
               glTranslatef(E->PosN[0],E->PosN[1],E->PosN[2]);
               PointSize = MB->rad/O->Radius*((double) OrreryWidth);
               if (PointSize < 6.0) {
                  glPointSize(6.0);
                  glBegin(GL_POINTS);
                     glVertex3d(0.0,0.0,0.0);
                  glEnd();
               }
               else {
                  glPushMatrix();
                  glScaled(MB->rad,MB->rad,MB->rad);
                  glCallList(OrrerySphereList);
                  glPopMatrix();
               }
               glColor3fv(WorldNameColor);
               glRasterPos3d(0.0,0.0,0.0);
               glBitmap(0,0,0.0,0.0,PointSize/2+2,5,NULL);
               DrawBitmapString(GLUT_BITMAP_8_BY_13,MB->Name);
               glPopMatrix();
            }
         }
      }

/* .. Draw RefOrbs */
      for(Iorb=0;Iorb<Norb;Iorb++) {
         if(Orb[Iorb].Exists) {
            E = &Orb[Iorb];
            if (E->Regime == ORB_CENTRAL) {
               if (E->rmin > 0.05*O->Radius) {
                  W = &World[E->World];
                  glPushMatrix();
                  glTranslated(W->PosH[0],W->PosH[1],W->PosH[2]);
                  RotateR2L(W->CNH);
                  if (E->ecc < 1.0) {
                     glBegin(GL_LINE_LOOP);
                        t1 = E->tp - 0.5*E->Period;
                        t2 = E->tp + 0.5*E->Period;
                        dt = 0.002*(t2-t1);
                        for(t=t1;t<t2;t+=dt) {
                           Eph2RV(E->mu,E->SLR,E->ecc,E->inc,E->RAAN,E->ArgP,
                              t,r,v,&anom);
                           if (r[2] < 0.0 && sqrt(r[0]*r[0]+r[1]*r[1]) < W->rad)
                              RefOrbColor[3] = 0.0;
                           else
                              RefOrbColor[3] = 1.0;
                           glColor4fv(RefOrbColor);
                           glVertex3dv(r);
                        }
                     glEnd();
                  }
                  else {
                     glBegin(GL_LINE_STRIP);
                        MaxRadFactor = 20.0;
                        MaxAnom = acos((1.0-MaxRadFactor+E->ecc)/(MaxRadFactor*E->ecc));
                        t2 = TimeSincePeriapsis(E->mu,E->SLR,E->ecc,MaxAnom);
                        t1 = -t2;
                        dt = 0.002*(t2-t1);
                        for(t=t1;t<t2;t+=dt) {
                           Eph2RV(E->mu,E->SLR,E->ecc,E->inc,E->RAAN,E->ArgP,
                              t,r,v,&anom);
                           if (r[2] < 0.0 && sqrt(r[0]*r[0]+r[1]*r[1]) < W->rad)
                              RefOrbColor[3] = 0.0;
                           else
                              RefOrbColor[3] = 1.0;
                           glColor4fv(RefOrbColor);
                           glVertex3dv(r);
                        }
                     glEnd();
                  }
                  glPopMatrix();
               }
            }
            else { /* E->Type == ORB_THREE_BODY */
               LS = &LagSys[E->Sys];
               W1 = &World[LS->Body1];
               W2 = &World[LS->Body2];
               glPushMatrix();
               glTranslated(W1->PosH[0],W1->PosH[1],W1->PosH[2]);
               RotateR2L(W1->CNH);
               if (sqrt(E->Ay*E->Ay+E->By*E->By+E->Az*E->Az) > 0.05*O->Radius) {
                  LP = &LS->LP[O->LP];
                  glBegin(GL_LINE_STRIP);
                     Period = TwoPi/LP->w1;
                     dt = 0.002*Period;
                     t1 = DynTime - 0.5*Period;
                     t2 = DynTime + 0.5*Period;
                     ta = t1+Period/6.0;
                     tc = t1+Period/2.0;
                     te = t1+5.0/6.0*Period;
                     for(t=t1;t<=t2;t+=dt) {
                        LagModes2RV(t,LS,E,r,v);
                        /* Red */
                        if (t < tc) {
                           Color[0] = 1.0;
                        }
                        else if (t < te) {
                           Color[0] = 1.0-0.5*(t-tc)/(te-tc);
                        }
                        else {
                           Color[0] = 0.5+0.5*(t-te)/(t2-te);
                        }
                        /* Green */
                        if (t < ta) {
                           Color[1] = 0.5;
                        }
                        else if (t < tc) {
                           Color[1] = 0.5+0.5*(t-ta)/(tc-ta);
                        }
                        else if (t < te) {
                           Color[1] = 1.0-0.5*(t-tc)/(te-tc);
                        }
                        else {
                           Color[1] = 0.5;
                        }
                        /* Blue */
                        if (t < ta) {
                           Color[2] = 1.0-0.5*(t-t1)/(ta-t1);
                        }
                        else if (t < tc) {
                           Color[2] = 0.5+0.5*(t-ta)/(tc-ta);
                        }
                        else {
                           Color[2] = 1.0;
                        }
                        glColor4fv(Color);
                        glVertex3dv(r);
                     }
                  glEnd();
               }
               glPopMatrix();
            }
         }
      }

/* .. Draw Spacecraft */
      glColor3fv(ScColor);
      for(Isc=0;Isc<Nsc;Isc++) {
         if(SC[Isc].Exists) {
            E = &Orb[SC[Isc].RefOrb];
            W = &World[E->World];
            glPushMatrix();
            glTranslated(W->PosH[0],W->PosH[1],W->PosH[2]);
            RotateR2L(W->CNH);
            glRasterPos3dv(SC[Isc].PosN);
            glBitmap(8,8,4.0,4.0,5.0,-13.0,ScGlyph);
            DrawBitmapString(GLUT_BITMAP_8_BY_13,SC[Isc].Label);
            glPopMatrix();
         }
      }

      glPointSize(1.0);
      glLineWidth(1.0);

      DrawOrreryHUD(O);

      glEnable(GL_LIGHTING);

}
/**********************************************************************/
void DrawSphereHUD(void)
{
      char top[6][3] = {"+X","-X","+Y","-Y","+Z","-Z"};
      char center[3][4][3] = {
                            {"+Y","-Y","+Z","-Z"},
                            {"+X","-X","+Z","-Z"},
                            {"+X","-X","+Y","-Y"} };
      char show[6][15] = {"Horizon","Sun","Moons","Planets",
                          "Galactic Plane","Constellations"};
      char vectors[6][17] = {"Magnetic Field","Angular Momentum",
                             "Velocity","Anti-velocity",
                             "Ecliptic North","Galactic North"};
      char fovs[3][19] = {"Coarse Sun Sensors","Fine Sun Sensors",
                          "Star Trackers"};
      char frames[3][2] = {"B","N","L"};

      int i,j;
      struct WidgetType *W;

/* .. Center Box */
      W = &CenterWidget;
      DrawWidget(W);

      glRasterPos2i((W->xmin+W->xmax)/2 - 12,W->ymax-14);
      DrawBitmapString(GLUT_BITMAP_8_BY_13,"Top");

      glRasterPos2i((W->xmin+W->xmax)/2 - 24,W->ymin+38);
      DrawBitmapString(GLUT_BITMAP_8_BY_13,"Center");

      for (i=0; i<6; i++) {
         glRasterPos2i(W->Spot[i].xmin+1,W->Spot[i].ymin+2);
         DrawBitmapString(GLUT_BITMAP_8_BY_13,top[i]);
      }

      /* Text displayed for spots 7-10 changes based on which axis is selected */
      if (W->Spot[0].Selected == 1 || W->Spot[1].Selected == 1) j = 0;
      else if (W->Spot[2].Selected == 1 || W->Spot[3].Selected == 1) j = 1;
      else if (W->Spot[4].Selected == 1 || W->Spot[5].Selected == 1) j = 2;

      for (i=6; i<10; i++) {
         glRasterPos2i(W->Spot[i].xmin+1,W->Spot[i].ymin+2);
         DrawBitmapString(GLUT_BITMAP_8_BY_13,center[j][i-6]);
      }

/* .. Show Box */
      W = &SphereShowWidget;
      DrawWidget(W);

      glRasterPos2i((W->xmin+W->xmax)/2 - 16,W->ymax-14);
      DrawBitmapString(GLUT_BITMAP_8_BY_13,"Show");

      for (i=0; i<6; i++) {
         glRasterPos2i(W->Spot[i].xmin+1,W->Spot[i].ymin+2);
         DrawBitmapString(GLUT_BITMAP_8_BY_13,show[i]);
      }

/* .. Vectors Box */
      W = &VectorsWidget;
      DrawWidget(W);

      glRasterPos2i((W->xmin+W->xmax)/2 - 28,W->ymax-14);
      DrawBitmapString(GLUT_BITMAP_8_BY_13,"Vectors");

      for (i=0; i<6; i++) {
         glRasterPos2i(W->Spot[i].xmin+1,W->Spot[i].ymin+2);
         DrawBitmapString(GLUT_BITMAP_8_BY_13,vectors[i]);
      }

/* .. FOVs Box */
      W = &FOVsWidget;
      DrawWidget(W);

      glRasterPos2i((W->xmin+W->xmax)/2 - 44,W->ymax-14);
      DrawBitmapString(GLUT_BITMAP_8_BY_13,"Sensor FOVs");

      for (i=0; i<3; i++) {
         glRasterPos2i(W->Spot[i].xmin+1,W->Spot[i].ymin+2);
         DrawBitmapString(GLUT_BITMAP_8_BY_13,fovs[i]);
      }

/* .. Grids Box */
      W = &GridsWidget;
      DrawWidget(W);

      glRasterPos2i(W->xmin+4,W->ymin+4);
      DrawBitmapString(GLUT_BITMAP_8_BY_13,"Grids");

      for (i=0; i<3; i++) {
         glRasterPos2i(W->Spot[i].xmin+1,W->Spot[i].ymin+2);
         DrawBitmapString(GLUT_BITMAP_8_BY_13,frames[i]);
      }

/* .. Axes Box */
      W = &AxesWidget;
      DrawWidget(W);

      glRasterPos2i(W->xmin+4,W->ymin+4);
      DrawBitmapString(GLUT_BITMAP_8_BY_13,"Axes");

      for (i=0; i<3; i++) {
         glRasterPos2i(W->Spot[i].xmin+1,W->Spot[i].ymin+2);
         DrawBitmapString(GLUT_BITMAP_8_BY_13,frames[i]);
      }

}
/**********************************************************************/
/* 24 possible cases for body axes orientation in Unit Sphere window -
   calculate appropriate DCM                                          */
void FindSphereWindowAxes(double C[3][3])
{
      struct WidgetType *W;
      double CT[3][3];

      W = &CenterWidget;

      long top,center,index;
      long i;

      long SEQ[24] = {21,21,21,2 ,21,21,2 ,21,
                      1 ,12,12,12,1 ,12,12,12,
                      3 ,3 ,3 ,3 ,1 ,13,13,13};
      double ang1[24] = {-90,-90,-90,-90, 90, 90, 90, 90,
                          90, 90, 90, 90,-90,-90,-90,-90,
                           0,180,-90, 90,180,180,180,180};
      double ang2[24] = {-90, 90,180,  0, 90,-90,  0,180,
                           0,180, 90,-90,  0,180, 90,-90,
                           0,  0,  0,  0,  0,180,-90, 90};

      for (i=0; i<6; i++) {
         if (W->Spot[i].Selected == 1) {
            top = i;
         }
      }

      for (i=6; i<10; i++) {
         if (W->Spot[i].Selected == 1) {
            center = i - 6;
         }
      }

      index = 4*top + center;

      A2C(SEQ[index],ang1[index]*D2R,ang2[index]*D2R,0.0,CT);
      MT(CT,C);

}
/**********************************************************************/
/* Draws a constellation given a constellation struct and DCM         */
void DrawConstellation(struct ConstellationType *C, double CVJ[3][3])
{
      long i,Nstars,Nlines;
      double VecV[3];
      double lngA,latA,lngB,latB;

      Nstars = C->Nstars;
      Nlines = C->Nlines;

      glPointSize(3.0);
      glBegin(GL_POINTS);
      for (i=0; i<Nstars; i++) {
         MxV(CVJ,C->StarVec[i],VecV);
         VecToLngLat(VecV,&lngA,&latA);
         latA *= R2D;
         lngA *= R2D;
         glVertex2f(lngA,latA);
      }
      glEnd();

      glRasterPos2f(lngA,latA);
      DrawBitmapString(GLUT_BITMAP_8_BY_13,C->Tag);

      for (i=0;i<Nlines;i++) {
         MxV(CVJ,C->StarVec[C->Star1[i]],VecV);
         VecToLngLat(VecV,&lngA,&latA);

         MxV(CVJ,C->StarVec[C->Star2[i]],VecV);
         VecToLngLat(VecV,&lngB,&latB);

         DrawMercatorLine(lngA,latA,lngB,latB);
      }
}
/**********************************************************************/
void DrawUnitSphere(void)
{
      double lat,lng;
      long i,j,Im;
      char label[20];
      
      struct FssType *F;
      struct StarTrackerType *ST;

      double CVB0[3][3]; /* DCM from Body0 frame to Viewing frame */
      double CVB[3][3];
      double CVL[3][3];
      double CVN[3][3];
      double CVH[3][3];
      double CVG[3][3];
      double CVS[3][3];
      double CVJ[3][3];
      double Cp[3][3]; /* Permute axes */
      double CVSp[3][3];
      double ZAxis[3] = {0.0,0.0,1.0};
      double x,y;

      double VecH[3],VecN[3],VecV[3];

      struct SCType *S = &SC[POV.Host.SC];
      struct WorldType *Wd = &World[Orb[S->RefOrb].World];
      struct WidgetType *W;

      GLfloat HorizonColor[4] = {0.3,0.6,0.6,1.0};
      GLfloat ValidCSSColor[4] = {0.9,0.95,0.05,1.0};
      GLfloat InvalidCSSColor[4] = {0.45,0.475,0.025,1.0};
      GLfloat ValidFSSColor[4] = {0.95,0.59,0.05,1.0};
      GLfloat InvalidFSSColor[4] = {0.48,0.3,0.025,1.0};
      GLfloat ValidStarTrackerColor[4] = {0.05,0.95,0.95,1.0};
      GLfloat InvalidStarTrackerColor[4] = {0.025,0.48,0.48,1.0};
      GLfloat MagFieldColor[4] = {0.59,0.27,0.05,1.0};
      GLfloat AngMomentumColor[4] = {0.49,0.06,0.58,1.0};
      GLfloat VelocityColor[4] = {0.06,0.88,0.04,1.0};
      GLfloat AntiVelColor[4] = {0.05,0.62,0.03,1.0};
      GLfloat GalacticPlaneColor[4] = {0.5,0.5,0.5,0.5};
      GLfloat ConstellationColor[4] = {0.48,0.69,0.95,1.0};
      GLfloat EclipticNorthColor[4] = {0.93,0.65,0.89,1.0};
      GLfloat GalacticNorthColor[4] = {0.68,0.5,0.75,1.0};

      GLfloat MenuOutlineColor[4] = {0.2,0.2,1.0,1.0};
      GLfloat MenuBackgroundColor[4] = {0.15,0.15,0.3,1.0};

      long MenuTop;
      double rad;
      double rmn[3],rmh[3];
      
      glClear(GL_COLOR_BUFFER_BIT);

      FindSphereWindowAxes(CVB0);
      MxM(CVB0,S->B[0].CN,CVN);
      MxMT(CVN,S->CLN,CVL);
      MxM(CVN,Wd->CNH,CVH);
      MxMT(CVH,CGH,CVG);
      MxMT(CVH,World[EARTH].CNH,CVJ);

/* .. Galactic plane */
      W = &SphereShowWidget;

      if (W->Spot[4].Selected) { /* Galactic plane on */
         glColor4fv(GalacticPlaneColor);
         glLineWidth(2);

         MxV(CVG,ZAxis,VecV);

         VecToLngLat(VecV,&lng,&lat);

         DrawSmallCircle(lng,lat,HalfPi);
         glLineWidth(1);
      }

/* .. Horizon (currently just an outline) */
      W = &SphereShowWidget;

      if (W->Spot[0].Selected) {
         rad = asin(Wd->rad / MAGV(S->PosN));

         for(i=0;i<3;i++) VecN[i] = -S->PosN[i];
         UNITV(VecN);
         MxV(CVN,VecN,VecV);

         VecToLngLat(VecV,&lng,&lat);

         glColor4fv(HorizonColor);
         glLineWidth(2);
         DrawSmallCircle(lng,lat,rad);
         glLineWidth(1);
         DrawMercatorVector(lng,lat,Wd->Name);
      }

/* .. Grids */
      W = &GridsWidget;

      if (W->Spot[0].Selected) { /* B grid */
         glColor4fv(BBrightColor);
         DrawMercatorGrid(CVB0);
      }
      if (W->Spot[1].Selected) { /* N grid */
         glColor4fv(NBrightColor);
         DrawMercatorGrid(CVN);
      }
      if (W->Spot[2].Selected) { /* L grid */
         glColor4fv(LBrightColor);
         DrawMercatorGrid(CVL);
      }

/* .. Sensor FOVs */
      W = &FOVsWidget;

      /* Coarse Sun Sensors */
      if (W->Spot[0].Selected) {
         for (i=0; i<S->Ncss; i++) {
            sprintf(label,"CSS%ld",i);

            MxMT(CVN,S->B[S->CSS[i].Body].CN,CVB);
            MxV(CVB,S->CSS[i].Axis,VecV);
            VecToLngLat(VecV,&lng,&lat);

            if (S->CSS[i].Valid) {
               glColor4fv(ValidCSSColor);
               DrawMercatorVector(lng,lat,label);

               rad = acos(S->CSS[i].Illum/S->CSS[i].Scale);
               DrawSmallCircle(lng,lat,rad);
            }
            else {
               glColor4fv(InvalidCSSColor);
               DrawMercatorVector(lng,lat,label);
            }
         }
      }

      /* Fine Sun Sensors */
      if (W->Spot[1].Selected) {
         for (i=0; i<S->Nfss; i++) {
            F = &S->FSS[i];
            sprintf(label,"FSS%ld",i);

            if (F->Valid) glColor4fv(ValidFSSColor);
            else glColor4fv(InvalidFSSColor);

            MxMT(CVB0,F->CB,CVS);
            Cp[F->BoreAxis][0] = 0.0;
            Cp[F->BoreAxis][1] = 0.0;
            Cp[F->BoreAxis][2] = 1.0;
            Cp[F->H_Axis][0] = 1.0;
            Cp[F->H_Axis][1] = 0.0;
            Cp[F->H_Axis][2] = 0.0;
            Cp[F->V_Axis][0] = 0.0;
            Cp[F->V_Axis][1] = 1.0;
            Cp[F->V_Axis][2] = 0.0;
            MxM(CVS,Cp,CVSp);
            DrawMercatorSquare(CVSp,F->FovHalfAng);

            MxV(CVSp,ZAxis,VecV);
            VecToLngLat(VecV,&lng,&lat);

            DrawMercatorVector(lng,lat,label);
         }
      }

      /* Star Trackers */
      if (W->Spot[2].Selected) {

         for (i=0; i<S->Nst; i++) {
            ST = &S->ST[i];
            sprintf(label,"ST%ld",i);
            if (ST->Valid) glColor4fv(ValidStarTrackerColor);
            else glColor4fv(InvalidStarTrackerColor);

            MxMT(CVB0,ST->CB,CVS);
            Cp[ST->BoreAxis][0] = 0.0;
            Cp[ST->BoreAxis][1] = 0.0;
            Cp[ST->BoreAxis][2] = 1.0;
            Cp[ST->H_Axis][0] = 1.0;
            Cp[ST->H_Axis][1] = 0.0;
            Cp[ST->H_Axis][2] = 0.0;
            Cp[ST->V_Axis][0] = 0.0;
            Cp[ST->V_Axis][1] = 1.0;
            Cp[ST->V_Axis][2] = 0.0;
            MxM(CVS,Cp,CVSp);
            DrawMercatorSquare(CVSp,ST->FovHalfAng);

            MxV(CVSp,ZAxis,VecV);
            VecToLngLat(VecV,&lng,&lat);

            DrawMercatorVector(lng,lat,label);
         }
      }

/* .. Axes */
      W = &AxesWidget;

      /* B Axes */
      if (W->Spot[0].Selected) {
         glColor4fv(BBrightColor);
         DrawMercatorAxes(CVB0,"B");
      }

      /* N Axes */
      if (W->Spot[1].Selected) {
         glColor4fv(NBrightColor);
         DrawMercatorAxes(CVN,"N");
      }

      /* L Axes */
      if (W->Spot[2].Selected) {
         glColor4fv(LBrightColor);
         DrawMercatorAxes(CVL,"L");
      }

/* .. Vectors */
      W = &VectorsWidget;

      /* Magnetic Field */
      if (W->Spot[0].Selected) {
         MxV(CVB0,S->bvb,VecV);
         VecToLngLat(VecV,&lng,&lat);
         glColor4fv(MagFieldColor);
         DrawMercatorVector(lng,lat,"Mag");
      }
      /* Angular Momentum */
      if (W->Spot[1].Selected) {
         MxV(CVB0,S->Hvb,VecV);
         VecToLngLat(VecV,&lng,&lat);
         glColor4fv(AngMomentumColor);
         DrawMercatorVector(lng,lat,"H");
      }
      /* Velocity */
      if (W->Spot[2].Selected) {
         MxV(CVN,S->VelN,VecV);
         VecToLngLat(VecV,&lng,&lat);
         glColor4fv(VelocityColor);
         DrawMercatorVector(lng,lat,"V");
      }
      /* Anti-velocity */
      if (W->Spot[3].Selected) {
         MxV(CVN,S->VelN,VecV);
         for(i=0;i<3;i++) VecV[i] = -VecV[i];
         VecToLngLat(VecV,&lng,&lat);
         glColor4fv(AntiVelColor);
         DrawMercatorVector(lng,lat,"-V");
      }
      /* Ecliptic North */
      if (W->Spot[4].Selected) {
         MxV(CVH,ZAxis,VecV);
         VecToLngLat(VecV,&lng,&lat);
         glColor4fv(EclipticNorthColor);
         DrawMercatorVector(lng,lat,"Ecl N");
      }
      /* Galactic North */
      if (W->Spot[5].Selected) {
         MxV(CVG,ZAxis,VecV);
         VecToLngLat(VecV,&lng,&lat);
         glColor4fv(GalacticNorthColor);
         DrawMercatorVector(lng,lat,"Gal N");
      }

/* .. Sprites */
      W = &SphereShowWidget;

      /* Sun */
      if (W->Spot[1].Selected) {
         MxV(CVB0,S->svb,VecV);
         VecToLngLat(VecV,&lng,&lat);
         lng *= R2D;
         lat *= R2D;
         glEnable(GL_TEXTURE_2D);
         glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
         glBindTexture(GL_TEXTURE_2D,SphereSunSpriteTexTag);
         x=4.0;
         y=x;
         glBegin(GL_QUADS);
            glTexCoord2f(0.0,1.0);
            glVertex2f(lng-x,lat-y);
            glTexCoord2f(1.0,1.0);
            glVertex2f(lng+x,lat-y);
            glTexCoord2f(1.0,0.0);
            glVertex2f(lng+x,lat+y);
            glTexCoord2f(0.0,0.0);
            glVertex2f(lng-x,lat+y);
         glEnd();
         glDisable(GL_TEXTURE_2D);
      }

      /* Moons */
      if (W->Spot[2].Selected) {
         if (Wd->Nsat > 0) {
            glEnable(GL_TEXTURE_2D);
            glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
            glBindTexture(GL_TEXTURE_2D,SphereMoonSpriteTexTag);
            x=4.0;
            y=x;
            for(Im=0;Im<Wd->Nsat;Im++) {
               for (j=0; j<3; j++) {
                  VecN[j] = World[Wd->Sat[Im]].eph.PosN[j] - S->PosN[j];
               }
               MxV(CVN,VecN,VecV);
               VecToLngLat(VecV,&lng,&lat);
               lng *= R2D;
               lat *= R2D;
               glBegin(GL_QUADS);
                  glTexCoord2f(0.0,1.0);
                  glVertex2f(lng-x,lat-y);
                  glTexCoord2f(1.0,1.0);
                  glVertex2f(lng+x,lat-y);
                  glTexCoord2f(1.0,0.0);
                  glVertex2f(lng+x,lat+y);
                  glTexCoord2f(0.0,0.0);
                  glVertex2f(lng-x,lat+y);
               glEnd();
            }
            glDisable(GL_TEXTURE_2D);
         }
         else if (Wd->Type == MOON) { /* Draw Parent Sprite */
            glEnable(GL_TEXTURE_2D);
            glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
            glBindTexture(GL_TEXTURE_2D,SphereMoonSpriteTexTag);
            for(i=0;i<3;i++) rmn[i] = -Wd->eph.PosN[i];
            MTxV(World[Wd->Parent].CNH,rmn,rmh);
            MxV(Wd->CNH,rmh,rmn);
            for(i=0;i<3;i++) VecN[i] = rmn[i] - S->PosN[i];
            MxV(CVN,VecN,VecV);
            VecToLngLat(VecV,&lng,&lat);
            x=4.0;
            y=x;
            glBegin(GL_QUADS);
               glTexCoord2f(0.0,1.0);
               glVertex2f(lng-x,lat-y);
               glTexCoord2f(1.0,1.0);
               glVertex2f(lng+x,lat-y);
               glTexCoord2f(1.0,0.0);
               glVertex2f(lng+x,lat+y);
               glTexCoord2f(0.0,0.0);
               glVertex2f(lng-x,lat+y);
            glEnd();
            glDisable(GL_TEXTURE_2D);
         }
      }

/* .. Planets */
      W = &SphereShowWidget;
      if (W->Spot[3].Selected) {
         for (i=MERCURY;i<=PLUTO;i++) {
            Wd = &World[i];
            if (Wd->Exists && i != POV.Host.World) {
               for (j=0; j<3; j++) {
                  VecH[j] = Wd->PosH[j] - S->PosH[j];
               }
               MxV(CVH,VecH,VecV);
               VecToLngLat(VecV,&lng,&lat);
               glColor4fv(Wd->Color);
               DrawMercatorVector(lng,lat,Wd->Name);
            }
         }
      }

/* .. Constellations */
      W = &SphereShowWidget;

      if (W->Spot[5].Selected) {
         glColor4fv(ConstellationColor);
         for (i=0;i<89;i++) {
            if (ShowConstellations[Constell[i].Class]) {
               if (Constell[i].Nstars > 0)
                  DrawConstellation(&Constell[i],CVJ);
            }
         }
      }

/* .. Menu */
      glMatrixMode(GL_PROJECTION);
      glPushMatrix();
      glLoadIdentity();
      gluOrtho2D(0,SphereWindowWidth,0,SphereWindowHeight);
      glMatrixMode(GL_MODELVIEW);

      glDisable(GL_LINE_SMOOTH);
      glBegin(GL_QUADS);
      glColor4fv(MenuBackgroundColor);

      MenuTop = 16*(NumSphereWindowMenuLines + 2);
      glVertex2i(0,0);
      glVertex2i(SphereWindowWidth,0);
      glVertex2i(SphereWindowWidth,MenuTop);
      glVertex2i(0,MenuTop);

      glEnd();

      glBegin(GL_LINE_LOOP);
      glColor4fv(MenuOutlineColor);

      glVertex2i(4,2);
      glVertex2i(508,2);
      glVertex2i(508,MenuTop-2);
      glVertex2i(4,MenuTop-2);

      glEnd();

      glLineWidth(1);
      DrawSphereHUD();
      glEnable(GL_LINE_SMOOTH);

      glMatrixMode(GL_PROJECTION);
      glPopMatrix();
      glMatrixMode(GL_MODELVIEW);

}
/**********************************************************************/
void InitCamWidgets(void)
{
      GLfloat BorderColor[4] = {0.64,0.32,0.075,1.0};
      GLfloat TextColor[4] = {0.8,0.44,0.0,1.0};
      GLfloat SelectedColor[4] = {1.0,1.0,1.0,0.5};
      GLfloat UnselectedColor[4] = {1.0,1.0,1.0,0.3};
      GLfloat CamShowColor[4] = {0.8,0.44,0.0,1.0};
      GLfloat ClearColor[4] = {0.0,0.0,0.0,0.0};
      long x0,y0;
      unsigned int Max;
      long i;

      ShowHUD = 1;

/* .. POV Widget */
      x0 = 2;
      y0 = 62;
      PovWidget.xmin = x0;
      PovWidget.xmax = x0 + 170;
      PovWidget.ymin = y0;
      PovWidget.ymax = y0 + 90;
      memcpy(PovWidget.BorderColor,BorderColor,4*sizeof(GLfloat));
      memcpy(PovWidget.TextColor,TextColor,4*sizeof(GLfloat));
      memcpy(PovWidget.SelectedColor,SelectedColor,4*sizeof(GLfloat));
      memcpy(PovWidget.UnselectedColor,UnselectedColor,4*sizeof(GLfloat));
      PovWidget.Nspot = 14;
      PovWidget.Spot =
         (struct SpotType *) calloc(PovWidget.Nspot,sizeof(struct SpotType));
      for(i=0;i<PovWidget.Nspot;i++) {
         PovWidget.Spot[i].Visible = 1;
         PovWidget.Spot[i].Selected = 0;
      }
      PovWidget.Spot[POV.Mode].Selected = 1;
      PovWidget.Spot[3+POV.Frame].Selected = 1;
      PovWidget.Spot[8+POV.BoreAxis%3].Selected = 1;
      PovWidget.Spot[11+POV.UpAxis%3].Selected = 1;
      for(i=0;i<3;i++) {
         PovWidget.Spot[i].xmin = x0+130+10*i;
         PovWidget.Spot[i].xmax = x0+138+10*i;
         PovWidget.Spot[i].ymin = y0+22;
         PovWidget.Spot[i].ymax = y0+30;
      }
      for(i=0;i<5;i++) {
         PovWidget.Spot[3+i].xmin = x0+110+10*i;
         PovWidget.Spot[3+i].xmax = x0+118+10*i;
         PovWidget.Spot[3+i].ymin = y0+35;
         PovWidget.Spot[3+i].ymax = y0+45;
      }
      for(i=0;i<3;i++) {
         PovWidget.Spot[8+i].xmin = x0+130+10*i;
         PovWidget.Spot[8+i].xmax = x0+138+10*i;
         PovWidget.Spot[8+i].ymin = y0+52;
         PovWidget.Spot[8+i].ymax = y0+60;
      }
      for(i=0;i<3;i++) {
         PovWidget.Spot[11+i].xmin = x0+130+10*i;
         PovWidget.Spot[11+i].xmax = x0+138+10*i;
         PovWidget.Spot[11+i].ymin = y0+67;
         PovWidget.Spot[11+i].ymax = y0+75;
      }

/* .. Host Widget */
      x0 = PovWidget.xmin;
      y0 = PovWidget.ymax + 2;
      HostWidget.xmin = x0;
      HostWidget.xmax = x0 + 170;
      HostWidget.ymin = y0;
      HostWidget.ymax = y0 + 145;
      memcpy(HostWidget.BorderColor,BorderColor,4*sizeof(GLfloat));
      memcpy(HostWidget.TextColor,TextColor,4*sizeof(GLfloat));
      memcpy(HostWidget.SelectedColor,SelectedColor,4*sizeof(GLfloat));
      memcpy(HostWidget.UnselectedColor,UnselectedColor,4*sizeof(GLfloat));
      HostWidget.Nspot = 9;
      HostWidget.Spot =
         (struct SpotType *) calloc(HostWidget.Nspot,sizeof(struct SpotType));
      for(i=0;i<HostWidget.Nspot;i++) {
         HostWidget.Spot[i].Visible = 1;
         HostWidget.Spot[i].Selected = 0;
      }
      HostWidget.Spot[POV.Host.Type].Selected = 1;
      for(i=0;i<5;i++) {
         HostWidget.Spot[i].xmin = x0+10;
         HostWidget.Spot[i].xmax = x0+18;
         HostWidget.Spot[i].ymin = y0+22+15*i;
         HostWidget.Spot[i].ymax = y0+30+15*i;
      }
      HostWidget.Spot[5].xmin = (HostWidget.xmin+HostWidget.xmax)/2+30;
      HostWidget.Spot[5].xmax = HostWidget.Spot[5].xmin+8;
      HostWidget.Spot[5].ymin = y0+5;
      HostWidget.Spot[5].ymax = y0+15;
      HostWidget.Spot[6].xmax = (HostWidget.xmin+HostWidget.xmax)/2-30;
      HostWidget.Spot[6].xmin = HostWidget.Spot[6].xmax-8;
      HostWidget.Spot[6].ymin = y0+5;
      HostWidget.Spot[6].ymax = y0+15;

      for(i=0;i<2;i++) {
         HostWidget.Spot[7+i].xmin = x0+140+10*i;
         HostWidget.Spot[7+i].xmax = x0+148+10*i;
         HostWidget.Spot[7+i].ymin = y0+110;
         HostWidget.Spot[7+i].ymax = y0+120;
      }

/* .. Target Widget */
      x0 = HostWidget.xmin;
      y0 = HostWidget.ymax + 2;
      TargetWidget.xmin = x0;
      TargetWidget.xmax = x0 + 170;
      TargetWidget.ymin = y0;
      TargetWidget.ymax = y0 + 145;
      memcpy(TargetWidget.BorderColor,BorderColor,4*sizeof(GLfloat));
      memcpy(TargetWidget.TextColor,TextColor,4*sizeof(GLfloat));
      memcpy(TargetWidget.SelectedColor,SelectedColor,4*sizeof(GLfloat));
      memcpy(TargetWidget.UnselectedColor,UnselectedColor,4*sizeof(GLfloat));
      TargetWidget.Nspot = 9;
      TargetWidget.Spot =
         (struct SpotType *) calloc(TargetWidget.Nspot,sizeof(struct SpotType));
      for(i=0;i<TargetWidget.Nspot;i++) {
         TargetWidget.Spot[i].Visible = 1;
         TargetWidget.Spot[i].Selected = 0;
      }
      TargetWidget.Spot[POV.Host.Type].Selected = 1;
      for(i=0;i<5;i++) {
         TargetWidget.Spot[i].xmin = x0+10;
         TargetWidget.Spot[i].xmax = x0+18;
         TargetWidget.Spot[i].ymin = y0+22+15*i;
         TargetWidget.Spot[i].ymax = y0+30+15*i;
      }
      TargetWidget.Spot[5].xmin = (TargetWidget.xmin+TargetWidget.xmax)/2+40;
      TargetWidget.Spot[5].xmax = TargetWidget.Spot[5].xmin+8;
      TargetWidget.Spot[5].ymin = y0+5;
      TargetWidget.Spot[5].ymax = y0+15;
      TargetWidget.Spot[6].xmax = (TargetWidget.xmin+TargetWidget.xmax)/2-40;
      TargetWidget.Spot[6].xmin = TargetWidget.Spot[6].xmax-8;
      TargetWidget.Spot[6].ymin = y0+5;
      TargetWidget.Spot[6].ymax = y0+15;

      for(i=0;i<2;i++) {
         TargetWidget.Spot[7+i].xmin = x0+140+10*i;
         TargetWidget.Spot[7+i].xmax = x0+148+10*i;
         TargetWidget.Spot[7+i].ymin = y0+110;
         TargetWidget.Spot[7+i].ymax = y0+120;
      }

/* .. CamShow Widget */
      Max = 0;
      for(i=0;i<CAM_MENU_SIZE;i++) {
         if (Max < strlen(CamShowLabel[i])) Max = strlen(CamShowLabel[i]);
      }
      x0 = 2;
      y0 = TargetWidget.ymax + 2;
      CamShowWidget.xmin = x0;
      CamShowWidget.xmax = x0 + 30 + 8*Max;
      CamShowWidget.ymin = y0;
      CamShowWidget.ymax = y0 + 25 + 15*CAM_MENU_SIZE;
      memcpy(CamShowWidget.BorderColor,CamShowColor,4*sizeof(GLfloat));
      memcpy(CamShowWidget.TextColor,CamShowColor,4*sizeof(GLfloat));
      memcpy(CamShowWidget.SelectedColor,ClearColor,4*sizeof(GLfloat));
      memcpy(CamShowWidget.UnselectedColor,ClearColor,4*sizeof(GLfloat));
      CamShowWidget.Nspot = CAM_MENU_SIZE;
      CamShowWidget.Spot =
         (struct SpotType *) calloc(CamShowWidget.Nspot,sizeof(struct SpotType));
      for(i=0;i<CamShowWidget.Nspot;i++) {
         CamShowWidget.Spot[i].Visible = 0;
         if (CamShow[i]) CamShowWidget.Spot[i].Selected = 1;
         else CamShowWidget.Spot[i].Selected = 0;
      }
      for(i=0;i<CamShowWidget.Nspot;i++) {
         CamShowWidget.Spot[i].xmin = CamShowWidget.xmin;
         CamShowWidget.Spot[i].xmax = CamShowWidget.xmax;
         CamShowWidget.Spot[i].ymin = y0 + 16 + 15*i;
         CamShowWidget.Spot[i].ymax = y0 + 30 + 15*i;
      }
}
/**********************************************************************/
void InitOrreryWidget(void)
{
      struct WidgetType *W;
      GLfloat BorderColor[4] = {0.118,0.565,1.0,1.0};
      GLfloat TextColor[4] = {0.118,0.565,1.0,1.0};
      GLfloat SelectedColor[4] = {0.141,0.12,0.27,1.0};
      GLfloat UnselectedColor[4] = {0.141,0.12,0.27,1.0};
      long x0,y0;
      long i;

      W = &OrreryWidget;

      x0 = OrreryWidth - 102;
      y0 = OrreryHeight - 122;
      W->xmin = x0;
      W->xmax = x0 + 100;
      W->ymin = y0;
      W->ymax = y0 + 120;
      memcpy(W->BorderColor,BorderColor,4*sizeof(GLfloat));
      memcpy(W->TextColor,TextColor,4*sizeof(GLfloat));
      memcpy(W->SelectedColor,SelectedColor,4*sizeof(GLfloat));
      memcpy(W->UnselectedColor,UnselectedColor,4*sizeof(GLfloat));

      /* Zoom bar = 2 spots. Pan = 4 */
      W->Nspot = 6;
      W->Spot =
         (struct SpotType *) calloc(W->Nspot,sizeof(struct SpotType));
      for(i=0;i<W->Nspot;i++) {
         W->Spot[i].Visible = 1;
         W->Spot[i].Selected = 0;
      }

/* .. Zoom In */
      W->Spot[0].xmin = x0+35;
      W->Spot[0].xmax = x0+65;
      W->Spot[0].ymin = y0+35;
      W->Spot[0].ymax = y0+49;

/* .. Zoom Out */
      W->Spot[1].xmin = x0+35;
      W->Spot[1].xmax = x0+65;
      W->Spot[1].ymin = y0+51;
      W->Spot[1].ymax = y0+65;

/* .. Pan Up */
      W->Spot[2].xmin = x0+40;
      W->Spot[2].xmax = x0+60;
      W->Spot[2].ymin = y0+10;
      W->Spot[2].ymax = y0+30;

/* .. Pan Dn */
      W->Spot[3].xmin = x0+40;
      W->Spot[3].xmax = x0+60;
      W->Spot[3].ymin = y0+70;
      W->Spot[3].ymax = y0+90;

/* .. Pan L */
      W->Spot[4].xmin = x0+10;
      W->Spot[4].xmax = x0+30;
      W->Spot[4].ymin = y0+40;
      W->Spot[4].ymax = y0+60;

/* .. Pan R */
      W->Spot[5].xmin = x0+70;
      W->Spot[5].xmax = x0+90;
      W->Spot[5].ymin = y0+40;
      W->Spot[5].ymax = y0+60;
}
/**********************************************************************/
void InitSphereWidgets(void)
{
      GLfloat BorderColor[4] = {0.64,0.32,0.075,1.0};
      GLfloat TextColor[4] = {0.8,0.44,0.0,1.0};
      GLfloat SelectedColor[4] = {1.0,1.0,1.0,0.3};
      GLfloat UnselectedColor[4] = {1.0,1.0,1.0,0.1};

      long x0,y0;
      long i;

      long xpos[2] = {4,34};
      long ypos[5] = {36,52,68,100,116};
      long Xgrid[3] = {512-82,512-57,512-33};
      long Xaxes[3] = {512-82,512-57,512-33};
      long x,y;

      long NLines = NumSphereWindowMenuLines;

      SphereShowWidget.Nspot = 6;
      VectorsWidget.Nspot = 6;
      FOVsWidget.Nspot = 3; /* Max of NLines - 3 */

/* .. Top/Center Widget */
      x0 = 8;
      y0 = 4;
      CenterWidget.xmin = x0;
      CenterWidget.xmax = x0 + 57;
      CenterWidget.ymin = y0;
      CenterWidget.ymax = y0 + 120;
      memcpy(CenterWidget.BorderColor,BorderColor,4*sizeof(GLfloat));
      memcpy(CenterWidget.TextColor,TextColor,4*sizeof(GLfloat));
      memcpy(CenterWidget.SelectedColor,SelectedColor,4*sizeof(GLfloat));
      memcpy(CenterWidget.UnselectedColor,UnselectedColor,4*sizeof(GLfloat));
      CenterWidget.Nspot = 10;
      CenterWidget.Spot =
         (struct SpotType *) calloc(CenterWidget.Nspot,sizeof(struct SpotType));
      for(i=0;i<CenterWidget.Nspot;i++) {
         CenterWidget.Spot[i].Visible = 1;
         CenterWidget.Spot[i].Selected = 0;
      }

      CenterWidget.Spot[4].Selected = 1;
      CenterWidget.Spot[6].Selected = 1;

      for (i=0; i<CenterWidget.Nspot; i++) {

         x = x0 + xpos[i%2];
         y = CenterWidget.ymax - ypos[i/2];

         CenterWidget.Spot[i].xmin = x;
         CenterWidget.Spot[i].xmax = x+18;
         CenterWidget.Spot[i].ymin = y;
         CenterWidget.Spot[i].ymax = y+13;
      }

/* .. Show Widget */
      x0 = CenterWidget.xmax + 3;
      y0 = 4;
      SphereShowWidget.xmin = x0;
      SphereShowWidget.xmax = x0 + 137;
      SphereShowWidget.ymin = y0;
      SphereShowWidget.ymax = 16*(NLines+2) - 4;
      memcpy(SphereShowWidget.BorderColor,BorderColor,4*sizeof(GLfloat));
      memcpy(SphereShowWidget.TextColor,TextColor,4*sizeof(GLfloat));
      memcpy(SphereShowWidget.SelectedColor,SelectedColor,4*sizeof(GLfloat));
      memcpy(SphereShowWidget.UnselectedColor,UnselectedColor,4*sizeof(GLfloat));

      SphereShowWidget.Spot =
         (struct SpotType *) calloc(SphereShowWidget.Nspot,sizeof(struct SpotType));
      for(i=0;i<SphereShowWidget.Nspot;i++) {
         SphereShowWidget.Spot[i].Visible = 1;
         SphereShowWidget.Spot[i].Selected = 0;
      }

      SphereShowWidget.Spot[0].Selected = 1;

      for (i=0; i<SphereShowWidget.Nspot; i++) {

         x = x0+3;
         y = SphereShowWidget.ymax - 36 - 16*i;

         SphereShowWidget.Spot[i].xmin = x;
         SphereShowWidget.Spot[i].xmax = SphereShowWidget.xmax - 3;
         SphereShowWidget.Spot[i].ymin = y;
         SphereShowWidget.Spot[i].ymax = y+13;
      }

/* .. Vectors Widget */
      x0 = SphereShowWidget.xmax + 3;
      y0 = 4;
      VectorsWidget.xmin = x0;
      VectorsWidget.xmax = x0 + 140;
      VectorsWidget.ymin = y0;
      VectorsWidget.ymax = 16*(NLines+2) - 4;
      memcpy(VectorsWidget.BorderColor,BorderColor,4*sizeof(GLfloat));
      memcpy(VectorsWidget.TextColor,TextColor,4*sizeof(GLfloat));
      memcpy(VectorsWidget.SelectedColor,SelectedColor,4*sizeof(GLfloat));
      memcpy(VectorsWidget.UnselectedColor,UnselectedColor,4*sizeof(GLfloat));

      VectorsWidget.Spot =
         (struct SpotType *) calloc(VectorsWidget.Nspot,sizeof(struct SpotType));
      for(i=0;i<VectorsWidget.Nspot;i++) {
         VectorsWidget.Spot[i].Visible = 1;
         VectorsWidget.Spot[i].Selected = 0;
      }

      VectorsWidget.Spot[0].Selected = 1;

      for (i=0; i<VectorsWidget.Nspot; i++) {

         x = x0+3;
         y = SphereShowWidget.ymax - 36 - 16*i;

         VectorsWidget.Spot[i].xmin = x;
         VectorsWidget.Spot[i].xmax = VectorsWidget.xmax - 3;
         VectorsWidget.Spot[i].ymin = y;
         VectorsWidget.Spot[i].ymax = y+13;
      }

/* .. FOVs Widget */
      x0 = VectorsWidget.xmax + 3;
      y0 = 52;
      FOVsWidget.xmin = x0;
      FOVsWidget.xmax = 512-8;
      FOVsWidget.ymin = y0;
      FOVsWidget.ymax = 16*(NLines+2) - 4;
      memcpy(FOVsWidget.BorderColor,BorderColor,4*sizeof(GLfloat));
      memcpy(FOVsWidget.TextColor,TextColor,4*sizeof(GLfloat));
      memcpy(FOVsWidget.SelectedColor,SelectedColor,4*sizeof(GLfloat));
      memcpy(FOVsWidget.UnselectedColor,UnselectedColor,4*sizeof(GLfloat));

      FOVsWidget.Spot =
         (struct SpotType *) calloc(FOVsWidget.Nspot,sizeof(struct SpotType));
      for(i=0;i<FOVsWidget.Nspot;i++) {
         FOVsWidget.Spot[i].Visible = 1;
         FOVsWidget.Spot[i].Selected = 0;
      }

      for (i=0; i<FOVsWidget.Nspot; i++) {

         x = x0+3;
         y = SphereShowWidget.ymax - 36 - 16*i;

         FOVsWidget.Spot[i].xmin = x;
         FOVsWidget.Spot[i].xmax = FOVsWidget.xmax - 3;
         FOVsWidget.Spot[i].ymin = y;
         FOVsWidget.Spot[i].ymax = y+13;
      }

/* .. Grids Widget */
      x0 = VectorsWidget.xmax + 3;
      y0 = 28;
      GridsWidget.xmin = x0;
      GridsWidget.xmax = 512-8;
      GridsWidget.ymin = y0;
      GridsWidget.ymax = y0 + 20;
      memcpy(GridsWidget.BorderColor,BorderColor,4*sizeof(GLfloat));
      memcpy(GridsWidget.TextColor,TextColor,4*sizeof(GLfloat));
      memcpy(GridsWidget.SelectedColor,SelectedColor,4*sizeof(GLfloat));
      memcpy(GridsWidget.UnselectedColor,UnselectedColor,4*sizeof(GLfloat));

      GridsWidget.Nspot = 3;
      GridsWidget.Spot =
         (struct SpotType *) calloc(GridsWidget.Nspot,sizeof(struct SpotType));
      for(i=0;i<GridsWidget.Nspot;i++) {
         GridsWidget.Spot[i].Visible = 1;
         GridsWidget.Spot[i].Selected = 0;
      }

      for (i=0; i<3; i++) {
         x = Xgrid[i];
         y = y0+4;

         GridsWidget.Spot[i].xmin = x;
         GridsWidget.Spot[i].xmax = x+10;
         GridsWidget.Spot[i].ymin = y;
         GridsWidget.Spot[i].ymax = y+13;
      }

/* .. Axes Widget */
      x0 = VectorsWidget.xmax + 3;
      y0 = 4;
      AxesWidget.xmin = x0;
      AxesWidget.xmax = 512-8;
      AxesWidget.ymin = y0;
      AxesWidget.ymax = y0 + 20;
      memcpy(AxesWidget.BorderColor,BorderColor,4*sizeof(GLfloat));
      memcpy(AxesWidget.TextColor,TextColor,4*sizeof(GLfloat));
      memcpy(AxesWidget.SelectedColor,SelectedColor,4*sizeof(GLfloat));
      memcpy(AxesWidget.UnselectedColor,UnselectedColor,4*sizeof(GLfloat));

      AxesWidget.Nspot = 3;
      AxesWidget.Spot =
         (struct SpotType *) calloc(AxesWidget.Nspot,sizeof(struct SpotType));
      for(i=0;i<AxesWidget.Nspot;i++) {
         AxesWidget.Spot[i].Visible = 1;
         AxesWidget.Spot[i].Selected = 0;
      }

      AxesWidget.Spot[0].Selected = 1;
      AxesWidget.Spot[2].Selected = 1;

      for (i=0; i<3; i++) {
         x = Xaxes[i];
         y = y0+4;

         AxesWidget.Spot[i].xmin = x;
         AxesWidget.Spot[i].xmax = x+10;
         AxesWidget.Spot[i].ymin = y;
         AxesWidget.Spot[i].ymax = y+13;
      }

}
/**********************************************************************/
void Load3DNoise(void)
{
      FILE *infile;
      long N,i;
      GLubyte *Tex;

      infile = FileOpen("./Model/","Noise3DTex.raw","rb");

      N = 256*256*256*4;
      Tex = (GLubyte *) calloc(N,sizeof(GLubyte));
      for(i=0;i<N;i++) Tex[i] = (GLubyte) fgetc(infile);
      fclose(infile);

      glActiveTexture(GL_TEXTURE0);
      glGenTextures(1,&NoiseTex);
      glBindTexture(GL_TEXTURE_3D,NoiseTex);
      glTexParameteri(GL_TEXTURE_3D,GL_TEXTURE_WRAP_S,GL_REPEAT);
      glTexParameteri(GL_TEXTURE_3D,GL_TEXTURE_WRAP_T,GL_REPEAT);
      glTexParameteri(GL_TEXTURE_3D,GL_TEXTURE_WRAP_R,GL_REPEAT);
      glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

      glTexImage3D(GL_TEXTURE_3D,0,GL_RGBA,256,256,256,0,GL_RGBA,
         GL_UNSIGNED_BYTE,Tex);

      free(Tex);
}
/*********************************************************************/
void LoadCamLists(void)
{
      long Isc,Ib,Iw,Ir;
      double MwAlphaMask[4] = {0.0,0.5,0.5,0.0};

/* .. Load SC Geom Display Lists */
      for(Isc=0;Isc<Nsc;Isc++) {
         for(Ib=0;Ib<SC[Isc].Nb;Ib++) {
            GeomToDisplayLists(&Geom[SC[Isc].B[Ib].GeomTag]);
         }
      }
/* .. Load World Geom Display Lists */
      for(Iw=0;Iw<NWORLD;Iw++) {
         if (World[Iw].GeomTag != 0)
            GeomToDisplayLists(&Geom[World[Iw].GeomTag]);
      }
/* .. Load Region Geom Display Lists */
      for(Ir=0;Ir<Nrgn;Ir++) {
         GeomToDisplayLists(&Geom[Rgn[Ir].GeomTag]);
      }

/* .. Load Stars and Worlds */
      MilkyWayList = LoadMilkyWay("./Model/","MilkyWay.ppm",CGH,1.0,
         MwAlphaMask);
      LoadStars(StarCatFileName,BuckyPf,BuckyNeighbor,StarList,
         1.0);
      LoadSkyGrid(30.0,5.0,0.1*SkyDistance,&MajSkyGridList,&MinSkyGridList);
      FermiSkyList = LoadSkyCube("./Model/","FermiSky",CGH,1.0);
      Load1FGL("1FGL Source Catalog.txt",BuckyPf,BuckyNeighbor,FermiSourceList,1.0);
      LoadEgretCatalog("Egret Source Catalog.txt",BuckyPf,BuckyNeighbor,EgretSourceList,1.0);
      LoadPulsars("Pulsar Catalog.txt",BuckyPf,BuckyNeighbor,PulsarList,1.0);

      SphereList = glGenLists(1);
      glNewList(SphereList,GL_COMPILE);
      #ifndef _USE_SHADERS_
         DrawUnitMercatorSphere(50,100);
      #else
         DrawUnitCubeSphere(32);
      #endif
      glEndList();
}
/**********************************************************************/
GLuint LoadSpectrum(const char *SpectrumName)
{
      FILE *infile;
      double *F,*R,*G,*B,f;
      GLubyte r,g,b,a;
      GLubyte *Tex;
      GLuint TexTag;
      char SearchString[80];
      char line[512];
      long i,N;

      infile = FileOpen("./Model/","Spectrum.txt","r");

      while(!feof(infile)) {
         /* Find desired spectrum by name */
         fgets(line,512,infile);
         sprintf(SearchString,"%s %%ld",SpectrumName);
         if (sscanf(line,SearchString,&N) == 1) {
            F = (double *) calloc(N,sizeof(double));
            R = (double *) calloc(N,sizeof(double));
            G = (double *) calloc(N,sizeof(double));
            B = (double *) calloc(N,sizeof(double));
            /* Read in RGBs */
            for(i=0;i<N;i++) {
               fgets(line,512,infile);
               sscanf(line,"%lf %lf %lf %lf",&F[i],&R[i],&G[i],&B[i]);
            }
         }
      }
      fclose(infile);

/* .. Save into 1-D texture */
      Tex = (GLubyte *) calloc(256*4,sizeof(GLubyte));
      for(i=0;i<256;i++) {
         f = ((double) i)/255.0;
         r = (GLubyte) (LinInterp(F,R,f,N)+0.5);
         Tex[4*i  ] = r;
         g = (GLubyte) (LinInterp(F,G,f,N)+0.5);
         Tex[4*i+1] = g;
         b = (GLubyte) (LinInterp(F,B,f,N)+0.5);
         Tex[4*i+2] = b;
         a = (GLubyte) 255;
         Tex[4*i+3] = a;
      }

      glGenTextures(1,&TexTag);
      glBindTexture(GL_TEXTURE_1D,TexTag);
      glTexParameteri(GL_TEXTURE_1D,GL_GENERATE_MIPMAP,GL_FALSE);
      glTexParameteri(GL_TEXTURE_1D,GL_TEXTURE_WRAP_S,GL_CLAMP);
      glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glTexImage1D(GL_TEXTURE_1D,0,GL_RGBA,256,0,GL_RGBA,
            GL_UNSIGNED_BYTE,Tex);

      free(F);
      free(R);
      free(G);
      free(B);
      free(Tex);
      return(TexTag);
}
/*********************************************************************/
void LoadCamTextures(void)
{
      struct MatlType *M;
      struct WorldType *MB;
      long Im,Iw,Ip,Ib;
      char s[80];

/* .. Load Material Textures */
      for(Im=0;Im<Nmatl;Im++) {
         M = &Matl[Im];
         if (strcmp(M->SpectrumName,"NONE")) {
            M->SpectrumTex = LoadSpectrum(M->SpectrumName);
         }
         if (strcmp(M->ColorTexFileName,"NONE")) {
            M->ColorTex = PpmToTexTag("./Model/",M->ColorTexFileName,
                                       3,GL_REPEAT);
         }
         if (strcmp(M->BumpTexFileName,"NONE")) {
            M->BumpTex = PpmToTexTag("./Model/",M->BumpTexFileName,
                                       3,GL_REPEAT);
         }
      }

      SkyCube = PpmToCubeTag("./World/","StarryEnvMap",3);

      World[SOL].Color[0] = 1.0;
      World[SOL].Color[1] = 1.0;
      World[SOL].Color[2] = 1.0;
      World[SOL].Color[3] = 1.0;

/* .. Load Worlds */
      RockballTexTag = PpmToTexTag("./World/","Rockball.ppm",3,GL_REPEAT);
      RockballColCubeTag = PpmToCubeTag("./World/","RockballCol",3);
      RockballBumpCubeTag = PpmToCubeTag("./World/","RockballBump",3);
      IceballTexTag = PpmToTexTag("./World/","Iceball.ppm",3,GL_REPEAT);
      IceballColCubeTag = PpmToCubeTag("./World/","IceballCol",3);
      NullTexTag = PpmToTexTag("./World/","NullTex.ppm",3,GL_REPEAT);
      NullColCubeTag = PpmToCubeTag("./World/","NullCol",3);
      NullBumpCubeTag = PpmToCubeTag("./World/","NullBump",3);
      NullCloudGlossCubeTag = PpmToCubeTag("./World/","NullCloudGloss",3);
      for(Ip=MERCURY;Ip<=PLUTO; Ip++) {
         if (World[Ip].Exists) {
            Iw = Ip;
            if (!strcmp(World[Iw].MapFileName,"NONE")) { /* strcmp returns 0 if match */
               World[Iw].TexTag = NullTexTag;
               World[Iw].ColCubeTag = NullColCubeTag;
            }
            else if (!strcmp(World[Iw].MapFileName,"Rockball")) {
               World[Iw].TexTag = RockballTexTag;
               World[Iw].ColCubeTag = RockballColCubeTag;
            }
            else if (!strcmp(World[Iw].MapFileName,"Iceball")) {
               World[Iw].TexTag = IceballTexTag;
               World[Iw].ColCubeTag = IceballColCubeTag;
            }
            else {
               sprintf(s,"%sCol",World[Iw].Name);
               World[Iw].TexTag = PpmToTexTag("./World/",World[Iw].MapFileName,3,GL_REPEAT);
               World[Iw].ColCubeTag = PpmToCubeTag("./World/",s,3);
            }
            World[Iw].BumpCubeTag = NullBumpCubeTag;
            World[Iw].CloudGlossCubeTag = NullCloudGlossCubeTag;
            World[Iw].RingTexTag = NullRingTexTag;

            for(Im=0;Im<World[Ip].Nsat;Im++) {
               Iw = World[Ip].Sat[Im];
               if (!strcmp(World[Iw].MapFileName,"NONE")) { /* strcmp returns 0 if match */
                  World[Iw].TexTag = NullTexTag;
                  World[Iw].ColCubeTag = NullColCubeTag;
               }
               else if (!strcmp(World[Iw].MapFileName,"Rockball")) {
                  World[Iw].TexTag = RockballTexTag;
                  World[Iw].ColCubeTag = RockballColCubeTag;
               }
               else if (!strcmp(World[Iw].MapFileName,"Iceball")) {
                  World[Iw].TexTag = IceballTexTag;
                  World[Iw].ColCubeTag = IceballColCubeTag;
               }
               else {
                  sprintf(s,"%sCol",World[Iw].Name);
                  World[Iw].TexTag = PpmToTexTag("./World/",World[Iw].MapFileName,3,GL_REPEAT);
                  World[Iw].ColCubeTag = PpmToCubeTag("./World/",s,3);
               }
               World[Iw].BumpCubeTag = NullBumpCubeTag;
               World[Iw].CloudGlossCubeTag = NullCloudGlossCubeTag;
               World[Iw].RingTexTag = NullRingTexTag;
            }
         }
      }
      for(Ib=0;Ib<Nmb;Ib++) {
         if (World[55+Ib].Exists) {
            MB = &World[55+Ib];
            if (!strcmp(MB->MapFileName,"NONE")) { /* strcmp returns 0 if match */
               MB->TexTag = NullTexTag;
               MB->ColCubeTag = NullColCubeTag;
               MB->BumpCubeTag = NullBumpCubeTag;
               MB->CloudGlossCubeTag = NullCloudGlossCubeTag;
            }
            else if (!strcmp(MB->MapFileName,"Rockball")) {
               MB->TexTag = RockballTexTag;
               MB->ColCubeTag = RockballColCubeTag;
               MB->BumpCubeTag = RockballBumpCubeTag;
               MB->CloudGlossCubeTag = NullCloudGlossCubeTag;
            }
            else if (!strcmp(MB->MapFileName,"Iceball")) {
               MB->TexTag = IceballTexTag;
               MB->ColCubeTag = IceballColCubeTag;
               MB->BumpCubeTag = NullBumpCubeTag;
               MB->CloudGlossCubeTag = NullCloudGlossCubeTag;
            }
            else {
               sprintf(s,"%sCol",MB->Name);
               MB->TexTag = PpmToTexTag("./World/",MB->MapFileName,3,GL_REPEAT);
               MB->ColCubeTag = PpmToCubeTag("./World/",s,3);
               sprintf(s,"%sBump",MB->Name);
               MB->BumpCubeTag = PpmToCubeTag("./World/",s,3);
               MB->CloudGlossCubeTag = NullCloudGlossCubeTag;
            }

            if (!strcmp(MB->ColTexFileName,"NONE")) {
            }
            else {
               MB->ColTexTag = PpmToTexTag("./World/",MB->ColTexFileName,3,GL_REPEAT);
               MB->BumpTexTag = PpmToTexTag("./World/",MB->BumpTexFileName,3,GL_REPEAT);
            }
         }
      }
      World[SATURN].RingTexTag = PpmToRingTexTag("./World/","SaturnRing.ppm");

      glGenTextures(1,&ShadowTexTag);
      glActiveTexture(GL_TEXTURE1);
      glEnable(GL_TEXTURE_2D);
      glBindTexture(GL_TEXTURE_2D,ShadowTexTag);
      glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
      glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
      glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
      glTexImage2D(GL_TEXTURE_2D,0,GL_DEPTH_COMPONENT32,2048,2048,0,
         GL_DEPTH_COMPONENT,GL_FLOAT,NULL);
      glDisable(GL_TEXTURE_2D);
      glActiveTexture(GL_TEXTURE0);

      /* For checkout of particular features */
      /*
      World[EARTH].ColCubeTag = PpmToCubeTag("./World/","MoonlikeCol",3);
      World[EARTH].BumpCubeTag = PpmToCubeTag("./World/","MoonlikeBump",3);
      */
      World[EARTH].BumpCubeTag = PpmToCubeTag("./World/","EarthBump",3);
      World[MARS].BumpCubeTag = PpmToCubeTag("./World/","MarsBump",3);
      World[EARTH].CloudGlossCubeTag = PpmToCubeTag("./World/","EarthCloudGloss",3);

      World[LUNA].ColCubeTag = PpmToCubeTag("./World/","LunaCol",3);
      World[LUNA].BumpCubeTag = PpmToCubeTag("./World/","LunaBump",3);

      NASAWatermarkTexTag = PpmToTexTag("./Model/","NASA_WatermarkAlpha.ppm",4,GL_CLAMP);
      GSFCWatermarkTexTag = PpmToTexTag("./Model/","GSFC_WatermarkAlpha.ppm",4,GL_CLAMP);
      FortyTwoWatermarkTexTag = PpmToTexTag("./Model/","42_WatermarkAlpha.ppm",4,GL_CLAMP);

}
/**********************************************************************/
void LoadShadowMapFBO(void)
{
      struct ShadowFBOType *S;
      GLenum Status;

      S = &ShadowMap;
      glGenFramebuffers(1,(GLuint *) &S->FrameTag);
      glBindFramebuffer(GL_FRAMEBUFFER,S->FrameTag);
      S->Height = 2048;
      S->Width = 2048;

      /* Create Texture */
      glGenTextures(1,(GLuint *) &S->TexTag);
      glBindTexture(GL_TEXTURE_2D,S->TexTag);
      glTexImage2D(GL_TEXTURE_2D,0,GL_DEPTH_COMPONENT32,S->Width,S->Height,0,
         GL_DEPTH_COMPONENT,GL_FLOAT,NULL);
      glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
      glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
      glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
      glBindTexture(GL_TEXTURE_2D,0);

      /* Create Render Buffer */
      glGenRenderbuffers(1,&S->RenderTag);
      glBindRenderbuffer(GL_RENDERBUFFER,S->RenderTag);
      glRenderbufferStorage(GL_RENDERBUFFER,
         GL_RGBA,S->Width,S->Height);
      glBindRenderbuffer(GL_RENDERBUFFER,0);

      /* Attach Texture to Depth Attachment Point */
      glFramebufferTexture2D(GL_FRAMEBUFFER,
         GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D,S->TexTag,0);

      /* Disable Color Buffer */
      glDrawBuffer(GL_NONE);
      glReadBuffer(GL_NONE);

      Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
      if (Status != GL_FRAMEBUFFER_COMPLETE) {
         printf("Error initializing FBO ShadowMap\n");
      }

      /* Redirect rendering to on-screen framebuffer */
      glBindFramebuffer(GL_FRAMEBUFFER,0);
}
/**********************************************************************/
void CreateStarrySkyEnvMap(void)
{
      char face[6][20] = {"PX.ppm","PY.ppm","PZ.ppm",
                          "MX.ppm","MY.ppm","MZ.ppm"};
      char filename[40];
      GLuint FboTag,TexTag;
      GLuint Height,Width;
      GLenum Status;
      float *Tex;
      long If,i;
      double CPH[6][3][3] = {
         {{ 0.0, 1.0, 0.0},
          { 0.0, 0.0, 1.0},
          { 1.0, 0.0, 0.0}}, /* PX */

         {{-1.0, 0.0, 0.0},
          { 0.0, 0.0, 1.0},
          { 0.0, 1.0, 0.0}}, /* PY */

         {{ 1.0, 0.0, 0.0},
          { 0.0, 1.0, 0.0},
          { 0.0, 0.0, 1.0}}, /* PZ */

         {{ 0.0,-1.0, 0.0},
          { 0.0, 0.0, 1.0},
          {-1.0, 0.0, 0.0}}, /* MX */

         {{ 1.0, 0.0, 0.0},
          { 0.0, 0.0, 1.0},
          { 0.0,-1.0, 0.0}}, /* MY */

         {{-1.0, 0.0, 0.0},
          { 0.0, 1.0, 0.0},
          { 0.0, 0.0,-1.0}}  /* MZ */
      };
      double LoS[3];

      Height = 1024;
      Width = 1024;

      Tex = (float *) calloc(Height*Width*3,sizeof(float));

      glGenFramebuffers(1,(GLuint *) &FboTag);
      glBindFramebuffer(GL_FRAMEBUFFER,FboTag);

      /* Create Texture */
      glActiveTexture(GL_TEXTURE0);
      glGenTextures(1,(GLuint *) &TexTag);
      glBindTexture(GL_TEXTURE_2D,TexTag);
      glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,Width,Height,0,
         GL_RGB,GL_UNSIGNED_BYTE,NULL);
      glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
      glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
      glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

      glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,TexTag,0);

      Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
      if (Status != GL_FRAMEBUFFER_COMPLETE) {
         printf("Error initializing StarrySkyEnvMap\n");
      }

      /* Draw each face into framebuffer, save to file */
      glMatrixMode(GL_PROJECTION);
      glPushMatrix();
      glMatrixMode(GL_MODELVIEW);
      glClearColor(0.0,0.0,0.0,1.0);
      for(If=0;If<6;If++) {
         glClear(GL_COLOR_BUFFER_BIT);

         /* Set camera orientation */
         glMatrixMode(GL_PROJECTION);
         glLoadIdentity();
         gluPerspective(90.0,1.0,0.1,10.0);
         glMatrixMode(GL_MODELVIEW);

         /* Transform from View frame to H frame */
         glLoadIdentity();
         RotateL2R(CPH[If]);

         /* Line of Sight, from POV toward back of view frustum */
         for(i=0;i<3;i++) LoS[i] = -CPH[If][2][i];

         /* Draw Milky Way, stars */
         if (0) glCallList(FermiSkyList);
         else if (0) glCallList(MilkyWayList);
         DrawStars(LoS,BuckyPf,StarList);

         /* Save to PPM */
         sprintf(filename,"StarryEnvMap");
         strcat(filename,face[If]);
         glReadPixels(0,0,Width,Height,GL_RGB,GL_FLOAT,Tex);
         TexToPpm("./World/",filename,Height,Width,3,Tex);
      }

      glMatrixMode(GL_PROJECTION);
      glPopMatrix();
      glMatrixMode(GL_MODELVIEW);

      glBindTexture(GL_TEXTURE_2D,0);

      /* Redirect rendering to on-screen framebuffer */
      glBindFramebuffer(GL_FRAMEBUFFER,0);

      glDeleteFramebuffers(1,&FboTag);
      glDeleteTextures(1,&TexTag);
      free(Tex);
}
/**********************************************************************/
#ifdef _USE_SHADERS_
void LoadCamShaders(void)
{
      GLint UniLoc;
      GLfloat Eye3x3[9] = {1.0,0.0,0.0, 0.0,1.0,0.0, 0.0,0.0,1.0};
      GLfloat Eye4x4[16] = {1.0,0.0,0.0,0.0, 0.0,1.0,0.0,0.0,
                            0.0,0.0,1.0,0.0, 0.0,0.0,0.0,1.0};
      size_t StrLen;
      char *ShaderText;

      FileToString("./Kit/Shaders/SunVtx.glsl",&ShaderText,&StrLen);
      SunVtxShader =
         TextToShader(ShaderText,GL_VERTEX_SHADER,"SunVtx");
      free(ShaderText);

      FileToString("./Kit/Shaders/SunFrag.glsl",&ShaderText,&StrLen);
      SunFragShader =
         TextToShader(ShaderText,GL_FRAGMENT_SHADER,"SunFrag");
      free(ShaderText);

      SunShaderProgram = BuildShaderProgram(SunVtxShader,SunFragShader,"Sun");

      FileToString("./Kit/Shaders/WorldVtx.glsl",&ShaderText,&StrLen);
      WorldVtxShader =
         TextToShader(ShaderText,GL_VERTEX_SHADER,"WorldVtx");
      free(ShaderText);

      FileToString("./Kit/Shaders/WorldFrag.glsl",&ShaderText,&StrLen);
      WorldFragShader =
         TextToShader(ShaderText,GL_FRAGMENT_SHADER,"WorldFrag");
      free(ShaderText);

      WorldShaderProgram = BuildShaderProgram(
         WorldVtxShader,WorldFragShader,"World");

      FileToString("./Kit/Shaders/BodyVtx.glsl",&ShaderText,&StrLen);
      BodyVtxShader =
         TextToShader(ShaderText,GL_VERTEX_SHADER,"BodyVtx");
      free(ShaderText);

      FileToString("./Kit/Shaders/BodyFrag.glsl",&ShaderText,&StrLen);
      BodyFragShader =
         TextToShader(ShaderText,GL_FRAGMENT_SHADER,"BodyFrag");
      free(ShaderText);

      BodyShaderProgram =
         BuildShaderProgram(BodyVtxShader,BodyFragShader,"Body");

      glUseProgram(SunShaderProgram);
      UniLoc = glGetUniformLocation(SunShaderProgram,"UnitSunVecE");
      glUniform3f(UniLoc,1.0,0.0,0.0);
      UniLoc = glGetUniformLocation(SunShaderProgram,"CosSunAng");
      glUniform1f(UniLoc,1.0);
      UniLoc = glGetUniformLocation(SunShaderProgram,"CosCoronaAng");
      glUniform1f(UniLoc,1.0);

      glUseProgram(WorldShaderProgram);
      UniLoc = glGetUniformLocation(WorldShaderProgram,"MapTexture");
      glUniform1i(UniLoc,0);
      UniLoc = glGetUniformLocation(WorldShaderProgram,"BumpTexture");
      glUniform1i(UniLoc,1);
      UniLoc = glGetUniformLocation(WorldShaderProgram,"CloudGlossTexture");
      glUniform1i(UniLoc,2);
      UniLoc = glGetUniformLocation(WorldShaderProgram,"RingTexture");
      glUniform1i(UniLoc,3);
      UniLoc = glGetUniformLocation(WorldShaderProgram,"HasAtmo");
      glUniform1i(UniLoc,0);
      UniLoc = glGetUniformLocation(WorldShaderProgram,"UnitWorldVecE");
      glUniform3f(UniLoc,0.0,0.0,1.0);
      UniLoc = glGetUniformLocation(WorldShaderProgram,"CosWorldAng");
      glUniform1f(UniLoc,0.0);
      UniLoc = glGetUniformLocation(WorldShaderProgram,"CosAtmoAng");
      glUniform1f(UniLoc,0.0);
      UniLoc = glGetUniformLocation(WorldShaderProgram,"WorldRad");
      glUniform1f(UniLoc,0.0);
      UniLoc = glGetUniformLocation(WorldShaderProgram,"PosEyeW");
      glUniform3f(UniLoc,0.0,0.0,0.0);
      UniLoc = glGetUniformLocation(WorldShaderProgram,"MagPosEye");
      glUniform1f(UniLoc,0.0);
      UniLoc = glGetUniformLocation(WorldShaderProgram,"CWE");
      glUniformMatrix3fv(UniLoc,1,1,Eye3x3);
      ValidateShaderProgram(WorldShaderProgram,"World");

      glUseProgram(BodyShaderProgram);
      UniLoc = glGetUniformLocation(BodyShaderProgram,"CNE");
      glUniformMatrix3fv(UniLoc,1,1,Eye3x3);
      UniLoc = glGetUniformLocation(BodyShaderProgram,"ShadowMatrix");
      glUniformMatrix4fv(UniLoc,1,1,Eye4x4);
      UniLoc = glGetUniformLocation(BodyShaderProgram,"ColorTexture");
      glUniform1i(UniLoc,0);
      UniLoc = glGetUniformLocation(BodyShaderProgram,"BumpTexture");
      glUniform1i(UniLoc,1);
      UniLoc = glGetUniformLocation(BodyShaderProgram,"EnvironmentMap");
      glUniform1i(UniLoc,2);
      UniLoc = glGetUniformLocation(BodyShaderProgram,"NoiseTexture");
      glUniform1i(UniLoc,3);
      UniLoc = glGetUniformLocation(BodyShaderProgram,"SpectrumTexture");
      glUniform1i(UniLoc,4);
      UniLoc = glGetUniformLocation(BodyShaderProgram,"ShadowMap");
      glUniform1i(UniLoc,5);
      UniLoc = glGetUniformLocation(BodyShaderProgram,"NoiseGain");
      glUniform4f(UniLoc,0.5,0.25,0.125,0.0625);
      UniLoc = glGetUniformLocation(BodyShaderProgram,"NoiseBias");
      glUniform1f(UniLoc,0.0);
      UniLoc = glGetUniformLocation(BodyShaderProgram,"NoiseScale");
      glUniform1f(UniLoc,1.0);
      UniLoc = glGetUniformLocation(BodyShaderProgram,"NoiseAxis");
      glUniform3f(UniLoc,0.0,0.0,1.0);
      UniLoc = glGetUniformLocation(BodyShaderProgram,"NoiseType");
      glUniform1i(UniLoc,0);
      UniLoc = glGetUniformLocation(BodyShaderProgram,"ColorTexEnabled");
      glUniform1i(UniLoc,0);
      UniLoc = glGetUniformLocation(BodyShaderProgram,"BumpTexEnabled");
      glUniform1i(UniLoc,0);
      UniLoc = glGetUniformLocation(BodyShaderProgram,"ReflectEnabled");
      glUniform1i(UniLoc,0);
      UniLoc = glGetUniformLocation(BodyShaderProgram,"NoiseColEnabled");
      glUniform1i(UniLoc,0);
      UniLoc = glGetUniformLocation(BodyShaderProgram,"NoiseBumpEnabled");
      glUniform1i(UniLoc,0);
      UniLoc = glGetUniformLocation(BodyShaderProgram,"ShadowsEnabled");
      glUniform1i(UniLoc,0);
      ValidateShaderProgram(BodyShaderProgram,"Body");

      glUseProgram(0);
}
#endif
/**********************************************************************/
#ifdef _USE_SHADERS_
void LoadMapShaders(void)
{
      GLint TexLoc;
      char *ShaderText;
      size_t StrLen;

      FileToString("./Kit/Shaders/MapVtx.glsl",&ShaderText,&StrLen);
      MapVtxShader =
         TextToShader(ShaderText,GL_VERTEX_SHADER,"MapVtx");
      free(ShaderText);

      FileToString("./Kit/Shaders/MapFrag.glsl",&ShaderText,&StrLen);
      MapFragShader =
         TextToShader(ShaderText,GL_FRAGMENT_SHADER,"MapFrag");
      free(ShaderText);

      MapShaderProgram =
         BuildShaderProgram(MapVtxShader,MapFragShader,"Map");

      glUseProgram(MapShaderProgram);

      TexLoc = glGetUniformLocation(MapShaderProgram,"MapTexture");
      glUniform1i(TexLoc,0);
      MapSunVecLoc = glGetUniformLocation(MapShaderProgram,"SunVec");
      MapCosEclLoc = glGetUniformLocation(MapShaderProgram,"CosEclipse");

      ValidateShaderProgram(MapShaderProgram,"Map");

      glUseProgram(0);

      FileToString("./Kit/Shaders/MoonMapFrag.glsl",&ShaderText,&StrLen);
      MoonMapFragShader =
         TextToShader(ShaderText,GL_FRAGMENT_SHADER,"MoonMapFrag");
      free(ShaderText);

      MoonMapShaderProgram =
         BuildShaderProgram(MapVtxShader,MoonMapFragShader,"MoonMap");

      glUseProgram(MoonMapShaderProgram);

      TexLoc = glGetUniformLocation(MoonMapShaderProgram,"MapTexture");
      glUniform1i(TexLoc,0);
      TexLoc = glGetUniformLocation(MoonMapShaderProgram,"BumpTexture");
      glUniform1i(TexLoc,1);
      MoonMapSunVecLoc = glGetUniformLocation(MoonMapShaderProgram,"SunVec");
      MoonMapCosEclLoc = glGetUniformLocation(MoonMapShaderProgram,"CosEclipse");

      ValidateShaderProgram(MoonMapShaderProgram,"Map");

      glUseProgram(0);
}
#endif
/*********************************************************************/
void ReadGraphicsInpFile(void)
{
      FILE *infile;
      char junk[120],newline;
      char response[120];
      long i;
      char Frame;
      long Host,Target;

/* .. Initialize POV */
      infile = FileOpen(InOutPath,"Inp_Graphics.txt","r");
/* .. 42 Graphics Configuration File */
      fscanf(infile,"%[^\n] %[\n]",junk,&newline);
/* .. GL Output Interval */
      fscanf(infile,"%lf %[^\n] %[\n]",&DTOUTGL,junk,&newline);
      fscanf(infile,"%s %[^\n] %[\n]",StarCatFileName,junk,&newline);
      fscanf(infile,"%s %[^\n] %[\n]",response,junk,&newline);
      MapWindowExists = DecodeString(response);
      fscanf(infile,"%s %[^\n] %[\n]",response,junk,&newline);
      OrreryWindowExists = DecodeString(response);
      fscanf(infile,"%s %[^\n] %[\n]",response,junk,&newline);
      SphereWindowExists = DecodeString(response);
/* .. POV */
      fscanf(infile,"%[^\n] %[\n]",junk,&newline);
      fscanf(infile,"%s %[^\n] %[\n]",response,junk,&newline);
      PauseFlag = DecodeString(response);
      fscanf(infile,"%s %[^\n] %[\n]",response,junk,&newline);
      POV.Mode = DecodeString(response);
      fscanf(infile,"%s %[^\n] %[\n]",response,junk,&newline);
      POV.Host.Type = DecodeString(response);
      fscanf(infile,"%ld %ld %c %[^\n] %[\n]",
         &Host,&POV.Host.Body,&Frame,junk,&newline);
      if (Host >= Nsc || !SC[Host].Exists) {
         printf("POV Host SC %ld doesn't exist.\n",Host);
         exit(1);
      }
      POV.Host.SC = Host;
      POV.Host.RefOrb = SC[Host].RefOrb;
      POV.Host.World = Orb[POV.Host.RefOrb].World;
      POV.Frame = DecodeString(response);
      if (Frame == 'N') POV.Frame = FRAME_N;
      else if (Frame == 'L') POV.Frame = FRAME_L;
      else if (Frame == 'F') POV.Frame = FRAME_F;
      else if (Frame == 'S') POV.Frame = FRAME_S;
      else if (Frame == 'B') POV.Frame = FRAME_B;
      else {
         printf("Nonsense frame in Inp_Graphics.txt\n");
         exit(1);
      }
      fscanf(infile,"%s %[^\n] %[\n]",response,junk,&newline);
      POV.Target.Type = DecodeString(response);
      fscanf(infile,"%ld %ld %c %[^\n] %[\n]",
         &Target,&POV.Target.Body,&Frame,junk,&newline);
      if (Target >= Nsc || !SC[Target].Exists) {
         printf("POV Target SC %ld doesn't exist.\n",Target);
         exit(1);
      }
      POV.Target.SC = Target;
      POV.Target.RefOrb = SC[Target].RefOrb;
      POV.Target.World = Orb[POV.Target.RefOrb].World;
      POV.Frame = DecodeString(response);
      if (Frame == 'N') POV.Frame = FRAME_N;
      else if (Frame == 'L') POV.Frame = FRAME_L;
      else if (Frame == 'F') POV.Frame = FRAME_F;
      else if (Frame == 'S') POV.Frame = FRAME_S;
      else if (Frame == 'B') POV.Frame = FRAME_B;
      else {
         printf("Nonsense frame in Inp_Graphics.txt\n");
         exit(1);
      }
      fscanf(infile,"%s %[^\n] %[\n]",response,junk,&newline);
      POV.BoreAxis = DecodeString(response);
      fscanf(infile,"%s %[^\n] %[\n]",response,junk,&newline);
      POV.UpAxis = DecodeString(response);
      fscanf(infile,"%lf %[^\n] %[\n]",&POV.Range,junk,&newline);
      fscanf(infile,"%lf %[^\n] %[\n]",&POV.Angle,junk,&newline);
      fscanf(infile,"%lf %lf %lf %[^\n] %[\n]",
         &POV.PosB[0],&POV.PosB[1],&POV.PosB[2],junk,&newline);
      fscanf(infile,"%s %[^\n] %[\n]",response,junk,&newline);
      POV.View = DecodeString(response);

/* .. CAM Parameters */
      fscanf(infile,"%[^\n] %[\n]",junk,&newline);
      fscanf(infile,"\"%[^\"]\" %[^\n] %[\n]",CamTitle,junk,&newline);
      fscanf(infile,"%ld %ld %[^\n] %[\n]",
         &CamWidth,&CamHeight,junk,&newline);
      fscanf(infile,"%lf  %[^\n] %[\n]",&MouseScaleFactor,junk,&newline);
      POV.AR = ((double) CamWidth)/((double) CamHeight);
      fscanf(infile,"%lf  %[^\n] %[\n]",&GammaCorrection,junk,&newline);
/* .. Cam Show/Hide */
      fscanf(infile,"%[^\n] %[\n]",junk,&newline);
      for(i=0;i<CAM_MENU_SIZE;i++) {
         fscanf(infile,"%s \"%[^\"]\" %[^\n] %[\n]",response,
            CamShowLabel[i],junk,&newline);
         CamShow[i] = DecodeString(response);
      }
      ShadowsEnabled = CamShow[CAM_SHADOWS];

/* .. MAP Parameters */
      fscanf(infile,"%[^\n] %[\n]",junk,&newline);
      fscanf(infile,"\"%[^\"]\" %[^\n] %[\n]",MapTitle,junk,&newline);
      fscanf(infile,"%ld %ld %[^\n] %[\n]",
         &MapWidth,&MapHeight,junk,&newline);
/* .. Map Show/Hide */
      fscanf(infile,"%[^\n] %[\n]",junk,&newline);
      for(i=0;i<MAP_MENU_SIZE;i++) {
         fscanf(infile,"%s \"%[^\"]\" %[^\n] %[\n]",response,
            MapShowLabel[i],junk,&newline);
         MapShow[i] = DecodeString(response);
      }
/* .. Sphere Window */
      fscanf(infile,"%[^\n] %[\n]",junk,&newline);
      fscanf(infile,"%s %[^\n] %[\n]",response,junk,&newline);
      ShowConstellations[MAJOR_CONSTELL] = DecodeString(response);
      fscanf(infile,"%s %[^\n] %[\n]",response,junk,&newline);
      ShowConstellations[ZODIAC_CONSTELL] = DecodeString(response);
      fscanf(infile,"%s %[^\n] %[\n]",response,junk,&newline);
      ShowConstellations[MINOR_CONSTELL] = DecodeString(response);

      fclose(infile);

}
/*********************************************************************/
void LoadFOVs(void)
{
      FILE *infile;
      char junk[120],newline;
      char response[120],response1[120],response2[120];
      double Ang1,Ang2,Ang3;
      long Seq;
      long i;

      infile = FileOpen(InOutPath,"Inp_FOV.txt","r");
      fscanf(infile,"%[^\n] %[\n]",junk,&newline);
      fscanf(infile,"%ld %[^\n] %[\n]",&Nfov,junk,&newline);
      FOV = (struct FovType *) calloc(Nfov,sizeof(struct FovType));
      for(i=0;i<Nfov;i++) {
         fscanf(infile,"%[^\n] %[\n]",junk,&newline);
         fscanf(infile,"\"%[^\"]\" %[^\n] %[\n]",
            FOV[i].Label,junk,&newline);
         fscanf(infile,"%ld %lf %[^\n] %[\n]",
            &FOV[i].Nv,&FOV[i].Length,junk,&newline);
         fscanf(infile,"%lf %lf %[^\n] %[\n]",
            &FOV[i].Width,&FOV[i].Height,junk,&newline);
         if (FOV[i].Width >= 180.0) {
            printf("FOV[%ld] Width >= 180 deg.  This is not allowed.  Bailing out.\n",i);
            exit(1);
         }
         if (FOV[i].Height >= 180.0) {
            printf("FOV[%ld] Height >= 180 deg.  This is not allowed.  Bailing out.\n",i);
            exit(1);
         }
         FOV[i].Width *= D2R;
         FOV[i].Height *= D2R;
         fscanf(infile,"%f %f %f %f %[^\n] %[\n]",
            &FOV[i].Color[0],&FOV[i].Color[1],&FOV[i].Color[2],&FOV[i].Color[3],
            junk,&newline);
         fscanf(infile,"%s %[^\n] %[\n]",response,junk,&newline);
         FOV[i].Type = DecodeString(response);
         fscanf(infile,"%s %s %[^\n] %[\n]",
            response1,response2,junk,&newline);
         FOV[i].NearExists = DecodeString(response1);
         FOV[i].FarExists = DecodeString(response2);
         fscanf(infile,"%ld %ld %[^\n] %[\n]",
            &FOV[i].SC,&FOV[i].Body,junk,&newline);
         if (FOV[i].SC >= Nsc) {
            printf("FOV[%ld].SC is out of range.\n",i);
            exit(1);
         }
         if (SC[FOV[i].SC].Exists && FOV[i].Body >= SC[FOV[i].SC].Nb) {
            printf("FOV[%ld].Body is out of range.\n",i);
            exit(1);
         }
         FOV[i].RefOrb = SC[FOV[i].SC].RefOrb;
         if (!SC[FOV[i].SC].Exists) {
            FOV[i].NearExists = FALSE;
            FOV[i].FarExists = FALSE;
         }

         fscanf(infile,"%lf %lf %lf %[^\n] %[\n]",
            &FOV[i].pb[0],&FOV[i].pb[1],&FOV[i].pb[2],junk,&newline);
         fscanf(infile,"%lf %lf %lf %ld %[^\n] %[\n]",
            &Ang1,&Ang2,&Ang3,&Seq,junk,&newline);
            A2C(Seq,Ang1*D2R,Ang2*D2R,Ang3*D2R,FOV[i].CB);
         fscanf(infile,"%s %[^\n] %[\n]",response,junk,&newline);
         FOV[i].BoreAxis = DecodeString(response);
         FOV[i].H_Axis = (FOV[i].BoreAxis+1)%3;
         FOV[i].V_Axis = (FOV[i].BoreAxis+2)%3;
      }
      fclose(infile);
}
/*********************************************************************/
void InitColors(void)
{
      long i;


      DistantDiffuseLightColor[0] = 0.95;
      DistantDiffuseLightColor[1] = 0.95;
      DistantDiffuseLightColor[2] = 0.95;
      DistantDiffuseLightColor[3] = 1.0;

      for(i=0;i<3;i++)
         DistantAmbientLightColor[i] = 1.0-DistantDiffuseLightColor[i];
      DistantAmbientLightColor[3] = 1.0;

      LocalDiffuseLightColor[0] = 0.5;
      LocalDiffuseLightColor[1] = 0.5;
      LocalDiffuseLightColor[2] = 0.5;
      LocalDiffuseLightColor[3] = 1.0;

      for(i=0;i<3;i++)
         LocalAmbientLightColor[i] = 1.0-LocalDiffuseLightColor[i];
      LocalAmbientLightColor[3] = 1.0;

      SpecularLightColor[0] = 1.0;
      SpecularLightColor[1] = 1.0;
      SpecularLightColor[2] = 1.0;
      SpecularLightColor[3] = 1.0;

      NBrightColor[0] = 1.0;
      NBrightColor[1] = 0.0;
      NBrightColor[2] = 0.0;
      NBrightColor[3] = 1.0;

      NDimColor[0] = 0.3;
      NDimColor[1] = 0.0;
      NDimColor[2] = 0.0;
      NDimColor[3] = 1.0;

      LBrightColor[0] = 1.0;
      LBrightColor[1] = 0.3;
      LBrightColor[2] = 1.0;
      LBrightColor[3] = 1.0;

      LDimColor[0] = 0.3;
      LDimColor[1] = 0.1;
      LDimColor[2] = 0.3;
      LDimColor[3] = 1.0;

      FBrightColor[0] = 0.3;
      FBrightColor[1] = 0.3;
      FBrightColor[2] = 1.0;
      FBrightColor[3] = 1.0;

      FDimColor[0] = 0.1;
      FDimColor[1] = 0.1;
      FDimColor[2] = 0.3;
      FDimColor[3] = 1.0;

      BBrightColor[0] = 0.0;
      BBrightColor[1] = 1.0;
      BBrightColor[2] = 0.0;
      BBrightColor[3] = 1.0;

      BDimColor[0] = 0.0;
      BDimColor[1] = 0.3;
      BDimColor[2] = 0.0;
      BDimColor[3] = 1.0;

      GBrightColor[0] = 0.0;
      GBrightColor[1] = 210.0/255.0;
      GBrightColor[2] = 1.0;
      GBrightColor[3] = 1.0;

      GDimColor[0] = 0.0;
      GDimColor[1] = 0.4*210.0/255.0;
      GDimColor[2] = 0.4*255.0/255.0;
      GDimColor[3] = 1.0;

      HUDColor[0] = 0.7;
      HUDColor[1] = 0.5;
      HUDColor[2] = 0.2;
      HUDColor[3] = 1.0;

      TdrsColor[0] = 0.118;
      TdrsColor[1] = 0.565;
      TdrsColor[2] = 1.0;
      TdrsColor[3] = 1.0;
}
/* #ifdef __cplusplus
** }
** #endif
*/

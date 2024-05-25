/*    This file is distributed with 42,                               */
/*    the (mostly harmless) spacecraft dynamics simulation            */
/*    created by Eric Stoneking of NASA Goddard Space Flight Center   */

/*    Copyright 2010 United States Government                         */
/*    as represented by the Administrator                             */
/*    of the National Aeronautics and Space Administration.           */

/*    No copyright is claimed in the United States                    */
/*    under Title 17, U.S. Code.                                      */

/*    All Other Rights Reserved.                                      */

/* GPGPU stands for "General Purpose computing                        */
/* on Graphics Processing Units".                                     */
/* We can leverage the parallelism of GPU processing, even if the     */
/* end result isn't pixels on the screen.                             */

#include "42.h"
#define EXTERN extern
#include "glkit.h"
#undef EXTERN

GLuint EarthAlbedoCubeTag;
GLuint GenericAlbedoCubeTag;
GLuint AlbedoWindow;


/**********************************************************************/
/* GLUT wants a display callback for AlbedoWindow */
void NullDisplay(void) {}
/**********************************************************************/
void InitAlbedo(void)
{
      size_t StrLen;
      char *ShaderText;
      GLint UniLoc;
      GLenum Status;
      GLfloat Eye3x3[9] = {1.0,0.0,0.0, 0.0,1.0,0.0, 0.0,0.0,1.0};
      struct AlbedoFBOType *A;
      GLuint Width = 64; /* Must be power of 2 */
      
      glutInitWindowSize(Width,Width);
      AlbedoWindow = glutCreateWindow("Albedo Workspace");
      glutSetWindow(AlbedoWindow);
      glViewport(0,0,Width,Width);
      glutDisplayFunc(NullDisplay);
      glEnable(GL_BLEND);
      glBlendFunc(GL_ONE,GL_ONE); 

/* .. Create FBO */
      A = &AlbedoFBO;
      glGenFramebuffers(1,(GLuint *) &A->FrameTag);
      glBindFramebuffer(GL_FRAMEBUFFER,A->FrameTag);
      A->Height = Width;
      A->Width = Width;
      A->Tex = (float *) calloc(A->Height*A->Width*3,sizeof(float));

      /* Create Textures */
      glGenTextures(1,(GLuint *) &A->TexTag);
      glBindTexture(GL_TEXTURE_2D,A->TexTag);
      glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,A->Width,A->Height,0,
         GL_RGBA,GL_FLOAT,NULL);
      glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
      glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
      glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
         
      /* Create Render Buffer */
      glGenRenderbuffers(1,&A->RenderTag);
      glBindRenderbuffer(GL_RENDERBUFFER,A->RenderTag);
      glRenderbufferStorage(GL_RENDERBUFFER,
         GL_RGBA,A->Width,A->Height);

      glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,A->TexTag,0);

      Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
      if (Status != GL_FRAMEBUFFER_COMPLETE) {
         printf("Error initializing Albedo FBO\n");
      }
      
      glBindTexture(GL_TEXTURE_2D,0);
      glBindRenderbuffer(GL_RENDERBUFFER,0);
      glBindFramebuffer(GL_FRAMEBUFFER,0);
      
/* .. Load Albedo Textures */
      EarthAlbedoCubeTag = PpmToCubeTag("./World/","TOMS_Albedo",3); 
      GenericAlbedoCubeTag = PpmToCubeTag("./World/","WhiteBall",3);

/* .. Load Shaders */
      FileToString("./Kit/Shaders/AlbedoVtx.glsl",&ShaderText,&StrLen);
      AlbedoVtxShader = TextToShader(ShaderText,GL_VERTEX_SHADER,"AlbedoVtx");
      free(ShaderText);

      FileToString("./Kit/Shaders/AlbedoFrag.glsl",&ShaderText,&StrLen);
      AlbedoFragShader = TextToShader(ShaderText,GL_FRAGMENT_SHADER,"AlbedoFrag");
      free(ShaderText);

      AlbedoShaderProgram = 
         BuildShaderProgram(AlbedoVtxShader,AlbedoFragShader,"Albedo"); 

      glUseProgram(AlbedoShaderProgram);
      UniLoc = glGetUniformLocation(AlbedoShaderProgram,"UnitWorldVecE");
      glUniform3f(UniLoc,0.0,0.0,1.0);
      UniLoc = glGetUniformLocation(AlbedoShaderProgram,"SunVecE");
      glUniform3f(UniLoc,0.0,0.0,1.0);
      UniLoc = glGetUniformLocation(AlbedoShaderProgram,"CosWorldAng");
      glUniform1f(UniLoc,0.0);
      UniLoc = glGetUniformLocation(AlbedoShaderProgram,"CosFov");
      glUniform1f(UniLoc,0.0);
      UniLoc = glGetUniformLocation(AlbedoShaderProgram,"WorldRad");
      glUniform1f(UniLoc,0.0);      
      UniLoc = glGetUniformLocation(AlbedoShaderProgram,"PosEyeW");
      glUniform3f(UniLoc,0.0,0.0,0.0);
      UniLoc = glGetUniformLocation(AlbedoShaderProgram,"CEF");
      glUniformMatrix3fv(UniLoc,1,1,Eye3x3);
      UniLoc = glGetUniformLocation(AlbedoShaderProgram,"CWE");
      glUniformMatrix3fv(UniLoc,1,1,Eye3x3);
      UniLoc = glGetUniformLocation(AlbedoShaderProgram,"TexWidth");
      glUniform1f(UniLoc,64.0);

      ValidateShaderProgram(AlbedoShaderProgram,"Albedo");

      glUseProgram(0);
      
}
/**********************************************************************/
/* Significant contributions to this algorithm were made by           */
/* Daniel Wilkinson, summer intern 2020, and                          */
/* Emily Ballantyne, summer intern 2021.                              */
void FindAlbedo(struct SCType *S, struct CssType *CSS) 
{
      struct BodyType *B;
      struct WorldType *W;
      struct AlbedoFBOType *A;
      double CEB[3][3],CEN[3][3],CWE[3][3],PosEyeW[3],WorldVecN[3],WorldDist;
      double UnitWorldVecE[3],CosWorldAng;
      double SunVecE[3];
      float TexWidth;
      unsigned int AlbedoCubeTag;
      GLint UniLoc;
      GLfloat CWEarray[9],CEFarray[9];
      long i,j,If;
      static long First = 1;
      
      /* E is "eye" frame, F is "face" frame */
      /* Faces are faces of a cube centered on Eye */
      double CEF[5][3][3] = {
         /* Ceiling */
         {{1.0,0.0,0.0},{0.0,1.0,0.0},{0.0,0.0,1.0}},
         /* North Wall */
         {{1.0,0.0,0.0},{0.0,0.0,-1.0},{0.0,1.0,0.0}},
         /* South Wall */
         {{1.0,0.0,0.0},{0.0,0.0,1.0},{0.0,-1.0,0.0}},
         /* East Wall */
         {{0.0,0.0,-1.0},{0.0,1.0,0.0},{1.0,0.0,0.0}},
         /* West Wall */
         {{0.0,0.0,1.0},{0.0,1.0,0.0},{-1.0,0.0,0.0}}
      };
      
      if (First) {
         First = 0;
         InitAlbedo();
      }
      glutSetWindow(AlbedoWindow);

      B = &S->B[CSS->Body];
      W = &World[Orb[S->RefOrb].World];
      A = &AlbedoFBO;
      
/* .. Setup geometry for Shader */
      for(i=0;i<3;i++) CEB[2][i] = -CSS->Axis[i];
      PerpBasis(CEB[2],CEB[0],CEB[1]);
      MxM(CEB,B->CN,CEN);
      MxMT(W->CWN,CEN,CWE);
      MxV(W->CWN,S->PosN,PosEyeW);
      for (i=0;i<3;i++) WorldVecN[i] = -S->PosN[i];
      WorldDist = UNITV(WorldVecN);
      MxV(CEN,WorldVecN,UnitWorldVecE);
      MxV(CEN,S->svn,SunVecE);
      CosWorldAng = sqrt(1.0-W->rad*W->rad/(WorldDist*WorldDist));
      
      if (Orb[S->RefOrb].World == EARTH) AlbedoCubeTag = EarthAlbedoCubeTag;
      else AlbedoCubeTag = GenericAlbedoCubeTag;
      
      CSS->Albedo = 0.0;
      
      glMatrixMode(GL_PROJECTION);
      glPushMatrix();
      glLoadIdentity();
      gluPerspective(90.0, 1.0, 0.1, 10.0); 
      glMatrixMode(GL_MODELVIEW);
      glPushMatrix();
      glLoadIdentity();
   
      glUseProgram(AlbedoShaderProgram);

      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_CUBE_MAP,AlbedoCubeTag);
      glActiveTexture(GL_TEXTURE1);
      glBindTexture(GL_TEXTURE_CUBE_MAP,W->CloudGlossCubeTag);

      UniLoc = glGetUniformLocation(AlbedoShaderProgram,"UnitWorldVecE");
      glUniform3f(UniLoc,UnitWorldVecE[0],UnitWorldVecE[1],UnitWorldVecE[2]);

      UniLoc = glGetUniformLocation(AlbedoShaderProgram,"SunVecE");
      glUniform3f(UniLoc,SunVecE[0],SunVecE[1],SunVecE[2]);

      UniLoc = glGetUniformLocation(AlbedoShaderProgram,"CosWorldAng");
      glUniform1f(UniLoc,CosWorldAng);

      UniLoc = glGetUniformLocation(AlbedoShaderProgram,"CosFov");
      glUniform1f(UniLoc,CSS->CosFov);

      UniLoc = glGetUniformLocation(AlbedoShaderProgram,"WorldRad");
      glUniform1f(UniLoc,W->rad);      

      UniLoc = glGetUniformLocation(AlbedoShaderProgram,"PosEyeW");
      glUniform3f(UniLoc,PosEyeW[0],PosEyeW[1],PosEyeW[2]);
      
      UniLoc = glGetUniformLocation(AlbedoShaderProgram,"CWE");
      for(i=0;i<3;i++) {
        for(j=0;j<3;j++) CWEarray[3*i+j] = CWE[i][j];
      }
      glUniformMatrix3fv(UniLoc,1,1,CWEarray);
   
      UniLoc = glGetUniformLocation(AlbedoShaderProgram,"TexWidth");
      TexWidth = ((float) A->Width);
      glUniform1f(UniLoc,TexWidth);

      glBindFramebuffer(GL_FRAMEBUFFER,A->FrameTag);
      glBindRenderbuffer(GL_RENDERBUFFER,A->RenderTag);

      glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,A->TexTag,0);

      /* Accumulate renders, read out only once */      
      glClear(GL_COLOR_BUFFER_BIT); 
      for(If=0;If<5;If++) {
         UniLoc = glGetUniformLocation(AlbedoShaderProgram,"CEF");
         for(i=0;i<3;i++) {
           for(j=0;j<3;j++) CEFarray[3*i+j] = CEF[If][i][j];
         }
         glUniformMatrix3fv(UniLoc,1,1,CEFarray);

         /* Draw to FBO */
         glBegin(GL_QUADS);
            glVertex3d(-1.0,-1.0,-1.0);
            glVertex3d( 1.0,-1.0,-1.0);
            glVertex3d( 1.0, 1.0,-1.0);
            glVertex3d(-1.0, 1.0,-1.0);
         glEnd();
         
      }
      /* Readout from FBO */
      /* glReadPixels is bottleneck.  To optimize, consider processing all CSS's */
      /* at once, then have only one glReadPixels call to retrieve all Albedo */
      glReadPixels(0,0,A->Width,A->Height,GL_RGBA,GL_FLOAT,A->Tex);
      for(j=0;j<A->Height;j++) {
         for(i=0;i<A->Width;i++) {
            CSS->Albedo += A->Tex[4*(A->Width*j+i)]; /* Scaled in shader */
         }
      }

      glBindRenderbuffer(GL_RENDERBUFFER,0);
      glBindFramebuffer(GL_FRAMEBUFFER,0);
      glUseProgram(0);
      glMatrixMode(GL_PROJECTION);
      glPopMatrix();
      glMatrixMode(GL_MODELVIEW);
      glPopMatrix();
      
      
}

/*    This file is distributed with 42,                               */
/*    the (mostly harmless) spacecraft dynamics simulation            */
/*    created by Eric Stoneking of NASA Goddard Space Flight Center   */

/*    Copyright 2010 United States Government                         */
/*    as represented by the Administrator                             */
/*    of the National Aeronautics and Space Administration.           */

/*    No copyright is claimed in the United States                    */
/*    under Title 17, U.S. Code.                                      */

/*    All Other Rights Reserved.                                      */


/**********************************************************************/
/**********************************************************************/
/**********************************************************************/
/*  World Vertex Shader                                               */

GLchar WorldVtxText[] = {"                                             \
                                                                       \
varying float RingTexCoord;                                            \
varying vec3 PosVec;                                                   \
varying vec3 LightVec;                                                 \
                                                                       \
void main(void)                                                        \
{                                                                      \
      float k;                                                         \
                                                                       \
      gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;          \
                                                                       \
      PosVec = normalize(gl_Normal);                                   \
      LightVec = normalize(vec3(                                       \
         gl_ModelViewMatrixInverse*gl_LightSource[0].position));       \
      k = step(0.0,-PosVec.z*LightVec.z)                               \
         *(-PosVec.z/LightVec.z);                                      \
      RingTexCoord =                                                   \
         clamp(length(vec2(PosVec+k*LightVec))-1.5,-0.5,10.0);         \
}                                                                      \
"};

/**********************************************************************/
/*  World Fragment Shader                                             */

GLchar WorldFragText[] = {"                                            \
                                                                       \
uniform samplerCube MapTexture;                                        \
uniform samplerCube BumpTexture;                                       \
uniform samplerCube CloudGlossTexture;                                 \
uniform sampler1D RingTexture;                                         \
uniform vec3 PovPosW;                                                  \
uniform bool HazeEnabled;                                              \
                                                                       \
varying float RingTexCoord;                                            \
varying vec3 PosVec;                                                   \
varying vec3 LightVec;                                                 \
                                                                       \
void main(void)                                                        \
{                                                                      \
      vec4 Ring;                                                       \
      float RingCoord;                                                 \
      float DiffIllum;                                                 \
      float SpecIllum;                                                 \
      vec3 HalfVec;                                                    \
      float NoH;                                                       \
      float Gloss;                                                     \
      vec4 HazeColor = vec4(0.7,0.7,0.7,1.0);                          \
      float HazeFrac;                                                  \
                                                                       \
/* .. Illumination */                                                  \
      /* Normal Vector */                                              \
      vec3 Normal =                                                    \
         normalize(PosVec+vec3(textureCube(BumpTexture,PosVec))-0.5);  \
      vec2 CloudGloss = vec2(textureCube(CloudGlossTexture,PosVec));   \
      /* Check for Ring Shadow */                                      \
      RingCoord = RingTexCoord;                                        \
      Ring = texture1D(RingTexture,RingCoord);                         \
      /* Diffuse Illumination */                                       \
      DiffIllum = smoothstep(0.0,1.0,                                  \
         1.0*dot(Normal,LightVec)*(1.0-Ring.a));                       \
      /* Specular Illumination */                                      \
      HalfVec = normalize(vec3(                                        \
         gl_ModelViewMatrixInverse*gl_LightSource[0].halfVector));     \
      Gloss = DiffIllum*CloudGloss.g;                                  \
      NoH = clamp(dot(Normal,HalfVec),0.0,1.0);                        \
      SpecIllum = Gloss*pow(NoH,gl_FrontMaterial.shininess);           \
                                                                       \
/* .. Perturb Material if Desired */                                   \
      vec4 MapColor =                                                  \
         vec4(vec3(textureCube(MapTexture,PosVec)),1.0);               \
                                                                       \
/* .. Atmospheric Haze */                                              \
      if (HazeEnabled) {                                               \
         HazeFrac = dot(normalize(PovPosW-PosVec),PosVec);             \
         MapColor = HazeFrac*MapColor + (1.0-HazeFrac)*HazeColor;      \
      }                                                                \
                                                                       \
/* .. Compute Colors */                                                \
      /* Sum Ambient with Diffuse Term */                              \
      vec4 Diffuse = gl_FrontLightModelProduct.sceneColor;             \
      Diffuse.a = 1.0;                                                 \
      vec4 Spec = vec4(0.0,0.0,0.0,1.0);                               \
      /* Primary Color */                                              \
      Diffuse += DiffIllum*gl_LightSource[0].diffuse;                  \
      /* Secondary Color */                                            \
      Spec += SpecIllum*gl_LightSource[0].specular;                    \
      /* Ground Color */                                               \
      vec4 GndColor = mix(Diffuse*MapColor,                            \
         Spec*gl_FrontMaterial.specular,0.2);                          \
                                                                       \
      /* Attenuate through Clouds */                                   \
      gl_FragColor = mix(GndColor,Diffuse,CloudGloss.r);               \
}                                                                      \
"};
/**********************************************************************/
/**********************************************************************/
/**********************************************************************/
/*  Ring Vertex Shader                                                */

GLchar RingVtxText[] = {"                                              \
                                                                       \
varying vec3 PosVec;                                                   \
varying vec3 LightVec;                                                 \
                                                                       \
void main(void)                                                        \
{                                                                      \
      gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;          \
                                                                       \
      gl_TexCoord[0] = gl_TextureMatrix[0]*gl_MultiTexCoord0;          \
                                                                       \
      PosVec = vec3(gl_Vertex);                                        \
      LightVec = normalize(vec3(                                       \
         gl_ModelViewMatrixInverse*gl_LightSource[0].position));       \
}                                                                      \
"};

/**********************************************************************/
/*  Ring Fragment Shader                                              */

GLchar RingFragText[] = {"                                             \
                                                                       \
uniform sampler1D RingTexture;                                         \
varying vec3 PosVec;                                                   \
varying vec3 LightVec;                                                 \
                                                                       \
void main(void)                                                        \
{                                                                      \
      float AoL;                                                       \
      vec3 ProjVec;                                                    \
                                                                       \
      vec4 RingColor =                                                 \
         texture1D(RingTexture,gl_TexCoord[0].s);                      \
      vec4 DiffIllum = gl_FrontLightModelProduct.sceneColor;           \
      DiffIllum.a = 1.0;                                               \
      AoL = dot(PosVec,LightVec);                                      \
      ProjVec = PosVec-AoL*LightVec;                                   \
      DiffIllum += max(step(1.0,dot(ProjVec,ProjVec)),                 \
         step(0.0,AoL))*gl_LightSource[0].diffuse;                     \
      gl_FragColor = RingColor*clamp(DiffIllum,0.0,1.0);               \
}                                                                      \
"};
/**********************************************************************/
/**********************************************************************/
/**********************************************************************/
/*  Body Vertex Shader                                                */

GLchar BodyVtxText[] = {"                                              \
                                                                       \
uniform mat3 CNE;                                                      \
uniform mat4 ShadowMatrix;                                             \
varying mat3 CEB;                                                      \
varying vec3 PosVecB;                                                  \
varying vec3 LightVecE;                                                \
varying vec3 NormalVecE;                                               \
varying vec3 EyeVecE;                                                  \
varying vec3 NormalVecN;                                               \
varying vec3 EyeVecN;                                                  \
                                                                       \
varying vec2 ColorTexCoord;                                            \
varying vec4 ShadowCoord;                                              \
                                                                       \
void main(void)                                                        \
{                                                                      \
      gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;          \
      PosVecB = vec3(gl_Vertex);                                       \
      LightVecE = normalize(vec3(gl_LightSource[0].position));         \
      EyeVecE = -normalize(vec3(gl_ModelViewMatrix * gl_Vertex));      \
      EyeVecN = CNE*EyeVecE;                                           \
      CEB = gl_NormalMatrix;                                           \
      NormalVecE = normalize(CEB*gl_Normal);                           \
      NormalVecN = normalize(CNE*NormalVecE);                          \
                                                                       \
      ColorTexCoord = gl_MultiTexCoord0.st;                            \
      ShadowCoord = ShadowMatrix*gl_Vertex;                            \
      ShadowCoord = ShadowCoord/ShadowCoord.w;                         \
                                                                       \
}                                                                      \
"};

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/
/*  Body Fragment Shader                                              */

GLchar BodyFragText[] = {"                                             \
                                                                       \
varying mat3 CEB;                                                      \
varying vec3 PosVecB;                                                  \
varying vec3 LightVecE;                                                \
varying vec3 NormalVecE;                                               \
varying vec3 EyeVecE;                                                  \
varying vec3 NormalVecN;                                               \
varying vec3 EyeVecN;                                                  \
                                                                       \
varying vec2 ColorTexCoord;                                            \
varying vec4 ShadowCoord;                                              \
                                                                       \
uniform sampler2D ColorTexture;                                        \
uniform sampler2D BumpTexture;                                         \
uniform samplerCube EnvironmentMap;                                    \
uniform sampler3D NoiseTexture;                                        \
uniform sampler1D SpectrumTexture;                                     \
uniform sampler2DShadow ShadowMap;                                     \
uniform vec4 NoiseGain;                                                \
uniform float NoiseBias;                                               \
uniform float NoiseScale;                                              \
uniform vec3 NoiseAxis;                                                \
uniform int  NoiseType;                                                \
uniform bool ColorTexEnabled;                                          \
uniform bool BumpTexEnabled;                                           \
uniform bool ReflectEnabled;                                           \
uniform bool NoiseColEnabled;                                          \
uniform bool NoiseBumpEnabled;                                         \
uniform bool ShadowsEnabled;                                           \
                                                                       \
/*********************************************************************/\
float MoaiVoronoi(vec3 Pos)                                            \
{                                                                      \
      int Ncell = 9;                                                   \
                                                                       \
      vec3 Cell[9];                                                    \
      Cell[0] = vec3(-0.3165728,  -0.0448891,   -2.9963881);           \
      Cell[1] = vec3( 0.8822868,   1.9886129,   3.6982760);            \
      Cell[2] = vec3(-0.1999030,  -0.2848643,   2.1759314);            \
      Cell[3] = vec3(-0.3391377,   1.1736689,   2.4655183);            \
      Cell[4] = vec3(-1.0510736,  -0.9507496,  -0.0154479);            \
      Cell[5] = vec3(-0.1347183,  -1.3097022,   0.4579103);            \
      Cell[6] = vec3( 0.4105493,  -1.7142873,   3.2193318);            \
      Cell[7] = vec3(-0.5988408,  -0.7212890,  -2.2433238);            \
      Cell[8] = vec3(-1.0599605,   0.8287739,  -0.0029123);            \
                                                                       \
      float F1;                                                        \
      float F2;                                                        \
      float D;                                                         \
                                                                       \
      /* Add noise to P */                                             \
      vec4 V = texture3D(NoiseTexture,Pos);                            \
      float f = dot(NoiseGain,V) + NoiseBias;                          \
      vec3 P = Pos + 0.08*vec3(V.x);                                   \
                                                                       \
      /* Find distances to Cell centers */                             \
      F1 = 10.0;                                                       \
      F2 = 10.0;                                                       \
      for(int i=0;i<Ncell;i++) {                                       \
         D = distance(P,Cell[i]);                                      \
         if (D < F1) {                                                 \
            F2 = F1;                                                   \
            F1 = D;                                                    \
         }                                                             \
         else if (D < F2) {                                            \
            F2 = D;                                                    \
         }                                                             \
      }                                                                \
                                                                       \
      return(F2-F1+0.1*f);                                             \
}                                                                      \
/*********************************************************************/\
vec4 NoiseCol(vec3 Pos)                                                \
{                                                                      \
      vec4 V = texture3D(NoiseTexture,Pos/NoiseScale);                 \
                                                                       \
      float f = dot(NoiseGain,V) + NoiseBias;                          \
                                                                       \
      if (NoiseType == 1) f = abs(f);                                  \
      if (NoiseType == 2) f = f*f;                                     \
      if (NoiseType == 3) {                                            \
         f = cos(dot(Pos,NoiseAxis) + f)*0.5+0.5;                      \
      }                                                                \
      if (NoiseType == 4) {                                            \
         f = cos(dot(Pos,NoiseAxis) + f)*0.5+0.5;                      \
         f = f*f;                                                      \
      }                                                                \
      if (NoiseType == 5) {                                            \
         f = MoaiVoronoi(Pos);                                         \
      }                                                                \
                                                                       \
      return(texture1D(SpectrumTexture,f));                            \
}                                                                      \
/*********************************************************************/\
vec3 NoiseBump(vec3 Pos)                                               \
{                                                                      \
      vec4 V1 = texture3D(NoiseTexture,Pos+vec3(0.2,0.2,0.0));         \
      vec4 V2 = texture3D(NoiseTexture,Pos+vec3(0.0,0.2,0.2));         \
      vec4 V3 = texture3D(NoiseTexture,Pos+vec3(0.2,0.0,0.2));         \
                                                                       \
      return(normalize(NormalVecE + 0.5*vec3(                          \
         V1.r+0.5*V1.g-0.75,                                           \
         V2.r+0.5*V2.g-0.75,                                           \
         V3.r+0.5*V3.g-0.75)));                                        \
}                                                                      \
/*********************************************************************/\
float OneSampleShadowLookup(void)                                      \
{                                                                      \
      float InLight = 0.0;                                             \
      if (ShadowCoord.x <= 0.001) InLight = 1.0;                       \
      if (ShadowCoord.x >= 0.999) InLight = 1.0;                       \
      if (ShadowCoord.y <= 0.001) InLight = 1.0;                       \
      if (ShadowCoord.y >= 0.999) InLight = 1.0;                       \
      if(shadow2DProj(ShadowMap,ShadowCoord).x == 1.0) InLight = 1.0;  \
      return(InLight);                                                 \
}                                                                      \
/*********************************************************************/\
float FourSampleShadowLookup(void)                                     \
{                                                                      \
      float depth;                                                     \
                                                                       \
      depth = shadow2DProj(ShadowMap,                                  \
         ShadowCoord+vec4(0.75,0.75,0.0,0.0)*2.0/2048.0).x;            \
      float InLight1 = (depth != 1.0 ? 0.0 : 0.25);                    \
                                                                       \
      depth = shadow2DProj(ShadowMap,                                  \
         ShadowCoord+vec4(-0.75,0.75,0.0,0.0)*2.0/2048.0).x;           \
      float InLight2 = (depth != 1.0 ? 0.0 : 0.25);                    \
                                                                       \
      depth = shadow2DProj(ShadowMap,                                  \
         ShadowCoord+vec4(0.75,-0.75,0.0,0.0)*2.0/2048.0).x;           \
      float InLight3 = (depth != 1.0 ? 0.0 : 0.25);                    \
                                                                       \
      depth = shadow2DProj(ShadowMap,                                  \
         ShadowCoord+vec4(-0.75,-0.75,0.0,0.0)*2.0/2048.0).x;          \
      float InLight4 = (depth != 1.0 ? 0.0 : 0.25);                    \
                                                                       \
      return(InLight1+InLight2+InLight3+InLight4);                     \
}                                                                      \
/*********************************************************************/\
float FiveSampleShadowLookup(void)                                     \
{                                                                      \
      float depth;                                                     \
                                                                       \
      depth = shadow2DProj(ShadowMap,ShadowCoord).x;                   \
      float InLight1 = (depth != 1.0 ? 0.0 : 0.2);                     \
                                                                       \
      depth = shadow2DProj(ShadowMap,                                  \
         ShadowCoord+vec4(0.5,0.5,0.0,0.0)/2048.0).x;                  \
      float InLight2 = (depth != 1.0 ? 0.0 : 0.2);                     \
                                                                       \
      depth = shadow2DProj(ShadowMap,                                  \
         ShadowCoord+vec4(-0.5,0.5,0.0,0.0)/2048.0).x;                 \
      float InLight3 = (depth != 1.0 ? 0.0 : 0.2);                     \
                                                                       \
      depth = shadow2DProj(ShadowMap,                                  \
         ShadowCoord+vec4(-0.5,0.5,0.0,0.0)/2048.0).x;                 \
      float InLight4 = (depth != 1.0 ? 0.0 : 0.2);                     \
                                                                       \
      depth = shadow2DProj(ShadowMap,                                  \
         ShadowCoord+vec4(-0.5,-0.5,0.0,0.0)/2048.0).x;                \
      float InLight5 = (depth != 1.0 ? 0.0 : 0.2);                     \
                                                                       \
      return(InLight1+InLight2+InLight3+InLight4+InLight5);            \
}                                                                      \
/*********************************************************************/\
void main(void)                                                        \
{                                                                      \
                                                                       \
      vec4 TexColor = vec4(1.0);                                       \
      float InLight = 1.0;                                             \
      float DiffIllum = 0.0;                                           \
      float SpecIllum = 0.0;                                           \
      float NoL,EoR;                                                   \
                                                                       \
      vec3 NormE = NormalVecE;                                         \
                                                                       \
      if (NoiseBumpEnabled)                                            \
         NormE = normalize(NoiseBump(PosVecB));                        \
                                                                       \
      vec3 EyeReflectVecN = normalize(reflect(-EyeVecN,NormalVecN));   \
      vec3 LightReflectVecE = normalize(reflect(-LightVecE,NormE));    \
                                                                       \
      if (ColorTexEnabled)                                             \
         TexColor = texture2D(ColorTexture,ColorTexCoord);             \
                                                                       \
      if (BumpTexEnabled)                                              \
         NormE = normalize(CEB*normalize(vec3(                         \
               texture2D(BumpTexture,ColorTexCoord))-0.5));            \
                                                                       \
      if (ReflectEnabled)                                              \
         TexColor = textureCube(EnvironmentMap,EyeReflectVecN);        \
                                                                       \
      if (NoiseColEnabled)                                             \
         TexColor = NoiseCol(PosVecB);                                 \
                                                                       \
      if (ShadowsEnabled) {                                            \
         InLight = OneSampleShadowLookup();                            \
      }                                                                \
                                                                       \
      NoL = dot(NormE,LightVecE);                                      \
      if (InLight > 0.0 && NoL > 0.0) {                                \
         /* Diffuse Illumination */                                    \
         DiffIllum = clamp(NoL,0.0,1.0);                               \
         /* Specular Illumination */                                   \
         EoR = clamp(dot(EyeVecE,LightReflectVecE),0.0,1.0);           \
         SpecIllum = pow(EoR,gl_FrontMaterial.shininess);              \
      }                                                                \
                                                                       \
      if (ReflectEnabled) {                                            \
         DiffIllum = 1.0;                                              \
         SpecIllum = 0.0;                                              \
      }                                                                \
                                                                       \
/* .. Compute Colors */                                                \
      /* Ambient Color */                                              \
      gl_FragColor =                                                   \
           gl_LightModel.ambient*TexColor*gl_FrontMaterial.ambient     \
      /* Add Diffuse Color */                                          \
         + DiffIllum*gl_LightSource[0].diffuse*TexColor                \
            *gl_FrontMaterial.diffuse                                  \
      /* Add Specular Color */                                         \
         + SpecIllum*gl_LightSource[0].specular*                       \
            gl_FrontMaterial.specular;                                 \
}                                                                      \
"};


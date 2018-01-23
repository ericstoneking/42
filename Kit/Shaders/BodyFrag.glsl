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
/*  Body Fragment Shader                                              */

varying mat3 CEB;                                                      
varying vec3 PosVecB;                                                  
varying vec3 LightVecE;                                                
varying vec3 NormalVecE;                                               
varying vec3 EyeVecE;                                                  
varying vec3 NormalVecN;                                               
varying vec3 EyeVecN;                                                  
                                                                       
varying vec2 ColorTexCoord;                                            
varying vec4 ShadowCoord;                                              
                                                                       
uniform sampler2D ColorTexture;                                        
uniform sampler2D BumpTexture;                                         
uniform samplerCube EnvironmentMap;                                    
uniform sampler3D NoiseTexture;                                        
uniform sampler1D SpectrumTexture;                                     
uniform sampler2DShadow ShadowMap;                                     
uniform vec4 NoiseGain;                                                
uniform float NoiseBias;                                               
uniform float NoiseScale;                                              
uniform vec3 NoiseAxis;                                                
uniform int  NoiseType;                                                
uniform bool ColorTexEnabled;                                          
uniform bool BumpTexEnabled;                                           
uniform bool ReflectEnabled;                                           
uniform bool NoiseColEnabled;                                          
uniform bool NoiseBumpEnabled;                                         
uniform bool ShadowsEnabled;                                           
                                                                       
/*********************************************************************/
float MoaiVoronoi(vec3 Pos)                                            
{                                                                      
      int Ncell = 20;                                                  
                                                                       
      vec3 Cell[20];                                                   
      Cell[ 0] = vec3(  -0.899240,   0.329625,  -0.551936);            
      Cell[ 1] = vec3(  -0.388832,  -1.401045,  -0.951608);            
      Cell[ 2] = vec3(   1.454074,  -0.140564,  -0.061765);            
      Cell[ 3] = vec3(   0.887226,  -0.219726,   1.626191);            
      Cell[ 4] = vec3(  -1.337870,   1.447301,   1.037350);            
      Cell[ 5] = vec3(   1.451972,   0.683331,  -1.685818);            
      Cell[ 6] = vec3(   2.355312,   0.255011,  -1.416038);            
      Cell[ 7] = vec3(  -1.422909,  -0.486901,  -1.757204);            
      Cell[ 8] = vec3(   1.679161,  -2.179792,  -0.873250);            
      Cell[ 9] = vec3(  -2.154019,   2.272110,   0.572772);            
      Cell[10] = vec3(  -0.861144,  -2.057742,  -2.175723);            
      Cell[11] = vec3(   2.414222,  -0.447044,  -1.538406);            
      Cell[12] = vec3(  -0.541811,  -0.945616,   2.172256);            
      Cell[13] = vec3(   1.065633,  -1.323453,   1.121513);            
      Cell[14] = vec3(  -0.593085,  -0.814691,   0.608026);            
      Cell[15] = vec3(   0.187492,  -1.209283,   1.236455);            
      Cell[16] = vec3(  -0.680762,  -1.636144,   0.542692);            
      Cell[17] = vec3(  -0.868446,  -1.106143,  -2.207942);            
      Cell[18] = vec3(   0.064421,  -0.050012,   0.660259);            
      Cell[19] = vec3(   1.836757,  -1.320785,   1.907873);            
                                                                       
      float F1;                                                        
      float F2;                                                        
      float D;                                                         
                                                                       
      /* Add noise to P */                                             
      vec4 V = texture3D(NoiseTexture,Pos);                            
      float f = dot(NoiseGain,V) + NoiseBias;                          
      vec3 P = Pos + 0.08*vec3(V.x);                                   
                                                                       
      /* Find distances to Cell centers */                             
      F1 = 1000.0;                                                     
      F2 = 1000.0;                                                     
      for(int i=0;i<Ncell;i++) {                                       
         D = distance(P,Cell[i]);                                      
         if (D < F1) {                                                 
            F2 = F1;                                                   
            F1 = D;                                                    
         }                                                             
         else if (D < F2) {                                            
            F2 = D;                                                    
         }                                                             
      }                                                                
                                                                       
      return(F2-F1+f);                                                 
}                                                                      
/*********************************************************************/
vec4 NoiseCol(vec3 Pos)                                                
{                                                                      
      vec4 V = texture3D(NoiseTexture,Pos/NoiseScale);                 
                                                                       
      float f = dot(NoiseGain,V) + NoiseBias;                          
                                                                       
      if (NoiseType == 1) f = abs(f);                                  
      if (NoiseType == 2) f = f*f;                                     
      if (NoiseType == 3) {                                            
         f = cos(dot(Pos,NoiseAxis) + f)*0.5+0.5;                      
      }                                                                
      if (NoiseType == 4) {                                            
         f = cos(dot(Pos,NoiseAxis) + f)*0.5+0.5;                      
         f = f*f;                                                      
      }                                                                
      if (NoiseType == 5) {                                            
         f = MoaiVoronoi(Pos);                                         
      }                                                                
                                                                       
      return(texture1D(SpectrumTexture,f));                            
}                                                                      
/*********************************************************************/
vec3 NoiseBump(vec3 Pos)                                               
{                                                                      
      vec4 V1 = texture3D(NoiseTexture,Pos+vec3(0.2,0.2,0.0));         
      vec4 V2 = texture3D(NoiseTexture,Pos+vec3(0.0,0.2,0.2));         
      vec4 V3 = texture3D(NoiseTexture,Pos+vec3(0.2,0.0,0.2));         
                                                                       
      return(normalize(NormalVecE + 0.5*vec3(                          
         V1.r+0.5*V1.g-0.75,                                           
         V2.r+0.5*V2.g-0.75,                                           
         V3.r+0.5*V3.g-0.75)));                                        
}                                                                      
/*********************************************************************/
float OneSampleShadowLookup(void)                                      
{                                                                      
      float InLight = 0.0;                                             
      if (ShadowCoord.x <= 0.001) InLight = 1.0;                       
      if (ShadowCoord.x >= 0.999) InLight = 1.0;                       
      if (ShadowCoord.y <= 0.001) InLight = 1.0;                       
      if (ShadowCoord.y >= 0.999) InLight = 1.0;                       
      if(shadow2DProj(ShadowMap,ShadowCoord).x == 1.0) InLight = 1.0;  
      return(InLight);                                                 
}                                                                      
/*********************************************************************/
float FourSampleShadowLookup(void)                                     
{                                                                      
      float depth;                                                     
                                                                       
      depth = shadow2DProj(ShadowMap,                                  
         ShadowCoord+vec4(0.75,0.75,0.0,0.0)*2.0/2048.0).x;            
      float InLight1 = (depth != 1.0 ? 0.0 : 0.25);                    
                                                                       
      depth = shadow2DProj(ShadowMap,                                  
         ShadowCoord+vec4(-0.75,0.75,0.0,0.0)*2.0/2048.0).x;           
      float InLight2 = (depth != 1.0 ? 0.0 : 0.25);                    
                                                                       
      depth = shadow2DProj(ShadowMap,                                  
         ShadowCoord+vec4(0.75,-0.75,0.0,0.0)*2.0/2048.0).x;           
      float InLight3 = (depth != 1.0 ? 0.0 : 0.25);                    
                                                                       
      depth = shadow2DProj(ShadowMap,                                  
         ShadowCoord+vec4(-0.75,-0.75,0.0,0.0)*2.0/2048.0).x;          
      float InLight4 = (depth != 1.0 ? 0.0 : 0.25);                    
                                                                       
      return(InLight1+InLight2+InLight3+InLight4);                     
}                                                                      
/*********************************************************************/
float FiveSampleShadowLookup(void)                                     
{                                                                      
      float depth;                                                     
                                                                       
      depth = shadow2DProj(ShadowMap,ShadowCoord).x;                   
      float InLight1 = (depth != 1.0 ? 0.0 : 0.2);                     
                                                                       
      depth = shadow2DProj(ShadowMap,                                  
         ShadowCoord+vec4(0.5,0.5,0.0,0.0)/2048.0).x;                  
      float InLight2 = (depth != 1.0 ? 0.0 : 0.2);                     
                                                                       
      depth = shadow2DProj(ShadowMap,                                  
         ShadowCoord+vec4(-0.5,0.5,0.0,0.0)/2048.0).x;                 
      float InLight3 = (depth != 1.0 ? 0.0 : 0.2);                     
                                                                       
      depth = shadow2DProj(ShadowMap,                                  
         ShadowCoord+vec4(-0.5,0.5,0.0,0.0)/2048.0).x;                 
      float InLight4 = (depth != 1.0 ? 0.0 : 0.2);                     
                                                                       
      depth = shadow2DProj(ShadowMap,                                  
         ShadowCoord+vec4(-0.5,-0.5,0.0,0.0)/2048.0).x;                
      float InLight5 = (depth != 1.0 ? 0.0 : 0.2);                     
                                                                       
      return(InLight1+InLight2+InLight3+InLight4+InLight5);            
}                                                                      
/*********************************************************************/
void main(void)                                                        
{                                                                      
                                                                       
      vec4 TexColor = vec4(1.0);                                       
      float InLight = 1.0;                                             
      float DiffIllum = 0.0;                                           
      float SpecIllum = 0.0;                                           
      float NoL,EoR;                                                   
                                                                       
      vec3 NormE = NormalVecE;                                         
                                                                       
      if (NoiseBumpEnabled)                                            
         NormE = normalize(NoiseBump(PosVecB));                        
                                                                       
      vec3 EyeReflectVecN = normalize(reflect(-EyeVecN,NormalVecN));   
      vec3 LightReflectVecE = normalize(reflect(-LightVecE,NormE));    
                                                                       
      if (ColorTexEnabled)                                             
         TexColor = texture2D(ColorTexture,ColorTexCoord);             
                                                                       
      if (BumpTexEnabled)                                              
         NormE = normalize(CEB*normalize(vec3(                         
               texture2D(BumpTexture,ColorTexCoord))-0.5));            
                                                                       
      if (ReflectEnabled)                                              
         TexColor = textureCube(EnvironmentMap,EyeReflectVecN);        
                                                                       
      if (NoiseColEnabled)                                             
         TexColor = NoiseCol(PosVecB);                                 
                                                                       
      if (ShadowsEnabled) {                                            
         InLight = OneSampleShadowLookup();                            
      }                                                                
                                                                       
      NoL = dot(NormE,LightVecE);                                      
      if (InLight > 0.0 && NoL > 0.0) {                                
         /* Diffuse Illumination */                                    
         DiffIllum = clamp(NoL,0.0,1.0);                               
         /* Specular Illumination */                                   
         EoR = clamp(dot(EyeVecE,LightReflectVecE),0.0,1.0);           
         SpecIllum = pow(EoR,gl_FrontMaterial.shininess);              
      }                                                                
                                                                       
      if (ReflectEnabled) {                                            
         DiffIllum = 1.0;                                              
         SpecIllum = 0.0;                                              
      }                                                                
                                                                       
/* .. Compute Colors */                                                
      /* Ambient Color */                                              
      gl_FragColor =                                                   
           gl_LightModel.ambient*TexColor*gl_FrontMaterial.ambient     
      /* Add Diffuse Color */                                          
         + DiffIllum*gl_LightSource[0].diffuse*TexColor                
            *gl_FrontMaterial.diffuse                                  
      /* Add Specular Color */                                         
         + SpecIllum*gl_LightSource[0].specular*                       
            gl_FrontMaterial.specular;                                 
}                                                                      

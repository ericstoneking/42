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
/*  World Fragment Shader                                             */

uniform samplerCube MapTexture;                                        
uniform samplerCube BumpTexture;                                       
uniform samplerCube CloudGlossTexture;                                 
uniform sampler1D RingTexture;                                         
uniform vec3 PovPosW;                                                  
uniform bool HasAtmo;                                                  
uniform vec3 AtmoColor;                                                
uniform float MaxHt;                                                   
                                                                       
varying float RingTexCoord;                                            
varying vec3 PosVec;                                                   
varying vec3 LightVec;                                                 
                                                                       
void main(void)                                                        
{                                                                      
      vec4 Ring;                                                       
      float RingCoord;                                                 
      float DiffIllum;                                                 
      float SpecIllum;                                                 
      vec3 HalfVec;                                                    
      float NoH;                                                       
      float Gloss;                                                     
      float AtmoFrac;                                                  
      vec3 RelPos,DirVec;                                              
      float Ra,Alt,NadirAng,HorNadirAng,fz,fg;                         
                                                                       
/* .. Illumination */                                                  
      /* Normal Vector */                                              
      vec3 Normal =                                                    
         normalize(PosVec+vec3(textureCube(BumpTexture,PosVec))-0.5);  
      vec2 CloudGloss = vec2(textureCube(CloudGlossTexture,PosVec));   
      /* Check for Ring Shadow */                                      
      RingCoord = RingTexCoord;                                        
      Ring = texture1D(RingTexture,RingCoord);                         
      /* Diffuse Illumination */                                       
      if (HasAtmo) {                                                   
         DiffIllum = smoothstep(0.0,1.0,                               
            5.0*dot(Normal,LightVec)*(1.0-Ring.a));                    
      }                                                                
      else {                                                           
         DiffIllum = smoothstep(0.0,1.0,                               
            dot(Normal,LightVec)*(1.0-Ring.a));                        
      }                                                                
      /* Specular Illumination */                                      
      HalfVec = normalize(LightVec+normalize(PovPosW));                
      Gloss = DiffIllum*CloudGloss.g;                                  
      NoH = clamp(dot(Normal,HalfVec),0.0,1.0);                        
      SpecIllum = Gloss*pow(NoH,gl_FrontMaterial.shininess);           
                                                                       
/* .. Perturb Material if Desired */                                   
      vec4 MapColor =                                                  
         vec4(vec3(textureCube(MapTexture,PosVec)),1.0);               
                                                                       
/* .. Atmosphere */                                                    
      if (HasAtmo) {                                                   
         RelPos = PovPosW - PosVec;                                    
         Ra = length(PovPosW);                                         
         Alt = Ra - 1.0;                                               
         DirVec = normalize(RelPos);                                   
         NadirAng = 3.14159-acos(DirVec[2]);                           
         HorNadirAng = asin(1.0/length(PovPosW));                      
         fz = 1.2*sqrt(Alt/MaxHt);                                     
         fg = 0.5*NadirAng/HorNadirAng;                                
         AtmoFrac = clamp(fz*fg,0.0,0.5);                              
         MapColor = AtmoFrac*vec4(AtmoColor,1.0) + (1.0-AtmoFrac)*MapColor;      
      }                                                                
                                                                       
/* .. Compute Colors */                                                
      /* Sum Ambient with Diffuse Term */                              
      vec4 Diffuse = gl_FrontLightModelProduct.sceneColor;             
      Diffuse.a = 1.0;                                                 
      vec4 Spec = vec4(0.0,0.0,0.0,1.0);                               
      /* Primary Color */                                              
      Diffuse += DiffIllum*gl_LightSource[0].diffuse;                  
      /* Secondary Color */                                            
      Spec += SpecIllum*gl_LightSource[0].specular;                    
      /* Ground Color */                                               
      vec4 GndColor = Diffuse*MapColor+                                
         Spec*gl_FrontMaterial.specular;                               
                                                                       
      /* Attenuate through Clouds */                                   
      gl_FragColor = mix(GndColor,Diffuse,CloudGloss.r);               
}                                                                      

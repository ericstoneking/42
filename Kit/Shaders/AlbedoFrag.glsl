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
/*  Albedo Fragment Shader                                            */

uniform samplerCube AlbedoTexture;                                        
uniform samplerCube CloudGlossTexture;                                 
uniform vec3 UnitWorldVecE;  
uniform vec3 SunVecE;                                                
uniform float CosWorldAng;
uniform float CosFov;
uniform float WorldRad;
uniform vec3 PosEyeW;
uniform mat3 CEF;
uniform mat3 CWE;
uniform float TexWidth; 
                                                                       
varying vec3 ViewVecInPlane;                                                

/**********************************************************************/
vec2 ProjectRayOntoSphere(vec3 Pos, vec3 Axis, float Rad)
{
      float PoA = dot(Pos,Axis);
      float P2 = dot(Pos,Pos);
      float Disc = PoA*PoA-P2+Rad*Rad;
      if (Disc > 0.0) {
         float SqrtD = sqrt(Disc);
         return(vec2(-PoA-SqrtD,-PoA+SqrtD));
      }
      else {
         return(vec2(0.0,0.0));
      }
}
/**********************************************************************/
void main(void)                                                        
{                                                                      
      
      gl_FragColor = vec4(0.0,0.0,0.0,1.0);

      vec3 ViewVecF = normalize(ViewVecInPlane);      
      vec3 ViewVecE = CEF*ViewVecF;
      float EoW = dot(ViewVecE,UnitWorldVecE); 

      if (EoW > CosWorldAng && ViewVecE.z < -CosFov) {
      
         float DiffIllum;                                                 
         float SpecIllum;                                                 
         vec3 HalfVec; 
         vec3 GndPosW;
         vec3 UnitGndPosW;
         vec2 Intersect;                                                   
         float NoH;
         vec4 AlbedoColor;                                                       
         vec4 MapColor;
         vec4 Diffuse,Spec;
         float SpecIntensity = 0.35; /* WAG */

         vec3 ViewVecW =  CWE*ViewVecE;
         vec3 SunVecW = CWE*SunVecE;
      
         float r = length(ViewVecInPlane);
         float DistScale = 1.0/(r*r); /* Corner pixels look smaller due to distance */
         float CosScale = 1.0/r; /* Pixel is tilted from LOS to F */
         float PixelScale = 4.0/TexWidth/TexWidth; /* Face area is 4.0 */
         float ViewScale = -ViewVecE.z; /* cos of angle from E axis */
         float PiScale = 1.0/3.1415926535897; /* So integral over hemisphere = 1.0 */
         float ScaleFactor = PixelScale*DistScale*CosScale*ViewScale*PiScale;
      
         Intersect = ProjectRayOntoSphere(PosEyeW,ViewVecW,WorldRad);
         /* Use nearest intersection with ground */
         GndPosW = PosEyeW + Intersect.x*ViewVecW;
         UnitGndPosW = normalize(GndPosW);

         AlbedoColor = vec4(vec3(textureCube(AlbedoTexture,UnitGndPosW)),1.0);
         //AlbedoColor = vec4(1.0,1.0,1.0,1.0);
         
         /* Illumination */
         vec2 CloudGloss = vec2(textureCube(CloudGlossTexture,UnitGndPosW));
         float Gloss = CloudGloss.g;  
         DiffIllum = clamp(dot(UnitGndPosW,SunVecW),0.0,1.0);                                                                
         /* Specular Illumination */                                      
         HalfVec = normalize(SunVecW+normalize(PosEyeW)); 
         NoH = clamp(dot(UnitGndPosW,HalfVec),0.0,1.0);                        
         SpecIllum = Gloss*pow(NoH,10.0);           
         Spec = SpecIllum*vec4(SpecIntensity,SpecIntensity,SpecIntensity,1.0);                    
      
         //gl_FragColor = DiffIllum*AlbedoColor+Spec; 
         gl_FragColor = DiffIllum*AlbedoColor; 
         gl_FragColor *= ScaleFactor;
         gl_FragColor.a = 1.0;
      }
}                                                                      

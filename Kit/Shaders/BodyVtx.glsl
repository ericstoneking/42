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
/*  Body Vertex Shader                                                */

uniform mat3 CNE;                                                      
uniform mat4 ShadowMatrix;                                             
varying mat3 CEB;                                                      
varying vec3 PosVecB;                                                  
varying vec3 LightVecE;                                                
varying vec3 NormalVecE;                                               
varying vec3 EyeVecE;                                                  
varying vec3 NormalVecN;                                               
varying vec3 EyeVecN;                                                  
                                                                       
varying vec2 ColorTexCoord;                                            
varying vec4 ShadowCoord;                                              
                                                                       
void main(void)                                                        
{                                                                      
      gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;          
      PosVecB = vec3(gl_Vertex);                                       
      LightVecE = normalize(vec3(gl_LightSource[0].position));         
      EyeVecE = -normalize(vec3(gl_ModelViewMatrix * gl_Vertex));      
      EyeVecN = CNE*EyeVecE;                                           
      CEB = gl_NormalMatrix;                                           
      NormalVecE = normalize(CEB*gl_Normal);                           
      NormalVecN = normalize(CNE*NormalVecE);                          
                                                                       
      ColorTexCoord = gl_MultiTexCoord0.st;                            
      ShadowCoord = ShadowMatrix*gl_Vertex;                            
      ShadowCoord = ShadowCoord/ShadowCoord.w;                         
                                                                       
}                                                                      

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
/*  Atmo Fragment Shader                                              */

uniform float Alt;                                                     
uniform float MaxHt;                                                   
uniform vec3 GasColor;                                                 
uniform vec3 DustColor;                                                
uniform float WorldRadius;                                             
varying vec3 PosVecL;                                                  
varying vec3 SunVecL;                                                  
                                                                       
void main(void)                                                        
{                                                                      
      float Ra = WorldRadius + Alt;                                    
      vec3 RelDir = normalize(PosVecL - vec3(0.0,0.0,Alt));            
      float ZenAng = acos(RelDir[2]);                                  
      float HorZenAng = 3.14159-asin(WorldRadius/Ra);                  
      float SunAng = acos(SunVecL[2]);                                 
                                                                       
      float fz = 1.2*sqrt(1.0-Alt/MaxHt);                              
      float fg = 0.5+0.5*ZenAng/HorZenAng;                             
      float fd = 0.1+0.5*pow(ZenAng/HorZenAng,4.0*(1.0+Alt/MaxHt));    
      float fs = clamp(1.0+10.0*(HorZenAng-SunAng),0.0,1.0);           
      vec3 SunColor = clamp(5.0E3*(dot(RelDir,SunVecL) - 0.99985),0.0,1.0) 
         *(vec3(1.0,1.0,0.9)-GasColor);                            
                                                                       
      gl_FragColor = vec4(fg*GasColor+fd*DustColor+SunColor,   
                          fz*fs);                                      
}                                                                      

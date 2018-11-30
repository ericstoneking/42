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
/*  Sun Fragment Shader                                               */

uniform vec3 UnitSunVecE;                                                  
uniform float CosSunAng;
uniform float CosCoronaAng;
                                                                       
varying vec3 DirVecInPlane;                                                
                                                                       
void main(void)                                                        
{                                                                      
      vec3 DirVecE = normalize(DirVecInPlane);
      
      float EoS = dot(DirVecE,UnitSunVecE); 
            
      gl_FragColor = vec4(0.0,0.0,0.0,0.0);
      
      if (EoS > CosSunAng) {
         gl_FragColor = vec4(1.0,1.0,1.0,1.0);
      }
      else if (EoS > CosCoronaAng) {
         float Fade = (EoS-CosCoronaAng)/(CosSunAng-CosCoronaAng);
         gl_FragColor = vec4(1.0,1.0,1.0,Fade);
      }
}                                                                      

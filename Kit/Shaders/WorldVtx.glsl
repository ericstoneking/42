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

varying float RingTexCoord;                                            
varying vec3 PosVec;                                                   
varying vec3 LightVec;                                                 
                                                                       
void main(void)                                                        
{                                                                      
      float k;                                                         
                                                                       
      gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;          
                                                                       
      PosVec = normalize(gl_Normal);                                   
      LightVec = normalize(vec3(                                       
         gl_ModelViewMatrixInverse*gl_LightSource[0].position));       
      k = step(0.0,-PosVec.z*LightVec.z)                               
         *(-PosVec.z/LightVec.z);                                      
      RingTexCoord =                                                   
         clamp(length(vec2(PosVec+k*LightVec))-1.5,-0.5,10.0);         
}                                                                      

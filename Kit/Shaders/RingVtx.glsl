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
/*  Ring Vertex Shader                                                */

varying vec3 PosVec;                                                   
varying vec3 LightVec;                                                 
                                                                       
void main(void)                                                        
{                                                                      
      gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;          
                                                                       
      gl_TexCoord[0] = gl_TextureMatrix[0]*gl_MultiTexCoord0;          
                                                                       
      PosVec = vec3(gl_Vertex);                                        
      LightVec = normalize(vec3(                                       
         gl_ModelViewMatrixInverse*gl_LightSource[0].position));       
}                                                                      

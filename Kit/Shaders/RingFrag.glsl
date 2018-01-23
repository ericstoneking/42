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
/*  Ring Fragment Shader                                              */

uniform sampler1D RingTexture;                                         
varying vec3 PosVec;                                                   
varying vec3 LightVec;                                                 
                                                                       
void main(void)                                                        
{                                                                      
      float AoL;                                                       
      vec3 ProjVec;                                                    
                                                                       
      vec4 RingColor =                                                 
         texture1D(RingTexture,gl_TexCoord[0].s);                      
      vec4 DiffIllum = gl_FrontLightModelProduct.sceneColor;           
      DiffIllum.a = 1.0;                                               
      AoL = dot(PosVec,LightVec);                                      
      ProjVec = PosVec-AoL*LightVec;                                   
      DiffIllum += max(step(1.0,dot(ProjVec,ProjVec)),                 
         step(0.0,AoL))*gl_LightSource[0].diffuse;                     
      gl_FragColor = RingColor*clamp(DiffIllum,0.0,1.0);               
}                                                                      

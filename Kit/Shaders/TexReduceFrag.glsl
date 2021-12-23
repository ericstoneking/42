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
/*  Texture Reduction Fragment Shader                                 */

uniform sampler2D Texture;
/**********************************************************************/
void main(void)                                                        
{                                                                      
            
      gl_FragColor = vec4(0.0,0.0,0.0,0.0);
      float xout = gl_TexCoord[0].x;
      float yout = gl_TexCoord[0].y;
      
      float xin = floor(2.0*xout);
      float yin = floor(2.0*yout);
      
      gl_FragColor = texture2D(Texture,vec2(xin,yin)); 
      
      xin += 1.0;
      gl_FragColor += texture2D(Texture,vec2(xin,yin));
      yin += 1.0;
      gl_FragColor += texture2D(Texture,vec2(xin,yin));
      xin -= 1.0;
      gl_FragColor += texture2D(Texture,vec2(xin,yin));
}                                                                      

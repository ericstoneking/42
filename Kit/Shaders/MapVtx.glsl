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
/*  Map Vertex Shader                                                 */

varying float Lng;                                                     
varying float Lat;                                                     
varying vec2 MapTexCoord;                                              
                                                                       
void main(void)                                                        
{                                                                      
                                                                       
      gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;          
      Lng = 0.017453*(gl_Vertex.x);                                    
      Lat = 0.017453*gl_Vertex.y;                                      
      MapTexCoord.x = 0.5+gl_Vertex.x/360.0;                           
      MapTexCoord.y = 0.5-gl_Vertex.y/180.0;                           
                                                                       
}                                                                      

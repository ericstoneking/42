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
/*  Texture Reduction Vertex Shader                                   */

void main(void)                                                        
{                                                                      
      gl_Position = gl_ProjectionMatrix*gl_Vertex; 
      gl_TexCoord[0] = gl_ProjectionMatrix*gl_Vertex;
}                                                                      

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

GLchar MapVtxText[] = {"                                               \
                                                                       \
varying float Lng;                                                     \
varying float Lat;                                                     \
varying vec2 MapTexCoord;                                              \
                                                                       \
void main(void)                                                        \
{                                                                      \
                                                                       \
      gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;          \
      Lng = 0.017453*(gl_Vertex.x);                                    \
      Lat = 0.017453*gl_Vertex.y;                                      \
      MapTexCoord.x = 0.5+gl_Vertex.x/360.0;                           \
      MapTexCoord.y = 0.5-gl_Vertex.y/180.0;                           \
                                                                       \
}                                                                      \
"};

/**********************************************************************/
/*  Map Fragment Shader                                               */
/*  Shade according to location in sunlight, night, or eclipse        */

GLchar MapFragText[] = {"                                              \
                                                                       \
uniform sampler2D MapTexture;                                          \
uniform vec3 SunVec;                                                   \
uniform float CosEclipse;                                              \
                                                                       \
varying float Lng;                                                     \
varying float Lat;                                                     \
varying vec2 MapTexCoord;                                              \
                                                                       \
void main(void)                                                        \
{                                                                      \
      float CosLng = cos(Lng);                                         \
      float SinLng = sin(Lng);                                         \
      float CosLat = cos(Lat);                                         \
      float SinLat = sin(Lat);                                         \
      vec3 PosVec = vec3(CosLng*CosLat,SinLng*CosLat,SinLat);          \
      vec4 MapColor;                                                   \
      MapColor = texture2D(MapTexture,MapTexCoord);                    \
      float PoS = dot(PosVec,SunVec);                                  \
      float Illum = 0.5 + 0.3*step(CosEclipse,PoS) + 0.2*step(0.0,PoS);\
      gl_FragColor = MapColor*clamp(Illum,0.0,1.0);                    \
                                                                       \
}                                                                      \
"};

/**********************************************************************/
/*  Moon Map Fragment Shader                                          */
/*  Shade according to location in sunlight, night, or eclipse        */

GLchar MoonMapFragText[] = {"                                          \
                                                                       \
uniform sampler2D MapTexture;                                          \
uniform sampler2D BumpTexture;                                         \
uniform vec3 SunVec;                                                   \
uniform float CosEclipse;                                              \
                                                                       \
varying float Lng;                                                     \
varying float Lat;                                                     \
varying vec2 MapTexCoord;                                              \
                                                                       \
void main(void)                                                        \
{                                                                      \
      float CosLng = cos(Lng);                                         \
      float SinLng = sin(Lng);                                         \
      float CosLat = cos(Lat);                                         \
      float SinLat = sin(Lat);                                         \
      vec3 PosVec = vec3(CosLng*CosLat,SinLng*CosLat,SinLat);          \
      vec4 MapColor;                                                   \
      MapColor = texture2D(MapTexture,MapTexCoord);                    \
      vec3 NormVec = normalize(                                        \
         vec3(texture2D(BumpTexture,MapTexCoord))-0.5);                \
      float PoS = dot(PosVec,SunVec);                                  \
      float NoS = dot(NormVec,SunVec);                                 \
      float Illum = 0.5 + 0.1*step(CosEclipse,PoS)                     \
         + 0.4*clamp(NoS,0.0,1.0)*step(0.0,PoS);                       \
      gl_FragColor = MapColor*clamp(Illum,0.0,1.0);                    \
                                                                       \
}                                                                      \
"};

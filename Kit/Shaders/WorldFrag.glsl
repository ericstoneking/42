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
/*  World Fragment Shader                                             */

uniform samplerCube MapTexture;                                        
uniform samplerCube BumpTexture;                                       
uniform samplerCube CloudGlossTexture;                                 
uniform sampler1D RingTexture;                                         
uniform bool HasAtmo;  
uniform vec3 Br;
uniform float Bm;
uniform float Hr;
uniform float Hm;
uniform float Gm;                                                
uniform vec3 UnitWorldVecE;                                                  
uniform float CosWorldAng;
uniform float CosAtmoAng;
uniform float CosRingAng;
uniform float WorldRad;
uniform vec3 PosEyeW;
uniform float MagPosEye; 
uniform mat3 CWE; 
uniform bool HasRing;
                                                                       
varying vec3 SunVecE; 
varying vec3 ViewVecInPlane;                                                

/**********************************************************************/
vec2 ProjectRayOntoSphere(vec3 Pos, vec3 Axis, float Rad)
{
      float PoA = dot(Pos,Axis);
      float P2 = dot(Pos,Pos);
      float Disc = PoA*PoA-P2+Rad*Rad;
      if (Disc > 0.0) {
         float SqrtD = sqrt(Disc);
         return(vec2(-PoA-SqrtD,-PoA+SqrtD));
      }
      else {
         return(vec2(0.0,0.0));
      }
}
/**********************************************************************/
/* Ang is the angle between the incoming and outgoing ray             */
float RayleighPhase(float CosAng)
{
      return(0.059683*(1.0+CosAng*CosAng));
}
/**********************************************************************/
/* Ang is the angle between the incoming and outgoing ray             */
float MiePhase(float CosAng, float g)
{
      float g2 = g*g;
      float x = 1.0+g2-2.0*g*CosAng;

      return(0.11937*(1.0-g2)*(1.0+CosAng*CosAng)/((2.0+g2)*x*sqrt(x)));
}
/**********************************************************************/
vec4 Transmittance(vec3 xa, vec3 xb, float Rg, float Rt,
   vec3 Br, float Hr, float Bm, float Hm)
{
      float du = 0.0625;
      vec3 dx;
      vec4 IntB = vec4(0.0,0.0,0.0,0.0);
      float u,h,er,em,ds;
      vec3 Y;

      dx = xb - xa;
      ds = length(dx)*du;

      for(u=0.5*du;u<1.0;u+=du) {
         Y = xa + u*dx;
         h = length(Y) - Rg;
         er = exp(-h/Hr);
         em = exp(-h/Hm);
         IntB.xyz += (Br*er+Bm*em)*ds;
         IntB.w += (er+em)*du;
      }

      /* Transmittance, T */
      return(vec4(exp(-IntB.xyz),IntB.w));
}
/**********************************************************************/
vec4 Inscattering(vec3 xa, vec3 xb, vec3 SunVec,float Rg, float Rt, 
   vec3 Br, float Hr, float Bm, float Hm,float g)
{
      vec3 dx, ViewVec;
      float ds,CosAng,Pr,Pm,u;
      vec3 Y;
      vec4 Txy;
      float du = 0.125;
      vec4 S = vec4(0.0,0.0,0.0,0.0);
      float m,YoS,YoY;

      dx = xb-xa;
      ds = length(dx)*du;

      ViewVec = normalize(dx);

      CosAng = dot(ViewVec,SunVec);
      Pr = RayleighPhase(CosAng);
      Pm = MiePhase(CosAng,g);

      for(u=0.5*du;u<1.0;u+=du) {
         Y = xa + u*dx;
         YoS = dot(Y,SunVec);
         YoY = dot(Y,Y);
         if (YoS > 0.0 || (YoY-YoS*YoS) > Rg*Rg) {
            Txy = Transmittance(xa,Y,Rg,Rt,Br,Hr,Bm,Hm);
            S.xyz += Txy.xyz*(Br*Pr+Bm*Pm)*ds;
            S.w += Txy.w;
         }
      }
      
      m = 0.0;
      if (S.x > m) m = S.x;
      if (S.y > m) m = S.y;
      if (S.z > m) m = S.z;
      if (m > 1.0) S.xyz /= m;
      S.xyz = 1.0-exp(-S.xyz);
      if (S.w > 1.0) S.w = 1.0;
      
      return(S);
}
/**********************************************************************/
void main(void)                                                        
{                                                                      
      vec4 Ring; 
      float RingK;                                                      
      float RingCoord;                                                 
      float DiffIllum;                                                 
      float SpecIllum;                                                 
      vec3 HalfVec; 
      vec3 GndPosW;
      vec3 UnitGndPosW;
      vec3 RingPosW;
      vec2 Intersect;                                                   
      float NoH;                                                       
      float Gloss;
      vec3 NearPosW,FarPosW,TopPosW;
      vec4 MapColor;
      vec4 AtmoColor; 
      vec4 TransTopGnd;
      vec4 TransGndEye;
      vec4 TransTopEye;
      vec4 InScat;
      vec4 Diffuse;
      vec4 AmbientLight = vec4(0.25,0.25,0.25,1.0);
      vec4 DiffuseLight = vec4(0.75,0.75,0.75,1.0);
      vec4 SpecularLight = vec4(1.0,1.0,1.0,1.0);
      
      vec3 ViewVecE = normalize(ViewVecInPlane);
      
      float EoW = dot(ViewVecE,UnitWorldVecE); 
      vec3 ViewVecW =  CWE*ViewVecE;
      vec3 SunVecW = CWE*SunVecE;         
      
      float AtmoHt = 8.0*Hr;
      float AtmoRad = WorldRad + AtmoHt; 
            
      gl_FragColor = vec4(0.0,0.0,0.0,0.0);

/* .. Draw World */
      
      if (EoW > CosWorldAng) {
         Intersect = ProjectRayOntoSphere(PosEyeW,ViewVecW,WorldRad);
         /* Use nearest intersection with ground */
         GndPosW = PosEyeW + Intersect.x*ViewVecW;
         UnitGndPosW = normalize(GndPosW);
         if (MagPosEye < AtmoRad) {
            NearPosW = PosEyeW;
         }
         else {
            Intersect = ProjectRayOntoSphere(PosEyeW,ViewVecW,AtmoRad);
            NearPosW = PosEyeW + Intersect.x*ViewVecW;
         }
         Intersect = ProjectRayOntoSphere(GndPosW,SunVecW,AtmoRad);
         TopPosW = GndPosW + Intersect.y*SunVecW;

         /* MapColor */
         MapColor = vec4(vec3(textureCube(MapTexture,UnitGndPosW)),1.0);
         
         /* Illumination */
         vec3 Normal = normalize(UnitGndPosW+vec3(textureCube(BumpTexture,UnitGndPosW))-0.5);  
         vec2 CloudGloss = vec2(textureCube(CloudGlossTexture,UnitGndPosW));  
         /* Check for Ring Shadow */
         if (HasRing) {
            RingK = step(0.0,-UnitGndPosW.z*SunVecW.z)*(-UnitGndPosW.z/SunVecW.z);                                      
            RingCoord = clamp(length(vec2(UnitGndPosW+RingK*SunVecW))-1.5,-0.5,10.0);                                        
            Ring = texture1D(RingTexture,RingCoord); 
            DiffIllum = clamp(dot(Normal,SunVecW),0.0,1.0)*(1.0-Ring.a);
         }
         else DiffIllum = clamp(dot(Normal,SunVecW),0.0,1.0);                                                                
         /* Specular Illumination */                                      
         HalfVec = normalize(SunVecW+normalize(PosEyeW)); 
         Gloss = DiffIllum*CloudGloss.g;                                  
         NoH = clamp(dot(Normal,HalfVec),0.0,1.0);                        
         SpecIllum = Gloss*pow(NoH,10.0);           
      
         /* Sum Ambient with Diffuse Term */                              
         Diffuse = AmbientLight;             
         vec4 Spec = vec4(0.0,0.0,0.0,1.0);                               
         /* Primary Color */                                              
         Diffuse += DiffIllum*DiffuseLight;                  
         /* Secondary Color */                                            
         Spec += SpecIllum*SpecularLight;                    
         /* Ground Color */                                               
         vec4 GndColor = Diffuse*MapColor+Spec*vec4(0.35,0.35,0.35,1.0); 
         GndColor.a = 1.0; 
         
         /* Atmo Scattering */
         if (HasAtmo) {
            TransTopGnd = Transmittance(GndPosW,TopPosW,WorldRad,AtmoRad,
               Br,Hr,Bm,Hm);
            TransGndEye = Transmittance(NearPosW,GndPosW,WorldRad,AtmoRad,
               Br,Hr,Bm,Hm);
            InScat = Inscattering(NearPosW,GndPosW,SunVecW,WorldRad,AtmoRad,
               Br,Hr,Bm,Hm,Gm);
                       
            /* Clouds? */
            //TransGndEye = mix(TransGndEye,vec4(1.0,1.0,1.0,1.0),CloudGloss.r);                             
                                                                       
            //gl_FragColor = mix(GndColor,Diffuse,CloudGloss.r);
            
            gl_FragColor.rgb = TransTopGnd.rgb*GndColor.rgb*TransGndEye.rgb
               + DiffuseLight.rgb*InScat.rgb + AmbientLight.rgb*MapColor.rgb;
         }
         else {
            gl_FragColor = GndColor;
         }         
         gl_FragColor.a = 1.0;
                   
      }
      else if (HasAtmo && EoW > CosAtmoAng) {
         Intersect = ProjectRayOntoSphere(PosEyeW,ViewVecW,AtmoRad);
         if (MagPosEye < AtmoRad) {
            NearPosW = PosEyeW;
         }
         else {
            NearPosW = PosEyeW + Intersect.x*ViewVecW;;
         }
         FarPosW = PosEyeW + Intersect.y*ViewVecW;;
         InScat = Inscattering(NearPosW,FarPosW,SunVecW,WorldRad,AtmoRad,
            Br,Hr,Bm,Hm,Gm);
         gl_FragColor = InScat;
      }
      
/* .. Draw Ring */
      if (HasRing) {
         if (EoW > CosWorldAng) {
            if (ViewVecW.z == 0.0) RingK = 0.0;
            else RingK = -PosEyeW.z/ViewVecW.z;
            RingPosW = (PosEyeW + RingK*ViewVecW)/WorldRad;
            float RoS = dot(RingPosW,SunVecW);
            if (RoS > 0.0 || length(RingPosW-RoS*SunVecW) > 1.0) {
               Diffuse = DiffuseLight;
            }
            else {
               Diffuse = AmbientLight;
            }
            if (dot(RingPosW,ViewVecW) < 0.0) {
               RingCoord = clamp(length(RingPosW)-1.5,-0.5,10.0);  
               Ring = texture1D(RingTexture,RingCoord);
               gl_FragColor.rgb = (1.0-Ring.a)*gl_FragColor.rgb + Diffuse.rgb*Ring.a*Ring.rgb;
               gl_FragColor.a = 1.0; 
            }
         }
         else if (EoW > CosRingAng) {
            if (ViewVecW.z == 0.0) RingK = 0.0;
            else RingK = -PosEyeW.z/ViewVecW.z;
            RingPosW = (PosEyeW + RingK*ViewVecW)/WorldRad;
            float RoS = dot(RingPosW,SunVecW);
            if (RoS > 0.0 || length(RingPosW-RoS*SunVecW) > 1.0) {
               Diffuse = DiffuseLight;
            }
            else {
               Diffuse = AmbientLight;
            }
            RingCoord = clamp(length(RingPosW)-1.5,-0.5,10.0);  
            Ring = texture1D(RingTexture,RingCoord);
            gl_FragColor = Diffuse*Ring;
         }
         else {
            gl_FragColor = vec4(0.0,0.0,0.0,0.0);
         }
      }
}                                                                      

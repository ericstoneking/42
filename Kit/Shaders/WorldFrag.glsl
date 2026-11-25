/*    This file is distributed with 42,                               */
/*    the (mostly harmless) spacecraft dynamics simulation            */
/*    created by Eric Stoneking of NASA Goddard Space Flight Center   */

/*    Copyright 2010 United States Government                         */
/*    as represented by the Administrator                             */
/*    of the National Aeronautics and Space Administration.           */

/*    No copyright is claimed in the United States                    */
/*    under Title 17, U.S. Code.                                      */

/*    All Other Rights Reserved.                                      */

#version 120

/**********************************************************************/
/*  World Fragment Shader                                             */

uniform samplerCube MapTexture;                                        
uniform samplerCube BumpTexture;                                       
uniform samplerCube CloudGlossTexture;                                 
uniform sampler1D RingTexture;                                         
uniform int OpticalDepthSamples; // Zero preserves the onboard camera integration.
uniform bool HasAtmo;
uniform bool HasRing;  
uniform vec3 GasColor;
uniform vec3 DustColor;
uniform vec3 Br;
uniform float Bm;
uniform float Hr;
uniform float Hm;                                               
uniform vec3 UnitWorldVecE;                                                  
uniform float CosWorldAng;
uniform float CosAtmoAng;
uniform float CosSunAng;
uniform float CosRingAng;
uniform float WorldRad;
uniform float AtmoRad;
uniform vec3 PosEyeW;
uniform float MagPosEye; 
uniform mat3 CWE; 
                                                                       
varying vec3 SunVecE; 
varying vec3 ViewVecInPlane;                                                

/**********************************************************************/
/* Returns (Near,Far,Meets)                                           */
vec3 ProjectRayOntoSphere(vec3 Pos, vec3 Axis, float Rad)
{
      float PoA = dot(Pos,Axis);
      float P2 = dot(Pos,Pos);
      float Disc = PoA*PoA-P2+Rad*Rad;
      if (Disc > 0.0) {
         float SqrtD = sqrt(Disc);
         return(vec3(-PoA-SqrtD,-PoA+SqrtD,1.0));
      }
      else {
         return(vec3(-PoA,-PoA,0.0));
      }
}
/**********************************************************************/
vec4 OpticalDepth(vec3 StartPt, vec3 EndPt, vec3 SunVec, float Rg)
{
      float du,dL,InteriorEnd;
      float h,u;
      float DayODr = 0.0;
      float EclODr = 0.0;
      float DayODm = 0.0;
      float EclODm = 0.0;
      float PoS;
      vec3 Pt;
      vec4 OD;
      
      if (OpticalDepthSamples > 0) {
         du = 1.0/float(OpticalDepthSamples);
         InteriorEnd = 1.0;
      }
      else {
         du = 0.001;
         InteriorEnd = 1.0-du;
      }
      dL = du*length(EndPt-StartPt);
      
      /* Trapezoid Integration */
      h = length(StartPt)-Rg;
      PoS = dot(StartPt,SunVec);
      if (PoS > 0.0 || length(StartPt-PoS*SunVec) > Rg) {
         /* Daylit */
         DayODr += 0.5*exp(-h/Hr);
         DayODm += 0.5*exp(-h/Hm);
      }
      else {
         /* Eclipsed */
         EclODr += 0.5*exp(-h/Hr);
         EclODm += 0.5*exp(-h/Hm);
      }
      
      for(u=du;u<InteriorEnd;u+=du) {
         Pt = (1.0-u)*StartPt + u*EndPt;
         h = length(Pt)-Rg;
         PoS = dot(Pt,SunVec);
         if (PoS > 0.0 || length(Pt-PoS*SunVec) > Rg) {
            /* Daylit */
            DayODr += exp(-h/Hr);
            DayODm += exp(-h/Hm);
         }
         else {
            /* Eclipsed */
            EclODr += exp(-h/Hr);
            EclODm += exp(-h/Hm);
         }
      }
      
      h = length(EndPt)-Rg;
      PoS = dot(EndPt,SunVec);
      if (PoS > 0.0 || length(EndPt-PoS*SunVec) > Rg) {
         /* Daylit */
         DayODr += 0.5*exp(-h/Hr);
         DayODm += 0.5*exp(-h/Hm);
      }
      else {
         /* Eclipsed */
         EclODr += 0.5*exp(-h/Hr);
         EclODm += 0.5*exp(-h/Hm);
      }  
      OD = vec4(DayODr,EclODr,DayODm,EclODm)*dL;    
      
      return(OD);
}
/**********************************************************************/
vec4 SkyColor(float EoW, vec3 ViewVecW, vec3 SunVecW)
{     
      vec4 SkyCol;
      vec3 Int3;
      float NearTop,FarTop;
      bool MeetsTop;
      vec4 OD;
      float TotOD;
      float m,f,VoS;
      vec3 SunCol;

      Int3 = ProjectRayOntoSphere(PosEyeW,ViewVecW,AtmoRad);
      NearTop = Int3[0];
      FarTop = Int3[1];
      MeetsTop = bool(Int3[2]);

      if (MagPosEye > AtmoRad) {
         OD = OpticalDepth(PosEyeW+NearTop*ViewVecW,PosEyeW+FarTop*ViewVecW,
            SunVecW,WorldRad);
      }
      else {
         OD = OpticalDepth(PosEyeW,PosEyeW+FarTop*ViewVecW,
            SunVecW,WorldRad);
      }
      TotOD = OD[0]+OD[1];
      SkyCol.rgb = (OD[0]+0.1*OD[1])/TotOD*GasColor;

      SkyCol.rgb += 0.5*(1.0-exp(-Br*OD[0]));
      SkyCol.a = 1.0-exp(-2.0*(OD[0]+0.1*OD[1])/Hr);

      /* Add Sun */
      SunCol = exp(-0.5*Br*TotOD);
      m = max(SunCol[0],max(SunCol[1],SunCol[2]));
      SunCol /= m;
      VoS = dot(ViewVecW,SunVecW);
      if (VoS > CosSunAng) {
         SkyCol = vec4(SunCol,1.0);
      }
      else if (VoS > 0.0 || 
         length(ViewVecW-VoS*SunVecW) > WorldRad) {
         f = clamp(1.0E-4/(1.0-0.9999995*VoS),0.0,1.0);
         SkyCol.rgb = (1.0-f)*SkyCol.rgb + f*SunCol;
      }

      return(SkyCol);
}
/**********************************************************************/
vec4 GroundColor(vec3 ViewVecW, vec3 SunVecW)
{
      vec4 AmbientLight = vec4(0.25,0.25,0.25,1.0);
      vec4 DiffuseLight = vec4(0.75,0.75,0.75,1.0);
      vec4 SpecularLight = vec4(1.0,1.0,1.0,1.0);
      vec4 Ring; 
      float RingK;                                                      
      float RingCoord;                                                 
      float DiffIllum;                                                 
      float SpecIllum;                                                 
      vec3 HalfVec; 
      vec3 GndPosW;
      vec3 UnitGndPosW;
      float NoH;                                                       
      float Gloss;
      vec4 MapColor;
      vec4 Diffuse;
      vec4 OD;
      float TotOD;
      vec4 AirCol = vec4(0.0,0.0,0.0,1.0);

      float EoV = dot(PosEyeW,ViewVecW);
      float E2 = dot(PosEyeW,PosEyeW);
      float Disc = EoV*EoV-E2+WorldRad*WorldRad;
      float Dist = -EoV-sqrt(Disc);
      GndPosW = PosEyeW+Dist*ViewVecW;
      UnitGndPosW = normalize(GndPosW);

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
      else if (HasAtmo) {
         DiffIllum = clamp(5.0*dot(Normal,SunVecW),0.0,1.0);
      }
      else DiffIllum = clamp(dot(Normal,SunVecW),0.0,1.0);                                                                
      /* Specular Illumination */                                      
      HalfVec = normalize(SunVecW+normalize(PosEyeW)); 
      Gloss = DiffIllum*CloudGloss.g;                                  
      NoH = clamp(dot(Normal,HalfVec),0.0,1.0);                        
      SpecIllum = Gloss*pow(NoH,50.0);           
   
      /* Sum Ambient with Diffuse Term */                              
      Diffuse = AmbientLight;             
      vec4 Spec = vec4(0.0,0.0,0.0,1.0);                               
      /* Primary Color */                                              
      Diffuse += DiffIllum*DiffuseLight;                  
      /* Secondary Color */                                            
      Spec += SpecIllum*SpecularLight;                    
      /* Ground Color */                                               
      vec4 GndColor = Diffuse*MapColor+Spec*vec4(0.5,0.5,0.5,1.0); 
      GndColor.a = 1.0; 

      /* Add aerial perspective */
      if (HasAtmo) {
         if (MagPosEye > AtmoRad) {
            Disc = EoV*EoV-E2+AtmoRad*AtmoRad;
            Dist = -EoV-sqrt(Disc);
            OD = OpticalDepth(PosEyeW+Dist*ViewVecW,GndPosW,
               SunVecW,WorldRad);
         }
         else {
            OD = OpticalDepth(PosEyeW,GndPosW,SunVecW,WorldRad);
         }
         TotOD = OD[0]+OD[1];
         AirCol.rgb = (OD[0]+0.1*OD[1])/TotOD*GasColor;
         AirCol.rgb *= 1.0-exp(-(OD[0]+0.1*OD[1])/Hr);
         GndColor.rgb += 0.25*AirCol.rgb;
      }

      return(GndColor);
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

      vec3 RingPosW;
      float NoH;                                                       
      float Gloss;
      vec4 Diffuse;
      vec4 AmbientLight = vec4(0.15,0.15,0.15,1.0);
      vec4 DiffuseLight = vec4(0.85,0.85,0.85,1.0);
      vec4 SpecularLight = vec4(1.0,1.0,1.0,1.0);
      
      vec3 ViewVecE = normalize(ViewVecInPlane);
      
      float EoW = dot(ViewVecE,UnitWorldVecE); 
      vec3 ViewVecW =  CWE*ViewVecE;
      vec3 SunVecW = CWE*SunVecE;         
            
      gl_FragColor = vec4(0.0,0.0,0.0,0.0);

/* .. Draw World */
      if (EoW > CosWorldAng) {
         gl_FragColor = GroundColor(ViewVecW,SunVecW);
      }
      else if (EoW > CosAtmoAng) {
         gl_FragColor = SkyColor(EoW,ViewVecW,SunVecW);
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

/*    This file is distributed with 42,                               */
/*    the (mostly harmless) spacecraft dynamics simulation            */
/*    created by Eric Stoneking of NASA Goddard Space Flight Center   */

/*    Copyright 2010 United States Government                         */
/*    as represented by the Administrator                             */
/*    of the National Aeronautics and Space Administration.           */

/*    No copyright is claimed in the United States                    */
/*    under Title 17, U.S. Code.                                      */

/*    All Other Rights Reserved.                                      */


#include "42.h"
#define EXTERN extern
#include "42gl.h"
#undef EXTERN
#define EXTERN 
#include "42glut.h"
#undef EXTERN

/* #ifdef __cplusplus
** namespace _42 {
** using namespace Kit;
** #endif
*/

/*********************************************************************/
void SetupViewVolume(int width, int height)
{
      glViewport(0,0,width,height);

      POV.AR = ((double) width)/((double) height);
      POV.Height = POV.Near*tan(0.5*POV.Angle*D2R);
      POV.Width = POV.AR*POV.Height;
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      gluPerspective(POV.Angle, POV.AR, POV.Near, POV.Far);
      glMatrixMode(GL_MODELVIEW);
      POV.CosFov = cos(1.2*sqrt(POV.Width*POV.Width+
                        POV.Height*POV.Height));
      POV.SinFov = sqrt(1.0-POV.CosFov*POV.CosFov);
}
/**********************************************************************/
void TimerHandler(int value)
{
      TimerHasExpired = 1;
}
/*********************************************************************/
void Idle(void)
{
      long MaxCamFrame = 20000;
      char CamFileName[40];
      long Done = 0;

      if (PauseFlag) {
         glutSetWindow(CamWindow);
         CamRenderExec();
         glutSwapBuffers();
         if (MapWindowExists) {
            glutSetWindow(MapWindow);
            DrawMap();
            glutSwapBuffers();
         }
         if (OrreryWindowExists) {
            glutSetWindow(OrreryWindow);
            DrawOrrery();
            glutSwapBuffers();
         }
         if (SphereWindowExists) {
            glutSetWindow(SphereWindow);
            DrawUnitSphere();
            glutSwapBuffers();
         }

         glutPostRedisplay();
      }
      else {
         POV.w[0] = 0.0;
         POV.w[1] = 0.0;
         POV.w[2] = 0.0;
         if (TimerHasExpired) {
            TimerHasExpired = 0;
            glutTimerFunc(TimerDuration,TimerHandler,0);
            Done = SimStep();
            if (GLOutFlag) {
               glutSetWindow(CamWindow);
               CamRenderExec();
               glutSwapBuffers();
               if (MapWindowExists) {
                  glutSetWindow(MapWindow);
                  DrawMap();
                  glutSwapBuffers();
               }
               if (OrreryWindowExists) {
                  glutSetWindow(OrreryWindow);
                  DrawOrrery();
                  glutSwapBuffers();
               }
               if (SphereWindowExists) {
                  glutSetWindow(SphereWindow);
                  DrawUnitSphere();
                  glutSwapBuffers();
               }

               glutPostRedisplay();
               if (CaptureCam) {
                  CamFrame++;
                  sprintf(CamFileName,"CamFrame%05ld.ppm",CamFrame);
                  glutSetWindow(CamWindow);
                  CaptureScreenToPpm("./Screenshots/",
                     CamFileName,CamHeight,CamWidth);
                  printf("Captured Cam Frame %ld\n",CamFrame);
                  if (CamFrame > MaxCamFrame) CamFrame = 0;
               }
            }
         }
      }

      if (Done) exit(0);
}
/**********************************************************************/
/* Backspace = 0x08, Tab = 0x09, Line Feed = 0x0A */
/* Carriage Return = 0x0D, Esc = 0x27, Delete = 0x7F */
void AsciiKeyHandler(unsigned char CharCode, int x, int y)
{

      static long Refresh = FALSE;
      int ShiftKey;

      /* Check for GLUT_ACTIVE_SHIFT, GLUT_ACTIVE_CTRL, GLUT_ACTIVE_ALT */
      ShiftKey = glutGetModifiers();

      if (ShiftKey & GLUT_ACTIVE_CTRL) {
         switch(CharCode) {
            case 'q' :
               exit(0);
               break;
         }
      }
      else {
         switch(CharCode) {
            case 'q' :
               exit(0);
               break;
            case 'p' :
               if (PauseFlag) PauseFlag = FALSE;
               else PauseFlag = TRUE;
               break;
            case 'c' :
               if (CaptureCam) CaptureCam = FALSE;
               else {
                  CaptureCam = TRUE;
                  CamFrame = 0;
               }
               break;
            case ' ' :
               if (ShowHUD) ShowHUD = 0;
               else ShowHUD = 1;
               break;
            case 'r' :
               break;
            case 'w' :
               if (ShowWatermark) ShowWatermark = 0;
               else ShowWatermark = 1;
         }
      }

      if (Refresh) {
         Refresh = FALSE;
         SetPovOrientation();
      }
}
/**********************************************************************/
void SpecialKeyHandler(int key, int x, int y)
{

      switch(key) {
         case GLUT_KEY_UP :
            break;
         case GLUT_KEY_DOWN :
            break;
         case GLUT_KEY_LEFT :
            POV.Angle *= 0.625;
            if (POV.Angle < 1.0) POV.Angle = 1.0;
            SetupViewVolume(CamWidth,CamHeight);
            break;
         case GLUT_KEY_RIGHT :
            POV.Angle *= 1.6;
            if (POV.Angle > 60.0) POV.Angle = 60.0;
            SetupViewVolume(CamWidth,CamHeight);
            break;
         case GLUT_KEY_PAGE_UP :
            break;
         case GLUT_KEY_PAGE_DOWN :
            break;
         case GLUT_KEY_HOME :
            break;
         case GLUT_KEY_END :
            break;
         case GLUT_KEY_INSERT :
            break;
         case GLUT_KEY_F1 :
            glutSetWindow(CamWindow);
            CaptureScreenToPpm("./Screenshots/",
               "CamSnap.ppm",CamHeight,CamWidth);
            break;
         case GLUT_KEY_F2 :
            if (MapWindowExists) {
               glutSetWindow(MapWindow);
               CaptureScreenToPpm("./Screenshots/",
                  "MapSnap.ppm",MapHeight,MapWidth);
            }
            break;
      }
}
/**********************************************************************/
void CamMouseButtonHandler(int Button, int State, int x, int y)
{
      static long Refresh = FALSE;
      long Pick,i;

      /* printf("Mouse Button Event\n"); */
      if (Button == GLUT_LEFT_BUTTON) {
         if (State == GLUT_DOWN) {
            if (x >= PovWidget.xmin && x <= PovWidget.xmax &&
                y >= PovWidget.ymin && y <= PovWidget.ymax) {
               /* POV Widget */
               Pick = 100; /* Bogus value */
               for(i=0;i<PovWidget.Nspot;i++) {
                  if (x >= PovWidget.Spot[i].xmin &&
                      x <= PovWidget.Spot[i].xmax &&
                      y >= PovWidget.Spot[i].ymin &&
                      y <= PovWidget.Spot[i].ymax) {
                     Pick = i;
                  }
               }
               if (Pick < 3) {
                  for(i=0;i<3;i++) PovWidget.Spot[i].Selected = 0;
                  PovWidget.Spot[Pick].Selected = 1;
                  POV.Mode = TRACK_HOST+Pick;
                  Refresh = TRUE;
               }
               else if (Pick < 8) {
                  for(i=0;i<5;i++) PovWidget.Spot[3+i].Selected = 0;
                  PovWidget.Spot[Pick].Selected = 1;
                  POV.Frame = FRAME_N+(Pick-3);
                  Refresh = TRUE;
               }
               else if (Pick < 11) {
                  for(i=0;i<6;i++) PovWidget.Spot[8+i].Selected = 0;
                  if (Pick == 8) {
                     PovWidget.Spot[8].Selected = 1;
                     PovWidget.Spot[13].Selected = 1;
                     if (POV.BoreAxis == POS_X) {
                        POV.BoreAxis = NEG_X;
                        POV.UpAxis = NEG_Z;
                     }
                     else {
                        POV.BoreAxis = POS_X;
                        POV.UpAxis = POS_Z;
                     }
                     Refresh = TRUE;
                  }
                  else if (Pick == 9) {
                     PovWidget.Spot[9].Selected = 1;
                     PovWidget.Spot[11].Selected = 1;
                     if (POV.BoreAxis == POS_Y) {
                        POV.BoreAxis = NEG_Y;
                        POV.UpAxis = NEG_X;
                     }
                     else {
                        POV.BoreAxis = POS_Y;
                        POV.UpAxis = POS_X;
                     }
                     Refresh = TRUE;
                  }
                  else if (Pick == 10) {
                     PovWidget.Spot[10].Selected = 1;
                     PovWidget.Spot[12].Selected = 1;
                     if (POV.BoreAxis == POS_Z) {
                        POV.BoreAxis = NEG_Z;
                        POV.UpAxis = NEG_Y;
                     }
                     else {
                        POV.BoreAxis = POS_Z;
                        POV.UpAxis = POS_Y;
                     }
                     Refresh = TRUE;
                  }
               }
               else if (Pick == 11) {
                  if (POV.BoreAxis != POS_X && POV.BoreAxis != NEG_X) {
                     PovWidget.Spot[11].Selected = 1;
                     PovWidget.Spot[12].Selected = 0;
                     PovWidget.Spot[13].Selected = 0;
                     if (POV.UpAxis == POS_X)
                        POV.UpAxis = NEG_X;
                     else
                        POV.UpAxis = POS_X;
                     Refresh = TRUE;
                  }
               }
               else if (Pick == 12) {
                  if (POV.BoreAxis != POS_Y && POV.BoreAxis != NEG_Y) {
                     PovWidget.Spot[11].Selected = 0;
                     PovWidget.Spot[12].Selected = 1;
                     PovWidget.Spot[13].Selected = 0;
                     if (POV.UpAxis == POS_Y)
                        POV.UpAxis = NEG_Y;
                     else
                        POV.UpAxis = POS_Y;
                     Refresh = TRUE;
                  }
               }
               else if (Pick == 13) {
                  if (POV.BoreAxis != POS_Z && POV.BoreAxis != NEG_Z) {
                     PovWidget.Spot[11].Selected = 0;
                     PovWidget.Spot[12].Selected = 0;
                     PovWidget.Spot[13].Selected = 1;
                     if (POV.UpAxis == POS_Z)
                        POV.UpAxis = NEG_Z;
                     else
                        POV.UpAxis = POS_Z;
                     Refresh = TRUE;
                  }
               }
            }
            else if (x >= HostWidget.xmin && x <= HostWidget.xmax &&
                     y >= HostWidget.ymin && y <= HostWidget.ymax) {
               /* Host Widget */
               Pick = 100; /* Bogus value */
               for(i=0;i<HostWidget.Nspot;i++) {
                  if (x >= HostWidget.Spot[i].xmin &&
                      x <= HostWidget.Spot[i].xmax &&
                      y >= HostWidget.Spot[i].ymin &&
                      y <= HostWidget.Spot[i].ymax) {
                     Pick = i;
                  }
               }
               if (Pick < 5) {
                  for(i=0;i<5;i++) HostWidget.Spot[i].Selected = 0;
                  HostWidget.Spot[Pick].Selected = 1;
                  POV.Host.Type = Pick;
               }
               else if (Pick == 5) {
                  if (POV.Host.Type == TARGET_WORLD) {
                     do {
                        POV.Host.World = (POV.Host.World+NWORLD-1)%NWORLD;
                     } while(!World[POV.Host.World].Exists);
                  }
                  else if (POV.Host.Type == TARGET_REFORB ||
                           POV.Host.Type == TARGET_FRM) {
                     POV.Host.RefOrb = (POV.Host.RefOrb+Norb-1)%Norb;
                     POV.Host.World = Orb[POV.Host.RefOrb].World;
                     while(!(SC[POV.Host.SC].Exists &&
                             SC[POV.Host.SC].RefOrb ==
                             POV.Host.RefOrb)) {
                        POV.Host.SC = (POV.Host.SC+1)%Nsc;
                     };
                  }
                  else if (POV.Host.Type == TARGET_SC) {
                     do {
                        POV.Host.SC = (POV.Host.SC+Nsc-1)%Nsc;
                     } while(!SC[POV.Host.SC].Exists);
                     POV.Host.RefOrb = SC[POV.Host.SC].RefOrb;
                     POV.Host.World = Orb[POV.Host.RefOrb].World;
                  }
                  else {
                     POV.Host.Body = (POV.Host.Body+SC[POV.Host.SC].Nb-1)
                        %SC[POV.Host.SC].Nb;
                  }
               }
               else if (Pick == 6) {
                  if (POV.Host.Type == TARGET_WORLD) {
                     do {
                        POV.Host.World = (POV.Host.World+1)%NWORLD;
                     } while(!World[POV.Host.World].Exists);
                  }
                  else if (POV.Host.Type == TARGET_REFORB ||
                           POV.Host.Type == TARGET_FRM) {
                     POV.Host.RefOrb = (POV.Host.RefOrb+1)%Norb;
                     POV.Host.World = Orb[POV.Host.RefOrb].World;
                     while(!(SC[POV.Host.SC].Exists &&
                             SC[POV.Host.SC].RefOrb ==
                             POV.Host.RefOrb)) {
                        POV.Host.SC = (POV.Host.SC+1)%Nsc;
                     };
                  }
                  else if (POV.Host.Type == TARGET_SC) {
                     do {
                        POV.Host.SC = (POV.Host.SC+1)%Nsc;
                     } while(!SC[POV.Host.SC].Exists);
                     POV.Host.RefOrb = SC[POV.Host.SC].RefOrb;
                     POV.Host.World = Orb[POV.Host.RefOrb].World;
                  }
                  else {
                     POV.Host.Body = (POV.Host.Body+1)%SC[POV.Host.SC].Nb;
                  }
               }
               else if (Pick == 7) {
                  POV.Range *= 1.6;
                  if (POV.Range > 10000.0) POV.Range = 10000.0;
               }
               else if (Pick == 8) {
                  POV.Range *= 0.625;
                  if (POV.Range < 1.0) POV.Range = 1.0;
               }
            }
            else if (x >= TargetWidget.xmin && x <= TargetWidget.xmax &&
                     y >= TargetWidget.ymin && y <= TargetWidget.ymax) {
               /* Target Widget */
               Pick = 100; /* Bogus value */
               for(i=0;i<TargetWidget.Nspot;i++) {
                  if (x >= TargetWidget.Spot[i].xmin &&
                      x <= TargetWidget.Spot[i].xmax &&
                      y >= TargetWidget.Spot[i].ymin &&
                      y <= TargetWidget.Spot[i].ymax) {
                     Pick = i;
                  }
               }
               if (Pick < 5) {
                  for(i=0;i<5;i++) TargetWidget.Spot[i].Selected = 0;
                  TargetWidget.Spot[Pick].Selected = 1;
                  POV.Target.Type = Pick;
               }
               else if (Pick == 5) {
                  if (POV.Target.Type == TARGET_WORLD) {
                     do {
                        POV.Target.World = (POV.Target.World+NWORLD-1)%NWORLD;
                     } while(!World[POV.Target.World].Exists);
                  }
                  else if (POV.Target.Type == TARGET_REFORB ||
                           POV.Target.Type == TARGET_FRM) {
                     POV.Target.RefOrb = (POV.Target.RefOrb+Norb-1)%Norb;
                     POV.Target.World = Orb[POV.Target.RefOrb].World;
                  }
                  else if (POV.Target.Type == TARGET_SC) {
                     do {
                        POV.Target.SC = (POV.Target.SC+Nsc-1)%Nsc;
                     } while(!SC[POV.Target.SC].Exists);
                     POV.Target.RefOrb = SC[POV.Target.SC].RefOrb;
                     POV.Target.World = Orb[POV.Target.RefOrb].World;
                  }
                  else {
                     POV.Target.Body = (POV.Target.Body+SC[POV.Target.SC].Nb-1)
                        %SC[POV.Target.SC].Nb;
                  }
               }
               else if (Pick == 6) {
                  if (POV.Target.Type == TARGET_WORLD) {
                     do {
                        POV.Target.World = (POV.Target.World+1)%NWORLD;
                     } while(!World[POV.Target.World].Exists);
                  }
                  else if (POV.Target.Type == TARGET_REFORB ||
                           POV.Target.Type == TARGET_FRM) {
                     POV.Target.RefOrb = (POV.Target.RefOrb+1)%Norb;
                     POV.Target.World = Orb[POV.Target.RefOrb].World;
                  }
                  else if (POV.Target.Type == TARGET_SC) {
                     do {
                        POV.Target.SC = (POV.Target.SC+1)%Nsc;
                     } while(!SC[POV.Target.SC].Exists);
                     POV.Target.RefOrb = SC[POV.Target.SC].RefOrb;
                     POV.Target.World = Orb[POV.Target.RefOrb].World;
                  }
                  else {
                     POV.Target.Body = (POV.Target.Body+1)%SC[POV.Target.SC].Nb;
                  }
               }
               else if (Pick == 7) {
                  POV.Range *= 1.6;
                  if (POV.Range > 10000.0) POV.Range = 10000.0;
               }
               else if (Pick == 8) {
                  POV.Range *= 0.625;
                  if (POV.Range < 1.0) POV.Range = 1.0;
               }
            }
            else if (x >= CamShowWidget.xmin && x <= CamShowWidget.xmax &&
                     y >= CamShowWidget.ymin && y <= CamShowWidget.ymax) {
               /* CamShow Widget */
               Pick = 100; /* Bogus value */
               for(i=0;i<CamShowWidget.Nspot;i++) {
                  if (x >= CamShowWidget.Spot[i].xmin &&
                      x <= CamShowWidget.Spot[i].xmax &&
                      y >= CamShowWidget.Spot[i].ymin &&
                      y <= CamShowWidget.Spot[i].ymax) {
                     Pick = i;
                  }
               }
               if (Pick < CAM_MENU_SIZE) {
                  if (CamShow[Pick]) CamShow[Pick] = 0;
                  else CamShow[Pick] = 1;
               }
               ShadowsEnabled = CamShow[CAM_SHADOWS];
            }
            else {
               /* Click and Drag POV */
               if (!PauseFlag) PausedByMouse = 1;
               else PausedByMouse = 0;
               PauseFlag = 1;
               MouseClickX = x;
               MouseClickY = y;
            }
            /* printf("Mouse Down\n"); */
         }
         else { /* GLUT_UP */
            /* printf("Mouse Up\n"); */
            if (PausedByMouse) PauseFlag = 0;
            PausedByMouse = 0;
            POV.w[1] = 0.0;
            POV.w[0] = 0.0;
         }
      }

      if (Refresh) {
         Refresh = FALSE;
         SetPovOrientation();
      }
}
/**********************************************************************/
void CamMouseActiveMotionHandler(int x, int y)
{
      if (POV.Mode == TRACK_HOST) {
         POV.w[1] = MouseScaleFactor * (double) (x-MouseClickX);
         POV.w[0] = MouseScaleFactor * (double) (y-MouseClickY);
         POV.w[2] = 0.0;
      }
}
/**********************************************************************/
void CamMousePassiveMotionHandler(int x, int y){}
/**********************************************************************/
void OrreryMouseButtonHandler(int Button, int State, int x, int y)
{
      struct SpotType *S;
      long i,j,Pick;
      double rwn[3],rwh[3],rw[3],xw,yw,dx,dy,d,LPrh[3];
      long Iw,Is,Ip;
      struct WorldType *W;
      struct LagrangeSystemType *LS;
      struct LagrangePointType *LP;
      struct OrreryPOVType *O;
      long Done;

      O = &Orrery;

      /* printf("Mouse Button Event\n"); */
      if (Button == GLUT_LEFT_BUTTON) {
         if (State == GLUT_DOWN) {
            /* Orrery Widget */
            if (x >= OrreryWidget.xmin && x <= OrreryWidget.xmax &&
                y >= OrreryWidget.ymin && y <= OrreryWidget.ymax) {
               Pick = 100; /* Bogus value */
               for(i=0;i<OrreryWidget.Nspot;i++) {
                  S = &OrreryWidget.Spot[i];
                  if (x >= S->xmin &&
                      x <= S->xmax &&
                      y >= S->ymin &&
                      y <= S->ymax) {
                     Pick = i;
                  }
               }
               switch (Pick) {
                  case 100:
                     break;
                  case 0:  /* Zoom In */
                     O->Zoom = (O->Zoom > 0 ? O->Zoom - 1 : 0);
                     O->Radius = ((double) OrreryWidth)/(2.0*80.0)*
                                       O->Scale[O->Zoom];
                     break;
                  case 1:  /* Zoom Out */
                     O->Zoom = (O->Zoom < 29 ? O->Zoom + 1 : 29);
                     O->Radius = ((double) OrreryWidth)/(2.0*80.0)*
                                       O->Scale[O->Zoom];
                     break;
                  case 2:  /* Pan Up */
                     if (O->Regime == ORB_CENTRAL) {
                        O->PosN[1] += 0.5*O->Radius;
                     }
                     else {
                        for(i=0;i<3;i++) O->PosN[i] +=
                           0.5*O->Radius*LagSys[O->LagSys].CLN[1][i];
                     }
                     break;
                  case 3:  /* Pan Down */
                     if (O->Regime == ORB_CENTRAL) O->PosN[1] -= 0.5*O->Radius;
                     else {
                        for(i=0;i<3;i++) O->PosN[i] -=
                           0.5*O->Radius*LagSys[O->LagSys].CLN[1][i];
                     }
                     break;
                  case 4:  /* Pan Left */
                     if (O->Regime == ORB_CENTRAL) O->PosN[0] -= 0.5*O->Radius;
                     else {
                        for(i=0;i<3;i++) O->PosN[i] -=
                           0.5*O->Radius*LagSys[O->LagSys].CLN[0][i];
                     }
                     break;
                  case 5:  /* Pan Right */
                     if (O->Regime == ORB_CENTRAL) O->PosN[0] += 0.5*O->Radius;
                     else {
                        for(i=0;i<3;i++) O->PosN[i] +=
                           0.5*O->Radius*LagSys[O->LagSys].CLN[0][i];
                     }
                     break;
                  case 6:  /* Rotate CW */
                     O->Angle -= 1.0*D2R;
                     if (O->Angle < -Pi) O->Angle += TwoPi;
                     printf("CW Button not connected yet\n");
                     break;
                  case 7:  /* Rotate CCW */
                     O->Angle += 1.0*D2R;
                     if (O->Angle > Pi) O->Angle -= TwoPi;
                     printf("CCW Button not connected yet\n");
                     break;
                  default:
                     break;
               }
            }
            /* Center on World or Lagrange Point */
            else {
               Done = 0;
               /* Check Worlds */
               for(Iw=0;Iw<NWORLD && !Done;Iw++) {
                  if (World[Iw].Exists) {
                     if (O->Regime == ORB_CENTRAL) {
                        for(i=0;i<3;i++) rwh[i] =
                           World[Iw].PosH[i] - World[O->World].PosH[i];
                        MxV(O->CNH,rwh,rwn);
                        for(i=0;i<3;i++) rw[i] = rwn[i] - O->PosN[i];
                     }
                     else {
                        LS = &LagSys[O->LagSys];
                        LP = &LS->LP[O->LP];
                        for(i=0;i<3;i++) rwh[i] =
                           World[Iw].PosH[i] - World[LS->Body1].PosH[i];
                        MxV(O->CNH,rwh,rwn);
                        for(i=0;i<3;i++) rwn[i] -= LP->PosN[i] + O->PosN[i];
                        MxV(LagSys[O->LagSys].CLN,rwn,rw);
                     }
                     xw = 0.5*(1.0+rw[0]/O->Radius)*OrreryWidth;
                     yw = 0.5*(1.0-rw[1]/(O->Radius*OrreryHeight/OrreryWidth))*OrreryHeight;
                     dx = ((double) x)-xw;
                     dy = ((double) y)-yw;
                     d = sqrt(dx*dx+dy*dy);
                     if (d < 20.0) { /* If within 20 pixels */
                        Done = 1;
                        O->Regime = ORB_CENTRAL;
                        O->World = Iw;
                        for(i=0;i<3;i++) {
                           for(j=0;j<3;j++) {
                              O->CNH[i][j] = World[Iw].CNH[i][j];
                              O->CN[i][j] = 0.0;
                           }
                           O->CN[i][i] = 1.0;
                           O->PosN[i] = 0.0;
                        }
                        MxM(O->CN,O->CNH,O->CH);
                     }
                  }
               }
               /* Check LagPts */
               for(Is=0;Is<3 && !Done;Is++) {
                  if (LagSys[Is].Exists) {
                     Iw = LagSys[Is].Body1;
                     W = &World[Iw];
                     for(Ip=0;Ip<5;Ip++) {
                        LP = &LagSys[Is].LP[Ip];
                        MTxV(W->CNH,LP->PosN,LPrh);
                        LPrh[0] += W->PosH[0];
                        LPrh[1] += W->PosH[1];
                        LPrh[2] += W->PosH[2];
                        for(i=0;i<3;i++) rwh[i] =
                           LPrh[i] - World[O->World].PosH[i];
                        MxV(O->CNH,rwh,rwn);
                        if (O->Regime == ORB_CENTRAL) {
                           for(i=0;i<3;i++) rw[i] = rwn[i] - O->PosN[i];
                        }
                        else {
                           for(i=0;i<3;i++)
                              rwn[i] -= LagSys[O->LagSys].LP[O->LP].PosN[i] + O->PosN[i];
                           MxV(LagSys[Is].CLN,rwn,rw);
                        }
                        xw = 0.5*(1.0+rw[0]/O->Radius)*OrreryWidth;
                        yw = 0.5*(1.0-rw[1]/(O->Radius*OrreryHeight/OrreryWidth))*OrreryHeight;
                        dx = ((double) x)-xw;
                        dy = ((double) y)-yw;
                        d = sqrt(dx*dx+dy*dy);
                        if (d < 20.0) {
                           O->Regime = ORB_THREE_BODY;
                           O->World = Iw;
                           O->LagSys = Is;
                           O->LP = Ip;
                           for(i=0;i<3;i++) {
                              for(j=0;j<3;j++) {
                                 O->CNH[i][j] = W->CNH[i][j];
                                 O->CN[i][j] = LagSys[Is].CLN[i][j];
                                 O->CH[i][j] = LagSys[Is].CLH[i][j];
                              }
                              O->PosN[i] = 0.0;
                           }
                        }
                     }
                  }
               }
            }
         }
      }
}
/**********************************************************************/
void OrreryMouseActiveMotionHandler(int x, int y)
{
}
/**********************************************************************/
void OrreryMousePassiveMotionHandler(int x, int y){}
/**********************************************************************/
void SphereMouseButtonHandler(int Button, int State, int x, int y)
{
      struct SpotType *S;
      struct WidgetType *W;
      long i,Pick;

      y = SphereWindowHeight - y; /* Flip vertically */

      if (Button == GLUT_LEFT_BUTTON) {
         if (State == GLUT_DOWN) {

            /* Center Widget */
            W = &CenterWidget;

            if (x >= W->xmin && x <= W->xmax && y >= W->ymin && y <= W->ymax) {
               Pick = 100;
               for (i=0; i<W->Nspot; i++) {
                  S = &W->Spot[i];
                  if (x >= S->xmin && x <= S->xmax &&
                      y >= S->ymin && y <= S->ymax) {
                     Pick = i;
                  }
               }

               if (Pick != 100) {

                  W->Spot[Pick].Selected = 1;

                  if (Pick < 6) {
                     for (i=0; i<6; i++) {
                        if (i != Pick) W->Spot[i].Selected = 0;
                     }
                  }
                  else {
                     for (i=6; i<10; i++) {
                        if (i != Pick) W->Spot[i].Selected = 0;
                     }
                  }
               }
            }

            /* Show Widget */
            W = &SphereShowWidget;

            if (x >= W->xmin && x <= W->xmax && y >= W->ymin && y <= W->ymax) {
               Pick = 100;
               for (i=0; i<W->Nspot; i++) {
                  S = &W->Spot[i];
                  if (x >= S->xmin && x <= S->xmax &&
                      y >= S->ymin && y <= S->ymax) {
                     Pick = i;
                  }
               }

               if (Pick != 100) {

                  if (W->Spot[Pick].Selected == 0) {
                     W->Spot[Pick].Selected = 1;
                  }
                  else {
                     W->Spot[Pick].Selected = 0;
                  }
               }
            }

            /* Vectors Widget */
            W = &VectorsWidget;

            if (x >= W->xmin && x <= W->xmax && y >= W->ymin && y <= W->ymax) {
               Pick = 100;
               for (i=0; i<W->Nspot; i++) {
                  S = &W->Spot[i];
                  if (x >= S->xmin && x <= S->xmax &&
                      y >= S->ymin && y <= S->ymax) {
                     Pick = i;
                  }
               }

               if (Pick != 100) {

                  if (W->Spot[Pick].Selected == 0) {
                     W->Spot[Pick].Selected = 1;
                  }
                  else {
                     W->Spot[Pick].Selected = 0;
                  }
               }
            }

            /* FOVs Widget */
            W = &FOVsWidget;

            if (x >= W->xmin && x <= W->xmax && y >= W->ymin && y <= W->ymax) {
               Pick = 100;
               for (i=0; i<W->Nspot; i++) {
                  S = &W->Spot[i];
                  if (x >= S->xmin && x <= S->xmax &&
                      y >= S->ymin && y <= S->ymax) {
                     Pick = i;
                  }
               }

               if (Pick != 100) {

                  if (W->Spot[Pick].Selected == 0) {
                     W->Spot[Pick].Selected = 1;
                  }
                  else {
                     W->Spot[Pick].Selected = 0;
                  }
               }
            }

            /* Grids Widget */
            W = &GridsWidget;

            if (x >= W->xmin && x <= W->xmax && y >= W->ymin && y <= W->ymax) {
               Pick = 100;
               for (i=0; i<W->Nspot; i++) {
                  S = &W->Spot[i];
                  if (x >= S->xmin && x <= S->xmax &&
                      y >= S->ymin && y <= S->ymax) {
                     Pick = i;
                  }
               }

               if (Pick != 100) {

                  if (W->Spot[Pick].Selected == 0) {
                     W->Spot[Pick].Selected = 1;
                  }
                  else {
                     W->Spot[Pick].Selected = 0;
                  }
               }
            }

            /* Axes Widget */
            W = &AxesWidget;

            if (x >= W->xmin && x <= W->xmax && y >= W->ymin && y <= W->ymax) {
               Pick = 100;
               for (i=0; i<W->Nspot; i++) {
                  S = &W->Spot[i];
                  if (x >= S->xmin && x <= S->xmax &&
                      y >= S->ymin && y <= S->ymax) {
                     Pick = i;
                  }
               }

               if (Pick != 100) {

                  if (W->Spot[Pick].Selected == 0) {
                     W->Spot[Pick].Selected = 1;
                  }
                  else {
                     W->Spot[Pick].Selected = 0;
                  }
               }
            }
         }
      }
}
/**********************************************************************/
void CamReshapeHandler(int width, int height)
{
      CamWidth = width;
      CamHeight = height;
      SetupViewVolume(CamWidth,CamHeight);
}
/**********************************************************************/
void MapReshapeHandler(int width, int height)
{
      MapWidth = width;
      MapHeight = height;
      glutReshapeWindow(width,height);
}
/**********************************************************************/
void OrreryReshapeHandler(int width, int height)
{
      struct OrreryPOVType *O;
      O = &Orrery;
      OrreryWidth = width;
      OrreryHeight = height;
      glutReshapeWindow(width,height);
      glViewport(0,0,width,height);
      InitOrreryWidget();
      O->Radius = ((double) OrreryWidth)/(2.0*80.0)*
                                       O->Scale[O->Zoom];
}
/**********************************************************************/
void SphereReshapeHandler(int width, int height)
{
      double ymin;

      if (width >= 512) SphereWindowWidth = width;
      else SphereWindowWidth = 512;

      SphereWindowHeight = SphereWindowWidth/2 + 16*(NumSphereWindowMenuLines + 2);

      glutReshapeWindow(SphereWindowWidth,SphereWindowHeight);

      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();

      ymin = -90.0 - 16*(NumSphereWindowMenuLines + 2) * 360.0/SphereWindowWidth;
      gluOrtho2D(180.0,-180.0,ymin,90.0);
      glMatrixMode(GL_MODELVIEW);


      InitSphereWidgets();
}
/*********************************************************************/
void InitCamWindow(void)
{
      int GlutParm;

      glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
      //glutInitDisplayString("rgba double depth hidpi");
      glutInitWindowSize(CamWidth,CamHeight);
      CamWindow = glutCreateWindow(CamTitle);
      #if (defined(GLEW_BUILD) || defined(GLEW_STATIC))
      if (GLEW_OK != glewInit()) {
         printf("glew failed to initialize in InitCamWindow\n");
         exit(1);
      }
      #endif

      glutSetWindow(CamWindow);
      glutPositionWindow(0,30);

/* .. Set GL state variables */
      glClearColor(0.0,0.0,0.0,1.0);
      glFrontFace(GL_CCW);
      glCullFace(GL_BACK);
      glEnable(GL_CULL_FACE);
      glEnable(GL_DEPTH_TEST);
      glDepthFunc(GL_LEQUAL);
      glEnable(GL_POINT_SMOOTH);
      glShadeModel(GL_SMOOTH);
      glPolygonMode(GL_FRONT, GL_FILL);
      /*glPolygonMode(GL_FRONT, GL_LINE);*/
      glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

/* .. Create light */
      glEnable(GL_LIGHTING);
      glEnable(GL_LIGHT0);
      glLightfv(GL_LIGHT0,GL_SPECULAR,SpecularLightColor);
      glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glEnable(GL_BLEND);
      glEnable(GL_NORMALIZE);

/* .. Set Callback Functions */
      glutDisplayFunc(CamRenderExec);
      glutIdleFunc(Idle);
      glutMouseFunc(CamMouseButtonHandler);
      glutMotionFunc(CamMouseActiveMotionHandler);
      glutPassiveMotionFunc(CamMousePassiveMotionHandler);
      glutKeyboardFunc(AsciiKeyHandler);
      glutSpecialFunc(SpecialKeyHandler);
      glutReshapeFunc(CamReshapeHandler);
      /*glutSetCursor(GLUT_CURSOR_INHERIT);*/

/* .. Set up view volume */
      POV.Near = 1.0;
      POV.Far = 50.0E3;  /* m */
      SkyDistance = 0.5*POV.Far;

      Font8x11Offset = LoadFont8x11();
      InitCamWidgets();
      SetupViewVolume(CamWidth,CamHeight);
      LoadShadowMapFBO();
      #ifdef _USE_SHADERS_
         printf("Loading Cam Shaders\n");
         LoadCamShaders();
      #endif
      printf("Loading Cam Textures\n");
      LoadCamTextures();
      printf("Loading 3D Noise\n");
      Load3DNoise();
      VREnabled = FALSE;
      SeeThruPassNeeded = FALSE;
      printf("Loading Cam Lists\n");
      LoadCamLists();

      /*CreateStarrySkyEnvMap();*/

      ShowWatermark = 0;

      sprintf(Banner," ");
      BannerColor[0] = 0.2;
      BannerColor[1] = 0.3;
      BannerColor[2] = 0.4;
      BannerColor[3] = 1.0;
      
      GlutParm = glutGet(GLUT_WINDOW_WIDTH);
      printf("Cam Window Width = %d\n",GlutParm);
      GlutParm = glutGet(GLUT_WINDOW_HEIGHT);
      printf("Cam Window Height = %d\n",GlutParm);
      GlutParm = glutGet(GLUT_SCREEN_WIDTH);
      printf("Cam Screen Width = %d\n",GlutParm);
      GlutParm = glutGet(GLUT_SCREEN_HEIGHT);
      printf("Cam Screen Height = %d\n",GlutParm);
      
      printf("Done Initializing Cam Window\n");

}
/*********************************************************************/
void InitMapWindow(void)
{
      GLfloat White[4] = {1.0,1.0,1.0,1.0};
      GLfloat Black[4] = {0.0,0.0,0.0,1.0};
      long Isc,Iw;
      GLuint RockballMapTexTag;
      GLuint IceballMapTexTag;

      glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
      glutInitWindowSize(MapWidth,MapHeight);
      MapWindow = glutCreateWindow(MapTitle);
      glutSetWindow(MapWindow);
      glutPositionWindow(CamWidth,30);

/* .. Set Callback Functions */
      glutDisplayFunc(DrawMap);
      glutIdleFunc(Idle);
      glutKeyboardFunc(AsciiKeyHandler);

/* .. Set GL state variables */
      glClearColor(0.0,0.0,0.3,1.0);
      glEnable(GL_POINT_SMOOTH);
      glPolygonMode(GL_FRONT, GL_FILL);

/* .. Create light */
      glEnable(GL_LIGHTING);
      glLightModelfv(GL_LIGHT_MODEL_AMBIENT,Black);
      glEnable(GL_LIGHT0);
      glLightfv(GL_LIGHT0,GL_DIFFUSE,White);
      glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glEnable(GL_BLEND);

/* .. Set up view volume */
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      gluOrtho2D(-180.0,180.0,-90.0,90.0);
      glMatrixMode(GL_MODELVIEW);

      for(Isc=0;Isc<Nsc;Isc++) {
         if (SC[Isc].Exists)
            SC[Isc].SpriteTexTag =
               PpmToTexTag("./Model/",SC[Isc].SpriteFileName,4,GL_CLAMP);
      }

      RockballMapTexTag = PpmToTexTag("./World/","Rockball.ppm",3,GL_REPEAT);
      IceballMapTexTag = PpmToTexTag("./World/","Iceball.ppm",3,GL_REPEAT);
      for(Iw=MERCURY;Iw<NWORLD;Iw++) {
         if (World[Iw].Exists) {
            if (!strcmp(World[Iw].MapFileName,"Rockball"))
               World[Iw].MapTexTag = RockballMapTexTag;
            else if (!strcmp(World[Iw].MapFileName,"Iceball"))
               World[Iw].MapTexTag = IceballMapTexTag;
            else if (strcmp(World[Iw].MapFileName,"NONE"))
               World[Iw].MapTexTag = PpmToTexTag("./World/",World[Iw].MapFileName,3,GL_CLAMP);
         }
      }
      World[EARTH].MapTexTag = PpmToTexTag("./World/","BlueMarble2.ppm",3,GL_CLAMP);
      World[LUNA].ColTexTag = PpmToTexTag("./World/",World[LUNA].ColTexFileName,3,GL_REPEAT);
      World[LUNA].BumpTexTag = PpmToTexTag("./World/",World[LUNA].BumpTexFileName,3,GL_REPEAT);

      LogoTexTag = PpmToTexTag("./Model/","GlastLogo.ppm",3,GL_CLAMP);
      SunSpriteTexTag = PpmToTexTag("./Model/","SunSpriteAlpha.ppm",4,GL_CLAMP);
      AntiSunSpriteTexTag = PpmToTexTag("./Model/","AntiSunSpriteAlpha.ppm",4,GL_CLAMP);
      MoonSpriteTexTag = PpmToTexTag("./Model/","MoonSpriteAlpha.ppm",4,GL_CLAMP);
      glGenTextures(1,&DayNightTexTag);

      #ifdef _USE_SHADERS_
         LoadMapShaders();
      #endif
}
/*********************************************************************/
void InitOrreryWindow(void)
{
      GLfloat White[4] = {1.0,1.0,1.0,1.0};
      GLfloat Black[4] = {0.0,0.0,0.0,1.0};
      char ScaleLabel[30][8] = {
         "1 km","2 km","5 km","10 km","20 km","50 km",
         "100 km","200 km","500 km","1000 km","2000 km","5000 km",
         "10K km","20K km","50K km","100K km","200K km","500K km",
         "1M km","2M km","5M km","10M km","20M km","50M km",
         "100M km","1 AU","2 AU","5 AU","10 AU","20 AU"};
      double Scale[30] = {
         1.0E3,2.0E3,5.0E3,1.0E4,2.0E4,5.0E4,
         1.0E5,2.0E5,5.0E5,1.0E6,2.0E6,5.0E6,
         1.0E7,2.0E7,5.0E7,1.0E8,2.0E8,5.0E8,
         1.0E9,2.0E9,5.0E9,1.0E10,2.0E10,5.0E10,
         1.0E11,1.0*AU2m,2.0*AU2m,5.0*AU2m,10.0*AU2m,20.0*AU2m};
      long i;
      struct OrreryPOVType *O;

      O = &Orrery;

/* .. Migrate to input file */
      OrreryWidth = 512;
      OrreryHeight = 512;
      O->Zoom = 26;
      O->Regime = ORB_CENTRAL;
      O->World = SOL;
      O->LagSys = SUNEARTH;
      O->LP = LAGPT_L1;
      strcpy(OrreryTitle,"42 Orrery");

      for(i=0;i<3;i++) {
         O->PosN[i] = 0.0;
         O->CN[i][i] = 1.0;
         O->CH[i][i] = 1.0;
         O->CNH[i][i] = 1.0;
      }

      for(i=0;i<30;i++) {
         O->Scale[i] = Scale[i];
         strcpy(O->ScaleLabel[i],ScaleLabel[i]);
      }

      glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
      glutInitWindowSize(OrreryWidth,OrreryHeight);
      OrreryWindow = glutCreateWindow(OrreryTitle);
      glutSetWindow(OrreryWindow);
      glutPositionWindow(CamWidth,MapHeight+66);

/* .. Set Callback Functions */
      glutDisplayFunc(DrawOrrery);
      glutIdleFunc(Idle);
      glutKeyboardFunc(AsciiKeyHandler);
      glutMouseFunc(OrreryMouseButtonHandler);
      glutMotionFunc(OrreryMouseActiveMotionHandler);
      glutPassiveMotionFunc(OrreryMousePassiveMotionHandler);
      glutReshapeFunc(OrreryReshapeHandler);

/* .. Set GL state variables */
      glClearColor(0.141,0.12,0.27,1.0);
      glEnable(GL_POINT_SMOOTH);
      glEnable(GL_DEPTH_TEST);
      glDepthFunc(GL_LEQUAL);
      glShadeModel(GL_SMOOTH);
      glPolygonMode(GL_FRONT, GL_FILL);
      glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

/* .. Create light */
      glEnable(GL_LIGHTING);
      glLightModelfv(GL_LIGHT_MODEL_AMBIENT,Black);
      glEnable(GL_LIGHT0);
      glLightfv(GL_LIGHT0,GL_DIFFUSE,White);
      glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glEnable(GL_BLEND);

/* .. Set up view volume */
      O->Radius = ((double) OrreryWidth)/(2.0*80.0)*O->Scale[O->Zoom];
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      glOrtho(-O->Radius,O->Radius,
              -O->Radius,O->Radius,
              -60.0*AU2m,60.0*AU2m);
      glMatrixMode(GL_MODELVIEW);

      InitOrreryWidget();

      OrrerySphereList = glGenLists(1);
      glNewList(OrrerySphereList,GL_COMPILE);
      DrawUnitCubeSphere(32);
      glEndList();

}
/*********************************************************************/
void InitSphereWindow(void)
{
      double ymin;
      glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);

      NumSphereWindowMenuLines = 6;
      /* In addition to changing this, the number of entries in each
         widget must be changed in InitSphereWidgets, and new entries
         are added in DrawSphereHUD                                  */

      SphereWindowWidth = 512;
      SphereWindowHeight = 256 + 16*(NumSphereWindowMenuLines + 2);
      glutInitWindowSize(SphereWindowWidth,SphereWindowHeight);
      SphereWindow = glutCreateWindow("42 Unit Sphere Viewer");
      glutSetWindow(SphereWindow);
      glutPositionWindow(CamWidth+MapWidth,30);

      glutDisplayFunc(DrawUnitSphere);
      glutIdleFunc(Idle);
      glutKeyboardFunc(AsciiKeyHandler);
      glutMouseFunc(SphereMouseButtonHandler);
      glutReshapeFunc(SphereReshapeHandler);

      glClearColor(0.0,0.0,0.0,1.0);
      glPolygonMode(GL_FRONT,GL_FILL);

      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();

      ymin = -90.0 - 16*(NumSphereWindowMenuLines + 2) * 90.0/128.0;
      gluOrtho2D(180.0,-180.0,ymin,90.0);
      glMatrixMode(GL_MODELVIEW);

      glEnable(GL_LINE_SMOOTH);
      glEnable(GL_POINT_SMOOTH);
      glEnable(GL_BLEND);
      glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glDisable(GL_LIGHTING);

      SphereSunSpriteTexTag = PpmToTexTag("./Model/","SunSpriteAlpha.ppm",4,GL_CLAMP);
      SphereMoonSpriteTexTag = PpmToTexTag("./Model/","MoonSpriteAlpha.ppm",4,GL_CLAMP);

      InitSphereWidgets();
}
/*********************************************************************/
long GuiCmdInterpreter(char CmdLine[512], double *CmdTime)
{
      char response[120];
      char FrameChar;
      double Ang1,Ang2,Ang3;
      double Val1,Val2,Val3,Val4;
      long NewCmdProcessed = FALSE;
      long Ifov;

      if (sscanf(CmdLine,"%lf POV.Host.SC %ld",CmdTime,&POV.Host.SC) == 2) {
         NewCmdProcessed = TRUE;
         POV.Host.RefOrb = SC[POV.Host.SC].RefOrb;
         POV.Host.World = Orb[POV.Host.RefOrb].World;
      }

      if (sscanf(CmdLine,"%lf POV.Host.RefOrb %ld",CmdTime,&POV.Host.RefOrb) == 2) {
         NewCmdProcessed = TRUE;
         POV.Host.Type = TARGET_REFORB;
         POV.Host.World = Orb[POV.Host.RefOrb].World;
      }

      if (sscanf(CmdLine,"%lf CaptureCam %s",CmdTime,response) == 2) {
         NewCmdProcessed = TRUE;
         CaptureCam = DecodeString(response);
      }

      if (sscanf(CmdLine,"%lf CamSnap %s",CmdTime,response) == 2) {
         NewCmdProcessed = TRUE;
         if (DecodeString(response) == TRUE) {
            glutSetWindow(CamWindow);
            CaptureScreenToPpm("./Screenshots/",
               "CamSnap.ppm",CamHeight,CamWidth);
         }
      }

      if (sscanf(CmdLine,"%lf MapSnap %s",CmdTime,response) == 2) {
         NewCmdProcessed = TRUE;
         if (DecodeString(response) == TRUE && MapWindowExists) {
            glutSetWindow(MapWindow);
            CaptureScreenToPpm("./Screenshots/",
               "MapSnap.ppm",MapHeight,MapWidth);
         }
      }

      if (sscanf(CmdLine,"%lf ShowHUD %s",CmdTime,response) == 2) {
         NewCmdProcessed = TRUE;
         ShowHUD = DecodeString(response);
      }

      if (sscanf(CmdLine,"%lf ShowWatermark %s",CmdTime,response) == 2) {
         NewCmdProcessed = TRUE;
         ShowWatermark = DecodeString(response);
      }

      if (sscanf(CmdLine,"%lf ShowShadows %s",CmdTime,response) == 2) {
         NewCmdProcessed = TRUE;
         CamShow[CAM_SHADOWS] = DecodeString(response);
         ShadowsEnabled = CamShow[CAM_SHADOWS];
      }

      if (sscanf(CmdLine,"%lf ShowProxOps %s",CmdTime,response) == 2) {
         NewCmdProcessed = TRUE;
         CamShow[PROX_OPS] = DecodeString(response);
      }

      if (sscanf(CmdLine,"%lf ShowFOV %s",CmdTime,response) == 2) {
         NewCmdProcessed = TRUE;
         CamShow[CAM_FOV] = DecodeString(response);
      }

      if (sscanf(CmdLine,"%lf FOV[%ld].NearExists =  %s",CmdTime,&Ifov,response) == 3) {
         NewCmdProcessed = TRUE;
         FOV[Ifov].NearExists = DecodeString(response);
      }

      if (sscanf(CmdLine,"%lf FOV[%ld].FarExists =  %s",CmdTime,&Ifov,response) == 3) {
         NewCmdProcessed = TRUE;
         FOV[Ifov].FarExists = DecodeString(response);
      }

      if (sscanf(CmdLine,"%lf Banner = \"%[^\"]\"",CmdTime,response) == 2) {
         NewCmdProcessed = TRUE;
         strcpy(Banner,response);
      }

      if (sscanf(CmdLine,"%lf BannerColor = [%lf %lf %lf %lf]",
         CmdTime,&Val1,&Val2,&Val3,&Val4) == 5) {
         NewCmdProcessed = TRUE;
         BannerColor[0] = Val1;
         BannerColor[1] = Val2;
         BannerColor[2] = Val3;
         BannerColor[3] = Val4;
      }

      if (sscanf(CmdLine,"%lf GL Output Step = %lf",CmdTime,&DTOUTGL) == 2) {
         NewCmdProcessed = TRUE;
      }
      if (sscanf(CmdLine,"%lf POV CmdRange = %lf",CmdTime,&POV.CmdRange) == 2) {
         NewCmdProcessed = TRUE;
      }
      if (sscanf(CmdLine,"%lf POV CmdSeq = %ld",CmdTime,&POV.CmdSeq) == 2) {
         NewCmdProcessed = TRUE;
      }
      if (sscanf(CmdLine,"%lf POV CmdPermute = [%lf %lf %lf; %lf %lf %lf; %lf %lf %lf]",
         CmdTime,
            &POV.CmdPermute[0][0],&POV.CmdPermute[0][1],&POV.CmdPermute[0][2],
            &POV.CmdPermute[1][0],&POV.CmdPermute[1][1],&POV.CmdPermute[1][2],
            &POV.CmdPermute[2][0],&POV.CmdPermute[2][1],&POV.CmdPermute[2][2]) == 10) {
         NewCmdProcessed = TRUE;
      }
      if (sscanf(CmdLine,"%lf POV CmdAngle = [%lf %lf %lf] deg",
         CmdTime,&Ang1,&Ang2,&Ang3) == 4) {
         NewCmdProcessed = TRUE;
         POV.CmdAngle[0] = Ang1*D2R;
         POV.CmdAngle[1] = Ang2*D2R;
         POV.CmdAngle[2] = Ang3*D2R;
      }
      if (sscanf(CmdLine,"%lf POV TimeToGo = %lf",CmdTime,&POV.TimeToGo) == 2) {
         NewCmdProcessed = TRUE;
      }
      if (sscanf(CmdLine,"%lf POV Frame = %c",CmdTime,&FrameChar) == 2) {
         NewCmdProcessed = TRUE;
         if (FrameChar == 'N') POV.Frame = FRAME_N;
         else if (FrameChar == 'L') POV.Frame = FRAME_L;
         else if (FrameChar == 'F') POV.Frame = FRAME_F;
         else if (FrameChar == 'S') POV.Frame = FRAME_S;
         else if (FrameChar == 'B') POV.Frame = FRAME_B;
      }

      return(NewCmdProcessed);
}
/*********************************************************************/
int HandoffToGui(int argc, char **argv)
{
      PausedByMouse = 0;

      ReadGraphicsInpFile();
      LoadFOVs();
      InitColors();
      SetPovOrientation();
      UpdatePOV();

      printf("Initializing GLUT\n");
      glutInit(&argc,argv);
      printf("Initializing Cam Window\n");
      InitCamWindow();
      if (MapWindowExists) {
         InitMapWindow();
      }
      if (OrreryWindowExists) {
         InitOrreryWindow();
      }
      if (SphereWindowExists) {
         InitSphereWindow();
      }
      
      /* Comment out when OpenGL installation is stable */
      /*CheckOpenGLProperties();*/

/* .. Dive into Event Loop */
      TimerDuration = 0;
      TimerHasExpired = 1;
      glutMainLoop();

      return(0);
}
/* #ifdef __cplusplus
** }
** #endif
*/

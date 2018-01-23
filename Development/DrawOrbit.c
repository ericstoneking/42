void DrawOrbit(struct OrbitType *O)
{

      double MinArc = 0.1;
      double MinRange = POV.Range;
      double ArcGain = MinArc/MinRange;

      float OrbitColor[4] = {0.7,0.7,0.7,0.5};

      double th = 0.0;
      double PosP[3]; /* Position in perifocal frame */
      double r,CPN[3][3],PosN[3],RelPosN[3],resp,dist,ds,dth;
      long i;

      /* Rotation from N to Perifocal frame */
      A2C(313,O->RAAN,O->inc,O->ArgP,CPN);

      PosP[0] = O->rmin;
      PosP[1] = 0.0;
      PosP[2] = 0.0;
      MTxV(CPN,PosP,PosN);

      glDisable(GL_LIGHTING);
      glColor4fv(OrbitColor);

      glBegin(GL_LINE_LOOP);
         glVertex3dv(PosN);
         while(th < TwoPi) {
            r = O->SLR/(1.0+O->ecc*cos(th));
            PosP[0] = r*cos(th);
            PosP[1] = r*sin(th);
            PosP[2] = 0.0;
            MTxV(CPN,PosP,PosN);

            for(i=0;i<3;i++) RelPosN[i] = PosN[i] - POV.PosN[i];
            dist = MAGV(RelPosN);
            ds = ArcGain*dist;
            resp = r*O->ecc*sin(th)/O->SLR;
            dth = ds/(r*sqrt(1.0+resp*resp));

            th += dth;
         }
      glEnd();

      glColor4f(0.0,0.0,0.0,1.0);
      glEnable(GL_LIGHTING);
}

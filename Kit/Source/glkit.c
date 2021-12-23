/*    This file is distributed with 42,                               */
/*    the (mostly harmless) spacecraft dynamics simulation            */
/*    created by Eric Stoneking of NASA Goddard Space Flight Center   */

/*    Copyright 2010 United States Government                         */
/*    as represented by the Administrator                             */
/*    of the National Aeronautics and Space Administration.           */

/*    No copyright is claimed in the United States                    */
/*    under Title 17, U.S. Code.                                      */

/*    All Other Rights Reserved.                                      */


#define EXTERN extern
#include "glkit.h"
#undef EXTERN

/* #ifdef __cplusplus
** namespace Kit {
** #endif
*/

/**********************************************************************/
/*  Available fonts:                                                  */
/*     GLUT_BITMAP_8_BY_13                                            */
/*     GLUT_BITMAP_9_BY_15                                            */
/*     GLUT_BITMAP_TIMES_ROMAN_10                                     */
/*     GLUT_BITMAP_TIMES_ROMAN_24                                     */
/*     GLUT_BITMAP_HELVETICA_10                                       */
/*     GLUT_BITMAP_HELVETICA_12                                       */
/*     GLUT_BITMAP_HELVETICA_18                                       */
void DrawBitmapString(void *font, const char *string)
{
      const char *c;
      for(c=string; *c != '\0'; c++) {
         glutBitmapCharacter(font, *c);
      }
}
/**********************************************************************/
/*  Available fonts:                                                  */
/*     GLUT_STROKE_ROMAN                                              */
/*     GLUT_STROKE_MONO_ROMAN                                         */
void DrawStrokeString(void *font, const char *string)
{
      const char *c;
      for(c=string; *c != '\0'; c++) {
         glutStrokeCharacter(font, *c);
      }
}
/*********************************************************************/
GLuint LoadFont8x11(void)
{
GLubyte Font8x11[95][11] = {
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, /* space */
{0x00, 0x10, 0x00, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10}, /* ! */
{0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x28, 0x28, 0x28}, /* " */
{0x00, 0x00, 0x00, 0x44, 0x44, 0xfe, 0x44, 0x44, 0xfe, 0x44, 0x44}, /* # */
{0x00, 0x00, 0x10, 0x38, 0x54, 0x18, 0x38, 0x50, 0x54, 0x38, 0x10}, /* $ */
{0x00, 0x00, 0x00, 0x42, 0x45, 0x25, 0x12, 0x48, 0xa4, 0xa2, 0x42}, /* % */
{0x00, 0x00, 0x00, 0x3a, 0x44, 0x4a, 0x50, 0x20, 0x44, 0x28, 0x10}, /* & */
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x10}, /* ' */
{0x00, 0x00, 0x00, 0x01, 0x02, 0x04, 0x04, 0x04, 0x04, 0x02, 0x01}, /* ( */
{0x00, 0x00, 0x00, 0x80, 0x40, 0x20, 0x20, 0x20, 0x20, 0x40, 0x80}, /* ) */

{0x00, 0x00, 0x00, 0x10, 0x54, 0x38, 0x10, 0x38, 0x54, 0x10, 0x00}, /* * */
{0x00, 0x00, 0x00, 0x10, 0x10, 0x10, 0xfe, 0x10, 0x10, 0x10, 0x00}, /* + */
{0x00, 0x20, 0x10, 0x10, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00}, /* , */
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x00, 0x00, 0x00, 0x00}, /* - */
{0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, /* . */
{0x00, 0x00, 0x00, 0x40, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x02}, /* / */
{0x00, 0x00, 0x00, 0x38, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x38}, /* 0 */
{0x00, 0x00, 0x00, 0x3e, 0x08, 0x08, 0x08, 0x08, 0x28, 0x18, 0x08}, /* 1 */
{0x00, 0x00, 0x00, 0x7e, 0x40, 0x20, 0x18, 0x04, 0x42, 0x42, 0x3c}, /* 2 */
{0x00, 0x00, 0x00, 0x3c, 0x42, 0x02, 0x02, 0x0c, 0x02, 0x42, 0x3c}, /* 3 */

{0x00, 0x00, 0x00, 0x04, 0x04, 0x7e, 0x44, 0x24, 0x14, 0x0c, 0x04}, /* 4 */
{0x00, 0x00, 0x00, 0x3c, 0x42, 0x02, 0x02, 0x7c, 0x40, 0x40, 0x7e}, /* 5 */
{0x00, 0x00, 0x00, 0x3c, 0x42, 0x42, 0x42, 0x7c, 0x40, 0x20, 0x18}, /* 6 */
{0x00, 0x00, 0x00, 0x10, 0x10, 0x10, 0x08, 0x04, 0x02, 0x02, 0x7e}, /* 7 */
{0x00, 0x00, 0x00, 0x3c, 0x42, 0x42, 0x42, 0x3c, 0x42, 0x42, 0x3c}, /* 8 */
{0x00, 0x00, 0x00, 0x18, 0x04, 0x02, 0x3e, 0x42, 0x42, 0x42, 0x3c}, /* 9 */
{0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00}, /* : */
{0x00, 0x20, 0x10, 0x10, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00}, /* ; */
{0x00, 0x00, 0x00, 0x00, 0x06, 0x38, 0xc0, 0x38, 0x06, 0x00, 0x00}, /* < */
{0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x00, 0xfe, 0x00, 0x00, 0x00}, /* = */

{0x00, 0x00, 0x00, 0x00, 0x60, 0x1c, 0x03, 0x1c, 0x60, 0x00, 0x00}, /* > */
{0x00, 0x08, 0x00, 0x00, 0x08, 0x08, 0x04, 0x02, 0x42, 0x42, 0x3c}, /* ? */
{0x00, 0x00, 0x1e, 0x20, 0x4e, 0x52, 0x52, 0x52, 0x4e, 0x22, 0x1c}, /* @ */
{0x00, 0x00, 0x00, 0x82, 0x82, 0xfe, 0x82, 0x44, 0x44, 0x28, 0x10}, /* A */
{0x00, 0x00, 0x00, 0x7c, 0x42, 0x42, 0x42, 0x7c, 0x42, 0x42, 0x7c}, /* B */
{0x00, 0x00, 0x00, 0x38, 0x44, 0x82, 0x80, 0x80, 0x82, 0x44, 0x38}, /* C */
{0x00, 0x00, 0x00, 0xf8, 0x84, 0x82, 0x82, 0x82, 0x82, 0x84, 0xf8}, /* D */
{0x00, 0x00, 0x00, 0x7e, 0x40, 0x40, 0x40, 0x78, 0x40, 0x40, 0x7e}, /* E */
{0x00, 0x00, 0x00, 0x40, 0x40, 0x40, 0x40, 0x78, 0x40, 0x40, 0x7e}, /* F */
{0x00, 0x00, 0x00, 0x3c, 0x42, 0x82, 0x8e, 0x80, 0x82, 0x44, 0x38}, /* G */

{0x00, 0x00, 0x00, 0x82, 0x82, 0x82, 0x82, 0xfe, 0x82, 0x82, 0x82}, /* H */
{0x00, 0x00, 0x00, 0x7c, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x7c}, /* I */
{0x00, 0x00, 0x00, 0x18, 0x24, 0x42, 0x42, 0x02, 0x02, 0x02, 0x07}, /* J */
{0x00, 0x00, 0x00, 0x42, 0x42, 0x44, 0x68, 0x50, 0x48, 0x44, 0x42}, /* K */
{0x00, 0x00, 0x00, 0x7e, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40}, /* L */
{0x00, 0x00, 0x00, 0x81, 0x81, 0x81, 0x81, 0x99, 0xa5, 0xc3, 0x81}, /* M */
{0x00, 0x00, 0x00, 0x42, 0x46, 0x4a, 0x4a, 0x52, 0x52, 0x62, 0x42}, /* N */
{0x00, 0x00, 0x00, 0x38, 0x44, 0x82, 0x82, 0x82, 0x82, 0x44, 0x38}, /* O */
{0x00, 0x00, 0x00, 0x40, 0x40, 0x40, 0x78, 0x44, 0x42, 0x44, 0x78}, /* P */
{0x00, 0x03, 0x44, 0x38, 0x44, 0x82, 0x82, 0x82, 0x82, 0x44, 0x38}, /* Q */

{0x00, 0x00, 0x00, 0x42, 0x44, 0x44, 0x78, 0x44, 0x42, 0x44, 0x78}, /* R */
{0x00, 0x00, 0x00, 0x38, 0x44, 0x04, 0x04, 0x38, 0x40, 0x44, 0x38}, /* S */
{0x00, 0x00, 0x00, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0xfe}, /* T */
{0x00, 0x00, 0x00, 0x38, 0x44, 0x82, 0x82, 0x82, 0x82, 0x82, 0x82}, /* U */
{0x00, 0x00, 0x00, 0x10, 0x28, 0x28, 0x44, 0x44, 0x82, 0x82, 0x82}, /* V */
{0x00, 0x00, 0x00, 0x44, 0x44, 0xaa, 0xaa, 0x92, 0x92, 0x82, 0x82}, /* W */
{0x00, 0x00, 0x00, 0x42, 0x42, 0x24, 0x18, 0x18, 0x24, 0x42, 0x42}, /* X */
{0x00, 0x00, 0x00, 0x10, 0x10, 0x10, 0x10, 0x28, 0x44, 0x82, 0x82}, /* Y */
{0x00, 0x00, 0x00, 0x7e, 0x20, 0x10, 0x10, 0x08, 0x08, 0x04, 0x7e}, /* Z */
{0x00, 0x00, 0x00, 0x1c, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x1c}, /* [ */

{0x00, 0x00, 0x00, 0x02, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x40}, /* Backslash */
{0x00, 0x00, 0x00, 0x38, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x38}, /* ] */
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x44, 0x28, 0x10}, /* ^ */
{0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, /* _ */
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x10}, /* ` */
{0x00, 0x00, 0x00, 0x3a, 0x46, 0x42, 0x46, 0x3a, 0x02, 0x3c, 0x00}, /* a */
{0x00, 0x00, 0x00, 0x5c, 0x62, 0x42, 0x42, 0x62, 0x5c, 0x40, 0x40}, /* b */
{0x00, 0x00, 0x00, 0x3c, 0x42, 0x40, 0x40, 0x42, 0x3c, 0x00, 0x00}, /* c */
{0x00, 0x00, 0x00, 0x3a, 0x46, 0x42, 0x42, 0x46, 0x3a, 0x02, 0x02}, /* d */
{0x00, 0x00, 0x00, 0x3c, 0x42, 0x40, 0x7e, 0x42, 0x3c, 0x00, 0x00}, /* e */

{0x00, 0x00, 0x00, 0x20, 0x20, 0x20, 0x70, 0x20, 0x24, 0x24, 0x18}, /* f */
{0x38, 0x44, 0x02, 0x3a, 0x46, 0x42, 0x42, 0x46, 0x3a, 0x00, 0x00}, /* g */
{0x00, 0x00, 0x00, 0x42, 0x42, 0x42, 0x42, 0x62, 0x5c, 0x40, 0x40}, /* h */
{0x00, 0x00, 0x00, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00, 0x00, 0x10}, /* i */
{0x30, 0x48, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x00, 0x00, 0x08}, /* j */
{0x00, 0x00, 0x00, 0x44, 0x68, 0x70, 0x48, 0x44, 0x40, 0x40, 0x40}, /* k */
{0x00, 0x00, 0x00, 0x18, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10}, /* l */
{0x00, 0x00, 0x00, 0x92, 0x92, 0x92, 0x92, 0xda, 0xa4, 0x00, 0x00}, /* m */
{0x00, 0x00, 0x00, 0x42, 0x42, 0x42, 0x42, 0x72, 0x5c, 0x00, 0x00}, /* n */
{0x00, 0x00, 0x00, 0x3c, 0x42, 0x42, 0x42, 0x42, 0x3c, 0x00, 0x00}, /* o */

{0x40, 0x40, 0x40, 0x5c, 0x62, 0x42, 0x42, 0x62, 0x5c, 0x00, 0x00}, /* p */
{0x02, 0x02, 0x02, 0x3a, 0x46, 0x42, 0x42, 0x46, 0x3a, 0x00, 0x00}, /* q */
{0x00, 0x00, 0x00, 0x40, 0x40, 0x40, 0x40, 0x64, 0x58, 0x00, 0x00}, /* r */
{0x00, 0x00, 0x00, 0x3c, 0x02, 0x06, 0x38, 0x20, 0x1e, 0x00, 0x00}, /* s */
{0x00, 0x00, 0x00, 0x0c, 0x10, 0x10, 0x10, 0x10, 0x3c, 0x10, 0x10}, /* t */
{0x00, 0x00, 0x00, 0x1a, 0x26, 0x22, 0x22, 0x22, 0x22, 0x00, 0x00}, /* u */
{0x00, 0x00, 0x00, 0x10, 0x28, 0x28, 0x44, 0x44, 0x44, 0x00, 0x00}, /* v */
{0x00, 0x00, 0x00, 0x28, 0x54, 0x54, 0x92, 0x82, 0x82, 0x00, 0x00}, /* w */
{0x00, 0x00, 0x00, 0x44, 0x28, 0x10, 0x10, 0x28, 0x44, 0x00, 0x00}, /* x */
{0x40, 0x20, 0x10, 0x08, 0x14, 0x24, 0x42, 0x42, 0x42, 0x00, 0x00}, /* y */

{0x00, 0x00, 0x00, 0x3c, 0x20, 0x10, 0x08, 0x04, 0x3c, 0x00, 0x00}, /* z */
{0x00, 0x00, 0x04, 0x08, 0x08, 0x08, 0x10, 0x08, 0x08, 0x08, 0x04}, /* { */
{0x00, 0x00, 0x00, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10}, /* | */
{0x00, 0x00, 0x20, 0x10, 0x10, 0x10, 0x08, 0x10, 0x10, 0x10, 0x20}, /* } */
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x92, 0x60, 0x00}  /* ~ */
};
      GLuint i, j;
      GLuint Offset;

      glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
      Offset = glGenLists(95);

      for (i = 0,j = ' '; i < 95; i++,j++) {
         glNewList(Offset + j, GL_COMPILE);
         glBitmap(8, 11, 0.0, 3.0, 9.0, 0.0, Font8x11[i]);
         glEndList();
      }

      return(Offset);
}
/**********************************************************************/
void DrawString8x11(const char *s)
{
      glPushAttrib (GL_LIST_BIT);
      glListBase(Font8x11Offset);
      glCallLists((GLsizei) strlen(s), GL_UNSIGNED_BYTE, (GLubyte *) s);
      glPopAttrib ();
}
/**********************************************************************/
/* Out-of-bounds colors can crash X server through glColor4fv.        */
/* This function clamps colors to [0.0:1.0] and                       */
/* returns 0 if color components in-range, 1 if not                   */
long ClampColor4fv(GLfloat *Color)
{
      long OutOfRange = 0;
      long i;

      for(i=0;i<4;i++) {
         if (Color[i] < 0.0f) {
            printf("Color[%ld] out of range (%f).  Clamped to 0.0\n",i,Color[i]);
            Color[i] = 0.0f;
            OutOfRange = 1;
         }
         if (Color[i] > 1.0f) {
            printf("Color[%ld] out of range (%f).  Clamped to 1.0\n",i,Color[i]);
            Color[i] = 1.0f;
            OutOfRange = 1;
         }
      }

      return(OutOfRange);
}
/**********************************************************************/
void FindSunColor(double T, GLfloat LightColor[3], GLfloat DiskColor[3])
{
      double rgb[3];
      double F1, F2, m;
      /* These wavelengths picked to make Sol (5800K) white */
      double lam1 = 0.552;
      double lam2 = 0.897;
      long i;
      long N = 60;
      double LT[60] = {  200.0,   400.0,   600.0,   800.0,  1000.0,
                        1200.0,  1400.0,  1600.0,  1800.0,  2000.0,
                        2200.0,  2400.0,  2600.0,  2800.0,  3000.0,
                        3200.0,  3400.0,  3600.0,  3800.0,  4000.0,
                        4200.0,  4400.0,  4600.0,  4800.0,  5000.0,
                        5200.0,  5400.0,  5600.0,  5800.0,  6000.0,
                        6200.0,  6400.0,  6600.0,  6800.0,  7000.0,
                        7200.0,  7400.0,  7600.0,  7800.0,  8000.0,
                        8500.0,  9000.0,  9500.0, 10000.0, 10500.0,
                       11000.0, 11500.0, 12000.0, 13000.0, 14000.0,
                       15000.0, 16000.0, 18000.0, 20000.0, 25000.0,
                       30000.0, 40000.0, 50000.0, 75000.0,100000.0};
      double F[60] = {0.000000, 0.000000, 0.000000, 0.000016, 0.000321,
                     0.002134, 0.007790, 0.019718, 0.039341, 0.066728,
                     0.100888, 0.140256, 0.183120, 0.227897, 0.273232,
                     0.318102, 0.361735, 0.403607, 0.443382, 0.480877,
                     0.516014, 0.548796, 0.579280, 0.607559, 0.633747,
                     0.658970, 0.680360, 0.701046, 0.720158, 0.737818,
                     0.754140, 0.769234, 0.783199, 0.796129, 0.808109,
                     0.819217, 0.829527, 0.839102, 0.848005, 0.856288,
                     0.874608, 0.890029, 0.903085, 0.914199, 0.923710,
                     0.931890, 0.939959, 0.945098, 0.955139, 0.962898,
                     0.969981, 0.973814, 0.980860, 0.985602, 0.992215,
                     0.995340, 0.997967, 0.998953, 0.999713, 0.999905};

      F1 = LinInterp(LT, F, lam1*T, N);
      F2 = LinInterp(LT, F, lam2*T, N);
      rgb[2] = F1;
      rgb[1] = F2 - F1;
      rgb[0] = 1.0 - F2;

      /* Scale max component to 1.0 */
      m = 0.0;
      for(i=0;i<3;i++) if (rgb[i] > m) m = rgb[i];
      for(i=0;i<3;i++) rgb[i] += 1.0-m;

      /* Pastelize */
      for(i=0;i<3;i++) LightColor[i] = 0.8+0.2*rgb[i];
      for(i=0;i<3;i++) DiskColor[i] = 0.6+0.4*rgb[i];

}
/**********************************************************************/
void DrawSkyGrid(GLfloat MajColor[4], GLfloat MinColor[4], double C[3][3],
                 GLuint MajList, GLuint MinList)
{

      GLfloat Black[4] = {0.0,0.0,0.0,1.0};

      glPushMatrix();
      RotateR2L(C);
      glDisable(GL_LIGHTING);
      glColor4fv(MinColor);
      glCallList(MinList);
      glColor4fv(MajColor);
      glCallList(MajList);
      glColor4fv(Black);
      glEnable(GL_LIGHTING);
      glPopMatrix();

}
/**********************************************************************/
void LoadSkyGrid(double MajGrid, double MinGrid, double SkyDistance,
                 GLuint *MajList, GLuint *MinList)
{
#define D2R (0.0174532925199433)

      double p[4];
      double LatMax,Lat1,Lat2,Lng1,Lng2,Gap;
      double lng,lat,c1,s1,c2,s2;
      long Ngrid,Igrid;
      char s[40];

      p[3] = 1.0/SkyDistance;

      *MinList = glGenLists(1);
      glNewList(*MinList,GL_COMPILE);

      LatMax = 90.0 - MinGrid;

      glLineWidth(2.0);

      for(lng = 0.0; lng < 360.0; lng += MinGrid) {
         c1=cos(lng*D2R);
         s1=sin(lng*D2R);
         glBegin(GL_LINE_STRIP);
            for(lat=-LatMax;lat<= -90.0+MajGrid;lat+=1.0) {
               c2=cos(lat*D2R);
               s2=sin(lat*D2R);
               p[0] = c1*c2;
               p[1] = s1*c2;
               p[2] = s2;
               glVertex4dv(p);
            }
         glEnd();
         glBegin(GL_LINE_STRIP);
            for(lat= 90.0-MajGrid;lat <= LatMax;lat+=1.0) {
               c2=cos(lat*D2R);
               s2=sin(lat*D2R);
               p[0] = c1*c2;
               p[1] = s1*c2;
               p[2] = s2;
               glVertex4dv(p);
            }
         glEnd();
         if (((long) lng) % ((long) MajGrid) != 0) {
            glBegin(GL_LINE_STRIP);
               for(lat=-LatMax;lat<=LatMax;lat+=1.0) {
                  c2=cos(lat*D2R);
                  s2=sin(lat*D2R);
                  p[0] = c1*c2;
                  p[1] = s1*c2;
                  p[2] = s2;
                  glVertex4dv(p);
               }
            glEnd();
         }
      }
      for(lat = 0.0; lat <= LatMax; lat += MinGrid) {
         if (((long) lat) % ((long) MajGrid) != 0) {
            c2=cos(lat*D2R);
            s2=sin(lat*D2R);
            glBegin(GL_LINE_LOOP);
               for(lng=0.0;lng<360.0;lng+=1.0) {
                  c1=cos(lng*D2R);
                  s1=sin(lng*D2R);
                  p[0] = c1*c2;
                  p[1] = s1*c2;
                  p[2] = s2;
                  glVertex4dv(p);
               }
            glEnd();
            glBegin(GL_LINE_LOOP);
               for(lng=0.0;lng<360.0;lng+=1.0){
                  c1=cos(lng*D2R);
                  s1=sin(lng*D2R);
                  p[0] =  c1*c2;
                  p[1] =  s1*c2;
                  p[2] = -s2;
                  glVertex4dv(p);
               }
            glEnd();
         }
      }

      glLineWidth(1.0);
      glEndList();

      *MajList = glGenLists(1);
      glNewList(*MajList,GL_COMPILE);

      LatMax = 90.0 - MajGrid;

      Ngrid = 2*((long) (LatMax/MajGrid));
      for(lng = 0.0; lng < 360.0; lng += MajGrid) {
         c1=cos(lng*D2R);
         s1=sin(lng*D2R);
         for(Igrid=0;Igrid<Ngrid;Igrid++) {
            Lat1 = -LatMax+Igrid*MajGrid;
            Lat2 = Lat1 + 0.45*MajGrid;
            lat = Lat1 + 0.5*MajGrid;
            c2=cos(lat*D2R);
            s2=sin(lat*D2R);
            p[0] = c1*c2;
            p[1] = s1*c2;
            p[2] = s2;
            glRasterPos4dv(p);
            sprintf(s,"%i",(int) lng);
            glBitmap(0,0,0,0,-4.0*strlen(s),-5,0);
            DrawString8x11(s);
            glBegin(GL_LINE_STRIP);
               for(lat=Lat1;lat<=Lat2;lat+=1.0) {
                  c2=cos(lat*D2R);
                  s2=sin(lat*D2R);
                  p[0] = c1*c2;
                  p[1] = s1*c2;
                  p[2] = s2;
                  glVertex4dv(p);
               }
            glEnd();
            Lat2 = Lat1 + MajGrid;
            Lat1 = Lat2 -0.45*MajGrid;
            glBegin(GL_LINE_STRIP);
               for(lat=Lat2;lat>=Lat1;lat-=1.0) {
                  c2=cos(lat*D2R);
                  s2=sin(lat*D2R);
                  p[0] = c1*c2;
                  p[1] = s1*c2;
                  p[2] = s2;
                  glVertex4dv(p);
               }
            glEnd();
         }
      }

      Ngrid = (long) (360.0/MajGrid);
      for(lat = -LatMax; lat <= LatMax; lat += MajGrid) {
         c2=cos(lat*D2R);
         s2=sin(lat*D2R);
         for(Igrid=0;Igrid<Ngrid;Igrid++) {
            Lng1 = Igrid*MajGrid;
            Lng2 = Lng1 + MajGrid;
            Gap = 0.05/c2*MajGrid;
            lng = Lng1 + 0.5*MajGrid;
            c1=cos(lng*D2R);
            s1=sin(lng*D2R);
            p[0] = c1*c2;
            p[1] = s1*c2;
            p[2] = s2;
            glRasterPos4dv(p);
            sprintf(s,"%i",(int) lat);
            glBitmap(0,0,0,0,-4.0*strlen(s),-5,0);
            DrawString8x11(s);
            glBegin(GL_LINE_STRIP);
               for(lng=Lng1;lng<=Lng1+0.5*MajGrid-Gap;lng+=0.2) {
                  c1=cos(lng*D2R);
                  s1=sin(lng*D2R);
                  p[0] = c1*c2;
                  p[1] = s1*c2;
                  p[2] = s2;
                  glVertex4dv(p);
               }
            glEnd();
            glBegin(GL_LINE_STRIP);
               for(lng=Lng1+0.5*MajGrid+Gap;lng<=Lng2;lng+=0.2) {
                  c1=cos(lng*D2R);
                  s1=sin(lng*D2R);
                  p[0] = c1*c2;
                  p[1] = s1*c2;
                  p[2] = s2;
                  glVertex4dv(p);
               }
            glEnd();
         }
      }
      glPointSize(4.0);
      glBegin(GL_POINTS);
         glVertex4d(0.0,0.0,1.0,1.0/SkyDistance);
         glVertex4d(0.0,0.0,-1.0,1.0/SkyDistance);
      glEnd();
      glPointSize(1.0);

      glRasterPos3d(0.0,0.0,SkyDistance);
      DrawString8x11("+90");
      glRasterPos3d(0.0,0.0,-SkyDistance);
      DrawString8x11("-90");

      glEndList();

#undef D2R
}
/*********************************************************************/
void DrawArrowhead(double v[3],double scale)
{
      double X[3],Y[3];
      double c[13] = {1.0,0.866,0.5,0.0,-0.5,-0.866,-1.0,-0.866,-0.5,0.0,0.5,0.866,1.0};
      double s[13] = {0.0,0.5,0.866,1.0,0.866,0.5,0.0,-0.5,-0.866,-1.0,-0.866,-0.5,0.0};
      long i;

      PerpBasis(v,X,Y);

      glBegin(GL_TRIANGLE_FAN);
         glVertex3d(scale*v[0],scale*v[1],scale*v[2]);
         for(i=0;i<13;i++)
            glVertex3d(scale*(0.9*v[0]+0.025*(c[i]*X[0]+s[i]*Y[0])),
                       scale*(0.9*v[1]+0.025*(c[i]*X[1]+s[i]*Y[1])),
                       scale*(0.9*v[2]+0.025*(c[i]*X[2]+s[i]*Y[2])));
      glEnd();
      glBegin(GL_POLYGON);
         for(i=12;i>=0;i--)
            glVertex3d(scale*(0.9*v[0]+0.025*(c[i]*X[0]+s[i]*Y[0])),
                       scale*(0.9*v[1]+0.025*(c[i]*X[1]+s[i]*Y[1])),
                       scale*(0.9*v[2]+0.025*(c[i]*X[2]+s[i]*Y[2])));
      glEnd();
}
/*********************************************************************/
void DrawNearFOV(long Nv,double Width,double Height,double Length,
   long BoreAxis, long H_Axis, long V_Axis, long Type,GLfloat Color[4])
{
      double c[13] = {1.0,0.866,0.5,0.0,-0.5,-0.866,-1.0,-0.866,-0.5,0.0,0.5,0.866,1.0};
      double s[13] = {0.0,0.5,0.866,1.0,0.866,0.5,0.0,-0.5,-0.866,-1.0,-0.866,-0.5,0.0};
      long i;
      double Apex[4] = {0.0,0.0,0.0,1.0};
      double TwoPi = 6.28318530717959;
      double r[4];
      double daz = TwoPi/((double) Nv);
      double az0 = 0.5*daz;
      double az;
      double a,b;
      long jmax;

      r[BoreAxis] = 1.0;
      ClampColor4fv(Color);
      glColor4fv(Color);
      glDisable(GL_CULL_FACE);
      r[3] = 1.0/Length;

      jmax = (long) (Nv/4)+1;
      a = tan(Width/2.0)/cos(0.5*daz);
      b = tan(Height/2.0)/sin((((double) jmax)-0.5)*daz);

      if (Type == 0) { /* FOV_WIREFRAME */
         glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
         glBegin(GL_TRIANGLE_FAN);
            glVertex4dv(Apex);
            for(az=az0;az<TwoPi;az+=daz) {
               r[H_Axis] = a*cos(az);
               r[V_Axis] = b*sin(az);
               glVertex4dv(r);
            }
            r[H_Axis] = a*cos(az0);
            r[V_Axis] = b*sin(az0);
            glVertex4dv(r);
         glEnd();
         glPolygonMode(GL_FRONT, GL_FILL);
      }
      else if (Type == 1) { /* FOV_SOLID */
         glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
         glBegin(GL_TRIANGLE_FAN);
            glVertex4dv(Apex);
            for(az=az0;az<TwoPi;az+=daz) {
               r[H_Axis] = a*cos(az);
               r[V_Axis] = b*sin(az);
               glVertex4dv(r);
            }
            r[H_Axis] = a*cos(az0);
            r[V_Axis] = b*sin(az0);
            glVertex4dv(r);
         glEnd();
      }
      else if (Type == 2) { /* FOV_VECTOR */
         glLineWidth(2.0);
         glBegin(GL_LINES);
            glVertex4dv(Apex);
            r[H_Axis] = 0.0;
            r[V_Axis] = 0.0;
            glVertex4dv(r);
         glEnd();
         glLineWidth(1.0);
         glBegin(GL_TRIANGLE_FAN);
            glVertex4dv(r);
            for(i=0;i<13;i++)
               glVertex4d(0.03*c[i],0.03*s[i],0.9,r[3]);
         glEnd();
         glBegin(GL_POLYGON);
            for(i=12;i>=0;i--)
               glVertex4d(0.03*c[i],0.03*s[i],0.9,r[3]);
         glEnd();
      }
      else if (Type == 3) { /* FOV_PLANE */
         glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
         glBegin(GL_TRIANGLE_FAN);
            glVertex4dv(Apex);
            r[BoreAxis] = 0.0;
            for(az=az0;az<TwoPi;az+=daz) {
               r[H_Axis] = cos(az);
               r[V_Axis] = sin(az);
               glVertex4dv(r);
            }
            r[H_Axis] = cos(az0);
            r[V_Axis] = sin(az0);
            glVertex4dv(r);
         glEnd();
      }
      else {
         printf("Invalid FOV Type %ld in DrawNearFOV\n",Type);
         exit(1);
      }

      glEnable(GL_CULL_FACE);
}
/*********************************************************************/
void DrawFarFOV(long Nv,double Width,double Height, 
   long BoreAxis, long H_Axis, long V_Axis, long Type, GLfloat Color[4],
                const char Label[40], double SkyDistance)
{
      double TwoPi = 6.28318530717959;
      double r[4];
      double daz = TwoPi/((double) Nv);
      double az0 = 0.5*daz;
      double az;
      double a,b;
      long jmax;

      ClampColor4fv(Color);
      glColor4fv(Color);
      r[BoreAxis] = 1.0;
      r[3] = 1.0/SkyDistance;

      jmax = (long) (Nv/4)+1;
      a = tan(Width/2.0)/cos(0.5*daz);
      b = tan(Height/2.0)/sin((((double) jmax)-0.5)*daz);

      if (Type == 0 || Type == 1) { /* FOV_WIREFRAME or FOV_SOLID */
         glLineWidth(2.0);
         glBegin(GL_LINE_LOOP);
            for(az=az0;az<TwoPi;az+=daz) {
               r[H_Axis] = a*cos(az);
               r[V_Axis] = b*sin(az);
               glVertex4dv(r);
            }
         glEnd();
         glLineWidth(1.0);
         /* Label */
         r[H_Axis] = 0.0;
         r[V_Axis] = 0.0;
         glRasterPos4dv(r);
         glBitmap(0,0,0,0,-4.0*strlen(Label),-5,0);
         DrawString8x11(Label);
      }
      else if (Type == 2) { /* FOV_VECTOR */
         glPointSize(4.0);
         glBegin(GL_POINTS);
            r[H_Axis] = 0.0;
            r[V_Axis] = 0.0;
            glVertex4dv(r);
         glEnd();
         /* Label */
         r[H_Axis] = 0.0;
         r[V_Axis] = 0.0;
         glRasterPos4dv(r);
         glBitmap(0,0,0,0,-4.0*strlen(Label),-17,0);
         DrawString8x11(Label);
         glPointSize(1.0);
      }
      else if (Type == 3) { /* FOV_PLANE */
         glLineWidth(2.0);
         r[BoreAxis] = 0.0;
         glBegin(GL_LINE_LOOP);
            for(az=az0;az<TwoPi;az+=daz) {
               r[H_Axis] = cos(az);
               r[V_Axis] = sin(az);
               glVertex4dv(r);
            }
         glEnd();
         glLineWidth(1.0);
         /* Label */
         for(az=az0;az<TwoPi;az+=daz) {
            r[H_Axis] = cos(az);
            r[V_Axis] = sin(az);
            glRasterPos4dv(r);
            glBitmap(0,0,0,0,-4.0*strlen(Label),-5,0);
            DrawString8x11(Label);
         }
      }
      else {
         printf("Invalid FOV Type %ld in DrawNearFOV\n",Type);
         exit(1);
      }


}
/*********************************************************************/
void RotateL2R(double C[3][3])
{
      float M[16]={1.0,0.0,0.0,0.0,
                   0.0,1.0,0.0,0.0,
                   0.0,0.0,1.0,0.0,
                   0.0,0.0,0.0,1.0};
      long i,j;

      for(i=0;i<3;i++) {
         for(j=0;j<3;j++) M[4*i+j] = (float) C[j][i];
      }
      glMultMatrixf(M);
}
/*********************************************************************/
void RotateR2L(double C[3][3])
{
      float M[16]={1.0,0.0,0.0,0.0,
                   0.0,1.0,0.0,0.0,
                   0.0,0.0,1.0,0.0,
                   0.0,0.0,0.0,1.0};
      long i,j;

      for(i=0;i<3;i++) {
         for(j=0;j<3;j++) M[4*i+j] = (float) C[i][j];
      }
      glMultMatrixf(M);
}
/**********************************************************************/
/* OpenGL stores its 4x4 transformation matrices columnwise in        */
/* 16-element arrays                                                  */
void MxM4f(float A[16], float B[16], float C[16])
{
      long i,j,k;
      for(i=0;i<4;i++) {
         for(j=0;j<4;j++) {
            C[4*i+j] = 0.0;
            for(k=0;k<4;k++) C[4*i+j] += A[4*k+j]*B[4*i+k];
         }
      }
}
/**********************************************************************/
float Det3x3(float A11,float A12, float A13,
             float A21, float A22, float A23,
             float A31, float A32, float A33)
{
      return( A11*A22*A23 + A12*A23*A31 + A13*A21*A32
             -A11*A23*A32 - A12*A21*A33 - A13*A22*A31);
}
/**********************************************************************/
/* OpenGL stores its 4x4 transformation matrices columnwise in        */
/* 16-element arrays                                                  */
void Minv4f(float A[16], float Ai[16])
{
      float DetA;
      float a11,a12,a13,a14;
      float a21,a22,a23,a24;
      float a31,a32,a33,a34;
      float a41,a42,a43,a44;
      float b11,b12,b13,b14;
      float b21,b22,b23,b24;
      float b31,b32,b33,b34;
      float b41,b42,b43,b44;

      a11 = A[0];
      a21 = A[1];
      a31 = A[2];
      a41 = A[3];

      a12 = A[4];
      a22 = A[5];
      a32 = A[6];
      a42 = A[7];

      a13 = A[8];
      a23 = A[9];
      a33 = A[10];
      a43 = A[11];

      a14 = A[12];
      a24 = A[13];
      a34 = A[14];
      a44 = A[15];

      DetA = a11*a22*a33*a44 + a11*a23*a34*a42 + a11*a24*a32*a43
           + a12*a21*a34*a43 + a12*a23*a31*a44 + a12*a24*a33*a41
           + a13*a21*a32*a42 + a13*a22*a34*a41 + a13*a24*a31*a42
           + a14*a21*a33*a42 + a14*a22*a31*a43 + a14*a23*a32*a41
           - a11*a22*a34*a43 - a11*a23*a32*a44 - a11*a24*a33*a42
           - a12*a21*a33*a44 - a12*a23*a34*a41 - a12*a24*a31*a43
           - a13*a21*a34*a42 - a13*a22*a31*a44 - a13*a24*a32*a41
           - a14*a21*a32*a43 - a14*a22*a33*a41 - a14*a23*a31*a42;

      b11 = a22*a33*a44 + a23*a34*a42 + a24*a32*a43
          - a22*a34*a43 - a23*a32*a44 - a24*a33*a42;

      b12 = a12*a34*a43 + a13*a32*a44 + a14*a33*a42
          - a12*a33*a44 - a13*a34*a42 - a14*a32*a43;

      b13 = a12*a23*a44 + a13*a24*a42 + a14*a22*a43
          - a12*a24*a43 - a13*a22*a44 - a14*a23*a42;

      b14 = a12*a24*a33 + a13*a22*a34 + a14*a23*a32
          - a12*a23*a34 - a13*a24*a32 - a14*a22*a33;

      b21 = a21*a34*a43 + a23*a31*a44 + a24*a33*a41
          - a21*a33*a44 - a23*a34*a41 - a24*a31*a43;

      b22 = a11*a33*a44 + a13*a34*a41 + a14*a31*a43
          - a11*a34*a43 - a13*a31*a44 - a14*a33*a41;

      b23 = a11*a24*a43 + a13*a21*a44 + a14*a23*a41
          - a11*a23*a44 - a13*a24*a41 - a14*a21*a43;

      b24 = a11*a23*a34 + a13*a24*a31 + a14*a21*a33
          - a11*a24*a33 - a13*a21*a34 - a14*a23*a31;

      b31 = a21*a32*a44 + a22*a34*a41 + a24*a31*a42
          - a21*a34*a42 - a22*a31*a44 - a24*a32*a41;

      b32 = a11*a34*a42 + a12*a31*a44 + a14*a32*a41
          - a11*a32*a44 - a12*a34*a41 - a14*a31*a42;

      b33 = a11*a22*a44 + a12*a24*a41 + a14*a21*a42
          - a11*a24*a42 - a12*a21*a44 - a14*a22*a41;

      b34 = a11*a24*a32 + a12*a21*a34 + a14*a22*a31
          - a11*a22*a34 - a12*a24*a31 - a14*a21*a32;

      b41 = a21*a33*a42 + a22*a31*a43 + a23*a32*a41
          - a21*a32*a43 - a22*a33*a41 - a23*a31*a42;

      b42 = a11*a32*a43 + a12*a33*a41 + a13*a31*a42
          - a11*a33*a42 - a12*a31*a43 - a13*a32*a41;

      b43 = a11*a23*a42 + a12*a21*a43 + a13*a22*a41
          - a11*a22*a43 - a12*a23*a41 - a13*a21*a42;

      b44 = a11*a22*a33 + a12*a23*a31 + a13*a21*a32
          - a11*a23*a32 - a12*a21*a33 - a13*a22*a31;

      Ai[ 0] = b11/DetA;
      Ai[ 1] = b21/DetA;
      Ai[ 2] = b31/DetA;
      Ai[ 3] = b41/DetA;
      Ai[ 4] = b12/DetA;
      Ai[ 5] = b22/DetA;
      Ai[ 6] = b32/DetA;
      Ai[ 7] = b42/DetA;
      Ai[ 8] = b13/DetA;
      Ai[ 9] = b23/DetA;
      Ai[10] = b33/DetA;
      Ai[11] = b43/DetA;
      Ai[12] = b14/DetA;
      Ai[13] = b24/DetA;
      Ai[14] = b34/DetA;
      Ai[15] = b44/DetA;

}
/**********************************************************************/
void BuildModelMatrix(double CBN[3][3], double pbn[3],
   float ModelMatrix[16])
{
      long i,j;

      for(i=0;i<3;i++) {
         for(j=0;j<3;j++) {
            ModelMatrix[4*i+j] = (float) CBN[i][j];
         }
         ModelMatrix[12+i] = (float) pbn[i];
         ModelMatrix[4*i+3] = 0.0f;
      }
      ModelMatrix[15] = 1.0f;
}
/**********************************************************************/
void BuildViewMatrix(double CEN[3][3], double pen[3],
   float ViewMatrix[16])
{
      double pene[3];
      long i,j;

      MxV(CEN,pen,pene);
      for(i=0;i<3;i++) {
         for(j=0;j<3;j++) {
            ViewMatrix[4*i+j] = (float) CEN[j][i];
         }
         ViewMatrix[12+i] = (float) (-pene[i]);
         ViewMatrix[4*i+3] = 0.0f;
      }
      ViewMatrix[15] = 1.0f;
}
/**********************************************************************/
void CaptureScreenToPpm(const char path[40], const char filename[40],
                        long Nh, long Nw)
{
      GLubyte *Data,*p;
      FILE *file;
      long i,j,Nr;

      Nr = 3*Nw;
      Data = (GLubyte *) calloc(3*Nh*Nw,sizeof(GLubyte));
      if (Data == NULL) {
         printf("calloc returned null pointer in CaptureScreenToPpm.  Bailing out!\n");
         exit(1);
      }
      glReadPixels(0,0,Nw,Nh,GL_RGB,GL_UNSIGNED_BYTE,Data);
      file = FileOpen(path,filename,"wb");
      fprintf(file,"P6\n#.\n%ld %ld \n255\n",Nw,Nh);
      /*fprintf(file,"P6\f#.\f%ld %ld \f255\f",Nw,Nh);*/
      fflush(file);
      /* Flip Y */
      for(i=Nh-1;i>=0;i--) {
         p = Data+Nr*i;
         for(j=0;j<Nr;j++) putc(*(p+j),file);
      }
      fclose(file);
      free(Data);
}
/**********************************************************************/
void TexToPpm(const char path[40], const char filename[40],
              long Nh, long Nw, long Nb, float *Data)
{
      FILE *file;
      long i,Nc;
      GLubyte c;

      file = FileOpen(path,filename,"wb");
      if (Nb==1) fprintf(file,"P5\n#.\n%ld %ld\n255\n",Nw,Nh);
      else fprintf(file,"P6\n#.\n%ld %ld\n255\n",Nw,Nh);
      fflush(file);
      Nc = Nh*Nw*Nb;
      for(i=0;i<Nc;i++) {
         c = (GLubyte) (255.0*Data[i]);
         fprintf(file,"%c",c);
      }
      fclose(file);
}
/**********************************************************************/
/* wrap parm is usually either GL_REPEAT or GL_CLAMP                  */
GLuint PpmToTexTag(const char path[40], const char filename[40],int BytesPerPixel,
                   GLuint wrap)
{
      FILE *infile;
      long N,i;
      long Nh,Nw,junk;
      char comment[80];
      GLuint TexTag;
      GLubyte *Tex;

      infile = FileOpen(path,filename,"rb");
      fscanf(infile,"P6\n%[^\n]\n%ld %ld\n%ld\n",comment,&Nw,&Nh,&junk); /* Header */
      N = Nw*Nh*BytesPerPixel;
      Tex = (GLubyte *) calloc(N,sizeof(GLubyte));
      if (Tex == NULL) {
         printf("calloc returned null pointer in PpmToTexTag.  Bailing out!\n");
         exit(1);
      }
      for(i=0;i<N;i++) {
         Tex[i] = (GLubyte) fgetc(infile);
      }
      fclose(infile);

      glGenTextures(1,&TexTag);
      glBindTexture(GL_TEXTURE_2D,TexTag);
      glTexParameteri(GL_TEXTURE_2D,GL_GENERATE_MIPMAP,GL_TRUE);
      glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,wrap);
      glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,wrap);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      if (BytesPerPixel == 1) {
         glTexImage2D(GL_TEXTURE_2D,0,GL_LUMINANCE,Nw,Nh,0,GL_LUMINANCE,
            GL_UNSIGNED_BYTE,Tex);
      }
      else if (BytesPerPixel == 3) {
         glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,Nw,Nh,0,GL_RGB,
            GL_UNSIGNED_BYTE,Tex);
      }
      else if (BytesPerPixel == 4) {
         glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,Nw,Nh,0,GL_RGBA,
            GL_UNSIGNED_BYTE,Tex);
      }

      free(Tex);
      return(TexTag);
}
/**********************************************************************/
/* wrap parm is usually either GL_REPEAT or GL_CLAMP                  */
GLuint Ppm1DToTexTag(const char path[40], const char filename[40],int BytesPerPixel,
                   GLuint wrap)
{
      FILE *infile;
      long N,i;
      long Nh,Nw,junk;
      char comment[80];
      GLuint TexTag;
      GLubyte *Tex;

      infile = FileOpen(path,filename,"rb");
      fscanf(infile,"P6\n%[^\n]\n%ld %ld\n%ld\n",comment,&Nw,&Nh,&junk); /* Header */
      N = Nw*BytesPerPixel;
      Tex = (GLubyte *) calloc(N,sizeof(GLubyte));
      if (Tex == NULL) {
         printf("calloc returned null pointer in PpmToTexTag.  Bailing out!\n");
         exit(1);
      }
      for(i=0;i<N;i++) {
         Tex[i] = (GLubyte) fgetc(infile);
      }
      fclose(infile);

      glGenTextures(1,&TexTag);
      glBindTexture(GL_TEXTURE_1D,TexTag);
      glTexParameteri(GL_TEXTURE_1D,GL_GENERATE_MIPMAP,GL_TRUE);
      glTexParameteri(GL_TEXTURE_1D,GL_TEXTURE_WRAP_S,wrap);
      glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
      glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      if (BytesPerPixel == 1) {
         glTexImage1D(GL_TEXTURE_1D,0,GL_LUMINANCE,Nw,0,GL_LUMINANCE,
            GL_UNSIGNED_BYTE,Tex);
      }
      else if (BytesPerPixel == 3) {
         glTexImage1D(GL_TEXTURE_1D,0,GL_RGB,Nw,0,GL_RGB,
            GL_UNSIGNED_BYTE,Tex);
      }
      else if (BytesPerPixel == 4) {
         glTexImage1D(GL_TEXTURE_1D,0,GL_RGBA,Nw,0,GL_RGBA,
            GL_UNSIGNED_BYTE,Tex);
      }

      free(Tex);
      return(TexTag);
}
/*********************************************************************/
#if 1
GLuint PpmToCubeTag(const char path[40], const char file[40], int BytesPerPixel)
{
      FILE *infile;
      long N,i,If;
      long Nh,Nw,junk;
      char comment[80];
      GLuint CubeTag;
      GLubyte *Tex;
      char PpmFace[6][20] = {"PX.ppm","PY.ppm","PZ.ppm",
                          "MX.ppm","MY.ppm","MZ.ppm"};
      char PgmFace[6][20] = {"PX.pgm","PY.pgm","PZ.pgm",
                          "MX.pgm","MY.pgm","MZ.pgm"};
      GLenum CubeFace[6] = {GL_TEXTURE_CUBE_MAP_POSITIVE_X,
                            GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
                            GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
                            GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
                            GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
                            GL_TEXTURE_CUBE_MAP_NEGATIVE_Z};
      char filename[40];

      glGenTextures(1,&CubeTag);
      glBindTexture(GL_TEXTURE_CUBE_MAP,CubeTag);

      glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_R,GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
      glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_GENERATE_MIPMAP,GL_TRUE);

      for(If=0;If<6;If++) {
         strcpy(filename,file);
         if (BytesPerPixel == 1) strcat(filename,PgmFace[If]);
         else strcat(filename,PpmFace[If]);
         infile = FileOpen(path,filename,"rb");
         if (BytesPerPixel == 1) {
            fscanf(infile,"P5\n%[^\n]\n%ld %ld\n%ld\n",comment,&Nw,&Nh,&junk); /* Header */
         }
         else {
            fscanf(infile,"P6\n%[^\n]\n%ld %ld\n%ld\n",comment,&Nw,&Nh,&junk); /* Header */
         }
         N = Nw*Nh*BytesPerPixel;
         Tex = (GLubyte *) calloc(N,sizeof(GLubyte));
         if (Tex == NULL) {
            printf("calloc returned null pointer in PpmToCubeTag.  Bailing out!\n");
            exit(1);
         }
         for(i=0;i<N;i++) {
            Tex[i] = (GLubyte) fgetc(infile);
         }
         fclose(infile);

         if (BytesPerPixel == 1)
            glTexImage2D(CubeFace[If],0,GL_LUMINANCE,Nw,Nh,0,GL_LUMINANCE,
               GL_UNSIGNED_BYTE,Tex);
         else if (BytesPerPixel == 3)
            glTexImage2D(CubeFace[If],0,GL_RGB,Nw,Nh,0,GL_RGB,
               GL_UNSIGNED_BYTE,Tex);
         else
            glTexImage2D(CubeFace[If],0,GL_RGBA,Nw,Nh,0,GL_RGBA,
               GL_UNSIGNED_BYTE,Tex);

         free(Tex);

      }
      return(CubeTag);
}
#endif
/*********************************************************************/
GLuint PpmToRingTexTag(const char path[80], const char filename[80])
{
      FILE *infile;
      long N;
      long Nh,Nw,junk;
      char comment[80];
      long k,i;
      GLubyte *Tex;
      GLubyte InByte[4];
      float Intensity;
      GLuint TexTag;

      infile = FileOpen(path,filename,"rb");
      fscanf(infile,"P6\n%[^\n]\n%ld %ld\n%ld\n",comment,&Nw,&Nh,&junk); /* Header */
      N = Nw*4;
      Tex = (GLubyte *) calloc(N,sizeof(GLubyte));
      if (Tex == NULL) {
         printf("calloc returned null pointer in PpmToTexTag.  Bailing out!\n");
         exit(1);
      }
      for(k=0;k<Nw;k++) {
         fscanf(infile,"%c%c%c%c",
         &InByte[0],&InByte[1],&InByte[2],&InByte[3]);
         for(i=0;i<4;i++) {
            Intensity = ((float) InByte[i])/255.0;
            Tex[4*k+i] = (GLubyte) (255.0*Intensity);
         }
      }
      fclose(infile);

      /* Ensure edges of ring are transparent.  Helps ring shadow on planet */
      Tex[3] = (GLubyte) 0;
      Tex[N-1] = (GLubyte) 0;

      glGenTextures(1,&TexTag);
      glBindTexture(GL_TEXTURE_1D,TexTag);
      glTexParameteri(GL_TEXTURE_1D,GL_TEXTURE_WRAP_S,GL_CLAMP);
      glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexImage1D(GL_TEXTURE_1D,0,GL_RGBA,Nw,0,GL_RGBA,
         GL_UNSIGNED_BYTE,Tex);

      free(Tex);
      return(TexTag);
}
/**********************************************************************/
void CubeToPpm(GLubyte *Cube, long N, const char pathname[40], const char filename[40])
{
      FILE *outfile;
      char outfilename[40];
      GLubyte R,G,B;
      long face,i,j;
      long idx = 0;
      char Face[6][20] = {"PX.ppm","PY.ppm","PZ.ppm",
                          "MX.ppm","MY.ppm","MZ.ppm"};

      for(face=0;face<6;face++) {
         strcpy(outfilename,filename);
         strcat(outfilename,Face[face]);
         outfile = FileOpen(pathname,outfilename,"w");
         fprintf(outfile,"P6\n#.\n%ld %ld\n255\n",N,N);
         /*fprintf(outfile,"P6\f#.\f%ld %ld\f255\f",N,N);*/
         fflush(outfile);
         for(i=0;i<N;i++) {
            for(j=0;j<N;j++) {
               R = Cube[idx++];
               G = Cube[idx++];
               B = Cube[idx++];
               fprintf(outfile,"%c%c%c",R,G,B);
            }
         }
         fclose(outfile);
      }
}
/*********************************************************************/
void LoadBucky(double BuckyPf[32][3], long BuckyNeighbor[32][6])
{

      double IcoPv[12][3] = {{0.0,0.0,1.0},
         {0.89442680992,0.0,0.44721435766},
         {0.27639308449,0.850650445924,0.44721435766},
         {-0.72360648945,0.525730888126,0.44721435766},
         {-0.72360648945,-0.525730888126,0.44721435766},
         {0.27639308449,-0.850650445924,0.44721435766},
         {0.72360648945,0.525730888126,-0.44721435766},
         {-0.27639308449,0.850650445924,-0.44721435766},
         {-0.89442680992,0.0,-0.44721435766},
         {-0.27639308449,-0.850650445924,-0.44721435766},
         {0.72360648945,-0.525730888126,-0.44721435766},
         {0.0,0.0,-1.0}};
      long BN[32][6] = {{12,13,14,15,16,-1},{12,16,17,21,26,-1},
                         {12,13,17,18,22,-1},{13,14,18,19,23,-1},
                         {14,15,19,20,24,-1},{15,16,20,21,25,-1},
                         {17,22,26,27,31,-1},{18,22,23,27,28,-1},
                         {19,23,24,28,29,-1},{20,24,25,29,30,-1},
                         {21,25,26,30,31,-1},{27,28,29,30,31,-1},
                         { 0, 1, 2,13,16,17},{ 0, 2, 3,12,14,18},
                         { 0, 3, 4,13,15,19},{ 0, 4, 5,14,16,20},
                         { 0, 1, 5,12,15,21},{ 1, 2, 6,12,22,26},
                         { 2, 3, 7,13,22,23},{ 3, 4, 8,14,23,24},
                         { 4, 5, 9,15,24,25},{ 1, 5,10,16,25,26},
                         { 2, 6, 7,17,18,27},{ 3, 7, 8,18,19,28},
                         { 4, 8, 9,19,20,29},{ 5, 9,10,20,21,30},
                         { 1, 6,10,17,21,31},{ 6, 7,11,22,28,31},
                         { 7, 8,11,23,27,29},{ 8, 9,11,24,28,30},
                         { 9,10,11,25,29,31},{ 6,10,11,26,27,30}};

      long i,j;

      for(i=0;i<32;i++) {
         for(j=0;j<6;j++) {
            BuckyNeighbor[i][j] = BN[i][j];
         }
      }

      /* Initialize Buckyball */
      for(i=0;i<12;i++) {
         for(j=0;j<3;j++) BuckyPf[i][j] = IcoPv[i][j];
      }
      for(i=12;i<32;i++) {
         for(j=0;j<3;j++) {
            BuckyPf[i][j] = (BuckyPf[BuckyNeighbor[i][0]][j]
                            +BuckyPf[BuckyNeighbor[i][1]][j]
                            +BuckyPf[BuckyNeighbor[i][2]][j])/3.0;
         }
         UNITV(BuckyPf[i]);
      }
}
/**********************************************************************/
void LoadStars(const char StarFileName[40],double BuckyPf[32][3],
               long BuckyNeighbor[32][6],GLuint StarList[32],
               double SkyDistance)
{
      /*double Fade = 0.4581;*/  /* log(sqrt(2.5)) */
      double MaxPoR,PoR;
      long ClosestVtx;
      long **ID,N[32];
      long i,j,k,Nstar;
      GLfloat Col;
      FILE *StarFile;
      struct StarType {
         double r[4];
         double m;
         GLfloat Color[4];
         double v1[4],v2[4],v3[4],v4[4];
      };
      struct StarType *Star;
      double qnh[4] = {-0.203123038887,  0.0,  0.0,  0.979153221449};
      double r[3],uhat[3],vhat[3];
      double QuadSize;
      GLuint StarTexTag;

      LoadBucky(BuckyPf,BuckyNeighbor);
      StarTexTag = PpmToTexTag("./Model/","StarTexture.ppm",4,GL_REPEAT);

      for(k=0;k<32;k++) N[k] = 0;

      StarFile = FileOpen("./Model/",StarFileName,"r");
      fscanf(StarFile,"%ld",&Nstar);
      ID = (long **) calloc(32,sizeof(long *));
      if (ID == NULL) {
         printf("ID calloc returned null pointer in LoadStars.  Bailing out!\n");
         exit(1);
      }
      for(k=0;k<32;k++) {
         ID[k] = (long *) calloc(Nstar,sizeof(long));
         if (ID[k] == NULL) {
            printf("ID[k] calloc returned null pointer in LoadStars.  Bailing out!\n");
            exit(1);
         }
      }
      Star = (struct StarType *) calloc(Nstar,sizeof(struct StarType));
      if (Star == NULL) {
         printf("Star calloc returned null pointer in LoadStars.  Bailing out!\n");
         exit(1);
      }

      for(i=0;i<Nstar;i++) {
         fscanf(StarFile,"%lf %lf %lf %lf",
            &r[0],&r[1],&r[2],
            &Star[i].m);

         /* Transform from input frame (probably ECI) to H frame */
         QTxV(qnh,r,Star[i].r);

         PerpBasis(Star[i].r,uhat,vhat);
         QuadSize = 0.0016*(10.0-Star[i].m);
         for(k=0;k<3;k++) {
            Star[i].v1[k] = Star[i].r[k] - QuadSize*uhat[k] - QuadSize*vhat[k];
            Star[i].v2[k] = Star[i].r[k] - QuadSize*uhat[k] + QuadSize*vhat[k];
            Star[i].v3[k] = Star[i].r[k] + QuadSize*uhat[k] + QuadSize*vhat[k];
            Star[i].v4[k] = Star[i].r[k] + QuadSize*uhat[k] - QuadSize*vhat[k];
         }

         Star[i].r[3] = 1.0/SkyDistance;
         Star[i].v1[3] = 1.0/SkyDistance;
         Star[i].v2[3] = 1.0/SkyDistance;
         Star[i].v3[3] = 1.0/SkyDistance;
         Star[i].v4[3] = 1.0/SkyDistance;

         /*Col = 1.0;*/
         if (Star[i].m <= 0.0) Col = 1.0;
         /*else Col = 0.25+0.75*exp(-Fade*Star[i].m);*/
         else if (Star[i].m > 9.0) Col = 0.0;
         else Col = 1.0-Star[i].m/18.0;
         Star[i].Color[0] = Col;
         Star[i].Color[1] = Col;
         Star[i].Color[2] = Col;
         Star[i].Color[3] = 1.0;

      }

      for(i=0;i<Nstar;i++) {
         MaxPoR = VoV(BuckyPf[0],Star[i].r);
         ClosestVtx = 0;
         for(k=1;k<32;k++) {
            PoR = VoV(BuckyPf[k],Star[i].r);
            if (PoR > MaxPoR) {
               MaxPoR = PoR;
               ClosestVtx = k;
            }
         }
         ID[ClosestVtx][N[ClosestVtx]] = i;
         N[ClosestVtx]++;
      }

      for(k=0;k<32;k++) {
         StarList[k] = glGenLists(1);
         glNewList(StarList[k],GL_COMPILE);
            glActiveTexture(GL_TEXTURE0);
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D,StarTexTag);
            glBegin(GL_QUADS);
            for(i=0;i<N[k];i++) {
               j=ID[k][i];
               glColor4fv(Star[j].Color);
               glTexCoord2f(0.0,0.0);
               glVertex4dv(Star[j].v1);
               glTexCoord2f(1.0,0.0);
               glVertex4dv(Star[j].v2);
               glTexCoord2f(1.0,1.0);
               glVertex4dv(Star[j].v3);
               glTexCoord2f(0.0,1.0);
               glVertex4dv(Star[j].v4);
            }
            glEnd();
            glDisable(GL_TEXTURE_2D);
         glEndList();
      }

      for(k=0;k<32;k++) free(ID[k]);
      free(ID);
      free(Star);
}
/**********************************************************************/
void DrawStars(double LineOfSight[3],double BuckyPf[32][3],
               GLuint StarList[32])
{
      double PoL;
      long k;

      glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
      glEnable(GL_COLOR_MATERIAL);
      glColorMaterial(GL_FRONT,GL_EMISSION);
      glDepthFunc(GL_ALWAYS);
      glBlendFunc(GL_ONE,GL_ONE);

      for(k=0;k<32;k++) {
         PoL = VoV(BuckyPf[k],LineOfSight);
         if (PoL > 0.0) {
            glCallList(StarList[k]);
         }
      }

      glDepthFunc(GL_LEQUAL);
      glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
      glDisable(GL_COLOR_MATERIAL);
}
/**********************************************************************/
void Draw1FGL(double LineOfSight[3],double BuckyPf[32][3],
               GLuint FermiSourceList[32])
{
      double PoL;
      long k;

      glDisable(GL_LIGHTING);
      glDepthFunc(GL_ALWAYS);
      /*glBlendFunc(GL_ONE,GL_ONE);*/
      for(k=0;k<32;k++) {
         PoL = VoV(BuckyPf[k],LineOfSight);
         if (PoL > 0.0) {
            glCallList(FermiSourceList[k]);
         }
      }
      glDepthFunc(GL_LEQUAL);
      /*glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);*/
      glEnable(GL_LIGHTING);
}
/**********************************************************************/
void DrawEgret(double LineOfSight[3],double BuckyPf[32][3],
               GLuint EgretSourceList[32])
{
      double PoL;
      long k;

      glDisable(GL_LIGHTING);
      glDepthFunc(GL_ALWAYS);
      glBlendFunc(GL_ONE,GL_ONE);
      for(k=0;k<32;k++) {
         PoL = VoV(BuckyPf[k],LineOfSight);
         if (PoL > 0.0) {
            glCallList(EgretSourceList[k]);
         }
      }
      glDepthFunc(GL_LEQUAL);
      glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
      glEnable(GL_LIGHTING);
}
/**********************************************************************/
void DrawPulsars(double LineOfSight[3],double BuckyPf[32][3],
               GLuint PulsarList[32])
{
      double PoL;
      long k;

      glDisable(GL_LIGHTING);
      glDepthFunc(GL_ALWAYS);
      glBlendFunc(GL_ONE,GL_ONE);
      for(k=0;k<32;k++) {
         PoL = VoV(BuckyPf[k],LineOfSight);
         if (PoL > 0.0) {
            glCallList(PulsarList[k]);
         }
      }
      glDepthFunc(GL_LEQUAL);
      glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
      glEnable(GL_LIGHTING);
}
/**********************************************************************/
GLuint LoadMilkyWay(const char *PathName, const char *FileName, double CGH[3][3],
   double SkyDistance, double AlphaMask[4])
{
      long i,j;
      double lat,lng,lat1,lat2,r[3],s,t1,t2;
      GLuint MilkyWayTexTag;
      GLuint ListTag;
      long Nlng = 64;
      double Pi;

      MilkyWayTexTag = PpmToTexTag(PathName,FileName,
         3,GL_REPEAT);

      Pi = 4.0*atan(1.0);
      lat = Pi/8.0;

      ListTag = glGenLists(1);
      glNewList(ListTag,GL_COMPILE);
         glPushMatrix();
         RotateR2L(CGH);
         glEnable(GL_COLOR_MATERIAL);
         glColorMaterial(GL_FRONT,GL_EMISSION);
         glActiveTexture(GL_TEXTURE0);
         glEnable(GL_TEXTURE_2D);
         glBindTexture(GL_TEXTURE_2D,MilkyWayTexTag);
         glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
         glScalef(SkyDistance,SkyDistance,SkyDistance);
         for(j=0;j<3;j++) {
            t1 = 1.0-((double) j)/3.0;
            t2 = t1-1.0/3.0;
            lat1 = -lat+((double) j)*2.0/3.0*lat;
            lat2 = lat1 + lat*2.0/3.0;
            glBegin(GL_QUAD_STRIP);
               for(i=0;i<=Nlng;i++) {
                  s = ((double) i)/((double) Nlng);
                  lng = s*2.0*Pi-Pi;
                  r[0] = cos(lng)*cos(lat1);
                  r[1] = sin(lng)*cos(lat1);
                  r[2] = sin(lat1);
                  glColor4d(AlphaMask[j],AlphaMask[j],
                            AlphaMask[j],AlphaMask[j]);
                  glTexCoord2f(-s,t1);
                  glVertex3dv(r);
                  r[0] = cos(lng)*cos(lat2);
                  r[1] = sin(lng)*cos(lat2);
                  r[2] = sin(lat2);
                  glColor4d(AlphaMask[j+1],AlphaMask[j+1],
                     AlphaMask[j+1],AlphaMask[j+1]);
                  glTexCoord2f(-s,t2);
                  glVertex3dv(r);
               }
            glEnd();
         }
         glDisable(GL_TEXTURE_2D);
         glDisable(GL_COLOR_MATERIAL);
         glPopMatrix();
      glEndList();

      return(ListTag);
}
/**********************************************************************/
GLuint LoadSkyCube(const char *PathName, const char *FileName, double CGH[3][3],
   double SkyDistance)
{
      GLuint TexTag;
      GLuint ListTag;

      TexTag = PpmToCubeTag(PathName,FileName,
         3);

      ListTag = glGenLists(1);
      glNewList(ListTag,GL_COMPILE);
         glActiveTexture(GL_TEXTURE0);
         glPushMatrix();
         RotateR2L(CGH);
         glEnable(GL_COLOR_MATERIAL);
         glColorMaterial(GL_FRONT,GL_EMISSION);
         glEnable(GL_TEXTURE_CUBE_MAP);
         glBindTexture(GL_TEXTURE_CUBE_MAP,TexTag);
         glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
         glScalef(SkyDistance,SkyDistance,SkyDistance);
         glBegin(GL_QUADS);
            /* +X */
            glTexCoord3f( 1.0, 1.0,-1.0);
            glVertex3d  ( 1.0,-1.0,-1.0);
            glTexCoord3f( 1.0, 1.0, 1.0);
            glVertex3d  ( 1.0,-1.0, 1.0);
            glTexCoord3f( 1.0,-1.0, 1.0);
            glVertex3d  ( 1.0, 1.0, 1.0);
            glTexCoord3f( 1.0,-1.0,-1.0);
            glVertex3d  ( 1.0, 1.0,-1.0);
            /* +Y */
            glTexCoord3f(-1.0,-1.0,-1.0);
            glVertex3d  (-1.0, 1.0,-1.0);
            glTexCoord3f( 1.0,-1.0,-1.0);
            glVertex3d  ( 1.0, 1.0,-1.0);
            glTexCoord3f( 1.0,-1.0, 1.0);
            glVertex3d  ( 1.0, 1.0, 1.0);
            glTexCoord3f(-1.0,-1.0, 1.0);
            glVertex3d  (-1.0, 1.0, 1.0);
            /* +Z */
            glTexCoord3f(-1.0, 1.0, 1.0);
            glVertex3d  (-1.0,-1.0, 1.0);
            glTexCoord3f(-1.0,-1.0, 1.0);
            glVertex3d  (-1.0, 1.0, 1.0);
            glTexCoord3f( 1.0,-1.0, 1.0);
            glVertex3d  ( 1.0, 1.0, 1.0);
            glTexCoord3f( 1.0, 1.0, 1.0);
            glVertex3d  ( 1.0,-1.0, 1.0);
            /* -X */
            glTexCoord3f(-1.0, 1.0,-1.0);
            glVertex3d  (-1.0,-1.0,-1.0);
            glTexCoord3f(-1.0,-1.0,-1.0);
            glVertex3d  (-1.0, 1.0,-1.0);
            glTexCoord3f(-1.0,-1.0, 1.0);
            glVertex3d  (-1.0, 1.0, 1.0);
            glTexCoord3f(-1.0, 1.0, 1.0);
            glVertex3d  (-1.0,-1.0, 1.0);
            /* -Y */
            glTexCoord3f(-1.0, 1.0,-1.0);
            glVertex3d  (-1.0,-1.0,-1.0);
            glTexCoord3f(-1.0, 1.0, 1.0);
            glVertex3d  (-1.0,-1.0, 1.0);
            glTexCoord3f( 1.0, 1.0, 1.0);
            glVertex3d  ( 1.0,-1.0, 1.0);
            glTexCoord3f( 1.0, 1.0,-1.0);
            glVertex3d  ( 1.0,-1.0,-1.0);
            /* -Z */
            glTexCoord3f(-1.0, 1.0,-1.0);
            glVertex3d  (-1.0,-1.0,-1.0);
            glTexCoord3f( 1.0, 1.0,-1.0);
            glVertex3d  ( 1.0,-1.0,-1.0);
            glTexCoord3f( 1.0,-1.0,-1.0);
            glVertex3d  ( 1.0, 1.0,-1.0);
            glTexCoord3f(-1.0,-1.0,-1.0);
            glVertex3d  (-1.0, 1.0,-1.0);
         glEnd();

         glDisable(GL_TEXTURE_CUBE_MAP);
         glDisable(GL_COLOR_MATERIAL);
         glPopMatrix();
      glEndList();

      return(ListTag);
}
/**********************************************************************/
/* Egret Catalog of Gamma-ray Sources                                 */
void LoadEgretCatalog(const char *EgretFileName,double BuckyPf[32][3],
               long BuckyNeighbor[32][6], GLuint EgretSourceList[32],
               double SkyDistance)
{
#define Nsource 262
#define D2R (0.0174532925199433)

      struct GammaSourceType {
         double r[4];
         char Type;
         char Label[40];
         GLfloat Color[4];
      };
      struct GammaSourceType GammaSource[Nsource];
      double MaxPoR,PoR,c1,s1,c2,s2,RA,Dec,r[3];
      double qnh[4] = {-0.203123038887,  0.0,  0.0,  0.979153221449};
      long ClosestVtx;
      long ID[32][Nsource],N[32];
      long i,j,k;
      float Black[4] = {0.0,0.0,0.0,1.0};
      FILE *SourceFile;

      for(k=0;k<32;k++) N[k] = 0;

      SourceFile = FileOpen("./Model/",EgretFileName,"r");
      for(i=0;i<Nsource;i++) {
         fscanf(SourceFile,"%lf %lf %c %[^\n]\n",
            &RA,&Dec,&GammaSource[i].Type,
            GammaSource[i].Label);

         c1 = cos(RA*D2R);
         s1 = sin(RA*D2R);
         c2 = cos(Dec*D2R);
         s2 = sin(Dec*D2R);
         r[0] = c1*c2;
         r[1] = s1*c2;
         r[2] = s2;

         /* Transform from input frame (probably ECI) to H frame */
         QTxV(qnh,r,GammaSource[i].r);

         GammaSource[i].r[3] = 1.0/SkyDistance;

         if (GammaSource[i].Type == 'A') {
            GammaSource[i].Color[0] = 0.0;
            GammaSource[i].Color[1] = 0.6;
            GammaSource[i].Color[2] = 1.0;
         }
         else if (GammaSource[i].Type == 'a') {
            GammaSource[i].Color[0] = 0.0;
            GammaSource[i].Color[1] = 0.2;
            GammaSource[i].Color[2] = 1.0;
         }
         else if (GammaSource[i].Type == 'P') {
            GammaSource[i].Color[0] = 1.0;
            GammaSource[i].Color[1] = 1.0;
            GammaSource[i].Color[2] = 0.0;
         }
         else {
            GammaSource[i].Color[0] = 1.0;
            GammaSource[i].Color[1] = 0.4;
            GammaSource[i].Color[2] = 0.0;
         }
      }
      fclose(SourceFile);

      for(i=0;i<Nsource;i++) {
         MaxPoR = VoV(BuckyPf[0],GammaSource[i].r);
         ClosestVtx = 0;
         for(k=1;k<32;k++) {
            PoR = VoV(BuckyPf[k],GammaSource[i].r);
            if (PoR > MaxPoR) {
               MaxPoR = PoR;
               ClosestVtx = k;
            }
         }
         ID[ClosestVtx][N[ClosestVtx]] = i;
         N[ClosestVtx]++;
      }

      for(k=0;k<32;k++) {
         EgretSourceList[k] = glGenLists(1);
         glNewList(EgretSourceList[k],GL_COMPILE);
            glPointSize(4.0);
            glBegin(GL_POINTS);
            for(i=0;i<N[k];i++) {
               j=ID[k][i];
               glColor4fv(GammaSource[j].Color);
               glVertex4dv(GammaSource[j].r);
            }
            glEnd();
            for(i=0;i<N[k];i++) {
               j=ID[k][i];
               glColor4fv(GammaSource[j].Color);
               glRasterPos4dv(GammaSource[j].r);
               glBitmap(0,0,0,0,8,-5,0);
               DrawBitmapString(GLUT_BITMAP_8_BY_13,
                  GammaSource[j].Label);
            }
            glColor4fv(Black);
         glEndList();
      }
      glPointSize(2.0);
#undef Nsource
#undef D2R
}
/**********************************************************************/
/* Fermi Source Catalog 1FGL                                          */
void Load1FGL(const char *FileName,double BuckyPf[32][3],
               long BuckyNeighbor[32][6],GLuint FermiSourceList[32],
               double SkyDistance)
{
#define Nsource 1452
#define D2R (0.0174532925199433)

#define CLASS_unc 0
#define CLASS_bzb 1
#define CLASS_bzq 2
#define CLASS_PSR 3
#define CLASS_glc 4
#define CLASS_agu 5
#define CLASS_sbg 6
#define CLASS_gal 7
#define CLASS_spp 8
#define CLASS_HXB 9
#define CLASS_agn 10
#define CLASS_psr 11
#define CLASS_SNR 12
#define CLASS_BZQ 13
#define CLASS_pwn 14
#define CLASS_MQO 15

      GLubyte FglGlyph[16][32] = {
      {0xff, 0xff,   0xff, 0xff,   0xc0, 0x03,   0xc0, 0x03,
       0xc0, 0x03,   0xc0, 0x03,   0xc0, 0x03,   0xc0, 0x03,
       0xc0, 0x03,   0xc0, 0x03,   0xc0, 0x03,   0xc0, 0x03,
       0xc0, 0x03,   0xc0, 0x03,   0xff, 0xff,   0xff, 0xff}, /* unc: Square */

      {0xc0, 0x03,   0xe0, 0x07,   0x70, 0x0e,   0x38, 0x1c,
       0x1c, 0x38,   0x0e, 0x70,   0x07, 0xe0,   0x03, 0xc0,
       0x03, 0xc0,   0x07, 0xe0,   0x0e, 0x70,   0x1c, 0x38,
       0x38, 0x1c,   0x70, 0x0e,   0xe0, 0x07,   0xc0, 0x03}, /* bzb: X */

      {0xc0, 0x03,   0xe0, 0x07,   0x70, 0x0e,   0x38, 0x1c,
       0x1c, 0x38,   0x0e, 0x70,   0x07, 0xe0,   0x03, 0xc0,
       0x03, 0xc0,   0x07, 0xe0,   0x0e, 0x70,   0x1c, 0x38,
       0x38, 0x1c,   0x70, 0x0e,   0xe0, 0x07,   0xc0, 0x03}, /* bzq: X */

      {0x30, 0x0c,   0x3c, 0x3c,   0x16, 0x68,   0x13, 0xc8,
       0x19, 0x98,   0x08, 0x10,   0x18, 0x18,   0x30, 0x0c,
       0xe0, 0x07,   0xf8, 0x1f,   0x0c, 0x30,   0x06, 0x60,
       0x02, 0x40,   0x03, 0xc0,   0x01, 0x80,   0x01, 0x80}, /* PSR: Star */

      {0x00, 0x00,   0xff, 0xff,   0xff, 0xff,   0x60, 0x06,
       0x60, 0x06,   0x30, 0x0c,   0x30, 0x0c,   0x18, 0x18,
       0x0c, 0x30,   0x0c, 0x30,   0x06, 0x60,   0x06, 0x60,
       0x03, 0xc0,   0x01, 0x80,   0x01, 0x80,   0x00, 0x00}, /* glc: Triangle */

      {0xc0, 0x03,   0xe7, 0xe7,   0x7f, 0xfe,   0x38, 0x1c,
       0x3c, 0x3c,   0x6e, 0x76,   0x67, 0xe6,   0x63, 0xc6,
       0x63, 0xc6,   0x67, 0xe6,   0x6e, 0x76,   0x3c, 0x3c,
       0x38, 0x1c,   0x7f, 0xfe,   0xe7, 0xe7,   0xc0, 0x03}, /* agu: X-Circle */

      {0x01, 0x80,   0x01, 0x80,   0x31, 0x8c,   0x39, 0x9c,
       0x1d, 0xb8,   0x0f, 0xf0,   0x07, 0xe0,   0xff, 0xff,
       0xff, 0xff,   0x07, 0xe0,   0x0f, 0xf0,   0x1d, 0xb8,
       0x39, 0x9c,   0x31, 0x8c,   0x01, 0x80,   0x01, 0x80}, /* sbg: 8-spoke Star */

      {0x01, 0x80,   0x01, 0x80,   0x01, 0x80,   0x01, 0x80,
       0x01, 0x80,   0x01, 0x80,   0x01, 0x80,   0xff, 0xff,
       0xff, 0xff,   0x01, 0x80,   0x01, 0x80,   0x01, 0x80,
       0x01, 0x80,   0x01, 0x80,   0x01, 0x80,   0x01, 0x80}, /* gal: + */

      {0xff, 0xff,   0xff, 0xff,   0xc0, 0x03,   0xc3, 0xc3,
       0xc7, 0xe3,   0xcc, 0x33,   0xd8, 0x1b,   0xd8, 0x1b,
       0xd8, 0x1b,   0xd8, 0x1b,   0xcc, 0x33,   0xc7, 0xe3,
       0xc3, 0xc3,   0xc0, 0x03,   0xff, 0xff,   0xff, 0xff}, /* spp: Square-Circle */

      {0xff, 0xff,   0xff, 0xff,   0xf0, 0x0f,   0xf8, 0x1f,
       0xdc, 0x3b,   0xce, 0x73,   0xc7, 0xe3,   0xc3, 0xc3,
       0xc3, 0xc3,   0xc7, 0xe3,   0xce, 0x73,   0xdc, 0x3b,
       0xf8, 0x1f,   0xf0, 0x0f,   0xff, 0xff,   0xff, 0xff}, /* HXB: X-Square */

      {0xc0, 0x03,   0xe0, 0x07,   0x70, 0x0e,   0x39, 0x9c,
       0x1f, 0xf8,   0x0f, 0xf0,   0x0f, 0xf0,   0x1f, 0xf8,
       0x1f, 0xf8,   0x0f, 0xf0,   0x0f, 0xf0,   0x1f, 0xf8,
       0x39, 0x9c,   0x70, 0x0e,   0xe0, 0x07,   0xc0, 0x03}, /* agn: X-dot */

      {0x30, 0x0c,   0x3c, 0x3c,   0x16, 0x68,   0x13, 0xc8,
       0x19, 0x98,   0x08, 0x10,   0x18, 0x18,   0x30, 0x0c,
       0xe0, 0x07,   0xf8, 0x1f,   0x0c, 0x30,   0x06, 0x60,
       0x02, 0x40,   0x03, 0xc0,   0x01, 0x80,   0x01, 0x80}, /* psr: Star */

      {0x07, 0xe0,   0x1f, 0xf8,   0x38, 0x1c,   0x70, 0x0e,
       0x60, 0x06,   0xc0, 0x03,   0xc0, 0x03,   0xc0, 0x03,
       0xc0, 0x03,   0xc0, 0x03,   0xc0, 0x03,   0x60, 0x06,
       0x70, 0x0e,   0x38, 0x1c,   0x1f, 0xf8,   0x07, 0xe0}, /* SNR: Circle */

      {0x07, 0xe0,   0x1f, 0xf8,   0x38, 0x1c,   0x70, 0x0e,
       0x60, 0x06,   0xc0, 0x03,   0xc1, 0x83,   0xc3, 0xc3,
       0xc3, 0xc3,   0xc1, 0x83,   0xc0, 0x03,   0x60, 0x06,
       0x70, 0x0e,   0x38, 0x1c,   0x1f, 0xf8,   0x07, 0xe0}, /* BZQ: Circle-dot */

      {0x01, 0x80,   0x03, 0xc0,   0x03, 0xc0,   0x06, 0x60,
       0x0e, 0x70,   0x0c, 0x30,   0x19, 0x98,   0x3b, 0xdc,
       0x3b, 0xdc,   0x19, 0x98,   0x0c, 0x30,   0x0e, 0x70,
       0x06, 0x60,   0x03, 0xc0,   0x03, 0xc0,   0x01, 0x80}, /* pwn: Diamond-dot */

      {0xff, 0xff,   0xff, 0xff,   0xf0, 0x0f,   0xf8, 0x1f,
       0xdc, 0x3b,   0xce, 0x73,   0xc7, 0xe3,   0xc3, 0xc3,
       0xc3, 0xc3,   0xc7, 0xe3,   0xce, 0x73,   0xdc, 0x3b,
       0xf8, 0x1f,   0xf0, 0x0f,   0xff, 0xff,   0xff, 0xff}  /* MQO: X-Square */
      };

      FILE *infile;
      double RA,Dec;
      double SourceVec[Nsource][4];
      long Type[Nsource];
      char line[512];
      double RAhr,RAmin,RAsec,DECdeg,DECmin,DECsec,flux,fluxerr,r[3];
      char Class[3];
      double MaxPoR,PoR;
      long ClosestVtx;
      long N[32], ID[32][Nsource];
      long i,j,k;
      /*char s[20];*/
      GLfloat Color[3][4] = {{0.663,0.663,0.663,1.0}, /* Unclassified */
                             {0.118,0.565,1.0,1.0}, /* Associated */
                             {0.863,0.078,0.235,1.0}}; /* Identified */
      long ColorIdx[16] = {0,1,1,2,1,1,1,1,1,2,1,1,2,2,1,2};
      float Black[4] = {0.0,0.0,0.0,1.0};
      double qnh[4] = {-0.203123038887,  0.0,  0.0,  0.979153221449};

/*
      unc = No association :  Gray Square
      bzb = Associated with AGN blazar :  Blue X
      bzq = Associated with AGN blazar :  Blue X
      PSR = Identified with Pulsar :  Red Star
      glc = Associated with Globular Cluster :  Blue Triangle
      agu = Associated with Active Galaxy of Uncertain Type :  Blue X-Circle
      sbg = Associated with Starburst Galaxy :  Blue 8-spoke star
      gal = Associated with Normal Galaxy :  Blue +
      spp = Associated with SNR or PWN :  Blue Square-Circle
      HXB = Identified with Other X-ray Binary :  Red X-square
      agn = Associated with Non-blazar Active Galaxy :  Blue X-dot
      psr = Associated with Pulsar :  Blue Star
      SNR = Identified with Supernova Remnant :  Red Circle
      BZQ = Identified with FSRQ Blazar :  Red Circle-dot
      pwn = Associated with Pulsar Wind Nebula :  Blue Diamond-Dot
      MQO = Identified with Micro-Quasar Object :  Red X-square
*/

      for(k=0;k<32;k++) N[k] = 0;

      infile = FileOpen("./Model/",FileName,"rt");
      i=0;
      while(i<Nsource) {
         fgets(line,512,infile);
         if(sscanf(line,"|%lf %lf %lf|%lf %lf %lf|%lf|%lf|%3c |",
            &RAhr,&RAmin,&RAsec,&DECdeg,&DECmin,&DECsec,
            &flux,&fluxerr,Class) == 9) {
            RA = 15.0*(RAhr+RAmin/60.0+RAsec/3600.0)*D2R;
            Dec = (DECdeg > 0.0 ? 1.0 : -1.0)*(fabs(DECdeg)+DECmin/60.0+DECsec/3600.0)*D2R;
            if (!strncmp(Class,"   ",3)) Type[i] = CLASS_unc;
            else if (!strncmp(Class,"bzb",3)) Type[i] = CLASS_bzb;
            else if (!strncmp(Class,"bzq",3)) Type[i] = CLASS_bzq;
            else if (!strncmp(Class,"PSR",3)) Type[i] = CLASS_PSR;
            else if (!strncmp(Class,"glc",3)) Type[i] = CLASS_glc;
            else if (!strncmp(Class,"agu",3)) Type[i] = CLASS_agu;
            else if (!strncmp(Class,"sbg",3)) Type[i] = CLASS_sbg;
            else if (!strncmp(Class,"gal",3)) Type[i] = CLASS_gal;
            else if (!strncmp(Class,"spp",3)) Type[i] = CLASS_spp;
            else if (!strncmp(Class,"HXB",3)) Type[i] = CLASS_HXB;
            else if (!strncmp(Class,"agn",3)) Type[i] = CLASS_agn;
            else if (!strncmp(Class,"psr",3)) Type[i] = CLASS_psr;
            else if (!strncmp(Class,"SNR",3)) Type[i] = CLASS_SNR;
            else if (!strncmp(Class,"BZQ",3)) Type[i] = CLASS_BZQ;
            else if (!strncmp(Class,"pwn",3)) Type[i] = CLASS_pwn;
            else if (!strncmp(Class,"MQO",3)) Type[i] = CLASS_MQO;
            else printf("1FGL Class unknown: %s\n",Class);
            r[0] = cos(RA)*cos(Dec);
            r[1] = sin(RA)*cos(Dec);
            r[2] = sin(Dec);
            /* Transform from input frame (probably ECI) to H frame */
            QTxV(qnh,r,SourceVec[i]);
            SourceVec[i][3] = 1.0/SkyDistance;
            i++;
         }
      }
      fclose(infile);

      for(i=0;i<Nsource;i++) {
         MaxPoR = VoV(BuckyPf[0],SourceVec[i]);
         ClosestVtx = 0;
         for(k=1;k<32;k++) {
            PoR = VoV(BuckyPf[k],SourceVec[i]);
            if (PoR > MaxPoR) {
               MaxPoR = PoR;
               ClosestVtx = k;
            }
         }
         ID[ClosestVtx][N[ClosestVtx]] = i;
         N[ClosestVtx]++;
      }

      for(k=0;k<32;k++) {
         FermiSourceList[k] = glGenLists(1);
         glNewList(FermiSourceList[k],GL_COMPILE);
            for(i=0;i<N[k];i++) {
               j = ID[k][i];
               glColor4fv(Color[ColorIdx[Type[j]]]);
               /*glBegin(GL_POINTS);
               **   glVertex4dv(SourceVec[j]);
               **glEnd();
               */
               glRasterPos4dv(SourceVec[j]);
               glBitmap(0,0,0,0,-8,-8,0);
               glBitmap(16,16,0.0,0.0,0.0,0.0,FglGlyph[Type[j]]);
               /*glBitmap(0,0,0,0,18,-5,0);
               **sprintf(s,"%ld",j);
               **DrawBitmapString(GLUT_BITMAP_8_BY_13,s);
               */
            }
            glColor4fv(Black);
         glEndList();
      }

#undef CLASS_unc
#undef CLASS_bzb
#undef CLASS_bzq
#undef CLASS_PSR
#undef CLASS_glc
#undef CLASS_agu
#undef CLASS_sbg
#undef CLASS_gal
#undef CLASS_spp
#undef CLASS_HXB
#undef CLASS_agn
#undef CLASS_psr
#undef CLASS_SNR
#undef CLASS_BZQ
#undef CLASS_pwn
#undef CLASS_MQO

#undef Nsource
#undef D2R
}
/**********************************************************************/
void LoadPulsars(const char *FileName,double BuckyPf[32][3],
               long BuckyNeighbor[32][6],GLuint PulsarList[32],
               double SkyDistance)
{
#define Npul 250
#define D2R (0.0174532925199433)

      GLubyte PulsarGlyph[32] =
      {0x01, 0x80,   0x01, 0x80,   0x31, 0x8c,   0x39, 0x9c,
       0x1d, 0xb8,   0x0f, 0xf0,   0x07, 0xe0,   0xff, 0xff,
       0xff, 0xff,   0x07, 0xe0,   0x0f, 0xf0,   0x1d, 0xb8,
       0x39, 0x9c,   0x31, 0x8c,   0x01, 0x80,   0x01, 0x80}; /* 8-spoke Star */

      FILE *infile;
      double RA,Dec;
      double SourceVec[Npul][4];
      double r[3];
      double MaxPoR,PoR;
      long ClosestVtx;
      long N[32], ID[32][Npul];
      long i,j,k;
      GLfloat Color[4] = {0.635,0.178,0.635,0.5};
      float Black[4] = {0.0,0.0,0.0,1.0};
      double qnh[4] = {-0.203123038887,  0.0,  0.0,  0.979153221449};
      char Identifier[Npul][40];
      double EclLng,EclLat;
      char line[512];

      for(k=0;k<32;k++) N[k] = 0;

      infile = FileOpen("./Model/",FileName,"rt");
      i = 0;
      while(!feof(infile)) {
         fgets(line,512,infile);
         if(sscanf(line,"%s %lf %lf %lf %lf",
            Identifier[i],&RA,&Dec,&EclLng,&EclLat)==5) {
            RA *= D2R;
            Dec *= D2R;
            r[0] = cos(RA)*cos(Dec);
            r[1] = sin(RA)*cos(Dec);
            r[2] = sin(Dec);
            /* Transform from input frame (probably ECI) to H frame */
            QTxV(qnh,r,SourceVec[i]);
            SourceVec[i][3] = 1.0/SkyDistance;
            i++;
         }
      }
      fclose(infile);

      for(i=0;i<Npul;i++) {
         MaxPoR = VoV(BuckyPf[0],SourceVec[i]);
         ClosestVtx = 0;
         for(k=1;k<32;k++) {
            PoR = VoV(BuckyPf[k],SourceVec[i]);
            if (PoR > MaxPoR) {
               MaxPoR = PoR;
               ClosestVtx = k;
            }
         }
         ID[ClosestVtx][N[ClosestVtx]] = i;
         N[ClosestVtx]++;
      }

      for(k=0;k<32;k++) {
         PulsarList[k] = glGenLists(1);
         glNewList(PulsarList[k],GL_COMPILE);
            for(i=0;i<N[k];i++) {
               j = ID[k][i];
               glColor4fv(Color);
               /*glBegin(GL_POINTS);
               **   glVertex4dv(SourceVec[j]);
               **glEnd();
               */
               glRasterPos4dv(SourceVec[j]);
               glBitmap(0,0,0,0,-8,-8,0);
               glBitmap(16,16,0.0,0.0,0.0,0.0,PulsarGlyph);
               glBitmap(0,0,0,0,18,-5,0);
               DrawBitmapString(GLUT_BITMAP_8_BY_13,Identifier[j]);
            }
            glColor4fv(Black);
         glEndList();
      }

#undef Npul
#undef D2R
}
/**********************************************************************/
void DrawUnitCubeSphere(long Ndiv)
{

      GLfloat r[3];
      GLfloat x,y,z,x1,x2,y1,y2,z1,z2;
      long i,j;

/* .. +X Face */
      x = 1.0;
      for(i=0;i<Ndiv;i++) {
         y1 = -1.0+2.0*((double) i)/((double) Ndiv);
         y2 = -1.0+2.0*((double) i+1)/((double) Ndiv);
         glBegin(GL_QUAD_STRIP);
            for(j=0;j<=Ndiv;j++) {
               z = -1.0+2.0*((double) j)/((double) Ndiv);
               r[0] = x/sqrt(x*x+y1*y1+z*z);
               r[1] = y1/sqrt(x*x+y1*y1+z*z);
               r[2] = z/sqrt(x*x+y1*y1+z*z);
               glNormal3fv(r);
               glVertex3fv(r);

               r[0] = x/sqrt(x*x+y2*y2+z*z);
               r[1] = y2/sqrt(x*x+y2*y2+z*z);
               r[2] = z/sqrt(x*x+y2*y2+z*z);
               glNormal3fv(r);
               glVertex3fv(r);

            }
         glEnd();
      }

/* .. -X Face */
      x = -1.0;
      for(i=0;i<Ndiv;i++) {
         y1 = 1.0-2.0*((double) i)/((double) Ndiv);
         y2 = 1.0-2.0*((double) i+1)/((double) Ndiv);
         glBegin(GL_QUAD_STRIP);
            for(j=0;j<=Ndiv;j++) {
               z = -1.0+2.0*((double) j)/((double) Ndiv);
               r[0] = x/sqrt(x*x+y1*y1+z*z);
               r[1] = y1/sqrt(x*x+y1*y1+z*z);
               r[2] = z/sqrt(x*x+y1*y1+z*z);
               glNormal3fv(r);
               glVertex3fv(r);

               r[0] = x/sqrt(x*x+y2*y2+z*z);
               r[1] = y2/sqrt(x*x+y2*y2+z*z);
               r[2] = z/sqrt(x*x+y2*y2+z*z);
               glNormal3fv(r);
               glVertex3fv(r);

            }
         glEnd();
      }

/* .. +Y Face */
      y = 1.0;
      for(i=0;i<Ndiv;i++) {
         z1 = -1.0+2.0*((double) i)/((double) Ndiv);
         z2 = -1.0+2.0*((double) i+1)/((double) Ndiv);
         glBegin(GL_QUAD_STRIP);
            for(j=0;j<=Ndiv;j++) {
               x = -1.0+2.0*((double) j)/((double) Ndiv);
               r[0] = x/sqrt(x*x+y*y+z1*z1);
               r[1] = y/sqrt(x*x+y*y+z1*z1);
               r[2] = z1/sqrt(x*x+y*y+z1*z1);
               glNormal3fv(r);
               glVertex3fv(r);

               r[0] = x/sqrt(x*x+y*y+z2*z2);
               r[1] = y/sqrt(x*x+y*y+z2*z2);
               r[2] = z2/sqrt(x*x+y*y+z2*z2);
               glNormal3fv(r);
               glVertex3fv(r);

            }
         glEnd();
      }

/* .. -Y Face */
      y = -1.0;
      for(i=0;i<Ndiv;i++) {
         z1 = 1.0-2.0*((double) i)/((double) Ndiv);
         z2 = 1.0-2.0*((double) i+1)/((double) Ndiv);
         glBegin(GL_QUAD_STRIP);
            for(j=0;j<=Ndiv;j++) {
               x = -1.0+2.0*((double) j)/((double) Ndiv);
               r[0] = x/sqrt(x*x+y*y+z1*z1);
               r[1] = y/sqrt(x*x+y*y+z1*z1);
               r[2] = z1/sqrt(x*x+y*y+z1*z1);
               glNormal3fv(r);
               glVertex3fv(r);

               r[0] = x/sqrt(x*x+y*y+z2*z2);
               r[1] = y/sqrt(x*x+y*y+z2*z2);
               r[2] = z2/sqrt(x*x+y*y+z2*z2);
               glNormal3fv(r);
               glVertex3fv(r);

            }
         glEnd();
      }

/* .. +Z Face */
      z = 1.0;
      for(i=0;i<Ndiv;i++) {
         x1 = -1.0+2.0*((double) i)/((double) Ndiv);
         x2 = -1.0+2.0*((double) i+1)/((double) Ndiv);
         glBegin(GL_QUAD_STRIP);
            for(j=0;j<=Ndiv;j++) {
               y = -1.0+2.0*((double) j)/((double) Ndiv);
               r[0] = x1/sqrt(x1*x1+y*y+z*z);
               r[1] = y/sqrt(x1*x1+y*y+z*z);
               r[2] = z/sqrt(x1*x1+y*y+z*z);
               glNormal3fv(r);
               glVertex3fv(r);

               r[0] = x2/sqrt(x2*x2+y*y+z*z);
               r[1] = y/sqrt(x2*x2+y*y+z*z);
               r[2] = z/sqrt(x2*x2+y*y+z*z);
               glNormal3fv(r);
               glVertex3fv(r);

            }
         glEnd();
      }

/* .. -Z Face */
      z = -1.0;
      for(i=0;i<Ndiv;i++) {
         x1 = 1.0-2.0*((double) i)/((double) Ndiv);
         x2 = 1.0-2.0*((double) i+1)/((double) Ndiv);
         glBegin(GL_QUAD_STRIP);
            for(j=0;j<=Ndiv;j++) {
               y = -1.0+2.0*((double) j)/((double) Ndiv);
               r[0] = x1/sqrt(x1*x1+y*y+z*z);
               r[1] = y/sqrt(x1*x1+y*y+z*z);
               r[2] = z/sqrt(x1*x1+y*y+z*z);
               glNormal3fv(r);
               glVertex3fv(r);

               r[0] = x2/sqrt(x2*x2+y*y+z*z);
               r[1] = y/sqrt(x2*x2+y*y+z*z);
               r[2] = z/sqrt(x2*x2+y*y+z*z);
               glNormal3fv(r);
               glVertex3fv(r);

            }
         glEnd();
      }
}
/**********************************************************************/
/* Draw a unit sphere, from the inside                                */
void DrawSkySphere(long Ndiv)
{

      GLfloat r[3];
      GLfloat x,y,z,x1,x2,y1,y2,z1,z2;
      long i,j;

/* .. +X Face */
      x = 1.0;
      for(i=0;i<Ndiv;i++) {
         y1 = -1.0+2.0*((double) i)/((double) Ndiv);
         y2 = -1.0+2.0*((double) i+1)/((double) Ndiv);
         glBegin(GL_QUAD_STRIP);
            for(j=0;j<=Ndiv;j++) {
               z = -1.0+2.0*((double) j)/((double) Ndiv);
               r[0] = x/sqrt(x*x+y2*y2+z*z);
               r[1] = y2/sqrt(x*x+y2*y2+z*z);
               r[2] = z/sqrt(x*x+y2*y2+z*z);
               glNormal3fv(r);
               glVertex3fv(r);

               r[0] = x/sqrt(x*x+y1*y1+z*z);
               r[1] = y1/sqrt(x*x+y1*y1+z*z);
               r[2] = z/sqrt(x*x+y1*y1+z*z);
               glNormal3fv(r);
               glVertex3fv(r);

            }
         glEnd();
      }

/* .. -X Face */
      x = -1.0;
      for(i=0;i<Ndiv;i++) {
         y1 = 1.0-2.0*((double) i)/((double) Ndiv);
         y2 = 1.0-2.0*((double) i+1)/((double) Ndiv);
         glBegin(GL_QUAD_STRIP);
            for(j=0;j<=Ndiv;j++) {
               z = -1.0+2.0*((double) j)/((double) Ndiv);
               r[0] = x/sqrt(x*x+y2*y2+z*z);
               r[1] = y2/sqrt(x*x+y2*y2+z*z);
               r[2] = z/sqrt(x*x+y2*y2+z*z);
               glNormal3fv(r);
               glVertex3fv(r);

               r[0] = x/sqrt(x*x+y1*y1+z*z);
               r[1] = y1/sqrt(x*x+y1*y1+z*z);
               r[2] = z/sqrt(x*x+y1*y1+z*z);
               glNormal3fv(r);
               glVertex3fv(r);

            }
         glEnd();
      }

/* .. +Y Face */
      y = 1.0;
      for(i=0;i<Ndiv;i++) {
         z1 = -1.0+2.0*((double) i)/((double) Ndiv);
         z2 = -1.0+2.0*((double) i+1)/((double) Ndiv);
         glBegin(GL_QUAD_STRIP);
            for(j=0;j<=Ndiv;j++) {
               x = -1.0+2.0*((double) j)/((double) Ndiv);
               r[0] = x/sqrt(x*x+y*y+z2*z2);
               r[1] = y/sqrt(x*x+y*y+z2*z2);
               r[2] = z2/sqrt(x*x+y*y+z2*z2);
               glNormal3fv(r);
               glVertex3fv(r);

               r[0] = x/sqrt(x*x+y*y+z1*z1);
               r[1] = y/sqrt(x*x+y*y+z1*z1);
               r[2] = z1/sqrt(x*x+y*y+z1*z1);
               glNormal3fv(r);
               glVertex3fv(r);

            }
         glEnd();
      }

/* .. -Y Face */
      y = -1.0;
      for(i=0;i<Ndiv;i++) {
         z1 = 1.0-2.0*((double) i)/((double) Ndiv);
         z2 = 1.0-2.0*((double) i+1)/((double) Ndiv);
         glBegin(GL_QUAD_STRIP);
            for(j=0;j<=Ndiv;j++) {
               x = -1.0+2.0*((double) j)/((double) Ndiv);
               r[0] = x/sqrt(x*x+y*y+z2*z2);
               r[1] = y/sqrt(x*x+y*y+z2*z2);
               r[2] = z2/sqrt(x*x+y*y+z2*z2);
               glNormal3fv(r);
               glVertex3fv(r);

               r[0] = x/sqrt(x*x+y*y+z1*z1);
               r[1] = y/sqrt(x*x+y*y+z1*z1);
               r[2] = z1/sqrt(x*x+y*y+z1*z1);
               glNormal3fv(r);
               glVertex3fv(r);

            }
         glEnd();
      }

/* .. +Z Face */
      z = 1.0;
      for(i=0;i<Ndiv;i++) {
         x1 = -1.0+2.0*((double) i)/((double) Ndiv);
         x2 = -1.0+2.0*((double) i+1)/((double) Ndiv);
         glBegin(GL_QUAD_STRIP);
            for(j=0;j<=Ndiv;j++) {
               y = -1.0+2.0*((double) j)/((double) Ndiv);
               r[0] = x2/sqrt(x2*x2+y*y+z*z);
               r[1] = y/sqrt(x2*x2+y*y+z*z);
               r[2] = z/sqrt(x2*x2+y*y+z*z);
               glNormal3fv(r);
               glVertex3fv(r);

               r[0] = x1/sqrt(x1*x1+y*y+z*z);
               r[1] = y/sqrt(x1*x1+y*y+z*z);
               r[2] = z/sqrt(x1*x1+y*y+z*z);
               glNormal3fv(r);
               glVertex3fv(r);

            }
         glEnd();
      }

/* .. -Z Face */
      z = -1.0;
      for(i=0;i<Ndiv;i++) {
         x1 = 1.0-2.0*((double) i)/((double) Ndiv);
         x2 = 1.0-2.0*((double) i+1)/((double) Ndiv);
         glBegin(GL_QUAD_STRIP);
            for(j=0;j<=Ndiv;j++) {
               y = -1.0+2.0*((double) j)/((double) Ndiv);
               r[0] = x2/sqrt(x2*x2+y*y+z*z);
               r[1] = y/sqrt(x2*x2+y*y+z*z);
               r[2] = z/sqrt(x2*x2+y*y+z*z);
               glNormal3fv(r);
               glVertex3fv(r);

               r[0] = x1/sqrt(x1*x1+y*y+z*z);
               r[1] = y/sqrt(x1*x1+y*y+z*z);
               r[2] = z/sqrt(x1*x1+y*y+z*z);
               glNormal3fv(r);
               glVertex3fv(r);

            }
         glEnd();
      }
}
/**********************************************************************/
void DrawUnitMercatorSphere(GLuint Nlat, GLuint Nlng)
{

      GLfloat Pi = 4.0*atan(1.0);
      GLfloat s1,s2,t,lng1,lng2,lat,r[3];
      GLuint i,j;

      for(i=0;i<Nlng;i++) {
         s1 = ((float) i)/((float) Nlng);
         s2 = ((float) i+1)/((float) Nlng);
         lng1 = s1*2.0*Pi-Pi;
         lng2 = s2*2.0*Pi-Pi;

         glBegin(GL_QUAD_STRIP);
            for(j=0;j<=Nlat;j++) {
               t = ((float) j)/((float) Nlat);
               lat = 0.5*Pi - t*Pi;
               r[0] = cos(lng2)*cos(lat);
               r[1] = sin(lng2)*cos(lat);
               r[2] = sin(lat);
               glNormal3fv(r);
               glTexCoord2f(s2,t);
               glVertex3fv(r);

               r[0] = cos(lng1)*cos(lat);
               r[1] = sin(lng1)*cos(lat);
               r[2] = sin(lat);
               glNormal3fv(r);
               glTexCoord2f(s1,t);
               glVertex3fv(r);
            }
         glEnd();
      }
}
/*********************************************************************/
void DrawBullseye(GLfloat Color[4],double p[4])
{
      GLfloat Black[4] = {0.0,0.0,0.0,1.0};

      glMaterialfv(GL_FRONT,GL_EMISSION,Color);
      glPointSize(12.0);
      glBegin(GL_POINTS);
         glVertex4dv(p);
      glEnd();
      glMaterialfv(GL_FRONT,GL_EMISSION,Black);
      glPointSize(8.0);
      glBegin(GL_POINTS);
         glVertex4dv(p);
      glEnd();
      glMaterialfv(GL_FRONT,GL_EMISSION,Color);
      glPointSize(4.0);
      glBegin(GL_POINTS);
         glVertex4dv(p);
      glEnd();
      glPointSize(1.0);

      glMaterialfv(GL_FRONT,GL_EMISSION,Black);
}
/*********************************************************************/
void DrawVector(double v[3], const char Label[8], const char Units[8],
                GLfloat Color[4],
                double VisScale, double MagScale, long UnitVec)
{
      GLubyte GlyphHat[4]={0x82,0x44,0x28,0x10};
      GLubyte GlyphVec[3]={0xfe,0x0c,0x10};
      double u[3],mag;
      char s[40];

      glDisable(GL_LIGHTING);
      glColor4fv(Color);

      if (UnitVec) {
         glBegin(GL_LINES);
            glVertex3d(0.0,0.0,0.0);
            glVertex3d(VisScale*v[0],VisScale*v[1],VisScale*v[2]);
         glEnd();
         DrawArrowhead(v,VisScale);

         glRasterPos3f(VisScale*v[0],VisScale*v[1],VisScale*v[2]);
         glBitmap(0,0,0,0,4,-5,0);
         DrawString8x11(Label);
         glBitmap(7,4,8.0*strlen(Label),-11.0,0.0,0.0,GlyphHat);
      }
      else {
         mag = CopyUnitV(v,u);
         glBegin(GL_LINES);
            glVertex3d(0.0,0.0,0.0);
            glVertex3d(VisScale*u[0],VisScale*u[1],VisScale*u[2]);
         glEnd();
         DrawArrowhead(u,VisScale);
         glRasterPos3f(VisScale*u[0],VisScale*u[1],VisScale*u[2]);
         glBitmap(0,0,0,0,4,-5,0);
         sprintf(s,"%s (%5.2lf %s)",Label,MagScale*mag,Units);
         DrawString8x11(s);
         glRasterPos3f(VisScale*u[0],VisScale*u[1],VisScale*u[2]);
         glBitmap(0,0,0,0,4,-5,0);
         glBitmap(7,3,0,-11.0,0.0,0.0,GlyphVec);
      }
      glColor4f(0.0,0.0,0.0,1.0);
      glEnable(GL_LIGHTING);
}
/*********************************************************************/
void DrawAxisLabels(long Iglyph,GLfloat Color[4],
                    GLfloat Xc, GLfloat Xmax,GLfloat Yc,GLfloat Ymax,
                    GLfloat Zc,GLfloat Zmax)
{
      GLfloat Black[4] = {0.0,0.0,0.0,1.0};
      GLubyte GlyphHat[4]={0x82,0x44,0x28,0x10};
      GLubyte GlyphSub1[7] = {0xe0,0x40,0x40,0x40,0x40,0xc0,0x40};
      GLubyte GlyphSub2[7] = {0xf0,0x80,0x40,0x20,0x10,0x90,0x60};
      GLubyte GlyphSub3[7] = {0x60,0x90,0x10,0x60,0x10,0x90,0x60};

      GLubyte Glyph[7][8]={{0x0,0x82,0x82,0x82,0x82,0xc4,0x38,0x0},
                           {0x0,0x7c,0x82,0x02,0x7c,0x80,0x82,0x7c},
                           {0xbc,0xc2,0x82,0x82,0xc4,0xb8,0x80,0x80},
                           {0x82,0x82,0x82,0xc4,0xb8,0x80,0x80,0x80},
                           {0x0,0x18,0x24,0x42,0x42,0x24,0x18,0x0},
                           {0x20,0x20,0x20,0x70,0x20,0x24,0x24,0x18},
                           {0x18,0x10,0x10,0x10,0x10,0x10,0x10,0x10}};

      glDisable(GL_LIGHTING);
      glColor4fv(Color);

      glLineWidth(2.0);

      glBegin(GL_LINES);
         glVertex3f(Xc,Yc,Zc);
         glVertex3f(Xmax,Yc,Zc);
         glVertex3f(Xc,Yc,Zc);
         glVertex3f(Xc,Ymax,Zc);
         glVertex3f(Xc,Yc,Zc);
         glVertex3f(Xc,Yc,Zmax);
      glEnd();

      glRasterPos3f(Xmax,Yc,Zc);
      glBitmap(0,0,0,0,4,4,0);
      glBitmap(7,8,0.0,0.0,0.0,9.0,Glyph[Iglyph]);
      glBitmap(7,4,0.0,0.0,9.0,-12.0,GlyphHat);
      glBitmap(4,7,0.0,0.0,0.0,0.0,GlyphSub1);

      glRasterPos3f(Xc,Ymax,Zc);
      glBitmap(0,0,0,0,4,4,0);
      glBitmap(7,8,0.0,0.0,0.0,9.0,Glyph[Iglyph]);
      glBitmap(7,4,0.0,0.0,9.0,-12.0,GlyphHat);
      glBitmap(4,7,0.0,0.0,0.0,0.0,GlyphSub2);

      glRasterPos3f(Xc,Yc,Zmax);
      glBitmap(0,0,0,0,4,4,0);
      glBitmap(7,8,0.0,0.0,0.0,9.0,Glyph[Iglyph]);
      glBitmap(7,4,0.0,0.0,9.0,-12.0,GlyphHat);
      glBitmap(4,7,0.0,0.0,0.0,0.0,GlyphSub3);

      glLineWidth(1.0);

      glColor4fv(Black);
      glEnable(GL_LIGHTING);
}
/*********************************************************************/
void DrawBodyLabel(long Ib,GLfloat Color[4],double p[3])
{
      GLfloat Black[4] = {0.0,0.0,0.0,1.0};
      char s[40];

      glMaterialfv(GL_FRONT,GL_EMISSION,Color);

      glRasterPos3dv(p);
      glBitmap(0,0,0,0,4,4,0);
      sprintf(s,"B[%ld]",Ib);
      DrawString8x11(s);
      glMaterialfv(GL_FRONT,GL_EMISSION,Black);
}
/*********************************************************************/
void DrawRollPitchYaw(long xc, long yc, long PixScale,
   double AngScale, double RateScale,
   double Roll, double Pitch, double Yaw,
   double RollRate, double PitchRate, double YawRate,
   double RollCmd, double PitchCmd, double YawCmd,
   GLfloat GaugeColor[4], GLfloat BarColor[4])
{
#define D2R (0.0174532925199433)


      long xmin,ymin,xmax,ymax;
      long Rmin,Rmax,x,y;
      double Ang,a;
      double Scale;
      long i;
      char s[40];

      Scale = ((double) PixScale)/AngScale;

      /* Pitch Gauge */
      xmin = xc + PixScale + 20;
      xmax = xmin + 12;
      ymin = yc - PixScale;
      ymax = yc + PixScale;
      glColor4fv(GaugeColor);
      glBegin(GL_LINE_LOOP);
         glVertex2i(xmin,ymin);
         glVertex2i(xmax,ymin);
         glVertex2i(xmax,ymax);
         glVertex2i(xmin,ymax);
      glEnd();
      glBegin(GL_LINES);
         glVertex2i(xmin,yc);
         glVertex2i(xmax,yc);
      glEnd();
      sprintf(s,"%4.1f",Pitch);
      x = xmin - 8*strlen(s) - 8;
      glRasterPos2i(x,yc+4);
      DrawString8x11(s);
      for(i=-9;i<10;i++) {
         y = yc - (long) (Scale*(10*i-Pitch));
         if (y >= ymin && y <= ymax) {
            glBegin(GL_LINES);
               glVertex2i(xmax,y);
               glVertex2i(xmax+8,y);
            glEnd();
            sprintf(s,"%3li",10*i);
            glRasterPos2i(xmax+12,y+4);
            DrawString8x11(s);
         }
      }
      y = yc - (long) (((double) PixScale)/RateScale*PitchRate);
      glBegin(GL_TRIANGLES);
         glVertex2i(xmax,y);
         glVertex2i(xmax+8,y+3);
         glVertex2i(xmax+8,y-3);
      glEnd();

      glColor4fv(BarColor);
      y = yc - (long) (Scale*(PitchCmd - Pitch));
      if (y < ymin) y = ymin;
      if (y > ymax) y = ymax;
      glBegin(GL_QUADS);
         if (y < yc) {
            glVertex2i(xmax-2,y);
            glVertex2i(xmin+2,y);
            glVertex2i(xmin+2,yc);
            glVertex2i(xmax-2,yc);
         }
         else {
            glVertex2i(xmin+2,y);
            glVertex2i(xmax-2,y);
            glVertex2i(xmax-2,yc);
            glVertex2i(xmin+2,yc);
         }
      glEnd();

      /* Yaw Gauge */
      xmin = xc - PixScale;
      xmax = xc + PixScale;
      ymin = yc + PixScale + 20;
      ymax = ymin+12;
      glColor4fv(GaugeColor);
      glBegin(GL_LINE_LOOP);
         glVertex2i(xmin,ymin);
         glVertex2i(xmax,ymin);
         glVertex2i(xmax,ymax);
         glVertex2i(xmin,ymax);
      glEnd();
      glBegin(GL_LINES);
         glVertex2i(xc,ymin);
         glVertex2i(xc,ymax);
      glEnd();
      sprintf(s,"%5.1f",Yaw);
      y = ymin - 6;
      glRasterPos2i(xc-4*strlen(s),y);
      DrawString8x11(s);
      for(i=0;i<36;i++) {
         Ang = 10.0*((double) i)-Yaw;
         while (Ang > 180.0) Ang -= 360.0;
         while (Ang < -180.0) Ang += 360.0;
         x = xc + (long) (Scale*Ang);
         if (x >= xmin && x <= xmax) {
            glBegin(GL_LINES);
               glVertex2i(x,ymax);
               glVertex2i(x,ymax+8);
            glEnd();
            sprintf(s,"%02li",i);
            glRasterPos2i(x-8,ymax+20);
            DrawString8x11(s);
         }
      }
      x = xc + (long) (((double) PixScale)/RateScale*YawRate);
      glBegin(GL_TRIANGLES);
         glVertex2i(x,ymax);
         glVertex2i(x-3,ymax+8);
         glVertex2i(x+3,ymax+8);
      glEnd();

      glColor4fv(BarColor);
      x = xc + (long) (Scale*(YawCmd - Yaw));
      if (x < xmin) x = xmin;
      if (x > xmax) x = xmax;
      glBegin(GL_QUADS);
         if (x < xc) {
            glVertex2i(x,ymin+2);
            glVertex2i(x,ymax-2);
            glVertex2i(xc,ymax-2);
            glVertex2i(xc,ymin+2);
         }
         else {
            glVertex2i(x,ymax-2);
            glVertex2i(x,ymin+2);
            glVertex2i(xc,ymin+2);
            glVertex2i(xc,ymax-2);
         }
      glEnd();

      /* Roll Gauge */
      Rmin = PixScale + 40;
      Rmax = Rmin+12;
      glColor4fv(GaugeColor);
      glBegin(GL_LINE_LOOP);
         for(i=0;i<21;i++) {
            Ang = -30.0 + ((double) i)/10.0*30.0;
            x = xc + (long) (Rmin*sin(Ang*D2R));
            y = yc - (long) (Rmin*cos(Ang*D2R));
            glVertex2i(x,y);
         }
         for(i=20;i>=0;i--) {
            Ang = -30.0 + ((double) i)/10.0*30.0;
            x = xc + (long) (Rmax*sin(Ang*D2R));
            y = yc - (long) (Rmax*cos(Ang*D2R));
            glVertex2i(x,y);
         }
      glEnd();
      glBegin(GL_LINES);
         glVertex2i(xc,yc-Rmin);
         glVertex2i(xc,yc-Rmax);
      glEnd();
      sprintf(s,"%5.1f",Roll);
      x = xc - 4*strlen(s) - 4;
      y = yc - Rmin + 14;
      glRasterPos2i(x,y);
      DrawString8x11(s);
      for(i=0;i<36;i++) {
         Ang = 10.0*((double) i)-Roll;
         while (Ang > 180.0) Ang -= 360.0;
         while (Ang < -180.0) Ang += 360.0;
         if (Ang >= -AngScale && Ang <= AngScale) {
            a = Ang*30.0/AngScale;
            glBegin(GL_LINES);
               x = xc + (long) (Rmax*sin(a*D2R));
               y = yc - (long) (Rmax*cos(a*D2R));
               glVertex2i(x,y);
               x = xc + (long) ((Rmax+8)*sin(a*D2R));
               y = yc - (long) ((Rmax+8)*cos(a*D2R));
               glVertex2i(x,y);
            glEnd();
            sprintf(s,"%02li",i);
            glRasterPos2i(x-8,y-4);
            DrawString8x11(s);
         }
      }
      Ang = 30.0/RateScale*RollRate;
      glBegin(GL_TRIANGLES);
         x = xc + (long) (Rmax*sin(Ang*D2R));
         y = yc - (long) (Rmax*cos(Ang*D2R));
         glVertex2i(x,y);
         x = xc + (long) ((Rmax+8)*sin((Ang+1.0)*D2R));
         y = yc - (long) ((Rmax+8)*cos((Ang+1.0)*D2R));
         glVertex2i(x,y);
         x = xc + (long) ((Rmax+8)*sin((Ang-1.0)*D2R));
         y = yc - (long) ((Rmax+8)*cos((Ang-1.0)*D2R));
         glVertex2i(x,y);
      glEnd();

      glColor4fv(BarColor);
      Ang = RollCmd - Roll;
      Scale = 30.0/AngScale;
      if (Ang < -AngScale) Ang = -AngScale;
      if (Ang > AngScale) Ang = AngScale;
      glBegin(GL_QUAD_STRIP);
         if (Ang < 0.0) {
            for(i=0;i<=10;i++) {
               a = 0.1*Scale*Ang*((double) i);
               x = xc + (long) ((Rmin+2)*sin(a*D2R));
               y = yc - (long) ((Rmin+2)*cos(a*D2R));
               glVertex2i(x,y);
               x = xc + (long) ((Rmax-2)*sin(a*D2R));
               y = yc - (long) ((Rmax-2)*cos(a*D2R));
               glVertex2i(x,y);
            }
         }
         else {
            for(i=0;i<=10;i++) {
               a = 0.1*Scale*Ang*((double) i);
               x = xc + (long) ((Rmax-2)*sin(a*D2R));
               y = yc - (long) ((Rmax-2)*cos(a*D2R));
               glVertex2i(x,y);
               x = xc + (long) ((Rmin+2)*sin(a*D2R));
               y = yc - (long) ((Rmin+2)*cos(a*D2R));
               glVertex2i(x,y);
            }
         }
      glEnd();
#undef D2R
}
/*********************************************************************/
/* Draw a small circle on a Mercator projection in active window.    */
void DrawSmallCircle(double lngc, double latc, double rad)
{
      double TwoPi = 6.28318530717959;
      double R2D = 57.2957795130823;
      double axis[3],norm[3],binorm[3],sigma[3],C[3][3],ang,p[3];
      double x,y,xold,yold;

      axis[0] = cos(lngc)*cos(latc);
      axis[1] = sin(lngc)*cos(latc);
      axis[2] = sin(latc);

      if (axis[2]*axis[2] < 0.5) {
         norm[0] = 0.0;
         norm[1] = 0.0;
         norm[2] = 1.0;
      }
      else {
         norm[0] = 1.0;
         norm[1] = 0.0;
         norm[2] = 0.0;
      }
      VxV(axis,norm,binorm);
      UNITV(binorm);
      SimpRot(binorm,rad,C);
      MxV(C,axis,sigma);
      SimpRot(axis,0.0,C);
      MxV(C,sigma,p);
      xold = atan2(p[1],p[0])*R2D;
      yold = asin(p[2])*R2D;
      glBegin(GL_LINES);
         for(ang=0.0;ang<TwoPi;ang+=0.005*TwoPi){
            SimpRot(axis,ang,C);
            MxV(C,sigma,p);
            x = atan2(p[1],p[0])*R2D;
            y = asin(p[2])*R2D;
            if (fabs(x-xold) < 180.0) {
               glVertex2f(xold,yold);
               glVertex2f(x,y);
            }
            else if (x > xold) {
               glVertex2f(xold+360.0,yold);
               glVertex2f(x,y);
               glVertex2f(xold,yold);
               glVertex2f(x-360.0,y);
            }
            else {
               glVertex2f(xold-360.0,yold);
               glVertex2f(x,y);
               glVertex2f(xold,yold);
               glVertex2f(x+360.0,y);
            }
            xold = x;
            yold = y;
         }
      glEnd();

}
/*********************************************************************/
/* Draws a full grid of any orientation on a Mercator projection.
   CVA is the DCM from the Axis frame to the Viewing frame           */
void DrawMercatorGrid(double CVA[3][3])
{
      long min = 30; /* Degrees between each minor gridline */
      long maj = 90; /* Degrees between each major gridline */

      double D2R = 0.0174532925199433;

      double norm[3];
      double x[3] = {1,0,0};
      double z[3] = {0,0,1};

      long ang; /* Tracked in degrees */
      double lng,lat;
      double CNA[3][3]; /* DCM from normal vector to axis frame */
      double CNV[3][3]; /* DCM from normal vector to viewing frame */

      /* Latitude lines */
      MxV(CVA,z,norm);

      VecToLngLat(norm,&lng,&lat);

      for (ang = min; ang < 179; ang = ang+min) {
         glLineWidth(1.0);
         DrawSmallCircle(lng, lat, ang*D2R);
      }

      for (ang = maj; ang < 179; ang = ang+maj) {
         glLineWidth(2.0);
         DrawSmallCircle(lng, lat, ang*D2R);
      }

      /* Longitude lines */
      for (ang = 0; ang < 179; ang = ang+min) {
         SimpRot(z,ang*D2R,CNA);
         MxMT(CNA,CVA,CNV);
         MTxV(CNV,x,norm);

         VecToLngLat(norm,&lng,&lat);

         glLineWidth(1.0);
         DrawSmallCircle(lng,lat,90*D2R);
      }

      for (ang = 0; ang < 179; ang = ang+maj) {
         SimpRot(z,ang*D2R,CNA);
         MxMT(CNA,CVA,CNV);
         MTxV(CNV,x,norm);

         VecToLngLat(norm,&lng,&lat);

         glLineWidth(2.0);
         DrawSmallCircle(lng,lat,90*D2R);
      }

      glLineWidth(1);
}
/*********************************************************************/
/* Draws a great circle arc between two points (angular distance must
   be less than 180 degrees)                                         */
void DrawMercatorLine(double lngA, double latA, double lngB, double latB)
{
      double R2D = 57.2957795130823;
      double A[3],B[3],norm[3],C[3][3],ang,totalang,p[3];
      double x,y,xold,yold;

      A[0] = cos(lngA)*cos(latA);
      A[1] = sin(lngA)*cos(latA);
      A[2] = sin(latA);

      B[0] = cos(lngB)*cos(latB);
      B[1] = sin(lngB)*cos(latB);
      B[2] = sin(latB);

      totalang = acos(VoV(A,B));

      VxV(A,B,norm);
      UNITV(norm);
      SimpRot(A,0.0,C);
      MxV(C,A,p);
      VecToLngLat(p,&xold,&yold);
      xold *= R2D;
      yold *= R2D;
      glBegin(GL_LINES);
         for(ang = 0.0; ang < 1.01*totalang; ang += 0.05*totalang) {
            SimpRot(norm,-ang,C);
            MxV(C,A,p);
            VecToLngLat(p,&x,&y);
            x *= R2D;
            y *= R2D;
            
            if (fabs(xold-x) > 180) { /* >180deg (crosses 180lng) - draw two lines */
               if (xold < 0) {
                  glVertex2f(xold,yold); 
                  glVertex2f(x-360,y);
                  glVertex2f(xold+360,yold);
                  glVertex2f(x,y);
               } 
               else {
                  glVertex2f(xold,yold); 
                  glVertex2f(x+360,y);
                  glVertex2f(xold-360,yold);
                  glVertex2f(x,y);
               }
            } 
            else { /* <180deg - draw one line */
               glVertex2f(xold,yold); 
               glVertex2f(x,y);
            }
            
            xold = x;
            yold = y;
         }
      glEnd();

}
/**********************************************************************/
/* Draws a square FOV on a Mercator projection using DrawMercatorLine.
   Square is centered on x-axis of CCV; FOV half-angles given in radians.
   CVS = DCM from Center of square to Viewing frame                   */
void DrawMercatorSquare(double CVS[3][3], double FOV[2])
{
      double p[3],q[3];
      double zaxis[3] = {0, 0, 1};
      double lngA,latA,lngB,latB;
      double xang[4],yang[4];
      double CPC[3][3]; /* DCM from Center to corner Point */
      long i,j;


      xang[0] = -FOV[0]; /* x to the left, y up, z out of the sensor in boresight direction */
      xang[1] =  FOV[0];
      xang[2] =  FOV[0];
      xang[3] = -FOV[0];

      yang[0] = -FOV[1];
      yang[1] = -FOV[1];
      yang[2] =  FOV[1];
      yang[3] =  FOV[1];

      for (i=0; i<4; i++) {
         A2C(21,xang[i],-yang[i],0.0,CPC);
         MxV(CPC,zaxis,q); /* Assume sensor axis is z-axis */
         MxV(CVS,q,p);
         VecToLngLat(p,&lngA,&latA);

         j = (i+1)%4;
         A2C(21,xang[j],-yang[j],0.0,CPC);
         MxV(CPC,zaxis,q); /* Assume sensor axis is z-axis */
         MxV(CVS,q,p);
         VecToLngLat(p,&lngB,&latB);

         DrawMercatorLine(lngA,latA,lngB,latB);
      }
}
/**********************************************************************/
/* Draws a small X at the lat/lng point of a vector, as well as label;
   lat and lng are in radians                                         */
void DrawMercatorVector(double lng, double lat, char *label)
{
      double R2D = 57.2957795130823;

      glLineWidth(1);

      lat *= R2D;
      lng *= R2D;

      glBegin(GL_LINES);
         glVertex2f(lng-2,lat-2);
         glVertex2f(lng+2,lat+2);
         glVertex2f(lng+2,lat-2);
         glVertex2f(lng-2,lat+2);
      glEnd();

      glRasterPos2f(lng+2.5*strlen(label),lat-12);
      DrawBitmapString(GLUT_BITMAP_8_BY_13,label);
}
/**********************************************************************/
/* Draws all 6 primary axes on a Mercator projection
   CAV is the DCM from the Axis frame to the Viewing frame            */
void DrawMercatorAxes(double CVA[3][3], char *label)
{
      double TwoPi = 6.28318530717959;

      double x[6] = {1,-1, 0, 0, 0, 0};
      double y[6] = {0, 0, 1,-1, 0, 0};
      double z[6] = {0, 0, 0, 0, 1,-1};
      
      double a[3],v[3];
      long i;
      double lat,lng;
      char str[20];

      for (i=0; i<6; i++) {
         a[0] = x[i];
         a[1] = y[i];
         a[2] = z[i];

         MxV(CVA,a,v);

         VecToLngLat(v,&lng,&lat);

         if (a[0]+a[1]+a[2] > 0) {
            sprintf(str,"+%s%ld",label,(long)i/2 + 1);
         } 
         else {
            sprintf(str,"-%s%ld",label,(long)i/2 + 1);            
         }

         DrawMercatorVector(lng,lat,str);

         if (lng > 180-8*strlen(label)) {
            DrawMercatorVector(lng-TwoPi,lat,str);
         }

         if (lng < -180+8*strlen(label)) {
            DrawMercatorVector(lng+TwoPi,lat,str);
         }

      }
}
/**********************************************************************/
/*  When porting to a new platform, check for OpenGL version,        */
/*  extensions, etc.                                                  */
void CheckOpenGLProperties(void)
{
      long ErrCode;
      GLint MaxTex;

      printf("OpenGL Vendor: %s\n",glGetString(GL_VENDOR));
      printf("OpenGL Renderer: %s\n",glGetString(GL_RENDERER));
      printf("OpenGL Version: %s\n",glGetString(GL_VERSION));
      printf("OpenGL Extensions: %s\n",glGetString(GL_EXTENSIONS));

#ifdef GL_GLEXT_PROTOTYPES
      printf("GL_GLEXT_PROTOTYPES defined\n");
#else
      printf("GL_GLEXT_PROTOTYPES not defined!\n");
#endif
#ifdef GL_VERSION_1_3
      printf("GL_VERSION_1_3 defined\n");
#endif
#ifdef GL_GLEXT_LEGACY
      printf("GL_GLEXT_LEGACY defined\n");
#endif
#ifdef GL_ARB_multitexture
      printf("GL_ARB_multitexture defined\n");
#endif
#ifdef ETS_GLEXT_CHECK
      printf("ETS_GLEXT_CHECK defined\n");
#endif

      ErrCode = (long) glGetError();
      printf("OpenGL Error %ld: %s\n",ErrCode,gluErrorString(ErrCode));

      glGetIntegerv(GL_MAX_TEXTURE_UNITS_ARB,&MaxTex);
      printf("OpenGL Max Texture Units: %i\n",(int) MaxTex);
}
/**********************************************************************/
/*  The Hammer (or Aitoff-Hammer) projection is an equal-area map     */
/*  projection.  This function copied from Wikipedia.                 */
/*  -2*SqrtTwo < x < +2*SqrtTwo                                       */
/*  -SqrtTwo < y < +SqrtTwo                                           */
void HammerProjection(double Lng, double Lat, double *x, double *y)
{
      double SQRTTWO = 1.41421356237310;

      double CosLat,SinLat,CosHalfLng,SinHalfLng,Den;

      CosLat = cos(Lat);
      SinLat = sin(Lat);
      CosHalfLng = cos(0.5*Lng);
      SinHalfLng = sin(0.5*Lng);
      Den = sqrt(1.0+CosLat*CosHalfLng);

      *x = 2.0*SQRTTWO*CosLat*SinHalfLng/Den;
      *y = SQRTTWO*SinLat/Den;

}
/**********************************************************************/
void VecToCube(long N, double p[3], long *f, long *i, long *j)
{
      double MaxComponent = 0.0;
      double MidPoint = 0.5*(N-1.0);
      long k;

      for(k=0;k<3;k++) {
         if (fabs(p[k]) > MaxComponent) {
            MaxComponent = fabs(p[k]);
            *f = k;
            if (p[k] < 0.0) *f += 3;
         }
      }

      if (*f == 0) {
         *i = (long) (MidPoint*(1.0-p[1]/p[0]));
         *j = (long) (MidPoint*(1.0-p[2]/p[0]));
      }
      else if (*f == 1) {
         *i = (long) (MidPoint*(1.0+p[2]/p[1]));
         *j = (long) (MidPoint*(1.0+p[0]/p[1]));
      }
      else if (*f == 2) {
         *i = (long) (MidPoint*(1.0-p[0]/p[2]));
         *j = (long) (MidPoint*(1.0+p[1]/p[2]));
      }
      else if (*f == 3) {
         *i = (long) (MidPoint*(1.0+p[1]/p[0]));
         *j = (long) (MidPoint*(1.0-p[2]/p[0]));
      }
      else if (*f == 4) {
         *i = (long) (MidPoint*(1.0+p[2]/p[1]));
         *j = (long) (MidPoint*(1.0+p[0]/p[1]));
      }
      else {
         *i = (long) (MidPoint*(1.0+p[0]/p[2]));
         *j = (long) (MidPoint*(1.0+p[1]/p[2]));
      }
}
/**********************************************************************/
void CubeToVec(long N, long f, long i, long j, double p[3])
{
      double MidPoint = 0.5*(N-1.0);
      double HalfN = 0.5*N;

      if (f == 0) {
         /* +X */
         p[0] = 1.0;
         p[1] = -(((double) i) - MidPoint)/HalfN;
         p[2] = -(((double) j) - MidPoint)/HalfN;
      }
      else if (f == 1) {
         /* +Y */
         p[1] = 1.0;
         p[2] =  (((double) i) - MidPoint)/HalfN;
         p[0] =  (((double) j) - MidPoint)/HalfN;
      }
      else if (f == 2) {
         /* +Z */
         p[2] = 1.0;
         p[1] = -(((double) i) - MidPoint)/HalfN;
         p[0] =  (((double) j) - MidPoint)/HalfN;
      }
      else if (f == 3) {
         /* -X */
         p[0] = -1.0;
         p[1] = -(((double) i) - MidPoint)/HalfN;
         p[2] =  (((double) j) - MidPoint)/HalfN;
      }
      else if (f == 4) {
         /* -Y */
         p[1] = -1.0;
         p[2] = -(((double) i) - MidPoint)/HalfN;
         p[0] =  (((double) j) - MidPoint)/HalfN;
      }
      else {
         /* -Z */
         p[2] = -1.0;
         p[1] = -(((double) i) - MidPoint)/HalfN;
         p[0] = -(((double) j) - MidPoint)/HalfN;
      }
      UNITV(p);
}
/*********************************************************************/
double ProcTex2D(double x, double y, double Xunit, double Yunit, long Noct)
{
      long k;
      long ix,iy;
      double kx,ky,fx,fy;
      double z = 0.0;

      for(k=1;k<=Noct;k++) {
         kx = ((double) k)*x/Xunit;
         ky = ((double) k)*y/Yunit;
         ix = (long) kx;
         iy = (long) ky;
         fx = kx-ix;
         fy = ky-iy;
         z += CubicInterp2D(PRN2D(ix,iy),PRN2D(ix+1,iy),
                            PRN2D(ix,iy+1),PRN2D(ix+1,iy+1),
                            fx,fy)/((double) k);
      }
      z = 0.5+0.5*z;
      if (z > 1.0) z = 1.0;
      if (z < 0.0) z = 0.0;
      return(z);
}
/*********************************************************************/
double ProcTex3D(double x, double y, double z,
                double Xunit, double Yunit, double Zunit,
                long Noct, double Persist)
{
      long k;
      long ix,iy,iz;
      double kx,ky,kz,fx,fy,fz;
      double PT3D = 0.0;
      double Scale = Persist;
      double SumScale = 0.0;

      /*while(x < 0.0) x += Xunit;
      **while(y < 0.0) y += Yunit;
      **while(z < 0.0) z += Zunit;
      */

      for(k=1;k<=Noct;k++) {
         kx = ((double) k)*x/Xunit;
         ky = ((double) k)*y/Yunit;
         kz = ((double) k)*z/Zunit;
         ix = (long) kx;
         iy = (long) ky;
         iz = (long) kz;
         fx = kx-ix;
         fy = ky-iy;
         fz = kz-iz;
         PT3D += Scale*CubicInterp3D(
            PRN3D(ix  ,iy  ,iz  ),PRN3D(ix+1,iy  ,iz  ),
            PRN3D(ix  ,iy+1,iz  ),PRN3D(ix+1,iy+1,iz  ),
            PRN3D(ix  ,iy  ,iz+1),PRN3D(ix+1,iy  ,iz+1),
            PRN3D(ix  ,iy+1,iz+1),PRN3D(ix+1,iy+1,iz+1),
            fx,fy,fz);
         SumScale += Scale;
         Scale *= Persist;
      }
      PT3D = 0.5+0.5*PT3D/SumScale;
      if (PT3D > 1.0) PT3D = 1.0;
      if (PT3D < 0.0) PT3D = 0.0;
      return(PT3D);
}
/**********************************************************************/
double SphereTex(double lng, double lat,
   double Xunit, double Yunit, double Zunit, long Noct, double Persist)
{
      double clat = cos(lat);
      double x = cos(lng)*clat;
      double y = sin(lng)*clat;
      double z = sin(lat);
      double f = ProcTex3D(x+1.0,y+1.0,z+1.0,Xunit,Yunit,Zunit,Noct,Persist);
      return(f);
}
/**********************************************************************/
#ifdef _USE_SHADERS_
GLuint TextToShader(GLchar *Text,GLuint Type,const char *Name)
{
      GLchar **SourcePtr;
      GLint Success;
      char *InfoLog;
      GLint LogSize;
      GLuint Shader;

      Shader = glCreateShader(Type);
      SourcePtr = &Text;
      glShaderSource(Shader,1,(const GLchar **) SourcePtr,NULL);
      glCompileShader(Shader);

      glGetShaderiv(Shader,GL_COMPILE_STATUS,&Success);

      if (!Success) printf("Error in %s Shader compile\n",Name);
      glGetShaderiv(Shader,GL_INFO_LOG_LENGTH,&LogSize);
      if (LogSize > 0) {
         InfoLog = (char *) calloc(LogSize,sizeof(char));
         glGetShaderInfoLog(Shader,LogSize,NULL,InfoLog);
         printf("%s Shader info log:\n%s\n",Name,InfoLog);
         free(InfoLog);
      }

      return(Shader);
}
/**********************************************************************/
GLuint BuildShaderProgram(GLuint VtxShader, GLuint FragShader, const char *Name)
{
      GLint Success;
      char *InfoLog;
      GLint LogSize;
      GLuint ShaderProgram;

      ShaderProgram = glCreateProgram();
      glAttachShader(ShaderProgram,VtxShader);
      glAttachShader(ShaderProgram,FragShader);
      glLinkProgram(ShaderProgram);

      glGetProgramiv(ShaderProgram,GL_LINK_STATUS,&Success);

      if (!Success) printf("Error in %s ShaderProgram link\n",Name);
      glGetProgramiv(ShaderProgram,GL_INFO_LOG_LENGTH,&LogSize);
      if (LogSize > 0) {
         InfoLog = (char *) calloc(LogSize,sizeof(char));
         glGetProgramInfoLog(ShaderProgram,LogSize,NULL,InfoLog);
         printf("%s ShaderProgram info log:\n%s\n",Name,InfoLog);
         free(InfoLog);
      }

      return(ShaderProgram);
}
/**********************************************************************/
void ValidateShaderProgram(GLuint ShaderProgram, const char *Name)
{

      GLint Success;
      char *InfoLog;
      GLint LogSize;

      glValidateProgram(ShaderProgram);
      glGetProgramiv(ShaderProgram,GL_VALIDATE_STATUS,&Success);

      if (!Success) printf("Error in %s ShaderProgram validation\n",Name);
      glGetProgramiv(ShaderProgram,GL_INFO_LOG_LENGTH,&LogSize);
      if (LogSize > 0) {
         InfoLog = (char *) calloc(LogSize,sizeof(char));
         glGetProgramInfoLog(ShaderProgram,LogSize,NULL,InfoLog);
         printf("ShaderProgram validation log:\n%s\n",InfoLog);
         free(InfoLog);
      }

}
#endif

/* #ifdef __cplusplus
** }
** #endif
*/

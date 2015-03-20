#include "val_atm_hp.h"
#include "val_array.h"
#include "val_directory.h"
//====================================================================
//  VAL -- Virtual Almanac Library
//====================================================================
bool Harris (const ZVector& pos, const ZVector& sunpos,
   const real& altitude, const real& f107, real& density)
   {
   // These are initialized once.  They are set in the first call
   // to this routine
   static bool tried = false;
   static ZArray1toN<real> F107(10,0.0);
   static ZArray1toN<real> Altitude (60,0.0);
   static ZArray2d1toN<real> MinDensity (10,60,0.0);
   static ZArray2d1toN<real> MaxDensity (10,60,0.0);
   static integer NDENS = 0;
   // now we begin
   density = 0;
   if (!tried)
      {
      tried = false;
      // Load Coefficients
      ZDirEntry de (ValDataPath + "dbs_atmosden.pc");
      de += "atmosphere";
      de += "earth";
      ifstream f (~de.GetPathName(),ios::binary);
      if (f.good())
         {
         for (integer i=1;  i<=10;  ++i)
            {
            for (integer j=1;  j<=60;  ++j)
               {
               f.read ((char*)&Altitude [j],8);
               f.read ((char*)&MinDensity (i,j),8);
               f.read ((char*)&MaxDensity (i,j),8);
               }
            char name[72];
            f.read(name,72);
            ZString namex(name);
            UnStr (namex(56,58),F107[i]);
            char junk[4];
            f.read (junk,4);
            f.read ((char*)&NDENS,4);
            char spare[160];
            f.read(spare,160);
            }
         }
      }

   if (NDENS == 0)
      return false;
   else
      {
      // Data loaded, can continue
      // altitude to0 high, density = 0
      if (altitude > Altitude[NDENS]) return true;
      // Calculate F107 (first) index ilo,ihi,ifactor
      integer ilo = 0;
      for (integer i=1;  i<=10-1 && ilo==0;  ++i)
         if (f107 < F107[i+1])
            ilo = i;
      if (ilo==0) ilo=9;
      integer ihi = ilo+1;
      real ifactor = (f107-F107[ilo]) / (F107[ihi]-F107[ilo]);
      // No extrapolation
      if (ifactor < 0) ifactor = 0;
      if (ifactor > 1) ifactor = 1;
      // Calculate Altitude (second) index jlo,jhi,jfactor
      integer jlo = 0;
      for (integer j=1;  j<=NDENS-1 && jlo==0;  ++j)
         if (altitude < Altitude[j+1])
            jlo = j;
      integer jhi = jlo+1;
      real jfactor = (altitude-Altitude[jlo]) / (Altitude[jhi]-Altitude[jlo]);
      // No extrapolation
      if (jfactor < 0) jfactor = 0;
      if (jfactor > 1) jfactor = 1;
      // do f107 (i) interpolation
      real jlomindensity = MinDensity(ilo,jlo) +
         ifactor * (MinDensity(ihi,jlo)-MinDensity(ilo,jlo));
      real jhimindensity = MinDensity(ilo,jhi) +
         ifactor * (MinDensity(ihi,jhi)-MinDensity(ilo,jhi));
      real jlomaxdensity = MaxDensity(ilo,jlo) +
         ifactor * (MaxDensity(ihi,jlo)-MaxDensity(ilo,jlo));
      real jhimaxdensity = MaxDensity(ilo,jhi) +
         ifactor * (MaxDensity(ihi,jhi)-MaxDensity(ilo,jhi));
      // do altitude interpolation (exponetial)
      real maxdensity=jlomaxdensity*pow(jhimaxdensity/jlomaxdensity,jfactor);
      real mindensity=jlomindensity*pow(jhimindensity/jlomindensity,jfactor);
      //   CALCULATE THE DIURNAL BULGE APEX UNIT VECTOR BY ROTATING THE
      //   SUN UNIT VECTOR BY RHO4.
      ZVector usunpos = sunpos.Unitize();
      real bulgelag = 30.0*KReal::PI/180.0; // Bulge lag is 30 deg
      real cb=cos(bulgelag);
      real sb=sin(bulgelag);
      ZVector ubulgepos (usunpos[1]*cb-usunpos[2]*sb,
                         usunpos[1]*sb+usunpos[2]*cb,
                         usunpos[3]);
      //   FIND COSINE OF ANGLE BETWEEN DIURNAL BULGE AND THE SPACECRAFT
      //   UNIT VECTOR.
      ZVector upos = pos.Unitize();
      real cospsi=upos*ubulgepos;
      //   SET (COS(PSI/2))**3  AND USE IT TO FIND density.
      real cos = power(sqrt(abs(1.0+cospsi)/2.0),3);
      density = mindensity+(maxdensity-mindensity)*cos;
      density*=1e-12;
      return true;
      }
   }
//====================================================================

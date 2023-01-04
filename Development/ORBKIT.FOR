CCCCCCCCCCCCCCCCCCCCCCC   ORBITAL SUBROUTINES   CCCCCCCCCCCCCCCCCCCCCCCC
C  All subroutines here assume the right-to-left convention.

CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C  Find position and velocity, given initial position, velocity, and
C  true anomaly difference.  This routine good for all orbits.

      SUBROUTINE RV02RV(mu,xr0,xv0,anom,xr,xv)

      IMPLICIT NONE
      REAL*8 mu,xr0(3),xv0(3),anom,xr(3),xv(3)
      REAL*8 sqmu,cth,sth,cth1,s0,R0xV0(3),p,r0,sqp,r,F,Ft,G,Gt
      INTEGER*4 i      

      sqmu=dsqrt(mu)
      cth=dcos(anom)
      sth=dsin(anom)
      cth1=1.0d0-cth

      CALL VoV(xr0,xv0,s0)
      s0=s0/sqmu
      CALL VxV(xr0,xv0,R0xV0)
      CALL VoV(R0xV0,R0xV0,p)
      p=p/mu         
      CALL MAGV(xr0,r0)
      sqp=dsqrt(p)
      
      r=p*r0/(r0+(p-r0)*cth-sqp*s0*sth)

      F=1.0d0-r/p*cth1

      G=r*r0*sth/(sqmu*sqp)

      Ft=sqmu/(r0*p)*(s0*cth1-sqp*sth)

      Gt=1.0d0-r0/p*cth1

      do i=1,3
         xr(i) = F *xr0(i) + G *xv0(i)
         xv(i) = Ft*xr0(i) + Gt*xv0(i)
      enddo

      RETURN
      END

CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C  Compute position and velocity given orbital elements.  Good only
C  for circular and elliptical orbits.
      SUBROUTINE EPH2RV(mu,a,e,i,LAN,omg,dt,th,r,v)

      IMPLICIT NONE
      REAL*8 mu,a,e,i,LAN,omg,th,r(3),v(3)
      REAL*8 p,CPN(3,3),cth,sth,pr(3),pv(3),c1,c2
      REAL*8 M,EA,dEA,w,dt,pi

      DATA pi /3.14159265359d0/

      w=dsqrt(mu/a**3.0d0)
      M = dmod(w*dt, 2.0d0*pi)
         EA = M
  5      dEA=(M-EA+e*dsin(EA))/(1.0d0-e*dcos(EA))
         EA=EA+dEA
         if (dabs(dEA) .gt. 1.0d-6) go to 5
         th=2.d0*datan(dsqrt((1.d0+e)/(1.d0-e))
     &           *dtan(EA/2.d0))

      sth = dsin(th)
      cth = dcos(th)
      p = a*(1.0d0-e*e)
      c1 = p/(1+e*cth)
      c2 = dsqrt(mu/p)
      pr(1) = c1*cth    
      pr(2) = c1*sth    
      pr(3) = 0.0d0
      pv(1) = -c2*sth
      pv(2) = c2*(e+cth)
      pv(3) = 0.0d0
      CALL A2C(313,LAN,i,omg,CPN)
      CALL MTxV(CPN,pr,r)
      CALL MTxV(CPN,pv,v)

      RETURN
      END

CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C  Compute orbital elements, given position and velocity.  Good only 
C  for circular and elliptical orbits.

      SUBROUTINE RV2EPH(mu,xr,xv,a,e,i,LAN,omg,th,tp)

      IMPLICIT NONE
      REAL*8 mu,a,e,i,LAN,omg,th,tp,xr(3),xv(3),CPN(3,3)
      REAL*8 xh(3),xe(3),rhat(3),r,v,vxh(3),cth,hxe(3),EA
      REAL*8 rohxe,twopi
      INTEGER*4 j

      DATA twopi /6.28318530718d0/

      CALL MAGV(xr,r)
      CALL MAGV(xv,v)
      a = 0.5d0*mu/(mu/r-0.5d0*v*v)

      CALL VxV(xr,xv,xh)
      do j=1,3
         rhat(j) = xr(j)
      enddo
      CALL UNITV(rhat)
      CALL VxV(xv,xh,vxh)
      do j=1,3
           xe(j) = vxh(j)/mu-rhat(j)
      enddo
      CALL MAGV(xe,e)

      CALL UNITV(xe)
      CALL VoV(rhat,xe,cth)
      th = dacos(cth)

      CALL UNITV(xh)
      CALL VxV(xh,xe,hxe)
      CALL VoV(rhat,hxe,rohxe)
      if (rohxe .lt. 0.0d0) th = twopi - th
      do j=1,3
         CPN(1,j) = xe(j)
         CPN(2,j) = hxe(j)
         CPN(3,j) = xh(j)
      enddo
      LAN = datan2(CPN(3,1),-CPN(3,2))
      i   =  dacos(CPN(3,3))
      omg =  datan2(CPN(1,3),CPN(2,3))

      EA=2.0d0*DATAN(DSQRT((1.0d0-e)/(1.0D0+e))
     & *DTAN(th/2.0D0))
      tp=-(EA-e*DSIN(EA))*DSQRT(a*a*a/mu)

      RETURN
      END

CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C   Converts NORAD 2-line elements to more familiar units.

      SUBROUTINE TWOLINE2EPH(EpochYear,EpochDay,Inc,RA,
     &   Ecc,omg,MeanAnom,MeanMotion,
     &   JDepoch,orbaxis,orbecc,orbinc,orbLAN,orbomg,
     &   orbtp)

      IMPLICIT NONE

      REAL*8 EpochDay,Inc,RA,Ecc,omg,MeanAnom
      REAL*8 MeanMotion,JDepoch,orbaxis,orbecc
      REAL*8 orbinc,orbLAN,orbomg,orbtp
      REAL*8 EpochYear,Year,Mon
      REAL*8 Day,w,TWOPI,D2R,mu

      DATA TWOPI /6.28318530718d0/
      DATA D2R /1.74532925199d-2/
      DATA mu /3.98604D14/

      IF (EpochYear .gt. 50.0d0) THEN
         Year=1900.0d0+EpochYear
      ELSE
         Year=2000.0d0+EpochYear
      ENDIF
      CALL DOY2MD(Year,EpochDay,Mon,Day)
      CALL YMD2JD(Year,Mon,Day,JDepoch)

      w=MeanMotion*TWOPI/86400.0d0
      orbaxis=(mu/w/w)**(1.0d0/3.0d0)
      orbecc=Ecc*1.0d-7
      orbinc=Inc*D2R
      orbLAN=RA*D2R
      orbomg=omg*D2R
      orbtp=-MeanAnom*D2R/w

      RETURN
      END

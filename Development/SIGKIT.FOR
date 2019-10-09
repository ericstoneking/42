CCCCCCCCCCCCCCCCCC   SIGNAL PROCESSING SUBROUTINES   CCCCCCCCCCCCCCCCCCC
C  No subroutines here are sensitive to right-to-left issues.

CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C     UNIFORM RANDOM NUMBER GENERATOR
C
C   This portable random number generator is taken from Numerical
C   Recipes, p. 271, where it is called 'ran1'.  It returns a 
C   uniform deviate in the range (0:1).

      SUBROUTINE RNG(X)

      IMPLICIT NONE
      INTEGER*4 idum,IA,IM,IQ,IR,NTAB,NDIV
      REAL*8 X,AM,EPS,RNMX
      PARAMETER(IA=16807,IM=2147483647,AM=1.0d0/IM,IQ=127773,IR=2836,
     &   NTAB=32,NDIV=1+(IM-1)/NTAB,EPS=1.2d-7,RNMX=1.0d0-EPS)
      INTEGER*4 j,k,iv(NTAB),iy

      SAVE iv,iy,idum

      DATA iv /NTAB*0/, iy /0/
      DATA idum /-5/  ! Any negative number to initialize sequence

      IF (idum .le. 0 .or. iy .eq. 0) THEN
         idum=max(-idum,1)
         do j=NTAB+8,1,-1
            k=idum/IQ
            idum=IA*(idum-k*IQ)-IR*k
            if (idum .lt. 0) idum=idum+IM
            if (j .le. NTAB) iv(j)=idum
         enddo
         iy=iv(1)
      ENDIF

      k=idum/IQ
      idum=IA*(idum-k*IQ)-IR*k
      if (idum .lt. 0) idum=idum+IM
      j=1+iy/NDIV
      iy=iv(j)
      iv(j)=idum
      X=min(AM*iy,RNMX)
      
      RETURN
      END

CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C     RANDOM NOISE GENERATOR
C       type = 1 gives uniform distribution
C       type = 2 gives Gaussian distribution

      SUBROUTINE AddNoise(var,type,mean,sdev)

      IMPLICIT NONE
      REAL*8 var
      INTEGER*4 type
      real*8 X1,X2,X3  
      real*8 mean,sdev
      real*8 twopi,X
      data twopi /6.28318530718D0/

      if (type .eq. 1) then
C        ! Uniform Distribution
         CALL RNG(X1)
         X = X1*sdev + (1.0D0-X1)*mean
       else
C        ! Gaussian Distribution
         CALL RNG(X1)
         CALL RNG(X2)
         X3 = dsqrt(-2.0D0*dlog(X1))*DCOS(twopi*X2)*sdev
         ! Limit at 4 sigma
         if (dabs(X3) .gt. 4.0D0*sdev) X3=dsign(4.0D0*sdev,X3)
         X=mean+X3
      endif
      var=var+X
      
      RETURN
      END

CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C     Discrete realization of a low-pass filter

C     X is filter input
C     Y is filter output
C     w is frequency, rad/sec
C     z is damping ratio
C     T is sample time
C     i is index, <= NF

      SUBROUTINE LOWPASS(X,Y,w,z,T,i)

      IMPLICIT NONE
      INTEGER*4 NF
C ... Realizing you may want to have more than one lowpass filter
C     in a sim, we introduce the index i.  NF is the maximum number
C     of lowpass filters you can run concurrently.
      PARAMETER(NF = 13)
      REAL*8 X, Y, w, z, T, B(NF), A0(NF), A1(NF), A2(NF)
      REAL*8 X0(NF), X1(NF), X2(NF), Y0(NF), Y1(NF), Y2(NF)
      INTEGER*4 i
      LOGICAL*1 First(NF)

      SAVE X1,X2,Y1,Y2,First

      DATA First /NF*.True./

      IF (First(i)) THEN
         First(i)=.False.

         ! Discrete coefficients
         A0(i) = 4.0d0/T/T + 4.0d0*z*w/T + w*w
         A1(i) = 2.0d0*(w*w - 4.0d0/T/T)
         A2(i) = 4.0d0/T/T - 4.0d0*z*w/T + w*w
         B(i)  = w*w

      ENDIF

      X0(i) = X

      Y0(i) = (B(i)*(X0(i)+2.0d0*X1(i)+X2(i))
     &        -A1(i)*Y1(i)-A2(i)*Y2(i))/A0(i)

      ! Trap underflow when zero inputs
      IF (dabs(Y0(i)) .lt. 1.0d-30) Y0(i) = 0.0d0

      Y = Y0(i)

      X2(i) = X1(i)
      X1(i) = X0(i)
      Y2(i) = Y1(i)
      Y1(i) = Y0(i)

      RETURN
      END

CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C     Discrete realization of a lead-lag filter

C     X is filter input
C     Y is filter output
C     a is lead root, rad/sec
C     b is lag root, rad/sec
C     T is sample time
C     i is index, <= NF

      SUBROUTINE LEADLAG(X,Y,a,b,T,i)

      IMPLICIT NONE
      INTEGER*4 NF
C ... Realizing you may want to have more than one lead-lag filter
C     in a sim, we introduce the index i.  NF is the maximum number
C     of lead-lag filters you can run concurrently.
      PARAMETER(NF = 3)
      REAL*8 X, Y, a, b, T, K(NF), alpha(NF), beta(NF)
      REAL*8 X0(NF), X1(NF), Y0(NF), Y1(NF)
      INTEGER*4 i
      LOGICAL*1 First(NF)

      SAVE X1,Y1,alpha,beta,K

      DATA First /NF*.True./

      IF (First(i)) THEN
         First(i)=.False.
         alpha(i)=dexp(-a*T)
         beta(i)=dexp(-b*T)
         K(i)=a*(1.0d0-beta(i))/(b*(1.0d0-alpha(i)))
      ENDIF

      X0(i) = X

      Y0(i) = beta(i)*Y1(i)+K(i)*(X0(i)-alpha(i)*X1(i))

      ! Trap underflow when zero inputs
      IF (dabs(Y0(i)) .lt. 1.0d-30) Y0(i) = 0.0d0

      Y = Y0(i)

      X1(i) = X0(i)
      Y1(i) = Y0(i)

      RETURN
      END

CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C     Discrete realization of a lag filter

C     X is filter input
C     Y is filter output
C     a is lag root, rad/sec
C     T is sample time
C     i is index, <= NF

      SUBROUTINE LAG(X,Y,a,T,i)

      IMPLICIT NONE
      INTEGER*4 NF
C ... Realizing you may want to have more than one lag filter
C     in a sim, we introduce the index i.  NF is the maximum number
C     of lag filters you can run concurrently.
      PARAMETER(NF = 3)
      REAL*8 X, Y, a, T, K(NF), alpha(NF)
      REAL*8 X0(NF), X1(NF), Y0(NF), Y1(NF)
      INTEGER*4 i
      LOGICAL*1 First(NF)

      SAVE K,alpha,X1,Y1,First

      DATA First /NF*.True./

      IF (First(i)) THEN
         First(i)=.False.

         alpha(i)=dexp(-a*T)
         K(i)=(1.0d0-alpha(i))/2.0d0
      ENDIF

      X0(i) = X

      Y0(i) = alpha(i)*Y1(i)+K(i)*(X0(i)+X1(i))

      ! Trap underflow when zero inputs
      IF (dabs(Y0(i)) .lt. 1.0d-30) Y0(i) = 0.0d0

      Y = Y0(i)

      X1(i) = X0(i)
      Y1(i) = Y0(i)

      RETURN
      END

CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
      SUBROUTINE LIMIT(X,MAX)

      IMPLICIT NONE
      REAL*8 X,MAX

      IF(DABS(X) .GT. MAX) X=DSIGN(MAX,X)

      RETURN
      END

CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
      SUBROUTINE QUANTIZE(x,q)

      IMPLICIT NONE
      REAL*8 x,q
      INTEGER*4 y

      y=IDINT(x/q)
      x=q*DBLE(y)

      RETURN
      END

CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C     Discrete realization of a 2nd-order bandpass filter

C     X is filter input
C     Y is filter output
C     wc is center frequency, rad/sec
C     bw is bandwidth, rad/sec
C     T is sample time
C     i is index, <= NF

      SUBROUTINE BANDPASS2(X,Y,wc,bw,T,i)

      IMPLICIT NONE
      INTEGER*4 NF
C ... Realizing you may want to have more than one bandpass filter
C     in a sim, we introduce the index i.  NF is the maximum number
C     of bandpass filters you can run concurrently.
      PARAMETER(NF = 3)
      REAL*8 X, Y, wc, bw, z, T, B0(NF), A0(NF), A1(NF), A2(NF)
      REAL*8 X0(NF), X1(NF), X2(NF), Y0(NF), Y1(NF), Y2(NF)
      INTEGER*4 i
      LOGICAL*1 First(NF)

      SAVE B0,A0,A1,A2,X1,X2,Y1,Y2,First

      DATA First /NF*.True./

      IF (First(i)) THEN
         First(i)=.False.

         ! Damping ratio related to center frequency, bandwidth
         z = bw/(2.0d0*wc)

         ! Discrete coefficients
         B0(i) = 4.0d0*z*wc*T

         A0(i) = 4.0d0+4.0d0*z*wc*T+wc*wc*T*T
         A1(i) = 2.0d0*wc*wc*T*T-8.0d0
         A2(i) = 4.0d0-4.0d0*z*wc*T+wc*wc*T*T

      ENDIF


      X0(i) = X

      Y0(i) = (B0(i)*(X0(i)-X2(i))-A1(i)*Y1(i)-A2(i)*Y2(i))/A0(i)

      ! Trap underflow when zero inputs
      IF (dabs(Y0(i)) .lt. 1.0d-30) Y0(i) = 0.0d0

      Y = Y0(i)

      X2(i) = X1(i)
      X1(i) = X0(i)
      Y2(i) = Y1(i)
      Y1(i) = Y0(i)

      RETURN
      END

CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C     Discrete realization of a 4th-order bandpass filter

C     X is filter input
C     Y is filter output
C     wc is center frequency, rad/sec
C     bw is bandwidth, rad/sec
C     T is sample time
C     i is index, <= NF

      SUBROUTINE BANDPASS4(X,Y,wc,bw,T,i)

      IMPLICIT NONE
      INTEGER*4 NF
C ... Realizing you may want to have more than one bandpass filter
C     in a sim, we introduce the index i.  NF is the maximum number
C     of bandpass filters you can run concurrently.
      PARAMETER(NF = 3)
      REAL*8 X,Y,wc,bw,z,T,B0(NF),A0(NF),A1(NF),A2(NF),A3(NF),A4(NF),wT
      REAL*8 X0(NF), X1(NF), X2(NF), X3(NF), X4(NF)
      REAL*8 Y0(NF), Y1(NF), Y2(NF), Y3(NF), Y4(NF)
      INTEGER*4 i
      LOGICAL*1 First(NF)

      SAVE B0,A0,A1,A2,A3,A4,X1,X2,X3,X4,Y1,Y2,Y3,Y4,First

      DATA First /NF*.True./

      IF (First(i)) THEN
         First(i)=.False.

         ! Damping ratio related to center frequency, bandwidth
         z = bw/(2.0d0*wc)
         wT=wc*T

         ! Discrete coefficients
         B0(i) = 16.0d0*z*z*wT*wT

         A0(i) = (4.0d0+4.0d0*z*wT+wT*wT)**2
         A1(i) = 4.0d0*(wT-2.0d0)*(wT+2.0d0)*(4.0d0+4.0d0*z*wT+wT*wT)
         A2(i) = 96.0d0-16.0d0*wT*wT*(1.0d0+2.0d0*z*z)+6.0d0*(wT)**4
         A3(i) = 4.0d0*(wT-2.0d0)*(wT+2.0d0)*(4.0d0-4.0d0*z*wT+wT*wT)
         A4(i) = (4.0d0-4.0d0*z*wT+wT*wT)**2
      ENDIF

      X0(i) = X

      Y0(i) = (B0(i)*(X0(i)-2.0d0*X2(i)+X4(i))
     &        -A1(i)*Y1(i)-A2(i)*Y2(i)-A3(i)*Y3(i)-A4(i)*Y4(i))/A0(i)

      ! Trap underflow when zero inputs
      IF (dabs(Y0(i)) .lt. 1.0d-30) Y0(i) = 0.0d0

      Y = Y0(i)

      X4(i) = X3(i)
      X3(i) = X2(i)
      X2(i) = X1(i)
      X1(i) = X0(i)
      Y4(i) = Y3(i)
      Y3(i) = Y2(i)
      Y2(i) = Y1(i)
      Y1(i) = Y0(i)

      RETURN
      END




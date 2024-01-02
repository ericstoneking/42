CCCCCCCCCCCCCCCCCCCCCCC   CONTROLS SUBROUTINES   CCCCCCCCCCCCCCCCCCCCCCC
C   No subroutines here are sensitive to right-to-left issues.

CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C  Computes gains for a PD controller
C    B is 3dB bandwidth, in rad/sec
C    z is damping ratio
      SUBROUTINE PDGains(B,z,I,Kr,Kp)

      IMPLICIT NONE

      REAL*8 w,z,I,Kr,Kp,B,z2

      z2=z*z
c      w=B/dsqrt(1.0d0+2.0d0*z2+dsqrt(2.0d0+4.0d0*z2+4.0d0*z2*z2))
      w=B
      Kr=2.0D0*I*z*w
      Kp=I*w*w

      RETURN
      END

CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C  Computes gains for a PID controller
C   w is frequency of PD roots, rad/sec
C   z is damping ratio
C   s is ratio of integrator root to PD roots (< 1.0)

      SUBROUTINE PIDGains(w,z,s,I,Kr,Kp,Ki)

      IMPLICIT NONE
      REAL*8 w,z,s,I,Kr,Kp,Ki,sig

      sig=s*w

      Kr=I*(sig+2.0D0*z*w)
      Kp=I*(w*w+2.0D0*z*w*sig)
      Ki=I*sig*w*w

      RETURN
      END

CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C  Computes gains for a TOPEX-like PIID controller
      SUBROUTINE TPXPIID(w,I,Kr,Kp,Ki,Kii)

      IMPLICIT NONE
      REAL*8 w,I,Kr,Kp,Ki,Kii

      Kr=I*w
      Kp=3.0D0*Kr*w/10.0D0
      Ki=3.0D0*Kr*w*w/100.0D0
      Kii=Kr*w*w*w/1000.0D0

      RETURN
      END

CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C  Computes gains for an EOS-like PIID controller
      SUBROUTINE EOSPIID(w,I,Kr,Kp,Ki,Kii)

      IMPLICIT NONE
      REAL*8 w,I,Kr,Kp,Ki,Kii

      Kr=0.7d0*I*w
      Kp=3.0D0*Kr*w/10.0D0
      Ki=3.0D0*Kr*w*w/100.0D0
      Kii=Kr*w*w*w/1000.0D0

      RETURN
      END

CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C   A very common law used in early acquisition uses MTB's to damp
C   body rates wrt magnetic field.

      SUBROUTINE BDOTLaw(BVB,BdotGain,Mcmd)

      IMPLICIT NONE
      REAL*8 BVB(3),Bold(3),Mcmd(3),BdotGain
      INTEGER*4 i

      do i=1,3
         Mcmd(i)=-BdotGain*(BVB(i)-Bold(i))
         Bold(i)=BVB(i)
      enddo

      RETURN
      END

CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C        Single Axis Optimal Bang-Bang Control Law

      SUBROUTINE BANGBANG(x, xdot, Tmax, I, Band, Tcom)

      IMPLICIT NONE
      REAL*8 x,xdot,Tmax,I,Band,Tcom,switch
   
      
      switch=DABS(xdot)*xdot*I/(2.0d0*Tmax)+x

      IF (x .GE. 0.0D0) THEN
         IF (switch .LE. -Band) THEN
            Tcom = Tmax
         ELSEIF (switch .GE. Band) THEN
            Tcom = -Tmax
         ELSE
            Tcom = 0.0d0
         ENDIF
      ELSE
         IF (switch .GE. Band) THEN
            Tcom = -Tmax
         ELSEIF (switch .LE. Band) THEN
            Tcom = Tmax
         ELSE
            Tcom = 0.0d0
         ENDIF
      ENDIF

      RETURN
      END

CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C     Three-axis quaternion-based optimal bang-bang controller,
C     with deadband.

      SUBROUTINE QBANG(q,w,Tmax,IB,Band,Tcom)

      IMPLICIT NONE
      REAL*8 w(3),q(4),Tmax,Band,Tcom(3),IB(3,3)
      REAL*8 Kband
      INTEGER*4 i
 
C  Find unsaturated band width (deadband)
      Kband = Tmax/Band

C  Compute Tcom
      DO 2 i=1,3
         Tcom(i)=-Kband*(dabs(w(i))*w(i)*IB(i,i)
     &                   +4.0d0*Tmax*q(i)*dsign(1.0d0,q(4)))
         call limit(Tcom(i),Tmax)
 2    CONTINUE

      RETURN
      END

CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C     CONVERT CONTINUOUS SYSTEM TO DISCRETE SYSTEM
C
      SUBROUTINE C2D(F,G,T,PHI,GAMMA)

      IMPLICIT NONE
      INTEGER*4 NS,NC
      PARAMETER(NS=6,NC=3)
      REAL*8 F(NS,NS),G(NS,NC),T,PHI(NS,NS),GAMMA(NS,NC)
      REAL*8 U(NS,NS),PSI(NS,NS),TEMP(NS,NS)
      INTEGER*4 I,J,K,M

      DO I=1,NS
         U(I,I) = 1.0D0
      ENDDO

C     ! Construct PSI
      DO M=5,2,-1
         DO I=1,NS
            DO J=1,NS
               TEMP(I,J)=0.0D0
               DO K=1,NS
                  TEMP(I,J)=TEMP(I,J)+F(I,K)*PSI(K,J)
               ENDDO
            ENDDO
         ENDDO
         DO I=1,NS
            DO J=1,NS
               PSI(I,J)=U(I,J)+T/M*TEMP(I,J)
            ENDDO
         ENDDO
      ENDDO

C     ! Construct Gamma
      DO I=1,NS
         DO J=1,NC
            GAMMA(I,J)=0.0D0
            DO K=1,NS
               GAMMA(I,J)=GAMMA(I,J)+PSI(I,K)*G(K,J)
            ENDDO
            GAMMA(I,J)=T*GAMMA(I,J)
         ENDDO
      ENDDO

C     ! Construct PHI
      DO I=1,NS
         DO J=1,NS
            PHI(I,J)=0.0D0
            DO K=1,NS
               PHI(I,J)=PHI(I,J)+F(I,K)*PSI(K,J)
            ENDDO
            PHI(I,J)=U(I,J)+T*PHI(I,J)
         ENDDO
      ENDDO

      RETURN
      END

CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C     Predictor Estimator

C     Assumes NS states, NM measurements, NC controls

      SUBROUTINE XEST(PHI,GAMMA,H,L,u,y,xhat)

      IMPLICIT NONE
      INTEGER*4 NS,NM,NC
      PARAMETER(NS=6,NM=3,NC=3)
      REAL*8 PHI(NS,NS),GAMMA(NS,NC),H(NM,NS),L(NS,NM),u(NC)
      REAL*8 y(NM),xhat(NS)
      REAL*8 Hx(NM),Lz(NS),PHIx(NS),GAMMAu(NS)
      INTEGER*4 i,j

      DO i=1,NM
         Hx(i)=0.0D0
         DO j=1,NS
            Hx(i)=Hx(i)+H(i,j)*xhat(j)
         ENDDO
      ENDDO
      DO i=1,NS
         Lz(i)=0.0D0
         DO j=1,NM
            Lz(i)=Lz(i)+L(i,j)*(y(j)-Hx(j))
         ENDDO
      ENDDO
      DO i=1,NS
         PHIx(i)=0.0D0
         DO j=1,NS
            PHIx(i)=PHIx(i)+PHI(i,j)*xhat(j)
         ENDDO
      ENDDO
      DO i=1,NS
         GAMMAu(i)=0.0D0
         DO j=1,NC
            GAMMAu(i)=GAMMAu(i)+GAMMA(i,j)*u(j)
         ENDDO
      ENDDO
      DO i=1,NS
         xhat(i)=PHIx(i)+GAMMAu(i)+Lz(i)
      ENDDO

      RETURN
      END

CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C   
C   Discrete time steady-state Kalman gain by sweep method 
C   (Bryson & Ho).  Assumes NS states, NC controls and NM measurements.
C
C   Rv = Sensor noise covariance matrix
C   Rw = Process noise covariance matrix
C   L  = Kalman gain matrix
C   Err, Maxiter there for end conditions.

      SUBROUTINE DLQE(PHI,GAMMA1,H,Rv,Rw,L,ERR,MAXITER)

      IMPLICIT NONE
      INTEGER*4 NS,NC,NM
      PARAMETER(NS=6,NC=3,NM=3)
      REAL*8 PHI(NS,NS),GAMMA1(NS,NC),H(NM,NS)
      REAL*8 Rv(NM,NM),Rw(NC,NC),L(NS,NM),ERR
      REAL*8 P(NS,NS),M(NS,NS),HM(NM,NS),HMH(NM,NM)
      REAL*8 HMHR(NM,NM),HMHRI(NM,NM)
      REAL*8 LHM(NS,NS),FP(NS,NS),FPF(NS,NS),GR(NS,NC),GRG(NS,NS)
      REAL*8 dP,dPmax,OldP(NS,NS)
      INTEGER*4 I,J,K,MAXITER,ITER

      SAVE OldP

      DO I=1,NS
         DO J=1,NC
            GR(I,J)=0.0D0
            DO K=1,NC
               GR(I,J)=GR(I,J)+GAMMA1(I,K)*Rw(K,J)
            ENDDO
         ENDDO
      ENDDO
      DO I=1,NS
         DO J=1,NS
            GRG(I,J)=0.0D0
            DO K=1,NC
               GRG(I,J)=GRG(I,J)+GR(I,K)*GAMMA1(J,K)
            ENDDO
            P(I,J)=0.0D0
         ENDDO
      ENDDO

      ITER=0
      DO WHILE (dPmax .GT. ERR .AND. ITER .LT. MAXITER)
         ITER=ITER+1

C        ! M = F P Ft - G Rw Gt
         DO I=1,NS
            DO J=1,NS
               FP(I,J)=0.0D0
               DO K=1,NS
                  FP(I,J)=FP(I,J)+PHI(I,K)*P(K,J)
               ENDDO
            ENDDO
         ENDDO     
         DO I=1,NS
            DO J=1,NS
               FPF(I,J)=0.0D0
               DO K=1,NS
                  FPF(I,J)=FPF(I,J)+FP(I,K)*PHI(J,K)
               ENDDO
            ENDDO
         ENDDO   
         DO I=1,NS
            DO J=1,NS
               M(I,J)=FPF(I,J)+GRG(I,J)
            ENDDO
         ENDDO

C        ! L = M Ht (H M Ht + Rv)^-1 
         DO I=1,NM
            DO J=1,NS
               HM(I,J)=0.0D0
               DO K=1,NS
                  HM(I,J)=HM(I,J)+H(I,K)*M(K,J)
               ENDDO
            ENDDO
         ENDDO
         DO I=1,NM
            DO J=1,NM
               HMH(I,J)=0.0D0
               DO K=1,NS
                  HMH(I,J)=HMH(I,J)+HM(I,K)*H(J,K)
               ENDDO
            ENDDO
         ENDDO    
         DO I=1,NM
            DO J=1,NM
               HMHR(I,J)=HMH(I,J)+Rv(I,J)
            ENDDO
         ENDDO
         CALL MINVG(HMHR,HMHRI,NM)
         DO I=1,NS
            DO J=1,NM
               L(I,J)=0.0D0
               DO K=1,NM
                  L(I,J)=L(I,J)+HM(K,I)*HMHRI(K,J)
               ENDDO
            ENDDO
         ENDDO

C        ! P = M - L H M
         DO I=1,NS
            DO J=1,NS
               LHM(I,J)=0.0D0
               DO K=1,NM
                  LHM(I,J)=LHM(I,J)+L(I,K)*HM(K,J)
               ENDDO
            ENDDO
         ENDDO
         DO I=1,NS
            DO J=1,NS
               P(I,J)=M(I,J)-LHM(I,J)
            ENDDO
         ENDDO

C        ! Check convergence
         dPmax=0.0D0
         DO I=1,NS
            DO J=1,NS
               dP=DABS(P(I,J)-OldP(I,J))
               IF (dP .GT. dPmax) dPmax=dP
               OldP(I,J)=P(I,J)
            ENDDO
         ENDDO

      ENDDO  ! End Do While loop

      RETURN
      END
      
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C  The following subroutines implement a U-D estimator, following Bierman 
C  (Bierman, Gerald J.:  Factorization Methods for Discrete Sequential 
C  Estimation, Academic Press, 1977.
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C  Bierman, p. 53
C
C  Matrix P is input.  Output U is upper triangular, with elements of
C  D stored on its diagonal.  (Bierman is big on saving space.)

      SUBROUTINE UDUFactor(P,U,N)

      IMPLICIT NONE
      INTEGER*4 N
      REAL*8 P(N,N),U(N,N)
      REAL*8 alpha,beta
      INTEGER*4 i,j,k
     
      do j=N,2,-1
         U(j,j)=P(j,j)
         alpha=1.0d0/U(j,j)
         do k=1,j-1
            beta=P(k,j)
            U(k,j)=alpha*beta
            do i=1,k
               P(i,k)=P(i,k)-beta*U(i,j)
            enddo
         enddo
      enddo
      U(1,1)=P(1,1)

      RETURN
      END

CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C   Bierman, p. 100-101
C
C   x = State vector (in/out)
C   U = Covariance matrix (in/out)
C   z = Measurement scalar (in), scaled residual (out)  
C   a = Observation vector (in), gets over-written
C   Rv = Measurement noise variance (in)
C
C   x = x + K*(z-a*x)


      SUBROUTINE UDMeasUpdate(x,U,z,a,Rv)

      IMPLICIT NONE
      INTEGER*4 NS
      PARAMETER(NS=6)  ! Number of states
      REAL*8 x(NS),U(NS,NS),z,a(NS),Rv
      REAL*8 b(NS),alpha,beta,gamma,lambda
      INTEGER*4 i,j,k

      do j=1,NS
         z=z-a(j)*x(j)
      enddo

      do j=NS,2,-1
         do k=1,j-1
            a(j)=a(j)+U(k,j)*a(k)
         enddo
         b(j)=U(j,j)*a(j)
      enddo
      b(1)=U(1,1)*a(1)

      alpha = Rv+b(1)*a(1)
      gamma=1.0d0/alpha
      U(1,1)=Rv*gamma*U(1,1)

      do j=2,NS
         beta=alpha
         alpha=alpha+b(j)*a(j)
         lambda=-a(j)*gamma
         gamma=1.0d0/alpha
         U(j,j)=beta*gamma*U(j,j)
         do i=1,j-1
            beta=U(i,j)
            U(i,j)=beta+b(i)*lambda
            b(i)=b(i)+b(j)*beta
         enddo
      enddo
      z=z*gamma

      do j=1,NS
         x(j)=x(j)+b(j)*z
      enddo

      RETURN
      END

CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C  Bierman, p. 132-133
C
C  x(k+1)=phi*x(k)
C  U(k+1)=Something messy

      SUBROUTINE UDTimeUpdate(x,U,phi,gam,y,Rw)

      IMPLICIT NONE
      INTEGER*4 NS,Nw,Naug
      PARAMETER(NS=6,Nw=3,Naug=NS+Nw)
      REAL*8 x(NS),U(NS,NS),Uaug(Naug,Naug),phi(NS,NS),Rw(Nw)
      REAL*8 gam(NS,Nw)
      REAL*8 D(Naug),sigma,v(Naug),a(Naug),Dinv,y(Nw),Gy(NS)
      INTEGER*4 i,j,k,Jm1

      do i=1,NS
         do j=1,NS
            Uaug(i,j)=U(i,j)
         enddo
      enddo
      do i=1,Nw
         D(NS+i)=Rw(i)
         Uaug(i,NS+i)=gam(i,i)
      enddo

      v(1)=0.0d0
      do j=NS,2,-1
         do i=1,j
            D(i)=Uaug(i,j)
         enddo
         do i=1,NS
            Uaug(i,j)=phi(i,j)
            do k=1,j-1
               Uaug(i,j)=Uaug(i,j)+phi(i,k)*D(k)
            enddo
         enddo
      enddo
      D(1)=Uaug(1,1)

      do j=1,NS
         v(j)=0.0d0
         do k=1,NS
            v(j)=v(j)+phi(j,k)*x(k)
         enddo
         Gy(j)=0.0d0
         do k=1,Nw
            Gy(j)=Gy(j)+gam(j,k)*y(k)
         enddo
      enddo

      do j=1,NS
         x(j)=v(j)+Gy(j)
         Uaug(j,1)=phi(j,1)
      enddo

      do j=NS,1,-1
         sigma=0.0d0
         do k=1,Naug
            v(k)=Uaug(j,k)
            a(k)=D(k)*v(k)
            sigma=sigma+v(k)*a(k)
         enddo
         Uaug(j,j)=sigma
         if (j.ne.1) then
            Dinv=1.0d0/sigma
            Jm1=j-1
            do k=1,Jm1
               sigma=0.0d0
               do i=1,Naug
                  sigma=sigma+Uaug(k,i)*a(i)
               enddo
               sigma=sigma*Dinv
               do i=1,Naug
                  Uaug(k,i)=Uaug(k,i)-sigma*v(i)
               enddo
               Uaug(j,k)=sigma
            enddo
         endif
      enddo

      do j=2,NS
         do i=1,j-1
            U(i,j)=Uaug(j,i)
            U(j,i)=0.0d0
         enddo
      enddo

      RETURN
      END

CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C  Kalman Filter Measurement Update.
C
C  x  = State
C  P  = Covariance Matrix
C  y  = Scalar Measurement
C  a  = Observation Vector (y=a*x)
C  Rv = Measurement Noise Covariance

      SUBROUTINE KFMeasUpdate(x,P,y,a,Rv)

      IMPLICIT NONE
      INTEGER*4 NS
      PARAMETER(NS=6)
      REAL*8 x(NS),P(NS,NS),a(NS),y,Rv
      REAL*8 z,Pa(NS),aPa,L(NS)
      INTEGER*4 i,j

C ... Form residual
      z=y
      do i=1,NS
         z=z-a(i)*x(i)
      enddo

C ... Some preliminary matrix multiplies
      do i=1,NS
         Pa(i)=0.0d0
         do j=1,NS
            Pa(i)=Pa(i)+P(i,j)*a(j)
         enddo
      enddo
      aPa=0.0d0
      do i=1,NS
         aPa=aPa+a(i)*Pa(i)
      enddo

C ... L = Kalman Gain vector
      do i=1,NS
         L(i)=Pa(i)/(aPa+Rv)
      enddo

C ... Update State Estimate
      do i=1,NS
         x(i)=x(i)+L(i)*z
      enddo

C ... Update Covariance Matrix
      do i=1,NS
         do j=1,NS
            P(i,j)=P(i,j)-L(i)*Pa(j)
         enddo
      enddo

      RETURN
      END

CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C  Kalman Filter Time Update (Propagation)
C
C  x(k+1)=phi*x(k)+gam*u(k)
C  P(k+1)=phi*P(k)*phi'+gam*Rw*gam'
C
      SUBROUTINE KFTimeUpdate(x,P,phi,gam,u,Rw)

      IMPLICIT NONE
      INTEGER*4 NS,Nw
      PARAMETER(NS=6,Nw=3)
      REAL*8 x(NS),P(NS,NS),phi(NS,NS),gam(NS,Nw),u(Nw),Rw(Nw,Nw)
      REAL*8 Fx(NS),Gu(NS),FP(NS,NS),GR(NS,Nw),FPF(NS,NS),GRG(NS,NS)
      INTEGER*4 i,j,k

C ... Propagate State
      do i=1,NS
         Fx(i)=0.0d0
         do j=1,NS
            Fx(i)=Fx(i)+phi(i,j)*x(j)
         enddo
         Gu(i)=0.0d0
         do j=1,Nw
            Gu(i)=Gu(i)+gam(i,j)*u(j)
         enddo
         x(i)=Fx(i)+Gu(i)
      enddo

C ... Propagate Covariance
      do i=1,NS
         do j=1,NS
            FP(i,j)=0.0d0
            do k=1,NS
               FP(i,j)=FP(i,j)+phi(i,k)*P(k,j)
            enddo
         enddo
         do j=1,Nw
            GR(i,j)=0.0d0
            do k=1,Nw
               GR(i,j)=GR(i,j)+gam(i,k)*Rw(k,j)
            enddo
         enddo
      enddo

      do i=1,NS
         do j=1,NS
            FPF(i,j)=0.0d0
            do k=1,NS
               FPF(i,j)=FPF(i,j)+FP(i,k)*phi(j,k)
            enddo
         enddo
         do j=1,NS
            GRG(i,j)=0.0d0
            do k=1,Nw
               GRG(i,j)=GRG(i,j)+GR(i,k)*gam(j,k)
            enddo
         enddo
      enddo

      do i=1,NS
         do j=1,NS
            P(i,j)=FPF(i,j)+GRG(i,j)
         enddo
      enddo

      RETURN
      END



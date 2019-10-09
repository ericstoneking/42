CCCCCCCCCCCCCCCCCCCCCCCC   MATH SUBROUTINES   CCCCCCCCCCCCCCCCCCCCCCCCCC
C  All subroutines here assume the right-to-left convention.

CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C  3x3 Matrix Product
      SUBROUTINE MxM (A,B,C)

      IMPLICIT NONE
      REAL*8 A(3,3),B(3,3),C(3,3)

      C(1,1)=A(1,1)*B(1,1)+A(1,2)*B(2,1)+A(1,3)*B(3,1)
      C(1,2)=A(1,1)*B(1,2)+A(1,2)*B(2,2)+A(1,3)*B(3,2)
      C(1,3)=A(1,1)*B(1,3)+A(1,2)*B(2,3)+A(1,3)*B(3,3)
      C(2,1)=A(2,1)*B(1,1)+A(2,2)*B(2,1)+A(2,3)*B(3,1)
      C(2,2)=A(2,1)*B(1,2)+A(2,2)*B(2,2)+A(2,3)*B(3,2)
      C(2,3)=A(2,1)*B(1,3)+A(2,2)*B(2,3)+A(2,3)*B(3,3)
      C(3,1)=A(3,1)*B(1,1)+A(3,2)*B(2,1)+A(3,3)*B(3,1)
      C(3,2)=A(3,1)*B(1,2)+A(3,2)*B(2,2)+A(3,3)*B(3,2)
      C(3,3)=A(3,1)*B(1,3)+A(3,2)*B(2,3)+A(3,3)*B(3,3)

      RETURN
      END

CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C  3x3 Matrix times Transpose of Matrix 
      SUBROUTINE MxMT (A,B,C)

      IMPLICIT NONE
      REAL*8 A(3,3),B(3,3),C(3,3)

      C(1,1)=A(1,1)*B(1,1)+A(1,2)*B(1,2)+A(1,3)*B(1,3)
      C(1,2)=A(1,1)*B(2,1)+A(1,2)*B(2,2)+A(1,3)*B(2,3)
      C(1,3)=A(1,1)*B(3,1)+A(1,2)*B(3,2)+A(1,3)*B(3,3)
      C(2,1)=A(2,1)*B(1,1)+A(2,2)*B(1,2)+A(2,3)*B(1,3)
      C(2,2)=A(2,1)*B(2,1)+A(2,2)*B(2,2)+A(2,3)*B(2,3)
      C(2,3)=A(2,1)*B(3,1)+A(2,2)*B(3,2)+A(2,3)*B(3,3)
      C(3,1)=A(3,1)*B(1,1)+A(3,2)*B(1,2)+A(3,3)*B(1,3)
      C(3,2)=A(3,1)*B(2,1)+A(3,2)*B(2,2)+A(3,3)*B(2,3)
      C(3,3)=A(3,1)*B(3,1)+A(3,2)*B(3,2)+A(3,3)*B(3,3)

      RETURN
      END

CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C  3x3 Transpose of Matrix times Matrix
      SUBROUTINE MTxM (A,B,C)

      IMPLICIT NONE
      REAL*8 A(3,3),B(3,3),C(3,3)

      C(1,1)=A(1,1)*B(1,1)+A(2,1)*B(2,1)+A(3,1)*B(3,1)
      C(1,2)=A(1,1)*B(1,2)+A(2,1)*B(2,2)+A(3,1)*B(3,2)
      C(1,3)=A(1,1)*B(1,3)+A(2,1)*B(2,3)+A(3,1)*B(3,3)
      C(2,1)=A(1,2)*B(1,1)+A(2,2)*B(2,1)+A(3,2)*B(3,1)
      C(2,2)=A(1,2)*B(1,2)+A(2,2)*B(2,2)+A(3,2)*B(3,2)
      C(2,3)=A(1,2)*B(1,3)+A(2,2)*B(2,3)+A(3,2)*B(3,3)
      C(3,1)=A(1,3)*B(1,1)+A(2,3)*B(2,1)+A(3,3)*B(3,1)
      C(3,2)=A(1,3)*B(1,2)+A(2,3)*B(2,2)+A(3,3)*B(3,2)
      C(3,3)=A(1,3)*B(1,3)+A(2,3)*B(2,3)+A(3,3)*B(3,3)

      RETURN
      END

CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C  1x3 Vector times 3x3 Matrix
      SUBROUTINE VxM (V,M,W)

      IMPLICIT NONE
      REAL*8 V(3),M(3,3),W(3)
      
      W(1)=V(1)*M(1,1)+V(2)*M(2,1)+V(3)*M(3,1)
      W(2)=V(1)*M(1,2)+V(2)*M(2,2)+V(3)*M(3,2)
      W(3)=V(1)*M(1,3)+V(2)*M(2,3)+V(3)*M(3,3)

      RETURN
      END

CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C  3x3 Matrix times 3x1 Vector
      SUBROUTINE MxV (M,V,W)

      IMPLICIT NONE
      REAL*8 V(3),M(3,3),W(3)
      
      W(1)=V(1)*M(1,1)+V(2)*M(1,2)+V(3)*M(1,3)
      W(2)=V(1)*M(2,1)+V(2)*M(2,2)+V(3)*M(2,3)
      W(3)=V(1)*M(3,1)+V(2)*M(3,2)+V(3)*M(3,3)

      RETURN
      END

CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C  1x3 Vector times transpose of 3x3 Matrix
      SUBROUTINE VxMT (V,M,W)

      IMPLICIT NONE
      REAL*8 V(3),M(3,3),W(3)
      
      W(1)=V(1)*M(1,1)+V(2)*M(1,2)+V(3)*M(1,3)
      W(2)=V(1)*M(2,1)+V(2)*M(2,2)+V(3)*M(2,3)
      W(3)=V(1)*M(3,1)+V(2)*M(3,2)+V(3)*M(3,3)

      RETURN
      END

CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C  Transpose of 3x3 Matrix times 3x1 Vector
      SUBROUTINE MTxV (M,V,W)

      IMPLICIT NONE
      REAL*8 V(3),M(3,3),W(3)
      
      W(1) = M(1,1)*V(1) + M(2,1)*V(2) + M(3,1)*V(3)
      W(2) = M(1,2)*V(1) + M(2,2)*V(2) + M(3,2)*V(3)
      W(3) = M(1,3)*V(1) + M(2,3)*V(2) + M(3,3)*V(3)

      RETURN
      END

CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C  Scalar times 3x1 Vector
      SUBROUTINE SxV(S,V,W)

      IMPLICIT NONE
      REAL*8 S,V(3),W(3)

      W(1) = S*V(1)
      W(2) = S*V(2)
      W(3) = S*V(3)

      RETURN
      END

CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C  Scalar times 3x3 Matrix
      SUBROUTINE SxM(S,A,B)

      IMPLICIT NONE
      REAL*8 S,A(3,3),B(3,3)

      B(1,1) = S*A(1,1)
      B(1,2) = S*A(1,2)
      B(1,3) = S*A(1,3)
      B(2,1) = S*A(2,1)
      B(2,2) = S*A(2,2)
      B(2,3) = S*A(2,3)
      B(3,1) = S*A(3,1)
      B(3,2) = S*A(3,2)
      B(3,3) = S*A(3,3)

      RETURN
      END

CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C  Inverse of a 3x3 Matrix
      SUBROUTINE MINV(A,B)

      IMPLICIT NONE
      REAL*8 A(3,3),B(3,3),DET
   
      DET=A(1,1)*A(2,2)*A(3,3)+A(1,2)*A(2,3)*A(3,1)
     &   +A(1,3)*A(2,1)*A(3,2)-A(3,1)*A(2,2)*A(1,3)
     &   -A(3,2)*A(2,3)*A(1,1)-A(3,3)*A(2,1)*A(1,2)
   
      IF (DET.EQ.0.0D0) then
         write (*,*) 'Cannot invert singular matrix'
         STOP
      ENDIF

      B(1,1)=(A(2,2)*A(3,3)-A(3,2)*A(2,3))/DET 
      B(1,2)=(A(3,2)*A(1,3)-A(1,2)*A(3,3))/DET
      B(1,3)=(A(1,2)*A(2,3)-A(2,2)*A(1,3))/DET

      B(2,1)=(A(3,1)*A(2,3)-A(2,1)*A(3,3))/DET
      B(2,2)=(A(1,1)*A(3,3)-A(3,1)*A(1,3))/DET
      B(2,3)=(A(2,1)*A(1,3)-A(1,1)*A(2,3))/DET

      B(3,1)=(A(2,1)*A(3,2)-A(3,1)*A(2,2))/DET 
      B(3,2)=(A(3,1)*A(1,2)-A(1,1)*A(3,2))/DET
      B(3,3)=(A(1,1)*A(2,2)-A(2,1)*A(1,2))/DET

      RETURN    
      END

CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C  Pseudo-inverse of a 4x3 matrix
      SUBROUTINE PINV4x3(A,Aplus)

      IMPLICIT NONE
      REAL*8 A(4,3),Aplus(3,4),AtA(3,3),AtAi(3,3)

      AtA(1,1)=A(1,1)*A(1,1)+A(2,1)*A(2,1)+A(3,1)*A(3,1)+A(4,1)*A(4,1)
      AtA(1,2)=A(1,1)*A(1,2)+A(2,1)*A(2,2)+A(3,1)*A(3,2)+A(4,1)*A(4,2)
      AtA(1,3)=A(1,1)*A(1,3)+A(2,1)*A(2,3)+A(3,1)*A(3,3)+A(4,1)*A(4,3)
      AtA(2,1)=A(1,2)*A(1,1)+A(2,2)*A(2,1)+A(3,2)*A(3,1)+A(4,2)*A(4,1)
      AtA(2,2)=A(1,2)*A(1,2)+A(2,2)*A(2,2)+A(3,2)*A(3,2)+A(4,2)*A(4,2)
      AtA(2,3)=A(1,2)*A(1,3)+A(2,2)*A(2,3)+A(3,2)*A(3,3)+A(4,2)*A(4,3)
      AtA(3,1)=A(1,3)*A(1,1)+A(2,3)*A(2,1)+A(3,3)*A(3,1)+A(4,3)*A(4,1)
      AtA(3,2)=A(1,3)*A(1,2)+A(2,3)*A(2,2)+A(3,3)*A(3,2)+A(4,3)*A(4,2)
      AtA(3,3)=A(1,3)*A(1,3)+A(2,3)*A(2,3)+A(3,3)*A(3,3)+A(4,3)*A(4,3)

      CALL MINV(AtA,AtAi)

      Aplus(1,1)=AtAi(1,1)*A(1,1)+AtAi(1,2)*A(1,2)+AtAi(1,3)*A(1,3)
      Aplus(1,2)=AtAi(1,1)*A(2,1)+AtAi(1,2)*A(2,2)+AtAi(1,3)*A(2,3)
      Aplus(1,3)=AtAi(1,1)*A(3,1)+AtAi(1,2)*A(3,2)+AtAi(1,3)*A(3,3)
      Aplus(1,4)=AtAi(1,1)*A(4,1)+AtAi(1,2)*A(4,2)+AtAi(1,3)*A(4,3)
      Aplus(2,1)=AtAi(2,1)*A(1,1)+AtAi(2,2)*A(1,2)+AtAi(2,3)*A(1,3)
      Aplus(2,2)=AtAi(2,1)*A(2,1)+AtAi(2,2)*A(2,2)+AtAi(2,3)*A(2,3)
      Aplus(2,3)=AtAi(2,1)*A(3,1)+AtAi(2,2)*A(3,2)+AtAi(2,3)*A(3,3)
      Aplus(2,4)=AtAi(2,1)*A(4,1)+AtAi(2,2)*A(4,2)+AtAi(2,3)*A(4,3)
      Aplus(3,1)=AtAi(3,1)*A(1,1)+AtAi(3,2)*A(1,2)+AtAi(3,3)*A(1,3)
      Aplus(3,2)=AtAi(3,1)*A(2,1)+AtAi(3,2)*A(2,2)+AtAi(3,3)*A(2,3)
      Aplus(3,3)=AtAi(3,1)*A(3,1)+AtAi(3,2)*A(3,2)+AtAi(3,3)*A(3,3)
      Aplus(3,4)=AtAi(3,1)*A(4,1)+AtAi(3,2)*A(4,2)+AtAi(3,3)*A(4,3)

      RETURN
      END

CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C  Transpose of a 3x3 Matrix
      SUBROUTINE MT(A,B)

      IMPLICIT NONE
      REAL*8 A(3,3),B(3,3)

      B(1,1)=A(1,1)
      B(1,2)=A(2,1)
      B(1,3)=A(3,1)
      B(2,1)=A(1,2)
      B(2,2)=A(2,2)
      B(2,3)=A(3,2)
      B(3,1)=A(1,3)
      B(3,2)=A(2,3)
      B(3,3)=A(3,3)
      
      RETURN
      END

CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C  Vector Dot Product
      SUBROUTINE VoV(A,B,D)

      IMPLICIT NONE
      REAL*8 A(3),B(3),D

      D=A(1)*B(1)+A(2)*B(2)+A(3)*B(3)

      RETURN
      END

CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C  Vector Cross Product
      SUBROUTINE VxV(A,B,C)

      IMPLICIT NONE
      REAL*8 A(3),B(3),C(3)

      C(1)=A(2)*B(3)-A(3)*B(2)
      C(2)=A(3)*B(1)-A(1)*B(3)
      C(3)=A(1)*B(2)-A(2)*B(1)

      RETURN
      END

CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C  Vector cross Matrix dot Vector
      SUBROUTINE vxMov(w,M,wxMow)

      IMPLICIT NONE
      REAL*8 w(3),M(3,3),Mow(3),wxMow(3)
   
      Mow(1)=M(1,1)*w(1)+M(1,2)*w(2)+M(1,3)*w(3)
      Mow(2)=M(2,1)*w(1)+M(2,2)*w(2)+M(2,3)*w(3)
      Mow(3)=M(3,1)*w(1)+M(3,2)*w(2)+M(3,3)*w(3)

      wxMow(1)=w(2)*Mow(3)-w(3)*Mow(2)
      wxMow(2)=w(3)*Mow(1)-w(1)*Mow(3)
      wxMow(3)=w(1)*Mow(2)-w(2)*Mow(1)

      RETURN
      END 

CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C  Magnitude of a 3-vector
      SUBROUTINE MAGV(V,A)

      IMPLICIT NONE
      REAL*8 V(3),A

      A=DSQRT(V(1)*V(1)+V(2)*V(2)+V(3)*V(3))

      RETURN
      END

CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C  Normalize a 3-vector
      SUBROUTINE UNITV(V)
      
      IMPLICIT NONE
      REAL*8 V(3),A

      A=DSQRT(V(1)*V(1)+V(2)*V(2)+V(3)*V(3))
      V(1)=V(1)/A
      V(2)=V(2)/A
      V(3)=V(3)/A

      RETURN
      END

CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C  Quaternion product
      SUBROUTINE QxQ(A,B,C)

      IMPLICIT NONE
      REAL*8 A(4),B(4),C(4)

      C(1)= A(4)*B(1)+A(3)*B(2)-A(2)*B(3)+A(1)*B(4)
      C(2)=-A(3)*B(1)+A(4)*B(2)+A(1)*B(3)+A(2)*B(4)
      C(3)= A(2)*B(1)-A(1)*B(2)+A(4)*B(3)+A(3)*B(4)
      C(4)=-A(1)*B(1)-A(2)*B(2)-A(3)*B(3)+A(4)*B(4)

      RETURN
      END

CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C  Product of the Complement of a Quaternion (A) with a Quaternion (B)
      SUBROUTINE QTxQ(A,B,C)

      IMPLICIT NONE
      REAL*8 A(4),B(4),C(4)

      C(1)= A(4)*B(1)-A(3)*B(2)+A(2)*B(3)-A(1)*B(4)
      C(2)= A(3)*B(1)+A(4)*B(2)-A(1)*B(3)-A(2)*B(4)
      C(3)=-A(2)*B(1)+A(1)*B(2)+A(4)*B(3)-A(3)*B(4)
      C(4)= A(1)*B(1)+A(2)*B(2)+A(3)*B(3)+A(4)*B(4)

      RETURN
      END

CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C  Product of a Quaternion (A) with the Complement of a Quaternion (B)
      SUBROUTINE QxQT(A,B,C)

      IMPLICIT NONE
      REAL*8 A(4),B(4),C(4)

      C(1)=-A(4)*B(1)-A(3)*B(2)+A(2)*B(3)+A(1)*B(4)
      C(2)= A(3)*B(1)-A(4)*B(2)-A(1)*B(3)+A(2)*B(4)
      C(3)=-A(2)*B(1)+A(1)*B(2)-A(4)*B(3)+A(3)*B(4)
      C(4)= A(1)*B(1)+A(2)*B(2)+A(3)*B(3)+A(4)*B(4)

      RETURN
      END

CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C  Normalize a quaternion
      SUBROUTINE UNITQ(Q)

      IMPLICIT NONE
      REAL*8 Q(4),A

      A=DSQRT(Q(1)*Q(1)+Q(2)*Q(2)+Q(3)*Q(3)+Q(4)*Q(4))
      Q(1)=Q(1)/A
      Q(2)=Q(2)/A
      Q(3)=Q(3)/A
      Q(4)=Q(4)/A

      RETURN
      END

CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C  Rectify a quaternion, forcing q(4) to be positive
      SUBROUTINE RECTIFYQ(Q)

      IMPLICIT NONE
      REAL*8 Q(4)

      IF(Q(4) .LT. 0.0d0) THEN
         Q(1) = -Q(1)
         Q(2) = -Q(2)
         Q(3) = -Q(3)
         Q(4) = -Q(4)
      ENDIF

      RETURN
      END

CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C  General Matrix Product
C  Multiply an RxS matrix by an SxT matrix, yielding an RxT matrix
      SUBROUTINE MxMG (A,B,C,R,S,T)

      IMPLICIT NONE
      INTEGER*4 I,J,K,R,S,T
      REAL*8 A(R,S),B(S,T),C(R,T)

      DO 1 I=1,R
         DO 2 J=1,T
            C(I,J)=0.0D0
            DO 3 K=1,S
               C(I,J)=C(I,J)+A(I,K)*B(K,J)
 3          CONTINUE
 2       CONTINUE
 1    CONTINUE

      RETURN
      END

CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C  General product of a matrix and the transpose of a matrix
C  Multiply an RxS matrix by transpose of a TxS matrix, yielding an 
C  RxT matrix
      SUBROUTINE MxMTG (A,B,C,R,S,T)

      IMPLICIT NONE
      INTEGER*4 I,J,K,R,S,T
      REAL*8 A(R,S),B(T,S),C(R,T)

      DO 1 I=1,R
         DO 2 J=1,T
            C(I,J)=0.0D0
            DO 3 K=1,S
               C(I,J)=C(I,J)+A(I,K)*B(J,K)
 3          CONTINUE
 2       CONTINUE
 1    CONTINUE

      RETURN
      END

CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C  General product of a transposed matrix and another matrix
C  Multiply transpose of an SxR matrix by an SxT matrix, yielding an 
C  RxT matrix
      SUBROUTINE MTxMG (A,B,C,R,S,T)

      IMPLICIT NONE
      INTEGER*4 I,J,K,R,S,T
      REAL*8 A(S,R),B(S,T),C(R,T)

      DO 1 I=1,R
         DO 2 J=1,T
            C(I,J)=0.0D0
            DO 3 K=1,S
               C(I,J)=C(I,J)+A(K,I)*B(K,J)
 3          CONTINUE
 2       CONTINUE
 1    CONTINUE

      RETURN
      END

CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C  General matrix-vector product
      SUBROUTINE MxVG (M,V,W,R,S)

      IMPLICIT NONE
      INTEGER*4 I,J,R,S
      REAL*8 V(S),M(R,S),W(R)

      DO 1 I=1,R
         W(I)=0.0D0
         DO 2 J=1,S
            W(I)=W(I)+M(I,J)*V(J)
 2       CONTINUE
 1    CONTINUE

      RETURN
      END

CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C                   GENERAL MATRIX INVERSE
C
C  Inverse of an NxN matrix, N <= Nmax 

      SUBROUTINE MINVG(A,AI,N)

      IMPLICIT NONE
      INTEGER*4 Nmax,N,I,J,IPIVOT,ROW
      PARAMETER(Nmax=10)
      REAL*8 A(N,N),M(Nmax,Nmax),AI(N,N)
      REAL*8 PIVOT,K,TA(Nmax),TB(Nmax)

      DO I=1,N
         DO J=1,N
            M(I,J)=A(I,J)
         ENDDO
         AI(I,I)=1.0D0
      ENDDO

      DO ROW = 1,N
         PIVOT = 0.0D0
         DO I = ROW,N
            IF (DABS(M(I, ROW)) .GT. DABS(PIVOT)) THEN
               PIVOT = M(I, ROW)
               IPIVOT = I
            ENDIF
         ENDDO
         IF (PIVOT .EQ. 0.0D0) THEN
            WRITE(*,*) 'Matrix is singular'
            STOP
         ENDIF

         DO J = 1, N
            TA(J) = M(IPIVOT, J)
            TB(J) = AI(IPIVOT, J)
            M(IPIVOT, J) = M(ROW, J)
            AI(IPIVOT, J) = AI(ROW, J)
            M(ROW, J) = TA(J) / PIVOT
            AI(ROW, J) = TB(J) / PIVOT
         ENDDO
         DO I = ROW+1,N
            K = M(I, ROW)
            DO J = 1, N
               M(I, J) = M(I, J) - K * M(ROW, J)
               AI(I, J) = AI(I, J) - K * AI(ROW, J)
            ENDDO
         ENDDO
      ENDDO

C     A is now upper diagonal

      DO ROW = N,2,-1
         DO I = 1,ROW-1
            K = M(I, ROW)
            DO J = 1, N
               M(I, J) = M(I, J) - K * M(ROW, J)
               AI(I, J) = AI(I, J) - K * AI(ROW, J)
            ENDDO
         ENDDO
      ENDDO

      RETURN
      END

CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C  Solution of NxN system      A * x = b

      SUBROUTINE LINSOLVE(AA,x,bb,n)

      IMPLICIT NONE
      INTEGER*4 n,Nmax,i,j,k,l,m
      parameter(Nmax = 10)
      real*8 A(Nmax,Nmax),x(n),b(Nmax),mm,a1(Nmax),b1,AA(n,n),bb(n)

      do i=1,n
         do j=1,n
            A(i,j)=AA(i,j)
         enddo
         b(i) = bb(i)
      enddo

      if (n .eq. 1) then
         x(1) = b(1)/A(1,1)
         RETURN
      endif

      do j=1,n-1
         mm = dabs(A(j,j))
         l=j
         do i=j+1,n
            if (dabs(A(i,j)) .ge. mm) then
               l=i
               mm=dabs(A(i,j))
            endif
         enddo
         if (l .ne. j) then
            do i=1,n
               a1(i)=A(j,i)
            enddo
            b1=b(j)
            do i=j,n
               A(j,i)=A(l,i)/A(l,j)
            enddo
            b(j)=b(l)/A(l,j)
            do i=1,n
               A(l,i)=a1(i)
            enddo
            b(l)=b1
         else
            b(j)=b(j)/A(j,j)
            do i=n,j,-1
               A(j,i)=A(j,i)/A(j,j)
            enddo
         endif
         do k=j+1,n
            b(k)=b(k)-A(k,j)*b(j)
         enddo
         do k=j+1,n
            do m=n,j,-1
               A(k,m)=A(k,m)-A(k,j)*A(j,m)
            enddo
         enddo
      enddo
      x(n)=b(n)/A(n,n)
      do i=n-1,1,-1
         x(i)=b(i)
         do k=i+1,n
            x(i)=x(i)-A(i,k)*x(k)
         enddo
      enddo

      RETURN
      END

CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C   Linear interpolation of tabulated data
C   TBLX(n)  =  "X" column of table
C   TBLY(n)  =  "Y" column of table
C   X        =  Independent variable
C   Y        =  Interpolated result
C   N        =  Number of entries in table

      SUBROUTINE LINTERP(TBLX,TBLY,X,Y,N)

      IMPLICIT NONE
      INTEGER*4 N,I
      REAL*8 TBLX(N),TBLY(N),X,Y,DX,DXN

      DX = X - TBLX(1)
      DXN = TBLX(N) - TBLX(1)
      IF (DABS(DXN) .LT. DABS(DX) .OR. DX*DXN .LT. 0.0D0) THEN
         WRITE(*,*) 'X outside range of interpolation table'
         WRITE(*,*) 'TBLX(1) = ',TBLX(1)
         WRITE(*,*) 'TBLX(N) = ',TBLX(N)
         WRITE(*,*) 'X = ',X
         STOP
      ENDIF

      I=2
 10   CONTINUE
      IF (DABS(DX) .GT. DABS(TBLX(I)-TBLX(1))) THEN
         I=I+1
         GO TO 10
      ELSE   
         Y=(TBLY(I)-TBLY(I-1))/(TBLX(I)-TBLX(I-1))*(X-TBLX(I-1))
     &     +TBLY(I-1)
      ENDIF
      
      RETURN
      END

CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C   Given an array with Nrow rows and Ncol columns, sorts rows in ascending
C   order based on the elements of the SortCol'th column.
C   Based on Numerical Recipes, p. 329.

      SUBROUTINE HEAPSORT(array,MaxRow,Nrow,Ncol,SortCol)

      IMPLICIT NONE
      INTEGER*4 MaxRow,Nrow,Ncol,SortCol,MaxCol
      PARAMETER(MaxCol=5)
      REAL*8 array(MaxRow,Ncol)
      INTEGER*4 i,ir,j,l,k
      REAL*8 rra(MaxCol)

      if (Nrow .lt. 2) return

      l=Nrow/2+1
      ir=Nrow

 10   continue
         if(l.gt.1) then
            l=l-1
            do k=1,Ncol
               rra(k)=array(l,k)
            enddo
         else
            do k=1,Ncol
               rra(k)=array(ir,k)
               array(ir,k)=array(1,k)
            enddo
            ir=ir-1
            if(ir.eq.1) then
               do k=1,Ncol
                  array(1,k)=rra(k)
               enddo
               return
            endif
         endif
         i=l
         j=l+l
 20      if(j.le.ir) then
            if(j.lt.ir) then
               if(array(j,SortCol).lt.array(j+1,SortCol)) j=j+1         
            endif
            if(rra(SortCol).lt.array(j,SortCol)) then
               do k=1,Ncol
                  array(i,k)=array(j,k)
               enddo
               i=j
               j=j+j
            else
               j=ir+1
            endif
         goto 20
         endif
         do k=1,Ncol
            array(i,k)=rra(k)
         enddo
      goto 10   

      END


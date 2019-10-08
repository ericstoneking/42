#include <stdlib.h>
#include <math.h>
#include <stdio.h>

/**********************************************************************/
double **CreateMatrix(long n, long m)
{
      double **A;
      long i;

      A = (double **) calloc(n,sizeof(double *));
      if (A == NULL) {
         printf("calloc failed in CreateMatrix.  Bailing out.\n");
         exit(1);
      }
      for(i=0;i<n;i++) {
         A[i] = (double *) calloc(m,sizeof(double));
         if (A[i] == NULL) {
            printf("calloc failed in CreateMatrix.  Bailing out.\n");
            exit(1);
         }
      }
      return(A);
}
/**********************************************************************/
void DestroyMatrix(double **A, long n)
{
      long i;
      for(i=0;i<n;i++) free(A[i]);
      free(A);
}
/**********************************************************************/
/*   Solution of NxN system      A * x = b                            */
/*   by Gaussian Elimination and Back Substitution, with pivoting     */
void LINSOLVE(double **A, double *x, double *b, long n)
{
      long i,j,k,l,m;
      double mm,*a1,b1;

      a1 = (double *) calloc(n,sizeof(double));

      if (n == 1) {
         x[0] = b[0]/A[0][0];
         return;
      }

      for(j=0;j<n-1;j++){
         mm = fabs(A[j][j]);
         l=j;
         for(i=j+1;i<n;i++){
            if (fabs(A[i][j]) >= mm){
               l=i;
               mm=fabs(A[i][j]);
            }
         }
         if (l != j){
            for(i=0;i<n;i++){
               a1[i]=A[j][i];
            }
            b1=b[j];
            for(i=j;i<n;i++){
               A[j][i]=A[l][i]/A[l][j];
            }
            b[j]=b[l]/A[l][j];
            for(i=0;i<n;i++){
               A[l][i]=a1[i];
            }
            b[l]=b1;
         }
         else {
            b[j]=b[j]/A[j][j];
            for(i=n-1;i>=j;i--){
               A[j][i]=A[j][i]/A[j][j];
            }
         }
         for(k=j+1;k<n;k++){
            b[k]-=A[k][j]*b[j];
         }
         for(k=j+1;k<n;k++){
            for(m=n-1;m>=j;m--){
               A[k][m]-=A[k][j]*A[j][m];
            }
         }
      }
      x[n-1]=b[n-1]/A[n-1][n-1];
      for(i=n-2;i>=0;i--){
         x[i]=b[i];
         for(k=i+1;k<n;k++){
            x[i]-=A[i][k]*x[k];
         }
      }

      free(a1);
}
/******************************************************************************/
/* Compute Chebyshev polynomials of first kind (T) and second kind (U)        */
void ChebyPolys(double u, long n, double T[20], double U[20]) 
{
      long k;
      
      if (u < -1.0 || u > 1.0) {
         printf("u out of range in ChebPolys.  Bailing out.\n");
         exit(1);
      }
      if (n > 20) {
         printf("n out of range in ChebPolys.  Bailing out.\n");
         exit(1);
      }
      
      T[0] = 1.0;
      T[1] = u;
      U[0] = 1.0;
      U[1] = 2.0*u;
      for(k=1;k<n-1;k++) {
         T[k+1] = 2.0*u*T[k] - T[k-1];
         U[k+1] = 2.0*u*U[k] - U[k-1];
      }
}
/******************************************************************************/
/* Using ChebyPolys, find "position" (P) and scaled velocity (dPdu)           */
void ChebyInterp(double T[20], double U[20], double Coef[20], long n, 
   double *P, double *dPdu)
{
      long k;
      
      if (n > 20) {
         printf("n out of range in ChebyInterp.  Bailing out.\n");
         exit(1);
      }
      
      *P = Coef[0]*T[0];
      *dPdu = 0.0;
      for(k=1;k<n;k++) {
         *P += Coef[k]*T[k];
         *dPdu += Coef[k]*((double) k)*U[k-1];
      }
}
/******************************************************************************/
void FindChebyCoefs(double *u, double *P, long Nu, long Nc, double Coef[20])
{
      long i,j,k;
      double T[20],U[20];
      double **AtA, *x, *Atb;
      
      if (Nc > 20) {
         printf("Nc out of range in FindChebyCoefs.  Bailing out.\n");
         exit(1);
      }
      
      AtA = CreateMatrix(Nc,Nc);
      x = (double *) calloc(Nc,sizeof(double));
      Atb = (double *) calloc(Nc,sizeof(double));
      
      for(k=0;k<Nu;k++) {
         ChebyPolys(u[k],Nc,T,U);
         for(i=0;i<Nc;i++) {
            for(j=0;j<Nc;j++) {
               AtA[i][j] += T[i]*T[j];
            }
            Atb[i] += T[i]*P[k];
         }
      }
      LINSOLVE(AtA,x,Atb,Nc);
      for(i=0;i<Nc;i++) Coef[i] = x[i];
      for(i=Nc;i<20;i++) Coef[i] = 0.0;
      
      DestroyMatrix(AtA,Nc);
      free(x);
      free(Atb);
}
/******************************************************************************/
int main(int argc, char **argv)
{
      double Coef[20],T[20],U[20];
      double *u,*P,Pf,dPduf;
      long Nu,Nc;
      long i;
      FILE *outfile;

      /* Generate data */
      Nu = 100;
      u = (double *) calloc(Nu,sizeof(double));
      P = (double *) calloc(Nu,sizeof(double));
      for(i=0;i<Nu;i++) {
         u[i] = 2.0*((double) i)/((double) Nu) - 1.0;  /* [-1:1] */
         P[i] = 0.5 - 2.0*u[i] + 3.0*u[i]*u[i] - 0.3*u[i]*u[i]*u[i];
      }

      /* Fit coefficients */
      Nc = 6;
      FindChebyCoefs(u,P,Nu,Nc,Coef);
      for(i=0;i<Nc;i++) printf("C[%ld]: %lf  ",i,Coef[i]);
      printf("\n\n");
      
      /* Compare fit with data */
      outfile = fopen("ChebyData.txt","wt");
      for(i=0;i<Nu;i++) {
         ChebyPolys(u[i],Nc,T,U);
         ChebyInterp(T,U,Coef,Nc,&Pf,&dPduf);
         fprintf(outfile,"%lf %lf %lf %lf\n",u[i],P[i],Pf,dPduf);
      }
      fclose(outfile);
      
      return(0);
}      

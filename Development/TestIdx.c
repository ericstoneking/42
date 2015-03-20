#include <stdlib.h>
#include <stdio.h>

#define IDX2(x,y,Ny)             (x*Ny+y)
#define IDX3(x,y,z,Ny,Nz)       ((x*Ny+y)*Nz+z)
#define IDX4(x,y,z,w,Ny,Nz,Nw) (((x*Ny+y)*Nz+z)*Nw+w)

int main(int argc, char **argv)
{
      long i,j,k,m;
      long I,J,K;
      long Nx = 3, Ny = 3, Nz = 3;

      for(i=0;i<Nx;i++) {
         for(j=0;j<Ny;j++) {
            for(k=0;k<Nz;k++) {
                  m = IDX3(i,j,k,Ny,Nz);
                  I = m/(Ny*Nz);
                  J = (m/Nz)%Ny;
                  K = m%Nz;
                  //if (i!=I || j!=J || k!=K) {
                     printf("%ld %ld %ld   %ld   %ld %ld %ld\n",i,j,k,m,I,J,K);
                  //}
               }
            }
         }

      return(0);
}

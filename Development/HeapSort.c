#include <stdlib.h>
#include <stdio.h>

/**********************************************************************/
long CompareFunction(double a,  double b)
{
      if (a >= b) return(1);
      else return(-1);
}
/**********************************************************************/
void FormHeap(double *x, long L)
{
      long Done,k;
      double Temp;

      do {
         Done = 1;
         for(k=0;k<L/2;k++) {
            /* Check against left child (2*k+1) */
            if (CompareFunction(x[k],x[2*k+1]) < 0) {
               Temp = x[k];
               x[k] = x[2*k+1];
               x[2*k+1] = Temp;
               Done = 0;
            }
            /* Check against right child (2*k+2) */
            if (2*k+2 < L) { /* Right child exists */
               if (CompareFunction(x[k],x[2*k+2]) < 0) {
                  Temp = x[k];
                  x[k] = x[2*k+2];
                  x[2*k+2] = Temp;
                  Done = 0;
               }
            }
         }
      } while(!Done);
}
/**********************************************************************/
/* Sorts from smallest to largest */
void HeapSort(double *x, long N)
{

      long HeapLength;
      double Temp;

/* .. Form heap so each parent is greater than both children */
      FormHeap(x,N);

      HeapLength = N;
      while (HeapLength > 1) {
         /* Take largest element from top of heap */
         Temp = x[0];
         x[0] = x[HeapLength-1];
         x[HeapLength-1] = Temp;

         /* Shorten heap */
         HeapLength--;
         /* Re-form heap */
         FormHeap(x,HeapLength);
      }
}
/**********************************************************************/
int main(int argc, char **argv)
{
      long N = 9;
      double x[9] = {5,7,2,8,1,7,3,9,-4};
      long i;

      for(i=0;i<N;i++) printf("%3.1lf ",x[i]);
      printf("\n");

      HeapSort(x,N);

      for(i=0;i<N;i++) printf("%3.1lf ",x[i]);
      printf("\n");

      return(0);
}

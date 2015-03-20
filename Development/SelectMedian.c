#include <stdlib.h>
#include <stdio.h>

/**********************************************************************/
long Partition(long *Idx, long LowEnd, long HighEnd, double *Val)
{
      long PivotIdx,TempIdx;
      long LowIdx;
      double PivotVal;

      PivotIdx = HighEnd;
      PivotVal = Val[Idx[PivotIdx]];
      LowIdx = LowEnd;

      while(LowIdx < PivotIdx) {
         if (Val[Idx[LowIdx]] > PivotVal) {
            TempIdx = Idx[PivotIdx];
            Idx[PivotIdx] = Idx[LowIdx];
            Idx[LowIdx] = Idx[PivotIdx-1];
            Idx[PivotIdx-1] = TempIdx;
            PivotIdx--;
         }
         else LowIdx++;
      }

      return(PivotIdx);
}
/**********************************************************************/
/* Look up Quick Select                                               */
long SelectMedian(long *Idx, long N, double *Val)
{
      long LowEnd,HighEnd,PivotIdx;

      LowEnd = 0;
      HighEnd = N-1;

      do {
         PivotIdx = Partition(Idx,LowEnd,HighEnd,Val);
         if (PivotIdx > N/2) HighEnd = PivotIdx-1;
         else if (PivotIdx < N/2) LowEnd = PivotIdx+1;
      } while(PivotIdx != N/2);

      return(Idx[PivotIdx]);
}
/**********************************************************************/
int main(int argc, char **argv)
{
#define L 16
      double Val[L] = {20,18,16,14,12,10,8,6,7,9,11,13,15,17,19,21};
      long Idx[L] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
      double MedVal;
      long MedIdx;
      long i;

      MedIdx = SelectMedian(Idx,L,Val);
      MedVal = Val[MedIdx];

      printf("Idx: ");
      for(i=0;i<L;i++) printf("%ld ",Idx[i]);
      printf("\n");

      printf("Val: ");
      for(i=0;i<L;i++) printf("%5.1lf ",Val[Idx[i]]);
      printf("\n");

      printf("Median: %lf\n",MedVal);

      return(MedVal);
#undef L
}

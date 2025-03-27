#include "42.h"
#define EXTERN extern
#include "Ac.h"
#undef EXTERN

/******************************************************************************/
long FindInBufLen(struct AcType *AC)
{
      int BufLen = 0;
      int NumBytes;
      int k;
      int i;

      NumBytes = sizeof(double);
      for(k=0;k<AC->Ng;k++) {
         for(i=0;i<3;i++) {
            BufLen += NumBytes;
         }
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Ng;k++) {
         for(i=0;i<4;i++) {
            BufLen += NumBytes;
         }
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Ng;k++) {
         for(i=0;i<4;i++) {
            BufLen += NumBytes;
         }
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Ngyro;k++) {
         BufLen += NumBytes;
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Nmag;k++) {
         BufLen += NumBytes;
      }

      NumBytes = sizeof(long);
      for(k=0;k<AC->Ncss;k++) {
         BufLen += NumBytes;
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Ncss;k++) {
         BufLen += NumBytes;
      }

      NumBytes = sizeof(long);
      for(k=0;k<AC->Nfss;k++) {
         BufLen += NumBytes;
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Nfss;k++) {
         for(i=0;i<2;i++) {
            BufLen += NumBytes;
         }
      }

      NumBytes = sizeof(long);
      for(k=0;k<AC->Nst;k++) {
         BufLen += NumBytes;
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Nst;k++) {
         for(i=0;i<4;i++) {
            BufLen += NumBytes;
         }
      }

      NumBytes = sizeof(long);
      for(k=0;k<AC->Ngps;k++) {
         BufLen += NumBytes;
      }

      NumBytes = sizeof(long);
      for(k=0;k<AC->Ngps;k++) {
         BufLen += NumBytes;
      }

      NumBytes = sizeof(long);
      for(k=0;k<AC->Ngps;k++) {
         BufLen += NumBytes;
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Ngps;k++) {
         BufLen += NumBytes;
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Ngps;k++) {
         for(i=0;i<3;i++) {
            BufLen += NumBytes;
         }
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Ngps;k++) {
         for(i=0;i<3;i++) {
            BufLen += NumBytes;
         }
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Ngps;k++) {
         for(i=0;i<3;i++) {
            BufLen += NumBytes;
         }
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Ngps;k++) {
         for(i=0;i<3;i++) {
            BufLen += NumBytes;
         }
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Ngps;k++) {
         BufLen += NumBytes;
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Ngps;k++) {
         BufLen += NumBytes;
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Ngps;k++) {
         BufLen += NumBytes;
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Ngps;k++) {
         BufLen += NumBytes;
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Ngps;k++) {
         BufLen += NumBytes;
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Ngps;k++) {
         BufLen += NumBytes;
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Nacc;k++) {
         BufLen += NumBytes;
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Nwhl;k++) {
         BufLen += NumBytes;
      }

      NumBytes = sizeof(double);
      for(i=0;i<4;i++) {
         BufLen += NumBytes;
      }

      NumBytes = sizeof(double);
      for(i=0;i<4;i++) {
         BufLen += NumBytes;
      }

      BufLen += 1; /* Pad for null termination */

      return((long) BufLen);
}
/******************************************************************************/
long FindOutBufLen(struct AcType *AC)
{
      int BufLen = 0;
      int NumBytes;
      int k;
      int i;

      NumBytes = sizeof(double);
      for(i=0;i<3;i++) {
         BufLen += NumBytes;
      }

      NumBytes = sizeof(double);
      for(i=0;i<3;i++) {
         BufLen += NumBytes;
      }

      NumBytes = sizeof(double);
      for(i=0;i<3;i++) {
         BufLen += NumBytes;
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Ng;k++) {
         for(i=0;i<4;i++) {
            BufLen += NumBytes;
         }
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Ng;k++) {
         for(i=0;i<4;i++) {
            BufLen += NumBytes;
         }
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Ng;k++) {
         for(i=0;i<3;i++) {
            BufLen += NumBytes;
         }
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Ng;k++) {
         for(i=0;i<3;i++) {
            BufLen += NumBytes;
         }
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Ng;k++) {
         for(i=0;i<3;i++) {
            BufLen += NumBytes;
         }
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Ng;k++) {
         for(i=0;i<3;i++) {
            BufLen += NumBytes;
         }
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Nwhl;k++) {
         BufLen += NumBytes;
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Nmtb;k++) {
         BufLen += NumBytes;
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Nthr;k++) {
         BufLen += NumBytes;
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Nthr;k++) {
         BufLen += NumBytes;
      }

      NumBytes = sizeof(double);
      for(i=0;i<4;i++) {
         BufLen += NumBytes;
      }

      NumBytes = sizeof(double);
      for(i=0;i<4;i++) {
         BufLen += NumBytes;
      }

      NumBytes = sizeof(double);
      for(i=0;i<3;i++) {
         BufLen += NumBytes;
      }

      NumBytes = sizeof(double);
      for(i=0;i<3;i++) {
         BufLen += NumBytes;
      }

      NumBytes = sizeof(double);
      for(i=0;i<3;i++) {
         BufLen += NumBytes;
      }

      NumBytes = sizeof(double);
      for(i=0;i<3;i++) {
         BufLen += NumBytes;
      }

      BufLen += 1; /* Pad for null termination */

      return((long) BufLen);
}
/******************************************************************************/
long FindTblBufLen(struct AcType *AC)
{
      int BufLen = 0;
      int NumBytes;
      int k;
      int i;
      int j;

      NumBytes = sizeof(long);
      BufLen += NumBytes;

      NumBytes = sizeof(long);
      BufLen += NumBytes;

      NumBytes = sizeof(long);
      BufLen += NumBytes;

      NumBytes = sizeof(long);
      BufLen += NumBytes;

      NumBytes = sizeof(long);
      BufLen += NumBytes;

      NumBytes = sizeof(long);
      BufLen += NumBytes;

      NumBytes = sizeof(long);
      BufLen += NumBytes;

      NumBytes = sizeof(long);
      BufLen += NumBytes;

      NumBytes = sizeof(long);
      BufLen += NumBytes;

      NumBytes = sizeof(long);
      BufLen += NumBytes;

      NumBytes = sizeof(long);
      BufLen += NumBytes;

      NumBytes = sizeof(long);
      BufLen += NumBytes;

      NumBytes = sizeof(long);
      BufLen += NumBytes;

      NumBytes = sizeof(double);
      BufLen += NumBytes;

      NumBytes = sizeof(double);
      BufLen += NumBytes;

      NumBytes = sizeof(double);
      BufLen += NumBytes;

      NumBytes = sizeof(double);
      BufLen += NumBytes;

      NumBytes = sizeof(double);
      for(i=0;i<3;i++) {
         BufLen += NumBytes;
      }

      NumBytes = sizeof(double);
      for(i=0;i<3;i++) {
         for(j=0;j<3;j++) {
            BufLen += NumBytes;
         }
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Nb;k++) {
         BufLen += NumBytes;
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Nb;k++) {
         for(i=0;i<3;i++) {
            BufLen += NumBytes;
         }
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Nb;k++) {
         for(i=0;i<3;i++) {
            for(j=0;j<3;j++) {
               BufLen += NumBytes;
            }
         }
      }

      NumBytes = sizeof(long);
      for(k=0;k<AC->Ng;k++) {
         BufLen += NumBytes;
      }

      NumBytes = sizeof(long);
      for(k=0;k<AC->Ng;k++) {
         BufLen += NumBytes;
      }

      NumBytes = sizeof(long);
      for(k=0;k<AC->Ng;k++) {
         BufLen += NumBytes;
      }

      NumBytes = sizeof(long);
      for(k=0;k<AC->Ng;k++) {
         BufLen += NumBytes;
      }

      NumBytes = sizeof(long);
      for(k=0;k<AC->Ng;k++) {
         BufLen += NumBytes;
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Ng;k++) {
         for(i=0;i<3;i++) {
            for(j=0;j<3;j++) {
               BufLen += NumBytes;
            }
         }
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Ng;k++) {
         for(i=0;i<3;i++) {
            for(j=0;j<3;j++) {
               BufLen += NumBytes;
            }
         }
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Ng;k++) {
         for(i=0;i<3;i++) {
            BufLen += NumBytes;
         }
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Ng;k++) {
         for(i=0;i<3;i++) {
            BufLen += NumBytes;
         }
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Ng;k++) {
         for(i=0;i<3;i++) {
            BufLen += NumBytes;
         }
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Ng;k++) {
         for(i=0;i<3;i++) {
            BufLen += NumBytes;
         }
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Ng;k++) {
         for(i=0;i<3;i++) {
            BufLen += NumBytes;
         }
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Ng;k++) {
         for(i=0;i<3;i++) {
            BufLen += NumBytes;
         }
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Ng;k++) {
         for(i=0;i<3;i++) {
            BufLen += NumBytes;
         }
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Ng;k++) {
         for(i=0;i<3;i++) {
            BufLen += NumBytes;
         }
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Ngyro;k++) {
         for(i=0;i<3;i++) {
            BufLen += NumBytes;
         }
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Nmag;k++) {
         for(i=0;i<3;i++) {
            BufLen += NumBytes;
         }
      }

      NumBytes = sizeof(long);
      for(k=0;k<AC->Ncss;k++) {
         BufLen += NumBytes;
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Ncss;k++) {
         for(i=0;i<3;i++) {
            BufLen += NumBytes;
         }
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Ncss;k++) {
         BufLen += NumBytes;
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Nfss;k++) {
         for(i=0;i<4;i++) {
            BufLen += NumBytes;
         }
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Nfss;k++) {
         for(i=0;i<3;i++) {
            for(j=0;j<3;j++) {
               BufLen += NumBytes;
            }
         }
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Nst;k++) {
         for(i=0;i<4;i++) {
            BufLen += NumBytes;
         }
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Nst;k++) {
         for(i=0;i<3;i++) {
            for(j=0;j<3;j++) {
               BufLen += NumBytes;
            }
         }
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Nacc;k++) {
         for(i=0;i<3;i++) {
            BufLen += NumBytes;
         }
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Nacc;k++) {
         for(i=0;i<3;i++) {
            BufLen += NumBytes;
         }
      }

      NumBytes = sizeof(long);
      for(k=0;k<AC->Nwhl;k++) {
         BufLen += NumBytes;
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Nwhl;k++) {
         for(i=0;i<3;i++) {
            BufLen += NumBytes;
         }
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Nwhl;k++) {
         for(i=0;i<3;i++) {
            BufLen += NumBytes;
         }
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Nwhl;k++) {
         BufLen += NumBytes;
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Nwhl;k++) {
         BufLen += NumBytes;
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Nwhl;k++) {
         BufLen += NumBytes;
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Nmtb;k++) {
         for(i=0;i<3;i++) {
            BufLen += NumBytes;
         }
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Nmtb;k++) {
         for(i=0;i<3;i++) {
            BufLen += NumBytes;
         }
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Nmtb;k++) {
         BufLen += NumBytes;
      }

      NumBytes = sizeof(long);
      for(k=0;k<AC->Nthr;k++) {
         BufLen += NumBytes;
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Nthr;k++) {
         for(i=0;i<3;i++) {
            BufLen += NumBytes;
         }
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Nthr;k++) {
         for(i=0;i<3;i++) {
            BufLen += NumBytes;
         }
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Nthr;k++) {
         for(i=0;i<3;i++) {
            BufLen += NumBytes;
         }
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Nthr;k++) {
         BufLen += NumBytes;
      }

      NumBytes = sizeof(double);
      BufLen += NumBytes;

      NumBytes = sizeof(double);
      BufLen += NumBytes;

      NumBytes = sizeof(double);
      BufLen += NumBytes;

      NumBytes = sizeof(double);
      BufLen += NumBytes;

      NumBytes = sizeof(double);
      BufLen += NumBytes;

      NumBytes = sizeof(double);
      for(i=0;i<3;i++) {
         BufLen += NumBytes;
      }

      NumBytes = sizeof(double);
      for(i=0;i<3;i++) {
         BufLen += NumBytes;
      }

      NumBytes = sizeof(double);
      BufLen += NumBytes;

      NumBytes = sizeof(double);
      BufLen += NumBytes;

      NumBytes = sizeof(double);
      BufLen += NumBytes;

      NumBytes = sizeof(double);
      BufLen += NumBytes;

      NumBytes = sizeof(double);
      BufLen += NumBytes;

      NumBytes = sizeof(double);
      for(i=0;i<3;i++) {
         BufLen += NumBytes;
      }

      NumBytes = sizeof(double);
      for(i=0;i<3;i++) {
         BufLen += NumBytes;
      }

      NumBytes = sizeof(double);
      BufLen += NumBytes;

      NumBytes = sizeof(double);
      for(i=0;i<3;i++) {
         BufLen += NumBytes;
      }

      NumBytes = sizeof(double);
      for(i=0;i<3;i++) {
         BufLen += NumBytes;
      }

      NumBytes = sizeof(double);
      BufLen += NumBytes;

      NumBytes = sizeof(double);
      for(i=0;i<3;i++) {
         BufLen += NumBytes;
      }

      NumBytes = sizeof(double);
      for(i=0;i<3;i++) {
         BufLen += NumBytes;
      }

      NumBytes = sizeof(double);
      for(i=0;i<3;i++) {
         BufLen += NumBytes;
      }

      NumBytes = sizeof(double);
      for(i=0;i<3;i++) {
         BufLen += NumBytes;
      }

      NumBytes = sizeof(double);
      BufLen += NumBytes;

      NumBytes = sizeof(double);
      BufLen += NumBytes;

      NumBytes = sizeof(double);
      for(i=0;i<3;i++) {
         BufLen += NumBytes;
      }

      NumBytes = sizeof(double);
      for(i=0;i<3;i++) {
         BufLen += NumBytes;
      }

      NumBytes = sizeof(double);
      BufLen += NumBytes;

      NumBytes = sizeof(double);
      for(i=0;i<3;i++) {
         BufLen += NumBytes;
      }

      NumBytes = sizeof(double);
      for(i=0;i<3;i++) {
         BufLen += NumBytes;
      }

      BufLen += 1; /* Pad for null termination */

      return((long) BufLen);
}
/******************************************************************************/
void WriteAcInToSocket(struct AcType *AC,struct AcIpcType *I)
{
      int Success;
      int NumBytes;
      char *BufPtr;
      char Ack[4] = "Ack\0";
      int k;
      int i;

      BufPtr = &I->AcInBuf[0];

      NumBytes = sizeof(double);
      for(k=0;k<AC->Ng;k++) {
         for(i=0;i<3;i++) {
            memcpy(BufPtr,&AC->G[k].Ang[i],NumBytes);
            BufPtr += NumBytes;
         }
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Ng;k++) {
         for(i=0;i<4;i++) {
            memcpy(BufPtr,&AC->G[k].GCmd.qrl[i],NumBytes);
            BufPtr += NumBytes;
         }
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Ng;k++) {
         for(i=0;i<4;i++) {
            memcpy(BufPtr,&AC->G[k].GCmd.qrn[i],NumBytes);
            BufPtr += NumBytes;
         }
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Ngyro;k++) {
         memcpy(BufPtr,&AC->Gyro[k].Rate,NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Nmag;k++) {
         memcpy(BufPtr,&AC->MAG[k].Field,NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(long);
      for(k=0;k<AC->Ncss;k++) {
         memcpy(BufPtr,&AC->CSS[k].Valid,NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Ncss;k++) {
         memcpy(BufPtr,&AC->CSS[k].Illum,NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(long);
      for(k=0;k<AC->Nfss;k++) {
         memcpy(BufPtr,&AC->FSS[k].Valid,NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Nfss;k++) {
         for(i=0;i<2;i++) {
            memcpy(BufPtr,&AC->FSS[k].SunAng[i],NumBytes);
            BufPtr += NumBytes;
         }
      }

      NumBytes = sizeof(long);
      for(k=0;k<AC->Nst;k++) {
         memcpy(BufPtr,&AC->ST[k].Valid,NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Nst;k++) {
         for(i=0;i<4;i++) {
            memcpy(BufPtr,&AC->ST[k].qn[i],NumBytes);
            BufPtr += NumBytes;
         }
      }

      NumBytes = sizeof(long);
      for(k=0;k<AC->Ngps;k++) {
         memcpy(BufPtr,&AC->GPS[k].Valid,NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(long);
      for(k=0;k<AC->Ngps;k++) {
         memcpy(BufPtr,&AC->GPS[k].Rollover,NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(long);
      for(k=0;k<AC->Ngps;k++) {
         memcpy(BufPtr,&AC->GPS[k].Week,NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Ngps;k++) {
         memcpy(BufPtr,&AC->GPS[k].Sec,NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Ngps;k++) {
         for(i=0;i<3;i++) {
            memcpy(BufPtr,&AC->GPS[k].PosN[i],NumBytes);
            BufPtr += NumBytes;
         }
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Ngps;k++) {
         for(i=0;i<3;i++) {
            memcpy(BufPtr,&AC->GPS[k].VelN[i],NumBytes);
            BufPtr += NumBytes;
         }
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Ngps;k++) {
         for(i=0;i<3;i++) {
            memcpy(BufPtr,&AC->GPS[k].PosW[i],NumBytes);
            BufPtr += NumBytes;
         }
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Ngps;k++) {
         for(i=0;i<3;i++) {
            memcpy(BufPtr,&AC->GPS[k].VelW[i],NumBytes);
            BufPtr += NumBytes;
         }
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Ngps;k++) {
         memcpy(BufPtr,&AC->GPS[k].Lng,NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Ngps;k++) {
         memcpy(BufPtr,&AC->GPS[k].Lat,NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Ngps;k++) {
         memcpy(BufPtr,&AC->GPS[k].Alt,NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Ngps;k++) {
         memcpy(BufPtr,&AC->GPS[k].WgsLng,NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Ngps;k++) {
         memcpy(BufPtr,&AC->GPS[k].WgsLat,NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Ngps;k++) {
         memcpy(BufPtr,&AC->GPS[k].WgsAlt,NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Nacc;k++) {
         memcpy(BufPtr,&AC->Accel[k].Acc,NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Nwhl;k++) {
         memcpy(BufPtr,&AC->Whl[k].H,NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(double);
      for(i=0;i<4;i++) {
         memcpy(BufPtr,&AC->Cmd.qrl[i],NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(double);
      for(i=0;i<4;i++) {
         memcpy(BufPtr,&AC->Cmd.qrn[i],NumBytes);
         BufPtr += NumBytes;
      }

      memset(BufPtr,0,1);


      Success = write(I->Socket,I->AcInBuf,I->AcInBufLen);
      if (Success < 0) {
         printf("Error writing to socket in WriteAcInToSocket.\n");
         exit(1);
      }
      read(I->Socket,Ack,4);
}
/******************************************************************************/
void ReadAcOutFromSocket(struct AcType *AC,struct AcIpcType *I)
{
      int Success;
      int NumBytes;
      char *BufPtr;
      char Ack[4] = "Ack\0";
      int k;
      int i;

      BufPtr = &I->AcOutBuf[0];
      Success = read(I->Socket,I->AcOutBuf,I->AcOutBufLen);
      if (Success < 0) {
         printf("Error reading from socket in ReadAcOutFromSocket.\n");
         exit(1);
      }
      write(I->Socket,Ack,4);

      NumBytes = sizeof(double);
      for(i=0;i<3;i++) {
         memcpy(&AC->svb[i],BufPtr,NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(double);
      for(i=0;i<3;i++) {
         memcpy(&AC->bvb[i],BufPtr,NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(double);
      for(i=0;i<3;i++) {
         memcpy(&AC->Hvb[i],BufPtr,NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Ng;k++) {
         for(i=0;i<4;i++) {
            memcpy(&AC->G[k].GCmd.qrl[i],BufPtr,NumBytes);
            BufPtr += NumBytes;
         }
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Ng;k++) {
         for(i=0;i<4;i++) {
            memcpy(&AC->G[k].GCmd.qrn[i],BufPtr,NumBytes);
            BufPtr += NumBytes;
         }
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Ng;k++) {
         for(i=0;i<3;i++) {
            memcpy(&AC->G[k].GCmd.AngRate[i],BufPtr,NumBytes);
            BufPtr += NumBytes;
         }
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Ng;k++) {
         for(i=0;i<3;i++) {
            memcpy(&AC->G[k].GCmd.Ang[i],BufPtr,NumBytes);
            BufPtr += NumBytes;
         }
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Ng;k++) {
         for(i=0;i<3;i++) {
            memcpy(&AC->G[k].GCmd.PosRate[i],BufPtr,NumBytes);
            BufPtr += NumBytes;
         }
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Ng;k++) {
         for(i=0;i<3;i++) {
            memcpy(&AC->G[k].GCmd.Pos[i],BufPtr,NumBytes);
            BufPtr += NumBytes;
         }
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Nwhl;k++) {
         memcpy(&AC->Whl[k].Tcmd,BufPtr,NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Nmtb;k++) {
         memcpy(&AC->MTB[k].Mcmd,BufPtr,NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Nthr;k++) {
         memcpy(&AC->Thr[k].PulseWidthCmd,BufPtr,NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Nthr;k++) {
         memcpy(&AC->Thr[k].ThrustLevelCmd,BufPtr,NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(double);
      for(i=0;i<4;i++) {
         memcpy(&AC->Cmd.qrl[i],BufPtr,NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(double);
      for(i=0;i<4;i++) {
         memcpy(&AC->Cmd.qrn[i],BufPtr,NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(double);
      for(i=0;i<3;i++) {
         memcpy(&AC->Cmd.AngRate[i],BufPtr,NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(double);
      for(i=0;i<3;i++) {
         memcpy(&AC->Cmd.Ang[i],BufPtr,NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(double);
      for(i=0;i<3;i++) {
         memcpy(&AC->Cmd.PosRate[i],BufPtr,NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(double);
      for(i=0;i<3;i++) {
         memcpy(&AC->Cmd.Pos[i],BufPtr,NumBytes);
         BufPtr += NumBytes;
      }
}
/******************************************************************************/
void WriteAcTblToSocket(struct AcType *AC,struct AcIpcType *I)
{
      int Success;
      int NumBytes;
      char *BufPtr;
      char Ack[4] = "Ack\0";
      int k;
      int i;
      int j;

      BufPtr = &I->AcTblBuf[0];

      NumBytes = sizeof(long);
      memcpy(BufPtr,&AC->ID,NumBytes);
      BufPtr += NumBytes;

      NumBytes = sizeof(long);
      memcpy(BufPtr,&AC->Nb,NumBytes);
      BufPtr += NumBytes;

      NumBytes = sizeof(long);
      memcpy(BufPtr,&AC->Ng,NumBytes);
      BufPtr += NumBytes;

      NumBytes = sizeof(long);
      memcpy(BufPtr,&AC->Nwhl,NumBytes);
      BufPtr += NumBytes;

      NumBytes = sizeof(long);
      memcpy(BufPtr,&AC->Nmtb,NumBytes);
      BufPtr += NumBytes;

      NumBytes = sizeof(long);
      memcpy(BufPtr,&AC->Nthr,NumBytes);
      BufPtr += NumBytes;

      NumBytes = sizeof(long);
      memcpy(BufPtr,&AC->Ngyro,NumBytes);
      BufPtr += NumBytes;

      NumBytes = sizeof(long);
      memcpy(BufPtr,&AC->Nmag,NumBytes);
      BufPtr += NumBytes;

      NumBytes = sizeof(long);
      memcpy(BufPtr,&AC->Ncss,NumBytes);
      BufPtr += NumBytes;

      NumBytes = sizeof(long);
      memcpy(BufPtr,&AC->Nfss,NumBytes);
      BufPtr += NumBytes;

      NumBytes = sizeof(long);
      memcpy(BufPtr,&AC->Nst,NumBytes);
      BufPtr += NumBytes;

      NumBytes = sizeof(long);
      memcpy(BufPtr,&AC->Ngps,NumBytes);
      BufPtr += NumBytes;

      NumBytes = sizeof(long);
      memcpy(BufPtr,&AC->Nacc,NumBytes);
      BufPtr += NumBytes;

      NumBytes = sizeof(double);
      memcpy(BufPtr,&AC->Pi,NumBytes);
      BufPtr += NumBytes;

      NumBytes = sizeof(double);
      memcpy(BufPtr,&AC->TwoPi,NumBytes);
      BufPtr += NumBytes;

      NumBytes = sizeof(double);
      memcpy(BufPtr,&AC->DT,NumBytes);
      BufPtr += NumBytes;

      NumBytes = sizeof(double);
      memcpy(BufPtr,&AC->mass,NumBytes);
      BufPtr += NumBytes;

      NumBytes = sizeof(double);
      for(i=0;i<3;i++) {
         memcpy(BufPtr,&AC->cm[i],NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(double);
      for(i=0;i<3;i++) {
         for(j=0;j<3;j++) {
            memcpy(BufPtr,&AC->MOI[i][j],NumBytes);
            BufPtr += NumBytes;
         }
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Nb;k++) {
         memcpy(BufPtr,&AC->B[k].mass,NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Nb;k++) {
         for(i=0;i<3;i++) {
            memcpy(BufPtr,&AC->B[k].cm[i],NumBytes);
            BufPtr += NumBytes;
         }
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Nb;k++) {
         for(i=0;i<3;i++) {
            for(j=0;j<3;j++) {
               memcpy(BufPtr,&AC->B[k].MOI[i][j],NumBytes);
               BufPtr += NumBytes;
            }
         }
      }

      NumBytes = sizeof(long);
      for(k=0;k<AC->Ng;k++) {
         memcpy(BufPtr,&AC->G[k].IsSpherical,NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(long);
      for(k=0;k<AC->Ng;k++) {
         memcpy(BufPtr,&AC->G[k].RotDOF,NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(long);
      for(k=0;k<AC->Ng;k++) {
         memcpy(BufPtr,&AC->G[k].TrnDOF,NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(long);
      for(k=0;k<AC->Ng;k++) {
         memcpy(BufPtr,&AC->G[k].RotSeq,NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(long);
      for(k=0;k<AC->Ng;k++) {
         memcpy(BufPtr,&AC->G[k].TrnSeq,NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Ng;k++) {
         for(i=0;i<3;i++) {
            for(j=0;j<3;j++) {
               memcpy(BufPtr,&AC->G[k].CGiBi[i][j],NumBytes);
               BufPtr += NumBytes;
            }
         }
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Ng;k++) {
         for(i=0;i<3;i++) {
            for(j=0;j<3;j++) {
               memcpy(BufPtr,&AC->G[k].CBoGo[i][j],NumBytes);
               BufPtr += NumBytes;
            }
         }
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Ng;k++) {
         for(i=0;i<3;i++) {
            memcpy(BufPtr,&AC->G[k].AngGain[i],NumBytes);
            BufPtr += NumBytes;
         }
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Ng;k++) {
         for(i=0;i<3;i++) {
            memcpy(BufPtr,&AC->G[k].AngRateGain[i],NumBytes);
            BufPtr += NumBytes;
         }
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Ng;k++) {
         for(i=0;i<3;i++) {
            memcpy(BufPtr,&AC->G[k].PosGain[i],NumBytes);
            BufPtr += NumBytes;
         }
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Ng;k++) {
         for(i=0;i<3;i++) {
            memcpy(BufPtr,&AC->G[k].PosRateGain[i],NumBytes);
            BufPtr += NumBytes;
         }
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Ng;k++) {
         for(i=0;i<3;i++) {
            memcpy(BufPtr,&AC->G[k].MaxAngRate[i],NumBytes);
            BufPtr += NumBytes;
         }
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Ng;k++) {
         for(i=0;i<3;i++) {
            memcpy(BufPtr,&AC->G[k].MaxPosRate[i],NumBytes);
            BufPtr += NumBytes;
         }
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Ng;k++) {
         for(i=0;i<3;i++) {
            memcpy(BufPtr,&AC->G[k].MaxTrq[i],NumBytes);
            BufPtr += NumBytes;
         }
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Ng;k++) {
         for(i=0;i<3;i++) {
            memcpy(BufPtr,&AC->G[k].MaxFrc[i],NumBytes);
            BufPtr += NumBytes;
         }
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Ngyro;k++) {
         for(i=0;i<3;i++) {
            memcpy(BufPtr,&AC->Gyro[k].Axis[i],NumBytes);
            BufPtr += NumBytes;
         }
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Nmag;k++) {
         for(i=0;i<3;i++) {
            memcpy(BufPtr,&AC->MAG[k].Axis[i],NumBytes);
            BufPtr += NumBytes;
         }
      }

      NumBytes = sizeof(long);
      for(k=0;k<AC->Ncss;k++) {
         memcpy(BufPtr,&AC->CSS[k].Body,NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Ncss;k++) {
         for(i=0;i<3;i++) {
            memcpy(BufPtr,&AC->CSS[k].Axis[i],NumBytes);
            BufPtr += NumBytes;
         }
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Ncss;k++) {
         memcpy(BufPtr,&AC->CSS[k].Scale,NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Nfss;k++) {
         for(i=0;i<4;i++) {
            memcpy(BufPtr,&AC->FSS[k].qb[i],NumBytes);
            BufPtr += NumBytes;
         }
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Nfss;k++) {
         for(i=0;i<3;i++) {
            for(j=0;j<3;j++) {
               memcpy(BufPtr,&AC->FSS[k].CB[i][j],NumBytes);
               BufPtr += NumBytes;
            }
         }
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Nst;k++) {
         for(i=0;i<4;i++) {
            memcpy(BufPtr,&AC->ST[k].qb[i],NumBytes);
            BufPtr += NumBytes;
         }
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Nst;k++) {
         for(i=0;i<3;i++) {
            for(j=0;j<3;j++) {
               memcpy(BufPtr,&AC->ST[k].CB[i][j],NumBytes);
               BufPtr += NumBytes;
            }
         }
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Nacc;k++) {
         for(i=0;i<3;i++) {
            memcpy(BufPtr,&AC->Accel[k].PosB[i],NumBytes);
            BufPtr += NumBytes;
         }
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Nacc;k++) {
         for(i=0;i<3;i++) {
            memcpy(BufPtr,&AC->Accel[k].Axis[i],NumBytes);
            BufPtr += NumBytes;
         }
      }

      NumBytes = sizeof(long);
      for(k=0;k<AC->Nwhl;k++) {
         memcpy(BufPtr,&AC->Whl[k].Body,NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Nwhl;k++) {
         for(i=0;i<3;i++) {
            memcpy(BufPtr,&AC->Whl[k].Axis[i],NumBytes);
            BufPtr += NumBytes;
         }
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Nwhl;k++) {
         for(i=0;i<3;i++) {
            memcpy(BufPtr,&AC->Whl[k].DistVec[i],NumBytes);
            BufPtr += NumBytes;
         }
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Nwhl;k++) {
         memcpy(BufPtr,&AC->Whl[k].J,NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Nwhl;k++) {
         memcpy(BufPtr,&AC->Whl[k].Tmax,NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Nwhl;k++) {
         memcpy(BufPtr,&AC->Whl[k].Hmax,NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Nmtb;k++) {
         for(i=0;i<3;i++) {
            memcpy(BufPtr,&AC->MTB[k].Axis[i],NumBytes);
            BufPtr += NumBytes;
         }
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Nmtb;k++) {
         for(i=0;i<3;i++) {
            memcpy(BufPtr,&AC->MTB[k].DistVec[i],NumBytes);
            BufPtr += NumBytes;
         }
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Nmtb;k++) {
         memcpy(BufPtr,&AC->MTB[k].Mmax,NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(long);
      for(k=0;k<AC->Nthr;k++) {
         memcpy(BufPtr,&AC->Thr[k].Body,NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Nthr;k++) {
         for(i=0;i<3;i++) {
            memcpy(BufPtr,&AC->Thr[k].PosB[i],NumBytes);
            BufPtr += NumBytes;
         }
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Nthr;k++) {
         for(i=0;i<3;i++) {
            memcpy(BufPtr,&AC->Thr[k].Axis[i],NumBytes);
            BufPtr += NumBytes;
         }
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Nthr;k++) {
         for(i=0;i<3;i++) {
            memcpy(BufPtr,&AC->Thr[k].rxA[i],NumBytes);
            BufPtr += NumBytes;
         }
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Nthr;k++) {
         memcpy(BufPtr,&AC->Thr[k].Fmax,NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(double);
      memcpy(BufPtr,&AC->InstantCtrl.wc,NumBytes);
      BufPtr += NumBytes;

      NumBytes = sizeof(double);
      memcpy(BufPtr,&AC->InstantCtrl.amax,NumBytes);
      BufPtr += NumBytes;

      NumBytes = sizeof(double);
      memcpy(BufPtr,&AC->InstantCtrl.vmax,NumBytes);
      BufPtr += NumBytes;

      NumBytes = sizeof(double);
      memcpy(BufPtr,&AC->InstantCtrl.Kprec,NumBytes);
      BufPtr += NumBytes;

      NumBytes = sizeof(double);
      memcpy(BufPtr,&AC->InstantCtrl.Knute,NumBytes);
      BufPtr += NumBytes;

      NumBytes = sizeof(double);
      for(i=0;i<3;i++) {
         memcpy(BufPtr,&AC->SandboxCtrl.Kr[i],NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(double);
      for(i=0;i<3;i++) {
         memcpy(BufPtr,&AC->SandboxCtrl.Kp[i],NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(double);
      memcpy(BufPtr,&AC->SpinnerCtrl.Ispin,NumBytes);
      BufPtr += NumBytes;

      NumBytes = sizeof(double);
      memcpy(BufPtr,&AC->SpinnerCtrl.Itrans,NumBytes);
      BufPtr += NumBytes;

      NumBytes = sizeof(double);
      memcpy(BufPtr,&AC->SpinnerCtrl.SpinRate,NumBytes);
      BufPtr += NumBytes;

      NumBytes = sizeof(double);
      memcpy(BufPtr,&AC->SpinnerCtrl.Knute,NumBytes);
      BufPtr += NumBytes;

      NumBytes = sizeof(double);
      memcpy(BufPtr,&AC->SpinnerCtrl.Kprec,NumBytes);
      BufPtr += NumBytes;

      NumBytes = sizeof(double);
      for(i=0;i<3;i++) {
         memcpy(BufPtr,&AC->ThreeAxisCtrl.Kr[i],NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(double);
      for(i=0;i<3;i++) {
         memcpy(BufPtr,&AC->ThreeAxisCtrl.Kp[i],NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(double);
      memcpy(BufPtr,&AC->ThreeAxisCtrl.Kunl,NumBytes);
      BufPtr += NumBytes;

      NumBytes = sizeof(double);
      for(i=0;i<3;i++) {
         memcpy(BufPtr,&AC->IssCtrl.Kr[i],NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(double);
      for(i=0;i<3;i++) {
         memcpy(BufPtr,&AC->IssCtrl.Kp[i],NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(double);
      memcpy(BufPtr,&AC->IssCtrl.Tmax,NumBytes);
      BufPtr += NumBytes;

      NumBytes = sizeof(double);
      for(i=0;i<3;i++) {
         memcpy(BufPtr,&AC->CmgCtrl.Kr[i],NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(double);
      for(i=0;i<3;i++) {
         memcpy(BufPtr,&AC->CmgCtrl.Kp[i],NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(double);
      for(i=0;i<3;i++) {
         memcpy(BufPtr,&AC->ThrCtrl.Kw[i],NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(double);
      for(i=0;i<3;i++) {
         memcpy(BufPtr,&AC->ThrCtrl.Kth[i],NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(double);
      memcpy(BufPtr,&AC->ThrCtrl.Kv,NumBytes);
      BufPtr += NumBytes;

      NumBytes = sizeof(double);
      memcpy(BufPtr,&AC->ThrCtrl.Kp,NumBytes);
      BufPtr += NumBytes;

      NumBytes = sizeof(double);
      for(i=0;i<3;i++) {
         memcpy(BufPtr,&AC->CfsCtrl.Kr[i],NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(double);
      for(i=0;i<3;i++) {
         memcpy(BufPtr,&AC->CfsCtrl.Kp[i],NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(double);
      memcpy(BufPtr,&AC->CfsCtrl.Kunl,NumBytes);
      BufPtr += NumBytes;

      NumBytes = sizeof(double);
      for(i=0;i<3;i++) {
         memcpy(BufPtr,&AC->ThrSteerCtrl.Kr[i],NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(double);
      for(i=0;i<3;i++) {
         memcpy(BufPtr,&AC->ThrSteerCtrl.Kp[i],NumBytes);
         BufPtr += NumBytes;
      }

      memset(BufPtr,0,1);


      Success = write(I->Socket,I->AcTblBuf,I->AcTblBufLen);
      if (Success < 0) {
         printf("Error writing to socket in WriteAcTblToSocket.\n");
         exit(1);
      }
      read(I->Socket,Ack,4);
}
/******************************************************************************/
void WriteAcArraySizesToSocket(struct AcType *AC, struct AcIpcType *I)
{
      char *Msg;
      char Ack[4] = "Ack\0";
      int NumBytes;
      int MsgLen = 0;
      int Success;

      Msg = (char *) calloc(12+1,sizeof(long));

      NumBytes = sizeof(long);
      memcpy(&Msg[MsgLen],&AC->Nb,NumBytes);
      MsgLen += NumBytes;

      NumBytes = sizeof(long);
      memcpy(&Msg[MsgLen],&AC->Ng,NumBytes);
      MsgLen += NumBytes;

      NumBytes = sizeof(long);
      memcpy(&Msg[MsgLen],&AC->Nwhl,NumBytes);
      MsgLen += NumBytes;

      NumBytes = sizeof(long);
      memcpy(&Msg[MsgLen],&AC->Nmtb,NumBytes);
      MsgLen += NumBytes;

      NumBytes = sizeof(long);
      memcpy(&Msg[MsgLen],&AC->Nthr,NumBytes);
      MsgLen += NumBytes;

      NumBytes = sizeof(long);
      memcpy(&Msg[MsgLen],&AC->Ngyro,NumBytes);
      MsgLen += NumBytes;

      NumBytes = sizeof(long);
      memcpy(&Msg[MsgLen],&AC->Nmag,NumBytes);
      MsgLen += NumBytes;

      NumBytes = sizeof(long);
      memcpy(&Msg[MsgLen],&AC->Ncss,NumBytes);
      MsgLen += NumBytes;

      NumBytes = sizeof(long);
      memcpy(&Msg[MsgLen],&AC->Nfss,NumBytes);
      MsgLen += NumBytes;

      NumBytes = sizeof(long);
      memcpy(&Msg[MsgLen],&AC->Nst,NumBytes);
      MsgLen += NumBytes;

      NumBytes = sizeof(long);
      memcpy(&Msg[MsgLen],&AC->Ngps,NumBytes);
      MsgLen += NumBytes;

      NumBytes = sizeof(long);
      memcpy(&Msg[MsgLen],&AC->Nacc,NumBytes);
      MsgLen += NumBytes;

      memset(&Msg[MsgLen],0,1);

      Success = write(I->Socket,Msg,MsgLen);
      if (Success < 0) {
         printf("Error writing to socket in WriteAcArraySizesToSocket.\n");
         exit(1);
      }
      free(Msg);
      read(I->Socket,Ack,4);
}
/******************************************************************************/
void WriteAcBufLensToSocket(struct AcIpcType *I)
{
      char *Msg;
      char Ack[4] = "Ack\0";
      int NumBytes;
      int MsgLen = 0;
      int Success;

      Msg = (char *) calloc(3,sizeof(long));

      NumBytes = sizeof(long);
      memcpy(&Msg[MsgLen],&I->AcInBufLen,NumBytes);
      MsgLen += NumBytes;

      NumBytes = sizeof(long);
      memcpy(&Msg[MsgLen],&I->AcOutBufLen,NumBytes);
      MsgLen += NumBytes;

      NumBytes = sizeof(long);
      memcpy(&Msg[MsgLen],&I->AcTblBufLen,NumBytes);
      MsgLen += NumBytes;

      memset(&Msg[MsgLen],0,1);

      Success = write(I->Socket,Msg,MsgLen);
      if (Success < 0) {
         printf("Error writing to socket in WriteAcBufLensToSocket.\n");
         exit(1);
      }
      free(Msg);
      read(I->Socket,Ack,4);
}

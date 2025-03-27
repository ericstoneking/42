#include "Ac.h"

/******************************************************************************/
void WriteAcOutToSocket(struct AcType *AC,struct AcIpcType *I)
{
      int Success;
      int NumBytes;
      char *BufPtr;
      char Ack[4] = "Ack\0";
      int k;
      int i;

      BufPtr = &I->AcOutBuf[0];

      NumBytes = sizeof(double);
      for(i=0;i<3;i++) {
         memcpy(BufPtr,&AC->svb[i],NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(double);
      for(i=0;i<3;i++) {
         memcpy(BufPtr,&AC->bvb[i],NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(double);
      for(i=0;i<3;i++) {
         memcpy(BufPtr,&AC->Hvb[i],NumBytes);
         BufPtr += NumBytes;
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
      for(k=0;k<AC->Ng;k++) {
         for(i=0;i<3;i++) {
            memcpy(BufPtr,&AC->G[k].GCmd.AngRate[i],NumBytes);
            BufPtr += NumBytes;
         }
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Ng;k++) {
         for(i=0;i<3;i++) {
            memcpy(BufPtr,&AC->G[k].GCmd.Ang[i],NumBytes);
            BufPtr += NumBytes;
         }
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Ng;k++) {
         for(i=0;i<3;i++) {
            memcpy(BufPtr,&AC->G[k].GCmd.PosRate[i],NumBytes);
            BufPtr += NumBytes;
         }
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Ng;k++) {
         for(i=0;i<3;i++) {
            memcpy(BufPtr,&AC->G[k].GCmd.Pos[i],NumBytes);
            BufPtr += NumBytes;
         }
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Nwhl;k++) {
         memcpy(BufPtr,&AC->Whl[k].Tcmd,NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Nmtb;k++) {
         memcpy(BufPtr,&AC->MTB[k].Mcmd,NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Nthr;k++) {
         memcpy(BufPtr,&AC->Thr[k].PulseWidthCmd,NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Nthr;k++) {
         memcpy(BufPtr,&AC->Thr[k].ThrustLevelCmd,NumBytes);
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

      NumBytes = sizeof(double);
      for(i=0;i<3;i++) {
         memcpy(BufPtr,&AC->Cmd.AngRate[i],NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(double);
      for(i=0;i<3;i++) {
         memcpy(BufPtr,&AC->Cmd.Ang[i],NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(double);
      for(i=0;i<3;i++) {
         memcpy(BufPtr,&AC->Cmd.PosRate[i],NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(double);
      for(i=0;i<3;i++) {
         memcpy(BufPtr,&AC->Cmd.Pos[i],NumBytes);
         BufPtr += NumBytes;
      }

      memset(BufPtr,0,1);


      Success = write(I->Socket,I->AcOutBuf,I->AcOutBufLen);
      if (Success < 0) {
         printf("Error writing to socket in WriteAcOutToSocket.\n");
         exit(1);
      }
      read(I->Socket,Ack,4);
}
/******************************************************************************/
void ReadAcInFromSocket(struct AcType *AC,struct AcIpcType *I)
{
      int Success;
      int NumBytes;
      char *BufPtr;
      char Ack[4] = "Ack\0";
      int k;
      int i;

      BufPtr = &I->AcInBuf[0];
      Success = read(I->Socket,I->AcInBuf,I->AcInBufLen);
      if (Success < 0) {
         printf("Error reading from socket in ReadAcInFromSocket.\n");
         exit(1);
      }
      write(I->Socket,Ack,4);

      NumBytes = sizeof(double);
      for(k=0;k<AC->Ng;k++) {
         for(i=0;i<3;i++) {
            memcpy(&AC->G[k].Ang[i],BufPtr,NumBytes);
            BufPtr += NumBytes;
         }
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
      for(k=0;k<AC->Ngyro;k++) {
         memcpy(&AC->Gyro[k].Rate,BufPtr,NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Nmag;k++) {
         memcpy(&AC->MAG[k].Field,BufPtr,NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(long);
      for(k=0;k<AC->Ncss;k++) {
         memcpy(&AC->CSS[k].Valid,BufPtr,NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Ncss;k++) {
         memcpy(&AC->CSS[k].Illum,BufPtr,NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(long);
      for(k=0;k<AC->Nfss;k++) {
         memcpy(&AC->FSS[k].Valid,BufPtr,NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Nfss;k++) {
         for(i=0;i<2;i++) {
            memcpy(&AC->FSS[k].SunAng[i],BufPtr,NumBytes);
            BufPtr += NumBytes;
         }
      }

      NumBytes = sizeof(long);
      for(k=0;k<AC->Nst;k++) {
         memcpy(&AC->ST[k].Valid,BufPtr,NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Nst;k++) {
         for(i=0;i<4;i++) {
            memcpy(&AC->ST[k].qn[i],BufPtr,NumBytes);
            BufPtr += NumBytes;
         }
      }

      NumBytes = sizeof(long);
      for(k=0;k<AC->Ngps;k++) {
         memcpy(&AC->GPS[k].Valid,BufPtr,NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(long);
      for(k=0;k<AC->Ngps;k++) {
         memcpy(&AC->GPS[k].Rollover,BufPtr,NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(long);
      for(k=0;k<AC->Ngps;k++) {
         memcpy(&AC->GPS[k].Week,BufPtr,NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Ngps;k++) {
         memcpy(&AC->GPS[k].Sec,BufPtr,NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Ngps;k++) {
         for(i=0;i<3;i++) {
            memcpy(&AC->GPS[k].PosN[i],BufPtr,NumBytes);
            BufPtr += NumBytes;
         }
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Ngps;k++) {
         for(i=0;i<3;i++) {
            memcpy(&AC->GPS[k].VelN[i],BufPtr,NumBytes);
            BufPtr += NumBytes;
         }
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Ngps;k++) {
         for(i=0;i<3;i++) {
            memcpy(&AC->GPS[k].PosW[i],BufPtr,NumBytes);
            BufPtr += NumBytes;
         }
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Ngps;k++) {
         for(i=0;i<3;i++) {
            memcpy(&AC->GPS[k].VelW[i],BufPtr,NumBytes);
            BufPtr += NumBytes;
         }
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Ngps;k++) {
         memcpy(&AC->GPS[k].Lng,BufPtr,NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Ngps;k++) {
         memcpy(&AC->GPS[k].Lat,BufPtr,NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Ngps;k++) {
         memcpy(&AC->GPS[k].Alt,BufPtr,NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Ngps;k++) {
         memcpy(&AC->GPS[k].WgsLng,BufPtr,NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Ngps;k++) {
         memcpy(&AC->GPS[k].WgsLat,BufPtr,NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Ngps;k++) {
         memcpy(&AC->GPS[k].WgsAlt,BufPtr,NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Nacc;k++) {
         memcpy(&AC->Accel[k].Acc,BufPtr,NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Nwhl;k++) {
         memcpy(&AC->Whl[k].H,BufPtr,NumBytes);
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
}
/******************************************************************************/
void ReadAcTblFromSocket(struct AcType *AC,struct AcIpcType *I)
{
      int Success;
      int NumBytes;
      char *BufPtr;
      char Ack[4] = "Ack\0";
      int k;
      int i;
      int j;

      BufPtr = &I->AcTblBuf[0];
      Success = read(I->Socket,I->AcTblBuf,I->AcTblBufLen);
      if (Success < 0) {
         printf("Error reading from socket in ReadAcTblFromSocket.\n");
         exit(1);
      }
      write(I->Socket,Ack,4);

      NumBytes = sizeof(long);
      memcpy(&AC->ID,BufPtr,NumBytes);
      BufPtr += NumBytes;

      NumBytes = sizeof(long);
      memcpy(&AC->Nb,BufPtr,NumBytes);
      BufPtr += NumBytes;

      NumBytes = sizeof(long);
      memcpy(&AC->Ng,BufPtr,NumBytes);
      BufPtr += NumBytes;

      NumBytes = sizeof(long);
      memcpy(&AC->Nwhl,BufPtr,NumBytes);
      BufPtr += NumBytes;

      NumBytes = sizeof(long);
      memcpy(&AC->Nmtb,BufPtr,NumBytes);
      BufPtr += NumBytes;

      NumBytes = sizeof(long);
      memcpy(&AC->Nthr,BufPtr,NumBytes);
      BufPtr += NumBytes;

      NumBytes = sizeof(long);
      memcpy(&AC->Ngyro,BufPtr,NumBytes);
      BufPtr += NumBytes;

      NumBytes = sizeof(long);
      memcpy(&AC->Nmag,BufPtr,NumBytes);
      BufPtr += NumBytes;

      NumBytes = sizeof(long);
      memcpy(&AC->Ncss,BufPtr,NumBytes);
      BufPtr += NumBytes;

      NumBytes = sizeof(long);
      memcpy(&AC->Nfss,BufPtr,NumBytes);
      BufPtr += NumBytes;

      NumBytes = sizeof(long);
      memcpy(&AC->Nst,BufPtr,NumBytes);
      BufPtr += NumBytes;

      NumBytes = sizeof(long);
      memcpy(&AC->Ngps,BufPtr,NumBytes);
      BufPtr += NumBytes;

      NumBytes = sizeof(long);
      memcpy(&AC->Nacc,BufPtr,NumBytes);
      BufPtr += NumBytes;

      NumBytes = sizeof(double);
      memcpy(&AC->Pi,BufPtr,NumBytes);
      BufPtr += NumBytes;

      NumBytes = sizeof(double);
      memcpy(&AC->TwoPi,BufPtr,NumBytes);
      BufPtr += NumBytes;

      NumBytes = sizeof(double);
      memcpy(&AC->DT,BufPtr,NumBytes);
      BufPtr += NumBytes;

      NumBytes = sizeof(double);
      memcpy(&AC->mass,BufPtr,NumBytes);
      BufPtr += NumBytes;

      NumBytes = sizeof(double);
      for(i=0;i<3;i++) {
         memcpy(&AC->cm[i],BufPtr,NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(double);
      for(i=0;i<3;i++) {
         for(j=0;j<3;j++) {
            memcpy(&AC->MOI[i][j],BufPtr,NumBytes);
            BufPtr += NumBytes;
         }
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Nb;k++) {
         memcpy(&AC->B[k].mass,BufPtr,NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Nb;k++) {
         for(i=0;i<3;i++) {
            memcpy(&AC->B[k].cm[i],BufPtr,NumBytes);
            BufPtr += NumBytes;
         }
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Nb;k++) {
         for(i=0;i<3;i++) {
            for(j=0;j<3;j++) {
               memcpy(&AC->B[k].MOI[i][j],BufPtr,NumBytes);
               BufPtr += NumBytes;
            }
         }
      }

      NumBytes = sizeof(long);
      for(k=0;k<AC->Ng;k++) {
         memcpy(&AC->G[k].IsSpherical,BufPtr,NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(long);
      for(k=0;k<AC->Ng;k++) {
         memcpy(&AC->G[k].RotDOF,BufPtr,NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(long);
      for(k=0;k<AC->Ng;k++) {
         memcpy(&AC->G[k].TrnDOF,BufPtr,NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(long);
      for(k=0;k<AC->Ng;k++) {
         memcpy(&AC->G[k].RotSeq,BufPtr,NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(long);
      for(k=0;k<AC->Ng;k++) {
         memcpy(&AC->G[k].TrnSeq,BufPtr,NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Ng;k++) {
         for(i=0;i<3;i++) {
            for(j=0;j<3;j++) {
               memcpy(&AC->G[k].CGiBi[i][j],BufPtr,NumBytes);
               BufPtr += NumBytes;
            }
         }
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Ng;k++) {
         for(i=0;i<3;i++) {
            for(j=0;j<3;j++) {
               memcpy(&AC->G[k].CBoGo[i][j],BufPtr,NumBytes);
               BufPtr += NumBytes;
            }
         }
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Ng;k++) {
         for(i=0;i<3;i++) {
            memcpy(&AC->G[k].AngGain[i],BufPtr,NumBytes);
            BufPtr += NumBytes;
         }
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Ng;k++) {
         for(i=0;i<3;i++) {
            memcpy(&AC->G[k].AngRateGain[i],BufPtr,NumBytes);
            BufPtr += NumBytes;
         }
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Ng;k++) {
         for(i=0;i<3;i++) {
            memcpy(&AC->G[k].PosGain[i],BufPtr,NumBytes);
            BufPtr += NumBytes;
         }
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Ng;k++) {
         for(i=0;i<3;i++) {
            memcpy(&AC->G[k].PosRateGain[i],BufPtr,NumBytes);
            BufPtr += NumBytes;
         }
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Ng;k++) {
         for(i=0;i<3;i++) {
            memcpy(&AC->G[k].MaxAngRate[i],BufPtr,NumBytes);
            BufPtr += NumBytes;
         }
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Ng;k++) {
         for(i=0;i<3;i++) {
            memcpy(&AC->G[k].MaxPosRate[i],BufPtr,NumBytes);
            BufPtr += NumBytes;
         }
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Ng;k++) {
         for(i=0;i<3;i++) {
            memcpy(&AC->G[k].MaxTrq[i],BufPtr,NumBytes);
            BufPtr += NumBytes;
         }
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Ng;k++) {
         for(i=0;i<3;i++) {
            memcpy(&AC->G[k].MaxFrc[i],BufPtr,NumBytes);
            BufPtr += NumBytes;
         }
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Ngyro;k++) {
         for(i=0;i<3;i++) {
            memcpy(&AC->Gyro[k].Axis[i],BufPtr,NumBytes);
            BufPtr += NumBytes;
         }
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Nmag;k++) {
         for(i=0;i<3;i++) {
            memcpy(&AC->MAG[k].Axis[i],BufPtr,NumBytes);
            BufPtr += NumBytes;
         }
      }

      NumBytes = sizeof(long);
      for(k=0;k<AC->Ncss;k++) {
         memcpy(&AC->CSS[k].Body,BufPtr,NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Ncss;k++) {
         for(i=0;i<3;i++) {
            memcpy(&AC->CSS[k].Axis[i],BufPtr,NumBytes);
            BufPtr += NumBytes;
         }
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Ncss;k++) {
         memcpy(&AC->CSS[k].Scale,BufPtr,NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Nfss;k++) {
         for(i=0;i<4;i++) {
            memcpy(&AC->FSS[k].qb[i],BufPtr,NumBytes);
            BufPtr += NumBytes;
         }
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Nfss;k++) {
         for(i=0;i<3;i++) {
            for(j=0;j<3;j++) {
               memcpy(&AC->FSS[k].CB[i][j],BufPtr,NumBytes);
               BufPtr += NumBytes;
            }
         }
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Nst;k++) {
         for(i=0;i<4;i++) {
            memcpy(&AC->ST[k].qb[i],BufPtr,NumBytes);
            BufPtr += NumBytes;
         }
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Nst;k++) {
         for(i=0;i<3;i++) {
            for(j=0;j<3;j++) {
               memcpy(&AC->ST[k].CB[i][j],BufPtr,NumBytes);
               BufPtr += NumBytes;
            }
         }
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Nacc;k++) {
         for(i=0;i<3;i++) {
            memcpy(&AC->Accel[k].PosB[i],BufPtr,NumBytes);
            BufPtr += NumBytes;
         }
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Nacc;k++) {
         for(i=0;i<3;i++) {
            memcpy(&AC->Accel[k].Axis[i],BufPtr,NumBytes);
            BufPtr += NumBytes;
         }
      }

      NumBytes = sizeof(long);
      for(k=0;k<AC->Nwhl;k++) {
         memcpy(&AC->Whl[k].Body,BufPtr,NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Nwhl;k++) {
         for(i=0;i<3;i++) {
            memcpy(&AC->Whl[k].Axis[i],BufPtr,NumBytes);
            BufPtr += NumBytes;
         }
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Nwhl;k++) {
         for(i=0;i<3;i++) {
            memcpy(&AC->Whl[k].DistVec[i],BufPtr,NumBytes);
            BufPtr += NumBytes;
         }
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Nwhl;k++) {
         memcpy(&AC->Whl[k].J,BufPtr,NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Nwhl;k++) {
         memcpy(&AC->Whl[k].Tmax,BufPtr,NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Nwhl;k++) {
         memcpy(&AC->Whl[k].Hmax,BufPtr,NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Nmtb;k++) {
         for(i=0;i<3;i++) {
            memcpy(&AC->MTB[k].Axis[i],BufPtr,NumBytes);
            BufPtr += NumBytes;
         }
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Nmtb;k++) {
         for(i=0;i<3;i++) {
            memcpy(&AC->MTB[k].DistVec[i],BufPtr,NumBytes);
            BufPtr += NumBytes;
         }
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Nmtb;k++) {
         memcpy(&AC->MTB[k].Mmax,BufPtr,NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(long);
      for(k=0;k<AC->Nthr;k++) {
         memcpy(&AC->Thr[k].Body,BufPtr,NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Nthr;k++) {
         for(i=0;i<3;i++) {
            memcpy(&AC->Thr[k].PosB[i],BufPtr,NumBytes);
            BufPtr += NumBytes;
         }
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Nthr;k++) {
         for(i=0;i<3;i++) {
            memcpy(&AC->Thr[k].Axis[i],BufPtr,NumBytes);
            BufPtr += NumBytes;
         }
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Nthr;k++) {
         for(i=0;i<3;i++) {
            memcpy(&AC->Thr[k].rxA[i],BufPtr,NumBytes);
            BufPtr += NumBytes;
         }
      }

      NumBytes = sizeof(double);
      for(k=0;k<AC->Nthr;k++) {
         memcpy(&AC->Thr[k].Fmax,BufPtr,NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(double);
      memcpy(&AC->InstantCtrl.wc,BufPtr,NumBytes);
      BufPtr += NumBytes;

      NumBytes = sizeof(double);
      memcpy(&AC->InstantCtrl.amax,BufPtr,NumBytes);
      BufPtr += NumBytes;

      NumBytes = sizeof(double);
      memcpy(&AC->InstantCtrl.vmax,BufPtr,NumBytes);
      BufPtr += NumBytes;

      NumBytes = sizeof(double);
      memcpy(&AC->InstantCtrl.Kprec,BufPtr,NumBytes);
      BufPtr += NumBytes;

      NumBytes = sizeof(double);
      memcpy(&AC->InstantCtrl.Knute,BufPtr,NumBytes);
      BufPtr += NumBytes;

      NumBytes = sizeof(double);
      for(i=0;i<3;i++) {
         memcpy(&AC->SandboxCtrl.Kr[i],BufPtr,NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(double);
      for(i=0;i<3;i++) {
         memcpy(&AC->SandboxCtrl.Kp[i],BufPtr,NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(double);
      memcpy(&AC->SpinnerCtrl.Ispin,BufPtr,NumBytes);
      BufPtr += NumBytes;

      NumBytes = sizeof(double);
      memcpy(&AC->SpinnerCtrl.Itrans,BufPtr,NumBytes);
      BufPtr += NumBytes;

      NumBytes = sizeof(double);
      memcpy(&AC->SpinnerCtrl.SpinRate,BufPtr,NumBytes);
      BufPtr += NumBytes;

      NumBytes = sizeof(double);
      memcpy(&AC->SpinnerCtrl.Knute,BufPtr,NumBytes);
      BufPtr += NumBytes;

      NumBytes = sizeof(double);
      memcpy(&AC->SpinnerCtrl.Kprec,BufPtr,NumBytes);
      BufPtr += NumBytes;

      NumBytes = sizeof(double);
      for(i=0;i<3;i++) {
         memcpy(&AC->ThreeAxisCtrl.Kr[i],BufPtr,NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(double);
      for(i=0;i<3;i++) {
         memcpy(&AC->ThreeAxisCtrl.Kp[i],BufPtr,NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(double);
      memcpy(&AC->ThreeAxisCtrl.Kunl,BufPtr,NumBytes);
      BufPtr += NumBytes;

      NumBytes = sizeof(double);
      for(i=0;i<3;i++) {
         memcpy(&AC->IssCtrl.Kr[i],BufPtr,NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(double);
      for(i=0;i<3;i++) {
         memcpy(&AC->IssCtrl.Kp[i],BufPtr,NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(double);
      memcpy(&AC->IssCtrl.Tmax,BufPtr,NumBytes);
      BufPtr += NumBytes;

      NumBytes = sizeof(double);
      for(i=0;i<3;i++) {
         memcpy(&AC->CmgCtrl.Kr[i],BufPtr,NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(double);
      for(i=0;i<3;i++) {
         memcpy(&AC->CmgCtrl.Kp[i],BufPtr,NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(double);
      for(i=0;i<3;i++) {
         memcpy(&AC->ThrCtrl.Kw[i],BufPtr,NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(double);
      for(i=0;i<3;i++) {
         memcpy(&AC->ThrCtrl.Kth[i],BufPtr,NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(double);
      memcpy(&AC->ThrCtrl.Kv,BufPtr,NumBytes);
      BufPtr += NumBytes;

      NumBytes = sizeof(double);
      memcpy(&AC->ThrCtrl.Kp,BufPtr,NumBytes);
      BufPtr += NumBytes;

      NumBytes = sizeof(double);
      for(i=0;i<3;i++) {
         memcpy(&AC->CfsCtrl.Kr[i],BufPtr,NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(double);
      for(i=0;i<3;i++) {
         memcpy(&AC->CfsCtrl.Kp[i],BufPtr,NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(double);
      memcpy(&AC->CfsCtrl.Kunl,BufPtr,NumBytes);
      BufPtr += NumBytes;

      NumBytes = sizeof(double);
      for(i=0;i<3;i++) {
         memcpy(&AC->ThrSteerCtrl.Kr[i],BufPtr,NumBytes);
         BufPtr += NumBytes;
      }

      NumBytes = sizeof(double);
      for(i=0;i<3;i++) {
         memcpy(&AC->ThrSteerCtrl.Kp[i],BufPtr,NumBytes);
         BufPtr += NumBytes;
      }
}
/******************************************************************************/
void ReadAcArraySizesFromSocket(struct AcType *AC, struct AcIpcType *I)
{
      int Success;
      char Msg[256];
      char Ack[4] = "Ack\0";
      int MsgIdx = 0;
      int NumBytes;

      Success = read(I->Socket,Msg,255);
      if (Success < 0) {
         printf("Error reading from socket in ReadAcArraySizesFromSocket.\n");
         exit(1);
      }
      write(I->Socket,Ack,4);

      NumBytes = sizeof(long);
      memcpy(&AC->Nb,&Msg[MsgIdx],NumBytes);
      MsgIdx += NumBytes;

      NumBytes = sizeof(long);
      memcpy(&AC->Ng,&Msg[MsgIdx],NumBytes);
      MsgIdx += NumBytes;

      NumBytes = sizeof(long);
      memcpy(&AC->Nwhl,&Msg[MsgIdx],NumBytes);
      MsgIdx += NumBytes;

      NumBytes = sizeof(long);
      memcpy(&AC->Nmtb,&Msg[MsgIdx],NumBytes);
      MsgIdx += NumBytes;

      NumBytes = sizeof(long);
      memcpy(&AC->Nthr,&Msg[MsgIdx],NumBytes);
      MsgIdx += NumBytes;

      NumBytes = sizeof(long);
      memcpy(&AC->Ngyro,&Msg[MsgIdx],NumBytes);
      MsgIdx += NumBytes;

      NumBytes = sizeof(long);
      memcpy(&AC->Nmag,&Msg[MsgIdx],NumBytes);
      MsgIdx += NumBytes;

      NumBytes = sizeof(long);
      memcpy(&AC->Ncss,&Msg[MsgIdx],NumBytes);
      MsgIdx += NumBytes;

      NumBytes = sizeof(long);
      memcpy(&AC->Nfss,&Msg[MsgIdx],NumBytes);
      MsgIdx += NumBytes;

      NumBytes = sizeof(long);
      memcpy(&AC->Nst,&Msg[MsgIdx],NumBytes);
      MsgIdx += NumBytes;

      NumBytes = sizeof(long);
      memcpy(&AC->Ngps,&Msg[MsgIdx],NumBytes);
      MsgIdx += NumBytes;

      NumBytes = sizeof(long);
      memcpy(&AC->Nacc,&Msg[MsgIdx],NumBytes);
      MsgIdx += NumBytes;

}
/******************************************************************************/
void ReadAcBufLensFromSocket(struct AcIpcType *I)
{
      int Success;
      char Msg[256];
      char Ack[4] = "Ack\0";
      int MsgIdx = 0;
      int NumBytes;

      Success = read(I->Socket,Msg,255);
      if (Success < 0) {
         printf("Error reading from socket in ReadAcBufLensFromSocket.\n");
         exit(1);
      }
      write(I->Socket,Ack,4);

      NumBytes = sizeof(long);
      memcpy(&I->AcInBufLen,&Msg[MsgIdx],NumBytes);
      MsgIdx += NumBytes;

      NumBytes = sizeof(long);
      memcpy(&I->AcOutBufLen,&Msg[MsgIdx],NumBytes);
      MsgIdx += NumBytes;

      NumBytes = sizeof(long);
      memcpy(&I->AcTblBufLen,&Msg[MsgIdx],NumBytes);
      MsgIdx += NumBytes;

}

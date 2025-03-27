#include "42.h"
#define EXTERN extern
#include "Ac.h"
#undef EXTERN

long AcTlmEnabled, MaxAcCtr;
long AcGTlmEnabled, MaxAcGCtr;
long Ac_GCmdTlmEnabled, MaxAc_GCmdCtr;
long AcGyroTlmEnabled, MaxAcGyroCtr;
long AcMAGTlmEnabled, MaxAcMAGCtr;
long AcCSSTlmEnabled, MaxAcCSSCtr;
long AcFSSTlmEnabled, MaxAcFSSCtr;
long AcSTTlmEnabled, MaxAcSTCtr;
long AcGPSTlmEnabled, MaxAcGPSCtr;
long AcAccelTlmEnabled, MaxAcAccelCtr;
long AcWhlTlmEnabled, MaxAcWhlCtr;
long AcMTBTlmEnabled, MaxAcMTBCtr;
long AcThrTlmEnabled, MaxAcThrCtr;
long AcCmdTlmEnabled, MaxAcCmdCtr;
/******************************************************************************/
void WriteAcToCsv(void)
{
      struct AcType *AC;
      static FILE **outfile;
      char FileName[80];
      long Isc;
      long i;
      static long First = 1;
      static long OutCtr = 1000000000;

      if (First) {
         First = 0;
         outfile = (FILE**) calloc(Nsc,sizeof(FILE *));
         for(Isc=0;Isc<Nsc;Isc++) {
            if (SC[Isc].Exists) {
               AC = &SC[Isc].AC;
               if (Nsc==1) sprintf(FileName,"Ac.csv");
               else if (Nsc<=10) sprintf(FileName,"Ac%1ld.csv",Isc);
               else sprintf(FileName,"Ac%02ld.csv",Isc);
               outfile[Isc] = FileOpen(InOutPath,FileName,"w");

               fprintf(outfile[Isc],"Ac_Time");
               for(i=0;i<3;i++) {
                  fprintf(outfile[Isc],",Ac_svb_%ld",i);
               }
               for(i=0;i<3;i++) {
                  fprintf(outfile[Isc],",Ac_bvb_%ld",i);
               }
               for(i=0;i<3;i++) {
                  fprintf(outfile[Isc],",Ac_Hvb_%ld",i);
               }
               fprintf(outfile[Isc],"\n");
            }
         }
      }

      OutCtr++;
      if (OutCtr > MaxAcCtr) {
         OutCtr = 1;
         for (Isc=0;Isc<Nsc;Isc++) {
            if (SC[Isc].Exists) {
               AC = &SC[Isc].AC;
               fprintf(outfile[Isc],"%18.12le",AC->Time);
               for(i=0;i<3;i++) {
                  fprintf(outfile[Isc],",%18.12le",AC->svb[i]);
               }
               for(i=0;i<3;i++) {
                  fprintf(outfile[Isc],",%18.12le",AC->bvb[i]);
               }
               for(i=0;i<3;i++) {
                  fprintf(outfile[Isc],",%18.12le",AC->Hvb[i]);
               }
               fprintf(outfile[Isc],"\n");
            }
         }
      }

}
/******************************************************************************/
void WriteAcGToCsv(void)
{
      struct AcType *AC;
      static FILE **outfile;
      char FileName[80];
      long Isc;
      long k;
      long i;
      static long First = 1;
      static long OutCtr = 1000000000;

      if (First) {
         First = 0;
         outfile = (FILE**) calloc(Nsc,sizeof(FILE *));
         for(Isc=0;Isc<Nsc;Isc++) {
            if (SC[Isc].Exists) {
               AC = &SC[Isc].AC;
               if (Nsc==1) sprintf(FileName,"AcG.csv");
               else if (Nsc<=10) sprintf(FileName,"AcG%1ld.csv",Isc);
               else sprintf(FileName,"AcG%02ld.csv",Isc);
               outfile[Isc] = FileOpen(InOutPath,FileName,"w");

               fprintf(outfile[Isc],"AcG_Time");
               for(k=0;k<AC->Ng;k++) {
                  for(i=0;i<3;i++) {
                     fprintf(outfile[Isc],",Ac_G%ld_Ang_%ld",k,i);
                  }
               }
               fprintf(outfile[Isc],"\n");
            }
         }
      }

      OutCtr++;
      if (OutCtr > MaxAcGCtr) {
         OutCtr = 1;
         for (Isc=0;Isc<Nsc;Isc++) {
            if (SC[Isc].Exists) {
               AC = &SC[Isc].AC;
               fprintf(outfile[Isc],"%18.12le",AC->Time);
               for(k=0;k<AC->Ng;k++) {
                  for(i=0;i<3;i++) {
                     fprintf(outfile[Isc],",%18.12le",AC->G[k].Ang[i]);
                  }
               }
               fprintf(outfile[Isc],"\n");
            }
         }
      }

}
/******************************************************************************/
void WriteAc_GCmdToCsv(void)
{
      struct AcType *AC;
      static FILE **outfile;
      char FileName[80];
      long Isc;
      long k;
      long i;
      static long First = 1;
      static long OutCtr = 1000000000;

      if (First) {
         First = 0;
         outfile = (FILE**) calloc(Nsc,sizeof(FILE *));
         for(Isc=0;Isc<Nsc;Isc++) {
            if (SC[Isc].Exists) {
               AC = &SC[Isc].AC;
               if (Nsc==1) sprintf(FileName,"Ac_GCmd.csv");
               else if (Nsc<=10) sprintf(FileName,"Ac_GCmd%1ld.csv",Isc);
               else sprintf(FileName,"Ac_GCmd%02ld.csv",Isc);
               outfile[Isc] = FileOpen(InOutPath,FileName,"w");

               fprintf(outfile[Isc],"Ac_GCmd_Time");
               for(k=0;k<AC->Ng;k++) {
                  for(i=0;i<4;i++) {
                     fprintf(outfile[Isc],",Ac_G%ld_qrl_%ld",k,i);
                  }
               }
               for(k=0;k<AC->Ng;k++) {
                  for(i=0;i<4;i++) {
                     fprintf(outfile[Isc],",Ac_G%ld_qrn_%ld",k,i);
                  }
               }
               for(k=0;k<AC->Ng;k++) {
                  for(i=0;i<3;i++) {
                     fprintf(outfile[Isc],",Ac_G%ld_AngRate_%ld",k,i);
                  }
               }
               for(k=0;k<AC->Ng;k++) {
                  for(i=0;i<3;i++) {
                     fprintf(outfile[Isc],",Ac_G%ld_Ang_%ld",k,i);
                  }
               }
               for(k=0;k<AC->Ng;k++) {
                  for(i=0;i<3;i++) {
                     fprintf(outfile[Isc],",Ac_G%ld_PosRate_%ld",k,i);
                  }
               }
               for(k=0;k<AC->Ng;k++) {
                  for(i=0;i<3;i++) {
                     fprintf(outfile[Isc],",Ac_G%ld_Pos_%ld",k,i);
                  }
               }
               fprintf(outfile[Isc],"\n");
            }
         }
      }

      OutCtr++;
      if (OutCtr > MaxAc_GCmdCtr) {
         OutCtr = 1;
         for (Isc=0;Isc<Nsc;Isc++) {
            if (SC[Isc].Exists) {
               AC = &SC[Isc].AC;
               fprintf(outfile[Isc],"%18.12le",AC->Time);
               for(k=0;k<AC->Ng;k++) {
                  for(i=0;i<3;i++) {
                     fprintf(outfile[Isc],",%18.12le",AC->G[k].Ang[i]);
                  }
               }
               for(k=0;k<AC->Ng;k++) {
                  for(i=0;i<4;i++) {
                     fprintf(outfile[Isc],",%18.12le",AC->G[k].GCmd.qrl[i]);
                  }
               }
               for(k=0;k<AC->Ng;k++) {
                  for(i=0;i<4;i++) {
                     fprintf(outfile[Isc],",%18.12le",AC->G[k].GCmd.qrn[i]);
                  }
               }
               for(k=0;k<AC->Ng;k++) {
                  for(i=0;i<3;i++) {
                     fprintf(outfile[Isc],",%18.12le",AC->G[k].GCmd.AngRate[i]);
                  }
               }
               for(k=0;k<AC->Ng;k++) {
                  for(i=0;i<3;i++) {
                     fprintf(outfile[Isc],",%18.12le",AC->G[k].GCmd.Ang[i]);
                  }
               }
               for(k=0;k<AC->Ng;k++) {
                  for(i=0;i<3;i++) {
                     fprintf(outfile[Isc],",%18.12le",AC->G[k].GCmd.PosRate[i]);
                  }
               }
               for(k=0;k<AC->Ng;k++) {
                  for(i=0;i<3;i++) {
                     fprintf(outfile[Isc],",%18.12le",AC->G[k].GCmd.Pos[i]);
                  }
               }
               fprintf(outfile[Isc],"\n");
            }
         }
      }

}
/******************************************************************************/
void WriteAcGyroToCsv(void)
{
      struct AcType *AC;
      static FILE **outfile;
      char FileName[80];
      long Isc;
      long k;
      static long First = 1;
      static long OutCtr = 1000000000;

      if (First) {
         First = 0;
         outfile = (FILE**) calloc(Nsc,sizeof(FILE *));
         for(Isc=0;Isc<Nsc;Isc++) {
            if (SC[Isc].Exists) {
               AC = &SC[Isc].AC;
               if (Nsc==1) sprintf(FileName,"AcGyro.csv");
               else if (Nsc<=10) sprintf(FileName,"AcGyro%1ld.csv",Isc);
               else sprintf(FileName,"AcGyro%02ld.csv",Isc);
               outfile[Isc] = FileOpen(InOutPath,FileName,"w");

               fprintf(outfile[Isc],"AcGyro_Time");
               for(k=0;k<AC->Ngyro;k++) {
                  fprintf(outfile[Isc],",Ac_Gyro%ld_Rate",k);
               }
               fprintf(outfile[Isc],"\n");
            }
         }
      }

      OutCtr++;
      if (OutCtr > MaxAcGyroCtr) {
         OutCtr = 1;
         for (Isc=0;Isc<Nsc;Isc++) {
            if (SC[Isc].Exists) {
               AC = &SC[Isc].AC;
               fprintf(outfile[Isc],"%18.12le",AC->Time);
               for(k=0;k<AC->Ngyro;k++) {
                  fprintf(outfile[Isc],",%18.12le",AC->Gyro[k].Rate);
               }
               fprintf(outfile[Isc],"\n");
            }
         }
      }

}
/******************************************************************************/
void WriteAcMAGToCsv(void)
{
      struct AcType *AC;
      static FILE **outfile;
      char FileName[80];
      long Isc;
      long k;
      static long First = 1;
      static long OutCtr = 1000000000;

      if (First) {
         First = 0;
         outfile = (FILE**) calloc(Nsc,sizeof(FILE *));
         for(Isc=0;Isc<Nsc;Isc++) {
            if (SC[Isc].Exists) {
               AC = &SC[Isc].AC;
               if (Nsc==1) sprintf(FileName,"AcMAG.csv");
               else if (Nsc<=10) sprintf(FileName,"AcMAG%1ld.csv",Isc);
               else sprintf(FileName,"AcMAG%02ld.csv",Isc);
               outfile[Isc] = FileOpen(InOutPath,FileName,"w");

               fprintf(outfile[Isc],"AcMAG_Time");
               for(k=0;k<AC->Nmag;k++) {
                  fprintf(outfile[Isc],",Ac_MAG%ld_Field",k);
               }
               fprintf(outfile[Isc],"\n");
            }
         }
      }

      OutCtr++;
      if (OutCtr > MaxAcMAGCtr) {
         OutCtr = 1;
         for (Isc=0;Isc<Nsc;Isc++) {
            if (SC[Isc].Exists) {
               AC = &SC[Isc].AC;
               fprintf(outfile[Isc],"%18.12le",AC->Time);
               for(k=0;k<AC->Nmag;k++) {
                  fprintf(outfile[Isc],",%18.12le",AC->MAG[k].Field);
               }
               fprintf(outfile[Isc],"\n");
            }
         }
      }

}
/******************************************************************************/
void WriteAcCSSToCsv(void)
{
      struct AcType *AC;
      static FILE **outfile;
      char FileName[80];
      long Isc;
      long k;
      static long First = 1;
      static long OutCtr = 1000000000;

      if (First) {
         First = 0;
         outfile = (FILE**) calloc(Nsc,sizeof(FILE *));
         for(Isc=0;Isc<Nsc;Isc++) {
            if (SC[Isc].Exists) {
               AC = &SC[Isc].AC;
               if (Nsc==1) sprintf(FileName,"AcCSS.csv");
               else if (Nsc<=10) sprintf(FileName,"AcCSS%1ld.csv",Isc);
               else sprintf(FileName,"AcCSS%02ld.csv",Isc);
               outfile[Isc] = FileOpen(InOutPath,FileName,"w");

               fprintf(outfile[Isc],"AcCSS_Time");
               for(k=0;k<AC->Ncss;k++) {
                  fprintf(outfile[Isc],",Ac_CSS%ld_Valid",k);
               }
               for(k=0;k<AC->Ncss;k++) {
                  fprintf(outfile[Isc],",Ac_CSS%ld_Illum",k);
               }
               fprintf(outfile[Isc],"\n");
            }
         }
      }

      OutCtr++;
      if (OutCtr > MaxAcCSSCtr) {
         OutCtr = 1;
         for (Isc=0;Isc<Nsc;Isc++) {
            if (SC[Isc].Exists) {
               AC = &SC[Isc].AC;
               fprintf(outfile[Isc],"%18.12le",AC->Time);
               for(k=0;k<AC->Ncss;k++) {
                  fprintf(outfile[Isc],",%ld",AC->CSS[k].Valid);
               }
               for(k=0;k<AC->Ncss;k++) {
                  fprintf(outfile[Isc],",%18.12le",AC->CSS[k].Illum);
               }
               fprintf(outfile[Isc],"\n");
            }
         }
      }

}
/******************************************************************************/
void WriteAcFSSToCsv(void)
{
      struct AcType *AC;
      static FILE **outfile;
      char FileName[80];
      long Isc;
      long k;
      long i;
      static long First = 1;
      static long OutCtr = 1000000000;

      if (First) {
         First = 0;
         outfile = (FILE**) calloc(Nsc,sizeof(FILE *));
         for(Isc=0;Isc<Nsc;Isc++) {
            if (SC[Isc].Exists) {
               AC = &SC[Isc].AC;
               if (Nsc==1) sprintf(FileName,"AcFSS.csv");
               else if (Nsc<=10) sprintf(FileName,"AcFSS%1ld.csv",Isc);
               else sprintf(FileName,"AcFSS%02ld.csv",Isc);
               outfile[Isc] = FileOpen(InOutPath,FileName,"w");

               fprintf(outfile[Isc],"AcFSS_Time");
               for(k=0;k<AC->Nfss;k++) {
                  fprintf(outfile[Isc],",Ac_FSS%ld_Valid",k);
               }
               for(k=0;k<AC->Nfss;k++) {
                  for(i=0;i<2;i++) {
                     fprintf(outfile[Isc],",Ac_FSS%ld_SunAng_%ld",k,i);
                  }
               }
               fprintf(outfile[Isc],"\n");
            }
         }
      }

      OutCtr++;
      if (OutCtr > MaxAcFSSCtr) {
         OutCtr = 1;
         for (Isc=0;Isc<Nsc;Isc++) {
            if (SC[Isc].Exists) {
               AC = &SC[Isc].AC;
               fprintf(outfile[Isc],"%18.12le",AC->Time);
               for(k=0;k<AC->Nfss;k++) {
                  fprintf(outfile[Isc],",%ld",AC->FSS[k].Valid);
               }
               for(k=0;k<AC->Nfss;k++) {
                  for(i=0;i<2;i++) {
                     fprintf(outfile[Isc],",%18.12le",AC->FSS[k].SunAng[i]);
                  }
               }
               fprintf(outfile[Isc],"\n");
            }
         }
      }

}
/******************************************************************************/
void WriteAcSTToCsv(void)
{
      struct AcType *AC;
      static FILE **outfile;
      char FileName[80];
      long Isc;
      long k;
      long i;
      static long First = 1;
      static long OutCtr = 1000000000;

      if (First) {
         First = 0;
         outfile = (FILE**) calloc(Nsc,sizeof(FILE *));
         for(Isc=0;Isc<Nsc;Isc++) {
            if (SC[Isc].Exists) {
               AC = &SC[Isc].AC;
               if (Nsc==1) sprintf(FileName,"AcST.csv");
               else if (Nsc<=10) sprintf(FileName,"AcST%1ld.csv",Isc);
               else sprintf(FileName,"AcST%02ld.csv",Isc);
               outfile[Isc] = FileOpen(InOutPath,FileName,"w");

               fprintf(outfile[Isc],"AcST_Time");
               for(k=0;k<AC->Nst;k++) {
                  fprintf(outfile[Isc],",Ac_ST%ld_Valid",k);
               }
               for(k=0;k<AC->Nst;k++) {
                  for(i=0;i<4;i++) {
                     fprintf(outfile[Isc],",Ac_ST%ld_qn_%ld",k,i);
                  }
               }
               fprintf(outfile[Isc],"\n");
            }
         }
      }

      OutCtr++;
      if (OutCtr > MaxAcSTCtr) {
         OutCtr = 1;
         for (Isc=0;Isc<Nsc;Isc++) {
            if (SC[Isc].Exists) {
               AC = &SC[Isc].AC;
               fprintf(outfile[Isc],"%18.12le",AC->Time);
               for(k=0;k<AC->Nst;k++) {
                  fprintf(outfile[Isc],",%ld",AC->ST[k].Valid);
               }
               for(k=0;k<AC->Nst;k++) {
                  for(i=0;i<4;i++) {
                     fprintf(outfile[Isc],",%18.12le",AC->ST[k].qn[i]);
                  }
               }
               fprintf(outfile[Isc],"\n");
            }
         }
      }

}
/******************************************************************************/
void WriteAcGPSToCsv(void)
{
      struct AcType *AC;
      static FILE **outfile;
      char FileName[80];
      long Isc;
      long k;
      long i;
      static long First = 1;
      static long OutCtr = 1000000000;

      if (First) {
         First = 0;
         outfile = (FILE**) calloc(Nsc,sizeof(FILE *));
         for(Isc=0;Isc<Nsc;Isc++) {
            if (SC[Isc].Exists) {
               AC = &SC[Isc].AC;
               if (Nsc==1) sprintf(FileName,"AcGPS.csv");
               else if (Nsc<=10) sprintf(FileName,"AcGPS%1ld.csv",Isc);
               else sprintf(FileName,"AcGPS%02ld.csv",Isc);
               outfile[Isc] = FileOpen(InOutPath,FileName,"w");

               fprintf(outfile[Isc],"AcGPS_Time");
               for(k=0;k<AC->Ngps;k++) {
                  fprintf(outfile[Isc],",Ac_GPS%ld_Valid",k);
               }
               for(k=0;k<AC->Ngps;k++) {
                  fprintf(outfile[Isc],",Ac_GPS%ld_Rollover",k);
               }
               for(k=0;k<AC->Ngps;k++) {
                  fprintf(outfile[Isc],",Ac_GPS%ld_Week",k);
               }
               for(k=0;k<AC->Ngps;k++) {
                  fprintf(outfile[Isc],",Ac_GPS%ld_Sec",k);
               }
               for(k=0;k<AC->Ngps;k++) {
                  for(i=0;i<3;i++) {
                     fprintf(outfile[Isc],",Ac_GPS%ld_PosN_%ld",k,i);
                  }
               }
               for(k=0;k<AC->Ngps;k++) {
                  for(i=0;i<3;i++) {
                     fprintf(outfile[Isc],",Ac_GPS%ld_VelN_%ld",k,i);
                  }
               }
               for(k=0;k<AC->Ngps;k++) {
                  for(i=0;i<3;i++) {
                     fprintf(outfile[Isc],",Ac_GPS%ld_PosW_%ld",k,i);
                  }
               }
               for(k=0;k<AC->Ngps;k++) {
                  for(i=0;i<3;i++) {
                     fprintf(outfile[Isc],",Ac_GPS%ld_VelW_%ld",k,i);
                  }
               }
               for(k=0;k<AC->Ngps;k++) {
                  fprintf(outfile[Isc],",Ac_GPS%ld_Lng",k);
               }
               for(k=0;k<AC->Ngps;k++) {
                  fprintf(outfile[Isc],",Ac_GPS%ld_Lat",k);
               }
               for(k=0;k<AC->Ngps;k++) {
                  fprintf(outfile[Isc],",Ac_GPS%ld_Alt",k);
               }
               for(k=0;k<AC->Ngps;k++) {
                  fprintf(outfile[Isc],",Ac_GPS%ld_WgsLng",k);
               }
               for(k=0;k<AC->Ngps;k++) {
                  fprintf(outfile[Isc],",Ac_GPS%ld_WgsLat",k);
               }
               for(k=0;k<AC->Ngps;k++) {
                  fprintf(outfile[Isc],",Ac_GPS%ld_WgsAlt",k);
               }
               fprintf(outfile[Isc],"\n");
            }
         }
      }

      OutCtr++;
      if (OutCtr > MaxAcGPSCtr) {
         OutCtr = 1;
         for (Isc=0;Isc<Nsc;Isc++) {
            if (SC[Isc].Exists) {
               AC = &SC[Isc].AC;
               fprintf(outfile[Isc],"%18.12le",AC->Time);
               for(k=0;k<AC->Ngps;k++) {
                  fprintf(outfile[Isc],",%ld",AC->GPS[k].Valid);
               }
               for(k=0;k<AC->Ngps;k++) {
                  fprintf(outfile[Isc],",%ld",AC->GPS[k].Rollover);
               }
               for(k=0;k<AC->Ngps;k++) {
                  fprintf(outfile[Isc],",%ld",AC->GPS[k].Week);
               }
               for(k=0;k<AC->Ngps;k++) {
                  fprintf(outfile[Isc],",%18.12le",AC->GPS[k].Sec);
               }
               for(k=0;k<AC->Ngps;k++) {
                  for(i=0;i<3;i++) {
                     fprintf(outfile[Isc],",%18.12le",AC->GPS[k].PosN[i]);
                  }
               }
               for(k=0;k<AC->Ngps;k++) {
                  for(i=0;i<3;i++) {
                     fprintf(outfile[Isc],",%18.12le",AC->GPS[k].VelN[i]);
                  }
               }
               for(k=0;k<AC->Ngps;k++) {
                  for(i=0;i<3;i++) {
                     fprintf(outfile[Isc],",%18.12le",AC->GPS[k].PosW[i]);
                  }
               }
               for(k=0;k<AC->Ngps;k++) {
                  for(i=0;i<3;i++) {
                     fprintf(outfile[Isc],",%18.12le",AC->GPS[k].VelW[i]);
                  }
               }
               for(k=0;k<AC->Ngps;k++) {
                  fprintf(outfile[Isc],",%18.12le",AC->GPS[k].Lng);
               }
               for(k=0;k<AC->Ngps;k++) {
                  fprintf(outfile[Isc],",%18.12le",AC->GPS[k].Lat);
               }
               for(k=0;k<AC->Ngps;k++) {
                  fprintf(outfile[Isc],",%18.12le",AC->GPS[k].Alt);
               }
               for(k=0;k<AC->Ngps;k++) {
                  fprintf(outfile[Isc],",%18.12le",AC->GPS[k].WgsLng);
               }
               for(k=0;k<AC->Ngps;k++) {
                  fprintf(outfile[Isc],",%18.12le",AC->GPS[k].WgsLat);
               }
               for(k=0;k<AC->Ngps;k++) {
                  fprintf(outfile[Isc],",%18.12le",AC->GPS[k].WgsAlt);
               }
               fprintf(outfile[Isc],"\n");
            }
         }
      }

}
/******************************************************************************/
void WriteAcAccelToCsv(void)
{
      struct AcType *AC;
      static FILE **outfile;
      char FileName[80];
      long Isc;
      long k;
      static long First = 1;
      static long OutCtr = 1000000000;

      if (First) {
         First = 0;
         outfile = (FILE**) calloc(Nsc,sizeof(FILE *));
         for(Isc=0;Isc<Nsc;Isc++) {
            if (SC[Isc].Exists) {
               AC = &SC[Isc].AC;
               if (Nsc==1) sprintf(FileName,"AcAccel.csv");
               else if (Nsc<=10) sprintf(FileName,"AcAccel%1ld.csv",Isc);
               else sprintf(FileName,"AcAccel%02ld.csv",Isc);
               outfile[Isc] = FileOpen(InOutPath,FileName,"w");

               fprintf(outfile[Isc],"AcAccel_Time");
               for(k=0;k<AC->Nacc;k++) {
                  fprintf(outfile[Isc],",Ac_Accel%ld_Acc",k);
               }
               fprintf(outfile[Isc],"\n");
            }
         }
      }

      OutCtr++;
      if (OutCtr > MaxAcAccelCtr) {
         OutCtr = 1;
         for (Isc=0;Isc<Nsc;Isc++) {
            if (SC[Isc].Exists) {
               AC = &SC[Isc].AC;
               fprintf(outfile[Isc],"%18.12le",AC->Time);
               for(k=0;k<AC->Nacc;k++) {
                  fprintf(outfile[Isc],",%18.12le",AC->Accel[k].Acc);
               }
               fprintf(outfile[Isc],"\n");
            }
         }
      }

}
/******************************************************************************/
void WriteAcWhlToCsv(void)
{
      struct AcType *AC;
      static FILE **outfile;
      char FileName[80];
      long Isc;
      long k;
      static long First = 1;
      static long OutCtr = 1000000000;

      if (First) {
         First = 0;
         outfile = (FILE**) calloc(Nsc,sizeof(FILE *));
         for(Isc=0;Isc<Nsc;Isc++) {
            if (SC[Isc].Exists) {
               AC = &SC[Isc].AC;
               if (Nsc==1) sprintf(FileName,"AcWhl.csv");
               else if (Nsc<=10) sprintf(FileName,"AcWhl%1ld.csv",Isc);
               else sprintf(FileName,"AcWhl%02ld.csv",Isc);
               outfile[Isc] = FileOpen(InOutPath,FileName,"w");

               fprintf(outfile[Isc],"AcWhl_Time");
               for(k=0;k<AC->Nwhl;k++) {
                  fprintf(outfile[Isc],",Ac_Whl%ld_H",k);
               }
               for(k=0;k<AC->Nwhl;k++) {
                  fprintf(outfile[Isc],",Ac_Whl%ld_Tcmd",k);
               }
               fprintf(outfile[Isc],"\n");
            }
         }
      }

      OutCtr++;
      if (OutCtr > MaxAcWhlCtr) {
         OutCtr = 1;
         for (Isc=0;Isc<Nsc;Isc++) {
            if (SC[Isc].Exists) {
               AC = &SC[Isc].AC;
               fprintf(outfile[Isc],"%18.12le",AC->Time);
               for(k=0;k<AC->Nwhl;k++) {
                  fprintf(outfile[Isc],",%18.12le",AC->Whl[k].H);
               }
               for(k=0;k<AC->Nwhl;k++) {
                  fprintf(outfile[Isc],",%18.12le",AC->Whl[k].Tcmd);
               }
               fprintf(outfile[Isc],"\n");
            }
         }
      }

}
/******************************************************************************/
void WriteAcMTBToCsv(void)
{
      struct AcType *AC;
      static FILE **outfile;
      char FileName[80];
      long Isc;
      long k;
      static long First = 1;
      static long OutCtr = 1000000000;

      if (First) {
         First = 0;
         outfile = (FILE**) calloc(Nsc,sizeof(FILE *));
         for(Isc=0;Isc<Nsc;Isc++) {
            if (SC[Isc].Exists) {
               AC = &SC[Isc].AC;
               if (Nsc==1) sprintf(FileName,"AcMTB.csv");
               else if (Nsc<=10) sprintf(FileName,"AcMTB%1ld.csv",Isc);
               else sprintf(FileName,"AcMTB%02ld.csv",Isc);
               outfile[Isc] = FileOpen(InOutPath,FileName,"w");

               fprintf(outfile[Isc],"AcMTB_Time");
               for(k=0;k<AC->Nmtb;k++) {
                  fprintf(outfile[Isc],",Ac_MTB%ld_Mcmd",k);
               }
               fprintf(outfile[Isc],"\n");
            }
         }
      }

      OutCtr++;
      if (OutCtr > MaxAcMTBCtr) {
         OutCtr = 1;
         for (Isc=0;Isc<Nsc;Isc++) {
            if (SC[Isc].Exists) {
               AC = &SC[Isc].AC;
               fprintf(outfile[Isc],"%18.12le",AC->Time);
               for(k=0;k<AC->Nmtb;k++) {
                  fprintf(outfile[Isc],",%18.12le",AC->MTB[k].Mcmd);
               }
               fprintf(outfile[Isc],"\n");
            }
         }
      }

}
/******************************************************************************/
void WriteAcThrToCsv(void)
{
      struct AcType *AC;
      static FILE **outfile;
      char FileName[80];
      long Isc;
      long k;
      static long First = 1;
      static long OutCtr = 1000000000;

      if (First) {
         First = 0;
         outfile = (FILE**) calloc(Nsc,sizeof(FILE *));
         for(Isc=0;Isc<Nsc;Isc++) {
            if (SC[Isc].Exists) {
               AC = &SC[Isc].AC;
               if (Nsc==1) sprintf(FileName,"AcThr.csv");
               else if (Nsc<=10) sprintf(FileName,"AcThr%1ld.csv",Isc);
               else sprintf(FileName,"AcThr%02ld.csv",Isc);
               outfile[Isc] = FileOpen(InOutPath,FileName,"w");

               fprintf(outfile[Isc],"AcThr_Time");
               for(k=0;k<AC->Nthr;k++) {
                  fprintf(outfile[Isc],",Ac_Thr%ld_PulseWidthCmd",k);
               }
               for(k=0;k<AC->Nthr;k++) {
                  fprintf(outfile[Isc],",Ac_Thr%ld_ThrustLevelCmd",k);
               }
               fprintf(outfile[Isc],"\n");
            }
         }
      }

      OutCtr++;
      if (OutCtr > MaxAcThrCtr) {
         OutCtr = 1;
         for (Isc=0;Isc<Nsc;Isc++) {
            if (SC[Isc].Exists) {
               AC = &SC[Isc].AC;
               fprintf(outfile[Isc],"%18.12le",AC->Time);
               for(k=0;k<AC->Nthr;k++) {
                  fprintf(outfile[Isc],",%18.12le",AC->Thr[k].PulseWidthCmd);
               }
               for(k=0;k<AC->Nthr;k++) {
                  fprintf(outfile[Isc],",%18.12le",AC->Thr[k].ThrustLevelCmd);
               }
               fprintf(outfile[Isc],"\n");
            }
         }
      }

}
/******************************************************************************/
void WriteAcCmdToCsv(void)
{
      struct AcType *AC;
      static FILE **outfile;
      char FileName[80];
      long Isc;
      long i;
      static long First = 1;
      static long OutCtr = 1000000000;

      if (First) {
         First = 0;
         outfile = (FILE**) calloc(Nsc,sizeof(FILE *));
         for(Isc=0;Isc<Nsc;Isc++) {
            if (SC[Isc].Exists) {
               AC = &SC[Isc].AC;
               if (Nsc==1) sprintf(FileName,"AcCmd.csv");
               else if (Nsc<=10) sprintf(FileName,"AcCmd%1ld.csv",Isc);
               else sprintf(FileName,"AcCmd%02ld.csv",Isc);
               outfile[Isc] = FileOpen(InOutPath,FileName,"w");

               fprintf(outfile[Isc],"AcCmd_Time");
               for(i=0;i<4;i++) {
                  fprintf(outfile[Isc],",Ac_Cmdqrl_%ld",i);
               }
               for(i=0;i<4;i++) {
                  fprintf(outfile[Isc],",Ac_Cmdqrn_%ld",i);
               }
               for(i=0;i<3;i++) {
                  fprintf(outfile[Isc],",Ac_CmdAngRate_%ld",i);
               }
               for(i=0;i<3;i++) {
                  fprintf(outfile[Isc],",Ac_CmdAng_%ld",i);
               }
               for(i=0;i<3;i++) {
                  fprintf(outfile[Isc],",Ac_CmdPosRate_%ld",i);
               }
               for(i=0;i<3;i++) {
                  fprintf(outfile[Isc],",Ac_CmdPos_%ld",i);
               }
               fprintf(outfile[Isc],"\n");
            }
         }
      }

      OutCtr++;
      if (OutCtr > MaxAcCmdCtr) {
         OutCtr = 1;
         for (Isc=0;Isc<Nsc;Isc++) {
            if (SC[Isc].Exists) {
               AC = &SC[Isc].AC;
               fprintf(outfile[Isc],"%18.12le",AC->Time);
               for(i=0;i<4;i++) {
                  fprintf(outfile[Isc],",%18.12le",AC->Cmd.qrl[i]);
               }
               for(i=0;i<4;i++) {
                  fprintf(outfile[Isc],",%18.12le",AC->Cmd.qrn[i]);
               }
               for(i=0;i<3;i++) {
                  fprintf(outfile[Isc],",%18.12le",AC->Cmd.AngRate[i]);
               }
               for(i=0;i<3;i++) {
                  fprintf(outfile[Isc],",%18.12le",AC->Cmd.Ang[i]);
               }
               for(i=0;i<3;i++) {
                  fprintf(outfile[Isc],",%18.12le",AC->Cmd.PosRate[i]);
               }
               for(i=0;i<3;i++) {
                  fprintf(outfile[Isc],",%18.12le",AC->Cmd.Pos[i]);
               }
               fprintf(outfile[Isc],"\n");
            }
         }
      }

}
/******************************************************************************/
void WriteAcVarsToCsv(void)
{
      FILE *infile;
      char response[80],junk[80],newline;
      double dt;
      static long First = 1;

      if (First) {
         First = 0;

         infile = FileOpen(InOutPath,"Inp_AcOutput.txt","r");
         fscanf(infile,"%[^\n] %[\n]",junk,&newline);

         fscanf(infile,"%s %lf %[^\n] %[\n]",response,&dt,junk,&newline);
         AcTlmEnabled = DecodeString(response);
         if (dt < DTSIM) {
            printf("Ac timestep < DTSIM.  You'll want to fix that.");
            exit(1);
         }
         else MaxAcCtr = (long) (dt/DTSIM+0.5);

         fscanf(infile,"%s %lf %[^\n] %[\n]",response,&dt,junk,&newline);
         AcGTlmEnabled = DecodeString(response);
         if (dt < DTSIM) {
            printf("AcG timestep < DTSIM.  You'll want to fix that.");
            exit(1);
         }
         else MaxAcGCtr = (long) (dt/DTSIM+0.5);

         fscanf(infile,"%s %lf %[^\n] %[\n]",response,&dt,junk,&newline);
         Ac_GCmdTlmEnabled = DecodeString(response);
         if (dt < DTSIM) {
            printf("Ac_GCmd timestep < DTSIM.  You'll want to fix that.");
            exit(1);
         }
         else MaxAc_GCmdCtr = (long) (dt/DTSIM+0.5);

         fscanf(infile,"%s %lf %[^\n] %[\n]",response,&dt,junk,&newline);
         AcGyroTlmEnabled = DecodeString(response);
         if (dt < DTSIM) {
            printf("AcGyro timestep < DTSIM.  You'll want to fix that.");
            exit(1);
         }
         else MaxAcGyroCtr = (long) (dt/DTSIM+0.5);

         fscanf(infile,"%s %lf %[^\n] %[\n]",response,&dt,junk,&newline);
         AcMAGTlmEnabled = DecodeString(response);
         if (dt < DTSIM) {
            printf("AcMAG timestep < DTSIM.  You'll want to fix that.");
            exit(1);
         }
         else MaxAcMAGCtr = (long) (dt/DTSIM+0.5);

         fscanf(infile,"%s %lf %[^\n] %[\n]",response,&dt,junk,&newline);
         AcCSSTlmEnabled = DecodeString(response);
         if (dt < DTSIM) {
            printf("AcCSS timestep < DTSIM.  You'll want to fix that.");
            exit(1);
         }
         else MaxAcCSSCtr = (long) (dt/DTSIM+0.5);

         fscanf(infile,"%s %lf %[^\n] %[\n]",response,&dt,junk,&newline);
         AcFSSTlmEnabled = DecodeString(response);
         if (dt < DTSIM) {
            printf("AcFSS timestep < DTSIM.  You'll want to fix that.");
            exit(1);
         }
         else MaxAcFSSCtr = (long) (dt/DTSIM+0.5);

         fscanf(infile,"%s %lf %[^\n] %[\n]",response,&dt,junk,&newline);
         AcSTTlmEnabled = DecodeString(response);
         if (dt < DTSIM) {
            printf("AcST timestep < DTSIM.  You'll want to fix that.");
            exit(1);
         }
         else MaxAcSTCtr = (long) (dt/DTSIM+0.5);

         fscanf(infile,"%s %lf %[^\n] %[\n]",response,&dt,junk,&newline);
         AcGPSTlmEnabled = DecodeString(response);
         if (dt < DTSIM) {
            printf("AcGPS timestep < DTSIM.  You'll want to fix that.");
            exit(1);
         }
         else MaxAcGPSCtr = (long) (dt/DTSIM+0.5);

         fscanf(infile,"%s %lf %[^\n] %[\n]",response,&dt,junk,&newline);
         AcAccelTlmEnabled = DecodeString(response);
         if (dt < DTSIM) {
            printf("AcAccel timestep < DTSIM.  You'll want to fix that.");
            exit(1);
         }
         else MaxAcAccelCtr = (long) (dt/DTSIM+0.5);

         fscanf(infile,"%s %lf %[^\n] %[\n]",response,&dt,junk,&newline);
         AcWhlTlmEnabled = DecodeString(response);
         if (dt < DTSIM) {
            printf("AcWhl timestep < DTSIM.  You'll want to fix that.");
            exit(1);
         }
         else MaxAcWhlCtr = (long) (dt/DTSIM+0.5);

         fscanf(infile,"%s %lf %[^\n] %[\n]",response,&dt,junk,&newline);
         AcMTBTlmEnabled = DecodeString(response);
         if (dt < DTSIM) {
            printf("AcMTB timestep < DTSIM.  You'll want to fix that.");
            exit(1);
         }
         else MaxAcMTBCtr = (long) (dt/DTSIM+0.5);

         fscanf(infile,"%s %lf %[^\n] %[\n]",response,&dt,junk,&newline);
         AcThrTlmEnabled = DecodeString(response);
         if (dt < DTSIM) {
            printf("AcThr timestep < DTSIM.  You'll want to fix that.");
            exit(1);
         }
         else MaxAcThrCtr = (long) (dt/DTSIM+0.5);

         fscanf(infile,"%s %lf %[^\n] %[\n]",response,&dt,junk,&newline);
         AcCmdTlmEnabled = DecodeString(response);
         if (dt < DTSIM) {
            printf("AcCmd timestep < DTSIM.  You'll want to fix that.");
            exit(1);
         }
         else MaxAcCmdCtr = (long) (dt/DTSIM+0.5);

      }

      if (AcTlmEnabled) WriteAcToCsv();
      if (AcGTlmEnabled) WriteAcGToCsv();
      if (Ac_GCmdTlmEnabled) WriteAc_GCmdToCsv();
      if (AcGyroTlmEnabled) WriteAcGyroToCsv();
      if (AcMAGTlmEnabled) WriteAcMAGToCsv();
      if (AcCSSTlmEnabled) WriteAcCSSToCsv();
      if (AcFSSTlmEnabled) WriteAcFSSToCsv();
      if (AcSTTlmEnabled) WriteAcSTToCsv();
      if (AcGPSTlmEnabled) WriteAcGPSToCsv();
      if (AcAccelTlmEnabled) WriteAcAccelToCsv();
      if (AcWhlTlmEnabled) WriteAcWhlToCsv();
      if (AcMTBTlmEnabled) WriteAcMTBToCsv();
      if (AcThrTlmEnabled) WriteAcThrToCsv();
      if (AcCmdTlmEnabled) WriteAcCmdToCsv();

}

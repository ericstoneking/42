/*    This file is distributed with 42,                               */
/*    the (mostly harmless) spacecraft dynamics simulation            */
/*    created by Eric Stoneking of NASA Goddard Space Flight Center   */

/*    Copyright 2010 United States Government                         */
/*    as represented by the Administrator                             */
/*    of the National Aeronautics and Space Administration.           */

/*    No copyright is claimed in the United States                    */
/*    under Title 17, U.S. Code.                                      */

/*    All Other Rights Reserved.                                      */


#include "42.h"
#include <time.h>
#include <unistd.h>

#ifdef _ENABLE_IPC_GMSEC_
   #include "gmsec4_c.h"
   GMSEC_Status status;
   GMSEC_Config cfg;
   GMSEC_ConnectionMgr ConnMgr;
   GMSEC_Message msg;
   GMSEC_Field Field;
#endif

/* #ifdef __cplusplus
** namespace _42 {
** using namespace Kit;
** #endif
*/

long Nmnem;
char **Mnemonic;
int Port;
char HostName[80];
FILE *statefile;

/*********************************************************************/
/*********************************************************************/
#ifdef _ENABLE_IPC_GMSEC_
/*********************************************************************/
void checkStatus(void)
{
      if (statusIsError(status) == GMSEC_TRUE) {
         GMSEC_ERROR("Error: %s\n", statusGet(status));
      }
}
/*********************************************************************/
void ConnectToMBServer(void)
{
      const char *name = NULL;
      const char *value = NULL;
      char PortString[80];

      status = statusCreate();
      cfg = configCreate();
      sprintf(PortString,"%d",Port);
      
      configAddValue(cfg,"connectiontype","gmsec_mb",NULL);
      configAddValue(cfg,"server",HostName,NULL);
      configAddValue(cfg,"port",PortString,NULL);
      configAddValue(cfg,"loglevel","info", NULL);
      
      configGetFirst(cfg,&name,&value,status);
      while(statusIsError(status) == GMSEC_FALSE) {
         printf("%s,%s\n",name,value);
         configGetNext(cfg,&name,&value,status);
      }
      
      ConnMgr = connectionManagerCreate(cfg, status);
      checkStatus();

      connectionManagerInitialize(ConnMgr, status);
      checkStatus();
      
}
/*********************************************************************/
void GmsecSend(const char *Header, const char *Text)
{
      msg = messageCreate(Header,GMSEC_PUBLISH,status);
      messageAddStringField(msg,"MSG-TEXT",Text,status);
      checkStatus();
      connectionManagerPublish(ConnMgr,msg,status);
      checkStatus();
      messageDestroy(&msg);
}
/*********************************************************************/
void ReadStatesFromGmsec(void)
{
      struct SCType *S;
      struct OrbitType *O;
      struct JointType *G;
      struct DynType *D;
      long IntVal1,IntVal2,IntVal3,IntVal4;
      double DblVal1,DblVal2,DblVal3,DblVal4,DblVal5,DblVal6;
      char line[512] = "Blank";
      char MnemString[80];
      long Isc,i;
      long RequestTimeRefresh = 0;
      long Done;

      /* Default SC to apply inputs to */
      Isc = 0;
      while(!SC[Isc].Exists) Isc++;
      S = &SC[Isc];
      O = &Orb[S->RefOrb];

      Done = 0;
      while(!Done) {
         msg = connectionManagerReceive(ConnMgr,GMSEC_WAIT_FOREVER,status);
         checkStatus();
         Field = messageGetField(msg,"MSG-TEXT",status);
         checkStatus();
         strcpy(line,stringFieldGetValue(Field,status));
         checkStatus();
         
         if (EchoEnabled) printf("%s",line);

         if (sscanf(line,"%s %ld-%ld-%ld:%ld:%lf",MnemString,
            &IntVal1,&IntVal2,&IntVal3,&IntVal4,&DblVal1) == 6) {
            if (!strcmp(MnemString,Mnemonic[MNEM_TIME])) {
               Year = IntVal1;
               doy = IntVal2;
               Hour = IntVal3;
               Minute = IntVal4;
               Second = DblVal1;
               RequestTimeRefresh = 1;
            }
         }
         if (sscanf(line,"%s %ld",MnemString,&IntVal1) == 2) {
            if (!strcmp(MnemString,Mnemonic[MNEM_SC])) {
               S = &SC[IntVal1];
               O = &Orb[S->RefOrb];
               S->RequestStateRefresh = 0;
            }
         }
         if (sscanf(line,"%s %lf %lf %lf",MnemString,
            &DblVal1,&DblVal2,&DblVal3) == 4) {
            if (!strcmp(MnemString,Mnemonic[MNEM_ORBPOS_N])) {
               O->PosN[0] = DblVal1;
               O->PosN[1] = DblVal2;
               O->PosN[2] = DblVal3;
               S->RequestStateRefresh = 1;
            }
            if (!strcmp(MnemString,Mnemonic[MNEM_ORBVEL_N])) {
               O->VelN[0] = DblVal1;
               O->VelN[1] = DblVal2;
               O->VelN[2] = DblVal3;
               S->RequestStateRefresh = 1;
            }
            if (!strcmp(MnemString,Mnemonic[MNEM_WBN])) {
               S->B[0].wn[0] = DblVal1;
               S->B[0].wn[1] = DblVal2;
               S->B[0].wn[2] = DblVal3;
               S->RequestStateRefresh = 1;
            }
            if (!strcmp(MnemString,Mnemonic[MNEM_SVB])) {
               S->AC.svb[0] = DblVal1;
               S->AC.svb[1] = DblVal2;
               S->AC.svb[2] = DblVal3;
            }
            if (!strcmp(MnemString,Mnemonic[MNEM_BVB])) {
               S->AC.bvb[0] = DblVal1;
               S->AC.bvb[1] = DblVal2;
               S->AC.bvb[2] = DblVal3;
            }
            if (!strcmp(MnemString,Mnemonic[MNEM_HVB])) {
               S->AC.Hvb[0] = DblVal1;
               S->AC.Hvb[1] = DblVal2;
               S->AC.Hvb[2] = DblVal3;
            }
            if (!strcmp(MnemString,Mnemonic[MNEM_POS_R])) {
               S->PosR[0] = DblVal1;
               S->PosR[1] = DblVal2;
               S->PosR[2] = DblVal3;
               S->RequestStateRefresh = 1;
            }
            if (!strcmp(MnemString,Mnemonic[MNEM_VEL_R])) {
               S->VelR[0] = DblVal1;
               S->VelR[1] = DblVal2;
               S->VelR[2] = DblVal3;
               S->RequestStateRefresh = 1;
            }
         }
         if (sscanf(line,"%s %lf %lf %lf %lf",MnemString,
            &DblVal1,&DblVal2,&DblVal3,&DblVal4) == 5) {
            if (!strcmp(MnemString,Mnemonic[MNEM_QBN])) {
               S->B[0].qn[0] = DblVal1;
               S->B[0].qn[1] = DblVal2;
               S->B[0].qn[2] = DblVal3;
               S->B[0].qn[3] = DblVal4;
               S->RequestStateRefresh = 1;
            }
         }
         if (sscanf(line,"%s %ld %lf %lf %lf %lf %lf %lf",MnemString,
            &IntVal1,&DblVal1,&DblVal2,&DblVal3,
            &DblVal4,&DblVal5,&DblVal6) == 8) {
            if (!strcmp(MnemString,Mnemonic[MNEM_JOINT])) {
               G = &S->G[IntVal1];
               G->Ang[0] = DblVal1;
               G->Ang[1] = DblVal2;
               G->Ang[2] = DblVal3;
               G->Pos[0] = DblVal4;
               G->Pos[1] = DblVal5;
               G->Pos[2] = DblVal6;
               S->RequestStateRefresh = 1;
            }
         }
         if (sscanf(line,"%s %ld %lf",MnemString,
            &IntVal1,&DblVal1) == 3) {
            if (!strcmp(MnemString,Mnemonic[MNEM_HWHL])) {
               S->Whl[IntVal1].H = DblVal1;
            }
         }
         if (!strncmp(line,"[EOF]",5)) Done = 1;

         connectionManagerRelease(ConnMgr,&msg,status);
         checkStatus();
      }
      
      /* Acknowledge receipt */
      sprintf(line,"Ack\n");
      GmsecSend("GMSEC.42.RX.MSG.LOG",line);
      
/* .. Refresh time, SC states that depend on inputs */

      if (RequestTimeRefresh) {
         /* Update AbsTime, SimTime, etc */
         DOY2MD(Year,doy,&Month,&Day);
         AbsTime = DateToAbsTime(Year,Month,Day,Hour,Minute,Second);
         JulDay = AbsTimeToJD(AbsTime);
         JDToGpsTime(JulDay,&GpsRollover,&GpsWeek,&GpsSecond);
         SimTime = AbsTime-AbsTime0;
      }

      for(Isc=0;Isc<Nsc;Isc++) {
         if (SC[Isc].RequestStateRefresh) {
            S = &SC[Isc];
            S->RequestStateRefresh = 0;
            if (S->Exists) {
               /* Update  RefOrb */
               O = &Orb[S->RefOrb];
               O->Epoch = AbsTime;
               for(i=0;i<3;i++) {
                  S->PosN[i] = O->PosN[i] + S->PosR[i];
                  S->VelN[i] = O->VelN[i] + S->VelR[i];
               }
               RV2Eph(O->Epoch,O->mu,O->PosN,O->VelN,
                  &O->SMA,&O->ecc,&O->inc,&O->RAAN,
                  &O->ArgP,&O->anom,&O->tp,
                  &O->SLR,&O->alpha,&O->rmin,
                  &O->MeanMotion,&O->Period);
               FindCLN(O->PosN,O->VelN,O->CLN,O->wln);

               /* Update Dyn */
               MapJointStatesToStateVector(S);
               D = &S->Dyn;
               MapStateVectorToBodyStates(D->u,D->x,D->uf,D->xf,S);
               MotionConstraints(S);
            }
         }
      }

}
/*********************************************************************/
void WriteStatesToGmsec(void)
{
      struct SCType *S;
      struct OrbitType *O;
      struct JointType *G;
      struct WhlType *W;
      double qnh[4],qwn[4];
      long Isc,Ig,Iw;
      char line[512];
      char Header[40] = "GMSEC.42.TX.MSG.LOG";

      sprintf(line,"%s  %ld-%03ld-%02ld:%02ld:%012.9lf\n",
         Mnemonic[MNEM_TIME],Year,doy,Hour,Minute,Second);
      GmsecSend(Header,line);
      if (EchoEnabled) printf("%s",line);

      for(Isc=0;Isc<Nsc;Isc++) {
         S = &SC[Isc];
         O = &Orb[S->RefOrb];
         
         sprintf(line,"%s %ld\n",
            Mnemonic[MNEM_SC], Isc);
         GmsecSend(Header,line);
         if (EchoEnabled) printf("%s",line);

         sprintf(line,"%s  %le %le %le\n",
            Mnemonic[MNEM_ORBPOS_N], O->PosN[0],O->PosN[1],O->PosN[2]);
         GmsecSend(Header,line);
         if (EchoEnabled) printf("%s",line);

         sprintf(line,"%s  %le %le %le\n",
            Mnemonic[MNEM_ORBVEL_N], O->VelN[0],O->VelN[1],O->VelN[2]);
         GmsecSend(Header,line);
         if (EchoEnabled) printf("%s",line);

         sprintf(line,"%s  %le %le %le\n",
            Mnemonic[MNEM_POS_R], S->PosR[0],S->PosR[1],S->PosR[2]);
         GmsecSend(Header,line);
         if (EchoEnabled) printf("%s",line);

         sprintf(line,"%s  %le %le %le\n",
            Mnemonic[MNEM_VEL_R], S->VelR[0],S->VelR[1],S->VelR[2]);
         GmsecSend(Header,line);
         if (EchoEnabled) printf("%s",line);

         sprintf(line,"%s  %le %le %le\n",
            Mnemonic[MNEM_POS_H], S->PosH[0],S->PosH[1],S->PosH[2]);
         GmsecSend(Header,line);
         if (EchoEnabled) printf("%s",line);

         sprintf(line,"%s  %le %le %le\n",
            Mnemonic[MNEM_WBN], S->B[0].wn[0],S->B[0].wn[1],S->B[0].wn[2]);
         GmsecSend(Header,line);
         if (EchoEnabled) printf("%s",line);

         sprintf(line,"%s  %le %le %le %le\n",
            Mnemonic[MNEM_QBN], S->B[0].qn[0],S->B[0].qn[1],
                                S->B[0].qn[2],S->B[0].qn[3]);
         GmsecSend(Header,line);
         if (EchoEnabled) printf("%s",line);

         sprintf(line,"%s  %le %le %le\n",
            Mnemonic[MNEM_SVB], S->svb[0],S->svb[1],S->svb[2]);
         GmsecSend(Header,line);
         if (EchoEnabled) printf("%s",line);

         sprintf(line,"%s  %le %le %le\n",
            Mnemonic[MNEM_BVB], S->bvb[0],S->bvb[1],S->bvb[2]);
         GmsecSend(Header,line);
         if (EchoEnabled) printf("%s",line);

         sprintf(line,"%s  %le %le %le\n",
            Mnemonic[MNEM_HVB], S->Hvb[0],S->Hvb[1],S->Hvb[2]);
         GmsecSend(Header,line);
         if (EchoEnabled) printf("%s",line);

         sprintf(line,"%s  %le %le %le\n",
            Mnemonic[MNEM_EARTH_POS_H], 
            World[EARTH].eph.PosN[0],
            World[EARTH].eph.PosN[1],
            World[EARTH].eph.PosN[2]);
         GmsecSend(Header,line);
         if (EchoEnabled) printf("%s",line);
         
         C2Q(World[EARTH].CNH,qnh);
         sprintf(line,"%s  %le %le %le %le\n",
            Mnemonic[MNEM_EARTH_QNH], 
            qnh[0],qnh[1],qnh[2],qnh[3]);
         GmsecSend(Header,line);
         if (EchoEnabled) printf("%s",line);         

         C2Q(World[EARTH].CWN,qwn);
         sprintf(line,"%s  %le %le %le %le\n",
            Mnemonic[MNEM_EARTH_QWN], 
            qwn[0],qwn[1],qwn[2],qwn[3]);
         GmsecSend(Header,line);
         if (EchoEnabled) printf("%s",line);         

         sprintf(line,"%s  %le %le %le\n",
            Mnemonic[MNEM_LUNA_POS_N], 
            World[LUNA].eph.PosN[0],
            World[LUNA].eph.PosN[1],
            World[LUNA].eph.PosN[2]);
         GmsecSend(Header,line);
         if (EchoEnabled) printf("%s",line);
         
         C2Q(World[LUNA].CNH,qnh);
         sprintf(line,"%s  %le %le %le %le\n",
            Mnemonic[MNEM_LUNA_QNH], 
            qnh[0],qnh[1],qnh[2],qnh[3]);
         GmsecSend(Header,line);
         if (EchoEnabled) printf("%s",line);         

         C2Q(World[LUNA].CWN,qwn);
         sprintf(line,"%s  %le %le %le %le\n",
            Mnemonic[MNEM_LUNA_QWN], 
            qwn[0],qwn[1],qwn[2],qwn[3]);
         GmsecSend(Header,line);
         if (EchoEnabled) printf("%s",line);         

         for(Ig=0;Ig<S->Ng;Ig++) {
            G = &S->G[Ig];
            sprintf(line,"%s  %ld %le %le %le %le %le %le\n",
               Mnemonic[MNEM_JOINT],Ig,G->Ang[0],G->Ang[1],G->Ang[2],
               G->Pos[0],G->Pos[1],G->Pos[2]);
            GmsecSend(Header,line);
            if (EchoEnabled) printf("%s",line);
         }

         for(Iw=0;Iw<S->Nw;Iw++) {
            W = &S->Whl[Iw];
            sprintf(line,"%s  %ld %le\n",
               Mnemonic[MNEM_HWHL],Iw,W->H);
            GmsecSend(Header,line);
            if (EchoEnabled) printf("%s",line);
         }
      }
      sprintf(line,"[EOF]\n\n");
      GmsecSend(Header,line);
      if (EchoEnabled) printf("%s",line);

      /* Wait for ack */
      msg = connectionManagerReceive(ConnMgr,GMSEC_WAIT_FOREVER,status);
      checkStatus();
      connectionManagerRelease(ConnMgr,&msg,status);
      checkStatus();
}
#endif
/*********************************************************************/
/*********************************************************************/
void WriteStatesToFile(void)
{
      struct SCType *S;
      struct OrbitType *O;
      struct JointType *G;
      struct WhlType *W;
      long Isc,Ig,Iw;
      char line[512];

      sprintf(line,"%s  %ld-%03ld-%02ld:%02ld:%012.9lf\n",
         Mnemonic[MNEM_TIME],Year,doy,Hour,Minute,Second);
      fprintf(statefile,"%s",line);
      if (EchoEnabled) printf("%s",line);

      for(Isc=0;Isc<Nsc;Isc++) {
         S = &SC[Isc];
         O = &Orb[S->RefOrb];
         if (S->Exists) {
            sprintf(line,"%s %ld\n",
               Mnemonic[MNEM_SC], Isc);
            fprintf(statefile,"%s",line);
            if (EchoEnabled) printf("%s",line);

            sprintf(line,"%s  %le %le %le\n",
               Mnemonic[MNEM_ORBPOS_N], O->PosN[0],O->PosN[1],O->PosN[2]);
            fprintf(statefile,"%s",line);
            if (EchoEnabled) printf("%s",line);

            sprintf(line,"%s  %le %le %le\n",
               Mnemonic[MNEM_ORBVEL_N], O->VelN[0],O->VelN[1],O->VelN[2]);
            fprintf(statefile,"%s",line);
            if (EchoEnabled) printf("%s",line);

            sprintf(line,"%s  %le %le %le\n",
               Mnemonic[MNEM_POS_R], S->PosR[0],S->PosR[1],S->PosR[2]);
            fprintf(statefile,"%s",line);
            if (EchoEnabled) printf("%s",line);

            sprintf(line,"%s  %le %le %le\n",
               Mnemonic[MNEM_VEL_R], S->VelR[0],S->VelR[1],S->VelR[2]);
            fprintf(statefile,"%s",line);
            if (EchoEnabled) printf("%s",line);

            sprintf(line,"%s  %le %le %le\n",
               Mnemonic[MNEM_WBN], S->B[0].wn[0],S->B[0].wn[1],S->B[0].wn[2]);
            fprintf(statefile,"%s",line);
            if (EchoEnabled) printf("%s",line);

            sprintf(line,"%s  %le %le %le %le\n",
               Mnemonic[MNEM_QBN], S->B[0].qn[0],S->B[0].qn[1],
                                   S->B[0].qn[2],S->B[0].qn[3]);
            fprintf(statefile,"%s",line);
            if (EchoEnabled) printf("%s",line);

            sprintf(line,"%s  %le %le %le\n",
               Mnemonic[MNEM_SVB], S->svb[0],S->svb[1],S->svb[2]);
            fprintf(statefile,"%s",line);
            if (EchoEnabled) printf("%s",line);

            sprintf(line,"%s  %le %le %le\n",
               Mnemonic[MNEM_BVB], S->bvb[0],S->bvb[1],S->bvb[2]);
            fprintf(statefile,"%s",line);
            if (EchoEnabled) printf("%s",line);

            sprintf(line,"%s  %le %le %le\n",
               Mnemonic[MNEM_HVB], S->Hvb[0],S->Hvb[1],S->Hvb[2]);
            fprintf(statefile,"%s",line);
            if (EchoEnabled) printf("%s",line);

            for(Ig=0;Ig<S->Ng;Ig++) {
               G = &S->G[Ig];
               sprintf(line,"%s  %ld %le %le %le %le %le %le\n",
                  Mnemonic[MNEM_JOINT],Ig,G->Ang[0],G->Ang[1],G->Ang[2],
                  G->Pos[0],G->Pos[1],G->Pos[2]);
               fprintf(statefile,"%s",line);
               if (EchoEnabled) printf("%s",line);
            }

            for(Iw=0;Iw<S->Nw;Iw++) {
               W = &S->Whl[Iw];
               sprintf(line,"%s  %ld %le\n",
                  Mnemonic[MNEM_HWHL],Iw,W->H);
               fprintf(statefile,"%s",line);
               if (EchoEnabled) printf("%s",line);
            }
         }
      }
      sprintf(line,"[EOL]\n\n");
      fprintf(statefile,"%s",line);
      if (EchoEnabled) printf("%s",line);

}
/*********************************************************************/
void ReadStatesFromFile(void)
{
      struct SCType *S;
      struct OrbitType *O;
      struct JointType *G;
      struct DynType *D;
      long IntVal1,IntVal2,IntVal3,IntVal4;
      double DblVal1,DblVal2,DblVal3,DblVal4,DblVal5,DblVal6;
      char line[512] = "Blank";
      char MnemString[80];
      long Isc,i;
      long RequestTimeRefresh = 0;

      /* Default SC to apply inputs to */
      Isc = 0;
      while(!SC[Isc].Exists) Isc++;
      S = &SC[Isc];

      while (1) {
         if (fgets(line,511,statefile) == NULL) break;
         if (strncmp(line,"[EOL]",5) == 0) break;
         if (EchoEnabled) printf("%s",line);
         if (sscanf(line,"%s %ld-%ld-%ld:%ld:%lf",MnemString,
            &IntVal1,&IntVal2,&IntVal3,&IntVal4,&DblVal1) == 6) {
            if (!strcmp(MnemString,Mnemonic[MNEM_TIME])) {
               Year = IntVal1;
               doy = IntVal2;
               Hour = IntVal3;
               Minute = IntVal4;
               Second = DblVal1;
               RequestTimeRefresh = 1;
            }
         }
         if (sscanf(line,"%s %ld",MnemString,&IntVal1) == 2) {
            if (!strcmp(MnemString,Mnemonic[MNEM_SC])) {
               S = &SC[IntVal1];
               O = &Orb[S->RefOrb];
               S->RequestStateRefresh = 0;
            }
         }
         if (sscanf(line,"%s %lf %lf %lf",MnemString,
            &DblVal1,&DblVal2,&DblVal3) == 4) {
            if (!strcmp(MnemString,Mnemonic[MNEM_ORBPOS_N])) {
               O->PosN[0] = DblVal1;
               O->PosN[1] = DblVal2;
               O->PosN[2] = DblVal3;
               S->RequestStateRefresh = 1;
            }
            if (!strcmp(MnemString,Mnemonic[MNEM_ORBVEL_N])) {
               O->VelN[0] = DblVal1;
               O->VelN[1] = DblVal2;
               O->VelN[2] = DblVal3;
               S->RequestStateRefresh = 1;
            }
            if (!strcmp(MnemString,Mnemonic[MNEM_POS_R])) {
               S->PosR[0] = DblVal1;
               S->PosR[1] = DblVal2;
               S->PosR[2] = DblVal3;
               S->RequestStateRefresh = 1;
            }
            if (!strcmp(MnemString,Mnemonic[MNEM_VEL_R])) {
               S->VelR[0] = DblVal1;
               S->VelR[1] = DblVal2;
               S->VelR[2] = DblVal3;
               S->RequestStateRefresh = 1;
            }
            if (!strcmp(MnemString,Mnemonic[MNEM_WBN])) {
               S->B[0].wn[0] = DblVal1;
               S->B[0].wn[1] = DblVal2;
               S->B[0].wn[2] = DblVal3;
               S->RequestStateRefresh = 1;
            }
            if (!strcmp(MnemString,Mnemonic[MNEM_SVB])) {
               S->AC.svb[0] = DblVal1;
               S->AC.svb[1] = DblVal2;
               S->AC.svb[2] = DblVal3;
            }
            if (!strcmp(MnemString,Mnemonic[MNEM_BVB])) {
               S->AC.bvb[0] = DblVal1;
               S->AC.bvb[1] = DblVal2;
               S->AC.bvb[2] = DblVal3;
            }
            if (!strcmp(MnemString,Mnemonic[MNEM_HVB])) {
               S->AC.Hvb[0] = DblVal1;
               S->AC.Hvb[1] = DblVal2;
               S->AC.Hvb[2] = DblVal3;
            }
         }
         if (sscanf(line,"%s %lf %lf %lf %lf",MnemString,
            &DblVal1,&DblVal2,&DblVal3,&DblVal4) == 5) {
            if (!strcmp(MnemString,Mnemonic[MNEM_QBN])) {
               S->B[0].qn[0] = DblVal1;
               S->B[0].qn[1] = DblVal2;
               S->B[0].qn[2] = DblVal3;
               S->B[0].qn[3] = DblVal4;
               S->RequestStateRefresh = 1;
            }
         }
         if (sscanf(line,"%s %ld %lf %lf %lf %lf %lf %lf",MnemString,
            &IntVal1,&DblVal1,&DblVal2,&DblVal3,
            &DblVal4,&DblVal5,&DblVal6) == 8) {
            if (!strcmp(MnemString,Mnemonic[MNEM_JOINT])) {
               G = &S->G[IntVal1];
               G->Ang[0] = DblVal1;
               G->Ang[1] = DblVal2;
               G->Ang[2] = DblVal3;
               G->Pos[0] = DblVal4;
               G->Pos[1] = DblVal5;
               G->Pos[2] = DblVal6;
               S->RequestStateRefresh = 1;
            }
         }
         if (sscanf(line,"%s %ld %lf",MnemString,
            &IntVal1,&DblVal1) == 3) {
            if (!strcmp(MnemString,Mnemonic[MNEM_HWHL])) {
               S->Whl[IntVal1].H = DblVal1;
            }
         }

      }

/* .. Refresh time, SC states that depend on inputs */

      if (RequestTimeRefresh) {
         /* Update AbsTime, SimTime, etc */
         DOY2MD(Year,doy,&Month,&Day);
         AbsTime = DateToAbsTime(Year,Month,Day,Hour,Minute,Second);
         JulDay = AbsTimeToJD(AbsTime);
         JDToGpsTime(JulDay,&GpsRollover,&GpsWeek,&GpsSecond);
         SimTime = AbsTime-AbsTime0;
      }

      for(Isc=0;Isc<Nsc;Isc++) {
         if (SC[Isc].RequestStateRefresh) {
            S = &SC[Isc];
            S->RequestStateRefresh = 0;
            if (S->Exists) {
               /* Update  RefOrb */
               O = &Orb[S->RefOrb];
               O->Epoch = AbsTime;
               for(i=0;i<3;i++) {
                  S->PosN[i] = O->PosN[i] + S->PosR[i];
                  S->VelN[i] = O->VelN[i] + S->VelR[i];
               }
               RV2Eph(O->Epoch,O->mu,O->PosN,O->VelN,
                  &O->SMA,&O->ecc,&O->inc,&O->RAAN,
                  &O->ArgP,&O->anom,&O->tp,
                  &O->SLR,&O->alpha,&O->rmin,
                  &O->MeanMotion,&O->Period);
               FindCLN(O->PosN,O->VelN,O->CLN,O->wln);

               /* Update Dyn */
               MapJointStatesToStateVector(S);
               D = &S->Dyn;
               MapStateVectorToBodyStates(D->u,D->x,D->uf,D->xf,S);
               MotionConstraints(S);
            }
         }
      }

}
/*********************************************************************/
void ReadStatesFromSocket(void)
{
      struct SCType *S;
      struct OrbitType *O;
      struct JointType *G;
      struct DynType *D;
      long IntVal1,IntVal2,IntVal3,IntVal4;
      double DblVal1,DblVal2,DblVal3,DblVal4,DblVal5,DblVal6;
      char line[256] = "Blank";
      char MnemString[80];
      long Isc,i;
      long RequestTimeRefresh = 0;
      long Done;
      char SocketMessage[4096];
      long Imsg,Iline;
      long MsgLen = 0;
      
      /* Default SC to apply inputs to */
      Isc = 0;
      while(!SC[Isc].Exists) Isc++;
      S = &SC[Isc];

      /* Messages occasionally get split up in transit.  Make sure you */
      /* have the whole message by watching for [EOF] */
      while(MsgLen < 7 || strncmp("[EOF]",&SocketMessage[MsgLen-7],5)) {
         MsgLen += recv(RxSocket,&SocketMessage[MsgLen],4096,0);
      }
      
      Done = 0;
      Imsg = 0;
      while(!Done) {
         /* Parse lines from SocketMessage, newline-delimited */
         Iline = 0;
         memset(line,'\0',256);
         while(SocketMessage[Imsg] != '\n') {
            line[Iline++] = SocketMessage[Imsg++];
         }
         line[Iline++] = SocketMessage[Imsg++];
         if (EchoEnabled) printf("%s",line);

         if (sscanf(line,"%s %ld-%ld-%ld:%ld:%lf",MnemString,
            &IntVal1,&IntVal2,&IntVal3,&IntVal4,&DblVal1) == 6) {
            if (!strcmp(MnemString,Mnemonic[MNEM_TIME])) {
               Year = IntVal1;
               doy = IntVal2;
               Hour = IntVal3;
               Minute = IntVal4;
               Second = DblVal1;
               RequestTimeRefresh = 1;
            }
         }
         if (sscanf(line,"%s %ld",MnemString,&IntVal1) == 2) {
            if (!strcmp(MnemString,Mnemonic[MNEM_SC])) {
               S = &SC[IntVal1];
               O = &Orb[S->RefOrb];
               S->RequestStateRefresh = 0;
            }
         }
         if (sscanf(line,"%s %lf %lf %lf",MnemString,
            &DblVal1,&DblVal2,&DblVal3) == 4) {
            if (!strcmp(MnemString,Mnemonic[MNEM_ORBPOS_N])) {
               O->PosN[0] = DblVal1;
               O->PosN[1] = DblVal2;
               O->PosN[2] = DblVal3;
               S->RequestStateRefresh = 1;
            }
            if (!strcmp(MnemString,Mnemonic[MNEM_ORBVEL_N])) {
               O->VelN[0] = DblVal1;
               O->VelN[1] = DblVal2;
               O->VelN[2] = DblVal3;
               S->RequestStateRefresh = 1;
            }
            if (!strcmp(MnemString,Mnemonic[MNEM_POS_R])) {
               S->PosR[0] = DblVal1;
               S->PosR[1] = DblVal2;
               S->PosR[2] = DblVal3;
               S->RequestStateRefresh = 1;
            }
            if (!strcmp(MnemString,Mnemonic[MNEM_VEL_R])) {
               S->VelR[0] = DblVal1;
               S->VelR[1] = DblVal2;
               S->VelR[2] = DblVal3;
               S->RequestStateRefresh = 1;
            }
            if (!strcmp(MnemString,Mnemonic[MNEM_WBN])) {
               S->B[0].wn[0] = DblVal1;
               S->B[0].wn[1] = DblVal2;
               S->B[0].wn[2] = DblVal3;
               S->RequestStateRefresh = 1;
            }
            if (!strcmp(MnemString,Mnemonic[MNEM_SVB])) {
               S->AC.svb[0] = DblVal1;
               S->AC.svb[1] = DblVal2;
               S->AC.svb[2] = DblVal3;
            }
            if (!strcmp(MnemString,Mnemonic[MNEM_BVB])) {
               S->AC.bvb[0] = DblVal1;
               S->AC.bvb[1] = DblVal2;
               S->AC.bvb[2] = DblVal3;
            }
            if (!strcmp(MnemString,Mnemonic[MNEM_HVB])) {
               S->AC.Hvb[0] = DblVal1;
               S->AC.Hvb[1] = DblVal2;
               S->AC.Hvb[2] = DblVal3;
            }
         }
         if (sscanf(line,"%s %lf %lf %lf %lf",MnemString,
            &DblVal1,&DblVal2,&DblVal3,&DblVal4) == 5) {
            if (!strcmp(MnemString,Mnemonic[MNEM_QBN])) {
               S->B[0].qn[0] = DblVal1;
               S->B[0].qn[1] = DblVal2;
               S->B[0].qn[2] = DblVal3;
               S->B[0].qn[3] = DblVal4;
               S->RequestStateRefresh = 1;
            }
         }
         if (sscanf(line,"%s %ld %lf %lf %lf %lf %lf %lf",MnemString,
            &IntVal1,&DblVal1,&DblVal2,&DblVal3,
            &DblVal4,&DblVal5,&DblVal6) == 8) {
            if (!strcmp(MnemString,Mnemonic[MNEM_JOINT])) {
               G = &S->G[IntVal1];
               G->Ang[0] = DblVal1;
               G->Ang[1] = DblVal2;
               G->Ang[2] = DblVal3;
               G->Pos[0] = DblVal4;
               G->Pos[1] = DblVal5;
               G->Pos[2] = DblVal6;
               S->RequestStateRefresh = 1;
            }
         }
         if (sscanf(line,"%s %ld %lf",MnemString,
            &IntVal1,&DblVal1) == 3) {
            if (!strcmp(MnemString,Mnemonic[MNEM_HWHL])) {
               S->Whl[IntVal1].H = DblVal1;
            }
         }
         if (!strncmp(line,"[EOF]",5)) {
            Done = 1;
            sprintf(line,"[EOF] reached\n");
         }
         if (Imsg > 4095) {
            Done = 1;
            printf("Imsg limit exceeded\n");
         }
      }
      /* Acknowledge receipt */
      sprintf(line,"Ack\n");
      send(RxSocket,line,strlen(line),0);

/* .. Refresh time, SC states that depend on inputs */

      if (RequestTimeRefresh) {
         /* Update AbsTime, SimTime, etc */
         DOY2MD(Year,doy,&Month,&Day);
         AbsTime = DateToAbsTime(Year,Month,Day,Hour,Minute,Second);
         JulDay = AbsTimeToJD(AbsTime);
         JDToGpsTime(JulDay,&GpsRollover,&GpsWeek,&GpsSecond);
         SimTime = AbsTime-AbsTime0;
      }

      for(Isc=0;Isc<Nsc;Isc++) {
         if (SC[Isc].RequestStateRefresh) {
            S = &SC[Isc];
            S->RequestStateRefresh = 0;
            if (S->Exists) {
               /* Update  RefOrb */
               O = &Orb[S->RefOrb];
               O->Epoch = AbsTime;
               for(i=0;i<3;i++) {
                  S->PosN[i] = O->PosN[i] + S->PosR[i];
                  S->VelN[i] = O->VelN[i] + S->VelR[i];
               }
               RV2Eph(O->Epoch,O->mu,O->PosN,O->VelN,
                  &O->SMA,&O->ecc,&O->inc,&O->RAAN,
                  &O->ArgP,&O->anom,&O->tp,
                  &O->SLR,&O->alpha,&O->rmin,
                  &O->MeanMotion,&O->Period);
               FindCLN(O->PosN,O->VelN,O->CLN,O->wln);

               /* Update Dyn */
               MapJointStatesToStateVector(S);
               D = &S->Dyn;
               MapStateVectorToBodyStates(D->u,D->x,D->uf,D->xf,S);
               MotionConstraints(S);
            }
         }
      }

}
/*********************************************************************/
void WriteStatesToSocket(void)
{
      struct SCType *S;
      struct OrbitType *O;
      struct JointType *G;
      struct WhlType *W;
      long Isc,Ig,Iw;
      char line[512];
      int Success;

      sprintf(line,"%s  %ld-%03ld-%02ld:%02ld:%012.9lf\n",
         Mnemonic[MNEM_TIME],Year,doy,Hour,Minute,Second);
      Success = send(TxSocket,line,strlen(line),0);
      if (EchoEnabled) printf("%s",line);

      for(Isc=0;Isc<Nsc;Isc++) {
         S = &SC[Isc];
         O = &Orb[S->RefOrb];
         sprintf(line,"%s %ld\n",
            Mnemonic[MNEM_SC], Isc);
         Success = send(TxSocket,line,strlen(line),0);
         if (EchoEnabled) printf("%s",line);

         sprintf(line,"%s  %le %le %le\n",
            Mnemonic[MNEM_ORBPOS_N], O->PosN[0],O->PosN[1],O->PosN[2]);
         Success = send(TxSocket,line,strlen(line),0);
         if (EchoEnabled) printf("%s",line);

         sprintf(line,"%s  %le %le %le\n",
            Mnemonic[MNEM_ORBVEL_N], O->VelN[0],O->VelN[1],O->VelN[2]);
         Success = send(TxSocket,line,strlen(line),0);
         if (EchoEnabled) printf("%s",line);

         sprintf(line,"%s  %le %le %le\n",
            Mnemonic[MNEM_POS_R], S->PosR[0],S->PosR[1],S->PosR[2]);
         Success = send(TxSocket,line,strlen(line),0);
         if (EchoEnabled) printf("%s",line);

         sprintf(line,"%s  %le %le %le\n",
            Mnemonic[MNEM_VEL_R], S->VelR[0],S->VelR[1],S->VelR[2]);
         Success = send(TxSocket,line,strlen(line),0);
         if (EchoEnabled) printf("%s",line);

         sprintf(line,"%s  %le %le %le\n",
            Mnemonic[MNEM_WBN], S->B[0].wn[0],S->B[0].wn[1],S->B[0].wn[2]);
         Success = send(TxSocket,line,strlen(line),0);
         if (EchoEnabled) printf("%s",line);

         sprintf(line,"%s  %le %le %le %le\n",
            Mnemonic[MNEM_QBN], S->B[0].qn[0],S->B[0].qn[1],
                                S->B[0].qn[2],S->B[0].qn[3]);
         Success = send(TxSocket,line,strlen(line),0);
         if (EchoEnabled) printf("%s",line);

         sprintf(line,"%s  %le %le %le\n",
            Mnemonic[MNEM_SVB], S->svb[0],S->svb[1],S->svb[2]);
         Success = send(TxSocket,line,strlen(line),0);
         if (EchoEnabled) printf("%s",line);

         sprintf(line,"%s  %le %le %le\n",
            Mnemonic[MNEM_BVB], S->bvb[0],S->bvb[1],S->bvb[2]);
         Success = send(TxSocket,line,strlen(line),0);
         if (EchoEnabled) printf("%s",line);

         sprintf(line,"%s  %le %le %le\n",
            Mnemonic[MNEM_HVB], S->Hvb[0],S->Hvb[1],S->Hvb[2]);
         Success = send(TxSocket,line,strlen(line),0);
         if (EchoEnabled) printf("%s",line);

         for(Ig=0;Ig<S->Ng;Ig++) {
            G = &S->G[Ig];
            sprintf(line,"%s  %ld %le %le %le %le %le %le\n",
               Mnemonic[MNEM_JOINT],Ig,G->Ang[0],G->Ang[1],G->Ang[2],
               G->Pos[0],G->Pos[1],G->Pos[2]);
            Success = send(TxSocket,line,strlen(line),0);
            if (EchoEnabled) printf("%s",line);
         }

         for(Iw=0;Iw<S->Nw;Iw++) {
            W = &S->Whl[Iw];
            sprintf(line,"%s  %ld %le\n",
               Mnemonic[MNEM_HWHL],Iw,W->H);
            Success = send(TxSocket,line,strlen(line),0);
            if (EchoEnabled) printf("%s",line);
         }
      }
      sprintf(line,"[EOF]\n\n");
      Success = send(TxSocket,line,strlen(line),0);
      if (EchoEnabled) printf("%s",line);

      /* Wait for ack */
      recv(TxSocket,line,512,0);
}
/*********************************************************************/
void InitInterProcessComm(void)
{
      FILE *infile;
      char junk[120],newline,response[120];
      long Im;
      long AllowBlocking = TRUE;

      infile = FileOpen(InOutPath,"Inp_IPC.txt","rt");
      fscanf(infile,"%[^\n] %[\n]",junk,&newline);
      fscanf(infile,"%s %[^\n] %[\n]",response,junk,&newline);
      IpcMode = DecodeString(response);
      fscanf(infile,"%s %[^\n] %[\n]",response,junk,&newline);
      SocketRole = DecodeString(response);
      fscanf(infile,"%s %d %[^\n] %[\n]",HostName,&Port,junk,&newline);
      fscanf(infile,"%s %[^\n] %[\n]",response,junk,&newline);
      AllowBlocking = DecodeString(response);
      fscanf(infile,"%s %[^\n] %[\n]",response,junk,&newline);
      EchoEnabled = DecodeString(response);
      fscanf(infile,"%ld %[^\n] %[\n]",&Nmnem,junk,&newline);
      Mnemonic = (char **) calloc(Nmnem,sizeof(char *));
      for(Im=0;Im<Nmnem;Im++) {
         Mnemonic[Im] = (char *) calloc(40,sizeof(char));
      }
      fscanf(infile,"%[^\n] %[\n]",junk,&newline);
      fscanf(infile,"%[^\n] %[\n]",junk,&newline);
      for(Im=0;Im<Nmnem;Im++) {
         fscanf(infile,"%s %[^\n] %[\n]",Mnemonic[Im],junk,&newline);
      }
      fclose(infile);

      if (IpcMode == IPC_TX) {
         if (SocketRole == IPC_SERVER) {
            TxSocket = InitSocketServer(Port,AllowBlocking);
         }
         else if (SocketRole == IPC_CLIENT) {
            TxSocket = InitSocketClient(HostName,Port,AllowBlocking);
         }
         #ifdef _ENABLE_IPC_GMSEC_
         else if (SocketRole == IPC_GMSEC_CLIENT) {
            ConnectToMBServer();
            connectionManagerSubscribe(ConnMgr,"GMSEC.42.RX.>",status);
            checkStatus();
         }
         #endif
         else {
            printf("Oops.  Unknown SocketRole %ld for TxSocket in InitInterProcessComm.  Bailing out.\n",SocketRole);
            exit(1);
         }
      }
      else if (IpcMode == IPC_RX) {
         if (SocketRole == IPC_SERVER) {
            RxSocket = InitSocketServer(Port,AllowBlocking);
         }
         else if (SocketRole == IPC_CLIENT) {
            RxSocket = InitSocketClient(HostName,Port,AllowBlocking);
         }
         #ifdef _ENABLE_IPC_GMSEC_
         else if (SocketRole == IPC_GMSEC_CLIENT) {
            ConnectToMBServer();
            connectionManagerSubscribe(ConnMgr,"GMSEC.42.TX.>",status);
            checkStatus();
         }
         #endif
         else {
            printf("Oops.  Unknown SocketRole %ld for RxSocket in InitInterProcessComm.  Bailing out.\n",SocketRole);
            exit(1);
         }

      }
      else if (IpcMode == IPC_WRITEFILE) {
         statefile = FileOpen(InOutPath,"States.42","wt");
      }
      else if (IpcMode == IPC_READFILE) {
         statefile = FileOpen(InOutPath,"States.42","rt");
      }
      else if (IpcMode == IPC_FFTB) {
         SocketRole = IPC_CLIENT; /* Spirent is Host */
         TxSocket = InitSocketClient(HostName,Port,AllowBlocking);
      }
}
/*********************************************************************/
void InterProcessComm(void)
{
      if (IpcMode == IPC_TX) {
         if (SocketRole != IPC_GMSEC_CLIENT) {
            WriteStatesToSocket();
         }
         #ifdef _ENABLE_IPC_GMSEC_
         else {
            WriteStatesToGmsec();
         }
         #endif
      }
      else if (IpcMode == IPC_RX) {
         if (SocketRole != IPC_GMSEC_CLIENT) {
            ReadStatesFromSocket();
         }
         #ifdef _ENABLE_IPC_GMSEC_
         else {
            ReadStatesFromGmsec();
         }
         #endif
      }
      else if (IpcMode == IPC_WRITEFILE) {
         WriteStatesToFile();
      }
      else if (IpcMode == IPC_READFILE) {
         ReadStatesFromFile();
      }
#ifdef _ENABLE_FFTB_CODE_
      else if (IpcMode == IPC_FFTB) {
         SendStatesToSpirent();
      }
#endif
}

/* #ifdef __cplusplus
** }
** #endif
*/

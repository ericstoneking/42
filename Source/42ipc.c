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

#ifdef _ENABLE_GMSEC_
   #include "gmseckit.h"
   GMSEC_Config cfg;
   GMSEC_ConnectionMgr ConnMgr;
   GMSEC_Status status;
   void WriteToGmsec(GMSEC_ConnectionMgr ConnMgr,GMSEC_Status status);
   void ReadFromGmsec(GMSEC_ConnectionMgr ConnMgr,GMSEC_Status status);
#endif

/* #ifdef __cplusplus
** namespace _42 {
** using namespace Kit;
** #endif
*/

void WriteToFile(FILE *StateFile, char **Prefix, long Nprefix, long EchoEnabled);
void WriteToSocket(SOCKET Socket,  char **Prefix, long Nprefix, long EchoEnabled);
void ReadFromFile(FILE *StateFile, long EchoEnabled);
void ReadFromSocket(SOCKET Socket, long EchoEnabled);

/*********************************************************************/
void InitInterProcessComm(void)
{
      FILE *infile;
      char junk[120],newline,response[120];
      struct IpcType *I;
      long Iipc,Ipx;
      char FileName[80],Prefix[80];

      infile = FileOpen(InOutPath,"Inp_IPC.txt","rt");
      fscanf(infile,"%[^\n] %[\n]",junk,&newline);
      fscanf(infile,"%ld %[^\n] %[\n]",&Nipc,junk,&newline);
      IPC = (struct IpcType *) calloc(Nipc,sizeof(struct IpcType));
      for(Iipc=0;Iipc<Nipc;Iipc++) {
         I = &IPC[Iipc];
         fscanf(infile,"%[^\n] %[\n]",junk,&newline);
         fscanf(infile,"%s %[^\n] %[\n]",response,junk,&newline);
         I->Mode = DecodeString(response);
         fscanf(infile,"%ld %[^\n] %[\n]",&I->AcsID,junk,&newline);
         fscanf(infile,"\"%[^\"]\" %[^\n] %[\n]",FileName,junk,&newline);
         fscanf(infile,"%s %[^\n] %[\n]",response,junk,&newline);
         I->SocketRole = DecodeString(response);
         fscanf(infile,"%s %ld %[^\n] %[\n]",I->HostName,&I->Port,junk,&newline);
         fscanf(infile,"%s %[^\n] %[\n]",response,junk,&newline);
         I->AllowBlocking = DecodeString(response);
         fscanf(infile,"%s %[^\n] %[\n]",response,junk,&newline);
         I->EchoEnabled = DecodeString(response);
         fscanf(infile,"%ld %[^\n] %[\n]",&I->Nprefix,junk,&newline);
         I->Prefix = (char **) calloc(I->Nprefix,sizeof(char *));
         for(Ipx=0;Ipx<I->Nprefix;Ipx++) {
            fscanf(infile,"\"%[^\"]\" %[^\n] %[\n]",Prefix,junk,&newline);
            I->Prefix[Ipx] = (char *) calloc(strlen(Prefix)+1,sizeof(char));
            strcpy(I->Prefix[Ipx],Prefix);
         }
         
         I->Init = 1;

         if (I->Mode == IPC_TX) {
            if (I->SocketRole == IPC_SERVER) {
               I->Socket = InitSocketServer(I->Port,I->AllowBlocking);
            }
            else if (I->SocketRole == IPC_CLIENT) {
               I->Socket = InitSocketClient(I->HostName,I->Port,I->AllowBlocking);
            }
            #ifdef _ENABLE_GMSEC_
            else if (I->SocketRole == IPC_GMSEC_CLIENT) {
               status = statusCreate();
               cfg = configCreate();
               ConnMgr = ConnectToMBServer(I->HostName,I->Port,status,cfg);
               connectionManagerSubscribe(ConnMgr,"GMSEC.42.RX.>",status);
               CheckGmsecStatus(status);
            }
            #endif
            else {
               printf("Oops.  Unknown SocketRole %ld for IPC[%ld] in InitInterProcessComm.  Bailing out.\n",I->SocketRole,Iipc);
               exit(1);
            }
         }
         else if (I->Mode == IPC_RX) {
            if (I->SocketRole == IPC_SERVER) {
               I->Socket = InitSocketServer(I->Port,I->AllowBlocking);
            }
            else if (I->SocketRole == IPC_CLIENT) {
               I->Socket = InitSocketClient(I->HostName,I->Port,I->AllowBlocking);
            }
            #ifdef _ENABLE_GMSEC_
            else if (I->SocketRole == IPC_GMSEC_CLIENT) {
               status = statusCreate();
               cfg = configCreate();
               ConnMgr = ConnectToMBServer(I->HostName,I->Port,status,cfg);
               connectionManagerSubscribe(ConnMgr,"GMSEC.42.TX.>",status);
               CheckGmsecStatus(status);
            }
            #endif
            else {
               printf("Oops.  Unknown SocketRole %ld for IPC[%ld] in InitInterProcessComm.  Bailing out.\n",I->SocketRole,Iipc);
               exit(1);
            }

         }
         else if (I->Mode == IPC_TXRX) {
            if (I->SocketRole == IPC_SERVER) {
               I->Socket = InitSocketServer(I->Port,I->AllowBlocking);
            }
            else if (I->SocketRole == IPC_CLIENT) {
               I->Socket = InitSocketClient(I->HostName,I->Port,I->AllowBlocking);
            }
            #ifdef _ENABLE_GMSEC_
            else if (I->SocketRole == IPC_GMSEC_CLIENT) {
               status = statusCreate();
               cfg = configCreate();
               ConnMgr = ConnectToMBServer(I->HostName,I->Port,status,cfg);
               connectionManagerSubscribe(ConnMgr,"GMSEC.42.TXRX.>",status);
               CheckGmsecStatus(status);
            }
            #endif
            else {
               printf("Oops.  Unknown SocketRole %ld for IPC[%ld] in InitInterProcessComm.  Bailing out.\n",I->SocketRole,Iipc);
               exit(1);
            }

         }
         else if (I->Mode == IPC_ACS) {
            I->Socket = InitSocketServer(I->Port,I->AllowBlocking);
         }
         else if (I->Mode == IPC_WRITEFILE) {
            I->File = FileOpen(InOutPath,FileName,"wt");
         }
         else if (I->Mode == IPC_READFILE) {
            I->File = FileOpen(InOutPath,FileName,"rt");
         }
         else if (I->Mode == IPC_FFTB) {
            I->SocketRole = IPC_CLIENT; /* Spirent is Host */
            I->Socket = InitSocketClient(I->HostName,I->Port,I->AllowBlocking);
         }
      }
      fclose(infile);
}
/*********************************************************************/
void InterProcessComm(void)
{
      struct IpcType *I;
      long Iipc;
      
      for(Iipc=0;Iipc<Nipc;Iipc++) {
         I = &IPC[Iipc];
         if (I->Mode == IPC_TX) {
            if (I->SocketRole != IPC_GMSEC_CLIENT) {
               WriteToSocket(I->Socket,I->Prefix,I->Nprefix,I->EchoEnabled);
            }
            #ifdef _ENABLE_GMSEC_
            else {
               WriteToGmsec(ConnMgr,status,I->Prefix,I->Nprefix,I->EchoEnabled);
            }
            #endif
         }
         else if (I->Mode == IPC_RX) {
            if (I->SocketRole != IPC_GMSEC_CLIENT) {
               ReadFromSocket(I->Socket,I->EchoEnabled);
            }
            #ifdef _ENABLE_GMSEC_
            else {
               ReadFromGmsec(ConnMgr,status,I->EchoEnabled);
            }
            #endif
         }
         else if (I->Mode == IPC_TXRX) {
            if (I->SocketRole != IPC_GMSEC_CLIENT) {
               WriteToSocket(I->Socket,I->Prefix,I->Nprefix,I->EchoEnabled);
               ReadFromSocket(I->Socket,I->EchoEnabled);
            }
            #ifdef _ENABLE_GMSEC_
            else {
               WriteToGmsec(ConnMgr,status,I->Prefix,I->Nprefix,I->EchoEnabled);
               ReadFromGmsec(ConnMgr,status,I->EchoEnabled);
            }
            #endif
         }
         else if (I->Mode == IPC_WRITEFILE) {
            WriteToFile(I->File,I->Prefix,I->Nprefix,I->EchoEnabled);
         }
         else if (I->Mode == IPC_READFILE) {
            ReadFromFile(I->File,I->EchoEnabled);
         }
         #ifdef _ENABLE_FFTB_CODE_
         else if (I->Mode == IPC_FFTB) {
            SendStatesToSpirent();
         }
         #endif
      }
}

/* #ifdef __cplusplus
** }
** #endif
*/

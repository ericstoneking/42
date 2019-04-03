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

int Port;
char HostName[80];
FILE *statefile;

void WriteToFile(FILE *StateFile);
void WriteToSocket(SOCKET Socket);
void ReadFromFile(FILE *StateFile);
void ReadFromSocket(SOCKET Socket);

/*********************************************************************/
void InitInterProcessComm(void)
{
      FILE *infile;
      char junk[120],newline,response[120];
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
      fclose(infile);

      if (IpcMode == IPC_TX) {
         if (SocketRole == IPC_SERVER) {
            TxSocket = InitSocketServer(Port,AllowBlocking);
         }
         else if (SocketRole == IPC_CLIENT) {
            TxSocket = InitSocketClient(HostName,Port,AllowBlocking);
         }
         #ifdef _ENABLE_GMSEC_
         else if (SocketRole == IPC_GMSEC_CLIENT) {
            status = statusCreate();
            cfg = configCreate();
            ConnMgr = ConnectToMBServer(HostName,Port,status,cfg);
            connectionManagerSubscribe(ConnMgr,"GMSEC.42.RX.>",status);
            CheckGmsecStatus(status);
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
         #ifdef _ENABLE_GMSEC_
         else if (SocketRole == IPC_GMSEC_CLIENT) {
            status = statusCreate();
            cfg = configCreate();
            ConnMgr = ConnectToMBServer(HostName,Port,status,cfg);
            connectionManagerSubscribe(ConnMgr,"GMSEC.42.TX.>",status);
            CheckGmsecStatus(status);
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
            WriteToSocket(TxSocket);
         }
         #ifdef _ENABLE_GMSEC_
         else {
            WriteToGmsec(ConnMgr,status);
         }
         #endif
      }
      else if (IpcMode == IPC_RX) {
         if (SocketRole != IPC_GMSEC_CLIENT) {
            ReadFromSocket(RxSocket);
         }
         #ifdef _ENABLE_GMSEC_
         else {
            ReadFromGmsec(ConnMgr,status);
         }
         #endif
      }
      else if (IpcMode == IPC_WRITEFILE) {
         WriteToFile(statefile);
      }
      else if (IpcMode == IPC_READFILE) {
         ReadFromFile(statefile);
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

/*    This file is distributed with 42,                               */
/*    the (mostly harmless) spacecraft dynamics simulation            */
/*    created by Eric Stoneking of NASA Goddard Space Flight Center   */

/*    Copyright 2010 United States Government                         */
/*    as represented by the Administrator                             */
/*    of the National Aeronautics and Space Administration.           */

/*    No copyright is claimed in the United States                    */
/*    under Title 17, U.S. Code.                                      */

/*    All Other Rights Reserved.                                      */


/* #ifdef __cplusplus
** namespace _42 {
** using namespace Kit;
** #endif
*/

#include "gmseckit.h"

/*********************************************************************/
void CheckGmsecStatus(GMSEC_Status status)
{
      if (statusIsError(status) == GMSEC_TRUE) {
         GMSEC_ERROR("Error: %s\n", statusGet(status));
      }
}
/*********************************************************************/
GMSEC_ConnectionMgr ConnectToMBServer(const char HostName[80], int Port, GMSEC_Status status,
   GMSEC_Config cfg)
{
      GMSEC_ConnectionMgr ConnMgr;
      const char *name = NULL;
      const char *value = NULL;
      char PortString[80];

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
      CheckGmsecStatus(status);

      connectionManagerInitialize(ConnMgr, status);
      CheckGmsecStatus(status);
      
      return(ConnMgr);
      
}
/*********************************************************************/
void GmsecSend(const char *Header, const char *Text,
   GMSEC_ConnectionMgr ConnMgr,GMSEC_Status status)
{
      GMSEC_Message msg;
      
      msg = messageCreate(Header,GMSEC_PUBLISH,status);
      messageAddStringField(msg,"MSG-TEXT",Text,status);
      CheckGmsecStatus(status);
      connectionManagerPublish(ConnMgr,msg,status);
      CheckGmsecStatus(status);
      messageDestroy(&msg);
}

/* #ifdef __cplusplus
** }
** #endif
*/

/*    This file is distributed with 42,                               */
/*    the (mostly harmless) spacecraft dynamics simulation            */
/*    created by Eric Stoneking of NASA Goddard Space Flight Center   */

/*    Copyright 2010 United States Government                         */
/*    as represented by the Administrator                             */
/*    of the National Aeronautics and Space Administration.           */

/*    No copyright is claimed in the United States                    */
/*    under Title 17, U.S. Code.                                      */

/*    All Other Rights Reserved.                                      */

#ifndef __GMSECKIT_H__
#define __GMSECKIT_H__

/* #ifdef __cplusplus
** namespace _42 {
** using namespace Kit;
** #endif
*/

#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include "gmsec4_c.h"

void CheckGmsecStatus(GMSEC_Status status);
GMSEC_ConnectionMgr ConnectToMBServer(const char HostName[80], int Port, GMSEC_Status status,
   GMSEC_Config cfg);
void GmsecSend(const char *Header, const char *Text,
   GMSEC_ConnectionMgr ConnMgr,GMSEC_Status status);

/* #ifdef __cplusplus
** }
** #endif
*/

#endif /* __GMSECKIT_H__ */

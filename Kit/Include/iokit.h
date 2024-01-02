/*    This file is distributed with 42,                               */
/*    the (mostly harmless) spacecraft dynamics simulation            */
/*    created by Eric Stoneking of NASA Goddard Space Flight Center   */

/*    Copyright 2010 United States Government                         */
/*    as represented by the Administrator                             */
/*    of the National Aeronautics and Space Administration.           */

/*    No copyright is claimed in the United States                    */
/*    under Title 17, U.S. Code.                                      */

/*    All Other Rights Reserved.                                      */


#ifndef __IOKIT_H__
#define __IOKIT_H__

/*
** #ifdef __cplusplus
** namespace Kit {
** #endif
*/

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#ifdef _WIN32
   #include <winsock2.h>
#else
   #include <sys/socket.h>
   #include <netinet/in.h>
   #include <netinet/tcp.h>
   #include <netdb.h>
   /* Finesse winsock SOCKET datatype */
   #define SOCKET int
#endif
/* #include <sys/un.h> */

FILE *FileOpen(const char *Path, const char *File, const char *CtrlCode);
void ByteSwapDouble(double *A);
int FileToString(const char *file_name, char **result_string,
                 size_t *string_len);

SOCKET InitSocketServer(int Port, int AllowBlocking);
SOCKET InitSocketClient(const char *hostname, int Port, int AllowBlocking);

/*
** #ifdef __cplusplus
** }
** #endif
*/

#endif /* __IOKIT_H__ */

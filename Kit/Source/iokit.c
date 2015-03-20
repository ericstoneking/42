/*    This file is distributed with 42,                               */
/*    the (mostly harmless) spacecraft dynamics simulation            */
/*    created by Eric Stoneking of NASA Goddard Space Flight Center   */

/*    Copyright 2010 United States Government                         */
/*    as represented by the Administrator                             */
/*    of the National Aeronautics and Space Administration.           */

/*    No copyright is claimed in the United States                    */
/*    under Title 17, U.S. Code.                                      */

/*    All Other Rights Reserved.                                      */


#include "iokit.h"

//#ifdef __cplusplus
//namespace Kit {
//#endif

/**********************************************************************/
FILE *FileOpen(const char *Path, const char *File, const char *CtrlCode)
{
      FILE *FilePtr;
      char *FileName;

      FileName = (char *) calloc(strlen(Path)+strlen(File)+1,sizeof(char));
      strcpy(FileName,Path);
      strcat(FileName,File);
      FilePtr=fopen(FileName,CtrlCode);
      if(FilePtr == NULL) {
         printf("Error opening %s: %s\n",FileName, strerror(errno));
         exit(1);
      }
      free(FileName);
      return(FilePtr);
}
/**********************************************************************/
void ByteSwapDouble(double *A)
{
      char fwd[8],bak[8];
      long i;

      memcpy(fwd,A,sizeof(double));
      for(i=0;i<8;i++) bak[i] = fwd[7-i];
      memcpy(A,bak,sizeof(double));
}
#ifdef _ENABLE_SOCKETS_
/**********************************************************************/
int InitSocketServer(int Port)
{
      int init_sockfd,sockfd,flags;
      socklen_t clilen;
      struct sockaddr_in Server, Client;

      init_sockfd = socket(AF_INET,SOCK_STREAM,0);
      if (init_sockfd < 0) {
         printf("Error opening server socket.\n");
         exit(1);
      }
      memset((char *) &Server,0,sizeof(Server));
      Server.sin_family = AF_INET;
      Server.sin_addr.s_addr = INADDR_ANY;
      Server.sin_port = htons(Port);
      if (bind(init_sockfd,(struct sockaddr *) &Server,sizeof(Server)) < 0) {
         printf("Error on binding server socket.\n");
         exit(1);
      }
      listen(init_sockfd,5);
      clilen = sizeof(Client);
      sockfd = accept(init_sockfd,(struct sockaddr *) &Client,&clilen);
      if (sockfd < 0) {
         printf("Error on accepting client socket.\n");
         exit(1);
      }
      printf("Server side of socket established.\n");
      close(init_sockfd);

      /* Keep read() from waiting for message to come */
      flags = fcntl(sockfd, F_GETFL, 0);
      fcntl(sockfd,F_SETFL, flags|O_NONBLOCK);

      return(sockfd);
}
/**********************************************************************/
int InitSocketClient(const char *hostname, int Port)
{
      int sockfd,flags;
      struct sockaddr_in Server;
      struct hostent *Host;

      sockfd = socket(AF_INET,SOCK_STREAM,0);
      if (sockfd < 0) {
         printf("Error opening client socket.\n");
         exit(1);
      }
      Host = gethostbyname(hostname);
      if (Host == NULL) {
         printf("Server not found by client socket.\n");
         exit(1);
      }
      memset((char *) &Server,0,sizeof(Server));
      Server.sin_family = AF_INET;
      memcpy((char *)&Server.sin_addr.s_addr,(char *)Host->h_addr,
         Host->h_length);
      Server.sin_port = htons(Port);
      if (connect(sockfd,(struct sockaddr *) &Server,sizeof(Server)) < 0) {
         printf("Error connecting client socket: %s.\n",strerror(errno));
         exit(1);
      }
      printf("Client side of socket established.\n");

      /* Keep read() from waiting for message to come */
      flags = fcntl(sockfd, F_GETFL, 0);
      fcntl(sockfd,F_SETFL, flags|O_NONBLOCK);

      return(sockfd);
}
#endif /* _ENABLE_SOCKETS_ */

//#ifdef __cplusplus
//}
//#endif

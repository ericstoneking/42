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

/* #ifdef __cplusplus
** namespace Kit {
** #endif
*/

/**********************************************************************/
FILE *FileOpen(const char *Path, const char *File, const char *CtrlCode)
{
      FILE *FilePtr;
      char FileName[1024];

      strcpy(FileName,Path);
      strcat(FileName,File);
      FilePtr=fopen(FileName,CtrlCode);
      if(FilePtr == NULL) {
         printf("Error opening %s: %s\n",FileName, strerror(errno));
         exit(1);
      }
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
/**********************************************************************/
/*  This function cribbed from an OpenCL example                      */
/*  on the Apple developer site                                       */
int FileToString(const char *file_name, char **result_string,
                 size_t *string_len)
{
      int fd;
      size_t file_len;
      struct stat file_status;
      int ret;

      *string_len = 0;
      fd = open(file_name, O_RDONLY);
      if (fd == -1) {
          printf("Error opening file %s\n", file_name);
          return -1;
      }
      ret = fstat(fd, &file_status);
      if (ret) {
          printf("Error reading status for file %s\n", file_name);
          return -1;
      }
      file_len = file_status.st_size;

      *result_string = (char *) calloc(file_len + 1, sizeof(char));
      ret = read(fd, *result_string, file_len);
      if (!ret) {
          printf("Error reading from file %s\n", file_name);
          return -1;
      }

      close(fd);

      *string_len = file_len;
      return 0;
}
/**********************************************************************/
SOCKET InitSocketServer(int Port, int AllowBlocking)
{
#if defined(_WIN32)

      WSADATA wsa;
      SOCKET init_sockfd,sockfd;
      u_long Blocking = 1;

      int clilen;
      struct sockaddr_in Server, Client;

      /* Initialize winsock */
      if (WSAStartup(MAKEWORD(2,2),&wsa) != 0) {
         printf("Error initializing winsock in InitSocketClient.\n");
         exit(1);
      }

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
      printf("Server is listening on port %i\n",Port);
      listen(init_sockfd,5);
      clilen = sizeof(Client);
      sockfd = accept(init_sockfd,(struct sockaddr *) &Client,&clilen);
      if (sockfd < 0) {
         printf("Error on accepting client socket.\n");
         exit(1);
      }
      printf("Server side of socket established.\n");
      closesocket(init_sockfd);

      /* Keep read() from waiting for message to come */
      if (!AllowBlocking) {
         /*flags = fcntl(sockfd, F_GETFL, 0);*/
         /*fcntl(sockfd,F_SETFL, flags|O_NONBLOCK);*/
         ioctlsocket(sockfd,FIONBIO,&Blocking);
      }
      
      return(sockfd);
#else

      SOCKET init_sockfd,sockfd;
      int flags;
      socklen_t clilen;
      struct sockaddr_in Server, Client;
      int opt = 1;
      int DisableNagle = 1;

      init_sockfd = socket(AF_INET,SOCK_STREAM,0);
      if (init_sockfd < 0) {
         printf("Error opening server socket.\n");
         exit(1);
      }
      
      /* Allowing reuse while in TIME_WAIT might make port available */
      /* more quickly after a socket has been broken */
      if (setsockopt(init_sockfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt)) == -1) {
        printf("Error setting socket option.\n");
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
      printf("Server is listening on port %i\n",Port);
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
      if (!AllowBlocking) {
         flags = fcntl(sockfd, F_GETFL, 0);
         fcntl(sockfd,F_SETFL, flags|O_NONBLOCK);
      }

      /* Allow TCP to send small packets (look up Nagle's algorithm) */
      /* Depending on your message sizes, this may or may not improve performance */
      setsockopt(sockfd,IPPROTO_TCP,TCP_NODELAY,&DisableNagle,sizeof(DisableNagle));

      return(sockfd);
#endif
}
/**********************************************************************/
SOCKET InitSocketClient(const char *hostname, int Port,int AllowBlocking)
{
#if defined(_WIN32)

      WSADATA wsa; /* winsock */
      SOCKET sockfd;
      u_long Blocking = 1;

      struct sockaddr_in Server;
      struct hostent *Host;

      /* Initialize winsock */
      if (WSAStartup(MAKEWORD(2,2),&wsa) != 0) {
         printf("Error initializing winsock in InitSocketClient.\n");
         exit(1);
      }
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
      printf("Client connecting to Server on Port %i\n",Port);
      if (connect(sockfd,(struct sockaddr *) &Server,sizeof(Server)) < 0) {
         printf("Error connecting client socket: %s.\n",strerror(errno));
         exit(1);
      }
      printf("Client side of socket established.\n");

      /* Keep read() from waiting for message to come */
      if (!AllowBlocking) {
         /*flags = fcntl(sockfd, F_GETFL, 0);*/
         /*fcntl(sockfd,F_SETFL, flags|O_NONBLOCK);*/
         ioctlsocket(sockfd,FIONBIO,&Blocking);
      }

      return(sockfd);
#else
      SOCKET sockfd;
      int flags;
      struct sockaddr_in Server;
      struct hostent *Host;
      int DisableNagle = 1;

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
      printf("Client connecting to Server on Port %i\n",Port);
      if (connect(sockfd,(struct sockaddr *) &Server,sizeof(Server)) < 0) {
         printf("Error connecting client socket: %s.\n",strerror(errno));
         exit(1);
      }
      printf("Client side of socket established.\n");

      /* Keep read() from waiting for message to come */
      if (!AllowBlocking) {
         flags = fcntl(sockfd, F_GETFL, 0);
         fcntl(sockfd,F_SETFL, flags|O_NONBLOCK);
      }

      /* Allow TCP to send small packets (look up Nagle's algorithm) */
      /* Depending on your message sizes, this may or may not improve performance */
      setsockopt(sockfd,IPPROTO_TCP,TCP_NODELAY,&DisableNagle,sizeof(DisableNagle));

      return(sockfd);
#endif /* _WIN32 */
}

/* #ifdef __cplusplus
** }
** #endif
*/

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
      
      /* Allow TCP to send small packets (look up Nagle's algorithm) */
      /* Depending on your message sizes, this may or may not improve performance */
      //setsockopt(sockfd,IPPROTO_TCP,TCP_NODELAY,&DisableNagle,sizeof(DisableNagle));
      
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

/**
 * Splits the `path_file` into a directory part: `path`, and file part: `file`
 * If `path_file` is just a filename with no containing folder, `path` will be a 0 length string.
*/
void SplitPath(char *path_file, char **path, char **file) {
   *path = (char *)malloc(strlen(path_file));
   // file = (char *)malloc(strlen(path_file));
   strcpy(*path, path_file);
   char *pos = strrchr(*path, OS_SEP);
   if (NULL == pos) {
      *file = strdup(path_file);
      (*path)[0] = '\0';
   } else {
      *pos = '\0';
      *file = strdup(pos + 1);
   }
}

/**
 * Attempts to retrieve the full path to the currently running executable
 * and populates `path` with it. Uses `max_path_len` to avoid overrunning `path`
 * `max_path_len` is the total size of `path`, which means it includes the
 * null terminating character.
*/
void GetExecutablePath(char *path, size_t max_path_len) {
   ssize_t len;
   memset(path, 0, max_path_len);
#if defined(_WIN32)
   strncpy(path, _pgmptr, max_path_len);
   len = strlen(path);
#else
   len = readlink("/proc/self/exe", path, max_path_len);
   if (len < 0) {
     printf("Error: error when getting executable path. Exiting.\n");
     exit(EXIT_FAILURE);
   }
#endif
   if (len >= max_path_len) {
     printf("Error: executable path received too long for file path buffer. Exiting.\n");
     exit(EXIT_FAILURE);
   }
}

/**
 * Looks for a trailing slash in `path`; if one isn't found it appends one to `path`
*/
void AddTrailingSlash(char *path) {
  char *end = strchr(path, '\0');
  if (end == NULL) {
    printf(
        "Error: path provided contains no null terminating character. "
        "Exiting.\n");
    exit(EXIT_FAILURE);
   }

   char *slash = strrchr(path, OS_SEP);
   if (slash != NULL) {
      size_t diff = end - slash;
      if (diff == 1) {
         return;
      }
   }

   char *sep = (char *)malloc(3);
   sprintf(sep, "%c", OS_SEP);
   strcat(path, sep);
   free(sep);
}

/* #ifdef __cplusplus
** }
** #endif
*/

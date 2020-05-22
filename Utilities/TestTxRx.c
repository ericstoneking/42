#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
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
/**********************************************************************/
int main(int argc, char **argv)
{
      SOCKET Socket;
      char hostname[80] = "localhost";
      int Port = 10001;
      int AllowBlocking = 1;
      char Msg[16384];
      long MsgLen = 0;
      long LineLen;
      char line[512];
      long Imsg,Iline,Isc;
      int NumBytes;
      long LongVal;
      long Done;
      long ReadCtr = 0;
      long WriteCtr = 0;
      char AckMsg[5] = "Ack\n";
      
      Socket = InitSocketClient(hostname,Port,AllowBlocking);
      
      while(1) {
         /* Receive message */
         memset(Msg,'\0',16384);
         MsgLen = 0;

         NumBytes = recv(Socket,Msg,16384,0);
         if (NumBytes <= 0) return(1); /* Bail out if no message */

         Done = 0;
         Imsg = 0;
         while(!Done) {
            /* Parse lines from Msg, newline-delimited */
            Iline = 0;
            memset(line,'\0',512);
            while((Msg[Imsg] != '\n') && (Iline < 511) && (Imsg < 16383)) {
               line[Iline++] = Msg[Imsg++];
            }
            line[Iline++] = Msg[Imsg++];
            printf("In: %s",line);

            if (sscanf(line,"SC[%ld].IpcOutCtr = %ld",
               &Isc,
               &LongVal) == 2) {
               ReadCtr = LongVal;
            }
            if (!strncmp(line,"[EOF]",5)) {
               Done = 1;
            }
         }
   
         /* Acknowledge receipt */
         send(Socket,AckMsg,strlen(AckMsg),0);

         /* Send message */
         WriteCtr++;
         sprintf(line,"SC[%ld].IpcInCtr = %ld\n",
            Isc,
            WriteCtr);
         LineLen = strlen(line);
         memcpy(&Msg[MsgLen],line,LineLen);
         MsgLen += LineLen;
         printf("Out: %s",line);

         sprintf(line,"[EOF]\n\n");
         printf("Out: %s",line);

         LineLen = strlen(line);
         memcpy(&Msg[MsgLen],line,LineLen);
         MsgLen += LineLen;
         send(Socket,Msg,strlen(Msg),0);
         
         /* Wait for Ack */
         recv(Socket,AckMsg,5,0);
      }

      return(0);
}

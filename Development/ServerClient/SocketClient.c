/* Socket-based interprocess communication */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

#define SOCK_PORT 4242


/**********************************************************************/
int main(int argc, char **argv)
{
      int sockfd,Success;
      struct sockaddr_in Server;
      struct hostent *Host;
      
      char line[512];
      long i,k;
      double RotAxis[3],RotCenter[3],RotRate;
      double CMx,CMy,CMz,Fx,Fy,Fz,Tx,Ty,Tz;
      
      sockfd = socket(AF_INET,SOCK_STREAM,0);
      if (sockfd < 0) {
         printf("Error opening socket.\n");
         exit(1);
      }
      Host = gethostbyname("localhost");
      if (Host == NULL) {
         printf("Host not found\n");
         exit(1);
      }
      bzero((char *) &Server,sizeof(Server));
      Server.sin_family = AF_INET;
      bcopy((char *)Host->h_addr,
         (char *)&Server.sin_addr.s_addr,
         Host->h_length);
      Server.sin_port = htons(SOCK_PORT);
      if (connect(sockfd,(struct sockaddr *) &Server,sizeof(Server)) < 0) {
         printf("Error connecting\n");
         exit(1);
      }

      for(i=1;i<4;i++) {
         for(k=0;k<3;k++) {
            RotAxis[k] = 0.1*((double) i)*((double) k+1);
            RotCenter[k] = 0.01*((double) i)*((double) k+1);
         }
         RotRate = ((double) i);
         
         sprintf(line,"%lf %lf %lf %lf %lf %lf %lf\n",
            RotAxis[0],RotAxis[1],RotAxis[2],
            RotCenter[0],RotCenter[1],RotCenter[2],RotRate);
         Success = write(sockfd,line,strlen(line));
         if (Success < 0) {
            printf("Error writing to socket.\n");
            exit(1);
         }

         bzero(line,512);
         Success = read(sockfd,line,511);
         if (Success < 0) {
            printf("Error reading from socket.\n");
            exit(1);
         }
         sscanf(line,"%lf %lf %lf %lf %lf %lf %lf %lf %lf",
            &CMx,&CMy,&CMz,&Fx,&Fy,&Fz,&Tx,&Ty,&Tz);
         printf("%li Client: %lf %lf %lf  %lf %lf %lf  %lf %lf %lf\n",i,
            CMx,CMy,CMz,Fx,Fy,Fz,Tx,Ty,Tz);
      }
      close(sockfd);

      return(0);
}
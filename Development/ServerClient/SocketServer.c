/* Socket-based interprocess communication */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

#define SOCK_PORT 4242

/***********************************************/
int main(int argc, char **argv)
{

      int ursockfd,sockfd;
      socklen_t clilen;
      char line[512];
      struct sockaddr_in Server, Client;
      int Success;
      
      double RotAxis[3],RotCenter[3],RotRate;
      double CMx,CMy,CMz,Fx,Fy,Fz,Tx,Ty,Tz;
      long i,k;
      double x;
      
      ursockfd = socket(AF_INET,SOCK_STREAM,0);
      if (ursockfd < 0) {
         printf("Error opening socket\n");
         exit(1);
      }
      bzero((char *) &Server,sizeof(Server));
      Server.sin_family = AF_INET;
      Server.sin_addr.s_addr = INADDR_ANY;
      Server.sin_port = htons(SOCK_PORT);
      if (bind(ursockfd,(struct sockaddr *) &Server,sizeof(Server)) < 0) {
         printf("Error on binding\n");
         exit(1);
      }
      listen(ursockfd,5);
      clilen = sizeof(Client);
      sockfd = accept(ursockfd,(struct sockaddr *) &Client,&clilen);
      if (sockfd < 0) {
         printf("Error on accept.\n");
         exit(1);
      }
      
      for(i=1;i<4;i++) {
         x = ((double) i);
         bzero(line,512);
         Success = read(sockfd,line,511);
         if (Success < 0) {
            printf("Error reading from socket\n");
            exit(1);
         }
         sscanf(line,"%lf %lf %lf %lf %lf %lf %lf",
            &RotAxis[0],&RotAxis[1],&RotAxis[2],
            &RotCenter[0],&RotCenter[1],&RotCenter[2],&RotRate);
         printf("%li Server: %lf %lf %lf %lf %lf %lf %lf\n",i,
            RotAxis[0],RotAxis[1],RotAxis[2],
            RotCenter[0],RotCenter[1],RotCenter[2],RotRate);

            CMx = 0.1*x;
            CMy = 0.2*x;
            CMz = 0.3*x;
            Fx = 0.01*x;
            Fy = 0.02*x;
            Fz = 0.03*x;
            Tx = 1.0*x;
            Ty = 2.0*x;
            Tz = 3.0*x;
            
         sprintf(line,"%lf %lf %lf %lf %lf %lf %lf %lf %lf\n",
            CMx,CMy,CMz,Fx,Fy,Fz,Tx,Ty,Tz);
         Success = write(sockfd,line,strlen(line));
         if (Success < 0) {
            printf("Error writing to socket.\n");
            exit(1);
         }
      }
      close(ursockfd);
      close(sockfd);
      
      return(0);
}
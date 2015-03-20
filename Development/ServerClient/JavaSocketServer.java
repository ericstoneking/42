package javasocketserver;

import java.net.*;
import java.lang.*;
import java.io.*;

public class JavaSocketServer {

   public static void main(String[] args) {
      ServerSocket Server;
      Socket Client;
      BufferedReader instream;
      PrintWriter outstream;
      double RotAxis[] = new double[3];
      double RotCenter[] = new double[3];
      double RotRate;
      double CMx,CMy,CMz,Fx,Fy,Fz,Tx,Ty,Tz;
      String inline;
      String outline;
      String RotString[];
      int i,k;
      
      try {
         
         Server = new ServerSocket(4242);
         Client = Server.accept();
         instream = new BufferedReader(new InputStreamReader(Client.getInputStream()));
         outstream = new PrintWriter(Client.getOutputStream(),true);
         for(i=1;i<4;i++) {
            inline = instream.readLine();
            RotString = inline.split(" ");
            for(k=0;k<3;k++) {
               RotAxis[k] = new Double(RotString[k]);
               RotCenter[k] = new Double(RotString[3+k]);
            }
            RotRate = new Double(RotString[6]);
            System.out.format("%d Server: %f %f %f %f %f %f %f\n",i,
               RotAxis[0],RotAxis[1],RotAxis[2],
               RotCenter[0],RotCenter[1],RotCenter[2],RotRate);
            
            CMx = 0.1*((double) i);
            CMy = 0.2*((double) i);
            CMz = 0.3*((double) i);
            Fx = 0.01*((double) i);
            Fy = 0.02*((double) i);
            Fz = 0.03*((double) i);
            Tx = 1.0*((double) i);
            Ty = 2.0*((double) i);
            Tz = 3.0*((double) i);
            
            outline = String.format("%f %f %f %f %f %f %f %f %f\n",
               CMx,CMy,CMz,Fx,Fy,Fz,Tx,Ty,Tz);
            outstream.write(outline,0,outline.length());
            outstream.flush();
         }
         instream.close();
         outstream.close();
         Client.close();
         Server.close();
      }
      catch(IOException oops) {
         System.out.println("Oops");
         System.exit(-1);
      }
   }
}

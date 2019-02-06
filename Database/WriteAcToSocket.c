/**********************************************************************/
void WriteAcToSocket(void)
{

      long i;
      int Success;

      for(i=0;i<AC.Nb;i++) {
         sprintf(line,"AC.B[%ld].mass = [ %lf]",i,AC.B[i].mass);
         Success = send(TxSocket,line,strlen(line),0);
         if (EchoEnabled) printf("%s",line);

         sprintf(line,"AC.B[%ld].cm = [ %lf %lf %lf]",i,AC.B[i].cm[0],AC.B[i].cm[1],AC.B[i].cm[2]);
         Success = send(TxSocket,line,strlen(line),0);
         if (EchoEnabled) printf("%s",line);

         sprintf(line,"AC.B[%ld].MOI = [ %lf %lf %lf %lf %lf %lf %lf %lf %lf]",i,AC.B[i].MOI[0][0],AC.B[i].MOI[0][1],AC.B[i].MOI[0][2],AC.B[i].MOI[1][0],AC.B[i].MOI[1][1],AC.B[i].MOI[1][2],AC.B[i].MOI[2][0],AC.B[i].MOI[2][1],AC.B[i].MOI[2][2]);
         Success = send(TxSocket,line,strlen(line),0);
         if (EchoEnabled) printf("%s",line);

      }

      for(i=0;i<AC.Ng;i++) {
         sprintf(line,"AC.G[%ld].Ang = [ %lf %lf %lf]",i,AC.G[i].Ang[0],AC.G[i].Ang[1],AC.G[i].Ang[2]);
         Success = send(TxSocket,line,strlen(line),0);
         if (EchoEnabled) printf("%s",line);

      }

      for(i=0;i<AC.Ngyro;i++) {
         sprintf(line,"AC.Gyro[%ld].Rate = [ %lf]",i,AC.Gyro[i].Rate);
         Success = send(TxSocket,line,strlen(line),0);
         if (EchoEnabled) printf("%s",line);

      }

      for(i=0;i<AC.Nmag;i++) {
         sprintf(line,"AC.MAG[%ld].Field = [ %lf]",i,AC.MAG[i].Field);
         Success = send(TxSocket,line,strlen(line),0);
         if (EchoEnabled) printf("%s",line);

      }

      for(i=0;i<AC.Ncss;i++) {
         sprintf(line,"AC.CSS[%ld].Valid = [ %ld]",i,AC.CSS[i].Valid);
         Success = send(TxSocket,line,strlen(line),0);
         if (EchoEnabled) printf("%s",line);

         sprintf(line,"AC.CSS[%ld].Illum = [ %lf]",i,AC.CSS[i].Illum);
         Success = send(TxSocket,line,strlen(line),0);
         if (EchoEnabled) printf("%s",line);

      }

      for(i=0;i<AC.Nfss;i++) {
         sprintf(line,"AC.FSS[%ld].Valid = [ %ld]",i,AC.FSS[i].Valid);
         Success = send(TxSocket,line,strlen(line),0);
         if (EchoEnabled) printf("%s",line);

         sprintf(line,"AC.FSS[%ld].SunAng = [ %lf %lf]",i,AC.FSS[i].SunAng[0],AC.FSS[i].SunAng[1]);
         Success = send(TxSocket,line,strlen(line),0);
         if (EchoEnabled) printf("%s",line);

      }

      for(i=0;i<AC.Nst;i++) {
         sprintf(line,"AC.ST[%ld].Valid = [ %ld]",i,AC.ST[i].Valid);
         Success = send(TxSocket,line,strlen(line),0);
         if (EchoEnabled) printf("%s",line);

         sprintf(line,"AC.ST[%ld].qn = [ %lf %lf %lf %lf]",i,AC.ST[i].qn[0],AC.ST[i].qn[1],AC.ST[i].qn[2],AC.ST[i].qn[3]);
         Success = send(TxSocket,line,strlen(line),0);
         if (EchoEnabled) printf("%s",line);

      }

      for(i=0;i<AC.Ngps;i++) {
         sprintf(line,"AC.GPS[%ld].Valid = [ %ld]",i,AC.GPS[i].Valid);
         Success = send(TxSocket,line,strlen(line),0);
         if (EchoEnabled) printf("%s",line);

         sprintf(line,"AC.GPS[%ld].Rollover = [ %ld]",i,AC.GPS[i].Rollover);
         Success = send(TxSocket,line,strlen(line),0);
         if (EchoEnabled) printf("%s",line);

         sprintf(line,"AC.GPS[%ld].Week = [ %ld]",i,AC.GPS[i].Week);
         Success = send(TxSocket,line,strlen(line),0);
         if (EchoEnabled) printf("%s",line);

         sprintf(line,"AC.GPS[%ld].Sec = [ %lf]",i,AC.GPS[i].Sec);
         Success = send(TxSocket,line,strlen(line),0);
         if (EchoEnabled) printf("%s",line);

         sprintf(line,"AC.GPS[%ld].PosN = [ %lf %lf %lf]",i,AC.GPS[i].PosN[0],AC.GPS[i].PosN[1],AC.GPS[i].PosN[2]);
         Success = send(TxSocket,line,strlen(line),0);
         if (EchoEnabled) printf("%s",line);

         sprintf(line,"AC.GPS[%ld].VelN = [ %lf %lf %lf]",i,AC.GPS[i].VelN[0],AC.GPS[i].VelN[1],AC.GPS[i].VelN[2]);
         Success = send(TxSocket,line,strlen(line),0);
         if (EchoEnabled) printf("%s",line);

         sprintf(line,"AC.GPS[%ld].PosW = [ %lf %lf %lf]",i,AC.GPS[i].PosW[0],AC.GPS[i].PosW[1],AC.GPS[i].PosW[2]);
         Success = send(TxSocket,line,strlen(line),0);
         if (EchoEnabled) printf("%s",line);

         sprintf(line,"AC.GPS[%ld].VelW = [ %lf %lf %lf]",i,AC.GPS[i].VelW[0],AC.GPS[i].VelW[1],AC.GPS[i].VelW[2]);
         Success = send(TxSocket,line,strlen(line),0);
         if (EchoEnabled) printf("%s",line);

         sprintf(line,"AC.GPS[%ld].Lng = [ %lf]",i,AC.GPS[i].Lng);
         Success = send(TxSocket,line,strlen(line),0);
         if (EchoEnabled) printf("%s",line);

         sprintf(line,"AC.GPS[%ld].Lat = [ %lf]",i,AC.GPS[i].Lat);
         Success = send(TxSocket,line,strlen(line),0);
         if (EchoEnabled) printf("%s",line);

         sprintf(line,"AC.GPS[%ld].Alt = [ %lf]",i,AC.GPS[i].Alt);
         Success = send(TxSocket,line,strlen(line),0);
         if (EchoEnabled) printf("%s",line);

      }

      for(i=0;i<AC.Nacc;i++) {
      }

      for(i=0;i<AC.Nwhl;i++) {
         sprintf(line,"AC.Whl[%ld].H = [ %lf]",i,AC.Whl[i].H);
         Success = send(TxSocket,line,strlen(line),0);
         if (EchoEnabled) printf("%s",line);

      }

      for(i=0;i<AC.Nmtb;i++) {
      }

      for(i=0;i<AC.Nthr;i++) {
      }

}

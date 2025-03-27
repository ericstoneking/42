using JSON

global cfile
global Tbl
global Ntbl

global WriteFcnString;
global ReadFcnString;

struct SweepType
   Type
   Name
   Alias
   Size
end
################################################################################
function FindStructName(Struct)

      if haskey(Struct,"Abbreviation")
         Name = Struct["Abbreviation"]
      else
         Name = Struct["Variable Name"]
      end

      return(Name)
end
################################################################################
function Prolog(cfile)

      write(cfile,"#include \"42.h\"\n");
      write(cfile,"#define EXTERN extern\n");
      write(cfile,"#include \"Ac.h\"\n");
      write(cfile,"#undef EXTERN\n\n");

end
################################################################################
function BeginWriteFunction(S)

      F = "/"*"*"^78*"/\n";
      F *= "void WriteToSocket(SOCKET Socket,  char **Prefix, long Nprefix, long EchoEnabled)\n";
      F *= "{\n";
      F *= "      struct SCType *S;\n";
      F *= "      struct WorldType *W;\n";
      F *= "      struct OrbitType *O;\n";
      F *= "      int Success;\n";
      F *= "      char Ack[4] = \"Ack\\0\";\n";
      F *= "      long Is,Ipfx;\n";
      F *= "      char Msg[16384];\n";
      F *= "      long MsgLen = 0;\n";
      F *= "      long LineLen,PfxLen;\n";
      F *= "      char line[512];\n";
      if occursin("k=0",S)
         F *= "      long k;\n";
      end
      if occursin("i=0",S)
         F *= "      long i;\n";
      end
      if occursin("j=0",S)
         F *= "      long j;\n";
      end
      F *= "\n";      

      F *= "      sprintf(line,\"TIME %ld-%03ld-%02ld:%02ld:%012.9lf\\n\",\n";
      F *= "         UTC.Year,UTC.doy,UTC.Hour,UTC.Minute,UTC.Second);\n";
      F *= "      LineLen = strlen(line);\n";
      F *= "      memcpy(&Msg[MsgLen],line,LineLen);\n";
      F *= "      MsgLen += LineLen;\n";
      F *= "      if (EchoEnabled) printf(\"%s\",line);\n";
      F *= "\n";
      F *= "      for(Ipfx=0;Ipfx<Nprefix;Ipfx++) {\n";
      F *= "         PfxLen = strlen(Prefix[Ipfx]);\n";
      F *= "\n";
      
      F *= S;
      
      return(F);
end
################################################################################
function EndWriteFunction(F)

      F *= "      }\n";
      F *= "\n";
      F *= "      sprintf(line,\"[ENDMSG]\\n\");\n";
      F *= "      LineLen = strlen(line);\n";
      F *= "      memcpy(&Msg[MsgLen],line,LineLen);\n";
      F *= "      MsgLen += LineLen;\n";
      F *= "      if (EchoEnabled) printf(\"%s\",line);\n";
      F *= "      if (EchoEnabled) printf(\"MsgLen = %ld\\n\",MsgLen);\n";
      F *= "      printf(\"\\n\");\n";
      F *= "\n";
      F *= "      Success = write(Socket,Msg,MsgLen);\n";
      F *= "      if (Success < 0) {\n";
      F *= "         printf(\"Error writing to socket in WriteToSocket.\\n\");\n";
      F *= "         exit(1);\n";
      F *= "      }\n";
      F *= "      read(Socket,Ack,4);\n";
      F *= "\n}\n";

      return(F);
end
################################################################################
function BeginReadFunction(S)

      F = "/"*"*"^78*"/\n";
      F *= "void ReadFromSocket(SOCKET Socket, long EchoEnabled)\n{\n\n";
      
      F *= "      struct SCType *S;\n";
      F *= "      struct OrbitType *O;\n";
      F *= "      struct DynType *D;\n";
      F *= "      long Is,i;\n";
      F *= "      char line[512] = \"Blank\";\n";
      F *= "      long RequestTimeRefresh = 0;\n";
      F *= "      long Done;\n";
      F *= "      char Msg[16384];\n";
      F *= "      long Imsg,Iline;\n";
      F *= "      int NumBytes;\n";
      F *= "      double DbleVal[30];\n";
      if occursin("LongVal",S)
         F *= "      long LongVal[30];\n";
      end
      F *= "      long Year,doy,Hour,Minute;\n";
      F *= "      double Second;\n";
      F *= "      char Ack[4] = \"Ack\\0\";\n";
      if occursin("&k",S)
         F *= "      long k;\n";
      end
      F *= "\n";      
      
      F *= "      NumBytes = read(Socket,Msg,16384);\n";
      F *= "      if (NumBytes < 0) {\n";
      F *= "         printf(\"Error reading from socket in ReadFromSocket.\\n\");\n";
      F *= "         exit(1);\n";
      F *= "      }\n";
      F *= "      write(Socket,Ack,4);\n\n";
      
      F *= "      Done = 0;\n";
      F *= "      Imsg = 0;\n";
      F *= "      while(!Done) {\n";
      F *= "         /* Parse lines from Msg, newline-delimited */\n";
      F *= "         Iline = 0;\n";
      F *= "         memset(line,'\\0',512);\n";
      F *= "         while((Msg[Imsg] != '\\n') && (Iline < 511) && (Imsg < 16383)) {\n";
      F *= "            line[Iline++] = Msg[Imsg++];\n";
      F *= "         }\n";
      F *= "         line[Iline++] = Msg[Imsg++];\n";
      F *= "         if (EchoEnabled) printf(\"%s\",line);\n\n";

      F *= "         if (sscanf(line,\"TIME %ld-%ld-%ld:%ld:%lf\",\n";
      F *= "            &Year,&doy,&Hour,&Minute,&Second) == 5) {\n";
      F *= "            RequestTimeRefresh = 1;\n";
      F *= "         }\n\n";
      
      
      F *= S;
      
      return(F);
end
################################################################################
function EndReadFunction(F)

      F *= "         if (!strncmp(line,\"[ENDMSG]\",8)) {\n";
      F *= "            Done = 1;\n";
      F *= "            //sprintf(line,\"[ENDMSG] reached\\n\");\n";
      F *= "         }\n";
      F *= "         if (Imsg >= 16383) {\n";
      F *= "            Done = 1;\n";
      F *= "            printf(\"Imsg limit exceeded\\n\");\n";
      F *= "         }\n";
      F *= "      }\n";
      F *= "      if (EchoEnabled) printf(\"MsgLen = %ld\\n\\n\",Imsg);\n\n";

      F *= "      if (RequestTimeRefresh) {\n";
      F *= "         /* Update time variables */\n";
      F *= "         UTC.Year = Year;\n";
      F *= "         UTC.doy = doy;\n";
      F *= "         UTC.Hour = Hour;\n";
      F *= "         UTC.Minute = Minute;\n";
      F *= "         UTC.Second = Second;\n";
      F *= "         DOY2MD(UTC.Year,UTC.doy,&UTC.Month,&UTC.Day);\n";
      F *= "         CivilTime = DateToTime(UTC.Year,UTC.Month,UTC.Day,UTC.Hour,UTC.Minute,UTC.Second);\n";
      F *= "         AtomicTime = CivilTime + LeapSec;\n";
      F *= "         GpsTime = AtomicTime - 19.0;\n";
      F *= "         DynTime = AtomicTime + 32.184;\n";
      F *= "         TT.JulDay = TimeToJD(DynTime);\n";
      F *= "         TimeToDate(DynTime,&TT.Year,&TT.Month,&TT.Day,\n";
      F *= "            &TT.Hour,&TT.Minute,&TT.Second,DTSIM);\n";
      F *= "         TT.doy = MD2DOY(TT.Year,TT.Month,TT.Day);\n";
      F *= "         UTC.JulDay = TimeToJD(CivilTime);\n";
      F *= "         GpsTimeToGpsDate(GpsTime,&GpsRollover,&GpsWeek,&GpsSecond);\n";
      F *= "         SimTime = DynTime-DynTime0;\n";
      F *= "      }\n\n";


      F *= "/* .. Refresh SC states that depend on inputs */\n\n";

      F *= "      for(Is=0;Is<Nsc;Is++) {\n";
      F *= "         if (SC[Is].RequestStateRefresh) {\n";
      F *= "            S = &SC[Is];\n";
      F *= "            S->RequestStateRefresh = 0;\n";
      F *= "            if (S->Exists) {\n";
      F *= "               /* Update  RefOrb */\n";
      F *= "               O = &Orb[S->RefOrb];\n";
      F *= "               O->Epoch = DynTime;\n";
      F *= "               for(i=0;i<3;i++) {\n";
      F *= "                  S->PosN[i] = O->PosN[i] + S->PosR[i];\n";
      F *= "                  S->VelN[i] = O->VelN[i] + S->VelR[i];\n";
      F *= "               }\n";
      F *= "               RV2Eph(O->Epoch,O->mu,O->PosN,O->VelN,\n";
      F *= "                  &O->SMA,&O->ecc,&O->inc,&O->RAAN,\n";
      F *= "                  &O->ArgP,&O->anom,&O->tp,\n";
      F *= "                  &O->SLR,&O->alpha,&O->rmin,\n";
      F *= "                  &O->MeanMotion,&O->Period);\n";
      F *= "               FindCLN(O->PosN,O->VelN,O->CLN,O->wln);\n\n";

      F *= "               /* Update Dyn */\n";
      F *= "               MapJointStatesToStateVector(S);\n";
      F *= "               D = &S->Dyn;\n";
      F *= "               MapStateVectorToBodyStates(D->u,D->x,D->h,D->a,D->uf,D->xf,S);\n";
      F *= "               MotionConstraints(S);\n";
      F *= "            }\n";
      F *= "         }\n";
      F *= "      }\n";
      F *= "}\n";

      return(F);
end
################################################################################
function Epilog(cfile)

end
################################################################################
function AddToWriteFunction(F,Sweep,StructName,StructSize,SubstructName,
   SubstructSize,VarName,VarSize,VarType)
   
      Indent = 15;
   
      HasI = false;
      HasJ = false;
      HasK = false;
      
      if (StructSize != "")
         HasK = true;
         Nk = StructSize;
      end
      
      Ni = 1;
      Nj = 1;
      if (VarSize != "")
         HasI = true;
         if occursin(',',VarSize)
            HasJ = true;
            Nij = split(VarSize,',');
            Ni = parse(Int,Nij[1]);
            Nj = parse(Int,Nij[2]);
         else
            Ni = parse(Int,VarSize);
         end
      end
      
      if (VarType == "double")
         Fmt = "%18.12le";
      elseif (VarType == "long")
         Fmt = "%ld";
      end

      F *= "\n";
      if (HasK) 
         F*= " "^Indent*"for(k=0;k<"*Sweep.Alias*"->"*StructSize*";k++) {\n";
         Indent += 3;
      end
      F *= " "^Indent*"sprintf(line,\"";
      F *= Sweep.Name*"[%ld]."
      F *= StructName;
      if (HasK) F *= "[%ld]"; end
      if (StructName != "") F *= "."; end
      if (SubstructName !="") F *= SubstructName*"."; end
      F *= VarName;
      F *= " = ";
      if (HasI) 
         F *= "[";
         if (HasJ) 
            F *= (Fmt*" ")^(Ni*Nj-1)*Fmt; 
         else 
            F *= (Fmt*" ")^(Ni-1)*Fmt; 
         end
         F *= "]";
      else
         F *= Fmt;
      end
      F *= "\\n\"";
      F *= ",Is";
      if (HasK) 
         F *= ",k"; 
      end
      F *= ",\n"*" "^(Indent+3)
      if (HasI)
         if (HasJ)
            for i=0:(Ni-1)
               for j=0:(Nj-1)
                  F *= Sweep.Alias*"->"
                  F *= StructName;
                  if (HasK) F *= "[k]"; end
                  if (StructName != "") F *= "."; end
                  if (SubstructName !="") F *= SubstructName*"."; end
                  F *= VarName*"[$i][$j]";
                  if (i < Ni-1 || j < Nj-1)
                     F *= ",";
                  end
               end
            end
                  
         else
            for i = 0:(Ni-1)
               F *= Sweep.Alias*"->"
               F *= StructName;
               if (HasK) F *= "[k]"; end
               if (StructName != "") F *= "."; end
               if (SubstructName !="") F *= SubstructName*"."; end
               F *= VarName*"[$i]";
               if (i < Ni-1)
                  F *= ",";
               end
            end
         end
      else
         F *= Sweep.Alias*"->";
         F *= StructName;
         if (HasK) F *= "[k]"; end
         F *= "."*VarName;
      end
      F *= ");\n"

      F *= " "^Indent*"if (!strncmp(line,Prefix[Ipfx],PfxLen)) {\n"
      F *= " "^Indent*"   LineLen = strlen(line);\n"
      F *= " "^Indent*"   memcpy(&Msg[MsgLen],line,LineLen);\n"
      F *= " "^Indent*"   MsgLen += LineLen;\n"
      F *= " "^Indent*"   if (EchoEnabled) printf(\"%s\",line);\n";
      F *= " "^Indent*"}\n"

      if (HasK)
         Indent -= 3; 
         F *= " "^Indent*"}\n";
      end

      return(F);
end
################################################################################
function AddToReadFunction(F,Sweep,StructName,StructSize,SubstructName,SubstructSize,
   VarName,VarSize,VarType)
   
      Indent = 6;
   
      HasI = false;
      HasJ = false;
      HasK = false;
      
      if (StructSize != "")
         HasK = true;
         Nk = StructSize;
      end
      
      Ni = 1;
      Nj = 1;
      if (VarSize != "")
         HasI = true;
         if occursin(',',VarSize)
            HasJ = true;
            Nij = split(VarSize,',');
            Ni = parse(Int,Nij[1]);
            Nj = parse(Int,Nij[2]);
         else
            Ni = parse(Int,VarSize);
         end
      end
      
      if (VarType == "double")
         Fmt = "%le";
         ReadVar = "DbleVal";
      elseif (VarType == "long")
         Fmt = "%ld";
         ReadVar = "LongVal";
      end

      F *= "\n";
      F *= " "^Indent*"if (sscanf(line,\"";
      F *= Sweep.Name*"[%ld]."
      F *= StructName;
      if (HasK) F *= "[%ld]"; end
      if (StructName != "") F *= "."; end
      if (SubstructName !="") F *= SubstructName*"."; end
      F *= VarName;
      F *= " = ";
      if (HasI) 
         F *= "[";
         if (HasJ) 
            F *= (Fmt*" ")^(Ni*Nj-1)*Fmt; 
         else 
            F *= (Fmt*" ")^(Ni-1)*Fmt; 
         end
         F *= "]";
      else
         F *= Fmt;
      end
      F *= "\",\n"
      Indent += 3;
      F *= " "^Indent*"&Is"
      if (HasK) F*= ",&k"; end
      F *= ",\n"
      Nin = Ni*Nj+1;
      if (HasK) Nin += 1; end
      for i=0:Ni-1
         for j=0:Nj-1
            F *= " "^Indent*"&"*ReadVar*"[$(Nj*i+j)]"
            if (i<Ni-1 || j<Nj-1) F *= ",\n"; end
         end
      end
      F *= ") == $(Nin))\n"
      F *= " "^(Indent-3)*"{\n";
      if (HasI)
         if (HasJ)
            for i=0:Ni-1
               for j=0:Nj-1
                  F *= " "^Indent*Sweep.Name*"[Is]."
                  F *= StructName;
                  if (HasK) F *= "[k]"; end
                  if (StructName != "") F *= "."; end
                  if (SubstructName !="") F *= SubstructName*"."; end
                  F *= VarName*"[$i][$j] = "*ReadVar*"[$(Nj*i+j)];\n";
               end
            end
         else
            for i=0:Ni-1
               F *= " "^Indent*Sweep.Name*"[Is]."
               F *= StructName;
               if (HasK) F *= "[k]"; end
               if (StructName != "") F *= "."; end
               if (SubstructName !="") F *= SubstructName*"."; end
               F *= VarName*"[$i] = "*ReadVar*"[$i];\n";
            end
         end
      else
         F *= " "^Indent*Sweep.Name*"[Is]."
         F *= StructName;
         if (HasK) F *= "[k]"; end
         if (StructName != "") F *= "."; end
         if (SubstructName !="") F *= SubstructName*"."; end
         F *= VarName*" = "*ReadVar*"[0];\n";
      end
      
      if (Sweep.Name == "SC") 
         F *= " "^Indent*"SC[Is].RequestStateRefresh = 1;\n"
      end
      
      Indent -= 3;
      F *= " "^Indent*"}\n";

      return(F);
end
################################################################################
function BeginSweepWriteBlock(F,Sweep)

      F *= "         for(Is=0;Is<"*Sweep.Size*";Is++) {\n";
      F *= "            if ("*Sweep.Name*"[Is].Exists) {\n";
      F *= "               "*Sweep.Alias*" = &"*Sweep.Name*"[Is];\n";
      
      return(F);
end
################################################################################
function EndSweepWriteBlock(F)

      F *= "            }\n";
      F *= "         }\n\n";
      
      return(F);
end
################################################################################
function SweepTbl(Sweep,WriteFcnString,ReadFcnString)

# ... Find S struct
      Is = 0;
      for It in 1:Ntbl
         if Tbl[It]["Table Name"] == Sweep.Type;
            Is = It;
         end
      end
      S = Tbl[Is]["Table Data"];
      Nv = length(S);
      
# ... Loop over Outputs
      for Iv=1:Nv
         V = S[Iv];
         StructName = "";
         StructSize = "";
         SubstructName = "";
         SubstructSize = "";
         VarName = V["Variable Name"]
         if haskey(V,"Array Size")
            VarSize = V["Array Size"];
         else
            VarSize = "";
         end
         VarType = V["Data Type"];
         if haskey(V,"IPC Role")
            if (V["IPC Role"] == "Tx" || V["IPC Role"] == "TxRx")
               WriteFcnString = AddToWriteFunction(WriteFcnString,Sweep,
               StructName,StructSize,SubstructName,SubstructSize,VarName,
               VarSize,VarType);
            end
            if (V["IPC Role"] == "Rx" || V["IPC Role"] == "TxRx")
               ReadFcnString = AddToReadFunction(ReadFcnString,Sweep,StructName,
               StructSize,SubstructName,SubstructSize,VarName,VarSize,VarType);
            end
         end
      end
      
# ... Loop over structs within S
      for Iv=1:Nv
         Struct = S[Iv];
         if Struct["I/O Role"] == "STRUCT"
            # Find associated Data Type in Tbl
            Is = 0
            for It=1:Ntbl
               if Tbl[It]["Table Name"] == Struct["Data Type"]
                  Is = It;
               end
            end
            if (Is > 0)
               StructType = Tbl[Is]["Table Data"];
               # Loop over Outputs
               Nvv = length(StructType);
               for Ivv=1:Nvv
                  V = StructType[Ivv];
                  StructName = Struct["Variable Name"];
                  if haskey(Struct,"Array Size")
                     StructSize = Struct["Array Size"];
                  else
                     StructSize = "";
                  end
                  SubstructName = "";
                  SubstructSize = "";
                  VarName = V["Variable Name"];
                  if haskey(V,"Array Size")
                     VarSize = V["Array Size"];
                  else
                     VarSize = "";
                  end
                  VarType = V["Data Type"];
                  if haskey(V,"IPC Role")
                     if (V["IPC Role"] == "Tx" || V["IPC Role"] == "TxRx")
                        WriteFcnString = AddToWriteFunction(WriteFcnString,
                           Sweep,StructName,StructSize,SubstructName,
                           SubstructSize,VarName,VarSize,VarType);
                     end
                     if (V["IPC Role"] == "Rx" || V["IPC Role"] == "TxRx")
                        ReadFcnString = AddToReadFunction(ReadFcnString,Sweep,
                           StructName,StructSize,SubstructName,SubstructSize,
                           VarName,VarSize,VarType);
                     end
                  end
               end
               # Loop over substructs
               Nvv = length(StructType);
               for Ivv=1:Nvv
                  Substruct = StructType[Ivv];
                  if Substruct["I/O Role"] == "STRUCT"
                     # Find associated Data Type in Tbl
                     Iss = 0
                     for It=1:Ntbl
                        if Tbl[It]["Table Name"] == Substruct["Data Type"]
                           Iss = It;
                        end
                     end  
                     if (Iss > 0)
                        SubstructType = Tbl[Iss]["Table Data"];
                        # Loop over Outputs
                        Nvvv = length(SubstructType);
                        for Ivvv=1:Nvvv
                           V = SubstructType[Ivvv];
                           StructName = Struct["Variable Name"];
                           if haskey(Struct,"Array Size")
                              StructSize = Struct["Array Size"];
                           else
                              StructSize = "";
                           end
                           SubstructName = Substruct["Variable Name"];
                           if haskey(Substruct,"Array Size")
                              SubstructSize = Substruct["Array Size"];
                           else
                              SubstructSize = "";
                           end
                           VarName = V["Variable Name"];
                           if haskey(V,"Array Size")
                              VarSize = V["Array Size"];
                           else
                              VarSize = "";
                           end
                           VarType = V["Data Type"];
                           if haskey(V,"IPC Role")
                              if (V["IPC Role"] == "Tx" || V["IPC Role"] == "TxRx")
                                 WriteFcnString = AddToWriteFunction(WriteFcnString,
                                    Sweep,StructName,StructSize,SubstructName,
                                    SubstructSize,VarName,VarSize,VarType);
                              end
                              if (V["IPC Role"] == "Rx" || V["IPC Role"] == "TxRx")
                                 ReadFcnString = AddToReadFunction(ReadFcnString,Sweep,
                                    StructName,StructSize,SubstructName,SubstructSize,
                                    VarName,VarSize,VarType);
                              end
                           end
                        end
                     end
                  end
               end
            end
         end
      end
            
      return(WriteFcnString,ReadFcnString);
end
################################################################################
function main()
      ScJson = JSON.parsefile("./MetaCode/42.json")
      global Tbl = ScJson["Table Definition"]
      global Ntbl = length(Tbl)

      global WriteFcnString = "";
      global ReadFcnString ="";
      
      SC = SweepType("SCType","SC","S","Nsc");
      World = SweepType("WorldType","World","W","NWORLD");
      Orb = SweepType("OrbitType","Orb","O","Norb");
                       
      WriteFcnString = BeginSweepWriteBlock(WriteFcnString,SC);
      (WriteFcnString,ReadFcnString) = SweepTbl(SC,WriteFcnString,ReadFcnString);
      WriteFcnString = EndSweepWriteBlock(WriteFcnString);
      
      WriteFcnString = BeginSweepWriteBlock(WriteFcnString,World);
      (WriteFcnString,ReadFcnString) = SweepTbl(World,WriteFcnString,ReadFcnString);
      WriteFcnString = EndSweepWriteBlock(WriteFcnString);

      WriteFcnString = BeginSweepWriteBlock(WriteFcnString,Orb);
      (WriteFcnString,ReadFcnString) = SweepTbl(Orb,WriteFcnString,ReadFcnString);
      WriteFcnString = EndSweepWriteBlock(WriteFcnString);

      WriteFcnString = BeginWriteFunction(WriteFcnString);
      ReadFcnString = BeginReadFunction(ReadFcnString);
      
      WriteFcnString = EndWriteFunction(WriteFcnString);
      ReadFcnString = EndReadFunction(ReadFcnString);

# ... Write strings to cfile      

      cfile = open("./Source/AutoCode/TxRxIPC.c","w");
      
      Prolog(cfile);
      write(cfile,WriteFcnString);
      write(cfile,ReadFcnString);
      Epilog(cfile);
      
      close(cfile)
      
end
################################################################################
# Run this file if called from command line, like "julia JsonToSocket.jl"
if abspath(PROGRAM_FILE) == @__FILE__
   main()
end

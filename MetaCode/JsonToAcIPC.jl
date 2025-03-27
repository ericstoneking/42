# TODO:  Calculate size of buffer required, since we know what we're sending and 
#        the size of each variable.  Allocate buffer.



using JSON

global ScFile  # WriteInputsToAc, ReadOutputsFromAc
global AcFile  # WriteOutputsToSc, ReadInputsFromSc
global Tbl
global Ntbl

global ArrayName
global BufName


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
function Prolog()

      write(ScFile,"#include \"42.h\"\n");
      write(ScFile,"#define EXTERN extern\n");
      write(ScFile,"#include \"Ac.h\"\n");
      write(ScFile,"#undef EXTERN\n\n");

      write(AcFile,"#include \"Ac.h\"\n\n");

end
################################################################################
function BeginFindBufLenFunction(FindInBufLenString,InOutString)

      F = "/"*"*"^78*"/\n";
      F *= "long Find"*InOutString*"BufLen(struct AcType *AC)\n";
      F *= "{\n";
      
      F *= "      int BufLen = 0;\n";
      F *= "      int NumBytes;\n";
      if occursin("k=0",FindInBufLenString)
         F *= "      int k;\n";
      end
      if occursin("i=0",FindInBufLenString)
         F *= "      int i;\n";
      end
      if occursin("j=0",FindInBufLenString)
         F *= "      int j;\n";
      end

      write(ScFile,F);

end
################################################################################
function EndFindBufLenFunction()

      write(ScFile,"\n");
      write(ScFile,"      BufLen += 1; /* Pad for null termination */\n\n");
      write(ScFile,"      return((long) BufLen);\n");
      write(ScFile,"}\n");

end
################################################################################
function BeginWriteFunction(String,Role,File)

      F = "/"*"*"^78*"/\n";
      F *= "void WriteAc"*Role*"ToSocket(struct AcType *AC,struct AcIpcType *I)\n";
      F *= "{\n";
      F *= "      int Success;\n";
      F *= "      int NumBytes;\n";
      F *= "      char *BufPtr;\n";
      F *= "      char Ack[4] = \"Ack\\0\";\n";
      if occursin("k=0",String)
         F *= "      int k;\n";
      end
      if occursin("i=0",String)
         F *= "      int i;\n";
      end
      if occursin("j=0",String)
         F *= "      int j;\n";
      end
      F *= "\n";      
      F *= "      BufPtr = &I->Ac"*Role*"Buf[0];\n";

      write(File,F);

end
################################################################################
function EndWriteFunction(Role,File)

      write(File,"\n");
      write(File,"      memset(BufPtr,0,1);\n");
      write(File,"\n");
      write(File,"\n");
      write(File,"      Success = write(I->Socket,I->Ac"*Role*"Buf,I->Ac"*Role*"BufLen);\n");
      write(File,"      if (Success < 0) {\n");
      write(File,"         printf(\"Error writing to socket in WriteAc"*Role*"ToSocket.\\n\");\n");
      write(File,"         exit(1);\n");
      write(File,"      }\n");
      write(File,"      read(I->Socket,Ack,4);\n");
      write(File,"}\n");

end
################################################################################
function BeginReadFunction(String,Role,File)

      F = "/"*"*"^78*"/\n";
      F *= "void ReadAc"*Role*"FromSocket(struct AcType *AC,struct AcIpcType *I)\n";
      F *= "{\n";

      F *= "      int Success;\n";
      F *= "      int NumBytes;\n";
      F *= "      char *BufPtr;\n";
      F *= "      char Ack[4] = \"Ack\\0\";\n";
      if occursin("k=0",String)
         F *= "      int k;\n";
      end
      if occursin("i=0",String)
         F *= "      int i;\n";
      end
      if occursin("j=0",String)
         F *= "      int j;\n";
      end
      F *= "\n";      
      F *= "      BufPtr = &I->Ac"*Role*"Buf[0];\n";
      
      F *= "      Success = read(I->Socket,I->Ac"*Role*"Buf,I->Ac"*Role*"BufLen);\n";
      F *= "      if (Success < 0) {\n";
      F *= "         printf(\"Error reading from socket in ReadAc"*Role*"FromSocket.\\n\");\n";
      F *= "         exit(1);\n";
      F *= "      }\n";
      F *= "      write(I->Socket,Ack,4);\n";

      write(File,F);

end
################################################################################
function EndReadFunction(File)

      write(File,"}\n");

end
################################################################################
function AddToWriteString(D,StructName,StructSize,SubstructName,SubstructSize,
   VarName,VarSize,VarType)
   
      Indent = 6;
   
      HasI = false;
      HasJ = false;
      HasK = false;
      
      if (StructSize != "")
         HasK = true;
         Nk = StructSize;
      end
      
      if (VarSize != "")
         HasI = true;
         if occursin(',',VarSize)
            HasJ = true;
            Nij = split(VarSize,',');
            Ni = Nij[1];
            Nj = Nij[2];
         else
            Ni = VarSize;
         end
      end
      
      if (VarType == "double")
         NumBytes = "sizeof(double)";
      elseif (VarType == "long")
         NumBytes = "sizeof(long)";
      end

      D *= "\n";
      D *= " "^Indent*"NumBytes = "*NumBytes*";\n";
      if (HasK)
         D *= " "^Indent*"for(k=0;k<AC->$(Nk);k++) {\n"
         Indent += 3;
      end
      if (HasI)
         D *= " "^Indent*"for(i=0;i<$(Ni);i++) {\n"
         Indent += 3;
      end      
      if (HasJ)
         D *= " "^Indent*"for(j=0;j<$(Nj);j++) {\n"
         Indent += 3;
      end      
      D *= " "^Indent*"memcpy(BufPtr,&AC->";
      D *= StructName;
      if (HasK) D *= "[k]"; end
      if (StructName != "") D *= "."; end
      if (SubstructName !="") D *= SubstructName*"."; end
      D *= VarName
      if (HasI) D *= "[i]"; end
      if (HasJ) D *= "[j]"; end
      D *= ",NumBytes);\n"
      D *= " "^Indent*"BufPtr += NumBytes;\n";
      if (HasJ)
         Indent -= 3;
         D *= " "^Indent*"}\n"
      end
      if (HasI)
         Indent -= 3;
         D *= " "^Indent*"}\n"
      end
      if (HasK)
         Indent -= 3;
         D *= " "^Indent*"}\n"
      end

      return(D);
end;
################################################################################
function AddToFindBufLenString(D,StructName,StructSize,SubstructName,SubstructSize,
   VarName,VarSize,VarType)
   
      Indent = 6;
   
      HasI = false;
      HasJ = false;
      HasK = false;
      
      if (StructSize != "")
         HasK = true;
         Nk = StructSize;
      end
      
      if (VarSize != "")
         HasI = true;
         if occursin(',',VarSize)
            HasJ = true;
            Nij = split(VarSize,',');
            Ni = Nij[1];
            Nj = Nij[2];
         else
            Ni = VarSize;
         end
      end
      
      if (VarType == "double")
         NumBytes = "sizeof(double)";
      elseif (VarType == "long")
         NumBytes = "sizeof(long)";
      end

      D *= "\n";
      D *= " "^Indent*"NumBytes = "*NumBytes*";\n";
      if (HasK)
         D *= " "^Indent*"for(k=0;k<AC->$(Nk);k++) {\n"
         Indent += 3;
      end
      if (HasI)
         D *= " "^Indent*"for(i=0;i<$(Ni);i++) {\n"
         Indent += 3;
      end      
      if (HasJ)
         D *= " "^Indent*"for(j=0;j<$(Nj);j++) {\n"
         Indent += 3;
      end      
      D *= " "^Indent*"BufLen += NumBytes;\n";
      if (HasJ)
         Indent -= 3;
         D *= " "^Indent*"}\n"
      end
      if (HasI)
         Indent -= 3;
         D *= " "^Indent*"}\n"
      end
      if (HasK)
         Indent -= 3;
         D *= " "^Indent*"}\n"
      end

      return(D);
end;
################################################################################
function AddToReadString(D,StructName,StructSize,SubstructName,SubstructSize,
   VarName,VarSize,VarType)
   
      Indent = 6;
   
      HasI = false;
      HasJ = false;
      HasK = false;
      
      if (StructSize != "")
         HasK = true;
         Nk = StructSize;
      end
      
      if (VarSize != "")
         HasI = true;
         if occursin(',',VarSize)
            HasJ = true;
            Nij = split(VarSize,',');
            Ni = Nij[1];
            Nj = Nij[2];
         else
            Ni = VarSize;
         end
      end
      
      if (VarType == "double")
         NumBytes = "sizeof(double)";
      elseif (VarType == "long")
         NumBytes = "sizeof(long)";
      end

      D *= "\n";
      D *= " "^Indent*"NumBytes = "*NumBytes*";\n";
      if (HasK)
         D *= " "^Indent*"for(k=0;k<AC->$(Nk);k++) {\n"
         Indent += 3;
      end
      if (HasI)
         D *= " "^Indent*"for(i=0;i<$(Ni);i++) {\n"
         Indent += 3;
      end      
      if (HasJ)
         D *= " "^Indent*"for(j=0;j<$(Nj);j++) {\n"
         Indent += 3;
      end      
      D *= " "^Indent*"memcpy(&AC->";
      D *= StructName;
      if (HasK) D *= "[k]"; end
      if (StructName != "") D *= "."; end
      if (SubstructName !="") D *= SubstructName*"."; end
      D *= VarName
      if (HasI) D *= "[i]"; end
      if (HasJ) D *= "[j]"; end
      D *= ",BufPtr,NumBytes);\n"
      D *= " "^Indent*"BufPtr += NumBytes;\n";
      if (HasJ)
         Indent -= 3;
         D *= " "^Indent*"}\n"
      end
      if (HasI)
         Indent -= 3;
         D *= " "^Indent*"}\n"
      end
      if (HasK)
         Indent -= 3;
         D *= " "^Indent*"}\n"
      end

      return(D);
end;
################################################################################
function BuildScArraySizeWriteString()

      N = length(ArrayName);

      F = "/"*"*"^78*"/\n";
      F *= "void WriteAcArraySizesToSocket(struct AcType *AC, struct AcIpcType *I)\n";
      F *= "{\n";
      F *= "      char *Msg;\n";
      F *= "      char Ack[4] = \"Ack\\0\";\n";
      F *= "      int NumBytes;\n";
      F *= "      int MsgLen = 0;\n";
      F *= "      int Success;\n";
      F *= "\n";
      F *= "      Msg = (char *) calloc($N+1,sizeof(long));\n";
      F *= "\n";
      for i=1:N
         F *= "      NumBytes = sizeof(long);\n";
         F *= "      memcpy(&Msg[MsgLen],&AC->"*ArrayName[i]*",NumBytes);\n";
         F *= "      MsgLen += NumBytes;\n\n";
      end
      
      F *= "      memset(&Msg[MsgLen],0,1);\n\n";
      
      F *= "      Success = write(I->Socket,Msg,MsgLen);\n";
      F *= "      if (Success < 0) {\n";
      F *= "         printf(\"Error writing to socket in WriteAcArraySizesToSocket.\\n\");\n";
      F *= "         exit(1);\n";
      F *= "      }\n";
      F *= "      free(Msg);\n";
      F *= "      read(I->Socket,Ack,4);\n";
      F *= "}\n";

      return(F);
end;
################################################################################
function BuildScBufLenWriteString()

      N = length(BufName);

      F = "/"*"*"^78*"/\n";
      F *= "void WriteAcBufLensToSocket(struct AcIpcType *I)\n";
      F *= "{\n";
      F *= "      char *Msg;\n";
      F *= "      char Ack[4] = \"Ack\\0\";\n";
      F *= "      int NumBytes;\n";
      F *= "      int MsgLen = 0;\n";
      F *= "      int Success;\n";
      F *= "\n";
      F *= "      Msg = (char *) calloc($N,sizeof(long));\n";
      F *= "\n";
      for i=1:N
         F *= "      NumBytes = sizeof(long);\n";
         F *= "      memcpy(&Msg[MsgLen],&I->"*BufName[i]*",NumBytes);\n";
         F *= "      MsgLen += NumBytes;\n\n";
      end
      
      F *= "      memset(&Msg[MsgLen],0,1);\n\n";

      F *= "      Success = write(I->Socket,Msg,MsgLen);\n";
      F *= "      if (Success < 0) {\n";
      F *= "         printf(\"Error writing to socket in WriteAcBufLensToSocket.\\n\");\n";
      F *= "         exit(1);\n";
      F *= "      }\n";
      F *= "      free(Msg);\n";
      F *= "      read(I->Socket,Ack,4);\n";
      F *= "}\n";

      return(F);
end;
################################################################################
function BuildAcArraySizeReadString()

      N = length(ArrayName);

      F = "/"*"*"^78*"/\n";
      F *= "void ReadAcArraySizesFromSocket(struct AcType *AC, struct AcIpcType *I)\n";
      F *= "{\n";
      F *= "      int Success;\n";
      F *= "      char Msg[256];\n";
      F *= "      char Ack[4] = \"Ack\\0\";\n";
      F *= "      int MsgIdx = 0;\n";
      F *= "      int NumBytes;\n";
      F *= "\n";      
      F *= "      Success = read(I->Socket,Msg,255);\n";
      F *= "      if (Success < 0) {\n";
      F *= "         printf(\"Error reading from socket in ReadAcArraySizesFromSocket.\\n\");\n";
      F *= "         exit(1);\n";
      F *= "      }\n";
      F *= "      write(I->Socket,Ack,4);\n\n";

      for i=1:N
         F *= "      NumBytes = sizeof(long);\n";
         F *= "      memcpy(&AC->"*ArrayName[i]*",&Msg[MsgIdx],NumBytes);\n";
         F *= "      MsgIdx += NumBytes;\n\n";
      end

      F *= "}\n";

      return(F);
end;
################################################################################
function BuildAcBufLenReadString()

      N = length(BufName);

      F = "/"*"*"^78*"/\n";
      F *= "void ReadAcBufLensFromSocket(struct AcIpcType *I)\n";
      F *= "{\n";
      F *= "      int Success;\n";
      F *= "      char Msg[256];\n";
      F *= "      char Ack[4] = \"Ack\\0\";\n";
      F *= "      int MsgIdx = 0;\n";
      F *= "      int NumBytes;\n";
      F *= "\n";      
      F *= "      Success = read(I->Socket,Msg,255);\n";
      F *= "      if (Success < 0) {\n";
      F *= "         printf(\"Error reading from socket in ReadAcBufLensFromSocket.\\n\");\n";
      F *= "         exit(1);\n";
      F *= "      }\n";
      F *= "      write(I->Socket,Ack,4);\n\n";

      for i=1:N
         F *= "      NumBytes = sizeof(long);\n";
         F *= "      memcpy(&I->"*BufName[i]*",&Msg[MsgIdx],NumBytes);\n";
         F *= "      MsgIdx += NumBytes;\n\n";
      end
      
      F *= "}\n";

      return(F);
end;
################################################################################
function Epilog()

end
################################################################################
function main()
      AcJson = JSON.parsefile("./MetaCode/Ac.json")
      global Tbl = AcJson["Table Definition"]
      global Ntbl = length(Tbl)

      global ScFile = open("./Source/AutoCode/ScIPC.c","w");
      global AcFile = open("./Source/AutoCode/AcIPC.c","w");
      
      FindInBufLenString = "";
      FindOutBufLenString = "";
      FindTblBufLenString = "";
      
      ScWriteString = "";
      ScReadString = "";
      AcWriteString = "";
      AcReadString = "";
      
      ScTblWriteString = "";
      AcTblReadString = "";
      
      ScArraySizeWriteString = "";
      AcArraySizeReadString = "";
      
      ScBufLenWriteString = "";
      AcBufLenReadString = "";
      
      global ArrayName = ["Nb","Ng","Nwhl","Nmtb","Nthr","Ngyro","Nmag","Ncss","Nfss","Nst","Ngps","Nacc"];
      global BufName = ["AcInBufLen","AcOutBufLen","AcTblBufLen"];
      
# ... Find AC struct
      Iac = 0;
      for It in 1:Ntbl
         if Tbl[It]["Table Name"] == "AcType";
            Iac = It;
         end
      end
      Ac = Tbl[Iac]["Table Data"];
      Nv = length(Ac);
      
# ... Loop over Inputs/Outputs
      # Loop over Inputs/Outputs
      for Iv=1:Nv
         V = Ac[Iv];
         if (V["I/O Role"] == "INPUT" || V["I/O Role"] == "OUTPUT" || 
             V["I/O Role"] == "CMD_PRM" || V["I/O Role"] == "TBL_PRM")
            StructName = "";
            StructSize = "";
            SubstructName = "";
            SubstructSize = "";
            VarName = V["Variable Name"];
            if haskey(V,"Array Size")
               VarSize = V["Array Size"];
            else
               VarSize = "";
            end
            VarType = V["Data Type"];
            if (V["I/O Role"] == "INPUT" || V["I/O Role"] == "CMD_PRM")
               ScWriteString = AddToWriteString(ScWriteString,StructName,StructSize,
                  SubstructName,SubstructSize,VarName,VarSize,VarType);
               AcReadString = AddToReadString(AcReadString,StructName,StructSize,
                  SubstructName,SubstructSize,VarName,VarSize,VarType);
               FindInBufLenString = AddToFindBufLenString(FindInBufLenString,StructName,StructSize,
                  SubstructName,SubstructSize,VarName,VarSize,VarType);
            end
            if (V["I/O Role"] == "OUTPUT" || V["I/O Role"] == "CMD_PRM")
               ScReadString = AddToReadString(ScReadString,StructName,StructSize,
                  SubstructName,SubstructSize,VarName,VarSize,VarType);
               AcWriteString = AddToWriteString(AcWriteString,StructName,StructSize,
                  SubstructName,SubstructSize,VarName,VarSize,VarType);
               FindOutBufLenString = AddToFindBufLenString(FindOutBufLenString,StructName,StructSize,
                  SubstructName,SubstructSize,VarName,VarSize,VarType);
            end
            if (V["I/O Role"] == "TBL_PRM")
               ScTblWriteString = AddToWriteString(ScTblWriteString,StructName,StructSize,
                  SubstructName,SubstructSize,VarName,VarSize,VarType);
               AcTblReadString = AddToReadString(AcTblReadString,StructName,StructSize,
                  SubstructName,SubstructSize,VarName,VarSize,VarType);
               FindTblBufLenString = AddToFindBufLenString(FindTblBufLenString,StructName,StructSize,
                  SubstructName,SubstructSize,VarName,VarSize,VarType);
            end
         end
      end

# ... Loop over structs within Ac
      for Iv=1:Nv
         Struct = Ac[Iv];
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
               Name = Struct["Variable Name"];
               # Loop over Outputs
               Nvv = length(StructType);
               for Ivv=1:Nvv
                  V = StructType[Ivv];
                  if (V["I/O Role"] == "INPUT" || V["I/O Role"] == "OUTPUT" || 
                      V["I/O Role"] == "CMD_PRM" || V["I/O Role"] == "TBL_PRM")
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
                     if (V["I/O Role"] == "INPUT" || V["I/O Role"] == "CMD_PRM")
                        ScWriteString = AddToWriteString(ScWriteString,StructName,StructSize,
                           SubstructName,SubstructSize,VarName,VarSize,VarType);
                        AcReadString = AddToReadString(AcReadString,StructName,StructSize,
                           SubstructName,SubstructSize,VarName,VarSize,VarType);
                        FindInBufLenString = AddToFindBufLenString(FindInBufLenString,StructName,StructSize,
                           SubstructName,SubstructSize,VarName,VarSize,VarType);
                     end
                     if (V["I/O Role"] == "OUTPUT" || V["I/O Role"] == "CMD_PRM")
                        ScReadString = AddToReadString(ScReadString,StructName,StructSize,
                           SubstructName,SubstructSize,VarName,VarSize,VarType);
                        AcWriteString = AddToWriteString(AcWriteString,StructName,StructSize,
                           SubstructName,SubstructSize,VarName,VarSize,VarType);
                        FindOutBufLenString = AddToFindBufLenString(FindOutBufLenString,StructName,StructSize,
                           SubstructName,SubstructSize,VarName,VarSize,VarType);
                     end
                     if (V["I/O Role"] == "TBL_PRM")
                        ScTblWriteString = AddToWriteString(ScTblWriteString,StructName,StructSize,
                           SubstructName,SubstructSize,VarName,VarSize,VarType);
                        AcTblReadString = AddToReadString(AcTblReadString,StructName,StructSize,
                           SubstructName,SubstructSize,VarName,VarSize,VarType);
                        FindTblBufLenString = AddToFindBufLenString(FindTblBufLenString,StructName,StructSize,
                           SubstructName,SubstructSize,VarName,VarSize,VarType);
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
                        Name = "_"*Substruct["Variable Name"];
                        # Loop over Outputs
                        Nvvv = length(SubstructType);
                        for Ivvv=1:Nvvv
                           V = SubstructType[Ivvv];
                           if (V["I/O Role"] == "INPUT" || V["I/O Role"] == "OUTPUT" || 
                               V["I/O Role"] == "CMD_PRM" || V["I/O Role"] == "TBL_PRM")
                              # Build Header string
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
                              if (V["I/O Role"] == "INPUT" || V["I/O Role"] == "CMD_PRM")
                                 ScWriteString = AddToWriteString(ScWriteString,StructName,StructSize,
                                    SubstructName,SubstructSize,VarName,VarSize,VarType);
                                 AcReadString = AddToReadString(AcReadString,StructName,StructSize,
                                    SubstructName,SubstructSize,VarName,VarSize,VarType);
                                 FindInBufLenString = AddToFindBufLenString(FindInBufLenString,StructName,StructSize,
                                    SubstructName,SubstructSize,VarName,VarSize,VarType);
                              end
                              if (V["I/O Role"] == "OUTPUT" || V["I/O Role"] == "CMD_PRM")
                                 ScReadString = AddToReadString(ScReadString,StructName,StructSize,
                                    SubstructName,SubstructSize,VarName,VarSize,VarType);
                                 AcWriteString = AddToWriteString(AcWriteString,StructName,StructSize,
                                    SubstructName,SubstructSize,VarName,VarSize,VarType);
                                 FindOutBufLenString = AddToFindBufLenString(FindOutBufLenString,StructName,StructSize,
                                    SubstructName,SubstructSize,VarName,VarSize,VarType);
                              end
                              if (V["I/O Role"] == "TBL_PRM")
                                 ScTblWriteString = AddToWriteString(ScTblWriteString,StructName,StructSize,
                                    SubstructName,SubstructSize,VarName,VarSize,VarType);
                                 AcTblReadString = AddToReadString(AcTblReadString,StructName,StructSize,
                                    SubstructName,SubstructSize,VarName,VarSize,VarType);
                                 FindTblBufLenString = AddToFindBufLenString(FindTblBufLenString,StructName,StructSize,
                                    SubstructName,SubstructSize,VarName,VarSize,VarType);
                              end
                           end
                        end
                     end
                  end
               end
            end
         end
      end
               
# ... Write strings to file      

      Prolog();
      
      BeginFindBufLenFunction(FindInBufLenString,"In");
      write(ScFile,FindInBufLenString);
      EndFindBufLenFunction();
      
      BeginFindBufLenFunction(FindOutBufLenString,"Out");
      write(ScFile,FindOutBufLenString);
      EndFindBufLenFunction();

      BeginFindBufLenFunction(FindTblBufLenString,"Tbl");
      write(ScFile,FindTblBufLenString);
      EndFindBufLenFunction();

      
      BeginWriteFunction(ScWriteString,"In",ScFile);
      write(ScFile,ScWriteString);
      EndWriteFunction("In",ScFile);
      
      BeginReadFunction(ScReadString,"Out",ScFile);
      write(ScFile,ScReadString);
      EndReadFunction(ScFile);
      
      BeginWriteFunction(ScTblWriteString,"Tbl",ScFile);
      write(ScFile,ScTblWriteString);
      EndWriteFunction("Tbl",ScFile);

      
      BeginWriteFunction(AcWriteString,"Out",AcFile);
      write(AcFile,AcWriteString);
      EndWriteFunction("Out",AcFile);
      
      BeginReadFunction(AcReadString,"In",AcFile);
      write(AcFile,AcReadString);
      EndReadFunction(AcFile);
      
      BeginReadFunction(AcTblReadString,"Tbl",AcFile);
      write(AcFile,AcTblReadString);
      EndReadFunction(AcFile);
      
      ScArraySizeWriteString = BuildScArraySizeWriteString();
      write(ScFile,ScArraySizeWriteString);
      ScBufLenWriteString = BuildScBufLenWriteString();
      write(ScFile,ScBufLenWriteString);
      AcArraySizeReadString = BuildAcArraySizeReadString();
      write(AcFile,AcArraySizeReadString);
      AcBufLenReadString = BuildAcBufLenReadString();
      write(AcFile,AcBufLenReadString);

      Epilog()
      
      close(ScFile)
      close(AcFile)
      
end
################################################################################
# Run this file if called from command line, like "julia JsonToAcIPC.jl"
if abspath(PROGRAM_FILE) == @__FILE__
   main()
end

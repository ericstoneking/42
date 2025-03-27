using JSON

global cfile
global inpfile
global Tbl
global Ntbl

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

      write(cfile,"#include \"42.h\"\n");
      write(cfile,"#define EXTERN extern\n");
      write(cfile,"#include \"Ac.h\"\n");
      write(cfile,"#undef EXTERN\n\n");

      write(inpfile,"<<<<<<<<<<<<<<<<<<  AC Outputs for 42  >>>>>>>>>>>>>>>>>>>\n");

end
################################################################################
function AddToHeaderString(H,StructName,StructSize,SubstructName,SubstructSize,
   VarName,VarSize)
   
      Indent = 15;
   
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

      if (HasK)
         H *= " "^Indent*"for(k=0;k<AC->$(Nk);k++) {\n"
         Indent += 3;
      end
      if (HasI)
         H *= " "^Indent*"for(i=0;i<$(Ni);i++) {\n"
         Indent += 3;
      end      
      if (HasJ)
         H *= " "^Indent*"for(j=0;j<$(Nj);j++) {\n"
         Indent += 3;
      end      
      H *= " "^Indent*"fprintf(outfile[Isc],\",Ac_";
      H *= StructName;
      if (HasK) H *= "%ld_"; end
      H *= VarName
      if (HasI) H *= "_%ld"; end
      if (HasJ) H *= "_%ld"; end
      H *= "\""
      if (HasK) H *= ",k"; end
      if (HasI) H *= ",i"; end
      if (HasJ) H *= ",j"; end
      H *= ");\n"
      if (HasJ)
         Indent -= 3;
         H *= " "^Indent*"}\n"
      end
      if (HasI)
         Indent -= 3;
         H *= " "^Indent*"}\n"
      end
      if (HasK)
         Indent -= 3;
         H *= " "^Indent*"}\n"
      end

      return(H);
end;
################################################################################
function AddToDataString(D,StructName,StructSize,SubstructName,SubstructSize,
   VarName,VarSize,VarType)
   
      Indent = 15;
   
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
         fmt = "%18.12le";
      elseif (VarType == "long")
         fmt = "%ld";
      end

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
      D *= " "^Indent*"fprintf(outfile[Isc],\","*fmt*"\",AC->";
      D *= StructName;
      if (HasK) D *= "[k]"; end
      if (StructName != "") D *= "."; end
      if (SubstructName !="") D *= SubstructName*"."; end
      D *= VarName
      if (HasI) D *= "[i]"; end
      if (HasJ) D *= "[j]"; end
      D *= ");\n"
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
function AddToFunctionString(F,Name,HeaderString,DataString)

      F *= "/"*"*"^78*"/\n";
      F *= "void Write$(Name)ToCsv(void)\n{\n";
      F *= "      struct AcType *AC;\n";
      F *= "      static FILE **outfile;\n";
      F *= "      char FileName[80];\n";
      F *= "      long Isc;\n";
      if occursin("k=0",HeaderString)
         F *= "      long k;\n";
      end
      if occursin("i=0",HeaderString)
         F *= "      long i;\n";
      end
      if occursin("j=0",HeaderString)
         F *= "      long j;\n";
      end
      F *= "      static long First = 1;\n";
      F *= "      static long OutCtr = 1000000000;\n\n";
      F *= "      if (First) {\n";
      F *= "         First = 0;\n";
      F *= "         outfile = (FILE**) calloc(Nsc,sizeof(FILE *));\n";
      F *= "         for(Isc=0;Isc<Nsc;Isc++) {\n";
      F *= "            if (SC[Isc].Exists) {\n";
      F *= "               AC = &SC[Isc].AC;\n";
      F *= "               if (Nsc==1) sprintf(FileName,\"$(Name).csv\");\n";
      F *= "               else if (Nsc<=10) sprintf(FileName,\"$(Name)%1ld.csv\",Isc);\n";
      F *= "               else sprintf(FileName,\"$(Name)%02ld.csv\",Isc);\n";
      F *= "               outfile[Isc] = FileOpen(InOutPath,FileName,\"w\");\n\n";
      F *= "               fprintf(outfile[Isc],\"$(Name)_Time\");\n";
      F *= HeaderString;
      F *= "               fprintf(outfile[Isc],\"\\n\");\n";
      F *= "            }\n";
      F *= "         }\n";
      F *= "      }\n\n";

      F *= "      OutCtr++;\n";
      F *= "      if (OutCtr > Max$(Name)Ctr) {\n";
      F *= "         OutCtr = 1;\n";
      F *= "         for (Isc=0;Isc<Nsc;Isc++) {\n";
      F *= "            if (SC[Isc].Exists) {\n";
      F *= "               AC = &SC[Isc].AC;\n";
      F *= "               fprintf(outfile[Isc],\"%18.12le\",AC->Time);\n";
      F *= DataString;
      F *= "               fprintf(outfile[Isc],\"\\n\");\n"
      F *= "            }\n";
      F *= "         }\n";
      F *= "      }\n\n";
      F *= "}\n";
      
      return(F);
end
################################################################################
function BeginMainFunction()

      write(cfile,"/"*"*"^78*"/\n");
      write(cfile,"void WriteAcVarsToCsv(void)\n{\n");
      
      write(cfile,"      FILE *infile;\n");
      write(cfile,"      char response[80],junk[80],newline;\n");
      write(cfile,"      double dt;\n");
      write(cfile,"      static long First = 1;\n\n");

      write(cfile,"      if (First) {\n");
      write(cfile,"         First = 0;\n\n");
      write(cfile,"         infile = FileOpen(InOutPath,\"Inp_AcOutput.txt\",\"r\");\n");
      write(cfile,"         fscanf(infile,\"%[^\\n] %[\\n]\",junk,&newline);\n\n");

      
end
################################################################################
function AddToFirstString(F,Name)

      F *= "         fscanf(infile,\"%s %lf %[^\\n] %[\\n]\",response,&dt,junk,&newline);\n"
      F *= "         $(Name)TlmEnabled = DecodeString(response);\n"
      F *= "         if (dt < DTSIM) {\n"
      F *= "            printf(\"$(Name) timestep < DTSIM.  You'll want to fix that.\");\n"
      F *= "            exit(1);\n"
      F *= "         }\n"
      F *= "         else Max$(Name)Ctr = (long) (dt/DTSIM+0.5);\n\n"
      
      return(F);
end
################################################################################
function Epilog()

      write(cfile,"\n}\n");
      
      write(inpfile,"\n");

end
################################################################################
function main()
      AcJson = JSON.parsefile("./MetaCode/Ac.json")
      global Tbl = AcJson["Table Definition"]
      global Ntbl = length(Tbl)

      global cfile = open("./Source/AutoCode/WriteAcToCsv.c","w");
      global inpfile = open("./MetaCode/Inp_AcOutput.txt","w");
      
      DeclareString = "long AcTlmEnabled, MaxAcCtr;\n"
      CallString = "      if (AcTlmEnabled) WriteAcToCsv();\n";
      EnableString = "TRUE  0.25   ! Ac Tlm Enabled, Interval\n";
      FunctionString = "";
      FirstString = "";
      
      Prolog();
      
# ... Find AC struct
      Iac = 0;
      for It in 1:Ntbl
         if Tbl[It]["Table Name"] == "AcType";
            Iac = It;
         end
      end
      Ac = Tbl[Iac]["Table Data"];
      Nv = length(Ac);
      
# ... Loop over Outputs
      # Loop over Outputs
      HeaderString = "";
      DataString = "";
      for Iv=1:Nv
         V = Ac[Iv];
         if (V["I/O Role"] == "INPUT" || 
             V["I/O Role"] == "OUTPUT" || 
             V["I/O Role"] == "CMD_PRM")
            #write(cfile,V["Variable Name"]);
            if haskey(V,"Array Size")
               #write(cfile," "*V["Array Size"]);
            end
            #write(cfile,"\n");
            # Build Header string
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
            HeaderString = AddToHeaderString(HeaderString,StructName,StructSize,
               SubstructName,SubstructSize,VarName,VarSize)
            DataString = AddToDataString(DataString,StructName,StructSize,
               SubstructName,SubstructSize,VarName,VarSize,VarType);
         end
      end
      # Write Function
      FunctionString = AddToFunctionString(FunctionString,"Ac",HeaderString,
         DataString);
      FirstString = AddToFirstString(FirstString,"Ac");
      
# ... Loop over structs within Ac
      for Iv=1:Nv
         HeaderString = "";
         DataString = "";
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
               #write(cfile,Name);
               if haskey(Struct,"Array Size")
                  #write(cfile," "*Struct["Array Size"]);
               end
               #write(cfile,"\n");               
               # Loop over Outputs
               Nvv = length(StructType);
               HasInOut = false;
               for Ivv=1:Nvv
                  V = StructType[Ivv];
                  if (V["I/O Role"] == "INPUT" || 
                      V["I/O Role"] == "OUTPUT" || 
                      V["I/O Role"] == "CMD_PRM")
                     #write(cfile,"   "*V["Variable Name"]);
                     if haskey(V,"Array Size")
                        #write(cfile," "*V["Array Size"]);
                     end
                     #write(cfile,"\n");
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
                     HeaderString = AddToHeaderString(HeaderString,StructName,
                        StructSize,SubstructName,SubstructSize,VarName,VarSize)
                     DataString = AddToDataString(DataString,StructName,
                        StructSize,SubstructName,SubstructSize,VarName,VarSize,
                        VarType);
                     HasInOut = true;
                  end
                  # Build Data string
               end
               if (HasInOut)
                  # Build Declare string
                  DeclareString *= "long Ac$(Name)TlmEnabled, MaxAc$(Name)Ctr;\n";
                  # Build Enable string
                  EnableString *= "FALSE  5.0   ! Ac$(Name) Tlm Enabled, Interval\n";
                  # Build Call string
                  CallString *= "      if (Ac$(Name)TlmEnabled) WriteAc$(Name)ToCsv();\n"
                  # Write Function
                  FunctionString = AddToFunctionString(FunctionString,"Ac$(Name)",
                     HeaderString,DataString);
                  FirstString = AddToFirstString(FirstString,"Ac$(Name)");
               end
               # Loop over substructs
               Nvv = length(StructType);
               for Ivv=1:Nvv
                  HeaderString = "";
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
                        #write(cfile,"   "*Name);
                        if haskey(Substruct,"Array Size")
                           #write(cfile," "*Substruct["Array Size"]);
                        end
                        #write(cfile,"\n");
                        # Loop over Outputs
                        Nvvv = length(SubstructType);
                        HasInOut = false;
                        for Ivvv=1:Nvvv
                           V = SubstructType[Ivvv];
                           if (V["I/O Role"] == "INPUT" || 
                               V["I/O Role"] == "OUTPUT" || 
                               V["I/O Role"] == "CMD_PRM")
                              #write(cfile,"      "*V["Variable Name"]*" "*V["Array Size"]*"\n");
                              HasInOut = true;
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
                              HeaderString = AddToHeaderString(HeaderString,
                                 StructName,StructSize,SubstructName,
                                 SubstructSize,VarName,VarSize)
                              DataString = AddToDataString(DataString,StructName,
                                 StructSize,SubstructName,SubstructSize,VarName,
                                 VarSize,VarType);
                           end
                        end
                        if (HasInOut)
                           # Build Declare string
                           DeclareString *= "long Ac$(Name)TlmEnabled, MaxAc$(Name)Ctr;\n";
                           # Build Enable string
                           EnableString *= "FALSE  5.0   ! Ac$(Name) Tlm Enabled, Interval\n";
                           # Build Call string
                           CallString *= "      if (Ac$(Name)TlmEnabled) WriteAc$(Name)ToCsv();\n"
                           # Write Function
                           FunctionString = AddToFunctionString(FunctionString,
                              "Ac$(Name)",HeaderString,DataString);
                           FirstString = AddToFirstString(FirstString,"Ac$(Name)");
                        end
                     end
                  end
               end
            end
         end
      end
               
# ... Write strings to cfile      

      write(cfile,DeclareString);
      
      write(cfile,FunctionString);
      
      BeginMainFunction();
      write(cfile,FirstString);
      write(cfile,"      }\n\n");
      
      write(cfile,CallString);

      write(inpfile,EnableString);

      Epilog()
      
      close(cfile)
      close(inpfile)
      
end
################################################################################
# Run this file if called from command line, like "julia JsonToAcCsv.jl"
if abspath(PROGRAM_FILE) == @__FILE__
   main()
end

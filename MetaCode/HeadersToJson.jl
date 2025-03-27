
################################################################################
function WriteTableDefProlog(StructType,IsFirstStruct)

      if (!IsFirstStruct)
         write(outfile,",\n");
      end
      write(outfile,"      {\n");
      
      Indent = " "^9;
      write(outfile,Indent*"\"Table Name\": \""*StructType*"\",\n");
      write(outfile,Indent*"\"Table Type\": \"Structure\",\n");
      write(outfile,Indent*"\"Table Data\": [");
end;
################################################################################
function WriteTableEntry(Name,Type,Size,InOutRole,IpcRole,Units,Abbrev,Description,IsFirstEntry)

      if (Name != "GN")
         if !IsFirstEntry
            write(outfile,",");
         end
         write(outfile,"\n");

         Indent = " "^12;
         write(outfile,Indent*"{\n");
      
         write(outfile,Indent*"   \"Variable Name\": \""*Name*"\"");
         if Abbrev != ""
            write(outfile,",\n"*Indent*"   \"Abbreviation\": \""*Abbrev*"\"");
         end
         if InOutRole != "INTERNAL"
            write(outfile,",\n"*Indent*"   \"I/O Role\": \""*InOutRole*"\"");
         end
         if IpcRole != ""
            write(outfile,",\n"*Indent*"   \"IPC Role\": \""*IpcRole*"\"");
         end
         write(outfile,",\n"*Indent*"   \"Data Type\": \""*Type*"\"");
         if Size != "1"
            write(outfile,",\n"*Indent*"   \"Array Size\": \""*Size*"\"");
         end
         if Units != ""
            write(outfile,",\n"*Indent*"   \"Units\": \""*Units*"\"");
         end
         if Description != ""
            write(outfile,",\n"*Indent*"   \"Description\": \""*Description*"\"");
         end
         write(outfile,"\n"*Indent*"}");
      end

end;
################################################################################
function WriteTableDefEpilog()

      write(outfile,"\n");
      write(outfile,"         ]\n");
      write(outfile,"      }");
end;
################################################################################
function GetInOutRole(line,InOutRole)

      if startswith(line,"/*~ Command Parameters ~*/")
         InOutRole = "CMD_PRM";
         line = readline(infile);
      elseif startswith(line,"/*~ Table Parameters ~*/")
         InOutRole = "TBL_PRM";
         line = readline(infile);
      elseif startswith(line,"/*~ Inputs ~*/")
         InOutRole = "INPUT";
         line = readline(infile);
      elseif startswith(line,"/*~ Outputs ~*/");
         InOutRole = "OUTPUT";
         line = readline(infile);
      elseif startswith(line,"/*~ Diagnostics ~*/");
         InOutRole = "DIAGNOSTIC";
         line = readline(infile);
      elseif startswith(line,"/*~ Internal Variables ~*/");
         InOutRole = "INTERNAL";
         line = readline(infile);
      elseif startswith(line,"/*~ Structures ~*/");
         InOutRole = "STRUCT";
         line = readline(infile);
      end
      return(InOutRole,line);
end;
################################################################################
function GetComment(line)

      Description = "";
      DynSize = "";
      IpcRole = "";
      Units = "";
      Abbrev = "";
      
      if occursin("/*",line)
         Start = findfirst("/*",line)[1];
         End = findlast("*/",line)[end];
         Comment = line[Start+2:End-2];
         line = line[1:Start-1];
      
         if occursin("[*",Comment)
            Start = findfirst("[*",Comment)[1];
            End = findlast("*]",Comment)[end];
            DynSize = Comment[Start+2:End-2];
            Comment = Comment[1:Start-1]*Comment[End+1:end];
         end
         
         if occursin("[[",Comment)
            Start = findfirst("[[",Comment)[1];
            End = findlast("]]",Comment)[end];
            Units = Comment[Start+2:End-2];
            Comment = Comment[1:Start-1]*Comment[End+1:end];
         end

         if occursin("[>",Comment)
            Start = findfirst("[>",Comment)[1];
            End = findlast("<]",Comment)[end];
            Abbrev = strip(Comment[Start+2:End-2]);
            Comment = Comment[1:Start-1]*Comment[End+1:end];
         end

         if occursin("[~",Comment)
            Start = findfirst("[~",Comment)[1];
            End = findlast("~]",Comment)[end];
            RoleString = Comment[Start+2:End-2];
            if occursin("=",RoleString) || 
               (occursin("<",RoleString) && occursin(">",RoleString))
               IpcRole = "TxRx";
            elseif occursin(">",RoleString)
               IpcRole = "Tx";
            elseif occursin("<",RoleString)
               IpcRole = "Rx";
            end
            Comment = Comment[1:Start-1]*Comment[End+1:end];            
         end
         
         Description = strip(Comment);
      end

      return(Description,Units,DynSize,IpcRole,Abbrev,line);
end;
################################################################################
function GetSize(Name,DynSize)

      if startswith(Name,"**")
         Name = Name[3:end];
         Size = DynSize;
      elseif startswith(Name,"*")
         Name = Name[2:end];
         Size = DynSize;
      else
         SplitName = split(Name,[']','[']);
         Name = SplitName[1];
         if (length(SplitName) == 1)
            Size = "1";
         else
            Size = SplitName[2];
         end
         if (length(SplitName) > 3)
            Size = Size*","*SplitName[4];
         end
      end
      
      return(Name,Size);
end;
################################################################################
function GetVariable(line,DynSize)

      line = strip(line);
      SplitLine = split(line);
      
      if startswith(line,"long")
         Type = "long";
         Name = SplitLine[2];
      elseif startswith(line,"double")
         Type = "double";
         Name = SplitLine[2];
      elseif startswith(line,"char")
         Type = "char";
         Name = SplitLine[2];
      elseif startswith(line,"float")
         Type = "float";
         Name = SplitLine[2];
      elseif startswith(line,"unsigned char")
         Type = "unsigned char";
         Name = SplitLine[3];
      elseif startswith(line,"unsigned int")
         Type = "unsigned int";
         Name = SplitLine[3];
      elseif startswith(line,"FILE")
         Type = "FILE";
         Name = SplitLine[2];
      elseif startswith(line,"struct")
         Type = SplitLine[2];
         Name = SplitLine[3];
      else
         Name = "";
         Type = "";
         Size = "0";
      end
      
      # Strip trailing semicolon
      Name = Name[1:end-1];

      (Name,Size) = GetSize(Name,DynSize);
      
      
      return(Name,Type,Size);
end;
################################################################################
function HeaderToJson(InfileName,IsFirstFile)

      global infile = open(InfileName);
      
      line = "";
      if IsFirstFile
         IsFirstStruct = true;
      else
         IsFirstStruct = false;
      end
      while !eof(infile) && !startswith(line,"/*~ Prototypes ~*/")
         line = readline(infile);
         if startswith(line,"struct")
            StructType = split(line)[2];
            WriteTableDefProlog(StructType,IsFirstStruct);
            IsFirstStruct = false;
            IsFirstEntry = true;
            InOutRole = "INTERNAL";
            IpcRole = "";
            Units = "";
            Abbrev = "";
            while !startswith(line,"}")
               line = strip(readline(infile));
               if length(line) > 0
                  (InOutRole,line) = GetInOutRole(line,InOutRole);
                  (Description,Units,DynSize,IpcRole,Abbrev,line) = GetComment(line);
                  (Name,Type,Size) = GetVariable(line,DynSize);
                  if (InOutRole != "INTERNAL" && Name != "")
                     WriteTableEntry(Name,Type,Size,InOutRole,IpcRole,Units,
                        Abbrev,Description,IsFirstEntry);
                     IsFirstEntry = false;
                  end
               end
            end
            WriteTableDefEpilog();
         end
      end
      
      close(infile);
end;
################################################################################
function main()

      global outfile = open("./MetaCode/42.json","w");
      write(outfile,"{\n   \"Table Definition\": [\n");
      
      HeaderToJson("./Include/42types.h",true);
      HeaderToJson("./Kit/Include/orbkit.h",false);
      
      write(outfile,"\n   ]\n}\n");
      close(outfile);

      outfile = open("./MetaCode/Ac.json","w");
      write(outfile,"{\n   \"Table Definition\": [\n");
      
      HeaderToJson("./Include/AcTypes.h",true);
      
      write(outfile,"\n   ]\n}\n");
      close(outfile);

end;
################################################################################
# Run this file if called from command line, like "julia HeadersToJson.jl"
if abspath(PROGRAM_FILE) == @__FILE__
   main()
end

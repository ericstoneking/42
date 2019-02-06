import sys
import os
import re # Regular Expressions
import json

########################################################################
def WriteProlog(outfile):

      outfile.write("/**********************************************************************/\n")
      outfile.write("long InterpretAcPrmCmds(char CmdLine[512],double *CmdTime)\n")
      outfile.write("{\n\n")
      outfile.write("      long Idx1,Idx2;\n")
      outfile.write("      long LongVal[24];\n")
      outfile.write("      double DblVal[24];\n")
      outfile.write("      long NewCmdProcessed = FALSE;\n\n\n")
      
########################################################################
def WriteEpilog(outfile):

      outfile.write("      return(NewCmdProcessed);\n}\n")
########################################################################
def WriteCodeBlock(outfile,FmtPrefix,ArgPrefix,VarString,IdxLen,Ni,Nj,StructIdxString,Nidx,FormatString):

      if FormatString == "%ld":
         ValVar = "LongVal"
      else:
         ValVar = "DblVal"
      #endif

# ... Construct the format string
      line = "      if (sscanf(CmdLine,\"%lf "
      line += FmtPrefix
      line += VarString
      line += " = ["
      for i in range (0,Ni):
         for j in range (0,Nj):
            line += " "+FormatString
         #next j
      #next i
      line += "]\",\n"
      
# ... Construct the argument list
      line += "         &CmdTime,"+StructIdxString+"\n"
      for i in range (0,Ni*Nj):
         line += "         &"+ValVar+"["+str(i)+"],\n"
      # next i
      line = line[:-2] # Remove trailing comma
      line += ") == "+str(Ni*Nj+Nidx+1)+") {\n"
      line += "         NewCmdProcessed = TRUE;\n"

# ... Make assignment statements
      if IdxLen == 2:
         for i in range (0,Ni):
            for j in range (0,Nj):
               line += "         "+ArgPrefix+VarString+"["+str(i)+"]["+str(j)+"] = "+ValVar+"["+str(Nj*i+j)+"];\n"
            #next j
         #next i
      elif IdxLen == 1:
         for i in range (0,Ni):
            line += "         "+ArgPrefix+VarString+"["+str(i)+"] = "+ValVar+"["+str(i)+"];\n"
         #next i
      else:
         line += "         "+ArgPrefix+VarString+" = "+ValVar+"[0];\n"
      #endif      
      
      line += "      }\n\n"

      outfile.write(line)
########################################################################
def ParseStruct(StructList,Struct,FmtPrefix,ArgPrefix,StructIdxString,Nidx,outfile):

      VarList = Struct["Table Data"]
      for Var in VarList:
         DataType = Var["Data Type"]
         if DataType in ["long","double"]:
            VarString = Var["Variable Name"]
            if "Array Size" in Var:
               IdxString = Var["Array Size"].strip(r"[]")
               IdxList =  IdxString.split(",")
               IdxLen = len(IdxList)
               if IdxLen == 2:
                  Ni = int(IdxList[0])
                  Nj = int(IdxList[1])
               else:
                  Ni = int(IdxList[0])
                  Nj = 1
               #endif
            else:
               IdxLen = 0
               Ni = 1
               Nj = 1
            #endif
            if DataType == "long":
               FormatString = "%ld"
            else:
               FormatString = "%lf"
            #endif
            WriteCodeBlock(outfile,FmtPrefix,ArgPrefix,VarString,IdxLen,Ni,Nj,StructIdxString,Nidx,FormatString)
         else: # struct
            for SubStruct in StructList:
               if SubStruct["Table Name"] == Var["Data Type"]:
                  LocalFmtPrefix = FmtPrefix + Var["Variable Name"]
                  LocalArgPrefix = ArgPrefix + Var["Variable Name"]
                  if "Array Size" in Var:
                     IdxString = Var["Array Size"].strip(r"[]")
                     IdxList =  IdxString.split(",")
                     IdxLen = len(IdxList)
                     if IdxLen == 2:
                        LocalFmtPrefix += "[%ld][%ld]."
                        LocalArgPrefix += "[Idx1][Idx2]."
                        StructIdxString += "&Idx1,&Idx2,"
                        Nidx += 2
                        ParseStruct(StructList,SubStruct,LocalFmtPrefix,LocalArgPrefix,StructIdxString,Nidx,outfile)
                     else:
                        LocalFmtPrefix += "[%ld]."
                        LocalArgPrefix += "[Idx1]."
                        StructIdxString += "&Idx1,"
                        Nidx += 1
                        ParseStruct(StructList,SubStruct,LocalFmtPrefix,LocalArgPrefix,StructIdxString,Nidx,outfile)
                     #endif
                  else:
                     LocalFmtPrefix += "."
                     LocalArgPrefix += "."
                     ParseStruct(StructList,SubStruct,LocalFmtPrefix,LocalArgPrefix,StructIdxString,Nidx,outfile)
                  #endif
               #endif
            #next SubStruct
         #endif           
      #next Var
      
########################################################################
def main():
      
      InfileName = 'Acs.json'
      OutfileName = 'AcPrmCmdInterpreter.c'
      RootTableName = "AcsType"
      FmtPrefix = "AC."
      ArgPrefix = "Acs::AC."
      
      infile = open(InfileName,'rU')
      StructDict = json.load(infile)
      infile.close()

      StructList = StructDict["Table Definition"] 
      for Struct in StructList:
         if Struct["Table Name"] == RootTableName:
            RootStruct = Struct
         #endif
      #next Struct
      
      outfile = open(OutfileName,'w')
      WriteProlog(outfile)
      ParseStruct(StructList,RootStruct,FmtPrefix,ArgPrefix,"",0,outfile) 
      WriteEpilog(outfile)           
      outfile.close()
      
########################################################################
if __name__ == '__main__': main()

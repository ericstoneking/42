import sys
import os
import re # Regular Expressions
import json

########################################################################
def WriteProlog(outfile):

      outfile.write("/**********************************************************************/\n")
      outfile.write("void WriteAcToSocket(void)\n")
      outfile.write("{\n\n")
      outfile.write("      long i;\n")
      outfile.write("      int Success;\n\n")
      
########################################################################
def WriteEpilog(outfile):

      outfile.write("}\n")
########################################################################
def WriteCodeBlock(outfile,Indent,FmtPrefix,ArgPrefix,VarString,IdxLen,Ni,Nj,StructIdxString,Nidx,FormatString):



      line = "      "+Indent+"sprintf(line,\""
      line += FmtPrefix
      line += VarString
      line += " = ["
      for i in range (0,Ni):
         for j in range (0,Nj):
            line += " "+FormatString
         #next j
      #next i
      line += "]\""+StructIdxString
      if Nj > 1:
         for i in range (0,Ni):
            for j in range (0,Nj):
               line += ","+ArgPrefix+VarString+"["+str(i)+"]["+str(j)+"]"
            #next j
         #next i
      elif Ni > 1:
         for i in range (0,Ni):
            line += ","+ArgPrefix+VarString+"["+str(i)+"]"
         #next i
      else:
         line += ","+ArgPrefix+VarString
      #endif
      
      line += ");\n"

      outfile.write(line)
      outfile.write("      "+Indent+"Success = send(TxSocket,line,strlen(line),0);\n")
      outfile.write("      "+Indent+"if (EchoEnabled) printf(\"%s\",line);\n\n")

########################################################################
def ParseStruct(StructList,Struct,Indent,FmtPrefix,ArgPrefix,StructIdxString,Nidx,outfile):

      VarList = Struct["Table Data"]
      for Var in VarList:
         DataType = Var["Data Type"]
         if DataType in ["long","double"]:
            SimReadWrite = Var["Sim Read/Write"]
            if SimReadWrite in ["WRITE","READ_WRITE"]:
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
               WriteCodeBlock(outfile,Indent,FmtPrefix,ArgPrefix,VarString,IdxLen,Ni,Nj,StructIdxString,Nidx,FormatString)
            #endif
         else: # struct
            for SubStruct in StructList:
               if SubStruct["Table Name"] == Var["Data Type"]:
                  LocalFmtPrefix = FmtPrefix + Var["Variable Name"]
                  LocalArgPrefix = ArgPrefix + Var["Variable Name"]
                  if "Array Size" in Var:
                     IdxString = Var["Array Size"].strip(r"[]")
                     IdxList =  IdxString.split(",")
                     IdxLen = len(IdxList)
                     if IdxString.isalpha():
                        outfile.write("      for(i=0;i<"+FmtPrefix+IdxString+";i++) {\n")
                        Indent = "   "
                        LocalFmtPrefix += "[%ld]."
                        LocalArgPrefix += "[i]."
                        StructIdxString = ",i"
                        Nidx += 1
                        ParseStruct(StructList,SubStruct,Indent,LocalFmtPrefix,LocalArgPrefix,StructIdxString,Nidx,outfile)
                        outfile.write("      }\n\n")
                        Indent = ""
                     elif IdxLen == 2:
                        LocalFmtPrefix += "[%ld][%ld]."
                        LocalArgPrefix += "[i][j]."
                        StructIdxString = ",i,j"
                        Nidx += 2
                        Indent = ""
                        ParseStruct(StructList,SubStruct,Indent,LocalFmtPrefix,LocalArgPrefix,StructIdxString,Nidx,outfile)
                     else:
                        LocalFmtPrefix += "[%ld]."
                        LocalArgPrefix += "[i]."
                        StructIdxString = ",i"
                        Nidx += 1
                        Indent = ""
                        ParseStruct(StructList,SubStruct,Indent,LocalFmtPrefix,LocalArgPrefix,StructIdxString,Nidx,outfile)
                     #endif
                  else:
                     LocalFmtPrefix += "."
                     LocalArgPrefix += "."
                     StructIdxString = ""
                     Indent = ""
                     ParseStruct(StructList,SubStruct,Indent,LocalFmtPrefix,LocalArgPrefix,StructIdxString,Nidx,outfile)
                  #endif
               #endif
            #next SubStruct
         #endif           
      #next Var
      
########################################################################
def main():
      
      InfileName = 'AC.json'
      OutfileName = 'WriteAcToSocket.c'
      RootTableName = "AcType"
      FmtPrefix = "AC."
      ArgPrefix = "AC."
      
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
      ParseStruct(StructList,RootStruct,"",FmtPrefix,ArgPrefix,"",0,outfile) 
      WriteEpilog(outfile)           
      outfile.close()
      
########################################################################
if __name__ == '__main__': main()

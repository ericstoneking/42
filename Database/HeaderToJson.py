import sys
import os
import re # Regular Expressions

########################################################################
def GetSize(Name,DynSize):

      if Name.startswith('*'):
         Name = Name[1:]
         Size = DynSize
      else:
         # Look for 2-D arrays
         match = re.search(r'\[(\d+)\]\[(\d+)\]',Name)
         if match:
            Size = str(match.group(1))+','+str(match.group(2))
            NameList = Name.split('[')
            Name = NameList[0]
         else:
            # Look for 1-D arrays
            match = re.search(r'\[(\d+)\]',Name)
            if match:
               Size = str(match.group(1))
               NameList = Name.split('[')
               Name = NameList[0]
            else:
               Size = 1
            #endif
         #endif
      #endif

      return(Name,Size)
########################################################################
def GetVariable(line,DynSize):

      if line.startswith('long'):
         Type = 'long'
         SplitLine = line.split()
         Name = SplitLine[1]
         (Name,Size) = GetSize(Name,DynSize)
      elif line.startswith('double'):
         Type = 'double'
         SplitLine = line.split()
         Name = SplitLine[1]
         (Name,Size) = GetSize(Name,DynSize)
      elif line.startswith('struct'):
         SplitLine = line.split()
         Type = SplitLine[1]
         Name = SplitLine[2]
         (Name,Size) = GetSize(Name,DynSize)
      else:
         Name = ''
         Type = ''
         Size = '0'
      #endif
      
      return(Name,Type,Size)
########################################################################
def GetComment(Line):

      CommentStart = Line.find('/*')
      if CommentStart != -1:
         Comment = Line[CommentStart:]
         Line = Line[0:CommentStart]

         UnitStart = Comment.find('[[')
         UnitEnd = Comment.find(']]')
         if UnitStart > 0 and UnitEnd > 0:
            Units = Comment[UnitStart+2:UnitEnd]
            Comment = Comment[:UnitStart-1]+Comment[UnitEnd+2:]
         else:
            Units = 'None'
         #endif
         
         SizeStart = Comment.find('[*')
         SizeEnd = Comment.find('*]')
         if SizeStart > 0 and SizeEnd > 0:
            DynSize = Comment[SizeStart+2:SizeEnd]
            Comment = Comment[:SizeStart-1]+Comment[SizeEnd+2:]
         else:
            DynSize = ''
         #endif

         Comment = Comment.replace('/*',' ')
         Comment = Comment.replace('>~*/',' ')
         Comment = Comment.replace('<~*/',' ')
         Comment = Comment.replace('=~*/',' ')
         Comment = Comment.replace('*/',' ')
         Comment = Comment.strip()
      else:
         Comment = ''
         Units = ''
         DynSize = ''
      #endif
      
      Line = Line.strip()
      # Strip trailing semicolon
      Line = Line[0:-1]

      return(Comment,Units,DynSize,Line)
########################################################################
def WriteTableDefProlog(outfile,StructType,IsFirstStruct):

      if IsFirstStruct:
         outfile.write('      {\n')
      else:
         outfile.write(',\n      {\n')
      #endif
      
      outfile.write('         \"Table Name\": \"'+StructType+'\",\n')
      outfile.write('         \"Table Type\": \"Structure\",\n')
      outfile.write('         \"Table Data\": [\n')

########################################################################
def AppendFile(outfile,FileName):

      appendfile = open(FileName,'r')
      for line in appendfile:
         outfile.write(line)
      #next line
      appendfile.close()

########################################################################
def main():

      if len(sys.argv) != 3:
         print "Usage: python HeaderToJson.py infile outfile\n"
         exit(0)
      else:
         InfileName = '../Include/'+sys.argv[1]
         OutfileName = sys.argv[2]
      #endif

      #InfileName = '../Include/fswtypes.h'
      #OutfileName = 'AC.json'
      
      outfile = open(OutfileName,'w')
      
      infile = open(InfileName,'rU')
      outfile.write('{\n')
      outfile.write('   \"Table Definition\": [\n')

      IsFirstStruct = 1
      
      Role = ''

      line = infile.readline()
      while (line != ''): # EOF
         if line.startswith('struct'): # Found 'struct'
            IsFirstEntry = 1;
            LineList = line.split()
            StructType = LineList[1]
            WriteTableDefProlog(outfile,StructType,IsFirstStruct)
            IsFirstStruct = 0
            
            line = infile.readline()
            while not line.startswith('};'):
               StrippedLine = line.strip()
               if StrippedLine.startswith('/*~ Parameters ~*/'):
                  Role = 'PRM'
                  StrippedLine = ''
               elif StrippedLine.startswith('/*~ Inputs ~*/'):
                  Role = 'IN'
                  StrippedLine = ''
               elif StrippedLine.startswith('/*~ Outputs ~*/'):
                  Role = 'OUT'
                  StrippedLine = ''
               elif StrippedLine.startswith('/*~ Diagnostics ~*/'):
                  Role = 'DIAG'
                  StrippedLine = ''
               elif StrippedLine.startswith('/*~ Internal Variables ~*/'):
                  Role = ''
                  StrippedLine = ''
               elif StrippedLine.startswith('/*~ Structures ~*/'):
                  Role = 'STRUCT'
                  StrippedLine = ''
               #endif
               
               if StrippedLine.endswith('>~*/'):
                  SimMsgDir = 'WRITE'
                  AppMsgDir = 'READ'
               elif StrippedLine.endswith('<~*/'):
                  SimMsgDir = 'READ'
                  AppMsgDir = 'WRITE'
               elif StrippedLine.endswith('=~*/'):
                  SimMsgDir = 'READ_WRITE'
                  AppMsgDir = 'READ_WRITE'
               else:
                  SimMsgDir = ''
                  AppMsgDir = ''
               #endif
               
               if StrippedLine != '' and (Role != '' or SimMsgDir != '' or AppMsgDir != ''):
                  # Parse Comment first
                  (Comment,Units,DynSize,StrippedLine) = GetComment(StrippedLine)
                  # Parse the rest
                  (Name,Type,Size) = GetVariable(StrippedLine,DynSize)
                  
                  if Name != '':
                     if IsFirstEntry:
                        outfile.write('            {\n')
                     else:
                        outfile.write(',\n            {\n')
                     #endif
                     IsFirstEntry = 0
                     outfile.write('               \"Variable Name\":  \"'+Name+'\",\n')
                     outfile.write('               \"Description\":  \"'+Comment+'\",\n')
                     outfile.write('               \"Units\":  \"'+Units+'\",\n')
                     if (Size != 1):
                        outfile.write('               \"Data Type\":  \"'+Type+'\",\n')
                        outfile.write('               \"Array Size\":  \"'+Size+'\",\n')
                     else:
                        outfile.write('               \"Data Type\":  \"'+Type+'\",\n')
                     #endif
                     outfile.write('               \"Sim Read/Write\":   \"'+SimMsgDir+'\",\n')
                     outfile.write('               \"App Read/Write\":   \"'+AppMsgDir+'\",\n')
                     outfile.write('               \"Packet Role\":   \"'+Role+'\"\n')
                  
                     outfile.write('            }')
                  #endif
               #endif   
               line = infile.readline()
            #end while
            outfile.write('\n')
            outfile.write('         ]\n')
            outfile.write('      }')
         #endif
            
         line = infile.readline()
         
      #end while

      #AppendFile(outfile,'TableTypeDef.json')
      #AppendFile(outfile,'DataTypeDef.json')
      outfile.write('\n   ]\n')
      outfile.write('}\n')
      
      outfile.close()   
      infile.close()
########################################################################
if __name__ == '__main__': main()



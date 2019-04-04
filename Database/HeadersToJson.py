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
      elif line.startswith('char'):
         Type = 'char'
         SplitLine = line.split()
         Name = SplitLine[1]
         (Name,Size) = GetSize(Name,DynSize)
      elif line.startswith('float'):
         Type = 'float'
         SplitLine = line.split()
         Name = SplitLine[1]
         (Name,Size) = GetSize(Name,DynSize)
      elif line.startswith('unsigned char'):
         Type = 'unsigned char'
         SplitLine = line.split()
         Name = SplitLine[2]
         (Name,Size) = GetSize(Name,DynSize)
      elif line.startswith('unsigned int'):
         Type = 'unsigned int'
         SplitLine = line.split()
         Name = SplitLine[2]
         (Name,Size) = GetSize(Name,DynSize)
      elif line.startswith('FILE'):
         Type = 'FILE'
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
         
         ReadWriteStart = Comment.find('[~')
         ReadWriteEnd = Comment.find('~]')
         if ReadWriteStart > 0 and ReadWriteEnd > 0:
            ReadWrite = Comment[ReadWriteStart+2:ReadWriteEnd]
            Comment = Comment[:ReadWriteStart-1]+Comment[ReadWriteEnd+2:]
         else:
            ReadWrite = ''
         #endif

         Comment = Comment.replace('/*',' ')
         Comment = Comment.replace('*/',' ')
         Comment = Comment.strip()
      else:
         Comment = ''
         Units = ''
         DynSize = ''
         ReadWrite = ''
      #endif
      
      Line = Line.strip()
      # Strip trailing semicolon
      Line = Line[0:-1]

      return(Comment,Units,DynSize,ReadWrite,Line)
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


      OutfileName = '42.json'
      InfileList = {'../Include/42types.h','../Include/AcTypes.h','../Kit/Include/orbkit.h'}
      
      outfile = open(OutfileName,'w')
      outfile.write('{\n')
      outfile.write('   \"Table Definition\": [\n')
      
      
      IsFirstStruct = 1
      
      Role = ''

      for InfileName in InfileList:
         infile = open(InfileName,'rU')

         line = infile.readline()
         if line.startswith('/*~ Prototypes ~*/'):
            line = ''
         #endif
         
         while (line != ''): # EOF or reached Prototypes
            if line.startswith('struct'): # Found 'struct'
               IsFirstEntry = 1;
               LineList = line.split()
               StructType = LineList[1]
               WriteTableDefProlog(outfile,StructType,IsFirstStruct)
               IsFirstStruct = 0
            
               line = infile.readline()
               if line.startswith('/*~ Prototypes ~*/'):
                  line = ''
               #endif
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
               
                  if StrippedLine != '':
                     # Parse Comment first
                     (Comment,Units,DynSize,ReadWrite,StrippedLine) = GetComment(StrippedLine)
                     # Parse the rest
                     (Name,Type,Size) = GetVariable(StrippedLine,DynSize)
                     
                     SimMsgDir = ''
                     AppMsgDir = ''
                     CmdMsgDir = ''
                     if '>' in ReadWrite:
                        SimMsgDir = 'WRITE'
                        AppMsgDir = 'READ'                  
                     #endif
                     if '<' in ReadWrite:
                        SimMsgDir = 'READ'
                        AppMsgDir = 'WRITE'
                     #endif
                     if '=' in ReadWrite:
                        SimMsgDir = 'READ_WRITE'
                        AppMsgDir = 'READ_WRITE'
                     #endif
                     if '!' in ReadWrite:
                        CmdMsgDir = 'READ'
                     #endif
                  
                     if Name != '' and (SimMsgDir != '' or AppMsgDir != '' or CmdMsgDir != '' or Role != ''):
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
                        outfile.write('               \"Cmd Read\":   \"'+CmdMsgDir+'\",\n')
                        outfile.write('               \"Packet Role\":   \"'+Role+'\"\n')
                  
                        outfile.write('            }')
                     #endif
                  #endif   
                  line = infile.readline()
                  if line.startswith('/*~ Prototypes ~*/'):
                     line = ''
                  #endif
               #end while
               outfile.write('\n')
               outfile.write('         ]\n')
               outfile.write('      }')
            #endif
            
            line = infile.readline()
            if line.startswith('/*~ Prototypes ~*/'): # Reached end of variable definitions
               line = '' 
            #endif
         
         #end while
         infile.close()
      #next InfileName

      outfile.write('\n   ]\n')
      outfile.write('}\n')
      
      outfile.close()   
########################################################################
if __name__ == '__main__': main()



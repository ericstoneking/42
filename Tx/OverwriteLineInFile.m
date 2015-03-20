function OverwriteLineInFile(Filename,LineNum,Text)


      infile = fopen(Filename);
      outfile = fopen('MatlabTemp.txt','w');

% ... Copy infile to outfile, up to line to be replaced
      for i=1:(LineNum-1),
         line = fgets(infile,512);
         %fputs(outfile,line);
         fprintf(outfile,'%s',line);
      end

% ... Read and discard line from infile
      line = fgets(infile,512);
% ... Write replacement to outfile
      fprintf(outfile,'%s',Text);

% ... Copy remainder of infile to outfile
      while(~feof(infile)),
         line = fgets(infile,512);
         %fputs(outfile,line);
         fprintf(outfile,'%s',line);
      end

      fclose(infile);
      fclose(outfile);         

% ... Overwrite old file with modified file
      line = sprintf('mv MatlabTemp.txt %s',Filename);
      system(line);


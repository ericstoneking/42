using DelimitedFiles;
using Dates;
using Plots;
gr();

################################################################################
function ReplaceLineInFile(FileName,LineNum,text)

      (tmppath,tmpio) = mktemp();
      infile = open(FileName,"r");
      for i = 1:LineNum-1
         line = readline(infile,keep=true);
         write(tmpio,line);
      end
      line = readline(infile,keep=true);
      write(tmpio,text*"\n");
      while !eof(infile)
         line = readline(infile,keep=true);
         write(tmpio,line);
      end
      close(infile);
      close(tmpio);
      mv(tmppath,FileName,force=true);
            
end;
################################################################################
# Simple Monte Carlo Campaign 
function RunMC()

# ... Initialization and Setup
      BasePath = "./Baseline";
      Nrun = 20;
      MaxCore = 12;
      
      # Allocate output variables
      SettleTime = zeros(Nrun);

# ... Campaign Loop
      Ndigits = Int(floor(log10(Nrun)))+1;
      Nbatch = Int(ceil(Nrun/MaxCore));
      StartTime = now();
      # Make sure graphics front end is FALSE
      s = "FALSE                           !  Graphics Front End?";
      ReplaceLineInFile(BasePath*"/Inp_Sim.txt",7,s);
            
      # Prepare batch inputs
      for Ibatch = 1:Nbatch
         if Nrun-(Ibatch-1)*MaxCore < MaxCore
            Ncore = Nrun-(Ibatch-1)*MaxCore
         else
            Ncore = MaxCore
         end
         BatchPath = "";
         RunPath = Vector{String}(undef,Ncore);
         RunProc = Vector{Any}(undef,Ncore);
         for Icore = 1:Ncore
            Irun = Int((Ibatch-1)*MaxCore+Icore);
            
            # Copy Base folder to Run folder
            RunPath[Icore] = "Run"*lpad(Irun,Ndigits,"0");
            cp(BasePath,RunPath[Icore],force=true);
            BatchPath = BatchPath*" "*RunPath[Icore];
            
            # Modify input files as desired
            EulAng = 20*(2*rand(3) .- 1);
            EulAng = round.(EulAng,digits=2);
            s = "$(EulAng[1]) $(EulAng[2]) $(EulAng[3])    213      ! Angles (deg) & Euler Sequence";
            ReplaceLineInFile(RunPath[Icore]*"/SC_Simple.txt",16,s);
            
         end

         # Perform runs
         FirstRunOfBatch = (Ibatch-1)*MaxCore+1;
         LastRunOfBatch = (Ibatch-1)*MaxCore+Ncore;
         println("Starting Runs $(FirstRunOfBatch) - $(LastRunOfBatch)");
         cd("..")
         t1 = now();
         #for Icore = 1:Ncore-1
         #   Path = "MonteCarlo/$(RunPath[Icore])"
         #   run(pipeline(`./42 $Path`,stdout="$Path/stdout.txt",stderr="$Path/stderr.txt"),wait=false);
         #end
         #Path = "MonteCarlo/$(RunPath[Ncore])"
         #run(pipeline(`./42 $Path`,stdout="$Path/stdout.txt",stderr="$Path/stderr.txt"),wait=true);
         for Icore = 1:Ncore
            Path = "MonteCarlo/$(RunPath[Icore])"
            RunProc[Icore] = run(pipeline(`./42 $Path`,stdout="$Path/stdout.txt",stderr="$Path/stderr.txt"),wait=false);
         end
         for Icore = 1:Ncore
            wait(RunProc[Icore]);
         end
         t2 = now();
         dt = Dates.value(t2-t1)/1000.0;
         println("All runs complete in $dt sec");
         cd("MonteCarlo");
      
         # Process batch results
         for Icore = 1:Ncore
            Irun = (Ibatch-1)*MaxCore + Icore;
            cd(RunPath[Icore]);
            time = readdlm("time.42");
            wbn = readdlm("wbn.42");

            N = length(time);
            for k=1:N-1,
               magwbn = sqrt(wbn[k,1]^2+wbn[k,2]^2+wbn[k,3]^2);
               if (magwbn > 1.0E-4*180/pi)
                  SettleTime[Irun] = time[k+1];
               end
            end
            
            # Save only selected .42 files
            SaveList = ["time","wbn"];
            dir = readdir();
            for i = 1:length(dir)
               if occursin(".42",dir[i]) && !any(occursin.(SaveList,dir[i]))
                  rm(dir[i]);
               end
            end
            
            # Delete unaltered *.txt files
            rm("Flex_Simple.txt");
            rm("Inp_Cmd.txt");
            rm("Inp_FOV.txt");
            rm("Inp_Graphics.txt");
            rm("Inp_IPC.txt");
            rm("Inp_NOS3.txt");
            rm("Inp_Region.txt");
            rm("Inp_Sim.txt");
            rm("Inp_TDRS.txt");
            rm("Nodes_Simple.txt");
            rm("Orb_LEO.txt");
            #rm("SC_Simple.txt");
            rm("Shaker_Simple.txt");
            rm("TRV.txt");
            rm("Whl_Simple.txt");
            
            cd("..")
         end
         
      end
            
# ... Post-processing
      open("SettleTime.txt","w") do io
         writedlm(io,SettleTime,' ');
      end
      
      EndTime = now();
      dt = Dates.value(EndTime-StartTime)/1000.0;
      println("Campaign Duration: $dt sec");

end
################################################################################
if abspath(PROGRAM_FILE) == @__FILE__
      RunMC();
end;

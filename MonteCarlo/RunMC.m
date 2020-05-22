% Simple Monte Carlo Campaign

% ... Initialization and Setup
      addpath('../Utilities');
      more off  % So octave won't buffer screen output
      %rand('seed',1);  % For reproducible runs, set seed
      BasePath = './Baseline';
      Nrun = 20;
      MaxCore = 6;

      Nbatch = ceil(Nrun/MaxCore);

      % Allocate output variables
      SettleTime = zeros(Nrun,1);

% ... Campaign Loop
      for Ibatch = 1:Nbatch,
         Ncore = min(MaxCore,Nrun-(Ibatch-1)*MaxCore);
         BatchPath = '';
         for Icore = 1:Ncore,
            Irun = (Ibatch-1)*MaxCore + Icore;

            % Copy Base folder to Run folder
            RunPath = sprintf('Run%03d',Irun);
            system(['rm -rf ',RunPath]);
            system(['cp -r ',BasePath,' ',RunPath]);
            BatchPath = [BatchPath,' ',RunPath];
            
            % Make sure graphics front end is FALSE.
            s = sprintf('FALSE                           !  Graphics Front End?\n');
            OverwriteLineInFile([RunPath,'/Inp_Sim.txt'],6,s);

            % Modify Input Files as desired
            EulAng = 20*(2*rand(3,1)-1);
            s = sprintf(...
               '%8.2f  %8.2f   %8.2f    213      ! Angles (deg) & Euler Sequence\n',...
               EulAng(1),EulAng(2),EulAng(3));
            OverwriteLineInFile([RunPath,'/SC_Simple.txt'],16,s);

         end

         % Perform Runs
         fprintf('Starting Runs %d - %d\n',(Ibatch-1)*MaxCore + 1, (Ibatch-1)*MaxCore+Ncore);
         system(['../Utilities/BatchRun.sh ',BatchPath]);

         % Process Batch Results
         for Icore = 1:Ncore,
            Irun = (Ibatch-1)*MaxCore + Icore;
            RunPath = sprintf('Run%03d',Irun);

            % Process Run in Matlab/Octave
            LoadString = [RunPath,'/time.42'];
            load(LoadString);
            LoadString = [RunPath,'/wbn.42'];
            load(LoadString);

            N = length(time);
            for k=1:N-1,
               magwbn = norm(wbn(k,:));
               if (magwbn > 1.0E-6*180/pi),
                  SettleTime(Irun) = time(k+1);
               end
            end

            % Save only selected *.42 files
            SaveList = {'time','wbn'};
            Nsave = length(SaveList);
            for Isave = 1:Nsave,
               system(['mv ',RunPath,'/',SaveList{Isave},'.42 ',RunPath,'/',SaveList{Isave},'.tmp']);
            end
            system(['rm ',RunPath,'/*.42']);
            for Isave = 1:Nsave,
               system(['mv ',RunPath,'/',SaveList{Isave},'.tmp ',RunPath,'/',SaveList{Isave},'.42']);
            end

            % Delete unaltered *.txt files
            system(['rm ',RunPath,'/Flex_Simple.txt']);
            system(['rm ',RunPath,'/Inp_Cmd.txt']);
            system(['rm ',RunPath,'/Inp_FOV.txt']);
            system(['rm ',RunPath,'/Inp_Graphics.txt']);
            system(['rm ',RunPath,'/Inp_IPC.txt']);
            system(['rm ',RunPath,'/Inp_NOS3.txt']);
            system(['rm ',RunPath,'/Inp_Region.txt']);
            %system(['rm ',RunPath,'/Inp_Sim.txt']);
            system(['rm ',RunPath,'/Inp_TDRS.txt']);
            system(['rm ',RunPath,'/Orb_LEO.txt']);
            %system(['rm ',RunPath,'/SC_Simple.txt']);

         end

      end

% ... Post-processing
      figure(1)
      hist(SettleTime)
      title('Distribution of Settle Times')
      xlabel('Settle Time, sec')
      ylabel('Occurrences')
      save -ascii SettleTime.txt SettleTime;

% ... Cleanup
      more on
      rmpath('../Utilities');

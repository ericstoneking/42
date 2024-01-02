% ... Read Nastran f06 file and generate 42-style Flex input files

% ... f06 input file name
      F06file = 'JDEM_ITT_Free_Free_cls_jitter_FINAL_stiffenedSC_trade8_deplSAs_b.f06';

% ... List of modes desired
      Mode = [7:12]';

% ... List of nodes desired
      NodeID = [500201 500202 500203 500204 500301 500302 500303 500304 2050991 2050992 2050993 2050994]';

% ... List of nodes corresponding to joints
      JointID = [500001 500101]';
      JointTag = [0 1];  % Global joint tags in 42's geometry model

% ... Name of file containing node locations
      NodeDefFile = 'FlexNodeLocations.txt';

% ... SC file to write outputs to
      SCfile = 'SC_WFIRST.txt';
% ... Body number [0,1,...]
      Ibody = 0;

% ... Flex file to write outputs to
      FlexFile = 'Flex_WFIRST_Body0.txt';

% ... Assumed modal damping ratio
      DampingRatio = 0.01;

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% ... Setup
      Nmode = max(size(Mode));
      Nnode = max(size(NodeID));
      Njoint = max(size(JointID));

      NodePosB = zeros(Nnode,3);
      JointPosB = zeros(Njoint,3);

      Mf = zeros(Nmode,1);
      Kf = zeros(Nmode,1);

      PSIn = zeros(Nmode,Nnode,3);
      THETAn = zeros(Nmode,Nnode,3);
      PSIj = zeros(Nmode,Njoint,3);
      THETAj = zeros(Nmode,Njoint,3);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Read Node Location File
%

% ... Find Node Locations
      for Inode=1:Nnode,
         infile=fopen(NodeDefFile);
         while ~feof(infile),
            line = fgets(infile,512);
            [ID1,ID2,ID3,x,y,z,count] = sscanf(line,'%i %i %i %f %f %f','C');
            if (ID1 == NodeID(Inode)),
               NodePosB(Inode,:) = [x y z];
            end
         end
         fclose(infile);
      end

% ... Find Joint Locations
      for Ijoint = 1:Njoint,
         infile=fopen(NodeDefFile);
         while ~feof(infile),
            line = fgets(infile,512);
            [ID1,ID2,ID3,x,y,z,count] = sscanf(line,'%i %i %i %f %f %f','C');
            if (ID1 == JointID(Ijoint)),
               JointPosB(Ijoint,:) = [x y z];
            end
         end
         fclose(infile);
      end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%   Read f06 file
%

      infile = fopen(F06file);

% ... Find mass matrix
      Foundline = 0;
      while(~Foundline),
         line = fgets(infile,512);
         %[massstr,axisstr,sysstr,count]=sscanf(line,' %s %s %s ','C');
         [Keywords,WordCount] = sscanf(line,' %s %s %s ',[3 1]);
         massstr = Keywords(1);
         axisstr = Keywords(2);
         sysstr = Keywords(3);
         if(strcmp(massstr,'MASS') && strcmp(axisstr,'AXIS') && strcmp(sysstr,'SYSTEM')),
            Foundline = 1;
         end
      end
      
% ... Extract mass, cm
      line = fgets(infile,512);
      [mass,junk,ycg,zcg,count] = sscanf(line,' X %f %f %f %f','C');
      line = fgets(infile,512);
      [mass,xcg,junk,zcg,count] = sscanf(line,' Y %f %f %f %f','C');
      line = fgets(infile,512);
      [mass,xcg,ycg,junk,count] = sscanf(line,' Z %f %f %f %f','C');

% ... Extract MOI
      line = fgets(infile,512);  % I(S)
      line = fgets(infile,512);
      [I11,I12,I13,count] = sscanf(line,' * %f %f %f *','C');
      line = fgets(infile,512);
      [I21,I22,I23,count] = sscanf(line,' * %f %f %f *','C');
      line = fgets(infile,512);
      [I31,I32,I33,count] = sscanf(line,' * %f %f %f *','C');
      Ixx = I11;
      Iyy = I22;
      Izz = I33;
      Ixy = -I12;
      Ixz = -I13;
      Iyz = -I23;

% ... Find generalized mass, stiffness
      Foundline = 0;
      while(~Foundline),
         line = fgets(infile,512);
         [Rstr,Estr,Astr,Lstr,count]=sscanf(line,' %s %s %s %s ','C');
         if(strcmp(Rstr,'R') && strcmp(Estr,'E') && strcmp(Astr,'A') && strcmp(Lstr,'L')),
            line = fgets(infile,512);
            [Afterstr,Augstr,count] = sscanf(line,' %s %s ','C');
            if(strcmp(Afterstr,'(AFTER') && strcmp(Augstr,'AUGMENTATION')),
               Foundline = 1;
            end
         end
      end
      line = fgets(infile,512); % Heading line 1
      line = fgets(infile,512); % Heading line 2

% ... Extract mass, stiffness elements
      NumModesFound = 0;
      while(NumModesFound < Nmode),
         line = fgets(infile,512);
         [ModeNo,ExtOrder,EigVal,Radians,Cycles,GenMass,GenStiff,count] = ...
            sscanf(line,' %f %f %f %f %f %f %f','C');
         for Imode = 1:Nmode,
            if (ModeNo == Mode(Imode)),
               NumModesFound = NumModesFound + 1;
               Mf(Imode) = GenMass;
               Kf(Imode) = GenStiff;
            end
         end
      end

% ... For each mode
      for Imode = 1:Nmode,
         ModeNo = Mode(Imode);

         FoundMode = 0;
         while(~FoundMode),
            line = fgets(infile,512);
            [Cycles,EigNo,count] = ...
               sscanf(line,' CYCLES =  %f R E A L   E I G E N V E C T O R   N O . %f','C');
            if (EigNo == ModeNo),
               FoundMode = 1;
            end
         end
         line = fgets(infile,512);
         line = fgets(infile,512);

         % Extract mode shapes, PSI and THETA
         NumNodesFound = 0;
         while(NumNodesFound < Nnode + Njoint),
            line = fgets(infile,512);
            [PointID,Type,T1,T2,T3,R1,R2,R3,count] = ...
               sscanf(line,' %i %s %f %f %f %f %f %f','C');

            for Inode = 1:Nnode
               if (NodeID(Inode) == PointID),
                  NumNodesFound = NumNodesFound + 1;
                  PSIn(Imode,Inode,:) = [T1 T2 T3];
                  THETAn(Imode,Inode,:) = [R1 R2 R3];
               end
            end

            for Ijoint = 1:Njoint
               if (JointID(Ijoint) == PointID),
                  NumNodesFound = NumNodesFound + 1;
                  PSIj(Imode,Ijoint,:) = [T1 T2 T3];
                  THETAj(Imode,Ijoint,:) = [R1 R2 R3];
               end
            end

         end
      end

      fclose(infile);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% ... Write to SC file


      infile = fopen(SCfile);
      outfile = fopen('TempFlex.txt','w');
      for(i=1:(28+7*Ibody)),
         line = fgets(infile,512);
         fputs(outfile,line);
      end

      % Mass, MOI, POI, cm
      line = fgets(infile,512);
      fprintf(outfile,'%f                       ! Mass\n',mass);
      line = fgets(infile,512);
      fprintf(outfile,'%f %f %f     ! Moments of Inertia (kg-m^2)\n',...
         Ixx,Iyy,Izz);
      line = fgets(infile,512);
      fprintf(outfile,'%f %f %f     ! Products of Inertia (xy,xz,yz)\n',...
         Ixy,Ixz,Iyz);
      line = fgets(infile,512);
      fprintf(outfile,'%f %f %f     ! Location of mass center, m\n',...
         xcg,ycg,zcg);

      % Geometry Input File Name
      line = fgets(infile,512);
      fputs(outfile,line);     

      line = fgets(infile,512);
      fprintf(outfile,'%s  ! Flex File Name\n',FlexFile);

      % Joint locations

      while(~feof(infile)),
         line = fgets(infile,512);
         fputs(outfile,line);
      end

      fclose(infile);
      fclose(outfile);         

      line = sprintf('mv TempFlex.txt %s',SCfile);
      system(line);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% ... Write to Flex file

      outfile = fopen(FlexFile,'w');

      fprintf(outfile,'<<<<<<<<<<<<<<  Flexible Body Inputs for 42  >>>>>>>>>>>>>>>>>>>\n');
      fprintf(outfile,'%s                 ! Description\n',FlexFile);

      % No. of modes
      fprintf(outfile,'%i                                     ! Number of Flex Modes\n',Nmode);

      % Initial Modal States
      fprintf(outfile,'****************** Initial Modal States x, u  ******************\n');
      for i=1:Nmode,
         fprintf(outfile,'0.0  0.0                             ! Mode %i  x, u\n',i-1);
      end

      % Node locations
      fprintf(outfile,'****************** Analysis Node Locations *********************\n');
      fprintf(outfile,'%i                                   ! Number of Analysis Nodes\n',Nnode);
      for Inode=1:Nnode,
         fprintf(outfile,'%i    %f %f %f  ! Node ID, location, m\n', ...
            NodeID(Inode),NodePosB(Inode,1),NodePosB(Inode,2),NodePosB(Inode,3));
      end

      % Joint Mode Shapes
      fprintf(outfile,'****************** Joint Node Mode Shapes *********************\n');
      fprintf(outfile,'*******  Non-zero Translation Mode Shape (PSI) Elements ********\n');
      NumNonZero = 0;
      for Imode = 1:Nmode,
         for Ijoint = 1:Njoint,
            for Iaxis = 1:3,
               if (PSIj(Imode,Ijoint,Iaxis) ~= 0.0),
                  NumNonZero = NumNonZero + 1;
               end
            end
         end
      end
      fprintf(outfile,'%i                            ! Number of non-zero elements\n',NumNonZero);
      for Imode = 1:Nmode,
         for Ijoint = 1:Njoint,
            for Iaxis = 1:3,
               if (PSIj(Imode,Ijoint,Iaxis) ~= 0.0),
                  fprintf(outfile,'  %i  %i  %i  %e   ! [Mode] [Joint] [Axis] Value\n',...
                     Imode-1,JointTag(Ijoint),Iaxis-1,PSIj(Imode,Ijoint,Iaxis));
               end
            end
         end
      end

      fprintf(outfile,'*******  Non-zero Rotation Mode Shape (THETA) Elements ********\n');
      NumNonZero = 0;
      for Imode = 1:Nmode,
         for Ijoint = 1:Njoint,
            for Iaxis = 1:3,
               if (THETAj(Imode,Ijoint,Iaxis) ~= 0.0),
                  NumNonZero = NumNonZero + 1;
               end
            end
         end
      end
      fprintf(outfile,'%i                            ! Number of non-zero elements\n',NumNonZero);
      for Imode = 1:Nmode,
         for Ijoint = 1:Njoint,
            for Iaxis = 1:3,
               if (THETAj(Imode,Ijoint,Iaxis) ~= 0.0),
                  fprintf(outfile,'  %i  %i  %i  %e   ! [Mode] [Joint] [Axis] Value\n',...
                     Imode-1,JointTag(Ijoint),Iaxis-1,THETAj(Imode,Ijoint,Iaxis));
               end
            end
         end
      end

      % Node Mode Shapes
      fprintf(outfile,'**************** Analysis Node Mode Shapes *********************\n');
      fprintf(outfile,'*******  Non-zero Translation Mode Shape (PSI) Elements ********\n');
      NumNonZero = 0;
      for Imode = 1:Nmode,
         for Inode = 1:Nnode,
            for Iaxis = 1:3,
               if (PSIn(Imode,Inode,Iaxis) ~= 0.0),
                  NumNonZero = NumNonZero + 1;
               end
            end
         end
      end
      fprintf(outfile,'%i                            ! Number of non-zero elements\n',NumNonZero);
      for Imode = 1:Nmode,
         for Inode = 1:Nnode,
            for Iaxis = 1:3,
               if (PSIn(Imode,Inode,Iaxis) ~= 0.0),
                  fprintf(outfile,'  %i  %i  %i  %e   ! [Mode] [Node] [Axis] Value\n',...
                     Imode-1,Inode-1,Iaxis-1,PSIn(Imode,Inode,Iaxis));
               end
            end
         end
      end

      fprintf(outfile,'*******  Non-zero Rotation Mode Shape (THETA) Elements ********\n');
      NumNonZero = 0;
      for Imode = 1:Nmode,
         for Inode = 1:Nnode,
            for Iaxis = 1:3,
               if (THETAn(Imode,Inode,Iaxis) ~= 0.0),
                  NumNonZero = NumNonZero + 1;
               end
            end
         end
      end
      fprintf(outfile,'%i                            ! Number of non-zero elements\n',NumNonZero);
      for Imode = 1:Nmode,
         for Inode = 1:Nnode,
            for Iaxis = 1:3,
               if (THETAn(Imode,Inode,Iaxis) ~= 0.0),
                  fprintf(outfile,'  %i  %i  %i  %e   ! [Mode] [Node] [Axis] Value\n',...
                     Imode-1,Inode-1,Iaxis-1,THETAn(Imode,Inode,Iaxis));
               end
            end
         end
      end

      % Mass Matrix
      fprintf(outfile,'****************  Non-zero Mass Matrix Elements ******************\n');
      fprintf(outfile,'%i                              ! Number of non-zero elements\n',Nmode);
      for Imode = 1:Nmode
         fprintf(outfile,'%i %i %e      ! [Row] [Col] Value\n',Imode-1,Imode-1,Mf(Imode));
      end

      % Stiffness Matrix
      fprintf(outfile,'*************  Non-zero Stiffness Matrix Elements ****************\n');
      fprintf(outfile,'%i                              ! Number of non-zero elements\n',Nmode);
      for Imode = 1:Nmode
         fprintf(outfile,'%i %i %e      ! [Row] [Col] Value\n',Imode-1,Imode-1,Kf(Imode));
      end

      % Damping Matrix
      fprintf(outfile,'*************  Non-zero Damping Matrix Elements ****************\n');
      fprintf(outfile,'%i                              ! Number of non-zero elements\n',Nmode);
      for Imode = 1:Nmode
         DampCoef = 2*DampingRatio*sqrt(Kf(Imode)*Mf(Imode));
         fprintf(outfile,'%i %i %e      ! [Row] [Col] Value\n',Imode-1,Imode-1,DampCoef);
      end

      % Modal Integrals
      fprintf(outfile,'*********  Linear Momentum Modal Integral, Pf, 3 x Nf  *********\n');
      for Imode = 1:Nmode,
         fprintf(outfile,'%e %e %e  ! Mode %i, [x y z]\n',0.0,0.0,0.0,Imode-1);
      end
      fprintf(outfile,'********  Angular Momentum Modal Integral, Hf, 3 x Nf  *********\n');
      for Imode = 1:Nmode,
         fprintf(outfile,'%e %e %e  ! Mode %i, [x y z]\n',0.0,0.0,0.0,Imode-1);
      end

      fprintf(outfile,'**********  Linear Modal Integral, L, 3 x 3 x Nf  **************\n');
      fprintf(outfile,'0                                    ! Number of non-zero elements\n');
      fprintf(outfile,'*********  Angular Modal Integral, N, 3 x 3 x Nf x Nf  *********\n');
      fprintf(outfile,'0                                    ! Number of non-zero elements\n');
      fprintf(outfile,'[EOF]\n');


      fclose(outfile);

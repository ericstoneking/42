% Thruster Selection by Pseudo-Inverse 
% with Nullspace Deflation
%    Eric Stoneking, March 2011

% Thruster Locations
r = [ 1  1  1;  1  1  1;  1  1  1;
     -1 -1  1; -1 -1  1; -1 -1  1;
      1 -1 -1;  1 -1 -1;  1 -1 -1;
     -1  1 -1; -1  1 -1; -1  1 -1];

% Thruster Axis Vectors
a = [ -1  0  0;  0 -1  0;  0  0 -1;
       1  0  0;  0  1  0; -1  0  0;
      -1  0  0;  0  1  0;  0  0  1;
       1  0  0;  0 -1  0;  0  0  1];

% "A" Matrix, so [F; T] = A*Thrust
Af = a;
At = cross(r,a);
A = [Af At]';

% Pseudo-inverse, so ThrCmd = Ap*[Fcmd; Tcmd]
Ap = pinv(A);

% Randomly generate Fcmd, Tcmd
Fcmd = 2*rand(3,1)-1;
Tcmd = 2*rand(3,1)-1;

% Echo Desired Fcmd, Tcmd
[Fcmd;Tcmd]

% First try at ThrCmd
ThrCmd = Ap*[Fcmd;Tcmd];

% To keep track of which thrusters get non-zero commands
ThrIdx = [1 2 3 4 5 6 7 8 9 10 11 12];

% Our nullspace has six dimensions, so we will 
% deflate it six times, removing six thrusters
% from the solution.  The eliminated thrusters
% are then assigned ThrCmd = 0.
for i=1:6,
   % Find thruster with the minimum ThrCmd.
   % This indicates that it contributes least
   % (maybe even is antagonistic to) desired 
   % F and T.
   [thrmin,imin] = min(ThrCmd);

   % Strike that thruster from the solution.
   % This deflates the nullspace by one dimension.
   A(:,imin) = [];
   ThrIdx(imin) = [];

   % Find reduced-dimension pseudo-inverse and ThrCmd
   Ap = pinv(A);
   ThrCmd = Ap*[Fcmd;Tcmd];
end

% Now, A is 6x6, and ThrCmd is 6x1.  
A*ThrCmd

ThrIdx
FinalThrCmd = zeros(12,1);
FinalThrCmd(ThrIdx) = ThrCmd;
FinalThrCmd

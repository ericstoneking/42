function Cmd = LinProg(Onpulsing)
% Thruster Selection by Linear Programming

% T is tableau
T=MakeTableau(Onpulsing);

[m,n]=size(T);

T=[T(1,:);GaussElim(T(2:m,:))];

while(any(T(2:m-1,n)<0.0)),
   % Pick columns to swap
   [incoming,outgoing]=FindSwap(T);
   % Swap Columns
   T=swapcol(T,incoming,outgoing);
   % Gaussian Elimination
   T=[T(1,:);GaussElim(T(2:m,:))];
end

% Interpret Tableau
Idx = T(1,1:n-1);
b = T(2:m-1,n);
Cost = -T(m,n)
Cmd = zeros(n-1,1);
for i=1:m-2,
   Cmd(Idx(i)) = b(i);
end
if ~Onpulsing,
   Cmd = 1.0-Cmd;
end

return




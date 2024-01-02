function T=MakeTableau(Onpulsing)

basis = [1 2 3 4];
A = [0 1 0 -1;1 0 -1 0];
c = [1 1 1 1];
f = [0.2 -0.7]';
if Onpulsing,
   b = f;
else
   b = A*ones(4,1)-f;
end

T=[basis 0;A b;c 0];

return
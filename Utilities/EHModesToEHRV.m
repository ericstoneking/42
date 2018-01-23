function [r,v] = EHModesToEHRV(A,Bc,Bs,C,Dc,Ds,n,nt);

s = sin(nt);
c = cos(nt);

x = 3/2*A*nt + 2*(Bc*s - Bs*c) + C;
y = Dc*s - Ds*c;
z = A + (Bc*c + Bs*s);
u = 3/2*A*n + 2*n*(Bc*c + Bs*s);
v = n*(Dc*c + Ds*s);
w = -n*(Bc*s - Bs*c);

r = [x,y,z];
v = [u,v,w];
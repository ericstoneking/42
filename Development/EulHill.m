mu = 3.98604E14;
a = 6378.145E3 + 500.0E3;
n = sqrt(mu/a^3);

A = 1000;
B = 0;
thp = 0;
Bc = B*cos(thp);
Bs = B*sin(thp);
C = -5000;
D = 0;
thn = 0;
Dc = D*cos(thn);
Ds = D*sin(thn);

dt = 10;
N = 4000;
nt = zeros(N,1);
r = zeros(N,3);
v = zeros(N,3);

Maneuver = 0;
for i=1:N,
   nt(i) = n*(i-1)*dt;
   c = cos(nt(i));
   s = sin(nt(i));

   r(i,1) = 1.5*A*nt(i) + 2*(Bc*s - Bs*c) + C;
   r(i,2) = Dc*s - Ds*c;
   r(i,3) = A + Bc*c + Bs*s;
   v(i,1) = 1.5*A*n + 2*n*(Bc*c + Bs*s);
   v(i,2) = n*(Dc*c + Ds*s);
   v(i,3) = -n*(Bc*s - Bs*c);

   if i>1,
      if (r(i-1,1) < 0 & r(i,1) > 0 & Maneuver == 0),
         Maneuver = 1;
         A = 500;
         B = 500;
         Bc = B*cos(nt(i));
         Bs = B*sin(nt(i));
         C = -2500;
      end 
      if (v(i-1,3) < 0 & v(i,3) > 0 & Maneuver == 1),
         Maneuver = 2;
         A = 0;
         B = 0;
         Bc = B*sin(nt(i));
         Bs = -B*cos(nt(i));
         C = r(i,1);
      end
   end

end

figure(1)
plot(nt,r)
legend("x","y","z")

figure(2)
plot(nt,v)
legend("u","v","w")

figure(3)
title("X vs. Z")
plot(r(:,1),r(:,3))
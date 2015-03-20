function [rin,rout] = ShellOfInfluence(m)

% Perturbation ratio threshold
% Outer shell isn't always well defined for 
% ratios much smaller
e = 0.1; 

p0 = m^0.4;

p = p0;
p1 = 1+p;
dp = 1.0;
while (abs(dp) > 1.0E-4*p),
   f = m*p1*p1*(1.0/p/p+1.0)-e;
   fp = 2.0*m*p1*((1.0/p/p+1.0) - p1/p/p/p);
   dp = -0.5*f/fp;
   if (dp < -0.25*p),
      dp = -0.25*p;
   end
   p = p + dp;
   p1 = 1.0+p;
end
rout = p;

p = p0;
p1 = 1.0-p;
dp = 1.0;
while (abs(dp) > 1.0E-4*p),
   f = 1.0/m*p*p*(1.0/p1/p1-1.0)-e;
   fp = 2.0/m*p*((1.0/p1/p1-1.0) - p/p1/p1/p1);
   dp = -0.5*f/fp;
   if dp < -0.25*p, 
      dp = -0.25*p;
   end
   p = p + dp;
   p1 = 1.0-p;
end
rin = p;

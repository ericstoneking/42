%  Build a model of the Earth-Moon Circular Restricted Three-Body Problem
%  to develop libration-point techniques for 42

mu1 = 398601; % Earth
mu2 = 4887;   % Moon
D = 384748;   % Earth-Moon

%mu1 = 1.32715E11;  % Sun
%mu2 = 398604;     % Earth
%D = 1.496E8;      % Sun-Earth

%mu1 = 0.99;
%mu2 = 0.01;
%D = 1;

n = sqrt((mu1+mu2)/D^3);
p = mu2/(mu1+mu2);
p1 = 1.0-p;

t=[0:pi/100:15*pi]';

%  L1 
x = -1.0;
dx = 1.0;
while abs(dx) > 2.0e-16,
    xp = x - p;
    x1p = xp + 1.0;
    f = x + p1/(xp*xp) - p/(x1p*x1p);
    fx = 1.0 - 2.0*p1/(xp*xp*xp) + 2.0*p/(x1p*x1p*x1p);
    dx = -f/fx;
    x = x + dx;
end
X0 = x;
Y0 = 0;

R1 = sqrt((X0-p)^2+Y0^2);
R2 = sqrt((X0+1-p)^2+Y0^2);

R13 = R1^3;
R15 = R1^5;
R23 = R2^3;
R25 = R2^5;

Uxx = 1.0-(p1*(1/R13 - 3*(X0-p)^2/R15)+p*(1/R23 - 3*(X0+1-p)^2/R25));
Uyy = 1.0-(p1*(1/R13 - 3*Y0^2/R15)+p*(1/R23 - 3*Y0^2/R25));
Uxy = 0.0;
Uzz = -(p1/R13 + p/R23);

B1 = 2.0-0.5*(Uxx+Uyy);
B2 = sqrt(-Uxx*Uyy);
wxy = sqrt(B1+sqrt(B1^2+B2^2));
wz = sqrt(-Uzz);

A = 0.1;
Az = 0.1;
B = A/(2*wxy)*(wxy^2+Uyy);
x1 = B*sin(wxy*t) + X0;
y1 = A*cos(wxy*t) + Y0;
z1 =  Az*sin(wz*t);

%  L2
x = -1.0;
dx = 1.0;
while abs(dx) > 2.0e-16,
    xp = x - p;
    x1p = xp + 1.0;
    f = x + p1/(xp*xp) + p/(x1p*x1p);
    fx = 1.0 - 2.0*p1/(xp*xp*xp) - 2.0*p/(x1p*x1p*x1p);
    dx = -f/fx;
    x = x + dx;
end
X0 = x;
Y0 = 0;

R1 = sqrt((X0-p)^2+Y0^2);
R2 = sqrt((X0+1-p)^2+Y0^2);

R13 = R1^3;
R15 = R1^5;
R23 = R2^3;
R25 = R2^5;

Uxx = 1.0-(p1*(1/R13 - 3*(X0-p)^2/R15)+p*(1/R23 - 3*(X0+1-p)^2/R25));
Uyy = 1.0-(p1*(1/R13 - 3*Y0^2/R15)+p*(1/R23 - 3*Y0^2/R25));
Uxy = 0.0;
Uzz = -(p1/R13 + p/R23);

B1 = 2.0-0.5*(Uxx+Uyy);
B2 = sqrt(-Uxx*Uyy);
wxy = sqrt(B1+sqrt(B1^2+B2^2));
wz = sqrt(-Uzz);

A = 0.1;
Az = 0.1;
B = A/(2*wxy)*(wxy^2+Uyy);
x2 = B*sin(wxy*t) + X0;
y2 = A*cos(wxy*t) + Y0;
z2 =  Az*sin(wz*t);

%  L3
x =  1.0;
dx = 1.0;
while abs(dx) > 2.0e-16,
    xp = x - p;
    x1p = xp + 1.0;
    f = x - p1/(xp*xp) - p/(x1p*x1p);
    fx = 1.0 + 2.0*p1/(xp*xp*xp) + 2.0*p/(x1p*x1p*x1p);
    dx = -f/fx;
    x = x + dx;
end
X0 = x;
Y0 = 0;

R1 = sqrt((X0-p)^2+Y0^2);
R2 = sqrt((X0+1-p)^2+Y0^2);

R13 = R1^3;
R15 = R1^5;
R23 = R2^3;
R25 = R2^5;

Uxx = 1.0-(p1*(1/R13 - 3*(X0-p)^2/R15)+p*(1/R23 - 3*(X0+1-p)^2/R25));
Uyy = 1.0-(p1*(1/R13 - 3*Y0^2/R15)+p*(1/R23 - 3*Y0^2/R25));
Uxy = 0.0;
Uzz = -(p1/R13 + p/R23);

B1 = 2.0-0.5*(Uxx+Uyy);
B2 = sqrt(-Uxx*Uyy);
wxy = sqrt(B1+sqrt(B1^2+B2^2));
wz = sqrt(-Uzz);

A = 0.1;
Az = 0.1;
B = A/(2*wxy)*(wxy^2+Uyy);
x3 = B*sin(wxy*t) + X0;
y3 = A*cos(wxy*t) + Y0;
z3 =  Az*sin(wz*t);

%  L4
X0 = p-0.5;
Y0 = 0.5*sqrt(3);

Uxx = 3/4;
Uyy = 9/4;
Uxy = -3*sqrt(3)/4*(1.0-2*p);
Uzz = -1.0;

w1 = sqrt(0.5*(1.0-sqrt(1-27*p*(1-p))));
w2 = sqrt(0.5*(1.0+sqrt(1-27*p*(1-p))));
wz = 1;

alpha=0.5*atan2(4*Uxy,-3);
ca=cos(alpha);
sa=sin(alpha);

A1 = 0.1;
A2 = 0.0;
B1 = A1*sqrt(((Uxy^2+4*w1^2)*ca^2+(w1^2+Uxx)^2*sa^2-2*Uxy*(w1^2+Uxx)*sa*ca)...
            /((Uxy^2+4*w1^2)*sa^2+(w1^2+Uxx)^2*ca^2-2*Uxy*(w1^2+Uxx)*sa*ca));
B2 = A2*sqrt(((Uxy^2+4*w2^2)*ca^2+(w2^2+Uxx)^2*sa^2-2*Uxy*(w2^2+Uxx)*sa*ca)...
            /((Uxy^2+4*w2^2)*sa^2+(w2^2+Uxx)^2*ca^2-2*Uxy*(w2^2+Uxx)*sa*ca));
Az = 0.1;

xi = B1*sin(w1*t)+B2*sin(w2*t);
eta = A1*cos(w1*t)+A2*cos(w2*t);

x4 = ca*xi - sa*eta + X0;
y4 = sa*xi + ca*eta + Y0;
z4 = Az*sin(wz*t);

%  L5
X0 = p-0.5;
Y0 = -0.5*sqrt(3);

Uxx = 3/4;
Uyy = 9/4;
Uxy =  3*sqrt(3)/4*(1.0-2*p);
Uzz = -1.0;

w1 = sqrt(0.5*(1.0-sqrt(1-27*p*(1-p))));
w2 = sqrt(0.5*(1.0+sqrt(1-27*p*(1-p))));
wz = 1;

alpha=0.5*atan2(4*Uxy,-3);
ca=cos(alpha);
sa=sin(alpha);

A1 = 0.0;
A2 = 0.1;
B1 = A1*sqrt(((Uxy^2+4*w1^2)*ca^2+(w1^2+Uxx)^2*sa^2-2*Uxy*(w1^2+Uxx)*sa*ca)...
            /((Uxy^2+4*w1^2)*sa^2+(w1^2+Uxx)^2*ca^2-2*Uxy*(w1^2+Uxx)*sa*ca));
B2 = A2*sqrt(((Uxy^2+4*w2^2)*ca^2+(w2^2+Uxx)^2*sa^2-2*Uxy*(w2^2+Uxx)*sa*ca)...
            /((Uxy^2+4*w2^2)*sa^2+(w2^2+Uxx)^2*ca^2-2*Uxy*(w2^2+Uxx)*sa*ca));
Az = 0.1;

xi = B1*sin(w1*t)+B2*sin(w2*t);
eta = A1*cos(w1*t)+A2*cos(w2*t);

x5 = ca*xi - sa*eta + X0;
y5 = sa*xi + ca*eta + Y0;
z5 = Az*sin(wz*t);



figure(1)
plot(x1,y1,x2,y2,x3,y3,x4,y4,x5,y5)
%figure(2)
%plot(t,[x4 y4 z4])

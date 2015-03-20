m1 = 1000;
m2 = 1500;
m3 = 2000;
m = diag([m1 m1 m1 m2 m2 m2 m3 m3 m3]);
I1 = diag([1000 1100 1200]);
I2 = diag([1300 1400 1500]);
I3 = diag([1600 1700 1800]);
I = [I1 zeros(3,6); zeros(3,3) I2 zeros(3,3); zeros(3,6) I3];
r11 = [0 1 1.5]';
r21 = [0 1 -1.5]';
r22 = [0 1 1.5]';
r32 = [0 1 -1.5]';

u = zeros(8,1);
x = zeros(8,1);
u(4) = 1*pi/180;
x(1) = 0;

% Loop

c1 = cos(x(1));
s1 = sin(x(1));
C1N = [1 0 0; 0 c1 s1; 0 -s1 c1];
c2 = cos(x(2));
s2 = sin(x(2));
C21 = [1 0 0; 0 c2 s2; 0 -s2 c2];
c3 = cos(x(3));
s3 = sin(x(3));
C32 = [1 0 0; 0 c3 s3; 0 -s3 c3];
C2N = C21*C1N;
C31 = C32*C21;
C3N = C32*C2N;

w1 = [u(1) 0 0]';
w2 = w1 + [u(4) 0 0]';
w3 = w2 + [u(5) 0 0]';

beta2 = C2N'*r21 - C1N'*r11;
beta3 = C3N'*r32 + C2N'*(r21-r22) - C1N'*r11;
rho21 = C2N'*r21;
rho32 = C3N'*r32;
rho31 = C3N'*r32 + C2N'*(r21-r22);

Beta2 = [0        -beta2(3)  beta2(2); 
         beta2(3)    0      -beta2(1); 
        -beta2(2)  beta2(1)         0];

Beta3 = [0        -beta3(3)  beta3(2); 
         beta3(3)    0      -beta3(1); 
        -beta3(2)  beta3(1)         0];

Rho21 = [0        -rho21(3)  rho21(2); 
         rho21(3)    0      -rho21(1); 
        -rho21(2)  rho21(1)         0];

Rho32 = [0        -rho32(3)  rho32(2); 
         rho32(3)    0      -rho32(1); 
        -rho32(2)  rho32(1)         0];

Rho31 = [0        -rho31(3)  rho31(2); 
         rho31(3)    0      -rho31(1); 
        -rho31(2)  rho31(1)         0];

Gamma1 = [1 0 0]';
Gamma2 = [1 0 0]';

PAngVel = [eye(3)                   zeros(3,5);
           C21   Gamma1             zeros(3,4);
           C31   C32*Gamma1  Gamma2 zeros(3,3)];

PVel = [zeros(3,5)                                        eye(3);
        Beta2*C1N'  Rho21*C2N'*Gamma1     zeros(3,1)      eye(3);
        Beta3*C1N'  Rho31*C2N'*Gamma1  Rho32*C3N'*Gamma2  eye(3)];

COEF = PAngVel'*I*PAngVel + PVel'*m*PVel;

wr11 = cross(w1,cross(w1,r11));
wr21 = cross(w2,cross(w2,r21));
wr22 = cross(w2,cross(w2,r22));
wr32 = cross(w3,cross(w3,r32));

ar = [zeros(3,1);
      C1N'*wr11 - C2N'*wr21;
      C1N'*wr11 + C2N'*(wr22-wr21) - C3N'*wr32];

RHS = PVel'*(-m*ar);

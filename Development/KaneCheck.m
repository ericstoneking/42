I1 = 100*eye(3);
I2 = 100*eye(3);
m1 = 100;
m2 = 100;
r11 = [2 0 0]';
r21 = [2 0 0]';

q1n = [0.0  0.0  0.0  1.0]';
theta = [0 0 0]*pi/180;
w1 = [0.0 0.0 1.0]'*pi/180;
sigma = [0.0 0.0 0.0]'*pi/180;

d = [0.1 0.0 0.0]';
spd = [0.0 0.0 0.0]';
D = [0 1 0; 0 0 1; 1 0 0];

s = sin(theta);
c = cos(theta);

G = [c(2)*c(3) s(3) 0; -c(2)*s(3) c(3) 0; s(2) 0 1];

C21 = [c(2)*c(3) s(1)*s(2)*c(3)+s(3)*c(1) -c(1)*s(2)*c(3)+s(3)*s(1);
      -c(2)*s(3) -s(1)*s(2)*s(3)+c(3)*c(1) c(1)*s(2)*s(3)+c(3)*s(1); 
      s(2) -s(1)*c(2) c(1)*c(2)];
C1N = [   1-2*q1n(2)^2-2*q1n(3)^2         2*(q1n(1)*q1n(2)-q1n(3)*q1n(4))    2*(q1n(3)*q1n(1)+q1n(2)*q1n(4));
       2*(q1n(2)*q1n(1)+q1n(3)*q1n(4))       1-2*q1n(3)^2-2*q1n(1)^2         2*(q1n(2)*q1n(3)-q1n(1)*q1n(4));
       2*(q1n(3)*q1n(1)-q1n(2)*q1n(4))    2*(q1n(2)*q1n(3)+q1n(1)*q1n(4))        1-2*q1n(1)^2-2*q1n(2)^2]';

Gds = [-sigma(1)*(sigma(2)*s(2)*c(3)+sigma(3)*c(2)*s(3))+sigma(2)*sigma(3)*c(3);
        sigma(1)*(sigma(2)*s(2)*s(3)-sigma(3)*c(2)*c(3))-sigma(2)*sigma(3)*s(3);
        sigma(1)*sigma(2)*c(2)];

C2N = C21*C1N;

Gs = G*sigma;

w2 = C21*w1 + Gs;

wxH = [cross(w1,I1*w1);cross(w2,I2*w2)];

wxGs = cross(w2,Gs);

beta2 = C2N'*r21 - C1N'*(r11+D*d);
rho21 = C2N'*r21;

B2 = [0 -beta2(3) beta2(2); beta2(3) 0 -beta2(1); -beta2(2) beta2(1) 0];
R21 = [0 -rho21(3) rho21(2); rho21(3) 0 -rho21(1); -rho21(2) rho21(1) 0];

KPAngVel = [eye(3) zeros(3,9); C21 G zeros(3,6)];
KPVel = [zeros(3,9) eye(3); B2*C1N' R21*C2N'*G C1N'*D eye(3)];

I = [I1 zeros(3,3); zeros(3,3) I2];
m = [m1*eye(3) zeros(3,3); zeros(3,3) m2*eye(3)];

KIPAngVel = I*KPAngVel;
KmPVel = m*KPVel;

Kalfr = [zeros(3,1); Gds + wxGs];
Kaccr = [zeros(3,1); -C2N'*cross(Kalfr(4:6),r21)+C1N'*cross(w1,cross(w1,r11+D*d))-C2N'*cross(w2,cross(w2,r21))+2*C1N'*cross(w1,D*spd)];
KInertTrq = -I*Kalfr - wxH;
KInertFrc = -m*Kaccr;
KCOEF = KPAngVel'*KIPAngVel + KPVel'*KmPVel;
KRHS = KPAngVel'*KInertTrq + KPVel'*KInertFrc;
Kudot = KCOEF\KRHS;

load PAngVel.42
load PVel.42
load COEF.42
load RHS.42
load InertiaFrc.42
load InertiaTrq.42
load AccR.42
load AlphaR.42


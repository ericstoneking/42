% Parameters
I1 = diag([100 100 100]);
I2 = diag([100 100 100]);
m1 = 10;
m2 = 10;
r1 = [1 0 0]';
r2 = [-1 0 0]';
Delta = [0 1 0]';

% Initial State
q1n = [0 0 0 1]';
d = 0.0;
x1 = [-1 0 0]';
w1 = [0 0 0]';
s = 0.01;
v1 = [0 -0.005 0]';

dt = 0.01;
t = [0:dt:100]';
N = max(size(t));

% 1-DOF joint
X = zeros(N,4+1+3);
U = zeros(N,3+1+3);

Hvn = zeros(N,3);
Pvn = zeros(N,3);
xcm = zeros(N,3);

U(1,:) = [w1' s' v1'];
X(1,:) = [q1n' d' x1'];

AlphaR = zeros(6,1);
AccR = zeros(6,1);

Kp = 0.1;
I = [I1 zeros(3);zeros(3) I2];
m = diag([m1 m1 m1 m2 m2 m2]);

for k=1:N-1,
   w1 = U(k,1:3)';
   w2 = w1;
   s = U(k,4);
   v1 = U(k,5:7)';
   q1n = X(k,1:4)';
   C1N = Q2C(q1n);
   d = X(k,5);
   x1 = X(k,6:8)';

   wxH = [cross(w1,I1*w1);cross(w2,I2*w2)];

   B2 = C1N'*(r2-Delta*d-r1);
   B2x = [  0   -B2(3)  B2(2);
          B2(3)    0   -B2(1);
         -B2(2)  B2(1)    0];

   v2 = v1 + B2x*w1 + C1N'*Delta*s;
   x2 = x1 - B2;
   xcm(k,:) = (m1*x1+m2*x2)'/(m1+m2);

   Pvn(k,:) = (m1*v1 + m2*v2)';
   Hvn(k,:) = (I1*w1 + I2*w2)'*C1N;
   
   PAngVel = [eye(3)  zeros(3,1) zeros(3,3);
              eye(3)  zeros(3,1) zeros(3,3)];

   PVel = [zeros(3,3) zeros(3,1)  eye(3);
              B2x     C1N'*Delta  eye(3)];

   F = C1N'*(Delta*(-Kp*d));
   Frc = [-F;F];
   %Trq = [cross(r1,       -Delta*(-Kp*d));
   %       cross(r2-Delta*d,Delta*(-Kp*d))];
   Trq = zeros(6,1);

   COEF = PAngVel'*I*PAngVel + PVel'*m*PVel;
   RHS = PAngVel'*(Trq - I*AlphaR - wxH) + PVel'*(Frc - m*AccR);

   udot = COEF\RHS;
   Q = [ q1n(4) -q1n(3)  q1n(2);
         q1n(3)  q1n(4) -q1n(1);
        -q1n(2)  q1n(1)  q1n(4);
        -q1n(1) -q1n(2) -q1n(3)];
   qdot = 1/2*Q*w1;

   U(k+1,:) = U(k,:) + udot'*dt;
   X(k+1,:) = X(k,:) + [qdot' s v1']*dt;
   X(k+1,1:4) = X(k+1,1:4)/norm(X(k+1,1:4));

end
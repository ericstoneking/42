I1 = 100*eye(3);
I2 = 100*eye(3);
m1 = 100*eye(3);
m2 = 100*eye(3);
r1 = [2 0 0]';
r2 = [2 0 0]';

CGiBi = eye(3);
CBoGo = diag([-1 -1 1]);

G = [1 0 0]';

AlphaR = zeros(6,1);

I = [I1 zeros(3,3); zeros(3,3) I2];
m = [m1 zeros(3,3); zeros(3,3) m2];

K = 0.1;

dt = 0.1;
t = [0:dt:100]';
N = max(size(t));

U = zeros(N,7);
X = zeros(N,8);
P1 = zeros(N,3);
P2 = zeros(N,3);
V1 = zeros(N,3);
V2 = zeros(N,3);
W1 = zeros(N,3);
W2 = zeros(N,3);
Iw1 = zeros(N,3);
Iw2 = zeros(N,3);
rxmv1 = zeros(N,3);
rxmv2 = zeros(N,3);
Hvn = zeros(N,3);

X(1,4) = 1.0;
X(1,5) = 30*pi/180;

% Find pcm
C1N = Q2C(X(1,1:4));
sa = sin(X(1,5));
ca = cos(X(1,5));
CGoGi = [1   0   0;
         0  ca  sa; 
         0 -sa  ca];
C21 = CBoGo*CGoGi*CGiBi;
C2N = C21*C1N;
p1 = X(1,6:8)';
p2 = p1 + C1N'*r1 - C2N'*r2;
pcm = 0.5*(p1+p2);
X(1,6:8) = X(1,6:8)-pcm';

for k=1:N-1,
   u = U(k,:)';
   x = X(k,:)';
   q = x(1:4);
   a = x(5);
   w1 = u(1:3);
   s = u(4);
   v1 = u(5:7);

   C1N = Q2C(q);
   CN1 = C1N';
   sa = sin(a);
   ca = cos(a);
   CGoGi = [1   0   0;
            0  ca  sa; 
            0 -sa  ca];
   C21 = CBoGo*CGoGi*CGiBi;
   C2N = C21*C1N;
   CN2 = C2N';
   Gs = [s 0 0]';
   Gds = [0 0 0]';

   w2 = C21*w1 + CBoGo*Gs;

   B = CN2*r2-CN1*r1;;
   Bx = SkewM(B);
   rx = CN2*SkewM(r2);

   PAngVel = [eye(3) zeros(3,4);C21 CBoGo*G zeros(3,3)];
   PVel =  [zeros(3,4) eye(3);Bx*CN1 rx*CN2*CBoGo*G eye(3)];

   W1(k,:) = w1';
   W2(k,:) = w2';
   V1(k,:) = (PVel(1:3,:)*u)';
   V2(k,:) = (PVel(4:6,:)*u)';
   P1(k,:) = x(6:8)';
   P2(k,:) = P1(k,:) - B';
   pcm = 0.5*(P1(k,:)+P2(k,:));
   P1(k,:) = P1(k,:) - pcm;
   P2(k,:) = P2(k,:) - pcm;
   vcm = 0.5*(V1(k,:)+V2(k,:));
   V1(k,:) = V1(k,:) - vcm;
   V2(k,:) = V2(k,:) - vcm;
   Iw1(k,:) = (CN1*I1*w1)';
   Iw2(k,:) = (CN2*I2*w2)';
   rxmv1(k,:) = cross(P1(k,:)',m1*V1(k,:)');
   rxmv2(k,:) = cross(P2(k,:)',m2*V2(k,:)');

   X(k,6:8) = X(k,6:8) - pcm;
   U(k,5:7) = U(k,5:7) - vcm;

   F1 = zeros(3,1);
   F2 = -F1;
   T1 = CGiBi'*[K*a;0;0];
   T2 = CBoGo*[-K*a;0;0];
   T = [T1;T2];
   F = [F1;F2];
   AlphaR = [zeros(3,1);CBoGo*Gds+cross(w2,CBoGo*Gs)];
   ar2 = CN1*cross(w1,cross(w1,r1))-CN2*cross(w2,cross(w2,r2));
   AccR = [zeros(3,1);ar2];

   COEF = PAngVel'*I*PAngVel + PVel'*m*PVel;
   RHS = PAngVel'*(T-I*AlphaR) + PVel'*(F-m*AccR);
   udot = COEF\RHS;

   qdot = QW2QDOT(q,w1)';

   xdot = [qdot;s;v1];

   U(k+1,:) = U(k,:) + udot'*dt;
   X(k+1,:) = X(k,:) + xdot'*dt + 0.5*dt*dt*[udot(1:3);0;udot(4:7)]';
end

Hvn = Iw1+rxmv1+Iw2+rxmv2;

figure(1)
plot(t,Hvn)


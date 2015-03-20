J = 100;
wc = 0.1;
zc = 0.7;
Kp = J*wc*wc;
Kr = J*2*zc*wc;

alpha = 1.1;
wf = diag([0 0.25]);
zf = diag([0 0.01]);
PHI = 1/sqrt(J)*[1 sqrt(alpha-1); 1 -sqrt(1/(alpha-1))];

Nt=5001;
th = zeros(2,Nt);
thdot = zeros(2,Nt);
eta = zeros(2,Nt);
etadot = zeros(2,Nt);
T = zeros(2,Nt);
N = zeros(2,Nt);

dt = 0.1;

th(:,1) = [1 1]';
eta(:,1) = inv(PHI)*th(:,1);

for k=1:Nt,
   N(:,k) = PHI'*T(:,k);
   acc = N(:,k) - 2*zf*wf*etadot(:,k) - wf*wf*eta(:,k);
   eta(:,k+1) = eta(:,k) + etadot(:,k)*dt + 0.5*acc*dt*dt;
   etadot(:,k+1) = etadot(:,k) + acc*dt;
   th(:,k) = PHI*eta(:,k);
   thdot(:,k) = PHI*etadot(:,k);
   T(1,k+1) = -Kr*thdot(1,k)-Kp*th(1,k);
end

figure(1)
plot(eta')

figure(2)
plot(th')


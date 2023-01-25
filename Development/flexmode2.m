J = 100;
wc = 0.1;
zc = 0.7;
Kp = J*wc*wc;
Kr = J*2*zc*wc;

alpha = [1.1 1.1 1.1];
wf = [0.05 0.1 0.15];
zf = [0.01 0.01 0.01];
phi = sqrt(alpha-1)/sqrt(J);

Nt=50001;
th = zeros(Nt,1);
thdot = zeros(Nt,1);
eta = zeros(Nt,3);
etadot = zeros(Nt,3);
T = zeros(Nt,1);

dt = 0.01;

th(1) = 1;
%eta(1,:) = [1 1 1];

for k=1:Nt,
   T(k) = -Kr*thdot(k)-Kp*th(k);
   etadotdot = phi*T(k) - 2*zf.*wf.*etadot(k,:) - wf.*wf.*eta(k,:);
   eta(k+1,:) = eta(k,:) + etadot(k,:)*dt + 0.5*etadotdot*dt*dt;
   etadot(k+1,:) = etadot(k,:) + etadotdot*dt;
   thdotdot = T(k)/J + sum(phi.*etadotdot);
   th(k+1) = th(k) + thdot(k)*dt + 0.5*thdotdot*dt*dt;
   thdot(k+1) = thdot(k) + thdotdot*dt;
end

figure(1)
plot(eta)

figure(2)
plot(th)


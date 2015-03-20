N = 1;
w = 1.0;
z = 0.1;
s = z*w;
wf = sqrt(w^2-s^2);

e0 = 0.0;
edot0 = 0.0;

if abs(N/w^2-e0) < 1.0e-6,
   psi = 0.5*pi;
   C = edot0/wf;
else,
   psi = atan((s-edot0/(N/w^2-e0))/wf);
   C = (e0-N/w^2)/cos(psi);
end

Nt = 10001;
t = zeros(Nt,1);
e = zeros(Nt,1);
edot = zeros(Nt,1);
edotdot = zeros(Nt,1);
eta = zeros(Nt,1);
etadot = zeros(Nt,1);
etadotdot = zeros(Nt,1);
dt = 0.01;

eta(1) = e0;
etadot(1) = edot0;

for k=1:Nt,
   t(k) = (k-1)*dt;
   e(k) = N/w^2 + C*exp(-s*t(k))*cos(wf*t(k)-psi);
   edot(k) = -C*exp(-s*t(k))*(s*cos(wf*t(k)-psi)+wf*sin(wf*t(k)-psi));
   edotdot(k) = C*exp(-s*t(k))*((s^2-wf^2)*cos(wf*t(k)-psi)+2*s*wf*sin(wf*t(k)-psi));
   etadotdot(k) = N - 2*z*w*etadot(k) - w*w*eta(k);
   eta(k+1) = eta(k) + etadot(k)*dt + 0.5*etadotdot(k)*dt*dt;
   etadot(k+1) = etadot(k) + etadotdot(k)*dt;
end

eta(Nt+1) = [];
etadot(Nt+1) = [];

figure(1)
plot(t,e,"k",t,eta,"r")
legend("e","eta")

figure(2)
plot(t,edot,"k",t,etadot,"r")
legend("edot","etadot")

figure(3)
plot(t,edotdot,"k",t,etadotdot,"r")
legend("edotdot","etadotdot")


DT = 0.1;

% SSIRU Noise Params
SigE = 0.003/sqrt(DT); % Angular Noise, arcsec
SigV = 1.5E-4; % Angle Random Walk, deg/rt-hr
Brig = 1.5E-3; % Bias Stability over Trig hours, deg/hr (= arcsec/sec)
Trig = 12; % Bias Stability Period, hours
SigU = Brig/Trig;

% Convert to consistent units
SigV = SigV*3600/60; % arcsec/rt-sec
SigU = SigU/3600; % arcsec/sec^2

% Noise Model
N = 100;
phi = zeros(N,1);
ARW = zeros(N,1);
b = zeros(N,1);
t = DT*[0:N-1]';
wg = zeros(N,1);

Z1 = 0.5*SigU*DT^(1.5);
Z2 = sqrt(SigV^2*DT+1/12*SigU^2*DT^3);
Z3 = SigU*sqrt(DT);

for k=1:N-1,
   phi(k+1) = phi(k) + DT*b(k)-Z1*randn(1)+ARW(k)+SigE*randn(1);
   ARW(k+1) = ARW(k) + Z2*randn(1);
   b(k+1) = b(k) + Z3*randn(1);
end 
wg = [0;diff(phi)/DT];

figure(1)
plot(t,phi)
title('phi')

figure(2)
plot(t,ARW)
title('ARW')

figure(3)
plot(t,b)
title('Bias')

figure(4)
plot(t,wg)
title('Rate Error')
ylabel('arcsec/sec')


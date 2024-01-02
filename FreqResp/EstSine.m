% Estimate coefficients for a sinusoid
% y(k) = A0 + A1*cos(w*t(k)) + B1*sin(w*t(k))
% x = [A0 A1 B1]';

dt = 0.1;
T = 10;
t = [0:dt:6*T]';
N = length(t);
w = 2*pi/T;
L = 2*dt/T;

A0 = 2*(rand(1)-0.5);
A1 = 2*(rand(1)-0.5);
B1 = 2*(rand(1)-0.5);

C = cos(w*t);
S = sin(w*t);

y = A0 + A1*C + B1*S;

A0hat = zeros(N,1);
A1hat = zeros(N,1);
B1hat = zeros(N,1);
yhat = zeros(N,1);
dA0 = zeros(N,1);
dA1 = zeros(N,1);
dB1 = zeros(N,1);

A0hat(1) = 2*(rand(1)-0.5);
A1hat(1) = 2*(rand(1)-0.5);
B1hat(1) = 2*(rand(1)-0.5);

for k=1:N-1,
   yhat(k) = A0hat(k) + A1hat(k)*C(k) + B1hat(k)*S(k);
   z = y(k) - yhat(k);
   A0hat(k+1) = A0hat(k) + L*z;
   A1hat(k+1) = A1hat(k) + L*C(k)*z;
   B1hat(k+1) = B1hat(k) + L*S(k)*z;

   dA0(k+1) = (A0hat(k+1)-A0hat(k))/(A0hat(k)+1.0E-4);
   dA1(k+1) = (A1hat(k+1)-A1hat(k))/(A1hat(k)+1.0E-4);
   dB1(k+1) = (B1hat(k+1)-B1hat(k))/(B1hat(k)+1.0E-4);
end

figure(1)
plot(t,[A0hat A1hat B1hat],[0;t(end)],[A0 A1 B1;A0 A1 B1])

figure(2)
plot(t,[y yhat])

figure(3)
plot(t,[dA0 dA1 dB1],[0;t(end)],[0.001 -0.001;0.001 -0.001])
axis([0 t(end) -0.005 0.005])



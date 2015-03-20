% Moment of Inertia about spin axis, kg-m^2
I3 = 200; 

% Transverse Moment of Inertia, kg-m^2
It = 100;

% Inertia Ratio
Ki = I3/It;
K1=Ki-1.0;

% Expected magnitude of cyclic torque, N-m
Tc = 3.0E-4;

% One-quarter orbit period, sec
dt = 0.25*6000;

% Allowable cyclic pointing offset, rad
alpha = 1.0*pi/180;

% Required Spin Rate, rad/sec
Om = Tc*dt/(I3*alpha)

% Expected magnitude of secular torque, N-m
Ts = 1.0E-3;

% Find roots for given Kn, Kp
R=zeros(100,8);
Cost = zeros(10,10);
for i=1:10,
   Kp = 0.125+0.0005*(i-1);
   for j=1:10,
      Kn = 12.5+0.05*(j-1);

      CE = [1 2*(Kn/It+Kp) K1^2*Om^2+Om^2+(Kn/It+Kp)^2 2*(Kn/It+K1^2*Kp)*Om^2 (Kn/It-K1*Kp)^2*Om^2+K1^2*Om^4];

      r=roots(CE);
      R(10*(i-1)+j,1:2:7) = real(r)';
      R(10*(i-1)+j,2:2:8) = imag(r)';

      Cost(i,j)=max(real(r));

   end
end

Cost = Cost';

save -ascii RootLocus R
save -ascii Cost Cost

%m=abs(r)
%d=sin(arg(r)-pi/2)
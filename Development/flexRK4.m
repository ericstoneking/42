% Flex experiment:  Does the RK4 stepsize have to be small when 
% high-frequency flex modes are present, or can a stepwise 
% closed-form solution be used?

% Parameters
J = 100;
wc = 0.005*2*pi;
zc = 0.7;
Kp = 0; % J*wc*wc;
Kr = 0; % J*2*zc*wc;

mu = 0.05;
alpha = 1/(1-mu);
wn = 0.035*2*pi;
zf = 0.01;
phi = sqrt(alpha-1)/sqrt(J);
sigma = zf*wn;
wf = sqrt(wn^2-sigma^2);

% Method 0:  Closed-form solution from initial eta deflection 
Nt = 1000;
dt = 10.0;

% u = [thdot etadot th eta]
u = zeros(4,Nt);

t0 = [0:Nt-1]*dt;

u(4,1) = 1.0;
psi = atan(sigma/wf);
C = u(4,1)/cos(psi);
for k=1:Nt,
   ang = wf*t0(k)-psi;
   u(4,k) = C*exp(-sigma*t0(k))*cos(ang);
   u(2,k) = C*exp(-sigma*t0(k))*(-sigma*cos(ang)-wf*sin(ang));
   u(1,k) = phi*u(2,k);
   u(3,k) = phi*u(4,k);
end
u0 = u(:,1:Nt);

% Method 5:  Stepwise closed-form solution 
Nt = 1000;
dt = 10.0;

% u = [thdot etadot th eta]
u = zeros(4,Nt);

t5 = [0:Nt-1]*dt;

u(4,1) = 1.0;
for k=1:Nt,
   psi = atan((sigma+u(2,k)/u(4,k))/wf);
   C = u(4,k)/cos(psi);
   ang = wf*dt-psi;
   u(4,k+1) = C*exp(-sigma*dt)*cos(ang);
   u(2,k+1) = C*exp(-sigma*dt)*(-sigma*cos(ang)-wf*sin(ang));
end
u(1,:) = phi*u(2,:);
u(3,:) = phi*u(4,:);
u5 = u(:,1:Nt);

% Method 1:  Tiny time steps, integrate flex
Nt = 5000;
dt = 2.0;

% u = [thdot etadot th eta]
u = zeros(4,Nt);

t1 = [0:Nt-1]*dt;

u(4,1) = 1.0;
u(3,1) = phi*u(4,1);

for k=1:Nt,
   T = -Kr*u(1,k)-Kp*u(3,k);

   % First RK call
   etadotdot = phi*T - 2*sigma*u(2,k) - wn^2*u(4,k);
   thdotdot = T/J + phi*etadotdot;
   m1 = [thdotdot etadotdot u(1,k) u(2,k)]';

   % Second RK call
   uu = u(:,k) + 0.5*m1*dt;
   etadotdot = phi*T - 2*sigma*uu(2) - wn^2*uu(4);
   thdotdot = T/J + phi*etadotdot;
   m2 = [thdotdot etadotdot uu(1) uu(2)]';

   % Third RK call
   uu = u(:,k) + 0.5*m2*dt;
   etadotdot = phi*T - 2*sigma*uu(2) - wn^2*uu(4);
   thdotdot = T/J + phi*etadotdot;
   m3 = [thdotdot etadotdot uu(1) uu(2)]';

   % Fourth RK call
   uu = u(:,k) + m3*dt;
   etadotdot = phi*T - 2*sigma*uu(2) - wn^2*uu(4);
   thdotdot = T/J  + phi*etadotdot;
   m4 = [thdotdot etadotdot uu(1) uu(2)]';

   % Propagate state
   u(:,k+1) = u(:,k) + (m1+2*(m2+m3)+m4)/6*dt;
   
end
u1 = u(:,1:Nt);

% Method 2:  Large time steps, integrate flex (expect lousy)
Nt = 2000;
dt = 5.0;

% u = [thdot etadot th eta]
u = zeros(4,Nt);

t2 = [0:Nt-1]*dt;

u(4,1) = 1.0;
u(3,1) = phi*u(4,1);

for k=1:Nt,
   T = -Kr*u(1,k)-Kp*u(3,k);

   % First RK call
   etadotdot = phi*T - 2*sigma*u(2,k) - wn^2*u(4,k);
   thdotdot = T/J + phi*etadotdot;
   m1 = [thdotdot etadotdot u(1,k) u(2,k)]';

   % Second RK call
   uu = u(:,k) + 0.5*m1*dt;
   etadotdot = phi*T - 2*sigma*uu(2) - wn^2*uu(4);
   thdotdot = T/J + phi*etadotdot;
   m2 = [thdotdot etadotdot uu(1) uu(2)]';

   % Third RK call
   uu = u(:,k) + 0.5*m2*dt;
   etadotdot = phi*T - 2*sigma*uu(2) - wn^2*uu(4);
   thdotdot = T/J + phi*etadotdot;
   m3 = [thdotdot etadotdot uu(1) uu(2)]';

   % Fourth RK call
   uu = u(:,k) + m3*dt;
   etadotdot = phi*T - 2*sigma*uu(2) - wn^2*uu(4);
   thdotdot = T/J  + phi*etadotdot;
   m4 = [thdotdot etadotdot uu(1) uu(2)]';

   % Propagate state
   u(:,k+1) = u(:,k) + (m1+2*(m2+m3)+m4)/6*dt;
   
end
u2 = u(:,1:Nt);


% Method 3:  Large time steps, instantaneous etadotdot
Nt = 2000;
dt = 5.0;

% u = [thdot etadot th eta]
u = zeros(4,Nt);

t3 = [0:Nt-1]*dt;

u(4,1) = 1.0;
u(3,1) = phi*u(4,1);

for k=1:Nt,
   T = -Kr*u(1,k)-Kp*u(3,k);
   N = phi*T;

   if abs(N/wn^2-u(4,k)) < 1.0e-8,
      psi = 0.5*pi;
      C = u(2,k)/wf;
   else,
      psi = atan(  (sigma-u(2,k)/ (N/wn^2-u(4,k)) ) /wf   );
      C = (u(4,k)-N/wn^2)/cos(psi);
   end

   % First RK call
   ang = -psi;
   etadotdot = C*((sigma^2-wf^2)*cos(ang) + 2*sigma*wf*sin(ang));
   thdotdot = T/J + phi*etadotdot;
   m1 = [thdotdot etadotdot u(1,k) u(2,k)]';

   % Second RK call
   uu = u(:,k) + 0.5*m1*dt;
   ang = 0.5*wf*dt-psi;
   etadotdot = C*exp(-sigma*0.5*dt)*((sigma^2-wf^2)*cos(ang) + 2*sigma*wf*sin(ang));
   thdotdot = T/J + phi*etadotdot;
   m2 = [thdotdot etadotdot uu(1) uu(2)]';

   % Third RK call
   uu = u(:,k) + 0.5*m2*dt;
   ang = 0.5*wf*dt-psi;
   etadotdot = C*exp(-sigma*0.5*dt)*((sigma^2-wf^2)*cos(ang) + 2*sigma*wf*sin(ang));
   thdotdot = T/J + phi*etadotdot;
   m3 = [thdotdot etadotdot uu(1) uu(2)]';

   % Fourth RK call
   uu = u(:,k) + m3*dt;
   ang = wf*dt-psi;
   etadotdot = C*exp(-sigma*dt)*((sigma^2-wf^2)*cos(ang) + 2*sigma*wf*sin(ang));
   thdotdot = T/J  + phi*etadotdot;
   m4 = [thdotdot etadotdot uu(1) uu(2)]';

   % Propagate state
   u(1,k+1) = u(1,k) + (m1(1)+2*(m2(1)+m3(1))+m4(1))/6*dt;
   u(3,k+1) = u(3,k) + (m1(3)+2*(m2(3)+m3(3))+m4(3))/6*dt;
   ang = wf*dt-psi;
   u(2,k+1) = C*exp(-sigma*dt)*(-sigma*cos(ang)-wf*sin(ang));
   u(4,k+1) = N/wn^2 + C*exp(-sigma*dt)*cos(ang);
   
end
u3 = u(:,1:Nt);


% Method 4:  Large time steps, time-averaged etadotdot
Nt = 2000;
dt = 5.0;

% u = [thdot etadot th eta]
u = zeros(4,Nt);

t4 = [0:Nt-1]*dt;

u(4,1) = 1.0;
u(3,1) = phi*u(4,1);

for k=1:Nt,
   T = -Kr*u(1,k)-Kp*u(3,k);
   N = phi*T;

   if abs(N/wn^2-u(4,k)) < 1.0e-6,
      psi = 0.5*pi;
      C = u(2,k)/wf;
   else,
      psi = atan(  (sigma-u(2,k)/ (N/wn^2-u(4,k)) ) /wf   );
      C = (u(4,k)-N/wn^2)/cos(psi);
   end

   % First RK call
   ang = wf*(-0.25*dt)-psi;
   etadot1 = C*exp(-sigma*(-0.25*dt))*(-sigma*cos(ang)-wf*sin(ang));
   ang = wf*(0.25*dt)-psi;
   etadot2 = C*exp(-sigma*(0.25*dt))*(-sigma*cos(ang)-wf*sin(ang));
   etadotdot = (etadot2-etadot1)/(0.5*dt);
   %ang = -psi;
   %etadotdot = C*((sigma^2-wf^2)*cos(ang) + 2*sigma*wf*sin(ang));
   thdotdot = T/J + phi*etadotdot;
   m1 = [thdotdot etadotdot u(1,k) u(2,k)]';

   % Second RK call
   uu = u(:,k) + 0.5*m1*dt;
   ang = wf*(0.25*dt)-psi;
   etadot1 = C*exp(-sigma*(0.25*dt))*(-sigma*cos(ang)-wf*sin(ang));
   ang = wf*(0.75*dt)-psi;
   etadot2 = C*exp(-sigma*(0.75*dt))*(-sigma*cos(ang)-wf*sin(ang));
   etadotdot = (etadot2-etadot1)/(0.5*dt);
   %ang = 0.5*wf*dt-psi;
   %etadotdot = C*exp(-sigma*0.5*dt)*((sigma^2-wf^2)*cos(ang) + 2*sigma*wf*sin(ang));
   thdotdot = T/J + phi*etadotdot;
   m2 = [thdotdot etadotdot uu(1) uu(2)]';

   % Third RK call
   uu = u(:,k) + 0.5*m2*dt;
   ang = wf*(0.25*dt)-psi;
   etadot1 = C*exp(-sigma*(0.25*dt))*(-sigma*cos(ang)-wf*sin(ang));
   ang = wf*(0.75*dt)-psi;
   etadot2 = C*exp(-sigma*(0.75*dt))*(-sigma*cos(ang)-wf*sin(ang));
   etadotdot = (etadot2-etadot1)/(0.5*dt);
   %ang = 0.5*wf*dt-psi;
   %etadotdot = C*exp(-sigma*0.5*dt)*((sigma^2-wf^2)*cos(ang) + 2*sigma*wf*sin(ang));
   thdotdot = T/J + phi*etadotdot;
   m3 = [thdotdot etadotdot uu(1) uu(2)]';

   % Fourth RK call
   uu = u(:,k) + m3*dt;
   ang = wf*(0.75*dt)-psi;
   etadot1 = C*exp(-sigma*(0.75*dt))*(-sigma*cos(ang)-wf*sin(ang));
   ang = wf*(1.25*dt)-psi;
   etadot2 = C*exp(-sigma*(1.25*dt))*(-sigma*cos(ang)-wf*sin(ang));
   etadotdot = (etadot2-etadot1)/(0.5*dt);
   %ang = wf*dt-psi;
   %etadotdot = C*exp(-sigma*dt)*((sigma^2-wf^2)*cos(ang) + 2*sigma*wf*sin(ang));
   thdotdot = T/J  + phi*etadotdot;
   m4 = [thdotdot etadotdot uu(1) uu(2)]';

   % Propagate state
   u(:,k+1) = u(:,k) + (m1+2*(m2+m3)+m4)/6*dt;
   ang = wf*dt-psi;
   u(2,k+1) = C*exp(-sigma*dt)*(-sigma*cos(ang)-wf*sin(ang));
   u(4,k+1) = N/wn^2 + C*exp(-sigma*dt)*cos(ang);
   
end
u4 = u(:,1:Nt);

% Method 6:  Large time steps, average etadotdot over whole timestep
Nt = 5000;
dt = 2.0;

% u = [thdot etadot th eta]
u = zeros(4,Nt);

t6 = [0:Nt-1]*dt;

u(4,1) = 1.0;
u(3,1) = phi*u(4,1);

for k=1:Nt,
   T = -Kr*u(1,k)-Kp*u(3,k);
   N = phi*T;

   if abs(N/wn^2-u(4,k)) < 1.0e-6,
      psi = 0.5*pi;
      C = u(2,k)/wf;
   else,
      psi = atan(  (sigma-u(2,k)/ (N/wn^2-u(4,k)) ) /wf   );
      C = (u(4,k)-N/wn^2)/cos(psi);
   end
   ang = -psi;
   etadotdot = C*((sigma^2-wf^2)*cos(ang) + 2*sigma*wf*sin(ang));

   % First RK call
   thdotdot = T/J + phi*etadotdot;
   m1 = [thdotdot etadotdot u(1,k) u(2,k)]';

   % Second RK call
   uu = u(:,k) + 0.5*m1*dt;
   thdotdot = T/J + phi*etadotdot;
   m2 = [thdotdot etadotdot uu(1) uu(2)]';

   % Third RK call
   uu = u(:,k) + 0.5*m2*dt;
   thdotdot = T/J + phi*etadotdot;
   m3 = [thdotdot etadotdot uu(1) uu(2)]';

   % Fourth RK call
   uu = u(:,k) + m3*dt;
   thdotdot = T/J  + phi*etadotdot;
   m4 = [thdotdot etadotdot uu(1) uu(2)]';

   % Propagate state
   u(:,k+1) = u(:,k) + (m1+2*(m2+m3)+m4)/6*dt;
   u(2,k+1) = C*exp(-sigma*dt)*(-sigma*cos(ang)-wf*sin(ang));
   u(4,k+1) = N/wn^2 + C*exp(-sigma*dt)*cos(ang);
   
end
u6 = u(:,1:Nt);


% Compare

figure(1)
%plot(t0,u0(3,:),t1,u1(3,:),t2,u2(3,:),t3,u3(3,:),t4,u4(3,:),t5,u5(3,:),t6,u6(3,:))
plot(t0,u0(3,:),t1,u1(3,:),t2,u2(3,:),t3,u3(3,:))
axis([0 10000 -0.03 0.03])
%legend("0","1","2","3","4","5","6")
legend("0","1","2","3")


figure(2)
plot(t0,u0(1,:),t1,u1(1,:),t2,u2(1,:),t3,u3(1,:),t4,u4(1,:),t5,u5(1,:),t6,u6(1,:))
axis([0 10000 -0.006 0.006])

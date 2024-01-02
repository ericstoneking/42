m = 100;

wc = 0.1*2*pi;
zc = 0.707;
Kx = m*wc^2;
Kv = 2.0*m*zc*wc;

T = 0.25; % Sample interval

f = logspace(-3,2.5,5000);

LoopGainFactor = 10^(0/20);

Nfig = 0;

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%  Open-loop frequency response


N = max(size(f));
MagRigid = zeros(N,1);
AngRigid = zeros(N,1);
MagFlex = zeros(N,1);
AngFlex = zeros(N,1);
MagZOH = zeros(N,1);
AngZOH = zeros(N,1);
MagDelay1 = zeros(N,1);
AngDelay1 = zeros(N,1);
MagDelay2 = zeros(N,1);
AngDelay2 = zeros(N,1);
Kcross = 1;
for k=1:N,
   w = 2*pi*f(k);
   s = 1i*w;
   z = exp(s*T);
   ZOH = (1-1/z)/(s*T);
   RigidBody = 1/(m*s^2);
   PD = (Kv*s+Kx);

   TF = PD*RigidBody;
   MagRigid(k) = abs(TF);
   AngRigid(k) = angle(TF);

   TF = ZOH*PD*RigidBody;
   MagZOH(k) = abs(TF);
   AngZOH(k) = angle(TF);

   TF = ZOH*z^(-1)*PD*RigidBody;
   MagDelay1(k) = abs(TF);
   AngDelay1(k) = angle(TF);

   TF = LoopGainFactor*ZOH*z^(-2)*PD*RigidBody;
   MagDelay2(k) = abs(TF);
   AngDelay2(k) = angle(TF);

   if (k > 1),
      if MagDelay1(k) < 1.0 && MagDelay1(k-1) > 1.0,
         Kcross = k;
      end
   end
end

a = [0:pi/100:2*pi]';
KOZx = -180 + 30*cos(a);
KOZy = 6*sin(a);

CritDelay = -180./f;

% Rigid 

dB = 20*log10(MagRigid);
ang = unwrap(AngRigid);
ang = ang*180/pi;

Nfig = Nfig + 1;
figure(Nfig)
plot(ang,dB,'b-',-180,0,'r+',KOZx,KOZy,'r-','LineWidth',2)
set(gca,'FontWeight','Bold','FontSize',18)
title('Nichols Plot: Rigid')
xlabel('Phase, deg')
ylabel('Gain, dB')
axis([-300 60 -80 80])
grid
%print(gcf,'NicholsRigid.jpg','-djpeg')

Nfig = Nfig + 1;
figure(Nfig)
semilogx(f,dB,'LineWidth',2)
set(gca,'FontWeight','Bold','FontSize',18)
title('Rigid Open-loop Frequency Response: Gain')
xlabel('Frequency, Hz')
ylabel('Gain, dB')
grid
%print(gcf,'BodeGainRigid.jpg','-djpeg')

Nfig = Nfig + 1;
figure(Nfig)
semilogx(f,ang,'LineWidth',2)
set(gca,'FontWeight','Bold','FontSize',18)
title('Rigid Open-loop Frequency Response: Phase')
xlabel('Frequency, Hz')
ylabel('Phase, deg')
grid
%print(gcf,'BodePhaseRigid.jpg','-djpeg')

% ZOH

dB = 20*log10(MagZOH);
ang = unwrap(AngZOH);
ang = ang*180/pi;

Nfig = Nfig + 1;
figure(Nfig)
plot(ang,dB,'b-',-180,0,'r+',KOZx,KOZy,'r-','LineWidth',2)
set(gca,'FontWeight','Bold','FontSize',18)
title('Nichols Plot: Zero-Order Hold')
xlabel('Phase, deg')
ylabel('Gain, dB')
axis([-300 60 -80 80])
grid
%print(gcf,'NicholsZOH.jpg','-djpeg')


% One Delay

dB = 20*log10(MagDelay1);
ang = unwrap(AngDelay1);
ang = ang*180/pi;

Nfig = Nfig + 1;
figure(Nfig)
plot(ang,dB,'b-',-180,0,'r+',KOZx,KOZy,'r-','LineWidth',2)
set(gca,'FontWeight','Bold','FontSize',18)
title('Nichols Plot: One Cycle Delay')
xlabel('Phase, deg')
ylabel('Gain, dB')
axis([-300 60 -80 80])
grid
%print(gcf,'NicholsDelay1.jpg','-djpeg')

% One Delay, showing Delay Margin

Nfig = Nfig + 1;
figure(Nfig)
plot((ang+180)./(360*f'),dB,'color',[0 0.5 0],'LineWidth',2)
set(gca,'FontWeight','Bold','FontSize',18)
title('One Cycle Delay, Delay Margin')
xlabel('Delay Margin, sec')
ylabel('Gain, dB')
%axis([-300 60 -80 80])
grid
%print(gcf,'NicholsDelay1.jpg','-djpeg')

% One Delay, showing Delay

Nfig = Nfig + 1;
figure(Nfig)
semilogx(f,(ang+180)./(360*f'),'color',[0 0.5 0],'LineWidth',2)
set(gca,'FontWeight','Bold','FontSize',18)
title('One Cycle Delay, Delay Margin')
xlabel('Frequency, Hz')
ylabel('Delay Margin, sec')
%axis([-300 60 -80 80])
grid
%print(gcf,'NicholsDelay1.jpg','-djpeg')

% One Delay, Bode Gain

Nfig = Nfig + 1;
figure(Nfig)
semilogx(f,dB,'color',[0 0.5 0],'LineWidth',2)
set(gca,'FontWeight','Bold','FontSize',18)
title('One Cycle Delay, Gain')
xlabel('Frequency, Hz')
ylabel('Gain, dB')
%axis([-300 60 -80 80])
grid


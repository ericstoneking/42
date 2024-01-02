load FreqResp00.42
f = FreqResp00(:,1);
dB = FreqResp00(:,2:4);
Phase = FreqResp00(:,5:7);
clear FreqResp00;
N = length(f);

wc = 0.1*2*pi;
zc = 0.7;
Kr = 2.0*zc*wc;
Kp = wc*wc;
j = sqrt(-1);
dBb = zeros(N,1);
Phaseb = zeros(N,1);
for(k=1:N),
   s = 2*pi*j*f(k);
   TF = (Kr*s+Kp)/s^2;
   %TF = TF/(1+TF);
   Mag = abs(TF);
   dBb(k) = 20.0*log10(Mag);
   Phaseb(k) = angle(TF)*180/pi;
end

for i=1:3,
   figure(i)
   semilogx(f,[dBb dB(:,i)],'LineWidth',3)
   grid
   
   figure(3+i)
   semilogx(f,[Phaseb Phase(:,i)],'LineWidth',3)
   
   figure(6+i)
   plot(Phaseb,dBb,Phase(:,i),dB(:,i),'LineWidth',3)
end



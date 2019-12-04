%Body/Joint   RotDOF   TrnDOF     u[] 
%  B(1)        wn       ---     01-03
%  G(1)         2        1      04-06
%  G(2)         1        1      07-08
%  G(3)         3        0      09-11
%  G(4)         3        0      12-14
%  B(1)       ---       vn      15-17

clear

Iw = 1:3;
Iv = 15:17;
I1 = 4:6;
I2 = 7:8;
I3 = 9:11;
I4 = 12:14;

M = load('COEF.42');
F = load('RHS.42');

% Shuffle to see triangularization more clearly
%M = [M(Iv,:); M(Iw,:); M(I1,:); M(I2,:); M(I3,:); M(I4,:)];
%M = [M(:,Iv) M(:,Iw) M(:,I1) M(:,I2) M(:,I3) M(:,I4)];
%F = [F(Iv);F(Iw);F(I1);F(I2);F(I3);F(I4)];

%Iv = 1:3;
%Iw = 4:6;
%I1 = 7:9;
%I2 = 10:11;
%I3 = 12:14;
%I4 = 15:17;

UDOT = M\F;

% Triangularization

   % G(4)
   M44i = inv(M(I4,I4));
   K = M(I2,I4)*M44i;
   M(I2,:) = M(I2,:) - K*M(I4,:);
   F(I2)   = F(I2)   - K*F(I4);
   K = M(Iw,I4)*M44i;
   M(Iw,:) = M(Iw,:) - K*M(I4,:);
   F(Iw)   = F(Iw)   - K*F(I4);
   K = M(Iv,I4)*M44i;
   M(Iv,:) = M(Iv,:) - K*M(I4,:);
   F(Iv)   = F(Iv)   - K*F(I4);

   % G(3)
   M33i = inv(M(I3,I3));
   K = M(I2,I3)*M33i;
   M(I2,:) = M(I2,:) - K*M(I3,:);
   F(I2)   = F(I2)   - K*F(I3);
   K = M(Iw,I3)*M33i;
   M(Iw,:) = M(Iw,:) - K*M(I3,:);
   F(Iw)   = F(Iw)   - K*F(I3);
   K = M(Iv,I3)*M33i;
   M(Iv,:) = M(Iv,:) - K*M(I3,:);
   F(Iv)   = F(Iv)   - K*F(I3);

   % G(2)
   M22i = inv(M(I2,I2));
   K = M(Iw,I2)*M22i;
   M(Iw,:) = M(Iw,:) - K*M(I2,:);
   F(Iw)   = F(Iw)   - K*F(I2);
   K = M(Iv,I2)*M22i;
   M(Iv,:) = M(Iv,:) - K*M(I2,:);
   F(Iv)   = F(Iv)   - K*F(I2);

   % G(1)
   M11i = inv(M(I1,I1));
   K = M(Iw,I1)*M11i;
   M(Iw,:) = M(Iw,:) - K*M(I1,:);
   F(Iw)   = F(Iw)   - K*F(I1);
   K = M(Iv,I1)*M11i;
   M(Iv,:) = M(Iv,:) - K*M(I1,:);
   F(Iv)   = F(Iv)   - K*F(I1);

   % w
   Mwwi = inv(M(Iw,Iw));
   K = M(Iv,Iw)*Mwwi;
   M(Iv,:) = M(Iv,:) - K*M(Iw,:);
   F(Iv)  =  F(Iv)   - K*F(Iw);

   M = 1.0E-6*round(1.0E6*M);
   F = 1.0E-6*round(1.0E6*F);

% Substitution

   udot = zeros(17,1);
   udot(Iv) = inv(M(Iv,Iv))*F(Iv);

   udot(Iw) = inv(M(Iw,Iw))*(F(Iw)-M(Iw,Iv)*udot(Iv));
   
   udot(I1) = inv(M(I1,I1))*(F(I1)-M(I1,Iv)*udot(Iv)-M(I1,Iw)*udot(Iw));
   
   udot(I2) = inv(M(I2,I2))*(F(I2)-M(I2,Iv)*udot(Iv)-M(I2,Iw)*udot(Iw));
   
   udot(I3) = inv(M(I3,I3))*(F(I3)-M(I3,Iv)*udot(Iv)-M(I3,Iw)*udot(Iw)-M(I3,I2)*udot(I2));
   
   udot(I4) = inv(M(I4,I4))*(F(I4)-M(I4,Iv)*udot(Iv)-M(I4,Iw)*udot(Iw)-M(I4,I2)*udot(I2));
   
   
   




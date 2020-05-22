%Body/Joint   RotDOF    u() 
%  B(1)        wn      01-03
%  G(1)         3      04-06
%  G(2)         3      07-09
%  G(3)         3      10-12
%  G(4)         3      13-15
%  B(1)       ---      16-18


Iw = 1:3;
Iv = 16:18;
I1 = 4:6;
I2 = 7:9;
I3 = 10:12;
I4 = 13:15;

M = load('COEF.42');
F = load('RHS.42');

UDOT = M\F;

Nanc = [0 1 2 3]';
Anc = [-1 -1 -1;
        1 -1 -1;
        1  2 -1;
        1  2  3];
Nu = [3 3 3 3]';
u0 = [3 6 9 12]';

% Triangularization

if 1,
   for ig = 4:-1:1,
      Ig = u0(ig)+1:u0(ig)+Nu(ig);
      Mi = inv(M(Ig,Ig));
      for ia = 1:Nanc(ig),
         j = Anc(ig,ia);
         Ia = u0(j)+1:u0(j)+Nu(j);
         
         K = M(Ia,Ig)*Mi;
         for ja = 1:Nanc(ig),
            Ja = u0(ja)+1:u0(ja)+Nu(ja);
            M(Ia,Ja) = M(Ia,Ja) - K*M(Ig,Ja);
         end
         M(Ia,Iw) = M(Ia,Iw) - K*M(Ig,Iw);
         M(Ia,Iv) = M(Ia,Iv) - K*M(Ig,Iv);
         F(Ia) = F(Ia) - K*F(Ig);
         
      end
      
      K = M(Iw,Ig)*Mi;
      for ja = 1:Nanc(ig),
         Ja = u0(ja)+1:u0(ja)+Nu(ja);
         M(Iw,Ja) = M(Iw,Ja) - K*M(Ig,Ja);
      end
      M(Iw,Iw) = M(Iw,Iw) - K*M(Ig,Iw);
      M(Iw,Iv) = M(Iw,Iv) - K*M(Ig,Iv);
      F(Iw) = F(Iw) - K*F(Ig);
      
      K = M(Iv,Ig)*Mi;
      for ja = 1:Nanc(ig),
         Ja = u0(ja)+1:u0(ja)+Nu(ja);
         M(Iv,Ja) = M(Iv,Ja) - K*M(Ig,Ja);
      end
      M(Iv,Iw) = M(Iv,Iw) - K*M(Ig,Iw);
      M(Iv,Iv) = M(Iv,Iv) - K*M(Ig,Iv);
      F(Iv) = F(Iv) - K*F(Ig);
   end

else,
   % G(4)
   M44i = inv(M(I4,I4));
   K = M(I3,I4)*M44i;
   M(I3,I3) = M(I3,I3) - K*M(I4,I3);
   M(I3,I2) = M(I3,I2) - K*M(I4,I2);
   M(I3,I1) = M(I3,I1) - K*M(I4,I1);
   M(I3,Iw) = M(I3,Iw) - K*M(I4,Iw);
   M(I3,Iv) = M(I3,Iv) - K*M(I4,Iv);
   F(I3)    = F(I3)    - K*F(I4);
   K = M(I2,I4)*M44i;
   M(I2,I3) = M(I2,I3) - K*M(I4,I3);
   M(I2,I2) = M(I2,I2) - K*M(I4,I2);
   M(I2,I1) = M(I2,I1) - K*M(I4,I1);
   M(I2,Iw) = M(I2,Iw) - K*M(I4,Iw);
   M(I2,Iv) = M(I2,Iv) - K*M(I4,Iv);
   F(I2)    = F(I2)    - K*F(I4);
   K = M(I1,I4)*M44i;
   M(I1,I3) = M(I1,I3) - K*M(I4,I3);
   M(I1,I2) = M(I1,I2) - K*M(I4,I2);
   M(I1,I1) = M(I1,I1) - K*M(I4,I1);
   M(I1,Iw) = M(I1,Iw) - K*M(I4,Iw);
   M(I1,Iv) = M(I1,Iv) - K*M(I4,Iv);
   F(I1)    = F(I1)    - K*F(I4);
   K = M(Iw,I4)*M44i;
   M(Iw,I3) = M(Iw,I3) - K*M(I4,I3);
   M(Iw,I2) = M(Iw,I2) - K*M(I4,I2);
   M(Iw,I1) = M(Iw,I1) - K*M(I4,I1);
   M(Iw,Iw) = M(Iw,Iw) - K*M(I4,Iw);
   M(Iw,Iv) = M(Iw,Iv) - K*M(I4,Iv);
   F(Iw)    = F(Iw)    - K*F(I4);
   K = M(Iv,I4)*M44i;
   M(Iv,I3) = M(Iv,I3) - K*M(I4,I3);
   M(Iv,I2) = M(Iv,I2) - K*M(I4,I2);
   M(Iv,I1) = M(Iv,I1) - K*M(I4,I1);
   M(Iv,Iw) = M(Iv,Iw) - K*M(I4,Iw);
   M(Iv,Iv) = M(Iv,Iv) - K*M(I4,Iv);
   F(Iv)    = F(Iv)    - K*F(I4);
   M4a = M;
   F4a = F;

   % G(3)
   M33i = inv(M(I3,I3));
   K = M(I2,I3)*M33i;
   M(I2,I2) = M(I2,I2) - K*M(I3,I2);
   M(I2,I1) = M(I2,I1) - K*M(I3,I1);
   M(I2,Iw) = M(I2,Iw) - K*M(I3,Iw);
   M(I2,Iv) = M(I2,Iv) - K*M(I3,Iv);
   F(I2)    = F(I2)    - K*F(I3);
   K = M(I1,I3)*M33i;
   M(I1,I2) = M(I1,I2) - K*M(I3,I2);
   M(I1,I1) = M(I1,I1) - K*M(I3,I1);
   M(I1,Iw) = M(I1,Iw) - K*M(I3,Iw);
   M(I1,Iv) = M(I1,Iv) - K*M(I3,Iv);
   F(I1)    = F(I1)    - K*F(I3);
   K = M(Iw,I3)*M33i;
   M(Iw,I2) = M(Iw,I2) - K*M(I3,I2);
   M(Iw,I1) = M(Iw,I1) - K*M(I3,I1);
   M(Iw,Iw) = M(Iw,Iw) - K*M(I3,Iw);
   M(Iw,Iv) = M(Iw,Iv) - K*M(I3,Iv);
   F(Iw)    = F(Iw)    - K*F(I3);
   K = M(Iv,I3)*M33i;
   M(Iv,I2) = M(Iv,I2) - K*M(I3,I2);
   M(Iv,I1) = M(Iv,I1) - K*M(I3,I1);
   M(Iv,Iw) = M(Iv,Iw) - K*M(I3,Iw);
   M(Iv,Iv) = M(Iv,Iv) - K*M(I3,Iv);
   F(Iv)    = F(Iv)    - K*F(I3);
   M3a = M;
   F3a = F;

   % G(2)
   M22i = inv(M(I2,I2));
   K = M(I1,I2)*M22i;
   M(I1,I1) = M(I1,I1) - K*M(I2,I1);
   M(I1,Iw) = M(I1,Iw) - K*M(I2,Iw);
   M(I1,Iv) = M(I1,Iv) - K*M(I2,Iv);
   F(I1)    = F(I1)    - K*F(I2);
   K = M(Iw,I2)*M22i;
   M(Iw,I1) = M(Iw,I1) - K*M(I2,I1);
   M(Iw,Iw) = M(Iw,Iw) - K*M(I2,Iw);
   M(Iw,Iv) = M(Iw,Iv) - K*M(I2,Iv);
   F(Iw)    = F(Iw)    - K*F(I2);
   K = M(Iv,I2)*M22i;
   M(Iv,I1) = M(Iv,I1) - K*M(I2,I1);
   M(Iv,Iw) = M(Iv,Iw) - K*M(I2,Iw);
   M(Iv,Iv) = M(Iv,Iv) - K*M(I2,Iv);
   F(Iv)    = F(Iv)    - K*F(I2);
   M2a = M;
   F2a = F;

   % G(1)
   M11i = inv(M(I1,I1));
   K = M(Iw,I1)*M11i;
   M(Iw,Iw) = M(Iw,Iw) - K*M(I1,Iw);
   M(Iw,Iv) = M(Iw,Iv) - K*M(I1,Iv);
   F(Iw)    = F(Iw)    - K*F(I1);
   K = M(Iv,I1)*M11i;
   M(Iv,Iw) = M(Iv,Iw) - K*M(I1,Iw);
   M(Iv,Iv) = M(Iv,Iv) - K*M(I1,Iv);
   F(Iv)    = F(Iv)    - K*F(I1);
   M1a = M;
   F1a = F;
end

   % w
   Mwwi = inv(M(Iw,Iw));
   K = M(Iv,Iw)*Mwwi;
   M(Iv,Iv) = M(Iv,Iv) - K*M(Iw,Iv);
   F(Iv)    = F(Iv)    - K*F(Iw);
   Mwa = M;
   Fwa = F;

   %M = 1.0E-6*round(1.0E6*M);
   %F = 1.0E-6*round(1.0E6*F);

% Substitution

   udot = zeros(18,1);
   udot(Iv) = inv(M(Iv,Iv))*F(Iv);

   udot(Iw) = inv(M(Iw,Iw))*(F(Iw)-M(Iw,Iv)*udot(Iv));
   
   udot(I1) = inv(M(I1,I1))*(F(I1)-M(I1,Iv)*udot(Iv)-M(I1,Iw)*udot(Iw));
   
   udot(I2) = inv(M(I2,I2))*(F(I2)-M(I2,Iv)*udot(Iv)-M(I2,Iw)*udot(Iw)-M(I2,I1)*udot(I1));
   
   udot(I3) = inv(M(I3,I3))*(F(I3)-M(I3,Iv)*udot(Iv)-M(I3,Iw)*udot(Iw)-M(I3,I1)*udot(I1)-M(I3,I2)*udot(I2));
   
   udot(I4) = inv(M(I4,I4))*(F(I4)-M(I4,Iv)*udot(Iv)-M(I4,Iw)*udot(Iw)-M(I4,I1)*udot(I1)-M(I4,I2)*udot(I2)-M(I4,I3)*udot(I3));
   
   
   




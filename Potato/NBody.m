%Body/Joint   RotDOF   TrnDOF     u[] 
%  B(1)        wn       ---     01-03
%  G(1)         2        1      04-06
%  G(2)         1        1      07-08
%  G(3)         3        0      09-11
%  G(4)         3        0      12-14
%  B(1)       ---       vn      15-17


Iw = 1:3;
Iv = 15:17;
I1 = 4:6;
I2 = 7:8;
I3 = 9:11;
I4 = 12:14;

M = load('COEF.42');
F = load('RHS.42');

UDOT = M\F;

Nanc = [0 0 1 1]';
Anc = [-1 -1 2 2]';
Nu = [3 2 3 3]';
u0 = [3 6 8 11]';

% Triangularization

if 1,
   for Ig = 4:-1:1,
      i = u0(Ig)+1:u0(Ig)+Nu(Ig);
      Mi = inv(M(i,i));
      if (Nanc(Ig) > 0),
         for Ia = 1:Nanc(Ig),
            Jg = Anc(Ig,Ia);
            j = u0(Jg)+1:u0(Jg)+Nu(Jg);
            
            K = M(j,i)*Mi;
            M(j,j) = M(j,j) - K*M(i,j);
            M(j,Iw) = M(j,Iw) - K*M(i,Iw);
            M(j,Iv) = M(j,Iv) - K*M(i,Iv);
            F(j) = F(j) - K*F(i);
            
            K = M(Iw,i)*Mi;
            M(Iw,j) = M(Iw,j) - K*M(i,j);
            M(Iw,Iw) = M(Iw,Iw) - K*M(i,Iw);
            M(Iw,Iv) = M(Iw,Iv) - K*M(i,Iv);
            F(Iw) = F(Iw) - K*F(i);
            
            K = M(Iv,i)*Mi;
            M(Iv,j) = M(Iv,j) - K*M(i,j);
            M(Iv,Iw) = M(Iv,Iw) - K*M(i,Iw);
            M(Iv,Iv) = M(Iv,Iv) - K*M(i,Iv);
            F(Iv) = F(Iv) - K*F(i);
         end
      else,
         K = M(Iw,i)*Mi;
         M(Iw,Iw) = M(Iw,Iw) - K*M(i,Iw);
         M(Iw,Iv) = M(Iw,Iv) - K*M(i,Iv);
         F(Iw) = F(Iw) - K*F(i);
         
         K = M(Iv,i)*Mi;
         M(Iv,Iw) = M(Iv,Iw) - K*M(i,Iw);
         M(Iv,Iv) = M(Iv,Iv) - K*M(i,Iv);
         F(Iv) = F(Iv) - K*F(i);
      end
   end

else,
   % G(4)
   M44i = inv(M(I4,I4));
   K = M(I2,I4)*M44i;
   M(I2,I2) = M(I2,I2) - K*M(I4,I2);
   M(I2,Iw) = M(I2,Iw) - K*M(I4,Iw);
   M(I2,Iv) = M(I2,Iv) - K*M(I4,Iv);
   F(I2)    = F(I2)    - K*F(I4);
   K = M(Iw,I4)*M44i;
   M(Iw,I2) = M(Iw,I2) - K*M(I4,I2);
   M(Iw,Iw) = M(Iw,Iw) - K*M(I4,Iw);
   M(Iw,Iv) = M(Iw,Iv) - K*M(I4,Iv);
   F(Iw)    = F(Iw)    - K*F(I4);
   K = M(Iv,I4)*M44i;
   M(Iv,I2) = M(Iv,I2) - K*M(I4,I2);
   M(Iv,Iw) = M(Iv,Iw) - K*M(I4,Iw);
   M(Iv,Iv) = M(Iv,Iv) - K*M(I4,Iv);
   F(Iv)    = F(Iv)    - K*F(I4);
   M4a = M;
   F4a = F;

   % G(3)
   M33i = inv(M(I3,I3));
   K = M(I2,I3)*M33i;
   M(I2,I2) = M(I2,I2) - K*M(I3,I2);
   M(I2,Iw) = M(I2,Iw) - K*M(I3,Iw);
   M(I2,Iv) = M(I2,Iv) - K*M(I3,Iv);
   F(I2)    = F(I2)    - K*F(I3);
   K = M(Iw,I3)*M33i;
   M(Iw,I2) = M(Iw,I2) - K*M(I3,I2);
   M(Iw,Iw) = M(Iw,Iw) - K*M(I3,Iw);
   M(Iw,Iv) = M(Iw,Iv) - K*M(I3,Iv);
   F(Iw)    = F(Iw)    - K*F(I3);
   K = M(Iv,I3)*M33i;
   M(Iv,I2) = M(Iv,I2) - K*M(I3,I2);
   M(Iv,Iw) = M(Iv,Iw) - K*M(I3,Iw);
   M(Iv,Iv) = M(Iv,Iv) - K*M(I3,Iv);
   F(Iv)    = F(Iv)    - K*F(I3);
   M3a = M;
   F3a = F;

   % G(2)
   M22i = inv(M(I2,I2));
   K = M(Iw,I2)*M22i;
   M(Iw,Iw) = M(Iw,Iw) - K*M(I2,Iw);
   M(Iw,Iv) = M(Iw,Iv) - K*M(I2,Iv);
   F(Iw)    = F(Iw)    - K*F(I2);
   K = M(Iv,I2)*M22i;
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

   udot = zeros(17,1);
   udot(Iv) = inv(M(Iv,Iv))*F(Iv);

   udot(Iw) = inv(M(Iw,Iw))*(F(Iw)-M(Iw,Iv)*udot(Iv));
   
   udot(I1) = inv(M(I1,I1))*(F(I1)-M(I1,Iv)*udot(Iv)-M(I1,Iw)*udot(Iw));
   
   udot(I2) = inv(M(I2,I2))*(F(I2)-M(I2,Iv)*udot(Iv)-M(I2,Iw)*udot(Iw));
   
   udot(I3) = inv(M(I3,I3))*(F(I3)-M(I3,Iv)*udot(Iv)-M(I3,Iw)*udot(Iw)-M(I3,I2)*udot(I2));
   
   udot(I4) = inv(M(I4,I4))*(F(I4)-M(I4,Iv)*udot(Iv)-M(I4,Iw)*udot(Iw)-M(I4,I2)*udot(I2));
   
   
   




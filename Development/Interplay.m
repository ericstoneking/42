% Explore Jacobi's Integral, and classification of orbits as central or
% interplay.

% Ref. A.E. Roy, 5.10

mu = 1/81;
x1 = -mu;
x2 = 1.0-mu;

F = 100;
coef1 = (F+1)*mu-1;
coef2 = 2*(F*mu^2+(1-mu)^2);
coef3 = F*mu^3-(1-mu)^3;
xc = roots([coef1 coef2 coef3])
if (abs(xc(2)+mu)) < abs((xc(1)+mu)), 
    xc = xc(2)
else
    xc = xc(1)
end
Ccrit = xc^2+2*((1-mu)/abs(xc+mu)+mu/abs(xc+mu-1))

[rin,rout] = ShellOfInfluence(mu,0.01);
ang=[0:pi/200:2*pi]';
xin=x2+rin*cos(ang);
yin=rin*sin(ang);
xout=x2+rout*cos(ang);
yout=rout*sin(ang);

N=1000;
C=zeros(N,N);

for i=1:N,
    x=(i-N/2)*5.0/N;
    for j=1:N,
        y=(j-N/2)*5.0/N;
        r1=sqrt((x-x1)^2+y^2);
        r2=sqrt((x-x2)^2+y^2);
        C(i,j)=x^2+y^2+2*(1-mu)/r1+2*mu/r2;
        if r2 > rin & r2 < rout,
           C(i,j) = 0;
        end
        if C(i,j)>Ccrit, 
            C(i,j)=Ccrit;
        end
    end
end

figure(1)
surf(C,'EdgeColor','none')
axis('square')
view(2)
colorbar


        
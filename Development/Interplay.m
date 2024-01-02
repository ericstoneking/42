% Explore Jacobi's Integral, and classification of orbits as central or
% interplay.

% Ref. A.E. Roy, 5.10

mu = 1/333000;
x1 = -mu;
x2 = 1.0-mu;

F = 400;
coef1 = (F+1)*mu-1;
coef2 = 2*(F*mu^2+(1-mu)^2);
coef3 = F*mu^3-(1-mu)^3;
xc = roots([coef1 coef2 coef3])
if (abs(xc(2)+mu)) < abs((xc(1)+mu)), 
    xc = xc(2)
else
    xc = xc(1)
end
%Ccrit = xc^2+2*((1-mu)/abs(xc+mu)+mu/abs(xc+mu-1))
Ccrit = 2.1;

%[rin,rout] = ShellOfInfluence(mu,0.01);
%ang=[0:pi/200:2*pi]';
%xin=x2+rin*cos(ang);
%yin=rin*sin(ang);
%xout=x2+rout*cos(ang);
%yout=rout*sin(ang);


N=1000;
C=zeros(N,N);

x = 1.01+(-0.025/2+0.025/N*[1:N]);
y = (-0.025/2+0.025/N*[1:N]);
z = (-0.025/2+0.025/N*[1:N]);

[X,Y] = meshgrid(x,y);

for i=1:N,
    for j=1:N,
        r1=sqrt((x(i)-x1)^2+y(j)^2);
        r2=sqrt((x(i)-x2)^2+y(j)^2);
        %C(i,j)=x(i)^2+y(j)^2+2*(1-mu)/r1+2*mu/r2;
        C(i,j)=2*(1-mu)/r1+2*mu/r2;
        %if r2 > rin && r2 < rout,
        %   C(i,j) = 0;
        %end
        if C(i,j)>Ccrit, 
            C(i,j)=Ccrit;
        end
    end
end

ang = [0:pi/200:2*pi]';

% Orbit of Moon
xm = 1.0+400E3/150E6*cos(ang);
ym = 400E3/150E6*sin(ang);

% Halo Orbit
xh = 1.01+200E3/150E6*cos(ang);
yh = 800E3/150E6*sin(ang);
zh = 400E3/150E6*sin(ang);

figure(1)
surf(X,Y,C','EdgeColor','none')
axis('equal')
view(0,90)
xlabel('X')
ylabel('Y')
hold on
plot3(xm,ym,Ccrit*ones(size(xm)),'k')
plot3(xh,yh,Ccrit*ones(size(xh)),'w')
hold off
        
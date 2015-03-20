% Determine flight regime by comparing energies

% Ref. A.E. Roy, 5.10

mu = 0.25;
x1 = -mu;
x2 = 1.0-mu;

%[rin,rout] = ShellOfInfluence(mu,0.01);
%ang=[0:pi/200:2*pi]';
%xin=x2+rin*cos(ang);
%yin=rin*sin(ang);
%xout=x2+rout*cos(ang);
%yout=rout*sin(ang);

N=200;
E1=zeros(N,N);
E2=zeros(N,N);
E12=zeros(N,N);
E=zeros(N,N);

v1 = [0 -mu];
v2 = [0 1-mu];
for i=1:N,
    x=(i-N/2)*5.0/N;
    for j=1:N,
        y=(j-N/2)*5.0/N;
        r = sqrt(x^2+y^2);
        r1=sqrt((x-x1)^2+y^2);
        r2=sqrt((x-x2)^2+y^2);
        v = [-y x];
        E(i,j) = log((1-mu)/r1 + mu/r2);
        E1(i,j) = log((1-mu)/r1);
        E2(i,j) = log(mu/r2);
        E12(i,j) = log(1/r);
    end
end

figure(1)
surf(E,'EdgeColor','none')
axis('square')
view(2)
colorbar

figure(2)
surf(E1,'EdgeColor','none')
axis('square')
view(2)
colorbar

figure(3)
surf(E2,'EdgeColor','none')
axis('square')
view(2)
colorbar

figure(4)
surf(E12,'EdgeColor','none')
axis('square')
view(2)
colorbar


        
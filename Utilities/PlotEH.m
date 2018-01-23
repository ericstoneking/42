mu=3.98604E14;
a = 6378.145E3+400E3;
n = sqrt(mu/a^3);

A = 0;
C = 0;
B = 2.5;
D = 2.5;


nt = [0:pi/100:2*pi]';
N = max(size(nt));

r1 = zeros(N,3);
v1 = zeros(N,3);
r2 = zeros(N,3);
v2 = zeros(N,3);
r3 = zeros(N,3);
v3 = zeros(N,3);

thz = 0;
Bs = B*sin(thz);
Bc = B*cos(thz);

thy = thz + 0*pi/180;
Ds = D*sin(thy);
Dc = D*cos(thy);

for k=1:N,
   [r1(k,:),v1(k,:)] = EHModesToEHRV(A,Bc,Bs,C,Dc,Ds,n,nt(k));
end

thz = 120*pi/180;
Bs = B*sin(thz);
Bc = B*cos(thz);

thy = thz + 240*pi/180;
Ds = D*sin(thy);
Dc = D*cos(thy);

for k=1:N,
   [r2(k,:),v2(k,:)] = EHModesToEHRV(A,Bc,Bs,C,Dc,Ds,n,nt(k));
end

thz = 240*pi/180;
Bs = B*sin(thz);
Bc = B*cos(thz);

thy = thz + 120*pi/180;
Ds = D*sin(thy);
Dc = D*cos(thy);

for k=1:N,
   [r3(k,:),v3(k,:)] = EHModesToEHRV(A,Bc,Bs,C,Dc,Ds,n,nt(k));
end

figure(1)
plot3(r1(:,1),r1(:,2),r1(:,3),r2(:,1),r2(:,2),r2(:,3),r3(:,1),r3(:,2),r3(:,3))

[r1(1,:);v1(1,:)]

[r2(1,:);v2(1,:)]

[r3(1,:);v3(1,:)]


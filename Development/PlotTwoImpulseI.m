load ti.txt

N2 = 200;
N1=max(size(ti))/N2;

t0=ti(1:N1:N2*N1,1);
TOF=ti(1:N1,2);
[M1,M2]=meshgrid(t0,TOF);
DV=zeros(N1,N2);
for i=1:N2,
   idx = N1*(i-1)+1:N1*i;
   DV(:,i) = ti(idx,3);
end

figure(1)
mesh(M1,M2,DV)
title("DV vs. t0 and TOF")
xlabel("t0")
ylabel("TOF")
zlabel("DV")
view(150,60)

figure(2)
contour(M1,M2,DV)
title("DV vs. t0 and TOF")
xlabel("t0")
ylabel("TOF")
zlabel("DV")
view(150,60)





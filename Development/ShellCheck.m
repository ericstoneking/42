N=100;
mu=logspace(-9,-4,N);
rin=zeros(N,1);
rout=zeros(N,1);
r0=zeros(N,1);
for i=1:N,
   [rin(i),rout(i)] = ShellOfInfluence(mu(i),0.01);
   r0(i)=mu(i)^0.4;
end

figure(1)
semilogx(mu,[rin r0 rout])
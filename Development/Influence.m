% Explore the "zone of interplay", defined by softening the 
% "sphere of influence" boundary.

mu1 = 398604;
mu2 = 4903;
rho = [384400 0];

F = 0.1;

N=1000;
R = 0.5*ones(N+1,N+1);

rho3 = norm(rho)^3;
for i=1:N+1,
    x=(i-N/2)*(1E6/N);
    for j=1:N+1,
        y=(j-N/2)*(1E6/N);
        r=[x y];
        d = r - rho;
        r3 = norm(r)^3;
        d3 = norm(d)^3;
        P1 = norm(-mu1/r3*r);
        D1 = norm(-mu2*(1/d3*d+1/rho3*rho));
        P2 = norm(-mu2/d3*d);
        D2 = norm(-mu1*(1/r3*r-1/rho3*rho));
        Rij = D1*P2/(P1*D2);
        if Rij < F,
            R(i,j) = 0.0;
        elseif Rij > 1/F,
            R(i,j) = 1.0;
        end
    end
end

figure(1)
surf(R,'EdgeColor','none')
view(2)


           
        
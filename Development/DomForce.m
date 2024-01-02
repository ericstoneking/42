% Find flight regime by determining the dominant gravitational force term.

% Ref. A.E. Roy, 5.10

eps = 0.1;
mu = 0.1;

N=1000;
C=zeros(N,N);
R1 = [-mu 0];
R2 = [1-mu 0];

for i=1:N,
    x=(i-N/2)*5.0/N;
    if mod(i,50) == 0,
       i
       fflush(stdout)
    end
    for j=1:N,
        y=(j-N/2)*5.0/N;
        r0 = [x y];
        r1 = [x+mu y];
        r2 = [x-1+mu y];

        a1 = (1-mu)*r1/norm(r1)^3;
        a2 = mu*r2/norm(r2)^3;

        p1 = (1-mu)*(-r1/norm(r1)^3 + (R2-R1)/norm(R2-R1)^3);
        p2 = mu*(-r2/norm(r2)^3 + (R2-R1)/norm(R2-R1)^3);

        a0 = (1-mu)*r0/norm(r0-R1)^3 + mu*r0/norm(r0-R2)^3;
        p10 = (1-mu)*R1/norm(r0-R1)^3;
        p20 = mu*R2/norm(r0-R2)^3;

        if norm(p2) < eps*norm(a1),
           C(i,j) = 1;
        elseif norm(p1) < eps*norm(a2),
           C(i,j) = 2;
        elseif norm(p10+p20) < eps*norm(a0),
           C(i,j) = 4;
        else,
           C(i,j) = 3;
        end

    end
end

figure(1)
surf(C,'EdgeColor','none')
axis('square')
view(2)
colorbar


        
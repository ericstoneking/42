Z = 10*rand(4);
A = Z+Z';

% Find L and D
L = zeros(4);
D = zeros(4,1);
for j=1:4,
   D(j) = A(j,j);
   for k=1:j-1,
      D(j) = D(j) - L(j,k)^2*D(k);
   end
   for i=1:4,
      L(i,j) = A(i,j);
      for k=1:j-1,
         L(i,j) = L(i,j) - L(i,k)*L(j,k)*D(k);
      end
      L(i,j) = L(i,j)/D(j);
   end
end

% Check
A1 = L*diag(D)*L';
A-A1

% Solve LDy = b
b = 5*rand(4,1);
y=zeros(4,1);
for i=1:4,
   LD(i,i) = D(i);
   for j = i+1:4,
      LD(j,i) = L(j,i)*D(i);
   end
end
for i=1:4,
   y(i) = b(i);
   for k=1:i-1,
      y(i) = y(i)-LD(i,k)*y(k);
   end
   y(i) = y(i)/LD(i,i);
end

% Check
LD*y-b

% Solve L'*x = y
x = zeros(4,1);
for i=4:-1:1,
   x(i) = y(i);
   for k=4:-1:i+1,
      x(i) = x(i) - L(k,i)*x(k);
   end
end

% Check
L'*x - y

A*x-b

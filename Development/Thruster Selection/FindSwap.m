function [incoming,outgoing]=FindSwap(T)

[m,n]=size(T);

A = T(2:m-1,1:n-1);
b = T(2:m-1,n);

[m,n]=size(A);

minratio = 1E9;
for i=1:m,
   if b(i) < 0.0,
      for j=m+1:n,
         if A(i,j) < 0.0,
            ratio = b(i)/A(i,j);
            if ratio < minratio;
               minratio = ratio;
               incoming = j;
               outgoing = i;
            end
         end
      end
   end
end

return
function A = GaussElim(A)

[m,n] = size(A);

m=m-1;

for row=1:m,
   pivot = A(row,row);
   ip = row;
   for i=row+1:m,
      if (abs(A(i,row)) > abs(pivot)),
         pivot = A(i,row);
         ip = i;
      end
   end
   if (pivot == 0.0),
      disp("Matrix is singular");
   end

   Temp = A(ip,:);
   A(ip,:) = A(row,:);
   A(row,:) = Temp/pivot;

   for i=row+1:m+1,
      k = A(i,row);
      A(i,:) = A(i,:)-k*A(row,:);
   end
end

for row=m:-1:2,
   for i=1:row-1,
      k = A(i,row);
      A(i,:) = A(i,:) - k*A(row,:);
   end
end

return
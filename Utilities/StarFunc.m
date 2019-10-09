x = [-1:1/31.5:1];
y = x;

for i=1:64,
   for j=1:64,
      %z(i,j) = exp(-(abs(x(i)^0.25)+abs(y(j)^0.25)));
      z(i,j) = exp(-5*sqrt(x(i)^2+y(j)^2));
      %z(i,j) = 1.0;
   end
end

Zmax = max(max(z));
Zmin = min(min(z));
H = Zmax-Zmin;
z = (z-Zmin)/H;

z = (z-0.2)/0.8;
for i=1:64,
   for j=1:64,
      if z(i,j) < 0,
         z(i,j) = 0;
      end
   end
end

surf(x,y,z)

starfile = fopen('StarTexture.ppm','wb');
fprintf(starfile,'P6\n#.\n64 64\n255\n');
for i=1:64,
   for j=1:64,
      b = floor(255*z(i,j));
      fprintf(starfile,'%c%c%c%c',b,b,b,b);
   end
end
fclose(starfile);
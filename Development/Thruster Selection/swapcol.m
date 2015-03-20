function T=swapcol(T,i1,i2)

temp = T(:,i1);
T(:,i1)=T(:,i2);
T(:,i2)=temp;

return

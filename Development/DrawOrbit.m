Re = 6378.145E3;
a = 5*Re;
e = 0.5;
p = a*(1-e^2);
POV = [a*(1-e) 0 50];
MinArc = 0.1;
MinRange = 50.0;
ArcGain = MinArc/MinRange;

th = 0;
N = 0;
while(th < 2*pi),
   r = p/(1+e*cos(th));
   dist = norm([r*cos(th) r*sin(th) 0] - POV);
   ds = ArcGain*dist;
   dth = 1/r*(1+(r*e*sin(th)/p)^2)^(-0.5)*ds;

   N = N+1;
   TH(N) = th;
   R(N) = r;
   DIST(N) = dist;
   DS(N) = ds;
   DTH(N) = dth;

   th = th + dth;
end


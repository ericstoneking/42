% Pole Placement for a Spinner

% Known Parameters
I3 = 300;
It = 100;
Ki = (I3-It)/It;
Om = 0.1;

% Assigned damping ratios
z1 = 0.1;
z2 = 0.5;

% Initial guess for state vector [Kn/It, Kp/It, w1, w2]
x = [0 0 Om Ki*Om]';

for i=1:10,
   f = [x(1)-z1*x(3)-z2*x(4);
        (Ki+1)^2*Om^2+x(1)^2-x(3)^2-x(4)^2-4*z1*z2*x(3)*x(4);
        Om^2*x(1)+(Ki-1)*Om*x(2)-x(3)*x(4)*(z1*x(4)+z2*x(3));
        Ki^2*Om^4+(x(2)-x(1)*Om)^2-(x(3)*x(4))^2];
   fx = [          1                  0                    -z1                            -z2;
                 2*x(1)               0             -2*x(3)-4*z1*z2*x(4)          -2*x(4)-4*z1*z2*x(3);
                  Om^2             (Ki-1)*Om      -2*z2*x(3)*x(4)-z1*x(4)^2     -2*z1*x(3)*x(4)-z2*x(3)^2;
         -2*Om*(x(2)-x(1)*Om)  2*(x(2)-x(1)*Om)        -2*x(3)*x(4)^2                 -2*x(3)^2*x(4)];
   x = x - fx\f;
end

f

% Scale for final gains
Kn = x(1)*It
Kp = x(2)*It
w1 = x(3)
w2 = x(4)
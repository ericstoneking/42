load time.42
load qbn.42

RPY = 2.0*qbn(:,1:3)*180/pi;

figure(1)
plot(time,RPY)
axis([0 time(end) -0.01 0.01])


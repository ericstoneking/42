% Size Actuators
load time.42
load EnvTrq00.42
N = max(size(time));

Trq = EnvTrq00(:,1:3);
Mom = EnvTrq00(:,4:6);

AvgTrq = mean(Trq)
SecMom = [AvgTrq(1)*time AvgTrq(2)*time AvgTrq(3)*time];
CycMom = Mom-SecMom;
CycMom = CycMom - [mean(CycMom(:,1))*ones(N,1) mean(CycMom(:,2))*ones(N,1) mean(CycMom(:,3))*ones(N,1)];

figure(1)
plot(time,Mom)
title('Accumulated Environmental Momentum')
xlabel('Time, sec')
ylabel('Nms')
legend('X','Y','Z')

figure(2)
plot(time,SecMom)
title('Secular Momentum')
xlabel('Time, sec')
ylabel('Nms')
legend('X','Y','Z')

figure(3)
plot(time,CycMom)
title('Cyclic Momentum')
xlabel('Time, sec')
ylabel('Nms')
legend('X','Y','Z')

WheelCap = max(abs(CycMom))

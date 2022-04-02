data = readtable('crash.txt');
%  data = readtable('filtered.txt');
x = data(:,1);
xs = table2array(x);
y = data(:,2);
ys = table2array(y);

set(0,'defaulttextinterpreter','latex')
set(0, 'defaultlegendinterpreter', 'latex')
set(groot,'defaultAxesTickLabelInterpreter','latex');  
% percentage of deaths for varying number of food sources
figure
figtemp = figure('units', 'centimeters');
plot(xs, 'LineWidth',3)
hold on 
plot(ys,'LineWidth',3)
ylabel('Acceleration ($m/s^2$)', 'FontSize', 20) % y-axis label
xlabel('Time (ms)', 'FontSize', 20) % y-axis label
title('Acceleration of Robot Hitting a Wall')
xlim([(0) (120)]); % not done yet
ylim([(-20000) (9000)]); % not done yet
legend('Accelerometer', 'Odometry System ', 'location', 'southeast')
 savepdf()
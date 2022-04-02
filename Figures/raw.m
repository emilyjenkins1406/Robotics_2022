data = readtable('raw.txt');
%  data = readtable('filtered.txt');
x = data(:,1);
xs = table2array(x);

set(0,'defaulttextinterpreter','latex')
set(0, 'defaultlegendinterpreter', 'latex')
set(groot,'defaultAxesTickLabelInterpreter','latex');  
% percentage of deaths for varying number of food sources
figure
figtemp = figure('units', 'centimeters');
plot(xs(50:1000,:))
ylabel('Acceleration ($m/s^2$)', 'FontSize', 20) % y-axis label
xlabel('Time (Arbitrary Units)', 'FontSize', 20) % y-axis label
title('Unfiltered Accelerometer Data')
xlim([(0) (951)]); % not done yet
savepdf()
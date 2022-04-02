data = readtable('putty.txt');

one = data(:,1);
oness = table2array(one);

two = data(:,2);
twos = table2array(two);

three = data(:,3);
threes = table2array(three);

group = [    ones(size(oness));
         2 * ones(size(twos));
         3 * ones(size(threes))];


% percentage of deaths for varying number of food sources
figure
figtemp = figure('units', 'centimeters');
boxplot([oness; twos; threes],group, 'BoxStyle','outline', 'Colors', 'rgbm', 'Symbol','.k', 'PlotStyle','compact');

ylabel('Error', 'FontSize', 20) % y-axis label
xlabel('Alpha ($\alpha$)', 'FontSize', 20) % y-axis label
title('Box and Whisker Plot')
ylim([(-500) (500)]); % not done yet
% xlim([(0.46) (0.54)]); % not done yet

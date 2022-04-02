
t=0.4:.001:0.6;
alpha = 0.6;
f=1;

sp=1*0.5*(square(2*pi*f*t)+1);

smoothings_three = [1];
smoothings_six = [1];
smoothings_nine = [1];
smoothings_zero = [1];
smoothings_one = [1];

for i = 1:length(sp)

    smoothings_six(end+1) =  (alpha * (sp(i)) + ((1 - alpha) * smoothings_six(i)));
end

smoothings_six = smoothings_six(2:end);

alpha = 0.9;

for i = 1:length(sp)

    smoothings_nine(end+1) =  (alpha * (sp(i)) + ((1 - alpha) * smoothings_nine(i)));
end

smoothings_nine = smoothings_nine(2:end);

alpha = 0.3;

for i = 1:length(sp)

    smoothings_three(end+1) =  (alpha * (sp(i)) + ((1 - alpha) * smoothings_three(i)));
end

smoothings_three = smoothings_three(2:end);
alpha = 0;

for i = 1:length(sp)

    smoothings_zero(end+1) =  (alpha * (sp(i)) + ((1 - alpha) * smoothings_zero(i)));
end

smoothings_zero = smoothings_zero(2:end);
alpha = 1;

for i = 1:length(sp)

    smoothings_one(end+1) =  (alpha * (sp(i)) + ((1 - alpha) * smoothings_one(i)));
end

smoothings_one = smoothings_one(2:end);

set(0,'defaulttextinterpreter','latex')
set(0, 'defaultlegendinterpreter', 'latex')
set(groot,'defaultAxesTickLabelInterpreter','latex');  
% percentage of deaths for varying number of food sources
figure
figtemp = figure('units', 'centimeters');
plot(t,smoothings_one, 'LineWidth', 4)
hold on
plot(t,smoothings_three, 'LineWidth', 4)
hold on 
plot(t,smoothings_six, 'LineWidth', 4)
hold on 
plot(t,smoothings_nine, 'LineWidth', 4)

ylabel('Amplitude', 'FontSize', 20) % y-axis label
xlabel('Time (Arbitrary Units)', 'FontSize', 20) % y-axis label
title('Square Wave With Smoothing')
ylim([(-0.2) (1.1)]); % not done yet
xlim([(0.46) (0.54)]); % not done yet
legend('$\alpha = 1$','$\alpha = 0.3$','$\alpha = 0.6$', '$\alpha = 0.9$','FontSize', 15, 'location', 'southwest')
savepdf()



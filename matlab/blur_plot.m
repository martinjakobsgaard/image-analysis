%% Plot all data
clc; clear; close;

%data = readtable("dynamic_range_dark_dataset.csv");
data = readtable("dynamic_range_dark_dataset_roi.csv");

hold on
scatter(data.exposure, data.result)
hold off

%% Yo
clc; clear; close;

data = readtable("dynamic_range_dark_dataset_roi.csv");
%data = readtable("blur_data_dark_roi.csv");
data = sortrows(data,'index','ascend');

size(data.exposure,1)

averageBlur = [];

currentExposure = 0;
tempBuffer = [];

exposureVal = [currentExposure];


subindex = 0;
avg_exposure = [];
avg_result = [];
for index = 1:5:(size(data.exposure,1)-5)
    avg_exposure = [avg_exposure; data.exposure(index)];
    avg_result = [avg_result; mean(data.result(index:index+4))];
end

late_avg_exposure = avg_exposure(11:60);
late_avg_result = avg_result(11:60);
fitlm(late_avg_exposure,late_avg_result);


% FIGURE SETTTINGS
f = figure();
f.Position = [100 100 1000 450];
set(gca,'FontSize',14)
set(gca,'linewidth',2)
set(gca,'FontWeight','Bold')

hold on
xx = min(avg_exposure)/10:0.001:max(avg_exposure)/10; % change your step size (currently 0.001) based on desired plot smoothness
yy = spline(avg_exposure/10,avg_result,xx);
plot(xx,yy);
ycalc = 7.994+0*xx;
plot(xx, ycalc,'--');

%scatter(avg_exposure, avg_result)
xlabel({'Exposure time [ms]'});
ylabel({'Avg. Dynamic range'});
legend('DR','log_2(255)')
grid on
%title('Dark garments')
xlim([0 15]);
ylim([0 9]);
xticks(0:1:15);
%legend('Exposure time','Regressed exposure time')
% MORE FIGURE MAGIC
set(findall(gca, 'Type', 'Line'),'LineWidth',2);
%set(gca,'linewidth',2)
hold off



%% Average plot
clc; clear; close;

data = readtable("dynamic_range_dark_dataset.csv");
%data = readtable("blur_data_dark_roi.csv");
data = sortrows(data,'index','ascend');

size(data.exposure,1)

averageBlur = [];

currentExposure = 0;
tempBuffer = [];

exposureVal = [currentExposure];

%for c = 1:size(data.exposure,1)
%    if ((data.exposure(c)) == currentExposure)
%        tempBuffer = [tempBuffer; data.result(c)];
%    else 
%        currentExposure = data.exposure(c);
%        averageBlur = [averageBlur; mean(tempBuffer)];
%        exposureVal = [ex:posureVal; currentExposure];
%        tempBuffer = [];
%        tempBuffer = [tempBuffer; data.result(c)];
%    end
%end

subindex = 0;
avg_exposure = [];
avg_result = [];
for index = 1:5:(size(data.exposure,1)-5)
    avg_exposure = [avg_exposure; data.exposure(index)];
    avg_result = [avg_result; mean(data.result(index:index+4))];
end

late_avg_exposure = avg_exposure(11:60);
late_avg_result = avg_result(11:60);
fitlm(late_avg_exposure,late_avg_result);
ycalc = 0.26748*late_avg_exposure-1.9832;

%averageBlur = [averageBlur; mean(tempBuffer)];

%options = optimset('MaxFunEvals',5000);

%f = @(b,x) b(1).*exp(b(2).*x)+b(3);                                   
%B = fminsearch(@(b) norm(averageBlur - f(b,exposureVal)), [-0; -1; 100]) 

% FIGURE SETTTINGS
f = figure();
f.Position = [100 100 1000 450];
set(gca,'FontSize',14)
set(gca,'linewidth',2)
set(gca,'FontWeight','Bold')



hold on
xx = min(avg_exposure)/10:0.001:max(avg_exposure)/10; % change your step size (currently 0.001) based on desired plot smoothness
yy = spline(avg_exposure/10,avg_result,xx);
plot(xx,yy);
ycalc = 0.26748*xx*10-1.9832;
plot(xx, ycalc,'--');

%scatter(avg_exposure, avg_result)
%plot(avg_result, f(B,avg_result), '-r')
%yyaxis left
xlabel({'Exposure time [ms]'});
ylabel({'Avg. variance of Laplacian'});

grid on
%title('Dark garments')
xlim([0 6]);
ylim([0 15]);
%legend('Exposure time','Regressed exposure time')
% MORE FIGURE MAGIC
set(findall(gca, 'Type', 'Line'),'LineWidth',2);
%set(gca,'linewidth',2)
hold off

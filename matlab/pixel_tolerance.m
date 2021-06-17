clear;clc;close;

top_speed = 4000; % mm/s
image_width = 640; % px
image_fov = 770; % mm
%pixel_tolerance = 0.5; % px

f = figure();
f.Position = [100 100 1000 450];
set(gca,'FontSize',14)
set(gca,'linewidth',2)
set(gca,'FontWeight','Bold')

image_resolution = image_width/image_fov;
%exposure = ((pixel_tolerance*image_resolution)/top_speed)*1000
set(findall(gca, 'Type', 'Line'),'LineWidth',2)
hold on
    for i = 1:8
        pt = @(e) ((e*top_speed)/(image_resolution))/1000;
        fplot(pt,[0,6], 'LineWidth', 2)
        top_speed = top_speed-500;
    end
legend('4000','3500','3000', '2500', '2000', '1500', '1000', '500');
xlabel({'Exposure time [ms]'});
grid on

ylabel({'Blur distance [px]'});
set(findall(gca, 'Type', 'Line'),'LineWidth',2)

hold off

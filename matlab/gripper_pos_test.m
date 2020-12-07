close; clc; clear;

data = readtable("blur_data_0.csv");

hold on
scatter(data.exposure, data.result)
hold off
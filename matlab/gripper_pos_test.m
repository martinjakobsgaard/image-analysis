close; clc; clear;

data = readtable("data-tiny-gripper-pos.csv");

hold on
scatter(data.robot_z, data.image_z)
hold off
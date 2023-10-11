

clc;
clear;




Fs = 44000;                         %%% WM8978的采样率
F_mo = 25;      %%%===调制波频率（HZ）
time = 600;     %%%===语音时间（sec）


x = linspace(0,2*pi,Fs*time);                     %%% 0-260 600s
y1 = sin(1000*x*time);                           %%% 1000HZ载波
y2 = sin(F_mo*x*time);                           %%% 30HZ调制波
y3 = y1.*y2;                                     %%% 合成调制波
voice1 = y3*1000;                                %%% 幅值新增1000




y4 = square(2*pi*1000*x*time,50);                %%%  1000HZ click 载波
y5 = (square(2*pi*F_mo*x*time,10)+1)/2; 
y6 = y5.*y4;
voice2 = y6*1000;



audiowrite('调制25HZ.wav',voice1,Fs);
audiowrite('click25HZ.wav',voice2,Fs);






t = 0:1/Fs:60;                 %%% 生成时间 60s
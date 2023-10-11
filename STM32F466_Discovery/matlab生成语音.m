

clc;
clear;




Fs = 44000;                         %%% WM8978�Ĳ�����
F_mo = 25;      %%%===���Ʋ�Ƶ�ʣ�HZ��
time = 600;     %%%===����ʱ�䣨sec��


x = linspace(0,2*pi,Fs*time);                     %%% 0-260 600s
y1 = sin(1000*x*time);                           %%% 1000HZ�ز�
y2 = sin(F_mo*x*time);                           %%% 30HZ���Ʋ�
y3 = y1.*y2;                                     %%% �ϳɵ��Ʋ�
voice1 = y3*1000;                                %%% ��ֵ����1000




y4 = square(2*pi*1000*x*time,50);                %%%  1000HZ click �ز�
y5 = (square(2*pi*F_mo*x*time,10)+1)/2; 
y6 = y5.*y4;
voice2 = y6*1000;



audiowrite('����25HZ.wav',voice1,Fs);
audiowrite('click25HZ.wav',voice2,Fs);






t = 0:1/Fs:60;                 %%% ����ʱ�� 60s
% hold on
% x_scatter = [19 25 31 38 44];
% y_scatter = [19.0 32.3 49.0 73.3 97.8];
% scatter(x_scatter,y_scatter);
% 
% x = linspace(19,44,100);
% y = 0.9726046+0.0500351*x.^2;
% plot(x,y,"r--")
% 
% hold off

hold on
x_scatter = [0 5 10 15 20 25 30 35 40 45 50 55];
y_scatter = [0 1.27 2.16 2.86 3.44 3.87 4.15 4.37 4.51 4.58 4.62 4.64];
scatter(x_scatter,y_scatter);

x = linspace(0,55,100);
y = 5.21510*exp(-7.49617./x);
plot(x,y,"r--")

hold off




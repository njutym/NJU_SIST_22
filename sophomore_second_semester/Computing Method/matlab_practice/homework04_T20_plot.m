x = linspace(-1,1,100);
y = sin(x/2);
plot(x,y,"r--")

hold on

y2 = -0.020545*x.^3+0.499938*x;
plot(x,y2,"b-")

hold off

y3 = y - y2;
plot(x,y3,"r--");

% 积分1
a = 0.1;b = 4;acc = 1e-7;
% 计算函数导数的值并找到最大值方便后面误差判断
syms x;
f = @(x)sin(x)/x; 
df = diff(f,x,2);
df = matlabFunction(df);

xx = linspace(a,b,1000);
yy = zeros(size(xx)); % 初始化一个向量来存储结果
for i = 1:length(xx)
    yy(i) = double(df(xx(i)));
end

max_value = max(yy);

[y,n,points] = trapezoid(f, a, b, acc,max_value);
display = [y,n,points];

% 积分2
% a = -2;
% b = 4;
% acc = 1e-7;
% syms x;
% f = @(x)exp(-(x^2)); 
% df = diff(f,x,2);
% df = matlabFunction(df);
% 
% xx = linspace(a,b,1000);
% yy = zeros(size(xx)); % 初始化一个向量来存储结果
% for i = 1:length(xx)
%     yy(i) = double(df(xx(i)));
% end
% 
% max_value = max(yy);
% [y,n,points] = trapezoid(f, a, b, acc,max_value);
% display = [y,n,points];


% 积分3
% a = 0;
% b = 1;
% acc = 1e-7;
% syms x;
% f = @(x)log(1+x)/x; 
% df = diff(f,x,2);
% df = matlabFunction(df);
% 
% xx = linspace(a,b,1000);
% yy = zeros(size(xx)); % 初始化一个向量来存储结果
% for i = 1:length(xx)
%     yy(i) = double(df(xx(i)));
% end
% max_value = max(yy);
% [y,n,points] = trapezoid(f, a, b, acc,max_value);
% display = [y,n,points];





function [y,n,points] = trapezoid(f, a, b, acc,max_value)

n = 1; %区间数字
points = n + 1;

y = (b-a)/2*(f(a)+f(b));%梯形公式

% 第三个积分0处为1，需要特别说明，改用以下式子,while中的f(a)也要改掉
%y = (b-a)/2*(1+f(b))

err = b-a;
while err >= acc
    n = n * 2;
    h = (b-a)/n;
    Sigma = 0;
    for k = 1:(n-1)
        Sigma = Sigma + f(a + k*h);
    end
    y = (h/2)*(f(a)+f(b)+2*Sigma);
    err = abs((b-a)/12*h*h*max_value) ;
end
    disp(y)
    disp(n)
    points = n + 1;
    disp(points)
    return
end


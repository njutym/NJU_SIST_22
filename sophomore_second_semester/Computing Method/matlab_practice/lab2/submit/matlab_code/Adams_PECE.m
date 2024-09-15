
% 定义函数和真实积分结果
f = @(x,y) ( (4*x.^2-2*x.*y)/(1+x.^2) );
exact_solution = @(x) (2+ (4/3)*x.^3)/(1+x.^2);

y = double(zeros(1,100));

y(1) = 2;

x = double(zeros(1,100));

h = 0.1;

for n = 1:51
    x(n) = 0.1*(n-1);
end


% 一种较差的实现，开销大
% frac = double(zeros(1,100));


% 好的实现，frac这个存储预测值的地方只需要4个空间
frac = double(zeros(1,4));
frac(1) = f(x(1),y(1));

% 利用经典四阶Runge_Kutta方法预测y2,y3,y4
[y(2),y(3),y(4),frac(2),frac(3),frac(4)] = classical_Runge_Kutta(f,x,y,h);


% 差的PECE实现，消耗空间多
% y_bar = double(zeros(1,100));
% f_bar = double(zeros(1,100));

% for n = 4:50
%     %预测
%     y_bar(n+1) = y(n) + h/24*(55*frac(n)-59*frac(n-1) ...
%         +37*frac(n-2)-9*frac(n-3));
%     f_bar(n+1) = f(x(n+1),y_bar(n+1));
%     %校正
%     y(n+1) = y(n) + h/24*(9*f_bar(n+1)+19*frac(n) ...
%         -5*frac(n-1)+frac(n-2));
%     frac(n+1) = f(x(n+1),y(n+1));
% end


% 好的PECE实现，省空间
for n = 4:50
    % 预测
    y_bar = y(n) + h/24*(55*frac(4)-59*frac(3) ...
        +37*frac(2)-9*frac(1));
    f_bar = f(x(n+1),y_bar);
    %校正
    y(n+1) = y(n) + h/24*(9*f_bar+19*frac(4) ...
        -5*frac(3)+frac(2));
    frac(1) = frac(2);
    frac(2) = frac(3);
    frac(3) = frac(4);
    frac(4) = f(x(n+1),y(n+1));
end


% 格式化输出结果,结果通过命令行窗口查看
fprintf(' i   xi    yi      y(xi)      y(xi) - yi\n');
fprintf('---------------------------------------------\n');
for i = 1:n+1
    yi = y(i);
    xi = x(i);
    exact_yi = exact_solution(xi);
    fprintf('%2d  %2.1f  %7.6f  %7.6f  %.10e\n', i-1, xi, yi, exact_yi, exact_yi - yi);
end

% 绘制图象
% 准确值为红色虚线
xx = linspace(0, 5, 100);  % 在 0 到 5 范围内生成 100 个点
yy = double(zeros(1,100));
for i = 1 : 100
    yy(i) = exact_solution(xx(i));
end
plot(xx, yy,'r--');
title('Plot of result');
xlabel('x');
ylabel('y');
grid on;
% 预测值为蓝色圆点(圆点也标为蓝色了)
hold on;
plot(x, y, 'bo');  
hold off;


% 利用经典四阶Runge_Kutta方法预测y2,y3,y4的函数
function [y2,y3,y4,frac2,frac3,frac4] = classical_Runge_Kutta(f,x,y,h)
    for n = 1:3
        K1 = f(x(n),y(n));
        K2 = f(x(n)+h/2,y(n)+h/2*K1);
        K3 = f(x(n)+h/2,y(n)+h/2*K2);
        K4 = f(x(n)+h,y(n)+h*K3);
        y(n+1) = y(n) + h/6*(K1 + 2*K2 + 2*K3 + K4);
    end
    y2 = y(2);
    y3 = y(3);
    y4 = y(4);
    frac2 = f(x(2),y2);
    frac3 = f(x(3),y3);
    frac4 = f(x(4),y4);
end















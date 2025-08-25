x = [1 3 8 12 15 20 24];
y = [8 9 16 23 22 18 10];

% 绘制原始数据和插值结果
hold on;
for i = 1:length(x)
    plot(x(i),y(i),'ro');
end
xlabel('x');
ylabel('y');
title('分段抛物线插值');
legend('原始数据');
hold off;

% 绘制分段插值的分段
hold on;
xx = linspace(x(1), x(length(x)), 1000000);
for i = 1:length(xx)
    yy(i) = fenduanpaowu(x, y, xx(i));
end
plot(xx,yy,'r--');
hold off;

function y = fenduanpaowu(xi, yi, x)
    if x < xi(2)
        L1 = (x - xi(2)) * (x - xi(3)) / (xi(1) - xi(2)) / (xi(1) - xi(3));
        L2 = (x - xi(1)) * (x - xi(3)) / (xi(2) - xi(1)) / (xi(2) - xi(3));
        L3 = (x - xi(1)) * (x - xi(2)) / (xi(3) - xi(1)) / (xi(3) - xi(2));
        y = L1 * yi(1) + L2 * yi(2) + L3 * yi(3);
    elseif x >= xi(end - 1)
        L1 = (x - xi(end - 1)) * (x - xi(end)) / (xi(end - 2) - xi(end - 1)) / (xi(end - 2) - xi(end));
        L2 = (x - xi(end - 2)) * (x - xi(end)) / (xi(end - 1) - xi(end - 2)) / (xi(end - 1) - xi(end));
        L3 = (x - xi(end - 2)) * (x - xi(end - 1)) / (xi(end) - xi(end - 2)) / (xi(end) - xi(end - 1));
        y = L1 * yi(end - 2) + L2 * yi(end - 1) + L3 * yi(end);
    else
        for k = 3:length(xi)-1
            if x < xi(k)
                if abs(x - xi(k-1)) < abs(x - xi(k))
                    i = k - 1;
                else
                    i = k;
                end
                L1 = (x - xi(i)) * (x - xi(i+1)) / (xi(i-1) - xi(i)) / (xi(i-1) - xi(i+1));
                L2 = (x - xi(i-1)) * (x - xi(i+1)) / (xi(i) - xi(i-1)) / (xi(i) - xi(i+1));
                L3 = (x - xi(i-1)) * (x - xi(i)) / (xi(i+1) - xi(i-1)) / (xi(i+1) - xi(i));
                y = L1 * yi(i-1) + L2 * yi(i) + L3 * yi(i+1);
                return
            end
        end
    end
end
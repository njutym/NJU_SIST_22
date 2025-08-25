%输入部分
%输入周期形式三次样条的三对角矩阵维数
n = 30;

%输入hi，其中h1为左下角和右上角的元素，hi(i>1)为三对角两旁元素
h = double(zeros(n,1));
for i = 1 : n
    h(i) = -1;
end

%输入gi，即主对角线元素,注意主对角线元素为2*gi
g = double(zeros(n,1));
for i = 1 : n
    g(i) = 2;
end

%输入右端向量d,其中每个元素为6di,i从1到n
d = double(zeros(n,1));
d(1) = 1; d(n) = 1;
for i = 2 : n-1
    d(i) = 8/6;
end

%计算部分
%计算pk，qk，uk
p = double(zeros(n-1,1));
q = double(zeros(n-1,1));
u = double(zeros(n-1,1));
p(1) = 2*g(1); q(1) = h(2)/p(1); u(1) = 6*d(1)/p(1);
for k = 2: n-1
    p(k) = 2*g(k) - h(k)*q(k-1);
    q(k) = h(k+1) / p(k);
    u(k) = (6*d(k)-h(k)*u(k-1))/p(k);
end

%计算sk
s = double(zeros(n,1));
s(1) = 1;
for k = 1:n-1
    s(k+1) = -h(k)*s(k)/p(k);
end

%计算tk,vk
t = double(zeros(n,1));
v = double(zeros(n,1));
t(n) = 1; v(n) = 0;
for k = n-1:-1:1
    t(k) = -q(k)*t(k+1) + s(k+1);
    v(k) = -q(k)*v(k+1) + u(k);
end

%计算x
x = double(zeros(n,1));
x(n) = (6*d(n)-h(1)*v(1)-h(n)*u(n-1))/(h(1)*t(1)+h(n)*(s(n)-q(n-1))+2*g(n));
for k = 2:n-1
    x(k-1) = t(k-1)*x(n) + v(k-1);
end
x(n-1) = u(n-1) + (s(n-1) - q(n-1))*x(n);

disp('方程组Ax=b的解x是：');
for i = 1:length(x)
    fprintf('x%d=%.4f\n', i, x(i));
end

% 方程组Ax=b的解x是：
%     3.2679
%     3.8038
%     3.9474
%     3.9859
%     3.9962
%     3.9990
%     3.9997
%     3.9999
%     4.0000
%     4.0000
%     4.0000
%     4.0000
%     4.0000
%     4.0000
%     4.0000
%     4.0000
%     4.0000
%     4.0000
%     4.0000
%     4.0000
%     4.0000
%     4.0000
%     3.9999
%     3.9997
%     3.9990
%     3.9962
%     3.9859
%     3.9474
%     3.8038
%     3.2679





%输入部分

%系数矩阵
A0 = [
    3 1 0 6;
    2 3 1 0;
    0 2 3 2;
    6 0 1 3
];
%右端向量
b0 = [3; 4; 5; 6];

%将xn先视作参数
syms xn;

%将n阶矩阵先转化为n-1阶三对角矩阵求解，其中xn为参数
[A,b] = transform(A0,b0,xn);

% LU 分解
[L, U] = lu_tridiagonal(A);

% 追赶法求解
x = solve_lu_tridiagonal(U, xn, A, b, A0, b0);

% 显示结果
disp('n-1阶L 矩阵为:');
disp(L);
disp('n-1阶U 矩阵为:');
disp(U);
disp('最终解向量 x 为:');
disp(x);

function [A,b] = transform(A0,b0,xn)
    n = size(A0, 1);
    A = sym(zeros(n-1,n-1));
    for i = 1 : n-1
        for j = 1 : n-1
            A(i,j) = sym(A0(i,j));
        end
    end
    b = sym(zeros(n-1,1));
    for i = 1 : n-1
        b(i) = sym(b0(i));
    end
    b(1) = b(1) - A0(1,n)*xn;
    b(n-1) = b(n-1) - A0(n-1,n)*xn;
end

function [L, U] = lu_tridiagonal(A)
    n = size(A, 1);
    L = double(zeros(n));
    U = double(zeros(n));
    
    L(1,1) = A(1,1);
    U(1,1) = 1;
    U(1,2) = A(1,2) / L(1,1);

    for i = 2:n-1
        L(i, i-1) = A(i,i-1);
        L(i,i) = A(i,i) - A(i,i-1)*U(i-1,i);
        U(i,i) = 1;
        U(i,i+1) = A(i,i+1) / (A(i,i) - A(i,i-1)*U(i-1,i));
    end

    L(n, n-1) = A(n,n-1);
    L(n,n) = A(n,n) - A(n,n-1)*U(n-1,n);
    U(n,n) = 1;
end

function x = solve_lu_tridiagonal(U,xn,A,b,A0,b0)
    n = length(b);
    y = sym(zeros(n, 1));
    x = sym(zeros(n, 1));

    % 前向替代解决 Ly = b
    y(1) = b(1)/A(1,1);
    for i = 2:n
        y(i) = (b(i)-A(i,i-1)*y(i-1)) / (A(i,i)-A(i,i-1)*U(i-1,i));
    end

    % 回代解决 Ux = y
    x(n) = y(n);
    for i = n-1:-1:1
        x(i) = y(i) - U(i,i+1)*x(i+1);
    end
    
    n = n + 1;
    % 至此x(1)到x(n-1)已经完全转化成了x(n)的表达式
    eqn = A0(n,1)*x(1) + A0(n,n-1)*x(n-1) + A0(n,n)*xn == b0(n);
    % 使用 solve 函数求解方程，解 xn
    sol = solve(eqn, xn);
    
    %将x(1)到x(n-1)中的xn代成具体解
    x = subs(x, xn, sol); % 这里的 sol 是之前通过 solve 函数解得的 xn

end


% 一开始错误的尝试，不用参数，将xn带换掉，但是得到的不再是三对角矩阵
% function [A,b] = transform(A0,b0)
%     n = size(A0, 1);
%     A = double(zeros(n-1,n-1));
%     for i = 1 : n-1
%         for j = 1 : n-1
%             A(i,j) = A0(i,j);
%         end
%     end
%     b = double(zeros(n-1,1));
%     for i = 1 : n-1
%         b(i) = b0(i);
%     end
%     b(1) = b(1) - b0(n)*A0(1,n)/A0(n,n);
%     A(1,1) = A(1,1)-A0(1,n)*A0(n,1)/A0(n,n);
%     A(1,n-1) = A(1,n-1)-A0(1,n)*A0(n,n-1)/A0(n,n);
% 
%     b(n-1) = b(n-1) - b0(n)*A0(n-1,n)/A0(n,n);
%     A(n-1,1) = A(n-1,1)-A0(n-1,n)*A0(n,1)/A0(n,n);
%     A(n-1,n-1) = A(n-1,n-1)-A0(n-1,n)*A0(n,n-1)/A0(n,n);
% 
% end









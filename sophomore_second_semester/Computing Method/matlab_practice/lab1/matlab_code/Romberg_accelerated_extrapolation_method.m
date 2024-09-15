
% 积分1
a = 0.1;
b = 4;
acc = 1e-7;
f = @(x)sin(x)/x; 
y = Romberg(f, a, b, acc);

% 积分2
% a = -2;
% b = 4;
% acc = 1e-7;
% f = @(x)exp(-(x^2)); 
% y = Romberg(f, a, b, acc);

% 积分3
% a = 0;
% b = 1;
% acc = 1e-7;
% f = @(x)log(1+x)/x; 
% y = Romberg(f, a, b, acc);



function y = Romberg(f, a, b, acc)

k = 0;

T = zeros(10,10,'double');

T(1,1) = (b-a)/2*(f(a)+f(b));%梯形公式求出T(0,0)

% 第三个积分0处为1，需要特别说明，改用以下式子
%T(1,1) = (b-a)/2*(1+f(b))

err = b-a;
while err >= acc
    k = k+1;
    Sigma = 0;
    for i = 1:pow2(k-1)
        Sigma = Sigma + f(a+(2*i-1)*(b-a)/pow2(k) );
    end
    T(1,k+1) = (1/2)*(T(1,k)+(b-a)/pow2(k-1) * Sigma ); 
    
    for j=1:k
        T(j+1,k-j+1) = (power(4,j)*T(j,k-j+1+1)-T(j,k-j+1))/(power(4,j)-1);
    end

    err= abs(T(k+1,1)-T(k,1));%误差为该次迭代的首元和上一次迭代首元的差
    y = T(k+1,1);
end
    disp(y)
    return
end


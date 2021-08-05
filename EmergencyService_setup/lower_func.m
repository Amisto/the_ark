function F = lower_func(z)
F = zeros(18,1);
points = csvread("input.csv");
x = points(:, 2);
y = points(:, 3);
y = 1./y;
for i = 1:6
    F(3*(i-1)+1) = z(3*(i-1)+1) * exp(-z(3*(i-1)+2) * x(i)^2) + z(3*(i-1)+3) - y(i);
    F(3*(i-1)+2) = z(3*(i-1)+1) * exp(-z(3*(i-1)+2) * x(i+6)^2) + z(3*(i-1)+3) - y(i+6);
    F(3*(i-1)+3) = z(3*(i-1)+1) * exp(-z(3*(i-1)+2) * x(i+12)^2) + z(3*(i-1)+3) - y(i+12);
end
end


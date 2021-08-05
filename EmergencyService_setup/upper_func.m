function outputarg = upper_func()
fun = @lower_func;
x0 = zeros(18, 1);
options = optimoptions('fsolve','Algorithm','levenberg-marquardt');
outputarg = fsolve(fun, x0, options);
writematrix(outputarg, "output.txt");
end


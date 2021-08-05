function show()
coefs=upper_func();
x = 0:0.001:100;
y1 = coefs(1) * exp(-coefs(2)*x.^2)+coefs(3);
plot(x, y1);
y2 = coefs(4) * exp(-coefs(5)*x.^2)+coefs(6);
y3 = coefs(7) * exp(-coefs(8)*x.^2)+coefs(9);
y4 = coefs(10) * exp(-coefs(11)*x.^2)+coefs(12);
y5 = coefs(13) * exp(-coefs(14)*x.^2)+coefs(15);
y6 = coefs(16) * exp(-coefs(17)*x.^2)+coefs(18);
plot(x, y1, x, y2, x,y3,x,y4,x,y5,x,y6,x, (y1+ y2+y3+y4+y5+y6))
end


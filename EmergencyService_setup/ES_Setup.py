from scipy import optimize as opt
import csv
import numpy


def lower_func(z):  # 6x3 equations for each distribution
    out = numpy.zeros(18)
    x = []
    y = []
    with open("input.csv") as inp_file:
        file_reader = csv.reader(inp_file, delimiter=",")
        for row in file_reader:
            x.append(int(row[1]))
            y.append(float(row[2]))
    cc = 0
    while cc != len(y):
        y[cc] = 1 / y[cc]
        cc += 1
    cc = 1
    while cc != 7:
        out[3*(cc-1)] = z[3*(cc-1)] * numpy.exp((-1) * z[3*(cc-1)+1] * x[cc - 1]**2) + z[3*(cc-1)+2] - y[cc-1]
        out[3*(cc-1)+1] = z[3*(cc-1)] * numpy.exp((-1) * z[3*(cc-1)+1] * x[cc + 5]**2) + z[3*(cc-1)+2] - y[cc + 5]
        out[3*(cc-1)+2] = z[3*(cc-1)] * numpy.exp((-1)*z[3*(cc-1)+1] * x[cc + 11]**2) + z[3*(cc-1)+2] - y[cc + 11]
        cc += 1
    return out


z0 = numpy.zeros(18)
coefficients = opt.fsolve(lower_func, z0, factor=0.1)
#print(coefficients)
out_file = open("output.txt", "w")
ccc = 0
while ccc != 18:
    out_file.write(str(coefficients[ccc]))
    out_file.write('\n')
    ccc += 1
out_file.close()

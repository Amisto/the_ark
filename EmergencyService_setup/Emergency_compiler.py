from scipy import optimize as opt
import csv
import numpy


def parse_texts(inp_f_name, out_f_name):
    inp = open(inp_f_name, encoding='utf-8')
    out = open(out_f_name, 'a', encoding='utf-8')
    out.write('\n' + "///" + '\n' + inp_f_name[19] + '\n')
    severity_names = ("!NEGLIGIBLE", "!LIGHT", "!MEDIUM", "!SEVERE", "!DISASTROUS", "!CATASTROPHIC")
    severity_check = ["!NEGLIGIBLE", "!LIGHT", "!MEDIUM", "!SEVERE", "!DISASTROUS", "!CATASTROPHIC"]  # throws
    # "ValueError: '%severity_name%' is not in list" exception if there are two or more identical severities in
    # a text file
    for line in inp:
        for str_name in severity_names:
            if line.find(str_name) != -1:
                try:
                    severity_check.pop(severity_check.index(str_name))
                except ValueError:
                    print('\n' + "TWO OR MORE BLOCKS OF " + str_name + " SEVERITY IN " + inp_f_name)
                    print("ONLY ONE IS ALLOWED")
                    raise SystemExit(1)
                out.write("//" + '\n' + str_name[1] + '\n')

        if line[0] == "#":
            second_mark = line.find("#", 2)
            if second_mark == -1:
                print("INCORRECT DATA LINE IN " + inp_f_name)
                raise SystemExit(1)
            acc_id = line[1]
            for i in range(2, second_mark):
                acc_id += line[i]
            out.write('#' + '\n' + acc_id + '\n')
            line = line[second_mark + 1:]
            line_length = len(line)
            if line_length != 0:
                if line_length % 2 == 0:
                    print("INCORRECT DATA LINE WITH ID " + acc_id + " IN " + inp_f_name)
                    out.close()
                    open(out_f_name, 'w', encoding='utf-8')
                    raise SystemExit(1)
                else:
                    cc = 0
                    while cc + 2 < len(line):
                        first = -1
                        second = -1
                        if line[cc] == "T":
                            first = 0
                        elif line[cc] == "B":
                            first = 1
                        elif line[cc] == "M":
                            first = 2
                        elif line[cc] == "N":
                            first = 3
                        elif line[cc] == "E":
                            first = 4
                        elif line[cc] == "S":
                            first = 5
                        if line[cc + 1] == "n":
                            second = 0
                        elif line[cc + 1] == "l":
                            second = 1
                        elif line[cc + 1] == "m":
                            second = 2
                        elif line[cc + 1] == "s":
                            second = 3
                        elif line[cc + 1] == "d":
                            second = 4
                        elif line[cc + 1] == "c":
                            second = 5
                        out.write(str(first) + str(second) + '\n')
                        cc += 2
                    out.write("##" + '\n')

    if len(severity_check) != 0:
        print("SEVERITY ", severity_check[0], " IS MISSING IN", inp_f_name)
        out.close()
        open(out_f_name, 'w', encoding='utf-8')
        raise SystemExit(1)

    out.close()
    inp.close()
    return


def functions_for_fsolve(z):  # 6x3 equations for each distribution
    out = numpy.zeros(18)
    x = []
    y = []
    with open("input.csv") as inp_file:
        file_reader = csv.reader(inp_file, delimiter=",")
        for row in file_reader:
            x.append(int(row[1]))
            y.append(float(row[2]))

    for it in range(len(y)):
        y[it] = 1 / y[it]

    if sum(y[0:5]) > 1 or sum(y[6:11]) > 1 or sum(y[12:18]) > 1:
        print("ERROR: Total probability is higher than 1! \n Check input.csv parameters.\n")
        raise SystemExit(1)

    for cc in range(1, 7):
        out[3*(cc-1)] = z[3*(cc-1)] * numpy.exp((-1) * z[3*(cc-1)+1] * x[cc - 1]**2) + z[3*(cc-1)+2] - y[cc-1]
        out[3*(cc-1)+1] = z[3*(cc-1)] * numpy.exp((-1) * z[3*(cc-1)+1] * x[cc + 5]**2) + z[3*(cc-1)+2] - y[cc + 5]
        out[3*(cc-1)+2] = z[3*(cc-1)] * numpy.exp((-1)*z[3*(cc-1)+1] * x[cc + 11]**2) + z[3*(cc-1)+2] - y[cc + 11]
    return out


z0 = numpy.zeros(18)
coefficients = opt.fsolve(functions_for_fsolve, z0, factor=0.1)
out_file = open("coefficients.txt", "w")
for lines in coefficients:
    out_file.write(str(lines) + "\n")
out_file.close()

of = "e_ids.txt"
out_f = open(of, 'w')
out_f.close()
a = "../texts/emergency/"
parse_texts(a + "Technical.txt", of)
parse_texts(a + "Biological.txt", of)
parse_texts(a + "Medical.txt", of)
parse_texts(a + "Navigation.txt", of)
parse_texts(a + "Emergency.txt", of)
parse_texts(a + "Social.txt", of)
out_f = open(of, 'a')
out_f.write('\n' + "!@#")
out_f.close()

from matplotlib import pyplot as plt
import csv

# объявление массивов
YEARS = []
PEOPLE = []
CHILDREN = []
ADULTS = []
OLD = []

TECHNICAL_SERVICE = []
BIOLOGICAL_SERVICE = []
MEDICAL_SERVICE = []
NAVIGATION_SERVICE = []
EMERGENCY_SERVICE = []
SOCIAL_SERVICE = []

# чтение файла и заполнение данных
with open("../TheArk.csv", encoding="utf-8") as r_file: # тут указывается название файла
    file_reader = csv.reader(r_file, delimiter=",")
    count = 0
    for row in file_reader:
        if count == 0:
            pass
        else:
            YEARS.append(int(row[0]))
            PEOPLE.append(int(row[1]))
            CHILDREN.append(int(row[2]))
            ADULTS.append(int(row[3]))
            OLD.append(int(row[4]))
            TECHNICAL_SERVICE.append(float(row[10]))
            BIOLOGICAL_SERVICE.append(float(row[11]))
            MEDICAL_SERVICE.append(float(row[12]))
            NAVIGATION_SERVICE.append(float(row[13]))
            EMERGENCY_SERVICE.append(float(row[14]))
            SOCIAL_SERVICE.append(float(row[15]))
        count += 1

# Population
fig = plt.figure()
plt.plot(YEARS, PEOPLE, label="PEOPLE")
plt.plot(YEARS, CHILDREN, label="CHILDREN")
plt.plot(YEARS, ADULTS, label="ADULTS")
plt.plot(YEARS, OLD, label="OLD")
plt.ylabel("Number of people")
plt.xlabel("Year")
plt.legend()
plt.grid()
plt.savefig("Population.png")

# Services
fig.clf()
plt.plot(YEARS, TECHNICAL_SERVICE, label="Technical")
plt.plot(YEARS, BIOLOGICAL_SERVICE, label="Biological")
plt.plot(YEARS, MEDICAL_SERVICE, label="Medical")
plt.plot(YEARS, NAVIGATION_SERVICE, label="Navigation")
plt.plot(YEARS, EMERGENCY_SERVICE, label="Emergency")
plt.plot(YEARS, SOCIAL_SERVICE, label="Social")
plt.ylabel("State")
plt.xlabel("Year")
plt.legend()
plt.grid()
plt.savefig("Services.png")

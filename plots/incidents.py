import matplotlib.pyplot as plt

# loading data
file = open('../Logs/Emergency_Log.txt')
data0 = file.read()
data0 = data0.replace(' ', '')
data0 = data0.replace('\n', '')
data = data0.split(',')
file.close()

# init arrays
TECH_SERV = [[], []]
BIO_SERV = [[], []]
MED_SERV = [[], []]
NAV_SERV = [[], []]
EMERG_SERV = [[], []]
SOC_SERV = [[], []]

# data processing
size = len(data) // 3
for counter in range(1, size):
    year = int(data[3 * counter])
    service = data[3 * counter + 1]
    severity = int(data[3 * counter + 2])
    if service == 'TechnicalService':
        TECH_SERV[0].append(year)
        TECH_SERV[1].append(severity)

    elif service == 'BiologicalService':
        BIO_SERV[0].append(year)
        BIO_SERV[1].append(severity)

    elif service == 'MedicalService':
        MED_SERV[0].append(year)
        MED_SERV[1].append(severity)

    elif service == 'NavigationService':
        NAV_SERV[0].append(year)
        NAV_SERV[1].append(severity)

    elif service == 'EmergencyService':
        EMERG_SERV[0].append(year)
        EMERG_SERV[1].append(severity)

    elif service == 'SocialService':
        SOC_SERV[0].append(year)
        SOC_SERV[1].append(severity)

# plots
fig, axs = plt.subplots(6, 1, figsize=(12, 7))

point_size = 7

axs[0].scatter(TECH_SERV[0], TECH_SERV[1], c='b', s=point_size, label='TECH_SERV')
axs[0].set_title('Accidents')

axs[1].scatter(BIO_SERV[0], BIO_SERV[1], c='g', s=point_size, label='BIO_SERV')

axs[2].scatter(MED_SERV[0], MED_SERV[1], c='r', s=point_size, label='MED_SERV')
axs[2].set_ylabel('Severity')

axs[3].scatter(NAV_SERV[0], NAV_SERV[1], c='c', s=point_size, label='NAV_SERV')

axs[4].scatter(EMERG_SERV[0], EMERG_SERV[1], c='m', s=point_size, label='EMERG_SERV')

axs[5].scatter(SOC_SERV[0], SOC_SERV[1], c='y', s=point_size, label='SOC_SERV')
axs[5].set_xlabel('Year')

for i in range(len(axs) - 1):
    axs[i].set_xticklabels([])

fig.legend(loc='lower center', ncol=6)

plt.savefig('incidents.png')


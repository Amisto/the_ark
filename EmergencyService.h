//
// Created by Amisto on 4/2/2021.
//

#ifndef THE_ARK_EMERGENCYSERVICE_H
#define THE_ARK_EMERGENCYSERVICE_H

#include <ctime>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <fstream>

#include "Service.h"

using std::array;
using std::setw;
using std::cout;
using std::endl;


class EmergencyService : public Service
{
private:
    double state;

    unsigned int staff; // Current staff amount
    unsigned int max_staff;

    unsigned int resources;
    unsigned int max_resources;
    unsigned int Junk;

    // Coefficients for probabilities of accidents depending on
    // service's state
    array< array<double, 3>, 6> distribution_coefficients;

    // Accidents' output
    std::ofstream emergency_log;
    const unsigned short CELL_WIDTH = 11;
    const unsigned short CELL_WIDTH_S = 20;

    // From 0 to 1, controls the impact of this Service on accident's severity
    const double EFFECTIVE_STATE_RATIO = 0.4; // Higher ratio — lower dependence on Emergency service's state

    void create_accident(Service* s);

public:
    EmergencyService();
    ~EmergencyService() override;

    void process_accident(AccidentSeverity as) override;    // каждая служба должна уметь в своих терминах обработать переданную ей аварию
    void process_year() override;                   // если у службы есть какая-то личная жизнь, она может заниматься ей тут
    double getState() override;                             // каждая служба должна уметь вернуть свое состояние в процентах, посчитав его в своих терминах
    void setState(double s) override;                       // функция для инициализации, каждая служба должна уметь получить состояние в процентах и пересчитать  его в своих терминах
    unsigned int getStaffDemand() override;
    unsigned int getResourceDemand() override;
    unsigned int returnJunk() override;

    void changeResources(int delta);
    void killStaff(int delta); // Has protection from delta > current Staff value

};


#endif //THE_ARK_EMERGENCYSERVICE_H

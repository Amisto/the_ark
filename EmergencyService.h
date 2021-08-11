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
#include "TheArk.h"
#include "Interface.h"

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
    array<array<double, 3>, 6> distribution_coefficients = {0};

    // Accidents' output
    std::ofstream emergency_log;
    const unsigned short CELL_WIDTH = 11;
    const unsigned short CELL_WIDTH_S = 20;

    // Controls the impact of this Service on accident's severity
    // Higher ratio — lower dependence on Emergency service's state,
    // while creating accident
    const double EFFECTIVE_STATE_RATIO = constFieldInit<double>("EFFECTIVE_STATE_RATIO", 0, 1, 0.5);

    void create_accident(Service* s);

    // This template is used for initializing const coefficients
    // Use any lower_bound == upper_bound if there are no limitations
    template<typename T>
    T constFieldInit(const std::string& name, double lower_bound, double upper_bound, double default_value) const
    {
        T result = (T)(std::stod(TheArk::get_instance()->getInterface()->getServices()
                [Emergency_Service][name]));

        if (lower_bound == upper_bound) {
            return result;
        }
        else if (upper_bound < lower_bound) {
            std::cerr << "EmergencyService error 1: incorrect constFieldInit(...) boundaries.";
        }
        else if (result < lower_bound or result > upper_bound) {
            std::cerr << "EmergencyService error 2: incorrect " << name << " value.";
        }
        else {
            return result;
        }
        std::cerr << endl << "Default value of " << default_value << " has been set." << endl;
        return (T)default_value;
    }

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

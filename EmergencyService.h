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
using std::endl;


class EmergencyService : public Service
{
private:
    // Shows how many resources service needs, 100% — enough, 0 — nothing
    double resources_state;
    // Shows how many staff members service needs, 100% — enough, 0 — nothing
    double staff_state;
    // Shows the state of tools, which are used during the emergency situations
    double tools_state;
    // Mean value of other states
    double system_state;

    // Shows how effective repairing of tools is for a full team
    const unsigned short REPAIR_PERCENT_PER_YEAR = constFieldInit<unsigned short>("REPAIR_PERCENT_PER_YEAR", 0, 100.0, 5);

    // Every year tool's state will decrease by this amount
    const double ANNUAL_DEGRADATION = constFieldInit<double>("ANNUAL_DEGRADATION", -100.0, 0, -2);

    // Current staff amount
    unsigned int staff;
    // Staff required for 100% staff_state
    unsigned int required_staff;

    // Resources per year required for 100% resource_staff
    unsigned int need_resources;
    unsigned int junk;

    // Coefficients for probabilities of accidents depending on
    // service's state
    array<array<double, 3>, 6> distribution_coefficients = {0};

    // Accidents' output
    std::ofstream emergency_log;
    const char CELL_WIDTH = 11;
    const char CELL_WIDTH_S = 20;

    // Controls the impact of this Service on accident's severity
    // Higher ratio — lower dependence on Emergency service's state,
    // while creating accident
    const double EFFECTIVE_STATE_RATIO = constFieldInit<double>("EFFECTIVE_STATE_RATIO", 0, 1, 0.5);

    void create_accident(Service* s);

    // This template is used for initializing const coefficients
    // Use any lower_bound == upper_bound if there are no limitations
    template<typename T>
    [[nodiscard]] T constFieldInit(const std::string& name, double lower_bound, double upper_bound, double default_value) const
    {
        T result = (T)(std::stod(TheArk::get_instance()->getInterface()->getServices()
                [Emergency_Service][name]));

        if (upper_bound < lower_bound) {
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

    void killStaff(int delta); // Has protection from delta > current Staff value

};


#endif //THE_ARK_EMERGENCYSERVICE_H

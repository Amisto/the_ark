//
// Created by Amisto on 4/2/2021.
//

#ifndef THE_ARK_NAVIGATIONSERVICE_H
#define THE_ARK_NAVIGATIONSERVICE_H

#include <random>
#include <chrono>
#include <algorithm>
#include <cmath>

#include "Interface.h"
#include "Service.h"
#include "NavigationBlock.h"
#include "TheArk.h"

using std::unique_ptr;
using std::clog;

enum FlightStage
{
    ACCELERATION,
    STABLE,
    MANEUVER
};

class NavigationService : public Service {
private:
    // Total amount of workers here
    unsigned int staff;

    // Hiring more people than this value * default
    // required_staff is useless for upgrades //CURRENTLY DISABLED
    //const double MAX_STAFF_RATIO = 1.3;

    // Basically Population / 8, but can be higher
    // More people — faster repair, higher efficiency
    unsigned int required_staff;

    // Starting population / this coef. is default staff number
    const unsigned short DEFAULT_STAFF_DENOMINATOR = 1.0 /
            constFieldInit<double>("Propotion_of_people", 0.001, 1, 0.15);

    // Staff required for fully functioning systems
    unsigned DEFAULT_STAFF;

    // Value in percents from initial amount of years
    const double MINIMAL_FLIGHT_TIME = constFieldInit<double>("MINIMAL_FLIGHT_TIME", 0.1, 1000, 90);

    // Shows how effective repairing is for a full team
    const unsigned short REPAIR_PERCENT_PER_YEAR = constFieldInit<unsigned short>("REPAIR_PERCENT_PER_YEAR", 0, NavDevAMOUNT * 100.0, 20);

    // For resources management
    unsigned int need_resources;
    unsigned int junk;

    // Stages' of the flight management
    FlightStage stage;
    unsigned short time_until_next_stage;
    FlightStage next_stage;

    // First and last years of flight are marked as ACCEL.
    const unsigned short ACCELERATION_TIME = 3;

    array<unique_ptr<NavigationBlock>, NavDevAMOUNT> devices;
    // Flag for process_year optimization //CURRENTLY DISABLED
    //bool changed_efficiency = false;

    // Is used with the next field
    //double gained_efficiency_points = 0; //CURRENTLY DISABLED
    // E.g. this value = 10, total efficiency of all devices
    // is always lower than 110%
    //const unsigned short MAX_UPGRADE_POINTS = 20; //CURRENTLY DISABLED
    //
    //const double MAX_UPGRADE_POINTS_PER_YEAR = 2; //CURRENTLY DISABLED

    // Shows gained or lost time due to the work
    // of devices, < 0 means faster flight
    double years_delta;

    // If total efficiency * state > this value
    // then total distance of flight decreases
    const double MINIMAL_PRODUCTIVITY = 0.35;

    // Total years amount is changing if years_delta is higher than this value
    const unsigned short BASIC_DELTA = 2;

    // If years_total is higher this value times,
    // there will be a warning to prevent from endless
    // travelling
    unsigned short int LOST_THE_WAY_WARNING = constFieldInit<unsigned short>("LOST_THE_WAY_WARNING", 2, 1000, 3);

    // Every year each device's state will decrease by this amount
    const double ANNUAL_DEGRADATION = constFieldInit<double>("ANNUAL_DEGRADATION", NavDevAMOUNT*(-100.0), 0, -2);

    // This template is used for initializing const coefficients
    // Use any lower_bound == upper_bound if there are no limitations
    template<typename T>
    [[nodiscard]] T constFieldInit(const std::string& name, double lower_bound, double upper_bound, double default_value) const
    {
        T result = (T)(std::stod(TheArk::get_instance()->getInterface()->getServices()
                [Navigation_Service][name]));

        if (upper_bound < lower_bound) {
            std::cerr << "NavigationService error 1: incorrect constFieldInit(...) boundaries.";
        }
        else if (result < lower_bound or result > upper_bound) {
            std::cerr << "NavigationService error 2: incorrect " << name << " value.";
        }
        else {
            return result;
        }
        std::cerr << endl << "Default value of " << default_value << " has been set." << endl;
        return (T)default_value;
    }

public:
    NavigationService();

    void process_accident(AccidentSeverity as)  override; // каждая служба должна уметь в своих терминах обработать переданную ей аварию
    void process_year()  override;                        // если у службы есть какая-то личная жизнь, она может заниматься ей тут
    [[nodiscard]] double getState()  override;                          // каждая служба должна уметь вернуть свое состояние в процентах, посчитав его в своих терминах
    void setState(double s)  override;                    // функция для инициализации, каждая служба должна уметь получить состояние в процентах и пересчитать его в своих терминах

    [[nodiscard]] unsigned int getStaffDemand() override;
    [[nodiscard]] unsigned int getResourceDemand() override;
    unsigned int returnJunk() override;

    // Kills some staff members. Var victims HAS to be less than staff!
    void killStaff(unsigned victims);
};

#endif //THE_ARK_NAVIGATIONSERVICE_H

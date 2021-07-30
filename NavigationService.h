//
// Created by Amisto on 4/2/2021.
//

#ifndef THE_ARK_NAVIGATIONSERVICE_H
#define THE_ARK_NAVIGATIONSERVICE_H

#include <random>
#include <chrono>
#include <algorithm>

#include "Service.h"
#include "TheArk.h"
#include "Resources.h"
#include "Interface.h"
#include "NavigationBlock.h"
#include "RandomNumberGenerator.h"

using std::unique_ptr;

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

    // Basically Population / 8, but can be higher
    // More people — faster repair, higher efficiency
    unsigned int required_staff;

    // Shows how effective repairing is for a full team
    const unsigned short REPAIR_PERCENT_PER_YEAR = 10;

    unsigned int need_resources;

    // Stages' of the flight management
    FlightStage stage;
    unsigned short time_until_next_stage;
    FlightStage next_stage;

    // First and last years of flight are marked as ACCEL.
    const unsigned short ACCELERATION_TIME = 3;

    array<unique_ptr<NavigationBlock>, NavDevAMOUNT> devices;
    // Flag for process_year optimization
    bool changed_efficiency = false;

    // Shows gained or lost time due to the work
    // of devices, < 0 means faster flight
    double years_delta;

    // Starting population / this coef. is default staff number
    const char DEFAULT_STAFF_DENOMINATOR = 9;

    // If years_total is higher this value times,
    // there will be a warning to prevent from endless
    // travelling
    unsigned short int LOST_THE_WAY_WARNING = 3;

    // Every year each device's state will decrease by this amount
    const double ANNUAL_DEGRADATION = -0.5;

public:
    NavigationService();

    void process_accident(AccidentSeverity as)  override; // каждая служба должна уметь в своих терминах обработать переданную ей аварию
    void process_year()  override;                        // если у службы есть какая-то личная жизнь, она может заниматься ей тут
    [[nodiscard]] double getState()  override;                          // каждая служба должна уметь вернуть свое состояние в процентах, посчитав его в своих терминах
    void setState(double s)  override;                    // функция для инициализации, каждая служба должна уметь получить состояние в процентах и пересчитать  его в своих терминах

    [[nodiscard]] unsigned int getStaffDemand() override;
    [[nodiscard]] unsigned int getResourceDemand() override;
    unsigned int returnJunk() override;

    // Kills some of staff. Victims HAS to be less than staff!
    void killStaff(unsigned victims);

    [[nodiscard]] bool isChangedEfficiency() const;
    void setChangedEfficiency(bool new_changed_efficiency);
};

#endif //THE_ARK_NAVIGATIONSERVICE_H

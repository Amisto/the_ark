//
// Created by Amisto on 4/2/2021.
//

#ifndef THE_ARK_NAVIGATIONSERVICE_H
#define THE_ARK_NAVIGATIONSERVICE_H

#include <memory>
#include <random>
#include <chrono>
#include <algorithm>
#include <iostream>

#include "Service.h"
#include "TheArk.h"
#include "Resources.h"
#include "Interface.h"

using std::unique_ptr;
using std::default_random_engine;
using std::uniform_real_distribution;
using std::cout;
using std::endl;

enum FlightStage
{
    ACCELERATION,
    STABLE,
    MANEUVER
};

enum NavigationDevices
{
    INS,
    OPTICAL,
    COMP,
    RADIO,
    MAGNETIC,
    NavDevAMOUNT
};

class NavigationBlock
{
private:
    float state;
    unsigned int consumption;  // Resources per year

    // Some devices are much more useful than other,
    // Can change during the flight
    float efficiency;

public:
    explicit NavigationBlock(NavigationDevices type);

    // Basic numbers for efficiency of each device
    unsigned short getDefaultEfficiency(NavigationDevices type);

    [[nodiscard]] float getState() const;
    [[nodiscard]] unsigned int getConsumption() const;
    [[nodiscard]] float getEfficiency() const;

    void setState(float new_state);     // Has protection form invalid value
    void setConsumption(unsigned new_consumption);
    void setEfficiency(float new_efficiency);

    void changeState(float delta); // Delta can be < 0, has protection form invalid value

    // Points + current state can be > 100, method returns unused points back
    float repairDevice(float available_points);

};

class CompareDevicesEfficiency{
public:
    bool operator() (const unique_ptr<NavigationBlock>& b1,
                     const unique_ptr<NavigationBlock>& b2)
    {
        return b1->getEfficiency() < b2->getEfficiency();
    }
};

class CompareDevicesState{
public:
    bool operator() (const unique_ptr<NavigationBlock>& b1,
                     const unique_ptr<NavigationBlock>& b2)
    {
        return b1->getState() < b2->getState();
    }
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
    float years_delta;

    // Starting population / this coef. is default staff number
    const char DEFAULT_STAFF_DENOMINATOR = 9;

    // If years_total is higher this value times,
    // there will be a warning to prevent from endless
    // travelling
    unsigned short int LOST_THE_WAY_WARNING = 3;

    // Every year each device's state will decrease by this amount
    const float ANNUAL_DEGRADATION = -0.5;

public:
    NavigationService();

    void process_accident(AccidentSeverity as)  override; // каждая служба должна уметь в своих терминах обработать переданную ей аварию
    void process_year()  override;                        // если у службы есть какая-то личная жизнь, она может заниматься ей тут
    [[nodiscard]] double getState()  override;                          // каждая служба должна уметь вернуть свое состояние в процентах, посчитав его в своих терминах
    void setState(double s)  override;                    // функция для инициализации, каждая служба должна уметь получить состояние в процентах и пересчитать  его в своих терминах

    [[nodiscard]] unsigned int getStaffDemand() override;
    [[nodiscard]] unsigned int getResourceDemand() override;

    // Kills some of staff. Victims HAS to be less than staff!
    void killStaff(unsigned victims);

    // Random numbers generators
    float getRandomFloat(float min, float max);
    int getRandomInt(int min, int max);

    [[nodiscard]] bool isChangedEfficiency() const;
    void setChangedEfficiency(bool changedEfficiency);
};

#endif //THE_ARK_NAVIGATIONSERVICE_H

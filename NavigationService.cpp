//
// Created by Amisto on 4/2/2021.
//

#include "NavigationService.h"

NavigationService::NavigationService(): staff(0), need_resources(0), years_delta(0)
{
    required_staff = required_staff = std::stoi(
            TheArk::get_instance()->getInterface()->getGeneral()["Population"]) / DEFAULT_STAFF_DENOMINATOR;
    stage = ACCELERATION;
    time_until_next_stage = ACCELERATION_TIME;
    next_stage = STABLE;

    for (auto i = 0; i < NavDevAMOUNT; i++)
        devices[i] =
                std::make_unique<NavigationBlock>(static_cast<NavigationDevices>(i));
}

void NavigationService::killStaff(unsigned int victims)
{
    list<shared_ptr<Human>>& my_staff =
            TheArk::get_instance()->getPopulation()->getServiceStaff(Navigation_Service);

    auto it = my_staff.begin();
    for (int i = 0; i < victims; i++)
    {
        (*it)->setIsAlive(false);
        it++;
    }
}

// NEGLIGIBLE — lost the way for some time
// LIGHT — lost some staff and some time
// MEDIUM — lost some staff and some device's health
// SEVERE — lost staff, devices' health and time
// DISASTROUS — lost a lot of staff, devices and probably time
// CATASTROPHIC — lost almost everything
void NavigationService::process_accident(AccidentSeverity as)
{
    return; // DISABLED UNTIL EMERGENCY IS FIXED
    unsigned killed_staff = 0;
    switch (as)
    {
        case NEGLIGIBLE:
            stage = MANEUVER;
            time_until_next_stage = 5;
            next_stage = STABLE;
            TheArk::get_instance()->setYearsTotal(
                    TheArk::get_instance()->getYearsTotal() + time_until_next_stage);

        case LIGHT:
            killed_staff = staff / 15;
            stage = MANEUVER;
            time_until_next_stage = 10;
            next_stage = STABLE;
            TheArk::get_instance()->setYearsTotal(
                    TheArk::get_instance()->getYearsTotal() + time_until_next_stage);

        case MEDIUM:
            killed_staff = staff / 10;
            devices[TheArk::get_instance()->getRandomGenerator()->getRandomInt(0, 5)]->changeState(
                    TheArk::get_instance()->getRandomGenerator()->getRandomDouble(-10.f, -5.f));

        case SEVERE:
            killed_staff = staff / 5;
            stage = MANEUVER;
            time_until_next_stage = 20;
            next_stage = STABLE;
            TheArk::get_instance()->setYearsTotal(
                    TheArk::get_instance()->getYearsTotal() + time_until_next_stage);
            devices[TheArk::get_instance()->getRandomGenerator()->getRandomInt(0, 5)]->changeState(
                    TheArk::get_instance()->getRandomGenerator()->getRandomDouble(-18.f, -10.f));

        case DISASTROUS:
            killed_staff = staff / 2;
            for (auto i = 0; i < 4; i++) {
                devices[TheArk::get_instance()->getRandomGenerator()->getRandomInt(0, 5)]->changeState(
                        TheArk::get_instance()->getRandomGenerator()->getRandomDouble(-60.f, -30.f));
            }
            if (TheArk::get_instance()->getRandomGenerator()->getRandomInt(0, 2) == 1) {
                stage = MANEUVER;
                time_until_next_stage = 20;
                next_stage = STABLE;
                TheArk::get_instance()->setYearsTotal(
                        TheArk::get_instance()->getYearsTotal() + time_until_next_stage);
            }

        case CATASTROPHIC:
            killed_staff = staff * 0.9;
            for (auto i = 0; i < 6; i++) {
                devices[TheArk::get_instance()->getRandomGenerator()->getRandomInt(0, 4)]->changeState(
                        TheArk::get_instance()->getRandomGenerator()->getRandomDouble(-90.f, -60.f));
            }
            stage = MANEUVER;
            time_until_next_stage = 50;
            next_stage = STABLE;
            TheArk::get_instance()->setYearsTotal(
                    TheArk::get_instance()->getYearsTotal() + time_until_next_stage);
    }

    killStaff(killed_staff);
    staff -= killed_staff;
}


void NavigationService::process_year()
{
    // Block for flight stage management
    time_until_next_stage--;
    if (not time_until_next_stage) {
        stage = next_stage;

        if (stage == STABLE) {
            time_until_next_stage = TheArk::get_instance()->getYearsTotal() -
                    ACCELERATION_TIME;
            next_stage = ACCELERATION;
        }
        else if (stage == ACCELERATION)  {
            time_until_next_stage = ACCELERATION_TIME;
            next_stage = STABLE;
        }
    }

    // years_delta section
    double effective_sum = 0; // Sum for all devices' performance
    for (auto & it : devices)
        effective_sum += it->getState() * it->getEfficiency() / 10000.f;

    years_delta -= effective_sum - 0.95;

    if (years_delta > 3) {
        TheArk::get_instance()->setYearsTotal(TheArk::get_instance()->getYearsTotal() + 3);
        years_delta -= 3;

        if (TheArk::get_instance()->getYearsTotal() >
            LOST_THE_WAY_WARNING
            * std::stoi(TheArk::get_instance()->getInterface()->getGeneral()["Years"])) {
                cout << "We've been travelling " << LOST_THE_WAY_WARNING << " times longer than estimated" << endl
                << "Probably we are lost forever" << endl
                << "Would you like to continue? (y/n)" << endl;
                char h = std::cin.get();
                if (h == 'n')
                    TheArk::get_instance()->setYearsTotal(TheArk::get_instance()->getCurrentYear() + 1);
                else
                    LOST_THE_WAY_WARNING += 2;
        }
    }
    else if (years_delta < -3) {
        TheArk::get_instance()->setYearsTotal(TheArk::get_instance()->getYearsTotal() - 3);
        years_delta -= -3;
    }
    //


    // Block for staff management
    if (TheArk::get_instance()->getPopulation()->getUnemployedPeople() >=
        (required_staff / 10) and required_staff < std::stoi(
            TheArk::get_instance()->getInterface()->getGeneral()["Population"]) / (DEFAULT_STAFF_DENOMINATOR - 1)) {
        required_staff += TheArk::get_instance()->getPopulation()->getUnemployedPeople() / 12;
    }
    if (staff < std::stoi(
            TheArk::get_instance()->getInterface()->getGeneral()["Population"]) / DEFAULT_STAFF_DENOMINATOR) {
        required_staff = std::stoi(
                TheArk::get_instance()->getInterface()->getGeneral()["Population"]) / DEFAULT_STAFF_DENOMINATOR;
    }

    staff = TheArk::get_instance()->getPopulation()->getServiceStaff(Navigation_Service).size();
    //

    // Block for resource management
    //

    // Block for devices
     // Efficiency upgrades
      // Adding points to the device with max state
    if(staff > std::stoi
        (TheArk::get_instance()->getInterface()->getGeneral()["Population"]) / DEFAULT_STAFF_DENOMINATOR) {
        setChangedEfficiency(true);
        double eff_points = (staff * 100.f) /
                (std::stof(TheArk::get_instance()->getInterface()->getGeneral()["Population"]) / DEFAULT_STAFF_DENOMINATOR)
                - 100;

        auto upgrd_dev = std::max_element(devices.begin(), devices.end(), CompareDevicesState());// upgraded device

        upgrd_dev->get()->setEfficiency(upgrd_dev->get()->getEfficiency() + eff_points);
    }
    else if (isChangedEfficiency()) {
        for (auto i = 0; i < NavDevAMOUNT; i++)
            devices[i]->setEfficiency(devices[i]->getDefaultEfficiency(
                    static_cast<NavigationDevices>(i)));
    }

     // Annual degradation
     for (auto & it : devices)
         it->changeState(ANNUAL_DEGRADATION);

    //

    // Repairing the most efficient device, then the one with the least state
    double CURRENT_YEAR_REPAIR_PERCENT = TheArk::get_instance()->getRandomGenerator()->getRandomDouble(0.7, 1.5)
            * REPAIR_PERCENT_PER_YEAR
            * (staff / (std::stof(
            TheArk::get_instance()->getInterface()->getGeneral()["Population"]) / DEFAULT_STAFF_DENOMINATOR));

    auto rprd_dev = std::max_element(devices.begin(), devices.end(), CompareDevicesEfficiency()); // repaired device
    CURRENT_YEAR_REPAIR_PERCENT = rprd_dev->get()->repairDevice(CURRENT_YEAR_REPAIR_PERCENT);

    if (CURRENT_YEAR_REPAIR_PERCENT) {
        rprd_dev = std::min_element(devices.begin(), devices.end(), CompareDevicesState());
        CURRENT_YEAR_REPAIR_PERCENT = rprd_dev->get()->repairDevice(CURRENT_YEAR_REPAIR_PERCENT);

        auto i = 0;
        while (i < NavDevAMOUNT and CURRENT_YEAR_REPAIR_PERCENT > 0) {
            CURRENT_YEAR_REPAIR_PERCENT = devices[i]->repairDevice(CURRENT_YEAR_REPAIR_PERCENT);
            i++;
        }
    }
    //
}

double NavigationService::getState()
{
    double result = 0;

    for (auto& it : devices)
        result += it->getState();

    return result / NavDevAMOUNT;
}

void NavigationService::setState(double s)
{
    if (not TheArk::get_instance()->getCurrentYear())
        required_staff = std::stoi(
                TheArk::get_instance()->getInterface()->getGeneral()["Population"]) / DEFAULT_STAFF_DENOMINATOR;
    for (auto& it : devices)
        it->setState(s);
}

unsigned int NavigationService::getStaffDemand() {
    if (required_staff > staff)
        return required_staff - staff;
    else
        return 0;
}

unsigned int NavigationService::getResourceDemand() {
    return 0;
}

bool NavigationService::isChangedEfficiency() const {
    return changed_efficiency;
}

void NavigationService::setChangedEfficiency(bool changedEfficiency) {
    changed_efficiency = changedEfficiency;
}

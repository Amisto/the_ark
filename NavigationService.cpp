//
// Created by Amisto on 4/2/2021.
//

#include "NavigationService.h"
#include "Resources.h"
#include "RandomNumberGenerator.h"

NavigationService::NavigationService(): staff(0), need_resources(0), years_delta(0), junk(0)
{
    DEFAULT_STAFF = std::stoi(
            TheArk::get_instance()->getInterface()->getGeneral()["Population"]) / DEFAULT_STAFF_DENOMINATOR;
    required_staff = DEFAULT_STAFF;
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
// LIGHT — lost some staff members and some time
// MEDIUM — lost some staff members and some device's health
// SEVERE — lost staff, devices' health and time
// DISASTROUS — lost a lot of staff, devices and probably time
// CATASTROPHIC — lost almost everything
void NavigationService::process_accident(AccidentSeverity as)
{
    unsigned killed_staff = 0;
    switch (as)
    {
        case NEGLIGIBLE:
            stage = MANEUVER;
            time_until_next_stage = 1;
            next_stage = STABLE;
            years_delta += time_until_next_stage;
            devices[TheArk::get_instance()->getRandomGenerator()->getRandomInt(0, 4)]->changeState(
                    TheArk::get_instance()->getRandomGenerator()->getRandomDouble(-5.f, -1.f));
            break;

        case LIGHT:
            killed_staff = staff / 20;
            stage = MANEUVER;
            time_until_next_stage = 2;
            next_stage = STABLE;
            years_delta += time_until_next_stage;
            devices[TheArk::get_instance()->getRandomGenerator()->getRandomInt(0, 4)]->changeState(
                    TheArk::get_instance()->getRandomGenerator()->getRandomDouble(-15.f, -5.f));
            break;

        case MEDIUM:
            killed_staff = staff / 10;
            devices[TheArk::get_instance()->getRandomGenerator()->getRandomInt(0, 4)]->changeState(
                    TheArk::get_instance()->getRandomGenerator()->getRandomDouble(-35.f, -15.f));
            break;

        case SEVERE:
            killed_staff = staff / 5;
            stage = MANEUVER;
            time_until_next_stage = 7;
            next_stage = STABLE;
            years_delta += time_until_next_stage;
            devices[TheArk::get_instance()->getRandomGenerator()->getRandomInt(0, 4)]->changeState(
                    TheArk::get_instance()->getRandomGenerator()->getRandomDouble(-35.f, -20.f));
            devices[TheArk::get_instance()->getRandomGenerator()->getRandomInt(0, 4)]->changeState(
                    TheArk::get_instance()->getRandomGenerator()->getRandomDouble(-40.f, -20.f));
            break;

        case DISASTROUS:
            killed_staff = staff / 2;
            for (auto i = 0; i < 4; i++) {
                devices[TheArk::get_instance()->getRandomGenerator()->getRandomInt(0, 4)]->changeState(
                        TheArk::get_instance()->getRandomGenerator()->getRandomDouble(-60.f, -30.f));
            }
            if (TheArk::get_instance()->getRandomGenerator()->getRandomInt(0, 2) == 1) {
                stage = MANEUVER;
                time_until_next_stage = 12;
                next_stage = STABLE;
                years_delta += time_until_next_stage;
            }
            break;

        case CATASTROPHIC:
            killed_staff = staff * 9 / 10;
            for (auto i = 0; i < 6; i++) {
                devices[TheArk::get_instance()->getRandomGenerator()->getRandomInt(0, 4)]->changeState(
                        TheArk::get_instance()->getRandomGenerator()->getRandomDouble(-90.f, -60.f));
            }
            stage = MANEUVER;
            time_until_next_stage = 17;
            next_stage = STABLE;
            years_delta += time_until_next_stage;
            break;
    }

    killStaff(killed_staff);
    staff -= killed_staff;
}


void NavigationService::process_year()
{
    std::clog << "NS_Years_Delta: " << years_delta << endl;
    std::clog << "Total_Y: " << TheArk::get_instance()->getYearsTotal() << endl;
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

    years_delta -= 10 * pow((effective_sum - MINIMAL_PRODUCTIVITY), 5);
    auto current_year_change = static_cast<int>(years_delta);

    if (years_delta > BASIC_DELTA) {
        TheArk::get_instance()->setYearsTotal(TheArk::get_instance()->getYearsTotal() + current_year_change);
        years_delta -= current_year_change;
    }
    else if (years_delta < -BASIC_DELTA) {
        TheArk::get_instance()->setYearsTotal(TheArk::get_instance()->getYearsTotal() + current_year_change);
        years_delta -= current_year_change;
    }

    if (TheArk::get_instance()->getCurrentYear() >
    LOST_THE_WAY_WARNING
    * std::stoi(TheArk::get_instance()->getInterface()->getGeneral()["Years"])) {
        std::cout << "We've been travelling " << LOST_THE_WAY_WARNING << " times longer than estimated" << endl
        << "Probably we are lost forever" << endl
        << "Would you like to continue? (y/n)" << endl;
        string h;
        std::cin >> h;
        if (h == "n")
            TheArk::get_instance()->setYearsTotal(TheArk::get_instance()->getCurrentYear() - 1);
        else
            LOST_THE_WAY_WARNING += 2;
    }
    if (std::stoi(TheArk::get_instance()->getInterface()->getGeneral()["Years"]) * 0.9 >
        TheArk::get_instance()->getYearsTotal())
        TheArk::get_instance()->setYearsTotal(std::stoi(TheArk::get_instance()->getInterface()->getGeneral()["Years"]) * 9 / 10);
    //


    // Block for staff management
    // CURRENTLY DISABLED
//    if (staff < DEFAULT_STAFF) {
//        required_staff = DEFAULT_STAFF;
//    }
//    else if (TheArk::get_instance()->getPopulation()->getUnemployedPeople() >=
//        (required_staff / 15) and required_staff < MAX_STAFF_RATIO * DEFAULT_STAFF) {
//        required_staff += TheArk::get_instance()->getPopulation()->getUnemployedPeople() / 8;
//    }

    staff = TheArk::get_instance()->getPopulation()->getServiceStaff(Navigation_Service).size();
    //

    // Block for resource management
    // CURRENTLY DISABLED BECAUSE OF STRANGE BEHAVIOUR IN RESOURCES.CPP
//    auto used_currently = TheArk::get_instance()->getResources()->getUsedByService(Navigation_Service);
//    junk = used_currently;
//    clog << "Used res: " << used_currently << endl;
//    if (used_currently < need_resources)
//        devices[TheArk::get_instance()->getRandomGenerator()->getRandomInt(0, 4)]->changeState(
//                TheArk::get_instance()->getRandomGenerator()->
//                getRandomDouble(-100 * (1.0 - used_currently * 1.0 / need_resources), 0.0));
//    need_resources = staff;
    //
    //

    // Block for devices
      // Efficiency upgrades: CURRENTLY DISABLED 'CAUSE STAFF CANNOT BE
      // HIGHER THAN DEFAULT VALUE
        // Adding points to the device with max state
//    if(staff > DEFAULT_STAFF) {
//        if (gained_efficiency_points != MAX_UPGRADE_POINTS)
//        {
//            setChangedEfficiency(true);
//            double x;
//            if (staff < MAX_STAFF_RATIO * DEFAULT_STAFF)
//                x = (staff * 1.0) / DEFAULT_STAFF;
//            else
//                x = MAX_STAFF_RATIO;
//            double current_max_points = MAX_UPGRADE_POINTS * pow((x - 1) / (MAX_STAFF_RATIO - 1), 0.1);
//
//            double eff_points = MAX_UPGRADE_POINTS_PER_YEAR * pow((x - 1) / (MAX_STAFF_RATIO - 1), 0.3);
//            if (staff >= MAX_STAFF_RATIO * DEFAULT_STAFF)
//                eff_points = MAX_UPGRADE_POINTS_PER_YEAR;
//            if (eff_points + gained_efficiency_points < current_max_points) {
//                gained_efficiency_points += eff_points;
//            }
//            else {
//                eff_points = current_max_points - gained_efficiency_points;
//                gained_efficiency_points = current_max_points;
//            }
//
//            auto upgrd_dev = std::max_element(devices.begin(), devices.end(), CompareDevicesState());// upgraded device
//            upgrd_dev->get()->setEfficiency(upgrd_dev->get()->getEfficiency() + eff_points);
//        }
//    }
//    else if (isChangedEfficiency()) {
//        for (auto i = 0; i < NavDevAMOUNT; i++)
//            devices[i]->setEfficiency(devices[i]->getDefaultEfficiency(
//                    static_cast<NavigationDevices>(i)));
//        gained_efficiency_points = 0;
//    }

     // Annual degradation
     for (auto & it : devices)
         it->changeState(ANNUAL_DEGRADATION);

    //

    // Repairing the most efficient device, then the one with the least state
    double CURRENT_YEAR_REPAIR_PERCENT = TheArk::get_instance()->getRandomGenerator()->getRandomDouble(0.7, 1.5)
            * REPAIR_PERCENT_PER_YEAR * staff / DEFAULT_STAFF;

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

void NavigationService::setState(double s) {
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
    return need_resources;
}

unsigned int NavigationService::returnJunk() {
    return junk;
}


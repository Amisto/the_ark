//
// Created by Amisto on 4/2/2021.
//

#include "NavigationService.h"
#include "Resources.h"
#include "RandomNumberGenerator.h"

NavigationService::NavigationService(): staff(0), required_resources(0), junk(0), distance_delta(0)
{
    speed = 0;
    acceleration = 1.0 / ACCELERATION_TIME;
    distance_left = INITIAL_DISTANCE;
    DEFAULT_STAFF = std::stoi(
            TheArk::get_instance()->getInterface()->getGeneral()["Population"]) / DEFAULT_STAFF_DENOMINATOR;
    required_staff = DEFAULT_STAFF;
    stage = ACCELERATION;

    for (auto i = 0; i < NavDevAMOUNT; i++)
        devices[i] =
                std::make_unique<NavigationBlock>(static_cast<NavigationDevices>(i));
}

void NavigationService::killStaff(unsigned int victims)
{
    list<shared_ptr<Human>>& my_staff =
            TheArk::get_instance()->getPopulation()->getServiceStaff(Navigation_Service);
    auto it = my_staff.begin();
    for (int i = 0; i < victims;)
    {
        if ((*it)->isAlive()) {
            (*it)->setIsAlive(false);
            i++;
        }
        it++;
    }
    staff -= victims;
}

// NEGLIGIBLE — lost some time
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
            speed -= speed * TheArk::get_instance()->getRandomGenerator()->getRandomFloat(0.01, 0.04);
            devices[TheArk::get_instance()->getRandomGenerator()->getRandomInt(0, 4)]->changeState(
                    TheArk::get_instance()->getRandomGenerator()->getRandomDouble(-5.0, -1.0));
            break;

        case LIGHT:
            killed_staff = staff /  TheArk::get_instance()->getRandomGenerator()->getRandomInt(19, 22);
            devices[TheArk::get_instance()->getRandomGenerator()->getRandomInt(0, 4)]->changeState(
                    TheArk::get_instance()->getRandomGenerator()->getRandomDouble(-15.0, -5.0));
            break;

        case MEDIUM:
            killed_staff = staff / TheArk::get_instance()->getRandomGenerator()->getRandomInt(10, 15);
            distance_left++;
            speed -= speed * TheArk::get_instance()->getRandomGenerator()->getRandomFloat(0.10, 0.20);
            devices[TheArk::get_instance()->getRandomGenerator()->getRandomInt(0, 4)]->changeState(
                    TheArk::get_instance()->getRandomGenerator()->getRandomDouble(-35.0, -15.0));
            break;

        case SEVERE:
            killed_staff = staff / TheArk::get_instance()->getRandomGenerator()->getRandomInt(6, 9);
            speed -= speed * TheArk::get_instance()->getRandomGenerator()->getRandomFloat(0.10, 0.20);
            if (stage != MANEUVER)
                stage = MANEUVER;
            maneuvering_time_left += 4;
            distance_left++;
            devices[TheArk::get_instance()->getRandomGenerator()->getRandomInt(0, 4)]->changeState(
                    TheArk::get_instance()->getRandomGenerator()->getRandomDouble(-35.0, -20.0));
            devices[TheArk::get_instance()->getRandomGenerator()->getRandomInt(0, 4)]->changeState(
                    TheArk::get_instance()->getRandomGenerator()->getRandomDouble(-40.0, -20.0));
            break;

        case DISASTROUS:
            killed_staff = staff / TheArk::get_instance()->getRandomGenerator()->getRandomInt(2, 4);
            for (auto i = 0; i < 4; i++) {
                devices[TheArk::get_instance()->getRandomGenerator()->getRandomInt(0, 4)]->changeState(
                        TheArk::get_instance()->getRandomGenerator()->getRandomDouble(-60.0, -30.0));
            }
            speed -= speed * TheArk::get_instance()->getRandomGenerator()->getRandomFloat(0.2, 0.5);
            if (TheArk::get_instance()->getRandomGenerator()->getRandomInt(0, 2) == 1) {
                if (stage != MANEUVER)
                    stage = MANEUVER;
                maneuvering_time_left += 5;
            }
            break;

        case CATASTROPHIC:
            killed_staff = staff * TheArk::get_instance()->getRandomGenerator()->getRandomInt(75, 92) / 100;
            for (auto i = 0; i < 6; i++) {
                devices[TheArk::get_instance()->getRandomGenerator()->getRandomInt(0, 4)]->changeState(
                        TheArk::get_instance()->getRandomGenerator()->getRandomDouble(-90.0, -60.0));
            }
            if (stage != MANEUVER)
                stage = MANEUVER;
            distance_left += TheArk::get_instance()->getRandomGenerator()->getRandomInt(2, 7);
            maneuvering_time_left += 9;
            speed -= speed * TheArk::get_instance()->getRandomGenerator()->getRandomFloat(0.3, 0.7);
            break;
    }

    killStaff(killed_staff);
}


void NavigationService::process_year()
{
    // Block for flight stage management
    auto degradation_ratio = 1.0;
    if (stage == STABLE) {
        if (speed < 1.0)
            stage = ACCELERATION;
        acceleration = 0;
    }
    else if (stage == ACCELERATION) {
        acceleration = 0.1 * sqrt(TheArk::get_instance()->getServices()[Technical_Service]->getState()) *
                (1.0 / ACCELERATION_TIME);
        speed += acceleration;
        if (speed >= 1.0) { // This part has to be reworked to be used with upgrades
            speed = 1.0;
            stage = STABLE;
        }
    }
    else {
        acceleration = 0;
        maneuvering_time_left--;
        if (!maneuvering_time_left)
            stage = ACCELERATION;
        degradation_ratio = MANEUVERING_DAMAGE_RATIO;
    }
    clog << "SP: " << speed << endl;
    clog << "DST: " << distance_left << endl;
    distance_left -= speed;
    if (distance_left > 0)
        TheArk::get_instance()->setYearsTotal(TheArk::get_instance()->getYearsTotal() + 1);
    else
        TheArk::get_instance()->setYearsTotal(TheArk::get_instance()->getCurrentYear() - 1);

    // distance_delta section
    double effective_sum = 0; // Sum for all devices' performance
    for (auto & it : devices)
        effective_sum += it->getState() * it->getEfficiency() / 10000.0;

    distance_delta -= 10 * pow((effective_sum - MINIMAL_PRODUCTIVITY), 5);
    auto current_year_change = static_cast<int>(distance_delta);
   // clog<< "CYC: " << current_year_change << endl;
    if (distance_delta < -DISTANCE_DELTA_THRESHOLD) {
        distance_left += current_year_change;
        distance_delta -= current_year_change;
    }

    if (TheArk::get_instance()->getCurrentYear() > LOST_THE_WAY_WARNING * INITIAL_DISTANCE) {
        std::cout << "We've been travelling " << LOST_THE_WAY_WARNING << " times longer than estimated" << endl
        << "Probably we are lost forever" << endl
        << "Would you like to continue? (y/n)" << endl;
        string h;
        std::cin >> h;
        if (h == "y")
            LOST_THE_WAY_WARNING += 2;
        else
            TheArk::get_instance()->setYearsTotal(TheArk::get_instance()->getCurrentYear() - 1);
    }
    if (INITIAL_DISTANCE * MINIMAL_FLIGHT_TIME / 100.0 > TheArk::get_instance()->getYearsTotal())
        TheArk::get_instance()->setYearsTotal(
                static_cast<unsigned>(INITIAL_DISTANCE * MINIMAL_FLIGHT_TIME / 100));
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
    auto used_currently = TheArk::get_instance()->getResources()->getUsedByService(Navigation_Service);
    junk = used_currently;

    if (used_currently < required_resources)
        devices[TheArk::get_instance()->getRandomGenerator()->getRandomInt(0, 4)]->changeState(
                TheArk::get_instance()->getRandomGenerator()->
                getRandomDouble(-100 * (1.0 - used_currently * 1.0 / required_resources), 0.0));
    required_resources = staff;
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
         it->changeState(ANNUAL_DEGRADATION * degradation_ratio);

    //

    // Repairing the most efficient device, then the one with the least state
    double CURRENT_YEAR_REPAIR_PERCENT = TheArk::get_instance()->getRandomGenerator()->getRandomDouble(0.7, 1.5)
            * REPAIR_PERCENT_PER_YEAR * staff / DEFAULT_STAFF;

    required_resources += static_cast<unsigned>(CURRENT_YEAR_REPAIR_PERCENT);

    auto rprd_dev = std::max_element(devices.begin(), devices.end(), CompareDevicesEfficiency()); // repaired device
    CURRENT_YEAR_REPAIR_PERCENT = rprd_dev->get()->repairDevice(CURRENT_YEAR_REPAIR_PERCENT);

    if (static_cast<bool>(CURRENT_YEAR_REPAIR_PERCENT)) {
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
    return required_resources;
}

unsigned int NavigationService::returnJunk() {
    return junk;
}

FlightStage NavigationService::getFlightStage() const{
    return stage;
}




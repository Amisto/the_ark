//
// Created by Amisto on 4/2/2021.
//

#include "NavigationService.h"

// NavigationBlock implementations
unsigned short NavigationBlock::getDefaultEfficiency(NavigationDevices type)
{
    switch (type) {
        case INS:
            return 15;

        case OPTICAL:
            return 25;

        case COMP:
            return 35;

        case RADIO:
            return 20;

        case MAGNETIC:
            return 5;

        case NavDevAMOUNT:
            cout << "ERROR IN getDefaultEfficiency" << endl;
    }
    return -1;
}

NavigationBlock::NavigationBlock(NavigationDevices type)
{
    state = 0;
    consumption = 0;
    efficiency = getDefaultEfficiency(type);
}

unsigned int NavigationBlock::getConsumption() const { return consumption; }

float NavigationBlock::getEfficiency() const { return efficiency; }

float NavigationBlock::getState() const { return state; }

void NavigationBlock::setConsumption(unsigned int new_consumption) {
    consumption = new_consumption;
}

void NavigationBlock::setEfficiency(float new_efficiency) {
    efficiency = new_efficiency;
}

void NavigationBlock::setState(float new_state) {
    if(new_state <= 100 and new_state >= 0)
        state = new_state;
}

void NavigationBlock::changeState(float delta) {
    state += delta;
    if (state < 0)
        state = 0;
    else if (state > 100)
        state = 100;
}

float NavigationBlock::repairDevice(float available_points) {
    if (available_points < 0) {
        cout <<"REPAIR DEVICE problem < 0" << endl;
    }
    else if (state + available_points > 100) {
        available_points -= (100 - state);
        state = 100;
        return available_points;
    }
    else {
        state += available_points;
        return 0;
    }
}

//

// NavigationService implementations

NavigationService::NavigationService(): staff(0), required_staff(10),
                                        need_resources(0), years_delta(0)
{
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

float NavigationService::getRandomFloat(float min, float max)
{
    unsigned seed = std::chrono::steady_clock::now().time_since_epoch().count();
    static default_random_engine e(seed);
    uniform_real_distribution<float> d(min, max);
    return d(e);
}

int NavigationService::getRandomInt(int min, int max)
{
    unsigned seed = std::chrono::steady_clock::now().time_since_epoch().count();
    static default_random_engine e(seed);
    std::uniform_int_distribution<int> d(min, max);
    return d(e);
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
            devices[getRandomInt(0, 5)]->changeState(
                    getRandomFloat(-10.f, -5.f));

        case SEVERE:
            killed_staff = staff / 5;
            stage = MANEUVER;
            time_until_next_stage = 20;
            next_stage = STABLE;
            TheArk::get_instance()->setYearsTotal(
                    TheArk::get_instance()->getYearsTotal() + time_until_next_stage);
            devices[getRandomInt(0, 5)]->changeState(
                    getRandomFloat(-18.f, -10.f));

        case DISASTROUS:
            killed_staff = staff / 2;
            for (auto i = 0; i < 4; i++) {
                devices[getRandomInt(0, 5)]->changeState(
                        getRandomFloat(-60.f, -30.f));
            }
            if (getRandomInt(0, 2) == 1) {
                stage = MANEUVER;
                time_until_next_stage = 20;
                next_stage = STABLE;
                TheArk::get_instance()->setYearsTotal(
                        TheArk::get_instance()->getYearsTotal() + time_until_next_stage);
            }

        case CATASTROPHIC:
            killed_staff = staff * 0.9;
            for (auto i = 0; i < 6; i++) {
                devices[getRandomInt(0, 4)]->changeState(
                        getRandomFloat(-90.f, -60.f));
            }
            stage = MANEUVER;
            time_until_next_stage = 50;
            next_stage = STABLE;
            TheArk::get_instance()->setYearsTotal(
                    TheArk::get_instance()->getYearsTotal() + time_until_next_stage);
    }

    //cout << "MOGUS" << as << "  " << TheArk::get_instance()->getCurrentYear() << endl;
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
    // years_delta section DISABLED UNTIL EMERGENCY IS FIXED
//    double effective_sum = 0; // Sum for all devices' performance
//    for (auto & it : devices)
//        effective_sum += it->getState() * it->getEfficiency() / 10000.f;
//    years_delta -= effective_sum - 1;
//    if (years_delta > 3) {
//        TheArk::get_instance()->setYearsTotal(TheArk::get_instance()->getYearsTotal() + 3);
//        years_delta -= 3;
//        cout << "+3 Years" << endl;
//    } else if (years_delta < -3) {
//        TheArk::get_instance()->setYearsTotal(TheArk::get_instance()->getYearsTotal() - 3);
//        years_delta -= -3;
//        cout << "-3 Years" << endl;
//    }
    //cout << years_delta << endl;
    //


    // Block for staff management
    if (TheArk::get_instance()->getPopulation()->getUnemployedPeople() >=
        (required_staff / 10)) {
        required_staff += TheArk::get_instance()->getPopulation()->getUnemployedPeople() / 15;
    }
    else if (staff < std::stoi(
            TheArk::get_instance()->getInterface()->getGeneral()["Population"]) / 8) {
        required_staff = std::stoi(
                TheArk::get_instance()->getInterface()->getGeneral()["Population"]) / 8;
    }

    staff = TheArk::get_instance()->getPopulation()->getServiceStaff(Navigation_Service).size();
    //cout <<staff << " - ";
    //

    // Block for resource management
    //

    // Block for devices
     // Efficiency upgrades
      // Adding points to the device with max state
    if(staff > std::stoi
        (TheArk::get_instance()->getInterface()->getGeneral()["Population"]) / 8) {
        setChangedEfficiency(true);
        float eff_points = (staff * 100.f) /
                (std::stof(TheArk::get_instance()->getInterface()->getGeneral()["Population"]) / 8.f)
                - 100;

// 1       NavigationDevices upgraded_device;
//        float max_state = 0;
//        for (auto i = 0; i < NavDevAMOUNT; i++) {
//            if (devices[i]->getState() > max_state) {
//                max_state = devices[i]->getState();
//                upgraded_device = static_cast<NavigationDevices>(i);
//            }
//        }
        for (auto i = 0; i < NavDevAMOUNT; i++) {
            cout << devices[i]->getState() << "   " << devices[i]->getEfficiency() << endl;
        }
        auto upgrd_dev = std::max_element(devices.begin(), devices.end(), CompareDevicesState());// upgraded device
        //std::cout << upgrd_dev->get()->getState() << " " << upgrd_dev->get()->getEfficiency() << endl;
//1        devices[upgraded_device]->setEfficiency(devices[upgraded_device]->getEfficiency()
//        + eff_points);
        upgrd_dev->get()->setEfficiency(upgrd_dev->get()->getEfficiency() + eff_points);
    }
    else if (isChangedEfficiency()) {
        for (auto i = 0; i < NavDevAMOUNT; i++)
            devices[i]->setEfficiency(devices[i]->getDefaultEfficiency(
                    static_cast<NavigationDevices>(i)));
    }
    //

    // Repair
    float CURRENT_YEAR_REPAIR_PERCENT = getRandomFloat(0.7, 1.5)
            * REPAIR_PERCENT_PER_YEAR
            * (staff / (std::stof(
            TheArk::get_instance()->getInterface()->getGeneral()["Population"]) / 8));

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
//cout << required_staff << endl;
//cout << getStaffDemand() << " AMOGUS - " << TheArk::get_instance()->getPopulation()->getUnemployedPeople() << endl;
}

double NavigationService::getState()
{
    float result = 0;

    for (auto& it : devices)
        result += it->getState();

    return result / NavDevAMOUNT;
}

void NavigationService::setState(double s)
{
    if (not TheArk::get_instance()->getCurrentYear())
        required_staff = std::stoi(
                TheArk::get_instance()->getInterface()->getGeneral()["Population"]) / 8;
    for (auto& it : devices)
        it->setState(s);
}

unsigned int NavigationService::getStaffDemand() {
    return required_staff - staff;
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

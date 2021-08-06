//
// Created by Sageblatt on 28.07.2021.
//

#include "NavigationBlock.h"

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
        default:
            std::cerr << "NAVBLOCK ERROR_1" << endl;
            return -1;
    }
}

NavigationBlock::NavigationBlock(NavigationDevices type)
{
    state = 0;
    consumption = 0;
    efficiency = getDefaultEfficiency(type);
}

unsigned int NavigationBlock::getConsumption() const { return consumption; }

double NavigationBlock::getEfficiency() const { return efficiency; }

double NavigationBlock::getState() const { return state; }

void NavigationBlock::setConsumption(unsigned int new_consumption) {
    consumption = new_consumption;
}

void NavigationBlock::setEfficiency(double new_efficiency) {
    efficiency = new_efficiency;
}

void NavigationBlock::setState(double new_state) {
    if(new_state <= 100 and new_state >= 0)
        state = new_state;
    else
        std::cerr << "NAVBLOCK ERROR_3" << endl;
}

void NavigationBlock::changeState(double delta) {
    state += delta;
    if (state < 0)
        state = 0;
    else if (state > 100)
        state = 100;
}

double NavigationBlock::repairDevice(double available_points) {
    if (state + available_points > 100) {
        available_points -= (100 - state);
        state = 100;
        return available_points;
    }
    else {
        if (available_points < 0) {
            std::cerr << "NAVBLOCK ERROR_2" << endl;
            return 0;
        }
        state += available_points;
        return 0;
    }
}

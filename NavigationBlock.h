//
// Created by Sageblatt on 28.07.2021.
//

#ifndef THE_ARK_NAVIGATIONBLOCK_H
#define THE_ARK_NAVIGATIONBLOCK_H

#include <memory>
#include <iostream>

using std::cout;
using std::endl;
using std::unique_ptr;

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
    static unsigned short getDefaultEfficiency(NavigationDevices type);

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

#endif //THE_ARK_NAVIGATIONBLOCK_H

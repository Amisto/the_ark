//
// Created by Amisto on 4/2/2021.
//

#ifndef THE_ARK_MEDICALSERVICE_H
#define THE_ARK_MEDICALSERVICE_H

#include "Service.h"
#include "Interface.h"
#include "TheArk.h"
#include <list>

using std::list;

class MedicalService : public Service 
{
private:

    // FIELDS //
    const double PROPOTION_OF_PEOPLE         = std::stod(TheArk::get_instance()->getInterface()->getServices()[Medical_Service]["Propotion_of_people"]);
    const double AVERAGE_BIRTH_RATE          = std::stod(TheArk::get_instance()->getInterface()->getServices()[Medical_Service]["Birth_rate"]);
    const double AVERAGE_DEATH_RATE_CHILDREN = std::stod(TheArk::get_instance()->getInterface()->getServices()[Medical_Service]["Death_rate_children"]);
    const double AVERAGE_DEATH_RATE_ADULTS   = std::stod(TheArk::get_instance()->getInterface()->getServices()[Medical_Service]["Death_rate_adults"]);
    const double AVERAGE_DEATH_RATE_OLD      = std::stod(TheArk::get_instance()->getInterface()->getServices()[Medical_Service]["Death_rate_old"]);
    const unsigned RETIREMENT_AGE            = std::stoi(TheArk::get_instance()->getInterface()->getServices()[Medical_Service]["Retirement_age"]);
    const unsigned RESOURCES_PER_HEALTH      = std::stoi(TheArk::get_instance()->getInterface()->getServices()[Medical_Service]["Resources_per_health"]);
    
    unsigned int n_staff;                                 // current number of workers
    unsigned int staff;                                   // current staff
    unsigned int resources;                               // current amount of resources
    unsigned int junk;					                  // amount of junk to be returned this year
    unsigned int demand_resources;                        // demand resources to be requested this year
    double State;                                         // state of service
    unsigned int retirementAge;                           // age Adults-->Old 
    double ChildrenDeath;                                 // possibility of child death
    double AdultDeath;                                    // possibility of adult death
    double OldDeath;                                      // possibility of old death
    double Birth;                                         // number of new people
    unsigned int HealthYearAgo;                           // total health year ago
    unsigned int CriticalHealth;                          // critical health to die

    // METHODS //
    void process_child(std::shared_ptr<Human> human);     // updates child state
    void process_adult(std::shared_ptr<Human> human);     // updates adult state
    void process_old(std::shared_ptr<Human> human);       // updates old state

public:
    MedicalService();

    // GETTERS //
    double getState() override;                           // returns State
    unsigned int getCriticalHealth() const;               // returns CriticalHealth
    unsigned int borderAdultsToOldmen() const;            // returns RetirementAge
    double deathRateChildren() const;                     // returns ChildrenDeath
    double deathRateAdult() const;                        // returns AdultDeath
    double deathRateOldmen() const;                       // returns OldDeath
    double BirthRate() const;                             // returns Birth
    unsigned int getResourceDemand() override;            // returns NeedResources
    unsigned int getResourcePriority();                   //
    unsigned int getStaffDemand() override;               // returns demand staff
    unsigned int getStaffPriority();                      //
    
    // SETTERS //
    void setState(double s) override;

    // GAME MECHANICS //
    void process_accident(AccidentSeverity as) override;  // 
    void process_year() override;                         //
    unsigned int returnJunk() override;                   //
                   
    // OTHER //       
    bool changeStaff(int delta);                          //
    bool changeResources(int delta);                      //
};


#endif //THE_ARK_MEDICALSERVICE_H

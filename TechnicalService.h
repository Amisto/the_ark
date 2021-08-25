//
// Created by Amisto on 4/2/2021.
//

#ifndef THE_ARK_TECHNICALSERVICE_H
#define THE_ARK_TECHNICALSERVICE_H

#include <vector>
#include "Service.h"
#include "Human.h"
#include "TheArk.h"
#include "Interface.h"

class TechnicalService : public Service {

private:

    // FIELDS //
    
    const int CRITICAL_STATE = std::stoi(TheArk::get_instance()->getInterface()->getServices()[Technical_Service]["Critical_state"]);
    const double PROPOTION_OF_PEOPLE = std::stod(TheArk::get_instance()->getInterface()->getServices()[Technical_Service]["Propotion_of_people"]);
    const double AMOUNT_OF_JUNK_AFTER_COMPONENTS = std::stod(TheArk::get_instance()->getInterface()->getServices()[Technical_Service]["Amount_of_junk_after_components"]);
    const int RESOURCES_PER_PERCENT = std::stoi(TheArk::get_instance()->getInterface()->getServices()[Technical_Service]["Resources_per_percent"]);

    double totalState;                // total state of the ark
    double engineState;               // state of engine
    double protectionState;           // state of protection of the ark
    double serviceState;              // 
    unsigned int resources;           // current number of resources
    unsigned int staff;               // current number of staff
    unsigned int maxStaff;            // current demand staff
    unsigned int maxResources;        // current demand resources
    unsigned int junk;                // junk resources to be returned after the year
    unsigned int demand_resources;    //


    // METHODS //

    void kill(unsigned int victims);           // kills -victims- people if it is needed


public:

    TechnicalService();
    
    // GETTERS //
    
    virtual unsigned int getResourceDemand() override;           // returns number of demand resources at current year

    virtual unsigned int getResourcePriority();                  // returns priority of resources

    virtual unsigned int getStaffDemand() override;              // returns number of demand people

    virtual unsigned int getStaffPriority();                     // returns priority of people
    
    double getState() override;                                  // returns State of theh service

    double getEngineState() const;                                     // returns state of the engine


    // SETTERS //

    void setState(double s) override; 

    
    // OTHER //

    void process_accident(AccidentSeverity as) override;    // processing accidents

    void process_year() override;                           // processing events during the year

    void emergencyRepair();                                 // emergency repair of the ark if state is low

    double efficiencyConsumablesToComponents();             // speed of recycling Consumables-->Components

    double efficiencyJunkToConsumables();                   // speed of recycling Junk-->Consumables

    double efficiencyJunkToRefuse();                        // speed of recycling Junk-->Refuse

    virtual unsigned int returnJunk() override;		        // returns Junk at current year

};

#endif //THE_ARK_TECHNICALSERVICE_H

//
// Created by Amisto on 4/2/2021.
//

#ifndef THE_ARK_POPULATION_H
#define THE_ARK_POPULATION_H
#include <vector>
#include <memory>
#include <list>
#include <array>
#include <fstream>
#include "Human.h"

using std::shared_ptr;
using std::vector;
using std::list;
using std::array;

class Population 
{

private:

    // FIELDS //
    unsigned int children;                               // current number of children
    unsigned int adults;                                 // current number of adults
    unsigned int oldmen;                                 // current number of oldmen
    unsigned int unemployed_people;                      // current number of unemployed    
    unsigned int disabled;                               // current number of disabled people
    unsigned int workers;                                // current number of workers

    double distribution_coef[6];                         // propotions of people in services
    array<unsigned, 6> distributed_staff;                // number of distributed people at current year
    array<unsigned, 6> demand_staff;                     // number of demand staff of each service at current year
    array<list<shared_ptr<Human>>, 7> service_workers;   // service staff lists
    list<shared_ptr<Human>> people;                      // all people on the ark

    std::ofstream population_stat;                            // stream for statistics

    // METHODS // 
    unsigned int borderChildrenToAdults();
    unsigned int borderAdultsToOldmen();

    void staff_distribution(list<shared_ptr<Human>>& staff, unsigned int demand_staff); // sends demand_staff in service list
    void check_people_in_services();                                                    // pops people from service lists

    double deathRateChildren();
    double deathRateAdults();
    double deathRateOldmen();

public:

    Population();
    ~Population();
    
    // GETTERS //
    unsigned int getChildren() const;
    unsigned int getAdults() const;
    unsigned int getOldmen() const;
    unsigned int getTotal() const;
    unsigned int getUnemployedPeople() const;
    list<shared_ptr<Human>>& getServiceStaff(Services service);
    list<shared_ptr<Human>>& getPeople();
    array<list<shared_ptr<Human>>, 7>& getAllClassification();
    array<unsigned, 6>& getDemandStaff();
    array<unsigned, 6>& getDistributedStaff();

    // OTHER //
    void init(unsigned int total);
    void processYear();
};

#endif //THE_ARK_POPULATION_H

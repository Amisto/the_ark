//
// Created by Amisto on 4/2/2021.
//

#include "Population.h"
#include "TheArk.h"
#include "MedicalService.h"
#include "SocialService.h"
#include "TechnicalService.h"
#include "BiologicalService.h"
#include "EmergencyService.h"
#include "NavigationService.h"
#include "RandomNumberGenerator.h"
#include "Enums.cpp"
#include <cstdlib>
#include <iostream>

Population::Population() : children(0), adults(0), oldmen(0), unemployed_people(0), demand_staff{0, 0, 0, 0, 0, 0}, distributed_staff{0, 0, 0, 0, 0, 0}
{
    for (int i = 0; i < this->service_workers.size() - 1; i++)
    {
        distribution_coef[i] = std::stod(TheArk::get_instance()->getInterface()->getServices()[i]["Propotion_of_people"]);
    }
}

// GETTERS //
unsigned int Population::getChildren() const {
    return this->children;
}

unsigned int Population::getAdults() const {
    return this->adults;
}

unsigned int Population::getOldmen() const {
    return this->oldmen;
}

unsigned int Population::getTotal() const {
    return this->people.size();
}

unsigned int Population::getUnemployedPeople() const {
    return this->unemployed_people;
}

list<shared_ptr<Human>>& Population::getPeople()
{
    return this->people;
}

array<list<shared_ptr<Human>>, 7>& Population::getAllClassification()
{
     return this->service_workers;
}

list<shared_ptr<Human>>& Population::getServiceStaff(Services service)
{
     return this->service_workers[service];
}

array<unsigned, 6>& Population::getDemandStaff()
{
    return this->demand_staff;
}

array<unsigned, 6>& Population::getDistributedStaff()
{
    return this->distributed_staff;
}


// BORDERS //
unsigned int Population::borderChildrenToAdults()
{
    return TheArk::get_instance()->getSocialService()->borderChildrenToAdults();
}

unsigned int Population::borderAdultsToOldmen()
{
    return TheArk::get_instance()->getMedicalService()->borderAdultsToOldmen();
}


// OPERATIONS WITH STAFF //
void Population::staff_distribution(list<shared_ptr<Human>>& staff, unsigned int demand_staff)
{
    auto it = this->people.begin();
    unsigned int counter = 0;
    while (it != this->people.end() && counter < demand_staff)
    {
        if((*it)->getAge() >= this->borderChildrenToAdults() && (*it)->getAge() < this->borderAdultsToOldmen())
        {
            if((*it)->getTypeAsAWorker() == UNDEFINED || (*it)->getTypeAsAWorker() == UNEMPLOYED)
            {
                staff.push_back(*it);
                (*it)->setTypeAsAWorker(WORKER);
                counter++;
                this->unemployed_people--;
            }
        }
        ++it;

    }
}


// RATES //
double Population::deathRateChildren()
{
    return TheArk::get_instance()->getMedicalService()->deathRateChildren();
}

double Population::deathRateAdults()
{
    return TheArk::get_instance()->getMedicalService()->deathRateAdult();
}

double Population::deathRateOldmen()
{
    return TheArk::get_instance()->getMedicalService()->deathRateOldmen();
}


// PROCESS YEAR //
void Population::processYear() 
{
    // BIRTH
    for(int i = 0; i < TheArk::get_instance()->getMedicalService()->BirthRate(); i++)
    {
        auto* micro_chelik = new Human();
        micro_chelik->setAge(0);
        micro_chelik->setTypeAsAWorker(CHILD);
        auto ptr = shared_ptr<Human>(micro_chelik);
        this->people.push_back(ptr);
    }

    // ANNUAL DISTRIBUTION OF PEOPLE
    this->demand_staff[0] = TheArk::get_instance()->getTechnicalService()->getStaffDemand();
    this->demand_staff[1] = TheArk::get_instance()->getBiologicalService()->getStaffDemand();
    this->demand_staff[2] = TheArk::get_instance()->getMedicalService()->getStaffDemand();
    this->demand_staff[3] = TheArk::get_instance()->getNavigationService()->getStaffDemand();
    this->demand_staff[4] = TheArk::get_instance()->getEmergencyService()->getStaffDemand();
    this->demand_staff[5] = TheArk::get_instance()->getSocialService()->getStaffDemand();
    
    int all_demand_staff = 0;
    for (int i = 0; i < this->service_workers.size() - 1; i++)
    {
        all_demand_staff += demand_staff[i];
    }

    for (int i = 0; i < this->service_workers.size() - 1; i++)
    {
        unsigned int new_staff = (unsigned int)(double(demand_staff[i]) / double(all_demand_staff) * double(this->unemployed_people));
        unsigned int current_staff_of_service = this->service_workers[i].size();
        unsigned int max_staff_of_service     = (unsigned int)(this->distribution_coef[i] * this->adults);
        if (current_staff_of_service < max_staff_of_service && new_staff > 1)
        {   
            if (current_staff_of_service + new_staff > max_staff_of_service) 
                new_staff = max_staff_of_service - current_staff_of_service;
            this->distributed_staff[i] = new_staff;
            staff_distribution(this->service_workers[i], new_staff);
        }
    }

    // UPDATE EVERY PERSON
    this->children = 0;
    this->adults = 0;
    this->oldmen = 0;
    this->unemployed_people = 0;
    unsigned int HisAge;
    unsigned int CriticalHealth = TheArk::get_instance()->getMedicalService()->getCriticalHealth();

    for (auto it = people.begin(); it != people.end();)
    {
        // AGE INCREMENT
        HisAge = (*it)->getAge();
        (*it)->setAge(HisAge + 1);
        HisAge++;

        if (HisAge == this->borderChildrenToAdults()) (*it)->setTypeAsAWorker(UNEMPLOYED);

        // RANDOM DEATHS AND UPDATE OF FIELDS
        if (HisAge < this->borderChildrenToAdults())  // CHILDREN
        {
            children++;
            if (TheArk::get_instance()->getRandomGenerator()->getRandomFloat(0, 1) <= this->deathRateChildren()
                || (*it)->getPhysicalHealth() < CriticalHealth
                or !(*it)->isAlive())
            {
                (*it)->setIsAlive(false);
                children--;
            }
        }
        if ((HisAge >= this->borderChildrenToAdults()) && (HisAge < borderAdultsToOldmen()))  // ADULTS
        {
            adults++;
            if (TheArk::get_instance()->getRandomGenerator()->getRandomFloat(0, 1) <= this->deathRateAdults()
                || (*it)->getPhysicalHealth() < CriticalHealth
                or !(*it)->isAlive())
            {
                (*it)->setIsAlive(false);
                adults--;
            }
            if ((*it)->getTypeAsAWorker() != WORKER)
                unemployed_people ++;
        }
        if (HisAge >= this->borderAdultsToOldmen())  // OLD
        {
            oldmen++;
            if (TheArk::get_instance()->getRandomGenerator()->getRandomFloat(0, 1) <= this->deathRateOldmen()
                || HisAge > 100  || (*it)->getPhysicalHealth() < CriticalHealth
                or !(*it)->isAlive())
            {
                (*it)->setIsAlive(false);
                oldmen--;
            }
        }
       
        // POP DEAD PERSON
        if (!(*it)->isAlive())
        {
            auto tmpit = it;
            ++it;
            this->people.erase(tmpit);
        }
        else
        {
            ++it;
        }
    }

    // POP DEAD PEOPLE IN SERVICE LISTS
    this->check_dead_people_in_services();
}

void Population::check_dead_people_in_services()
{
    for (list<shared_ptr<Human>>& classification: this->service_workers)
    {
        for (auto it = classification.begin(); it != classification.end();)
        {
            if (!(*it)->isAlive() || (*it)->getAge() >= this->borderAdultsToOldmen())
            {
                (*it)->setTypeAsAWorker(RETIRED);
                auto nit = it;
                ++it;
                classification.erase(nit);
            }
            else{
                ++it;
            }
        }
    }
}

// INITIALIZATION //
void Population::init(unsigned int total) 
{
    this->children = (unsigned int)(std::stod(TheArk::get_instance()->getInterface()->getGeneral()["Children"]) * total);
    this->oldmen   = (unsigned int)(std::stod(TheArk::get_instance()->getInterface()->getGeneral()["Old"]) * total);
    this->adults   = total - children - oldmen;
    this->unemployed_people = this->adults;

    // CREATING PEOPLE
    for(int i = 0; i < this->children; i++) // CHILDREN
    {   
        auto* person = new Human();
        person->setAge(TheArk::get_instance()->getRandomGenerator()->getRandomInt(0, borderChildrenToAdults() - 1) );
        person->setTypeAsAWorker(CHILD);
        auto ptr = shared_ptr<Human>(person);
        this->people.push_back(ptr);
    }

    for(int i = 0; i < this->oldmen; i++) // OLD
    {
        auto *person = new Human;
        person->setAge(TheArk::get_instance()->getRandomGenerator()->getRandomInt(borderAdultsToOldmen(), 100));
        person->setTypeAsAWorker(RETIRED);
        auto ptr = shared_ptr<Human>(person);
        this->people.push_back(ptr);
    }

    for(int i = 0; i < this->adults; i++) // ADULTS
    {
        auto* person = new Human;
        person->setAge(TheArk::get_instance()->getRandomGenerator()->getRandomInt(borderChildrenToAdults(), borderAdultsToOldmen() - 1));
        person->setTypeAsAWorker(UNEMPLOYED);
        auto ptr = shared_ptr<Human>(person);
        this->people.push_back(ptr);
    }

    // WORKERS DISTRIBUTION TO SERVICES
    unsigned int number_of_people_to_service[6];
    for (int i = 0; i < 6; i ++)
    {
        number_of_people_to_service[i] = (unsigned int)(this->distribution_coef[i] * this->adults);
    }
    int number_of_service = 0;
    int counter = 0;
    for (auto human_it = this->people.begin(); human_it != this->people.end(); human_it++)
    {
        if ((*human_it)->getAge() >= this->borderChildrenToAdults() && (*human_it)->getAge() < this->borderAdultsToOldmen())
        {
            this->service_workers[number_of_service].push_back(*human_it);
            (*human_it)->setTypeAsAWorker(WORKER);
            counter ++;
            this->unemployed_people--;
        }
        if (counter >= number_of_people_to_service[number_of_service])
        {
            number_of_service++;
            counter = 0;
        }
        if (number_of_service > this->service_workers.size() - 2)
            break;
    }
}



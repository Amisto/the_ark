//
// Created by Amisto on 4/2/2021.
//

#include "MedicalService.h"
#include "Population.h"
#include "BiologicalService.h"
#include "SocialService.h"
#include "Human.h"
#include <cmath>
#include <list>
#include <iostream>


MedicalService::MedicalService() : retirementAge(65), ChildrenDeath(0.0001), AdultDeath(0.001), OldDeath(0.01), State(100), n_staff(0), resources(1000), Junk(0), Birth(0), NeedResources(1000), HealthYearAgo(100), CriticalHealth(10) 
{}

void MedicalService::process_accident(AccidentSeverity as) 
{
    // HURT PEOPLE
    std::list<std::shared_ptr<Human>>& people = TheArk::get_instance()->getPopulation()->getPeople(); 
    unsigned int number_of_people = people.size();
    unsigned int victims = 0.05 * as * number_of_people;
    
    std::list<shared_ptr<Human>>::iterator human = people.begin();
    int counter;
    for (int i = 0; i < 3; i++)
    {
        counter = 0;
        while (counter < victims / 3)
        {
            (*human)->setPhysicalHealth((*human)->getPhysicalHealth() - as);
            human++;
            counter++;
        }
        for (int j = 0; j < number_of_people / 3; j++)
            human++;
    }
    
    // KILL WORKERS
    std::list<shared_ptr<Human>>& workers = TheArk::get_instance()->getPopulation()->getServiceStaff(Medical_Service);
    unsigned int number_of_workers = workers.size();
    victims = 0.01 * as * number_of_workers;
    std::list<shared_ptr<Human>>::iterator worker = workers.begin();

    for (int i = 0; i < victims; i++)
    {
        (*worker)->setIsAlive(false);
        worker++;
    }
}

void MedicalService::process_year() 
{
    // UPDATE OF FIELDS //
    retirementAge = RETIREMENT_AGE;
    n_staff = (unsigned int)(PROPOTION_OF_PEOPLE * TheArk::get_instance()->getPopulation()->getTotal());
    NeedResources = static_cast<unsigned int>(20 * TheArk::get_instance()->getPopulation()->getTotal());
    double ResourcePercent = double(resources) / NeedResources;
    double StaffPercent = double(TheArk::get_instance()->getPopulation()->getServiceStaff(Medical_Service).size()) / n_staff;
    double birth_splash = double(this->State) / 100 * AVERAGE_BIRTH_RATE;
    Birth = (unsigned int)(RNG.getRandomDouble(AVERAGE_BIRTH_RATE - birth_splash, AVERAGE_BIRTH_RATE + birth_splash) * TheArk::get_instance()->getPopulation()->getAdults());

    // UPDATING HUMAN STATE //
    unsigned int total_health = 0;                                                                                  
    for (auto &it : TheArk::get_instance()->getPopulation()->getPeople()) 
    {                                             
        total_health += it->getPhysicalHealth();    

        // CHILDREN                                                                  
        if (it->getAge() < TheArk::get_instance()->getSocialService()->borderChildrenToAdults()) 
        {
            this->process_child(it);
        }

        // ADULTS
        if (it->getAge() >= TheArk::get_instance()->getSocialService()->borderChildrenToAdults() && it->getAge() <= borderAdultsToOldmen()) 
        {
            this->process_adult(it);
        }

        // OLD
        if (it->getAge() > borderAdultsToOldmen()) 
        {
            this->process_old(it);
        }
    }

    if (TheArk::get_instance()->getPopulation()->getTotal() != 0) 
    {
        State = total_health / TheArk::get_instance()->getPopulation()->getTotal();                                   
    } 
    else 
        State = 0;
    
    if (State < 75)
        CriticalHealth = (100 - State) / 5;
    else 
        CriticalHealth = 5;
    
    ChildrenDeath = AVERAGE_DEATH_RATE_CHILDREN;                                
    AdultDeath    = AVERAGE_DEATH_RATE_ADULTS;
    OldDeath      = AVERAGE_DEATH_RATE_OLD;
}

void MedicalService::process_child(std::shared_ptr<Human> human)
{
    if ((human->getPhysicalHealth() < 60) && (human->getPhysicalHealth() > 30))
    {     
        human->setPhysicalHealth(static_cast<unsigned int>(round(human->getPhysicalHealth() + RNG.getRandomInt(0, 3) - RNG.getRandomInt(0, 4) + (TheArk::get_instance()->getSocialService()->getState() * State - 90000) / 10000)));
    }

    if (human->getPhysicalHealth() < 30) 
    {
        human->setPhysicalHealth(static_cast<unsigned int>(round(human->getPhysicalHealth() + RNG.getRandomInt(0, 3) - RNG.getRandomInt(0, 3) + (TheArk::get_instance()->getSocialService()->getState() * State - 90000) / 10000)));
    }

    if (human->getPhysicalHealth() > 100) 
        human->setPhysicalHealth(100);
}

void MedicalService::process_adult(std::shared_ptr<Human> human)
{
    if (human->getPhysicalHealth() < 60 && human->getPhysicalHealth() > 30) 
    {
        human->setPhysicalHealth(static_cast<unsigned int>(round(human->getPhysicalHealth() + RNG.getRandomInt(0, 2) - RNG.getRandomInt(0, 2))));
    }

    if (human->getPhysicalHealth() < 30) 
    {
        human->setPhysicalHealth(static_cast<unsigned int>(round(human->getPhysicalHealth() + State / 10)));             
    }

    if (human->getPhysicalHealth() > 100) 
        human->setPhysicalHealth(100);
}

void MedicalService::process_old(std::shared_ptr<Human> human)
{
    if (human->getPhysicalHealth() < 60 && human->getPhysicalHealth() > 30) 
    {
        human->setPhysicalHealth(static_cast<unsigned int>(round(human->getPhysicalHealth() + RNG.getRandomInt(0, 2) - RNG.getRandomInt(0, 3) + (TheArk::get_instance()->getSocialService()->getState() * State - 90000) / 10000)));
    }

    if (human->getPhysicalHealth() < 30) 
    {
        human->setPhysicalHealth(static_cast<unsigned int>(round(human->getPhysicalHealth() + RNG.getRandomInt(0, 2) - RNG.getRandomInt(0, 4) + (TheArk::get_instance()->getSocialService()->getState() * State - 90000) / 10000)));
    }

    if (human->getPhysicalHealth() > 100) 
        human->setPhysicalHealth(100);
}

void MedicalService::setState(double s) 
{
    for (auto &person: TheArk::get_instance()->getPopulation()->getPeople())
        person->setPhysicalHealth((unsigned int)s);
}

unsigned int MedicalService::getCriticalHealth() const
{
    return this->CriticalHealth;
}

unsigned int MedicalService::getStaffDemand() 
{
    return static_cast<unsigned int>(round(double(n_staff - TheArk::get_instance()->getPopulation()->getServiceStaff(Medical_Service).size())));
}

unsigned int MedicalService::getResourceDemand() 
{
    return static_cast<unsigned int>(round((NeedResources - resources) * (double(1.8) - (State / 100.0))));
}

unsigned int MedicalService::getStaffPriority() 
{
    return static_cast<unsigned int>(round(exp(1.8 * (1 - double(TheArk::get_instance()->getPopulation()->getServiceStaff(Medical_Service).size())/n_staff))));
}

unsigned int MedicalService::getResourcePriority() 
{
    return static_cast<unsigned int>(round(exp(1.8 * (1 - double(resources) / NeedResources))));
}

unsigned int MedicalService::borderAdultsToOldmen() const 
{
    return retirementAge;
}

double MedicalService::deathRateChildren() const 
{
    return ChildrenDeath;
}

double MedicalService::deathRateAdult() const 
{
    return AdultDeath;
}

double MedicalService::deathRateOldmen() const 
{
    return OldDeath;
}

double MedicalService::BirthRate() const 
{
    return Birth;
}

double MedicalService::getState() 
{
    return State;
}

bool MedicalService::changeStaff(int delta) 
{
    return true;
}

bool MedicalService::changeResources(int delta) 
{
    resources += delta;
    Junk = delta;
    return true;
}

unsigned int MedicalService::returnJunk()
{
	return Junk;
}

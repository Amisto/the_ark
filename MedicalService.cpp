//
// Created by Amisto on 4/2/2021.
//

#include "MedicalService.h"
#include "TheArk.h"
#include "Population.h"
#include "BiologicalService.h"
#include "SocialService.h"
#include "RandomNumberGenerator.h"
#include <cmath>
#include <iostream>

MedicalService::MedicalService() : retirementAge(65), ChildrenDeath(0.0001), AdultDeath(0.001), OldDeath(0.01),
                                   State(100), n_staff(0), resources(1000),Junk(0), Birth(0), NeedResources(1000), HealthYearAgo(100),
                                   CriticalHealth(10) 
 {}

void MedicalService::process_accident(AccidentSeverity as) {
}

void MedicalService::process_year() {

    RandomNumberGenerator RNG;
    retirementAge = static_cast<unsigned int>(round(51 * (2 - TheArk::get_instance()->getSocialService()->getState() *
                                                              State * State / 1000000)));
    n_staff = static_cast<unsigned int>(round(0.1 * TheArk::get_instance()->getPopulation()->getTotal()));
    NeedResources = static_cast<unsigned int>(20 * TheArk::get_instance()->getPopulation()->getTotal());
    double ResourcePercent = double(resources) / NeedResources;
    double StaffPercent = double(TheArk::get_instance()->getPopulation()->getServiceStaff(Medical_Service).size()) / n_staff;
    Birth = RNG.getRandomInt(1, 3) * TheArk::get_instance()->getPopulation()->getAdults() / 100;

    unsigned int oll_health = 0;                                                                                        // общее здоровье всего корабля
    unsigned int HIlChild = 0, HIlAd = 0, HIlOld = 0;                                                                   // количество тяжелобольных
    for (auto &it : TheArk::get_instance()->getPopulation()->getPeople()) {                                             // обходим по всем людям на корабле
        oll_health += it->getPhysicalHealth();                                                                          // считаем общий показатель здоровья
        if (it->getAge() < TheArk::get_instance()->getSocialService()->borderChildrenToAdults()) {
            if ((it->getPhysicalHealth() < 60) && (it->getPhysicalHealth() > 30)) {                                     // обработка детей со здоровьем от 30 до 60
                it->setPhysicalHealth(static_cast<unsigned int>(round(
                        it->getPhysicalHealth() + RNG.getRandomInt(0, 3) - RNG.getRandomInt(0, 4) +
                        (TheArk::get_instance()->getSocialService()->getState() *
                         State - 90000) / 10000)));
            }
            if (it->getPhysicalHealth() < 30) {                                                                         // обработка тяжелобольных детей со здоровьем <30
                HIlChild++;                                                                                             // считаем количество тяжелобольных детей
                it->setPhysicalHealth(static_cast<unsigned int>(round(
                        it->getPhysicalHealth() + RNG.getRandomInt(0, 3) - RNG.getRandomInt(0, 3) +
                        (TheArk::get_instance()->getSocialService()->getState() *
                         State - 90000) / 10000)));
            }
        }
        if ((it->getAge() >= TheArk::get_instance()->getSocialService()->borderChildrenToAdults()) &&
            (it->getAge() <= borderAdultsToOldmen())) {
            if ((it->getPhysicalHealth() < 60) && (it->getPhysicalHealth() > 30)) {
                it->setPhysicalHealth(static_cast<unsigned int>(round(
                        it->getPhysicalHealth() + RNG.getRandomInt(0, 2) - RNG.getRandomInt(0, 2))));
            }
            if (it->getPhysicalHealth() < 30) {
                HIlAd++;
                it->setPhysicalHealth(static_cast<unsigned int>(round(it->getPhysicalHealth() + State / 10)));
                        
            }
            if (it->getPhysicalHealth() > 100) it->setPhysicalHealth(100);
        }
        if (it->getAge() > borderAdultsToOldmen()) {
            if ((it->getPhysicalHealth() < 60) && (it->getPhysicalHealth() > 30)) {
                it->setPhysicalHealth(static_cast<unsigned int>(round(
                        it->getPhysicalHealth() + RNG.getRandomInt(0, 2) - RNG.getRandomInt(0, 3) +
                        (TheArk::get_instance()->getSocialService()->getState() *
                         State - 90000) / 10000)));
            }
            if (it->getPhysicalHealth() < 30) {
                HIlOld++;
                it->setPhysicalHealth(static_cast<unsigned int>(round(
                        it->getPhysicalHealth() + RNG.getRandomInt(0, 2) - RNG.getRandomInt(0, 4) +
                        (TheArk::get_instance()->getSocialService()->getState() *
                         State - 90000) / 10000)));
            }
            if (it->getPhysicalHealth() > 100) it->setPhysicalHealth(100);
        }
    }

    if (TheArk::get_instance()->getPopulation()->getTotal() != 0) {
        State = 1.0 * oll_health /
                TheArk::get_instance()->getPopulation()->getTotal();                                                    // вычислятся State исходя из среднего здоровья всего корабля
    } else State = 0;
    
    if (State < 75)
        CriticalHealth = (100 - State) / 5;
    else 
        CriticalHealth = 5;
    
    ChildrenDeath = (1 - (TheArk::get_instance()->getBiologicalService()->getState() / 100.0)) *
                    (1 - ResourcePercent) * (1 - StaffPercent) *
                    (1 - (TheArk::get_instance()->getSocialService()->getState() / 100.0)) *
                    HIlChild / TheArk::get_instance()->getPopulation()->getChildren();                                  // вычислятся веротность смерти из процента тяжелобольных
    AdultDeath = (1 - (TheArk::get_instance()->getBiologicalService()->getState() / 100.0)) *
                 (1 - ResourcePercent) * (1 - StaffPercent) *
                 (1 - (TheArk::get_instance()->getSocialService()->getState() / 100.0)) *
                 HIlAd / TheArk::get_instance()->getPopulation()->getAdults();
    OldDeath = (1 - (TheArk::get_instance()->getBiologicalService()->getState() / 100.0)) *
               (1 - ResourcePercent) * (1 - StaffPercent) *
               (1 - (TheArk::get_instance()->getSocialService()->getState() / 100.0)) *
               HIlOld / TheArk::get_instance()->getPopulation()->getOldmen();

    resources -= (HealthYearAgo - oll_health) / 2;
}

void MedicalService::setState(double s) {
    for (auto &person: TheArk::get_instance()->getPopulation()->getPeople())
        person->setPhysicalHealth((unsigned int)s);
}

unsigned int MedicalService::getCriticalHealth() const
{
    return this->CriticalHealth;
}

unsigned int MedicalService::getStaffDemand() {
    return static_cast<unsigned int>(round(double(n_staff - TheArk::get_instance()->getPopulation()->getServiceStaff(Medical_Service).size())));
}

unsigned int MedicalService::getResourceDemand() {
    return static_cast<unsigned int>(round((NeedResources - resources) * (double(1.8) - (State / 100.0))));
}

unsigned int MedicalService::getStaffPriority() {
    return static_cast<unsigned int>(round(exp(1.8 * (1 - double(TheArk::get_instance()->getPopulation()->getServiceStaff(Medical_Service).size())/n_staff))));
}

unsigned int MedicalService::getResourcePriority() {
    return static_cast<unsigned int>(round(exp(1.8 * (1 - double(resources) / NeedResources))));
}

unsigned int MedicalService::borderAdultsToOldmen() const {
    return retirementAge;
}

double MedicalService::deathRateChildren() const {
    return ChildrenDeath;
}

double MedicalService::deathRateAdult() const {
    return AdultDeath;
}

double MedicalService::deathRateOldmen() const {
    return OldDeath;
}

double MedicalService::BirthRate() const {
    return Birth;
}

double MedicalService::getState() {
    return State;
}

bool MedicalService::changeStaff(int delta) {
    return true;
}

bool MedicalService::changeResources(int delta) {
    resources += delta;
    Junk = delta;
    return true;
}

unsigned int MedicalService::returnJunk()
{
	return Junk;
}

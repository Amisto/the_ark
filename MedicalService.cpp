//
// Created by Amisto on 4/2/2021.
//

#include "MedicalService.h"
#include "TheArk.h"
#include "Population.h"
#include "BiologicalService.h"
#include "SocialService.h"
#include <cmath>

MedicalService::MedicalService() : retirementAge(65), ChildrenDeath(0.0001), AdultDeath(0.001), OldDeath(0.01),
                                   State(100), n_engineers(70), resources(1000) {}

void MedicalService::process_accident(AccidentSeverity as) {
}

void MedicalService::process_year() {
    unsigned int oll_health = 0, IlChild = 0, IlAd = 0, IlOld = 0;                                          // количество больных
    unsigned int HIlChild = 0, HIlAd = 0, HIlOld = 0;                                                       // количество тяжелобольных
    for (auto &it : TheArk::get_instance()->getPopulation()->getPeople()) {                                 // обходим по всем людям на корабле
        oll_health += it->getPhysicalHealth();                                                              // считаем общий показатель здоровья
        if (it->getAge() < TheArk::get_instance()->getSocialService()->borderChildrenToAdults()) {
            if ((it->getPhysicalHealth() < 60) && (it->getPhysicalHealth() > 30)) {
                IlChild++;                                                                                   // считаем количество больных детей
                it->setPhysicalHealth(it->getPhysicalHealth() + rand() % 4 - rand() % 3);
            }
            if (it->getPhysicalHealth() < 30) {
                HIlChild++;                                                                                  // считаем количество тяжелобольных детей
                it->setPhysicalHealth(it->getPhysicalHealth() + rand() % 4 - rand() % 4);
            }
            it->setPhysicalHealth(it->getPhysicalHealth() + rand() % 5 - rand() % 3);
            if (it->getPhysicalHealth() > 100) it->setPhysicalHealth(100);
        }
        if ((it->getAge() >= TheArk::get_instance()->getSocialService()->borderChildrenToAdults()) &&
            (it->getAge() <= borderAdultsToOldmen())) {
            if ((it->getPhysicalHealth() < 60) && (it->getPhysicalHealth() > 30)) {
                IlAd++;
                it->setPhysicalHealth(it->getPhysicalHealth() + rand() % 3 - rand() % 3);
            }
            if (it->getPhysicalHealth() < 30) {
                HIlAd++;
                it->setPhysicalHealth(it->getPhysicalHealth() + rand() % 3 - rand() % 4);
            }
            it->setPhysicalHealth(it->getPhysicalHealth() + rand() % 4 - rand() % 4);
            if (it->getPhysicalHealth() > 100) it->setPhysicalHealth(100);
        }
        if (it->getAge() > borderAdultsToOldmen()) {
            if ((it->getPhysicalHealth() < 60) && (it->getPhysicalHealth() > 30)) {
                IlOld++;
                it->setPhysicalHealth(it->getPhysicalHealth() + rand() % 2 - rand() % 3);
            }
            if (it->getPhysicalHealth() < 30) {
                HIlOld++;
                it->setPhysicalHealth(it->getPhysicalHealth() + rand() % 2 - rand() % 5);
            }
            it->setPhysicalHealth(it->getPhysicalHealth() + rand() % 3 - rand() % 5);
            if (it->getPhysicalHealth() > 100) it->setPhysicalHealth(100);
        }
    }
    State = 1.0 * oll_health /
            TheArk::get_instance()->getPopulation()->getTotal();                                        // вычислятся State исходя из среднего здоровья всего корабля
    ChildrenDeath = 0.01 * TheArk::get_instance()->getSocialService()->getState() * HIlChild /
                    TheArk::get_instance()->getPopulation()->getChildren();                             // вычислятся веротность смерти из процента тяжелобольных
    AdultDeath = 0.05 * TheArk::get_instance()->getSocialService()->getState() * HIlAd /
                 TheArk::get_instance()->getPopulation()->getAdults();
    OldDeath = 0.1 * TheArk::get_instance()->getSocialService()->getState() * HIlOld /
               TheArk::get_instance()->getPopulation()->getOldmen();

    retirementAge = 51 * (2 - TheArk::get_instance()->getSocialService()->getState() *
                              State);         // вычислятся пенсионный возраст исходя из состояния медицины и социума

}

void MedicalService::setState(double s) {
    n_engineers = 0.035 * TheArk::get_instance()->getPopulation()->getTotal();
    if (s != double(100)) {
        unsigned int delta = (100 - s);
        for (auto &it : TheArk::get_instance()->getPopulation()->getPeople())
            it->setPhysicalHealth((it->getPhysicalHealth() - delta) * (95 + rand() % 10) / 100);
    }
}

unsigned int MedicalService::getStaffDemand() {
    return round((0.035 * TheArk::get_instance()->getPopulation()->getTotal() - n_engineers) * (double(1.8) - State));
}

unsigned int MedicalService::getStaffPriority() {
    return round(2 * exp(1 - n_engineers));
}

unsigned int MedicalService::borderAdultsToOldmen() {
    return retirementAge;
}

double MedicalService::deathRateChildren() {
    return ChildrenDeath;
}

double MedicalService::deathRateAdult() {
    return AdultDeath;
}

double MedicalService::deathRateOldmen() {
    return OldDeath;
}

double MedicalService::getState() {
    return State;
}

unsigned int MedicalService::getResourceDemand() {
    return 500;
}

unsigned int MedicalService::getResourcePriority() {
    return 3;
}

bool MedicalService::changeStaff(int delta) {
    return true;
}

bool MedicalService::changeResources(int delta) {
    return true;
}

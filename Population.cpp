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
#include "Enums.cpp"
#include <cstdlib>
#include <iostream>

Population::Population() : children(0), adults(0), oldmen(0), unemployed_people(0){}

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

unsigned int Population::borderChildrenToAdults()
{
    return TheArk::get_instance()->getSocialService()->borderChildrenToAdults();
}

unsigned int Population::borderAdultsToOldmen()
{
    return TheArk::get_instance()->getMedicalService()->borderAdultsToOldmen();
}

unsigned int Population::number_staff(Services service)
{
    switch(service){
        case Technical_Service:
            return TheArk::get_instance()->getTechnicalService()->getRecommendedNStaff();
        case Biological_Service:
            return TheArk::get_instance()->getBiologicalService()->getRecommendedNStaff();
        case Medical_Service:
            return TheArk::get_instance()->getMedicalService()->getRecommendedNStaff();
        case Navigation_Service:
            return TheArk::get_instance()->getNavigationService()->getRecommendedNStaff();
        case Emergency_Service:
            return TheArk::get_instance()->getEmergencyService()->getRecommendedNStaff();
        case Social_Service:
            return TheArk::get_instance()->getSocialService()->getRecommendedNStaff();
        default:
            return 0;
    }

}

void Population::stuff_distribution(list<shared_ptr<Human>>& stuff, unsigned int demand_stuff)
{
    auto it = this->people.begin();
    unsigned int counter = 0;
    while(it != this->people.end() && counter < demand_stuff)
    {
        if((*it)->getAge() > this->borderChildrenToAdults() && (*it)->getAge() < this->borderAdultsToOldmen())
        {
            if((*it)->getTypeAsAWorker() == UNDEFINED || (*it)->getTypeAsAWorker() == UNEMPLOYED)
            {
                stuff.push_back(*it);
                (*it)->setTypeAsAWorker(WORKER);
                counter++;
                this->unemployed_people--;
            }
        }
        ++it;

    }
}

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

list<shared_ptr<Human>>& Population::getPeople()
{
    return this->people;
}

array<list<shared_ptr<Human>>, 7>& Population::getAllClassification()
{
     return this->classifications_of_humans;
}

list<shared_ptr<Human>>& Population::getServiceStaff(Services service)
{
     return this->classifications_of_humans[service];
}

void Population::processYear() {
    //рождаемость
    for(int i = 0; i < TheArk::get_instance()->getMedicalService()->BirthRate(); i++)
    {
        auto* micro_chelik = new Human();
        micro_chelik->setAge(0);
        auto ptr = shared_ptr<Human>(micro_chelik);
        this->people.push_back(ptr);
    }
    //распределение стафа, сделал бы в цикле, но нужно вызывать каждую ф-ю отдельно
    unsigned int demand_staff[classifications_of_humans.size() - 1];
    demand_staff[0] = TheArk::get_instance()->getTechnicalService()->getStaffDemand();
    demand_staff[1] = TheArk::get_instance()->getBiologicalService()->getStaffDemand();
    demand_staff[2] = TheArk::get_instance()->getMedicalService()->getStaffDemand();
    demand_staff[3] = TheArk::get_instance()->getNavigationService()->getStaffDemand();
    demand_staff[4] = TheArk::get_instance()->getEmergencyService()->getStaffDemand();
    demand_staff[5] = TheArk::get_instance()->getSocialService()->getStaffDemand();

    int all_demand_staff = 0;
    for (int i = 0; i < this->classifications_of_humans.size() - 1; i++)
    {
        all_demand_staff += demand_staff[i];
    }
    if (double(demand_staff[0]) / double(all_demand_staff) * double(this->unemployed_people > 1))
    {
        for (int i = 0; i < this->classifications_of_humans.size() - 1; i++)
        {
            stuff_distribution(this->classifications_of_humans[i], (unsigned int)(double(demand_staff[i]) / double(all_demand_staff) * double(this->unemployed_people)));
        }   
        
    }
    //


    // обработка по возрасту и смертность сразу же, чтобы два раза не ходить
    children = 0;
    adults = 0;
    oldmen = 0;
    unemployed_people = 0;
    unsigned int HisAge = 0;
    unsigned int CriticalHealth = TheArk::get_instance()->getMedicalService()->getCriticalHealth();


    for (auto it = people.begin(); it != people.end();)
    {

        HisAge = (*it)->getAge();
        //подсчёт количества населения по группам и обработка случайной смертности
        if (HisAge < this->borderChildrenToAdults())
        {
            children++;
            if (rand() <= this->deathRateChildren() * RAND_MAX || (*it)->getPhysicalHealth() < CriticalHealth)
            {
                (*it)->setIsAlive(false);
                children--;
            }
        }
        if ((HisAge >= this->borderChildrenToAdults()) && (HisAge < borderAdultsToOldmen()))
        {
            adults++;
            if (rand() <= this->deathRateAdults() * RAND_MAX  || (*it)->getPhysicalHealth() < CriticalHealth)
            {
                (*it)->setIsAlive(false);
                adults--;
            }
            if ((*it)->getTypeAsAWorker() == UNEMPLOYED)
                unemployed_people ++;
        }
        if (HisAge >= this->borderAdultsToOldmen())
        {
            oldmen++;
            if (rand() <= this->deathRateOldmen() * RAND_MAX || HisAge > 100  || (*it)->getPhysicalHealth() < CriticalHealth)
            {
                (*it)->setIsAlive(false);
                oldmen--;
            }
        }
       
        //старение
        (*it)->setAge(HisAge + 1);
        if (HisAge + 1 == this->borderChildrenToAdults()) this->unemployed_people ++;

        //попанье мертвых
        if (!(*it)->isAlive())
        {
            auto tmpit = it;
            ++it;
            this->people.erase(tmpit);
        }
        else{
            ++it;
        }

    }
    // конец обработки по возрасту

    this->check_dead_people_is_classifications(); // см коммент в ф-ции

    // тут ещё безусловно нужно написать рождаемость, иначе все подохнут
    // дальше идет просмотр обучающихся и обработка тех, кто отучился. Их пихаем в службы по запросам
    // распределение остальных людей по соответвующим категориям

    //конец обработки
}

void Population::check_dead_people_is_classifications()
{
    // удаление людей с is_alive == false из всех классификаций
    for (list<shared_ptr<Human>>& classification: this->classifications_of_humans)
    {
        for (auto it = classification.begin(); it != classification.end();)
        {
            if (!(*it)->isAlive()){
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

void Population::init(unsigned int total) {
    this->children = (unsigned int)(0.15 * total);
    this->oldmen = (unsigned int)(0.15 * total);
    this->adults = total - children - oldmen;
    this->unemployed_people = this->adults;

    for(int i = 0; i < this->children; i++)
    {          // заполняем детьми
        auto* person = new Human();
        person->setAge((rand()% this->borderChildrenToAdults()));
        person->setTypeAsAWorker(CHILD);
        auto ptr = shared_ptr<Human>(person);
        this->people.push_back(ptr);
    }
    for(int i = 0; i < this->oldmen; i++)
    {            // заполняем стариками
        auto *person = new Human;
        person->setAge((this->borderAdultsToOldmen() + rand() % (100 - this->borderAdultsToOldmen() + 1)));
        person->setTypeAsAWorker(RETIRED);
        auto ptr = shared_ptr<Human>(person);
        this->people.push_back(ptr);
    }
    for(int i = 0; i < this->adults; i++)
    {        // заполняем взрослыми всех остальных людей
        auto* person = new Human;
        person->setAge((this->borderChildrenToAdults()+ rand()% (this->borderAdultsToOldmen() - this->borderChildrenToAdults() + 1)));
        person->setTypeAsAWorker(UNEMPLOYED);
        auto ptr = shared_ptr<Human>(person);
        this->people.push_back(ptr);
    }
    // распределение рабочих по службам
    unsigned int number_of_people_to_service = this->adults / (this->classifications_of_humans.size() - 1);
    int number_of_service = 0;
    int counter = 0;
    for (auto human_it = this->people.begin(); human_it != this->people.end(); human_it++)
    {
        if ((*human_it)->getAge() >= this->borderChildrenToAdults() && (*human_it)->getAge() <= this->borderAdultsToOldmen())
        {
            this->classifications_of_humans[number_of_service].push_back(*human_it);
            (*human_it)->setTypeAsAWorker(WORKER);
            counter ++;
            this->unemployed_people--;
        }
        if (counter >= number_of_people_to_service)
        {
            number_of_service++;
            counter = 0;
        }
        if (number_of_service > this->classifications_of_humans.size() - 2)
            break;
    }
}



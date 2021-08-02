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

Population::Population() : children(0), adults(0), oldmen(0), unemployed_people(0), distribution_coef{0.2, 0.2, 0.3, 0.05, 0.05, 0.2}
{
    
}

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

void Population::staff_distribution(list<shared_ptr<Human>>& staff, unsigned int demand_staff)
{
    auto it = this->people.begin();
    unsigned int counter = 0;
    while(it != this->people.end() && counter < demand_staff)
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
     return this->service_workers;
}

list<shared_ptr<Human>>& Population::getServiceStaff(Services service)
{
     return this->service_workers[service];
}

void Population::processYear() {
    //рождаемость
    for(int i = 0; i < TheArk::get_instance()->getMedicalService()->BirthRate(); i++)
    {
        auto* micro_chelik = new Human();
        micro_chelik->setAge(0);
        micro_chelik->setTypeAsAWorker(CHILD);
        auto ptr = shared_ptr<Human>(micro_chelik);
        this->people.push_back(ptr);
    }
    //распределение стафа, сделал бы в цикле, но нужно вызывать каждую ф-ю отдельно
    unsigned int demand_staff[service_workers.size() - 1];
    demand_staff[0] = TheArk::get_instance()->getTechnicalService()->getStaffDemand();
    demand_staff[1] = TheArk::get_instance()->getBiologicalService()->getStaffDemand();
    demand_staff[2] = TheArk::get_instance()->getMedicalService()->getStaffDemand();
    demand_staff[3] = TheArk::get_instance()->getNavigationService()->getStaffDemand();
    demand_staff[4] = TheArk::get_instance()->getEmergencyService()->getStaffDemand();
    demand_staff[5] = TheArk::get_instance()->getSocialService()->getStaffDemand();
    
    int all_demand_staff = 0;
    for (int i = 0; i < this->service_workers.size() - 1; i++)
    {
        all_demand_staff += demand_staff[i];
    }

    for (int i = 0; i < this->service_workers.size() - 1; i++)
    {
        unsigned int new_staff = (unsigned int)(double(demand_staff[i]) / double(all_demand_staff) * double(this->unemployed_people));
        unsigned int current_staff_of_service = this->service_workers[i].size();
        unsigned int max_staff_of_service = (unsigned int)(this->distribution_coef[i] * this->adults);
        if (current_staff_of_service < max_staff_of_service && new_staff > 1)
        {   
            if (current_staff_of_service + new_staff > max_staff_of_service) 
                new_staff = max_staff_of_service - current_staff_of_service;
            staff_distribution(this->service_workers[i], new_staff);
        }
    }  
        
    //


    // обработка по возрасту и смертность сразу же, чтобы два раза не ходить
    children = 0;
    adults = 0;
    oldmen = 0;
    unemployed_people = 0;
    unsigned int HisAge;
    unsigned int CriticalHealth = TheArk::get_instance()->getMedicalService()->getCriticalHealth();


    for (auto it = people.begin(); it != people.end();)
    {
        // старение
        HisAge = (*it)->getAge();
        (*it)->setAge(HisAge + 1);
        HisAge++;

        if (HisAge == this->borderChildrenToAdults()) (*it)->setTypeAsAWorker(UNEMPLOYED);


        // подсчёт количества населения по группам и обработка случайной смертности
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
            if ((*it)->getTypeAsAWorker() != WORKER)
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

    this->check_dead_people_in_services(); // см коммент в ф-ции

    // тут ещё безусловно нужно написать рождаемость, иначе все подохнут
    // дальше идет просмотр обучающихся и обработка тех, кто отучился. Их пихаем в службы по запросам
    // распределение остальных людей по соответвующим категориям

    //конец обработки
}

void Population::check_dead_people_in_services()
{
    // удаление людей с is_alive == false из всех классификаций
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



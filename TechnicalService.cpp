//
// Created by Amisto on 4/2/2021.
//

#include "TechnicalService.h"
#include "TheArk.h"
#include <cstdlib>
#include <ctime>

TechnicalService::TechnicalService()
{
    this->totalState = 100;
    this->engineState = 100;
    this->protectionState = 100;
    this->serviceState = 0;
    this->maxStaff = 0.1 * TheArk::get_instance()->getPopulation()->getTotal();
    this->maxResources = 200;
    this->staff = 0;
    this->resources = 100;
}

// идет "корректировка" состояния корабля в зависимости от степени аварии
void TechnicalService::process_accident(AccidentSeverity as)
{
    srand(time(nullptr));
    this->protectionState -= as * 0.5; //подумать коэф
    this->engineState -= as * (100 - this->protectionState) * 0.1;
    if (this->protectionState < 15)
        emergencyRepair();
    // kill some people
    kill((rand() % 10) / 100 * this->staff);
}

void TechnicalService::kill(int victims)
{
    list<shared_ptr<Human>>& humans = TheArk::get_instance()->getPopulation()->getServiceStaff(Technical_Service);
    auto it = humans.begin();
    for (int i = 0; i < victims; i ++)
    {
        (*it)->setIsAlive(false);
        it ++;
    }
}

// считает и возвращает состояние службы в зависимости от количества работающих людей и
double TechnicalService::getState() {
    return this->serviceState;
}

void TechnicalService::emergencyRepair()
{
    srand(time(nullptr));
    double repairing = double(this->staff) / this->maxStaff * double(this->resources) / this->maxResources * 50;
    this->protectionState += repairing;
    if (this->resources - int(repairing / 100) * maxResources > 0)
    {
        this->protectionState += repairing;
        this->resources -= int(repairing / 100) * maxResources;
    }
    // убить много людей, так как экстренная и масштабная починка
    kill((rand() % 3 + 1) / 10 * this->staff);
}

void TechnicalService::process_year()
{
    srand(time(nullptr));

    // обновление полей
    this->staff = TheArk::get_instance()->getPopulation()->getServiceStaff(Technical_Service).size();
    this->totalState = 0.5 * (0.8 * this->protectionState + 1.2 * this->engineState);
    this->maxStaff = 0.1 * TheArk::get_instance()->getPopulation()->getTotal();

    // обновление состояния службы
    if (this->totalState > 100)
        this->totalState = 100;
    if (this->totalState < 0)
        this->totalState = 0;
    this->serviceState = (int)this->totalState;

    // починка корабля
    double repairing = double(this->staff) / this->maxStaff * double(this->resources) / this->maxResources * 10;
    if (this->protectionState < 90 && this->resources - int(repairing / 100) * this->maxResources > 0)
    {
        this->protectionState += repairing;
        this->resources -= int(repairing / 100) * this->maxResources;
        // можно убить пару людей в зависимости от масштаба ремонта
    }
    if (this->protectionState > 60 && this->engineState < 90 && this->resources - int(repairing / 100) * this->maxResources > 0)
    {
        this->engineState += repairing;
        this->resources -= int(repairing / 100) * this->maxResources;
    }
    // износ корабля
    this->protectionState -= (101 - this->protectionState) * (rand() % 3) / 100;
    srand(time(nullptr));
    this->engineState -= (100 - this->protectionState) * (rand() % 3) / 100;
}

double TechnicalService::efficiencyConsumablesToComponents() {
    return double(this->staff) / this->maxStaff * 0.9;
}

double TechnicalService::efficiencyJunkToConsumables() {
    return double(this->staff) / this->maxStaff * 0.7;
}

double TechnicalService::efficiencyJunkToRefuse() {
    return double(this->staff) / this->maxStaff * 0.5;
}

void TechnicalService::setState(double s)
{
    this->serviceState = s;
}

unsigned int TechnicalService::getResourceDemand() {
    return this->maxResources - this->resources;
}

unsigned int TechnicalService::getResourcePriority() {
    return 1;
}

unsigned int TechnicalService::getStaffDemand() {
    if (this->maxStaff - this->staff > 0)
        return this->maxStaff - this->staff;
    else
        return 0;
}

unsigned int TechnicalService::getStaffPriority() {
    return 1;
}

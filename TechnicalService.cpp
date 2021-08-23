//
// Created by Amisto on 4/2/2021.
//

#include "TechnicalService.h"
#include "TheArk.h"
#include "Resources.h"
#include <cstdlib>
#include <iostream>
#include <ctime>

TechnicalService::TechnicalService() 
{
    this->totalState      = 100;
    this->engineState     = 100;
    this->protectionState = 100;
    this->serviceState    = 100;
    this->maxStaff        = 1;
    this->maxResources    = 500;
    this->demand_resources= 0;
    this->staff           = 0;
    this->resources       = 50000;
    this->junk            = 0;
}

// идет "корректировка" состояния корабля в зависимости от степени аварии
void TechnicalService::process_accident(AccidentSeverity as)
{
    this->protectionState -= as * 0.5; //подумать коэф
    this->engineState     -= as * (100 - this->protectionState) * 0.1;

    if (this->protectionState < CRITICAL_STATE)
        this->emergencyRepair();

    // kill 1%-as% of all staff
    kill((unsigned int)(random.getRandomDouble(1, as) / 100 * this->staff));
}

void TechnicalService::kill(unsigned int victims)
{
    list<shared_ptr<Human>>& humans = TheArk::get_instance()->getPopulation()->getServiceStaff(Technical_Service);

    auto it = humans.begin();
    for (int i = 0; i < victims; i++)
    {
        (*it)->setIsAlive(false);
        it++;
    }
}

// считает и возвращает состояние службы в зависимости от количества работающих людей и
double TechnicalService::getState() 
{
    return this->serviceState;
}

// экстренный ремонт при чрезвычайной ситуации
void TechnicalService::emergencyRepair()
{
    // calculating quantity
    double repair = double(this->staff) / this->maxStaff * 10;

    if (this->resources > int(repair * RESOURCES_PER_PERCENT))
    {
        this->protectionState += repair;
        this->resources -= int(repair * RESOURCES_PER_PERCENT);
        this->junk += AMOUNT_OF_JUNK_AFTER_COMPONENTS * int(repair * RESOURCES_PER_PERCENT);
        this->demand_resources += int(repair * RESOURCES_PER_PERCENT);
    }

    // убить много людей, так как экстренная и масштабная починка
    kill(random.getRandomDouble(0.1, 0.3) * this->staff);
}

void TechnicalService::process_year()
{
    // обновление полей
    this->staff = TheArk::get_instance()->getPopulation()->getServiceStaff(Technical_Service).size();
    this->totalState = 0.5 * (0.8 * this->protectionState + 1.2 * this->engineState);
    this->maxStaff = PROPOTION_OF_PEOPLE * TheArk::get_instance()->getPopulation()->getAdults();
    //this->resources = TheArk::get_instance()->getResources()->getUsedByService(Technical_Service);   

    // обновление состояния службы
    if (this->totalState > 100)
        this->totalState = 100;
    if (this->totalState < 0)
        this->totalState = 0;
    this->serviceState = (int)this->totalState;

    // ежегодная починка корабля
    double repair = double(this->staff) / this->maxStaff * 5;
    if (this->protectionState < 90 && this->resources > int(repair * RESOURCES_PER_PERCENT))
    {
        this->protectionState += repair;
        this->resources -= int(repair * RESOURCES_PER_PERCENT);
        this->junk += AMOUNT_OF_JUNK_AFTER_COMPONENTS * int(repair * RESOURCES_PER_PERCENT);
        this->demand_resources += int(repair * RESOURCES_PER_PERCENT);
        // можно убить пару людей в зависимости от масштаба ремонта
    }
    if (this->protectionState > 60 && this->engineState < 90 && this->resources > int(repair * RESOURCES_PER_PERCENT))
    {
        this->engineState += repair;
        this->resources -= int(repair * RESOURCES_PER_PERCENT);
        this->junk += AMOUNT_OF_JUNK_AFTER_COMPONENTS * int(repair * RESOURCES_PER_PERCENT);
        this->demand_resources += int(repair * RESOURCES_PER_PERCENT);
    }

    // износ корабля
    this->protectionState -= random.getRandomDouble(1.0, 1.5);
    this->engineState     -= (100 - this->protectionState) * random.getRandomDouble(0.01, 0.1); 
}

double TechnicalService::efficiencyConsumablesToComponents() {
    return double(this->staff) / this->maxStaff * 0.5;
}

double TechnicalService::efficiencyJunkToConsumables() {
    return double(this->staff) / this->maxStaff * 0.3;
}

double TechnicalService::efficiencyJunkToRefuse() {
    return double(this->staff) / this->maxStaff * 0.5;
}

void TechnicalService::setState(double s)
{
    this->engineState = s;
    this->protectionState = s;
}

unsigned int TechnicalService::getResourceDemand() {
    unsigned int tmp = this->demand_resources;
    this->demand_resources = 0;
    return tmp;
}

unsigned int TechnicalService::returnJunk() {
    unsigned int tmp = this->junk;
    this->junk = 0;
    return tmp;
}

unsigned int TechnicalService::getResourcePriority() {
    return 1;
}

unsigned int TechnicalService::getStaffDemand() {
    if (this->maxStaff > this->staff)
        return this->maxStaff - this->staff;
    else
        return 0;
}

unsigned int TechnicalService::getStaffPriority() {
    return 1;
}

double TechnicalService::getEngineState() const {
    return engineState;
}

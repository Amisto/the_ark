#include "TheArk.h"
#include "TechnicalService.h"
#include "BiologicalService.h"
#include "MedicalService.h"
#include "NavigationService.h"
#include "EmergencyService.h"
#include "SocialService.h"
#include "Population.h"
#include "Resources.h"
#include "Interface.h"
#include <iostream>

TheArk* TheArk::instance = nullptr;

TheArk* TheArk::get_instance()
{
    if (instance)
        return instance;
    instance = new TheArk();
    return instance;
}

TheArk::TheArk() : years_total(0),current_year(0)
{
    for (auto &s: services)
        s = nullptr;
    population = nullptr;
    resources = nullptr;
}

void TheArk::deleteArk()
{
    delete instance;
    instance = nullptr;
}

TheArk::~TheArk()
{
    for (auto &s: services) {
        delete s;
        s = nullptr;
    }
    delete population;
    delete resources;
}

void TheArk::init()
{
    interface = new Interface();
    interface->init();

    std::array<std::string, 6> services_names = {"Technical", "Biological",
                                                 "Medical", "Navigation",
                                                 "Emergency", "Social"};

    population = new Population();
    resources = new Resources();
    services[0] = new TechnicalService();
    services[1] = new BiologicalService();
    services[2] = new MedicalService();
    services[3] = new NavigationService();
    services[4] = new EmergencyService();
    services[5] = new SocialService();

    years_total = std::stoi(interface->getGeneral()["Years"]);

    for (int i = 0; i < 6; ++i)
    {
        services[i]->setState(std::stoi(interface->getServices()[i]["State"]));
    }

    population->init(std::stoi(interface->getGeneral()["Population"]));
    resources->init(std::stoi(interface->getGeneral()["Resources"]));
}

unsigned int TheArk::getYearsTotal() const {
    return years_total;
}

void TheArk::setYearsTotal(unsigned int yearsTotal) {
    years_total = yearsTotal;
}

unsigned int TheArk::getCurrentYear() const {
    return current_year;
}

void TheArk::setCurrentYear(unsigned int currentYear) {
    current_year = currentYear;
}

TechnicalService* TheArk::getTechnicalService()
{
    return dynamic_cast<TechnicalService*>(services[0]);
}

BiologicalService* TheArk::getBiologicalService()
{
    return dynamic_cast<BiologicalService*>(services[1]);
}

MedicalService* TheArk::getMedicalService()
{
    return dynamic_cast<MedicalService*>(services[2]);
}

NavigationService* TheArk::getNavigationService()
{
    return dynamic_cast<NavigationService*>(services[3]);
}

EmergencyService* TheArk::getEmergencyService()
{
    return dynamic_cast<EmergencyService*>(services[4]);
}

SocialService* TheArk::getSocialService()
{
    return dynamic_cast<SocialService*>(services[5]);
}

Interface* TheArk::getInterface()
{
    return interface;
}

void TheArk::processYear() {
    population->processYear();
    resources->processYear();
    for (auto s: services)
    {
         s->process_year();
    }
    // перераспределение ресурсов
    // перераспределение работников
}

void TheArk::flight() {

    for (current_year = 0; current_year < years_total; current_year++)
    {
        processYear();
        interface->snap(current_year + 1);
    }
}

Population *TheArk::getPopulation() const {
    return population;
}

Resources *TheArk::getResources() const {
    return resources;
}

std::array<Service*, 6>& TheArk::getServices()
{
    return services;
}

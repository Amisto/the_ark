//
// Created by Amisto on 4/2/2021.
//

#ifndef THE_ARK_EMERGENCYSERVICE_H
#define THE_ARK_EMERGENCYSERVICE_H

#include <ctime>
#include <cmath>
#include "Service.h"
#include "Enums.cpp"
#include "TheArk.h"
#include "MedicalService.h"
#include "NavigationService.h"
#include "BiologicalService.h"
#include "SocialService.h"
#include "TechnicalService.h"
#include "Human.h"
#include "Population.h"
#include "Resources.h"


class EmergencyService : public Service
{
private:
    double State;

    unsigned int Staff; //персонала в данный момент
    unsigned int max_Staff;//максимальное количество персонала в службе
    unsigned int Resources;
    unsigned int Junk;
    unsigned int max_Resources;

public:
    EmergencyService();

    unsigned int getNStaff();
    double accident_probability();
    double damage_factor();
    void determine_severity(Service* s);
    void create_accident(Service* s);
    void process_accident(AccidentSeverity as) override;    // каждая служба должна уметь в своих терминах обработать переданную ей аварию
    void process_year() override;                   // если у службы есть какая-то личная жизнь, она может заниматься ей тут
    double getState() override;                             // каждая служба должна уметь вернуть свое состояние в процентах, посчитав его в своих терминах
    void setState(double s) override;                       // функция для инициализации, каждая служба должна уметь получить состояние в процентах и пересчитать  его в своих терминах
    
    unsigned int getStaffDemand() override;
    unsigned int getResourceDemand() override;
    unsigned int returnJunk() override;

    bool changeResources(int delta);
    bool changeStaff(int delta);

};


#endif //THE_ARK_EMERGENCYSERVICE_H

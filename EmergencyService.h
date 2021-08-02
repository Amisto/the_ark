//
// Created by Amisto on 4/2/2021.
//

#ifndef THE_ARK_EMERGENCYSERVICE_H
#define THE_ARK_EMERGENCYSERVICE_H

#include <ctime>
#include <cmath>
#include <iostream>
#include "Service.h"
#include "Enums.cpp"
#include "TheArk.h"
#include "Interface.h"
//#include "MedicalService.h"
//#include "NavigationService.h"
//#include "BiologicalService.h"
//#include "SocialService.h"
//#include "TechnicalService.h"
//#include "Human.h"
//#include "Population.h"
#include "Resources.h"
#include "RandomNumberGenerator.h"


class EmergencyService : public Service
{
private:
    double state;

    unsigned int staff; // Current staff amount
    unsigned int max_staff; // Staff has to be <= this value
    unsigned int Junk;

    unsigned int resources;
    unsigned int max_resources;

public:
    EmergencyService();

    void process_accident(AccidentSeverity as) override;    // каждая служба должна уметь в своих терминах обработать переданную ей аварию
    void process_year() override;                   // если у службы есть какая-то личная жизнь, она может заниматься ей тут
    double getState() override;                             // каждая служба должна уметь вернуть свое состояние в процентах, посчитав его в своих терминах
    void setState(double s) override;                       // функция для инициализации, каждая служба должна уметь получить состояние в процентах и пересчитать  его в своих терминах
    unsigned int getStaffDemand() override;
    unsigned int getResourceDemand() override;
    unsigned int returnJunk() override;

    double damage_factor(Service* s);
    void determine_severity(Service* s);
    void create_accident(Service* s);

    void changeResources(int delta);
    void killStaff(int delta); // Has protection from delta > current Staff value

};


#endif //THE_ARK_EMERGENCYSERVICE_H

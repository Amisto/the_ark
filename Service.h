//
// Created by Amisto on 4/2/2021.
//

#ifndef THE_ARK_SERVICE_H
#define THE_ARK_SERVICE_H

#include "Population.h"
#include "Enums.cpp"

class Service {
private:
    unsigned int recommended_n_staff; // amount of workers
    unsigned int recommended_resources;

public:
    Service();
    unsigned int getRecommendedNStaff() const;

    virtual void process_year() = 0;

    virtual void process_accident(AccidentSeverity as) = 0; // каждая служба должна уметь в своих терминах обработать переданную ей аварию
    virtual double getState() = 0;                          // каждая служба должна уметь вернуть свое состояние в процентах, посчитав его в своих терминах
    virtual void setState(double s) = 0;                    // функция для инициализации, каждая служба должна уметь получить состояние в процентах и пересчитать  его в своих терминах
    virtual unsigned int getResourceDemand() = 0;           // сколько ресурсов требуется
    virtual unsigned int returnJunk() = 0;		    // сколько возвращает мусора
    virtual unsigned int getStaffDemand() = 0;              // сколько людей требуется

    virtual ~Service();
};

#endif //THE_ARK_SERVICE_H

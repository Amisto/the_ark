//
// Created by Amisto on 4/2/2021.
//

#ifndef THE_ARK_TECHNICALSERVICE_H
#define THE_ARK_TECHNICALSERVICE_H

#include "Service.h"
#include "Human.h"
#include <vector>

class TechnicalService : public Service {
private:
    double totalState;                // общее состояние корабля
    double engineState;               // состояние двигателя
    double protectionState;           // состояние защиты корабля
    double serviceState;           // состояние службы
    unsigned int resources;        // количество нужных ресурсов в процентах
    unsigned int staff;
    unsigned int maxStaff;        // желательное количество людей
    unsigned int maxResources;     // желательное количество ресурсов

public:
    TechnicalService();

    void process_accident(AccidentSeverity as) override;    // каждая служба должна уметь в своих терминах обработать переданную ей аварию
    void process_year() override;                        // если у службы есть какая-то личная жизнь, она может заниматься ей тут
    double getState() override;                             // каждая служба должна уметь вернуть свое состояние в процентах, посчитав его в своих терминах
    void setState(double s) override;                       // функция для инициализации, каждая служба должна уметь получить состояние в процентах и пересчитать  его в своих терминах
    void emergencyRepair ();

    double efficiencyConsumablesToComponents();     // как быстро расходники перерабатываются в компоненты
    double efficiencyJunkToConsumables();           // как быстро хлам перерабатываются в расходники
    double efficiencyJunkToRefuse();                // как быстро хлам перерабатываются в отходы

    virtual unsigned int getResourceDemand() override;           // сколько ресурсов требуется
    virtual unsigned int getResourcePriority() override;         // с каким приоритетом служба будет требовать ресурсы
    virtual unsigned int getStaffDemand() override;              // сколько людей требуется
    virtual unsigned int getStaffPriority() override;            // с каким приоритетом слуюба будет требовать людей
    virtual bool changeStaff (int delta) override;               // сколько людей добавили или забрали (в т.ч. смертность)
    virtual bool changeResources(int delta) override;            // сколько ресурсов добавили или забрали (в т.ч. износ)

};


#endif //THE_ARK_TECHNICALSERVICE_H

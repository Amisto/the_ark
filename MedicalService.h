//
// Created by Amisto on 4/2/2021.
//

#ifndef THE_ARK_MEDICALSERVICE_H
#define THE_ARK_MEDICALSERVICE_H

#include "Service.h"
#include <list>

using std::list;

class MedicalService : public Service {
private:
    unsigned int n_staff;                                 // количество рабочих всего
    unsigned int resources;                               // количество задействованных ресурсов
    double State;                                         // состояние системы по 100-бальной шкале
    unsigned int retirementAge;                           // возраст старения
    double ChildrenDeath;                                 // средняя вероятность, что один ребенок подгибнет в течение года
    double AdultDeath;                                    // средняя вероятность, что один взрослый подгибнет в течение года
    double OldDeath;                                      // средняя вероятность, что один старик подгибнет в течение года
    double Birth{};                                         // рождаемость в год
public:
    MedicalService();

    void process_accident(AccidentSeverity as) override;  // каждая служба должна уметь в своих терминах обработать переданную ей аварию
    void process_year() override;                         // если у службы есть какая-то личная жизнь, она может заниматься ей тут
    double getState() override;                           // каждая служба должна уметь вернуть свое состояние в процентах, посчитав его в своих терминах
    void setState(double s) override;                     // функция для инициализации, каждая служба должна уметь получить состояние в процентах и пересчитать  его в своих терминах

    unsigned int borderAdultsToOldmen() const;                  // возраст старения
    double deathRateChildren() const;                           // средняя вероятность, что один ребенок подгибнет в течение года
    double deathRateAdult() const;                              // средняя вероятность, что один взрослый подгибнет в течение года
    double deathRateOldmen() const;                             // средняя вероятность, что один старик подгибнет в течение года
    double BirthRate() const;                                   // рождаемость в год

    unsigned int getResourceDemand() override;            // сколько ресурсов требуется
    unsigned int getResourcePriority() override;          // с каким приоритетом служба будет требовать ресурсы
    unsigned int getStaffDemand() override;               // сколько людей требуется
    unsigned int getStaffPriority() override;             // с каким приоритетом слуюба будет требовать людей
    bool changeStaff(int delta) override;                 // сколько людей добавили или забрали (в т.ч. смертность)
    bool changeResources(int delta) override;             // сколько ресурсов добавили или забрали (в т.ч. износ)

};


#endif //THE_ARK_MEDICALSERVICE_H

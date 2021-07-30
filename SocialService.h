//
// Created by Amisto on 4/2/2021.
//

// ТЕРМИНОЛОГИЯ: ПСИХОЛОГИ - это ученые и инженеры, УЧЕНЫЕ - это ученые

#ifndef THE_ARK_SOCIALSERVICE_H
#define THE_ARK_SOCIALSERVICE_H

#include "TheArk.h"
#include "Service.h"
#include "Population.h"
#include <queue>
#include <stdlib.h>

using std::priority_queue;

class ComparePersonsByMentalHealth{
public:
    bool operator() (const Human* per1, const Human* per2)
    {
        return per1->getMoralHealth() < per2->getMoralHealth();
    }
};

class SocialService : public Service {
private:
    unsigned int efficiency_percentage; // эффективность работы одного психолога в количестве вылечивших за год
    unsigned int n_years_of_education;
    unsigned int suicide_counter;

    unsigned int count_all_accident_severity = 0; // полное число поступивих чрезвычайных событий
    unsigned int count_resolved_accident_severity = 0; // число чрезвычайных событий, которые удалось успешно решить
    unsigned int count_unresolved_accident_severity = 0; // число чрезвычайных событий, которые не удалось успешно решить

    unsigned int n_staff_we_want{};

    void update_n_staff_we_want();
    void update_person(Human& person);
    void update_people();
public:
    SocialService();
    void process_accident(AccidentSeverity as) override;    // каждая служба должна уметь в своих терминах обработать переданную ей аварию
    void process_year() override;                        // если у службы есть какая-то личная жизнь, она может заниматься ей тут
    double getState() override;                             // каждая служба должна уметь вернуть свое состояние в процентах, посчитав его в своих терминах
    void setState(double s) override;                       // функция для инициализации, каждая служба должна уметь получить состояние в процентах и пересчитать  его в своих терминах

    unsigned int getResourceDemand() override;           // сколько ресурсов требуется
    unsigned int returnJunk() override;		// сколько мусора вернули
    unsigned int getResourcePriority();         // с каким приоритетом служба будет требовать ресурсы
    unsigned int getStaffDemand() override;              // сколько людей требуется
    unsigned int getStaffPriority();            // с каким приоритетом слуюба будет требовать людей
    bool changeStaff (int delta) ;               // сколько людей добавили или забрали (в т.ч. смертность)
    bool changeResources(int delta);            // сколько ресурсов добавили или забрали (в т.ч. износ)

    unsigned int borderChildrenToAdults();
};


#endif //THE_ARK_SOCIALSERVICE_H

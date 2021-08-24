//
// Created by Amisto on 4/2/2021.
//

// ТЕРМИНОЛОГИЯ: ПСИХОЛОГИ - это ученые и инженеры, УЧЕНЫЕ - это ученые

/*TODO Make an education system */

#ifndef THE_ARK_SOCIALSERVICE_H
#define THE_ARK_SOCIALSERVICE_H

#include "TheArk.h"
#include "Service.h"
#include "Population.h"
#include <queue>

using std::priority_queue;

class ComparePersonsByMentalHealth{
public:
    bool operator() (const std::shared_ptr<Human>& per1, const std::shared_ptr<Human>& per2)
    {
        return per1->getMentalHealth() > per2->getMentalHealth();
    }
};

class SocialService : public Service {
private:
    unsigned int n_years_of_education;
    unsigned int suicide_counter;
    unsigned state;

    priority_queue<std::shared_ptr<Human>, std::vector<shared_ptr<Human>>, ComparePersonsByMentalHealth> clients;

    unsigned int count_all_accident_severity = 0; // полное число поступивих чрезвычайных событий
    unsigned int count_resolved_accident_severity = 0; // число чрезвычайных событий, которые удалось успешно решить
    unsigned int count_unresolved_accident_severity = 0; // число чрезвычайных событий, которые не удалось успешно решить

    //---------Special Integers-------//
    unsigned n_staff_we_want;
    unsigned n_resources_we_want;
    unsigned n_ills;
    unsigned border_children_to_adults;
    unsigned eff_of_one_psyh;
    unsigned dynamic_koeffisient_of_border_children_to_adults;

    void setState();
    void updatePerson(std::shared_ptr<Human> person);
    void updatePeople();
    void fixPeople();
    void updateBorderChildrenToAdults();
    unsigned countWorkableStaff(list<shared_ptr<Human>>& list_of_pep);
    unsigned countFreeWorkableStaff();
public:
    SocialService();
    void process_accident(AccidentSeverity as) override;    // каждая служба должна уметь в своих терминах обработать переданную ей аварию
    void process_year() override;                        // если у службы есть какая-то личная жизнь, она может заниматься ей тут
    double getState() override;                             // каждая служба должна уметь вернуть свое состояние в процентах, посчитав его в своих терминах
    void setState(double s) override;                       // функция для инициализации, каждая служба должна уметь получить состояние в процентах и пересчитать  его в своих терминах

    unsigned int getResourceDemand() override;           // сколько ресурсов требуется
    unsigned int returnJunk() override;		// сколько мусора вернули
    unsigned int getStaffDemand() override;              // сколько людей требуется

    unsigned int borderChildrenToAdults();

    void setSpecialIntegers();
};


#endif //THE_ARK_SOCIALSERVICE_H

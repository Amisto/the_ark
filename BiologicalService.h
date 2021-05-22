//
// Created by Amisto on 4/2/2021.
//

#ifndef THE_ARK_BIOLOGICALSERVICE_H
#define THE_ARK_BIOLOGICALSERVICE_H

#include "Service.h"

class BiologicalService : public Service {
private:
    int BioState;
public:
    BiologicalService();

    void process_accident(AccidentSeverity as) override;    // каждая служба должна уметь в своих терминах обработать переданную ей аварию
    void process_year() override {};                       // если у службы есть какая-то личная жизнь, она может заниматься ей тут
    double getState() override;                             // каждая служба должна уметь вернуть свое состояние в процентах, посчитав его в своих терминах
    void setState(double s) override;                       // функция для инициализации, каждая служба должна уметь получить состояние в процентах и пересчитать  его в своих терминах
};

class BioRes
{
private:
int k,mass,junk,worker_0;
double hp,KPD;
public:
BioRes(unsigned int start_mass,int consume_for1);
RecycleRes(int worker,int Energy,int people);
profit(int worker);
BioRes::GetResource();

BioRes::GetJunk();

BioRes::Gethp();

BioRes::Sethp(double health_of_system);

BioRes::GetPeople(int worker):


}

struct coord;

int GetTypeId (int x);

#endif //THE_ARK_BIOLOGICALSERVICE_H

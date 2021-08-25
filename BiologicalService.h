#ifndef THE_ARK_BIOLOGICALSERVICE_H
#define THE_ARK_BIOLOGICALSERVICE_H

#include "Service.h"

class BiologicalService : public Service {
private:
    int BioState;
public:
    BiologicalService();

    void process_accident(AccidentSeverity as) override;   
    void
    process_year() override;
    double
    getState() override;                             
    void setState(double s) override;                    
    unsigned int getResourceDemand() override {return 0;};
    unsigned int getStaffDemand() override {return 0;};
    unsigned int returnJunk() override {return 0;};
    class BioRes {
    private:
        int k, mass, junk, worker_0;
        double hp, KPD;
    public:
        BioRes(unsigned int start_mass, double KPD_e, int worker_optimum, int max_profit, double health_of_system,
               int consume_for1);

        void RecycleRes(int worker, int Energy, int people);

        int profit(int worker);

        int GetResource();

        double GetJunk();

        double Gethp();

        void Sethp(double health_of_system);

        int GetPeople(int worker);
    };

};



struct coord;

int GetTypeId(int x);

#endif //THE_ARK_BIOLOGICALSERVICE_H

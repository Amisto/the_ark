//
// Created by Amisto on 4/2/2021.
//

#include "BiologicalService.h"
#include "TheArk.h"
#include "Resources.h"
#include <iostream>
#include <cmath>
#include <vector>
#include <ctime>
#include <cstdlib>

BiologicalService::BiologicalService()
{

}

void BiologicalService::process_accident(AccidentSeverity as)
{
// take parametr of disaster and analyze it - not did
}

struct coord
{
    int x;
    int y;
};

int GetTypeId (int x)
{
    int c = 10;
    while( x > 9)
    {
        x = x / 10;
        c = c * 10;
    }
    return c;
}

BiologicalService::BioRes::BioRes(unsigned int start_mass,double KPD_e,int worker_optimum,int max_profit,double health_of_system, int consume_for1)
{
    mass=start_mass; //set start mass of resource
    int k=consume_for1; //consume of 1 human per year
    junk=0; //CO2 for air, poor water for water, composte for eat
    KPD=KPD_e;
    worker_0=worker_optimum;
    hp=health_of_system;
/* KPD_mass=effencecy_mass; //junk*KPD=profit
KPD_energy=effencecy_energy; //Energy*KPD=profit */
}

int BiologicalService::BioRes::profit(int worker)
{
if (KPD*hp*worker/worker_0<junk)
    {return KPD*hp*worker/worker_0;}
else
    {return junk;}
}

void BiologicalService::BioRes::RecycleRes(int worker,int Energy,int people) //how mass change
{
    if (0<mass+profit(worker)-people*k)
{
    mass=profit(worker)+mass-people*k; //production per year
    junk=junk+people*k;
}
    else
{
/*TheArk::get_instance()->getResources()->setUsedToConsumables(-profit(worker)-mass+people*k); //take resources from main store (Alena's)*/
    junk=junk+people*k;
    mass=0;
}

}
int BiologicalService::BioRes::GetPeople(int worker)
{
return int(KPD*hp*worker/worker_0/k);
}
int BiologicalService::BioRes::GetResource()
    {return mass;}
double BiologicalService::BioRes::GetJunk()
    {return junk;}
double BiologicalService::BioRes::Gethp()
    {return hp;}
void BiologicalService::BioRes::Sethp(double health_of_system)
    {hp=health_of_system;}

class Animal {
public:
    char name;
    int id_biom;
    int heal;
    bool predator;
    bool haveBaby=false;
    void take_food(int feed) {

    };
};
class Pig : public Animal{
public:
    Pig()
    {
        this->name='P';
        this->heal=5;
        this->predator = true;
    };
};
class Cow : public Animal{
public:
    Cow()
    {
        this->name='C';
        this->heal=10;
        this->predator = false;
    };
};
class Wolf : public Animal
{
public:
    Wolf()
    {
        this->name='W';
        this->heal = 0;
        this->predator = true;
    }
};
class Biom
{
public:
    double water; // <= 10000, if water 0 -> no animals
    double land; // <= 10000  if land 0 -> no animals
    double forest; // <= 10000, if fprest 0 -> no animals
    int id; //
    int free = 0; //value of free spaces
    bool init = false; // was init?

    Biom()
    {
        water = 0;
        forest = 0;
        id = 0;
    };

    Biom (double water, double forest, int id)
    {
        this->water = water;
        this->forest = forest;
        this->id = id;
    };

    ~Biom () = default;

    void InitFree() // Init function
    {
        double drink = water / 7;
        double food = forest / 9;
        double square = land / 100;
        free = int( std::min(square, std::min(drink, food) ) );
        init = true;
    }

};

const int x = 5;
const int y = 5;

void process_year()
{
    double w, f, l;
    int n;
    n = 0;
    int i0 = 0;
    int c = GetTypeId(x);

    Biom territory[x][y];

    vector <coord> coords(x*y);

    srand(time(0));
// Create bioms
    for(int i = 0; i < x; ++i)
        for(int j = 0; j < y; ++j)
        {
            w = 51;
            f = 45;
            l = 341;
            territory[i][j].water = w;
            territory[i][j].forest = f;
            territory[i][j].land = l;
            territory[i][j].id = 1 + j + c * (i + 1);
            territory[i][j].free = 1 + rand() % 5;
            n = n + territory[i][j].free;
            coords[i0].x = i;
            coords[i0].y = j;
            ++i0;
        }
// Randomize sequence of bioms for settlement of animals
    for(int i = 0; i < coords.size(); ++i)
    {
        int r = + rand() % ( coords.size() - i );
        std::swap(coords[i], coords[r]);
    };

    // Create vector of animals, add grazing
    vector <Animal> Animals;
    for(int i = 0; i < n - 11; ++i)
    {
        if(rand() % 2)
            Animals.push_back(Pig());
        else
            Animals.push_back(Cow());
    }
// Add predators in vector
    for(int i = 0; i < 11; ++i)
        Animals.push_back(Wolf());
// Randomize vector of animals
    for(int i = 0; i < Animals.size(); ++i)
    {
        int r = + rand() % ( Animals.size() - i );
        std::swap(Animals[i], Animals[r]);
    };

    // Add to animals ids of their bioms
    i0 = 0;
    for(int i = 0; i < Animals.size(); ++i)
    {
        if( territory[coords[i0].x][coords[i0].y].free > 0)
        {
            Animals[i].id_biom = territory[coords[i0].x][coords[i0].y].id;
            --territory[coords[i0].x][coords[i0].y].free;
        }
        else
        {
            ++i0;
            --i;
        }
    };
    // Erase of dead grazing animals
    for(int i = 0; i < Animals.size(); ++i)
    {
        if((Animals[i].heal <= 0)&&(Animals[i].predator==false))
        {
            Animals.erase(Animals.begin() + i);
            --i;
        }
    };
// Eating grazing by predators
    int xi, yi, p;
    for (int i = 0; i < Animals.size(); ++i) {
        if ((Animals[i].heal <= 0) && (Animals[i].predator)) {
            for (int j = 0; j < Animals.size(); ++j) {
                if (((abs(Animals[i].id_biom - Animals[j].id_biom) == 1) ||
                     (abs(Animals[i].id_biom - Animals[j].id_biom) == 10) || (Animals[i].id_biom == Animals[j].id_biom))&&
                    (Animals[j].predator==false)) {
                    Animals[i].heal=Animals[i].heal+1;
                    xi=((Animals[j].id_biom)/10-1);
                    yi=((Animals[j].id_biom)%10-1);
                    ++territory[xi][yi].free;
                    Animals.erase(Animals.begin() + j);
                    --i;
                    --j;
                };
            };
        };
    };
// Erase of dead predators
    for(int i = 0; i < Animals.size(); ++i)
    {
        if((Animals[i].heal <= 0)&&(Animals[i].predator==true))
        {
            Animals.erase(Animals.begin() + i);
            --i;
        }
    };
    // Reproduction of animals
    p = Animals.size();
    for (int i = 0; i < p; ++i) {
        for (int j = 0; j < p; ++j) {
            xi=((Animals[i].id_biom)/10-1);
            yi=((Animals[i].id_biom)%10-1);
            if((Animals[i].id_biom==Animals[j].id_biom)&&(Animals[i].name==Animals[j].name)&&(territory[xi][yi].free>0)&&(Animals[j].haveBaby==false)&&(Animals[i].haveBaby==false)&&(i != j)){
                if (Animals[i].name == 'W')
                {
                    Animals[i].haveBaby=Animals[j].haveBaby=true;
                    Animals.push_back(Wolf());
                    Animals[Animals.size()-1].id_biom=Animals[i].id_biom;
                    Animals[Animals.size()-1].haveBaby=true;
                    --territory[xi][yi].free;
                };
                if(Animals[i].name == 'C')
                {
                    Animals[i].haveBaby=Animals[j].haveBaby=true;
                    Animals.push_back(Cow());
                    Animals[Animals.size()-1].id_biom=Animals[i].id_biom;
                    Animals[Animals.size()-1].haveBaby=true;
                    --territory[xi][yi].free;
                };
                if(Animals[i].name == 'P')
                {
                    Animals[i].haveBaby=Animals[j].haveBaby=true;
                    Animals.push_back(Pig());
                    Animals[Animals.size()-1].id_biom=Animals[i].id_biom;
                    Animals[Animals.size()-1].haveBaby=true;
                    --territory[xi][yi].free;
                };
            };
        };
    };
}

double BiologicalService::getState() {
    return BioState;  // give state in procents
}

void BiologicalService::setState(double s) {
    BioState = s;   // initilize statement
}

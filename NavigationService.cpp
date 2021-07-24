//
// Created by Amisto on 4/2/2021.
//

#include "NavigationService.h"
//double(TheArk::get_instance()->getPopulation()->getAllClassification()[4].size())

NavigationService::NavigationService():State(100),staff(200),
resources(static_cast<unsigned int>(0.2*(TheArk::get_instance()->getResources()->getComponents()+TheArk::get_instance()->getResources()->getConsumables()))),
need_staff(200),
need_resources(static_cast<unsigned int>(0.2*(TheArk::get_instance()->getResources()->getComponents()+TheArk::get_instance()->getResources()->getConsumables()))),
stage(0),lost_years(0)
{

}

void NavigationService::process_accident(AccidentSeverity as){int k=0;
    if(AccidentSeverity::NEGLIGIBLE == as){
        k = 1;


    }
    if(AccidentSeverity::LIGHT == as){
        k = 2 ;


    }
    if(AccidentSeverity::MEDIUM == as){
        k = 3 ;


    }
    if(AccidentSeverity::SEVERE == as){
        k = 4;


    }
    if(AccidentSeverity::DISASTROUS == as){
        k = 5;


    }
    if(AccidentSeverity::CATASTROPHIC == as){
        k = 6;


    }
    this->lost_years += k*(double(TheArk::get_instance()->getYearsTotal()-TheArk::get_instance()->getCurrentYear())/125);
    this->changeResources(-k * TheArk::get_instance()->getResources()->getComponents()/100);
    this->changeStaff(k*2);

}
void NavigationService::process_year(){
    this->stage = round((TheArk::get_instance()->getCurrentYear()/TheArk::get_instance()->getYearsTotal())*100);
    int f=0;
    /*f=rand() % 100 + 1;

    if (f>33 && this->State<95)                   //шанс на удачное сокращение потерянного на инциденты времени
    {this->lost_years-=rand()%5+1;


    }
    */
    if (stage !=0 && (stage<5))
    {this->changeResources(0);
    }
    if (stage >= 5 && stage <=20)
    {
        this->changeResources(-TheArk::get_instance()->getResources()->getComponents()/1000);
    }
    if (stage >= 20 && stage <=80)
    {
        this->changeResources(-TheArk::get_instance()->getResources()->getComponents()/100);
    }
    if (stage>100)
        this->stage=100;
    if (stage >= 80 && stage <=101)
    {
        this->changeResources(0);
    }
    this->lost_years-=2;
    if (this->lost_years < 0)
        this->lost_years==0;
   // if (this->staff==0) this->State=0;
    //else
   // this->State=(100- this->State=(100- (this->lost_years)/TheArk::get_instance()->getYearsTotal()*100);-(this->staff/this->need_staff));
if (TheArk::get_instance()->getPopulation()->getTotal()==0 || this->State<=0)
    this->State=0;
else
    this->State=100*(1-(this->staff/this->need_staff+lost_years/TheArk::get_instance()->getYearsTotal()));

}
double NavigationService::getState() {
    return this->State;


}

void NavigationService::setState(double s) {
    this->staff=round((s - 1) / 100 * need_staff);
   
    this->State = (s);


}

bool NavigationService:: changeResources(int delta)
{if (delta < 0)
    {
        TheArk::get_instance()->getResources()->setUsedToJunk( delta, Navigation_Service);
    }

    this->resources += delta;
    return true;

}


bool NavigationService::changeStaff(int delta)
{

        list<shared_ptr<Human>>& people = TheArk::get_instance()->getPopulation()->getAllClassification()[Navigation_Service];

        if (this->staff > delta && delta > 0)
        {
            int counter = 0;

            for (auto it = people.begin(); it != people.end(); it++)
            {
                while (counter < delta)
                {
                    (*it)->setIsAlive(false);
                    counter++;
                }

            }

            this->staff -= delta;
        }

        else
            if (delta <= 0)
            this->staff +=delta;

        return true;


}

unsigned int NavigationService::getStaffDemand()
{
    //return this->need_staff - this->staff;
    return (need_staff-staff);

}unsigned int NavigationService::getResourceDemand()
{
    //return this->need_resources - this->resources;
    return 0;

}



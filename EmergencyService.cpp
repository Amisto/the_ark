
// Created by Amisto on 4/2/2021.

#include "EmergencyService.h"

EmergencyService::EmergencyService(): state(0), staff(0), resources(0), max_resources(0), Junk(0) {
    max_staff = std::stoi(TheArk::get_instance()->getInterface()->getGeneral()["Population"]) / 10;
}

// From 1 to 100, low Emergency Service's state increases the value and
// the damage from accidents
double EmergencyService::damage_factor(Service* s)
{
    /*if (getState() + s->getState() != 0)
        return 100.f / (0.1 * getState() + 0.9 * s->getState());
    else
        return 100.f;*/
    return 4000.f / ((0.5 * getState() + 0.5 * s->getState()) + 30) - 30;
}

// Generation of accidents
// Low service's state increases the probability of accident
void EmergencyService::create_accident(Service* s)
{
    double k = TheArk::get_instance()->getRandomGenerator()->getRandomInt(20, 119)
                    * (1 - pow(3, -0.03 * s->getState()));
    if (k > 20 && k < 100)
        this->determine_severity(s);
    else
        return;
}


void EmergencyService::determine_severity(Service* s)
{
    //std::cout << TheArk::get_instance()->getCurrentYear() << " THROWN " << typeid(*s).name() << " DF " << damage_factor(s) << std::endl;
    if (damage_factor(s) > 0 && damage_factor(s) < 11)
        s->process_accident(NEGLIGIBLE);

    else if (this->damage_factor(s) > 10 && this->damage_factor(s) < 31)
        s->process_accident(LIGHT);

    else if (this->damage_factor(s) > 30 && this->damage_factor(s) < 51)
        s->process_accident(MEDIUM);

    else if (this->damage_factor(s) > 50 && this->damage_factor(s) < 71)
        s->process_accident(SEVERE);

    else if (this->damage_factor(s) > 70 && this->damage_factor(s) < 91)
        s->process_accident(DISASTROUS);

    else if (this->damage_factor(s) > 90 && this->damage_factor(s) < 101)
        s->process_accident(CATASTROPHIC);
}



void EmergencyService:: process_year()
{
    // Updating State depending on Resources, current state and accidents
    staff = TheArk::get_instance()->getPopulation()->getServiceStaff(Emergency_Service).size();
    //this->changeResources(this->getResourceDemand() - 10);
    //this->killStaff(3);
    //this->setState(100 * (double)((this->staff + this->resources) / 205.f));

    setState(staff * 100.f / max_staff);
    if(getState() > 100)
        setState(100);
    // Creating accidents

    for (auto s : TheArk::get_instance()->getServices())
    {
        this->create_accident(s);
    }

}

// Damaging this service depending on the severity
void EmergencyService::process_accident(AccidentSeverity as)
{
    this->changeResources(-(12 * as + 10));
    this->killStaff(4 * as + 10);
    this->setState(100 - (as * 5.1 + 10));
}

double EmergencyService::getState() {
    return state;
}

void EmergencyService::setState(double s) {
    state = s;
}

// Resources management
//-----------------------------
void EmergencyService::changeResources(int delta)
{
    if (delta < 0)//отняли ресурсы; из-за аварии 
    {
	this->Junk = delta;
        //TheArk::get_instance()->getResources()->setUsedToJunk(- delta, Emergency_Service);  //вернули мусор
    }
    else //добавляем ресурсы в количестве недостающих - 10 - ежегодный износ
    {
        //TheArk::get_instance()->getResources()->setComponentsToUsed(delta, Emergency_Service);
    }
    this->resources += delta;
}

unsigned int EmergencyService::getResourceDemand()
{
    return this->max_resources - this->resources;
    
}

unsigned int EmergencyService::returnJunk()
{
	return this->Junk;
}

//-------------------------------------

// Staff management

//-------------------------------------

void EmergencyService::killStaff(int delta)
{
    list<shared_ptr<Human>>& people = TheArk::get_instance()->getPopulation()->getAllClassification()[Emergency_Service];
    staff = people.size();
    if (this->staff > delta)
    {
        auto it = people.begin();
        for (auto i = 0; i < delta; i++)
        {
            (*it)->setIsAlive(false);
        }
        this->staff -= delta;
    }
    else {
        staff = 0;
    }
}

unsigned int EmergencyService::getStaffDemand()
{
    return this->max_staff - this->staff;
}

//----------------------------------------

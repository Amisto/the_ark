
// Created by Amisto on 4/2/2021.

#include "EmergencyService.h"

#include <iostream>

EmergencyService::EmergencyService()
{
    this->State = 100;
    this->Staff = 100; //персонала в данный момент
    this->max_Staff = 100;//максимальное количество персонала в службе
    this->Resources = 100;
    this->Junk = 0;
    this->max_Resources = 100;
}


//от 0 до 1, чем хуже состояние службы - тем выше вероятность аварии
double EmergencyService::accident_probability()
{
    double s = TheArk::get_instance()->getMedicalService()->getState() / 100;
    return 1 / pow(3, 3 * s);
}

//от 1 до 100 чем хуже состояние чс, тем больше коэффициент - больше ущерб от аварии
double EmergencyService::damage_factor()
{
    if (this->getState() != 0)
        return 100 / (this->getState());
    else return 100;
}

//генерация аварий
void EmergencyService::create_accident(Service* s)
{
    srand(time(0));
    int temp = (20 + rand() % 100);
    double k = temp * (1 - this->accident_probability());
    if (k > 20 && k < 100)
        this->determine_severity(s);
    else return;
}


void EmergencyService::determine_severity(Service* s)
{
    if (this->damage_factor() > 0 && this->damage_factor() < 11)
        s->process_accident(NEGLIGIBLE);

    else if (this->damage_factor() > 10 && this->damage_factor() < 31)
        s->process_accident(LIGHT);

    else if (this->damage_factor() > 30 && this->damage_factor() < 51)
        s->process_accident(MEDIUM);

    else if (this->damage_factor() > 50 && this->damage_factor() < 71)
        s->process_accident(SEVERE);

    else if (this->damage_factor() > 70 && this->damage_factor() < 91)
        s->process_accident(DISASTROUS);

    else if (this->damage_factor() > 90 && this->damage_factor() < 101)
        s->process_accident(CATASTROPHIC);
}



void EmergencyService:: process_year()
{
 // обновление состояния корабля в зависимости от кол-ва людей, ресурсов и аварий

    this->changeResources(this->getResourceDemand() - 10);
    this->changeStaff(3);
    this->setState(100 * (double)((this->Staff + this->Resources) / 205));

//генерация чс

    for (auto s : TheArk::get_instance()->getServices())
    {
        this->create_accident(s);
    }

}

//для обработки переданных аварий; меняет состояние корабля в зависимости от тяжести аварии
void EmergencyService::process_accident(AccidentSeverity as)
{
    this->changeResources(-(12 * as + 10));
    this->changeStaff(4 * as + 10);
    this->setState(100 - (as * 5.1 + 10));
}

double EmergencyService::getState()
{
    return this->State;
}

void EmergencyService::setState(double s)
{
    this->State = s;
}

// управление ресурсами
//----------------------------
bool EmergencyService::changeResources(int delta)
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
    this->Resources += delta;
    return true;
}

unsigned int EmergencyService::getResourceDemand()
{
    return this->max_Resources - this->Resources;
    
}

unsigned int EmergencyService::returnJunk()
{
	return this->Junk;
}

//-------------------------------------

//управление перосналом

//-------------------------------------


unsigned int EmergencyService:: getNStaff() 
{
    return TheArk::get_instance()->getPopulation()->getAllClassification()[Emergency_Service].size();
}

//добавление недостающего персонала реализовано в getRecommendedNStaff; здесь только убиваем в случае аварий + ежегодая убыль
bool EmergencyService::changeStaff(int delta)
{
    list<shared_ptr<Human>>& people = TheArk::get_instance()->getPopulation()->getAllClassification()[Emergency_Service];

    if (this->Staff > delta)
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

        this->Staff -= delta;
    }

    else
        this->Staff = 0;

    return true;
}

unsigned int EmergencyService::getStaffDemand()
{
    return this->max_Staff - this->Staff;

}

//----------------------------------------

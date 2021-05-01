
// Created by Amisto on 4/2/2021.

#include "EmergencyService.h"

EmergencyService::EmergencyService()
{
    this->State = 100;
    this->Staff = 100; //��������� � ������ ������
    this->max_Staff = 100;//������������ ���������� ��������� � ������
    this->Resources = 100;
    this->max_Resources = 100;

}


//�� 0 �� 1, ��� ���� ��������� ������ - ��� ���� ����������� ������
double EmergencyService::accident_propability()
{
    double s = TheArk::get_instance()->getMedicalService()->getState() / 100;
    return 1 / pow(3, 3 * s);
}

//�� 1 �� 100 ��� ���� ��������� ��, ��� ������ ����������� - ������ ����� �� ������
double EmergencyService::damage_factor()
{
    if (this->getState() != 0)
        return 100 / (this->getState());
    else return 100;
}

//��������� ������
void EmergencyService::create_accident(Service* s)
{
    srand(time(0));
    int temp = (20 + rand() % 100);
    double k = temp * (1 - this->accident_propability());
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
    // ���������� ��������� ������� � ����������� �� ���-�� �����, �������� � ������

    this->changeResources(this->getResourceDemand() - 10);
    this->changeStaff(5);
    this->setState(100 * ((this->Staff + this->Resources) / 205));

    //��������� ��
    for (auto s : TheArk::get_instance()->getServices())
    {
        this->create_accident(s);
    }
}

//��� ��������� ���������� ������; ������ ��������� ������� � ����������� �� ������� ������
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

// ���������� ���������
//----------------------------
bool EmergencyService::changeResources(int delta)
{
    if (delta < 0)//������ �������; ��-�� ������ 
    {
        TheArk::get_instance()->getResources()->setUsedToJunk(- delta, 5); //������� �����
    }
    else //��������� ������� � ���������� ����������� - 10 - ��������� �����
    {
        TheArk::get_instance()->getResources()->setComponentsToUsed(delta, 5);
    }
    this->Resources += delta;
    return true;
}

unsigned int EmergencyService::getResourceDemand()
{
    return this->max_Resources - this->Resources;
    
}

//-------------------------------------

//���������� ����������

//-------------------------------------

unsigned int EmergencyService::getNStaff()
{
    return TheArk::get_instance()->getPopulation()->getAllClassification()[Emergency_Service].size();
}

//���������� ������������ ��������� ����������� � getNStaff; ����� ������ ������� � ������ ������ + �������� �����
bool EmergencyService::changeStaff(int delta)
{
    this->Staff -= delta;
    list<shared_ptr<Human>>& people = TheArk::get_instance()->getPopulation()->getAllClassification()[Emergency_Service];
    
    int counter = 0;

    for (auto it = people.begin(); it != people.end(); it++)
    {  
        if (counter < delta)
        {
            (*it)->setIsAlive(false);
        }
        else
        {
            (*it)->setIsAlive(true);
        }
        counter++;
    }

    return true;
}

unsigned int EmergencyService::getStaffDemand()
{
    return this->max_Staff - this->Staff;
}

//----------------------------------------
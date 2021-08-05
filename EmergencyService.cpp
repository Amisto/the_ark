
// Created by Amisto on 4/2/2021.

#include "EmergencyService.h"

EmergencyService::EmergencyService(): state(0), staff(0), resources(0), max_resources(0), Junk(0) {
    max_staff = std::stoi(TheArk::get_instance()->getInterface()->getGeneral()["Population"])
            * std::stod(TheArk::get_instance()->getInterface()->getServices()
            [Emergency_Service]["Propotion_of_people"]);
    std::ifstream inp;
    inp.open("../EmergencyService_setup/output.txt");
    for(auto i = 0; i < 6; i++) {
        for(auto k = 0; k < 3; k++) {
            inp >> distribution_coefficients[i][k];
        }
    }
    inp.close();
    emergency_log.open("../Emergency_Log.txt");
    emergency_log << setw(CELL_WIDTH + 1) << "Year,"
    << setw(CELL_WIDTH_S + 1) << "Service,"
    << setw(CELL_WIDTH + 1) << "Severity," << endl;
}

// Generation of accidents
// Low service's state increases the probability of accident
void EmergencyService::create_accident(Service* s)
{
    double effective_state = EFFECTIVE_STATE_RATIO * this->state + (1.f - EFFECTIVE_STATE_RATIO) * s->getState(); // Emergency
    // usually helps to fix the accident, so effective state is combined

    std::array <double, 7> probabilities {0}; // For each severity and one for their sum
    for (auto i = 0; i < 6; i++) {
        probabilities[i] = distribution_coefficients[i][0] *
                std::exp( (-1.f) * distribution_coefficients[i][1] * pow(effective_state, 2)) +
                distribution_coefficients[i][2]; // y = a * exp(-b * x^2) + c
        probabilities[6] += probabilities[i];
    }

    auto rand_number = TheArk::get_instance()->getRandomGenerator()->getRandomDouble(0, 1);

    if (rand_number < probabilities[6]) {
        unsigned short resulting_damage = 0;

        while (rand_number > std::accumulate(probabilities.begin(), probabilities.begin() + resulting_damage, 0.l))
            resulting_damage++;
        resulting_damage--;

        s->process_accident(static_cast<AccidentSeverity>(resulting_damage));
        string service_name = typeid(*s).name();
        service_name.erase(0, 2);

        emergency_log << setw(CELL_WIDTH) << TheArk::get_instance()->getCurrentYear() << ","
        << setw(CELL_WIDTH_S) << service_name << "," << setw(CELL_WIDTH)
        << static_cast<AccidentSeverity>(resulting_damage) << "," << endl;
    }
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
    //changeResources(-(12 * as + 10));
    killStaff((0.0243 * pow((as + 1), 2) + 0.0257) * max_staff);
    //setState(100 - (as * 5.1 + 10));
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

EmergencyService::~EmergencyService() {
    emergency_log.close();
}

//----------------------------------------

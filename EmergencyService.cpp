//
// Created by Amisto on 4/2/2021.
//

#include "EmergencyService.h"
#include "Resources.h"
#include "RandomNumberGenerator.h"

EmergencyService::EmergencyService():
system_state(0), staff(0), need_resources(1), junk(0),
resources_state(0), staff_state(0), tools_state(0)
{
    required_staff = std::stoi(TheArk::get_instance()->getInterface()->getGeneral()["Population"])
                     * std::stod(TheArk::get_instance()->getInterface()->getServices()
            [Emergency_Service]["Propotion_of_people"]);

    if (!required_staff)
        std::cerr << "ERROR: EmergencyService's staff is empty!" << endl;

    std::ifstream inp;
    inp.open("../EmergencyService_setup/coefficients.txt");
    for(auto i = 0; i < 6; i++) {
        for(auto k = 0; k < 3; k++) {
            inp >> distribution_coefficients[i][k];
        }
    }
    inp.close();

    accPropertiesInit();

    emergency_log.open("../Logs/Emergency_Log.txt");
    emergency_log << setw(CELL_WIDTH + 1) << "Year,"
    << setw(CELL_WIDTH_S + 1) << "Service,"
    << setw(CELL_WIDTH + 1) << "Severity," << setw(CELL_WIDTH + 1) << "Id," << endl;
}

void EmergencyService::accPropertiesInit()
{
    std::ifstream inp;
    inp.open("../EmergencyService_setup/e_ids.txt");
    std::string order_check_services[6] = {"T", "B", "M", "N", "E", "S"};
    std::string order_check_severities[6] = {"N", "L", "M", "S", "D", "C"};
    std::string line = " ";
    char service = -1;
    char severity = -1;
    while (line != "!@#")
    {
        inp >> line;
        if (line == "///") {
            service++;
            severity = -1;
            inp >> line;
            if (line != order_check_services[service]) {
                std::cerr << "EMERGENCY ERROR: Wrong order of severities in e_ids.txt" << endl
                          << "Received line: " << line << endl << "Expected line: " << order_check_services[service] << endl;
            }
            else { inp >> line; }
        }
        if (line == "//") {
            severity++;
            inp >> line;
            if (line != order_check_severities[severity]) {
                std::cerr << "EMERGENCY ERROR: Wrong order of severities in e_ids.txt" << endl
                << "Received line: " << line << endl << "Expected line: " << order_check_severities[severity] << endl
                << "Service: " << order_check_services[severity] << endl;
            }
            else { inp >> line; }
        }
        if (line == "#") {
            unsigned id;
            inp >> id;
            list<pair<char, char>> temp_list;
            inp >> line;
            while (line != "##") {
                auto first = std::stoi(line) / 10;
                auto second = std::stoi(line) % 10;
                clog << first <<"---" << second << endl;
                temp_list.emplace_back(std::make_pair(first, second));
                inp >> line;
            }
            acc_properties[service][severity].emplace(std::make_pair(id, temp_list));
        }
    }
    inp.close();
}

// Generation of accidents
// Low service's state increases the probability of accident
void EmergencyService::create_accident(Service* s)
{
    double effective_state = EFFECTIVE_STATE_RATIO * this->system_state + (1.0 - EFFECTIVE_STATE_RATIO) * s->getState(); // Emergency
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
        static array<char, 6> services_order = {'T', 'B', 'M', 'N', 'E', 'S'};
        unsigned short resulting_damage = 0;

        while (rand_number > std::accumulate(probabilities.begin(), probabilities.begin() + resulting_damage, 0.l))
            resulting_damage++;
        resulting_damage--;

        s->process_accident(static_cast<AccidentSeverity>(resulting_damage));
        string service_name = typeid(*s).name();
        service_name.erase(0, 2);

        auto service_number = std::find(services_order.begin(), services_order.end(), service_name[0])
                - services_order.begin();
        auto current_map = acc_properties[service_number][resulting_damage];
        auto current_id = TheArk::get_instance()->getRandomGenerator()->getRandomInt(1, current_map.size());

        if (!(current_map[current_id].empty() or CHAIN_REACTION_FLAG)) {
            for (auto & it : current_map[current_id])
                TheArk::get_instance()->getServices()[it.first]->process_accident(static_cast<AccidentSeverity>(it.second));
            CHAIN_REACTION_FLAG = true;
        }

        emergency_log << setw(CELL_WIDTH) << TheArk::get_instance()->getCurrentYear() << ","
        << setw(CELL_WIDTH_S) << service_name << "," << setw(CELL_WIDTH)
        << resulting_damage << "," << setw(CELL_WIDTH) << current_id << "," << endl;
    }
}


void EmergencyService::process_year()
{
    CHAIN_REACTION_FLAG = false;

    staff = TheArk::get_instance()->getPopulation()->getServiceStaff(Emergency_Service).size();
    auto used_currently = TheArk::get_instance()->getResources()->getUsedByService(Emergency_Service);

    // Creating accidents
    for (auto s : TheArk::get_instance()->getServices())
    {
        this->create_accident(s);
    }

    tools_state += ANNUAL_DEGRADATION;

    // Updating states
    staff_state = staff * 100.0 / required_staff;
    if (staff_state > 100.0)
        staff_state = 100.0;

    resources_state = used_currently * 100.0 / need_resources;

    tools_state += (staff_state * 0.7 + resources_state * 0.3) * REPAIR_PERCENT_PER_YEAR / 100.0;
    if (tools_state < 0)
        tools_state = 0;
    else if (tools_state > 100)
        tools_state = 100;

    system_state = (resources_state + tools_state + staff_state) / 3.0;

    // Resource management
    junk = used_currently;
    need_resources = staff;
    if (!need_resources)
        need_resources = 1;
}

// Damaging this service depending on the severity
void EmergencyService::process_accident(AccidentSeverity as) {
    killStaff((0.0243 * pow((as + 1), 2) + 0.0257) * required_staff);
    tools_state -= pow((as + 1), 2.3);
}

double EmergencyService::getState() {
    return system_state;
}

void EmergencyService::setState(double s) {
    system_state = s;
    resources_state = s;
    staff_state = s;
    tools_state = s;
}

unsigned int EmergencyService::getResourceDemand() {
    return need_resources;
}

unsigned int EmergencyService::returnJunk() {
	return this->junk;
}

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

unsigned int EmergencyService::getStaffDemand() {
    return required_staff - staff;
}

EmergencyService::~EmergencyService() {
    emergency_log.close();
}


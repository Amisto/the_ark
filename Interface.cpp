//
// Created by Amisto on 7/27/2021.
//

#include "Interface.h"
#include "TheArk.h"
#include "Population.h"
#include "Resources.h"
#include "Service.h"
#include "Enums.cpp"
#include <iostream>
#include <iomanip>

using std::cin;
using std::cerr;
using std::endl;
using std::setw;

Interface::Interface() {

}

Interface::~Interface() {
    os.close();
}

void Interface::init() {
    string ifname;
    cin >> ifname;
    ifname = "../" + ifname + ".cfg";
    std::ifstream fi(ifname, std::ios::in);

    if (!fi.is_open()) {
        cerr << "No config file with name " << ifname << " exists." << endl;
        return;
    }

    string s0, s1;

    fi >> s0;
    if (s0 != "General") {
        cerr << "No General block in config file." << endl;
        return;
    }

    fi >> s0;
    while (s0 != "Services")
    {
        fi >> s1;
        general[s0] = s1;
        if (fi.eof())
        {
            cerr << "No Services block in config file." << endl;
            return;
        }
        fi >> s0;
    }

    fi >> s0;
    if (s0 != "Technical")
    {
        cerr << "No Technical service block in config file." << endl;
        return;
    }
    fi >> s0;
    while (s0 != "Biological")
    {
        fi >> s1;
        services[Services::Technical_Service][s0] = s1;
        if (fi.eof())
        {
            cerr << "No Biological block in config file." << endl;
            return;
        }
        fi >> s0;
    }
    fi >> s0;
    while (s0 != "Medical")
    {
        fi >> s1;
        services[Services::Biological_Service][s0] = s1;
        if (fi.eof())
        {
            cerr << "No Medical block in config file." << endl;
            return;
        }
        fi >> s0;
    }
    fi >> s0;
    while (s0 != "Navigation")
    {
        fi >> s1;
        services[Services::Medical_Service][s0] = s1;
        if (fi.eof())
        {
            cerr << "No Navigation block in config file." << endl;
            return;
        }
        fi >> s0;
    }
    fi >> s0;
    while (s0 != "Emergency")
    {
        fi >> s1;
        services[Services::Navigation_Service][s0] = s1;
        if (fi.eof())
        {
            cerr << "No Emergency block in config file." << endl;
            return;
        }
        fi >> s0;
    }
    fi >> s0;
    while (s0 != "Social")
    {
        fi >> s1;
        services[Services::Emergency_Service][s0] = s1;
        if (fi.eof())
        {
            cerr << "No Social block in config file." << endl;
            return;
        }
        fi >> s0;
    }
    while (!fi.eof())
    {
        fi >> s0 >> s1;
        services[Services::Social_Service][s0] = s1;
    }

    os.open("../" + general["Output"] + ".csv");
    print_table_header(os);
}

void Interface::snap(unsigned int current) {

    auto &os_snap = os;
    auto population = TheArk::get_instance()->getPopulation();
    auto resources = TheArk::get_instance()->getResources();
    auto services = TheArk::get_instance()->getServices();

    os_snap << std::setw(CELL_WIDTH) << current << ",";

    os_snap << std::setw(CELL_WIDTH) << population->getTotal() << ","
            << std::setw(CELL_WIDTH) << population->getChildren() << ","
            << std::setw(CELL_WIDTH) << population->getAdults() << ","
            << std::setw(CELL_WIDTH) << population->getOldmen() << ",";

    os_snap << std::setw(CELL_WIDTH) << resources->getConsumables() << ","
            << std::setw(CELL_WIDTH) << resources->getComponents() << ","
            << std::setw(CELL_WIDTH) << resources->getUsed() << ","
            << std::setw(CELL_WIDTH) << resources->getJunk() << ","
            << std::setw(CELL_WIDTH) << resources->getRefuse() << ",";

    for (auto s: services)
        os_snap << std::setw(CELL_WIDTH) << s->getState() << ",";
    os_snap << std::endl;
}

void Interface::print_table_header(std::ostream & fout)
{
    std::array<std::string, 6> services_names = {"Technical", "Biological",
                                                 "Medical", "Navigation",
                                                 "Emergency", "Social"};
    fout << setw(CELL_WIDTH) << "Year" << ","
         << setw(CELL_WIDTH) << "Total" << ","
         << setw(CELL_WIDTH) << "Child" << ","
         << setw(CELL_WIDTH) << "Adults" << ","
         << setw(CELL_WIDTH) << "Old" << ","
         << setw(CELL_WIDTH) << "Consum" << ","
         << setw(CELL_WIDTH) << "Compon" << ","
         << setw(CELL_WIDTH) << "Used" << ","
         << setw(CELL_WIDTH) << "Junk" << ","
         << setw(CELL_WIDTH) << "Ref" << ",";

    for (int i = 0; i < 6; ++i)
        fout << setw(CELL_WIDTH) << services_names[i] << ",";

    fout << endl;
}

map<string, string> &Interface::getGeneral() {
    return general;
}

array<map<string, string>, 6> &Interface::getServices() {
    return services;
}

void Interface::finalSnap() {
    auto ta = TheArk::get_instance();
    std::basic_ofstream<char> ocfg("../" + general["Output"] + "_out.cfg");
    ocfg << "General" << endl << endl;
    general["Output"] += "_out";
    for (auto param : general)
        ocfg << "    " << param.first << " " << param.second << endl;
    ocfg << endl << endl;
    ocfg << "Services" << endl << endl;
    ocfg << "    Technical" << endl;
    for (auto param : services[Services::Technical_Service])
        ocfg << "        " << param.first << " " << param.second << endl;
    ocfg << endl;
    ocfg << "    Biological" << endl;
    for (auto param : services[Services::Biological_Service])
        ocfg << "        " << param.first << " " << param.second << endl;
    ocfg << endl;
    ocfg << "    Medical" << endl;
    for (auto param : services[Services::Medical_Service])
        ocfg << "        " << param.first << " " << param.second << endl;
    ocfg << endl;
    ocfg << "    Navigation" << endl;
    for (auto param : services[Services::Navigation_Service])
        ocfg << "        " << param.first << " " << param.second << endl;
    ocfg << endl;
    ocfg << "    Emergency" << endl;
    for (auto param : services[Services::Emergency_Service])
        ocfg << "        " << param.first << " " << param.second << endl;
    ocfg << endl;
    ocfg << "    Social" << endl;
    for (auto param : services[Services::Social_Service])
        ocfg << "        " << param.first << " " << param.second << endl;
    ocfg << endl;
    ocfg.close();
}

//
// Created by Amisto on 7/27/2021.
//

#ifndef THE_ARK_INTERFACE_H
#define THE_ARK_INTERFACE_H

#include <map>
#include <array>
#include <string>
#include <fstream>

using std::map;
using std::array;
using std::string;

class Interface {
private:
    std::basic_ofstream<char> os;

    void print_table_header(std::ostream & fout);

    map<string, string> general;
    array<map<string, string>, 6> services;
public:
    Interface();
    void init();
    void snap(unsigned int current_year);
    void finalSnap();

    virtual ~Interface();
    map<string, string>& getGeneral();
    array<map<string, string>, 6>& getServices();

};


#endif //THE_ARK_INTERFACE_H

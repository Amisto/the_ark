#include <iostream>
#include <iomanip>
#include <fstream>
#include "TheArk.h"

using std::cin;
using std::cout;
using std::endl;
using std::setw;

void print_table_header(std::ostream & fout);

int main() {

    TheArk & ark = *TheArk::get_instance();

    std::ofstream fout("../TheArk.csv");

    ark.init(&cin, &fout);
    print_table_header(fout);

    ark.flight();

    ark.deleteArk();
    return 0;
}

void print_table_header(std::ostream & fout)
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
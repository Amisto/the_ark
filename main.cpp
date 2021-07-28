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

    ark.init();

    ark.flight();

    ark.deleteArk();

    return 0;
}


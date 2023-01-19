#include <iostream>
#include "table.h"

int main(int argc, char* argv[]) {
    if (argc < 2)
    {
        std::cout << "Usage: YadroCSV.exe filename.csv" << std::endl;
        return 1;
    }
    const std::string fname = argv[1];

//    const std::string fname = "test_wrong_row.csv";

    Table table;

    try
    {
        table = Table(fname);
    }
    catch (const std::invalid_argument& e)
    {
        std::cout << e.what() << std::endl;
        return 1;
    }

    // table.print();

    // std::cout << std::endl;

    table.calculate();

    table.print();


    return 0;
}

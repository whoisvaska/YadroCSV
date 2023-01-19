//
// Created by vaska on 16-Jan-23.
//

#ifndef YADROCSV_TABLE_H
#define YADROCSV_TABLE_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <stack>
#include <algorithm>
#include "string_helper.h"

class Table {
public:
    explicit Table();
    explicit Table(const std::string& fname);

    std::string getValue(const std::string& col, const std::string& row);
    int setValue(const std::string& col, const std::string& row, std::string value);

    int print();
    int calculate();

    void operator=(const Table& other)
    {
        data = other.data;
        columnsCount = other.columnsCount;
        rowsCount = other.rowsCount;
    }

private:
    std::map<std::string, std::vector<std::string>> data;
    std::map<std::string, std::vector<std::string>> sortedData;

    int columnsCount = 0;
    int rowsCount = 0;

    std::vector<std::string> unresolvedExpressions;


    int
    splitExpression(std::string expression, std::string &col1, std::string &row1, std::string &col2, std::string &row2,
                    std::string &op);

    int calculateExpression(const std::string& expression, int result);
};


#endif //YADROCSV_TABLE_H

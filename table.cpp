#include "table.h"

Table::Table() {}

Table::Table(const std::string& fname)
{
    std::string line;
    std::vector<std::string> values;
    std::vector<std::string> columns;

    std::ifstream fin;

    fin.open(fname);

    if(!fin)
    {
        throw std::invalid_argument("Error opening file: " + fname);
    }

    //for header
    std::getline(fin, line);
    values = splitString(line, ',');
    for (const auto& col : values)
    {
        if (col.length() > 0)
        {
            columns.push_back(col);
            this->data[col] = std::vector<std::string>({});
            this->columnsCount += 1;
        }
    }

    values.clear();

    int row = 1;
    while (std::getline(fin, line))
    {
        int column = 1;

        values = splitString(line, ',');

        if ((values.size() - 1 < this->columnsCount) || (values.empty()))
        {
            throw std::invalid_argument("Incorrect number of columns in row: " + line);
        }


        try
        {
            if (std::stoi(values.front()) != row)
            {
                throw std::invalid_argument("Wrong row number in line: " + line );
            }
        }
        catch (std::exception& e)
        {
            std::cout << e.what() + line << std::endl;
        }

        for (const auto& col : columns)
        {
            this->data[col].push_back(values[column++]);
        }
        this->rowsCount += 1;
        row += 1;
    }
    fin.close();
}


int Table::print()
{
    //header
    for (const auto& v : this->data)
    {
        std::cout << "," << v.first;
    }
    std::cout << std::endl;

    for (int row = 0; row < this->rowsCount; row++)
    {
        std::cout << row + 1;
        for (const auto& v : this->data)
        {
            std::cout << ',' << v.second[row];
        }
        std::cout << std::endl;
    }

    return 0;
}


std::string Table::getValue(const std::string& col, const std::string& row)
{
    if ((col.length() < 1) || (row.length() < 1))
    {
        throw std::invalid_argument("Column or row index cannot be zero: " + col + row);
    }

    if ((std::stoi(row) - 1 < 0) || (std::stoi(row) > this->rowsCount))
    {
        throw std::invalid_argument("Row number must be in range (0," + std::to_string(this->rowsCount) + "): " + row);
    }
    if (this->data.find(col) == this->data.end())
    {
        throw std::invalid_argument("No such column exist: " + col);
    }

    return (this->data[col][std::stoi(row) - 1]);
}


int Table::setValue(const std::string& col, const std::string& row, std::string value)
{
    if ((col.length() < 1) || (row.length() < 1))
    {
        throw std::invalid_argument("Column or row index cannot be zero: " + col + row);
    }

    if ((std::stoi(row) - 1 < 0) || (std::stoi(row) > this->rowsCount))
    {
        throw std::invalid_argument("Row number must be in range (0," + std::to_string(this->rowsCount) + "): " + row);
    }

    if (this->data.find(col) == this->data.end())
    {
        throw std::invalid_argument("No such column exist: " + col);
    }
    this->data[col][std::stoi(row) - 1] = value;

    return 0;
}


int Table::splitExpression(std::string expression, std::string& col1, std::string& row1,
                           std::string& col2, std::string& row2, std::string& op)
{
    int charNumber = 0;

    if (expression[charNumber] == '=')
    {
        charNumber += 1;
    }

    while(isalpha(expression[charNumber]) && (charNumber <= expression.length()))
    {
        col1.push_back(expression[charNumber]);
        charNumber += 1;
    }
    while(isalnum(expression[charNumber]) && (charNumber <= expression.length()))
    {
        row1.push_back(expression[charNumber]);
        charNumber += 1;
    }


    if (    (expression[charNumber] != '/') &&
            (expression[charNumber] != '*') &&
            (expression[charNumber] != '+') &&
            (expression[charNumber] != '-')
            )
    {
        op = "";
        return 0;
    }

    op = expression[charNumber];
    charNumber += 1;

    while(isalpha(expression[charNumber]) && (charNumber <= expression.length()))
    {
        col2.push_back(expression[charNumber]);
        charNumber += 1;
    }
    while(isalnum(expression[charNumber]) && (charNumber <= expression.length()))
    {
        row2.push_back(expression[charNumber]);
        charNumber += 1;
    }

    std::cout << col1 << ',' << row1 << ',' << col2 << ',' << row2  << ',' << op << std::endl;
    return 0;
}


int Table::calculateExpression(const std::string& expression, int result) {
    int value1, value2 = 0;
    std::string col1, col2, row1, row2, op;

    splitExpression(expression, col1, row1, col2, row2, op);

    if (col1.empty())
    {
        if (!row1.empty())
        {
            try
            {
                value1 = std::stoi(row1);
            }
            catch (std::invalid_argument& e)
            {
                throw std::invalid_argument("Cannot convert value to int: " + row1);
                value1 = 0;
            }
        }
        else
        {

        }
    }
    else
    {
        if (std::find(unresolvedExpressions.begin(), unresolvedExpressions.end(), col1 + row1) != unresolvedExpressions.end())
        {
            throw std::invalid_argument("Cannot calculate cell: " + col1 + row1);
        }
        unresolvedExpressions.push_back(col1 + row1);
        if (getValue(col1, row1)[0] == '=')
        {
            value1 = calculateExpression(getValue(col1, row1), result);
        }
        else
        {
            try
            {
                value1 = std::stoi(getValue(col1, row1));
            }
            catch(std::invalid_argument& e)
            {
                std::cout << "Cannot convert value to int: " + getValue(col1, row1) << std::endl;
                value1 = 0;
            }
        }
    }

    if (col2.empty())
    {
        if (!row2.empty())
        {
            try
            {
                value2 = std::stoi(row2);
            }
            catch (std::invalid_argument& e)
            {
                throw std::invalid_argument("Cannot convert value to int: " + row2);
                value2 = 0;
            }
        }
        else
        {

        }
    }
    else
    {
        if (std::find(unresolvedExpressions.begin(), unresolvedExpressions.end(), col2 + row2) != unresolvedExpressions.end())
        {
            throw std::invalid_argument("Cannot calculate cell: " + col2 + row2);
        }
        unresolvedExpressions.push_back(col2 + row2);

        if (getValue(col2, row2)[0] == '=' )
        {
            value2 = calculateExpression(getValue(col2, row2), result);
        }
        else
        {
            try
            {
                value2 = std::stoi(getValue(col2, row2));
            }
            catch(std::invalid_argument& e)
            {
                std::cout << "Cannot convert value to int: " + getValue(col2, row2) << std::endl;
                value2 = 0;
            }
        }
    }

    if (op == "+")
    {
        result = result + value1 + value2;
    }
    else if (op == "-")
    {
        result = result + value1 - value2;
    }
    else if (op == "*")
    {
        result = result + value1 * value2;
    }
    else if (op == "/")
    {

        if (value2 == 0)
        {
            throw std::invalid_argument("Division by zero: " + expression);
        }

        result = result + value1 / value2;
    }
    else if (op.length() < 1)
    {
        result = value1 + value2;
    }

    return result;
}


int Table::calculate()
{
    for (int rowNumber = 0; rowNumber < this->rowsCount; rowNumber++)
    {
        for (const auto& v : this->data)
        {
            int charNumber = 0;
            std::string expression = getValue(v.first, std::to_string(rowNumber + 1));
            if (expression[0] == '=')
            {
                int value = 0;
                try
                {
                    value = calculateExpression(expression, 0);
                    unresolvedExpressions.clear();
                }
                catch (std::invalid_argument& e)
                {
                    std::cout << e.what() << std::endl;
                    return -1;
                }
                try
                {
                    setValue(v.first, std::to_string(rowNumber + 1), std::to_string(value));
                }
                catch (std::invalid_argument e)
                {
                    std::cout << e.what() << std::endl;
                    return -1;
                }
            }
        }
    }
    return 0;
}


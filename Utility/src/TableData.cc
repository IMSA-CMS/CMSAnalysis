#include <vector>
#include <algorithm>
#include <string>
#include "CMSAnalysis/Utility/interface/TableData.hh"
#include <iostream>


TableData::TableData(std::vector<std::vector<std::string>> outputs, std::vector<std::string> columnNames, std::vector<std::string> rowNames)
{
    entries = outputs;
    columns = columnNames;
    rows = rowNames;
}


void TableData::addRow(std::string massTarget)
{
    rows.push_back(massTarget);
    std::vector<std::string> emptyRow(columns.size(), "");
    entries.push_back(emptyRow);
}

void TableData::addColumn(std::string process)
{
    columns.push_back(process);
    int count = 0;
    while(static_cast<std::vector<int>::size_type>(count) < entries.size())
    {
        entries.at(count).push_back("");
        count++;
    }
}

void TableData::addEntry(std::vector<std::string> entry)
{
    int columnIndex = std::find(columns.begin(), columns.end(), entry.at(0)) - columns.begin();
    int rowIndex = std::find(rows.begin(), rows.end(), entry.at(1)) - rows.begin(); 
    addEntry(rowIndex, columnIndex, entry.at(2));
}

//Overriding (first addEntry calls this one)
void TableData::addEntry(int rowIndex, int colIndex, std::string entry) 
{
    entries.at(rowIndex).at(colIndex) = entry;
    
}
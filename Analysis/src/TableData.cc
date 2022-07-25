#include <vector>
#include <algorithm>
#include <string>
#include "CMSAnalysis/Analysis/interface/TableData.hh"
#include <iostream>


TableData::TableData(std::vector<std::vector<std::string>> outputs)
{
    for(std::vector<std::string> output : outputs) {
        if(std::find(rows.begin(), rows.end(), output.at(1)) == rows.end()) {
            addRow(output.at(1));
        }
        if(std::find(columns.begin(), columns.end(), output.at(0)) == columns.end()) {
            addColumn(output.at(0));
        }
    }
    /*for(std::vector<std::string> row : entries) {
        for(std::string entry : row) {
            std::cout << "entry" << std::endl;
        }
    }*/
    for(std::vector<std::string> output : outputs) {
        addEntry(output);
    }
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

void TableData::addEntry(int rowIndex, int colIndex, std::string entry) 
{
    entries.at(rowIndex).at(colIndex) = entry;
}
#include "CMSAnalysis/Analysis/interface/TableData.hh"
#include "CMSAnalysis/Analysis/interface/TableWriter.hh"
#include "CMSAnalysis/Analysis/interface/PowerpointTable.hh"
#include <iostream>
#include <vector>
#include <string>
#include <memory>

void PowerpointTable::makeTable(std::shared_ptr<TableData> data, std::ostream& outputStream)
{
    std::vector<std::string> rows = data->getRows();
    std::vector<std::string> cols = data->getColumns();
    std::vector<std::vector<std::string>> entries = data->getEntries();
    std::string table = "";
    outputStream << makeHeaderRow(cols);
    int count = 0;
    while(static_cast<std::vector<int>::size_type>(count) < rows.size()) {
        table = table + makeRow(rows.at(count), entries.at(count));
        count++;
    }
    outputStream << table;
}


std::string PowerpointTable::makeHeaderRow(std::vector<std::string> data)
{
    std::string row = "";
    for(std::string entry : data) 
    {
        row = row + "," + entry;
    }
    row = row + "\n";
    return row;
}

std::string PowerpointTable::makeRow(std::string rowName, std::vector<std::string> data)
{
    std::string row = rowName;
    for(std::string entry : data) 
    {
        row = row + "," + entry;
    }
    row = row + "\n";
    return row;
}
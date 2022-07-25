#include "CMSAnalysis/Analysis/interface/TableData.hh"
#include "CMSAnalysis/Analysis/interface/TableWriter.hh"
#include "CMSAnalysis/Analysis/interface/HTMLTable.hh"
#include <iostream>
#include <vector>
#include <string>
#include <memory>

void HTMLTable::makeTable(std::shared_ptr<TableData> data, std::ostream& outputStream)
{
    std::vector<std::string> rows = data->getRows();
    std::vector<std::string> cols = data->getColumns();
    std::vector<std::vector<std::string>> entries = data->getEntries();
    std::string table = "";
    outputStream << "<style>" << std::endl;
    outputStream << "table, th, td {\n\tborder:1px solid black;\n}" << std::endl;
    outputStream << "</style>" << std::endl;
    outputStream << "<table style=\"width:100%\">" << std::endl;
    table = table + makeHeaderRow(cols);
    int count = 0;
    while(static_cast<std::vector<int>::size_type>(count) < rows.size()) {
        table = table + makeRow(rows.at(count), entries.at(count));
        count++;
    }
    outputStream << table;
    outputStream << "</table>" << std::endl;
}


std::string HTMLTable::makeHeaderRow(std::vector<std::string> data)
{
    std::string row = "<tr>\n";
    row = row + "\t<th></th>\n";
    for(std::string entry : data) 
    {
        row = row + "\t<th>" + entry + "</th>\n" ;
    }
    row = row + "</tr>\n";
    return row;
}

std::string HTMLTable::makeRow(std::string rowName, std::vector<std::string> data)
{
    std::string row = "<tr>\n";
    row = row + "\t<td>" + rowName + "</td>\n";
    for(std::string entry : data) 
    {
        row = row + "\t<td>" + entry + "</td>\n" ;
    }
    row = row + "</tr>\n";
    return row;
}
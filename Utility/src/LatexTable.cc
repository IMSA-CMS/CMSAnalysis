#include "CMSAnalysis/Utility/interface/TableData.hh"
#include "CMSAnalysis/Utility/interface/TableWriter.hh"
#include "CMSAnalysis/Utility/interface/LatexTable.hh"
#include <iostream>
#include <vector>
#include <string>
#include <memory>

void LatexTable::makeTable(std::shared_ptr<TableData> data, std::ostream& outputStream)
{
    std::vector<std::string> rows = data->getRows();
    std::vector<std::string> cols = data->getColumns();
    std::vector<std::vector<std::string>> entries = data->getEntries();
    std::string table = "";
    outputStream << "\\begin{center}" << std::endl;
    outputStream << "\\begin{tabular}";
    std::string toAdd = "{|c";
    int count = 0;
    while(count < static_cast<int>(cols.size())) {
        toAdd = toAdd + " c";
        count++;
    }
    outputStream << toAdd << "|}" << std::endl;
    outputStream << "\\hline" << std::endl;
    outputStream << makeHeaderRow(cols);
    outputStream << "\\hline" << std::endl;
    count = 0;
    while(static_cast<std::vector<int>::size_type>(count) < rows.size()) {
        table = table + makeRow(rows.at(count), entries.at(count));
        count++;
    }
    outputStream << table;
    outputStream << "\\hline" << std::endl;
    outputStream << "\\end{tabular}" << std::endl;
    outputStream << "\\end{center}" << std::endl;
}


std::string LatexTable::makeHeaderRow(std::vector<std::string> data)
{
    std::string row = "";
    for(std::string entry : data) 
    {
        row = row + " & " + entry;
    }
    row = row + "\\\\\n";
    return row;
}

std::string LatexTable::makeRow(std::string rowName, std::vector<std::string> data)
{
    std::string row = rowName;
    for(std::string entry : data) 
    {
        row = row + " & " + entry;
    }
    row = row + "\\\\\n";
    return row;
}
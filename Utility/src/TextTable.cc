#include "CMSAnalysis/Utility/interface/TableData.hh"
#include "CMSAnalysis/Utility/interface/TableWriter.hh"
#include "CMSAnalysis/Utility/interface/TextTable.hh"
#include <iostream>
#include <vector>
#include <string>
#include <memory>

void TextTable::makeTable(std::shared_ptr<TableData> data, std::ostream& outputStream)
{
    std::vector<std::string> rows = data->getRows();
    std::vector<std::string> cols = data->getColumns();
    std::vector<std::vector<std::string>> entries = data->getEntries();
    std::string table = "";
    int colWidth = 0;
    for(std::string rowName : rows)
    {
        if((int) rowName.length() > colWidth) 
        {
            colWidth = (int) rowName.length();
        }
    }
    for(std::string colName : cols) 
    {
        if((int) colName.length() > colWidth) 
        {
            colWidth = (int) colName.length();
        }
    }
    for(std::vector<std::string> row : entries) 
    {
        for(std::string entry : row) 
        {
            if((int) entry.length() > colWidth) 
            {
                colWidth = (int) entry.length();
            }
        }
    }
    table += makeDivider(colWidth, static_cast<int>(cols.size()));
    table += makeRow("", cols, colWidth);
    table += makeDivider(colWidth, static_cast<int>(cols.size()));
    int count = 0;
    while(static_cast<std::vector<int>::size_type>(count) < rows.size()) 
    {
        table += makeRow(rows.at(count), entries.at(count), colWidth);
        count++;
    }
    table += makeDivider(colWidth, static_cast<int>(cols.size()));
    outputStream << table;
}

std::string TextTable::makeSpace(int length)
{
    std::string spacer(length, ' ');
    return spacer;
}

std::string TextTable::makeRow(std::string rowName, std::vector<std::string> data, int colSize)
{
    std::string spacer;
    std::string row = "| ";
    spacer = makeSpace(colSize - (int) rowName.length());
    row = row + rowName + spacer + " | ";
    for(std::string entry : data) 
    {
        spacer = makeSpace(colSize - (int) entry.length());
        row = row + entry + spacer + " | ";
    }
    row = row + "\n";
    return row;
}

std::string TextTable::makeDivider(int colSize, int numCols) {
    std::string singleColDivider(colSize, '-');
    std::string divider = "|-";
    singleColDivider = divider + singleColDivider + "-";
    std::string toOutput = "";
    int count = 0;
    while(count < numCols + 1) {
        toOutput = toOutput + singleColDivider;
        count++;
    }
    toOutput = toOutput + "|\n";
    return toOutput;
}
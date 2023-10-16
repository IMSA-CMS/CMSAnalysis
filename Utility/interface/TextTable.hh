#ifndef TEXTTABLE_HH
#define TEXTTABLE_HH

#include "TableWriter.hh"
#include "TableData.hh"
#include <memory>
#include <iostream>
#include <string>


class TextTable : public TableWriter
{
    public:
        TextTable() {}
        void makeTable(std::shared_ptr<TableData> data, std::ostream& outputStream) override;
        std::string makeSpace(int length);
        std::string makeRow(std::string rowName, std::vector<std::string> data, int colSize);
        std::string makeDivider(int colSize, int numCols);
};


#endif
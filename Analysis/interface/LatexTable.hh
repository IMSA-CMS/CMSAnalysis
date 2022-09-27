#ifndef LATEXTABLE_HH
#define LATEXTABLE_HH

#include "TableWriter.hh"
#include "TableData.hh"
#include <memory>
#include <iostream>
#include <string>

class LatexTable : public TableWriter
{
    public:
        LatexTable() {}
        void makeTable(std::shared_ptr<TableData> data, std::ostream& outputStream) override;
        std::string makeHeaderRow(std::vector<std::string> data);
        std::string makeRow(std::string rowName, std::vector<std::string> data);
};

#endif
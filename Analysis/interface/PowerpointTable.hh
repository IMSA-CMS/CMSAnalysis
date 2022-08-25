#ifndef POWERPOINTTABLE_HH
#define POWERPOINTTABLE_HH

#include "TableWriter.hh"
#include "TableData.hh"
#include <memory>
#include <iostream>
#include <string>

class PowerpointTable : public TableWriter
{
    public:
        PowerpointTable() {}
        void makeTable(std::shared_ptr<TableData> data, std::ostream& outputStream) override;
        std::string makeHeaderRow(std::vector<std::string> data);
        std::string makeRow(std::string rowName, std::vector<std::string> data);
};

#endif
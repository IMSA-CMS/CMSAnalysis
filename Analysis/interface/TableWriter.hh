#ifndef TABLEWRITER_HH
#define TABLEWRITER_HH

#include <ostream>
#include <memory>

class TableData;

class TableWriter 
{
    public:
        virtual void makeTable(std::shared_ptr<TableData> data, std::ostream& outputStream) = 0;
        virtual ~TableWriter() {}
};


#endif
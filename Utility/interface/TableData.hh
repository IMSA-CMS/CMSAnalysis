#ifndef TABLEDATA_HH
#define TABLEDATA_HH

#include <vector>
#include <string>

class TableData
{
    public:
        TableData(std::vector<std::vector<std::string>> outputs, std::vector<std::string> columnNames, std::vector<std::string> rowNames);
        const std::vector<std::string>& getColumns() const {return columns;}
        const std::vector<std::string>& getRows() const {return rows;}
        const std::vector<std::vector<std::string>>& getEntries() const {return entries;}
        void addRow(std::string massTarget);
        void addColumn(std::string process);
        void addEntry(std::vector<std::string> entry);
        void addEntry(int rowIndex, int colIndex, std::string entry);
        template<typename T>
        //Adds entry of any data type
        void addEntry(int rowIndex, int colIndex, T entry);

    private:
        std::vector<std::string> columns;
        std::vector<std::string> rows;
        std::vector<std::vector<std::string>> entries;
};

template<typename T>
inline void TableData::addEntry(int rowIndex, int colIndex, T entry)
{
    addEntry(rowIndex, colIndex, std::to_string(entry));    
}


#endif
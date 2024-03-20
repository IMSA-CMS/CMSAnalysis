#include "CMSAnalysis/DataCollection/interface/EventLoader.hh"
#include <iostream>


int main(int argc, char **argv)
{
    auto files = EventLoader::fetchRootFiles(argv[1]);

    for (auto fil : files) 
    {
        for (auto name : fil->getFileList()) 
        {
            std::cout << name << std::endl;
        }
    }

    return 0;

}
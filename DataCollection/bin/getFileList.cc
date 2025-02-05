#include "CMSAnalysis/DataCollection/interface/EventLoader.hh"
#include <iostream>

// first parameter is the textfile(like Run2PickFiles/ZZ.txt)
// adding any second param will return the number of files instead
// this is to avoid a bug where we overcount the lines because the program outputs the list file name
int main(int argc, char **argv)
{
    auto files = EventLoader::fetchRootFiles(argv[1]);

    int count = 0;
    for (auto fil : files) 
    {
        for (auto name : fil->getFileList()) 
        {
            count++;
            if(!argv[2]) std::cout << "name: " << name << std::endl;
        }
    }
    if(argv[2]) std::cout << count << std::endl;

    return 0;

}
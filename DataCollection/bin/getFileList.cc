#include "CMSAnalysis/DataCollection/interface/DataCollectionPlan.hh"
#include <iostream>

    // for each loop, then print to screen. include header file for data collection plan 
    // compile is scram b -j, then run it by typing getFileList and then a data card

int main(int argc, char **argv)
{
    auto files = DataCollectionPlan::fetchRootFiles(argv[1]);

    for (const std::string fil : files) {

        std::cout << fil << std::endl;

    }

    return 0;

}
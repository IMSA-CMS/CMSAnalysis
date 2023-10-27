#include "CMSAnalysis/DataCollection/interface/EventLoader.hh"
#include <iostream>


int main(int argc, char **argv)
{
    auto files = EventLoader::fetchRootFiles(argv[1]);

    for (const std::string fil : files) {

        std::cout << fil << std::endl;

    }

    return 0;

}
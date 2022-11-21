#ifndef SIMPLEANALYSIS_HH
#define SIMPLEANALYSIS_HH

#include <vector>
#include <memory>
#include "CrossSectionReader.hh"
#include "SingleProcess.hh"
#include "SimpleChannel.hh"

class SimpleAnalysis
{
    public:
        virtual ~SimpleAnalysis() {}
        //To my knowledge, this has to be defined in derived classes to use that class' variables
        virtual std::shared_ptr<SimpleChannel> getChannel(std::string name) = 0;
};

#endif

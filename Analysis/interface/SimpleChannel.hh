#ifndef SIMPLECHANNEL_HH
#define SIMPLECHANNEL_HH

#include <vector>
#include <TH1.h>
#include <memory>
#include "SimpleProcess.hh"
#include "HistVariable.hh"

class SimpleChannel {
    public:
        SimpleChannel(std::string iName) : name(iName) {}
	std::string getName() {return name;}
	std::vector<std::string> getNames();
        void addProcess(std::shared_ptr<SimpleProcess> process) {processes.push_back(process);}
        std::vector<TH1*> getHists(std::string histType);

    private:
        std::vector<std::shared_ptr<SimpleProcess>> processes;
	std::string name;
};

#endif

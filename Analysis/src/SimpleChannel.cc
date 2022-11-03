#include "CMSAnalysis/Analysis/interface/SimpleChannel.hh"
#include <vector>
#include "TH1.h"
#include "CMSAnalysis/Analysis/interface/SimpleProcess.hh"
#include <memory>

std::vector<TH1*> SimpleChannel::getHists(HistVariable histType) {
    std::vector<TH1*> hists;
    for(std::shared_ptr<SimpleProcess> process : processes) {
        hists.push_back(process->getHist(histType));
    }
    return hists;
}

std::vector<std::string> SimpleChannel::getNames() {
    std::vector<std::string> names;
    for(std::shared_ptr<SimpleProcess> process : processes) {
	names.push_back(process->getName());
    }
    return names;
}

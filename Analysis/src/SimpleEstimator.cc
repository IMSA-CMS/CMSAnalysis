#include "CMSAnalysis/Analysis/interface/SimpleEstimator.hh"
#include "CMSAnalysis/Analysis/interface/Estimator.hh"
#include "CMSAnalysis/Analysis/interface/SingleProcess.hh"
#include "TH1.h"

double SimpleEstimator::getExpectedYield(const SingleProcess* process, std::string dataType, double luminosity) const {
    //Takes the histogram wanted from the file, assigns it hist
    TH1 *hist = dynamic_cast<TH1 *>(process->getHist(dataType));
    if (!hist) {
        throw std::runtime_error("Hist not found");
    }

    int totalEventsInt = process->getTotalEvents();
    double totaleventsran = totalEventsInt;

    //Finds crosssection (from spreadsheet)
    double crosssection = process->getCrossSection();

    double efficiency = hist->GetEntries() / totaleventsran;

    double signalest = crosssection * 1000 * luminosity * efficiency * hist->Integral();
    return signalest;
}
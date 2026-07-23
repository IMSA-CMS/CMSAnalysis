#include "CMSAnalysis/Analysis/interface/HiggsKansasStateNameFinder.hh"
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

std::string HiggsKansasStateNameFinder::getHistName(const HistVariable &histVariable) const
{
    // We only have code to extract the two Higgs masses
    if (!histVariable.isXProjection() && !histVariable.isYProjection())
    {
        throw std::runtime_error("Histogram must be a projection in either the X or Y direction.");
    }
    std::string histName = histVariable.isXProjection() ? "h_mDCH1" : "h_mDCH2";
    const std::string path = channel + "/" + histName;

    return path;
}
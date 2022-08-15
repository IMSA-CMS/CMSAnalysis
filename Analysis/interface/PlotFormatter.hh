#ifndef PLOTFORMATTER_HH
#define PLOTFORMATTER_HH

#include "TCanvas.h"
#include "THStack.h"
#include "TString.h"
#include "Channel.hh"
#include <memory>
#include "HistVariable.hh"

class PlotFormatter
{
    public:
        static TCanvas* formatPlot(std::shared_ptr<Channel> processes, HistVariable histvariable, bool drawLogo, TString extraText, TString xAxisTitle, TString yAxisTitle);
    private:
        PlotFormatter() {}
};

#endif

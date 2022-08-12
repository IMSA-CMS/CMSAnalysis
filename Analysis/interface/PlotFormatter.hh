#ifndef PLOTFORMATTER_HH
#define PLOTFORMATTER_HH

#include "TCanvas.h"
#include "THStack.h"

class PlotFormatter
{
    public:
        static TCanvas* formatPlot(THStack* first, THStack* second);
    private:
        PlotFormatter() {}
};

#endif
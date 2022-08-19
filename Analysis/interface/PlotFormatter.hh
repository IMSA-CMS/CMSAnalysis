#ifndef PLOTFORMATTER_HH
#define PLOTFORMATTER_HH

#include "TCanvas.h"
#include "THStack.h"
#include "TString.h"
#include "Channel.hh"
#include <memory>
#include "HistVariable.hh"
#include "Process.hh"

class PlotFormatter
{
    public:
        //Superimposed stacks
        static TCanvas* superImposedStackHistogram(std::shared_ptr<Channel> processes, HistVariable histvariable, bool drawLogo, TString extraText, TString xAxisTitle, TString yAxisTitle);
        //Superimposed hist
        static TCanvas* superImposedHist(std::shared_ptr<Channel> processes, HistVariable histvariable, bool scaleToExpected, bool drawLogo, TString extraText, TString xAxisTitle, TString yAxisTitle);
        //Single 1D hist
        static TCanvas* simple1DHist(std::shared_ptr<Process> process, HistVariable histvariable, bool scaleToExpected, bool drawLogo, TString extraText, TString xAxisTitle, TString yAxisTitle);
        //Single 2D hist
        static TCanvas* simple2DHist(std::shared_ptr<Process> process, HistVariable histvariable, bool drawLogo, TString extraText, TString xAxisTitle, TString yAxisTitle);
        //Single stacked histogram
        static TCanvas* simpleStackHist(std::shared_ptr<Channel> processes, HistVariable histvariable, bool drawLogo, TString extraText, TString xAxisTitle, TString yAxisTitle);
        //Formats the canvas based on the margins
        static TCanvas* makeFormat(int w, int h, float t, float b, float l, float r);
        //Writes text and draws logo based on the margins
        static void writeText(int w, int h, float t, float b, float l, float r, bool drawLogo, TString extraText);


    private:
        PlotFormatter() {}
};

#endif

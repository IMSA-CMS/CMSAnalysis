#ifndef PLOTFORMATTER_HH
#define PLOTFORMATTER_HH

#include "TCanvas.h"
#include "THStack.h"
#include "TString.h"
#include "Channel.hh"
#include <memory>
#include "HistVariable.hh"
#include "FullAnalysis.hh"
#include "Process.hh"
#include <vector>
#include "TH1.h"
#include "TH2.h"

class PlotFormatter
{
    public:
        PlotFormatter(bool iDrawLogo, TString iExtraText) : drawLogo(iDrawLogo), extraText(iExtraText) {}
        //Superimposed stacks
        TCanvas* superImposedStackHist(std::shared_ptr<Channel> processes, std::string histvariable, 
            TString xAxisTitle, TString yAxisTitle);

        //Superimposed hist (second function is for the simple SuperImpose.C script)
        TCanvas* superImposedHist(std::shared_ptr<Channel> processes, std::string histvariable, 
            TString xAxisTitle, TString yAxisTitle);

        TCanvas* simpleSuperImposedHist(std::vector<TH1*> hists, std::vector<int> colors, std::vector<TString> names, 
            TString xAxisTitle, TString yAxisTitle);

        TCanvas* noScaleSimpleSuperImposedHist(std::vector<TH1*> hists, std::vector<int> colors, std::vector<TString> names, 
            TString xAxisTitle, TString yAxisTitle);

	    TCanvas* simpleAnalysisHist(std::vector<TH1*> hists, std::vector<std::string> names, 
            TString xAxisTitle, TString yAxisTitle);

        //Single 1D hist
        TCanvas* simple1DHist(std::shared_ptr<Process> process, std::string histvariable, bool scaleToExpected, 
            TString xAxisTitle, TString yAxisTitle);

        TCanvas* simple1DHist(TH1* process, TString xAxisTitle, TString yAxisTitle);

        //Single 2D hist
        TCanvas* simple2DHist(std::shared_ptr<Process> process, std::string histvariable, 
            TString xAxisTitle, TString yAxisTitle);

        //Single stacked histogram
        TCanvas* simpleStackHist(std::shared_ptr<Channel> processes, std::string histvariable, 
            TString xAxisTitle, TString yAxisTitle);
            
        //Complete Signal/Background plot
        TCanvas* completePlot(std::shared_ptr<FullAnalysis> analysis, std::string histvariable,
            TString xAxisTitle, TString yAxisTitle, double massTarget, bool scaleTodata, std::string channelName = "");

        //Formats the canvas based on the margins
        TCanvas* makeFormat(int w, int h, float t, float b, float l, float r);
        //Writes text and draws logo based on the margins
        void writeText(int w, int h, float t, float b, float l, float r);
        //Deletes all hist pointers made with the PlotFormatter
        void deleteHists();
    private:
        bool drawLogo;
        TString extraText;
        std::vector<TH1*> histVector;
        std::vector<TH2*> th2Vector;
        std::vector<THStack*> stackVector;
};

#endif
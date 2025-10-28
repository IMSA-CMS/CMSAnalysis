#ifndef PLOTFORMATTER_HH
#define PLOTFORMATTER_HH

#include "TCanvas.h"
#include "THStack.h"
#include "TString.h"
#include "Channel.hh"
#include <memory>
#include "HistVariable.hh"
#include "FullAnalysis.hh"
#include "MultiSystematic.hh"
#include "Process.hh"
#include <vector>
#include "TH1.h"
#include "TH2.h"
#include "TGraph.h"
#include "TGraphErrors.h"

class PlotFormatter
{
    public:
        PlotFormatter(bool iDrawLogo, TString iExtraText) : drawLogo(iDrawLogo), extraText(iExtraText) {}
        //Superimposed stacks
        TCanvas* superImposedStackHist(std::shared_ptr<Channel> processes, HistVariable histvariable, 
            TString xAxisTitle, TString yAxisTitle);

        //Superimposed hist (second function is for the simple SuperImpose.C script)
        TCanvas* superImposedHist(std::shared_ptr<Channel> processes, HistVariable histvariable, 
            TString xAxisTitle, TString yAxisTitle);

        TCanvas* simpleSuperImposedHist(std::vector<TH1*> hists, std::vector<int> colors, std::vector<TString> names, 
            TString xAxisTitle, TString yAxisTitle);

        TCanvas* noScaleSimpleSuperImposedHist(std::vector<TH1*> hists, std::vector<int> colors, std::vector<TString> names, 
            TString xAxisTitle, TString yAxisTitle);

	    TCanvas* simpleAnalysisHist(std::vector<TH1*> hists, std::vector<std::string> names, 
            TString xAxisTitle, TString yAxisTitle);

        //Single 1D hist
        TCanvas* simple1DHist(std::shared_ptr<Process> process, HistVariable histvariable, bool scaleToExpected, 
            TString xAxisTitle, TString yAxisTitle);

        TCanvas* simple1DHist(TH1* process, TString xAxisTitle, TString yAxisTitle);

        //Single 2D hist
        TCanvas* simple2DHist(std::shared_ptr<Process> process, HistVariable histvariable, 
            TString xAxisTitle, TString yAxisTitle);

        //Single stacked histogram
        TCanvas* simpleStackHist(std::shared_ptr<Channel> processes, HistVariable histvariable, 
            TString xAxisTitle, TString yAxisTitle);
            
        //Complete Signal/Background plot
        TCanvas* completePlot(std::shared_ptr<FullAnalysis> analysis, HistVariable histvariable,
            TString xAxisTitle, TString yAxisTitle, bool scaleTodata, bool includeSignal, bool includeData, std::string channelName = "");

        //Deletes all hist pointers made with the PlotFormatter
        void deleteHists();

        void setUpperMasslimit(double limit) { upperMasslimit = limit;}

        double getUpperMasslimit() const { return upperMasslimit;}  

        void setRebinFactor(int bins) { rebinFactor = bins;}

        int getRebinFactor() const { return rebinFactor;} 

        void setFirstBin(int bin) { firstBin = bin;}

        int getFirstBin() const { return firstBin;} 
            
    private:
        //Formats the canvas based on the margins
        TCanvas* makeFormat(int w, int h, float t, float b, float l, float r);
        //Writes text and draws logo based on the margins
        void writeText(int w, int h, float t, float b, float l, float r);
    
        TLegend* GetLegend(THStack* background, std::shared_ptr<Channel> processes, TH1* data,  TH1* signal, bool includeSignal, bool includeData);
        
        void GraphFormat(TGraph*& graph, TGraphErrors*& errorgraph2, TString xAxisTitle, float xLabelSize, float xTitleSize, float yLableSize, float yTitleSize, float markerSize, float maximum, float minimum, float firstBin, float upperMasslimit);
        
        void GetBottomPadValues(TH1*& data, THStack*& background, TH1* backgroundHist, std::vector<double>& x,  std::vector<double>& y,  std::vector<double>& xerror2,  std::vector<double>& yerror2 , std::vector<double>& centers);

        void IntegralScaling(double& upperMasslimit, bool& scaleTodata, std::vector<TH1*>& backgroundHists, int& firstBin, float& dataIntegral, float& backgroundIntegral);
    
        int GetOrder(TH1* data, TH1* signal, THStack* background);

        void DrawOtherHistograms(std::vector<TH1*>& hists, std::vector<int>& colors, int firstIndex);

        void ChangeAxisTitles(TH1*& hist, TString xAxisTitle, TString yAxisTitle);

        void FormatSignalData(THStack*& background, TH1*& signal, TH1*& data, std::vector<TH1*>& backgroundHists, int rebinFactor);
    
        TLegend* GetSimpleLegend(std::vector<TH1*> hists, std::vector<std::string> names);

        TLegend* GetSimpleLegend(std::vector<TH1*> hists, std::vector<TString> names);

        TLegend* GetSuperImposedLegend(std::shared_ptr<Channel> processes, HistVariable histvariable);

        TLegend* GetStackedLegend(std::shared_ptr<Channel> processes, THStack* hists);

        void DrawOtherHistograms(std::vector<TH1*>& hists, int& firstIndex);

        void GetImposedOrder(THStack*& background, THStack*& signal, THStack*& first, THStack*& second);

        void Bin(std::vector<TH1*>& hists, TH1*& first, int& firstIndex, double& maximum, int& count, bool scaleToExpected);

        void GetOrder(std::vector<TH1*>& hists, TH1*& first, int& firstIndex, double& maximum);

        // void SetErrors(std::shared_ptr<Process> process, HistVariable histVariable, TH1* original);

        TH1* CreateErrorHistogram(THStack* hists, std::vector<std::shared_ptr<Process>> processes);

        bool drawLogo;
        TString extraText;
        std::vector<TH1*> histVector;
        std::vector<TH2*> th2Vector;
        std::vector<THStack*> stackVector;
        double upperMasslimit;
        int rebinFactor;
        int firstBin;

};

#endif
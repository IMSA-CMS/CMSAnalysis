#ifndef ROOTFILEINPUT_HH
#define ROOTFILEINPUT_HH

#include "Input.hh"
#include <memory>
#include "HistogramFinder.hh"
#include "HistVariable.hh"

class TFile;

class RootFileInput : public Input
{
	public:
		RootFileInput(std::string fileName, std::shared_ptr<HistogramFinder> iHistFinder);
		//Gets hists, if 2D projects it down to 1D
		TH1* getHist(HistVariable histType) const override;
		//Gets hist without projecting (if 2D)
		TH1* get2DHist(HistVariable histType) const override;
		int getTotalEvents() const override;
		TFile* getFile() const {return file;}
		std::shared_ptr<HistogramFinder> getHistFinder() const override {return histFinder;}
	private:
		std::shared_ptr<HistogramFinder> histFinder;
		std::string fileSource;
		TFile* file;
};


#endif
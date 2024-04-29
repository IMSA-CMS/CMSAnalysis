#ifndef ROOTFILEINPUT_HH
#define ROOTFILEINPUT_HH

#include "Input.hh"
#include <memory>
#include "HistVariable.hh"

class TFile;

class RootFileInput : public Input
{
	public:
		RootFileInput(std::string fileName, std::vector<HistVariable> iHistVariables);
		TFile* getFile(std::string fileName) const;
		//Gets hists, if 2D projects it down to 1D
		TH1* getHist(std::string histType) const override;
		//Gets hist without projecting (if 2D)
		TH1* get2DHist(std::string histType) const override;
		//Gets histogram using its name from within the file, as opposed to its HistVariable type
		TH1* getHistFromName(std::string histName) const;
		int getTotalEvents() const override;
		std::vector<HistVariable> getHistVariables() const override {return histVariables;}
	private:
		std::vector<HistVariable> histVariables;
		std::string fileSource;

};


#endif

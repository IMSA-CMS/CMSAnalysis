#ifndef ROOTFILEINPUT_HH
#define ROOTFILEINPUT_HH

#include "Input.hh"
#include <memory>
#include "HistVariable.hh"
#include <map>

class TFile;

class HistNameFinder;

class RootFileInput : public Input
{
	public:
		// RootFileInput(std::string fileName, std::vector<HistVariable> iHistVariables);
		RootFileInput(std::string fileName, std::vector<HistVariable> iHistVariables, 
		std::shared_ptr<HistNameFinder> histVariableToFileMapping);
		TFile* getFile(std::string fileName) const;
		//Gets hists, if 2D projects it down to 1D
		TH1* getHist(HistVariable histType) const override;
		//Gets hist without projecting (if 2D)
		TH1* get2DHist(HistVariable histType) const override;
		//Gets histogram using its name from within the file, as opposed to its HistVariable type
		TH1* getHistFromName(std::string histName) const;
		int getTotalEvents() const override;
		std::vector<HistVariable> getHistVariables() const override {return histVariables;}
		std::shared_ptr<HistNameFinder> getHistVariableToFileMapping() const {return histVariableToFileMapping; }

	private:
		std::vector<HistVariable> histVariables;
		std::string fileSource;
		std::shared_ptr<HistNameFinder> histVariableToFileMapping;

};


#endif

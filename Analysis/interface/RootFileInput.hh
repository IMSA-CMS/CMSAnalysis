#ifndef ROOTFILEINPUT_HH
#define ROOTFILEINPUT_HH

#include "Input.hh"

class TFile;

class RootFileInput : public Input
{
	public:
		RootFileInput(std::string fileName, std::string iName);
		//Gets hists, if 2D projects it down to 1D
		TH1* getHist() const override;
		//Gets hist without projecting (if 2D)
		TH1* get2DHist() const override;
		int getTotalEvents() const override;
		TFile* getFile() const {return file;}
		std::string getName() const override {return name;}
	private:
		std::string name;
		TFile* file;
};


#endif
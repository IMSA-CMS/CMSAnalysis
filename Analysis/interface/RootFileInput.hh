#ifndef ROOTFILEINPUT_HH
#define ROOTFILEINPUT_HH

#include "Input.hh"

class TFile;

class RootFileInput : public Input
{
	public:
		RootFileInput(std::string fileName, std::string iName);
		TH1* getHist() const override;
		TH1* get2DHist() const override;
		int getTotalEvents() const override;
		TFile* getFile() const {return file;}
		std::string getName() const override {return name;}
	private:
		std::string name;
		TFile* file;
};


#endif
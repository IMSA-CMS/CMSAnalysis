#ifndef ROOTFILEINPUT_HH
#define ROOTFILEINPUT_HH

#include "Input.hh"

class TFile;

class RootFileInput : public Input
{
	private:
		TFile* file;
	public:
		RootFileInput(std::string fileName);
		TH1* getInput(std::string name) const override;
		int getTotalEvents() const override;
};


#endif
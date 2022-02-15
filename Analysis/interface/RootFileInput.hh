#ifndef ROOTFILEINPUT_HH
#define FOOTFILEINPUT_HH

#include "Input.hh"

class TFile

class RootFileInput : public Input
{
	private:
		TFile* file;
	public:
		RootFileInput(std::string fileName);
		TH1* GetInput(std::string name) override;
};


#endif
#ifndef HISTOGRAMPROTOTYPE_HH
#define HISTOGRAMPROTOTYPE_HH

#include <string>
#include <vector>
#include <memory>
#include "CIAnalysis/CIStudies/interface/InputModule.hh"

namespace edm
{
  class EventBase;
}

class FilterModule;

class HistogramPrototype
{
public:
	HistogramPrototype(const std::string& iname, int inBins, double iminimum, double imaximum) :
		name(iname),
		nBins(inBins),
		minimum(iminimum),
		maximum(imaximum)
	{}

	void setInput(std::shared_ptr<InputModule> iInput) {input = iInput;}

	virtual ~HistogramPrototype() {} // Empty virtual destructor
	
	virtual std::vector<double> value() const = 0;

	std::string getName() const {return name;}
	int getNBins() const {return nBins;}
	double getMinimum() const {return minimum;}
	double getMaximum() const {return maximum;}

	bool shouldDraw() const; // Bool switch to determine if a histogram is to be filled
	std::string getFilteredName() const {return (getFilterString() + getName());}

	void addFilter(std::shared_ptr<FilterModule> filterMod) {filters.push_back(filterMod);} // Adds a FilterModule& to filters (the vector)

	std::string getFilterString() const;

protected:
	std::shared_ptr<InputModule> getInput() const {return input;}

private:
	std::shared_ptr<InputModule> input;
	std::string name;
	int nBins;
	double minimum;
	double maximum;
	std::vector<std::shared_ptr<FilterModule>> filters; // Vector of FilterModule&'s
};

#endif

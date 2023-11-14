#ifndef HISTOGRAMPROTOTYPE_HH
#define HISTOGRAMPROTOTYPE_HH

#include <string>
#include <vector>
#include <memory>
#include "Filter.hh"

namespace edm
{
  class EventBase;
}

class EventInput;
class FilterModule;
class TH1;

class HistogramPrototype
{
public:
	HistogramPrototype(const std::string& iname) :
		name(iname)
	{}

	void setInput(const EventInput* iInput);

	virtual ~HistogramPrototype() {} // Empty virtual destructor
	
	virtual std::vector<double> value() const = 0;

    std::string getName() const {return name;}


	bool shouldDraw() const; // Bool switch to determine if a histogram is to be filled
	std::string getFilteredName() const {return (getFilterString() + getName());}

	void addFilter(std::shared_ptr<Filter> filterMod) {filters.push_back(filterMod);} // Adds a FilterModule& to filters (the vector)

	std::string getFilterString() const;

	virtual TH1* makeHistogram() const = 0;
    virtual TH1* makeHistogram(std::string name, std::string title) const = 0; // Makes the histogram with a modified name & title


protected:
	const EventInput* getInput() const {return input;}
	void setName(std::string newName) {name = newName;}
private:
	const EventInput* input;
	std::string name;
	std::vector<std::shared_ptr<Filter>> filters; // Vector of FilterModule&'s
};

#endif

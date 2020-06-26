#ifndef HISTOGRAMPROTOTYPE_HH
#define HISTOGRAMPROTOTYPE_HH

#include <string>

class HistogramPrototype
{
public:
	HistogramPrototype(const std::string& iname, int inBins, double iminimum, double imaximum) :
		name(iname),
		nBins(inBins),
		minimum(iminimum),
		maximum(imaximum)
	{}
	
	virtual ~HistogramPrototype() {} // Empty virtual destructor
	
	virtual double value() const = 0;

	std::string getName() const {return name;}
	int getNBins() const {return nBins;}
	double getMinimum() const {return minimum;}
	double getMaximum() const {return maximum;}

private:
	std::string name;
	int nBins;
	double minimum;
	double maximum;
};

#endif

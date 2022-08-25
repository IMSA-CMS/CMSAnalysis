#ifndef ESTIMATOR_HH
#define ESTIMATOR_HH

#include <memory>
#include "HistVariable.hh"
class SingleProcess;

class Estimator 
{

public: 
	virtual ~Estimator() {}
	virtual double getMassTarget() const = 0;
	virtual double getExpectedYield(const SingleProcess* process, HistVariable dataType, double luminosity) const = 0;
};



#endif
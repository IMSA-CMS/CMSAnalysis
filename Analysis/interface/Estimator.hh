#ifndef ESTIMATOR_HH
#define ESTIMATOR_HH

#include <memory>
#include "HistVariable.hh"

class SingleProcess;

class Estimator 
{

public: 
	virtual ~Estimator() {}
	virtual double getExpectedYield(const SingleProcess* process, const HistVariable& dataType) const = 0;
};



#endif

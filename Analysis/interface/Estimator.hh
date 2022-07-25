#ifndef ESTIMATOR_HH
#define ESTIMATOR_HH

#include <memory>
class SingleProcess;

class Estimator 
{

public: 
	virtual ~Estimator() {}
	virtual double getMassTarget() const = 0;
	virtual double getExpectedYield(const SingleProcess* process, double luminosity) const = 0;
};



#endif
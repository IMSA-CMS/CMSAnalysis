#ifndef FITESTIMATOR_HH
#define FITESTIMATOR_HH

#include "Estimator.hh"

class SingleProcess;

class FitEstimator : public Estimator
{
public: 
	FitEstimator(SingleProcess* process, double massTarget) : Estimator(process), massTarget(massTarget)
	{}
	double getExpectedYield(double luminosity) const override; 
	double getMassTarget() const {return massTarget;}
private: 
	const double massTarget;
};



#endif
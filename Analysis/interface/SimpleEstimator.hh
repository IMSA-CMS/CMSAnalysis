#ifndef SIMPLEESTIMATOR_HH
#define SIMPLEESTIMATOR_HH

#include "Estimator.hh"
#include "Input.hh"
#include <memory>
#include <iostream>
#include "SingleProcess.hh"
#include "HistVariable.hh"

class SingleProcess;

class SimpleEstimator : public Estimator
{		
	public: 
		SimpleEstimator() {}
		double getMassTarget() const override  {return 0;}
		double getExpectedYield(const SingleProcess* process, std::string dataType, double luminosity) const override;
};	



#endif

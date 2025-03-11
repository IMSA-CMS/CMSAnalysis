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
		SimpleEstimator(bool data = false) : isData(data) {}
		double getMassTarget() const override  {return 0;}
		double getExpectedYield(const SingleProcess* process, HistVariable dataType, double luminosity) const override;

	private:
		bool isData;
};	



#endif

#ifndef TRIVIALESTIMATOR_HH
#define TRIVIALESTIMATOR_HH

#include "Estimator.hh"
#include "Input.hh"
#include <memory>
#include <iostream>
#include "SingleProcess.hh"
#include "HistVariable.hh"

class SingleProcess;

class TrivialEstimator : public Estimator
{		
	public: 
		double getMassTarget() const override  {return 0;}
		double getExpectedYield(const SingleProcess* process, const HistVariable& dataType, double luminosity) const override;
		double getBranchingRatioFixer() const { return 1; }

	private:
		
};	



#endif

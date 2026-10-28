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
		TrivialEstimator(double scaleFactor = 1) : scaleFactor(scaleFactor) {}
		double getExpectedYield(const SingleProcess* process, const HistVariable& dataType) const override;
		double getBranchingRatioFixer() const { return 1; }

	private:
		double scaleFactor;
};	



#endif

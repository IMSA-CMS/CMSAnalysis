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
		SimpleEstimator(double scaleFactor = 1, bool data = false, double branchingRatioFixer = 1) : scaleFactor(scaleFactor), isData(data), isBranchingRatioFixer(branchingRatioFixer) {}
		double getMassTarget() const override  {return 0;}
		double getExpectedYield(const SingleProcess* process, const HistVariable& dataType, double luminosity) const override;
		double getBranchingRatioFixer() const { return isBranchingRatioFixer; }

	private:
		double scaleFactor;
		bool isData;
		double isBranchingRatioFixer;
};	



#endif

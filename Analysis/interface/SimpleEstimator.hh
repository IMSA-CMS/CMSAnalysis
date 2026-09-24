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
		SimpleEstimator(double scaleFactor = 1, bool data = false, double branchingRatioFixer = 1) : scaleFactor(scaleFactor), isData(data), branchingRatioFixer(branchingRatioFixer) {}
		double getMassTarget() const override  {return 0;}
		double getExpectedYield(const SingleProcess* process, const HistVariable& dataType, double luminosity) const override;
		double getBranchingRatioFixer() const { return branchingRatioFixer; }
		
		static inline bool verbose = false;

	private:
		double scaleFactor;
		bool isData;
		double branchingRatioFixer;
};	



#endif

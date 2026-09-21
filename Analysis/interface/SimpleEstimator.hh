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
		SimpleEstimator(const std::shared_ptr<CrossSectionReader> iReader, double luminosity, double scaleFactor = 1, 
			bool data = false, double branchingRatioFixer = 1) : reader(iReader), luminosity(luminosity), scaleFactor(scaleFactor), 
			isData(data), isBranchingRatioFixer(branchingRatioFixer) {}
		double getExpectedYield(const SingleProcess* process, const HistVariable& dataType) const override;
		double getBranchingRatioFixer() const { return isBranchingRatioFixer; }

	private:
		const std::shared_ptr<CrossSectionReader> reader;
		double luminosity;
		double scaleFactor;
		bool isData;
		double isBranchingRatioFixer;
};	



#endif

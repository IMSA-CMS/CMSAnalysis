#ifndef FITESTIMATOR_HH
#define FITESTIMATOR_HH

#include "Estimator.hh"
#include "Input.hh"
#include <memory>

class SingleProcess;

class FitEstimator : public Estimator
{
	public: 
		FitEstimator(double massTarget, const std::shared_ptr<Input> iInput) : massTarget(massTarget), fitInput(iInput) {}
		//Expected yield of the singleProcess the estimator is estimating
		double getExpectedYield(const SingleProcess* process, double luminosity) const override; 
		double getMassTarget() const override; 
		TH1* getFitHist() const;
	private: 
		const double massTarget;
		const std::shared_ptr<Input> fitInput;
};



#endif
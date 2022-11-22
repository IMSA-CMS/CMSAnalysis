#ifndef WINDOWESTIMATOR_HH
#define WINDOWESTIMATOR_HH

#include "Estimator.hh"
#include "Input.hh"
#include <memory>
#include <iostream>
#include "SingleProcess.hh"
#include "HistVariable.hh"

class SingleProcess;

class WindowEstimator : public Estimator
{		
	public: 
		WindowEstimator(double low, double high) : lowBound(low), highBound(high) {}
		double getExpectedYield(const SingleProcess* process, std::string dataType, double luminosity) const override;
		double getMassTarget() const override;
	private:
		double lowBound;
		double highBound;
};	



#endif

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
		WindowEstimator(std::shared_ptr<CrossSectionReader> reader, double luminosity, double low, double high) : reader(reader),
			luminosity(luminosity), lowBound(low), highBound(high) {}
		double getExpectedYield(const SingleProcess* process, const HistVariable& dataType) const override;

	private:
		std::shared_ptr<CrossSectionReader> reader;
		double luminosity;
		double lowBound;
		double highBound;
};	



#endif

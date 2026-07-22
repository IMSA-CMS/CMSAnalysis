#ifndef FITESTIMATOR_HH
#define FITESTIMATOR_HH

#include "Estimator.hh"
#include "HistVariable.hh"
#include "Input.hh"
#include "CrossSectionReader.hh"
#include <memory>

class SingleProcess;

class FitEstimator : public Estimator
{
	public: 
		FitEstimator(double massTarget, double luminosity, const std::shared_ptr<Input> iInput, const std::shared_ptr<CrossSectionReader> iReader) :
		 	massTarget(massTarget), luminosity(luminosity), fitInput(iInput), reader(iReader) {}
		//Expected yield of the singleProcess the estimator is estimating
		double getExpectedYield(const SingleProcess* process, const HistVariable& dataType) const override; 
		TH1* getFitHist(const HistVariable& histType) const;
		
	private: 
		const double massTarget;
		const double luminosity;
		const std::shared_ptr<Input> fitInput;
		const std::shared_ptr<CrossSectionReader> reader;
};



#endif

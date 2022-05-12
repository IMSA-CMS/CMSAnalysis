#ifndef FITESTIMATOR_HH
#define FITESTIMATOR_HH
class SingleProcess;
class FitEstimator 
{

public: 
	virtual double getExpectedYield(double luminosity) const override; 
	int massTarget() const {return masstarget;}
private: 
	const int masstarget;




};



#endif
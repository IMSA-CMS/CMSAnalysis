#ifndef ESTIMATOR_HH
#define ESTIMATOR_HH

class Estimator 
{

public: 
	virtual double getExpectedYield(double luminosity) const = 0;

protected: 
	SingleProcess* getProcess() const {return process;}
private: 
	SingleProcess* process;
	


};



#endif
#ifndef ESTIMATOR_HH
#define ESTIMATOR_HH

class SingleProcess;

class Estimator 
{

public: 
	Estimator(SingleProcess* process) : process(process) {}

	virtual ~Estimator() {}
	
	virtual double getExpectedYield(double luminosity) const = 0;

protected: 
	SingleProcess* getProcess() const {return process;}
private: 
	SingleProcess* process;
	


};



#endif
#ifndef SINGLEPROCESS_HH
#define SINGLEPROCESS_HH

#include <string>
#include <vector>

class Estimator;
class ProcessInput;

//SingleProcess Class
class SingleProcess
{
public:
    std::string getName() const {return name;} //Inlining
    double crossSection() const {return crossSection;}
    

private:
    const std::string name;
    const double crossSection;
    const ProcessInput* input;
    Estimator* estimator; 

};




#endif
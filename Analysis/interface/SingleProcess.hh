#ifndef SINGLEPROCESS_HH
#define SINGLEPROCESS_HH

#include <string>
#include <vector>

class Estimator
{
public: 

    std::string getFileName() const {return fileName;}
    double getFileName() const {return fileName;}

private:
    const std::string fileName;
    const double massTarget;
    const double totalEvents;
    const std::string histogramName;
    const double crossSection;
    const bool signalSwitch;
    const std::string analysisName; 
    const std::string nickname;
    const double sumSwitch;
};
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
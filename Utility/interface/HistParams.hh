#ifndef HISTPARAMS_HH
#define HISTPARAMS_HH
#include <functional>
#include <string>
#include <vector>

class Particle;  // Forward declaration of the Particle class

class HistParams {
private:
    std::function<std::vector<double>(Particle)> valueFunction;
    std::string name;
    int bins;
    double minimum;
    double maximum;


public:
    HistParams(const std::string& iname, int inBins, double iminimum, double imaximum, std::function<std::vector<double>(Particle)> function);


    std::function<std::vector<double>(Particle)> getValueFunction() const;

    std::string getName() const;

    int getBins() const;

    double getMinimum() const;
    
    double getMaximum() const;

    void setName(std::string);

    
};

#endif 


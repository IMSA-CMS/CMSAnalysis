#ifndef SINGLEPROCESS_HH
#define SINGLEPROCESS_HH

#include "Input.hh"
#include "CrossSectionReader.hh"
#include "Correction.hh"
#include <string>
#include <vector>
#include <iostream>
#include <memory>
#include "Estimator.hh"
#include "FitEstimator.hh"
#include "HistVariable.hh"

class Estimator;

//SingleProcess Class
class SingleProcess
{
    public:
        SingleProcess(std::string iCrossSectionName, const std::shared_ptr<Input> iInput, const std::shared_ptr<CrossSectionReader> iCrossSectionReader, const std::shared_ptr<Estimator> iEstimator, const double iLuminosity, std::vector<std::shared_ptr<Correction>> iCorrections) : 
        crossSectionName(iCrossSectionName), input(iInput), reader(iCrossSectionReader), estimator(iEstimator), luminosity(iLuminosity), corrections(iCorrections) {}
        std::string getName() const {return crossSectionName;}
        TH1* getHist(HistVariable histType, bool scaleToExpected = false) const;
        TH1* get2DHist(HistVariable histType) const;
        int getTotalEvents() const;
        double getCrossSection() const {return reader->getCrossSection(crossSectionName);}
        double getExpectedYield(HistVariable dataType) const;
        double getLuminosity() const {return luminosity;}
        double getMassTarget() const {return estimator->getMassTarget();}
        std::shared_ptr<Input> getInput() const {return input;}
        //Makes sure that the process contains all of the hists that will be needed
        bool checkValidity();

    private:
        const std::string nickname;
        const std::string crossSectionName;
        const std::shared_ptr<Input> input;
        const std::shared_ptr<CrossSectionReader> reader;
        const std::shared_ptr<Estimator> estimator; 
        const double luminosity;
        std::vector<std::shared_ptr<Correction>> corrections;
};

#endif

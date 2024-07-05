#include "CMSAnalysis/Modules/interface/JSONScaleFactor.hh"
#include "CMSAnalysis/Utility/interface/Particle.hh"
#include <iostream>
#include <fstream>
#include <sstream>
#include "CMSAnalysis/Modules/interface/EventInput.hh"

JSONScaleFactor::JSONScaleFactor(std::string filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return; // Exit constructor if file cannot be opened
    }

    std::string line;
//    bool nextLineIsScaleFactor = false;

    while (std::getline(file, line)) {
        if (line == "\"key\": \"nominal\",") {
 //           nextLineIsScaleFactor = true;
            std::getline(file, line); // Read the next line containing scale factors
            std::istringstream iss(line);
            double pt, eta, sf;
            iss >> pt >> eta >> sf;
            this->scaleFactors[pt][eta] = sf; // Using 'this->' to access member variable
            std::cout << "pt: " << pt << " eta: " << eta << " sf: " << sf << std::endl;
        }
        if (line == "\"key\": \"systup\",") {
  //          nextLineIsScaleFactor = true;
            std::getline(file, line); 
            std::istringstream iss(line);
            double pt, eta, sf;
            iss >> pt >> eta >> sf;
            this->scaleFactors[pt][eta] = sf;
            std::cout << "pt: " << pt << " eta: " << eta << " sf: " << sf << std::endl;
        }
        if (line == "\"key\": \"systdown\",") {
 //           nextLineIsScaleFactor = true;
            std::getline(file, line); 
            std::istringstream iss(line);
            double pt, eta, sf;
            iss >> pt >> eta >> sf;
            this->scaleFactors[pt][eta] = sf; 
            std::cout << "pt: " << pt << " eta: " << eta << " sf: " << sf << std::endl;
        }
    }
}
double JSONScaleFactor::getScaleFactor(const EventInput* input) const {
    double eventWeight = 1.0;

    for (auto lepton : input->getLeptons(EventInput::RecoLevel::Reco)) 
    {
        double pt = lepton.getPt();
        double eta = lepton.getEta();
        
        if (this->scaleFactors.count(pt) && this->scaleFactors.at(pt).count(eta)) 
        {
            eventWeight *= this->scaleFactors.at(pt).at(eta);
        } else 
        {
            // Handle case when scale factor is not found
            // For now, default to 1.0
            eventWeight *= 1.0;
        }
    }

    return eventWeight;
}
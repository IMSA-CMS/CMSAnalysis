#include "CMSAnalysis/Modules/interface/MultiYearScaleFactor.hh"
#include "CMSAnalysis/Modules/interface/EventInput.hh"
#include "CMSAnalysis/Utility/interface/FileParams.hh"
#include "CMSAnalysis/Modules/interface/MuonScaleFactor.hh"
#include "CMSAnalysis/Modules/interface/ElectronScaleFactor.hh"
#include "CMSAnalysis/Utility/interface/ParticleCollection.hh"
#include <stdexcept>
#include <memory>
#include <string>
#include <map>

void MultiYearScaleFactor::addMuonScaleFactor(std::string year, std::shared_ptr<MuonScaleFactor> scaleFactor)
{
	muonScaleFactors.insert({year, scaleFactor});
}
void MultiYearScaleFactor::addElectronScaleFactor(std::string year, std::shared_ptr<ElectronScaleFactor> scaleFactor)
{
	electronScaleFactors.insert({year, scaleFactor});
}



double MultiYearScaleFactor::getScaleFactor(const EventInput* input, SystematicType type) const //add cout statements to see if muons and electrons are getting different numbers
{
	//std::cout << "Process: " << input->getFileParams()->getProcess() << std::endl;
	
	if (input->getFileParams()->getProcess() == "Data")
	{
		//std::cout << "Process is Data, returning scale factor 1.0" << std::endl;
		return 1.0;
	}

	auto year = input->getFileParams()->getParameters().find("Year");
	if (year == input->getFileParams()->getParameters().end())
	{
		throw std::runtime_error ("Year not found in file parameters");
	}
	auto electronsf = electronScaleFactors.find(year->second);
	//std::cout << "Electron Scale Factor: " << electronsf->second->getScaleFactor(input) << std::endl;
	auto muonsf = muonScaleFactors.find(year->second);
	//std::cout << "Muon Scale Factor: " << muonsf->second->getScaleFactor(input) << std::endl;
	if (muonsf == muonScaleFactors.end())
    {
        throw std::runtime_error ("Scale factors for year not found");
    }
	if (muonsf == muonScaleFactors.end())
	{
		throw std::runtime_error ("Scale factors for year not found");
	}
	return electronsf->second->getScaleFactor(input, type) * muonsf->second->getScaleFactor(input, type);
}


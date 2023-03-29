#include "CMSAnalysis/Modules/interface/LeptonJetDumpModule.hh"

#include <cmath>
#include <stdexcept>
#include <fstream>
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Muon.h"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/FWLite/interface/Event.h"
#include "CMSAnalysis/Modules/interface/InputModule.hh"
#include "CMSAnalysis/Utility/interface/ParticleCollection.hh"

LeptonJetDumpModule::LeptonJetDumpModule(std::shared_ptr<LeptonJetMatchingModule> iLepJet, std::shared_ptr<MatchingModule> iMatch, int inumEvents):
    lepJet(iLepJet),
    matchMod(iMatch),
    numEvents(inumEvents)
{
    counter = 1;
    std::ofstream my_file;
    my_file.open("LeptonJetDump.txt", std::ofstream::out | std::ofstream::trunc);
    my_file.close();
}

bool LeptonJetDumpModule::process()
{
    std::ofstream my_file;

    my_file.open("LeptonJetDump.txt", std::ios::app);

    printLeptons(my_file);

    my_file << std::endl;
    my_file << "----------SHOWING INDIVIDUAL LEPTONS JETS---------------" << std::endl;

    printRecoJets(my_file);
    
    my_file << std::endl;
    my_file << "----------SHOWING PARTICLES RECONSTRUCTED AS LEPTONS---------------" << std::endl;

    printWrongRecoLeptons(my_file);

    my_file.close();

    return true;
}

void LeptonJetDumpModule::writeAll()
{

}

void LeptonJetDumpModule::printLeptons(std::ofstream& my_file)
{
    std::vector<Particle> genSim(getInput()->getParticles(InputModule::RecoLevel::GenSim, ParticleType::none()).getParticles());

    std::vector<GenSimParticle> genSimVector;

    for(Particle genParticle: genSim)
    {
      genSimVector.push_back(GenSimParticle(genParticle));
    }

    const MatchingPairCollection& bestMatches = matchMod->getMatchingBestPairs();
    const std::vector<GenSimParticle> genMatch = bestMatches.getGenParticles().getParticles();
    const std::vector<Particle> recoMatch = bestMatches.getRecoParticles().getParticles();

    std::vector<Particle> genLeptons;

    for (Particle lepton : genMatch)
    {
      if (lepton.getPt() >= 5 && (lepton.getType() == ParticleType::electron() || lepton.getType() == ParticleType::muon()))
      {
        genLeptons.push_back(lepton);
      }
    }

    my_file << "------ EVENT "<< counter << " ------" <<std::endl;

    counter++;

    my_file << std::left << std::setw(8) << "element" << std::setw(11) << "| pdfId"
    << std::setw(10) << "| status"
    << std::setw(20) << "| mothers"
    << std::setw(20) << "| daughters"
    << std::setw(15) << "| pT"
    << std::setw(15) << "| Eta"
    << std::setw(15) << "| Phi"
    << std::setw(15) << "| E"
    << std::setw(5) << "| mass\n";

    if(genLeptons.size() == 0) // checks if event is empty
    {
      return;
    }

    std::vector<Particle> particleParentage;

    for(GenSimParticle lepton : genLeptons)
    {
      GenSimParticle particle = lepton;
      Particle recoLepton = recoMatch.at(std::find(genMatch.begin(),genMatch.end(),lepton) - genMatch.begin());
      particleParentage.insert(particleParentage.begin(), recoLepton);
      particleParentage.insert(particleParentage.begin(), Particle::nullParticle()); //will be used as new line for each lepton lineage

      while(particle.isNotNull() && !(lepJet -> isQuark(particle) || lepJet -> isSquark(particle) || particle.status() == 4))
      {

        particleParentage.insert(particleParentage.begin(), particle);
        particle = particle.mother(); // **Probably later change to unique mother, but for now keep**

      } 

      particleParentage.insert(particleParentage.begin(), particle); //this prints the squark/quark/proton information

    } 
    //particleParentage.erase(particleParentage.begin());

    int eventElement = 0;
    for (size_t i = 0; i < particleParentage.size(); i++)
    {
        if(particleParentage[i] != Particle::nullParticle())
        {
            my_file << std::setw(8) << eventElement <<std::pair<GenSimParticle, std::vector<GenSimParticle>>{particleParentage[i], genSimVector} << std::endl;
            eventElement++;
        }
        else
        {
            i++;
            my_file << std::setw(8) << eventElement << GenSimParticle(particleParentage[i]) << std::endl;
            my_file << std::endl;
            // std::cout<<std::endl;
            // std::cout<< i << " "<< particleParentage.size() <<std::endl;
            //std::cout<<particleParentage[i].isNotNull() <<std::endl;
            eventElement++;
        }
    }
    
}
void LeptonJetDumpModule::printRecoJets(std::ofstream& my_file)
{
    std::vector<std::pair<Particle,LeptonJet>> recoJets = lepJet->getMatchingPairs();
    for (std::pair<Particle,LeptonJet> leptonPair : recoJets)
    {
        my_file << std::setw(8) << "" << GenSimParticle(leptonPair.first) <<std::endl;
        for(GenSimParticle lepton : leptonPair.second.getParticles())
        {
            my_file << std::setw(8) << "" << lepton <<std::endl;
        }
        my_file << std::endl;
    }
}
void LeptonJetDumpModule::printWrongRecoLeptons(std::ofstream& my_file)
{
    std::vector<Particle> genSim(getInput()->getParticles(InputModule::RecoLevel::GenSim, ParticleType::none()).getParticles());

    std::vector<GenSimParticle> genSimVector;

    for(Particle genParticle: genSim)
    {
      genSimVector.push_back(GenSimParticle(genParticle));
    }

    const MatchingPairCollection& bestMatches = matchMod->getMatchingBestPairs();
    const std::vector<GenSimParticle> genMatch = bestMatches.getGenParticles().getParticles();
    const std::vector<Particle> recoMatch = bestMatches.getRecoParticles().getParticles();

    for (size_t i = 0; i < genMatch.size(); i++)
    {
        if(recoMatch[i].getType() == ParticleType::electron() || recoMatch[i].getType() == ParticleType::muon())
        {
            if(genMatch[i].getType() != ParticleType::electron() || genMatch[i].getType() != ParticleType::muon())
            {
                my_file << std::setw(8) <<""<<std::pair<GenSimParticle, std::vector<GenSimParticle>>{genMatch[i], genSimVector} << std::endl;
                my_file << std::setw(8) <<""<< GenSimParticle(recoMatch[i]) << std::endl;
                my_file << std::endl;
            }
        }
    }
    
}
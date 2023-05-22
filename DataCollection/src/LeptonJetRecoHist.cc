#include "CMSAnalysis/DataCollection/interface/LeptonJetRecoHist.hh"

#include "CMSAnalysis/DataCollection/interface/MatchingModule.hh"

#include "CMSAnalysis/DataCollection/interface/LeptonJetReconstructionModule.hh"

#include "CMSAnalysis/DataCollection/interface/ParticleCollection.hh"

//#include "CMSAnalysis/DataCollection/interface/MiniAODEventFile.hh"

 

#include <vector>

 

LeptonJetRecoHist::LeptonJetRecoHist(std::shared_ptr<MatchingModule> iRecoMatchingModule,bool ielec,std::shared_ptr<LeptonJetReconstructionModule> ileptonJetReco, const std::string& iname, int iNBins, double iminimum, double imaximum) :

  HistogramPrototype1D(iname, iNBins, iminimum, imaximum),

  recoMatchingModule(iRecoMatchingModule),

  leptonJetRecoModule(ileptonJetReco)

 

{

    elec = ielec;

}

 

std::vector<double> LeptonJetRecoHist::value() const

{

    //const auto &jets = leptonJetRecoModule->getLeptonJets();

    std::vector<double> counts;

   

    //const ParticleCollection<GenSimParticle> allGenParticles = getInput()->getParticles(InputModule::RecoLevel::GenSim);

    //const ParticleCollection<Particle> allRecoParticles = getInput()->getParticles(InputModule::RecoLevel::Reco);

    std::vector<Particle> allGenParticles(getInput()->getParticles(InputModule::RecoLevel::GenSim).getParticles());

    std::vector<Particle> recoCandidates(getInput()->getParticles(InputModule::RecoLevel::Reco).getParticles());

    //std::cout<<"size of GenParticles "<<getInput()->getParticles(InputModule::RecoLevel::GenSim).getParticles().size()<<std::endl;

    //std::cout<<"size of RecoParticles "<<getInput()->getParticles(InputModule::RecoLevel::Reco).getParticles().size()<<std::endl;

 

    const MatchingPairCollection& bestLeptonPairs = recoMatchingModule->getMatchingBestPairs();

    const ParticleCollection<GenSimParticle> underlyingLepton = bestLeptonPairs.getGenParticles();

    const std::vector<Particle> recoLeptons = bestLeptonPairs.getRecoParticles().getParticles();

 

    const std::vector<LeptonJet> recoJets = leptonJetRecoModule -> getLeptonJets();

    std::vector<Particle> matchedReco;

    for(GenSimParticle genParticle: allGenParticles){

        //genParticle = GenSimParticle(genParticle);

        //std::cout<< "Particle ID is "<<genParticle.pdgId()<<std::endl;

        //std::cout<< "True Number of Daughters? "<<genParticle.numberOfDaughters()<<std::endl;

        //std::cout<< "True Number of Daughters? "<<genParticle.mother().pdgId()<<std::endl;

        //std::cout<< "FinalDaughter PdgID "<<genParticle.finalDaughter().numberOfDaughters()<<std::endl;

        //std::cout<<std::endl;

        if(genParticle.pdgId() == 4900022 && genParticle.finalDaughter() == genParticle){

            //std::cout<<"Number of Daughters "<< genParticle.numberOfDaughters()<<std::endl;

            if(genParticle.numberOfDaughters() == 0){

                continue;

            }

            double count = 0;

            int daughterID = genParticle.daughter(0).pdgId();

            if((elec && abs(daughterID)==11) || ((!elec) && abs(daughterID) == 13)){

                int index = 0;

                std::cout<<underlyingLepton.getParticles().size()<<std::endl;

                for(GenSimParticle matchedGenParticle : underlyingLepton){

                    if(matchedGenParticle == genParticle.daughter(0).finalDaughter() || matchedGenParticle == genParticle.daughter(1).finalDaughter()){

                        count++;

                        if(genParticle.daughter(0).finalDaughter().getPt() >= 5){

                            if(genParticle.daughter(1).finalDaughter().getPt() >= 5){

                                if(abs(genParticle.daughter(0).finalDaughter().getEta()) <= 1.8){

                                    if(abs(genParticle.daughter(1).finalDaughter().getEta()) <= 1.8){

                                        //count++;

                                        //matchedReco.push_back(recoLeptons.at(index));

                                    }

                                //countAndDeltaR.push_back(match);

                                }

                            }

                        }

                    }

                    if(count>2){

                        std::cerr<<"No Bueno"<<std::endl;

                    }

                    index++;

                }

                std::cout<<count<<std::endl;

                counts.push_back(count);

                // cuts particles that dont have required pt or eta

            }

        }

    }

    // int efficiency = 0;

    // for(Particle matchedRecoParticle:matchedReco){

    //     for(LeptonJet recoJet: recoJets){

    //         std::vector<Particle> recoParticles = recoJet.getParticles();

    //         for(Particle recoJetParticles: recoParticles){

    //             if(matchedRecoParticle == recoJetParticles){

    //                 efficiency++;

    //             }

    //         }

    //     }

    // }

    return counts;

}
// #include "CMSAnalysis/DataCollection/interface/EventInput.hh"
// #include "CMSAnalysis/DataCollection/interface/ParticleCollection.hh"
// #include "CMSAnalysis/DataCollection/interface/EventLoader.hh"
// #include "CMSAnalysis/DataCollection/interface/Selector.hh"

// EventInput::EventInput(const EventLoader *iEventLoader) : eventLoader(iEventLoader)
// {
// }

// ParticleCollection EventInput::getLeptons(RecoLevel level) const
// {
//     ParticleCollection leptons;
//     auto electrons = getParticles(level, Particle::Type::Electron).getParticles();
//     auto muons = getParticles(level, Particle::Type::Muon).getParticles();
//     for (const auto &p : electrons)
//     {
//         if (!leptonSelector || leptonSelector->checkParticle(p))
//         {
//             leptons.addParticle(p);
//         }
//     }
//     for (const auto &p : muons)
//     {
//         if (!leptonSelector || leptonSelector->checkParticle(p))
//         {
//             leptons.addParticle(p);
//         }
//     }
//     return leptons;
// }

// ParticleCollection EventInput::getParticles(RecoLevel level, Particle::Type particleType,  std::shared_ptr<Selector> selector) const
// {
//     ParticleCollection particleList;
//     if (level == RecoLevel::GenSim)
//     {
//         auto particles = eventLoader->getFile()->getGenSimParticles().getParticles();
//         for (const auto &p : particles)
//         {
//             if ((p.getType() == particleType || particleType == Particle::Type::None) && p.isFinalState())
//             {
//                 if (!selector || selector->checkParticle(p))
//                 {
//                     particleList.addParticle(p);
//                 }
//             }
//         }
//     }
//     else if (level == RecoLevel::Reco)
//     {
//         auto particles = eventLoader->getFile()->getRecoParticles().getParticles();
//         for (const auto &p : particles)
//         {
//             if ((p.getType() == particleType || particleType == Particle::Type::None))
//             {
//                 if (!selector || selector->checkParticle(p))
//                 {
//                     particleList.addParticle(p);
//                 }
//             }
//         }
//     }
//     return particleList;
// }

// /* TODO: getJets */
// ParticleCollection EventInput::getJets(RecoLevel level, double pTcut) const
// {
//     ParticleCollection particleList;
//     if (level == RecoLevel::GenSim)
//     {
//         throw std::runtime_error("GenSim Jets not implemented");
//     }
//     else if (level == RecoLevel::Reco)
//     {
//         auto particles = eventLoader->getFile()->getRecoJets().getParticles();
//         for (const auto &p : particles)
//         {
// 	  if (p.getPt() >= pTcut)
//             {
//                 particleList.addParticle(p);
//             }
//         }
//     }
//     return particleList;
// }

// /*
// std::vector<PileupSummaryInfo> EventInput::getPileupInfo() const
// {
//     return eventLoader->getPileupInfo();
// }
// */

// double EventInput::getMET() const
// {
//     return eventLoader->getFile()->getMET();
// }       
// std::vector<bool> EventInput::getTriggerResults(std::string subProcess) const
// {
//     return eventLoader->getFile()->getTriggerResults(subProcess);
// }
// std::vector<std::string> EventInput::getTriggerNames(std::string subProcess) const
// {
//     return eventLoader->getFile()->getTriggerNames(subProcess);
// }

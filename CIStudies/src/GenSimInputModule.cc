#include "GenSimInputModule.hh"

GenSimInputModule::GenSimInputModule()
{

}

bool GenSimInputModule::process()
{

  //std::cerr << "ENTERING GenSimIdentificationModule" << std::endl;
  genParticles.clear();

  //Get Events Tree and create handle for GEN

  edm::Handle<std::vector<reco::GenParticle>> genParticlesHandle;
  getEvent().getByLabel(std::string("prunedGenParticles"), genParticlesHandle);
 
  const int electronCode = 11;
  const int muonCode = 13;

  auto particle = getFileParams().getLeptonType();

  int targetCode = particle == "Electron" ? electronCode : muonCode;

}
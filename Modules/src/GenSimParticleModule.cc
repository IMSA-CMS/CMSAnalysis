#include "CMSAnalysis/Modules/interface/GenSimParticleModule.hh"
#include "TText.h"
#include "TCanvas.h"
#include "TF1.h"
#include <cmath>
#include <stdexcept>
#include "TPad.h"
#include "TFile.h"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/FWLite/interface/Event.h"

GenSimParticleModule::GenSimParticleModule(int itargetPdgId):
  targetPdgId(itargetPdgId)
{}

//update to remove event parameter
bool GenSimParticleModule::process()
{
  genParticles.clear();

  //Get Events Tree and create handle for GEN

  auto rawGenParticles = getInput()->getLeptons(InputModule::RecoLevel::GenSim).getParticles();
 
  //Begin GEN looping
  //Loop through Particle list&
  for (auto p : rawGenParticles)
  {
    auto genSimP = GenSimParticle(p);
    if (genSimP.pdgId() == targetPdgId)
    {
      int numDaughters = genSimP.numberOfDaughters();
      if (numDaughters == 0)
      {
        genParticles.addParticle(genSimP);
      }
      else
      {
        int counter = 0;
        for (int i = 0; i < numDaughters; ++i)
        {
          auto partDaughter = genSimP.daughter(i);
          if (genSimP.pdgId() == partDaughter.pdgId())
          {
            counter += 1;
          }
        }
        if (counter == 0)
        {
          genParticles.addParticle(genSimP);
        }
      }
    }
  }
 return true;
}
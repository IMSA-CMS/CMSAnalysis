#include "CMSAnalysis/Modules/interface/HiggsCombineStripModule.hh"

HiggsCombineStripModule::HiggsCombineStripModule(){

}

void HiggsCombineStripModule::addVariables(){
    addVariable("massPos", SpecialVariableModule::VariableType::Float);
    addVariable("massNeg", SpecialVariableModule::VariableType::Float);

}

void HiggsCombineStripModule::calculateVariables(ParticleCollection<Particle> particles){
    double cuttoff = 100;
    auto PositiveParticles = particles.getPosParticles();
    auto NegativeParticles = particles.getNegParticles();
    auto HighMassPosParticles = PositiveParticles.chooseParticles(false,true);
    auto HighMassNegParticles = NegativeParticles.chooseParticles(false,true);
    ParticleCollection<Particle> PosPair;
    PosPair.addParticle(HighMassPosParticles.first);
    PosPair.addParticle(HighMassPosParticles.second);
    ParticleCollection<Particle> NegPair;
    NegPair.addParticle(HighMassNegParticles.first);
    NegPair.addParticle(HighMassNegParticles.second);

    if(HighMassPosParticles.first.isNotNull() && HighMassPosParticles.second.isNotNull()){
        if(PosPair.calculateSameSignInvariantMass(false, true) > cuttoff){
            addValue("massPos", PosPair.calculateSameSignInvariantMass(false,true));
        }
    }
    if(HighMassNegParticles.first.isNotNull() && HighMassNegParticles.second.isNotNull()){
        if(NegPair.calculateSameSignInvariantMass(false, true) > cuttoff){
            addValue("massNeg", NegPair.calculateSameSignInvariantMass(false,true));
        }
    }
}

#include "CMSAnalysis/Modules/interface/LeptonJetReconstructionModule.hh"
#include "CMSAnalysis/Utility/interface/LeptonJetCalculation.hh"
#include "DataFormats/Math/interface/deltaR.h"
#include <iostream>
#include <functional>

LeptonJetReconstructionModule::LeptonJetReconstructionModule(double deltaRCut) : DeltaRCut(deltaRCut) {}

bool LeptonJetReconstructionModule::process()
{
    if (particleHistograms.size() == 0) {
        auto inputDeltaRHist = std::make_shared<MLStripHist>(
            "Input Delta R Values", 100, 0, 0.5, LeptonJetCalculation::calculateDeltaR);
        auto deltaPtHist = std::make_shared<MLStripHist>(
            "Input Delta Pt Values", 100, 0, 1000, LeptonJetCalculation::calculateDeltaPt);
        auto leadingPtHist = std::make_shared<MLStripHist>(
            "Input Leading Pt Values", 100, 0, 1000, LeptonJetCalculation::calculateLeadingPt);
        auto sumPtHist =
            std::make_shared<MLStripHist>("Input Sum Pt Values", 100, 0, 1100, LeptonJetCalculation::calculateSumPt);
        auto nParticlesHist = std::make_shared<MLStripHist>(
            "Input nParticles Values", 5, 0, 5, LeptonJetCalculation::calculateNParticles);
        auto etaHist =
            std::make_shared<MLStripHist>("Input Eta Values", 100, -3.5, 3.5, LeptonJetCalculation::calculateEta);
        auto maxIsolationHist = std::make_shared<MLStripHist>(
            "Input MaxIsolation Values", 50, 0, 3, LeptonJetCalculation::calculateMaxIsolation);

        particleHistograms.insert({"deltaR", inputDeltaRHist});
        particleHistograms.insert({"deltaPt", deltaPtHist});
        particleHistograms.insert({"leadingPt", leadingPtHist});
        particleHistograms.insert({"sumPt", sumPtHist});
        particleHistograms.insert({"nParticles", nParticlesHist});
        particleHistograms.insert({"eta", etaHist});
        particleHistograms.insert({"maxIsolation", maxIsolationHist});

        histMod->addHistogram(inputDeltaRHist);
        histMod->addHistogram(deltaPtHist);
        histMod->addHistogram(leadingPtHist);
        histMod->addHistogram(sumPtHist);
        histMod->addHistogram(nParticlesHist);
        histMod->addHistogram(etaHist);
        histMod->addHistogram(maxIsolationHist);
    }

    const auto &recoCandidates = getInput()->getParticles(EventInput::RecoLevel::Reco, ParticleType::muon());
    leptonJets = findLeptonJets(recoCandidates);
    findDeltaRValues();
    findPtValues();

    return true;
}

const std::vector<LeptonJet> LeptonJetReconstructionModule::findLeptonJets(ParticleCollection<Lepton> recoCandidates)
{
    auto recoLeptons = recoCandidates.getParticles();
    std::vector<LeptonJet> leptonJetList;
    while (!recoLeptons.empty()) {
        Lepton highestPtLepton = findHighestPtLepton(recoLeptons);
        if (highestPtLepton.getPt() <= 5) {
            break;
        } else if (abs(highestPtLepton.getEta()) > 3) {
            recoLeptons.erase(std::remove(recoLeptons.begin(), recoLeptons.end(), highestPtLepton), recoLeptons.end());
            continue;
        }
        LeptonJet jet = createLeptonJet(highestPtLepton);
        auto highestPtLeptonFourVector = highestPtLepton.getFourVector();
        recoLeptons.erase(std::remove(recoLeptons.begin(), recoLeptons.end(), highestPtLepton), recoLeptons.end());

        for (auto it = recoLeptons.begin(); it != recoLeptons.end(); ++it) {
            auto fourVector = it->getFourVector();
            double deltaR = reco::deltaR(highestPtLeptonFourVector, fourVector);
            if (deltaR < DeltaRCut && it->getPt() >= 5 && abs(it->getEta()) <= 3) {
                jet.addParticle(*it);
                it = recoLeptons.erase(it);
                --it;
            }
        }
        if (jet.getNumParticles() > 1) {
            count++;
            histJet = jet;

            for (const auto &pair : particleHistograms) {
                std::shared_ptr<MLStripHist> hist = pair.second;
                hist->setLeptonJet(jet);
            }

            leptonJetList.push_back(jet);
        }
    }

    return leptonJetList;
}

LeptonJet LeptonJetReconstructionModule::createLeptonJet(Lepton highestPtLepton) const
{
    LeptonJet leptonJet;
    leptonJet.addParticle(highestPtLepton);
    return leptonJet;
}

Particle LeptonJetReconstructionModule::findHighestPtLepton(std::vector<Lepton> leptons) const
{
    if (leptons.empty()) {
        throw std::runtime_error("No leptons available to find the highest Pt lepton.");
    }

    auto it = std::max_element(leptons.begin(), leptons.end(), [](const Lepton& a, const Lepton& b) {
        return a.getPt() < b.getPt();
    });

    return *it;
}

void LeptonJetReconstructionModule::findDeltaRValues()
{
    deltaRValues.clear();
    for (const auto &jet : leptonJets) {
        auto jetParticles = jet.getParticles();
        for (const auto &particle : jetParticles) {
            auto initFourVector = particle.getFourVector();
            for (const auto &part : jetParticles) {
                if (part != particle) {
                    auto nextFourVector = part.getFourVector();
                    double deltaR = reco::deltaR(initFourVector, nextFourVector);
                }
            }
        }
    }
}

void LeptonJetReconstructionModule::findPtValues()
{
    pTValues.clear();
    for (const auto &jet : leptonJets) {
        auto jetParticles = jet.getParticles();
        for (const auto &part : jetParticles) {
            double pT = part.getPt();
            pTValues.push_back(pT);
        }
    }
}

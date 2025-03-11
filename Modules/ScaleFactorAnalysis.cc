#include "CMSAnalysis/Modules/interface/ScaleFactorAnalysis.hh"
#include "CMSAnalysis/Modules/interface/EventInput.hh"
#include <iostream>

void ScaleFactorAnalysis::initialize()
{
    bool oldVal = TH1::GetDefaultSumw2();
    defHist = new TH1F("Unweighted", "Unweighted", histBins, low, high);
    TH1::SetDefaultSumw2(kTRUE);

    // Create histograms for with and without scale factors
    withScaleHist = new TH1F("WithScale", "With Scale Factors", histBins, low, high);
    withoutScaleHist = new TH1F("WithoutScale", "Without Scale Factors", histBins, low, high);

    // Create the histogram vector
    for (int i = 0; i < 100; ++i) {
        std::string s = "PDFWeight" + std::to_string(i);
        const char* thName = s.c_str();
        TH1* newHist = new TH1F(thName, thName, histBins, low, high);
        histVec.push_back(newHist);
    }
    TH1::SetDefaultSumw2(oldVal);
}

bool ScaleFactorAnalysis::process()
{
    auto recoParticles = getInput()->getLeptons(EventInput::RecoLevel::Reco);
    if (recoParticles.size() == 0) {
        return true;
    }

    auto& particle = recoParticles[0];
    for (int i = 0; i < 100; ++i) {
        auto pdfWeight = particle.getInfo("pweight" + std::to_string(i));
        auto hist = histVec[i];

        // Fill histograms
        hist->Fill(recoInvMass, pdfWeight);

        // With scale factors
        double scaleFactor = particle.getInfo("scaleFactor");  // Example placeholder
        withScaleHist->Fill(recoInvMass, pdfWeight * scaleFactor);

        // Without scale factors
        withoutScaleHist->Fill(recoInvMass, pdfWeight);
    }

    return true;
}

void ScaleFactorAnalysis::finalize()
{
    // Create residual histograms
    withScaleResHist = new TH1F("WithScaleResidual", "With Scale Residual", histBins, low, high);
    withoutScaleResHist = new TH1F("WithoutScaleResidual", "Without Scale Residual", histBins, low, high);

    for (int i = 0; i < histBins; ++i) {
        auto defBin = defHist->GetBinContent(i);
        auto withBin = withScaleHist->GetBinContent(i);
        auto withoutBin = withoutScaleHist->GetBinContent(i);

        // Avoid division by zero
        if (defBin != 0) {
            withScaleResHist->SetBinContent(i, (defBin - withBin) / defBin);
            withoutScaleResHist->SetBinContent(i, (defBin - withoutBin) / defBin);
        } else {
            withScaleResHist->SetBinContent(i, 0);
            withoutScaleResHist->SetBinContent(i, 0);
        }
    }

    // Write histograms
    defHist->Write();
    withScaleHist->Write();
    withoutScaleHist->Write();
    withScaleResHist->Write();
    withoutScaleResHist->Write();
}

#include <TCanvas.h>
#include <TFile.h>
#include <TString.h>
#include <TText.h>

void CombineCanvases() {
    // Define the list of ROOT files
    std::vector<TString> fileList = {
        "MultiSuperPlots/superplot_RecoConfig1_DPTrigger_MuPtCut_Inputs_1stHighestLeptonJetPt_HMOS.root",
        "MultiSuperPlots/superplot_RecoConfig1_DPTrigger_MuPtCut_Inputs_1stHighestLeptonJetPt_HMSS.root",
        "MultiSuperPlots/superplot_RecoConfig1_DPTrigger_MuPtCut_Inputs_1stHighestLeptonJetPt_LMSS.root",
        "MultiSuperPlots/superplot_RecoConfig1_DPTrigger_MuPtCut_Inputs_1stHighestmu-Pt_HMOS.root",
        "MultiSuperPlots/superplot_RecoConfig1_DPTrigger_MuPtCut_Inputs_1stHighestmu-Pt_HMSS.root",
        "MultiSuperPlots/superplot_RecoConfig1_DPTrigger_MuPtCut_Inputs_1stHighestmu-Pt_LMSS.root",
        "MultiSuperPlots/superplot_RecoConfig1_DPTrigger_MuPtCut_Inputs_InputDeltaPtValues_HMOS.root",
        "MultiSuperPlots/superplot_RecoConfig1_DPTrigger_MuPtCut_Inputs_InputDeltaPtValues_HMSS.root",
        "MultiSuperPlots/superplot_RecoConfig1_DPTrigger_MuPtCut_Inputs_InputDeltaPtValues_LMSS.root",
        "MultiSuperPlots/superplot_RecoConfig1_DPTrigger_MuPtCut_Inputs_InputLeadingPtValues_HMOS.root",
        "MultiSuperPlots/superplot_RecoConfig1_DPTrigger_MuPtCut_Inputs_InputLeadingPtValues_HMSS.root",
        "MultiSuperPlots/superplot_RecoConfig1_DPTrigger_MuPtCut_Inputs_InputLeadingPtValues_LMSS.root",
        // Add more files here
    };

    // Create a large canvas to hold all the individual canvases
    TCanvas *largeCanvas = new TCanvas("largeCanvas", "Combined Canvases", 3600, 600 * static_cast<int>(fileList.size()));
    largeCanvas->Divide(2, fileList.size()); // Adjust the number of divisions as needed

    int padIndex = 1;

    for (const auto& fileName : fileList) {
        // Open the ROOT file
        TFile *file = TFile::Open(fileName);
        if (!file || file->IsZombie()) {
            std::cerr << "Error: Could not open file " << fileName << std::endl;
            continue;
        }

        // Get the canvas from the file
        TCanvas *c1 = (TCanvas*)file->Get("Canvas800_600"); // Assuming the canvas is named "c1"
        if (!c1) {
            std::cerr << "Error: Canvas 'c1' not found in file " << fileName << std::endl;
            file->Close();
            continue;
        }

        // Draw the canvas on the corresponding pad of the large canvas
        largeCanvas->cd(padIndex);
        c1->DrawClonePad();

        // Add the filename as a label
        largeCanvas->cd(padIndex + 1);
        TText *text = new TText(0.1, 0.5, fileName);
        text->SetTextSize(0.05);
        text->Draw();

        padIndex += 2; // Move to the next pair of pads

        // Close the file
        file->Close();
    }

    // Update the large canvas
    largeCanvas->Update();

    // Optionally save the large canvas to a file
    largeCanvas->SaveAs("combined_canvases.png");
}

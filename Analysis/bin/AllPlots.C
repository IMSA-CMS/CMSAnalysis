// ============================================================================
// AllPlots.C — Higgs 3-lepton analysis, five presentation plots
//
// Run on cmslpc (needs CMSSW env sourced, HiggsCompleteAnalysis / PlotFormatter
// on your include path). This will NOT run outside that environment — there's
// no ROOT, no CMSSW, and none of the analysis data anywhere else.
//
//   root -l 'AllPlots.C'
//
// Each Plot_N() function prints GetEntries()/Integral() (or the raw numbers
// for Plot 2/5) to stdout before saving. Check those against the confirmed
// values below before trusting the PNG/PDF.
//
// Sourced facts used below:
//   - Signal process string:      "Higgs Signal 1000"
//   - Background processes (sum): "ZZ Background", "WJets Background",
//                                  "Drell-Yan Background", "QCD Background",
//                                  "t#bar{t}, Multiboson Background"
//   - HistVariable for 3-lepton:  VariableType::RecoSameSignInvariantMass
//   - getHist signature:          getHist(HistVariable, processName, bool, channelName)
//       -> the bool arg's real meaning was never confirmed from source, only
//          inferred from call-site usage. Flagged inline as VERIFY.
//   - completePlot hardcodes color internally (PlotFormatter.cc:507,
//     FormatSignalData) and takes no color argument — that's why Plots 1/3/4
//     go through getHist() + simpleSuperImposedHist() instead, which is
//     confirmed index-safe: colors.at(i) maps to hists.at(i) directly
//     (verified via DrawOtherHistograms, PlotFormatter.cc:820).
//   - Plot 2 numbers: 07/15/26 tuning notes, eet_ channel, numFiles=5 held
//     constant across all five cuts (numFiles=1 background counts were
//     statistically meaningless).
//   - Plot 3 channel: "eet_" is not a real directory in the ROOT files
//     (confirmed via rootls). Using "ete_" instead: same lepton content (2
//     electrons + 1 tau), different ordering, confirmed-real directory with
//     its own distinct nonzero yield (945 signal / 17 background, numFiles=1).
//   - Plot 5 numbers: live runAnalyzer output, both signal and background
//     sides, for the 9 channels with confirmed nonzero background reco.
//   - Plot 4: 3-lepton (eee_) vs 4-lepton (eeee) shape comparison, normalized
//     to unit area.
//   - Margins/style taken from WindowAnalysis.C: left/right/bottom
//     0.12/0.05/0.12, top 0.16, SetGridx/SetGridy(true), graph titles via the
//     "Title;X;Y" semicolon form, TLatex pseudo-legend ("CMSAnalysis Internal"
//     + color-matched subtitle lines) instead of TLegend, Modified()/Update()
//     before saving, Unlink() before SaveAs, SaveAs for PNG but
//     Print(file,"pdf") for PDF, explicit delete cleanup on graphs/hists/
//     canvases at the end of each block. Applied exactly on Plot 2/5 (canvases
//     owned outright).
//   - Plots 1/3/4 canvases come from simpleSuperImposedHist's internal
//     makeFormat(...) — not overriding margins on those canvases, only colors
//     (confirmed safe) plus grid lines via gPad and the same TLatex
//     pseudo-legend pattern, since that's drawing on top, not margin
//     ownership.
// ============================================================================

#include "CMSAnalysis/Analysis/interface/HiggsCompleteAnalysis.hh"
#include "CMSAnalysis/Analysis/interface/PlotFormatter.hh"
#include "CMSAnalysis/Analysis/interface/HistVariable.hh"
#include "TCanvas.h"
#include "TColor.h"
#include "TH1.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TSystem.h"
#include "TCollection.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <memory>
#include <vector>
#include <string>

namespace
{
const std::string kSignalProcess = "Higgs Signal 1000";
const std::vector<std::string> kBackgroundProcesses = {
    "ZZ Background", "WJets Background", "Drell-Yan Background",
    "QCD Background", "t#bar{t}, Multiboson Background"
};

const Color_t kSignalColor = TColor::GetColor("#CC0000");     // red
const Color_t kBackgroundColor = TColor::GetColor("#6699CC"); // light blue
const Color_t kFourLepColor = TColor::GetColor("#339966");    // green, Plot 4 second series
const Color_t kSqrtBColor = TColor::GetColor("#CCCC00");      // dark yellow, Plot 2

// Sums getHist() over all five background processes for a given channel/variable.
// VERIFY: the bool arg to getHist was inferred from call-site usage, never confirmed
// from its declaration. If a histogram comes back empty/wrong, check this first.
TH1 *SumBackground(std::shared_ptr<FullAnalysis> analysis, const HistVariable &histvariable,
                    const std::string &channelName, const char *cloneName)
{
    TH1 *backgroundHist = nullptr;
    for (const auto &name : kBackgroundProcesses)
    {
        TH1 *h = analysis->getHist(histvariable, name, true, channelName);
        if (!h)
        {
            std::cout << "WARNING: getHist returned null for background process " << name
                      << " in channel " << channelName << std::endl;
            continue;
        }
        if (!backgroundHist)
        {
            backgroundHist = (TH1 *)h->Clone(cloneName);
        }
        else
        {
            backgroundHist->Add(h);
        }
    }
    return backgroundHist;
}

// Matches WindowAnalysis.C's cSensitivity/cRatio canvas setup exactly.
void ApplyWindowMargins(TCanvas *canvas)
{
    canvas->SetLeftMargin(0.12);
    canvas->SetRightMargin(0.05);
    canvas->SetBottomMargin(0.12);
    canvas->SetTopMargin(0.16);
    canvas->SetGridx(true);
    canvas->SetGridy(true);
}

// House-style pseudo-legend: "CMSAnalysis Internal" header line, then one
// color-matched TLatex subtitle per series, in place of a TLegend box.
// Header/subtitle/series labels are stacked in a uniform 0.035 NDC vertical
// grid, all left-aligned at x=0.13, so nothing shares a row.
void DrawInternalLabel(const std::string &subtitle = "")
{
    TLatex label;
    label.SetNDC(true);
    label.SetTextSize(0.035);
    label.DrawLatex(0.13, 0.92, "CMSAnalysis Internal");
    if (!subtitle.empty())
    {
        label.SetTextSize(0.028);
        label.DrawLatex(0.13, 0.885, subtitle.c_str());
    }
}

void DrawSeriesLabel(double x, double y, Color_t color, const std::string &text, double size = 0.028)
{
    TLatex label;
    label.SetNDC(true);
    label.SetTextSize(size);
    label.SetTextColor(color);
    label.DrawLatex(x, y, text.c_str());
}

// Builds a y-axis title that states the actual bin width - a histogram bin's
// numeric value has no meaning without it (10 "events" in a 5 GeV bin and
// 10 "events" in a 50 GeV bin are not the same density). Reads the width
// straight off the histogram instead of assuming a number. Falls back to a
// second histogram if the first is null, and drops the unit entirely rather
// than guess if neither is available.
std::string BinWidthAxisTitle(const std::string &baseLabel, TH1 *primary, TH1 *fallback = nullptr)
{
    TH1 *h = primary ? primary : fallback;
    if (!h || h->GetNbinsX() < 1)
    {
        return baseLabel;
    }
    double width = h->GetXaxis()->GetBinWidth(1);
    char buf[96];
    if (std::abs(width - std::round(width)) < 1e-6)
    {
        snprintf(buf, sizeof(buf), "%s / %d GeV", baseLabel.c_str(), (int)std::round(width));
    }
    else
    {
        snprintf(buf, sizeof(buf), "%s / %.1f GeV", baseLabel.c_str(), width);
    }
    return std::string(buf);
}

// Centers the x- and y-axis titles on their respective axes and gives them
// a bit of extra offset so they clear the tick labels.
void CenterAxisTitles(TH1 *hist)
{
    if (!hist)
    {
        return;
    }
    hist->GetXaxis()->CenterTitle(true);
    hist->GetYaxis()->CenterTitle(true);
    hist->GetXaxis()->SetTitleOffset(1.2);
    hist->GetYaxis()->SetTitleOffset(1.3);
}

void CenterAxisTitles(TGraph *graph)
{
    if (!graph)
    {
        return;
    }
    graph->GetXaxis()->CenterTitle(true);
    graph->GetYaxis()->CenterTitle(true);
    graph->GetXaxis()->SetTitleOffset(1.2);
    graph->GetYaxis()->SetTitleOffset(1.3);
}

// Applies centered titles to every histogram sitting on the current pad
// (used for Plots 1/3/4, where the canvas comes back from
// simpleSuperImposedHist and we don't have a direct handle on the drawn
// TH1s at the call site).
void CenterAxisTitlesOnPad(TVirtualPad *pad)
{
    if (!pad)
    {
        return;
    }
    TIter next(pad->GetListOfPrimitives());
    TObject *obj = nullptr;
    while ((obj = next()))
    {
        if (obj->InheritsFrom(TH1::Class()))
        {
            CenterAxisTitles((TH1 *)obj);
        }
    }
}

// Removes the ACTUAL legend box entirely — the real TLegend that
// simpleSuperImposedHist() builds internally (top-right corner, color swatch
// + text per entry). Shrinking it still left it sitting on top of the peak,
// so instead of resizing it, this pulls it off the pad completely. The
// DrawInternalLabel/DrawSeriesLabel TLatex text (top-left) already identifies
// which color is which, so nothing is lost.
void RemoveLegend(TVirtualPad *pad)
{
    if (!pad)
    {
        return;
    }
    TIter next(pad->GetListOfPrimitives());
    TObject *obj = nullptr;
    std::vector<TObject *> toRemove;
    while ((obj = next()))
    {
        if (obj->InheritsFrom(TLegend::Class()))
        {
            toRemove.push_back(obj);
        }
    }
    for (TObject *legend : toRemove)
    {
        pad->GetListOfPrimitives()->Remove(legend);
        delete legend;
    }
}

// Save PNG via SaveAs, PDF via Print(file, "pdf") — matches WindowAnalysis.C.
// Caller is responsible for canvas->Modified()/Update() before this runs.
void SaveCanvas(TCanvas *canvas, const std::string &baseName)
{
    gSystem->Unlink((baseName + ".png").c_str());
    gSystem->Unlink((baseName + ".pdf").c_str());
    canvas->SaveAs((baseName + ".png").c_str());
    canvas->Print((baseName + ".pdf").c_str(), "pdf");
}
} // namespace

// ----------------------------------------------------------------------------
// Plot 1: eee_ channel, signal vs. summed background, RecoSameSignInvariantMass
// ----------------------------------------------------------------------------
void Plot1_eee_SignalBackground(std::shared_ptr<FullAnalysis> analysis, PlotFormatter &formatter)
{
    std::string channelName = "eee_";
    HistVariable histvariable(HistVariable::VariableType::RecoSameSignInvariantMass, "GeV");

    TH1 *signalHist = analysis->getHist(histvariable, kSignalProcess, true, channelName);
    TH1 *backgroundHist = SumBackground(analysis, histvariable, channelName, "backgroundHist_eee_");

    std::cout << "[Plot 1: eee_] signal entries=" << (signalHist ? signalHist->GetEntries() : -1)
              << " integral=" << (signalHist ? signalHist->Integral() : -1) << std::endl;
    std::cout << "[Plot 1: eee_] background entries=" << (backgroundHist ? backgroundHist->GetEntries() : -1)
              << " integral=" << (backgroundHist ? backgroundHist->Integral() : -1) << std::endl;
    std::cout << "[Plot 1: eee_] reference (confirmed reco yields, numFiles=1): signal=1008 background=13 -- "
              << "raw reco counts, NOT the same quantity as the histogram below (see next line)." << std::endl;
    std::cout << "[Plot 1: eee_] signal max bin=" << (signalHist ? signalHist->GetMaximum() : -1)
              << " background max bin=" << (backgroundHist ? backgroundHist->GetMaximum() : -1)
              << " -- confirmed (Plot 3's run) these are luminosity/cross-section-weighted expected "
              << "yields from getHist(), not raw counts, which is why they're fractions of an event." << std::endl;

    if (signalHist)
    {
        signalHist->GetXaxis()->SetRangeUser(signalHist->GetXaxis()->GetXmin(), 1200);
    }
    if (backgroundHist)
    {
        backgroundHist->GetXaxis()->SetRangeUser(backgroundHist->GetXaxis()->GetXmin(), 1200);
    }

    std::vector<TH1 *> hists = {signalHist, backgroundHist};
    std::vector<Color_t> colors = {kSignalColor, kBackgroundColor};
    std::vector<TString> names = {"Higgs Signal", "Background"};

    std::string yAxisTitle = BinWidthAxisTitle("Expected Events", signalHist, backgroundHist);
    TCanvas *canvas = formatter.simpleSuperImposedHist(hists, colors, names,
                                                         "Reconstructed Invariant Mass [GeV]", yAxisTitle.c_str());
    canvas->cd();
    canvas->SetGridx(true);
    canvas->SetGridy(true);
    CenterAxisTitlesOnPad(canvas);
    RemoveLegend(canvas);

    DrawInternalLabel("Three-Electron Channel, 137.9 fb^{-1}");
    DrawSeriesLabel(0.52, 0.885, kSignalColor, "Red: Signal");
    DrawSeriesLabel(0.71, 0.885, kBackgroundColor, "Blue: Background");

    canvas->Modified();
    canvas->Update();
    SaveCanvas(canvas, "Plot1_eee_SignalBackground");
}

// ----------------------------------------------------------------------------
// Plot 2: s/#sqrt{b} vs. tauThreeChannelCut, eet_ channel, single curve
// Source: 07/15/26 tuning notes, numFiles=5 held constant across all five cuts
// ----------------------------------------------------------------------------
void Plot2_SoverSqrtB()
{
    const int nPoints = 5;
    double cuts[nPoints] = {150, 200, 250, 300, 350};
    double sOverSqrtB[nPoints] = {105.3, 155.9, 209.3, 251.1, 228.1};

    std::cout << "[Plot 2: s/sqrt(b)] using fixed values from 07/15/26 notes (numFiles=5, eet_ channel):" << std::endl;
    for (int i = 0; i < nPoints; ++i)
    {
        std::cout << "  cut=" << cuts[i] << "  s/sqrt(b)=" << sOverSqrtB[i] << std::endl;
    }
    std::cout << "[Plot 2] Note: 300/350 both had background=3 events at numFiles=5, so the 350 dip is "
              << "statistical noise, not a real reversal." << std::endl;

    TCanvas *canvas = new TCanvas("cSensitivity", "Cut Sensitivity", 1200, 800);
    ApplyWindowMargins(canvas);

    TGraph *graph = new TGraph(nPoints, cuts, sOverSqrtB);
    graph->SetTitle("Cut Sensitivity;Tau Selection Cut [GeV];Signal Significance (S / #sqrt{B})");
    graph->SetLineColor(kSqrtBColor);
    graph->SetMarkerColor(kSqrtBColor);
    graph->SetMarkerStyle(20);
    graph->SetMarkerSize(1.2);
    graph->SetLineWidth(3);
    graph->SetMinimum(0.0);
    graph->SetMaximum(*std::max_element(sOverSqrtB, sOverSqrtB + nPoints) * 1.35);
    graph->Draw("ALP");
    CenterAxisTitles(graph);

    DrawInternalLabel("Two-Electron, One-Tau Channel (eet_)");
    DrawSeriesLabel(0.13, 0.85, kSqrtBColor, "Dark yellow: Signal Significance per Cut");

    canvas->Modified();
    canvas->Update();
    SaveCanvas(canvas, "Plot2_SoverSqrtB");

    delete graph;
    delete canvas;
}

// ----------------------------------------------------------------------------
// Plot 3: ete_ channel, signal vs. summed background, RecoSameSignInvariantMass
//
// Originally requested as "eet_"; rootls confirmed no such directory exists
// in the ROOT files. Using "ete_" instead: same lepton content (2 electrons
// + 1 tau), only the ordering differs, and it's a confirmed-real directory
// with its own distinct nonzero yield. This is a different physics channel
// than eet_ — every label below says ete_ explicitly.
// ----------------------------------------------------------------------------
void Plot3_ete_SignalBackground(std::shared_ptr<FullAnalysis> analysis, PlotFormatter &formatter)
{
    std::string channelName = "ete_";
    HistVariable histvariable(HistVariable::VariableType::RecoSameSignInvariantMass, "GeV");

    TH1 *signalHist = analysis->getHist(histvariable, kSignalProcess, true, channelName);
    TH1 *backgroundHist = SumBackground(analysis, histvariable, channelName, "backgroundHist_ete_");

    std::cout << "[Plot 3: ete_] signal entries=" << (signalHist ? signalHist->GetEntries() : -1)
              << " integral=" << (signalHist ? signalHist->Integral() : -1) << std::endl;
    std::cout << "[Plot 3: ete_] background entries=" << (backgroundHist ? backgroundHist->GetEntries() : -1)
              << " integral=" << (backgroundHist ? backgroundHist->Integral() : -1) << std::endl;
    std::cout << "[Plot 3: ete_] reference (confirmed reco yields, numFiles=1): signal=945 background=17 -- "
              << "raw reco counts, NOT the same quantity as the histogram below (see next line)." << std::endl;
    std::cout << "[Plot 3: ete_] signal max bin=" << (signalHist ? signalHist->GetMaximum() : -1)
              << " background max bin=" << (backgroundHist ? backgroundHist->GetMaximum() : -1)
              << " -- confirmed: these are luminosity/cross-section-weighted expected yields from "
              << "getHist(), not raw counts, which is why they're fractions of an event." << std::endl;

    // Guard kept as a safety net - channelName resolves to a real directory,
    // so this isn't expected to trigger, but it's cheaper to keep than strip.
    bool signalEmpty = !signalHist || signalHist->GetEntries() == 0;
    bool backgroundEmpty = !backgroundHist || backgroundHist->GetEntries() == 0;
    if (signalEmpty && backgroundEmpty)
    {
        std::cout << "[Plot 3: ete_] SKIPPED - both signal and background histograms are empty." << std::endl;
        return;
    }

    if (signalHist)
    {
        signalHist->GetXaxis()->SetRangeUser(signalHist->GetXaxis()->GetXmin(), 1200);
    }
    if (backgroundHist)
    {
        backgroundHist->GetXaxis()->SetRangeUser(backgroundHist->GetXaxis()->GetXmin(), 1200);
    }

    std::vector<TH1 *> hists = {signalHist, backgroundHist};
    std::vector<Color_t> colors = {kSignalColor, kBackgroundColor};
    std::vector<TString> names = {"Higgs Signal", "Background"};

    std::string yAxisTitle = BinWidthAxisTitle("Expected Events", signalHist, backgroundHist);
    TCanvas *canvas = formatter.simpleSuperImposedHist(hists, colors, names,
                                                         "Reconstructed Invariant Mass [GeV]", yAxisTitle.c_str());
    canvas->cd();
    canvas->SetGridx(true);
    canvas->SetGridy(true);
    CenterAxisTitlesOnPad(canvas);
    RemoveLegend(canvas);

    DrawInternalLabel("Two-Electron, One-Tau Channel, Alternate Ordering (ete_), 137.9 fb^{-1}");
    DrawSeriesLabel(0.13, 0.85, kSignalColor, "Red: Higgs Signal");
    DrawSeriesLabel(0.32, 0.85, kBackgroundColor, "Blue: Background");

    canvas->Modified();
    canvas->Update();
    SaveCanvas(canvas, "Plot3_ete_SignalBackground");
}

// ----------------------------------------------------------------------------
// Plot 4: 3-lepton (eee_) vs 4-lepton (eeee) shape comparison, normalized to
// unit area (raw counts would only show sample size, not shape).
// ----------------------------------------------------------------------------
void Plot4_3vs4LeptonShape(std::shared_ptr<FullAnalysis> analysis, PlotFormatter &formatter)
{
    HistVariable histvariable(HistVariable::VariableType::RecoSameSignInvariantMass, "GeV");

    TH1 *hist3Lep = analysis->getHist(histvariable, kSignalProcess, true, "eee_");
    TH1 *hist4Lep = analysis->getHist(histvariable, kSignalProcess, true, "eeee");

    std::cout << "[Plot 4] 3-lepton (eee_) entries=" << (hist3Lep ? hist3Lep->GetEntries() : -1)
              << " integral(pre-norm)=" << (hist3Lep ? hist3Lep->Integral() : -1) << std::endl;
    std::cout << "[Plot 4] 4-lepton (eeee) entries=" << (hist4Lep ? hist4Lep->GetEntries() : -1)
              << " integral(pre-norm)=" << (hist4Lep ? hist4Lep->Integral() : -1) << std::endl;

    if (hist3Lep && hist3Lep->Integral() > 0)
    {
        hist3Lep = (TH1 *)hist3Lep->Clone("hist3Lep_norm");
        hist3Lep->Scale(1.0 / hist3Lep->Integral());
    }
    if (hist4Lep && hist4Lep->Integral() > 0)
    {
        hist4Lep = (TH1 *)hist4Lep->Clone("hist4Lep_norm");
        hist4Lep->Scale(1.0 / hist4Lep->Integral());
    }

    std::vector<TH1 *> hists = {hist3Lep, hist4Lep};
    std::vector<Color_t> colors = {kSignalColor, kFourLepColor};
    std::vector<TString> names = {"Three-Lepton Events", "Four-Lepton Events"};

    std::string yAxisTitle = BinWidthAxisTitle("Fraction of Events", hist3Lep, hist4Lep);
    TCanvas *canvas = formatter.simpleSuperImposedHist(hists, colors, names,
                                                         "Reconstructed Invariant Mass [GeV]", yAxisTitle.c_str());
    canvas->cd();
    canvas->SetGridx(true);
    canvas->SetGridy(true);
    CenterAxisTitlesOnPad(canvas);
    RemoveLegend(canvas);

    DrawInternalLabel("Reconstructed Mass Shape: Three-Lepton vs. Four-Lepton Events");
    DrawSeriesLabel(0.13, 0.85, kSignalColor, "Red: Three-Lepton (eee)");
    DrawSeriesLabel(0.55, 0.85, kFourLepColor, "Green: Four-Lepton (eeee)");

    canvas->Modified();
    canvas->Update();
    SaveCanvas(canvas, "Plot4_3vs4LeptonShape");
}

// ----------------------------------------------------------------------------
// Plot 5: 3-lepton channel yields, signal vs. background, 9 confirmed channels
// Source: live runAnalyzer output, both sides confirmed nonzero, no inference.
// Built manually (not through getHist/completePlot) so full WindowAnalysis
// margins apply here.
//
// This still includes "eet_" as one of the 9 labels, unlike Plot 3, because
// these counts came straight from the runAnalyzer rReco counter in the log,
// not from a ROOT histogram directory. The directory-naming gap that forced
// the swap in Plot 3 only affects getHist()/simpleSuperImposedHist, not
// these hardcoded counter values.
// ----------------------------------------------------------------------------
void Plot5_ChannelYields()
{
    const int nChannels = 9;
    const char *channelLabels[nChannels] = {
        "eee", "eet", "ete", "ett", "eue", "eut", "euu", "utu", "uuu"
    };
    double signalCounts[nChannels]     = {1008, 362, 945, 329, 2017, 693, 2133, 906, 1042};
    double backgroundCounts[nChannels] = {13,   1,   17,  2,   25,   1,   25,   9,   19};

    std::cout << "[Plot 5] channel yields (confirmed, numFiles=1):" << std::endl;
    for (int i = 0; i < nChannels; ++i)
    {
        std::cout << "  " << channelLabels[i] << "  signal=" << signalCounts[i]
                  << "  background=" << backgroundCounts[i] << std::endl;
    }
    std::cout << "[Plot 5] Note: 8 other 3-lepton channels (eeu_, tte_, ttt_, ttu_, ute_, utt_, uue_, uut_) "
              << "showed zero background reco and are excluded here rather than plotted as an inferred zero."
              << std::endl;

    TH1D *signalHist = new TH1D("signalYield", "", nChannels, 0, nChannels);
    TH1D *backgroundHist = new TH1D("backgroundYield", "", nChannels, 0, nChannels);
    for (int i = 0; i < nChannels; ++i)
    {
        signalHist->SetBinContent(i + 1, signalCounts[i]);
        backgroundHist->SetBinContent(i + 1, backgroundCounts[i]);
        signalHist->GetXaxis()->SetBinLabel(i + 1, channelLabels[i]);
        backgroundHist->GetXaxis()->SetBinLabel(i + 1, channelLabels[i]);
    }

    TCanvas *canvas = new TCanvas("cChannelYields", "3-Lepton Channel Yields", 1200, 800);
    ApplyWindowMargins(canvas);
    canvas->SetLogy();

    signalHist->SetTitle(";Lepton Flavor Channel;Reconstructed Events (unweighted)");
    signalHist->SetLineColor(kSignalColor);
    signalHist->SetLineWidth(3);
    signalHist->SetFillColorAlpha(kSignalColor, 0.15);
    signalHist->SetMaximum(signalHist->GetMaximum() * 5);
    signalHist->SetMinimum(1e-1);
    signalHist->Draw("HIST");
    CenterAxisTitles(signalHist);

    backgroundHist->SetLineColor(kBackgroundColor);
    backgroundHist->SetLineWidth(3);
    backgroundHist->SetFillColorAlpha(kBackgroundColor, 0.15);
    backgroundHist->Draw("HIST SAME");

    DrawInternalLabel("Signal and Background Yields by Lepton Flavor Channel");
    DrawSeriesLabel(0.13, 0.85, kSignalColor, "Red: Higgs Signal");
    DrawSeriesLabel(0.50, 0.85, kBackgroundColor, "Blue: Background");

    canvas->Modified();
    canvas->Update();
    SaveCanvas(canvas, "Plot5_ChannelYields");

    delete signalHist;
    delete backgroundHist;
    delete canvas;
}

// ----------------------------------------------------------------------------
// Entry point — filename must match this function name for `root -l AllPlots.C`
// ----------------------------------------------------------------------------
void AllPlots()
{
    gStyle->SetOptStat(0);

    auto analysis = std::make_shared<HiggsCompleteAnalysis>();
    PlotFormatter formatter(true, "");

    Plot1_eee_SignalBackground(analysis, formatter);
    Plot2_SoverSqrtB();
    Plot3_ete_SignalBackground(analysis, formatter);
    Plot4_3vs4LeptonShape(analysis, formatter);
    Plot5_ChannelYields();

    std::cout << "\nAll five plots saved (PNG + PDF). Check the entries/integral lines above "
              << "against the reference numbers before trusting the images.\n" << std::endl;
}
#include "CMSAnalysis/Analysis/interface/HiggsCompleteAnalysis.hh"
#include "CMSAnalysis/Analysis/interface/Channel.hh"
#include "CMSAnalysis/Analysis/interface/CrossSectionReader.hh"
#include "CMSAnalysis/Analysis/interface/Estimator.hh"
#include "CMSAnalysis/Analysis/interface/FitEstimator.hh"
#include "CMSAnalysis/Analysis/interface/HiggsHistNameFinder.hh"
#include "CMSAnalysis/Analysis/interface/HistVariable.hh"
#include "CMSAnalysis/Analysis/interface/Process.hh"
#include "CMSAnalysis/Analysis/interface/SingleProcess.hh"
#include "CMSAnalysis/Analysis/interface/WindowEstimator.hh"
#include "CMSAnalysis/Utility/interface/ParticleType.hh"
#include "TH1.h"
#include <memory>
#include <string>
#include <vector>

const std::vector<std::string> HiggsCompleteAnalysis::genSimDecays{
    "eeee", "eeeu", "eeet", "eeuu", "eeut", "eett", "eueu", "euet", "euuu", "euut", "eutt",
    "etet", "etuu", "etut", "ettt", "uuuu", "uuut", "uutt", "utut", "uttt", "tttt"};

const std::vector<std::string> HiggsCompleteAnalysis::recoDecays{
    "eeee", "eeeu", "eeet", "eeuu", "eeut", "eett", "eueu", "euet", "euuu", "euut", "eutt", "etet",
    "etuu", "etut", "ettt", "uuuu", "uuut", "uutt", "utut", "uttt", "tttt", "eee_", "eeu_", "eue_",
    "euu_", "uue_", "uuu_", "eet_", "ete_", "eut_", "etu_", "uut_", "utu_", "ett_", "utt_", "ttt_",
    "ee__", "e_e_", "eu__", "e_u_", "uu__", "u_u_", "tt__", "t_t_", "et__", "e_t_", "ut__", "u_t_"};

const std::vector<std::string> systematics{"ElectronScaleFactor", "MuonIDISOScaleFactor", "MuonRecoScaleFactor",
                                           "MuonTriggerScaleFactor"};

// Actual masses for the Higgs signal
const std::vector<int> HiggsCompleteAnalysis::massTargets{500, 600, 700, 800, 900, 1000, 1100, 1200, 1300, 1400, 1500};

constexpr auto bgFilePath = "/uscms/home/bhenning/nobackup/HiggsReprocessing/";
constexpr auto signalFilePath = "/uscms/home/bhenning/nobackup/HiggsReprocessing/";
constexpr auto dataFilePath = "/uscms/home/bhenning/nobackup/030426HiggsData/";

double HiggsCompleteAnalysis::getBranchingRatio(const std::string &channel) const
{
    std::unordered_map<std::string, double> originalRatios = {{"ee", 3.0 / 2}, {"eu", 3.0 / 4}, {"uu", 3.0 / 2},
                                                              {"et", 3.0 / 4}, {"ut", 3.0 / 4}, {"tt", 3.0 / 2}};

    std::string firstPair = channel.substr(0, 2);
    std::string secondPair = channel.substr(2, 2);

    return originalRatios[firstPair] * originalRatios[secondPair];
}

std::vector<std::string> HiggsCompleteAnalysis::getSystematics() const
{
    return systematics;
}

HiggsCompleteAnalysis::HiggsCompleteAnalysis()
{
    const int higgsColor = kOrange + 10;
    const int ttbarColor = kBlue;
    const int drellYanBackColor = kPink + 10;
    const int QCDBackColor = kCyan + 3;
    const int ZZBackgroundColor = kGreen + 10;
    const int WJetsBackgroundColor = kViolet;

    // Change this file to your folder to use your own cross sections
    // filePath is shared between most files. The rest of the filePath to a given file is still given when making

    auto reader = std::make_shared<CrossSectionReader>(
        "/uscms/homes/s/sdulam/analysis/CMSSW_14_0_4/src/CMSAnalysis/DataCollection/bin/crossSections.txt");

    const double luminosity = 137.94;

    TH1::SetDefaultSumw2();
    for (bool zSelection : {true, false})
    {
        for (const std::string &recoDecay : recoDecays)
        {
            std::vector<std::shared_ptr<Process>> processes;

            for (const double massTarget : massTargets)
            {
                auto higgsMassGroup = std::make_shared<Process>("Higgs Signal " + std::to_string((int)massTarget), 1);

                for (const auto &genSimDecay : genSimDecays)
                {
                    auto histMapperLowMass = std::make_shared<HiggsHistNameFinder>(recoDecay, genSimDecay, zSelection, true);
                    auto histMapperHighMass = std::make_shared<HiggsHistNameFinder>(recoDecay, genSimDecay, zSelection, false);

                    double branchingRatioFixer = getBranchingRatio(genSimDecay);

                    auto higgsSignal =
                        std::make_shared<Process>("Higgs signal " + genSimDecay + " " + std::to_string((int)massTarget), 1);
                    addSingleProcess(higgsSignal, signalFilePath, "Higgs" + std::to_string((int)massTarget) + ".root",
                                     "higgs4l" + std::to_string((int)massTarget), reader, luminosity,
                                     histMapperLowMass, histMapperHighMass, false, branchingRatioFixer);
                    processes.push_back(higgsSignal);
                    addSingleProcess(higgsMassGroup, signalFilePath, "Higgs" + std::to_string((int)massTarget) + ".root",
                                     "higgs4l" + std::to_string((int)massTarget), reader, luminosity,
                                     histMapperLowMass, histMapperHighMass, false, branchingRatioFixer);
                }
                processes.push_back(higgsMassGroup);
            }


            auto histMapperLowMass = std::make_shared<HiggsHistNameFinder>(recoDecay, "", zSelection, true);
            auto histMapperHighMass = std::make_shared<HiggsHistNameFinder>(recoDecay, "", zSelection, false);

            auto zzBackground = std::make_shared<Process>("ZZ Background", ZZBackgroundColor);
            addSingleProcess(zzBackground, bgFilePath, "ZZ.root", "zzto4l", reader, luminosity,
                histMapperLowMass, histMapperHighMass, false, 1);

            auto wJetsBackground = std::make_shared<Process>("WJets Background", WJetsBackgroundColor);
            addSingleProcess(wJetsBackground, bgFilePath, "WJets.root", "wjets", reader, luminosity,
                histMapperLowMass, histMapperHighMass, false, 1);

            // cross sections should be all lowercase
            auto ttBarandMultiBosonBackground = std::make_shared<Process>("t#bar{t}, Multiboson Background", ttbarColor);
            addSingleProcess(ttBarandMultiBosonBackground, bgFilePath, "TTbar.root", "ttbar_lep", reader, luminosity,
                histMapperLowMass, histMapperHighMass, false, 1);
            addSingleProcess(ttBarandMultiBosonBackground, bgFilePath, "TTW.root", "ttw", reader, luminosity,
                histMapperLowMass, histMapperHighMass, false, 1);
            addSingleProcess(ttBarandMultiBosonBackground, bgFilePath, "TTZ.root", "ttz", reader, luminosity,
                histMapperLowMass, histMapperHighMass, false, 1);

            // auto other = std::make_shared<Process>("Other Background", ttbarColor);
            addSingleProcess(ttBarandMultiBosonBackground, bgFilePath, "TTW.root", "ttw", reader, luminosity,
                histMapperLowMass, histMapperHighMass, false, 1);
            addSingleProcess(ttBarandMultiBosonBackground, bgFilePath, "ZZZ.root", "zzz", reader, luminosity,
                histMapperLowMass, histMapperHighMass, false, 1);
            addSingleProcess(ttBarandMultiBosonBackground, bgFilePath, "WW.root", "wwto2l2nu", reader, luminosity,
                histMapperLowMass, histMapperHighMass, false, 1);
            addSingleProcess(ttBarandMultiBosonBackground, bgFilePath, "WWW.root", "www", reader, luminosity,
                histMapperLowMass, histMapperHighMass, false, 1);
            addSingleProcess(ttBarandMultiBosonBackground, bgFilePath, "WWZ.root", "wwz", reader, luminosity,
                histMapperLowMass, histMapperHighMass, false, 1);
            addSingleProcess(ttBarandMultiBosonBackground, bgFilePath, "WZ.root", "wzto3lnu", reader, luminosity,
                histMapperLowMass, histMapperHighMass, false, 1);
            addSingleProcess(ttBarandMultiBosonBackground, bgFilePath, "WZZ.root", "wzz", reader, luminosity,   
                histMapperLowMass, histMapperHighMass, false, 1);

            auto dyBackground = std::make_shared<Process>("Drell-Yan Background", drellYanBackColor);
            addSingleProcess(dyBackground, bgFilePath, "DY10-50.root", "dy10to50", reader, luminosity,
                histMapperLowMass, histMapperHighMass, false, 1);
            addSingleProcess(dyBackground, bgFilePath, "DY50-inf.root", "dy50toInf", reader, luminosity,
                histMapperLowMass, histMapperHighMass, false, 1);        


            auto qcdBackground = std::make_shared<Process>("QCD Background", QCDBackColor);
            addSingleProcess(qcdBackground, bgFilePath, "QCD100-200.root", "QCD_100-200", reader, luminosity,
                histMapperLowMass, histMapperHighMass, false, 1);
            addSingleProcess(qcdBackground, bgFilePath, "QCD200-300.root", "QCD_200-300", reader, luminosity,
                histMapperLowMass, histMapperHighMass, false, 1);
            addSingleProcess(qcdBackground, bgFilePath, "QCD300-500.root", "QCD_300-500", reader, luminosity,
                histMapperLowMass, histMapperHighMass, false, 1);
            addSingleProcess(qcdBackground, bgFilePath, "QCD500-700.root", "QCD_500-700", reader, luminosity,
                histMapperLowMass, histMapperHighMass, false, 1);
            addSingleProcess(qcdBackground, bgFilePath, "QCD700-1000.root", "QCD_700-1000", reader, luminosity,
                histMapperLowMass, histMapperHighMass, false, 1);
            addSingleProcess(qcdBackground, bgFilePath, "QCD1000-1500.root", "QCD_1000-1500", reader, luminosity,
                histMapperLowMass, histMapperHighMass, false, 1);
            addSingleProcess(qcdBackground, bgFilePath, "QCD1500-2000.root", "QCD_1500-2000", reader, luminosity,
                histMapperLowMass, histMapperHighMass, false, 1);
            addSingleProcess(qcdBackground, bgFilePath, "QCD2000-inf.root", "QCD_2000-inf", reader, luminosity,
                histMapperLowMass, histMapperHighMass, false, 1);

            auto higgsData = std::make_shared<Process>("Data", higgsColor);
            addSingleProcess(higgsData, dataFilePath, "Electron2016.root", "Electron2016", reader, luminosity,
                histMapperLowMass, histMapperHighMass, true, 1);
            addSingleProcess(higgsData, dataFilePath, "Electron2016APV.root", "Electron2016APV", reader, luminosity,
                histMapperLowMass, histMapperHighMass, true, 1);
            addSingleProcess(higgsData, dataFilePath, "Electron2017.root", "Electron2017", reader, luminosity,
                histMapperLowMass, histMapperHighMass, true, 1);
            addSingleProcess(higgsData, dataFilePath, "Electron2018.root", "Electron2018", reader, luminosity,
                histMapperLowMass, histMapperHighMass, true, 1);
            addSingleProcess(higgsData, dataFilePath, "Muon2016.root", "Muon2016", reader, luminosity,
                histMapperLowMass, histMapperHighMass, true, 1);
            addSingleProcess(higgsData, dataFilePath, "Muon2016APV.root", "Muon2016APV", reader, luminosity,
                histMapperLowMass, histMapperHighMass, true, 1);
            addSingleProcess(higgsData, dataFilePath, "Muon2017.root", "Muon2017", reader, luminosity,
                histMapperLowMass, histMapperHighMass, true, 1);
            addSingleProcess(higgsData, dataFilePath, "Muon2018.root", "Muon2018", reader, luminosity,
                histMapperLowMass, histMapperHighMass, true, 1);

            processes.push_back(dyBackground);
            processes.push_back(higgsData);
            processes.push_back(qcdBackground);
            processes.push_back(zzBackground);
            processes.push_back(ttBarandMultiBosonBackground);
            processes.push_back(wJetsBackground);

            // for(std::shared_ptr<Process> process : processes)
            // {
            //     for(std::string systematic : systematics)
            //     {
            //         auto sys = process->calcSystematic(HistVariable(HistVariable::VariableType::SameSignInvariantMass),
            //             systematic, true); 
            //         process->addSystematic(sys);
            //     }
            // }

            std::string modeLabel = zSelection ? "_ZPeak" : "";

            auto leptonProcesses = std::make_shared<Channel>(recoDecay + modeLabel, processes);

            for (std::string processName : leptonProcesses->getNames())
            {
                if (processName == "Higgs Signal 1000")
                //if (processName == "Higgs signal eueu 1000")
                {
                    leptonProcesses->labelProcess(Channel::Label::Signal, processName);
                }
                else if (processName.starts_with("Group") || processName.starts_with("Higgs"))
                {
                    // leptonProcesses->labelProcess("signal", processName);
                    //  std::cout << "Labeled Signal: " << processName << std::endl;
                }
                // "Monte Carlo Data"
                else if (processName == "Data")
                { // This line is only used for complete plots
                    leptonProcesses->labelProcess(Channel::Label::Data, processName);
                    // std::cout << "Labeled Data: " << processName << std::endl;
                }
                else
                {
                    leptonProcesses->labelProcess(Channel::Label::Background, processName);
                    // std::cout << "Labeled Background: " << processName << std::endl;
                }
            }
            getChannelsProtected().push_back(leptonProcesses);
        }
    }
}

void HiggsCompleteAnalysis::addSingleProcess(std::shared_ptr<Process> process, std::string filePathway, std::string fileName, 
    std::string crossSectionName, std::shared_ptr<CrossSectionReader> crossReader, double luminosity,
    std::shared_ptr<HistNameFinder> mappingLowMass, std::shared_ptr<HistNameFinder> mappingHighMass, bool isData,
    double branchingRatioAdjustment)
{
    process->addProcess(makeBasicProcess(filePathway, fileName, crossSectionName, crossReader, luminosity,
                                        mappingLowMass, isData, branchingRatioAdjustment));
    process->addProcess(makeBasicProcess(filePathway, fileName, crossSectionName, crossReader, luminosity,
                                        mappingHighMass, isData, branchingRatioAdjustment));                                        
}
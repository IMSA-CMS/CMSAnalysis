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
#include <iostream>
#include <memory>
#include <string>
#include <vector>

std::vector<std::string> HiggsCompleteAnalysis::genSimDecays{"eeee", "eeeu", "eeet", "eeuu", "eeut", "eett", "eueu",
                                            "euet", "euuu", "euut", "eutt", "etet", "etuu", "etut",
                                            "ettt", "uuuu", "uuut", "uutt", "utut", "uttt", "tttt"};

std::vector<std::string> HiggsCompleteAnalysis::recoDecays{"eeee", "eeeu", "eeuu", "eueu", "euuu", "uuuu", "eee", "eeu", "eue", "euu",
                                          "uue",  "uuu",  "ee",   "e e",  "eu",   "e u",  "uu",  "u u", "none"};


double HiggsCompleteAnalysis::getBranchingRatio(const std::string &channel)
{
    std::unordered_map<std::string, double> originalRatios = {{"ee", 3.0 / 2}, {"eu", 3.0 / 4}, {"uu", 3.0 / 2},
                                                              {"et", 3.0 / 4}, {"ut", 3.0 / 4}, {"tt", 3.0 / 2}};

    std::string firstPair = channel.substr(0, 2);
    std::string secondPair = channel.substr(2, 2);

    return originalRatios[firstPair] * originalRatios[secondPair];
}

HiggsCompleteAnalysis::HiggsCompleteAnalysis()
{
    // This is a mass we put into background because the
    // makeBasicProcess method requires it. It doesn't actually
    // make sense so in the future fix this
    //  double tempMass = 1400;

    const int higgsColor = kOrange + 10;
    const int ttbarColor = kBlue;
    const int drellYanBackColor = kPink + 10;
    const int QCDBackColor = kCyan + 3;
    const int ZZBackgroundColor = kGreen + 10;
    const int WJetsBackgroundColor = kViolet;

    // Actual masses for the Higgs signal
    std::vector<double> massTargets{500, 600, 700, 800, 900, 1000, 1100, 1200, 1300, 1400, 1500};
    // const std::vector<std::string> recoDecays{"u u", "uu", "uuu", "uuuu"};
    // const std::vector<std::string> recoDecays{"ee", "eu", "e u", "uu", "u u"};
    // const std::vector<std::string> recoDecays{"u u"};

    // Change this file to your folder to use your own cross sections
    // filePath is shared between most files. The rest of the filePath to a given file is still given when making
    // singleProcesses. auto reader =
    // std::make_shared<CrossSectionReader>("/uscms/homes/m/mchen2/analysis/CMSSW_14_0_4/src/CMSAnalysis/DataCollection/bin/crossSections.txt");
    auto reader = std::make_shared<CrossSectionReader>(
        "/uscms/homes/s/sdulam/analysis/CMSSW_14_0_4/src/CMSAnalysis/DataCollection/bin/crossSections.txt");
        //  const std::string filePath = "/uscms/home/bhenning/Analysis/CMSSW_14_0_1/src/CMSAnalysis/Output/HiggsFinal/";
    // const std::string signalFilePath = "/uscms/home/bhenning/Analysis/CMSSW_14_0_1/src/CMSAnalysis/Output/HiggsFinal/";

    const std::string filePath = "/uscms/homes/v/vyou/analysis/CMSSW_14_0_4/src/CMSAnalysis/Output/HiggsProjection/";
    const std::string signalFilePath =
        "/uscms/homes/v/vyou/analysis/CMSSW_14_0_4/src/CMSAnalysis/Output/HiggsProjection/";
    // const std::string myPath = "/uscms/homes/s/sdulam/analysis/CMSSW_14_0_4/src/CMSAnalysis/Output/Higgs/";
    // const std::string filePath = "/uscms/homes/m/mchen2/analysis/CMSSW_14_0_4/src/CMSAnalysis/Output/Higgs/";
    // const std::string signalFilePath = "/uscms/homes/m/mchen2/analysis/CMSSW_14_0_4/src/CMSAnalysis/Output/Higgs/";

    // Luminosity: 1.39
    double luminosity = 139;

    std::vector<HistVariable> histVariablesBackground;
    std::vector<HistVariable> histVariablesData;

    std::vector<std::string> rowNames = {"eeee", "eeeu", "eeuu", "eueu", "euuu", "uuuu", "eee", "eeu", "eue", "euu", "uue", "uuu", "ee", "e e", "eu", "e u", "uu", "u u"};
    ///std::vector<std::string> rowNames = {"ee", "eu", "uu", "e u", "u u", "e e"};
    std::vector<std::string> dataConnecters = {"_Pass_1st Highest mu- ", "_Pass_1st Highest e- "};
    std::vector<std::string> connecters = {""};

    std::vector<ParticleType> particleTypes = {ParticleType::electron(), ParticleType::muon(), ParticleType::tau()};
    std::vector<int> orders = {1, 2, 3, 4}; //whatever max order
    std::vector<HistVariable::VariableType> variableTypes = 
    {
    HistVariable::VariableType::Pt,
    HistVariable::VariableType::Eta,
    HistVariable::VariableType::Phi,
    HistVariable::VariableType::SameSignInvariantMass,
    HistVariable::VariableType::OppositeSignInvariantMass,
    HistVariable::VariableType::InvariantMass
    };

    for (ParticleType pType : particleTypes)
    {
        for (int order : orders)
        {
            for (auto varType : variableTypes)
            {
                histVariablesBackground.push_back(HistVariable(pType, order, varType));
                histVariablesData.push_back(HistVariable(pType, order, varType));
            }
        }
    }


    TH1::SetDefaultSumw2();
    for (const std::string &recoDecay : recoDecays)
    {


        std::vector<std::shared_ptr<Process>> processes;
        for (double massTarget : massTargets)
        {
            for (const auto &genSimDecay : genSimDecays)
            {
                std::string decayName = recoDecay + "_" + genSimDecay;
            // auto higgsSignal = std::make_shared<Process>("Higgs Signal", 5);

            // not really sure why we need this process at all
            // auto higgsGroupSignal = std::make_shared<Process>("Higgs Group " + recoDecay, 5);

                auto higgsMassGroup = std::make_shared<Process>("Higgs Signal " + std::to_string((int)massTarget), 1);

                auto histVariableToFileMapping = std::make_shared<HiggsHistNameFinder>(decayName);
                
                //histVariableToFileMapping["Same Sign Invariant Mass"] = decayName + "__hists/" + decayName + "_Reco Same Sign Invariant Mass";
                //histVariableToFileMapping["X Projection"] = decayName + "__hists/" + decayName + "_Reco Invariant Mass Background X Projection";
                //histVariableToFileMapping["Y Projection"] = decayName + "__hists/" + decayName + "_Reco Invariant Mass Background Y Projection";
                //std::vector<HistVariable> histVariablesSignal;

                // histVariablesSignal.push_back(HistVariable::sameSignMass(decayName + "__hists/" + decayName + "_Reco Same Sign Invariant Mass"));
                //histVariablesSignal.push_back(HistVariable(Selector::None, HistVariable::VariableType::SameSignInvariantMass));
                double branchingRatioFixer = getBranchingRatio(genSimDecay);
                // std::cout << "GENSIMDECAY: " << genSimDecay << std::endl;
                // std::cout << "BRANCHINGRATIOFIXER " << branchingRatioFixer << std::endl;
                auto higgsProcess = makeBasicProcess(signalFilePath, "Higgs" + std::to_string((int)massTarget) + ".root", "higgs4l" + std::to_string((int)massTarget), reader, luminosity, histVariableToFileMapping, false, branchingRatioFixer);
                auto higgsSignal = std::make_shared<Process>("Higgs signal " + decayName + " " + std::to_string((int)massTarget), 1);
                higgsSignal->addProcess(higgsProcess);
                processes.push_back(higgsSignal);
                higgsMassGroup->addProcess(higgsProcess);

                processes.push_back(higgsMassGroup);
            }
        }


        auto histVariableToFileMapping = std::make_shared<HiggsHistNameFinder>(recoDecay);

        

        auto zzBackground = std::make_shared<Process>("ZZ Background", ZZBackgroundColor);
        zzBackground->addProcess(makeBasicProcess(filePath, "ZZ.root", "zzto4l", reader, luminosity, histVariableToFileMapping));
        
        //auto zzBackgroundNoVeto = std::make_shared<Process>("ZZ Background No Veto", 3);
        //zzBackgroundNoVeto->addProcess(makeBasicProcess("/uscms/homes/v/vyou/analysis/CMSSW_14_0_4/src/CMSAnalysis/Output/NoZVeto/", "ZZ.root", "zzto4l", reader, luminosity, histVariableToFileMapping));

        auto wJetsBackground = std::make_shared<Process>("WJets Background", WJetsBackgroundColor);
        wJetsBackground->addProcess(makeBasicProcess(filePath, "WJets.root", "WJets", reader, luminosity, histVariableToFileMapping));

        // cross sections should be all lowercase
        auto ttBarandMultiBosonBackground = std::make_shared<Process>("t#bar{t}, Multiboson Background", ttbarColor);
        ttBarandMultiBosonBackground->addProcess(makeBasicProcess(filePath, "TTbar.root", "ttbar_lep", reader, luminosity, histVariableToFileMapping));
        ttBarandMultiBosonBackground->addProcess(makeBasicProcess(filePath, "TTW.root", "ttw", reader, luminosity, histVariableToFileMapping));
        ttBarandMultiBosonBackground->addProcess(makeBasicProcess(filePath, "TTZ.root", "ttz", reader, luminosity, histVariableToFileMapping));

        auto ttbarProcess = std::make_shared<Process>("TTbar Background", ttbarColor);
        ttbarProcess->addProcess(makeBasicProcess(filePath, "TTbar.root", "ttbar_lep", reader, luminosity, histVariableToFileMapping));
        
        auto ttzProcess = std::make_shared<Process>("TTZ Background", ttbarColor);
        ttzProcess->addProcess(makeBasicProcess(filePath, "TTZ.root", "ttz", reader, luminosity, histVariableToFileMapping));
        
        auto other = std::make_shared<Process>("Other Background", ttbarColor);
        other->addProcess(makeBasicProcess(filePath, "TTW.root", "ttw", reader, luminosity, histVariableToFileMapping));
        other->addProcess(makeBasicProcess(filePath, "ZZZ.root", "zzz", reader, luminosity, histVariableToFileMapping));
        other->addProcess(makeBasicProcess(filePath, "WW.root", "wwto2l2nu", reader, luminosity, histVariableToFileMapping));
        other->addProcess(makeBasicProcess(filePath, "WWW.root", "www", reader, luminosity, histVariableToFileMapping));
        other->addProcess(makeBasicProcess(filePath, "WWZ.root", "wwz", reader, luminosity, histVariableToFileMapping));
        other->addProcess(makeBasicProcess(filePath, "WZ.root", "wzto3lnu", reader, luminosity, histVariableToFileMapping));
        other->addProcess(makeBasicProcess(filePath, "WZZ.root", "wzz", reader, luminosity, histVariableToFileMapping));

        auto dyBackground = std::make_shared<Process>("Drell-Yan Background", drellYanBackColor);
        dyBackground->addProcess(makeBasicProcess(filePath, "DY10-50.root", "dy10to50", reader, luminosity, histVariableToFileMapping));
        dyBackground->addProcess(makeBasicProcess(filePath, "DY50-inf.root", "dy50toInf", reader, luminosity, histVariableToFileMapping));

        //auto dyBackgroundNoVeto = std::make_shared<Process>("Drell-Yan Background No Veto", 2);
        //dyBackgroundNoVeto->addProcess(makeBasicProcess("/uscms/homes/v/vyou/analysis/CMSSW_14_0_4/src/CMSAnalysis/Output/NoZVeto/", "DY10-50.root", "dy10to50", reader, luminosity, histVariableToFileMapping));
        //dyBackgroundNoVeto->addProcess(makeBasicProcess("/uscms/homes/v/vyou/analysis/CMSSW_14_0_4/src/CMSAnalysis/Output/NoZVeto/", "DY50-inf.root", "dy50toInf", reader, luminosity, histVariableToFileMapping));
        
        auto qcdBackground = std::make_shared<Process>("QCD Background", QCDBackColor);
        qcdBackground->addProcess(makeBasicProcess(filePath, "QCD100-200.root", "QCD_100-200", reader, luminosity, histVariableToFileMapping));
        qcdBackground->addProcess(makeBasicProcess(filePath, "QCD200-300.root", "QCD_200-300", reader, luminosity, histVariableToFileMapping));
        qcdBackground->addProcess(makeBasicProcess(filePath, "QCD300-500.root", "QCD_300-500", reader, luminosity, histVariableToFileMapping));
        qcdBackground->addProcess(makeBasicProcess(filePath, "QCD500-700.root", "QCD_500-700", reader, luminosity, histVariableToFileMapping));
        qcdBackground->addProcess(makeBasicProcess(filePath, "QCD700-1000.root", "QCD_700-1000", reader, luminosity, histVariableToFileMapping));
        qcdBackground->addProcess(makeBasicProcess(filePath, "QCD1000-1500.root", "QCD_1000-1500", reader, luminosity, histVariableToFileMapping));
        qcdBackground->addProcess(makeBasicProcess(filePath, "QCD1500-2000.root", "QCD_1500-2000", reader, luminosity, histVariableToFileMapping));
        // qcdBackground->addProcess(makeBasicProcess(filePath, "QCD2000-inf.root", "QCD_2000-inf", reader, luminosity, histVariableToFileMapping));

        auto higgsData = std::make_shared<Process>("Data", higgsColor);
        // 150022816 events in Data_Trigger_SingleMuon_Year_2016B.root before TriggerCut change
        higgsData->addProcess(makeBasicProcess(filePath, "Electron2016.root", "Electron2016", reader, luminosity, histVariableToFileMapping, true));
        higgsData->addProcess(makeBasicProcess(filePath, "Electron2016APV.root", "Electron2016APV", reader, luminosity, histVariableToFileMapping, true));
        higgsData->addProcess(makeBasicProcess(filePath, "Electron2017.root", "Electron2017", reader, luminosity, histVariableToFileMapping, true));
        higgsData->addProcess(makeBasicProcess(filePath, "Electron2018.root", "Electron2018", reader, luminosity, histVariableToFileMapping, true));
        higgsData->addProcess(makeBasicProcess(filePath, "Muon2016.root", "Muon2016", reader, luminosity, histVariableToFileMapping, true));
        higgsData->addProcess(makeBasicProcess(filePath, "Muon2016APV.root", "Muon2016APV", reader, luminosity, histVariableToFileMapping, true));
        higgsData->addProcess(makeBasicProcess(filePath, "Muon2017.root", "Muon2017", reader, luminosity, histVariableToFileMapping, true));
        higgsData->addProcess(makeBasicProcess(filePath, "Muon2018.root", "Muon2018", reader, luminosity, histVariableToFileMapping, true));

        
        processes.push_back(dyBackground);
        //processes.push_back(dyBackgroundNoVeto);
        processes.push_back(higgsData);
        processes.push_back(qcdBackground);
        processes.push_back(zzBackground);
        //processes.push_back(zzBackgroundNoVeto);
        // processes.push_back(ttBarandMultiBosonBackground);
        processes.push_back(ttbarProcess);
        processes.push_back(ttzProcess);
        
        processes.push_back(other);
        processes.push_back(wJetsBackground);


        auto leptonProcesses = std::make_shared<Channel>(recoDecay, processes);

        for (std::string processName : leptonProcesses->getNames())
        {
            // std::cout << processName << std::endl;
            // Change this line to make the described name your signal process name.
            if (processName == "Higgs Signal 1000")
            {
                leptonProcesses->labelProcess("signal", processName);
            }
            else if (processName.substr(0, 5) == "Group" || processName.substr(0, 5) == "Higgs")
            {
                //leptonProcesses->labelProcess("signal", processName);
                // std::cout << "Labeled Signal: " << processName << std::endl;
            }
            // "Monte Carlo Data"
            else if (processName == "Data")
            { // This line is only used for complete plots
                leptonProcesses->labelProcess("data", processName);
                // std::cout << "Labeled Data: " << processName << std::endl;
            }
            else
            {
                leptonProcesses->labelProcess("background", processName);
                // std::cout << "Labeled Background: " << processName << std::endl;
            }
        }
        // leptonBackgrounds->cleanProcesses();
        getChannelsProtected().push_back(leptonProcesses);
    }
}


#include "CMSAnalysis/Analysis/interface/HiggsKansasStateAnalysis.hh"
#include "CMSAnalysis/Analysis/interface/Channel.hh"
#include "CMSAnalysis/Analysis/interface/CrossSectionReader.hh"
#include "CMSAnalysis/Analysis/interface/FitFunction.hh"
#include "CMSAnalysis/Analysis/interface/FitFunctionCollection.hh"
#include "CMSAnalysis/Analysis/interface/HiggsHistNameFinder.hh"
#include "CMSAnalysis/Analysis/interface/HistVariable.hh"
#include "CMSAnalysis/Analysis/interface/Process.hh"
#include "CMSAnalysis/Utility/interface/Utility.hh"
#include "TH1.h"
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/unordered_map.hpp>
#include <cassert>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

const std::vector<std::string> HiggsKansasStateAnalysis::genSimDecays{
    "eeee", "eeeu", "eeet", "eeuu", "eeut", "eett", "eueu", "euet", "euuu", "euut", "eutt",
    "etet", "etuu", "etut", "ettt", "uuuu", "uuut", "uutt", "utut", "uttt", "tttt"};

const std::vector<std::string> HiggsKansasStateAnalysis::recoDecays{
    "eeee", "eeeu", "eeet", "eeuu", "eeut", "eett", "eueu", "euet", "euuu", "euut", "eutt", "etet",
    "etuu", "etut", "ettt", "uuuu", "uuut", "uutt", "utut", "uttt", "tttt", "eee_", "eeu_", "eue_",
    "euu_", "uue_", "uuu_", "eet_", "ete_", "eut_", "etu_", "uut_", "utu_", "ett_", "utt_", "ttt_",
    "ee__", "e_e_", "eu__", "e_u_", "uu__", "u_u_", "tt__", "t_t_", "et__", "e_t_", "ut__", "u_t_"};

const std::vector<std::string> systematics{"ElectronScaleFactor", "MuonIDISOScaleFactor", "MuonRecoScaleFactor",
                                           "MuonTriggerScaleFactor"};

// Actual masses for the Higgs signal
const std::vector<int> HiggsKansasStateAnalysis::massTargets{500, 600, 700, 800, 900, 1000, 1100, 1200, 1300, 1400, 1500};

constexpr auto bgFilePath = "/uscms/home/bhenning/nobackup/HiggsReprocessing/";
constexpr auto signalFilePath = "/uscms/home/bhenning/nobackup/HiggsReprocessing/";
constexpr auto dataFilePath = "/uscms/home/bhenning/nobackup/030426HiggsData/";
const auto signalParamPath = Utility::getBasePath() + "Analysis/bin/fitting/H++SignalParameterFunctions.txt";
const auto bgParamPath =
    "/uscms/home/kprasad/cmsReleaseArea/CMSSW_15_0_4/src/CMSAnalysis/Analysis/bin/fitting/H++BackgroundFunctions.txt";

double HiggsKansasStateAnalysis::getBranchingRatio(const std::string &channel) const
{
    std::unordered_map<std::string, double> originalRatios = {{"ee", 3.0 / 2}, {"eu", 3.0 / 4}, {"uu", 3.0 / 2},
                                                              {"et", 3.0 / 4}, {"ut", 3.0 / 4}, {"tt", 3.0 / 2}};

    std::string firstPair = channel.substr(0, 2);
    std::string secondPair = channel.substr(2, 2);

    return originalRatios[firstPair] * originalRatios[secondPair];
}

std::vector<std::string> HiggsKansasStateAnalysis::getSystematics() const
{
    return systematics;
}

HiggsKansasStateAnalysis::HiggsKansasStateAnalysis()
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

    auto signalParams = FitFunctionCollection::loadFunctions(signalParamPath);

    //                 (genSim     , reco       )
    std::map<std::tuple<std::string, std::string>,
             //
             std::vector<std::tuple<HistVariable,
                                    //       paramName
                                    std::map<std::string, FitFunction>>>>
        signalParamMap;

    for (const auto &pair : signalParams.getFunctions())
    {
        const auto name = pair.first;
        // (HistVar, reco, genSim, param)
        const auto parsed = parseSignalParamFuncName(name);
        const auto histVar = std::get<0>(parsed);
        const auto reco = std::get<1>(parsed);
        const auto genSim = std::get<2>(parsed);
        const auto paramName = std::get<3>(parsed);

        const auto func = pair.second;

        auto &inner = signalParamMap[{genSim, reco}];
        std::map<std::string, FitFunction> *value = nullptr;
        for (auto &entry : inner)
        {
            if (std::get<0>(entry) == histVar)
            {
                value = &std::get<1>(entry);
                break;
            }
        }
        if (!value)
        {
            inner.push_back({histVar, {}});
            value = &std::get<1>(inner.at(inner.size() - 1));
        }

        (*value)[paramName] = func;
    }

    auto bgParams = FitFunctionCollection::loadFunctions(bgParamPath);

    //                 (reco       , bg name     )
    std::map<std::tuple<std::string, std::string>,
             //
             std::vector<std::tuple<HistVariable, FitFunction>>>
        bgParamMap;

    for (const auto &pair : bgParams.getFunctions())
    {
        const auto name = pair.first;
        const auto parsed = parseBgFuncName(name);
        const auto histVar = std::get<0>(parsed);
        const auto channelName = std::get<1>(parsed);
        const auto bgName = std::get<2>(parsed);
        bgParamMap[{channelName, bgName}].push_back({histVar, pair.second});
    }

    const double luminosity = 137.94;

    TH1::SetDefaultSumw2();
    for (bool zSelection : {true, false})
    {
        for (const std::string &recoDecay : recoDecays)
        {
            std::vector<std::shared_ptr<Process>> processes;
            std::map<int, std::shared_ptr<Process>> signalProcesses;

            const std::string modeLabel = zSelection ? "_ZPeak" : "";
            const auto channelName = recoDecay + modeLabel;

            for (const double massTarget : massTargets)
            {
                auto higgsMassGroup = std::make_shared<Process>("Higgs Signal " + std::to_string((int)massTarget), 1);

                for (const auto &genSimDecay : genSimDecays)
                {
                    auto histMapperLowMass =
                        std::make_shared<HiggsHistNameFinder>(recoDecay, genSimDecay, zSelection, true);
                    auto histMapperHighMass =
                        std::make_shared<HiggsHistNameFinder>(recoDecay, genSimDecay, zSelection, false);

                    double branchingRatioFixer = getBranchingRatio(genSimDecay);

                    auto higgsSignal = std::make_shared<Process>(
                        "Higgs signal " + genSimDecay + " " + std::to_string((int)massTarget), 1);

                    try
                    {
                        for (auto fit : signalParamMap.at(std::tuple(genSimDecay, channelName)))
                        {
                            const auto histVar = std::get<0>(fit);
                            auto params = std::get<1>(fit);
                            // TODO: Make this more robust
                            auto funcType = FitFunction::FunctionType::DoubleSidedCrystalBall;
                            if (params.contains("mul_{2}"))
                            {
                                funcType = FitFunction::FunctionType::DoubleGaussian;
                            }
                            auto func = FitFunction::createFunctionOfType(funcType, "", "", 0, 2000, channelName);

                            auto *tf1 = func.getFunction();

                            for (auto param : params)
                            {
                                const auto name = std::get<0>(param);
                                auto fit = std::get<1>(param);
                                const auto *fitTf1 = fit.getFunction();
                                const auto value = fitTf1->Eval(massTarget);
                                tf1->SetParameter(name, value);
                            }
                            higgsSignal->setPlot(histVar, func);
                        }
                    }
                    catch (std::out_of_range &e)
                    {
                    }
                    addSingleProcess(higgsSignal, signalFilePath, "HppM" + std::to_string((int)massTarget) + "_2016.root",
                                     "higgs4l" + std::to_string((int)massTarget), reader, luminosity, histMapperLowMass,
                                     histMapperHighMass, false, branchingRatioFixer);
                    addSingleProcess(higgsSignal, signalFilePath, "HppM" + std::to_string((int)massTarget) + "_2017.root",
                                     "higgs4l" + std::to_string((int)massTarget), reader, luminosity, histMapperLowMass,
                                     histMapperHighMass, false, branchingRatioFixer);
                    addSingleProcess(higgsSignal, signalFilePath, "HppM" + std::to_string((int)massTarget) + "_2018.root",
                                     "higgs4l" + std::to_string((int)massTarget), reader, luminosity, histMapperLowMass,
                                     histMapperHighMass, false, branchingRatioFixer);

                    processes.push_back(higgsSignal);
                    addSingleProcess(higgsMassGroup, signalFilePath,
                                     "HppM" + std::to_string((int)massTarget) + "_2016.root",
                                     "higgs4l" + std::to_string((int)massTarget), reader, luminosity, histMapperLowMass,
                                     histMapperHighMass, false, branchingRatioFixer);
                    addSingleProcess(higgsMassGroup, signalFilePath,
                                     "HppM" + std::to_string((int)massTarget) + "_2017.root",
                                     "higgs4l" + std::to_string((int)massTarget), reader, luminosity, histMapperLowMass,
                                     histMapperHighMass, false, branchingRatioFixer);
                    addSingleProcess(higgsMassGroup, signalFilePath,
                                     "HppM" + std::to_string((int)massTarget) + "_2018.root",
                                     "higgs4l" + std::to_string((int)massTarget), reader, luminosity, histMapperLowMass,
                                     histMapperHighMass, false, branchingRatioFixer);
                    
                }
                processes.push_back(higgsMassGroup);
            }

            auto histMapperLowMass = std::make_shared<HiggsHistNameFinder>(recoDecay, "", zSelection, true);
            auto histMapperHighMass = std::make_shared<HiggsHistNameFinder>(recoDecay, "", zSelection, false);

            auto zzBackground = std::make_shared<Process>("ZZ Background", ZZBackgroundColor);
            std::vector<std::string> names = {"ZZTo2L2Nu", "ZZTo2Q2L", "ZZTo4L"};
            for (auto name : names)
            {
                addSingleProcess(zzBackground, bgFilePath, name + "_2016.root", reader, luminosity,
                             histMapperLowMass, histMapperHighMass, false);
                addSingleProcess(zzBackground, bgFilePath, name + "_2017.root", reader, luminosity,
                             histMapperLowMass, histMapperHighMass, false, 1);
                addSingleProcess(zzBackground, bgFilePath, name + "_2018.root", reader, luminosity,
                             histMapperLowMass, histMapperHighMass, false, 1);
            } 

            auto wJetsBackground = std::make_shared<Process>("WJets Background", WJetsBackgroundColor);
            std::vector<std::string> names = {"WJetsToLNu_NLO", "WJetsToLNu_HT-800To1200", "WJetsToLNu_HT-70To100", 
            "WJetsToLNu_HT-600To800", "WJetsToLNu_HT-400To600", "WJetsToLNu_HT-2500ToInf", "WJetsToLNu_HT-200To400",
            "WJetsToLNu_HT-1200To2500", "WJetsToLNu_HT-100To200", "WJetsToLNu_2J", "WJetsToLNu", "WJetsToLNu_1J",
            "WJetsToLNu_0J"};
            for (auto name : names)
            {
                addSingleProcess(wJetsBackground, bgFilePath, name + "_2016.root", reader, luminosity,
                             histMapperLowMass, histMapperHighMass, false);
                addSingleProcess(wJetsBackground, bgFilePath, name + "_2017.root", reader, luminosity,
                             histMapperLowMass, histMapperHighMass, false, 1);
                addSingleProcess(wJetsBackground, bgFilePath, name + "_2018.root", reader, luminosity,
                             histMapperLowMass, histMapperHighMass, false, 1);
            } 

            // cross sections should be all lowercase
            auto ttBarandMultiBosonBackground = std::make_shared<Process>("t#bar{t}, Multiboson Background", ttbarColor);
            std::vector<std::string> names = {"TTTo2L2Nu", "TTToHadronic", "TTToSemiLeptonic", "ttWJets", "ttZJets",
            "TWZToLL", "TWZToLL", "WGToLNuG", " ST_s-channel", "ST_t-channel_antitop", "ST_t-channel_top", "ST_tW_antitop",
            "ST_tW_top", "WW", " WWTo2L2Nu", "WWW", "WWZ", "WZ", " WZTo2Q2L", "WZTo3LNu", "WZZ", "ZHToMuMu", "ZHToTauTau", "HZJ_HToWWTo2L2Nu_ZTo2L",
            "ttHJetToNonbb", "ttHTo2L2Nu", "ttHToEE", "ttHToTauTau", "ZZZ"};
            for (auto name : names)
            {
                addSingleProcess(ttBarandMultiBosonBackground, bgFilePath, name + "_2016.root", reader, luminosity,
                             histMapperLowMass, histMapperHighMass, false);
                addSingleProcess(ttBarandMultiBosonBackground, bgFilePath, name + "_2017.root", reader, luminosity,
                             histMapperLowMass, histMapperHighMass, false, 1);
                addSingleProcess(ttBarandMultiBosonBackground, bgFilePath, name + "_2018.root", reader, luminosity,
                             histMapperLowMass, histMapperHighMass, false, 1);
            }
            

            auto dyBackground = std::make_shared<Process>("Drell-Yan Background", drellYanBackColor);
            std::vector<std::string> names = {"DYJetsToLLM10to50_", "DYJetsToLLM50"};
            for (auto name : names)
            {
                addSingleProcess(dyBackground, bgFilePath, name + "_2016.root", reader, luminosity,
                             histMapperLowMass, histMapperHighMass, false);
                addSingleProcess(dyBackground, bgFilePath, name + "_2017.root", reader, luminosity,
                             histMapperLowMass, histMapperHighMass, false, 1);
                addSingleProcess(dyBackground, bgFilePath, name + "_2018.root", reader, luminosity,
                             histMapperLowMass, histMapperHighMass, false, 1);
            }

            auto qcdBackground = std::make_shared<Process>("QCD Background", QCDBackColor);
            std::vector<std::string> names = {"QCD_HT1000to1500", " QCD_HT100to200", "QCD_HT1500to2000", "QCD_HT2000toInf",
            "QCD_HT200to300", "QCD_HT300to500", "QCD_HT500to700", "QCD_HT50to100", "QCD_HT700to1000"};
            for (auto name : names)
            {
                addSingleProcess(qcdBackground, bgFilePath, name + "_2016.root", reader, luminosity,
                             histMapperLowMass, histMapperHighMass, false);
                addSingleProcess(qcdBackground, bgFilePath, name + "_2017.root", reader, luminosity,
                             histMapperLowMass, histMapperHighMass, false, 1);
                addSingleProcess(qcdBackground, bgFilePath, name + "_2018.root", reader, luminosity,
                             histMapperLowMass, histMapperHighMass, false, 1);
            }

            auto higgsData = std::make_shared<Process>("Data", higgsColor);
            std::vector<std::string> names = {"SingleMuonA", "SingleMuonB", "SingleMuonC", "SingleMuonD", "SingleMuonE", 
            "SingleMuonF", "SingleMuonG", "SingleMuonH", "EgammaA", "EGammaB", "EGammaC", "EGammaD",
            "SingleElectronB", "SingleElectronC", "SingleElectronD", "SingleElectronE", "SingleElectronF", "SingleElectronG",
            "SingleElectronH"};
            for (auto name : names)
            {
                addSingleProcess(higgsData, bgFilePath, name + "_2016.root", reader, luminosity,
                             histMapperLowMass, histMapperHighMass, false);
                addSingleProcess(higgsData, bgFilePath, name + "_2017.root", reader, luminosity,
                             histMapperLowMass, histMapperHighMass, false, 1);
                addSingleProcess(higgsData, bgFilePath, name + "_2018.root", reader, luminosity,
                             histMapperLowMass, histMapperHighMass, false, 1);
            }


            processes.push_back(dyBackground);
            processes.push_back(higgsData);
            processes.push_back(qcdBackground);
            processes.push_back(zzBackground);
            processes.push_back(ttBarandMultiBosonBackground);
            processes.push_back(wJetsBackground);

            // for (auto process : processes)
            // {
            //     for (auto systematic : systematics)
            //     {
            //         auto sys = std::make_shared<ShapeSystematic>(systematic);
            //         process->addSystematic(sys);
            //     }
            // }

            auto leptonProcesses = std::make_shared<Channel>(channelName, processes);

            for (const std::string &processName : leptonProcesses->getNames())
            {
                if (processName == "Higgs Signal 1000")
                // if (processName == "Higgs signal eueu 1000")
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
                {
                    // This line is only used for complete plots
                    leptonProcesses->labelProcess(Channel::Label::Data, processName);
                    // std::cout << "Labeled Data: " << processName << std::endl;
                }
                else
                {
                    leptonProcesses->labelProcess(Channel::Label::Background, processName);
                    // std::cout << "Labeled Background: " << processName << std::endl;
                }
            }

            for (const auto &proc : leptonProcesses->getWithLabel(Channel::Label::Background))
            {
                try
                {
                    for (auto fit : bgParamMap.at(std::tuple(channelName, proc->getName())))
                    {
                        proc->setPlot(std::get<0>(fit), std::get<1>(fit));
                    }
                }
                catch (std::out_of_range &e)
                {
                }
            }

            getChannelsProtected().push_back(leptonProcesses);
        }
    }
}

void HiggsKansasStateAnalysis::addSingleProcess(std::shared_ptr<Process> process, std::string filePathway,
                                             std::string fileName, bool isData)
{
    process->addProcess(makeBasicProcess(filePathway, fileName, crossSectionName, crossReader, luminosity,
                                         mappingLowMass, isData, branchingRatioAdjustment));
    process->addProcess(makeBasicProcess(filePathway, fileName, crossSectionName, crossReader, luminosity,
                                         mappingHighMass, isData, branchingRatioAdjustment));
}

std::tuple<HistVariable, std::string, std::string, std::string> HiggsKansasStateAnalysis::parseSignalParamFuncName(
    const std::string &name)
{
    // Name format: uttt_uttt/#alpha_{low} 1400 MuonTriggerScaleFactor Up X projection
    std::vector<std::string> parts;
    boost::split(parts, name, boost::is_any_of("/"));
    const auto genReco = parts.at(0);
    const auto subName = parts.at(1);

    std::vector<std::string> parts2;
    boost::split(parts2, genReco, boost::is_any_of("_"));
    const auto genSim = parts2.at(0);
    const auto reco = parts2.at(1);

    std::vector<std::string> parts3;
    boost::split(parts3, subName, boost::is_any_of(" "));
    const auto paramName = parts3.at(0);
    const auto projName = parts3.at(parts3.size() - 2);
    const auto systName = parts3.at(2);

    const auto xProj = projName == "X";
    const auto yProj = projName == "Y";
    assert(xProj || yProj);

    auto histVar = HistVariable(HistVariable::VariableType::InvariantMass, "", xProj, yProj);

    if (systName != "Nominal")
    {
        auto systType = ScaleFactor::SystematicType::Nominal;
        const auto &typeName = parts3.at(3);
        if (typeName == "Up")
        {
            systType = ScaleFactor::SystematicType::Up;
        }
        else if (typeName == "Down")
        {
            systType = ScaleFactor::SystematicType::Down;
        }
        else
        {
            throw std::runtime_error("Error parsing signal parameterization");
        }
        histVar.setSystematic(systType, systName);
    }

    return {histVar, reco, genSim, paramName};
}

std::tuple<HistVariable, std::string, std::string> HiggsKansasStateAnalysis::parseBgFuncName(const std::string &name)
{
    // Name format: t#bar{t}, Multiboson Background->eueu_ZPeak/Reco Invariant Mass Background Y Projection
    std::vector<std::string> parts;
    boost::split(parts, name, boost::is_any_of("/"));
    const auto bgReco = parts.at(0);
    const auto subName = parts.at(1);

    std::vector<std::string> parts2;
    boost::iter_split(parts2, bgReco, boost::first_finder("->"));
    const auto bgName = parts2.at(0);
    const auto reco = parts2.at(1);

    std::vector<std::string> parts3;
    boost::split(parts3, subName, boost::is_any_of(" "));
    const auto projName = parts3.at(4);
    const auto systName = parts3.at(6);

    const auto xProj = projName == "X";
    const auto yProj = projName == "Y";
    assert(xProj || yProj);

    auto histVar = HistVariable(HistVariable::VariableType::InvariantMass, "", xProj, yProj);

    if (systName != "Nominal")
    {
        auto systType = ScaleFactor::SystematicType::Nominal;
        const auto &typeName = parts3.at(7);
        if (typeName == "Up")
        {
            systType = ScaleFactor::SystematicType::Up;
        }
        else if (typeName == "Down")
        {
            systType = ScaleFactor::SystematicType::Down;
        }
        else
        {
            throw std::runtime_error("Error parsing bg parameterization");
        }
        histVar.setSystematic(systType, systName);
    }

    return {histVar, reco, bgName};
}
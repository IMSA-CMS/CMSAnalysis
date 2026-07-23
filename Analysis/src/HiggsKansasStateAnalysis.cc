#include "CMSAnalysis/Analysis/interface/HiggsKansasStateAnalysis.hh"
#include "CMSAnalysis/Analysis/interface/Channel.hh"
#include "CMSAnalysis/Analysis/interface/CrossSectionReader.hh"
#include "CMSAnalysis/Analysis/interface/FitFunction.hh"
#include "CMSAnalysis/Analysis/interface/FitFunctionCollection.hh"
#include "CMSAnalysis/Analysis/interface/HiggsHistNameFinder.hh"
#include "CMSAnalysis/Analysis/interface/HistVariable.hh"
#include "CMSAnalysis/Analysis/interface/HiggsKansasStateNameFinder.hh"
#include "CMSAnalysis/Analysis/interface/Process.hh"
#include "CMSAnalysis/Analysis/interface/RootFileInput.hh"
#include "CMSAnalysis/Analysis/interface/TrivialEstimator.hh"
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

const std::vector<std::string> HiggsKansasStateAnalysis::genSimDecays{};

const std::vector<std::string> HiggsKansasStateAnalysis::recoDecays{
    "0tau", "1tau", "2tau", "3tau"};

const std::vector<std::string> systematics{"ElectronScaleFactor", "MuonIDISOScaleFactor", "MuonRecoScaleFactor",
                                           "MuonTriggerScaleFactor"};

// Actual masses for the Higgs signal
const std::vector<int> HiggsKansasStateAnalysis::massTargets{500, 600, 700, 800, 900, 1000, 1100, 1200, 1300, 1400, 1500};

const double lumi = 137.94;

constexpr auto bgFilePath = "/eos/uscms/store/user/greddy/DCH_files/inputs_nopair/hist_MY/";
constexpr auto signalFilePath = "/eos/uscms/store/user/greddy/DCH_files/inputs_nopair/hist_MY/";
constexpr auto dataFilePath = "/eos/uscms/store/user/greddy/DCH_files/inputs_nopair/hist_MY/";
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

HiggsKansasStateAnalysis::HiggsKansasStateAnalysis() :
    FullAnalysis(lumi)
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
        Utility::getBasePath() + "DataCollection/bin/crossSections.txt");
        
    auto signalParams = FitFunctionCollection::loadFunctions(signalParamPath);

    //                 (genSim     , reco       )
    // std::map<std::tuple<std::string, std::string>,
    //          //
    //          std::vector<std::tuple<HistVariable,
    //                                 //       paramName
    //                                 std::map<std::string, FitFunction>>>>
    //     signalParamMap;

    // for (const auto &pair : signalParams.getFunctions())
    // {
    //     const auto name = pair.first;
    //     // (HistVar, reco, genSim, param)
    //     const auto parsed = parseSignalParamFuncName(name);
    //     const auto histVar = std::get<0>(parsed);
    //     const auto reco = std::get<1>(parsed);
    //     const auto genSim = std::get<2>(parsed);
    //     const auto paramName = std::get<3>(parsed);

    //     const auto func = pair.second;

    //     auto &inner = signalParamMap[{genSim, reco}];
    //     std::map<std::string, FitFunction> *value = nullptr;
    //     for (auto &entry : inner)
    //     {
    //         if (std::get<0>(entry) == histVar)
    //         {
    //             value = &std::get<1>(entry);
    //             break;
    //         }
    //     }
    //     if (!value)
    //     {
    //         inner.push_back({histVar, {}});
    //         value = &std::get<1>(inner.at(inner.size() - 1));
    //     }

    //     (*value)[paramName] = func;
    // }

    // auto bgParams = FitFunctionCollection::loadFunctions(bgParamPath);

    // //                 (reco       , bg name     )
    // std::map<std::tuple<std::string, std::string>,
    //          //
    //          std::vector<std::tuple<HistVariable, FitFunction>>>
    //     bgParamMap;

    // for (const auto &pair : bgParams.getFunctions())
    // {
    //     const auto name = pair.first;
    //     const auto parsed = parseBgFuncName(name);
    //     const auto histVar = std::get<0>(parsed);
    //     const auto channelName = std::get<1>(parsed);
    //     const auto bgName = std::get<2>(parsed);
    //     bgParamMap[{channelName, bgName}].push_back({histVar, pair.second});
    // }

    TH1::SetDefaultSumw2();
    for (bool zSelection : {true, false})
    {
        for (const std::string &recoDecay : recoDecays)
        {
            std::vector<std::shared_ptr<Process>> processes;
            std::map<int, std::shared_ptr<Process>> signalProcesses;

            const std::string modeLabel = zSelection ? "_ZPeak" : "";
            const auto channelName = recoDecay + modeLabel;

            auto histMapper =
                std::make_shared<HiggsKansasStateNameFinder>(recoDecay);

            for (const double massTarget : massTargets)
            {
                auto higgsMassGroup = std::make_shared<Process>("Higgs Signal " + std::to_string((int)massTarget), 1);

                auto higgsSignal = std::make_shared<Process>(
                    "Higgs signal " + std::to_string((int)massTarget), 1);

                // try
                // {
                //     for (auto fit : signalParamMap.at(std::tuple(genSimDecay, channelName)))
                //     {
                //         const auto histVar = std::get<0>(fit);
                //         auto params = std::get<1>(fit);
                //         // TODO: Make this more robust
                //         auto funcType = FitFunction::FunctionType::DoubleSidedCrystalBall;
                //         if (params.contains("mul_{2}"))
                //         {
                //             funcType = FitFunction::FunctionType::DoubleGaussian;
                //         }
                //         auto func = FitFunction::createFunctionOfType(funcType, "", "", 0, 2000, channelName);

                //         auto *tf1 = func.getFunction();

                //         for (auto param : params)
                //         {
                //             const auto name = std::get<0>(param);
                //             auto fit = std::get<1>(param);
                //             const auto *fitTf1 = fit.getFunction();
                //             const auto value = fitTf1->Eval(massTarget);
                //             tf1->SetParameter(name, value);
                //         }
                //         higgsSignal->setPlot(histVar, func);
                //     }
                // }
                // catch (std::out_of_range &e)
                // {
                // }

                addSingleProcess(higgsSignal, signalFilePath, "HppM" + std::to_string((int)massTarget), histMapper);

                processes.push_back(higgsSignal);

                addSingleProcess(higgsMassGroup, signalFilePath, "HppM" + std::to_string((int)massTarget), histMapper);
                processes.push_back(higgsMassGroup);
            }


            auto zzBackground = std::make_shared<Process>("ZZ Background", ZZBackgroundColor);
            for (auto name : {"ZZTo2L2Nu", "ZZTo2Q2L", "ZZTo4L"})
            {
                addSingleProcess(zzBackground, bgFilePath, name, histMapper);
            } 

            auto wJetsBackground = std::make_shared<Process>("WJets Background", WJetsBackgroundColor);
           
            for (auto name : {"WJetsToLNu_NLO", "WJetsToLNu_HT-800To1200", "WJetsToLNu_HT-70To100", 
            "WJetsToLNu_HT-600To800", "WJetsToLNu_HT-400To600", "WJetsToLNu_HT-2500ToInf", "WJetsToLNu_HT-200To400",
            "WJetsToLNu_HT-1200To2500", "WJetsToLNu_HT-100To200", "WJetsToLNu_2J", "WJetsToLNu", "WJetsToLNu_1J",
            "WJetsToLNu_0J"})
            {
                addSingleProcess(wJetsBackground, bgFilePath, name, histMapper);
            } 

            auto ttBarandMultiBosonBackground = std::make_shared<Process>("t#bar{t}, Multiboson Background", ttbarColor);

            for (auto name : {"TTTo2L2Nu", "TTToHadronic", "TTToSemiLeptonic", "ttWJets", "ttZJets",
            "TWZToLL", "TWZToLL", "WGToLNuG", " ST_s-channel", "ST_t-channel_antitop", "ST_t-channel_top", "ST_tW_antitop",
            "ST_tW_top", "WW", " WWTo2L2Nu", "WWW", "WWZ", "WZ", " WZTo2Q2L", "WZTo3LNu", "WZZ", "ZHToMuMu", "ZHToTauTau", "HZJ_HToWWTo2L2Nu_ZTo2L",
            "ttHJetToNonbb", "ttHTo2L2Nu", "ttHToEE", "ttHToTauTau", "ZZZ"})
            {
                addSingleProcess(ttBarandMultiBosonBackground, bgFilePath, name, histMapper);
            }
            

            auto dyBackground = std::make_shared<Process>("Drell-Yan Background", drellYanBackColor);

            for (auto name : {"DYJetsToLLM10to50_", "DYJetsToLLM50"})
            {
                addSingleProcess(dyBackground, bgFilePath, name, histMapper);
            }

            auto qcdBackground = std::make_shared<Process>("QCD Background", QCDBackColor);
            for (auto name : {"QCD_HT1000to1500", " QCD_HT100to200", "QCD_HT1500to2000", "QCD_HT2000toInf",
            "QCD_HT200to300", "QCD_HT300to500", "QCD_HT500to700", "QCD_HT50to100", "QCD_HT700to1000"})
            {
                addSingleProcess(qcdBackground, bgFilePath, name, histMapper);
            }

            auto higgsData = std::make_shared<Process>("Data", higgsColor);
            for (auto name : {"SingleMuonA", "SingleMuonB", "SingleMuonC", "SingleMuonD", "SingleMuonE", 
            "SingleMuonF", "SingleMuonG", "SingleMuonH", "EgammaA", "EGammaB", "EGammaC", "EGammaD",
            "SingleElectronB", "SingleElectronC", "SingleElectronD", "SingleElectronE", "SingleElectronF", "SingleElectronG",
            "SingleElectronH"})
            {
                addSingleProcess(higgsData, dataFilePath, name, histMapper);
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

            // for (const auto &proc : leptonProcesses->getWithLabel(Channel::Label::Background))
            // {
            //     try
            //     {
            //         for (auto fit : bgParamMap.at(std::tuple(channelName, proc->getName())))
            //         {
            //             proc->setPlot(std::get<0>(fit), std::get<1>(fit));
            //         }
            //     }
            //     catch (std::out_of_range &e)
            //     {
            //     }
            // }

            getChannelsProtected().push_back(leptonProcesses);
        }
    }
}

void HiggsKansasStateAnalysis::addSingleProcess(std::shared_ptr<Process> process, std::string filePathway,
                                                std::string fileName, std::shared_ptr<HistNameFinder> finder)
{
    auto input1 = std::make_shared<RootFileInput>(filePathway + fileName + "_2016.root", finder);
    auto input2 = std::make_shared<RootFileInput>(filePathway + fileName + "_2017.root", finder);
    auto input3 = std::make_shared<RootFileInput>(filePathway + fileName + "_2018.root", finder);
    auto histEstimator = std::make_shared<TrivialEstimator>();

    process->addProcess(SingleProcess(fileName, input1, histEstimator));
    process->addProcess(SingleProcess(fileName, input2, histEstimator));
    process->addProcess(SingleProcess(fileName, input3, histEstimator));
}
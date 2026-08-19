#include "CMSAnalysis/Analysis/interface/FitFunctionCollection.hh"
#include "CMSAnalysis/Analysis/interface/HiggsCompleteAnalysis.hh"
#include "TCanvas.h"
#include "TClass.h"
#include "TColor.h"
#include "TDirectory.h"
#include "TFile.h"
#include "TGraph.h"
#include "TKey.h"
#include "TLatex.h"
#include "TLine.h"
#include "TList.h"
#include "TStyle.h"
#include "TSystem.h"
#include "TF1.h"
#include "TH1.h"
#include <algorithm>
#include <cctype>
#include <chrono>
#include <cmath>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

// Window analysis driver for H++ studies.
// The goal is to keep output behavior the same while supporting two input sources:
// 1) local HiggsCompleteAnalysis channels
// 2) Guru EOS ROOT files loaded directly from disk

// These are the signal mass hypotheses scanned by this script.
const std::vector<int> TEST_MASSES = {500, 600, 700, 800, 900, 1000, 1100, 1200, 1300, 1400, 1500};
// The fixed half-width used to build [mass - halfwidth, mass + halfwidth] counting windows.
const double WINDOW_HALFWIDTH = 40.0;

enum class InputFlavor
{
    AutoDetect,
    LocalGroup,
    GuruGroup
};

// Default behavior for plain "WindowAnalysis.C+" runs (no explicit argument).
const InputFlavor DEFAULT_INPUT_FLAVOR = InputFlavor::LocalGroup;

// Maps CLI mode integers to input flavors.
// mode=0 -> local, mode=1 -> guru, mode=2 -> auto-detect, anything else -> default local.
InputFlavor inputFlavorFromMode(const int mode)
{
    if (mode == 0)
        return InputFlavor::LocalGroup;
    if (mode == 1)
        return InputFlavor::GuruGroup;
    if (mode == 2)
        return InputFlavor::AutoDetect;
    std::cout << "WARNING: Unknown mode " << mode << ". Falling back to LocalGroup.\n";
    return InputFlavor::LocalGroup;
}

// Maps human-readable mode names to integer mode codes used internally.
// Accepted strings: "local", "guru", "auto" / "autodetect".
int modeCodeFromString(std::string modeText)
{
    std::transform(modeText.begin(), modeText.end(), modeText.begin(),
                   [](unsigned char c)
                   {
                       return static_cast<char>(std::tolower(c));
                   });
    if (modeText == "local")
        return 0;
    if (modeText == "guru")
        return 1;
    if (modeText == "auto" || modeText == "autodetect")
        return 2;
    std::cout << "WARNING: Unknown mode string '" << modeText << "'. Falling back to LocalGroup.\n";
    return 0;
}

// Parameterized background function dump used only in local mode.
const std::string BACKGROUND_FUNCTIONS_FILE = "H++BackgroundFunctions.txt";
// Main textual report written by this script.
const std::string OUTPUT_FILE = "WindowAnalysis_results.txt";
// Main sensitivity plot outputs.
const std::string PLOT_PNG_FILE = "WindowAnalysis_poster_sensitivity.png";
const std::string PLOT_PDF_FILE = "WindowAnalysis_poster_sensitivity.pdf";
// Method-ratio plot outputs (only meaningful when parameterization is enabled).
const std::string RATIO_PLOT_PNG_FILE = "WindowAnalysis_poster_ratio.png";
const std::string RATIO_PLOT_PDF_FILE = "WindowAnalysis_poster_ratio.pdf";

// Canonical EOS location for Guru histogram files.
const std::string GURU_INPUT_DIR = "/eos/uscms/store/user/greddy/DCH_files/inputs_nopair/hist_MY/";
// Guru files are split by year, so we always sum these three years.
const std::vector<std::string> GURU_YEARS = {"2016", "2017", "2018"};

// Main physics backgrounds used in the existing local workflow.
const std::vector<std::pair<std::string, std::string>> BACKGROUNDS = {
    {"Drell-Yan Background",            "Drell-Yan"},
    {"QCD Background",                  "QCD"},
    {"ZZ Background",                   "ZZ"},
    {"WJets Background",                "WJets1"},
    {"t#bar{t}, Multiboson Background", "ttbar/Multiboson"}
};

// Guru file groups mirrored from existing *FromFile fitting utilities.
const std::unordered_map<std::string, std::vector<std::string>> GURU_BACKGROUND_FILE_BASES = {
    {"Drell-Yan Background", {
        "DYJetsToLLM10to50", "DYJetsToLLM50"
    }},
    {"QCD Background", {
        "QCD_HT1000to1500", "QCD_HT100to200", "QCD_HT1500to2000", "QCD_HT2000toInf",
        "QCD_HT200to300", "QCD_HT300to500", "QCD_HT500to700", "QCD_HT50to100", "QCD_HT700to1000"
    }},
    {"ZZ Background", {
        "ZZTo2L2Nu", "ZZTo2Q2L", "ZZTo4L"
    }},
    {"WJets Background", {
        "WJetsToLNu", "WJetsToLNu_0J", "WJetsToLNu_1J", "WJetsToLNu_2J",
        "WJetsToLNu_HT-100To200", "WJetsToLNu_HT-200To400",
        "WJetsToLNu_HT-400To600", "WJetsToLNu_HT-600To800", "WJetsToLNu_HT-70To100",
        "WJetsToLNu_HT-800To1200", "WJetsToLNu_HT-1200To2500", "WJetsToLNu_HT-2500ToInf", "WJetsToLNu_NLO"
    }},
    {"t#bar{t}, Multiboson Background", {
        "ST_s-channel", "ST_t-channel_antitop", "ST_t-channel_top", "ST_tW_antitop", "ST_tW_top",
        "TTTo2L2Nu", "TTToHadronic", "TTToSemiLeptonic", "ttHJetToNonbb", "ttHTo2L2Nu",
        "ttHToEE", "ttHToTauTau", "ttWJets", "ttZJets", "GluGluZH", "HZJ_HToWWTo2L2Nu_ZTo2L",
        "TWZToLL", "WGToLNuG", "WWTo2L2Nu", "WWW", "WWZ", "WW", "WZTo2Q2L", "WZTo3LNu",
        "WZZ", "WZ", "ZHToMuMu", "ZHToTauTau", "ZZZ"
    }}
};

std::string toLower(std::string value)
{
    // Transform each character to lowercase in place.
    std::transform(value.begin(), value.end(), value.begin(),
                   [](unsigned char c)
                   {
                        return static_cast<char>(std::tolower(c));
                   });
    // Return normalized string for case-insensitive comparisons.
    return value;
}

std::string getCurrentTimeString()
{
    // Capture current wall-clock time from the system clock.
    const auto now = std::chrono::system_clock::now();
    // Convert to calendar time for formatting.
    const auto timeValue = std::chrono::system_clock::to_time_t(now);
    // Also keep millisecond precision so logs are unique and easy to correlate.
    const auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

    // Build a "YYYY-MM-DD HH:MM:SS.mmm" timestamp string.
    std::stringstream ss;
    ss << std::put_time(std::localtime(&timeValue), "%Y-%m-%d %H:%M:%S");
    ss << "." << std::setfill('0') << std::setw(3) << ms.count();
    // Return the finished timestamp.
    return ss.str();
}

// Data-like names must never be analyzed in this workflow.
bool isDataLikeProcessName(const std::string &processName)
{
    // Normalize to lowercase so this guard is robust to naming-case differences.
    const std::string lower = toLower(processName);
    // These are const because they are read-only booleans for this function call.
    const bool electronDataLike = lower.find("electron20") == 0;
    // Same rationale as above: fixed decision value, no mutation needed.
    const bool muonDataLike = lower.find("muon20") == 0;
    // Any data-like label means "do not analyze" to avoid catastrophic data mixing.
    return lower.find("single") != std::string::npos ||
           lower.find("egamma") != std::string::npos ||
           electronDataLike ||
           muonDataLike ||
           lower == "data";
}

// Local channels are the existing 4-char flavor combinations.
bool isLocalChannelName(const std::string &name)
{
    // Local control-region channels are excluded from this physics scan.
    if (name.find("ZPeak") != std::string::npos)
        return false;

    // Some channel names include projection suffixes; strip those before validation.
    std::string base = name;
    const size_t underscore = name.find('_');
    if (underscore != std::string::npos)
        base = name.substr(0, underscore);

    // Local channels are exactly four flavor letters (e/u/m/t).
    if (base.length() != 4)
        return false;

    // Enforce the expected alphabet to avoid accidental foreign channel formats.
    for (const char c : base)
    {
        if (c != 'e' && c != 'u' && c != 'm' && c != 't')
            return false;
    }
    // All checks passed: this is a local channel.
    return true;
}

bool detectGuruInputFromChannels(const std::vector<std::shared_ptr<Channel>> &channels)
{
    // Count channels that look local vs those that don't.
    size_t localLike = 0;
    size_t nonLocalLike = 0;

    for (const auto &channel : channels)
    {
        const std::string name = channel->getName();
        // Ignore control regions when deciding the global input flavor.
        if (name.find("ZPeak") != std::string::npos)
            continue;

        if (isLocalChannelName(name))
            ++localLike;
        else
            ++nonLocalLike;
    }

    // If non-local channels dominate, treat this as Guru-like input.
    return nonLocalLike > 0 && nonLocalLike >= localLike;
}

// Guru analysis should stay on tau channels and ignore 3lep categories.
bool isGuruAnalysisChannel(const std::string &name)
{
    const std::string lower = toLower(name);
    if (lower.find("3lep") != std::string::npos)
        return false;
    return lower.find("tau") != std::string::npos;
}

std::string projectionSuffix(const std::string &legacySuffix, const bool useGuruInput)
{
    // Keep legacy suffixes in local mode.
    if (!useGuruInput)
        return legacySuffix;
    // In Guru mode map local projection conventions to Guru naming.
    return (legacySuffix == "_x") ? "_1" : "_2";
}

std::string parameterizationProjectionLabel(const std::string &legacySuffix)
{
    // Parameterization lookup names use X/Y labels rather than suffix tokens.
    return (legacySuffix == "_x") ? "X" : "Y";
}

std::vector<std::string> guruHistogramNameCandidates(const std::string &legacySuffix)
{
    // Preferred by Guru: mH1/mH2. Keep h_mll1/h_mll2 as compatibility fallback.
    return (legacySuffix == "_x")
               ? std::vector<std::string>{"mH1", "h_mll1"}
               : std::vector<std::string>{"mH2", "h_mll2"};
}

std::string guruFilePath(const std::string &baseName, const std::string &year)
{
    // Build a full EOS filename from sample base name + year.
    return GURU_INPUT_DIR + baseName + "_" + year + ".root";
}

// Returns only years that actually exist on EOS for the given sample base.
// Cached per base name to avoid repeated filesystem checks inside nested loops.
const std::vector<std::string> &guruExistingYearsForBase(const std::string &baseName)
{
    static std::unordered_map<std::string, std::vector<std::string>> cache;
    const auto it = cache.find(baseName);
    if (it != cache.end())
        return it->second;

    std::vector<std::string> years;
    years.reserve(GURU_YEARS.size());
    for (const std::string &year : GURU_YEARS)
    {
        const std::string fullPath = guruFilePath(baseName, year);
        if (gSystem->AccessPathName(fullPath.c_str()) == 0)
            years.push_back(year);
    }

    auto inserted = cache.emplace(baseName, std::move(years));
    return inserted.first->second;
}

// Sums one histogram across many files/years for a given channel.
TH1 *combineGuruHistograms(const std::vector<std::string> &fileBases,
                           const std::string &channelName,
                           const std::vector<std::string> &histNames)
{
    // Accumulator histogram starts empty and is filled as files are found.
    TH1 *combined = nullptr;

    // Loop over all file samples that belong to one physics process.
    for (const std::string &base : fileBases)
    {
        // Loop only over years that physically exist for this sample.
        for (const std::string &year : guruExistingYearsForBase(base))
        {
            // Compose the full EOS filename for this sample/year.
            const std::strin    // Preferred by Guru: mDCH1/mDCH2. Keep h_mll1/h_mll2 as compatibility fallback.hip bound to this scope.
            std::unique_ptr<TFile> file(TFile::Open(fuDCllPath.c_str()));
            // Skip missing/corrupt filesDC instead of aborting the whole analysis.
            if (!file || file->IsZombie())
                continue;

            // Channels are stored as top-level directories in these ROOT files.
            TDirectory *dir = file->GetDirectory(channelName.c_str());
            // If a channel is not present in this sample/year, skip it.
            if (!dir)
                continue;

            // Pull the first available histogram from priority-ordered candidate names.
            TH1 *hist = nullptr;
            for (const std::string &histName : histNames)
            {
                hist = dir->Get<TH1>(histName.c_str());
                if (hist)
                    break;
            }
            // If histogram is absent, skip this file/year contribution.
            if (!hist)
                continue;

            // First valid histogram becomes the owned accumulator.
            if (!combined)
            {
                combined = dynamic_cast<TH1 *>(hist->Clone());
                // If clone fails for any reason, skip safely.
                if (!combined)
                    continue;
                // Detach from source file so histogram survives file close.
                combined->SetDirectory(nullptr);
            }
            else
            {
                // Add subsequent yearly/sample contributions into one histogram.
                combined->Add(hist);
            }
        }
    }

    // Caller owns this histogram pointer and must delete it.
    return combined;
}

TH1 *loadGuruSignalHistogram(const int mass, const std::string &channelName, const std::vector<std::string> &histCandidates)
{
    // Try known signal naming variants and return the first non-empty histogram.
    const std::vector<std::vector<std::string>> signalCandidates = {
        {"HppM" + std::to_string(mass)},
        {"Higgs" + std::to_string(mass)}
    };

    for (const auto &candidate : signalCandidates)
    {
        // Reuse the same combiner used for backgrounds to include all years.
        TH1 *hist = combineGuruHistograms(candidate, channelName, histCandidates);
        // Return first candidate that exists and has events.
        if (hist && hist->GetEntries() > 0)
            return hist;
        // Delete empty/non-usable histogram before trying next naming pattern.
        delete hist;
    }

    // No signal histogram could be loaded for this channel/mass/projection.
    return nullptr;
}

std::vector<std::string> discoverGuruChannels()
{
    // Use a set so channels are unique even if discovered from many probe files.
    std::set<std::string> names;

    // Probe a few representative files to collect directory-level channel names.
    const std::vector<std::string> probeBases = {"HppM500", "DYJetsToLLM50", "ZZTo4L"};
    for (const std::string &base : probeBases)
    {
        for (const std::string &year : guruExistingYearsForBase(base))
        {
            const std::string fullPath = guruFilePath(base, year);
            std::unique_ptr<TFile> file(TFile::Open(fullPath.c_str()));
            if (!file || file->IsZombie())
                continue;

            // Discover top-level keys to find channel directories.
            TList *keys = file->GetListOfKeys();
            if (!keys)
                continue;

            for (int i = 0; i < keys->GetSize(); ++i)
            {
                // Skip null key slots defensively.
                TObject *obj = keys->At(i);
                if (!obj)
                    continue;

                // We only care about TKey objects here.
                TKey *key = dynamic_cast<TKey *>(obj);
                if (!key)
                    continue;

                // ROOT stores class names as strings; resolve to runtime class.
                const char *className = key->GetClassName();
                if (!className)
                    continue;

                // Keep only directory-like keys because channels are directories.
                TClass *clazz = TClass::GetClass(className);
                if (!clazz || !clazz->InheritsFrom("TDirectory"))
                    continue;

                // Candidate channel name is the key name itself.
                const std::string candidate = key->GetName();
                if (candidate.empty())
                    continue;
                // Skip control region channels.
                if (candidate.find("ZPeak") != std::string::npos)
                    continue;
                // Skip anything that looks like data naming.
                if (isDataLikeProcessName(candidate))
                    continue;
                if (!isGuruAnalysisChannel(candidate))
                    continue;

                // Record channel in the unique set.
                names.insert(candidate);
            }
        }
    }

    // Convert unique set to vector for downstream loops.
    return std::vector<std::string>(names.begin(), names.end());
}

void performCleanup()
{
    // Keep cleanup conservative: remove only known transient files.
    const std::vector<std::string> tempFiles = {};
    for (const auto &file : tempFiles)
    {
        // AccessPathName returns 0 when the file exists.
        if (gSystem->AccessPathName(file.c_str()) == 0)
            // Remove known temp artifact.
            gSystem->Unlink(file.c_str());
    }
}

void WindowAnalysisImpl(const int mode = -1)
{
    // Open the plain-text results file up front so all paths share one sink.
    std::ofstream outFile(OUTPUT_FILE);
    if (!outFile.is_open())
    {
        // Hard fail early if results cannot be written.
        std::cerr << "ERROR: Could not open " << OUTPUT_FILE << " for writing\n";
        return;
    }

    // Timestamp run start for reproducibility and debugging.
    const std::string runTimestamp = getCurrentTimeString();
    // Mirror major state to stdout for live monitoring.
    std::cout << "Run started at: " << runTimestamp << "\n";
    std::cout << "Writing results to " << OUTPUT_FILE << "...\n";

    // Write report header describing run metadata and metric definition.
    outFile << "================================================================\n";
    outFile << "H++ Window Analysis Results\n";
    outFile << "================================================================\n";
    outFile << "Run timestamp:       " << runTimestamp << "\n";
    outFile << "Analysis type:       Signal significance in fixed mass windows\n";
    outFile << "Significance metric: s / sqrt(b + 2.5)\n";
    outFile << "================================================================\n";

    // Build local analysis model; this remains the source of truth in local mode.
    auto analysis = HiggsCompleteAnalysis();
    // Pull all channels now so mode detection and local loops reuse this list.
    const auto channels = analysis.getChannels();

    // Decide whether this run uses Guru EOS files or local framework channels.
    // mode=-1 means "use DEFAULT_INPUT_FLAVOR".
    const InputFlavor requestedFlavor = (mode == -1) ? DEFAULT_INPUT_FLAVOR : inputFlavorFromMode(mode);
    bool useGuruInput = false;
    if (requestedFlavor == InputFlavor::GuruGroup)
        // Explicit configuration override: force Guru mode.
        useGuruInput = true;
    else if (requestedFlavor == InputFlavor::LocalGroup)
        // Explicit configuration override: force local mode.
        useGuruInput = false;
    else
    {
        // Prefer channel-based detection; only fall back to Guru if no local-like channels are present.
        useGuruInput = detectGuruInputFromChannels(channels);
        if (!useGuruInput)
        {
            // Check whether we have any clearly local channels at all.
            bool hasLocalLikeChannels = false;
            for (const auto &channel : channels)
            {
                if (isLocalChannelName(channel->getName()))
                {
                    hasLocalLikeChannels = true;
                    break;
                }
            }
            // If local channels are absent but Guru path exists, assume Guru run.
            if (!hasLocalLikeChannels && gSystem->AccessPathName(GURU_INPUT_DIR.c_str()) == 0)
                useGuruInput = true;
        }
    }

    // Parameterization is disabled for Guru mode by requirement.
    bool useParameterization = !useGuruInput;

    struct ChannelContext
    {
        std::string channelName;
        std::unordered_map<std::string, std::shared_ptr<Process>> processByName;
        std::vector<std::shared_ptr<Process>> backgroundProcesses;
    };

    // Build cached channel/process lookup structures for local mode only.
    std::vector<ChannelContext> selectedLocalChannels;
    if (!useGuruInput)
    {
        // Reserve to avoid repeated reallocations while collecting channels.
        selectedLocalChannels.reserve(channels.size());
        for (const auto &channel : channels)
        {
            // Keep only valid local physics channels.
            const std::string channelName = channel->getName();
            if (!isLocalChannelName(channelName))
                continue;

            // Populate channel container used by the core analysis loop.
            ChannelContext context;
            context.channelName = channelName;

            // Index processes by name for O(1) lookup during mass scans.
            for (const auto &process : channel->getProcesses())
                context.processByName[process->getName()] = process;

            // Pre-resolve the background process pointers in fixed display order.
            context.backgroundProcesses.reserve(BACKGROUNDS.size());
            for (const auto &background : BACKGROUNDS)
            {
                auto bgIt = context.processByName.find(background.first);
                if (bgIt == context.processByName.end())
                    // Missing process is represented as nullptr and handled later.
                    context.backgroundProcesses.push_back(nullptr);
                else
                    // Save direct pointer for fast reuse in each mass/projection iteration.
                    context.backgroundProcesses.push_back(bgIt->second);
            }

            // Store assembled context for analysis loops.
            selectedLocalChannels.push_back(std::move(context));
        }
    }

    // In Guru mode we discover channels from ROOT directory structure at runtime.
    std::vector<std::string> selectedGuruChannels;
    if (useGuruInput)
    {
        selectedGuruChannels = discoverGuruChannels();
    }

    // Emit mode-specific counts to both console and report for diagnostics.
    std::string modeLabel = "Local(default)";
    if (mode != -1)
    {
        if (mode == 0)
            modeLabel = "Local(arg)";
        else if (mode == 1)
            modeLabel = "Guru(arg)";
        else if (mode == 2)
            modeLabel = "Auto(arg)";
        else
            modeLabel = "Local(fallback)";
    }
    std::cout << "Mode selector: " << modeLabel << "\n";
    std::cout << "Input mode: " << (useGuruInput ? "Guru" : "Local") << "\n";
    std::cout << "Single-event counting is always enabled.\n";
    if (useGuruInput)
        std::cout << "Guru channels discovered: " << selectedGuruChannels.size() << "\n";
    else
        std::cout << "Local channels selected: " << selectedLocalChannels.size() << "\n";

    // Persist run configuration in the report so outputs are self-describing.
    outFile << "\nConfiguration:\n";
    outFile << "  Mode selector:       " << modeLabel << "\n";
    outFile << "  Input mode:           " << (useGuruInput ? "Guru" : "Local") << "\n";
    outFile << "  Window half-width:    " << WINDOW_HALFWIDTH << " GeV\n";
    outFile << "  Test masses:          ";
    for (size_t i = 0; i < TEST_MASSES.size(); ++i)
    {
        if (i > 0)
            outFile << ", ";
        outFile << TEST_MASSES[i];
    }
    outFile << " GeV\n";
    outFile << "  Data safety guard:    data-like names (Single/EGamma/Electron20*/Muon20*/Data) are excluded\n";
    if (useGuruInput)
    {
        // Record the exact EOS path used for Guru input discovery/loading.
        outFile << "  Guru input dir:       " << GURU_INPUT_DIR << "\n";
        // Record histogram choice policy so results are self-describing.
        outFile << "  Guru histogram pref:  mH1/mH2 (fallback: h_mll1/h_mll2)\n";
        // Record that Guru channel discovery is intentionally tau-only.
        outFile << "  Guru channels used:   names containing 'tau' and excluding '3lep'\n";
        // Explicitly document requirement-driven disabling of parameterization.
        outFile << "  Parameterization:     disabled in Guru mode\n";
    }

    // Load fit-function parameterization only if this run allows it.
    FitFunctionCollection backgroundFunctions;
    if (useParameterization)
    {
        // Log what file is being loaded for later troubleshooting.
        std::cout << "Loading background functions from " << BACKGROUND_FUNCTIONS_FILE << "...\n";
        try
        {
            // Parse function collection from text serialization.
            backgroundFunctions = FitFunctionCollection::loadFunctions(BACKGROUND_FUNCTIONS_FILE);
            std::cout << "Loaded " << backgroundFunctions.size() << " background functions\n";
            outFile << "  Background functions loaded: " << backgroundFunctions.size() << "\n";
        }
        catch (const std::runtime_error &error)
        {
            // If loading fails, continue safely with counting-only mode.
            std::cout << "WARNING: Could not load parameterization file ('" << BACKGROUND_FUNCTIONS_FILE
                      << "'): " << error.what() << "\n";
            std::cout << "Falling back to single-event counting only for this run.\n";
            outFile << "  WARNING: Parameterization file not found. Using counting only.\n";
            useParameterization = false;
        }
    }

    // Keep x/y in local mode and map to 1/2 naming in Guru mode.
    const std::vector<std::pair<HistVariable, std::string>> histConfigs = {
        {HistVariable(HistVariable::VariableType::InvariantMass, "", false, true), "_x"},
        {HistVariable(HistVariable::VariableType::InvariantMass, "", true, false), "_y"}
    };

    // These vectors hold the best sensitivity per mass for final summary plots.
    std::vector<double> massPoints;
    std::vector<double> bestSensitivityHist;
    std::vector<douDCble>DC bestSensitivityFit;
    std::vector<std::string> bestChannelHist;
    std::vector<std::string> bestChannelFit;

    outFile << "\n================================================================\n";
    outFile << "DETAILED RESULTS BY MASS AND CHANNEL\n";
    outFile << "================================================================\n";

    // Iterate over target masses and compute channel-level sensitivities.
    for (size_t massIdx = 0; massIdx < TEST_MASSES.size(); ++massIdx)
    {
        // Current scan mass used to define signal window and process name.
        const int testMass = TEST_MASSES[massIdx];
        // Keep x-axis value for later plotting.
        massPoints.push_back(testMass);
        // Initialize best values for this mass; they will be updated by comparisons.
        bestSensitivityHist.push_back(0.0);
        bestSensitivityFit.push_back(0.0);
        bestChannelHist.push_back("none");
        bestChannelFit.push_back("none");

        outFile << "\n================================================================\n";
        outFile << "MASS: " << testMass << " GeV\n";
        outFile << "================================================================\n";

        if (!useGuruInput)
        {
            // Local mode: pull histograms through Process::getHist from framework objects.
            for (const auto &channelContext : selectedLocalChannels)
            {
                const std::string &channelName = channelContext.channelName;
                for (const auto &histConfig : histConfigs)
                {
                    // HistVariable controls which projection histogram is requested.
                    const HistVariable &histVar = histConfig.first;
                    // Legacy suffix identifies x/y projection branch.
                    const std::string &legacySuffix = histConfig.second;
                    // Human-readable/report suffix adjusted to current input flavor.
                    const std::string suffix = projectionSuffix(legacySuffix, false);
                    // X/Y label is used for parameterization function-key lookup.
                    const std::string projection = parameterizationProjectionLabel(legacySuffix);

                    // Build expected signal process name convention for this channel/mass.
                    const std::string signalName = "Higgs signal " + channelName + " " + std::to_string(testMass);
                    // Lookup signal process in pre-built map.
                    auto signalIt = channelContext.processByName.find(signalName);
                    if (signalIt == channelContext.processByName.end())
                        continue;

                    // Extract process pointer once and apply safety guard.
                    const auto &signalProcess = signalIt->second;
                    if (!signalProcess || isDataLikeProcessName(signalProcess->getName()))
                        continue;

                    // Pull signal histogram with expected-event scaling enabled.
                    TH1 *signalHist = signalProcess->getHist(histVar, true);
                    if (!signalHist || signalHist->GetEntries() == 0)
                        continue;

                    // Compute fixed counting window around current mass hypothesis.
                    const double lowEdge = testMass - WINDOW_HALFWIDTH;
                    const double highEdge = testMass + WINDOW_HALFWIDTH;
                    // Convert physics window edges to ROOT bin indices.
                    const int binLow = signalHist->FindBin(lowEdge);
                    const int binHigh = signalHist->FindBin(highEdge);
                    // Keep total signal for fractional diagnostics in the output report.
                    const double totalSignal = signalHist->Integral("width");
                    // Integrate signal in the target window only.
                    const double signalInWindow = signalHist->Integral(binLow, binHigh, "width");
                    if (signalInWindow == 0)
                        continue;

                    // Histogram-based background accounting per named process group.
                    std::vector<double> bgAmountsHist(BACKGROUNDS.size(), 0.0);
                    double totalBgHist = 0.0;
                    for (size_t i = 0; i < BACKGROUNDS.size(); ++i)
                    {
                        const auto &bgProcess = channelContext.backgroundProcesses[i];
                        // Skip missing backgrounds and anything that looks like data.
                        if (!bgProcess || isDataLikeProcessName(bgProcess->getName()))
                            continue;

                        // Retrieve background histogram in the same projection as signal.
                        TH1 *bgHist = bgProcess->getHist(histVar, true);
                        if (!bgHist || bgHist->GetEntries() == 0)
                            continue;

                        // Integrate background in the same window as signal.
                        const int bgBinLow = bgHist->FindBin(lowEdge);
                        const int bgBinHigh = bgHist->FindBin(highEdge);
                        bgAmountsHist[i] = bgHist->Integral(bgBinLow, bgBinHigh, "width");
                        // Accumulate total background for significance denominator.
                        totalBgHist += bgAmountsHist[i];
                    }

                    // Parameterized-background path exists only in local mode.
                    std::vector<double> bgAmountsFit(BACKGROUNDS.size(), 0.0);
                    double totalBgFit = 0.0;
                    if (useParameterization)
                    {
                        // Cache function map reference to avoid repeated map retrieval.
                        auto &functionMap = backgroundFunctions.getFunctions();
                        for (size_t i = 0; i < BACKGROUNDS.size(); ++i)
                        {
                            // Build exact key used when fit functions were stored.
                            std::string functionName = BACKGROUNDS[i].first + "->" + channelName +
                                                       "/Reco Invariant Mass Background " + projection + " Projection Nominal";
                            auto it = functionMap.find(functionName);
                            if (it != functionMap.end())
                            {
                                TF1 *tf1 = it->second.getFunction();
                                if (tf1)
                                {
                                    // Integrate fitted function over same mass window.
                                    bgAmountsFit[i] = tf1->Integral(lowEdge, highEdge);
                                    totalBgFit += bgAmountsFit[i];
                                }
                            }
                        }
                    }

                    // Core sensitivity metric currently used by this analysis note chain.
                    const double sensitivityHist = signalInWindow / std::sqrt(totalBgHist + 2.5);
                    // Keep fit-based sensitivity separate; only valid when enabled.
                    double sensitivityFit = 0.0;
                    if (useParameterization)
                        sensitivityFit = signalInWindow / std::sqrt(totalBgFit + 2.5);

                    // Build channel label that includes projection suffix.
                    const std::string fullChannelName = channelName + suffix;
                    // Update best counting channel if this channel beats previous best.
                    if (sensitivityHist > bestSensitivityHist[massIdx])
                    {
                        bestSensitivityHist[massIdx] = sensitivityHist;
                        bestChannelHist[massIdx] = fullChannelName;
                    }
                    // Update best fit channel similarly when fit mode is active.
                    if (useParameterization && sensitivityFit > bestSensitivityFit[massIdx])
                    {
                        bestSensitivityFit[massIdx] = sensitivityFit;
                        bestChannelFit[massIdx] = fullChannelName;
                    }

                    // Channel-level report block begins.
                    outFile << "\n";
                    outFile << "Channel: " << fullChannelName << "\n";
                    outFile << "  Window: " << testMass << " +/- " << WINDOW_HALFWIDTH
                            << " GeV  (" << lowEdge << " - " << highEdge << ")\n";
                    outFile << "  H++:                 " << signalInWindow
                            << "  (" << (totalSignal > 0 ? signalInWindow / totalSignal * 100 : 0)
                            << "% of total " << totalSignal << ")\n";

                    // Helper writes aligned background rows for either method block.
                    auto writeBackgrounds = [&](const std::vector<double> &amounts)
                    {
                        for (size_t i = 0; i < BACKGROUNDS.size(); ++i)
                        {
                            outFile << "  " << BACKGROUNDS[i].second << ":";
                            // Pad labels so values line up in monospaced text output.
                            int pad = 20 - (int)BACKGROUNDS[i].second.length();
                            for (int p = 0; p < pad; ++p)
                                outFile << ' ';
                            outFile << amounts[i] << "\n";
                        }
                    };

                    outFile << "  [Single Event Counting]\n";
                    writeBackgrounds(bgAmountsHist);
                    outFile << "  Total background:    " << totalBgHist << "\n";
                    outFile << "  s/sqrt(b+2.5):       " << sensitivityHist << "\n";

                    if (useParameterization)
                    {
                        // Optional parameterization section mirrors counting section format.
                        outFile << "  [Parameterization]\n";
                        writeBackgrounds(bgAmountsFit);
                        outFile << "  Total background:    " << totalBgFit << "\n";
                        outFile << "  s/sqrt(b+2.5):       " << sensitivityFit << "\n";
                    }
                }
            }
        }
        else
        {
            // Guru mode: read histograms directly from EOS ROOT files, not local getHist.
            if (selectedGuruChannels.empty())
            {
                outFile << "No Guru channels discovered under " << GURU_INPUT_DIR << "\n";
            }

            // Iterate discovered channel directories in Guru files.
            for (const std::string &channelName : selectedGuruChannels)
            {
                // Evaluate both projection definitions (prefer mH1/mH2; fall back to h_mll1/h_mll2).
                for (const auto &histConfig : histConfigs)
                {
                    const std::string &legacySuffix = histConfig.second;
                    const std::string suffix = projectionSuffix(legacySuffix, true);
                    // Convert local x/y projection token to Guru histogram object name.
                    const std::vector<std::string> histNameCandidates = guruHistogramNameCandidates(legacySuffix);

                    // Load and year-sum the signal histogram for this mass/channel/projection.
                    TH1 *signalHist = loadGuruSignalHistogram(testMass, channelName, histNameCandidates);
                    if (!signalHist || signalHist->GetEntries() == 0)
                    {
                        delete signalHist;
                        continue;
                    }

                    // Build counting window exactly as in local mode for output consistency.
                    const double lowEdge = testMass - WINDOW_HALFWIDTH;
                    const double highEdge = testMass + WINDOW_HALFWIDTH;
                    const int binLow = signalHist->FindBin(lowEdge);
                    const int binHigh = signalHist->FindBin(highEdge);
                    const double totalSignal = signalHist->Integral("width");
                    // Integrate signal in window and skip empty contributions.
                    const double signalInWindow = signalHist->Integral(binLow, binHigh, "width");
                    if (signalInWindow == 0)
                    {
                        delete signalHist;
                        continue;
                    }

                    // Sum each background group by aggregating configured file base names.
                    std::vector<double> bgAmountsHist(BACKGROUNDS.size(), 0.0);
                    double totalBgHist = 0.0;
                    for (size_t i = 0; i < BACKGROUNDS.size(); ++i)
                    {
                        // Resolve Guru file list for this background label.
                        const auto bgIt = GURU_BACKGROUND_FILE_BASES.find(BACKGROUNDS[i].first);
                        if (bgIt == GURU_BACKGROUND_FILE_BASES.end())
                            continue;

                        // Load and year-sum background histogram for this channel/projection.
                        TH1 *bgHist = combineGuruHistograms(bgIt->second, channelName, histNameCandidates);
                        if (!bgHist || bgHist->GetEntries() == 0)
                        {
                            delete bgHist;
                            continue;
                        }

                        // Integrate background in same window and then release memory.
                        const int bgBinLow = bgHist->FindBin(lowEdge);
                        const int bgBinHigh = bgHist->FindBin(highEdge);
                        bgAmountsHist[i] = bgHist->Integral(bgBinLow, bgBinHigh, "width");
                        totalBgHist += bgAmountsHist[i];
                        delete bgHist;
                    }

                    // Guru mode always reports counting metric only.
                    const double sensitivityHist = signalInWindow / std::sqrt(totalBgHist + 2.5);
                    const std::string fullChannelName = channelName + suffix;
                    // Keep per-mass best-channel tracker updated for summary/plots.
                    if (sensitivityHist > bestSensitivityHist[massIdx])
                    {
                        bestSensitivityHist[massIdx] = sensitivityHist;
                        bestChannelHist[massIdx] = fullChannelName;
                    }

                    // Write channel-level Guru results in same textual style as local mode.
                    outFile << "\n";
                    outFile << "Channel: " << fullChannelName << "\n";
                    outFile << "  Window: " << testMass << " +/- " << WINDOW_HALFWIDTH
                            << " GeV  (" << lowEdge << " - " << highEdge << ")\n";
                    outFile << "  H++:                 " << signalInWindow
                            << "  (" << (totalSignal > 0 ? signalInWindow / totalSignal * 100 : 0)
                            << "% of total " << totalSignal << ")\n";

                    for (size_t i = 0; i < BACKGROUNDS.size(); ++i)
                    {
                        outFile << "  " << BACKGROUNDS[i].second << ":";
                        int pad = 20 - (int)BACKGROUNDS[i].second.length();
                        for (int p = 0; p < pad; ++p)
                            outFile << ' ';
                        outFile << bgAmountsHist[i] << "\n";
                    }
                    outFile << "  Total background:    " << totalBgHist << "\n";
                    outFile << "  s/sqrt(b+2.5):       " << sensitivityHist << "\n";

                    // Release owned signal histogram now that this block is finished.
                    delete signalHist;
                }
            }
        }
    }

    // Emit best-channel summary for quick scan across masses.
    outFile << "\n================================================================\n";
    outFile << "BEST CHANNEL SUMMARY (per mass)\n";
    outFile << "================================================================\n";
    for (size_t i = 0; i < massPoints.size(); ++i)
    {
        outFile << "Mass " << massPoints[i] << " GeV\n";
        outFile << "  Best [Single Event Counting]: " << bestChannelHist[i]
                << "  (s/sqrt(b+2.5) = " << bestSensitivityHist[i] << ")\n";
        if (useParameterization)
        {
            outFile << "  Best [Parameterization]:      " << bestChannelFit[i]
                    << "  (s/sqrt(b+2.5) = " << bestSensitivityFit[i] << ")\n";
            const double ratio = (bestSensitivityHist[i] > 0.0)
                                     ? (bestSensitivityFit[i] / bestSensitivityHist[i])
                                     : 0.0;
            outFile << "  Ratio [Parameterization/Single Event Counting]: " << ratio << "\n";
        }
        else
        {
            outFile << "  Best [Parameterization]:      DISABLED\n";
            outFile << "  Ratio [Parameterization/Single Event Counting]: N/A\n";
        }
    }

    // Finalize report with completion timestamp.
    outFile << "\n================================================================\n";
    outFile << "END OF ANALYSIS\n";
    outFile << "Analysis completed at: " << getCurrentTimeString() << "\n";
    outFile << "================================================================\n";
    outFile.close();

    // Draw summary sensitivity plot for final presentation.
    gStyle->SetOptStat(0);
    TCanvas *canvas = new TCanvas("cSensitivity", "Poster Sensitivity Summary", 1200, 800);
    canvas->SetLeftMargin(0.12);
    canvas->SetRightMargin(0.05);
    canvas->SetBottomMargin(0.12);
    canvas->SetTopMargin(0.16);
    canvas->SetLogy();
    canvas->SetGridx(true);
    canvas->SetGridy(true);

    // Initialize graph objects from per-mass best sensitivity vectors.
    TGraph *gHist = new TGraph((int)massPoints.size(), massPoints.data(), bestSensitivityHist.data());
    TGraph *gFit = nullptr;
    if (useParameterization)
        gFit = new TGraph((int)massPoints.size(), massPoints.data(), bestSensitivityFit.data());

    // Determine dynamic y-axis maximum based on available sensitivities.
    double maxY = 0.0;
    for (size_t i = 0; i < bestSensitivityHist.size(); ++i)
    {
        maxY = std::max(maxY, bestSensitivityHist[i]);
        if (useParameterization)
            maxY = std::max(maxY, bestSensitivityFit[i]);
    }
    if (maxY <= 0.0)
        maxY = 1.0;

    // Apply visual style choices for counting curve.
    const int tealColor = TColor::GetColor("#008080");
    const int redColor = TColor::GetColor("#C62828");
    gHist->SetTitle("H^{#pm#pm} Sensitivity Summary;Test Mass [GeV];Best s/#sqrt{b+2.5} across channels");
    gHist->SetLineColor(tealColor);
    gHist->SetMarkerColor(tealColor);
    gHist->SetMarkerStyle(20);
    gHist->SetMarkerSize(1.2);
    gHist->SetLineWidth(3);
    gHist->SetMinimum(1e-6);
    gHist->SetMaximum(maxY * 1.35);
    gHist->Draw("ALP");

    // Draw fit curve only in local runs with parameterization available.
    if (useParameterization)
    {
        gFit->SetLineColor(redColor);
        gFit->SetMarkerColor(redColor);
        gFit->SetMarkerStyle(24);
        gFit->SetMarkerSize(1.2);
        gFit->SetLineWidth(3);
        gFit->SetLineStyle(2);
        gFit->Draw("LP SAME");
    }

    // Add plot labels/legend-like text annotations.
    TLatex label;
    label.SetNDC(true);
    label.SetTextSize(0.035);
    label.DrawLatex(0.13, 0.92, "CMSAnalysis Internal");
    label.SetTextSize(0.028);
    const std::string windowText = "Window = #pm " + std::to_string((int)WINDOW_HALFWIDTH) + " GeV";
    label.DrawLatex(0.13, 0.875, windowText.c_str());
    label.SetTextColor(tealColor);
    label.DrawLatex(0.44, 0.875, "Solid teal: Single Event Counting");
    if (useParameterization)
    {
        label.SetTextColor(redColor);
        label.DrawLatex(0.73, 0.875, "Dashed red: Parameterization");
    }
    else
    {
        label.SetTextColor(kBlack);
        label.DrawLatex(0.73, 0.875, "Counting only");
    }
    label.SetTextColor(kBlack);

    // Persist plot files, replacing prior outputs if present.
    canvas->Modified();
    canvas->Update();
    gSystem->Unlink(PLOT_PNG_FILE.c_str());
    gSystem->Unlink(PLOT_PDF_FILE.c_str());
    canvas->SaveAs(PLOT_PNG_FILE.c_str());
    canvas->Print(PLOT_PDF_FILE.c_str(), "pdf");

    // Release plot heap objects.
    if (gFit)
        delete gFit;
    delete gHist;
    delete canvas;

    // Ratio plot is produced only when both counting and fit metrics exist.
    if (useParameterization)
    {
        // Fill parameterization/counting ratio vector per mass point.
        std::vector<double> sensitivityRatio(massPoints.size(), 0.0);
        double ratioMin = 1e9;
        double ratioMax = -1e9;
        for (size_t i = 0; i < massPoints.size(); ++i)
        {
            if (bestSensitivityHist[i] > 0.0)
                sensitivityRatio[i] = bestSensitivityFit[i] / bestSensitivityHist[i];

            if (sensitivityRatio[i] > 0.0)
            {
                ratioMin = std::min(ratioMin, sensitivityRatio[i]);
                ratioMax = std::max(ratioMax, sensitivityRatio[i]);
            }
        }

        if (ratioMin > ratioMax)
        {
            ratioMin = 0.95;
            ratioMax = 1.05;
        }
        // Expand ratio axis range slightly for readability.
        const double pad = 0.1 * (ratioMax - ratioMin + 1e-6);
        ratioMin -= pad;
        ratioMax += pad;

        // Configure and draw ratio graph.
        TCanvas *ratioCanvas = new TCanvas("cRatio", "Poster Sensitivity Ratio", 1200, 800);
        ratioCanvas->SetLeftMargin(0.16);
        ratioCanvas->SetRightMargin(0.05);
        ratioCanvas->SetBottomMargin(0.12);
        ratioCanvas->SetTopMargin(0.16);
        ratioCanvas->SetGridx(true);
        ratioCanvas->SetGridy(true);

        TGraph *gRatio = new TGraph((int)massPoints.size(), massPoints.data(), sensitivityRatio.data());
        gRatio->SetTitle("Method Comparison;Test Mass [GeV];Sensitivity Ratio (Param. / Counting)");
        gRatio->SetLineColor(kAzure + 2);
        gRatio->SetMarkerColor(kAzure + 2);
        gRatio->SetMarkerStyle(20);
        gRatio->SetMarkerSize(1.2);
        gRatio->SetLineWidth(3);
        gRatio->SetMinimum(ratioMin);
        gRatio->SetMaximum(ratioMax);
        gRatio->Draw("ALP");

        // Draw horizontal unity reference line (ratio = 1).
        TLine *unityLine = new TLine(massPoints.front(), 1.0, massPoints.back(), 1.0);
        unityLine->SetLineColor(kOrange + 7);
        unityLine->SetLineStyle(2);
        unityLine->SetLineWidth(2);
        unityLine->Draw();

        TLatex ratioLabel;
        ratioLabel.SetNDC(true);
        ratioLabel.SetTextSize(0.035);
        ratioLabel.DrawLatex(0.13, 0.92, "CMSAnalysis Internal");
        ratioLabel.SetTextSize(0.028);
        ratioLabel.DrawLatex(0.13, 0.875, windowText.c_str());

        // Save ratio outputs and free resources.
        ratioCanvas->Modified();
        ratioCanvas->Update();
        gSystem->Unlink(RATIO_PLOT_PNG_FILE.c_str());
        gSystem->Unlink(RATIO_PLOT_PDF_FILE.c_str());
        ratioCanvas->SaveAs(RATIO_PLOT_PNG_FILE.c_str());
        ratioCanvas->Print(RATIO_PLOT_PDF_FILE.c_str(), "pdf");

        delete unityLine;
        delete gRatio;
        delete ratioCanvas;
    }
    else
    {
        // Ensure stale ratio files are removed in counting-only runs.
        gSystem->Unlink(RATIO_PLOT_PNG_FILE.c_str());
        gSystem->Unlink(RATIO_PLOT_PDF_FILE.c_str());
    }

    // Final console status lines for run bookkeeping.
    std::cout << "Analysis complete. Results written to " << OUTPUT_FILE << "\n";
    std::cout << "Poster plot written to " << PLOT_PNG_FILE << " and " << PLOT_PDF_FILE << "\n";
    if (useParameterization)
        std::cout << "Ratio plot written to " << RATIO_PLOT_PNG_FILE << " and " << RATIO_PLOT_PDF_FILE << "\n";

    // Cleanup hook is intentionally conservative and currently a no-op list.
    performCleanup();
}

// No-argument entry point used by: root -b -q 'WindowAnalysis.C+'
void WindowAnalysis()
{
    WindowAnalysisImpl(-1);
}

// Integer-argument entry point used by: root -b -q 'WindowAnalysis.C+(1)'
// 0=Local, 1=Guru, 2=AutoDetect
void WindowAnalysis(int mode)
{
    WindowAnalysisImpl(mode);
}

// String-argument entry point used by:
//   root -b -q 'WindowAnalysis.C+("local")'
//   root -b -q 'WindowAnalysis.C+("guru")'
//   root -b -q 'WindowAnalysis.C+("auto")'
void WindowAnalysis(const char *mode)
{
    if (!mode)
    {
        WindowAnalysisImpl(0);
        return;
    }
    WindowAnalysisImpl(modeCodeFromString(std::string(mode)));
}

#include "CMSAnalysis/DataCollection/interface/Analyzer.hh"
#include "CMSAnalysis/DataCollection/interface/EventLoader.hh"
#include "CMSAnalysis/Modules/interface/AnalyzerEventInput.hh"
#include "CMSAnalysis/Modules/interface/Module.hh"
#include "CMSAnalysis/Plans/interface/ModuleCollection.hh"
#include "CMSAnalysis/Utility/interface/TDisplayText.h"
#include "TFile.h"
#include "TH1.h"
#include <RtypesCore.h>
#include <algorithm>
#include <iostream>
#include <string>

Analyzer::Analyzer() : eventInterface(nullptr), input(new AnalyzerEventInput(&eventInterface))
{
}

Analyzer::Analyzer(const Analyzer &analyzer) : eventInterface(nullptr), input(analyzer.input)
{
}

Analyzer::~Analyzer()
{
    delete input;
}

void Analyzer::writeOutputFile()
{
    std::map<std::string, TDirectory *> filterDirectories;

    // Create the output file
    outputRootFile->cd();
    // Finalize the modules
    for (const auto &module : productionModules)
    {
        module->finalizeEvent();
    }
    for (const auto &module : filterModules)
    {
        module->finalizeEvent();
    }

    // Finalize separately for each filterString, to be safe
    // std::cout << "There are " << analysisModules.size() << " analysis modules\n";
    for (const auto &module : analysisModules)
    {
        // Write the output
        module->doneProcessing();
        std::cout << "FilterModules size : " << filterModules.size() << "\n";
        std::cout << "Finalizing analysis module: " << module->getName() << "\n";
        module->finalizeEvent();
        module->writeObjects(outputRootFile);
    }

    for (const auto &module : getAllModules())
    {
        std::cout << "Time taken by " << module->getName() << ": " << module->getElapsedTime() << " s\n";
    }

    // Write total number of events
    auto *eventsText = new TDisplayText(std::to_string(numOfEvents).c_str());
    eventsText->Write("NEvents");
    // Clean up
    // outputRootFile->Close();
    delete outputRootFile;
}

// gets modules from module collection
void Analyzer::addModules(ModuleCollection modules)
{
    productionModules = modules.getProductionModules();
    filterModules = modules.getFilterModules();
    analysisModules = modules.getAnalysisModules();
}

std::vector<std::shared_ptr<Module>> Analyzer::getAllModules() const
{
    std::vector<std::shared_ptr<Module>> modules;
    for (const auto &mod : productionModules)
    {
        modules.push_back(mod);
    }
    for (const auto &mod : filterModules)
    {
        modules.push_back(mod);
    }
    for (const auto &mod : analysisModules)
    {
        modules.push_back(mod);
    }
    return modules;
}

void Analyzer::initialize(const std::string &outputDirectory,
                          const std::string &outputFile) // am I allowed to change this??
{
    std::string outputPath = outputDirectory + "/" + outputFile;

    // This keeps the histograms separate from the files they came from, avoiding errors
    TH1::AddDirectory(kFALSE);
    TH1::SetDefaultSumw2(kTRUE);

    outputRootFile = new TFile(outputPath.c_str(), "RECREATE"); //<<<<<<<<<<<<<<<

    // Checks if all dependencies are loaded properly
    for (const auto &module : getAllModules())
    {
        if (!checkModuleDependencies(module))
        {
            std::cout << typeid(*module).name() << "'s dependencies have not been loaded properly!\n";
        }
    }

    std::cout << "Finished checking module dependencies\n";

    // Initialize all modules
    for (const auto &module : getAllModules())
    {
        if (!module->getInput())
        {
            module->setInput(input);
        }
        module->initialize(); //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<same initialize?
    }
}

void Analyzer::processOneEvent(const EventInterface *eInterface)
{
    eventInterface = eInterface;
    numOfEvents++;

    // Processes event through production modules
    for (const auto &module : productionModules)
    {
        if (!module->processEvent())
        {
            return;
        }
    }

    auto filterStrings = std::vector<std::string>();
    // Processes event through filter modules
    for (const auto &module : filterModules)
    {
        if (!module->processEvent())
        {
            return;
        }
        filterStrings.push_back(module->getFilterString());
    }

    // Processes event through analysis modules
    for (const auto &module : analysisModules)
    {
        module->setFilterStrings(filterStrings);
        if (!module->processEvent())
        {
            return;
        }
    }
}

void Analyzer::addProductionModule(std::shared_ptr<ProductionModule> module)
{
    productionModules.push_back(std::move(module));
}

void Analyzer::addFilterModule(std::shared_ptr<FilterModule> module)
{
    filterModules.push_back(std::move(module));
}
void Analyzer::addAnalysisModule(std::shared_ptr<AnalysisModule> module)
{
    analysisModules.push_back(std::move(module));
}

bool Analyzer::checkModuleDependencies(std::shared_ptr<Module> module)
{
    auto modules = getAllModules();
    auto dependencies = module->getDependencies();
    for (const auto &modulePtr : dependencies)
    {
        if (std::ranges::find(modules, modulePtr) == modules.end())
        {
            return false;
        }
    }

    return true;
}